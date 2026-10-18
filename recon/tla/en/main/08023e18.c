// Draft, differ 26 of 93: draws the field actors for map modes 3 and 4:
// copies the frame decoder into heap slot 80, takes the camera eye and
// target (or their overrides), sets the depth from the direction between
// them, applies the look-at (with the fixed tilt gData_0802eb98 while game
// flag 363 is set) and draws the 64 actor slots from the last. No ☀️ twin
// found. Left: the native has a 12-byte frame nothing here explains, stores
// the halfword 0 from movs r3, #0 where agscc loads it from the pool (a
// halfword constant store), and places its literal pool after the function
// instead of after the first call (branch offsets).
#include "types.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"
#include "graphics.h"
#include "transform.h"
#include "iwram_call.h"
#include "ram_buffer.h"

struct Camera {
  u8 unk00[12];
  s32 target[3];
  s32 *eyeOverride;
  s32 *targetOverride;
};

struct ActorSlot {
  u8 unk00[2];
  u16 active;
  u8 unk04[124];
};

extern u8 Render_DecodeFrameCode[];
extern u8 Render_DecodeFrameCopyEnd[];
extern u8 Render_DecodeFrameCodeSize[];
extern const s32 gData_0802eb98[];

s32 ArcTan2(s32, s32);
s32 GameFlag_Test(s32 flag);
void Func_08023d80(struct ActorSlot *actor, s32 depth);

void Func_08023e18(void) {
  struct Camera *camera = Ram_HeapSlots->cameraWork;
  s16 *angle = (s16 *)Ram_HeapSlots->objectSystem;
  s32 *eye;
  s32 *target;
  s32 depth;
  struct ActorSlot *actor;
  s32 i;

  Dma_Set(Render_DecodeFrameCode, Runtime_AllocateHeapBlock(80, (s32)Render_DecodeFrameCodeSize),
          0x84000000 |
              ((u32)(Render_DecodeFrameCopyEnd - Render_DecodeFrameCode) >> 2),
          REG_DMA3);
  eye = (s32 *)camera;
  target = camera->target;
  if (camera->eyeOverride != 0)
    eye = camera->eyeOverride;
  if (camera->targetOverride != 0)
    target = camera->targetOverride;
  depth = (s16)ArcTan2((eye[0] - target[0]) >> 16, (eye[2] - target[2]) >> 16);
  *angle = 0;
  if (GameFlag_Test(363)) {
    Render_ResetTransformState();
    depth += -0x2000;
    Iwram_TransformMatrix(gData_0802eb98);
    Transform_ApplyLookAt(eye, target);
  } else {
    SceneTransform_RewindStack();
    Transform_SetLookAt(eye, target);
  }
  actor = (struct ActorSlot *)((u8 *)Ram_HeapSlots->scriptObjects + 0x1f80);
  for (i = 63; i >= 0; i--) {
    if (actor->active != 0)
      Func_08023d80(actor, depth);
    actor--;
  }
  Runtime_ReleaseHeapBlock(80);
}
