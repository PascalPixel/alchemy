/// @file
/// The scene code of overlay 0x683, in ROM order.

#include "shared/actors.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "version.h"
#include "event_work.h"
#include "scene_ids.h"
#include "event_inline.h"
#include "module/scene/champa_interiors.h"
#include "module/scene/scene.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

void Func_02000178(s32 a, s32 b);

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov683 &&
      (u16)(gMainRoutineWork.entrance - 8) <= 1)
    return (const struct ActorPlacement *)gData_020003a8;
  return (const struct ActorPlacement *)gData_02000228;
}

void Func_0200008c(s32 value) { Func_02000178(25, value); }

void Func_02000098(s32 value) { Func_02000180(10, value); }

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov683 &&
      (u16)(gMainRoutineWork.entrance - 8) <= 1)
    return (const struct Trigger *)gData_02000780;
  if (GameFlag_Test(0x97f))
    return (const struct Trigger *)gData_020005ac;
  return (const struct Trigger *)gData_020003d8;
}

s32 Scene_Initialize(void) {
  s32 *effect = &((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect;
#if !TLA_PATCH_1
  s32 value = 0x201;
#else
  s32 value = 0x209;
#endif

  /* FAKEMATCH(barrier): an empty asm after the address and the value, so gMainRoutineWork's address loads just before the store; as one assignment, 2
   * of 32 instructions differ in tla-en. */
  __asm__("");
  *effect = value;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov683 &&
      (u16)(gMainRoutineWork.entrance - 8) <= 1)
    Func_0200014c();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

DEFINE_SCENE_FADE_IN_AND_CLEAR_FLAG(Func_0200014c)

#include "text/msg_ids.h"

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 416, 0, 664, FACING_NORTH, 0, 288, 536, 528, 696, 0},
    {9, CONDITION_ALWAYS, 608, 0, 664, FACING_NORTH, 0, 488, 528, 728, 704, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(183),
    SCENE_EXIT_IF(1, 182, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 182, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 182, 3, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 182, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 182, 5, CONDITION_ALWAYS),
    SCENE_EXIT_IF(6, 182, 6, CONDITION_ALWAYS),
    SCENE_EXIT(8, 134, 11),
    SCENE_EXIT(9, 131, 2),
    SCENE_EXITS_END,
};

// clang-format off
u32 gData_02000228[] = {
    0xffff0073, 1, 0x800000, 0, 0x480000, 0x4000,
    0xffff0074, 1, 0x680000, 0, 0x580000, 0,
    0xffff0088, 2, 0x900000, 0, 0x880000, 0x15000,
    0xffff008a, 1, 0x680000, 0, 0x680000, 0,
    0xffff00c3, 1, 0x1300000, 0, 0x480000, 0x10000,
    0xffff00c3, 1, 0x1180000, 0, 0x700000, 0x1d000,
    0xffff00c3, 1, 0x1480000, 0, 0x580000, 0x8000,
    0xffff0070, 2, 0x980000, 0, 0x1680000, 0x4000,
    0xffff0071, 1, 0x680000, 0, 0x1500000, 0x18000,
    0xffff0072, 2, 0x1580000, 0, 0x1680000, 0xc000,
    0xffff0080, 1, 0x1280000, 0, 0x1500000, 0x10000,
    0xffff006f, 1, 0xd00000, 0, 0x2500000, 0x15000,
    0xffff0072, 1, 0x700000, 0, 0x2480000, 0x18000,
    0xffff00c7, 1, 0x2580000, 0, 0x500000, 0x15000,
    0xffff00d1, 1, 0x1d80000, 0, 0x5b0000, 0x18000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_020003a8[] = {
    0xffff004a, 3, 0x2600000, 0, 0x2640000, 0x4000,
    0xffff, 0, 0, 0, 0, 0,
};
// clang-format on

s32 gData_020003d8[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    0,
    (s32)0xffff0008,
    MSGNO(MsgThereSNot),
    0,
    (s32)0xffff0009,
    MSGNO(MsgEveryoneInChampa),
    0,
    (s32)0xffff000a,
    MSGNO(MsgWhenMyFather),
    0x8400,
    (s32)0xffff000b,
    MSGNO(MsgMyDadWent),
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgTheseRaftsAre),
    0x8d15,
    (s32)0xffff0009,
    MSGNO(MsgIfDadHadn),
    0x8d15,
    (s32)0xffff000a,
    MSGNO(MsgIHopeDad),
    0x8d15,
    (s32)0xffff000b,
    MSGNO(MsgFatherWorriesSo),
    0,
    (s32)0xffff000c,
    MSGNO(MsgTheTidalWave),
    0,
    (s32)0xffff000d,
    MSGNO(MsgItLookedTo),
    0,
    (s32)0xffff000e,
    MSGNO(MsgMostOfThe),
    0x8d15,
    (s32)0xffff000c,
    MSGNO(MsgTheFishAre),
    0x8d15,
    (s32)0xffff000d,
    MSGNO(MsgSomethingMustHave),
    0x8d15,
    (s32)0xffff000e,
    MSGNO(MsgYouHaveTo),
    0,
    (s32)0xffff000f,
    MSGNO(MsgWeVeSeen),
    0,
    (s32)0xffff0010,
    MSGNO(MsgAGroupOf),
    0x8d15,
    (s32)0xffff000f,
    MSGNO(MsgMoreVisitorsMeans),
    0x8d15,
    (s32)0xffff0010,
    MSGNO(MsgNowThatI),
    0,
    (s32)0xffff0011,
    MSGNO(MsgTheOceanHas),
    0,
    (s32)0xffff0012,
    MSGNO(MsgIVeHeardRumors),
    0x8d15,
    (s32)0xffff0011,
    MSGNO(MsgWeChampaRely),
    0x8d15,
    (s32)0xffff0012,
    MSGNO(MsgAllTheSailors),
    0xc400,
    (s32)0xffff0013,
    (s32)Func_0200008c,
    0,
    (s32)0xffff0013,
    MSGNO(MsgIHaveSome),
    0,
    (s32)0xffff0014,
    MSGNO(MsgIfYouGo),
    0x8d15,
    (s32)0xffff0013,
    MSGNO(MsgOurProductsCost),
    0x8d15,
    (s32)0xffff0014,
    MSGNO(MsgTheStuffIn),
    0xc400,
    (s32)0xffff0015,
    (s32)Func_02000098,
    0,
    (s32)0xffff0015,
    MSGNO(MsgListenIfYou),
    0,
    (s32)0xffff0016,
    MSGNO(MsgWeUsedTo),
    0x8d15,
    (s32)0xffff0015,
    MSGNO(MsgItSA),
    0x8d15,
    (s32)0xffff0016,
    MSGNO(MsgWatchingOurCatches),
    -1,
    0,
    0,
};

s32 gData_020005ac[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    0,
    (s32)0xffff0008,
    MSGNO(MsgWeDonT),
    0,
    (s32)0xffff0009,
    MSGNO(MsgItSTrue),
    0,
    (s32)0xffff000a,
    MSGNO(MsgFatherSActing),
    0x8400,
    (s32)0xffff000b,
    MSGNO(MsgWhenBriggsReturned),
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgIDRather),
    0x8d15,
    (s32)0xffff0009,
    MSGNO(MsgBriggsIsA),
    0x8d15,
    (s32)0xffff000a,
    MSGNO(MsgIfFatherSits),
    0x8d15,
    (s32)0xffff000b,
    MSGNO(MsgQuittingTheirJobs),
    0,
    (s32)0xffff000c,
    MSGNO(MsgIHearThat),
    0,
    (s32)0xffff000d,
    MSGNO(MsgWhileIM),
    0,
    (s32)0xffff000e,
    MSGNO(MsgBriggsReallySaved),
    0x8d15,
    (s32)0xffff000c,
    MSGNO(MsgTheNerveOf),
    0x8d15,
    (s32)0xffff000d,
    MSGNO(MsgIShouldJust),
    0x8d15,
    (s32)0xffff000e,
    MSGNO(MsgIRefuseTo),
    0,
    (s32)0xffff000f,
    MSGNO(MsgIVeBeenWondering),
    0,
    (s32)0xffff0010,
    MSGNO(MsgWeVillageElders),
    0x8d15,
    (s32)0xffff000f,
    MSGNO(MsgANewImage),
    0x8d15,
    (s32)0xffff0010,
    MSGNO(MsgObservingBriggsRecently),
    0,
    (s32)0xffff0011,
    MSGNO(MsgItSeemsThe),
    0,
    (s32)0xffff0012,
    MSGNO(MsgIForOne),
    0x8d15,
    (s32)0xffff0011,
    MSGNO(MsgSureBriggsSaved),
    0x8d15,
    (s32)0xffff0012,
    MSGNO(MsgSupportingAnEntire),
    0xc400,
    (s32)0xffff0013,
    (s32)Func_0200008c,
    0,
    (s32)0xffff0013,
    MSGNO(MsgTheJewelsBriggs),
    0,
    (s32)0xffff0014,
    MSGNO(MsgThanksToBriggs),
    0x8d15,
    (s32)0xffff0013,
    MSGNO(MsgBriggsSMen),
    0x8d15,
    (s32)0xffff0014,
    MSGNO(MsgNowSThe),
    0xc400,
    (s32)0xffff0015,
    (s32)Func_02000098,
    0,
    (s32)0xffff0015,
    MSGNO(MsgWeStillHaven),
    0,
    (s32)0xffff0016,
    MSGNO(MsgItSNice),
    0x8d15,
    (s32)0xffff0015,
    MSGNO(MsgMostPeopleHere),
    0x8d15,
    (s32)0xffff0016,
    MSGNO(MsgTheOnlyWay),
    -1,
    0,
    0,
};

s32 gData_02000780[] = {
    1,
    (s32)0xffff0008,
    8,
    1,
    (s32)0xffff0009,
    9,
    0,
    (s32)0xffff0008,
    MSGNO(MsgDidYouKnow),
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgIMSoGrateful),
    -1,
    0,
    0,
};
