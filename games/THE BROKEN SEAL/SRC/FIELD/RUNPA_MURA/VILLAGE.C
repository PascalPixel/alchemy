#include "TYPES.H"
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

enum LunpaMessage {
    MSG_WELL_PEERED_INTO = 0x947,
    MSG_WEST_GUARD_ASKS_ABOUT_ENTERING = 0x1b9c,
    MSG_EAST_GUARD_ASKS_IF_FROM_KALAY = 0x1b9f,
    MSG_WEST_GUARD_SEALED_THOUGHTS = 0x1ba4,
    MSG_EAST_GUARD_TRUSTS_CAVE_GATE = 0x1ba5,
    MSG_VILLAGER_A_ASKS_HOW_LONG = 0x1ba6,
    MSG_VILLAGER_B_SEALED = 0x1ba9,
    MSG_VILLAGER_C_ASKS_ABOUT_KIDNAPPING = 0x1baa,
    MSG_VILLAGER_D_SEALED = 0x1bad,
    MSG_VILLAGER_E_SEALED = 0x1bae,
    MSG_VILLAGER_F_SEALED = 0x1baf,
    MSG_VILLAGER_G_ASKS_ABOUT_DONPA = 0x1bb0,
    MSG_VILLAGER_H_SEALED = 0x1bb3,
    MSG_LEFT_GUARD_RESENTS_DODONPA = 0x1bb5,
    MSG_RIGHT_GUARD_BOASTS = 0x1bb6,
    MSG_VILLAGER_A_SEALED_THOUGHTS = 0x1bb7,
    MSG_VILLAGER_B_SEALED_THOUGHTS,
    MSG_VILLAGER_C_SEALED_THOUGHTS,
    MSG_VILLAGER_D_SEALED_THOUGHTS,
    MSG_VILLAGER_E_SEALED_THOUGHTS,
    MSG_VILLAGER_F_SEALED_THOUGHTS,
    MSG_VILLAGER_G_SEALED_THOUGHTS,
    MSG_VILLAGER_H_SEALED_THOUGHTS,
    MSG_LEFT_GUARD_THOUGHTS = 0x1bbf,
    MSG_EAST_GUARD_DEMANDS_AUTHORIZATION = 0x1be1,
    MSG_EAST_GUARD_FEARS_BLAME = 0x1be2,
    MSG_GERALD_REFUSES_TO_RETURN = 0x24cf,
    MSG_VILLAGER_A_REOPENED = 0x24d0,
    MSG_VILLAGER_B_SHIVERS = 0x24d1,
    MSG_VILLAGER_C_REOPENED = 0x24d2,
    MSG_VILLAGER_D_ASKS_ABOUT_COMMOTION = 0x24d3,
    MSG_VILLAGER_E_REOPENED = 0x24d6,
    MSG_VILLAGER_F_REOPENED,
    MSG_VILLAGER_G_REOPENED,
    MSG_VILLAGER_H_REOPENED,
    MSG_GUARDS_WARN_PARTY_AWAY = 0x24da,
    MSG_RIGHT_GUARD_WONDERS_HOW = 0x24dd,
    MSG_VILLAGER_A_REOPENED_THOUGHTS = 0x24df,
    MSG_VILLAGER_B_REOPENED_THOUGHTS,
    MSG_VILLAGER_C_REOPENED_THOUGHTS,
    MSG_VILLAGER_D_REOPENED_THOUGHTS,
    MSG_VILLAGER_E_REOPENED_THOUGHTS,
    MSG_VILLAGER_F_REOPENED_THOUGHTS,
    MSG_VILLAGER_G_REOPENED_THOUGHTS,
    MSG_VILLAGER_H_REOPENED_THOUGHTS,
    MSG_LEFT_GUARD_GLIMPSED_MERCHANT = 0x24e7,
    MSG_RIGHT_GUARD_REOPENED_THOUGHTS = 0x24e8,
    MSG_WEST_GUARD_REOPENED = 0x2509,
    MSG_EAST_GUARD_REOPENED = 0x250a,
    MSG_WEST_GUARD_REOPENED_THOUGHTS = 0x250b,
    MSG_EAST_GUARD_THINKS_MERCHANT_HARMLESS = 0x250c,
    MSG_WELL_FROGS = 0x29e1
};

/* The guards' warning to a party they catch is a run of lines. */
extern u8 LinkedMessage_GuardsCatchParty;

enum CatchLine {
    CATCH_LEFT_GUARD_CHALLENGES,
    CATCH_RIGHT_GUARD_WONDERS,
    CATCH_LEFT_GUARD_REFUSES_ENTRY,
    CATCH_RIGHT_GUARD_SENDS_PARTY_OFF
};

/* The right guard's thoughts before the escape. */
extern u8 LinkedMessage_RightGuardThoughts;

/* The left guard half recognizes Hammet, then dismisses the thought. */
extern u8 LinkedMessage_LeftGuardRecognizesHammet;

enum RecognitionLine {
    RECOGNITION_SEEN_THAT_MAN,
    RECOGNITION_IMPOSSIBLE
};

enum {
    MSG_GUARD_FORBIDS_RETURN = 0x2410,
    MSG_LEFT_GUARD_HEARS_SOMEONE = 0x2411,
    MSG_RIGHT_GUARD_FEELS_CREEPY = 0x2412
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

/* Scenes resolve when the overlay links. */
extern u8 LinkedScene_RunpaMura;
extern u8 LinkedScene_RunpaJoGate;

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

void Reveal_HideSecrets(void);
void Gateway_Reopen(void);
void Guards_Watch(void);
void Leader_KickUpDust(void);
void Party_ThrownOut(void);
void Leader_SneaksOut(void);
s32 FloatingNut_Update(union FieldObject *object);
void FloatingNut_Initialize(s32 actor);

const struct SceneEntrance *Scene_GetEntrances(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&LinkedScene_RunpaMura) {
        return gLunpaEntrances;
    }
    if (scene == (s32)&LinkedScene_RunpaJoGate) {
        return gGateEntrances;
    }
    return gLunpaEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return NULL;
}

const u32 *Scene_GetExits(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&LinkedScene_RunpaMura) {
        return gLunpaExits;
    }
    if (scene == (s32)&LinkedScene_RunpaJoGate) {
        return gGateExits;
    }
    return gLunpaExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&LinkedScene_RunpaMura) {
        return gLunpaPlacements;
    }
    if (scene == (s32)&LinkedScene_RunpaJoGate) {
        return gGatePlacements;
    }
    return gLunpaPlacements;
}

void FloatingNut_Catch(void)
{
    Event_Begin();
    Actor_SetPosition(ACTOR_FLOATING_NUT, 0, 0);
    GameFlag_Set(FLAG_LUNPA_NUT_CAUGHT);
    Item_ShowFound(ITEM_NUT, 3);
    Party_GiveItem(ITEM_NUT, 0);
    Event_End();
}

