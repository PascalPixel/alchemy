#include "types.h"














/* Fade the selected encounter actor and current map actor together for sixty
 * frames, then select the scene-specific presentation result. */
extern s32 Func_02001e70(s32 counterId);
extern u8 * Func_02001ea8(s32 actorId);
extern u8 * Func_02001eb0(s32 actorId);
extern void Func_02001ea6(void);
extern void Func_02001f68(s32 value, s32 direction, s32 distance, s32 enabled);
extern void Func_02001fb6(s32 resourceId);
extern void Func_02001e86(u8 *actor, s32 mode);
extern void Func_02001e74(s32 frames);
extern void Func_02001fd4(void);
extern void Func_02001fe0(void);
extern void Func_02001f14(void);
extern void Func_02001eec(s32 flagId);
extern s32 Func_02001f16(s32 counterId);
extern void Func_02001ff2(s32 mode, s32 value);
extern void Func_02001ffc(s32 mode, s32 value);
void Func_02000c00(void)
{
    s32 encounterActorId = Func_02001e70(0x218);
    s32 mapActorId = *(volatile s32 *)0x02000434;
    u8 *mapActor = Func_02001ea8(mapActorId);
    u8 *encounterActor = Func_02001eb0(encounterActorId);
    u32 frame;

    Func_02001ea6();
    Func_02001f68(-1, -1, -1, 0);
    Func_02001fb6(219);
    Func_02001e86(mapActor, 0);
    encounterActor[85] = 0;
    mapActor[85] = 0;
    *(s32 *)(mapActor + 40) = 0;
    mapActor[97] = 1;
    encounterActor[97] = 1;

    for (frame = 0; frame < 60; frame++) {
        *(s32 *)(mapActor + 40) += 0x3333;
        *(s32 *)(encounterActor + 40) += 0x3333;
        Func_02001e74(1);
    }

    Func_02001fd4();
    Func_02001fe0();
    Func_02001f14();
    Func_02001eec(0x122);

    if (*(volatile s16 *)0x02000400 == 0xa5 &&
        Func_02001f16(0x218) == 11) {
        Func_02001ff2(2, 77);
    } else {
        Func_02001ffc(2, 27);
    }
}
