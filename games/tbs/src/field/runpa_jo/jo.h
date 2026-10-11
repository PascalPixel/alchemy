#include "global_cells.h"
#ifndef RUNPA_JO_JO_H
#define RUNPA_JO_JO_H

#include "mapcopy.h"

#include "types.h"
#include "layout_guard.h"
#include "field_scene.h"
#include "system.h"

#include "game_state.h"
#include "field_event.h"
#include "field_service.h"

#ifndef MODERN // modern GCC keeps the headers' volatile declaration
extern u32 gFrameCount;
#endif

s32  Engine_PartyInventoryFindOwner(s32 item_id);
s32  Engine_BattleFxPlayCueAndStartEmitterOnTarget(s32 value, s32 actor, s32 item);
void Engine_BattleFxSetWeightedResult(s32 slot, s32 value);
void Engine_BattleFxRunPageEffectForSlot(s32 slot, s32 first, s32 second);
void Engine_PartySetFields1ceAnd1d0(s32 first, s32 second);
void Object_RefreshSelectorById(s32 actor);
void Engine_MapSetWorkFlagBits9To11(s32 first);

#endif
