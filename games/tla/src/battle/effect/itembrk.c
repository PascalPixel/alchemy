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
#include "system.h"
#include "ram_buffer.h"
#include "field_event.h"

struct BattleFxScene {
    u8 unknown_00[16];
    void *volatile main_object;
};


void BattleFx_RunItemBreakSequence(void)
{
    /* FAKEMATCH: an unused 12-byte local explains the native 12-byte frame. */
    s32 work[3];
    struct BattleFxScene *scene;
    void *object;

    scene = Ram_HeapSlots->effect_work;
    object = scene->main_object;

    BattleEffect_InitializeSharedScene();
    object = BattleFx_StartItemBreak(object);
    BattleFx_SnapScaleToFull(object);
    if (object != 0) {
        Object_SetMode(object, 4);
        WaitFrames(30);
    }
    BattleFx_PrepareBufferInterpolation();
    UpdateRisingParticleBurst(object);
}
