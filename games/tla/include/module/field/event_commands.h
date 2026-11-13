#ifndef ALCHEMY_MODULE_FIELD_EVENT_COMMANDS_H
#define ALCHEMY_MODULE_FIELD_EVENT_COMMANDS_H

/// @file
/// What games/tla/src/field/ declares for the files that use it, written once.

#include "types.h"
#include "effect_sprite.h"
#include "field/field_particle.h"

struct Actor;
struct AnimationEntry;
struct Effect04Vector;
struct EffectVector;

struct ObjectPairPosition {
  u8 unk00[6];
  s16 angle;
  s32 x;
  s32 y;
  s32 z;
};

struct ObjectPart {
  u8 unk00[5];
  u8 attribute;
  u8 unk06[0xa];
  void *graphics;
};

struct ObjectPartList {
  u8 unk00[0x19];
  u8 dirty;
  u8 unk1a;
  u8 count;
  u8 unk1c[0xc];
  struct ObjectPart *parts[1];
};

struct EventWaitWork {
  u8 unk000[0x1b8];
  s32 waitMode;
};

struct EventWordWork {
  u8 unk00[0x10];
  s32 word10;
};

struct OwnerVitals {
  u8 unk000[20];
  s16 hpRatio;
  s16 ppRatio;
  u8 unk018[28];
  s16 maxHp;
  s16 maxPp;
  s16 hp;
  s16 pp;
  u8 unk03c[0x131 - 60];
  u8 status;
};

struct ItemHead {
  u8 unk00[3];
  u8 flags;
};

struct WaveFxWork {
  u16 lines[2][162];
  u16 phase;
  u8 page;
  s8 red;
  s8 green;
  s8 blue;
  u16 hue;
  u16 sourceId;
  u16 targetId;
  u8 delay;
  u8 counter;
};

struct ScatterPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct ParticleTimer {
  u8 unk00[0x5e];
  u16 timer;
};

// ⚓️'s composite sprite (as field.h's CompositeAnimation).
struct CloakSprite {
  u8 unk00[0x19];
  u8 dirty; // redraw the parts
  u8 blend; // 2 draws the caster half transparent, 1 shows it plainly
  u8 unk1b[0x0d];
  struct AnimationEntry *entries[4];
};

struct CloakCaster {
  u8 unk00[0x50];
  struct CloakSprite *sprite;
  u8 unk54[0x18];
  void (*hook)(void);
};

struct TouchCell {
  s16 x;
  s16 z;
};

struct TouchLayer {
  u8 unk00[8];
  s32 x;
  s32 z;
  u8 unk10[40];
};

struct TouchMap {
  u8 unk000[264];
  struct TouchLayer layers[1];
};

struct TouchCopy {
  u8 toX;
  u8 toZ;
  u8 fromX;
  u8 fromZ;
  u8 cellX;
  u8 cellZ;
  u8 unk06[2];
};

struct TouchCopies {
  struct TouchCell unk0000[1491];
  struct TouchCopy copies[64];
  s8 count;
};

struct BurstParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 heading;
  s32 speed;
  s32 spin;
  s32 delay;
};

struct BurstWork {
  u8 tiles[384];
  struct EffectSprite sprites[96];
  struct BurstParticle particles[96];
  s16 resource;
  s16 vram;
  u8 unk1b04[2];
  s16 frame;
  s16 active;
};

struct BurstCell {
  s16 x;
  s16 z;
};

struct CueObject {
  u8 unk00[8];
  s32 x;
  s32 base;
  s32 top;
};

struct ScaledSprite {
  u8 unk00[0xc];
  s32 scale;
};

struct SparklePicture {
  s16 dy;
  u16 tile;
};

void Motion_LaunchFromFocusedObject(u32 arg0, s32 arg1, s32 arg2, s32 arg3);

extern const s32 Data_080f085c[];
extern const s32 Data_080f0874[];

s32 PartyInventory_FindRoomFar(s32 item);

s32 ItemPicker_RunToDropFar(s32 *member, s32 *slot);

void FieldEffect_AdvanceHueCycle(void);
void BattleEffect_SetCallbackWhenTargetUnset(struct Actor *object);
void Actor_SetSpritePalette(void *object, s32 value);

void FieldParticle_Initialize(struct FieldParticle *effect, s32 kind, s32 x,
                              s32 z);

s32 ActorScript_CheckOverlapFar(struct Actor *object,
                                struct Effect04Vector *position);

void FieldPsynergy_WatchLeaderDistance(void);

void FieldEffect_SpawnBurstParticleField(void);

s32 Actor_GetSpriteIdFar(struct Actor *object);

extern const s32 gFieldEffect_SourceHoldScript[];

struct Actor *FieldEffect_SpawnLiftHand(s32 x, s32 y, s32 z, s32 angle);

