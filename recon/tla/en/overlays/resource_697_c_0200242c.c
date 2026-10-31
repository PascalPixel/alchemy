// Near miss, differ 366 of 879 (tla-en, 2026-10-31): the control flow, calls,
// arguments and field accesses are the native's; what is left is allocation.
// The native keeps map in r8, entrance in r10, a zero in r9 across both
// layers and the calls after them, the columns 34 and 68 in r10 and r8, and
// reloads Iwram_MulQ16's address for each call; this form puts entrance in
// r8 and map in r10, keeps the routine's address in r9 across the four
// calls, keeps the zero (in r8) only on entrance 55, holds 68 in r10 and
// rematerializes 34, and the deal's sum and pattern take r7 where the native
// has r6 (px r6 for r7). The stage-resume test's field16[0] is moved into
// both predecessors by GCSE (ldrh, then lsls/asrs at the call) where the
// native reloads it with ldrsh at the call.
//
// The Trial Road's map setup. In room 1, by entrance:
// 31 and 32 (or 33 and 34, with a different start) deal the stage order once
// from the leader's stats (flag 896), reset the stage and load its pattern;
// 50 deals and resets for the race; others set up the course: its two scrolled
// layers and their cells, either the course's mirrored halves (odd entrance)
// or the plain course with the chests placed, the stage's pieces from
// gData_02005bac, the unused pieces put away, the leader set on the ground,
// and with flag 2559 the stage resumed (Func_02000218 when field16[0] is
// below field1a or flag 642 is set, flags 642 and 644, the link display, the
// stage's music and its frame counter). In room 2, with flag 2559 and not 901,
// it stamps the finish by mirroring. Returns 0.
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"
#include "iwram_call.h"
#include "map.h"
#include "event_work.h"

extern char SceneId_Ov697Room1;
extern char SceneId_Ov697Room2;
extern char SceneId_Ov697Room3;
extern char SceneId_Ov697Room4;
extern u8 gMapShapeGrid[];
extern const s32 *const gData_02005b84[];
extern const s16 gData_02005bac[][2];
extern const s8 gData_02005bcc[];
extern const s8 gData_02005bd6[];

