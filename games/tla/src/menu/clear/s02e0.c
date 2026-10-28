#include "types.h"
#include "sequence_parent_work.h"
#include "scene_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 GameFlag_Test(s32 flag);

s32 Func_020002e0(void) {
  struct SequenceParentWork *party;
  s32 mode;

  if (GameFlag_Test(324) == 0)
    return 0;
  party = &gSequenceParentWork;
  /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the party
   * state's address before it builds the field's offset (2 differ without it).
   */
  do {
  } while (0);
  if (FIELD_AT(party, s16, 0x25e) == 2)
    return 0;
  mode = party->scene ^ (s32)&SceneId_WorldMap;
  return -((u32)(-mode | mode) >> 31);
}
