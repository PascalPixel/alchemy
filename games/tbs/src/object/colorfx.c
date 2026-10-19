#include "audio_engine_symbols.h"
#include "unit/object/colorfx.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "battle_effect_work.h"
#include "heap_state.h"
#include "game_state.h"
#include "types.h"
#include "macros.h"
#include "dma.h"
#include "iwram_call.h"
#include "resource.h"
#include "io_reg.h"
#include "songs.h"
#include "mem.h"
#include "fixed_math.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "callback_scheduler.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "resource_ids.h"
#include "tbs_edition.h"
#include "text/msg_ids.h"
#include "lzpack.h"
#include "frame.h"
#include "oambuf.h"
#include "item_ids.h"

//--------------------------------------------------------------------------
// The slot machine (the reel game): its palette steps, canvas transfers and
// line drawing, and an LZ packer.
//--------------------------------------------------------------------------

/// The sparks of the jackpot, kept in the map cell buffer.
#define REEL_SPARKS ((struct EffectStep *)Ram_MapCellBuffer)

/// The slot machine's own work block (struct ReelWork), beside the battle
/// effect blocks it borrows.
#define REEL_HEAP_SLOT 45

/// The paylines' canvas: 256 x 128 pixels of 8-bit colour, shown on BG2 from
/// its tile 212, where the third row of its map begins.
#define REEL_CANVAS_SIZE (256 * 128)
#define REEL_CANVAS_TILES (BG_CHAR_BLOCK(0) + 0x3500)

/// Plays the slot machine: blanks the screen, seeds the random numbers with the
/// play time and runs the game. Returns 0.
s32 ReelGame_Start(void) {
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
  gRandomSeed = gGameState.playTime;
  AudioCommand_PlayFar(MUS_009);
  ReelGame_Run();
  return 0;
}

/// Writes count RGB555 colours from src to dst scaled by scale (16.16, 0x10000
/// keeps them). Returns 0.
s32 Graphics_ScaleRgb555(u16 *src, u16 *dst, s32 scale, s32 count) {
  s32 n;
  u32 redMask;
  u32 greenMask;
  u32 blueMask;
  u32 col;
  u32 r;
  u32 g;
  u32 b;

  if (count > 0) {
    redMask = 0x1f;
    greenMask = 0x3e0;
    blueMask = 0x7c00;
    n = count;
    do {
      // Scale each channel where it stands and keep the top 16 bits.
      col = *src;
      r = col & redMask;
      g = col & greenMask;
      b = blueMask & col;
      r *= scale;
      g *= scale;
      b *= scale;
      col = ((r >> 16) & redMask) | ((g >> 16) & greenMask);
      col |= (b >> 16) & blueMask;
      *dst = col;
      src++;
      dst++;
      n--;
    } while (n != 0);
  }
  return 0;
}

/// A scheduler callback: on a frame whose canvas is ready (transferPending),
/// sends the canvas to its tiles in VRAM (REEL_CANVAS_TILES), then clears it
/// or fades it into trails as transferMode says.
void ReelGame_TransferCanvas(void) {
  struct BattleEffectWork *work;
  u8 *buf;

  work = gBattleFxWork[0];
  if (work->transferPending == 1) {
    buf = gBattleFxWork[1];
    switch (work->transferMode) {
    case 1:
      // Then fill the canvas with transferValue.
      Dma_Set(buf, (void *)REEL_CANVAS_TILES,
              (DMA_ENABLE | DMA_32BIT) << 16 | REEL_CANVAS_SIZE / 4, REG_DMA3);
      Iwram_FillWords(buf, REEL_CANVAS_SIZE, work->transferValue);
      break;
    case 2:
      // Trails: fade nonzero colour numbers to 1/2 or 3/4.
      if (work->transferValue == 50)
        ColorBuffer_BackupAndHalveNonzero(buf, REEL_CANVAS_TILES,
                                          REEL_CANVAS_SIZE);
      else
        ColorBuffer_BackupAndScaleNonzeroThreeQuarters(buf, REEL_CANVAS_TILES,
                                                       REEL_CANVAS_SIZE);
      break;
    }
    work->transferPending = 0;
    work->framesSinceTransfer = 1;
  } else {
    (work->framesSinceTransfer)++;
  }
}

/// Darkens the screen one step: lowers each channel of the 16 colours of BG
/// palette 10 and of OBJ colours 1 to 239 by 1.
void Palette_DarkenSceneStep(void) {
  u16 *col;
  s32 i;

  col = (u16 *)&BG_PLTT_COLOR(10, 0);
  for (i = 0; i != 16; i++) {
    s32 b = (*col >> 10) & 0x1f;
    s32 g = (*col >> 5) & 0x1f;
    s32 r = *col & 0x1f;

    b--;
    g--;
    r--;

    if (b < 0)
      b = 0;
    if (g < 0)
      g = 0;
    if (r < 0)
      r = 0;
    *col++ = RGB(r, g, b);
  }
  col = (u16 *)&OBJ_PLTT[1];
  for (i = 0; i != 239; i++) {
    s32 b = (*col >> 10) & 0x1f;
    s32 g = (*col >> 5) & 0x1f;
    s32 r = *col & 0x1f;

    b--;
    g--;
    r--;

    if (b < 0)
      b = 0;
    if (g < 0)
      g = 0;
    if (r < 0)
      r = 0;
    *col++ = RGB(r, g, b);
  }
}

