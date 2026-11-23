/// @file
/// Hover: its glow, the cast and its end.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "resource_ids.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"
#include "field/field_event.h"
#include "field/field_sequence.h"

/// The levels in gHoverGlow_Levels's cycle, and the steps of the glow's
/// colours.
#define HOVER_GLOW_WAVE_LENGTH 96
#define HOVER_GLOW_STEPS 24

/// A cycle of 96 colour levels (31 down to 0, 32 at 0, then 0 up to 31),
/// read a third of a turn apart for red, green and blue.
extern const u8 gHoverGlow_Levels[];

/// The colour of Hover's glow (HoverGlow_Update) for a step of
/// its 24-step cycle and a brightness level (7 is neutral): three channels of
/// the wave, each raised or lowered by the level and held to 0..31, packed as
/// RGB555.
s32 HoverGlow_GetColor(s32 counter, s32 level) {
  s16 phase;
  s16 bias;
  s16 red;
  s16 green;
  s16 blue;
  s16 redLevel;
  s32 step;
  const u8 *wave;

  step = (counter + HOVER_GLOW_STEPS / 2) % HOVER_GLOW_STEPS;
  wave = gHoverGlow_Levels;
  phase = step * (HOVER_GLOW_WAVE_LENGTH / HOVER_GLOW_STEPS);
  redLevel = wave[(s16)(phase % HOVER_GLOW_WAVE_LENGTH)];
  bias = level - 7;
  red = redLevel + bias;
  green = wave[(phase + HOVER_GLOW_WAVE_LENGTH / 3) % HOVER_GLOW_WAVE_LENGTH] +
          bias;
  blue =
      wave[(phase + HOVER_GLOW_WAVE_LENGTH * 2 / 3) % HOVER_GLOW_WAVE_LENGTH] +
      bias;
  if (red < 0)
    red = 0;
  if (red > RGB_LEVEL_MAX)
    red = RGB_LEVEL_MAX;
  if (green < 0)
    green = 0;
  if (green > RGB_LEVEL_MAX)
    green = RGB_LEVEL_MAX;
  if (blue < 0)
    blue = 0;
  if (blue > RGB_LEVEL_MAX)
    blue = RGB_LEVEL_MAX;
  return RGB(red, green, blue);
}

/// The glow's colours: sprite palette CANVAS_PALETTE.
#define HOVER_GLOW_PALETTE ((u16 *)&OBJ_PLTT_COLOR(CANVAS_PALETTE, 0))

/// The glow's strength, in frames: full at 112, and it fades out a frame at a
/// time once the player leaves the place that feeds it.
#define GLOW_FULL 112
/// The glow's strength set as the ring grows, before the glow is held.
#define GLOW_RISING 100
#define GLOW_SPENT_CUE (ABILITY_HOVER | ACT_PACKED)
/// The frames the ring under the leader takes to grow, or to shrink away.
#define HOVER_RING_GROW_FRAMES 32

/// Hover's work (heap slot HEAP_SLOT_BATTLE_EFFECT): the glowing ring under the
/// hovering leader, its scale, tile and stage.
struct HoverWork {
  u8 tiles[4096]; ///< the ring's picture, decoded to be loaded
  struct EffectSprite sprite;
  s32 scale;
  s16 resource;
  u16 tile;
  u16 unused; ///< cleared as Hover starts; nothing reads it
  u16 phase;
  s16 fading;
};

s32 MapGrid_IsHoverCell(s32 x, s32 z, s32 layer);

