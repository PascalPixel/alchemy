#ifndef ALCHEMY_WORKSPACE_OPTIONS_H
#define ALCHEMY_WORKSPACE_OPTIONS_H

#include "unit/graphics/window/window.h"
#include "unit/graphics/window/window8.h"
#include "unit/graphics/window/window9.h"
#include "unit/graphics/window/window10.h"
#include "unit/graphics/window/window11.h"
#include "unit/graphics/window/window12.h"
#include "types.h"
#include "render_input.h"

union MenuCursor
{
	struct RenderOutput *output;
	u8					 data[0x10];
};

struct WorkspaceWork
{
	u8					 unknown_000[0x574];
	u16					 page;
	u16					 tint[3];
	u16					 cursor_frame;
	u16					 preset;
	u8					 unknown_580[0x14];
	s8					 option[5];
	s8					 option_count[5];
	u8					 unknown_59e[6];
	union MenuCursor	 cursor;
	union MenuCursor	 marker[2];
	u8					 unknown_5d4[0x18];
	struct RenderOutput *frame[3][3];
	struct RenderOutput *icon[6];
};

extern struct WorkspaceWork *gSelectionWork;

#endif
