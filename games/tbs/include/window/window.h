#ifndef ALCHEMY_WINDOW_H
#define ALCHEMY_WINDOW_H

/// @file
/// Windows, the messages that type into them, and the window renderer's
/// work.

#include "window/window.h"
#include "window/screens.h"
#include "types.h"
#include "tbs_edition.h"
#include "static_assert.h"
#include "graphics/render_input.h"
#include "system/heap.h"

/// Where the windows are drawn (WindowWork.battleMode).
enum WindowBattleMode {
  WINDOW_FIELD = 0,  ///< on the field; the field setup clears the block
  WINDOW_BATTLE = 1, ///< in battle, from WindowSystem_InitBattle
  /// In battle, while BattleMessage_Open decodes a battle message: the
  /// Japanese decoder then sets names off with spaces and style codes.
  WINDOW_BATTLE_MESSAGE = 2,
};

/// A window: a bordered rectangle of the tilemap with its window sprites, its
/// opening or closing animation and the rectangle it last drew.
struct Window {
  struct WindowSpriteList sprites; ///< the sprites the window carries
  u16 width;                       ///< tiles
  u16 height;                      ///< tiles
  u16 x;                           ///< tiles
  u16 y;                           ///< tiles
  u16 opened; ///< 1 once opened, 0 once freed; nothing reads it
  /// The animation a message's TEXT_WINDOW_STATE code sets.
  u16 animation;
  /// 0 while a message is typing into the window; 1 once its text has
  /// ended or closed the window, which frees the message for the next text.
  u16 state;
  u16 flags; ///< WINDOW_FLAG_ flags; 0 once closing or free
  /// The zoom's stage: counts down from 7 to 0 while opening, and up from 0 to
  /// duration while closing.
  u16 zoomStage;
  s16 duration;       ///< frames of the zoom under way; 0 when none
  u16 previousX;      ///< tiles
  u16 previousY;      ///< tiles
  u16 previousWidth;  ///< tiles
  u16 previousHeight; ///< tiles
};

STATIC_ASSERT(sizeof(struct Window) == 0x24);
STATIC_ASSERT(offsetof(struct Window, width) == 8);
STATIC_ASSERT(offsetof(struct Window, zoomStage) == 0x18);
STATIC_ASSERT(offsetof(struct Window, previousHeight) == 0x22);

/// A message, one of three, which types decoded text into a window a few
/// characters a frame and runs its control codes.
struct Message {
  struct Window *window; ///< the window typed into; NULL when free
  u16 x;                 ///< pen, 8.8 fixed-point pixels
  u16 y;                 ///< pen, 8.8 fixed-point pixels
  /// Extra advance per line, in 1/256 pixels: after every character in
  /// Japanese, after a space in the other editions.
  u16 spacing[4];
  u16 line;        ///< line of the page, indexes spacing; at most 3
  u16 textIndex;   ///< next code in WindowWork.textBuffer
  u16 countdown;   ///< frames left of a pause, a page wait or a swell
  u16 colour;      ///< text colour; 15 by default
  u16 outline;     ///< outline colour; 0 for none
  u16 lineSpacing; ///< 10 by default
  u16 transition;  ///< frames left of the canvas scrolling up a line
  u16 startX;      ///< pen x each new line returns to, 8.8 fixed-point pixels
  u16 ended;       ///< 1 once a TEXT_END code is reached
  u16 glyphDelay;  ///< frames to wait after a glyph while no button is held
  u16 flags;       ///< nonzero keeps the window's size from page to page
  u16 unused;
};

STATIC_ASSERT(sizeof(struct Message) == 0x28);
STATIC_ASSERT(offsetof(struct Message, textIndex) == 0x12);
STATIC_ASSERT(offsetof(struct Message, countdown) == 0x14);
STATIC_ASSERT(offsetof(struct Message, transition) == 0x1c);
STATIC_ASSERT(offsetof(struct Message, flags) == 0x24);

/// Requests transfer of the four-row tilemap band containing window row y.
#define Window_MarkTilemapRow(render, y)                                       \
  do {                                                                         \
    (render)->dirty |= 2 << ((u32)(y) >> 2);                                   \
  } while (0)

#include "edition.h"

/// Window sprites in the shared pool: every sprite the windows show at once,
/// glyphs, icons and portraits together.
enum { WINDOW_SPRITE_COUNT = 64 };

/// WindowWork.portraitPictures of a free portrait slot, as Portrait_ClearSlots
/// writes it.
#define PORTRAIT_SLOT_EMPTY 999

/// The sprite palette of portrait slot 0; slot 1 uses the next.
#define PORTRAIT_PALETTE 14

