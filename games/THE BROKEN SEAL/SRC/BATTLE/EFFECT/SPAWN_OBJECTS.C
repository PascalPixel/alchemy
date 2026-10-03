#include "BATTLE_EFFECT_WORK.H"
#include "HEAP_STATE.H"
#include "ANIMSPR.H"
#include "CANVAS.H"
#include "TYPES.H"
#include "SCENE.H"


/* The first OAM part's existing byte attribute view; not an allocation owner. */
struct AnimationAttribute {
    u8 unknown_00[9];
    u8 low : 2;
    u8 variant : 2;
    u8 high : 4;
};


extern struct BattleEffectWork *gBattleFxWork;

/* battle/effects/common/spawn_objects.c */

struct AnimationObject *GetBattleEffectObject(s32);
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *, s32);

void BattleFx_SpawnObjects(s32 entry_count, s32 kind, u32 variant)
{
    /* FAKEMATCH: the existing packed byte9 store keeps the signed mask live across calls; the ordinary byte mask shortened the object loop by eight bytes. */
    struct BattleEffectWork *work = gBattleFxWork;
    s32 entry_index = 0;


    if (entry_count == 0) {
        return;
    }
    do {
        struct AnimationObject *object = GetBattleEffectObject(kind);

        work->objects[entry_index] = object;
        if (object != 0) {
            object->flags = 0;
            AnimationObjects_SelectAnimationFar(object, entry_index);
            /* The two-bit variant is written in byte 9. */
            ((struct AnimationAttribute *)work->objects[entry_index])->variant = variant;
        }
        entry_index++;
    } while (entry_index != entry_count);
}

/* battle/effects/common/no_effect.c */
void BattleFx_RunNoEffect(void)
{
}
