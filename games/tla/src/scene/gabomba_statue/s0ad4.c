#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern char SceneId_Ov671Room4;
extern s32 gData_02005af0;
struct Actor *Func_020005b4(struct Actor *object, s32 kind);
void Func_020006ac(struct Actor *object, s32 dx, s32 dz, s32 speed);
void Func_02000a20(struct Actor *object);

// Moves the controlled character with the moving platforms (Func_020005b4 finds
// the one under it, 360 forward and 361 back): it stays within 2 pixels of the
// platform's x and is carried along z by the speed in gData_02005af0; past 20
// pixels from the platform it steps off (Func_020006ac). In Ov671Room4 at cell
// (28, 15) or (30, 15) it ends the ride (Func_02000a20) and runs exit 5 or 6
// (Import_EventSetExitRequest).
void Func_02000ad4(void) {
  struct Actor *actor, *platform;
  s32 speed, d, sign, x, z, platformZ, offset;

  if (gData_02005af0 == 0)
    return;
  actor = Actor_Get(FieldParty_GetLeaderActor());
  speed = gData_02005af0 << 15;
  platform = Func_020005b4(actor, 360);
  if (platform != 0) {
    if (platform->x - actor->x < -0x20000)
      actor->x = platform->x + 0x20000;
    if (platform->x - actor->x > 0x20000)
      actor->x = platform->x - 0x20000;
    z = actor->z + speed;
    actor->z = z;
    platformZ = platform->z;
    d = platformZ - z;
    if (d >= 0 ? d >= 0x140000 : z - platformZ >= 0x140000) {
      sign = 0;
      offset = actor->z - platformZ;
      if (offset != 0) {
        sign = 1;
        if (offset <= 0)
          sign = -sign;
      }
      Func_020006ac(actor, 0, (sign * 3) << 18, 0x20000);
    }
  }
  platform = Func_020005b4(actor, 361);
  if (platform == 0)
    return;
  if (platform->x - actor->x < -0x20000)
    actor->x = platform->x + 0x20000;
  if (platform->x - actor->x > 0x20000)
    actor->x = platform->x - 0x20000;
  z = actor->z - speed;
  actor->z = z;
  platformZ = platform->z;
  d = platformZ - z;
  if (d >= 0 ? d >= 0x140000 : z - platformZ >= 0x140000) {
    sign = 0;
    offset = actor->z - platformZ;
    if (offset != 0) {
      sign = 1;
      if (offset <= 0)
        sign = -sign;
    }
    Func_020006ac(actor, 0, (sign * 3) << 18, 0x20000);
  }
  if (gMainRoutineWork.scene != (s32)&SceneId_Ov671Room4)
    return;
  x = actor->x >> 20;
  if (actor->z >> 20 != 15)
    return;
  if (x == 28) {
    Func_02000a20(actor);
    FieldEvent_SetExitRequest(5);
  } else if (x == 30) {
    Func_02000a20(actor);
    FieldEvent_SetExitRequest(6);
  }
}
