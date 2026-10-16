//--------------------------------------------------------------------------
// object.c
// Objects: create and release field objects, and render them each frame.
//--------------------------------------------------------------------------

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "system.h"
#include "mem.h"
#include "edition.h"
#include "objdisp.h"
#include "resource.h"
#include "metadata_lookup.h"
#include "object_runtime.h"
#include "object_dispatch.h"
#include "io_reg.h"
#include "dma.h"
#include "ramcode.h"
#include "battle_system.h"
#include "transform.h"

#if GAME_TLA
#include "ram_buffer.h"
#else
#include "types.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "global_cells.h"
#include "map_scroll.h"
#include "script.h"
#include "animspr.h"
#include "field_sprite.h"
#include "field_obj.h"
#include "script_object_runtime.h"
#include "script_motion.h"
#include "vram_block.h"
#include "iwram_call.h"

extern struct ObjectRuntime *gObjectSlots;

extern struct ObjectSystemWork *gMenuCtrlWork;

void Object_SetPositionAndResetMotion(struct ObjectRuntime *object, s32 x,
                                      s32 y, s32 z);
s32 AnimationObjects_SelectAnimation(struct AnimationObject *, s32);
s32 ResourceMetadata_Register(struct AnimationObject *state, s32 id);

s32 BattleFx_ApplyColorToTargetBufferFar(s32, s32);
s32 BattleFx_StartBufferInterpolationFar(s32);

//--------------------------------------------------------------------------
// One step of erasing graphics pixel by pixel.
// Erase one pixel per 8x8, 256-color tile according to the erase order table.
// Offset pos by one per tile; erase only tiles whose pos is 64-127.
// obj: graphics (AnimationObject).
// pos: iteration index (0-127).
//--------------------------------------------------------------------------
void Map_RenderAnimatedTileFrame(u8 *obj, u32 pos) {
  u16 *dst;
  s32 cnt;
  u32 n;
  u8 *table;
  u32 highMask;
  u32 indexMask;
  u8 offsetMask;

  struct AnimationObject *spr = (struct AnimationObject *)obj;

  dst = (u16 *)(0x06010000 + gVramBlockCache[spr->slot].offset); // OBJ VRAM.
  cnt = (spr->width * spr->height) / 64;
  n = 0;

  if (n < cnt) {
    table = gMap_TileDissolveOrder;
    highMask = 0xff00;
    indexMask = 0x3f;
    offsetMask = 0x3e;

    for (; n < cnt; n++, dst += 32, pos++) {
      if ((pos - 64) <= 63) {
        u8 dot;
        u16 *cell;
        u32 odd;

        // VRAM cannot accept byte writes; read two pixels and erase one.
        dot = table[(pos + n * 16) & indexMask];
        cell = (u16 *)((u8 *)dst + (dot & offsetMask));
        odd = 1;
        odd &= dot;
        if (odd != 0)
          *cell = *(u8 *)cell;
        else
          *cell &= highMask;
      }
    }
  }
}

//--------------------------------------------------------------------------
// Erase graphics pixel by pixel: four steps per frame, finishing in 32 frames.
// obj: graphics (AnimationObject).
//--------------------------------------------------------------------------
void Map_RenderAnimatedTileFramesForObject(u8 *obj) {
  u32 pos;

  pos = 0;
  do {
    Map_RenderAnimatedTileFrame(obj, pos);
    Map_RenderAnimatedTileFrame(obj, pos + 1);
    Map_RenderAnimatedTileFrame(obj, pos + 2);
    Map_RenderAnimatedTileFrame(obj, pos + 3);
    pos += 4;
    WaitFrames(1);
  } while (pos <= 127);
}

//--------------------------------------------------------------------------
// Erase several graphics together, pixel by pixel, over 32 frames.
// table: graphics list.
// cnt: graphics count.
//--------------------------------------------------------------------------
void Map_RenderAllAnimatedTileFrames(u8 **table, s32 cnt) {
  u8 **top;
  u8 **p;
  s32 n;
  u32 pos;
  u32 pos1;
  u32 pos2;
  u32 pos3;

  top = table;
  pos = 0;
  do {
    if (cnt > 0) {
      pos1 = pos + 1;
      pos2 = pos + 2;
      p = top;
      pos3 = pos + 3;
      n = cnt;
      do {
        Map_RenderAnimatedTileFrame(*p, pos);
        Map_RenderAnimatedTileFrame(*p, pos1);
        Map_RenderAnimatedTileFrame(*p, pos2);
        n -= 1;
        Map_RenderAnimatedTileFrame(*p++, pos3);
      } while (n != 0);
    }
    WaitFrames(1U);
    pos += 4;
  } while (pos <= 0x7fU);
}

