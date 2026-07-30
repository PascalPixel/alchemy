typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 3c8 at 0x02002f8c: select a data table from the current scene id.
 *
 * The owner is complete: `push {lr}` at 0x02002f8c and the matching
 * `pop {r1} ; bx r1` at 0x02002fd4..0x02002fd6, followed by a 13-word literal
 * pool (0x02002fd8-0x0200300b) that is branched over and is data, not code.
 * The next prologue begins at 0x0200300c.  No live frame or register state
 * escapes the row and the owner contains no calls.
 *
 * `pop {r1} ; bx r1` (N != 0) leaves r0 alive, and every path sets r0 to one
 * of the pooled table addresses, so the owner returns that address.
 *
 * This is the same construct as the byte-exact sibling 0x02000e04, which
 * reads `Data_02000240[224]` - the scene id halfword at 0x02000240 + 448 -
 * and returns one of six in-image tables.  The selector is read with `ldrsh`,
 * so the comparison is signed.
 *
 * The 0xb6 case exists in the original as its own `beq` into the default
 * block: it selects the same table as the fallthrough.  It is spelled
 * explicitly here because the branch is present in the code.
 *
 * The byte-exact sibling spells the six selector constants as the addresses
 * of link-time `Value_000000b5`.. symbols, which is a code-generation device
 * for placing them in the literal pool.  This lane spells them as the plain
 * integers they compare against.
 *
 * Link base: the returned words 0x0200ee44..0x0200f4f8 are in-image data
 * addresses.  Three independent witnesses put this overlay's link base at
 * 0x02008000 (the installed handler words 0x020086a1, 0x0200896d and
 * 0x02008b99 in the byte-exact siblings 0x02000754, 0x020009c8 and
 * 0x02000c5c are exactly the local entry points 0x020006a0, 0x0200096c and
 * 0x02000b98 plus the Thumb bit), so these tables live at file offsets
 * 0x6e44..0x74f8.  The raw pool value is kept as the symbol name, matching
 * the byte-exact sources' `Data_0200dd68`-style spelling.
 */

extern s16 Data_02000240[];

extern u8 Data_0200ee44[];
extern u8 Data_0200ef1c[];
extern u8 Data_0200f120[];
extern u8 Data_0200f300[];
extern u8 Data_0200f3b4[];
extern u8 Data_0200f4f8[];

s32 Func_02002f8c(void)
{
    s16 scene = Data_02000240[224];

    if (scene == 0xb5) {
        return (s32)Data_0200ee44;
    }
    if (scene == 0xb6) {
        return (s32)Data_0200ef1c;
    }
    if (scene == 0xb7) {
        return (s32)Data_0200f120;
    }
    if (scene == 0xb8) {
        return (s32)Data_0200f300;
    }
    if (scene == 0xb9) {
        return (s32)Data_0200f3b4;
    }
    if (scene == 0xba) {
        return (s32)Data_0200f4f8;
    }
    return (s32)Data_0200ef1c;
}
