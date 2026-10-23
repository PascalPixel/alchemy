#ifndef ALCHEMY_DEBUG_LEVEL_H
#define ALCHEMY_DEBUG_LEVEL_H

#include "types.h"
#include "owner_state.h"

#include "window.h"

extern struct EventWork *gEventWork;
#include "global_cells.h"

void Engine_ObjectEventSetAnimation();
void Engine_ObjectEventShowEmote();
void Engine_ScriptStart(void);
void Engine_ScriptEnd(void);
void Engine_ObjectEventSetPosition(u32 objectEvent, s32 x, s32 z);
void Engine_ScriptSetMessage();
void Engine_ScriptShowMessage();
void Engine_ScriptRequestWarp();
void Engine_AudioPlayCue();
void Engine_TaskWait();

#endif
