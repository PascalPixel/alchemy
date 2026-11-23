#ifndef ALCHEMY_WINDOW_H
#define ALCHEMY_WINDOW_H

/// @file
/// Windows, the messages that type into them, and the window system's work,
/// as window.c declares them for the files that use them.

#include "types.h"
#include "system/system.h"
#include "system/frame.h"
#include "system/io_reg.h"
#include "window/window_tile.h"
#include "window/message_argument.h"
#include "window/message_speed.h"
#include "edition.h"
#include "tla_edition.h"
#include "graphics/oam.h"
#include "graphics/render_input.h"
#include "graphics/uiflow.h"
#include "static_assert.h"

/// The window sprites WindowWork keeps, free or in a window.
enum { WINDOW_SPRITE_COUNT = 64 };

/// The window tiles text is drawn into: TEXT_TILES from TEXT_TILE_FIRST, and
/// with WindowWork.extraTextTiles as many more from TEXT_EXTRA_TILE_FIRST.
#define TEXT_TILE_FIRST 128
#define TEXT_TILES 128
#define TEXT_EXTRA_TILE_FIRST 512

/// The window system's work: its text tile and upload state, the windows'
/// tilemap, the windows, the three text messages, the 64 window sprites, and
/// the decoded message and its arguments.
struct WindowWork {
  u16 nextTextTile;  ///< the text tile the next search for a free one starts at
  u8 extraTextTiles; ///< 1 lets text use the second range of tiles
  /// The tilemap's 4-row bands to upload: bit 0 for all, bit n + 1 for band n.
  u8 dirty;
  u8 mindRead;          ///< nonzero widens a message window by 2
  u8 battleMode;        ///< 1 from the work's setup
  u8 holdTilemapUpload; ///< while set, the tilemap is not uploaded
  u8 textPalette;       ///< text tiles' palette; 15 by default
  u16 tilemap[20][32];  ///< the windows' BG tilemap
  struct Window windows[WINDOW_COUNT];
  struct Message messages[3];
  struct WindowSprite sprites[WINDOW_SPRITE_COUNT];
  struct WindowSpriteList freeSprites; ///< the sprites not in any window
  /// Per text tile (tiles 128 to 255, then 512 to 639): bit 0 once allocated,
  /// bit 1 while the tilemap shows it, 0 when free.
  u8 textTileState[2 * TEXT_TILES];
  u16 lineSpacing;            ///< the current message's, copied in to draw
  u16 unusedAfterLineSpacing; ///< set to 1 with the text style; not read
  u16 outline;                ///< the current message's, copied in to draw
  u16 colour;                 ///< the current message's, copied in to draw
  u16 textBuffer[TEXT_BUFFER_MASK + 1]; ///< decoded text, a ring of codes
  u16 glyphSpacing;                     ///< 9 with the text style
  u16 textBufferEnd;     ///< where the next code goes in textBuffer
  u16 lastMessageStart;  ///< where the last decoded message began in textBuffer
  u16 glyphResource;     ///< VRAM cache slot of the page arrow; 99 when none
  u16 glyphTiles;        ///< first of the glyph tiles (cache entry 95)
  u32 argumentValues[8]; ///< message argument values (MessageArgument_Push)
  u16 argumentKinds[8];  ///< the MESSAGE_ARGUMENT_ kind of each; 0 when free
  u16 portraitPictures[2]; ///< picture shown in each portrait slot; 999 when
                           ///< free
  u16 portraitSprites[2];  ///< VRAM cache slot of each portrait slot's picture
  u16 speakerVoiceSound;   ///< first of four voice blip sounds; 0 for none
  u16 speakerVoiceCountdown; ///< characters until the next voice blip
  u8 pageArrowShown;         ///< 1 while the next-page arrow is shown
  u8 waitingForSound;        ///< set while a message waits on its sound
  u8 haltingSpeech;          ///< cleared together with spacingAfter
  u8 spacingAfter;
  /// Nonzero lets a tall message fit in 32 pixels, placing its glyphs by
  /// y / 320 (Message.compressLines).
  s8 textMode;
};

