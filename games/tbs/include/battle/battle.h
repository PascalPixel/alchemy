#ifndef ALCHEMY_BATTLE_H
#define ALCHEMY_BATTLE_H

/// @file
/// The battle bank's routines and globals, as the other banks see them.

#include "types.h"
#include "edition.h"
#include "system/vector3.h"
#include "field/speaker.h"
#include "gamepak/far_call.h"

struct BattleAction;
struct BattleEffectParams;
struct Actor;

/// Battlefield (x, y) pixel pairs by place: 0 to 3 the party's column at x 80,
/// 32 pixels apart, and 5 to 10 a column at x -80, 20 apart (battle_tables.c).
extern const s8 gBattlePlacement_StepPairs[];
/// Read as a halfword by Actor_SetOthersPaused: the actor id of the
/// player's actor, which it leaves alone.
extern u8 gPlayerActorId[];
/// Each weapon's kind: bits 15..9 the kind, which indexes the
/// gBattleUnit_WeaponAnimations tables, and 8..0 the item id; ended by 0xffff
/// (BattleUnit_GetWeaponKind).
extern u16 gBattleUnit_WeaponKinds[];

struct SpriteAnimation;
struct Sprite;
struct BattleSideOrder;
struct BattleBackgroundWork;
struct SpriteScale;
struct MindReadWork;

/// One of the star field's three rings, moving outward on the effect canvas.
struct Ring {
  s32 x;         ///< 1/1024 canvas pixels
  s32 y;         ///< 1/1024 canvas pixels
  s32 velocityX; ///< 1/1024 canvas pixels per frame
  s32 velocityY; ///< 1/1024 canvas pixels per frame
  s32 age;       ///< frames
};
extern const u8 gMindRead_CasterPaletteCycle[];
/// The weights an enemy picks among its eight actions by, one table for each
/// weighted action strategy (enum EnemyActionStrategy, battle_internal.h):
/// even, falling by 6, or falling steeply from 113 to 5.
extern const u8 gBattleAI_ActionWeights[3][8];
/// The knockback's speed gained each frame by variant (enum BattleKnockback),
/// 16.16 pixels a frame: 1, but 8 for KNOCKBACK_HURL.
extern s32 gBattleKnockback_Acceleration[];
/// The knockback's reach by variant, in percent of the unit's distance from
/// the centre: 140 to 200, 100 for KNOCKBACK_BOUNCE and KNOCKBACK_JOLT.
extern s32 gBattleKnockback_DistancePercent[];
/// The knockback's top speed by variant, 16.16 pixels a frame: 4, but 20 for
/// KNOCKBACK_HURL.
extern s32 gBattleKnockback_SpeedLimit[];
/// The knockback's hop by variant, 16.16 pixels a frame upward: 2 to 10.
extern s32 gBattleKnockback_VelocityY[];
/// A fixed formation's places by order, as offsets from the middle place:
/// 0, -1, 1, -2, 2 and so on out to 4.
extern const s8 gBattleParty_CenterOrderOffsets[];
/// Turn-end effects go to the party (BATTLE_SIDE_MASK_PARTY), then the enemies
/// (BATTLE_SIDE_MASK_ENEMIES).
extern const struct BattleSideOrder gBattleParty_TurnEndSideOrder;
/// A lone SC_END: a battle unit's actor script, which BattleObject_Make
/// starts.
extern const s32 gBattleObject_EmptyScript[];
/// The advance arrow beside a battle message: eight 128-byte pictures of a
/// 256-colour sprite, one every four frames.
extern u8 gBattleMessage_AdvanceArrowTiles[];
/// The battle transition's curtain tiles, 32 bytes (one 4-bit tile) each.
extern const u8 gBattleTransition_CurtainTiles[];
/// Eight curtain tiles, 32 bytes each, of which the transition copies one at a
/// time into the curtain's first tile.
extern const u8 gBattleTransition_CurtainVariants[];
/// Garet's weapon sprite by weapon kind (BattleUnit_GetWeaponSprite).
extern const u16 gBattleUnit_WeaponAnimationsGaret[];
/// Ivan's weapon sprite by weapon kind.
extern const u16 gBattleUnit_WeaponAnimationsIvan[];
/// Mia's weapon sprite by weapon kind.
extern const u16 gBattleUnit_WeaponAnimationsMia[];
/// Jenna's weapon sprite by weapon kind; Felix draws none.
extern const u16 gBattleUnit_WeaponAnimationsJenna[];
/// The size of BitDecoder_DecodeImage, which BattleBackground_Load copies to
/// HEAP_SLOT_RAM_CODE_TEMP; ld_script.ld defines it as an address equal to
/// the size.
extern u8 BitDecoder_Size[];
/// The extra matrix the battle camera's projection applies first while
/// commands are chosen (FLAG_CHOOSING_COMMANDS), in 16.16 words.
extern const s32 gBattleCamera_CommandMatrix[];
/// Where SparkWork.sheet holds the spark's square picture for each frame of
/// life left, 0 to 6: byte offsets, a byte a pixel.
extern s32 gElementGlow_StarPictures[];
/// The spark's width and height in pixels by life left: 1 to 4, 4 from three.
extern u8 gElementGlow_StarSizes[];
/// Where SparkWork.sheet holds the ring's four 32 by 32 frames: byte offsets,
/// 1,024 apart.
extern s32 gElementGlow_RingPictures[];
/// The highlighted units' BLDY lightening: 15 down to 0 once, then the second
/// sixteen (a pulse up to 8 and back) over and over.
extern s8 gBattleHighlight_BlendRamp[];
/// The 104 encounter zones (encounter_zones.c), read as struct EncounterZone
/// rows.
extern u16 gEncounterZones[];
/// The HEAP_SLOT_MIND_READ heap slot: Mind Read's wave work while its effect
/// runs.
#define gMindReadWork                                                          \
  (*(struct MindReadWork **)&gHeapSlots[HEAP_SLOT_MIND_READ])
