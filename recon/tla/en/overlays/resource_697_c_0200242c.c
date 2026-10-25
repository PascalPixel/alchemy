// Draft, differ 902 of 879: the Trial Road's map setup. In room 1, by entrance:
// 31 and 32 (or 33 and 34, with a different start) deal the stage order once
// from the leader's stats (flag 896), reset the stage and load its pattern;
// 50 deals and resets for the race; others set up the course: its two scrolled
// layers and their cells, either the course's mirrored halves (odd entrance)
// or the plain course with the chests placed, the stage's pieces from
// gData_02005bac, the unused pieces put away, the leader set on the ground,
// and with flag 2559 the stage resumed (Func_02000218, flags 642 and 644, the
// link display, the stage's music and its frame counter). In room 2, with
// flag 2559 and not 901, it stamps the finish by mirroring. Returns 0.
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"
#include "iwram_call.h"

struct TrialRoadStage {
  s16 order[8];
  s16 pattern;
  s16 mirrored;
  s16 timer;
  s16 current;
  s16 stage;
  s16 unk1a;
  s16 best;
};

extern char SceneId_Ov697Room1;
extern char SceneId_Ov697Room2;
extern char SceneId_Ov697Room3;
extern char SceneId_Ov697Room4;
extern u8 gMapCellBuffer[];
extern u8 gMapShapeGrid[];
extern const s32 *const gData_02005b84[];
extern const s16 gData_02005bac[][2];
extern const s8 gData_02005bcc[];
extern const s8 gData_02005bd6[];

