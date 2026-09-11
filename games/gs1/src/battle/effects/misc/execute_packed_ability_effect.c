#include "scene.h"
#include "abi/battle/effects/misc/execute_packed_ability_effect.h"
#include "effect_runtime.h"
#include "object_lookup.h"
#include "battle_effect_runtime.h"

/* gCell is struct BattleWork (battle_effect_runtime.h); its
 * object_id field sits at the same 0x1f4 offset this owner reads as
 * selected_object, matching main:0808e23c's use of the same shared symbol. */

u8 *Ability_GetData(s32);

void BattleFx_SetupObjectPair(s32, s32);

void FieldEvent_RunTypeHandler(void);
void EffectRuntime_StopCurrentObject(void);

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
    ObjectTable_Get(gCell.object_id);
    first = (void *)Battle_Place(0x30000005, object, &output);
    second = (void *)Battle_Place(0x20000005, object, &output);
    Battle_Apply(index, 0);
    BattleFx_SetupObjectPair(gCell.object_id, output);
    Battle_Place2(first, mode, output);
    FieldEvent_RunTypeHandler();
    EffectRuntime_StopCurrentObject();
    Battle_Place2(second, mode, output);
    Battle_Run();
    return 0;
}