/// The heap slots from HEAP_SLOT_BATTLE_EFFECT, then the canvas and sheet:
/// the running battle effect's BattleEffectWork first.
#define gBattleEffectWork (*(void *(*)[]) & gHeapSlots[HEAP_SLOT_BATTLE_EFFECT])
/// The HEAP_SLOT_ENEMY_UNITS heap slot: the six enemy battle units,
/// BATTLE_UNIT_SIZE bytes each, that Unit_Get returns for unit ids 128 to 133
/// during a battle.
#define gEnemyUnits (*(u8 **)&gHeapSlots[HEAP_SLOT_ENEMY_UNITS])
/// Random16's seed; Lucky Wheels seeds it with the play time, and a link
/// battle's parent sends it as the turn's gBattleRandomSeed.
extern u32 gRandomSeed;
/// The HEAP_SLOT_BATTLE_BACKGROUND heap slot: the camera's turn, which
/// battle_staging.c, battle_auto_battle.c and battle_turn.c set for each
/// action, and the background's display mode.
#define gBattleBackgroundWork                                                  \
  (*(struct BattleBackgroundWork **)&gHeapSlots[HEAP_SLOT_BATTLE_BACKGROUND])
/// SC_FACE_WATCHED every frame, forever: the script of an actor told to
/// keep facing its watched actor.
extern const u8 gActor_FaceWatchedScript[];
/// Where the party returns after a defeat: {scene, entrance (-1 any),
/// return scene, return entrance} rows, ended by scene -1
/// (Scene_SetRestartPoint).
extern const s16 gScene_RestartRules[];
/// Isaac's weapon sprite by weapon kind (BattleUnit_GetWeaponSprite).
extern const u16 gBattleUnit_WeaponAnimationsIsaac[];
/// Battle backgrounds by scene: a background word (bit 31 set, resource id
/// below) and then its scenes, ended by 0; a scene not listed takes the last
/// background (BattleBackground_SelectForMap).
extern s32 gScene_BattleBackgroundRules[];
/// The template BattleEffect_BuildBlitter walks word by word to write a blitter
/// (blitter_fragments.s); none of it runs where it is.
extern const u32 gBlitterTemplate[];
/// The message voice's rate by message speed option: 1, 3, then 10
/// (Message_SetVoiceFromActor). The same values as
/// gMessageSpeed_VoiceGaps, which the message reads, in a copy of its own.
extern u8 gMessageSpeed_VoiceRates[];
extern const u16 gWorldMap_CursorDirectionAngles[];
extern const u8 gWorldMap_MarkerBlendCycle[];
/// The World Map's area markers, {tile << 16 | flag, area} for
/// each area's actor id from 11, ended by -1; a zero row reserves an
/// actor id with no marker.
extern const s32 gWorldPicture_AreaMarkers[];
u32 __udivsi3(u32 numerator, u32 denominator);
void _call_via_r0(u32 routine);
void BitDecoder_DecodeImage(void);
void FarCall_WindowTable(void);

