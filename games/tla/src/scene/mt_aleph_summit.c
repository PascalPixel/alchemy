/// @file
/// Overlay 0x6b4's scene: its code between field/field_effect/move and field/field_effect/advance_motion, then its tables, in ROM order.

#include "types.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "event_inline.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

#include "objcmd.h"
#include "frame.h"

extern void LinkLobby_RunRoundResult(struct FieldEffect *effect, s32 mode);
extern void Func_02001464(struct FieldEffect *effect, const s32 *script);
extern s32 gData_02001650[];
void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct FieldEffect *party = ((struct FieldEffect *)Actor_GetById(gMainRoutineWork.currentOwner));
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Import_ActorCreate(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  LinkLobby_RunRoundResult(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Func_02001464(effect, gData_02001650[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSingleChildFlags(effect, 0);
  effect->update = BattleEffect_UpdateObjectMotionScaleAndLinkedAngle;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    Actor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 targetX;
    s32 count;

    script = gData_02001650[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - 0x10000) / count;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    LinkLobby_RunRoundResult(effect, 1);
    Func_02001464(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Func_020002a4(u16 *colors, u16 *target, s32 count) {
  s32 i = 0;

  if (count == 0)
    return;
next:
  {
    s32 to_r = *target & 31;
    s32 to_g = (*target >> 5) & 31;
    s32 to_b = (*target >> 10) & 31;
    s32 r = *colors & 31;
    s32 g = (*colors >> 5) & 31;
    s32 b = (*colors >> 10) & 31;

    if (r < to_r)
      r++;
    else if (r > to_r)
      r--;
    if (g < to_g)
      g++;
    else if (g > to_g)
      g--;
    if (b < to_b)
      b++;
    else if (b > to_b)
      b--;
    *colors = b << 10 | g << 5 | r;
  }
  i++;
  target++;
  colors++;
  if (i != count)
    goto next;
}

void Func_0200032c(u16 *colors, s32 count, s32 delta) {
  s32 i;
  s32 r, g, b;

  i = 0;
  if (count == 0)
    return;
loop:
  r = (*colors & 0x1f) + delta;
  g = ((*colors >> 5) & 0x1f) + delta;
  b = ((*colors >> 10) & 0x1f) + delta;
  if (r > 31)
    r = 31;
  if (r < 0)
    r = 0;
  if (g > 31)
    g = 31;
  if (g < 0)
    g = 0;
  if (b > 31)
    b = 31;
  if (b < 0)
    b = 0;
  *colors++ = (b << 10) | (g << 5) | r;
  i++;
  if (i != count)
    goto loop;
}

void Func_02000394(void) {
  u16 *src = (u16 *)0x050000c2;
  u16 *dst = (u16 *)0x05000082;
  s32 delta = (Math_Sin((gFrameCount << 11) >> 1) * 2) >> 16;
  s32 i = 0;

next:
  {
    s32 r = (*src & 31) + delta;
    s32 g = ((*src >> 5) & 31) + delta;
    s32 b = ((*src >> 10) & 31) + delta;

    if (r > 31)
      r = 31;
    if (r < 0)
      r = 0;
    if (g > 31)
      g = 31;
    if (g < 0)
      g = 0;
    if (b > 31)
      b = 31;
    if (b < 0)
      b = 0;
    *dst = b << 10 | g << 5 | r;
  }
  i++;
  src++;
  dst++;
  if (i != 15)
    goto next;
}

extern u32 gData_0200159c[];
extern u32 gData_020015d8[];
extern u32 gData_02001614[];

// clang-format off
u32 gData_0200159c[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_020015d8[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02001614[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};
// clang-format on

s32 gData_02001650[] = {
    (s32)gData_0200159c,
    (s32)gData_020015d8,
    (s32)gData_02001614,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {57, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(344), FACING_NORTHWEST, 0,
     0},
    {311, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(24), FACING_SOUTHEAST, 0,
     0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGERS_END},
};