/* The frozen puddle stands as a pillar of ice the party can climb. */
void HiddenPuddle_Freeze(void)
{
    struct FieldActor *pillar;

    pillar = Actor_Get(ACTOR_HIDDEN_PUDDLE);
    Actor_Get(ACTOR_PARTY_LEADER);
    Map_CopyCellAttributes(17, 4, 1, 1, 14, 4);
    Map_CopyCellAttributes(15, 3, 1, 1, 15, 4);
    Map_CopyCellAttributes(15, 3, 1, 1, 13, 4);
    if (pillar != NULL) {
        Actor_SetSpriteFlags(pillar, 0);
        pillar->motion_flags = ACTOR_FALLS;
        pillar->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
    }
    GameFlag_Set(FLAG_LUNPA_PUDDLE_FROZEN);
}

/* A leader standing on the pillar draws above it. */
void IcePillar_UpdateDrawOrder(void)
{
    if (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed >= PILLAR_TOP_HEIGHT) {
        Actor_Get(ACTOR_HIDDEN_PUDDLE)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    } else {
        Actor_Get(ACTOR_HIDDEN_PUDDLE)->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
    }
}

void Reveal_ShowSecrets(void)
{
    struct FieldActor *actor;
    s32 cell_x;
    s32 cell_z;

    actor = Actor_Get(ACTOR_PARTY_LEADER);
    cell_x = actor->x.fixed / CELL_SIZE;
    cell_z = actor->z.fixed / CELL_SIZE;
    if (GameFlag_IsSet(FLAG_LUNPA_PSYNERGY_STONE) == 0) {
        if (cell_x == 7 && cell_z == 16) {
            Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 16);
        }
        MapObject_SetPosition(MAP_OBJECT_PSYNERGY_STONE, -1, -1);
        Map_CopyCellAttributes(28, 31, 1, 1, 7, 16);
    }
    Map_CopyCells(47, 4, 1, 1, 46, 4);
    Map_CopyCellAttributes(34, 37, 3, 3, 13, 3);
    Actor_SetPosition(ACTOR_HIDDEN_PUDDLE, PIXELS(232), PIXELS(72));
    Actor_Get(ACTOR_HIDDEN_PUDDLE)->y.fixed = 0;
    if (GameFlag_IsSet(FLAG_LUNPA_PASSAGE_OPEN) != 0) {
        Map_CopyCells(41, 49, 3, 4, 1, 14);
        Map_CopyCells(44, 49, 3, 4, 33, 14);
        Map_CopyCells(47, 49, 3, 4, 1, 46);
    } else {
        Actor_SetPosition(ACTOR_SWITCH_GLINT, PIXELS(56), PIXELS(268));
        Actor_SetSpriteFlags(Actor_Get(ACTOR_SWITCH_GLINT), 0);
        actor = Actor_Get(ACTOR_SWITCH_GLINT);
        if (actor != NULL) {
            actor->motion_flags = GLINT_MOTION_FLAGS;
            actor->y.fixed = PIXELS(16);
            actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
            actor->scale_x = GLINT_SCALE_X;
            actor->scale_y = GLINT_SCALE_Y;
        }
    }
    Task_AddCallback(IcePillar_UpdateDrawOrder, TASK_PRIORITY_SCENE);
    GameFlag_Clear(FLAG_LUNPA_SECRETS_HIDDEN);
}

void Reveal_HideSecrets(void)
{
    struct FieldActor *puddle;

    Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetPosition(ACTOR_SWITCH_GLINT, 0, 0);
    Actor_SetPosition(ACTOR_HIDDEN_PUDDLE, 0, 0);
    Map_CopyCells(38, 38, 1, 1, 46, 4);
    Map_CopyCellAttributes(37, 37, 3, 3, 13, 3);
    Map_CopyCellAttributes(37, 37, 1, 1, 14, 2);
    Map_CopyCellAttributes(8, 16, 1, 1, 7, 16);
    MapObject_SetPosition(MAP_OBJECT_PSYNERGY_STONE, 0, 0);
    Map_CopyCells(32, 42, 3, 2, 1, 15);
    GameFlag_Clear(FLAG_LUNPA_PUDDLE_FROZEN);
    Actor_SetAnimation(ACTOR_HIDDEN_PUDDLE, ANIM_STAND);
    puddle = Actor_Get(ACTOR_HIDDEN_PUDDLE);
    puddle->update = NULL;
    Object_SetPartPalettes(Actor_Get(ACTOR_HIDDEN_PUDDLE), 0);
    Task_RemoveCallback(IcePillar_UpdateDrawOrder);
    GameFlag_Set(FLAG_LUNPA_SECRETS_HIDDEN);
}

void Well_Search(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_WELL_PEERED_INTO, 1);
    Message_ShowCentered(MSG_WELL_FROGS, 1);
    Event_End();
}

const struct SceneEvent *Scene_GetEvents(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&LinkedScene_RunpaJoGate) {
        /* The gate asks whether Lunpa trades again but answers alike either way. */
        GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED);
        return gGateEvents;
    }
    if (scene == (s32)&LinkedScene_RunpaMura && GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        return gLunpaReopenedEvents;
    }
    return gLunpaSealedEvents;
}

void WestGuard_Talk(void)
{
    Event_Begin();
    Actor_ShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | 2, 60);
    Event_SetMessage(MSG_WEST_GUARD_ASKS_ABOUT_ENTERING);
    Event_AskYesNo(ACTOR_WEST_GUARD, 0);
    Event_End();
}

/*
 * Once the guards suspect Kalay, the east guard questions the party; each
 * reply draws a different line from him.
 */
void EastGuard_Talk(void)
{
    Event_Begin();
    if (GameFlag_IsSet(FLAG_GUARDS_SUSPECT_KALAY) == 0) {
        Event_SetMessage(MSG_EAST_GUARD_DEMANDS_AUTHORIZATION);
        Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    } else {
        Event_SetMessage(MSG_EAST_GUARD_ASKS_IF_FROM_KALAY);
        Event_OpenMessage(ACTOR_EAST_GUARD, 0);
        if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
            gEventWork->message++;
            Event_OpenMessage(ACTOR_EAST_GUARD, 0);
            if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
                gEventWork->message++;
            }
        }
        Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    }
    Event_End();
}

void EastGuard_MindRead(void)
{
    s32 thoughts;

    if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) == 0) {
        thoughts = GameFlag_IsSet(FLAG_GUARDS_SUSPECT_KALAY);
        if (thoughts == 0) {
            thoughts = MSG_EAST_GUARD_FEARS_BLAME;
        } else {
            thoughts = MSG_EAST_GUARD_TRUSTS_CAVE_GATE;
        }
        Event_SetMessage(thoughts);
        Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    } else {
        Event_SetMessage(MSG_EAST_GUARD_THINKS_MERCHANT_HARMLESS);
        Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    }
}

