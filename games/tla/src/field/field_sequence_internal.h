#ifndef ALCHEMY_FIELD_SEQUENCE_INTERNAL_H
#define ALCHEMY_FIELD_SEQUENCE_INTERNAL_H

/// @file
/// What every part of field_sequence.c shares: its includes, and the
/// declarations its parts call before their definitions or once declared
/// differently. field_sequence.c includes it before the parts, and each part
/// includes it too, so that a part read on its own resolves. It places no
/// bytes.

// battle.h declares Actor_Destroy with its battle view; these parts pass the
// actor itself, declared below.
#define OWN_Actor_Destroy

#include "types.h"
#include "field/world_map.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_ids.h"
#include "scene_tables.h"
#include "actor.h"
#include "event_work.h"
#include "map.h"
#include "field_sprite.h"
#include "dma.h"
#include "io_reg.h"
#include "system/vector3.h"
#include "owner_value.h"
#include "battle_unit.h"
#include "owner_state.h"
#include "fixed_math.h"
#include "range_falloff.h"
#include "field_event.h"
#include "input.h"
#include "macros.h"
#include "version.h"
#include "field/event_work.h"
#include "scene.h"
#include "object_effect.h"
#include "item.h"
#include "battle_system.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "actor_script_view.h"
#include "objdisp.h"
#include "heap_release.h"
#include "flag_call.h"
#include "display_transition.h"
#include "task.h"
#include "io_write_queue.h"
#include "irq.h"
#include "frame.h"
#include "palette_buffers.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "sprite.h"
#include "resource.h"
#include "vram_tab.h"
#include "io_write.h"

// The object's functions its parts call before their definitions.
void Func_080ca4a8(void);
void Func_080ca514(s32 terrain);
void BattleEffect_SelectBattleCue(s32 zone, s32 slot);
s32 Func_080ca9cc(s32 id, s32 *position);
void ObjectTable_Clear(void);
struct Actor *ObjectTable_Get(u32 objectId);
void BattleParty_ApplyDrain(s32 amount, s32 unused);
void BattleParty_ApplyHealthDelta(s32 amount, s32 scaled);
s32 BattleParty_ApplyStatusDamage(void);
void *Func_080cb8e8(void);
s32 Func_080cc54c(void);
u32 Stream_ReadU16(u8 **cursor);
s16 Stream_ReadS16(u8 **cursor);
void BattleMap_ApplySceneEntranceView(void);
void Debug_RunPaletteEditor(void);

// Outside routines and data the parts had declared each in its own way, with
// the one type they share.
struct Actor;
extern void *gOverlayArea[]; ///< the scene overlay's entry table
extern s8 gData_02001000[];
void Actor_Destroy(struct Actor *object);
void Actor_SetSingleChildFlagsFar(struct Actor *object, u32 flags);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
s32 Map_GetTerrainHeightFar(u8 layer, s32 x, s32 z);
void *Unit_Get(s32 owner);
void wait(s32 frames);
s32 Party_CountActiveOwnersFar(void);
s32 GameFlag_Test(s32 flag);
void *HeapSlot_Alloc(s32 slot, s32 size);
void *HeapSlot_AllocEwram(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
struct Actor *Actor_GetById(s32 actorId);

#endif
