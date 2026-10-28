// Draft, differ 650 of 1647 (777 before; 2026-10-26: the selected position and
// the candidate count declared earlier, the target count later, the eased
// track x stored first): battle target selection, The Broken Seal's
// BattleTarget_RunSelection (games/tbs/src/window/screens/targets.c) on this
// game's work and messages. The modes are numbered differently (2 the
// party, 4 the acting unit alone, 1 the enemies, whose downed ones a link
// battle skips); a new hint (8) starts on the unit the last command aimed at,
// which the navigation work remembers (0x10c); the enemies "Mystery Man" and
// "Mystery Woman" are enemy kinds 355 and 352. Structure and the arrays'
// frame order follow the listing; left: the stack frame is 4 bytes smaller here, the
// curing tests read their status bytes one at a time where the native tests
// some as a word or halfword, and the allocation differs.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"

extern u8 Msg_0c5a[];
extern u8 Msg_0d03[];
extern u8 Msg_0d04[];
extern u8 Msg_0d05[];
extern u8 Msg_0d06[];
extern u8 Msg_0d07[];
extern u8 Msg_0d08[];
extern u8 Msg_0d09[];
extern u8 Msg_0d0a[];
extern u8 Msg_0d0b[];
extern u8 Msg_0d0c[];

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

struct TargetUnit {
  u8 name[0x34];
  s16 hpMax;
  s16 ppMax;
  s16 hp;
  s16 pp;
  u8 unk03c[0xf5];
  s8 poison;
  u8 unk132[6];
  u8 delusion;
  u8 unk139[2];
  u8 stun;
  u8 sleep;
  u8 psySeal;
  u8 unk13e[2];
  u8 haunt;
  u8 grimReaper;
  u8 unk142[8];
  u16 enemyKind;
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

s32 VramCache_AllocEntry(s32 size);
s32 VramCache_Reload(s32 entry, const void *tiles);
struct TargetUnit *Unit_Get(s32 unit);
s32 GameFlag_Test(s32 flag);
void BattleMotion_ProjectConditionalPositionFar(s32 unit,
                                                struct TargetPoint *point);
struct TargetWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                     s32 flags);
s32 Trig_Sin(s32 angle);
void Oam_AddSpriteAtDepth(void *sprite, s32 depth);
s32 AffineMatrix_BuildForEffect(struct TargetAffine *affine);
void BattlePres_SetActorModesFar(u16 *units, s32 mode);
void UiWork_Finalize(void *window, s32 mode);
void UiText_DrawStringAtOffset(const u8 *text, struct TargetWindow *window,
                               s32 x, s32 y);
void UiText_DrawNumberInWindow(s32 value, s32 digits,
                               struct TargetWindow *window, s32 x, s32 y);
void UiText_DrawCharacterAtOffset(s32 message, struct TargetWindow *window,
                                  s32 x, s32 y);
void Func_08041f70(s32 color);
void UiText_CopyMessageString(s32 message, u16 *text, s32 capacity);
s32 UiText_GetWideStringWidth(u16 *text);
void Ui_ClearVramBlock(void);
void UiText_RenderWideStringAtOffset(u16 *text, struct TargetWindow *window,
                                     s32 x, s32 y);
void Sound_PlayFar(s32 cue);
void WaitFrames(s32 frames);
void Resource_ResetEntry(s32 slot);

// The help window's column beside the aimed member, kept on screen.
#define TARGET_HELP_COLUMN(x, back, width, last)                               \
  ((x) / 8 + (width) > 29 ? (last) : (x) / 8 - (back))

