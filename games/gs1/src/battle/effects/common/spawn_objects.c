#include "types.h"

typedef struct {
    u8 reserved_00[9];
    u8 flags09_0 : 2;
    u8 variant : 2;
    u8 flags09_4 : 4;
    u8 reserved_0a[28];
    u8 enabled;
} BattleEffectObject;

extern u32 Data_03001eec;
extern u8 Value_000077d8;
#define GetBattleEffectObject Func_08009030

#define InitBattleEffectObject Object_InitializeMode

BattleEffectObject *GetBattleEffectObject(s32);
void InitBattleEffectObject(BattleEffectObject *, s32);

void BattleEffect_SpawnObjects(s32 entry_count, s32 kind, u32 variant) {
    u32 base = Data_03001eec;
    s32 entry_index = 0;
    u32 offset;

    if (entry_count == 0) {
        return;
    }
    offset = (u32)&Value_000077d8;
    do {
        BattleEffectObject *object = GetBattleEffectObject(kind);

        *(BattleEffectObject **)(offset + base) = object;
        if (object != 0) {
            object->enabled = 0;
            InitBattleEffectObject(object, entry_index);
            (*(BattleEffectObject **)(offset + base))->variant = variant;
        }
        entry_index++;
        offset += 4;
    } while (entry_index != entry_count);
}
