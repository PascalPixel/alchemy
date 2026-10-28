// First draft of Func_080eb960, written from the listing; no ☀️ twin. An
// object vanishes in a whirl of sparks: 64 sprites of picture 0x1ea (8x8, 256
// colours, the object's priority) circle out from it, each spark rising and
// closing in for 50 frames. Ten frames in the sparks stop coming and the
// object spins and widens from nothing back to full width over 64 frames,
// when its sprite and its child flags come back; 74 frames later it ends.
//
// Differ 135 of 325 (draft 322; five minutes of permuter found nothing
// better). The shape follows the listing; left: the stack frame is 48 bytes in the
// native, with a spill slot at sp + 4 for the sprite pointer of the second
// loop, which the native keeps in r4 and copies to r6 for the drawing (this
// keeps one pointer in r6), and the stack slots of the saved angle, depth,
// tiles, entry and phase in another order; the first loop's sprite is r5 and
// the 15 mask r6 in the native, the other way round here.
#include "types.h"
#include "mem.h"
#include "system.h"
#include "resource.h"
#include "actor.h"
#include "effect_sprite.h"

// One spark: where it circles round, its angle and radius, and its age in
// frames (-1: unused).
struct Spark {
  s32 centre[3];
  s32 angle;
  s32 unk10;
  s32 radius;
  s32 age;
};

// The heap block of the effect: the picture, then the sparks and their
// sprites.
struct SparkWork {
  u8 picture[0x400];
  struct Spark sparks[64];
  struct EffectSprite sprites[64];
};

#define HEAP_SLOT_SPARKS 92
#define SPARK_LIFE 50
#define PHASE_DONE 999

s32 BattleEffect_GetObjectDepth(struct Actor *object);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
void Actor_SetSingleChildFlagsFar(struct Actor *object,
                                           u32 flags);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
void Camera_WorldToScreen(s32 *position);

void Func_080eb960(s32 objectId) {
  struct SparkWork *work =
      HeapSlot_Alloc(HEAP_SLOT_SPARKS, sizeof(struct SparkWork));
  struct Actor *object = ObjectTable_Get(objectId);
  s32 depth = BattleEffect_GetObjectDepth(object);
  u16 angle = object->angle;
  s32 entry;
  s32 tiles;
  struct Spark *spark;
  struct EffectSprite *sprite;
  s32 i;
  s32 n;
  s32 spawning;
  s32 phase;
  s32 next;
  s32 frame;
  s32 position[3];

  Resource_DecodeType01(Resource_GetTableEntry(0x1ea), work);
  entry = Resource_FindFreeEntry();
  tiles = VramCache_Load(entry, sizeof(work->picture), work);
  spark = work->sparks;
  sprite = work->sprites;
  for (i = 0; i < 64; i++, sprite++, spark++) {
    Func_080eaf98(sprite, 4, 4, 0, tiles + (i & 15) * 2);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->depth = BattleEffect_GetObjectDepth(object);
    sprite->oam.priority = BattleEffect_GetObjectPriority(object);
    spark->age = -1;
  }
  object->scaleX = 0;
  object->scaleY = 0x10000;
  Actor_SetSingleChildFlagsFar(object, 0);
  spawning = 1;
  phase = 0;
  next = 0;
  frame = 0;
  do {
    if (spawning != 0) {
      for (n = spawning; n != 0; n--) {
        s32 sparkAngle = Random16();
        s32 radius = Random16() * 6 + 0xc0000;

        spark = &work->sparks[next];
        spark->age = 0;
        spark->centre[0] = object->x;
        spark->centre[1] = object->y + 0x140000;
        spark->centre[2] = object->z;
        Vector_AddPolarOffset(Random16(), sparkAngle, spark->centre);
        next++;
        spark->angle = sparkAngle;
        spark->radius = radius;
        next %= 64;
      }
    }
    spark = work->sparks;
    sprite = work->sprites;
    for (i = 63; i >= 0; i--, sprite++, spark++) {
      if (spark->age < 0)
        continue;
      if ((u32)spark->age < SPARK_LIFE) {
        position[0] = spark->centre[0];
        position[1] = spark->centre[1];
        position[2] = spark->centre[2];
        Vector_AddPolarOffset(spark->radius, spark->angle, position);
        if (position[2] > object->z)
          sprite->depth = depth + 2;
        else
          sprite->depth = depth - 2;
        Camera_WorldToScreen(position);
        sprite->x = position[0];
        sprite->y = position[2];
        sprite->oam.index = tiles + (spark->age & 3) * 2;
        Func_080eb01c(sprite);
        spark->angle += 0x800;
        spark->radius -= 0x3333;
        spark->centre[1] -= 0x4000;
      }
      if (++spark->age == SPARK_LIFE)
        spark->age = -1;
    }
    switch (phase) {
    case 0:
      if (frame == 10) {
        phase = 1;
        frame = -1;
      }
      break;
    case 1:
      if (frame < 64) {
        object->angle -= 0x1000;
        object->scaleX = frame << 10;
        if (object->scaleX > 0x10000)
          object->scaleX = 0x10000;
      }
      if (frame == 64) {
        object->angle = angle;
        Actor_SetSingleChildFlagsFar(object, 1);
      }
      if (frame == 10)
        spawning = 0;
      if (frame == 74)
        phase = PHASE_DONE;
      break;
    }
    wait(1);
    frame++;
  } while (phase != PHASE_DONE);
  Resource_ResetEntry(entry);
  HeapSlot_Release(HEAP_SLOT_SPARKS);
}
