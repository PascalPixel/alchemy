// Draft, differ 403 (501 before statements reordered 2026-10-26) of 444 (2026-10-25): Move cast on a target, The Broken
// Seal's FieldEffect_SteerPushedTarget with this game's calls. The hand
// (BattleEffect_StartItemBreak) appears 16 pixels above the target on the
// caster's side and pushes it 16 pixels the way the d-pad points, for as long
// as the player pushes; A, B, L or R end it unless the effect scene's mode is
// set. Where it cannot go, cue 114 plays every 16 frames. This game adds the
// scene's direction hook (its 0xffff ends the effect), the target's layer
// kept and restored, the push's pending event (Func_080ce458) run once the
// target has moved, and its own test of the cell beyond the caster
// (Func_080202f0). The blocks follow the native's layout (each refusal
// written out, as The Broken Seal writes them); left: register allocation
// (the native keeps the target in r10, the hand in r9, the push's direction
// in r7).
#include "types.h"
#include "ram_buffer.h"
#include "battle/fx_scene.h"
#include "frame.h"
#include "input.h"
#include "actor.h"

struct Vec3 {
  s32 x;
  s32 y;
  s32 z;
};

struct PendingEvent;

void BattleEffect_InitializeSharedScene(void);
void BattleEffect_PrepareBufferInterpolation(void);
struct Actor *BattleEffect_StartItemBreak(struct Actor *object);
void BattleEffect_SnapScaleToFull(struct Actor *object);
s32 Func_080ce31c(s32 kind);
struct PendingEvent *Func_080ce458(u32 filter, s32 kind, s32 selector);
void Actor_StartScriptFar(struct Actor *object,
                                  const void *script);
void Animation_ApplyChildValuesFar(struct Actor *object, u32 values);
void Vector_AddPolarOffset(s32 radius, s32 angle, struct Vec3 *position);
u32 BattleEffect_GetCycledTableWord(u32 keys);
s32 Func_08020210(struct Actor *object, struct Vec3 *position);
struct Actor *
ScriptObject_FindOverlappingEntryFar(struct Actor *object,
                                     struct Vec3 *position);
s32 Func_080202f0(s32 layer);
struct Actor *Actor_GetById(s32 id);
void Actor_SetPositionAndResetMotionFar(struct Actor *object, s32 x,
                                         s32 y, s32 z);
s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode,
                                 s32 target);
void BattleEffect_StopMainActor(void);
void UpdateRisingParticleBurst(void *object);
void Actor_ApplyRandomChildValues(void);
void WaitFrames(s32 frames);

extern const s32 gData_080f0e60[];
extern const u8 Data_080f0ee4[];

#define MOVE_CELL_MASK 0xfff00000 ///< a 16-pixel cell

