#include "types.h"
#include "actor.h"
#include "adventure.h"

struct Actor *Actor_GetById(s32 id);
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
struct Actor *Func_080eaf28(s32 *position, s32 check_y);

struct Actor *Func_080eaeb4(void) {
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);
  s32 position[3];
  s32 angle = ((leader->angle >> 12) + 2) & 12;
  struct Actor *found;

  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  Vector_AddPolarOffset(0x100000, angle << 12, position);
  found = Func_080eaf28(position, 1);
  if (found != NULL)
    return found;
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  Vector_AddPolarOffset(0x200000, angle << 12, position);
  found = Func_080eaf28(position, 1);
  if (found != NULL)
    return found;
  return NULL;
}
