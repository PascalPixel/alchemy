#ifndef ALCHEMY_FIELD_EVENT_COMMANDS_H
#define ALCHEMY_FIELD_EVENT_COMMANDS_H

/// @file
/// What the event commands share with the field and the scenes: the field
/// Psynergy's casts and their states, the Djinn's join effects, the map
/// screen's state, and the encounter zones of the story's battles.

#include "types.h"
#include "system/vector3.h"
#include "sprite_ids.h"
#include "resource_ids.h"
#include "field/effect_sprite.h"
#include "field/field_particle.h"
#include "field/world_map_marker.h"

struct Actor;
struct SpriteAnimation;
struct Vector3;

/// A hue's degrees, a full turn of the colour wheel.
#define HUE_DEGREES 360

/// Mind Read's wave (heap slot HEAP_SLOT_MIND_READ): each scanline's BG0
/// scroll on two pages, the hue it tints the screen with, its caster and
/// target, and its delay and particle cycle.
struct MindReadState {
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

/// MindReadState.counter held still: the wave sends no particles and plays no
/// sound, as while Lonesome George carries the party.
#define MIND_READ_QUIET 99

/// A cell a Psynergy touched, in cells, as FieldState.psynergyCells lists them
/// (x of -1 ends the list).
struct TouchCell {
  s16 x;
  s16 z;
};

/// A cell's tiles that Cyclone copied (TileCopies_CopyTouchedCells): where it
/// went, where it came from and the touched cell it opened, each in cells.
struct TouchCopy {
  u8 toX;
  u8 toZ;
  u8 fromX;
  u8 fromZ;
  u8 cellX;
  u8 cellZ;
  u8 unusedAfterCellZ[2]; ///< no code names it
};

/// The copies of cells Cyclone blew away, kept after the scene's own state so
/// that TileCopies_Restore makes them again.
struct TouchCopies {
  struct TouchCell sceneState[1491]; ///< the scene's own state, not read here
  struct TouchCopy copies[64];
  s8 count;
};

/// The cells Psynergy moved, kept in the scene's own state (gSceneLocalState).
#define gTouchCopies (*(struct TouchCopies *)gSceneLocalState)

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

/// The particles of the particle burst.
#define PARTICLE_BURST_COUNT 96

/// The particle burst's state (ParticleBurst_Start): its tiles, 96 sprites and
/// particles, its VRAM cache entry, a flash on frame 30 and its frame count.
struct ParticleBurstState {
  u8 tiles[384];
  struct EffectSprite sprites[PARTICLE_BURST_COUNT];
  struct BurstParticle particles[PARTICLE_BURST_COUNT];
  s16 cacheEntry; ///< the VRAM cache entry holding the leaves' tiles
  s16 tile;       ///< their first tile in OBJ VRAM
  s16 flash;      ///< set on frame 30 only
  s16 frame;
  s16 active;
};

/// A cell the particle burst covers, in cells.
struct BurstCell {
  s16 x;
  s16 z;
};

/// One step of a sparkle's rise (gFieldEffect_SparklePictures): its height
/// above its spot in screen pixels, and its tile.
struct SparklePicture {
  s16 dy;
  u16 tile;
};

/// PartyInventory_FindRoom through its far call: the first party member with
/// room for item, or -1.
s32 PartyInventory_FindRoomFar(s32 item);
/// ItemPicker_RunToDrop through its far call: the item picker for an item to
/// drop, which sets the member and slot chosen.
s32 ItemPicker_RunToDropFar(s32 *member, s32 *slot);
/// ActorScript_CheckOverlap through its far call: -1 when the actor at
/// position would overlap a solid actor, otherwise 0.
s32 ActorScript_CheckOverlapFar(struct Actor *actor, struct Vector3 *position);
/// Actor_GetSpriteId through its far call: the sprite id the actor is drawn
/// with, or 0 for none.
s32 Actor_GetSpriteIdFar(struct Actor *actor);
/// MapGrid_CopyCellTilesCellsRect through its far call: copies width by
/// height cells as they are from (fromX, fromZ) to (toX, toZ).
void MapGrid_CopyCellTilesCellsRectFar(s32 toX, s32 toZ, s32 width, s32 height,
                                       s32 fromX, s32 fromZ);
/// ActorScript_FindOverlappingActor through its far call: the first solid actor
/// the actor at position would overlap, or NULL.
struct Actor *ActorScript_FindOverlappingActorFar(struct Actor *actor,
                                                  struct Vector3 *position);

/// The field Psynergy, each cast as the event commands run it, with the
/// routines that end a lasting one, act on a target or resume it.
void FieldPsynergy_StartCast(void);
void FieldPsynergy_End(void);
void FieldPsynergy_Move(void);
void FieldPsynergy_Carry(void);
void FieldPsynergy_SteerLiftedTarget(s32 targetId);
void FieldPsynergy_Force(void);
void FieldPsynergy_Lash(void);
void FieldPsynergy_LashOnTarget(s32 value);
void FieldPsynergy_Pound(void);
void FieldPsynergy_Tremor(void);
void FieldPsynergy_Scoop(void);
void FieldPsynergy_Growth(void);
void FieldPsynergy_GrowthOnTarget(s32 unused);
void FieldPsynergy_Whirlwind(void);
void FieldPsynergy_Parch(void);
void FieldPsynergy_Sand(void);
void FieldPsynergy_EndSand(void);
void FieldPsynergy_Burst(void);
void FieldPsynergy_Blaze(void);
void FieldPsynergy_Ply(void);
void FieldPsynergy_Halt(void);
void FieldPsynergy_HaltActor(s32 id);
s32 FieldPsynergy_FilterHaltTarget(s32 id);
void FieldPsynergy_ResumeHaltedActor(s32 id);
void FieldPsynergy_Catch(void);
void FieldPsynergy_Arrow(void);
void FieldPsynergy_Retreat(void);
void FieldPsynergy_Avoid(void);
void FieldPsynergy_ShowAvoidWoreOff(void);
void FieldPsynergy_StartCloak(void);
void FieldPsynergy_EndCloak(void);
void FieldPsynergy_ResumeCloakCheck(void);
void FieldPsynergy_Reveal(void);
void FieldPsynergy_EndReveal(void);
void FieldPsynergy_Hover(void);
void FieldPsynergy_EndHover(void);
void FieldPsynergy_Grind(void);
void FieldPsynergy_Teleport(void);
void FieldPsynergy_Ring(void);
void FieldPsynergy_EndRing(void);
void FieldPsynergy_WatchLeaderDistance(void);

/// The stage Burst passes its target's event each frame (BurstState.shake).
enum BurstStage {
  BURST_STAGE_QUIET,       ///< no explosion this frame
  BURST_STAGE_DYING_DOWN,  ///< the great blast dying down
  BURST_STAGE_SMALL_BLAST, ///< a small explosion shakes the ground
  BURST_STAGE_GREAT_BLAST  ///< the last and largest blast
};

/// Mind Read's wave, its tint and the palette it greys.
void FieldPsynergy_UpdateMindReadWave(void);
void FieldPsynergy_StopMindReadWave(void);
void FieldPsynergy_GreyMindReadPalette(void);
void FieldPsynergy_RestoreMindReadPalette(void);
void MindRead_AdvanceTint(void);
void MindRead_UpdateCrescent(struct Actor *actor);
void MindRead_StartRipple(void);

/// The pieces of the casts: their particles, hands and sparks, the tiles they
/// leave, and the scripts and tables they run.
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
struct Actor *FieldEffect_SpawnCarryHand(s32 x, s32 y, s32 z, s32 angle);
struct Actor *FieldEffect_SpawnLiftHand(s32 x, s32 y, s32 z, s32 angle);
s32 FieldEffect_SpawnHeavyImpact(struct Actor *source);
void FieldEffect_SpawnBurstParticleField(void);
void FieldEffect_ClearOwnedSlot(struct FieldParticle *slot);
void FieldEffect_UpdateRandomTargetParticle(struct FieldParticle *effect);
void FieldEffect_CheckCloakTile(void);
void FieldParticle_Initialize(struct FieldParticle *effect, s32 kind, s32 x,
                              s32 z);
void ParticleBurst_Update(void);
void Whirlwind_UpdateTarget(struct Actor *actor);
void Retreat_UpdateSpark(struct FieldParticle *slot);
void Teleport_PlaySparksAndFade(void);
void Teleport_PlayWhirl(void);
void TileCopies_Restore(void);
/// The caster's look while it casts, BattleEffect_SetObjectUpdateMode's mode.
enum CasterLook {
  CASTER_LOOK_NORMAL = 0, ///< its own colours
  /// Flashing white, with swelling arcs (FieldEffect_FlickerActorAndSpawnArcs).
  CASTER_LOOK_FLASHING_WHITE = 1,
  /// Cycling colours, with swelling arcs (FieldEffect_CycleActorAndSpawnArcs).
  CASTER_LOOK_CYCLING_COLORS = 2
};
void BattleEffect_SetObjectUpdateMode(struct Actor *actor, s32 mode);
void BattleEffect_DrawCanvasBlock(const u8 *src, s32 x, s32 y, s32 width,
                                  s32 height);
void BattleEffect_StartArcEffect(void);
void BattleEffect_WaitForArcGroups(void);
void BattleEffect_EndArcEffect(void);
extern const s32 gDustPuff_LongScript[];
extern const s32 gDustPuff_ShortScript[];
extern const s32 gFieldEffect_SourceHoldScript[];
extern const s32 gForce_FistScript[];
extern const u8 gMindRead_CasterColorCycle[];
extern const u8 gMoveHand_PushPoses[];

/// Three coordinates of a burst piece's place, x, y and z, filled in turn as
/// Vector_AddPolarOffset reads them.
struct BurstParticleVector {
  s32 values[3];
};

/// One part of a sprite (struct AnimationSpritePart) as Mind Read's burst
/// copies its image's attributes to its shadow: a byte at a time, under the OAM
/// attributes' names.
struct BurstParticleVisual {
  u8 unused[5];            ///< the part's link and attribute 0's y
  u8 affineModeMosaic : 5; ///< attribute 0's affine, objectMode and mosaic
  u8 fullColor : 1;        ///< 256 colours
  u8 shape : 2;            ///< 0 square, 1 wide, 2 tall
  u8 unusedAfterShape;     ///< attribute 1's x, low byte
  u8 xHighAffineIndex : 6; ///< x bit 8 and the affine matrix
  u8 size : 2;             ///< with shape, the sprite's size
  union {
    struct {
      u16 tile : 10; ///< first sprite tile in VRAM
      u16 priorityPalette : 6;
    } bits;
    struct {
      u8 tileLow;
      u8 tileHighPriority : 4;
      u8 palette : 4;
    } bytes;
  } attributes; ///< attribute 2
};

/// A sprite (struct Sprite) as Mind Read's burst reads it: its image, and its
/// shadow 28 bytes in.
struct BurstParticleVisualGroup {
  struct BurstParticleVisual primary;
  u8 unusedAfterPrimary[16]; ///< no code names them
  struct BurstParticleVisual child;
};

/// The lifted actor as Lift on a target reads it: struct Actor's y, motionFlags
/// (flag) and update (callback); read through struct Actor, the flag store
/// moves after the anchor's load.
struct EffectChild {
  u8 unused[12]; ///< no code names them
  s32 y;
  u8 unusedAfterY[69]; ///< no code names them
  u8 flag;
  u8 unusedAfterFlag[22]; ///< no code names them
  void (*callback)(void);
};

/// The Djinn: what each element's join effect shows, and settling a Djinni
/// after its battle.
void Djinn_RunVenusJoinEffect(s32 actorId);
void Djinn_RunMercuryJoinEffect(s32 actorId);
void Djinn_RunMarsJoinEffect(s32 actorId);
void Djinn_RunJupiterJoinEffect(s32 actorId);
void Djinn_ResolvePending(s32 shown);
void Djinn_RestoreActorAfterBattle(void);

/// The map screen, which the World Map's Select shows: the leader's point on
/// it and the tiles a cast has used.
void MapScreen_GetLeaderPoint(s32 *screenX, s32 *screenY);
void FieldPsynergy_MarkWorldMapTilesUsed(void);

/// Never set (EWRAM, zero from boot). Set, the map screen would list
/// only the places before the first end of the place table, not the second list
/// from entry 128 on, and show each place's second marker (the high nibble of
/// its value).
// XXX (no evidence): what the switch was for; no edition, debug room or other
// code writes it, so the game never shows it set.
extern s16 gMapScreen_FirstListOnly;

/// The places' markers' blend, a level for every two frames of a 64-frame
/// cycle.
extern const u8 gWorldMap_MarkerBlendCycle[];
/// The cursor's heading for each way the +Control Pad is held.
extern const u16 gWorldMap_CursorDirectionAngles[];

/// Where the map screen's cursor glides to on its own, 16.16; 0 when it does
/// not.
extern s32 gMapScreen_GlideTargetX;
/// As gMapScreen_GlideTargetX, the second coordinate.
extern s32 gMapScreen_GlideTargetY;
/// Nonzero while the map screen is zoomed in.
extern s16 gMapScreen_Zoomed;
/// Set while the place name waits to be drawn again.
extern s16 gMapScreen_NameRedrawPending[];
/// Frames before the place name is drawn again.
extern s16 gMapScreen_NameRedrawDelay[];
/// The map screen's cursor marker.
extern struct WorldMapMarker gMapScreen_CursorMarker;

/// The battles the events start: the encounter zones of the story's battles,
/// the Mad Plants' leap and the transition into a battle.
void Field_StartEncounterTransition(s32 unused);
void MadPlant_LeapAtLeader(s32 sprite, s32 x, s32 z);
void Field_RequestBattleFromEncounterTable(s32 zone, s32 slot);
/// Field_RequestDjinnBattle's flag: the Djinni does not hop first.
#define DJINN_NO_HOP 0x800

/// The encounter zone of the hidden Mad Plants' battles, one slot each, in
/// FLAG_MAD_PLANT's order.
#define ENCOUNTER_ZONE_MAD_PLANTS 106

/// The encounter zone of the early story battles: the Ruffian waiting for
/// backup on Venus Lighthouse's plaza (slot 0), the Ruffian on the Suhalla road
/// (1), the three Ruffians at the crevice (2) and the Punch Ant (3) at Suhalla
/// Gate, the Chestbeaters in the Kandorean Temple (4) and Echo's lesson battle
/// on the World Map (5).
#define ENCOUNTER_ZONE_SCRIPTED_EARLY_STORY 9
/// The encounter zone of two story battles: the King Scorpion's in the Yampi
/// Desert (slot 0), then Briggs and his crew's on his ship at Alhafra (slot
/// 1).
#define ENCOUNTER_ZONE_SCRIPTED_YAMPI_DESERT_ALHAFRA 10
/// The encounter zone of the battles below deck of the Lemurian Ship: slot 0
/// an Aqua Jelly, slot 1 the Aqua Hydra.
#define ENCOUNTER_ZONE_AQUA_JELLIES 11
/// The encounter zone of three story battles: the Serpent's in Gaia Rock, one
/// slot for each beam of sunlight let into its lair (ENEMY_SERPENT to
/// ENEMY_SERPENT_FOUR_BEAMS, slots 0 to 4), then Avimander's in Champa (slot
/// 5) and Poseidon's at the Sea of Time (slot 6).
#define ENCOUNTER_ZONE_SERPENT 12
/// The encounter zone of Moapa and his two Knights at the Trial Road's peak,
/// one slot per stage door the party reached first (slots 0 to 4), then Karst
/// and Agatio's three battles on Jupiter Lighthouse's aerie (slots 5 to 7).
#define ENCOUNTER_ZONE_MOAPA 13
/// The World Map's encounter zone of region 1's beach, shallows and grassland,
/// which the Psynergy test room's battles also take (slot 0, one or two Giant
/// Bats).
#define ENCOUNTER_ZONE_WORLD_MAP_BEACH_AND_GRASSLAND 14

/// The event battles: the Flame Dragons in Mars Lighthouse (slot 0), the Doom
/// Dragon on its aerie (1), the gladiators in the Shaman Village Cave (2), the
/// World Map's bandits (3) and Agatio and Karst's fight with Isaac in Jupiter
/// Lighthouse (4).
#define ENCOUNTER_ZONE_SCRIPTED 102

/// The encounter zone of the battles with four guardians of hidden places, one
/// slot each (HiddenBoss).
#define ENCOUNTER_ZONE_HIDDEN_BOSSES 103

/// The slots of ENCOUNTER_ZONE_HIDDEN_BOSSES.
enum HiddenBoss {
  HIDDEN_BOSS_TREASURE_ISLE, ///< the Star Magician, at the end of Treasure Isle
  HIDDEN_BOSS_VALUKAR,       ///< Valukar, in the Yampi Desert Cave
  HIDDEN_BOSS_SENTINEL,      ///< the Sentinel, at the end of the Islet Cave
  /// The shadow at the entrance of the Anemos Inner Sanctum.
  HIDDEN_BOSS_ANEMOS
};

/// The rest of what the event commands share: actors, cells, sound and the
/// party.
s32 Actor_GetCellIndex(struct Actor *actor);
s32 Actor_GetScreenPosition(s32 objectId, s32 *position);
/// Actor_SetLayersColorMode's value that cycles the actor's colours
/// (gMindRead_CasterColorCycle); any other is a colour mode plus one.
#define LAYERS_COLOR_CYCLING 0
void Actor_SetLayersColorMode(void *actor, s32 value);
void Actor_SetOthersPaused(struct Actor *excludedObject, s32 groupMode,
                           s32 action);
void Camera_WorldToScreen(struct Vector3 *position);
s32 MapCollision_FindOrAddTag(s32 unused, s32 key, s32 value);
void *Sprite_ShareVramCacheEntry(void *sprite, void *last);
void Scene_QueueMusic(s32 soundId);
void Weather_UpdateLightning(void);
void SummonTablet_PlayRunes(s32 targetId);
void Party_LearnAbility(s32 unit, s32 ability);
void Party_RemoveMemberRestored(s32 owner);

/// Actor_JumpDownLedge's mode bit that skips the wait for the landing.
#define LEDGE_JUMP_NO_LANDING_WAIT (1 << 12)
void Actor_JumpDownLedge(s32 id, s32 mode, s32 particle);

/// What Input_GetDpadAngleEightWay returns when no direction is held,
/// or right and left or up and down together: the blank entries of
/// gInput_DpadAnglesEightWay.
#define DPAD_ANGLE_NONE 65535

#endif
