#include "audio_engine_symbols.h"
#include "unit/overworld/lucky_wheels.h"
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
#include "battle_anim_work.h"
#include "heap.h"
#include "common.h"
#include "types.h"
#include "macros.h"
#include "dma.h"
#include "iwram_call.h"
#include "resource.h"
#include "io_reg.h"
#include "songs.h"
#include "mem.h"
#include "fixed_math.h"
#include "particle.h"
#include "blitter.h"
#include "task.h"
#include "map_grid_scroll.h"
#include "ram_buffer.h"
#include "resource_ids.h"
#include "tbs_edition.h"
#include "text/msg_ids.h"
#include "lzpack.h"
#include "frame.h"
#include "oambuf.h"
#include "item_ids.h"

//--------------------------------------------------------------------------
// Lucky Wheels: its palette steps, canvas transfers and line drawing, and an LZ
// packer.
//--------------------------------------------------------------------------

/// The sparks of the jackpot, kept in the map cell buffer.
#define LUCKY_WHEELS_SPARKS ((struct Particle *)Ram_MapGridCellBuffer)

/// Lucky Wheels' own work block (struct LuckyWheelsWork), beside the
/// battle animation blocks it borrows.
#define LUCKY_WHEELS_HEAP_SLOT 45

/// The paylines' canvas: 256 x 128 pixels of 8-bit colour, shown on BG2 from
/// its tile 212, where the third row of its map begins.
#define LUCKY_WHEELS_CANVAS_SIZE (256 * 128)
#define LUCKY_WHEELS_CANVAS_TILES (BG_CHAR_BLOCK(0) + 0x3500)

/// Plays Lucky Wheels: blanks the screen, seeds the random numbers with the
/// play time and runs the game. Returns 0.
s32 LuckyWheels_Start(void) {
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
  gRandomSeed = gCommon.playTime;
  Sound_PlaySongFar(MUS_DJINN_LESSON);
  LuckyWheels_Run();
  return 0;
}

