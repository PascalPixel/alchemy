// ---- games/tla/src/scene/lemuria_palace.c ----
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

struct DoorStep {
  s32 script;
  u16 x;
  u16 z;
};

void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void Func_02001fcc();
void Func_02001fd4();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000054(struct DoorStep *step, s32 countdown, s32 unused) {
  Func_02001fcc();
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
  Func_02001fd4();
}

extern s32 gData_02002368[];
extern s32 gData_02002378[];

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
s32 Func_02000158(void) {
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

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Func_02001f9c(struct Actor *object);

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
  Func_02001f9c(first);
  Actor_WaitForMove(second);
  Func_02001f9c(second);
  FieldEvent_Wait(30);
}

extern u8 gData_0200308c[];
extern u8 Data_02003098[];
extern u8 Data_02002fd7[];
extern u8 gData_02002fe8[];
extern u8 Data_02003090[];
extern u8 Data_02003024[];
extern u8 Data_02003060[];
extern u8 Data_02003030[];
s32 __modsi3();
s32 Func_020020d4();
s32 Func_0200210c();

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
  Func_020020d4(&position);
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

struct Unk_020003a0Sprite {
  u8 unk00[5];
  u8 unk05_0 : 2;
  u8 shape : 2;
  u8 unk05_4 : 1;
  u8 blend : 1;
  u8 unk05_6 : 2;
  u8 unk06[2];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unk0a;
  // XXX (no evidence): what these hold; read as s32, whose alias set orders
  // the stores as the listing does.
  s32 unk0c[4];
  u16 unk1c;
  u16 unk1e;
};

extern const u8 Data_02002620[];
extern const u8 Data_02002af8[];
extern s32 gData_02003020;
extern s32 gData_02003094;
extern s32 Data_02003088;
extern s32 Data_02003058;
/* VIEW(s03a0): variable Data_02003060 as extern struct Unk_020003a0Sprite Data_02003060; lemuria_palace has extern u8 Data_02003060[]; tried as the one declaration extern struct Unk_020003a0Sprite Data_02003060, it does not compile in tla-ja: games/tla/src/scene/lemuria_palace.c:275: conflicting types for `Data_02003060'. */
extern struct Unk_020003a0Sprite Data_02003060__s03a0 __asm__("Data_02003060");
/* VIEW(s03a0): variable Data_02003030 as extern struct Unk_020003a0Sprite Data_02003030; lemuria_palace has extern u8 Data_02003030[]; tried as the one declaration extern struct Unk_020003a0Sprite Data_02003030, it does not compile in tla-ja: games/tla/src/scene/lemuria_palace.c:277: conflicting types for `Data_02003030'. */
extern struct Unk_020003a0Sprite Data_02003030__s03a0 __asm__("Data_02003030");
/* VIEW(s03a0): variable Data_02003090 as extern s32 Data_02003090; lemuria_palace has extern u8 Data_02003090[]; tried as the one declaration extern s32 Data_02003090, it does not compile in tla-ja: games/tla/src/scene/lemuria_palace.c:279: conflicting types for `Data_02003090'. */
extern s32 Data_02003090__s03a0 __asm__("Data_02003090");
/* VIEW(s03a0): variable Data_02003024 as extern s32 Data_02003024; lemuria_palace has extern u8 Data_02003024[]; tried as the one declaration extern s32 Data_02003024, it does not compile in tla-ja: games/tla/src/scene/lemuria_palace.c:281: conflicting types for `Data_02003024'. */
extern s32 Data_02003024__s03a0 __asm__("Data_02003024");
/* VIEW(s03a0): variable Data_02003098 as extern s32 Data_02003098; lemuria_palace has extern u8 Data_02003098[]; tried as the one declaration extern s32 Data_02003098, it does not compile in tla-ja: games/tla/src/scene/lemuria_palace.c:283: conflicting types for `Data_02003098'. */
extern s32 Data_02003098__s03a0 __asm__("Data_02003098");
/* VIEW(s03a0): variable gData_0200308c as extern u8 gData_0200308c; lemuria_palace has extern u8 gData_0200308c[]; tried as the definition's extern struct Trigger gData_0200308c[], which the object already declares, it does not compile in tla-ja: games/tla/src/scene/lemuria_palace.c:330: `gData_0200308c' has an incomplete type. */
extern u8 gData_0200308c__s03a0 __asm__("gData_0200308c");

