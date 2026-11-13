/// @file
/// Lucky Wheels, from LuckyWheels_Start to the LZ packer that ends its code,
/// in the order it has in the ROM.

#include "types.h"
#include "task.h"
#include "io_reg.h"
#include "main_routine_work.h"
#include "system.h"
#include "dma.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "resource.h"
#include "input.h"
#include "frame.h"
#include "effect_step.h"
#include "edition.h"
#include "version.h"
#include "tla_edition.h"
#include "fixed_math.h"
#include "bg_scroll.h"
#include "lucky_wheels_lzpack.h"

void Sound_PlayFar(s32 cue);
void LuckyWheels_Run(void);

/// Blanks the display but for 1D objects, seeds the random state from the
/// play time, plays sound 709 and runs Lucky Wheels.
// As The Broken Seal's.
// XXX: sound 709's name; The Lost Age has no sounds.h yet, and The Broken
// Seal plays MUS_DJINN_LESSON here.
s32 LuckyWheels_Start(void) {
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
  gRandomState = gMainRoutineWork.playTime;
  Sound_PlayFar(709);
  LuckyWheels_Run();
  return 0;
}

s32 Graphics_ScaleRgb555(u16 *source, u16 *destination, s32 scale, s32 count) {
  s32 remaining;
  u32 red_mask;
  u32 green_mask;
  u32 blue_mask;
  u32 pixel;
  u32 red;
  u32 green;
  u32 blue;

  if (count > 0) {
    red_mask = 0x1f;
    green_mask = 0x3e0;
    blue_mask = 0x7c00;
    remaining = count;
    do {
      pixel = *source;
      red = pixel & red_mask;
      green = pixel & green_mask;
      blue = blue_mask & pixel;
      red *= scale;
      green *= scale;
      blue *= scale;
      pixel = ((red >> 16) & red_mask) | ((green >> 16) & green_mask);
      pixel |= (blue >> 16) & blue_mask;
      *destination = pixel;
      source++;
      destination++;
      remaining--;
    } while (remaining != 0);
  }
  return 0;
}

struct BattleFxTransferWork {
  u8 unk0000[0x7780];
  s32 transferMode;
  s32 transferValue;
  u8 unk_7788[0x78e4 - 0x7788];
  s32 vblanksSinceTransfer;
  s32 transferPending;
};

void ColorBuffer_HalveNonzero(u8 *source, u8 *destination, s32 size);
void ColorBuffer_ScaleNonzeroThreeQuarters(u8 *source, u8 *destination,
                                           s32 size);

static __inline__ s32 BattleStaging_FillWords(void *destination, s32 size,
                                              u32 value) {
  return Iwram_FillWords(destination, size, value);
}

/// Sends the 256x128 8bpp canvas to VRAM 0x06003500 on the frames its transfer
/// is pending, then clears or fades it; a task, as The Broken
/// Seal's.
void BattleStaging_ProcessPendingTileTransfer(void) {
  struct BattleFxTransferWork *work;
  u8 *buf;

  work = (struct BattleFxTransferWork *)Ram_HeapSlots->battleFxWork[0];
  if (work->transferPending == 1) {
    buf = Ram_HeapSlots->battleFxWork[1];
    switch (work->transferMode) {
    case 1: // Send it, then fill it with transferValue.
      Dma_Set(buf, (void *)0x06003500, DMA_COPY_WORDS(0x8000 / 4), REG_DMA3);
      /* FAKEMATCH(wrapper): the fill called through the inline
       * BattleStaging_FillWords, which loads the routine's address before the
       * size; as Iwram_FillWords called directly, 4 of 79 instructions differ
       * in tla-en. */
      BattleStaging_FillWords(buf, 0x8000, work->transferValue);
      break;
    case 2: // Afterimage: nonzero colour numbers drop to 1/2 or 3/4.
      if (work->transferValue == 50)
        ColorBuffer_HalveNonzero(buf, (u8 *)0x06003500, 0x8000);
      else
        ColorBuffer_ScaleNonzeroThreeQuarters(buf, (u8 *)0x06003500, 0x8000);
      break;
    }
    work->transferPending = 0;
    work->vblanksSinceTransfer = 1;
  } else {
    work->vblanksSinceTransfer++;
  }
}

void Palette_DarkenSceneStep(void) {
  u16 *color;
  s32 i;

  color = (u16 *)0x05000140;
  for (i = 0; i != 16; i++) {
    s32 blue = (*color >> 10) & 0x1f;
    s32 green = (*color >> 5) & 0x1f;
    s32 red = *color & 0x1f;

    blue--;
    green--;
    red--;

    if (blue < 0)
      blue = 0;
    if (green < 0)
      green = 0;
    if (red < 0)
      red = 0;
    *color++ = RGB(red, green, blue);
  }
  color = (u16 *)0x05000202;
  for (i = 0; i != 239; i++) {
    s32 blue = (*color >> 10) & 0x1f;
    s32 green = (*color >> 5) & 0x1f;
    s32 red = *color & 0x1f;

    blue--;
    green--;
    red--;

    if (blue < 0)
      blue = 0;
    if (green < 0)
      green = 0;
    if (red < 0)
      red = 0;
    *color++ = RGB(red, green, blue);
  }
}

/// Moves the first 64 BG colours one step toward the palette of resource; the
/// backdrop colour is not written back.
void Palette_StepTowardResource(s32 resource) {
  u16 dst[64];
  u16 *col = (u16 *)BG_PLTT;
  s32 i;

  Dma_Set(Resource_GetTableEntry(resource), dst, DMA_COPY_WORDS(0x20),
          REG_DMA3);
  for (i = 0; i != 64; i++, col++) {
    s32 r = *col & 0x1f;
    s32 g = (*col >> 5) & 0x1f;
    s32 b = (*col >> 10) & 0x1f;
    s32 targetRed = dst[i] & 0x1f;
    s32 targetGreen = (dst[i] >> 5) & 0x1f;
    s32 targetBlue = (dst[i] >> 10) & 0x1f;

    if (r < targetRed)
      r++;
    else if (r > targetRed)
      r--;
    if (g < targetGreen)
      g++;
    else if (g > targetGreen)
      g--;
    if (b < targetBlue)
      b++;
    else if (b > targetBlue)
      b--;
    dst[i] = RGB(r, g, b);
  }
  Dma_Set(dst + 1, (void *)&BG_PLTT[1], DMA_COPY_HALFWORDS(63), REG_DMA3);
}

#define ABS(v) ((v) < 0 ? -(v) : (v))

