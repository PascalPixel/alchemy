#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "btltype.h"
#include "scene.h"
#include "btlsmn.h"
#include "btlpty.h"
#include "owner_state.h"
#include "btlsys.h"
void UnitCalcFar(s32 unit_id);

#define Trade_AddOfferFar ((void (*)(s32,s32,s32))Trade_AddOfferFar)

s32 Djinn_ActivateFar(s32 id, s32 element, s32 index);
s32 Trade_RemoveOfferFar(s32 id, s32 element, s32 index);

struct DjinnRecoveryTable *Trade_GetOfferStateFar(s32 side);
struct BtlObj *BtlObjGet(s32 object_id);

s32 BattleParty_ResetActiveRuntimeFields(void)
{
    u16 owners[10];
    s32 count;
    s32 i;

    count = BattleParty_PrepareActiveOwners(owners);

    i = 0;
    if (i < count) {
        struct BtlUnit *unit;
        u8 *cursor;
        s32 remaining;

        do {
            unit = UnitGetFar(owners[i]);
            cursor = (u8 *)&unit->elm_lv[3];
            remaining = 3;

            do {
                remaining--;
                *cursor-- = 0;
            } while (remaining >= 0);

            unit->atk_cnt = 0;
            unit->atk_lv = 0;
            unit->def_cnt = 0;
            unit->def_lv = 0;
            unit->res_cnt = 0;
            unit->res_lv = 0;
            unit->maboroshi = 0;
            unit->konran = 0;
            unit->charm = 0;
            unit->shibire = 0;
            unit->nemuri = 0;
            unit->fuuji = 0;
            unit->refrain = 0;
            unit->reflect = 0;
            unit->shiryo = 0;
            unit->unknown_142[0] = 0;
            unit->unknown_142[1] = 0;
            unit->sonae = 0;
            unit->miugoki = 0;
            unit->agi_cnt = 0;
            unit->agi_lv = 0;
            unit->chouhatsu = 0;

            UnitCalcFar(owners[i]);
            i++;
        } while (i < count);
    }
}

s32 BattleParty_ReconcileDjinnRecovery(void)
{
    u16 owners[10];
    s32 count;
    s32 i;
    s32 owner;
    s32 element;
    s32 index;

    count = BattleParty_PrepareActiveOwners(owners);

    for (i = 0; i < count; i++) {
        owner = owners[i];
        for (element = 0; element <= 3; element++) {
            for (index = 0; index <= 19; index++) {
                if (Trade_CanOfferDjinnFar(owner, element, index) != 0) {
                    struct DjinnRecoveryList *list = &Trade_GetOfferStateFar((owner < 0 || owner > 7) ? 1 : 0)->list;
                    s32 j;

                    for (j = 0; j < list->count; j++) {
                        if (element == list->entries[j].element && index == list->entries[j].index)
                            break;
                    }
                    if (j == list->count)
                        Trade_AddOfferFar(owner, element, index);
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
                if (entry->turns == permanent_timer && BtlObjGet(entry->unit_id) == 0) {
                    u8 id = entry->unit_id;
                    u8 element = entry->element;
                    u8 index = entry->index;

                    Djinn_ActivateFar(id, element, index);
                    Trade_RemoveOfferFar(id, element, index);
                }
                i++;
                entry++;
            } while (i < list->count);
        }
    }
}
