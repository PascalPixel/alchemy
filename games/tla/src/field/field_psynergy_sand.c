/// @file
/// Sand: the leader sinking into the ground and moving as sand, the grains it
/// throws up, the task that ends it off sand, and the dust puff it ends with.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "system/input.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/heap_release.h"
#include "macros.h"
#include "field/field_sequence_event.h"
#include "field/field_psynergy_state.h"
#include "system/main_routine_state.h"
#include "field/field_sequence.h"

/// The grains of sand thrown up round the leader, and how many of them are
/// drawn as it sinks and as it rises.
#define SAND_GRAIN_COUNT 64
#define SAND_SINK_GRAINS 33
#define SAND_RISE_GRAINS 30
/// The leader's picture, 32 pixels square; Sand takes only a leader drawn so.
#define SAND_PICTURE_SIZE 32
/// The phase the leader's sunk picture rises from, 1.5 less each frame.
#define SAND_RISE_PHASE 92
/// An OAM attribute's high byte: the bits below its top two, the shape in
/// attribute 0 and the size in attribute 1, and the size bits of a sprite 32
/// pixels square.
#define OAM_BELOW_SHAPE_OR_SIZE 63
#define OAM_SIZE_32X32_BYTE (OBJ_SIZE_32X32 >> 24)

/// Sand's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the leader's picture and
/// the copy it sinks into the ground, the grains of sand thrown up round it and
/// the sprite the sinking leader is drawn with.
struct SandState {
  /// The leader's picture, 8 bits a pixel.
  u8 picture[SAND_PICTURE_SIZE * SAND_PICTURE_SIZE];
  /// The picture with its columns sunk by a wave.
  u8 sunken[SAND_PICTURE_SIZE * SAND_PICTURE_SIZE];
  struct EffectSprite grainSprites[SAND_GRAIN_COUNT];
  struct FieldPsynergyParticle grains[SAND_GRAIN_COUNT];
  struct EffectSprite leader;
  u8 unused[4];
};

s32 MapGrid_IsClimbableCell(s32 x, s32 z, s32 layer);

/// Actor_CheckBlocked's bit for another solid actor overlapping the actor.
#define BLOCKED_BY_ACTOR 2

/// Why the Sand ends (FieldPsynergy_WatchSandGround).
enum SandEnd {
  SAND_GOES_ON = 0,
  SAND_END_UPPER_LEVEL = 1, ///< the leader is on a scene's upper level
  SAND_END_OFF_SAND = 2,
  SAND_END_OFF_CLIMB = 3,  ///< climbing as sand, it left the climbable cells
  SAND_END_FACING_WALL = 4 ///< climbing as sand, the cell above is not one
};

/// Task of the Sand: ends it (cue ABILITY_SAND | ACT_PACKED) when the leader,
/// not blocked by another actor, stands off sand or on a scene's upper level
/// (gMainRoutineState.onUpperLevel), or while it climbs as sand it leaves a
/// climbable cell or faces one that is not.
void FieldPsynergy_WatchSandGround(void) {
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct MainRoutineState *party = &gMainRoutineState;
  struct Actor *actor = Actor_Lookup(party->leaderActorId);
  s32 reason = SAND_GOES_ON;

  if (party->onUpperLevel != 0)
    reason = SAND_END_UPPER_LEVEL;
  if (MapGrid_IsSandCell(actor->x.fixed, actor->z.fixed, actor->layer) == 0)
    reason = SAND_END_OFF_SAND;
  if (party->movementMode == LEADER_MOVEMENT_SAND && actor->userValue != 0 &&
      actor->timer == 0) {
    FIXED x;
    FIXED z;

    if (MapGrid_IsClimbableCell(actor->x.fixed, actor->z.fixed, 2) == 0)
      reason = SAND_END_OFF_CLIMB;
    x = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
    z = (actor->z.fixed & FIELD_CELL_MASK) - toFIXED(8);
    if (MapGrid_IsClimbableCell(x, z, 2) == 0)
      reason = SAND_END_FACING_WALL;
  }
  if (reason != SAND_GOES_ON &&
      (Actor_CheckBlocked(actor) & BLOCKED_BY_ACTOR) == 0)
    field->psynergy = ABILITY_SAND | ACT_PACKED;
}

