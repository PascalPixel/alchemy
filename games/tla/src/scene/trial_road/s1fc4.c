#include "trial_road.h"
#include "system.h"

#include "iwram_call.h"
#include "io_reg.h"

extern s32 gData_02007e20;
extern s32 gData_02007e24;
extern u8 gData_02007e28[];

void Func_02001f08(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Words_Copy(void *destination, const void *source,
                                  s32 size) {
  Iwram_CopyWords(destination, source, size);
}

void Func_02001fc4(s32 mode) {
  gData_02007e24 = 1;
  if (mode != 2)
    Words_Copy(gData_02007e28, (void *)&BG_PLTT_COLOR(5, 0), 32);
  if (mode == 1) {
    /* FAKEMATCH(temporary): a local holding the zero builds it in r2 and the
     * counter address in r3, as overlay 699's twin does; as
     * gData_02007e20 = 0, 6 of 25 instructions differ in tla-en. */
    s32 zero = 0;

    gData_02007e20 = zero;
    Task_Add(Func_02001f08, 0x480);
  }
}

#include "types.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "map.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "field.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02001754();
void Func_02002db0(void);
void Func_02002db4(void);
void Func_020034b4(void);
void Func_020037c8(void);
void Func_02005868();

void Func_0200200c(void) {
  s32 t7, t8, t10, t13, t16, p3;

  if (FIELD_AT(&gMainRoutineWork, s16, 482) != 4) {
    p3 = FIELD_AT(&gMainRoutineWork, s16, 482);
    goto L_0200a02e;
  }
  Func_02005868();
  Func_020034b4();
  Import_EventSetExitRequest(4);
  goto L_0200a07c;
L_0200a02e:
  if (p3 != 5)
    goto L_0200a042;
  Func_02005868();
  Func_020037c8();
  Import_EventSetExitRequest(5);
  goto L_0200a07c;
L_0200a042:
  t7 = GameFlag_Test(0x9ff);
  if (t7 == 0)
    goto L_0200a07c;
  t8 = GameFlag_Test(0x283);
  if (t8 == 0)
    goto L_0200a064;
  Func_02001754(0);
L_0200a064:
  t10 = GameFlag_Test(0x385);
  if (t10 == 0)
    goto L_0200a078;
  Func_02002db0();
  goto L_0200a07c;
L_0200a078:
  Func_02002db4();
L_0200a07c:
  t13 = ((s32)Actor_GetById(gMainRoutineWork.currentOwner));
  FIELD_AT(t13, u8, 35) = (64 | FIELD_AT(t13, u8, 35));
  Func_02001fc4(1);
  Func_020006b4(10);
  t16 = GameFlag_Test(0x9fc);
  if (t16 == 0)
    goto L_0200a0bc;
  Actor_SetPosition(10, PIXELS(136), PIXELS(328));
L_0200a0bc:
}

extern s32 (*const gData_02005b64[])(void);

void Func_02003900(void);
void Func_02001dd0(void);
void Func_02001eb0(void);

s32 Func_020020c4(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  s32 pattern = gTrialRoadState->pattern;

  if (*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 344) != 0)
    return 0;
  if (scene == (s32)&SceneId_Ov697Room1) {
    if (entrance <= 7) {
      GameFlag_SetBit(324);
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
      gData_02005b64[pattern & 7]();
    } else if (entrance == 55) {
      Func_02003900();
    }
  }
  if (scene == (s32)&SceneId_Ov697Room3)
    Func_02001dd0();
  if (scene == (s32)&SceneId_Ov697Room4)
    Func_02001eb0();
  if (scene == (s32)&SceneId_Ov697Room2) {
    GameFlag_SetBit(324);
    Func_0200200c();
  }
  return 0;
}

extern u8 gMapShapeGrid[];

void Func_0200217c(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                   s32 dstY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  s32 dst = (dstY << 7) + dstX;
  u8 *dstShapes = gMapShapeGrid + dst;
  s32 src = (srcY << 7) + srcX;
  u8 *srcShapes = gMapShapeGrid + src;
  u32 *dstCells = cells + dst;
  s16 row;
  s16 col;

  cells += src;
  for (row = height - 1; row >= 0; row--) {
    u32 *dstCell = dstCells + ((row << 7) + width) - 1;
    u32 *srcCell = cells + ((row << 7) + width) - 1;
    u8 *dstShape = dstShapes + (row << 7) + width - 1;
    u8 *srcShape = srcShapes + (row << 7) + width - 1;

    for (col = width - 1; col >= 0; col--) {
      *dstCell-- = *srcCell--;
      *dstShape = *srcShape;
      dstShape--;
      srcShape--;
    }
  }
}

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

