#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "FIXED_MATH.H"
#include "SCENE_IDS.H"
#include "SCENE.H"
#include "GAME_STATE.H"
#include "FX_SCENE.H"
#include "ANIMSPR.H"

extern u32 gFrameCount;
void BattleFx_SpawnDescendingArcParticles(void *);

extern char MsgAbilityWoreOff;
extern char MsgItemWoreOff;
void UiText_DrawQuantity(s32 arg0, s32 arg1);
void UiText_DrawMessage(void *arg0, s32 arg1);

/* Object updates of the paired arc effect (battle effect 16). */
void BattleFx_UpdateEffect16State(void)
{
    struct ArcEffectObject *object;
    struct AnimationObject *group;
    struct AnimationEntry *entry;
    s32 phase;

    object = ObjectTable_Get(gGameState.selected_actor);
    group = object->records;
    entry = group->entries[0];
    phase = gFrameCount % 5;
    if (phase == 0) {
        group->dirty = 1;
        group->flags = 3;
    } else if (phase == 2) {
        entry->param = 0;
        group->dirty = 1;
        group->flags = 1;
    }
}

/* Spawns the descending arc particles every seventh frame in the Clear
   scene and every fifth frame elsewhere; in phase 1 the arc also spins. */
void BattleFx_UpdatePairedArcSpawner(struct ArcEffectObject *object)
{
    s16 phase;
    s16 counter;

    phase = object->phase;
    counter = object->step++;

    if (gGameState.map == (s32)&SceneId_Clear) {
        if (__modsi3(counter, 7) == 0)
            BattleFx_SpawnDescendingArcParticles(object);
    } else if (__modsi3(counter, 5) == 0) {
        BattleFx_SpawnDescendingArcParticles(object);
    }

    if (phase == 1)
        object->angle += 0xC00;
}

void FieldEvent_ShowStatusMessage(void)
{
    gGameState.avoid_distance = 0;
    if (gGameState.avoid_by_item == 0) {
        UiText_DrawQuantity(0x96, 4);
        UiText_DrawMessage(&MsgAbilityWoreOff, 1);
        return;
    }
    UiText_DrawQuantity(0xEC, 2);
    UiText_DrawMessage(&MsgItemWoreOff, 1);
}
