/// @file
/// Lucky Wheels, from LuckyWheels_Start to the LZ packer that ends its code,
/// in the order it has in the ROM.

#include "types.h"
#include "sounds.h"
#include "system/task.h"
#include "system/io_reg.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "system/dma.h"
#include "system/iwram_call.h"
#include "system/ram_buffer.h"
#include "graphics/resource.h"
#include "system/input.h"
#include "system/frame.h"
#include "field/particle.h"
#include "edition.h"
#include "version.h"
#include "tla_edition.h"
#include "fixed_math.h"
#include "system/bg_scroll.h"
#include "field/lucky_wheels_lzpack.h"
#include "field/actor.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/window.h"
#include "field/lucky_wheels_title.h"
#include "field/field_bank.h"
#include "field/field_sequence.h"
#include "system/system_bank.h"
#include "resource_ids.h"
#include "graphics/graphics.h"

void LuckyWheels_Run(void);

/// The paylines' canvas: 256 x 128 pixels of 8-bit colour, shown on BG2 from
/// its tile 212, where the third row of its map begins.
#define LUCKY_WHEELS_CANVAS_SIZE (256 * 128)
#define LUCKY_WHEELS_CANVAS_TILES (BG_CHAR_BLOCK(0) + 0x3500)
/// The machine's five wheels; the cursor's next place, past the last wheel,
/// is the lever.
#define LUCKY_WHEELS_REEL_COUNT 5
#define LUCKY_WHEELS_LEVER LUCKY_WHEELS_REEL_COUNT
/// Pulling the lever: 48 frames down and back up, then 8 settling; only then
/// can the wheels be stopped.
#define LUCKY_WHEELS_LEVER_PULL_FRAMES 48
#define LUCKY_WHEELS_LEVER_SETTLE_FRAMES 8
#define LUCKY_WHEELS_LEVER_FRAMES                                              \
  (LUCKY_WHEELS_LEVER_PULL_FRAMES + LUCKY_WHEELS_LEVER_SETTLE_FRAMES)
/// The lever sprite's place at rest, in screen pixels.
#define LUCKY_WHEELS_LEVER_X 208
#define LUCKY_WHEELS_LEVER_Y 40
/// Left alone, the spinning wheels stop one every 32 frames; R stops one at
/// once.
#define LUCKY_WHEELS_AUTO_STOP_FRAMES 32
/// The pause after a winning spin before the win is shown, and after the last
/// spin loses before the game ends.
#define LUCKY_WHEELS_WIN_PAUSE 60
#define LUCKY_WHEELS_LOSS_PAUSE 45
/// The stop prompt's sound plays this many frames into a spin.
#define LUCKY_WHEELS_STOP_PROMPT_FRAME 16
/// Where each picture of icons.png starts in sprite tiles, in OAM's 32-byte
/// steps (a 256-colour tile is two): the sheet is 32 pixels wide, so a picture
/// starts at the tile numbered as its row. The six symbols are 16 apart from 0.
#define LUCKY_WHEELS_LAMP_TILES 96       ///< a wheel's lamp, not held
#define LUCKY_WHEELS_HELD_LAMP_TILES 128 ///< a held wheel's lamp
#define LUCKY_WHEELS_CORNER_LIGHT_TILES 224
#define LUCKY_WHEELS_FRAME_LIGHT_TILES 232
#define LUCKY_WHEELS_LEVER_TILES 256
#define LUCKY_WHEELS_LINE_TILES 272     ///< a payline's gem, not bet on
#define LUCKY_WHEELS_LIT_LINE_TILES 464 ///< a payline's gem, bet on
/// The slot the lever runs in, drawn in pieces down beside the wheels.
#define LUCKY_WHEELS_LEVER_SLOT_TILES 320
#define LUCKY_WHEELS_HAND_TILES 496  ///< the cursor's hand
#define LUCKY_WHEELS_PULL_TILES 528  ///< a pull's number, the current one large
#define LUCKY_WHEELS_ARROW_TILES 688 ///< the cursor's blinking arrows
/// The symbols round a wheel, each 16 pixels tall: a wheel turns once in 336
/// pixels, 8 a frame, and stops only with a symbol centred on a line.
#define LUCKY_WHEELS_REEL_SYMBOLS 21
#define LUCKY_WHEELS_SYMBOL_HEIGHT 16
#define LUCKY_WHEELS_REEL_TURN                                                 \
  (LUCKY_WHEELS_REEL_SYMBOLS * LUCKY_WHEELS_SYMBOL_HEIGHT)
#define LUCKY_WHEELS_SPIN_SPEED 8
/// The seven paylines: the two diagonals (0 and 6) and the five rows between.
#define LUCKY_WHEELS_PAYLINES 7
#define LUCKY_WHEELS_MIDDLE_LINE 3
#define LUCKY_WHEELS_BET_MAX 4
/// The pulls of the lever after the first, before betting again.
#define LUCKY_WHEELS_RESPINS 4
/// The symbol that matches any other on a payline
/// (gLuckyWheels_SymbolPrizes).
#define LUCKY_WHEELS_WILD_SYMBOL 5
/// The machine fades in and out over 16 frames, a sixteenth of full brightness
/// a frame; the win flashes the screen darker over as many.
#define LUCKY_WHEELS_FADE_FRAMES 16
/// The win cycles the background through five palettes, 16 frames each.
#define LUCKY_WHEELS_CYCLE_FRAMES 16
#define LUCKY_WHEELS_CYCLE_PALETTES 5
/// The jackpot sparks: four bursts of 256 in turn, one every eight frames.
#define LUCKY_WHEELS_BURST_SPARKS 256
#define LUCKY_WHEELS_SPARK_COUNT (4 * LUCKY_WHEELS_BURST_SPARKS)
/// A held direction moves the cursor once, waits 12 frames, then moves it
/// again with 4 frames between each move.
#define LUCKY_WHEELS_REPEAT_DELAY 12
#define LUCKY_WHEELS_REPEAT_INTERVAL 4
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

/// The positions of the machine's lights, in screen pixels: its four corners
/// first, then the lights along its frame.
#define LUCKY_WHEELS_MACHINE_LIGHTS 14
#define LUCKY_WHEELS_CORNER_LIGHTS 4
// The LZ packer: a window of PACK_WINDOW_SIZE bytes, matches of up to
// PACK_MATCH_MAX bytes at most PACK_DISTANCE_MAX back, and PACK_LOOKAHEAD bytes
// read ahead.
#define PACK_WINDOW_SIZE 1024

// Lucky Wheels' tables, the same in both games.

/// Where each spark cell (a square 2 to 20 pixels on a side) starts.
const u16 gLuckyWheels_SparkCellOffsets[] = {0,   4,   20,  56,  120,
                                             220, 364, 560, 816, 1140};

