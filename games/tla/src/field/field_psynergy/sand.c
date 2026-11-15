/// @file
/// Sand: the leader sinking into the ground and moving as sand, the grains it
/// throws up, the task that ends it off sand, and the dust puff it ends with.
// Part of field_psynergy.c.

#include "../field_psynergy_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "system/input.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/heap_release.h"
#include "macros.h"
#include "field/field_sequence_event.h"
#include "field/field_psynergy_work.h"
#include "system/main_routine_work.h"

s32 MapGrid_IsClimbableCell(s32 x, s32 z, s32 layer);

/// Task of the Sand: ends it (cue 0x208b) when the leader, not blocked, stands
/// off sand or gMainRoutineWork's byte 707 is set, or while it climbs as sand
/// it leaves a climbable cell or faces one that is not.
void FieldPsynergy_WatchSandGround(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object = ObjectTable_Get(party->leaderActorId);
  s32 reason = 0;

  if ((s8)party->unk2c3 != 0)
    reason = 1;
  if (MapGrid_IsSandCell(object->x.fixed, object->z.fixed, object->layer) == 0)
    reason = 2;
  if (party->movementMode == 5 && *(s16 *)((u8 *)object + 102) != 0 &&
      object->timer == 0) {
    s32 x;
    s32 z;
    s32 back;

    if (MapGrid_IsClimbableCell(object->x.fixed, object->z.fixed, 2) == 0)
      reason = 3;
    x = object->x.fixed & 0xfff00000;
    z = object->z.fixed & 0xfff00000;
    back = -0x80000;
    /* FAKEMATCH(barrier): back through an empty asm. */
    asm("" : "+l"(back));
    /* FAKEMATCH(dead): an empty once-only block. */
    do {
    } while (0);
    if (MapGrid_IsClimbableCell(x + 0x80000, z + back, 2) == 0)
      reason = 4;
  }
  if (reason != 0 && (Actor_CheckBlocked(object) & 2) == 0)
    event->psynergy = 0x208b;
}

// How a picture is stored.
#define PICTURE_FORMAT_TYPE01 1 ///< one type-01 stream
#define PICTURE_FORMAT_COMPRESSED                                              \
  3 ///< compressed, then run through the decoder

struct PictureSource {
  u8 unk00[5];
  u8 decodeMode;
  u8 unk06;
  u8 format;
  const void **pictures;
  u8 unk0c[10];
  u8 picture;
};

typedef void (*PictureDecoder)(const void *source, void *destination, s32 mode);

extern const u8 BattleEffect_DecodePictureCode[];
extern const u8 BattleEffect_DecodePictureCopyEnd[];
/// The decoder with its tables and work area (ld_script.ld). The block is
/// larger than the copy, which stops before the work area.
extern u8 BattleEffect_DecodePictureBlockSize[];

/// Draws the current picture into a cleared buffer of the given
/// size. The ARM picture decoder runs from RAM: when no copy is loaded yet,
/// this loads one for the call and releases it afterwards.
void FieldPsynergy_DecodePicture(struct PictureSource *source, void *buffer,
                                 s32 size) {
  s32 decoderLoaded = 1;
  PictureDecoder decode = Ram_HeapSlots->frameScratch;

  if (decode == NULL) {
    Dma_Set(BattleEffect_DecodePictureCode,
            HeapSlot_Alloc(HEAP_SLOT_FRAME_SCRATCH,
                           (s32)BattleEffect_DecodePictureBlockSize),
            DMA_COPY_WORDS((u32)(BattleEffect_DecodePictureCopyEnd -
                                 BattleEffect_DecodePictureCode) >>
                           2),
            REG_DMA3);
    decode = Ram_HeapSlots->frameScratch;
    decoderLoaded = 0;
  }

  Iwram_ClearWordsEntry(buffer, size);
  if (source->format == PICTURE_FORMAT_TYPE01) {
    Decompress_TaggedLz(source->pictures[source->picture], buffer);
  } else if (source->format == PICTURE_FORMAT_COMPRESSED) {
    const void *stream =
        Iwram_Decompress(source->pictures[source->picture], buffer);

    if (stream != NULL)
      decode(stream, buffer, 0);
  } else {
    decode(source->pictures[source->picture], buffer, source->decodeMode);
  }

  if (!decoderLoaded)
    HeapSlot_Release(HEAP_SLOT_FRAME_SCRATCH);
}
#undef OBJECT_DEFINITION
#undef SCRIPT_OBJECT_COUNT
#undef EVENT_MODE_NO_CELL_TEST
#undef PICTURE_FORMAT_TYPE01
#undef PICTURE_FORMAT_COMPRESSED

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

