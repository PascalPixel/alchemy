/// @file
/// Cyclone and the whirlwinds: the 96-particle burst, the leader carried or
/// lifted to a Cyclone cell, Cyclone itself, an actor leaving or arriving in a
/// whirlwind (Actor_DepartInRing, Actor_ArriveInRing), then Blaze's jet of
/// flame and the smoke it leaves.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "sounds.h"
#include "macros.h"
#include "field/effect_sprite.h"
#include "graphics/resource.h"
#include "system/task.h"
#include "fixed_math.h"
#include "field/actor.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "battle/motobj.h"
#include "field/canvas_sprite.h"
#include "field/actor_view.h"
#include "system/frame.h"

/// The 96-particle burst's task (ParticleBurstState in HEAP_SLOT_BATTLE_EFFECT,
/// started by ParticleBurst_Start). Each particle whose delay counter is
/// between 0 and 63 shows the picture (delay / 8) % 6 of the burst's tiles 48
/// pixels out along its heading, then turns by its spin, rises by its speed and
/// spins eight units faster; every counter advances. The state's flash flag is
/// set on frame 30 only, and the burst stops being active on frame 112.
void ParticleBurst_Update(void) {
  struct ParticleBurstState *state;
  struct BurstParticle *particle;
  struct EffectSprite *sprite;
  s32 position[3];
  s32 i;

  state = (struct ParticleBurstState *)Ram_HeapSlots->battleEffect;
  particle = state->particles;

  for (i = 0; i < PARTICLE_BURST_COUNT; i++, particle++) {
    sprite = &state->sprites[i];
    if ((u32)particle->delay < 64) {
      sprite->oam.index = state->tile + (particle->delay >> 3) % 6 * 2;
      position[0] = particle->x;
      position[1] = particle->y;
      position[2] = particle->z;
      Vector_AddPolarOffset(toFIXED(48), particle->heading, position);
      Camera_WorldToScreen(position);
      sprite->x = position[0];
      sprite->y = position[2];
      EffectSprite_Draw(sprite);
      particle->heading += particle->spin;
      particle->y += particle->speed;
      particle->spin =
          particle->spin >= 0 ? particle->spin + 8 : particle->spin - 8;
    }
    particle->delay++;
  }
  state->flash = 0;
  if (state->frame == 30)
    state->flash = 1;
  if (state->frame == 112)
    state->active = 0;
  state->frame++;
}

/// Ends the particle burst (ParticleBurst_Start): stops its task and frees its
/// tiles.
void ParticleBurst_End(void) {
  struct ParticleBurstState *state = Ram_HeapSlots->battleEffect;

  Task_Remove((u32)ParticleBurst_Update);
  VramCache_FreeEntry(state->cacheEntry);
}

/// The frames the whirlwind takes to carry or lift the leader.
#define CYCLONE_CARRY_FRAMES 128

/// How far the whirlwind turns the leader round each frame of the ride.
#define CYCLONE_CARRY_TURN_STEP (TRIG_TURN * 5 / 256)

/// Task of the Cyclone: the whirlwind carries the leader over 128 frames from
/// where it stood to the nearby CYCLONE_CELL_CARRY cell (a Cyclone teleporter),
/// circling and arcing up and down on the way, its follower with it.
void Cyclone_CarryLeader(void) {
  struct FieldEffectState *effect = Ram_HeapSlots->fieldEffectState;
  struct CycloneState *state = Ram_HeapSlots->battleEffect;
  struct Actor *owner = effect->caster;
  s32 timer = state->timer;
  s32 position[3];

  switch (state->state) {
  case 0:
    state->radius = Trig_Sin(timer * (TRIG_TURN / 2 / CYCLONE_CARRY_FRAMES))
                    << 5;
    state->angle += CYCLONE_CARRY_TURN_STEP;
    position[0] = state->from[0] + (state->to[0] - state->from[0]) * timer /
                                       CYCLONE_CARRY_FRAMES;
    position[1] =
        state->from[1] +
        (state->to[1] - state->from[1]) * timer / CYCLONE_CARRY_FRAMES +
        (Trig_Sin(timer * (TRIG_TURN / 2 / CYCLONE_CARRY_FRAMES)) << 6);
    position[2] = state->from[2] + (state->to[2] - state->from[2]) * timer /
                                       CYCLONE_CARRY_FRAMES;
    Vector_AddPolarOffset(state->radius, state->angle, position);
    SET_ACTOR_POSITION(owner, position[0], position[1], position[2]);
    if (timer >= CYCLONE_CARRY_FRAMES) {
      state->state++;
      state->timer = -1;
    }
    break;
  case 1:
    state->state = FIELD_PSYNERGY_FINISHED;
    break;
  }
  state->timer++;
  state->follower->x.fixed = owner->x.fixed;
  state->follower->y.fixed = owner->y.fixed;
  state->follower->z.fixed = owner->z.fixed;
}