void VillagerA_Talk(void)
{
    Event_Begin();
    Event_SetMessage(MSG_VILLAGER_A_ASKS_HOW_LONG);
    Event_AskYesNo(ACTOR_VILLAGER_A, 0);
    Event_End();
}

void VillagerC_Talk(void)
{
    Event_Begin();
    Event_SetMessage(MSG_VILLAGER_C_ASKS_ABOUT_KIDNAPPING);
    Event_AskYesNo(ACTOR_VILLAGER_C, 0);
    Event_End();
}

void VillagerG_Talk(void)
{
    Event_Begin();
    Event_SetMessage(MSG_VILLAGER_G_ASKS_ABOUT_DONPA);
    Event_AskYesNo(ACTOR_VILLAGER_G, 0);
    Event_End();
}

void VillagerB_Shivers(void)
{
    Event_Begin();
    Actor_RunRepeatedMotion(ACTOR_VILLAGER_B, 3);
    Event_SetMessage(MSG_VILLAGER_B_SHIVERS);
    Event_ShowMessage(ACTOR_VILLAGER_B, 0);
    Event_End();
}

void VillagerD_Talk(void)
{
    Event_Begin();
    Event_SetMessage(MSG_VILLAGER_D_ASKS_ABOUT_COMMOTION);
    Event_AskYesNo(ACTOR_VILLAGER_D, 0);
    Event_End();
}

/*
 * While Hammet travels with the party, the left guard thinks he has seen
 * him before.
 */
void LeftGuard_Talk(void)
{
    s32 recognition;

    if (gGameState.cloaked != 0) {
        Event_SetMessage(MSG_LEFT_GUARD_HEARS_SOMEONE);
    } else if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0
               && GameFlag_IsSet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
        Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 1, 60);
        recognition = (s32)&LinkedMessage_LeftGuardRecognizesHammet;
        Event_SetMessage(recognition + RECOGNITION_SEEN_THAT_MAN);
        Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
        Actor_StartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
        Event_SetMessage(recognition + RECOGNITION_IMPOSSIBLE);
        GameFlag_Set(FLAG_GATE_GUARD_SAW_HAMMET);
    } else {
        Event_SetMessage(MSG_LEFT_GUARD_RESENTS_DODONPA);
    }
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
}

void RightGuard_Talk(void)
{
    if (gGameState.cloaked != 0) {
        Event_SetMessage(MSG_RIGHT_GUARD_FEELS_CREEPY);
    } else if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Event_SetMessage(MSG_RIGHT_GUARD_WONDERS_HOW);
    } else {
        Event_SetMessage(MSG_RIGHT_GUARD_BOASTS);
    }
    Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
}

/* Reading the left guard's mind while Hammet is near makes him look up. */
void LeftGuard_MindRead(void)
{
    s32 seen;

    if (gGameState.cloaked == 0 && GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0
        && GameFlag_IsSet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
        seen = GameFlag_IsSet(FLAG_GATE_GUARD_SAW_HAMMET);
        if (seen == 0) {
            gEventWork->psynergy_request = seen;
            Psynergy_Cancel();
            Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
            Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 1, 60);
            Event_SetMessage((s32)&LinkedMessage_LeftGuardRecognizesHammet
                             + RECOGNITION_SEEN_THAT_MAN);
            GameFlag_Set(FLAG_GATE_GUARD_SAW_HAMMET);
        } else {
            Event_SetMessage(MSG_LEFT_GUARD_GLIMPSED_MERCHANT);
        }
        Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
        Actor_StartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
        Event_SetMessage((s32)&LinkedMessage_LeftGuardRecognizesHammet + RECOGNITION_IMPOSSIBLE);
    } else {
        Event_SetMessage(MSG_LEFT_GUARD_THOUGHTS);
    }
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
}

void RightGuard_MindRead(void)
{
    if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Event_SetMessage(MSG_RIGHT_GUARD_REOPENED_THOUGHTS);
    } else {
        Event_SetMessage((s32)&LinkedMessage_RightGuardThoughts);
    }
    Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
}

/* After the escape, walking up to the fortress raises Gerald's objection. */
void Party_WatchForFortress(void)
{
    s32 cell_z = Actor_Get(ACTOR_PARTY_LEADER)->z.fixed / CELL_SIZE;

    if (GameFlag_IsSet(FLAG_GATE_TURNING_BACK) == 0 && cell_z == 10) {
        GameFlag_Set(FLAG_GATE_TURNING_BACK);
        gEventWork->touched_trigger = TRIGGER_FORTRESS_APPROACH;
    }
}

void Gerald_RefusesToReturn(void)
{
    Event_Begin();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Event_SetMessage(MSG_GERALD_REFUSES_TO_RETURN);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 100);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 12);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    GameFlag_Clear(FLAG_GATE_TURNING_BACK);
    Event_End();
}

/* The door opens, the leader steps through, and the scene changes. */
void Door_Enter(void)
{
    struct EventWork *work;
    struct FieldActor *actor;
    u32 id;
    s32 index;

    work = gEventWork;
    Event_Begin();
    for (id = ACTOR_FIRST_PLACED; id <= ACTOR_LAST_PLACED; id++) {
        actor = Actor_Get(id);
        if (actor != NULL) {
            actor->motion_flags = 0;
        }
    }
    Audio_PlayCue(SOUND_DOOR_OPEN);
    index = work->touched_trigger - TRIGGER_FIRST_HOME_DOOR;
    Map_AnimateCells(gLunpaDoors[index].steps, gLunpaDoors[index].x, gLunpaDoors[index].y);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_Get(ACTOR_PARTY_LEADER)->motion_flags = 0;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    if (index != DOOR_TEMPLE) {
        Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
        Event_Wait(10);
    }
    Event_RequestExit(work->touched_trigger);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
}

/* The opened passage can be walked into only while Reveal lasts. */
void HiddenPassage_Enter(void)
{
    if (GameFlag_IsSet(FLAG_LUNPA_PASSAGE_OPEN) != 0) {
        if (GameFlag_IsSet(FLAG_LUNPA_SECRETS_HIDDEN) == 0) {
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
            Actor_Get(ACTOR_PARTY_LEADER)->motion_flags = 0;
            Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
            Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
            Event_Wait(13);
            Event_RequestExit(LUNPA_EXIT_TO_JAIL);
        }
    }
}

void FortressGate_Enter(void)
{
    GameFlag_Set(FLAG_GATE_FORTRESS_ENTERED);
    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
    Actor_Get(ACTOR_PARTY_LEADER)->motion_flags = 0;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_WalkBy(ACTOR_PARTY_LEADER, 0, -8);
    Audio_PlayCue(SOUND_DOOR_OPEN);
    Map_CopyCells(53, 4, 2, 2, 41, 4);
    Event_Wait(10);
    Map_CopyCells(53, 6, 2, 2, 41, 4);
    Event_Wait(10);
    Event_RequestExit(GATE_EXIT_TO_FORTRESS);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
}

