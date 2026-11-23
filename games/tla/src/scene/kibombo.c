/// @file
/// Kibombo, the warriors' village below the Gabomba Statue, in the six rooms of
/// this overlay: the village, its houses, shops, inn and Sanctum
/// (Kibombo_OpenWeaponShop, Kibombo_OpenInn), and the grounds before the
/// statue, each by day and on the ceremony night.
///
/// The party arrives on the night of the ceremony. The crowd fills the way
/// north, so it ties the rope in the village's south west corner with Lash and
/// jumps along the painted poles (Kibombo_JumpForward) to the grounds. There
/// Akafubu offers the Black Orb to the Great Gabomba (Kibombo_CeremonyStarts to
/// Kibombo_CeremonyEnds), and Piers, come to take his orb back, joins the
/// party once Move drops a pillar for him (Kibombo_PiersJoinsParty). The path
/// east of the statue ends at four marks on the ground; digging them shows the
/// party that the patch it passed covers a vent into the statue
/// (Kibombo_DeadEndByTheStatue, Kibombo_ScoopVent). The villagers' talk
/// changes once Akafubu is the witch doctor (Kibombo_TalkPathToPeace).

#include "graphics/palette_buffers.h"
#include "system/task.h"
#include "flags.h"
#include "item_ids.h"
#include "field/sprite.h"
#include "types.h"
#include "macros.h"
#include "field/field_sequence_event.h"
#include "gamepak/far_call/system.h"
#include "field/event_commands.h"
#include "scene/kibombo.h"
#include "graphics/resource.h"
#include "battle/battle.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "field/ground_mask.h"
#include "field/field_psynergy_work.h"
#include "game/game.h"
#include "graphics/glyph.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "scene_ids.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "system/io_reg.h"
#include "scene/scene.h"
#include "field/field_effect.h"
#include "field/field_sprite.h"
#include "system/vector3.h"
#include "system/input.h"
#include "version.h"
#include "fixed_math.h"
#include "text/msg_ids.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "system/frame.h"
#include "system/system_call.h"

void GameFlag_Set(s32 flag);
void GameFlag_Clear(s32 flag);
void Actor_SetPositionAndResetMotion(struct Actor *actor, FIXED x, FIXED y,
                                     FIXED z);
void Actor_SetSpriteColor(struct Actor *actor, s32 value);
void Actor_FaceActorById(s32 actor, s32 target, s32 frames);
void Actor_FaceDirectionById(s32 actor, s32 angle, s32 frames);

void MapGrid_CopyCellPictureIndicesRect(s32 a, s32 b, s32 c, s32 d, s32 e,
                                        s32 f);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Event_ShowMessageThenWait(s32 actor, s32 a, s32 b);
void Event_ShowMessageAndWait(s32 speaker, s32 flags);
void Actor_FaceDirectionAndRefreshById(s32 actor, s32 angle);
void Actor_DisableById(s32 actor);
void FlagBlock_ApplyFlags(const void *script);
void Shop_Run(s32 shop, s32 keeper);
void Sanctum_Run(s32 keeper);
void Inn_CheckIn(s32 shop, s32 keeper);
void Sound_Play(s32 cue);
struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Actor_Release(struct Actor *actor);
void Actor_SetSpriteFlags(struct Actor *actor, u32 value);
void Actor_SetAnimationById(u32 actorId, s32 animation);
void FieldPsynergy_RunHandler(s32 type);
void FieldCamera_WaitForArrival(void);
void Import_EventSetExitRequest(s32 countdown);
void FieldPsynergy_LoadResources(s32 a, s32 b);
void BattleEffect_StopMainActor(void);

void Field_TryJumpForward(void);
void FieldEffect_StartBlankOverlay(s32 a);
void Rope_Initialize(void);
void Rope_Place(s32 a, s32 b, s32 c, s32 d);
void FlagBlock_PlaceAll(const void *script);
void Actor_SetMoveTarget(struct Actor *actor, FIXED x, FIXED y, FIXED z);
void Actor_WaitForMove(struct Actor *actor);

void FieldCamera_MoveWithinMapBounds(FIXED x, FIXED y, FIXED z, s32 mode);
struct Actor *FieldCamera_GetActor(void);
void Camera_SetSpeedParameters(FIXED speed, FIXED acceleration);
void Actor_SetSpeedParameters(s32 actorId, FIXED speed, FIXED acceleration);
void Actor_WalkToAndWaitById(s32 actorId, s32 x, s32 z);
void Actor_PlaceOnActor(s32 actorId, s32 target);
void Actor_SetMovementTypeById(s32 actorId, const void *callback);
void Actor_SetMovementTypeAndWaitById(s32 actorId, const void *callback);
s32 Event_AskYesNoWithActorAnimation(s32 actorId, s32 flags);
void FieldEffect_SetAttached(s32 actorId, s32 emote);
void Actor_Jump(s32 actorId, s32 speed, s32 frames);
void Actor_SetAnimationAndWait(s32 actorId, s32 mode);
void Actor_WaitForMoveById(s32 actorId);
void Event_AddPartyMember(s32 actorId, s32 value);

void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 actorId, s32 x, s32 z,
                                                    s32 angle);

void Actor_WalkToById(s32 actorId, s32 x, s32 z);

void PaletteFade_SetTarget(s32 a, s32 b);

void Kibombo_InitializeVillage(void);
s32 Kibombo_ClampColor(s32 value);
void Kibombo_FlashSprite(struct Actor *actor);
void Kibombo_UncoverNorthMark(void);
void Kibombo_UncoverSouthMark(void);
void Kibombo_UncoverWestMark(void);
void Kibombo_UncoverEastMark(void);
void Kibombo_PiersHasForgottenParty(void);
void Kibombo_PiersTakesPartyForEnemies(void);
void Kibombo_AkafubuOffersCrystal(void);
void Kibombo_PiersJoinsParty(void);
void Kibombo_FlickerColor(s32 index, s32 entry, s32 delta);
void Kibombo_FlickerRedColor(s32 index, s32 entry, s32 delta);
void Kibombo_FlickerVillageFires(void);
void Kibombo_FlickerCeremonyFires(void);
void Kibombo_FlickerStatueFires(void);
void Kibombo_RaiseEmbers(void);
void Kibombo_RaiseSmoke(void);
void Kibombo_LightSignalFire(void);
void Actor_Hide(s32 id);
void Kibombo_RestoreBlocks(void);
void Kibombo_AkafubuChants(void);
void Kibombo_SetStatueMouth(s32 open, s32 glow);
void Kibombo_SetStatueTongue(s32 open, s32 mode);
void Kibombo_StopFireFlicker(void);
void Kibombo_ClimbDownLadder(s32 x, s32 z);
void Kibombo_OpenCeremonyScene(void);
void Kibombo_CrowdCheers(void);

extern s32 gKibombo_EnterDoorScript[];
extern u32 gKibombo_KradenMeetsPiersScript[];
extern u32 gKibombo_JennaMeetsPiersScript[];
extern s32 gKibombo_ShebaMeetsPiersScript[];
extern u32 gKibombo_PartyGathersScript[];
extern s32 gKibombo_EyesShakeScript[];
extern s32 gKibombo_EyesPopScript[];
extern s16 gKibombo_FrozenPuddle[];
u8 gKibombo_UnusedBeforeFireColors[4] BSS_DATA;
s16 gKibombo_FireColors[4] BSS_DATA;
struct Actor *gKibombo_BlackCrystal BSS_DATA;

/// A background colour's place in BG_PLTT: colour `color` of 16-colour palette
/// `palette`, as the fires' flicker indexes the colours it sets.
#define BG_PLTT_INDEX(palette, color) ((palette) * PALETTE_COLORS + (color))

/// The fires' colours in the background palettes, which each room keeps in
/// gKibombo_FireColors as it opens and its flicker task brightens and dims:
/// four in the night village, four on the night grounds, the first the same,
/// and three before the statue's face.
enum KibomboFireColor {
  FIRE_COLOR_FIRST = BG_PLTT_INDEX(0, 9),
  VILLAGE_FIRE_COLOR_SECOND = BG_PLTT_INDEX(5, 7),
  VILLAGE_FIRE_COLOR_THIRD = BG_PLTT_INDEX(6, 7),
  VILLAGE_FIRE_COLOR_FOURTH = BG_PLTT_INDEX(7, 7),
  GROUNDS_FIRE_COLOR_SECOND = BG_PLTT_INDEX(0, 7),
  GROUNDS_FIRE_COLOR_THIRD = BG_PLTT_INDEX(0, 6),
  GROUNDS_FIRE_COLOR_FOURTH = BG_PLTT_INDEX(4, 12),
  STATUE_FIRE_COLOR_FIRST = BG_PLTT_INDEX(10, 9),
  STATUE_FIRE_COLOR_SECOND = BG_PLTT_INDEX(10, 7),
  STATUE_FIRE_COLOR_THIRD = BG_PLTT_INDEX(10, 6)
};

/// Set once the party has found the dead end by the statue and turned back
/// (Kibombo_DeadEndByTheStatue).
#define FLAG_KIBOMBO_DEAD_END_SEEN 0x8f5
/// Set while Piers goes about Kibombo with the party: as he joins it, and as he
/// thanks it; cleared when he stays behind.
#define FLAG_KIBOMBO_PIERS_WITH_PARTY 0x92e
/// Set once Piers has refused to leave the village without the orb and stayed
/// behind (Kibombo_PiersRefusesToLeave).
#define FLAG_KIBOMBO_PIERS_STAYED_BEHIND 0x92f

/// Story flags: the vent Scoop uncovers by the statue, the pillar Moved for
/// Piers, the ceremony begun and Piers joined.
#define FLAG_KIBOMBO_VENT_UNCOVERED 0x86a
#define FLAG_KIBOMBO_PILLAR_MOVED_FOR_PIERS 0x9ea
#define FLAG_KIBOMBO_CEREMONY_BEGUN 0x8f1
#define FLAG_PIERS_JOINED 0x8f4
/// Set once the party has heard of the foreign ceremony.
#define FLAG_HEARD_OF_THE_FOREIGN_CEREMONY 0x9df
/// Set once Lash has tied the village's rope across.
#define FLAG_KIBOMBO_VILLAGE_ROPE_TIED 0x9e9
/// Set once the party has been to the statue's grounds on the ceremony night.
#define FLAG_SAW_THE_GROUNDS_CEREMONY 0x869
/// Set as the party climbs down the ladder from Akafubu's home: he stands
/// apart, turns to whoever talks to him and speaks, as his father does, of
/// the days after the ceremony.
#define FLAG_KIBOMBO_LEFT_HOME_OF_AKAFUBU 0x8ee
/// For this visit: each of the four marks at the dead end by the statue
/// uncovered, and the one Scoop is uncovering.
#define FLAG_NORTH_MARK_UNCOVERED FLAG_SCENE_TEMP_04
#define FLAG_SOUTH_MARK_UNCOVERED FLAG_SCENE_TEMP_05
#define FLAG_WEST_MARK_UNCOVERED FLAG_SCENE_TEMP_06
#define FLAG_EAST_MARK_UNCOVERED FLAG_SCENE_TEMP_07
#define FLAG_SCOOPING_NORTH_MARK FLAG_SCENE_TEMP_20
#define FLAG_SCOOPING_SOUTH_MARK FLAG_SCENE_TEMP_21
#define FLAG_SCOOPING_WEST_MARK FLAG_SCENE_TEMP_22
#define FLAG_SCOOPING_EAST_MARK FLAG_SCENE_TEMP_23
/// For this visit: the room's unseen blocks drawn (Kibombo_RestoreBlocks).
#define FLAG_KIBOMBO_BLOCKS_SHOWN FLAG_SCENE_TEMP_00
/// For this visit: the puddle frozen into a pillar (gKibombo_FrozenPuddle). On
/// the grounds by day it is set until the ceremony night has been seen, so
/// Frost leaves the puddle alone.
#define FLAG_KIBOMBO_PUDDLE_FROZEN FLAG_SCENE_TEMP_01
/// For this visit: the room's fires flicker, and a way out stops them.
#define FLAG_KIBOMBO_FIRES_FLICKER FLAG_SCENE_TEMP_02
/// For this visit: the village's doors are shut on the ceremony night.
#define FLAG_KIBOMBO_DOORS_SHUT FLAG_SCENE_TEMP_09

/// The puddle Frost freezes, the first row of the village's and the grounds'
/// placements.
enum { ACTOR_PUDDLE = 8 };

/// The grounds before the statue (SceneId_KibomboStatueGrounds and, on the
/// ceremony night, SceneId_KibomboStatueGroundsCeremony), numbering the rows of
/// their placements; the night's rows place the warriors and Akafubu's party
/// after them, and Kraden after those.
enum KibomboStatueGroundsActorId {
  /// The stone pillar Move pushes for Piers (Kibombo_MovePillarForPiers).
  ACTOR_GROUNDS_PILLAR = 11,
  /// The Great Gabomba's two eyes, which light up as its mouth opens
  /// (Kibombo_SetStatueMouth), and its tongue (Kibombo_SetStatueTongue).
  ACTOR_STATUE_LEFT_EYE = 12,
  ACTOR_STATUE_RIGHT_EYE = 13,
  ACTOR_STATUE_TONGUE = 14,
  /// Two unseen blocks (SPRITE_UNSEEN_BLOCK, black squares drawn
  /// transparent), hidden as the room opens (Kibombo_HideGroundsBlocks).
  ACTOR_GROUNDS_FIRST_BLOCK = 15,
  ACTOR_GROUNDS_SECOND_BLOCK = 16,
  /// On the ceremony night: Akafubu, "Let us now present our jewel to the
  /// Great Gabomba!", and the eight warriors in a ring before him.
  ACTOR_AKAFUBU = 17,
  ACTOR_WARRIOR_A = 18,
  ACTOR_WARRIOR_B = 19,
  ACTOR_WARRIOR_C = 20,
  ACTOR_WARRIOR_D = 21,
  ACTOR_WARRIOR_E = 22,
  ACTOR_WARRIOR_F = 23,
  ACTOR_WARRIOR_G = 24,
  ACTOR_WARRIOR_H = 25,
  ACTOR_KRADEN = 26,
  /// An unseen marker the Black Crystal rides on as Akafubu raises it
  /// (Kibombo_CarryBlackCrystal); Piers's, Jenna's and Sheba's rows take
  /// their own numbers, so it follows Kraden.
  ACTOR_CRYSTAL_MARKER = 27
};

/// The village by day and on the ceremony night: after the puddle, the two
/// candlesticks by a house's door, the flame (by day the cooking fire's, at
/// night burning on the totem pole in the square) and the rope's two posts. By
/// day a woman remarks on the warriors, two warriors keep the gate and the
/// signal fire burns; at night villagers crowd the square, an unseen block
/// stands at a house's door and six candlesticks burn.
enum {
  ACTOR_CANDLESTICK_A = 9,
  ACTOR_CANDLESTICK_B,
  ACTOR_FLAME,
  ACTOR_ROPE_POST_A,
  ACTOR_ROPE_POST_B,
  /// By day, the villagers, each named for what they say: a small child,
  /// "Akafubu has finally completed his ceremony...", and the woman, "It looks
  /// like there are fewer warriors in Kibombo now, huh?"
  ACTOR_CHILD_GLAD_OF_THE_CALM,
  ACTOR_WOMAN_NOTING_FEWER_WARRIORS,
  /// "The Great Gabomba's mouth will remain open until Akafubu has earned the
  /// gift of the Gabomba."
  ACTOR_MAN_ON_THE_OPEN_MOUTH,
  /// "Now that the ceremony is over, I feel a touch depressed."
  ACTOR_WOMAN_MISSING_THE_CEREMONY,
  /// "I couldn't for the life of me figure out how our warriors got to Indra
  /// without a boat."
  ACTOR_OLD_MAN_PUZZLED_BY_INDRA,
  /// "Oh... I smell something delectable."
  ACTOR_OLD_WOMAN_SMELLING_MEAT,
  /// "Good ol' meat... I love meat."
  ACTOR_BOY_WHO_LOVES_MEAT,
  /// "We get to eat normal animals again!"
  ACTOR_GIRL_TIRED_OF_MONSTERS,
  ACTOR_GATE_GUARD_A = 22,
  ACTOR_GATE_GUARD_B,
  ACTOR_SIGNAL_FIRE = 25,
  ACTOR_DOOR_BLOCK = 29,
  ACTOR_LIT_CANDLESTICK_A,
  ACTOR_LIT_CANDLESTICK_B,
  ACTOR_LIT_CANDLESTICK_C,
  ACTOR_LIT_CANDLESTICK_D,
  ACTOR_LIT_CANDLESTICK_E,
  ACTOR_LIT_CANDLESTICK_F
};

/// The houses, inn and shops (SceneId_KibomboHouses).
enum {
  /// Akafubu at home after the ceremony: "I'm going to be the one to get the
  /// Great Gabomba's magic! You'll see!"
  ACTOR_AKAFUBU_AT_HOME = 8,
  /// "My son has been so kind and compassionate since the ceremony."
  ACTOR_FATHER_OF_AKAFUBU = 9,
  ACTOR_KRADEN_AT_HOME = 10,
  /// "Are you going to head to western Gondowan?"
  /// The other villagers at home, each named for what they say: "I have heard
  /// that if you head west from southern Gondowan, you'll see the Eastern
  /// Sea."
  ACTOR_CHILD_ON_THE_EASTERN_SEA = 11,
  ACTOR_WOMAN_ASKING_ABOUT_THE_WEST,
  /// "You can see Atteka from the bridge in western Gondowan."
  ACTOR_BOY_WHO_HEARD_OF_ATTEKA,
  /// "If you are looking for treasure, look west of the river."
  ACTOR_MAN_ON_TREASURE_WEST,
  /// "Each year, when the rains stop, our warriors look for treasure in the
  /// belly of the continent."
  ACTOR_WOMAN_ON_THE_TREASURE_HUNTS,
  /// The old man lying in bed, his back aching since the ceremony.
  ACTOR_MAN_WITH_ACHING_BACK,
  /// "As soon as Akafubu became our recognized witch doctor, he became so much
  /// nicer."
  ACTOR_OLD_WOMAN_ON_A_KINDER_AKAFUBU,
  /// Two warriors at home: "Many in Gondowan fear the might of the Kibombo
  /// warriors." and "We think it is time to inform other villages..."
  ACTOR_WARRIOR_NOT_TO_BE_FEARED,
  ACTOR_WARRIOR_TO_TELL_THE_VILLAGES,
  ACTOR_WEAPON_SHOPKEEPER,
  ACTOR_ARMOR_SHOPKEEPER,
  ACTOR_ITEM_SHOPKEEPER,
  ACTOR_INNKEEPER,
  /// "I can't remember how long it's been since we've had any guests."
  ACTOR_INN_WORKER,
  ACTOR_HEALER,
  /// Two unseen blocks in the inn's back doorway, hidden as the room opens
  /// (Kibombo_HideInnBlocks).
  ACTOR_INN_FIRST_BLOCK,
  ACTOR_INN_SECOND_BLOCK,
  /// An unseen marker on the bed where the man with the aching back lies,
  /// which answers for him from that side.
  ACTOR_ACHING_BACK_BEDSIDE
};

/// The village by day: "This is a signal fire. We use these to send messages
/// to neighboring..."
enum { ACTOR_SIGNAL_FIRE_KEEPER = 24 };

/// The village on the ceremony night: the crowd in the square, each named for
/// what they say before Akafubu has asked the party's help; some move or leave
/// once he has. Last, the healer who talks of the foreign ceremony.
enum {
  /// "During the last ceremony, Akafubu made the jewel float in the air..."
  ACTOR_MAN_WHO_SAW_THE_JEWEL_FLOAT = 14,
  /// "Akafubu looks really confident tonight."
  ACTOR_MAN_SURE_OF_AKAFUBU,
  /// "Akafubu is attempting the ceremony again tonight. Oeia performed the
  /// same..."
  ACTOR_OLD_MAN_RECALLING_OEIA,
  /// "Is making the jewel float with the power of his mind all there is to
  /// it?"
  ACTOR_WOMAN_DOUBTING_THE_JEWEL,
  /// "Akafubu has tried this before, but he has always failed."
  ACTOR_WOMAN_RECALLING_HIS_FAILURES,
  /// "Akafubu blames his failures on the jewel he originally used."
  ACTOR_OLD_WOMAN_ON_THE_JEWEL,
  /// "Akafubu's ceremony is set to begin soon."
  ACTOR_BOY_KEPT_AWAKE,
  ACTOR_GIRL_WHO_HOPES,
  /// "When the Gabomba is angered, his eyes glow with rage and fire shoots
  /// out of his mouth."
  ACTOR_WOMAN_WARNING_OF_ANGER,
  /// "He who is the witch doctor of Kibombo holds the most revered office in
  /// our whole town."
  ACTOR_OLD_MAN_ON_THE_OFFICE,
  /// "If the Great Gabomba's mouth doesn't open, Akafubu must not yet be a
  /// true witch doctor."
  ACTOR_OLD_WOMAN_WATCHING_THE_MOUTH,
  /// "If the Great Gabomba likes the jewel, his mouth will open and his tongue
  /// will stick out."
  ACTOR_MAN_WHO_KNOWS_THE_SIGN,
  ACTOR_WOMAN_NO_PUSHING = 26,
  ACTOR_AFRAID_MAN,
  ACTOR_MAN_WHO_KNEW_OEIA,
  ACTOR_HEALER_ON_THE_CEREMONY = 36
};

/// The village on the ceremony night: where the party starts again, before the
/// healer, after a defeat (gParty_PairResolveRules).
enum { ENTRANCE_CEREMONY_AFTER_DEFEAT = 19 };

/// The rooms' own cell behaviours; where a cell leads out of a room its
/// behaviour is the exit's number, named in scene_exits.h.
enum KibomboCellBehavior {
  /// The village: the ring of cells (19, 7) to (21, 10) among the painted poles
  /// in its north, and the two cells inside it, (20, 8) and (20, 9).
  CELL_BEHAVIOR_MIDDLE_OF_POLE_RING = 15,
  CELL_BEHAVIOR_POLE_RING,
  /// The village: the jump north from cell (19, 8) among the poles, and by day
  /// the jump west from the ledge by the puddle, cell (25, 27).
  CELL_BEHAVIOR_JUMP_NORTH_AMONG_POLES = 20,
  CELL_BEHAVIOR_JUMP_WEST_BY_PUDDLE,
  /// The statue's grounds: the patch over the vent, cell (23, 18), and the
  /// four marks at the dead end, north (16, 7), south (16, 11), west (14, 9)
  /// and east (18, 9).
  CELL_BEHAVIOR_VENT_PATCH = 10,
  CELL_BEHAVIOR_NORTH_MARK,
  CELL_BEHAVIOR_SOUTH_MARK,
  CELL_BEHAVIOR_WEST_MARK,
  CELL_BEHAVIOR_EAST_MARK,
  /// The grounds on the ceremony night: the ledge where Piers stands, cells
  /// (3, 25) to (3, 26); the way up to the dead end, (21, 14) and (22, 14);
  /// and the top of the grounds beside Piers once he has stayed behind,
  /// (16, 5).
  CELL_BEHAVIOR_BY_PIERS = 20,
  CELL_BEHAVIOR_WAY_TO_DEAD_END,
  CELL_BEHAVIOR_BY_PIERS_WAITING,
  /// The houses: the cells beside the three ladders down, each the number of
  /// the exit it leads by: in the item shop's upper room, from Akafubu's home
  /// to the Sanctum, and in the upper room with two beds.
  CELL_BEHAVIOR_ITEM_SHOP_LADDER = 7,
  CELL_BEHAVIOR_AKAFUBUS_LADDER = 9,
  CELL_BEHAVIOR_TWO_BEDS_LADDER = 13,
  /// The Sanctum: the counter before the healer, cell (23, 22).
  CELL_BEHAVIOR_SANCTUM_COUNTER = 24
};