/// Cyclone_CarryLeader for a CYCLONE_CELL_LIFT cell: the leader is carried
/// there 32 pixels up, circling, without the arc.
void Cyclone_LiftLeader(void) {
  struct FieldEffectState *effect = Ram_HeapSlots->fieldEffectState;
  struct CycloneState *state = Ram_HeapSlots->battleEffect;
  struct Actor *owner = effect->caster;
  s32 timer = state->timer;
  s32 position[3];

  switch (state->state) {
  case 0:
    state->radius = Trig_Sin(timer * (TRIG_TURN / 2 / CYCLONE_CARRY_FRAMES))
                    << 5;
    state->angle += CYCLONE_CARRY_TURN_STEP;
    position[0] = state->from[0] + (state->to[0] - state->from[0]) * timer /
                                       CYCLONE_CARRY_FRAMES;
    position[1] = state->from[1] + (state->to[1] - state->from[1]) * timer /
                                       CYCLONE_CARRY_FRAMES;
    position[2] = state->from[2] + (state->to[2] - state->from[2]) * timer /
                                       CYCLONE_CARRY_FRAMES;
    Vector_AddPolarOffset(state->radius, state->angle, position);
    SET_ACTOR_POSITION(owner, position[0], position[1], position[2]);
    if (timer >= CYCLONE_CARRY_FRAMES) {
      state->state++;
      state->timer = -1;
    }
    break;
  case 1:
    owner->y.fixed -= toFIXED(2) / 5;
    if (effect->sceneVariant == 1) {
      if (timer == 40)
        state->state = FIELD_PSYNERGY_FINISHED;
    } else if (timer == 40) {
      state->state++;
      state->timer = -1;
    }
    break;
  case 2:
    owner->y.fixed += toFIXED(19) / 5;
    if (timer == 40)
      state->state = FIELD_PSYNERGY_FINISHED;
    break;
  }
  state->timer++;
  state->follower->x.fixed = owner->x.fixed;
  state->follower->y.fixed = owner->y.fixed;
  state->follower->z.fixed = owner->z.fixed;
}

/// Per-frame update: turns an actor 1/16 turn a frame.
void Actor_UpdateSpin(struct MotionObject *object) {
  object->angle += TRIG_TURN / 16;
}

/// Cyclone cast on a target: runs FieldPsynergy_Cyclone.
void FieldPsynergy_CycloneOnTarget(void) { FieldPsynergy_Cyclone(); }

/// The frames the Cyclone's whirlwind takes to rise to full height, and the
/// frames its whirl of wind is drawn for, then whirls on as the Cyclone acts.
#define CYCLONE_WHIRLWIND_RISE_FRAMES 20
#define CYCLONE_WHIRL_FRAMES 60
#define CYCLONE_SPIN_FRAMES 16
/// A whirlwind widens or narrows by WHIRLWIND_RESIZE_STEP of its width a
/// frame, and an actor by the same of its size: the frames for the whole
/// whirlwind and for half an actor.
#define WHIRLWIND_RESIZE_STEP (FIXED_ONE / 32)
#define WHIRLWIND_RESIZE_FRAMES 32
#define ACTOR_RESIZE_FRAMES 16
/// The whirl's arcs spread by a step that keeps CYCLONE_SPREAD_KEPT of every
/// CYCLONE_SPREAD_SCALE parts of itself each frame.
#define CYCLONE_SPREAD_KEPT 123
#define CYCLONE_SPREAD_SCALE 128

