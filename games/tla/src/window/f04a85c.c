// ---- games/tla/src/window/f04a85c.c ----
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"
#include "battle/battle_unit.h"
#include "version.h"
#include "module/system/system.h"

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

extern u8 MsgMysteryMan[];
extern u8 MsgEffectNone[];
extern u8 MsgEffectCurePoison[];
extern u8 MsgRemoveDelusion[];
extern u8 MsgRemoveStun[];
extern u8 MsgWakeFromSleep[];
extern u8 MsgBreakPsynergySeal[];
extern u8 MsgDispelGrimReaper[];
extern u8 MsgDispelEvilSpirit[];
extern u8 MsgEffectDowned[];
extern u8 MsgSetDjinni[];

struct TargetWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

struct TargetNavigation {
  u8 unk00[0x28];
  s32 targetViewOffset;
  u8 unk2c[0x20];
  s32 timer;
  u8 unk50[0x88];
  s32 tutorialStep;
  s32 tutorialDelay;
  u8 unke0[0x2c];
  s32 lastTarget;
};

struct TargetBattle {
  u8 unk00[0x58];
  s16 partyUnits[7];
  s16 enemyIds[7];
};

struct TargetPoint {
  s32 x;
  s32 y;
  s32 z;
};

struct TargetTrack {
  u8 x;
  u8 y;
  u8 flags;
  s8 index;
};

struct TargetSprite {
  u8 unk00[4];
  u8 y;
  u8 affine : 2;
  u8 mode : 2;
  u8 high05 : 4;
  u16 x : 9;
  u16 matrix : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 attr2high : 6;
  u16 unk0a;
};

struct TargetAffine {
  u16 scaleX;
  u16 scaleY;
  u16 angle;
  u16 unk06;
};

#define NAVIGATION ((struct TargetNavigation *)Ram_HeapSlots->unke4[0])

extern const u8 gData_0805c9c4[];
extern const u8 gOwnerStatus_HpString[];
extern const u8 gOwnerStatus_SlashString[];
extern const u8 gOwnerStatus_PpString[];
#if TLA_PATCH_2
extern const u8 gOwnerStatus_KpString[];
extern const u8 gOwnerStatus_PvString[];
s32 System_GetLanguage(void);
#endif

s32 VramCache_AllocEntry(s32 size);
/* VIEW(f04a85c): routine VramCache_Reload as s32 VramCache_Reload(s32 entry, const void *tiles); resource.h has s32 VramCache_Reload(s32 slot, s32 source); tried as the definition's s32 VramCache_Reload(s32 no, s32 src), Func_0804a85c moves: 1 of 1647 instructions in tla-ja. */
extern s32 VramCache_Reload__f04a85c(s32 entry, const void *tiles) __asm__("VramCache_Reload");
/* VIEW(f04a85c): routine Unit_Get as struct BattleUnit *Unit_Get(s32 unit); the definition has s32 Func_0804b8b8(s32 slot) { u16 owners[6]; u16 selected[2]; struct BattleWork *battle; s32 count; count = BattleParty_PrepareReserveOwners...; tried as the definition's void *Unit_Get(s32 owner), which the object already declares, it does not compile in tla-ja: games/tla/src/window/f04a85c.c:328: invalid type argument of `->'. */
extern struct BattleUnit *Unit_Get__f04a85c(s32 unit) __asm__("Unit_Get");
s32 GameFlag_Test(s32 flag);
s32 BattleMotion_ProjectConditionalPositionFar(s32 unit,
                                               struct TargetPoint *point);
struct TargetWindow *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                   s32 flags);
s32 Trig_Sin(s32 angle);
/* VIEW(f04a85c): routine Oam_AddSpriteAtDepth as s32 Oam_AddSpriteAtDepth(void *sprite, s32 depth); task.h has void Oam_AddSpriteAtDepth(void *entry, s32 priority); tried as the definition's void Oam_AddSpriteAtDepth(void *entry, s32 slot), Func_0804a85c moves: 2 of 1647 instructions in tla-ja. */
extern s32 Oam_AddSpriteAtDepth__f04a85c(void *sprite, s32 depth) __asm__("Oam_AddSpriteAtDepth"); // Returns void; see the tag.
s32 AffineMatrix_BuildForEffect(struct TargetAffine *affine);
void BattleHighlight_SetModesFar(u16 *units, s32 mode);
/* VIEW(f04a85c): routine Window_Close as void Window_Close(void *window, s32 mode); uiflow.h has void Window_Close(struct Window *window, s32 release); tried as the definition's void Window_Close(struct Window *work, s32 release), it does not compile in tla-ja: games/tla/include/graphics/uiflow.h:7: conflicting types for `Window_Close'. */
extern void Window_Close__f04a85c(void *window, s32 mode) __asm__("Window_Close");
/* VIEW(f04a85c): routine Text_DrawStringAtOffset as void Text_DrawStringAtOffset(const u8 *text, struct TargetWindow *window, s32 x, s32 y); text_render.h has void Text_DrawStringAtOffset(u8 *text, struct RenderInput *work, s32 offsetX, s32 offsetY); tried as the definition's void Text_DrawStringAtOffset(u8 *text, struct RenderInput *work, s32 offsetX, s32 offsetY), which the object already declares, Func_0804a85c moves: 8 of 1647 instructions in tla-ja. */
extern void Text_DrawStringAtOffset__f04a85c(const u8 *text, struct TargetWindow *window, s32 x,
                             s32 y) __asm__("Text_DrawStringAtOffset");
void Text_DrawNumberInWindow(s32 value, s32 digits, struct TargetWindow *window,
                             s32 x, s32 y);
/* VIEW(f04a85c): routine Text_DrawCharacterAtOffset as void Text_DrawCharacterAtOffset(s32 message, struct TargetWindow *window, s32 x, s32 y); text_render.h has void Text_DrawCharacterAtOffset(s32 character, struct RenderInput *work, u32 offsetX, u32 offsetY); tried as the definition's void Text_DrawCharacterAtOffset(s32 character, struct RenderInput *position, u32 offsetX, u32 offsetY), it does not compile in tla-ja: games/tla/include/graphics/text_render.h:10: conflicting types for `Text_DrawCharacterAtOffset'. */
extern void Text_DrawCharacterAtOffset__f04a85c(s32 message, struct TargetWindow *window, s32 x,
                                s32 y) __asm__("Text_DrawCharacterAtOffset");
void Func_08041f70(s32 color);
void Message_CopyExpanded(s32 message, u16 *text, s32 capacity);
void Message_DecodeRaw(s32 message, u16 *text, s32 capacity);
s32 Text_GetWideStringWidth(u16 *text);
/* VIEW(f04a85c): routine Vram_ClearWindowBlock as void Vram_ClearWindowBlock(void); graphics.h has s32 Vram_ClearWindowBlock(void); tried as the definition's s32 Vram_ClearWindowBlock(void), which the object already declares, Func_0804a85c moves: 1 of 1647 instructions in tla-ja. */
extern void Vram_ClearWindowBlock__f04a85c(void) __asm__("Vram_ClearWindowBlock");
void Text_DrawWideStringAtOffset(u16 *text, struct TargetWindow *window, s32 x,
                                 s32 y);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
s32 Resource_ResetEntry(u32 slot);

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