void Camera_WorldToScreen(s32 *position);

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

/// Sets up the sprite the Sand draws the sinking leader as: the leader's
/// depth, priority, scale (three quarters on the world map) and place.
void Sand_InitLeaderSprite(struct EffectSprite *sprite, s32 tiles) {
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  s32 position[3];

  EffectSprite_Init(sprite, 16, 30, 0x80000000, tiles);
  sprite->depth = BattleEffect_GetObjectDepth(scene->mainActor);
  sprite->oam.priority = BattleEffect_GetObjectPriority(scene->mainActor);
  sprite->oam.palette256 = 1;
  sprite->oam.palette = 0;
  if (event->mode == ACTOR_SYSTEM_WORLD_MAP) {
    sprite->scaleX = Q16_ONE * 3 / 4;
    sprite->scaleY = Q16_ONE * 3 / 4;
  } else {
    sprite->scaleX = scene->mainActor->scaleX;
    sprite->scaleY = scene->mainActor->scaleY;
  }
  position[0] = scene->mainActor->x.fixed;
  position[1] = scene->mainActor->y.fixed;
  position[2] = scene->mainActor->z.fixed;
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
}

/// Draws Sand_InitLeaderSprite's sprite.
void Sand_DrawLeaderSprite(void *sprite) { EffectSprite_Draw(sprite); }

/// One grain of sand thrown up round the leader as it sinks: placed 2 pixels
/// up and 2 out when the leader stands (high), else 4 out, drifting outward,
/// drawn small just in front of the leader.
void Sand_SpawnGrain(struct EffectSprite *sprite,
                     struct EffectParticle *particle, s32 unused, s32 tiles,
                     s32 high) {
  struct Actor *mainActor =
      ((struct FieldEffectWork *)Ram_HeapSlots->fieldEffectWork)->mainActor;
  s32 position[3];
  s32 y;

  position[0] = mainActor->x.fixed;
  position[1] = y = mainActor->y.fixed;
  position[2] = mainActor->z.fixed;
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
  sprite->oam.priority = BattleEffect_GetObjectPriority(mainActor);
  sprite->depth = BattleEffect_GetObjectDepth(mainActor) + 2;
  sprite->oam.palette256 = 1;
  sprite->oam.palette = 0;
}

void Camera_WorldToScreen(s32 *position);

/// Advances one particle by a frame. For its first 32 frames its sprite
/// steps through eight pictures of eight tiles from tiles, four frames each,
/// grows by a tenth of its size a frame to full size, is placed on screen at
/// the particle's position and drawn, and the particle moves on; after that
/// only its age counts on. Reached from FieldPsynergy_EndSand and
/// FieldPsynergy_Sand.
void GrowingParticle_Update(struct EffectSprite *sprite,
                            struct EffectParticle *particle, s32 unused,
                            s32 tiles) {
  s32 position[3];
  s32 scale;
  s32 picture;

  if ((u32)particle->timer <= 31) {
    picture = (particle->timer >> 2) & 7;
    sprite->oam.index = tiles + picture * 8;
    scale = sprite->scaleX + 0x1999;
    sprite->scaleX = scale;
    if (scale > Q16_ONE)
      scale = Q16_ONE;
    sprite->scaleX = scale;
    sprite->scaleY = scale;
    position[0] = particle->x;
    position[1] = particle->y;
    position[2] = particle->z;
    Camera_WorldToScreen(position);
    sprite->x = position[0];
    sprite->y = position[2];
    EffectSprite_Draw(sprite);
    BattleEffect_IntegrateVector2(&particle->x, 60, 0);
  }
  particle->timer++;
}

struct PendingEvent;

void Actor_SetFirstSpriteAnimationFar(struct Actor *object, s32 index);
void Animation_ApplyChildValueToFirstFar(void *animation, s32 value);