STATIC_ASSERT(offsetof(struct WindowWork, tilemap) == 8);
STATIC_ASSERT(offsetof(struct WindowWork, windows) == 1288);
STATIC_ASSERT(offsetof(struct WindowWork, messages) == 1720);
STATIC_ASSERT(offsetof(struct WindowWork, sprites) == 1840);
STATIC_ASSERT(offsetof(struct WindowWork, freeSprites.head) == 3632);
STATIC_ASSERT(offsetof(struct WindowWork, freeSprites.tailLink) == 3636);

STATIC_ASSERT(offsetof(struct WindowWork, lineSpacing) == 3896);
STATIC_ASSERT(offsetof(struct WindowWork, colour) == 3902);
STATIC_ASSERT(offsetof(struct WindowWork, textBuffer) == 3904);
STATIC_ASSERT(offsetof(struct WindowWork, battleMode) == 5);
STATIC_ASSERT(offsetof(struct WindowWork, textPalette) == 7);
STATIC_ASSERT(offsetof(struct WindowWork, textTileState) == 3640);

#if (LANGUAGE != LANGUAGE_JAPANESE)
STATIC_ASSERT(offsetof(struct WindowWork, textBufferEnd) == 4930);
STATIC_ASSERT(offsetof(struct WindowWork, glyphResource) == 4934);
STATIC_ASSERT(offsetof(struct WindowWork, argumentValues) == 4940);
STATIC_ASSERT(offsetof(struct WindowWork, argumentKinds) == 4972);
STATIC_ASSERT(sizeof(struct WindowWork) == 5008);
#else
STATIC_ASSERT(offsetof(struct WindowWork, textBufferEnd) == 4418);
STATIC_ASSERT(offsetof(struct WindowWork, glyphResource) == 4422);
STATIC_ASSERT(offsetof(struct WindowWork, argumentValues) == 4428);
STATIC_ASSERT(offsetof(struct WindowWork, argumentKinds) == 4460);
STATIC_ASSERT(sizeof(struct WindowWork) == 4496);
#endif

/// Message_SetDecodeFlags' flags, which set WindowWork.haltingSpeech and
/// spacingAfter for the messages decoded next.
enum MessageDecodeFlag {
  MESSAGE_DECODE_HALTING_SPEECH = 1, ///< a pause before each character
  MESSAGE_DECODE_SPACING_AFTER = 2   ///< a wide gap after some characters
};

/// WindowWork.glyphResource while the page arrow has no VRAM cache slot.
#define NO_GLYPH_RESOURCE 99

/// The last character code the window's text tiles cover.
#define TEXT_LAST_FONT_CODE 255
/// In Japanese, the codes after the font's are kanji (gFont_KanjiGlyphs).
#define KANJI_FIRST_CODE (TEXT_LAST_FONT_CODE + 1)
/// A main-font glyph row's pixels fill its halfword from the high bit, a bit
/// each: shifted up by this, the leftmost pixel is the word's sign bit.
#define GLYPH_ROW_BITS 16

/// WindowSprite.kind: what a window sprite owns besides itself, which
/// WindowSprite_Release frees.
enum WindowSpriteKind {
  WINDOW_SPRITE_GLYPH = 0,   ///< a glyph in the window's glyph tiles
  WINDOW_SPRITE_ICON = 1,    ///< its own VRAM cache entry
  WINDOW_SPRITE_PORTRAIT = 2 ///< a VRAM cache entry and a palette slot
};

/// How a window sprite moves (WindowSprite.motion); 0 is a free one.
enum {
  WINDOW_SPRITE_MOTION_PAGE_ARROW = 2 ///< the next-page arrow, bobbing
};

/// What Window_PutGlyph draws.
enum GlyphDrawMode {
  GLYPH_DRAW_CHARACTER = 0, ///< the character given, as a tile
  GLYPH_DRAW_PAGE_ARROW = 1 ///< the next-page arrow, in the window's corner
};

void WindowSprite_InitializeFreeList(void);

