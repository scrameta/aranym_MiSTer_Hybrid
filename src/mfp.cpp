/* Joy 2001 */

#include "sysdeps.h"
#include "hardware.h"
#include "cpu_emulation.h"
#include "memory.h"
#include "mfp.h"

static bool dP = false;		/* debug print */

static const int HW = 0xfffa00;

	MFP_Timer::MFP_Timer(int value) {
		name = 'A' + value;
		control = start_data = current_data = 0;
		state = false;
	}

	bool MFP_Timer::isRunning() {
		return ((control & 0x0f) > 0);
	}

	void MFP_Timer::setControl(uae_u8 value) {
		control = value & 0x0f;
		if (value & 0x10)
			state = false;
		if (dP)
			fprintf(stderr, "Set MFP Timer%c control to $%x\n", name, value);
	}

	uae_u8 MFP_Timer::getControl() {
		return control | (state << 5);
	}

	void MFP_Timer::setData(uae_u8 value) {
		if (dP)
			fprintf(stderr, "Set MFP Timer%c data to %d\n", name, value);
		start_data = value;
		if (! isRunning())
			current_data = value;
	}

	void MFP_Timer::tick() {
	// fprintf(stderr, "tick for Timer%c\n", name);
		if (isRunning()) {
			// if (current_data > 1)
			if (current_data == start_data && current_data > 1)
				current_data--;
			else {
				state = true;
				// Trigger 200Hz interrupt
				TriggerMFP(5);
				if (dP)
					fprintf(stderr, "TriggerMFP($4BA) = %ld\n", ReadMacInt32(0x4ba));
				current_data = start_data;
			}
		}
	}

	uae_u8 MFP_Timer::getData() {
		// fprintf(stderr, "get MFP Timer%c data = %d\n", name, current_data);

		if (isRunning() && current_data > 2)
			current_data--;		// hack to overcome microseconds delays in TOS (e.g. at $E02570)

		return current_data;
	}

/*************************************************************************/

	MFP::MFP() {}

	uae_u8 MFP::handleRead(uaecptr addr) {
		addr -= HW;
		if (addr < 0 || addr > 0x2f)
			return 0;	// unhandled

		switch(addr) {
			case 0x01:	return GPIP_data & ~ 0x20;
						break;

			case 0x03:	return active_edge;
						break;

			case 0x05:	return data_direction;
						break;

			case 0x07:	return irq_enable >> 8;
						break;

			case 0x09:	if (dP) fprintf(stderr, "Read: TimerC IRQ %sabled\n", (irq_enable & 0x20) ? "en" : "dis");
						return irq_enable;
						break;

			case 0x0b:	return 0x20; //(irq_pending >> 8) | (tA->getControl() & 0x10);	// finish
						break;

			case 0x0d:	if (dP) fprintf(stderr, "Read: TimerC IRQ %s pending\n", (irq_pending & 0x20) ? "" : "NOT");
						return irq_pending;
						break;

			case 0xf:	return irq_inservice >> 8;
						break;

			case 0x11:	if (dP) fprintf(stderr, "Read: TimerC IRQ %s in-service\n", (irq_inservice & 0x20) ? "" : "NOT");
						return irq_inservice;
						break;
						
			case 0x13:	return irq_mask >> 8;
						break;
						
			case 0x15:	if (dP) fprintf(stderr, "Read: TimerC IRQ %s masked\n", (irq_mask & 0x20) ? "" : "NOT");
						return irq_mask;
						break;
						
			case 0x17:	return automaticServiceEnd ? 0x48 : 0x40;
						break;

			case 0x19:	return A.getControl();
						break;

			case 0x1b:	return B.getControl();
						break;

			case 0x1d:	return (C.getControl() << 4) | D.getControl();
						break;

			case 0x1f:	return A.getData();
						break;

			case 0x21:	return B.getData();
						break;

			case 0x23:	return C.getData();
						break;
						
			case 0x25:	return D.getData();
						break;
						
			case 0x27:
			case 0x29:
			case 0x2b:
			case 0x2d:
			case 0x2f:
			default:
				return 0;
		};
	}

	void MFP::handleWrite(uaecptr addr, uae_u8 value) {
		addr -= HW;
		if (addr < 0 || addr > 0x2f)
			return;	// unhandled

		switch(addr) {
			case 0x01:	GPIP_data = value;
						break;

			case 0x03:	active_edge = value;
						break;

			case 0x05:	data_direction = value;
						break;

			case 0x07:	irq_enable = (irq_enable & 0x00ff) | (value << 8);
						break;

			case 0x09:	if ((irq_enable ^ value) & 0x20)
							fprintf(stderr, "Write: TimerC IRQ %sabled\n", (value & 20) ? "en" : "dis");
						irq_enable = (irq_enable & 0xff00) | value;
						break;

			case 0x0b:	irq_pending = (irq_pending & 0x00ff) | (value << 8);
						break;

			case 0x0d:	if ((irq_pending ^ value) & 0x20)
							fprintf(stderr, "Write: TimerC IRQ %s pending\n", (value & 20) ? "" : "NOT");
						irq_pending = (irq_pending & 0xff00) | value;
						break;

			case 0xf:	irq_inservice = (irq_inservice & 0x00ff) | (value << 8);
						break;

			case 0x11:	if (dP && (irq_inservice ^ value) & 0x20)
							fprintf(stderr, "Write: TimerC IRQ %s in-service at %08x\n", (value & 20) ? "" : "NOT", showPC());
						irq_inservice = (irq_inservice & 0xff00) | (irq_inservice & value);
						break;
						
			case 0x13:	irq_mask = (irq_mask & 0x00ff) | (value << 8);
						break;
						
			case 0x15:	if (dP && (irq_mask ^ value) & 0x20)
							fprintf(stderr, "Write: TimerC IRQ %s masked\n", (value & 20) ? "" : "NOT");
						irq_mask = (irq_mask & 0xff00) | value;
						break;
						
			case 0x17:	automaticServiceEnd = (value & 0x08) ? true : false;
						if (dP) fprintf(stderr, "MFP autoServiceEnd: %s\n", automaticServiceEnd ? "YES" : "NO");
						break;

			case 0x19:	A.setControl(value);
						break;

			case 0x1b:	B.setControl(value);
						break;

			case 0x1d:	C.setControl(value >> 4);
						D.setControl(value & 0x0f);
						break;

			case 0x1f:	A.setData(value);
						break;

			case 0x21:	B.setData(value);
						break;

			case 0x23:	C.setData(value);
						break;
						
			case 0x25:	D.setData(value);
						break;
						
			case 0x27:
			case 0x29:
			case 0x2b:
			case 0x2d:
			case 0x2f:
			default:
				break;
		};
	}

void MFP::IRQ(int no) {
	switch(no) {
		case 0:	break;	// BUSY
		case 5: C.tick(); break;	// TimerC 200 Hz
		case 6: {
					GPIP_data &= ~0x10;
					// irq_inservice |= 0x40;
					TriggerMFP(6);
				}
				break;
	}
}
