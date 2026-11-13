#ifndef ALCHEMY_WINDOW_H
#define ALCHEMY_WINDOW_H

/// @file
/// Windows, the messages that type into them, and the window renderer's
/// work.

#include "module/window/window.h"
#include "module/window/screens.h"
#include "types.h"
#include "tbs_edition.h"
#include "static_assert.h"
#include "render_input.h"
#include "heap.h"

/// Where the windows are drawn (WindowWork.battleMode). In battle, typing
/// speeds up when a frame overruns, and the bottom rows of a cleared
/// rectangle keep the battle tile.
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
  /// The animation a message's TEXT_WINDOW_STATE code sets. Only
  /// WINDOW_ANIMATION_SWELL is acted on; its message types nothing more
  /// until the swell ends and clears it.
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
///
/// It is heap slot HEAP_SLOT_WINDOW (gWindowWork), which
/// WindowSystem_InitField takes for the field and
/// WindowSystem_InitBattle for battle; it lasts until the next scene change.
/// Japanese has more windows and a shorter text ring than the other editions
/// (WINDOW_COUNT, TEXT_BUFFER_MASK).
struct WindowWork {
  u16 tilemap[640]; ///< the windows' BG tilemap, 32 by 20 tiles
  struct Window windows[WINDOW_COUNT]; ///< 12 in Japanese, 8 in the others
  // XXX (no evidence): why three messages, and why the international
  // editions gave up four windows when they doubled textBuffer; only Camelot's
  // sources could say how the counts were chosen.
  struct Message messages[3]; ///< the messages, NULL work when free
  struct WindowSprite sprites[WINDOW_SPRITE_COUNT];
  struct WindowSpriteList freeSprites; ///< the sprites not in any window
  /// Per text tile (tiles 128 to 255, then 512 to 639): bit 0 once
  /// allocated, bit 1 while the tilemap shows it, 0 when free. The first
  /// three hold the bottom border tiles' lower rows and are 4.
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

#endif