void FieldEffect_ClearOwnedSlot(struct FieldParticle *slot);

void FieldPsynergy_Ply(void);
void FieldPsynergy_EndCloak(void);

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);

struct Actor *FieldEffect_SpawnCarryHand(s32 x, s32 y, s32 z, s32 angle);

void MapGrid_CopyCellPictureCellsRectFar(s32 toX, s32 toZ, s32 width,
                                         s32 height, s32 fromX, s32 fromZ);

void ParticleBurst_Update(void);

extern char ResourceId_Leaves;

void BattleEffect_SetObjectUpdateMode(struct Actor *object, s32 mode);

void FieldPsynergy_Blaze(void);

void FieldPsynergy_Sand(void);

void FieldPsynergy_Burst(void);

void FieldPsynergy_Pound(void);

void FieldPsynergy_Scoop(void);
void FieldPsynergy_Parch(void);

void FieldPsynergy_Hover(void);

void FieldPsynergy_EndSand(void);

extern char ResourceId_SparkleFade;

s32 Actor_GetCellIndex(struct Actor *object);
s32 MapCollision_FindOrAddTag(s32 unused, s32 key, s32 value);

void WorldPicture_GetLeaderPoint(s32 *screenX, s32 *screenY);

extern s16 gData_0202a642;

void FieldPsynergy_Lash(void);

void FieldEffect_ShrinkActorScaleUntilHalf(struct Actor *object);

void Camera_WorldToScreen(struct EffectVector *position);

void BattleEffect_DrawCanvasBlock(const u8 *src, s32 x, s32 y, s32 width,
                                  s32 height);

void FieldPsynergy_Unnamed29(void);

void FieldPsynergy_Halt(void);

void FieldPsynergy_Catch(void);
void FieldEffect_UpdateRadialLaunch(struct FieldParticle *slot);

void FieldPsynergy_Whirlwind(void);

void BattleEffect_StartArcEffect(void);

void BattleEffect_WaitForArcGroups(void);

void BattleEffect_EndArcEffect(void);

void BattleEffect_CleanupSceneObjects(void);
void EventObject_Initialize(void);

void Djinn_ResolvePending(s32 shown);

void TileCopies_Restore(void);

void Djinn_RestoreActorAfterBattle(void);

void FieldPsynergy_ResumeCloakCheck(void);

void Teleport_PlaySparksAndFade(void);
void Teleport_PlayWhirl(void);

void Field_StartEncounterTransition(s32 unused);

void SummonTablet_PlayRunes(s32 targetId);

void FieldPsynergy_ResumeHaltedActor(s32 id);

void Actor_JumpDownLedge(s32 id, s32 mode, s32 particle);

void *Sprite_ShareVramCacheEntry(void *sprite, void *last);

void Djinn_RunVenusJoinEffect(s32 actor);

void Djinn_RunMercuryJoinEffect(s32 actor);

void Djinn_RunMarsJoinEffect(s32 actor);

void Djinn_RunJupiterJoinEffect(s32 actor);

void FieldPsynergy_Move(void);

void FieldPsynergy_Carry(void);

void FieldPsynergy_Force(void);

void FieldPsynergy_Growth(void);

void FieldPsynergy_Retreat(void);

void FieldPsynergy_Avoid(void);

void FieldPsynergy_StartCloak(void);

void FieldPsynergy_Reveal(void);

void FieldPsynergy_Tremor(void);

void FieldPsynergy_Grind(void);

void FieldPsynergy_Teleport(void);

void FieldPsynergy_Unnamed28(void);

s32 FieldPsynergy_FilterHaltTarget(s32 id);
void FieldPsynergy_HaltActor(s32 id);

void FieldPsynergy_EndReveal(void);

void FieldPsynergy_SteerLiftedTarget(s32 targetId);

void FieldPsynergy_GrowthOnTarget(s32 unused);

void FieldPsynergy_LashOnTarget(s32 value);

void FieldPsynergy_ShowAvoidWoreOff(void);

void FieldPsynergy_EndHover(void);

void FieldPsynergy_EndUnnamed28(void);

void FieldPsynergy_StopMindReadWave(void);

void Scene_QueueMusic(s32 soundId);

void FieldEffect_UpdateRandomTargetParticle(struct FieldParticle *effect);

s32 Actor_GetScreenPosition(s32 objectId, s32 *position);

void Weather_UpdateLightning(void);

void Field_RequestBattleFromEncounterTable(s32 row, s32 column);

void Party_LearnAbility(s32 unit, s32 ability);
void Party_RemoveMemberRestored(s32 owner);

struct Actor;

// The field event work's pending Psynergy request (eventWork + 0x166).
struct CloakEvent {
  u8 unk000[0x166];
  s16 psynergyRequest;
};

