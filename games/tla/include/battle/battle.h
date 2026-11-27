#ifndef ALCHEMY_BATTLE_H
#define ALCHEMY_BATTLE_H

/* @file */
/* The battle engine's functions as other files call them: the battle */
/* module's own, file by file, then the battle effects, the field engine's */
/* functions the battle and its effects borrow, and the stub calls into the */
/* main image. */

#include "fixed_math.h"
#include "types.h"
#include "edition.h"
#include "system.h"
#include "battle/battle_type.h"
#include "field/sprite.h"

struct BATTLE_OBJECT;
struct MOTION_OBJECT;

/* The battle module's functions in the order of its stub table */
/* (src/battle/stubs.s). */
/* Debug: the battle test menu, which never returns. It fights the first */
/* encounter again and again with auto battle on, unless Down is held at the */
/* start to open the chooser of encounter and parameter set. */
void btDebugBattleRunMenu(void);
/* Fades the screen to black over sixteen frames as every background's */
/* mosaic grows, then shows no layer. Sets REG_BLDCNT to darken the */
/* backgrounds, leaves REG_MOSAIC at its largest and REG_DISPCNT in mode 1 */
/* with every layer off; the four BGxCNT are put back as they were. It draws */
/* four random numbers a frame, which only advances the random number */
/* generator. Returns 0. */
s32 btTransitionRunMosaicFadeOut(void);
/* Readies the sprites of the units in list, ended by BATTLE_UNIT_LIST_END, and
 */
/* destroys every other unit's; with refresh, sets the listed units' poses */
/* again. */
void btActorSpawnObjectsForList(s16 *list, s32 refresh);
/* Fades BG2 out over the backdrop in eight frames, its blend weight falling */
/* by 2 a frame from 15, while the backdrop's stays 16. Leaves REG_BLDCNT */
/* blending BG2 over the backdrop. */
void btBackgroundFadeAlphaOut(void);
/* Fades BG2 in over the backdrop in eight frames, its blend weight rising by */
/* 2 a frame from 1, while the backdrop's stays 16. Leaves REG_BLDCNT */
/* blending BG2 over the backdrop. */
void btBackgroundFadeAlphaIn(void);
/* Points the battle camera at (x, depth, y) with a zoom in percent, and lines
 */
/* the zoomed background up with it: where the battlefield's origin falls on */
/* the screen moves the plane as much, and the zoom scales the focal length */
/* and the far depth. */
void btCameraSetView(s32 x, s32 depth, s32 y, s32 zoom);
/* Points the battle camera at the ground between two units, with a zoom in */
/* percent. */
void btCameraSetViewAtPairMidpoint(s32 firstUnitId, s32 secondUnitId, s32 zoom);
/* Switches the battle background's display mode and rebuilds both its maps. */
/* A nonzero fade darkens the saved palette by taking fade from each colour */
/* channel, and BATTLE_BACKGROUND_FADE_RESTORE puts it back undarkened; the */
/* background's id is not read. */
void btBackgroundSetMode(s32 mode, s32 backgroundId, s32 fade);
/* Loads a battle background: its picture decoded into BG VRAM, its palette */
/* darkened by level steps of 1/60 (none when level is negative), its tile */
/* table and tilemap, and the HBlank scroll the first time. */
void btBackgroundLoad(s32 mode, s32 resource, s32 level);
/* Darkens the battle background's palette by level steps of a sixtieth; */
/* level 0 restores its saved colours. */
void btBackgroundSetPaletteLevel(s32 unused, s32 level);
/* Returns the height an animation is drawn at over a class, in pixels, or 0 */
/* for its size's default; 0 out of range. */
u32 btSprGetEffectHeight(u32 index);
/* Returns 1 for a class whose blow lands with a heavier impact; 0 out of */
/* range. */
s32 btSprHasHeavyImpact(s32 index);
/* Returns how high a class hops as it runs up to attack, in pixels; 0 out of */
/* range. */
u32 btSprGetAttackHop(u32 index);
/* Returns 1 for a small class, which takes one enemy sprite slot instead of */
/* two and stands closer to its neighbours; 0 out of range. */
s32 btSprIsSmall(s32 index);
/* Returns the height above a unit where its battle effects play, in 16.16 */
/* pixels: its class's own, else EFFECT_HEIGHT_SMALL for a small enemy and */
/* EFFECT_HEIGHT_LARGE for others. */
FIXED btUnitGetEffectHeight(s32 unitId);
/* Sends one unit leaping three quarters of the way to another, arriving in */
/* travelFrames frames. */
void btMotionApproachTarget(s32 actorId, s32 targetId, s32 travelFrames,
							s32 initialVelocityY);