/// The window system's work: the windows' tilemap, the windows, the three text
/// messages, the 64 window sprites, and the decoded message and its arguments.
struct WindowWork {
  u16 tilemap[640]; ///< the windows' BG tilemap, 32 by 20 tiles
  struct Window windows[WINDOW_COUNT]; ///< 12 in Japanese, 8 in the others
  struct Message messages[3];          ///< the messages, NULL work when free
  struct WindowSprite sprites[WINDOW_SPRITE_COUNT];
  struct WindowSpriteList freeSprites; ///< the sprites not in any window
  /// Per text tile (tiles 128 to 255, then 512 to 639): bit 0 once allocated,
  /// bit 1 while the tilemap shows it, 0 when free.
  u8 textTileState[256];
  u16 nextTextTile;  ///< the text tile the next search for a free one starts at
  u8 extraTextTiles; ///< 1 lets text use 256 tiles, adding tiles 512 to 639
  /// The tilemap's 4-row bands to upload: bit 0 for all, bit n + 1 for band n.
  u8 dirty;
  /// Set while Mind Read shows thoughts: round border corners, colour 8 text
  /// with no shadow, and latched button presses to dismiss text.
  u8 mindRead;
  u8 battleMode;        ///< enum WindowBattleMode
  u8 holdTilemapUpload; ///< while set, the tilemap is not uploaded
  u8 textPalette;  ///< text tiles' palette; 15 by default, 14 or 5 for low HP
  u16 lineSpacing; ///< the current message's, copied in to draw
  u16 unusedAfterLineSpacing; ///< set to 1 with the text style; not read
  u16 outline;                ///< the current message's, copied in to draw
  u16 colour;                 ///< the current message's, copied in to draw
  u16 textBuffer[TEXT_BUFFER_MASK + 1]; ///< decoded text, a ring of codes
  u16 letterSpacing;                    ///< pixels between sprite glyphs; 9
  u16 textBufferEnd;    ///< where the next code goes in textBuffer
  u16 lastMessageStart; ///< where the last decoded message began in textBuffer
  u16 glyphResource;    ///< VRAM cache slot of the page arrow; 99 when none
  u16 glyphTiles;       ///< first sprite tile of the glyph sheet
                        ///< (VRAM_CACHE_ENTRY_GLYPH_SHEET)
  /// Message argument values (MessageArgument_Push).
  u32 argumentValues[8];
  u16 argumentKinds[8]; ///< the MESSAGE_ARGUMENT_ kind of each value; 0 for a
                        ///< free slot
  u16 portraitPictures[2]; ///< picture shown in each portrait slot;
                           ///< PORTRAIT_SLOT_EMPTY when free
  u16 portraitSprites[2];  ///< VRAM cache slot of each portrait slot's picture
  u16 voiceSound;          ///< first of four voice blip sounds; 0 for none
  u16 voiceCountdown;      ///< characters until the next voice blip
  u8 pageArrowShown;       ///< 1 while the next-page arrow is shown
  /// Set while a message waits on its sound: the text ends its pauses and
  /// pages once the sound is over.
  u8 waitingForSound;
  /// 1 for halting speech: a pause before each character, broken off with an
  /// ellipsis after the first word (Message_SetDecodeFlags bit 0).
  u8 haltingSpeech;
  /// 1 puts code 222 after certain characters in English and Japanese
  /// (Message_SetDecodeFlags bit 1).
  u8 spacingAfter;
};

/// The byte of WindowWork that Mind Read sets, for the scenes that read it
/// through a byte pointer.
#define WINDOW_WORK_MIND_READ_OFFSET offsetof(struct WindowWork, mindRead)

/// The window renderer's work, heap slot HEAP_SLOT_WINDOW under its own name.
#define gWindowWork (*(u8 *(*)[]) & gHeapSlots[HEAP_SLOT_WINDOW])

#include "graphics/glyph.h"

struct AbilityListEntry;
struct BattleAction;
struct MenuSprite;
struct Window;
struct WindowSprite;
struct WindowSpriteList;
struct SelectionNode;
struct SelectionMenuWork;
struct MessageReader;
struct Message;
struct OptionMenuWork;

/// The flags of a UI window (struct Window.flags), which Window_Create
/// takes as its attributes.
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

// The text canvas tiles in VRAM: 120 4bpp tiles from tile 296, enough for the
// interior of a full-width window six tiles tall.
#define TEXT_CANVAS_VRAM ((void *)0x06002500)
#define TEXT_CANVAS_SIZE (120 * TILE_SIZE_4BPP)
#define TEXT_CANVAS_FIRST_TILE 296

/// The UI tilemap in VRAM (BG screen block 4), which WindowSystem_UploadTilemap
/// copies the work block's tilemap to.
#define WINDOW_TILEMAP_VRAM ((u8 *)0x06002000)

/// The BG palette of the windows and their text, the UI palette.
#define WINDOW_PALETTE 15

// The window colour settings (MainRoutineWork.windowHue and windowBrightness),
// which WindowColor_Apply turns into WINDOW_PALETTE's colours 4 to 10.
#define WINDOW_HUE_COUNT 24 ///< "Window color" takes 0 to 23
/// "Window brightness" takes 0 to 14.
#define WINDOW_BRIGHTNESS_COUNT 15
/// The brightness that adds nothing to the hue's colour.
#define WINDOW_BRIGHTNESS_NEUTRAL 7
/// Steps round gWindowColor_WaveTable's colour wheel; each hue is four.
#define WINDOW_COLOR_WHEEL_STEPS 96

