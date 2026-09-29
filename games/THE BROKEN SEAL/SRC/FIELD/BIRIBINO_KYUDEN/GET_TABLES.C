#include "KYUDEN.H"

extern u8 gBiribinoKyudenPlacements[];
extern const struct ScenePlacement gBiribinoKyudenPlacementsOther[];
extern const struct SceneEvent gBiribinoKyudenEvents[];
extern const struct SceneEvent gBiribinoKyudenEventsOther[];

void FieldScene_PrepareActors(u8 *placements);

struct SceneRecord {
    u8 unk_000[166];
    u8 field_166;
    u8 unk_167[23];
    u8 field_190;
    u8 unk_191[23];
    u8 field_214;
    u8 unk_215[23];
    u8 field_238;
};

/* The actors placed in the palace. Taking the reward changes four entries
   of the palace's table, which is prepared first. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    u8 *p;

    /* A signed halfword read. */
    if (gGameState.scene == (s32)&SceneId_BiribinoKyuden) {
        p = gBiribinoKyudenPlacements;
        FieldScene_PrepareActors(p);

        if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
            struct SceneRecord *rec = (struct SceneRecord *)p;

            rec->field_166 = 2;
            rec->field_190 = 0;
            rec->field_214 = 3;
            rec->field_238 = 1;
        }

        return (const struct ScenePlacement *)p;
    }
    return gBiribinoKyudenPlacementsOther;
}

/* What the palace answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_BiribinoKyuden) {
        return gBiribinoKyudenEvents;
    }
    return gBiribinoKyudenEventsOther;
}