/* Unused: sends unitId leaping back to 1.3 times its spot's distance from the
 */
/* centre, in POSE_DAMAGE. */
void btMotionLeapBack(s32 unitId);
/* Knocks a unit back as the knockback variant (BATTLE_KNOCKBACK) says, in
 */
/* its damage pose; the Fusion Dragon and the Doom Dragon's three forms only */
/* take the pose. */
void btMotionApplyKnockback(s32 id, s32 variant);
/* Lists the units in the battle on the sides in sideMask, standing or not, and
 */
/* returns how many. */
s32 btUnitListIds(s32 sideMask, u16 *unitIds);
struct BATTLE_OBJECT *btObjectGet(s32 objectId);
/* Fights the battle against the enemy formation enemyGroup, from the opening */
/* to the end. Returns the number of enemies defeated when the party won or the
 */
/* link broke, BATTLE_OUTCOME_FLED or BATTLE_OUTCOME_LOST. */
s32 btRun(s32 enemyGroup);
/* Projects a battle object's position to the screen, in projected. */
s32 btMotionProjectPos(s32 id, s32 *projected);
/* Projects the point above a unit where its battle effects play */
/* (btUnitGetEffectHeight) onto the screen, into screen, scaled as its */
/* sprite is. Returns 0. */
s32 btMotionProjectScaledPos(s32 unitId, s32 *screen);
/* Projects the point above a unit onto the screen, into screen: the target */
/* cursor's place, EFFECT_HEIGHT_SMALL up for a small enemy and */
/* EFFECT_HEIGHT_LARGE for others, scaled as the sprite is. Returns 0. */
s32 btMotionProjectCursorPoint(s32 unitId, s32 *screen);
/* Lists the units still standing on the sides in sideMask, party first, into */
/* unitIds when it is not null, and returns how many. */
s32 btUnitListLiving(s32 sideMask, u16 *unitIds);
/* Lists the party members who fight, three in a link battle, into owners when
 */
/* it is not null, and returns how many. */
s32 btPartyPrepareMembers(u16 *owners);
/* Sets the camera up for the fixed view the battle shows while commands are */
/* chosen, raised by verticalOffset pixels. */
void btCameraSetCommandView(s32 verticalOffset);
/* Returns sprite recordIndex of an actor (0 only, for a single sprite), or */
/* null when it has no such sprite. */
void *btActorGetSpr(struct MOTION_OBJECT *object, s32 recordIndex);
/* Highlights the units in actors (up to UNIT_MAX, ended by */
/* BATTLE_UNIT_LIST_END), the others taking the other blend, or with */
/* HIGHLIGHT_OFF stops the highlight. */
void btHighlightSetModes(u16 *actors, s32 mode);
/* Starts playback of the action's targets when the effect's blow lands, with */
/* cue as the hit's sound (0 keeps the one set); later calls change nothing. */
void btPlaybackStart(s32 cue);
/* Writes size BGR555 colours of source to destination scaled by value / 256, */
/* at most 1.0: a palette dimmed toward black. */
void btColBufferScale(u16 *source, u32 destination, s32 value, s32 size);
/* Brings the party's recovering Djinn one turn nearer, setting each that is */
/* ready, and returns 1 when any was set. */
s32 btDjinnRecoveryCountDown(void);
/* Returns the palette of a unit's battle object. */
s32 btMotionGetSlotPalette(s32 id);
/* Runs btDjinnRecoveryCountDown twenty times, which sets every recovering */
/* Djinni of a party member with HP; those on standby stay so. Returns 0. */
s32 btDjinnRecoveryCountDownTwenty(void);
/* Debug: replaces the party with parameter set number set, each member at its
 */