/// The BG palette the menus keep for their selected row or page: a copy of
/// WINDOW_PALETTE with its background lightened (Palette_LightenHighlight),
/// which the item icons borrow (Palette_LoadIconsIntoHighlight).
#define HIGHLIGHT_PALETTE 14
/// How many levels HIGHLIGHT_PALETTE lightens each channel of
/// WINDOW_BACKGROUND_COLOR by (Palette_LightenHighlight).
#define HIGHLIGHT_LIGHTEN_STEPS 9
/// The window palette's background colour, behind its text; in
/// HIGHLIGHT_PALETTE, the selected row's.
#define WINDOW_BACKGROUND_COLOR 4
/// Eight 4bpp pixels of WINDOW_BACKGROUND_COLOR, for filling the text canvas
/// or a tile a word at a time.
#define TEXT_CANVAS_BACKGROUND_WORD 0x44444444
/// The colour that text drawn in a palette other than WINDOW_PALETTE stands
/// on (Text_DrawStringAsTiles); the menus copy WINDOW_BACKGROUND_COLOR into it.
#define TEXT_BACKGROUND_COLOR 14
/// The BG palette that battle and the Djinn menu fill with the first basic
/// sprite palette, its TEXT_BACKGROUND_COLOR the window background: the element
/// icons and the coloured numbers are drawn in it (WindowWork.textPalette).
#define BASIC_COLORS_PALETTE 5

/// Text colours (Text_SetColor), named as both the window palette (through
/// gFont_ColorPens) and the basic sprite palette show them.
enum TextColor {
  TEXT_COLOR_RED = 2,
  TEXT_COLOR_YELLOW = 4,
  TEXT_COLOR_PINK =
      9, ///< in BASIC_COLORS_PALETTE; WINDOW_PALETTE shows it white
  TEXT_COLOR_GREY = 14,
  TEXT_COLOR_WHITE = 15, ///< a message's colour at start
};

/// The OAM shape and size a window sprite is created with
/// (WindowSprite_Create): attribute 0's shape in the low half, attribute 1's
/// size in the high half.
#define OUTPUT_SIZE_16X16 0x40000000
#define OUTPUT_SIZE_32X32 0x80000000
#define OUTPUT_SIZE_32X8 0x40004000
#define OUTPUT_SIZE_32X16 0x80004000
#define OUTPUT_SIZE_16X32 0x80008000

/// A tilemap entry of WINDOW_PALETTE.
#define WINDOW_TILE(tile) (0xf000 | (tile))
/// Two copies of a tilemap entry, for filling a run of entries a word at a
/// time.
#define WINDOW_TILE_PAIR(entry) ((u32)(entry) << 16 | (entry))

// The two page arrows beside a paged list's page numbers, in the menus and
// the File Screen: tiles 296 and 297 of character block 0, which
// Link_DrawPageArrows slides back and forth each frame, and their
// tilemap entries.
#define PAGE_ARROW_TILE 296
#define PAGE_ARROW_VRAM                                                        \
  ((s32)BG_CHAR_BLOCK(0) + PAGE_ARROW_TILE * TILE_SIZE_4BPP)
#define PAGE_ARROW_LEFT WINDOW_TILE(PAGE_ARROW_TILE)
#define PAGE_ARROW_RIGHT WINDOW_TILE(PAGE_ARROW_TILE + 1)

/// The window border's tiles.
enum {
  WINDOW_TILE_BLANK = WINDOW_TILE(0),
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
  /// a horizontal line leaving right
  WINDOW_TILE_DIVIDER_TEE_RIGHT = WINDOW_TILE(11),
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
  /// In battle, the tile the command row fills its empty panels with
  /// (screens/battle_command_row.c), and Window_EraseBorderRect below row 16.
  WINDOW_TILE_COMMAND_PANEL = WINDOW_TILE(127),
  /// The HP and PP labels before a number (Text_DrawPrefixedNumberAtOffset)
  /// or a Psynergy's cost: each label's first tile, and the second tile the
  /// HP label always takes.
  WINDOW_TILE_HP_LABEL = WINDOW_TILE(29),
  WINDOW_TILE_HP_LABEL_END = WINDOW_TILE(30),
  WINDOW_TILE_PP_LABEL = WINDOW_TILE(31),
};

/// The PP label's second tile: the HP label's in English, German and
/// Japanese, a tile of its own in Spanish, French and Italian.
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define WINDOW_TILE_PP_LABEL_END WINDOW_TILE(5)
#else
#define WINDOW_TILE_PP_LABEL_END WINDOW_TILE_HP_LABEL_END
#endif

/// A tilemap entry's tile number, the bits below its flip bits and palette
/// number in the hardware's screen entry.
#define TILE_NUMBER_MASK 0x3ff
/// The tilemap entry flip bits.
#define TILE_HFLIP 0x400
#define TILE_VFLIP 0x800

