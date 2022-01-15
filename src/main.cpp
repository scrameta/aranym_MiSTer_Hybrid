/*
 * main.cpp - startup/shutdown code
 *
 * Copyright (c) 2001-2008 Petr Stehlik of ARAnyM dev team (see AUTHORS)
 * 
 * Inspired by Christian Bauer's Basilisk II
 *
 * This file is part of the ARAnyM project which builds a new and powerful
 * TOS/FreeMiNT compatible virtual machine running on almost any hardware.
 *
 * ARAnyM is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ARAnyM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ARAnyM; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "sysdeps.h"
#include "cpu_emulation.h"
#include "main.h"
#include "parameters.h"
#include "aranym_exception.h"
#include "disasm-glue.h"

#define DEBUG 0
#include "debug.h"

#ifdef HAVE_NEW_HEADERS
# include <cstdlib>
#else
# include <stdlib.h>
#endif

#ifdef RTC_TIMER
#include <linux/rtc.h>
#include <errno.h>
#endif

//For starting debugger
#ifdef OS_irix
void setactvdebug()
#else
void setactvdebug(int)
#endif
{
#ifdef DEBUGGER
	activate_debugger();
#endif
}

// CPU and FPU type, addressing mode
int CPUType;
bool CPUIs68060;
int FPUType;

// Timer stuff
static uint32 lastTicks = 0;

#if DEBUG
static int early_interrupts = 0;
static int multiple_interrupts = 0;
static int multiple_interrupts2 = 0;
static int multiple_interrupts3 = 0;
static int multiple_interrupts4 = 0;
static int max_mult_interrupts = 0;
static int total_interrupts = 0;
#endif

uint32 InterruptFlags = 0;
SDL_mutex *InterruptFlagLock;

void SetInterruptFlag(uint32 flag)
{
	if (SDL_LockMutex(InterruptFlagLock) == -1) {
		panicbug("Internal error! LockMutex returns -1.");
		abort();
	}
        InterruptFlags |= flag;
	if (SDL_UnlockMutex(InterruptFlagLock) == -1) {
		panicbug("Internal error! UnlockMutex returns -1.");
		abort();
	}
}

void ClearInterruptFlag(uint32 flag)
{
	if (SDL_LockMutex(InterruptFlagLock) == -1) {
		panicbug("Internal error! LockMutex returns -1.");
		abort();
	}
	InterruptFlags &= ~flag;
	if (SDL_UnlockMutex(InterruptFlagLock) == -1) {
		panicbug("Internal error! UnlockMutex returns -1.");
		abort();
	}
}

/*
 *  Initialize everything, returns false on error
 */

bool InitAll(void)
{
#ifndef NOT_MALLOC
	if (ROMBaseHost == NULL) {
		if ((RAMBaseHost = (uint8 *)malloc(RAMSize + ROMSize + HWSize + FastRAMSize)) == NULL) {
			panicbug("Not enough free memory.");
			return false;
		}
		ROMBaseHost = (uint8 *)(RAMBaseHost + ROMBase);
		HWBaseHost = (uint8 *)(RAMBaseHost + HWBase);
		FastRAMBaseHost = (uint8 *)(RAMBaseHost + FastRAMBase);
	}
#endif

	if (!InitMEM())
		return false;

	// For InterruptFlag controling
	InterruptFlagLock = SDL_CreateMutex();

	CPUType = 4;
	FPUType = CPUType == 4 || CPUType == 6 ? CPUType : 1;

#ifdef HAVE_DISASM_M68K
	D(bug("Initializing disassembler..."));
	m68k_disasm_init(&disasm_info, CPU_68040);
#endif

	// Init 680x0 emulation
	if (!Init680x0())
		return false;

#ifdef DEBUGGER
	if (bx_options.startup.debugger && !startupGUI) {
		D(bug("Activate debugger..."));
		activate_debugger();
	}
#endif

	if (RAMBaseHost == NULL)
		return false;

	return true;
}


/*
 *  Deinitialize everything
 */

void ExitAll(void)
{
#ifdef HAVE_DISASM_M68K
	m68k_disasm_exit(&disasm_info);
#endif
}

void RestartAll(bool cold)
{
	lastTicks = 0;

	// memory init should be added here?

	/*
	 * Emulated Atari hardware and virtual hardware provided by NativeFeatures
	 * is initialized by the RESET instruction in the AtariReset() handler
	 * in the aranym_glue.cpp so it doesn't have to be initialized here.
	 * The RESET instruction is at beginning on every operating system (TOS
	 * and EmuTOS for sure, and it is added in our integrated LILO as well)
	 */

	// CPU init
	Restart680x0();
}

/*
vim:ts=4:sw=4:
*/