void Func_080dd054(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *user = scene->mainObject;
  struct Actor *target = scene->child;
  s32 angle = (u16)scene->angle + 0x8000;
  s32 moved = 0;
  u8 *layer = &target->terrainId;
  s32 pushed = 0;
  u8 savedLayer = *layer;
  struct Actor *pusher;
  struct Vec3 position;
  struct Vec3 beyond;
  struct Actor *hit;
  u8 *pusherFlags;
  struct PendingEvent *event;
  s32 direction;
  s32 value;
  s32 cellX, cellZ;
  s32 x, z;

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

  event = Func_080ce458(0x50000005, scene->fieldKind,
                        Func_080ce31c(scene->fieldKind));
  position.x = target->x;
  pusher->watchedObject = target;
  position.y = target->y + 0x100000;
  position.z = target->z;
  Vector_AddPolarOffset(0x100000, angle, &position);
  Actor_SetTargetPosition(pusher, position.x, position.y, position.z);
  BattleEffect_SnapScaleToFull(pusher);
  value = 0x8000;
  pusher->speedLimit = 0x40000;
  pusher->acceleration = value;
  pusherFlags = &pusher->motionFlags;
  *pusherFlags = 4;
  target->update = Actor_ApplyRandomChildValues;
  target->speedLimit = 0x6666;
  target->acceleration = 0x3333;
  target->actionFlags = moved;
  *layer = 2;
  while (1) {
    WaitFrames(1);
    if ((s8)scene->mode == 0 && (gInput.pressed & 0x303))
      break;

    direction = (u16)BattleEffect_GetCycledTableWord(gInput.held);
    if (scene->finishCallback != NULL) {
      direction = ((s32 (*)(void))scene->finishCallback)();
      if (direction == 0xffff)
        break;
    } else if (direction == 0xffff) {
      position.x = target->x;
      position.y = target->y + 0x100000;
      position.z = target->z;
      Vector_AddPolarOffset(0x100000, angle, &position);
      Actor_SetTargetPosition(pusher, position.x, position.y, position.z);
      Actor_SetMode(pusher, 1);
      // XXX: the hand's velocity is set from a variable the function reuses
      // (0x8000, 16 pixels or the cell mask), where The Broken Seal clears
      // it; likely a slip, because nothing reads the value as a speed.
      pusher->velocityX = value;
      pusher->velocityY = value;
      pusher->velocityZ = value;
      continue;
    }

    position.x = target->x;
    value = 0x100000;
    position.y = target->y + value;
    position.z = target->z;
    Vector_AddPolarOffset(value, angle, &position);
    Vector_AddPolarOffset(0x20000, direction, &position);
    Actor_SetTargetPosition(pusher, position.x, position.y, position.z);
    Actor_CommitPosition(pusher);
    position.x = target->x;
    position.y = target->y;
    position.z = target->z;
    Vector_AddPolarOffset(value, direction, &position);
    beyond.x = target->x;
    beyond.y = target->y;
    beyond.z = target->z;
    Vector_AddPolarOffset(0x200000, direction, &beyond);
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
      cellZ = user->z & MOVE_CELL_MASK;
      cellX = user->x & MOVE_CELL_MASK;
      if (cellX == (position.x & MOVE_CELL_MASK) &&
          cellZ == (position.z & MOVE_CELL_MASK)) {
        Actor_SetMode(pusher, 4);
        if ((gFrameCount & 15) == 0)
          Sound_PlayFar(114);
        continue;
      }
      value = MOVE_CELL_MASK;
      if (cellX == (beyond.x & value) && cellZ == (beyond.z & value)) {
        if (Func_080202f0(user->terrainId) != 0) {
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
    Actor_SetMode(pusher, Data_080f0ee4[(u16)(angle - direction) >> 14]);
    WaitFrames(15);
    target->movementState = 0;
    target->speedLimit = 0x3333;
    target->acceleration = 0x3333;
    *pusherFlags = 0;
    Actor_SetTargetPosition(target, position.x, position.y, position.z);
    pusher->speedLimit = 0x3333;
    pusher->acceleration = 0x3333;
    value = 0x100000;
    Vector_AddPolarOffset(value, direction, &position);
    Actor_SetTargetPosition(pusher, position.x, position.y + value, position.z);
    if (moved == 1) {
      Actor_GetById(scene->firstObjectId)->actionFlags &= 0xfe;
      user->speedLimit = 0x3333;
      user->acceleration = 0x3333;
      Actor_SetTargetPosition(user, beyond.x, beyond.y, beyond.z);
    }
    Actor_CommitPosition(target);
    if (event == NULL) {
      target->x = x;
      target->velocityX = 0;
      target->z = z;
      target->velocityZ = 0;
    }
    break;
  }

  *layer = savedLayer;
  Animation_ApplyChildValuesFar(target, scene->savedPalette);
  Actor_StartScriptFar(target, scene->savedScript);
  target->update = (void (*)(void))scene->savedCallback;
  if (pushed) {
    pusher->update = NULL;
    Actor_SetMode(pusher, 4);
    Actor_SetPositionAndResetMotionFar(pusher, pusher->x, pusher->y,
                                        pusher->z);
    Field_RunPsynergyEvent(event, scene->firstObjectId,
                                 scene->secondObjectId);
  }
  BattleEffect_StopMainActor();
  if (moved == 1)
    Actor_GetById(scene->firstObjectId)->actionFlags |= 1;
  BattleEffect_PrepareBufferInterpolation();
  UpdateRisingParticleBurst(pusher);
}
