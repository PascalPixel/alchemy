// Draft, differ 539 of 1122, skeleton gap 56 (tla-en, 2026-11-01)
// BattleEffect_RunTrident: same calls and control flow, draft 1119 instructions.
// Left, the one blocker first: the frame is 96 bytes, not 104. Two slots are
// missing, and most of the differ is sp offsets moved by them.
// - sp+20: "mov r0, sp; adds r0, #60; str r0, [sp, #20]" is a spilled second
//   pointer to texture. gcse PRE deletes it here ("redundant insn" in the gcse
//   dump) because the first &texture dominates it, so it becomes r8 and pushes
//   picture out of r8. Not stopped by: an offset spelled through
//   &texture.pixels (folded), tex2 = model->texture (takes a register, loads).
//   Calls do not kill sfp in gcse.c; a libcall block would hide an insn from
//   the hash table, but no libcall sits in either block.
// - sp+44: never referenced in the listing, so a function-level local that
//   reload spilled and whose stores were deleted; not identified.
// Slot order in the listing, high to low: effect 56, work 52, canvas 48, ? 44,
// blit 40, sprites 36, centerX 32, circles 28, vertices 24, tex2 20; declaring
// them so gives the right order 8 bytes low, but measures 20 worse until the
// two slots exist (559 against 539), so the declarations here stay as they were.
// Next: order only. offset's pool load comes before the 0xf8 id's (the listing
// has it after), which shifts about ten pool offsets; moving the statement past
// the call swaps size and offset (r6, r7). The rest is sched2 order and reload
// rotation after the frame.
// Found: fixed constants at the droplet base, the tier's cell base and the
// strike's sheet (a pool load made by reload, so it holds no local-alloc
// register); offset doubles as the cell base, which makes it global and r7;
// the reloads of 0x78f0 and size = 2 rise over calls by sched2, no local;
// Poseidon's x and y are givs of i, not hand-stepped; tier is taken before the
// call so combine keeps asrs and lsls; scale doubling as the trident slot
// puts that in r5; a do-while(0) around Func_08196404 stops CSE, so the width
// and height tables stay in the glint loop; start in the trident loop is its
// own biv; the tridents and the strike's draw loops take their own walkers.

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
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

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // BattleEffect_TintBattler
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
void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void EffectPosition_ProjectBattler(s32 id,
                                                 struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 count, s32 resource, s32 mode);   // SpawnObjects
void BattleEffect_SelectLivingTargets(struct BattleEffectArgument *effect); // SelectLivingTargets
void Func_080150e4(s32 angle); // SceneTransform_ApplyRoll
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void Render_ApplyProjectedPlacementFar(void *object, s32 *request, s32 *source,
                                       s32 flags);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void Func_08158ce0(s32 randomMask, u32 shakeRange); // Camera_ApplyShake
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                      s32 drag);
void **GetBattleObjectSlotFar(s32 objectId);
void *Heap_AllocEwram(s32 size);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);
s32 Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void Render_BuildQuadStrip(void *mesh, s32 columns, s32 width, s32 height);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void Transform_SetLookAt(s32 *eye, s32 *target);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void Transform_Push(void);
void Transform_Pop(void);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1])

#define LOAD_GLINT_BLITTER()                                                    \
  do {                                                                         \
    Func_08196404(188, 7, 7, 3 | Data_08199e54[Random16() & 3], 1);             \
  } while (0)

void Func_0815e22c(s32 *center); // the targets' centre on screen
void Func_0815f0a0(s32 palette);
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                   s32 mode);
void ResourceObject_ReleaseFar(void *object);

extern char ResourceId_TornadoSheet;
extern char ResourceId_Data0F8;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_Data0C1;
extern char ResourceId_Data0FA;
extern char ResourceId_Data0DA;
extern char ResourceId_Data134;
extern char ResourceId_EffectPalette02;
extern char ResourceId_OrangePaletteB;
extern u8 gMapCellBuffer[];
extern u8 Data_02011000[];
extern u8 Data_02011400[];
extern u8 Data_02012964[]; // 128 sparks
struct PlacementSource {
  s32 x;
  s32 y;
};
extern const struct PlacementSource Data_08196f18;
extern const u16 gData_08197410[];
extern const u16 gData_08197486[]; // the glints' cells
extern const u8 gData_08197492[];  // widths
extern const u8 gData_08197498[];  // heights
extern const u8 Data_08198cac[];
extern const u8 Data_08198ec4[];
extern const u8 gData_08199268[];
extern const u8 Data_08199dcb[];  // each ring's start
extern const u8 Data_08199dd3[];  // growth
extern const s8 Data_08199ddb[];  // height
extern const s32 Data_08199de4[]; // each trident's yaw and turn
extern const u8 Data_08199e24[];
extern const u8 Data_08199e54[];

