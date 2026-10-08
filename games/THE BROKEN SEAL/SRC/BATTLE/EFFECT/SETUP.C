#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "GAME_STATE.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "IO_REG.H"
#include "CALLBACK_SCHEDULER.H"
#include "BATTLE_EFFECT_RUNTIME.H"
#include "FIELD_EVENT.H"

void Object_EnableEffectSpawnCallback(void)
{
    Scheduler_EnableCallbacks(Object_EffectSpawnCallback);
}

void Object_DisableEffectSpawnCallback(void)
{
    Scheduler_DisableCallbacks(Object_EffectSpawnCallback);
}

struct BattleEffectEntry *BattleFx_FindDefinition(u32 id)
{
    struct BattleEffectEntry *entry = BattleFx_DefinitionTable;
    u32 index = 0;

    if (entry->id != id) {
        do {
            index += 1;
            entry++;
            if (index > 0x81)
                break;
        } while (entry->id != id);
    }
    return entry;
}

s32 BattleFx_GetAnimationValue(void)
{
    struct BattleRenderObject *object = ObjectTable_Get();

    if (object->kind != 1 ||
        object->animation == NULL ||
        object->animation->value_28 == NULL) {
        return 0;
    }
    return *object->animation->value_28;
}

s32 BattleFx_GetResourceId(u32 id)
{
    u8 value;

    if (gGameState.speech == 0 ||
        (value = BattleFx_FindDefinition(id)->value) == 0xFF) {
        return 0;
    }
    return value + 0x100;
}

u8 BattleFx_GetFlags(void)
{
    return BattleFx_FindDefinition(BattleFx_GetAnimationValue())->flags;
}

extern volatile u8 gDebugMode;
extern volatile s32 gKeyState;

void Battle_UpdateModeFromShoulderButtons(void)
{
    struct BattleRuntime *runtime = (struct BattleRuntime *)gEventWork;

    if (gDebugMode != 0) {
        if (gKeyState & KEY_L) {
            runtime->mode_1cc = 0;
        }
        if (gKeyState & KEY_R) {
            runtime->mode_1cc = -1;
        }
    }
}

void Battle_WaitMode0(s32 frames)
{
    if (((struct BattleRuntime *)gEventWork)->mode_1cc == 0 && frames != 0) {
        WaitFrames(frames);
    }
}

void Battle_InitializeRenderObject(void)
{
    struct BattleRenderObject *object;

    object = ObjectTable_Get(gGameState.selected_actor);
    object->unknown_30 = 0x10000;
    object->unknown_34 = 0x8000;
    object->unknown_38 = 0x80000000;
    object->unknown_40 = 0x80000000;
    object->unknown_24 = 0;
    object->unknown_2c = 0;
    if (gGameState.movement_mode == 1) {
        Object_SetMode((s32)object, 0xC);
        return;
    }
    Object_SetMode((s32)object, 1);
}

void Battle_ResetEffectCounter(void);

void Battle_Reset(void)
{
    struct BattleRuntime *runtime = (struct BattleRuntime *)gEventWork;

    UiTimedNotice_CloseIfActiveFar();
    Battle_InitializeRenderObject();
    if (runtime->unknown_cb6 != 0) {
        Battle_ResetEffectCounter();
    }
    {
        s32 zero = 0;
        runtime->unknown_cc2 = zero;
        runtime->unknown_cc4 = zero;
        runtime->unknown_1c8 = 0x10;
        runtime->mode_1cc = zero;
        runtime->unknown_1da = 0xFFFF;
        runtime->unknown_1dc = -1;
        runtime->unknown_1de = -1;
        Scheduler_AddOrUpdateCallback((const void *)Battle_UpdateModeFromShoulderButtons, 0xC80);
        GameFlag_ClearBitFar(0x132);
        runtime->object_id = gGameState.selected_actor;
        runtime->unknown_1f8 = zero;
    }
}

void BattleFx_FinishAction(void)
{
    Scheduler_RemoveCallback(Battle_UpdateModeFromShoulderButtons);
    Object_AttachWorkTargetToObject(gGameState.selected_actor, 1);
    GameFlag_RefreshLureCapFar();
}
