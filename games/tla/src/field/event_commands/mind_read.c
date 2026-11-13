/// @file
/// The field Psynergy from Mind Read to Cloak, as The Broken Seal's
/// event_commands.c has them in its parts mind_read.c to cloak.c: Mind Read's
/// wave, Move, Reveal, Growth, Lift, Force, Frost, Ply and the start of Cloak.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "dma.h"
#include "io_reg.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "objdisp.h"
#include "frame.h"
#include "input.h"
#include "edition.h"
#include "fixed_math.h"
#include "object_effect_scripts.h"
#include "field_event.h"
#include "field/field_psynergy_work.h"
#include "field_particle.h"
#include "debug.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "field_psynergy_work.h"
#include "display_transition.h"
#include "event_work.h"
#include "task.h"
#include "actor_view.h"
#include "field_sprite.h"
#include "scene.h"
#include "macros.h"
#include "sprite.h"

// The parts, in ROM order.
#include "event.h"
#include "module/field/event_commands.h"
#include "module/system/system.h"

/* VIEW(f0dcb44): routine BattleEffect_ApplyColorToTargetBuffer as void
 * BattleEffect_ApplyColorToTargetBuffer(u32 color, s32 mode); event.h has void
 * BattleEffect_ApplyColorToTargetBuffer(s32 value, s32 mode). */
extern void BattleEffect_ApplyColorToTargetBuffer__f0dcb44(
    u32 color, s32 mode) __asm__("BattleEffect_ApplyColorToTargetBuffer");
/* VIEW(f0dcb44): routine Scene_FindActorPlacement as struct WaveFxDescriptor
 * *Scene_FindActorPlacement(s32 id); battle.h has struct ActionDescriptor
 * *Scene_FindActorPlacement(s32 id). */
extern struct WaveFxDescriptor *
Scene_FindActorPlacement__f0dcb44(s32 id) __asm__("Scene_FindActorPlacement");
/* VIEW(f0dcb44): routine Resource_GetMetadataRecordFarFar as struct
 * WaveFxMetadata *Resource_GetMetadataRecordFarFar(s32 sprite);
 * sprite_metadata.h has struct AnimationMetadata
 * *Resource_GetMetadataRecordFarFar(u32 number). */
extern struct WaveFxMetadata *Resource_GetMetadataRecordFarFar__f0dcb44(
    s32 sprite) __asm__("Resource_GetMetadataRecordFarFar");
struct Actor *Actor_CreateFar(s32 kind, s32 x, s32 y, s32 z);

// Each frame after the delay: rewrites the 160-line wave into the other
// page, cycles the colour every other frame, and at counts 0, 8 and 16 of
// its 61-frame cycle sends a particle from the target back to the user.
void FieldPsynergy_UpdateMindReadWave(void) {
  struct WaveFxWork *work = Ram_HeapSlots->unk4c[3];
  struct WaveFxVector from;
  struct WaveFxVector to;
  struct WaveFxDescriptor *descriptor;
  s32 spriteId;
  struct WaveFxMetadata *metadata;
  struct WaveFxSprite *sprite;
  struct Actor *source;
  struct Actor *target;
  struct Actor *object;
  u16 *line;
  s32 i;

  if (work->delay != 0) {
    work->delay--;
    return;
  }

  line = work->lines[work->page ^ 1];
  for (i = 0; i < 160; i++)
    *line++ = Trig_Sin(((work->phase + i * 8) << 16) / 160) >> 14;
  work->phase += 4;
  work->page ^= 1;
  if (work->page != 0) {
    FieldEffect_AdvanceHueCycle();
    BattleEffect_ApplyColorToTargetBuffer__f0dcb44(
        (work->blue << 10) | (work->green << 5) | work->red | 0x200000, 1);
    PaletteFade_StartInterpolation(1);
  }
  Actor_SetSpritePalette(Actor_GetById(work->sourceId), 0);

  if (work->counter == 0 || work->counter == 8 || work->counter == 16) {
    source = Actor_GetById(work->sourceId);
    target = Actor_GetById(work->targetId);
    if (source != NULL && target != NULL) {
      descriptor = Scene_FindActorPlacement__f0dcb44(work->sourceId);
      spriteId = descriptor->sprite;
      from.x = source->x;
      metadata = Resource_GetMetadataRecordFarFar__f0dcb44(spriteId);
      from.y = source->y + (metadata->boxX << 16) - 0x20000;
      from.z = source->z;
      sprite = target->sprite;
      descriptor = Scene_FindActorPlacement__f0dcb44(work->targetId);
      spriteId = descriptor->sprite;
      to.x = target->x;
      metadata = Resource_GetMetadataRecordFarFar__f0dcb44(spriteId);
      {
        s32 box = metadata->boxX;
        s32 y = target->y;

        y += box << 16;
        to.y = y - 0x20000;
      }
      to.z = target->z;
      if (sprite->orientation == 0x8000) {
        to.y -= 0x180000;
      } else if (sprite->orientation == 0x4000) {
        to.x += 0x60000;
        to.y -= 0xc0000;
      } else if (sprite->orientation == 0xc000) {
        to.x -= 0x60000;
        to.y -= 0xc0000;
      }
      object = Actor_CreateFar(677, to.x, to.y, to.z);
      if (object != NULL) {
        sprite = object->sprite;
        object->motionFlags = 0;
        object->speedLimit = 0xa3d7;
        object->acceleration = 0xa3d7;
        object->facing = ArcTan2(from.z - to.z, from.x - to.x);
        object->update =
            (void (*)(void))BattleEffect_SetCallbackWhenTargetUnset;
        sprite->palette = 0;
        sprite->mode = 1;
        Actor_SetMoveTargetFar(object, from.x, from.y, from.z);
      }
    }
  }

  if (work->counter != 99) {
    if (work->counter == 0)
      Sound_PlayFar(130);
    if (++work->counter > 60)
      work->counter = 0;
  }
}
#include "module/field/event_commands.h"
#include "module/field/object.h"
#include "module/system/system.h"

/// Applies BG0 line scrolling with HBlank DMA.
void FieldEffect_ArmBg0HBlankDma(void) {
  struct WaveFxScroll *work = Ram_HeapSlots->unk4c[3];

  if (!work->delay) {
    u16 *lines = work->lines[work->page];
    volatile u16 *dma = REG_DMA0;

    dma[5] &= 0xc5ff; // Clear start timing and repeat.
    dma[5] &= 0x7fff; // Stop.
    (void)dma[5];
    // One halfword per HBlank, repeating with destination reload.
    Dma_Set(lines, REG_BG0HOFS, DMA_HBLANK_HALFWORD_PER_LINE,
            (volatile u32 *)dma);
  }
}

/// One colour channel's level (0 to 31) at a hue in degrees: rising from 0
/// to 60, full from 60 to 180, falling from 180 to 240, else 0.
s32 FieldEffect_HueChannelRamp(s16 hue) {
  s32 level = 0;
  s16 angle = hue;

  angle = (angle + 360) % 360;
  if (angle < 60) {
    level = angle * 31 / 60;
  } else if (angle < 180) {
    level = 31;
  } else if (angle < 240) {
    level = (240 - angle) * 31 / 60;
  }
  return level;
}

/// Advances the hue by four degrees and calculates its colour.
void FieldEffect_AdvanceHueCycle(void) {
  struct WaveFxWork *work = Ram_HeapSlots->unk4c[3];
  s16 r;
  s16 g;
  s16 b;
  u32 limit;

  r = FieldEffect_HueChannelRamp(work->hue);
  g = FieldEffect_HueChannelRamp(work->hue + 120);
  b = FieldEffect_HueChannelRamp(work->hue + 240);

  // Scale 0-31 down to 4-11.
  work->red = (r / 4 + 4) & 31;
  work->green = (g / 4 + 4) & 31;
  work->blue = (b / 4 + 4) & 31;
  limit = 359 << 16;
  if ((u32)((work->hue += 4) << 16) > limit)
    work->hue = 0;
}

void BattleEffect_SetCallbackWhenTargetUnset(struct Actor *target) {
  s32 tx;
  s32 ty;

  tx = target->targetX;
  if (tx == 0x80000000) {
    ty = target->targetY;
    if ((ty == tx) && (target->targetZ == ty)) {
      Actor_StartScriptFar((struct Actor *)target,
                           (u32)gBattleEffect_UntargetedObjectScript);
    }
  }
}

void Actor_SetSpritePalette(void *object, s32 value) {
  s32 child = value - 1;

  if (value == 0)
    child = gData_080f0e9c[(gFrameCount >> 1) & 7];
  Actor_SetSpriteColorModeFar(object, child);
}

u16 BattleEffect_GetCycledTableWord(void) {
  struct InputState *input = &gInput;
  u16 *words = gBattleEffect_CyclePatternWords;

  return *(u16 *)((u8 *)words + (((input->held >> 4) & 15) << 1));
}

