#ifndef ALCHEMY_MODULE_BATTLE_PRESENTATION_H
#define ALCHEMY_MODULE_BATTLE_PRESENTATION_H

/// @file
/// What the battle object's later parts (games/tla/src/battle/battle/, from
/// motion_graphics.c to spoils.c) declare for the files that use them,
/// written once.

#include "types.h"
#include "battle/battle_action.h"
#include "battle/battle_side.h"

struct BattleActionRecord;
struct BattlePlan;

u32 BattleUnit_IsListed(u32 id);

s32 BattleFormation_BuildEnemyList(s32 recordId);

void BattleBackground_Init(void);
void BattleBackground_Load(s32 mode, s32 resource, s32 level);

void BattleTransition_Run(s32 arg, s32 kind);

void Battle_TurnEndNoOp(void);

void BattleBackground_Free(void);

s32 BattleSprite_GetSprite(s32 index);
s32 BattleSprite_GetPalette(s32 index);

s32 BattleActionRun_Build(struct BattleActionRecord *request,
                          struct BattlePlan *plan);

s32 BattleStaging_RunAttack(struct BattlePlan *plan, s32 mode);

s32 DjinnRecovery_CountDownSide(s32 side);

void BattleBackground_SetMode(s32 mode, s32 background, s32 value);

s32 BattleAI_ChooseAction(struct BattleActionRecord *request, s32 retry);

u32 BattleSprite_GetAttackEffect(u32 index);
u32 BattleSprite_GetDefeatSound(s32 index);

s32 BattleAI_ChooseRandomTarget(s32 party);

s32 BattleDjinn_CountStandby(u32 arg0, u8 *counts);

struct BattlePresentationWork;

typedef void (*ModelDecoder)(const void *source, void *destination);

struct PlacementList {
  struct DjinnRecoveryEntry entries[72];
  s32 count;
};

struct PlacementTable {
  u8 padding[8];
  struct PlacementList list;
};

struct ActionRunBattleWork {
  u8 unk000[0x44];
  u8 twoSided;
  u8 unk045[0x58 - 0x45];
  s16 partyUnits[7];
  s16 enemyIds[7];
  u8 unk074[0x862 - 0x74];
  u16 summonedMask;
  u8 unk864;
  u8 summonCount;
};

struct DjinnRecoveryEntryDraft {
  u8 element;
  u8 index;
  u8 unitId;
  s8 turns;
};

struct DjinnRecoveryListDraft {
  struct DjinnRecoveryEntryDraft entries[72];
  s32 count;
};

// The parts of the battle work the backdrop uses.
struct BattleBackdropWork {
  u8 unk000[0x56c];
  u16 palette[128];
  s32 brightness;
  u8 unk670[0x1f7];
  u8 keepBackdropPalette;
};

struct SavedBattleObj {
  struct BattleObj object;
  u32 *buffer;
  u8 unitId;
  u8 single;
  u8 reactivate;
  u16 classId;
};

struct Work0811e7dc {
  u8 unk00[0x2c];
  void *buffer;
  u8 objectId;
};

struct Record0811e7dc {
  u8 unk00[26];
  u8 flags;
};

struct AttackAnimationEntry {
  s16 animationId;
};

struct AttackAnimation {
  u8 unk00[40];
  struct AttackAnimationEntry *entries[1];
};

struct ItemAnimationEntry {
  u8 unk00[5];
  u8 param;
};

struct ItemAnimation {
  u8 unk00[27];
  u8 count;
  u8 unk1c[12];
  struct ItemAnimationEntry *entries[1];
};

struct RecordChild {
  u8 unk00[5];
  u8 param;
  u8 unk06[0x10];
  u8 frame;
};

struct ChildRecord {
  u8 unk00[0x1b];
  u8 count;
  u8 unk1c[0xc];
  struct RecordChild *entries[4];
};

/// One kept formation: its record and its average level, -1 while empty.
struct FormationCandidate {
  s16 recordId;
  s32 score : 16;
};

struct OwnerElementStats {
  s16 power;
  s16 res;
};

struct SummonChargeState {
  u8 unk00[0x10];
  u16 classIds[6];
  u32 usedMasks[6];
  s8 channels[6];
  u8 unk3a[6];
  u8 count;
};

struct BattleActorDefinition {
  u8 name[14];
  u8 unk0e[0x11b];
  u8 unavailable;
  u8 unk12a[0x20];
  u16 classId;
};

