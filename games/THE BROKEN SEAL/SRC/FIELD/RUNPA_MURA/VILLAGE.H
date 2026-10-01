#ifndef GUARD_VILLAGE_H
#define GUARD_VILLAGE_H

#include "TYPES.H"
#include "text/MSG_IDS.H"
#include "ABILITY_IDS.H"
#include "FIELD_EFFECT.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FLAG_IDS.H"
#include "ITEM_IDS.H"
#include "SOUND_IDS.H"

/*
 * Lunpa, the thieves' village, and the gates of Lunpa Fortress above it.
 *
 * Two guards keep strangers at the village entrance while Dodonpa holds
 * Hammet in the fortress. Reveal uncovers the village's secrets for as
 * long as it lasts: a puddle that Frost turns into a pillar of ice, a
 * Psynergy Stone, and a switch that opens the hidden passage to the jail.
 * A Nut hovers out of reach until Catch pulls it down.
 *
 * At the gates, two more guards turn the party back. Cloak lets the party
 * slip past them; a party they catch is marched away, and one caught inside
 * the fortress is thrown out onto the gate road. After the escape with
 * Hammet the guards talk of the intruders and the party will not go back.
 */

enum LunpaEntrance {
    LUNPA_ENTRANCE_FROM_WORLD_MAP = 1,
    LUNPA_ENTRANCE_FROM_GATE,
    LUNPA_ENTRANCE_FROM_CAVE,
    LUNPA_ENTRANCE_FROM_FIRST_HOME,
    LUNPA_ENTRANCE_FROM_SECOND_HOME,
    LUNPA_ENTRANCE_FROM_THIRD_HOME,
    LUNPA_ENTRANCE_FROM_ARMS_SHOP,
    LUNPA_ENTRANCE_FROM_ITEM_SHOP,
    LUNPA_ENTRANCE_FROM_INN,
    LUNPA_ENTRANCE_FROM_TEMPLE,
    LUNPA_ENTRANCE_FROM_EIGHTH_ROOM,
    LUNPA_ENTRANCE_FROM_JAIL
};

/* A door's trigger and exit share the number of the room it leads to. */
enum LunpaExit {
    LUNPA_EXIT_TO_WORLD_MAP = 1,
    LUNPA_EXIT_TO_GATE,
    LUNPA_EXIT_TO_CAVE,
    LUNPA_EXIT_TO_FIRST_HOME,
    LUNPA_EXIT_TO_SECOND_HOME,
    LUNPA_EXIT_TO_THIRD_HOME,
    LUNPA_EXIT_TO_ARMS_SHOP,
    LUNPA_EXIT_TO_ITEM_SHOP,
    LUNPA_EXIT_TO_INN,
    LUNPA_EXIT_TO_TEMPLE,
    LUNPA_EXIT_TO_EIGHTH_ROOM,
    LUNPA_EXIT_TO_JAIL
};

enum GateEntrance {
    GATE_ENTRANCE_FROM_FORTRESS = 1,
    GATE_ENTRANCE_FROM_LUNPA,
    GATE_ENTRANCE_SNEAKING_OUT,
    GATE_ENTRANCE_THROWN_OUT
};

enum GateExit {
    GATE_EXIT_TO_FORTRESS = 1,
    GATE_EXIT_TO_LUNPA,
    GATE_EXIT_THIRD
};

/* Where the exits arrive in the scenes they lead to. */
enum {
    WORLD_MAP_ENTRANCE_FROM_LUNPA = 34,
    CAVE_ENTRANCE_FROM_LUNPA = 2,
    ROOM_ENTRANCE_FIRST = 1,
    JAIL_ENTRANCE_FROM_LUNPA = 2,
    FORTRESS_ENTRANCE_FROM_GATE = 1
};

/* The rooms behind the doors take the numbers the interiors give them. */
#define ROOM_ENTRANCE(exit) ((exit) - LUNPA_EXIT_TO_FIRST_HOME + ROOM_ENTRANCE_FIRST)

enum LunpaTrigger {
    TRIGGER_WORLD_MAP_ROAD = 1,
    TRIGGER_GATE_ROAD,
    TRIGGER_CAVE_ROAD,
    TRIGGER_FIRST_HOME_DOOR,
    TRIGGER_SECOND_HOME_DOOR,
    TRIGGER_THIRD_HOME_DOOR,
    TRIGGER_ARMS_SHOP_DOOR,
    TRIGGER_ITEM_SHOP_DOOR,
    TRIGGER_INN_DOOR,
    TRIGGER_TEMPLE_DOOR,
    TRIGGER_EIGHTH_ROOM_DOOR,
    TRIGGER_HIDDEN_PASSAGE,
    TRIGGER_HIDDEN_SWITCH = 20,
    TRIGGER_WELL = 90,
    TRIGGER_GRAVE_SLEEP_BOMB = 100,
    TRIGGER_BARREL_NUT,
    TRIGGER_PSYNERGY_STONE
};

