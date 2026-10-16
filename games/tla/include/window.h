#ifndef ALCHEMY_WINDOW_H
#define ALCHEMY_WINDOW_H

#include "edition.h"
#include "output.h"
#include "uiflow.h"
#include "layout_guard.h"

enum { UI_WINDOW_COUNT = 12, UI_OUTPUT_COUNT = 64 };

#if EDITION_INTERNATIONAL
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

LAYOUT_OFFSET_GUARD(UiRenderWork_Tilemap, struct UiRenderWork, tilemap, 8);
LAYOUT_OFFSET_GUARD(UiRenderWork_Windows, struct UiRenderWork, windows, 0x508);
LAYOUT_OFFSET_GUARD(UiRenderWork_Channels, struct UiRenderWork, channels,
                    0x6b8);
LAYOUT_OFFSET_GUARD(UiRenderWork_Outputs, struct UiRenderWork, outputs, 0x730);
LAYOUT_OFFSET_GUARD(UiRenderWork_FreeHead, struct UiRenderWork,
                    freeOutputs.head, 0xe30);
LAYOUT_OFFSET_GUARD(UiRenderWork_FreeTail, struct UiRenderWork,
                    freeOutputs.tailLink, 0xe34);

LAYOUT_OFFSET_GUARD(UiRenderWork_LineSpacing, struct UiRenderWork, lineSpacing,
                    0xf38);
LAYOUT_OFFSET_GUARD(UiRenderWork_Colour, struct UiRenderWork, colour, 0xf3e);
LAYOUT_OFFSET_GUARD(UiRenderWork_Entries, struct UiRenderWork, entries, 0xf40);
LAYOUT_OFFSET_GUARD(UiRenderWork_MenuState, struct UiRenderWork, menuState, 5);
LAYOUT_OFFSET_GUARD(UiRenderWork_Level, struct UiRenderWork, level, 7);
LAYOUT_OFFSET_GUARD(UiRenderWork_TileAttributes, struct UiRenderWork,
                    tileAttributes, 0xe38);
#if EDITION_INTERNATIONAL
LAYOUT_OFFSET_GUARD(UiRenderWork_Count, struct UiRenderWork, count, 0x1342);
LAYOUT_OFFSET_GUARD(UiRenderWork_GlyphResource, struct UiRenderWork,
                    glyphResource, 0x1346);
LAYOUT_OFFSET_GUARD(UiRenderWork_Values, struct UiRenderWork, values, 0x134c);
LAYOUT_OFFSET_GUARD(UiRenderWork_Names, struct UiRenderWork, names, 0x136c);
LAYOUT_SIZE_GUARD(UiRenderWork_Size, struct UiRenderWork, 0x1390);
#else
LAYOUT_OFFSET_GUARD(UiRenderWork_Count, struct UiRenderWork, count, 0x1142);
LAYOUT_OFFSET_GUARD(UiRenderWork_GlyphResource, struct UiRenderWork,
                    glyphResource, 0x1146);
LAYOUT_OFFSET_GUARD(UiRenderWork_Values, struct UiRenderWork, values, 0x114c);
LAYOUT_OFFSET_GUARD(UiRenderWork_Names, struct UiRenderWork, names, 0x116c);
LAYOUT_SIZE_GUARD(UiRenderWork_Size, struct UiRenderWork, 0x1190);
#endif

void UiWork_InitFreeList(void);

#endif
