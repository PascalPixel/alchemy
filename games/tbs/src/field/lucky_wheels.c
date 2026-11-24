/// @file
/// Lucky Wheels, Tolbi's game of chance: the player bets game tickets on
/// paylines, pulls the lever up to five times and locks wheels between pulls
/// to line up matching symbols. It draws its own wheels and paylines, and
/// keeps an unused LZ packer.

#include "field/lucky_wheels.h"
#include "item_ids.h"
#include "macros.h"
#include "resource_ids.h"
#include "battle/battle.h"
#include "battle/battle_effect_state.h"
#include "battle/blitter.h"
#include "field/field.h"
#include "field/lucky_wheels_lzpack.h"
#include "field/map_grid_scroll.h"
#include "system/dma.h"
#include "system/iwram_call.h"
#include "system/main_routine_state.h"
#include "system/oam_words.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "text/msg_ids.h"

/// The sparks of the jackpot, kept in the map cell buffer.
#define LUCKY_WHEELS_SPARKS ((struct Particle *)Ram_MapGridCellBuffer)

/// Lucky Wheels' own state (struct LuckyWheelsState), in the heap slot Lucky
/// Dice keeps its sprites in, beside the battle effect slots it borrows.
#define LUCKY_WHEELS_HEAP_SLOT HEAP_SLOT_LUCKY_GAME

/// The paylines' canvas: 256 x 128 pixels of 8-bit colour, shown on BG2 from
/// its tile 212 in the rows of its map from LUCKY_WHEELS_CANVAS_TOP_ROW to
/// LUCKY_WHEELS_CANVAS_BOTTOM_ROW; the rows above and below show a clear tile,
/// in the part of BG1's map below the screen that LuckyWheels_Run clears.
#define LUCKY_WHEELS_CANVAS_SIZE (256 * 128)
#define LUCKY_WHEELS_CANVAS_FIRST_TILE 212
#define LUCKY_WHEELS_CANVAS_TOP_ROW 2
#define LUCKY_WHEELS_CANVAS_BOTTOM_ROW 16
#define LUCKY_WHEELS_CANVAS_TILES                                              \
  (BG_CHAR_BLOCK(0) + LUCKY_WHEELS_CANVAS_FIRST_TILE * TILE_SIZE_8BPP)
#define LUCKY_WHEELS_CANVAS_CLEAR_TILE 191
/// The canvas's first 64 colours, its sparks', from a Djinn sprite sheet's
/// palette and cycled through others while a win shows.
#define LUCKY_WHEELS_SPARK_COLORS 64
/// The paylines' green on the canvas, colour 64, and its lighter centre.
#define LUCKY_WHEELS_LINE_COLOR 64
#define LUCKY_WHEELS_LINE_LIGHT (LUCKY_WHEELS_LINE_COLOR + 1)
/// The machine's picture: its 16 colours in BG palette 10, and its tiles from
/// tile 424 of BG1's characters, all but those of the opening the wheels show
/// through, which BG1's map fills with a clear tile.
#define LUCKY_WHEELS_MACHINE_PALETTE 10
#define LUCKY_WHEELS_MACHINE_TILE 424
#define LUCKY_WHEELS_CLEAR_TILE 422
/// The machine's opening, in map cells: columns 5 to 24 and rows 3 to 13.
#define LUCKY_WHEELS_IN_OPENING(column, row)                                   \
  ((column) >= 5 && (column) <= 24 && (row) > 2 && (row) <= 13)
/// The icons' picture begins with its 240 colours, the sprites' first 240.
#define LUCKY_WHEELS_ICON_COLORS 240
/// The jackpot banner's and the title letters' sprite palette.
#define LUCKY_WHEELS_JACKPOT_PALETTE 15
/// A palette's 16 colours in bytes, as each 16-colour picture's resource
/// begins.
#define PALETTE_BYTES (PALETTE_COLORS * 2)
/// The Spanish title's first letter is its last letter's tile turned over both
/// ways, drawn 16 pixels left and 8 down.
#define LUCKY_WHEELS_TITLE_FIRST_TURNED (LANGUAGE == LANGUAGE_SPANISH)

/// Plays Lucky Wheels: blanks the screen, seeds the random numbers with the
/// play time and runs the game. Returns 0.
s32 LuckyWheels_Start(void) {
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
  gRandomSeed = gMainRoutineState.playTime;
  Sound_PlayFar(MUS_TRY_YOUR_LUCK);
  LuckyWheels_Run();
  return 0;
}

#include "palette_scale.c"

/// A VBlank-pass task: on a VBlank after the canvas is ready (transferPending),
/// sends the canvas to its tiles in VRAM (LUCKY_WHEELS_CANVAS_TILES), then
/// clears it or fades it into trails as transferMode says.
void LuckyWheels_TransferCanvas(void) {
  struct BattleEffectState *effect;
  u8 *canvas;

  effect = gBattleEffectState[0];
  if (effect->transferPending == 1) {
    canvas = gBattleEffectState[1];
    switch (effect->transferMode) {
    case CANVAS_COPY_AND_CLEAR:
      Dma_Set(canvas, (void *)LUCKY_WHEELS_CANVAS_TILES,
              DMA_COPY_WORDS(LUCKY_WHEELS_CANVAS_SIZE / 4), REG_DMA3);
      Iwram_FillWords(canvas, LUCKY_WHEELS_CANVAS_SIZE, effect->transferValue);
      break;
    case CANVAS_COPY_AND_FADE:
      // Trails: nonzero colour numbers drop to 1/2 or 3/4.
      if (effect->transferValue == CANVAS_FADE_HALF)
        Canvas_CopyAndHalveNonzero(canvas, LUCKY_WHEELS_CANVAS_TILES,
                                   LUCKY_WHEELS_CANVAS_SIZE);
      else
        Canvas_CopyAndScaleNonzeroThreeQuarters(
            canvas, LUCKY_WHEELS_CANVAS_TILES, LUCKY_WHEELS_CANVAS_SIZE);
      break;
    }
    effect->transferPending = 0;
    effect->vblanksSinceTransfer = 1;
  } else {
    (effect->vblanksSinceTransfer)++;
  }
}