enum GateTrigger {
    TRIGGER_FORTRESS_DOOR = 1,
    TRIGGER_LUNPA_ROAD,
    TRIGGER_THIRD_ROAD,
    TRIGGER_GUARDED_GROUND = 10,
    TRIGGER_FORTRESS_APPROACH = 20,
    TRIGGER_PARTY_SPOTTED = 91
};

/* The doors a party walks through, in trigger order. */
enum {
    DOOR_COUNT = 8,
    DOOR_TEMPLE = TRIGGER_TEMPLE_DOOR - TRIGGER_FIRST_HOME_DOOR
};

/* Reveal's secrets stay hidden while FLAG_LUNPA_SECRETS_HIDDEN is set. */
enum LunpaFlag {
    FLAG_LUNPA_PUDDLE_FROZEN = 0x200,
    FLAG_LUNPA_SECRETS_HIDDEN = 0x201,
    FLAG_LUNPA_PASSAGE_OPEN = 0x202,
    FLAG_GATE_GUARDS_BLOCKING = 0x240,
    FLAG_GATE_CLOAK_CAST = 0x241,
    FLAG_GATE_FORTRESS_ENTERED = 0x242,
    FLAG_GATE_TURNING_BACK = 0x243,
    FLAG_GATE_PARTY_CAUGHT = 0x244,
    FLAG_GUARDS_SUSPECT_KALAY = 0x85a,
    FLAG_GATE_GUARD_SAW_HAMMET = 0x9af,
    FLAG_LUNPA_PSYNERGY_STONE = 0xf27,
    FLAG_LUNPA_GRAVE_SLEEP_BOMB = 0xf88,
    FLAG_LUNPA_BARREL_NUT = 0xf89,
    FLAG_LUNPA_NUT_CAUGHT = 0xfd1
};

/*
 * The fortress remembers what the party did inside only for one visit; the
 * gate clears ten flags, 0x943 to 0x94c, whenever the party arrives.
 */
enum {
    FLAG_FORTRESS_VISIT = 0x943
};

enum LunpaActor {
    ACTOR_HIDDEN_PUDDLE = ACTOR_FIRST_PLACED,
    ACTOR_VILLAGER_A,
    ACTOR_VILLAGER_B,
    ACTOR_VILLAGER_C,
    ACTOR_VILLAGER_D,
    ACTOR_VILLAGER_E,
    ACTOR_VILLAGER_F,
    ACTOR_VILLAGER_G,
    ACTOR_VILLAGER_H,
    ACTOR_WEST_GUARD,
    ACTOR_EAST_GUARD,
    ACTOR_SWITCH_GLINT,
    ACTOR_FLOATING_NUT
};

/* Door scripts stop every actor the scene can place. */
enum {
    ACTOR_LAST_PLACED = 65
};

enum GateActor {
    ACTOR_LEFT_GUARD = ACTOR_FIRST_PLACED,
    ACTOR_RIGHT_GUARD,
    ACTOR_GATE_GERALD
};

enum LunpaSprite {
    SPRITE_ITEM_ICON = 0x16,
    SPRITE_TOWNSPERSON_G = 0x66,
    SPRITE_TOWNSPERSON_B = 0x67,
    SPRITE_TOWNSPERSON_C = 0x6a,
    SPRITE_TOWNSPERSON_H = 0x6b,
    SPRITE_TOWNSPERSON_D = 0x6c,
    SPRITE_TOWNSPERSON_I = 0x6d,
    SPRITE_TOWNSPERSON_E = 0x6f,
    SPRITE_TOWNSPERSON_J = 0x73,
    SPRITE_GUARD = 0x96,
    SPRITE_PUDDLE = 0xe3,
    SPRITE_GLINT = 0x11d
};

/* The placement behaviour of the floating Nut. */
enum {
    ACTOR_FLOAT = 7
};

/* The phases of Reveal and Cloak their area events answer. */
enum {
    REVEAL_SHOWN = 5,
    REVEAL_FADED = 4,
    CLOAK_CAST = 0,
    CLOAK_FADED = 2
};

/* The glint over the hidden switch hovers, stretched, above the ground. */
enum {
    GLINT_MOTION_FLAGS = 8,
    GLINT_SCALE_X = 0x13333,
    GLINT_SCALE_Y = 0x18000
};

/* Retreat from the gate returns the party to this entrance. */
enum {
    GATE_RETREAT_ENTRANCE = 10
};