struct MenuCursor;
struct Window;

void Message_CloseFinishedWindows(void);

s32 Message_AllTyped(void);

void Text_DrawMessage(s32 message, s32 window, s32 x, s32 y);

/// The text canvas tiles in VRAM: 120 4bpp tiles from tile 296, enough for the
/// interior of a full-width window six tiles tall.
#define TEXT_CANVAS_VRAM ((void *)0x06002500)
#define TEXT_CANVAS_SIZE (120 * TILE_SIZE_4BPP)
#define TEXT_CANVAS_FIRST_TILE 296

/// The UI tilemap in VRAM (BG screen block 4), which WindowSystem_UploadTilemap
/// copies the work block's tilemap to.
#define WINDOW_TILEMAP_VRAM ((u8 *)0x06002000)

/// The sprite palette of portrait slot 0; slot 1 uses the next.
#define PORTRAIT_PALETTE 14

/// The BG palette the menus keep for their selected row or page: a copy of
/// WINDOW_PALETTE with its background lightened (Palette_LightenHighlight),
/// which the item icons borrow.
#define HIGHLIGHT_PALETTE 14
/// The window palette's background colour, behind its text; in
/// HIGHLIGHT_PALETTE, the selected row's.
#define WINDOW_BACKGROUND_COLOR 4
/// Eight 4bpp pixels of WINDOW_BACKGROUND_COLOR, for filling the text canvas
/// or a tile a word at a time.
#define TEXT_CANVAS_BACKGROUND_WORD 0x44444444
/// The colour that text drawn in a palette other than WINDOW_PALETTE stands
/// on; the menus copy WINDOW_BACKGROUND_COLOR into it.
#define TEXT_BACKGROUND_COLOR 14
/// WINDOW_PALETTE's white, black and two greys, which Mind Read turns grey
/// while it shows thoughts.
#define WINDOW_COLOR_WHITE 1
#define WINDOW_COLOR_BLACK 3
#define WINDOW_COLOR_DARK_GREY 11
#define WINDOW_COLOR_LIGHT_GREY 12
/// The BG palette that battle and the Djinn menu fill with the first basic
/// sprite palette, its TEXT_BACKGROUND_COLOR the window background: the element
/// icons and the coloured numbers are drawn in it.
#define BASIC_COLORS_PALETTE 5

/// The two page arrows beside a paged list's page numbers, in the menus and
/// the File Screen: tiles 296 and 297 of character block 0, which
/// Link_DrawPageArrows slides back and forth each frame, and their
/// tilemap entries.
#define PAGE_ARROW_TILE 296
#define PAGE_ARROW_VRAM                                                        \
  ((s32)BG_CHAR_BLOCK(0) + PAGE_ARROW_TILE * TILE_SIZE_4BPP)
#define PAGE_ARROW_LEFT WINDOW_TILE(PAGE_ARROW_TILE)
#define PAGE_ARROW_RIGHT WINDOW_TILE(PAGE_ARROW_TILE + 1)

/// The page numbers of a paged list, the battle lists' and the File Screen
/// password grid's: 20 tiles from this one of character block 0, which
/// Graphics_ExpandVramTilesByColorTable builds from the UI's digits '0' to '9',
/// first with colour 1 drawn as 10, then in their own colours; and their
/// tilemap entries, + n giving page n + 1's number, the page shown lit.
#define PAGE_DIGITS_TILE 768
#define PAGE_NUMBER_TILE WINDOW_TILE(PAGE_DIGITS_TILE + 1)
#define PAGE_NUMBER_LIT_TILE WINDOW_TILE(PAGE_DIGITS_TILE + 10 + 1)

/// How Window_SetTilemapEntry writes its value: as it is, not at all, or as a
/// tile in one of three palettes.
enum TilemapEntryMode {
  TILEMAP_ENTRY_PLAIN = 0,
  TILEMAP_ENTRY_NONE = 1,
  TILEMAP_ENTRY_HIGHLIGHT = 2, ///< in HIGHLIGHT_PALETTE
  TILEMAP_ENTRY_WINDOW = 3,    ///< in WINDOW_PALETTE
  /// In palette 1, which the Status screens fill with the basic sprite
  /// colours for their element icons.
  TILEMAP_ENTRY_ICON = 4
};