s32 Func_0804a85c(s32 selection, s32 mode, u32 spread, u32 kind) {
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
        candidate = Unit_Get__f04a85c(candidateIds[j]);
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
         (GameFlag_Test(0x16c) && mode == 1 &&
          Unit_Get__f04a85c(candidateIds[targetIndex])->hp == 0)) {
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
  window = Window_Create(0, 12, 30, 4, 74);

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
    firstSprite->tile = VramCache_Reload__f04a85c(
        cursorSlot, ((gFrameCount >> 2) & 31) * 256 + gData_0805c9c4);
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
    Oam_AddSpriteAtDepth__f04a85c(firstSprite, 240);
    if (spread == 255) {
      effect.scaleX = 256;
      effect.scaleY = 256;
    } else {
      effect.scaleX = 176;
      effect.scaleY = 176;
    }
    effect.angle = 0;
    matrix = AffineMatrix_BuildForEffect(&effect);

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
          unit = Unit_Get__f04a85c(candidateIds[targetIndex]);
          BattleMotion_ProjectConditionalPositionFar(candidateIds[targetIndex],
                                                     &selectedPosition);
          if (conditionWindow != NULL)
            Window_Close__f04a85c(conditionWindow, 1);
          switch (kind) {
          case 7:
#if TLA_PATCH_2
          {
            s32 anchor = selectedPosition.x / 8;

            helpColumn = anchor - 4;
            if (System_GetLanguage() == CARTRIDGE_LANGUAGE_ITALIAN) {
              if (anchor + 6 > 29)
                helpColumn = 20;
              conditionWindow = Window_Create(helpColumn, 8, 10, 3, 6);
            } else if (System_GetLanguage() == CARTRIDGE_LANGUAGE_SPANISH) {
              if (anchor + 7 > 29)
                helpColumn = 19;
              conditionWindow = Window_Create(helpColumn, 8, 11, 3, 6);
            } else {
              if (anchor + 5 > 29)
                helpColumn = 21;
              conditionWindow = Window_Create(helpColumn, 8, 9, 3, 6);
            }
          }
#else
            TARGET_HELP_COLUMN(4, 4, 22);
            conditionWindow =
                Window_Create(helpColumn, 8, TARGET_SET_DJINNI_WIDTH, 3, 6);
#endif
            Func_08041f70(2);
            Text_DrawCharacterAtOffset__f04a85c((s32)MsgSetDjinni, conditionWindow, 0,
                                       0);
            Func_08041f70(15);
            break;
          case 1:
            TARGET_HELP_COLUMN(7, 6, 17);
            conditionWindow = Window_Create(helpColumn, 8, 13, 3, 6);
#if TLA_PATCH_2
            switch (System_GetLanguage()) {
            case CARTRIDGE_LANGUAGE_GERMAN:
              Text_DrawStringAtOffset__f04a85c(gOwnerStatus_KpString, conditionWindow, 0,
                                      0);
              break;
            case CARTRIDGE_LANGUAGE_FRENCH:
            case CARTRIDGE_LANGUAGE_SPANISH:
              Text_DrawStringAtOffset__f04a85c(gOwnerStatus_PvString, conditionWindow, 0,
                                      0);
              break;
            case CARTRIDGE_LANGUAGE_ITALIAN:
              Text_DrawStringAtOffset__f04a85c(gOwnerStatus_HpString, conditionWindow, 0,
                                      0);
              break;
            }
#else
            Text_DrawStringAtOffset__f04a85c(gOwnerStatus_HpString, conditionWindow, 0,
                                    0);
#endif
            Text_DrawNumberInWindow(unit->hp, 4, conditionWindow, 16, 0);
            Text_DrawStringAtOffset__f04a85c(gOwnerStatus_SlashString, conditionWindow,
                                    48, 0);
            Text_DrawNumberInWindow(unit->maxHp, 4, conditionWindow, 56, 0);
            break;
          case 2:
            TARGET_HELP_COLUMN(7, 6, 17);
            conditionWindow = Window_Create(helpColumn, 8, 13, 3, 6);
            Text_DrawStringAtOffset__f04a85c(gOwnerStatus_PpString, conditionWindow, 0,
                                    0);
            Text_DrawNumberInWindow(unit->pp, 4, conditionWindow, 16, 0);
            Text_DrawStringAtOffset__f04a85c(gOwnerStatus_SlashString, conditionWindow,
                                    48, 0);
            Text_DrawNumberInWindow(unit->maxPp, 4, conditionWindow, 56, 0);
            break;
          case 5:
            TARGET_HELP_COLUMN(7, 5, 18);
            conditionWindow = Window_Create(helpColumn, 8, 12, 3, 6);
            if (unit->hp != 0)
              goto no_condition;
            Text_DrawCharacterAtOffset__f04a85c((s32)MsgEffectDowned, conditionWindow, 0,
                                       0);
            break;
          case 3:
            TARGET_HELP_COLUMN(7, 5, 18);
            conditionWindow = Window_Create(helpColumn, 8, 12, 3, 6);
            if (unit->poison == 0)
              goto no_condition;
            Text_DrawCharacterAtOffset__f04a85c((s32)MsgEffectCurePoison,
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
              Text_DrawCharacterAtOffset__f04a85c((s32)MsgRemoveDelusion,
                                         conditionWindow, 0, 0);
              count = 1;
            }
            if (unit->stun != 0) {
              Text_DrawCharacterAtOffset__f04a85c((s32)MsgRemoveStun, conditionWindow, 0,
                                         count * 8);
              count++;
            }
            if (unit->sleep != 0) {
              Text_DrawCharacterAtOffset__f04a85c((s32)MsgWakeFromSleep, conditionWindow,
                                         0, count * 8);
              count++;
            }
            if (unit->psySeal != 0) {
              Text_DrawCharacterAtOffset__f04a85c((s32)MsgBreakPsynergySeal,
                                         conditionWindow, 0, count * 8);
              count++;
            }
            if (unit->grimReaper != 0) {
              Text_DrawCharacterAtOffset__f04a85c((s32)MsgDispelGrimReaper,
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
              Text_DrawCharacterAtOffset__f04a85c((s32)MsgEffectCurePoison,
                                         conditionWindow, 0, 0);
              count = 1;
            }
            if (unit->delusion != 0) {
              Text_DrawCharacterAtOffset__f04a85c((s32)MsgRemoveDelusion,
                                         conditionWindow, 0, count * 8);
              count++;
            }
            if (unit->stun != 0) {
              Text_DrawCharacterAtOffset__f04a85c((s32)MsgRemoveStun, conditionWindow, 0,
                                         count * 8);
              count++;
            }
            if (unit->sleep != 0) {
              Text_DrawCharacterAtOffset__f04a85c((s32)MsgWakeFromSleep, conditionWindow,
                                         0, count * 8);
              count++;
            }
            if (unit->psySeal != 0) {
              Text_DrawCharacterAtOffset__f04a85c((s32)MsgBreakPsynergySeal,
                                         conditionWindow, 0, count * 8);
              count++;
            }
            if (unit->grimReaper != 0) {
              Text_DrawCharacterAtOffset__f04a85c((s32)MsgDispelGrimReaper,
                                         conditionWindow, 0, count * 8);
              count++;
            }
            if (unit->haunt != 0) {
              Text_DrawCharacterAtOffset__f04a85c((s32)MsgDispelEvilSpirit,
                                         conditionWindow, 0, count * 8);
              count++;
            }
            if (count != 0)
              break;
          }
          no_condition:
            Func_08041f70(2);
            Text_DrawCharacterAtOffset__f04a85c((s32)MsgEffectNone, conditionWindow, 0,
                                       0);
            Func_08041f70(15);
            break;
          }
        }
      } else if (spread != 255) {
        target = Unit_Get__f04a85c(candidateIds[targetIndex]);
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
        Vram_ClearWindowBlock__f04a85c();
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
        Oam_AddSpriteAtDepth__f04a85c(marker, 240);
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
  Resource_ResetEntry(cursorSlot);
  if (conditionWindow != NULL)
    Window_Close__f04a85c(conditionWindow, 1);
  Window_Close__f04a85c(window, 1);
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

// ---- games/tla/src/window/countdown.c ----
#include "task.h"
#include "scene.h"
#include "fixed_math.h"
void Camera_ConfigureSceneFar(s32 offset);
void Link_DrawShiftedTilePair(s32 destination);
s32 Link_CreateCountdownLabelWindow(void);

#define COUNTDOWN_START_FRAMES 900
#define FRAMES_PER_SECOND 60

struct CountdownDisplayEntry {
  u8 data[12];
};

struct LinkSignature {
  u8 padding000[8];
  u16 e;
  u16 d;
  u16 v;
  u16 s;
  u8 padding010[8];
};

struct LinkCountdownState {
  struct CountdownDisplayEntry entries[3];
  u8 entryActive[3];
  u8 padding027;
  s32 targetOffset;
  s32 currentOffset;
  u8 padding030[20];
  s32 displayHandle;
  s32 secondaryHandle;
  s32 timer;
  s32 enabled;
};

struct LinkRuntimeState {
  u8 padding000[80];
  u8 side;
  u8 padding051;
  u8 paused;
};

extern struct LinkCountdownState *gLinkCountdownWork;
extern struct LinkRuntimeState *gBattleWork;
extern struct LinkSignature gLinkPeerSignatures[];


void Sound_PlayFar(s32 soundId);

void UpdateLinkSessionCountdown(void) {
  struct LinkCountdownState *state =
      ((struct LinkCountdownState **)Ram_HeapSlots)[57];
  struct LinkRuntimeState *runtime;
  struct CountdownDisplayEntry *entry;
  u8 *active;
  struct LinkSignature *signature;
  s32 target;
  s32 current;
  s32 difference;
  s32 step;
  s32 newOffset;
  s32 timer;
  s32 nextTimer;
  s32 seconds;
  s32 i;

  if (state != 0) {
    target = state->targetOffset;
    current = state->currentOffset;
    if (target != current) {
      difference = target - current;
      step = __divsi3(difference, 3);
      if (step == 0) {
        step--;
        if (difference >= 0)
          step = 1;
      }
      newOffset = current + step;
      state->currentOffset = newOffset;
      Camera_ConfigureSceneFar(newOffset);
    }

    entry = state->entries;
    active = state->entryActive;
    i = 2;
    do {
      if (*active++ != 0)
        Oam_AddSpriteAtDepth(entry, 240);
      i--;
      entry++;
    } while (i >= 0);
    Link_DrawShiftedTilePair(0x06006680);

    if (state->enabled == 0)
      goto done;

    runtime = (struct LinkRuntimeState *)Ram_HeapSlots->battleWork;
    if (runtime->paused == 0)
      goto load_timer;
    state->timer = 0;
    goto done;

  signature_tail:
    if (signature->v != 'V')
      goto signature_done;
    if (signature->s != 'S')
      goto signature_done;
    state->timer = COUNTDOWN_START_FRAMES;
    timer = state->timer;
    goto signature_done;

  load_timer:
    timer = state->timer;
    if (timer >= 0)
      goto timer_ready;
    signature = &gLinkPeerSignatures[runtime->side ^ 1];
    if (signature->e != 'E')
      goto signature_done;
    if (signature->d == 'D')
      goto signature_tail;

  signature_done:
    if (timer < 0)
      goto done;

  timer_ready:
    if (state->displayHandle == 0 && state->secondaryHandle == 0) {
      state->displayHandle = Link_CreateCountdownLabelWindow();
      timer = state->timer;
    }

    if (timer > 0) {
      nextTimer = timer - 1;
      state->timer = nextTimer;
      timer = nextTimer;
    }
    if (timer < 0)
      goto done;

    seconds = __divsi3(timer + FRAMES_PER_SECOND - 1, FRAMES_PER_SECOND);
    if (seconds != 0 && seconds * FRAMES_PER_SECOND == timer)
      Sound_PlayFar(108);

    if (state->displayHandle != 0)
      Text_DrawNumberInWindow(seconds, 2, state->displayHandle, 16, 8);
  }

done:
  return;
}

#include "edition.h"
#include "io_reg.h"
#include "system.h"
#include "main_routine_work.h"
unsigned char Vram_FillWindowBlockPattern(void);

// The Japanese edition decodes the message (Message_DecodeRaw) into at most 32
// halfwords; the others copy its text (Message_CopyExpanded) into at most 52.
#if LANGUAGE != LANGUAGE_JAPANESE
#define BATTLE_MENU_MESSAGE_CAPACITY 52
#define BATTLE_MENU_MESSAGE_COPY Message_CopyExpanded__countdown
#else
#define BATTLE_MENU_MESSAGE_CAPACITY 32
#define BATTLE_MENU_MESSAGE_COPY Message_DecodeRaw
#endif


/* VIEW(countdown): routine Message_CopyExpanded as s32 Message_CopyExpanded(s32, s16 *, s32); f04a85c has void Message_CopyExpanded(s32 message, u16 *text, s32 capacity); tried as the definition's s32 Message_CopyExpanded(s32 key, u16 *destination, u32 capacity), it does not compile in tla-ja: games/tla/src/window/f04a85c.c:977: conflicting types for `Message_CopyExpanded'. */
extern s32 Message_CopyExpanded__countdown(s32, s16 *, s32) __asm__("Message_CopyExpanded");

extern s32 gKeyState;
extern u8 MsgNoTimeToRun[];

s32 Message_ShowLocalizedAndWait(void) {
  s16 buffer[64];
  s32 work;
  s32 result;
  void *state;
  s32 mode;
  s32 attrs;

  state = Ram_HeapSlots->battleWork;
  mode = gMainRoutineWork.battleRule24b;
  if (gMainRoutineWork.battleRule24b == 2 || mode <= 2 ||
      (result = 1, mode > 4)) {
    result = 0;
  }
  if (result == 0) {
    if (*(s8 *)((u8 *)state + 0x43) != 0) {
      result = 1;
    }
  }
  if (result != 0) {
    attrs = 42;
    work = Window_Create(0, 7, 30, 4, attrs);
    Vram_FillWindowBlockPattern();
    BATTLE_MENU_MESSAGE_COPY((s32)&MsgNoTimeToRun, buffer,
                             BATTLE_MENU_MESSAGE_CAPACITY);
    Text_DrawWideStringAtOffset(buffer, work, 0, 4);
    do {
      wait(1);
    } while ((gInput.pressed & (KEY_A | KEY_B)) == 0 &&
             *(s32 *)((u8 *)((void **)Ram_HeapSlots)[57] + 0x4C) != 0);
    Window_Close(work, 1);
  }
  return result;
}

#include "battle_system.h"

/// Classifies an ability's effect for target selection.
/// 0 other, 1 restore HP, 2 restore PP, 3 cure poison, 4 cure an ailment,
/// 5 revive, 6 cure all ailments.
/// act: ability data.
s32 Battle_ClassifyEntryKind(const struct BattleAct *act) {
  s32 val;
  s32 type;
  s32 kind;
  u8 effect;

  kind = 0;
  type = act->flags & 15;
  if (type == 1) // Restore HP.
    kind = 1;
  if (type == 11) // Restore PP.
    kind = 2;
  val = (s8)act->effect;
  effect = val;
  if (effect == 3) // Cure poison.
    kind = 3;
  if (effect == 4) // Cure an ailment.
    kind = 4;
  if (effect == 64) // Cure all ailments.
    kind = 6;
  if (Battle_IsReviveEffectFar(act->effect) != 0) // Revive.
    kind = 5;
  return kind;
}

#include "battle_work.h"
#include "actor.h"

s32 BattleParty_PrepareReserveOwnersFar(u16 *owners);
s32 Func_08118140(s32, s32);
struct BattleObj *GetBattleObjectSlotFar(s32 objectId);
void Actor_SetPositionAndResetMotionFar(struct Actor *object, s32 x, s32 y,
                                        s32 z);
void PartyStatus_Draw(s32 flags);
void BattleActor_CommitPlacementFar(void);
void BattleHighlight_SetModesFar(u16 *, s32);
s32 BattleLayout_HighlightPartyPanels(u16 *ids);

// The keys, read through a volatile view: the loop reloads them on each test.
#define INPUT ((volatile struct InputState *)&gInput)

/// Lets the player choose which reserve member takes a party slot in battle,
/// previewing each choice in place: left and right cycle through the reserve,
/// A confirms and swaps the two in gMainRoutineWork.activeOwners, and B puts
/// the slot's member back.
s32 Func_0804b8b8(s32 slot) {
  u16 owners[6];
  u16 selected[2];
  struct BattleWork *battle;
  s32 count;

  count = BattleParty_PrepareReserveOwnersFar(owners);
  {
    s32 sel;
    s32 redraw;
    s32 result;
    s32 *reserve;
    s32 current;
    s32 newId;
    s32 i;

    sel = 0;
    redraw = 1;
    battle = Ram_HeapSlots->battleWork;
    result = 0;
    reserve = Ram_HeapSlots->unke4[0];
    if (count <= 0)
      return -1;
    slot &= 0xff;
    current = battle->partyUnits[slot];
    Sound_PlayFar(112);
    // Once-only block: the native's code order after the first sound.
    do {
      goto test;
      for (;;) {
        // Japanese also takes A on the first frame, before the first preview
        // has been drawn.
#if TLA_PATCH_1
        if ((INPUT->pressed & 1) && !redraw) {
#else
        if (INPUT->pressed & 1) {
#endif
          if (Func_08118140(1, 0) == 0) {
            Sound_PlayFar(114);
            goto test;
          }
          Sound_PlayFar(110);
          reserve[65] = current;
          reserve[66] = slot;
          for (i = 0; i < 8; i++) {
            if (gMainRoutineWork.activeOwners[i] == current)
              gMainRoutineWork.activeOwners[i] = newId;
            else if (gMainRoutineWork.activeOwners[i] == newId)
              gMainRoutineWork.activeOwners[i] = current;
          }
          goto done;
        }
        if (INPUT->repeat & 0x20) {
          sel = (sel + count - 1) % count;
          Sound_PlayFar(112);
          redraw = 1;
        } else if (INPUT->repeat & 0x10) {
          sel = (sel + 1) % count;
          Sound_PlayFar(112);
          redraw = 1;
        }
        if (redraw) {
          struct BattleObj *from;
          struct BattleObj *to;
          struct Actor *object;
          struct Actor *place;

          redraw = 0;
          newId = battle->partyUnits[slot] = owners[sel];
          from = GetBattleObjectSlotFar(current);
          to = GetBattleObjectSlotFar(newId);
          to->anchorX = from->anchorX;
          place = (struct Actor *)from->object;
          object = (struct Actor *)to->object;
          to->anchorZ = from->anchorZ;
          Actor_SetPositionAndResetMotionFar(object, place->x, place->y,
                                             place->z);
          PartyStatus_Draw(Ram_HeapSlots->battleWork->partyStatus);
          BattleActor_CommitPlacementFar();
          selected[0] = newId;
          selected[1] = 0xff;
          BattleHighlight_SetModesFar(selected, 2);
          BattleLayout_HighlightPartyPanels(selected);
        }
        wait(1);
      test:
        newId = battle->partyUnits[slot] = owners[sel];
        if (INPUT->pressed & 2)
          break;
      }
      battle->partyUnits[slot] = current;
      result = -1;
      Sound_PlayFar(113);
    done:
      selected[0] = 0xff;
      BattleHighlight_SetModesFar(selected, 0);
      BattleLayout_HighlightPartyPanels(selected);
      PartyStatus_Draw(Ram_HeapSlots->battleWork->partyStatus);
      BattleActor_CommitPlacementFar();
      wait(1);
    } while (0);
    return result;
  }
}

#include "battle_unit.h"
#include "battle_action.h"

s32 BattleParty_ListLivingUnitsFar(s32 sideMask, u16 *unitIds);
void *Unit_Get(s32 owner);

s32 Func_0804babc(u16 *ready, struct BattleActionRecord **cursor,
                  s32 *forcedCount) {
  u16 *unitIds = Heap_AllocEwram(9);
  struct BattleActionRecord *entries = *cursor;
  s32 count = BattleParty_ListLivingUnitsFar(1, unitIds);
  s32 readyCount = 0;
  s32 forced = 0;
  s32 i;
  s32 action;

  for (i = 0; i < count; i++) {
    struct BattleUnit *unit = Unit_Get(unitIds[i]);

    for (action = 0; action < unit->actionEntryCount; action++) {
      if (unit->sleep != 0 || unit->confusion != 0 || unit->charm != 0 ||
          unit->stun != 0) {
        struct BattleActionRecord *entry = &entries[forced];

        entry->unitId = unitIds[i];
        entry->priority = unit->agi;
        entry->command = 8;
        entry->parameter = 0;
        entry->target = 384;
        forced++;
      } else {
        ready[readyCount++] = unitIds[i];
      }
    }
  }
  *forcedCount = forced;
  entries += forced;
  *cursor = entries;
  return readyCount;
}
#undef COUNTDOWN_START_FRAMES
#undef FRAMES_PER_SECOND
#undef BATTLE_MENU_MESSAGE_CAPACITY
#undef BATTLE_MENU_MESSAGE_COPY
#undef INPUT

// ---- games/tla/src/window/battle_party_commands.c ----
// Collecting the party's battle commands, as The Broken Seal's
// Battle_CollectPartyCommands does on this game's work and menus, then
// Func_0804cda8, which draws a menu's icons each frame, the selected one
// bouncing, and fits the window's lines around it.

#include "graphics/uiwindow.h"
#include "battle/battle_summon.h"
#include "battle/battle_system.h"
#include "window.h"

// Where the Japanese layout differs: the summon window, the item window and
// the summon's Djinn costs, in tiles; the x of the acting member's icon and of
// the command icon in the summon list, beside a chosen summon and in the Djinn
// list, in pixels.
#if LANGUAGE != LANGUAGE_JAPANESE
#define SUMMON_WINDOW_X 10
#define SUMMON_WINDOW_WIDTH 17
#define SUMMON_COST_X 11
#define ITEM_WINDOW_WIDTH 16
#define ACTOR_ICON_X 0
#define SUMMON_MENU_ICON_X 80
#define SUMMON_CHOSEN_ICON_X 56
#define DJINN_MENU_ICON_X 140
#else
#define SUMMON_WINDOW_X 9
#define SUMMON_WINDOW_WIDTH 16
#define SUMMON_COST_X 10
#define ITEM_WINDOW_WIDTH 15
#define ACTOR_ICON_X 4
#define SUMMON_MENU_ICON_X 88
#define SUMMON_CHOSEN_ICON_X 48
#define DJINN_MENU_ICON_X 144
#endif

// The x of the Attack window's name, in pixels.
#if LANGUAGE == LANGUAGE_FRENCH
#define ATTACK_NAME_X 14
#else
#define ATTACK_NAME_X 16
#endif

// The x of the command icon in the Psynergy list, in pixels.
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)
#define PSYNERGY_MENU_ICON_X 40
#else
#define PSYNERGY_MENU_ICON_X 48
#endif

extern u8 MsgAbilityNothing[];
extern u8 MsgCommandName[];
extern u8 MsgDjinnName[];
extern u8 MsgItemName[];

/// A menu sprite: a list link and its OAM attributes, as two words or as
/// fields.
struct MenuSprite {
  struct MenuSprite *next;
  union {
    struct {
      unsigned long attr01;
      unsigned long attr23;
    } word;
    struct {
      u16 y : 8;
      u16 affine : 2;
      u16 mode : 2;
      u16 mosaic : 1;
      u16 colors : 1;
      u16 shape : 2;
      u16 x : 9;
      u16 affineIndex : 5;
      u16 size : 2;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
      u16 unused;
    } f;
  } oam;
};

/// The battle menu work (heap slot 57), The Broken Seal's BattleMenuWork.
struct BattleMenuWork {
  struct MenuSprite icons[3];
  u8 iconShown[3];
  u8 unk27;
  s32 targetViewOffset;
  s32 viewOffset;
  s32 row;
  s32 page;
  s32 preferredRow;
  s32 commandSelection[2];
  void *timerWindow;
  s32 timerHidden;
  s32 timer;
  s32 linkTimer;
  u8 standbyDjinn[8][4];
  u8 listSlots[34];
  u16 listAbilities[33];
  u8 unkd8[0];
  s32 tutorialStep;
  s32 tutorialDelay;
  s32 commandActor;
  u32 chosenDjinn[8];
  s32 swappedUnit;  ///< the unit a reserve swap replaced, -1 for none
  s32 swappedPlace; ///< its party place
  s32 lastTarget;
};

struct BattleAction {
  u16 unitId;
  u16 unk02;
  s16 priority;
  u16 command;
  u16 parameter;
  u16 target;
  u16 range;
  u16 unk0e;
};

#define NAVIGATION ((struct BattleMenuWork *)Ram_HeapSlots->unke4[0])

extern u16 gData_02003a74[];
extern u16 gLinkStatus;
/* VIEW(battle_party_commands): variable gLinkPeerSignatures as extern u16 gLinkPeerSignatures[][12]; countdown has extern struct LinkSignature gLinkPeerSignatures[]; tried as the 2 declarations' extern u16 gLinkPeerSignatures[][12], it does not compile in tla-ja: games/tla/src/window/f04a85c.c:1344: conflicting types for `gLinkPeerSignatures'. */
extern u16 gLinkPeerSignatures__battle_party_commands[][12] __asm__("gLinkPeerSignatures");
/* VIEW(battle_party_commands): variable gMainRoutineWork as extern u8 gMainRoutineWork[]; main_routine_work.h has extern struct MainRoutineWork gMainRoutineWork; tried as the 3 declarations' extern u8 gMainRoutineWork[], it does not compile in tla-ja: games/tla/src/window/f04a85c.c:1346: conflicting types for `gMainRoutineWork'. */
extern u8 gMainRoutineWork__battle_party_commands[] __asm__("gMainRoutineWork");
extern u8 gDebugMode;
extern u8 MsgWaitingForOpponent[];

s32 VramCache_AllocEntry(s32 size);
void *HeapSlot_AllocEwram(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 Resource_LoadIndexedIntoBuffer(s32 slot, s32 index);
void Graphics_ExpandVramTilesByColorTable(u16 *tiles);
/* VIEW(battle_party_commands): routine Link_DrawShiftedTilePair as void Link_DrawShiftedTilePair(u32 address); countdown has void Link_DrawShiftedTilePair(s32 destination); tried as the rest of the object declares it, Func_0804bc08 moves: 1 of 1909 instructions in tla-ja. */
extern void Link_DrawShiftedTilePair__battle_party_commands(u32 address) __asm__("Link_DrawShiftedTilePair");
void Menu_BuildLocalizedPatternTiles(void);
void wait(s32 frames);
void UpdateLinkSessionCountdown(void);
/* VIEW(battle_party_commands): routine Func_0804babc as s32 Func_0804babc(u16 *actors, struct BattleAction **cursor, s32 *forcedCount); the definition has s32 Func_0804b8b8(s32 slot) { u16 owners[6]; u16 selected[2]; struct BattleWork *battle; s32 count; count = BattleParty_PrepareReserveOwners...; tried as the rest of the object declares it, Func_0804bc08 moves: 1 of 1909 instructions in tla-ja. */
extern s32 Func_0804babc__battle_party_commands(u16 *actors, struct BattleAction **cursor, s32 *forcedCount) __asm__("Func_0804babc");
/* VIEW(battle_party_commands): routine BattleLayout_HighlightPartyPanels as void BattleLayout_HighlightPartyPanels(u16 *units); countdown has s32 BattleLayout_HighlightPartyPanels(u16 *ids); tried as the definition's s32 BattleLayout_HighlightPartyPanels(u16 *ids), which the object already declares, Battle_DrawPartyPanelsWithEmptyList.0 moves: 1 of 18 instructions, Func_0804bc08 moves: 3 of 1909 instructions in tla-ja. */
extern void BattleLayout_HighlightPartyPanels__battle_party_commands(u16 *units) __asm__("BattleLayout_HighlightPartyPanels");
void BattleHighlight_SetModesFar(u16 *units, s32 mode);
s32 GameFlag_Test(s32 flag);
s32 Func_080457d0(s32 mode, s32 fleeAllowed);
void PartyStatus_Draw(s32 flags);
void BattleActor_CommitPlacementFar(void);
void Func_08118158(void);
void Sound_PlayFar(s32 cue);
s32 Func_0804a85c(s32 selection, s32 mode, u32 spread, u32 kind);
s32 Func_0804b8b8(s32 slot);
void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
/* VIEW(battle_party_commands): routine BattleParty_ListActorIdsFar as s32 BattleParty_ListActorIdsFar(s32 side, u16 *units); graphics.h has s32 BattleParty_ListActorIdsFar(s32 side, s32 group); tried as the rest of the object declares it, Func_0804bc08 moves: 2 of 1909 instructions in tla-ja. */
extern s32 BattleParty_ListActorIdsFar__battle_party_commands(s32 side, u16 *units) __asm__("BattleParty_ListActorIdsFar");
s32 BattleParty_PrepareReserveOwnersFar(u16 *units);
s32 Func_08046b08(u16 *list, s32 listCount, s32 member);
s32 Message_ShowLocalizedAndWait(void);
/* VIEW(battle_party_commands): routine Message_DecodeRaw as void Message_DecodeRaw(s32 message, u16 *text, s32 capacity); countdown has void Message_DecodeRaw(s32, s16 *, s32); tried as the definition's void Message_DecodeRaw(s32 message, u16 *text, s32 capacity), which the object already declares, Func_0804bbd0.1 moves: 2 of 24 instructions in tla-ja. */
extern void Message_DecodeRaw__battle_party_commands(s32 message, u16 *text, s32 capacity) __asm__("Message_DecodeRaw");
void Func_08118130(s32 kind, u8 *counts);
void *Unit_Get(s32 owner);
s32 Func_08045528(s32 portrait, s32 slot);
void Func_08041b68(void);
/* VIEW(battle_party_commands): routine Window_Create as struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags); f04a85c has struct TargetWindow *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags); tried as the definition's struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs), which the object already declares, Func_0804bc08 moves: 7 of 1909 instructions in tla-ja. */
extern struct Window *Window_Create__battle_party_commands(s32 x, s32 y, s32 width, s32 height, s32 flags) __asm__("Window_Create");
/* VIEW(battle_party_commands): routine Text_DrawCharacterAtOffset as void Text_DrawCharacterAtOffset(s32 message, struct Window *window, s32 x, s32 y); f04a85c has void Text_DrawCharacterAtOffset(s32 message, struct TargetWindow *window, s32 x, s32 y); tried as the definition's void Text_DrawCharacterAtOffset(s32 character, struct RenderInput *position, u32 offsetX, u32 offsetY), Func_0804bc08 moves: 7 of 1909 instructions in tla-ja. */
extern void Text_DrawCharacterAtOffset__battle_party_commands(s32 message, struct Window *window, s32 x,
                                s32 y) __asm__("Text_DrawCharacterAtOffset");
s32 Func_0804a134(s32 unit, u16 *actions, s32 count);
s32 Resource_LoadIndexedEntryToBuffer(s32 id, s32 slot);
void Func_08041f70(s32 color);
/* VIEW(battle_party_commands): routine Text_DrawNumberAtOffset as void Text_DrawNumberAtOffset(s32 value, s32 digits, struct Window *window, s32 x, s32 y); text_render.h has void Text_DrawNumberAtOffset(s32 value, s32 format, struct RenderInput *work, s32 offsetX, s32 offsetY); tried as the definition's void Text_DrawNumberAtOffset(s32 value, s32 width, struct RenderInput *work, s32 x, s32 y), Func_0804bc08 moves: 2 of 1909 instructions in tla-ja. */
extern void Text_DrawNumberAtOffset__battle_party_commands(s32 value, s32 digits, struct Window *window,
                             s32 x, s32 y) __asm__("Text_DrawNumberAtOffset");
/* VIEW(battle_party_commands): routine Window_SetTilemapEntry as void Window_SetTilemapEntry(struct Window *window, s32 entry, s32 x, s32 y, s32 flags); graphics.h has void Window_SetTilemapEntry(struct Window *window, s32 value, s32 x, s32 y, u32 mode); tried as the definition's void Window_SetTilemapEntry(struct Window *window, s32 value, s32 x, s32 y, u32 mode), which the object already declares, Func_0804bc08 moves: 11 of 1909 instructions in tla-ja. */
extern void Window_SetTilemapEntry__battle_party_commands(struct Window *window, s32 entry, s32 x, s32 y,
                            s32 flags) __asm__("Window_SetTilemapEntry");
s32 Resource_ResetEntry(u32 slot);
s32 Func_08049154(s32 unused0, s32 unused1, const u8 *standby);
void Func_0803c274(struct Window *window, s32 character, s32 x, s32 y,
                   s32 mode);
s32 Func_0804868c(s32 member, s32 unused);
s32 Djinn_IsActiveFar(s32 member, s32 element, s32 index);
s32 Djinn_GetDefinitionHeaderFar(s32 element, s32 index);
s32 Func_0804519c(void);
void *Message_ShowAndWait(s32 message, s32 x, s32 y);
s32 Message_AllTyped(void);
s32 Func_08049a30(s32 member, u16 *entries, s32 count);
/* VIEW(battle_party_commands): routine Item_Get as u16 *Item_Get(s32 item); item.h has struct ItemDefinition *Item_Get(s32 item); tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/window/f04a85c.c:2047: subscripted value is neither array nor pointer. */
extern u16 *Item_Get__battle_party_commands(s32 item) __asm__("Item_Get");
s32 Item_ClassifyUseAbility(s32 member, s32 item);
s32 ItemIcon_LoadIntoSlot(s32 item, s32 slot);
u32 Random16(void);
/* VIEW(battle_party_commands): routine Vram_FillWindowBlockPattern as void Vram_FillWindowBlockPattern(void); countdown has unsigned char Vram_FillWindowBlockPattern(void); tried as the definition's void Vram_FillWindowBlockPattern(void), which the object already declares, Func_0804bc08 moves: 1 of 1909 instructions in tla-ja. */
extern void Vram_FillWindowBlockPattern__battle_party_commands(void) __asm__("Vram_FillWindowBlockPattern");
/* VIEW(battle_party_commands): routine Message_CopyExpanded as s32 Message_CopyExpanded(s32 key, u16 *destination, u32 capacity); f04a85c has void Message_CopyExpanded(s32 message, u16 *text, s32 capacity); tried as the definition's s32 Message_CopyExpanded(s32 key, u16 *destination, u32 capacity), it does not compile in tla-ja: games/tla/src/window/f04a85c.c:1419: conflicting types for `Message_CopyExpanded'. */
extern s32 Message_CopyExpanded__battle_party_commands(s32 key, u16 *destination, u32 capacity) __asm__("Message_CopyExpanded");
/* VIEW(battle_party_commands): routine Text_DrawWideStringAtOffset as void Text_DrawWideStringAtOffset(u16 *text, struct Window *window, s32 x, s32 y); f04a85c has void Text_DrawWideStringAtOffset(u16 *text, struct TargetWindow *window, s32 x, s32 y); tried as the definition's void Text_DrawWideStringAtOffset(u16 *text, struct Window *window, s32 x, s32 y), it does not compile in tla-ja: games/tla/src/window/f04a85c.c:1421: conflicting types for `Text_DrawWideStringAtOffset'. */
extern void Text_DrawWideStringAtOffset__battle_party_commands(u16 *text, struct Window *window, s32 x,
                                 s32 y) __asm__("Text_DrawWideStringAtOffset");
void Camera_ConfigureSceneFar(s32 mode);

#define SET_CURSOR(page_, row_, preferred_)                                    \
  do {                                                                         \
    NAVIGATION->page = (page_);                                                \
    NAVIGATION->row = (row_);                                                  \
    NAVIGATION->preferredRow = (preferred_);                                   \
  } while (0)

#define COMMAND_ICON_X(state) ((state)->icons[0].oam.f.x)

/// Collects the party's commands for a turn into `commands`, one for each of
/// the `commandCount` units in `actors`, with two nested functions: the party
/// panels' clear and the link wait's message (Func_0804bbd0). Unlike The Broken
/// Seal's, each round first appends the forced actions of statused members
/// (Func_0804babc), whose count is added to the result; B on the party's row
/// undoes a reserve swap; command 49 swaps a reserve member in (Func_0804b8b8
/// after choosing the slot); Status lists the reserve as well; Flee is command
/// 99; and the Djinn tutorial's hint follows an unleash.
s32 Func_0804bc08(struct BattleAction *commands, u16 *actors,
                  s32 commandCount) {
  u16 signatureHeader[2];
  struct BattleMenuWork *state;
  struct BattleAction *commandsOut;
  s32 forcedCount;
  struct BattleAction *command;
  struct BattleUnit *unit;
  s32 target;
  s32 actorId;
  s32 commandKind;
  s32 parameter;
  s32 largeIconSlot;
  s32 smallIconSlot;
  s32 commandIndex;
  s32 summonIconSlot;
  s32 mode;
  struct Window *commandWindow;

  void Battle_DrawPartyPanelsWithEmptyList(void) {
    u16 data[2];

    data[0] = 255;
    BattleLayout_HighlightPartyPanels__battle_party_commands(data);
    BattleHighlight_SetModesFar(data, 1);
  }

  target = 256;
  // Default: set for real at the top of each round.
  commandsOut = NULL;
  // Default: each is set for real by the row it reaches.
  mode = 0;
  commandKind = 0;
  parameter = 0;
  largeIconSlot = VramCache_AllocEntry(1024);
  smallIconSlot = VramCache_AllocEntry(512);
  state = HeapSlot_AllocEwram(228, 272);
  // Default: set again before each round's forced actions.
  forcedCount = 0;
  Resource_LoadIndexedIntoBuffer(largeIconSlot, 0);
  Graphics_ExpandVramTilesByColorTable((u16 *)0x06006000);
  Link_DrawShiftedTilePair__battle_party_commands(0x06006680);
  Menu_BuildLocalizedPatternTiles();
  state->swappedUnit = -1;
  state->lastTarget = -1;
  {
    struct BattleWork *battle = Ram_HeapSlots->battleWork;
    s32 k;

    for (k = 0; k < 8; k++)
      state->chosenDjinn[k] = 0x80000000;
    for (k = 0; k < 3; k++)
      state->iconShown[k] = 0;
    state->targetViewOffset = 0;
    state->viewOffset = 0;
    state->commandSelection[0] = 0;
    state->commandSelection[1] = 0;
    state->linkTimer = 0;
    state->timerHidden = 0;
    state->timer = -1;
    state->timerWindow = 0;
    if (battle->twoSided != 0) {
      s32 missedLinkFrames;
      u16 *signature;

      state->linkTimer = 1;
      signature = gData_02003a74;
      signature[4] = 'V';
      signature[5] = 'S';
      signature[6] = 'S';
      signature[7] = 'T';
      missedLinkFrames = 0;
      for (;;) {
        u16 *peerSignature = gLinkPeerSignatures__battle_party_commands[battle->linkSide ^ 1];

        if (battle->linkPaused != 0) {
          commandCount = -1;
          state->linkTimer = 0;
          break;
        }
        if ((gLinkStatus & 3) != 3) {
          if (++missedLinkFrames > 24) {
            commandCount = -1;
            state->linkTimer = 0;
            break;
          }
        } else {
          missedLinkFrames = 0;
          if (peerSignature[4] == 'V' && peerSignature[5] == 'S' &&
              peerSignature[6] == 'S' && peerSignature[7] == 'T')
            break;
          if (peerSignature[4] == 'E' && peerSignature[5] == 'D' &&
              peerSignature[6] == 'V' && peerSignature[7] == 'S')
            break;
          if (!(peerSignature[0] == 'E' && peerSignature[1] == 'X' &&
                peerSignature[2] == 'E' && peerSignature[3] == 'C')) {
            commandCount = -1;
            state->linkTimer = 0;
            break;
          }
        }
        wait(1);
      }
    }
  }
  Task_Add(UpdateLinkSessionCountdown, 0x480);

  for (;;) {
    commandsOut = commands;
    if (commandCount >= 0) {
      forcedCount = 0;
      commandCount = Func_0804babc__battle_party_commands(actors, &commandsOut, &forcedCount);
    }
    Battle_DrawPartyPanelsWithEmptyList();
    state->iconShown[2] = 0;
    state->commandActor = -1;
    state->tutorialStep = 0;
    if (GameFlag_Test(0x16e)) {
      state->tutorialStep = 1;
      state->tutorialDelay = 60;
    }
    if (commandCount >= 0)
      mode = Func_080457d0(0, commandCount);
    else
      mode = 14;

    // B on the party's row: undo the last reserve swap.
    if (mode == -1) {
      s32 swapped = state->swappedUnit;
      struct BattleWork *battle;
      s32 old;
      s32 shortSwapped;
      s32 k;

      if (swapped == mode)
        continue;
      battle = Ram_HeapSlots->battleWork;
      old = battle->partyUnits[state->swappedPlace];
      battle->partyUnits[state->swappedPlace] = swapped;
      shortSwapped = (s16)swapped;
      for (k = 0; k < 8; k++) {
        if (gMainRoutineWork__battle_party_commands[0x218 + k] == shortSwapped)
          gMainRoutineWork__battle_party_commands[0x218 + k] = old;
        else if (gMainRoutineWork__battle_party_commands[0x218 + k] == old)
          gMainRoutineWork__battle_party_commands[0x218 + k] = shortSwapped;
      }
      PartyStatus_Draw(Ram_HeapSlots->battleWork->partyStatus);
      BattleActor_CommitPlacementFar();
      state->swappedUnit = -1;
      Func_08118158();
      NAVIGATION->commandSelection[0] = 1;
      continue;
    }
    // Command 49: choose a party place, then the reserve member for it.
    if (mode == 49) {
      Sound_PlayFar(112);
    choose_place:
      mode = Func_0804a85c(0, 2, 1, 8);
      if (mode == -1)
        continue;
      mode = Func_0804b8b8(mode);
      if (mode == -1)
        goto choose_place;
      Func_08118158();
      NAVIGATION->commandSelection[0] = 0;
      continue;
    }
    // Status: the summary screens of the party and the reserve, or in the
    // debug build with Start held the enemies'.
    if (mode == 7) {
      u16 *block = Heap_Alloc(18);
      s32 side;
      s32 count;

      if (gDebugMode != 0 && (gInput.held & 8))
        side = 2;
      else
        side = 1;
      count = BattleParty_ListActorIdsFar__battle_party_commands(side, block);
      if (side == 1)
        count += BattleParty_PrepareReserveOwnersFar(block + count);
      wait(1);
      Func_08046b08(block, count, block[0]);
      Heap_Release(block);
      continue;
    }
    // Flee: one command 99 for the party, first in the round.
    if (mode == 4) {
      s32 refused = Message_ShowLocalizedAndWait();

      if (refused != 0)
        continue;
      commandCount = 1;
      command = commands;
      command->unitId = actors[0];
      forcedCount = refused;
      command->priority = 0x7ffe;
      command->command = 99;
      command->parameter = refused;
      command->target = 256;
      break;
    }
    if (mode != 14)
      break;
    Sound_PlayFar(154);
    commandIndex = 0;
    if (commandIndex >= commandCount)
      break;

    for (;;) {
      if (commandIndex == 0) {
        Func_08118130(0, state->standbyDjinn[0]);
      } else {
        s32 k;

        for (k = 0; k < 4; k++)
          state->standbyDjinn[commandIndex][k] =
              state->standbyDjinn[commandIndex - 1][k];
      }
      command = &commandsOut[commandIndex];
      actorId = actors[commandIndex];
      // UB: Unit_Get returns a pointer, not the cast's doubleword result.
#ifdef UBFIX
      unit = Unit_Get(actorId);
#else
      unit = (struct BattleUnit *)(s32)((s64 (*)(s32))Unit_Get)(actorId);
#endif
      {
        struct BattleMenuWork *navigation = NAVIGATION;
        struct MenuSprite *iconSprite;

        navigation->commandActor = actorId;
        navigation->commandSelection[1] = 0;
        iconSprite = &navigation->icons[2];
        iconSprite->oam.word.attr01 = 0x80000400;
        iconSprite->oam.word.attr23 = 0;
        iconSprite->oam.f.tile = Func_08045528(unit->enemyId, smallIconSlot);
        iconSprite->oam.f.x = ACTOR_ICON_X;
        iconSprite->oam.f.y = 0x80;
        iconSprite->oam.f.palette = 14;
        state->iconShown[2] = 1;
      }

    mark_visible:
      Func_08041b68();
      {
        struct BattleMenuWork *navigation = NAVIGATION;

        navigation->iconShown[0] = 0;
        navigation->chosenDjinn[commandIndex] = 0x80000000;
      }
      signatureHeader[0] = actorId;
      signatureHeader[1] = 255;
      BattleHighlight_SetModesFar(signatureHeader, 1);
      BattleLayout_HighlightPartyPanels__battle_party_commands(signatureHeader);
      wait(1);
      mode = Func_080457d0(1, 0);
      wait(1);
      if (mode == -2) {
        u16 *block = Heap_Alloc(12);
        s32 count = BattleParty_ListActorIdsFar__battle_party_commands(1, block);

        state->iconShown[2] = 0;
        wait(1);
        Func_08046b08(block, count, actorId);
        state->iconShown[2] = 1;
        Heap_Release(block);
        continue;
      }
      BattleHighlight_SetModesFar(signatureHeader, 0);
      if (mode == -1) {
        if (commandIndex == 0)
          break;
        commandIndex--;
        continue;
      }
      {
        struct BattleMenuWork *navigation = NAVIGATION;
        struct MenuSprite *iconSprite = &navigation->icons[0];

        if (navigation->timer == 0)
          mode = 3;
        iconSprite->oam.word.attr23 = 0;
        iconSprite->oam.word.attr01 = 0x80002400;
        iconSprite->oam.f.tile =
            Resource_LoadIndexedIntoBuffer(largeIconSlot, mode);
        if (mode == 15)
          iconSprite->oam.f.x = 128;
        else
          iconSprite->oam.f.x = 96;
        iconSprite->oam.f.y = 136;
        state->iconShown[0] = 1;
      }

      switch (mode) {
      case 0:
        commandWindow = Window_Create__battle_party_commands(11, 17, 11, 3, 6);
        Text_DrawCharacterAtOffset__battle_party_commands((s32)MsgCommandName, commandWindow,
                                   ATTACK_NAME_X, 0);
        COMMAND_ICON_X(state) = 64;
        Sound_PlayFar(112);
        mode = Func_0804a85c(actorId, 1, 1, 0);
        Window_Close(commandWindow, 1);
        if (mode == -1)
          goto mark_visible;
        commandKind = 0;
        target = mode;
        command->range = 1;
        break;

      case 1: {
        s32 k;
        struct MenuSprite *iconSprite;
        struct BattleAct *ability;
        struct WindowWork *work;
        u16 *list;
        u8 *pick;
        s32 cost;
        s32 handle;
        s32 cnt;
        s32 entryValue;

        Sound_PlayFar(112);
        mode = 0;
        SET_CURSOR(mode, mode, mode);
      psynergy_menu:
        list = state->listAbilities;
        pick = state->listSlots;
        COMMAND_ICON_X(state) = PSYNERGY_MENU_ICON_X;
        cnt = 0;
        k = 0;
        entryValue = unit->actionSlots[k].encodedAction & 0x3fff;
        if (entryValue != 0) {
          do {
            if (BattleAction_Get(entryValue)->flags & 0x80) {
              pick[cnt] = k;
              list[cnt] = entryValue;
              cnt++;
            }
            k++;
            if (k == 32)
              break;
            entryValue = unit->actionSlots[k].encodedAction & 0x3fff;
          } while (entryValue != 0);
        }
        pick[cnt] = 0;
        list[cnt] = 0;
        mode = Func_0804a134(actorId, list, cnt);
        if (mode == -1)
          goto mark_visible;
        parameter = unit->actionSlots[pick[mode]].encodedAction & 0x3fff;
        ability = BattleAction_Get(parameter);
        cost = ability->range;
        iconSprite = &state->icons[1];
        handle = VramCache_AllocEntry(128);
        work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
        commandWindow = Window_Create__battle_party_commands(8, 17, 18, 3, 6);
        COMMAND_ICON_X(state) = 40;
        iconSprite->oam.word.attr01 = 0x40000400;
        iconSprite->oam.word.attr23 = 0;
        iconSprite->oam.f.x = (s16)commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = (s16)commandWindow->y * 8 + 4;
        /* FAKEMATCH(barrier): counted as 4 bytes, the empty asm puts the pool
         * after mov r1, r10 (constant 40); without it, 18 of 1907
         * instructions differ in tla-en. */
        __asm__ volatile("");
        iconSprite->oam.f.tile =
            Resource_LoadIndexedEntryToBuffer(parameter, handle);
        state->iconShown[1] = 1;
        work->level = 5;
        if (ability->ppCost > unit->pp)
          Func_08041f70(2);
        else if (unit->psySeal != 0)
          Func_08041f70(9);
        Text_DrawCharacterAtOffset__battle_party_commands(parameter + (s32)MsgAbilityNothing,
                                   commandWindow, 16, 0);
        Text_DrawNumberAtOffset__battle_party_commands(ability->ppCost, 2, commandWindow, 104, 0);
        work->level = 15;
        Func_08041f70(15);
        Window_SetTilemapEntry__battle_party_commands(commandWindow, WINDOW_TILE_PP_LABEL, 11, 0, 0);
        Window_SetTilemapEntry__battle_party_commands(commandWindow, WINDOW_TILE_PP_LABEL_END, 12, 0,
                               0);
        if (ability->element != 4)
          Window_SetTilemapEntry__battle_party_commands(commandWindow, ability->element + 0x5001, 15,
                                 0, 0);
        command->range = cost;
        Sound_PlayFar(112);
        mode = Func_0804a85c(actorId, ability->targetMode, cost,
                             Battle_ClassifyEntryKind(ability));
        state->iconShown[1] = 0;
        Resource_ResetEntry(handle);
        Window_Close(commandWindow, 1);
        if (mode == -1)
          goto psynergy_menu;
        commandKind = 1;
        target = mode;
        break;
      }

      case 16: {
        const struct SummonDefinition *summon;
        struct BattleAct *ability;
        struct MenuSprite *iconSprite;
        s32 ok;
        s32 j;
        s32 n;

        Sound_PlayFar(112);
        SET_CURSOR(0, 0, 0);
      summon_menu:
        COMMAND_ICON_X(state) = SUMMON_MENU_ICON_X;
        BattleLayout_HighlightPartyPanels__battle_party_commands(signatureHeader);
        mode = Func_08049154(0, 0, state->standbyDjinn[commandIndex]);
        BattleLayout_HighlightPartyPanels__battle_party_commands(signatureHeader);
        if (mode == -1)
          goto mark_visible;
        commandKind = 6;
        parameter = mode;
        wait(1);
        iconSprite = &state->icons[1];
        summon = SummonDefinition_Get(mode);
        ability = BattleAction_Get(summon->nameMessageId);
        summonIconSlot = VramCache_AllocEntry(128);
        commandWindow = Window_Create__battle_party_commands(SUMMON_WINDOW_X, 17, SUMMON_WINDOW_WIDTH,
                                      3, commandKind);
        n = 0;
        for (j = 0; j <= 3; j++) {
          if (summon->djinnRequired[j] > state->standbyDjinn[commandIndex][j])
            break;
        }
        ok = j == 4;
        COMMAND_ICON_X(state) = SUMMON_CHOSEN_ICON_X;
        // Written through a u32 pointer, not the union: these stores then no
        // longer conflict with the load of state, which sched2 needs to order
        // eors first.
        ((u32 *)&iconSprite->oam)[0] = 0x40000400;
        ((u32 *)&iconSprite->oam)[1] = 0;
        /* FAKEMATCH(barrier): counted as 4 bytes, the empty asm puts the pool
         * after mov r1, r11 (constant 56); without it, 22 of 1907
         * instructions differ in tla-en. */
        __asm__ volatile("");
        iconSprite->oam.f.x = (s16)commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = (s16)commandWindow->y * 8 + 4;
        iconSprite->oam.f.tile = Resource_LoadIndexedEntryToBuffer(
            summon->nameMessageId & 0x3fff, summonIconSlot);
        state->iconShown[1] = 1;
        if (ok == 0)
          Func_08041f70(2);
        Text_DrawCharacterAtOffset__battle_party_commands(SummonDefinition_Get(mode)->nameMessageId +
                                       (s32)MsgAbilityNothing,
                                   commandWindow, 16, 0);
        for (j = 0; j <= 3; j++) {
          if (summon->djinnRequired[j] != 0) {
            Window_SetTilemapEntry__battle_party_commands(commandWindow, j + 0x5001,
                                   n * 2 + SUMMON_COST_X, 0, 0);
            Func_0803c274(commandWindow, summon->djinnRequired[j] + '0',
                          n * 2 + SUMMON_COST_X + 1, 0, 0);
            n++;
          }
        }
        if (ok != 0)
          Sound_PlayFar(112);
        else
          Sound_PlayFar(114);
        mode = Func_0804a85c(actorId, ability->targetMode, ability->range,
                             Battle_ClassifyEntryKind(ability));
        command->range = ability->range;
        state->iconShown[1] = 0;
        Resource_ResetEntry(summonIconSlot);
        Window_Close(commandWindow, 1);
        if (mode == -1)
          goto summon_menu;
        for (j = 0; j <= 3; j++) {
          if (summon->djinnRequired[j] > state->standbyDjinn[commandIndex][j]) {
            state->standbyDjinn[commandIndex][j] = 0;
            break;
          }
          state->standbyDjinn[commandIndex][j] -= summon->djinnRequired[j];
        }
        target = mode;
        break;
      }

      case 15: {
        struct BattleAct *ability;
        s32 cost;
        s32 element;
        s32 index;

        Sound_PlayFar(112);
        SET_CURSOR(0, 0, 0);
      djinn_menu:
        COMMAND_ICON_X(state) = DJINN_MENU_ICON_X;
        NAVIGATION->chosenDjinn[commandIndex] = 0x80000000;
        mode = Func_0804868c(actorId, 1);
        command->range = 0;
        if (mode == -1)
          goto mark_visible;
        parameter = mode;
        commandKind = 5;
        NAVIGATION->chosenDjinn[commandIndex] = mode;
        element = (parameter >> 8) & 15;
        index = parameter & 255;
        if (Djinn_IsActiveFar(actorId, element, index) != 0) {
          ability =
              BattleAction_Get(Djinn_GetDefinitionHeaderFar(element, index));
          cost = ability->range;
          commandWindow = Window_Create__battle_party_commands(11, 17, 10, 3, 6);
          COMMAND_ICON_X(state) = 64;
          Window_SetTilemapEntry__battle_party_commands(commandWindow, element + 0x5001, 0, 0, 0);
          Text_DrawCharacterAtOffset__battle_party_commands(element * 20 + index + (s32)MsgDjinnName,
                                     commandWindow, 16, 0);
          command->range = cost;
          wait(1);
          Sound_PlayFar(112);
          mode = Func_0804a85c(actorId, ability->targetMode, cost,
                               Battle_ClassifyEntryKind(ability));
          if (state->tutorialStep != 0) {
            void *hint = Message_ShowAndWait(Func_0804519c() + 14, 15, 8);

            while (Message_AllTyped() == 0)
              wait(1);
            Window_Close(hint, 1);
            state->tutorialStep++;
            state->tutorialDelay = 45;
          }
          Window_Close(commandWindow, 1);
          if (mode == -1)
            goto djinn_menu;
          target = mode;
          break;
        }
        commandWindow = Window_Create__battle_party_commands(11, 17, 10, 3, 6);
        COMMAND_ICON_X(state) = 64;
        Func_08041f70(2);
        Window_SetTilemapEntry__battle_party_commands(commandWindow, element + 0x5001, 0, 0, 0);
        Text_DrawCharacterAtOffset__battle_party_commands(element * 20 + index + (s32)MsgDjinnName,
                                   commandWindow, 16, 0);
        Func_08041f70(15);
        command->range = 1;
        wait(1);
        Sound_PlayFar(112);
        mode = Func_0804a85c(actorId, 4, 0, 7);
        Window_Close(commandWindow, 1);
        if (mode == -1)
          goto djinn_menu;
        target = mode;
        break;
      }

      case 2: {
        s32 k;
        struct MenuSprite *iconSprite;
        struct BattleAct *ability;
        u16 *list;
        u8 *pick;
        s32 cost;
        s32 handle;
        s32 cnt;
        s32 entryValue;
        s32 selection;

        Sound_PlayFar(112);
        mode = 0;
        SET_CURSOR(mode, mode, mode);
      item_menu:
        COMMAND_ICON_X(state) = 96;
        pick = state->listSlots;
        list = state->listAbilities;
        cnt = 0;
        k = 0;
        entryValue = unit->inventory[k];
        if (entryValue != 0) {
          do {
            Item_Get__battle_party_commands(entryValue);
            if (Item_ClassifyUseAbility(actorId, entryValue) == 0) {
              list[cnt] = entryValue;
              pick[cnt++] = k;
            }
            k++;
            if (k == 15)
              break;
            entryValue = unit->inventory[k];
          } while (entryValue != 0);
        }
        k = 0;
        entryValue = unit->inventory[k];
        if (entryValue != 0) {
          do {
            Item_Get__battle_party_commands(entryValue);
            if (Item_ClassifyUseAbility(actorId, entryValue) != 0) {
              list[cnt] = entryValue;
              pick[cnt++] = k;
            }
            k++;
            if (k == 15)
              break;
            entryValue = unit->inventory[k];
          } while (entryValue != 0);
        }
        list[cnt] = 0;
        mode = Func_08049a30(actorId, list, cnt);
        if (mode == -1)
          goto mark_visible;
        selection = pick[mode];
        parameter = selection;
        ability = BattleAction_Get(Item_Get__battle_party_commands(unit->inventory[selection])[20]);
        cost = ability->range;
        iconSprite = &state->icons[1];
        handle = VramCache_AllocEntry(128);
        commandWindow = Window_Create__battle_party_commands(9, 17, ITEM_WINDOW_WIDTH, 3, 6);
        COMMAND_ICON_X(state) = 48;
        iconSprite->oam.word.attr01 = 0x40000400;
        /* FAKEMATCH(barrier): counted as 4 bytes, the empty asm puts the pool
         * after strb r3, [r5, #4] (constant 48); without it, 12 of 1907
         * instructions differ in tla-en. */
        __asm__ volatile("");
        iconSprite->oam.word.attr23 = 0;
        iconSprite->oam.f.x = (s16)commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = (s16)commandWindow->y * 8 + 4;
        iconSprite->oam.f.tile =
            ItemIcon_LoadIntoSlot(unit->inventory[selection], handle);
        state->iconShown[1] = 1;
        Text_DrawCharacterAtOffset__battle_party_commands((unit->inventory[selection] & 0x1ff) +
                                       (s32)MsgItemName,
                                   commandWindow, 24, 0);
        command->range = cost;
        Sound_PlayFar(112);
        mode = Func_0804a85c(actorId, ability->targetMode, cost,
                             Battle_ClassifyEntryKind(ability));
        state->iconShown[1] = 0;
        Resource_ResetEntry(handle);
        Window_Close(commandWindow, 1);
        if (mode == -1)
          goto item_menu;
        commandKind = 2;
        target = mode;
        break;
      }

      case 3:
        commandKind = 3;
        break;
      }

      Sound_PlayFar(110);
      command->unitId = actorId;
      {
        s32 priority = unit->agi;

        if (priority != 0)
          priority += (u32)(unit->agi * Random16()) >> 20;
        command->priority = priority;
        if (commandIndex != 0 &&
            actors[commandIndex] == actors[commandIndex - 1])
          command->priority = (s16)priority / 2;
      }
      if (command->priority < 0)
        command->priority = 2000;
      command->command = commandKind;
      command->parameter = parameter;
      command->target = target;
      commandIndex++;
      if (commandIndex < commandCount)
        continue;
      break;
    }
    if (commandIndex < commandCount)
      continue;
    break;
  }

  if (state->linkTimer != 0) {
    u16 *signature = gData_02003a74;

    signature[4] = 'E';
    signature[5] = 'D';
    signature[6] = 'V';
    signature[7] = 'S';
  }
  if (state->timerWindow != NULL)
    Window_Close(state->timerWindow, 1);
  Resource_ResetEntry(smallIconSlot);
  Resource_ResetEntry(largeIconSlot);
  Task_Remove(UpdateLinkSessionCountdown);
  if (state->linkTimer != 0) {
    struct BattleWork *link;

    void Func_0804bbd0(void) {
      u16 text[64];

#if LANGUAGE != LANGUAGE_JAPANESE
      Message_CopyExpanded__battle_party_commands((s32)MsgWaitingForOpponent, text, 52);
#else
      Message_DecodeRaw__battle_party_commands((s32)MsgWaitingForOpponent, text, 32);
#endif
      Text_DrawWideStringAtOffset__battle_party_commands(text, state->timerWindow, 0, 4);
    }

    link = Ram_HeapSlots->battleWork;
    mode = 0;
    if (state->timerWindow == NULL && link->linkPaused == 0) {
      state->timerWindow = Window_Create__battle_party_commands(0, 16, 30, 4, 42);
      Vram_FillWindowBlockPattern__battle_party_commands();
      Func_0804bbd0();
    } else {
      state->timerWindow = NULL;
    }
    for (;;) {
      u16 *peerSignature = gLinkPeerSignatures__battle_party_commands[link->linkSide ^ 1];

      if (link->linkPaused != 0) {
        commandCount = -1;
        break;
      }
      if ((gLinkStatus & 3) != 3) {
        mode++;
        if (mode > 24) {
          commandCount = -1;
          break;
        }
      } else {
        mode = 0;
        if (peerSignature[4] == 'E' && peerSignature[5] == 'D' &&
            peerSignature[6] == 'V' && peerSignature[7] == 'S')
          break;
        if (!(peerSignature[4] == 'V' && peerSignature[5] == 'S' &&
              peerSignature[6] == 'S' && peerSignature[7] == 'T'))
          mode = 1;
      }
      wait(1);
    }
    if (state->timerWindow != NULL)
      Window_Close(state->timerWindow, 1);
  }
  Camera_ConfigureSceneFar(0);
  HeapSlot_Release(228);
  if (commandCount >= 0)
    commandCount += forcedCount;
  return commandCount;
}

// The command icon menu's frame task (The Broken Seal's IconMenu_DrawIcons):
// pulses the selected icon with one affine matrix, draws the others plain,
// and while the screen transition runs widens window 1 over the icon row.
#include "resource_menu.h"
#include "display_transition.h"
#include "vram_tab.h"

// games/tla/include/system/affine.h's transform, written out: the stale
// affine.h beside these drafts would shadow it.
struct AffineTransform {
  u16 scaleX;
  u16 scaleY;
  u16 angle;
  u16 unk06;
};


extern const u16 gWindowSprite_BounceScales[];

struct WindowLine {
  u16 left;
  u16 span;
};

struct WindowBuffer {
  u8 unk000[4];
  struct WindowLine lines[160];
};

// The transition's byte after its fields; while it is set the window lines
// are left alone.
#define TRANSITION_BUSY(state) (((s8 *)(state))[0x53f])

void Func_0804cda8(void) {
  struct ResourceMenuWork *work;
  struct ResourceMenuEntry *sprite;
  struct AffineTransform effect;
  s32 scale;
  s32 matrix;
  s32 edge;
  s32 y;
  u32 end;
  u32 i;
  u16 *line;
  struct ScreenTransitionState *buffers;
  struct WindowBuffer *window;
  s32 span;

  work = Ram_HeapSlots->menuSelectWork;
  sprite = work->entries;
  scale = gWindowSprite_BounceScales[(gFrameCount * 2) & 31];
  buffers = Ram_HeapSlots->screenTransition;
  scale = (scale - 256) / 4 + 304;
  effect.scaleX = scale;
  effect.scaleY = scale;
  effect.angle = 0;
  matrix = AffineMatrix_BuildForEffect(&effect);
  for (i = 0; i < work->count; i++, sprite++) {
    u32 *attr;
    s32 slot;
    s32 px;

    px = sprite->x;
    if (px == 0)
      continue;
    if (i == work->selection) {
      s32 left = px + scale * 7 / 512 - 20;

      if (sprite->y != 0)
        y = sprite->y + scale * 3 / 256 - 20;
      else
        y = (scale * 15 / 256 - 30) & 255;
      attr = &sprite->link;
      *attr++ = 0;
      *attr++ = (matrix << 25) | y | (left << 16) | 0x80002300;
      *attr = gResourceTableEntries[sprite->slot].offset >> 5;
      slot = 246;
    } else {
      u32 base;
      u32 value;

      attr = &sprite->link;
      y = sprite->y;
      *attr++ = 0;
      value = y | (px << 16) | 0x80002000;
      base = gResourceTableEntries[sprite->slot].offset;
      *attr++ = value;
      *attr = base >> 5;
      slot = 245;
    }
    Oam_AddSpriteAtDepth(sprite, slot);
  }
  if (buffers == 0 || TRANSITION_BUSY(buffers) != 0 || work->count == 0)
    return;
  window = (struct WindowBuffer *)buffers->pages[buffers->page];
  if (work->row == 0) {
    edge = work->entries[work->selection].x;
    span = (scale * 12 - 2817) / 256;
    edge = ((edge - span) << 8) + (edge + span) + 23;
    end = work->entries[work->selection].y + 24;
    end += (scale * 32 - 7937) / 512 + 1;
    line = &window->lines[24].span;
    i = 24;
    if (i < end) {
      do {
        *line = (*line & 0xff) | edge;
        i++;
        line += 2;
      } while (i < end);
    }
    edge = work->entries[0].x;
    if (work->selection == 0)
      edge -= (scale * 12 - 2817) / 256;
    edge <<= 8;
    line = &window->lines[0].span;
    for (i = 0; i <= 23; i++) {
      *line = (*line & 0xff) | edge;
      line += 2;
    }
  } else {
    s32 start;

    edge = work->entries[work->selection].x;
    span = (scale * 12 - 2817) / 256;
    edge = ((edge - span) << 8) + (edge + span) + 23;
    start = work->entries[work->selection].y;
    start -= (scale * 32 - 7937) / 512 + 1;
    line = &window->lines[start].span;
    i = start;
    if (i <= 135) {
      do {
        *line = (*line & 0xff) | edge;
        i++;
        line += 2;
      } while (i <= 135);
    }
    edge = work->entries[0].x;
    if (work->selection == 0)
      edge -= (scale * 12 - 2817) / 256;
    edge <<= 8;
    line = &window->lines[136].span;
    for (i = 136; i <= 159; i++) {
      // start is reused for the mask; as (*line & 0xff), 194 of 386
      // instructions differ in tla-en.
      start = 0xff;
      *line = (*line & start) | edge;
      line += 2;
    }
  }
}
#undef SUMMON_WINDOW_X
#undef SUMMON_WINDOW_WIDTH
#undef SUMMON_COST_X
#undef ITEM_WINDOW_WIDTH
#undef ACTOR_ICON_X
#undef SUMMON_MENU_ICON_X
#undef SUMMON_CHOSEN_ICON_X
#undef DJINN_MENU_ICON_X
#undef ATTACK_NAME_X
#undef PSYNERGY_MENU_ICON_X
#undef NAVIGATION
#undef SET_CURSOR
#undef COMMAND_ICON_X
#undef TRANSITION_BUSY

// ---- games/tla/src/window/resource_work.c ----
#include "heap_release.h"
#include "dma.h"
#include "resource.h"
#include "uiflow.h"

#define AFFINE_EFFECT_SLOT 232
void Func_0804cda8(void);

void *AffineEffect_InitializeWork(void) {
  void *work = HeapSlot_AllocEwram(AFFINE_EFFECT_SLOT, 152);

  Dma_Fill32(0, work, 152);
  Task_Add((TaskFunc)Func_0804cda8, TASK_PASS_VBLANK - 10);
  return work;
}

void Menu_EndResourceSelection(void) {
  struct Window *child;
  s32 i;
  u16 *entry;
  void *work;

  work = Ram_HeapSlots->menuSelectWork;
  Task_Remove(Func_0804cda8);
  child = FIELD_AT_OFFSET(work, struct Window **, 0x78);
  if (child != 0) {
    Window_Close(child, 2);
  }
  i = 0;
  while (i < (s32)FIELD_AT_OFFSET(work, s16 *, 0x8E)) {
    entry = (u16 *)((u8 *)work + 0x12) + i * 10;
    Resource_ResetEntry(*entry);
    i += 1;
  }
  HeapSlot_Release(232);
  wait(1U);
}

#include "text/msg_ids.h"
#include "text_render.h"

void Sound_PlayFar(s32 cue);
void WindowSprite_PrepareForRedraw(struct Window *);

s32 Menu_RunResourceSelectionLoop(s32 initial) {
  struct ResourceMenuWork *work = Ram_HeapSlots->menuSelectWork;
  s32 resource;

  work->selection = initial;
redraw:
  WindowSprite_PrepareForRedraw(work->window);
  if (work->resourceBase != 0)
    resource = work->resourceBase + work->selection;
  else
    resource = work->resourceIds[work->selection] + (s32)MsgCommandName;
  Text_DrawCharacterAtOffset(resource, work->window, 0, 0);
  for (;;) {
    wait(1);
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(112);
      return work->selection;
    }
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(113);
      return -1;
    }
    if (gInput.pressed & KEY_START) {
      Sound_PlayFar(113);
      return -1;
    }
    if ((gInput.repeat & KEY_LEFT) || (gInput.repeat & KEY_UP)) {
      Sound_PlayFar(111);
      work->selection--;
      if (work->selection < 0)
        work->selection = work->count - 1;
    } else if ((gInput.repeat & KEY_RIGHT) || (gInput.repeat & KEY_DOWN)) {
      Sound_PlayFar(111);
      work->selection++;
      if (work->selection >= work->count)
        work->selection = 0;
    } else
      continue;
    goto redraw;
  }
}

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"

