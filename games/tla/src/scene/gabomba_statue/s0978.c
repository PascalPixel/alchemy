#include "types.h"
#include "actor.h"
#include "event_inline.h"

extern s32 gData_02005af0;
struct Actor *Func_020005b4(struct Actor *origin, s32 id);
void Func_020006ac(struct Actor *object, s32 dx, s32 dz, s32 velocity);

// Moves the controlled character with the platform kind 359 along x by the
// speed in gData_02005af0, backward when the platform faces half a turn; it
// stays within 2 pixels of the platform's z, and past 20 pixels on x it steps
// off (Func_020006ac).
void Func_02000978(void) {
  s32 *speed = &gData_02005af0;

  if (*speed != 0) {
    struct Actor *actor = Actor_GetById(Party_GetLeaderActor());
    struct Actor *found = Func_020005b4(actor, 359);
    s32 dx = *speed << 15;

    if (found != NULL) {
      s32 diff, x, foundX;

      if (found->angle == 0x8000)
        dx = -dx;
      if (found->z - actor->z < -0x20000)
        actor->z = found->z + 0x20000;
      if (found->z - actor->z > 0x20000)
        actor->z = found->z - 0x20000;
      x = actor->x + dx;
      actor->x = x;
      foundX = found->x;
      diff = foundX - x;
      if (diff >= 0 ? diff >= 0x140000 : x - foundX >= 0x140000) {
        s32 sign;
        s32 offset;

        sign = 0;
        offset = actor->x - foundX;
        if (offset != 0) {
          sign = 1;
          if (offset <= 0)
            sign = -sign;
        }
        Func_020006ac(actor, sign * 0xc0000, 0, 0x20000);
      }
    }
  }
}