/// Darkens the screen one step: lowers each channel of the machine's 16
/// colours and of the icons' by 1.
void LuckyWheels_DarkenStep(void) {
  u16 *col;
  s32 i;

  col = (u16 *)&BG_PLTT_COLOR(LUCKY_WHEELS_MACHINE_PALETTE, 0);
  for (i = 0; i != PALETTE_COLORS; i++) {
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
  for (i = 0; i != LUCKY_WHEELS_ICON_COLORS - 1; i++) {
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

/// Moves the sparks' colours one step toward the palette of resource; the
/// backdrop colour is not written back.
void LuckyWheels_StepSparkColors(s32 resource) {
  u16 dst[LUCKY_WHEELS_SPARK_COLORS];
  u16 *col = (u16 *)BG_PLTT;
  s32 i;

  Dma_Set(Resource_GetTableEntry(resource), dst, DMA_COPY_WORDS(sizeof dst / 4),
          REG_DMA3);
  for (i = 0; i != LUCKY_WHEELS_SPARK_COLORS; i++, col++) {
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
  Dma_Set(dst + 1, (void *)&BG_PLTT[1],
          DMA_COPY_HALFWORDS(LUCKY_WHEELS_SPARK_COLORS - 1), REG_DMA3);
}

/// The byte of the canvas's pixel (x, y): its tiles run 32 to a row, 64 bytes
/// each.
static __inline__ u32 LuckyWheels_CanvasOffset(u32 x, u32 y) {
  return ((y / 8 * 32 + x / 8) * 8 + y % 8) * 8 + x % 8;
}

/// A line's step along its shorter axis, in 256ths of a pixel.
#define LINE_STEP_ONE (1 << 8)

/// Draws a line from (x0, y0) to (x1, y1) on the 256 x 128 8bpp canvas (32
/// tiles across), writing only over lower colour numbers.
void LuckyWheels_DrawCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color) {
  s32 dx = x1 - x0;
  s32 dy = y1 - y0;
  s32 frac = LINE_STEP_ONE / 2;
  u8 *canvas = ((u8 **)gBattleEffectState)[1];
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
      /* LEFTOVER(template): dy recomputed after the swap and never read again.
       */
      dy = y1 - y0;
    }
    step = ABS(x1 - x0) * LINE_STEP_ONE / ABS(y1 - y0);
    x = x0;
    for (i = y0; i != y1; i++) {
      offset = LuckyWheels_CanvasOffset(x, i);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac & LINE_STEP_ONE) {
        if (dx > 0)
          x++;
        else
          x--;
        frac &= ~LINE_STEP_ONE;
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
      /* LEFTOVER(template): dx recomputed after the swap and never read again.
       */
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = ABS(y1 - y0) * LINE_STEP_ONE / ABS(x1 - x0);
    y = y0;
    for (i = x0; i != x1; i++) {
      offset = LuckyWheels_CanvasOffset(i, y);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac & LINE_STEP_ONE) {
        if (dy > 0)
          y++;
        else
          y--;
        frac &= ~LINE_STEP_ONE;
      }
    }
  }
}

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
/// The lever sprite's place at rest, in screen pixels, and how far it is
/// pulled down.
#define LUCKY_WHEELS_LEVER_X 208
#define LUCKY_WHEELS_LEVER_Y 40
#define LUCKY_WHEELS_LEVER_PULL 64
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
/// steps (a 256-colour tile is two): the sprite sheet is 32 pixels wide, so a
/// picture starts at the tile numbered as its row. The six symbols are 16 apart
/// from 0.
#define LUCKY_WHEELS_LAMP_TILES 96       ///< a wheel's lamp, not held
#define LUCKY_WHEELS_HELD_LAMP_TILES 128 ///< a held wheel's lamp
#define LUCKY_WHEELS_CORNER_LIGHT_TILES 224
#define LUCKY_WHEELS_FRAME_LIGHT_TILES 232
#define LUCKY_WHEELS_LEVER_TILES 256
#define LUCKY_WHEELS_LINE_TILES 272     ///< a payline's gem, not bet on
#define LUCKY_WHEELS_LIT_LINE_TILES 464 ///< a payline's gem, bet on
/// The slot the lever runs in, drawn in eight pieces 16 pixels tall down
/// beside the wheels from (LUCKY_WHEELS_LEVER_SLOT_X,
/// LUCKY_WHEELS_LEVER_SLOT_Y).
#define LUCKY_WHEELS_LEVER_SLOT_TILES 320
#define LUCKY_WHEELS_LEVER_SLOT_PIECES 8
#define LUCKY_WHEELS_LEVER_SLOT_X 206
#define LUCKY_WHEELS_LEVER_SLOT_Y 12
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
/// The pulls of the lever a bet buys: "You have only 5 chances to pull the
/// lever."
#define LUCKY_WHEELS_PULLS 5
/// The wheels' symbols, in icons.png's order.
enum LuckyWheelsSymbol {
  LUCKY_WHEELS_SHIRT,
  LUCKY_WHEELS_HEART,
  LUCKY_WHEELS_BOOT,
  LUCKY_WHEELS_STAR,
  LUCKY_WHEELS_RING,
  LUCKY_WHEELS_MOON
};
/// The symbol that matches any other on a payline.
#define LUCKY_WHEELS_WILD_SYMBOL LUCKY_WHEELS_MOON
/// The machine fades in and out over 16 frames, a sixteenth of full brightness
/// a frame; the win flashes the screen darker over as many.
#define LUCKY_WHEELS_FADE_FRAMES 16

/// The win cycles the background through five palettes, 16 frames each.
#define LUCKY_WHEELS_CYCLE_FRAMES 16
#define LUCKY_WHEELS_CYCLE_PALETTES 5

/// The jackpot sparks: four bursts of 256 in turn, one every eight frames.
#define LUCKY_WHEELS_BURST_SPARKS 256
#define LUCKY_WHEELS_SPARK_COUNT (4 * LUCKY_WHEELS_BURST_SPARKS)
/// A spark's size steps down every 12 frames of its life, from 2 or 3 to 1.
#define LUCKY_WHEELS_SPARK_SHRINK_FRAMES 12
/// A held direction moves the cursor once, waits 12 frames, then moves it
/// again with 4 frames between each move.
#define LUCKY_WHEELS_REPEAT_DELAY 12
#define LUCKY_WHEELS_REPEAT_INTERVAL 4

/// One of Lucky Wheels' five wheels: how far it has turned, its 21
/// symbols, whether it is held, and the countdown to its stop (-1 until one
/// starts).
struct Reel {
  s32 position; ///< pixels, LUCKY_WHEELS_SYMBOL_HEIGHT a symbol
  u8 symbols[LUCKY_WHEELS_REEL_SYMBOLS];
  u8 held;
  s8 stop; ///< frames
};

struct Window;

/// Lucky Wheels' phases (LuckyWheelsState.state).
enum LuckyWheelsPhase {
  LUCKY_WHEELS_BETTING = 0,  ///< Up and Down change the bet, A spins, B leaves
  LUCKY_WHEELS_SPINNING = 1, ///< the wheels spin until R or time stops each
  LUCKY_WHEELS_WON = 2,      ///< a payline won: a pause
  LUCKY_WHEELS_SHOWING_WIN = 3,    ///< the win is shown until A
  LUCKY_WHEELS_CHOOSING_HOLDS = 5, ///< choose wheels to hold, or pull the lever
  LUCKY_WHEELS_DONE = 10,          ///< the game ends
  LUCKY_WHEELS_NO_WIN = 11,        ///< no payline won: A to choose holds
  /// The fifth and last pull lost: a pause, then the end.
  LUCKY_WHEELS_OUT_OF_SPINS = 20
};

/// The prompt the top window shows while wheels are locked
/// (LuckyWheelsState.prompt), so that it is drawn again only when it changes.
enum LuckyWheelsPrompt {
  LUCKY_WHEELS_PROMPT_NONE = 0,
  /// "If you lock all the slots,", its first line.
  LUCKY_WHEELS_PROMPT_ALL_HELD = 1,
  /// Then "you can't pull the lever!" under it.
  LUCKY_WHEELS_PROMPT_ALL_HELD_BOTH_LINES = 2,
  LUCKY_WHEELS_PROMPT_START = 3,   ///< ":Start machine", on the lever
  LUCKY_WHEELS_PROMPT_HOLD = 4,    ///< ":Stop", on a spinning wheel
  LUCKY_WHEELS_PROMPT_RELEASE = 5, ///< ":Spin", on a locked wheel
  /// After a pull that won nothing: "to lock, pull the lever!" is drawn.
  LUCKY_WHEELS_PROMPT_PULL_LEVER = 1
};

/// Lucky Wheels' state in LUCKY_WHEELS_HEAP_SLOT: its wheels, state, bet and
/// spins, keys, timer, winning lines, sprites, windows and the scanline
/// offsets that shade the wheels.
struct LuckyWheelsState {
  struct Reel reels[LUCKY_WHEELS_REEL_COUNT];
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
  s32 winningLines[LUCKY_WHEELS_PAYLINES];
  struct OamWords oam[128];
  struct Window *window;
  struct Window *subWindow;
  u8 unusedAfterWindows[8]; ///< no code names it
  u16 scanlineOffsets[DISPLAY_HEIGHT];
#if TBS_PATCH_1
  /// The prompt shown (enum LuckyWheelsPrompt). The Japanese release has
  /// none: it closes and reopens its prompt window every frame.
  s32 prompt;
#endif
};

// Lucky Wheels' tables, the same in both games.

/// Where each spark picture (a square 2 to 20 pixels on a side) starts: size s
/// (1 to 10) is a square 2s pixels a side, after the smaller squares' pixels.
const u16 gLuckyWheels_SparkPictures[] = {0,   4,   20,  56,  120,
                                          220, 364, 560, 816, 1140};

/// The prize of each wheel symbol (enum LuckyWheelsPrize), kept in
/// gMainRoutineState.holdings.wonPrizes for each winning line.
const u8 gLuckyWheels_SymbolPrizes[] = {
    [LUCKY_WHEELS_SHIRT] = LUCKY_WHEELS_PRIZE_UNDERSHIRT,
    [LUCKY_WHEELS_HEART] = LUCKY_WHEELS_PRIZE_HEART_ITEM,
    [LUCKY_WHEELS_BOOT] = LUCKY_WHEELS_PRIZE_BOOTS,
    [LUCKY_WHEELS_STAR] = LUCKY_WHEELS_PRIZE_STAR_ITEM,
    [LUCKY_WHEELS_RING] = LUCKY_WHEELS_PRIZE_RING,
    [LUCKY_WHEELS_MOON] = LUCKY_WHEELS_PRIZE_ANYTHING,
};

/// The tile of each title letter, the letter's place in its sprite sheet:
/// jackpot.png holds `Jackpot!` and jackpot_ja.png `ジャックポト!`.
const u8 gLuckyWheels_TitleLetterTiles[LUCKY_WHEELS_TITLE_LETTERS] = {
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

#define LUCKY_WHEELS_MACHINE_LIGHTS 14 ///< the machine's lights
#define LUCKY_WHEELS_CORNER_LIGHTS 4   ///< the first of them, at its corners
/// The positions of the machine's lights, in screen pixels: its four corners
/// first, then the lights along its frame.
const u8 gLuckyWheels_MachineSpriteX[LUCKY_WHEELS_MACHINE_LIGHTS] = {
    2, 0, 232, 232, 32, 32, 200, 200, 28, 28, 208, 208, 219, 219};
const u8 gLuckyWheels_MachineSpriteY[LUCKY_WHEELS_MACHINE_LIGHTS] = {
    2, 152, 0, 152, 8, 104, 8, 104, 118, 140, 118, 140, 6, 108};

/// The advance of each title letter, in pixels, from LUCKY_WHEELS_TITLE_X. The
/// last letter's advance is never used; Japanese leaves it out (0).
const u8 gLuckyWheels_TitleLetterWidths[LUCKY_WHEELS_TITLE_LETTERS] = {
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

/// Lucky Wheels' VBlank task: reads the keys, runs the game's state
/// (betting, spinning and stopping the wheels, scoring the paylines, choosing
/// wheels to hold, showing a win) and builds all 128 sprites: the cursor, the
/// title letters of a win, the frame, the hold lamps, the spin counter, the
/// paylines, the wheel symbols, the lever and the bottom bar.
void LuckyWheels_Update(void) {
  struct LuckyWheelsState *wheels;
  struct BattleEffectState *effect;
  volatile u16 *dma;
  s32 sprite;
  s32 objMode;
  u16 keys;
  u32 dpad;
  s32 tickets;
  s32 allHeld;
  s32 stoppedReels;
  s32 line;
  s32 arrowFrame;
  s32 x;
  s32 y;
  s32 lever;
  s32 k;
  s32 i;

  wheels =
      ((struct LuckyWheelsState **)gBattleEffectState)[LUCKY_WHEELS_HEAP_SLOT -
                                                       HEAP_SLOT_BATTLE_EFFECT];
  effect = gBattleEffectState[0];
  sprite = 0;
  objMode = OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);

  Random16();

  dma = REG_DMA0;
  dma[DMA_CNT_H_INDEX] &= ~DMA_START_TIMING_DRQ_REPEAT;
  dma[DMA_CNT_H_INDEX] &= ~DMA_ENABLE;
  (void)dma[DMA_CNT_H_INDEX];
  // One halfword at each HBlank (repeating, destination reloaded).
  Dma_Set(wheels->scanlineOffsets, (void *)&REG_BLDY,
          DMA_HBLANK_HALFWORD_PER_LINE, (volatile u32 *)dma);

  keys = gKeysHeld;
  dpad = gKeysHeld & KEYS_DPAD;
  wheels->pressed = keys & ~wheels->keys;
  wheels->direction = dpad;
  if ((wheels->keys & KEYS_DPAD) == wheels->direction) {
    if (wheels->repeat > LUCKY_WHEELS_REPEAT_DELAY)
      wheels->repeat = LUCKY_WHEELS_REPEAT_DELAY;
    if (wheels->repeat == 0) {
      wheels->repeat = LUCKY_WHEELS_REPEAT_INTERVAL;
    } else {
      wheels->repeat--;
      wheels->direction = 0;
    }
  } else {
    wheels->repeat = LUCKY_WHEELS_REPEAT_DELAY;
  }
  wheels->keys = keys;

  if (gDebugPaused == 0) {
    if (wheels->state == LUCKY_WHEELS_BETTING) {
      tickets = PartyInventory_CountItemFar(ITEM_GAME_TICKET);
      Text_DrawNumberInWindowFar(tickets - wheels->bet,
                                 LUCKY_WHEELS_TICKET_DIGITS, wheels->subWindow,
                                 LUCKY_WHEELS_TICKET_X, 0);
      Text_DrawNumberInWindowFar(wheels->bet, 2, wheels->subWindow, 64, 8);
      if (wheels->pressed & KEY_B) {
        struct PartyHoldings *holdings = &gMainRoutineState.holdings;

        wheels->state = LUCKY_WHEELS_DONE;
        holdings->wonPrizes[0] = LUCKY_WHEELS_PRIZE_LIST_DONE;
        Window_CloseFar(wheels->window, 1);
      } else {
        if (wheels->pressed & KEY_UP) {
          if (wheels->bet < LUCKY_WHEELS_BET_MAX && tickets > wheels->bet) {
            wheels->bet++;
            Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          } else {
            Sound_PlayFar(SE_MENU_CANCEL);
          }
        }
        if (wheels->pressed & KEY_DOWN) {
          if (wheels->bet > 1) {
            wheels->bet--;
            Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          } else {
            Sound_PlayFar(SE_MENU_CANCEL);
          }
        }
        REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_DARKEN | BLDCNT_TGT2_ALL;
        REG_BLDALPHA = BLEND_ALPHA(16, 0);
        if (wheels->pressed & KEY_A) {
          wheels->state = LUCKY_WHEELS_SPINNING;
          effect->frame = 0;
          Window_CloseFar(wheels->window, 1);
          for (k = 0; k != wheels->bet; k++)
            PartyInventory_RemoveFar(ITEM_GAME_TICKET);
          Window_CloseFar(wheels->subWindow, 1);
          Sound_PlayFar(SE_LUCKY_WHEELS_SPIN);
        }
      }
    } else if (wheels->state == LUCKY_WHEELS_CHOOSING_HOLDS) {
      allHeld = 0;
      wheels->timer++;
      for (k = 0; k != LUCKY_WHEELS_REEL_COUNT && wheels->reels[k].held != 0;
           k++)
        ;
      if (k == LUCKY_WHEELS_REEL_COUNT)
        allHeld = 1;
      if (wheels->pressed & KEY_A) {
        wheels->timer = 0;
        effect->frame = 0;
        if (wheels->spins == LUCKY_WHEELS_PULLS - 1) {
          wheels->spins = 0;
          wheels->cursor = 0;
          wheels->state = LUCKY_WHEELS_BETTING;
          for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
            wheels->reels[i].held = 0;
            wheels->reels[i].stop = -1;
          }
        } else if (wheels->cursor < LUCKY_WHEELS_LEVER) {
          Sound_PlayFar(SE_LUCKY_WHEELS_HOLD);
          wheels->reels[wheels->cursor].held ^= 1;
        } else if (allHeld == 0) {
          Sound_PlayFar(SE_LUCKY_WHEELS_SPIN);
          wheels->state = LUCKY_WHEELS_SPINNING;
          wheels->cursor = 0;
          for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++)
            wheels->reels[i].stop = -1;
          wheels->spins++;
        } else {
          Sound_PlayFar(SE_MENU_CANCEL);
        }
      } else {
        if (wheels->direction & KEY_RIGHT) {
          wheels->cursor = (wheels->cursor + 1) % (LUCKY_WHEELS_LEVER + 1);
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        }
        if (wheels->direction & KEY_LEFT) {
          wheels->cursor =
              (wheels->cursor + LUCKY_WHEELS_LEVER) % (LUCKY_WHEELS_LEVER + 1);
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        }
      }

      if (wheels->state == LUCKY_WHEELS_CHOOSING_HOLDS) {
#if TBS_PATCH_1
        if (wheels->cursor == LUCKY_WHEELS_LEVER) {
          if (allHeld != 0) {
            if (wheels->prompt != LUCKY_WHEELS_PROMPT_ALL_HELD &&
                wheels->prompt != LUCKY_WHEELS_PROMPT_ALL_HELD_BOTH_LINES) {
              Window_CloseFar(wheels->window, 1);
              wheels->window = Window_CreateFar(
                  LUCKY_WHEELS_ALL_HELD_X, 0, LUCKY_WHEELS_ALL_HELD_WIDTH, 4,
                  WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
              Text_DrawMessageAsTilesFar(MsgSlotsAllHeld, wheels->window, 0, 0);
              wheels->prompt = LUCKY_WHEELS_PROMPT_ALL_HELD;
            } else if (wheels->prompt == LUCKY_WHEELS_PROMPT_ALL_HELD) {
              Text_DrawMessageAsTilesFar(MsgSlotsCannotPull, wheels->window, 0,
                                         8);
              wheels->prompt = LUCKY_WHEELS_PROMPT_ALL_HELD_BOTH_LINES;
            }
          } else {
            if (wheels->prompt != LUCKY_WHEELS_PROMPT_START) {
              Window_CloseFar(wheels->window, 1);
              wheels->window = Window_CreateFar(
                  LUCKY_WHEELS_START_X, 0, LUCKY_WHEELS_START_WIDTH, 3,
                  WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
              Text_DrawMessageAsTilesFar(MsgSlotsStart, wheels->window, 0, 0);
            }
            wheels->prompt = LUCKY_WHEELS_PROMPT_START;
          }
        } else if (wheels->reels[wheels->cursor].held == 0) {
          if (wheels->prompt != LUCKY_WHEELS_PROMPT_HOLD) {
            Window_CloseFar(wheels->window, 1);
            wheels->window = Window_CreateFar(
                LUCKY_WHEELS_HOLD_X, 0, LUCKY_WHEELS_HOLD_WIDTH, 3,
                WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
            Text_DrawMessageAsTilesFar(MsgSlotsHold, wheels->window, 0, 0);
          }
          wheels->prompt = LUCKY_WHEELS_PROMPT_HOLD;
        } else {
          if (wheels->prompt != LUCKY_WHEELS_PROMPT_RELEASE) {
            Window_CloseFar(wheels->window, 1);
            wheels->window = Window_CreateFar(
                LUCKY_WHEELS_RELEASE_X, 0, LUCKY_WHEELS_RELEASE_WIDTH, 3,
                WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
            Text_DrawMessageAsTilesFar(MsgSlotsRelease, wheels->window, 0, 0);
          }
          wheels->prompt = LUCKY_WHEELS_PROMPT_RELEASE;
        }
#else
        Window_CloseFar(wheels->window, 1);
        if (wheels->cursor == LUCKY_WHEELS_LEVER) {
          if (allHeld != 0) {
            wheels->window = Window_CreateFar(
                12, 0, 18, 3, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
            Text_DrawMessageAsTilesFar(MsgSlotsAllHeld, wheels->window, 0, 0);
          } else {
            wheels->window = Window_CreateFar(
                18, 0, 12, 3, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
            Text_DrawMessageAsTilesFar(MsgSlotsStart, wheels->window, 0, 0);
          }
        } else {
          wheels->window = Window_CreateFar(
              18, 0, 12, 3, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
          if (wheels->reels[wheels->cursor].held == 0)
            Text_DrawMessageAsTilesFar(MsgSlotsHold, wheels->window, 0, 0);
          else
            Text_DrawMessageAsTilesFar(MsgSlotsRelease, wheels->window, 0, 0);
        }
#endif
      } else {
        Window_CloseFar(wheels->window, 1);
      }
    } else if (wheels->state == LUCKY_WHEELS_WON) {
      wheels->timer++;
      objMode = 0;
      if (wheels->timer == LUCKY_WHEELS_WIN_PAUSE) {
        wheels->state = LUCKY_WHEELS_SHOWING_WIN;
        Sound_PlayFar(MUS_SUPER_LUCKY);
        wheels->timer = 0;
        REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        REG_BLDALPHA = BLEND_ALPHA(16, 16);
        effect->transferMode = CANVAS_COPY_AND_FADE;
        effect->transferValue = CANVAS_FADE_THREE_QUARTERS;
      }
    } else if (wheels->state == LUCKY_WHEELS_SHOWING_WIN) {
      wheels->timer++;
      objMode = 0;
      if (wheels->pressed & KEY_A) {
        wheels->state = LUCKY_WHEELS_DONE;
        Sound_PlayFar(SE_MENU_CONFIRM);
      }
    } else if (wheels->state == LUCKY_WHEELS_NO_WIN) {
#if TBS_PATCH_1
      if (wheels->prompt == LUCKY_WHEELS_PROMPT_NONE) {
        wheels->prompt = LUCKY_WHEELS_PROMPT_PULL_LEVER;
        Text_DrawMessageAsTilesFar(MsgSlotsPullLever, wheels->window, 0, 8);
      }
#endif
      if (wheels->pressed & KEY_A) {
        wheels->state = LUCKY_WHEELS_CHOOSING_HOLDS;
#if TBS_PATCH_1
        wheels->prompt = LUCKY_WHEELS_PROMPT_NONE;
#endif
        Sound_PlayFar(SE_MENU_CONFIRM);
        Window_CloseFar(wheels->window, 1);
      }
    } else if (wheels->state == LUCKY_WHEELS_OUT_OF_SPINS) {
      wheels->timer++;
      if (wheels->timer == LUCKY_WHEELS_LOSS_PAUSE)
        wheels->state = LUCKY_WHEELS_DONE;
    } else if (wheels->state != LUCKY_WHEELS_DONE) {
      if (wheels->timer == 4) {
        wheels->window =
            Window_CreateFar(LUCKY_WHEELS_STOP_X, 17, LUCKY_WHEELS_STOP_WIDTH,
                             3, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
        Text_DrawMessageAsTilesFar(MsgSlotsStopReels, wheels->window, 0, 0);
      }
      if (wheels->timer == LUCKY_WHEELS_STOP_PROMPT_FRAME)
        Sound_PlayFar(SE_LUCKY_WHEELS_STOP_PROMPT);
      if (wheels->timer > LUCKY_WHEELS_LEVER_FRAMES) {
        if (effect->frame >= LUCKY_WHEELS_AUTO_STOP_FRAMES ||
            (wheels->pressed & KEY_R)) {
          effect->frame = 0;
          for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
            if (wheels->reels[i].held == 0 && wheels->reels[i].stop == -1) {
              wheels->reels[i].stop = (Random16() & 3) + 4;
              Sound_PlayFar(SE_LUCKY_WHEELS_STOP);
              break;
            }
          }
        }
      }

      for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
        if (wheels->reels[i].stop > 0)
          wheels->reels[i].stop--;
      }

      stoppedReels = 0;
      for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
        if (wheels->reels[i].held == 1)
          stoppedReels++;
        else if (wheels->reels[i].stop == 0 &&
                 (wheels->reels[i].position &
                  (LUCKY_WHEELS_SYMBOL_HEIGHT - 1)) ==
                     LUCKY_WHEELS_SYMBOL_HEIGHT / 2)
          stoppedReels++;
      }

      if (stoppedReels == LUCKY_WHEELS_REEL_COUNT) {
        struct PartyHoldings *holdings = &gMainRoutineState.holdings;
        s32 hits = 0;

        for (line = 0; line != LUCKY_WHEELS_PAYLINES; line++) {
          s32 mixed;
          s32 found;

          found = -1;
          mixed = 0;
          wheels->winningLines[line] = 0;
          if (line > LUCKY_WHEELS_MIDDLE_LINE - wheels->bet &&
              line < wheels->bet + LUCKY_WHEELS_MIDDLE_LINE) {
            for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
              s32 symbol;

              if (line == 0)
                symbol =
                    wheels->reels[i].symbols[(i -
                                              wheels->reels[i].position /
                                                  LUCKY_WHEELS_SYMBOL_HEIGHT +
                                              LUCKY_WHEELS_REEL_SYMBOLS + 1) %
                                             LUCKY_WHEELS_REEL_SYMBOLS];
              else if (line == LUCKY_WHEELS_PAYLINES - 1)
                symbol =
                    wheels->reels[i].symbols[(-i -
                                              wheels->reels[i].position /
                                                  LUCKY_WHEELS_SYMBOL_HEIGHT +
                                              LUCKY_WHEELS_REEL_SYMBOLS + 5) %
                                             LUCKY_WHEELS_REEL_SYMBOLS];
              else
                symbol =
                    wheels->reels[i].symbols[(line -
                                              wheels->reels[i].position /
                                                  LUCKY_WHEELS_SYMBOL_HEIGHT +
                                              LUCKY_WHEELS_REEL_SYMBOLS) %
                                             LUCKY_WHEELS_REEL_SYMBOLS];
              if (symbol != LUCKY_WHEELS_WILD_SYMBOL) {
                if (found == -1)
                  found = symbol;
                else if (found != symbol)
                  mixed = 1;
              }
            }
            if (mixed == 0) {
              wheels->winningLines[line] = 1;
              holdings->wonPrizes[hits] = gLuckyWheels_SymbolPrizes[found];
              hits++;
            }
          }
        }
        wheels->timer = 0;
        if (hits != 0) {
          holdings->wonPrizes[hits] = LUCKY_WHEELS_PRIZE_LIST_END;
          wheels->state = LUCKY_WHEELS_WON;
          Sound_PlayFar(SE_THUNDER);
          effect->transferMode = CANVAS_COPY_AND_CLEAR;
          effect->transferValue = 0;
          REG_BLDCNT = 0;
          Window_CloseFar(wheels->window, 1);
        } else {
          wheels->state = LUCKY_WHEELS_NO_WIN;
#if TBS_PATCH_1
          wheels->prompt = LUCKY_WHEELS_PROMPT_NONE;
          Window_CloseFar(wheels->window, 1);
#if LUCKY_WHEELS_HINT_BEFORE_LAST_SPIN
          if (wheels->spins < LUCKY_WHEELS_PULLS - 1)
#endif
          {
            wheels->window = Window_CreateFar(
                3, 16, 24, 4, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
            Text_DrawMessageAsTilesFar(MsgSlotsChooseHeld, wheels->window, 0,
                                       0);
          }
#else
          Window_CloseFar(wheels->window, 1);
          {
            s32 message;

            wheels->window = Window_CreateFar(
                7, 16, 16, 4, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
            // "When you've chosen which slots" above "to lock, pull the
            // lever!".
            message = MsgSlotsChooseHeld;
            Text_DrawMessageAsTilesFar(message, wheels->window, 0, 0);
            Text_DrawMessageAsTilesFar(message + 1, wheels->window, 0, 8);
          }
#endif
          if (wheels->spins == LUCKY_WHEELS_PULLS - 1) {
            s32 left = PartyInventory_CountItemFar(ITEM_GAME_TICKET);
            struct Window *window;
            s32 message;

            // DEAD: both arms set the out-of-spins state, so the ticket count
            // cannot change it; the ROM keeps the compare and both stores.
            if (left > 0)
              wheels->state = LUCKY_WHEELS_OUT_OF_SPINS;
            else
              wheels->state = LUCKY_WHEELS_OUT_OF_SPINS;
            if (wheels->bet > left)
              wheels->bet = left;
            wheels->spins = 0;
            wheels->cursor = 0;
            for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
              wheels->reels[i].held = 0;
              wheels->reels[i].stop = -1;
            }
            effect->transferMode = CANVAS_COPY_AND_CLEAR;
            effect->transferValue = 0;
            REG_BLDCNT = 0;
            window = Window_CreateFar(18, 0, 12, 4,
                                      WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
            wheels->subWindow = window;
            // "Game ticket" above "Bet".
            message = MsgSlotsBet;
            Text_DrawMessageAsTilesFar(message, window, 0, 8);
            Text_DrawMessageAsTilesFar(message - 1, wheels->subWindow, 0, 0);
          }
        }
      }

      if (wheels->state == LUCKY_WHEELS_SPINNING) {
        for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
          struct Reel *reel = &wheels->reels[i];

          if (reel->held == 0) {
            if (reel->stop != 0)
              reel->position += LUCKY_WHEELS_SPIN_SPEED;
            else if ((reel->position & (LUCKY_WHEELS_SYMBOL_HEIGHT - 1)) !=
                     LUCKY_WHEELS_SYMBOL_HEIGHT / 2)
              reel->position += LUCKY_WHEELS_SPIN_SPEED;
            if (reel->position == LUCKY_WHEELS_REEL_TURN)
              reel->position = 0;
          }
        }
      }
      (effect->frame)++;
      wheels->timer++;
    }
  }

  if (wheels->state == LUCKY_WHEELS_CHOOSING_HOLDS) {
    x = wheels->cursor * 36 + 36;
    y = 128;
    arrowFrame = 0;
    if ((wheels->timer & 15) <= 7)
      arrowFrame = 1;
    if (wheels->cursor == LUCKY_WHEELS_LEVER) {
      x = LUCKY_WHEELS_LEVER_X;
      y = 32;
    }
    wheels->oam[sprite].attr01 =
        OBJ_X(x - 12) | objMode | (y + 8) | OBJ_SIZE_32X16 | OBJ_256_COLORS;
    wheels->oam[sprite].attr2 =
        arrowFrame * SPRITE_TILES_8BPP(32, 16) + LUCKY_WHEELS_ARROW_TILES;
    sprite++;
    wheels->oam[sprite].attr01 = OBJ_X(x + 12) | objMode | (y + 8) |
                                 OBJ_SIZE_32X16 | OBJ_256_COLORS | OBJ_HFLIP;
    wheels->oam[sprite].attr2 =
        arrowFrame * SPRITE_TILES_8BPP(32, 16) + LUCKY_WHEELS_ARROW_TILES;
    sprite++;
    wheels->oam[sprite].attr01 =
        OBJ_X(x) | objMode | y | OBJ_SIZE_32X32 | OBJ_256_COLORS;
    wheels->oam[sprite].attr2 = LUCKY_WHEELS_HAND_TILES;
    sprite++;
  }

  if (wheels->state == LUCKY_WHEELS_SHOWING_WIN) {
    for (i = 0; i != LUCKY_WHEELS_TITLE_LETTERS; i++) {
      struct Particle *p = &effect->particles[i];

      if (LUCKY_WHEELS_TITLE_FIRST_TURNED && i == 0)
        wheels->oam[sprite].attr01 =
            OBJ_X(((s16)FIXEDtoINT(p->x) + 512 - 16) & OBJ_X_MASK) | objMode |
            (((s16)FIXEDtoINT(p->y) + OBJ_Y_RANGE + 8) & (OBJ_Y_RANGE - 1)) |
            OBJ_SIZE_32X32 | OBJ_HFLIP | OBJ_VFLIP;
      else
        wheels->oam[sprite].attr01 =
            OBJ_X((s16)FIXEDtoINT(p->x)) | objMode |
            (((s16)FIXEDtoINT(p->y) + OBJ_Y_RANGE) & (OBJ_Y_RANGE - 1)) |
            OBJ_SIZE_32X32;
      wheels->oam[sprite].attr2 =
          (gLuckyWheels_TitleLetterTiles[i] * SPRITE_TILES(32, 32) +
           LUCKY_WHEELS_TITLE_TILES) |
          OBJ_PALETTE(LUCKY_WHEELS_JACKPOT_PALETTE);
      p->y += p->velocityY;
      p->velocityY += toFIXED(1) / 4;
      if (wheels->timer % 256 == i * 4 + 200) {
        p->velocityY = toFIXED(6);
        p->variant = 0;
      }
      if (p->y > toFIXED(64)) {
        p->y = toFIXED(64);
        if (p->variant <= 1)
          p->velocityY = -p->velocityY / 2;
        p->variant++;
      }
      sprite++;
    }
  }

  for (k = 0; k != LUCKY_WHEELS_MACHINE_LIGHTS; k++) {
    wheels->oam[sprite].attr01 = OBJ_X(gLuckyWheels_MachineSpriteX[k]) |
                                 objMode | gLuckyWheels_MachineSpriteY[k] |
                                 OBJ_SIZE_32X16 | OBJ_256_COLORS;
    if (k < LUCKY_WHEELS_CORNER_LIGHTS)
      wheels->oam[sprite].attr2 =
          OBJ_PRIORITY(1) | LUCKY_WHEELS_CORNER_LIGHT_TILES;
    else
      wheels->oam[sprite].attr2 =
          OBJ_PRIORITY(1) | LUCKY_WHEELS_FRAME_LIGHT_TILES;
    sprite++;
  }

  for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
    if (wheels->reels[i].held == 0) {
      wheels->oam[sprite].attr01 =
          objMode | OBJ_X(i * 36 + 32) | 124 | OBJ_SIZE_32X32 | OBJ_256_COLORS;
      wheels->oam[sprite].attr2 = OBJ_PRIORITY(1) | LUCKY_WHEELS_LAMP_TILES;
    } else {
      wheels->oam[sprite].attr01 =
          objMode | OBJ_X(i * 36 + 32) | 124 | OBJ_SIZE_32X32 | OBJ_256_COLORS;
      wheels->oam[sprite].attr2 =
          OBJ_PRIORITY(1) | LUCKY_WHEELS_HELD_LAMP_TILES;
    }
    sprite++;
  }

  // One sprite for each pull of the lever, the first and its re-pulls; the
  // current pull's takes other tiles.
  for (i = 0; i != LUCKY_WHEELS_PULLS; i++) {
    wheels->oam[sprite].attr01 =
        objMode | OBJ_X(i * 16 + 32) | 3 | OBJ_SIZE_32X16 | OBJ_256_COLORS;
    if (i == wheels->spins)
      wheels->oam[sprite].attr2 =
          (i * 2 * SPRITE_TILES_8BPP(32, 16) + LUCKY_WHEELS_PULL_TILES) |
          OBJ_PRIORITY(1);
    else
      wheels->oam[sprite].attr2 =
          (i * 2 * SPRITE_TILES_8BPP(32, 16) + LUCKY_WHEELS_PULL_TILES +
           SPRITE_TILES_8BPP(32, 16)) |
          OBJ_PRIORITY(1);
    sprite++;
  }

  for (line = 0; line != LUCKY_WHEELS_PAYLINES; line++) {
    wheels->oam[sprite].attr01 = OBJ_X((516 - ((line & 1) << 3)) & OBJ_X_MASK) |
                                 objMode |
                                 (line * LUCKY_WHEELS_SYMBOL_HEIGHT + 5) |
                                 OBJ_SIZE_32X32 | OBJ_256_COLORS;
    if (line > LUCKY_WHEELS_MIDDLE_LINE - wheels->bet &&
        line < wheels->bet + LUCKY_WHEELS_MIDDLE_LINE)
      wheels->oam[sprite].attr2 = OBJ_PRIORITY(1) | LUCKY_WHEELS_LIT_LINE_TILES;
    else
      wheels->oam[sprite].attr2 = OBJ_PRIORITY(1) | LUCKY_WHEELS_LINE_TILES;
    sprite++;
  }

  for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
    struct Reel *reel = &wheels->reels[i];

    for (line = 0; line != LUCKY_WHEELS_PAYLINES; line++) {
      wheels->oam[sprite].attr01 =
          (line * LUCKY_WHEELS_SYMBOL_HEIGHT +
           reel->position % LUCKY_WHEELS_SYMBOL_HEIGHT + 4) |
          OBJ_X(i * 32 + 40) | OBJ_SIZE_32X16 | OBJ_256_COLORS;
      wheels->oam[sprite].attr2 =
          reel->symbols[(line - reel->position / LUCKY_WHEELS_SYMBOL_HEIGHT +
                         LUCKY_WHEELS_REEL_SYMBOLS) %
                        LUCKY_WHEELS_REEL_SYMBOLS] *
              SPRITE_TILES_8BPP(32, 16) |
          OBJ_PRIORITY(2);
      sprite++;
    }
  }

  // The lever, pulled down and let back up, then bouncing as it settles.
  lever = LUCKY_WHEELS_LEVER_Y;
  if (wheels->state == LUCKY_WHEELS_SPINNING) {
    if (wheels->timer < LUCKY_WHEELS_LEVER_PULL_FRAMES)
      lever = FIXEDtoINT(
                  Trig_Sin(wheels->timer *
                           (DEGtoANG(180) / LUCKY_WHEELS_LEVER_PULL_FRAMES)) *
                  LUCKY_WHEELS_LEVER_PULL) +
              LUCKY_WHEELS_LEVER_Y;
    else if (wheels->timer < LUCKY_WHEELS_LEVER_FRAMES)
      lever =
          FIXEDtoINT(
              Trig_Sin(wheels->timer * (TRIG_TURN / 16) - TRIG_TURN * 3) * 4) +
          LUCKY_WHEELS_LEVER_Y;
  }
  wheels->oam[sprite].attr01 = lever | objMode | OBJ_X(LUCKY_WHEELS_LEVER_X) |
                               OBJ_SIZE_32X16 | OBJ_256_COLORS;
  wheels->oam[sprite].attr2 = OBJ_PRIORITY(1) | LUCKY_WHEELS_LEVER_TILES;
  sprite++;

  // The lever's slot, in eight pieces from its top: two of its own, four alike
  // and two for its bottom.
  for (k = 0; k != LUCKY_WHEELS_LEVER_SLOT_PIECES; k++) {
    wheels->oam[sprite].attr01 =
        objMode | (k * 16 + LUCKY_WHEELS_LEVER_SLOT_Y) |
        OBJ_X(LUCKY_WHEELS_LEVER_SLOT_X) | OBJ_SIZE_32X16 | OBJ_256_COLORS;
    if (k == 0)
      wheels->oam[sprite].attr2 =
          OBJ_PRIORITY(1) | LUCKY_WHEELS_LEVER_SLOT_TILES;
    else if (k == 1)
      wheels->oam[sprite].attr2 =
          OBJ_PRIORITY(1) |
          (LUCKY_WHEELS_LEVER_SLOT_TILES + SPRITE_TILES_8BPP(32, 16));
    else if (k == 6)
      wheels->oam[sprite].attr2 =
          OBJ_PRIORITY(1) |
          (LUCKY_WHEELS_LEVER_SLOT_TILES + 3 * SPRITE_TILES_8BPP(32, 16));
    else if (k == 7)
      wheels->oam[sprite].attr2 =
          OBJ_PRIORITY(1) |
          (LUCKY_WHEELS_LEVER_SLOT_TILES + 4 * SPRITE_TILES_8BPP(32, 16));
    else
      wheels->oam[sprite].attr2 =
          OBJ_PRIORITY(1) |
          (LUCKY_WHEELS_LEVER_SLOT_TILES + 2 * SPRITE_TILES_8BPP(32, 16));
    sprite++;
  }

  for (; sprite != OAM_SIZE / sizeof(struct OamWords); sprite++) {
    wheels->oam[sprite].attr01 =
        OBJ_X(DISPLAY_WIDTH) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
    wheels->oam[sprite].attr2 = 0;
  }

  Dma_Set(wheels->oam, OAM, DMA_COPY_WORDS(sprite * 2), REG_DMA3);
}

/// The screen lines of the wheels' top and bottom edges, and the lines from
/// each that LuckyWheels_SetUpBetting shades.
#define LUCKY_WHEELS_WHEELS_TOP 23
#define LUCKY_WHEELS_WHEELS_BOTTOM 110
#define LUCKY_WHEELS_SHADE_LINES 40
/// The darkening at the wheels' very edges, in BLDY's sixteenths.
#define LUCKY_WHEELS_SHADE_LEVEL 6

/// Sets up the bouncing title letters of a win, the brightness wave of the
/// screen's lines and the betting window, and starts at betting.
void LuckyWheels_SetUpBetting(void) {
  struct LuckyWheelsState *wheels;
  struct BattleEffectState *effect;
  struct Particle *letter;
  const u8 *width;
  s32 left;
  s32 step;
  FIXED y;
  s32 i;
  s32 angle;
  struct Window *window;
  s32 message;

  wheels =
      ((struct LuckyWheelsState **)gBattleEffectState)[LUCKY_WHEELS_HEAP_SLOT -
                                                       HEAP_SLOT_BATTLE_EFFECT];
  effect = gBattleEffectState[0];
  left = 0;
  // Twice the sparks the win uses are cleared.
  for (i = 0; i != 2 * LUCKY_WHEELS_SPARK_COUNT; i++)
    LUCKY_WHEELS_SPARKS[i].variant = 0;

  letter = effect->particles;
  y = -toFIXED(32);
  width = gLuckyWheels_TitleLetterWidths;
  for (i = 0; i != LUCKY_WHEELS_TITLE_LETTERS; i++) {
    letter[i].x = INTtoFIXED(left + LUCKY_WHEELS_TITLE_X);
    step = *width;
    width++;
    left += step;
    letter[i].y = y;
    letter[i].velocityY = 0;
    letter[i].variant = 0;
    y += -toFIXED(8);
  }

  for (i = 0; i != DISPLAY_HEIGHT; i++)
    wheels->scanlineOffsets[i] = 0;

  // The wheels darken toward their top and bottom edges, as a drum turns away:
  // BLDY falls from LUCKY_WHEELS_SHADE_LEVEL at each edge to 0 over a quarter
  // turn of the cosine.
  for (i = 0; i != LUCKY_WHEELS_SHADE_LINES; i++) {
    angle = i * (DEGtoANG(90) / LUCKY_WHEELS_SHADE_LINES);
    wheels->scanlineOffsets[LUCKY_WHEELS_WHEELS_TOP + i] =
        FIXEDtoINT((u32)(Trig_Cos(angle) * LUCKY_WHEELS_SHADE_LEVEL));
    wheels->scanlineOffsets[LUCKY_WHEELS_WHEELS_BOTTOM - i] =
        FIXEDtoINT((u32)(Trig_Cos(angle) * LUCKY_WHEELS_SHADE_LEVEL));
  }

  wheels->spins = 0;
  wheels->state = LUCKY_WHEELS_BETTING;
  wheels->cursor = 0;
  effect->transferMode = CANVAS_COPY_AND_CLEAR;
  effect->transferValue = 0;
  REG_BLDCNT = 0;
  window =
      Window_CreateFar(18, 0, 12, 4, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
  wheels->subWindow = window;
  // "Game ticket" above "Bet".
  message = MsgSlotsBet;
  Text_DrawMessageAsTilesFar(message, window, 0, 8);
  Text_DrawMessageAsTilesFar(message - 1, wheels->subWindow, 0, 0);
}

/// The places on each wheel given a set symbol after the random fill: one of
/// each plain symbol, then three moons.
#define LUCKY_WHEELS_PLACED_SYMBOLS 8

/// Lucky Wheels: loads its backgrounds, sprites and palettes, fills each of
/// the five wheels with 21 random symbols, among them each plain symbol and
/// three moons, then runs until the player leaves: fading in, drawing the
/// paylines on the canvas while betting or winning, and the jackpot's sparks
/// and colour cycle while showing a win. Then fades out and frees its memory.
void LuckyWheels_Run(void) {
  s32 picks[LUCKY_WHEELS_PLACED_SYMBOLS];
  s32 litLines[LUCKY_WHEELS_PAYLINES];
  DrawRectangle draw[2];
  u8 *sheet;
  void *canvas;
  struct BattleEffectState *effect;
  struct LuckyWheelsState *wheels;
  struct PartyHoldings *holdings;
  u8 *entry;
  u8 *tiles;
  s32 frame;
  u32 count;
  s32 i;
  s32 j;
  s32 k;

  sheet = HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, GLOW_SHEET_ROOM);
  canvas = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, LUCKY_WHEELS_CANVAS_SIZE);
  effect = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_EFFECT,
                               sizeof(struct BattleEffectState));
  wheels = HeapSlot_AllocEwram(LUCKY_WHEELS_HEAP_SLOT,
                               sizeof(struct LuckyWheelsState));
  tiles = Ram_MapGridCellBuffer;
  holdings = &gMainRoutineState.holdings;
  Stub_NoOpHook(RESOURCE_BATTLE_EFFECT_FAR_CALLS);
  holdings->wonPrizes[0] = LUCKY_WHEELS_PRIZE_LIST_END;
  wheels->repeat = 0;
  wheels->bet = 1;
  Task_ResetAll();
  gOamCopyEnabled = 0;

  // BG1 is the machine, its tiles in order but for the opening's.
  count = 0;
  j = 0;
  for (k = 0; k != DISPLAY_TILE_HEIGHT; k++) {
    for (i = 0; i != BG_SCREEN_WIDTH; i++, j += 2) {
      if (LUCKY_WHEELS_IN_OPENING(i, k)) {
        *(volatile u16 *)(BG_SCREEN_BLOCK(5) + j) =
            BG_TILE_PALETTE(LUCKY_WHEELS_MACHINE_PALETTE) |
            LUCKY_WHEELS_CLEAR_TILE;
      } else if (i >= DISPLAY_TILE_WIDTH) {
        *(volatile u16 *)(BG_SCREEN_BLOCK(5) + j) = 0;
      } else {
        *(volatile u16 *)(BG_SCREEN_BLOCK(5) + j) =
            (BG_TILE_PALETTE(LUCKY_WHEELS_MACHINE_PALETTE) |
             LUCKY_WHEELS_MACHINE_TILE) +
            count;
        count++;
      }
    }
  }
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_BLUE_GLOW_SHEET), sheet);
  entry = Resource_GetTableEntry(RESOURCE_LUCKY_WHEELS_MACHINE_PICTURE);
  Dma_Set(entry, (void *)&BG_PLTT_COLOR(LUCKY_WHEELS_MACHINE_PALETTE, 0),
          DMA_COPY_WORDS(PALETTE_BYTES / 4), REG_DMA3);
  entry += PALETTE_BYTES;
  Decompress_TaggedLz(entry, tiles);
  j = 0;
  for (k = 0; k != DISPLAY_TILE_HEIGHT; k++) {
    for (i = 0; i != DISPLAY_TILE_WIDTH; i++) {
      if (!LUCKY_WHEELS_IN_OPENING(i, k)) {
        Dma_Set(tiles + (k * DISPLAY_TILE_WIDTH + i) * TILE_SIZE_4BPP,
                (void *)(BG_CHAR_BLOCK(2) +
                         LUCKY_WHEELS_MACHINE_TILE * TILE_SIZE_4BPP + j),
                DMA_COPY_WORDS(TILE_SIZE_4BPP / 4), REG_DMA3);
        j += TILE_SIZE_4BPP;
      }
    }
  }
  // Clear BG1's map below the screen, then show the canvas on BG2.
  Iwram_ClearWords(
      BG_SCREEN_BLOCK(5) + DISPLAY_TILE_HEIGHT * BG_SCREEN_WIDTH * sizeof(u16),
      BG_SCREEN_SIZE - DISPLAY_TILE_HEIGHT * BG_SCREEN_WIDTH * sizeof(u16));
  j = 0;
  for (k = 0; k != DISPLAY_TILE_HEIGHT; k++) {
    for (i = 0; i != BG_SCREEN_WIDTH; i++, j += 2) {
      if (k < LUCKY_WHEELS_CANVAS_TOP_ROW || k > LUCKY_WHEELS_CANVAS_BOTTOM_ROW)
        *(volatile u16 *)(BG_SCREEN_BLOCK(6) + j) =
            LUCKY_WHEELS_CANVAS_CLEAR_TILE;
      else
        *(volatile u16 *)(BG_SCREEN_BLOCK(6) + j) =
            (k - LUCKY_WHEELS_CANVAS_TOP_ROW) * BG_SCREEN_WIDTH + i +
            LUCKY_WHEELS_CANVAS_FIRST_TILE;
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
  // DEAD: a window round the opening, replaced at once by the whole screen;
  // the ROM keeps both stores.
  REG_WIN0H = WIN_RANGE(40, 200);
  REG_WIN0V = WIN_RANGE(24, 120);
  REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
  REG_WIN0V = WIN_RANGE(0, DISPLAY_HEIGHT);
  REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
  REG_WIN1V = WIN_RANGE(0, DISPLAY_HEIGHT);

  {
    // The palettes as loaded, kept at the start of the effect's sheet to fade
    // from: the background's, then the sprites'.
    u16 *bgShade = (u16 *)effect->sheet;
    u16 *shade = (u16 *)(effect->sheet + BG_PLTT_SIZE);
    s32 size = LUCKY_WHEELS_CANVAS_SIZE;

    wheels->state = LUCKY_WHEELS_BETTING;
    wheels->cursor = 0;
    wheels->spins = 0;
    effect->frame = 0;
    wheels->timer = 0;
    Dma_Set(Resource_GetTableEntry(RESOURCE_JUPITER_DJINN_SHEET),
            (void *)BG_PLTT, DMA_COPY_WORDS(LUCKY_WHEELS_SPARK_COLORS / 2),
            REG_DMA3);
    BG_PLTT[LUCKY_WHEELS_LINE_COLOR] = RGB(11, 28, 11);
    BG_PLTT[LUCKY_WHEELS_LINE_LIGHT] = RGB(22, 31, 22);
    entry = Resource_GetTableEntry(RESOURCE_LUCKY_WHEELS_ICONS);
    Dma_Set(entry, (void *)OBJ_PLTT,
            DMA_COPY_WORDS(LUCKY_WHEELS_ICON_COLORS / 2), REG_DMA3);
    entry += LUCKY_WHEELS_ICON_COLORS * 2;
    Decompress_TaggedLz(entry, tiles);
    Dma_Set(tiles, (void *)OBJ_VRAM0, DMA_COPY_WORDS(6960), REG_DMA3);
    entry = Resource_GetTableEntry(RESOURCE_LUCKY_WHEELS_JACKPOT);
    Dma_Set(entry, (void *)&OBJ_PLTT_COLOR(LUCKY_WHEELS_JACKPOT_PALETTE, 0),
            DMA_COPY_WORDS(PALETTE_BYTES / 4), REG_DMA3);
    entry += PALETTE_BYTES;
    Decompress_TaggedLz(entry, tiles);
    Dma_Set(tiles, (void *)LUCKY_WHEELS_JACKPOT_TILES,
            LUCKY_WHEELS_JACKPOT_COPY, REG_DMA3);
    WindowSystem_InitFieldFar();
    LuckyWheels_SetUpBetting();

    for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
      struct Reel *reel = &wheels->reels[i];

      reel->position = LUCKY_WHEELS_SYMBOL_HEIGHT / 2;
      reel->held = 0;
      reel->stop = -1;
      for (k = 0; k != LUCKY_WHEELS_REEL_SYMBOLS; k++)
        // One of the plain symbols, all numbered below the wild one.
        reel->symbols[k] = Random16() % LUCKY_WHEELS_WILD_SYMBOL;
    }
    // Then eight different places on each wheel take one of each plain symbol
    // and three moons, so every symbol is on every wheel.
    for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
      struct Reel *reel = &wheels->reels[i];

      for (j = 0; j != LUCKY_WHEELS_PLACED_SYMBOLS; j++) {
        picks[j] = Random16() % LUCKY_WHEELS_REEL_SYMBOLS;
        for (k = 0; k != j; k++) {
          if (picks[j] == picks[k]) {
            j--;
            break;
          }
        }
      }
      for (k = 0; k != LUCKY_WHEELS_PLACED_SYMBOLS; k++) {
        s32 symbol = k;

        if (symbol > LUCKY_WHEELS_WILD_SYMBOL)
          symbol = LUCKY_WHEELS_WILD_SYMBOL;
        reel->symbols[picks[k]] = symbol;
      }
    }

    BattleEffect_BuildBlitterFar(HEAP_SLOT_RAM_CODE, 8, 7, 3, 2);
    draw[0] = gHeapSlots[HEAP_SLOT_RAM_CODE];
    BattleEffect_BuildBlitterFar(HEAP_SLOT_RAM_CODE_PAIRED, 8, 7, 3, 3);
    draw[1] = gHeapSlots[HEAP_SLOT_RAM_CODE_PAIRED];
    Iwram_FillWords(canvas, size, 0);
    Dma_Set(canvas, (void *)LUCKY_WHEELS_CANVAS_TILES,
            DMA_COPY_WORDS(LUCKY_WHEELS_CANVAS_SIZE / 4), REG_DMA3);
    Dma_Set((void *)BG_PLTT, bgShade, DMA_COPY_WORDS(BG_PLTT_SIZE / 4),
            REG_DMA3);
    Dma_Set((void *)OBJ_PLTT, shade, DMA_COPY_WORDS(BG_PLTT_SIZE / 4),
            REG_DMA3);
    Palette_ScaleRgb555(shade, (u16 *)OBJ_PLTT, 0, BG_PLTT_COLORS);
    Palette_ScaleRgb555(bgShade, (u16 *)BG_PLTT, 0, BG_PLTT_COLORS);
    REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
                  DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON;
    if (PartyInventory_CountItemFar(ITEM_GAME_TICKET) == 1) {
      struct Window *window = Window_CreateFar(
          6, 16, 18, 3, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);

      wheels->window = window;
      Text_DrawMessageAsTilesFar(MsgSlotsControls, window, 0, 0);
    } else {
      struct Window *window = Window_CreateFar(
          LUCKY_WHEELS_HELP_WINDOW_X, 16, LUCKY_WHEELS_HELP_WINDOW_WIDTH, 4,
          WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
      s32 message;

      wheels->window = window;
      // "Press up or down to change value" above the controls.
      message = MsgSlotsChangeBet;
      Text_DrawMessageAsTilesFar(message, window, 0, 0);
      Text_DrawMessageAsTilesFar(message + 1, wheels->window, 0, 8);
    }
    effect->transferPending = 0;
    Task_Add(LuckyWheels_Update, TASK_PASS_VBLANK);
    Task_Add(LuckyWheels_TransferCanvas, TASK_PASS_VBLANK);

    for (frame = 0; wheels->state != LUCKY_WHEELS_DONE; frame++) {
      if (frame <= LUCKY_WHEELS_FADE_FRAMES) {
        Palette_ScaleRgb555(shade, (u16 *)OBJ_PLTT,
                            frame * (FIXED_ONE / LUCKY_WHEELS_FADE_FRAMES),
                            BG_PLTT_COLORS);
        Palette_ScaleRgb555(bgShade, (u16 *)BG_PLTT,
                            frame * (FIXED_ONE / LUCKY_WHEELS_FADE_FRAMES),
                            BG_PLTT_COLORS);
      }
      if (wheels->state == LUCKY_WHEELS_SHOWING_WIN) {
        s32 phase =
            frame % (LUCKY_WHEELS_CYCLE_PALETTES * LUCKY_WHEELS_CYCLE_FRAMES);

        if (phase < LUCKY_WHEELS_CYCLE_FRAMES)
          LuckyWheels_StepSparkColors(RESOURCE_MERCURY_DJINN_SHEET);
        else if (phase < LUCKY_WHEELS_CYCLE_FRAMES * 2)
          LuckyWheels_StepSparkColors(RESOURCE_VENUS_DJINN_SHEET);
        else if (phase < LUCKY_WHEELS_CYCLE_FRAMES * 3)
          LuckyWheels_StepSparkColors(RESOURCE_EMBER_STREAK_SHEET);
        else if (phase < LUCKY_WHEELS_CYCLE_FRAMES * 4)
          LuckyWheels_StepSparkColors(RESOURCE_LIME_PALETTE);
        else
          LuckyWheels_StepSparkColors(RESOURCE_JUPITER_DJINN_SHEET);
        if (wheels->timer < LUCKY_WHEELS_FADE_FRAMES)
          LuckyWheels_DarkenStep();
        if (wheels->timer > LUCKY_WHEELS_FADE_FRAMES && (frame & 7) == 0) {
          s32 x = (Random16() & 127) + 56;
          s32 y = (Random16() & 31) + 48;
          struct Particle *spark =
              &LUCKY_WHEELS_SPARKS[(frame / 8 & 3) * LUCKY_WHEELS_BURST_SPARKS];

          for (j = 0; j != LUCKY_WHEELS_BURST_SPARKS; j++) {
            s32 speed;
            s32 angle;

            speed = (Random16() & 255) + 64;
            angle = Random16() & TRIG_ANGLE_MASK;
            spark->x = INTtoFIXED(x);
            spark->y = INTtoFIXED(y);
            spark->velocityX = Trig_Sin(angle) * speed >> 6;
            spark->velocityY = -(Trig_Cos(angle) * speed) >> 6;
            spark->variant = (Random16() & 15) + 16;
            spark++;
          }
        }
        for (j = 0; j != LUCKY_WHEELS_SPARK_COUNT; j++) {
          struct Particle *spark = &LUCKY_WHEELS_SPARKS[j];

          if (spark->variant > 0) {
            s32 x;

            spark->variant--;
            x = spark->x;
            if (x >= 0 && x <= toFIXED(256) - 1) {
              s32 y = spark->y;

              if (y <= toFIXED(128) - 1) {
                if (y >= 0) {
                  s32 drawX = FIXEDtoINT(x);
                  s32 drawY = FIXEDtoINT(y);
                  s32 size =
                      spark->variant / LUCKY_WHEELS_SPARK_SHRINK_FRAMES + 1;

                  draw[j & 1](canvas,
                              sheet + gLuckyWheels_SparkPictures[size - 1],
                              drawX - size, drawY - size, size * 2, size * 2);
                }
              }
            }
            spark->x += spark->velocityX;
            spark->y += spark->velocityY;
            spark->velocityX = spark->velocityX * 60 / DAMPING_SCALE;
            spark->velocityY = spark->velocityY * 60 / DAMPING_SCALE;
          }
        }
      }
      // The paylines bet on while betting, or those won, blinking.
      if (wheels->state == LUCKY_WHEELS_BETTING ||
          wheels->state == LUCKY_WHEELS_WON) {
        for (j = 0; j != LUCKY_WHEELS_PAYLINES; j++)
          litLines[j] = 0;
        if (wheels->state == LUCKY_WHEELS_BETTING) {
          litLines[LUCKY_WHEELS_MIDDLE_LINE] = 1;
          if (wheels->bet > 1) {
            litLines[LUCKY_WHEELS_MIDDLE_LINE + 1] = 1;
            litLines[LUCKY_WHEELS_MIDDLE_LINE - 1] = 1;
          }
          if (wheels->bet > 2) {
            litLines[LUCKY_WHEELS_MIDDLE_LINE + 2] = 1;
            litLines[LUCKY_WHEELS_MIDDLE_LINE - 2] = 1;
          }
          if (wheels->bet > 3) {
            litLines[LUCKY_WHEELS_MIDDLE_LINE + 3] = 1;
            litLines[LUCKY_WHEELS_MIDDLE_LINE - 3] = 1;
          }
        } else if ((frame & 7) <= 3) {
          for (j = 0; j != LUCKY_WHEELS_PAYLINES; j++)
            litLines[j] = wheels->winningLines[j];
        }
        // Each three pixels thick, its middle the lighter green.
        for (j = 0; j != 3; j++) {
          s32 color = j != 1;

          color = LUCKY_WHEELS_LINE_LIGHT - color;
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
      effect->transferPending = 1;
      wait(1);
    }

    for (j = 0; j != LUCKY_WHEELS_FADE_FRAMES + 1; j++) {
      Palette_ScaleRgb555(shade, (u16 *)OBJ_PLTT,
                          FIXED_ONE -
                              j * (FIXED_ONE / LUCKY_WHEELS_FADE_FRAMES),
                          BG_PLTT_COLORS);
      Palette_ScaleRgb555(bgShade, (u16 *)BG_PLTT,
                          FIXED_ONE -
                              j * (FIXED_ONE / LUCKY_WHEELS_FADE_FRAMES),
                          BG_PLTT_COLORS);
      wait(1);
    }
    HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    Task_Remove(LuckyWheels_TransferCanvas);
    Task_Remove(LuckyWheels_Update);
    HeapSlot_Release(LUCKY_WHEELS_HEAP_SLOT);
    HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
    HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
    HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
  }
}

/// The LZ packer: a window of PACK_WINDOW_SIZE bytes with a hash chain for each
/// byte value, PACK_LOOKAHEAD bytes read ahead of the one being packed and the
/// PACK_HISTORY behind it kept in the chains, and matches of up to
/// PACK_MATCH_MAX bytes at most PACK_DISTANCE_MAX back.
#define PACK_WINDOW_SIZE 1024
#define PACK_WINDOW_MASK (PACK_WINDOW_SIZE - 1)
#define PACK_BYTE_VALUES 256
#define PACK_LOOKAHEAD 672
#define PACK_HISTORY 60
#define PACK_DISTANCE_MAX 63
/// A match of up to 16 bytes takes two bytes; a longer one three, its length
/// less 17 in the third, up to PACK_MATCH_MAX.
#define PACK_SHORT_MATCH_MAX 16
#define PACK_MATCH_MAX 272
/// A group's flag byte is read from its top bit down, a bit an item.
#define PACK_FIRST_FLAG (1 << 7)
/// A match's code, a halfword written high byte first: its distance's bits
/// from 8 up in bits 12 to 15, its length less one in bits 8 to 11 for a short
/// match, and its distance's low byte.
#define PACK_CODE_DISTANCE_HIGH (~0xfff)
#define PACK_CODE_LENGTH 0xf00
#define PACK_CODE_DISTANCE_LOW 0xff

/// Empties the LZ packer's hash chains and numbers its window nodes.
void LzPack_ResetChains(void) {
  s32 index;
  struct LzPackNode **bucket;

  for (index = 0; index < PACK_WINDOW_SIZE; index++) {
    gLzPackState->nodes[index].slot = index;
    gLzPackState->nodes[index].previous = NULL;
  }
  bucket = gLzPackState->bucket;
  for (index = 0; index < PACK_BYTE_VALUES; index++) {
    *bucket++ = NULL;
  }
}

/// Adds window position index to the chain of the byte it holds.
void LzPack_InsertSlotNode(s32 index) {
  struct LzPackState *state = gLzPackState;

  state->nodes[index].previous =
      (struct LzPackNode *)&state->bucket[state->bucketBySlot[index]];
  state->nodes[index].next = state->bucket[state->bucketBySlot[index]];
  state->bucket[state->bucketBySlot[index]] = &state->nodes[index];
  if (state->nodes[index].next != NULL)
    state->nodes[index].next->previous = &state->nodes[index];
}

/// Removes window position slot from its chain.
void LzPack_RemoveSlotNode(s32 slot) {
  if (gLzPackState->nodes[slot].previous != NULL) {
    if (gLzPackState->nodes[slot].next != NULL)
      gLzPackState->nodes[slot].next->previous =
          gLzPackState->nodes[slot].previous;
    gLzPackState->nodes[slot].previous->next = gLzPackState->nodes[slot].next;
  }
}

/// Reads count more input bytes into the window from position start, dropping
/// the bytes they replace from their chains; past the end of the input the
/// positions are marked empty (-1).
void LzPack_ReadInput(s32 start, s32 count, const u8 *input) {
  s32 current = 0;

  if (current < count) {
    u32 mask = PACK_WINDOW_MASK;
    // The byte PACK_HISTORY behind the one being packed leaves its chain.
    u32 removal = start + PACK_WINDOW_SIZE - PACK_LOOKAHEAD - PACK_HISTORY;

    do {
      u32 slot = start + current;
      struct LzPackState *state;
      u32 readOffset;
      u32 nextOffset;
      u8 value;

      LzPack_RemoveSlotNode(removal & mask);
      state = gLzPackState;
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
  if (current < count) {
    u32 mask = PACK_WINDOW_MASK;

    do {
      u32 slot = (start + current) & mask;
      struct LzPackState *state;

      LzPack_RemoveSlotNode(slot);
      state = gLzPackState;
      current++;
      state->bucketBySlot[slot] = -1;
    } while (current < count);
  }
}

/// Appends the finished group (a flag byte and up to eight items) to the
/// output.
void LzPack_CopyBufferedBytes(u8 *destination) {
  struct LzPackState *state = gLzPackState;
  s32 index;

  for (index = 0; index != state->outCount; index++) {
    destination[state->outTotal] = state->outBuffer[index];
    state->outTotal++;
  }
}

/// Finds the longest earlier match (up to PACK_MATCH_MAX bytes, at most
/// PACK_DISTANCE_MAX back) for the bytes at window position start; matchLength
/// stays 1 for none.
static __inline__ void LzPack_FindWindowMatch(s32 start) {
  struct LzPackNode *node;
  s32 len;

  gLzPackState->matchLength = 1;
  if (gLzPackState->bucketBySlot[start] == -1) {
    return;
  }
  node = gLzPackState->bucket[gLzPackState->bucketBySlot[start]];
  if (start + PACK_MATCH_MAX > PACK_WINDOW_MASK) {
    while (node != NULL) {
      s32 dist = (start - node->slot) & PACK_WINDOW_MASK;

      if (dist > 0 && dist <= PACK_DISTANCE_MAX) {
        for (len = 1; len < PACK_MATCH_MAX; len++) {
          if (gLzPackState->bucketBySlot[(start + len) & PACK_WINDOW_MASK] !=
              gLzPackState
                  ->bucketBySlot[(node->slot + len) & PACK_WINDOW_MASK]) {
            break;
          }
        }
        if (gLzPackState->matchLength < len) {
          gLzPackState->matchOffset = dist;
          gLzPackState->matchLength = len;
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
          if (gLzPackState->bucketBySlot[start + len] !=
              gLzPackState
                  ->bucketBySlot[(node->slot + len) & PACK_WINDOW_MASK]) {
            break;
          }
        }
        if (gLzPackState->matchLength < len) {
          gLzPackState->matchOffset = dist;
          gLzPackState->matchLength = len;
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
/// position matches further. A zero match ends the stream.
UNUSED s32 LzPack_PackData(const u8 *input, u8 *dst, s32 size) {
  s32 defer;
  s32 len;
  s32 saveOffset;
  s32 saveLen;
  s32 reach[2];
  s16 code;

  defer = 0;
  gLzPackState = (struct LzPackState *)gMapGridCellBuffer;
  LzPack_ResetChains();
  gLzPackState->inputLimit = size;
  gLzPackState->position = 0;
  gLzPackState->inputCursor = 0;
  gLzPackState->outTotal = 0;
  gLzPackState->flagMask = PACK_FIRST_FLAG;
  gLzPackState->outBuffer[0] = 0;
  gLzPackState->outCount = 1;
  LzPack_ReadInput(0, PACK_LOOKAHEAD, input);

  while (gLzPackState->bucketBySlot[gLzPackState->position] != -1) {
    LzPack_FindWindowMatch(gLzPackState->position);
    if (defer == 0) {
      len = gLzPackState->matchLength;
      if (gLzPackState->matchLength > 1) {
        saveOffset = gLzPackState->matchOffset;
        saveLen = len;
        LzPack_FindWindowMatch((gLzPackState->position + 1) & PACK_WINDOW_MASK);
        if (gLzPackState->matchLength > 2) {
          reach[0] = gLzPackState->matchLength + 1;
          LzPack_FindWindowMatch((gLzPackState->position + len) &
                                 PACK_WINDOW_MASK);
          reach[1] = gLzPackState->matchLength + len;
          if (reach[0] >= reach[1]) {
            saveLen = 1;
            defer = 1;
          }
        }
        gLzPackState->matchOffset = saveOffset;
        gLzPackState->matchLength = saveLen;
      }
    }

    if (gLzPackState->matchLength > 1) {
      defer = 0;
      gLzPackState->outBuffer[0] |= gLzPackState->flagMask;
      if (gLzPackState->matchLength <= PACK_SHORT_MATCH_MAX) {
        code = ((gLzPackState->matchOffset << 4) & PACK_CODE_DISTANCE_HIGH) |
               (gLzPackState->matchOffset & PACK_CODE_DISTANCE_LOW) |
               (((gLzPackState->matchLength - 1) << 8) & PACK_CODE_LENGTH);
        gLzPackState->outBuffer[gLzPackState->outCount++] = (u16)code >> 8;
        gLzPackState->outBuffer[gLzPackState->outCount++] = code;
      } else {
        code = ((gLzPackState->matchOffset << 4) & PACK_CODE_DISTANCE_HIGH) |
               (gLzPackState->matchOffset & PACK_CODE_DISTANCE_LOW);
        gLzPackState->outBuffer[gLzPackState->outCount++] = (u16)code >> 8;
        gLzPackState->outBuffer[gLzPackState->outCount++] = code;
        gLzPackState->outBuffer[gLzPackState->outCount++] =
            gLzPackState->matchLength - (PACK_SHORT_MATCH_MAX + 1);
      }
    } else {
      gLzPackState->outBuffer[gLzPackState->outCount++] =
          gLzPackState->bucketBySlot[gLzPackState->position];
      gLzPackState->matchLength = 1;
    }

    LzPack_ReadInput(gLzPackState->position + PACK_LOOKAHEAD,
                     gLzPackState->matchLength, input);
    gLzPackState->position =
        (gLzPackState->position + gLzPackState->matchLength) & PACK_WINDOW_MASK;
    gLzPackState->flagMask >>= 1;
    if (gLzPackState->flagMask == 0) {
      LzPack_CopyBufferedBytes(dst);
      gLzPackState->flagMask = PACK_FIRST_FLAG;
      gLzPackState->outBuffer[0] = 0;
      gLzPackState->outCount = 1;
    }
  }

  gLzPackState->outBuffer[0] |= gLzPackState->flagMask;
  gLzPackState->outBuffer[gLzPackState->outCount++] = 0;
  gLzPackState->outBuffer[gLzPackState->outCount++] = 0;
  LzPack_CopyBufferedBytes(dst);
  return gLzPackState->outTotal;
}
