#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "FIELD_SCENE.H"
#include "FRAME.H"
#include "FIELD_EVENT.H"

const u32 *Scene_GetExits(void)
{
    return gSceneExits;
}

struct FieldPartBytes {
    u8 unknown_00[5];
    u8 frame;
};

struct FieldAnimation {
    u8 unknown_00[40];
    struct FieldPartBytes *part;
};

s32 Func_02000364(struct FieldActor *actor)
{
    u32 frame = gFrameCount;

    if (frame % 10 == 0)
        ((struct FieldAnimation *)actor->sprite)->part->frame = frame % 20 / 10 + 3;
    return 1;
}
