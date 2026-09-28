#include "TYPES.H"
#include "BATTLE_EFFECT_RUNTIME.H"

s32 Scheduler_EnableCallbacks(u32 callback);
void Object_EffectSpawnCallback(void);

void Object_EnableEffectSpawnCallback(void)
{
    Scheduler_EnableCallbacks((u32)Object_EffectSpawnCallback);
}

s32 Scheduler_DisableCallbacks(u32 callback);
void Object_EffectSpawnCallback(void);

void Object_DisableEffectSpawnCallback(void)
{
    Scheduler_DisableCallbacks((u32)Object_EffectSpawnCallback);
}

extern struct BattleEffectEntry BattleFx_DefinitionTable[];

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

    if (Data_02000240.enabled_20a == 0 ||
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
    struct BattleRuntime *runtime = Data_03001ebc;

    /*
     * 0x03001c94 is the button latch. 0x200 is L and 0x100 is R in the GBA
     * key layout, so the two arms are the shoulder buttons setting the mode
     * word either way round.
     */
    if (gDebugMode != 0) {
        if (gKeyState & 0x200) {
            runtime->mode_1cc = 0;
        }
        if (gKeyState & 0x100) {
            runtime->mode_1cc = -1;
        }
    }
}

void WaitFrames(void);

void Battle_WaitMode0(s32 should_wait)
{
    if (Data_03001ebc->mode_1cc == 0 && should_wait != 0) {
        WaitFrames();
    }
}

s32 Object_SetMode(s32, s32);

void Battle_InitializeRenderObject(void)
{
    struct BattleRenderObject *object;

    object = ObjectTable_Get(Data_02000240.object_id);
    object->unknown_30 = 0x10000;
    object->unknown_34 = 0x8000;
    object->unknown_38 = 0x80000000;
    object->unknown_40 = 0x80000000;
    object->unknown_24 = 0;
    object->unknown_2c = 0;
    if (Data_02000240.mode_1f2 == 1) {
        Object_SetMode((s32)object, 0xC);
        return;
    }
    Object_SetMode((s32)object, 1);
}

void UiTimedNotice_CloseIfActiveFar(void);
void Battle_InitializeRenderObject(void);
void Battle_ResetEffectCounter(void);
void Scheduler_AddOrUpdateCallback(const void *, s32);
u32 GameFlag_ClearBitFar(s32);

void Battle_Reset(void)
{
    struct BattleRuntime *runtime = Data_03001ebc;

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
        runtime->object_id = Data_02000240.object_id;
        runtime->unknown_1f8 = zero;
    }
}

void Scheduler_RemoveCallback(u32);
void GameFlag_RefreshLureCapFar(void);
void Object_AttachWorkTargetToObject(s32 value, s32 enabled);

void BattleFx_FinishAction(void)
{
    Scheduler_RemoveCallback((u32)Battle_UpdateModeFromShoulderButtons);
    Object_AttachWorkTargetToObject(Data_02000240.object_id, 1);
    GameFlag_RefreshLureCapFar();
}
