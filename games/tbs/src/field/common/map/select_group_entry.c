#include "types.h"
#include "scene.h"
#include "btltype.h"
#include "game_state.h"
#include "fieldrun.h"
#include "btlsys.h"
#include "battle.h"

void MapGroupTable_SelectEntry(s32 kind)
{
    s16 cur;
    const u32 *p;
    const u32 *start;
    u32 word;
    s32 id;
    s32 result;
    s32 sub;
    s32 entry_sub;
    s32 entry_kind;
    s32 flag;

    cur = gGameState.scene;
    p = gOverlayArea.exits();
    result = 999;
    sub = 0;
    if (kind == 999)
        return;

    for (;;) {
        id = *p++;
        if (id & 0xfffff000)
            continue;
        id &= 0xfff;
        if (id == 0x1ff)
            break;
        if (id != cur)
            continue;
        start = p;
        for (;;) {
            word = *p++;
            id = word & 0xfff;
            entry_sub = (word & 0xff000) >> 12;
            entry_kind = (word & 0x0ff00000) >> 20;
            flag = word & 0x10000000;
            if (flag)
                flag = *p++;
            if (id == 0x1ff || entry_kind == 0) {
                result = *start & 0x1ff;
                break;
            }
            if (entry_kind == 0xff || entry_kind == kind) {
                if (flag == 0 || GameFlag_TestFar(flag) == 0) {
                    result = id;
                    sub = entry_sub;
                    break;
                }
            }
        }
        break;
    }

    if (result != 999) {
        gGameState.scene = result;
        gGameState.entrance = sub;
    }
}

static __inline__ s32 Vitals_Ratio(s32 value, s32 max)
{
    s32 ratio = (value << 14) / max;
    s32 clamped = 0x4000;

    if (ratio <= 0x4000) {
        clamped = 0;
        if (ratio >= 0)
            clamped = ratio;
    }
    return clamped;
}

static __inline__ void Unit_UpdateGauges(struct BtlUnit *unit)
{
    unit->hp_gauge = Vitals_Ratio(unit->hp, unit->max_hp);
    if (unit->hp_gauge == 0 && unit->hp != 0)
        unit->hp_gauge = 1;
    unit->ep_gauge = Vitals_Ratio(unit->ep, unit->max_ep);
    if (unit->ep_gauge == 0 && unit->ep != 0)
        unit->ep_gauge = 1;
}

void Party_SetReturnPoint(s32 reason)
{
    struct BtlUnit *unit;
    s32 i;
    s32 scene;
    s32 entrance;

    gGameState.scene_change_reason = reason;
    if (reason == -1) {
        unit = UnitGetFar(gGameState.selected_actor);
        if (unit->hp == 0) {
            unit->hp = 1;
            Unit_UpdateGauges(unit);
        }
        if (GameFlag_TestFar(32)) {
            for (i = 0; i < 2; i++) {
                unit = UnitGetFar(i);
                /* FAKEMATCH: needed to match: the one-pass block keeps both copies ahead of the ratio */
                do {
                    unit->hp = unit->max_hp;
                    unit->ep = unit->max_ep;
                } while (0);
                Unit_UpdateGauges(unit);
            }
        }
        scene = gGameState.defeat_scene;
        entrance = gGameState.defeat_entrance;
        if (scene == -1 && entrance == -1) {
            gGameState.scene = gGameState.saved_scene;
            gGameState.entrance = gGameState.saved_entrance;
        } else {
            if (scene != -1)
                gGameState.scene = scene;
            else
                gGameState.scene = gGameState.default_scene;
            if (entrance != -1)
                gGameState.entrance = entrance;
            else
                gGameState.entrance = gGameState.default_entrance;
        }
    } else {
        scene = gGameState.next_scene;
        entrance = gGameState.next_entrance;
        if (scene != -1 || entrance != -1) {
            if (scene != -1)
                gGameState.scene = scene;
            else
                gGameState.scene = gGameState.default_scene;
            if (entrance != -1)
                gGameState.entrance = entrance;
            else
                gGameState.entrance = gGameState.default_entrance;
        } else {
            gGameState.scene = gGameState.default_scene;
            gGameState.entrance = gGameState.default_entrance;
            GameFlag_SetBitFar(0x109);
        }
    }
}