void Party_CheckAhead(void)
{
    Leader_CheckAhead();
}

void Reveal_PlayTreasureCue(void)
{
    if (GameFlag_IsSet(FLAG_LUNPA_SECRETS_HIDDEN) == 0) {
        Audio_PlayCue(SOUND_TREASURE_FOUND);
    }
}

/* The switch grinds the hidden passage open, a block of cells at a time. */
void HiddenSwitch_Pull(void)
{
    if (GameFlag_IsSet(FLAG_LUNPA_SECRETS_HIDDEN) != 0) {
        return;
    }
    if (GameFlag_IsSet(FLAG_LUNPA_PASSAGE_OPEN) != 0) {
        return;
    }
    Actor_SetPosition(ACTOR_SWITCH_GLINT, 0, 0);
    Audio_PlayCue(SOUND_HIDDEN_PASSAGE_OPEN);
    Task_Wait(1);
    Map_CopyCells(32, 45, 3, 4, 1, 14);
    Map_CopyCells(35, 45, 3, 4, 33, 14);
    Map_CopyCells(38, 45, 3, 4, 1, 46);
    Task_Wait(10);
    Map_CopyCells(41, 45, 3, 4, 1, 14);
    Map_CopyCells(44, 45, 3, 4, 33, 14);
    Map_CopyCells(47, 45, 3, 4, 1, 46);
    Task_Wait(10);
    Map_CopyCells(50, 45, 3, 4, 1, 14);
    Map_CopyCells(53, 45, 3, 4, 33, 14);
    Map_CopyCells(56, 45, 3, 4, 1, 46);
    Task_Wait(10);
    Map_CopyCells(32, 49, 3, 4, 1, 14);
    Map_CopyCells(35, 49, 3, 4, 33, 14);
    Map_CopyCells(38, 49, 3, 4, 1, 46);
    Task_Wait(10);
    Map_CopyCells(41, 49, 3, 4, 1, 14);
    Map_CopyCells(44, 49, 3, 4, 33, 14);
    Map_CopyCells(47, 49, 3, 4, 1, 46);
    Task_Wait(10);
    GameFlag_Set(FLAG_LUNPA_PASSAGE_OPEN);
}

/* Uncloaked, the party that nears the gate finds the guards in its way. */
void Guards_BlockGate(void)
{
    if (gGameState.cloaked == 0) {
        Event_Begin();
        Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 0, 2);
        Actor_ShowEmote(ACTOR_RIGHT_GUARD, EMOTE_IN_FRONT | 0, 15);
        Event_Wait(30);
        Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 168);
        Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 168);
        Actor_WaitForMove(ACTOR_LEFT_GUARD);
        Actor_WaitForMove(ACTOR_RIGHT_GUARD);
        Actor_Stop(ACTOR_LEFT_GUARD);
        Actor_SetAnimation(ACTOR_LEFT_GUARD, 0);
        Actor_FaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
        Actor_Stop(ACTOR_RIGHT_GUARD);
        Actor_SetAnimation(ACTOR_RIGHT_GUARD, 0);
        Actor_FaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH + FACING_STEP, 0);
        Event_SetMessage(MSG_GUARDS_WARN_PARTY_AWAY);
        Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
        GameFlag_Set(FLAG_GATE_GUARDS_BLOCKING);
        Map_CopyCellAttributes(6, 11, 1, 1, 7, 11);
        Map_CopyCellAttributes(6, 11, 1, 1, 8, 11);
        Map_CopyCellAttributes(6, 11, 1, 1, 9, 11);
        Event_End();
    }
}

void Scene_DoNothing(void)
{
}

/* The cloaked party finds the gateway closed in front of it. */
void Cloak_Begin(void)
{
    Map_CopyCellAttributes(6, 11, 1, 1, 7, 11);
    Map_CopyCellAttributes(6, 11, 1, 1, 8, 11);
    Map_CopyCellAttributes(6, 11, 1, 1, 9, 11);
    GameFlag_Set(FLAG_GATE_CLOAK_CAST);
}

/*
 * The guards challenge the party they have spotted, then march it back
 * down the road and watch for it again.
 */
void Guards_CatchParty(void)
{
    struct FieldActor *leader;
    s32 warning;

    if (GameFlag_IsSet(FLAG_GATE_PARTY_CAUGHT) != 0) {
        return;
    }
    GameFlag_Set(FLAG_GATE_PARTY_CAUGHT);
    Event_Begin();
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
    Actor_StartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
    Actor_StartRepeatedMotion(ACTOR_RIGHT_GUARD, 1);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 2, 60);
    warning = (s32)&LinkedMessage_GuardsCatchParty;
    Event_SetMessage(warning + CATCH_LEFT_GUARD_CHALLENGES);
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_SetSpeed(ACTOR_LEFT_GUARD, 0x20000, 0x10000);
    Actor_SetSpeed(ACTOR_RIGHT_GUARD, 0x20000, 0x10000);
    Actor_SetAnimation(ACTOR_RIGHT_GUARD, ANIM_SHAKE_HEAD);
    Event_Wait(35);
    Event_SetMessage(warning + CATCH_RIGHT_GUARD_WONDERS);
    Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
    Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 3, 30);
    Event_SetMessage(warning + CATCH_LEFT_GUARD_REFUSES_ENTRY);
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
    Actor_SetAnimation(ACTOR_RIGHT_GUARD, ANIM_NOD);
    Event_Wait(25);
    Event_SetMessage(warning + CATCH_RIGHT_GUARD_SENDS_PARTY_OFF);
    Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
    Actor_WalkTo(ACTOR_LEFT_GUARD, leader->x.part.pixel - 1, leader->z.part.pixel);
    Actor_WaitForMove(ACTOR_LEFT_GUARD);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 160, 216);
    Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 200);
    Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 200);
    Actor_WaitForMove(ACTOR_LEFT_GUARD);
    Actor_WaitForMove(ACTOR_RIGHT_GUARD);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
    Event_Wait(12);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 160, 272);
    Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 256);
    Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 256);
    Actor_WaitForMove(ACTOR_LEFT_GUARD);
    Actor_WaitForMove(ACTOR_RIGHT_GUARD);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Event_End();
    Task_AddCallback(Guards_Watch, TASK_PRIORITY_SCENE);
}

void Gateway_Reopen(void)
{
    Map_CopyCellAttributes(7, 12, 1, 1, 7, 11);
    Map_CopyCellAttributes(7, 12, 1, 1, 8, 11);
    Map_CopyCellAttributes(7, 12, 1, 1, 9, 11);
}

