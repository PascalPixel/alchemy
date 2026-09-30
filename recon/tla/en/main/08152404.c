#include "TYPES.H"
#include "SCENE.H"

/* battle/effects/common/spawn_objects.c */
typedef struct {
    u8 reserved_00[9];
    u8 flags09_0 : 2;
    u8 variant : 2;
    u8 flags09_4 : 4;
    u8 reserved_0a[28];
    u8 enabled;
} BattleEffectObject;

extern u32 gBattleFxWork;


BattleEffectObject *GetBattleEffectObject(s32);
void AnimationObjects_SelectAnimationFar(BattleEffectObject *, s32);

void BattleFx_SpawnObjects(s32 entry_count, s32 kind, u32 variant)
{
    u32 base = gBattleFxWork;
    s32 entry_index = 0;
    u32 offset;

    if (entry_count == 0) {
        return;
    }
    /* The object pointers start 0x77d8 bytes into the effect work. */
    offset = 0x77d8;
    do {
        BattleEffectObject *object = GetBattleEffectObject(kind);

        *(BattleEffectObject **)(offset + base) = object;
        if (object != 0) {
            object->enabled = 0;
            AnimationObjects_SelectAnimationFar(object, entry_index);
            (*(BattleEffectObject **)(offset + base))->variant = variant;
        }
        entry_index++;
        offset += 4;
    } while (entry_index != entry_count);
}