TEXT_MESSAGE_ENUM(MsgRunpaVillageWestGuardSealedThoughts);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerBSealed);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerDSealed);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerESealed);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerFSealed);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerHSealed);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerASealedThoughts);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerBSealedThoughts);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerCSealedThoughts);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerDSealedThoughts);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerESealedThoughts);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerFSealedThoughts);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerGSealedThoughts);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerHSealedThoughts);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerAReopened);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerCReopened);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerEReopened);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerFReopened);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerGReopened);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerHReopened);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerAReopenedThoughts);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerBReopenedThoughts);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerCReopenedThoughts);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerDReopenedThoughts);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerEReopenedThoughts);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerFReopenedThoughts);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerGReopenedThoughts);
TEXT_MESSAGE_ENUM(MsgRunpaVillageVillagerHReopenedThoughts);
TEXT_MESSAGE_ENUM(MsgRunpaVillageWestGuardReopened);
TEXT_MESSAGE_ENUM(MsgRunpaVillageEastGuardReopened);
TEXT_MESSAGE_ENUM(MsgRunpaVillageWestGuardReopenedThoughts);

/* The guards' warning to a party they catch is a run of lines. */

enum CatchLine {
    CATCH_LEFT_GUARD_CHALLENGES,
    CATCH_RIGHT_GUARD_WONDERS,
    CATCH_LEFT_GUARD_REFUSES_ENTRY,
    CATCH_RIGHT_GUARD_SENDS_PARTY_OFF
};

/* The right guard's thoughts before the escape. */

/* The left guard half recognizes Hammet, then dismisses the thought. */

enum RecognitionLine {
    RECOGNITION_SEEN_THAT_MAN,
    RECOGNITION_IMPOSSIBLE
};


/* A thrown-out leader lies sprawled on the road. */
enum {
    ANIM_SPRAWLED = 19
};

/* A leader on the pillar of ice stands at least this high; map cells are as wide. */
enum {
    PILLAR_TOP_HEIGHT = PIXELS(16),
    CELL_SIZE = PIXELS(16)
};

/* The map object that marks the Psynergy Stone. */
enum {
    MAP_OBJECT_PSYNERGY_STONE = 102
};

/* The heap slot the floating Nut decodes its icon into. */
enum {
    HEAP_ITEM_ICON = 17,
    ITEM_ICON_BUFFER_SIZE = 0x608,
    ITEM_ICON_TILES = 0x400,
    ITEM_ICON_TILE_BYTES = 128
};


struct LunpaDoor {
    const u16 *steps;
    u16 x;
    u16 y;
};

/*
 * The floating Nut keeps its hover in the object record: an angle where a
 * walking actor keeps its speed, and its resting point where a walking actor
 * keeps its destination.
 */
struct FloatingNut {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    u8 unknown_10[0x13];
    u8 priority_flags;
    u8 unknown_24[0x0c];
    s32 angle;
    u8 unknown_34[4];
    s32 rest_x;
    s32 rest_y;
    u8 unknown_40[0x10];
    struct FieldSprite *sprite;
    u8 unknown_54;
    u8 motion_flags;
    u8 status;
    u8 unknown_57[5];
    u8 ready;
    u8 unknown_5d[4];
    /* Set to move only by velocity, skipping the walk to a destination. */
    u8 free_motion;
    u8 unknown_62[10];
    s32 (*update)(union FieldObject *object);
};

extern const struct SceneEntrance gLunpaEntrances[];
extern const struct SceneEntrance gGateEntrances[];
extern const u32 gLunpaExits[];
extern const u32 gGateExits[];
extern const struct ScenePlacement gLunpaPlacements[];
extern const struct ScenePlacement gGatePlacements[];
extern const struct SceneEvent gLunpaSealedEvents[];
extern const struct SceneEvent gLunpaReopenedEvents[];
extern const struct SceneEvent gGateEvents[];
extern const struct LunpaDoor gLunpaDoors[];

void FloatingNut_Catch(void);
void HiddenPuddle_Freeze(void);
void Reveal_ShowSecrets(void);
void Well_Search(void);
void WestGuard_Talk(void);
void EastGuard_Talk(void);
void EastGuard_MindRead(void);
void VillagerA_Talk(void);
void VillagerC_Talk(void);
void VillagerG_Talk(void);
void VillagerB_Shivers(void);
void VillagerD_Talk(void);
void LeftGuard_Talk(void);
void RightGuard_Talk(void);
void LeftGuard_MindRead(void);
void RightGuard_MindRead(void);
void Party_WatchForFortress(void);
void Gerald_RefusesToReturn(void);
void Door_Enter(void);
void HiddenPassage_Enter(void);
void FortressGate_Enter(void);
void HiddenSwitch_Pull(void);
void Guards_BlockGate(void);
void Cloak_Begin(void);
void Guards_CatchParty(void);
void Cloak_End(void);
void Reveal_HideSecrets(void);
void Gateway_Reopen(void);
void Guards_Watch(void);
void Leader_KickUpDust(void);
void Party_ThrownOut(void);
void Leader_SneaksOut(void);
s32 FloatingNut_Update(union FieldObject *object);
void FloatingNut_Initialize(s32 actor);

#endif
