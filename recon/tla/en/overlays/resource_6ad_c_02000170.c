// Matches: differ 0 (tla-en, 2026-11-02)
// The same form as games/tla/src/scene/mars_lighthouse_room_f/s0170.c, which
// already holds the non-Japanese editions. The call must go through room.h's
// Vector_AddPolar inline: a direct Vector_AddPolarOffsetFar call has the same
// skeleton shape only at gap 6, differ 18. The Japanese form is a lone call to
// Func_02005688 and stays on its listing.
#include "../../../../games/tla/src/scene/mars_lighthouse_room_f/room.h"

void Func_02000170(void) {
  s32 position[3];
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 angle = object->angle >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = object->x;
  position[1] = (object->y + 0x40000) & 0xfff80000;
  position[2] = object->z;
  Vector_AddPolar(0x100000, angle, position);
  object = Func_02005680(position, 1);
  if (object != NULL) {
    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z;
    Vector_AddPolar(0x100000, angle, position);
    if (Func_02005680(position, 1) != NULL)
      return;
  }
  Func_02005688();
}
