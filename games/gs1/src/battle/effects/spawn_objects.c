#include "types.h"
#include "scene.h"

/* battle/effects/common/spawn_objects.c */
typedef struct {
    u8 reserved_00[9];
    u8 flags09_0 : 2;
    u8 variant : 2;
    u8 flags09_4 : 4;
    u8 reserved_0a[28];
    u8 enabled;
} BattleEffectObject;

extern u32 gIw;
extern u8 Value_000077d8;

#define InitBattleEffectObject Object_InitializeMode

BattleEffectObject *GetBattleEffectObject(s32);
void InitBattleEffectObject(BattleEffectObject *, s32);

void BattleFx_SpawnObjects(s32 entry_count, s32 kind, u32 variant)
{
    u32 base = gIw;
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

/* battle/effects/common/no_effect.c */
void BattleFx_RunNoEffect(void)
{
}

/* battle/effects/series_i/mode_0.c */
s32 BattleEffectI(s32, s32);

void BattleFx_RunSeriesIMode0(s32 effect)
{
    BattleEffectI(effect, 0);
}

/* battle/effects/series_i/mode_2.c */

void BattleFx_RunSeriesIMode2(s32 effect)
{
    BattleEffectI(effect, 2);
}

/* battle/effects/series_i/mode_6.c */

void BattleFx_RunSeriesIMode6(s32 effect)
{
    BattleEffectI(effect, 6);
}

/* battle/effects/series_i/mode_3.c */

void BattleFx_RunSeriesIMode3(s32 effect)
{
    BattleEffectI(effect, 3);
}

/* battle/effects/series_i/mode_5.c */

void BattleFx_RunSeriesIMode5(s32 effect)
{
    BattleEffectI(effect, 5);
}

/* battle/effects/series_i/mode_7.c */

void BattleFx_RunSeriesIMode7(s32 effect)
{
    BattleEffectI(effect, 7);
}

/* battle/effects/series_i/mode_4.c */

void BattleFx_RunSeriesIMode4(s32 effect)
{
    BattleEffectI(effect, 4);
}
