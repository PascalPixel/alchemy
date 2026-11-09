#include "mars_lighthouse_star_room.h"
#include "version.h"

void Func_02000188(void) {
  struct Actor *object = Actor_Get(10);

#if !TLA_PATCH_1
  if (Event_GetFlag(0xa4b) == 0) {
#elif !TLA_PATCH_2
  if (Event_GetFlag(0x238) == 0 || Event_GetFlag(0xa4b) == 0) {
#else
  if (Event_GetFlag(0x238) == 0 && Event_GetFlag(0xa4b) == 0) {
#endif
    Event_PlaySound(0x23d);
    object->x = object->action << 16;
    object->z = *(s16 *)((u8 *)object + 0x66) << 16;
    if (Event_GetFlag(0xa4a) == 0) {
      object->scaleX = 0x9999;
      object->scaleY = 0x9999;
    }
  }
}

void Func_020001e8(void) {
  struct Actor *object = Actor_Get(10);

#if !TLA_PATCH_1
  if (Event_GetFlag(0xa4b) == 0) {
#elif !TLA_PATCH_2
  if (Event_GetFlag(0x238) == 0 || Event_GetFlag(0xa4b) == 0) {
#else
  if (Event_GetFlag(0x238) == 0 && Event_GetFlag(0xa4b) == 0) {
#endif
    Event_PlaySound(0x23d);
    Event_SetFlag(0x233);
    object->x = object->action << 16;
    object->z = *(s16 *)((u8 *)object + 0x66) << 16;
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    Event_SetFlag(0xa4a);
  }
}
