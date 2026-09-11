#include "types.h"
#include "scene.h"
#include "abi/battle/effects/item_break/run_item_break_sequence.h"

struct BattleEffectScene {
    u8 pad00[16];
    void *volatile main_object;
};

extern struct BattleEffectScene *gIw;

void *BattleFx_StartItemBreak(void *object);

void WaitFrames(s32 frames);
void BattleFx_PrepareBufferInterpolation(void);
void UpdateRisingParticleBurst(void *object);

void BattleFx_RunItemBreakSequence(void)
{
    s32 work[3];
    struct BattleEffectScene *scene;
    void *object;

    scene = gIw;
    object = scene->main_object;

    do {
        Battle_Run();
    } while (0);
    object = BattleFx_StartItemBreak(object);
    Battle_Do(object);
    if (object != 0) {
        Battle_Apply(object, 4);
        WaitFrames(30);
    }
    BattleFx_PrepareBufferInterpolation();
    UpdateRisingParticleBurst(object);
}
