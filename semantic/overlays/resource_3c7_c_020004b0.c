typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 3c7 script selector at 0x020004b0.
 *
 * Complete owner: `push {lr}` at 0x020004b0 through `pop {r1} / bx r1` at
 * 0x020004e6.  r0 is not the popped return address, so it survives and is the
 * result (HANDOVER §0).  0x020004ea is an alignment halfword and
 * 0x020004ec-0x02000507 is this owner's literal pool; neither is executed.
 *
 * This is the two-by-two sibling of 0x02000084: the same scene-id test and the
 * same 0x9a7 predicate, but here the predicate is evaluated on BOTH sides of
 * the scene-id test rather than only the matching one.  The two `bl` sites at
 * 0x020004c6 and 0x020004d8 reach the same import veneer with the same
 * argument, and are written as two calls because they are genuinely two sites
 * on disjoint paths (site count 2, matching `overlay_call_targets.ts`).
 *
 * Link base 0x02008000: the four returned pool words 0x0200a010 / 0x02009eb4 /
 * 0x02009ca4 / 0x02009a94 are in-image data at file offsets 0x2010 / 0x1eb4 /
 * 0x1ca4 / 0x1a94, all even and so data rather than Thumb entries.
 *
 * The scene-state block at 0x02000240 is the cross-overlay idiom; index 224
 * (byte offset 448) is the signed scene/progress id.
 */

extern s16 Data_02000240[];

/* Four scripts embedded in this overlay's own data. */
extern u8 Data_0200a010[];
extern u8 Data_02009eb4[];
extern u8 Data_02009ca4[];
extern u8 Data_02009a94[];

/* Import veneer; used in a condition, so the return type is stated and the
 * arity left open. */
s32 Func_080770c0();

u8 *Func_020004b0(void)
{
    if (Data_02000240[224] == 0xb4) {
        if (Func_080770c0(0x9a7) != 0) {
            return Data_0200a010;
        }
        return Data_02009eb4;
    }
    if (Func_080770c0(0x9a7) != 0) {
        return Data_02009ca4;
    }
    return Data_02009a94;
}