struct Actor;

/// An actor's update: turns its sprite a little each frame.
s32 Kibombo_SpinSprite(struct Actor *actor) {
  ((struct FieldSprite *)actor->sprite)->rotation += TRIG_TURN * 3 / 128;
  return 0;
}

/// An actor's update: keeps its position as the center Kibombo_ShakeInPlace
/// shakes it about.
s32 Kibombo_KeepShakeCenter(struct Actor *actor) {
  actor->bounce = actor->x.fixed;
  actor->gravity = actor->y.fixed;
  return 0;
}

extern s16 gKibombo_HousesExitRegions[];

/// An actor's update: every other frame, puts it up to a pixel either way of
/// its kept center.
s32 Kibombo_ShakeInPlace(struct Actor *actor) {
  if (gFrameCount & 1) {
    FIXED dx;
    FIXED dy;

    dx = -toFIXED(1);
    dy = dx;
    dx += toFIXED(Random_Below(3));
    dy += toFIXED(Random_Below(3));

    Actor_SetPositionAndResetMotion(actor, actor->bounce + dx,
                                    actor->gravity + dy, actor->z.fixed);
  }
  return 1;
}

/// An actor's update: flashes its sprite's colour on and off every two frames.
void Kibombo_FlashSprite(struct Actor *actor) {
  if (gFrameCount & 2)
    Actor_SetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
  else
    Actor_SetSpriteColor(actor, SPRITE_COLOR_NORMAL);
}

/// An actor's update: every ten to twenty frames, faces a random way.
s32 Kibombo_FaceRandomWay(struct Actor *actor) {
  if (actor->riseCounter != 0) {
    actor->riseCounter--;
  } else {
    actor->riseCounter = (Random_Below(10)) + 10;
    actor->facing = Random16();
  }
  return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

/// The houses have exit regions of their own; the other rooms have none.
u8 *Scene_GetExitRegions(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_KIBOMBO_HOUSES)
    return gKibombo_HousesExitRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gKibombo_VillageActorPlacements[];
extern struct ActorPlacement gKibombo_CeremonyActorPlacements[];
extern struct ActorPlacement gKibombo_StatueGroundsActorPlacements[];
extern struct ActorPlacement gKibombo_StatueGroundsCeremonyActorPlacements[];
extern struct ActorPlacement gKibombo_HousesActorPlacements[];
extern struct ActorPlacement gKibombo_ActorPlacements[];
extern struct Trigger gKibombo_VillageTriggers[];
extern struct Trigger gKibombo_CeremonyTriggers[];
extern struct Trigger gKibombo_StatueGroundsTriggers[];
extern struct Trigger gKibombo_StatueGroundsCeremonyTriggers[];
extern struct Trigger gKibombo_HousesTriggers[];
extern struct Trigger gKibombo_Triggers[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_KIBOMBO)
    return gKibombo_VillageActorPlacements;
  if (scene == SCENE_KIBOMBO_CEREMONY)
    return gKibombo_CeremonyActorPlacements;
  if (scene == SCENE_KIBOMBO_STATUE_GROUNDS)
    return gKibombo_StatueGroundsActorPlacements;
  if (scene == SCENE_KIBOMBO_STATUE_GROUNDS_CEREMONY)
    return gKibombo_StatueGroundsCeremonyActorPlacements;
  if (scene == SCENE_KIBOMBO_HOUSES) {
    if (GameFlag_Get(FLAG_KIBOMBO_LEFT_HOME_OF_AKAFUBU))
      gKibombo_HousesActorPlacements[0].talkFacing = TALK_FACING_PARTY;
    return gKibombo_HousesActorPlacements;
  }
  return gKibombo_ActorPlacements;
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_KIBOMBO)
    return gKibombo_VillageTriggers;
  if (scene == SCENE_KIBOMBO_CEREMONY)
    return gKibombo_CeremonyTriggers;
  if (scene == SCENE_KIBOMBO_STATUE_GROUNDS)
    return gKibombo_StatueGroundsTriggers;
  if (scene == SCENE_KIBOMBO_STATUE_GROUNDS_CEREMONY)
    return gKibombo_StatueGroundsCeremonyTriggers;
  if (scene == SCENE_KIBOMBO_HOUSES)
    return gKibombo_HousesTriggers;
  return gKibombo_Triggers;
}

/// A woman asks whether the party has noticed there are fewer warriors.
void Kibombo_AskAboutFewerWarriors(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgItLooksLikeThere);
  // "It looks like there are fewer warriors in Kibombo now, huh?"
  Event_AskYesNoAndShowResponse(ACTOR_WOMAN_NOTING_FEWER_WARRIORS, 0);
  Event_End();
}

/// Talking to the west gate guard: the guards ask whether the party has come
/// for revenge for the raid, the east guard speaking.
void Kibombo_AskAboutRevenge(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgYouDidnT);
  // "You didn't come to Kibombo to get revenge on us for that raid, did you?"
  Event_AskYesNoAndShowResponse(ACTOR_GATE_GUARD_B, 0);
  Event_End();
}

/// The signal fire's keeper tells of the path to peace under Akafubu, and of
/// the signal fire.
void Kibombo_TalkPathToPeace(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgNowThatAkafubu);
  // "Now that Akafubu is our new witch doctor, we shall embark on a path..."
  Event_ShowMessageAndWait(ACTOR_SIGNAL_FIRE_KEEPER, 0);
  Actor_FaceActorById(ACTOR_SIGNAL_FIRE_KEEPER, gMainRoutineWork.leaderActorId,
                      0);
  Event_Wait(20);
  // "This is a signal fire. We use these to send messages to neighboring..."
  Event_ShowMessageAndWait(ACTOR_SIGNAL_FIRE_KEEPER, 0);
  Actor_FaceDirectionAndWait(ACTOR_SIGNAL_FIRE_KEEPER, FACING_SOUTH_SOUTHEAST);
  Event_End();
}

/// A woman asks whether the party is heading to western Gondowan.
void Kibombo_AskGoingWest(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAreYouGoing);
  // "Are you going to head to western Gondowan?"
  Event_AskYesNoAndShowResponse(ACTOR_WOMAN_ASKING_ABOUT_THE_WEST, 0);
  Event_End();
}

#include "shared/leader_is_facing_north.h"

/// Kibombo's shops, as Shop_Run numbers them.
enum {
  SHOP_KIBOMBO_WEAPONS = 17,
  SHOP_KIBOMBO_ARMOR = 18,
  SHOP_KIBOMBO_ITEMS = 19
};

/// Kibombo's inn, as Inn_CheckIn numbers the inns.
enum { KIBOMBO_INN = 6 };

/// Talking to the weapon shopkeeper: across the counter, the shop; from
/// elsewhere, a word about Kibombo's weapons.
void Kibombo_OpenWeaponShop(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(SHOP_KIBOMBO_WEAPONS, ACTOR_WEAPON_SHOPKEEPER);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgTheWeaponsI);
    // "The weapons I sell in this shop are ones we do not use in Kibombo...."
    Event_ShowMessageAndWait(ACTOR_WEAPON_SHOPKEEPER, 0);
    Event_End();
  }
}

/// Talking to the armor shopkeeper: across the counter, the shop; from
/// elsewhere, a word about shields and war paint.
void Kibombo_OpenArmorShop(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(SHOP_KIBOMBO_ARMOR, ACTOR_ARMOR_SHOPKEEPER);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgTheOnlyDefenses);
    // "The only defenses we Kibombo need are our shields and our war paint...."
    Event_ShowMessageAndWait(ACTOR_ARMOR_SHOPKEEPER, 0);
    Event_End();
  }
}

/// Talking to the item shopkeeper: across the counter, the shop; from
/// elsewhere, a word about the spearhead poison.
void Kibombo_OpenItemShop(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(SHOP_KIBOMBO_ITEMS, ACTOR_ITEM_SHOPKEEPER);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgIMSorryBut);
    // "I'm sorry, but the Kibombo spearhead poison is a tribal secret, used..."
    Event_ShowMessageAndWait(ACTOR_ITEM_SHOPKEEPER, 0);
    Event_End();
  }
}

/// Talking to the innkeeper: across the counter, the inn; from elsewhere, a
/// word about the beds.
void Kibombo_OpenInn(void) {
  if (Leader_IsFacingNorth()) {
    Inn_CheckIn(KIBOMBO_INN, ACTOR_INNKEEPER);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDonTWorryAbout);
    // "Don't worry about the beds here... We used to have insect problems..."
    Event_ShowMessageAndWait(ACTOR_INNKEEPER, 0);
    Event_End();
  }
}

/// Talking to the healer, or checking the counter before her, cell (23, 22):
/// across the counter, the Sanctum; from elsewhere, a word about the peace the
/// ceremony brought.
void Kibombo_OpenSanctum(void) {
  if (Leader_IsFacingNorth()) {
    Sanctum_Run(ACTOR_HEALER);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgKibomboSPeace);
    // "Kibombo's peace is restored now that the ceremony has reached its..."
    Event_ShowMessageAndWait(ACTOR_HEALER, 0);
    Event_End();
  }
}

/// The healer on the ceremony night: the first time, she muses on foreign
/// ceremonies; then she turns to the party and runs her Sanctum.
void Kibombo_TalkForeignCeremony(void) {
  if (Event_GetFlag(FLAG_HEARD_OF_THE_FOREIGN_CEREMONY)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_FaceActorById(ACTOR_HEALER_ON_THE_CEREMONY,
                        gMainRoutineWork.leaderActorId, 0);
    Event_Wait(10);
    Sanctum_Run(ACTOR_HEALER_ON_THE_CEREMONY);
    Actor_FaceDirection(ACTOR_HEALER_ON_THE_CEREMONY, FACING_NORTH_NORTHWEST,
                        0);
    Event_End();
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgOhOhMy);
    // "Oh... Oh my... I've heard of ceremonies like these... Foreign..."
    Event_ShowMessageThenWait(ACTOR_HEALER_ON_THE_CEREMONY, 0, 40);
    // "Hmm?"
    Event_ShowMessageAndWait(ACTOR_HEALER_ON_THE_CEREMONY, 0);
    Actor_FaceActorById(ACTOR_HEALER_ON_THE_CEREMONY,
                        gMainRoutineWork.leaderActorId, 0);
    Event_Wait(10);
    Sanctum_Run(ACTOR_HEALER_ON_THE_CEREMONY);
    Actor_FaceDirection(ACTOR_HEALER_ON_THE_CEREMONY, FACING_NORTH_NORTHWEST,
                        0);
    GameFlag_Set(FLAG_HEARD_OF_THE_FOREIGN_CEREMONY);
    Event_End();
  }
}

/// The man whose back has ached since the ceremony.
void Kibombo_TalkAchingBack(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgOhMyBack);
  // "Oh, my back... It's been aching ever since that ceremony. I guess I..."
  Event_ShowMessageAndWait(ACTOR_MAN_WITH_ACHING_BACK, 0);
  Event_End();
}

/// Mind Read on the man with the aching back.
void Kibombo_ReadMindNeverAgain(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgISwearThat);
  // "I swear, that's the last time I'll ever let loose like that again!"
  Event_ShowMessageAndWait(ACTOR_MAN_WITH_ACHING_BACK, 0);
  Event_End();
}

/// Draws (show 1) or stops drawing the room's two unseen blocks: on the
/// statue's grounds by its paths, in the houses in the inn's back doorway.
void Kibombo_SetBlocksShown(s32 show) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_KIBOMBO_STATUE_GROUNDS ||
      scene == SCENE_KIBOMBO_STATUE_GROUNDS_CEREMONY)
    Actors_SetDrawn(show, ACTOR_GROUNDS_FIRST_BLOCK, 2);
  else if (scene == SCENE_KIBOMBO_HOUSES)
    Actors_SetDrawn(show, ACTOR_INN_FIRST_BLOCK, 2);
}

/// Unused: draws the room's two unseen blocks and notes them shown.
UNUSED void Kibombo_ShowBlocks(void) {
  Kibombo_SetBlocksShown(1);
  GameFlag_Set(FLAG_KIBOMBO_BLOCKS_SHOWN);
}

/// Stops drawing the room's two unseen blocks.
void Kibombo_HideBlocks(void) {
  Kibombo_SetBlocksShown(0);
  GameFlag_Clear(FLAG_KIBOMBO_BLOCKS_SHOWN);
}

#include "shared/actors_set_hidden.c"

/// Felix casts Frost on the puddle: it freezes into a pillar of ice to jump
/// onto.
void Kibombo_FreezePuddle(void) { FlagBlock_ApplyFlags(gKibombo_FrozenPuddle); }

/// Scoop runs its event every frame of the dig, passing this stage on the
/// frame the ground opens.
enum { SCOOP_STAGE_DUG = 1 };

/// Shows the vent by the statue uncovered, cell (23, 18), and lets the party
/// climb down into it.
void Kibombo_UncoverVent(void) {
  MapGrid_CopyCellPictureIndicesRect(34, 33, 23, 18, 1, 1);
  MapGrid_CopyCellAttributes(34, 33, 1, 1, 23, 18);
  GameFlag_Set(FLAG_KIBOMBO_VENT_UNCOVERED);
}

/// Felix casts Scoop on the patch of ground on the statue's grounds, cell
/// (23, 18), which the party passed on the way to the dead end: as the dig
/// opens the ground, the vent under it is uncovered, the way into the Gabomba
/// Statue.
void Kibombo_ScoopVent(s32 stage) {
  if (stage == SCOOP_STAGE_DUG)
    Kibombo_UncoverVent();
}

/// Uncovers the north one of the four marks at the dead end by the statue,
/// cell (16, 7), as Scoop does.
void Kibombo_UncoverNorthMark(void) {
  MapGrid_CopyCellPictureIndicesRect(36, 34, 16, 7, 1, 1);
  MapGrid_CopyCellAttributes(36, 34, 1, 1, 16, 7);
  GameFlag_Set(FLAG_NORTH_MARK_UNCOVERED);
}

/// Felix casts Scoop on the north mark at the dead end by the statue, cell
/// (16, 7), one of four in a diamond on the ground there: as the dig opens the
/// ground, the mark is dug out.
void Kibombo_ScoopNorthMark(s32 stage) {
  if (stage == SCOOP_STAGE_DUG)
    Kibombo_UncoverNorthMark();
}

/// Uncovers the south mark, cell (16, 11).
void Kibombo_UncoverSouthMark(void) {
  MapGrid_CopyCellPictureIndicesRect(38, 34, 16, 11, 1, 1);
  MapGrid_CopyCellAttributes(38, 34, 1, 1, 16, 11);
  GameFlag_Set(FLAG_SOUTH_MARK_UNCOVERED);
}

/// Felix casts Scoop on the south mark at the dead end by the statue, cell
/// (16, 11), the one Kraden first tells him to dig: as the dig opens the
/// ground, the mark is dug out.
void Kibombo_ScoopSouthMark(s32 stage) {
  if (stage == SCOOP_STAGE_DUG)
    Kibombo_UncoverSouthMark();
}

/// Uncovers the west mark, cell (14, 9).
void Kibombo_UncoverWestMark(void) {
  MapGrid_CopyCellPictureIndicesRect(36, 36, 14, 9, 1, 1);
  MapGrid_CopyCellAttributes(36, 36, 1, 1, 14, 9);
  GameFlag_Set(FLAG_WEST_MARK_UNCOVERED);
}

/// Felix casts Scoop on the west mark at the dead end by the statue, cell
/// (14, 9): as the dig opens the ground, the mark is dug out.
void Kibombo_ScoopWestMark(s32 stage) {
  if (stage == SCOOP_STAGE_DUG)
    Kibombo_UncoverWestMark();
}

/// Uncovers the east mark, cell (18, 9).
void Kibombo_UncoverEastMark(void) {
  MapGrid_CopyCellPictureIndicesRect(38, 36, 18, 9, 1, 1);
  MapGrid_CopyCellAttributes(38, 36, 1, 1, 18, 9);
  GameFlag_Set(FLAG_EAST_MARK_UNCOVERED);
}

/// Felix casts Scoop on the east mark at the dead end by the statue, cell
/// (18, 9): as the dig opens the ground, the mark is dug out.
void Kibombo_ScoopEastMark(s32 stage) {
  if (stage == SCOOP_STAGE_DUG)
    Kibombo_UncoverEastMark();
}

/// Felix uses Move on the stone pillar by Piers in the west of the grounds and
/// pushes it west off its ledge to cell (6, 18), where it drops with a thump
/// into the place Piers could not reach; Piers then joins the party.
void Kibombo_MovePillarForPiers(void) {
  struct Actor *actor = Actor_GetById(ACTOR_GROUNDS_PILLAR);

  if (FIELD_CELL(actor->x.fixed) == 6 && FIELD_CELL(actor->z.fixed) == 18) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_GetById(ACTOR_GROUNDS_PILLAR)->motionFlags =
        ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    do
      wait(1);
    while (actor->velocityY != 0);
    Sound_Play(SE_THUMP);
    wait(10);
    actor->motionFlags = 0;
    Actor_GetById(ACTOR_GROUNDS_PILLAR)->motionFlags = 0;
    MapGrid_CopyCellAttributes(5, 18, 1, 1, 6, 18);
    GameFlag_Set(FLAG_KIBOMBO_PILLAR_MOVED_FOR_PIERS);
    if (!GameFlag_Get(FLAG_PIERS_JOINED))
      Kibombo_PiersJoinsParty();
    Event_End();
  }
}

/// Felix walks up onto the ledge in the west of the statue's grounds where
/// Piers stands, cells (3, 25) to (3, 26): Piers turns, startled, "Wh-Who are
/// you?!?", and asks what the party wants.
void Kibombo_MeetPiers(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(0.6);
  FieldCamera_MoveTo(88, CAMERA_KEEP, 392, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 72, 392);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 88, 392);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_SOUTHWEST, 10);
  Event_SetMessageId((s32)MsgWhWhoAre);
  // "Wh-Who are you?!?"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_PlaceOnActor(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  Actor_PlaceOnActor(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
  Actor_PlaceOnActor(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
  wait(1);
  Actor_SetSpritePriority(ACTOR_KRADEN, 2);
  Actor_SetSpritePriority(ACTOR_JENNA, 2);
  Actor_SetSpritePriority(ACTOR_SHEBA, 2);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_SetMovementTypeById(ACTOR_KRADEN, gKibombo_KradenMeetsPiersScript);
  Actor_SetMovementTypeById(ACTOR_JENNA, gKibombo_JennaMeetsPiersScript);
  Actor_SetMovementTypeAndWaitById(ACTOR_SHEBA, gKibombo_ShebaMeetsPiersScript);
  // "What... What are your intentions?"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  if (GameFlag_Get(FLAG_PIERS_INTERROGATED))
    Kibombo_PiersHasForgottenParty();
  else
    Kibombo_PiersTakesPartyForEnemies();
  Kibombo_AkafubuOffersCrystal();
  Actor_SetSpeed(ACTOR_KRADEN, 1.6);
  Actor_SetSpeed(ACTOR_JENNA, 1.6);
  Actor_SetSpeed(ACTOR_SHEBA, 1.6);
  Actor_SetMovementTypeById(ACTOR_JENNA, gKibombo_PartyGathersScript);
  Actor_SetMovementTypeById(ACTOR_SHEBA, gKibombo_PartyGathersScript);
  Actor_SetMovementTypeAndWaitById(ACTOR_KRADEN, gKibombo_PartyGathersScript);
  GameFlag_Set(FLAG_KIBOMBO_PIERS_MET);
  Event_End();
}

/// The party's answer that Piers does not know them: Jenna wonders whether he
/// has forgotten them, until he remembers the travellers who came to his cell,
/// and asks when he ever asked for their help.
void Kibombo_PiersHasForgottenParty(void) {
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_WEST, 20);
  Event_SetMessageId((s32)MsgItLooksLikeHas);
  // "It looks like Piers has already forgotten us, huh?" Yes: "I know! We met
  // Piers when he was in JAIL!" No: "What, is he so obsessed with his black
  // orb..."
  FieldMessage_ShowAtActor(ACTOR_SHEBA, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 10);
    Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
    // "I know! We met Piers when he was in JAIL! And it's not like he had..."
    Event_ShowMessageAndWait(ACTOR_JENNA, 0);
    gFieldWork->message++;
  } else {
    Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 10);
    Actor_SetAnimationById(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    gFieldWork->message++;
    // "What, is he so obsessed with his black orb that he can't even..."
    Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  }
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "Wait a second... I know you..."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 40);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 20);
  // "You're the travelers who came seeking me when I was in jail."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  // "So, you do remember!"
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  // "What are you doing in this place?"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  // "Actually, that's what we wanted to ask you..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 40);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  // "We were worried about you, Piers, so we followed you here..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  // "And that's what brings you to this dangerous place?"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_Jump(ACTOR_SHEBA, 2, 10);
  Actor_Jump(ACTOR_SHEBA, 4, 10);
  // "What's that supposed to mean?"
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 10);
  // "We came here to help you, Piers. Why are you being like that about it?"
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 10);
  // "Tell me something... Exactly when did I ask you for your help?"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
}

/// The party's answer that it is no enemy: Piers doubts it, the party tells him
/// of the tidal wave and the boat it seeks, and he finds it hard to believe.
void Kibombo_PiersTakesPartyForEnemies(void) {
  Event_SetMessageId((s32)MsgHeSeemsTo);
  // "He seems to believe we are enemies..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 10);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  // "I hope he doesn't think we're with the Kibombo."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "I must admit, you don't look much like Kibombo..."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 40);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  FieldEffect_SetAttached(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  // "But that begs the question, if you're not Kibombo, then who are you..."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  // "They are...warriors. But how did you get in here?"
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
  // "Why are these warriors in a place like this?"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  // "I'm not sure how to answer that..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 40);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  // "Actually, we've been looking for a boat... We kinda need one."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  // "You're looking for a boat... in the middle of the continent? That..."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_Jump(ACTOR_SHEBA, 2, 10);
  Actor_Jump(ACTOR_SHEBA, 4, 10);
  // "Hey, this wasn't exactly our first choice, you know?"
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 10);
  // "A tidal wave destroyed all the boats in the coastal towns."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 10);
  // "You're asking me to believe rather a lot, don't you think?"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
}