/// Hover's frame callback: keeps the glow's strength full while the player
/// stands where MapGrid_IsHoverCell allows, and runs it down otherwise (once it
/// is fading, the glow shrinks with it and the event takes its cue when it is
/// spent). It cycles HOVER_GLOW_PALETTE through HoverGlow_GetColor's colours,
/// flickers between two pictures every other frame, and draws the glow under
/// the player, flattened to three quarters of its height.
void HoverGlow_Update(void) {
  struct HoverWork *work = Ram_HeapSlots->battleEffect;
  struct Actor *player = Actor_Lookup(Party_GetLeaderActor());
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  u16 *palette;
  FIXED position[3];
  s32 level;
  s32 i;
  s32 scaleX;
  s32 scaleY;
  struct EffectSprite *sprite;

  // The glow's strength.
  if (MapGrid_IsHoverCell(player->x.fixed, player->z.fixed, player->layer) ==
      0) {
    if (event->glowHeld != 0)
      event->glow--;
  } else {
    event->glow = GLOW_FULL;
  }

  if (work->fading == 1) {
    level = event->glow;
    if (level > GLOW_FULL)
      level = GLOW_FULL;
    if (level < 0)
      level = 0;
    work->scale = level * (FIXED_ONE / 2) / GLOW_FULL + FIXED_ONE / 2;
    if (level == 0)
      event->psynergy = GLOW_SPENT_CUE;
  }

  // Cycle the glow's colours; the last takes the 15th's level.
  palette = HOVER_GLOW_PALETTE;
  for (i = 0; i < PALETTE_COLORS; i++) {
    Io_Write16(
        HoverGlow_GetColor((((s16)work->phase >> 2) + i / 2) % HOVER_GLOW_STEPS,
                           i == PALETTE_COLORS - 1 ? PALETTE_COLORS - 2 : i),
        palette++);
  }
  work->phase++;

  // Draw it under the player.
  scaleX = work->scale;
  scaleY = scaleX * 6 / 8;
  sprite = &work->sprite;
  sprite->oam.index = work->tile + ((gFrameCount >> 1) & 1) * 32;
  sprite->oam.priority = BattleEffect_GetObjectPriority(player);
  sprite->depth = BattleEffect_GetObjectDepth(player) - 1;
  position[0] = player->x.fixed;
  position[1] = player->y.fixed;
  position[2] = player->z.fixed;
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
  sprite->scaleX = scaleX;
  sprite->scaleY = scaleY;
  EffectSprite_Draw(sprite);
}

/// Hover cast on a target: runs FieldPsynergy_Hover.
void FieldPsynergy_HoverOnTarget(void) { FieldPsynergy_Hover(); }
#undef HOVER_GLOW_PALETTE
#undef GLOW_SPENT_CUE