/// BattleMotion_ApplyKnockback's knockbacks, the columns of the
/// gBattleKnockback_* tables in knockback_tables.s. Each is named by how far
/// and how high it sends the unit, as percent of its spot's distance from the
/// centre and its hop in pixels a frame; all but KNOCKBACK_HURL accelerate by
/// 1 pixel a frame to at most 4.
enum BattleKnockback {
  KNOCKBACK_NUDGE,  ///< 140 percent, a hop of 3
  KNOCKBACK_HOP,    ///< 140 percent, a hop of 6
  KNOCKBACK_LEAP,   ///< 150 percent, a hop of 7
  KNOCKBACK_LAUNCH, ///< 200 percent, a hop of 10
  /// The hardest: 200 percent at 8 pixels a frame more each frame, to at
  /// most 20, with a hop of 5.
  KNOCKBACK_HURL,
  /// 150 percent, a hop of 2. From here on a unit already in the air takes
  /// the new hop too.
  KNOCKBACK_SKID,
  KNOCKBACK_BOUNCE, ///< the spot itself, a hop of 3
  KNOCKBACK_JOLT    ///< the spot itself, a hop of 5.5
};

/// The battle sprites' animations (Sprite_StartAnimation numbers)
/// that the battle code picks by name.
enum BattlePose {
  POSE_ENEMY_HELD = 0, ///< an enemy asleep, stunned or held
  POSE_STANDING = 1,
  POSE_LEAP = 2,       ///< running up to a target and leaping
  POSE_CAST = 3,       ///< a hand raised to cast or use an item
  POSE_PARTY_HELD = 4, ///< asleep, stunned or held; also a downed party member
  POSE_DAMAGE = 5,     ///< taking damage
  POSE_ENEMY_DOWN = 5  ///< a downed enemy stays in its damage pose
};

/// A battle unit's sprites' OBJ mode, as BattleHighlight_SetSpriteMode
/// sets it: faded units are semi-transparent, and the battle's blends act on
/// them.
enum BattleSpriteMode { SPRITE_OPAQUE, SPRITE_SEMI_TRANSPARENT };

/// BattleHighlight_SetModes' mode, kept in BattleWork.highlightMode:
/// whether the selected units are highlighted, every other unit faded.
enum BattleHighlight { HIGHLIGHT_OFF, HIGHLIGHT_SELECTED };

/// The glows ElementGlow_Play plays about a unit in its element.
enum BattleElementGlow {
  /// The star field gathering on the unit while the background darkens: a
  /// cast.
  GLOW_STAR_FIELD,
  GLOW_STREAKS, ///< the element's streaks: an enemy's cast
  GLOW_PILLAR,  ///< the element's pillar, at a strength: a summon's boost
  GLOW_BURST    ///< the element's burst: a Djinni set
};

struct BattleAction;
struct BattlePlaybackWork;
struct BattleEffectParams;
struct BattleSummonState;
struct BattleTransitionEntry;
struct AbilityDefinition;
struct BattleObject;
struct BattleActionRun;
struct FixedPointPosition;
struct Actor;

