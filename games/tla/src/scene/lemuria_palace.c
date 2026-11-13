/// @file
/// The scene code of overlay 0x68d, in ROM order.

// ---- games/tla/src/scene/lemuria_palace.c ----
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
#include "main_routine_work.h"
#include "event_inline.h"
#include "frame.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "actor.h"
#include "iwram_call.h"
#include "text/msg_ids.h"
#include "system/vector3.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/field/event_commands.h"
#include "module/scene/lemuria_palace.h"
#include "module/scene/scene.h"
#include "module/game/game.h"

void Event_Start();
/* VIEW(lemuria_palace): routine Event_End as void Event_End(); event_work.h has void Event_End(void); not tried: Event_End's bound declaration was not found. */
extern void Event_End__lemuria_palace() __asm__("Event_End");

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000054(struct DoorStep *step, s32 countdown, s32 unused) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(158);
  MapGrid_AnimateCells(step->script, step->x, step->z);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(countdown);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End__lemuria_palace();
}

void Func_020000c8(s32 kind) {
  const u8 *data = gData_02002368;

  if (kind == 3)
    data += 8;
  if (kind == 16)
    data = gData_02002378;
  Func_02000054(data, kind, 0);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/* VIEW(lemuria_palace): variable gData_02002608 as extern s16 gData_02002608[]; the definition has u32 gData_02002608[] = { 0x76c07b00, 0x6e407280, 0x65c06a00, 0x65c06180, 0x6e406a00, 0x76c07280, }; tried as the definition's extern u32 gData_02002608[], Func_020000f4 moves: 2 of 35 instructions in tla-ja. */
extern s16 gData_02002608__lemuria_palace[] __asm__("gData_02002608");

static inline u32 Frame_Mod6(u32 frame) {
  /* FAKEMATCH(wrapper): the inline keeps gFrameCount's address out of a
   * callee-saved register; as gFrameCount % 6, 16 of 35 instructions differ in
   * tla-en. */
  return frame % 6;
}

/// Every sixth frame rotates colours 1-14 of background palette 12 by one
/// place; every frame sets its colour 15 from a twelve-step cycle.
void Func_020000f4(void) {
  /* FAKEMATCH(pin): the colour stays in r1; as a plain local, 4 of 35
   * instructions differ in tla-en. */
  register s32 color asm("r1");

  if (Frame_Mod6(gFrameCount) == 0) {
    volatile u16 *palette;
    /* FAKEMATCH(pin): the saved colour takes r1 and leaves r0 to the index; as
     * a plain local, 12 of 35 instructions differ in tla-en. */
    register s32 saved asm("r1");
    s32 index;

    /* FAKEMATCH(pin): the read stays in r3; as a plain local, 6 of 35
     * instructions differ in tla-en. */
    register s32 raw asm("r3") = *(volatile u16 *)&BG_PLTT_COLOR(12, 14);

    /* FAKEMATCH(barrier): the barrier keeps the sign extension apart from
     * the read; without it, 6 of 35 instructions differ in tla-en. */
    asm("" : "+r"(raw));
    saved = (s16)raw;
    index = 14;
  loop:
    palette = (volatile u16 *)&BG_PLTT_COLOR(12, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto loop;
    *(volatile u16 *)&BG_PLTT_COLOR(12, 1) = saved;
  }
  color = gData_02002608__lemuria_palace[(gFrameCount >> 2) % 12];
  *(volatile u16 *)&BG_PLTT_COLOR(12, 15) = color;
}

/// The room's setup: it schedules the palette cycle Func_020000f4.
s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x201;
  Task_AddFrame(Func_020000f4, 0xc80);
  FieldActor_SetSpritePriority(8, 3);
  if (gMainRoutineWork.entrance == 6)
    Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  if (Event_GetFlag(0x96a)) {
    Actor_SetPosition(7, 0, 0);
    Actor_SetPosition(15, 0, 0);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_020001d8(void) {
  struct Actor *object = Actor_Get(13);
  struct Actor *first;
  struct Actor *second;

  Actor_SetAnimation(13, 5);
  FieldEvent_Wait(28);
  Event_PlaySound(112);
  first =
      Actor_Spawn(674, object->x + 0xa0000, object->y + 0x100000, object->z);
  first->speedLimit = 0x20000;
  first->acceleration = 0x20000;
  first->motionFlags = 0;
  FieldActor_SetMoveTarget(first, first->x + PIXELS(6), first->y + PIXELS(8),
                      first->z);
  second =
      Actor_Spawn(674, object->x + 0xa0000, object->y + 0x100000, object->z);
  second->speedLimit = 0x20000;
  second->acceleration = 0x20000;
  second->motionFlags = 0;
  FieldActor_SetMoveTarget(second, first->x + PIXELS(8), first->y - PIXELS(4),
                      first->z);
  Actor_WaitForMove(first);
  Actor_Release(first);
  Actor_WaitForMove(second);
  Actor_Release(second);
  FieldEvent_Wait(30);
}

extern u8 gData_0200308c[];
extern u8 gData_02002fe8[];
s32 __modsi3();

void Func_020002b4(void) {
  struct Vector3 position;
  s8 mode = gData_0200308c[0];

  if (mode == 1) {
    s32 *time = (s32 *)Data_02003098;
    s32 t = *time;
    s32 level = Data_02002fd7[(t >> 3) & 15];

    {
      s32 value = level << 8;
      /* FAKEMATCH(dead): an empty block holds the shift first; with no
       * block, 2 of 96 instructions differ in tla-en. */
      do {
      } while (0);
      value |= 16 - level;

      REG_BLDALPHA = value;
    }
    /* FAKEMATCH(dead): the step in a once-only block holds it after the
     * store; as the plain statement, 2 of 96 instructions differ in tla-en. */
    do {
      t++;
    } while (0);
    *time = t;
  } else if (mode == 2) {
    s32 *time = (s32 *)Data_02003098;
    s32 step = (*time >> 2) % 28;

    {
      u16 *table = (u16 *)gData_02002fe8;
      /* FAKEMATCH(temporary): the offset in a local puts the table first in the
       * load; as table[step], 2 of 96 instructions differ in tla-en. */
      s32 at = step * 2;
      s32 value = *(u16 *)((u8 *)table + at);

      REG_BLDALPHA = value;
    }
    if (step == 0) {
      *(s32 *)Data_02003090 = 1;
      *(s32 *)Data_02003024 = step;
      Event_PlaySound(153);
    }
    if (step == 14) {
      *(s32 *)Data_02003090 = 0;
      *(s32 *)Data_02003024 = 1;
      Event_PlaySound(153);
    }
    (*time)++;
  }
  position.x = 0x3400000;
  position.y = 0;
  position.z = 0x1c00000;
  Camera_WorldToScreen(&position);
  if (*(s32 *)Data_02003090 != 0) {
    ((struct Actor *)Data_02003060)->y = position.x;
    ((struct Actor *)Data_02003060)->z = position.z;
    Func_0200210c(Data_02003060);
  }
  if (*(s32 *)Data_02003024 != 0) {
    ((struct Actor *)Data_02003030)->y = position.x;
    ((struct Actor *)Data_02003030)->z = position.z;
    Func_0200210c(Data_02003030);
  }
}

// ---- games/tla/src/scene/lemuria_palace/s03a0.c ----
#include "task.h"
#include "call.h"
#include "system/io_write.h"

extern s32 gData_02003020;
/* VIEW(s03a0): variable Data_02003060 as extern struct Unk_020003a0Sprite Data_02003060; lemuria_palace has extern u8 Data_02003060[]; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/scene/lemuria_palace.c:264: request for member `unk1e' in something not a structure or union. */
extern struct Unk_020003a0Sprite Data_02003060__s03a0 __asm__("Data_02003060");
/* VIEW(s03a0): variable Data_02003030 as extern struct Unk_020003a0Sprite Data_02003030; lemuria_palace has extern u8 Data_02003030[]; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/scene/lemuria_palace.c:275: request for member `unk1e' in something not a structure or union. */
extern struct Unk_020003a0Sprite Data_02003030__s03a0 __asm__("Data_02003030");
/* VIEW(s03a0): variable Data_02003090 as extern s32 Data_02003090; lemuria_palace has extern u8 Data_02003090[]; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/scene/lemuria_palace.c:280: `Data_02003090' has an incomplete type. */
extern s32 Data_02003090__s03a0 __asm__("Data_02003090");
/* VIEW(s03a0): variable Data_02003024 as extern s32 Data_02003024; lemuria_palace has extern u8 Data_02003024[]; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/scene/lemuria_palace.c:281: `Data_02003024' has an incomplete type. */
extern s32 Data_02003024__s03a0 __asm__("Data_02003024");
/* VIEW(s03a0): variable Data_02003098 as extern s32 Data_02003098; lemuria_palace has extern u8 Data_02003098[]; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/scene/lemuria_palace.c:282: `Data_02003098' has an incomplete type. */
extern s32 Data_02003098__s03a0 __asm__("Data_02003098");
/* VIEW(s03a0): variable gData_0200308c as extern u8 gData_0200308c; lemuria_palace has extern u8 gData_0200308c[]; tried as the definition's extern struct Trigger gData_0200308c[], which the object already declares, it does not compile in tla-ja: games/tla/src/scene/lemuria_palace.c:283: `gData_0200308c' has an incomplete type. */
extern u8 gData_0200308c__s03a0 __asm__("gData_0200308c");

void Func_020003a0(void) {
  /* FAKEMATCH(wrapper): forwarded, so that its 0x1000 goes straight to r0; as a
   * direct call, 47 of 126 instructions differ in tla-en. */
  void *buffer = (void *)Value1((s32 (*)())Heap_Alloc, 0x1000);
  s32 *firstSlot;
  s32 *firstTile;
  s32 *secondSlot;
  s32 *secondTile;

  Io_Write16(0x3f40, &REG_BLDCNT);
  Io_Write16(0x1000, &REG_BLDALPHA);
  Resource_DecodeType01(Data_02002620, buffer);
  firstSlot = &gData_02003020;
  *firstSlot = Resource_FindFreeEntry();
  firstTile = &Data_02003088;
  *firstTile = Vram_Load(gData_02003020, 0x1000, buffer);
  Func_02002104(&Data_02003060__s03a0, 0, 0, 0xc0000000, Data_02003088);
  Data_02003060__s03a0.unk1e = 1;
  Data_02003060__s03a0.priority = 2;
  Data_02003060__s03a0.shape = 1;
  Data_02003060__s03a0.blend = 1;
  Data_02003060__s03a0.palette = 0;
  Resource_DecodeType01(Data_02002af8, buffer);
  secondSlot = &gData_02003094;
  *secondSlot = Resource_FindFreeEntry();
  secondTile = &Data_02003058;
  *secondTile = Vram_Load(gData_02003094, 0x1000, buffer);
  Func_02002104(&Data_02003030__s03a0, 0, 0, 0xc0000000, Data_02003058);
  Data_02003030__s03a0.unk1e = 2;
  Data_02003030__s03a0.priority = 2;
  Data_02003030__s03a0.shape = 1;
  Data_02003030__s03a0.blend = 1;
  Data_02003030__s03a0.palette = 0;
  Data_02003090__s03a0 = 0;
  Data_02003024__s03a0 = 0;
  Data_02003098__s03a0 = 0;
  gData_0200308c__s03a0 = 0;
  Task_AddFrame(Func_020002b4, TASK_PASS_VBLANK);
  Heap_Release(buffer);
}

void Func_020004e8(void) {
  FieldTask_Remove(Func_020002b4);
  Resource_ResetEntry(gData_02003020);
  Resource_ResetEntry(gData_02003094);
}

static __inline__ void Blend_WriteAlpha(s32 value) {
  // The s32 parameter keeps each constant a word built with shifts; a u16 store
  // loads it from the literal pool.
  REG_BLDALPHA = value;
}

// Each alpha write is the macro's own once-only block; with the plain inline
// the fades differ 4 and 2.
#define BLEND_SET_ALPHA(value)                                                 \
  do {                                                                         \
    Blend_WriteAlpha(value);                                                   \
  } while (0)

/// Fades the second target out and the first in over 16 steps of four frames.
void Func_02000510(void) {
  s32 i;

  // The loop is written with goto: a for, do-while or while (1) loop differs
  // 39, as the loop pass hoists the register address and 16 out of it.
  BLEND_SET_ALPHA(0x1000);
  gData_0200308c__s03a0 = 0;
  i = 0;
loop:
  BLEND_SET_ALPHA(((16 - i) << 8) | i);
  System_Wait(4);
  i++;
  if (i < 16)
    goto loop;
  BLEND_SET_ALPHA(16);
  gData_0200308c__s03a0 = 1;
}

/// The reverse fade, in steps of three frames.
void Func_02000550(void) {
  s32 i;

  BLEND_SET_ALPHA(16);
  gData_0200308c__s03a0 = 0;
  i = 0;
loop:
  BLEND_SET_ALPHA((i << 8) | (16 - i));
  System_Wait(3);
  i++;
  if (i < 16)
    goto loop;
  BLEND_SET_ALPHA(0x1000);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Event_Start();
/* VIEW(s03a0): routine Event_End as s32 Event_End(); lemuria_palace has void Event_End(); not tried: Event_End's bound declaration was not found. */
extern s32 Func_02001fd4__s03a0() __asm__("Event_End");
/* VIEW(lemuria_palace): routine FieldCamera_WaitForArrival as void FieldCamera_WaitForArrival(); module/scene/scene.h has void FieldCamera_WaitForArrival(s32 value); tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/scene/lemuria_palace.c:411: too few arguments to function `FieldCamera_WaitForArrival'. */
extern void Func_020020ac__lemuria_palace() __asm__("FieldCamera_WaitForArrival");
/* VIEW(s03a0): routine Func_020001d8 as s32 Func_020001d8(); the definition has void Func_020001d8(void); tried as the definition's void Func_020001d8(struct Actor *object), it does not compile in tla-ja: games/tla/src/scene/lemuria_palace.c:356: conflicting types for `Func_020001d8'. */
extern s32 Func_020001d8__s03a0() __asm__("Func_020001d8");
s32 __modsi3();
void Field_StartPaletteTransition();

s32 Func_0200058c(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r5_1, r7_1, r8_1, r5_3, r5_4, r5_5;
  s32 v0, v2, r0, v10, v14, v17, v18, r2, r3, v29;
  struct Actor *v3, *v6, *v7, *v12, *v13, *v19, *v20, *v22, *v23, *v24, *v25,
      *v26, *v27;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaPalace11);
  Actor_SetSpeed(16, 0x13333, 0x9999);
  Actor_WalkByAndWait(16, -24, 0);
  Actor_FaceDirectionAndWait(16,
        FACING_SOUTH + FACING_STEP);
  {
    s32 party = (s32)&gMainRoutineWork;

    r5_1 = party + 532;
  }
  Actor_FaceActor(16, FIELD_AT(r5_1, s32, 0), 0);
  Event_OpenMessage(16, 0);
  v0 = Event_AskYesNoWithActorAnimation(4, 0);
  r7_1 = v0;
  if (r7_1 == 0)
    goto L_0200863c;
  Event_Wait(30);
  Actor_SetAnimationAndWait(16, 4);
  Event_ShowMessageThenWait(16, 0, 5);
  Actor_WalkByAndWait(FIELD_AT(r5_1, s32, 0), 0, 16);
  Actor_WalkByAndWait(16, 24, 0);
  Actor_FaceDirectionAndWait(16,
        FACING_SOUTH + FACING_STEP);
  v2 = Func_02001fd4__s03a0();
  r0 = v2;
  goto L_02009f20;
L_0200863c:
  Actor_SetAnimationAndWait(16, 3);
  Event_Wait(10);
  Actor_WalkByAndWait(16, 24, 0);
  Actor_FaceDirection(16, FACING_SOUTH + FACING_STEP, 0);
  Actor_WatchAndSetScript(16, FIELD_AT(r5_1, s32, 0));
  Event_SetFlag(0x96a);
  Event_SetFlag(0x96b);
  Party_AddMember(7);
  v3 = Actor_GetById(64);
  v3->scaleY = r7_1;
  Actor_FaceDirection(14, FACING_EAST, 0);
  Actor_SetSpeed(13, 0xb333, 0x5999);
  Actor_SetSpeed(14, 0xb333, 0x5999);
  Actor_SetSpeed(7, 0xb333, 0x5999);
  Actor_WalkToAndWait(4, 872, 528);
  FieldCamera_MoveTo(PIXELS(872), -1, PIXELS(408),
        1);
  Func_020020ac__lemuria_palace();
  Event_Wait(10);
  Actor_FaceDirection(16, FACING_SOUTH + FACING_STEP, 0);
  Actor_ShowEmote(13, 0x100, 45);
  Actor_FaceDirectionAndWait(13,
        FACING_SOUTH + FACING_STEP);
  Event_Wait(10);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452) =
      (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(15,
        FACING_SOUTH + FACING_STEP);
  Event_Wait(30);
  Actor_FaceDirectionAndWait(14, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_SetAnimationAndWait(13, 4);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_RunRepeatedMotion(15, 2);
  Event_Wait(5);
  Actor_FaceDirectionAndWait(15, FACING_WEST);
  Actor_ShowEmote(15, 0x103, 30);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Actor_FaceDirection(13, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(14, FACING_EAST);
  Event_Wait(10);
  Actor_ShowEmote(13, 0x108, 30);
  Event_ShowMessageThenWait(13, 0, 5);
  Actor_RunRepeatedMotion(15, 2);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(13, 0, 5);
  Actor_FaceDirectionAndWait(13,
        FACING_SOUTH + FACING_STEP);
  Event_Wait(10);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Camera_FollowActor(13, 1);
  Actor_WalkTo(13, 864, 444);
  Event_Wait(20);
  Actor_WalkTo(14, 848, 444);
  Event_Wait(20);
  Actor_WalkTo(7, 832, 444);
  Actor_WaitForMoveById(13);
  Actor_SetAnimation(13, 1);
  Actor_FaceDirection(13, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WaitForMoveById(14);
  Actor_SetAnimation(14, 1);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WaitForMoveById(7);
  Actor_SetAnimation(7, 1);
  Actor_FaceDirectionAndWait(7, FACING_SOUTH);
  FieldCamera_MoveTo(PIXELS(872), -1, PIXELS(496),
        1);
  Func_020020ac__lemuria_palace();
  Event_Wait(5);
  FieldActor_LaunchFromLeader(17, 16, 0, FACING_NORTHWEST + FACING_STEP);
  FieldActor_LaunchFromLeader(5, -16, 0, FACING_NORTH);
  FieldActor_LaunchFromLeader(6, -32, 0, FACING_NORTH);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_WaitForMoveById(6);
  Event_Wait(10);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_FaceDirectionAndWait(7, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(7, 3);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTH);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirection(14, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(13,
        FACING_SOUTH + FACING_STEP);
  Actor_WalkByAndWait(6, 0, -8);
  Event_Wait(10);
  Actor_SetAnimationAndWait(6, 3);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTH);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(13,
        FACING_SOUTH + FACING_STEP);
  Actor_WalkByAndWait(5, 0, -8);
  Event_Wait(10);
  Actor_SetAnimationAndWait(5, 3);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHEAST);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(13,
        FACING_SOUTHEAST + FACING_STEP);
  Actor_WalkByAndWait(4, 0, -8);
  Event_Wait(10);
  Actor_SetAnimationAndWait(4, 3);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHEAST);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(13,
        FACING_SOUTHEAST + FACING_STEP);
  v6 = Actor_GetById(17);
  v6->actionFlags &= ~1;
  Actor_WalkByAndWait(17, 0, -8);
  Event_Wait(1);
  v7 = Actor_GetById(17);
  v7->actionFlags |= 1;
  Event_Wait(10);
  Actor_SetAnimationAndWait(17, 3);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(14, FACING_EAST);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_FaceDirectionAndWait(13, FACING_WEST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(20);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(13,
        FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  Actor_ShowEmote(13, 0x108, 30);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_StartRepeatedMotion(17, 3);
  Actor_SetAttachedEffect(17, 0x102);
  Event_Wait(37);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 0);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_FaceDirectionAndWait(13,
        FACING_NORTH + FACING_STEP);
  Event_Wait(15);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_FaceDirectionAndWait(13,
        FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_ShowEmote(13, 0x105, 30);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_RunRepeatedMotion(13, 2);
  Event_Wait(5);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 0);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_RunRepeatedMotion(7, 2);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(7, 4);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirectionAndWait(13, FACING_NORTH + FACING_STEP);
  Event_Wait(5);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_SetAnimationAndWait(13, 4);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_RunRepeatedMotion(17, 2);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(13,
        FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  Func_020001d8__s03a0(13);
  Field_StartColorTransition(32, 0x2108);
  Event_Wait(32);
  Event_Wait(20);
  Func_020003a0();
  r8_1 = 1;
  Data_02003090__s03a0 = r8_1;
  Data_02003024__s03a0 = r7_1;
  Event_PlaySound(153);
  Func_02000510();
  Actor_ShowEmote(4, 0x100, 0);
  Actor_ShowEmote(17, 0x100, 0);
  Actor_ShowEmote(6, 0x100, 0);
  Actor_ShowEmote(5, 0x100, 55);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Event_ShowMessageThenWait(15, 0, 10);
  FieldActor_ShowEmote(17, 0x105, 50);
  v10 = Func_020001d8__s03a0(13);
  Func_02000550();
  Event_Wait(30);
  Data_02003090__s03a0 = r7_1;
  Data_02003024__s03a0 = r8_1;
  Event_PlaySound(153);
  Func_02000510();
  FieldEvent_ShowMessageThenWait(0x200d, 0, 10);
  Event_ShowMessageThenWait(14, 0, 5);
  v12 = Actor_GetById(17);
  v12->actionFlags &= ~1;
  Actor_WalkByAndWait(17, 0, -16);
  Event_Wait(1);
  v13 = Actor_GetById(17);
  v13->actionFlags |= 1;
  Event_Wait(25);
  Actor_StartRepeatedMotion(17, 3);
  Actor_SetAttachedEffect(17, 0x102);
  Event_Wait(40);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_ShowEmote(13, 0x108, 30);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_FaceDirectionAndWait(6, FACING_EAST);
  Event_Wait(10);
  Actor_ShowEmote(6, 0x101, 30);
  Event_ShowMessageThenWait(6, 0, 5);
  Actor_WalkByAndWait(17, 0, 16);
  Actor_FaceDirectionAndWait(17, FACING_WEST);
  Event_Wait(10);
  Actor_ShowEmote(17, 0x109, 30);
  Event_ShowMessageThenWait(17, 0, 5);
  FieldActor_ShowEmote(5, 0x100, 30);
  Actor_FaceDirectionAndWait(5, FACING_EAST);
  Event_Wait(5);
  Event_ShowMessageThenWait(5, 0, 5);
  Actor_RunRepeatedMotion(14, 2);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_FaceDirection(17, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(6, FACING_NORTH);
  Event_Wait(20);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x200d, 0, 5);
  v14 = Func_020001d8__s03a0(13);
  Func_02000550();
  Event_Wait(10);
  Data_02003098__s03a0 = r7_1;
  gData_0200308c__s03a0 = 2;
  Event_Wait(150);
  Actor_StartRepeatedMotion(17, 3);
  Actor_SetAttachedEffect(17, 0x102);
  Event_Wait(40);
  Event_ShowMessageThenWait(17, 0, 0);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_SetSpeed(15, 0x10000, 0x8000);
  Actor_WalkToAndWait(15, 888, 444);
  Actor_FaceDirectionAndWait(15, FACING_SOUTH + FACING_STEP);
  Event_Wait(5);
  FieldCamera_MoveTo(PIXELS(872), -1, PIXELS(472), 1);
  Func_020020ac__lemuria_palace();
  Event_Wait(10);
  Actor_FaceDirectionAndWait(15, FACING_WEST);
  Event_Wait(10);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_FaceDirectionAndWait(13, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(13, 4);
  Event_ShowMessageThenWait(13, 0, 5);
  Actor_RunRepeatedMotion(15, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(15, 0, 5);
  FieldActor_ShowEmote(13, 0x109, 30);
  Actor_FaceDirectionAndWait(13, FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  Func_020001d8__s03a0(13);
  Event_PlaySound(153);
  r5_3 = 0;
  goto L_02008ff2;
L_02008ff0:
  r5_3 = r5_3 + 1;
L_02008ff2:
  if (r5_3 <= 119) {
    wait(1);
    v17 = __modsi3((Data_02003098__s03a0) >> 2, 14);
    r0 = v17;
    if (v17 != 0)
      goto L_02008ff0;
  }
  Func_020004e8();
  Field_StartPaletteTransition(32);
  Event_Wait(40);
  Actor_FaceDirectionAndWait(13, FACING_EAST);
  Event_Wait(10);
  Event_ShowMessageThenWait(13, 0, 5);
  Actor_RunRepeatedMotion(13, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(13, 0, 5);
  Actor_SetAnimationAndWait(15, 3);
  Event_Wait(15);
  Actor_FaceDirection(14, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(13, 0x7fff);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_FaceDirectionAndWait(7, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(7, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_ShowEmote(13, 0x108, 30);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_SetAnimationAndWait(7, 4);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_ShowEmote(13, 0x101, 50);
  Actor_ShowEmote(7, 0x102, 30);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_ShowEmote(14, 0x100, 0);
  Actor_ShowEmote(13, 0x100, 50);
  Actor_FaceDirection(14, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(13, FACING_EAST);
  Event_Wait(20);
  Actor_RunRepeatedMotion(15, 2);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_RunRepeatedMotion(17, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(15, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(13, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(4, FACING_EAST);
  Event_Wait(25);
  Actor_FaceDirectionAndWait(17, FACING_WEST);
  Event_Wait(20);
  Actor_ShowEmote(17, 0x102, 30);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_FaceDirectionAndWait(17, FACING_NORTHWEST + FACING_STEP);
  Event_Wait(15);
  Actor_RunRepeatedMotion(17, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_ShowEmote(13, 0x108, 30);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_SetAnimationAndWait(17, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_SetAnimationAndWait(13, 4);
  Event_ShowMessageThenWait(0x200d, 0, 5);
  Actor_ShowEmote(17, 0x109, 30);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_RunRepeatedMotion(7, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  Event_Wait(20);
  Actor_SetAnimationAndWait(14, 4);
  Event_ShowMessageThenWait(14, 0, 5);
  FieldActor_ShowEmote(15, 0x102, 40);
  Actor_FaceDirectionAndWait(15, FACING_WEST);
  Event_Wait(10);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_SetAnimationAndWait(13, 4);
  Actor_FaceDirectionAndWait(13, FACING_EAST);
  Event_Wait(10);
  Event_ShowMessageThenWait(13, 0, 5);
  FieldActor_ShowEmote(15, 0x101, 50);
  Actor_SetAnimationAndWait(15, 4);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(13, 0, 5);
  FieldActor_ShowEmote(15, 0x100, 45);
  Actor_FaceDirectionAndWait(13, FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(5);
  Event_ShowMessageThenWait(13, 0, 5);
  Actor_FaceDirectionAndWait(15, FACING_SOUTH + FACING_STEP);
  Event_Wait(10);
  Actor_ShowEmote(15, 0x103, 30);
  Event_OpenMessage(15, 0);
  v18 = Event_AskYesNoWithActorAnimation(4, 0);
  if (v18 == 0) {
    Event_Wait(15);
    Event_ShowMessageThenWait(15, 0, 5);
    FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
    r2 = ((s32)Ram_HeapSlots->eventWork) + 452;
    r3 = (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
  } else {
    Event_Wait(35);
    FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
    Event_ShowMessageThenWait(15, 0, 5);
  }
  Actor_FaceDirectionAndWait(15, FACING_WEST);
  Event_Wait(10);
  Actor_ShowEmote(15, 0x107, 30);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_FaceDirectionAndWait(13, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(13, 0, 5);
  Actor_ShowEmote(13, 0x109, 30);
  Event_ShowMessageThenWait(13, 0, 5);
  Actor_SetAnimationAndWait(15, 4);
  Actor_SetSpeed(15, 0x8000, 0x4000);
  v19 = Actor_GetById(15);
  v19->actionFlags &= ~1;
  Actor_WalkByAndWait(15, 32, 0);
  Event_Wait(1);
  v20 = Actor_GetById(15);
  v20->actionFlags = 1 | (v20->actionFlags);
  Actor_FaceDirection(7, FACING_EAST, 0);
  Actor_FaceDirection(14, FACING_EAST, 0);
  Actor_FaceDirection(13, FACING_EAST, 0);
  Actor_FaceDirection(17, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  Actor_FaceDirection(5, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(4, FACING_NORTHEAST);
  Event_Wait(5);
  Actor_RunRepeatedMotion(15, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_SetAnimationAndWait(15, 4);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_RunRepeatedMotion(7, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_SetAnimationAndWait(15, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_ShowEmote(4, 0x102, 0);
  Actor_ShowEmote(5, 0x102, 0);
  Actor_ShowEmote(6, 0x102, 0);
  Actor_ShowEmote(17, 0x102, 0);
  Actor_ShowEmote(7, 0x102, 60);
  Actor_ShowEmote(15, 0x108, 30);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_RunRepeatedMotion(15, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_SetSpeed(15, 0x13333, 0x9999);
  Actor_WalkByAndWait(15, -50, 24);
  Actor_WatchAndSetScript(13, 15);
  Actor_WatchAndSetScript(14, 15);
  Actor_WatchAndSetScript(7, 15);
  Actor_WatchAndSetScript(4, 15);
  Actor_WatchAndSetScript(5, 15);
  Actor_WatchAndSetScript(6, 15);
  Actor_WatchAndSetScript(17, 15);
  Camera_FollowActor(15, 1);
  Actor_WalkByAndWait(15, -8, 32);
  Actor_FaceDirection(16, FACING_SOUTH + FACING_STEP, 0);
  Actor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  Actor_StartRepeatedMotion(5, 3);
  Actor_SetAttachedEffect(5, 0x102);
  Event_Wait(37);
  Actor_SetSpeed(4, 0x23333, 0x11999);
  Actor_SetSpeed(5, 0x23333, 0x11999);
  Actor_WalkBy(5, -8, 16);
  Actor_WalkByAndWait(4, 8, 16);
  Actor_WaitForMoveById(5);
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(5, FACING_EAST);
  Event_Wait(15);
  Actor_WatchAndSetScript(4, 15);
  Actor_WatchAndSetScript(5, 15);
  Actor_WalkByAndWait(15, 0, 64);
  Event_Wait(15);
  Actor_FaceDirectionAndWait(15, FACING_NORTH + FACING_STEP);
  Event_Wait(10);
  Actor_ShowEmote(15, 0x107, 40);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_FaceDirectionAndWait(15,
        FACING_SOUTH + FACING_STEP);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, FACING_WEST);
  Event_Wait(15);
  Actor_FaceDirectionAndWait(16,
        FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(15);
  Actor_SetAnimation(16, 5);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_WalkByAndWait(15, 0, 96);
  Event_Wait(20);
  Actor_SetPosition(15, 0, 0);
  Actor_SetSpeed(4, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_WalkBy(5, 8, -16);
  Actor_WalkByAndWait(4, -8, -16);
  Actor_WaitForMoveById(5);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(13, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  FieldCamera_MoveTo(PIXELS(872), -1, PIXELS(480), 1);
  Func_020020ac__lemuria_palace();
  Event_Wait(10);
  Actor_Stop(16);
  Actor_SetAnimation(16, 1);
  Actor_FaceDirectionAndWait(17,
        FACING_NORTHWEST + FACING_STEP);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_SetAnimationAndWait(13, 4);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_SetAnimationAndWait(13, 4);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_FaceDirection(7, FACING_EAST, 0);
  Actor_FaceDirection(14, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(17, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(4, FACING_EAST);
  Event_Wait(40);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(13, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(17, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  Event_Wait(20);
  Actor_RunRepeatedMotion(13, 2);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_RunRepeatedMotion(6, 2);
  Event_ShowMessageThenWait(6, 0, 5);
  Actor_ShowEmote(13, 0x108, 30);
  Event_ShowMessageThenWait(0x200d, 0, 5);
  Actor_FaceDirectionAndWait(13, FACING_WEST);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(14, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(20);
  Actor_FaceDirection(13, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(14, FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(5);
  Actor_WalkByAndWait(14, 24, 26);
  Actor_WalkByAndWait(14, 0, 8);
  Event_Wait(10);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(10);
  Actor_Jump(14, 4, 20);
  r5_4 = 15;
  goto L_02009988;
L_02009988:
  v22 = Actor_GetById(64);
  v22->scaleY = (v22->scaleY) + 4096;
  r5_4 = r5_4 - 1;
  wait(1);
  if (r5_4 >= 0)
    goto L_02009988;
  v23 = Actor_GetById(64);
  v23->scaleY = 0x10000;
  Event_Wait(5);
  Actor_ShowEmote(5, 0x101, 0);
  Actor_ShowEmote(17, 0x101, 0);
  Actor_ShowEmote(6, 0x101, 0);
  Actor_ShowEmote(4, 0x101, 60);
  Actor_WalkByAndWait(14, -24, -26);
  Actor_WalkByAndWait(14, 0, -8);
  Actor_FaceDirectionAndWait(14,
        FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(17, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(4, FACING_EAST);
  Event_Wait(30);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(17, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  Event_Wait(15);
  Actor_RunRepeatedMotion(13, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_FaceDirectionAndWait(7, FACING_EAST);
  Event_Wait(10);
  Actor_ShowEmote(7, 0x101, 30);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirectionAndWait(13, FACING_WEST);
  Actor_SetAnimationAndWait(13, 4);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(13,
        FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(5);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_SetAnimationAndWait(17, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_FaceDirection(14, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(13, FACING_WEST);
  Event_Wait(30);
  Actor_SetAnimation(14, 3);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(20);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(13,
        FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_ShowEmote(13, 0x102, 30);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200d, 0, 5);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(17, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(4, FACING_EAST);
  Event_Wait(30);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimationAndWait(17, 3);
  Event_Wait(10);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(17, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  Event_Wait(20);
  Actor_RunRepeatedMotion(13, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x200d, 0, 10);
  Actor_FaceDirectionAndWait(13, FACING_NORTH + FACING_STEP);
  Event_Wait(5);
  Actor_WalkToAndWait(13, 864, 392);
  Actor_FaceDirectionAndWait(13,
        FACING_SOUTHEAST + FACING_STEP);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_FaceDirectionAndWait(14, FACING_WEST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_FaceDirectionAndWait(14,
        FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_FaceDirection(17, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(4, FACING_EAST);
  Event_Wait(20);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimationAndWait(17, 3);
  Event_Wait(10);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_SetSpeed(17, 0x13333, 0x9999);
  Actor_SetAnimation(17, 2);
  {
    s32 party = (s32)&gMainRoutineWork;

    r5_5 = party + 532;
  }
  v24 = Actor_GetById(FIELD_AT(r5_5, s32, 0));
  if (v24 != 0) {
    Actor_SetDestination(17, ((s16 *)&v24->x)[1], ((s16 *)&v24->z)[1]);
  }
  Actor_WaitForMoveById(17);
  Actor_SetPosition(17, 0, 0);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetAnimation(5, 2);
  v25 = Actor_GetById(FIELD_AT(r5_5, s32, 0));
  if (v25 != 0) {
    Actor_SetDestination(5, ((s16 *)&v25->x)[1], ((s16 *)&v25->z)[1]);
  }
  Actor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetAnimation(6, 2);
  v26 = Actor_GetById(FIELD_AT(r5_5, s32, 0));
  if (v26 != 0) {
    Actor_SetDestination(6, ((s16 *)&v26->x)[1], ((s16 *)&v26->z)[1]);
  }
  Actor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Event_Wait(10);
  Actor_WalkByAndWait(4, -32, -48);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  Actor_FaceDirection(14, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  Event_Wait(10);
  Actor_SetAnimationAndWait(4, 3);
  Event_Wait(20);
  Actor_SetAnimationAndWait(7, 3);
  Event_Wait(10);
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetAnimation(7, 2);
  v27 = Actor_GetById(FIELD_AT(r5_5, s32, 0));
  if (v27 != 0) {
    Actor_SetDestination(7, ((s16 *)&v27->x)[1], ((s16 *)&v27->z)[1]);
  }
  Actor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(15);
  Actor_WalkToAndWait(14, 840, 408);
  Actor_FaceDirectionAndWait(14,
        FACING_SOUTHEAST + FACING_STEP);
  Actor_FaceDirection(16, FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(10);
  v29 = Func_02001fd4__s03a0();
  r0 = v29;
L_02009f20:
  return r0;
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 288, 0, 454, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(204),
    SCENE_EXIT_IF(1, 202, 11, -1),
    SCENE_EXIT_IF(2, 204, 6, -1),
    SCENE_EXIT_IF(3, 204, 7, -1),
    SCENE_EXIT_IF(4, 204, 8, -1),
    SCENE_EXIT_IF(5, 204, 9, -1),
    SCENE_EXIT_IF(6, 204, 2, -1),
    SCENE_EXIT_IF(7, 204, 3, -1),
    SCENE_EXIT_IF(8, 204, 4, -1),
    SCENE_EXIT_IF(9, 204, 5, -1),
    SCENE_EXIT_IF(10, 202, 10, -1),
    SCENE_EXIT_IF(11, 204, 13, -1),
    SCENE_EXIT_IF(12, 204, 14, -1),
    SCENE_EXIT_IF(13, 204, 11, -1),
    SCENE_EXIT_IF(14, 204, 12, -1),
    SCENE_EXIT_IF(15, 204, 16, -1),
    SCENE_EXIT_IF(16, 204, 15, -1),
    SCENE_EXIT_IF(17, 202, 17, -1),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gActorPlacements[] = {
    {157, CONDITION_ALWAYS, 1, PIXELS(912), 0, PIXELS(624), FACING_SOUTH + FACING_STEP, 1, 0},
    {156, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(520), FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {160, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(456), FACING_EAST, 3, 0},
    {165, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(360), FACING_EAST, 1, 0},
    {164, CONDITION_ALWAYS, 1, PIXELS(80), 0, PIXELS(136), FACING_NORTH + FACING_STEP, 1, 0},
    {31, CONDITION_ALWAYS, 1, PIXELS(864), 0, PIXELS(392), FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {43, CONDITION_ALWAYS, 1, PIXELS(840), 0, PIXELS(408), FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {34, CONDITION_ALWAYS, 1, PIXELS(888), 0, PIXELS(392), FACING_WEST, 1, 0},
    {231, CONDITION_ALWAYS, 1, PIXELS(888), 0, PIXELS(560), FACING_SOUTH + FACING_STEP, 1, 0},
    {56, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, PIXELS(824), 0, PIXELS(424), FACING_NORTHEAST, 2,
     0},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
s16 gData_0200233c[] = {
    1, 52, 1, 2, 6, 2, 52, 1,
    2, 6, -1, 2, 54, 2, 2, 6,
    4, 54, 2, 2, 6, -1,
};

DEFINE_G_DATA_0200064C(gData_02002368, gData_0200233c, 0x170005, gData_0200233c,
    0x17001e)

s32 gData_02002378[] = {
    (s32)gData_0200233c,
    0x40038,
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_020000c8)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_020000c8)},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {0xc602, 16, CONDITION_ALWAYS, EVENT(Func_020000c8)},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_STEP, 20, 0x96a, EVENT(Func_0200058c)},
    {TRIGGER_TALK, 8, 0x196a, MSGNO(MsgLemuriaPalace12)},
    {TRIGGER_TALK, 9, 0x196a, MSGNO(MsgLemuriaPalace13)},
    {TRIGGER_TALK, 10, 0x196a, MSGNO(MsgLemuriaPalace14)},
    {TRIGGER_TALK, 11, 0x196a, MSGNO(MsgLemuriaPalace15)},
    {TRIGGER_TALK, 12, 0x196a, MSGNO(MsgLemuriaPalace16)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x196a, MSGNO(MsgLemuriaPalace17)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x196a, MSGNO(MsgLemuriaPalace18)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x196a, MSGNO(MsgLemuriaPalace19)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x196a, MSGNO(MsgLemuriaPalace20)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x196a, MSGNO(MsgLemuriaPalace21)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace01)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace02)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace03)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace04)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace05)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace06)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace07)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace08)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace09)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace10)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace22)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace23)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace24)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace25)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace26)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace27)},
    {243, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors27))},
    {243, 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors28))},
    {243, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors29))},
    {243, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors30))},
    {243, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors31))},
    {243, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors32))},
    {243, 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors33))},
    {243, 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors34))},
    {451, 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors35))},
    {TRIGGERS_END},
};

u32 gData_02002608[] = {
    0x76c07b00, 0x6e407280, 0x65c06a00, 0x65c06180, 0x6e406a00, 0x76c07280,
};
// clang-format on
#undef BLEND_SET_ALPHA
#undef FIELD_AT
