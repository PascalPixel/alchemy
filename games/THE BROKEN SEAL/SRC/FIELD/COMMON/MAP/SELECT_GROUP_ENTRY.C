#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_TYPES.H"
#include "GAME_STATE.H"

struct BattleUnit *Owner_GetStateFar(s32 unit_id);
s32 GameFlag_TestFar(s32 flag_id);
void GameFlag_SetBitFar(s32 flag_id);

/*
 * Walks the word table returned by the overlay's table provider and
 * replaces the current scene with the first entry
 * of the matching group that passes the kind and flag checks.
 *
 * Table layout: a group header word has its upper 20 bits clear and a
 * 12-bit id in the low bits (0x1ff terminates the table); every other word
 * is an entry: bits 0-11 id (0x1ff ends the group), bits 12-19 sub value,
 * bits 20-27 kind (0 ends the group, 0xff matches any kind), bit 28 marks
 * that a flag id follows in the next word.
 */

typedef u32 *(*TableProvider_0808a5f8)(void);

struct Services_0808a5f8 {
    u8 padding000[0x14];
    TableProvider_0808a5f8 table_provider;
};

extern struct Services_0808a5f8 gOverlayArea;

void MapGroupTable_SelectEntry(s32 kind)
{
    s16 cur;
    u32 *p;
    u32 *start;
    u32 word;
    s32 id;
    s32 result;
    s32 sub;
    s32 entry_sub;
    s32 entry_kind;
    s32 flag;

    cur = gGameState.scene;
    p = gOverlayArea.table_provider();
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

/* A unit's HP or PP as a fraction of its maximum, in 1/0x4000. */
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

/* A gauge shows empty only when nothing is left. */
static __inline__ void Unit_UpdateGauges(struct BattleUnit *unit)
{
    unit->hp_gauge = Vitals_Ratio(unit->hp, unit->max_hp);
    if (unit->hp_gauge == 0 && unit->hp != 0)
        unit->hp_gauge = 1;
    unit->pp_gauge = Vitals_Ratio(unit->pp, unit->max_pp);
    if (unit->pp_gauge == 0 && unit->pp != 0)
        unit->pp_gauge = 1;
}

/*
 * Chooses the scene and entrance the coming scene change leads to. A reason
 * of -1 is the party's defeat: the selected member wakes with 1 HP, the first
 * two members are fully restored while flag 32 is set, and the party goes to the
 * defeat destination, or to the saved one when none is set. Any other reason
 * takes the requested destination. A half left at -1 takes the default; a
 * request that names neither half takes the default whole and sets flag 0x109.
 */
void Party_SetReturnPoint(s32 reason)
{
    struct BattleUnit *unit;
    s32 i;
    s32 scene;
    s32 entrance;

    gGameState.scene_change_reason = reason;
    if (reason == -1) {
        unit = Owner_GetStateFar(gGameState.selected_actor);
        if (unit->hp == 0) {
            unit->hp = 1;
            Unit_UpdateGauges(unit);
        }
        if (GameFlag_TestFar(32)) {
            for (i = 0; i < 2; i++) {
                unit = Owner_GetStateFar(i);
                /* FAKEMATCH: the one-pass block keeps both copies ahead of the ratio */
                do {
                    unit->hp = unit->max_hp;
                    unit->pp = unit->max_pp;
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