/* A party still near the gate when Cloak fades is spotted. */
void Cloak_End(void)
{
    struct EventWork *work;
    struct FieldActor *leader;

    work = gEventWork;
    GameFlag_Clear(FLAG_GATE_CLOAK_CAST);
    GameFlag_Clear(FLAG_GATE_GUARDS_BLOCKING);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader->x.fixed > PIXELS(104) && leader->x.fixed < PIXELS(240)
        && leader->z.fixed > PIXELS(160) && leader->z.fixed < PIXELS(248)) {
        Task_RemoveCallback(Guards_Watch);
        work->raised_trigger = TRIGGER_PARTY_SPOTTED;
    }
    Gateway_Reopen();
    GameFlag_Clear(FLAG_GATE_PARTY_CAUGHT);
}

/* An uncloaked party that walks up to the gate is spotted. */
void Guards_Watch(void)
{
    struct EventWork *work;
    struct FieldActor *leader;

    work = gEventWork;
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (gGameState.cloaked == 0 && (u32)(leader->x.fixed - PIXELS(144)) <= PIXELS(32)
        && leader->z.fixed >= PIXELS(168) && leader->z.fixed < PIXELS(176)) {
        Task_RemoveCallback(Guards_Watch);
        work->raised_trigger = TRIGGER_PARTY_SPOTTED;
    }
}

/* Every sixteenth frame the dragged leader kicks up a puff of dust. */
void Leader_KickUpDust(void)
{
    struct FieldActor *leader;
    s32 angle;
    s32 velocity[3];

    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if ((gFrameCount & 15) == 0) {
        angle = (((u32)Random_Next() * 52) >> 16) * 64 + 230;
        velocity[0] = Math_Cos(angle) / 4;
        velocity[1] = 0;
        velocity[2] = Math_Sin(angle) / 2;
        Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed, velocity[0], velocity[1],
                     velocity[2], 0, NULL);
    }
}

/*
 * The guards drag the party caught inside the fortress down to the gate
 * road and throw it; the leader bounces twice and lies sprawled.
 */
void Party_ThrownOut(void)
{
    Event_Begin();
    Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(160), PIXELS(128));
    Actor_SetPosition(ACTOR_LEFT_GUARD, PIXELS(152), PIXELS(112));
    Actor_SetPosition(ACTOR_RIGHT_GUARD, PIXELS(168), PIXELS(112));
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    /* The script also turns the village guards, whom the gate does not place. */
    Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH + FACING_STEP, 0);
    Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Event_OpenScreen();
    Event_Wait(30);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1cccc, 0xe666);
    Actor_SetSpeed(ACTOR_LEFT_GUARD, 0x1cccc, 0xe666);
    Actor_SetSpeed(ACTOR_RIGHT_GUARD, 0x1cccc, 0xe666);
    Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 288);
    Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 288);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 160, 296);
    Task_AddCallback(Leader_KickUpDust, TASK_PRIORITY_SCENE);
    Event_Wait(1);
    Audio_PlayCue(SOUND_SCUFFLE);
    Event_Wait(20);
    Actor_SetSpritePriority(ACTOR_LEFT_GUARD, 3);
    Actor_SetSpritePriority(ACTOR_RIGHT_GUARD, 3);
    Audio_PlayCue(SOUND_SCUFFLE);
    Event_Wait(30);
    Actor_Get(ACTOR_LEFT_GUARD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_Get(ACTOR_RIGHT_GUARD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
    Audio_PlayCue(SOUND_SCUFFLE);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_LEFT_GUARD, ANIM_STAND);
    Actor_SetAnimation(ACTOR_RIGHT_GUARD, ANIM_STAND);
    Task_RemoveCallback(Leader_KickUpDust);
    Actor_Get(ACTOR_PARTY_LEADER)->motion_flags |= ACTOR_FALLS;
    Actor_Get(ACTOR_PARTY_LEADER)->velocity_y = PIXELS(6);
    Actor_Get(ACTOR_PARTY_LEADER)->velocity_z = PIXELS(6);
    Event_Wait(1);
    while (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed != 0) {
        Event_Wait(1);
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_SPRAWLED);
    Audio_PlayCue(SOUND_LANDING_THUD);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
    Task_AddCallback(Leader_KickUpDust, TASK_PRIORITY_SCENE);
    Event_Wait(2);
    Actor_Get(ACTOR_PARTY_LEADER)->velocity_y = PIXELS(3);
    Event_Wait(1);
    while (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed != 0) {
        Event_Wait(1);
    }
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Task_RemoveCallback(Leader_KickUpDust);
    Event_Wait(50);
    Event_SetMessage(MSG_GUARD_FORBIDS_RETURN);
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
    Actor_Get(ACTOR_LEFT_GUARD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_Get(ACTOR_RIGHT_GUARD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetSpeed(ACTOR_LEFT_GUARD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_RIGHT_GUARD, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_LEFT_GUARD, 144, 200);
    Actor_WalkTo(ACTOR_RIGHT_GUARD, 176, 200);
    Actor_WaitForMove(ACTOR_LEFT_GUARD);
    Actor_WaitForMove(ACTOR_RIGHT_GUARD);
    Actor_SetAnimation(ACTOR_LEFT_GUARD, ANIM_STAND);
    Actor_SetAnimation(ACTOR_RIGHT_GUARD, ANIM_STAND);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH + FACING_STEP, 0);
    Event_End();
}

/* The leader leaving the fortress casts Cloak and slips around the guards. */
void Leader_SneaksOut(void)
{
    Event_Begin();
    Event_OpenScreen();
    Actor_WalkTo(ACTOR_PARTY_LEADER, 152, 168);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Event_Wait(20);
    Psynergy_Begin(ABILITY_CLOAK, 1);
    Psynergy_SetTarget(ACTOR_PARTY_LEADER, 0);
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Psynergy_LowerHands();
    Actor_WalkTo(ACTOR_PARTY_LEADER, 144, 184);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 184);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 200);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 72, 200);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 72, 288);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 288);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Event_End();
}

s32 Scene_Initialize(void)
{
    struct FieldActor *puddle;

    if (gGameState.scene == (s32)&LinkedScene_RunpaMura) {
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        Reveal_HideSecrets();
        if (GameFlag_IsSet(FLAG_LUNPA_NUT_CAUGHT) == 0) {
            FloatingNut_Initialize(ACTOR_FLOATING_NUT);
        }
        puddle = Actor_Get(ACTOR_HIDDEN_PUDDLE);
        if (puddle != NULL) {
            Actor_SetSpriteFlags(puddle, 0);
        }
        GameFlag_Set(FLAG_LUNPA_SECRETS_HIDDEN);
    }
    if (gGameState.scene == (s32)&LinkedScene_RunpaJoGate) {
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        gGameState.retreat_entrance = GATE_RETREAT_ENTRANCE;
        if (gGameState.entrance == GATE_ENTRANCE_THROWN_OUT
            && GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
            Party_ThrownOut();
        }
        if (gGameState.entrance == GATE_ENTRANCE_SNEAKING_OUT
            && GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
            Leader_SneaksOut();
        }
        if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0
            && GameFlag_IsSet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
            Task_AddCallback(Party_WatchForFortress, TASK_PRIORITY_SCENE);
        }
        Task_AddCallback(Guards_Watch, TASK_PRIORITY_SCENE);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 1);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 2);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 3);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 4);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 5);
        GameFlag_Clear(FLAG_FORTRESS_VISIT);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 6);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 7);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 8);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 9);
    }
    return 0;
}