void *Heap_Alloc(s32 size);
void Heap_Release(void *p);
void Func_02001f6c(const u8 *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Func_02002104(struct Unk_020003a0Sprite *sprite, s32 a, s32 b, u32 attr,
                   s32 tile);
void Func_020002b4(void);

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
  Func_02001f6c(Data_02002620, buffer);
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
  Func_02001f6c(Data_02002af8, buffer);
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

s32 Resource_ResetEntry(u32 slot);

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

void Func_02001fcc();
/* VIEW(s03a0): routine Func_02001fd4 as s32 Func_02001fd4(); lemuria_palace has void Func_02001fd4(); tried as the definition's void Func_02001fd4(void), it does not compile in tla-ja: games/tla/src/scene/lemuria_palace.c:442: void value not ignored as it ought to be. */
extern s32 Func_02001fd4__s03a0() __asm__("Func_02001fd4");
s32 Party_AddActiveOwner();
void Func_020020ac();
/* VIEW(s03a0): routine Func_020001d8 as s32 Func_020001d8(); the definition has void Func_020001d8(void); tried as the definition's void Func_020001d8(struct Actor *object), it does not compile in tla-ja: games/tla/src/scene/lemuria_palace.c:406: conflicting types for `Func_020001d8'. */
extern s32 Func_020001d8__s03a0() __asm__("Func_020001d8");
void Func_02002114();
s32 __modsi3();
void Field_StartPaletteTransition();

s32 Func_0200058c(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r5_1, r7_1, r8_1, r5_3, r5_4, r5_5;
  s32 v0, v2, r0, v10, v14, v17, v18, r2, r3, v29;
  struct Actor *v3, *v6, *v7, *v12, *v13, *v19, *v20, *v22, *v23, *v24, *v25,
      *v26, *v27;

  Func_02001fcc();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaPalace11);
  Call3((void (*)())Actor_SetSpeedParameters, 16, 0x13333, 0x9999);
  Call3((void (*)())Actor_CommitPositionAndActivate, 16, -24, 0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 16,
        FACING_SOUTH + FACING_STEP);
  {
    s32 party = (s32)&gMainRoutineWork;

    r5_1 = party + 532;
  }
  Actor_SetAngleToward(16, FIELD_AT(r5_1, s32, 0), 0);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 16, 0);
  v0 = Event_AskYesNoWithActorAnimation(4, 0);
  r7_1 = v0;
  if (r7_1 == 0)
    goto L_0200863c;
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(16, 4);
  Event_ShowMessageThenWait(16, 0, 5);
  Actor_CommitPositionAndActivate(FIELD_AT(r5_1, s32, 0), 0, 16);
  Actor_CommitPositionAndActivate(16, 24, 0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 16,
        FACING_SOUTH + FACING_STEP);
  v2 = Func_02001fd4__s03a0();
  r0 = v2;
  goto L_02009f20;
