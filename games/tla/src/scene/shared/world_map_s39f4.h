#include "types.h"
#include "system/system.h"
#include "system/iwram_call.h"
#include "actor.h"
#include "field_sprite.h"
#include "macros.h"
#include "field/event_inline.h"
#include "actor_view.h"

extern s32 gData_02004f90[];
extern u32 gData_02004f9c[];

void Actor_StartScript(struct Actor *object, const s32 *script);

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
  spark = FieldActor_Create(271, object->x, object->y, object->z);
  if (spark != NULL) {
    spark->terrainHeight = object->terrainHeight;
    sprite = spark->sprite;
    Actor_StartScript(spark, gData_02004f90);
    spark->motionFlags = 0;
    spark->timer = 0;
    spark->watchedObject = object;
    if (sprite != NULL) {
      Animation_ApplyChildArgument(sprite, 2);
      sprite->nextFrame = 0;
      sprite->priority = 1;
    }
  }
  spark = FieldActor_Create(271, object->x, object->y, object->z);
  if (spark != NULL) {
    spark->terrainHeight = object->terrainHeight;
    sprite = spark->sprite;
    Actor_StartScript(spark, gData_02004f90);
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
  angle = Random16();
  FieldVector_AddPolarOffset(angle * 20, Random16(), position);
  particle = (FieldActor_Create(674, position[0], position[1], position[2]));
  if (particle != NULL) {
    particle->terrainHeight = object->terrainHeight;
    sprite = particle->sprite;
    Actor_StartScript(particle, gData_02004f9c);
    Animation_ApplyChildArgument(sprite, 2);
    particle->gravity = 0x200;
    particle->scaleX = 0xcccc;
    particle->scaleY = 0xcccc;
    if (sprite != NULL)
      sprite->nextFrame = 0;
  }
}
