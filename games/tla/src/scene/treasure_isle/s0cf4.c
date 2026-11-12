/// @file
/// Scene code of overlay 0x6af, in ROM order, from s0cf4's offset to the end of
/// its code, then the overlay's data.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "task.h"
#include "scene_tables.h"
#include "event_inline.h"

struct TreasureIsleEventWork {
  u8 unk000[0x1ac];
  s32 route;
};

extern char SceneId_Ov6afRoom1;
extern char SceneId_Ov6afRoom2;
extern char SceneId_Ov6afRoom3;
extern char SceneId_Ov6afRoom4;
extern u32 gData_0200117c[];
extern u32 gData_020011cc[];

void Func_020003fc(void *cells, s32 index);
void Func_02000ba0(void);
void Func_0200105c(void);
s32 Func_02001064(s32 a, s32 b, s32 c);

static __inline__ void PlaceObject(s32 id, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h); as direct calls, 12
   * of 241 instructions differ in tla-en. */
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

/// Treasure Isle's setup for each of its four rooms: places the objects whose
/// flags are set, schedules Func_02000ba0 in rooms 2 and 3 and opens again
/// the cells already opened there (Func_020003fc), and in room 4 raises the
/// part priority of objects 9 to 16.
s32 Func_02000cf4(void) {
  s32 i;

  ((struct TreasureIsleEventWork *)Ram_HeapSlots->eventWork)->route = 0x204;
  Func_0200105c();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom1) {
    Event_SetFlag(0x300);
    Event_SetFlag(0x301);
    Event_SetFlag(0x302);
    if (Event_GetFlag(0x300))
      PlaceObject(8, PIXELS(264), PIXELS(440));
    if (Event_GetFlag(0x301))
      PlaceObject(9, PIXELS(328), PIXELS(440));
    if (Event_GetFlag(0x302))
      PlaceObject(10, PIXELS(440), PIXELS(344));
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom2) {
    Task_AddFrame(Func_02000ba0, TASK_PASS_VBLANK);
    Func_02001064(0, 8, 9);
    if (Event_GetFlag(0x308))
      PlaceObject(10, PIXELS(264), PIXELS(184));
    if (Event_GetFlag(0x303))
      Func_020003fc(gData_0200117c, 0);
    if (Event_GetFlag(0x304))
      Func_020003fc(gData_0200117c, 1);
    if (Event_GetFlag(0x305))
      Func_020003fc(gData_0200117c, 2);
    if (Event_GetFlag(0x306))
      Func_020003fc(gData_0200117c, 3);
    if (Event_GetFlag(0x307))
      Func_020003fc(gData_0200117c, 4);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom3) {
    Task_AddFrame(Func_02000ba0, TASK_PASS_VBLANK);
    Func_02001064(0, 8, 9);
    if (Event_GetFlag(0x309))
      PlaceObject(10, PIXELS(664), PIXELS(248));
    if (Event_GetFlag(0x30a))
      Func_020003fc(gData_020011cc, 0);
    if (Event_GetFlag(0x30b))
      Func_020003fc(gData_020011cc, 1);
    if (Event_GetFlag(0x30c))
      Func_020003fc(gData_020011cc, 2);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom4) {
    if (Event_GetFlag(0x30d))
      PlaceObject(8, PIXELS(696), PIXELS(600));
    /* FAKEMATCH(signature): Func_02001064 declared returning s32, though it
     * reaches Func_080d9e98, whose draft returns nothing; as void, which sets
     * r0 first for these calls, 4 of 241 instructions differ in tla-en. */
    Func_02001064(0, 17, 18);
    Func_02001064(1, 19, 20);
    Func_02001064(2, 21, 22);
    i = 0;
  next:
    {
      u8 *flags = (u8 *)Actor_Get(i + 9)->animation + 37;

      *flags |= 12;
    }
    if (++i <= 7)
      goto next;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_0200102c();

void Func_02000f4c(void) { Func_0200102c(); }

#include "text/msg_ids.h"
#include "edition.h"

// The Rusty Sword's name; the Japanese edition holds message 0 in its place.
#if LANGUAGE == LANGUAGE_JAPANESE
#define RUSTY_SWORD_MESSAGE 0
#else
#define RUSTY_SWORD_MESSAGE MSGNO(MsgRustySword2)
#endif

void Func_020005ac(void);
void Func_02000684(void);
void Func_02000728(void);
void Func_02000760(void);
void Func_02000864(void);
void Func_02000870(void);
void Func_02000880(void);
void Func_02000914(void);
void Func_02000940(void);
void Func_02000970(void);
void Func_020009a0(void);
void Func_020009d0(void);
void Func_020009fc(void);
void Func_02000a2c(void);
void Func_02000a5c(void);
void Func_02000a8c(void);

extern u32 gData_02001074[];
extern u32 gData_020010b0[];
extern u32 gData_020010ec[];
extern u32 gData_0200131c[];
extern u32 gData_020014e0[];

// clang-format off
u32 gData_02001074[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020010b0[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020010ec[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02001128[] = {
    39, 2, 0x80010000, 0, 1, 13,
    44, 0xc0010000, 0, 1, 2, 0,
    0, 0, 0, 1, 38, 17,
};

s32 gData_02001170[] = {
    (s32)gData_02001074,
    (s32)gData_020010b0,
    (s32)gData_020010ec,
};

u32 gData_0200117c[] = {
    7, 21, 12, 771, 7, 17,
    13, 772, 12, 15, 14, 773,
    9, 11, 15, 774, 46, 38,
    16, 775,
};

u32 gData_020011cc[] = {
    41, 23, 11, 778, 47, 23,
    12, 779, 53, 23, 13, 780,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(298),
    SCENE_EXIT_IF(1, 2, 87, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 298, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 298, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 298, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 298, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(6, 298, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(7, 298, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(8, 299, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(299),
    SCENE_EXIT_IF(1, 298, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 299, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 299, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 300, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 300, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(300),
    SCENE_EXIT_IF(1, 299, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 299, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 300, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 300, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 301, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(301),
    SCENE_EXIT_IF(1, 300, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 301, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 301, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 301, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 301, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(6, 302, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(302),
    SCENE_EXIT_IF(1, 301, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 302, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 302, 2, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

u32 gData_0200131c[] = {
    39, 1, 0, 1, 17,
};

struct ActorPlacement gData_02001330[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(440), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(440), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(344), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02001390[] = {
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(600), 0, PIXELS(616),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(680), 0, PIXELS(616),
     FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(664), 0, PIXELS(664), FACING_SOUTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02001480[] = {
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(824), 0, PIXELS(232),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(824), 0, PIXELS(360),
     FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(696), 0, PIXELS(248), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

u32 gData_020014e0[] = {
    22, 9, 0xcccc, 22, 10, 0xcccc,
    0, 1, 17,
};

struct ActorPlacement gData_02001504[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(728), 0, PIXELS(600), FACING_SOUTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gData_020014e0, PIXELS(328), 0, PIXELS(328),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gData_020014e0, PIXELS(216), 0, PIXELS(312),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gData_020014e0, PIXELS(104), 0, PIXELS(328),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gData_020014e0, PIXELS(312), 0, PIXELS(216),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gData_020014e0, PIXELS(216), 0, PIXELS(200),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gData_020014e0, PIXELS(120), 0, PIXELS(216),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gData_020014e0, PIXELS(744), 0, PIXELS(728),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gData_020014e0, PIXELS(424), 0, PIXELS(680),
     FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(360), 0, PIXELS(216),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(360), 0, PIXELS(296),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(152), 0, PIXELS(216),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(152), 0, PIXELS(296),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(72), 0, PIXELS(216),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(72), 0, PIXELS(296),
     FACING_SOUTH, 2, 0},
    {246, 0x7d, 1, PIXELS(88), 0, PIXELS(328), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_0200169c[] = {
    {169, 0x9bb, 1, PIXELS(792), 0, PIXELS(136), FACING_SOUTH, 0, 0},
    {243, 0x43, 1, PIXELS(200), 0, PIXELS(136), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gData_020016e4[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {1, 7, CONDITION_ALWAYS, 7},
    {1, 8, CONDITION_ALWAYS, 8},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000760)},
    {0x8c15, 8, 0x300, EVENT(Func_02000760)},
    {0x8c15, 9, 0x301, EVENT(Func_02000760)},
    {0x8c15, 10, 0x302, EVENT(Func_02000760)},
    {TRIGGERS_END},
};

struct Trigger gData_02001780[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {2, 30, CONDITION_ALWAYS, EVENT(Func_020005ac)},
    {2, 31, CONDITION_ALWAYS, EVENT(Func_02000728)},
    {0x602, 35, CONDITION_ALWAYS, EVENT(Func_02000f4c)},
    {0x50009805, 40, 0x1303, EVENT(Func_02000880)},
    {0x50009805, 41, 0x1304, EVENT(Func_02000880)},
    {0x50009805, 42, 0x1305, EVENT(Func_02000880)},
    {0x50009805, 43, 0x1306, EVENT(Func_02000880)},
    {0x50009805, 44, 0x1307, EVENT(Func_02000880)},
    {0x8c15, 10, 0x308, EVENT(Func_02000760)},
    {0x8c15, 11, CONDITION_ALWAYS, EVENT(Func_02000760)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000760)},
    {TRIGGERS_END},
};

struct Trigger gData_0200184c[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {49, 5, CONDITION_ALWAYS, 5},
    {0xc602, 35, CONDITION_ALWAYS, EVENT(Func_02000f4c)},
    {0x602, 35, CONDITION_ALWAYS, EVENT(Func_02000f4c)},
    {2, 30, CONDITION_ALWAYS, EVENT(Func_02000684)},
    {0x50009805, 50, 0x130a, EVENT(Func_02000880)},
    {0x50009805, 51, 0x130b, EVENT(Func_02000880)},
    {0x50009805, 52, 0x130c, EVENT(Func_02000880)},
    {0x8c15, 10, 0x309, EVENT(Func_02000760)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000760)},
    {TRIGGERS_END},
};

s32 gData_020018f4[] = {
    33,
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
    0x8602,
    0x200001f,
    (s32)Func_02000f4c,
    0x602,
    0x2000020,
    (s32)Func_02000f4c,
    0x8602,
    0x2010021,
    (s32)Func_02000f4c,
    0x602,
    0x2010022,
    (s32)Func_02000f4c,
    0x8602,
    0x2020023,
    (s32)Func_02000f4c,
    0x602,
    0x2020024,
    (s32)Func_02000f4c,
    0x8602,
    0x2030025,
    (s32)Func_02000f4c,
    0x602,
    0x2030026,
    (s32)Func_02000f4c,
    0x8602,
    0x2040027,
    (s32)Func_02000f4c,
    0x602,
    0x2040028,
    (s32)Func_02000f4c,
    0x8602,
    0x2050029,
    (s32)Func_02000f4c,
    0x602,
    0x205002a,
    (s32)Func_02000f4c,
    RUSTY_SWORD_MESSAGE,
    (s32)0xffff0017,
    (s32)Func_02000864,
    0x8c15,
    0x30d0008,
    (s32)Func_02000760,
    9,
    -0x10000,
    (s32)Func_02000760,
    0x8f15,
    0x2000009,
    (s32)Func_02000914,
    0x8f15,
    0x201000a,
    (s32)Func_02000940,
    0x8f15,
    0x202000b,
    (s32)Func_02000970,
    0x8f15,
    0x203000c,
    (s32)Func_020009a0,
    0x8f15,
    0x204000d,
    (s32)Func_020009d0,
    0x8f15,
    0x205000e,
    (s32)Func_020009fc,
    0x8f15,
    0x206000f,
    (s32)Func_02000a2c,
    0x8f15,
    0x2070010,
    (s32)Func_02000a5c,
    -1,
    0,
    0,
};

struct Trigger gData_02001a5c[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {2, 42, 0x9bb, EVENT(Func_02000a8c)},
    {0, 9, CONDITION_ALWAYS, EVENT(Func_02000870)},
    {TRIGGERS_END},
};

s32 gData_02001aa4[] = {
    -1,
};
