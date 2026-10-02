/* Battle: clear the active party members' status bytes and stat modifiers.
   Returns int: its epilogue returns through r1. */
#include "TYPES.H"
#include "BATTLE_TYPES.H"
#include "SCENE.H"
#include "BATTLE_SUMMON.H"
#include "BATTLE_PARTY.H"
#include "OWNER_STATE.H"

s32 BattleParty_PrepareActiveOwners(u16 *owners);
void BattleUnit_Recalculate(s32 owner);

s32 Trade_CanOfferDjinnFar(s32 id, s32 x, s32 y);
void Trade_AddOfferFar(s32 id, s32 x, s32 y);
s32 GameFlag_TestFar(s32 message);
s32 Djinn_ActivateFar(s32 id, s32 x, s32 y);
s32 Trade_RemoveOfferFar(s32 id, s32 x, s32 y);

struct DjinnRecoveryTable *Trade_GetOfferStateFar(s32 side);
struct BattleObjectSlot *GetBattleObjectSlot(s32 object_id);

s32 BattleParty_ResetActiveRuntimeFields(void)
{
    u16 owners[10];
    s32 count;
    s32 i;

    count = BattleParty_PrepareActiveOwners(owners);

    i = 0;
    if (i < count) {
        struct BattleUnit *unit;
        u8 *cursor;
        s32 remaining;

        do {
            unit = Owner_GetStateFar(owners[i]);
            cursor = (u8 *)&unit->element_modifier[3];
            remaining = 3;

            do {
                remaining--;
                *cursor-- = 0;
            } while (remaining >= 0);

            unit->attack_modifier_turns = 0;
            unit->attack_modifier = 0;
            unit->defense_modifier_turns = 0;
            unit->defense_modifier = 0;
            unit->res_modifier_turns = 0;
            unit->res_modifier = 0;
            unit->delusion = 0;
            unit->confusion = 0;
            unit->charm = 0;
            unit->stun = 0;
            unit->sleep = 0;
            unit->psy_seal = 0;
            unit->refrain = 0;
            unit->reflect = 0;
            unit->death_count = 0;
            unit->unknown_142[0] = 0;
            unit->unknown_142[1] = 0;
            unit->ready_pose = 0;
            unit->cannot_move = 0;
            unit->agility_modifier_turns = 0;
            unit->agility_modifier = 0;
            unit->battle_end_state = 0;

            BattleUnit_Recalculate(owners[i]);
            i++;
        } while (i < count);
    }
}

s32 BattlePlacement_UpdateEntries(void)
{
    u16 owners[10];
    s32 count;
    s32 i;
    s32 owner;
    s32 x;
    s32 y;

    count = BattleParty_PrepareActiveOwners(owners);

    for (i = 0; i < count; i++) {
        owner = owners[i];
        for (x = 0; x <= 3; x++) {
            for (y = 0; y <= 19; y++) {
                if (Trade_CanOfferDjinnFar(owner, x, y) != 0) {
                    struct DjinnRecoveryList *list = &Trade_GetOfferStateFar((u32)owner > 7 ? 1 : 0)->list;
                    s32 j;

                    for (j = 0; j < list->count; j++) {
                        if (x == list->entries[j].element && y == list->entries[j].index)
                            break;
                    }
                    if (j == list->count)
                        Trade_AddOfferFar(owner, x, y);
                }
            }
        }
    }

    if (GameFlag_TestFar(364) != 0)
        return;

    {
        struct DjinnRecoveryList *list = &Trade_GetOfferStateFar(0)->list;
        struct DjinnRecoveryEntry *entry;

        i = 0;
        if (i < list->count) {
            s32 permanent_timer = -1;

            entry = list->entries;
            do {
                if (entry->turns == permanent_timer && GetBattleObjectSlot(entry->unit_id) == 0) {
                    u8 id = entry->unit_id;
                    u8 ex = entry->element;
                    u8 ey = entry->index;

                    Djinn_ActivateFar(id, ex, ey);
                    Trade_RemoveOfferFar(id, ex, ey);
                }
                i++;
                entry++;
            } while (i < list->count);
        }
    }
}
