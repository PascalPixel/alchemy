#include "types.h"
#include "iwram_call.h"
#include "object_runtime.h"
#include "field_sprite.h"

extern const u8 gData_02004f90[];
extern const u8 gData_02004f9c[];

u32                   Random16Far(void);
void                  Vector_AddPolarOffsetFar(s32 angle, s32 distance, s32 *position);
struct ObjectRuntime *Func_02004b30(s32 kind, s32 x, s32 y, s32 z);
void                  Func_02004b20(struct ObjectRuntime *object, const u8 *script);
void                  Animation_ApplyChildArgument(void *sprite, s32 argument);

static __inline__ struct ObjectRuntime *Func_02004b30_(s32 kind, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: forwarding through the inline builds the kind after the position loads, as in the native (direct: differ 4). */
    return Func_02004b30(kind, x, y, z);
}

// Spawns a pair of sparks at the object, as ☀️'s MercuryLighthouse_SpawnSparkPair
// does at a spawn point.
void Func_020039f4(struct ObjectRuntime *object)
{
    struct ObjectRuntime *spark;
    struct FieldSprite   *sprite;
    s32 (*mulQ16)(s32 a, s32 b) = Iwram_MulQ16;

    /* FAKEMATCH: through a local pointer the resident multiply's entry loads before the factor's shift, as in the native (direct: differ 2; its result kept in a local: 2). */
    mulQ16(object->scaleX, 0x18000);
    spark = Func_02004b30_(271, object->x, object->y, object->z);
    if (spark != NULL)
    {
        spark->terrainHeight = object->terrainHeight;
        sprite = spark->animation;
        Func_02004b20(spark, gData_02004f90);
        spark->motionFlags = 0;
        spark->action = 0;
        spark->linkedObject = object;
        if (sprite != NULL)
        {
            Animation_ApplyChildArgument(sprite, 2);
            sprite->nextFrame = 0;
            sprite->priority = 1;
        }
    }
    spark = Func_02004b30_(271, object->x, object->y, object->z);
    if (spark != NULL)
    {
        spark->terrainHeight = object->terrainHeight;
        sprite = spark->animation;
        Func_02004b20(spark, gData_02004f90);
        spark->motionFlags = 0;
        spark->action = 0;
        spark->linkedObject = object;
        spark->terrainId = 1;
        if (sprite != NULL)
        {
            Animation_ApplyChildArgument(sprite, 1);
            sprite->nextFrame = 0;
        }
    }
}

// Spawns a particle at a random spot around the object.
void Func_02003aac(struct ObjectRuntime *object)
{
    s32                   position[3];
    s32                   angle;
    struct ObjectRuntime *particle;
    struct FieldSprite   *sprite;

    Iwram_MulQ16(object->scaleX, 0x18000);
    position[0] = object->x;
    position[1] = object->y + 0x20000;
    position[2] = object->z;
    angle = Random16Far();
    Vector_AddPolarOffsetFar(angle * 20, Random16Far(), position);
    particle = Func_02004b30(674, position[0], position[1], position[2]);
    if (particle != NULL)
    {
        particle->terrainHeight = object->terrainHeight;
        sprite = particle->animation;
        Func_02004b20(particle, gData_02004f9c);
        Animation_ApplyChildArgument(sprite, 2);
        particle->gravity = 0x200;
        particle->scaleX = 0xcccc;
        particle->scaleY = 0xcccc;
        if (sprite != NULL)
            sprite->nextFrame = 0;
    }
}