/// How a picture is stored.
#define PICTURE_FORMAT_TAGGED_LZ 1 ///< one tagged LZ stream
#define PICTURE_FORMAT_COMPRESSED                                              \
  3 ///< compressed, then run through the decoder

/// A sprite layer (struct SpriteAnimation) as FieldPsynergy_DecodePicture
/// reads it: how its pictures are coded and the one shown.
struct PictureSource {
  u8 unused[5];  ///< no code names it
  u8 decodeMode; ///< its colorMode
  u8 priority;   ///< no code here names it
  u8 format;     ///< its mode
  const void **pictures;
  u8 unusedAfterPictures[10]; ///< no code names it
  u8 picture;
};

typedef void (*PictureDecoder)(const void *source, void *destination, s32 mode);

/// The ARM picture decoder FieldPsynergy_DecodePicture copies into RAM.
extern const u8 BattleEffect_DecodePictureCode[];
/// The end of the part of the decoder that is copied.
extern const u8 BattleEffect_DecodePictureCopyEnd[];
/// The decoder with its tables and its state (ld_script.ld). The block is
/// larger than the copy, which stops before the state.
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
  if (source->format == PICTURE_FORMAT_TAGGED_LZ) {
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
#undef ACTOR_FIRST_LAYER
#undef PICTURE_FORMAT_TAGGED_LZ
#undef PICTURE_FORMAT_COMPRESSED

/// Picture_ShiftColumnsByWave as the Sand sinks the leader into the ground:
/// each column's pixels below a wave line move down by phase (1.3 pixels a
/// step), those above it stay.
void Picture_SinkColumnsByWave(u8 *source, u8 *destination, s32 phase) {
  s32 width = SAND_PICTURE_SIZE;
  s32 height = SAND_PICTURE_SIZE;
  s32 column;
  s32 row;

  Iwram_ClearWords(destination, SAND_PICTURE_SIZE * SAND_PICTURE_SIZE);
  for (column = 0; column < width; column++) {
    s32 wave = Trig_Sin(column * (TRIG_TURN / 8)) * 8 + toFIXED(8);

    for (row = 0; row < height; row++) {
      s32 base = phase * (toFIXED(13) / 10);
      s32 rowOffset = row * toFIXED(4) - toFIXED(124);
      s32 shift = base + rowOffset - wave;

      shift = FIXEDtoINT(shift);
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
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  struct FieldState *field = Ram_HeapSlots->fieldState;
  FIXED position[3];

  EffectSprite_Init(sprite, 16, 30, OBJ_SIZE_32X32, tiles);
  sprite->depth = Actor_GetSpriteDepth(scene->caster);
  sprite->oam.priority = Actor_GetSpritePriority(scene->caster);
  sprite->oam.fullColor = 1;
  sprite->oam.palette = 0;
  if (field->mode == ACTOR_SYSTEM_WORLD_MAP) {
    sprite->scaleX = FIXED_ONE * 3 / 4;
    sprite->scaleY = FIXED_ONE * 3 / 4;
  } else {
    sprite->scaleX = scene->caster->scaleX;
    sprite->scaleY = scene->caster->scaleY;
  }
  position[0] = scene->caster->x.fixed;
  position[1] = scene->caster->y.fixed;
  position[2] = scene->caster->z.fixed;
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
}

/// Draws Sand_InitLeaderSprite's sprite.
void Sand_DrawLeaderSprite(void *sprite) { EffectSprite_Draw(sprite); }

/// One grain of sand thrown up round the leader as it sinks: 4 pixels out,
/// or, cast off sand (high), 2 pixels up and 2 out and drifting more slowly;
/// drawn small just in front of the leader.
void Sand_SpawnGrain(struct EffectSprite *sprite,
                     struct FieldPsynergyParticle *particle, s32 unused,
                     s32 tiles, s32 high) {
  struct Actor *mainActor =
      ((struct FieldEffectState *)Ram_HeapSlots->fieldEffectState)->caster;
  FIXED position[3];
  FIXED y;

  position[0] = mainActor->x.fixed;
  position[1] = y = mainActor->y.fixed;
  position[2] = mainActor->z.fixed;
  if (high) {
    position[1] = y + toFIXED(2);
    Vector_AddPolarOffset(toFIXED(2), Random16(), position);
  } else {
    Vector_AddPolarOffset(toFIXED(4), Random16(), position);
  }
  SET_POSITION(particle, position[0], position[1], position[2]);
  position[2] = 0;
  position[0] = 0;
  if (high)
    Vector_AddPolarOffset(toFIXED(0.2), Random16(), position);
  else
    Vector_AddPolarOffset(toFIXED(0.8), Random16(), position);
  particle->velocityX = position[0];
  particle->velocityY = 0;
  particle->velocityZ = position[2];
  EffectSprite_Init(sprite, 8, 8, OBJ_SIZE_16X16, tiles);
  sprite->scaleX = FIXED_ONE / 5;
  sprite->scaleY = FIXED_ONE / 5;
  sprite->oam.priority = Actor_GetSpritePriority(mainActor);
  sprite->depth = Actor_GetSpriteDepth(mainActor) + 2;
  sprite->oam.fullColor = 1;
  sprite->oam.palette = 0;
}

/// Advances one particle by a frame. For its first 32 frames its sprite
/// steps through eight pictures of eight tiles from tiles, four frames each,
/// grows by a tenth of its size a frame to full size, is placed on screen at
/// the particle's position and drawn, and the particle moves on; after that
/// only its age counts on. Reached from FieldPsynergy_EndSand and
/// FieldPsynergy_Sand.
void GrowingParticle_Update(struct EffectSprite *sprite,
                            struct FieldPsynergyParticle *particle, s32 unused,
                            s32 tiles) {
  s32 position[3];
  s32 scale;
  s32 picture;

  if ((u32)particle->timer <= 31) {
    picture = (particle->timer >> 2) & 7;
    sprite->oam.index = tiles + picture * 8;
    scale = sprite->scaleX + FIXED_ONE / 10;
    sprite->scaleX = scale;
    if (scale > FIXED_ONE)
      scale = FIXED_ONE;
    sprite->scaleX = scale;
    sprite->scaleY = scale;
    position[0] = particle->x;
    position[1] = particle->y;
    position[2] = particle->z;
    Camera_WorldToScreen(position);
    sprite->x = position[0];
    sprite->y = position[2];
    EffectSprite_Draw(sprite);
    Field_AdvanceWithGravity2D(&particle->x, 60, 0);
  }
  particle->timer++;
}

struct PendingEvent;

/// Gives an actor's first sprite layer another sprite's animations.
void Actor_SetFirstSpriteAnimationFar(struct Actor *actor, s32 index);
/// Sets a sprite's first layer to a SpriteColorMode and redraws it.
void Sprite_SetFirstAnimationColorModeFar(void *animation, s32 value);

/// Ends Sand: the leader rises out of the ground in a spray of grains and takes
/// its own sprite back; on sand, the place's Sand event then runs.
void FieldPsynergy_EndSand(void) {
  struct SandState *state =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct SandState));
  struct FieldEffectState *effect = Ram_HeapSlots->fieldEffectState;
  struct FieldState *field = Ram_HeapSlots->fieldState;
  s32 pictureEntry;
  struct Actor *owner = effect->caster;
  s32 particleEntry;
  s32 particleTiles;
  u8 *motionFlags;
  u8 *rippled;
  /* FAKEMATCH(temporary): source, the sprite's first entry held in a local for
   * the decode; as the entry passed to the decode. */
  void *source;
  /* FAKEMATCH(temporary): sprite, the leader's first tile, first holding the
   * picture's size for the decode and the load; as sizeof(state->picture) at
   * both. */
  s32 sprite;
  struct FieldPsynergyParticle *particle;
  struct EffectSprite *particleSprite;
  u8 *animation;
  s32 pictureOffset;
  s32 ripple;
  s32 frame;
  s32 i;
  s32 delay;
  s32 mask;

  FieldPsynergy_MarkWorldMapTilesUsed();
  gMainRoutineState.sandActive = 0;
  Actor_SetOthersPaused(owner, 1, 0);
  Task_Remove(FieldPsynergy_WatchSandGround);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_DUST_PUFF_A), state);
  particleEntry = VramCache_FindFreeEntry();
  particleTiles = VramCache_Load(particleEntry, 2048, state);
  i = 0;
  particle = state->grains;
  delay = 0;
  particleSprite = state->grainSprites;
  for (; i < SAND_GRAIN_COUNT; i++) {
    Sand_SpawnGrain(particleSprite, particle, i, particleTiles,
                    field->standing);
    particle->timer = delay;
    particleSprite++;
    delay -= 2;
    particle++;
  }
  FieldCamera_MoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  /* FAKEMATCH(temporary): motionFlags, a pointer standing in for
   * owner->motionFlags's address. */
  motionFlags = &owner->motionFlags;
  *motionFlags = 0;
  mask = OAM_BELOW_SHAPE_OR_SIZE;
  Actor_SetPositionAndResetMotionFar(
      owner, owner->x.fixed, owner->y.fixed + toFIXED(10), owner->z.fixed);
  Actor_SetFirstSpriteAnimationFar(owner, gMainRoutineState.leaderActorId);
  // The image's OAM shape and size: square, 32 by 32 pixels.
  ((u8 *)owner->sprite)[5] &= mask;
  mask &= ((u8 *)owner->sprite)[7];
  mask |= OAM_SIZE_32X32_BYTE;
  ((u8 *)owner->sprite)[7] = mask;
  if (field->mode == ACTOR_SYSTEM_WORLD_MAP)
    ((struct Sprite *)owner->sprite)->scale = FIXED_ONE * 3 / 4;
  owner->facing = DEGtoANG(90);
  Actor_SelectAnimationFar(owner, 0);
  Sprite_SetFirstAnimationColorModeFar(owner->sprite, SPRITE_COLOR_HIDDEN);
  wait(1);
  Sprite_SetFirstAnimationColorModeFar(owner->sprite, SPRITE_COLOR_NORMAL);
  source = ((struct Sprite *)owner->sprite)->entries[0];
  sprite = sizeof(state->picture);
  FieldPsynergy_DecodePicture(source, state->picture, sprite);
  Sprite_SetFirstAnimationColorModeFar(owner->sprite, SPRITE_COLOR_HIDDEN);
  pictureEntry = VramCache_FindFreeEntry();
  sprite = VramCache_Load(pictureEntry, sprite, NULL);
  Sand_InitLeaderSprite(&state->leader, sprite);
  wait(1);

  frame = 0;
  rippled = state->sunken;
  ripple = 0;
  pictureOffset = sprite * TILE_SIZE_4BPP;
  for (; frame < 78; frame++) {
    if (frame == 0)
      Sound_PlayFar(SE_SAND_END);
    if (frame <= 64) {
      Picture_SinkColumnsByWave(state->picture, rippled,
                                SAND_RISE_PHASE - ripple / 2);
      ((void (*)(void *, s32, s32, void *))Ram_HeapSlots->pictureUploader)(
          rippled, SAND_PICTURE_SIZE, SAND_PICTURE_SIZE,
          (u8 *)OBJ_VRAM0 + pictureOffset);
      Sand_DrawLeaderSprite(&state->leader);
    }
    if (frame == 66) {
      Actor_SelectAnimationFar(owner, 1);
      Sprite_SetFirstAnimationColorModeFar(owner->sprite, SPRITE_COLOR_NORMAL);
      Actor_SetSpriteFlagsFar(Actor_GetById(effect->casterId),
                              SPRITE_FLAG_SHADOW);
      FieldCamera_FollowActor(effect->casterId, 1);
      Sound_PlayFar(SE_SAND_EMERGE);
      if (field->mode == ACTOR_SYSTEM_WORLD_MAP) {
        gMainRoutineState.movementMode = LEADER_MOVEMENT_WORLD_MAP;
        *motionFlags = ACTOR_FALLS;
      } else {
        gMainRoutineState.movementMode = LEADER_MOVEMENT_WALK;
        *motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      }
      if (field->standing == 0)
        Field_RunPsynergyEvent(
            Scene_FindPsynergyTrigger(
                PSYNERGY_TRIGGER(PSYNERGY_PHASE_FADES), effect->fieldPsynergy,
                Scene_SelectPsynergyTarget(effect->fieldPsynergy)),
            effect->casterId, -1);
    }
    i = 0;
    particle = state->grains;
    particleSprite = state->grainSprites;
    for (; i < SAND_RISE_GRAINS; i++) {
      GrowingParticle_Update(particleSprite, particle, i, particleTiles);
      particleSprite++;
      particle++;
    }
    wait(1);
    ripple += 3;
  }
  Actor_SetOthersPaused(owner, 0, ANIM_STEP_NORMAL);
  VramCache_FreeEntry(pictureEntry);
  VramCache_FreeEntry(particleEntry);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// Sand cast on a target: runs FieldPsynergy_Sand.