s32 DebugParameterSet_Load(s32 set);
void DebugBattle_ViewMessages(void);
void DebugBattle_RunMenu(void);
void BattleCamera_Update(void);
void BattleIntro_AnnounceEncounter(s32 enemyCount);
void BattleUnit_Collect(void);
s32 Battle_ShiftVramTiles(void);
void Battle_ClearBg0Vofs(void);
void Party_BattleSetupNoOp(s32 unused);
UNREAD_RESULT BattleParty_ResetActiveRuntimeFields(void);
UNREAD_RESULT DjinnRecovery_Reconcile(void);
struct BattleSummonState *BattleSummon_UpdateAvailability(void);
void BattleSummon_ReservedNoOp(void);
s32 LinkBattle_ReceiveParty(void);
UNREAD_RESULT LinkBattle_SendParty(void);
u8 *Text_CopyFiveCharsPaddingUnderscore(u32 unused0, u32 unused1, u16 *source);
s32 LinkBattle_WaitSync(void);
s32 LinkBattle_SyncTurn(void);
UNREAD_RESULT BattleParty_AssignOpponentSlots(void);
s32 Battle_ClearSerialTransferPayload(void);
s32 Battle_Run(s32 enemyGroup);
void BattleCamera_AdjustByShoulderKeys(void);
void BattleCamera_AdjustByShoulderKeysAlt(void);
s32 BattleUnit_CountDownDefenseStage(s32 unitId);
s32 BattleUnit_CountDownResistanceStage(s32 unitId);
s32 BattleUnit_CountDownDelusion(s32 unitId);
s32 BattleUnit_CountDownCharm(s32 unitId);
s32 BattleUnit_CountDownStun(s32 unitId);
s32 BattleUnit_CountDownSleep(s32 unitId);

/// Pushes a message to show, waiting for the player to read it.
#define PUSH_MESSAGE(message) BattlePlayback_Push(PLAYBACK_MESSAGE, (message))

/// Moves camera yaw by 512 while L/R is held; two identical functions exist.
#define DEFINE_ADJUST_CAMERA(name)                                             \
  void name(void) {                                                            \
    struct BattleCamera *cam = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);              \
    struct BattleBackgroundWork *view =                                        \
        HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);                          \
                                                                               \
    if ((gKeysHeld & KEY_L) != 0) {                                            \
      cam->yaw += 512;                                                         \
    }                                                                          \
    if ((gKeysHeld & KEY_R) != 0) {                                            \
      cam->yaw -= 512;                                                         \
    }                                                                          \
    if (view->flag == 0) {                                                     \
      BattleCamera_SetRange(px(120), px(120), 0, 0, Q16_ONE);                  \
    }                                                                          \
  }

s32 BattleParty_PrepareMembers(u16 *unitIds);
s32 BattleEnemy_List(s16 *unitIds);
s32 BattleUnit_ListLiving(s32 sideMask, u16 *unitIds);
s32 BattleUnit_ListIds(s32 sideMask, u16 *unitIds);
void BattleObject_Put(void);
void BattleUnit_ListAllAndSubmit(void);
s32 BattleObject_GetPalette(s32 unitId);
s32 Enemy_HasSpriteRoom(s32 classId);
s32 EnemySprite_LoadIntoSlots(s32 unitId);
UNREAD_RESULT BattleObject_ReleaseSpriteSlots(s32 unitId);
s32 BattleUnit_GetWeaponKind(s32 itemId);
s32 BattleUnit_GetWeaponSprite(s32 unitId);
void BattleObject_Make(struct BattleObject *actor, s32 unitId, s32 x, s32 y);
void BattlePlacement_GetStepPair(s32 partyPlace, s32 *x, s32 *y);
void BattlePlacement_LayoutEnemies(u16 *actorIds, s32 count, s32 *xPositions,
                                   s32 *zPositions);
s32 Enemy_GetFreeUnitId(void);
UNREAD_RESULT BattlePlacement_RelayoutEnemies(void);
void BattleUnit_RefreshPlacement(void);
s32 BattlePlacement_ContainsId(s16 *list, s32 place);
void BattleObject_UpdateSpritePriorities(void);
UNREAD_RESULT BattleUnit_BuildStatusFlags(s32 unitId,
                                          struct BattleObject *battleObject);