//--------------------------------------------------------------------------
// Return the object's screen position in pixels in out[0] and out[1].
// Return 0 near the screen; outside it, return -1 and write (0, 0).
// obj: object.
// out: x and y.
//--------------------------------------------------------------------------
s32 Map_GetScreenRelativePosition(struct ObjectRuntime *obj, s32 *out) {
  struct MapScrollWork *map = gMapWork[0];
  s32 *org = &map->viewX;
  s32 ox;
  s32 oz;
  s32 x;
  s32 y;

  ox = org[0] & 0xffff0000;
  oz = org[1] & 0xffff0000;
  x = obj->x - ox;
  y = obj->z - oz;
  if ((x >= -0x1fffff && x <= 0x10fffff) && y > 0 && y < 0xe00000) {
    *out++ = x >> 16;
    *out = y >> 16;
    return 0;
  }
  *out++ = 0;
  *out = 0;
  return -1;
}

//--------------------------------------------------------------------------
// Initialize the object system.
// Allocate and zero 64 objects, then register the per-frame callback.
// mode: 4 uses Object_UpdateAllMotion; 3 and 4 use the fixed camera.
//--------------------------------------------------------------------------
void ObjectSystem_Initialize(s32 mode) {
  struct ObjectSystemWork *sys;
  struct ObjectRuntime *objs;
  volatile u32 fill;

  sys = Runtime_AllocateBlock(6, sizeof(*sys));
  objs = Runtime_AllocateBlock(5, 64 * sizeof(*objs));
  ObjectSystem_Configure(mode);
  fill = 0;
  Dma_Set((const void *)&fill, objs, 0x85000700, REG_DMA3); // Fill with words.
  fill = 0;
  Dma_Set((const void *)&fill, sys, 0x85000017, REG_DMA3);
  if (mode == 4)
    Scheduler_AddOrUpdateCallback(Object_UpdateAllMotion, 0xc8a);
  else
    Scheduler_AddOrUpdateCallback(Object_UpdateAllThumb, 0xc8a);
  if (mode >= 3 && mode <= 4) {
    Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCameraFixed, 0xc80);
  } else {
    Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCamera, 0xc80);
    gIwram_Data2 = 0;
    gIwram_Data1 = 0;
  }
  sys->edge = 15;
  sys->fill = 0;
}

//--------------------------------------------------------------------------
// Do nothing.
//--------------------------------------------------------------------------
void ResourceTable_ReservedNoOpC0C4(void) {}

//--------------------------------------------------------------------------
// Do nothing.
//--------------------------------------------------------------------------
void ResourceTable_ReservedNoOp2(void) {}

#endif

//--------------------------------------------------------------------------
// Return an unused object (one without a script) from the 64 slots.
// Return 0 if none are free.
//--------------------------------------------------------------------------
void *ObjectDispatch_FindFreeObject(void) {
#if GAME_TLA
  struct ObjectRuntime *obj = Ram_HeapSlots->scriptObjects;
#else
  struct ObjectRuntime *obj = gObjectSlots;
#endif
  void *ret = 0;
  s32 n = 0;

  while (n <= 63) {
    if (obj->script == 0) {
      ret = obj;
      break;
    }
    n++;
    obj++;
  }
  return ret;
}

//--------------------------------------------------------------------------
// Release the object's graphics and zero the object to free it.
// obj: object.
//--------------------------------------------------------------------------
void ObjectDispatch_Release(struct DispatchObject *obj) {
  volatile u32 zero;
  volatile u32 *src;
  s32 n;
  struct ResourceObjectWork **child;

  if (obj) {
    switch (obj->kind & 15) {
    case 1: // One graphic.
      ResourceObject_Release((struct ResourceObjectWork *)obj->target.child);
      break;
    case 2: // Up to four graphics.
      child = (struct ResourceObjectWork **)obj->target.children;
      n = 3;
      do {
        struct ResourceObjectWork *res = *child++;
        if (res)
          ResourceObject_Release(res);
      } while (--n >= 0);
      break;
    }
    src = &zero;
    *src = 0;
    Dma_Set(src, obj, 0x85000000 | (sizeof(struct ObjectRuntime) / 4),
            REG_DMA3);
  }
}