/* level with its Djinn, abilities and equipped items. Returns 1 when the table
 */
/* ends before that set. */
s32 btDebugParmSetLoad(s32 set);
/* Readies every unit's sprites, destroying those of units no longer in the */
/* battle, and sets each unit's pose again. */
void btObjectPut(void);
/* Destroys a unit's sprites, its status icon and shadow with them, leaving its
 */
/* battle object with none. */
void btObjectDestroySprs(s32 unitId);
/* Shows the cursor bobbing at (x, y) and waits for the player to go on with A,
 */
/* B, L or R; returns 1. */
s32 btMessageWaitForAdvanceAt(s32 x, s32 y);
/* Counts the standby Djinn on a unit's side, by element into counts. */
s32 btDjinnCountStandby(u32 unitId, u8 *counts);
/* Lists the party's reserve members, those past the fourth (none in a link */
/* battle), into owners when it is not null, puts each on the party's side and
 */
/* returns how many there are. */
s32 btPartyPrepareReserveOwners(u16 *owners);
/* As btUnitListIds, listing only the units whose HP is not 0. */
s32 btUnitListLivingIds(s32 sideMask, u16 *unitIds);
/* Readies the sprites of the units in unitIds (ended by BATTLE_UNIT_LIST_END):
 */
/* with releaseOthers, first destroys those of every unit not listed; then */
/* creates each listed unit's sprites (body, overlay, status icon and shadow; */
/* or the two parts of a two-part enemy) unless it has them, builds its status
 */
/* icons and sets its pose. With refreshPoses, sets every listed unit's pose */
/* again afterwards. */
void btObjectSpawnObjectsForList(s16 *unitIds, s32 refreshPoses,
								 s32 releaseOthers);
/* Replaces the first enemy with the Doom Dragon's next form: */
/* ENEMY_DOOM_DRAGON_E after FORMATION_DOOM_DRAGON_A, else ENEMY_DOOM_DRAGON_H.
 */
/* The form change's battle effect calls it through its stub call. */
void btChangeEnemyForm(s32 id);
/* Works out which of the summons up to SUMMON_THOR the party can call: those */
/* whose Djinn of every element the party holds (djinnHeldCnts, set or not),
 */
/* all members together, and stores them in the party's summon mask beside */
/* the ones learned. Whether enough are on standby is checked as */
/* a summon is chosen (mnBattleSummonMenuRun) and built */
/* (btActionRunBuild). btRun calls it as the battle is set up and at */
/* the start of each turn. */
void btSummonUpdateAvailability(void);

struct ACTION_DESCRIPTOR;
struct ACTOR;
struct AREA_NAME_RULE;
struct BATTLE_ACTION_RECORD;
struct BATTLE_EFFECT_PARM;
struct BATTLE_PLAN;
struct BATTLE_PRESENTATION_STATE;
struct FIELD_PARTICLE;
struct MAP_MUSIC_RULE;
struct PALETTE_FADE;
struct PENDING_EVENT;
struct SPR;
struct TRIGGER;
struct UNIT;
struct WORLD_MAP_STATE;

/* A battle unit's sprites' OBJ mode, as btHighlightSetSprMode */
/* sets it: faded units are semi-transparent, and the battle's blends act on */
/* them. */
typedef enum BATTLE_SPR_MODE {
	SPRITE_OPAQUE,
	SPRITE_SEMI_TRANSPARENT
} BATTLE_SPR_MODE;

/* The glows btElementGlowPlay plays about a unit in its element. */
typedef enum BATTLE_ELEMENT_GLOW {
	/* The star field gathering on the unit while the background darkens: a */
	/* cast. */
	GLOW_STAR_FIELD,
	GLOW_STREAKS, /* the element's streaks: an enemy's cast */
	GLOW_PILLAR,  /* the element's pillar, at a strength: a summon's boost */
	GLOW_BURST	  /* the element's burst: a Djinni set */
} BATTLE_ELEMENT_GLOW;

