// Draft, differ 12 of 697 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer (device search).
// Near miss, differ 34 of 697: The Broken Seal's English
// Message_StepScript with ⚓️'s changes: the VBlanks the last frame took clamp
// at 3, the page and wait codes take their countdown from gData_080aa0df by text speed
// and clear the pending presses in a window mode, the page arrow is skipped
// for an empty window, code 176 is a no-op control code, the window effect
// counts 11 frames, a text-mode channel with spacing[3] set draws at y / 320
// - 3, and a glyph that draws nothing closes the channel. Both reads of
// globals the twin reads volatile are volatile here (the held keys' first
// sample and gLastFrameVBlanks, which ☀️'s frame.h declares volatile; plain,
// differ 68). Left, all one-slot scheduling: the parameter copy and the
// keys sample swap, and five times a split constant's first half is issued
// between a store and the load that follows it (the native fills the
// store-to-load slot), as in this lane's other near misses.
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "glyph.h"
#include "input.h"
#include "main_routine_work.h"
#include "frame.h"

extern const u8 gText_SpeedDelays[];
extern const u8 gData_080aa0df[];
extern const u8 gText_SpeedSteps[];
extern const u8 gText_SpeedCueGaps[];

void Func_0803975c(s32 step);
void Func_08039754(struct UiChannelSlot *channel);
void Func_0803972c(struct UiChannelSlot *channel);
s32 Message_CheckCancelByInput(struct UiChannelSlot *channel);
s32 Message_CheckCancelByModeInput(struct UiChannelSlot *channel);
s32 Func_0803bde4(struct Window *window, u32 c, s32 x, s32 y, s32 flags);
void WindowSprite_PrepareForRedraw(struct Window *window);
void Window_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
void Window_FitOnScreen(s32 no, s32 *px, s32 *py, u32 *pw, u32 *ph,
                          u16 *spacing, u32 flags);
void Window_DrawBorder(s32 x, s32 y, s32 width, s32 height);
s32 Resource_ResetEntry(u32 slot);
void Sound_PlayFar(s32 cue);

#define TEXT_SPEED gMainRoutineWork.eventMode
#define ENTRY_MASK (UI_TEXT_ENTRY_COUNT - 1)

