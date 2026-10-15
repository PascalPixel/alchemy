#include "types.h"
#include "battle.h"
#include "battle_display.h"
#include "battle_obj.h"
#include "motobj.h"
#include "owner_state.h"
#include "ram_buffer.h"
#include "system.h"
#include "text/msg_ids.h"

void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 travelDivisor, s32 initialVelocityY);
void Func_08138008(struct BattlePresentationWork *work);

// The actor announces its attack, walks up to its target and plays the
// attack's effect on it. Returns -1 when either id is not a battle object.
s32 BattlePres_RunApproachAction(struct BattleActionRecord *input)
{
    struct BattlePresentationWork work;

    if (Ram_HeapSlots->transition_work->target_yaw == 0x2000)
    {
        Ram_HeapSlots->transition_work->target_yaw = 0x2000;
        WaitFrames(10);
    }
    else
    {
        Ram_HeapSlots->transition_work->target_yaw = 0x2000;
        WaitFrames(30);
    }

    work.actor = input->unit_id;
    if (BattleObject_IsValidId(work.actor) < 0)
        return -1;

    work.actors[0] = input->target;
    if (BattleObject_IsValidId(work.actors[0]) < 0)
        return -1;

    UnitGet(work.actor);
    UnitGet(work.actors[0]);
    Random16();
    UiText_DrawQuantity(work.actor, 1);
    UiText_ShowMessageAndWaitCoreFar((s32)MsgActorAttacks);
    BattleMotion_ApproachTarget(work.actor, work.actors[0], 13, 0);
    ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)BattleObjGet(work.actor)->object, 16);
    BattleObjGet(work.actors[0]);

    work.count = 1;
    if ((u16)work.actors[0] <= 7)
        work.side = 1;
    else
        work.side = 0;
    work.flags = 0;

    WaitFrames(4);
    Func_08138008(&work);
    ObjMotRst(work.actors[0]);
    ObjMotRst(work.actor);
    return 0;
}