/// Back on the grounds after Piers stayed behind, Felix walks up beside him
/// where he waits at the top of the grounds, cell (16, 5): Piers thanks the
/// party for coming to help get his orb back, walks over to Felix and joins
/// it again.
void Kibombo_PiersThanksParty(void) {
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(280, CAMERA_KEEP, 88, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Event_SetMessageId((s32)MsgYouVeCome);
  // "You've come to help me get my orb back? Thank you, all of you!"
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_WEST);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_SetSpeed(ACTOR_PIERS, 1.6);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_PIERS, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_PIERS);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  GameFlag_Clear(FLAG_KIBOMBO_PIERS_STAYED_BEHIND);
  GameFlag_Set(FLAG_KIBOMBO_PIERS_WITH_PARTY);
  Event_AddPartyMember(PIERS, 0);
  Event_End();
}

void Kibombo_CountAnswer(void);
void Kibombo_CarryBlackCrystal(void);
void Kibombo_CrowdBows(void);
void Kibombo_CrowdStands(void);
void Kibombo_EndOffering(void);

/// Akafubu raises the Black Crystal before the Great Gabomba while the warriors
/// chant.
void Kibombo_AkafubuOffersCrystal(void) {
  Kibombo_CrowdStands();
  Sound_Play(MUS_SILENCE);
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 10);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(264, CAMERA_KEEP, 388, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_AKAFUBU, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  Sound_Play(MUS_JUPITER_LIGHTHOUSE);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CRYSTAL_MARKER), 0);
  Actor_GetById(ACTOR_CRYSTAL_MARKER)->motionFlags = 0;
  Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_CRYSTAL_MARKER),
                                  toFIXED(265), toFIXED(24), toFIXED(344));
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(264, CAMERA_KEEP, 340, 1);
  Actor_SetSpeed(ACTOR_AKAFUBU, 0.2);
  Actor_WalkToAndWait(ACTOR_AKAFUBU, 264, 374);
  FieldTask_AddFrame(Kibombo_CarryBlackCrystal, TASK_PASS_FRAME);
  Event_Wait(20);
  Task_Remove(Kibombo_FlickerCeremonyFires);
  Kibombo_AkafubuCastsOnCrystal();
  Task_Add(Kibombo_FlickerCeremonyFires, TASK_PASS_FRAME);
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Sound_Play(MUS_RITUALS);
  Kibombo_EndOffering();
  Event_Wait(160);
  Event_ShowMessageThenWait(ACTOR_AKAFUBU, 0, 80);
  Sound_Play(SE_STATUE_STIRS);
  Kibombo_SetStatueMouth(1, 1);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_AKAFUBU, 0, 80);
  Sound_Play(SE_RATTLE);
  Kibombo_SetStatueMouth(0, 1);
  Event_Wait(20);
  FieldEffect_SetAttached(ACTOR_AKAFUBU,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_AKAFUBU, 0, 20);
  Task_Remove(Kibombo_CarryBlackCrystal);
  wait(1);
  Actor_SetPosition(ACTOR_CRYSTAL_MARKER, 0, 0);
  Actor_FaceDirection(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 0);
  if (gKibombo_BlackCrystal != 0) {
    if (gKibombo_BlackCrystal->y.fixed > toFIXED(36)) {
      do {
        Actor_FaceDirection(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 0);
        gKibombo_BlackCrystal->y.fixed += toFIXED(-0.25);
        wait(1);
      } while (gKibombo_BlackCrystal->y.fixed > toFIXED(36));
    }
  }
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(264, 16, 420, 1);
  Actor_SetSpeed(ACTOR_AKAFUBU, 1.6);
  Actor_WalkToAndWaitById(ACTOR_AKAFUBU, 264, 392);
  Event_Wait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_AKAFUBU, 0);
  Sound_Play(SE_BOSS_ROAR);
  Sound_Play(MUS_RITUALS);
  Kibombo_CrowdBows();
  Event_Wait(80);
  FieldCamera_MoveTo(88, 32, 392, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_SetSpritePriority(ACTOR_PIERS, 2);
  Actor_SetSpeed(ACTOR_PIERS, 1.6);
  Actor_WalkToAndWait(ACTOR_PIERS, 152, 386);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 40);
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_WalkToAndWait(ACTOR_PIERS, 124, 356);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 20);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 40);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkToById(ACTOR_KRADEN, 104, 368);
  Event_Wait(20);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(111, CAMERA_KEEP, 356, 1);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_STAND);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 40);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
  Event_OpenMessage(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Event_ShowMessageAndWait(
        SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    Event_ShowMessageAndWait(
        SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
    Kibombo_CountAnswer();
  }
}

/// Felix walks off the grounds by the south-west path, cells (4, 30) and
/// (5, 30). With Piers along and the orb not yet won back, Piers will not
/// leave without it: he says so, walks back up the path and stays behind as
/// the party goes down to the village.
void Kibombo_PiersRefusesToLeave(void) {
  if (!Event_GetFlag(FLAG_KIBOMBO_PIERS_WITH_PARTY)) {
    Import_EventSetExitRequest(
        KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_WEST_B);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_PlaceOnActor(ACTOR_PIERS, gMainRoutineWork.leaderActorId);
  Actor_SetSpritePriority(ACTOR_PIERS, ((struct FieldSprite *)Actor_GetById(
                                            gMainRoutineWork.leaderActorId)
                                            ->sprite)
                                           ->priority);
  wait(1);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHWEST, 0);
  Actor_SetSpeed(ACTOR_PIERS, 1.6);
  Actor_WalkToAndWaitById(ACTOR_PIERS, 62, 444);
  Actor_FaceDirectionAndRefreshById(ACTOR_PIERS, FACING_SOUTHEAST);
  Event_SetMessageId((s32)MsgNowThatIKnow);
  // "Now that I know the orb is here, I won't leave this village without..."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_GetById(ACTOR_PIERS)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_WalkToAndWait(ACTOR_PIERS, 56, 420);
  Actor_WalkToAndWait(ACTOR_PIERS, 40, 344);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Actor_WalkTo(gMainRoutineWork.leaderActorId, 80, 510);
  Event_Wait(10);
  Import_EventSetExitRequest(
      KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_WEST_B);
  GameFlag_Clear(FLAG_KIBOMBO_PIERS_WITH_PARTY);
  GameFlag_Set(FLAG_KIBOMBO_PIERS_STAYED_BEHIND);
  Party_RemoveMemberRestored(PIERS);
  Event_End();
}

void Party_GivePiersDjinn(void);
extern u32 gKibombo_LeaderStepsUpScript[];
extern u32 gKibombo_PiersStepsUpScript[];
extern u32 gKibombo_KradenJoinsPiersScript[];
extern u32 gKibombo_JennaJoinsPiersScript[];
extern u32 gKibombo_ShebaJoinsPiersScript[];
extern u32 gKibombo_PartyFollowsScript[];

/// Piers, who saw the party's Psynergy, tells them he too is an Adept, from
/// Lemuria in the middle of the Eastern Sea, and joins the party with his
/// Djinn; Felix's answers to Kraden choose the lines that follow.
void Kibombo_PiersJoinsParty(void) {
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_NORTH, 0);
  Event_SetMessageId(MsgPiersStartled);
  // "?!?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(3.2);
  FieldCamera_MoveTo(96, CAMERA_KEEP, 340, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 0);
  FieldEffect_SetAttached(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_AskYesNo(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(96, CAMERA_KEEP, 372, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.2);
  Actor_SetSpeed(ACTOR_PIERS, 1.2);
  Actor_SetMovementTypeById(ACTOR_PIERS, gKibombo_PiersStepsUpScript);
  Actor_SetMovementTypeAndWaitById(gMainRoutineWork.leaderActorId,
                                   gKibombo_LeaderStepsUpScript);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_SetPosition(ACTOR_KRADEN, 56, 372);
  Actor_SetPosition(ACTOR_JENNA, 56, 372);
  Actor_SetPosition(ACTOR_SHEBA, 56, 372);
  wait(1);
  Actor_SetMovementTypeById(ACTOR_KRADEN, gKibombo_KradenJoinsPiersScript);
  Actor_SetMovementTypeById(ACTOR_JENNA, gKibombo_JennaJoinsPiersScript);
  Actor_SetMovementTypeAndWaitById(ACTOR_SHEBA, gKibombo_ShebaJoinsPiersScript);
  Event_Wait(20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_ShowEmoteById(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 40);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_WEST);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHEAST);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Event_OpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      1) {
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
    gFieldWork->message += 1;
  } else {
    gFieldWork->message += 2;
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  }
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 20);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 20);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 20);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH_SOUTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 40);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 10);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, ACTOR_PIERS);
  Actor_WatchAndSetScript(ACTOR_JENNA, ACTOR_PIERS);
  Actor_WatchAndSetScript(ACTOR_SHEBA, ACTOR_PIERS);
  Actor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_PIERS);
  Actor_SetSpeed(ACTOR_PIERS, 1.6);
  Actor_WalkToAndWaitById(ACTOR_PIERS, 72, 368);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_WalkToAndWait(ACTOR_PIERS, 40, 348);
  Actor_WalkToAndWait(ACTOR_PIERS, 40, 332);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_SHEBA);
  Actor_Stop(ACTOR_KRADEN);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 40);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_PIERS, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_PIERS, 0);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_PIERS, 0);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 40);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Event_OpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 20);
    Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
    gFieldWork->message += 3;
  } else {
    gFieldWork->message += 1;
    Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
    Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHEAST);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
    Actor_FaceDirectionAndRefreshById(ACTOR_SHEBA, FACING_EAST);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
    Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
    FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    Kibombo_CountAnswer();
  }
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_PIERS, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_NORTH);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Event_ShowMessageAndWait(
      SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_SetSpeed(ACTOR_PIERS, 0.4);
  Actor_WalkToAndWaitById(ACTOR_PIERS, 40, 350);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_SetSpeed(ACTOR_PIERS, 0.8);
  Actor_SetMovementTypeById(ACTOR_JENNA, gKibombo_PartyFollowsScript + 0);
  Actor_SetMovementTypeById(ACTOR_SHEBA, gKibombo_PartyFollowsScript + 0);
  Actor_SetMovementTypeById(ACTOR_KRADEN, gKibombo_PartyFollowsScript + 0);
  Actor_SetMovementTypeAndWaitById(ACTOR_PIERS,
                                   gKibombo_PartyFollowsScript + 0);
  GameFlag_Set(FLAG_PIERS_JOINED);
  GameFlag_Set(FLAG_KIBOMBO_PIERS_WITH_PARTY);
  Event_AddPartyMember(PIERS, 1);
  Party_GivePiersDjinn();
}

extern u32 gKibombo_JennaAtDeadEndScript[];
extern u32 gKibombo_ShebaAtDeadEndScript[];
extern u32 gKibombo_KradenAtDeadEndScript[];
extern u32 gKibombo_PiersAtDeadEndScript[];
extern u32 gKibombo_PartyLeavesDeadEndScript[];
void Kibombo_CastScoop(void);

/// The first time Felix comes up the path east of the statue to the dead end,
/// cells (21, 14) and (22, 14), before the vent is uncovered: the party stops
/// among the four marks on the ground, "It's a dead end...". Kraden has Felix
/// Scoop each mark in turn, and the party works out that a vent into the
/// statue lies under the patch it passed on the way.
void Kibombo_DeadEndByTheStatue(void) {
  s32 done = Event_GetFlag(FLAG_KIBOMBO_VENT_UNCOVERED);

  if (done != 0)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 344, 208);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 308, 200);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 290, 188);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHWEST, 0);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_SetSpeed(ACTOR_PIERS, 0.8);
  Actor_PlaceOnActor(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
  Actor_PlaceOnActor(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
  Actor_PlaceOnActor(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  Actor_PlaceOnActor(ACTOR_PIERS, gMainRoutineWork.leaderActorId);
  wait(1);
  Actor_SetMovementTypeById(ACTOR_JENNA, gKibombo_JennaAtDeadEndScript);
  Actor_SetMovementTypeById(ACTOR_SHEBA, gKibombo_ShebaAtDeadEndScript);
  Actor_SetMovementTypeById(ACTOR_KRADEN, gKibombo_KradenAtDeadEndScript);
  Actor_SetMovementTypeAndWaitById(ACTOR_PIERS, gKibombo_PiersAtDeadEndScript);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Event_SetMessageId(MsgItSADead);
  // "It's a dead end..."
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_PIERS, 0);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_GetActor()->motionFlags = done;
  FieldCamera_MoveTo(200, CAMERA_KEEP, 180, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  FieldCamera_MoveTo(174, CAMERA_KEEP, 224, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  FieldCamera_MoveTo(268, CAMERA_KEEP, 120, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  FieldCamera_MoveTo(272, CAMERA_KEEP, 180, 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  // "But this was the only path..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  // "Isn't it a little strange that there would be a path that just goes..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 20);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, ACTOR_KRADEN);
  Actor_WatchAndSetScript(ACTOR_JENNA, ACTOR_KRADEN);
  Actor_WatchAndSetScript(ACTOR_SHEBA, ACTOR_KRADEN);
  Actor_WatchAndSetScript(ACTOR_PIERS, ACTOR_KRADEN);
  FieldCamera_SetSpeed(1.2);
  FieldCamera_MoveTo(268, CAMERA_KEEP, 146, 1);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 290, 116);
  Actor_WalkToAndWait(ACTOR_KRADEN, 268, 109);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 40);
  // "Hmm..."
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
  FieldCamera_MoveTo(268, CAMERA_KEEP, 188, 1);
  Actor_WalkToAndWaitById(ACTOR_KRADEN, 218, 156);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 40);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_SHEBA);
  Actor_Stop(ACTOR_PIERS);
  // "I see..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 20);
  Actor_SetSpeed(ACTOR_KRADEN, 0.4);
  Actor_WalkToAndWaitById(ACTOR_KRADEN, 248, 156);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "So that's how it works..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, ACTOR_KRADEN);
  Actor_WatchAndSetScript(ACTOR_JENNA, ACTOR_KRADEN);
  Actor_WatchAndSetScript(ACTOR_SHEBA, ACTOR_KRADEN);
  Actor_WatchAndSetScript(ACTOR_PIERS, ACTOR_KRADEN);
  Event_Wait(20);
  Actor_GetById(ACTOR_KRADEN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWaitById(ACTOR_KRADEN, 248, 188);
  Event_Wait(1);
  Actor_GetById(ACTOR_KRADEN)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(20);
  Actor_SetAnimationById(ACTOR_KRADEN, 7);
  Event_Wait(20);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_SHEBA);
  Actor_Stop(ACTOR_PIERS);
  // "Listen up, Felix..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.4);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 280, 188);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_STAND);
  Event_Wait(10);
  Actor_GetById(ACTOR_KRADEN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWaitById(ACTOR_KRADEN, 238, 188);
  Event_Wait(1);
  Actor_GetById(ACTOR_KRADEN)->steeringFlags |= ACTOR_FACES_MOTION;
  // "Dig there!"
  FieldMessage_ShowAtActor(ACTOR_KRADEN, 0);
  Actor_ShowEmoteById(gMainRoutineWork.leaderActorId,
                      EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
ask:
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    gFieldWork->message += 1;
  } else {
    // "Don't worry. Just use Scoop... You'll see why in just a few seconds..."
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    gFieldWork->message -= 2;
    // "I see..."
    FieldMessage_ShowAtActor(ACTOR_KRADEN, 0);
    goto ask;
  }
  Event_SetFlag(FLAG_SCOOPING_SOUTH_MARK);
  Kibombo_CastScoop();
  GameFlag_Clear(FLAG_SCOOPING_SOUTH_MARK);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_WatchAndSetScript(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
  Actor_WatchAndSetScript(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
  Actor_WatchAndSetScript(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  Actor_WatchAndSetScript(ACTOR_PIERS, gMainRoutineWork.leaderActorId);
  FieldCamera_MoveTo(268, CAMERA_KEEP, 164, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 280, 120);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_WEST);
  Event_SetFlag(FLAG_SCOOPING_NORTH_MARK);
  Kibombo_CastScoop();
  GameFlag_Clear(FLAG_SCOOPING_NORTH_MARK);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 232, 142);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTH);
  Event_SetFlag(FLAG_SCOOPING_WEST_MARK);
  Kibombo_CastScoop();
  GameFlag_Clear(FLAG_SCOOPING_WEST_MARK);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 296, 142);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTH);
  Event_SetFlag(FLAG_SCOOPING_EAST_MARK);
  Kibombo_CastScoop();
  GameFlag_Clear(FLAG_SCOOPING_EAST_MARK);
  FieldCamera_MoveTo(268, CAMERA_KEEP, 188, 1);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 284, 188);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 10);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_SHEBA);
  Actor_Stop(ACTOR_KRADEN);
  Actor_Stop(ACTOR_PIERS);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 20);
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 10);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  FieldCamera_MoveTo(268, CAMERA_KEEP, 164, 1);
  Actor_WalkToAndWait(ACTOR_JENNA, 296, 168);
  Actor_WalkToAndWait(ACTOR_JENNA, 280, 154);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 40);
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  FieldCamera_MoveTo(268, CAMERA_KEEP, 188, 1);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 40);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 20);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 10);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetSpeed(ACTOR_PIERS, 1.2);
  Actor_SetMovementTypeById(ACTOR_JENNA, gKibombo_PartyLeavesDeadEndScript);
  Actor_SetMovementTypeById(ACTOR_SHEBA, gKibombo_PartyLeavesDeadEndScript);
  Actor_SetMovementTypeById(ACTOR_KRADEN, gKibombo_PartyLeavesDeadEndScript);
  Actor_SetMovementTypeAndWaitById(ACTOR_PIERS,
                                   gKibombo_PartyLeavesDeadEndScript);
  GameFlag_Set(FLAG_KIBOMBO_DEAD_END_SEEN);
  Event_End();
}

/// Nothing visible happens as Felix steps onto the ring of cells (19, 7) to
/// (21, 10) among the painted poles in the north of the village. The village
/// keeps two masks (`FadeOverlayWork`, drawn by `BlankOverlay_DrawMarks`) that
/// cover whatever of Felix a raised cell beside him should hide; from here they
/// follow him.
void Kibombo_OverlayFollowsLeader(void) {
  // Walking round the ring's west side from cell (18, 9), up to the painted
  // pole and back, Felix stays drawn whole in front of the pole: nothing of
  // him is hidden there.
  // Walking east along the ring's north side from cell (18, 7), behind the
  // pole, Felix is hidden by it as he passes, and further east the thatched
  // roof of the hut beside the ring hides all of him but the top of his head.
  struct FadeOverlayWork *work = Ram_HeapSlots->fadeOverlay;

  work->actor =
      (struct FadeActor *)Actor_GetById(gMainRoutineWork.leaderActorId);
}

/// Nothing visible happens as Felix steps onto cell (20, 8) or (20, 9), in the
/// middle of that ring among the painted poles; the masks that hide him behind
/// raised cells stop following him.
void Kibombo_OverlayStopsFollowing(void) {
  struct FadeOverlayWork *work = Ram_HeapSlots->fadeOverlay;

  work->actor = 0;
}

/// Clamps a colour channel to 0 to RGB_LEVEL_MAX.
s32 Kibombo_ClampColor(s32 value) {
  if (value > RGB_LEVEL_MAX)
    value = RGB_LEVEL_MAX;
  if (value < 0)
    value = 0;
  return value;
}

/// Sets a palette colour from its fire colour, brightened by delta (half for
/// green, a quarter for blue).
void Kibombo_FlickerColor(s32 index, s32 entry, s32 delta) {
  u16 color = gKibombo_FireColors[entry];
  s16 r = RGB_RED(color);
  s16 g = RGB_GREEN(color);
  s16 b = RGB_BLUE(color);

  r += delta;
  g += delta >> 1;
  b += delta >> 2;

  r = Kibombo_ClampColor(r);
  g = Kibombo_ClampColor(g);
  b = Kibombo_ClampColor(b);
  {
    u16 *dst = &BG_PLTT[index];

    r |= RGB(0, g, b);
    *dst = r;
  }
}

/// As Kibombo_FlickerColor, four steps redder and the green and blue four steps
/// dimmer.
void Kibombo_FlickerRedColor(s32 index, s32 entry, s32 delta) {
  u16 color = gKibombo_FireColors[entry];
  s16 r = RGB_RED(color);
  s16 g = RGB_GREEN(color);
  s16 b = RGB_BLUE(color);

  r += delta + 4;
  delta -= 4;
  g += delta >> 1;
  b += delta >> 2;

  r = Kibombo_ClampColor(r);
  g = Kibombo_ClampColor(g);
  b = Kibombo_ClampColor(b);
  {
    u16 *dst = &BG_PLTT[index];

    r |= RGB(0, g, b);
    *dst = r;
  }
}

#define DEFINE_KIBOMBO_FLICKER_FIRES(name, secondIndex, secondRange,           \
                                     secondBias, thirdIndex, flickerFourth,    \
                                     fourthIndex, fourthRange, fourthBias)     \
  void name(void) {                                                            \
    struct FieldWork *work = Ram_HeapSlots->fieldWork;                         \
                                                                               \
    if (work->psynergy == 0 && work->fieldMenu == 0 && work->pauseMenu == 0 && \
        (gFrameCount & 7) == 0) {                                              \
      Kibombo_FlickerColor(FIRE_COLOR_FIRST, 0, (Random_Below(10)) - 5);       \
      Kibombo_FlickerColor(secondIndex, 1,                                     \
                           (Random_Below(secondRange)) - secondBias);          \
      Kibombo_FlickerColor(thirdIndex, 2, (Random_Below(4)) - 2);              \
      flickerFourth(fourthIndex, 3, (Random_Below(fourthRange)) - fourthBias); \
    }                                                                          \
  }

/// A frame task in the night village: every eighth frame outside menus and
/// Psynergy, the four fire colours flicker.
DEFINE_KIBOMBO_FLICKER_FIRES(Kibombo_FlickerVillageFires,
                             VILLAGE_FIRE_COLOR_SECOND, 8, 4,
                             VILLAGE_FIRE_COLOR_THIRD, Kibombo_FlickerColor,
                             VILLAGE_FIRE_COLOR_FOURTH, 4, 2)

/// As Kibombo_FlickerVillageFires on the night grounds, its fourth colour
/// flickering redder.
DEFINE_KIBOMBO_FLICKER_FIRES(Kibombo_FlickerCeremonyFires,
                             GROUNDS_FIRE_COLOR_SECOND, 6, 3,
                             GROUNDS_FIRE_COLOR_THIRD, Kibombo_FlickerRedColor,
                             GROUNDS_FIRE_COLOR_FOURTH, 8, 4)