/// Draws a line from (x0, y0) to (x1, y1) on the 256 x 128 8bpp canvas (32
/// tiles across), writing only over lower colour numbers.
void LuckyWheels_DrawCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color) {
  s32 dx = x1 - x0;
  s32 dy = y1 - y0;
  s32 frac = 0x80;
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  s32 step;
  s32 i;
  s32 x;
  s32 y;
  s32 t;
  u32 off;

  if (ABS(dx) < ABS(dy)) {
    if (dy < 0) {
      t = x0;
      x0 = x1;
      x1 = t;
      t = y0;
      y0 = y1;
      y1 = t;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (ABS(x1 - x0) << 8) / ABS(y1 - y0);
    x = x0;
    for (i = y0; i != y1; i++) {
      off = (((((u32)i >> 3) << 5) + ((u32)x >> 3)) << 3) + (i & 7);
      off = (off << 3) + (x & 7);
      if (canvas[off] < color)
        canvas[off] = color;
      frac += step;
      if (frac & 0x100) {
        if (dx > 0)
          x++;
        else
          x--;
        frac &= ~0x100;
      }
    }
  } else {
    if (dx < 0) {
      t = x0;
      x0 = x1;
      x1 = t;
      t = y0;
      y0 = y1;
      y1 = t;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (ABS(y1 - y0) << 8) / ABS(x1 - x0);
    y = y0;
    for (i = x0; i != x1; i++) {
      off = (((((u32)y >> 3) << 5) + ((u32)i >> 3)) << 3) + (y & 7);
      off = (off << 3) + (i & 7);
      if (canvas[off] < color)
        canvas[off] = color;
      frac += step;
      if (frac & 0x100) {
        if (dy > 0)
          y++;
        else
          y--;
        frac &= ~0x100;
      }
    }
  }
}

// The prompt windows (all held, start, hold, release, stop the reels, choose
// the reels to hold) and the ticket count's digits, placed and sized for each
// language's words. Japanese shows hold and release in one window.
#if LANGUAGE == LANGUAGE_JAPANESE
#define LUCKY_WHEELS_ALL_HELD_X 12
#define LUCKY_WHEELS_ALL_HELD_WIDTH 18
#define LUCKY_WHEELS_START_X 18
#define LUCKY_WHEELS_START_WIDTH 12
#elif LANGUAGE == LANGUAGE_ITALIAN
#define LUCKY_WHEELS_ALL_HELD_X 10
#define LUCKY_WHEELS_ALL_HELD_WIDTH 20
#define LUCKY_WHEELS_START_X 21
#define LUCKY_WHEELS_START_WIDTH 9
#elif LANGUAGE == LANGUAGE_SPANISH
#define LUCKY_WHEELS_ALL_HELD_X 6
#define LUCKY_WHEELS_ALL_HELD_WIDTH 24
#define LUCKY_WHEELS_START_X 22
#define LUCKY_WHEELS_START_WIDTH 8
#else
#define LUCKY_WHEELS_ALL_HELD_X 11
#define LUCKY_WHEELS_ALL_HELD_WIDTH 19
#define LUCKY_WHEELS_START_X 16
#define LUCKY_WHEELS_START_WIDTH 14
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
#define LUCKY_WHEELS_HOLD_X 18
#define LUCKY_WHEELS_HOLD_WIDTH 12
#elif LANGUAGE == LANGUAGE_GERMAN
#define LUCKY_WHEELS_HOLD_X 22
#define LUCKY_WHEELS_HOLD_WIDTH 8
#define LUCKY_WHEELS_RELEASE_X 21
#define LUCKY_WHEELS_RELEASE_WIDTH 9
#elif LANGUAGE == LANGUAGE_ITALIAN
#define LUCKY_WHEELS_HOLD_X 21
#define LUCKY_WHEELS_HOLD_WIDTH 9
#define LUCKY_WHEELS_RELEASE_X 21
#define LUCKY_WHEELS_RELEASE_WIDTH 9
#elif LANGUAGE == LANGUAGE_SPANISH
#define LUCKY_WHEELS_HOLD_X 22
#define LUCKY_WHEELS_HOLD_WIDTH 8
#define LUCKY_WHEELS_RELEASE_X 22
#define LUCKY_WHEELS_RELEASE_WIDTH 8
#else
#define LUCKY_WHEELS_HOLD_X 23
#define LUCKY_WHEELS_HOLD_WIDTH 7
#define LUCKY_WHEELS_RELEASE_X 23
#define LUCKY_WHEELS_RELEASE_WIDTH 7
#endif
#if LANGUAGE == LANGUAGE_FRENCH
#define LUCKY_WHEELS_COIN_DIGITS 3
#define LUCKY_WHEELS_COIN_X 56
#else
#define LUCKY_WHEELS_COIN_DIGITS 2
#define LUCKY_WHEELS_COIN_X 64
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
#define LUCKY_WHEELS_STOP_X 23
#define LUCKY_WHEELS_STOP_WIDTH 7
#elif LANGUAGE == LANGUAGE_SPANISH
#define LUCKY_WHEELS_STOP_X 21
#define LUCKY_WHEELS_STOP_WIDTH 9
#else
#define LUCKY_WHEELS_STOP_X 18
#define LUCKY_WHEELS_STOP_WIDTH 12
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
#define LUCKY_WHEELS_CHOOSE_HELD_X 7
#define LUCKY_WHEELS_CHOOSE_HELD_WIDTH 16
#else
#define LUCKY_WHEELS_CHOOSE_HELD_X 3
#define LUCKY_WHEELS_CHOOSE_HELD_WIDTH 24
#endif
// The title's letters; the Spanish title's first letter is its last letter's
// tile turned over both ways, drawn 16 pixels left and 8 down.
#if LANGUAGE == LANGUAGE_SPANISH
#define REEL_TITLE_LETTERS 9
#define REEL_TITLE_FIRST_TURNED 1
#else
#define REEL_TITLE_LETTERS 8
#define REEL_TITLE_FIRST_TURNED 0
#endif
// The first tile of the title's letters: the Japanese letters take fewer tiles,
// so they start later in VRAM (0x06017000 rather than 0x06016e00) and end at
// the same place.
#if LANGUAGE == LANGUAGE_JAPANESE
#define REEL_TITLE_TILE 0x380
#else
#define REEL_TITLE_TILE 0x370
#endif

struct Reel {
  s32 position;
  u8 symbols[21];
  u8 held;
  s8 stop;
};

struct Window;

struct ReelSprite {
  volatile u32 attr01;
  volatile u32 attr2;
};

struct ReelWork {
  struct Reel reels[5];
  s32 state;
  s32 cursor;
  s32 spins;
  s32 bet;
  u16 keys;
  u16 direction;
  u16 pressed;
  u16 repeat;
  s32 timer;
  s32 spriteCount;
  s32 winningLines[7];
  struct ReelSprite obj[128];
  struct Window *window;
  struct Window *subWindow;
  u8 unk4d0[8];
  u16 scanlineOffsets[160];
#if TLA_PATCH_1
  // The Japanese release has no phase: it closes and reopens its prompt
  // window every frame.
  s32 phase;
#endif
};

struct ReelEffectWork {
  struct EffectStep particles[REEL_TITLE_LETTERS];
  u8 unkAfterParticles[0x700 - REEL_TITLE_LETTERS * sizeof(struct EffectStep)];
  u16 bgShade[0x100];
  u16 objShade[0x100];
  u8 unk0b00[0x7780 - 0xb00];
  s32 transferMode;
  s32 transferValue;
  u8 unk7788[4];
  s32 frame;
  u8 unk7790[0x78e8 - 0x7790];
  s32 transferPending;
};

struct ReelSave {
  u8 unk000[0x140];
  s8 wonPrizes[16];
};

extern u8 Data_0200024c[];
extern const u8 gLuckyWheels_SymbolPrizes[];
extern const u8 gLuckyWheels_TitleLetterTiles[];
extern const u8 gLuckyWheels_MachineSpriteX[];
extern const u8 gLuckyWheels_MachineSpriteY[];

s32 PartyInventory_CountItemFar(s32 item);
void PartyInventory_RemoveFar(s32 item);
void Text_DrawNumberInWindowFar(s32 value, s32 width, struct Window *window,
                                s32 x, s32 y);
void Window_CloseFar(struct Window *window, s32 mode);
struct Window *Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 flags);
void Text_DrawCharacterAtOffsetFar(s32 message, struct Window *window, s32 x,
                                   s32 y);

extern char MsgSlotsBet[];
extern char MsgSlotsStopReels[];
extern char MsgSlotsChooseHeld[];
extern char MsgSlotsPullLever[];
extern char MsgSlotsHold[];
extern char MsgSlotsRelease[];
extern char MsgSlotsStart[];
extern char MsgSlotsAllHeld[];
extern char MsgSlotsCannotPull[];

void Func_081b243c(void) {
  struct ReelEffectWork *fx;
  s32 objMode;
  u16 keys;
  u32 dpad;
  s32 tickets;
  s32 allHeld;
  s32 stoppedReels;
  s32 col;
  s32 v;
  s32 x;
  s32 y;
  s32 lever;
  s32 k;
  s32 i;
  struct ReelWork *work;

  work = (struct ReelWork *)Ram_HeapSlots->canvasMap;
  fx = (struct ReelEffectWork *)Ram_HeapSlots->battleFxWork[0];
  objMode = 0x400;
  work->spriteCount = 0;

  Random16();

  keys = *(volatile u32 *)&gInput.held;
  dpad = *(volatile u32 *)&gInput.held & 0xf0;
  work->pressed = keys & ~work->keys;
  work->direction = dpad;
  if ((work->keys & 0xf0) == work->direction) {
    if (work->repeat > 12)
      work->repeat = 12;
    if (work->repeat == 0) {
      work->repeat = 4;
    } else {
      work->repeat--;
      work->direction = 0;
    }
  } else {
    work->repeat = 12;
  }
  work->keys = keys;

  if (gDebugPaused == 0) {
    if (work->state == 0) {
      tickets = PartyInventory_CountItemFar(228);
      Text_DrawNumberInWindowFar(tickets - work->bet, LUCKY_WHEELS_COIN_DIGITS,
                                 work->subWindow, LUCKY_WHEELS_COIN_X, 0);
      Text_DrawNumberInWindowFar(work->bet, 2, work->subWindow, 64, 8);
      if (work->pressed & KEY_B) {
        struct ReelSave *save = (struct ReelSave *)Data_0200024c;

        work->state = 10;
        save->wonPrizes[0] = -2;
        Window_CloseFar(work->window, 1);
      } else {
        if (work->pressed & KEY_UP) {
          if (work->bet <= 3 && tickets > work->bet) {
            work->bet++;
            Sound_PlayFar(111);
          } else {
            Sound_PlayFar(113);
          }
        }
        if (work->pressed & KEY_DOWN) {
          if (work->bet > 1) {
            work->bet--;
            Sound_PlayFar(111);
          } else {
            Sound_PlayFar(113);
          }
        }
        REG_BLDCNT = 0x3fd0;
        REG_BLDALPHA = 0x10;
        if (work->pressed & KEY_A) {
          work->state = 1;
          fx->frame = 0;
          Window_CloseFar(work->window, 1);
          for (k = 0; k != work->bet; k++)
            PartyInventory_RemoveFar(228);
          Window_CloseFar(work->subWindow, 1);
          Sound_PlayFar(304);
        }
      }
    } else if (work->state == 5) {
      allHeld = 0;
      work->timer++;
      for (k = 0; k != 5 && work->reels[k].held != 0; k++)
        ;
      if (k == 5)
        allHeld = 1;
      if (work->pressed & KEY_A) {
        work->timer = 0;
        fx->frame = 0;
        if (work->spins == 4) {
          work->spins = 0;
          work->cursor = 0;
          work->state = 0;
          for (i = 0; i != 5; i++) {
            work->reels[i].held = 0;
            work->reels[i].stop = -1;
          }
        } else if (work->cursor <= 4) {
          Sound_PlayFar(305);
          work->reels[work->cursor].held ^= 1;
        } else if (allHeld == 0) {
          Sound_PlayFar(304);
          work->state = 1;
          work->cursor = 0;
          for (i = 0; i != 5; i++)
            work->reels[i].stop = -1;
          work->spins++;
        } else {
          Sound_PlayFar(113);
        }
      } else {
        if (work->direction & KEY_RIGHT) {
          work->cursor = (work->cursor + 1) % 6;
          Sound_PlayFar(111);
        }
        if (work->direction & KEY_LEFT) {
          work->cursor = (work->cursor + 5) % 6;
          Sound_PlayFar(111);
        }
      }

      if (work->state == 5) {
#if TLA_PATCH_1
        if (work->cursor == 5) {
          if (allHeld != 0) {
            if (work->phase != 1 && work->phase != 2) {
              Window_CloseFar(work->window, 1);
              work->window =
                  Window_CreateFar(LUCKY_WHEELS_ALL_HELD_X, 0,
                                   LUCKY_WHEELS_ALL_HELD_WIDTH, 4, 6);
              Text_DrawCharacterAtOffsetFar((s32)MsgSlotsAllHeld, work->window,
                                            0, 0);
              work->phase = 1;
            } else if (work->phase == 1) {
              Text_DrawCharacterAtOffsetFar((s32)MsgSlotsCannotPull,
                                            work->window, 0, 8);
              work->phase = 2;
            }
          } else {
            if (work->phase != 3) {
              Window_CloseFar(work->window, 1);
              work->window = Window_CreateFar(LUCKY_WHEELS_START_X, 0,
                                              LUCKY_WHEELS_START_WIDTH, 3, 6);
              Text_DrawCharacterAtOffsetFar((s32)MsgSlotsStart, work->window, 0,
                                            0);
            }
            work->phase = 3;
          }
        } else if (work->reels[work->cursor].held == 0) {
          if (work->phase != 4) {
            Window_CloseFar(work->window, 1);
            work->window = Window_CreateFar(LUCKY_WHEELS_HOLD_X, 0,
                                            LUCKY_WHEELS_HOLD_WIDTH, 3, 6);
            Text_DrawCharacterAtOffsetFar((s32)MsgSlotsHold, work->window, 0,
                                          0);
          }
          work->phase = 4;
        } else {
          if (work->phase != 5) {
            Window_CloseFar(work->window, 1);
            work->window = Window_CreateFar(LUCKY_WHEELS_RELEASE_X, 0,
                                            LUCKY_WHEELS_RELEASE_WIDTH, 3, 6);
            Text_DrawCharacterAtOffsetFar((s32)MsgSlotsRelease, work->window, 0,
                                          0);
          }
          work->phase = 5;
        }
#else
        Window_CloseFar(work->window, 1);
        if (work->cursor == 5) {
          if (allHeld != 0) {
            work->window = Window_CreateFar(LUCKY_WHEELS_ALL_HELD_X, 0,
                                            LUCKY_WHEELS_ALL_HELD_WIDTH, 3, 6);
            Text_DrawCharacterAtOffsetFar((s32)MsgSlotsAllHeld, work->window, 0,
                                          0);
          } else {
            work->window = Window_CreateFar(LUCKY_WHEELS_START_X, 0,
                                            LUCKY_WHEELS_START_WIDTH, 3, 6);
            Text_DrawCharacterAtOffsetFar((s32)MsgSlotsStart, work->window, 0,
                                          0);
          }
        } else {
          work->window = Window_CreateFar(LUCKY_WHEELS_HOLD_X, 0,
                                          LUCKY_WHEELS_HOLD_WIDTH, 3, 6);
          if (work->reels[work->cursor].held == 0)
            Text_DrawCharacterAtOffsetFar((s32)MsgSlotsHold, work->window, 0,
                                          0);
          else
            Text_DrawCharacterAtOffsetFar((s32)MsgSlotsRelease, work->window, 0,
                                          0);
        }
#endif
      } else {
        Window_CloseFar(work->window, 1);
      }
    } else if (work->state == 2) {
      work->timer++;
      objMode = 0;
      if (work->timer == 60) {
        work->state = 3;
        Sound_PlayFar(93);
        work->timer = 0;
        REG_BLDCNT = 0x3f44;
        REG_BLDALPHA = 0x1010;
        fx->transferMode = 2;
        fx->transferValue = 75;
      }
    } else if (work->state == 3) {
      work->timer++;
      objMode = 0;
      if (work->pressed & KEY_A) {
        work->state = 10;
        Sound_PlayFar(112);
      }
    } else if (work->state == 11) {
#if TLA_PATCH_1
      if (work->phase == 0) {
        work->phase = 1;
        Text_DrawCharacterAtOffsetFar((s32)MsgSlotsPullLever, work->window, 0,
                                      8);
      }
#endif
      if (work->pressed & KEY_A) {
        work->state = 5;
#if TLA_PATCH_1
        work->phase = 0;
#endif
        Sound_PlayFar(112);
        Window_CloseFar(work->window, 1);
      }
    } else if (work->state == 20) {
      work->timer++;
      if (work->timer == 45)
        work->state = 10;
    } else if (work->state != 10) {
      if (work->timer == 4) {
        work->window = Window_CreateFar(LUCKY_WHEELS_STOP_X, 17,
                                        LUCKY_WHEELS_STOP_WIDTH, 3, 6);
        Text_DrawCharacterAtOffsetFar((s32)MsgSlotsStopReels, work->window, 0,
                                      0);
      }
      if (work->timer == 16)
        Sound_PlayFar(306);
      if (work->timer > 56) {
        if (fx->frame > 31 || (work->pressed & KEY_R)) {
          fx->frame = 0;
          for (i = 0; i != 5; i++) {
            if (work->reels[i].held == 0 && work->reels[i].stop == -1) {
              work->reels[i].stop = (Random16() & 3) + 4;
              Sound_PlayFar(307);
              break;
            }
          }
        }
      }

      for (i = 0; i != 5; i++) {
        if (work->reels[i].stop > 0)
          work->reels[i].stop--;
      }

      stoppedReels = 0;
      for (i = 0; i != 5; i++) {
        if (work->reels[i].held == 1)
          stoppedReels++;
        else if (work->reels[i].stop == 0 &&
                 (work->reels[i].position & 15) == 8)
          stoppedReels++;
      }

      if (stoppedReels == 5) {
        struct ReelSave *save = (struct ReelSave *)Data_0200024c;
        s32 hits = 0;

        for (col = 0; col != 7; col++) {
          s32 mixed;
          s32 found;

          found = -1;
          mixed = 0;
          work->winningLines[col] = 0;
          if (col > 3 - work->bet && col < work->bet + 3) {
            for (i = 0; i != 5; i++) {
              s32 sym;

              if (col == 0)
                sym =
                    work->reels[i]
                        .symbols[(i - work->reels[i].position / 16 + 22) % 21];
              else if (col == 6)
                sym =
                    work->reels[i]
                        .symbols[(-i - work->reels[i].position / 16 + 26) % 21];
              else
                sym = work->reels[i]
                          .symbols[(col - work->reels[i].position / 16 + 21) %
                                   21];
              if (sym != 5) {
                if (found == -1)
                  found = sym;
                else if (found != sym)
                  mixed = 1;
              }
            }
            if (mixed == 0) {
              work->winningLines[col] = 1;
              save->wonPrizes[hits] = gLuckyWheels_SymbolPrizes[found];
              hits++;
            }
          }
        }
        work->timer = 0;
        if (hits != 0) {
          save->wonPrizes[hits] = -1;
          work->state = 2;
          Sound_PlayFar(171);
          fx->transferMode = 1;
          fx->transferValue = 0;
          REG_BLDCNT = 0;
          Window_CloseFar(work->window, 1);
        } else {
          work->state = 11;
#if TLA_PATCH_1
          work->phase = 0;
#endif
          Window_CloseFar(work->window, 1);
          if (work->spins <= 3) {
            s32 message;

            work->window =
                Window_CreateFar(LUCKY_WHEELS_CHOOSE_HELD_X, 16,
                                 LUCKY_WHEELS_CHOOSE_HELD_WIDTH, 4, 6);
            message = (s32)MsgSlotsChooseHeld;
            Text_DrawCharacterAtOffsetFar(message, work->window, 0, 0);
            Text_DrawCharacterAtOffsetFar(message + 1, work->window, 0, 8);
          }
          if (work->spins == 4) {
            s32 left = PartyInventory_CountItemFar(228);
            struct Window *window;
            s32 message;

            // DEAD: both arms set state 20, so the ticket count cannot change
            // it; the ROM keeps the compare and both stores.
            if (left > 0)
              work->state = 20;
            else
              work->state = 20;
            if (work->bet > left)
              work->bet = left;
            work->spins = 0;
            work->cursor = 0;
            for (i = 0; i != 5; i++) {
              work->reels[i].held = 0;
              work->reels[i].stop = -1;
            }
            fx->transferMode = 1;
            fx->transferValue = 0;
            REG_BLDCNT = 0;
            window = Window_CreateFar(18, 0, 12, 4, 6);
            work->subWindow = window;
            message = (s32)MsgSlotsBet;
            Text_DrawCharacterAtOffsetFar(message, window, 0, 8);
            Text_DrawCharacterAtOffsetFar(message - 1, work->subWindow, 0, 0);
          }
        }
      }

      if (work->state == 1) {
        for (i = 0; i != 5; i++) {
          struct Reel *reel = &work->reels[i];

          if (reel->held == 0) {
            if (reel->stop != 0)
              reel->position += 8;
            else if ((reel->position & 15) != 8)
              reel->position += 8;
            if (reel->position == 336)
              reel->position = 0;
          }
        }
      }
      (fx->frame)++;
      work->timer++;
    }
  }

  if (work->state == 5) {
    x = work->cursor * 36 + 36;
    y = 128;
    v = 0;
    if ((work->timer & 15) <= 7)
      v = 1;
    if (work->cursor == 5) {
      x = 208;
      y = 32;
    }
    work->obj[work->spriteCount].attr01 =
        ((x - 12) << 16) | objMode | (y + 8) | 0x80006000;
    work->obj[work->spriteCount].attr2 = (v << 4) + 0x2b0;
    work->spriteCount++;
    work->obj[work->spriteCount].attr01 =
        ((x + 12) << 16) | objMode | (y + 8) | 0x90006000;
    work->obj[work->spriteCount].attr2 = (v << 4) + 0x2b0;
    work->spriteCount++;
    work->obj[work->spriteCount].attr01 = (x << 16) | objMode | y | 0x80002000;
    work->obj[work->spriteCount].attr2 = 0x1f0;
    work->spriteCount++;
  }

  if (work->state == 3) {
    for (i = 0; i != REEL_TITLE_LETTERS; i++) {
      struct EffectStep *p = &fx->particles[i];

      if (REEL_TITLE_FIRST_TURNED && i == 0)
        work->obj[work->spriteCount].attr01 =
            (((*(s16 *)((u8 *)&p->x + 2) + 496) & 0x1ff) << 16) | objMode |
            ((*(s16 *)((u8 *)&p->y + 2) + 264) & 255) | 0xb0000000;
      else
        work->obj[work->spriteCount].attr01 =
            (*(s16 *)((u8 *)&p->x + 2) << 16) | objMode |
            ((*(s16 *)((u8 *)&p->y + 2) + 256) & 255) | 0x80000000;
      work->obj[work->spriteCount].attr2 =
          ((gLuckyWheels_TitleLetterTiles[i] << 4) + REEL_TITLE_TILE) | 0xf000;
#if TLA_PATCH_1
      // The Japanese release draws the letters without moving them.
      p->y += p->velocityY;
      p->velocityY += 0x4000;
      if (work->timer % 256 == i * 4 + 200) {
        p->velocityY = 0x60000;
        p->variant = 0;
      }
      if (p->y > 0x400000) {
        p->y = 0x400000;
        if (p->variant <= 1)
          p->velocityY = -p->velocityY / 2;
        p->variant++;
      }
#endif
      work->spriteCount++;
    }
  }

  for (k = 0; k != 14; k++) {
    work->obj[work->spriteCount].attr01 =
        (gLuckyWheels_MachineSpriteX[k] << 16) | objMode |
        gLuckyWheels_MachineSpriteY[k] | 0x80006000;
    if (k <= 3)
      work->obj[work->spriteCount].attr2 = 0x4e0;
    else
      work->obj[work->spriteCount].attr2 = 0x4e8;
    work->spriteCount++;
  }

  for (i = 0; i != 5; i++) {
    if (work->reels[i].held == 0) {
      work->obj[work->spriteCount].attr01 =
          objMode | ((i * 36 + 32) << 16) | 0x8000207c;
      work->obj[work->spriteCount].attr2 = 0x460;
    } else {
      work->obj[work->spriteCount].attr01 =
          objMode | ((i * 36 + 32) << 16) | 0x8000207c;
      work->obj[work->spriteCount].attr2 = 0x480;
    }
    work->spriteCount++;
  }

  for (i = 0; i != 5; i++) {
    work->obj[work->spriteCount].attr01 =
        objMode | ((i * 16 + 32) << 16) | 0x80006003;
    if (i == work->spins)
      work->obj[work->spriteCount].attr2 = (i * 32 + 0x210) | 0x400;
    else
      work->obj[work->spriteCount].attr2 = (i * 32 + 0x220) | 0x400;
    work->spriteCount++;
  }

  for (col = 0; col != 7; col++) {
    work->obj[work->spriteCount].attr01 =
        (((0x204 - ((col & 1) << 3)) & 0x1ff) << 16) | objMode |
        (col * 16 + 5) | 0x80002000;
    if (col > 3 - work->bet && col < work->bet + 3)
      work->obj[work->spriteCount].attr2 = 0x5d0;
    else
      work->obj[work->spriteCount].attr2 = 0x510;
    work->spriteCount++;
  }

  for (i = 0; i != 5; i++) {
    struct Reel *reel = &work->reels[i];

    for (col = 0; col != 7; col++) {
      s32 symbolY = col * 16 + reel->position % 16;
      s32 index = work->spriteCount;

      symbolY += 4;
      work->obj[index].attr01 = symbolY | ((i * 32 + 40) << 16) | 0x80006000;
      work->obj[work->spriteCount].attr2 =
          (reel->symbols[(col - reel->position / 16 + 21) % 21] << 4) | 0x800;
      work->spriteCount++;
    }
  }

  lever = 40;
  if (work->state == 1) {
    if (work->timer <= 47)
      lever = ((Trig_Sin(work->timer * 682) << 6) >> 16) + 40;
    else if (work->timer <= 55)
      lever = ((Trig_Sin((work->timer << 12) - 0x30000) << 2) >> 16) + 40;
  }
  work->obj[work->spriteCount].attr01 = lever | objMode | 0x80d06000;
  work->obj[work->spriteCount].attr2 = 0x500;
  work->spriteCount++;

  for (k = 0; k != 8; k++) {
    work->obj[work->spriteCount].attr01 = objMode | (k * 16 + 12) | 0x80ce6000;
    if (k == 0)
      work->obj[work->spriteCount].attr2 = 0x540;
    else if (k == 1)
      work->obj[work->spriteCount].attr2 = 0x550;
    else if (k == 6)
      work->obj[work->spriteCount].attr2 = 0x570;
    else if (k == 7)
      work->obj[work->spriteCount].attr2 = 0x580;
    else
      work->obj[work->spriteCount].attr2 = 0x560;
    work->spriteCount++;
  }

  while (work->spriteCount != 128) {
    work->obj[work->spriteCount].attr01 = 0x40f02000;
    work->obj[work->spriteCount].attr2 = 0;
    work->spriteCount++;
  }
}

// Lucky Wheels' tables, which end the bank, are The Broken Seal's.

/// The slot machine's per-frame tail: while a win shows (state 3) it bounces
/// the eight title letters, then restarts the HBlank DMA of the scanline
/// offsets into BLDY and copies the frame's sprites to OAM.
// Split out of what is ☀️'s LuckyWheels_Update.
void Func_081b3294(void) {
  struct ReelWork *work;
  struct ReelEffectWork *fx;
  volatile u16 *dma;
  s32 i;

  work = (struct ReelWork *)Ram_HeapSlots->canvasMap;
  fx = (struct ReelEffectWork *)Ram_HeapSlots->battleFxWork[0];
  if (work->state == 3) {
    for (i = 0; i != 8; i++) {
      struct EffectStep *p = &fx->particles[i];

      p->y += p->velocityY;
      p->velocityY += 0x4000;
      if (work->timer % 256 == i * 4 + 200) {
        p->velocityY = 0x60000;
        p->variant = 0;
      }
      if (p->y > 0x400000) {
        p->y = 0x400000;
        if (p->variant <= 1)
          p->velocityY = -p->velocityY / 2;
        p->variant++;
      }
    }
  }

  dma = REG_DMA0;
  dma[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
  dma[5] &= ~DMA_ENABLE;
  (void)dma[5];
  // One halfword at each HBlank (repeating, destination reloaded).
  Dma_Set(work->scanlineOffsets, (void *)&REG_BLDY,
          DMA_HBLANK_HALFWORD_PER_LINE, (volatile u32 *)dma);
  if (work->spriteCount > 0)
    Dma_Set(work->obj, OAM, DMA_COPY_WORDS(work->spriteCount * 2), REG_DMA3);
}

#include "../graphics/lucky_wheels/reel_tables.h"

// The title's letters and the left edge of the first: the Spanish title has
// nine letters and starts further left.
#if (LANGUAGE == LANGUAGE_SPANISH)
#define REEL_TITLE_LETTERS 9
#define REEL_TITLE_LEFT 8
#else
#define REEL_TITLE_LETTERS 8
#define REEL_TITLE_LEFT 24
#endif

extern const u8 gLuckyWheels_TitleLetterWidths[];

#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer)

/// The Broken Seal's LuckyWheels_InitializeTitle on this game's heap slots (the
/// reel work at 0xb4, the effect work at 0x5c with its particles first).
void Func_081b336c(void) {
  struct ReelWork *state;
  struct ReelEffectWork *work;
  struct EffectStep *letter;
  s32 left;
  s32 i;
  s32 angle;
  struct Window *window;
  s32 message;

  state = (struct ReelWork *)Ram_HeapSlots->canvasMap;
  work = (struct ReelEffectWork *)Ram_HeapSlots->battleFxWork[0];
  left = 0;
  for (i = 0; i != 0x800; i++)
    SPARKS[i].variant = 0;

  letter = work->particles;
  // The Broken Seal walks a pointer and steps a height; this game's bytes match
  // only with the table read by name and the height worked out from the index.
  for (i = 0; i != REEL_TITLE_LETTERS; i++) {
    letter[i].x = (left + REEL_TITLE_LEFT) << 16;
    left += gLuckyWheels_TitleLetterWidths[i];
    letter[i].y = (-32 - i * 8) << 16;
    letter[i].velocityY = 0;
    letter[i].variant = 0;
  }

  for (i = 0; i != 160; i++)
    state->scanlineOffsets[i] = 0;

  for (i = 0; i != 40; i++) {
    angle = i * 0x199;
    state->scanlineOffsets[23 + i] = (u32)(Trig_Cos(angle) * 3) >> 15;
    state->scanlineOffsets[110 - i] = (u32)(Trig_Cos(angle) * 3) >> 15;
  }

  state->spins = 0;
  state->state = 0;
  state->cursor = 0;
  work->transferMode = 1;
  work->transferValue = 0;
  REG_BLDCNT = 0;
  window = Window_CreateFar(18, 0, 12, 4, 6);
  state->subWindow = window;
  message = (s32)MsgSlotsBet;
  Text_DrawCharacterAtOffsetFar(message, window, 0, 8);
  Text_DrawCharacterAtOffsetFar(message - 1, state->subWindow, 0, 0);
}

// The help window is narrower in Japanese, and one row shorter there when it
// shows only the controls. The Japanese jackpot banner is shorter, so its
// tiles start later in VRAM and end at the same place.
#if LANGUAGE == LANGUAGE_JAPANESE
#define HELP_WINDOW_X 6
#define HELP_WINDOW_WIDTH 18
#define CONTROLS_WINDOW_HEIGHT 3
#define JACKPOT_TILES 0x06017000
#define JACKPOT_COPY DMA_COPY_WORDS(1024)
#else
#define HELP_WINDOW_X 2
#define HELP_WINDOW_WIDTH 26
#define CONTROLS_WINDOW_HEIGHT 4
#define JACKPOT_TILES 0x06016e00
#define JACKPOT_COPY DMA_COPY_WORDS(1152)
#endif

typedef void (*DrawRectangle)(void *canvas, const u8 *source, s32 x, s32 y,
                              s32 width, s32 height);
extern const u16 gLuckyWheels_SparkCellOffsets[];

void Resource_ReservedNoOp(s32 id);
s32 Resource_FarCall00C(s32 slot, s32 a, s32 b, s32 c, s32 d);
void WindowSystem_InitFieldFar(void);

extern char ResourceId_FarCall00C;
extern char ResourceId_ParticleSpritesD;
extern char ResourceId_SlotsMachinePicture;
extern char ResourceId_SlotsIcons;
extern char ResourceId_SlotsJackpotBanner;
extern char ResourceId_JupiterDjinnSheet;
extern char ResourceId_MercuryDjinnSheet;
extern char ResourceId_VenusDjinnSheet;
extern char ResourceId_EmberStreakSheet;
extern char ResourceId_LimePalette;
extern char MsgSlotsChangeBet[];
extern char MsgSlotsControls[];

void LuckyWheels_Run(void) {
  s32 picks[8];
  s32 litLines[7];
  DrawRectangle draw[2];
  u8 *sheet;
  void *canvas;
  struct ReelEffectWork *work;
  struct ReelWork *state;
  struct ReelSave *save;
  u8 *entry;
  u8 *tiles;
  s32 frame;
  u32 count;
  s32 i;
  s32 j;
  s32 k;

  sheet = HeapSlot_Alloc(100, 0x60e);
  canvas = HeapSlot_Alloc(96, 0x8000);
  work = HeapSlot_AllocEwram(92, 0x7b7c);
  state = HeapSlot_AllocEwram(180, sizeof(struct ReelWork));
  tiles = Ram_MapCellBuffer;
  save = (struct ReelSave *)Data_0200024c;
  Resource_ReservedNoOp((s32)&ResourceId_FarCall00C);
  save->wonPrizes[0] = -1;
  state->repeat = 0;
  state->bet = 1;
  Task_ResetAll();
  gRenderOamEnabled = 0;

  count = 0;
  j = 0;
  for (k = 0; k != 20; k++) {
    for (i = 0; i != 32; i++, j += 2) {
      if (i >= 5 && i <= 24 && k > 2 && k <= 13) {
        *(volatile u16 *)(0x06002800 + j) = 0xa1a6;
      } else if (i > 29) {
        *(volatile u16 *)(0x06002800 + j) = 0;
      } else {
        *(volatile u16 *)(0x06002800 + j) = 0xa1a8 + count;
        count++;
      }
    }
  }
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD), sheet);
  entry = Resource_GetTableEntry((s32)&ResourceId_SlotsMachinePicture);
  Dma_Set(entry, (void *)0x05000140, 0x84000008, REG_DMA3);
  entry += 32;
  Resource_DecodeType01(entry, tiles);
  j = 0;
  for (k = 0; k != 20; k++) {
    for (i = 0; i != 30; i++) {
      if (!(i >= 5 && i <= 24 && k > 2 && k <= 13)) {
        Dma_Set(tiles + (k * 30 + i) * 32, (void *)(0x0600b500 + j), 0x84000008,
                REG_DMA3);
        j += 32;
      }
    }
  }
  Iwram_ClearWords((void *)0x06002d00, 0x300);
  j = 0;
  for (k = 0; k != 20; k++) {
    for (i = 0; i != 32; i++, j += 2) {
      if (k < 2 || k > 16)
        *(volatile u16 *)(0x06003000 + j) = 0xbf;
      else
        *(volatile u16 *)(0x06003000 + j) = k * 32 + i + 148;
    }
  }
  REG_BG1CNT = 0x509;
  REG_BG2CNT = 0x680;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  gBgScroll[1].x = 0;
  gBgScroll[1].y = 0;
  gBgScroll[2].x = 0;
  gBgScroll[2].y = 0;
  REG_WININ = 0x3737;
  REG_WINOUT = 0x2727;
  REG_BLDCNT = 0x3f44;
  REG_BLDALPHA = 0x1010;
  REG_BG1HOFS = 0;
  REG_BG2HOFS = 0;
  REG_BG1VOFS = 0xff60;
  REG_BG2VOFS = 0xff60;
  REG_WIN0H = 0x28c8;
  REG_WIN0V = 0x1878;
  REG_WIN0H = 0xf0;
  REG_WIN0V = 0xa0;
  REG_WIN1H = 0xf0;
  REG_WIN1V = 0xa0;

  {
    u16 *bgShade = work->bgShade;
    u16 *shade = work->objShade;
    s32 size = 0x8000;

    state->state = 0;
    state->cursor = 0;
    state->spins = 0;
    work->frame = 0;
    state->timer = 0;
    Dma_Set(Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet),
            (void *)0x05000000, 0x84000020, REG_DMA3);
    *(u16 *)0x05000080 = 0x2f8b;
    *(u16 *)0x05000082 = 0x5bf6;
    entry = Resource_GetTableEntry((s32)&ResourceId_SlotsIcons);
    Dma_Set(entry, (void *)0x05000200, 0x84000078, REG_DMA3);
    entry += 480;
    Resource_DecodeType01(entry, tiles);
    Dma_Set(tiles, (void *)0x06010000, 0x84001b30, REG_DMA3);
    entry = Resource_GetTableEntry((s32)&ResourceId_SlotsJackpotBanner);
    Dma_Set(entry, (void *)0x050003e0, 0x84000008, REG_DMA3);
    entry += 32;
    Resource_DecodeType01(entry, tiles);
    Dma_Set(tiles, (void *)JACKPOT_TILES, JACKPOT_COPY, REG_DMA3);
    WindowSystem_InitFieldFar();
    Func_081b336c();

    for (i = 0; i != 5; i++) {
      struct Reel *reel = &state->reels[i];

      reel->position = 8;
      reel->held = 0;
      reel->stop = -1;
      for (k = 0; k != 21; k++)
        reel->symbols[k] = Random16() % 5;
    }
    for (i = 0; i != 5; i++) {
      struct Reel *reel = &state->reels[i];

      for (j = 0; j != 8; j++) {
        picks[j] = Random16() % 21;
        for (k = 0; k != j; k++) {
          if (picks[j] == picks[k]) {
            j--;
            break;
          }
        }
      }
      for (k = 0; k != 8; k++) {
        s32 rank = k;

        if (rank > 5)
          rank = 5;
        reel->symbols[picks[k]] = rank;
      }
    }

    Resource_FarCall00C(104, 8, 7, 3, 1);
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    Resource_FarCall00C(188, 8, 7, 3, 0);
    draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
    Iwram_FillWordsEntry(canvas, size, 0);
    Dma_Set(canvas, (void *)0x06003500, 0x84002000, REG_DMA3);
    Dma_Set((void *)0x05000000, bgShade, 0x84000080, REG_DMA3);
    Dma_Set((void *)0x05000200, shade, 0x84000080, REG_DMA3);
    Graphics_ScaleRgb555(shade, (u16 *)0x05000200, 0, 0x100);
    Graphics_ScaleRgb555(bgShade, (u16 *)0x05000000, 0, 0x100);
    REG_DISPCNT = 0x3740;
    if (PartyInventory_CountItemFar(228) == 1) {
      struct Window *window = Window_CreateFar(
          HELP_WINDOW_X, 16, HELP_WINDOW_WIDTH, CONTROLS_WINDOW_HEIGHT, 6);

      state->window = window;
      Text_DrawCharacterAtOffsetFar((s32)MsgSlotsControls, window, 0, 0);
    } else {
      struct Window *window =
          Window_CreateFar(HELP_WINDOW_X, 16, HELP_WINDOW_WIDTH, 4, 6);
      s32 message;

      state->window = window;
      message = (s32)MsgSlotsChangeBet;
      Text_DrawCharacterAtOffsetFar(message, window, 0, 0);
      Text_DrawCharacterAtOffsetFar(message + 1, state->window, 0, 8);
    }
    work->transferPending = 0;
    Task_Add(Func_081b243c, 0x480);
    Task_Add(Func_081b3294, 0xc80);
    Task_Add(BattleStaging_ProcessPendingTileTransfer, 0xc80);

    for (frame = 0; state->state != 10; frame++) {
      if (frame <= 16) {
        Graphics_ScaleRgb555(shade, (u16 *)0x05000200, frame << 12, 0x100);
        Graphics_ScaleRgb555(bgShade, (u16 *)0x05000000, frame << 12, 0x100);
      }
      if (state->state == 3) {
        s32 phase = frame % 80;

        if (phase <= 15)
          Palette_StepTowardResource((s32)&ResourceId_MercuryDjinnSheet);
        else if (phase <= 31)
          Palette_StepTowardResource((s32)&ResourceId_VenusDjinnSheet);
        else if (phase <= 47)
          Palette_StepTowardResource((s32)&ResourceId_EmberStreakSheet);
        else if (phase <= 63)
          Palette_StepTowardResource((s32)&ResourceId_LimePalette);
        else
          Palette_StepTowardResource((s32)&ResourceId_JupiterDjinnSheet);
        if (state->timer <= 15)
          Palette_DarkenSceneStep();
        if (state->timer > 16 && (frame & 7) == 0) {
          s32 x = (Random16() & 127) + 56;
          s32 y = (Random16() & 31) + 48;
          struct EffectStep *spark = &SPARKS[(frame / 8 & 3) * 256];

          for (j = 0; j != 256; j++) {
            s32 speed;
            s32 angle;

            speed = 255;
            speed &= Random16();
            angle = Random16() & 0xffff;
            spark->x = x << 16;
            spark->y = y << 16;
            spark->velocityX = Trig_Sin(angle) * (speed + 64) >> 6;
            spark->velocityY = -(Trig_Cos(angle) * (speed + 64)) >> 6;
            spark->variant = (Random16() & 15) + 16;
            spark++;
          }
        }
        for (j = 0; j != 1024; j++) {
          struct EffectStep *spark = &SPARKS[j];

          if (spark->variant > 0) {
            s32 x;

            spark->variant--;
            x = spark->x;
            if (x >= 0 && x <= 0xffffff) {
              s32 y = spark->y;

              if (y <= 0x7fffff) {
                if (y >= 0) {
                  s32 px = x >> 16;
                  s32 py = y >> 16;
                  s32 size = spark->variant / 12 + 1;

                  draw[j & 1](canvas,
                              sheet + gLuckyWheels_SparkCellOffsets[size - 1],
                              px - size, py - size, size * 2, size * 2);
                }
              }
            }
            spark->x += spark->velocityX;
            spark->y += spark->velocityY;
            spark->velocityX = spark->velocityX * 60 / 64;
            spark->velocityY = spark->velocityY * 60 / 64;
          }
        }
      }
      if (state->state == 0 || state->state == 2) {
        for (j = 0; j != 7; j++)
          litLines[j] = 0;
        if (state->state == 0) {
          litLines[3] = 1;
          if (state->bet > 1) {
            litLines[4] = 1;
            litLines[2] = 1;
          }
          if (state->bet > 2) {
            litLines[5] = 1;
            litLines[1] = 1;
          }
          if (state->bet > 3) {
            litLines[6] = 1;
            litLines[0] = 1;
          }
        } else if ((frame & 7) <= 3) {
          for (j = 0; j != 7; j++)
            litLines[j] = state->winningLines[j];
        }
        for (j = 0; j != 3; j++) {
          s32 color = j != 1;

          color = 65 - color;
          if (litLines[1])
            LuckyWheels_DrawCanvasLine(20, j + 19, 200, j + 19, color);
          if (litLines[2])
            LuckyWheels_DrawCanvasLine(28, j + 35, 200, j + 35, color);
          if (litLines[3])
            LuckyWheels_DrawCanvasLine(20, j + 51, 200, j + 51, color);
          if (litLines[4])
            LuckyWheels_DrawCanvasLine(28, j + 67, 200, j + 67, color);
          if (litLines[5])
            LuckyWheels_DrawCanvasLine(20, j + 83, 200, j + 83, color);
          if (litLines[0])
            LuckyWheels_DrawCanvasLine(28, j + 5, 200, j + 91, color);
          if (litLines[6])
            LuckyWheels_DrawCanvasLine(28, j + 97, 200, j + 11, color);
        }
      }
      work->transferPending = 1;
      wait(1);
    }

    for (j = 0; j != 17; j++) {
      Graphics_ScaleRgb555(shade, (u16 *)0x05000200, 0x10000 - (j << 12),
                           0x100);
      Graphics_ScaleRgb555(bgShade, (u16 *)0x05000000, 0x10000 - (j << 12),
                           0x100);
      wait(1);
    }
    HeapSlot_Release(188);
    HeapSlot_Release(104);
    Task_Remove(BattleStaging_ProcessPendingTileTransfer);
    Task_Remove(Func_081b243c);
    Task_Remove(Func_081b3294);
    HeapSlot_Release(180);
    HeapSlot_Release(96);
    HeapSlot_Release(92);
    HeapSlot_Release(100);
  }
}