L_0200863c:
  Motion_SetModeAndWaitAnimation(16, 3);
  Event_Wait(10);
  Actor_CommitPositionAndActivate(16, 24, 0);
  Call3((void (*)())Actor_ArmCallback, 16, FACING_SOUTH + FACING_STEP, 0);
  Actor_WatchAndSetScript(16, FIELD_AT(r5_1, s32, 0));
  GameFlag_SetBit(0x96a);
  GameFlag_SetBit(0x96b);
  Party_AddActiveOwner(7);
  v3 = Actor_GetById(64);
  v3->scaleY = r7_1;
  Actor_ArmCallback(14, FACING_EAST, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 13, 0xb333, 0x5999);
  Call3((void (*)())Actor_SetSpeedParameters, 14, 0xb333, 0x5999);
  Call3((void (*)())Actor_SetSpeedParameters, 7, 0xb333, 0x5999);
  Call3((void (*)())Actor_SetPositionAndReset, 4, 872, 528);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(872), -1, PIXELS(408),
        1);
  Func_020020ac();
  Event_Wait(10);
  Call3((void (*)())Actor_ArmCallback, 16, FACING_SOUTH + FACING_STEP, 0);
  Value3((s32 (*)())Func_080d47b4, 13, 0x100, 45);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 13,
        FACING_SOUTH + FACING_STEP);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(10);
  FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452) =
      (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTH, 0);
  Call3((void (*)())Actor_ArmCallback, 14, FACING_SOUTHEAST + FACING_STEP, 0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 15,
        FACING_SOUTH + FACING_STEP);
  Event_Wait(30);
  Actor_ArmCallbackAndRefresh(14, FACING_EAST);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Motion_SetModeAndWaitAnimation(13, 4);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Motion_SetVarCbAndRefresh(15, 2);
  Event_Wait(5);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 15, FACING_WEST);
  Value3((s32 (*)())Func_080d47b4, 15, 0x103, 30);
  Event_ShowMessageThenWait(15, 0, 5);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_NORTHEAST, 0);
  Actor_ArmCallback(13, FACING_EAST, 0);
  Actor_ArmCallbackAndRefresh(14, FACING_EAST);
  Event_Wait(10);
  Value3((s32 (*)())Func_080d47b4, 13, 0x108, 30);
  Event_ShowMessageThenWait(13, 0, 5);
  Motion_SetVarCbAndRefresh(15, 2);
  Event_ShowMessageThenWait(15, 0, 5);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(13, 0, 5);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 13,
        FACING_SOUTH + FACING_STEP);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(10);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Camera_FollowActor(13, 1);
  Call3((void (*)())Actor_ResetAndSetPositionInMode2, 13, 864, 444);
  Event_Wait(20);
  Call3((void (*)())Actor_ResetAndSetPositionInMode2, 14, 848, 444);
  Event_Wait(20);
  Call3((void (*)())Actor_ResetAndSetPositionInMode2, 7, 832, 444);
  Actor_WaitForMoveById(13);
  Actor_SetModeById(13, 1);
  Call3((void (*)())Actor_ArmCallback, 13, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WaitForMoveById(14);
  Actor_SetModeById(14, 1);
  Call3((void (*)())Actor_ArmCallback, 14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WaitForMoveById(7);
  Actor_SetModeById(7, 1);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 7, FACING_SOUTH);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(872), -1, PIXELS(496),
        1);
  Func_020020ac();
  Event_Wait(5);
  Call4((void (*)())Func_080d3070, 17, 16, 0, FACING_NORTHWEST + FACING_STEP);
  Call4((void (*)())Func_080d3070, 5, -16, 0, FACING_NORTH);
  Call4((void (*)())Func_080d3070, 6, -32, 0, FACING_NORTH);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTH, 0);
  Actor_WaitForMoveById(6);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(10);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Actor_ArmCallbackAndRefresh(7, FACING_EAST);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_Wait(10);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 7, FACING_SOUTH);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 5);
  Call3((void (*)())Actor_ArmCallback, 14, FACING_SOUTH + FACING_STEP, 0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 13,
        FACING_SOUTH + FACING_STEP);
  Call3((void (*)())Actor_CommitPositionAndActivate, 6, 0, -8);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(6, 3);
  Event_Wait(10);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 7, FACING_SOUTH);
  Event_ShowMessageThenWait(7, 0, 5);
  Call3((void (*)())Actor_ArmCallback, 14, FACING_SOUTHEAST + FACING_STEP, 0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 13,
        FACING_SOUTH + FACING_STEP);
  Call3((void (*)())Actor_CommitPositionAndActivate, 5, 0, -8);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(5, 3);
  Event_Wait(10);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 7, FACING_SOUTHEAST);
  Event_ShowMessageThenWait(7, 0, 5);
  Call3((void (*)())Actor_ArmCallback, 14, FACING_SOUTHEAST + FACING_STEP, 0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 13,
        FACING_SOUTHEAST + FACING_STEP);
  Call3((void (*)())Actor_CommitPositionAndActivate, 4, 0, -8);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(10);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 7, FACING_SOUTHEAST);
  Event_ShowMessageThenWait(7, 0, 5);
  Call3((void (*)())Actor_ArmCallback, 14, FACING_SOUTHEAST + FACING_STEP, 0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 13,
        FACING_SOUTHEAST + FACING_STEP);
  v6 = Actor_GetById(17);
  v6->actionFlags &= ~1;
  Actor_CommitPositionAndActivate(17, 0, -8);
  Event_Wait(1);
  v7 = Actor_GetById(17);
  v7->actionFlags |= 1;
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_Wait(10);
  Actor_ArmCallbackAndRefresh(14, FACING_EAST);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 13, FACING_WEST);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(20);
  Call3((void (*)())Actor_ArmCallback, 14, FACING_SOUTHEAST + FACING_STEP, 0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 13,
        FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  Value3((s32 (*)())Func_080d47b4, 13, 0x108, 30);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Actor_StartRepeatedMotion(17, 3);
  Call2((void (*)())Func_080d489c, 17, 0x102);
  Event_Wait(37);
  Event_ShowMessageThenWait(17, 0, 5);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(10);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 0);
  Event_ShowMessageThenWait(15, 0, 5);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 13,
        FACING_NORTH + FACING_STEP);
  Event_Wait(15);
  Event_ShowMessageThenWait(15, 0, 5);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 13,
        FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Value3((s32 (*)())Func_080d47b4, 13, 0x105, 30);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Motion_SetVarCbAndRefresh(13, 2);
  Event_Wait(5);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 0);
  Event_ShowMessageThenWait(15, 0, 5);
  Motion_SetVarCbAndRefresh(7, 2);
  Event_Wait(10);
  Actor_ArmCallbackAndRefresh(7, FACING_NORTHEAST);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(7, 4);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_ArmCallbackAndRefresh(13, FACING_NORTH + FACING_STEP);
  Event_Wait(5);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Motion_SetModeAndWaitAnimation(13, 4);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Motion_SetVarCbAndRefresh(17, 2);
  Event_ShowMessageThenWait(17, 0, 5);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 14, FACING_SOUTHEAST + FACING_STEP, 0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 13,
        FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(10);
  Func_020001d8__s03a0(13);
  Func_02002114(32, 0x2108);
  Event_Wait(32);
  Event_Wait(20);
  Func_020003a0();
  r8_1 = 1;
  Data_02003090__s03a0 = r8_1;
  Data_02003024__s03a0 = r7_1;
  Sound_Play(153);
  Func_02000510();
  Value3((s32 (*)())Func_080d47b4, 4, 0x100, 0);
  Value3((s32 (*)())Func_080d47b4, 17, 0x100, 0);
  Value3((s32 (*)())Func_080d47b4, 6, 0x100, 0);
  Value3((s32 (*)())Func_080d47b4, 5, 0x100, 55);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Event_ShowMessageThenWait(15, 0, 10);
  Func_080d47b4(17, 0x105, 50);
  v10 = Func_020001d8__s03a0(13);
  Func_02000550();
  Event_Wait(30);
  Data_02003090__s03a0 = r7_1;
  Data_02003024__s03a0 = r8_1;
  Call1((void (*)())Sound_Play, 153);
  Func_02000510();
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 10);
  Event_ShowMessageThenWait(14, 0, 5);
  v12 = Actor_GetById(17);
  v12->actionFlags &= ~1;
  Actor_CommitPositionAndActivate(17, 0, -16);
  Event_Wait(1);
  v13 = Actor_GetById(17);
  v13->actionFlags |= 1;
  Event_Wait(25);
  Actor_StartRepeatedMotion(17, 3);
  Call2((void (*)())Func_080d489c, 17, 0x102);
  Event_Wait(40);
  Event_ShowMessageThenWait(17, 0, 5);
  Value3((s32 (*)())Func_080d47b4, 13, 0x108, 30);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Actor_ArmCallbackAndRefresh(6, FACING_EAST);
  Event_Wait(10);
  Value3((s32 (*)())Func_080d47b4, 6, 0x101, 30);
  Event_ShowMessageThenWait(6, 0, 5);
  Actor_CommitPositionAndActivate(17, 0, 16);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 17, FACING_WEST);
  Event_Wait(10);
  Value3((s32 (*)())Func_080d47b4, 17, 0x109, 30);
  Event_ShowMessageThenWait(17, 0, 5);
  Func_080d47b4(5, 0x100, 30);
  Actor_ArmCallbackAndRefresh(5, FACING_EAST);
  Event_Wait(5);
  Event_ShowMessageThenWait(5, 0, 5);
  Motion_SetVarCbAndRefresh(14, 2);
  Event_ShowMessageThenWait(14, 0, 5);
  Call3((void (*)())Actor_ArmCallback, 17, FACING_NORTHWEST + FACING_STEP, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_NORTH, 0);
  Actor_ArmCallbackAndRefresh(6, FACING_NORTH);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x200d, 0, 5);
  v14 = Func_020001d8__s03a0(13);
  Func_02000550();
  Event_Wait(10);
  Data_02003098__s03a0 = r7_1;
  gData_0200308c__s03a0 = 2;
  Event_Wait(150);
  Actor_StartRepeatedMotion(17, 3);
  Func_080d489c(17, 0x102);
  Event_Wait(40);
  Event_ShowMessageThenWait(17, 0, 0);
  Event_ShowMessageThenWait(15, 0, 5);
  Call3((void (*)())Actor_SetSpeedParameters, 15, 0x10000, 0x8000);
  Call3((void (*)())Actor_SetPositionAndReset, 15, 888, 444);
  Actor_ArmCallbackAndRefresh(15, FACING_SOUTH + FACING_STEP);
  Event_Wait(5);
  Camera_MoveWithinMapBounds(PIXELS(872), -1, PIXELS(472), 1);
  Func_020020ac();
  Event_Wait(10);
  Actor_ArmCallbackAndRefresh(15, FACING_WEST);
  Event_Wait(10);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_ArmCallbackAndRefresh(13, FACING_EAST);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(13, 4);
  Event_ShowMessageThenWait(13, 0, 5);
  Motion_SetVarCbAndRefresh(15, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(15, 0, 5);
  Func_080d47b4(13, 0x109, 30);
  Actor_ArmCallbackAndRefresh(13, FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  Func_020001d8__s03a0(13);
  Sound_Play(153);
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
  Actor_ArmCallbackAndRefresh(13, FACING_EAST);
  Event_Wait(10);
  Event_ShowMessageThenWait(13, 0, 5);
  Motion_SetVarCbAndRefresh(13, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(13, 0, 5);
  Motion_SetModeAndWaitAnimation(15, 3);
  Event_Wait(15);
  Call3((void (*)())Actor_ArmCallback, 14, FACING_WEST, 0);
  Actor_ArmCallbackAndRefresh(13, 0x7fff);
  Event_Wait(10);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Actor_ArmCallbackAndRefresh(7, FACING_EAST);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 5);
  Value3((s32 (*)())Func_080d47b4, 13, 0x108, 30);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Motion_SetModeAndWaitAnimation(7, 4);
  Event_ShowMessageThenWait(7, 0, 5);
  Value3((s32 (*)())Func_080d47b4, 13, 0x101, 50);
  Value3((s32 (*)())Func_080d47b4, 7, 0x102, 30);
  Event_ShowMessageThenWait(7, 0, 5);
  Value3((s32 (*)())Func_080d47b4, 14, 0x100, 0);
  Value3((s32 (*)())Func_080d47b4, 13, 0x100, 50);
  Actor_ArmCallback(14, FACING_EAST, 0);
  Actor_ArmCallbackAndRefresh(13, FACING_EAST);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(15, 2);
  Event_ShowMessageThenWait(15, 0, 5);
  Motion_SetVarCbAndRefresh(17, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(17, 0, 5);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 15, FACING_SOUTH + FACING_STEP, 0);
  Call3((void (*)())Actor_ArmCallback, 14, FACING_SOUTHEAST + FACING_STEP, 0);
  Call3((void (*)())Actor_ArmCallback, 13, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_ArmCallbackAndRefresh(4, FACING_EAST);
  Event_Wait(25);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 17, FACING_WEST);
  Event_Wait(20);
  Value3((s32 (*)())Func_080d47b4, 17, 0x102, 30);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_ArmCallbackAndRefresh(17, FACING_NORTHWEST + FACING_STEP);
  Event_Wait(15);
  Motion_SetVarCbAndRefresh(17, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 5);
  Motion_SetVarCbAndRefresh(14, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(14, 0, 5);
  Value3((s32 (*)())Func_080d47b4, 13, 0x108, 30);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 5);
  Motion_SetModeAndWaitAnimation(13, 4);
  Event_ShowMessageThenWait(0x200d, 0, 5);
  Value3((s32 (*)())Func_080d47b4, 17, 0x109, 30);
  Event_ShowMessageThenWait(17, 0, 5);
  Motion_SetVarCbAndRefresh(7, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(7, 0, 5);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_NORTH, 0);
  Actor_ArmCallbackAndRefresh(4, FACING_NORTH);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(14, 4);
  Event_ShowMessageThenWait(14, 0, 5);
  Func_080d47b4(15, 0x102, 40);
  Actor_ArmCallbackAndRefresh(15, FACING_WEST);
  Event_Wait(10);
  Event_ShowMessageThenWait(15, 0, 5);
  Motion_SetModeAndWaitAnimation(13, 4);
  Actor_ArmCallbackAndRefresh(13, FACING_EAST);
  Event_Wait(10);
  Event_ShowMessageThenWait(13, 0, 5);
  Func_080d47b4(15, 0x101, 50);
  Motion_SetModeAndWaitAnimation(15, 4);
  Event_ShowMessageThenWait(15, 0, 5);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(13, 0, 5);
  Func_080d47b4(15, 0x100, 45);
  Actor_ArmCallbackAndRefresh(13, FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(5);
  Event_ShowMessageThenWait(13, 0, 5);
  Actor_ArmCallbackAndRefresh(15, FACING_SOUTH + FACING_STEP);
  Event_Wait(10);
  Value3((s32 (*)())Func_080d47b4, 15, 0x103, 30);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 15, 0);
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
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 15, FACING_WEST);
  Event_Wait(10);
  Value3((s32 (*)())Func_080d47b4, 15, 0x107, 30);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_ArmCallbackAndRefresh(13, FACING_EAST);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(13, 0, 5);
  Value3((s32 (*)())Func_080d47b4, 13, 0x109, 30);
  Event_ShowMessageThenWait(13, 0, 5);
  Motion_SetModeAndWaitAnimation(15, 4);
  Call3((void (*)())Actor_SetSpeedParameters, 15, 0x8000, 0x4000);
  v19 = Actor_GetById(15);
  v19->actionFlags &= ~1;
  Actor_CommitPositionAndActivate(15, 32, 0);
  Event_Wait(1);
  v20 = Actor_GetById(15);
  v20->actionFlags = 1 | (v20->actionFlags);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Actor_ArmCallback(14, FACING_EAST, 0);
  Actor_ArmCallback(13, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 17, FACING_NORTH + FACING_STEP, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_NORTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_NORTHEAST, 0);
  Actor_ArmCallbackAndRefresh(4, FACING_NORTHEAST);
  Event_Wait(5);
  Motion_SetVarCbAndRefresh(15, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(15, 0, 5);
  Motion_SetModeAndWaitAnimation(15, 4);
  Event_ShowMessageThenWait(15, 0, 5);
  Motion_SetVarCbAndRefresh(7, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 5);
  Motion_SetModeAndWaitAnimation(15, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(15, 0, 5);
  Value3((s32 (*)())Func_080d47b4, 4, 0x102, 0);
  Value3((s32 (*)())Func_080d47b4, 5, 0x102, 0);
  Value3((s32 (*)())Func_080d47b4, 6, 0x102, 0);
  Value3((s32 (*)())Func_080d47b4, 17, 0x102, 0);
  Value3((s32 (*)())Func_080d47b4, 7, 0x102, 60);
  Value3((s32 (*)())Func_080d47b4, 15, 0x108, 30);
  Event_ShowMessageThenWait(15, 0, 5);
  Motion_SetVarCbAndRefresh(15, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(15, 0, 5);
  Call3((void (*)())Actor_SetSpeedParameters, 15, 0x13333, 0x9999);
  Call3((void (*)())Actor_CommitPositionAndActivate, 15, -50, 24);
  Actor_WatchAndSetScript(13, 15);
  Actor_WatchAndSetScript(14, 15);
  Actor_WatchAndSetScript(7, 15);
  Actor_WatchAndSetScript(4, 15);
  Actor_WatchAndSetScript(5, 15);
  Actor_WatchAndSetScript(6, 15);
  Actor_WatchAndSetScript(17, 15);
  Camera_FollowActor(15, 1);
  Call3((void (*)())Actor_CommitPositionAndActivate, 15, -8, 32);
  Call3((void (*)())Actor_ArmCallback, 16, FACING_SOUTH + FACING_STEP, 0);
  Actor_StartRepeatedMotion(4, 3);
  Call2((void (*)())Func_080d489c, 4, 0x102);
  Actor_StartRepeatedMotion(5, 3);
  Func_080d489c(5, 0x102);
  Event_Wait(37);
  Call3((void (*)())Actor_SetSpeedParameters, 4, 0x23333, 0x11999);
  Call3((void (*)())Actor_SetSpeedParameters, 5, 0x23333, 0x11999);
  Call3((void (*)())Actor_OffsetPositionAndReset, 5, -8, 16);
  Actor_CommitPositionAndActivate(4, 8, 16);
  Actor_WaitForMoveById(5);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_WEST, 0);
  Actor_ArmCallbackAndRefresh(5, FACING_EAST);
  Event_Wait(15);
  Actor_WatchAndSetScript(4, 15);
  Actor_WatchAndSetScript(5, 15);
  Actor_CommitPositionAndActivate(15, 0, 64);
  Event_Wait(15);
  Actor_ArmCallbackAndRefresh(15, FACING_NORTH + FACING_STEP);
  Event_Wait(10);
  Value3((s32 (*)())Func_080d47b4, 15, 0x107, 40);
  Event_ShowMessageThenWait(15, 0, 5);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 15,
        FACING_SOUTH + FACING_STEP);
  Event_Wait(10);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 16, FACING_WEST);
  Event_Wait(15);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 16,
        FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(15);
  Actor_SetModeById(16, 5);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Actor_CommitPositionAndActivate(15, 0, 96);
  Event_Wait(20);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 4, 0x10000, 0x8000);
  Call3((void (*)())Actor_SetSpeedParameters, 5, 0x10000, 0x8000);
  Call3((void (*)())Actor_OffsetPositionAndReset, 5, 8, -16);
  Actor_CommitPositionAndActivate(4, -8, -16);
  Actor_WaitForMoveById(5);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 14, FACING_SOUTHEAST + FACING_STEP, 0);
  Call3((void (*)())Actor_ArmCallback, 13, FACING_SOUTHEAST + FACING_STEP, 0);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_NORTH, 0);
  Camera_MoveWithinMapBounds(PIXELS(872), -1, PIXELS(480), 1);
  Func_020020ac();
  Event_Wait(10);
  Actor_Stop(16);
  Actor_SetModeById(16, 1);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 17,
        FACING_NORTHWEST + FACING_STEP);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 5);
  Motion_SetModeAndWaitAnimation(13, 4);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Motion_SetModeAndWaitAnimation(13, 4);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 14, FACING_WEST, 0);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_WEST, 0);
  Call3((void (*)())Actor_ArmCallback, 17, FACING_WEST, 0);
  Actor_ArmCallbackAndRefresh(4, FACING_EAST);
  Event_Wait(40);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 14, FACING_SOUTHEAST + FACING_STEP, 0);
  Call3((void (*)())Actor_ArmCallback, 13, FACING_SOUTH + FACING_STEP, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 17, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_ArmCallbackAndRefresh(4, FACING_NORTH);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(13, 2);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Motion_SetVarCbAndRefresh(6, 2);
  Event_ShowMessageThenWait(6, 0, 5);
  Value3((s32 (*)())Func_080d47b4, 13, 0x108, 30);
  Event_ShowMessageThenWait(0x200d, 0, 5);
  Actor_ArmCallbackAndRefresh(13, FACING_WEST);
  Event_Wait(20);
  Actor_ArmCallbackAndRefresh(14, FACING_EAST);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_Wait(20);
  Call3((void (*)())Actor_ArmCallback, 13, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_ArmCallbackAndRefresh(14, FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(5);
  Actor_CommitPositionAndActivate(14, 24, 26);
  Actor_CommitPositionAndActivate(14, 0, 8);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(14, 2);
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
  Value3((s32 (*)())Func_080d47b4, 5, 0x101, 0);
  Value3((s32 (*)())Func_080d47b4, 17, 0x101, 0);
  Value3((s32 (*)())Func_080d47b4, 6, 0x101, 0);
  Value3((s32 (*)())Func_080d47b4, 4, 0x101, 60);
  Call3((void (*)())Actor_CommitPositionAndActivate, 14, -24, -26);
  Actor_CommitPositionAndActivate(14, 0, -8);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 14,
        FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(14, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_WEST, 0);
  Call3((void (*)())Actor_ArmCallback, 17, FACING_WEST, 0);
  Actor_ArmCallbackAndRefresh(4, FACING_EAST);
  Event_Wait(30);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 17, FACING_NORTHWEST + FACING_STEP, 0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 4, FACING_NORTH);
  Event_Wait(15);
  Motion_SetVarCbAndRefresh(13, 2);
  Event_Wait(10);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Actor_ArmCallbackAndRefresh(7, FACING_EAST);
  Event_Wait(10);
  Value3((s32 (*)())Func_080d47b4, 7, 0x101, 30);
  Event_ShowMessageThenWait(7, 0, 5);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 13, FACING_WEST);
  Motion_SetModeAndWaitAnimation(13, 4);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTHEAST, 0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 13,
        FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(5);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_ArmCallback(14, FACING_EAST, 0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 13, FACING_WEST);
  Event_Wait(30);
  Actor_SetModeById(14, 3);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(20);
  Call3((void (*)())Actor_ArmCallback, 14, FACING_SOUTHEAST + FACING_STEP, 0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 13,
        FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Value3((s32 (*)())Func_080d47b4, 13, 0x102, 30);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(10);
  Call3((void (*)())Event_ShowMessageThenWait, 0x200d, 0, 5);
  Motion_SetVarCbAndRefresh(14, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_WEST, 0);
  Call3((void (*)())Actor_ArmCallback, 17, FACING_WEST, 0);
  Actor_ArmCallbackAndRefresh(4, FACING_EAST);
  Event_Wait(30);
  Actor_SetModeById(4, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_Wait(10);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 17, FACING_NORTHWEST + FACING_STEP, 0);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 4, FACING_NORTH);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(13, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x200d, 0, 10);
  Actor_ArmCallbackAndRefresh(13, FACING_NORTH + FACING_STEP);
  Event_Wait(5);
  Call3((void (*)())Actor_SetPositionAndReset, 13, 864, 392);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 13,
        FACING_SOUTHEAST + FACING_STEP);
  Motion_SetVarCbAndRefresh(14, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(14, 0, 5);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 14, FACING_WEST);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 14,
        FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(14, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Call3((void (*)())Actor_ArmCallback, 17, FACING_WEST, 0);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_WEST, 0);
  Actor_ArmCallbackAndRefresh(4, FACING_EAST);
  Event_Wait(20);
  Actor_SetModeById(4, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_Wait(10);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTH, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 17, 0x13333, 0x9999);
  Actor_SetModeById(17, 2);
  {
    s32 party = (s32)&gMainRoutineWork;

    r5_5 = party + 532;
  }
  v24 = Actor_GetById(FIELD_AT(r5_5, s32, 0));
  if (v24 != 0) {
    Actor_ResetAndSetPosition(17, ((s16 *)&v24->x)[1], ((s16 *)&v24->z)[1]);
  }
  Actor_WaitForMoveById(17);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 5, 0x13333, 0x9999);
  Actor_SetModeById(5, 2);
  v25 = Actor_GetById(FIELD_AT(r5_5, s32, 0));
  if (v25 != 0) {
    Actor_ResetAndSetPosition(5, ((s16 *)&v25->x)[1], ((s16 *)&v25->z)[1]);
  }
  Actor_WaitForMoveById(5);
  Actor_SetHorizontalPositionWithTerrain(5, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 6, 0x13333, 0x9999);
  Actor_SetModeById(6, 2);
  v26 = Actor_GetById(FIELD_AT(r5_5, s32, 0));
  if (v26 != 0) {
    Actor_ResetAndSetPosition(6, ((s16 *)&v26->x)[1], ((s16 *)&v26->z)[1]);
  }
  Actor_WaitForMoveById(6);
  Actor_SetHorizontalPositionWithTerrain(6, 0, 0);
  Event_Wait(10);
  Call3((void (*)())Actor_CommitPositionAndActivate, 4, -32, -48);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTH, 0);
  Call3((void (*)())Actor_ArmCallback, 14, FACING_SOUTH + FACING_STEP, 0);
  Actor_ArmCallbackAndRefresh(4, FACING_NORTH);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_Wait(10);
  Call3((void (*)())Actor_SetSpeedParameters, 7, 0x13333, 0x9999);
  Actor_SetModeById(7, 2);
  v27 = Actor_GetById(FIELD_AT(r5_5, s32, 0));
  if (v27 != 0) {
    Actor_ResetAndSetPosition(7, ((s16 *)&v27->x)[1], ((s16 *)&v27->z)[1]);
  }
  Actor_WaitForMoveById(7);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_Wait(15);
  Call3((void (*)())Actor_SetPositionAndReset, 14, 840, 408);
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 14,
        FACING_SOUTHEAST + FACING_STEP);
  Call3((void (*)())Actor_ArmCallback, 16, FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(10);
  v29 = Func_02001fd4__s03a0();
  r0 = v29;
L_02009f20:
  return r0;
}


