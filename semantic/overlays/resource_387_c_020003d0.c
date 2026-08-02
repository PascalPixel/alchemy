typedef unsigned char u8;
typedef signed int s32;

/* resource_387 actor-10 tile-23 transition owner, 108 bytes through its pool. */

extern void Func_080091c0(s32 left, s32 top, s32 width, s32 height,
                          s32 right, s32 bottom);
extern void Func_080091e0(u8 *actor, s32 mode);
extern void Func_080770c8(s32 flagId);
extern void Func_0808a010(s32 frames);
extern u8 *Func_0808a080(s32 actor);

static s32 Fixed20_020003d0(s32 value)
{
    if (value < 0) {
        value += 0x0fffff;
    }
    return value >> 20;
}

void Func_020003d0(void)
{
    s32 tileX = Fixed20_020003d0(*(s32 *)(Func_0808a080(10) + 8));
    u8 *actor;

    if (tileX != 23) {
        return;
    }

    Func_0808a010(10);
    actor = Func_0808a080(10);
    actor[35] = 2;
    actor = Func_0808a080(10);
    actor[85] = 0;
    actor = Func_0808a080(10);
    Func_080091e0(actor, 0);
    Func_080091c0(54, 17, 1, 1, tileX, 17);
    Func_080770c8(0x863);
}
