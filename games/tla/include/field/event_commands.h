#ifndef ALCHEMY_FIELD_EVENT_COMMANDS_H
#define ALCHEMY_FIELD_EVENT_COMMANDS_H

/// @file
/// What games/tla/src/field/ declares for the files that use it, written once.

#include "types.h"
#include "sprite_ids.h"
#include "resource_ids.h"
#include "field/effect_sprite.h"
#include "field/field_particle.h"
#include "field/world_map_marker.h"

struct Actor;
struct SpriteAnimation;
struct Effect04Vector;
struct EffectVector;

struct ItemHead {
  u8 unused[3]; ///< no code names it
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

/// A composite sprite, as field.h's CompositeAnimation.
struct CloakSprite {
  u8 unused[0x19]; ///< no code names it
  u8 dirty;        // redraw the parts
  u8 blend;        // 2 draws the caster half transparent, 1 shows it plainly
  u8 unk1b[0x0d];
  struct SpriteAnimation *entries[4];
};

struct CloakCaster {
  u8 unused[0x50]; ///< no code names it
  struct CloakSprite *sprite;
  u8 unusedAfterSprite[0x18]; ///< no code names it
  void (*hook)(void);
};

struct TouchCell {
  s16 x;
  s16 z;
};

struct TouchLayer {
  u8 unused[8]; ///< no code names it
  s32 x;
  s32 z;
  u8 unusedAfterZ[40]; ///< no code names it
};

struct TouchMap {
  u8 unused[264]; ///< no code names it
  struct TouchLayer layers[1];
};

struct TouchCopy {
  u8 toX;
  u8 toZ;
  u8 fromX;
  u8 fromZ;
  u8 cellX;
  u8 cellZ;
  u8 unusedAfterCellZ[2]; ///< no code names it
};

struct TouchCopies {
  struct TouchCell unk0000[1491];
  struct TouchCopy copies[64];
  s8 count;
};

/// A particle of the particle burst (ParticleBurst_Start): its place, heading,
/// speed, spin and the frames before it starts.
struct BurstParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 heading;
  s32 speed;
  s32 spin;
  s32 delay;
};

/// The particle burst's work (ParticleBurst_Start): its tiles, 96 sprites and
/// particles, its VRAM cache entry, a flash on frame 30 and its frame count.
struct ParticleBurstWork {
  u8 tiles[384];
  struct EffectSprite sprites[96];
  struct BurstParticle particles[96];
  s16 resource;
  s16 vram;
  s16 flash; ///< set on frame 30 only
  s16 frame;
  s16 active;
};

/// A cell the particle burst covers, in cells.
struct BurstCell {
  s16 x;
  s16 z;
};

