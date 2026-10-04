/* Scene_ResolveInteractionResult (☀️ name; it picks the scene's cue): differ 2.
   The native loads the rule table's address before building 0x1e2 for the
   entrance; this draft loads it after. Declaration order and types, the
   loop's initializer and 200 device-search steps did not move it. SongFlag_Set
   (081c11ac) misplaces a pool load around a built constant the same way.
   Appends to DATA_LOOKUP_ROUTINES.C on a match. */
#include "TYPES.H"
#include "PARTY_STATE.H"

s32 GameFlag_Test(s32 flag);

/* A scene's cue: the scene or its area, an entrance (-1 for any) and a
   flag (-1 for none) that must be set. */
struct SceneCueRule {
    s16 id;
    s16 entrance : 15;
    u16 by_scene : 1;
    s16 flag;
    s16 cue;
};

extern const struct SceneCueRule Data_080ef094[];

/* ☀️'s Scene_ResolveInteractionResult: picks the cue of the first rule
   that matches the scene, or of its area, and the entrance; 79 when none
   does. */
void Scene_ResolveInteractionResult(void)
{
    s16 cue = 79;
    s16 scene = gPartyState.scene;
    s16 entrance = gPartyState.entrance;
    s16 group = gPartyState.scene_group;
    const struct SceneCueRule *rule = Data_080ef094;

    for (; rule->id != -1; rule++) {
        if (rule->by_scene) {
            if (rule->id != scene)
                continue;
            if (rule->entrance != -1 && rule->entrance != entrance)
                continue;
            if (rule->flag != -1 && GameFlag_Test(rule->flag) == 0)
                continue;
            cue = rule->cue;
            break;
        } else {
            if (rule->id != group)
                continue;
            if (rule->entrance != -1 && rule->entrance != entrance)
                continue;
            if (rule->flag != -1 && GameFlag_Test(rule->flag) == 0)
                continue;
            cue = rule->cue;
            break;
        }
    }

    gPartyState.cue = cue;
}
