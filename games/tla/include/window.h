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

struct UiRenderWork {
  u16 tileCursor;
  u8 alt;
  u8 dirty;
  u8 mode;
  u8 menuState;
  u8 menuBusy;
  u8 level;
  u16 tilemap[20][32];
  struct UiWindow windows[UI_WINDOW_COUNT];
  struct UiChannelSlot channels[3];
  struct RenderOutput outputs[UI_OUTPUT_COUNT];
  struct RenderOutputList freeOutputs;
  u8 tileAttributes[0x100];
  u16 lineSpacing;
  u16 unknownAfterLineSpacing;
  u16 outline;
  u16 colour;
  u16 entries[UI_TEXT_ENTRY_COUNT];
  u16 glyphSpacing;
  u16 count;
  u16 unknownAfterCount;
  u16 glyphResource;
  u16 glyphTiles;
  u8 unknownBeforeValues[2];
  u32 values[8];
  u16 names[8];
  u16 resourceIds[2];
  u16 outputIds[2];
  u16 speakerVoiceSound;
  u16 speakerVoiceCountdown;
  u8 unk1388;
  u8 busy;
  u8 unk138a[2];
  s8 textMode;
  u8 unk138d[3];
};

STATIC_ASSERT(offsetof(struct UiRenderWork, tilemap) == 8);
STATIC_ASSERT(offsetof(struct UiRenderWork, windows) == 0x508);
STATIC_ASSERT(offsetof(struct UiRenderWork, channels) == 0x6b8);
STATIC_ASSERT(offsetof(struct UiRenderWork, outputs) == 0x730);
STATIC_ASSERT(offsetof(struct UiRenderWork, freeOutputs.head) == 0xe30);
STATIC_ASSERT(offsetof(struct UiRenderWork, freeOutputs.tailLink) == 0xe34);

STATIC_ASSERT(offsetof(struct UiRenderWork, lineSpacing) == 0xf38);
STATIC_ASSERT(offsetof(struct UiRenderWork, colour) == 0xf3e);
STATIC_ASSERT(offsetof(struct UiRenderWork, entries) == 0xf40);
STATIC_ASSERT(offsetof(struct UiRenderWork, menuState) == 5);
STATIC_ASSERT(offsetof(struct UiRenderWork, level) == 7);
STATIC_ASSERT(offsetof(struct UiRenderWork, tileAttributes) == 0xe38);
#if (LANGUAGE != LANGUAGE_JAPANESE)
STATIC_ASSERT(offsetof(struct UiRenderWork, count) == 0x1342);
STATIC_ASSERT(offsetof(struct UiRenderWork, glyphResource) == 0x1346);
STATIC_ASSERT(offsetof(struct UiRenderWork, values) == 0x134c);
STATIC_ASSERT(offsetof(struct UiRenderWork, names) == 0x136c);
STATIC_ASSERT(sizeof(struct UiRenderWork) == 0x1390);
#else
STATIC_ASSERT(offsetof(struct UiRenderWork, count) == 0x1142);
STATIC_ASSERT(offsetof(struct UiRenderWork, glyphResource) == 0x1146);
STATIC_ASSERT(offsetof(struct UiRenderWork, values) == 0x114c);
STATIC_ASSERT(offsetof(struct UiRenderWork, names) == 0x116c);
STATIC_ASSERT(sizeof(struct UiRenderWork) == 0x1190);
#endif

void UiWork_InitFreeList(void);

#endif
