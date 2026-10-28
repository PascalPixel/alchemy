// Near miss, differ 32 of 376 (56 before, 2026-10-26: the affine identity,
// REG_BG3Y, REG_BLDALPHA and REG_DISPCNT written with Io_Write16 and the
// scroll cleared layer by layer, as the twin does; 250 before that): The Broken
// Seal's Map_InitializePerspectiveScene on this game's map work (0x978 bytes,
// heap slot offset 32) and affine work (offset 28), with the world map's update
// (0802c240) and affine frame (0802c088) callbacks. Resource ids 0x197 to
// 0x199: PerspectiveDataA and DefaultMapCells as ☀️ names them, the third a
// placeholder. The ARM routine is copied by its linker size
// (Data_0802146cSize, to define in constants.ld as Render_ConvertMapCode -
// Data_0802146c when this adopts), the halfword constants the native builds
// in registers go through int stores (Io_Set16, Store16), and the frame
// fields are at 0x968 and 0x96c. Left: scheduling: the native clears the eye
// offset's x and y after the pitch call's load, two pool loads a row apart,
// and builds 0x106 afresh where this adds 2 to 0x104.
#include "types.h"
#include "io_write.h"
#include "dma.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "map.h"
#include "mem.h"
#include "resource.h"
#include "ram_code.h"
#include "iwram_call.h"
#include "bg_scroll.h"
#include "frame.h"

extern char ResourceId_PerspectiveDataA;
extern char ResourceId_DefaultMapCells;
extern char ResourceId_Data199;
extern u8 gMapCellBuffer[];
extern u8 gData_0202e000[];
extern u32 gCameraSceneParameters[];
extern const u8 Data_0802146c[];
extern u8 Data_0802146cSize[];
extern u32 gData_03001144;
extern u32 Data_03001244;

void Fade_FromBlack(s32 vblanks);
s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);
void Projection_Set(s32 distance, s32 nearPlane, s32 farPlane);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(void *position);
void SceneTransform_ApplyYaw(s32 yaw);
void SceneTransform_ApplyPitch(s32 pitch);
void Transform_SetLookAt(s32 *eye, s32 *target);
void Func_0802dd70(s32 ratio, s32 *position, void *affineWork);
s32 Task_Add(void (*func)(void), s32 order);
void Func_0802c240(void);
void Func_0802c088(void);

static __inline__ void Io_Set16(s32 value, volatile u16 *reg) { *reg = value; }
static __inline__ void Store16(s32 value, u16 *field) { *field = value; }

struct EyeOffset {
  s32 x, y, z;
};

