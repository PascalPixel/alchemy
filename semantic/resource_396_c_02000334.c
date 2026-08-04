#include "types.h"

extern s32 Data_02000240[];
extern s16 *Data_0200add0;

extern u8 *Func_0808a080(s32 actor);
extern s32 Func_080770c0(s32 flag);
extern void Func_080091b8(s32 left, s32 top, s32 width, s32 height,
                          s32 right, s32 bottom);
extern void Func_080770c8(s32 flag);
extern void Func_080f9010(s32 cue);
extern void Func_0808a018(void);
extern void Func_0808a238(s32 scene, s32 variant);
extern void Func_0808a100(s32 actor, s32 pose);
extern void Func_080091e0(u8 *actor, s32 phase);
extern void Func_0808a1f0(s32 actor, s32 pose);
extern void Func_0808a010(s32 actor);
extern void Func_0808a210(s32 x, s32 y, s32 z, s32 mode);
extern void Func_0808a1e0(s32 actor, s32 mode);
extern void Func_080000c0(s32 frames);

/* Gate and stage the story-driven actor entrance for a new variant. */
void Func_02000334(s32 flag, s32 left, s32 top, s32 variant)
{
    s32 right = (Data_02000240[119] >> 20) + 64;
    s32 bottom = Data_02000240[121] >> 20;
    s32 actorId = Data_02000240[125];
    u8 *actor = Func_0808a080(actorId);
    s32 frame;

    if (variant == *Data_0200add0)
        return;
    *Data_0200add0 = (s16)variant;

    if (Func_080770c0(flag) == 0) {
        Func_080091b8(left, top, 1, 1, right, bottom);
        Func_080770c8(flag);
        return;
    }

    *Data_0200add0 = -1;
    Func_080091b8(left, top + 1, 1, 1, right, bottom);
    Func_080f9010(206);
    Func_0808a018();
    Func_0808a238(45, variant);
    Func_0808a100(actorId, 27);
    Func_080091e0(Func_0808a080(actorId), 0);
    Func_0808a1f0(actorId, 0x101);
    Func_0808a010(30);
    Func_0808a210(-1, -1, -1, 0);

    actor[85] = 2;
    *(s32 *)(actor + 20) = (s32)0xff600000;
    *(s32 *)(actor + 72) = 0x8000;
    Func_080f9010(204);
    Func_0808a010(3);
    actor[34] = 2;
    Func_0808a1e0(actorId, 3);

    for (frame = 29; frame >= 0; frame--) {
        *(u16 *)(actor + 6) += 0x2000;
        Func_080000c0(1);
    }

    if (variant != 50)
        Func_080770c8(0x122);
}