/// Moves the first 64 BG colours one step toward the palette of resource; the
/// backdrop colour is not written back.
void Palette_StepTowardResource(s32 resource) {
  u16 dst[64];
  u16 *col = (u16 *)BG_PLTT;
  s32 i;

  Dma_Set(Resource_GetTableEntry(resource), dst,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x20, REG_DMA3);
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
  Dma_Set(dst + 1, (void *)&BG_PLTT[1], (DMA_ENABLE << 16) | 63, REG_DMA3);
}

/// Draws a line from (x0, y0) to (x1, y1) on the 256 x 128 8bpp canvas (32
/// tiles across), writing only over lower colour numbers.
void ReelGame_DrawCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color) {
  s32 dx = x1 - x0;
  s32 dy = y1 - y0;
  s32 frac = 0x80; // 0.5 in 8-bit fixed point
  u8 *canvas = ((u8 **)gBattleFxWork)[1];
  s32 step;
  s32 i;
  s32 x;
  s32 y;
  s32 tmp;
  u32 offset;

  if (ABS(dx) < ABS(dy)) {
    // Steep: step y one pixel at a time and follow x in fixed point.
    if (dy < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (ABS(x1 - x0) << 8) / ABS(y1 - y0);
    x = x0;
    for (i = y0; i != y1; i++) {
      offset = (((((u32)i >> 3) << 5) + ((u32)x >> 3)) << 3) + (i & 7);
      offset = (offset << 3) + (x & 7);
      if (canvas[offset] < color)
        canvas[offset] = color;
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
    // Shallow: step x one pixel at a time and follow y in fixed point.
    if (dx < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (ABS(y1 - y0) << 8) / ABS(x1 - x0);
    y = y0;
    for (i = x0; i != x1; i++) {
      offset = (((((u32)y >> 3) << 5) + ((u32)i >> 3)) << 3) + (y & 7);
      offset = (offset << 3) + (i & 7);
      if (canvas[offset] < color)
        canvas[offset] = color;
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

/// One of the slot machine's five reels: how far it has turned, its 21
/// symbols, whether it is held, and the countdown to its stop (-1 until one
/// starts).
struct Reel {
  s32 position; ///< pixels, 16 a symbol
  u8 symbols[21];
  u8 held;
  s8 stop; ///< frames
};

struct UiWindow;

/// The slot machine's states (ReelWork.state).
enum ReelGameState {
  REEL_BETTING = 0,        ///< Up and Down change the bet, A spins, B leaves
  REEL_SPINNING = 1,       ///< the reels spin until R or time stops each
  REEL_WON = 2,            ///< a payline won: a pause
  REEL_SHOWING_WIN = 3,    ///< the win is shown until A
  REEL_CHOOSING_HOLDS = 5, ///< choose reels to hold, or pull the lever
  REEL_DONE = 10,          ///< the game ends
  REEL_NO_WIN = 11,        ///< no payline won: A to choose holds
  REEL_OUT_OF_SPINS = 20,  ///< the fourth spin lost: a pause, then the end
};

/// The slot machine's work in heap slot 45: its reels, state, bet and spins,
/// keys, timer, winning lines, sprites and windows.
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
  u8 unk0a4[4];
  s32 timer;
  s32 winningLines[7];
  struct OamShadow obj[128];
  struct UiWindow *window;
  struct UiWindow *subWindow;
  u8 unk4d0[8];
  u16 scanlineOffsets[160];
#if TBS_PATCH_1
  s32 phase;
#endif
};

/// gGameState seen from +12, the base the bytes load: wonPrizes is
/// gGameState.wonPrizes (through the field itself ReelGame_RunFrame differs 12)
struct ReelSave {
  u8 unk000[0x120];
  s8 wonPrizes[16];
};

#include "../graphics/slots/reel_tables.h"

/// The slot machine's frame callback: reads the keys, runs the game's state
/// (betting, spinning and stopping the reels, scoring the paylines, choosing
/// reels to hold, showing a win) and builds all 128 sprites: the cursor, the
/// title letters of a win, the frame, the hold lamps, the spin counter, the
/// paylines, the reel symbols, the lever and the bottom bar.
void ReelGame_RunFrame(void) {
  /* FAKEMATCH(volatile): struct OamShadow's volatile words (oambuf.h) keep
   * each list loop's offset givs; as plain u32 words, 675 of 1729
   * instructions differ in tbs-en. */
  struct ReelWork *work;
  struct BattleEffectWork *fx;
  volatile u16 *dma;
  s32 sprite;
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

  work = ((struct ReelWork **)
              gBattleFxWork)[REEL_HEAP_SLOT - HEAP_SLOT_BATTLE_EFFECT];
  fx = gBattleFxWork[0];
  sprite = 0;
  objMode = 0x400;

  Random16();

  dma = REG_DMA0;
  dma[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
  dma[5] &= ~DMA_ENABLE;
  (void)dma[5];
  // One halfword at each HBlank (repeating, destination reloaded).
  Dma_Set(work->scanlineOffsets, (void *)&REG_BLDY,
          DMA_HBLANK_HALFWORD_PER_LINE, (volatile u32 *)dma);

  keys = gKeysHeld;
  dpad = gKeysHeld & KEYS_DPAD;
  work->pressed = keys & ~work->keys;
  work->direction = dpad;
  if ((work->keys & KEYS_DPAD) == work->direction) {
    if (work->repeat > 12)
      work->repeat = 12;
    if (work->repeat == 0) {
      work->repeat = 4;
    } else {
      work->repeat--;
      work->direction = sprite; // 0: no sprite is built yet
    }
  } else {
    work->repeat = 12;
  }
  work->keys = keys;

  if (gDebugPaused == 0) {
    if (work->state == REEL_BETTING) {
      tickets = PartyInventory_CountItemFar(ITEM_GAME_TICKET);
      UiText_DrawNumberInWindowFar(tickets - work->bet, REEL_COIN_DIGITS,
                                   work->subWindow, REEL_COIN_X, 0);
      UiText_DrawNumberInWindowFar(work->bet, 2, work->subWindow, 64, 8);
      if (work->pressed & KEY_B) {
        struct ReelSave *save = (struct ReelSave *)((u8 *)&gGameState + 12);

        work->state = REEL_DONE;
        save->wonPrizes[0] = -2;
        UiWork_FinalizeFar(work->window, 1);
      } else {
        if (work->pressed & KEY_UP) {
          if (work->bet <= 3 && tickets > work->bet) {
            work->bet++;
            AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
          } else {
            AudioCommand_PlayFar(SE_MENU_CANCEL);
          }
        }
        if (work->pressed & KEY_DOWN) {
          if (work->bet > 1) {
            work->bet--;
            AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
          } else {
            AudioCommand_PlayFar(SE_MENU_CANCEL);
          }
        }
        REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_DARKEN | BLDCNT_TGT2_ALL;
        REG_BLDALPHA = BLEND_ALPHA(16, 0);
        if (work->pressed & KEY_A) {
          work->state = REEL_SPINNING;
          fx->frame = 0;
          UiWork_FinalizeFar(work->window, 1);
          for (k = 0; k != work->bet; k++)
            PartyInventory_RemoveFar(ITEM_GAME_TICKET);
          UiWork_FinalizeFar(work->subWindow, 1);
          AudioCommand_PlayFar(SE_304);
        }
      }
    } else if (work->state == REEL_CHOOSING_HOLDS) {
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
          work->state = REEL_BETTING;
          for (i = 0; i != 5; i++) {
            work->reels[i].held = 0;
            work->reels[i].stop = -1;
          }
        } else if (work->cursor <= 4) {
          AudioCommand_PlayFar(SE_305);
          work->reels[work->cursor].held ^= 1;
        } else if (allHeld == 0) {
          AudioCommand_PlayFar(SE_304);
          work->state = REEL_SPINNING;
          work->cursor = 0;
          for (i = 0; i != 5; i++)
            work->reels[i].stop = -1;
          work->spins++;
        } else {
          AudioCommand_PlayFar(SE_MENU_CANCEL);
        }
      } else {
        if (work->direction & KEY_RIGHT) {
          work->cursor = (work->cursor + 1) % 6;
          AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        }
        if (work->direction & KEY_LEFT) {
          work->cursor = (work->cursor + 5) % 6;
          AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        }
      }

      if (work->state == REEL_CHOOSING_HOLDS) {
#if TBS_PATCH_1
        if (work->cursor == 5) {
          if (allHeld != 0) {
            if (work->phase != 1 && work->phase != 2) {
              UiWork_FinalizeFar(work->window, 1);
              work->window = UiWindow_CreateFar(REEL_ALL_HELD_X, 0,
                                                REEL_ALL_HELD_WIDTH, 4, 6);
              UiText_DrawCharacterAtOffsetFar(MsgSlotsAllHeld, work->window, 0,
                                              0);
              work->phase = 1;
            } else if (work->phase == 1) {
              UiText_DrawCharacterAtOffsetFar(MsgSlotsCannotPull, work->window,
                                              0, 8);
              work->phase = 2;
            }
          } else {
            if (work->phase != 3) {
              UiWork_FinalizeFar(work->window, 1);
              work->window =
                  UiWindow_CreateFar(REEL_START_X, 0, REEL_START_WIDTH, 3, 6);
              UiText_DrawCharacterAtOffsetFar(MsgSlotsStart, work->window, 0,
                                              0);
            }
            work->phase = 3;
          }
        } else if (work->reels[work->cursor].held == 0) {
          if (work->phase != 4) {
            UiWork_FinalizeFar(work->window, 1);
            work->window =
                UiWindow_CreateFar(REEL_HOLD_X, 0, REEL_HOLD_WIDTH, 3, 6);
            UiText_DrawCharacterAtOffsetFar(MsgSlotsHold, work->window, 0, 0);
          }
          work->phase = 4;
        } else {
          if (work->phase != 5) {
            UiWork_FinalizeFar(work->window, 1);
            work->window =
                UiWindow_CreateFar(REEL_RELEASE_X, 0, REEL_RELEASE_WIDTH, 3, 6);
            UiText_DrawCharacterAtOffsetFar(MsgSlotsRelease, work->window, 0,
                                            0);
          }
          work->phase = 5;
        }
#else
        UiWork_FinalizeFar(work->window, 1);
        if (work->cursor == 5) {
          if (allHeld != 0) {
            work->window = UiWindow_CreateFar(12, 0, 18, 3, 6);
            UiText_DrawCharacterAtOffsetFar(MsgSlotsAllHeld, work->window, 0,
                                            0);
          } else {
            work->window = UiWindow_CreateFar(18, 0, 12, 3, 6);
            UiText_DrawCharacterAtOffsetFar(MsgSlotsStart, work->window, 0, 0);
          }
        } else {
          work->window = UiWindow_CreateFar(18, 0, 12, 3, 6);
          if (work->reels[work->cursor].held == 0)
            UiText_DrawCharacterAtOffsetFar(MsgSlotsHold, work->window, 0, 0);
          else
            UiText_DrawCharacterAtOffsetFar(MsgSlotsRelease, work->window, 0,
                                            0);
        }
#endif
      } else {
        UiWork_FinalizeFar(work->window, 1);
      }
    } else if (work->state == REEL_WON) {
      work->timer++;
      objMode = 0;
      if (work->timer == 60) {
        work->state = REEL_SHOWING_WIN;
        AudioCommand_PlayFar(MUS_093);
        work->timer = 0;
        REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        REG_BLDALPHA = BLEND_ALPHA(16, 16);
        fx->transferMode = 2;
        fx->transferValue = 75;
      }
    } else if (work->state == REEL_SHOWING_WIN) {
      work->timer++;
      objMode = 0;
      if (work->pressed & KEY_A) {
        work->state = REEL_DONE;
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
      }
    } else if (work->state == REEL_NO_WIN) {
#if TBS_PATCH_1
      if (work->phase == 0) {
        work->phase = 1;
        UiText_DrawCharacterAtOffsetFar(MsgSlotsPullLever, work->window, 0, 8);
      }
#endif
      if (work->pressed & KEY_A) {
        work->state = REEL_CHOOSING_HOLDS;
#if TBS_PATCH_1
        work->phase = 0;
#endif
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        UiWork_FinalizeFar(work->window, 1);
      }
    } else if (work->state == REEL_OUT_OF_SPINS) {
      work->timer++;
      if (work->timer == 45)
        work->state = REEL_DONE;
    } else if (work->state != REEL_DONE) {
      if (work->timer == 4) {
        work->window =
            UiWindow_CreateFar(REEL_STOP_X, 17, REEL_STOP_WIDTH, 3, 6);
        UiText_DrawCharacterAtOffsetFar(MsgSlotsStopReels, work->window, 0, 0);
      }
      if (work->timer == 16)
        AudioCommand_PlayFar(SE_306);
      if (work->timer > 56) {
        if (fx->frame > 31 || (work->pressed & KEY_R)) {
          fx->frame = 0;
          for (i = 0; i != 5; i++) {
            if (work->reels[i].held == 0 && work->reels[i].stop == -1) {
              work->reels[i].stop = (Random16() & 3) + 4;
              AudioCommand_PlayFar(SE_307);
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
        struct ReelSave *save = (struct ReelSave *)((u8 *)&gGameState + 12);
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
              save->wonPrizes[hits] = gReelGame_SymbolPrizes[found];
              hits++;
            }
          }
        }
        work->timer = 0;
        if (hits != 0) {
          save->wonPrizes[hits] = -1;
          work->state = REEL_WON;
          AudioCommand_PlayFar(SE_171);
          fx->transferMode = 1;
          fx->transferValue = 0;
          REG_BLDCNT = 0;
          UiWork_FinalizeFar(work->window, 1);
        } else {
          work->state = REEL_NO_WIN;
#if TBS_PATCH_1
          work->phase = 0;
          UiWork_FinalizeFar(work->window, 1);
#if REEL_HINT_BEFORE_LAST_SPIN
          if (work->spins <= 3)
#endif
          {
            work->window = UiWindow_CreateFar(3, 16, 24, 4, 6);
            UiText_DrawCharacterAtOffsetFar(MsgSlotsChooseHeld, work->window, 0,
                                            0);
          }
#else
          UiWork_FinalizeFar(work->window, 1);
          {
            s32 message;

            work->window = UiWindow_CreateFar(7, 16, 16, 4, 6);
            message = MsgSlotsChooseHeld;
            UiText_DrawCharacterAtOffsetFar(message, work->window, 0, 0);
            UiText_DrawCharacterAtOffsetFar(message + 1, work->window, 0, 8);
          }
#endif
          if (work->spins == 4) {
            s32 left = PartyInventory_CountItemFar(ITEM_GAME_TICKET);
            struct UiWindow *window;
            s32 message;

            if (left > 0)
              work->state = REEL_OUT_OF_SPINS;
            else
              work->state = REEL_OUT_OF_SPINS;
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
            window = UiWindow_CreateFar(18, 0, 12, 4, 6);
            work->subWindow = window;
            message = MsgSlotsBet;
            UiText_DrawCharacterAtOffsetFar(message, window, 0, 8);
            UiText_DrawCharacterAtOffsetFar(message - 1, work->subWindow, 0, 0);
          }
        }
      }

      if (work->state == REEL_SPINNING) {
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

  if (work->state == REEL_CHOOSING_HOLDS) {
    x = work->cursor * 36 + 36;
    y = 128;
    v = 0;
    if ((work->timer & 15) <= 7)
      v = 1;
    if (work->cursor == 5) {
      x = 208;
      y = 32;
    }
    work->obj[sprite].attr01 =
        ((x - 12) << 16) | objMode | (y + 8) | 0x80006000;
    work->obj[sprite].attr2 = (v << 4) + 0x2b0;
    sprite++;
    work->obj[sprite].attr01 =
        ((x + 12) << 16) | objMode | (y + 8) | 0x90006000;
    work->obj[sprite].attr2 = (v << 4) + 0x2b0;
    sprite++;
    work->obj[sprite].attr01 = (x << 16) | objMode | y | 0x80002000;
    work->obj[sprite].attr2 = 0x1f0;
    sprite++;
  }

  if (work->state == REEL_SHOWING_WIN) {
    for (i = 0; i != REEL_TITLE_LETTERS; i++) {
      struct EffectStep *p = &fx->particles[i];

#if (LANGUAGE == LANGUAGE_SPANISH)
      if (i == 0)
        work->obj[sprite].attr01 =
            (((*(s16 *)((u8 *)&p->x + 2) + 0x1f0) & 0x1ff) << 16) | objMode |
            ((*(s16 *)((u8 *)&p->y + 2) + 0x108) & 255) | 0xb0000000;
      else
#endif
        work->obj[sprite].attr01 = (*(s16 *)((u8 *)&p->x + 2) << 16) | objMode |
                                   ((*(s16 *)((u8 *)&p->y + 2) + 256) & 255) |
                                   0x80000000;
      work->obj[sprite].attr2 =
          ((gReelGame_TitleLetterTiles[i] << 4) + REEL_TITLE_TILES) | 0xf000;
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
      sprite++;
    }
  }

  for (k = 0; k != 14; k++) {
    work->obj[sprite].attr01 = (gReelGame_FrameSpriteX[k] << 16) | objMode |
                               gReelGame_FrameSpriteY[k] | 0x80006000;
    if (k <= 3)
      work->obj[sprite].attr2 = 0x4e0;
    else
      work->obj[sprite].attr2 = 0x4e8;
    sprite++;
  }

  for (i = 0; i != 5; i++) {
    if (work->reels[i].held == 0) {
      work->obj[sprite].attr01 = objMode | ((i * 36 + 32) << 16) | 0x8000207c;
      work->obj[sprite].attr2 = 0x460;
    } else {
      work->obj[sprite].attr01 = objMode | ((i * 36 + 32) << 16) | 0x8000207c;
      work->obj[sprite].attr2 = 0x480;
    }
    sprite++;
  }

  for (i = 0; i != 5; i++) {
    work->obj[sprite].attr01 = objMode | ((i * 16 + 32) << 16) | 0x80006003;
    if (i == work->spins)
      work->obj[sprite].attr2 = (i * 32 + 0x210) | 0x400;
    else
      work->obj[sprite].attr2 = (i * 32 + 0x220) | 0x400;
    sprite++;
  }

  for (col = 0; col != 7; col++) {
    work->obj[sprite].attr01 = (((0x204 - ((col & 1) << 3)) & 0x1ff) << 16) |
                               objMode | (col * 16 + 5) | 0x80002000;
    if (col > 3 - work->bet && col < work->bet + 3)
      work->obj[sprite].attr2 = 0x5d0;
    else
      work->obj[sprite].attr2 = 0x510;
    sprite++;
  }

  for (i = 0; i != 5; i++) {
    struct Reel *reel = &work->reels[i];

    for (col = 0; col != 7; col++) {
      work->obj[sprite].attr01 = (col * 16 + reel->position % 16 + 4) |
                                 ((i * 32 + 40) << 16) | 0x80006000;
      work->obj[sprite].attr2 =
          (reel->symbols[(col - reel->position / 16 + 21) % 21] << 4) | 0x800;
      sprite++;
    }
  }

  lever = 40;
  if (work->state == REEL_SPINNING) {
    if (work->timer <= 47)
      lever = ((Trig_Sin(work->timer * 682) << 6) >> 16) + 40;
    else if (work->timer <= 55)
      lever = ((Trig_Sin((work->timer << 12) - 0x30000) << 2) >> 16) + 40;
  }
  work->obj[sprite].attr01 = lever | objMode | 0x80d06000;
  work->obj[sprite].attr2 = 0x500;
  sprite++;

  for (k = 0; k != 8; k++) {
    work->obj[sprite].attr01 = objMode | (k * 16 + 12) | 0x80ce6000;
    if (k == 0)
      work->obj[sprite].attr2 = 0x540;
    else if (k == 1)
      work->obj[sprite].attr2 = 0x550;
    else if (k == 6)
      work->obj[sprite].attr2 = 0x570;
    else if (k == 7)
      work->obj[sprite].attr2 = 0x580;
    else
      work->obj[sprite].attr2 = 0x560;
    sprite++;
  }

  while (sprite != 128) {
    work->obj[sprite].attr01 = 0x40f02000;
    work->obj[sprite].attr2 = 0;
    sprite++;
  }

  Dma_Set(work->obj, OAM, (sprite * 2) | (DMA_ENABLE | DMA_32BIT) << 16,
          REG_DMA3);
}

/// Sets up the bouncing title letters of a win, the brightness wave of the
/// screen's lines and the betting window, and starts at betting.
void ReelGame_InitTitle(void) {
  struct ReelWork *state;
  struct BattleEffectWork *work;
  struct EffectStep *letter;
  const u8 *width;
  s32 left;
  s32 step;
  s32 y;
  s32 i;
  s32 angle;
  struct UiWindow *window;
  s32 message;

  state = ((struct ReelWork **)
               gBattleFxWork)[REEL_HEAP_SLOT - HEAP_SLOT_BATTLE_EFFECT];
  work = gBattleFxWork[0];
  left = 0;
  for (i = 0; i != 0x800; i++)
    REEL_SPARKS[i].variant = 0;

  letter = work->particles;
  y = -0x200000;
  width = gReelGame_TitleLetterWidths;
  for (i = 0; i != REEL_TITLE_LETTERS; i++) {
    letter[i].x = (left + REEL_TITLE_X) << 16;
    step = *width;
    width++;
    left += step;
    letter[i].y = y;
    letter[i].velocityY = 0;
    letter[i].variant = 0;
    y += -0x80000;
  }

  for (i = 0; i != 160; i++)
    state->scanlineOffsets[i] = 0;

  for (i = 0; i != 40; i++) {
    angle = i * 0x199;
    state->scanlineOffsets[23 + i] = (u32)(Trig_Cos(angle) * 3) >> 15;
    state->scanlineOffsets[110 - i] = (u32)(Trig_Cos(angle) * 3) >> 15;
  }

  state->spins = 0;
  state->state = REEL_BETTING;
  state->cursor = 0;
  work->transferMode = 1;
  work->transferValue = 0;
  REG_BLDCNT = 0;
  window = UiWindow_CreateFar(18, 0, 12, 4, 6);
  state->subWindow = window;
  message = MsgSlotsBet;
  UiText_DrawCharacterAtOffsetFar(message, window, 0, 8);
  UiText_DrawCharacterAtOffsetFar(message - 1, state->subWindow, 0, 0);
}

/// The slot machine: loads its backgrounds, sprites and palettes, fills each of
/// the five reels with 21 random symbols (with eight placed symbols of rank 0
/// to 5), then runs until the player leaves: fading in, drawing the paylines
/// on the canvas while betting or winning, and the jackpot's sparks and
/// colour cycle while showing a win. Then fades out and frees its memory.
void ReelGame_Run(void) {
  s32 picks[8];
  s32 litLines[7];
  DrawRectangle draw[2];
  u8 *sheet;
  void *canvas;
  struct BattleEffectWork *work;
  struct ReelWork *state;
  struct ReelSave *save;
  u8 *entry;
  u8 *tiles;
  s32 frame;
  u32 count;
  s32 i;
  s32 j;
  s32 k;

  sheet = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x60e);
  canvas = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, REEL_CANVAS_SIZE);
  work = Runtime_AllocateBlock(HEAP_SLOT_BATTLE_EFFECT,
                               sizeof(struct BattleEffectWork));
  state = Runtime_AllocateBlock(REEL_HEAP_SLOT, sizeof(struct ReelWork));
  tiles = Ram_MapCellBuffer;
  save = (struct ReelSave *)((u8 *)&gGameState + 12);
  RuntimeDispatch_NoOpHook((s32)&ResourceId_EffectFarCalls);
  save->wonPrizes[0] = -1;
  state->repeat = 0;
  state->bet = 1;
  Scheduler_ResetTaskTable();
  gOamCopyEnabled = 0;

  count = 0;
  j = 0;
  for (k = 0; k != 20; k++) {
    for (i = 0; i != 32; i++, j += 2) {
      if (i >= 5 && i <= 24 && k > 2 && k <= 13) {
        *(volatile u16 *)(BG_SCREEN_BLOCK(5) + j) = 0xa1a6;
      } else if (i > 29) {
        *(volatile u16 *)(BG_SCREEN_BLOCK(5) + j) = 0;
      } else {
        *(volatile u16 *)(BG_SCREEN_BLOCK(5) + j) = 0xa1a8 + count;
        count++;
      }
    }
  }
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD), sheet);
  entry = Resource_GetTableEntry((s32)&ResourceId_SlotsFramePicture);
  Dma_Set(entry, (void *)&BG_PLTT_COLOR(10, 0),
          (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  entry += 32;
  Resource_DecodeType01(entry, tiles);
  j = 0;
  for (k = 0; k != 20; k++) {
    for (i = 0; i != 30; i++) {
      if (!(i >= 5 && i <= 24 && k > 2 && k <= 13)) {
        Dma_Set(tiles + (k * 30 + i) * 32,
                (void *)(BG_CHAR_BLOCK(2) + 0x3500 + j),
                (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
        j += 32;
      }
    }
  }
  Iwram_ClearWords(BG_SCREEN_BLOCK(5) + 0x500, 0x300);
  j = 0;
  for (k = 0; k != 20; k++) {
    for (i = 0; i != 32; i++, j += 2) {
      if (k < 2 || k > 16)
        *(volatile u16 *)(BG_SCREEN_BLOCK(6) + j) = 0xbf;
      else
        *(volatile u16 *)(BG_SCREEN_BLOCK(6) + j) = k * 32 + i + 148;
    }
  }
  REG_BG1CNT = BGCNT_SCREEN_BASE(5) | BGCNT_CHAR_BASE(2) | BGCNT_PRIORITY(1);
  REG_BG2CNT = BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6);
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  gBgScroll[1].x = 0;
  gBgScroll[1].y = 0;
  gBgScroll[2].x = 0;
  gBgScroll[2].y = 0;
  REG_WININ = WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 |
              WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG0 |
              WININ_WIN1_BG1 | WININ_WIN1_BG2 | WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 |
               WINOUT_WIN01_CLR | WINOUT_WINOBJ_BG0 | WINOUT_WINOBJ_BG1 |
               WINOUT_WINOBJ_BG2 | WINOUT_WINOBJ_CLR;
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BG1HOFS = 0;
  REG_BG2HOFS = 0;
  REG_BG1VOFS = 0xff60;
  REG_BG2VOFS = 0xff60;
  REG_WIN0H = WIN_RANGE(40, 200);
  REG_WIN0V = WIN_RANGE(24, 120);
  REG_WIN0H = WIN_RANGE(0, 240);
  REG_WIN0V = WIN_RANGE(0, 160);
  REG_WIN1H = WIN_RANGE(0, 240);
  REG_WIN1V = WIN_RANGE(0, 160);

  {
    u16 *shade = (u16 *)(work->sheet + 0x200);
    s32 size = REEL_CANVAS_SIZE;

    state->state = REEL_BETTING;
    state->cursor = 0;
    state->spins = 0;
    work->frame = 0;
    state->timer = 0;
    Dma_Set(Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet),
            (void *)BG_PLTT, (DMA_ENABLE | DMA_32BIT) << 16 | 32, REG_DMA3);
    // The paylines' two greens, colours 64 and 65 on the canvas.
    BG_PLTT_COLOR(4, 0) = RGB(11, 28, 11);
    BG_PLTT_COLOR(4, 1) = RGB(22, 31, 22);
    entry = Resource_GetTableEntry((s32)&ResourceId_SlotsIcons);
    Dma_Set(entry, (void *)OBJ_PLTT, (DMA_ENABLE | DMA_32BIT) << 16 | 480 / 4,
            REG_DMA3);
    entry += 480;
    Resource_DecodeType01(entry, tiles);
    Dma_Set(tiles, (void *)OBJ_VRAM0, (DMA_ENABLE | DMA_32BIT) << 16 | 0x1b30,
            REG_DMA3);
    entry = Resource_GetTableEntry((s32)&ResourceId_SlotsJackpot);
    Dma_Set(entry, (void *)&OBJ_PLTT_COLOR(15, 0),
            (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
    entry += 32;
    Resource_DecodeType01(entry, tiles);
    Dma_Set(tiles, (void *)REEL_JACKPOT_TILES, REEL_JACKPOT_COPY, REG_DMA3);
    UiWork_InitializeWithResourceCountersFar();
    ReelGame_InitTitle();

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

    BattleEffect_LoadWorkFar(HEAP_SLOT_BLITTER, 8, 7, 3, 2);
    draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
    BattleEffect_LoadWorkFar(HEAP_SLOT_BLITTER_ALTERNATE, 8, 7, 3, 3);
    draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    Iwram_FillWords(canvas, size, 0);
    Dma_Set(canvas, (void *)REEL_CANVAS_TILES,
            (DMA_ENABLE | DMA_32BIT) << 16 | REEL_CANVAS_SIZE / 4, REG_DMA3);
    Dma_Set((void *)BG_PLTT, work, (DMA_ENABLE | DMA_32BIT) << 16 | 0x80,
            REG_DMA3);
    Dma_Set((void *)OBJ_PLTT, shade, (DMA_ENABLE | DMA_32BIT) << 16 | 0x80,
            REG_DMA3);
    Graphics_ScaleRgb555(shade, (u16 *)OBJ_PLTT, 0, 0x100);
    Graphics_ScaleRgb555((u16 *)work, (u16 *)BG_PLTT, 0, 0x100);
    REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
                  DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON;
    if (PartyInventory_CountItemFar(ITEM_GAME_TICKET) == 1) {
      struct UiWindow *window = UiWindow_CreateFar(6, 16, 18, 3, 6);

      state->window = window;
      UiText_DrawCharacterAtOffsetFar(MsgSlotsControls, window, 0, 0);
    } else {
      struct UiWindow *window = UiWindow_CreateFar(
          REEL_HELP_WINDOW_X, 16, REEL_HELP_WINDOW_WIDTH, 4, 6);
      s32 message;

      state->window = window;
      message = MsgSlotsChangeBet;
      UiText_DrawCharacterAtOffsetFar(message, window, 0, 0);
      UiText_DrawCharacterAtOffsetFar(message + 1, state->window, 0, 8);
    }
    work->transferPending = 0;
    Scheduler_AddOrUpdateCallback(ReelGame_RunFrame, 0x480);
    Scheduler_AddOrUpdateCallback(ReelGame_TransferCanvas, 0x480);

    for (frame = 0; state->state != REEL_DONE; frame++) {
      if (frame <= 16) {
        Graphics_ScaleRgb555(shade, (u16 *)OBJ_PLTT, frame << 12, 0x100);
        Graphics_ScaleRgb555((u16 *)work, (u16 *)BG_PLTT, frame << 12, 0x100);
      }
      if (state->state == REEL_SHOWING_WIN) {
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
          struct EffectStep *spark = &REEL_SPARKS[(frame / 8 & 3) * 256];

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
          struct EffectStep *spark = &REEL_SPARKS[j];

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
                              sheet + gReelGame_SparkCellOffsets[size - 1],
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
      if (state->state == REEL_BETTING || state->state == REEL_WON) {
        for (j = 0; j != 7; j++)
          litLines[j] = 0;
        if (state->state == REEL_BETTING) {
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
            ReelGame_DrawCanvasLine(20, j + 19, 200, j + 19, color);
          if (litLines[2])
            ReelGame_DrawCanvasLine(28, j + 35, 200, j + 35, color);
          if (litLines[3])
            ReelGame_DrawCanvasLine(20, j + 51, 200, j + 51, color);
          if (litLines[4])
            ReelGame_DrawCanvasLine(28, j + 67, 200, j + 67, color);
          if (litLines[5])
            ReelGame_DrawCanvasLine(20, j + 83, 200, j + 83, color);
          if (litLines[0])
            ReelGame_DrawCanvasLine(28, j + 5, 200, j + 91, color);
          if (litLines[6])
            ReelGame_DrawCanvasLine(28, j + 97, 200, j + 11, color);
        }
      }
      work->transferPending = 1;
      WaitFrames(1);
    }

    for (j = 0; j != 17; j++) {
      Graphics_ScaleRgb555(shade, (u16 *)OBJ_PLTT, 0x10000 - (j << 12), 0x100);
      Graphics_ScaleRgb555((u16 *)work, (u16 *)BG_PLTT, 0x10000 - (j << 12),
                           0x100);
      WaitFrames(1);
    }
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    Scheduler_RemoveCallback(ReelGame_TransferCanvas);
    Scheduler_RemoveCallback(ReelGame_RunFrame);
    Runtime_ReleaseHeapBlock(REEL_HEAP_SLOT);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
  }
}

// The LZ packer: a window of 1024 bytes, matches of up to PACK_MATCH_MAX
// bytes at most PACK_DISTANCE_MAX back, and PACK_LOOKAHEAD bytes read ahead.
#define PACK_WINDOW_MASK 0x3ff
#define PACK_MATCH_MAX 272
#define PACK_DISTANCE_MAX 63
#define PACK_LOOKAHEAD 672

/// The LZ packer below: empties its 256 hash chains (one per byte value) and
/// numbers its 1024 window nodes.
void LzPack_ResetSlotBuckets(void) {
  s32 index;
  struct LzPackNode **bucket;

  for (index = 0; index < 0x400; index++) {
    gFlash_Handler3->nodes[index].slot = index;
    gFlash_Handler3->nodes[index].prev = NULL;
  }
  bucket = gFlash_Handler3->bucket;
  for (index = 0xFF; index >= 0; index--) {
    *bucket++ = NULL;
  }
}

/// Adds window position index to the chain of the byte it holds.
void LzPack_InsertSlotNode(s32 index) {
  struct LzPackWork *work = gFlash_Handler3;

  work->nodes[index].prev =
      (struct LzPackNode *)&work->bucket[work->bucketBySlot[index]];
  work->nodes[index].next = work->bucket[work->bucketBySlot[index]];
  work->bucket[work->bucketBySlot[index]] = &work->nodes[index];
  if (work->nodes[index].next != NULL)
    work->nodes[index].next->prev = &work->nodes[index];
}

/// Removes window position slot from its chain.
void LzPack_RemoveSlotNode(s32 slot) {
  if (gFlash_Handler3->nodes[slot].prev != NULL) {
    if (gFlash_Handler3->nodes[slot].next != NULL)
      gFlash_Handler3->nodes[slot].next->prev =
          gFlash_Handler3->nodes[slot].prev;
    gFlash_Handler3->nodes[slot].prev->next = gFlash_Handler3->nodes[slot].next;
  }
}

/// Reads count more input bytes into the window from position start, dropping
/// the bytes they replace from their chains; past the end of the input the
/// positions are marked empty (-1).
void LzPack_ConsumeSlotBytes(s32 start, s32 count, const u8 *input) {
  s32 limit = count;
  s32 current = 0;
  u32 base = start;

  if (current < limit) {
    u32 mask = PACK_WINDOW_MASK;
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
    u32 mask = PACK_WINDOW_MASK;
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

/// Appends the finished group (a flag byte and up to eight items) to the
/// output.
void LzPack_CopyBufferedBytes(u8 *destination) {
  struct LzPackWork *work = gFlash_Handler3;
  s32 index;

  for (index = 0; index != work->outCnt; index++) {
    destination[work->outTotal] = work->outBuf[index];
    work->outTotal++;
  }
}

/// Finds the longest earlier match (up to PACK_MATCH_MAX bytes, at most
/// PACK_DISTANCE_MAX back) for the bytes at window position start; matchLen
/// stays 1 for none.
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

/// Packs size bytes of input into dst with a small LZ scheme and returns the
/// packed size. Each group starts with a flag byte, a set bit marking a match:
/// two bytes of distance and length (lengths 2 to 16), or three for lengths up
/// to 272; a match is put off by one byte when the next position matches
/// further. A zero match ends the stream. Nothing in this game calls it.
UNUSED s32 LzPack_PackStream(const u8 *input, u8 *dst, s32 size) {
  s32 defer;
  s32 len;
  s32 saveOffset;
  s32 saveLen;
  s32 reach[2];
  s16 code;

  defer = 0;
  gFlash_Handler3 = (struct LzPackWork *)gMapCellBuffer;
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
