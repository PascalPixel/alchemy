#include "../shared/tables.h"
#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "version.h"
#include "module/scene/yallam.h"

void Yallam_HideSprite(void);
void Yallam_EnterDoor(void);
void Yallam_DigThirdHole(void);
void Yallam_DigFifthHole(void);
void Yallam_TalkAboutSongCircles(void);

extern s32 gYallam_PillarShadowSunk[];

s32 gYallam_PillarRaised[] = {
    39, 3, 22, 26, 0, 23, 6, 0x20000, 46, (s32)Yallam_HideSprite, 17,
};

s32 gYallam_PillarSunk[] = {
    22, 26, 0, 23, 6, -0x100000, 46, (s32)Yallam_HideSprite, 17,
};

s32 gYallam_PillarShadowSunk[] = {
    22, 9, 0x9999, 22, 10,        0x9999, 22,
    26, 0, 23,     6,  -0x100000, 46,     (s32)Yallam_HideSprite,
    17,
};

// clang-format off
DEFINE_G_DATA_02002634(gYallam_FirstVillagerPaces, 0x1480000, 0x2f00000, 0x1680000,
    0x1780000, 0x3080000, 0x1680000, 0x3180000, 0x1480000, 0x1380000)

DEFINE_G_DATA_02002634(gYallam_SecondVillagerPaces, 0x1680000, 0x3180000, 0x1480000,
    0x1380000, 0x3080000, 0x1480000, 0x2f00000, 0x1680000, 0x1780000)

u32 gYallam_SongGameGather9[] = {
    39, 2, 4, 0x1500000, 0, 0x3180000,
    1, 39, 1, 0, 1, 22,
    3, 0x5000, 0, 10, 17,
};

u32 gYallam_SongGameGather10[] = {
    39, 2, 4, 0x1600000, 0, 0x3180000,
    1, 39, 1, 0, 1, 22,
    3, 0x5000, 0, 10, 17,
};

u32 gYallam_SongGameForm8[] = {
    39, 2, 4, 0x13c0000, 0, 0x3780000,
    1, 4, 0x1500000, 0, 0x3780000, 1,
    4, 0x1580000, 0, 0x3680000, 1, 39,
    1, 0, 1, 22, 3, 0x8000,
    0, 10, 17,
};

u32 gYallam_SongGameForm9[] = {
    39, 2, 4, 0x16a0000, 0, 0x3180000,
    1, 4, 0x1800000, 0, 0x3500000, 1,
    4, 0x17e0000, 0, 0x3780000, 1, 39,
    1, 0, 1, 22, 3, 0x8000,
    0, 10, 17,
};

u32 gYallam_SongGameForm10[] = {
    39, 2, 4, 0x16c0000, 0, 0x3280000,
    1, 4, 0x1760000, 0, 0x36e0000, 1,
    4, 0x1680000, 0, 0x3780000, 1, 39,
    1, 0, 1, 22, 3, 0x8000,
    0, 10, 17,
};

u32 gYallam_SongGameForm11[] = {
    39, 2, 4, 0x1120000, 0, 0x3780000,
    1, 4, 0x1480000, 0, 0x3780000, 1,
    39, 1, 22, 3, 0x8000, 0,
    10, 17,
};

u32 gYallam_SongGameStart8[] = {
    39, 2, 4, 0x1580000, 0, 0x3480000,
    1, 39, 1, 17,
};

u32 gYallam_SongGameStart10[] = {
    39, 2, 4, 0x1580000, 0, 0x3780000,
    1, 4, 0x1580000, 0, 0x3580000, 1,
    39, 1, 0, 1, 17,
};

u32 gYallam_SongGameStart9[] = {
    39, 2, 4, 0x1580000, 0, 0x3780000,
    1, 4, 0x1580000, 0, 0x36a0000, 1,
    39, 1, 0, 1, 17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(139),
    SCENE_EXIT(1, 141, 1),
    SCENE_EXIT(2, 141, 2),
    SCENE_EXIT(3, 142, 1),
    SCENE_EXIT(4, 141, 4),
    SCENE_EXIT(5, 142, 3),
    SCENE_EXIT(6, 142, 2),
    SCENE_EXIT(7, 141, 3),
    SCENE_EXIT(8, 58, 4),
    SCENE_EXIT(9, 140, 6),
    SCENE_EXIT(10, 2, 34),
    SCENE_EXIT(11, 2, 33),
    SCENE_EXIT(12, 139, 15),
    SCENE_EXIT(13, 140, 7),
    SCENE_EXIT(15, 139, 12),
    SCENE_EXITS(140),
    SCENE_EXIT(6, 139, 9),
    SCENE_EXIT(7, 139, 13),
    SCENE_EXITS_END,
};

