#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"
#include "task.h"
#include "battle.h"
#include "motobj.h"

struct FxPartyList;
struct Model;
struct StripQuad;

struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const void *pixels;
};

struct Corner {
  s8 x;
  s8 y;
  s8 z;
};

struct RenderModel {
  s32 kind;
  s32 unk04;
  const u8 *mesh;
  s16 *vertices;
  struct ModelTexture *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // BattleEffect_TintBattler
void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void Func_0815585c(struct BattleEffectArgument *effect, u32 packed, s32 *screen, s32 *world);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind, u32 variant); // SpawnObjects
void BattleEffect_SelectLivingTargets(struct FxPartyList *party); // SelectLivingTargets
void Func_080150e4(s32 angle);            // SceneTransform_ApplyRoll
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);

void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void Transform_SetLookAt(s32 *eye, s32 *target);

struct StarTarget {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);

void *Resource_GetTableEntry(s32 resource);
s32 Task_Add(void (*func)(void), s32 order);

void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height, s32 count);

// Columns of Data_08199484, one row per Fume level (effect->variant): the
// plume's width, the frame its tail starts, its arc's height, the sparks'
// spread, count and lift.
enum {
  FUME_WIDTH,
  FUME_TAIL_START,
  FUME_HEIGHT,
  FUME_SPREAD,
  FUME_SPARKS,
  FUME_LIFT,
  FUME_COLUMNS
};

extern char ResourceId_SmokeBandSheet;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_ClawSlashSheet;
extern u8 gMapCellBuffer[];
extern const u16 Data_08199484[][FUME_COLUMNS];
extern u8 Data_02011000[];
extern u8 Data_02012000[];
extern const u8 gData_08199340[]; // the head's mesh
extern const u8 Data_081991e0[];  // its corners

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define HI(v) (((s16 *)&(v))[1])
#define FUME_FRAMES 50
#define FUME_SEGMENTS 33