/* btHighlightSetModes's modes: the units listed take one blend and */
/* every other unit the other (mode bit 0), and the battle steps the blend */
/* each frame (btBackgroundStepHighlightBlend). */
typedef enum BATTLE_HIGHLIGHT {
	HIGHLIGHT_OFF = 0,		/* every unit opaque, the blend stopped */
	HIGHLIGHT_SELECTED = 1, /* the units listed lighten by the ramp */
	/* The units listed, semi-transparent, pulse by the ramp: the reserve member
	 */
	/* a party command offers. */
	HIGHLIGHT_PULSE = 2,
	HIGHLIGHT_FADE_OUT = 4, /* the units listed fade out over 32 frames */
	HIGHLIGHT_FADE_IN = 6	/* the units listed fade in over 32 frames */
} BATTLE_HIGHLIGHT;

/* The battle sprites' animations (AnimationObject_SelectAnimation numbers) */
/* that the battle code picks by name. */
typedef enum BATTLE_POSE {
	/* The battler held still: an enemy asleep, stunned or held, or a party */
	/* member standing again between turns. */
	POSE_STILL = 0,
	POSE_STANDING = 1,
	POSE_LEAP = 2,		 /* running up to a target and leaping */
	POSE_CAST = 3,		 /* a hand raised to cast or use an item */
	POSE_PARTY_HELD = 4, /* asleep, stunned or held; also a downed member */
	POSE_DAMAGE = 5,	 /* taking damage */
	POSE_ENEMY_DOWN = 5	 /* a downed enemy stays in its damage pose */
} BATTLE_POSE;

/* BG1's vertical scroll that sets the battle background in the battle view. */
#define BATTLE_BACKGROUND_SCROLL_Y 32
/* BG1's map in a battle, a screen block: while the battle background lies */
/* flat, lines outside its plane read the padded map there; the command row */
/* places its icons' tiles in it, and the effects that take BG1 over (the */
/* canvas, Ulysses's rune circles) lay their own maps there. */
#define BATTLE_BG1_SCREEN_BLOCK 31
/* btCameraSetView's zoom, in percent, for the battle's usual view. */
#define BATTLE_ZOOM_NORMAL 100

/* The battle's multipliers are 256ths: this one keeps an amount as it is, */
/* and gmBattleGetElementMultiplier returns it for no element difference. */
#define BATTLE_MULTIPLIER_ONE 256
/* The element power at which healing is neither stronger nor weaker */
/* (gmBattleCalcRecovery): each point above it counts as two points of */
/* element difference. */
#define HEALING_BASE_POWER 100

/* The tables other modules lend the battle, in the order they are declared. */
extern const s8 Halt_FlickerColorModes[];
extern const s8 BattlePlacement_Spots[];
extern u8 Encounter_DjinnBattles[];
extern u16 BattleUnit_WeaponKinds[];
/* The 32x16 spark the swelling arcs over a caster are drawn with */
/* (field_effects.s). */
extern const u8 FieldEffect_SparkTiles[];
extern const u16 Input_DpadAnglesEightWay[];
extern const s32 ItemDisplay_SparkleScript[];
extern const u8 MindRead_CrescentEndScript[];
extern const u8 ColorBuffer_DarkenOneLevelMap[32];
extern const u8 ColorBuffer_DarkenTwoLevelsMap[32];
extern const u8 ColorBuffer_DarkenFourLevelsMap[32];

/* The battle's run and turn: the battle itself, the actions gathered and */
/* sorted, the turn's end and the spoils. */
void btIntroAnnounceEncounter(s32 enemyCount);
void btTurnStartNoOp(void);
s32 btGatherActions(struct BATTLE_ACTION_RECORD *actions);
#ifndef OWN_Battle_BuildOpponentActions
s32 btBuildEnemyActions(struct BATTLE_ACTION_RECORD *entries);
#endif
#ifndef OWN_Battle_BuildUnitActions
s32 btBuildUnitActions(struct BATTLE_ACTION_RECORD *entries);
#endif
s32 btLinkBattleWaitSync(void);
s32 btAppendLinkedActions(struct BATTLE_ACTION_RECORD *actions, s32 count);
void btActionOrderSort(struct BATTLE_ACTION_RECORD *actions, s32 count);
s32 btFindUnitPlace(u32 id);
void btUnitResetGuardLevels(void);
void btUnitProcessActionEnd(BATTLE_PLAN *plan);
void btProcessTurnEnd(void);
void btSpoilsClear(void);
s32 btEnemyRecordDefeat(s32 unitId, s32 earned);
void btAwardSpoils(void);