/// Writes count RGB555 colours from src to dst scaled by scale (16.16, 0x10000
/// keeps them). Returns 0.
s32 LuckyWheels_ScaleRgb555(u16 *src, u16 *dst, s32 scale, s32 count) {
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

/// A VBlank-pass task: on a VBlank after the canvas is ready (transferPending),
/// sends the canvas to its tiles in VRAM (LUCKY_WHEELS_CANVAS_TILES), then
/// clears it or fades it into trails as transferMode says.
void LuckyWheels_TransferCanvas(void) {
  struct BattleAnimWork *work;
  u8 *buf;

  work = gBattleAnimWork[0];
  if (work->transferPending == 1) {
    buf = gBattleAnimWork[1];
    switch (work->transferMode) {
    case 1:
      // Then fill the canvas with transferValue.
      Dma_Set(buf, (void *)LUCKY_WHEELS_CANVAS_TILES,
              (DMA_ENABLE | DMA_32BIT) << 16 | LUCKY_WHEELS_CANVAS_SIZE / 4,
              REG_DMA3);
      Iwram_FillWords(buf, LUCKY_WHEELS_CANVAS_SIZE, work->transferValue);
      break;
    case 2:
      // Trails: fade nonzero colour numbers to 1/2 or 3/4.
      if (work->transferValue == 50)
        PaletteFade_BackupAndHalveNonzero(buf, LUCKY_WHEELS_CANVAS_TILES,
                                          LUCKY_WHEELS_CANVAS_SIZE);
      else
        PaletteFade_BackupAndScaleNonzeroThreeQuarters(
            buf, LUCKY_WHEELS_CANVAS_TILES, LUCKY_WHEELS_CANVAS_SIZE);
      break;
    }
    work->transferPending = 0;
    work->vblanksSinceTransfer = 1;
  } else {
    (work->vblanksSinceTransfer)++;
  }
}

/// Darkens the screen one step: lowers each channel of the 16 colours of BG
/// palette 10 and of OBJ colours 1 to 239 by 1.
void Palette_DarkenScreenStep(void) {
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
void LuckyWheels_DrawCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color) {
  s32 dx = x1 - x0;
  s32 dy = y1 - y0;
  s32 frac = 0x80; // 0.5 in 8-bit fixed point
  u8 *canvas = ((u8 **)gBattleAnimWork)[1];
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

/// One of Lucky Wheels' five wheels: how far it has turned, its 21
/// symbols, whether it is held, and the countdown to its stop (-1 until one
/// starts).
struct Reel {
  s32 position; ///< pixels, 16 a symbol
  u8 symbols[21];
  u8 held;
  s8 stop; ///< frames
};

struct Window;

/// Lucky Wheels' states (LuckyWheelsWork.state).
enum LuckyWheelsState {
  LUCKY_WHEELS_BETTING = 0,  ///< Up and Down change the bet, A spins, B leaves
  LUCKY_WHEELS_SPINNING = 1, ///< the wheels spin until R or time stops each
  LUCKY_WHEELS_WON = 2,      ///< a payline won: a pause
  LUCKY_WHEELS_SHOWING_WIN = 3,    ///< the win is shown until A
  LUCKY_WHEELS_CHOOSING_HOLDS = 5, ///< choose wheels to hold, or pull the lever
  LUCKY_WHEELS_DONE = 10,          ///< the game ends
  LUCKY_WHEELS_NO_WIN = 11,        ///< no payline won: A to choose holds
  LUCKY_WHEELS_OUT_OF_SPINS =
      20, ///< the fourth spin lost: a pause, then the end
};

/// Lucky Wheels' work in heap slot 45: its wheels, state, bet and spins,
/// keys, timer, winning lines, sprites and windows.
struct LuckyWheelsWork {
  struct Reel reels[5];
  s32 state;
  s32 cursor;
  s32 spins;
  s32 bet;
  u16 keys;
  u16 direction;
  u16 pressed;
  u16 repeat;
  u8 unused[4]; ///< no code names it
  s32 timer;
  s32 winningLines[7];
  struct OamWords oam[128];
  struct Window *window;
  struct Window *subWindow;
  u8 unusedAfterWindows[8]; ///< no code names it
  u16 scanlineOffsets[160];
#if TBS_PATCH_1
  s32 phase;
#endif
};

/// gCommon seen from +12, the base the bytes load: wonPrizes is
/// gCommon.wonPrizes (through the field itself LuckyWheels_Update
/// differs 12)
struct CommonWonPrizesView {
  u8 beforeWonPrizes[0x120]; ///< gCommon from summonMask to wonPrizes
  s8 wonPrizes[16];
};

#include "../graphics/lucky_wheels/reel_tables.h"

/// Lucky Wheels' VBlank task: reads the keys, runs the game's state
/// (betting, spinning and stopping the wheels, scoring the paylines, choosing
/// wheels to hold, showing a win) and builds all 128 sprites: the cursor, the
/// title letters of a win, the frame, the hold lamps, the spin counter, the
/// paylines, the wheel symbols, the lever and the bottom bar.
void LuckyWheels_Update(void) {
  /* FAKEMATCH(volatile): struct OamWords's volatile words (oambuf.h) keep
   * each list loop's offset givs; as plain u32 words, 1242 of 1729
   * instructions differ in tbs-en. */
  struct LuckyWheelsWork *work;
  struct BattleAnimWork *fx;
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

  work = ((struct LuckyWheelsWork **)
              gBattleAnimWork)[LUCKY_WHEELS_HEAP_SLOT - HEAP_SLOT_BATTLE_ANIM];
  fx = gBattleAnimWork[0];
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
    if (work->state == LUCKY_WHEELS_BETTING) {
      tickets = PartyInventory_CountItemFar(ITEM_GAME_TICKET);
      Text_DrawNumberInWindowFar(tickets - work->bet, LUCKY_WHEELS_COIN_DIGITS,
                                 work->subWindow, LUCKY_WHEELS_COIN_X, 0);
      Text_DrawNumberInWindowFar(work->bet, 2, work->subWindow, 64, 8);
      if (work->pressed & KEY_B) {
        struct CommonWonPrizesView *save =
            (struct CommonWonPrizesView *)((u8 *)&gCommon + 12);

        work->state = LUCKY_WHEELS_DONE;
        save->wonPrizes[0] = -2;
        Window_CloseFar(work->window, 1);
      } else {
        if (work->pressed & KEY_UP) {
          if (work->bet <= 3 && tickets > work->bet) {
            work->bet++;
            Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
          } else {
            Sound_PlaySongFar(SE_MENU_CANCEL);
          }
        }
        if (work->pressed & KEY_DOWN) {
          if (work->bet > 1) {
            work->bet--;
            Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
          } else {
            Sound_PlaySongFar(SE_MENU_CANCEL);
          }
        }
        REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_DARKEN | BLDCNT_TGT2_ALL;
        REG_BLDALPHA = BLEND_ALPHA(16, 0);
        if (work->pressed & KEY_A) {
          work->state = LUCKY_WHEELS_SPINNING;
          fx->frame = 0;
          Window_CloseFar(work->window, 1);
          for (k = 0; k != work->bet; k++)
            PartyInventory_RemoveFar(ITEM_GAME_TICKET);
          Window_CloseFar(work->subWindow, 1);
          Sound_PlaySongFar(SE_LUCKY_WHEELS_SPIN);
        }
      }
    } else if (work->state == LUCKY_WHEELS_CHOOSING_HOLDS) {
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
          work->state = LUCKY_WHEELS_BETTING;
          for (i = 0; i != 5; i++) {
            work->reels[i].held = 0;
            work->reels[i].stop = -1;
          }
        } else if (work->cursor <= 4) {
          Sound_PlaySongFar(SE_LUCKY_WHEELS_HOLD);
          work->reels[work->cursor].held ^= 1;
        } else if (allHeld == 0) {
          Sound_PlaySongFar(SE_LUCKY_WHEELS_SPIN);
          work->state = LUCKY_WHEELS_SPINNING;
          work->cursor = 0;
          for (i = 0; i != 5; i++)
            work->reels[i].stop = -1;
          work->spins++;
        } else {
          Sound_PlaySongFar(SE_MENU_CANCEL);
        }
      } else {
        if (work->direction & KEY_RIGHT) {
          work->cursor = (work->cursor + 1) % 6;
          Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        }
        if (work->direction & KEY_LEFT) {
          work->cursor = (work->cursor + 5) % 6;
          Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        }
      }

      if (work->state == LUCKY_WHEELS_CHOOSING_HOLDS) {
#if TBS_PATCH_1
        if (work->cursor == 5) {
          if (allHeld != 0) {
            if (work->phase != 1 && work->phase != 2) {
              Window_CloseFar(work->window, 1);
              work->window =
                  Window_CreateFar(LUCKY_WHEELS_ALL_HELD_X, 0,
                                   LUCKY_WHEELS_ALL_HELD_WIDTH, 4, 6);
              Text_DrawMessageAsTilesFar(MsgSlotsAllHeld, work->window, 0, 0);
              work->phase = 1;
            } else if (work->phase == 1) {
              Text_DrawMessageAsTilesFar(MsgSlotsCannotPull, work->window, 0,
                                         8);
              work->phase = 2;
            }
          } else {
            if (work->phase != 3) {
              Window_CloseFar(work->window, 1);
              work->window = Window_CreateFar(LUCKY_WHEELS_START_X, 0,
                                              LUCKY_WHEELS_START_WIDTH, 3, 6);
              Text_DrawMessageAsTilesFar(MsgSlotsStart, work->window, 0, 0);
            }
            work->phase = 3;
          }
        } else if (work->reels[work->cursor].held == 0) {
          if (work->phase != 4) {
            Window_CloseFar(work->window, 1);
            work->window = Window_CreateFar(LUCKY_WHEELS_HOLD_X, 0,
                                            LUCKY_WHEELS_HOLD_WIDTH, 3, 6);
            Text_DrawMessageAsTilesFar(MsgSlotsHold, work->window, 0, 0);
          }
          work->phase = 4;
        } else {
          if (work->phase != 5) {
            Window_CloseFar(work->window, 1);
            work->window = Window_CreateFar(LUCKY_WHEELS_RELEASE_X, 0,
                                            LUCKY_WHEELS_RELEASE_WIDTH, 3, 6);
            Text_DrawMessageAsTilesFar(MsgSlotsRelease, work->window, 0, 0);
          }
          work->phase = 5;
        }
#else
        Window_CloseFar(work->window, 1);
        if (work->cursor == 5) {
          if (allHeld != 0) {
            work->window = Window_CreateFar(12, 0, 18, 3, 6);
            Text_DrawMessageAsTilesFar(MsgSlotsAllHeld, work->window, 0, 0);
          } else {
            work->window = Window_CreateFar(18, 0, 12, 3, 6);
            Text_DrawMessageAsTilesFar(MsgSlotsStart, work->window, 0, 0);
          }
        } else {
          work->window = Window_CreateFar(18, 0, 12, 3, 6);
          if (work->reels[work->cursor].held == 0)
            Text_DrawMessageAsTilesFar(MsgSlotsHold, work->window, 0, 0);
          else
            Text_DrawMessageAsTilesFar(MsgSlotsRelease, work->window, 0, 0);
        }
#endif
      } else {
        Window_CloseFar(work->window, 1);
      }
    } else if (work->state == LUCKY_WHEELS_WON) {
      work->timer++;
      objMode = 0;
      if (work->timer == 60) {
        work->state = LUCKY_WHEELS_SHOWING_WIN;
        Sound_PlaySongFar(MUS_FANFARE_LARGE);
        work->timer = 0;
        REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        REG_BLDALPHA = BLEND_ALPHA(16, 16);
        fx->transferMode = CANVAS_COPY_AND_FADE;
        fx->transferValue = 75;
      }
    } else if (work->state == LUCKY_WHEELS_SHOWING_WIN) {
      work->timer++;
      objMode = 0;
      if (work->pressed & KEY_A) {
        work->state = LUCKY_WHEELS_DONE;
        Sound_PlaySongFar(SE_MENU_CONFIRM);
      }
    } else if (work->state == LUCKY_WHEELS_NO_WIN) {
#if TBS_PATCH_1
      if (work->phase == 0) {
        work->phase = 1;
        Text_DrawMessageAsTilesFar(MsgSlotsPullLever, work->window, 0, 8);
      }
#endif
      if (work->pressed & KEY_A) {
        work->state = LUCKY_WHEELS_CHOOSING_HOLDS;
#if TBS_PATCH_1
        work->phase = 0;
#endif
        Sound_PlaySongFar(SE_MENU_CONFIRM);
        Window_CloseFar(work->window, 1);
      }
    } else if (work->state == LUCKY_WHEELS_OUT_OF_SPINS) {
      work->timer++;
      if (work->timer == 45)
        work->state = LUCKY_WHEELS_DONE;
    } else if (work->state != LUCKY_WHEELS_DONE) {
      if (work->timer == 4) {
        work->window = Window_CreateFar(LUCKY_WHEELS_STOP_X, 17,
                                        LUCKY_WHEELS_STOP_WIDTH, 3, 6);
        Text_DrawMessageAsTilesFar(MsgSlotsStopReels, work->window, 0, 0);
      }
      if (work->timer == 16)
        Sound_PlaySongFar(SE_LUCKY_WHEELS_STOP_PROMPT);
      if (work->timer > 56) {
        if (fx->frame > 31 || (work->pressed & KEY_R)) {
          fx->frame = 0;
          for (i = 0; i != 5; i++) {
            if (work->reels[i].held == 0 && work->reels[i].stop == -1) {
              work->reels[i].stop = (Random16() & 3) + 4;
              Sound_PlaySongFar(SE_LUCKY_WHEELS_STOP);
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
        struct CommonWonPrizesView *save =
            (struct CommonWonPrizesView *)((u8 *)&gCommon + 12);
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
          work->state = LUCKY_WHEELS_WON;
          Sound_PlaySongFar(SE_THUNDER);
          fx->transferMode = CANVAS_COPY_AND_CLEAR;
          fx->transferValue = 0;
          REG_BLDCNT = 0;
          Window_CloseFar(work->window, 1);
        } else {
          work->state = LUCKY_WHEELS_NO_WIN;
#if TBS_PATCH_1
          work->phase = 0;
          Window_CloseFar(work->window, 1);
#if LUCKY_WHEELS_HINT_BEFORE_LAST_SPIN
          if (work->spins <= 3)
#endif
          {
            work->window = Window_CreateFar(3, 16, 24, 4, 6);
            Text_DrawMessageAsTilesFar(MsgSlotsChooseHeld, work->window, 0, 0);
          }
#else
          Window_CloseFar(work->window, 1);
          {
            s32 message;

            work->window = Window_CreateFar(7, 16, 16, 4, 6);
            message = MsgSlotsChooseHeld;
            Text_DrawMessageAsTilesFar(message, work->window, 0, 0);
            Text_DrawMessageAsTilesFar(message + 1, work->window, 0, 8);
          }
#endif
          if (work->spins == 4) {
            s32 left = PartyInventory_CountItemFar(ITEM_GAME_TICKET);
            struct Window *window;
            s32 message;

            if (left > 0)
              work->state = LUCKY_WHEELS_OUT_OF_SPINS;
            else
              work->state = LUCKY_WHEELS_OUT_OF_SPINS;
            if (work->bet > left)
              work->bet = left;
            work->spins = 0;
            work->cursor = 0;
            for (i = 0; i != 5; i++) {
              work->reels[i].held = 0;
              work->reels[i].stop = -1;
            }
            fx->transferMode = CANVAS_COPY_AND_CLEAR;
            fx->transferValue = 0;
            REG_BLDCNT = 0;
            window = Window_CreateFar(18, 0, 12, 4, 6);
            work->subWindow = window;
            message = MsgSlotsBet;
            Text_DrawMessageAsTilesFar(message, window, 0, 8);
            Text_DrawMessageAsTilesFar(message - 1, work->subWindow, 0, 0);
          }
        }
      }

      if (work->state == LUCKY_WHEELS_SPINNING) {
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

  if (work->state == LUCKY_WHEELS_CHOOSING_HOLDS) {
    x = work->cursor * 36 + 36;
    y = 128;
    v = 0;
    if ((work->timer & 15) <= 7)
      v = 1;
    if (work->cursor == 5) {
      x = 208;
      y = 32;
    }
    work->oam[sprite].attr01 =
        ((x - 12) << 16) | objMode | (y + 8) | 0x80006000;
    work->oam[sprite].attr2 = (v << 4) + 0x2b0;
    sprite++;
    work->oam[sprite].attr01 =
        ((x + 12) << 16) | objMode | (y + 8) | 0x90006000;
    work->oam[sprite].attr2 = (v << 4) + 0x2b0;
    sprite++;
    work->oam[sprite].attr01 = (x << 16) | objMode | y | 0x80002000;
    work->oam[sprite].attr2 = 0x1f0;
    sprite++;
  }

  if (work->state == LUCKY_WHEELS_SHOWING_WIN) {
    for (i = 0; i != LUCKY_WHEELS_TITLE_LETTERS; i++) {
      struct Particle *p = &fx->particles[i];

#if (LANGUAGE == LANGUAGE_SPANISH)
      if (i == 0)
        work->oam[sprite].attr01 =
            (((*(s16 *)((u8 *)&p->x + 2) + 0x1f0) & 0x1ff) << 16) | objMode |
            ((*(s16 *)((u8 *)&p->y + 2) + 0x108) & 255) | 0xb0000000;
      else
#endif
        work->oam[sprite].attr01 = (*(s16 *)((u8 *)&p->x + 2) << 16) | objMode |
                                   ((*(s16 *)((u8 *)&p->y + 2) + 256) & 255) |
                                   0x80000000;
      work->oam[sprite].attr2 =
          ((gLuckyWheels_TitleLetterTiles[i] << 4) + LUCKY_WHEELS_TITLE_TILES) |
          0xf000;
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
    work->oam[sprite].attr01 = (gLuckyWheels_FrameSpriteX[k] << 16) | objMode |
                               gLuckyWheels_FrameSpriteY[k] | 0x80006000;
    if (k <= 3)
      work->oam[sprite].attr2 = 0x4e0;
    else
      work->oam[sprite].attr2 = 0x4e8;
    sprite++;
  }

  for (i = 0; i != 5; i++) {
    if (work->reels[i].held == 0) {
      work->oam[sprite].attr01 = objMode | ((i * 36 + 32) << 16) | 0x8000207c;
      work->oam[sprite].attr2 = 0x460;
    } else {
      work->oam[sprite].attr01 = objMode | ((i * 36 + 32) << 16) | 0x8000207c;
      work->oam[sprite].attr2 = 0x480;
    }
    sprite++;
  }

  for (i = 0; i != 5; i++) {
    work->oam[sprite].attr01 = objMode | ((i * 16 + 32) << 16) | 0x80006003;
    if (i == work->spins)
      work->oam[sprite].attr2 = (i * 32 + 0x210) | 0x400;
    else
      work->oam[sprite].attr2 = (i * 32 + 0x220) | 0x400;
    sprite++;
  }

  for (col = 0; col != 7; col++) {
    work->oam[sprite].attr01 = (((0x204 - ((col & 1) << 3)) & 0x1ff) << 16) |
                               objMode | (col * 16 + 5) | 0x80002000;
    if (col > 3 - work->bet && col < work->bet + 3)
      work->oam[sprite].attr2 = 0x5d0;
    else
      work->oam[sprite].attr2 = 0x510;
    sprite++;
  }

  for (i = 0; i != 5; i++) {
    struct Reel *reel = &work->reels[i];

    for (col = 0; col != 7; col++) {
      work->oam[sprite].attr01 = (col * 16 + reel->position % 16 + 4) |
                                 ((i * 32 + 40) << 16) | 0x80006000;
      work->oam[sprite].attr2 =
          (reel->symbols[(col - reel->position / 16 + 21) % 21] << 4) | 0x800;
      sprite++;
    }
  }

  lever = 40;
  if (work->state == LUCKY_WHEELS_SPINNING) {
    if (work->timer <= 47)
      lever = ((Trig_Sin(work->timer * 682) << 6) >> 16) + 40;
    else if (work->timer <= 55)
      lever = ((Trig_Sin((work->timer << 12) - 0x30000) << 2) >> 16) + 40;
  }
  work->oam[sprite].attr01 = lever | objMode | 0x80d06000;
  work->oam[sprite].attr2 = 0x500;
  sprite++;

  for (k = 0; k != 8; k++) {
    work->oam[sprite].attr01 = objMode | (k * 16 + 12) | 0x80ce6000;
    if (k == 0)
      work->oam[sprite].attr2 = 0x540;
    else if (k == 1)
      work->oam[sprite].attr2 = 0x550;
    else if (k == 6)
      work->oam[sprite].attr2 = 0x570;
    else if (k == 7)
      work->oam[sprite].attr2 = 0x580;
    else
      work->oam[sprite].attr2 = 0x560;
    sprite++;
  }

  while (sprite != 128) {
    work->oam[sprite].attr01 = 0x40f02000;
    work->oam[sprite].attr2 = 0;
    sprite++;
  }

  Dma_Set(work->oam, OAM, (sprite * 2) | (DMA_ENABLE | DMA_32BIT) << 16,
          REG_DMA3);
}

/// Sets up the bouncing title letters of a win, the brightness wave of the
/// screen's lines and the betting window, and starts at betting.
void LuckyWheels_InitializeTitle(void) {
  struct LuckyWheelsWork *state;
  struct BattleAnimWork *work;
  struct Particle *letter;
  const u8 *width;
  s32 left;
  s32 step;
  s32 y;
  s32 i;
  s32 angle;
  struct Window *window;
  s32 message;

  state = ((struct LuckyWheelsWork **)
               gBattleAnimWork)[LUCKY_WHEELS_HEAP_SLOT - HEAP_SLOT_BATTLE_ANIM];
  work = gBattleAnimWork[0];
  left = 0;
  for (i = 0; i != 0x800; i++)
    LUCKY_WHEELS_SPARKS[i].variant = 0;

  letter = work->particles;
  y = -0x200000;
  width = gLuckyWheels_TitleLetterWidths;
  for (i = 0; i != LUCKY_WHEELS_TITLE_LETTERS; i++) {
    letter[i].x = (left + LUCKY_WHEELS_TITLE_X) << 16;
    step = *width;
    width++;
    left += step;
    letter[i].y = y;
    letter[i].velocityY = 0;
    letter[i].variant = 0;
    y += -0x80000;
  }

  for (i = 0; i != DISPLAY_HEIGHT; i++)
    state->scanlineOffsets[i] = 0;

  for (i = 0; i != 40; i++) {
    angle = i * 0x199;
    state->scanlineOffsets[23 + i] = (u32)(Trig_Cos(angle) * 3) >> 15;
    state->scanlineOffsets[110 - i] = (u32)(Trig_Cos(angle) * 3) >> 15;
  }

  state->spins = 0;
  state->state = LUCKY_WHEELS_BETTING;
  state->cursor = 0;
  work->transferMode = CANVAS_COPY_AND_CLEAR;
  work->transferValue = 0;
  REG_BLDCNT = 0;
  window = Window_CreateFar(18, 0, 12, 4, 6);
  state->subWindow = window;
  message = MsgSlotsBet;
  Text_DrawMessageAsTilesFar(message, window, 0, 8);
  Text_DrawMessageAsTilesFar(message - 1, state->subWindow, 0, 0);
}

/// Lucky Wheels: loads its backgrounds, sprites and palettes, fills each of
/// the five wheels with 21 random symbols (with eight placed symbols of rank 0
/// to 5), then runs until the player leaves: fading in, drawing the paylines
/// on the canvas while betting or winning, and the jackpot's sparks and
/// colour cycle while showing a win. Then fades out and frees its memory.
void LuckyWheels_Run(void) {
  s32 picks[8];
  s32 litLines[7];
  DrawRectangle draw[2];
  u8 *sheet;
  void *canvas;
  struct BattleAnimWork *work;
  struct LuckyWheelsWork *state;
  struct CommonWonPrizesView *save;
  u8 *entry;
  u8 *tiles;
  s32 frame;
  u32 count;
  s32 i;
  s32 j;
  s32 k;

  sheet = AllocHeapSlot(HEAP_SLOT_BATTLE_SHEET, 0x60e);
  canvas = AllocHeapSlot(HEAP_SLOT_BATTLE_CANVAS, LUCKY_WHEELS_CANVAS_SIZE);
  work =
      AllocHeapSlotEwram(HEAP_SLOT_BATTLE_ANIM, sizeof(struct BattleAnimWork));
  state = AllocHeapSlotEwram(LUCKY_WHEELS_HEAP_SLOT,
                             sizeof(struct LuckyWheelsWork));
  tiles = Ram_MapGridCellBuffer;
  save = (struct CommonWonPrizesView *)((u8 *)&gCommon + 12);
  Stub_NoOpHook((s32)&ResourceId_BattleAnimFarCalls);
  save->wonPrizes[0] = -1;
  state->repeat = 0;
  state->bet = 1;
  Task_ResetAll();
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
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD),
                      sheet);
  entry = Resource_GetTableEntry((s32)&ResourceId_LuckyWheelsFramePicture);
  Dma_Set(entry, (void *)&BG_PLTT_COLOR(10, 0),
          (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  entry += 32;
  Decompress_TaggedLz(entry, tiles);
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
  REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
  REG_WIN0V = WIN_RANGE(0, DISPLAY_HEIGHT);
  REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
  REG_WIN1V = WIN_RANGE(0, DISPLAY_HEIGHT);

  {
    u16 *shade = (u16 *)(work->sheet + 0x200);
    s32 size = LUCKY_WHEELS_CANVAS_SIZE;

    state->state = LUCKY_WHEELS_BETTING;
    state->cursor = 0;
    state->spins = 0;
    work->frame = 0;
    state->timer = 0;
    Dma_Set(Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet),
            (void *)BG_PLTT, (DMA_ENABLE | DMA_32BIT) << 16 | 32, REG_DMA3);
    // The paylines' two greens, colours 64 and 65 on the canvas.
    BG_PLTT_COLOR(4, 0) = RGB(11, 28, 11);
    BG_PLTT_COLOR(4, 1) = RGB(22, 31, 22);
    entry = Resource_GetTableEntry((s32)&ResourceId_LuckyWheelsIcons);
    Dma_Set(entry, (void *)OBJ_PLTT, (DMA_ENABLE | DMA_32BIT) << 16 | 480 / 4,
            REG_DMA3);
    entry += 480;
    Decompress_TaggedLz(entry, tiles);
    Dma_Set(tiles, (void *)OBJ_VRAM0, (DMA_ENABLE | DMA_32BIT) << 16 | 0x1b30,
            REG_DMA3);
    entry = Resource_GetTableEntry((s32)&ResourceId_LuckyWheelsJackpot);
    Dma_Set(entry, (void *)&OBJ_PLTT_COLOR(15, 0),
            (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
    entry += 32;
    Decompress_TaggedLz(entry, tiles);
    Dma_Set(tiles, (void *)LUCKY_WHEELS_JACKPOT_TILES,
            LUCKY_WHEELS_JACKPOT_COPY, REG_DMA3);
    WindowSystem_InitFieldFar();
    LuckyWheels_InitializeTitle();

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

    BattleAnim_BuildBlitterFar(HEAP_SLOT_RAM_CODE, 8, 7, 3, 2);
    draw[0] = gHeapSlots[HEAP_SLOT_RAM_CODE];
    BattleAnim_BuildBlitterFar(HEAP_SLOT_RAM_CODE_2, 8, 7, 3, 3);
    draw[1] = gHeapSlots[HEAP_SLOT_RAM_CODE_2];
    Iwram_FillWords(canvas, size, 0);
    Dma_Set(canvas, (void *)LUCKY_WHEELS_CANVAS_TILES,
            (DMA_ENABLE | DMA_32BIT) << 16 | LUCKY_WHEELS_CANVAS_SIZE / 4,
            REG_DMA3);
    Dma_Set((void *)BG_PLTT, work, (DMA_ENABLE | DMA_32BIT) << 16 | 0x80,
            REG_DMA3);
    Dma_Set((void *)OBJ_PLTT, shade, (DMA_ENABLE | DMA_32BIT) << 16 | 0x80,
            REG_DMA3);
    LuckyWheels_ScaleRgb555(shade, (u16 *)OBJ_PLTT, 0, 0x100);
    LuckyWheels_ScaleRgb555((u16 *)work, (u16 *)BG_PLTT, 0, 0x100);
    REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
                  DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON;
    if (PartyInventory_CountItemFar(ITEM_GAME_TICKET) == 1) {
      struct Window *window = Window_CreateFar(6, 16, 18, 3, 6);

      state->window = window;
      Text_DrawMessageAsTilesFar(MsgSlotsControls, window, 0, 0);
    } else {
      struct Window *window = Window_CreateFar(
          LUCKY_WHEELS_HELP_WINDOW_X, 16, LUCKY_WHEELS_HELP_WINDOW_WIDTH, 4, 6);
      s32 message;

      state->window = window;
      message = MsgSlotsChangeBet;
      Text_DrawMessageAsTilesFar(message, window, 0, 0);
      Text_DrawMessageAsTilesFar(message + 1, state->window, 0, 8);
    }
    work->transferPending = 0;
    Task_Add(LuckyWheels_Update, TASK_PASS_VBLANK);
    Task_Add(LuckyWheels_TransferCanvas, TASK_PASS_VBLANK);

    for (frame = 0; state->state != LUCKY_WHEELS_DONE; frame++) {
      if (frame <= 16) {
        LuckyWheels_ScaleRgb555(shade, (u16 *)OBJ_PLTT, frame << 12, 0x100);
        LuckyWheels_ScaleRgb555((u16 *)work, (u16 *)BG_PLTT, frame << 12,
                                0x100);
      }
      if (state->state == LUCKY_WHEELS_SHOWING_WIN) {
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
          Palette_DarkenScreenStep();
        if (state->timer > 16 && (frame & 7) == 0) {
          s32 x = (Random16() & 127) + 56;
          s32 y = (Random16() & 31) + 48;
          struct Particle *spark = &LUCKY_WHEELS_SPARKS[(frame / 8 & 3) * 256];

          for (j = 0; j != 256; j++) {
            s32 speed;
            s32 angle;

            speed = 255;
            // Masked in place, the mask assigned first; as Random16() & mask, 3
            // instructions differ.
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
          struct Particle *spark = &LUCKY_WHEELS_SPARKS[j];

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
      if (state->state == LUCKY_WHEELS_BETTING ||
          state->state == LUCKY_WHEELS_WON) {
        for (j = 0; j != 7; j++)
          litLines[j] = 0;
        if (state->state == LUCKY_WHEELS_BETTING) {
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
      WaitFrames(1);
    }

    for (j = 0; j != 17; j++) {
      LuckyWheels_ScaleRgb555(shade, (u16 *)OBJ_PLTT, 0x10000 - (j << 12),
                              0x100);
      LuckyWheels_ScaleRgb555((u16 *)work, (u16 *)BG_PLTT, 0x10000 - (j << 12),
                              0x100);
      WaitFrames(1);
    }
    FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
    FreeHeapSlot(HEAP_SLOT_RAM_CODE);
    Task_Remove(LuckyWheels_TransferCanvas);
    Task_Remove(LuckyWheels_Update);
    FreeHeapSlot(LUCKY_WHEELS_HEAP_SLOT);
    FreeHeapSlot(HEAP_SLOT_BATTLE_CANVAS);
    FreeHeapSlot(HEAP_SLOT_BATTLE_ANIM);
    FreeHeapSlot(HEAP_SLOT_BATTLE_SHEET);
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
    gWaitForFlashWrite->nodes[index].slot = index;
    gWaitForFlashWrite->nodes[index].previous = NULL;
  }
  bucket = gWaitForFlashWrite->bucket;
  for (index = 0xFF; index >= 0; index--) {
    *bucket++ = NULL;
  }
}

/// Adds window position index to the chain of the byte it holds.
void LzPack_InsertSlotNode(s32 index) {
  struct LzPackWork *work = gWaitForFlashWrite;

  work->nodes[index].previous =
      (struct LzPackNode *)&work->bucket[work->bucketBySlot[index]];
  work->nodes[index].next = work->bucket[work->bucketBySlot[index]];
  work->bucket[work->bucketBySlot[index]] = &work->nodes[index];
  if (work->nodes[index].next != NULL)
    work->nodes[index].next->previous = &work->nodes[index];
}

/// Removes window position slot from its chain.
void LzPack_RemoveSlotNode(s32 slot) {
  if (gWaitForFlashWrite->nodes[slot].previous != NULL) {
    if (gWaitForFlashWrite->nodes[slot].next != NULL)
      gWaitForFlashWrite->nodes[slot].next->previous =
          gWaitForFlashWrite->nodes[slot].previous;
    gWaitForFlashWrite->nodes[slot].previous->next =
        gWaitForFlashWrite->nodes[slot].next;
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
      state = gWaitForFlashWrite;
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
    struct LzPackWork **root = &gWaitForFlashWrite;
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
  struct LzPackWork *work = gWaitForFlashWrite;
  s32 index;

  for (index = 0; index != work->outCount; index++) {
    destination[work->outTotal] = work->outBuffer[index];
    work->outTotal++;
  }
}

/// Finds the longest earlier match (up to PACK_MATCH_MAX bytes, at most
/// PACK_DISTANCE_MAX back) for the bytes at window position start; matchLength
/// stays 1 for none.
static __inline__ void LzPack_FindWindowMatch(s32 start) {
  struct LzPackNode *node;
  s32 len;

  gWaitForFlashWrite->matchLength = 1;
  if (gWaitForFlashWrite->bucketBySlot[start] == -1) {
    return;
  }
  node = gWaitForFlashWrite->bucket[gWaitForFlashWrite->bucketBySlot[start]];
  if (start + PACK_MATCH_MAX > PACK_WINDOW_MASK) {
    while (node != NULL) {
      s32 dist = (start - node->slot) & PACK_WINDOW_MASK;

      if (dist > 0 && dist <= PACK_DISTANCE_MAX) {
        for (len = 1; len < PACK_MATCH_MAX; len++) {
          if (gWaitForFlashWrite
                  ->bucketBySlot[(start + len) & PACK_WINDOW_MASK] !=
              gWaitForFlashWrite
                  ->bucketBySlot[(node->slot + len) & PACK_WINDOW_MASK]) {
            break;
          }
        }
        if (gWaitForFlashWrite->matchLength < len) {
          gWaitForFlashWrite->matchOffset = dist;
          gWaitForFlashWrite->matchLength = len;
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
          if (gWaitForFlashWrite->bucketBySlot[start + len] !=
              gWaitForFlashWrite
                  ->bucketBySlot[(node->slot + len) & PACK_WINDOW_MASK]) {
            break;
          }
        }
        if (gWaitForFlashWrite->matchLength < len) {
          gWaitForFlashWrite->matchOffset = dist;
          gWaitForFlashWrite->matchLength = len;
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
  gWaitForFlashWrite = (struct LzPackWork *)gMapGridCellBuffer;
  LzPack_ResetSlotBuckets();
  gWaitForFlashWrite->inputLimit = size;
  gWaitForFlashWrite->position = 0;
  gWaitForFlashWrite->inputCursor = 0;
  gWaitForFlashWrite->outTotal = 0;
  gWaitForFlashWrite->flagMask = 0x80;
  gWaitForFlashWrite->outBuffer[0] = 0;
  gWaitForFlashWrite->outCount = 1;
  LzPack_ConsumeSlotBytes(0, PACK_LOOKAHEAD, input);

  while (gWaitForFlashWrite->bucketBySlot[gWaitForFlashWrite->position] != -1) {
    LzPack_FindWindowMatch(gWaitForFlashWrite->position);
    if (defer == 0) {
      len = gWaitForFlashWrite->matchLength;
      if (gWaitForFlashWrite->matchLength > 1) {
        saveOffset = gWaitForFlashWrite->matchOffset;
        saveLen = len;
        LzPack_FindWindowMatch((gWaitForFlashWrite->position + 1) &
                               PACK_WINDOW_MASK);
        if (gWaitForFlashWrite->matchLength > 2) {
          reach[0] = gWaitForFlashWrite->matchLength + 1;
          LzPack_FindWindowMatch((gWaitForFlashWrite->position + len) &
                                 PACK_WINDOW_MASK);
          reach[1] = gWaitForFlashWrite->matchLength + len;
          if (reach[0] >= reach[1]) {
            saveLen = 1;
            defer = 1;
          }
        }
        gWaitForFlashWrite->matchOffset = saveOffset;
        gWaitForFlashWrite->matchLength = saveLen;
      }
    }

    if (gWaitForFlashWrite->matchLength > 1) {
      defer = 0;
      gWaitForFlashWrite->outBuffer[0] |= gWaitForFlashWrite->flagMask;
      if (gWaitForFlashWrite->matchLength <= 16) {
        code = ((gWaitForFlashWrite->matchOffset << 4) & ~0xfff) |
               (gWaitForFlashWrite->matchOffset & 0xff) |
               (((gWaitForFlashWrite->matchLength - 1) << 8) & 0xf00);
        gWaitForFlashWrite->outBuffer[gWaitForFlashWrite->outCount++] =
            (u16)code >> 8;
        gWaitForFlashWrite->outBuffer[gWaitForFlashWrite->outCount++] = code;
      } else {
        code = ((gWaitForFlashWrite->matchOffset << 4) & ~0xfff) |
               (gWaitForFlashWrite->matchOffset & 0xff);
        gWaitForFlashWrite->outBuffer[gWaitForFlashWrite->outCount++] =
            (u16)code >> 8;
        gWaitForFlashWrite->outBuffer[gWaitForFlashWrite->outCount++] = code;
        gWaitForFlashWrite->outBuffer[gWaitForFlashWrite->outCount++] =
            gWaitForFlashWrite->matchLength - 17;
      }
    } else {
      gWaitForFlashWrite->outBuffer[gWaitForFlashWrite->outCount++] =
          gWaitForFlashWrite->bucketBySlot[gWaitForFlashWrite->position];
      gWaitForFlashWrite->matchLength = 1;
    }

    LzPack_ConsumeSlotBytes(gWaitForFlashWrite->position + PACK_LOOKAHEAD,
                            gWaitForFlashWrite->matchLength, input);
    gWaitForFlashWrite->position =
        (gWaitForFlashWrite->position + gWaitForFlashWrite->matchLength) &
        PACK_WINDOW_MASK;
    gWaitForFlashWrite->flagMask >>= 1;
    if (gWaitForFlashWrite->flagMask == 0) {
      LzPack_CopyBufferedBytes(dst);
      gWaitForFlashWrite->flagMask = 0x80;
      gWaitForFlashWrite->outBuffer[0] = 0;
      gWaitForFlashWrite->outCount = 1;
    }
  }

  gWaitForFlashWrite->outBuffer[0] |= gWaitForFlashWrite->flagMask;
  gWaitForFlashWrite->outBuffer[gWaitForFlashWrite->outCount++] = 0;
  gWaitForFlashWrite->outBuffer[gWaitForFlashWrite->outCount++] = 0;
  LzPack_CopyBufferedBytes(dst);
  return gWaitForFlashWrite->outTotal;
}
