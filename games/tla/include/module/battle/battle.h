#ifndef ALCHEMY_MODULE_BATTLE_BATTLE_H
#define ALCHEMY_MODULE_BATTLE_BATTLE_H

/// @file
/// What games/tla/src/battle/ declares for the files that use it, written once.

#include "types.h"
#include "battle_type.h"
#include "field/sprite.h"

struct AnimationEntry;
struct BattleActionRecord;
struct BattlePlan;
struct BattlePresentationTransition;
struct BattlePresentationWork;
struct BattleUnit;
struct SpriteEntry;

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

struct CastAnimationEntry {
  u8 unk00[5];
  u8 param;
};

struct CastAnimation {
  u8 unk00[27];
  u8 count;
  u8 unk1c[12];
  struct CastAnimationEntry *entries[1];
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

s32 AutoBattle_ShowDefend(s16 *);
s32 BattleStaging_RunUnitAction(s16 *);
extern u8 gData_03001ae8[];
s32 AutoBattle_RunAttackOrItem(struct BattleActionRecord *action);
s32 AutoBattle_RunPsynergy(struct BattleActionRecord *);

void Func_08138008(struct BattlePresentationWork *work);
void Func_08138018(struct BattlePresentationWork *work);
void Func_08138020(struct BattlePresentationWork *work);

s32 Func_081197f0(s32 enemyGroup);

extern struct Snapshot Data_0200ff58;
extern struct SnapshotCopy Data_0200ff6c[];
extern struct SnapshotVitals Data_0200ffec[];

extern u8 Data_0812a16c[];

void Unit_SetPpFar(struct BattleUnit *unit, s32 pp);

extern struct DebugPartyEntry gData_0812a174[];

extern u16 Data_02000436;

// The Broken Seal's gBattleMotion_Variant* tables, indexed by the variant.
extern const s32 Data_0812cbb0[]; ///< acceleration
extern const s32 Data_0812cbd8[]; ///< speed limit
extern const s32 Data_0812cc00[]; ///< upward velocity
extern const s32 Data_0812cc28[]; ///< distance, in percent of the anchor's x

void BattleMotion_ReservedNoOp83B0();
s32 BattleUnit_GetEffectHeight(s32 owner);

extern const s32 gData_08128868[];

extern const s32 Data_0812cacc[];

s32 Func_0811a24c(s32 sideMask, u16 *unitIds);

void Func_08138050(struct BattleFxRequest *request, s32 value);

s32 Event_GetSpecialValueFar(void);

void BattleUnit_RefreshPlacement(void);

void BattleSpoils_Clear(void);

s32 BattleUnit_ListIds(s32 groups, u16 *dst);

void Func_0811b9fc(void);

void Func_0811d7e4(void);

s32 Battle_GatherActions(struct BattleActionRecord *actions);

s32 AutoBattle_BuildActions(struct BattleSortedUnitEntry *entries);

s32 Func_0811d9cc(struct BattleActionRecord *action, s32 delay);

s32 AutoBattle_RunAction(s16 *action);

s32 Battle_FindTaggedSlotByValue(u32 id);

void Battle_ProcessTurnEnd(void);

void Battle_AwardSpoils(void);

extern const s8 gData_081287c8[];

s32 ResourceSlot_LoadFar(u32 slot, u32 *buffer, s32 number, u32 variant);

extern const u16 gData_081287d4[];
extern const u16 gData_081287e2[];
extern const u16 gData_081287f0[];
extern const u16 gData_081287fe[];
extern const u16 gData_0812880c[];
extern const u16 gData_0812881a[];
extern const u16 gData_08128828[];
extern const u16 gData_08128836[];

s32 Func_0811df70(struct BattlePlan *plan, s32 mode);

s32 BattleStaging_RunPsynergyCast(struct BattlePlan *plan, s32 mode);
s32 BattleStaging_RunItem(struct BattlePlan *plan, s32 unused,
                          struct BattleActionRecord *selection);

s32 BattleStaging_RunFlee(struct BattlePlan *plan);

void BattleUnit_ProcessActionEnd(struct BattlePlan *plan);

void Func_08127308(s32 unitId, s32 element, s32 mode, s32 value);
void BattleBackground_RestorePalette(void);

s32 Func_080381c0(struct BattleActionRecord *entries, void *buffer, s32 mode);

extern struct BattlePresentationTransition *gTransitionWork;

void Animation_SetWorkEntryFar(void *work, s32 no);

s32 Func_0811d79c(s32 kind);

s32 SummonSlot_RegisterActorSprites(s32 unit);

void BattleCamera_SetViewAtPairMidpoint(s32 first, s32 second, s32 scale);

void BattleParty_ListAllUnitsAndSubmit(void);

s32 BattlePlayback_WaitForReady(void);

void Func_0811ddd8(struct BattlePlan *plan,
                   struct BattlePresentationWork *effect);

s32 BattleEnemy_RecordDefeat(s32 unitId, s32 earned);

void BattleObject_UpdatePose(s32 id);

u32 BattlePlayback_Reset(void);

void BattleIntro_AnnounceEncounter(s32 enemyCount);

#endif