/* The battle's units: who is living, on which side, and where. */
s32 btUnitListLivingWithReserve(s32 sideMask, u16 *unitIds);
void btUnitRefreshPlacement(void);
s32 btUnitGetPartyAttackEffect(s32 kind);
s32 btAbilityApplyToTarget(BATTLE_PLAN *plan, s32 slot);

/* An action's staging, and the battle as Auto Battle plays it. */
s32 btStagingDispatchAction(struct BATTLE_ACTION_RECORD *action, s32 delay);
s32 btStagingRunFullScr(BATTLE_PLAN *plan, s32 mode);
s32 btStagingRunPsynergyCast(BATTLE_PLAN *plan, s32 mode);
s32 btStagingRunItem(BATTLE_PLAN *plan, s32 unused,
					 struct BATTLE_ACTION_RECORD *selection);
s32 btStagingRunFlee(BATTLE_PLAN *plan);
void btStagingBuildTargetList(BATTLE_PLAN *plan,
							  struct BATTLE_PRESENTATION_STATE *effect);
s32 btAutoBattleBuildActions(struct BATTLE_ACTION_RECORD *entries);
s32 btAutoBattleRunAction(struct BATTLE_ACTION_RECORD *action);
s32 btAutoBattleShowDefend(s16 *);
s32 btAutoBattleRunAttackOrItem(struct BATTLE_ACTION_RECORD *action);
s32 btAutoBattleRunPsynergy(struct BATTLE_ACTION_RECORD *action);
void btPlaybackRun(void);
s32 btPlaybackWaitForReady(void);
u32 btPlaybackReset(void);
void btMessageWaitForAdvance(void);

/* The battle objects: each unit's sprites on the battlefield, and how they */
/* move. */
#ifndef OWN_BattleObject_Make
void btObjectMake(struct BATTLE_OBJECT *battleObject, s32 unitId, s32 x, s32 y);
#endif
s32 btObjectRemoveFromLists(s32 id);
void btObjectReleaseSprSlots(s32 objectId);
void btObjectSetColMode(struct MOTION_OBJECT *object, s32 value);
void btObjectUpdatePose(s32 id);
void btObjectPutKeepingPoses(void);
void btObjectUpdateSprPriorities(void);
s32 btEnemySprLoadIntoSlots(s32 unit);
void btMotionHopBack(s32 objectId);
void btMotionPlayFall(s32 unitId);
void btMotionSetupEscapeObject(s32 objectId);
void btMotionReturnObjectToSpot(struct BATTLE_OBJECT *slot);
void btMotionDestroyAllSlotObjects(void);
void btMotionApproachTargetStub(s32 actorId, s32 targetId, s32 travelFrames,
								s32 initialVelocityY);

/* The camera, the background and the units' highlight. */
void btCameraSetDefaultView(void);
void btBackgroundRestorePalette(void);
void btElementGlowPlay(s32 unitId, s32 element, s32 mode, s32 value);
void btHighlightSetModesStub(u16 *, s32);

/* The battle effects these call by name (battle_effect/). */
void btEffectDoNothing(void);
void btEffectRunTargetMotes(struct BATTLE_EFFECT_PARM *effect, u32 mode);
void btEffectRunProjectileVolley(struct BATTLE_EFFECT_PARM *effect, s32 mode);
void btEffectRunHealingMotes(struct BATTLE_EFFECT_PARM *effect, u32 mode);
void btEffectRunBurstShower(struct BATTLE_EFFECT_PARM *effect, u32 mode);
void btEffectRunSparkGroups(struct BATTLE_EFFECT_PARM *effect, s32 kind);
void btEffectRunCanvasBurst(struct BATTLE_EFFECT_PARM *effect, u32 mode);
void btEffectRunStatReduction(struct BATTLE_EFFECT_PARM *effect, s32 mode);
void fdBattleEffectUpdateArcEffect(void);