/// The prize of each reel symbol.
const u8 gLuckyWheels_SymbolPrizes[] = {0, 3, 1, 4, 2, 5};

/// The tile of each title letter.
const u8 gLuckyWheels_TitleLetterTiles[REEL_TITLE_LETTERS] = {
#if LANGUAGE == LANGUAGE_JAPANESE
    0, 1, 2, 3, 4,
    2, 5, 6
#elif LANGUAGE == LANGUAGE_SPANISH
    7, 0, 1, 2, 3,
    4, 5, 6, 7
#else
    0, 1, 2, 3, 4,
    5, 6, 7
#endif
};

/// The positions of the machine's lights, in screen pixels: its four corners
/// first, then the lights along its frame.
const u8 gLuckyWheels_MachineSpriteX[LUCKY_WHEELS_MACHINE_LIGHTS] = {
    2, 0, 232, 232, 32, 32, 200, 200, 28, 28, 208, 208, 219, 219};
const u8 gLuckyWheels_MachineSpriteY[LUCKY_WHEELS_MACHINE_LIGHTS] = {
    2, 152, 0, 152, 8, 104, 8, 104, 118, 140, 118, 140, 6, 108};

/// The advance of each title letter.
const u8 gLuckyWheels_TitleLetterWidths[REEL_TITLE_LETTERS] = {
#if LANGUAGE == LANGUAGE_JAPANESE
    32, 24, 22, 28,
    28, 22, 18
#elif LANGUAGE == LANGUAGE_SPANISH
    16, 28, 28, 26, 26,
    28, 28, 20, 24
#else
    28, 28, 26, 26, 28,
    28, 20, 24
#endif
};

/// Plays Lucky Wheels: blanks the screen, seeds the random numbers with the
/// play time and runs the game. Returns 0.
s32 LuckyWheels_Start(void) {
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
  gRandomSeed = gMainRoutineWork.playTime;
  Sound_PlayFar(MUS_LEMURIA_SPRING);
  LuckyWheels_Run();
  return 0;
}

/// Writes count RGB555 colours from src to dst scaled by scale (16.16, Q16_ONE
/// keeps them). Returns 0.
s32 LuckyWheels_ScaleRgb555(u16 *src, u16 *dst, s32 scale, s32 count) {
  s32 remaining;
  u32 color;
  u32 red;
  u32 green;
  u32 blue;

  if (count > 0) {
    u32 redMask = RGB(31, 0, 0);
    u32 greenMask = RGB(0, 31, 0);
    u32 blueMask = RGB(0, 0, 31);

    // Each channel is scaled where it stands, and keeps the top 16 bits.
    remaining = count;
    do {
      color = *src;
      red = color & redMask;
      green = color & greenMask;
      blue = blueMask & color;
      red *= scale;
      green *= scale;
      blue *= scale;
      color = ((red >> 16) & redMask) | ((green >> 16) & greenMask);
      color |= (blue >> 16) & blueMask;
      *dst = color;
      src++;
      dst++;
      remaining--;
    } while (remaining != 0);
  }
  return 0;
}

