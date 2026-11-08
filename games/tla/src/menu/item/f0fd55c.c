#include "types.h"
#include "main_routine_work.h"
#include "edition.h"
#include "version.h"

#if LANGUAGE == LANGUAGE_JAPANESE
// Japanese drops the colon after "L" and "R" and draws the ability's name 8
// pixels further left when that name is wider than this.
#define COLON_NAME_WIDTH_LIMIT 11
#elif TLA_PATCH_2
// German, Spanish, French and Italian leave out the owner's name when the
// ability's name is wider than this.
#define OWNER_NAME_WIDTH_LIMIT 10
#endif

extern u8 MsgShortcutSetL[], MsgShortcutSetR[];
extern u8 MsgAbilityNothing[];
#if LANGUAGE == LANGUAGE_JAPANESE
extern const u8 gMenu_ShortcutLString[];
extern const u8 gMenu_ShortcutLColonString[];
extern const u8 gMenu_ShortcutRString[];
extern const u8 gMenu_ShortcutRColonString[];
#endif

void Text_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Text_GetMessageSizeFar(u8 *message, s32 *left, s32 *top, s32 *width,
                            s32 *height);
#if LANGUAGE == LANGUAGE_JAPANESE
void Text_DrawStringInWindowFar(const u8 *text, s32 window, s32 x, s32 y);
#else
void MessageArgument_PushFar(s32 item, s32 digits);
#endif
void *Unit_Get(s32 unit);
void Text_DrawStringAtOffsetFar(s32 text, s32 window, s32 x, s32 y);

/// Draws the two packed values of gMainRoutineWork (owner << 10 | ability), the
/// shortcuts' messages, into a window: a heading (one when both are set,
/// another otherwise), then for each its ability's name and the owner's name,
/// or an empty-slot message. Measures each ability's name first. The
/// international editions draw the name through MessageArgument_PushFar and a
/// label message; German, Spanish, French and Italian leave out the owner's
/// name when the ability's name is wide. Japanese draws "L" or "L:" (and "R"
/// or "R:") itself, then the name, dropping the colon for a wide name. Returns
/// 1.
#if LANGUAGE != LANGUAGE_JAPANESE
s32 Func_080fd55c(s32 window) {
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
  if (width < 0 || width > OWNER_NAME_WIDTH_LIMIT)
    wide = 1;
  else
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
  if (width < 0 || width > OWNER_NAME_WIDTH_LIMIT)
    wide = 1;
  else
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
#else
s32 Func_080fd55c(s32 window) {
  s32 left;
  s32 top;
  s32 width;
  s32 height;
  u8 *name;

  if (gMainRoutineWork.packedEvents[0] != 0 &&
      gMainRoutineWork.packedEvents[1] != 0)
    Text_DrawCharacterAtOffsetFar((s32)MsgShortcutSetR + 1, window, 0, -8);
  else
    Text_DrawCharacterAtOffsetFar((s32)MsgShortcutSetL - 2, window, 0, -8);

  name = &MsgAbilityNothing[gMainRoutineWork.packedEvents[0] & 0x3ff];
  Text_GetMessageSizeFar(name, &left, &top, &width, &height);
  if (gMainRoutineWork.packedEvents[0] != 0) {
    if (width < 0 || width > COLON_NAME_WIDTH_LIMIT) {
      Text_DrawStringInWindowFar(gMenu_ShortcutLString, window, 0, 0);
      Text_DrawCharacterAtOffsetFar((s32)name, window, 8, 0);
    } else {
      Text_DrawStringInWindowFar(gMenu_ShortcutLColonString, window, 0, 0);
      Text_DrawCharacterAtOffsetFar((s32)name, window, 16, 0);
    }
    Text_DrawStringAtOffsetFar(
        (s32)Unit_Get(gMainRoutineWork.packedEvents[0] >> 10), window, 80, 0);
  } else {
    Text_DrawStringInWindowFar(gMenu_ShortcutLColonString, window, 0, 0);
    Text_DrawCharacterAtOffsetFar((s32)MsgShortcutSetR + 2, window, 24, 0);
  }

  name = &MsgAbilityNothing[gMainRoutineWork.packedEvents[1] & 0x3ff];
  Text_GetMessageSizeFar(name, &left, &top, &width, &height);
  if (gMainRoutineWork.packedEvents[1] != 0) {
    if (width < 0 || width > COLON_NAME_WIDTH_LIMIT) {
      Text_DrawStringInWindowFar(gMenu_ShortcutRString, window, 0, 8);
      Text_DrawCharacterAtOffsetFar((s32)name, window, 8, 8);
    } else {
      Text_DrawStringInWindowFar(gMenu_ShortcutRColonString, window, 0, 8);
      Text_DrawCharacterAtOffsetFar((s32)name, window, 16, 8);
    }
    Text_DrawStringAtOffsetFar(
        (s32)Unit_Get(gMainRoutineWork.packedEvents[1] >> 10), window, 80, 8);
  } else {
    Text_DrawStringInWindowFar(gMenu_ShortcutRColonString, window, 0, 8);
    Text_DrawCharacterAtOffsetFar((s32)MsgShortcutSetR + 3, window, 24, 8);
  }
  return 1;
}
#endif