/// A frame task before the statue's face: every eighth frame outside menus and
/// Psynergy, the three fire colours flicker.
void Kibombo_FlickerStatueFires(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  if (work->psynergy == 0 && work->fieldMenu == 0 && work->pauseMenu == 0 &&
      (gFrameCount & 7) == 0) {
    Kibombo_FlickerColor(STATUE_FIRE_COLOR_FIRST, 0, (Random_Below(4)) - 2);
    Kibombo_FlickerColor(STATUE_FIRE_COLOR_SECOND, 1, (Random_Below(2)) - 1);
    Kibombo_FlickerColor(STATUE_FIRE_COLOR_THIRD, 2, Random16() >> 16);
  }
}

/// Creates the Black Crystal, drawn with the item's own icon.
void Kibombo_CreateBlackCrystal(void) {
  struct Actor **slot = &gKibombo_BlackCrystal;
  u8 clear = 0;
  struct Actor *actor = Import_ActorCreate(SPRITE_UNSEEN_MARKER, toFIXED(265),
                                           toFIXED(36), toFIXED(344));

  *slot = actor;
  if (actor != 0) {
    struct FieldSprite *sprite = actor->sprite;
    GlyphTransfer *glyph;

    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 1;
    actor->motionFlags = clear;
    actor->retainSpriteCache = 1;
    sprite->flags = clear;
    sprite->countAndShadow[0] = clear;
    glyph = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
    Icon_DrawItem(ITEM_BLACK_CRYSTAL);
    VramCache_Load(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  }
}

/// Unused: with the leader up high, takes the door's unseen block away and
/// draws the first lit candlestick in front; lower down, puts the block back at
/// the house's door and the candlestick behind.
UNUSED void Kibombo_PlaceDoorBlockByHeight(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  if (leader->y.fixed > toFIXED(16)) {
    Actor_SetPosition(ACTOR_DOOR_BLOCK, 0, 0);
    Actor_SetSpritePriority(ACTOR_LIT_CANDLESTICK_A, 2);
  } else {
    FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_DOOR_BLOCK),
                                         toFIXED(120), 0, toFIXED(168));
    Actor_SetSpritePriority(ACTOR_LIT_CANDLESTICK_A, 3);
  }
}

/// Keeps the Black Crystal above the one who holds it.
void Kibombo_CarryBlackCrystal(void) {
  if (gKibombo_BlackCrystal != 0) {
    struct Actor *actor = Actor_GetById(ACTOR_CRYSTAL_MARKER);

    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, actor->x.fixed,
                                    actor->y.fixed + toFIXED(12),
                                    actor->z.fixed);
  }
}

#include "../field/field_effect_advance_motion.c"

#undef DEFINE_KIBOMBO_FLICKER_FIRES
#undef Actor_SetSpriteFlags
#include "shared/tables.h"

void Actor_StartScript(struct Actor *actor, const void *script);
extern u32 gKibombo_EmberScript[];

/// Embers rise from the statue's fire every fourth frame.
void Kibombo_RaiseEmbers(void) {
  s32 phase = gFrameCount & 3;

  if (phase == 0) {
    struct Actor *actor = Import_ActorCreate(SPRITE_BLAZE_FLAME, toFIXED(264),
                                             toFIXED(32), toFIXED(316));
    s32 value;

    if (actor != 0) {
      actor->bounce = Trig_Cos(((Random16() << 13) >> 16) + TRIG_TURN * 3 / 16);
      actor->gravity = toFIXED(1);
      actor->riseBaseY = toFIXED(4) - (u16)Random16();
      value = (u16)Random16() + 0xffff8000;
      actor->timer = value;
      actor->motionFlags = 0;
      Actor_SetSpriteFlags(actor, 0);
      Actor_SelectAnimation(actor, 2);
      Actor_StartScript(actor, gKibombo_EmberScript);
      ((struct FieldSprite *)actor->sprite)->priority = 1;
      actor->timer = 0;
      actor->speedLimit = 0;
      actor->acceleration = 0;
      actor->update = (ObjectUpdate)FieldEffect_AdvanceMotion;
    }
  }
}

/// A cloud of smoke's update: drifts sideways and up, swelling, until its
/// SMOKE_FRAMES are out.
void Kibombo_UpdateSmoke(struct Actor *actor) {
  actor->x.fixed += actor->timer << 8;

  actor->y.fixed += toFIXED(1) / 2;

  actor->scaleX += FIXED_ONE / 50;
  actor->scaleY += FIXED_ONE / 50;
  actor->timer += 2;
  if (--*(s32 *)&actor->watchedActor == 0)
    Actor_Release(actor);
}

/// Smoke rises from the cooking fire every 64 frames.
void Kibombo_RaiseSmoke(void) {
  if ((gFrameCount & 63) == 0) {
    struct Actor *actor = Import_ActorCreate(SPRITE_DUST_CLOUD, toFIXED(88),
                                             toFIXED(16), toFIXED(314));

    if (actor != 0) {
      actor->retainSpriteCache = 2;
      actor->motionFlags = 0;
      actor->timer = 0;
      actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      ((struct FieldSprite *)actor->sprite)->priority = 2;
      Actor_SetSpriteFlags(actor, 0);
      actor->scaleX = FIXED_ONE / 2;
      actor->scaleY = FIXED_ONE / 2;
      *(s32 *)&actor->watchedActor = SMOKE_FRAMES;
      actor->update = (ObjectUpdate)Kibombo_UpdateSmoke;
      Actor_SelectAnimation(actor, 5);
      Actor_SetBlendMode(actor, 1);
    }
  }
}

/// A flame's update: every ten to twenty frames it flips between wide and
/// mirrored.
void Kibombo_FlickerFlame(struct Actor *actor) {
  if (actor->timer != 0) {
    actor->timer--;
  } else {
    actor->timer = (Random_Below(10)) + 10;
    if (actor->userValue != 0) {
      actor->scaleX = FIXED_ONE * 3 / 2;
      actor->userValue = 0;
    } else {
      actor->scaleX = -FIXED_ONE * 3 / 2;
      actor->userValue = 1;
    }
  }
}

/// Creates one of the signal fire's flames at x, y, z, flickering by its own
/// timer.
void Kibombo_SpawnFlame(s32 x, s32 y, s32 z) {
  struct Actor *actor = Import_ActorCreate(SPRITE_FLAME_AND_SMOKE, x, y, z);
  struct FieldSprite *sprite;

  if (actor != 0) {
    actor->motionFlags = 0;
    actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    actor->scaleY = FIXED_ONE * 2;
    actor->timer = (Random_Below(10)) + 10;
    actor->userValue = 0;
    actor->update = (ObjectUpdate)Kibombo_FlickerFlame;
    sprite = actor->sprite;
    sprite->priority = 1;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SelectAnimation(actor, 5);
    Actor_SetBlendMode(actor, 1);
  }
}

/// Lights the signal fire: five flames rise in a column above it, blended over
/// the background.
void Kibombo_LightSignalFire(void) {
  FIXED x;

  REG_BLDALPHA = BLEND_ALPHA(8, 12);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  x = toFIXED(440);
  Kibombo_SpawnFlame(x, toFIXED(40), toFIXED(450));
  Kibombo_SpawnFlame(x, toFIXED(46), toFIXED(438));
  Kibombo_SpawnFlame(x, toFIXED(52), toFIXED(426));
  Kibombo_SpawnFlame(x, toFIXED(58), toFIXED(414));
  Kibombo_SpawnFlame(x, toFIXED(64), toFIXED(402));
}

#include "shared/actor_hide.h"

/// Back from a battle or a menu, stops drawing the room's unseen blocks again
/// unless they were shown; otherwise notes them shown.
void Kibombo_RestoreBlocks(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    if (GameFlag_Get(FLAG_KIBOMBO_BLOCKS_SHOWN) == 0)
      Kibombo_HideBlocks();
  } else {
    GameFlag_Set(FLAG_KIBOMBO_BLOCKS_SHOWN);
  }
}

/// Hides the grounds' two unseen blocks as the room opens, the first raised 32
/// pixels.
void Kibombo_HideGroundsBlocks(void) {
  u32 i;

  for (i = 0; i <= 1; i++) {
    s32 id = i + ACTOR_GROUNDS_FIRST_BLOCK;
    struct Actor *actor = Actor_GetById(id);

    Actor_Hide(id);
    actor->motionFlags = 0;
  }
  Actor_GetById(ACTOR_GROUNDS_FIRST_BLOCK)->y.fixed = toFIXED(32);
  Actor_GetById(ACTOR_GROUNDS_SECOND_BLOCK)->y.fixed = 0;
  Kibombo_RestoreBlocks();
}

/// Hides the two unseen blocks in the inn's back doorway as the room opens.
void Kibombo_HideInnBlocks(void) {
  u32 i;

  for (i = 0; i <= 1; i++) {
    s32 id = i + ACTOR_INN_FIRST_BLOCK;
    struct Actor *actor = Actor_GetById(id);

    Actor_Hide(id);
    actor->motionFlags = 0;
  }
  Kibombo_RestoreBlocks();
}

#define DEFINE_KIBOMBO_SET_CROWD_MODE(name, animation)                         \
  void name(void) {                                                            \
    u32 i;                                                                     \
                                                                               \
    for (i = 0; i <= ACTOR_WARRIOR_H - ACTOR_WARRIOR_A; i++)                   \
      Actor_SetAnimationById(i + ACTOR_WARRIOR_A, animation);                  \
  }

/// The eight warriors, ACTOR_WARRIOR_A to H, bow (their sheet's animation 5).
DEFINE_KIBOMBO_SET_CROWD_MODE(Kibombo_CrowdBows, 5)

/// The eight warriors stand again.
DEFINE_KIBOMBO_SET_CROWD_MODE(Kibombo_CrowdStands, ANIM_STAND)

/// Akafubu's Psynergy on the Black Crystal.
void Kibombo_AkafubuCastsOnCrystal(void) {
  FieldPsynergy_LoadResources(ABILITY_LIFT, 1);
  FieldPsynergy_SetupActorPair(ACTOR_AKAFUBU, ACTOR_CRYSTAL_MARKER);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  FieldPsynergy_End();
}

s32 VramCache_FreeEntry(u32 slot);

/// A spark's update while Akafubu casts on the crystal: it circles its center
/// at its radius, wobbling in and out, and rises.
void Kibombo_SpinSpark(union FieldObject *object) {
  FIXED wobble = 0;
  u8 *timer = &object->actor.riseEnabled;
  s16 *action;
  u8 *stage;
  struct Vector3 center;
  s32 radius;
  s32 spin;
  s32 turn;

  if (*timer != 0)
    wobble = Trig_Sin((object->actor.timer + (s16)object->actor.userValue) * 9
                      << 9) *
             6;
  action = &object->actor.timer;
  center.x = object->actor.targetX;
  center.y = object->actor.targetY;
  center.z = object->actor.targetZ;
  radius = object->actor.riseBaseY;
  spin = (s16)object->actor.userValue;
  turn = *action;
  Vector_AddPolarOffset(toFIXED(radius) + wobble,
                        turn * (TRIG_TURN * 3 / 256) + spin, &center);
  object->actor.x.fixed = center.x;
  object->actor.y.fixed = center.y;
  object->actor.z.fixed = center.z;
  stage = &object->actor.riseCounter;
  switch (*stage) {
  case 0:
    *action += 4;
    object->actor.riseBaseY = 1;
    ((struct FieldSprite *)object->actor.sprite)->rotation += TRIG_TURN / 16;
    object->actor.scaleX += toFIXED(0.025);
    object->actor.scaleY += toFIXED(0.025);
    if (object->actor.scaleX > FIXED_ONE * 3 / 2 &&
        ((struct FieldSprite *)object->actor.sprite)->rotation == 0) {
      *stage = 1;
      *timer = 1;
    }
    break;
  case 1:
    *action += 4;
    ((struct FieldSprite *)object->actor.sprite)->rotation += TRIG_TURN / 16;
    if (((struct FieldSprite *)object->actor.sprite)->rotation == 0 &&
        --*timer == 0) {
      Sound_Play(SE_SHIMMER);
      *stage = 2;
    }
    break;
  case 2:
    *action += 1;
    object->actor.targetY += toFIXED(4);
    if (object->actor.targetY > toFIXED(100))
      *stage = 3;
    break;
  case 3:
    *action += 1;
    object->actor.riseBaseY += 2;
    *timer = 10;
    if (object->actor.riseBaseY > 40)
      *stage = 4;
    break;
  case 4:
    *action += 1;
    if (--*timer == 0)
      *stage = 5;
    break;
  case 5:
    *action += 1;
    object->actor.riseBaseY -= 2;
    if (object->actor.targetY < toFIXED(136))
      object->actor.targetY += toFIXED(2);
    if (object->actor.riseBaseY <= 1) {
      object->actor.riseBaseY = 0;
      *stage = 6;
    }
    break;
  case 6:
    *action += 4;
    ((struct FieldSprite *)object->actor.sprite)->rotation += TRIG_TURN / 16;
    object->actor.scaleX += -FIXED_ONE / 16;
    object->actor.scaleY += -FIXED_ONE / 16;
    if (object->actor.scaleX < FIXED_ONE * 9 / 128) {
      object->actor.targetX = 100;
      object->actor.targetZ = 100;
      *stage = 7;
    }
    break;
  case 7:
    VramCache_FreeEntry(((struct FieldSprite *)object->actor.sprite)->slot);
    Actor_Release(&object->actor);
    break;
  }
}

/// The sparks that circle Akafubu as he chants, 22 degrees apart.
#define CHANT_SPARKS 16

/// Akafubu glows and sparks circle him.
void Kibombo_AkafubuChants(void) {
  struct Actor *center = Actor_GetById(ACTOR_AKAFUBU);
  struct Actor *spark;
  s32 previous = 0;
  u32 i;
  u32 angle; // degrees in 16.16; userValue takes angle / 360, 65,536ths of a
             // turn
  FIXED x;
  FIXED y;
  FIXED z;

  Sound_Play(SE_CAST);
  for (i = 0; i <= 7; i++) {
    Actor_SetSpriteColor(Actor_GetById(ACTOR_AKAFUBU), SPRITE_COLOR_WHITE);
    wait(2);
    Actor_SetSpriteColor(Actor_GetById(ACTOR_AKAFUBU), SPRITE_COLOR_NORMAL);
    wait(2);
  }
  Sound_Play(SE_MERCURY_LIGHTHOUSE_DOOR_OPEN);
  i = 0;
  angle = 0;
  for (; i < CHANT_SPARKS; i++) {
    x = center->x.fixed;
    y = center->y.fixed + toFIXED(24);
    z = center->z.fixed;
    spark = Import_ActorCreate(SPRITE_SPARKLE, x, y, z);
    if (spark != 0) {
      previous = Sprite_ShareVramCacheEntry(spark->sprite, previous);
      spark->motionFlags = 0;
      ((struct FieldSprite *)spark->sprite)->priority = 1;
      Actor_SetSpriteFlags(spark, 0);
      Actor_SelectAnimation(spark, 1);
      spark->timer = 0;
      spark->riseBaseY = 0;
      spark->userValue = angle / 360;
      spark->riseCounter = 0;
      spark->targetX = center->x.fixed;
      spark->targetY = center->y.fixed + toFIXED(24);
      spark->targetZ = center->z.fixed;
      spark->update = (ObjectUpdate)Kibombo_SpinSpark;
    }
    angle += toFIXED(22);
  }
}

/// Ends the offering: the fires stop flickering and, while the crystal is still
/// out, Akafubu chants.
void Kibombo_EndOffering(void) {
  Task_Remove(Kibombo_FlickerCeremonyFires);
  wait(1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  if (gKibombo_BlackCrystal != 0)
    Kibombo_AkafubuChants();
}

/// Opens or shuts the Great Gabomba's mouth; while it is open its two eyes
/// show, ringed with light if `glow`.
void Kibombo_SetStatueMouth(s32 open, s32 glow) {
  if (open != 0) {
    MapGrid_CopyCellsTo(49, 37, 79, 15, 3, 2);
    FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_STATUE_LEFT_EYE),
                                         toFIXED(253), toFIXED(32),
                                         toFIXED(289));
    Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_STATUE_RIGHT_EYE),
                                    toFIXED(275), toFIXED(32), toFIXED(289));
    if (glow != 0) {
      Actor_SetAnimationById(ACTOR_STATUE_LEFT_EYE, 0);
      Actor_SetAnimationById(ACTOR_STATUE_RIGHT_EYE, 0);
    } else {
      Actor_SetAnimationById(ACTOR_STATUE_LEFT_EYE, 1);
      Actor_SetAnimationById(ACTOR_STATUE_RIGHT_EYE, 1);
    }
  } else {
    MapGrid_CopyCellsTo(45, 37, 79, 15, 3, 2);
    Actor_SetPosition(ACTOR_STATUE_LEFT_EYE, 0, 0);
    Actor_SetPosition(ACTOR_STATUE_RIGHT_EYE, 0, 0);
  }
}

/// Puts out the Great Gabomba's tongue, in the pose given, or draws it back.
void Kibombo_SetStatueTongue(s32 open, s32 mode) {
  if (open != 0) {
    MapGrid_CopyCellPictureIndicesRect(41, 38, 79, 16, 3, 3);
    MapGrid_CopyCellAttributes(12, 21, 1, 1, 16, 22);
    if (mode != 0)
      Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_STATUE_TONGUE),
                                      toFIXED(264), toFIXED(32), toFIXED(308));
    switch (mode) {
    case 0:
      Actor_SetPosition(ACTOR_STATUE_TONGUE, 0, 0);
      break;
    case 1:
      Actor_SetAnimationById(ACTOR_STATUE_TONGUE, 0);
      break;
    case 2:
      Actor_SetAnimationById(ACTOR_STATUE_TONGUE, 1);
      break;
    case 3:
      Actor_SetAnimationById(ACTOR_STATUE_TONGUE, 2);
      break;
    case 4:
      Actor_SetAnimationById(ACTOR_STATUE_TONGUE, 3);
      break;
    case 5:
      Actor_SetAnimationById(ACTOR_STATUE_TONGUE, 6);
      Actor_GetById(ACTOR_STATUE_TONGUE)->y.fixed -= toFIXED(34);
      break;
    case 6:
      Actor_SetAnimationById(ACTOR_STATUE_TONGUE, 7);
      Actor_GetById(ACTOR_STATUE_TONGUE)->y.fixed -= toFIXED(34);
      break;
    }
  } else {
    MapGrid_CopyCellPictureIndicesRect(45, 38, 79, 16, 3, 3);
    MapGrid_CopyCellAttributes(17, 22, 1, 1, 16, 22);
    Actor_SetPosition(ACTOR_STATUE_TONGUE, 0, 0);
  }
}

/// While Scoop is cast, uncovers the mark the event is scooping.
void Kibombo_UncoverMarkUnderLeader(void) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;

  if (work->stage != 0) {
    if (GameFlag_Get(FLAG_SCOOPING_NORTH_MARK))
      Kibombo_UncoverNorthMark();
    else if (GameFlag_Get(FLAG_SCOOPING_SOUTH_MARK))
      Kibombo_UncoverSouthMark();
    else if (GameFlag_Get(FLAG_SCOOPING_WEST_MARK))
      Kibombo_UncoverWestMark();
    else if (GameFlag_Get(FLAG_SCOOPING_EAST_MARK))
      Kibombo_UncoverEastMark();
  }
}

/// Felix casts Scoop on the mark he stands on.
void Kibombo_CastScoop(void) {
  FieldPsynergy_LoadResources(ABILITY_SCOOP, 1);
  FieldTask_AddFrame(Kibombo_UncoverMarkUnderLeader, TASK_PASS_FRAME);
  FieldPsynergy_SetupActorPair(gMainRoutineWork.leaderActorId, -1);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  Task_Remove(Kibombo_UncoverMarkUnderLeader);
  FieldPsynergy_End();
}

/// Stops the room's fire flicker task.
void Kibombo_StopFireFlicker(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_KIBOMBO_CEREMONY)
    Task_Remove(Kibombo_FlickerVillageFires);
  else if (scene == SCENE_KIBOMBO_STATUE_GROUNDS_CEREMONY)
    Task_Remove(Kibombo_FlickerCeremonyFires);
  else if (scene == SCENE_GABOMBA_STATUE_FACE)
    Task_Remove(Kibombo_FlickerStatueFires);
}

/// Felix walks off the village or the statue's grounds by one of their ways
/// out on the ceremony night: the village's south edge to the world map, its
/// north and north-west paths up to the statue's grounds, or the grounds'
/// south and south-west paths back down. The torches stop flickering, the exit
/// sound plays and the screen closes.
void Kibombo_LeaveRoom(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Get(FLAG_KIBOMBO_FIRES_FLICKER))
    Kibombo_StopFireFlicker();
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->stepTrigger);
}

/// Felix walks into the Gabomba Statue's mouth on the statue's grounds, cell
/// (16, 19): the screen closes in on him and he goes inside, the torches
/// going still as he leaves.
void Kibombo_LeaveRoomFading(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Kibombo_LeaveRoom();
}

/// Felix walks north into the door of one of the village's six houses: he
/// lines up with it and steps into the dark doorway, the screen fades, and
/// he stands inside the house.
void Kibombo_EnterDoor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader;
  FIXED x;
  s32 pixelX;

  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  x = leader->x.fixed;
  pixelX = FIXEDtoINT(x);
  Actor_SetMoveTarget(leader, toFIXED(8 - pixelX % 16) + x, leader->y.fixed,
                      leader->z.fixed);
  Actor_WaitForMove(leader);
  leader->facing = FACING_NORTH;
  if (GameFlag_Get(FLAG_KIBOMBO_FIRES_FLICKER))
    Kibombo_StopFireFlicker();
  Actor_SetMovementTypeById(gMainRoutineWork.leaderActorId,
                            gKibombo_EnterDoorScript);
  wait(12);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->stepTrigger);
}

/// The leader walks to the top of the ladder at x, z, climbs down it and the
/// scene leaves by the step's exit.
void Kibombo_ClimbDownLadder(s32 x, s32 z) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_GetById(party->leaderActorId);

  Event_Start();
  Event_ResolvePendingActions(0);
  leader->motionFlags = 0;
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, x, z);
  Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                    FACING_NORTH);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId,
                         ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
  FieldActor_SetMoveTarget(leader, x, -12, z);
  Actor_WaitForMove(leader);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_LEADER_CLIMB);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->stepTrigger);
}

/// Felix steps beside the ladder in the item shop's upper room, cell (43, 22)
/// or (42, 23): he climbs down it only while he presses on toward it, Left or
/// Up.
void Kibombo_StepOntoFirstLadder(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 x = FIELD_CELL(leader->x.fixed);
  s32 z = FIELD_CELL(leader->z.fixed);

  if (x == 42 && z == 23 && (gInput.held & KEY_UP) == 0)
    return;
  if (x == 43 && z == 22 && (gInput.held & KEY_LEFT) == 0)
    return;
  Kibombo_ClimbDownLadder(680, 352);
}

