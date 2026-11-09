#include "types.h"
#include "ram_buffer.h"
#include "field/field_psynergy_work.h"
#include "frame.h"
#include "input.h"
#include "actor.h"
#include "fixed_math.h"

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
void Actor_StartScriptFar(struct Actor *object, const void *script);
void Animation_ApplyChildValuesFar(struct Actor *object, u32 values);
void Vector_AddPolarOffset(s32 radius, s32 angle, struct Vec3 *position);
u32 BattleEffect_GetCycledTableWord(u32 keys);
s32 Func_08020210(struct Actor *object, struct Vec3 *position);
struct Actor *ScriptObject_FindOverlappingEntryFar(struct Actor *object,
                                                   struct Vec3 *position);
s32 Func_080202f0(s32 layer, s32 x, s32 z);
struct Actor *Actor_GetById(s32 id);
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

  event = Func_080ce458(0x50000005, scene->fieldKind,
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

    direction = (u16)BattleEffect_GetCycledTableWord(gInput.held);
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
