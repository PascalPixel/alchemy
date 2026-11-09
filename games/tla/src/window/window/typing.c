/// @file
/// The per-frame message update: typing text and running its control codes.
// Part of window.c.

#include "../window_internal.h"

#define PANEL_TILE_ROWS ((u32 *)0x06002500)
#define PANEL_ROW_COUNT 30
#define PANEL_ROW_WORDS 32
#define PANEL_ROW_LEAD 8

// Shifts each panel row left by step characters.
void Func_0803975c(s32 step) {
  s32 shift = step * 6;
  u32 *base = PANEL_TILE_ROWS;
  s32 n;

  for (n = 0; n < PANEL_ROW_COUNT; n++) {
    u32 *row = base + n * PANEL_ROW_WORDS;

    Dma_Set(row + PANEL_ROW_LEAD + shift, row + PANEL_ROW_LEAD,
            DMA_COPY_WORDS(PANEL_ROW_WORDS - PANEL_ROW_LEAD - shift), REG_DMA3);
    Iwram_FillWords(row + (PANEL_ROW_WORDS - shift), shift * 4, 0);
  }
}

s32 Message_StepScript(struct UiChannelSlot *channel);
struct Window_0803cbf8;
void Func_0803cbf8(struct Window_0803cbf8 *window);

void Message_UpdateAll(void) {
  u8 *channel = (u8 *)Ram_HeapSlots->windowTiles + 0x6b8;
  s32 channelNo = 0;
  s32 one = 1;

  do {
    u8 *current = *(u8 **)channel;

    if (current != 0 && *(s32 *)(current + 0x18) == 0) {
      u16 flags = *(u16 *)(current + 0x16);

      if (flags == 0) {
        *(u8 **)channel = (u8 *)0;
      } else {
        s32 pending = *(u16 *)(current + 0x12);
        s32 kind;

        if (pending != 0) {
          Func_0803cbf8((struct Window_0803cbf8 *)channel);
        } else {
          kind = Message_StepScript((struct UiChannelSlot *)channel);
          switch (kind) {
          case 8:
            *(u16 *)(*(u8 **)channel + 0x14) = one;
            break;
          case 9: {
            u8 *entity = *(u8 **)channel;

            Window_Close((struct Window *)entity,
                            (u16)(*(u16 *)(entity + 0x16) & 2));
            *(u16 *)(channel + 0x04) = pending;
            *(u16 *)(channel + 0x06) = pending;
            *(u16 *)(channel + 0x12) = pending;
            *(u16 *)(channel + 0x14) = pending;
            *(u16 *)(channel + 0x16) = pending;
            *(u16 *)(channel + 0x18) = pending;
            *(u16 *)(channel + 0x1a) = pending;
            *(u16 *)(*(u8 **)channel + 0x14) = one;
            break;
          }
          }
        }
      }
    }
    channelNo++;
    channel += 0x28;
  } while (channelNo != 3);
}

extern const u8 gText_SpeedDelays[];
extern const u8 gData_080aa0df[];
extern const u8 gText_SpeedSteps[];
extern const u8 gText_SpeedCueGaps[];

void Func_0803975c(s32 step);
void Func_08039754(struct UiChannelSlot *channel);
void Func_0803972c(struct UiChannelSlot *channel);
s32 Message_CheckCancelByInput(struct UiChannelSlot *channel);
s32 Message_CheckCancelByModeInput(struct UiChannelSlot *channel);
void WindowSprite_PrepareForRedraw(struct Window *window);
void Window_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
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

  /* FAKEMATCH(volatile): the form kept; as a plain read of gInput.held, 31 of
   * 697 instructions differ in tla-en. */
  (void)keys;

  if (work->menuState != 0) {
    /* FAKEMATCH(volatile): the form kept; as a plain read of gLastFrameVBlanks,
     * 25 of 697 instructions differ in tla-en. */
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

#if LANGUAGE != LANGUAGE_JAPANESE
    if (code <= 30 || code == 176) {
#else
    if (code <= 30) {
#endif
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
          Func_0803bde4(pane, 1, pane->width * 4 - 8, pane->height * 8 - 16, 1);
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
          Window_EraseBorderRect((s16)window->x, (s16)window->y, window->width,
                                 window->height);
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
#if !TLA_PATCH_2
      if (work->mode != 0)
        pixelX += 8;
#endif
      nextEntryIndex = (nextEntryIndex + 1) & ENTRY_MASK;
      nextCode = work->entries[nextEntryIndex];
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
      if (nextCode == 222) {
        code |= 1 << 14;
        channel->entry = (channel->entry + 1) & ENTRY_MASK;
      } else if (nextCode == 223) {
        code |= 2 << 14;
        channel->entry = (channel->entry + 1) & ENTRY_MASK;
      }
#endif
#if LANGUAGE != LANGUAGE_JAPANESE
#if TLA_PATCH_4
      if (!(channel->work->flags & 8) && code > 32 && nextCode > 32) {
        s16 pairWidth =
            gFont_Glyphs[code - 32].width + gFont_Glyphs[nextCode - 32].width;

        if (work->outline == 1) {
          if ((u16)pairWidth <= 14) {
            code |= nextCode << 8;
            channel->entry = (channel->entry + 1) & ENTRY_MASK;
          }
        } else if ((u16)pairWidth <= 15) {
          code |= nextCode << 8;
          channel->entry = (channel->entry + 1) & ENTRY_MASK;
        }
      }
#else
      if (!(channel->work->flags & 8) && code > 32 && nextCode > 32 &&
          (u16)(gFont_Glyphs[code - 32].width +
                gFont_Glyphs[nextCode - 32].width) <= 15) {
        code |= nextCode << 8;
        channel->entry = (channel->entry + 1) & ENTRY_MASK;
      }
#endif
#endif
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
#if LANGUAGE == LANGUAGE_JAPANESE
      glyphAdvance += channel->spacing[channel->line];
#elif LANGUAGE != LANGUAGE_GERMAN
      if (code == 32)
        glyphAdvance += channel->spacing[channel->line];
#endif
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

#undef PANEL_TILE_ROWS
#undef PANEL_ROW_COUNT
#undef PANEL_ROW_WORDS
#undef PANEL_ROW_LEAD
#undef TEXT_SPEED
#undef ENTRY_MASK
