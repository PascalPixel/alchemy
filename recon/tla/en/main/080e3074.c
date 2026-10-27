// Draft, differ 449 of 713: the native keeps the work in r9, the owner in r10
// and the canvas in r8 (here sl, r8 and others) and builds the palette ramp's
// three terms before its loop; the rest follows from that allocation.
// A field Psynergy that whirls the effect owner up in a
// canvas-drawn whirlwind and carries it to the place Func_080e2264 found.
// The heap slot 0x5c work (0x1b38 bytes) is shared with Func_080e2ec0,
// Func_080e2d64, Func_080e2acc and Func_080e249c. The owner spins
// (BattleEffect_SpinObject) with its angle kept in its action field; OBJ
// palette 14 becomes a ramp of pale blues; the canvas sprite (256 colours,
// drawn as left, 1.5 times its size) sits on the owner while object 280 grows
// under it for 20 frames (cue 207); then for 60 frames four random rings a
// frame are drawn into the canvas by Func_080e2a68 at a spread that grows by
// a step that shrinks to 123/128 each frame, while the canvas turns 0x400 a
// frame. What follows depends on what Func_080e2264 found: a kind-3 cell
// (0xd40 of the event work) or a kind-2 cell (0xd3c) carries the owner there
// through Func_080e2ec0 or Func_080e2d64 until the work's state parks at 999;
// a kind-1 cell waits for the work's count at 0x1b08 to empty; with none the
// owner just spins on. Each ends with cue 136, object 280 destroyed, the
// owner's update cleared and its angle restored.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "canvas_sprite.h"

struct WhirlWork {
  u8 unk0000[0x1b08];
  s16 count;
  u8 unk1b0a[2];
  struct ObjectRuntime *follower;
  s16 state;
  s16 timer;
  s32 angle;
  s32 radius;
  s32 from[3];
  s32 to[3];
  s8 kind;
};

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void BattleEffect_InitializeSharedScene(void);
void Func_080e2264(void);
void BattleEffect_SetObjectTickMode(struct ObjectRuntime *object, s32 mode);
void BattleEffect_SpinObject(void);
void Camera_WorldToScreen(s32 *position);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void WaitTicks(s32 ticks);
void Audio_PlayCue(s32 cue);
u32 Random16(void);
void Func_080e2a68(s32 radius, s32 angle, u16 *offsets, u8 *pixels);
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080e2acc(void);
void Func_080e249c(void);
void Func_080e2ec0(void);
void Func_080e2d64(void);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x,
                                         s32 y, s32 z);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           s32 flags);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Scheduler_RemoveCallback(void (*callback)(void));
void Object_Destroy(struct ObjectRuntime *object);
void BattleEffect_EndX(void);

