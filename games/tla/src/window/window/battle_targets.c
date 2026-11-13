/// @file
/// Choosing a battle target.
// Part of window.c.

#include "../window_internal.h"
#include "flag_call.h"
#include "gamepak/far_call/battle.h"
#include "item_icon.h"
#include "module/tla.h"
#include "module/window/window.h"

// Widths in tiles that fit each language's text: the "Set Djinni" window
// (MsgSetDjinni), the condition list, and the Mystery names' copy, which
// Japanese decodes raw where the other languages copy it expanded.
#if LANGUAGE == LANGUAGE_JAPANESE
#define TARGET_SET_DJINNI_WIDTH 8
#define TARGET_CONDITION_LIST_WIDTH 14
#define TARGET_MYSTERY_NAME_COPY Message_DecodeRaw
#define TARGET_MYSTERY_NAME_CAPACITY 10
#else
#define TARGET_SET_DJINNI_WIDTH 9
#define TARGET_CONDITION_LIST_WIDTH 16
#define TARGET_MYSTERY_NAME_COPY Message_CopyExpanded
#define TARGET_MYSTERY_NAME_CAPACITY 14
#endif

#define NAVIGATION ((struct TargetNavigation *)Ram_HeapSlots->unke4[0])

#if TLA_PATCH_2
extern const u8 gOwnerStatus_KpString[];
extern const u8 gOwnerStatus_PvString[];
s32 System_GetLanguage(void);
#endif

/* VIEW(menus): routine VramCache_Reload as s32 VramCache_Reload(s32 entry,
 * const void *tiles); resource.h has s32 VramCache_Reload(s32 slot, s32
 * source); tried as the definition's s32 VramCache_Reload(s32 no, s32 src),
 * BattleTarget_RunSelection moves: 1 of 1647 instructions in tla-ja. */
extern s32
VramCache_Reload__menus(s32 entry,
                        const void *tiles) __asm__("VramCache_Reload");
/* VIEW(menus): routine Unit_Get as struct BattleUnit *Unit_Get(s32 unit); the
 * definition has s32 BattleParty_ChooseReserve(s32 slot) { u16 owners[6]; u16
 * selected[2]; struct BattleWork *battle; s32 count; count =
 * BattleParty_PrepareReserveOwners...; not tried: Unit_Get's declaration holds
 * a FAKEMATCH tag. */
extern struct BattleUnit *Unit_Get__menus(s32 unit) __asm__("Unit_Get");
/* VIEW(menus): routine Window_Create as struct TargetWindow *Window_Create(s32
 * x, s32 y, s32 width, s32 height, s32 flags); module/window/window.h has
 * struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
 * not tried: Window_Create's declaration holds a FAKEMATCH tag. */
extern struct TargetWindow *
Window_Create__menus(s32 x, s32 y, s32 width, s32 height,
                     s32 flags) __asm__("Window_Create");
/* VIEW(menus): routine Oam_AddSpriteAtDepth as s32 Oam_AddSpriteAtDepth(void
 * *sprite, s32 depth); task.h has void Oam_AddSpriteAtDepth(void *entry, s32
 * priority); tried as the definition's void Oam_AddSpriteAtDepth(void *entry,
 * s32 slot), BattleTarget_RunSelection moves: 2 of 1647 instructions in tla-ja.
 */
extern s32 Oam_AddSpriteAtDepth__menus(void *sprite, s32 depth) __asm__(
    "Oam_AddSpriteAtDepth"); // Returns void; see the tag.
/* VIEW(menus): routine OamMatrix_AllocFromTransform as s32
 * OamMatrix_AllocFromTransform(struct TargetAffine *affine); affine.h has s32
 * OamMatrix_AllocFromTransform(struct AffineTransform *source); not tried:
 * OamMatrix_AllocFromTransform's bound declaration was not found. */
extern s32 AffineMatrix_BuildForEffect__menus(
    struct TargetAffine *affine) __asm__("OamMatrix_AllocFromTransform");
/* VIEW(menus): routine Window_Close as void Window_Close(void *window, s32
 * mode); uiflow.h has void Window_Close(struct Window *window, s32 release);
 * not tried: Window_Close's declaration holds a FAKEMATCH tag. */