#if !GAME_TLA
//--------------------------------------------------------------------------
// Create a field object; return 0 if no slot is free.
// id: quotient by 4096 is the type (0: one graphic, 2: two); low 12 bits select
// graphics. x, y, z: 16.16 fixed-point position.
//--------------------------------------------------------------------------
struct ObjectRuntime *FieldObject_Create(s32 id, s32 x, s32 y, s32 z) {
  struct ObjectRuntime *obj;
  struct FieldActor *act;
  struct ScriptMotionObject *mot;
  struct ScriptObjectRuntime *sc;
  void *spr;
  s32 kind;
  void **ent;
  volatile u32 zero;

  ObjectDispatch_FindFreeObject();
  kind = id / 4096;
  id &= 0xfff;
  obj = ObjectDispatch_FindFreeObject();
  if (obj != NULL) {
    act = (struct FieldActor *)obj;
    act->radius = 16;
    switch (kind) {
    case 0:
      spr = ResourceObject_Create(id);
      if (spr != NULL) {
        obj->animationKind = 1;
        obj->animation = spr;
        act->radius = Resource_GetMetadataRecordFar(id)->boxY >> 1;
      } else {
        obj->animationKind = 0;
      }
      break;
    case 2:
      // Allocate and zero four graphics slots, then insert graphics id and id
      // + 1.
      ent = (void **)((u32 *)gMenuCtrlWork + gMenuCtrlWork->count++ + 2);
      obj->animationKind = kind;
      zero = 0;
      obj->animation = ent;
      Dma_Set(&zero, ent, 0x85000004, REG_DMA3);
      spr = ResourceObject_Create(id);
      if (spr != NULL) {
        act->radius = Resource_GetMetadataRecordFar(id)->boxY >> 1;
        *ent++ = spr;
      }
      spr = ResourceObject_Create(id + 1);
      if (spr != NULL)
        *ent = spr;
      break;
    }
  }
  if (obj != NULL) {
    mot = (struct ScriptMotionObject *)obj;
    sc = (struct ScriptObjectRuntime *)obj;
    Object_SetPositionAndResetMotion(obj, x, y, z);
    obj->script = (s32 *)gObjectDispatch_DefaultScript;
    obj->speedLimit = 0x20000;
    obj->step = 0;
    act->scaleX = 0x10000;
    act->scaleY = 0x10000;
    obj->acceleration = 0x10000;
    obj->flags = 3;
    mot->gravity = 0x10000;
    mot->vertical.bounce = 0x4000;
    obj->unk59 = 0;
    obj->actionFlags = 1;
    act->riseBaseY = 0;
    obj->angle = 0x4000;
    sc->homeX = x / 65536; // Home position in pixels.
    sc->homeZ = z / 65536;
  }
  return obj;
}

//--------------------------------------------------------------------------
// Assign an object's script and start it from the beginning.
// obj: object.
// script: script.
//--------------------------------------------------------------------------
void ObjectDispatch_Initialize(struct DispatchObject *obj, const s32 *script) {
  if (obj != 0) {
    obj->value04 = 0;
    obj->value00 = (u32)script;
    obj->value5b = 0;
    obj->value5d = 0;
    obj->value57 = 0;
  }
}

//--------------------------------------------------------------------------
// Set animation no on all of the object's graphics.
//--------------------------------------------------------------------------
void ObjectDispatch_ApplyArgumentToChildren(void *raw, s32 no) {
  struct DispatchObject *obj;
  void **list;
  s32 n;
  void *spr;

  obj = raw;
  if (obj != 0) {
    switch (obj->kind & 0xf) {
    case 1:
      AnimationObjects_SelectAnimation(obj->target.child, no);
      break;
    case 2:
      list = obj->target.children;
      n = 3;
      do {
        spr = *list++;
        if (spr != 0)
          AnimationObjects_SelectAnimation(spr, no);
        n--;
      } while (n >= 0);
      break;
    }
  }
}