struct ScaledSprite {
  u8 unused[0xc]; ///< no code names it
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
void FieldEffect_ReleaseOnArrival(struct Actor *object);
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

void BattleEffect_SetObjectUpdateMode(struct Actor *object, s32 mode);

void FieldPsynergy_Blaze(void);

void FieldPsynergy_Sand(void);

void FieldPsynergy_Burst(void);

void FieldPsynergy_Pound(void);

void FieldPsynergy_Scoop(void);
void FieldPsynergy_Parch(void);

void FieldPsynergy_Hover(void);

void FieldPsynergy_EndSand(void);

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

void FieldPsynergy_End(void);
void FieldPsynergy_StartCast(void);

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

/// The encounter table row of the hidden Mad Plants' battles, one column
/// each, in FLAG_MAD_PLANT's order.
#define ENCOUNTER_TABLE_MAD_PLANTS 106
void MadPlant_LeapAtLeader(s32 sprite, s32 x, s32 z);

void Party_LearnAbility(s32 unit, s32 ability);
void Party_RemoveMemberRestored(s32 owner);

struct Actor;

struct FallingObject {
  u8 unused[0xc]; ///< no code names it
  s32 y;
  u8 unusedAfterY[4]; ///< no code names it
  s32 floor;
  u8 unusedAfterFloor[0x46]; ///< no code names it
  s16 dispatchState;
  u8 unusedAfterDispatchState[0xc]; ///< no code names it
  void *update;
};

struct Effect05Sprite {
  u32 unused[2]; ///< no code names it
  u8 unk08;
  u32 unk09_0 : 2;
  u32 mode : 2;
  u32 unk09_4 : 4;
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

// A flag entry as the world map reads it: its world position.
struct WorldMapPlace {
  u8 flag;
  u8 value;
  s16 unk02;
  s16 x;
  s16 z;
  u8 unusedAfterZ[4];            ///< no code names it
  s16 name;                      ///< the place name's message
  s16 nameEntrance;              ///< the name's message for the entrance
  u8 unusedAfterNameEntrance[4]; ///< no code names it
};

// A halfword typed beside a word, for the FAKEMATCH that reads the name box's
// y; no such data exists.
struct WorldMapAliasedHalfword {
  s16 value;
  s32 unk04;
};

void FieldEffect_CheckCloakTile(void);

void FieldPsynergy_MarkWorldMapTilesUsed(void);

void Actor_SetOthersPaused(struct Actor *excludedObject, s32 groupMode,
                           s32 action);

extern const u8 gWorldMap_MarkerBlendCycle[];

extern const u16 gWorldMap_CursorDirectionAngles[];

extern s32 gWorldPicture_GlideTargetX;
extern s32 gWorldPicture_GlideTargetY;
extern s16 gWorldPicture_Zoomed;
extern s16 gWorldPicture_NameRedrawPending[];
extern s16 gWorldPicture_NameRedrawDelay[];
extern struct WorldMapMarker gWorldPicture_CursorMarker;

void FieldPsynergy_GreyMindReadPalette(void);

void FieldPsynergy_UpdateMindReadWave(void);

void FieldPsynergy_RestoreMindReadPalette(void);

void FieldEffect_ArmBg0HBlankDma(void);

struct Actor;
struct Vec3;

struct WaveFxSprite {
  u32 unused[2]; ///< no code names it
  u8 unk08;
  u32 unk09_0 : 2;
  u32 mode : 2;
  u32 unk09_4 : 4;
  u8 unusedB[8]; ///< no code names it
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
  u8 unused[8]; ///< no code names it
  s8 boxX;
};

struct ResourceEntry {
  u8 unused[0x10]; ///< no code names it
  u8 vramBlock;
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

/// A sprite whose second part is 28 bytes in.
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
  void *mainActor;
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
  u8 unused[8]; ///< no code names it
  s32 x;
  s32 y;
  s32 z;
};

struct RadialBurstEffect {
  u8 unused[12]; ///< no code names it
  s32 x;
  s32 z;
  s32 sourceX;
  s32 sourceZ;
  u8 unusedAfterSourceZ[4]; ///< no code names it
  s32 velocityX;
  s32 velocityZ;
  u8 unusedAfterVelocityZ[10]; ///< no code names it
  u16 scale;
  u8 unusedAfterScale[12]; ///< no code names it
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
  u8 unused[2]; ///< no code names it
  u8 page;
  u8 unusedAfterPage[9]; ///< no code names it
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
  u8 unused[8]; ///< no code names it
  s32 y;
  u8 unusedAfterY[44]; ///< no code names it
  s32 baseY;
  u8 unusedAfterBaseY[40]; ///< no code names it
  s16 phase;
};

struct OrbitVector {
  s32 x;
  s32 y;
  s32 z;
};

struct SparkObject {
  u8 unused[8]; ///< no code names it
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[4];
  s32 scaleX;
  s32 scaleY;
  u8 unusedAfterScaleY[24]; ///< no code names it
  s32 anchorX;
  u8 unusedAfterAnchorX[12]; ///< no code names it
  s32 gravity;
  u8 unusedAfterGravity[9]; ///< no code names it
  u8 motionFlags;
  u8 unusedAfterMotionFlags[8]; ///< no code names it
  u16 unk5e;
  u8 unusedB[4]; ///< no code names it
  s16 phase;
  u8 unusedAfterPhase[6]; ///< no code names it
  void (*update)(struct SparkObject *);
};

struct OwnerObject {
  u8 unused[8]; ///< no code names it
  s32 x;
  u8 unusedAfterX[4]; ///< no code names it
  s32 z;
  u8 unk14[14];
  u8 layer;
};

extern const u8 gBattleEffect_HeavyImpactScript[];

extern const u8 gData_080f0e58[];
extern u8 gAnim_ChildPaletteCycle[];

struct Actor *ActorScript_FindOverlappingEntryFar(struct Actor *object,
                                                  struct Vec3 *position);

extern const u8 gUnused_BattleEffectBytes[];

#endif
