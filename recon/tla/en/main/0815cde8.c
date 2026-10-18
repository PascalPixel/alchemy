// Draft of BattleFx_RunAzul, decompiled from the listing: differ 2020 of 2209
// (draft 2179), the same 130 calls and control flow; allocation differs.

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

void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_08143bb8(void); // BattleFx_EndCanvasLayer
void Func_08143000(void); // BattleFx_ProcessPendingGraphicsTransfer
void Func_081434f8(void); // ObjectGroup_TickMemberTimers
void Func_081963ec(s32 slot, s32 modeAndFlags); // BattleFx_LoadWork
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // ObjectGroup_UpdateMembers
void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void Func_0815585c(struct BattleEffectArgument *effect, s32 packed, s32 *screen,
                   s32 *otherPoint);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct RenderModel *Func_081969f8(s32 count);
void Func_08196958(const void *corners, void *vertices, s32 count);
void Func_08196a7c(struct RenderModel *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id,
                                                 struct EffectPosition *out);
void Func_08152404(s32 count, s32 resource, s32 mode);   // SpawnObjects
void Func_0814cc4c(struct BattleEffectArgument *effect); // SelectLivingTargets
void Func_080150e4(s32 angle); // SceneTransform_ApplyRoll
void BattleFx_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void Render_ApplyProjectedPlacementFar(void *object, s32 *request, s32 *source,
                                       s32 flags);
void BattleFx_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattleEventRuntime_BeginPhaseFar
void Func_08158ce0(s32 randomMask, u32 shakeRange); // Camera_ApplyShake
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                      s32 drag);
void **GetBattleObjectSlotFar(s32 objectId);
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

void Func_08178680(void *mesh, s32 columns, s32 width, s32 height);
void BattleFxKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
s32 Battle_GetObjectTableValueFar(s32 unit);
void Transform_SetLookAt(s32 *eye, s32 *target);
void BattleFx_BeginTiledCanvas(s32 bgControl);
void BattleFx_BeginTiledCanvasFilled(s32 bgControl);
void Transform_Push(void);
void Transform_Pop(void);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1])

#include "bg_scroll.h"
#include "input.h"
#include "io_write_queue.h"

struct ResourceTableEntry {
  u16 unk0;
  u16 tile;
};

// A sprite object as Azul edits it: its tile number in the low ten bits of
// the halfword at 8, its priority bits in the byte at 9 and its shape at 18.
struct AzulObject {
  u8 unk0[8];
  u16 tile : 10;
  u16 unk08 : 6;
  u8 unk0a[8];
  u16 shape;
};

#define OBJ_PRIORITY(obj) (((u8 *)(obj))[9])

extern struct ResourceTableEntry gResourceTableEntries[];
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern const u8 Data_081986f2[];
extern const u8 Data_08198828[];
extern const u8 Data_08198830[];
extern const u8 Data_0819883c[];
extern const u8 Data_08198848[];
extern const s8 Data_08198850[];
extern const s8 Data_08198858[];
extern const u8 Data_081991e0[];
extern const u8 gData_08199340[];
extern const u8 gData_08199364[];

struct AzulObject *Func_0815b3b0(s32 width, s32 height, u32 shape, s32 mode);
void ResourceObject_ReleaseFar(void *object);
void Func_0813ba50(void);
void Func_08164bb4(void); // BattleFx_ArmPaletteHBlankDma
void Func_0818de3c(void *triangles, s32 columns, s32 rows, s32 cellWidth,
                   s32 cellHeight);
void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
void ReleaseBattleObjectRecordsFar(s32 actor);
void *GetMotionRecordFar(void *object, s32 index);
void ResourceMetadata_UnregisterFar(void *record, s32 entry);
void Func_08118148(s16 *actors, s32 a, s32 b);
void MemFree(void *buffer);

#define SLOT_ALT 188 // unkb8[1]

// Clamps a sprite's projected y to the screen's band.
#define CLAMP_Y(place)                                                         \
  do {                                                                         \
    if ((place)[2] < -0x10000)                                                 \
      (place)[2] = -0x10000;                                                   \
    if ((place)[2] > 0x9fffff)                                                 \
      (place)[2] = 0xa00000;                                                   \
  } while (0)