u16 BattleEffect_GetEightWayHeadingWord(void) {
  struct InputState *input = &gInput;
  u16 *words = gBattleEffect_EightWayHeadingWords;

  return *(u16 *)((u8 *)words + (((input->held >> 4) & 15) << 1));
}

// Effects: motion of the main object in an item-breaking effect.

/* VIEW(f0dcb44): routine Vector_AddPolarOffset as void
 * Vector_AddPolarOffset(s32, s32, struct ItemBreakFragmentPosition *);
 * module/system/system.h has void Vector_AddPolarOffset(s32 radius, s32 angle,
 * s32 *position). */
extern void Vector_AddPolarOffset__f0dcb44(
    s32, s32,
    struct ItemBreakFragmentPosition *) __asm__("Vector_AddPolarOffset");
/* VIEW(f0dcb44): routine Actor_Spawn as struct ItemBreakFragmentObject
 * *Actor_Spawn(s32, s32, s32, s32); module/field/event_commands.h has struct
 * Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z). */
extern struct ItemBreakFragmentObject *
Actor_Spawn__f0dcb44(s32, s32, s32, s32) __asm__("Actor_Spawn");

/// Updates the broken item's main object: with a facing target it turns toward
/// it by at most 0x1000 (22.5 degrees), then emits one fragment particle
/// nearby.
/// src: main object.
void FieldEffect_UpdateMoveHand(struct ItemBreakFragmentSource *src) {
  struct ItemBreakFragmentSource *target;
  struct ItemBreakFragmentPosition pos;
  struct ItemBreakFragmentObject *obj;
  s32 turn;
  s32 dist;

  target = src->target;
  if (target != 0) {
    s32 dx = target->x - src->x;
    s32 dz = target->z - src->z;

    if (dx != 0 || dz != 0) {
      s32 dir = ArcTan2(dz, dx);
      s32 cur = src->angle;
      s32 lim = 128;

      turn = (s16)(dir - cur);
      lim <<= 5;
      if (turn > lim)
        turn = lim;
      if (turn < -0x1000)
        turn = -0x1000;
      src->angle += turn;
    }
    src->flag5a = 0;
  }

  // Emit the particle with a y offset of 8-24 and a random direction
  // at radius 0-48 from the main object.
  pos.x = src->x;
  pos.y = src->y - (Random16() << 4) - 0x80000;
  pos.z = src->z;
  dist = Random16() * 3;
  dist <<= 4;
  Vector_AddPolarOffset__f0dcb44(dist, Random16(), &pos);

  obj = Actor_Spawn__f0dcb44(0x2a1, pos.x, pos.y, pos.z);
  if (obj != 0) {
    obj->mode55 = 2;
    obj->field48 = 0x1999;
    Actor_SelectAnimationFar(obj, 0);
    obj->field5e = 12;
    Actor_StartScriptFar((struct Actor *)obj,
                         gBattleEffect_SharedParticleScript);
  }
}

struct Vec3 {
  s32 x;
  s32 y;
  s32 z;
};

struct PendingEvent;

/* VIEW(f0dd054): routine Scene_FindPsynergyTrigger as struct PendingEvent
 * *Scene_FindPsynergyTrigger(u32 filter, s32 kind, s32 selector); battle.h has
 * struct TriggerEvent *Scene_FindPsynergyTrigger(s32 filter, s32 kind, s32
 * selector). */
extern struct PendingEvent *
Func_080ce458__f0dd054(u32 filter, s32 kind,
                       s32 selector) __asm__("Scene_FindPsynergyTrigger");
/* VIEW(f0dd054): routine BattleEffect_GetCycledTableWord as u32
 * BattleEffect_GetCycledTableWord(u32 keys); the definition has u16
 * BattleEffect_GetCycledTableWord(void); tried as the definition's u16
 * BattleEffect_GetCycledTableWord(void), which the object already declares, it
 * does not compile in tla-ja:
 * games/tla/src/field/event_commands/mind_read.c:463: too many arguments to
 * function `BattleEffect_GetCycledTableWord'. */
extern u32 BattleEffect_GetCycledTableWord__f0dd054(u32 keys) __asm__(
    "BattleEffect_GetCycledTableWord");
s32 MapGrid_CheckCollisionFar(struct Actor *object, struct Vec3 *position);
s32 MapGrid_GetTerrainKindFar(s32 layer, s32 x, s32 z);
void Actor_ApplyRandomChildValues(void);

#define MOVE_CELL_MASK 0xfff00000 ///< a 16-pixel cell

void FieldPsynergy_SteerPushedTarget(void) {
  struct Vec3 position;
  struct Vec3 beyond;
  struct BattleFxScene *scene;
  struct Actor *user;
  struct Actor *target;
  struct Actor *pusher;
  struct Actor *hit;
  s32 x;
  s32 z;
  s32 angle;
  s32 moved;
  s32 direction;
  s32 cellX;
  s32 cellZ;
  s32 (*callback)(void);
  u8 savedLayer;
  u8 *layer;
  struct PendingEvent *event;
  s32 pushed;
  u8 *userFlags;
  const u8 *modes;

  scene = Ram_HeapSlots->effectWork;
  user = scene->mainObject;
  target = scene->child;
  angle = (u16)scene->angle + TRIG_TURN / 2;
  moved = 0;
  /* FAKEMATCH(temporary): a byte pointer standing in for target->terrainId at
   * the save and the restore, whose stack slot then comes after pushed's and
   * event's; as target->terrainId, 18 of 444 instructions differ in tla-en.
   */
  layer = &target->terrainId;
  savedLayer = *layer;
  pushed = 0;
  if (target == NULL)
    return;
  FieldPsynergy_FadeToCastColor();
  user->watchedObject = target;
  Actor_StartScriptFar(user, gFieldEffect_SourceHoldScript);
  pusher = FieldEffect_SpawnMoveHand(user);
  if (pusher == NULL) {
    FieldPsynergy_FadeFromCastColor();
    return;
  }

  event = Func_080ce458__f0dd054(0x50000005, scene->fieldKind,
                                 Func_080ce31c(scene->fieldKind));
  pusher->watchedObject = target;
  position.x = target->x;
  position.y = target->y + (16 << 16);
  position.z = target->z;
  Vector_AddPolarOffset__f0dcb44(16 << 16, angle, &position);
  Actor_SetMoveTargetFar(pusher, position.x, position.y, position.z);
  FieldEffect_SnapScaleToFull(pusher);
  pusher->speedLimit = 4 << 16;
  pusher->acceleration = Q16_ONE / 2;
  pusher->motionFlags = 4;
  target->update = Actor_ApplyRandomChildValues;
  target->speedLimit = 0x6666;
  target->acceleration = 0x3333;
  target->actionFlags = moved;
  // Through target rather than layer, which keeps moved's load after the
  // acceleration's value is built; through layer, 2 of 444 instructions differ
  // in tla-en.
  target->terrainId = 2;
  while (1) {
    wait(1);
    // The mode read as a signed byte, which the test against 0 does not change.
    if ((s8)scene->mode == 0 && (gInput.pressed & 0x303))
      break;

    direction = (u16)BattleEffect_GetCycledTableWord__f0dd054(gInput.held);
    // The bytes read the hook's return as the direction; the field is
    // declared void (*)(void) in field_psynergy_work.h, which needs s32.
    callback = (s32 (*)(void))scene->finishCallback;
    if (callback != NULL) {
      direction = callback();
      if (direction == 0xffff)
        break;
    } else if (direction == 0xffff) {
      position.x = target->x;
      position.y = target->y + (16 << 16);
      position.z = target->z;
      Vector_AddPolarOffset__f0dcb44(16 << 16, angle, &position);
      Actor_SetMoveTargetFar(pusher, position.x, position.y, position.z);
      Actor_SelectAnimationFar(pusher, 1);
      pusher->velocityX = 0;
      pusher->velocityY = 0;
      pusher->velocityZ = 0;
      continue;
    }

    position.x = target->x;
    position.y = target->y + (16 << 16);
    position.z = target->z;
    Vector_AddPolarOffset__f0dcb44(16 << 16, angle, &position);
    Vector_AddPolarOffset__f0dcb44(2 << 16, direction, &position);
    Actor_SetMoveTargetFar(pusher, position.x, position.y, position.z);
    Actor_WaitForMoveFar(pusher);
    position.x = target->x;
    position.y = target->y;
    position.z = target->z;
    Vector_AddPolarOffset__f0dcb44(16 << 16, direction, &position);
    beyond.x = target->x;
    beyond.y = target->y;
    beyond.z = target->z;
    Vector_AddPolarOffset__f0dcb44(32 << 16, direction, &beyond);
    if (MapGrid_CheckCollisionFar(target, &position) > 0) {
      Actor_SelectAnimationFar(pusher, 4);
      if ((gFrameCount & 15) == 0)
        Sound_PlayFar(114);
      continue;
    }
    hit = ActorScript_FindOverlappingEntryFar(target, &position);
    if (hit != NULL) {
      if (hit != user) {
        Actor_SelectAnimationFar(pusher, 4);
        if ((gFrameCount & 15) == 0)
          Sound_PlayFar(114);
        continue;
      }
      cellX = user->x & MOVE_CELL_MASK;
      cellZ = user->z & MOVE_CELL_MASK;
      if (cellX == (position.x & MOVE_CELL_MASK) &&
          cellZ == (position.z & MOVE_CELL_MASK)) {
        Actor_SelectAnimationFar(pusher, 4);
        if ((gFrameCount & 15) == 0)
          Sound_PlayFar(114);
        continue;
      }
      if (cellX == (beyond.x & MOVE_CELL_MASK) &&
          cellZ == (beyond.z & MOVE_CELL_MASK)) {
        if (MapGrid_GetTerrainKindFar(user->terrainId, beyond.x, beyond.z) !=
            0) {
          Actor_SelectAnimationFar(pusher, 4);
          if ((gFrameCount & 15) == 0)
            Sound_PlayFar(114);
          continue;
        }
        moved = 1;
      }
    }

    Sound_PlayFar(175);
    pushed = 1;
    x = position.x;
    z = position.z;
    modes = gUnused_BattleEffectBytes;
    Actor_SelectAnimationFar(pusher, modes[(u16)(angle - direction) >> 14]);
    wait(15);
    target->movementState = 0;
    target->speedLimit = 0x3333;
    target->acceleration = 0x3333;
    Actor_SetMoveTargetFar(target, position.x, position.y, position.z);
    pusher->motionFlags = 0;
    pusher->speedLimit = 0x3333;
    pusher->acceleration = 0x3333;
    Vector_AddPolarOffset__f0dcb44(16 << 16, direction, &position);
    Actor_SetMoveTargetFar(pusher, position.x, position.y + (16 << 16),
                           position.z);
    if (moved == 1) {
      Actor_GetById(scene->firstObjectId)->actionFlags &= 0xfe;
      user->speedLimit = 0x3333;
      user->acceleration = 0x3333;
      Actor_SetMoveTargetFar(user, beyond.x, beyond.y, beyond.z);
    }
    Actor_WaitForMoveFar(target);
    if (event == NULL) {
      target->x = x;
      target->z = z;
      target->velocityX = 0;
      target->velocityZ = 0;
    }
    break;
  }

  *layer = savedLayer;
  // The palette byte sits at 0x40 here, not the struct's savedPalette at 0x44.
  Actor_SetSpriteColorModeFar(target, scene->unk40[0]);
  Actor_StartScriptFar(target, scene->savedScript);
  target->update = (void (*)(void))scene->savedCallback;
  if (pushed) {
    pusher->update = NULL;
    Actor_SelectAnimationFar(pusher, 4);
    Actor_SetPositionAndResetMotionFar(pusher, pusher->x, pusher->y, pusher->z);
    Field_RunPsynergyEvent(event, scene->firstObjectId, scene->secondObjectId);
  }
  BattleEffect_StopMainActor();
  if (moved == 1) {
    /* FAKEMATCH(temporary): a byte pointer standing in for the actor's
     * actionFlags, which loads them into r3 and the 1 into r2; as
     * Actor_GetById(...)->actionFlags |= 1, 4 of 444 instructions differ in
     * tla-en. */
    userFlags = &Actor_GetById(scene->firstObjectId)->actionFlags;
    *userFlags |= 1;
  }
  FieldPsynergy_FadeFromCastColor();
  UpdateRisingParticleBurst(pusher);
}

