/// @file
/// Lucky Wheels, Contigo's game of chance: the player trades game tickets for
/// lines, pulls the lever up to five times and locks wheels to line up
/// matching icons. It draws its own wheels and lines, and keeps an unused LZ
/// packer.

#include "types.h"
#include "macros.h"
#include "sounds.h"
#include "system/task.h"
#include "system/io_reg.h"
#include "system/main_routine_state.h"
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
#include "field/field.h"
#include "field/dice_game.h"
#include "field/field_sequence.h"
#include "battle/battle_effect_state.h"
#include "resource_ids.h"
#include "graphics/graphics.h"
#include "item_ids.h"

void LuckyWheels_Run(void);

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
/// The stop prompt's window opens this many frames into a spin, and its sound
/// plays at LUCKY_WHEELS_STOP_PROMPT_FRAME.
#define LUCKY_WHEELS_STOP_WINDOW_FRAME 4
#define LUCKY_WHEELS_STOP_PROMPT_FRAME 16
/// Where each picture of icons.png starts in sprite tiles, in OAM's 32-byte
/// steps (a 256-colour tile is two): the sprite sheet is 32 pixels wide, so a
/// picture starts at the tile numbered as its row. The six symbols are 16 apart
/// from 0.
#define LUCKY_WHEELS_LAMP_TILES 96         ///< a wheel's lamp, not locked
#define LUCKY_WHEELS_LOCKED_LAMP_TILES 128 ///< a locked wheel's lamp
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
/// The pulls of the lever after the first, before betting again.
#define LUCKY_WHEELS_RESPINS 4
/// The places on each wheel given the symbols 0 to 4 and three wilds.
#define LUCKY_WHEELS_PLACED_SYMBOLS 8
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
/// What a line of each symbol wins, as the prize sign reads it: "Shirts win
/// undershirts, Rings win rings, Boots win boots, Stars and Hearts win you
/// items..." and Moons "can win you anything." Contigo then gives one of three
/// items of the kind.
enum LuckyWheelsPrize {
  LUCKY_WHEELS_PRIZE_UNDERSHIRT, ///< a Mythril, Silk or Running Shirt
  LUCKY_WHEELS_PRIZE_BOOTS,      ///< Hyper, Quick or Fur Boots
  LUCKY_WHEELS_PRIZE_RING,       ///< an Adept, War or Sleep Ring
  LUCKY_WHEELS_PRIZE_HEART_ITEM, ///< a Potion, Vial or Nut
  LUCKY_WHEELS_PRIZE_STAR_ITEM,  ///< a Water of Life, Psy Crystal or Elixir
  LUCKY_WHEELS_PRIZE_ANYTHING    ///< one of the five kinds above
};
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
/// Lucky Wheels' stages (LuckyWheelsState.state).
enum LuckyWheelsStage {
  LUCKY_WHEELS_BETTING = 0,  ///< Up and Down change the bet, A spins, B leaves
  LUCKY_WHEELS_SPINNING = 1, ///< the wheels spin until R or time stops each
  LUCKY_WHEELS_WON = 2,      ///< a payline won: a pause
  LUCKY_WHEELS_SHOWING_WIN = 3,    ///< the win is shown until A
  LUCKY_WHEELS_CHOOSING_LOCKS = 5, ///< choose wheels to lock, or pull the lever
  LUCKY_WHEELS_DONE = 10,          ///< the game ends
  LUCKY_WHEELS_NO_WIN = 11,        ///< no payline won: A to choose locks
  LUCKY_WHEELS_OUT_OF_SPINS =
      20, ///< the fourth spin lost: a pause, then the end
};

/// The machine's lights, its four corners first and then the lights along its
/// frame.
#define LUCKY_WHEELS_MACHINE_LIGHTS 14
#define LUCKY_WHEELS_CORNER_LIGHTS 4

// Lucky Wheels' tables, the same in both games.

/// Where each spark picture (a square 2 to 20 pixels on a side) starts: size s
/// (1 to 10) is a square 2s pixels a side, after the smaller squares' pixels.
const u16 gLuckyWheels_SparkCellOffsets[] = {0,   4,   20,  56,  120,
                                             220, 364, 560, 816, 1140};

/// The prize of each wheel symbol (enum LuckyWheelsPrize), kept in
/// ReelSave.wonPrizes for each winning line.
const u8 gLuckyWheels_SymbolPrizes[] = {
    [LUCKY_WHEELS_SHIRT] = LUCKY_WHEELS_PRIZE_UNDERSHIRT,
    [LUCKY_WHEELS_HEART] = LUCKY_WHEELS_PRIZE_HEART_ITEM,
    [LUCKY_WHEELS_BOOT] = LUCKY_WHEELS_PRIZE_BOOTS,
    [LUCKY_WHEELS_STAR] = LUCKY_WHEELS_PRIZE_STAR_ITEM,
    [LUCKY_WHEELS_RING] = LUCKY_WHEELS_PRIZE_RING,
    [LUCKY_WHEELS_MOON] = LUCKY_WHEELS_PRIZE_ANYTHING,
};

/// The tile of each title letter.
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

/// The positions of the machine's lights, in screen pixels: its four corners
/// first, then the lights along its frame.
const u8 gLuckyWheels_MachineSpriteX[LUCKY_WHEELS_MACHINE_LIGHTS] = {
    2, 0, 232, 232, 32, 32, 200, 200, 28, 28, 208, 208, 219, 219};
const u8 gLuckyWheels_MachineSpriteY[LUCKY_WHEELS_MACHINE_LIGHTS] = {
    2, 152, 0, 152, 8, 104, 8, 104, 118, 140, 118, 140, 6, 108};

/// The advance of each title letter.
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

/// Plays Lucky Wheels: blanks the screen, seeds the random numbers with the
/// play time and runs the game. Returns 0.
s32 LuckyWheels_Start(void) {
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
  gRandomSeed = gMainRoutineState.playTime;
  Sound_PlayFar(MUS_TRY_YOUR_LUCK);
  LuckyWheels_Run();
  return 0;
}

