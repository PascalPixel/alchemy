#ifndef ALCHEMY_BATTLE_H
#define ALCHEMY_BATTLE_H

/* The battle engine's functions as other files call them: the battle module's
   own, file by file, then the battle effects, the field engine's functions the
   battle and its effects borrow, and the stub calls into the main image. */

#include "fixed_math.h"
#include "types.h"
#include "edition.h"
#include "system.h"
#include "battle/battle_type.h"
#include "field/sprite.h"

struct BATTLE_OBJECT;
struct MOTION_OBJECT;

/* The battle module's functions in the order of its stub table
   (src/battle/stubs.s). Debug: the battle test menu, which never returns. It
   fights the first encounter again and again with auto battle on, unless Down
   is held at the start to open the chooser of encounter and parameter set. */
void btDebugBattleRunMenu(void);
/* Fades the screen to black over sixteen frames as every background's mosaic
   grows, then shows no layer. Sets REG_BLDCNT to darken the backgrounds, leaves
   REG_MOSAIC at its largest and REG_DISPCNT in mode 1 with every layer off; the
   four BGxCNT are put back as they were. It draws four random numbers a frame,
   which only advances the random number generator. Returns 0. */
Sint32 btTransitionRunMosaicFadeOut(void);
/* Readies the sprites of the units in list, ended by BATTLE_UNIT_LIST_END, and
   destroys every other unit's; with refresh, sets the listed units' poses
   again. */
void btActorSpawnObjectsForList(Sint16 *list, Sint32 refresh);
/* Fades BG2 out over the backdrop in eight frames, its blend weight falling by
   2 a frame from 15, while the backdrop's stays 16. Leaves REG_BLDCNT blending
   BG2 over the backdrop. */
void btBackgroundFadeAlphaOut(void);
/* Fades BG2 in over the backdrop in eight frames, its blend weight rising by 2
   a frame from 1, while the backdrop's stays 16. Leaves REG_BLDCNT blending BG2
   over the backdrop. */
void btBackgroundFadeAlphaIn(void);
/* Points the battle camera at (x, depth, y) with a zoom in percent, and lines
   the zoomed background up with it: where the battlefield's origin falls on the
   screen moves the plane as much, and the zoom scales the focal length and the
   far depth. */
void btCameraSetView(Sint32 x, Sint32 depth, Sint32 y, Sint32 zoom);
/* Points the battle camera at the ground between two units, with a zoom in
   percent. */
void btCameraSetViewAtPairMidpoint(
	Sint32 firstUnitId, Sint32 secondUnitId, Sint32 zoom);
/* Switches the battle background's display mode and rebuilds both its maps. A
   nonzero fade darkens the saved palette by taking fade from each colour
   channel, and BATTLE_BACKGROUND_FADE_RESTORE puts it back undarkened; the
   background's id is not read. */
void btBackgroundSetMode(Sint32 mode, Sint32 backgroundId, Sint32 fade);
/* Loads a battle background: its picture decoded into BG VRAM, its palette
   darkened by level steps of 1/60 (none when level is negative), its tile table
   and tilemap, and the HBlank scroll the first time. */
void btBackgroundLoad(Sint32 mode, Sint32 resource, Sint32 level);
/* Darkens the battle background's palette by level steps of a sixtieth; level 0
   restores its saved colours. */
void btBackgroundSetPaletteLevel(Sint32 unused, Sint32 level);
/* Returns the height an animation is drawn at over a class, in pixels, or 0 for
   its size's default; 0 out of range. */
Uint32 btSprGetEffectHeight(Uint32 index);
/* Returns 1 for a class whose blow lands with a heavier impact; 0 out of
   range. */
Sint32 btSprHasHeavyImpact(Sint32 index);
/* Returns how high a class hops as it runs up to attack, in pixels; 0 out of
   range. */
Uint32 btSprGetAttackHop(Uint32 index);
/* Returns 1 for a small class, which takes one enemy sprite slot instead of two
   and stands closer to its neighbours; 0 out of range. */