/// Cyclone ("Conjure wind to scatter weeds."): the caster spins in a whirlwind
/// of blue and white drawn on the canvas sprite, and the weeds on the cells
/// round it (FieldPsynergy_GatherNearbyCells) are scattered.
void FieldPsynergy_Cyclone(void) {
  struct CycloneState *state =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct CycloneState));
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct FieldEffectState *effect = Ram_HeapSlots->fieldEffectState;
  struct Actor *owner = effect->caster;
  s16 *points = &field->psynergyCells[0];
  FIXED spread = toFIXED(4);
  s32 step = toFIXED(7) / 8;
  struct CanvasSpriteState *canvas;
  struct Actor *whirlwind;
  u16 *offsets;
  u8 *pixels;
  u16 *palette;
  FIXED pos[3];
  s32 mode;
  s32 frame;
  s32 i, j;
  s32 blue, c;
  s32 point;

  FieldPsynergy_FadeToCastColor();
  FieldPsynergy_GatherNearbyCells();
  BattleEffect_SetObjectUpdateMode(effect->caster, 0);
  owner->update = Actor_UpdateSpin;
  owner->timer = owner->facing;
  // The whirl's colours: blue full, red and green rising from half to full.
  palette = (u16 *)&OBJ_PLTT_COLOR(CANVAS_PALETTE, 0);
  blue = RGB(0, 0, RGB_LEVEL_MAX);
  for (j = 0; j < PALETTE_COLORS; j++) {
    c = j + (RGB_LEVEL_MAX + 1) / 2;
    Io_Write16(RGB(0, c, 0) | blue | c, palette);
    palette++;
  }
  BattleEffect_StartCanvasSprite();
  canvas = Ram_HeapSlots->canvasSprite;
  pos[0] = effect->caster->x.fixed;
  pos[1] = effect->caster->y.fixed;
  pos[2] = effect->caster->z.fixed;
  Camera_WorldToScreen(pos);
  canvas->x = (s16)FIXEDtoINT(pos[0]);
  canvas->y = (s16)FIXEDtoINT(pos[2]);
  canvas->priority = 1;
  canvas->depth = OAM_DEPTH_WINDOW;
  canvas->mode = CANVAS_SPRITE_KEPT;
  canvas->scaleX = FIXED_ONE * 3 / 2;
  canvas->scaleY = FIXED_ONE * 3 / 2;
  EffectSprite_Init(&canvas->sprite, 32, 32, OBJ_SIZE_64X64, canvas->tile);
  canvas->sprite.oam.fullColor = 1;
  canvas->sprite.oam.palette = 0;
  offsets = Ram_HeapSlots->canvasMap;
  pixels = Ram_HeapSlots->battleCanvas;
  whirlwind = Actor_Spawn(SPRITE_WHIRLWIND, owner->x.fixed,
                          owner->y.fixed - toFIXED(6), owner->z.fixed);
  Actor_SelectAnimationFar(whirlwind, 2);
  whirlwind->scaleX = FIXED_ONE * 3 / 2;
  whirlwind->scaleY = 0;
  wait(1);
  Sound_PlayFar(SE_RUSH);
  for (i = 0; i < CYCLONE_WHIRLWIND_RISE_FRAMES; i++) {
    whirlwind->scaleY = FIXED_ONE * i / CYCLONE_WHIRLWIND_RISE_FRAMES;
    wait(1);
  }
  for (i = 0; i < CYCLONE_WHIRL_FRAMES; i++) {
    for (j = 0; j < 4; j++)
      Cyclone_PlotArc(
          FIXEDtoINT((s32)(spread + Random16() * 4 - INTtoFIXED(j))),
          Random16(), offsets, pixels);
    spread += step;
    if (spread > toFIXED(28))
      spread = toFIXED(28);
    step = step * CYCLONE_SPREAD_KEPT / CYCLONE_SPREAD_SCALE;
    canvas->angle += TRIG_TURN / 64;
    canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
    wait(1);
  }
  FieldPsynergy_FadeFromCastColor();
  state->follower = whirlwind;
  state->from[0] = owner->x.fixed;
  state->from[1] = owner->y.fixed;
  state->from[2] = owner->z.fixed;
  state->to[0] = owner->x.fixed;
  state->to[1] = owner->y.fixed;
  state->to[2] = owner->z.fixed;
  state->angle = 0;
  state->state = 0;
  state->timer = 0;
  state->state = 0;
  if (field->cycloneLiftX != -1)
    mode = CYCLONE_CELL_LIFT;
  else if (field->cycloneCarryX != -1)
    mode = CYCLONE_CELL_CARRY;
  else {
    point = points[0];
    mode = point != -1;
  }

  if (mode == CYCLONE_CELL_LIFT) {
    if (state->kind)
      ParticleBurst_Start();
    for (i = 0; i < CYCLONE_SPIN_FRAMES; i++) {
      canvas->angle += TRIG_TURN / 64;
      canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
      wait(1);
    }
    FieldCamera_MoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    Actor_SetPositionAndResetMotionFar(owner, owner->x.fixed, owner->y.fixed,
                                       owner->z.fixed);
    Actor_SetSpriteFlagsFar(owner, 0);
    Actor_SetSpritePriority(effect->casterId, 1);
    owner->motionFlags = 0;
    frame = 0;
    wait(1);
    state->to[0] = toFIXED(field->cycloneLiftX);
    state->to[1] = owner->y.fixed + toFIXED(32);
    state->to[2] = toFIXED(field->cycloneLiftZ);
    Task_Add(Cyclone_LiftLeader, TASK_PASS_FRAME);
    while (state->state != FIELD_PSYNERGY_FINISHED) {
      if (frame == 30)
        FieldPsynergy_RunTouchedEvents();
      frame++;
      wait(1);
    }
    Task_Remove(Cyclone_LiftLeader);
    if (effect->sceneVariant == 0)
      owner->scaleX = 0;
    whirlwind->scaleX = 0;
    if (state->kind)
      ParticleBurst_End();
  } else if (mode == CYCLONE_CELL_CARRY) {
    if (state->kind)
      ParticleBurst_Start();
    for (i = 0; i < CYCLONE_SPIN_FRAMES; i++) {
      canvas->angle += TRIG_TURN / 64;
      canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
      wait(1);
    }
    FieldCamera_MoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    Actor_SetPositionAndResetMotionFar(owner, owner->x.fixed, owner->y.fixed,
                                       owner->z.fixed);
    Actor_SetSpriteFlagsFar(owner, 0);
    owner->motionFlags = 0;
    wait(1);
    state->to[0] = toFIXED(field->cycloneCarryX);
    state->to[1] = owner->y.fixed;
    state->to[2] = toFIXED(field->cycloneCarryZ);
    Task_Add(Cyclone_CarryLeader, TASK_PASS_FRAME);
    frame = 0;
    while (state->state != FIELD_PSYNERGY_FINISHED) {
      if (frame == 30)
        FieldPsynergy_RunTouchedEvents();
      frame++;
      wait(1);
    }
    Task_Remove(Cyclone_CarryLeader);
    for (i = 0; i < ACTOR_RESIZE_FRAMES; i++) {
      owner->scaleX += -WHIRLWIND_RESIZE_STEP;
      owner->scaleY = owner->scaleX;
      wait(1);
    }
    owner->scaleX = 0;
    for (i = 0; i < WHIRLWIND_RESIZE_FRAMES; i++) {
      whirlwind->scaleX += -WHIRLWIND_RESIZE_STEP;
      whirlwind->scaleY += -(FIXED_ONE / 128);
      wait(1);
    }
    whirlwind->scaleX = 0;
    if (state->kind)
      ParticleBurst_End();
  } else if (mode == CYCLONE_CELL_BURST) {
    Sound_PlayFar(SE_WHOOSH);
    Actor_SelectAnimationFar(whirlwind, 6);
    if (state->kind)
      ParticleBurst_Start();
    for (i = 0; i < CYCLONE_SPIN_FRAMES; i++) {
      canvas->angle += TRIG_TURN / 64;
      canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
      wait(1);
    }
    owner->update = NULL;
    owner->facing = owner->timer;
    frame = 0;
    while (state->count != 0) {
      if (frame == 30)
        FieldPsynergy_RunTouchedEvents();
      wait(1);
      frame++;
    }
    if (state->kind)
      ParticleBurst_End();
  } else {
    Sound_PlayFar(SE_WHOOSH);
    Actor_SelectAnimationFar(whirlwind, 6);
    for (i = 0; i < CYCLONE_SPIN_FRAMES; i++) {
      owner->facing += FACING_STEP;
      canvas->angle += TRIG_TURN / 64;
      canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
      wait(1);
    }
  }
  Sound_PlayFar(SE_WHOOSH);
  Actor_ReleaseFar(whirlwind);
  owner->update = NULL;
  owner->facing = owner->timer;
  BattleEffect_EndCanvasSprite();
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// The frames Actor_DepartInRing's actor circles in its whirlwind.
#define DEPART_FRAMES 128