/// Writes count RGB555 colours from src to dst scaled by scale (16.16,
/// FIXED_ONE keeps them). Returns 0.
s32 LuckyWheels_ScaleRgb555(u16 *src, u16 *dst, FIXED scale, s32 count) {
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
      color = (FIXEDtoINT(red) & redMask) | (FIXEDtoINT(green) & greenMask);
      color |= FIXEDtoINT(blue) & blueMask;
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
  struct BattleEffectState *state;
  u8 *buf;

  state = Ram_HeapSlots->battleEffect;
  if (state->transferPending == 1) {
    buf = Ram_HeapSlots->battleCanvas;
    switch (state->transferMode) {
    case CANVAS_COPY_AND_CLEAR:
      Dma_Set(buf, (void *)LUCKY_WHEELS_CANVAS_TILES,
              DMA_COPY_WORDS(LUCKY_WHEELS_CANVAS_SIZE / 4), REG_DMA3);
      Iwram_FillWords(buf, LUCKY_WHEELS_CANVAS_SIZE, state->transferValue);
      break;
    case CANVAS_COPY_AND_FADE:
      // Trails: nonzero colour numbers drop to 1/2 or 3/4.
      if (state->transferValue == CANVAS_FADE_HALF)
        ColorBuffer_HalveNonzero(buf, (u8 *)LUCKY_WHEELS_CANVAS_TILES,
                                 LUCKY_WHEELS_CANVAS_SIZE);
      else
        ColorBuffer_ScaleNonzeroThreeQuarters(buf, LUCKY_WHEELS_CANVAS_TILES,
                                              LUCKY_WHEELS_CANVAS_SIZE);
      break;
    }
    state->transferPending = 0;
    state->vblanksSinceTransfer = 1;
  } else {
    (state->vblanksSinceTransfer)++;
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

/// A line's step along its shorter axis, in 256ths of a pixel.
#define LINE_STEP_ONE (1 << 8)

/// Draws a line from (x0, y0) to (x1, y1) on the 256 x 128 8bpp canvas (32
/// tiles across), writing only over lower colour numbers.
void LuckyWheels_DrawCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color) {
  s32 dx = x1 - x0;
  s32 dy = y1 - y0;
  s32 frac = LINE_STEP_ONE / 2;
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
    step = ABS(x1 - x0) * LINE_STEP_ONE / ABS(y1 - y0);
    x = x0;
    for (i = y0; i != y1; i++) {
      off = (((((u32)i >> 3) << 5) + ((u32)x >> 3)) << 3) + (i & 7);
      off = (off << 3) + (x & 7);
      if (canvas[off] < color)
        canvas[off] = color;
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
    step = ABS(y1 - y0) * LINE_STEP_ONE / ABS(x1 - x0);
    y = y0;
    for (i = x0; i != x1; i++) {
      off = (((((u32)y >> 3) << 5) + ((u32)i >> 3)) << 3) + (y & 7);
      off = (off << 3) + (i & 7);
      if (canvas[off] < color)
        canvas[off] = color;
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

/// The prompt windows (all locked, start, lock and spin a wheel, stop the
/// slots, choose the slots to lock) and the ticket count's digits, placed and
/// sized for each language's words. Lock reads "Stop", and Japanese shows it
/// and Spin in one window.
#if LANGUAGE == LANGUAGE_JAPANESE
#define LUCKY_WHEELS_ALL_LOCKED_X 12
#define LUCKY_WHEELS_ALL_LOCKED_WIDTH 18
#define LUCKY_WHEELS_START_X 18
#define LUCKY_WHEELS_START_WIDTH 12
#elif LANGUAGE == LANGUAGE_ITALIAN
#define LUCKY_WHEELS_ALL_LOCKED_X 10
#define LUCKY_WHEELS_ALL_LOCKED_WIDTH 20
#define LUCKY_WHEELS_START_X 21
#define LUCKY_WHEELS_START_WIDTH 9
#elif LANGUAGE == LANGUAGE_SPANISH
#define LUCKY_WHEELS_ALL_LOCKED_X 6
#define LUCKY_WHEELS_ALL_LOCKED_WIDTH 24
#define LUCKY_WHEELS_START_X 22
#define LUCKY_WHEELS_START_WIDTH 8
#else
#define LUCKY_WHEELS_ALL_LOCKED_X 11
#define LUCKY_WHEELS_ALL_LOCKED_WIDTH 19
#define LUCKY_WHEELS_START_X 16
#define LUCKY_WHEELS_START_WIDTH 14
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
#define LUCKY_WHEELS_LOCK_X 18
#define LUCKY_WHEELS_LOCK_WIDTH 12
#elif LANGUAGE == LANGUAGE_GERMAN
#define LUCKY_WHEELS_LOCK_X 22
#define LUCKY_WHEELS_LOCK_WIDTH 8
#define LUCKY_WHEELS_SPIN_X 21
#define LUCKY_WHEELS_SPIN_WIDTH 9
#elif LANGUAGE == LANGUAGE_ITALIAN
#define LUCKY_WHEELS_LOCK_X 21
#define LUCKY_WHEELS_LOCK_WIDTH 9
#define LUCKY_WHEELS_SPIN_X 21
#define LUCKY_WHEELS_SPIN_WIDTH 9
#elif LANGUAGE == LANGUAGE_SPANISH
#define LUCKY_WHEELS_LOCK_X 22
#define LUCKY_WHEELS_LOCK_WIDTH 8
#define LUCKY_WHEELS_SPIN_X 22
#define LUCKY_WHEELS_SPIN_WIDTH 8
#else
#define LUCKY_WHEELS_LOCK_X 23
#define LUCKY_WHEELS_LOCK_WIDTH 7
#define LUCKY_WHEELS_SPIN_X 23
#define LUCKY_WHEELS_SPIN_WIDTH 7
#endif
#if LANGUAGE == LANGUAGE_FRENCH
#define LUCKY_WHEELS_TICKET_DIGITS 3
#define LUCKY_WHEELS_TICKET_X 56
#else
#define LUCKY_WHEELS_TICKET_DIGITS 2
#define LUCKY_WHEELS_TICKET_X 64
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
#define LUCKY_WHEELS_CHOOSE_LOCKED_X 7
#define LUCKY_WHEELS_CHOOSE_LOCKED_WIDTH 16
#else
#define LUCKY_WHEELS_CHOOSE_LOCKED_X 3
#define LUCKY_WHEELS_CHOOSE_LOCKED_WIDTH 24
#endif
/// The Spanish title's first letter is its last letter's tile turned over both
/// ways, drawn 16 pixels left and 8 down.
#define LUCKY_WHEELS_TITLE_FIRST_TURNED (LANGUAGE == LANGUAGE_SPANISH)
/// The first tile of the title's letters: the Japanese letters take fewer
/// tiles, so they start later and end at the same place.
#if LANGUAGE == LANGUAGE_JAPANESE
#define LUCKY_WHEELS_TITLE_TILES 896
#else
#define LUCKY_WHEELS_TITLE_TILES 880
#endif

/// Lucky Wheels' state: the five reels, the game's stage, cursor, spins and
/// bet, the keys read, the winning lines, the reel sprites, the windows and
/// the scanline offsets that bend the reels.
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
  s32 timer;
  s32 spriteCount;
  s32 winningLines[LUCKY_WHEELS_PAYLINES];
  struct OamWords oam[128];
  struct Window *window;
  struct Window *subWindow;
  u8 unusedAfterSubWindow[8]; ///< no code names it
  u16 scanlineOffsets[160];
#if TLA_PATCH_1
  // The Japanese release has no phase: it closes and reopens its prompt
  // window every frame.
  s32 phase; ///< the prompt shown while choosing locks (LuckyWheelsPrompt)
#endif
};

/// The prompt window LuckyWheelsState.phase says is open, so that it is drawn
/// once rather than every frame. The all-locked prompt draws its two lines on
/// two frames; after a losing spin the first value says the "pull the lever"
/// line is drawn.
enum LuckyWheelsPrompt {
  LUCKY_WHEELS_PROMPT_NONE,
  LUCKY_WHEELS_PROMPT_ALL_LOCKED_FIRST_LINE,
  LUCKY_WHEELS_PROMPT_PULL_LEVER = LUCKY_WHEELS_PROMPT_ALL_LOCKED_FIRST_LINE,
  LUCKY_WHEELS_PROMPT_ALL_LOCKED,
  LUCKY_WHEELS_PROMPT_START,
  LUCKY_WHEELS_PROMPT_LOCK,
  LUCKY_WHEELS_PROMPT_SPIN,
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
/// wheels to lock, showing a win) and builds all 128 sprites: the cursor, the
/// title letters of a win, the frame, the lock lamps, the spin counter, the
/// paylines, the wheel symbols, the lever and the bottom bar.
void LuckyWheels_Update(void) {
  struct ReelEffectState *fx;
  s32 objMode;
  u16 keys;
  u32 dpad;
  s32 tickets;
  s32 allLocked;
  s32 stoppedReels;
  s32 col;
  s32 v;
  s32 x;
  s32 y;
  s32 lever;
  s32 k;
  s32 i;
  struct LuckyWheelsState *state;

  state = (struct LuckyWheelsState *)Ram_HeapSlots->canvasMap;
  fx = (struct ReelEffectState *)Ram_HeapSlots->battleEffect;
  objMode = OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
  state->spriteCount = 0;

  Random16();

  keys = gInput.held;
  dpad = gInput.held & KEYS_DPAD;
  state->pressed = keys & ~state->keys;
  state->direction = dpad;
  if ((state->keys & KEYS_DPAD) == state->direction) {
    if (state->repeat > LUCKY_WHEELS_REPEAT_DELAY)
      state->repeat = LUCKY_WHEELS_REPEAT_DELAY;
    if (state->repeat == 0) {
      state->repeat = LUCKY_WHEELS_REPEAT_INTERVAL;
    } else {
      state->repeat--;
      state->direction = 0;
    }
  } else {
    state->repeat = LUCKY_WHEELS_REPEAT_DELAY;
  }
  state->keys = keys;

  if (gDebugPaused == 0) {
    if (state->state == LUCKY_WHEELS_BETTING) {
      tickets = PartyInventory_CountItemFar(ITEM_GAME_TICKET);
      Text_DrawNumberInWindowFar(tickets - state->bet,
                                 LUCKY_WHEELS_TICKET_DIGITS, state->subWindow,
                                 LUCKY_WHEELS_TICKET_X, 0);
      Text_DrawNumberInWindowFar(state->bet, 2, state->subWindow, 64, 8);
      if (state->pressed & KEY_B) {
        struct ReelSave *save = (struct ReelSave *)gPartyDjinnRecovery;

        state->state = LUCKY_WHEELS_DONE;
        // The player quit while betting.
        save->wonPrizes[0] = WHEELS_PRIZES_NONE_TO_HAND_OUT;
        Window_CloseFar(state->window, 1);
      } else {
        if (state->pressed & KEY_UP) {
          if (state->bet < LUCKY_WHEELS_BET_MAX && tickets > state->bet) {
            state->bet++;
            Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          } else {
            Sound_PlayFar(SE_MENU_CANCEL);
          }
        }
        if (state->pressed & KEY_DOWN) {
          if (state->bet > 1) {
            state->bet--;
            Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          } else {
            Sound_PlayFar(SE_MENU_CANCEL);
          }
        }
        REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_DARKEN | BLDCNT_TGT2_ALL;
        REG_BLDALPHA = BLEND_ALPHA(16, 0);
        if (state->pressed & KEY_A) {
          state->state = LUCKY_WHEELS_SPINNING;
          fx->frame = 0;
          Window_CloseFar(state->window, 1);
          for (k = 0; k != state->bet; k++)
            PartyInventory_RemoveFar(ITEM_GAME_TICKET);
          Window_CloseFar(state->subWindow, 1);
          Sound_PlayFar(SE_LUCKY_WHEELS_SPIN);
        }
      }
    } else if (state->state == LUCKY_WHEELS_CHOOSING_LOCKS) {
      allLocked = 0;
      state->timer++;
      for (k = 0; k != LUCKY_WHEELS_REEL_COUNT && state->reels[k].held != 0;
           k++)
        ;
      if (k == LUCKY_WHEELS_REEL_COUNT)
        allLocked = 1;
      if (state->pressed & KEY_A) {
        state->timer = 0;
        fx->frame = 0;
        if (state->spins == LUCKY_WHEELS_RESPINS) {
          state->spins = 0;
          state->cursor = 0;
          state->state = LUCKY_WHEELS_BETTING;
          for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
            state->reels[i].held = 0;
            state->reels[i].stop = -1;
          }
        } else if (state->cursor < LUCKY_WHEELS_LEVER) {
          Sound_PlayFar(SE_LUCKY_WHEELS_HOLD);
          state->reels[state->cursor].held ^= 1;
        } else if (allLocked == 0) {
          Sound_PlayFar(SE_LUCKY_WHEELS_SPIN);
          state->state = LUCKY_WHEELS_SPINNING;
          state->cursor = 0;
          for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++)
            state->reels[i].stop = -1;
          state->spins++;
        } else {
          Sound_PlayFar(SE_MENU_CANCEL);
        }
      } else {
        if (state->direction & KEY_RIGHT) {
          state->cursor = (state->cursor + 1) % (LUCKY_WHEELS_LEVER + 1);
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        }
        if (state->direction & KEY_LEFT) {
          state->cursor =
              (state->cursor + LUCKY_WHEELS_LEVER) % (LUCKY_WHEELS_LEVER + 1);
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        }
      }

      if (state->state == LUCKY_WHEELS_CHOOSING_LOCKS) {
#if TLA_PATCH_1
        if (state->cursor == LUCKY_WHEELS_LEVER) {
          if (allLocked != 0) {
            if (state->phase != LUCKY_WHEELS_PROMPT_ALL_LOCKED_FIRST_LINE &&
                state->phase != LUCKY_WHEELS_PROMPT_ALL_LOCKED) {
              Window_CloseFar(state->window, 1);
              state->window =
                  Window_CreateFar(LUCKY_WHEELS_ALL_LOCKED_X, 0,
                                   LUCKY_WHEELS_ALL_LOCKED_WIDTH, 4, 6);
              Text_DrawMessageAsTilesFar((s32)MsgSlotsAllHeld, state->window, 0,
                                         0);
              state->phase = LUCKY_WHEELS_PROMPT_ALL_LOCKED_FIRST_LINE;
            } else if (state->phase ==
                       LUCKY_WHEELS_PROMPT_ALL_LOCKED_FIRST_LINE) {
              Text_DrawMessageAsTilesFar((s32)MsgSlotsCannotPull, state->window,
                                         0, 8);
              state->phase = LUCKY_WHEELS_PROMPT_ALL_LOCKED;
            }
          } else {
            if (state->phase != LUCKY_WHEELS_PROMPT_START) {
              Window_CloseFar(state->window, 1);
              state->window = Window_CreateFar(LUCKY_WHEELS_START_X, 0,
                                               LUCKY_WHEELS_START_WIDTH, 3, 6);
              Text_DrawMessageAsTilesFar((s32)MsgSlotsStart, state->window, 0,
                                         0);
            }
            state->phase = LUCKY_WHEELS_PROMPT_START;
          }
        } else if (state->reels[state->cursor].held == 0) {
          if (state->phase != LUCKY_WHEELS_PROMPT_LOCK) {
            Window_CloseFar(state->window, 1);
            state->window = Window_CreateFar(LUCKY_WHEELS_LOCK_X, 0,
                                             LUCKY_WHEELS_LOCK_WIDTH, 3, 6);
            Text_DrawMessageAsTilesFar(MsgSlotsHold, state->window, 0, 0);
          }
          state->phase = LUCKY_WHEELS_PROMPT_LOCK;
        } else {
          if (state->phase != LUCKY_WHEELS_PROMPT_SPIN) {
            Window_CloseFar(state->window, 1);
            state->window = Window_CreateFar(LUCKY_WHEELS_SPIN_X, 0,
                                             LUCKY_WHEELS_SPIN_WIDTH, 3, 6);
            Text_DrawMessageAsTilesFar((s32)MsgSlotsRelease, state->window, 0,
                                       0);
          }
          state->phase = LUCKY_WHEELS_PROMPT_SPIN;
        }
#else
        Window_CloseFar(state->window, 1);
        if (state->cursor == LUCKY_WHEELS_LEVER) {
          if (allLocked != 0) {
            state->window =
                Window_CreateFar(LUCKY_WHEELS_ALL_LOCKED_X, 0,
                                 LUCKY_WHEELS_ALL_LOCKED_WIDTH, 3, 6);
            Text_DrawMessageAsTilesFar((s32)MsgSlotsAllHeld, state->window, 0,
                                       0);
          } else {
            state->window = Window_CreateFar(LUCKY_WHEELS_START_X, 0,
                                             LUCKY_WHEELS_START_WIDTH, 3, 6);
            Text_DrawMessageAsTilesFar((s32)MsgSlotsStart, state->window, 0, 0);
          }
        } else {
          state->window = Window_CreateFar(LUCKY_WHEELS_LOCK_X, 0,
                                           LUCKY_WHEELS_LOCK_WIDTH, 3, 6);
          if (state->reels[state->cursor].held == 0)
            Text_DrawMessageAsTilesFar(MsgSlotsHold, state->window, 0, 0);
          else
            Text_DrawMessageAsTilesFar((s32)MsgSlotsRelease, state->window, 0,
                                       0);
        }
#endif
      } else {
        Window_CloseFar(state->window, 1);
      }
    } else if (state->state == LUCKY_WHEELS_WON) {
      state->timer++;
      objMode = 0;
      if (state->timer == LUCKY_WHEELS_WIN_PAUSE) {
        state->state = LUCKY_WHEELS_SHOWING_WIN;
        Sound_PlayFar(MUS_SUPER_LUCKY);
        state->timer = 0;
        REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        REG_BLDALPHA = BLEND_ALPHA(16, 16);
        fx->transferMode = CANVAS_COPY_AND_FADE;
        fx->transferValue = CANVAS_FADE_THREE_QUARTERS;
      }
    } else if (state->state == LUCKY_WHEELS_SHOWING_WIN) {
      state->timer++;
      objMode = 0;
      if (state->pressed & KEY_A) {
        state->state = LUCKY_WHEELS_DONE;
        Sound_PlayFar(SE_MENU_CONFIRM);
      }
    } else if (state->state == LUCKY_WHEELS_NO_WIN) {
#if TLA_PATCH_1
      if (state->phase == LUCKY_WHEELS_PROMPT_NONE) {
        state->phase = LUCKY_WHEELS_PROMPT_PULL_LEVER;
        Text_DrawMessageAsTilesFar(MsgSlotsPullLever, state->window, 0, 8);
      }
#endif
      if (state->pressed & KEY_A) {
        state->state = LUCKY_WHEELS_CHOOSING_LOCKS;
#if TLA_PATCH_1
        state->phase = LUCKY_WHEELS_PROMPT_NONE;
#endif
        Sound_PlayFar(SE_MENU_CONFIRM);
        Window_CloseFar(state->window, 1);
      }
    } else if (state->state == LUCKY_WHEELS_OUT_OF_SPINS) {
      state->timer++;
      if (state->timer == LUCKY_WHEELS_LOSS_PAUSE)
        state->state = LUCKY_WHEELS_DONE;
    } else if (state->state != LUCKY_WHEELS_DONE) {
      if (state->timer == LUCKY_WHEELS_STOP_WINDOW_FRAME) {
        state->window = Window_CreateFar(LUCKY_WHEELS_STOP_X, 17,
                                         LUCKY_WHEELS_STOP_WIDTH, 3, 6);
        Text_DrawMessageAsTilesFar(MsgSlotsStopReels, state->window, 0, 0);
      }
      if (state->timer == LUCKY_WHEELS_STOP_PROMPT_FRAME)
        Sound_PlayFar(SE_LUCKY_WHEELS_STOP_PROMPT);
      if (state->timer > LUCKY_WHEELS_LEVER_FRAMES) {
        if (fx->frame >= LUCKY_WHEELS_AUTO_STOP_FRAMES ||
            (state->pressed & KEY_R)) {
          fx->frame = 0;
          for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
            if (state->reels[i].held == 0 && state->reels[i].stop == -1) {
              state->reels[i].stop = (Random16() & 3) + 4;
              Sound_PlayFar(SE_LUCKY_WHEELS_STOP);
              break;
            }
          }
        }
      }

      for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
        if (state->reels[i].stop > 0)
          state->reels[i].stop--;
      }

      stoppedReels = 0;
      for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
        if (state->reels[i].held == 1)
          stoppedReels++;
        else if (state->reels[i].stop == 0 &&
                 (state->reels[i].position &
                  (LUCKY_WHEELS_SYMBOL_HEIGHT - 1)) ==
                     LUCKY_WHEELS_SYMBOL_HEIGHT / 2)
          stoppedReels++;
      }

      if (stoppedReels == LUCKY_WHEELS_REEL_COUNT) {
        struct ReelSave *save = (struct ReelSave *)gPartyDjinnRecovery;
        s32 hits = 0;

        for (col = 0; col != LUCKY_WHEELS_PAYLINES; col++) {
          s32 mixed;
          s32 found;

          found = -1;
          mixed = 0;
          state->winningLines[col] = 0;
          if (col > LUCKY_WHEELS_MIDDLE_LINE - state->bet &&
              col < state->bet + LUCKY_WHEELS_MIDDLE_LINE) {
            for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
              s32 sym;

              if (col == 0)
                sym = state->reels[i].symbols[(i -
                                               state->reels[i].position /
                                                   LUCKY_WHEELS_SYMBOL_HEIGHT +
                                               LUCKY_WHEELS_REEL_SYMBOLS + 1) %
                                              LUCKY_WHEELS_REEL_SYMBOLS];
              else if (col == LUCKY_WHEELS_PAYLINES - 1)
                sym = state->reels[i].symbols[(-i -
                                               state->reels[i].position /
                                                   LUCKY_WHEELS_SYMBOL_HEIGHT +
                                               LUCKY_WHEELS_REEL_SYMBOLS + 5) %
                                              LUCKY_WHEELS_REEL_SYMBOLS];
              else
                sym = state->reels[i].symbols[(col -
                                               state->reels[i].position /
                                                   LUCKY_WHEELS_SYMBOL_HEIGHT +
                                               LUCKY_WHEELS_REEL_SYMBOLS) %
                                              LUCKY_WHEELS_REEL_SYMBOLS];
              if (sym != LUCKY_WHEELS_WILD_SYMBOL) {
                if (found == -1)
                  found = sym;
                else if (found != sym)
                  mixed = 1;
              }
            }
            if (mixed == 0) {
              state->winningLines[col] = 1;
              save->wonPrizes[hits] = gLuckyWheels_SymbolPrizes[found];
              hits++;
            }
          }
        }
        state->timer = 0;
        if (hits != 0) {
          save->wonPrizes[hits] = WHEELS_PRIZES_END;
          state->state = LUCKY_WHEELS_WON;
          Sound_PlayFar(SE_THUNDER);
          fx->transferMode = CANVAS_COPY_AND_CLEAR;
          fx->transferValue = 0;
          REG_BLDCNT = 0;
          Window_CloseFar(state->window, 1);
        } else {
          state->state = LUCKY_WHEELS_NO_WIN;
#if TLA_PATCH_1
          state->phase = LUCKY_WHEELS_PROMPT_NONE;
#endif
          Window_CloseFar(state->window, 1);
          if (state->spins < LUCKY_WHEELS_RESPINS) {
            s32 message;

            state->window =
                Window_CreateFar(LUCKY_WHEELS_CHOOSE_LOCKED_X, 16,
                                 LUCKY_WHEELS_CHOOSE_LOCKED_WIDTH, 4, 6);
            message = (s32)MsgSlotsChooseHeld;
            Text_DrawMessageAsTilesFar(message, state->window, 0, 0);
            Text_DrawMessageAsTilesFar(message + 1, state->window, 0, 8);
          }
          if (state->spins == LUCKY_WHEELS_RESPINS) {
            s32 left = PartyInventory_CountItemFar(ITEM_GAME_TICKET);
            struct Window *window;
            s32 message;

            // DEAD: both arms set the out-of-spins state, so the ticket count
            // cannot change it; the ROM keeps the compare and both stores.
            if (left > 0)
              state->state = LUCKY_WHEELS_OUT_OF_SPINS;
            else
              state->state = LUCKY_WHEELS_OUT_OF_SPINS;
            if (state->bet > left)
              state->bet = left;
            state->spins = 0;
            state->cursor = 0;
            for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
              state->reels[i].held = 0;
              state->reels[i].stop = -1;
            }
            fx->transferMode = CANVAS_COPY_AND_CLEAR;
            fx->transferValue = 0;
            REG_BLDCNT = 0;
            window = Window_CreateFar(18, 0, 12, 4, 6);
            state->subWindow = window;
            // "Game ticket" above "Bet".
            message = MsgSlotsBet;
            Text_DrawMessageAsTilesFar(message, window, 0, 8);
            Text_DrawMessageAsTilesFar(message - 1, state->subWindow, 0, 0);
          }
        }
      }

      if (state->state == LUCKY_WHEELS_SPINNING) {
        for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
          struct Reel *reel = &state->reels[i];

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
      (fx->frame)++;
      state->timer++;
    }
  }

  if (state->state == LUCKY_WHEELS_CHOOSING_LOCKS) {
    x = state->cursor * 36 + 36;
    y = 128;
    v = 0;
    if ((state->timer & 15) <= 7)
      v = 1;
    if (state->cursor == LUCKY_WHEELS_LEVER) {
      x = LUCKY_WHEELS_LEVER_X;
      y = 32;
    }
    state->oam[state->spriteCount].attr01 =
        OBJ_X(x - 12) | objMode | (y + 8) | OBJ_SIZE_32X16 | OBJ_256_COLORS;
    state->oam[state->spriteCount].attr2 =
        v * SPRITE_TILES_8BPP(32, 16) + LUCKY_WHEELS_ARROW_TILES;
    state->spriteCount++;
    state->oam[state->spriteCount].attr01 = OBJ_X(x + 12) | objMode | (y + 8) |
                                            OBJ_SIZE_32X16 | OBJ_256_COLORS |
                                            OBJ_HFLIP;
    state->oam[state->spriteCount].attr2 =
        v * SPRITE_TILES_8BPP(32, 16) + LUCKY_WHEELS_ARROW_TILES;
    state->spriteCount++;
    state->oam[state->spriteCount].attr01 =
        OBJ_X(x) | objMode | y | OBJ_SIZE_32X32 | OBJ_256_COLORS;
    state->oam[state->spriteCount].attr2 = LUCKY_WHEELS_HAND_TILES;
    state->spriteCount++;
  }

  if (state->state == LUCKY_WHEELS_SHOWING_WIN) {
    for (i = 0; i != LUCKY_WHEELS_TITLE_LETTERS; i++) {
      struct Particle *p = &fx->particles[i];

      if (LUCKY_WHEELS_TITLE_FIRST_TURNED && i == 0)
        state->oam[state->spriteCount].attr01 =
            OBJ_X((PIXEL_PART(p->x) + 512 - 16) & OBJ_X_MASK) | objMode |
            ((PIXEL_PART(p->y) + 256 + 8) & OBJ_Y_MASK) | OBJ_SIZE_32X32 |
            OBJ_HFLIP | OBJ_VFLIP;
      else
        state->oam[state->spriteCount].attr01 =
            OBJ_X(PIXEL_PART(p->x)) | objMode |
            ((PIXEL_PART(p->y) + 256) & OBJ_Y_MASK) | OBJ_SIZE_32X32;
      state->oam[state->spriteCount].attr2 =
          (gLuckyWheels_TitleLetterTiles[i] * SPRITE_TILES(32, 32) +
           LUCKY_WHEELS_TITLE_TILES) |
          OBJ_PALETTE(LUCKY_WHEELS_JACKPOT_PALETTE);
#if TLA_PATCH_1
      // The Japanese release draws the letters without moving them.
      p->y += p->velocityY;
      p->velocityY += toFIXED(1) / 4;
      if (state->timer % 256 == i * 4 + 200) {
        p->velocityY = toFIXED(6);
        p->variant = 0;
      }
      if (p->y > toFIXED(64)) {
        p->y = toFIXED(64);
        if (p->variant <= 1)
          p->velocityY = -p->velocityY / 2;
        p->variant++;
      }
#endif
      state->spriteCount++;
    }
  }

  for (k = 0; k != LUCKY_WHEELS_MACHINE_LIGHTS; k++) {
    state->oam[state->spriteCount].attr01 =
        OBJ_X(gLuckyWheels_MachineSpriteX[k]) | objMode |
        gLuckyWheels_MachineSpriteY[k] | OBJ_SIZE_32X16 | OBJ_256_COLORS;
    if (k < LUCKY_WHEELS_CORNER_LIGHTS)
      state->oam[state->spriteCount].attr2 =
          OBJ_PRIORITY(1) | LUCKY_WHEELS_CORNER_LIGHT_TILES;
    else
      state->oam[state->spriteCount].attr2 =
          OBJ_PRIORITY(1) | LUCKY_WHEELS_FRAME_LIGHT_TILES;
    state->spriteCount++;
  }

  for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
    if (state->reels[i].held == 0) {
      state->oam[state->spriteCount].attr01 =
          objMode | OBJ_X(i * 36 + 32) | 124 | OBJ_SIZE_32X32 | OBJ_256_COLORS;
      state->oam[state->spriteCount].attr2 =
          OBJ_PRIORITY(1) | LUCKY_WHEELS_LAMP_TILES;
    } else {
      state->oam[state->spriteCount].attr01 =
          objMode | OBJ_X(i * 36 + 32) | 124 | OBJ_SIZE_32X32 | OBJ_256_COLORS;
      state->oam[state->spriteCount].attr2 =
          OBJ_PRIORITY(1) | LUCKY_WHEELS_LOCKED_LAMP_TILES;
    }
    state->spriteCount++;
  }

  // One sprite for each pull of the lever, the first and its re-pulls; the
  // current pull's takes other tiles.
  for (i = 0; i != LUCKY_WHEELS_RESPINS + 1; i++) {
    state->oam[state->spriteCount].attr01 =
        objMode | OBJ_X(i * 16 + 32) | 3 | OBJ_SIZE_32X16 | OBJ_256_COLORS;
    if (i == state->spins)
      state->oam[state->spriteCount].attr2 =
          (i * 2 * SPRITE_TILES_8BPP(32, 16) + LUCKY_WHEELS_PULL_TILES) |
          OBJ_PRIORITY(1);
    else
      state->oam[state->spriteCount].attr2 =
          (i * 2 * SPRITE_TILES_8BPP(32, 16) + LUCKY_WHEELS_PULL_TILES +
           SPRITE_TILES_8BPP(32, 16)) |
          OBJ_PRIORITY(1);
    state->spriteCount++;
  }

  for (col = 0; col != LUCKY_WHEELS_PAYLINES; col++) {
    state->oam[state->spriteCount].attr01 =
        OBJ_X((516 - ((col & 1) << 3)) & OBJ_X_MASK) | objMode |
        (col * LUCKY_WHEELS_SYMBOL_HEIGHT + 5) | OBJ_SIZE_32X32 |
        OBJ_256_COLORS;
    if (col > LUCKY_WHEELS_MIDDLE_LINE - state->bet &&
        col < state->bet + LUCKY_WHEELS_MIDDLE_LINE)
      state->oam[state->spriteCount].attr2 =
          OBJ_PRIORITY(1) | LUCKY_WHEELS_LIT_LINE_TILES;
    else
      state->oam[state->spriteCount].attr2 =
          OBJ_PRIORITY(1) | LUCKY_WHEELS_LINE_TILES;
    state->spriteCount++;
  }

  for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
    struct Reel *reel = &state->reels[i];

    for (col = 0; col != LUCKY_WHEELS_PAYLINES; col++) {
      s32 symbolY = col * LUCKY_WHEELS_SYMBOL_HEIGHT +
                    reel->position % LUCKY_WHEELS_SYMBOL_HEIGHT;
      s32 index = state->spriteCount;

      symbolY += 4;
      state->oam[index].attr01 =
          symbolY | OBJ_X(i * 32 + 40) | OBJ_SIZE_32X16 | OBJ_256_COLORS;
      state->oam[state->spriteCount].attr2 =
          reel->symbols[(col - reel->position / LUCKY_WHEELS_SYMBOL_HEIGHT +
                         LUCKY_WHEELS_REEL_SYMBOLS) %
                        LUCKY_WHEELS_REEL_SYMBOLS] *
              SPRITE_TILES_8BPP(32, 16) |
          OBJ_PRIORITY(2);
      state->spriteCount++;
    }
  }

  // The lever, pulled down and let back up, then bouncing as it settles.
  lever = LUCKY_WHEELS_LEVER_Y;
  if (state->state == LUCKY_WHEELS_SPINNING) {
    if (state->timer < LUCKY_WHEELS_LEVER_PULL_FRAMES)
      lever =
          FIXEDtoINT(Trig_Sin(state->timer * (DEGtoANG(180) /
                                              LUCKY_WHEELS_LEVER_PULL_FRAMES)) *
                     LUCKY_WHEELS_LEVER_PULL) +
          LUCKY_WHEELS_LEVER_Y;
    else if (state->timer < LUCKY_WHEELS_LEVER_FRAMES)
      lever =
          FIXEDtoINT(Trig_Sin(state->timer * (TRIG_TURN / 16) - TRIG_TURN * 3) *
                     4) +
          LUCKY_WHEELS_LEVER_Y;
  }
  state->oam[state->spriteCount].attr01 = lever | objMode |
                                          OBJ_X(LUCKY_WHEELS_LEVER_X) |
                                          OBJ_SIZE_32X16 | OBJ_256_COLORS;
  state->oam[state->spriteCount].attr2 =
      OBJ_PRIORITY(1) | LUCKY_WHEELS_LEVER_TILES;
  state->spriteCount++;

  // The lever's slot, in eight pieces from its top: two of its own, four alike
  // and two for its bottom.
  for (k = 0; k != LUCKY_WHEELS_LEVER_SLOT_PIECES; k++) {
    state->oam[state->spriteCount].attr01 =
        objMode | (k * 16 + LUCKY_WHEELS_LEVER_SLOT_Y) |
        OBJ_X(LUCKY_WHEELS_LEVER_SLOT_X) | OBJ_SIZE_32X16 | OBJ_256_COLORS;
    if (k == 0)
      state->oam[state->spriteCount].attr2 =
          OBJ_PRIORITY(1) | LUCKY_WHEELS_LEVER_SLOT_TILES;
    else if (k == 1)
      state->oam[state->spriteCount].attr2 =
          OBJ_PRIORITY(1) |
          (LUCKY_WHEELS_LEVER_SLOT_TILES + SPRITE_TILES_8BPP(32, 16));
    else if (k == 6)
      state->oam[state->spriteCount].attr2 =
          OBJ_PRIORITY(1) |
          (LUCKY_WHEELS_LEVER_SLOT_TILES + 3 * SPRITE_TILES_8BPP(32, 16));
    else if (k == 7)
      state->oam[state->spriteCount].attr2 =
          OBJ_PRIORITY(1) |
          (LUCKY_WHEELS_LEVER_SLOT_TILES + 4 * SPRITE_TILES_8BPP(32, 16));
    else
      state->oam[state->spriteCount].attr2 =
          OBJ_PRIORITY(1) |
          (LUCKY_WHEELS_LEVER_SLOT_TILES + 2 * SPRITE_TILES_8BPP(32, 16));
    state->spriteCount++;
  }

  while (state->spriteCount != OAM_SIZE / sizeof(struct OamWords)) {
    state->oam[state->spriteCount].attr01 =
        OBJ_X(DISPLAY_WIDTH) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
    state->oam[state->spriteCount].attr2 = 0;
    state->spriteCount++;
  }
}

