/// @file
/// Hover: its glow, the cast and its end.
// Part of field_psynergy.c.

#include "../field_psynergy_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"
#include "field_event.h"

/// A cycle of 96 colour levels (31 down to 0, 32 at 0, then 0 up to 31),
/// read a third of a turn apart for red, green and blue.
extern const u8 gData_080f0fe0[];

/// The colour of field Psynergy kind 24's glow (HoverGlow_Update) for a step of
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

  step = (counter + 12) % 24;
  wave = gData_080f0fe0;
  phase = step * 4;
  redLevel = wave[(s16)(phase % 96)];
  bias = level - 7;
  red = redLevel + bias;
  green = wave[(phase + 32) % 96] + bias;
  blue = wave[(phase + 64) % 96] + bias;
  if (red < 0)
    red = 0;
  if (red > 31)
    red = 31;
  if (green < 0)
    green = 0;
  if (green > 31)
    green = 31;
  if (blue < 0)
    blue = 0;
  if (blue > 31)
    blue = 31;
  return (blue << 10) | (green << 5) | red;
}

#define OBJ_PALETTE_14 ((u16 *)(OBJ_PLTT + 14 * 16))

// The glow's strength, in frames: full at 112, and it fades out a frame at a
// time once the player leaves the place that feeds it.
#define GLOW_FULL 112
#define GLOW_SPENT_CUE 0x2099

// Kind 24's work (battle_fx_work[0]).
struct HoverGlowWork {
  u8 unk0000[0x1000];
  struct EffectSprite sprite;
  s32 scale;
  s16 resource;
  u16 tile;
  u16 unk1030;
  u16 phase;
  s16 fading;
};

struct GlowEvent {
  u8 unk000[0x166];
  u16 cue;
  u8 unk168[0xbdc];
  s16 glow;
  s16 glowHeld;
};

s32 MapGrid_IsHoverCell(s32 x, s32 z, s32 layer);
void Camera_WorldToScreen(s32 *position);

// Kind 24's frame callback: keeps the glow's strength full while the player
// stands where MapGrid_IsHoverCell allows, and runs it down otherwise (once it
// is fading, the glow shrinks with it and the event takes its cue when it is
// spent). It cycles OBJ palette 14 through HoverGlow_GetColor's colours,
// flickers between two pictures every other frame, and draws the glow under the
// player, flattened to three quarters of its height.
void HoverGlow_Update(void) {
  struct HoverGlowWork *work = Ram_HeapSlots->battleEffect;
  struct Actor *player = ObjectTable_Get(Party_GetLeaderActor());
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  u16 *palette;
  s32 position[3];
  s32 level;
  s32 i;
  s32 scaleX;
  s32 scaleY;
  struct EffectSprite *sprite;

  // The glow's strength.
  if (MapGrid_IsHoverCell(player->x, player->z, player->layer) == 0) {
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
    work->scale = (level << 15) / GLOW_FULL + Q16_ONE / 2;
    if (level == 0)
      event->psynergy = GLOW_SPENT_CUE;
  }

  // Cycle the glow's colours; the last takes the 15th's level.
  palette = OBJ_PALETTE_14;
  for (i = 0; i < 16; i++) {
    /* FAKEMATCH(dead): the colour's store in a once-only block. */
    do {
      *palette++ = HoverGlow_GetColor((((s16)work->phase >> 2) + i / 2) % 24,
                                      i == 15 ? 14 : i);
    } while (0);
  }
  work->phase++;

  // Draw it under the player.
  scaleX = work->scale;
  scaleY = scaleX * 6 / 8;
  sprite = &work->sprite;
  sprite->oam.index = work->tile + ((gFrameCount >> 1) & 1) * 32;
  sprite->oam.priority = BattleEffect_GetObjectPriority(player);
  sprite->depth = BattleEffect_GetObjectDepth(player) - 1;
  position[0] = player->x;
  position[1] = player->y;
  position[2] = player->z;
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
  sprite->scaleX = scaleX;
  sprite->scaleY = scaleY;
  EffectSprite_Draw(sprite);
}

/// Runs field Psynergy animation 24 for the dispatcher; the target is unused.
void FieldPsynergy_HoverOnTarget(void) { FieldPsynergy_Hover(); }
#undef VIEW_MOVE_WORK_SIZE
#undef SWAY_START
#undef SWAY_PEAK
#undef SWAY_RAMP
#undef SWAY_HOLD_FRAMES
#undef OBJ_PALETTE_14
#undef GLOW_FULL
#undef GLOW_SPENT_CUE