extern void Window_Close__menus(void *window, s32 mode) __asm__("Window_Close");
/* VIEW(menus): routine Text_DrawStringAtOffset as void
 * Text_DrawStringAtOffset(const u8 *text, struct TargetWindow *window, s32 x,
 * s32 y); text_render.h has void Text_DrawStringAtOffset(u8 *text, struct
 * RenderInput *work, s32 offsetX, s32 offsetY); tried as the definition's void
 * Text_DrawStringAtOffset(u8 *text, struct RenderInput *work, s32 offsetX, s32
 * offsetY), which the object already declares, BattleTarget_RunSelection moves:
 * 8 of 1647 instructions in tla-ja. */
extern void
Text_DrawStringAtOffset__menus(const u8 *text, struct TargetWindow *window,
                               s32 x, s32 y) __asm__("Text_DrawStringAtOffset");
/* VIEW(menus): routine Text_DrawNumberInWindow as void
 * Text_DrawNumberInWindow(s32 value, s32 digits, struct TargetWindow *window,
 * s32 x, s32 y); module/window/window.h has void Text_DrawNumberInWindow(s32
 * value, s32 width, s32 output, s32 x, s32 y); not tried:
 * Text_DrawNumberInWindow's declaration holds a FAKEMATCH tag. */
extern void
Text_DrawNumberInWindow__menus(s32 value, s32 digits,
                               struct TargetWindow *window, s32 x,
                               s32 y) __asm__("Text_DrawNumberInWindow");
/* VIEW(menus): routine Text_DrawMessageAsTiles as void
 * Text_DrawMessageAsTiles(s32 message, struct TargetWindow *window, s32 x, s32
 * y); text_render.h has void Text_DrawMessageAsTiles(s32 character, struct
 * RenderInput *work, u32 offsetX, u32 offsetY); not tried:
 * Text_DrawMessageAsTiles's bound declaration was not found. */
extern void
Text_DrawCharacterAtOffset__menus(s32 message, struct TargetWindow *window,
                                  s32 x,
                                  s32 y) __asm__("Text_DrawMessageAsTiles");
/* VIEW(menus): routine Message_CopyExpanded as void Message_CopyExpanded(s32
 * message, u16 *text, s32 capacity); module/window/window.h has s32
 * Message_CopyExpanded(s32 key, u16 *destination, u32 capacity); not tried:
 * Message_CopyExpanded's declaration holds a FAKEMATCH tag. */
extern void
Message_CopyExpanded__menus(s32 message, u16 *text,
                            s32 capacity) __asm__("Message_CopyExpanded");
/* VIEW(menus): routine Text_ClearCanvas as void Text_ClearCanvas(void);
 * graphics.h has s32 Text_ClearCanvas(void); not tried: Text_ClearCanvas's
 * bound declaration was not found. */
extern void Vram_ClearWindowBlock__menus(void) __asm__("Text_ClearCanvas");
/* VIEW(menus): routine Text_DrawWideStringAtOffset as void
 * Text_DrawWideStringAtOffset(u16 *text, struct TargetWindow *window, s32 x,
 * s32 y); window/window has void Text_DrawWideStringAtOffset(u16 *text, struct
 * Window *window, s32 x, s32 y) {; not tried: Text_DrawWideStringAtOffset's
 * declaration holds a FAKEMATCH tag. */
void Text_DrawWideStringAtOffset__menus(
    u16 *text, struct TargetWindow *window, s32 x,
    s32 y) __asm__("Text_DrawWideStringAtOffset");
#define Text_DrawWideStringAtOffset Text_DrawWideStringAtOffset__menus

// The help window's column beside the aimed member, kept on screen.
#define TARGET_HELP_COLUMN(back, width, last)                                  \
  {                                                                            \
    s32 anchor = selectedPosition.x / 8;                                       \
    helpColumn = anchor - (back);                                              \
    if (anchor + (width) > 29)                                                 \
      helpColumn = (last);                                                     \
  }