UNREAD_RESULT BattleStatusIcon_Cycle(struct BattleObject *battleObject);
UNREAD_RESULT BattleObject_UpdatePose(s32 unitId);
void ReleaseBattleObjectRecords(s32 unitId);
void BattleObject_SpawnObjectsForList(s16 *unitIds, s32 refreshPoses);
struct BattleObject *BattleObject_Get(s32 unitId);
void ResetMotionRecordGroup(void *recordGroup);
void ResetBattleObjectRecordGroups(struct Actor *actor);
s32 BattleObject_MarkForRelease(s32 unitId);
#ifdef UBFIX
void BattleObject_DestroyAll(void);
#else
void *BattleObject_DestroyAll(void);
#endif
s32 Battle_GetWorkEntryPair(s32 place, struct FixedPointPosition *position);
UNREAD_RESULT BattleCamera_ApplyTransform(void);
s32 BattleMotion_ProjectPosition(s32 unitId, s32 *projected);
void *GetMotionRecord(struct Actor *actor, s32 recordIndex);
void BattleCamera_SetDefaultView(void);
void BattleMotion_ReturnToSpot(s32 unitId);
void BattleMotion_SetupEscapeObject(s32 unitId);
void BattleMotion_LeapToward(struct BattleObject *startSlot,
                             struct BattleObject *endSlot, s32 progress);
void BattleMotion_ReturnObjectToSpot(struct BattleObject *battleObject);
void BattleMotion_HopBack(s32 unitId);
void BattleMotion_LeapBack(s32 unitId);
void BattleMotion_ApplyKnockback(s32 unitId, s32 variant);
void BattleMotion_ApproachTarget(s32 unitId, s32 targetId, s32 travelDivisor,
                                 s32 initialVelocityY);
void BattleMotion_RestartLeap(s32 unitId);
void BattleMotion_MidpointNoOp();
void BattleMotion_ApplyPairMidpoint(s32 firstId, s32 secondId);
void BattleMotion_PlayHitFlash(s32 unitId);
s32 BattleMotion_ProjectScaledPosition(s32 unitId, s32 *projected);
s32 BattleMotion_ProjectConditionalPosition(s32 unitId, s32 *projected);
s32 BattleUnit_GetEffectHeight(s32 unitId);

s32 AutoBattle_BuildActions(struct BattleAction *entries);
s32 AutoBattle_RunAction(struct BattleAction *action);
s32 BattleObject_IsValidId(u32 unitId);
s32 BattleFlee_PlayRun(struct BattleAction *action);
s32 AutoBattle_ShowDefend(const struct BattleAction *action);
s32 AutoBattle_RunPsynergy(struct BattleAction *action);
s32 AutoBattle_RunApproachAttack(struct BattleAction *input);
s32 AutoBattle_RunAttackOrItem(struct BattleAction *action);
void AutoBattle_ApplyDamage(u32 unitId, s32 damage, s32 showMessage,
                            u8 *context);
void BattleMotion_PlayFall(s32 unitId);
s32 BattleAbility_ReplaceDefeated(const struct BattleAction *action);

void Palette_UpdatePulseBrightness(void);
void BattleCamera_SetCommandView(s32 verticalOffset);
void BattleUnit_ResetGuardLevels(void);
s32 BattleFlee_CheckSuccess(void);
s32 Battle_BuildPartyActions(struct BattleAction *entries);
s32 Battle_BuildEnemyActions(struct BattleAction *entries);
void BattleActionOrder_Sort(struct BattleAction *entries, s32 count);
s32 Battle_AppendLinkedActions(struct BattleAction *actions, s32 count);
void Palette_CopyWithBrightnessOffset(s32 offset);
s32 Battle_GatherActions(struct BattleAction *actions);
s32 Battle_GetUnitAtPlace(s32 taggedPlace);
s32 Battle_FindUnitPlace(u32 unitId);
void Battle_TurnStartNoOp(void);

s32 BattleStaging_DispatchAction(struct BattleAction *request, s32 delay);
UNREAD_RESULT BattleStaging_BuildTargetList(struct BattleActionRun *plan,
                                            struct BattleEffectParams *args);
