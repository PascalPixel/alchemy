#ifndef ALCHEMY_FIELD_SEQUENCE_INTERNAL_H
#define ALCHEMY_FIELD_SEQUENCE_INTERNAL_H

/// @file
/// What every part of field_sequence.c shares: its includes, and the
/// declarations its parts call before their definitions or once declared
/// differently. field_sequence.c includes it before the parts, and each part
/// includes it too, so that a part read on its own resolves. It places no
/// bytes.

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

// The event work as the parts read it.
struct EventWorkView {
  u8 unk000[0x15a];
  s16 unk15a;
  s16 unk15c;
  s16 unk15e;
  u8 unk160[4];
  s16 unk164;
  s16 unk166;
  u8 unk168[0x1c];
  s16 blockedSteps;
  u16 pushKind;
  u8 unk188[4];
  u8 settings[8];
  s32 unk194;
  s32 unk198;
};

// A row of the field Psynergy target table (gData_080eff28).
struct DescriptorEntry {
  s16 id;
  u8 unk02[2];
  s16 range;
  u8 unk06[2];
};

// A party member's unit as the parts read it.
struct FieldUnit {
  u8 unk00[20];
  s16 hpGauge;
  s16 ppGauge;
  u8 unk18[28];
  s16 maxHp;
  s16 maxPp;
  s16 hp;
  s16 pp;
};

// The object's functions its parts call before their definitions.
struct TriggerView;
void Func_080ca4a8(void);
void BattleBackground_SelectForEnvironment(s32 terrain);
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
s32 GameFlag_IsConditionActive(s32 condition);
struct TriggerView *Func_080ccd78(s32 kind, s32 target);
void *Scene_FindActorTalkTrigger(s32 id);
s32 Func_080ccec8(s32 id);
s32 Scene_RunRaisedEvent(s32 id);
s32 Scene_RunTouchOrExit(s32 value);
s32 Func_080cd680(s32 keys);
s32 Func_080cd808(s32 id);
s32 Field_FindNearbyTalkTarget(s32 id);
s32 Func_080cdc74(void);
void Func_080cdd80(void);
void Event_RunMessage(s32 actor, s32 message);
void Event_ResolveAllPendingActions(void);
void Event_ResolvePendingActions(s32 skip);
s32 Party_GetLeaderActor(void);
s32 Field_UseItem(s32 arg, s32 slot);
s32 Event_ExecutePackedAction(u32 packed);
s32 Func_080ce61c(u32 code);
struct PendingEvent;
s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode, s32 target);
s16 *Func_080ceba8(void);
void Func_080cec34(void);
s32 Actor_GetSpriteShape(u32 objectId);
void ObjectEvent_SetMode5WithCue(u32 objectId);
void ObjectEvent_Spring(u32 objectId);
void ObjectEvent_SetMode4WithCue(u32 objectId);
void ObjectEvent_SetMode2(u32 objectId);
void ObjectEvent_SetMode6WithCue(u32 objectId);
struct Actor *ObjectEvent_GetObject(u32 objectId);
void ObjectEvent_Remove(u32 objectId);

// Outside routines and data the parts had declared each in its own way, with
// the one type they share.
struct Actor;
extern void *gOverlayArea[]; ///< the scene overlay's entry table
extern s8 gData_02001000[];
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
s32 Map_GetTerrainHeightFar(u8 layer, s32 x, s32 z);
void wait(s32 frames);
s32 Party_CountActiveOwnersFar(void);
s32 GameFlag_Test(s32 flag);
void *HeapSlot_Alloc(s32 slot, s32 size);
void *HeapSlot_AllocEwram(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
struct Actor *Actor_GetById(u32 objectId);
s32 Func_08020310(s32 x, s32 z, s32 layer, s32 height);
void FieldMessage_ShowFar(s32 message, s32 mode);
void MessageArgument_PushFar(s32 value, s32 kind);
void BattleEffect_LoadActionEffectResources(s32 action, s32 flags);
void Actor_ReleaseIfPresent(struct Actor *object);
void ItemDisplay_RaiseAboveLeader(struct Actor *object);
struct ParticleEmitter;
void Marker_MoveLeaderAwayFromEmitter(struct ParticleEmitter *emitter);

#endif