void Func_0802b998(void) {
  u8 *perspective;
  u8 *camera;
  u8 *affineWork;
  u8 *affinePages;
  s32 *cameraPosition;
  s32 *distance;
  u16 *yaw;
  u16 *pitch;
  volatile u32 fill;
  struct EyeOffset eyeOffset;
  s32 farPlane;
  s32 tileIndex;

  REG_DISPCNT &= 0xc1ff;
  Fade_FromBlack(0);
  perspective = HeapSlot_Alloc(32, 0x978);
  Dma_Fill32Cell(fill, 0, perspective, 0x978);
  *(s32 *)(perspective + 0xe4) = 0;
  *(s32 *)(perspective + 0xe8) = 0;
  *(s32 *)(perspective + 0xec) = 0x200000;
  *(s32 *)(perspective + 0xf0) = 0x400000;
  *(s32 *)(perspective + 0xf4) = 0x3fe00000;
  *(s32 *)(perspective + 0xf8) = 0x3fe00000;
  *(u8 **)(perspective + 0x10) = NULL;
  *(void **)(perspective + 0x114) =
      Resource_GetTableEntry((s32)&ResourceId_PerspectiveDataA);
  Io_Set16(0x3f9e, &REG_BLDCNT);
  Io_Write16(0x1010, &REG_BLDALPHA);
  REG_BLDY = 0;
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), gMapCellBuffer);
  Store16(0x1f00, (u16 *)(perspective + 0x14));
  perspective[0x16] = 0x80;
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Data199),
                        gData_0202e000);
  Io_Set16(0xa80a, &REG_BG3CNT);
  Io_Set16(0xaa0e, &REG_BG2CNT);
  Io_Set16(0x8501, &REG_BG1CNT);
  Io_Write16(0x100, &REG_BG2PA);
  REG_BG2PB = 0;
  REG_BG2PC = 0;
  Io_Write16(0x100, &REG_BG2PD);
  REG_BG2X = 0;
  REG_BG2Y = 0;
  Io_Write16(0x100, &REG_BG3PA);
  REG_BG3PB = 0;
  REG_BG3PC = 0;
  Io_Write16(0x100, &REG_BG3PD);
  REG_BG3X = 0;
  Io_Write16(0, &REG_BG3Y);

  camera = HeapSlot_AllocEwram(48, 76);
  affineWork = HeapSlot_Alloc(28, 0x3484);
  affinePages = affineWork + 0xc80;
  farPlane = 0x1fe0000;
  *(s32 *)(perspective + 0x95c) = farPlane;
  distance = (s32 *)(perspective + 0x960);
  *distance = farPlane;
  *(s32 *)(perspective + 0x968) = 0x10000;
  cameraPosition = (s32 *)(camera + 12);
  *(u16 *)(perspective + 0x96c) = 0;
  *(s32 *)(camera + 24) = 0;
  *(s32 *)(camera + 28) = 0;
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 96;
  Projection_Set(farPlane, farPlane >> 1, farPlane << 1);
  cameraPosition[0] = 0;
  cameraPosition[1] = 0;
  cameraPosition[2] = 0;
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(cameraPosition);
  yaw = (u16 *)(perspective + 0x11e);
  SceneTransform_ApplyYaw(*yaw);
  pitch = (u16 *)(perspective + 0x11c);
  eyeOffset.x = 0;
  eyeOffset.y = 0;
  SceneTransform_ApplyPitch(*pitch);
  eyeOffset.z = farPlane;
  Iwram_TransformVector(&eyeOffset, (s32 *)camera);
  Render_ResetTransformState();
  Transform_SetLookAt((s32 *)camera, cameraPosition);
  RamCode_LoadSlot(104, Data_0802146c, Data_0802146cSize);
  Func_0802dd70(Iwram_RatioMulQ14(Trig_Cos(*pitch), Trig_Sin(*pitch)),
                cameraPosition, affineWork);
  Data_03001244 = 0;
  gData_03001144 = *pitch;
  ((void (*)(void *, s32 *, void *, void *))Ram_HeapSlots->unk64[1])(
      camera, cameraPosition, affineWork,
      affinePages + (gFrameCount & 1) * 0x1400);
  cameraPosition[0] = 0;
  cameraPosition[1] = 0;
  cameraPosition[2] = 0;
  Render_ResetTransformState();
  Io_Set16(0xe000, pitch);
  *yaw = 0;
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(cameraPosition);
  SceneTransform_ApplyYaw(*yaw);
  SceneTransform_ApplyPitch(*pitch);
  if (*(s8 *)(perspective + 0x976) == 0) {
    eyeOffset.x = 0;
    eyeOffset.y = 0;
    eyeOffset.z = *distance + 0x10000;
    Iwram_TransformVector(&eyeOffset, (s32 *)camera);
  }
  REG_MOSAIC = 0;
  Io_Write16(0x42, &REG_DISPCNT);
  gBgScroll[1].x = 0;
  gBgScroll[1].y = 0;
  gBgScroll[2].x = 0;
  gBgScroll[2].y = 0;
  gBgScroll[3].x = 0;
  gBgScroll[3].y = 0;
  *(u16 *)(perspective + 0x104) = 0;
  Store16(159, (u16 *)(perspective + 0x106));
  Task_Add(Func_0802c240, 0x485);
  Task_Add(Func_0802c088, 0xc80);
  for (tileIndex = 0; tileIndex <= 0x3ff; tileIndex++)
    ((u16 *)(perspective + 0x148))[tileIndex] = tileIndex;
}