#define DEFINE_KIBOMBO_STEP_WHEN_HELD(name, sideZ, westX, eastX, southX,       \
                                      southZ, ladderX, ladderZ)                \
  void name(void) {                                                            \
    struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);      \
    s32 x = FIELD_CELL(leader->x.fixed);                                       \
    s32 z = FIELD_CELL(leader->z.fixed);                                       \
                                                                               \
    if (z == sideZ) {                                                          \
      if (x == westX && (gInput.held & KEY_RIGHT) == 0)                        \
        return;                                                                \
      if (x == eastX && (gInput.held & KEY_LEFT) == 0)                         \
        return;                                                                \
    } else if (x == southX && z == southZ && (gInput.held & KEY_UP) == 0) {    \
      return;                                                                  \
    }                                                                          \
    Kibombo_ClimbDownLadder(ladderX, ladderZ);                                 \
  }

/// As Kibombo_StepOntoFirstLadder, for the ladder down from Akafubu's home to
/// the Sanctum, cells (23, 38), (25, 38) and (24, 39): Felix may step onto it
/// from the west, the east or the south.
DEFINE_KIBOMBO_STEP_WHEN_HELD(Kibombo_StepOntoSecondLadder, 38, 23, 25, 24, 39,
                              392, 608)

/// As Kibombo_StepOntoSecondLadder, for the ladder in the upper room with two
/// beds, cells (42, 54), (44, 54) and (43, 55), down to the room of beds
/// below.
DEFINE_KIBOMBO_STEP_WHEN_HELD(Kibombo_StepOntoThirdLadder, 54, 42, 44, 43, 55,
                              696, 864)

/// Felix walks south into the uncovered vent on the statue's grounds, cell
/// (23, 17): he climbs down its ladder into the Gabomba Statue.
void Kibombo_EnterVent(void) {
  if (GameFlag_Get(FLAG_KIBOMBO_FIRES_FLICKER))
    Kibombo_StopFireFlicker();
  Kibombo_ClimbDownLadder(376, 288);
}

/// During the ceremony the screen shows the Great Gabomba's face close up, its
/// fires flickering, for about three seconds or until A, B or Start is held,
/// then goes dark and returns to the grounds.
///
/// The flicker is `Kibombo_FlickerStatueFires`, run for up to 160 frames after
/// a wait of 20.
void Kibombo_GazeAtStatue(void) {
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Task_Add(Kibombo_FlickerStatueFires, TASK_PASS_FRAME);
  Event_Wait(20);
  for (i = 0; i < 160; i++) {
    Event_Wait(1);
    if (gInput.held & (KEY_A | KEY_B | KEY_START))
      break;
  }
  Task_Remove(Kibombo_FlickerStatueFires);
  gFieldWork->transitionFrames = 32;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(
      GABOMBA_STATUE_FACE_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_B);
}

/// During the ceremony, as the Great Gabomba answers, the screen flashes white
/// with a sound and its face is shown close up with its eyes shut, the fires
/// on it flickering. It flashes again, and the face has its eyes half open;
/// and again, and the eyes are wide open, staring. After a while the screen
/// goes dark and the party is back on the grounds. Each view is a copy of the
/// face in the map, at (248, 248), (584, 248) and (248, 536).
void Kibombo_StatueFlashes(void) {
  Sound_Play(SE_POWER_UP);
  PaletteFade_ApplyTarget(RGB_WHITE, 0);
  PaletteFade_StartInterpolation(1);
  wait(1);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 0);
  gFieldWork->transitionFrames = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldTask_AddFrame(Kibombo_FlickerStatueFires, TASK_PASS_FRAME);
  wait(60);
  Task_Remove(Kibombo_FlickerStatueFires);
  wait(1);

  Sound_Play(SE_POWER_UP);
  PaletteFade_ApplyTarget(RGB_WHITE, 0);
  PaletteFade_StartInterpolation(1);
  wait(1);
  FieldCamera_MoveTo(584, CAMERA_KEEP, 248, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(1);
  wait(1);
  FieldTask_AddFrame(Kibombo_FlickerStatueFires, TASK_PASS_FRAME);
  wait(60);
  Task_Remove(Kibombo_FlickerStatueFires);
  wait(1);

  Sound_Play(SE_POWER_UP);
  PaletteFade_SetTarget(RGB_WHITE, 0);
  PaletteFade_StartInterpolation(1);
  wait(1);
  FieldCamera_MoveTo(248, CAMERA_KEEP, 536, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(1);
  wait(1);
  Task_Add(Kibombo_FlickerStatueFires, TASK_PASS_FRAME);
  wait(180);
  Task_Remove(Kibombo_FlickerStatueFires);
  wait(1);

  gFieldWork->transitionFrames = 1;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  Import_EventSetExitRequest(
      GABOMBA_STATUE_FACE_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_A);
}

/// The sacred night begins: Akafubu calls on the village to pray for his
/// success, and the party hurries to watch.
void Kibombo_CeremonyStarts(void) {
  struct Actor *leader;

  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  Event_SetMessageId((s32)MsgAtLastThe);
  // "At last, the sacred night has come!"
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 86, 455);
  Actor_SetPosition(ACTOR_KRADEN, 86, 455);
  wait(1);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkToAndWait(ACTOR_KRADEN, 72, 455);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(264, 16, 420, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(264, 32, 320, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(80);
  Actor_Jump(ACTOR_AKAFUBU, 4, 40);
  // "Tonight, Akafubu becomes the Kibombo's new witch doctor!"
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(264, 32, 400, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST, 20);
  // "All of you, pray in the depths of your hearts for my success!"
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  Sound_Play(MUS_RITUALS);
  GameFlag_Set(FLAG_GABOMBA_CEREMONY_UNDER_WAY);
  Sound_Play(SE_BOSS_ROAR);
  Actor_GetById(ACTOR_WARRIOR_A)->velocityY = toFIXED(4);
  wait(5);
  Actor_GetById(ACTOR_WARRIOR_B)->velocityY = toFIXED(4);
  wait(5);
  Actor_GetById(ACTOR_WARRIOR_C)->velocityY = toFIXED(4);
  wait(5);
  Actor_GetById(ACTOR_WARRIOR_D)->velocityY = toFIXED(4);
  wait(5);
  Actor_GetById(ACTOR_WARRIOR_E)->velocityY = toFIXED(4);
  wait(5);
  Actor_GetById(ACTOR_WARRIOR_F)->velocityY = toFIXED(4);
  wait(5);
  Actor_GetById(ACTOR_WARRIOR_G)->velocityY = toFIXED(4);
  wait(5);
  Actor_GetById(ACTOR_WARRIOR_H)->velocityY = toFIXED(4);
  wait(20);
  Actor_FaceDirectionById(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 20);
  Kibombo_CrowdBows();
  Event_Wait(40);
  FieldCamera_SetSpeed(0.2);
  FieldCamera_MoveTo(264, 0, 400, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(200);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(111, 32, 456, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  // "It looks like the initiation ceremony is beginning."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_WEST, 20);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "I'm not sure what will happen, but we'd better hurry, Felix."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_KRADEN, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  GameFlag_Set(FLAG_KIBOMBO_CEREMONY_BEGUN);
}

void Kibombo_CarryBlackCrystal(void);

/// Akafubu presents the Black Orb to the Great Gabomba, and the statue notices
/// its light.
void Kibombo_PresentOrbToGabomba(void) {
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId),
                       SPRITE_COLOR_HIDDEN);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldTask_AddFrame(Kibombo_FlickerCeremonyFires, TASK_PASS_FRAME);
  Event_Wait(40);
  Sound_Play(SOUND_FADE_OUT_BGM);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(264, CAMERA_KEEP, 388, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_AKAFUBU, ANIM_NOD);
  Event_SetMessageId((s32)MsgLetUsNow);
  // "Let us now present our jewel to the Great Gabomba!"
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  Sound_Play(MUS_JUPITER_LIGHTHOUSE);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CRYSTAL_MARKER), 0);
  Actor_GetById(ACTOR_CRYSTAL_MARKER)->motionFlags = 0;
  Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_CRYSTAL_MARKER),
                                  toFIXED(265), toFIXED(24), toFIXED(344));
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(264, CAMERA_KEEP, 340, 1);
  Actor_SetSpeed(ACTOR_AKAFUBU, 0.2);
  Actor_WalkToAndWaitById(ACTOR_AKAFUBU, 264, 374);
  wait(1);
  FieldTask_AddFrame(Kibombo_CarryBlackCrystal, TASK_PASS_FRAME);
  Event_Wait(20);
  Task_Remove(Kibombo_FlickerCeremonyFires);
  Kibombo_AkafubuCastsOnCrystal();
  FieldTask_AddFrame(Kibombo_FlickerCeremonyFires, TASK_PASS_FRAME);
  // "O Great Gabomba! Please accept this, our humble gift!"
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Sound_Play(MUS_RITUALS);
  Kibombo_EndOffering();
  Event_Wait(160);
  // "Please, Great Gabomba! Accept this jewel!"
  Event_ShowMessageThenWait(ACTOR_AKAFUBU, 0, 80);
  Sound_Play(SE_STATUE_STIRS);
  Kibombo_SetStatueMouth(1, 1);
  Event_Wait(20);
  // "Look! The Great Gabomba has noticed the light of the jewel!"
  Event_ShowMessageThenWait(ACTOR_AKAFUBU, 0, 40);
  Task_Remove(Kibombo_FlickerCeremonyFires);
  wait(1);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(
      KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_ARROWS_NORTH_A);
}

/// The Great Gabomba stays silent, and Akafubu asks whether it is still not
/// pleased.
void Kibombo_GabombaNotPleased(void) {
  struct Actor *actor;

  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId),
                       SPRITE_COLOR_HIDDEN);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CRYSTAL_MARKER), 0);
  Actor_GetById(ACTOR_CRYSTAL_MARKER)->motionFlags = 0;
  Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_CRYSTAL_MARKER),
                                  toFIXED(265), toFIXED(24), toFIXED(344));
  Kibombo_SetStatueMouth(1, 1);
  wait(1);
  if (gKibombo_BlackCrystal != 0) {
    gKibombo_BlackCrystal->y.fixed += toFIXED(32);
    wait(1);
    gKibombo_BlackCrystal->motionFlags = ACTOR_FLOATS;
  }
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Task_Add(Kibombo_FlickerCeremonyFires, TASK_PASS_FRAME);
  Event_Wait(40);
  Sound_Play(SE_RATTLE);
  Kibombo_SetStatueMouth(0, 1);
  Event_Wait(20);
  FieldEffect_SetAttached(ACTOR_AKAFUBU,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_SetMessageId((s32)MsgYouAreStill);
  // "You are still not pleased?"
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  actor = gKibombo_BlackCrystal;
  if (actor != 0) {
    actor->motionFlags = 0;
    while (actor->y.fixed > toFIXED(36)) {
      Actor_FaceDirection(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 0);
      gKibombo_BlackCrystal->y.fixed -= toFIXED(1) / 4;
      wait(1);
      actor = gKibombo_BlackCrystal;
    }
  }
  Actor_SetAnimationAndWait(ACTOR_AKAFUBU, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  GameFlag_Clear(FLAG_GABOMBA_MUSIC_SILENCED);
  Sound_Play(MUS_RITUALS);
  Event_Wait(80);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(
      KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_ARROWS_NORTH_B);
}

/// Akafubu feels the witch doctor's power fill his body.
void Kibombo_WitchDoctorFeelsPower(void) {
  struct FieldWork *work;
  s32 effect;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId),
                       SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  Kibombo_CrowdBows();
  Task_Add(Kibombo_FlickerCeremonyFires, TASK_PASS_FRAME);
  work = Ram_HeapSlots->fieldWork;
  work->transition = effect =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Event_SetMessageId((s32)MsgOhTheMagic);
  // "Oh! The magic, the power of the witch doctor fills my body!"
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHWEST);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  Task_Remove(Kibombo_FlickerCeremonyFires);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST);
  PaletteFade_SetSource(PALETTE_TINT_NONE, 0);
  PaletteFade_SetTarget(0, 0);
  gFieldWork->transition = effect;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_GABOMBA_BACK_FROM_CEREMONY);
  GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  Import_EventSetExitRequest(
      KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_ARROWS_SOUTH);
}

/// The screen opens on the ceremony at the statue, with Felix hidden, the
/// fires flickering and the Great Gabomba's mouth open, its eyes ringed with
/// light; each step of the offering that follows sets the mouth as it needs.
void Kibombo_OpenCeremonyScene(void) {
  struct FieldWork *work;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId),
                       SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  Task_Add(Kibombo_FlickerCeremonyFires, TASK_PASS_FRAME);
  Kibombo_SetStatueMouth(1, 1);
  if (gKibombo_BlackCrystal != 0) {
    gKibombo_BlackCrystal->y.fixed += toFIXED(32);
    wait(1);
    gKibombo_BlackCrystal->motionFlags = ACTOR_FLOATS;
  }
  wait(1);
  work = Ram_HeapSlots->fieldWork;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
}

extern s32 gKibombo_CrowdDanceScript[];

/// The warriors jump and cheer, then dance.
void Kibombo_CrowdCheers(void) {
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(264, 0, 400, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_Jump(ACTOR_WARRIOR_A, 6, 0);
  Actor_Jump(ACTOR_WARRIOR_B, 6, 0);
  Actor_Jump(ACTOR_WARRIOR_C, 6, 0);
  Actor_Jump(ACTOR_WARRIOR_D, 6, 0);
  Actor_Jump(ACTOR_WARRIOR_E, 6, 0);
  Actor_Jump(ACTOR_WARRIOR_F, 6, 0);
  Actor_Jump(ACTOR_WARRIOR_G, 6, 0);
  Actor_Jump(ACTOR_WARRIOR_H, 6, 40);
  Actor_FaceDirectionAndWait(ACTOR_WARRIOR_C, FACING_SOUTH_SOUTHWEST);
  Actor_Jump(ACTOR_WARRIOR_C, 2, 0);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_WARRIOR_C, 0);
  Actor_FaceDirectionAndWait(ACTOR_WARRIOR_F, FACING_SOUTH_SOUTHEAST);
  Actor_Jump(ACTOR_WARRIOR_F, 2, 0);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_WARRIOR_F, 0);
  Actor_SetMovementTypeById(ACTOR_WARRIOR_A, gKibombo_CrowdDanceScript);
  Actor_SetMovementTypeById(ACTOR_WARRIOR_B, gKibombo_CrowdDanceScript);
  Actor_SetMovementTypeById(ACTOR_WARRIOR_C, gKibombo_CrowdDanceScript);
  Actor_SetMovementTypeById(ACTOR_WARRIOR_D, gKibombo_CrowdDanceScript);
  Actor_SetMovementTypeById(ACTOR_WARRIOR_E, gKibombo_CrowdDanceScript);
  Actor_SetMovementTypeById(ACTOR_WARRIOR_F, gKibombo_CrowdDanceScript);
  Actor_SetMovementTypeById(ACTOR_WARRIOR_G, gKibombo_CrowdDanceScript);
  Actor_SetMovementTypeById(ACTOR_WARRIOR_H, gKibombo_CrowdDanceScript);
  Event_Wait(40);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
  Actor_Jump(ACTOR_AKAFUBU, 2, 10);
  Actor_Jump(ACTOR_AKAFUBU, 4, 10);
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Actor_Stop(ACTOR_WARRIOR_A);
  Actor_Stop(ACTOR_WARRIOR_B);
  Actor_Stop(ACTOR_WARRIOR_C);
  Actor_Stop(ACTOR_WARRIOR_D);
  Actor_Stop(ACTOR_WARRIOR_E);
  Actor_Stop(ACTOR_WARRIOR_F);
  Actor_Stop(ACTOR_WARRIOR_G);
  Actor_Stop(ACTOR_WARRIOR_H);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_WARRIOR_A, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_WARRIOR_B, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_WARRIOR_C, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_WARRIOR_D, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_WARRIOR_E, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_WARRIOR_F, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_WARRIOR_G, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_WARRIOR_H, FACING_NORTH_NORTHWEST, 0);
  Actor_ShowEmoteById(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_AKAFUBU, 0);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_AKAFUBU, 0);
  Actor_SetAnimationAndWait(ACTOR_AKAFUBU, ANIM_NOD);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_AKAFUBU, 0);
  Actor_Jump(ACTOR_AKAFUBU, 4, 10);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_AKAFUBU, 0);
}

/// Still nothing from the statue: the warriors call for more drums.
void Kibombo_GabombaStillNotPleased(void) {
  struct Actor **slot;
  struct Actor *actor;

  Kibombo_OpenCeremonyScene();
  Sound_Play(SE_RATTLE);
  Kibombo_SetStatueMouth(0, 1);
  Event_Wait(20);
  Event_SetMessageId((s32)MsgYouAreStillNot);
  // "You are still not pleased?"
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  actor = gKibombo_BlackCrystal;
  if (actor != 0) {
    actor->motionFlags = 0;
    if (actor->y.fixed > toFIXED(36)) {
      do {
        Actor_FaceDirectionById(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 0);
        slot = &gKibombo_BlackCrystal;
        actor = *slot;
        actor->y.fixed += toFIXED(-0.25);
        wait(1);
      } while ((*slot)->y.fixed > toFIXED(36));
    }
  }
  Actor_SetAnimationAndWait(ACTOR_AKAFUBU, ANIM_SHAKE_HEAD);
  // "No! This time, he seemed almost like he might..."
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
  // "We must continue! Drums! More drums!"
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  GameFlag_Set(FLAG_GABOMBA_BACK_FROM_CEREMONY);
  GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  Import_EventSetExitRequest(
      KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_ARROWS_SOUTH);
}

/// The warriors cry that the Great Gabomba has seen their jewel.
void Kibombo_GabombaSeesOrb(void) {
  Kibombo_OpenCeremonyScene();
  Sound_Play(SE_STATUE_STIRS);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_LEFT_EYE),
                       SPRITE_COLOR_WHITE);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_RIGHT_EYE),
                       SPRITE_COLOR_WHITE);
  wait(8);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_LEFT_EYE),
                       SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_RIGHT_EYE),
                       SPRITE_COLOR_NORMAL);
  wait(8);
  Actor_ShowEmoteById(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_SetMessageId((s32)MsgLookLookThe);
  // "Look! Look! The Great Gabomba has seen our jewel!"
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Sound_Play(SE_GABOMBA_JAWS_OPEN);
  Actor_SetMovementTypeById(ACTOR_STATUE_LEFT_EYE, gKibombo_EyesPopScript);
  Actor_SetMovementTypeAndWaitById(ACTOR_STATUE_RIGHT_EYE,
                                   gKibombo_EyesPopScript);
  Actor_SetMovementTypeById(ACTOR_STATUE_LEFT_EYE, gKibombo_EyesShakeScript);
  Actor_SetMovementTypeById(ACTOR_STATUE_RIGHT_EYE, gKibombo_EyesShakeScript);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_LEFT_EYE), SPRITE_COLOR_DARK);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_RIGHT_EYE),
                       SPRITE_COLOR_DARK);
  Event_Wait(120);
  Kibombo_CrowdCheers();
  GameFlag_Set(FLAG_GABOMBA_BACK_FROM_CEREMONY);
  GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  Import_EventSetExitRequest(
      KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_ARROWS_SOUTH);
}

/// The warriors cry that the Great Gabomba has responded to their gift.
void Kibombo_GabombaResponds(void) {
  Kibombo_OpenCeremonyScene();
  Event_PlaySound(SE_STATUE_STIRS);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_LEFT_EYE),
                       SPRITE_COLOR_WHITE);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_RIGHT_EYE),
                       SPRITE_COLOR_WHITE);
  wait(8);
  Actor_GetById(ACTOR_STATUE_LEFT_EYE)->update =
      (ObjectUpdate)Kibombo_FlashSprite;
  Actor_GetById(ACTOR_STATUE_RIGHT_EYE)->update =
      (ObjectUpdate)Kibombo_FlashSprite;
  Actor_ShowEmoteById(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_SetMessageId((s32)MsgLookTheGreat);
  // "Look! The Great Gabomba has responded to our gift!"
  Event_ShowMessageThenWait(ACTOR_AKAFUBU, 0, 20);
  Sound_Play(SE_STATUE_STIRS);
  Kibombo_SetStatueTongue(1, 0);
  Event_Wait(20);
  Sound_Play(SE_WAVE);
  Task_Add(Kibombo_RaiseEmbers, TASK_PASS_FRAME);
  Event_Wait(120);
  Kibombo_CrowdCheers();
  GameFlag_Set(FLAG_GABOMBA_BACK_FROM_CEREMONY);
  GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  Import_EventSetExitRequest(
      KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_ARROWS_SOUTH);
}

