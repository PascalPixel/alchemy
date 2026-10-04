#include "TYPES.H"
#include "GAME_STATE.H"
#include "BATTLE_UNIT.H"

void Engine_PartyRemoveActiveOwner(s32 id);
void Engine_PartyAddActiveOwner(s32 id);
void Engine_ObjectAttachWorkTarget(s32 id, s32 target);
struct BattleUnit *Engine_OwnerGetState(s32 id);
void Engine_EventClearInvalidPackedValues(void);

/* Colosso: leave only the competitor in the active party, select it, cure
 * its poison and fill its HP and PP, then set both gauges from them, each
 * clamped to 0..0x4000 and kept nonzero while the points are. The same
 * function sits in each of the three Colosso trial overlays. */
void Korosseo_SelectSoloCompetitor(s32 id)
{
    struct BattleUnit *unit;
    s32 v;
    s32 t;

    Engine_PartyRemoveActiveOwner(0);
    Engine_PartyRemoveActiveOwner(1);
    Engine_PartyRemoveActiveOwner(2);
    Engine_PartyRemoveActiveOwner(3);
    Engine_PartyRemoveActiveOwner(5);
    Engine_PartyAddActiveOwner(id);
    gGameState.selected_actor = id;
    Engine_ObjectAttachWorkTarget(id, 0);
    unit = Engine_OwnerGetState(id);
    unit->hp = unit->max_hp;
    unit->pp = unit->max_pp;
    unit->poison = 0;
    v = (unit->hp << 14) / unit->max_hp;
    t = 0x4000;
    if (v <= 0x4000) {
        t = 0;
        if (v >= 0)
            t = v;
    }
    unit->hp_gauge = t;
    if ((u16)t == 0 && unit->hp != 0) {
        unit->hp_gauge = 1;
    }
    v = (unit->pp << 14) / unit->max_pp;
    t = 0x4000;
    if (v <= 0x4000) {
        t = 0;
        if (v >= 0)
            t = v;
    }
    unit->pp_gauge = t;
    if ((u16)t == 0 && unit->pp != 0) {
        unit->pp_gauge = 1;
    }
    Engine_EventClearInvalidPackedValues();
}