/// The actor leaves in a whirlwind (SPRITE_WHIRLWIND): as the screen
/// transition starts it circles inside it for DEPART_FRAMES, from 96 pixels up
/// and 32 out down to where it stood, then the whirlwind vanishes with a whoosh
/// and the camera follows the actor again.
void Actor_DepartInRing(s32 actorId) {
  struct Actor *actor = Actor_GetById(actorId);
  s32 angle = actor->facing;
  u8 motionFlags = actor->motionFlags;
  struct Actor *whirlwind;
  FIXED x, y, z;
  s32 spin;
  FIXED position[3];
  s32 i;

  FieldCamera_MoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  actor->motionFlags = 0;
  actor->update = Actor_UpdateSpin;
  whirlwind = Actor_Spawn(SPRITE_WHIRLWIND, actor->x.fixed, actor->y.fixed,
                          actor->z.fixed);
  Actor_SelectAnimationFar(whirlwind, 2);
  whirlwind->scaleX = FIXED_ONE * 3 / 2;
  whirlwind->scaleY = FIXED_ONE;
  x = actor->x.fixed;
  y = actor->y.fixed;
  z = actor->z.fixed;
  spin = 0;
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_StartScreenTransition();
  for (i = 0; i < DEPART_FRAMES; i++) {
    position[0] = x;
    position[1] = y + Trig_Cos(i * (TRIG_TURN / 4 / DEPART_FRAMES)) * 96;
    position[2] = z;
    Vector_AddPolarOffset(Trig_Cos(i * (TRIG_TURN / 4 / DEPART_FRAMES)) * 32,
                          spin, position);
    SET_ACTOR_POSITION(actor, position[0], position[1], position[2]);
    spin += TRIG_TURN / 64;
    SET_ACTOR_POSITION(whirlwind, actor->x.fixed, actor->y.fixed - toFIXED(6),
                       actor->z.fixed);
    wait(1);
  }
  Actor_SetPositionAndResetMotionFar(actor, x, y, z);
  Sound_PlayFar(SE_WHOOSH);
  Actor_SelectAnimationFar(whirlwind, 6);
  Event_Wait(15);
  Actor_ReleaseFar(whirlwind);
  actor->update = NULL;
  actor->facing = angle;
  actor->motionFlags = motionFlags;
  wait(1);
  FieldCamera_FollowActor(actorId, 1);
}