/// The Great Gabomba responds to the gift: its tongue reaches for it and its
/// mouth opens and shuts.
void Kibombo_GabombaRespondsToGift(void) {
  struct Actor *actor;
  u32 i;

  actor = Actor_GetById(ACTOR_STATUE_TONGUE);
  Kibombo_OpenCeremonyScene();
  Actor_SetSpritePriority(ACTOR_AKAFUBU, 1);
  Event_PlaySound(SE_STATUE_STIRS);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_LEFT_EYE),
                       SPRITE_COLOR_WHITE);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_RIGHT_EYE),
                       SPRITE_COLOR_WHITE);
  wait(8);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_LEFT_EYE),
                       SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_RIGHT_EYE),
                       SPRITE_COLOR_NORMAL);
  wait(8);
  Actor_ShowEmoteById(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_SetMessageId((s32)MsgLookTheGreatGabomba);
  // "Look! The Great Gabomba has responded to our gift!"
  Event_ShowMessageThenWait(ACTOR_AKAFUBU, 0, 20);
  Event_PlaySound(SE_STATUE_STIRS);
  Kibombo_SetStatueTongue(1, 0);
  Event_Wait(20);
  Sound_Play(SE_CART_ROLL);
  actor->scaleY = FIXED_ONE / 10;
  Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_STATUE_TONGUE),
                                  toFIXED(264), toFIXED(24), toFIXED(308));
  wait(1);
  Actor_SetAnimationById(ACTOR_STATUE_TONGUE, 3);
  for (i = 0; i < 64; i++) {
    actor->scaleY += FIXED_ONE / 64;
    actor->y.fixed += toFIXED(1) / 8;
    wait(1);
  }
  Sound_Play(SE_STOP_EXIT_SE);
  if (gKibombo_BlackCrystal != 0) {
    struct Actor *gift = gKibombo_BlackCrystal;

    gift->motionFlags = 0;
    if (gift->y.fixed > toFIXED(36)) {
      do {
        gKibombo_BlackCrystal->y.fixed -= toFIXED(1) / 2;
        wait(1);
      } while (gKibombo_BlackCrystal->y.fixed > toFIXED(36));
    }
  }
  Sound_Play(SE_CRUNCH);
  if (gKibombo_BlackCrystal != 0) {
    gKibombo_BlackCrystal->motionFlags = 0;
    for (i = 0; i < 4; i++) {
      gKibombo_BlackCrystal->y.fixed += toFIXED(6);
      actor->scaleY -= FIXED_ONE / 8;
      actor->y.fixed -= toFIXED(1) / 2;
      wait(1);
    }
    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, 0, 0, 0);
    for (i = 0; i < 4; i++) {
      actor->scaleY -= FIXED_ONE / 8;
      actor->y.fixed -= toFIXED(1) / 2;
      wait(1);
    }
  }
  Actor_SetPosition(ACTOR_STATUE_TONGUE, 0, 0);
  Kibombo_SetStatueTongue(0, 0);
  Kibombo_SetStatueMouth(1, 1);
  Event_Wait(10);
  Kibombo_SetStatueMouth(0, 1);
  Event_Wait(40);
  Event_PlaySound(SE_STATUE_STIRS);
  Kibombo_SetStatueMouth(1, 1);
  Kibombo_SetStatueTongue(1, 0);
  Sound_Play(SE_WATER);
  Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_STATUE_TONGUE),
                                  toFIXED(264), toFIXED(24), toFIXED(308));
  wait(1);
  for (i = 0; i < 8; i++) {
    actor->scaleY += FIXED_ONE / 8;
    actor->y.fixed += toFIXED(1) / 2;
    wait(1);
  }
  FieldEffect_SetAttached(ACTOR_AKAFUBU,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  FieldActor_ShowEmote(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  Actor_SetAnimationAndWait(ACTOR_AKAFUBU, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  Actor_SetSpeed(ACTOR_AKAFUBU, 0.8);
  Actor_WalkToAndWait(ACTOR_AKAFUBU, 264, 320);
  Actor_SetPosition(ACTOR_AKAFUBU, 0, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_GABOMBA_RESPONDED);
  Import_EventSetExitRequest(
      KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_CHAMBER_IDOL_NORTH);
}

/// After the ceremony, at Akafubu's home: Kraden, Akafubu and his father talk
/// it over, and Kraden walks out with the party.
void Kibombo_CeremonyEnds(void) {
  struct FieldWork *work;
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_MoveTo(384, CAMERA_KEEP, 601, 0);
  wait(1);
  Actor_SetHorizontalPositionAndAngleWithTerrain(
      gMainRoutineWork.leaderActorId, toFIXED(370), toFIXED(636), FACING_NORTH);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  PaletteFade_SetSource(0, 0);
  PaletteFade_SetTarget(PALETTE_TINT_SEPIA, 0);
  gFieldWork->transitionFrames = 60;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_KRADEN_AT_HOME, FACING_NORTH_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN_AT_HOME, FACING_NORTH_NORTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN_AT_HOME, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_FATHER_OF_AKAFUBU, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_AKAFUBU_AT_HOME, ANIM_NOD);
  Actor_FaceDirection(ACTOR_KRADEN_AT_HOME, FACING_NORTH_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN_AT_HOME, FACING_NORTH_NORTHEAST, 20);
  Actor_GetById(ACTOR_KRADEN_AT_HOME)->velocityY = toFIXED(3);
  Event_Wait(40);
  Actor_SetAnimationById(ACTOR_FATHER_OF_AKAFUBU, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_AKAFUBU_AT_HOME, ANIM_NOD);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(40);
  wait(60);
  Actor_SetAnimationAndWait(ACTOR_KRADEN_AT_HOME, ANIM_NOD);
  Event_SetMessageId((s32)MsgRightWellI);
  // "Right... Well, I guess that's that!"
  Event_ShowMessageAndWait(ACTOR_KRADEN_AT_HOME, 0);
  Actor_SetAnimationAndWait(ACTOR_FATHER_OF_AKAFUBU, ANIM_NOD);
  // "You are virtuous men, coming here to deliver this message. My son..."
  Event_ShowMessageAndWait(ACTOR_FATHER_OF_AKAFUBU, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN_AT_HOME, FACING_EAST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_KRADEN_AT_HOME, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 10);
  Actor_SetAnimationById(ACTOR_KRADEN_AT_HOME, ANIM_NOD);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_Jump(ACTOR_AKAFUBU_AT_HOME, 4, 10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_AKAFUBU_AT_HOME, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_FATHER_OF_AKAFUBU, FACING_EAST);
  Actor_ShowEmoteById(ACTOR_AKAFUBU_AT_HOME, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                      20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_AKAFUBU_AT_HOME, 0);
  Actor_ShowEmoteById(ACTOR_FATHER_OF_AKAFUBU,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessageAndWait(ACTOR_FATHER_OF_AKAFUBU, 0);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU_AT_HOME, FACING_WEST);
  Actor_SetAnimationById(ACTOR_AKAFUBU_AT_HOME, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_AKAFUBU_AT_HOME, 0);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU_AT_HOME, FACING_SOUTH_SOUTHWEST);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_AKAFUBU_AT_HOME, 0);
  Actor_SetAnimationById(ACTOR_AKAFUBU_AT_HOME, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_AKAFUBU_AT_HOME, 0);
  Actor_FaceDirection(ACTOR_FATHER_OF_AKAFUBU, FACING_NORTH_NORTHEAST, 0);
  Actor_SetSpeed(ACTOR_AKAFUBU_AT_HOME, 1.6);
  Actor_WalkToAndWait(ACTOR_AKAFUBU_AT_HOME, 364, 576);
  Actor_FaceDirection(ACTOR_AKAFUBU_AT_HOME, FACING_NORTH_NORTHWEST, 0);
  Actor_ShowEmoteById(ACTOR_FATHER_OF_AKAFUBU, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                      80);
  Event_ShowMessageAndWait(ACTOR_FATHER_OF_AKAFUBU, 0);
  Actor_FaceDirectionAndWait(ACTOR_FATHER_OF_AKAFUBU, FACING_SOUTH_SOUTHEAST);
  Event_ShowMessageAndWait(ACTOR_FATHER_OF_AKAFUBU, 0);
  Actor_SetAnimationById(ACTOR_FATHER_OF_AKAFUBU, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_FATHER_OF_AKAFUBU, 0);
  Event_ShowMessageAndWait(ACTOR_FATHER_OF_AKAFUBU, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN_AT_HOME, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_KRADEN_AT_HOME, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN_AT_HOME, FACING_EAST);
  Event_ShowMessageAndWait(ACTOR_KRADEN_AT_HOME, 0);
  Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                    FACING_WEST);
  Actor_SetAnimationById(ACTOR_KRADEN_AT_HOME, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_KRADEN_AT_HOME, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetSpeed(ACTOR_KRADEN_AT_HOME, 0.8);
  Actor_SetAnimationById(ACTOR_KRADEN_AT_HOME, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_KRADEN_AT_HOME, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_KRADEN_AT_HOME);
  Actor_SetPosition(ACTOR_KRADEN_AT_HOME, 0, 0);
  work = Ram_HeapSlots->fieldWork;
  work->transitionFrames = 16;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  GameFlag_Set(FLAG_GABOMBA_CEREMONY_ENDED);
  Event_End();
}

/// Sets up whichever of the overlay's six rooms the party enters.
s32 Scene_Initialize(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_KIBOMBO)
    Kibombo_InitializeVillageByDay();
  else if (scene == SCENE_KIBOMBO_CEREMONY)
    Kibombo_InitializeCeremony();
  else if (scene == SCENE_KIBOMBO_STATUE_GROUNDS)
    Kibombo_InitializeStatueGroundsByDay();
  else if (scene == SCENE_KIBOMBO_STATUE_GROUNDS_CEREMONY)
    Kibombo_InitializeStatueGroundsCeremony();
  else if (scene == SCENE_GABOMBA_STATUE_FACE)
    Kibombo_InitializeStatueFace();
  else if (scene == SCENE_KIBOMBO_HOUSES)
    Kibombo_InitializeHouses();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// The village: the frozen puddles, the rope and the roofs.
void Kibombo_InitializeVillage(void) {
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANDLESTICK_A), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANDLESTICK_B), 0);
  FieldEffect_StartBlankOverlay(0);
  FlagBlock_PlaceAll(gKibombo_FrozenPuddle);
  Actor_SetSpritePriority(ACTOR_PUDDLE, 3);
  Rope_Initialize();
  Rope_Place(0, FLAG_KIBOMBO_VILLAGE_ROPE_TIED, ACTOR_ROPE_POST_A,
             ACTOR_ROPE_POST_B);
  Actor_GetById(ACTOR_GATE_GUARD_A)->collisionFlags |= ACTOR_BLOCKED_BY_SOLID;
  Actor_GetById(ACTOR_GATE_GUARD_B)->collisionFlags |= ACTOR_BLOCKED_BY_SOLID;
}

/// The village by day: its signal fire smoking and the row of flames lit.
void Kibombo_InitializeVillageByDay(void) {
  Actor_SetPosition(ACTOR_FLAME, 88, 314);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FLAME), 0);
  Actor_GetById(ACTOR_FLAME)->scaleY = FIXED_ONE / 2;
  Kibombo_InitializeVillage();
  GameFlag_Clear(FLAG_KIBOMBO_AT_CEREMONY);
  Actor_DisableById(ACTOR_SIGNAL_FIRE);
  Actor_SetAnimationById(ACTOR_SIGNAL_FIRE, 2);
  Task_Add(Kibombo_RaiseSmoke, TASK_PASS_FRAME);
  Kibombo_LightSignalFire();
}

struct Unit *Unit_Get(u32 unitId);

/// The village on the ceremony night: the signal fire's flame is raised, and
/// when the party starts again before the healer after a defeat with Piers
/// along, he is healed and stays behind. The fires' colours are kept for their
/// flicker, the candlesticks lit and, once Akafubu has offered his help, the
/// ceremony's actors moved; the screen fades in with the village fires
/// flickering.
void Kibombo_InitializeCeremony(void) {
  struct Actor *actor = Actor_GetById(ACTOR_FLAME);
  struct Unit *unit;
  s32 done;

  Event_Start();
  Event_ResolvePendingActions(0);
  done = GameFlag_Get(FLAG_KEEP_PARTY_POSITION);
  if (!done) {
    actor->motionFlags = done;
    actor->y.fixed = toFIXED(42);
    actor->z.fixed -= toFIXED(42);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FLAME), 0);
    Actor_SetSpritePriority(ACTOR_FLAME, 1);
    if (gMainRoutineWork.entrance == ENTRANCE_CEREMONY_AFTER_DEFEAT &&
        GameFlag_Get(FLAG_IN_PARTY(ACTOR_PIERS))) {
      unit = Unit_Get(PIERS);
      unit->hp = unit->hpMax;
      unit->pp = unit->ppMax;
      Unit_UpdateGauges(unit);
      unit->poison = 0;
      unit->haunt = 0;
      GameFlag_Set(FLAG_KIBOMBO_PIERS_STAYED_BEHIND);
      GameFlag_Clear(FLAG_KIBOMBO_PIERS_WITH_PARTY);
      Party_RemoveMemberRestored(PIERS);
    }
  }
  GameFlag_Set(FLAG_KIBOMBO_FIRES_FLICKER);
  gKibombo_FireColors[0] = *(u16 *)&BG_PLTT[FIRE_COLOR_FIRST];
  gKibombo_FireColors[1] = *(u16 *)&BG_PLTT[VILLAGE_FIRE_COLOR_SECOND];
  gKibombo_FireColors[2] = *(u16 *)&BG_PLTT[VILLAGE_FIRE_COLOR_THIRD];
  gKibombo_FireColors[3] = *(u16 *)&BG_PLTT[VILLAGE_FIRE_COLOR_FOURTH];
  Kibombo_InitializeVillage();
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LIT_CANDLESTICK_A), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LIT_CANDLESTICK_B), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LIT_CANDLESTICK_C), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LIT_CANDLESTICK_D), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LIT_CANDLESTICK_E), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LIT_CANDLESTICK_F), 0);
  GameFlag_Set(FLAG_KIBOMBO_DOORS_SHUT);
  actor = Actor_GetById(ACTOR_DOOR_BLOCK);
  actor->motionFlags = 0;
  actor->y.fixed = 0;
  Actor_Hide(ACTOR_DOOR_BLOCK);
  Actor_SetSpritePriority(ACTOR_LIT_CANDLESTICK_A, 3);
  if (GameFlag_Get(FLAG_AKAFUBU_OFFERED_HELP)) {
    Actor_SetPosition(ACTOR_WOMAN_NO_PUSHING, 0, 0);
    Actor_SetPosition(ACTOR_AFRAID_MAN, 0, 0);
    Actor_SetPosition(ACTOR_MAN_WHO_KNEW_OEIA, 0, 0);
    Actor_SetPosition(ACTOR_GIRL_WHO_HOPES, 302, 172);
  }
  GameFlag_Set(FLAG_KIBOMBO_AT_CEREMONY);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  {
    s32 priority = TASK_PASS_FRAME;

    Task_Add(Kibombo_FlickerVillageFires, priority);
  }
  Event_End();
}

void Kibombo_CreateBlackCrystal(void);

/// The grounds before the statue: the Black Crystal, the marks uncovered, the
/// vent and the pillar Moved for Piers.
void Kibombo_InitializeStatueGrounds(void) {
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANDLESTICK_A), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANDLESTICK_B), 0);
  if (!GameFlag_Get(FLAG_GABOMBA_RESPONDED))
    Kibombo_CreateBlackCrystal();
  Kibombo_HideGroundsBlocks();
  if (!GameFlag_Get(FLAG_SAW_THE_GROUNDS_CEREMONY))
    GameFlag_Set(FLAG_KIBOMBO_PUDDLE_FROZEN);
  FlagBlock_PlaceAll(gKibombo_FrozenPuddle);
  Actor_SetSpritePriority(ACTOR_PUDDLE, 2);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_GROUNDS_PILLAR), 0);
  Actor_GetById(ACTOR_GROUNDS_PILLAR)->motionFlags = 0;
  if (GameFlag_Get(FLAG_KIBOMBO_PILLAR_MOVED_FOR_PIERS)) {
    FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_GROUNDS_PILLAR),
                                         toFIXED(104), 0, toFIXED(296));
    MapGrid_CopyCellAttributes(5, 18, 1, 1, 6, 18);
    wait(10);
  }
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  if (GameFlag_Get(FLAG_KIBOMBO_VENT_UNCOVERED))
    Kibombo_UncoverVent();
  if (GameFlag_Get(FLAG_NORTH_MARK_UNCOVERED))
    Kibombo_UncoverNorthMark();
  if (GameFlag_Get(FLAG_SOUTH_MARK_UNCOVERED))
    Kibombo_UncoverSouthMark();
  if (GameFlag_Get(FLAG_WEST_MARK_UNCOVERED))
    Kibombo_UncoverWestMark();
  if (GameFlag_Get(FLAG_EAST_MARK_UNCOVERED))
    Kibombo_UncoverEastMark();
  Actor_DisableById(ACTOR_STATUE_LEFT_EYE);
  Actor_DisableById(ACTOR_STATUE_RIGHT_EYE);
  Actor_DisableById(ACTOR_STATUE_TONGUE);
  Actor_SetSpritePriority(ACTOR_STATUE_LEFT_EYE, 1);
  Actor_SetSpritePriority(ACTOR_STATUE_RIGHT_EYE, 1);
  Actor_SetSpritePriority(ACTOR_STATUE_TONGUE, 1);
}

/// Entrances of the grounds no exit leads to and no map lists, each showing the
/// statue at a stage of the ceremony: its mouth shut, then open with its eyes
/// lit, its tongue out, and its tongue reaching for the gift.
enum {
  ENTRANCE_STATUE_REACHING = 96,
  ENTRANCE_STATUE_TONGUE_OUT = 97,
  ENTRANCE_STATUE_OPEN = 98,
  ENTRANCE_STATUE_SHUT = 99
};

/// The grounds by day, with the statue's mouth and tongue as the ceremony left
/// them.
void Kibombo_InitializeStatueGroundsByDay(void) {
  s16 entrance;

  Kibombo_InitializeStatueGrounds();
  if (GameFlag_Get(FLAG_GABOMBA_RESPONDED)) {
    Kibombo_SetStatueMouth(1, 0);
    Kibombo_SetStatueTongue(1, 5);
  }
  wait(1);
  entrance = gMainRoutineWork.entrance;
  if (entrance == ENTRANCE_STATUE_SHUT) {
    Kibombo_SetStatueMouth(0, 0);
    Kibombo_SetStatueTongue(0, 0);
    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, 0, 0, 0);
  } else if (entrance == ENTRANCE_STATUE_OPEN) {
    Kibombo_SetStatueMouth(1, 0);
    Kibombo_SetStatueTongue(1, 0);
    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, 0, 0, 0);
  } else if (entrance == ENTRANCE_STATUE_TONGUE_OUT) {
    Kibombo_SetStatueMouth(1, 0);
    Kibombo_SetStatueTongue(1, 2);
    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, 0, 0, 0);
  } else if (entrance == ENTRANCE_STATUE_REACHING) {
    Kibombo_SetStatueMouth(1, 1);
    Kibombo_SetStatueTongue(1, 4);
    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, 0, 0, 0);
    Actor_SetAnimationById(ACTOR_STATUE_TONGUE, 4);
  }
}

/// The grounds on the ceremony night: the fires' colours kept, the mouth as the
/// statue has answered, and the entrances from the Gabomba Statue's rooms
/// playing each step of the offering.
void Kibombo_InitializeStatueGroundsCeremony(void) {
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  GameFlag_Set(FLAG_SAW_THE_GROUNDS_CEREMONY);
  GameFlag_Set(FLAG_KIBOMBO_FIRES_FLICKER);
  gKibombo_FireColors[0] = *(u16 *)&BG_PLTT[FIRE_COLOR_FIRST];
  gKibombo_FireColors[1] = *(u16 *)&BG_PLTT[GROUNDS_FIRE_COLOR_SECOND];
  gKibombo_FireColors[2] = *(u16 *)&BG_PLTT[GROUNDS_FIRE_COLOR_THIRD];
  gKibombo_FireColors[3] = *(u16 *)&BG_PLTT[GROUNDS_FIRE_COLOR_FOURTH];
  Kibombo_InitializeStatueGrounds();
  if (GameFlag_Get(FLAG_GABOMBA_RESPONDED)) {
    Kibombo_SetStatueMouth(1, 1);
    Kibombo_SetStatueTongue(1, 4);
  }
  wait(1);
  switch (gMainRoutineWork.entrance) {
  case KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_NORTH_A:
    Kibombo_PresentOrbToGabomba();
    return;
  case KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_NORTH_B:
    Kibombo_GabombaNotPleased();
    return;
  case KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_SOUTH:
    Kibombo_WitchDoctorFeelsPower();
    return;
  case KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_CHAMBER_IDOL_NORTH:
    Kibombo_GabombaStillNotPleased();
    return;
  case KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_NORTH_D:
    Kibombo_GabombaSeesOrb();
    return;
  case KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_NORTH_E:
    Kibombo_GabombaResponds();
    return;
  case KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_FACE_EAST_A:
    Kibombo_GabombaRespondsToGift();
    return;
  }
  Actor_SetSpritePriority(ACTOR_AKAFUBU, 1);
  Actor_SetSpritePriority(ACTOR_PIERS, 2);
  Actor_SetSpritePriority(ACTOR_KRADEN, 2);
  Actor_SetSpritePriority(ACTOR_JENNA, 2);
  Actor_SetSpritePriority(ACTOR_SHEBA, 2);
  if (GameFlag_Get(FLAG_KIBOMBO_PIERS_WITH_PARTY))
    Actor_SetPosition(ACTOR_PIERS, 0, 0);
  else if (GameFlag_Get(FLAG_KIBOMBO_PIERS_STAYED_BEHIND)) {
    Actor_SetPositionAndFacing(ACTOR_PIERS, toFIXED(304), toFIXED(88),
                               FACING_SOUTHEAST);
  }
  if (GameFlag_Get(FLAG_AKAFUBU_OFFERED_HELP)) {
    Kibombo_SetStatueTongue(1, 6);
    Actor_SetPosition(ACTOR_AKAFUBU, 0, 0);
  }
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Task_Add(Kibombo_FlickerCeremonyFires, TASK_PASS_FRAME);
  for (i = 0; i <= ACTOR_WARRIOR_H - ACTOR_WARRIOR_A; i++) {
    u8 *field = &Actor_GetById(i + ACTOR_WARRIOR_A)->retainSpriteCache;
    s32 value = 2;

    *field = value;
  }
  if (GameFlag_Get(FLAG_KIBOMBO_CEREMONY_BEGUN)) {
    if (!GameFlag_Get(FLAG_AKAFUBU_OFFERED_HELP))
      Kibombo_CrowdBows();
  } else if (gMainRoutineWork.entrance ==
             KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_KIBOMBO_CEREMONY_WEST_B) {
    Kibombo_CeremonyStarts();
  }
  Event_End();
  if (gMainRoutineWork.entrance == ENTRANCE_STATUE_SHUT) {
    Kibombo_SetStatueMouth(0, 0);
    Kibombo_SetStatueTongue(0, 0);
    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, 0, 0, 0);
  } else if (gMainRoutineWork.entrance == ENTRANCE_STATUE_OPEN) {
    Kibombo_SetStatueMouth(1, 1);
    Kibombo_SetStatueTongue(1, 0);
    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, 0, 0, 0);
  } else if (gMainRoutineWork.entrance == ENTRANCE_STATUE_TONGUE_OUT) {
    Kibombo_SetStatueMouth(1, 1);
    Kibombo_SetStatueTongue(1, 4);
    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, 0, 0, 0);
  } else if (gMainRoutineWork.entrance == ENTRANCE_STATUE_REACHING) {
    Kibombo_SetStatueMouth(1, 1);
    Kibombo_SetStatueTongue(1, 4);
    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, 0, 0, 0);
    Actor_SetAnimationById(ACTOR_STATUE_TONGUE, 5);
  }
}

/// Before the statue's face: brought here from the grounds as the Great Gabomba
/// answers, its three flashes; brought here for the close look during the
/// ceremony, the gaze into its fires; otherwise its fires flicker.
void Kibombo_InitializeStatueFace(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  GameFlag_Set(FLAG_KIBOMBO_FIRES_FLICKER);
  gKibombo_FireColors[0] = *(u16 *)&BG_PLTT[STATUE_FIRE_COLOR_FIRST];
  gKibombo_FireColors[1] = *(u16 *)&BG_PLTT[STATUE_FIRE_COLOR_SECOND];
  gKibombo_FireColors[2] = *(u16 *)&BG_PLTT[STATUE_FIRE_COLOR_THIRD];
  if (gMainRoutineWork.entrance ==
      GABOMBA_STATUE_FACE_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_A) {
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
    Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId),
                         SPRITE_COLOR_HIDDEN);
    Kibombo_StatueFlashes();
  } else if (gMainRoutineWork.entrance ==
             GABOMBA_STATUE_FACE_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_B) {
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
    Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId),
                         SPRITE_COLOR_HIDDEN);
    Kibombo_GazeAtStatue();
  } else {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Task_Add(Kibombo_FlickerStatueFires, TASK_PASS_FRAME);
    Event_End();
  }
}

void Actor_FallOverById(s32 actor, s32 angle);

