typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 3c7 script selector at 0x02000084.
 *
 * Complete owner: `push {lr}` at 0x02000084 through the interworking return
 * `pop {r1} / bx r1` at 0x020000ac.  Because the popped return address lands
 * in r1 (not r0), r0 survives the epilogue and IS the result (HANDOVER §0).
 * The 24 bytes from 0x020000b0 to 0x020000c7 are this owner's literal pool;
 * the body reaches its return without ever executing them.
 *
 * Link base: this overlay is linked at 0x02008000, as everywhere else in the
 * project.  The three returned pool words 0x02009974 / 0x0200989c /
 * 0x02009734 are therefore in-image data at file offsets 0x1974 / 0x189c /
 * 0x1734, and all three are even, i.e. data addresses rather than Thumb
 * function entries (the one-bit parity test in HANDOVER §0).
 *
 * The scene-state block at 0x02000240 is the cross-overlay idiom: the signed
 * halfword at byte offset 448 (index 224) is the scene/progress id, computed
 * here as `movs r1,#224 / lsls r1,#1` and read with `ldrsh`.
 *
 * Uncertainty: Func_080770c0 is an import veneer resolved by
 * `overlay_call_targets.ts`; it is called with a single id (0x9a7) and its
 * result is only tested against zero, so it is spelled as a predicate.  Which
 * flag file 0x9a7 names is not established here.
 */

extern s16 Data_02000240[];

/* Three animation/dialogue scripts embedded in this overlay's own data. */
extern u8 Data_02009974[];
extern u8 Data_0200989c[];
extern u8 Data_02009734[];

/* Import veneer.  Old-style declaration: used in a condition, so the return
 * type must be given, but the arity is left open (HANDOVER §0). */
s32 Func_080770c0();

u8 *Func_02000084(void)
{
    if (Data_02000240[224] != 0xb4) {
        return Data_02009734;
    }
    if (Func_080770c0(0x9a7) != 0) {
        return Data_02009974;
    }
    return Data_0200989c;
}