/// The DMA control word that fills count words from one fixed source word.
#define DMA_FILL32(count)                                                      \
  ((DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (count))

/// How a window sprite moves (struct WindowSprite.motion), run each frame
/// by WindowSprite_UpdateAll. 0 is a free window sprite.
enum {
  WINDOW_SPRITE_MOTION_STILL = 1,
  WINDOW_SPRITE_MOTION_PAGE_ARROW =
      2,                           ///< the animated next-page arrow, bobbing
  WINDOW_SPRITE_MOTION_WOBBLE = 4, ///< swings along a small arc
  WINDOW_SPRITE_MOTION_SHAKE = 5,  ///< jitters by a pixel every other frame
  WINDOW_SPRITE_MOTION_POP = 6,    ///< shown at double size, then normal size
  WINDOW_SPRITE_MOTION_SPIN = 7,   ///< turns while circling its place
  WINDOW_SPRITE_MOTION_PULSE = 8,  ///< shown at 1.25 times, then normal size
  WINDOW_SPRITE_MOTION_SWELL = 9,  ///< pulses between 1 and 1.5 times its size
  WINDOW_SPRITE_MOTION_SWELL_HALF = 10, ///< the same at half the scale
  WINDOW_SPRITE_MOTION_SHRINK =
      11, ///< shrinks from 1.5 times to its size, once
  WINDOW_SPRITE_MOTION_SHRINK_HALF = 12, ///< the same at half the scale
  WINDOW_SPRITE_MOTION_HIDDEN = 13,      ///< not drawn
  /// Three motions WindowSprite_UpdateAll runs alike: a hop down and back.
  WINDOW_SPRITE_MOTION_HOP_DOWN = 14,
  WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE = 15,
  WINDOW_SPRITE_MOTION_HOP_DOWN_3 = 16,
  WINDOW_SPRITE_MOTION_HOP_UP = 17,
  WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT = 18,
};

/// The command icons (ResourceId_CommandIcons), each drawn with the command
/// name of the same number (MsgCommandName + icon).
enum CommandIcon {
  COMMAND_ICON_ATTACK = 0,
  COMMAND_ICON_PSYNERGY = 1,
  COMMAND_ICON_ITEM = 2,
  COMMAND_ICON_DEFEND = 3,
  COMMAND_ICON_FLEE = 4,
  COMMAND_ICON_YES = 5,
  COMMAND_ICON_NO = 6,
  COMMAND_ICON_STATUS = 7,
  COMMAND_ICON_CHECK = 8,
  COMMAND_ICON_USE = 9,
  COMMAND_ICON_GIVE = 10,
  COMMAND_ICON_DROP = 11,
  COMMAND_ICON_SPEAK = 12,
  COMMAND_ICON_EQUIP = 13,
  COMMAND_ICON_FIGHT = 14,
  COMMAND_ICON_DJINN = 15,
  COMMAND_ICON_SUMMON = 16,
  COMMAND_ICON_BUY = 17,
  COMMAND_ICON_SELL = 18,
  COMMAND_ICON_ARTIFACTS = 19,
  COMMAND_ICON_REPAIR = 20,
  COMMAND_ICON_NEW_GAME = 21,
  COMMAND_ICON_CONTINUE = 22,
  COMMAND_ICON_COPY = 23,
  COMMAND_ICON_ERASE = 24,
  COMMAND_ICON_REVIVE = 25,
  COMMAND_ICON_CURE_POISON = 26,
  COMMAND_ICON_REPEL_EVIL = 27,
  COMMAND_ICON_REMOVE_CURSE = 28,
  COMMAND_ICON_BATTLE = 29,
  COMMAND_ICON_SEND = 30,
  COMMAND_ICON_CHANGE = 31,
  COMMAND_ICON_COINS = 32,
  COMMAND_ICON_LUCKY_MEDALS = 33,
  // The settings menu's choices and the Pause menu's entries
  // (gSettingsMenu_SpeedIcons to gPauseMenu_Icons), whose command names are
  // placeholders.
  COMMAND_ICON_SPEED_SLOW = 34,
  COMMAND_ICON_SPEED_NORMAL = 35,
  COMMAND_ICON_SPEED_FAST = 36,
  COMMAND_ICON_SPEECH_OFF = 37,
  COMMAND_ICON_SPEECH_ON = 38,
  COMMAND_ICON_AUTO_SLEEP_OFF = 39,
  COMMAND_ICON_AUTO_SLEEP_ON = 40,
  COMMAND_ICON_SAVE_QUEST = 41,
  COMMAND_ICON_SLEEP = 42,
  COMMAND_ICON_CHANGE_SETTINGS = 43,
  COMMAND_ICON_PASSWORD = 44,
  COMMAND_ICON_CABLE = 45,
  COMMAND_ICON_GOLD = 46,
  COMMAND_ICON_SILVER = 47,
  COMMAND_ICON_BRONZE = 48
};

/// What Glyph_Draw and Window_PutGlyph draw.
enum GlyphDrawMode {
  GLYPH_DRAW_CHARACTER = 0, ///< the character given
  GLYPH_DRAW_PAGE_ARROW = 1 ///< the next-page arrow, in the window's corner
};

/// The window FieldMessage_ShowCentered shows its message in.
enum CenteredMessageMode {
  /// No frame or size: it opens and closes at once.
  CENTERED_MESSAGE_BARE = 0,
  /// A zooming window sized to the message, waited for as it closes.
  CENTERED_MESSAGE_WINDOW = 1
};

/// The flags FieldMessage_Show takes.
enum FieldMessageFlag {
  FIELD_MESSAGE_ZOOM = 1, ///< the window zooms open and shut
  FIELD_MESSAGE_WAITING_FOR_SOUND =
      2,                            ///< the window work is busy while it shows
  FIELD_MESSAGE_KEEP_OPEN = 4,      ///< the window is left open
  FIELD_MESSAGE_LOWER = 8,          ///< 4 tiles below the top rows' centre
  FIELD_MESSAGE_HOLD_UPLOAD = 0x20, ///< the tilemap upload is held off
  FIELD_MESSAGE_LOWEST = 0x40       ///< 12 tiles below the top rows' centre
};

/// The Pause menu's entries, as PauseMenu_Choose returns them
/// (counted from Save your quest even when only Change settings shows).
enum PauseMenuEntry {
  PAUSE_MENU_SAVE_QUEST = 0,
  PAUSE_MENU_SLEEP = 1,
  PAUSE_MENU_CHANGE_SETTINGS = 2,
  // The debug entries.
  PAUSE_MENU_ROUGH_MENU = 3,
  PAUSE_MENU_RECOVERY_TEST = 4,
  PAUSE_MENU_RESTART_QUEST = 5
};

/// The kinds of icon the selection screen lists (SelectionNode.kind) and
/// Resource_LoadByMode, Resource_LoadByModeIntoSlot and
/// WindowSprite_CreateFromResource load.
enum IconKind {
  ICON_KIND_NONE = 0,    ///< a free selection icon entry
  ICON_KIND_COMMAND = 1, ///< a command's icon, from its number
  /// An item's icon, with its count above one and its equipped and broken
  /// marks (and the artifact mark where a slot is given).
  ICON_KIND_ITEM = 2,
  ICON_KIND_ABILITY = 4, ///< a Psynergy's icon, from its ability number
  ICON_KIND_YES_NO = 6,  ///< a yes or no command, drawn as a command
  /// An item's icon without the broken mark, for the equipment view.
  ICON_KIND_EQUIPMENT_ITEM = 7,
  ICON_KIND_PATTERN = 8, ///< one of the other icons (Icon_BuildMiscIconTiles)
  /// The one tile Resource_LoadTableEntry loads.
  ICON_KIND_TABLE_ENTRY = 9
};

// The layers ItemIcon_Compose draws over an item's icon.
#define ICON_BASE 1            ///< the base under the icon
#define ICON_COUNT_ABOVE_ONE 2 ///< the count, unless it is 1
#define ICON_COUNT 4           ///< the count, always
/// Mark 1, the cross, on an item with INVENTORY_BROKEN.
#define ICON_BROKEN 8
/// Mark 0, the equip mark, on an item with INVENTORY_EQUIPPED.
#define ICON_EQUIPPED 16
/// Mark 2, Icon_MarkPhi, on a cursed item that cannot be unequipped
/// (ITEM_CURSED and ITEM_CANNOT_UNEQUIP) when its code has INVENTORY_EQUIPPED.
#define ICON_ARTIFACT 32

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

struct WindowSprite *WindowSprite_AcquireFree(void);
void WindowSprite_ReleaseFree(struct WindowSprite *out);
void WindowSprite_InitializeFreeList(void);
void WindowSystem_InitField(void);
void WindowSystem_InitBattle(s32 loadGlyphTiles);
void WindowSystem_UploadTilemap(void);
void Window_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
void Window_DrawOpened(struct Window *window);
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs);
void Window_WaitZoom(struct Window *window);
void Window_Close(struct Window *window, s32 instant);
void WindowSprite_PrepareForRedraw(struct Window *window);
void WindowSprite_RedrawSavedRect(struct Window *window);
void WindowSprite_ClearList(void *sprites);
void Window_ClearInteriorTiles(const struct Window *window, u32 left, u32 top,
                               u32 right, u32 bottom);
