/// @file
/// The Sand sinking the leader into the ground: its picture, its sprite and
/// the grains it throws up.
// Part of radial.c.

#include "../radial_internal.h"
#include "fixed_math.h"
#include "macros.h"

/* VIEW(f0e446c): tag struct EffectSprite as struct EffectSprite__f0e446c;
 * effect_sprite.h has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/field/event_commands/radial/sand_sinking.c:74: structure has no
 * member named `priority'. */
struct EffectSprite__f0e446c {
  u8 unk00[4];
  u16 attr0_0 : 13;
  u16 color : 1;
  u16 attr0_14 : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 x;
  s32 y;
  s32 scaleX;
  s32 scaleY;
  u8 unk1c[2];
  u16 depth;
  u8 unk20[8];
};

struct EffectScene {
  u16 mode;
  u8 unk02[14];
  struct Actor *mainObject;
};

/// A particle's position and velocity (16.16), integrated each frame, and its
/// age in frames.
struct EffectParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

s32 BattleEffect_GetObjectDepth(struct Actor *object);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
void Camera_WorldToScreen(s32 *position);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);

/// Picture_ShiftColumnsByWave as the Sand sinks the leader into the ground:
/// each column's pixels below a wave line move down by phase (1.3 pixels a
/// step), those above it stay.
void Picture_SinkColumnsByWave(u8 *source, u8 *destination, s32 phase) {
  s32 width = 32;
  s32 height = 32;
  s32 column;
  s32 row;

  Iwram_ClearWords(destination, 0x400);
  for (column = 0; column < width; column++) {
    s32 wave = (Trig_Sin(column << 13) << 3) + 0x80000;

    for (row = 0; row < height; row++) {
      s32 base = phase * 0x14ccc;
      s32 rowOffset = row * 0x40000 - 0x7c0000;
      s32 shift = base + rowOffset - wave;

      shift >>= 16;
      if (shift < 0) {
        destination[row * width + column] = source[row * width + column];
      } else if (shift + row < height) {
        destination[((shift + row) << 5) + column] =
            source[row * width + column];
      }
    }
  }
}

#include "field/field_sequence_event.h"

/// Sets up the sprite the Sand draws the sinking leader as: the leader's
/// depth, priority, scale (three quarters on the world map) and place.
void Sand_InitLeaderSprite(struct EffectSprite__f0e446c *sprite, s32 tiles) {
  struct EffectScene *scene = Ram_HeapSlots->effectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  s32 position[3];

  EffectSprite_Init(sprite, 16, 30, 0x80000000, tiles);
  sprite->depth = BattleEffect_GetObjectDepth(scene->mainObject);
  sprite->priority = BattleEffect_GetObjectPriority(scene->mainObject);
  sprite->color = 1;
  sprite->palette = 0;
  if (event->mode == 3) {
    sprite->scaleX = Q16_ONE * 3 / 4;
    sprite->scaleY = Q16_ONE * 3 / 4;
  } else {
    sprite->scaleX = scene->mainObject->scaleX;
    sprite->scaleY = scene->mainObject->scaleY;
  }
  position[0] = scene->mainObject->x;
  position[1] = scene->mainObject->y;
  position[2] = scene->mainObject->z;
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
}

/// Draws Sand_InitLeaderSprite's sprite.
void Sand_DrawLeaderSprite(void *sprite) { EffectSprite_Draw(sprite); }

/// One grain of sand thrown up round the leader as it sinks: placed 2 pixels
/// up and 2 out when the leader stands (high), else 4 out, drifting outward,
/// drawn small just in front of the leader.
void Sand_SpawnGrain(struct EffectSprite__f0e446c *sprite,
                     struct EffectParticle *particle, s32 unused, s32 tiles,
                     s32 high) {
  struct Actor *mainObject =
      ((struct EffectScene *)Ram_HeapSlots->effectWork)->mainObject;
  s32 position[3];
  s32 y;

  position[0] = mainObject->x;
  position[1] = y = mainObject->y;
  position[2] = mainObject->z;
  if (high) {
    position[1] = y + 0x20000;
    Vector_AddPolarOffset(px(2), Random16(), position);
  } else {
    Vector_AddPolarOffset(px(4), Random16(), position);
  }
  SET_POSITION(particle, position[0], position[1], position[2]);
  position[2] = 0;
  position[0] = 0;
  if (high)
    Vector_AddPolarOffset(0x3333, Random16(), position);
  else
    Vector_AddPolarOffset(0xcccc, Random16(), position);
  particle->velocityX = position[0];
  particle->velocityY = 0;
  particle->velocityZ = position[2];
  EffectSprite_Init(sprite, 8, 8, 0x40000000, tiles);
  sprite->scaleX = 0x3333;
  sprite->scaleY = 0x3333;
  sprite->priority = BattleEffect_GetObjectPriority(mainObject);
  sprite->depth = BattleEffect_GetObjectDepth(mainObject) + 2;
  sprite->color = 1;
  sprite->palette = 0;
}
