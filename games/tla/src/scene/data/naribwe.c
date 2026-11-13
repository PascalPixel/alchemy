/// @file
/// Naribwe's data: its scripts, door, placements and triggers.

#include "../shared/tables.h"
#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "module/scene/naribwe.h"

void Naribwe_PraiseFortuneTeller(void);
void Naribwe_PraiseFortuneTellerOnReturn(void);
void Naribwe_TalkToPilgrim(void);
void Naribwe_TalkToPilgrimOnReturn(void);
void Naribwe_HideStone(void);

/// The village's placements, the same before and after the return with Piers.
#define DEFINE_NARIBWE_PLACEMENTS(name)                                        \
  struct ActorPlacement name[] = {                                             \
      {365, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(246), FACING_SOUTH, 0, \
       0},                                                                     \
      {304, CONDITION_ALWAYS, (s32)gNaribwe_BrazierBurns, PIXELS(316), 0,      \
       PIXELS(132), FACING_SOUTH, 0, 0},                                       \
      {304, CONDITION_ALWAYS, (s32)gNaribwe_BrazierBurns, PIXELS(372), 0,      \
       PIXELS(132), FACING_SOUTH, 0, 0},                                       \
      {410, CONDITION_ALWAYS, (s32)gNaribwe_FlameRises, PIXELS(316), 0,        \
       PIXELS(132), FACING_SOUTH, 0, 0},                                       \
      {410, CONDITION_ALWAYS, (s32)gNaribwe_FlameRises, PIXELS(372), 0,        \
       PIXELS(132), FACING_SOUTH, 0, 0},                                       \
      {115, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(344), FACING_SOUTH, 1, \
       0},                                                                     \
      {116, CONDITION_ALWAYS, 3, PIXELS(200), 0, PIXELS(296), FACING_EAST, 0,  \
       0},                                                                     \
      {114, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(152), FACING_SOUTH, 1, \
       0},                                                                     \
      {128, CONDITION_ALWAYS, 3, PIXELS(88), 0, PIXELS(376), FACING_NORTHEAST, \
       0, 0},                                                                  \
      {112, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(88), FACING_SOUTHEAST, \
       1, 0},                                                                  \
      {113, CONDITION_ALWAYS, 1, PIXELS(368), 0, PIXELS(224),                  \
       FACING_SOUTHEAST, 1, 0},                                                \
      {136, CONDITION_ALWAYS, 2, PIXELS(200), 0, PIXELS(200),                  \
       FACING_NORTHEAST, 0, 0},                                                \
      {147, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(232),                  \
       FACING_SOUTHWEST, 1, 0},                                                \
      {112, CONDITION_ALWAYS, 2, PIXELS(56), 0, PIXELS(232), FACING_EAST, 0,   \
       0},                                                                     \
      {110, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(360), FACING_EAST, 1,  \
       0},                                                                     \
      {307, CONDITION_ALWAYS, (s32)gNaribwe_IvyAnimation, PIXELS(40), 0,       \
       PIXELS(168), FACING_SOUTH, 2, 0},                                       \
      {407, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(376), FACING_SOUTH, 2, \
       0},                                                                     \
      {489, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(376), FACING_SOUTH, 2, \
       0},                                                                     \
      {201, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(168), FACING_SOUTH, 1, \
       0},                                                                     \
      {328, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(104), FACING_SOUTH, 1, \
       0},                                                                     \
      {SCENE_TABLE_END},                                                       \
  };

// clang-format off
/// Unused: a list of actor 26 and flag 0x200; nothing reads it.
s16 gNaribwe_UnusedList[] = {
    26, 512, -1, 0,
};

/// The braziers burn, the fire's flames rising above them.
u32 gNaribwe_BrazierBurns[] = {
    39, 2, 17,
};

u32 gNaribwe_FlameRises[] = {
    39, 0, 22, 26, 0, 23,
    6, 0x180000, 17,
};

u32 gNaribwe_IvyAnimation[] = {
    39, 3, 17,
};

/// The cell animation of the village's doors.
s16 gNaribwe_DoorOpening[] = {
    8, 42, 1, 2, 6, -1,
};

DEFINE_G_DATA_0200064C(gNaribwe_Doors, gNaribwe_DoorOpening, 0x330012, gNaribwe_DoorOpening,
    0x220015)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 224, 0, 190, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

DEFINE_G_SCENE_EXITS_4(105, 106, 2, 23, 24)

DEFINE_NARIBWE_PLACEMENTS(gNaribwe_Placements)

struct ActorPlacement gNaribwe_Entrance10Placements[] = {
    {364, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(246), FACING_SOUTH, 0, 0},
    {305, CONDITION_ALWAYS, 1, PIXELS(316), 0, PIXELS(132), FACING_SOUTH, 0, 0},
    {305, CONDITION_ALWAYS, 1, PIXELS(372), 0, PIXELS(132), FACING_SOUTH, 0, 0},
    {410, CONDITION_ALWAYS, 1, PIXELS(316), 0, PIXELS(132), FACING_SOUTH, 0, 0},
    {410, CONDITION_ALWAYS, 1, PIXELS(372), 0, PIXELS(132), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};
// clang-format on

DEFINE_NARIBWE_PLACEMENTS(gNaribwe_PlacementsOnReturn)

struct Trigger gNaribwe_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Naribwe_EnterDoor)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Naribwe_EnterDoor)},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgNaribwe01)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS,
     EVENT(Naribwe_AskAboutKibomboExpedition)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgNaribwe04)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgNaribwe05)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgNaribwe06)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(Naribwe_AskWhereFrom)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(Naribwe_AskGoingToKibombo)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgNaribwe10)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgNaribwe19)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(Naribwe_PraiseFortuneTeller)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, EVENT(Naribwe_TalkToPilgrim)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe11)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe12)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe13)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe14)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe15)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe16)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe17)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe18)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe20)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe22)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe24)},
    {0x1f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgNaribwe47))},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     CONDITION_ALWAYS, EVENT(Naribwe_PrepareBlaze)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 23, 0x201,
     EVENT(Naribwe_BlowAwayIvy)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 24, 0x202, 0},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Naribwe_RevealStone)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Naribwe_HideStone)},
    {TRIGGER_TABLE_END},
};

struct Trigger gNaribwe_TriggersOnReturn[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Naribwe_EnterDoor)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Naribwe_EnterDoor)},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgNaribwe25)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgNaribwe26)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgNaribwe27)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgNaribwe28)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgNaribwe29)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgNaribwe30)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgNaribwe31)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgNaribwe32)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgNaribwe41)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS,
     EVENT(Naribwe_PraiseFortuneTellerOnReturn)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, EVENT(Naribwe_TalkToPilgrimOnReturn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe33)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe34)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe35)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe36)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe37)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe38)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe39)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe40)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe42)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe44)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe46)},
    {0x1f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgNaribwe47))},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     CONDITION_ALWAYS, EVENT(Naribwe_PrepareBlaze)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 23, 0x201,
     EVENT(Naribwe_BlowAwayIvy)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 24, 0x202, 0},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Naribwe_RevealStone)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Naribwe_HideStone)},
    {TRIGGER_TABLE_END},
};
