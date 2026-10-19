#ifndef ALCHEMY_WORKSPACE_OPTIONS_H
#define ALCHEMY_WORKSPACE_OPTIONS_H

/// @file
/// The options screen's work and the selection work.

#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "types.h"
#include "render_input.h"
#include "cursor.h"

struct WorkspaceWork {
  u8 unk000[0x574];
  u16 page;
  u16 tint[3];
  u16 cursorFrame;
  u16 preset;
  u8 unk580[0x14];
  s8 option[5];
  s8 optionCount[5];
  u8 unk59e[6];
  union MenuCursor cursor;
  union MenuCursor marker[2];
  u8 unk5d4[0x18];
  struct RenderOutput *frame[3][3];
  struct RenderOutput *icon[6];
};

extern struct WorkspaceWork *gSelectionWork;

#endif
