#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "OBJDISP.H"
#include "OBJECT_EFX.H"

u32 Random16(void);

struct FallingObject {
    u8 unknown_00[12];
    s32 y;
    u8 unknown_10[4];
    s32 floor;
    u8 unknown_18[70];
    s16 dispatch_state;
    u8 unknown_60[12];
    void *update;
};

void BattleFx_LandFallingObject(struct FallingObject *object)
{
    if (object->y <= object->floor) {
        object->dispatch_state = 2;
        ObjectDispatch_InitializeFar((struct DispatchObject *)object,
            (u32)BattleFx_CommonParticleScript);
        object->update = 0;
    }
}

void BattleFx_UpdateDriftingFallObject(struct ObjectRuntime *object)
{
    s32 r;

    object->y -= 0x4ccc;
    r = Random16();
    object->x += r - Random16();
    if (object->y <= object->terrain_height) {
        ObjectDispatch_InitializeFar((struct DispatchObject *)object,
            (u32)BattleFx_CommonParticleScript);
    }
}

void BattleFx_CallEffect05(void)
{
    RunBattleEffect05();
}