struct FallingObject {
  u8 unk00[0xc];
  s32 y;
  u8 unk10[4];
  s32 floor;
  u8 unk18[0x46];
  s16 dispatchState;
  u8 unk60[0xc];
  void *update;
};

struct Effect05Sprite {
  u32 unk00[2];
  u8 unk08;
  u32 unk09_0 : 2;
  u32 mode : 2;
  u32 unk09_4 : 4;
};

struct Effect05State {
  u16 angle;
  u8 unk02[2];
  s32 x;
  s32 y;
  s32 z;
  struct Actor *target;
  struct Actor *anchor;
  s16 firstObjectId;
  s16 secondObjectId;
  u8 unk1c[4];
  u8 highArc;
  u8 unk21[0x13];
  u8 variant;
  u8 unk35[0x0c];
  u8 alternate;
};

struct FallVector {
  s32 x;
  s32 y;
  s32 z;
};

struct FxPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct FxObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 ground;
  u8 unk18[10];
  u8 mode;
  u8 unk23;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 speed;
  s32 lift;
  u8 unk38[12];
  s32 gravity;
  u8 unk48[13];
  u8 flag;
  u8 unk56[4];
  u8 unk5a;
  u8 unk5b;
  u8 unk5c[12];
  struct FxObject *linked;
  void *callback;
};

// A flag entry as the world map reads it: its world position.
struct WorldMapPlace {
  u8 flag;
  u8 value;
  s16 unk02;
  s16 x;
  s16 z;
  u8 unk08[12];
};

struct WorldMapMarker {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 objectMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
};

// A halfword typed beside a word, for the FAKEMATCH that reads the name box's
// y; no such data exists.
struct WorldMapAliasedHalfword {
  s16 value;
  s32 unk04;
};

struct LoadEventWork {
  u8 unk000[0xca5];
  u8 unkca5;
  u8 unkca6[7];
  u8 unkcad;
  u8 unkcae[8];
  s8 unkcb6;
  u8 unkcb7[0x79];
  u8 unkd30;
  s8 target;
  s8 targetY;
  s8 targetZ;
  u8 unkd34[4];
  u8 unkd38;
};

struct PairScene {
  u16 angle;  ///< Four directions.
  u16 angle8; ///< Eight directions.
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
  struct Actor *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u8 unk1c[0x1c];
  void *savedCallback;
  const s32 *savedScript;
  u8 savedPalette;
  u8 unk41[0x6d7];
  u8 savedTerrain;
  u8 savedPriority;
};

struct FinishWork {
  u8 unk000[16];
  struct Actor *object;
  u8 unk014[8];
  s16 quantity;
  u8 unk01e[4];
  s8 sound;
  s8 showQuantity;
  u8 unk024[0x6ee];
  s8 messageRow;
  u8 messageShown;
};

struct EffectSceneWork {
  u8 reserved00[16];
  struct Actor *mainObject;
};

struct CleanupWork {
  u8 unk000[16];
  struct Actor *owner;
  u8 unk014[10];
  s16 animation;
  u8 unk020[36];
  s32 savedShakeX;
  s32 savedShakeY;
  s32 savedShakeDecay;
  struct FieldParticle slots[24];
  u8 unk710[8];
  u8 ownerByte;
  u8 layer;
};

struct CleanupEventWork {
  u8 unk000[394];
  s8 mode;
  u8 unk18b[0xca5 - 395];
  s8 unkca5;
  u8 unkca6[7];
  s8 unkcad;
  u8 unkcae[8];
  s8 unkcb6;
  s8 unkcb7;
};

void FieldEffect_CheckCloakTile(void);

void Func_080dc954(void);

void Actor_SetOthersPaused(struct Actor *excludedObject, s32 groupMode,
                           s32 action);

extern const u8 gWorldMap_MarkerBlendCycle[];

extern const u16 gWorldMap_CursorDirectionAngles[];

extern s32 Data_0202a644;
extern s32 Data_0202a648;
extern s16 Data_0202a640;
extern s16 Data_0202a654[];
extern s16 Data_0202a656[];
extern struct WorldMapMarker Data_0202a620;

void FieldPsynergy_GreyMindReadPalette(void);

void FieldPsynergy_UpdateMindReadWave(void);

void FieldPsynergy_RestoreMindReadPalette(void);

void FieldEffect_ArmBg0HBlankDma(void);

struct Actor;
struct Vec3;

struct WaveFxSprite {
  u32 unk00[2];
  u8 unk08;
  u32 unk09_0 : 2;
  u32 mode : 2;
  u32 unk09_4 : 4;
  u8 unk0a[8];
  u16 orientation;
  u8 unk14[6];
  u8 palette;
};

struct WaveFxVector {
  s32 x;
  s32 y;
  s32 z;
};

