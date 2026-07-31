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
 * Uncertainty: callees unidentified beyond call shape.  Func_02008bd8
 * is called with one argument for the first companion and with
 * (first, 224) for the second, so it is declared old-style.  The
 * object layout (+80 record pointer, record +5/+9 flag bytes, +38/+39
 * clear pair, +28 sprite byte) matches the shapes the other rows of
 * this overlay touch.
 */

extern s32 Func_02008b38(s32 kind);
extern s32 Func_02008bd8();
extern void Func_02008b50(s32 object, s32 descriptor);
extern s32 Func_02008b5e(s32 row, s32 source);
extern void Func_02008c06(s32 arg0);
extern void Func_02008b8c(s32 sprite, s32 arg1, s32 destination);
extern void Func_02008b8a(s32 row);
extern void Func_02008e00(s32 arg0);
extern void Func_02008df0(s32 object, s32 arg1);
extern void Func_02008c80(s32 first, s32 second);
extern void Func_02008c48(s32 first, s32 arg1);
extern void Func_02008bd6(s32 object);
extern void Func_02008d1e(s32 arg0, s32 arg1);

s32 Func_02004260(s32 arg0)
{
    s32 object = Func_02008b38(22);
    s32 first = Func_02008bd8(224);
    s32 second = Func_02008bd8(first, 224);
    u8 *record;
    s32 slot;

    if (object == 0)
        return first;

    Func_02008b50(object, 0x0200cbe4);
    record = *(u8 **)(object + 80);
    record[38] = 0;
    record[39] = 0;
    record[5] &= ~33;
    record[9] &= 15;
    *(s32 *)(object + 40) = 0x28000;
    *(s32 *)(object + 72) = 0x4000;
    slot = Func_02008b5e(17, 0x608);
    Func_02008c06(arg0);
    Func_02008b8c(record[28], 128, slot + 0x400);
    Func_02008b8a(17);
    Func_02008e00(0x53);
    Func_02008df0(object, 3);
    Func_02008c80(first, second);
    Func_02008c48(first, arg0);
    Func_02008bd6(object);
    Func_02008d1e(0, 1);
    return first;
}