/// The window map entry of the first element icon, in BASIC_COLORS_PALETTE;
/// the four elements follow it in order (enum Element).
#define ELEMENT_ICON_TILE (BG_TILE_PALETTE(BASIC_COLORS_PALETTE) | 1)

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
  COMMAND_ICON_UPDATE = 31,
  COMMAND_ICON_COINS = 32,
  COMMAND_ICON_LUCKY_MEDALS = 33,
  // The settings menu's choices and the Pause menu's entries
  // (gSettingsMenu_SpeedIcons to gSettingsMenu_SleepIcons), whose command names
  // are placeholders.
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
  COMMAND_ICON_LINK = 45,
  COMMAND_ICON_GOLD = 46,
  COMMAND_ICON_SILVER = 47,
  COMMAND_ICON_BRONZE = 48,
  COMMAND_ICON_SWITCH = 49
};

/// Two copies of a tilemap entry, for filling a run of entries a word at a
/// time.
#define WINDOW_TILE_PAIR(entry) ((u32)(entry) << 16 | (entry))

/// A tilemap entry's tile number, the bits below its flip bits and palette
/// number in the hardware's screen entry.
#define TILE_NUMBER_MASK 0x3ff
/// The tilemap entry flip bits.
#define TILE_HFLIP 0x400
#define TILE_VFLIP 0x800
/// A screen entry of tile n turned half a turn, flipped both ways.
#define FLIP_XY(n) ((n) | TILE_HFLIP | TILE_VFLIP)
/// A screen entry of tile n flipped top to bottom.
#define FLIP_Y(n) ((n) | TILE_VFLIP)

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

/// How a window sprite moves (WindowSprite.motion), run each frame by
/// WindowSprite_UpdateAll; 0 is a free window sprite and
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
  // A hop and back, each in a direction of its own.
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

/// The spinning hand of the selection screen and the battle target cursor,
/// 16 by 16 pixels in 256 colours, 256 bytes a frame (sprite_cursors.s).
extern const u8 gMenu_AnimatedCursorTiles[];

extern const u8 gOwnerStatus_HpString[];
extern const u8 gOwnerStatus_SlashString[];
extern const u8 gOwnerStatus_PpString[];

void Text_SetColor(s32 color);

void Message_DecodeRaw(s32 message, u16 *text, s32 capacity);

s32 Text_GetWideStringWidth(u16 *text);

s32 PauseMenu_Choose(s32 initial);

s32 Save_ToChosenFile(void);

s32 SettingsMenu_Run(void);

void Icon_LoadWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                          s32 palette, s32 reuse);

s32 BattleParty_ChooseReserve(s32 slot);

s32 BattleDjinn_CountStandbyFar(u32 unitId, u8 *counts);

void Window_MarkVisibleTextTiles(void);

s32 Djinn_GetGiveTutorialMessage(void);

/// Nonzero offers Battle on the File Screen, when a file has any game flag
/// counted in its summary (Save_ScanFileSummaries).
extern s16 gTitleBattleOptionEnabled;
/// Nonzero offers Update on the File Screen, when a file has a party whose
/// summary is not marked reunited (Save_ScanFileSummaries).
extern s16 gTitleUpdateOptionEnabled;

s32 Save_ScanFileSummaries(void);

void Text_UseExtraTiles(s32 allowAlternateTiles);

s32 Message_CopyExpanded(s32 key, u16 *destination, u32 capacity);
s32 Message_DecodeAndClearArguments(s32 message);
void Message_DecodeAppend(s32 message);

void MenuCursor_AdvanceFar(struct MenuCursor *cursor);
void MenuCursor_MoveTowardTargetFar(struct MenuCursor *cursor);

void Portrait_CloseWindow(s32 id);

u8 *String_FormatNumber(u8 *buffer, s32 input, s32 width);

