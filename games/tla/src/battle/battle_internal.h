#ifndef ALCHEMY_BATTLE_INTERNAL_H
#define ALCHEMY_BATTLE_INTERNAL_H

/// @file
/// What every part of the battle object shares: its includes, in their
/// original order, and the names two or more parts use. battle.c includes it
/// before the parts, and each part includes it too, so that a part read on its
/// own still resolves. It places no bytes.

#include "types.h"
#include "owner_state.h"
#include "input.h"
#include "main_routine_work.h"
#include "fixed_point_position.h"
#include "iwram_call.h"
#include "battle_display.h"
#include "battle_work.h"
#include "ram_buffer.h"
#include "serial.h"
#include "text/msg_ids.h"
#include "battle_side.h"
#include "dma.h"
#include "io_reg.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "battle_system.h"
#include "battle_type.h"
#include "battle_summon.h"
#include "battle_unit.h"
#include "heap_release.h"
#include "tla_edition.h"
#include "edition.h"
#include "battle_action.h"
#include "motobj.h"
#include "macros.h"
#include "fixed_math.h"
#include "version.h"
#include "character_ids.h"
#include "objdisp.h"
#include "resource.h"
#include "sprite.h"
#include "sprite_metadata.h"
#include "task.h"
#include "scene.h"
#include "actor.h"
#include "battle_obj.h"
#include "battle_motion.h"
#include "battle_message.h"
#include "battle_escape.h"
#include "battle_target.h"
#include "actor_view.h"
#include "minigame.h"
#include "battle_random.h"
#include "irq_lock.h"
#include "battle_event.h"
#include "module/battle/battle.h"
#include "module/system/system.h"

// Far calls the parts make, each passing its own view of the record:
// ResourceMetadata_Register and SpriteAnimation_Start (field/resource_slot.c)
// take the metadata slot state and the animation entry.
s32 ResourceMetadata_RegisterFar(void *state, s32 id);
void Sprite_RemoveAnimationFar(void *state, s32 handle);

#include "battle_type.h"
#include "field/sprite.h"

struct BattleSortedUnitEntry {
  u16 unitId;
  u16 unk02;
  s16 value;
  s16 width;
  s16 mode;
  s16 priority;
  u8 unk0c[4];
};

struct BattleObjectAnimation {
  u8 unk00[0x28];
  struct AnimationEntry *entry;
};

struct SnapshotVitals {
  u16 hp;
  u16 pp;
};

struct Snapshot {
  u8 listed[8];
  u8 tags[3][4];
  u8 unk14[0x80];
  struct SnapshotVitals vitals[5];
};

struct SnapshotUnit {
  u8 unk00[0x38];
  s16 hp;
  u16 unk3a;
  u8 unk3c[0xf0];
  u8 tags[4];
  u8 snapshot[25];
};

struct SnapshotCopy {
  u8 bytes[25];
};

struct DebugPartyEntry {
  s8 owner;
  u8 level;
  s8 djinnCounts[4];
  u16 items[6];
  u16 actions[2];
  u16 unk16;
};

struct LinkOfferEntry {
  u8 unk00[2];
  u8 unitId;
  u8 unk03;
};

struct LinkOfferList {
  struct LinkOfferEntry entries[72];
  s32 count;
};

struct LinkOfferTable {
  u8 unk00[8];
  struct LinkOfferList list;
};

struct LinkUnit {
  u8 unk000[0x12a];
  u8 side;
};

struct SlotEffectObject {
  u8 unk00[0x50];
  void *resource;
  u8 kind;
};

struct SlotEffectEntry {
  struct SlotEffectObject *object;
};

struct SlotMotionRecord {
  u8 unk00[12];
  s32 height;
};

struct SummonOwnerState {
  u8 unk000[0x14a];
  u16 summon;
};

struct StatusIconRecord {
  u8 unk00[0x14];
  u8 width;
  u8 unk15[4];
  u8 dirty;
};

struct StatusIconEntry {
  u8 unk00[6];
  u8 priority;
};

struct TlaSpriteRecord {
  struct AnimationSpritePart image;
  s32 scale;
  u8 unk10[4];
  u8 width;
  u8 unk15[4];
  u8 dirty;
  u8 flags;
  u8 unk1b[13];
  struct SpriteEntry *entries[1];
};

struct AnimationEntryGroup {
  u8 unk00[0x28];
  struct AnimationEntry *entries[4];
};

struct MotionRecordScale {
  u8 unk00[0xc];
  s32 scale;
};

struct BattleFxRequest {
  s32 field00;
  s32 field04;
  s32 field08;
  s32 field0c;
  s32 field10;
  s32 field14;
  s32 field18;
  u8 unk1c[8];
  s16 field24;
  u8 unk26[0x32];
};

struct Layout {
  u8 pad[4];
  s16 field[6];
};

struct BattleSpriteSlots {
  s32 enemyGroup;
  s16 spriteSlots[6];
};

struct ActionTargets {
  u8 side;
  u8 count;
  u8 unk02;
  u8 ids[14];
  u8 stateA[14];
  u8 stateB[14];
  u8 stateC[14];
  u8 stateD[14];
  u16 value4a;
  s32 message;
  s32 value50;
  s32 value54;
  s32 cue;
  s32 unk5c;
  s32 extra;
  s32 unk64;
};

struct DispatchBattleWork {
  u8 unk000[0x66c];
  s32 brightness;
  u16 background;
  u8 unk672[0x67c - 0x672];
  struct BattlePlan plan;
  s32 skipPlayback;
  u8 unk6e4[0x84e - 0x6e4];
  u16 unk84e;
  u8 unk850[0x869 - 0x850];
  u8 actorMode;
};

struct EscapeTransition {
  s32 targetYaw;
  s32 unk04[3];
  s32 active;
};

struct BattleEscapeState {
  u8 reserved00[0x45];
  u8 guaranteed;
  u8 failedAttempts;
};

struct BattleLinkedActionState {
  s32 count;
  s32 check;
  s32 seed;
  u8 unk0c[28];
};

extern u8 gData_03001ae8[];

extern struct Snapshot Data_0200ff58;

extern struct SnapshotCopy Data_0200ff6c[];

extern struct SnapshotVitals Data_0200ffec[];

extern u8 gScriptedBattle_Units[];

extern struct DebugPartyEntry gDebug_ParameterSets[];

extern u16 Data_02000436;

// The Broken Seal's gBattleMotion_Variant* tables, indexed by the variant.
extern const s32 Data_0812cbb0[]; ///< acceleration

extern const s32 Data_0812cbd8[]; ///< speed limit

extern const s32 gBattleKnockback_DistancePercent[]; ///< upward velocity

extern const s32 Data_0812cc28[]; ///< distance, in percent of the anchor's x

extern const s32 gData_08128868[];

extern const s32 gBattleObject_EmptyScript[];

extern const s8 gBattleParty_CenterOrderOffsets[];

extern const u16 gData_081287d4[];

extern const u16 gData_081287e2[];

extern const u16 gData_081287f0[];

extern const u16 gBattleUnit_WeaponAnimationsIsaac[];

extern const u16 gBattleUnit_WeaponAnimationsGaret[];

extern const u16 gBattleUnit_WeaponAnimationsIvan[];

extern const u16 gBattleUnit_WeaponAnimationsMia[];

extern const u16 gBattleUnit_WeaponAnimationsJenna[];

extern struct BattlePresentationTransition *gTransitionWork;

#endif
