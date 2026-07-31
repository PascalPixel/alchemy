typedef int s32;
typedef unsigned char u8;

/*
 * resource_380 owner at 0x0200227c, 388 bytes: six-actor scene wiring.
 * Creates a shared handle, enables six actors (5, 9, 11, 10, 14, 13,
 * each through its own resident enable entry), places each at a fixed
 * fixed-point coordinate pair, then for each actor fetches its object,
 * stores the shared handle at +104, sets flag bit 1 at +90, and binds
 * the shared descriptor table 0x0200cbd0.  Actor 14 additionally gets
 * position 0x10000/0x10000 (+24/+28), copies actor 11's +85 byte,
 * zeroes +12, and actor 13's bind is followed by one trailing
 * parameterless call.
 *
 * Complete owner: `push {r5,r6,lr}` + r8-sl spill at 0x0200227c
 * through `pop {r0} / bx r0` at 0x020023e4, then the literal pool
 * 0x020023e8-0x020023ff (five fixed-point x coordinates and the
 * descriptor 0x0200cbd0); next owner's prologue at 0x02002400.
 *
 * Uncertainty: callees unidentified beyond call shape -- each actor id
 * appears to have its own dedicated enable/fetch/bind entry points in
 * the resident module (6-to-20-byte address strides), which is why the
 * extern list is long.  Func_02006c8e enables (13, 1) early and binds
 * (object, table) late; it is declared old-style.  The final
 * Func_02006d7a call reuses whatever the preceding bind returned; both
 * are old-style for that reason.
 */

extern s32 Func_02006c48(s32 arg0);
extern void Func_02006c2e(void);
extern void Func_02006c66(s32 actor, s32 arg1);
extern void Func_02006c6e(s32 actor, s32 arg1);
extern void Func_02006c76(s32 actor, s32 arg1);
extern void Func_02006c7e(s32 actor, s32 arg1);
extern void Func_02006c86(s32 actor, s32 arg1);
extern s32 Func_02006c8e();
extern void Func_02006cd2(s32 actor, s32 x, s32 y);
extern void Func_02006cde(s32 actor, s32 x, s32 y);
extern void Func_02006cea(s32 actor, s32 x, s32 y);
extern void Func_02006cf6(s32 actor, s32 x, s32 y);
extern void Func_02006d04(s32 actor, s32 x, s32 y);
extern void Func_02006d10(s32 actor, s32 x, s32 y);
extern s32 Func_02006cce(s32 actor);
extern s32 Func_02006cf2(s32 actor);
extern s32 Func_02006d0c(s32 actor);
extern s32 Func_02006d26(s32 actor);
extern s32 Func_02006d40(s32 actor);
extern s32 Func_02006d5e(s32 actor);
extern s32 Func_02006d7a();
extern void Func_02006bec(s32 object, s32 table);
extern void Func_02006c06(s32 object, s32 table);
extern void Func_02006c20(s32 object, s32 table);
extern void Func_02006c3a(s32 object, s32 table);
extern void Func_02006c74(s32 object, s32 table);

void Func_0200227c(void)
{
    s32 handle = Func_02006c48(0);
    s32 table = 0x0200cbd0;
    s32 object;
    s32 helper;

    Func_02006c2e();
    Func_02006c66(5, 1);
    Func_02006c6e(9, 1);
    Func_02006c76(11, 1);
    Func_02006c7e(10, 1);
    Func_02006c86(14, 1);
    Func_02006c8e(13, 1);
    Func_02006cd2(5, 0x01db0000, 0x14c0000);
    Func_02006cde(9, 0x01eb0000, 0x14c0000);
    Func_02006cea(11, 0x01cb0000, 0x15c0000);
    Func_02006cf6(10, 0x01fb0000, 0x15c0000);
    Func_02006d04(14, 0x1cc0000, 0x1680000);
    Func_02006d10(13, 0x01d70000, 0x1320000);

    object = Func_02006cce(5);
    *(s32 *)(object + 104) = handle;
    *(u8 *)(object + 90) |= 1;
    Func_02006bec(object, table);

    object = Func_02006cf2(9);
    *(s32 *)(object + 104) = handle;
    *(u8 *)(object + 90) |= 1;
    Func_02006c06(object, table);

    object = Func_02006d0c(11);
    *(s32 *)(object + 104) = handle;
    *(u8 *)(object + 90) |= 1;
    Func_02006c20(object, table);

    object = Func_02006d26(10);
    *(s32 *)(object + 104) = handle;
    *(u8 *)(object + 90) |= 1;
    Func_02006c3a(object, table);

    object = Func_02006d40(14);
    *(s32 *)(object + 104) = handle;
    *(u8 *)(object + 90) |= 1;
    *(s32 *)(object + 24) = 0x10000;
    *(s32 *)(object + 28) = 0x10000;
    helper = Func_02006d5e(11);
    *(u8 *)(object + 85) = *(u8 *)(helper + 85);
    *(s32 *)(object + 12) = 0;
    Func_02006c74(object, table);

    object = Func_02006d7a(13);
    *(s32 *)(object + 104) = handle;
    *(u8 *)(object + 90) |= 1;
    Func_02006d7a(Func_02006c8e(object, table));
}