// The window listing the aimed member's conditions, count lines tall. The
// European editions share one image and widen it at run time for Spanish.
#if TLA_PATCH_2
#define TARGET_CONDITION_LIST_WINDOW()                                         \
  {                                                                            \
    s32 anchor = selectedPosition.x / 8;                                       \
    helpColumn = anchor - 7;                                                   \
    if (System_GetLanguage() == CARTRIDGE_LANGUAGE_SPANISH) {                  \
      if (anchor + 10 > 29)                                                    \
        helpColumn = 13;                                                       \
      conditionWindow = Window_Create(helpColumn, helpRow, 17, count + 2, 6);  \
    } else {                                                                   \
      if (anchor + 9 > 29)                                                     \
        helpColumn = 14;                                                       \
      conditionWindow = Window_Create(helpColumn, helpRow, 16, count + 2, 6);  \
    }                                                                          \
  }
#else
#define TARGET_CONDITION_LIST_WINDOW()                                         \
  {                                                                            \
    TARGET_HELP_COLUMN(7, TARGET_CONDITION_LIST_WIDTH - 7,                     \
                       30 - TARGET_CONDITION_LIST_WIDTH);                      \
    conditionWindow = Window_Create(                                           \
        helpColumn, helpRow, TARGET_CONDITION_LIST_WIDTH, count + 2, 6);       \
  }
#endif

// The cursors' bob, a sine turn every 16 frames scaled down to 2 pixels.
// The signed rounding is spelled out: as bob / 32768, 344 of 1647 differ.
#define TARGET_BOB(y)                                                          \
  {                                                                            \
    s32 bob = Trig_Sin(gFrameCount << 12);                                     \
                                                                               \
    if (bob < 0)                                                               \
      bob += 32767;                                                            \
    (y) += bob >> 15;                                                          \
  }