extern const s32 *const gData_02005b84[];

s32 Func_020056e8(void);
s32 Func_020000fc(s32 value);
void Func_020056c4(s32 a);
void Func_020002a4(s32 mirrored, s32 stage);

void Func_0200223c(void) {
  struct TrialRoadStage *state = (struct TrialRoadStage *)gTrialRoadState;
  struct TrialRoadState *road = gTrialRoadState;
  s32 side;

  if (state->stage > 3)
    return;
  if (Func_020056e8() != 0) {
    if (state->timer == 0) {
      side = Func_020000fc(1) ^ 1;
      Func_020056c4(gData_02005b84[side + 8]);
      state->timer = side * 5 * 120 + 1;
    } else {
      if (state->stage >= state->best)
        state->best = state->stage + 1;
      if (++road->field16[1] <= 3) {
        s32 pattern =
            state->order[state->stage * 2 + (s16)(state->mirrored ^ 1)];

        Func_020056c4(gData_02005b84[pattern & 7]);
        Actor_GetById(8)->terrainHeight = 0;
        Actor_GetById(8)->y = 0;
      } else {
        Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
      }
      state->timer = 0;
    }
  }
  {
    u8 *kind = &Actor_GetById(8)->animationKind;

    if (state->current == state->stage)
      *kind = 1;
    else
      *kind = 0;
  }
  if (state->timer == 0)
    return;
  if (GameFlag_Test(260))
    return;
  if (state->current == state->stage) {
    switch (state->timer) {
    case 1:
    case 601:
      Sound_Play(124);
      if (state->mirrored != 0) {
        Actor_SetModeById(9, 6);
        if (state->timer > 600)
          Actor_SetModeById(10, 6);
      } else {
        Actor_SetModeById(11, 6);
        if (state->timer > 600)
          Actor_SetModeById(12, 6);
      }
      break;
    case 655:
      Sound_Play(125);
      if (state->mirrored != 0)
        Actor_SetModeById(10, 5);
      else
        Actor_SetModeById(12, 5);
      break;
    case 653:
    case 708:
      Actor_GetById(8)->angle = 0xc000;
      break;
    case 50:
    case 710:
      Sound_Play(125);
      if (state->mirrored != 0)
        Actor_SetModeById(9, 5);
      else
        Actor_SetModeById(11, 5);
      break;
    case 70:
    case 730:
      Func_020002a4(state->mirrored, state->stage);
      Sound_Play(138);
      break;
    }
  }
  road->field14++;
}

extern const s16 gData_02005bac[][2];
extern const s8 gData_02005bcc[];
extern const s8 gData_02005bd6[];