s32 gYallam_Placements[] = {
    (s32)0xffff0146,
    (s32)gYallam_PillarSunk,
    0xb80000,
    0,
    0xc80000,
    0x8000,
    (s32)0xffff0146,
    (s32)gYallam_PillarRaised,
    0xb80000,
    0,
    0xc80000,
    0x8000,
    (s32)0xffff0147,
    (s32)gYallam_PillarShadowSunk,
    0x80000,
    0,
    0x80000,
    0x8000,
    (s32)0xffff0073,
    1,
    0x2680000,
    0,
    0xd80000,
    0x32000,
    (s32)0xffff0074,
    1,
    0x2c80000,
    0,
    0xe80000,
    0x14000,
    (s32)0xffff0072,
#if !TLA_PATCH_1
    2,
    0x2280000,
    0,
    0xe80000,
#else
    3,
    0x2280000,
    0,
    0xf80000,
#endif
    0x16000,
    (s32)0xffff0080,
    2,
    0x1680000,
    0,
    0x1680000,
    0x1a000,
    (s32)0xffff0070,
    1,
    0x1c80000,
    0,
    0x1180000,
    0x12000,
    (s32)0xffff0071,
    1,
    0xd80000,
    0,
    0x1780000,
    0x1e000,
    (s32)0xffff00e2,
    1,
    0x1d00000,
    0,
    0x880000,
    0x16000,
    (s32)0xffff00e2,
    1,
    0x1880000,
    0,
    0x1080000,
    0x14000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

struct ActorPlacement gYallam_Room2Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gYallam_SongGamePlacements[] = {
    {136, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(872), FACING_WEST, 1, 0},
    {136, CONDITION_ALWAYS, (s32)gYallam_FirstVillagerPaces, PIXELS(312), 0, PIXELS(776),
     FACING_NORTH, 2, 0},
    {138, CONDITION_ALWAYS, (s32)gYallam_SecondVillagerPaces, PIXELS(376), 0, PIXELS(776),
     FACING_SOUTH, 2, 0},
    {138, CONDITION_ALWAYS, 1, PIXELS(272), 0, PIXELS(800), 0xb000, 0, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

s16 gYallam_DoorOpening[] = {
    93, 28, 1, 2, 6, 92, 28, 1,
    2, 6, -1,
};

s16 gYallam_SecondDoorOpening[] = {
    95, 28, 1, 2, 6, 94, 28, 1,
    2, 6, -1,
};

s16 gYallam_ThirdDoorOpening[] = {
    89, 28, 1, 2, 6, 90, 28, 1,
    2, 6, -1,
};

s16 gYallam_FourthDoorOpening[] = {
    92, 30, 2, 2, 6, 94, 30, 2,
    2, 6, -1,
};

s32 gYallam_Doors[] = {
    (s32)gYallam_DoorOpening,
    0x130062,
    (s32)gYallam_SecondDoorOpening,
    0x14005c,
    (s32)gYallam_ThirdDoorOpening,
    0x120054,
    (s32)gYallam_DoorOpening,
    0x7004f,
    (s32)gYallam_ThirdDoorOpening,
    0x100050,
    (s32)gYallam_ThirdDoorOpening,
    0x11004d,
    (s32)gYallam_SecondDoorOpening,
    0x80063,
    (s32)gYallam_FourthDoorOpening,
    0x6005a,
};

struct Trigger gYallam_Triggers[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {0xc602, 7, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {0xc401, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {0x4401, 13, CONDITION_ALWAYS, 13},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 20, CONDITION_ALWAYS, EVENT(Yallam_DigFirstHole)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 21, CONDITION_ALWAYS, EVENT(Yallam_DigSecondHole)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 22, CONDITION_ALWAYS, EVENT(Yallam_DigThirdHole)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 23, CONDITION_ALWAYS, EVENT(Yallam_DigFourthHole)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 24, CONDITION_ALWAYS, EVENT(Yallam_DigFifthHole)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 25, CONDITION_ALWAYS, EVENT(Yallam_DigSixthHole)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgYallamDoesnT)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgOurMasterBoatsman)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgIfYeppWere)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgYeppFoundThis)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgThereUsedTo)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgWhenWeLost)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgYeppWasThe)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgYeppToldMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgYeppWasTheOne)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgSingingWithYepp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgThosePearlsYepp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgYeppNeverTold)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgYeppWasAmazing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgSunshineIsA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgIVeSearched)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgYeppAlwaysSang)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST), 8, 0x300, (s32)(Yallam_DoNothing)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), 8, 0x300, EVENT(Yallam_ForcePillarDown)},
    {TRIGGER_TABLE_END},
};

struct Trigger gYallam_RoomTwoTriggers[] = {
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0xc401, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gYallam_EntranceFifteenTriggers[] = {
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {0x400, 8, CONDITION_ALWAYS, EVENT(Yallam_OfferYeppSong)},
    {0x2400, 8, CONDITION_ALWAYS, EVENT(Yallam_OfferYeppSong)},
    {0xe400, 8, CONDITION_ALWAYS, EVENT(Yallam_OfferYeppSong)},
    {0xc400, 8, CONDITION_ALWAYS, EVENT(Yallam_OfferYeppSong)},
    {0x4400, 8, CONDITION_ALWAYS, EVENT(Yallam_OfferYeppSong)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgYeppSSongs)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(Yallam_TalkAboutSongCircles)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     EVENT(Yallam_TalkAboutSongCircles)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Yallam_TalkAboutSongGame)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     EVENT(Yallam_TalkAboutSongGame)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgYeppWroteAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgTheStoryIs)},
    {TRIGGER_TABLE_END},
};
