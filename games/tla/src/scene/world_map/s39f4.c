#include "types.h"
#include "system.h"
#include "iwram_call.h"
#include "actor.h"
#include "field_sprite.h"
#include "macros.h"
#include "event_inline.h"

extern s32 gData_02004f90[];
extern u32 gData_02004f9c[];

void Func_02004b20(struct Actor *object, const s32 *script);
void Animation_ApplyChildArgument(void *sprite, s32 argument);

static __inline__ struct Actor *Func_02004b30_(s32 kind, s32 x, s32 y,
                                                       s32 z) {
  /* FAKEMATCH(wrapper): forwarding through the inline builds the kind after the
   * position loads, as in the native; as a direct call, 4 of 77 instructions
   * of Func_020039f4 differ in tla-en. */
  return ((struct Actor *)Import_ActorCreate(kind, x, y, z));
}

/// Spawns a pair of sparks at the object.
// As ☀️'s MercuryLighthouse_SpawnSparkPair does at a spawn point.
void Func_020039f4(struct Actor *object) {
  struct Actor *spark;
  struct FieldSprite *sprite;
  s32 (*mulQ16)(s32 a, s32 b) = Iwram_MulQ16;

  /* FAKEMATCH(temporary): the resident multiply called through a local pointer,
   * so its entry loads before the factor's shift, as in the native; as a direct
   * call, 2 of 77 instructions differ in tla-en. */
  mulQ16(object->scaleX, 0x18000);
  spark = Func_02004b30_(271, object->x, object->y, object->z);
  if (spark != NULL) {
    spark->terrainHeight = object->terrainHeight;
    sprite = spark->sprite;
    Func_02004b20(spark, gData_02004f90);
    spark->motionFlags = 0;
    spark->timer = 0;
    spark->watchedObject = object;
    if (sprite != NULL) {
      Animation_ApplyChildArgument(sprite, 2);
      sprite->nextFrame = 0;
      sprite->priority = 1;
    }
  }
  spark = Func_02004b30_(271, object->x, object->y, object->z);
  if (spark != NULL) {
    spark->terrainHeight = object->terrainHeight;
    sprite = spark->sprite;
    Func_02004b20(spark, gData_02004f90);
    spark->motionFlags = 0;
    spark->timer = 0;
    spark->watchedObject = object;
    spark->terrainId = 1;
    if (sprite != NULL) {
      Animation_ApplyChildArgument(sprite, 1);
      sprite->nextFrame = 0;
    }
  }
}

/// Spawns a particle at a random spot around the object.
void Func_02003aac(struct Actor *object) {
  s32 position[3];
  s32 angle;
  struct Actor *particle;
  struct FieldSprite *sprite;

  Iwram_MulQ16(object->scaleX, 0x18000);
  SET_VECTOR(position, object->x, object->y + 0x20000, object->z);
  angle = Random16Far();
  FieldVector_AddPolarOffset(angle * 20, Random16Far(), position);
  particle = (FieldActor_Create(674, position[0], position[1], position[2]));
  if (particle != NULL) {
    particle->terrainHeight = object->terrainHeight;
    sprite = particle->sprite;
    Func_02004b20(particle, gData_02004f9c);
    Animation_ApplyChildArgument(sprite, 2);
    particle->gravity = 0x200;
    particle->scaleX = 0xcccc;
    particle->scaleY = 0xcccc;
    if (sprite != NULL)
      sprite->nextFrame = 0;
  }
}