void FieldPsynergy_EndSand(void) {
  u8 *work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, 0x192c);
  struct FieldEffectWork *effect = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  s32 pictureEntry;
  struct Actor *owner = effect->mainActor;
  s32 particleEntry;
  s32 particleTiles;
  u8 *motionFlags;
  u8 *rippled;

  s32 sprite;
  u8 *particle;
  u8 *particleSprite;
  u8 *animation;
  struct MainRoutineWork *party;
  s32 pictureOffset;
  s32 ripple;
  s32 frame;
  s32 i;
  s32 delay;
  s32 mask;

  FieldPsynergy_MarkWorldMapTilesUsed();
  gMainRoutineWork.pendingThird = 0;
  Actor_SetOthersPaused(owner, 1, 0);
  Task_Remove(FieldPsynergy_WatchSandGround);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_DustPuffA), work);
  particleEntry = VramCache_FindFreeEntry();
  particleTiles = VramCache_Load(particleEntry, 0x800, work);
  i = 0;
  particle = work + 0x1200;
  delay = 0;
  particleSprite = work + 0x800;
  for (; i < 64; i++) {
    Sand_SpawnGrain(particleSprite, particle, i, particleTiles,
                    event->standing);
    *(s32 *)(particle + 24) = delay;
    particleSprite += 40;
    delay -= 2;
    particle += 28;
  }
  FieldCamera_MoveWithinMapBounds(-1, -1, -1, 0);
  motionFlags = &owner->motionFlags;
  *motionFlags = 0;
  // Set before the move: 63 is held in a saved register across both calls.
  mask = 63;
  Actor_SetPositionAndResetMotionFar(owner, owner->x.fixed,
                                     owner->y.fixed + 0xa0000, owner->z.fixed);
  party = &gMainRoutineWork;
  Actor_SetFirstSpriteAnimationFar(owner, party->leaderActorId);
  ((u8 *)owner->sprite)[5] &= mask;
  mask &= ((u8 *)owner->sprite)[7];
  mask |= 128;
  ((u8 *)owner->sprite)[7] = mask;
  if (event->mode == ACTOR_SYSTEM_WORLD_MAP)
    *(s32 *)((u8 *)owner->sprite + 12) = 0xc000;
  owner->facing = TRIG_TURN / 4;
  Actor_SelectAnimationFar(owner, 0);
  Animation_ApplyChildValueToFirstFar(owner->sprite, 15);
  wait(1);
  Animation_ApplyChildValueToFirstFar(owner->sprite, 0);
  {
    void *source = *(void **)((u8 *)owner->sprite + 40);

    sprite = 0x400;
    {
      /* FAKEMATCH(pin): the copy's size pinned to r2. */
      register s32 size asm("r2") = sprite;

      FieldPsynergy_DecodePicture(source, work, size);
    }
  }
  Animation_ApplyChildValueToFirstFar(owner->sprite, 15);
  /* FAKEMATCH(barrier): an empty asm reading party. */
  asm("" : : "r"(party));
  pictureEntry = VramCache_FindFreeEntry();
  sprite = VramCache_Load(pictureEntry, sprite, NULL);
  Sand_InitLeaderSprite(work + 0x1900, sprite);
  wait(1);

  frame = 0;
  rippled = work + 0x400;
  ripple = 0;
  pictureOffset = sprite * 32;
  for (; frame < 78; frame++) {
    if (frame == 0)
      Sound_PlayFar(SE_544);
    if (frame <= 64) {
      Picture_SinkColumnsByWave(work, rippled, 92 - ripple / 2);
      ((void (*)(void *, s32, s32, void *))Ram_HeapSlots->pictureUploader)(
          rippled, 32, 32, (u8 *)OBJ_VRAM0 + pictureOffset);
      Sand_DrawLeaderSprite(work + 0x1900);
    }
    if (frame == 66) {
      Actor_SelectAnimationFar(owner, 1);
      Animation_ApplyChildValueToFirstFar(owner->sprite, 0);
      Actor_SetSpriteFlagsFar(Actor_GetById(effect->firstActorId), 1);
      FieldCamera_FollowActor(effect->firstActorId, 1);
      Sound_PlayFar(SE_539);
      if (event->mode == ACTOR_SYSTEM_WORLD_MAP) {
        gMainRoutineWork.movementMode = 1;
        *motionFlags = 2;
      } else {
        gMainRoutineWork.movementMode = 0;
        *motionFlags = 3;
      }
      if (event->standing == 0)
        Field_RunPsynergyEvent(
            Scene_FindPsynergyTrigger(
                0x40000005, effect->fieldPsynergy,
                Scene_SelectPsynergyTarget(effect->fieldPsynergy)),
            effect->firstActorId, -1);
    }
    i = 0;
    particle = work + 0x1200;
    particleSprite = work + 0x800;
    for (; i < 30; i++) {
      GrowingParticle_Update(particleSprite, particle, i, particleTiles);
      particleSprite += 40;
      particle += 28;
    }
    wait(1);
    ripple += 3;
  }
  Actor_SetOthersPaused(owner, 0, 16);
  VramCache_FreeEntry(pictureEntry);
  VramCache_FreeEntry(particleEntry);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// Psynergy effect 23 cast on a target: runs its effect.
void FieldPsynergy_SandOnTarget(void) { FieldPsynergy_Sand(); }

