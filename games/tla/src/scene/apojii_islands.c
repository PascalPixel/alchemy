/// @file
/// The scene code of overlay 0x679 that its scene tables belong to, in ROM order; the rest of the overlay is in apojii_islands/.

#include "apojii_islands/local.h"
#include "types.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "call.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "system/io_reg.h"
#include "event_inline.h"

void Func_0200033c(void) { Func_080d7788(10, 3, 11); }

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_02002200[];
extern const u8 gData_02002218[];
extern const u8 gData_02002320[];
extern const u8 gData_02002458[];

const u8 *Func_02000360(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov679Room1)
    return gData_02002218;
  if (scene == (s32)&SceneId_Ov679Room3)
    return gData_02002320;
  if (scene == (s32)&SceneId_Ov679Room2)
    return gData_02002458;
  return gData_02002200;
}

void Func_020003b0(void) {
  Actor_Get(gMainRoutineWork.currentOwner)->terrainId = 2;
}

void Func_020003cc(void) {
  Actor_Get(gMainRoutineWork.currentOwner)->terrainId = 0;
}

extern s32 gData_020024c4;
extern u16 Data_02002958[], Data_02002978[], gData_02002998[], gData_020029b8[];

void Func_020003e8(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 phase, k, offset;
  u16 *firstCopy, *secondCopy;

  if (*(s16 *)(work + 0x166) != 0)
    return;
  if (*(s8 *)(work + 0xca4) != 0)
    return;
  if (gData_020024c4 == -1) {
    Call3((void (*)())Iwram_CopyWords, Data_02002958, (void *)0x05000060, 32);
    Iwram_CopyWords(Data_02002978, Data_02002958, 32);
    Iwram_CopyWords(gData_02002998, (void *)0x05000080, 32);
    Iwram_CopyWords(gData_020029b8, gData_02002998, 32);
    gData_020024c4 = 0;
  }
  phase = gData_020024c4 / 8 % 4;
  k = 0;
  firstCopy = Data_02002978;
  secondCopy = gData_020029b8;
  for (; k <= 3; k++) {
    offset = (phase + k) % 4 * 2 + 24;
    *(u16 *)((u8 *)firstCopy + offset) = Data_02002958[12 + k];
    *(u16 *)((u8 *)secondCopy + offset) = gData_02002998[12 + k];
  }
  IO_WRITE_QUEUE_PUSH(0x05000060, (u32)firstCopy, 0x84000008);
  IO_WRITE_QUEUE_PUSH(0x05000080, (u32)secondCopy, 0x84000008);
  gData_020024c4++;
}

extern s32 gData_020024c8;
extern u16 Data_020029d8[], Data_020029f8[], Data_02002a18[], Data_02002a38[];

void Func_02000540(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 busy, phase, k, j;

  if (*(s16 *)(work + 0x166) != 0)
    return;
  busy = *(s8 *)(work + 0xca4);
  if (busy != 0)
    return;
  if (gData_020024c8 == -1) {
    Call3((void (*)())Iwram_CopyWords, (s32)Data_020029d8, 0x05000060, 32);
    Iwram_CopyWords(Data_020029f8, Data_020029d8, 32);
    Iwram_CopyWords(Data_02002a18, (void *)0x050001a0, 32);
    Iwram_CopyWords(Data_02002a38, Data_02002a18, 32);
    gData_020024c8 = busy;
  }
  phase = gData_020024c8 / 8 % 4;
  for (k = 0; k <= 3; k++) {
    /* FAKEMATCH(dead): the phase and the first colour in a once-only block,
     * whose end holds the order around it; as plain statements, 6 of 152
     * instructions differ in tla-en. */
    do {
      j = (phase + k) % 4;
      Data_020029f8[12 + j] = Data_020029d8[12 + k];
    } while (0);
    Data_02002a38[12 + j] = Data_02002a18[12 + k];
  }
  IO_WRITE_QUEUE_PUSH(0x05000060, (u32)Data_020029f8, 0x84000008);
  IO_WRITE_QUEUE_PUSH(0x050001a0, (u32)Data_02002a38, 0x84000008);
  gData_020024c8++;
}

void Func_02000694(void) {
  Func_02001bf4(Func_02000540);
  System_Wait(3);
}

void Func_020006a8(void) {
  Func_02001bfc(Func_02000540);
  System_Wait(3);
}

extern volatile s32 gData_020024cc[];
extern u16 gData_02002a58[16];
extern u16 gData_02002a78[16];

s32 __modsi3(s32 a, s32 b);

void Func_020006bc(void) {
  s32 base;
  s32 i;

  if (gData_020024cc[0] == -1) {
    Iwram_CopyWords(gData_02002a58, (void *)0x05000080, 32);
    gData_020024cc[0] = 0;
  }
  base = (gData_020024cc[0] / 2) % 15;
  for (i = 0; i < 15; i++)
    gData_02002a78[(base + i) % 15 + 1] = gData_02002a58[i + 1];
  IO_WRITE_QUEUE_PUSH(0x050003c0, (u32)gData_02002a78, 0x84000008);
  gData_020024cc[0]++;
}

void Func_02000770(struct Actor *object) {
  Animation_ApplyChildValues(object, 0);
  object->update = 0;
}

void Func_02000780(struct Actor *object) {
  if (gFrameCount & 2)
    Animation_ApplyChildValues(object, 7);
  else
    Animation_ApplyChildValues(object, 0);
}
