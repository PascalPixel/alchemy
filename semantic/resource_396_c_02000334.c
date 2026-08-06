#include "types.h"

extern s32 Data_02000240[];
extern s16 *Data_0200add0;
















/* Gate and stage the story-driven actor entrance for a new variant. */
extern u8 * Func_02001e68(s32 actor);
extern s32 Func_02001e52(s32 flag);
extern void Func_02001e32(s32 left, s32 top, s32 width, s32 height,
                          s32 right, s32 bottom);
extern void Func_02001e78(s32 flag);
extern void Func_02001e5e(s32 left, s32 top, s32 width, s32 height,
                          s32 right, s32 bottom);
extern void Func_02001fcc(s32 cue);
extern void Func_02001eb8(void);
extern void Func_02001f88(s32 scene, s32 variant);
extern void Func_02001f08(s32 actor, s32 pose);
extern void Func_02001ee6(u8 *actor, s32 phase);
extern u8 * Func_02001e9e(s32 actor);
extern void Func_02001f8e(s32 actor, s32 pose);
extern void Func_02001ee4(s32 actor);
extern void Func_02001fb6(s32 x, s32 y, s32 z, s32 mode);
extern void Func_0200202e(s32 cue);
extern void Func_02001f14(s32 actor);
extern void Func_02001fc2(s32 actor, s32 mode);
extern void Func_02001e7c(s32 frames);
extern void Func_02001f3e(s32 flag);
void Func_02000334(s32 flag, s32 left, s32 top, s32 variant)
{
    s32 right = (Data_02000240[119] >> 20) + 64;
    s32 bottom = Data_02000240[121] >> 20;
    s32 actorId = Data_02000240[125];
    u8 *actor = Func_02001e68(actorId);
    s32 frame;

    if (variant == *Data_0200add0)
        return;
    *Data_0200add0 = (s16)variant;

    if (Func_02001e52(flag) == 0) {
        Func_02001e32(left, top, 1, 1, right, bottom);
        Func_02001e78(flag);
        return;
    }

    *Data_0200add0 = -1;
    Func_02001e5e(left, top + 1, 1, 1, right, bottom);
    Func_02001fcc(206);
    Func_02001eb8();
    Func_02001f88(45, variant);
    Func_02001f08(actorId, 27);
    Func_02001ee6(Func_02001e9e(actorId), 0);
    Func_02001f8e(actorId, 0x101);
    Func_02001ee4(30);
    Func_02001fb6(-1, -1, -1, 0);

    *(s32 *)(actor + 20) = (s32)0xff600000;
    *(s32 *)(actor + 72) = 0x8000;
    actor[85] = 2;
    Func_0200202e(204);
    Func_02001f14(3);
    Func_02001fc2(actorId, 3);
    actor[34] = 2;

    for (frame = 29; frame >= 0; frame--) {
        Func_02001e7c(1);
        *(u16 *)(actor + 6) += 0x2000;
    }

    if (variant != 50)
        Func_02001f3e(0x122);
}
