/// @file
/// Reveal and Growth.
// Part of f0dcb44.c.

#include "../f0dcb44_internal.h"
#include "event.h"
#include "module/field/event_commands.h"
#include "module/field/f0dcb44.h"
#include "module/field/object.h"

/* VIEW(f0dd820): routine BattleEffect_RunBusyWaitDelay as void BattleEffect_RunBusyWaitDelay(s32 value); the definition has void BattleEffect_RunBusyWaitDelay(void); tried as the definition's void BattleEffect_RunBusyWaitDelay(void), which the object already declares, it does not compile in tla-ja: games/tla/src/field/event_commands/f0dcb44.c:877: too many arguments to function `BattleEffect_RunBusyWaitDelay'. */
extern void BattleEffect_RunBusyWaitDelay__f0dd820(s32 value) __asm__("BattleEffect_RunBusyWaitDelay");

#define FIELD_FX_REVEAL 8
#define REVEAL_EVENT_START 0x50000005 // the Reveal event, stage 5
#define REVEAL_FRAMES 600
#define SE_REVEAL 131

// Reveal: open a circle of the transition on the caster, wash the screen
// plain, run the Reveal event that matches, then show the markers nearby
// and watch how far the leader walks.
void Func_080dd820(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct RevealEvent *event;
  struct ScreenTransitionState *transition;
  struct Actor *caster;
  void *pending;
  s32 frame;

  BattleEffect_RunBusyWaitDelay__f0dd820(6);
  event = Ram_HeapSlots->eventWork;
  ScreenTransition_InitializeBattleEffectState(FIELD_FX_REVEAL);
  transition = Ram_HeapSlots->screenTransition;
  caster = scene->mainObject;
  transition->centreX = caster->x;
  transition->centreY = caster->z - caster->y;
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer__f0dcb44(0x10001, 1);
  BattleEffect_StartBufferInterpolation(1);
  event->active = 0;
  wait(1);

  pending = Func_080ce458(REVEAL_EVENT_START, FIELD_FX_REVEAL,
                          Func_080ce31c(FIELD_FX_REVEAL));
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
  Func_080cf554();
  frame = 0;
  do {
    wait(1);
    transition->value = frame;
    frame++;
  } while (frame <= 18);
  if ((s8)scene->mode == 0)
    Task_Add(Func_080dd754, 0x480);
}

void Func_080dd950(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct ScreenTransitionState *transition = Ram_HeapSlots->screenTransition;
  struct RevealEvent *event = Ram_HeapSlots->eventWork;
  void *pending;

  if (event->active != 0) {
    Sound_PlayFar(167);
    Task_Remove(Func_080dd754);
    event->active = 0;
    event->frames = 0;
    BattleEffect_RunBusyWaitDelay__f0dd820(0);
    BattleEffect_ApplyColorToTargetBuffer__f0dcb44(0x10000, 1);
    BattleEffect_StartBufferInterpolation(1);
    BattleEffect_ApplyColorToSourceBuffer(0, 0);
    BattleEffect_ApplyColorToTargetBuffer__f0dcb44(0x10000, 0);
    BattleEffect_StartBufferInterpolation(30);
    wait(1);
    pending = Func_080ce458(0x40000005, FIELD_FX_REVEAL,
                            Func_080ce31c(FIELD_FX_REVEAL));
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

extern const s32 gData_080f0ee8[];

/* VIEW(f0dd820): routine Vector_AddPolarOffset as void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position); hue has extern void Vector_AddPolarOffset(s32, s32, struct ItemBreakFragmentPosition *). */
extern void Vector_AddPolarOffset__f0dd820(s32 radius, s32 angle, s32 *position) __asm__("Vector_AddPolarOffset");
/* VIEW(f0dd820): routine Actor_Spawn as struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z); hue has extern struct ItemBreakFragmentObject *Actor_Spawn(s32, s32, s32, s32). */
extern struct Actor *Actor_Spawn__f0dd820(s32 kind, s32 x, s32 y, s32 z) __asm__("Actor_Spawn");

#define BURST_FALL_OBJECT 281 ///< the object that falls round the target

// The two kinds of target that do not sink into the ground.
#define TARGET_KIND_SCATTERS 372 ///< scatters five falling pieces instead
#define TARGET_KIND_STAYS 394

/// Animation 12 on a target: the burst of particles, then the target sinks
/// to the underfoot priority. A target of kind 372 instead throws five pieces
/// round the scene's position; one of kind 394 only plays the cue (sound 126).
void Func_080dda30(s32 unused) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *target = scene->child;
  s32 kind = 0;

  if (target == 0)
    return;

  Func_080ddbd8();
  if (Func_08020330(target) == TARGET_KIND_SCATTERS)
    kind = 1;
  if (Func_08020330(target) == TARGET_KIND_STAYS)
    kind = 2;

  if (kind == 0) {
    Actor_SetMode(target, 2);
    Actor_SetSingleChildFlagsFar(target, 0);
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
      piece = Actor_Spawn__f0dd820(BURST_FALL_OBJECT, p->values[0], p->values[1],
                          p->values[2]);
      if (piece != 0) {
        Actor_StartScriptFar(piece, gData_080f0ee8);
        piece->motionFlags = 2;
      }
      wait((((u32)Random16() * 2) >> 16) + 2);
      n--;
    } while (n >= 0);
    Func_080daecc(scene->secondObjectId, 1);
  } else {
    wait(10);
    Sound_PlayFar(126);
  }
  BattleEffect_PrepareBufferInterpolation();
}

void FieldEffect_RunGrowth(void) {
  u8 *state = Ram_HeapSlots->effectWork;
  struct BurstParticleVector position;
  struct BurstParticleVector *p;
  s32 entryCount;

  Func_080ddbd8();
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
    object = Actor_Spawn__f0dd820(0x119, p->values[0], p->values[1], p->values[2]);
    if (object != 0) {
      Actor_StartScriptFar(object, gData_080f0ee8);
      *((u8 *)object + 0x55) = 2;
    }
    wait((((u32)Random16() * 2) >> 16) + 2);
    entryCount--;
  } while (entryCount >= 0);
  wait(30);
  BattleEffect_PrepareBufferInterpolation();
}

void Func_080ddbd8(void) {
  struct BattleFxScene *work;
  struct Actor *user;
  s32 pos[3];
  s32 n;
  s32 *p;

  work = Ram_HeapSlots->effectWork;
  user = work->mainObject;
  BattleEffect_InitializeSharedScene();

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
      Animation_ApplyChildValuesFar(obj, 11);
      Actor_SetMode(obj, 7);
      Actor_StartScriptFar(obj, gBattleEffect_SharedParticleScript + 1);

      Actor_SetSingleChildFlagsFar(obj, 1);

      p[0] = work->x;
      p[1] = work->y;
      p[2] = work->z;
      if (*(u16 *)&work->angle == 0xc000)
        Vector_AddPolarOffset__f0dd820(0xe0000, *(u16 *)&work->angle, p);
      dist = Random16() * 6 + 0x40000;
      Vector_AddPolarOffset__f0dd820(dist, Random16(), p);
      Actor_SetTargetPosition(obj, p[0], p[1], p[2]);
    }
    Sound_PlayFar(131);
    wait(2);
  }
  wait(8);
}
