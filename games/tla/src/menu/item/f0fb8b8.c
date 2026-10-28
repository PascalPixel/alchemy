#include "types.h"

#include "item.h"
#include "edition.h"

// The headings are indented in English and Japanese; German, Spanish and
// Italian start all three at the window's left edge, French the uses heading
// only.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
#define ITEM_DETAILS_HEADING_X 0
#define ITEM_DETAILS_USES_X 0
#define ITEM_DETAILS_NUMBER_X 0
#elif LANGUAGE == LANGUAGE_FRENCH
#define ITEM_DETAILS_HEADING_X 8
#define ITEM_DETAILS_USES_X 0
#define ITEM_DETAILS_NUMBER_X 16
#elif LANGUAGE == LANGUAGE_JAPANESE
#define ITEM_DETAILS_HEADING_X 16
#define ITEM_DETAILS_USES_X 16
#define ITEM_DETAILS_NUMBER_X 16
#else
#define ITEM_DETAILS_HEADING_X 8
#define ITEM_DETAILS_USES_X 16
#define ITEM_DETAILS_NUMBER_X 16
#endif

// Japanese draws the values nearer the names, and a multiplier further left to
// leave room for its suffix.
#if LANGUAGE == LANGUAGE_JAPANESE
#define ITEM_DETAILS_VALUE_X 64
#define ITEM_DETAILS_MULTIPLIER_X 48
#else
#define ITEM_DETAILS_VALUE_X 72
#define ITEM_DETAILS_MULTIPLIER_X 72
#endif

extern u8 MsgEquipEffectHeading[], MsgStatLabel[], MsgDefenseLabel[],
    MsgEquipEffectName[];
extern u8 MsgItemCursed[], MsgBestowsPsynergy[], MsgUsesHeading[],
    MsgSingleUse[];
extern u8 MsgBrokenNotice[], MsgMightBreak[], MsgNumberHeading[];
extern u8 MsgRareItem[], MsgImportantItem[], MsgDetailsUnknown[];
#if LANGUAGE == LANGUAGE_JAPANESE
/// "ばい", times, after a multiplier.
extern u8 MsgTimesSuffix[];
#endif
extern u8 gMenu_PointString[];

void Text_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Text_DrawStringInWindowFar(u8 *text, s32 window, s32 x, s32 y);
void Text_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x,
                                  s32 y);
void Window_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y,
                                 s32 palette);
void MessageArgument_PushFar(s32 value, s32 kind);
void ItemMenu_DrawStat(s32 delta, s32 unused, s32 window, s32 x, s32 y);

