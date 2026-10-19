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

enum { UI_OUTPUT_COUNT = 64 };

/// The window system's work: the windows' tilemap, the windows, the three text
/// channels, the 64 render outputs, and the decoded message and its arguments.
struct UiRenderWork {
  u16 tilemap[640]; ///< the windows' BG tilemap, 32 by 20 tiles
  struct UiWindow windows[WINDOW_COUNT];
  struct UiChannelSlot channels[3];
  struct RenderOutput outputs[UI_OUTPUT_COUNT];
  struct RenderOutputList freeOutputs; ///< the outputs not in any window
  /// Per text tile (tiles 0x80 to 0xFF, then 0x200 to 0x27F): bit 0 once
  /// allocated, bit 1 while the tilemap shows it, 0 when free. The first
  /// three hold the bottom frame tiles' lower rows and are 4.
  u8 tileAttributes[256];
  u16 tileCursor; ///< the text tile the next search for a free one starts at
  u8 alt;         ///< 1 lets text use 256 tiles, adding tiles 0x200 to 0x27F
  /// The tilemap's 4-row bands to upload: bit 0 for all, bit n + 1 for band n.
  u8 dirty;
  /// Set while Mind Read shows thoughts: round frame corners, colour 8 text
  /// with no shadow, and latched button presses to dismiss text.
  u8 mode;
  /// 1 from the work's setup; 2 while a battle message is decoded. While
  /// nonzero, typing speeds up with the lag frames shown and the bottom rows
  /// of a cleared rectangle keep the battle tile.
  u8 menuState;
  u8 menuBusy;     ///< while set, the tilemap is not uploaded
  u8 level;        ///< text tiles' palette; 15 by default, 14 or 5 for low HP
  u16 lineSpacing; ///< the current channel's, copied in to draw
  u16 unknownAfterLineSpacing; ///< set to 1 with the text style; not read
  u16 outline;                 ///< the current channel's, copied in to draw
  u16 colour;                  ///< the current channel's, copied in to draw
  u16 entries[RENDER_ENTRY_MASK + 1]; ///< decoded text, a ring of codes
  u16 letterSpacing;                  ///< pixels between sprite glyphs; 9
  u16 count;                          ///< where the next code goes in entries
  u16 messageStart;  ///< where the last decoded message began in entries
  u16 glyphResource; ///< VRAM cache slot of the page arrow; 99 when none
  u16 glyphTiles;    ///< first sprite tile of the glyph sheet (picture 95)
  /// Not named by TBS code.
  u8 unknownBeforeValues[RENDER_VALUE_TABLE_OFFSET - RENDER_COUNTER_OFFSET - 4];
  /// Message argument values (UiWork_PushValueSlot). A portrait's release
  /// writes its palette slot state at RENDER_PALETTE_TABLE_OFFSET, in here.
  u32 values[8];
  u16 names[8];          ///< the MSG_ARG_ kind of each value; 0 for a free slot
  u16 resourceIds[2];    ///< picture shown in each portrait slot; 999 when free
  u16 outputIds[2];      ///< VRAM cache slot of each portrait slot's picture
  u16 speakerVoiceSound; ///< first of four voice blip sounds; 0 for none
  u16 speakerVoiceCountdown; ///< characters until the next voice blip
  u8 pageArrowShown;         ///< 1 while the next-page arrow is shown
  /// Set while a message waits on its sound: the text ends its pauses and
  /// pages once the sound is over.
  u8 busy;
  /// 1 for halting speech: a pause before each character, broken off with an
  /// ellipsis after the first word (UiWork_SetBusyFlags bit 0).
  u8 spacingBefore;
  /// 1 puts code 222 after certain characters in English and Japanese
  /// (UiWork_SetBusyFlags bit 1).
  u8 spacingAfter;
};

STATIC_ASSERT(offsetof(struct UiRenderWork, windows) == 0x500);
STATIC_ASSERT(offsetof(struct UiRenderWork, channels) == RENDER_CHANNEL_OFFSET);
STATIC_ASSERT(offsetof(struct UiRenderWork, outputs) ==
              RENDER_OUTPUT_TABLE_OFFSET);
STATIC_ASSERT(offsetof(struct UiRenderWork, freeOutputs.head) ==
              RENDER_FREE_HEAD_OFFSET);
STATIC_ASSERT(offsetof(struct UiRenderWork, freeOutputs.tailLink) ==
              RENDER_FREE_TAIL_OFFSET);
STATIC_ASSERT(offsetof(struct UiRenderWork, tileAttributes) ==
              RENDER_TILE_ATTR_OFFSET);
STATIC_ASSERT(offsetof(struct UiRenderWork, tileCursor) ==
              (RENDER_ALT_OFFSET - 2));
STATIC_ASSERT(offsetof(struct UiRenderWork, alt) == RENDER_ALT_OFFSET);
STATIC_ASSERT(offsetof(struct UiRenderWork, dirty) == RENDER_DIRTY_OFFSET);
STATIC_ASSERT(offsetof(struct UiRenderWork, lineSpacing) ==
              RENDER_WORD2_OFFSET);
STATIC_ASSERT(offsetof(struct UiRenderWork, outline) == RENDER_WORD_OFFSET);
STATIC_ASSERT(offsetof(struct UiRenderWork, colour) == RENDER_PARAM_OFFSET);
STATIC_ASSERT(offsetof(struct UiRenderWork, entries) ==
              RENDER_ENTRY_TABLE_OFFSET);
STATIC_ASSERT(offsetof(struct UiRenderWork, messageStart) ==
              (RENDER_ENTRY_COUNT_OFFSET + 2));
STATIC_ASSERT(offsetof(struct UiRenderWork, count) ==
              RENDER_ENTRY_COUNT_OFFSET);
STATIC_ASSERT(offsetof(struct UiRenderWork, glyphResource) ==
              RENDER_COUNTER_OFFSET);
STATIC_ASSERT(offsetof(struct UiRenderWork, glyphTiles) ==
              (RENDER_COUNTER_OFFSET + 2));
STATIC_ASSERT(offsetof(struct UiRenderWork, values) ==
              RENDER_VALUE_TABLE_OFFSET);
STATIC_ASSERT(offsetof(struct UiRenderWork, names) == RENDER_NAME_TABLE_OFFSET);
STATIC_ASSERT(offsetof(struct UiRenderWork, resourceIds) ==
              RENDER_SLOT_ID_OFFSET);
STATIC_ASSERT(offsetof(struct UiRenderWork, outputIds) ==
              RENDER_SLOT_VALUE_OFFSET);
STATIC_ASSERT(offsetof(struct UiRenderWork, speakerVoiceSound) ==
              RENDER_RESULT_OFFSET);
STATIC_ASSERT(offsetof(struct UiRenderWork, speakerVoiceCountdown) ==
              (RENDER_RESULT_OFFSET + 2));
STATIC_ASSERT(offsetof(struct UiRenderWork, busy) == RENDER_BUSY_OFFSET);
STATIC_ASSERT(offsetof(struct UiRenderWork, spacingBefore) ==
              (RENDER_BUSY_OFFSET + 1));
STATIC_ASSERT(offsetof(struct UiRenderWork, spacingAfter) ==
              (RENDER_BUSY_OFFSET + 2));
STATIC_ASSERT(sizeof(struct UiRenderWork) == RENDER_WORK_SIZE);

extern u8 *gWindowWork[];

#endif
