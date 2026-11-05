/// @file
/// Records which of eleven listed cells persons 10 and 11 stand on, as the two halves of the byte flag 0x210.
#include "iwram_call.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern const s16 gData_0200530c[][2];
struct Actor *Actor_GetById(s32 id);
s32 GameFlag_SetByte(s32 flag, s32 value);

s32 Func_02002974(void) {
  struct Actor *first = Actor_GetById(10);
  s8 firstIndex = 0;
  s32 firstX = first->x >> 19;
  s8 secondIndex = 0;
  s32 firstZ = first->z >> 19;
  struct Actor *second = Actor_GetById(11);
  s32 secondX = second->x >> 19;
  s32 secondZ = second->z >> 19;
  s8 i;

  for (i = 1; i <= 11; i++) {
    if (firstX == gData_0200530c[i][0] && firstZ == gData_0200530c[i][1])
      firstIndex = i;
    if (secondX == gData_0200530c[i][0] && secondZ == gData_0200530c[i][1])
      secondIndex = i;
  }
  {
    u32 high = secondIndex << 28;
    return GameFlag_SetByte(0x210, (s32)((firstIndex << 24) | high) >> 24);
  }
}