void WindowSprite_UpdateListTail(struct WindowSpriteList *list);
// screens.c calls WindowSprite_AppendToList with no prototype in scope.
void WindowSprite_Release(struct WindowSprite *entry);
// screens.c calls Message_Start with no prototype in scope: the join messages
// pass five of its six arguments.
struct Message *BattleMessage_Continue(struct Window *window, s32 entry,
                                       s32 keepPosition);
s32 Text_ClearCanvas(void);
s32 Text_FillCanvasBackground(void);
void Message_ResetIdle(void);
void Message_ApplyStyle(struct Message *message);
void Text_StartCanvasScroll(struct Message *message);
void Text_ScrollCanvasUp(s32 step);
void Message_UpdateAll(void);
s32 Message_Type(struct Message *message);
void Window_UpdateZooms(void);
void Window_DrawZoomBorder(void *windowArg, s32 savePosition);
u16 *Memory_FillHalfwordsDma(u16 *dst, s32 value, s32 count);
void Window_DrawBorder(s32 x, s32 y, u32 width, u32 height);
void Window_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height,
                               s32 fullWidth);
s32 Message_AllTyped(void);
s32 Window_IsClosed(struct Window *window);
void Text_ResetStyle();
void WindowSystem_StartFieldUpdate(void);
void WindowSystem_StartBattleUpdate(s32 loadGlyphTiles);
void BattleMessage_Close(void);
void BattleMessage_Open(s32 argument);
void BattleMessage_ShowAndWait(s32 argument);
s32 Message_OpenDecoded(struct Window *window, s32 argument);
void Message_SetDecodeFlags(s32 flags);
struct Window *Message_OpenWindow(s32 message, s32 x, s32 y, u32 packed);
void FieldMessage_Show(s32 no, s32 flags);
void WindowSystem_Update(void);
#if LANGUAGE != LANGUAGE_JAPANESE
s32 Text_DrawGlyphPair(s32 code, u32 *out);
#else
s32 Text_DrawGlyphPair(u32 code, u32 *out);
#endif
s32 Text_GetWideStringWidth(u16 *text);
void Text_DrawWideStringAtOffset(u16 *text, struct Window *window, s32 x,
                                 s32 y);