// Abilities: Fume.
//
// A band of smoke arcs from the caster to the target (33 segments of two
// vertices each, a scrolling textured strip), its head growing 44 units a
// frame and its tail following from the level's start frame; a claw strikes
// at the arc's end until frame 22, when the target is hit, pushed back and
// sparks burst from it. The camera shakes harder at higher levels.
void BattleEffect_RunFume(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  // Unused: the second word gives the stack frame its 4 bytes (as a plain
  // local, 106 differ).
  struct {
    DrawRectangle draw;
    s32 unused;
  } blit;
  s32 j;
  s32 frame;
  s32 endX;
  s32 endY;
  struct {
    s32 head;
    s32 tail;
  } arc;
  struct EffectPosition caster;
  struct EffectPosition target;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct EffectStep *spark;
  struct EffectStep *particle;
  s32 i;
  void *buffer;
  u8 *strip;
  struct Corner *corners;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  BattleEffect_LoadBlitter(
      SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  REG_BLDALPHA = 0x1010;
  blit.draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  endX = 0;
  endY = 0;
  EffectPosition_ProjectBattler(effect->actor, &caster);
  EffectPosition_ProjectBattler(effect->actors[0], &target);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeBandSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x800, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ClawSlashSheet,
                             work->sheet + 0x4000, 1, 0);
  REG_BG2PA = 0x80;
  Render_BuildQuadStrip(Data_02012000, 4, 32, 32);

  for (frame = 0; frame != FUME_FRAMES; frame++) {
    if (frame == 0) {
      arc.head = 0;
      arc.tail = 0;
      for (i = 0; i != 64; i++) {
        work->particles[i].variant = 24;
      }
    }
    if (frame == 22) {
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      Func_081180e8(144);
      work->shakeFrames = 8;
      BattleEffect_StartActorMotion(effect->actors[0], 1, 0x20000, 0x40000,
                                    0x20000, 120);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 20);
      for (i = 0; i != 32; i++) {
        spark = &work->particles[i];
        spark->variant = -i / 4 + 2;
        spark->x = (target.x / 2) << 16;
        spark->y = (target.y + (Random16() & 15) - 32) << 16;
        spark->velocityX =
            ((Random16() % Data_08199484[effect->variant][FUME_SPREAD]) -
             (Data_08199484[effect->variant][FUME_SPREAD] >> 1))
            << 12;
        spark->velocityY = (192 - (Random16() & 255)) << 10;
      }
    }
    for (i = 0; i != Data_08199484[effect->variant][FUME_SPARKS]; i++) {
      particle = &work->particles[i];
      if ((u32)particle->variant <= 23) {
        if (particle->variant > 1) {
          blit.draw(canvas,
                    work->sheet + 0x800 + ((particle->variant / 4) << 11),
                    HI(particle->x) - 16, HI(particle->y) - 32, 32, 64);
        }
        BattleEffectKernels_IntegrateVector2(
            particle, 60, -Data_08199484[effect->variant][FUME_LIFT]);
      }
      particle->variant++;
    }

    buffer = Data_02011000;
    // The strip mesh held in a per-frame local, as the vertex buffer is.
    strip = Data_02012000;
    corners = (struct Corner *)gMapCellBuffer;
    // DEAD: frame never falls below 0; the source keeps the test.
    if (frame >= 0) {
      s32 width;
      s32 step;
      s32 sweep;
      s32 vertex;
      s32 shade;

      width = Data_08199484[effect->variant][FUME_WIDTH];
      arc.head = frame * 44;
      if (arc.head > 988) {
        arc.head = 988;
      }
      if (frame >= Data_08199484[effect->variant][FUME_TAIL_START]) {
        arc.tail =
            (frame - Data_08199484[effect->variant][FUME_TAIL_START]) * 44;
        if (arc.tail > 988) {
          arc.tail = 988;
        }
      }

      // The arc's vertices, two per segment, widening over the first six.
      for (j = 0, step = arc.head - arc.tail, sweep = 0, vertex = 0;
           j != FUME_SEGMENTS; sweep += step, vertex += 2, j++) {
        s32 distance = caster.x / 8 - target.x / 8;
        s32 angle = sweep + arc.tail * 33;
        s32 wobble = ((Trig_Sin((frame + j) << 12) << 2) >> 16) * j / 32;
        s32 height;
        s32 thickness;
        s32 radiusX;
        s32 radiusY;
        s32 half;
        s32 baseY;

        if (distance < 0) {
          distance = -distance;
        }
        height = Data_08199484[effect->variant][FUME_HEIGHT];
        thickness = j * 4;
        if (thickness > 24) {
          thickness = 24;
        }
        thickness = thickness * width / 0x10000;
        i = 0;
        half = thickness / -2;
        baseY = height + half;
        radiusY = baseY + wobble;
        radiusX = distance + half + wobble;
        for (; i != 2; i++) {
          corners[vertex + i].x =
              ((Trig_Cos(angle) * radiusX) >> 16) - distance;
          corners[vertex + i].y = -((Trig_Sin(angle) * radiusY) >> 16);
          corners[vertex + i].z = 0;
          radiusY += thickness;
          radiusX += thickness;
        }
        if (j == 32) {
          endX = (((distance + wobble) * Trig_Cos(angle)) >> 16) - distance;
          endY = -(((height + wobble) * Trig_Sin(angle)) >> 16);
        }
      }

      shade = 0;
      if (frame > 33) {
        shade = (34 - frame) * 8;
      }
      if (shade > -64) {
        model = Render_AllocateModelList(1);
        model->kind = 6;
        model->texture = &texture;
        model->shade = shade;
        model->scrollV = 0;
        model->vertices = buffer;
        if (arc.head > arc.tail) {
          texture.logWidth = 6;
          texture.logHeight = 5;
          texture.pixels = work->sheet;
          model->mesh = strip;
          model->scrollU = 127 & ~(frame << 3);
          Render_ResetTransformState();
          Func_08015160((caster.x / 2 - 64) << 16,
                        (caster.y << 16) + 0xffc00000, 0);
          Func_080151e4(0x8000, 0x10000, 0x8000);
          if (effect->side == 1) {
            SceneTransform_ApplyYaw(0x8000);
          }
          Func_0801521c(0x40000);
          Render_TransformVertices(corners, buffer, 66);
          Render_DrawModelList(model);

          // The claw at the arc's end.
          if (frame <= 22) {
            model->mesh = gData_08199340;
            texture.logWidth = 6;
            texture.logHeight = 6;
            model->scrollU = 0;
            texture.pixels = work->sheet + 0x4000;
            Func_08015160(endX << 16, endY << 16, 0);
            Func_080150e4(0x4000 - arc.head * 32);
            Func_0801521c(width * 28 / 32);
            Render_TransformVertices(Data_081991e0, buffer, 4);
            Render_DrawModelList(model);
          }
        }
        Heap_Release(model);
      }
    }
    BattleEffect_CountDownBattlerTints();
    BattleCamera_Shake(effect->variant * 2 + 2, effect->variant * 2 + 2);
    work->transferPending = 1;
    wait(1);
  }
  HeapSlot_Release(SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