//--------------------------------------------------------------------------
// Call AnimationObjects_SetActiveTimerSteps for all of the object's graphics.
//--------------------------------------------------------------------------
void ObjectDispatch_ApplyValueToChildren(struct DispatchObject *obj, s32 val) {
  s32 n;
  void *spr;
  void **list;

  if (obj != 0) {
    switch (obj->kind & 0xf) {
    case 1:
      AnimationObjects_SetActiveTimerSteps(obj->target.child, val);
      return;
    case 2:
      list = obj->target.children;
      n = 3;
      do {
        spr = *list++;
        if (spr != 0)
          AnimationObjects_SetActiveTimerSteps(spr, val);
        n--;
      } while (n >= 0);
      break;
    }
  }
}

//--------------------------------------------------------------------------
// Combine the previous two operations: set animation no and pass steps.
//--------------------------------------------------------------------------
void ObjectDispatch_ApplyPairToChildren(struct DispatchObject *obj, s32 no,
                                        s32 steps) {
  void **list;
  void *spr;
  s32 n;

  if (obj != 0) {
    switch (obj->kind & 15) {
    case 1:
      AnimationObjects_SelectAnimation(obj->target.child, no);
      AnimationObjects_SetActiveTimerSteps(obj->target.child, steps);
      break;
    case 2:
      list = obj->target.children;
      for (n = 3; n >= 0; n--) {
        spr = *list++;
        if (spr != 0) {
          AnimationObjects_SelectAnimation(spr, no);
          AnimationObjects_SetActiveTimerSteps(spr, steps);
        }
      }
      break;
    }
  }
}

//--------------------------------------------------------------------------
// Set rotation when the object has a single graphic.
//--------------------------------------------------------------------------
void ObjectDispatch_SetChildRotation(struct DispatchObject *obj, u32 rot) {
  if (obj != 0 && (obj->kind & 0xf) == 1)
    ((struct FieldSprite *)obj->target.child)->rotation = rot;
}

//--------------------------------------------------------------------------
// Set animation no and restart it when the object has a single graphic.
// Do nothing if no is negative.
//--------------------------------------------------------------------------
void Animation_SetIndexAndInitObjects(void *raw, s32 no) {
  struct DispatchObject *obj = raw;
  struct AnimationObject *spr;

  if (obj != NULL && (obj->kind & 0xf) == 1) {
    spr = obj->target.child;
    if (no >= 0) {
      spr->entries[0]->animId = no;
      Animation_InitializeObjects(spr);
    }
  }
}

//--------------------------------------------------------------------------
// Register no's information when the object has a single graphic.
// Do nothing if no is negative.
//--------------------------------------------------------------------------
void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *obj, s32 no) {
  if (obj != 0 && (obj->kind & 0xf) == 1) {
    struct AnimationObject *spr = obj->target.child;
    if (no >= 0)
      ResourceMetadata_Register(spr, no);
  }
}

//--------------------------------------------------------------------------
// Start the object with a predefined script.
//--------------------------------------------------------------------------
void ObjectDispatch_InitFromTable5WithArgument(struct DispatchObject *obj,
                                               s32 arg) {
  ObjectDispatch_Initialize(obj, gObjectDispatch_Table5Script);
  obj->argument = arg;
}

void ObjectDispatch_InitFromTable0(struct DispatchObject *obj) {
  ObjectDispatch_Initialize(obj, gObjectDispatch_Table0Script);
}

void ObjectDispatch_InitFromTable1(struct DispatchObject *obj) {
  ObjectDispatch_Initialize(obj, gObjectDispatch_Table1Script);
}

void ObjectDispatch_InitFromTable2(struct DispatchObject *obj) {
  ObjectDispatch_Initialize(obj, gObjectDispatch_Table2Script);
}

void ObjectDispatch_InitFromTable3(struct DispatchObject *obj) {
  ObjectDispatch_Initialize(obj, gObjectDispatch_Table3Script);
}

void ObjectDispatch_InitFromTable6(struct DispatchObject *obj) {
  ObjectDispatch_Initialize(obj, gObjectDispatch_Table6Script);
}