/// The actor arrives in a whirlwind, Actor_DepartInRing's counterpart: once
/// the screen transition is done, a whirlwind (SPRITE_WHIRLWIND) widens on the
/// unseen, spinning actor for WHIRLWIND_RESIZE_FRAMES, then vanishes with a
/// whoosh as the actor grows back from half its size and shows again.
void Actor_ArriveInRing(s32 actorId) {
  struct Actor *actor = Actor_GetById(actorId);
  s32 angle = actor->facing;
  struct Actor *whirlwind;
  s32 i;

  actor->update = Actor_UpdateSpin;
  actor->scaleX = 0;
  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_SetSpriteFlagsFar(Actor_GetById(actorId), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  whirlwind = Actor_Spawn(SPRITE_WHIRLWIND, actor->x.fixed, actor->y.fixed,
                          actor->z.fixed);
  Actor_SelectAnimationFar(whirlwind, 2);
  whirlwind->scaleX = FIXED_ONE / 2;
  whirlwind->scaleY = FIXED_ONE * 3 / 4;
  for (i = 0; i < WHIRLWIND_RESIZE_FRAMES; i++) {
    whirlwind->scaleX += WHIRLWIND_RESIZE_STEP;
    whirlwind->scaleY += FIXED_ONE / 128;
    wait(1);
  }
  whirlwind->scaleX = FIXED_ONE * 3 / 2;
  whirlwind->scaleY = FIXED_ONE;
  Sound_PlayFar(SE_WHOOSH);
  Actor_SelectAnimationFar(whirlwind, 6);
  actor->scaleY = actor->scaleX = FIXED_ONE / 2;
  for (i = 0; i < ACTOR_RESIZE_FRAMES; i++) {
    actor->scaleY = actor->scaleX = actor->scaleX + WHIRLWIND_RESIZE_STEP;
    wait(1);
  }
  Actor_SetSpriteFlagsFar(Actor_GetById(actorId), SPRITE_FLAG_SHADOW);
  Event_Wait(10);
  Actor_ReleaseFar(whirlwind);
  actor->update = NULL;
  actor->facing = angle;
}

/// What Blaze does (BlazeState.mode).
enum BlazeMode {
  BLAZE_FLICKER = 0,     ///< cast at nothing with no torch in reach: a flicker
  BLAZE_LIGHT_TORCH = 1, ///< cast at nothing: lights the nearest torch in reach
  BLAZE_SPRAY = 2,       ///< cast at a target: the jet of flame
  /// The jet, faster and longer (FieldState.blazeLongSpray).
  BLAZE_LONG_SPRAY = 3
};

/// The flames of Blaze's jet, and the smoke puffs they leave.
#define BLAZE_DROPS 16
/// The frames a flame of the jet lives, in BLAZE_LONG_SPRAY and otherwise.
#define BLAZE_LONG_FLAME_FRAMES 33
#define BLAZE_FLAME_FRAMES 25

/// Blaze's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the flame tiles, 16
/// flames and 16 smoke puffs with their motion, where the jet starts and where
/// it points, and the actor it has reached.
struct BlazeState {
  u8 unused[4096]; ///< no code names it
  struct EffectSprite drops[BLAZE_DROPS];
  struct FieldPsynergyParticle dropMotion[BLAZE_DROPS];
  struct EffectSprite puffs[BLAZE_DROPS];
  struct FieldPsynergyParticle puffMotion[BLAZE_DROPS];
  s32 origin[3];
  u16 dropTile;
  u16 puffTile;
  u16 angle;
  u16 targetAngle;
  s16 hitObject;
  s8 hit;
  s8 emitting;
  s32 speed;
  s32 lifetime;
  s32 gravity;
  s8 mode; ///< enum BlazeMode
};

/// The frame callback of FieldPsynergy_Blaze's spray. Every fourth frame while
/// it is emitting, a free one of sixteen flames starts at the origin with the
/// mode's speed (BLAZE_LONG_SPRAY faster, longer and lighter) along the
/// spray's angle.
/// Each live flame grows, cycles its four tiles, is drawn at its position, and
/// dies after the mode's lifetime, on reaching an object of the event state's
/// list (whose index is kept and which stops the spray for the frame) or on
/// something solid; an odd flame that dies leaves one of sixteen puffs of smoke
/// that rise for 32 frames. The angle then turns toward the scene's facing by
/// at most 1/64 turn a frame.
void Blaze_UpdateSpray(void) {
  struct BlazeState *state = (struct BlazeState *)Ram_HeapSlots->battleEffect;
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct EffectSprite *sprite;
  struct FieldPsynergyParticle *particle;
  struct FieldPsynergyParticle *puff;
  s32 pos[3];
  s32 i;
  s32 j;
  u16 target;
  s32 turn;

  if (state->emitting != 0 && (gFrameCount & 3) == 0) {
    particle = state->dropMotion;
    for (i = 0; i < BLAZE_DROPS; i++, particle++) {
      if (particle->timer == -1) {
        sprite = &state->drops[i];
        if (scene->target != NULL)
          sprite->depth = Actor_GetSpriteDepth(scene->target);
        else
          sprite->depth = Actor_GetSpriteDepth(scene->caster);
        if (state->mode == BLAZE_LONG_SPRAY) {
          state->speed = toFIXED(12) / 5;
          state->lifetime = BLAZE_LONG_FLAME_FRAMES;
          state->gravity = toFIXED(3) / 32;
        } else {
          state->speed = toFIXED(9) / 5;
          state->lifetime = BLAZE_FLAME_FRAMES;
          state->gravity = toFIXED(1) / 8;
        }
        SET_POSITION(particle, state->origin[0], state->origin[1],
                     state->origin[2]);
        particle->velocityX = 0;
        particle->velocityY = -(toFIXED(6) / 5);
        particle->velocityZ = 0;
        Vector_AddPolarOffset(state->speed, state->angle, &particle->velocityX);
        particle->timer = 0;
        break;
      }
    }
  }

  state->hit = 0;
  particle = state->dropMotion;
  for (i = 0; i < BLAZE_DROPS; i++, particle++) {
    if (particle->timer == -1)
      continue;
    sprite = &state->drops[i];
    sprite->scaleY = sprite->scaleX =
        particle->timer * (FIXED_ONE / 25) + FIXED_ONE / 2;
    sprite->oam.index =
        state->dropTile + (((particle->timer >> 1) + i) & 3) * 32;
    if (state->angle > DEGtoANG(180))
      sprite->depth--;
    else
      sprite->depth++;
    EffectSprite_DrawAtWorldPosition(sprite, particle);
    if (++particle->timer == state->lifetime)
      particle->timer = -1;
    for (j = 0; j < FIELD_ACTOR_COUNT; j++) {
      struct Actor *other = field->actors[j];
      if (other != NULL && other->script != NULL &&
          (other->collisionFlags & ACTOR_SOLID) && other != scene->caster &&
          other != scene->target &&
          FieldPsynergy_CheckOverlap(&particle->x, 2, &other->x.fixed,
                                     other->radius - 2) >= 0) {
        state->hit = 1;
        state->hitObject = j;
        particle->timer = -1;
        break;
      }
    }
    if (FIELD_CELL(particle->x) != FIELD_CELL(state->origin[0]) ||
        FIELD_CELL(particle->z) != FIELD_CELL(state->origin[2])) {
      pos[0] = particle->x;
      pos[1] = particle->y;
      pos[2] = particle->z;
      if (MapGrid_CheckCollisionFar(scene->caster, pos) > 0)
        particle->timer = -1;
    }
    if (particle->timer == -1 && (i & 1)) {
      pos[0] = particle->x;
      pos[1] = particle->y - (Random16() << 2);
      pos[2] = particle->z;
      Vector_AddPolarOffset(toFIXED(10), state->angle + DEGtoANG(180), pos);
      Vector_AddPolarOffset(toFIXED(12), Random16(), pos);
      puff = &state->puffMotion[i];
      SET_POSITION(puff, pos[0], pos[1], pos[2]);
      puff->timer = 0;
    }
    if (state->hit != 0)
      break;
    Field_AdvanceWithGravity3D(&particle->x, 64, state->gravity);
  }

  particle = state->puffMotion;
  for (i = 0; i < BLAZE_DROPS; i++, particle++) {
    if (particle->timer >= 0) {
      sprite = &state->puffs[i];
      sprite->oam.index = state->puffTile + ((particle->timer >> 2) & 7) * 8;
      pos[0] = particle->x;
      pos[1] = particle->y;
      pos[2] = particle->z;
      Camera_WorldToScreen(pos);
      sprite->x = pos[0];
      sprite->y = pos[2];
      EffectSprite_Draw(sprite);
      particle->y += toFIXED(4) / 5;
      if (++particle->timer == 32)
        particle->timer = -1;
    }
  }

  target = scene->angle8 - DEGtoANG(90);
  turn = (s16)(state->targetAngle - state->angle);
  if ((u16)(state->targetAngle - target) - (u16)(state->angle - target) ==
      TRIG_TURN / 2) {
    state->angle = state->angle + TRIG_TURN / 64;
  } else {
    if (turn > TRIG_TURN / 64)
      turn = TRIG_TURN / 64;
    if (turn < -(TRIG_TURN / 64))
      turn = -(TRIG_TURN / 64);
    state->angle = state->angle + turn;
  }
}

#undef CYCLONE_WHIRLWIND_RISE_FRAMES
#undef CYCLONE_WHIRL_FRAMES
#undef CYCLONE_SPIN_FRAMES
#undef WHIRLWIND_RESIZE_STEP
#undef WHIRLWIND_RESIZE_FRAMES
#undef ACTOR_RESIZE_FRAMES
#undef DEPART_FRAMES
#undef CYCLONE_SPREAD_KEPT
#undef CYCLONE_SPREAD_SCALE
#undef BLAZE_LONG_FLAME_FRAMES
#undef BLAZE_FLAME_FRAMES

/// Blaze cast on a target: runs FieldPsynergy_Blaze.
void FieldPsynergy_BlazeOnTarget(void) { FieldPsynergy_Blaze(); }