// Azul: the sea rises into a wall of water, a great serpent coils out of it
// and swallows the foes, then the wave breaks over them in a spray.
void BattleFx_RunAzul(struct BattleEffectArgument *effect) {
  u8 *canvas;
  struct BattleEffectWork *work;
  u8 *sprites;
  DrawRectangle blit;
  DrawRectangle blit2;
  u8 *battle;
  u8 *faces;
  s8 *verts;
  u8 *grid;
  u8 *sphere;
  s32 frame;
  s32 i;
  s32 j;
  s32 k;
  u32 t;
  s32 baseX;
  s32 baseY;
  s32 velX;
  s32 velY;
  s32 posX;
  s32 posY;
  s32 place[4];
  struct ModelTexture texture;
  struct ModelTexture texture2;
  struct EffectStep *p;
  struct RenderModel *model;
  void *vertices;
  struct AzulObject *obj;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  Func_0813ba50();
  BattleFx_BeginCanvasLayer(3);
  REG_BLDCNT = 0x3f44;
  Func_081963ec(SLOT_BLITTER, 3);
  blit = Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress(0x134, sprites, 0, 0);
  Func_0814cc4c(work->effect);
  work->transferMode = 1;
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(Func_08164bb4, 0xcfe);
  Scheduler_AddOrUpdateCallback(Func_08143000, 3200);
  battle = (u8 *)Ram_HeapSlots->battleWork;
  grid = work->sheet + 0xc00;
  verts = (s8 *)work->sheet;
  faces = work->sheet + 0x1f0;
  sphere = work->sheet + 0x800;
  {
    u8 *cells = gMapCellBuffer;

    for (i = 0; i < 120; i++) {
      for (j = 0; j < 0x800; j += 64) {
        Iwram_CopyWords(
            cells, (void *)(0x06008000 + ((i / 8 * 256 + (i & 7)) * 8 + j)), 8);
        cells += 8;
      }
    }
  }
  {
    u8 *cells = work->sheet + 0x2bb8;
    u8 *dst;

    Resource_LoadAndDecompress(0x190, cells, 1, 1);
    dst = work->sheet + 0x2df8;
    for (i = 0; i != 288; i++) {
      *dst++ = *cells;
      cells += 2;
    }
  }
  WaitFrames(1);
  Func_08152404(12, 0x28c, 1);
  for (i = 0; i != 16; i++) {
    obj = Func_0815b3b0(32, 32, 0x80002000, 0);
    work->objects[i + 12] = obj;
    Iwram_CopyWords(obj, work->objects[5 + (i & 3)], 24);
    obj = work->objects[i];
    OBJ_PRIORITY(obj) = (OBJ_PRIORITY(obj) & ~12) | 4;
  }
  for (frame = 0; frame != 228; frame++) {
    if ((gInput.repeat & 3) && frame > 15 && frame <= 185) {
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      work->transferPending = 1;
      WaitFrames(1);
      frame = 186;
      REG_BLDALPHA = 0x1010;
      for (i = 0; i != 28; i++) {
        ((struct AzulObject *)work->objects[i])->shape = 0x4000;
      }
    }
    if (frame <= 8) {
      ColorBuffer_ScaleFar(battle + 0x56c, (void *)(BG_PLTT + 0xc0),
                           0x10000 - (24 - frame * 3) * 1092, 128);
    }
    if (frame == 186) {
      baseX = 0x300000;
      baseY = 0x420003;
      velX = 0;
      velY = -0x60000;
    }
    if (frame == 0) {
      REG_BLDALPHA = 0x10;
      work->transferMode = 1;
      work->transferValue = frame;
      for (i = 0; i != 28; i++) {
        ((struct AzulObject *)work->objects[i])->shape = 0xc000;
      }
      velX = 0;
      baseY = -0x780000;
      baseX = 0x780000;
      velY = 0xc0000;
      IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
      for (i = 0; i != 31; i++) {
        const u8 *piece = Data_081986f2 + i * 10;
        u8 *face = faces + i * 36;
        s8 *v = verts + i * 16;

        for (j = 0; j != 4; j++) {
          v[j * 4 + 0] = piece[2 + j * 2] - piece[0];
          v[j * 4 + 1] = piece[3 + j * 2] - piece[1];
          v[j * 4 + 2] = 0;
        }
        face[1] = 1;
        face[2] = 2;
        face[0] = 0;
        face[13] = 2;
        face[14] = 3;
        face[12] = 0;
        face[24] = 0;
        face[25] = 0;
        face[26] = 0;
        for (j = 0; j != 3; j++) {
          face[4 + j * 2] = Data_081986f2[(i * 5 + face[j]) * 2 + 2];
          face[5 + j * 2] = Data_081986f2[(i * 5 + face[j]) * 2 + 3];
          face[16 + j * 2] = Data_081986f2[(i * 5 + face[12 + j]) * 2 + 2];
          face[17 + j * 2] = Data_081986f2[(i * 5 + face[12 + j]) * 2 + 3];
        }
      }
      for (i = 0; i != effect->count; i++) {
        ((s32 *)*GetBattleObjectSlotFar(effect->actors[i]))[3] = 0;
      }
      REG_BG2CNT = 0x787;
    }
    t = frame - 10;
    if (t <= 16) {
      REG_BLDALPHA = t << 8 | 0x10;
    }
    if ((u32)(frame - 16) <= 63) {
      if (frame == 16) {
        p = &work->particles[31];
        for (i = 0; i != 27; i++, p++) {
          p->x = ((Random16() & 63) + 32) << 16;
          p->y = ((Random16() & 15) + 120) << 16;
          p->velocityX = ((Random16() & 31) - 16) << 13;
          p->velocityY = ((Random16() & 31) - 63) << 12;
          p->variant = (Trig_Cos(i << 9) << 5 >> 16) - 32;
        }
      }
      p = &work->particles[31];
      for (i = 0; i != 27; i++, p++) {
        if (p->variant >= 0) {
          s32 s = Trig_Sin((frame << 10) + i * 0x1a16);

          blit(canvas, work->sheet + 0x2df8, ((p->x + (s << 4)) >> 16) - 6,
               HI(p->y) - 12, 12, 24);
          BattleFxKernels_IntegrateVector2(p, 63, -0x4000);
        }
        p->variant++;
      }
    }
    if (frame <= 95 || (u32)(frame - 160) <= 31) {
      u16 *c = (u16 *)(work->unk7300 + 0x340);
      s32 level = frame / 8;

      if (frame > 159) {
        level = (frame - 160) / 4 + 12;
        if (level > 20) {
          level = 20;
        }
      } else if (level > 12) {
        level = 12;
      }
      for (i = 0; i != 15; i++) {
        *c++ = 0;
      }
      for (; i != 135; i++) {
        s32 d = i - 15;
        s32 b = -(d / 15) - level + 40;
        s32 g = -(d / 16) - level + 32;
        s32 r = -(d / 17) - level + 16;

        if (b < 0) {
          b = 0;
        }
        if (b > 31) {
          b = 31;
        }
        if (g < 0) {
          g = 0;
        }
        if (g > 31) {
          g = 31;
        }
        if (r < 0) {
          r = 0;
        }
        if (r > 31) {
          r = 31;
        }
        *c++ = b << 10 | g << 5 | r;
      }
      for (; i != 160; i++) {
        *c++ = 0;
      }
    }
    if (frame >= 0) {
      baseX += velX;
      baseY += velY;
    }
    if (frame == 168) {
      velX = 0;
      velY = -0x60000;
      baseX = 0x300000;
      baseY = 0xa80000;
      for (i = 0; i != 28; i++) {
        ((struct AzulObject *)work->objects[i])->shape = 0x4000;
      }
      for (i = 0; i != effect->count; i++) {
        s32 *actor = *GetBattleObjectSlotFar(effect->actors[i]);

        BattleFx_StartActorMotion(effect->actors[i], 1, 0, 0, 0x40000, 100);
        actor[18] = 0;
      }
    }
    if (frame == 186) {
      velY = -0x100000;
    }
    if ((frame < 54 || frame > 167) && frame < 214) {
      place[3] = 0;
      place[1] = 0xff0000;
      if (frame > 167) {
        posY += -0x60000;
      }
      if (frame <= 53) {
        for (i = 0; i != 12; i++) {
          place[0] = ((40 - Data_08198830[i]) << 16) + baseX;
          place[2] = ((128 - Data_0819883c[i] * 31 / 32) << 16) + baseY;
          if (i > 8) {
            place[2] += -0xf00000;
          }
          CLAMP_Y(place);
          Render_ApplyProjectedPlacementFar(work->objects[i], place,
                                            (s32 *)Data_08198828, 0);
        }
        for (i = 0; i != 16; i++) {
          k = (i & 3) + 5;
          place[0] = ((39 - Data_08198830[k]) << 16) + baseX;
          place[2] = ((-(Data_0819883c[k] * 30 / 32) - i / 4 * 60) << 16) +
                     baseY + 0x420000;
          CLAMP_Y(place);
          obj = work->objects[i + 12];
          obj->tile = ((struct AzulObject *)work->objects[k])->tile;
          Render_ApplyProjectedPlacementFar(obj, place, (s32 *)Data_08198828,
                                            0);
        }
      } else {
        for (i = 0; i != 12; i++) {
          place[0] = (Data_08198830[i] << 16) + baseX;
          place[2] = (Data_0819883c[i] << 16) + baseY;
          if (i > 8) {
            place[2] += 0x1000000;
          }
          CLAMP_Y(place);
          Render_ApplyProjectedPlacementFar(work->objects[i], place,
                                            (s32 *)Data_08198828, 0);
        }
        for (i = 0; i != 16; i++) {
          k = (i & 3) + 5;
          place[0] = (Data_08198830[k] << 16) + baseX + -0x10000;
          place[2] = ((Data_0819883c[k] + i / 4 * 64) << 16) + baseY + 0x400000;
          CLAMP_Y(place);
          obj = work->objects[i + 12];
          obj->tile = ((struct AzulObject *)work->objects[k])->tile;
          Render_ApplyProjectedPlacementFar(obj, place, (s32 *)Data_08198828,
                                            0);
        }
      }
    } else if (frame >= 54 && frame < 168) {
      s32 x;
      s32 y;

      if (frame == 54) {
        for (i = 0; i != 28; i++) {
          ((struct AzulObject *)work->objects[i])->shape = 0;
        }
        baseX = 0x1000000;
        baseY = 0x480000;
        velX = -0x60000;
        velY = 0;
      }
      if (frame == 138) {
        velX = 0x80000;
      }
      place[3] = 0;
      place[1] = 0xff0000;
      velX = velX * 56 / 64;
      velY = velY * 56 / 64;
      if (frame <= 137) {
        posX = (Trig_Sin(frame << 9) << 4) + baseX;
        x = posX;
        posY = (Trig_Cos(frame << 9) << 3) + baseY;
        y = posY;
      } else {
        x = baseX + (Trig_Sin(frame << 9) << 4);
        y = (Trig_Cos(frame << 9) << 3) + baseY;
      }
      for (i = 0; i != 8; i++) {
        place[0] = (Data_08198850[i] << 16) + x;
        place[2] = (Data_08198858[i] << 16) + y;
        Render_ApplyProjectedPlacementFar(work->objects[i], place,
                                          (s32 *)Data_08198848, 0);
      }
    }
    if ((u32)(frame - 138) <= 29) {
      s32 d = -0x20000;

      if (frame > 151) {
        d = (frame << 13) + -0x150000;
      }
      posX += d;
    }
    if (frame == 78) {
      Audio_PlayCue(140);
    }
    if (frame == 138) {
      Audio_PlayCue(104);
    }
    if (frame == 152) {
      Audio_PlayCue(163);
    }
    if (frame == 7) {
      Audio_PlayCue(212);
    }
    if (frame == 10) {
      const u8 *piece = Data_081986f2;

      Audio_PlayCue(144);
      p = work->particles;
      for (i = 0; i != 31; i++, piece += 10, p++) {
        s32 cx = piece[0] >> 1;
        s32 cy = piece[1];

        p->x = cx << 16;
        p->y = cy << 16;
        p->z = 0;
        p->velocityX = (cx - 60) << 12;
        p->velocityY = (cy - 60) << 10;
        p->velocityZ = ((Random16() & 127) << 2) + 512;
        if (Random16() & 1) {
          p->velocityZ = -p->velocityZ;
        }
      }
      IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
    }
    if (frame == 10 || frame == 186) {
      for (i = 0; i != effect->count; i++) {
        void **slot = GetBattleObjectSlotFar(effect->actors[i]);
        s32 *actor = slot[0];
        void *record;

        if (frame == 186) {
          ReleaseBattleObjectRecordsFar(effect->actors[i]);
        }
        if (frame == 10) {
          for (j = 0; (record = GetMotionRecordFar(slot[0], j)) != 0; j++) {
            ResourceMetadata_UnregisterFar(record, (s32)slot[9]);
          }
          Func_0814cd48(effect->actors[i], 9, 5, i, 8);
          BattleFx_StartActorMotion(effect->actors[i], 1, 0, 0,
                                    (Random16() << 1) + 0x10000, 100);
          actor[18] = 0;
        }
      }
      if (frame == 186) {
        effect->actors[effect->count] = 0xff;
        Func_08118148(effect->actors, 0, 0);
      }
    }
    if (frame > 9) {
      for (i = 0; i != effect->count; i++) {
        s32 *actor = *GetBattleObjectSlotFar(effect->actors[i]);

        actor[10] = actor[10] * 60 / 64;
      }
    }
    if (t <= 43) {
      vertices = Runtime_BumpAllocateAlternatePool(32);
      model = Func_081969f8(1);
      texture.logWidth = 8;
      texture.logHeight = 7;
      texture.pixels = gMapCellBuffer;
      model->texture = &texture;
      model->kind = 8;
      model->vertices = vertices;
      model->scrollU = gBgScroll[1].x;
      p = work->particles;
      for (i = 0; i != 31; i++, p++) {
        model->mesh = faces + i * 36;
        Render_ResetTransformState();
        Func_08015160(p->x + -0x400000, p->y + -0x400000, 0);
        Func_080151e4(0x8000, 0x10000, 0x8000);
        switch (i & 3) {
        case 0:
          SceneTransform_ApplyPitch(p->z);
          Func_080150e4(p->z);
          break;
        case 1:
          SceneTransform_ApplyPitch(p->z);
          Func_080150e4(-p->z);
          break;
        case 2:
          SceneTransform_ApplyPitch(p->z);
          SceneTransform_ApplyYaw(p->z);
          break;
        case 3:
          Func_080150e4(p->z);
          SceneTransform_ApplyYaw(-p->z);
          break;
        }
        Func_08196958(verts + i * 16, vertices, 4);
        BattleFxKernels_IntegrateVector3(p, 62, 0);
        if (frame >= (8 - i) * 2 + 16) {
          p->velocityY += -0x8000;
        }
        Func_08196a7c(model);
      }
      MemFree(model);
      MemFree(vertices);
    }
    if (frame == 78) {
      s32 angle = 0;
      s32 offset = 0;

      Func_0818de3c(grid, 16, 16, 8, 8);
      for (i = 0; i != 16; i++, angle += 0x888, offset += 64) {
        s32 y = -Trig_Cos(angle) * 48 >> 16;
        s32 radius = Trig_Sin(angle) * 48 >> 16;
        s8 *v = (s8 *)sphere + offset;

        for (j = 0; j != 16; j++) {
          v[0] = radius * Trig_Sin(j << 12) >> 16;
          v[1] = y;
          v[2] = radius * Trig_Cos(j << 12) >> 16;
          v += 4;
        }
      }
      Resource_LoadAndDecompress(0x16a, gData_02014000, 1, 0);
      for (i = 0; i != 6; i++) {
        for (j = 0; j != 16; j++) {
          u8 *src = gData_02014000 + i * 2048 + j * 128;
          u8 *dst = work->sheet + 0x3308 + i * 1024 + j;

          for (k = 0; k != 64; k++) {
            *dst = *src++;
            dst -= 16;
          }
        }
      }
      Resource_LoadAndDecompress(0xec, gData_02014000, 1, 0);
      Func_0816ae40(gData_02014000, gMapCellBuffer, 64, 64);
      Resource_LoadAndDecompress(0xc2, gData_02014000, 0, 0);
      work->transferMode = 2;
      work->transferValue = 50;
    }
    if (frame == 152) {
      Resource_LoadAndDecompress(0xb4, gData_02014000, 0, 0);
    }
    t = frame - 80;
    if (t <= 107) {
      s32 scale;

      REG_BG2CNT = 0x784;
      if ((u32)(frame - 168) <= 17) {
        s32 y;
        s32 f = frame;

        Func_081963ec(SLOT_ALT, 7);
        blit2 = Ram_HeapSlots->unkb8[1];
        y = (baseY >> 16) - 32;
        for (i = 0; i != 2; i++, f += 3) {
          u8 *cells = work->sheet + 0x2f18 + (f % 6 << 10);

          blit(canvas, cells, 30, y, 16, 64);
          blit2(canvas, cells, 46, y, 16, 64);
        }
        Runtime_ReleaseHeapBlock(SLOT_ALT);
      }
      if (frame > 185) {
        goto spray;
      }
      model = Func_081969f8(1);
      scale = t * 3 << 8;
      if (scale > 0x10000) {
        scale = 0x10000;
      }
      texture2.logWidth = 7;
      texture2.logHeight = 7;
      texture2.pixels = gMapCellBuffer;
      model->kind = 7;
      model->texture = &texture2;
      model->mesh = grid;
      model->shade = 0;
      model->vertices = verts;
      model->scrollU = 0;
      model->scrollV = 0;
      Render_ResetTransformState();
      Func_08015160(posX / 2 + -0x440000, posY + -0x500000, 0);
      Func_080151e4(scale, scale * 2, scale);
      Func_08015160(-0x280000, 0, 0);
      SceneTransform_ApplyPitch(frame << 10);
      SceneTransform_ApplyYaw(frame << 10);
      Func_08196958(sphere, verts, 256);
      Func_08196a7c(model);
      if (frame > 137) {
        s32 shade = 0x6a4 - frame * 10;
        s32 size = (frame << 13) + -0x110000;

        if (shade > 0) {
          shade = 0;
        }
        if (shade > -64) {
          texture2.pixels = gData_02014000;
          texture2.logWidth = 7;
          model->mesh = gData_08199364;
          texture2.logHeight = 7;
          model->shade = shade;
          Render_ResetTransformState();
          Func_08015160(0x2c0000, 0, 0);
          SceneTransform_ApplyYaw(0x3448);
          Func_0801521c(size);
          Func_08196958(Data_081991e0, verts, 4);
          Func_08196a7c(model);
        }
      }
      if (frame > 151) {
        s32 size;

        work->shakeFrames = 2;
        size = (frame << 13) + -0x130000;
        texture2.logWidth = 6;
        texture2.logHeight = 6;
        texture2.pixels = gData_02014000;
        model->shade = 0;
        model->mesh = gData_08199340;
        Render_ResetTransformState();
        Func_08015160(-0x100000, posY + -0x500000, 0);
        Func_080151e4(size >> 1, size, size >> 1);
        Func_080150e4(frame << 11);
        Func_08196958(Data_081991e0, verts, 4);
        Func_08196a7c(model);
      }
      MemFree(model);
    }
    if (frame > 185) {
    spray:
      if (frame == 186) {
        p = (struct EffectStep *)gMapCellBuffer;
        for (i = 0; i != 256; i++, p++) {
          s32 r = Random16() & 127;
          s32 vy;

          p->velocityX = r * Trig_Sin(i << 8) >> 5;
          vy = r * Trig_Cos(i << 8) >> 4;
          p->x = p->velocityX * 6 + 0x300000;
          p->y = vy * 5;
          p->velocityY = vy;
          p->variant = (Random16() & 31) + 32;
        }
      }
      p = (struct EffectStep *)gMapCellBuffer;
      for (i = 0; i != 256; i++, p++) {
        if (p->variant >= 0) {
          blit(canvas, work->sheet + 0x2df8, HI(p->x) - 6, HI(p->y) - 12, 12,
               24);
          BattleFxKernels_IntegrateVector2(p, 63, 0x10000);
          if (p->y > 0x600000) {
            p->velocityY = -p->velocityY / 2;
            p->velocityX <<= 2;
            p->variant -= 8;
          }
        }
        p->variant--;
      }
    }
    if (frame == 186) {
      BattleBackground_LoadFar(1, *(u16 *)(battle + 0x670), 24);
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
      for (i = 0; i != effect->count; i++) {
        Func_0814cd48(effect->actors[i], 7, 5, i, 16);
        Func_08118088(effect->actors[i], 4);
      }
      Audio_PlayCue(145);
      work->shakeFrames = 16;
    }
    if (frame == 214) {
      Func_081180e8(134);
    }
    if (frame > 185) {
      Func_08158ce0(8, 8);
    } else {
      Func_08158ce0(2, 2);
    }
    if (frame > 185) {
      Func_081434f8();
    }
    work->transferPending = 1;
    WaitFrames(1);
  }
  Scheduler_RemoveCallback(Func_08143000);
  Scheduler_RemoveCallback(Func_08164bb4);
  Runtime_ReleaseHeapBlock(SLOT_BLITTER);
  for (i = 0; i != 28; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  Func_08143bb8();
}
