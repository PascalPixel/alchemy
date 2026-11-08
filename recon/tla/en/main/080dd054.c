// Differ 2 of 444 (tla-en, 2026-11-08, -mtune=gba).
// Later on 2026-11-08: pushed for moved as the actionFlags value (2), moved = 0 set
// later or folded into the store (66, 80), moved declared first (52), once-only
// blocks round the acceleration store or before or after the actionFlags store (4
// to 6), (s8)moved and a folded pusherFlags assignment (2) left the tie.
// 2026-11-07: all 120 orders of the target's four stores and *layer = 2, moved
// as u8 or s16, temporaries, asm barriers and 8 permuter minutes left it at 2.
// Move cast on a target, the twin of The Broken Seal's FieldPsynergy_SteerPushedTarget (same
// locals in the same order, the same blocks and the same push, with this
// game's calls). The hand (BattleEffect_StartItemBreak) appears 16 pixels above
// the target on the caster's side and pushes it 16 pixels the way the d-pad
// points, for as long as the player pushes; A, B, L or R end it unless the
// effect scene's mode is set. Where it cannot go, cue 114 plays every 16
// frames. This game adds the scene's direction hook (its 0xffff ends the
// effect), the target's layer kept and restored, the push's pending event
// (Func_080ce458) run once the target has moved, a target that lands on its
// cell only with no event, and the palette byte read from 0x40 rather than the
// shared struct's savedPalette at 0x44. Func_080202f0 takes the layer and
// beyond's x and z, as the twin's cell test does. Stack slots follow the
// declaration order, so the declarations are the twin's.
// Left: one scheduling pair, moved's ldrb before adds r3 #51 (the listing has
// it after; in sched2 the two are ready together). The *layer pair now
// matches. Registers, slots and blocks all agree. Tried without gain: seven orders
// of the opening assignments, three of the target's field stores, user and
// target swapped, s32 and u32 local types, target->actionFlags = 0, a direct
// read of terrainId and a combined layer assignment. A load-order tie in sched2.
// 2026-11-08: once-only blocks around each nearby store, (u8)moved, pushed for
// moved, the declaration order of moved and pushed, a constant local for
// 0x3333 and 7 permuter minutes (22,560 candidates) gave 2 to 22.
#include "types.h"
#include "ram_buffer.h"
#include "field/field_psynergy_work.h"
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
s32 Func_080202f0(s32 layer, s32 x, s32 z);
struct Actor *Actor_GetById(s32 id);
void Actor_SetPositionAndResetMotionFar(struct Actor *object, s32 x,
                                         s32 y, s32 z);
s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode,
                                 s32 target);
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
  struct Vec3 *candidate;
  s32 (*callback)(void);
  u8 savedLayer;
  u8 *layer;
  struct PendingEvent *event;
  s32 pushed;
  u8 *pusherFlags;
  u8 *userFlags;
  const u8 *modes;

  scene = Ram_HeapSlots->effectWork;
  user = scene->mainObject;
  target = scene->child;
  angle = (u16)scene->angle + 0x8000;
  moved = 0;
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
  candidate = &position;
  candidate->x = target->x;
  candidate->y = target->y + 0x100000;
  candidate->z = target->z;
  Vector_AddPolarOffset(0x100000, angle, candidate);
  Actor_SetTargetPosition(pusher, candidate->x, candidate->y, candidate->z);
  BattleEffect_SnapScaleToFull(pusher);
  pusher->speedLimit = 0x40000;
  pusher->acceleration = 0x8000;
  pusherFlags = &pusher->motionFlags;
  *pusherFlags = 4;
  target->update = Actor_ApplyRandomChildValues;
  target->speedLimit = 0x6666;
  target->acceleration = 0x3333;
  target->actionFlags = moved;
  *layer = 2;
  while (1) {
    wait(1);
    if ((s8)scene->mode == 0 && (gInput.pressed & 0x303))
      break;

    direction = (u16)BattleEffect_GetCycledTableWord(gInput.held);
    callback = (s32 (*)(void))scene->finishCallback;
    if (callback != NULL) {
      direction = callback();
      if (direction == 0xffff)
        break;
    } else if (direction == 0xffff) {
      position.x = target->x;
      position.y = target->y + 0x100000;
      position.z = target->z;
      Vector_AddPolarOffset(0x100000, angle, &position);
      Actor_SetTargetPosition(pusher, position.x, position.y, position.z);
      Actor_SetMode(pusher, 1);
      pusher->velocityX = 0;
      pusher->velocityY = 0;
      pusher->velocityZ = 0;
      continue;
    }

    position.x = target->x;
    position.y = target->y + 0x100000;
    position.z = target->z;
    Vector_AddPolarOffset(0x100000, angle, &position);
    Vector_AddPolarOffset(0x20000, direction, &position);
    Actor_SetTargetPosition(pusher, position.x, position.y, position.z);
    Actor_CommitPosition(pusher);
    position.x = target->x;
    position.y = target->y;
    position.z = target->z;
    Vector_AddPolarOffset(0x100000, direction, &position);
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
    hit = ScriptObject_FindOverlappingEntryFar(target, candidate);
    if (hit != NULL) {
      if (hit != user) {
        Actor_SetMode(pusher, 4);
        if ((gFrameCount & 15) == 0)
          Sound_PlayFar(114);
        continue;
      }
      cellX = user->x & MOVE_CELL_MASK;
      cellZ = user->z & MOVE_CELL_MASK;
      if (cellX == (candidate->x & MOVE_CELL_MASK) &&
          cellZ == (candidate->z & MOVE_CELL_MASK)) {
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
    *pusherFlags = 0;
    pusher->speedLimit = 0x3333;
    pusher->acceleration = 0x3333;
    Vector_AddPolarOffset(0x100000, direction, &position);
    Actor_SetTargetPosition(pusher, position.x, position.y + 0x100000,
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
    Actor_SetPositionAndResetMotionFar(pusher, pusher->x, pusher->y,
                                        pusher->z);
    Field_RunPsynergyEvent(event, scene->firstObjectId,
                                 scene->secondObjectId);
  }
  BattleEffect_StopMainActor();
  if (moved == 1) {
    userFlags = &Actor_GetById(scene->firstObjectId)->actionFlags;
    *userFlags |= 1;
  }
  BattleEffect_PrepareBufferInterpolation();
  UpdateRisingParticleBurst(pusher);
}
