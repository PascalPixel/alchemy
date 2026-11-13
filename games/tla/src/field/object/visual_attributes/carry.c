/// @file
/// Carry.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "sounds.h"

#include "types.h"
#include "system/system.h"
#include "field/objdisp.h"
#include "actor_view.h"
#include "actor.h"
#include "field/field_psynergy_work.h"
#include "system/ram_buffer.h"
#include "battle/battle.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/field/object.h"

void FieldPsynergy_Carry(void) {
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
  pair[0] = FieldEffect_SpawnCarryHand(pos.x + 0x200000, pos.y, pos.z, 0x8000);
  pair[1] = FieldEffect_SpawnCarryHand(pos.x - 0x200000, pos.y, pos.z, 0);

  wait(15);
  walk = pair;
  for (n = 0; n < 2; n++) {
    obj = *walk++;
    if (obj != 0)
      Actor_SetTargetPositionFromMagnitudeAngle(obj, px(24), obj->facing);
  }

  Actor_WaitForMoveFar(pair[0]);
  Sound_PlayFar(SE_HEAVY_IMPACT);
  for (n = 0; n < 24; n++) {
    pos.x = work->x;
    pos.y = work->y + 0x100000;
    pos.z = work->z;
    obj = Actor_Spawn(673, pos.x, pos.y, pos.z);
    if (obj != 0) {
      Actor_StartScriptFar(obj, gFieldEffect_FragmentScript);
      obj->speedLimit = Random16() + 0x20000;
      obj->acceleration = px(2);
      obj->motionFlags = 0;
      dist = Random16() * 24 + 0x80000;
      Actor_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());
    }
  }

  Actor_ReleaseFar(pair[0]);
  Actor_ReleaseFar(pair[1]);
  FieldPsynergy_FadeFromCastColor();
}

#include "fixed_math.h"

/// Spawns object 278 and grows it from a quarter to full size, an eighth of a
/// turn a frame, then faces it to angle.
struct Actor *FieldEffect_SpawnCarryHand(s32 x, s32 y, s32 z, s32 angle) {
  struct Actor *object;
  s32 scale;

  Sound_PlayFar(SE_ITEM_BREAK);
  object = Actor_Spawn(278, x, y, z);
  if (object != NULL) {
    object->scaleY = Q16_ONE / 4;
    object->scaleX = Q16_ONE / 4;
    object->speedLimit = px(3);
    object->acceleration = px(3);
    object->actionFlags = 0;
    Actor_SelectAnimationFar(object, 1);
    scale = object->scaleX;
    if (scale < Q16_ONE) {
      do {
        scale += 0x800;
        object->scaleY = scale;
        object->scaleX = scale;
        object->facing += TRIG_TURN / 8;
        wait(1);
        scale = object->scaleX;
      } while (scale <= 0xffff);
    }
    object->facing = angle;
  }
  return object;
}