/// The menu sounds.
// Named as The Broken Seal's sounds.h names them (SE_MENU_*).
#define SOUND_MENU_CURSOR_MOVE 111
#define SOUND_MENU_CONFIRM 112

struct MenuSelectionState {
  u8 padding000[0x78];
  void *work;
  u8 padding07c[8];
  u8 resourceIds[8];
  s16 selection;
  s16 itemCount;
  s16 field090;
  s16 resourceBase;
};

extern u8 gMenu_SelectionStepDelays[];

static inline s32 AbsoluteDifference(s32 diff, s32 lhs, s32 rhs) {
  if (diff >= 0)
    return diff;
  return rhs - lhs;
}

/// Steps the selection from start to goal, redrawing the label each step and
/// slowing as it nears the goal, then confirms.
// ☀️'s function of the same name in window/battle_command.c.
s32 Menu_SelectResource(s32 start, s32 goal) {
  s16 resourceBase;
  s16 cur;
  s32 diff;
  s32 resourceId;
  s32 dist;
  s32 pos;
  s32 step;
  s32 delay;
  const u8 *table;
  struct MenuSelectionState *state;

  state = Ram_HeapSlots->menuSelectWork;
  step = 1;
  delay = 12;
  state->selection = (s16)start;
  if (goal < start)
    step = -1;
  pos = start;

  for (;;) {
    WindowSprite_PrepareForRedraw(state->work);
    resourceBase = state->resourceBase;
    if (resourceBase != 0) {
      resourceId = resourceBase + state->selection;
    } else {
      resourceId = state->resourceIds[state->selection] + (s32)MsgCommandName;
    }
    Text_DrawCharacterAtOffset(resourceId, state->work, 0, 0);

    cur = state->selection;
    table = gMenu_SelectionStepDelays;
    diff = cur - goal;
    dist = AbsoluteDifference(diff, cur, goal);
    wait(table[dist] + delay);

    if (pos == goal)
      break;

    state->selection = (s16)((u16)state->selection + step);
    Sound_PlayFar(SOUND_MENU_CURSOR_MOVE);
    delay = 0;
    pos += step;
  }

  wait(48);
  Sound_PlayFar(SOUND_MENU_CONFIRM);
  return goal;
}

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(void *, void *))Resource_DecodeByteLzInRam)

