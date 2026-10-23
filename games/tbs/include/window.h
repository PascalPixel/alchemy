#ifndef ALCHEMY_WINDOW_H
#define ALCHEMY_WINDOW_H

/// @file
/// The window renderer's work.

#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "types.h"
#include "tbs_edition.h"
#include "static_assert.h"
#include "uiflow.h"
#include "output.h"

/// Window sprites in the shared pool: every sprite the windows show at once,
/// glyphs, icons and portraits together.
enum { WINDOW_SPRITE_COUNT = 64 };

/// The window system's work: the windows' tilemap, the windows, the three text
/// printers, the 64 window sprites, and the decoded message and its arguments.
///
/// It is heap slot HEAP_SLOT_WINDOW (gWindowWork), which
/// UiWork_InitializeWithResourceCounters takes for the field and
/// UiWork_Initialize for battle; it lasts until the next map change. Its
/// size and offsets differ between Japanese and the other editions
/// (tbs_edition.h's RENDER_ values).
struct WindowWork {
  u16 tilemap[640]; ///< the windows' BG tilemap, 32 by 20 tiles
  struct Window windows[WINDOW_COUNT]; ///< 12 in Japanese, 8 in the others
  struct TextPrinter printers[3]; ///< the text printers, NULL work when free
  struct WindowSprite sprites[WINDOW_SPRITE_COUNT];
  struct WindowSpriteList freeSprites; ///< the sprites not in any window
  /// Per text tile (tiles 0x80 to 0xFF, then 0x200 to 0x27F): bit 0 once
  /// allocated, bit 1 while the tilemap shows it, 0 when free. The first
  /// three hold the bottom frame tiles' lower rows and are 4.
  u8 textTileState[256];
  u16 nextTextTile;  ///< the text tile the next search for a free one starts at
  u8 extraTextTiles; ///< 1 lets text use 256 tiles, adding tiles 0x200 to 0x27F
  /// The tilemap's 4-row bands to upload: bit 0 for all, bit n + 1 for band n.
  u8 dirty;
  /// Set while Mind Read shows thoughts: round frame corners, colour 8 text
  /// with no shadow, and latched button presses to dismiss text.
  u8 mindRead;
  /// 1 from the work's setup; 2 while a battle message is decoded. While
  /// nonzero, typing speeds up with the lag frames shown and the bottom rows
  /// of a cleared rectangle keep the battle tile.
  u8 battleMode;
  u8 holdTilemapUpload; ///< while set, the tilemap is not uploaded
  u8 textPalette;  ///< text tiles' palette; 15 by default, 14 or 5 for low HP
  u16 lineSpacing; ///< the current printer's, copied in to draw
  u16 unknownAfterLineSpacing; ///< set to 1 with the text style; not read
  u16 outline;                 ///< the current printer's, copied in to draw
  u16 colour;                  ///< the current printer's, copied in to draw
  u16 textBuffer[TEXT_BUFFER_MASK + 1]; ///< decoded text, a ring of codes
  u16 letterSpacing;                    ///< pixels between sprite glyphs; 9
  u16 textBufferEnd;    ///< where the next code goes in textBuffer
  u16 lastMessageStart; ///< where the last decoded message began in textBuffer
  u16 glyphResource;    ///< VRAM cache slot of the page arrow; 99 when none
  u16 glyphTiles;       ///< first sprite tile of the glyph sheet (picture 95)
  /// Two bytes that align values to a word, in every edition; nothing reads
  /// them.
  u8 padding[WINDOW_WORK_ARGUMENT_VALUES_OFFSET - WINDOW_WORK_COUNTER_OFFSET -
             4];
  /// Message argument values (MessageArgument_Push). A portrait's release
  /// writes its palette slot state at WINDOW_WORK_PALETTE_TABLE_OFFSET, in
  /// here.
  u32 argumentValues[8];
  u16 argumentKinds[8]; ///< the MESSAGE_ARGUMENT_ kind of each value; 0 for a
                        ///< free slot
  u16 portraitPictures[2]; ///< picture shown in each portrait slot; 999 when
                           ///< free
  u16 portraitSprites[2];  ///< VRAM cache slot of each portrait slot's picture
  u16 voiceSound;          ///< first of four voice blip sounds; 0 for none
  u16 voiceCountdown;      ///< characters until the next voice blip
  u8 pageArrowShown;       ///< 1 while the next-page arrow is shown
  /// Set while a message waits on its sound: the text ends its pauses and
  /// pages once the sound is over.
  u8 waitingForSound;
  /// 1 for halting speech: a pause before each character, broken off with an
  /// ellipsis after the first word (UiWork_SetBusyFlags bit 0).
  u8 haltingSpeech;
  /// 1 puts code 222 after certain characters in English and Japanese
  /// (UiWork_SetBusyFlags bit 1).
  u8 spacingAfter;
};