struct FormationBattleWork {
  u8 unk00[0x3c];
  u16 firstDefeated;
  u16 defeatState;
  u8 unk40;
  u8 unk41;
  u8 battleType;
  u8 unk43[0x825];
  u8 unk868;
};

struct AnimationRecord {
  u8 unk00[0x10];
  u8 slot;
  u8 unk11[3];
  u8 width;
  u8 height;
};

struct MessageWindow {
  u8 unk00[0x0c];
  s16 x;
  s16 y;
};

struct MessageOffset {
  u8 unk00[4];
  u16 x;
  u16 y;
};

struct BattleMessageDisplay {
  struct MessageWindow *window;
  struct MessageOffset *offset;
  s32 marked;
};

struct ChildEntry {
  u8 unk00[5];
  u8 colorMode;
  u8 unk06[16];
  u8 frame;
};

struct MotionRecord {
  u8 unk00[40];
  struct ChildEntry *entries[1];
};

struct DefeatSpoils {
  s32 coins;
  s32 experience;
  s32 defeated;
  u16 items[4];
  u16 lastEnemyIds[2];
  u8 lastDigits[2];
  s8 lastSlot;
  s8 lastCount;
};

struct DefeatBattleWork {
  u8 unk00[0x10];
  u16 enemyClasses[6];
  u8 unk1c[0x20];
  u16 firstDefeated;
  u16 defeatState;
  u8 unk40[0x510];
  struct DefeatSpoils spoils;
};

struct AdvanceSprite {
  struct AdvanceSprite *next;
  u16 y : 8;
  u16 flags : 8;
  u16 x : 9;
  u16 size : 7;
  u16 tile : 10;
  u16 palette : 6;
  u16 affineParam;
};

struct AdvanceWindow {
  u8 unk00[0xc];
  s16 x; ///< tiles
  s16 y; ///< tiles
};

struct AdvanceOffset {
  u8 unk00[4];
  u16 x; ///< 1/256 pixels
  u16 y; ///< 1/256 pixels
};

/// The battle message window and its scroll offset, in heap slot 0x94.
struct AdvanceDisplay {
  struct AdvanceWindow *window;
  struct AdvanceOffset *offset;
};

struct BattleEventFlagWork {
  u8 unk00[0x16c];
  u32 flags;
};

struct BattleDisplayFlags {
  u8 unk00[8];
  s32 marked;
};

struct AIState {
  unsigned initialized : 1;
  unsigned selection : 3;
  unsigned form : 2;
  unsigned remaining : 26;
};

struct AIEnemyRest {
  u8 unk00[0x2c];
  u16 abilities[8];
};

struct AIBattleWork {
  u8 unk000[0x564];
  u16 queuedSummons[2];
  u8 queuedCharges[2];
  s8 queueHead;
  s8 queueCount;
  u8 unk56c[0x869 - 0x56c];
  u8 unk869;
  u8 unk86a[2];
  u32 aiStates[4];
};

struct TurnEndList {
  struct DjinnRecoveryEntry entries[72];
  s32 count;
};

struct TurnEndTable {
  u8 padding[8];
  struct TurnEndList list;
};

struct BattleGroupOrder {
  s32 group[2];
};

extern const u32 Data_08128c50[];
extern u8 Data_08128972[];

extern u8 Data_0812896c[];

extern const u8 Model_DecodeCode[];

extern u8 Model_DecodeCodeSize[];

void Animation_SkipToStopFar(void *record);

void Sprite_RemoveAnimationAtFar(void *record, s32 slot);

s32 SpriteAnimation_SumCommandLengthsFar(s32 animation, s32 a, s32 b);

void ColorBuffer_Scale(u16 *source, u32 destination, s32 value, s32 size);

/// The enemy flags (0x600 + id) cleared before every search.
extern const u16 Data_08130c5c[];

void BattleMessage_OpenFar(s32 message);

void SpriteDissolve_StepFar(void *record, s32 step);

extern u16 Data_0200049c;

s32 Unit_TryLevelUpFar(s32 unitId, s16 *gains);

extern u8 gBattleMessage_AdvanceArrowTiles[];

extern u8 gBattleAI_ActionWeights[3][8];

extern u16 Data_08128960[];

extern const u8 gBattleTransition_CurtainVariants[];

extern u8 gBattleTransition_CurtainTiles[];

void BattleEffect_RunShatteringOpeningFar(struct BattlePresentationWork *work);

extern char ResourceId_SerpentStarsBackdrop;

extern const struct BattleGroupOrder gBattleParty_TurnEndSideOrder;

void Battle_ApplyActionExtras(s32 unitId, s32 all, s32 announce);

#endif
