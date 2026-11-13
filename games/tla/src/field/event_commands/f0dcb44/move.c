/// @file
/// The hue cycle and its colours, Move, and the broken item scattering into
/// fragments.
// Part of f0dcb44.c.

#include "../f0dcb44_internal.h"

struct WaveFxScroll {
  u16 lines[2][162];
  u8 unk288[2];
  u8 page;
  u8 unk28b[9];
  u8 delay;
};

/// Applies BG0 line scrolling with HBlank DMA.
void BattleEffect_ArmBg0HBlankDma(void) {
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
s32 BattleEffect_HueChannelRamp(s16 hue) {
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
void BattleEffect_AdvanceHueCycle(void) {
  struct WaveFxWork *work = Ram_HeapSlots->unk4c[3];
  s16 r;
  s16 g;
  s16 b;
  u32 limit;

  r = BattleEffect_HueChannelRamp(work->hue);
  g = BattleEffect_HueChannelRamp(work->hue + 120);
  b = BattleEffect_HueChannelRamp(work->hue + 240);

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

extern u8 gData_080f0e9c[];

void BattleEffect_ApplyFlickerValue(void *object, s32 value) {
  s32 child = value - 1;

  if (value == 0)
    child = gData_080f0e9c[(gFrameCount >> 1) & 7];
  Animation_ApplyChildValuesFar(object, child);
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


struct ItemBreakFragmentPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct ItemBreakFragmentSource {
  u8 reserved00[6];
  u16 angle; ///< Facing angle (0x10000 is one turn).
  s32 x;
  s32 y;
  s32 z;
  u8 reserved14[70];
  u8 flag5a;
  u8 reserved5b[13];
  struct ItemBreakFragmentSource *target; ///< Facing target.
};

struct ItemBreakFragmentObject {
  u8 reserved00[72];
  s32 field48; ///< Gravity (same offset as gravity).
  u8 reserved4c[9];
  u8 mode55; ///< Same offset as motionFlags.
  u8 reserved56[8];
  u16 field5e;
};

extern s32 ArcTan2(s32, s32);

extern void Vector_AddPolarOffset(s32, s32, struct ItemBreakFragmentPosition *);
extern struct ItemBreakFragmentObject *Actor_Spawn(s32, s32, s32, s32);

/// Updates the broken item's main object: with a facing target it turns toward
/// it by at most 0x1000 (22.5 degrees), then emits one fragment particle
/// nearby.
/// src: main object.
void BattleEffect_UpdateItemBreakFragment(struct ItemBreakFragmentSource *src) {
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
  Vector_AddPolarOffset(dist, Random16(), &pos);

  obj = Actor_Spawn(0x2a1, pos.x, pos.y, pos.z);
  if (obj != 0) {
    obj->mode55 = 2;
    obj->field48 = 0x1999;
    Actor_SetMode(obj, 0);
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

void BattleEffect_InitializeSharedScene(void);
void BattleEffect_PrepareBufferInterpolation(void);
void BattleEffect_SnapScaleToFull(struct Actor *object);
s32 Func_080ce31c(s32 kind);
/* VIEW(f0dd054): routine Func_080ce458 as struct PendingEvent *Func_080ce458(u32 filter, s32 kind, s32 selector); battle.h has struct TriggerEvent *Func_080ce458(s32 filter, s32 kind, s32 selector). */
extern struct PendingEvent *Func_080ce458__f0dd054(u32 filter, s32 kind, s32 selector) __asm__("Func_080ce458");
/* VIEW(f0dd054): routine BattleEffect_GetCycledTableWord as u32 BattleEffect_GetCycledTableWord(u32 keys); the definition has u16 BattleEffect_GetCycledTableWord(void); tried as the definition's u16 BattleEffect_GetCycledTableWord(void), which the object already declares, it does not compile in tla-ja: games/tla/src/field/event_commands/f0dcb44.c:463: too many arguments to function `BattleEffect_GetCycledTableWord'. */
extern u32 BattleEffect_GetCycledTableWord__f0dd054(u32 keys) __asm__("BattleEffect_GetCycledTableWord");
s32 Func_08020210(struct Actor *object, struct Vec3 *position);
struct Actor *ScriptObject_FindOverlappingEntryFar(struct Actor *object,
                                                   struct Vec3 *position);
s32 Func_080202f0(s32 layer, s32 x, s32 z);
struct Actor *Actor_GetById(u32 objectId);
void Actor_SetPositionAndResetMotionFar(struct Actor *object, s32 x, s32 y,
                                        s32 z);
s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode, s32 target);
void BattleEffect_StopMainActor(void);
void UpdateRisingParticleBurst(void *object);
void Actor_ApplyRandomChildValues(void);
void wait(s32 frames);

extern const s32 gData_080f0e60[];
extern const u8 Data_080f0ee4[];

#define MOVE_CELL_MASK 0xfff00000 ///< a 16-pixel cell

void Func_080dd054(void) {
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
  BattleEffect_InitializeSharedScene();
  user->watchedObject = target;
  Actor_StartScriptFar(user, gData_080f0e60);
  pusher = BattleEffect_StartItemBreak(user);
  if (pusher == NULL) {
    BattleEffect_PrepareBufferInterpolation();
    return;
  }

  event = Func_080ce458__f0dd054(0x50000005, scene->fieldKind,
                        Func_080ce31c(scene->fieldKind));
  pusher->watchedObject = target;
  position.x = target->x;
  position.y = target->y + (16 << 16);
  position.z = target->z;
  Vector_AddPolarOffset(16 << 16, angle, &position);
  Actor_SetTargetPosition(pusher, position.x, position.y, position.z);
  BattleEffect_SnapScaleToFull(pusher);
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
      Vector_AddPolarOffset(16 << 16, angle, &position);
      Actor_SetTargetPosition(pusher, position.x, position.y, position.z);
      Actor_SetMode(pusher, 1);
      pusher->velocityX = 0;
      pusher->velocityY = 0;
      pusher->velocityZ = 0;
      continue;
    }

    position.x = target->x;
    position.y = target->y + (16 << 16);
    position.z = target->z;
    Vector_AddPolarOffset(16 << 16, angle, &position);
    Vector_AddPolarOffset(2 << 16, direction, &position);
    Actor_SetTargetPosition(pusher, position.x, position.y, position.z);
    Actor_CommitPosition(pusher);
    position.x = target->x;
    position.y = target->y;
    position.z = target->z;
    Vector_AddPolarOffset(16 << 16, direction, &position);
    beyond.x = target->x;
    beyond.y = target->y;
    beyond.z = target->z;
    Vector_AddPolarOffset(32 << 16, direction, &beyond);
    if (Func_08020210(target, &position) > 0) {
      Actor_SetMode(pusher, 4);
      if ((gFrameCount & 15) == 0)
        Sound_PlayFar(114);
      continue;
    }
    hit = ScriptObject_FindOverlappingEntryFar(target, &position);
    if (hit != NULL) {
      if (hit != user) {
        Actor_SetMode(pusher, 4);
        if ((gFrameCount & 15) == 0)
          Sound_PlayFar(114);
        continue;
      }
      cellX = user->x & MOVE_CELL_MASK;
      cellZ = user->z & MOVE_CELL_MASK;
      if (cellX == (position.x & MOVE_CELL_MASK) &&
          cellZ == (position.z & MOVE_CELL_MASK)) {
        Actor_SetMode(pusher, 4);
        if ((gFrameCount & 15) == 0)
          Sound_PlayFar(114);
        continue;
      }
      if (cellX == (beyond.x & MOVE_CELL_MASK) &&
          cellZ == (beyond.z & MOVE_CELL_MASK)) {
        if (Func_080202f0(user->terrainId, beyond.x, beyond.z) != 0) {
          Actor_SetMode(pusher, 4);
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
    modes = Data_080f0ee4;
    Actor_SetMode(pusher, modes[(u16)(angle - direction) >> 14]);
    wait(15);
    target->movementState = 0;
    target->speedLimit = 0x3333;
    target->acceleration = 0x3333;
    Actor_SetTargetPosition(target, position.x, position.y, position.z);
    pusher->motionFlags = 0;
    pusher->speedLimit = 0x3333;
    pusher->acceleration = 0x3333;
    Vector_AddPolarOffset(16 << 16, direction, &position);
    Actor_SetTargetPosition(pusher, position.x, position.y + (16 << 16),
                            position.z);
    if (moved == 1) {
      Actor_GetById(scene->firstObjectId)->actionFlags &= 0xfe;
      user->speedLimit = 0x3333;
      user->acceleration = 0x3333;
      Actor_SetTargetPosition(user, beyond.x, beyond.y, beyond.z);
    }
    Actor_CommitPosition(target);
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
  Animation_ApplyChildValuesFar(target, scene->unk40[0]);
  Actor_StartScriptFar(target, scene->savedScript);
  target->update = (void (*)(void))scene->savedCallback;
  if (pushed) {
    pusher->update = NULL;
    Actor_SetMode(pusher, 4);
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
  BattleEffect_PrepareBufferInterpolation();
  UpdateRisingParticleBurst(pusher);
}


void BattleEffect_ClearOwnedSlot(struct FieldParticle *);

struct Output_08097f80 {
  s32 x;
  s32 y;
  s32 z;
};

void BattleEffect_UpdateOrbitAndReturn(struct FieldParticle *effect) {
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
    Vector_AddPolarOffset(0x1e0000, (u16)angle, &position);
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
    BattleEffect_ClearOwnedSlot(effect);
}
#undef MOVE_CELL_MASK



void FieldEffect_RunMove(void) {
  // Unused: gives the stack frame its 12 bytes.
  s32 work[3];
  struct BattleFxScene *scene;
  void *object;

  scene = Ram_HeapSlots->effectWork;
  object = scene->mainObject;

  BattleEffect_InitializeSharedScene();
  object = BattleEffect_StartItemBreak(object);
  BattleEffect_SnapScaleToFull(object);
  if (object != 0) {
    Actor_SetMode(object, 4);
    wait(30);
  }
  BattleEffect_PrepareBufferInterpolation();
  UpdateRisingParticleBurst(object);
}


extern const s32 gData_080f0e78[];

/* VIEW(item_break): routine Actor_Spawn as struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z); hue has extern struct ItemBreakFragmentObject *Actor_Spawn(s32, s32, s32, s32). */
extern struct Actor *Actor_Spawn__item_break(s32 kind, s32 x, s32 y, s32 z) __asm__("Actor_Spawn");
void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle);
/* VIEW(item_break): routine BattleEffect_UpdateItemBreakFragment as void BattleEffect_UpdateItemBreakFragment(void); the definition has void BattleEffect_UpdateItemBreakFragment(struct ItemBreakFragmentSource *src). */
extern void BattleEffect_UpdateItemBreakFragment__item_break(void) __asm__("BattleEffect_UpdateItemBreakFragment");

#define SE_ITEM_BREAK 138

/// The Broken Seal's BattleEffect_StartItemBreak with this game's objects
/// 278 and 673: spawns the breaking item above object and eight fragments
/// that scatter from it.
void *BattleEffect_StartItemBreak(void *object) {
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
  parent = Actor_Spawn__item_break(278, source->x, source->y + (16 << 16), source->z);
  if (parent == 0)
    return 0;
  parent->scaleY = 0x4000;
  parent->scaleX = 0x4000;
  parent->update = BattleEffect_UpdateItemBreakFragment__item_break;
  parent->speedLimit = 2 << 16;
  parent->acceleration = 2 << 16;
  parent->motionFlags = 0;
  Actor_SetMode(parent, 3);
  Motion_SetTargetPositionFromMagnitudeAngle(parent, 16 << 16, angle);

  for (fragmentIndex = 7; fragmentIndex >= 0; fragmentIndex--) {
    child = Actor_Spawn__item_break(673, source->x, source->y + (16 << 16), source->z);
    if (child != 0) {
      Actor_StartScriptFar(child, gData_080f0e78);
      speed = Random16() + Q16_ONE;
      child->acceleration = 1 << 16;
      child->speedLimit = speed;
      child->motionFlags = 2;
      child->gravity = 0x51e;
      spread = Random16();
      child->velocityY = spread - Random16();
      radius = Random16() * 24 + (8 << 16);
      jitter = Random16();
      Motion_SetTargetPositionFromMagnitudeAngle(
          child, radius, ((jitter - Random16()) >> 3) + source->facing);
    }
  }
  Sound_PlayFar(SE_ITEM_BREAK);
  return parent;
}

/// Raises the object's scale in sixteenths to full size and commits its
/// position.
void BattleEffect_SnapScaleToFull(struct Actor *object) {
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
    Actor_CommitPosition(object);
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

      Actor_StartScriptFar(child, gData_080f0e78);
      speed = Random16();
      child->acceleration = unit;
      child->speedLimit = speed + unit;
      child->motionFlags = 2;
      child->gravity = 0xa3d;
      spread = Random16();
      child->velocityY = spread - Random16();
      dist = Random16() * 24;
      dist += 8 << 16;
      Motion_SetTargetPositionFromMagnitudeAngle(child, dist, Random16());
    }
  }
  Sound_PlayFar(131);
  Actor_Destroy(src);
}

/// Burns 63 loop iterations and returns; callers pass an argument it ignores.
void BattleEffect_RunBusyWaitDelay(void) {
  s32 i = 63;

loop:
  i--;
  if (i >= 0)
    goto loop;
}


/// The Broken Seal's FieldEffect_WatchLeaderDistance with this game's
/// offsets: counts the timer down and, once the leader is 60 pixels from the
/// centre or the timer has run out, requests the effect's end.
void Func_080dd754(void) {
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