s32 BattleStaging_RunFlee(struct BattleActionRun *plan);
s32 BattleStaging_HideField(struct BattleActionRun *plan, s32 mode);
UNREAD_RESULT BattleStaging_RunActorEntries(struct BattleActionRun *plan,
                                            s32 unused);
s32 BattleStaging_RunAttack(struct BattleActionRun *plan, s32 mode);
s32 BattleStaging_RunPlainAttack(struct BattleActionRun *input, s32 flags);
s32 BattleStaging_RunItem(struct BattleActionRun *plan, s32 unused,
                          struct BattleAction *selection);
UNREAD_RESULT BattleObject_SetColorMode(struct Actor *actor, s32 palette);
s32 BattleStaging_RunPsynergyCast(struct BattleActionRun *plan, s32 mode);

void BattleMotion_FlashHit(s32 unitId, s32 unused0, s32 unused1);
void BattleObject_RemoveFromLists(s32 unitId);
void BattleMotion_ShowDowned(s32 unitId);
s32 BattleAI_ChooseRandomTarget(s32 requireLivingUnit);
s32 BattleAI_ChooseTarget(s32 userId, struct AbilityDefinition *action);
UNREAD_RESULT BattleUnit_ClearStatuses(s32 unitId);
UNREAD_RESULT BattleMessage_WaitForAdvance(void);
UNREAD_RESULT BattleMessage_WaitForAdvanceAt(s32 x, s32 y);
void BattlePlayback_MarkMessage(void);
UNREAD_RESULT BattleObject_DestroyTemporaryObject(s32 unitId);
void BattlePlayback_GrantDjinnBonus(struct BattlePlaybackWork *runtime,
                                    u32 operand);
#ifdef UBFIX
void BattlePlayback_RunAll(void);
#else
u32 BattlePlayback_RunAll(void);
#endif
u32 BattlePlayback_Push(u32 opcode, u32 operand);
s32 Ability_EffectAppliesToDowned(s32 effect);
UNREAD_RESULT BattleAbility_ApplyToTarget(struct BattleActionRun *plan,
                                          s32 slot);
u32 Ability_IsPsynergy(s32 abilityId);
UNREAD_RESULT BattleAI_ChooseAction(struct BattleAction *request, s32 retry);
void Dma_StopAllThenRunHook(void);
void BattlePlayback_Start(s32 cue);
s32 BattlePlayback_SchedulePhase(s32 frames);
void BattlePlayback_Run(void);
void BattlePlayback_Reset(void);
UNREAD_RESULT BattlePlayback_WaitForReady(void);
u32 BattleUnit_IsListed(u32 unitId);
s32 BattleDjinn_CountStandby(u32 unitId, u8 *counts);
s32 BattleActionRun_Build(struct BattleAction *request,
                          struct BattleActionRun *plan);

s32 BattleUnit_RollCounterEarlyEnd(s32 unitId, s32 turns, s32 bias);
s32 BattleUnit_CountDownAttackStage(s32 unitId);
s32 BattleUnit_CountDownConfusion(s32 unitId);
s32 BattleUnit_CountDownPsynergySeal(s32 unitId);
s32 BattleUnit_CountDownRegenerate(s32 unitId);
s32 BattleUnit_CountDownReflect(s32 unitId);
s32 BattleUnit_CountDownAgilityStage(s32 unitId);
s32 DjinnRecovery_CountDown(void);
s32 DjinnRecovery_CountDownTwenty(void);
void Battle_ReservedNoOpF674(void);
UNREAD_RESULT Battle_ProcessTurnEnd(void);
UNREAD_RESULT BattleUnit_ProcessActionEnd(struct BattleActionRun *plan);