void Window_DrawDividerLine(struct Window *win, u32 x1, u32 y1, u32 x2, u32 y2);
void Text_DrawStringInWindow(u8 *text, s32 window, u32 x, u32 y);
void Text_DrawNumberInWindow(s32 value, s32 width, s32 output, s32 x, s32 y);

void MessageArgument_Push(u32 value, u32 kind);

void FileList_PlacePartySprites(void);

/// The pointing hand cursor, a 16x16 sprite, shared by the battle menus, the
/// name entry screen and the talk choices.
extern const u8 gPointingHandTiles[];

s32 Text_FillCanvasBackground(void);

void PartyStatus_OpenAndWait(void);

// The layers ItemIcon_Compose draws over an item's icon.
#define ICON_BASE 1            ///< the base under the icon
#define ICON_COUNT_ABOVE_ONE 2 ///< the count, unless it is 1
#define ICON_COUNT 4           ///< the count, always
/// Mark 1, the cross, on an item with INVENTORY_BROKEN.
#define ICON_BROKEN 8
/// Mark 0, the equip mark, on an item with INVENTORY_EQUIPPED.
#define ICON_EQUIPPED 16
/// Mark 2 on a cursed item that cannot be unequipped (ITEM_CURSED and
/// ITEM_CANNOT_UNEQUIP) when its code has INVENTORY_EQUIPPED.
#define ICON_ARTIFACT 32

/// An action's id in its encoded list entry, below its flags.
#define ACTION_ID_MASK 0x3fff

/// The party status window's flags (PartyStatus_Draw,
/// PartyStatus_CreateWindow): one column of six tiles per party member
/// (name, HP and, when shown, PP with a bar under each), and in battle an
/// optional five-tile column of standby Djinn counts on its left.
enum PartyStatusFlag {
  PARTY_STATUS_HP_ONLY = 0,     ///< names and HP
  PARTY_STATUS_SHOW_PP = 1,     ///< add the PP row
  PARTY_STATUS_SHOW_DJINN = 2,  ///< add the Djinn column (needs the PP row)
  PARTY_STATUS_CLOSED = 9,      ///< erase the window
  PARTY_STATUS_KEEP_FLAGS = -1, ///< redraw with the flags it has
  /// BattleWork.partyStatus while the party cannot summon.
  PARTY_STATUS_NO_SUMMONS = PARTY_STATUS_SHOW_PP,
  /// BattleWork.partyStatus while it can, drawn only while a Djinni is on
  /// standby.
  PARTY_STATUS_SUMMONS = PARTY_STATUS_SHOW_PP | PARTY_STATUS_SHOW_DJINN
};

/// Where the windows are drawn (WindowWork.battleMode).
enum WindowBattleMode {
  WINDOW_FIELD = 0,  ///< on the field; the field setup clears the block
  WINDOW_BATTLE = 1, ///< in battle, from WindowSystem_InitBattle
  /// In battle, while BattleMessage_Open decodes a battle message: the
  /// Japanese decoder then sets names off with spaces and style codes.
  WINDOW_BATTLE_MESSAGE = 2,
  /// A flag with WINDOW_BATTLE: glyphs are drawn blended (semi-transparent),
  /// while the Djinn tutorial's line shows over the command row.
  WINDOW_GLYPHS_BLENDED = 4,
};

/// WindowWork.portraitPictures of a free portrait slot, as Portrait_ClearSlots
/// writes it.
#define PORTRAIT_SLOT_EMPTY 999

/// The window FieldMessage_ShowCentered shows its message in.
enum CenteredMessageMode {
  /// No frame or size: it opens and closes at once.
  CENTERED_MESSAGE_BARE = 0,
  /// A zooming window sized to the message, waited for as it closes.
  CENTERED_MESSAGE_WINDOW = 1
};
/// The colour a bare centred message is written in, which a scene sets white
/// again before each one while the screen is warm-tinted (Mars Lighthouse's
/// inscriptions, the Gabomba Statue's chamber).
#define CENTERED_TEXT_COLOR OBJ_PLTT_COLOR(0, 15)

