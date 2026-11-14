#ifndef ALCHEMY_MODULE_WINDOW_WINDOW_H
#define ALCHEMY_MODULE_WINDOW_WINDOW_H

/// @file
/// What games/tla/src/window/ declares for the files that use it, written once.

#include "types.h"
#include "system/io_reg.h"
#include "window_tile.h"

struct NameCursorMotion;
struct Window;

void Message_CloseFinishedWindows(void);

s32 Message_AllTyped(void);

void Text_DrawMessage(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

// The text canvas tiles in VRAM: 120 4bpp tiles from tile 296, enough for the
// interior of a full-width window six tiles tall.
#define TEXT_CANVAS_VRAM ((void *)0x06002500)
#define TEXT_CANVAS_SIZE (120 * TILE_SIZE_4BPP)
#define TEXT_CANVAS_FIRST_TILE 296

/// The UI tilemap in VRAM (BG screen block 4), which WindowSystem_UploadTilemap
/// copies the work block's tilemap to.
#define WINDOW_TILEMAP_VRAM ((u8 *)0x06002000)

/// The window palette's background colour, behind its text.
#define WINDOW_BACKGROUND_COLOR 4
/// Eight 4bpp pixels of WINDOW_BACKGROUND_COLOR, for filling the text canvas
/// or a tile a word at a time.
#define TEXT_CANVAS_BACKGROUND_WORD 0x44444444

/// Two copies of a tilemap entry, for filling a run of entries a word at a
/// time.
#define WINDOW_TILE_PAIR(entry) ((u32)(entry) << 16 | (entry))

/// A tilemap entry's tile number, the bits below its flip bits and palette
/// number in the hardware's screen entry.
#define TILE_NUMBER_MASK 0x3ff
/// The tilemap entry flip bits.
#define TILE_HFLIP 0x400
#define TILE_VFLIP 0x800

/// The window border's tiles.
enum {
  WINDOW_TILE_BLANK = WINDOW_TILE(0),
  /// The frame a window flashes with as it swells (Window_DrawFlashBorder),
  /// which Window_ShowFlashFrame loads over tiles 8 to 15.
  WINDOW_TILE_FLASH_TOP_LEFT = WINDOW_TILE(8),
  WINDOW_TILE_FLASH_TOP = WINDOW_TILE(9),
  WINDOW_TILE_FLASH_TOP_RIGHT = WINDOW_TILE(10),
  WINDOW_TILE_FLASH_BOTTOM_LEFT = WINDOW_TILE(11),
  WINDOW_TILE_FLASH_BOTTOM = WINDOW_TILE(12),
  WINDOW_TILE_FLASH_BOTTOM_RIGHT = WINDOW_TILE(13),
  WINDOW_TILE_FLASH_LEFT = WINDOW_TILE(14),
  WINDOW_TILE_FLASH_RIGHT = WINDOW_TILE(15),
  WINDOW_TILE_BORDER_TOP_LEFT = WINDOW_TILE(16),
  WINDOW_TILE_BORDER_TOP = WINDOW_TILE(17),
  WINDOW_TILE_BORDER_TOP_RIGHT = WINDOW_TILE(18),
  WINDOW_TILE_BORDER_BOTTOM_LEFT = WINDOW_TILE(19),
  WINDOW_TILE_BORDER_BOTTOM = WINDOW_TILE(20),
  WINDOW_TILE_BORDER_BOTTOM_RIGHT = WINDOW_TILE(21),
  WINDOW_TILE_BORDER_LEFT = WINDOW_TILE(22),
  WINDOW_TILE_BORDER_RIGHT = WINDOW_TILE(23),
  WINDOW_TILE_BORDER_ROUND_CORNER = WINDOW_TILE(28),
  WINDOW_TILE_WINDOW_FILL = WINDOW_TILE(32),
  // Divider lines (Window_DrawDividerLine) and their joins.
  WINDOW_TILE_DIVIDER_TEE_DOWN =
      WINDOW_TILE(9), ///< a vertical line leaving below
  WINDOW_TILE_DIVIDER_TEE_UP =
      WINDOW_TILE(10), ///< a vertical line leaving above
  WINDOW_TILE_DIVIDER_TEE_RIGHT =
      WINDOW_TILE(11), ///< a horizontal line leaving right
  WINDOW_TILE_DIVIDER_TEE_LEFT =
      WINDOW_TILE(12), ///< a horizontal line leaving left
  WINDOW_TILE_DIVIDER_CROSS = WINDOW_TILE(13),
  WINDOW_TILE_DIVIDER_HORIZONTAL = WINDOW_TILE(14),
  WINDOW_TILE_DIVIDER_VERTICAL = WINDOW_TILE(15),
  WINDOW_TILE_BORDER_TOP_TEE = WINDOW_TILE(24), ///< the top edge meeting a line
  WINDOW_TILE_BORDER_BOTTOM_TEE =
      WINDOW_TILE(25), ///< the bottom edge meeting a line
  WINDOW_TILE_BORDER_LEFT_TEE =
      WINDOW_TILE(26), ///< the left edge meeting a line
  WINDOW_TILE_BORDER_RIGHT_TEE =
      WINDOW_TILE(27), ///< the right edge meeting a line
  /// In battle, the bottom border tiles' lower rows, which
  /// WindowSystem_InitBattle copies to tiles 128 to 130.
  WINDOW_TILE_BATTLE_BORDER_BOTTOM_LEFT = WINDOW_TILE(128),
  WINDOW_TILE_BATTLE_BORDER_BOTTOM = WINDOW_TILE(129),
  WINDOW_TILE_BATTLE_BORDER_BOTTOM_RIGHT = WINDOW_TILE(130),
  /// In battle, the tile the command row fills its empty panels with, and
  /// Window_EraseBorderRect below row 16.
  WINDOW_TILE_COMMAND_PANEL = WINDOW_TILE(127),
};

/// How a window sprite moves (WindowSprite.active), run each frame by
/// WindowSprite_UpdateAll, as The Broken Seal's; 0 is a free window sprite and
/// WINDOW_SPRITE_MOTION_PAGE_ARROW (2) is window.h's.
enum {
  WINDOW_SPRITE_MOTION_STILL = 1,
  WINDOW_SPRITE_MOTION_WOBBLE = 4, ///< swings along a small arc
  WINDOW_SPRITE_MOTION_SHAKE = 5,  ///< jitters by a pixel every other frame
  WINDOW_SPRITE_MOTION_POP = 6,    ///< shown at double size, then normal size
  WINDOW_SPRITE_MOTION_SPIN = 7,   ///< turns while circling its place
  WINDOW_SPRITE_MOTION_SWELL = 9,  ///< pulses between 1 and 1.5 times its size
  WINDOW_SPRITE_MOTION_SWELL_HALF = 10, ///< the same at half the scale
  WINDOW_SPRITE_MOTION_SHRINK =
      11, ///< shrinks from 1.5 times to its size, once
  WINDOW_SPRITE_MOTION_SHRINK_HALF = 12, ///< the same at half the scale
  WINDOW_SPRITE_MOTION_HIDDEN = 13,      ///< not drawn
  // A hop and back, each a way of its own here where The Broken Seal's 14
  // and 16 hop down as 15 does.
  WINDOW_SPRITE_MOTION_HOP_RIGHT = 14,
  WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE = 15,
  WINDOW_SPRITE_MOTION_HOP_LEFT = 16,
  WINDOW_SPRITE_MOTION_HOP_UP = 17,
  WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT = 18,
  /// Still, and drawn only while within 64 pixels of the screen.
  WINDOW_SPRITE_MOTION_ON_SCREEN = 19,
};

/// Text colours (Text_SetColor), named as both the window palette and the basic
/// sprite palette show them.
enum TextColor {
  TEXT_COLOR_RED = 2,
  TEXT_COLOR_YELLOW = 4,
  TEXT_COLOR_PINK =
      9, ///< in the basic colours' palette; the window palette's white
  TEXT_COLOR_GREY = 14,
  TEXT_COLOR_WHITE = 15, ///< a message's colour at start
};

/// The flags of a UI window (struct Window.flags), which Window_Create takes as
/// its attributes.
enum {
  WINDOW_FLAG_OPEN = 0x1,    ///< the window slot is in use
  WINDOW_FLAG_INSTANT = 0x2, ///< opens and closes at once, with no zoom
  /// text is drawn into a canvas of bitmap tiles (Window_MapTextCanvasTiles)
  WINDOW_FLAG_TEXT_CANVAS = 0x8,
  /// the canvas has a border and a filled background; without it the canvas is
  /// transparent and borderless
  WINDOW_FLAG_BORDERED_CANVAS = 0x20,
  WINDOW_FLAG_OUTLINED_TEXT = 0x40, ///< the canvas text gets an outline
  /// when a new page resizes the window, its bottom edge stays put
  WINDOW_FLAG_ANCHOR_BOTTOM = 0x80,
  /// when a new page resizes the window, its left edge stays put
  WINDOW_FLAG_FIXED_X = 0x100,
};

struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void WindowSprite_RedrawSavedRect(struct Window *window);

void MessageArgument_ClearAll(void);

extern const u8 gMenu_AnimatedCursorTiles[];

extern const u8 gOwnerStatus_HpString[];
extern const u8 gOwnerStatus_SlashString[];
extern const u8 gOwnerStatus_PpString[];

void Text_SetColor(s32 param);

void Message_DecodeRaw(s32 message, u16 *text, s32 capacity);

s32 Text_GetWideStringWidth(u16 *text);

s32 PauseMenu_Choose(s32 initial);

s32 Save_ToChosenFile(void);

s32 SettingsMenu_Run(void);

void Icon_LoadWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                          s32 palette, s32 reuse);