void FieldEffect_UpdateOutAndBack(struct FieldParticle *effect) {
  struct Output_08097f80 position;
  s8 *statePointer = &effect->state;
  s32 state;

next_state:
  state = *statePointer;
  if (state == 0) {
    u32 angle;

    position.x = effect->originX;
    position.z = effect->originZ;
    angle = Random16();
    Vector_AddPolarOffset__f0dcb44(0x1e0000, (u16)angle, &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = 0x40000;
    effect->maxSpeed = 0x40000;
    effect->limitTurnStep = state;
    (*statePointer)++;
    return;
  }

  if (state == 1) {
    if (FieldParticle_HasPendingTarget(effect) == 0) {
      (*statePointer)++;
      goto next_state;
    }
    return;
  }

  if (state == 2) {
    effect->targetX = effect->originX;
    effect->targetZ = effect->originZ;
    {
      u32 value = 0x400;

      effect->maxTurnStep = value;
    }
    effect->limitTurnStep = 1;
    (*statePointer)++;
    return;
  }

  if (state == 3 && FieldParticle_HasPendingTarget(effect) == 0)
    FieldEffect_ClearOwnedSlot(effect);
}
#undef MOVE_CELL_MASK

void FieldPsynergy_Move(void) {
  // Unused: gives the stack frame its 12 bytes.
  s32 work[3];
  struct BattleFxScene *scene;
  void *object;

  scene = Ram_HeapSlots->effectWork;
  object = scene->mainObject;

  FieldPsynergy_FadeToCastColor();
  object = FieldEffect_SpawnMoveHand(object);
  FieldEffect_SnapScaleToFull(object);
  if (object != 0) {
    Actor_SelectAnimationFar(object, 4);
    wait(30);
  }
  FieldPsynergy_FadeFromCastColor();
  UpdateRisingParticleBurst(object);
}

extern const s32 gFieldEffect_FragmentScript[];

/* VIEW(item_break): routine Actor_Spawn as struct Actor *Actor_Spawn(s32 kind,
 * s32 x, s32 y, s32 z); hue has extern struct ItemBreakFragmentObject
 * *Actor_Spawn(s32, s32, s32, s32). */
extern struct Actor *Actor_Spawn__item_break(s32 kind, s32 x, s32 y,
                                             s32 z) __asm__("Actor_Spawn");
/* VIEW(item_break): routine FieldEffect_UpdateMoveHand as void
 * FieldEffect_UpdateMoveHand(void); the definition has void
 * FieldEffect_UpdateMoveHand(struct ItemBreakFragmentSource *src). */
extern void BattleEffect_UpdateItemBreakFragment__item_break(void) __asm__(
    "FieldEffect_UpdateMoveHand");

#define SE_ITEM_BREAK 138

/// The Broken Seal's FieldEffect_SpawnMoveHand with this game's objects
/// 278 and 673: spawns the breaking item above object and eight fragments
/// that scatter from it.
void *FieldEffect_SpawnMoveHand(void *object) {
  struct Actor *source = object;
  struct Actor *parent;
  struct Actor *child;
  s32 angle;
  s32 radius;
  s32 speed;
  s32 fragmentIndex;
  u32 spread;
  u32 jitter;

  angle = (source->facing + 0x2000) & 0xc000;
  parent = Actor_Spawn__item_break(278, source->x, source->y + (16 << 16),
                                   source->z);
  if (parent == 0)
    return 0;
  parent->scaleY = 0x4000;
  parent->scaleX = 0x4000;
  parent->update = BattleEffect_UpdateItemBreakFragment__item_break;
  parent->speedLimit = 2 << 16;
  parent->acceleration = 2 << 16;
  parent->motionFlags = 0;
  Actor_SelectAnimationFar(parent, 3);
  Actor_SetTargetPositionFromMagnitudeAngle(parent, 16 << 16, angle);

  for (fragmentIndex = 7; fragmentIndex >= 0; fragmentIndex--) {
    child = Actor_Spawn__item_break(673, source->x, source->y + (16 << 16),
                                    source->z);
    if (child != 0) {
      Actor_StartScriptFar(child, gFieldEffect_FragmentScript);
      speed = Random16() + Q16_ONE;
      child->acceleration = 1 << 16;
      child->speedLimit = speed;
      child->motionFlags = 2;
      child->gravity = 0x51e;
      spread = Random16();
      child->velocityY = spread - Random16();
      radius = Random16() * 24 + (8 << 16);
      jitter = Random16();
      Actor_SetTargetPositionFromMagnitudeAngle(
          child, radius, ((jitter - Random16()) >> 3) + source->facing);
    }
  }
  Sound_PlayFar(SE_ITEM_BREAK);
  return parent;
}

/// Raises the object's scale in sixteenths to full size and commits its
/// position.
void FieldEffect_SnapScaleToFull(struct Actor *object) {
  s32 next;
  s32 scale;

  if (object != NULL) {
    scale = object->scaleX;
    if (scale <= 0xffff) {
      do {
        next = scale + Q16_ONE / 16;
        scale = next;
      } while (next <= 0xffff);
      object->scaleX = next;
      object->scaleY = next;
    }
    Actor_WaitForMoveFar(object);
  }
}

/// The object spins as it rises for 31 frames, shrinks, and bursts into 8
/// particles (object 673).
// As ☀️'s UpdateRisingParticleBurst.
void UpdateRisingParticleBurst(void *object) {
  struct Actor *src = object;
  struct Actor *child;
  s32 i;
  s32 dist;
  u32 spread;

  Sound_PlayFar(154);
  for (i = 30; i >= 0; i--) {
    src->y += 1 << 16;
    src->facing += 0x2000; // 1/8 turn
    src->scaleX += -Q16_ONE / 32;
    src->scaleY += -Q16_ONE / 32;
    wait(1);
  }
  for (i = 7; i >= 0; i--) {
    child = Actor_Spawn__item_break(673, src->x, src->y, src->z);
    if (child != 0) {
      s32 speed;
      // One 1.0 for both uses, set before the block's calls; a literal at
      // each use differs.
      s32 unit = 0x10000;

      Actor_StartScriptFar(child, gFieldEffect_FragmentScript);
      speed = Random16();
      child->acceleration = unit;
      child->speedLimit = speed + unit;
      child->motionFlags = 2;
      child->gravity = 0xa3d;
      spread = Random16();
      child->velocityY = spread - Random16();
      dist = Random16() * 24;
      dist += 8 << 16;
      Actor_SetTargetPositionFromMagnitudeAngle(child, dist, Random16());
    }
  }
  Sound_PlayFar(131);
  Actor_ReleaseFar(src);
}

/// Burns 63 loop iterations and returns; callers pass an argument it ignores.
void BattleEffect_RunBusyWaitDelay(void) {
  s32 i = 63;

loop:
  i--;
  if (i >= 0)
    goto loop;
}

/// As The Broken Seal's, with this game's offsets: counts the timer down and,
/// once the leader is 60 pixels from the centre or the timer has run out,
/// requests the effect's end.
void FieldPsynergy_WatchLeaderDistance(void) {
  u8 *work;
  struct Actor *actor;
  s16 *timer;
  s32 dx;
  s32 dz;
  struct MainRoutineWork *party;

  work = Ram_HeapSlots->eventWork;
  party = &gMainRoutineWork;
  actor = ObjectTable_Get(party->currentOwner);
  if (*(s8 *)(work + 0xca5) != 0) {
    timer = (s16 *)(work + 0xca6);
    if (*timer != 0)
      (*timer)--;
  }

  dx = Iwram_MulQ16(*(s16 *)(work + 0xca8) - actor->x / Q16_ONE, 0xd105);
  dz = *(s16 *)(work + 0xcaa) - (actor->z - actor->y) / Q16_ONE;
  if (dx * dx + dz * dz >= 3600 || *(s16 *)(work + 0xca6) == 0) {
    u16 *slot = (u16 *)(work + 358);
    u32 request = 0x2090;

    *slot = request;
  }
}
#undef SE_ITEM_BREAK
#include "event.h"
#include "module/field/event_commands.h"
#include "module/field/object.h"

/* VIEW(f0dd820): routine BattleEffect_RunBusyWaitDelay as void
 * BattleEffect_RunBusyWaitDelay(s32 value); the definition has void
 * BattleEffect_RunBusyWaitDelay(void); tried as the definition's void
 * BattleEffect_RunBusyWaitDelay(void), which the object already declares, it
 * does not compile in tla-ja:
 * games/tla/src/field/event_commands/mind_read.c:877: too many arguments to
 * function `BattleEffect_RunBusyWaitDelay'. */
extern void BattleEffect_RunBusyWaitDelay__f0dd820(s32 value) __asm__(
    "BattleEffect_RunBusyWaitDelay");

#define REVEAL_EVENT_START 0x50000005 // the Reveal event, stage 5
#define REVEAL_FRAMES 600
#define SE_REVEAL 131

// Reveal: open a circle of the transition on the caster, wash the screen
// plain, run the Reveal event that matches, then show the markers nearby
// and watch how far the leader walks.
void FieldPsynergy_Reveal(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct RevealEvent *event;
  struct ScreenTransitionState *transition;
  struct Actor *caster;
  void *pending;
  s32 frame;

  BattleEffect_RunBusyWaitDelay__f0dd820(6);
  event = Ram_HeapSlots->eventWork;
  ScreenTransition_InitializeBattleEffectState(FIELD_PSYNERGY_REVEAL);
  transition = Ram_HeapSlots->screenTransition;
  caster = scene->mainObject;
  transition->centreX = caster->x;
  transition->centreY = caster->z - caster->y;
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer__f0dcb44(0x10001, 1);
  PaletteFade_StartInterpolation(1);
  event->active = 0;
  wait(1);

  pending = Scene_FindPsynergyTrigger(REVEAL_EVENT_START, FIELD_PSYNERGY_REVEAL,
                                      Func_080ce31c(FIELD_PSYNERGY_REVEAL));
  if (pending != 0)
    Field_RunPsynergyEvent(pending, scene->firstObjectId,
                           scene->secondObjectId);
  // Leave the centre (in pixels) and the time in the event work, and put
  // up the markers nearby.
  // The cue and the active flag in a once-only block keep active's store
  // before the centre's reload; as plain statements, 4 of 137 instructions
  // differ in tla-en.
  do {
    Sound_PlayFar(SE_REVEAL);
    event->active = 1;
  } while (0);
  event->centreX = transition->centreX / Q16_ONE;
  event->centreY = transition->centreY / Q16_ONE;
  event->frames = REVEAL_FRAMES;
  event->markers = 1;
  FieldEffect_SpawnRevealMarkers();
  frame = 0;
  do {
    wait(1);
    transition->value = frame;
    frame++;
  } while (frame <= 18);
  if ((s8)scene->mode == 0)
    Task_Add(FieldPsynergy_WatchLeaderDistance, 0x480);
}

void FieldPsynergy_EndReveal(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct ScreenTransitionState *transition = Ram_HeapSlots->screenTransition;
  struct RevealEvent *event = Ram_HeapSlots->eventWork;
  void *pending;

  if (event->active != 0) {
    Sound_PlayFar(167);
    Task_Remove(FieldPsynergy_WatchLeaderDistance);
    event->active = 0;
    event->frames = 0;
    BattleEffect_RunBusyWaitDelay__f0dd820(0);
    BattleEffect_ApplyColorToTargetBuffer__f0dcb44(0x10000, 1);
    PaletteFade_StartInterpolation(1);
    BattleEffect_ApplyColorToSourceBuffer(0, 0);
    BattleEffect_ApplyColorToTargetBuffer__f0dcb44(0x10000, 0);
    PaletteFade_StartInterpolation(30);
    wait(1);
    pending = Scene_FindPsynergyTrigger(0x40000005, FIELD_PSYNERGY_REVEAL,
                                        Func_080ce31c(FIELD_PSYNERGY_REVEAL));
    if (pending != NULL)
      Field_RunPsynergyEvent(pending, scene->firstObjectId,
                             scene->secondObjectId);
    if (scene->childOption == 0) {
      transition->nextMode = 0;
      transition->duration = 1;
      transition->step = 1;
      wait(10);
    }
  }
}

extern const s32 gFieldEffect_HeavyImpactScript[];

/* VIEW(f0dd820): routine Vector_AddPolarOffset as void
 * Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position); hue has extern
 * void Vector_AddPolarOffset(s32, s32, struct ItemBreakFragmentPosition *). */
extern void
Vector_AddPolarOffset__f0dd820(s32 radius, s32 angle,
                               s32 *position) __asm__("Vector_AddPolarOffset");
/* VIEW(f0dd820): routine Actor_Spawn as struct Actor *Actor_Spawn(s32 kind, s32
 * x, s32 y, s32 z); hue has extern struct ItemBreakFragmentObject
 * *Actor_Spawn(s32, s32, s32, s32). */
extern struct Actor *Actor_Spawn__f0dd820(s32 kind, s32 x, s32 y,
                                          s32 z) __asm__("Actor_Spawn");

#define BURST_FALL_OBJECT 281 ///< the object that falls round the target

// The two sprites of target that do not sink into the ground.
#define TARGET_KIND_SCATTERS 372 ///< scatters five falling pieces instead
#define TARGET_KIND_STAYS 394

/// Animation 12 on a target: the burst of particles, then the target sinks
/// to the underfoot priority. A target of kind 372 instead throws five pieces
/// round the scene's position; one of kind 394 only plays the cue (sound 126).
void FieldPsynergy_GrowthOnTarget(s32 unused) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *target = scene->child;
  s32 kind = 0;

  if (target == 0)
    return;

  FieldEffect_SpawnBurstParticleField();
  if (Actor_GetSpriteIdFar(target) == TARGET_KIND_SCATTERS)
    kind = 1;
  if (Actor_GetSpriteIdFar(target) == TARGET_KIND_STAYS)
    kind = 2;

  if (kind == 0) {
    Actor_SelectAnimationFar(target, 2);
    Actor_SetSpriteFlagsFar(target, 0);
    target->collisionFlags = 0;
    target->priorityFlags |= 2;
    wait(10);
    Sound_PlayFar(126);
    wait(40);
  } else if (kind == 1) {
    struct BurstParticleVector position;
    struct BurstParticleVector *p = &position;
    s32 n = 4;

    do {
      struct Actor *piece;
      s32 distance;

      p->values[0] = scene->x;
      p->values[2] = scene->z;
      distance = Random16() * 6 + 0x40000;
      Vector_AddPolarOffset__f0dd820(distance, Random16(), p->values);
      p->values[1] = scene->y;
      piece = Actor_Spawn__f0dd820(BURST_FALL_OBJECT, p->values[0],
                                   p->values[1], p->values[2]);
      if (piece != 0) {
        Actor_StartScriptFar(piece, gFieldEffect_HeavyImpactScript);
        piece->motionFlags = 2;
      }
      wait((((u32)Random16() * 2) >> 16) + 2);
      n--;
    } while (n >= 0);
    HangingVine_Hang(scene->secondObjectId, 1);
  } else {
    wait(10);
    Sound_PlayFar(126);
  }
  FieldPsynergy_FadeFromCastColor();
}