u8 *Unit_Get(s32 id);
void Func_02002c30(u32 seed);
void Func_020056c4(s32 a);
void Func_02005828(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_0200217c(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                   s32 dstY);
void Func_020057e8(void);
void Func_020055c4(s32 a, s32 b);
void Func_02005688(s32 a);
void Func_0200223c(void);
void Func_02000218(s32 side, s32 phase);
void Func_020002a4(s32 side, s32 phase);

// The state as this function reads it: its first 16 bytes are the dealt order.
struct TrialRoadDeal {
  s16 order[8];
  s16 pattern;
  s16 mirrored;
  s16 field14;
  s16 field16[2];
  s16 field1a;
  s16 field1c;
};

#define ORDER(state) ((state)->order)

// Places `layer` `column` cells into the map: its offset (column << 20), its
// cells and terrain from that column on, and its position from the view. The
// offset takes the literal, the cells and terrain the column's variable:
// with the variable in the offset too, CSE keeps a shift of the register
// (lsls rN, #20) where the native stores the folded constant (33 more
// differ); with the literal in the cells, the buffer and offset fold into
// one literal where the native adds 136 to the buffer's address (20 more);
// through gMapCellBuffer's name rather than Ram_MapCellBuffer, the address
// stays in a register across the calls where the native reloads it (86
// more).
#define SET_LAYER(column, x)                                                   \
  words = (s32 *)layer;                                                        \
  x = (column);                                                                \
  words[2] = (column) << 20;                                                   \
  layer->cells = (struct MapCell *)Ram_MapCellBuffer + x;                      \
  words[3] = 0;                                                                \
  layer->terrainIds = gMapShapeGrid + x;                                       \
  words[0] = Iwram_MulQ16(*viewX, words[4]) + words[2];                        \
  words[1] = Iwram_MulQ16(*viewZ, words[5]) + words[3]

s32 Func_0200242c(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;

  if (scene == (s32)&SceneId_Ov697Room1) {
    struct MapWork *map = Ram_HeapSlots->mapWork;
    struct TrialRoadDeal *state = (struct TrialRoadDeal *)gTrialRoadState;
    struct Actor *leader;
    u8 *unit;
    s32 value;
    s32 n;
    s32 px;
    s32 pz;
    u32 i;

    if ((u32)(entrance - 31) <= 1) {
      if (!GameFlag_Test(896)) {
        unit = Unit_Get(gMainRoutineWork.currentOwner);
        Iwram_ClearWords(state, 32);
        value = 0;
        for (n = 0; n <= 13; n++)
          value += unit[n];
        Func_02002c30(value);
        GameFlag_SetBit(896);
      }
      state->mirrored = (entrance - 1) & 1;
      state->field16[0] = 0;
      state->field16[1] = 0;
      state->field1a = 0;
      state->field1c = 0;
      value = ORDER(state)[state->field16[0] * 2 + (state->mirrored ^ 1)];
      Func_020056c4((s32)gData_02005b84[value & 7]);
      state->pattern = ORDER(state)[state->field16[0] * 2 + state->mirrored];
      n = entrance % 10;
      Event_SetDestination((s32)&SceneId_Ov697Room1, n);
    } else if ((u32)(entrance - 33) <= 1) {
      if (!GameFlag_Test(896)) {
        unit = Unit_Get(gMainRoutineWork.currentOwner);
        Iwram_ClearWords(state, 32);
        value = 0;
        for (n = 0; n <= 13; n++)
          value += unit[n];
        Func_02002c30(value);
        GameFlag_SetBit(896);
      }
      state->mirrored = (entrance - 1) & 1;
      state->field16[0] = 3;
      state->field16[1] = 3;
      state->field1a = 4;
      state->field1c = 4;
      value = ORDER(state)[state->field16[0] * 2 + (state->mirrored ^ 1)];
      Func_020056c4((s32)gData_02005b84[value & 7]);
      state->pattern = ORDER(state)[state->field16[0] * 2 + state->mirrored];
      n = entrance % 10;
      Event_SetDestination((s32)&SceneId_Ov697Room1, n);
    } else if (entrance == 50) {
      unit = Unit_Get(gMainRoutineWork.currentOwner);
      Iwram_ClearWords(state, 32);
      value = 0;
      for (n = 0; n <= 13; n++)
        value += unit[n];
      Func_02002c30(value);
      state->mirrored = 0;
      state->field16[0] = 0;
      state->field16[1] = 0;
      state->field1a = 0;
      state->field1c = 0;
      state->pattern = ORDER(state)[0];
      Event_SetDestination((s32)&SceneId_Ov697Room1, 55);
    } else {
      s32 *viewX = &map->viewX;
      s32 *viewZ = &map->viewZ;
      struct MapLayer *layer;
      s32 *words;
      s32 x1;
      s32 x2;

      if (entrance == 55) {
        layer = &map->layers[1];
        SET_LAYER(34, x1);
        layer = &map->layers[2];
        SET_LAYER(68, x2);
        Actor_SetPartAttribute(Actor_GetById(9), 3);
        Actor_SetPartAttribute(Actor_GetById(10), 3);
        Actor_SetPartAttribute(Actor_GetById(11), 3);
        Actor_SetPartAttribute(Actor_GetById(12), 3);
        Func_02005828(0, 12, 1, 21, 0, 13);
        Func_02005828(x1, 12, 1, 21, x1, 13);
        Func_02005828(x2, 12, 1, 21, x2, 13);
        Func_02005828(0, 13, 33, 21, 1, 13);
        Func_02005828(x1, 13, 33, 21, x1 + 1, 13);
        Func_02005828(x2, 13, 33, 21, x2 + 1, 13);
        value = state->pattern;
        px = gData_02005bac[value][0];
        pz = gData_02005bac[value][1];
        Func_02005828(px, pz, 13, 21, 1, 13);
        Func_02005828(px + 42, pz, 13, 21, x1 + 1, 13);
        Func_02005828(px + 84, pz, 13, 21, x2 + 1, 13);
        // The native neither hoists the table's address nor strength-reduces
        // the index; as a for loop, 10 more instructions differ (376).
        i = 0;
      hideUnused55:
        if (gData_02005bcc[i] != value)
          Func_020058e0(i + 64, 0, 0);
        i++;
        if (i <= 9)
          goto hideUnused55;
        Func_020057e8();
      } else {
        layer = &map->layers[1];
        SET_LAYER(34, x1);
        layer = &map->layers[2];
        SET_LAYER(68, x2);
        if (entrance & 1) {
          Func_0200217c(0, 0, 102, 13, 9, 0);
          Func_02005828(0, 0, 10, 1, 1, 0);
          Func_02005828(0, 0, 10, 12, 0, 1);
          Func_02005828(x1, 0, 10, 12, x1, 1);
          Func_02005828(x2, 0, 10, 12, x2, 1);
          Func_0200217c(0, 34, 102, 10, 9, 34);
          Func_02005828(0, 33, 10, 10, 0, 34);
          Func_02005828(x1, 33, 10, 10, x1, 34);
          Func_02005828(x2, 33, 10, 10, x2, 34);
        } else {
          Func_02005828(10, 0, 92, 13, 0, 0);
          Func_02005828(23, 0, 10, 1, 24, 0);
          Func_02005828(23, 0, 10, 12, 23, 1);
          Func_02005828(57, 0, 10, 12, 57, 1);
          Func_02005828(91, 0, 10, 12, 91, 1);
          Func_02005828(10, 34, 92, 10, 0, 34);
          Func_02005828(24, 33, 10, 10, 24, 34);
          Func_02005828(58, 33, 10, 10, 58, 34);
          Func_02005828(92, 33, 10, 10, 92, 34);
          Func_020058e0(9, 160 << 14, 152 << 16);
          Func_020058e0(10, 192 << 13, 152 << 16);
          Func_020058e0(11, 184 << 16, 152 << 16);
          Func_020058e0(12, 200 << 16, 152 << 16);
        }
        Actor_SetPartAttribute(Actor_GetById(9), 3);
        Actor_SetPartAttribute(Actor_GetById(10), 3);
        Actor_SetPartAttribute(Actor_GetById(11), 3);
        Actor_SetPartAttribute(Actor_GetById(12), 3);
        value = state->pattern;
        px = gData_02005bac[value][0];
        pz = gData_02005bac[value][1];
        Func_02005828(px, pz, 13, 21, 10, 13);
        Func_02005828(px + 42, pz, 13, 21, 44, 13);
        Func_02005828(px + 84, pz, 13, 21, 78, 13);
        // A goto loop for the same reason as the one above.
        i = 0;
      hideUnused:
        if (gData_02005bd6[i] != value)
          Func_020058e0(i + 64, 0, 0);
        i++;
        if (i <= 9)
          goto hideUnused;
        Func_020057e8();
        leader = Actor_GetById(gMainRoutineWork.currentOwner);
        if (!GameFlag_Test(265))
          leader->y = leader->terrainHeight =
              Map_GetTerrainHeight(0, leader->x, leader->z);
        if (GameFlag_Test(2559)) {
          struct TrialRoadState *road;

          GameFlag_SetBit(324);
          // A pointer of its own keeps the state's address in r5 across the
          // calls; through gTrialRoadState each test reloads it (5 more).
          road = gTrialRoadState;
          if (road->field16[0] < road->field1a || GameFlag_Test(642)) {
            Func_02000218(road->mirrored, road->field16[0]);
            GameFlag_SetBit(642);
            GameFlag_SetBit(644);
          }
          if (road->field16[0] < road->field1c)
            Func_020002a4(road->mirrored, road->field16[0]);
          Func_020055c4(gMainRoutineWork.currentOwner, 8);
          Func_02005688((s16)(state->mirrored ^ 1));
          Task_Add(Func_0200223c, 0x480);
        } else {
          Func_02000218(state->mirrored, state->field16[0]);
          GameFlag_SetBit(642);
          GameFlag_SetBit(644);
          Func_020002a4(state->mirrored, state->field16[0]);
        }
      }
    }
  } else if (scene == (s32)&SceneId_Ov697Room3) {
  } else if (scene == (s32)&SceneId_Ov697Room4) {
  } else if (scene == (s32)&SceneId_Ov697Room2 && GameFlag_Test(2559) &&
             !GameFlag_Test(901)) {
    if (gTrialRoadState->mirrored != 0)
      Map_Stamp(26, 79, 1, 2, 13, 79);
    else
      Map_Stamp(26, 79, 1, 2, 18, 79);
  }
  return 0;
}