s32 BattleTarget_RunSelection(s32 selection, s32 mode, u32 spread, u32 kind) {
  /* FAKEMATCH(signature): Oam_AddSpriteAtDepth declared returning s32, though
   * it returns nothing, so the call sets r0 and the marker's last attribute
   * byte is stored before its address goes to r0; as void, 2 of 1647
   * instructions differ in tla-en. */
  u16 candidateIds[8];
  struct TargetSprite cursorSprites[6];
  struct TargetTrack cursorTracks[6];
  struct TargetPoint actorPosition;
  u16 spreadActorIds[14];
  u8 spreadTrackIndices[8];
  struct TargetPoint selectedPosition;
  u16 targetName[15];
  struct TargetPoint namePosition;
  struct TargetPoint markerPosition;
  struct TargetAffine effect;
  struct BattleUnit *unit;
  struct TargetBattle *battle;
  struct TargetWindow *window;
  struct TargetWindow *conditionWindow;
  s32 targetIndex;
  s32 targetCount;
  s32 candidateCount;
  s32 redraw;
  s32 cursorSlot;
  s32 drawCursors;
  s32 matrix;
  s32 found;
  s32 helpColumn;
  s32 helpRow;
  s32 count;
#if TLA_PATCH_2
  s32 *countAddress;
#endif
  s32 i;
  s32 j;
  s32 n;
  s32 cursorX;
  s32 cursorY;
  s32 pressed;
  s32 y;
  s32 width;
  s32 repeated;
  s32 result;
  struct TargetTrack *track;
  struct TargetSprite *marker;
  struct TargetSprite *firstSprite;
  struct BattleUnit *target;

  battle = Ram_HeapSlots->battleWork;
  targetCount = 0;
  redraw = 0xffff;
  cursorSlot = VramCache_AllocEntry(256);
  /* FAKEMATCH(dead): a null computed from selection, which the compiler cannot
   * fold to a constant, keeps conditionWindow in a register; as
   * conditionWindow = NULL, 492 of 1647 instructions differ in tla-en. */
  conditionWindow = (struct TargetWindow *)((u32)(u16)selection >> 16);
  if (spread == 0)
    spread = 1;
  if (mode == 2 || mode == 4)
    NAVIGATION->targetViewOffset = -2;
  else
    NAVIGATION->targetViewOffset = 16;
  for (i = 0; i < 6; i++)
    cursorTracks[i].flags = 0;
  targetIndex = -1;
  if (mode == 2) {
    for (i = 0; i <= 5 && battle->partyUnits[i] != 255; i++) {
      candidateIds[targetCount] = battle->partyUnits[i];
      targetCount++;
    }
  } else if (mode == 4) {
    candidateIds[0] = selection;
    targetCount = 1;
  } else {
    for (i = 0; i <= 5 && battle->enemyIds[i] != 255; i++) {
      candidateIds[targetCount] = battle->enemyIds[i];
      targetCount++;
    }
  }
  candidateIds[targetCount] = 255;
  candidateCount = targetCount;
  if (mode == 2) {
    if (spread != 255 && kind != 0) {
      found = 0;
      for (j = 0; j < targetCount; j++) {
        struct BattleUnit *candidate;

        if (candidateIds[j] == 254)
          continue;
        candidate = Unit_Get__menus(candidateIds[j]);
        switch (kind) {
        case 5:
          if (candidate->hp == 0) {
            selection = candidateIds[j];
            found = 1;
          }
          break;
        case 3:
          if (candidate->poison != 0)
            found = 1;
          break;
        case 4:
          if (candidate->delusion != 0 || candidate->stun != 0 ||
              candidate->sleep != 0 || candidate->psySeal != 0 ||
              candidate->grimReaper != 0)
            found = 1;
          break;
        case 6:
          if (candidate->delusion != 0 || candidate->stun != 0 ||
              candidate->sleep != 0 || candidate->psySeal != 0 ||
              candidate->grimReaper != 0 || candidate->poison != 0 ||
              candidate->haunt != 0)
            found = 1;
          break;
        case 8:
          if (NAVIGATION->lastTarget == candidateIds[j])
            found = 1;
          break;
        }
        if (found != 0) {
          selection = candidateIds[j];
          break;
        }
      }
    }
    for (j = 0; j < targetCount; j++) {
      if (candidateIds[j] == selection)
        break;
    }
    if (j != targetCount)
      targetIndex = j;
  }
  if (targetIndex < 0)
    targetIndex = (targetCount - 1) / 2;
  while (candidateIds[targetIndex] == 254 ||
         (GameFlag_Get(0x16c) && mode == 1 &&
          Unit_Get__menus(candidateIds[targetIndex])->hp == 0)) {
    targetIndex = targetIndex + targetCount - 1;
    targetIndex %= targetCount;
  }
  if (mode != 2) {
    BattleMotion_ProjectConditionalPositionFar(selection, &actorPosition);
    cursorTracks[0].flags = 8;
    cursorTracks[0].x = actorPosition.x;
    cursorTracks[0].y = 128;
  }
#if !TLA_PATCH_2
  /* FAKEMATCH(barrier): an empty asm clobbering r8 and r10 once selection and
   * mode are dead, so conditionWindow takes r9, selection r10 and mode r8; as
   * no asm, 96 of 1647 instructions differ in tla-en. */
  __asm__("" : : : "r8", "r10");
#endif
  window = Window_Create__menus(0, 12, 30, 4, 74);

  for (;;) {
    // A once-only block: its end keeps the tile reload after the sprite's
    // words.
    do {
      /* FAKEMATCH(temporary): firstSprite holds &cursorSprites[0]; as
       * cursorSprites used directly, 399 of 1647 instructions differ in tla-en.
       */
      firstSprite = cursorSprites;
      drawCursors = 0;
      BattleMotion_ProjectConditionalPositionFar(candidateIds[targetIndex],
                                                 &selectedPosition);
      ((u32 *)firstSprite)[1] = 0x40002000;
      ((u32 *)firstSprite)[2] = 0;
    } while (0);
    firstSprite->tile =
        VramCache_Reload__menus(cursorSlot, ((gFrameCount >> 2) & 31) * 256 +
                                                gMenu_AnimatedCursorTiles);
    TARGET_BOB(selectedPosition.y);
    y = selectedPosition.y;
    if (cursorTracks[0].flags & 1) {
      cursorX = (selectedPosition.x + cursorTracks[0].x) / 2;
      cursorY = (y + cursorTracks[0].y) / 2;
      if ((selectedPosition.x - cursorX < 0
               ? -(selectedPosition.x - cursorX)
               : selectedPosition.x - cursorX) <= 7)
        drawCursors = 1;
      selectedPosition.x = cursorX;
      cursorTracks[0].x = cursorX;
      selectedPosition.y = cursorY;
      cursorTracks[0].y = cursorY;
    } else if (cursorTracks[0].flags <= 3) {
      cursorTracks[0].x = selectedPosition.x;
      selectedPosition.y = y;
      cursorTracks[0].y = y;
      cursorTracks[0].flags = 1;
    } else {
      selectedPosition.x = cursorTracks[0].x;
      selectedPosition.y = cursorTracks[0].y;
      cursorTracks[0].flags -= 4;
      if (cursorTracks[0].flags <= 3)
        cursorTracks[0].flags = 1;
    }
    firstSprite->x = selectedPosition.x - 8;
    firstSprite->y = selectedPosition.y - 16;
    Oam_AddSpriteAtDepth__menus(firstSprite, 240);
    if (spread == 255) {
      effect.scaleX = 256;
      effect.scaleY = 256;
    } else {
      effect.scaleX = 176;
      effect.scaleY = 176;
    }
    effect.angle = 0;
    matrix = AffineMatrix_BuildForEffect__menus(&effect);

    if (redraw & 1) {
      targetCount = 0;
      for (i = 0; i < 6; i++)
        cursorTracks[i].flags &= ~2;
      for (i = 0; (u32)i < spread; i++) {
        n = targetIndex + i;
        if (n < candidateCount && candidateIds[n] != 254) {
          spreadActorIds[targetCount] = candidateIds[n];
          cursorTracks[i].flags |= 2;
          if (cursorTracks[i].index != i) {
            cursorTracks[i].flags &= ~1;
            cursorTracks[i].index = i;
          }
          spreadTrackIndices[targetCount] = i;
          targetCount++;
        }
        if (i != 0 && (n = targetIndex - i) >= 0 && candidateIds[n] != 254) {
          spreadActorIds[targetCount] = candidateIds[n];
          cursorTracks[6 - i].flags |= 2;
          if (cursorTracks[6 - i].index != -i) {
            cursorTracks[6 - i].flags &= ~1;
            cursorTracks[6 - i].index = -i;
          }
          spreadTrackIndices[targetCount] = 6 - i;
          targetCount++;
        }
      }
      for (i = 0; i < 6; i++) {
        if ((cursorTracks[i].flags & 2) == 0)
          cursorTracks[i].index = 6;
      }
      spreadActorIds[targetCount] = 255;
      BattleHighlight_SetModesFar(spreadActorIds, 1);
      // The (u32) compares the stored id unsigned, as the bytes do.
      if ((u32)(NAVIGATION->lastTarget = candidateIds[targetIndex]) <= 7) {
        if (spread != 255 && kind != 0) {
          unit = Unit_Get__menus(candidateIds[targetIndex]);
          BattleMotion_ProjectConditionalPositionFar(candidateIds[targetIndex],
                                                     &selectedPosition);
          if (conditionWindow != NULL)
            Window_Close__menus(conditionWindow, 1);
          switch (kind) {
          case 7:
#if TLA_PATCH_2
          {
            s32 anchor = selectedPosition.x / 8;

            helpColumn = anchor - 4;
            if (System_GetLanguage() == CARTRIDGE_LANGUAGE_ITALIAN) {
              if (anchor + 6 > 29)
                helpColumn = 20;
              conditionWindow = Window_Create__menus(helpColumn, 8, 10, 3, 6);
            } else if (System_GetLanguage() == CARTRIDGE_LANGUAGE_SPANISH) {
              if (anchor + 7 > 29)
                helpColumn = 19;
              conditionWindow = Window_Create__menus(helpColumn, 8, 11, 3, 6);
            } else {
              if (anchor + 5 > 29)
                helpColumn = 21;
              conditionWindow = Window_Create__menus(helpColumn, 8, 9, 3, 6);
            }
          }
#else
            TARGET_HELP_COLUMN(4, 4, 22);
            conditionWindow = Window_Create__menus(
                helpColumn, 8, TARGET_SET_DJINNI_WIDTH, 3, 6);
#endif
            Text_SetColor(2);
            Text_DrawCharacterAtOffset__menus((s32)MsgSetDjinni,
                                              conditionWindow, 0, 0);
            Text_SetColor(15);
            break;
          case 1:
            TARGET_HELP_COLUMN(7, 6, 17);
            conditionWindow = Window_Create__menus(helpColumn, 8, 13, 3, 6);
#if TLA_PATCH_2
            switch (System_GetLanguage()) {
            case CARTRIDGE_LANGUAGE_GERMAN:
              Text_DrawStringAtOffset__menus(gOwnerStatus_KpString,
                                             conditionWindow, 0, 0);
              break;
            case CARTRIDGE_LANGUAGE_FRENCH:
            case CARTRIDGE_LANGUAGE_SPANISH:
              Text_DrawStringAtOffset__menus(gOwnerStatus_PvString,
                                             conditionWindow, 0, 0);
              break;
            case CARTRIDGE_LANGUAGE_ITALIAN:
              Text_DrawStringAtOffset__menus(gOwnerStatus_HpString,
                                             conditionWindow, 0, 0);
              break;
            }
#else
            Text_DrawStringAtOffset__menus(gOwnerStatus_HpString,
                                           conditionWindow, 0, 0);
#endif
            Text_DrawNumberInWindow__menus(unit->hp, 4, conditionWindow, 16, 0);
            Text_DrawStringAtOffset__menus(gOwnerStatus_SlashString,
                                           conditionWindow, 48, 0);
            Text_DrawNumberInWindow__menus(unit->maxHp, 4, conditionWindow, 56,
                                           0);
            break;
          case 2:
            TARGET_HELP_COLUMN(7, 6, 17);
            conditionWindow = Window_Create__menus(helpColumn, 8, 13, 3, 6);
            Text_DrawStringAtOffset__menus(gOwnerStatus_PpString,
                                           conditionWindow, 0, 0);
            Text_DrawNumberInWindow__menus(unit->pp, 4, conditionWindow, 16, 0);
            Text_DrawStringAtOffset__menus(gOwnerStatus_SlashString,
                                           conditionWindow, 48, 0);
            Text_DrawNumberInWindow__menus(unit->maxPp, 4, conditionWindow, 56,
                                           0);
            break;
          case 5:
            TARGET_HELP_COLUMN(7, 5, 18);
            conditionWindow = Window_Create__menus(helpColumn, 8, 12, 3, 6);
            if (unit->hp != 0)
              goto no_condition;
            Text_DrawCharacterAtOffset__menus((s32)MsgEffectDowned,
                                              conditionWindow, 0, 0);
            break;
          case 3:
            TARGET_HELP_COLUMN(7, 5, 18);
            conditionWindow = Window_Create__menus(helpColumn, 8, 12, 3, 6);
            if (unit->poison == 0)
              goto no_condition;
            Text_DrawCharacterAtOffset__menus((s32)MsgEffectCurePoison,
                                              conditionWindow, 0, 0);
            break;
          case 4: {
            count = 0;
            if (unit->delusion != 0)
              count = 1;
            if (unit->stun != 0)
              count++;
            if (unit->sleep != 0)
              count++;
            if (unit->psySeal != 0)
              count++;
            if (unit->grimReaper != 0)
              count++;
            if (count == 0)
              count = 1;
            helpRow = 9 - count;
            if (helpRow <= 3)
              helpRow = 4;
            TARGET_CONDITION_LIST_WINDOW();
            count = 0;
            if (unit->delusion != 0) {
              Text_DrawCharacterAtOffset__menus((s32)MsgRemoveDelusion,
                                                conditionWindow, 0, 0);
              count = 1;
            }
            if (unit->stun != 0) {
              Text_DrawCharacterAtOffset__menus((s32)MsgRemoveStun,
                                                conditionWindow, 0, count * 8);
              count++;
            }
            if (unit->sleep != 0) {
              Text_DrawCharacterAtOffset__menus((s32)MsgWakeFromSleep,
                                                conditionWindow, 0, count * 8);
              count++;
            }
            if (unit->psySeal != 0) {
              Text_DrawCharacterAtOffset__menus((s32)MsgBreakPsynergySeal,
                                                conditionWindow, 0, count * 8);
              count++;
            }
            if (unit->grimReaper != 0) {
              Text_DrawCharacterAtOffset__menus((s32)MsgDispelGrimReaper,
                                                conditionWindow, 0, count * 8);
              count++;
            }
            if (count == 0)
              goto no_condition;
            break;
          }
          default:
            break;
          case 6: {
            count = 0;
            if (unit->poison != 0)
              count = 1;
            if (unit->delusion != 0)
              count++;
#if TLA_PATCH_2
            /* FAKEMATCH(temporary): count read back through a pointer to it,
             * which keeps count in its stack slot; as count == 0, 142 of 1748
             * instructions differ in tla-de. */
            countAddress = &count;
#endif
            if (unit->stun != 0)
              count++;
            if (unit->sleep != 0)
              count++;
            if (unit->psySeal != 0)
              count++;
            if (unit->grimReaper != 0)
              count++;
            if (unit->haunt != 0)
              count++;
#if TLA_PATCH_2
            if (*countAddress == 0)
#else
            if (count == 0)
#endif
              count = 1;
            helpRow = 9 - count;
            if (helpRow <= 3)
              helpRow = 4;
            TARGET_CONDITION_LIST_WINDOW();
            count = 0;
            if (unit->poison != 0) {
              Text_DrawCharacterAtOffset__menus((s32)MsgEffectCurePoison,
                                                conditionWindow, 0, 0);
              count = 1;
            }
            if (unit->delusion != 0) {
              Text_DrawCharacterAtOffset__menus((s32)MsgRemoveDelusion,
                                                conditionWindow, 0, count * 8);
              count++;
            }
            if (unit->stun != 0) {
              Text_DrawCharacterAtOffset__menus((s32)MsgRemoveStun,
                                                conditionWindow, 0, count * 8);
              count++;
            }
            if (unit->sleep != 0) {
              Text_DrawCharacterAtOffset__menus((s32)MsgWakeFromSleep,
                                                conditionWindow, 0, count * 8);
              count++;
            }
            if (unit->psySeal != 0) {
              Text_DrawCharacterAtOffset__menus((s32)MsgBreakPsynergySeal,
                                                conditionWindow, 0, count * 8);
              count++;
            }
            if (unit->grimReaper != 0) {
              Text_DrawCharacterAtOffset__menus((s32)MsgDispelGrimReaper,
                                                conditionWindow, 0, count * 8);
              count++;
            }
            if (unit->haunt != 0) {
              Text_DrawCharacterAtOffset__menus((s32)MsgDispelEvilSpirit,
                                                conditionWindow, 0, count * 8);
              count++;
            }
            if (count != 0)
              break;
          }
          no_condition:
            Text_SetColor(2);
            Text_DrawCharacterAtOffset__menus((s32)MsgEffectNone,
                                              conditionWindow, 0, 0);
            Text_SetColor(15);
            break;
          }
        }
      } else if (spread != 255) {
        target = Unit_Get__menus(candidateIds[targetIndex]);
        BattleMotion_ProjectConditionalPositionFar(candidateIds[targetIndex],
                                                   &namePosition);
        TARGET_BOB(namePosition.y);
        if (target->enemyId == 355 || target->enemyId == 352) {
          width = (s32)MsgMysteryMan;
          if (target->enemyId == 355)
            width++;
          TARGET_MYSTERY_NAME_COPY(width, targetName,
                                   TARGET_MYSTERY_NAME_CAPACITY);
        } else {
          for (i = 0; i <= 13; i++) {
            targetName[i] = n = target->name[i];
            if (!n)
              break;
          }
          targetName[i] = 0;
        }
        width = Text_GetWideStringWidth(targetName);
        namePosition.x -= width / 2;
        namePosition.x -= 8;
        if (namePosition.x + width > 224)
          namePosition.x = 224 - width;
        if (namePosition.x < 0)
          namePosition.x = 0;
        Vram_ClearWindowBlock__menus();
        Text_DrawWideStringAtOffset(targetName, window, namePosition.x, 4);
      }
      redraw &= ~1;
    }

    if (drawCursors != 0) {
      for (i = 1; i < targetCount; i++) {
        marker = &firstSprite[i];
        track = &cursorTracks[spreadTrackIndices[i]];
        BattleMotion_ProjectConditionalPositionFar(spreadActorIds[i],
                                                   &markerPosition);
        TARGET_BOB(markerPosition.y);
        *marker = *firstSprite;
        if (track->flags & 1) {
          s32 x = (markerPosition.x + track->x) / 2;
          s32 y = (markerPosition.y + track->y) / 2;

          markerPosition.x = x;
          track->x = x;
          markerPosition.y = y;
          track->y = y;
        } else {
          s32 x = marker->x;
          s32 y = marker->y + 8;

          track->flags = 1;
          markerPosition.x = x;
          track->x = x;
          markerPosition.y = y;
          track->y = y;
        }
        marker->mode = 1;
        marker->x = markerPosition.x - 8;
        marker->y = markerPosition.y - 12;
        if (spread == 255)
          marker->affine = 0;
        else
          marker->affine = 1;
        marker->matrix = matrix;
        Oam_AddSpriteAtDepth__menus(marker, 240);
      }
    }

    pressed = gInput.pressed;
    repeated = gInput.repeat;
    if (NAVIGATION->tutorialStep != 0) {
      repeated = 0;
      pressed = 0;
      if (NAVIGATION->tutorialDelay == 0) {
        NAVIGATION->tutorialDelay = 60;
        repeated = 1;
        pressed = 1;
      } else {
        NAVIGATION->tutorialDelay--;
      }
    }
    if (pressed & 1) {
      s32 id = candidateIds[targetIndex];
      s32 k;

      redraw = 0;
      result = -1;
      for (k = 0; k <= 5 && battle->partyUnits[k] != 255; k++) {
        if (battle->partyUnits[k] == id) {
          result = 0x100 | k;
          break;
        }
      }
      if (result < 0) {
        for (k = 0; k <= 5 && battle->enemyIds[k] != 255; k++) {
          if (battle->enemyIds[k] == id) {
            result = 0x180 | k;
            break;
          }
        }
      }
      targetIndex = result;
    } else if (spread != 255) {
      if (repeated & 0x90) {
        Sound_PlayFar(111);
        do {
          targetIndex++;
          targetIndex %= candidateCount;
        } while (candidateIds[targetIndex] == 254);
        redraw |= 1;
      }
      if (repeated & 0x60) {
        Sound_PlayFar(111);
        do {
          targetIndex = targetIndex + candidateCount - 1;
          targetIndex %= candidateCount;
        } while (candidateIds[targetIndex] == 254);
        redraw |= 1;
      }
    }
    if (NAVIGATION->timer == 0 || (pressed & 2)) {
      Sound_PlayFar(113);
      targetIndex = -1;
      break;
    }
    wait(1);
    if (redraw == 0)
      break;
  }
  wait(1);
  VramCache_FreeEntry(cursorSlot);
  if (conditionWindow != NULL)
    Window_Close__menus(conditionWindow, 1);
  Window_Close__menus(window, 1);
  BattleHighlight_SetModesFar(candidateIds, 0);
  NAVIGATION->targetViewOffset = 0;
  wait(1);
  return targetIndex;
}
#undef TARGET_SET_DJINNI_WIDTH
#undef TARGET_CONDITION_LIST_WIDTH
#undef TARGET_MYSTERY_NAME_COPY
#undef TARGET_MYSTERY_NAME_CAPACITY
#undef NAVIGATION
#undef TARGET_HELP_COLUMN
#undef TARGET_CONDITION_LIST_WINDOW
#undef TARGET_BOB
