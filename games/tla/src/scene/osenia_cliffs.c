/// @file
/// The Osenia Cliffs, across the Madra Drawbridge, where the wreck of a Champa
/// ship lies on the shore and three people from Madra look it over.
///
/// Four rope posts make two ropes. The first is tied as the scene starts; the
/// party ties the second with Lash (OseniaCliffs_LashRope).

#include "shared/tables.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "system/io_reg.h"
#include "scene_tables.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "system/frame.h"
#include "module/scene/scene.h"

extern u32 gOseniaCliffs_PaletteCycleStep[];

#ifdef MODERN
void Rope_Tie(s32 a, s32 b, s32 c);
#endif

void Rope_Place(s32 a, s32 flag, s32 b, s32 c);

void OseniaCliffs_CyclePalette(void);

/// Set once the party has lashed the second rope, which stays tied for the
/// visit.
#define FLAG_ROPE_LASHED 0x203

/// The actors, numbering the rows of gActorPlacements.
enum OseniaCliffsActorId {
  ACTOR_VILLAGER_CHAMPA_BOAT = ACTOR_FIRST_PLACED,
  ACTOR_VILLAGER_NOT_CHAMPA,
  ACTOR_VILLAGER_WAVE_DAMAGE,
  /// The posts of the first rope, tied as the scene starts.
  ACTOR_FIRST_ROPE_POST,
  ACTOR_SECOND_ROPE_POST,
  /// The posts of the rope the party ties with Lash.
  ACTOR_THIRD_ROPE_POST,
  ACTOR_FOURTH_ROPE_POST
};

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

#ifdef MODERN
void Rope_Lash(s32 a, s32 b, s32 c);
#endif

/// The party lashes the second rope, from post ACTOR_THIRD_ROPE_POST to
/// ACTOR_FOURTH_ROPE_POST, and it stays tied for the rest of the visit.
void OseniaCliffs_LashRope(void) {
  Rope_Lash(1, 13, 14);
  Event_SetFlag(FLAG_ROPE_LASHED);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Rope_Initialize(void);

s32 Scene_Initialize(void) {
  struct MapBits *map = (struct MapBits *)Ram_HeapSlots->mapWork;

  map->mode = 1;
  gOseniaCliffs_PaletteCycleStep[0] = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 256;
  Task_Add(OseniaCliffs_CyclePalette, 0x480);
  {
    u8 *flags =
        (u8 *)((void *)Actor_GetById(gMainRoutineWork.currentOwner)) + 35;
    u32 value = *flags;
    u32 bit = 32;

    /* FAKEMATCH(barrier): the asm on the bit builds it in the register it ORs
     * into. */
    __asm__("" : "+l"(bit));
    *flags = bit | value;
  }
  Rope_Initialize();
  Rope_Tie(0, 12, 11);
  Actor_SetAnimation(ACTOR_SECOND_ROPE_POST, 4);
  Actor_SetAnimation(ACTOR_FIRST_ROPE_POST, 4);
  {
    s32 flag = 0x104;
    s32 a0;

    a0 = 1;
    /* FAKEMATCH(barrier): the asm on the first argument keeps its load right
     * after the first half of flag + 0xff, where without it it loads last. */
    asm volatile("" : "+l"(a0));
    Rope_Place(a0, flag + 0xff, 13, 14);
  }
  return 0;
}

/// The palette cycle Scene_Initialize schedules: every 16 frames it copies four
/// of the eight colours at 0x05000180, starting one further each time, to the
/// colours at 0x050000c2.
void OseniaCliffs_CyclePalette(void) {
  if ((gFrameCount & 15) == 0) {
    volatile u16 *colors;
    s32 i;

    if (gOseniaCliffs_PaletteCycleStep[0] == 8)
      gOseniaCliffs_PaletteCycleStep[0] = 0;
    i = gOseniaCliffs_PaletteCycleStep[0]++;
    colors = (u16 *)&BG_PLTT_COLOR(12, 0);
    BG_PLTT_COLOR(6, 1) = colors[i];
    BG_PLTT_COLOR(6, 2) = colors[(i + 1) & 7];
    BG_PLTT_COLOR(6, 3) = colors[(i + 2) & 7];
    BG_PLTT_COLOR(6, 4) = colors[(i + 3) & 7];
  }
}

s32 Scene_PrepareMap(void) { return 0; }

#include "text/msg_ids.h"

u32 gOseniaCliffs_PaletteCycleStep[] = {
    0,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(OseniaCliffs),
    SCENE_EXIT_UNLESS(1, WorldMap, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, WorldMap, 13, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(192, 0x8ff, MOVEMENT_TYPE_STAND, 200, 0, 226, FACING_EAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(187, 0x8ff, MOVEMENT_TYPE_STAND, 259, 0, 312, FACING_NORTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(190, 0x8ff, MOVEMENT_TYPE_STAND, 242, 0, 313, FACING_NORTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 240, 0, 184,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 104,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 104,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TALK, ACTOR_VILLAGER_CHAMPA_BOAT, CONDITION_ALWAYS,
     MSGNO(MsgMadraChampaBoat)},
    {TRIGGER_TALK, ACTOR_VILLAGER_NOT_CHAMPA, CONDITION_ALWAYS,
     MSGNO(MsgMadraSailorNotChampa)},
    {TRIGGER_TALK, ACTOR_VILLAGER_WAVE_DAMAGE, CONDITION_ALWAYS,
     MSGNO(MsgMadraWaveDamage)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_CHAMPA_BOAT,
     CONDITION_ALWAYS, MSGNO(MsgMadraCannotEscapeBySea)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_NOT_CHAMPA,
     CONDITION_ALWAYS, MSGNO(MsgMadraBriggsNewShip)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WAVE_DAMAGE,
     CONDITION_ALWAYS, MSGNO(MsgMadraPirateAttackParties)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_THIRD_ROPE_POST, FLAG_ROPE_LASHED,
     EVENT(OseniaCliffs_LashRope)},
    {TRIGGER_TABLE_END},
};
