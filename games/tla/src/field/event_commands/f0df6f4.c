#include "types.h"
#include "system.h"
#include "objdisp.h"
#include "actor_view.h"
#include "actor.h"
#include "field_psynergy_work.h"
#include "ram_buffer.h"

struct ScatterPosition {
  s32 x;
  s32 y;
  s32 z;
};

extern const s32 gData_080f0e78[];

void BattleEffect_InitializeSharedScene(void);
void BattleEffect_PrepareBufferInterpolation(void);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
struct Actor *BattleEffect_SpawnItemBreakMode1(s32 x, s32 y, s32 z, s32 angle);
void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle);
void Actor_Destroy(struct Actor *object);

void Func_080df6f4(void) {
  struct BattleFxScene *work;
  struct Actor *obj;
  struct Actor *pair[2];
  struct ScatterPosition pos;
  struct Actor **walk;
  s32 n;
  s32 dist;

  work = Ram_HeapSlots->effectWork;
  BattleEffect_InitializeSharedScene();
  pos.x = work->x;
  pos.y = work->y + 0x100000;
  pos.z = work->z;
  pair[0] =
      BattleEffect_SpawnItemBreakMode1(pos.x + 0x200000, pos.y, pos.z, 0x8000);
  pair[1] = BattleEffect_SpawnItemBreakMode1(pos.x - 0x200000, pos.y, pos.z, 0);

  wait(15);
  walk = pair;
  for (n = 0; n < 2; n++) {
    obj = *walk++;
    if (obj != 0)
      Motion_SetTargetPositionFromMagnitudeAngle(obj, 0x180000, obj->facing);
  }

  Actor_CommitPosition(pair[0]);
  Sound_PlayFar(134);
  for (n = 0; n < 24; n++) {
    pos.x = work->x;
    pos.y = work->y + 0x100000;
    pos.z = work->z;
    obj = Actor_Spawn(673, pos.x, pos.y, pos.z);
    if (obj != 0) {
      Actor_StartScriptFar(obj, gData_080f0e78);
      obj->speedLimit = Random16() + 0x20000;
      obj->acceleration = 0x20000;
      obj->motionFlags = 0;
      dist = Random16() * 24 + 0x80000;
      Motion_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());
    }
  }

  Actor_Destroy(pair[0]);
  Actor_Destroy(pair[1]);
  BattleEffect_PrepareBufferInterpolation();
}

#include "fixed_math.h"

/// Spawns object 278 and grows it from a quarter to full size, an eighth of a
/// turn a frame, then faces it to angle.
struct Actor *BattleEffect_SpawnItemBreakMode1(s32 x, s32 y, s32 z, s32 angle) {
  struct Actor *object;
  s32 scale;

  Sound_PlayFar(138);
  object = Actor_Spawn(278, x, y, z);
  if (object != NULL) {
    object->scaleY = 0x4000;
    object->scaleX = 0x4000;
    object->speedLimit = 0x30000;
    object->acceleration = 0x30000;
    object->actionFlags = 0;
    Actor_SetMode(object, 1);
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
