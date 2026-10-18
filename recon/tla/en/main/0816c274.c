// Draft of BattleFx_RunCrystal, decompiled from the listing: differ 310 of
// 491 (draft 491), same calls and control flow. Left: the frame is 100
// bytes for 104 (the native keeps one more spill slot), so the stack
// offsets shift, and the disc loop counter takes sl for r8.

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "mem.h"
#include "ram_buffer.h"

struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const void *pixels;
};

struct RenderModel {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct ModelTexture *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

struct BattleObjectPosition {
  u8 unk00[8];
  s32 x;
};

void BattleFx_BeginCanvasLayer(s32 bgControl);
void BattleFx_EndCanvasLayer(void); // BattleFx_EndCanvasLayer
void BattleFx_ProcessPendingGraphicsTransfer(void); // BattleFx_ProcessPendingGraphicsTransfer
void ObjectGroup_TickMemberTimers(void); // ObjectGroup_TickMemberTimers
void BattleFx_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleFx_LoadWork
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // ObjectGroup_UpdateMembers
void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void Func_0815585c(struct BattleEffectArgument *effect, s32 packed, s32 *screen,
                   s32 *otherPoint);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct RenderModel *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct RenderModel *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_ResetTransformState(void);
void Transform_SetLookAt(void *eye, void *target);
struct BattleObjectPosition **GetBattleObjectSlotFar(s32 objectId);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
void Audio_PlayCue(s32 cue);
void WaitFrames(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

extern char ResourceId_Data0FB;
extern char ResourceId_VenusDjinnSmallSheet;
extern char ResourceId_BlastSheet;
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern const u8 gData_08199364[]; // the crystal's mesh
extern const u8 gData_08199210[]; // its four corners
extern u32 gCameraSceneParameters[];

#define BLITTER ((DrawRectangle)Ram_HeapSlots->unk64[1])
#define CRYSTAL_FRAMES 103
#define CRYSTAL_SHARDS 32

// Abilities: Crystal.
//
// Sixteen shaded discs, 2 to 32 pixels across, are built in the map cell
// buffer. A pillar of light rises at the target for 20 frames while a
// textured crystal spins up over it from frame 16; from frame 48 thirty-two
// shards, one every two frames, drift up from around the target, pulsing in
// size and jittering until they settle, and the targets flash at frames 64,
// 68, ... before the scene fades out from frame 96.
void BattleFx_RunCrystal(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  u16 *discOffsets;
  DrawRectangle draw;
  void *camera;
  struct BattleObjectPosition *target;
  struct ModelTexture texture;
  struct EffectPosition screen;
  s32 other[3];
  s32 center[3];
  s32 frame;
  s32 i;
  s32 size;
  s32 offset;
  struct EffectStep *shard;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  discOffsets = (u16 *)(work->sheet + 0x2710);
  Iwram_ClearWords(gMapCellBuffer, 0x8000);
  for (i = 0, offset = 0, size = 2; i != 16; i++, size += 2) {
    Func_0815b434(gMapCellBuffer + offset, size, 0x10000);
    discOffsets[i] = offset;
    offset += (i * 2 + 3) * size / 2;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data0FB, work->sheet, 0, 0);
  Func_0816ae40(work->sheet, gData_02014000, 64, 64);
  BattleFx_BeginCanvasLayer(0);
  Func_0815585c(effect, ((effect->side ^ 1) << 4) | 32, center, other);
  Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet,
                             work->sheet + 0x65c0, 1, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_BlastSheet), 128);
  REG_BLDALPHA = 0x100e;
  if (effect->side == 1) {
    BattleFx_LoadBlitter(104, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
  } else {
    BattleFx_LoadBlitter(104, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  }
  draw = BLITTER;
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0xc80);
  camera = Ram_HeapSlots->cameraWork;
  target = *GetBattleObjectSlotFar(effect->actors[0]);

  for (frame = 0; frame != CRYSTAL_FRAMES; frame++) {
    if (frame == 0) {
      for (i = 0, shard = work->particles; i != CRYSTAL_SHARDS; i++, shard++) {
        s32 angle = Random16();
        s32 radius = (Random16() & 63) + 80;

        shard->x = target->x + ((Trig_Sin(angle) * radius) >> 1);
        shard->y = 0x80000;
        shard->z = (Trig_Cos(angle) * radius) >> 1;
        shard->velocityX = Random16();
        shard->velocityY = (Random16() & 0xffff) + 0x40000;
        shard->velocityZ = (Random16() & 7) + 4;
        shard->variant = -1;
      }
    }
    if (frame > 95) {
      REG_BLDALPHA = (0xce - frame * 2) | 0x1000;
    }
    Render_ResetTransformState();
    Transform_SetLookAt(camera, (u8 *)camera + 12);

    // The pillar of light.
    if (frame <= 19) {
      s32 x = ((Trig_Sin(frame << 11) * 8) >> 16) + center[0] / 2 - 10;
      s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + center[1] - 24 + frame * 3;

      draw(canvas, work->sheet + 0x65c0, x, y, 20, 40);
      if (frame <= 3) {
        draw(canvas, work->sheet + 0x65c0, x, y, 20, 40);
      }
    }

    // The shards, one more every two frames from frame 48.
    if (frame > 47) {
      for (i = 0, shard = work->particles; i != CRYSTAL_SHARDS; i++, shard++) {
        if (frame >= i * 2 + 48) {
          s32 width;

          EffectPosition_ApplyBaseAndYOffset(&shard->x, &screen);
          screen.x >>= 1;
          width = shard->velocityZ;
          width += width * Trig_Sin(shard->velocityX) / 0x20000;
          shard->velocityX += 0x400;
          width *= 2;
          if ((u32)(width - 2) <= 61) {
            s32 cell = (width - 2) / 2;
            s32 x = screen.x;
            s32 y = screen.y;

            if (shard->variant == 0) {
              x += (Random16() & 7) - 4;
              y += (Random16() & 7) - 4;
            }
            Func_0818caa8(gMapCellBuffer + (s16)discOffsets[cell], x, y, width);
          }
          shard->y += shard->velocityY;
        }
      }
    }

    for (i = 0; i != effect->count; i++) {
      if (frame == i * 4 + 64) {
        Audio_PlayCue(126);
        Func_0814cd48(effect->actors[i], 7, -1, i, 24);
      }
    }

    // The crystal, spinning and growing from frame 16.
    Render_ResetTransformState();
    gCameraSceneParameters[4] = 104;
    {
      void *vertices = Runtime_BumpAllocateAlternatePool(32);
      struct RenderModel *model = Render_AllocateModelList(1);

      model->kind = 6;
      model->mesh = gData_08199364;
      model->texture = &texture;
      model->vertices = vertices;
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = gData_02014000;
      if (frame > 15) {
        s32 scale = (frame << 12) - 0x10000;

        if (scale > 0x1bd50) {
          scale = 0x1bd50;
        }
        model->shade = 0;
        SceneTransform_ApplyPitch(-0xbb8);
        Func_08015160((center[0] / 2 - 64) << 16, 0x280000, 0);
        Func_0801521c(scale >> 1);
        SceneTransform_ApplyYaw(frame << 9);
        Render_TransformVertices(gData_08199210, vertices, 4);
        Render_DrawModelList(model);
      }
      MemFree(model);
      MemFree(vertices);
    }
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(104);
  BattleFx_EndCanvasLayer();
}
