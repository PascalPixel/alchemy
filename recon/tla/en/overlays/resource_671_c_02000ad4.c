// Draft, differ 120 of 159: moves the controlled character with the moving
// platforms (Func_020005b4 finds the one under it, 360 forward and 361
// back): it is kept within 2 pixels of the platform's x and carried along
// z by the speed in gData_02005af0; past 20 pixels from the platform it
// steps off (Func_020006ac). In Ov671Room4 at cell (28, 15) or (30, 15) it
// ends the ride (Func_02000a20) and runs exit 5 or 6 (Func_020057c4). Left: the native keeps the speed in r6 and the actor in r5, shares one sign block per platform and orders the tests otherwise.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

extern char SceneId_Ov671Room4;
extern s32 gData_02005af0;
s32 Func_0200584c(void);
struct ObjectRuntime *Object_GetById(u32 id);
struct ObjectRuntime *Func_020005b4(struct ObjectRuntime *object, s32 kind);
void Func_020006ac(struct ObjectRuntime *object, s32 dx, s32 dz, s32 speed);
void Func_02000a20(struct ObjectRuntime *object);
void Func_020057c4(s32 exit);

void Func_02000ad4(void) {
  struct ObjectRuntime *actor, *platform;
  s32 speed, d, sign, x;

  if (gData_02005af0 == 0)
    return;
  actor = Object_GetById(Func_0200584c());
  speed = gData_02005af0 << 15;
  platform = Func_020005b4(actor, 360);
  if (platform != 0) {
    if (platform->x - actor->x < -0x20000)
      actor->x = platform->x + 0x20000;
    if (platform->x - actor->x > 0x20000)
      actor->x = platform->x - 0x20000;
    actor->z += speed;
    d = platform->z - actor->z;
    if (d >= 0 ? d >= 0x140000 : actor->z - platform->z >= 0x140000) {
      d = actor->z - platform->z;
      sign = 0;
      if (d != 0) {
        sign = 1;
        if (d <= 0)
          sign = -1;
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
  actor->z -= speed;
  d = platform->z - actor->z;
  if (d >= 0 ? d >= 0x140000 : actor->z - platform->z >= 0x140000) {
    d = actor->z - platform->z;
    sign = 0;
    if (d != 0) {
      sign = 1;
      if (d <= 0)
        sign = -1;
    }
    Func_020006ac(actor, 0, (sign * 3) << 18, 0x20000);
  }
  if (gPartyState.scene != (s32)&SceneId_Ov671Room4)
    return;
  x = actor->x >> 20;
  if (actor->z >> 20 != 15)
    return;
  if (x == 28) {
    Func_02000a20(actor);
    Func_020057c4(5);
  } else if (x == 30) {
    Func_02000a20(actor);
    Func_020057c4(6);
  }
}