/// The title letters LuckyWheels_FinishFrame bounces in every edition: the
/// Japanese title's eight. The other editions also bounce every letter as
/// LuckyWheels_Update draws it, so there the first eight move twice a frame and
/// the Spanish title's ninth once.
// XXX: how the Spanish title's ninth letter bounces beside the others; a
// recording of a Spanish win would settle it.
#define LUCKY_WHEELS_BOUNCED_LETTERS 8

/// The end of Lucky Wheels' frame: while a win shows it bounces the title
/// letters, then restarts the HBlank DMA of the scanline offsets into BLDY and
/// copies the frame's sprites to OAM.
void LuckyWheels_FinishFrame(void) {
  struct LuckyWheelsState *state;
  struct ReelEffectState *fx;
  volatile u16 *dma;
  s32 i;

  state = (struct LuckyWheelsState *)Ram_HeapSlots->canvasMap;
  fx = (struct ReelEffectState *)Ram_HeapSlots->battleEffect;
  if (state->state == LUCKY_WHEELS_SHOWING_WIN) {
    for (i = 0; i != LUCKY_WHEELS_BOUNCED_LETTERS; i++) {
      struct Particle *p = &fx->particles[i];

      p->y += p->velocityY;
      p->velocityY += toFIXED(1) / 4;
      if (state->timer % 256 == i * 4 + 200) {
        p->velocityY = toFIXED(6);
        p->variant = 0;
      }
      if (p->y > toFIXED(64)) {
        p->y = toFIXED(64);
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
  Dma_Set(state->scanlineOffsets, (void *)&REG_BLDY,
          DMA_HBLANK_HALFWORD_PER_LINE, (volatile u32 *)dma);
  if (state->spriteCount > 0)
    Dma_Set(state->oam, OAM, DMA_COPY_WORDS(state->spriteCount * 2), REG_DMA3);
}

/// The left edge of the title's first letter: the Spanish title has nine
/// letters and starts further left.
#if LANGUAGE == LANGUAGE_SPANISH
#define LUCKY_WHEELS_TITLE_X 8
#else
#define LUCKY_WHEELS_TITLE_X 24
#endif

/// The sparks of the jackpot, kept in the map cell buffer.
#define LUCKY_WHEELS_SPARKS ((struct Particle *)Ram_MapCellBuffer)

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
  struct LuckyWheelsState *state;
  struct ReelEffectState *reelEffectState;
  struct Particle *letter;
  s32 left;
  s32 i;
  s32 angle;
  struct Window *window;
  s32 message;

  state = (struct LuckyWheelsState *)Ram_HeapSlots->canvasMap;
  reelEffectState = (struct ReelEffectState *)Ram_HeapSlots->battleEffect;
  left = 0;
  // Twice the sparks the win uses are cleared.
  for (i = 0; i != 2 * LUCKY_WHEELS_SPARK_COUNT; i++)
    LUCKY_WHEELS_SPARKS[i].variant = 0;

  letter = reelEffectState->particles;
  for (i = 0; i != LUCKY_WHEELS_TITLE_LETTERS; i++) {
    letter[i].x = INTtoFIXED(left + LUCKY_WHEELS_TITLE_X);
    left += gLuckyWheels_TitleLetterWidths[i];
    letter[i].y = -toFIXED(32) - i * toFIXED(8);
    letter[i].velocityY = 0;
    letter[i].variant = 0;
  }

  for (i = 0; i != DISPLAY_HEIGHT; i++)
    state->scanlineOffsets[i] = 0;

  // The wheels darken toward their top and bottom edges, as a drum turns away:
  // BLDY falls from LUCKY_WHEELS_SHADE_LEVEL at each edge to 0 over a quarter
  // turn of the cosine.
  for (i = 0; i != LUCKY_WHEELS_SHADE_LINES; i++) {
    angle = i * (DEGtoANG(90) / LUCKY_WHEELS_SHADE_LINES);
    state->scanlineOffsets[LUCKY_WHEELS_WHEELS_TOP + i] =
        FIXEDtoINT((u32)(Trig_Cos(angle) * LUCKY_WHEELS_SHADE_LEVEL));
    state->scanlineOffsets[LUCKY_WHEELS_WHEELS_BOTTOM - i] =
        FIXEDtoINT((u32)(Trig_Cos(angle) * LUCKY_WHEELS_SHADE_LEVEL));
  }

  state->spins = 0;
  state->state = LUCKY_WHEELS_BETTING;
  state->cursor = 0;
  reelEffectState->transferMode = CANVAS_COPY_AND_CLEAR;
  reelEffectState->transferValue = 0;
  REG_BLDCNT = 0;
  window = Window_CreateFar(18, 0, 12, 4, 6);
  state->subWindow = window;
  // "Game ticket" above "Bet".
  message = MsgSlotsBet;
  Text_DrawMessageAsTilesFar(message, window, 0, 8);
  Text_DrawMessageAsTilesFar(message - 1, state->subWindow, 0, 0);
}

/// The help window is narrower in Japanese, and one row shorter there when it
/// shows only the controls. The jackpot banner's tiles are the title's.
#if LANGUAGE == LANGUAGE_JAPANESE
#define LUCKY_WHEELS_HELP_WINDOW_X 6
#define LUCKY_WHEELS_HELP_WINDOW_WIDTH 18
#define LUCKY_WHEELS_CONTROLS_WINDOW_HEIGHT 3
#define LUCKY_WHEELS_JACKPOT_COPY DMA_COPY_WORDS(1024)
#else
#define LUCKY_WHEELS_HELP_WINDOW_X 2
#define LUCKY_WHEELS_HELP_WINDOW_WIDTH 26
#define LUCKY_WHEELS_CONTROLS_WINDOW_HEIGHT 4
#define LUCKY_WHEELS_JACKPOT_COPY DMA_COPY_WORDS(1152)
#endif
#define LUCKY_WHEELS_JACKPOT_TILES                                             \
  (OBJ_VRAM0 + LUCKY_WHEELS_TITLE_TILES * TILE_SIZE_4BPP)

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
  s32 picks[LUCKY_WHEELS_PLACED_SYMBOLS];
  s32 litLines[LUCKY_WHEELS_PAYLINES];
  DrawRectangle draw[2];
  u8 *sheet;
  void *canvas;
  struct ReelEffectState *reelEffectState;
  struct LuckyWheelsState *state;
  struct ReelSave *save;
  u8 *entry;
  u8 *tiles;
  s32 frame;
  u32 count;
  s32 i;
  s32 j;
  s32 k;

  sheet = HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, 1550);
  canvas = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, LUCKY_WHEELS_CANVAS_SIZE);
  reelEffectState = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_EFFECT, 31612);
  state = HeapSlot_AllocEwram(HEAP_SLOT_CANVAS_MAP,
                              sizeof(struct LuckyWheelsState));
  tiles = Ram_MapCellBuffer;
  save = (struct ReelSave *)gPartyDjinnRecovery;
  Resource_ReservedNoOp(RESOURCE_BATTLE_EFFECT_FAR_CALLS);
  save->wonPrizes[0] = WHEELS_PRIZES_END;
  state->repeat = 0;
  state->bet = 1;
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
  entry = Resource_GetTableEntry(RESOURCE_SLOTS_MACHINE_PICTURE);
  Dma_Set(entry, (void *)&BG_PLTT_COLOR(LUCKY_WHEELS_MACHINE_PALETTE, 0),
          DMA_COPY_WORDS(8), REG_DMA3);
  entry += PALETTE_BYTES;
  Decompress_TaggedLz(entry, tiles);
  j = 0;
  for (k = 0; k != DISPLAY_TILE_HEIGHT; k++) {
    for (i = 0; i != DISPLAY_TILE_WIDTH; i++) {
      if (!LUCKY_WHEELS_IN_OPENING(i, k)) {
        Dma_Set(tiles + (k * DISPLAY_TILE_WIDTH + i) * TILE_SIZE_4BPP,
                (void *)(BG_CHAR_BLOCK(2) +
                         LUCKY_WHEELS_MACHINE_TILE * TILE_SIZE_4BPP + j),
                DMA_COPY_WORDS(8), REG_DMA3);
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
    u16 *bgShade = reelEffectState->bgShade;
    u16 *shade = reelEffectState->spriteShade;
    s32 size = LUCKY_WHEELS_CANVAS_SIZE;

    state->state = LUCKY_WHEELS_BETTING;
    state->cursor = 0;
    state->spins = 0;
    reelEffectState->frame = 0;
    state->timer = 0;
    Dma_Set(Resource_GetTableEntry(RESOURCE_JUPITER_DJINN_SHEET),
            (void *)BG_PLTT, DMA_COPY_WORDS(LUCKY_WHEELS_SPARK_COLORS / 2),
            REG_DMA3);
    *(u16 *)&BG_PLTT[LUCKY_WHEELS_LINE_COLOR] = RGB(11, 28, 11);
    *(u16 *)&BG_PLTT[LUCKY_WHEELS_LINE_LIGHT] = RGB(22, 31, 22);
    entry = Resource_GetTableEntry(RESOURCE_SLOTS_ICONS);
    Dma_Set(entry, (void *)OBJ_PLTT,
            DMA_COPY_WORDS(LUCKY_WHEELS_ICON_COLORS / 2), REG_DMA3);
    entry += LUCKY_WHEELS_ICON_COLORS * 2;
    Decompress_TaggedLz(entry, tiles);
    Dma_Set(tiles, (void *)OBJ_VRAM0, DMA_COPY_WORDS(6960), REG_DMA3);
    entry = Resource_GetTableEntry(RESOURCE_SLOTS_JACKPOT_BANNER);
    Dma_Set(entry, (void *)&OBJ_PLTT_COLOR(LUCKY_WHEELS_JACKPOT_PALETTE, 0),
            DMA_COPY_WORDS(8), REG_DMA3);
    entry += PALETTE_BYTES;
    Decompress_TaggedLz(entry, tiles);
    Dma_Set(tiles, (void *)LUCKY_WHEELS_JACKPOT_TILES,
            LUCKY_WHEELS_JACKPOT_COPY, REG_DMA3);
    WindowSystem_InitFieldFar();
    LuckyWheels_SetUpBetting();

    for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
      struct Reel *reel = &state->reels[i];

      reel->position = LUCKY_WHEELS_SYMBOL_HEIGHT / 2;
      reel->held = 0;
      reel->stop = -1;
      for (k = 0; k != LUCKY_WHEELS_REEL_SYMBOLS; k++)
        // One of the plain symbols, all numbered below the wild one.
        reel->symbols[k] = Random16() % LUCKY_WHEELS_WILD_SYMBOL;
    }
    // Then eight different places on each wheel take symbols 0 to 4 and three
    // wilds, so every symbol is on every wheel.
    for (i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
      struct Reel *reel = &state->reels[i];

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
    Dma_Set(canvas, (void *)LUCKY_WHEELS_CANVAS_TILES,
            DMA_COPY_WORDS(LUCKY_WHEELS_CANVAS_SIZE / 4), REG_DMA3);
    Dma_Set((void *)BG_PLTT, bgShade, DMA_COPY_WORDS(BG_PLTT_SIZE / 4),
            REG_DMA3);
    Dma_Set((void *)OBJ_PLTT, shade, DMA_COPY_WORDS(BG_PLTT_SIZE / 4),
            REG_DMA3);
    LuckyWheels_ScaleRgb555(shade, (u16 *)OBJ_PLTT, 0, BG_PLTT_COLORS);
    LuckyWheels_ScaleRgb555(bgShade, (u16 *)BG_PLTT, 0, BG_PLTT_COLORS);
    REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
                  DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON;
    if (PartyInventory_CountItemFar(ITEM_GAME_TICKET) == 1) {
      struct Window *window = Window_CreateFar(
          LUCKY_WHEELS_HELP_WINDOW_X, 16, LUCKY_WHEELS_HELP_WINDOW_WIDTH,
          LUCKY_WHEELS_CONTROLS_WINDOW_HEIGHT, 6);

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
    reelEffectState->transferPending = 0;
    Task_Add(LuckyWheels_Update, TASK_PASS_FRAME);
    Task_Add(LuckyWheels_FinishFrame, TASK_PASS_VBLANK);
    Task_Add(LuckyWheels_TransferCanvas, TASK_PASS_VBLANK);

    for (frame = 0; state->state != LUCKY_WHEELS_DONE; frame++) {
      if (frame <= LUCKY_WHEELS_FADE_FRAMES) {
        LuckyWheels_ScaleRgb555(shade, (u16 *)OBJ_PLTT,
                                frame * (FIXED_ONE / LUCKY_WHEELS_FADE_FRAMES),
                                BG_PLTT_COLORS);
        LuckyWheels_ScaleRgb555(bgShade, (u16 *)BG_PLTT,
                                frame * (FIXED_ONE / LUCKY_WHEELS_FADE_FRAMES),
                                BG_PLTT_COLORS);
      }
      if (state->state == LUCKY_WHEELS_SHOWING_WIN) {
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
        if (state->timer < LUCKY_WHEELS_FADE_FRAMES)
          LuckyWheels_DarkenStep();
        if (state->timer > LUCKY_WHEELS_FADE_FRAMES && (frame & 7) == 0) {
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
                              sheet + gLuckyWheels_SparkCellOffsets[size - 1],
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
      if (state->state == LUCKY_WHEELS_BETTING ||
          state->state == LUCKY_WHEELS_WON) {
        for (j = 0; j != LUCKY_WHEELS_PAYLINES; j++)
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
      reelEffectState->transferPending = 1;
      wait(1);
    }

    for (j = 0; j != LUCKY_WHEELS_FADE_FRAMES + 1; j++) {
      LuckyWheels_ScaleRgb555(shade, (u16 *)OBJ_PLTT,
                              FIXED_ONE -
                                  j * (FIXED_ONE / LUCKY_WHEELS_FADE_FRAMES),
                              BG_PLTT_COLORS);
      LuckyWheels_ScaleRgb555(bgShade, (u16 *)BG_PLTT,
                              FIXED_ONE -
                                  j * (FIXED_ONE / LUCKY_WHEELS_FADE_FRAMES),
                              BG_PLTT_COLORS);
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
void LzPack_ResetSlotBuckets(void) {
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
void LzPack_ConsumeSlotBytes(s32 start, s32 count, const u8 *input) {
  s32 limit = count;
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
  if (current < limit) {
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
  gLzPackState = (struct LzPackState *)Ram_MapCellBuffer;
  LzPack_ResetSlotBuckets();
  gLzPackState->inputLimit = size;
  gLzPackState->position = 0;
  gLzPackState->inputCursor = 0;
  gLzPackState->outTotal = 0;
  gLzPackState->flagMask = PACK_FIRST_FLAG;
  gLzPackState->outBuffer[0] = 0;
  gLzPackState->outCount = 1;
  LzPack_ConsumeSlotBytes(0, PACK_LOOKAHEAD, input);

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

    LzPack_ConsumeSlotBytes(gLzPackState->position + PACK_LOOKAHEAD,
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