void ObjectDispatch_InitFromTable4WithArgument(struct DispatchObject *obj,
                                               s32 arg) {
  ObjectDispatch_Initialize(obj, gObjectDispatch_Table4Script);
  if (arg != 0) {
    obj->value34 = 0x8000;
    obj->value30 = 0x40000;
    obj->argument = arg;
    obj->value64 = 0;
  }
}

//--------------------------------------------------------------------------
// Wait up to 300 frames for script command 0x10.
//--------------------------------------------------------------------------
void ObjectDispatch_WaitForValue16(void *raw) {
  struct ScriptInterpreter *sc = raw;
  s32 cnt;

  cnt = 0;
  if (sc->script[sc->cursor] != 0x10) {
    do {
      WaitFrames(1);
      cnt++;
      if (cnt > 299)
        break;
    } while (sc->script[sc->cursor] != 0x10);
  }
}

//--------------------------------------------------------------------------
// Set graphics flags when the object has a single graphic.
//--------------------------------------------------------------------------
void ObjectDispatch_SetSingleChildFlags(struct DispatchObject *obj, u32 flags) {
  if (obj != NULL) {
    if ((obj->kind & 0xf) == 1)
      ((struct AnimationObject *)obj->target.child)->flags = flags;
  }
}

//--------------------------------------------------------------------------
// Set graphics translucency mode when the object has a single graphic.
//--------------------------------------------------------------------------
void ObjectDispatch_SetChildBlendMode(struct DispatchObject *obj, u32 mode) {
  if (obj != 0 && obj->kind == 1) {
    struct FieldSprite *spr = obj->target.child;
    spr->blendMode = mode;
  }
}

//--------------------------------------------------------------------------
// Set graphics displayFlags bit 1 when the object has a single graphic.
//--------------------------------------------------------------------------
void ObjectDispatch_SetChildDisplayFlag1(struct DispatchObject *obj, u32 on) {
  if (obj != 0 && obj->kind == 1) {
    struct FieldSprite *spr = obj->target.child;
    spr->unk1d1 = on;
  }
}

//--------------------------------------------------------------------------
// Call ObjectGroup_SetChildValueUnlessFifteen for a single-graphic object.
//--------------------------------------------------------------------------
void Animation_ApplyChildValues(struct DispatchObject *obj, u32 val) {
  if (obj != NULL && obj->kind == 1)
    ObjectGroup_SetChildValueUnlessFifteen(obj->target.child, val);
}

//--------------------------------------------------------------------------
// Start camera updates, hide BG1-3, and show OBJ.
//--------------------------------------------------------------------------
void Graphics_EnableObjLayerAndCallbacks(void) {
  Scheduler_SuspendTasks(ObjectSystem_UpdateCamera);
  Scheduler_SuspendTasks(ObjectSystem_UpdateCameraFixed);
  BattleFx_ApplyColorToTargetBufferFar(0x10000, 1);
  BattleFx_StartBufferInterpolationFar(1);
  WaitFrames(1);
  REG_DISPCNT = (REG_DISPCNT & 0xf1ff) | DISPCNT_OBJ_ON;
}

//--------------------------------------------------------------------------
// Stop camera updates and hide BG1-3 and OBJ.
//--------------------------------------------------------------------------
void ObjectDispatch_StopCallbacksAndHideLayers(void) {
  Scheduler_ResumeTasks(ObjectSystem_UpdateCamera);
  Scheduler_ResumeTasks(ObjectSystem_UpdateCameraFixed);
  REG_DISPCNT &= 0xe1ff;
}

s32 ObjectDispatch_ReturnTrue(void) { return 1; }

// Map cell.
struct CameraTile {
  u32 unk00 : 12;
  u32 layer : 2; // If nonzero, the layer of objects above this cell plus one.
  u32 priority
      : 2; // If nonzero, the display priority of objects above this cell.
  u32 unk10 : 16;
};

// Object graphics, including OAM attributes.
struct CameraSprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 flipX : 1;
  u16 flipY : 1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[0x0a];
  u16 secondTile : 10;
  u16 secondPriority : 2;
  u16 secondPalette : 4;
  u8 unk16[2];
  s32 scale;
  u8 resource; // VRAM block index.
  u8 flags1d;
  u8 unk1e[7];
  u8 activated;
};

