#include "types.h"
#include "battle_effect_runtime.h"

s32 Func_080042c8(u32 callback);
void Func_080912b8(void);

void Object_EnableEffectSpawnCallback(void)
{
    Func_080042c8((u32)Func_080912b8);
}

s32 Func_0800439c(u32 callback);
void Func_080912b8(void);

void Object_DisableEffectSpawnCallback(void)
{
    Func_0800439c((u32)Func_080912b8);
}

extern struct BattleEffectEntry Data_0809e9f0[];

struct BattleEffectEntry *BattleFx_FindDefinition(u32 id)
{
    struct BattleEffectEntry *entry = Data_0809e9f0;
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

struct BattleEffectEntry *Func_08091560(u32 id);

s32 BattleFx_GetResourceId(u32 id)
{
    u8 value;

    if (Data_02000240.enabled_20a == 0 ||
        (value = Func_08091560(id)->value) == 0xFF) {
        return 0;
    }
    return value + 0x100;
}

struct BattleEffectEntry *Func_08091560(u32 id);
s32 Func_08091584(void);

u8 BattleFx_GetFlags(void)
{
    return Func_08091560(Func_08091584())->flags;
}

extern volatile u8 Data_03001f54;
extern volatile s32 Data_03001c94;

void Battle_UpdateModeFromShoulderButtons(void)
{
    struct BattleRuntime *runtime = Data_03001ebc;

    /*
     * 0x03001c94 is the button latch. 0x200 is L and 0x100 is R in the GBA
     * key layout, so the two arms are the shoulder buttons setting the mode
     * word either way round.
     */
    if (Data_03001f54 != 0) {
        if (Data_03001c94 & 0x200) {
            runtime->mode_1cc = 0;
        }
        if (Data_03001c94 & 0x100) {
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

void Func_08015208(void);
void Func_08091660(void);
void Func_0808e118(void);
void ScheduleCallbackAfterFrames(const void *, s32);
void Func_080915ec(void);
u32 GameFlag_Clear(s32);

void Battle_Reset(void)
{
    struct BattleRuntime *runtime = Data_03001ebc;

    Func_08015208();
    Func_08091660();
    if (runtime->unknown_cb6 != 0) {
        Func_0808e118();
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
        ScheduleCallbackAfterFrames((const void *)Func_080915ec, 0xC80);
        GameFlag_Clear(0x132);
        runtime->object_id = Data_02000240.object_id;
        runtime->unknown_1f8 = zero;
    }
}
