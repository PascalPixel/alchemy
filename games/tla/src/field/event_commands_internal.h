#ifndef ALCHEMY_EVENT_COMMANDS_INTERNAL_H
#define ALCHEMY_EVENT_COMMANDS_INTERNAL_H

/// @file
/// What every part of event_commands.c shares: its includes, and the
/// declarations its parts call before their definitions or once declared
/// differently. event_commands.c includes it before the parts, and each part
/// includes it too, so that a part read on its own resolves. It places no
/// bytes.

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

// The object's functions its parts call before their definitions.
struct FacingObject;
s32 Func_080d295c(s32 a);
struct Actor *Actor_GetById(u32 objectId);
void Actor_SetHorizontalPositionWithTerrain(u32 objectId, s32 x, s32 z);
void Actor_MoveToObject(u32 objectId, u32 otherId);
void Actor_SetAngleToward(s32 firstId, s32 secondId, s32 frames);
void FacingObject_TurnPairToFaceEachOther(struct FacingObject *first,
                                          struct FacingObject *second);
void Actor_ApplyIndexedChildValue(struct Actor *object);
void Actor_SetPartAttribute(struct Actor *object, s32 attribute);

// Outside routines the parts had declared each in its own way, with the one
// type they share (the definition's, where it is C).
struct Actor;
struct ActionDescriptor;
s32 GameFlag_Test(s32 flag);
void FieldMessage_ShowFar(s32 message, s32 mode);
void MessageArgument_PushFar(s32 value, s32 slot);
void Sound_PlayFar(s32 cue);
s32 ArcTan2(s32 y, s32 x);
struct Actor *ObjectTable_Get(u32 objectId);
s32 Encounter_ResolveWorldMapZoneAndBackdrop(void *position);
void BattleEffect_SelectBattleCue(s32 zone, s32 slot);

#endif
