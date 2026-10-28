#ifndef ALCHEMY_WINDOW_H
#define ALCHEMY_WINDOW_H

#include "edition.h"
#include "output.h"
#include "uiflow.h"
#include "static_assert.h"

enum { UI_WINDOW_COUNT = 12, UI_OUTPUT_COUNT = 64 };

#if (LANGUAGE != LANGUAGE_JAPANESE)
enum { UI_TEXT_ENTRY_COUNT = 512 };
#else
enum { UI_TEXT_ENTRY_COUNT = 256 };
#endif

/// The window system's work, as ☀️'s with its bytes before the tilemap: the
/// windows' tilemap, the windows, the three text channels, the 64 render
/// outputs, and the decoded message and its arguments.
struct WindowWork {
  u16 tileCursor; ///< the text tile the next search for a free one starts at
  u8 alt;         ///< 1 lets text use the second range of tiles
  /// The tilemap's 4-row bands to upload: bit 0 for all, bit n + 1 for band n.
  u8 dirty;
  u8 mode;      ///< nonzero widens a message window by 2
  u8 menuState; ///< 1 from the work's setup
  u8 menuBusy;  ///< while set, the tilemap is not uploaded
  u8 level;     ///< text tiles' palette; 15 by default
  u16 tilemap[20][32]; ///< the windows' BG tilemap
  struct Window windows[UI_WINDOW_COUNT];
  struct UiChannelSlot channels[3];
  struct WindowSprite outputs[UI_OUTPUT_COUNT];
  struct WindowSpriteList freeOutputs; ///< the outputs not in any window
  u8 tileAttributes[0x100];            ///< per text tile, as ☀️'s
  u16 lineSpacing;             ///< the current channel's, copied in to draw
  u16 unknownAfterLineSpacing; ///< set to 1 with the text style; not read
  u16 outline;                 ///< the current channel's, copied in to draw
  u16 colour;                  ///< the current channel's, copied in to draw
  u16 entries[UI_TEXT_ENTRY_COUNT]; ///< decoded text, a ring of codes
  u16 glyphSpacing;      ///< 9 with the text style, as ☀️'s letterSpacing
  u16 count;             ///< where the next code goes in entries
  u16 unknownAfterCount; ///< not named by ⚓️ code
  u16 glyphResource;     ///< VRAM cache slot of the page arrow; 99 when none
  u16 glyphTiles;        ///< first sprite tile of the glyph sheet (picture 95)
  u8 unknownBeforeValues[2]; ///< not named by ⚓️ code
  u32 values[8];             ///< message argument values
  u16 names[8]; ///< the kind of each value; 0 for a free slot
  u16 resourceIds[2]; ///< picture shown in each portrait slot; 999 when free
  u16 outputIds[2];   ///< VRAM cache slot of each portrait slot's picture
  u16 speakerVoiceSound;     ///< first of four voice blip sounds; 0 for none
  u16 speakerVoiceCountdown; ///< characters until the next voice blip
  u8 unk1388; ///< 1 while the next-page arrow is shown (☀️'s pageArrowShown)
  u8 busy;    ///< set while a message waits on its sound
  u8 unk138a[2]; ///< cleared together, as ☀️'s spacingBefore and spacingAfter
  /// Nonzero lets a tall message fit in 32 pixels, its channel then placing
  /// glyphs by y / 320 (UiChannelSlot.unk26).
  s8 textMode;
  u8 unk138d[3]; ///< not named by ⚓️ code
};

STATIC_ASSERT(offsetof(struct WindowWork, tilemap) == 8);
STATIC_ASSERT(offsetof(struct WindowWork, windows) == 0x508);
STATIC_ASSERT(offsetof(struct WindowWork, channels) == 0x6b8);
STATIC_ASSERT(offsetof(struct WindowWork, outputs) == 0x730);
STATIC_ASSERT(offsetof(struct WindowWork, freeOutputs.head) == 0xe30);
STATIC_ASSERT(offsetof(struct WindowWork, freeOutputs.tailLink) == 0xe34);

STATIC_ASSERT(offsetof(struct WindowWork, lineSpacing) == 0xf38);
STATIC_ASSERT(offsetof(struct WindowWork, colour) == 0xf3e);
STATIC_ASSERT(offsetof(struct WindowWork, entries) == 0xf40);
STATIC_ASSERT(offsetof(struct WindowWork, menuState) == 5);
STATIC_ASSERT(offsetof(struct WindowWork, level) == 7);
STATIC_ASSERT(offsetof(struct WindowWork, tileAttributes) == 0xe38);
#if (LANGUAGE != LANGUAGE_JAPANESE)
STATIC_ASSERT(offsetof(struct WindowWork, count) == 0x1342);
STATIC_ASSERT(offsetof(struct WindowWork, glyphResource) == 0x1346);
STATIC_ASSERT(offsetof(struct WindowWork, values) == 0x134c);
STATIC_ASSERT(offsetof(struct WindowWork, names) == 0x136c);
STATIC_ASSERT(sizeof(struct WindowWork) == 0x1390);
#else
STATIC_ASSERT(offsetof(struct WindowWork, count) == 0x1142);
STATIC_ASSERT(offsetof(struct WindowWork, glyphResource) == 0x1146);
STATIC_ASSERT(offsetof(struct WindowWork, values) == 0x114c);
STATIC_ASSERT(offsetof(struct WindowWork, names) == 0x116c);
STATIC_ASSERT(sizeof(struct WindowWork) == 0x1190);
#endif

void WindowSprite_InitializeFreeList(void);

#endif