extern char ResourceId_CommandIcons;

void Menu_LoadResourceSlot(s32 slot, s32 index) {
  s32 size = 1024;
  void *buffer = (void *)Heap_Alloc(size);
  u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

  Resource_DecodeByteLzInRam((void *)((u32)base + base[index]), buffer);
  VramCache_Load(slot, size, buffer);
  Heap_Release(buffer);
}

void Menu_AppendResourceEntry(s32 no) {
  struct ResourceMenuWork *work;
  struct ResourceMenuEntry *entry;
  s16 index;
  s32 slot;
  s32 flags;

  work = Ram_HeapSlots->menuSelectWork;
  index = work->count;
  if (index <= 5) {
    work->count++;
    entry = &work->entries[index];
    slot = Resource_FindFreeEntry();
    Menu_LoadResourceSlot(slot, no);
    entry->x = index * 24 + 32;
    flags = 136;
    entry->y = flags;
    entry->slot = slot;
    work->resourceIds[index] = no;
  }
}

void Menu_CenterResourceEntries(s32 row, s32 width, s32 resourceBase) {
  struct ResourceMenuWork *menu = Ram_HeapSlots->menuSelectWork;
  s32 x;
  s32 i;
  s32 count;

  width += 2;
  menu->width = width;
  menu->resourceBase = resourceBase;
  menu->row = row;
  count = menu->count;
  x = 15 - (count * 3 + menu->width * 2 / 3) / 2;
  for (i = 0; i < menu->count; i++) {
    struct ResourceMenuEntry *entry = &menu->entries[i];

    entry->x = x * 8;
    entry->y = row * 8;
    x += 3;
  }
  menu->window = Window_Create(x, row, menu->width, 3, 2);
}