void LzPack_ResetSlotBuckets(void) {
  s32 index;
  struct LzPackNode **bucket;

  for (index = 0; index < 1024; index++) {
    gFlash_Handler3->nodes[index].slot = index;
    gFlash_Handler3->nodes[index].prev = NULL;
  }
  bucket = gFlash_Handler3->bucket;
  for (index = 255; index >= 0; index--) {
    *bucket++ = NULL;
  }
}

void LzPack_InsertSlotNode(s32 index) {
  struct LzPackWork *work = gFlash_Handler3;

  work->nodes[index].prev =
      (struct LzPackNode *)&work->bucket[work->bucketBySlot[index]];
  work->nodes[index].next = work->bucket[work->bucketBySlot[index]];
  work->bucket[work->bucketBySlot[index]] = &work->nodes[index];
  if (work->nodes[index].next != NULL)
    work->nodes[index].next->prev = &work->nodes[index];
}

void LzPack_RemoveSlotNode(s32 slot) {
  if (gFlash_Handler3->nodes[slot].prev != NULL) {
    if (gFlash_Handler3->nodes[slot].next != NULL)
      gFlash_Handler3->nodes[slot].next->prev =
          gFlash_Handler3->nodes[slot].prev;
    gFlash_Handler3->nodes[slot].prev->next = gFlash_Handler3->nodes[slot].next;
  }
}

