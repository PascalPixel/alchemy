#include "TYPES.H"

s32 Scheduler_EnableCallbacks(u32 callback);
void Object_EffectSpawnCallback(void);

s32 BattleFx_GetResourceId(u32 id)
{
    u8 value;

    if (Data_02000240.enabled_20a == 0 ||
        (value = BattleFx_FindDefinition(id)->value) == 0xFF) {
        return 0;
    }
    return value + 0x100;
}