struct WaveFxDescriptor {
  s16 sprite;
};

struct WaveFxMetadata {
  u8 unk00[8];
  s8 boxX;
};

struct ResourceEntry {
  u8 unk00[0x10];
  u8 vramBlock;
};

struct Effect04Scene {
  u16 angle;
  u8 unk02[2];
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
  struct Actor *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u8 unk1c[4];
  s8 enabled;
  u8 unk21[0x13];
  s8 childOption;
  s8 childMode;
  u8 unk36[0x1a];
  struct FieldParticle slots[24];
};

struct Effect04Vector {
  s32 x;
  s32 y;
  s32 z;
};

struct OrbitingParticleVector {
  s32 x;
  s32 y;
  s32 z;
};

struct OrbitingParticleChild {
  u8 reserved00[35];
  u8 flags;
};

// The field event work's Reveal state (eventWork + 0xca4).
struct RevealEvent {
  u8 unk000[0xca4];
  s8 active;   // Reveal is showing
  u8 markers;  // the nearby markers are up
  s16 frames;  // how long it lasts
  s16 centreX; // pixels
  s16 centreY;
};

struct BurstParticleVector {
  s32 values[3];
};

struct BurstParticleVisual {
  u8 reserved00[5];
  u8 flagsALow : 5;
  u8 flip : 1;
  u8 flagsAHigh : 2;
  u8 reserved06;
  u8 flagsBLow : 6;
  u8 flagsBHigh : 2;
  union {
    struct {
      u16 shape : 10;
      u16 unused : 6;
    } bits;
    struct {
      u8 lo;
      u8 highLow : 4;
      u8 palette : 4;
    } bytes;
  } attributes;
};

// ⚓️'s sprite keeps its second part 28 bytes in.
struct BurstParticleVisualGroup {
  struct BurstParticleVisual primary;
  u8 reserved0c[16];
  struct BurstParticleVisual child;
};

struct BattleEffectScene {
  u8 reserved00[4];
  s32 x;
  s32 y;
  s32 z;
  void *mainObject;
  void *child;
};

struct EffectChild {
  u8 reserved00[12];
  s32 y;
  u8 reserved10[0x55 - 0x10];
  u8 flag;
  u8 reserved56[0x6c - 0x56];
  void (*callback)(void);
};

struct EffectTarget {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct RadialBurstEffect {
  u8 unk00[12];
  s32 x;
  s32 z;
  s32 sourceX;
  s32 sourceZ;
  u8 unk1c[4];
  s32 velocityX;
  s32 velocityZ;
  u8 unk28[10];
  u16 scale;
  u8 unk34[12];
  s8 state;
  u8 unk41;
  u8 enabled;
};

struct ImpactVector {
  s32 x;
  s32 y;
  s32 z;
};

struct WaveFxScroll {
  u16 lines[2][162];
  u8 unk288[2];
  u8 page;
  u8 unk28b[9];
  u8 delay;
};

struct ItemBreakFragmentPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct ItemBreakFragmentSource {
  u8 reserved00[6];
  u16 angle; ///< Facing angle (0x10000 is one turn).
  s32 x;
  s32 y;
  s32 z;
  u8 reserved14[70];
  u8 flag5a;
  u8 reserved5b[13];
  struct ItemBreakFragmentSource *target; ///< Facing target.
};

struct ItemBreakFragmentObject {
  u8 reserved00[72];
  s32 field48; ///< Gravity (same offset as gravity).
  u8 reserved4c[9];
  u8 mode55; ///< Same offset as motionFlags.
  u8 reserved56[8];
  u16 field5e;
};

struct Output_08097f80 {
  s32 x;
  s32 y;
  s32 z;
};

struct SwayObject {
  u8 unk00[8];
  s32 y;
  u8 unk0c[44];
  s32 baseY;
  u8 unk3c[40];
  s16 phase;
};

struct OrbitVector {
  s32 x;
  s32 y;
  s32 z;
};

struct SparkObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[4];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[24];
  s32 anchorX;
  u8 unk3c[12];
  s32 gravity;
  u8 unk4c[9];
  u8 motionFlags;
  u8 unk56[8];
  u16 unk5e;
  u8 unk60[4];
  s16 phase;
  u8 unk66[6];
  void (*update)(struct SparkObject *);
};

struct OwnerObject {
  u8 unk00[8];
  s32 x;
  u8 unk0c[4];
  s32 z;
  u8 unk14[14];
  u8 layer;
};

extern const u8 gData_080f0ef8[];

extern const u8 gData_080f0e58[];
extern u8 gData_080f0e9c[];

struct Actor *ActorScript_FindOverlappingEntryFar(struct Actor *object,
                                                  struct Vec3 *position);

extern const u8 gUnused_BattleEffectBytes[];

#endif