void FieldPsynergy_Growth(void) {
  u8 *state = Ram_HeapSlots->effectWork;
  struct BurstParticleVector position;
  struct BurstParticleVector *p;
  s32 entryCount;

  FieldEffect_SpawnBurstParticleField();
  Sound_PlayFar(134);
  p = &position;
  entryCount = 4;
  do {
    void *object;
    s32 randomValue;

    p->values[0] = *(s32 *)(state + 4);
    p->values[2] = *(s32 *)(state + 12);
    randomValue = (Random16() * 6) + 0x40000;
    Vector_AddPolarOffset__f0dd820(randomValue, Random16(), p->values);
    p->values[1] = *(s32 *)(state + 8);
    object =
        Actor_Spawn__f0dd820(0x119, p->values[0], p->values[1], p->values[2]);
    if (object != 0) {
      Actor_StartScriptFar(object, gFieldEffect_HeavyImpactScript);
      *((u8 *)object + 0x55) = 2;
    }
    wait((((u32)Random16() * 2) >> 16) + 2);
    entryCount--;
  } while (entryCount >= 0);
  wait(30);
  FieldPsynergy_FadeFromCastColor();
}

void FieldEffect_SpawnBurstParticleField(void) {
  struct BattleFxScene *work;
  struct Actor *user;
  s32 pos[3];
  s32 n;
  s32 *p;

  work = Ram_HeapSlots->effectWork;
  user = work->mainObject;
  FieldPsynergy_FadeToCastColor();

  p = pos;
  for (n = 23; n >= 0; n--) {
    struct Actor *obj;
    struct BurstParticleVisual *visual;
    struct BurstParticleVisual *child;
    s32 dist;

    if (*(u16 *)&work->angle == 0x4000) {
      pos[0] = user->x;
      pos[1] = user->y + 0xa0000;
      pos[2] = user->z;
    } else if (*(u16 *)&work->angle == 0xc000) {
      pos[0] = user->x;
      pos[1] = user->y + 0x180000;
      pos[2] = user->z;
    } else {
      pos[0] = user->x;
      pos[1] = user->y + 0xa0000;
      pos[2] = user->z;
      Vector_AddPolarOffset__f0dd820(0xa0000, *(u16 *)&work->angle, pos);
    }

    obj = Actor_Spawn__f0dd820(672, pos[0], pos[1], pos[2]);
    visual = &((struct BurstParticleVisualGroup *)obj->sprite)->primary;
    child = &((struct BurstParticleVisualGroup *)obj->sprite)->child;
    child->flip = visual->flip;
    child->flagsBHigh = visual->flagsBHigh;
    child->flagsAHigh = visual->flagsAHigh;
    child->attributes.bits.shape = visual->attributes.bits.shape;
    child->attributes.bytes.palette = visual->attributes.bytes.palette;

    if (obj != 0) {
      ((s32 *)obj)[7] = 0xb333;
      ((s32 *)obj)[6] = 0xb333;
      obj->acceleration = 0x18000;
      obj->speedLimit = 0x18000;
      obj->motionFlags = 0;
      Actor_SetSpriteColorModeFar(obj, 11);
      Actor_SelectAnimationFar(obj, 7);
      Actor_StartScriptFar(obj, gBattleEffect_SharedParticleScript + 1);

      Actor_SetSpriteFlagsFar(obj, 1);

      p[0] = work->x;
      p[1] = work->y;
      p[2] = work->z;
      if (*(u16 *)&work->angle == 0xc000)
        Vector_AddPolarOffset__f0dd820(0xe0000, *(u16 *)&work->angle, p);
      dist = Random16() * 6 + 0x40000;
      Vector_AddPolarOffset__f0dd820(dist, Random16(), p);
      Actor_SetMoveTargetFar(obj, p[0], p[1], p[2]);
    }
    Sound_PlayFar(131);
    wait(2);
  }
  wait(8);
}
#include "module/field/event_commands.h"
#include "module/field/object.h"

