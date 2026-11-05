// Measured under -Os, 5 November 2026: differ 51 (tla-en); notes below may predate it.
// Draft, differ 88 of 75: records which of eleven listed cells persons 10 and
// 11 stand on, as the two halves of the byte flag 0x210. Translated from its
// listing; names and structure still to do.
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

extern u8 gData_0200530c[];
struct Actor *Actor_GetById(s32 id);
s32 GameFlag_SetByte(s32 flag, s32 value);

s32 Func_02002974(void) {
  struct Actor *first = Actor_GetById(10);
  s32 firstX = first->x >> 19;
  s32 firstZ = first->z >> 19;
  u32 firstIndex = 0;
  u32 secondIndex = 0;
  struct Actor *second = Actor_GetById(11);
  s32 secondX = second->x >> 19;
  s32 secondZ = second->z >> 19;
  s8 i;

  for (i = 1; i <= 11; i++) {
    if (firstX == *(s16 *)(gData_0200530c + i * 4) && firstZ == *(s16 *)(gData_0200530c + i * 4 + 2))
      firstIndex = i;
    if (secondX == *(s16 *)(gData_0200530c + i * 4) && secondZ == *(s16 *)(gData_0200530c + i * 4 + 2))
      secondIndex = i;
  }
  return GameFlag_SetByte(0x210, (s32)((firstIndex << 24) | (secondIndex << 28)) >> 24);
}
