#include "types.h"
#include "event_inline.h"

struct TreasureIsleCell {
  s32 x;
  s32 z;
  s32 unk08;
  u16 flag;
  u16 unk0e;
};

extern struct TreasureIsleCell gData_0200117c[];
extern struct TreasureIsleCell gData_020011cc[];

void Func_02000880(s32 unused, s32 frame, s32 id) {
  struct TreasureIsleCell *cells;

  if (id < 50) {
    cells = gData_0200117c;
    id -= 40;
  } else {
    cells = gData_020011cc;
    id -= 50;
  }
  if (frame == 15) {
    MapGrid_CopyCellsLayered(3, 100, 3, 4, cells[id].x + 63, cells[id].z - 2);
  } else if (frame == 30) {
    MapGrid_CopyCellsLayered(6, 100, 3, 4, cells[id].x + 63, cells[id].z - 2);
  } else if (frame == 45) {
    MapGrid_CopyCellsLayered(9, 100, 3, 4, cells[id].x + 63, cells[id].z - 2);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, cells[id].x, cells[id].z);
    Event_ClearFlag(cells[id].flag);
  }
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000914(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(9));
  FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
      ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
  FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
  Event_SetFlag(0x200);
}

void Func_02000940(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(10));
  FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
      ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
  FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
  Event_SetFlag(0x201);
}

void Func_02000970(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(11));
  FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
      ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
  FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
  Event_SetFlag(0x202);
}

void Func_020009a0(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(12));
  FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
      ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
  FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
  Event_SetFlag(0x203);
}

void Func_020009d0(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(13));
  FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
      ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
  FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
  Event_SetFlag(0x204);
}

void Func_020009fc(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(14));
  FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
      ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
  FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
  Event_SetFlag(0x205);
}

void Func_02000a2c(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(15));
  FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
      ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
  FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
  Event_SetFlag(0x206);
}

void Func_02000a5c(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(16));
  FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
      ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
  FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
  Event_SetFlag(0x207);
}

#include "call.h"
#include "main_routine_work.h"
void Func_02000fcc(void);
s32 Func_0200101c();
extern u8 MsgIAmTheGuardian[];

void Func_02000a8c(void) {
  s32 t12;

  Func_02000fcc();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_StartRepeatedMotion(8, 2);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_Interpolate(0x3c);
  FieldEvent_Wait(60);
  FieldMessage_ShowCentered((s32)MsgIAmTheGuardian, 0, 6);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(0x3c);
  FieldEvent_Wait(60);
  t12 = Actor_Get(gMainRoutineWork.currentOwner);
  if (t12 != 0) {
    Actor_SetDestination(8, FIELD_AT(t12, s16, 10), FIELD_AT(t12, s16, 18));
  }
  FieldEvent_Wait(20);
  gMainRoutineWork.pendingDjinnEvent = 0x19bb;
  gMainRoutineWork.battleRule24b = 2;
  Call2(Func_0200101c, 0x67, 0);
}

#include "scene_ids.h"

extern const u8 gData_020016e4[];
extern const u8 gData_02001780[];
extern const u8 gData_0200184c[];
extern const u8 gData_020018f4[];
extern const u8 gData_02001a5c[];

const u8 *Func_02000b3c(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6afRoom2)
    return gData_02001780;
  if (scene == (s32)&SceneId_Ov6afRoom3)
    return gData_0200184c;
  if (scene == (s32)&SceneId_Ov6afRoom4)
    return gData_020018f4;
  if (scene == (s32)&SceneId_Ov6afRoom5)
    return gData_02001a5c;
  return gData_020016e4;
}

#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"

struct TreasureIsleEventWork {
  u8 unk000[0x166];
  s16 unk166;
  u8 unk168[0xb3c];
  u8 unkca4;
};

extern s32 gData_02001aa4;
extern u16 Data_02001aa8[];
extern volatile u16 gData_02001ac8[];
extern volatile u16 Data_02001ae8[];
extern u16 Data_02001b08[];

void Func_02000ba0(void) {
  struct TreasureIsleEventWork *work = Ram_HeapSlots->eventWork;
  s32 phase;
  s32 i;
  s32 busy;

  if (work->unk166 != 0)
    return;
  busy = (s8)work->unkca4;
  if (busy != 0)
    return;
  if (gData_02001aa4 == -1) {
    Call3((void (*)())Iwram_CopyWords, Data_02001aa8, (void *)0x05000060, 32);
    Iwram_CopyWords((void *)gData_02001ac8, Data_02001aa8, 32);
    Iwram_CopyWords((void *)Data_02001ae8, (void *)0x05000140, 32);
    Iwram_CopyWords(Data_02001b08, (void *)Data_02001ae8, 32);
    gData_02001aa4 = 0;
  }
  /* FAKEMATCH(volatile): two of the colour arrays read and written as volatile,
   * which keeps the second read after the first store; as plain u16, 6 of 152
   * instructions differ in tla-en. */
  phase = gData_02001aa4 / 8 % 4;
  for (i = 0; i <= 3; i++) {
    gData_02001ac8[(phase + i) % 4 + 2] = Data_02001aa8[i + 2];
    Data_02001b08[(phase + i) % 4 + 2] = Data_02001ae8[i + 2];
  }
  IO_WRITE_QUEUE_PUSH(0x05000060, (u32)gData_02001ac8, 0x84000008);
  IO_WRITE_QUEUE_PUSH(0x05000140, (u32)Data_02001b08, 0x84000008);
  gData_02001aa4++;
}