Sint32 btSprIsSmall(Sint32 index);
/* Returns the height above a unit where its battle effects play, in 16.16
   pixels: its class's own, else EFFECT_HEIGHT_SMALL for a small enemy and
   EFFECT_HEIGHT_LARGE for others. */
FIXED btUnitGetEffectHeight(Sint32 unitId);
/* Sends one unit leaping three quarters of the way to another, arriving in
   travelFrames frames. */
void btMotionApproachTarget(Sint32 actorId, Sint32 targetId,
	Sint32 travelFrames, Sint32 initialVelocityY);
/* Unused: sends unitId leaping back to 1.3 times its spot's distance from the
   centre, in POSE_DAMAGE. */
void btMotionLeapBack(Sint32 unitId);
/* Knocks a unit back as the knockback variant (BATTLE_KNOCKBACK) says, in its
   damage pose; the Fusion Dragon and the Doom Dragon's three forms only take
   the pose. */
void btMotionApplyKnockback(Sint32 id, Sint32 variant);
/* Lists the units in the battle on the sides in sideMask, standing or not, and
   returns how many. */
Sint32				  btUnitListIds(Sint32 sideMask, Uint16 *unitIds);
struct BATTLE_OBJECT *btObjectGet(Sint32 objectId);
/* Fights the battle against the enemy formation enemyGroup, from the opening to
   the end. Returns the number of enemies defeated when the party won or the
   link broke, BATTLE_OUTCOME_FLED or BATTLE_OUTCOME_LOST. */
Sint32 btRun(Sint32 enemyGroup);
/* Projects a battle object's position to the screen, in projected. */
Sint32 btMotionProjectPos(Sint32 id, Sint32 *projected);
/* Projects the point above a unit where its battle effects play
   (btUnitGetEffectHeight) onto the screen, into screen, scaled as its sprite
   is. Returns 0. */
Sint32 btMotionProjectScaledPos(Sint32 unitId, Sint32 *screen);
/* Projects the point above a unit onto the screen, into screen: the target
   cursor's place, EFFECT_HEIGHT_SMALL up for a small enemy and
   EFFECT_HEIGHT_LARGE for others, scaled as the sprite is. Returns 0. */
Sint32 btMotionProjectCursorPoint(Sint32 unitId, Sint32 *screen);
/* Lists the units still standing on the sides in sideMask, party first, into
   unitIds when it is not null, and returns how many. */
Sint32 btUnitListLiving(Sint32 sideMask, Uint16 *unitIds);
/* Lists the party members who fight, three in a link battle, into owners when
   it is not null, and returns how many. */
Sint32 btPartyPrepareMembers(Uint16 *owners);
/* Sets the camera up for the fixed view the battle shows while commands are
   chosen, raised by verticalOffset pixels. */
void btCameraSetCommandView(Sint32 verticalOffset);
/* Returns sprite recordIndex of an actor (0 only, for a single sprite), or null
   when it has no such sprite. */
void *btActorGetSpr(struct MOTION_OBJECT *object, Sint32 recordIndex);
/* Highlights the units in actors (up to UNIT_MAX, ended by
   BATTLE_UNIT_LIST_END), the others taking the other blend, or with
   HIGHLIGHT_OFF stops the highlight. */
void btHighlightSetModes(Uint16 *actors, Sint32 mode);
/* Starts playback of the action's targets when the effect's blow lands, with
   snd as the hit's sound (0 keeps the one set); later calls change nothing. */
void btPlaybackStart(Sint32 snd);
/* Writes size BGR555 colours of source to destination scaled by value / 256, at
   most 1.0: a palette dimmed toward black. */
void btColBufferScale(
	Uint16 *source, Uint32 destination, Sint32 value, Sint32 size);
/* Brings the party's recovering Djinn one turn nearer, setting each that is
   ready, and returns 1 when any was set. */
