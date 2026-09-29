#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement KuupuappuHeya_SceneTableA[];
extern const struct ScenePlacement KuupuappuHeya_SceneTableB[];

void FieldScene_PrepareActors(const struct ScenePlacement *placements);

/* The actors placed in the Vault houses: entrances 15 to 17 have their own
   table. The chosen table is prepared before it is returned. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    const struct ScenePlacement *table;
    /* FAKEMATCH: a temporary holding the first entrance keeps the compiler
       from rewriting the test as greater than 14. */
    s32 first = 15;

    if (gGameState.entrance <= 17) {
        if (gGameState.entrance >= first) {
            table = KuupuappuHeya_SceneTableB;
            goto prepare;
        }
    }
    table = KuupuappuHeya_SceneTableA;
prepare:
    FieldScene_PrepareActors(table);
    return table;
}
