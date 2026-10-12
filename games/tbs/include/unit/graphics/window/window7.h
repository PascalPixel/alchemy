#ifndef ALCHEMY_UNIT_GRAPHICS_WINDOW_WINDOW7_H
#define ALCHEMY_UNIT_GRAPHICS_WINDOW_WINDOW7_H

#include "types.h"
#include "edition.h"
#include "glyph.h"

struct RenderInput;
struct RenderOutput;

struct RenderOutput *RenderOutput_Create(s32 slot, s32 flags, struct RenderInput *input, s32 offset_x, s32 offset_y);
struct RenderOutput *RenderOutput_CreateFromResource(s32 mode, s32 resource, struct RenderInput *input, s32 x, s32 y);
struct RenderOutput *RenderOutput_CreateLoaded(s32 code, s32 layers, struct RenderInput *input, s32 x, s32 y);
struct RenderOutput *RenderOutput_CreateWithTransform(s32 icon, struct RenderInput *input, s32 x, s32 y);
struct RenderOutput *RenderOutput_CreateFromTable(s32 table_entry, struct RenderInput *input, s32 x, s32 y);
struct RenderOutput *CreateSideObject(s32 object_kind, s32 position, s32 side, s32 arg3, s32 arg4, s32 arg5);
void Ui_LoadCharacterEntryForSlot(u32 slot, s32 character, s32 value);
void RenderOutput_SetAnimation(struct RenderOutput *out, s32 mode);
void RenderOutput_SetInvertedSentinel(void *arg0, s32 arg1);
void UiWindow_FillFromScene(void *dst);
void UiWindow_FillScreenBlockRect(s32 unused0, s32 unused1, u32 width, u32 height, s32 value);

#endif
