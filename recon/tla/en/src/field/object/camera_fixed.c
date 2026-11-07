#include "types.h"
#include "ram_buffer.h"
#include "sprite.h"
#include "io_reg.h"
#include "dma.h"
#include "iwram_call.h"
#include "graphics.h"
#include "heap_release.h"
#include "transform.h"

// The fixed camera: each frame it places every object's sprites, facing them
// by the angle from the eye to the target. The Lost Age places each object in
// a function of its own, the body of ☀️'s ObjectSystem_UpdateCameraFixed
// loop.

struct CameraPoint {
  s32 x;
  s32 y;
  s32 z;
};

struct FixedCamera {
  struct CameraPoint eye;
  struct CameraPoint target;
  struct CameraPoint *eyeOverride;    ///< when set, used as the eye
  struct CameraPoint *targetOverride; ///< when set, used as the target
};

struct PlacedObject {
  u8 unk00[2];
  u16 active;
  u8 unk04[2];
  u16 angle;
  s32 position[4];
  s32 scale[2];
  u8 unk20[2];
  u8 layer;
  u8 flags;
  u8 unk24[0x2c];
  void *target;
  u8 kind;
  u8 unk55[0x2b];
};

// The decoder's block: its code, tables and work area (constants.ld). Only
// the code and tables are copied.
extern u8 Render_DecodePictureCode[];
extern u8 Render_DecodePictureCopyEnd[];
extern u8 Render_DecodePictureBlockSize[];
extern const s32 gData_0802eb90[2];
extern const s32 gData_0802eb98[12];
s32 ArcTan2(s32 x, s32 y);
s32 GameFlag_Test(s32 flag);
void Func_0802254c(void *sprite, s32 *point, const s32 *scale, s32 facing,
                   s32 depth);

s32 ObjectCamera_ReturnTrue(void) { return 1; }

/// Places an object's sprite (kind 1, unless flag 0x10 hides it) or each of its
/// group's four sprites (kind 2), turned by angle. While battle commands are
/// chosen (flag 363) the sprites are drawn at 1.0 scale.
void Func_08023d80(struct PlacedObject *object, s32 angle) {
  const s32 *scale = object->scale;
  s32 *point;
  void *sprite;
  void **sprites;
  s32 remaining;

  if (GameFlag_Test(363))
    scale = gData_0802eb90;
  switch (object->kind & 15) {
  case 1:
    point = object->position;
    sprite = object->target;
    if (!(object->flags & 0x10))
      Func_0802254c(sprite, point, scale, object->angle + angle, object->layer);
    break;
  case 2:
    sprites = object->target;
    for (remaining = 3; remaining >= 0; remaining--) {
      sprite = *sprites++;
      if (sprite != NULL)
        Func_0802254c(sprite, object->position, scale, object->angle + angle,
                      object->layer);
    }
    break;
  case 0:
    break;
  }
}

/// Each frame, with the fixed camera: places every active object, from the
/// 64th down. While battle commands are chosen (flag 363) it uses the fixed
/// view matrix and turns the objects by an eighth of a turn less.
void ObjectSystem_UpdateCameraFixed(void) {
  struct FixedCamera *camera = Ram_HeapSlots->cameraWork;
  struct ObjectSystemWork *system = Ram_HeapSlots->objectSystem;
  struct CameraPoint *eye;
  struct CameraPoint *target;
  struct PlacedObject *object;
  s32 angle;
  s32 i;
  /* FAKEMATCH(dead): an unused point for the stack frame's 12 bytes; as no local,
   * 12 of 93 instructions differ in tla-en. */
  struct CameraPoint unused;

  /* FAKEMATCH(one-pass): the decoder's copy in a once-only block, which
   * makes the stack frame before the first call's arguments; as a plain call, 6 of
   * 93 instructions differ in tla-en. */
  do {
    Dma_Set(
        Render_DecodePictureCode,
        HeapSlot_Alloc(80, (s32)Render_DecodePictureBlockSize),
        DMA_COPY_WORDS(
            (u32)(Render_DecodePictureCopyEnd - Render_DecodePictureCode) >> 2),
        REG_DMA3);
  } while (0);
  eye = &camera->eye;
  target = &camera->target;
  if (camera->eyeOverride != NULL)
    eye = camera->eyeOverride;
  if (camera->targetOverride != NULL)
    target = camera->targetOverride;
  angle = (s16)ArcTan2((eye->x - target->x) >> 16, (eye->z - target->z) >> 16);
  system->used = 0;
  if (GameFlag_Test(363)) {
    Render_ResetTransformState();
    angle += 0xffffe000;
    Iwram_TransformMatrixEntry(gData_0802eb98);
    Transform_ApplyLookAt((s32 *)eye, (s32 *)target);
  } else {
    SceneTransform_RewindStack();
    Transform_SetLookAt((s32 *)eye, (s32 *)target);
  }
  object = Ram_HeapSlots->scriptObjects;
  object += 63;
  for (i = 63; i >= 0; i--, object--) {
    if (object->active != 0)
      Func_08023d80(object, angle);
  }
  HeapSlot_Release(80);
}