u8 *Unit_Get(s32 id);
void Func_02002c30(u32 seed);
void Func_020056c4(const s32 *pattern);
void Func_02005988(s32 scene);
void Func_02005828(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_0200217c(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                   s32 dstY);
void Func_020057e8(void);
void Func_020055c4(s32 a, s32 b);
void Func_02005688(s32 side);
void Func_0200223c(void);

static __inline__ void Deal(struct TrialRoadStage *state) {
  u8 *unit;
  u8 *p;
  u32 sum = 0;

  unit = Unit_Get(gPartyState.currentOwner);
  Iwram_ClearWords(state, 32);
  for (p = unit; p <= unit + 13; p++)
    sum += *p;
  Func_02002c30(sum);
}

static __inline__ void SetLayer(u8 *map, s32 offset, s32 x, u8 *cells,
                                u8 *shapes) {
  s32 *layer = (s32 *)(map + offset);

  layer[2] = x;
  layer[12] = (s32)cells;
  layer[3] = 0;
  layer[13] = (s32)shapes;
  layer[0] = Iwram_MulQ16(*(s32 *)(map + 228), layer[4]) + layer[2];
  layer[1] = Iwram_MulQ16(*(s32 *)(map + 232), layer[5]) + layer[3];
}

s32 Func_0200242c(void) {
  struct TrialRoadStage *state = (struct TrialRoadStage *)gTrialRoadState;
  u8 *map;
  s32 scene = gPartyState.scene;
  s32 entrance = gPartyState.entrance;
  struct ObjectRuntime *leader;
  s32 piece;
  s32 px;
  s32 pz;
  u32 i;

  if (scene != (s32)&SceneId_Ov697Room1) {
    if (scene == (s32)&SceneId_Ov697Room3 ||
        scene == (s32)&SceneId_Ov697Room4)
      return 0;
    if (scene == (s32)&SceneId_Ov697Room2 && GameFlag_Test(2559) &&
        !GameFlag_Test(901)) {
      if (state->mirrored != 0)
        Map_Stamp(26, 79, 1, 2, 13, 79);
      else
        Map_Stamp(26, 79, 1, 2, 18, 79);
    }
    return 0;
  }
  map = (u8 *)Ram_HeapSlots->mapWork;
  if ((u32)(entrance - 31) <= 1) {
    if (!GameFlag_Test(896)) {
      Deal(state);
      GameFlag_SetBit(896);
    }
    state->mirrored = (entrance - 1) & 1;
    state->stage = 0;
    state->current = 0;
    state->unk1a = 0;
    state->best = 0;
    Func_020056c4(gData_02005b84[state->order[state->mirrored ^ 1] & 7]);
    state->pattern = state->order[state->stage * 2 + state->mirrored];
    Func_02005988(entrance % 10);
    return 0;
  }
  if ((u32)(entrance - 33) <= 1) {
    if (!GameFlag_Test(896)) {
      Deal(state);
      GameFlag_SetBit(896);
    }
    state->mirrored = (entrance - 1) & 1;
    state->stage = 3;
    state->current = 3;
    state->unk1a = 4;
    state->best = 4;
    Func_020056c4(
        gData_02005b84[state->order[(state->mirrored ^ 1) + 6] & 7]);
    state->pattern = state->order[state->stage * 2 + state->mirrored];
    Func_02005988(entrance % 10);
    return 0;
  }
  if (entrance == 50) {
    Deal(state);
    state->mirrored = 0;
    state->stage = 0;
    state->current = 0;
    state->unk1a = 0;
    state->best = 0;
    state->pattern = state->order[0];
    Func_02005988(55);
    return 0;
  }
  SetLayer(map, 320, 136 << 18, gMapCellBuffer + 136, gMapShapeGrid + 34);
  SetLayer(map, 376, 136 << 19, gMapCellBuffer + 272, gMapShapeGrid + 68);
  if (entrance == 55) {
    for (i = 9; i <= 12; i++)
      Object_SetPartAttribute(Object_GetById(i), 3);
    Func_02005828(0, 12, 1, 21, 0, 13);
    Func_02005828(34, 12, 1, 21, 34, 13);
    Func_02005828(68, 12, 1, 21, 68, 13);
    Func_02005828(0, 13, 33, 21, 1, 13);
    Func_02005828(34, 13, 33, 21, 35, 13);
    Func_02005828(68, 13, 33, 21, 69, 13);
  } else {
    if (entrance & 1) {
      Func_0200217c(0, 102, 13, 9, 0, 0);
      Func_02005828(0, 0, 10, 1, 0, 1);
      Func_02005828(0, 0, 10, 12, 0, 1);
      Func_02005828(34, 0, 10, 12, 34, 1);
      Func_02005828(68, 0, 10, 12, 68, 1);
      Func_0200217c(0, 34, 102, 10, 9, 34);
      Func_02005828(0, 33, 10, 10, 0, 34);
      Func_02005828(34, 33, 10, 10, 34, 34);
      Func_02005828(68, 33, 10, 10, 68, 34);
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
    for (i = 9; i <= 12; i++)
      Object_SetPartAttribute(Object_GetById(i), 3);
  }
  piece = state->pattern;
  px = gData_02005bac[piece][0];
  pz = gData_02005bac[piece][1];
  Func_02005828(px, pz, 13, 21, 10, 13);
  Func_02005828(px + 42, pz, 13, 21, 44, 13);
  Func_02005828(px + 84, pz, 13, 21, 78, 13);
  for (i = 0; i <= 9; i++) {
    if ((entrance == 55 ? gData_02005bcc : gData_02005bd6)[i] != piece)
      Func_020058e0(i + 64, 0, 0);
  }
  Func_020057e8();
  if (entrance == 55)
    return 0;
  leader = Object_GetById(gPartyState.currentOwner);
  if (!GameFlag_Test(265))
    leader->terrainHeight = leader->y =
        Map_GetTerrainHeight(0, leader->x, leader->z);
  if (GameFlag_Test(2559)) {
    GameFlag_SetBit(324);
    if (state->stage >= state->unk1a || GameFlag_Test(642)) {
      Func_02000218(state->mirrored, state->stage);
      GameFlag_SetBit(642);
      GameFlag_SetBit(644);
    }
    if (state->stage < state->best)
      Func_020002a4(state->mirrored);
    Func_020055c4(gPartyState.currentOwner, 8);
    Func_02005688((s16)(state->mirrored ^ 1));
    Scheduler_AddOrUpdateCallback(Func_0200223c, 0x480);
  } else {
    Func_02000218(state->mirrored, state->stage);
    GameFlag_SetBit(642);
    GameFlag_SetBit(644);
    Func_020002a4(state->mirrored, state->stage);
  }
  return 0;
}
