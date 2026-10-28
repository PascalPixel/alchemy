#include "types.h"
#include "sequence_parent_work.h"

extern u8 MsgShortcutSetL[], MsgShortcutSetR[];
extern u8 MsgAbilityNothing[];

void Text_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Text_GetMessageSizeFar(u8 *message, s32 *left, s32 *top,
                                     s32 *width, s32 *height);
void MessageArgument_PushFar(s32 item, s32 digits);
void *Unit_Get(s32 unit);
void Text_DrawStringAtOffsetFar(s32 text, s32 window, s32 x, s32 y);

/// Draws the two packed values of gSequenceParentWork (owner << 10 | ability), the
/// shortcuts' messages, into a window: a heading (one when both are set,
/// another otherwise), then for each its value through MessageArgument_PushFar, a
/// label and the owner's name, or an empty-slot message. Measures each
/// ability's name (result unused). Returns 1.
s32 Func_080fd55c(s32 window) {
  // German, Spanish, French and Italian skip the owner's name when the
  // ability's name is wider than 10, and Japanese chooses its row by whether
  // the name is wider than 11; all five stay on their scaffolds. Written so for
  // German, gSequenceParentWork's address and the id mask stay in registers the
  // native reloads at each use.
  s32 left;
  s32 top;
  s32 width;
  s32 height;
  u8 *name;

  if (gSequenceParentWork.packedEvents[0] != 0 && gSequenceParentWork.packedEvents[1] != 0)
    Text_DrawCharacterAtOffsetFar((s32)MsgShortcutSetR + 1, window, 0, -8);
  else
    Text_DrawCharacterAtOffsetFar((s32)MsgShortcutSetL - 2, window, 0, -8);

  name = &MsgAbilityNothing[gSequenceParentWork.packedEvents[0] & 0x3ff];
  Text_GetMessageSizeFar(name, &left, &top, &width, &height);
  if (gSequenceParentWork.packedEvents[0] != 0) {
    MessageArgument_PushFar(gSequenceParentWork.packedEvents[0] & 0x3ff, 4);
    Text_DrawCharacterAtOffsetFar((s32)MsgShortcutSetR + 4, window, 0, 0);
    Text_DrawStringAtOffsetFar(
        (s32)Unit_Get(gSequenceParentWork.packedEvents[0] >> 10), window, 80, 0);
  } else {
    Text_DrawCharacterAtOffsetFar((s32)MsgShortcutSetR + 2, window, 0, 0);
  }

  name = &MsgAbilityNothing[gSequenceParentWork.packedEvents[1] & 0x3ff];
  Text_GetMessageSizeFar(name, &left, &top, &width, &height);
  if (gSequenceParentWork.packedEvents[1] != 0) {
    MessageArgument_PushFar(gSequenceParentWork.packedEvents[1] & 0x3ff, 4);
    Text_DrawCharacterAtOffsetFar((s32)MsgShortcutSetR + 5, window, 0, 8);
    Text_DrawStringAtOffsetFar(
        (s32)Unit_Get(gSequenceParentWork.packedEvents[1] >> 10), window, 80, 8);
  } else {
    Text_DrawCharacterAtOffsetFar((s32)MsgShortcutSetR + 3, window, 0, 8);
  }
  return 1;
}