void Actor_ApplyRandomChildValues(void);

void FieldPsynergy_LiftOnTarget(void) {
  struct BattleEffectScene *scene;
  void *mainObject;
  void *child;
  void *anchors[2];
  s32 position[3];
  s32 x, y, z;
  s32 index;
  s32 startY;

  scene = Ram_HeapSlots->effectWork;
  child = scene->child;
  mainObject = scene->mainObject;
  if (child == 0)
    return;

  FieldPsynergy_FadeToCastColor();
  *(void **)((u8 *)mainObject + 0x68) = child;
  Actor_StartScriptFar(mainObject, gFieldEffect_SourceHoldScript);

  x = scene->x;
  position[0] = x;
  y = scene->y + 0x100000;
  position[1] = y;
  z = scene->z;
  position[2] = z;
  anchors[0] = FieldEffect_SpawnLiftHand(x + 0x200000, y, z, 0x8000);
  anchors[1] = FieldEffect_SpawnLiftHand(position[0] - 0x200000, position[1],
                                         position[2], 0);

  wait(15);
  for (index = 0; index < 2; index++) {
    void *anchor = anchors[index];
    if (anchor != 0)
      Actor_SetTargetPositionFromMagnitudeAngle(anchor, 0xe0000,
                                                *(u16 *)((u8 *)anchor + 6));
  }

  Actor_WaitForMoveFar(anchors[0]);
  ((struct EffectChild *)child)->callback = Actor_ApplyRandomChildValues;
  Sound_PlayFar(130);
  ((struct EffectChild *)child)->flag = 4;

  startY = ((struct EffectChild *)child)->y;
  if (anchors[0] != 0 && anchors[1] != 0 && startY <= startY + 0x200000) {
    do {
      *(s32 *)((u8 *)anchors[0] + 0xc) += 0x4000;
      *(s32 *)((u8 *)anchors[1] + 0xc) += 0x4000;
      ((struct EffectChild *)child)->y += 0x4000;
      wait(1);
    } while (((struct EffectChild *)child)->y <= startY + 0x200000);
  }

  UpdateRisingParticleBurst(anchors[0]);
  UpdateRisingParticleBurst(anchors[1]);
  FieldPsynergy_FadeFromCastColor();
}

extern const s32 gFieldEffect_FragmentScript[];

void FieldPsynergy_Lift(void) {
  struct BattleFxScene *work;
  struct Actor *obj;
  struct Actor *pair[2];
  struct ScatterPosition pos;
  struct Actor **walk;
  s32 n;
  s32 dist;

  work = Ram_HeapSlots->effectWork;
  FieldPsynergy_FadeToCastColor();
  pos.x = work->x;
  pos.y = work->y + 0x100000;
  pos.z = work->z;
  pair[0] = FieldEffect_SpawnLiftHand(pos.x + 0x200000, pos.y, pos.z, 0x8000);
  pair[1] = FieldEffect_SpawnLiftHand(pos.x - 0x200000, pos.y, pos.z, 0);

  wait(15);
  walk = pair;
  for (n = 0; n < 2; n++) {
    obj = *walk++;
    if (obj != 0)
      Actor_SetTargetPositionFromMagnitudeAngle(obj, 0x180000, obj->facing);
  }

  Actor_WaitForMoveFar(pair[0]);
  Sound_PlayFar(134);
  for (n = 0; n < 24; n++) {
    pos.x = work->x;
    pos.y = work->y + 0x100000;
    pos.z = work->z;
    obj = Actor_Spawn__f0dd820(673, pos.x, pos.y, pos.z);
    if (obj != 0) {
      Actor_StartScriptFar(obj, gFieldEffect_FragmentScript);
      obj->speedLimit = Random16() + 0x20000;
      obj->acceleration = 0x20000;
      obj->motionFlags = 0;
      dist = Random16() * 24 + 0x80000;
      Actor_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());
    }
  }

  Actor_ReleaseFar(pair[0]);
  Actor_ReleaseFar(pair[1]);
  FieldPsynergy_FadeFromCastColor();
}