void FieldPsynergy_Sand(void) {
  u8 *work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, 0x192c);
  struct FieldEffectWork *effect = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *owner;
  s32 pictureEntry;
  s32 particleEntry;
  s32 particleTiles;
  s32 savedY;
  u8 savedFlags;
  u8 *motionFlags;
  s32 pictureTiles;
  u8 *sprite;
  u8 *particle;
  s32 frame;
  s32 delay;
  s32 i;

  owner = effect->mainActor;
  savedY = owner->y.fixed;
  motionFlags = &owner->motionFlags;
  savedFlags = *motionFlags;
  if (*(u16 *)((u8 *)owner->sprite + 20) != 0x2020)
    goto end;
  FieldPsynergy_MarkWorldMapTilesUsed();
  Actor_SetOthersPaused(owner, 1, 0);
  if (MapGrid_IsSandCell(owner->x.fixed, owner->z.fixed, owner->layer) == 0)
    event->standing = 1;
  else
    event->standing = 0;
  FieldPsynergy_FadeToCastColor();
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_DustPuffA), work);
  particleEntry = VramCache_FindFreeEntry();
  particleTiles = VramCache_Load(particleEntry, 0x800, work);
  i = 0;
  particle = work + 0x1200;
  delay = -12;
  sprite = work + 0x800;
  for (; i < 64; i++) {
    Sand_SpawnGrain(sprite, particle, i, particleTiles, event->standing);
    *(s32 *)(particle + 24) = delay;
    sprite += 40;
    delay -= 2;
    particle += 28;
  }
  FieldCamera_MoveWithinMapBounds(-1, -1, -1, 0);
  BattleEffect_SetObjectUpdateMode(effect->mainActor, 0);
  Actor_SetPositionAndResetMotionFar(owner, owner->x.fixed, owner->y.fixed,
                                     owner->z.fixed);
  *motionFlags = 0;
  wait(2);
  for (frame = 0; frame < 40; frame++) {
    if (frame > 0)
      owner->y.fixed += 0x3999;
    if (frame & 31)
      Sound_PlayFar(SE_566);
    owner->facing += TRIG_TURN / 8;
    wait(1);
  }
  owner->facing = TRIG_TURN / 4;
  wait(1);
  FieldPsynergy_DecodePicture(*(void **)((u8 *)owner->sprite + 40), work,
                              0x400);
  pictureEntry = VramCache_FindFreeEntry();
  pictureTiles = VramCache_Load(pictureEntry, 0x400, NULL);
  Sand_InitLeaderSprite(work + 0x1900, pictureTiles);
  frame = 15;
  for (; frame <= 92; frame++) {
    Picture_SinkColumnsByWave(work, work + 0x400, frame * 3 / 2);
    ((void (*)(void *, s32, s32, void *))Ram_HeapSlots->pictureUploader)(
        work + 0x400, 32, 32, (u8 *)OBJ_VRAM0 + pictureTiles * 32);
    Sand_DrawLeaderSprite(work + 0x1900);
    i = 0;
    particle = work + 0x1200;
    sprite = work + 0x800;
    for (; i <= 32; i++) {
      GrowingParticle_Update(sprite, particle, i, particleTiles);
      sprite += 40;
      particle += 28;
    }
    if (frame == 16)
      Animation_ApplyChildValueToFirstFar(owner->sprite, 15);
    if (frame == 45)
      Sound_PlayFar(SE_WHOOSH);
    if (frame == 60) {
      Actor_SetFirstSpriteAnimationFar(owner, 251);
      ((u8 *)owner->sprite)[5] &= 63;
      ((u8 *)owner->sprite)[7] = (((u8 *)owner->sprite)[7] & 63) | 64;
      Actor_SelectAnimationFar(owner, 1);
      Actor_SetSpriteFlagsFar(Actor_GetById(effect->firstActorId), 0);
      FieldCamera_FollowActor(effect->firstActorId, 1);
      owner->y.fixed = savedY;
      *motionFlags = savedFlags;
    }
    if (frame == 61)
      Animation_ApplyChildValueToFirstFar(owner->sprite, 0);
    wait(1);
  }
  if (event->mode == ACTOR_SYSTEM_WORLD_MAP)
    gMainRoutineWork.movementMode = 9;
  else
    gMainRoutineWork.movementMode = 5;
  owner->timer = 0;
  owner->userValue = 0;
  event->unkd4a = 3;
  gMainRoutineWork.pendingThird = 1;
  if (event->standing == 0)
    Task_Add(FieldPsynergy_WatchSandGround, 0x480);
  Actor_SetOthersPaused(owner, 0, 16);
  VramCache_FreeEntry(pictureEntry);
  VramCache_FreeEntry(particleEntry);
  FieldPsynergy_FadeFromCastColor();
end:
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  if (event->standing != 0)
    FieldPsynergy_EndSand();
}
