#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_scene.h"
#include "frame.h"
#include "field_event.h"

const u32 *Scene_GetExits(void)
{
    return gSceneExits;
}

struct FieldPartBytes
{
    u8 unk00[5];
    u8 frame;
};

struct FieldAnimation
{
    u8                     unk00[40];
    struct FieldPartBytes *part;
};

s32 Func_02000364(struct FieldActor *actor)
{
    u32 frame = gFrameCount;

    if (frame % 10 == 0)
        ((struct FieldAnimation *)actor->sprite)->part->frame = frame % 20 / 10 + 3;
    return 1;
}
