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
#include "battle_system.h"
#include "battle_unit.h"
#include "event_work.h"

void BattleEffect_CleanupSceneObjects(void);

s32 EventRuntime_ExecutePackedAction(u32 packed)
{
    s32 action, mode, kind, selector, target;
    struct PendingEvent *first, *second;

    action = packed & 0x3ff;
    mode = (packed >> 10) & 15;
    kind = BattleActGet(action)->unknown_04[2];
    selector = Func_080ce31c(kind);
    first = Func_080ce458(0x30000005, kind, selector);
    second = Func_080ce458(0x20000005, kind, selector);
    target = -1;
    if (first != NULL || second != NULL) {
        if (selector != -1 && (selector & 0x100))
            target = selector & 255;
    }
    BattleFx_LoadActionEffectResources(action, 0);
    BattleFx_SetupObjectPair(EventRuntime_GetControlledOwner(), target);
    EventRuntime_RunPendingEvent(first, mode, target);
    FieldEvent_RunTypeHandler();
    EffectRuntime_StopCurrentObject();
    EventRuntime_RunPendingEvent(second, mode, target);
    BattleEffect_CleanupSceneObjects();
    return 0;
}