/// Spawns object 278 and grows it from a quarter to full size, an eighth of a
/// turn a frame, then faces it to angle.
struct Actor *FieldEffect_SpawnLiftHand(s32 x, s32 y, s32 z, s32 angle) {
  struct Actor *object;
  s32 scale;

  Sound_PlayFar(138);
  object = Actor_Spawn__f0dd820(278, x, y, z);
  if (object != NULL) {
    object->scaleX = object->scaleY = 0x4000;
    object->acceleration = object->speedLimit = 0x30000;
    ((struct FieldSprite *)object->sprite)->priority = 0;
    Actor_SelectAnimationFar(object, 3);
    scale = object->scaleX;
    if (scale < Q16_ONE) {
      do {
        scale += 0x800;
        object->scaleY = scale;
        object->scaleX = scale;
        object->facing += 0x2000;
        wait(1);
        scale = object->scaleX;
      } while (scale <= 0xffff);
    }
    object->facing = angle;
  }
  return object;
}

struct EffectPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct EffectWork {
  u16 angle;
  u8 unk02[14];
  struct EffectTarget *target;
};

/* VIEW(f0dcb44): routine Camera_WorldToScreen as void
 * Camera_WorldToScreen(struct EffectPosition *value);
 * module/field/event_commands.h has void Camera_WorldToScreen(struct
 * EffectVector *position). */
extern void Camera_WorldToScreen__f0dcb44(struct EffectPosition *value) __asm__(
    "Camera_WorldToScreen");
/* VIEW(f0dd820): routine FieldEffect_ClearOwnedSlot as void
 * FieldEffect_ClearOwnedSlot(struct RadialBurstEffect *effect); f0dd054 has
 * void FieldEffect_ClearOwnedSlot(struct FieldParticle *). */
extern void BattleEffect_ClearOwnedSlot__f0dd820(
    struct RadialBurstEffect *effect) __asm__("FieldEffect_ClearOwnedSlot");

void FieldEffect_UpdateRadialBurst(struct RadialBurstEffect *effect) {
  struct EffectWork *work = Ram_HeapSlots->effectWork;
  struct EffectPosition value;
  s32 state;

again:
  state = effect->state;
  if (state == 0) {
    value.x = effect->sourceX;
    value.z = effect->sourceZ;
    Vector_AddPolarOffset__f0dd820(0x190000, (u16)Random16(), &value);
    effect->x = value.x;
    effect->z = value.z;
    effect->velocityZ = 0x30000;
    effect->velocityX = 0x30000;
    effect->enabled = 0;
    effect->state++;
    return;
  }

  if (state == 1) {
    if (FieldParticle_HasPendingTarget(effect) == 0) {
      effect->state++;
      goto again;
    }
    return;
  }

  if (state == 2) {
    struct EffectTarget *target = work->target;

    value.x = target->x;
    value.y = target->y + 0x100000;
    value.z = target->z;
    Vector_AddPolarOffset__f0dd820(0x80000, work->angle, &value);
    Camera_WorldToScreen__f0dcb44(&value);
    Vector_AddPolarOffset__f0dd820(0x40000, Random16(), &value);
    effect->x = value.x;
    effect->z = value.z;
    effect->scale = 0x800;
    effect->enabled = 1;
    effect->state++;
    return;
  }

  if (state == 3 && FieldParticle_HasPendingTarget(effect) == 0)
    BattleEffect_ClearOwnedSlot__f0dd820(effect);
}

/// A heavy impact, as The Broken Seal's: an actor placed below the source
/// for a moment, scattering shards from it.
s32 SpawnHeavyImpactEffect(struct Actor *src) {
  struct ImpactVector pos;
  struct Actor *obj;
  struct Actor *frag;
  s32 i;

  Sound_PlayFar(134);
  pos.x = src->x;
  pos.y = src->y;
  pos.z = src->z;
  obj = Actor_Spawn__f0dd820(671, pos.x, pos.y - (32 << 16), pos.z);
  if (obj != NULL) {
    obj->motionFlags = 0;
    ((struct ParticleTimer *)obj)->timer = 20;
    Actor_StartScriptFar(obj, gBattleEffect_SharedParticleScript);
  }

  for (i = 0; i < 12; i++) {
    frag = Actor_Spawn__f0dd820(673, pos.x, pos.y, pos.z);
    if (frag != NULL) {
      Actor_StartScriptFar(frag, gFieldEffect_FragmentScript);
      frag->speedLimit = Random16() + (1 << 16);
      frag->acceleration = 1 << 16;
      frag->motionFlags = 0;
      Actor_SetTargetPositionFromMagnitudeAngle(
          frag, Random16() * 24 + (8 << 16), Random16());
    }
  }
  return 0;
}

/* VIEW(f0dd820): routine FieldPsynergy_Force as void FieldPsynergy_Force(struct
 * BattleEffectArgument *effect); the definition has void
 * FieldPsynergy_Force(void). */
extern void Func_080de21c__f0dd820(struct BattleEffectArgument *effect) __asm__(
    "FieldPsynergy_Force");

/// Runs field Psynergy animation 4; the target is unused.
void FieldPsynergy_ForceOnTarget(struct BattleEffectArgument *effect) {
  Func_080de21c__f0dd820(effect);
}
#include "item_icon.h"
#include "module/field/event_commands.h"

/* VIEW(f0de21c): routine Camera_WorldToScreen as void
 * Camera_WorldToScreen(struct Effect04Vector *position); f0dd820 has void
 * Camera_WorldToScreen(struct EffectPosition *value). */
extern void Camera_WorldToScreen__f0de21c(
    struct Effect04Vector *position) __asm__("Camera_WorldToScreen");
/* VIEW(f0de21c): routine Vector_AddPolarOffset as void
 * Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position); hue has extern
 * void Vector_AddPolarOffset(s32, s32, struct ItemBreakFragmentPosition *). */
extern void
Vector_AddPolarOffset__f0de21c(s32 radius, s32 angle,
                               s32 *position) __asm__("Vector_AddPolarOffset");
/* VIEW(f0de21c): routine Actor_Spawn as struct Actor *Actor_Spawn(s32 kind, s32
 * x, s32 y, s32 z); hue has extern struct ItemBreakFragmentObject
 * *Actor_Spawn(s32, s32, s32, s32). */
extern struct Actor *Actor_Spawn__f0de21c(s32 kind, s32 x, s32 y,
                                          s32 z) __asm__("Actor_Spawn");
/* VIEW(f0dcb44): routine Sprite_ShareVramCacheEntry as struct ResourceEntry
 * *Sprite_ShareVramCacheEntry(void *sprite, struct ResourceEntry *last);
 * module/field/event_commands.h has void *Sprite_ShareVramCacheEntry(void
 * *sprite, void *last). */
extern struct ResourceEntry *Sprite_ShareVramCacheEntry__f0dcb44(
    void *sprite,
    struct ResourceEntry *last) __asm__("Sprite_ShareVramCacheEntry");
/* VIEW(f0de21c): routine MapGrid_CheckCollisionFar as s32
 * MapGrid_CheckCollisionFar(struct Actor *object, struct Effect04Vector
 * *position); f0dd054 has s32 MapGrid_CheckCollisionFar(struct Actor *object,
 * struct Vec3 *position). */
extern s32 Func_08020210__f0de21c(
    struct Actor *object,
    struct Effect04Vector *position) __asm__("MapGrid_CheckCollisionFar");
/* VIEW(f0de21c): routine FieldEffect_UpdateRadialBurst as void
 * FieldEffect_UpdateRadialBurst(struct FieldParticle *slot); the definition has
 * void FieldEffect_UpdateRadialBurst(struct RadialBurstEffect *effect). */
extern void Func_080de060__f0de21c(struct FieldParticle *slot) __asm__(
    "FieldEffect_UpdateRadialBurst");

