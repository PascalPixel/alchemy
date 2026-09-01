#include "types.h"

/*
 * Interrupt-slot install/remove routine. index selects one of the 14 IRQ
 * dispatch slots (REG_IE bit index, and the same-indexed entry in the
 * interrupt-dispatch table at 0x030000e0); handler is the function
 * installed for that slot, or NULL to restore the slot to the reserved
 * no-op handler and disable the IRQ. vcount is only consumed for index==2
 * (V-Counter IRQ), where it becomes the REG_DISPSTAT V-Count trigger
 * scanline.
 *
 * Evidence, all from games/gs1/asm/0800307c.s's literal pool and
 * already-adopted sibling owners in this repository (no other GS
 * decompilation project consulted):
 *   - 0x04000208/0x04000200/0x04000004 are REG_IME/REG_IE/REG_DISPSTAT,
 *     already named this way in games/gs1/recon/en/main/serial_runtime_family.h
 *     and games/gs1/recon/en/main/0800300c.c.
 *   - 0x030000e0 is the 14-entry interrupt-dispatch table that
 *     Func_0800300c (games/gs1/recon/en/main/0800300c.c) DMAs into IWRAM
 *     from ROM 0x08007320, per games/gs1/recon/en/dossiers.json#main:0800300c.
 *   - 0x08003009 is Func_08003008 (RuntimeDispatch_ReservedNoOp03008,
 *     games/gs1/src/runtime/dispatch/reserved_no_op_f.c) plus the thumb
 *     bit -- the reserved no-op dispatch table already installs as its
 *     default entry per 0800300c.json, and this owner restores it here on
 *     removal.
 *   - This owner is already forward-declared
 *     "void Func_0800307c(s32 index, s32 priority, InterruptHandler
 *     handler)" in serial_runtime_family.h and called from three
 *     already-recovered owners, which is what pins the parameter roles:
 *     games/gs1/recon/en/main/08005d10.c has
 *     "Func_0800307c(7, 0, Func_08006240)" (index 7, second argument
 *     unused so 0, a real handler pointer);
 *     games/gs1/src/link/serial/remove_serial_irq_handlers.c has
 *     "Func_0800307c(7, 0, (InterruptHandler)(handler = 0))" and
 *     "Func_0800307c(handler, 0, 0)" with handler==6 (NULL handler to
 *     remove a slot, second argument still unused so 0); and
 *     games/gs1/src/unidentified/main/field/field_owner_104.c has
 *     "Func_0800307c(2, 136, (InterruptHandler)address)" -- index 2 is
 *     the V-Counter IRQ and 136 is a plausible V-Count scanline value,
 *     which is what identifies the second parameter's V-Count role.
 */

typedef void (*InterruptHandler)(void);

#define REG_IME (*(volatile u16 *)0x04000208)
#define REG_IE (*(volatile u16 *)0x04000200)
#define REG_DISPSTAT (*(volatile u16 *)0x04000004)

extern InterruptHandler Data_030000e0[14];
extern void RuntimeDispatch_ReservedNoOp03008(void);

void Func_0800307c(s32 index, s32 vcount, InterruptHandler handler)
{
    s32 saved_ime;
    s32 mask;
    s32 ie;

    if ((u32)index > 13) {
        return;
    }

    saved_ime = REG_IME;
    REG_IME = 0;

    mask = 1;
    ie = REG_IE;
    mask <<= index;
    ie &= ~mask;
    if (handler != 0) {
        ie |= mask;
    }
    REG_IE = ie;

    if ((u32)index <= 2) {
        s32 bit = 8 << index;
        s32 clear_mask = ~bit;
        s32 dispstat;

        if (index == 2) {
            bit |= vcount << 8;
            clear_mask &= 0xff;
        }

        dispstat = REG_DISPSTAT & clear_mask;
        if (handler != 0) {
            dispstat |= bit;
        }
        REG_DISPSTAT = dispstat;
    }

    if (handler != 0) {
        Data_030000e0[index] = handler;
    } else {
        Data_030000e0[index] = (InterruptHandler)RuntimeDispatch_ReservedNoOp03008;
    }

    REG_IME = saved_ime;
}