s32 Func_0804a85c(s32 selection, s32 mode, u32 spread, u32 kind) {
  struct TargetPoint selectedPosition;
  u16 candidateIds[8];
  struct TargetSprite cursorSprites[6];
  struct TargetTrack cursorTracks[6];
  struct TargetPoint actorPosition;
  u16 spreadActorIds[14];
  u8 spreadTrackIndices[8];
  u16 targetName[15];
  struct TargetPoint namePosition;
  struct TargetPoint markerPosition;
  struct TargetAffine effect;
  struct TargetBattle *battle;
  s32 redraw;
  s32 targetCount;
  s32 candidateCount;
  s32 cursorSlot;
  /* FAKEMATCH(pin): the condition window in r9, where the native keeps it, as
   * The Broken Seal's; as an ordinary local it is spilled and 952 of 1647
   * instructions differ in tla-en (937 with the pin). */
  register struct TargetWindow *conditionWindow asm("r9");
  s32 targetIndex;
  s32 drawCursors;
  s32 matrix;
  s32 found;
  s32 count;
  s32 i;
  s32 j;
  s32 n;
  s32 pressed;
  s32 repeated;
  s32 result;
  struct TargetWindow *window;
  struct TargetUnit *unit;
  struct TargetTrack *track;
  struct TargetSprite *marker;
  struct TargetSprite *firstSprite;

  battle = Ram_HeapSlots->battleWork;
  targetCount = 0;
  redraw = 0xffff;
  cursorSlot = VramCache_AllocEntry(256);
  conditionWindow = NULL;
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
        struct TargetUnit *candidate;

        if (candidateIds[j] == 254)
          continue;
        candidate = Unit_Get(candidateIds[j]);
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
          Unit_Get(candidateIds[targetIndex])->hp == 0)) {
    targetIndex = targetIndex + targetCount - 1;
    targetIndex %= targetCount;
  }
  if (mode != 2) {
    BattleMotion_ProjectConditionalPositionFar(selection, &actorPosition);
    cursorTracks[0].flags = 8;
    cursorTracks[0].x = actorPosition.x;
    cursorTracks[0].y = 0x80;
  }
  window = UiWindow_Create(0, 12, 30, 4, 74);

  for (;;) {
    s32 y;

    firstSprite = cursorSprites;
    drawCursors = 0;
    BattleMotion_ProjectConditionalPositionFar(candidateIds[targetIndex],
                                               &selectedPosition);
    ((u32 *)firstSprite)[1] = 0x40002000;
    ((u32 *)firstSprite)[2] = drawCursors;
    firstSprite->tile = VramCache_Reload(
        cursorSlot, ((gFrameCount >> 2) & 31) * 256 + gData_0805c9c4);
    selectedPosition.y += Trig_Sin(gFrameCount << 12) / 32768;
    y = selectedPosition.y;
    if (cursorTracks[0].flags & 1) {
      s32 cursorX = (selectedPosition.x + cursorTracks[0].x) / 2;
      s32 cursorY = (y + cursorTracks[0].y) / 2;

      if ((selectedPosition.x - cursorX < 0
               ? -(selectedPosition.x - cursorX)
               : selectedPosition.x - cursorX) <= 7)
        drawCursors = 1;
      cursorTracks[0].x = cursorX;
      selectedPosition.x = cursorX;
      selectedPosition.y = cursorY;
      cursorTracks[0].y = cursorY;
    } else if (cursorTracks[0].flags <= 3) {
      selectedPosition.y = y;
      cursorTracks[0].x = selectedPosition.x;
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
    Oam_AddSpriteAtDepth(firstSprite, 240);
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
      BattlePres_SetActorModesFar(spreadActorIds, 1);
      if ((u32)(NAVIGATION->lastTarget = candidateIds[targetIndex]) <= 7) {
        if (spread != 255 && kind != 0) {
          unit = Unit_Get(candidateIds[targetIndex]);
          BattleMotion_ProjectConditionalPositionFar(candidateIds[targetIndex],
                                                     &selectedPosition);
          if (conditionWindow != NULL)
            UiWork_Finalize(conditionWindow, 1);
          switch (kind) {
          case 7:
            conditionWindow = UiWindow_Create(
                TARGET_HELP_COLUMN(selectedPosition.x, 4, 4, 22), 8, 9, 3, 6);
            Func_08041f70(2);
            UiText_DrawCharacterAtOffset((s32)Msg_0d0c, conditionWindow, 0, 0);
            Func_08041f70(15);
            break;
          case 1:
            conditionWindow = UiWindow_Create(
                TARGET_HELP_COLUMN(selectedPosition.x, 7, 6, 17), 8, 13, 3, 6);
            UiText_DrawStringAtOffset(gOwnerStatus_HpString, conditionWindow, 0,
                                      0);
            UiText_DrawNumberInWindow(unit->hp, 4, conditionWindow, 16, 0);
            UiText_DrawStringAtOffset(gOwnerStatus_SlashString, conditionWindow,
                                      48, 0);
            UiText_DrawNumberInWindow(unit->hpMax, 4, conditionWindow, 56, 0);
            break;
          case 2:
            conditionWindow = UiWindow_Create(
                TARGET_HELP_COLUMN(selectedPosition.x, 7, 6, 17), 8, 13, 3, 6);
            UiText_DrawStringAtOffset(gOwnerStatus_PpString, conditionWindow, 0,
                                      0);
            UiText_DrawNumberInWindow(unit->pp, 4, conditionWindow, 16, 0);
            UiText_DrawStringAtOffset(gOwnerStatus_SlashString, conditionWindow,
                                      48, 0);
            UiText_DrawNumberInWindow(unit->ppMax, 4, conditionWindow, 56, 0);
            break;
          case 5:
            conditionWindow = UiWindow_Create(
                TARGET_HELP_COLUMN(selectedPosition.x, 7, 5, 18), 8, 12, 3, 6);
            if (unit->hp != 0)
              goto no_condition;
            UiText_DrawCharacterAtOffset((s32)Msg_0d0b, conditionWindow, 0, 0);
            break;
          case 3:
            conditionWindow = UiWindow_Create(
                TARGET_HELP_COLUMN(selectedPosition.x, 7, 5, 18), 8, 12, 3, 6);
            if (unit->poison == 0)
              goto no_condition;
            UiText_DrawCharacterAtOffset((s32)Msg_0d04, conditionWindow, 0, 0);
            break;
          case 4: {
            s32 helpRow;

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
            conditionWindow = UiWindow_Create(
                TARGET_HELP_COLUMN(selectedPosition.x, 7, 9, 14), helpRow, 16,
                count + 2, 6);
            count = 0;
            if (unit->delusion != 0) {
              UiText_DrawCharacterAtOffset((s32)Msg_0d05, conditionWindow, 0,
                                           0);
              count = 1;
            }
            if (unit->stun != 0) {
              UiText_DrawCharacterAtOffset((s32)Msg_0d06, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (unit->sleep != 0) {
              UiText_DrawCharacterAtOffset((s32)Msg_0d07, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (unit->psySeal != 0) {
              UiText_DrawCharacterAtOffset((s32)Msg_0d08, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (unit->grimReaper != 0) {
              UiText_DrawCharacterAtOffset((s32)Msg_0d09, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (count == 0)
              goto no_condition;
            break;
          }
          default:
            break;
          case 6: {
            s32 helpRow;

            count = 0;
            if (unit->poison != 0)
              count = 1;
            if (unit->delusion != 0)
              count++;
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
            if (count == 0)
              count = 1;
            helpRow = 9 - count;
            if (helpRow <= 3)
              helpRow = 4;
            conditionWindow = UiWindow_Create(
                TARGET_HELP_COLUMN(selectedPosition.x, 7, 9, 14), helpRow, 16,
                count + 2, 6);
            count = 0;
            if (unit->poison != 0) {
              UiText_DrawCharacterAtOffset((s32)Msg_0d04, conditionWindow, 0,
                                           0);
              count = 1;
            }
            if (unit->delusion != 0) {
              UiText_DrawCharacterAtOffset((s32)Msg_0d05, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (unit->stun != 0) {
              UiText_DrawCharacterAtOffset((s32)Msg_0d06, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (unit->sleep != 0) {
              UiText_DrawCharacterAtOffset((s32)Msg_0d07, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (unit->psySeal != 0) {
              UiText_DrawCharacterAtOffset((s32)Msg_0d08, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (unit->grimReaper != 0) {
              UiText_DrawCharacterAtOffset((s32)Msg_0d09, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (unit->haunt != 0) {
              UiText_DrawCharacterAtOffset((s32)Msg_0d0a, conditionWindow, 0,
                                           count * 8);
              count++;
            }
            if (count != 0)
              break;
          }
          no_condition:
            Func_08041f70(2);
            UiText_DrawCharacterAtOffset((s32)Msg_0d03, conditionWindow, 0, 0);
            Func_08041f70(15);
            break;
          }
        }
      } else if (spread != 255) {
        struct TargetUnit *target = Unit_Get(candidateIds[targetIndex]);
        s32 width;

        BattleMotion_ProjectConditionalPositionFar(candidateIds[targetIndex],
                                                   &namePosition);
        namePosition.y += Trig_Sin(gFrameCount << 12) / 32768;
        if (target->enemyKind == 355 || target->enemyKind == 352) {
          width = (s32)Msg_0c5a;
          if (target->enemyKind == 355)
            width++;
          UiText_CopyMessageString(width, targetName, 14);
        } else {
          for (i = 0; i <= 13; i++) {
            targetName[i] = n = target->name[i];
            if (!n)
              break;
          }
          targetName[i] = 0;
        }
        width = UiText_GetWideStringWidth(targetName);
        namePosition.x -= width / 2;
        namePosition.x -= 8;
        if (namePosition.x + width > 224)
          namePosition.x = 224 - width;
        if (namePosition.x < 0)
          namePosition.x = 0;
        Ui_ClearVramBlock();
        UiText_RenderWideStringAtOffset(targetName, window, namePosition.x, 4);
      }
      redraw &= ~1;
    }

    if (drawCursors != 0) {
      for (i = 1; i < targetCount; i++) {
        marker = &firstSprite[i];
        track = &cursorTracks[spreadTrackIndices[i]];
        BattleMotion_ProjectConditionalPositionFar(spreadActorIds[i],
                                                   &markerPosition);
        markerPosition.y += Trig_Sin(gFrameCount << 12) / 32768;
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
        Oam_AddSpriteAtDepth(marker, 240);
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
    WaitFrames(1);
    if (redraw == 0)
      break;
  }
  WaitFrames(1);
  Resource_ResetEntry(cursorSlot);
  if (conditionWindow != NULL)
    UiWork_Finalize(conditionWindow, 1);
  UiWork_Finalize(window, 1);
  BattlePres_SetActorModesFar(candidateIds, 0);
  NAVIGATION->targetViewOffset = 0;
  WaitFrames(1);
  return targetIndex;
}