void LzPack_ConsumeSlotBytes(s32 start, s32 count, const u8 *input) {
  s32 limit = count;
  s32 current = 0;
  u32 base = start;

  if (current < limit) {
    u32 mask = 0x3ff;
    u32 removal = base + 0x124;

    do {
      u32 slot = base + current;
      struct LzPackWork *state;
      u32 readOffset;
      u32 nextOffset;
      u8 value;

      LzPack_RemoveSlotNode(removal & mask);
      state = gFlash_Handler3;
      readOffset = state->inputCursor;
      value = input[readOffset];
      nextOffset = readOffset + 1;
      state->inputCursor = nextOffset;
      if (nextOffset == state->inputLimit) {
        state->bucketBySlot[slot & mask] = -1;
        break;
      }

      state->bucketBySlot[slot & mask] = value;
      current++;
      LzPack_InsertSlotNode(slot & mask);
      removal++;
    } while (current < limit);
  }

  current++;
  if (current < limit) {
    u32 mask = 0x3ff;
    struct LzPackWork **root = &gFlash_Handler3;
    s32 empty = -1;

    do {
      u32 slot = (base + current) & mask;
      struct LzPackWork *state;

      LzPack_RemoveSlotNode(slot);
      state = *root;
      current++;
      state->bucketBySlot[slot] = empty;
    } while (current < limit);
  }
}