// Twelve sparks burst over the user, two frames apart; a ball grows in
// front of it and flies off with three trailing copies, and the target is
// pushed 16 pixels away unless something blocks it.
void FieldPsynergy_Force(void) {
  struct Actor *copies[4];
  struct Effect04Vector pos;
  struct Effect04Scene *scene;
  struct Actor *target;
  struct Actor *source;
  struct Actor *object;
  struct Actor *copy;
  struct FieldParticle *slot;
  struct ResourceEntry *resource;
  struct PendingEvent *event;
  s32 scale;
  s32 i;
  u8 marker;

  scene = Ram_HeapSlots->effectWork;
  target = scene->child;
  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(130);
  slot = scene->slots;

  for (i = 0; i <= 11; i++) {
    source = scene->mainObject;
    pos.x = source->x;
    pos.y = source->y + 0x100000;
    pos.z = source->z;
    Camera_WorldToScreen__f0de21c(&pos);
    FieldParticle_Initialize(slot, 0x2a0, pos.x, pos.z);
    FieldParticle_SetCallback(slot, Func_080de060__f0de21c);
    FieldParticle_SetActorMode(slot, 7);
    Animation_ApplyChildValuesToRecordFar(slot->object, 9);
    slot->scaleY = 0xb333;
    slot->scaleX = 0xb333;
    wait(2);
    slot++;
  }

  source = scene->mainObject;
  pos.x = source->x;
  pos.y = source->y + 0x100000;
  pos.z = source->z;
  Vector_AddPolarOffset__f0de21c(0x80000, scene->angle, &pos);
  object = Actor_Spawn__f0de21c(278, pos.x, pos.y, pos.z);
  if (object == NULL) {
    FieldPsynergy_FadeFromCastColor();
    return;
  }
  object->scaleY = 0x4000;
  object->scaleX = 0x4000;
  object->facing = scene->angle;
  object->speedLimit = 0x40000;
  object->acceleration = 0x40000;
  object->motionFlags = 0;
  Actor_SelectAnimationFar(object, 5);
  Actor_SetSpriteColorModeFar(object, 3);
  scale = object->scaleX;
  if (scale < 0x10000) {
    do {
      scale += 0x500;
      object->scaleY = scale;
      object->scaleX = scale;
      wait(1);
      scale = object->scaleX;
    } while (scale <= 0xffff);
  }
  wait(3);

  resource = NULL;
  for (i = 2; i >= 0; i--) {
    copy = copies[i] =
        Actor_Spawn__f0de21c(278, object->x, object->y, object->z);
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->facing = scene->angle;
      copy->speedLimit = 0x40000;
      copy->acceleration = 0x40000;
      copy->motionFlags = 0;
      Actor_SelectAnimationFar(copy, 5);
      Actor_SetSpriteColorModeFar(copy, 2);
      resource = Sprite_ShareVramCacheEntry__f0dcb44(copy->sprite, resource);
    }
  }
  marker = resource->vramBlock;

  if (scene->enabled != 0) {
    source = scene->mainObject;
    pos.x = source->x;
    pos.y = source->y + 0x100000;
    pos.z = source->z;
    Vector_AddPolarOffset__f0de21c(0x380000, scene->angle, &pos);
  } else {
    pos.x = scene->x;
    pos.y = scene->y + 0x100000;
    pos.z = scene->z;
  }
  Actor_SetMoveTargetFar(object, pos.x, pos.y, pos.z);
  Actor_StartScriptFar(object, gData_080f0ef8);
  for (i = 0; i < 3; i++) {
    copy = copies[i];
    if (copy != NULL) {
      wait(3);
      Actor_SetMoveTargetFar(copy, pos.x, pos.y, pos.z);
      Actor_StartScriptFar(copy, gData_080f0e58);
    }
  }

  for (i = 0; i <= 59 && object->script != NULL; i++)
    wait(1);

  if (target != NULL && scene->childMode == 0) {
    if (scene->childOption != 0)
      target->velocityY = 0x80000;
    pos.x = target->x;
    pos.y = target->y;
    pos.z = target->z;
    Vector_AddPolarOffset__f0de21c(0x100000, scene->angle, &pos);
    if (Func_08020210__f0de21c(target, &pos) == 0 &&
        ActorScript_CheckOverlapFar(target, &pos) == 0) {
      target->acceleration = 0x10000;
      target->speedLimit = 0x10000;
      Actor_SetMoveTargetFar(target, pos.x, pos.y, pos.z);
    }
  }

  event = Scene_FindPsynergyTrigger(0x50000005, 4, Func_080ce31c(4));
  if (event != NULL)
    Field_RunPsynergyEvent(event, scene->firstObjectId, scene->secondObjectId);
  wait(10);
  FieldPsynergy_FadeFromCastColor();
  wait(20);
  if (marker != 96)
    VramCache_FreeEntry(marker);
}

struct OrbitingParticle;

s32 FieldEffect_UpdateFrostGlint(struct OrbitingParticle *particle) {
  s32 phase = gFrameCount & 7;

  if (phase == 0) {
    Actor_SetSpriteColorModeFar((struct Actor *)particle, 2);
  } else if (phase == 2) {
    Actor_SetSpriteColorModeFar((struct Actor *)particle, 0);
  }
  return 0;
}

void FieldEffect_UpdateOrbitingParticleFade(struct Actor *object) {
  s32 scale;

  if (object != NULL) {
    scale = object->scaleX - 0x1000;
    object->scaleY -= 0x1000;
    object->scaleX = scale;
    if (scale <= 0x1000)
      Actor_StartScriptFar((struct Actor *)object,
                           (u32)gBattleEffect_SharedParticleScript);
  }
}

void FieldEffect_UpdateOrbitingParticleLeft(struct Actor *particle) {
  struct OrbitingParticleVector pos;
  s16 life;
  s32 count;

  if (particle != NULL) {
    count = (u16)particle->timer - 1;
    particle->timer = count;
    life = count;
    if (life != 0) {
      pos.x = particle->targetX;
      pos.y = particle->targetY;
      pos.z = particle->targetZ;
      Vector_AddPolarOffset__f0de21c(life << 17,
                                     (s16)particle->unk66 + (life << 11), &pos);
      SET_POSITION(particle, pos.x, pos.y, pos.z);
    } else {
      particle->update = (void (*)(void))FieldEffect_UpdateOrbitingParticleFade;
    }
  }
}

void FieldEffect_UpdateOrbitingParticleRight(struct Actor *particle) {
  struct OrbitingParticleVector pos;
  s16 life;
  s32 count;

  if (particle != NULL) {
    count = (u16)particle->timer - 1;
    particle->timer = count;
    life = count;
    if (life != 0) {
      pos.x = particle->targetX;
      pos.y = particle->targetY;
      pos.z = particle->targetZ;
      Vector_AddPolarOffset__f0de21c(life << 17,
                                     (s16)particle->unk66 - (life << 11), &pos);
      SET_POSITION(particle, pos.x, pos.y, pos.z);
    } else {
      particle->update = (void (*)(void))FieldEffect_UpdateOrbitingParticleFade;
    }
  }
}

void FieldPsynergy_FrostOnTarget(void) {
  struct BattleFxScene *state = Ram_HeapSlots->effectWork;
  struct OrbitingParticleChild *child = state->child;

  if (child != 0) {
    if (state->childMode != 0) {
      state->enabled = 1;
    }
    child->flags |= 2;
    FieldPsynergy_Frost();
  }
}

/// Sixteen particles, one every three frames, orbit points scattered 2 to 6
/// pixels around the effect; the target then flashes eight times, takes the
/// orbiting particles' main update and runs its pending event.
void FieldPsynergy_Frost(void) {
  struct OrbitingParticleVector pos;
  struct OrbitingParticleVector *p;
  struct BattleFxScene *scene;
  struct Actor *target;
  struct Actor *particle;
  struct PendingEvent *ev;
  s32 cnt;

  scene = Ram_HeapSlots->effectWork;
  target = scene->child;
  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(115);

  p = &pos;
  cnt = 15;
  do {
    particle = Actor_Spawn__f0de21c(681, 0, 0, 0);
    if (particle != NULL) {
      u32 scale;
      s32 dist;

      scale = (Random16() >> 1) + 0x8000;
      particle->scaleY = scale;
      particle->scaleX = scale;
      if ((Random16() & 1) != 0)
        particle->update =
            (void (*)(void))FieldEffect_UpdateOrbitingParticleLeft;
      else
        particle->update =
            (void (*)(void))FieldEffect_UpdateOrbitingParticleRight;

      particle->facing = Random16();
      particle->timer = 60;
      particle->unk66 = Random16();
      Actor_SetSpriteColorModeFar((struct Actor *)particle, 9);

      SET_POSITION(p, scene->x, scene->y, scene->z);
      dist = (Random16() << 2) + (2 << 16);
      Vector_AddPolarOffset__f0de21c(dist, Random16(), p);
      particle->targetX = p->x;
      particle->targetY = p->y;
      particle->targetZ = p->z;
    }

    wait(3);
    cnt--;
  } while (cnt >= 0);

  wait(10);
  Sound_PlayFar(115);
  wait(50);

  if (target != NULL && scene->enabled == 0) {
    Sound_PlayFar(212);

    cnt = 7;
    do {
      Actor_SetSpriteColorModeFar((struct Actor *)target, 7);
      wait(1);
      Actor_SetSpriteColorModeFar((struct Actor *)target, 0);
      wait(4);
      cnt--;
    } while (cnt >= 0);

    if (scene->childOption == 0) {
      Sound_PlayFar(220);
      Actor_SelectAnimationFar(target, 2);
    }

    target->update = (void (*)(void))FieldEffect_UpdateFrostGlint;
    ev = Scene_FindPsynergyTrigger(0x50000005, 6, Func_080ce31c(6));
    if (ev != NULL)
      Field_RunPsynergyEvent(ev, scene->firstObjectId, scene->secondObjectId);
    wait(20);
  }

  FieldPsynergy_FadeFromCastColor();
}
#include "module/field/event_commands.h"
#include "module/field/object.h"