struct GlowScene {
  u8 unk00[0x10];
  struct Actor *mainObject;
  u8 unk14[4];
  s16 unk18;
  s16 unk1a;
  u8 unk1c[2];
  s16 kind;
};

extern char ResourceId_RingOrb;

void FieldPsynergy_Hover(void) {
  struct HoverGlowWork *work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, 0x1038);
  struct GlowScene *scene = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *player = scene->mainObject;
  struct PendingEvent *pending;
  struct EffectSprite *sprite;
  struct Actor *other;
  s32 position[3];
  s32 found;
  s32 slot;
  struct Actor **list;
  s32 tile;
  s32 i;
  s32 n;

  pending = Scene_FindPsynergyTrigger(0x50000005, scene->kind,
                                      Scene_SelectPsynergyTarget(scene->kind));
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_RingOrb), work);
  slot = VramCache_FindFreeEntry();
  tile = VramCache_Load(slot, 0x800, work);
  work->resource = slot;
  work->tile = tile;
  work->scale = 0;
  sprite = &work->sprite;
  EffectSprite_Init(sprite, 15, 15, 0x80000000, (s16)work->tile);
  sprite->oam.palette256 = 1;
  sprite->oam.palette = 0;
  Actor_SetPositionAndResetMotionFar(player, player->x, player->y, player->z);
  BattleEffect_SetObjectUpdateMode(player, 0);
  Actor_SelectAnimationFar(player, 5);
  Sound_PlayFar(SE_PSYNERGY_START);
  wait(1);
  work->unk1030 = 0;
  work->phase = 0;
  work->fading = 0;
  Task_Add(HoverGlow_Update, TASK_PASS_VBLANK);

  found = 0;
  if (MapGrid_IsHoverCell(player->x, player->z, player->layer) == 0) {
    found = 1;
  } else {
    for (n = 0; n < 80; n++) {
      other = event->actors[n];
      if (other != NULL && other->script != NULL &&
          (other->collisionFlags & 1) && other != scene->mainObject) {
        position[0] = other->x;
        position[1] = other->groundHeight;
        position[2] = other->z;
        if (FieldPsynergy_CheckOverlap(&player->x, player->radius, &other->x,
                                       other->radius - 2) >= 0) {
          found = 1;
          break;
        }
      }
    }
  }

  if (found) {
    wait(5);
    Sound_PlayFar(SE_GLITTER);
    for (i = 0; i < 32; i++) {
      work->scale = i << 11;
      wait(1);
    }
    wait(10);
    if (pending != NULL)
      Field_RunPsynergyEvent(pending, scene->unk18, scene->unk1a);
    for (i = 31; i != 0; i--) {
      work->scale = i << 11;
      wait(1);
    }
    wait(5);
    Task_Remove(HoverGlow_Update);
    VramCache_FreeEntry(work->resource);
    HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  } else {
    player->motionFlags = 0;
    player->y = player->groundHeight;
    work->scale = 0;
    event->glowHeld = 0;
    event->glow = 100;
    for (i = 0; i < 32; i++) {
      if (i == 10)
        Sound_PlayFar(SE_GLITTER);
      player->y += px(1);
      work->scale = i << 11;
      wait(1);
    }
    player->motionFlags = 4;
    player->y = player->groundHeight + 0x248000;
    player->gravity = px(3);
    gMainRoutineWork.movementMode = 10;
    work->fading = 1;
    event->glowHeld = 1;
    event->glow = 112;
  }
}

// Kind 24's work (battle_fx_work[0]).

void FieldPsynergy_EndHover(void) {
  struct HoverGlowWork *work =
      (struct HoverGlowWork *)Ram_HeapSlots->battleEffect;
  struct Actor *object = Actor_GetById(Party_GetLeaderActor());
  s32 i;

  Task_Remove(HoverGlow_Update);
  Actor_SetPositionAndResetMotionFar(object, object->x, object->y, object->z);
  object->motionFlags = 3;
  gMainRoutineWork.movementMode = 0;
  for (i = 0; i < 60; i++) {
    if (object->y <= object->groundHeight)
      break;
    wait(1);
  }
  Sound_PlayFar(SE_WATER);
  wait(1);
  for (i = 0; i < 20; i++) {
    if (object->y <= object->groundHeight)
      break;
    wait(1);
  }
  wait(1);
  for (i = 0; i < 20; i++) {
    if (object->y <= object->groundHeight)
      break;
    wait(1);
  }
  wait(1);
  object->bounce = 0;
  object->velocityY = 0;
  object->y = object->groundHeight;
  object->gravity = px(1);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}