#include "tla_edition.h"


/// Lays the entries out in a row three cells apart from x, on row y, and
/// opens the menu's window after them.
void Menu_LayoutResourceEntries(s32 x, s32 y, s32 width, s32 resourceBase) {
  struct ResourceMenuWork *work;
  s32 i;
  struct ResourceMenuEntry *entry;
  s16 *widthp;

  work = Ram_HeapSlots->menuSelectWork;
  widthp = &work->width;
  *widthp = width + 2;
  work->resourceBase = resourceBase;
  work->row = y;
  for (i = 0; i < work->count; i++) {
    entry = &work->entries[i];
    entry->y = y << 3;
    entry->x = x << 3;
    x += 3;
  }
  work->window = Window_Create(x, y, *widthp, 3, 2);
}

static __inline__ s32 TableGet(s8 *table, s32 index) { return table[index]; }

s32 Menu_SelectTopEntry(s32 selection) {
  s32 group = 0;
  s32 offset;
  s32 triple;
  s32 ret;
  s8 *table;

  if (Party_SumDjinnCountsFar(-1) == 0) {
    group = 1;
  }

  triple = group * 3;
  table = gMenu_TopEntryPositionByCommand;
  offset = triple << 1;
  selection = TableGet(table, selection + offset) - 1;
  if (selection < 0) {
    selection = 0;
  }

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(1);
  if (group == 0) {
    Menu_AppendResourceEntry(15);
  }
  Menu_AppendResourceEntry(2);
  Menu_AppendResourceEntry(7);
  Menu_CenterResourceEntries(17, SELECT_MENU_WIDTH, 0);
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();

  if (ret >= 0) {
    s8 *commands = gMenu_TopEntryCommandByPosition;
    s32 index;

    index = ret + offset + 1;
    ret = commands[index];
  }

  return ret;
}

