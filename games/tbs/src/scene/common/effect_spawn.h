#include "types.h"
#include "field_effect.h"
#include "objdisp.h"
#include "field_scene.h"

void BattleFx_UpdateObjectMotionScaleAndLinkedAngle(union FieldObject *object)
{
    object->effect.x += object->effect.velocityX;
    object->effect.y += object->effect.velocityY;
    object->effect.z += object->effect.velocityZ;
    object->effect.scaleX += object->effect.scaleRateX;
    object->effect.scaleY += object->effect.scaleRateY;
    object->effect.sprite->rotation += object->effect.spin;
}

// 効果オブジェクトを出す関数の本体。常駐側（遠隔呼び出し）とオーバーレイ側（直接呼び出し）で
// 同じ本体を 1 回ずつコンパイルする

extern const s32 *gEffectScripts[];

enum
{
    EFFECT_DEFAULT_TYPE = 222,
    EFFECT_SCALE_ONE = 0x10000,
    EFFECT_OPTION_MASK = 0xffff0000,
    EFFECT_SCRIPT_REPEAT_COUNT = 3
};

#define EFFECT_SPAWN(name, create, set_mode, set_script)                                           \
    void name(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags,         \
        const struct EffectOptions *options)                                                       \
    {                                                                                              \
        struct FieldActor  *party = Actor_Get(ACTOR_PARTY_LEADER);                                 \
        struct FieldEffect *effect;                                                                \
        struct FieldSprite *sprite;                                                                \
        const s32          *script;                                                                \
                                                                                                   \
        if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)                                        \
            effect = (struct FieldEffect *)create(options->type, x, y, z);                         \
        else                                                                                       \
            effect = (struct FieldEffect *)create(EFFECT_DEFAULT_TYPE, x, y, z);                   \
        if (effect == 0)                                                                           \
            return;                                                                                \
                                                                                                   \
        sprite = effect->sprite;                                                                   \
        set_mode((struct FieldActor *)effect, (flags + 1) & EFFECT_SCRIPT_MASK);                   \
        set_script((void *)effect, gEffectScripts[flags & EFFECT_SCRIPT_MASK]);                    \
        effect->motionFlags = 0;                                                                   \
        sprite->flags = 0;                                                                         \
        effect->update = BattleFx_UpdateObjectMotionScaleAndLinkedAngle;                           \
        effect->velocityX = velocityX;                                                             \
        effect->velocityY = velocityY;                                                             \
        effect->velocityZ = velocityZ;                                                             \
        sprite->priority = party->sprite->priority;                                                \
        effect->scaleRateX = 0;                                                                    \
        effect->scaleRateY = 0;                                                                    \
        effect->spin = 0;                                                                          \
                                                                                                   \
        if ((flags & EFFECT_OPTION_MASK) == 0 || options == 0)                                     \
            return;                                                                                \
        if ((flags & EFFECT_USE_PALETTE) != 0)                                                     \
            ObjectGroup_SetChildValue((struct FieldActor *)effect, options->palette);              \
        if ((flags & EFFECT_USE_PRIORITY) != 0)                                                    \
        {                                                                                          \
            effect->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;                                    \
            sprite->priority = options->priority;                                                  \
        }                                                                                          \
        if ((flags & EFFECT_USE_START_SCALE) != 0)                                                 \
        {                                                                                          \
            effect->scaleX = options->startScaleX;                                                 \
            effect->scaleY = options->startScaleY;                                                 \
        }                                                                                          \
        if ((flags & EFFECT_SCALE_TO_TARGET) != 0)                                                 \
        {                                                                                          \
            script = gEffectScripts[flags & EFFECT_SCRIPT_MASK];                                   \
            if ((flags & EFFECT_USE_START_SCALE) != 0)                                             \
            {                                                                                      \
                effect->scaleRateX =                                                               \
                    (options->targetScaleX - effect->scaleX) / script[EFFECT_SCRIPT_REPEAT_COUNT]; \
                effect->scaleRateY =                                                               \
                    (options->targetScaleY - effect->scaleY) / script[EFFECT_SCRIPT_REPEAT_COUNT]; \
            }                                                                                      \
            else                                                                                   \
            {                                                                                      \
                effect->scaleRateX = (options->targetScaleX - EFFECT_SCALE_ONE)                    \
                                     / script[EFFECT_SCRIPT_REPEAT_COUNT];                         \
                effect->scaleRateY = (options->targetScaleY - EFFECT_SCALE_ONE)                    \
                                     / script[EFFECT_SCRIPT_REPEAT_COUNT];                         \
            }                                                                                      \
        }                                                                                          \
        if ((flags & EFFECT_USE_SCRIPT) != 0)                                                      \
        {                                                                                          \
            set_mode((struct FieldActor *)effect, 1);                                              \
            set_script((void *)effect, options->script);                                           \
        }                                                                                          \
        if ((flags & EFFECT_USE_ROTATION) != 0)                                                    \
            sprite->rotation = options->rotation;                                                  \
        if ((flags & EFFECT_USE_SPIN) != 0)                                                        \
            effect->spin = options->spin;                                                          \
        if ((flags & EFFECT_USE_UPDATE) != 0)                                                      \
            effect->update = options->update;                                                      \
    }

#ifdef EFFECT_SPAWN_RESIDENT
// 常駐版: エンジンを遠隔呼び出しで使う
EFFECT_SPAWN(Effect_SpawnResident, Object_CreateFar, ObjectDispatch_ApplyArgumentToChildrenFar,
    ObjectDispatch_InitializeFar)
#else
// オーバーレイに 1 つずつ入る版
EFFECT_SPAWN(Effect_Spawn, Object_Create, Object_SetMode, Object_SetScript)
#endif