// Object.
struct CameraObject {
  u32 active;
  u16 unk04;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  s32 height;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[2];
  u8 layer;
  u8 flags;
  u8 unk24[0x2c];
  void *sprite;
  u8 kind; // 1: one graphic; 2: up to four graphics.
  u8 unk55[7];
  u8 held;
  u8 unk5d[0x13];
};

struct CameraLayer {
  struct CameraTile *tiles;
  u8 unk04[44];
};

// Map.
struct CameraState {
  u8 unk000[0xe4];
  s32 x; // Viewing position.
  s32 z;
  u8 unk0ec[0x44];
  struct CameraLayer layers[1];
};

struct CameraSync {
  s16 count;
  s16 unk02;
  s16 frozen;
};

struct CameraPoint {
  s32 x;
  s32 y;
  s32 z;
};

// Fixed camera.
struct FixedCamera {
  struct CameraPoint eye;
  struct CameraPoint target;
  struct CameraPoint *eyeOverride;    // Use this eye position when present.
  struct CameraPoint *targetOverride; // Use this look-at target when present.
};

extern u8 Render_DecodeFrame[];

s32 ArcTan2(s32 x, s32 y);
void Render_ApplyProjectedPlacement(void *sprite, s32 *position, s32 *scale,
                                    u16 angle);
void Render_PlaceProjectedSprite(void *sprite, s32 *position, s32 *scale,
                                 s32 angle, s32 layer);

//--------------------------------------------------------------------------
// Per frame: position object graphics on screen with the normal camera.
// For objects near the screen, choose layer and priority from the cell beneath
// them. Release graphics VRAM blocks for objects outside that range.
//--------------------------------------------------------------------------
void ObjectSystem_UpdateCamera(void) {
  /* FAKEMATCH: needed to match: unused; the native frame keeps room for three
   * points above the position. */
  struct CameraPoint spare[3];
  s32 cnt;
  struct CameraState *map;
  s32 camX;
  s32 camZ;
  struct CameraSync *sync;
  struct CameraObject *obj;
  struct CameraSprite *spr;
  s32 *cam;
  u32 kind;
  s32 dx;
  s32 dz;
  s32 top;
  struct CameraTile *tile;
  u32 bits;
  s32 scale[2];
  s32 pos[4];
  s32 y;
  s32 height;
  u32 bitsVal;

  map = *(struct CameraState **)(((u8 *)&gObjectSlots) + 12);
  cam = &map->x;
  camX = cam[0] & 0xffff0000;
  camZ = cam[1] & 0xffff0000;
  sync = *(struct CameraSync **)(((u8 *)&gObjectSlots) + 4);
  // Copy the rendering routine (Render_DecodeFrame) into the workspace.
  RamCode_LoadSlot(52, Render_DecodeFrame, Render_DecodeFrameCodeSize);
  sync->count = 0;
  obj = *(struct CameraObject **)((u8 *)&gObjectSlots);
  for (cnt = 0; cnt < 64; cnt++, obj++) {
    if (obj->active == 0)
      continue;
    if (obj->x != 0 || obj->z != 0) {
      kind = obj->kind & 15;
      if (kind == 0)
        continue;
      if (kind != 1)
        continue;
      if (sync->frozen != 0 && obj->held == 0) {
        struct CameraSprite *frozen = obj->sprite;

        VramCache_ReleaseVram(frozen->resource);
        frozen->activated = kind;
        continue;
      }
      dx = obj->x - camX;
      dz = obj->z - camZ;
      top = dz - obj->y;
      spr = obj->sprite;
      if (dx > -0x200000 && dx < 0x1100000 && top > -0x200000 &&
          top < 0xe00000) {
        // Cell information beneath the object.
        tile = &map->layers[obj->layer]
                    .tiles[(obj->x >> 20) + ((obj->z >> 20) << 7)];
        if (obj->flags & 1) {
          bits = tile->priority;
          if (bits != 0) {
            spr->priority = bits;
            spr->secondPriority = bits;
          }
        }
        bitsVal = tile->layer;
        if (bitsVal != 0)
          obj->layer = bitsVal - 1;
        scale[0] = Iwram_MulQ16(obj->scaleX, spr->scale);
        scale[1] = Iwram_MulQ16(obj->scaleY, spr->scale);
        pos[0] = dx;
        pos[1] = obj->y;
        pos[2] = dz;
        pos[3] = obj->height;
        if (obj->flags & 2) {
          pos[1] += 0xfec00000; // -20 pixels.
          pos[2] += 0xfec00000;
          pos[3] += 0xfec00000;
        }
        if (obj->flags & 4) {
          pos[1] += 0x1400000; // +20 pixels.
          pos[2] += 0x1400000;
          pos[3] += 0x1400000;
        }
        Render_ApplyProjectedPlacement(spr, pos, scale, obj->angle);
        continue;
      }
      if (obj->held == 0) {
        if (!(spr->flags1d & 1)) {
          VramCache_ReleaseVram(spr->resource);
          spr->activated = 1;
        }
      }
    } else {
      kind = obj->kind & 15;
      if (kind == 1) {
        spr = obj->sprite;
        if (obj->held == 0 && !(spr->flags1d & 1)) {
          VramCache_ReleaseVram(spr->resource);
          spr->activated = kind;
        }
      }
    }
  }
  Runtime_ReleaseHeapBlock(52);
}