s32 Menu_AnimateSelectionToEntry(s32 arg0, s32 arg1) {
  PartyStatus_OpenAndWait();
  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(1);
  Menu_AppendResourceEntry(0xF);
  Menu_AppendResourceEntry(2);
  Menu_AppendResourceEntry(7);
  // Only Japanese centres the entries before selecting.
#if LANGUAGE == LANGUAGE_JAPANESE
  Menu_CenterResourceEntries(17, SELECT_MENU_WIDTH, 0);
#endif
  arg1 = Menu_SelectResource(arg0, arg1 - 1);
  Menu_EndResourceSelection();
  Window_CloseAndRelease();
  return arg1;
}

extern s16 gData_020036d8;
extern s16 gData_02005350;
extern s8 gMenu_SaveSlotActionByPosition[];

s32 Func_080432a4(void);

/// Offers the actions the files allow (plus two optional entries
/// behind EWRAM flags) and maps the chosen position to an action. The second
/// optional entry is 31, and with the first option off, action 4 becomes 5.
// As ☀️'s Menu_SelectSaveSlotAction. A module of its own: in the Japanese
// edition it follows Menu_AnimateSelectionToEntry, which is still scaffolded
// there.
s32 Menu_SelectSaveSlotAction(void) {
  s32 type;
  s32 ret;
  s32 initial;
  u32 group;

  group = 0;
  initial = 0;
  type = Func_080432a4();
  if (type < 0)
    return -1;
  if (type == 0)
    return 0;
  if (type == 3)
    group = 1;
  else if (type == 103)
    group = 2;
  else if (type > 100)
    group = 3;
  else
    initial = 1;
  AffineEffect_InitializeWork();
  if (group == 0 || group == 3)
    Menu_AppendResourceEntry(21);
  if (group <= 1)
    Menu_AppendResourceEntry(22);
  if (group == 0 || group == 3)
    Menu_AppendResourceEntry(23);
  Menu_AppendResourceEntry(24);
  if (gData_020036d8 != 0)
    Menu_AppendResourceEntry(29);
  if (gData_02005350 != 0)
    Menu_AppendResourceEntry(31);
  Menu_CenterResourceEntries(17, SAVE_SLOT_MENU_WIDTH, 0);
  ret = Menu_RunResourceSelectionLoop(initial);
  Menu_EndResourceSelection();
  if (ret >= 0) {
    u32 twice = group * 2;

    ret = gMenu_SaveSlotActionByPosition[ret + (twice + group) * 2];
  }
  if (gData_020036d8 == 0 && ret == 4)
    ret = 5;
  return ret;
}

// The label area is narrower in Japanese and wider in French.
#if LANGUAGE == LANGUAGE_JAPANESE
#define LABEL_RIGHT 80
#elif LANGUAGE == LANGUAGE_FRENCH
#define LABEL_RIGHT 160
#else
#define LABEL_RIGHT 128
#endif

struct MenuModeLabelState {
  u8 unk00[0x7c];
  void *window;
  u8 unk80[0x0c];
  s16 mode;
  u8 unk8e[8];
  s16 previousMode;
};

void Window_ClearInteriorTiles(void *window, s32 x, s32 y, s32 right,
                               s32 bottom);

/// When the selected mode changes, clears the lower window's label area and
/// writes the mode's lines of text. The area starts at x 8 and ends at
/// LABEL_RIGHT; a mode past 2 draws nothing.
// ☀️'s function of the same name.
void Menu_DrawModeLabel(void) {
  struct MenuModeLabelState *state = Ram_HeapSlots->menuSelectWork;

  if (state->previousMode != state->mode) {
    state->previousMode = state->mode;
    Window_ClearInteriorTiles(state->window, 8, 40, LABEL_RIGHT, 80);

    switch (state->mode) {
    case 0: {
      s32 text = (s32)MsgPasswordLevel;

      Text_DrawCharacterAtOffset(text, state->window, 8, 40);
      Text_DrawCharacterAtOffset(text + 1, state->window, 8, 48);
      Text_DrawCharacterAtOffset(text + 2, state->window, 8, 56);
      Text_DrawCharacterAtOffset(text + 3, state->window, 8, 64);
      text += 4;
      Text_DrawCharacterAtOffset(text, state->window, 8, 72);
      break;
    }
    case 1: {
      s32 text = (s32)MsgPasswordLevel;

      Text_DrawCharacterAtOffset(text, state->window, 8, 40);
      Text_DrawCharacterAtOffset(text + 1, state->window, 8, 48);
      text += 2;
      Text_DrawCharacterAtOffset(text, state->window, 8, 56);
      break;
    }
    case 2: {
      s32 text = (s32)MsgPasswordLevel;

      Text_DrawCharacterAtOffset(text++, state->window, 8, 40);
      Text_DrawCharacterAtOffset(text, state->window, 8, 48);
      break;
    }
    }
  }
}

void Text_DrawMessage(s32 message, void *window, s32 x, s32 y);

// The Spanish descriptions are indented.
#if LANGUAGE == LANGUAGE_SPANISH
#define HELP_X 6
#else
#define HELP_X 0
#endif

/// Redraws the selected method's description in the lower window. Spanish,
/// French and Italian describe two methods, the password and everything else
/// as the cable; French clears the window with WindowSprite_ClearList and
/// redraws only when the description changes.
void Menu_DrawModeIndicator(void) {
  struct ResourceMenuWork *state = Ram_HeapSlots->menuSelectWork;
  s16 *shown = &state->previousMode;
  s16 *current = &state->selection;

#if LANGUAGE == LANGUAGE_FRENCH
  if (*shown != *current) {
    if (*current == 0) {
      WindowSprite_ClearList(state->lowerWindow);
      Text_DrawMessage((s32)MsgPasswordTransferHelp, state->lowerWindow, 8, 4);
      Text_DrawMessage((s32)MsgPasswordTransferHelp + 1, state->lowerWindow, 0,
                       16);
    } else if (*shown == 0) {
      WindowSprite_ClearList(state->lowerWindow);
      Text_DrawMessage((s32)MsgCableTransferHelp, state->lowerWindow, 0, 4);
      Text_DrawMessage((s32)MsgCableTransferHelp + 1, state->lowerWindow, 0,
                       16);
      Text_DrawMessage((s32)MsgCableTransferHelp + 2, state->lowerWindow, 0,
                       28);
    }
    // Through the pointers, the selection would be loaded once at the top.
    state->previousMode = state->selection;
  }
#else
  if (*shown != *current) {
    *shown = *current;
    WindowSprite_PrepareForRedraw(state->lowerWindow);
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
    if (*current == 0) {
      Text_DrawMessage((s32)MsgPasswordTransferHelp, state->lowerWindow, HELP_X,
                       4);
      Text_DrawMessage((s32)MsgPasswordTransferHelp + 1, state->lowerWindow,
                       HELP_X, 16);
    } else {
      Text_DrawMessage((s32)MsgCableTransferHelp, state->lowerWindow, HELP_X,
                       4);
      Text_DrawMessage((s32)MsgCableTransferHelp + 1, state->lowerWindow,
                       HELP_X, 16);
      Text_DrawMessage((s32)MsgCableTransferHelp + 2, state->lowerWindow,
                       HELP_X, 28);
    }
#else
    switch (*current) {
    case 0:
      Text_DrawMessage((s32)MsgPasswordTransferHelp, state->lowerWindow, 0, 4);
      Text_DrawMessage((s32)MsgPasswordTransferHelp + 1, state->lowerWindow, 0,
                       16);
      break;
    case 1:
      Text_DrawMessage((s32)MsgCableTransferHelp, state->lowerWindow, 0, 4);
      Text_DrawMessage((s32)MsgCableTransferHelp + 1, state->lowerWindow, 0,
                       16);
      Text_DrawMessage((s32)MsgCableTransferHelp + 2, state->lowerWindow, 0,
                       28);
      break;
    default:
      Text_DrawMessage((s32)MsgClearDataTransferHelp, state->lowerWindow, 0, 4);
      Text_DrawMessage((s32)MsgClearDataTransferHelp + 1, state->lowerWindow, 0,
                       16);
      break;
    }
#endif
  }
#endif
}

#include "minigame.h"

