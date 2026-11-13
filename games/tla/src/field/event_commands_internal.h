#ifndef ALCHEMY_EVENT_COMMANDS_INTERNAL_H
#define ALCHEMY_EVENT_COMMANDS_INTERNAL_H

/// @file
/// What every part under event_commands/ that field_sequence.c holds shares:
/// its includes, and the declarations its parts call before their definitions
/// or once declared differently. field_sequence.c includes it before those
/// parts, and each part includes it too, so that a part read on its own
/// resolves. It places no bytes.

#include "types.h"
#include "input.h"
#include "ram_buffer.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "event_work.h"
#include "main_routine_work.h"
#include "actor.h"
#include "task.h"
#include "game_flags.h"
#include "fixed_math.h"
#include "owner_state.h"
#include "text/msg_ids.h"
#include "version.h"
#include "inventory.h"
#include "scene.h"
#include "uiwindow.h"
#include "edition.h"
#include "display_transition.h"
#include "field_event.h"
#include "macros.h"
#include "objdisp.h"
#include "sprite.h"
#include "field_sprite.h"
#include "facing_object.h"
#include "actor_view.h"
#include "flag_call.h"
#include "gamepak/far_call/window.h"
#include "module/field/field.h"
#include "module/system/system.h"

// The object's functions its parts call before their definitions.
s32 FieldYesNo_Ask(s32 a);
struct Actor *Actor_GetById(u32 objectId);
void Actor_SetHorizontalPositionWithTerrain(u32 objectId, s32 x, s32 z);
void Actor_MoveToObject(u32 objectId, u32 otherId);
void Actor_FaceActorById(s32 firstId, s32 secondId, s32 frames);

// Outside routines the parts had declared each in its own way, with the one
// type they share (the definition's, where it is C).

#endif
