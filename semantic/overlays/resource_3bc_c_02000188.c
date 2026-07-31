typedef signed int s32;

/*
 * Resource 3bc, owner at 0x02000188.  44 strict bytes: 30 bytes of code
 * (0x0188-0x01a5), a 2-byte alignment `nop`, and a three-word literal pool at
 * 0x01a8-0x01b3.
 *
 * Link base: this overlay is linked at 0x02008000, so a pool word in
 * 0x0200_8xxx..0200_dxxx is an in-image address at `offset = value - 0x8000`.
 * Witnesses used here:
 *   - 0x0200804d = Func_0200004c + 1 (Thumb bit) — the byte-exact sibling
 *     assets/code/resource_3bc_c_020001b4.c passes exactly this word to the
 *     task installer, so it is a code pointer, not data;
 *   - 0x0200d480 / 0x0200d484 are the even (data) words at file offsets
 *     0x5480 / 0x5484 — the overlay's own writable EWRAM state.
 *
 * Epilogue is `pop {r5} / pop {r0} / bx r0`: r0 holds the popped return
 * address, so the owner is void.
 *
 * Two call sites:
 *   0x0200019c -> veneer 0x02004850 -> Func_080000d8 (one argument, r0)
 *   0x020001a0 -> 0x02004bc4, which is this overlay's OWN call_via bank
 *     (0x02004bb0+, `bx rN / nop` pairs in register order r0..r7).  0x4bc4 is
 *     the r5 slot, and r5 still holds 0x0200804d across the preceding call
 *     (r5 is callee-saved), so this is an indirect call to Func_0200004c.
 *     Spelled here as a call through a function pointer built from that word.
 */

typedef void (*TaskEntry)(void);

extern s32 Data_0200d480;
extern s32 Data_0200d484;

void Func_080000d8();

void Func_02000188(void)
{
    TaskEntry entry;

    Data_0200d484 = 0;
    entry = (TaskEntry)0x0200804D;
    Data_0200d480 = 0;

    Func_080000d8(entry);
    entry();
}