s32 BattleParty_ChooseReserve(s32 slot);

void BattleDjinn_CountStandbyFar(s32 kind, u8 *counts);

void Window_MarkVisibleTextTiles(void);

s32 Djinn_GetGiveTutorialMessage(void);

void WindowSprite_PrepareForRedraw(struct Window *);

extern s16 gData_020036d8;
extern s16 gData_02005350;

s32 Save_ScanFileSummaries(void);

extern s32 gKeyState;

void Text_UseExtraTiles(s32 alt);

s32 Message_CopyExpanded(s32 key, u16 *destination, u32 capacity);
s32 Message_DecodeMode1(s32 message);
void Message_DecodeAppend(s32 message);

void MenuCursor_AdvanceFar(struct NameCursorMotion *cursor);
void MenuCursor_MoveTowardTargetFar(struct NameCursorMotion *cursor);

void Portrait_CloseWindow(s32 id);

u8 *String_FormatNumber(u8 *buffer, s32 input, s32 width);

void Window_DrawDividerLine(struct Window *win, u32 x1, u32 y1, u32 x2, u32 y2);
void Text_DrawStringInWindow(u8 *text, s32 arg1, u32 x, u32 y);
void Text_DrawNumberInWindow(s32 value, s32 width, s32 output, s32 x, s32 y);

/// The kinds of message argument (MessageArgument_Push), which the message
/// codes such as {argument_item_name} read.
enum {
  MESSAGE_ARGUMENT_NONE = 0, ///< an empty place in the argument table
  MESSAGE_ARGUMENT_ACTOR = 1,
  MESSAGE_ARGUMENT_ITEM = 2,
  MESSAGE_ARGUMENT_CLASS = 3,
  MESSAGE_ARGUMENT_ABILITY = 4,
  MESSAGE_ARGUMENT_NUMBER = 5,
  MESSAGE_ARGUMENT_LOCATION = 6,
};

void MessageArgument_Push(u32 value, u32 kind);

void FileList_PlacePartySprites(void);

extern const u8 gResource_FixedBlockBTiles[];

void Text_FillCanvasBackground(void);

void PartyStatus_OpenAndWait(void);

#endif