void Text_DrawGlyphTileAtWorkOffset(u16 *buffer, const struct Window *window,
                                    s32 offsetX, s32 offsetY);
// screens.c calls Text_DrawWideStringInWindow with no prototype in scope.
u8 *String_FormatNumber(u8 *buffer, s32 input, s32 width);
u32 Message_AppendArticleName(s32 mode, u16 *name, u32 pos, u16 *entry, s32 no,
                              s32 plural, s32 *suffix);
s32 Message_Decode(s32 script, s32 clear);
void Window_FitOnScreen(s32 entryIndex, s32 *windowX, s32 *windowY,
                        u32 *windowWidth, u32 *windowHeight, u16 *spacing,
                        u32 flags);
void Text_MeasureMessage(s32 no, s32 *x, s32 *y);
s32 Text_GetMessageSize(s32 no, s32 *x, s32 *y, u32 *width, u32 *height);
s32 Text_GetMessageSizeAlt(s32 no, s32 *x, s32 *y, u32 *width, u32 *height);
void Text_MeasureDecoded(s32 pos, u32 *out_width, u32 *out_height,
                         u16 *spacing);
void Text_MeasureAllPages(s32 start, u32 *out_width, u32 *out_height,
                          u16 *spacing);
s32 Glyph_Draw(struct Window *window, s32 character, s32 x, s32 y, s32 mode);
// screens.c calls Window_PutGlyph with no prototype in scope, and
// PartyStatus_Draw passes four of its five arguments.

/// How Window_SetTilemapEntry writes its value (its mode argument): as given,
/// or as a tile number in one of the palettes the menus keep for their
/// windows.
enum TilemapEntryMode {
  TILEMAP_ENTRY_PLAIN, ///< as given, palette bits included
  TILEMAP_ENTRY_NONE,  ///< nothing
  /// In HIGHLIGHT_PALETTE, the highlight Palette_LightenHighlight makes for
  /// the selected row or page.
  TILEMAP_ENTRY_HIGHLIGHT,
  TILEMAP_ENTRY_WINDOW, ///< in WINDOW_PALETTE
  /// In palette 1, which the Status screen and the party rearranging fill
  /// with the element icons' colours for the rows that are not highlighted
  /// (StatusScreen_Run, PartyMemberChooser_RunRearrange).
  TILEMAP_ENTRY_ICON
};
void Window_SetTilemapEntry(struct Window *window, s32 value, s32 x, s32 y,
                            u32 mode);
void WindowSprite_UpdateScaleAnimation(struct WindowSprite *sprite);
void WindowSprite_UpdateAll(void);
void Message_DecodeRawUnbounded(s32 message, u16 *buf);
s32 Message_CopyExpanded(s32 key, u16 *destination, u32 capacity);
void Message_DecodeRaw(s32 message, u16 *text, s32 capacity);
s32 *Memory_ClearWordIfSet(s32 *p);
void Window_CloseAll(s32 unused);
void Window_StepSwellEffect(struct Message *message);
void MessageArgument_ClearAll(void);
void MessageArgument_Push(u32 value, u32 kind);
u32 MessageArgument_Lookup(u32 kind, u32 clear);
s32 Text_CheckPauseSkipped(struct Message *message);
s32 Text_CheckTextDismissed(struct Message *message);
void Message_CloseFinishedWindows(void);
void FieldMessage_ShowCentered(s32 message, s32 mode, s32 yOffset);
s32 Message_DecodeAndClearArguments(s32 message);
void Message_Find(struct MessageReader *reader, s32 message);
s32 Text_DecodeSymbolInC(struct MessageReader *reader);
void Portrait_ClearSlots(void);
s32 Portrait_GetForSpeaker(u32 value);
struct Window *Portrait_OpenWindow(s32 speaker, s32 portraitOption, s32 x,
                                   s32 y);