/// A VBlank-pass task: on a VBlank after the canvas is ready (transferPending),
/// sends the canvas to its tiles in VRAM (LUCKY_WHEELS_CANVAS_TILES), then
/// clears it or fades it into trails as transferMode says.
void LuckyWheels_TransferCanvas(void) {
  struct BattleFxTransferWork *work;
  u8 *buf;

  work = Ram_HeapSlots->battleEffect;
  if (work->transferPending == 1) {
    buf = Ram_HeapSlots->battleCanvas;
    switch (work->transferMode) {
    case 1: // Send it, then fill it with transferValue.
      Dma_Set(buf, (void *)LUCKY_WHEELS_CANVAS_TILES,
              DMA_COPY_WORDS(LUCKY_WHEELS_CANVAS_SIZE / 4), REG_DMA3);
      Iwram_FillWords(buf, LUCKY_WHEELS_CANVAS_SIZE, work->transferValue);
      break;
    case 2: // Afterimage: nonzero colour numbers drop to 1/2 or 3/4.
      if (work->transferValue == 50)
        ColorBuffer_HalveNonzero(buf, (u8 *)LUCKY_WHEELS_CANVAS_TILES,
                                 LUCKY_WHEELS_CANVAS_SIZE);
      else
        ColorBuffer_ScaleNonzeroThreeQuarters(buf, LUCKY_WHEELS_CANVAS_TILES,
                                              LUCKY_WHEELS_CANVAS_SIZE);
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
    s32 b = RGB_BLUE(*col);
    s32 g = RGB_GREEN(*col);
    s32 r = RGB_RED(*col);

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
    s32 b = RGB_BLUE(*col);
    s32 g = RGB_GREEN(*col);
    s32 r = RGB_RED(*col);

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

  Dma_Set(Resource_GetTableEntry(resource), dst, DMA_COPY_WORDS(sizeof dst / 4),
          REG_DMA3);
  for (i = 0; i != 64; i++, col++) {
    s32 r = RGB_RED(*col);
    s32 g = RGB_GREEN(*col);
    s32 b = RGB_BLUE(*col);
    s32 targetRed = RGB_RED(dst[i]);
    s32 targetGreen = RGB_GREEN(dst[i]);
    s32 targetBlue = RGB_BLUE(dst[i]);

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
  s32 frac = 128; // 0.5 in 8-bit fixed point
  u8 *canvas = Ram_HeapSlots->battleCanvas;
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

/// Lucky Wheels' work: the five reels, the game's state, cursor, spins and
/// bet, the keys read, the winning lines, the reel sprites, the windows and
/// the scanline offsets that bend the reels.
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
  u8 unusedAfterSubWindow[8]; ///< no code names it
  u16 scanlineOffsets[160];
#if TLA_PATCH_1
  // The Japanese release has no phase: it closes and reopens its prompt
  // window every frame.
  s32 phase;
#endif
};

extern char MsgSlotsBet[];        ///< "Bet"
extern char MsgSlotsStopReels[];  ///< "{button_icon:4}:Stop slots"
extern char MsgSlotsChooseHeld[]; ///< "When you've chosen which slots"
extern char MsgSlotsPullLever[];  ///< "to lock, pull the lever!"
extern char MsgSlotsHold[];       ///< "{button_icon:1}:Stop"
extern char MsgSlotsRelease[];    ///< "{button_icon:1}:Spin"
extern char MsgSlotsStart[];      ///< "{button_icon:1}:Start machine"
extern char MsgSlotsAllHeld[];    ///< "If you lock all the slots,"
extern char MsgSlotsCannotPull[]; ///< "you can't pull the lever!"

/// Lucky Wheels' VBlank task: reads the keys, runs the game's state
/// (betting, spinning and stopping the wheels, scoring the paylines, choosing
/// wheels to hold, showing a win) and builds all 128 sprites: the cursor, the
/// title letters of a win, the frame, the hold lamps, the spin counter, the
/// paylines, the wheel symbols, the lever and the bottom bar.
void LuckyWheels_Update(void) {
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
  fx = (struct ReelEffectWork *)Ram_HeapSlots->battleEffect;
  objMode = 0x400;
  work->spriteCount = 0;

  Random16();

  keys = gInput.held;
  dpad = gInput.held & KEYS_DPAD;
  work->pressed = keys & ~work->keys;
  work->direction = dpad;
  if ((work->keys & KEYS_DPAD) == work->direction) {
    if (work->repeat > LUCKY_WHEELS_REPEAT_DELAY)
      work->repeat = LUCKY_WHEELS_REPEAT_DELAY;
    if (work->repeat == 0) {
      work->repeat = LUCKY_WHEELS_REPEAT_INTERVAL;
    } else {
      work->repeat--;
      work->direction = 0;
    }
  } else {
    work->repeat = LUCKY_WHEELS_REPEAT_DELAY;
  }
  work->keys = keys;

  if (gDebugPaused == 0) {
    if (work->state == LUCKY_WHEELS_BETTING) {
      tickets = PartyInventory_CountItemFar(228);
      Text_DrawNumberInWindowFar(tickets - work->bet, LUCKY_WHEELS_COIN_DIGITS,
                                 work->subWindow, LUCKY_WHEELS_COIN_X, 0);
      Text_DrawNumberInWindowFar(work->bet, 2, work->subWindow, 64, 8);
      if (work->pressed & KEY_B) {
        struct ReelSave *save = (struct ReelSave *)Data_0200024c;

        work->state = LUCKY_WHEELS_DONE;
        save->wonPrizes[0] = -2;
        Window_CloseFar(work->window, 1);
      } else {
        if (work->pressed & KEY_UP) {
          if (work->bet < LUCKY_WHEELS_BET_MAX && tickets > work->bet) {
            work->bet++;
            Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          } else {
            Sound_PlayFar(SE_MENU_CANCEL);
          }
        }
        if (work->pressed & KEY_DOWN) {
          if (work->bet > 1) {
            work->bet--;
            Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          } else {
            Sound_PlayFar(SE_MENU_CANCEL);
          }
        }
        REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_DARKEN | BLDCNT_TGT2_ALL;
        REG_BLDALPHA = BLEND_ALPHA(16, 0);
        if (work->pressed & KEY_A) {
          work->state = LUCKY_WHEELS_SPINNING;
          fx->frame = 0;
          Window_CloseFar(work->window, 1);
          for (k = 0; k != work->bet; k++)
            PartyInventory_RemoveFar(228);
          Window_CloseFar(work->subWindow, 1);
          Sound_PlayFar(SE_LUCKY_WHEELS_SPIN);
        }
      }
    } else if (work->state == LUCKY_WHEELS_CHOOSING_HOLDS) {
      allHeld = 0;
      work->timer++;
      for (k = 0; k != LUCKY_WHEELS_REEL_COUNT && work->reels[k].held != 0; k++)
        ;
      if (k == LUCKY_WHEELS_REEL_COUNT)
        allHeld = 1;
      if (work->pressed & KEY_A) {
        work->timer = 0;
        fx->frame = 0;
        if (work->spins == LUCKY_WHEELS_RESPINS) {
          work->spins = 0;
          work->cursor = 0;
          work->state = LUCKY_WHEELS_BETTING;
          for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
            work->reels[i].held = 0;
            work->reels[i].stop = -1;
          }
        } else if (work->cursor < LUCKY_WHEELS_LEVER) {
          Sound_PlayFar(SE_LUCKY_WHEELS_HOLD);
          work->reels[work->cursor].held ^= 1;
        } else if (allHeld == 0) {
          Sound_PlayFar(SE_LUCKY_WHEELS_SPIN);
          work->state = LUCKY_WHEELS_SPINNING;
          work->cursor = 0;
          for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++)
            work->reels[i].stop = -1;
          work->spins++;
        } else {
          Sound_PlayFar(SE_MENU_CANCEL);
        }
      } else {
        if (work->direction & KEY_RIGHT) {
          work->cursor = (work->cursor + 1) % (LUCKY_WHEELS_LEVER + 1);
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        }
        if (work->direction & KEY_LEFT) {
          work->cursor = (work->cursor + 5) % 6;
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        }
      }

      if (work->state == LUCKY_WHEELS_CHOOSING_HOLDS) {
#if TLA_PATCH_1
        if (work->cursor == LUCKY_WHEELS_LEVER) {
          if (allHeld != 0) {
            if (work->phase != 1 && work->phase != 2) {
              Window_CloseFar(work->window, 1);
              work->window =
                  Window_CreateFar(LUCKY_WHEELS_ALL_HELD_X, 0,
                                   LUCKY_WHEELS_ALL_HELD_WIDTH, 4, 6);
              Text_DrawMessageAsTilesFar((s32)MsgSlotsAllHeld, work->window, 0,
                                         0);
              work->phase = 1;
            } else if (work->phase == 1) {
              Text_DrawMessageAsTilesFar((s32)MsgSlotsCannotPull, work->window,
                                         0, 8);
              work->phase = 2;
            }
          } else {
            if (work->phase != 3) {
              Window_CloseFar(work->window, 1);
              work->window = Window_CreateFar(LUCKY_WHEELS_START_X, 0,
                                              LUCKY_WHEELS_START_WIDTH, 3, 6);
              Text_DrawMessageAsTilesFar((s32)MsgSlotsStart, work->window, 0,
                                         0);
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
            Text_DrawMessageAsTilesFar((s32)MsgSlotsRelease, work->window, 0,
                                       0);
          }
          work->phase = 5;
        }
#else
        Window_CloseFar(work->window, 1);
        if (work->cursor == LUCKY_WHEELS_LEVER) {
          if (allHeld != 0) {
            work->window = Window_CreateFar(LUCKY_WHEELS_ALL_HELD_X, 0,
                                            LUCKY_WHEELS_ALL_HELD_WIDTH, 3, 6);
            Text_DrawMessageAsTilesFar((s32)MsgSlotsAllHeld, work->window, 0,
                                       0);
          } else {
            work->window = Window_CreateFar(LUCKY_WHEELS_START_X, 0,
                                            LUCKY_WHEELS_START_WIDTH, 3, 6);
            Text_DrawMessageAsTilesFar((s32)MsgSlotsStart, work->window, 0, 0);
          }
        } else {
          work->window = Window_CreateFar(LUCKY_WHEELS_HOLD_X, 0,
                                          LUCKY_WHEELS_HOLD_WIDTH, 3, 6);
          if (work->reels[work->cursor].held == 0)
            Text_DrawMessageAsTilesFar(MsgSlotsHold, work->window, 0, 0);
          else
            Text_DrawMessageAsTilesFar((s32)MsgSlotsRelease, work->window, 0,
                                       0);
        }
#endif
      } else {
        Window_CloseFar(work->window, 1);
      }
    } else if (work->state == LUCKY_WHEELS_WON) {
      work->timer++;
      objMode = 0;
      if (work->timer == LUCKY_WHEELS_WIN_PAUSE) {
        work->state = LUCKY_WHEELS_SHOWING_WIN;
        Sound_PlayFar(MUS_JINGLE_LARGE);
        work->timer = 0;
        REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        REG_BLDALPHA = BLEND_ALPHA(16, 16);
        fx->transferMode = 2;
        fx->transferValue = 75;
      }
    } else if (work->state == LUCKY_WHEELS_SHOWING_WIN) {
      work->timer++;
      objMode = 0;
      if (work->pressed & KEY_A) {
        work->state = LUCKY_WHEELS_DONE;
        Sound_PlayFar(SE_MENU_CONFIRM);
      }
    } else if (work->state == LUCKY_WHEELS_NO_WIN) {
#if TLA_PATCH_1
      if (work->phase == 0) {
        work->phase = 1;
        Text_DrawMessageAsTilesFar(MsgSlotsPullLever, work->window, 0, 8);
      }
#endif
      if (work->pressed & KEY_A) {
        work->state = LUCKY_WHEELS_CHOOSING_HOLDS;
#if TLA_PATCH_1
        work->phase = 0;
#endif
        Sound_PlayFar(SE_MENU_CONFIRM);
        Window_CloseFar(work->window, 1);
      }
    } else if (work->state == LUCKY_WHEELS_OUT_OF_SPINS) {
      work->timer++;
      if (work->timer == LUCKY_WHEELS_LOSS_PAUSE)
        work->state = LUCKY_WHEELS_DONE;
    } else if (work->state != LUCKY_WHEELS_DONE) {
      if (work->timer == 4) {
        work->window = Window_CreateFar(LUCKY_WHEELS_STOP_X, 17,
                                        LUCKY_WHEELS_STOP_WIDTH, 3, 6);
        Text_DrawMessageAsTilesFar(MsgSlotsStopReels, work->window, 0, 0);
      }
      if (work->timer == LUCKY_WHEELS_STOP_PROMPT_FRAME)
        Sound_PlayFar(SE_LUCKY_WHEELS_STOP_PROMPT);
      if (work->timer > LUCKY_WHEELS_LEVER_FRAMES) {
        if (fx->frame >= LUCKY_WHEELS_AUTO_STOP_FRAMES ||
            (work->pressed & KEY_R)) {
          fx->frame = 0;
          for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
            if (work->reels[i].held == 0 && work->reels[i].stop == -1) {
              work->reels[i].stop = (Random16() & 3) + 4;
              Sound_PlayFar(SE_LUCKY_WHEELS_STOP);
              break;
            }
          }
        }
      }

      for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
        if (work->reels[i].stop > 0)
          work->reels[i].stop--;
      }

      stoppedReels = 0;
      for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
        if (work->reels[i].held == 1)
          stoppedReels++;
        else if (work->reels[i].stop == 0 &&
                 (work->reels[i].position & 15) == 8)
          stoppedReels++;
      }

      if (stoppedReels == 5) {
        struct ReelSave *save = (struct ReelSave *)Data_0200024c;
        s32 hits = 0;

        for (col = 0; col != LUCKY_WHEELS_PAYLINES; col++) {
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
          Sound_PlayFar(SE_THUNDER);
          fx->transferMode = 1;
          fx->transferValue = 0;
          REG_BLDCNT = 0;
          Window_CloseFar(work->window, 1);
        } else {
          work->state = LUCKY_WHEELS_NO_WIN;
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
            Text_DrawMessageAsTilesFar(message, work->window, 0, 0);
            Text_DrawMessageAsTilesFar(message + 1, work->window, 0, 8);
          }
          if (work->spins == 4) {
            s32 left = PartyInventory_CountItemFar(228);
            struct Window *window;
            s32 message;

            // DEAD: both arms set the out-of-spins state, so the ticket count
            // cannot change it; the ROM keeps the compare and both stores.
            if (left > 0)
              work->state = LUCKY_WHEELS_OUT_OF_SPINS;
            else
              work->state = LUCKY_WHEELS_OUT_OF_SPINS;
            if (work->bet > left)
              work->bet = left;
            work->spins = 0;
            work->cursor = 0;
            for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
              work->reels[i].held = 0;
              work->reels[i].stop = -1;
            }
            fx->transferMode = 1;
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
        for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
          struct Reel *reel = &work->reels[i];

          if (reel->held == 0) {
            if (reel->stop != 0)
              reel->position += LUCKY_WHEELS_SPIN_SPEED;
            else if ((reel->position & 15) != 8)
              reel->position += LUCKY_WHEELS_SPIN_SPEED;
            if (reel->position == LUCKY_WHEELS_REEL_TURN)
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
    if (work->cursor == LUCKY_WHEELS_LEVER) {
      x = LUCKY_WHEELS_LEVER_X;
      y = 32;
    }
    work->obj[work->spriteCount].attr01 =
        OBJ_X(x - 12) | objMode | (y + 8) | 0x80006000;
    work->obj[work->spriteCount].attr2 = (v << 4) + LUCKY_WHEELS_ARROW_TILES;
    work->spriteCount++;
    work->obj[work->spriteCount].attr01 =
        OBJ_X(x + 12) | objMode | (y + 8) | 0x90006000;
    work->obj[work->spriteCount].attr2 = (v << 4) + LUCKY_WHEELS_ARROW_TILES;
    work->spriteCount++;
    work->obj[work->spriteCount].attr01 = OBJ_X(x) | objMode | y | 0x80002000;
    work->obj[work->spriteCount].attr2 = LUCKY_WHEELS_HAND_TILES;
    work->spriteCount++;
  }

  if (work->state == LUCKY_WHEELS_SHOWING_WIN) {
    for (i = 0; i != REEL_TITLE_LETTERS; i++) {
      struct Particle *p = &fx->particles[i];

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
      p->velocityY += px(1) / 4;
      if (work->timer % 256 == i * 4 + 200) {
        p->velocityY = px(6);
        p->variant = 0;
      }
      if (p->y > px(64)) {
        p->y = px(64);
        if (p->variant <= 1)
          p->velocityY = -p->velocityY / 2;
        p->variant++;
      }
#endif
      work->spriteCount++;
    }
  }

  for (k = 0; k != LUCKY_WHEELS_MACHINE_LIGHTS; k++) {
    work->obj[work->spriteCount].attr01 =
        OBJ_X(gLuckyWheels_MachineSpriteX[k]) | objMode |
        gLuckyWheels_MachineSpriteY[k] | 0x80006000;
    if (k < LUCKY_WHEELS_CORNER_LIGHTS)
      work->obj[work->spriteCount].attr2 =
          OBJ_PRIORITY(1) | LUCKY_WHEELS_CORNER_LIGHT_TILES;
    else
      work->obj[work->spriteCount].attr2 =
          OBJ_PRIORITY(1) | LUCKY_WHEELS_FRAME_LIGHT_TILES;
    work->spriteCount++;
  }

  for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
    if (work->reels[i].held == 0) {
      work->obj[work->spriteCount].attr01 = objMode | ((i * 36 + 32) << 16) |
                                            124 | OBJ_SIZE_32X32 |
                                            OBJ_256_COLORS;
      work->obj[work->spriteCount].attr2 =
          OBJ_PRIORITY(1) | LUCKY_WHEELS_LAMP_TILES;
    } else {
      work->obj[work->spriteCount].attr01 = objMode | ((i * 36 + 32) << 16) |
                                            124 | OBJ_SIZE_32X32 |
                                            OBJ_256_COLORS;
      work->obj[work->spriteCount].attr2 =
          OBJ_PRIORITY(1) | LUCKY_WHEELS_HELD_LAMP_TILES;
    }
    work->spriteCount++;
  }

  for (i = 0; i != 5; i++) {
    work->obj[work->spriteCount].attr01 =
        objMode | ((i * 16 + 32) << 16) | 3 | OBJ_SIZE_32X16 | OBJ_256_COLORS;
    if (i == work->spins)
      work->obj[work->spriteCount].attr2 =
          (i * 32 + LUCKY_WHEELS_PULL_TILES) | OBJ_PRIORITY(1);
    else
      work->obj[work->spriteCount].attr2 =
          (i * 32 + LUCKY_WHEELS_PULL_TILES + 16) | OBJ_PRIORITY(1);
    work->spriteCount++;
  }

  for (col = 0; col != LUCKY_WHEELS_PAYLINES; col++) {
    work->obj[work->spriteCount].attr01 =
        (((0x204 - ((col & 1) << 3)) & 0x1ff) << 16) | objMode |
        (col * 16 + 5) | 0x80002000;
    if (col > 3 - work->bet && col < work->bet + 3)
      work->obj[work->spriteCount].attr2 =
          OBJ_PRIORITY(1) | LUCKY_WHEELS_LIT_LINE_TILES;
    else
      work->obj[work->spriteCount].attr2 =
          OBJ_PRIORITY(1) | LUCKY_WHEELS_LINE_TILES;
    work->spriteCount++;
  }

  for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
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

  lever = LUCKY_WHEELS_LEVER_Y;
  if (work->state == LUCKY_WHEELS_SPINNING) {
    if (work->timer < LUCKY_WHEELS_LEVER_PULL_FRAMES)
      lever = ((Trig_Sin(work->timer * 682) << 6) >> 16) + LUCKY_WHEELS_LEVER_Y;
    else if (work->timer < LUCKY_WHEELS_LEVER_FRAMES)
      lever = ((Trig_Sin((work->timer << 12) - TRIG_TURN * 3) << 2) >> 16) +
              LUCKY_WHEELS_LEVER_Y;
  }
  work->obj[work->spriteCount].attr01 = lever | objMode | 0x80d06000;
  work->obj[work->spriteCount].attr2 =
      OBJ_PRIORITY(1) | LUCKY_WHEELS_LEVER_TILES;
  work->spriteCount++;

  for (k = 0; k != 8; k++) {
    work->obj[work->spriteCount].attr01 = objMode | (k * 16 + 12) | 0x80ce6000;
    if (k == 0)
      work->obj[work->spriteCount].attr2 =
          OBJ_PRIORITY(1) | LUCKY_WHEELS_LEVER_SLOT_TILES;
    else if (k == 1)
      work->obj[work->spriteCount].attr2 =
          OBJ_PRIORITY(1) | (LUCKY_WHEELS_LEVER_SLOT_TILES + 16);
    else if (k == 6)
      work->obj[work->spriteCount].attr2 =
          OBJ_PRIORITY(1) | (LUCKY_WHEELS_LEVER_SLOT_TILES + 48);
    else if (k == 7)
      work->obj[work->spriteCount].attr2 =
          OBJ_PRIORITY(1) | (LUCKY_WHEELS_LEVER_SLOT_TILES + 64);
    else
      work->obj[work->spriteCount].attr2 =
          OBJ_PRIORITY(1) | (LUCKY_WHEELS_LEVER_SLOT_TILES + 32);
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
// Split out of what The Broken Seal keeps in LuckyWheels_Update.
void LuckyWheels_FinishFrame(void) {
  struct ReelWork *work;
  struct ReelEffectWork *fx;
  volatile u16 *dma;
  s32 i;

  work = (struct ReelWork *)Ram_HeapSlots->canvasMap;
  fx = (struct ReelEffectWork *)Ram_HeapSlots->battleEffect;
  if (work->state == 3) {
    for (i = 0; i != 8; i++) {
      struct Particle *p = &fx->particles[i];

      p->y += p->velocityY;
      p->velocityY += px(1) / 4;
      if (work->timer % 256 == i * 4 + 200) {
        p->velocityY = px(6);
        p->variant = 0;
      }
      if (p->y > px(64)) {
        p->y = px(64);
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

// The title's letters and the left edge of the first: the Spanish title has
// nine letters and starts further left.
#if (LANGUAGE == LANGUAGE_SPANISH)
#define REEL_TITLE_LETTERS 9
#define REEL_TITLE_LEFT 8
#else
#define REEL_TITLE_LETTERS 8
#define REEL_TITLE_LEFT 24
#endif

#define SPARKS ((struct Particle *)Ram_MapCellBuffer)

/// Sets up the bouncing title letters of a win, as The Broken Seal does, on
/// this game's heap slots (the reel work at 0xb4, the effect work at 0x5c with
/// its particles first).
void LuckyWheels_InitializeTitle(void) {
  struct ReelWork *state;
  struct ReelEffectWork *work;
  struct Particle *letter;
  s32 left;
  s32 i;
  s32 angle;
  struct Window *window;
  s32 message;

  state = (struct ReelWork *)Ram_HeapSlots->canvasMap;
  work = (struct ReelEffectWork *)Ram_HeapSlots->battleEffect;
  left = 0;
  for (i = 0; i != 2048; i++)
    SPARKS[i].variant = 0;

  letter = work->particles;
  for (i = 0; i != REEL_TITLE_LETTERS; i++) {
    letter[i].x = px(left + REEL_TITLE_LEFT);
    left += gLuckyWheels_TitleLetterWidths[i];
    letter[i].y = (-32 - i * 8) << 16;
    letter[i].velocityY = 0;
    letter[i].variant = 0;
  }

  for (i = 0; i != DISPLAY_HEIGHT; i++)
    state->scanlineOffsets[i] = 0;

  for (i = 0; i != 40; i++) {
    angle = i * 409;
    state->scanlineOffsets[23 + i] = (u32)(Trig_Cos(angle) * 3) >> 15;
    state->scanlineOffsets[110 - i] = (u32)(Trig_Cos(angle) * 3) >> 15;
  }

  state->spins = 0;
  state->state = LUCKY_WHEELS_BETTING;
  state->cursor = 0;
  work->transferMode = 1;
  work->transferValue = 0;
  REG_BLDCNT = 0;
  window = Window_CreateFar(18, 0, 12, 4, 6);
  state->subWindow = window;
  message = MsgSlotsBet;
  Text_DrawMessageAsTilesFar(message, window, 0, 8);
  Text_DrawMessageAsTilesFar(message - 1, state->subWindow, 0, 0);
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

extern char MsgSlotsChangeBet[]; ///< "Press up or down to change value"
extern char MsgSlotsControls[];  ///< "{button_icon:1}:Start machine
                                 ///< {button_icon:2}:Quit"

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

  sheet = HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, 0x60e);
  canvas = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, 0x8000);
  work = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_EFFECT, 0x7b7c);
  state = HeapSlot_AllocEwram(HEAP_SLOT_CANVAS_MAP, sizeof(struct ReelWork));
  tiles = Ram_MapCellBuffer;
  save = (struct ReelSave *)Data_0200024c;
  Resource_ReservedNoOp((s32)&ResourceId_BattleEffectFarCalls);
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
  entry = Resource_GetTableEntry((s32)&ResourceId_SlotsMachinePicture);
  Dma_Set(entry, (void *)&BG_PLTT_COLOR(10, 0), DMA_COPY_WORDS(8), REG_DMA3);
  entry += 32;
  Decompress_TaggedLz(entry, tiles);
  j = 0;
  for (k = 0; k != 20; k++) {
    for (i = 0; i != 30; i++) {
      if (!(i >= 5 && i <= 24 && k > 2 && k <= 13)) {
        Dma_Set(tiles + (k * 30 + i) * 32, (void *)(0x0600b500 + j),
                DMA_COPY_WORDS(8), REG_DMA3);
        j += TILE_SIZE_4BPP;
      }
    }
  }
  Iwram_ClearWords(BG_SCREEN_BLOCK(5) + 0x500, 768);
  j = 0;
  for (k = 0; k != 20; k++) {
    for (i = 0; i != 32; i++, j += 2) {
      if (k < 2 || k > 16)
        *(volatile u16 *)(BG_SCREEN_BLOCK(6) + j) = 191;
      else
        *(volatile u16 *)(0x06003000 + j) = k * 32 + i + 148;
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
  REG_BG1VOFS = -DISPLAY_HEIGHT;
  REG_BG2VOFS = -DISPLAY_HEIGHT;
  REG_WIN0H = WIN_RANGE(40, 200);
  REG_WIN0V = WIN_RANGE(24, 120);
  REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
  REG_WIN0V = WIN_RANGE(0, DISPLAY_HEIGHT);
  REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
  REG_WIN1V = WIN_RANGE(0, DISPLAY_HEIGHT);

  {
    u16 *bgShade = work->bgShade;
    u16 *shade = work->objShade;
    s32 size = 0x8000;

    state->state = LUCKY_WHEELS_BETTING;
    state->cursor = 0;
    state->spins = 0;
    work->frame = 0;
    state->timer = 0;
    Dma_Set(Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet),
            (void *)BG_PLTT, DMA_COPY_WORDS(32), REG_DMA3);
    *(u16 *)&BG_PLTT_COLOR(4, 0) = 0x2f8b;
    *(u16 *)(BG_PLTT + 65) = 0x5bf6;
    entry = Resource_GetTableEntry((s32)&ResourceId_SlotsIcons);
    Dma_Set(entry, (void *)OBJ_PLTT, DMA_COPY_WORDS(120), REG_DMA3);
    entry += 480;
    Decompress_TaggedLz(entry, tiles);
    Dma_Set(tiles, (void *)OBJ_VRAM0, DMA_COPY_WORDS(6960), REG_DMA3);
    entry = Resource_GetTableEntry((s32)&ResourceId_SlotsJackpotBanner);
    Dma_Set(entry, (void *)&OBJ_PLTT_COLOR(15, 0), DMA_COPY_WORDS(8), REG_DMA3);
    entry += 32;
    Decompress_TaggedLz(entry, tiles);
    Dma_Set(tiles, (void *)JACKPOT_TILES, JACKPOT_COPY, REG_DMA3);
    WindowSystem_InitFieldFar();
    LuckyWheels_InitializeTitle();

    for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
      struct Reel *reel = &state->reels[i];

      reel->position = LUCKY_WHEELS_SYMBOL_HEIGHT / 2;
      reel->held = 0;
      reel->stop = -1;
      for (k = 0; k != 21; k++)
        reel->symbols[k] = Random16() % 5;
    }
    for (i = 0; i != 5; i++) {
      struct Reel *reel = &state->reels[i];

      for (j = 0; j != 8; j++) {
        picks[j] = Random16() % LUCKY_WHEELS_REEL_SYMBOLS;
        for (k = 0; k != j; k++) {
          if (picks[j] == picks[k]) {
            j--;
            break;
          }
        }
      }
      for (k = 0; k != 8; k++) {
        s32 rank = k;

        if (rank > LUCKY_WHEELS_WILD_SYMBOL)
          rank = LUCKY_WHEELS_WILD_SYMBOL;
        reel->symbols[picks[k]] = rank;
      }
    }

    BattleEffect_BuildBlitterFar(HEAP_SLOT_RAM_CODE, 8, 7, 3, 1);
    draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
    BattleEffect_BuildBlitterFar(HEAP_SLOT_RAM_CODE_PAIRED, 8, 7, 3, 0);
    draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
    Iwram_FillWordsEntry(canvas, size, 0);
    Dma_Set(canvas, (void *)LUCKY_WHEELS_CANVAS_TILES, DMA_COPY_WORDS(8192),
            REG_DMA3);
    Dma_Set((void *)BG_PLTT, bgShade, DMA_COPY_WORDS(128), REG_DMA3);
    Dma_Set((void *)OBJ_PLTT, shade, DMA_COPY_WORDS(128), REG_DMA3);
    LuckyWheels_ScaleRgb555(shade, (u16 *)OBJ_PLTT, 0, 0x100);
    LuckyWheels_ScaleRgb555(bgShade, (u16 *)BG_PLTT, 0, 0x100);
    REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
                  DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON;
    if (PartyInventory_CountItemFar(228) == 1) {
      struct Window *window = Window_CreateFar(
          HELP_WINDOW_X, 16, HELP_WINDOW_WIDTH, CONTROLS_WINDOW_HEIGHT, 6);

      state->window = window;
      Text_DrawMessageAsTilesFar(MsgSlotsControls, window, 0, 0);
    } else {
      struct Window *window =
          Window_CreateFar(HELP_WINDOW_X, 16, HELP_WINDOW_WIDTH, 4, 6);
      s32 message;

      state->window = window;
      message = MsgSlotsChangeBet;
      Text_DrawMessageAsTilesFar(message, window, 0, 0);
      Text_DrawMessageAsTilesFar(message + 1, state->window, 0, 8);
    }
    work->transferPending = 0;
    Task_Add(LuckyWheels_Update, TASK_PASS_VBLANK);
    Task_Add(LuckyWheels_FinishFrame, TASK_PASS_FRAME);
    Task_Add(LuckyWheels_TransferCanvas, TASK_PASS_FRAME);

    for (frame = 0; state->state != 10; frame++) {
      if (frame <= 16) {
        LuckyWheels_ScaleRgb555(shade, (u16 *)OBJ_PLTT, frame << 12, 0x100);
        LuckyWheels_ScaleRgb555(bgShade, (u16 *)BG_PLTT, frame << 12, 0x100);
      }
      if (state->state == LUCKY_WHEELS_SHOWING_WIN) {
        s32 phase =
            frame % (LUCKY_WHEELS_CYCLE_PALETTES * LUCKY_WHEELS_CYCLE_FRAMES);

        if (phase < LUCKY_WHEELS_CYCLE_FRAMES)
          Palette_StepTowardResource((s32)&ResourceId_MercuryDjinnSheet);
        else if (phase < LUCKY_WHEELS_CYCLE_FRAMES * 2)
          Palette_StepTowardResource((s32)&ResourceId_VenusDjinnSheet);
        else if (phase < LUCKY_WHEELS_CYCLE_FRAMES * 3)
          Palette_StepTowardResource((s32)&ResourceId_EmberStreakSheet);
        else if (phase < LUCKY_WHEELS_CYCLE_FRAMES * 4)
          Palette_StepTowardResource((s32)&ResourceId_LimePalette);
        else
          Palette_StepTowardResource((s32)&ResourceId_JupiterDjinnSheet);
        if (state->timer < LUCKY_WHEELS_FADE_FRAMES)
          Palette_DarkenScreenStep();
        if (state->timer > LUCKY_WHEELS_FADE_FRAMES && (frame & 7) == 0) {
          s32 x = (Random16() & 127) + 56;
          s32 y = (Random16() & 31) + 48;
          struct Particle *spark = &SPARKS[(frame / 8 & 3) * 256];

          for (j = 0; j != LUCKY_WHEELS_BURST_SPARKS; j++) {
            s32 speed;
            s32 angle;

            speed = 255;
            speed &= Random16();
            angle = Random16() & 0xffff;
            spark->x = px(x);
            spark->y = px(y);
            spark->velocityX = Trig_Sin(angle) * (speed + 64) >> 6;
            spark->velocityY = -(Trig_Cos(angle) * (speed + 64)) >> 6;
            spark->variant = (Random16() & 15) + 16;
            spark++;
          }
        }
        for (j = 0; j != LUCKY_WHEELS_SPARK_COUNT; j++) {
          struct Particle *spark = &SPARKS[j];

          if (spark->variant > 0) {
            s32 x;

            spark->variant--;
            x = spark->x;
            if (x >= 0 && x <= 0xffffff) {
              s32 y = spark->y;

              if (y <= 0x7fffff) {
                if (y >= 0) {
                  s32 px = FIELD_PIXEL(x);
                  s32 py = FIELD_PIXEL(y);
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
        if (state->state == LUCKY_WHEELS_BETTING) {
          litLines[LUCKY_WHEELS_MIDDLE_LINE] = 1;
          if (state->bet > 1) {
            litLines[LUCKY_WHEELS_MIDDLE_LINE + 1] = 1;
            litLines[LUCKY_WHEELS_MIDDLE_LINE - 1] = 1;
          }
          if (state->bet > 2) {
            litLines[LUCKY_WHEELS_MIDDLE_LINE + 2] = 1;
            litLines[LUCKY_WHEELS_MIDDLE_LINE - 2] = 1;
          }
          if (state->bet > 3) {
            litLines[LUCKY_WHEELS_MIDDLE_LINE + 3] = 1;
            litLines[LUCKY_WHEELS_MIDDLE_LINE - 3] = 1;
          }
        } else if ((frame & 7) <= 3) {
          for (j = 0; j != LUCKY_WHEELS_PAYLINES; j++)
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
      LuckyWheels_ScaleRgb555(shade, (u16 *)OBJ_PLTT, Q16_ONE - (j << 12),
                              0x100);
      LuckyWheels_ScaleRgb555(bgShade, (u16 *)BG_PLTT, Q16_ONE - (j << 12),
                              0x100);
      wait(1);
    }
    HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    Task_Remove(LuckyWheels_TransferCanvas);
    Task_Remove(LuckyWheels_Update);
    Task_Remove(LuckyWheels_FinishFrame);
    HeapSlot_Release(HEAP_SLOT_CANVAS_MAP);
    HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
    HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
    HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
  }
}

/// The LZ packer below: empties its 256 hash chains (one per byte value) and
/// numbers its window nodes.
void LzPack_ResetSlotBuckets(void) {
  s32 index;
  struct LzPackNode **bucket;

  for (index = 0; index < PACK_WINDOW_SIZE; index++) {
    gLzPackWork->nodes[index].slot = index;
    gLzPackWork->nodes[index].previous = NULL;
  }
  bucket = gLzPackWork->bucket;
  for (index = 0; index < 256; index++) {
    *bucket++ = NULL;
  }
}

/// Adds window position index to the chain of the byte it holds.
void LzPack_InsertSlotNode(s32 index) {
  struct LzPackWork *work = gLzPackWork;

  work->nodes[index].previous =
      (struct LzPackNode *)&work->bucket[work->bucketBySlot[index]];
  work->nodes[index].next = work->bucket[work->bucketBySlot[index]];
  work->bucket[work->bucketBySlot[index]] = &work->nodes[index];
  if (work->nodes[index].next != NULL)
    work->nodes[index].next->previous = &work->nodes[index];
}

/// Removes window position slot from its chain.
void LzPack_RemoveSlotNode(s32 slot) {
  if (gLzPackWork->nodes[slot].previous != NULL) {
    if (gLzPackWork->nodes[slot].next != NULL)
      gLzPackWork->nodes[slot].next->previous =
          gLzPackWork->nodes[slot].previous;
    gLzPackWork->nodes[slot].previous->next = gLzPackWork->nodes[slot].next;
  }
}

/// Reads count more input bytes into the window from position start, dropping
/// the bytes they replace from their chains; past the end of the input the
/// positions are marked empty (-1).
void LzPack_ConsumeSlotBytes(s32 start, s32 count, const u8 *input) {
  s32 limit = count;
  s32 current = 0;
  u32 base = start;

  if (current < count) {
    u32 mask = 0x3ff;
    u32 removal = start + 292;

    do {
      u32 slot = start + current;
      struct LzPackWork *state;
      u32 readOffset;
      u32 nextOffset;
      u8 value;

      LzPack_RemoveSlotNode(removal & mask);
      state = gLzPackWork;
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
    } while (current < count);
  }

  current++;
  if (current < limit) {
    u32 mask = 0x3ff;
    struct LzPackWork **root = &gLzPackWork;
    s32 empty = -1;

    do {
      u32 slot = (start + current) & mask;
      struct LzPackWork *state;

      LzPack_RemoveSlotNode(slot);
      state = gLzPackWork;
      current++;
      state->bucketBySlot[slot] = (-1);
    } while (current < count);
  }
}

/// Appends the finished group (a flag byte and up to eight items) to the
/// output.
void LzPack_CopyBufferedBytes(u8 *destination) {
  struct LzPackWork *work = gLzPackWork;
  s32 index;

  for (index = 0; index != work->outCount; index++) {
    destination[work->outTotal] = work->outBuffer[index];
    work->outTotal++;
  }
}

#define PACK_WINDOW_MASK 0x3ff
#define PACK_MATCH_MAX 272
#define PACK_DISTANCE_MAX 63
#define PACK_LOOKAHEAD 672

/// Finds the longest earlier match (up to PACK_MATCH_MAX bytes, at most
/// PACK_DISTANCE_MAX back) for the bytes at window position start; matchLength
/// stays 1 for none.
static __inline__ void LzPack_FindWindowMatch(s32 start) {
  struct LzPackNode *node;
  s32 len;

  gLzPackWork->matchLength = 1;
  if (gLzPackWork->bucketBySlot[start] == -1) {
    return;
  }
  node = gLzPackWork->bucket[gLzPackWork->bucketBySlot[start]];
  if (start + PACK_MATCH_MAX > PACK_WINDOW_MASK) {
    while (node != NULL) {
      s32 dist = (start - node->slot) & PACK_WINDOW_MASK;

      if (dist > 0 && dist <= PACK_DISTANCE_MAX) {
        for (len = 1; len < PACK_MATCH_MAX; len++) {
          if (gLzPackWork->bucketBySlot[(start + len) & PACK_WINDOW_MASK] !=
              gLzPackWork
                  ->bucketBySlot[(node->slot + len) & PACK_WINDOW_MASK]) {
            break;
          }
        }
        if (gLzPackWork->matchLength < len) {
          gLzPackWork->matchOffset = dist;
          gLzPackWork->matchLength = len;
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
          if (gLzPackWork->bucketBySlot[start + len] !=
              gLzPackWork
                  ->bucketBySlot[(node->slot + len) & PACK_WINDOW_MASK]) {
            break;
          }
        }
        if (gLzPackWork->matchLength < len) {
          gLzPackWork->matchOffset = dist;
          gLzPackWork->matchLength = len;
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
  gLzPackWork = (struct LzPackWork *)Ram_MapCellBuffer;
  LzPack_ResetSlotBuckets();
  gLzPackWork->inputLimit = size;
  gLzPackWork->position = 0;
  gLzPackWork->inputCursor = 0;
  gLzPackWork->outTotal = 0;
  gLzPackWork->flagMask = 0x80;
  gLzPackWork->outBuffer[0] = 0;
  gLzPackWork->outCount = 1;
  LzPack_ConsumeSlotBytes(0, PACK_LOOKAHEAD, input);

  while (gLzPackWork->bucketBySlot[gLzPackWork->position] != -1) {
    LzPack_FindWindowMatch(gLzPackWork->position);
    if (defer == 0) {
      len = gLzPackWork->matchLength;
      if (gLzPackWork->matchLength > 1) {
        saveOffset = gLzPackWork->matchOffset;
        saveLen = len;
        LzPack_FindWindowMatch((gLzPackWork->position + 1) & PACK_WINDOW_MASK);
        if (gLzPackWork->matchLength > 2) {
          reach[0] = gLzPackWork->matchLength + 1;
          LzPack_FindWindowMatch((gLzPackWork->position + len) &
                                 PACK_WINDOW_MASK);
          reach[1] = gLzPackWork->matchLength + len;
          if (reach[0] >= reach[1]) {
            saveLen = 1;
            defer = 1;
          }
        }
        gLzPackWork->matchOffset = saveOffset;
        gLzPackWork->matchLength = saveLen;
      }
    }

    if (gLzPackWork->matchLength > 1) {
      defer = 0;
      gLzPackWork->outBuffer[0] |= gLzPackWork->flagMask;
      if (gLzPackWork->matchLength <= 16) {
        code = ((gLzPackWork->matchOffset << 4) & ~0xfff) |
               (gLzPackWork->matchOffset & 0xff) |
               (((gLzPackWork->matchLength - 1) << 8) & 0xf00);
        gLzPackWork->outBuffer[gLzPackWork->outCount++] = (u16)code >> 8;
        gLzPackWork->outBuffer[gLzPackWork->outCount++] = code;
      } else {
        code = ((gLzPackWork->matchOffset << 4) & ~0xfff) |
               (gLzPackWork->matchOffset & 0xff);
        gLzPackWork->outBuffer[gLzPackWork->outCount++] = (u16)code >> 8;
        gLzPackWork->outBuffer[gLzPackWork->outCount++] = code;
        gLzPackWork->outBuffer[gLzPackWork->outCount++] =
            gLzPackWork->matchLength - 17;
      }
    } else {
      gLzPackWork->outBuffer[gLzPackWork->outCount++] =
          gLzPackWork->bucketBySlot[gLzPackWork->position];
      gLzPackWork->matchLength = 1;
    }

    LzPack_ConsumeSlotBytes(gLzPackWork->position + PACK_LOOKAHEAD,
                            gLzPackWork->matchLength, input);
    gLzPackWork->position =
        (gLzPackWork->position + gLzPackWork->matchLength) & PACK_WINDOW_MASK;
    gLzPackWork->flagMask >>= 1;
    if (gLzPackWork->flagMask == 0) {
      LzPack_CopyBufferedBytes(dst);
      gLzPackWork->flagMask = 0x80;
      gLzPackWork->outBuffer[0] = 0;
      gLzPackWork->outCount = 1;
    }
  }

  gLzPackWork->outBuffer[0] |= gLzPackWork->flagMask;
  gLzPackWork->outBuffer[gLzPackWork->outCount++] = 0;
  gLzPackWork->outBuffer[gLzPackWork->outCount++] = 0;
  LzPack_CopyBufferedBytes(dst);
  return gLzPackWork->outTotal;
}