void FieldPsynergy_SandOnTarget(void) { FieldPsynergy_Sand(); }

/// Sand: the leader spins and sinks into the ground in a spray of grains, then
/// moves on as a mound of sand (LEADER_MOVEMENT_SAND, or
/// LEADER_MOVEMENT_WORLD_SAND on the World Map) until
/// FieldPsynergy_WatchSandGround ends it. Off sand it rises again at once; a
/// leader whose sprite is not 32 by 32 pixels does nothing.
void FieldPsynergy_Sand(void) {
  struct SandState *state =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct SandState));
  struct FieldEffectState *effect = Ram_HeapSlots->fieldEffectState;
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct Actor *owner;
  s32 pictureEntry;
  s32 particleEntry;
  s32 particleTiles;
  FIXED savedY;
  u8 savedFlags;
  u8 *motionFlags;
  s32 pictureTiles;
  struct EffectSprite *sprite;
  struct FieldPsynergyParticle *particle;
  s32 frame;
  s32 delay;
  s32 i;

  owner = effect->caster;
  savedY = owner->y.fixed;
  motionFlags = &owner->motionFlags;
  savedFlags = *motionFlags;
  if (((struct Sprite *)owner->sprite)->width == SAND_PICTURE_SIZE &&
      ((struct Sprite *)owner->sprite)->height == SAND_PICTURE_SIZE) {
    FieldPsynergy_MarkWorldMapTilesUsed();
    Actor_SetOthersPaused(owner, 1, 0);
    if (MapGrid_IsSandCell(owner->x.fixed, owner->z.fixed, owner->layer) == 0)
      field->standing = 1;
    else
      field->standing = 0;
    FieldPsynergy_FadeToCastColor();
    Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_DUST_PUFF_A), state);
    particleEntry = VramCache_FindFreeEntry();
    particleTiles = VramCache_Load(particleEntry, 2048, state);
    i = 0;
    particle = state->grains;
    delay = -12;
    sprite = state->grainSprites;
    for (; i < SAND_GRAIN_COUNT; i++) {
      Sand_SpawnGrain(sprite, particle, i, particleTiles, field->standing);
      particle->timer = delay;
      sprite++;
      delay -= 2;
      particle++;
    }
    FieldCamera_MoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    BattleEffect_SetObjectUpdateMode(effect->caster, CASTER_LOOK_NORMAL);
    Actor_SetPositionAndResetMotionFar(owner, owner->x.fixed, owner->y.fixed,
                                       owner->z.fixed);
    *motionFlags = 0;
    wait(2);
    for (frame = 0; frame < 40; frame++) {
      if (frame > 0)
        owner->y.fixed += toFIXED(9) / 40;
      if (frame & 31)
        Sound_PlayFar(SE_SPIRAL);
      owner->facing += DEGtoANG(45);
      wait(1);
    }
    owner->facing = DEGtoANG(90);
    wait(1);
    FieldPsynergy_DecodePicture(
        (struct PictureSource *)((struct Sprite *)owner->sprite)->entries[0],
        state->picture, sizeof(state->picture));
    pictureEntry = VramCache_FindFreeEntry();
    pictureTiles = VramCache_Load(pictureEntry, sizeof(state->picture), NULL);
    Sand_InitLeaderSprite(&state->leader, pictureTiles);
    frame = 15;
    for (; frame <= 92; frame++) {
      Picture_SinkColumnsByWave(state->picture, state->sunken, frame * 3 / 2);
      ((void (*)(void *, s32, s32, void *))Ram_HeapSlots->pictureUploader)(
          state->sunken, SAND_PICTURE_SIZE, SAND_PICTURE_SIZE,
          (u8 *)OBJ_VRAM0 + pictureTiles * TILE_SIZE_4BPP);
      Sand_DrawLeaderSprite(&state->leader);
      i = 0;
      particle = state->grains;
      sprite = state->grainSprites;
      for (; i < SAND_SINK_GRAINS; i++) {
        GrowingParticle_Update(sprite, particle, i, particleTiles);
        sprite++;
        particle++;
      }
      if (frame == 16)
        Sprite_SetFirstAnimationColorModeFar(owner->sprite,
                                             SPRITE_COLOR_HIDDEN);
      if (frame == 45)
        Sound_PlayFar(SE_WHOOSH);
      if (frame == 60) {
        Actor_SetFirstSpriteAnimationFar(owner, SPRITE_SAND_RIPPLE);
        // The image's OAM shape and size: 16 by 16 pixels.
        ((struct FieldSprite *)owner->sprite)->shape = OBJ_SHAPE_SQUARE;
        ((struct FieldSprite *)owner->sprite)->size = 1;
        Actor_SelectAnimationFar(owner, 1);
        Actor_SetSpriteFlagsFar(Actor_GetById(effect->casterId), 0);
        FieldCamera_FollowActor(effect->casterId, 1);
        owner->y.fixed = savedY;
        *motionFlags = savedFlags;
      }
      if (frame == 61)
        Sprite_SetFirstAnimationColorModeFar(owner->sprite,
                                             SPRITE_COLOR_NORMAL);
      wait(1);
    }
    if (field->mode == ACTOR_SYSTEM_WORLD_MAP)
      gMainRoutineState.movementMode = LEADER_MOVEMENT_WORLD_SAND;
    else
      gMainRoutineState.movementMode = LEADER_MOVEMENT_SAND;
    owner->timer = 0;
    owner->userValue = 0;
    field->sandTrailPriority = 3;
    gMainRoutineState.sandActive = 1;
    if (field->standing == 0)
      Task_Add(FieldPsynergy_WatchSandGround, TASK_PASS_FRAME);
    Actor_SetOthersPaused(owner, 0, ANIM_STEP_NORMAL);
    VramCache_FreeEntry(pictureEntry);
    VramCache_FreeEntry(particleEntry);
    FieldPsynergy_FadeFromCastColor();
  }
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  if (field->standing != 0)
    FieldPsynergy_EndSand();
}
#undef SAND_GRAIN_COUNT
#undef SAND_SINK_GRAINS
#undef SAND_RISE_GRAINS
#undef SAND_PICTURE_SIZE
#undef SAND_RISE_PHASE
#undef OAM_BELOW_SHAPE_OR_SIZE
#undef OAM_SIZE_32X32_BYTE
#undef BLOCKED_BY_ACTOR