/// Hover: a glowing ring grows under the leader, who rises and floats
/// (LEADER_MOVEMENT_HOVER) while HoverGlow_Update holds the glow up. Where it
/// cannot hover (off a hover cell, or touching a solid actor) the ring shrinks
/// away and the place's Hover event runs instead.
void FieldPsynergy_Hover(void) {
  struct HoverWork *work =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct HoverWork));
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *player = scene->mainActor;
  struct PendingEvent *pending;
  struct EffectSprite *sprite;
  struct Actor *other;
  FIXED position[3];
  s32 blocked;
  s32 slot;
  struct Actor **list;
  s32 tile;
  s32 i;
  s32 n;

  pending = Scene_FindPsynergyTrigger(
      PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS), scene->fieldPsynergy,
      Scene_SelectPsynergyTarget(scene->fieldPsynergy));
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_RING_ORB), work);
  slot = VramCache_FindFreeEntry();
  tile = VramCache_Load(slot, 2048, work);
  work->resource = slot;
  work->tile = tile;
  work->scale = 0;
  sprite = &work->sprite;
  EffectSprite_Init(sprite, 15, 15, OBJ_SIZE_32X32, (s16)work->tile);
  sprite->oam.fullColor = 1;
  sprite->oam.palette = 0;
  Actor_SetPositionAndResetMotionFar(player, player->x.fixed, player->y.fixed,
                                     player->z.fixed);
  BattleEffect_SetObjectUpdateMode(player, 0);
  Actor_SelectAnimationFar(player, ANIM_LEADER_RUN);
  Sound_PlayFar(SE_PSYNERGY_START);
  wait(1);
  // DEAD: nothing reads it; the ROM keeps the store.
  work->unused = 0;
  work->phase = 0;
  work->fading = 0;
  Task_Add(HoverGlow_Update, TASK_PASS_FRAME);

  blocked = 0;
  if (MapGrid_IsHoverCell(player->x.fixed, player->z.fixed, player->layer) ==
      0) {
    blocked = 1;
  } else {
    for (n = 0; n < FIELD_ACTOR_COUNT; n++) {
      other = event->actors[n];
      if (other != NULL && other->script != NULL &&
          (other->collisionFlags & ACTOR_SOLID) && other != scene->mainActor) {
        position[0] = other->x.fixed;
        position[1] = other->groundHeight;
        position[2] = other->z.fixed;
        if (FieldPsynergy_CheckOverlap(&player->x.fixed, player->radius,
                                       &other->x.fixed,
                                       other->radius - 2) >= 0) {
          blocked = 1;
          break;
        }
      }
    }
  }

  if (blocked) {
    wait(5);
    Sound_PlayFar(SE_GLITTER);
    for (i = 0; i < HOVER_RING_GROW_FRAMES; i++) {
      work->scale = FIXED_ONE * i / HOVER_RING_GROW_FRAMES;
      wait(1);
    }
    wait(10);
    if (pending != NULL)
      Field_RunPsynergyEvent(pending, scene->firstActorId,
                             scene->secondActorId);
    for (i = HOVER_RING_GROW_FRAMES - 1; i != 0; i--) {
      work->scale = FIXED_ONE * i / HOVER_RING_GROW_FRAMES;
      wait(1);
    }
    wait(5);
    Task_Remove(HoverGlow_Update);
    VramCache_FreeEntry(work->resource);
    HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  } else {
    player->motionFlags = 0;
    player->y.fixed = player->groundHeight;
    work->scale = 0;
    event->glowHeld = 0;
    // DEAD: HoverGlow_Update holds the glow at GLOW_FULL from the next frame,
    // the leader standing on a hover cell; the ROM keeps the store.
    event->glow = GLOW_RISING;
    for (i = 0; i < HOVER_RING_GROW_FRAMES; i++) {
      if (i == 10)
        Sound_PlayFar(SE_GLITTER);
      player->y.fixed += toFIXED(1);
      work->scale = FIXED_ONE * i / HOVER_RING_GROW_FRAMES;
      wait(1);
    }
    player->motionFlags = ACTOR_FLOATS;
    player->y.fixed = player->groundHeight + toFIXED(73) / 2;
    player->gravity = toFIXED(3);
    gMainRoutineWork.movementMode = LEADER_MOVEMENT_HOVER;
    work->fading = 1;
    event->glowHeld = 1;
    event->glow = GLOW_FULL;
  }
}

#undef GLOW_FULL
#undef GLOW_RISING
#undef HOVER_RING_GROW_FRAMES

/// Ends Hover: the leader drops back to the ground with a splash and the ring
/// is freed.
void FieldPsynergy_EndHover(void) {
  struct HoverWork *work = (struct HoverWork *)Ram_HeapSlots->battleEffect;
  struct Actor *actor = Actor_GetById(Party_GetLeaderActor());
  s32 i;

  Task_Remove(HoverGlow_Update);
  Actor_SetPositionAndResetMotionFar(actor, actor->x.fixed, actor->y.fixed,
                                     actor->z.fixed);
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
  for (i = 0; i < 60; i++) {
    if (actor->y.fixed <= actor->groundHeight)
      break;
    wait(1);
  }
  Sound_PlayFar(SE_WATER);
  wait(1);
  for (i = 0; i < 20; i++) {
    if (actor->y.fixed <= actor->groundHeight)
      break;
    wait(1);
  }
  wait(1);
  for (i = 0; i < 20; i++) {
    if (actor->y.fixed <= actor->groundHeight)
      break;
    wait(1);
  }
  wait(1);
  actor->bounce = 0;
  actor->velocityY = 0;
  actor->y.fixed = actor->groundHeight;
  actor->gravity = toFIXED(1);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}