/// The flags FieldMessage_Show takes.
enum FieldMessageFlag {
  FIELD_MESSAGE_ZOOM = 0x01, ///< the window zooms open and shut
  /// The window work is busy while it shows.
  FIELD_MESSAGE_WAITING_FOR_SOUND = 0x02,
  FIELD_MESSAGE_KEEP_OPEN = 0x04,   ///< the window is left open
  FIELD_MESSAGE_LOWER = 0x08,       ///< 4 tiles below the top rows' centre
  FIELD_MESSAGE_HOLD_UPLOAD = 0x20, ///< the tilemap upload is held off
  FIELD_MESSAGE_LOWEST = 0x40       ///< 12 tiles below the top rows' centre
};

/// The flags a yes-or-no question's message takes: it zooms open, stays open
/// for the Yes/No menu below it and sits lower.
#define QUESTION_MESSAGE                                                       \
  (FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN | FIELD_MESSAGE_LOWER)

/// The Pause menu's entries, as PauseMenu_Choose returns them
/// (counted from Save your quest even when only Change settings shows).
enum PauseMenuEntry {
  PAUSE_MENU_SAVE_QUEST = 0,
  PAUSE_MENU_SLEEP = 1,
  PAUSE_MENU_CHANGE_SETTINGS = 2,
  /// In debug mode, R and L also leave the menu with these; PauseMenu_Run acts
  /// on neither.
  PAUSE_MENU_DEBUG_R = 20,
  PAUSE_MENU_DEBUG_L = 21
};

#define WINDOW_COLOR_COUNT 24 ///< "Window color" takes 0 to 23
/// "Window brightness" takes 0 to 14.
#define WINDOW_BRIGHTNESS_COUNT 15

/// The brightness that adds nothing to the hue's colour.
#define WINDOW_BRIGHTNESS_NEUTRAL 7

/// Steps round gWindowColor_WaveTable's colour wheel; each hue is four.
#define WINDOW_COLOR_WHEEL_STEPS 96

/// How many levels HIGHLIGHT_PALETTE lightens each channel of
/// WINDOW_BACKGROUND_COLOR by (Palette_LightenHighlight).
#define HIGHLIGHT_LIGHTEN_STEPS 9

/// The bob of a prompt or cursor: up and down by up to 2 pixels, a sine every
/// 16 frames.
#define CURSOR_BOB_Y()                                                         \
  (Trig_Sin(gFrameCount * (TRIG_TURN / 16)) / (FIXED_ONE / 2))

/// Message_OpenWindow's packed argument: the speaker's voice in bits 16 to 27,
/// above the window options.
#define MESSAGE_WINDOW_VOICE(voice) ((voice) << 16)
/// The speaker's voice in a packed argument, as Message_OpenWindow takes it
/// out.
#define MESSAGE_WINDOW_VOICE_OF(packed) (((u32)(packed) << 4) >> 20)
/// The window options of a packed argument: the bits below the speaker's
/// voice.
#define MESSAGE_WINDOW_OPTIONS_MASK (MESSAGE_WINDOW_VOICE(1) - 1)

/// Message_OpenWindow's window options, in packed's low bits.
enum MessageWindowOption {
  MESSAGE_WINDOW_ZOOM = 1, ///< zoom open rather than appear at once
  /// Set by the shopkeepers' windows and some others; Message_OpenWindow never
  /// reads it.
  MESSAGE_WINDOW_UNREAD = 2,
  MESSAGE_WINDOW_TEXT_CANVAS = 8,    ///< WINDOW_FLAG_TEXT_CANVAS
  MESSAGE_WINDOW_ANCHOR_BOTTOM = 16, ///< WINDOW_FLAG_ANCHOR_BOTTOM
  MESSAGE_WINDOW_FIXED_X = 32        ///< WINDOW_FLAG_FIXED_X
};

/// Portrait_OpenWindow's window is PORTRAIT_WINDOW_TILES tiles square, a tile
/// wider while minds are read.
#define PORTRAIT_WINDOW_TILES 5

#endif
