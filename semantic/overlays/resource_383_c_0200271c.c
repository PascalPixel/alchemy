typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;

/*
 * Set or clear the scene flag selected by the actor-0 position window. This
 * owner is installed as the in-image callback 0x0200a71d (the Thumb pointer
 * for 0x0200271c) by the entry driver at 0x02002758.
 *
 * It starts with push {r5,lr} at 0x0200271c, returns through pop {r5}/pop
 * {r0}/bx r0 at 0x02002752-0x02002756, and is followed immediately by the
 * entry driver at 0x02002758. The complete owner is 60 bytes with no pool.
 */

extern u8 *Func_0808a080(s32 actor);
extern void Func_080770c8(s32 flag);
extern void Func_080770d0(s32 flag);

void Func_0200271c(void)
{
    u8 *actor0 = Func_0808a080(0);
    s32 x = (*(s32 *)(actor0 + 8) >> 20) - 34;
    s32 z;

    actor0 = Func_0808a080(0);
    z = *(s32 *)(actor0 + 16) >> 20;

    if ((u32)x <= 1 && z > 40 && z <= 42) {
        Func_080770c8(148 << 2);
    } else {
        Func_080770d0(148 << 2);
    }
}