s32 ObjectCamera_ReturnTrue(void) { return 1; }

//--------------------------------------------------------------------------
// Per frame: position object graphics on screen with the fixed camera.
// Derive orientation from eye and target, then place all 64 objects in reverse.
// While selecting commands (flag 363), use a fixed-orientation matrix
// and scale 1.
//--------------------------------------------------------------------------
void ObjectSystem_UpdateCameraFixed(void) {
  s32 cnt;
  struct FixedCamera *cam;
  struct CameraSync *sync;
  struct CameraPoint *eye;
  struct CameraPoint *target;
  struct CameraObject *obj;
  s32 angle;
  s32 part;
  s32 kind;
  s32 *pos;
  s32 one;
  void **parts;
  void *spr;
  s32 scale2[2];
  s32 scale[2];
  /* FAKEMATCH: needed to match: unused; the native frame keeps room for two
   * points above the scales. */
  struct CameraPoint spare[2];

  cam = *(struct FixedCamera **)(((u8 *)&gObjectSlots) + 0x1c);
  sync = *(struct CameraSync **)(((u8 *)&gObjectSlots) + 4);
  RamCode_LoadSlot(52, Render_DecodeFrame, Render_DecodeFrameCodeSize);
  eye = &cam->eye;
  target = &cam->target;
  if (cam->eyeOverride != NULL)
    eye = cam->eyeOverride;
  if (cam->targetOverride != NULL)
    target = cam->targetOverride;
  angle = (s16)ArcTan2((eye->x - target->x) >> 16, (eye->z - target->z) >> 16);
  sync->count = 0;
  Render_ResetTransformState();
  if (GameFlag_TestFar(363)) {
    angle += 0xffffe000;
    Iwram_TransformMatrix(gCamera_FixedViewMatrix);
    Transform_ApplyLookAt(eye, target);
  } else {
    Transform_SetLookAt(eye, target);
  }
  obj = *(struct CameraObject **)((u8 *)&gObjectSlots);
  obj += 63;
  one = 0x10000;
  for (cnt = 63; cnt >= 0; cnt--, obj--) {
    if (obj->active == 0)
      continue;
    kind = obj->kind & 15;
    switch (kind) {
    case 1:
      pos = &obj->x;
      scale[0] = obj->scaleX;
      scale[1] = obj->scaleY;
      spr = obj->sprite;
      if (GameFlag_TestFar(363)) {
        scale[0] = one;
        scale[1] = one;
      }
      Render_PlaceProjectedSprite(spr, pos, scale, obj->angle + angle,
                                  obj->layer);
      break;
    case 2:
      scale2[0] = obj->scaleX;
      scale2[1] = obj->scaleY;
      if (GameFlag_TestFar(363)) {
        scale2[0] = one;
        scale2[1] = one;
      }
      parts = obj->sprite;
      for (part = 3; part >= 0; part--) {
        spr = *parts++;
        if (spr != NULL)
          Render_PlaceProjectedSprite(spr, &obj->x, scale2, obj->angle + angle,
                                      obj->layer);
      }
      break;
    case 0:
      break;
    }
  }
  Runtime_ReleaseHeapBlock(52);
}

#endif