/// Per-frame update: sways 4 pixels either side of the anchor, once every
/// 128 frames.
void FieldEffect_SwayParticle(struct SwayObject *object) {
  s16 *phase = &object->phase;
  s32 amplitude = 0x40000;

  object->y = object->baseY + Iwram_MulQ16(amplitude, Trig_Sin(*phase << 9));
  (*phase)++;
  *phase = (*phase + 128) % 128;
}

/// The particle circles 10 pixels above the scene's position, its radius in
/// pixels the frames it has left; at 0 it runs the shared particle script.
void FieldEffect_UpdateShrinkingOrbitActor(struct Actor *object) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct OrbitVector pos;
  s16 life;
  s32 count;
  s32 radius;
  s32 lift;

  if (object != NULL) {
    count = (u16)object->timer - 1;
    object->timer = count;
    life = count;
    if (life != 0) {
      pos.x = scene->x;
      lift = 160;
      pos.y = scene->y + (lift << 12);
      radius = life << 16;

      pos.z = scene->z;
      Vector_AddPolarOffset__f0de21c(radius, (s16)object->unk66 + (life << 11),
                                     &pos);
      SET_POSITION(object, pos.x, pos.y, pos.z);
    } else {
      Actor_StartScriptFar((struct Actor *)object,
                           gBattleEffect_SharedParticleScript);
    }
  }
}

void FieldEffect_RunSparkEmitter(struct SparkObject *obj) {
  struct BattleFxScene *scene;
  s16 *phase;
  struct SparkObject *spark;
  s32 pos[3];

  scene = Ram_HeapSlots->effectWork;
  phase = &obj->phase;
  if (*phase != -1) {
    s32 amplitude = 6 << 16;

    obj->x = scene->x + Iwram_MulQ16(amplitude, Trig_Sin(*phase << 10));
    obj->y = scene->y + (16 << 16);
    obj->z = scene->z;
    (*phase)++;
    *phase = (*phase + 64) % 64;
  }
  if (gFrameCount % 3 == 0) {
    pos[0] = obj->x;
    pos[1] = obj->y + (2 << 16);
    pos[2] = obj->z;
    Vector_AddPolarOffset__f0de21c(Random16() * 6, Random16(), pos);
    spark = Actor_Spawn__f0de21c(673, pos[0], pos[1], pos[2]);
    if (spark != 0) {
      spark->update = FieldEffect_SwayParticle;
      spark->scaleX = spark->scaleY = 0x9999;
      spark->motionFlags = 2;
      spark->gravity = 458;
      spark->phase = Random16() >> 9;
      spark->anchorX = spark->x;
      Actor_SetSpriteColorModeFar(spark, 9);
      spark->unk5e = 72;
      Actor_StartScriptFar((struct Actor *)spark,
                           gBattleEffect_SharedParticleScript);
    }
  }
}

/// Effect 3 cast on a target: runs effect 3, then stops the caster's hook.
void FieldPsynergy_PlyOnTarget(void) {
  FieldPsynergy_Ply();
  BattleEffect_StopMainActor();
}

/// Eight particles spiral in on the main object, one a frame; then a centre
/// object flashes for 60 frames, emits sparks and flies off.
void FieldPsynergy_Ply(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *target = scene->mainObject;
  struct Actor *object;
  struct ResourceEntry *last;
  u8 marker;
  s32 i;

  FieldPsynergy_FadeToCastColor();

  last = 0;
  for (i = 0; i < 8; i++) {
    object =
        Actor_Spawn__f0de21c(296, target->x, target->y + 0x200000, target->z);
    if (object != 0) {
      object->scaleY = 0xb333;
      object->scaleX = 0xb333;
      object->update = (void (*)(void))FieldEffect_UpdateShrinkingOrbitActor;
      object->timer = 120;
      object->unk66 = i << 13;
      object->motionFlags = 4;
      last = Sprite_ShareVramCacheEntry__f0dcb44(object->sprite, last);
    }
    wait(1);
  }

  marker = last->vramBlock;
  Sound_PlayFar(130);
  wait(110);

  target = Actor_Spawn__f0de21c(296, 0, 0, 0);
  object = target;
  if (target != 0) {
    target->scaleY = 0xb333;
    target->scaleX = 0xb333;
    SET_POSITION(target, scene->x, scene->y + 0x100000, scene->z);
    target->motionFlags = 4;
    Actor_SetSpriteColorModeFar(target, 7);
  }

  Sound_PlayFar(131);
  wait(12);
  if (target != 0) {
    for (i = 0; i < 30; i++) {
      /* FAKEMATCH(temporary): a redundant copy keeps object as a second
       * register copy of the centre object (r5 beside r6); without it, 123 of
       * 188 instructions differ in tla-en. */
      target = object;
      if (i & 3)
        Actor_SetSpriteColorModeFar(object, 9);
      else
        Actor_SetSpriteColorModeFar(object, 10);
      wait(2);
    }
  }

  Actor_SetSpriteColorModeFar(object, 0);
  Sound_PlayFar(84);
  if (object != 0) {
    target->update = (void (*)(void))FieldEffect_RunSparkEmitter;
    target->timer = 0;
    if (scene->enabled != 0)
      wait(128);
    else
      wait(192);
  }

  if (target != 0) {
    target->timer = -1;
    target->speedLimit = 0x50000;
    target->acceleration = 0x6666;
    target->actionFlags = 0;
    Actor_SetTargetPositionFromMagnitudeAngle(target, 0xc00000, 0xe800);
    Actor_WaitForMoveFar(target);
    Actor_ReleaseFar(target);
  }
  if (marker != 0x60)
    VramCache_FreeEntry(marker);
  if (scene->finishCallback != 0)
    scene->finishCallback();
  FieldPsynergy_FadeFromCastColor();
}

/* VIEW(f0de21c): tag struct EventWork as struct EventWork__f0de21c;
 * event_work.h has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/field/event_commands/mind_read.c:2076: structure has no member
 * named `state'. */
struct EventWork__f0de21c {
  u8 unk000[0x166];
  u16 state;
};

s32 Func_080dbca8(s32 x, s32 z, s32 layer);

/// A task of Cloak: when the controlled character does not
/// stand on a tile Cloak can hide on, requests CLOAK_END (0x2092).
void FieldEffect_CheckCloakTile(void) {
  struct EventWork__f0de21c *event = Ram_HeapSlots->eventWork;
  struct OwnerObject *owner = ObjectTable_Get(Party_GetLeaderActor());

  if (Func_080dbca8(owner->x, owner->z, owner->layer) == 0)
    event->state = 0x2092;
}

#define SE_CLOAK_END 154

// The caster's palette while it flickers.
#define CLOAK_FLICKER_PALETTE 7

/// Show the Cloaked caster again: stop checking the ground under it, then
/// flicker it back into view over twenty frames and clear the cloak
/// (FieldPsynergy_RunExtraHandler).
void FieldPsynergy_EndCloak(void) {
  struct CloakCaster *caster =
      ((struct BattleFxScene *)Ram_HeapSlots->effectWork)->mainObject;
  struct CloakSprite *sprite = caster->sprite;
  struct AnimationEntry *entry = sprite->entries[0];
  u32 i;

  Sound_PlayFar(SE_CLOAK_END);
  Task_Remove(FieldEffect_CheckCloakTile);
  Actor_SelectAnimationFar((struct Actor *)caster, 0);
  caster->hook = 0;

  // Blink between the flicker palette half transparent and solid...
  for (i = 0; i < 5; ++i) {
    entry->param = CLOAK_FLICKER_PALETTE;
    sprite->blend = 2;
    sprite->dirty = 1;
    wait(2);
    sprite->dirty = 1;
    sprite->blend = 0;
    wait(2);
  }

  // ...then between the flicker palette and the caster's own.
  for (i = 0; i < 5; ++i) {
    entry->param = CLOAK_FLICKER_PALETTE;
    sprite->blend = 0;
    sprite->dirty = 1;
    wait(2);
    entry->param = 0;
    sprite->dirty = 1;
    wait(2);
  }

  sprite->blend = 1;
  gMainRoutineWork.cloaked = 0;
}

void FieldPsynergy_ResumeCloakCheck(void) {
  if (gMainRoutineWork.cloaked != 0)
    Task_Add(FieldEffect_CheckCloakTile, 0x480);
}
#undef SE_CLOAK_END
#undef CLOAK_FLICKER_PALETTE