/// Describes an item (an inventory entry) in window, one 8-pixel row a line:
/// its equip bonuses and effects, a curse, the Psynergy it bestows, its uses
/// and its count, or failing all of these whether it is rare or important.
/// count is the count to show, which only Japanese reads;
/// ItemMenu_DrawItemDetails passes -1, the entry's own. The twin of The Broken
/// Seal's ItemMenu_DrawItemDetails.
void Func_080fb8b8(s32 window, s32 item, s32 count) {
  struct ItemDefinition *def;
  s32 i;
  s32 found;
  s32 amount;
  s32 consumable;
  s8 row;

  row = 0;
  consumable = 0;
  def = Item_Get(item & 0x1ff);
  if (def->type != 0) {
    found = 0;
    if (def->primaryBonus == 0 && def->secondaryBonus == 0) {
      for (i = 0; i < 4; i++) {
        if (def->effects[i].kind != 0 || def->useType == 3) {
          found = 1;
          break;
        }
      }
    } else {
      found = 1;
    }
    if (found == 1) {
      Text_DrawCharacterAtOffsetFar((s32)MsgEquipEffectHeading, window,
                                      ITEM_DETAILS_HEADING_X, 0);
      row = 1;
    }
    if (def->primaryBonus != 0) {
      Text_DrawCharacterAtOffsetFar((s32)MsgStatLabel, window, 0, row * 8);
      amount = def->primaryBonus;
      ItemMenu_DrawStat(amount, 3, window, ITEM_DETAILS_VALUE_X, row * 8);
      row++;
    }
    if (def->secondaryBonus != 0) {
      Text_DrawCharacterAtOffsetFar((s32)MsgDefenseLabel, window, 0, row * 8);
      amount = def->secondaryBonus;
      ItemMenu_DrawStat(amount, 3, window, ITEM_DETAILS_VALUE_X, row * 8);
      row++;
    }
  }
  for (i = 0; i < 4; i++) {
    if (def->effects[i].kind == 0)
      continue;
    amount = def->effects[i].amount;
    switch (def->effects[i].kind) {
    case 0:
      break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 26:
      Text_DrawCharacterAtOffsetFar(
          def->effects[i].kind + (s32)MsgEquipEffectName, window, 0, row * 8);
      ItemMenu_DrawStat(amount, 3, window, ITEM_DETAILS_VALUE_X, row * 8);
      break;
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
      Window_SetTilemapEntryFar(
          window, (u8)((def->effects[i].kind - 15) % 4) + 1, 0, row, 2);
      Text_DrawCharacterAtOffsetFar(
          def->effects[i].kind + (s32)MsgEquipEffectName, window, 8, row * 8);
      ItemMenu_DrawStat(amount, 3, window, ITEM_DETAILS_VALUE_X, row * 8);
      break;
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
      Text_DrawCharacterAtOffsetFar(
          def->effects[i].kind + (s32)MsgEquipEffectName, window, 0, row * 8);
      if (amount > 9) {
        Text_DrawNumberInWindowFar(1, 1, window, ITEM_DETAILS_MULTIPLIER_X,
                                     row * 8);
        Text_DrawStringInWindowFar(gMenu_PointString, window,
                                     ITEM_DETAILS_MULTIPLIER_X + 8, row * 8);
        Text_DrawNumberInWindowFar(amount - 10, 1, window,
                                     ITEM_DETAILS_MULTIPLIER_X + 16, row * 8);
      } else {
        Text_DrawNumberInWindowFar(0, 1, window, ITEM_DETAILS_MULTIPLIER_X,
                                     row * 8);
        Text_DrawStringInWindowFar(gMenu_PointString, window,
                                     ITEM_DETAILS_MULTIPLIER_X + 8, row * 8);
        Text_DrawNumberInWindowFar(amount, 1, window,
                                     ITEM_DETAILS_MULTIPLIER_X + 16, row * 8);
      }
#if LANGUAGE == LANGUAGE_JAPANESE
      Text_DrawCharacterAtOffsetFar((s32)MsgTimesSuffix, window,
                                      ITEM_DETAILS_MULTIPLIER_X + 24, row * 8);
#endif
      break;
    case 23:
    case 25:
    case 27:
      Text_DrawCharacterAtOffsetFar(
          def->effects[i].kind + (s32)MsgEquipEffectName, window, 0, row * 8);
#if LANGUAGE == LANGUAGE_GERMAN
      // German continues these three names on a second line.
      row++;
      if (def->effects[i].kind == 23)
        Text_DrawCharacterAtOffsetFar((s32)MsgEquipEffectName + 28, window, 0,
                                        row * 8);
      else if (def->effects[i].kind == 25)
        Text_DrawCharacterAtOffsetFar((s32)MsgEquipEffectName + 29, window, 0,
                                        row * 8);
      else if (def->effects[i].kind == 27)
        Text_DrawCharacterAtOffsetFar((s32)MsgEquipEffectName + 30, window, 0,
                                        row * 8);
#endif
      break;
    }
    row++;
  }
  if (def->flags & 1) {
    Text_DrawCharacterAtOffsetFar((s32)MsgItemCursed, window, 0, row * 8);
    row++;
  }
  if (def->useType == 3) {
    Text_DrawCharacterAtOffsetFar((s32)MsgBestowsPsynergy, window, 0,
                                    row * 8);
    consumable = 1;
    row++;
  }
  if (def->useType != 4 && def->useType != 0) {
    if (!consumable) {
      Text_DrawCharacterAtOffsetFar((s32)MsgUsesHeading, window,
                                      ITEM_DETAILS_USES_X, row * 8);
      row++;
    }
    switch (def->useType) {
    case 0:
      break;
    case 1:
      Text_DrawCharacterAtOffsetFar((s32)MsgSingleUse, window, 0, row * 8);
      row++;
      break;
    case 2: {
      s32 m;

      if (item & 0x400) {
        m = (s32)MsgBrokenNotice;
        Text_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
#if LANGUAGE == LANGUAGE_GERMAN
        // German ends the sentence on a third line, six messages on.
        Text_DrawCharacterAtOffsetFar(m + 1, window, 0, row * 8);
        row++;
        m += 6;
        Text_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
#else
        m++;
        Text_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
#endif
      } else {
        m = (s32)MsgMightBreak;
        Text_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
#if LANGUAGE == LANGUAGE_GERMAN
        Text_DrawCharacterAtOffsetFar(m + 1, window, 0, row * 8);
        row++;
        m += 7;
        Text_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
#else
        m++;
        Text_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
#endif
      }
      break;
    }
    }
  }
  if (def->flags & 16) {
    if (row != 0)
      row++;
    i = (s32)MsgNumberHeading;
    Text_DrawCharacterAtOffsetFar(i, window, ITEM_DETAILS_NUMBER_X, row * 8);
    row++;
#if LANGUAGE == LANGUAGE_JAPANESE
    // Japanese draws the count itself: the entry's own for -1, "none" for 0.
    if (count == -1) {
      amount = (item & 0xf800) / 2048;
      Text_DrawNumberInWindowFar(amount + 1, 2, window, 0, row * 8);
      Text_DrawCharacterAtOffsetFar(i + 1, window, 16, row * 8);
      row++;
    } else if (count == 0) {
      Text_DrawCharacterAtOffsetFar(i + 8, window, 0, row * 8);
      row++;
    } else {
      amount = count;
      Text_DrawNumberInWindowFar(amount, 3, window, 0, row * 8);
      Text_DrawCharacterAtOffsetFar(i + 1, window, 24, row * 8);
      row++;
    }
#else
    amount = (item & 0xf800) / 2048;
    MessageArgument_PushFar(amount + 1, 5);
    Text_DrawCharacterAtOffsetFar(i + 1, window, 0, row * 8);
    row++;
#endif
  }
  found = 0;
  if (row == 0) {
    if (def->flags & 4) {
      Text_DrawCharacterAtOffsetFar((s32)MsgRareItem, window, 0, 0);
      found = 1;
    }
    if (!found) {
      if (def->flags & 8) {
        Text_DrawCharacterAtOffsetFar((s32)MsgImportantItem, window, 0, 0);
        found = 1;
      }
      if (!found)
        Text_DrawCharacterAtOffsetFar((s32)MsgDetailsUnknown, window, 0, 0);
    }
  }
}
