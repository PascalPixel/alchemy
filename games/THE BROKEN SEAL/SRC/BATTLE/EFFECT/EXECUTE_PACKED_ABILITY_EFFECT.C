#include "EFFECT_RUNTIME.H"
#include "OBJECT_LOOKUP.H"
#include "BATTLE_EFFECT_RUNTIME.H"

/* Data_02000240 is struct BattleWork (battle_effect_runtime.h); its
 * object_id field sits at the same 0x1f4 offset this owner reads as
 * selected_object, matching main:0808e23c's use of the same shared symbol. */

u8 *Ability_GetData(s32);
s32 Func_0808e4b4(s32, s32, void *);
#define BattleFx_FindMatchingEvent Func_0808e4b4
void Func_08096fb0(s32, s32);
void Func_080970f8(s32, s32);
#define BattleFx_SetupObjectPair Func_080970f8
s32 Func_08096b28(void *, s32, s32);
#define BattleFx_RunEventAction Func_08096b28
void FieldEvent_RunTypeHandler(void);
void EffectRuntime_StopCurrentObject(void);
void Func_08097194(void);
#define BattleEffect_CleanupSceneObjects Func_08097194

s32 BattleFx_ExecutePackedAbilityEffect(s32 packed)
{
    s32 output;
    s32 index;
    s32 mode;
    u8 object;
    void *first;
    void *second;

    index = packed & 0x3FF;
    mode = ((u32)packed >> 10) & 0xF;
    object = Ability_GetData(index)[0xC];
    ObjectTable_Get(Data_02000240.object_id);
    first = (void *)BattleFx_FindMatchingEvent(0x30000005, object, &output);
    second = (void *)BattleFx_FindMatchingEvent(0x20000005, object, &output);
    Func_08096fb0(index, 0);
    BattleFx_SetupObjectPair(Data_02000240.object_id, output);
    BattleFx_RunEventAction(first, mode, output);
    FieldEvent_RunTypeHandler();
    EffectRuntime_StopCurrentObject();
    BattleFx_RunEventAction(second, mode, output);
    BattleEffect_CleanupSceneObjects();
    return 0;
}
