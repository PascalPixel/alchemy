/* 2026-09-29: callees carry the build's names and eight minutes of
 * permutation stored the action through pointer arithmetic (*(unit_stack +
 * 14)); alchemy permute scores 1935, from 2195. Messages 0x648 and 0x654
 * are still Value_ symbols. */
/* Draft, not exact (2026-09-24): candidate=516 reference=516 differing_halfwords=147. Constants the reference loads from
   the literal pool are spelled as link-time Value_ symbols, which restores
   the reference size; wraps marked FAKEMATCH only move scheduling. */
#include "TYPES.H"
extern u8 Value_00000129;
extern u8 Value_00000644;
extern u8 Value_0000c000;
extern u8 Value_00002000;
extern u8 Value_00000654;
extern u8 Value_000006a8;
extern u8 Value_00000648;

struct BattlePresentationTransition {
    s32 blend;
    s32 timer;
    u8 reserved08[12];
    s32 active;
};

s32 BattlePresentation_DispatchAction(s16 *action, s32 delay)
{
    u16 unit_stack[16];
    struct BattlePresentationTransition *transition;
    u8 *battle;
    u8 *actor;
    u8 *render_state;
    u8 **transition_slot;
    s32 result;
    s32 visible_count;
    s32 index;
    s32 preserve_action;

    preserve_action = 0;
    if (action[0] == 0xff)
        return 0;
    actor = Owner_GetStateFar(action[0]);
    if (*(s16 *)(actor + 0x38) == 0)
        return -1;
    if (actor[0x129] == 0)
        BattleCommand_SelectAutomatic(action, 1);

    transition_slot = (u8 **)0x03001f00;
    transition = *(struct BattlePresentationTransition **)transition_slot;
    transition->timer = 60;
    battle = *(transition_slot - 35);
    transition->active = 0;
    *(s32 *)(battle + 0x644) = 0x10000;
    render_state = *(transition_slot - 32);
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork(render_state, render_state + 12);
    do { Camera_StoreSceneParameters(0x01fe0000, _call_via_r3(0x01fe0000, 0xc000), 0x7fff0000); } while (0); /* FAKEMATCH */

    if (delay != 0) {
        transition->blend = 0x2000;
        WaitFrames(delay);
    }
    *(unit_stack + 14) = action[0];
    do { unit_stack[15] = 0xff; } while (0); /* FAKEMATCH */
    BattlePres_SetActorModes(unit_stack + 14, 1);

    result = BattleCommand_BuildPlan(action, battle + 0x654);
    if (result == 0) {
        switch (*(s32 *)(battle + 0x6a8)) {
        case 1: BattlePres_RunActorEntries(battle + 0x654, 0); break;
        case 2: RunBattlePresentation(battle + 0x654, 0); break;
        case 3: BattlePresentation_RunUnitTransition(battle + 0x654, 1); break;
        case 4: BattlePresentation_RunUnitTransition(battle + 0x654, 0); break;
        case 5: Func_080ba978(battle + 0x654, 0); break;
        case 6: Func_080ba978(battle + 0x654, 1); break;
        case 7: Func_080ba978(battle + 0x654, 2); break;
        case 8: Func_080ba6ac(battle + (s32)&Value_00000654, 0, action); break;
        case 9:
            if (BattlePresentation_RunEncounterOrUnitTrigger(battle + (s32)&Value_00000654) != 0)
                preserve_action = 1;
            break;
        }
        if (preserve_action != 0)
            goto finish;
    } else {
        if (result == -1) {
            BattlePresentation_WaitForAdvance();
            WaitFrames(3);
        }
        BattlePres_SetActorModes(0, 0);
    }

    BattleMotion_DestroyAllSlotObjects();
    BattleUnit_ProcessTurnEnd(battle + 0x654);
    BattleActor_CommitPlacement();
    visible_count = BattleParty_ListActorIds(3, unit_stack);
    for (index = 0; index < visible_count; index++)
        Actor_ResetMotionAtAnchor(unit_stack[index]);
    action[0] = 0xff;

finish:
    BattlePresentation_ConfigurePaletteFade(2, *(u16 *)(battle + (s32)&Value_00000648), 0);
    return preserve_action;
}