void Portrait_CloseWindow(u32 entry);

#define WINDOW_WORK ((struct WindowWork *)gWindowWork[0])

#define ACTION_ID_MASK 0x3fff

/// Defines name(no, x, y, width, height): decodes message no and fits a window
/// for it on the screen near tile (x, y) (Window_FitOnScreen with flags),
/// returning its place and size in tiles. Returns 0 for an empty message.
#define DEFINE_GET_RESOURCE_DIMENSIONS(name, flags)                            \
  s32 name(s32 no, s32 *x, s32 *y, u32 *width, u32 *height) {                  \
    struct WindowWork *work;                                                   \
    s32 idx;                                                                   \
                                                                               \
    work = WINDOW_WORK;                                                        \
    idx = Message_Decode(no, 0);                                               \
    if (work->textBuffer[idx] == 0) {                                          \
      return 0;                                                                \
    }                                                                          \
    Window_FitOnScreen(idx, x, y, width, height, 0, flags);                    \
    return 1;                                                                  \
  }

s32 Icon_CountItemIcons(void);
s32 Icon_CountPsynergyIcons(void);
void Icon_BuildCommandIconTiles(s32 overlayIcon, s32 baseIcon, s32 *slot,
                                s32 *tileOut, s32 reuse);
void Ability_RequestGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile,
                          s32 reuse);
void Icon_BuildItemIconTiles(u32 glyph, s32 with_base, s32 *src, s32 *dst,
                             s32 reuse);
s32 ItemIcon_Compose(u32 code, u32 layers);
s32 Icon_CopyResourceToSlot(s32 inventoryCode, s32 layers, s32 cacheSlot);
s32 Icon_BuildMiscIconTiles(s32 icon, s32 unused, s32 slot);
s32 Resource_LoadTableEntry(u32 value, s32 unused, u32 slot);
void Icon_DrawItem(s32 code);
void Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile,
                       s32 reuse);
void Icon_BuildAbilityIconTiles(u32 glyph, s32 with_base, s32 *src, s32 *dst,
                                s32 reuse);
void Icon_DrawMisc(u32 index);
void Icon_LoadWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                          s32 palette, s32 reuse);
void Icon_ReservedNoOp(void);
void Icon_Decode(GlyphTransfer *glyph, s32 outlined);

void SelectionMenu_Create(void);
void SelectionMenu_Clear(void);
void SelectionMenu_AddEntry(u32 first, u32 second);
void SelectionMenu_BuildEntries(u32 unused);
void SelectionMenu_ReservedNoOp(void);
struct SelectionNode *SelectionMenu_FindFreeIcon(s32 kind);
void SelectionMenu_StartDrawing(void);
void SelectionMenu_StopDrawing(void);
void SelectionMenu_Draw(void);
void SelectionMenu_DrawArrow(struct SelectionMenuWork *state, s32 index);
void SelectionMenu_OpenNameWindow(s32 mode, u32 count);
void SelectionMenu_Close(void);
void SelectionMenu_SetPosition(u32 first, u32 second);
void Menu_SetupSelectionBothSides(void);
void SelectionMenu_SetupArrow(struct SelectionMenuWork *state, s32 index);
struct SelectionNode *
SelectionMenu_GetCursorNode(struct SelectionMenuWork *screen);
s32 SelectionMenu_Run(s32 mode);
u32 SelectionMenu_RunWithSounds(u32 mode);
void Menu_MoveSelectionForward(struct SelectionMenuWork *screen);
void Menu_MoveSelectionBackward(struct SelectionMenuWork *screen);
void SelectionMenu_StepRight(struct SelectionMenuWork *state);
void SelectionMenu_StepLeft(struct SelectionMenuWork *state);
void SelectionMenu_RedrawIcon(struct SelectionMenuWork *state, u32 index);
void SelectionMenu_HighlightIcon(struct SelectionMenuWork *state, u32 index);
void Menu_ClearBattleHighlight(struct SelectionMenuWork *screen);
void SelectionMenu_Scroll(struct SelectionMenuWork *screen, s32 forward);
s32 Resource_LoadByMode(s32 mode, s32 value);
s32 Resource_LoadByModeIntoSlot(s32 mode, s32 value, s32 result, s32 option);
void SelectionMenu_SetupIcon(u32 kind, s32 base, struct SelectionNode *node,
                             s32 reuse);