Sint32 btDjinnRecoveryCountDown(void);
/* Returns the palette of a unit's battle object. */
Sint32 btMotionGetSlotPalette(Sint32 id);
/* Runs btDjinnRecoveryCountDown twenty times, which sets every recovering
   Djinni of a party member with HP; those on standby stay so. Returns 0. */
Sint32 btDjinnRecoveryCountDownTwenty(void);
/* Debug: replaces the party with parameter set number set, each member at its
   level with its Djinn, abilities and equipped items. Returns 1 when the table
   ends before that set. */
Sint32 btDebugParmSetLoad(Sint32 set);
/* Readies every unit's sprites, destroying those of units no longer in the
   battle, and sets each unit's pose again. */
void btObjectPut(void);
/* Destroys a unit's sprites, its status icon and shadow with them, leaving its
   battle object with none. */
void btObjectDestroySprs(Sint32 unitId);
/* Shows the cursor bobbing at (x, y) and waits for the player to go on with A,
   B, L or R; returns 1. */
Sint32 btMessageWaitForAdvanceAt(Sint32 x, Sint32 y);
/* Counts the standby Djinn on a unit's side, by element into counts. */
Sint32 btDjinnCountStandby(Uint32 unitId, Uint8 *counts);
/* Lists the party's reserve members, those past the fourth (none in a link
   battle), into owners when it is not null, puts each on the party's side and
   returns how many there are. */
Sint32 btPartyPrepareReserveOwners(Uint16 *owners);
/* As btUnitListIds, listing only the units whose HP is not 0. */
Sint32 btUnitListLivingIds(Sint32 sideMask, Uint16 *unitIds);
/* Readies the sprites of the units in unitIds (ended by BATTLE_UNIT_LIST_END):
   with releaseOthers, first destroys those of every unit not listed; then
   creates each listed unit's sprites (body, overlay, status icon and shadow; or
   the two parts of a two-part enemy) unless it has them, builds its status
   icons and sets its pose. With refreshPoses, sets every listed unit's pose
   again afterwards. */
void btObjectSpawnObjectsForList(
	Sint16 *unitIds, Sint32 refreshPoses, Sint32 releaseOthers);
/* Replaces the first enemy with the Doom Dragon's next form:
   ENEMY_DOOM_DRAGON_E after FORMATION_DOOM_DRAGON_A, else ENEMY_DOOM_DRAGON_H.
   The form change's battle effect calls it through its stub call. */
void btChangeEnemyForm(Sint32 id);
/* Works out which of the summons up to SUMMON_THOR the party can call: those
   whose Djinn of every element the party holds (djinnHeldCnts, set or not), all
   members together, and stores them in the party's summon mask beside the ones
   learned. Whether enough are on standby is checked as a summon is chosen
   (mnBattleSummonMenuRun) and built (btActionRunBuild). btRun calls it as the
   battle is set up and at the start of each turn. */
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
struct PENDING_CUE;
struct SPR;
struct CUE;
struct UNIT;
struct WORLD_MAP_STATE;

/* A battle unit's sprites' OBJ mode, as btHighlightSetSprMode sets it: faded
   units are semi-transparent, and the battle's blends act on them. */
typedef enum BATTLE_SPR_MODE {
	SPRITE_OPAQUE,
	SPRITE_SEMI_TRANSPARENT
} BATTLE_SPR_MODE;

/* The glows btElementGlowPlay plays about a unit in its element. */
typedef enum BATTLE_ELEMENT_GLOW {
	/* The star field gathering on the unit while the background darkens: a
	   cast. */
	GLOW_STAR_FIELD,
	GLOW_STREAKS, /* the element's streaks: an enemy's cast */
	GLOW_PILLAR,  /* the element's pillar, at a strength: a summon's boost */
	GLOW_BURST	  /* the element's burst: a Djinni set */
} BATTLE_ELEMENT_GLOW;

/* btHighlightSetModes's modes: the units listed take one blend and every other
   unit the other (mode bit 0), and the battle steps the blend each frame
   (btBackgroundStepHighlightBlend). */