/* The Nut circles its resting point, bobbing and swaying. */
s32 FloatingNut_Update(union FieldObject *object)
{
    struct FloatingNut *nut = (struct FloatingNut *)object;
    struct FieldSprite *sprite = nut->sprite;
    s32 bob;
    s32 first;
    s32 second;

    bob = Math_Sin(nut->angle) * 2;
    if (bob > 0) {
        bob = -bob;
    }
    nut->x = nut->rest_x + Math_Cos(nut->angle) * 2;
    nut->y = nut->rest_y + bob;
    sprite->rotation = Math_Cos(nut->angle + 0x8000) / 8;
    first = Random_Next();
    second = Random_Next();
    nut->angle = nut->angle + (((u32)first << 9 >> 16) + ((u32)second << 9 >> 16)) + 0x400;
    return 0;
}

/* The Nut shows its item icon and hovers out of the party's reach. */
void FloatingNut_Initialize(s32 actor)
{
    struct FloatingNut *nut;
    struct FieldSprite *sprite;
    u8 *icon;

    nut = (struct FloatingNut *)Actor_Get(actor);
    sprite = nut->sprite;
    sprite->priority = 1;
    sprite->full_color = 0;
    sprite->palette = 0;
    sprite->part_count = 0;
    Actor_SetSpriteFlags((struct FieldActor *)nut, 0);
    nut->ready = 0;
    nut->motion_flags = 0;
    if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
        nut->y += PIXELS(32);
    }
    nut->priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
    nut->free_motion = 1;
    icon = Heap_Allocate(HEAP_ITEM_ICON, ITEM_ICON_BUFFER_SIZE);
    Item_LoadIcon(ITEM_NUT);
    Vram_Load(sprite->vram_block, ITEM_ICON_TILE_BYTES, &icon[ITEM_ICON_TILES]);
    Heap_Release(HEAP_ITEM_ICON);
    nut->rest_x = nut->x;
    nut->angle = 0;
    nut->rest_y = nut->y;
    nut->ready = 1;
    nut->update = FloatingNut_Update;
    nut->status = 0;
}

/* Lunpa's entrances leave the camera free; the gate's keep it on the road. */
#define LUNPA_ENTRANCE(entrance, x, z, facing) \
    { (entrance), CONDITION_ALWAYS, (x), 0, (z), (facing), 0, -1, -1, -1, -1, 0 }
#define GATE_ENTRANCE(entrance, x, z, facing) \
    { (entrance), CONDITION_ALWAYS, (x), 0, (z), (facing), 0, 32, 48, 272, 344, 0 }

const struct SceneEntrance gLunpaEntrances[] = {
    LUNPA_ENTRANCE(0, 172, 149, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_WORLD_MAP, 224, 472, FACING_NORTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_GATE, 232, 32, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_CAVE, 120, 104, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_FIRST_HOME, 248, 184, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_SECOND_HOME, 344, 232, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_THIRD_HOME, 344, 328, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_ARMS_SHOP, 232, 360, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_ITEM_SHOP, 328, 120, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_INN, 104, 184, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_TEMPLE, 48, 168, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_EIGHTH_ROOM, 216, 344, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_JAIL, 40, 328, FACING_SOUTH),
    { SCENE_TABLE_END },
};

const struct SceneEntrance gGateEntrances[] = {
    GATE_ENTRANCE(0, 152, 328, FACING_NORTH),
    GATE_ENTRANCE(GATE_ENTRANCE_FROM_FORTRESS, 160, 104, FACING_SOUTH),
    GATE_ENTRANCE(GATE_ENTRANCE_FROM_LUNPA, 152, 328, FACING_NORTH),
    GATE_ENTRANCE(GATE_ENTRANCE_SNEAKING_OUT, 144, 104, FACING_SOUTH),
    GATE_ENTRANCE(GATE_ENTRANCE_THROWN_OUT, 160, 224, FACING_EAST),
    { SCENE_TABLE_END },
};

/* A door leads to the room of the same number in the interiors. */
#define ROOM_EXIT(exit) SCENE_EXIT((exit), SCENE_RUNPA_SUHARA, ROOM_ENTRANCE(exit))

const u32 gLunpaExits[] = {
    SCENE_EXITS(SCENE_RUNPA_MURA),
    SCENE_EXIT(LUNPA_EXIT_TO_WORLD_MAP, SCENE_WORLD_MAP, WORLD_MAP_ENTRANCE_FROM_LUNPA),
    SCENE_EXIT(LUNPA_EXIT_TO_GATE, SCENE_RUNPA_JO_GATE, GATE_ENTRANCE_FROM_LUNPA),
    SCENE_EXIT(LUNPA_EXIT_TO_CAVE, SCENE_RUNPA_DOU, CAVE_ENTRANCE_FROM_LUNPA),
    ROOM_EXIT(LUNPA_EXIT_TO_FIRST_HOME),
    ROOM_EXIT(LUNPA_EXIT_TO_SECOND_HOME),
    ROOM_EXIT(LUNPA_EXIT_TO_THIRD_HOME),
    ROOM_EXIT(LUNPA_EXIT_TO_ARMS_SHOP),
    ROOM_EXIT(LUNPA_EXIT_TO_ITEM_SHOP),
    ROOM_EXIT(LUNPA_EXIT_TO_INN),
    ROOM_EXIT(LUNPA_EXIT_TO_TEMPLE),
    ROOM_EXIT(LUNPA_EXIT_TO_EIGHTH_ROOM),
    SCENE_EXIT(LUNPA_EXIT_TO_JAIL, SCENE_KUUPUAPPU_RUNPA, JAIL_ENTRANCE_FROM_LUNPA),
    SCENE_EXITS_END,
};

const u32 gGateExits[] = {
    SCENE_EXITS(SCENE_RUNPA_JO_GATE),
    SCENE_EXIT(GATE_EXIT_TO_FORTRESS, SCENE_RUNPA_JO, FORTRESS_ENTRANCE_FROM_GATE),
    SCENE_EXIT(GATE_EXIT_TO_LUNPA, SCENE_RUNPA_MURA, LUNPA_ENTRANCE_FROM_GATE),
    SCENE_EXITS_END,
};