// The entries' width and the windows' places and widths, in tiles, and where
// the lines of text start, in pixels, are fitted to each language's text.
#if LANGUAGE == LANGUAGE_JAPANESE
#define ENTRY_WIDTH 5
#define PASSWORD_UPPER_X 9
#define PASSWORD_UPPER_WIDTH 13
#define PASSWORD_LOWER_X 5
#define PASSWORD_LOWER_WIDTH 21
#define TRANSFER_UPPER_X 9
#define TRANSFER_UPPER_WIDTH 13
#define TRANSFER_TITLE_X 0
#define TRANSFER_LOWER_X 5
#define TRANSFER_LOWER_WIDTH 21
#elif LANGUAGE == LANGUAGE_GERMAN
#define ENTRY_WIDTH 7
#define PASSWORD_UPPER_X 7
#define PASSWORD_UPPER_WIDTH 16
#define PASSWORD_LOWER_X 4
#define PASSWORD_LOWER_WIDTH 22
#define TRANSFER_UPPER_X 6
#define TRANSFER_UPPER_WIDTH 18
#define TRANSFER_TITLE_X 6
#define TRANSFER_LOWER_X 2
#define TRANSFER_LOWER_WIDTH 26
#elif LANGUAGE == LANGUAGE_SPANISH
#define ENTRY_WIDTH 9
#define PASSWORD_UPPER_X 5
#define PASSWORD_UPPER_WIDTH 21
#define PASSWORD_LOWER_X 3
#define PASSWORD_LOWER_WIDTH 25
#define TRANSFER_UPPER_X 4
#define TRANSFER_UPPER_WIDTH 22
#define TRANSFER_TITLE_X 2
#define TRANSFER_LOWER_X 1
#define TRANSFER_LOWER_WIDTH 28
#elif LANGUAGE == LANGUAGE_FRENCH
#define ENTRY_WIDTH 9
#define PASSWORD_UPPER_X 5
#define PASSWORD_UPPER_WIDTH 20
#define PASSWORD_LOWER_X 3
#define PASSWORD_LOWER_WIDTH 25
#define TRANSFER_UPPER_X 5
#define TRANSFER_UPPER_WIDTH 20
#define TRANSFER_TITLE_X 2
#define TRANSFER_LOWER_X 1
#define TRANSFER_LOWER_WIDTH 28
#elif LANGUAGE == LANGUAGE_ITALIAN
#define ENTRY_WIDTH 7
#define PASSWORD_UPPER_X 7
#define PASSWORD_UPPER_WIDTH 17
#define PASSWORD_LOWER_X 3
#define PASSWORD_LOWER_WIDTH 25
#define TRANSFER_UPPER_X 5
#define TRANSFER_UPPER_WIDTH 20
#define TRANSFER_TITLE_X 2
#define TRANSFER_LOWER_X 1
#define TRANSFER_LOWER_WIDTH 28
#else
#define ENTRY_WIDTH 7
#define PASSWORD_UPPER_X 8
#define PASSWORD_UPPER_WIDTH 14
#define PASSWORD_LOWER_X 4
#define PASSWORD_LOWER_WIDTH 22
#define TRANSFER_UPPER_X 6
#define TRANSFER_UPPER_WIDTH 18
#define TRANSFER_TITLE_X 12
#define TRANSFER_LOWER_X 5
#define TRANSFER_LOWER_WIDTH 21
#endif

// Spanish, French and Italian indent the password's lines and set them
// closer together.
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
#define PASSWORD_TEXT_X 8
#define PASSWORD_TEXT_Y 0
#define PASSWORD_LINE_HEIGHT 11
#else
#define PASSWORD_TEXT_X 0
#define PASSWORD_TEXT_Y 4
#define PASSWORD_LINE_HEIGHT 12
#endif

s32 Func_0804d7fc(s32 mode) {
  struct ResourceMenuWork *work;
  struct Window *window;
  s32 msg;
  struct Window **upper;
  s32 result;

  mode &= 1;
  AffineEffect_InitializeWork();
  work = Ram_HeapSlots->menuSelectWork;
  if (mode == 0) {
    Menu_AppendResourceEntry(44);
    Menu_AppendResourceEntry(45);
  } else {
    Menu_AppendResourceEntry(46);
    Menu_AppendResourceEntry(47);
    Menu_AppendResourceEntry(48);
  }
  Menu_CenterResourceEntries(17, ENTRY_WIDTH, 0);
  if (mode != 0) {
    Task_Add(Menu_DrawModeLabel, TASK_PASS_VBLANK - 10);
    work->previousMode = 0xffff;
    window = Window_Create(PASSWORD_UPPER_X, 0, PASSWORD_UPPER_WIDTH, 4, 2);
    msg = (s32)MsgPasswordEntry;
    upper = &work->upperWindow;
    *upper = window;
    Text_DrawMessage(msg, window, 0, 4);
    window = Window_Create(PASSWORD_LOWER_X, 4, PASSWORD_LOWER_WIDTH, 12, 2);
    work->lowerWindow = window;
    Text_DrawMessage(msg + 1, window, PASSWORD_TEXT_X, PASSWORD_TEXT_Y);
#if LANGUAGE == LANGUAGE_FRENCH
    // French writes a third line: "Niveaux" (MsgPasswordLevel).
    Text_DrawMessage(msg + 2, work->lowerWindow, PASSWORD_TEXT_X,
                     PASSWORD_TEXT_Y + PASSWORD_LINE_HEIGHT);
    msg += 3;
    Text_DrawMessage(msg, work->lowerWindow, PASSWORD_TEXT_X,
                     PASSWORD_TEXT_Y + PASSWORD_LINE_HEIGHT * 2);
#else
    msg += 2;
    Text_DrawMessage(msg, work->lowerWindow, PASSWORD_TEXT_X,
                     PASSWORD_TEXT_Y + PASSWORD_LINE_HEIGHT);
#endif
  } else {
    Task_Add(Menu_DrawModeIndicator, TASK_PASS_VBLANK - 10);
    work->previousMode = 0xffff;
    window = Window_Create(TRANSFER_UPPER_X, 0, TRANSFER_UPPER_WIDTH, 4, 2);
    upper = &work->upperWindow;
    *upper = window;
    Text_DrawMessage((s32)MsgTransferMethod, window, TRANSFER_TITLE_X, 4);
    work->lowerWindow =
        Window_Create(TRANSFER_LOWER_X, 5, TRANSFER_LOWER_WIDTH, 7, 2);
  }
  result = Menu_RunResourceSelectionLoop(0);
  if (mode != 0)
    Task_Remove(Menu_DrawModeLabel);
  else
    Task_Remove(Menu_DrawModeIndicator);
  WindowSprite_PrepareForRedraw(*upper);
  WindowSprite_PrepareForRedraw(work->lowerWindow);
  Window_Close(*upper, 2);
  Window_Close(work->lowerWindow, 2);
  wait(1);
  Menu_EndResourceSelection();
  return result;
}

// Menu: select command icons, such as Yes and No.

/// Selects a shop command from icons 0x11-0x14, centred on row 17, showing
/// the selected command's name in a window. Icons are indices into
/// ResourceId_CommandIcons.
/// selection: initial selection. Returns the selected position.
s32 Menu_SelectShopCommand(s32 selection) {
  s32 ret;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x11);
  Menu_AppendResourceEntry(0x12);
  Menu_AppendResourceEntry(0x13);
  Menu_AppendResourceEntry(0x14);
#if (LANGUAGE == LANGUAGE_GERMAN)
  Menu_CenterResourceEntries(17, 8, 0); // Name window width.
#elif (LANGUAGE != LANGUAGE_JAPANESE)
  Menu_CenterResourceEntries(17, 7, 0);
#else
  Menu_CenterResourceEntries(17, 6, 0);
#endif
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return ret;
}

/// Selects a sanctum service from icons 0x19-0x1c, centred on row 17.
s32 Menu_SelectSanctumService(s32 selection) {
  s32 ret;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x19);
  Menu_AppendResourceEntry(0x1A);
  Menu_AppendResourceEntry(0x1B);
  Menu_AppendResourceEntry(0x1C);
#if (LANGUAGE != LANGUAGE_JAPANESE)
  Menu_CenterResourceEntries(17, 10, 0);
#else
  Menu_CenterResourceEntries(17, 8, 0);
#endif
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return ret;
}

/// Selects Yes or No (icons 5 and 6). Cancel returns 1 (No).
/// low: row 0 if zero, otherwise row 17.
/// message: first name message, or 0 for command names.
/// w: name window width, or 3 if zero.
/// selection: initial selection.
s32 Menu_RunConfirmSelection(s32 low, s32 message, s32 w, s32 selection) {
  s32 row;

  row = 0;

  AffineEffect_InitializeWork();

  if (w == 0) {
    w = 3;
  }
  if (low != 0) {
    row = 17;
  }

  Menu_AppendResourceEntry(5);
  Menu_AppendResourceEntry(6);
  Menu_CenterResourceEntries(row, w, message);

  selection = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  if (selection == -1) {
    selection = 1;
  }
  return selection;
}

/// Selects Yes or No at (x, y). Cancel returns 1 (No).
s32 Menu_RunConfirmSelectionAt(s32 x, s32 y, s32 selection) {
  s32 message = (s32)MsgCommandYes;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(5);
  Menu_AppendResourceEntry(6);
  Menu_LayoutResourceEntries(x, y, 3, message);
  selection = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  if (selection == -1) {
    selection = 1;
  }
  return selection;
}

//--------------------------------------------------------------------------

// select_entry_20_to_21.c

// Menu: select between command icons 0x20 and 0x21.

//--------------------------------------------------------------------------

// Name window width for each edition.
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define PAIR_WIDTH 7
#elif (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define PAIR_WIDTH 10
#else
#define PAIR_WIDTH 9
#endif

/// Selects coins or medals from icons 0x20 and 0x21, centred on row 17.
/// selection: initial selection. Returns the selected position.
s32 Menu_SelectCoinsOrMedals(s32 selection) {
  s32 ret;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x20);
  Menu_AppendResourceEntry(0x21);
  Menu_CenterResourceEntries(17, PAIR_WIDTH, 0);
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return ret;
}

/* VIEW(resource_work): routine Menu_RunSelection as void Menu_RunSelection(void); the definition has s16 Menu_RunSelection(void); tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/window/f04a85c.c:3143: `Menu_RunSelection' undeclared (first use in this function). */
extern void Menu_RunSelection__resource_work(void) __asm__("Menu_RunSelection");

void Func_0804dabc(void) { Task_Add(Menu_RunSelection__resource_work, TASK_PASS_VBLANK); }
#undef AFFINE_EFFECT_SLOT
#undef SOUND_MENU_CURSOR_MOVE
#undef SOUND_MENU_CONFIRM
#undef Resource_DecodeByteLzInRam
#undef LABEL_RIGHT
#undef HELP_X
#undef ENTRY_WIDTH
#undef PASSWORD_UPPER_X
#undef PASSWORD_UPPER_WIDTH
#undef PASSWORD_LOWER_X
#undef PASSWORD_LOWER_WIDTH
#undef TRANSFER_UPPER_X
#undef TRANSFER_UPPER_WIDTH
#undef TRANSFER_TITLE_X
#undef TRANSFER_LOWER_X
#undef TRANSFER_LOWER_WIDTH
#undef PASSWORD_TEXT_X
#undef PASSWORD_TEXT_Y
#undef PASSWORD_LINE_HEIGHT
#undef PAIR_WIDTH

// ---- games/tla/src/window/selrow.c ----
s32 Scene_GetAreaNameIndexFar(s32 scene, s32 entrance);
void Text_DrawNumber(s32 value, s32 digits, void *window, s32 x, s32 y);
void Text_DrawString(u8 *text, s32 arg1, s32 arg2, s32 arg3);
void Text_DrawMessage(s32 message, void *window, s32 x, s32 y);
extern const u8 gMenu_ColonString[];
extern u8 MsgDebugAreaName[];
extern u8 MsgDebugSceneCode[];

#if (LANGUAGE == LANGUAGE_JAPANESE)
#define SELROW_TEXT_X 100
#define SELROW_LABEL_X 110
#else
#define SELROW_TEXT_X 74
#define SELROW_LABEL_X 82
#endif

void Menu_DrawSelectionRow(void *window, s16 first, const s16 *second) {
  s16 selected = first;
  s32 label =
      Scene_GetAreaNameIndexFar(selected, *second) + (s32)MsgDebugAreaName;

  WindowSprite_PrepareForRedraw(window);
  Text_DrawNumber(selected, 3, window, 0, 14);
  Text_DrawNumber(*second, 3, window, SELROW_LABEL_X, 14);
  Text_DrawString(gMenu_ColonString, window, SELROW_TEXT_X, 0);
  Text_DrawMessage(selected + (s32)MsgDebugSceneCode, window, 0, 0);
  Text_DrawString(gMenu_ColonString, window, SELROW_TEXT_X, 14);
  Text_DrawMessage(label, window, SELROW_LABEL_X, 0);
}


struct Work;

struct TextObject {
  u8 storage[12];
};

void Event_SetDestinationFar(s16 primary, s16 secondary);
void UiTextResource_Release(s32 resource);
void Window_Close(struct Window *work, s32 release);
/* VIEW(selrow): routine Menu_HandleSelectionRowInput as s32 Menu_HandleSelectionRowInput(struct Work *work, s16 value, s16 *sub, s16 *mode); the definition has s32 Menu_HandleSelectionRowInput(s32 arg, s16 value, s16 *sub, s16 *mode); tried as the rest of the object declares it, it does not compile in tla-ja (make modern): games/tla/src/window/f04a85c.c:129:74: note: expected 'struct TargetWindow *' but argument is of type 's32' {aka 'int'}. */
extern s32 Menu_HandleSelectionRowInput__selrow(struct Work *work, s16 value, s16 *sub,
                                 s16 *mode) __asm__("Menu_HandleSelectionRowInput");

/// Picks a destination (primary and secondary) in a row menu and travels
/// there; returns -1 after travelling and -2 when cancelled.
s16 Menu_RunSelection(void) {
  s32 resource;
  s16 mode;
  s16 secondary;
  struct TextObject object;
  s16 primary;
  struct Work *work;
  s16 result;

  work = 0;
  mode = 0;
  primary = gMainRoutineWork.primary;
  secondary = gMainRoutineWork.secondary;
  work = Window_Create(0, 7, 30, 5, 2);
  Menu_DrawSelectionRow(work, primary, &secondary);
  UiTextResource_Initialize(&object, &resource);

  while (gInput.held != 0)
    wait(1);

  for (;;) {
    result =
        (s16)Menu_HandleSelectionRowInput__selrow(work, primary, &secondary, &mode);
    if (result == -1) {
      UiTextResource_Release(resource);
      Window_Close(work, 2);
      Event_SetDestinationFar(primary, secondary);
      return result;
    }
    if (result == -2) {
      UiTextResource_Release(resource);
      Window_Close(work, 2);
      return result;
    }

    UiTextResource_SetPosition(&object, MENU_TEXT_X, mode * 14 + MENU_TEXT_Y);
    primary = result;
    wait(1);
  }
}
#undef SELROW_TEXT_X
#undef SELROW_LABEL_X