/* The field engine's Psynergy, effects and palette fade, which the battle's */
/* effects and the field engine's own share. */
void fdPsynergyFrost(void);
void fdPsynergyDouse(void);
void fdPsynergyCyclone(void);
void fdPsynergyWhirlwindOnTarget(s32 unused);
void fdEffectDrawScaledActor(struct FIELD_PARTICLE *object);
void fdParticleUpdateMotion(struct FIELD_PARTICLE *effect);
/* fdAdvanceWithGravity3D for x and y alone: the third word is left as it */
/* is, and the velocities follow it. */
void fdAdvanceWithGravity2D(s32 *vector, s32 damping, s32 gravity);
/* Moves a point (three 16.16 coordinates, their velocities after them) by its
 */
/* velocity, then adds gravity to the second velocity and scales each by */
/* damping / 64. */
void fdAdvanceWithGravity3D(s32 *vector, s32 damping, s32 gravity);
void fdActorSnapToTerrain(void *actor);
struct ACTOR *fdItemDispShowItemInFront(s32 actor, s32 item);
void fdPaletteFadeBuild(u32 mode, u16 *src, u16 *dst, s32 half);
void fdPaletteFadeUpdate(void);

/* The scene's triggers and encounters, as field Psynergy and the battle's */
/* start read them. */
#ifndef OWN_BattleEffect_GetResourceGroup
s32 scGetArea(s32 sceneId);
#endif
struct ACTION_DESCRIPTOR *scFindActorPlacement(s32 id);
s32 scSelectPsynergyTarget(s32 kind);
struct TRIGGER *scFindPsynergyTrigger(s32 filter, s32 kind, s32 selector);
s32 fdMeasurePsynergyReach(s32 kind, s32 selfId, s32 otherId);
s32 fdEncounterResolveWorldMapZoneAndBackdrop(void *position);

/* The game logic: units, Djinn, flags and elements. */
s32 gmBattleGetElementMultiplier(s32 selector, s32 mode);
s32 gmEnemyGetAttackElement(UNIT *unit);
#ifndef OWN_Unit_CalculateStub
/* Works a unit's stats out again from its base stats, equipment, set Djinn, */
/* class and battle stat levels, keeping its HP and PP at the same fraction. */
void gmUnitCalcStub(s32 unitId);
#endif
/* Sets the unit's HP, kept between 0 and its maximum, and updates its gauges.
 */
void gmUnitSetHpStub(UNIT *unit, s32 hp);
s32 gmPartyCountMembersStub(void);
s32 gmDjinnSetStub(s32 unitId, s32 element, s32 index);
struct DJINN_RECOVERY_TABLE *gmDjinnGetRecoveryTableStub(s32 owner);
s32 btDjinnRecoveryRemoveStub(s32 unitId, s32 element, s32 index);

/* The main image's actors, sprites and windows. */
void fdActorSetPosAndResetMotionStub(struct ACTOR *actor, s32 x, s32 y, s32 z);
/* Sets the timer step of every sprite of an actor (16 is normal speed). */
void fdActorSetAnimationSpeedStub(struct ACTOR *actor, s32 value);
void fdActorReleaseStub(struct ACTOR *actor);
void btRenderApplyProjectedPlacementStub(SPR *sprite, s32 *position, s32 *scale,
										 s32 mode);
s32 fdSprDissolveRunStub(void *object);
void fdSprDissolveRunManyStub(void **, s32);
void mnMessageArgumentClearAllStub(void);
void mnMessageArgumentPushStub(s32 value, s32 kind);
void mnBattleMessageShowAndWaitStub(s32 messageId);
void mnBattleMessageCloseStub(void);
void mnPartyStatDrawStub(s32 mode);
void mnPartyStatHighlightMembersStub(u16 *);

#endif