/* The puddle and the glint wait at the map's far corner until Reveal places them. */
const struct ScenePlacement gLunpaPlacements[] = {
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(520), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_TOWNSPERSON_D, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(112), 0, PIXELS(168), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_C, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(280), 0, PIXELS(352), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_TOWNSPERSON_I, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(120), 0, PIXELS(312), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_J, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(136), 0, PIXELS(328), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_B, CONDITION_ALWAYS, ACTOR_WANDER,
      PIXELS(368), 0, PIXELS(256), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_E, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(280), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_H, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(216), 0, PIXELS(40), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_TOWNSPERSON_G, CONDITION_ALWAYS, ACTOR_WANDER,
      PIXELS(152), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_GUARD, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(192), 0, PIXELS(448), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_GUARD, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(256), 0, PIXELS(448), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_GLINT, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(520), 0, PIXELS(520), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_ITEM_ICON, FLAG_LUNPA_NUT_CAUGHT, ACTOR_FLOAT,
      PIXELS(120), 0, PIXELS(472), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END, CONDITION_ALWAYS },
};

const struct ScenePlacement gGatePlacements[] = {
    { SPRITE_GUARD, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(144), 0, PIXELS(200), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_GUARD, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(176), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { ACTOR_GERALD, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

#define TALK(actor, value) { EVENT_TALK, (actor), CONDITION_ALWAYS, (value) }
#define MIND_READ(actor, value) \
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), (actor), CONDITION_ALWAYS, (value) }

/* A door opens as the leader walks north into it. */
#define DOOR(trigger) \
    { TOUCH_WALKING_INTO(FACING_NORTH), (trigger), CONDITION_ALWAYS, EVENT_SCRIPT(Door_Enter) }

#define LUNPA_ROADS_AND_DOORS \
    { EVENT_EXIT, TRIGGER_WORLD_MAP_ROAD, CONDITION_ALWAYS, LUNPA_EXIT_TO_WORLD_MAP }, \
    { EVENT_EXIT, TRIGGER_GATE_ROAD, CONDITION_ALWAYS, LUNPA_EXIT_TO_GATE }, \
    { EVENT_EXIT, TRIGGER_CAVE_ROAD, CONDITION_ALWAYS, LUNPA_EXIT_TO_CAVE }, \
    DOOR(TRIGGER_FIRST_HOME_DOOR), \
    DOOR(TRIGGER_SECOND_HOME_DOOR), \
    DOOR(TRIGGER_THIRD_HOME_DOOR), \
    DOOR(TRIGGER_ARMS_SHOP_DOOR), \
    DOOR(TRIGGER_ITEM_SHOP_DOOR), \
    DOOR(TRIGGER_INN_DOOR), \
    DOOR(TRIGGER_TEMPLE_DOOR), \
    DOOR(TRIGGER_EIGHTH_ROOM_DOOR), \
    { TOUCH_WALKING_INTO(FACING_NORTH), TRIGGER_HIDDEN_PASSAGE, CONDITION_ALWAYS, \
      EVENT_SCRIPT(HiddenPassage_Enter) }

#define LUNPA_SECRETS \
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_HIDDEN_PUDDLE, FLAG_LUNPA_PUDDLE_FROZEN, \
      EVENT_SCRIPT(HiddenPuddle_Freeze) }, \
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, REVEAL_SHOWN), 0, CONDITION_ALWAYS, \
      EVENT_SCRIPT(Reveal_ShowSecrets) }, \
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, REVEAL_FADED), 0, CONDITION_ALWAYS, \
      EVENT_SCRIPT(Reveal_HideSecrets) }, \
    { SEARCH(SEARCH_UNNAMED), TRIGGER_HIDDEN_SWITCH, FLAG_LUNPA_SECRETS_HIDDEN, \
      EVENT_SCRIPT(HiddenSwitch_Pull) }, \
    { SEARCH(SEARCH_UNNAMED), TRIGGER_PSYNERGY_STONE, FLAG_LUNPA_PSYNERGY_STONE, \
      FIND_PSYNERGY_STONE }, \
    { SEARCH(SEARCH_GRAVE), TRIGGER_GRAVE_SLEEP_BOMB, FLAG_LUNPA_GRAVE_SLEEP_BOMB, \
      FIND_ITEM(ITEM_SLEEP_BOMB) }, \
    { SEARCH(SEARCH_BARREL), TRIGGER_BARREL_NUT, FLAG_LUNPA_BARREL_NUT, FIND_ITEM(ITEM_NUT) }, \
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_FLOATING_NUT, FLAG_LUNPA_NUT_CAUGHT, \
      EVENT_SCRIPT(FloatingNut_Catch) }, \
    { SEARCH(SEARCH_UNNAMED), TRIGGER_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(Well_Search) }, \
    { SCENE_EVENTS_END }

const struct SceneEvent gLunpaSealedEvents[] = {
    LUNPA_ROADS_AND_DOORS,
    TALK(ACTOR_VILLAGER_A, EVENT_SCRIPT(VillagerA_Talk)),
    TALK(ACTOR_VILLAGER_B, MSG_VILLAGER_B_SEALED),
    TALK(ACTOR_VILLAGER_C, EVENT_SCRIPT(VillagerC_Talk)),
    TALK(ACTOR_VILLAGER_D, MSG_VILLAGER_D_SEALED),
    TALK(ACTOR_VILLAGER_E, MSG_VILLAGER_E_SEALED),
    TALK(ACTOR_VILLAGER_F, MSG_VILLAGER_F_SEALED),
    TALK(ACTOR_VILLAGER_G, EVENT_SCRIPT(VillagerG_Talk)),
    TALK(ACTOR_VILLAGER_H, MSG_VILLAGER_H_SEALED),
    TALK(ACTOR_WEST_GUARD, EVENT_SCRIPT(WestGuard_Talk)),
    TALK(ACTOR_EAST_GUARD, EVENT_SCRIPT(EastGuard_Talk)),
    MIND_READ(ACTOR_VILLAGER_A, MSG_VILLAGER_A_SEALED_THOUGHTS),
    MIND_READ(ACTOR_VILLAGER_B, MSG_VILLAGER_B_SEALED_THOUGHTS),
    MIND_READ(ACTOR_VILLAGER_C, MSG_VILLAGER_C_SEALED_THOUGHTS),
    MIND_READ(ACTOR_VILLAGER_D, MSG_VILLAGER_D_SEALED_THOUGHTS),
    MIND_READ(ACTOR_VILLAGER_E, MSG_VILLAGER_E_SEALED_THOUGHTS),
    MIND_READ(ACTOR_VILLAGER_F, MSG_VILLAGER_F_SEALED_THOUGHTS),
    MIND_READ(ACTOR_VILLAGER_G, MSG_VILLAGER_G_SEALED_THOUGHTS),
    MIND_READ(ACTOR_VILLAGER_H, MSG_VILLAGER_H_SEALED_THOUGHTS),
    MIND_READ(ACTOR_WEST_GUARD, MSG_WEST_GUARD_SEALED_THOUGHTS),
    MIND_READ(ACTOR_EAST_GUARD, EVENT_SCRIPT(EastGuard_MindRead)),
    LUNPA_SECRETS,
};