void *Unit_Get(s32 owner);
void Func_02002c30(u32 seed);
void Func_02005828(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020057e8(void);
void Func_020055c4(s32 a, s32 b);
void Func_02005688(s32 a);
void Func_02000218(s32 side, s32 phase);

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

// Places `layer` `column` cells into the map: its offset (column << 20), its
// cells and terrain from that column on, and its position from the view. The
// offset takes the literal, the cells and terrain the column's variable:
// with the variable in the offset too, CSE keeps a shift of the register;
// with the literal in the cells, the buffer and offset fold into one literal;
// through gMapCellBuffer's name, the address stays in a register across calls.
#define SET_LAYER(column, x)                                                   \
  words = (s32 *)layer;                                                        \
  words[2] = (column) << 20;                                                   \
  words[3] = row;                                                              \
  x = (column);                                                                \
  layer->cells = (struct MapCell *)Ram_MapCellBuffer + x;                      \
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
      value = state->order[state->mirrored ^ 1];
      Func_020056c4((s32)gData_02005b84[value & 7]);
      state->pattern = state->order[state->field16[0] * 2 + state->mirrored];
      n = entrance % 10;
      Func_02005988((s32)&SceneId_Ov697Room1, n);
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
      {
        /* FAKEMATCH(temporary): side, for both the store and its flip; as
         * state->mirrored, 2 of 879 instructions differ in tla-en. */
        s16 side = (entrance - 1) & 1;

        state->mirrored = side;
        state->field16[0] = 3;
        state->field16[1] = 3;
        state->field1a = 4;
        state->field1c = 4;
        side ^= 1;
        {
          /* FAKEMATCH(temporary): index; as state->order[side + 6], 2 of 879
           * instructions differ in tla-en. */
          s32 index = side + 6;

          value = state->order[index];
        }
      }
      Func_020056c4((s32)gData_02005b84[value & 7]);
      state->pattern = state->order[state->field16[0] * 2 + state->mirrored];
      n = entrance % 10;
      Func_02005988((s32)&SceneId_Ov697Room1, n);
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
      state->pattern = state->order[0];
      Func_02005988((s32)&SceneId_Ov697Room1, 55);
    } else {
      s32 *viewX = &map->viewX;
      s32 *viewZ = &map->viewZ;
      struct MapLayer *layer;
      s32 *words;
      s32 row;

      if (entrance == 55) {
        s32 x1;
        s32 x2;

        row = 0;
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
        row = gData_02005bac[value][1];
        Func_02005828(px, row, 13, 21, 1, 13);
        Func_02005828(px + 42, row, 13, 21, x1 + 1, 13);
        Func_02005828(px + 84, row, 13, 21, x2 + 1, 13);
        // The native neither hoists the table's address nor strength-reduces
        // the index; as a for loop, 10 more instructions differ (376).
        i = 0;
      hideUnused55:
        if (gData_02005bcc[i] != value)
          Actor_SetHorizontalPositionWithTerrain(i + 64, 0, 0);
        i++;
        if (i <= 9)
          goto hideUnused55;
        Func_020057e8();
      } else {
        s32 x1;
        s32 x2;
        s32 destinationY;

        row = 0;
        layer = &map->layers[1];
        SET_LAYER(34, x1);
        layer = &map->layers[2];
        SET_LAYER(68, x2);
        value = 1;
        if (entrance & value) {
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
          Actor_SetPosition(9, 160 << 14, 152 << 16);
          Actor_SetPosition(10, 192 << 13, 152 << 16);
          Actor_SetPosition(11, 184 << 16, 152 << 16);
          Actor_SetPosition(12, 200 << 16, 152 << 16);
        }
        Actor_SetPartAttribute(Actor_GetById(9), 3);
        Actor_SetPartAttribute(Actor_GetById(10), 3);
        Actor_SetPartAttribute(Actor_GetById(11), 3);
        Actor_SetPartAttribute(Actor_GetById(12), 3);
        destinationY = 13;
        value = state->pattern;
        px = gData_02005bac[value][0];
        row = gData_02005bac[value][1];
        Func_02005828(px, row, 13, 21, 10, destinationY);
        Func_02005828(px + 42, row, 13, 21, 44, destinationY);
        Func_02005828(px + 84, row, 13, 21, 78, destinationY);
        // A goto loop for the same reason as the one above.
        i = 0;
      hideUnused:
        if (gData_02005bd6[i] != value)
          Actor_SetHorizontalPositionWithTerrain(i + 64, 0, 0);
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
          // The call's argument reads through the other struct: through the
          // same one GCSE moves the load into both predecessors (11 more of
          // gap).
          if (road->field16[0] < road->field1a || GameFlag_Test(642)) {
            Func_02000218(road->mirrored,
                          ((struct TrialRoadDeal *)road)->field16[0]);
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
      MapGrid_CopyCellsLayered(26, 79, 1, 2, 13, 79);
    else
      MapGrid_CopyCellsLayered(26, 79, 1, 2, 18, 79);
  }
  return 0;
}

// Deals the Trial Road's stage order from a seed: shuffles 0 to 7 into the
// eight slots at the start of the stage state, again until both the even and
// the odd slots hold at least one of the four values in gData_02005be0; the
// random state is restored after.

extern const s8 gData_02005be0[4];

/// Deals the Trial Road's stage order from a seed: shuffles 0 to 7 into the
/// eight slots at the start of the stage state, again until both the even and
/// the odd slots hold at least one of the four values in gData_02005be0; the
/// random state is restored after.
void Func_02002c30(u32 seed) {
  s16 *order = (s16 *)gTrialRoadState;
  u32 saved = gRandomState;
  s32 counts[2];
  s32 i;
  s32 k;
  s32 offset;
  s8 value;

  gRandomState = seed;
  do {
    for (i = 0; i <= 7; i++)
      order[i] = -1;
    for (i = 0; i <= 7; i++) {
      do {
        offset = ((Random16Far() << 3) >> 16) * 2;
      } while (*(s16 *)((u8 *)order + offset) != -1);
      *(s16 *)((u8 *)order + offset) = i;
    }
    counts[1] = 0;
    counts[0] = 0;
    for (i = 0; i <= 7; i++) {
      for (k = 0; k <= 3; k++) {
        value = gData_02005be0[k];
        if (order[i] == value)
          counts[i & 1]++;
      }
    }
  } while (counts[0] == 0 || counts[1] == 0);
  gRandomState = saved;
}

s32 Func_02002ce8(s32 side) {
  struct TrialRoadState *state;

  state = gTrialRoadState;
  return state->field16[side & 1];
}

s32 Func_02002cfc(void) { return gTrialRoadState->mirrored; }

s32 Func_02002d08(s32 x) {
  if (Func_02002cfc())
    return 512 - x;
  return x;
}

s32 Func_02002d20(s32 x) {
  if (Func_02002cfc())
    return (64 - x) << 3;
  return x << 3;
}

s32 Func_02002d38(s32 x) {
  if (Func_02002cfc())
    return -x << 3;
  return x << 3;
}

s32 Func_02002d50(s32 angle) {
  if (Func_02002cfc())
    return (0x8000 - angle) & 0xffff;
  return angle & 0xffff;
}

void Func_02002d78(s32 id, s32 x, s32 z, s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(
      id, Func_02002d08(x) << 16, z << 16, (u16)Func_02002d50(angle));
}

void Func_02002db0(void) {}

void Func_02002db4(void) {
  Func_02002d78(8, 288, 264, 0x8000);
  Func_02002d78(13, 296, 248, 0x8000);
  Func_02002d78(14, 296, 280, 0x8000);
}

void Func_02005880(void);
void Func_020058f0(s32 actor, s32 value);
void Func_02005980(void);

void Func_02002dec(void) {
  struct Actor *object;

  Func_02005880();
  Event_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(0x1000000, -1, 0x1080000, 1);
  Func_020058f0(5, 4);
  Func_020058f0(6, 4);
  Func_020058f0(7, 4);
  Actor_SetSpeed(4, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(6, 0x10000, 0x8000);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_ResetAndSetPositionInMode2(4, Func_02002d20(28), 272);
  Actor_WalkTo(7, Func_02002d20(28), 256);
  Actor_ResetAndSetPositionInMode2(5, Func_02002d20(27), 248);
  Actor_WalkTo(6, Func_02002d20(27), 280);
  Actor_WaitForMoveById(4);
  Actor_ArmCallback(4, Func_02002d50(0), 0);
  Actor_WaitForMoveById(7);
  Actor_ArmCallback(7, Func_02002d50(0), 0);
  Actor_WaitForMoveById(5);
  Actor_ArmCallback(5, Func_02002d50(0), 0);
  Actor_WaitForMoveById(6);
  Actor_ArmCallback(6, Func_02002d50(0), 0);
  Func_02005980();
  wait(20);
  Sound_Play(138);
  wait(20);
  Func_02002d78(8, 344, 296, 0xc000);
  object = Actor_GetById(8);
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->motionFlags = 0;
  Actor_SetMoveTarget(object, object->x, object->y + 0x580000, object->z);
  Func_020057c8(object, 2);
  Script_WaitForEventTimeout(object);
  Func_020057c8(object, 1);
  object->motionFlags = 2;
  object->terrainHeight = 0x800000;
  object->velocityY = 0x40000;
  object->speedLimit = 0x30000;
  object->acceleration = 0x20000;
  Actor_SetMoveTarget(object, object->x, object->y, object->z - 0x100000);
  Script_WaitForEventTimeout(object);
  wait(12);
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  FieldActor_SetMoveTarget(object, Func_02002d20(41) << 16, object->y,
                           264 << 16);
  Script_WaitForEventTimeout(object);
  wait(6);
  Sound_Play(152);
  object->velocityY = 0x60000;
  object->speedLimit = 0x30000;
  object->acceleration = 0x20000;
  Actor_SetMoveTarget(object, Func_02002d20(37) << 16, object->y, 264 << 16);
  Script_WaitForEventTimeout(object);
  wait(6);
  Func_020058f0(13, 8);
  Func_020058f0(14, 8);
  Actor_SetSpeedParameters(8, 0x10000, 0x8000);
  Actor_SetSpeedParameters(13, 0x10000, 0x8000);
  Actor_SetSpeedParameters(14, 0x10000, 0x8000);
  Actor_ResetAndSetPositionInMode2(8, Func_02002d20(36), 264);
  Actor_ResetAndSetPositionInMode2(13, Func_02002d20(37), 248);
  Actor_ResetAndSetPositionInMode2(14, Func_02002d20(37), 280);
  Actor_WaitForMoveById(8);
  Actor_ArmCallback(8, Func_02002d50(0x8000), 0);
  Actor_WaitForMoveById(13);
  Actor_ArmCallback(13, Func_02002d50(0x8000), 0);
  Actor_WaitForMoveById(14);
  Actor_ArmCallback(14, Func_02002d50(0x8000), 0);
  Event_SetMessageId((s32)MsgTrialRoadSwiftChallenge);
  Func_080d47b4(8, 256, 10);
  Event_ShowMessageAndWait(8, 0);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetModeById(4, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  Event_Wait(15);
  Actor_SetSpeedParameters(4, 0x20000, 0x10000);
  Actor_SetSpeed(5, 0x1e666, 0xf333);
  Actor_SetSpeed(6, 0x1b333, 0xd999);
  Actor_SetSpeed(7, 0x1cccc, 0xe666);
  Actor_SetSpeedParameters(8, 0x20000, 0x10000);
  Actor_SetSpeed(13, 0x1e666, 0xf333);
  Actor_SetSpeedParameters(14, 0x1b333, 0xd999);
  Actor_OffsetPositionAndReset(4, Func_02002d38(3), 0);
  Actor_OffsetPositionAndReset(8, Func_02002d38(-3), 0);
  wait(1);
  Actor_OffsetPositionAndReset(5, Func_02002d38(3), 0);
  Actor_OffsetPositionAndReset(13, Func_02002d38(-3), 0);
  wait(1);
  Actor_OffsetPositionAndReset(7, Func_02002d38(3), 0);
  Actor_OffsetPositionAndReset(14, Func_02002d38(-3), 0);
  wait(1);
  Actor_OffsetPositionAndReset(6, Func_02002d38(3), 0);
  wait(1);
  Event_Wait(6);
}

void Func_0200322c(void) {
  Func_02005880();
  Event_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  Camera_MoveWithinMapBounds(PIXELS(256), -1, PIXELS(264), 1);
  Func_020058f0(5, 4);
  Func_020058f0(6, 4);
  Func_020058f0(7, 4);
  Actor_SetSpeed(4, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(6, 0x10000, 0x8000);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_ResetAndSetPositionInMode2(4, Func_02002d20(28), 272);
  Actor_WalkTo(7, Func_02002d20(28), 256);
  Actor_ResetAndSetPositionInMode2(5, Func_02002d20(27), 248);
  Actor_ResetAndSetPositionInMode2(6, Func_02002d20(27), 280);
  Actor_WaitForMoveById(4);
  Actor_ArmCallback(4, Func_02002d50(0), 0);
  Actor_WaitForMoveById(7);
  Actor_ArmCallback(7, Func_02002d50(0), 0);
  Actor_WaitForMoveById(5);
  Actor_ArmCallback(5, Func_02002d50(0), 0);
  Actor_WaitForMoveById(6);
  Actor_ArmCallback(6, Func_02002d50(0), 0);
  Func_02005980();
  Event_SetMessageId((s32)MsgTrialRoadSlowArrival);
  Func_080d47b4(8, 256, 10);
  Event_ShowMessageAndWait(8, 0);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetModeById(4, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  Event_Wait(15);
  Actor_SetSpeed(4, 0x20000, 0x10000);
  Actor_SetSpeed(5, 0x1e666, 0xf333);
  Actor_SetSpeed(6, 0x1b333, 0xd999);
  Actor_SetSpeed(7, 0x1cccc, 0xe666);
  Actor_SetSpeed(8, 0x20000, 0x10000);
  Actor_SetSpeed(13, 0x1e666, 0xf333);
  Actor_SetSpeedParameters(14, 0x1b333, 0xd999);
  Actor_OffsetPositionAndReset(4, Func_02002d38(3), 0);
  Actor_OffsetPositionAndReset(8, Func_02002d38(-3), 0);
  wait(1);
  Actor_OffsetPositionAndReset(5, Func_02002d38(3), 0);
  Actor_OffsetPositionAndReset(13, Func_02002d38(-3), 0);
  wait(1);
  Actor_OffsetPositionAndReset(7, Func_02002d38(3), 0);
  Actor_OffsetPositionAndReset(14, Func_02002d38(-3), 0);
  wait(1);
  Actor_OffsetPositionAndReset(6, Func_02002d38(3), 0);
  wait(1);
  Event_Wait(6);
}

/// The byte at 0x5a of an actor, as the field its bit 0 is set through.
struct ActorSolid {
  u8 unk00[0x5a];
  u8 solid : 8;
};

void Func_02005860(s32 cue);

void Func_020034b4(void) {
  Func_02005880();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02002d78(4, 248, 272, 0);
  Func_02002d78(7, 224, 248, 0);
  Func_02002d78(5, 232, 264, 0);
  Func_02002d78(6, 224, 280, 0);
  Func_02002d78(9, 248, 256, 0);
  Func_02002d78(8, 272, 264, 0x4000);
  Func_02002d78(13, 288, 248, 0x4000);
  Func_02002d78(14, 288, 280, 0x4000);
  Actor_SetModeById(8, 6);
  Actor_SetModeById(13, 6);
  Actor_SetModeById(14, 6);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(30);
  Event_SetMessageId((s32)MsgTrialRoadHeroesProven);
  Actor_StartRepeatedMotion(13, 2);
  wait(4);
  Actor_StartRepeatedMotion(14, 2);
  wait(10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetModeById(8, 7);
  Event_Wait(15);
  Actor_SetModeById(13, 7);
  Event_Wait(10);
  Actor_SetModeById(14, 7);
  Event_Wait(5);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  Actor_ArmCallback(8, Func_02002d50(0x8000), 0);
  Actor_SetModeById(13, 1);
  Event_Wait(10);
  Actor_ArmCallback(13, Func_02002d50(0x8000), 0);
  Actor_SetModeById(14, 1);
  Event_Wait(5);
  Actor_ArmCallback(14, Func_02002d50(0x8000), 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetSpeedParameters(8, 0x10000, 0x8000);
  Actor_ResetAndSetPositionInMode2(8, Func_02002d20(32), 264);
  Actor_TurnToFaceEachOther(9, 7, 0);
  Actor_TurnToFaceEachOther(5, 6, 0);
  Actor_WaitForMoveById(8);
  Actor_SetAngleToward(9, 4, 0);
  Actor_SetAngleToward(7, 4, 0);
  Actor_SetAngleToward(6, 4, 0);
  Actor_SetAngleToward(5, 4, 0);
  Event_Wait(20);
  Func_02005860(65);
  PartyInventory_GiveItem(215, 0);
  Actor_SetAngleToward(4, 8, 0);
  OBJECT_BYTE(Actor_GetById(8), 0x5a) &= ~1;
  Actor_SetPositionAndReset(8, Func_02002d20(35), 264);
  Event_Wait(1);
  /* FAKEMATCH(signature): the byte typed as a whole-byte bit-field, as in
   * overlay 685; as a plain byte, which stores in the order of a store that
   * aliases what follows, 4 of 304 instructions differ in tla-en. */
  ((struct ActorSolid *)Actor_GetById(8))->solid |= 1;
  Event_Wait(20);
  Func_080d47b4(8, 261, 10);
  Event_ShowMessageAndWait(8, 0);
  Actor_ArmCallbackAndRefresh(8, Func_02002d50(0));
  Actor_SetAngleToward(13, 8, 0);
  Actor_SetAngleToward(14, 8, 0);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetModeById(13, 3);
  wait(3);
  Motion_SetModeAndWaitAnimation(14, 3);
  Actor_SetAngleToward(4, 9, 0);
  Actor_SetAngleToward(7, 9, 0);
  Actor_SetAngleToward(5, 9, 0);
  Actor_SetAngleToward(6, 9, 0);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetModeById(4, 3);
  wait(2);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  wait(1);
  Actor_SetModeById(5, 3);
  wait(30);
}

void Func_020037c8(void) {
  Func_02005880();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02002d78(8, 264, 264, 0x8000);
  Func_02002d78(13, 280, 248, 0x8000);
  Func_02002d78(14, 280, 280, 0x8000);
  Func_02002d78(4, 240, 276, 0);
  Func_02002d78(7, 216, 254, 0);
  Func_02002d78(5, 232, 244, 0);
  Func_02002d78(6, 224, 296, 0);
  Actor_SetModeById(4, 38);
  Actor_SetModeById(5, 19);
  Actor_SetModeById(6, 19);
  Actor_SetModeById(7, 19);
  Func_080d3928(7, 0x4000);
  Actor_SetSingleChildFlags(Actor_GetById(4), 0);
  Actor_SetSingleChildFlags(Actor_GetById(5), 0);
  Actor_SetSingleChildFlags(Actor_GetById(6), 0);
  Actor_SetSingleChildFlags(Actor_GetById(7), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(30);
  Event_SetMessageId((s32)MsgTrialRoadLeaveVillage);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Actor_StartRepeatedMotion(4, 2);
  wait(1);
  Actor_StartRepeatedMotion(6, 2);
  wait(2);
  Actor_StartRepeatedMotion(5, 2);
  Actor_StartRepeatedMotion(7, 2);
  Event_Wait(15);
}

/// The byte at 0x5a of an actor, as the field its bit 0 is set through.

extern const u8 gData_02005c88[];
extern const u8 gData_02005be8[];
extern const u8 gData_02005d28[];
extern const u8 gData_02005dc8[];
extern const u8 gData_02005e40[];
extern const u8 gData_02005ed4[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_OffsetPositionAndResetMotion_(s32 actor, s32 dx,
                                                           s32 dz) {
  Actor_OffsetPositionAndResetMotion(actor, dx, dz);
}

void Func_02003900(void) {
  s32 i;

  Func_02005880();
  Event_ResolvePendingActions(0);
  Actor_SetPosition(gMainRoutineWork.currentOwner, 0, PIXELS(1016));
  Actor_SetPosition(8, PIXELS(1016), PIXELS(1016));
  for (i = 9; i < 80; i++) {
    struct Actor *object = Actor_GetById(i);

    if (object->x != 0)
      object->x -= 0x900000;
  }
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetMovementTypeById(13, gData_02005c88);
  Actor_SetMovementTypeById(14, gData_02005be8);
  Event_StartScreenTransition();
  Event_Wait(20);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(184 << 16, -1, 600 << 16, 1);
  Func_02005980();
  FieldCamera_MoveTo(120 << 16, -1, 552 << 16, 1);
  Func_02005980();
  FieldCamera_MoveTo(120 << 16, -1, 408 << 16, 1);
  Func_02005980();
  FieldCamera_MoveTo(120 << 16, -1, 120 << 16, 1);
  Event_SetMessageId((s32)MsgTrialRoadTrapInstructions);
  Event_ShowMessageAndWait(8, 0);
  Func_02005980();
  FieldCamera_MoveTo(216 << 16, -1, 120 << 16, 1);
  Func_02005980();
  Event_ShowMessageAndWait(8, 0);
  Event_ShowMessageAndWait(4, 0);
  Actor_SetMovementTypeById(13, gData_02005d28);
  Actor_RefreshSelectorById(13);
  Actor_SetModeById(13, 1);
  Event_Wait(10);
  Actor_SetSpeed(13, 0x4000, 0x2000);
  OBJECT_BYTE(Actor_GetById(13), 0x5a) &= ~1;
  Actor_CommitPositionAndActivate(13, 0, 8);
  Event_Wait(1);
  OBJECT_BYTE(Actor_GetById(13), 0x5a) |= 1;
  Event_Wait(20);
  Actor_SetSpeed(13, 0x20000, 0x10000);
  Actor_SetModeById(13, 5);
  Actor_OffsetPositionAndResetMotion_(13, 0, -40);
  Actor_WaitForMoveById(13);
  Sound_Play(103);
  MapGrid_SetShake(0x20000, 0x40000, 0x10000);
  Actor_GetById(13)->velocityY = 0x40000;
  MapGrid_SetShake(-1, -1, 0xe666);
  OBJECT_BYTE(Actor_GetById(13), 0x5a) &= ~1;
  Actor_CommitPositionAndActivate(13, 0, 16);
  Event_Wait(1);
  ((struct ActorSolid *)Actor_GetById(13))->solid |= 1;
  Event_Wait(60);
  Actor_ArmCallback(13, FACING_SOUTH, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(13, 4);
  Event_ShowMessageAndWait(8, 0);
  Actor_FaceDirection(13, FACING_NORTHWEST, 0);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(10);
  Actor_SetSpeed(13, 0x10000, 0x8000);
  Actor_WalkByAndWait(13, -16, -16);
  Actor_FaceDirection(13, FACING_WEST, 0);
  Sound_Play(124);
  Actor_SetModeById(9, 6);
  Event_Wait(30);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetPositionAndReset(13, 200, 168);
  Actor_FaceDirection(13, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(125);
  Actor_SetModeById(9, 5);
  Event_Wait(20);
  Sound_Play(138);
  Func_0802b450(127, 3, 1, 2, 48, 3);
  Map_CopyCellAttributeRect(0, 127, 1, 1, 14, 8);
  Event_Wait(20);
  Actor_SetMovementTypeById(13, gData_02005dc8);
  Actor_RefreshSelectorById(13);
  Sound_Play(123);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetMovementTypeById(14, gData_02005e40);
  FieldCamera_MoveTo(328 << 16, -1, 120 << 16, 1);
  Func_02005980();
  Event_ShowMessageAndWait(8, 0);
  Event_ShowMessageAndWait(8, 0);
  Actor_FaceDirection(14, FACING_NORTHEAST, 0);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_Wait(10);
  Actor_SetSpeed(14, 0x10000, 0x8000);
  Actor_WalkByAndWait(14, 16, -16);
  Actor_ArmCallback(14, 0, 0);
  Sound_Play(124);
  Actor_SetModeById(11, 6);
  Actor_SetModeById(12, 6);
  Event_Wait(30);
  Actor_WalkToAndWait(14, 350, 168);
  Actor_FaceDirection(14, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(125);
  Actor_SetModeById(11, 5);
  Event_Wait(40);
  Actor_WalkToAndWait(14, 364, 168);
  Actor_ArmCallback(14, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(125);
  Actor_SetModeById(12, 5);
  Event_Wait(20);
  Sound_Play(138);
  Func_0802b450(127, 3, 1, 2, 53, 3);
  Map_CopyCellAttributeRect(0, 127, 1, 1, 19, 8);
  Event_Wait(30);
  Actor_ArmCallback(14, FACING_NORTHWEST, 0);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_ShowMessageAndWait(8, 0);
  Camera_MoveWithinMapBounds(408 << 16, -1, 120 << 16, 1);
  Func_02005980();
  Event_ShowMessageAndWait(8, 0);
  Actor_SetMovementTypeById(14, gData_02005ed4);
  Actor_RefreshSelectorById(14);
  Event_ShowMessageAndWait(8, 0);
  Import_EventSetExitRequest(10);
}

// Updates the sixteen movers at gData_020023c4 + 0x188 each frame: at its
// upAt and downAt phases a mover calls Sound_Play (206 or 395) and moves by
// 4 or -8 a frame until it reaches its high or low offset; its phase counts
// round its period unless flag 260 is set. Each mover's shape, collision
// height and cell attribute byte follow its actor and offset, and its marker
// actor is placed over it, bobbing on a sine while the offset is not 0.

struct TrialRoadMover {
  s16 id;
  s16 phase;
  s16 offset;
  s16 period;
  s16 upAt;
  s16 downAt;
  s16 high;
  s16 low;
  s16 unk10;
  s16 speed;
  s16 unk14;
  s16 bob;
  struct Actor *marker;
};

struct TrialRoadMovers {
  u8 unk00[2];
  s16 shape;
  struct TrialRoadMover movers[16];
};

/// Updates the sixteen movers at gData_020023c4 + 0x188 each frame: at its upAt
/// and downAt phases a mover calls Sound_Play (206 or 395) and moves by 4 or -8
/// a frame until it reaches its high or low offset; its phase counts round its
/// period unless flag 260 is set. Each mover's shape, collision height and cell
/// attribute byte follow its actor and offset, and its marker actor is placed
/// over it, bobbing on a sine while the offset is not 0.
void Func_02003da0(void) {
  struct TrialRoadMovers *work =
      (struct TrialRoadMovers *)(gData_020023c4 + 0x188);
  struct TrialRoadMover *mover = work->movers;
  struct TrialRoadMapWork *map = Ram_HeapSlots->mapWork;
  s32 shape = work->shape;
  u8 *shapes = *(u8 **)((u8 *)map + 316);
  u8 *cells = map->layers[0].cells;
  s32 *collision;
  struct Actor *actor;
  struct Actor *marker;
  s32 cell;
  u8 *attributes;
  s32 i;

  i = 0;
  collision = (s32 *)Ram_MapCollision + shape;
  for (; i <= 15; collision++, i++, mover++) {
    if (mover->id == 0)
      continue;
    actor = Actor_GetById(mover->id);
    if (actor->x == 0 && actor->z == 0)
      continue;
    if (mover->phase == mover->upAt) {
      Sound_Play(206);
      mover->speed = 4;
    }
    if (mover->phase == mover->downAt) {
      Sound_Play(395);
      mover->speed = -8;
    }
    if (mover->speed != 0) {
      mover->offset += mover->speed;
      if (mover->offset >= mover->high) {
        mover->offset = mover->high;
        mover->speed = 0;
        mover->bob = 0;
      }
      if (mover->offset <= mover->low) {
        mover->offset = mover->low;
        mover->speed = 0;
        mover->bob = 0;
      }
    }
    if (!GameFlag_Test(260)) {
      if (++mover->phase >= mover->period)
        mover->phase = 0;
    }
    cell = (actor->x >> 20) + ((actor->z >> 20) << 7);
    shapes[cell] = (u8)work->shape + i;
    *collision = ((((s16 *)&actor->y)[1] + mover->offset) / 8) << 8;
    attributes = cells + cell * 4;
    if (mover->speed != 0)
      attributes[3] = 0;
    else {
      /* FAKEMATCH(temporary): the form kept; as attributes[3] |= 128, 4 of 213
         instructions differ in tla-en. */
      s32 bits = 128;
      bits |= attributes[3];
      attributes[3] = bits;
    }
    if (mover->offset != 0) {
      /* FAKEMATCH(temporary): the form kept; as attributes[3] |= 16, 4 of 213
         instructions differ in tla-en. */
      s32 bits = 16;
      bits |= attributes[3];
      attributes[3] = bits;
    } else
      attributes[3] = 0;
    marker = mover->marker;
    if (marker != 0) {
      if (mover->offset == 0) {
        marker->y = actor->y - 0x100000;
      } else {
        s32 wave;
        mover->bob += 0x1000;
        wave = Iwram_MulQ16(0x20000, Math_Sine(mover->bob)) - 0xe0000;
        marker->y = actor->y + (mover->offset << 16) + wave;
      }
      marker->x = actor->x;
      marker->z = actor->z;
    }
  }
}
