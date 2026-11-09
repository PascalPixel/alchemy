#include "types.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_020002e0(void) {
  struct MainRoutineWork *party;
  s32 mode;

  if (Event_GetFlag(324) == 0)
    return 0;
  party = &gMainRoutineWork;

  if (FIELD_AT(party, s16, 0x25e) == 2)
    return 0;
  mode = party->scene ^ (s32)&SceneId_WorldMap;
  return -((u32)(-mode | mode) >> 31);
}
