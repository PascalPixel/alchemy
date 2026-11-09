#include "types.h"
#include "actor.h"
#include "version.h"
#include "event_inline.h"


// The Japanese edition drops an object with no cue; the later editions play
// cue 181 as it drops. The first drop goes through the call layer.
#if !TLA_PATCH_1
void Func_02000100(s32 id);
#define DROP_OBJECT_CALLED(id) Func_02000100(id)
#define DROP_OBJECT(id) Func_02000100(id)
#else
void Func_02000100(s32 id, s32 cue);
#define DROP_OBJECT_CALLED(id) Func_02000100(id, 181)
#define DROP_OBJECT(id) Func_02000100(id, 181)
#endif

/// Copies cells by where the object with the given id has moved, its position
/// counted in 8-pixel steps: object 16, at x 11 or less while flag 0x920 is
/// clear, drops and sets the flag; object 19, while flag 0x921 is clear, copies
/// cells at x 77, drops at (79, 12) and at x 81 sets the flag.
void Func_02000164(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  struct Actor *p;

  MapGrid_CopyCellAttributes(8, 90, 1, 1, 8, 91);
  if (id == 16 && Event_GetFlag(0x920) == 0 &&
      (object->x >> 19) <= 11) {
    DROP_OBJECT_CALLED(16);
    Event_SetFlag(0x920);
    MapGrid_CopyCellAttributes(6, 27, 1, 1, 5, 27);
  }
  if (id == 19 && Event_GetFlag(0x921) == 0) {
    if ((object->x >> 19) == 77)
      MapGrid_CopyCellAttributes(33, 102, 1, 2, 37, 102);
    /* FAKEMATCH(temporary): the object read again through p for the last two
     * tests; as object throughout, 22 of 109 instructions differ in tla-en. */
    p = object;
    if ((p->x >> 19) == 79 && (p->y >> 19) == 12) {
      DROP_OBJECT(19);
      MapGrid_CopyCellAttributes(38, 46, 1, 1, 39, 46);
    }
    if ((p->x >> 19) == 81) {
      Event_SetFlag(0x921);
      {
        /* FAKEMATCH(temporary): the row in its own register; as a plain 46,
         * 2 of 109 instructions differ in tla-en. */
        s32 row = 46;

        MapGrid_CopyCellAttributes(41, row, 1, 1, 39, 46);
      }
      MapGrid_CopyCellAttributes(27, 44, 1, 1, 40, 46);
    }
  }
}