const struct SceneEvent gLunpaReopenedEvents[] = {
    LUNPA_ROADS_AND_DOORS,
    TALK(ACTOR_VILLAGER_A, MSG_VILLAGER_A_REOPENED),
    TALK(ACTOR_VILLAGER_B, EVENT_SCRIPT(VillagerB_Shivers)),
    TALK(ACTOR_VILLAGER_C, MSG_VILLAGER_C_REOPENED),
    TALK(ACTOR_VILLAGER_D, EVENT_SCRIPT(VillagerD_Talk)),
    TALK(ACTOR_VILLAGER_E, MSG_VILLAGER_E_REOPENED),
    TALK(ACTOR_VILLAGER_F, MSG_VILLAGER_F_REOPENED),
    TALK(ACTOR_VILLAGER_G, MSG_VILLAGER_G_REOPENED),
    TALK(ACTOR_VILLAGER_H, MSG_VILLAGER_H_REOPENED),
    TALK(ACTOR_WEST_GUARD, MSG_WEST_GUARD_REOPENED),
    TALK(ACTOR_EAST_GUARD, MSG_EAST_GUARD_REOPENED),
    MIND_READ(ACTOR_VILLAGER_A, MSG_VILLAGER_A_REOPENED_THOUGHTS),
    MIND_READ(ACTOR_VILLAGER_B, MSG_VILLAGER_B_REOPENED_THOUGHTS),
    MIND_READ(ACTOR_VILLAGER_C, MSG_VILLAGER_C_REOPENED_THOUGHTS),
    MIND_READ(ACTOR_VILLAGER_D, MSG_VILLAGER_D_REOPENED_THOUGHTS),
    MIND_READ(ACTOR_VILLAGER_E, MSG_VILLAGER_E_REOPENED_THOUGHTS),
    MIND_READ(ACTOR_VILLAGER_F, MSG_VILLAGER_F_REOPENED_THOUGHTS),
    MIND_READ(ACTOR_VILLAGER_G, MSG_VILLAGER_G_REOPENED_THOUGHTS),
    MIND_READ(ACTOR_VILLAGER_H, MSG_VILLAGER_H_REOPENED_THOUGHTS),
    MIND_READ(ACTOR_WEST_GUARD, MSG_WEST_GUARD_REOPENED_THOUGHTS),
    MIND_READ(ACTOR_EAST_GUARD, EVENT_SCRIPT(EastGuard_MindRead)),
    LUNPA_SECRETS,
};

const struct SceneEvent gGateEvents[] = {
    { EVENT_EXIT, TRIGGER_LUNPA_ROAD, CONDITION_ALWAYS, GATE_EXIT_TO_LUNPA },
    { EVENT_EXIT, TRIGGER_THIRD_ROAD, CONDITION_ALWAYS, GATE_EXIT_THIRD },
    { PSYNERGY_IN_AREA(ABILITY_CLOAK, CLOAK_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Cloak_Begin) },
    { PSYNERGY_IN_AREA(ABILITY_CLOAK, CLOAK_FADED), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Cloak_End) },
    { EVENT_TOUCH, TRIGGER_FORTRESS_DOOR, FLAG_GATE_FORTRESS_ENTERED,
      EVENT_SCRIPT(FortressGate_Enter) },
    { EVENT_TOUCH, TRIGGER_GUARDED_GROUND, FLAG_GATE_GUARDS_BLOCKING,
      EVENT_SCRIPT(Guards_BlockGate) },
    { EVENT_TOUCH, TRIGGER_FORTRESS_APPROACH, CONDITION_ALWAYS,
      EVENT_SCRIPT(Gerald_RefusesToReturn) },
    { EVENT_RAISED, TRIGGER_PARTY_SPOTTED, CONDITION_ALWAYS, EVENT_SCRIPT(Guards_CatchParty) },
    TALK(ACTOR_LEFT_GUARD, EVENT_SCRIPT(LeftGuard_Talk)),
    TALK(ACTOR_RIGHT_GUARD, EVENT_SCRIPT(RightGuard_Talk)),
    MIND_READ(ACTOR_LEFT_GUARD, EVENT_SCRIPT(LeftGuard_MindRead)),
    MIND_READ(ACTOR_RIGHT_GUARD, EVENT_SCRIPT(RightGuard_MindRead)),
    { SEARCH(SEARCH_UNNAMED), TRIGGER_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(Well_Search) },
    { SCENE_EVENTS_END },
};

/* Each door swings open in two steps of cells copied over the doorway. */
const u16 gLunpaDoorSteps[DOOR_COUNT][11] = {
    { MAP_CELL_STEP(38, 35, 2, 2, 5), MAP_CELL_STEP(40, 35, 2, 2, 5), MAP_CELL_STEPS_END },
    { MAP_CELL_STEP(42, 35, 2, 2, 5), MAP_CELL_STEP(44, 35, 2, 2, 5), MAP_CELL_STEPS_END },
    { MAP_CELL_STEP(46, 33, 2, 2, 5), MAP_CELL_STEP(48, 33, 2, 2, 5), MAP_CELL_STEPS_END },
    { MAP_CELL_STEP(46, 35, 2, 2, 5), MAP_CELL_STEP(48, 35, 2, 2, 5), MAP_CELL_STEPS_END },
    { MAP_CELL_STEP(50, 33, 2, 2, 5), MAP_CELL_STEP(52, 33, 2, 2, 5), MAP_CELL_STEPS_END },
    { MAP_CELL_STEP(50, 35, 2, 2, 5), MAP_CELL_STEP(52, 35, 2, 2, 5), MAP_CELL_STEPS_END },
    { MAP_CELL_STEP(42, 33, 4, 2, 5), MAP_CELL_STEP(38, 33, 4, 2, 5), MAP_CELL_STEPS_END },
    { MAP_CELL_STEP(54, 33, 2, 2, 5), MAP_CELL_STEP(56, 33, 2, 2, 5), MAP_CELL_STEPS_END },
};

/* The doorway cells each door's steps are copied onto. */
const struct LunpaDoor gLunpaDoors[] = {
    { gLunpaDoorSteps[0], 46, 9 },
    { gLunpaDoorSteps[1], 52, 12 },
    { gLunpaDoorSteps[2], 52, 18 },
    { gLunpaDoorSteps[3], 45, 20 },
    { gLunpaDoorSteps[4], 51, 5 },
    { gLunpaDoorSteps[5], 37, 9 },
    { gLunpaDoorSteps[6], 33, 6 },
    { gLunpaDoorSteps[7], 44, 17 },
};

/* Nothing in the scene reads this word. */
const u32 gLunpaUnusedWord = 0;
