#include "types.h"
#include "main_routine_work.h"
#include "version.h"

#if TLA_PATCH_2
// German, Spanish, French and Italian leave out the owner's name when the
// ability's name is wider than this.
#define OWNER_NAME_WIDTH_LIMIT 10
#endif

extern u8 MsgShortcutSetL[], MsgShortcutSetR[];
extern u8 MsgAbilityNothing[];

void Text_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Text_GetMessageSizeFar(u8 *message, s32 *left, s32 *top, s32 *width,
                            s32 *height);
void MessageArgument_PushFar(s32 item, s32 digits);
void *Unit_Get(s32 unit);
void Text_DrawStringAtOffsetFar(s32 text, s32 window, s32 x, s32 y);

/// Draws the two packed values of gMainRoutineWork (owner << 10 | ability), the
/// shortcuts' messages, into a window: a heading (one when both are set,
/// another otherwise), then for each its value through MessageArgument_PushFar,
/// a label and the owner's name, or an empty-slot message. Measures each
/// ability's name: German, Spanish, French and Italian leave out the owner's
/// name when that name is wide. Returns 1.
s32 Func_080fd55c(s32 window) {
  // Japanese chooses its row by whether the name is wider than 11 and stays on
  // its scaffold. German, Spanish, French and Italian stay on theirs too:
  // differ 88 of 165 in tla-de, allocation only. Their native keeps &height in
  // r8 and rebuilds the slot's address and the id mask after each measurement;
  // this form keeps the mask in r8, the slot's address in r7 across the call,
  // and spills &height (a 24-byte frame for 20). Neither u8 nor u32 for wide or
  // width, a slot pointer, nor 8 minutes of permute did better.
  s32 left;
  s32 top;
  s32 width;
  s32 height;
  u8 *name;
#if TLA_PATCH_2
  s32 wide;
#endif

  if (gMainRoutineWork.packedEvents[0] != 0 &&
      gMainRoutineWork.packedEvents[1] != 0)
    Text_DrawCharacterAtOffsetFar((s32)MsgShortcutSetR + 1, window, 0, -8);
  else
    Text_DrawCharacterAtOffsetFar((s32)MsgShortcutSetL - 2, window, 0, -8);

  name = &MsgAbilityNothing[gMainRoutineWork.packedEvents[0] & 0x3ff];
  Text_GetMessageSizeFar(name, &left, &top, &width, &height);
#if TLA_PATCH_2
  wide = 1;
  if ((u32)width <= OWNER_NAME_WIDTH_LIMIT)
    wide = 0;
#endif
  if (gMainRoutineWork.packedEvents[0] != 0) {
    MessageArgument_PushFar(gMainRoutineWork.packedEvents[0] & 0x3ff, 4);
    Text_DrawCharacterAtOffsetFar((s32)MsgShortcutSetR + 4, window, 0, 0);
#if TLA_PATCH_2
    if (!wide)
#endif
      Text_DrawStringAtOffsetFar(
          (s32)Unit_Get(gMainRoutineWork.packedEvents[0] >> 10), window, 80, 0);
  } else {
    Text_DrawCharacterAtOffsetFar((s32)MsgShortcutSetR + 2, window, 0, 0);
  }

  name = &MsgAbilityNothing[gMainRoutineWork.packedEvents[1] & 0x3ff];
  Text_GetMessageSizeFar(name, &left, &top, &width, &height);
#if TLA_PATCH_2
  wide = 1;
  if ((u32)width <= OWNER_NAME_WIDTH_LIMIT)
    wide = 0;
#endif
  if (gMainRoutineWork.packedEvents[1] != 0) {
    MessageArgument_PushFar(gMainRoutineWork.packedEvents[1] & 0x3ff, 4);
    Text_DrawCharacterAtOffsetFar((s32)MsgShortcutSetR + 5, window, 0, 8);
#if TLA_PATCH_2
    if (!wide)
#endif
      Text_DrawStringAtOffsetFar(
          (s32)Unit_Get(gMainRoutineWork.packedEvents[1] >> 10), window, 80, 8);
  } else {
    Text_DrawCharacterAtOffsetFar((s32)MsgShortcutSetR + 3, window, 0, 8);
  }
  return 1;
}