void LzPack_CopyBufferedBytes(u8 *destination) {
  struct LzPackWork *work = gFlash_Handler3;
  s32 index;

  for (index = 0; index != work->outCnt; index++) {
    destination[work->outTotal] = work->outBuf[index];
    work->outTotal = work->outTotal + 1;
  }
}

#define PACK_WINDOW_MASK 0x3ff
#define PACK_MATCH_MAX 272
#define PACK_DISTANCE_MAX 63
#define PACK_LOOKAHEAD 672

static __inline__ void LzPack_FindWindowMatch(s32 start) {
  struct LzPackNode *node;
  s32 len;

  gFlash_Handler3->matchLen = 1;
  if (gFlash_Handler3->bucketBySlot[start] == -1) {
    return;
  }
  node = gFlash_Handler3->bucket[gFlash_Handler3->bucketBySlot[start]];
  if (start + PACK_MATCH_MAX > PACK_WINDOW_MASK) {
    while (node != NULL) {
      s32 dist = (start - node->slot) & PACK_WINDOW_MASK;

      if (dist > 0 && dist <= PACK_DISTANCE_MAX) {
        for (len = 1; len < PACK_MATCH_MAX; len++) {
          if (gFlash_Handler3->bucketBySlot[(start + len) & PACK_WINDOW_MASK] !=
              gFlash_Handler3
                  ->bucketBySlot[(node->slot + len) & PACK_WINDOW_MASK]) {
            break;
          }
        }
        if (gFlash_Handler3->matchLen < len) {
          gFlash_Handler3->matchOffset = dist;
          gFlash_Handler3->matchLen = len;
          if (len == PACK_MATCH_MAX) {
            return;
          }
        }
      }
      node = node->next;
    }
  } else {
    while (node != NULL) {
      s32 dist = (start - node->slot) & PACK_WINDOW_MASK;

      if (dist > 0 && dist <= PACK_DISTANCE_MAX) {
        for (len = 1; len < PACK_MATCH_MAX; len++) {
          if (gFlash_Handler3->bucketBySlot[start + len] !=
              gFlash_Handler3
                  ->bucketBySlot[(node->slot + len) & PACK_WINDOW_MASK]) {
            break;
          }
        }
        if (gFlash_Handler3->matchLen < len) {
          gFlash_Handler3->matchOffset = dist;
          gFlash_Handler3->matchLen = len;
          if (len == PACK_MATCH_MAX) {
            return;
          }
        }
      }
      node = node->next;
    }
  }
}