s32 BattleTransition_RunMosaicFadeOut(void);
void Graphics_BuildSequentialTileTable(void *tilemap);
void BattleTransition_BuildTilemap(void *tilemap);
void BattleBackground_ShowZoomedPage(void);
void BattleTransition_UploadTileVariant(void);
void BattleTransition_AdvanceTimer(void);
void BattleTransition_DrawRows(void);
void BattleTransition_ClearBg0Vofs(void);
void BattleTransition_Run(s32 enemyCount, s32 encounterId);
void BattleBackground_SetPaletteLevel(s32 unused, s32 level);
void BattleBackground_SetMode(s32 mode, s32 unusedResource, s32 fade);
void BattleBackground_Init(void);
void BattleBackground_Free(void);
void BattleBackground_Load(s32 mode, s32 resourceId, s32 level);
void BattleCamera_SetRange(s32 centerX, s32 centerY, s32 offsetX, s32 offsetY,
                           s32 scale);
#ifdef UBFIX
void BattleCamera_SetViewAtDepth(s32 x, s32 depth, s32 y, s32 zoom);
#else
void BattleCamera_SetViewAtDepth(s32 x, s32 depth, s32 y);
#endif
void BattleCamera_SetView(s32 x, s32 depth, s32 y, s32 zoom);
void BattleCamera_SetViewAtPairMidpoint(s32 firstUnitId, s32 secondUnitId,
                                        s32 zoom);
void BattleBackground_FadeAlphaOut(void);
void BattleBackground_FadeAlphaIn(void);
void BattleBackground_SetBlendControl(void);
void BattleTransition_InitializeEntries(struct BattleTransitionEntry *entries);
s32 BattleBackground_DivideBy16(s32 value);
s32 Battle_AdjustDebugValue(s32 selectedValue);
void BattleHighlight_SetSpriteMode(s32 unitId, s32 mode);
void BattleHighlight_SetOtherSpriteModes(s32 selectedUnitId);
void BattleHighlight_ClearAllSpriteModes(void);
void BattleBackground_StepHighlightBlend(void);
void BattleHighlight_SetModes(u16 *selectedUnits, s32 mode);

UNREAD_RESULT ElementGlow_UpdateStarField(void);
#ifdef UBFIX
void ElementGlow_UploadStarField(void);
#else
u32 ElementGlow_UploadStarField(void);
#endif
s32 ElementGlow_InitializeStarField(s32 mode);
s32 ElementGlow_End(s32 unused);
s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale,
                                s32 count);
// The native implicit-int declaration disagrees with the later void definition.
// A plain prototype keeps EN size but changes its first caller at .text+0x4c62.
#ifdef UBFIX
void ElementGlow_Play(s32 unitId, s32 element, s32 mode, s32 variant);
#endif
void BattleBackground_RestorePalette(void);
void Irq_RemoveVCountHandler(void);

s32 BattleFormation_AverageLevel(s32 recordId);
s32 BattleFormation_SelectLevelMatchedCandidate(s32 *outMargin);
#ifdef UBFIX
void BattleEnemy_AdjustLevel(s32 unitId, s32 levels);
#else
s32 BattleEnemy_AdjustLevel(s32 unitId, s32 levels);
#endif
s32 BattleEnemy_ClaimNameLetter(s32 classId, s32 unused);
UNREAD_RESULT BattleEnemy_ReleaseNameLetter(s32 unitId);
UNREAD_RESULT BattleEnemy_LetterFirstOfKind(s32 classId);
s32 BattleFormation_GetRandomMember(s32 recordId);
s32 BattleFormation_BuildEnemyList(s32 recordId);

u32 BattleSprite_GetDefeatSound(s32 index);
s32 BattleSprite_GetSprite(s32 index);
s32 BattleSprite_GetPalette(s32 index);
s32 BattleSprite_IsSmall(s32 index);
u32 BattleSprite_GetAttackEffect(u32 index);
u32 BattleSprite_GetAttackHop(u32 index);
s32 BattleSprite_HasHeavyImpact(s32 index);
s32 BattleSprite_GetEffectHeight(s32 index);

u32 BattleSpoils_RankItem(u32 inventoryEntry);
void BattleSpoils_Clear(void);
s32 BattleEnemy_RecordDefeat(s32 unitId, s32 djinnBonus);
void Battle_AwardSpoils(void);
void Battle_DebugSelectNoOp(void);

#endif