/// The houses' entrance at the foot of the ladder down from Akafubu's home, in
/// the Sanctum.
enum { ENTRANCE_HOUSES_FROM_AKAFUBUS_LADDER = 14 };

/// The houses: Akafubu at home after the ceremony, or the ceremony's end
/// played, and the man with the aching back lying where he fell.
void Kibombo_InitializeHouses(void) {
  struct Actor *actor;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Kibombo_HideInnBlocks();
  if (gMainRoutineWork.entrance ==
      KIBOMBO_HOUSES_FROM_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR) {
    if (GameFlag_Get(FLAG_GABOMBA_CEREMONY_ENDED)) {
      Actor_SetPosition(ACTOR_KRADEN_AT_HOME, 0, 0);
      if (GameFlag_Get(FLAG_KIBOMBO_LEFT_HOME_OF_AKAFUBU)) {
        Actor_SetPositionAndFacing(ACTOR_AKAFUBU_AT_HOME, toFIXED(434),
                                   toFIXED(634), FACING_NORTH_NORTHWEST);
      } else {
        Actor_SetPositionAndFacing(ACTOR_AKAFUBU_AT_HOME, toFIXED(364),
                                   toFIXED(576), FACING_NORTH_NORTHWEST);
      }
    } else {
      Kibombo_CeremonyEnds();
    }
  }
#if TLA_PATCH_1
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    actor = Actor_GetById(ACTOR_MAN_WITH_ACHING_BACK);
    actor->x.fixed -= toFIXED(14);
    actor->z.fixed -= toFIXED(8);
  }
#endif
  Actor_FallOverById(ACTOR_MAN_WITH_ACHING_BACK, 0);
  actor = Actor_GetById(ACTOR_MAN_WITH_ACHING_BACK);
  ((struct FieldSprite *)actor->sprite)->rotation = 0;
  actor->scaleX = FIXED_ONE;
  actor->scaleY = FIXED_ONE;
  actor->x.fixed += toFIXED(14);
  actor->z.fixed += toFIXED(8);
  Actor_SetAnimationById(ACTOR_MAN_WITH_ACHING_BACK, 6);
  if (gMainRoutineWork.entrance == ENTRANCE_HOUSES_FROM_AKAFUBUS_LADDER)
    GameFlag_Set(FLAG_KIBOMBO_LEFT_HOME_OF_AKAFUBU);
}

/// Felix walks west off the ledge by the inn in the south east of the village,
/// cell (25, 27), and jumps across onto the top of the nearest painted pole;
/// standing on top of the pole at cell (19, 8), among the poles in the north
/// of the village, and walking north, he jumps onto the top of the next pole
/// north.
void Kibombo_JumpForward(void) { Field_TryJumpForward(); }

void Kibombo_AskAboutFewerWarriors(void);
void Kibombo_AskAboutRevenge(void);
void Kibombo_TalkPathToPeace(void);
void Kibombo_AskGoingWest(void);
void Kibombo_OpenWeaponShop(void);
void Kibombo_OpenArmorShop(void);
void Kibombo_OpenItemShop(void);
void Kibombo_OpenInn(void);
void Kibombo_OpenSanctum(void);
void Kibombo_TalkForeignCeremony(void);
void Kibombo_TalkAchingBack(void);
void Kibombo_ReadMindNeverAgain(void);
void Kibombo_FreezePuddle(void);
void Kibombo_MovePillarForPiers(void);
void Kibombo_MeetPiers(void);
void Kibombo_PiersThanksParty(void);
void Kibombo_PiersRefusesToLeave(void);
void Kibombo_DeadEndByTheStatue(void);
void Kibombo_OverlayFollowsLeader(void);
void Kibombo_OverlayStopsFollowing(void);

// clang-format off
u32 gKibombo_EmberScript[] = {
    SC_WAIT(28),
    SC_SET(SW_HOOK, 0),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

s32 gKibombo_EnterDoorScript[] = {                                                               
      SC_SET(SW_MOTION, 0),                                                    
      SC_ADD(SW_Z, toFIXED(-0.5)),                                                   
      SC_ADD(SW_SCALE_X, -FIXED_ONE / 64),                                               
      SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),                                               
      SC_WAIT(1),                                                              
      SC_LOOP(24, SC_TOP),                                                     
      SC_END,                                                                  
  };

u32 gKibombo_KradenMeetsPiersScript[] = {                                                               
      SC_ANIMATION(2),   SC_MOVE(98, 0, 408),                         
      SC_SYNC,     SC_ANIMATION(1),                                                  
      SC_WAIT(1), SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),              
      SC_WAIT(1), SC_END,                                                     
  };

u32 gKibombo_JennaMeetsPiersScript[] = {                                                               
      SC_ANIMATION(2),   SC_MOVE(80, 0, 376), SC_SYNC,     SC_ANIMATION(1), 
      SC_WAIT(1), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(1), SC_END,    
  };

s32 gKibombo_ShebaMeetsPiersScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(68, 0, 392),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(1),
    SC_END,
    SC_ADD(SW_SCALE_X, FIXED_ONE / 32),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 32),
    SC_CALL(Kibombo_SpinSprite),
    SC_WAIT(1),
    SC_LOOP(10, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 32),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_CALL(Kibombo_SpinSprite),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_RELEASE,
};

u32 gKibombo_PartyGathersScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(88, 0, 392),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

u32 gKibombo_LeaderStepsUpScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(72, 0, 296),
    SC_SYNC,
    SC_MOVE(40, 0, 340),
    SC_SYNC,
    SC_MOVE(40, 0, 356),
    SC_SYNC,
    SC_MOVE(56, 0, 372),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

u32 gKibombo_PiersStepsUpScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2), SC_MOVE(88, 0, 372), SC_SYNC, SC_ANIMATION(1),   
          SC_END,                                                              
  };

u32 gKibombo_KradenJoinsPiersScript[] = {                                                               
      SC_ANIMATION(2),   SC_MOVE(48, 0, 384), SC_SYNC,     SC_ANIMATION(1), 
      SC_WAIT(1), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(1), SC_END,    
  };

u32 gKibombo_JennaJoinsPiersScript[] = {                                                               
      SC_ANIMATION(2),   SC_MOVE(48, 0, 360), SC_SYNC,     SC_ANIMATION(1), 
      SC_WAIT(1), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(1), SC_END,    
  };

u32 gKibombo_ShebaJoinsPiersScript[] = {                                                               
      SC_ANIMATION(2),   SC_MOVE(36, 0, 372), SC_SYNC,     SC_ANIMATION(1), 
      SC_WAIT(1), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(1), SC_END,    
  };

u32 gKibombo_PartyFollowsScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(56, 0, 372),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

u32 gKibombo_JennaAtDeadEndScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(298, 0, 200),                     
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(1), SC_SET(SW_FACING, FACING_NORTHWEST),                    
          SC_WAIT(1), SC_END,                                                 
  };

u32 gKibombo_ShebaAtDeadEndScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(308, 0, 188),                     
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(1), SC_SET(SW_FACING, FACING_NORTHWEST),                    
          SC_WAIT(1), SC_END,                                                 
  };

u32 gKibombo_KradenAtDeadEndScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(298, 0, 176),                     
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(1), SC_SET(SW_FACING, FACING_NORTHWEST),                    
          SC_WAIT(1), SC_END,                                                 
  };

u32 gKibombo_PiersAtDeadEndScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2), SC_MOVE(222, 0, 184), SC_SYNC, SC_ANIMATION(1),   
          SC_END,                                                              
  };

u32 gKibombo_PartyLeavesDeadEndScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(284, 0, 188),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

s32 gKibombo_EyesShakeScript[] = {
    SC_CALL((s32)Kibombo_KeepShakeCenter),
    SC_CALL((s32)Kibombo_ShakeInPlace),
};

s32 gKibombo_EyesPopScript[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE * 3 / 2),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 3 / 2),
    SC_ADD(SW_Y, toFIXED(-2)),
    SC_WAIT(20),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_ADD(SW_Y, toFIXED(2)),
    SC_WAIT(1),
    SC_END,
};

s32 gKibombo_CrowdDanceScript[] = {
    SC_SET(SW_RISE_COUNTER, 0),
    SC_CALL(Kibombo_FaceRandomWay),
    SC_SPEED(0.4),
    SC_ACCELERATION(0.4),
    SC_LABEL(1),
    SC_ANIMATION(2),
    SC_MOVE(230, 0, 486),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(40),
    SC_ANIMATION(2),
    SC_MOVE(256, 0, 486),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_SPEED(0.4),
    SC_ACCELERATION(0.4),
    SC_LABEL(1),
    SC_ANIMATION(2),
    SC_MOVE(298, 0, 486),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(40),
    SC_ANIMATION(2),
    SC_MOVE(272, 0, 486),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s16 gKibombo_FrozenPuddle[] = {
    ACTOR_PUDDLE, FLAG_KIBOMBO_PUDDLE_FROZEN, -1, 0,
};

#include "shared/scene_entrances_default.c"

s16 gKibombo_HousesExitRegions[] = {
    400, 40, 368, 416, 56, 384, -1, 14,
    112, 40, 624, 128, 56, 640, -1, 15,
    384, 40, 880, 400, 56, 896, -1, 16,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(Kibombo),
    SCENE_EXIT(KIBOMBO_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_KIBOMBO),
    SCENE_EXIT(KIBOMBO_TO_KIBOMBO_STATUE_GROUNDS_EAST, KibomboStatueGrounds, KIBOMBO_STATUE_GROUNDS_FROM_KIBOMBO_EAST),
    SCENE_EXIT(KIBOMBO_TO_KIBOMBO_HOUSES_NORTHWEST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_NORTHWEST),
    SCENE_EXIT(KIBOMBO_TO_KIBOMBO_HOUSES_NORTHEAST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_NORTHEAST),
    SCENE_EXIT(KIBOMBO_TO_KIBOMBO_HOUSES_EAST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_EAST),
    SCENE_EXIT(KIBOMBO_TO_KIBOMBO_HOUSES_WEST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_WEST),
    SCENE_EXIT(KIBOMBO_TO_KIBOMBO_HOUSES_SOUTHWEST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_SOUTHWEST),
    SCENE_EXIT(KIBOMBO_TO_KIBOMBO_HOUSES_SOUTHEAST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_SOUTHEAST),
    SCENE_EXIT(KIBOMBO_TO_KIBOMBO_STATUE_GROUNDS_WEST_A, KibomboStatueGrounds, KIBOMBO_STATUE_GROUNDS_FROM_KIBOMBO_WEST_A),
    SCENE_EXIT(KIBOMBO_TO_KIBOMBO_STATUE_GROUNDS_WEST_B, KibomboStatueGrounds, KIBOMBO_STATUE_GROUNDS_FROM_KIBOMBO_WEST_B),
    SCENE_EXITS(KibomboCeremony),
    SCENE_EXIT(KIBOMBO_CEREMONY_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_KIBOMBO),
    SCENE_EXIT(KIBOMBO_CEREMONY_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_KIBOMBO_CEREMONY_EAST),
    SCENE_EXIT(KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_NORTHWEST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_NORTHWEST),
    SCENE_EXIT(KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_NORTHEAST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_NORTHEAST),
    SCENE_EXIT(KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_EAST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_EAST),
    SCENE_EXIT(KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_WEST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_WEST),
    SCENE_EXIT(KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_SOUTHWEST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_SOUTHWEST),
    SCENE_EXIT(KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_SOUTHEAST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_SOUTHEAST),
    SCENE_EXIT(KIBOMBO_CEREMONY_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_WEST_A, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_KIBOMBO_CEREMONY_WEST_A),
    SCENE_EXIT(KIBOMBO_CEREMONY_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_WEST_B, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_KIBOMBO_CEREMONY_WEST_B),
    SCENE_EXITS(KibomboStatueGrounds),
    SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_TO_KIBOMBO_EAST, Kibombo, KIBOMBO_FROM_KIBOMBO_STATUE_GROUNDS_EAST),
    SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_TO_KIBOMBO_WEST_A, Kibombo, KIBOMBO_FROM_KIBOMBO_STATUE_GROUNDS_WEST_A),
    SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_TO_GABOMBA_STATUE_PINION, GabombaStatuePinion, GABOMBA_STATUE_PINION_FROM_KIBOMBO_STATUE_GROUNDS),
    SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_TO_GABOMBA_STATUE_CHAMBER_IDOL, GabombaStatueChamberIdol, GABOMBA_STATUE_CHAMBER_IDOL_FROM_KIBOMBO_STATUE_GROUNDS),
    SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_TO_KIBOMBO_WEST_B, Kibombo, KIBOMBO_FROM_KIBOMBO_STATUE_GROUNDS_WEST_B),
    SCENE_EXITS(KibomboStatueGroundsCeremony),
    SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_EAST, KibomboCeremony, KIBOMBO_CEREMONY_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST),
    SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_WEST_A, KibomboCeremony, KIBOMBO_CEREMONY_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_WEST_A),
    SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_PINION, GabombaStatuePinion, GABOMBA_STATUE_PINION_FROM_KIBOMBO_STATUE_GROUNDS),
    SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_CHAMBER_IDOL_SOUTH, GabombaStatueChamberIdol, GABOMBA_STATUE_CHAMBER_IDOL_FROM_KIBOMBO_STATUE_GROUNDS),
    SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_WEST_B, KibomboCeremony, KIBOMBO_CEREMONY_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_WEST_B),
    SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_ARROWS_NORTH_A, GabombaStatueArrows, GABOMBA_STATUE_ARROWS_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_A),
    SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_ARROWS_NORTH_B, GabombaStatueArrows, GABOMBA_STATUE_ARROWS_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_B_A),
    SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_ARROWS_SOUTH, GabombaStatueArrows, GABOMBA_STATUE_ARROWS_FROM_GABOMBA_STATUE_CHAMBER_IDOL_EAST),
    SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_CHAMBER_IDOL_NORTH, GabombaStatueChamberIdol, GABOMBA_STATUE_CHAMBER_IDOL_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH),
    SCENE_EXITS(KibomboHouses),
    SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_NORTHWEST, Kibombo, KIBOMBO_FROM_KIBOMBO_HOUSES_NORTHWEST, FLAG_KIBOMBO_AT_CEREMONY),
    SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_NORTHWEST, KibomboCeremony, KIBOMBO_CEREMONY_FROM_KIBOMBO_HOUSES_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, KibomboHouses, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, KibomboHouses, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_WEST, Kibombo, KIBOMBO_FROM_KIBOMBO_HOUSES_WEST, FLAG_KIBOMBO_AT_CEREMONY),
    SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_WEST, KibomboCeremony, KIBOMBO_CEREMONY_FROM_KIBOMBO_HOUSES_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_EAST, Kibombo, KIBOMBO_FROM_KIBOMBO_HOUSES_EAST, FLAG_KIBOMBO_AT_CEREMONY),
    SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_EAST, KibomboCeremony, KIBOMBO_CEREMONY_FROM_KIBOMBO_HOUSES_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_NORTHEAST, Kibombo, KIBOMBO_FROM_KIBOMBO_HOUSES_NORTHEAST, FLAG_KIBOMBO_AT_CEREMONY),
    SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_NORTHEAST, KibomboCeremony, KIBOMBO_CEREMONY_FROM_KIBOMBO_HOUSES_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, KibomboHouses, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_SOUTHWEST, Kibombo, KIBOMBO_FROM_KIBOMBO_HOUSES_SOUTHWEST, FLAG_KIBOMBO_AT_CEREMONY),
    SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_SOUTHWEST, KibomboCeremony, KIBOMBO_CEREMONY_FROM_KIBOMBO_HOUSES_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, KibomboHouses, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_SOUTHEAST, Kibombo, KIBOMBO_FROM_KIBOMBO_HOUSES_SOUTHEAST, FLAG_KIBOMBO_AT_CEREMONY),
    SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_SOUTHEAST, KibomboCeremony, KIBOMBO_CEREMONY_FROM_KIBOMBO_HOUSES_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, KibomboHouses, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, KibomboHouses, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, KibomboHouses, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, KibomboHouses, KIBOMBO_HOUSES_FROM_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, KibomboHouses, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, KibomboHouses, 13, EXIT_FLAG_NONE),
    SCENE_EXITS(GabombaStatueFace),
    SCENE_EXIT(GABOMBA_STATUE_FACE_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_A, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_FACE_EAST_A),
    SCENE_EXIT(GABOMBA_STATUE_FACE_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_B, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_NORTH_A),
    SCENE_EXITS_END,
};