s32 Message_StepScript(struct UiChannelSlot *channel) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u32 keys = *(volatile u32 *)&gInput.held;
  s32 glyphsLeft = gText_SpeedSteps[TEXT_SPEED];
  s32 frameVBlanks;
  u32 code;
  u32 nextCode;
  struct Window *window;
  s32 x;
  s32 y;
  u32 newWidth;
  u32 newHeight;
  s32 oldWidth;
  s32 oldHeight;
  s32 entryIndex;
  s32 nextEntryIndex;
  s32 pixelX;
  s32 pixelY;
  s32 voiceGap;
  s32 glyphWidth;
  s32 glyphAdvance;

  (void)keys;

  if (work->menuState != 0) {
    frameVBlanks = *(volatile u16 *)&gLastFrameVBlanks;
    if (frameVBlanks < 0)
      frameVBlanks = 0;
    if (frameVBlanks > 3)
      frameVBlanks = 3;
    glyphsLeft = frameVBlanks * 5 + 3;
  }

  if (channel->transition != 0) {
    Func_0803975c(1);
    channel->transition = (s16)channel->transition - 1;
    return 0;
  }
  if (gInput.held == 0 && channel->glyphDelay != 0) {
    channel->glyphDelay--;
    return 0;
  }

  do {
    code = 0;
    if (channel->ended == 0)
      code = work->entries[channel->entry];

    if (code <= 30 || code == 176) {
      switch (code) {
      case 3:
        channel->x = channel->startX;
        if (channel->work->flags & 8) {
          if (channel->y >= 13 << 8) {
            Func_08039754(channel);
            glyphsLeft = 1;
          } else {
            channel->y += 13 << 8;
          }
        } else {
          channel->y += 15 << 8;
          if (channel->line <= 2)
            channel->line++;
        }
        break;

      case 1:
        if (channel->countdown == 0) {
          channel->countdown = gData_080aa0df[TEXT_SPEED];
          if (work->mode != 0)
            gInput.pendingPressed = 0;
        }
        if (Message_CheckCancelByModeInput(channel) == 0) {
          struct Window *pane = channel->work;

          if (pane->width == 0 || pane->height == 0 || work->unk1388 != 0)
            break;
          Func_0803bde4(pane, 1, pane->width * 4 - 8, pane->height * 8 - 16,
                        1);
          work->unk1388 = 1;
          break;
        }
        window = channel->work;
        entryIndex = channel->entry;
        x = (s16)window->x;
        y = (s16)window->y;
        oldWidth = window->width;
        oldHeight = window->height;
        work->unk1388 = 0;
        WindowSprite_PrepareForRedraw(window);
        if (channel->flags == 0 && (window->width | window->height) != 0)
          Window_EraseBorderRect((s16)window->x, (s16)window->y,
                                   window->width, window->height);
        entryIndex = (entryIndex + 1) & ENTRY_MASK;
        if (work->entries[entryIndex] != 0 &&
            (window->width | window->height) != 0) {
          if (channel->flags != 0) {
            Window_EraseBorderRect((s16)window->x, (s16)window->y,
                                     window->width, window->height);
          } else {
            Window_FitOnScreen(entryIndex, &x, &y, &newWidth, &newHeight,
                                 channel->spacing, 0);
            if (window->flags & 0x80) {
              if (oldHeight != newHeight)
                y -= newHeight - oldHeight;
              if (y < 0)
                y = 0;
            }
            if (!(window->flags & 0x100)) {
              x += (s32)(oldWidth - newWidth) / 4;
              Window_FitOnScreen(entryIndex, &x, &y, &newWidth, &newHeight,
                                   channel->spacing, 2);
            }
            window->x = x;
            window->y = y;
            window->width = newWidth;
            window->height = newHeight;
          }
          Window_DrawBorder((s16)window->x, (s16)window->y, window->width,
                             window->height);
        }
        channel->x = channel->startX;
        channel->y = 0;
        channel->line = 0;
        channel->colour = 15;
        channel->outline = 0;
        channel->lineSpacing = 10;
        Func_0803972c(channel);
        Resource_ResetEntry(work->glyphResource);
        work->glyphResource = 99;
        break;

      case 2:
        if (channel->countdown == 0) {
          channel->countdown = gData_080aa0df[TEXT_SPEED];
          if (work->mode != 0)
            gInput.pendingPressed = 0;
        }
        if (Message_CheckCancelByModeInput(channel) != 0)
          return 9;
        break;

      case 5:
        if (channel->countdown == 0)
          channel->countdown = 20;
        work->speakerVoiceCountdown = 0;
        Message_CheckCancelByInput(channel);
        break;

      case 6:
        if (channel->countdown == 0)
          channel->countdown = 120;
        work->speakerVoiceCountdown = 0;
        Message_CheckCancelByInput(channel);
        break;

      case 4:
        if (channel->countdown == 0)
          channel->countdown = 60;
        work->speakerVoiceCountdown = 0;
        break;

      case 8:
        channel->entry = (channel->entry + 1) & ENTRY_MASK;
        channel->colour = work->entries[channel->entry];
        Func_0803972c(channel);
        break;

      case 9:
        channel->entry = (channel->entry + 1) & ENTRY_MASK;
        channel->outline = work->entries[channel->entry];
        Func_0803972c(channel);
        break;

      case 10:
        channel->entry = (channel->entry + 1) & ENTRY_MASK;
        channel->lineSpacing = work->entries[channel->entry];
        Func_0803972c(channel);
        break;

      case 7:
        channel->colour = 15;
        channel->outline = 0;
        channel->lineSpacing = 10;
        Func_0803972c(channel);
        break;

      case 15:
        channel->entry = (channel->entry + 1) & ENTRY_MASK;
        channel->work->effect = work->entries[channel->entry];
        channel->countdown = 11;
        channel->entry = (channel->entry + 1) & ENTRY_MASK;
        break;

      case 0:
      case 30:
        channel->ended = 1;
        return 8;
      }

      if (work->menuState == 0)
        glyphsLeft = 1;
    } else {
      pixelX = (channel->x + 128) / 256;
      pixelY = (channel->y + 128) / 256;
      voiceGap = gText_SpeedCueGaps[TEXT_SPEED];
      nextEntryIndex = channel->entry;
      if (work->mode != 0)
        pixelX += 8;
      nextEntryIndex = (nextEntryIndex + 1) & ENTRY_MASK;
      nextCode = work->entries[nextEntryIndex];
      if (nextCode == 222) {
        code |= 1 << 14;
        channel->entry = (channel->entry + 1) & ENTRY_MASK;
      } else if (nextCode == 223) {
        code |= 2 << 14;
        channel->entry = (channel->entry + 1) & ENTRY_MASK;
      }
      if (!(channel->work->flags & 8) && code > 32 && nextCode > 32 &&
          (u16)(gFont_Glyphs[code - 32].width +
                gFont_Glyphs[nextCode - 32].width) <= 15) {
        code |= nextCode << 8;
        channel->entry = (channel->entry + 1) & ENTRY_MASK;
      }
      if (work->textMode != 0 && channel->unk26 != 0)
        pixelY = (channel->y + 128) / 320 - 3;
      glyphWidth = Func_0803bde4(channel->work, code, pixelX, pixelY, 0);
      channel->glyphDelay = gText_SpeedDelays[TEXT_SPEED];
      if (glyphWidth == 0)
        return 9;
      if (work->speakerVoiceSound != 0) {
        if (work->speakerVoiceCountdown == 0) {
          if (code != 32) {
            Sound_PlayFar(work->speakerVoiceSound + (code & 3));
            work->speakerVoiceCountdown = voiceGap;
          }
        } else {
          work->speakerVoiceCountdown--;
        }
      }
      glyphAdvance = glyphWidth << 8;
      if (code == 32)
        glyphAdvance += channel->spacing[channel->line];
      channel->x += glyphAdvance;
      if (code == 32 && work->menuState == 0)
        glyphsLeft = 1;
    }

    if (channel->countdown != 0) {
      channel->countdown--;
      if (channel->countdown != 0)
        continue;
    }
    channel->entry = (channel->entry + 1) & ENTRY_MASK;
  } while (--glyphsLeft != 0);

  return 0;
}
