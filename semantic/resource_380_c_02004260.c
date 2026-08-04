typedef int s32;
typedef unsigned char u8;

/*
 * resource_380 owner at 0x02004260, 200 bytes: allocate-and-wire for
 * one kind-22 object.  Creates the object, two id-224 companions, and
 * on success binds a resident descriptor (constant 0x0200cbe4), clears
 * two record bytes, masks record flags, seeds fixed-point fields
 * (+40 = 0x28000, +72 = 0x4000), uploads graphics (row 17, source
 * 0x608, +0x400 into the returned slot), and links the companions to
 * the object and the caller's argument.  Returns the first companion.
 *
 * Complete owner: `push {r5,r6,r7,lr}` + r8-sl spill at 0x02004260
 * through `pop {r1} / bx r1` (a genuine s32 return) at 0x02004322,
 * then the one-word literal pool 0x02004324 (0x0200cbe4); next owner's
 * prologue at 0x02004328.
 *
 * CORRECTION (name sweep): this file's callee names came from a naive
 * pc-relative decode and were wrong; they are resolved here through the
 * overlay's import-veneer table under the +2 rule
 * (tools/overlay_call_targets.ts) to their main-ROM identities.  The
 * earlier "one argument for the first companion, (first, 224) for the
 * second, so declared old-style" reading was an artifact: one phantom
 * name covered two different functions (Func_08077040 and
 * Func_08077038).  Nothing here is old-style.
 *
 * Uncertainty: callee roles beyond call shape remain open.  The object
 * layout (+80 record pointer, record +5/+9 flag bytes, +38/+39 clear
 * pair, +28 sprite byte) matches the shapes the other rows of this
 * overlay touch.
 */

extern s32 Func_08000140(s32 row, s32 source);
extern void Func_08000150(s32 row);
extern void Func_080001c8(s32 sprite, s32 arg1, s32 destination);
extern void Func_08009098(s32 object, s32 descriptor);
extern s32 Func_080090c8(s32 kind);
extern void Func_080090d0(s32 object);
extern void Func_08015250(s32 arg0);
extern void Func_08077028(s32 first, s32 arg1);
extern s32 Func_08077038(s32 arg0, s32 arg1);
extern s32 Func_08077040(s32 arg0);
extern void Func_080772b0(s32 first, s32 second);
extern void Func_0808a100(s32 arg0, s32 arg1);
extern void Func_0808a390(s32 object, s32 arg1);
extern void Func_080f9010(s32 arg0);

s32 Func_02004260(s32 arg0)
{
    s32 object = Func_080090c8(22);
    s32 first = Func_08077040(224);
    s32 second = Func_08077038(first, 224);
    u8 *record;
    s32 slot;

    if (object == 0)
        return first;

    Func_08009098(object, 0x0200cbe4);
    record = *(u8 **)(object + 80);
    record[38] = 0;
    record[39] = 0;
    record[5] &= ~33;
    record[9] &= 15;
    *(s32 *)(object + 40) = 0x28000;
    *(s32 *)(object + 72) = 0x4000;
    slot = Func_08000140(17, 0x608);
    Func_08015250(arg0);
    Func_080001c8(record[28], 128, slot + 0x400);
    Func_08000150(17);
    Func_080f9010(0x53);
    Func_0808a390(object, 3);
    Func_080772b0(first, second);
    Func_08077028(first, arg0);
    Func_080090d0(object);
    Func_0808a100(0, 1);
    return first;
}
