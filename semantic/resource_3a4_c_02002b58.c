typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_3a4 owner at 0x02002b58, 88 bytes: on one value of a packed
 * status word, spawn a presentation record from a three-word coordinate
 * block, stamp its timing fields, and hand it to Func_08009080 -- AND
 * INSTALL 0x02002a48 INTO IT AS A PLAIN WORD AT +108.  A PUBLISHED
 * CALLBACK itself (sweep B), and a publisher of another one, so this
 * single row sits on both ends of the mechanism the 2026-08-01 closure
 * standard exists to catch.
 *
 * Complete owner: `push {r5, lr}` at 0x02002b58 through the
 * interworking return `pop {r5} / pop {r0} / bx r0` at 0x02002b9c-
 * 0x02002ba0, one alignment halfword, and the three-word literal pool
 * 0x02002ba4-0x02002baf; the next owner begins at 0x02002bb0 and is
 * already drafted as resource_3a4_c_02002bb0.c.  measureSpan reports
 * 74; the pool makes it 88.  No arguments, void.
 *
 * THE INSTALLED POINTER: pool word 0x02002bac is 0x0200aa49, which is
 * image offset 0x2a48 with the Thumb bit -- resource_3a4's own
 * 0x02002a48, itself unowned residue on this sweep.  It is stored into
 * the new record at +108 and never called here.  That is precisely the
 * shape a BL sweep cannot see, and it is why 0x02002a48 shows up in
 * sweep B with no caller: this row is its caller, by data.
 *
 * Three calls, all resolved by targetOffset: Func_080090c8 (which
 * RETURNS the record, and may return zero -- the row guards on it),
 * Func_02002a10 in this same overlay, and Func_08009080.
 * Func_02002a10's drafted signature takes a record pointer, and r0 at
 * that call site still holds the record Func_080090c8 returned, so the
 * argument is supplied without being re-materialised; the existing
 * draft and this call site corroborate each other.
 *
 * Uncertainty: the packed status word at 0x03001e40 is masked with 3
 * here, with 7 at 0x02002ce0 and with bit 1 at 0x02002ffc -- a shared
 * field across this overlay's callbacks, observed rather than proven.
 * Data_0200d240 is the three-word coordinate block the existing files
 * already declare.  222, 30, 1, 20 and 1 are literal; record
 * fields +100, +102, +104 and +108 are recorded by offset with roles
 * open, though +108 is demonstrably a callback slot.
 */

extern s32 Data_03001e40;
extern s32 Data_0200d240[3];   /* image offset 0x5240 */

extern u8 *Func_080090c8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_08009080(u8 *record, s32 arg1);
extern void Func_02002a10(u8 *record);

void Func_02002b58(void)
{
    u8 *record;

    if ((Data_03001e40 & 3) != 0) {
        return;
    }
    record = Func_080090c8(222, Data_0200d240[0], Data_0200d240[1], Data_0200d240[2]);
    if (record == 0) {
        return;
    }
    *(u16 *)(record + 100) = 30;
    *(u16 *)(record + 102) = 1;
    *(s32 *)(record + 104) = 20;
    Func_02002a10(record);
    *(s32 *)(record + 108) = 0x0200aa49;
    Func_08009080(record, 1);
}
