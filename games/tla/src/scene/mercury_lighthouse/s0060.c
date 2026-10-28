#include "types.h"
#include "actor.h"
#include "scene_tables.h"

u32 Random16Far(void);

void Func_02000060(struct Actor *object) {
  u8 *timer = (u8 *)object + 98;
  u8 *state;

  if (*timer != 0) {
    (*timer)--;
    return;
  }
  *timer = ((Random16Far() * 80) >> 16) + 40;
  /* FAKEMATCH: the empty one-pass block ends the scheduling region, so the
   * timer is stored before the state's address is started as in the native
   * (differ 2 without it). */
  do {
  } while (0);
  state = (u8 *)object + 99;
  switch (*state) {
  case 0:
    object->angle = FACING_WEST;
    *state = 1;
    break;
  case 1:
    object->angle = FACING_NORTHWEST + FACING_STEP;
    *state = 0;
    break;
  case 2:
    object->angle = 0;
    *state = 3;
    break;
  case 3:
    object->angle = FACING_NORTH + FACING_STEP;
    *state = 2;
    break;
  }
}
