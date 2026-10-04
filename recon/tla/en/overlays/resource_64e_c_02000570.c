/*
 * Draft: OverlayObject_UpdateEveryFourFrames, ☀️'s VINASU_HEYA twin; it goes
 * before Scene_GetEntrances in FIELD/VINASU_CHOJO/SCENE_TABLES.C. Score 2:
 * the listing builds the mask between loading gFrameCount's address and its
 * value; this C after the value. It matches with load scheduling.
 */
#include "TYPES.H"

void Object_SetPartAttribute(void *object, s32 value);
void Engine_AudioPlayCue(s32 cue);
s32 Random16Far(void);

extern u32 gFrameCount;

struct SceneEffectSource {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct SceneEffectParams {
    s32 unk00;
    s32 mode;
    s32 color1;
    s32 color2;
    u8 unknown_10[0x18];
};

void Func_02000150(s32 x, s32 y, s32 z, s32 unused, s32 speed, s32 phase,
                   s32 life, struct SceneEffectParams *params);

s32 OverlayObject_UpdateEveryFourFrames(void *obj)
{
    if ((gFrameCount & 3) == 0)
        Object_SetPartAttribute(obj, 7);
    else
        Object_SetPartAttribute(obj, 0);

    if ((gFrameCount & 7) == 0)
        Engine_AudioPlayCue(138);
    return 0;
}