/// Unused: packs size bytes of input into dst with a small LZ scheme and
/// returns the packed size. Each group starts with a flag byte, a set bit
/// marking a match: two bytes of distance and length (lengths 2 to 16), or
/// three for lengths up to 272; a match is put off by one byte when the next
/// position matches further. A zero match ends the stream. Nothing in this game
/// calls it.
UNUSED s32 LzPack_PackStream(const u8 *input, u8 *dst, s32 size) {
  s32 defer;
  s32 len;
  s32 saveOffset;
  s32 saveLen;
  s32 reach[2];
  s16 code;

  defer = 0;
  gFlash_Handler3 = (struct LzPackWork *)Ram_MapCellBuffer;
  LzPack_ResetSlotBuckets();
  gFlash_Handler3->inputLimit = size;
  gFlash_Handler3->pos = 0;
  gFlash_Handler3->inputCursor = 0;
  gFlash_Handler3->outTotal = 0;
  gFlash_Handler3->flagMask = 0x80;
  gFlash_Handler3->outBuf[0] = 0;
  gFlash_Handler3->outCnt = 1;
  LzPack_ConsumeSlotBytes(0, PACK_LOOKAHEAD, input);

  while (gFlash_Handler3->bucketBySlot[gFlash_Handler3->pos] != -1) {
    LzPack_FindWindowMatch(gFlash_Handler3->pos);
    if (defer == 0) {
      len = gFlash_Handler3->matchLen;
      if (gFlash_Handler3->matchLen > 1) {
        saveOffset = gFlash_Handler3->matchOffset;
        saveLen = len;
        LzPack_FindWindowMatch((gFlash_Handler3->pos + 1) & PACK_WINDOW_MASK);
        if (gFlash_Handler3->matchLen > 2) {
          reach[0] = gFlash_Handler3->matchLen + 1;
          LzPack_FindWindowMatch((gFlash_Handler3->pos + len) &
                                 PACK_WINDOW_MASK);
          reach[1] = gFlash_Handler3->matchLen + len;
          if (reach[0] >= reach[1]) {
            saveLen = 1;
            defer = 1;
          }
        }
        gFlash_Handler3->matchOffset = saveOffset;
        gFlash_Handler3->matchLen = saveLen;
      }
    }

    if (gFlash_Handler3->matchLen > 1) {
      defer = 0;
      gFlash_Handler3->outBuf[0] |= gFlash_Handler3->flagMask;
      if (gFlash_Handler3->matchLen <= 16) {
        code = ((gFlash_Handler3->matchOffset << 4) & ~0xfff) |
               (gFlash_Handler3->matchOffset & 0xff) |
               (((gFlash_Handler3->matchLen - 1) << 8) & 0xf00);
        gFlash_Handler3->outBuf[gFlash_Handler3->outCnt++] = (u16)code >> 8;
        gFlash_Handler3->outBuf[gFlash_Handler3->outCnt++] = code;
      } else {
        code = ((gFlash_Handler3->matchOffset << 4) & ~0xfff) |
               (gFlash_Handler3->matchOffset & 0xff);
        gFlash_Handler3->outBuf[gFlash_Handler3->outCnt++] = (u16)code >> 8;
        gFlash_Handler3->outBuf[gFlash_Handler3->outCnt++] = code;
        gFlash_Handler3->outBuf[gFlash_Handler3->outCnt++] =
            gFlash_Handler3->matchLen - 17;
      }
    } else {
      gFlash_Handler3->outBuf[gFlash_Handler3->outCnt++] =
          gFlash_Handler3->bucketBySlot[gFlash_Handler3->pos];
      gFlash_Handler3->matchLen = 1;
    }

    LzPack_ConsumeSlotBytes(gFlash_Handler3->pos + PACK_LOOKAHEAD,
                            gFlash_Handler3->matchLen, input);
    gFlash_Handler3->pos =
        (gFlash_Handler3->pos + gFlash_Handler3->matchLen) & PACK_WINDOW_MASK;
    gFlash_Handler3->flagMask >>= 1;
    if (gFlash_Handler3->flagMask == 0) {
      LzPack_CopyBufferedBytes(dst);
      gFlash_Handler3->flagMask = 0x80;
      gFlash_Handler3->outBuf[0] = 0;
      gFlash_Handler3->outCnt = 1;
    }
  }

  gFlash_Handler3->outBuf[0] |= gFlash_Handler3->flagMask;
  gFlash_Handler3->outBuf[gFlash_Handler3->outCnt++] = 0;
  gFlash_Handler3->outBuf[gFlash_Handler3->outCnt++] = 0;
  LzPack_CopyBufferedBytes(dst);
  return gFlash_Handler3->outTotal;
}
