#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "FIXED_MATH.H"
#include "SCENE_IDS.H"
#include "SCENE.H"
#include "GAME_STATE.H"
#include "FX_SCENE.H"
#include "ANIMSPR.H"
#include "text/MSG_IDS.H"

#define BattleFx_SpawnDescendingArcParticles ((void (*)(void*))BattleFx_SpawnDescendingArcParticles)
#define UiText_DrawMessage ((void (*)(void*,s32))UiText_DrawMessage)

extern u32 gFrameCount;

void UiText_DrawQuantity(s32 arg0, s32 arg1);

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
        object->angle += 3072;
}

void FieldEvent_ShowStatusMessage(void)
{
    gGameState.avoid_distance = 0;
    if (gGameState.avoid_by_item == 0) {
        UiText_DrawQuantity(150, 4);
        UiText_DrawMessage(MsgAbilityWoreOff, 1);
        return;
    }
    UiText_DrawQuantity(236, 2);
    UiText_DrawMessage(MsgItemWoreOff, 1);
}
