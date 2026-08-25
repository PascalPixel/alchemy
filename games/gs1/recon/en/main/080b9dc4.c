#include "battle_party.h"
#include "battle_types.h"
#include "types.h"

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

struct BattleUnit *Func_08077008(s32 unit_id);
void Func_080c10e8(u16 *, s32);
void Func_080151c8(s32 message_id);
void Func_080bb65c(void);
void Func_080b8064(s32 unit_id);
void Func_080030f8(s32 frames);
u32 Func_08004458(void);
void Func_080bac6c(s32 unit_id);
void Func_080b7e60(s32 unit_id);

s32 Func_080b9dc4(struct BattleTrigger *trigger)
{
    struct BattlePresentationState *presentation =
        *(struct BattlePresentationState **)0x03001f00;
    struct BattleSceneContext *scene =
        *(struct BattleSceneContext **)0x03001e74;
    s32 completed = 0;

    presentation->mode = 0x2000;
    presentation->active = 1;
    Func_080c10e8(0, 0);

    if (trigger->unit_id <= 7) {
        if (scene->encounter_mode == 2) {
            Func_080151c8(0x847);
            Func_080bb65c();
        } else {
            u16 unit_ids[14];
            s32 index = BattleParty_ListLivingUnits(1, unit_ids) - 1;

            while (index >= 0) {
                struct BattleUnit *unit = Func_08077008(unit_ids[index]);

                if (unit->stun == 0 && unit->sleep == 0) {
                    Func_080b8064(unit_ids[index]);
                    Func_080030f8(8);
                }
                index--;
            }
            Func_080030f8(22);
            completed = 1;
        }
    } else if (((Func_08004458() * 10) >> 16) <= 6) {
        s16 event[2];

        event[0] = trigger->unit_id;
        event[1] = 0xff;
        Func_080b8064(event[0]);
        Func_080030f8(8);
        Func_080bac6c(trigger->unit_id);
        Func_080b7e60(trigger->unit_id);
    } else {
        Func_080151c8(0x847);
        Func_080bb65c();
    }

    presentation->active = 0;
    return completed;
}