STATIC_ASSERT(offsetof(struct WindowWork, windows) == 0x500);
STATIC_ASSERT(offsetof(struct WindowWork, printers) ==
              WINDOW_WORK_PRINTERS_OFFSET);
STATIC_ASSERT(offsetof(struct WindowWork, sprites) ==
              WINDOW_WORK_SPRITES_OFFSET);
STATIC_ASSERT(offsetof(struct WindowWork, freeSprites.head) ==
              WINDOW_WORK_FREE_HEAD_OFFSET);
STATIC_ASSERT(offsetof(struct WindowWork, freeSprites.tailLink) ==
              WINDOW_WORK_FREE_TAIL_OFFSET);
STATIC_ASSERT(offsetof(struct WindowWork, textTileState) ==
              WINDOW_WORK_TEXT_TILE_STATE_OFFSET);
STATIC_ASSERT(offsetof(struct WindowWork, nextTextTile) ==
              (WINDOW_WORK_EXTRA_TEXT_TILES_OFFSET - 2));
STATIC_ASSERT(offsetof(struct WindowWork, extraTextTiles) ==
              WINDOW_WORK_EXTRA_TEXT_TILES_OFFSET);
STATIC_ASSERT(offsetof(struct WindowWork, dirty) == WINDOW_WORK_DIRTY_OFFSET);
STATIC_ASSERT(offsetof(struct WindowWork, lineSpacing) ==
              WINDOW_WORK_WORD2_OFFSET);
STATIC_ASSERT(offsetof(struct WindowWork, outline) == WINDOW_WORK_WORD_OFFSET);
STATIC_ASSERT(offsetof(struct WindowWork, colour) ==
              WINDOW_WORK_PARAMETER_OFFSET);
STATIC_ASSERT(offsetof(struct WindowWork, textBuffer) ==
              WINDOW_WORK_TEXT_BUFFER_OFFSET);
STATIC_ASSERT(offsetof(struct WindowWork, lastMessageStart) ==
              (WINDOW_WORK_TEXT_BUFFER_END_OFFSET + 2));
STATIC_ASSERT(offsetof(struct WindowWork, textBufferEnd) ==
              WINDOW_WORK_TEXT_BUFFER_END_OFFSET);
STATIC_ASSERT(offsetof(struct WindowWork, glyphResource) ==
              WINDOW_WORK_COUNTER_OFFSET);
STATIC_ASSERT(offsetof(struct WindowWork, glyphTiles) ==
              (WINDOW_WORK_COUNTER_OFFSET + 2));
STATIC_ASSERT(offsetof(struct WindowWork, argumentValues) ==
              WINDOW_WORK_ARGUMENT_VALUES_OFFSET);
STATIC_ASSERT(offsetof(struct WindowWork, argumentKinds) ==
              WINDOW_WORK_ARGUMENT_KINDS_OFFSET);
STATIC_ASSERT(offsetof(struct WindowWork, portraitPictures) ==
              WINDOW_WORK_PORTRAIT_PICTURES_OFFSET);
STATIC_ASSERT(offsetof(struct WindowWork, portraitSprites) ==
              WINDOW_WORK_PORTRAIT_SPRITES_OFFSET);
STATIC_ASSERT(offsetof(struct WindowWork, voiceSound) ==
              WINDOW_WORK_VOICE_SOUND_OFFSET);
STATIC_ASSERT(offsetof(struct WindowWork, voiceCountdown) ==
              (WINDOW_WORK_VOICE_SOUND_OFFSET + 2));
STATIC_ASSERT(offsetof(struct WindowWork, waitingForSound) ==
              WINDOW_WORK_WAITING_FOR_SOUND_OFFSET);
STATIC_ASSERT(offsetof(struct WindowWork, haltingSpeech) ==
              (WINDOW_WORK_WAITING_FOR_SOUND_OFFSET + 1));
STATIC_ASSERT(offsetof(struct WindowWork, spacingAfter) ==
              (WINDOW_WORK_WAITING_FOR_SOUND_OFFSET + 2));
STATIC_ASSERT(sizeof(struct WindowWork) == WINDOW_WORK_SIZE);

/// The window renderer's work, heap slot HEAP_SLOT_WINDOW under its own name.
extern u8 *gWindowWork[];

#endif
