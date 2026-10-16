#ifndef ALCHEMY_WINDOW_H
#define ALCHEMY_WINDOW_H

#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "types.h"
#include "tbs_edition.h"
#include "layout_guard.h"
#include "uiflow.h"
#include "output.h"

enum { UI_OUTPUT_COUNT = 64 };

struct UiRenderWork {
  u16 tilemap[640];
  struct UiWindow windows[WINDOW_COUNT];
  struct UiChannelSlot channels[3];
  struct RenderOutput outputs[UI_OUTPUT_COUNT];
  struct RenderOutputList freeOutputs;
  u8 tileAttributes[256];
  u16 tileCursor;
  u8 alt;
  u8 dirty;
  u8 mode;
  u8 menuState;
  u8 menuBusy;
  u8 level;
  u16 lineSpacing;
  u16 unknownAfterLineSpacing;
  u16 outline;
  u16 colour;
  u16 entries[RENDER_ENTRY_MASK + 1];
  u16 unknownBeforeCount;
  u16 count;
  u16 messageStart;
  u16 glyphResource;
  u16 glyphTiles;
  u8 unknownBeforeValues[RENDER_VALUE_TABLE_OFFSET - RENDER_COUNTER_OFFSET - 4];
  u32 values[8];
  u16 names[8];
  u16 resourceIds[2];
  u16 outputIds[2];
  u16 speakerVoiceSound;
  u16 speakerVoiceCountdown;
  u8 pageArrowShown;
  u8 busy;
  u8 spacingBefore;
  u8 spacingAfter;
};

LAYOUT_OFFSET_GUARD(UiRenderWork_Windows, struct UiRenderWork, windows, 0x500);
LAYOUT_OFFSET_GUARD(UiRenderWork_Channels, struct UiRenderWork, channels,
                    RENDER_CHANNEL_OFFSET);
LAYOUT_OFFSET_GUARD(UiRenderWork_Outputs, struct UiRenderWork, outputs,
                    RENDER_OUTPUT_TABLE_OFFSET);
LAYOUT_OFFSET_GUARD(UiRenderWork_FreeHead, struct UiRenderWork,
                    freeOutputs.head, RENDER_FREE_HEAD_OFFSET);
LAYOUT_OFFSET_GUARD(UiRenderWork_FreeTail, struct UiRenderWork,
                    freeOutputs.tailLink, RENDER_FREE_TAIL_OFFSET);
LAYOUT_OFFSET_GUARD(UiRenderWork_TileAttributes, struct UiRenderWork,
                    tileAttributes, RENDER_TILE_ATTR_OFFSET);
LAYOUT_OFFSET_GUARD(UiRenderWork_TileCursor, struct UiRenderWork, tileCursor,
                    RENDER_ALT_OFFSET - 2);
LAYOUT_OFFSET_GUARD(UiRenderWork_Alt, struct UiRenderWork, alt,
                    RENDER_ALT_OFFSET);
LAYOUT_OFFSET_GUARD(UiRenderWork_Dirty, struct UiRenderWork, dirty,
                    RENDER_DIRTY_OFFSET);
LAYOUT_OFFSET_GUARD(UiRenderWork_LineSpacing, struct UiRenderWork, lineSpacing,
                    RENDER_WORD2_OFFSET);
LAYOUT_OFFSET_GUARD(UiRenderWork_Outline, struct UiRenderWork, outline,
                    RENDER_WORD_OFFSET);
LAYOUT_OFFSET_GUARD(UiRenderWork_Colour, struct UiRenderWork, colour,
                    RENDER_PARAM_OFFSET);
LAYOUT_OFFSET_GUARD(UiRenderWork_Entries, struct UiRenderWork, entries,
                    RENDER_ENTRY_TABLE_OFFSET);
LAYOUT_OFFSET_GUARD(UiRenderWork_MessageStart, struct UiRenderWork,
                    messageStart, RENDER_ENTRY_COUNT_OFFSET + 2);
LAYOUT_OFFSET_GUARD(UiRenderWork_Count, struct UiRenderWork, count,
                    RENDER_ENTRY_COUNT_OFFSET);
LAYOUT_OFFSET_GUARD(UiRenderWork_GlyphResource, struct UiRenderWork,
                    glyphResource, RENDER_COUNTER_OFFSET);
LAYOUT_OFFSET_GUARD(UiRenderWork_GlyphTiles, struct UiRenderWork, glyphTiles,
                    RENDER_COUNTER_OFFSET + 2);
LAYOUT_OFFSET_GUARD(UiRenderWork_Values, struct UiRenderWork, values,
                    RENDER_VALUE_TABLE_OFFSET);
LAYOUT_OFFSET_GUARD(UiRenderWork_Names, struct UiRenderWork, names,
                    RENDER_NAME_TABLE_OFFSET);
LAYOUT_OFFSET_GUARD(UiRenderWork_ResourceIds, struct UiRenderWork, resourceIds,
                    RENDER_SLOT_ID_OFFSET);
LAYOUT_OFFSET_GUARD(UiRenderWork_OutputIds, struct UiRenderWork, outputIds,
                    RENDER_SLOT_VALUE_OFFSET);
LAYOUT_OFFSET_GUARD(UiRenderWork_SpeakerVoiceSound, struct UiRenderWork,
                    speakerVoiceSound, RENDER_RESULT_OFFSET);
LAYOUT_OFFSET_GUARD(UiRenderWork_SpeakerVoiceCountdown, struct UiRenderWork,
                    speakerVoiceCountdown, RENDER_RESULT_OFFSET + 2);
LAYOUT_OFFSET_GUARD(UiRenderWork_Busy, struct UiRenderWork, busy,
                    RENDER_BUSY_OFFSET);
LAYOUT_OFFSET_GUARD(UiRenderWork_SpacingBefore, struct UiRenderWork,
                    spacingBefore, RENDER_BUSY_OFFSET + 1);
LAYOUT_OFFSET_GUARD(UiRenderWork_SpacingAfter, struct UiRenderWork,
                    spacingAfter, RENDER_BUSY_OFFSET + 2);
LAYOUT_SIZE_GUARD(UiRenderWork_Size, struct UiRenderWork, RENDER_WORK_SIZE);

extern u8 *gWindowWork[];

#endif