// ---- games/tla/src/window/handle_selection_row_input.c ----
// The debug selection row's keys (The Broken Seal's
// DebugSceneSelect_HandleRowInput): A returns -1 and B -2; Up or Down switches
// the field, Left and Right step the value (0 to 324) or the second number
// (0 to 99) by one, R and L by ten, and the row is redrawn after a step.


s32 Menu_HandleSelectionRowInput(s32 arg, s16 value, s16 *sub, s16 *mode) {
  if (gInput.repeat & KEY_A)
    return -1;
  if (gInput.repeat & KEY_B)
    return -2;
  if (gInput.repeat & KEY_DOWN) {
    *mode ^= 1;
  } else if (gInput.repeat & KEY_UP) {
    *mode ^= 1;
  } else if (gInput.repeat & KEY_RIGHT) {
    if (*mode == 0) {
      value++;
    } else {
      (*sub)++;
      if (*sub > 99)
        *sub = 0;
    }
    if (value > 324)
      value = 0;
    Menu_DrawSelectionRow(arg, value, sub);
  } else if (gInput.repeat & KEY_LEFT) {
    if (*mode == 0) {
      value--;
    } else {
      (*sub)--;
      if (*sub < 0)
        *sub = 99;
    }
    if (value < 0)
      value = 324;
    Menu_DrawSelectionRow(arg, value, sub);
  } else if (gInput.repeat & KEY_R) {
    if (*mode == 0) {
      *sub = 0;
      value += 10;
    } else {
      *sub += 10;
      if (*sub > 99)
        *sub -= 99;
    }
    if (value > 324)
      value = 0;
    Menu_DrawSelectionRow(arg, value, sub);
  } else if (gInput.repeat & KEY_L) {
    if (*mode == 0) {
      *sub = 0;
      value -= 10;
    } else {
      *sub -= 10;
      if (*sub < 0)
        *sub += 99;
    }
    if (value < 0)
      value = 324;
    Menu_DrawSelectionRow(arg, value, sub);
  }
  return value;
}

// Menu: flag list (move the cursor and press A to toggle).


/// Shows 16x16 flags per page and takes input until closed, starting on page
/// 8. Returns 0.
s32 Menu_RunSelectionWithCursorObject(void) {
  u8 obj[12]; // Cursor object.
  s32 cur[2]; // Cursor column and row.
  s32 res;
  s32 page;
  s32 win;
  s32 ret;

  page = 8;
  win = 0;
  cur[0] = win;
  cur[1] = win;
  win = Window_Create(1, 0, 28, 20, 2);
  Menu_DrawFlagBitTable(win, page);
  UiTextResource_Initialize(obj, &res);
  for (;;) {
    wait(1);
    ret = Menu_HandleFlagGridInput(win, &page, cur);
    if (ret == -1)
      break; // Close.
    if (ret == 1)
      Menu_DrawFlagBitTable(win, page); // A flag or the page changed.
    UiTextResource_SetPosition(obj, cur[0] * 8 + 58, cur[1] * 8 + 20);
  }
  UiTextResource_Release(res);
  Window_Close(win, 2);
  return 0;
}

/// Writes the lowest count digits of value (up to five), most significant
/// first, to out as uppercase hexadecimal, without a terminating zero.
void Text_FormatHex(u32 value, u32 count, u8 *out) {
  u8 buf[8];
  u32 i;

  if (count > 5)
    count = 5;

  for (i = 0; i != count; i++) {
    u32 d = value & 15;
    if (d <= 9)
      buf[i] = d + '0';
    else
      buf[i] = d + 'A' - 10;
    value >>= 4;
  }

  for (i = count - 1; (s32)i >= 0; i--)
    *out++ = buf[i];
}

extern const u8 gMenu_HexDigitsString[];

extern const u8 gMenu_ColonString[];

/* VIEW(handle_selection_row_input): routine WindowSprite_PrepareForRedraw as void WindowSprite_PrepareForRedraw(void); resource_work has void WindowSprite_PrepareForRedraw(struct Window *); tried as the definition's void WindowSprite_PrepareForRedraw(struct Window *work), which the object already declares, it does not compile in tla-ja: games/tla/src/window/f04a85c.c:3393: too few arguments to function `WindowSprite_PrepareForRedraw'. */
extern void WindowSprite_PrepareForRedraw__handle_selection_row_input(void) __asm__("WindowSprite_PrepareForRedraw");
void Text_DrawStringInWindow(s32 text, s32 window, s32 x, s32 y);
s32 GameFlag_Test(s32 flag);

void Menu_DrawFlagBitTable(s32 window, s32 startFlag) {
  s32 row;
  s32 y;
  s32 flag;
  u8 label[5];
  char bits[17];

  WindowSprite_PrepareForRedraw__handle_selection_row_input();
  Text_DrawStringInWindow((s32)gMenu_HexDigitsString, window, 48, 0);

  flag = startFlag << 8;
  for (row = 0; row != 16; row++) {
    s32 i;

    y = row * 8 + 16;

    for (i = 0; i != 5; i++) {
      label[i] = 0;
    }
    Text_FormatHex(flag, 3, label);
    Text_DrawStringInWindow((s32)label, window, 0, y);
    Text_DrawStringInWindow((s32)gMenu_ColonString, window, 32, y);

    for (i = 0; i < 16; i++) {
      s32 val = GameFlag_Test(flag);
      bits[i] = (val != 0) + 48;
      flag++;
    }
    bits[i] = 0;
    Text_DrawStringInWindow((s32)bits, window, 48, y);
  }
}

void GameFlag_ClearBit(s32);

s32 Menu_HandleFlagGridInput(s32 window, s32 *page, s32 *cursor) {
  s32 *row = &cursor[1];

  if (gInput.repeat & 1) {
    s32 flag = ((*page << 4) + *row << 4) + cursor[0];

    if (GameFlag_Test(flag))
      GameFlag_ClearBit(flag);
    else
      GameFlag_SetBit(flag);
    return 1;
  }
  if ((gInput.pressed & 2) || (gInput.repeat & 4))
    return -1;
  if (gInput.repeat & 0x40) {
    if (--*row < 0)
      *row = 15;
  } else if (gInput.repeat & 0x80) {
    if (++*row > 15)
      *row = 0;
  } else if (gInput.repeat & 0x20) {
    if (--cursor[0] < 0)
      cursor[0] = 15;
  } else if (gInput.repeat & 0x10) {
    if (++cursor[0] > 15)
      cursor[0] = 0;
  } else if ((gInput.repeat & 0x200) && (gInput.repeat & 8)) {
    *page -= 10;
    if (*page < 0)
      *page = 15;
    return 1;
  } else if ((gInput.repeat & 0x100) && (gInput.repeat & 8)) {
    *page += 10;
    if (*page > 15)
      *page = 0;
    return 1;
  } else if (gInput.repeat & 0x200) {
    if (--*page < 0)
      *page = 15;
    return 1;
  } else if (gInput.repeat & 0x100) {
    if (++*page > 15)
      *page = 0;
    return 1;
  }
  return 0;
}

void Menu_ReservedNoOp(void) {}

void Djinn_SetFoundFlagAndAdd(s32 arg0, s32 arg1, s32 arg2) {
  GameFlag_SetBit((arg1 * 0x14) + arg2 + 0x30);
  Djinn_AddToOwnerFar(arg0, arg1, arg2);
}

// ---- games/tla/src/window/flag_bit_rect.c ----
#include "scene_call.h"

s32 Func_08040798(s32 selection);
s32 Func_08043690(void);
s32 Func_080400e8(void);

/// Runs the save screen's selection until a choice completes: saving (refused
/// while flag 0x17c or 0x17d is set), sleep mode or the options. Returns -1
/// when the screen is cancelled and 0 otherwise.
s32 Func_0804e0d0(void) {
  s32 selection = 0;

again:
  selection = Func_08040798(selection);
  if (selection == -1)
    return -1;
  if (selection == 0) {
    if (GameFlag_Test(0x17c)) {
      FieldMessage_Show((s32)MsgCannotSaveHere, 1);
      goto again;
    }
    if (GameFlag_Test(0x17d)) {
      FieldMessage_Show((s32)MsgNoPlaceForThat, 1);
      goto again;
    }
    if (Func_08043690() == -1)
      goto again;
  } else if (selection == 1) {
    FieldMessage_Show((s32)MsgSleepMode, 1);
    gSleepRequested = selection;
  } else if (selection == 2) {
    if (Func_080400e8() == -1)
      goto again;
  }
  return 0;
}

void Render_SetTilemapFlagBitRect(const struct Window *object, s32 x, s32 y,
                                  s32 width, s32 height, u32 field) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;

  x += (s16)object->x + 1;
  y += (s16)object->y + 1;
  field &= 1;
  field <<= 12;
  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > 29) {
    width = 30 - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  if (y + height > 29) {
    height = 20 - y;
  }
  if (width > 0 && height > 0) {
    do {
      u16 *pixel = &work->tilemap[y][x];
      s32 remaining = width;
      while (remaining != 0) {
        u32 value = *pixel;
        value &= 0xFFFFEFFF;
        value |= field;
        remaining--;
        *pixel = value;
        pixel++;
      }
      height--;
      y++;
    } while (height != 0);
    work->dirty = 1;
  }
}

#include "sprite.h"
#include "render_input.h"

extern u8 gWorkspace_ItemTitle[];
extern u8 gWorkspace_EnergyTitle[];
extern u8 gWorkspace_StatusTitle[];
extern u8 gWorkspace_PageLabel[];
extern u8 gWorkspace_RangeDash[];

void wait(s32 frames);
void Icon_BuildItemIconTiles(s32 icon, s32 with_base, s32 *slot, s32 *tile,
                             s32 reuse);
void Icon_BuildAbilityIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot, s32 *tileOut, s32 reuse);
s32 Resource_FindFreeEntry(void);
void Icon_BuildMiscIconTiles(s32 pattern, s32 unused, s32 slot);

s32 Func_0804e1d8(void) {
  s32 redraw;
  s32 tile;
  s32 page;
  s32 mode;
  struct RenderInput *window;
  s32 slot;
  s32 base;
  s32 i;
  s32 row;
  s32 x;
  s32 y;

  window = NULL;
  page = 0;
  mode = 0;
  redraw = 1;
  Ram_HeapSlots->objectSystem->suspended = redraw;
  wait(1);

  while (1) {
    if (gInput.repeat & 0x20) {
      page--;
      redraw = 1;
    }
    if (gInput.repeat & 0x10) {
      redraw = 1;
      page++;
    }
    if (gInput.repeat & 0x200) {
      mode--;
      redraw = 1;
    }
    if (gInput.repeat & 0x100) {
      redraw = 1;
      mode++;
    }
    if (gInput.repeat & 1)
      break;
    if (gInput.repeat & 2)
      break;
    if (redraw != 0) {
      redraw = 0;
      page = (page + 12) % 12;
      mode = (mode + 3) % 3;
      Window_Close(window, 2);
      window = Window_Create(10, 0, 18, 12, 2);
      if (mode == 0)
        Text_DrawStringInWindow(gWorkspace_ItemTitle, window, 0, 0);
      else if (mode == 1)
        Text_DrawStringInWindow(gWorkspace_EnergyTitle, window, 0, 0);
      else
        Text_DrawStringInWindow(gWorkspace_StatusTitle, window, 0, 0);
      Text_DrawStringInWindow(gWorkspace_PageLabel, window, 0, 8);
      Text_DrawNumberInWindow(page, 0, window, 40, 8);
      base = page << 5;
      Text_DrawNumberInWindow(base, 3, window, 64, 8);
      Text_DrawStringInWindow(gWorkspace_RangeDash, window, 88, 8);
      Text_DrawNumberInWindow(base + 31, 3, window, 96, 8);
      for (i = 0; i <= 31; i++) {
        slot = -1;
        row = i / 8;
        x = (i - row * 8) * 16;
        y = row * 16 + 16;
        if (mode == 0) {
          Icon_BuildItemIconTiles(base + i, 1, &slot, &tile, mode);
          WindowSprite_Create(slot, 0x40000000, window, x, y);
        } else if (mode == 1) {
          Icon_BuildAbilityIconTiles(base + i, 1, &slot, &tile, 0);
          WindowSprite_Create(slot, 0x40000000, window, x, y);
        } else {
          slot = Resource_FindFreeEntry();
          Icon_BuildMiscIconTiles(i, 0, slot);
          WindowSprite_Create(slot, 0x40000000, window, x, y);
        }
      }
    }
    wait(1);
  }
  Window_Close(window, 2);
  Ram_HeapSlots->objectSystem->suspended = 0;
  return 0;
}


extern s16 gPortrait_CharacterMap[];
extern s16 gPortrait_SpriteMap[];

struct RenderInput *Func_0803d3c0(s32 x, s32 y, s32 width, s32 height);
void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                                  s32 palette, s32 reuse);

/// A debug browser of the portraits: left and right step through
/// gPortrait_CharacterMap and then gPortrait_SpriteMap, L and R by ten, each
/// drawn with its number and its name from MsgTestName; A or B leaves.
s32 Func_0804e3f4(void) {
  struct WindowWork *work;
  struct RenderInput *portrait;
  s32 redraw;
  struct RenderInput *window;
  s32 count;
  s32 total;
  s32 index;
  s32 i;
  s32 glyph;
  s32 tile;
  s32 slot;
  s32 cancel;
  u32 slotOffset;
  s32 palette;

  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  redraw = 1;
  portrait = Func_0803d3c0(0, 0, 10, 5);
  window = Window_Create(10, 10, 14, 3, 2);
  index = 0;
  palette = 15;
  for (i = 0; gPortrait_CharacterMap[i * 2] != -1; i++) {
  }
  count = i;
  for (i = 0; gPortrait_SpriteMap[i * 2] != -1; i++) {
  }
  total = count + i;
  /* FAKEMATCH(dead): a zero from the count keeps the mask in r9 and work in
   * fp; as cancel = 2, 30 of 180 instructions differ in tla-en. */
  cancel = ((u32)(u16)count >> 16) | 2;
  slotOffset = offsetof(struct WindowWork, outputIds[1]);
  while (1) {
    if (gInput.repeat & 0x20) {
      redraw = 1;
      index--;
    }
    if (gInput.repeat & 0x10) {
      redraw = 1;
      index++;
    }
    if (gInput.repeat & 0x200) {
      redraw = 1;
      index -= 10;
    }
    if (gInput.repeat & 0x100) {
      redraw = 1;
      index += 10;
    }
    if (gInput.repeat & 1)
      break;
    if (gInput.repeat & cancel)
      break;
    if (redraw) {
      redraw = 0;
      index = (index + total) % total;
      WindowSprite_PrepareForRedraw(window);
      if (index < count)
        glyph = gPortrait_CharacterMap[index * 2 + 1];
      else
        glyph = gPortrait_SpriteMap[(index - count) * 2 + 1] + 128;
      slot = *(u16 *)((u8 *)work + slotOffset);
      UiGlyph_LoadEntryWithPalette(glyph, 0, &slot, &tile, palette, 1);
      Text_DrawNumberInWindow(index, 2, window, 0, 0);
      Text_DrawCharacterAtOffset(index + (s32)MsgTestName, window, 24, 0);
    }
    wait(1);
  }
  Window_Close((struct Window *)window, 2);
  Window_Close((struct Window *)portrait, 2);
  wait(1);
  return 0;
}