void Func_080e3074(void) {
  struct WhirlWork *work = Runtime_AllocateHeapBlock(92, 0x1b38);
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *effect = Ram_HeapSlots->effectWork;
  struct ObjectRuntime *owner = *(struct ObjectRuntime **)(effect + 16);
  s16 *points = (s16 *)(event + 0xcc0);
  s32 spread = 0x40000;
  s32 step = 0xe000;
  struct CanvasSpriteWork *canvas;
  struct ObjectRuntime *ring;
  u16 *offsets;
  u8 *pixels;
  u16 *palette;
  s32 pos[3];
  s32 mode;
  s32 frame;
  s32 i, j;

  BattleEffect_InitializeSharedScene();
  Func_080e2264();
  BattleEffect_SetObjectTickMode(*(struct ObjectRuntime **)(effect + 16), 0);
  owner->update = BattleEffect_SpinObject;
  owner->action = owner->angle;
  palette = (u16 *)0x050003c0;
  for (i = 0; i < 16; i++)
    *palette++ = ((i + 16) << 5) | (31 << 10) | (i + 16);
  Func_080eb824();
  canvas = Ram_HeapSlots->unkec[1];
  pos[0] = (*(struct ObjectRuntime **)(effect + 16))->x;
  pos[1] = (*(struct ObjectRuntime **)(effect + 16))->y;
  pos[2] = (*(struct ObjectRuntime **)(effect + 16))->z;
  Camera_WorldToScreen(pos);
  canvas->x = (s16)(pos[0] >> 16);
  canvas->y = (s16)(pos[2] >> 16);
  canvas->priority = 1;
  canvas->depth = 240;
  canvas->mode = CANVAS_SPRITE_KEPT;
  canvas->scaleX = 0x18000;
  canvas->scaleY = 0x18000;
  Func_080eaf98(&canvas->sprite, 32, 32, 0xc0000000, canvas->tile);
  canvas->sprite.oam.palette256 = 1;
  canvas->sprite.oam.palette = 0;
  offsets = Ram_HeapSlots->canvasMap;
  pixels = Ram_HeapSlots->battleFxWork[1];
  ring = Object_Spawn(280, owner->x, owner->y - 0x60000, owner->z);
  Object_SetMode(ring, 2);
  ring->scaleX = 0x18000;
  ring->scaleY = 0;
  WaitTicks(1);
  Audio_PlayCue(207);
  for (i = 0; i < 20; i++) {
    ring->scaleY = (i << 16) / 20;
    WaitTicks(1);
  }
  for (i = 0; i < 60; i++) {
    for (j = 0; j < 4; j++)
      Func_080e2a68((s32)(spread + Random16() * 4 - (j << 16)) >> 16,
                    Random16(), offsets, pixels);
    spread += step;
    if (spread > 0x1c0000)
      spread = 0x1c0000;
    step = step * 123 / 128;
    canvas->angle += 0x400;
    canvas->command = 3;
    WaitTicks(1);
  }
  BattleEffect_PrepareBufferInterpolation();
  work->follower = ring;
  work->from[0] = owner->x;
  work->from[1] = owner->y;
  work->from[2] = owner->z;
  work->to[0] = owner->x;
  work->to[1] = owner->y;
  work->to[2] = owner->z;
  work->angle = 0;
  work->state = 0;
  work->timer = 0;
  work->state = 0;
  if (*(s16 *)(event + 0xd40) != -1)
    mode = 3;
  else if (*(s16 *)(event + 0xd3c) != -1)
    mode = 2;
  else
    mode = points[0] != -1;

  if (mode == 3) {
    if (work->kind)
      Func_080e2acc();
    for (i = 0; i < 16; i++) {
      canvas->angle += 0x400;
      canvas->command = 3;
      WaitTicks(1);
    }
    Camera_MoveWithinMapBounds(-1, -1, -1, 0);
    Object_SetPositionAndResetMotionFar(owner, owner->x, owner->y, owner->z);
    ObjectDispatch_SetSingleChildFlagsFar(owner, 0);
    frame = 0;
    ObjectMotion_SetActionVariant(*(s16 *)(effect + 24), 1);
    owner->motionFlags = 0;
    WaitTicks(1);
    work->to[0] = *(s16 *)(event + 0xd40) << 16;
    work->to[1] = owner->y + 0x200000;
    work->to[2] = *(s16 *)(event + 0xd42) << 16;
    Scheduler_AddOrUpdateCallback(Func_080e2ec0, 0x480);
    while (work->state != 999) {
      if (frame == 30)
        Func_080e249c();
      WaitTicks(1);
      frame++;
    }
    Scheduler_RemoveCallback(Func_080e2ec0);
    if (*(s8 *)(effect + 52) == 0)
      owner->scaleX = 0;
    ring->scaleX = 0;
    if (work->kind)
      BattleEffect_EndX();
  } else if (mode == 2) {
    if (work->kind)
      Func_080e2acc();
    for (i = 0; i < 16; i++) {
      canvas->angle += 0x400;
      canvas->command = 3;
      WaitTicks(1);
    }
    Camera_MoveWithinMapBounds(-1, -1, -1, 0);
    Object_SetPositionAndResetMotionFar(owner, owner->x, owner->y, owner->z);
    ObjectDispatch_SetSingleChildFlagsFar(owner, 0);
    owner->motionFlags = 0;
    WaitTicks(1);
    work->to[0] = *(s16 *)(event + 0xd3c) << 16;
    work->to[1] = owner->y;
    work->to[2] = *(s16 *)(event + 0xd3e) << 16;
    Scheduler_AddOrUpdateCallback(Func_080e2d64, 0x480);
    frame = 0;
    while (work->state != 999) {
      if (frame == 30)
        Func_080e249c();
      WaitTicks(1);
      frame++;
    }
    Scheduler_RemoveCallback(Func_080e2d64);
    for (i = 0; i < 16; i++) {
      owner->scaleX += -0x800;
      owner->scaleY = owner->scaleX;
      WaitTicks(1);
    }
    owner->scaleX = 0;
    for (i = 0; i < 32; i++) {
      ring->scaleX += -0x800;
      ring->scaleY += -0x200;
      WaitTicks(1);
    }
    ring->scaleX = 0;
    if (work->kind)
      BattleEffect_EndX();
  } else if (mode == 1) {
    Audio_PlayCue(136);
    Object_SetMode(ring, 6);
    if (work->kind)
      Func_080e2acc();
    for (i = 0; i < 16; i++) {
      canvas->angle += 0x400;
      canvas->command = 3;
      WaitTicks(1);
    }
    owner->update = NULL;
    owner->angle = owner->action;
    frame = 0;
    while (work->count != 0) {
      if (frame == 30)
        Func_080e249c();
      WaitTicks(1);
      frame++;
    }
    if (work->kind)
      BattleEffect_EndX();
  } else {
    Audio_PlayCue(136);
    Object_SetMode(ring, 6);
    for (i = 0; i < 16; i++) {
      owner->angle += 0x1000;
      canvas->angle += 0x400;
      canvas->command = 3;
      WaitTicks(1);
    }
  }
  Audio_PlayCue(136);
  Object_Destroy(ring);
  owner->update = NULL;
  owner->angle = owner->action;
  BattleEffect_EndCanvasSprite();
  Runtime_ReleaseHeapBlock(92);
}