struct ActorPlacement gKibombo_ActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gKibombo_VillageActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 72, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 300, 0, 228, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 248, 0, 288, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 208, 0, 176, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 288, 0, 364, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 432, 0, 274, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 58, 0, 298, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 102, 0, 328, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 336, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 246, 0, 488, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 282, 0, 488, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_OLIVE_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 430, 0, 442, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNAL_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 460, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKibombo_CeremonyActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 72, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_OLIVE_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 104, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 114, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208, 0, 152, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 292, 0, 128, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 230, 0, 112, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 304, 0, 220, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 278, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 320, 0, 158, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 234, 0, 132, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 326, 0, 88, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 308, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 278, 0, 146, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 138, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_OLIVE_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 196, 0, 142, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_OLIVE_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 234, 0, 224, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 181, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 194, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 242, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 274, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 338, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 370, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 412, 0, 60, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKibombo_StatueGroundsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GABOMBA_EYE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GABOMBA_EYE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GABOMBA_TONGUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKibombo_StatueGroundsCeremonyActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GABOMBA_EYE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GABOMBA_EYE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GABOMBA_TONGUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_AKAFUBU, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 392, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 218, 0, 420, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 432, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 438, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 440, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 438, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 432, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 310, 0, 422, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 406, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 130, 0, 364, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKibombo_HousesActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_AKAFUBU_WITCH_DOCTOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 366, 0, 586, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_AKAFUBUS_FATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 586, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 634, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 108, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 124, 0, 120, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 100, 0, 132, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 710, 0, 134, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 108, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 432, 0, 120, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 448, 0, 108, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 106, 0, 360, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 352, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 92, 0, 600, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 600, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 348, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_INNKEEPER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 84, 0, 858, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_INN_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 148, 0, 878, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 374, 0, 344, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 824, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 824, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 448, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gKibombo_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gKibombo_VillageTriggers[] = {
    {TRIGGER_EXIT, KIBOMBO_TO_WORLD_MAP, CONDITION_ALWAYS,
     KIBOMBO_TO_WORLD_MAP},
    {TRIGGER_EXIT, KIBOMBO_TO_KIBOMBO_STATUE_GROUNDS_EAST, CONDITION_ALWAYS,
     KIBOMBO_TO_KIBOMBO_STATUE_GROUNDS_EAST},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     KIBOMBO_TO_KIBOMBO_HOUSES_NORTHWEST, CONDITION_ALWAYS,
     EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     KIBOMBO_TO_KIBOMBO_HOUSES_NORTHEAST, CONDITION_ALWAYS,
     EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), KIBOMBO_TO_KIBOMBO_HOUSES_EAST,
     CONDITION_ALWAYS, EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), KIBOMBO_TO_KIBOMBO_HOUSES_WEST,
     CONDITION_ALWAYS, EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     KIBOMBO_TO_KIBOMBO_HOUSES_SOUTHWEST, CONDITION_ALWAYS,
     EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     KIBOMBO_TO_KIBOMBO_HOUSES_SOUTHEAST, CONDITION_ALWAYS,
     EVENT(Kibombo_EnterDoor)},
    {TRIGGER_EXIT, KIBOMBO_TO_KIBOMBO_STATUE_GROUNDS_WEST_A, CONDITION_ALWAYS,
     KIBOMBO_TO_KIBOMBO_STATUE_GROUNDS_WEST_A},
    {TRIGGER_EXIT, KIBOMBO_TO_KIBOMBO_STATUE_GROUNDS_WEST_B, CONDITION_ALWAYS,
     KIBOMBO_TO_KIBOMBO_STATUE_GROUNDS_WEST_B},
    {TRIGGER_STEP, CELL_BEHAVIOR_POLE_RING, CONDITION_ALWAYS,
     EVENT(Kibombo_OverlayFollowsLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MIDDLE_OF_POLE_RING, CONDITION_ALWAYS,
     EVENT(Kibombo_OverlayStopsFollowing)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_KIBOMBO_PUDDLE_FROZEN,
     EVENT(Kibombo_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_POST_A,
     FLAG_KIBOMBO_VILLAGE_ROPE_TIED, 0},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     CELL_BEHAVIOR_JUMP_NORTH_AMONG_POLES, CONDITION_ALWAYS,
     EVENT(Kibombo_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_JUMP_WEST_BY_PUDDLE,
     CONDITION_ALWAYS, EVENT(Kibombo_JumpForward)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FLAME, CONDITION_ALWAYS, 0},
    {TRIGGER_TALK, ACTOR_CHILD_GLAD_OF_THE_CALM, CONDITION_ALWAYS,
     MSGNO(MsgAkafubuHasFinally)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_GLAD_OF_THE_CALM,
     CONDITION_ALWAYS, MSGNO(MsgEveryoneWasTerribly)},
    {TRIGGER_TALK, ACTOR_WOMAN_NOTING_FEWER_WARRIORS, CONDITION_ALWAYS,
     EVENT(Kibombo_AskAboutFewerWarriors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_NOTING_FEWER_WARRIORS,
     CONDITION_ALWAYS, MSGNO(MsgCoveringOurWarriors)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_OPEN_MOUTH, CONDITION_ALWAYS,
     MSGNO(MsgTheGreatGabomba)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_OPEN_MOUTH,
     CONDITION_ALWAYS, MSGNO(MsgIWantTo)},
    {TRIGGER_TALK, ACTOR_WOMAN_MISSING_THE_CEREMONY, CONDITION_ALWAYS,
     MSGNO(MsgNowThatThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_MISSING_THE_CEREMONY,
     CONDITION_ALWAYS, MSGNO(MsgNowThatTheWitch)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_PUZZLED_BY_INDRA, CONDITION_ALWAYS,
     MSGNO(MsgICouldnT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_PUZZLED_BY_INDRA,
     CONDITION_ALWAYS, MSGNO(MsgBecauseKibomboLies)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_SMELLING_MEAT, CONDITION_ALWAYS,
     MSGNO(MsgOhISmell)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_SMELLING_MEAT,
     CONDITION_ALWAYS, MSGNO(MsgYouDonTThink)},
    {TRIGGER_TALK, ACTOR_BOY_WHO_LOVES_MEAT, CONDITION_ALWAYS,
     MSGNO(MsgGoodOlMeat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WHO_LOVES_MEAT,
     CONDITION_ALWAYS, MSGNO(MsgGulpTheSmell)},
    {TRIGGER_TALK, ACTOR_GIRL_TIRED_OF_MONSTERS, CONDITION_ALWAYS,
     MSGNO(MsgWeGetTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_TIRED_OF_MONSTERS,
     CONDITION_ALWAYS, MSGNO(MsgLatelyTheMonsters)},
    {TRIGGER_TALK, ACTOR_GATE_GUARD_A, CONDITION_ALWAYS,
     EVENT(Kibombo_AskAboutRevenge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GATE_GUARD_A, CONDITION_ALWAYS,
     MSGNO(MsgNoOneInKibombo)},
    {TRIGGER_TALK, ACTOR_GATE_GUARD_B, CONDITION_ALWAYS,
     MSGNO(MsgTheOtherWarriors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GATE_GUARD_B, CONDITION_ALWAYS,
     MSGNO(MsgWithAllOur)},
    {TRIGGER_TALK, ACTOR_SIGNAL_FIRE_KEEPER, CONDITION_ALWAYS,
     EVENT(Kibombo_TalkPathToPeace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SIGNAL_FIRE_KEEPER,
     CONDITION_ALWAYS, MSGNO(MsgIWonderIfI)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKibombo_CeremonyTriggers[] = {
    {TRIGGER_STEP, KIBOMBO_CEREMONY_TO_WORLD_MAP, CONDITION_ALWAYS,
     EVENT(Kibombo_LeaveRoom)},
    {TRIGGER_STEP, KIBOMBO_CEREMONY_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST,
     CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_NORTHWEST, FLAG_KIBOMBO_DOORS_SHUT,
     EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_NORTHEAST, FLAG_KIBOMBO_DOORS_SHUT,
     EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_EAST, FLAG_KIBOMBO_DOORS_SHUT,
     EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_WEST, FLAG_KIBOMBO_DOORS_SHUT,
     EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_SOUTHWEST, FLAG_KIBOMBO_DOORS_SHUT,
     EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_SOUTHEAST, FLAG_KIBOMBO_DOORS_SHUT,
     EVENT(Kibombo_EnterDoor)},
    {TRIGGER_STEP, KIBOMBO_CEREMONY_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_WEST_A,
     CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {TRIGGER_STEP, KIBOMBO_CEREMONY_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_WEST_B,
     CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {TRIGGER_STEP, CELL_BEHAVIOR_POLE_RING, CONDITION_ALWAYS,
     EVENT(Kibombo_OverlayFollowsLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MIDDLE_OF_POLE_RING, CONDITION_ALWAYS,
     EVENT(Kibombo_OverlayStopsFollowing)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_KIBOMBO_PUDDLE_FROZEN,
     EVENT(Kibombo_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_POST_A,
     FLAG_KIBOMBO_VILLAGE_ROPE_TIED, 0},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     CELL_BEHAVIOR_JUMP_NORTH_AMONG_POLES, CONDITION_ALWAYS,
     EVENT(Kibombo_JumpForward)},
    {TRIGGER_TALK, ACTOR_BOY_KEPT_AWAKE, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgAkafubuSCeremony)},
    {TRIGGER_TALK, ACTOR_BOY_KEPT_AWAKE, CONDITION_ALWAYS,
     MSGNO(MsgTheyToldMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_KEPT_AWAKE,
     FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgItSStrange)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_KEPT_AWAKE,
     CONDITION_ALWAYS, MSGNO(MsgAsLongAsThis)},
    {TRIGGER_TALK, ACTOR_MAN_WHO_SAW_THE_JEWEL_FLOAT, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgDuringTheLast)},
    {TRIGGER_TALK, ACTOR_MAN_WHO_SAW_THE_JEWEL_FLOAT, CONDITION_ALWAYS,
     MSGNO(MsgAkafubuHasBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_SAW_THE_JEWEL_FLOAT,
     FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgTheGreatGabombaIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_SAW_THE_JEWEL_FLOAT,
     CONDITION_ALWAYS, MSGNO(MsgWhatSTaking)},
    {TRIGGER_TALK, ACTOR_AFRAID_MAN, CONDITION_ALWAYS, MSGNO(MsgIMAfraid)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFRAID_MAN, CONDITION_ALWAYS,
     MSGNO(MsgWhenHeFails)},
    {TRIGGER_TALK, ACTOR_GIRL_WHO_HOPES, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgIHopeThe)},
    {TRIGGER_TALK, ACTOR_GIRL_WHO_HOPES, CONDITION_ALWAYS,
     MSGNO(MsgWhewThankGoodness)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_WHO_HOPES,
     FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgIfTheGreat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_WHO_HOPES,
     CONDITION_ALWAYS, MSGNO(MsgAmIGoing)},
    {TRIGGER_TALK, ACTOR_WOMAN_DOUBTING_THE_JEWEL, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgIsMakingThe)},
    {TRIGGER_TALK, ACTOR_WOMAN_DOUBTING_THE_JEWEL, CONDITION_ALWAYS,
     MSGNO(MsgOnceTheCeremony)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_DOUBTING_THE_JEWEL,
     FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgHeSFailed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_DOUBTING_THE_JEWEL,
     CONDITION_ALWAYS, MSGNO(MsgKibomboWasSo)},
    {TRIGGER_TALK, ACTOR_WOMAN_WARNING_OF_ANGER, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgWhenTheGabomba)},
    {TRIGGER_TALK, ACTOR_WOMAN_WARNING_OF_ANGER, CONDITION_ALWAYS,
     MSGNO(MsgLastTimeAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WARNING_OF_ANGER,
     FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgIHopeAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WARNING_OF_ANGER,
     CONDITION_ALWAYS, MSGNO(MsgAkafubuSAlways)},
    {TRIGGER_TALK, ACTOR_MAN_WHO_KNEW_OEIA, CONDITION_ALWAYS,
     MSGNO(MsgAkafubuWasA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_KNEW_OEIA,
     CONDITION_ALWAYS, MSGNO(MsgAkafubuHasNot)},
    {TRIGGER_TALK, ACTOR_MAN_SURE_OF_AKAFUBU, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgAkafubuLooksReally)},
    {TRIGGER_TALK, ACTOR_MAN_SURE_OF_AKAFUBU, CONDITION_ALWAYS,
     MSGNO(MsgIThinkAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_SURE_OF_AKAFUBU,
     FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgHeTravelsTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_SURE_OF_AKAFUBU,
     CONDITION_ALWAYS, MSGNO(MsgAkafubuIsObsessed)},
    {TRIGGER_TALK, ACTOR_MAN_WHO_KNOWS_THE_SIGN, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgIfTheGreatGabomba)},
    {TRIGGER_TALK, ACTOR_MAN_WHO_KNOWS_THE_SIGN, CONDITION_ALWAYS,
     MSGNO(MsgIBelieveIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_KNOWS_THE_SIGN,
     FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgIfTheGreatGabombaActually)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_KNOWS_THE_SIGN,
     CONDITION_ALWAYS, MSGNO(MsgAkafubuAndOeia)},
    {TRIGGER_TALK, ACTOR_WOMAN_RECALLING_HIS_FAILURES,
     FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgAkafubuHasTried)},
    {TRIGGER_TALK, ACTOR_WOMAN_RECALLING_HIS_FAILURES, CONDITION_ALWAYS,
     MSGNO(MsgWhatDonT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_RECALLING_HIS_FAILURES,
     FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgAsAChildAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_RECALLING_HIS_FAILURES,
     CONDITION_ALWAYS, MSGNO(MsgIMSoFar)},
    {TRIGGER_TALK, ACTOR_WOMAN_NO_PUSHING, CONDITION_ALWAYS,
     MSGNO(MsgHeyNoPushing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_NO_PUSHING,
     CONDITION_ALWAYS, MSGNO(MsgThereAreSo)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_RECALLING_OEIA, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgAkafubuIsAttempting)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_RECALLING_OEIA, CONDITION_ALWAYS,
     MSGNO(MsgTonightSeemsTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_RECALLING_OEIA,
     FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgOeiaSSudden)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_RECALLING_OEIA,
     CONDITION_ALWAYS, MSGNO(MsgWillTheGreat)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_OFFICE, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgHeWhoIs)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_OFFICE, CONDITION_ALWAYS,
     MSGNO(MsgOeiaSDeath)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_OFFICE,
     FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgIfOnlyOeia)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_OFFICE,
     CONDITION_ALWAYS, MSGNO(MsgISuspectAkafubu)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_JEWEL, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgAkafubuBlamesHis)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_JEWEL, CONDITION_ALWAYS,
     MSGNO(MsgTheySayThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_JEWEL,
     FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgTheyAttackedSome)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_JEWEL,
     CONDITION_ALWAYS, MSGNO(MsgTheyReSaying)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_WATCHING_THE_MOUTH,
     FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgIfTheGreatGabombaS)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_WATCHING_THE_MOUTH, CONDITION_ALWAYS,
     MSGNO(MsgAkafubuSeesThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_WATCHING_THE_MOUTH,
     FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgIFeelSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_WATCHING_THE_MOUTH,
     CONDITION_ALWAYS, MSGNO(MsgIReallyWonder)},
    {TRIGGER_TALK, ACTOR_HEALER_ON_THE_CEREMONY, CONDITION_ALWAYS,
     EVENT(Kibombo_TalkForeignCeremony)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER_ON_THE_CEREMONY,
     CONDITION_ALWAYS, MSGNO(MsgIWonderIfA)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKibombo_StatueGroundsTriggers[] = {
    {TRIGGER_STEP, KIBOMBO_STATUE_GROUNDS_TO_KIBOMBO_EAST, CONDITION_ALWAYS,
     EVENT(Kibombo_LeaveRoom)},
    {TRIGGER_STEP, KIBOMBO_STATUE_GROUNDS_TO_KIBOMBO_WEST_A, CONDITION_ALWAYS,
     EVENT(Kibombo_LeaveRoom)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH),
     KIBOMBO_STATUE_GROUNDS_TO_GABOMBA_STATUE_PINION,
     IF_FLAG_SET(FLAG_KIBOMBO_VENT_UNCOVERED), EVENT(Kibombo_EnterVent)},
    {TRIGGER_STEP, KIBOMBO_STATUE_GROUNDS_TO_GABOMBA_STATUE_CHAMBER_IDOL,
     CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoomFading)},
    {TRIGGER_STEP, KIBOMBO_STATUE_GROUNDS_TO_KIBOMBO_WEST_B, CONDITION_ALWAYS,
     EVENT(Kibombo_LeaveRoom)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_KIBOMBO_PUDDLE_FROZEN,
     EVENT(Kibombo_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GROUNDS_PILLAR,
     FLAG_KIBOMBO_PILLAR_MOVED_FOR_PIERS, EVENT(Kibombo_MovePillarForPiers)},
    {TRIGGER_PUSHED, 0, FLAG_KIBOMBO_PILLAR_MOVED_FOR_PIERS,
     EVENT(Kibombo_MovePillarForPiers)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_VENT_PATCH, FLAG_KIBOMBO_VENT_UNCOVERED,
     EVENT(Kibombo_ScoopVent)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_NORTH_MARK, FLAG_NORTH_MARK_UNCOVERED,
     EVENT(Kibombo_ScoopNorthMark)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_SOUTH_MARK, FLAG_SOUTH_MARK_UNCOVERED,
     EVENT(Kibombo_ScoopSouthMark)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_WEST_MARK, FLAG_WEST_MARK_UNCOVERED,
     EVENT(Kibombo_ScoopWestMark)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_EAST_MARK, FLAG_EAST_MARK_UNCOVERED,
     EVENT(Kibombo_ScoopEastMark)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKibombo_StatueGroundsCeremonyTriggers[] = {
    {TRIGGER_STEP, KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_EAST,
     CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {TRIGGER_STEP, KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_WEST_A,
     CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH),
     KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_PINION,
     IF_FLAG_SET(FLAG_KIBOMBO_VENT_UNCOVERED), EVENT(Kibombo_EnterVent)},
    {TRIGGER_STEP,
     KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_CHAMBER_IDOL_SOUTH,
     CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoomFading)},
    {TRIGGER_STEP, KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_WEST_B,
     FLAG_KIBOMBO_PIERS_MET, EVENT(Kibombo_LeaveRoom)},
    {TRIGGER_STEP, KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_WEST_B,
     FLAG_KIBOMBO_PIERS_STAYED_BEHIND, EVENT(Kibombo_PiersRefusesToLeave)},
    {TRIGGER_STEP, KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_WEST_B,
     CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_KIBOMBO_PUDDLE_FROZEN,
     EVENT(Kibombo_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GROUNDS_PILLAR,
     FLAG_KIBOMBO_PILLAR_MOVED_FOR_PIERS, EVENT(Kibombo_MovePillarForPiers)},
    {TRIGGER_PUSHED, 0, FLAG_KIBOMBO_PILLAR_MOVED_FOR_PIERS,
     EVENT(Kibombo_MovePillarForPiers)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_VENT_PATCH, FLAG_KIBOMBO_VENT_UNCOVERED,
     EVENT(Kibombo_ScoopVent)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_NORTH_MARK, FLAG_NORTH_MARK_UNCOVERED,
     EVENT(Kibombo_ScoopNorthMark)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_SOUTH_MARK, FLAG_SOUTH_MARK_UNCOVERED,
     EVENT(Kibombo_ScoopSouthMark)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_WEST_MARK, FLAG_WEST_MARK_UNCOVERED,
     EVENT(Kibombo_ScoopWestMark)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_EAST_MARK, FLAG_EAST_MARK_UNCOVERED,
     EVENT(Kibombo_ScoopEastMark)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BY_PIERS, FLAG_KIBOMBO_PIERS_MET,
     EVENT(Kibombo_MeetPiers)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BY_PIERS_WAITING,
     FLAG_KIBOMBO_PIERS_WITH_PARTY, EVENT(Kibombo_PiersThanksParty)},
    {TRIGGER_TALK, ACTOR_PIERS, CONDITION_ALWAYS, MSGNO(MsgTheOrbIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIERS, CONDITION_ALWAYS,
     MSGNO(MsgIAmCertain)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WAY_TO_DEAD_END, FLAG_KIBOMBO_DEAD_END_SEEN,
     EVENT(Kibombo_DeadEndByTheStatue)},
    {TRIGGER_TALK, ACTOR_WARRIOR_A, CONDITION_ALWAYS, MSGNO(MsgAhhSomethingIs)},
    {TRIGGER_TALK, ACTOR_WARRIOR_B, CONDITION_ALWAYS, MSGNO(MsgYouLookHuman)},
    {TRIGGER_TALK, ACTOR_WARRIOR_C, CONDITION_ALWAYS,
     MSGNO(MsgAkafubuPossessesThe)},
    {TRIGGER_TALK, ACTOR_WARRIOR_D, CONDITION_ALWAYS, MSGNO(MsgWeMustStand)},
    {TRIGGER_TALK, ACTOR_WARRIOR_E, CONDITION_ALWAYS, MSGNO(MsgMenOrGods)},
    {TRIGGER_TALK, ACTOR_WARRIOR_F, CONDITION_ALWAYS, MSGNO(MsgItSeemsThese)},
    {TRIGGER_TALK, ACTOR_WARRIOR_G, CONDITION_ALWAYS, MSGNO(MsgThisIsnT)},
    {TRIGGER_TALK, ACTOR_WARRIOR_H, CONDITION_ALWAYS, MSGNO(MsgItIsProper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_A, CONDITION_ALWAYS,
     MSGNO(MsgTheyLookLike)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_B, CONDITION_ALWAYS,
     MSGNO(MsgIfNoPerson)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_C, CONDITION_ALWAYS,
     MSGNO(MsgISawAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_D, CONDITION_ALWAYS,
     MSGNO(MsgIHopeAkafubuCompletes)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_E, CONDITION_ALWAYS,
     MSGNO(MsgWhatIfThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_F, CONDITION_ALWAYS,
     MSGNO(MsgLookAtThese)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_G, CONDITION_ALWAYS,
     MSGNO(MsgSureThisIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_H, CONDITION_ALWAYS,
     MSGNO(MsgKoffKoffAll)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKibombo_HousesTriggers[] = {
    {TRIGGER_EXIT, KIBOMBO_HOUSES_TO_KIBOMBO_NORTHWEST, CONDITION_ALWAYS,
     KIBOMBO_HOUSES_TO_KIBOMBO_NORTHWEST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, KIBOMBO_HOUSES_TO_KIBOMBO_WEST, CONDITION_ALWAYS,
     KIBOMBO_HOUSES_TO_KIBOMBO_WEST},
    {TRIGGER_EXIT, KIBOMBO_HOUSES_TO_KIBOMBO_EAST, CONDITION_ALWAYS,
     KIBOMBO_HOUSES_TO_KIBOMBO_EAST},
    {TRIGGER_EXIT, KIBOMBO_HOUSES_TO_KIBOMBO_NORTHEAST, CONDITION_ALWAYS,
     KIBOMBO_HOUSES_TO_KIBOMBO_NORTHEAST},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_ITEM_SHOP_LADDER, CONDITION_ALWAYS,
     EVENT(Kibombo_StepOntoFirstLadder)},
    {TRIGGER_EXIT, KIBOMBO_HOUSES_TO_KIBOMBO_SOUTHWEST, CONDITION_ALWAYS,
     KIBOMBO_HOUSES_TO_KIBOMBO_SOUTHWEST},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_AKAFUBUS_LADDER, CONDITION_ALWAYS,
     EVENT(Kibombo_StepOntoSecondLadder)},
    {TRIGGER_EXIT, KIBOMBO_HOUSES_TO_KIBOMBO_SOUTHEAST, CONDITION_ALWAYS,
     KIBOMBO_HOUSES_TO_KIBOMBO_SOUTHEAST},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_TWO_BEDS_LADDER, CONDITION_ALWAYS,
     EVENT(Kibombo_StepOntoThirdLadder)},
    {TRIGGER_TALK, ACTOR_AKAFUBU_AT_HOME, FLAG_KIBOMBO_LEFT_HOME_OF_AKAFUBU,
     MSGNO(MsgIMGoing)},
    {TRIGGER_TALK, ACTOR_AKAFUBU_AT_HOME, CONDITION_ALWAYS, MSGNO(MsgIJustCan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AKAFUBU_AT_HOME,
     CONDITION_ALWAYS, MSGNO(MsgDangWhyDid)},
    {TRIGGER_TALK, ACTOR_FATHER_OF_AKAFUBU, FLAG_KIBOMBO_LEFT_HOME_OF_AKAFUBU,
     MSGNO(MsgIThinkThat)},
    {TRIGGER_TALK, ACTOR_FATHER_OF_AKAFUBU, CONDITION_ALWAYS,
     MSGNO(MsgMySonHasBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FATHER_OF_AKAFUBU,
     CONDITION_ALWAYS, MSGNO(MsgAfterHeBecame)},
    {TRIGGER_TALK, ACTOR_CHILD_ON_THE_EASTERN_SEA, CONDITION_ALWAYS,
     MSGNO(MsgIHaveHeard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_ON_THE_EASTERN_SEA,
     CONDITION_ALWAYS, MSGNO(MsgIKnowI)},
    {TRIGGER_TALK, ACTOR_WOMAN_ASKING_ABOUT_THE_WEST, CONDITION_ALWAYS,
     EVENT(Kibombo_AskGoingWest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ASKING_ABOUT_THE_WEST,
     CONDITION_ALWAYS, MSGNO(MsgCrossingTheRiver)},
    {TRIGGER_TALK, ACTOR_BOY_WHO_HEARD_OF_ATTEKA, CONDITION_ALWAYS,
     MSGNO(MsgYouCanSee)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WHO_HEARD_OF_ATTEKA,
     CONDITION_ALWAYS, MSGNO(MsgIfIM)},
    {TRIGGER_TALK, ACTOR_MAN_ON_TREASURE_WEST, CONDITION_ALWAYS,
     MSGNO(MsgIfYouAreLooking)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_TREASURE_WEST,
     CONDITION_ALWAYS, MSGNO(MsgSureOurLegends)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_TREASURE_HUNTS, CONDITION_ALWAYS,
     MSGNO(MsgEachYearWhen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_TREASURE_HUNTS,
     CONDITION_ALWAYS, MSGNO(MsgIWonderWhatOeia)},
    {TRIGGER_TALK, ACTOR_MAN_WITH_ACHING_BACK, CONDITION_ALWAYS,
     EVENT(Kibombo_TalkAchingBack)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WITH_ACHING_BACK,
     CONDITION_ALWAYS, EVENT(Kibombo_ReadMindNeverAgain)},
    {TRIGGER_TALK, ACTOR_ACHING_BACK_BEDSIDE, CONDITION_ALWAYS,
     EVENT(Kibombo_TalkAchingBack)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ACHING_BACK_BEDSIDE,
     CONDITION_ALWAYS, EVENT(Kibombo_ReadMindNeverAgain)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_A_KINDER_AKAFUBU, CONDITION_ALWAYS,
     MSGNO(MsgAsSoonAs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_A_KINDER_AKAFUBU,
     CONDITION_ALWAYS, MSGNO(MsgAkafubuGetsSo)},
    {TRIGGER_TALK, ACTOR_WARRIOR_NOT_TO_BE_FEARED, CONDITION_ALWAYS,
     MSGNO(MsgManyInGondowan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_NOT_TO_BE_FEARED,
     CONDITION_ALWAYS, MSGNO(MsgItWonT)},
    {TRIGGER_TALK, ACTOR_WARRIOR_TO_TELL_THE_VILLAGES, CONDITION_ALWAYS,
     MSGNO(MsgWeThinkIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_TO_TELL_THE_VILLAGES,
     CONDITION_ALWAYS, MSGNO(MsgIAmAMighty)},
    {TRIGGER_TALK, ACTOR_WEAPON_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(Kibombo_OpenWeaponShop)},
    {TRIGGER_EXAMINE, ACTOR_WEAPON_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(Kibombo_OpenWeaponShop)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_SHOPKEEPER,
     CONDITION_ALWAYS, MSGNO(MsgOurWarriorsAppear)},
    {TRIGGER_TALK, ACTOR_ARMOR_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(Kibombo_OpenArmorShop)},
    {TRIGGER_EXAMINE, ACTOR_ARMOR_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(Kibombo_OpenArmorShop)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_SHOPKEEPER,
     CONDITION_ALWAYS, MSGNO(MsgBecauseOurPaint)},
    {TRIGGER_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(Kibombo_OpenItemShop)},
    {TRIGGER_EXAMINE, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(Kibombo_OpenItemShop)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SHOPKEEPER,
     CONDITION_ALWAYS, MSGNO(MsgIfIHad)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(Kibombo_OpenInn)},
    {TRIGGER_EXAMINE, ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(Kibombo_OpenInn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgThatBedMakes)},
    {TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgICanTRemember)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
     MSGNO(MsgILoveKibombo)},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Kibombo_OpenSanctum)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_SANCTUM_COUNTER, CONDITION_ALWAYS,
     EVENT(Kibombo_OpenSanctum)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS,
     MSGNO(MsgICanTBelieve)},
    {TRIGGER_TABLE_END},
};
#undef DEFINE_KIBOMBO_SET_CROWD_MODE
#undef DEFINE_KIBOMBO_STEP_WHEN_HELD
#undef Actor_SetSpriteFlags
