/* 2026-09-30 asm-only (inline asm not yet permitted to workers): as 080b6d30, a FAKEMATCH-tagged one-instruction asm copy of the zero result into the counter. */
/* Draft, not exact (2026-09-24): 1 differing halfword. The reference copies a
   zero from another register (mov) where this spelling materialises movs #0.
   cse.c picks src_folded on a cost tie and COST(const_int 0) is 0 against 1
   for a pseudo, so the copy survives only where CSE cannot see the zero;
   do/while, cast, reordered and conditional spellings all keep the movs.
   2026-09-29: callees carry the build's names and the presentation cell is
   gTransitionWork, so alchemy permute scores 200: the one inserted movs and
   the one missing copy, nothing else. Not adoptable as written either way:
   the scene cell is reached as gTransitionWork - 140 (the reference folds
   gBattleWork's address into that subtraction), a cross-object offset that
   depends on where the linker places the two cells. */
#include "BATTLE_PARTY.H"
#include "BATTLE_TYPES.H"
#include "TYPES.H"

struct BattlePresentationState {
    s32 mode;
    u8 unknown_04[12];
    s32 active;
};

struct BattleSceneContext {
    u8 unknown_00[0x45];
    u8 encounter_mode;
};

struct BattleTrigger {
    u8 unit_id;
};

struct BattleUnit *Owner_GetStateFar(s32 unit_id);
void BattlePres_SetActorModes(u16 *, s32);
void UiText_ShowMessageAndWaitCoreFar(s32 message_id);
void BattlePresentation_WaitForAdvance(void);
void BattleMotion_SetupEscapeObject(s32 unit_id);
void WaitFrames(s32 frames);
u32 Random16(void);
void BattleActor_RemoveFromLists(s32 unit_id);
void ActivateBattleObjectSlot(s32 unit_id);

extern u8 gTransitionWork[];

s32 BattlePresentation_RunEncounterOrUnitTrigger(struct BattleTrigger *trigger)
{
    u8 *presentation_addr = gTransitionWork;
    struct BattlePresentationState *presentation =
        *(struct BattlePresentationState **)presentation_addr;
    struct BattleSceneContext *scene =
        *(struct BattleSceneContext **)(presentation_addr - 140);
    s32 completed = 0;
    s32 party_mode;

    presentation->mode = 0x2000;
    presentation->active = 1;
    BattlePres_SetActorModes(0, 0);

    if (trigger->unit_id <= 7) {
        party_mode = completed;

        if (scene->encounter_mode != 2) {
            party_mode = 1;
        }

        if (!party_mode) {
            UiText_ShowMessageAndWaitCoreFar(0x847);
            BattlePresentation_WaitForAdvance();
        } else {
            s16 unit_ids[14];
            s32 index = BattleParty_ListLivingUnits(1, (u16 *)unit_ids) - 1;

            while (index != -1) {
                struct BattleUnit *unit = Owner_GetStateFar(unit_ids[index]);

                if (unit->stun == 0 && unit->sleep == 0) {
                    BattleMotion_SetupEscapeObject(unit_ids[index]);
                    WaitFrames(8);
                }
                index--;
            }
            WaitFrames(22);
            completed = 1;
        }
    } else if (((Random16() * 10) >> 16) <= 6) {
        s16 event[2];

        event[0] = trigger->unit_id;
        event[1] = 0xff;
        BattleMotion_SetupEscapeObject(event[0]);
        WaitFrames(8);
        BattleActor_RemoveFromLists(trigger->unit_id);
        ActivateBattleObjectSlot(trigger->unit_id);
    } else {
        UiText_ShowMessageAndWaitCoreFar(0x847);
        BattlePresentation_WaitForAdvance();
    }

    presentation->active = 0;
    return completed;
}