typedef enum BATTLE_HIGHLIGHT {
	HIGHLIGHT_OFF = 0,		/* every unit opaque, the blend stopped */
	HIGHLIGHT_SELECTED = 1, /* the units listed lighten by the ramp */
	/* The units listed, semi-transparent, pulse by the ramp: the reserve member
	   a party command offers. */
	HIGHLIGHT_PULSE = 2,
	HIGHLIGHT_FADE_OUT = 4, /* the units listed fade out over 32 frames */
	HIGHLIGHT_FADE_IN = 6	/* the units listed fade in over 32 frames */
} BATTLE_HIGHLIGHT;

/* The battle sprites' animations (AnimationObject_SelectAnimation numbers) that
   the battle code picks by name. */
typedef enum BATTLE_POSE {
	/* The battler held still: an enemy asleep, stunned or held, or a party
	   member standing again between turns. */
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
/* BG1's map in a battle, a screen block: while the battle background lies flat,
   lines outside its plane read the padded map there; the command row places its
   icons' tiles in it, and the effects that take BG1 over (the canvas, Ulysses's
   rune circles) lay their own maps there. */
#define BATTLE_BG1_SCREEN_BLOCK	   31
/* btCameraSetView's zoom, in percent, for the battle's usual view. */
#define BATTLE_ZOOM_NORMAL		   100

/* The battle's multipliers are 256ths: this one keeps an amount as it is, and
   gmBattleGetElementMultiplier returns it for no element difference. */
#define BATTLE_MULTIPLIER_ONE 256
/* The element power at which healing is neither stronger nor weaker
   (gmBattleCalcRecovery): each point above it counts as two points of element
   difference. */
#define HEALING_BASE_POWER	  100

/* The tables other modules lend the battle, in the order they are declared. */
extern const Sint8 Halt_FlickerColorModes[];
extern const Sint8 BattlePlacement_Spots[];
extern Uint8	   Encounter_DjinnBattles[];
extern Uint16	   BattleUnit_WeaponKinds[];
/* The 32x16 spark the swelling arcs over a caster are drawn with
   (field_effects.s). */
extern const Uint8	FieldEffect_SparkTiles[];
extern const Uint16 Input_DpadAnglesEightWay[];
extern const Sint32 ItemDisplay_SparkleScript[];
extern const Uint8	MindRead_CrescentEndScript[];
extern const Uint8	ColorBuffer_DarkenOneLevelMap[32];
extern const Uint8	ColorBuffer_DarkenTwoLevelsMap[32];
extern const Uint8	ColorBuffer_DarkenFourLevelsMap[32];

/* The battle's run and turn: the battle itself, the actions gathered and
   sorted, the turn's end and the spoils. */
void   btIntroAnnounceEncounter(Sint32 enemyCount);
void   btTurnStartNoOperation(void);
Sint32 btGatherActions(struct BATTLE_ACTION_RECORD *actions);
#ifndef OWN_btBuildEnemyActions
Sint32 btBuildEnemyActions(struct BATTLE_ACTION_RECORD *entries);
#endif
#ifndef OWN_btBuildUnitActions
Sint32 btBuildUnitActions(struct BATTLE_ACTION_RECORD *entries);
#endif
Sint32 btLinkBattleWaitSync(void);
Sint32 btAppendLinkedActions(
	struct BATTLE_ACTION_RECORD *actions, Sint32 count);
void   btActionOrderSort(struct BATTLE_ACTION_RECORD *actions, Sint32 count);
Sint32 btFindUnitPlace(Uint32 id);
void   btUnitResetGuardLevels(void);
void   btUnitProcessActionEnd(BATTLE_PLAN *plan);
void   btProcessTurnEnd(void);
void   btSpoilsClear(void);
Sint32 btEnemyRecordDefeat(Sint32 unitId, Sint32 earned);
void   btAwardSpoils(void);

/* The battle's units: who is living, on which side, and where. */
Sint32 btUnitListLivingWithReserve(Sint32 sideMask, Uint16 *unitIds);
void   btUnitRefreshPlacement(void);
Sint32 btUnitGetPartyAttackEffect(Sint32 kind);
Sint32 btAbilityApplyToTarget(BATTLE_PLAN *plan, Sint32 slot);

/* An action's staging, and the battle as Auto Battle plays it. */
Sint32 btStagingDispatchAction(
	struct BATTLE_ACTION_RECORD *action, Sint32 delay);
Sint32 btStagingRunFullScreen(BATTLE_PLAN *plan, Sint32 mode);
Sint32 btStagingRunPsynergyCast(BATTLE_PLAN *plan, Sint32 mode);
Sint32 btStagingRunItem(
	BATTLE_PLAN *plan, Sint32 unused, struct BATTLE_ACTION_RECORD *selection);
Sint32 btStagingRunFlee(BATTLE_PLAN *plan);
void   btStagingBuildTargetList(
	  BATTLE_PLAN *plan, struct BATTLE_PRESENTATION_STATE *effect);
Sint32 btAutoBattleBuildActions(struct BATTLE_ACTION_RECORD *entries);
Sint32 btAutoBattleRunAction(struct BATTLE_ACTION_RECORD *action);
Sint32 btAutoBattleShowDefend(Sint16 *);
Sint32 btAutoBattleRunAttackOrItem(struct BATTLE_ACTION_RECORD *action);
Sint32 btAutoBattleRunPsynergy(struct BATTLE_ACTION_RECORD *action);
void   btPlaybackRun(void);
Sint32 btPlaybackWaitForReady(void);
Uint32 btPlaybackReset(void);
void   btMessageWaitForAdvance(void);

/* The battle objects: each unit's sprites on the battlefield, and how they
   move. */
#ifndef OWN_btObjectMake
void btObjectMake(
	struct BATTLE_OBJECT *battleObject, Sint32 unitId, Sint32 x, Sint32 y);
#endif
Sint32 btObjectRemoveFromLists(Sint32 id);
void   btObjectReleaseSprSlots(Sint32 objectId);
void   btObjectSetColMode(struct MOTION_OBJECT *object, Sint32 value);
void   btObjectUpdatePose(Sint32 id);
void   btObjectPutKeepingPoses(void);
void   btObjectUpdateSprPriorities(void);
Sint32 btEnemySprLoadIntoSlots(Sint32 unit);
void   btMotionHopBack(Sint32 objectId);
void   btMotionPlayFall(Sint32 unitId);
void   btMotionSetupEscapeObject(Sint32 objectId);
void   btMotionReturnObjectToSpot(struct BATTLE_OBJECT *slot);
void   btMotionDestroyAllSlotObjects(void);
void   btMotionApproachTargetStub(Sint32 actorId, Sint32 targetId,
	  Sint32 travelFrames, Sint32 initialVelocityY);

/* The camera, the background and the units' highlight. */
void btCameraSetDefaultView(void);
void btBackgroundRestorePalette(void);
void btElementGlowPlay(
	Sint32 unitId, Sint32 element, Sint32 mode, Sint32 value);
void btHighlightSetModesStub(Uint16 *, Sint32);

/* The battle effects these call by name (battle_effect/). */
void btEffectNoOperation(void);
void btEffectRunTargetMotes(struct BATTLE_EFFECT_PARM *effect, Uint32 mode);
void btEffectRunProjectileVolley(
	struct BATTLE_EFFECT_PARM *effect, Sint32 mode);
void btEffectRunHealingMotes(struct BATTLE_EFFECT_PARM *effect, Uint32 mode);
void btEffectRunBurstShower(struct BATTLE_EFFECT_PARM *effect, Uint32 mode);
void btEffectRunSparkGroups(struct BATTLE_EFFECT_PARM *effect, Sint32 kind);
void btEffectRunCanvasBurst(struct BATTLE_EFFECT_PARM *effect, Uint32 mode);
void btEffectRunStatReduction(struct BATTLE_EFFECT_PARM *effect, Sint32 mode);
void fdBattleEffectUpdateArcEffect(void);

/* The field engine's Psynergy, effects and palette fade, which the battle's
   effects and the field engine's own share. */
void fdPsynergyFrost(void);
void fdPsynergyDouse(void);
void fdPsynergyCyclone(void);
void fdPsynergyWhirlwindOnTarget(Sint32 unused);
void fdEffectDrawScaledActor(struct FIELD_PARTICLE *object);
void fdParticleUpdateMotion(struct FIELD_PARTICLE *effect);
/* fdAdvanceWithGravity3D for x and y alone: the third word is left as it is,
   and the velocities follow it. */
void fdAdvanceWithGravity2D(Sint32 *vector, Sint32 damping, Sint32 gravity);
/* Moves a point (three 16.16 coordinates, their velocities after them) by its
   velocity, then adds gravity to the second velocity and scales each by damping
   / 64. */
void fdAdvanceWithGravity3D(Sint32 *vector, Sint32 damping, Sint32 gravity);
void fdActorSnapToTerrain(void *actor);
struct ACTOR *fdItemDispShowItemInFront(Sint32 actor, Sint32 item);
void fdPaletteFadeBuild(Uint32 mode, Uint16 *src, Uint16 *dst, Sint32 half);
void fdPaletteFadeUpdate(void);

/* The scene's cues and encounters, as field Psynergy and the battle's start
   read them. */
#ifndef OWN_gmGetArea
Sint32 gmGetArea(Sint32 sceneId);
#endif
struct ACTION_DESCRIPTOR *fdFindActorPlacement(Sint32 id);
Sint32					  fdSelectPsynergyTarget(Sint32 kind);
struct CUE *fdFindPsynergyCue(Sint32 filter, Sint32 kind, Sint32 selector);
Sint32		fdMeasurePsynergyReach(Sint32 kind, Sint32 selfId, Sint32 otherId);
Sint32		fdEncounterResolveWorldMapZoneAndBackdrop(void *position);

/* The game logic: units, Djinn, flags and elements. */
Sint32 gmBattleGetElementMultiplier(Sint32 selector, Sint32 mode);
Sint32 gmEnemyGetAttackElement(UNIT *unit);
#ifndef OWN_gmUnitCalcStub
/* Works a unit's stats out again from its base stats, equipment, set Djinn,
   class and battle stat levels, keeping its HP and PP at the same fraction. */
void gmUnitCalcStub(Sint32 unitId);
#endif
/* Sets the unit's HP, kept between 0 and its maximum, and updates its
   gauges. */
void   gmUnitSetHpStub(UNIT *unit, Sint32 hp);
Sint32 gmPartyCountMembersStub(void);
Sint32 gmDjinnSetStub(Sint32 unitId, Sint32 element, Sint32 index);
struct DJINN_RECOVERY_TABLE *gmDjinnGetRecoveryTableStub(Sint32 owner);
Sint32 gmDjinnRecoveryRemoveStub(Sint32 unitId, Sint32 element, Sint32 index);

/* The main image's actors, sprites and windows. */
void fdActorSetPosAndResetMotionStub(
	struct ACTOR *actor, Sint32 x, Sint32 y, Sint32 z);
/* Sets the timer step of every sprite of an actor (16 is normal speed). */
void fdActorSetAnimationSpeedStub(struct ACTOR *actor, Sint32 value);
void fdActorReleaseStub(struct ACTOR *actor);
void fdRenderApplyProjectedPlacementStub(
	SPR *sprite, Sint32 *position, Sint32 *scale, Sint32 mode);
Sint32 fdSprDissolveRunStub(void *object);
void   fdSprDissolveRunManyStub(void **, Sint32);
void   mnMessageArgumentClearAllStub(void);
void   mnMessageArgumentPushStub(Sint32 value, Sint32 kind);
void   mnBattleMessageShowAndWaitStub(Sint32 messageId);
void   mnBattleMessageCloseStub(void);
void   mnPartyStatDrawStub(Sint32 mode);
void   mnPartyStatHighlightMembersStub(Uint16 *);

#endif