u32 Menu_ConfirmSelection(struct SelectionMenuWork *screen);
void MenuArrow_NoOpCallback(void);
void MenuArrow_NoOpCallbackBank(void);
void MenuArrow_NoOpCallback1(void);
void MenuArrow_NoOpCallback2(void);
void MenuArrow_NoOpCallback3(void);
void MenuArrow_NoOpCallback4(void);
void MenuArrow_Create(struct MenuSprite *object, s32 *slot);
void MenuArrow_SetPosition(struct MenuSprite *obj, s32 x, s32 y);
void MenuArrow_Release(s32 value);
void SelectionMenu_LoadLargeIcon(void);
void Resource_ResetPendingTransfer(void);
s32 FieldMenu_Run(void);
void PartyStatus_OpenAndWait(void);
// The matching build calls this before its void definition and uses the
// result. FieldMenu_Run keeps that form outside UBFIX.
#ifdef UBFIX
void PartyStatus_CloseWithMenu(void);
#endif
void SelectionMenu_CreateEmpty(void);
u32 Menu_RunSelectionForValue(u32 value);

void AreaNameWindow_Show(void);
void AreaNameWindow_Update(void);
void AreaNameWindow_Hide(void);
s32 Item_RemoveFromInventory(s32 unused, s32 itemId);
s32 Item_ReturnTrue(void);
void SettingsMenu_StepWindowHue(s32 keys);
void Debug_SelectShortcuts(void);
s32 Party_ListPsynergy(struct AbilityListEntry *output);
void Menu_FindShortcutEntries(u32 *first_index, u32 *second_index,
                              const struct AbilityListEntry *entries);
void Menu_RunSelectionHook(void);

void Menu_AllocateSelectionWorkspace(void);
void Menu_ReleaseSelectionWorkspace(void);
void SettingsMenu_ReservedNoOpC9BC(void);
void SettingsMenu_ReservedNoOpC9C0(void);
void SettingsMenu_ReservedNoOpC9C4(void);
void SettingsMenu_ResetEntries(struct OptionMenuWork *work);
void WindowColor_CalculateTint(struct OptionMenuWork *work);
void WindowColor_WritePalette(struct OptionMenuWork *work);
u16 WindowColor_ScaleTint(struct OptionMenuWork *work, s32 scale_r, s32 scale_g,
                          s32 scale_b);
u16 Color_ScaleComponents(s16 *rgb, s32 red_scale, s32 green_scale,
                          s32 blue_scale);
void WindowColor_Apply(s32 phase, s32 brightness);
void SettingsMenu_PreviousRow(struct OptionMenuWork *work);
void SettingsMenu_NextRow(struct OptionMenuWork *work);
void SettingsMenu_LowerSetting(struct OptionMenuWork *work);
void SettingsMenu_RaiseSetting(struct OptionMenuWork *work);
void SettingsMenu_SelectionNoOp(void);
void SettingsMenu_AnimateCursors(void);
void SettingsMenu_InitializeWork(void);
void SettingsMenu_Release(void);
struct Window *SettingsMenu_Open(void);
s32 SettingsMenu_Run(void);
void PauseMenu_AnimateCursor(void);
void PauseMenu_Initialize(void);
void PauseMenu_Release(void);
struct Window *PauseMenu_Open(void);
s32 PauseMenu_Choose(void);

void Window_OverlayTilePattern(u16 *entry, u16 *mirror, s32 pattern, u8 *remap);
s32 Text_DrawStringAsTiles(u16 *script, u16 *tiles, u16 *mirror, s32 x);
// Window_ClearTextTilesInRect is called before its definition with no
// prototype in scope.
void Window_MarkVisibleTextTiles(void);
void Text_UseExtraTiles(s32 flag);
void Window_ReservedNoOp(void);
void Window_DrawDividerLine(struct Window *win, u32 x1, u32 y1, u32 x2, u32 y2);
void Text_SetColor(s32 param);
void Text_SetStyle(u16 value);
void Text_DrawMessage(s32 message, struct Window *window, s32 x, s32 y);
void Text_DrawMessageAsTiles(s32 message, const struct Window *window,
                             u32 offsetX, u32 offsetY);
void Text_DrawString(const u8 *text, struct Window *window, s32 x, s32 y);
void Text_DrawStringAtOffset(const u8 *text, const struct Window *window,
                             s32 offsetX, s32 offsetY);
void Text_DrawStringInWindow(const u8 *text, struct Window *window, u32 x,
                             u32 y);
void Text_DrawNumber(s32 value, s32 width, struct Window *window, s32 x, s32 y);
void Text_DrawNumberAtOffset(s32 value, s32 width, const struct Window *window,
                             s32 x, s32 y);
void Text_DrawNumberInWindow(s32 value, s32 width, struct Window *window, s32 x,
                             s32 y);
// screens.c calls Text_DrawPrefixedNumberAtOffset with no prototype in scope.

/// The menus' compressed 64x16 headings, Save to Setup (menu_titles.s).
extern const u8 *gMenuTitlePictures[];

#endif