extern s16 gData_0200233c[];

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

s32 gData_02002368[] = {
    (s32)gData_0200233c,
    0x170005,
    (s32)gData_0200233c,
    0x17001e,
};

s32 gData_02002378[] = {
    (s32)gData_0200233c,
    0x40038,
};

struct Trigger gSceneEvents[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_020000c8)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_020000c8)},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {1, 7, CONDITION_ALWAYS, 7},
    {1, 8, CONDITION_ALWAYS, 8},
    {1, 9, CONDITION_ALWAYS, 9},
    {1, 10, CONDITION_ALWAYS, 10},
    {1, 11, CONDITION_ALWAYS, 11},
    {1, 12, CONDITION_ALWAYS, 12},
    {1, 13, CONDITION_ALWAYS, 13},
    {1, 14, CONDITION_ALWAYS, 14},
    {1, 15, CONDITION_ALWAYS, 15},
    {0xc602, 16, CONDITION_ALWAYS, EVENT(Func_020000c8)},
    {1, 17, CONDITION_ALWAYS, 17},
    {2, 20, 0x96a, EVENT(Func_0200058c)},
    {0, 8, 0x196a, MSGNO(MsgLemuriaPalace12)},
    {0, 9, 0x196a, MSGNO(MsgLemuriaPalace13)},
    {0, 10, 0x196a, MSGNO(MsgLemuriaPalace14)},
    {0, 11, 0x196a, MSGNO(MsgLemuriaPalace15)},
    {0, 12, 0x196a, MSGNO(MsgLemuriaPalace16)},
    {0x8d15, 8, 0x196a, MSGNO(MsgLemuriaPalace17)},
    {0x8d15, 9, 0x196a, MSGNO(MsgLemuriaPalace18)},
    {0x8d15, 10, 0x196a, MSGNO(MsgLemuriaPalace19)},
    {0x8d15, 11, 0x196a, MSGNO(MsgLemuriaPalace20)},
    {0x8d15, 12, 0x196a, MSGNO(MsgLemuriaPalace21)},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace01)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace02)},
    {0, 10, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace03)},
    {0, 11, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace04)},
    {0, 12, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace05)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace06)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace07)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace08)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace09)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace10)},
    {0, 13, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace22)},
    {0, 14, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace23)},
    {0, 16, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace24)},
    {0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace25)},
    {0x8d15, 14, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace26)},
    {0x8d15, 16, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace27)},
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
