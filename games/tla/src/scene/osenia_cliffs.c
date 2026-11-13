/// @file
/// Overlay 0x65c's scene: its code, then its tables, in the order the ROM has them.

#include "shared/tables.h"
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
#include "event_inline.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "frame.h"
#include "module/scene/osenia_cliffs.h"
#include "module/scene/scene.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_020001a0(s32 a, s32 b, s32 c);

void Func_02000054(void) {
  s32 a1 = 13;
  s32 a2 = 14;

  /* FAKEMATCH(barrier): the asm on the later arguments loads them before the
   * first, as the native does, where without it the first loads earlier; as
   * Func_020001a0(1, 13, 14), 2 of 10 instructions differ in tla-en. */
  asm volatile("" : "+l"(a1), "+l"(a2));
  Func_020001a0(1, a1, a2);
  GameFlag_SetBit(0x203);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Func_02000190(void);

s32 Scene_Initialize(void) {
  struct MapBits *map = (struct MapBits *)Ram_HeapSlots->mapWork;

  map->mode = 1;
  gData_020001b0[0] = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 256;
  Task_Add(Func_020000fc, 0x480);
  {
    u8 *flags =
        (u8 *)((void *)Actor_GetById(gMainRoutineWork.currentOwner)) + 35;
    u32 value = *flags;
    u32 bit = 32;

    /* FAKEMATCH(barrier): the asm on the bit builds it in the register it ORs
     * into, as the native does, where without it the OR goes into the loaded
     * byte; as *flags |= 32, 4 of 55 instructions differ in tla-en. */
    __asm__("" : "+l"(bit));
    *flags = bit | value;
  }
  Func_02000190();
  {
    s32 a2 = 11;
    s32 a1 = 12;

    /* FAKEMATCH(barrier): the asm on the arguments loads them last to first, as
     * the native does, where without it the first loads before the second; as
     * Func_02000198(0, 12, 11), 2 of 55 instructions differ in tla-en. */
    asm volatile("" : "+l"(a2), "+l"(a1));
    Func_02000198(0, a1, a2);
  }
  Actor_SetModeById(12, 4);
  Actor_SetModeById(11, 4);
  {
    s32 flag = 0x104;
    s32 a0;

    a0 = 1;
    /* FAKEMATCH(barrier): the asm on the first argument keeps its load right
     * after the first half of flag + 0xff, where without it it loads last; as
     * Func_020001a8(1, flag + 0xff, 13, 14), 2 of 55 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    Func_020001a8(a0, flag + 0xff, 13, 14);
  }
  return 0;
}

/// The palette cycle Scene_Initialize schedules: every 16 frames it copies four
/// of the eight colours at 0x05000180, starting one further each time, to the
/// colours at 0x050000c2.
void Func_020000fc(void) {
  if ((gFrameCount & 15) == 0) {
    volatile u16 *colors;
    s32 i;

    if (gData_020001b0[0] == 8)
      gData_020001b0[0] = 0;
    i = gData_020001b0[0]++;
    colors = (u16 *)0x05000180;
    *(u16 *)0x050000c2 = colors[i];
    *(u16 *)0x050000c4 = colors[(i + 1) & 7];
    *(u16 *)0x050000c6 = colors[(i + 2) & 7];
    *(u16 *)0x050000c8 = colors[(i + 3) & 7];
  }
}

s32 Scene_PrepareMap(void) { return 0; }

#include "text/msg_ids.h"

u32 gData_020001b0[] = {
    0,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

DEFINE_G_SCENE_EXITS_5(54, 12, 2, 13)

struct ActorPlacement gActorPlacements[] = {
    {192, 0x8ff, 1, PIXELS(200), 0, PIXELS(226), FACING_EAST, 1, 0},
    {187, 0x8ff, 1, PIXELS(259), 0, PIXELS(312), FACING_NORTH, 1, 0},
    {190, 0x8ff, 1, PIXELS(242), 0, PIXELS(313), FACING_NORTH, 1, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(240), 0, PIXELS(184), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(184), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(104), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(104), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgMadraChampaBoat)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgMadraSailorNotChampa)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgMadraWaveDamage)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgMadraCannotEscapeBySea)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgMadraBriggsNewShip)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgMadraPirateAttackParties)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 13, 0x203, EVENT(Func_02000054)},
    {TRIGGERS_END},
};
