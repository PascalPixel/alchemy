#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "object_runtime.h"
#include "objdisp.h"
#include "object_efx.h"

u32 Random16(void);

struct FallingObject {
    u8 unknown_00[0xc];
    s32 y;
    u8 unknown_10[4];
    s32 floor;
    u8 unknown_18[0x46];
    s16 dispatch_state;
    u8 unknown_60[0xc];
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