// Abilities: Trident (Poseidon).
//
// Poseidon (four objects) rises behind the targets as rings of foam
// spread across the water; seven spinning tridents fly in from frame 18,
// spray bursts up around them, and at 64 the waters strike every target.
void BattleEffect_RunTrident(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  DrawRectangle blit;
  s16 *circles;
  s32 center[3];
  s32 placement[4];
  struct PlacementSource source;
  struct ModelTexture texture;
  struct RenderModel *model;
  void *vertices;
  struct EffectStep *p;
  s32 centerX;
  s32 frame;
  s32 i;
  s32 size;
  s32 offset;
  s32 x;
  struct ModelTexture *tex;
  struct ModelTexture *tex2;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_SelectLivingTargets(work->effect);
  BattleEffect_SpawnObjects(4, 664, 1);
  REG_WININ = 0x2737;
  REG_WINOUT = 0x3f21;
  REG_WIN0V = 0x107c;
  REG_WIN1V = 0x1088;
  BattleEffect_LoadBlitter(SLOT_BLITTER,
                BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  work->transferMode = 2;
  work->transferValue = 50;
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  circles = (s16 *)(work->sheet + 0x4c56);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  offset = (s32)gMapCellBuffer;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0F8, work->sheet + 0xc56, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             (u8 *)offset + 0x5000, 1, 0);
  offset = 0x2400;
  size = 2;
  // Twelve shaded droplets, 2 to 24 wide.
  for (i = 0; i != 12; i++) {
    Func_0815b434(Ram_MapCellBuffer + offset, size, 0x10000);
    circles[i] = offset;
    offset += (i * 2 + 3) * size / 2;
    size += 2;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, Ram_MapCellBuffer, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0FA, Data_02011000, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, Data_02011400, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_EffectPalette02),
                  128);
  for (i = 0, p = work->particles; i != 32; i++, p++) {
    s32 radius = i * 2;
    s32 angle = Random16() & 0xffff;

    p->x = Trig_Sin(angle) * radius;
    p->y = -(Trig_Cos(angle) * radius);
    p->variant = i / 2 + 25;
  }
  Func_0815e22c(center);
  centerX = center[0] / 2;

  for (frame = 0; frame != 104; frame++) {
    if (frame == 0) {
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_EffectPalette02),
                      128);
    }
    if (frame > 76) {
      Func_0815f0a0((s32)&ResourceId_EffectPalette02);
    } else if (frame > 64) {
      Func_0815f0a0((s32)&ResourceId_OrangePaletteB);
    } else if (frame > 23) {
      Func_0815f0a0((s32)&ResourceId_EffectPalette02);
    } else if (frame > 8) {
      Func_0815f0a0((s32)&ResourceId_OrangePaletteB);
    }
    if (frame == 8) {
      work->shakeFrames = frame;
      for (i = 0; i != effect->count; i++) {
        Func_0814cd48(effect->actors[i], 7, 5, i, 16);
      }
    }
    if (frame == 48) {
      work->shakeFrames = 8;
    }
    if (frame == 64) {
      work->shakeFrames = 16;
      Func_081180e8(144);
      for (i = 0; i != effect->count; i++) {
        BattleEffect_StartActorMotion(effect->actors[i], 1, 0x20000, 0x80000,
                                  0x80000, 120);
      }
    }
    if (frame == 0) {
      Sound_PlayFar(212);
    }
    if (frame == 4) {
      Sound_PlayFar(144);
    }
    if (frame == 16) {
      Sound_PlayFar(164);
    }
    if (frame == 64) {
      Sound_PlayFar(145);
    }

    // Poseidon rises.
    if (frame <= 63) {
      s32 left = centerX - 8;
      s32 visible = 128;
      s32 rise;

      if (frame <= 9) {
        rise = frame * 16 - 144;
      } else {
        rise = 0;
      }
      if (rise + 128 > 108) {
        visible = visible - rise - 20;
      }
      if (visible > 0) {
        source = Data_08196f18;
        placement[3] = 0;
        placement[1] = 255 << 16;
        for (i = 0; i != 4; i++) {
          placement[0] = (left << 17) + 0x100000;
          placement[2] = (rise << 16) + 0x280000 + (i << 21);
          Render_ApplyProjectedPlacementFar(work->objects[i], placement,
                                            (s32 *)&source, 0);
        }
      }
    }

    // Spray: a pool of foam, then droplets thrown up.
    if ((u32)(frame - 12) <= 52) {
      s32 grow;

      if (frame == 12) {
        for (i = 0; i != 32; i++) {
          work->particles[i].variant = -2 * i;
        }
      }
      grow = frame * 4 - 156;
      for (i = 0, p = work->particles; i != 14; i++, p++) {
        p->variant++;
        if (i == 0) {
          if (frame > 39) {
            u8 *pool = work->sheet + 0x4c86;

            Func_0815b434(pool, grow, 0x10000);
            Func_0818caa8(pool, centerX, 100, grow);
          }
          continue;
        }
        if (p->variant == 0) {
          s32 angle = Random16() & 0x7fff;
          s32 heading = angle + 0x4000;
          s32 speed = (Random16() & 511) + 512;
          s32 sine;

          sine = Trig_Sin(heading) * speed;
          x = centerX << 16;
          p->x = (sine >> 4) + x;
          p->y = ((Trig_Cos(heading) * speed) >> 3) + 0x680000;
          angle += 0xc000;
          p->velocityX = (Trig_Sin(angle) * speed) >> 9;
          p->velocityY = (Trig_Cos(angle) * speed) >> 8;
          p->velocityZ = (Random16() & 7) + 4;
        }
        if ((u32)p->variant <= 31) {
          s32 drop = ((Trig_Sin(p->variant << 10) * p->velocityZ) >> 16) * 2;
          s32 index = drop - 2;

          if ((u32)index <= 61) {
            s32 dropY = HI(p->y);
            s32 dropX = HI(p->x);
            s32 tier = index >> 1;

            BattleEffectKernels_IntegrateVector2(p, 64, 0);
            Func_0818caa8(Ram_MapCellBuffer + circles[tier], dropX, dropY, drop);
          }
        }
      }
    }

    if (frame > 7) {
      s32 scale;

      vertices = Heap_AllocEwram(0x400);
      model = Render_AllocateModelList(1);
      texture.logWidth = 7;
      texture.logHeight = 7;
      model->unk04 = 0;
      tex = &texture;
      tex->pixels = Ram_MapCellBuffer;
      model->kind = 7;
      model->texture = tex;
      model->mesh = Data_08198ec4;
      model->vertices = vertices;
      model->scrollV = model->scrollU = 0;

      // Eight rings of foam.
      for (i = 0; i != 8; i++) {
        s32 start = Data_08199dcb[i];
        s32 shade;

        if (frame <= start) {
          continue;
        }
        scale = (frame - start) * Data_08199dd3[i] * 1500 + 0x4180;
        shade = (start - frame) * 8 + 56;
        if (shade > 0) {
          shade = 0;
        }
        if (shade <= -64) {
          continue;
        }
        model->shade = shade;
        Render_ResetTransformState();
        Func_08015160((centerX - 64) << 16, Data_08199ddb[i] << 16, 0);
        Func_080151e4(scale, scale * 2, scale);
        model->scrollU = ((frame + i * 5) & 7) << 4;
        SceneTransform_ApplyPitch(3000);
        Render_TransformVertices(Data_08198cac, vertices, 32);
        Render_DrawModelList(model);
      }

      // The tridents.
      if (frame <= 71) {
        const s32 *spin;
        s32 start;
        struct EffectStep *t;
        struct EffectStep *spinner;

        tex->logWidth = 5;
        tex2 = &texture;
        tex2->logHeight = 7;
        model->texture = tex2;
        model->kind = 7;
        model->mesh = gData_08199268;
        model->vertices = vertices;
        model->scrollU = 0;
        model->scrollV = 0;
        if (frame == 8) {
          for (i = 0, t = &work->particles[32], spin = Data_08199de4; i != 8;
               i++, t++, spin += 2) {
            t->z = spin[0];
            t->velocityZ = spin[1];
            t->variant = (i % 6) * 4;
          }
        }
        for (i = 0, spinner = &work->particles[32], start = 18; i != 7;
             i++, spinner++, start += 4) {
          s32 picture;

          scale = i * 4;
          if (frame < start || frame >= scale + 42) {
            continue;
          }
          picture = (spinner->variant / 8) % 3;
          model->shade = -16;
          tex2->pixels = work->sheet + (picture << 12) + 0xc56;
          Render_ResetTransformState();
          Func_08015160((centerX - 64) << 16, 0x200000, 0);
          Func_080150e4(spinner->z);
          if (i & 1) {
            SceneTransform_ApplyYaw(0x8000);
          }
          spinner->z += spinner->velocityZ;
          Func_080151e4(0x20000, 0x12710, 0x10000);
          Render_TransformVertices(Data_08199e24 + picture * 16, vertices, 4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }

    // The strike: flashes and a fountain of sparks.
    if (frame > 63) {
      struct EffectStep *drawn;

      if (frame == 64) {
        x = centerX << 16;
        for (i = 0, p = work->particles; i != 32; i++, p++) {
          s32 angle = Random16();
          s32 speed = (Random16() & 511) + 512;
          s32 velocityY;
          s32 delay;

          p->velocityX = (Trig_Sin(angle) * speed) >> 8;
          velocityY = (Trig_Cos(angle) * speed) >> 7;
          p->velocityY = velocityY;
          if (i > 15) {
            p->x = (p->velocityX << 2) + x;
            p->y = (velocityY << 2) + 0x640000;
            delay = 16 - i;
          } else {
            p->x = x;
            p->y = (Random16() % 104) << 16;
            delay = -i;
          }
          p->variant = delay / 4;
        }
        for (i = 0, p = (struct EffectStep *)Data_02012964; i != 128; i++, p++) {
          s32 angle = (Random16() & 0x7fff) + 0x4000;
          s32 speed = (Random16() & 511) + 512;

          p->x = x;
          p->y = 0x640000;
          p->velocityX = (Trig_Sin(angle) * speed) >> 6;
          p->velocityY = (Trig_Cos(angle) * speed) >> 5;
          p->variant = (Random16() & 15) + 16;
        }
      }
      for (i = 0, drawn = work->particles; i != 32; i++, drawn++) {
        if ((u32)drawn->variant <= 23) {
          blit(canvas, Ram_MapCellBuffer + 0x5000 + ((drawn->variant / 4) << 11),
               HI(drawn->x) - 16, HI(drawn->y) - 32, 32, 64);
          BattleEffectKernels_IntegrateVector2(drawn, 62, -0x4000);
        }
        drawn->variant++;
      }
      for (i = 0, drawn = (struct EffectStep *)Data_02012964; i != 128;
           i++, drawn++) {
        s32 spark = drawn->variant;

        if (spark < 0) {
          continue;
        }
        spark >>= 3;
        spark++;
        blit(canvas, sprites + gData_08197410[spark - 1],
             HI(drawn->x) - spark / 2, HI(drawn->y) - spark, spark, spark * 2);
        drawn->variant--;
        BattleEffectKernels_IntegrateVector2(drawn, 56, 0x2000);
      }
    }

    // Glints over the water.
    if ((u32)(frame - 24) <= 39) {
      for (i = 0; i != 4; i++) {
        s32 k = i & 3;
        s32 angle = Random16();
        s32 x =
            centerX + ((Trig_Sin(angle) << 3) >> 16) - (gData_08197492[k] >> 1);
        s32 y = (i << 5) + ((Trig_Cos(angle) << 4) >> 16) -
                (gData_08197498[k] >> 1);

        LOAD_GLINT_BLITTER();
        ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
            canvas, work->sheet + gData_08197486[k], x, y, gData_08197492[k],
            gData_08197498[k]);
        HeapSlot_Release(188);
      }
    }
    Func_08158ce0(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  for (i = 0; i != 4; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  BattleEffect_EndCanvasLayer();
}
