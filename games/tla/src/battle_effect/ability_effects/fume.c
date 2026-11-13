/// @file
/// Fume.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

struct FxPartyList;
struct Model;
struct StripQuad;

struct Corner {
  s8 x;
  s8 y;
  s8 z;
};

/* VIEW(f17c6ac): tag struct RenderModel as struct RenderModel__f17c6ac;
 * render_model.h has another definition; tried with the definition the rest of
 * the object has, BattleEffect_RunFume moves: 2 of 625 instructions in tla-ja.
 */
struct RenderModel__f17c6ac {
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
void BattleEffect_CountDownBattlerTints(void);
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter
void BattleEffect_TintBattler(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                              s32 timerFrames);
void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void BattleEffect_RunDjinniEntrance(struct BattleEffectArgument *effect,
                                    u32 packed, s32 *screen, s32 *world);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);
void SceneTransform_ApplyPosition3(s32 x, s32 y, s32 z);
void SceneTransform_ApplyUniformScale(s32 scale);
void Transform_ApplyPitch(s32 angle);
void Transform_ApplyYaw(s32 angle);
void SceneTransform_ApplyScale3(s32 x, s32 y, s32 z);
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets
void Transform_ApplyRoll(s32 angle);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);

void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);

struct StarTarget {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};
void BattleMotion_ApplyKnockbackFar(
    s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void BattlePlayback_StartFar(s32 phase);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);

void *Resource_GetTableEntry(s32 resource);

void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);

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
  struct RenderModel__f17c6ac *model;
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
      Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
      BattlePlayback_StartFar(144);
      work->shakeFrames = 8;
      BattleEffect_StartActorMotion(effect->actors[0], 1, 0x20000, 0x40000,
                                    0x20000, 120);
      BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 20);
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
          Transform_Reset();
          SceneTransform_ApplyPosition3((caster.x / 2 - 64) << 16,
                                        (caster.y << 16) + 0xffc00000, 0);
          SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
          if (effect->side == 1) {
            Transform_ApplyYaw(0x8000);
          }
          SceneTransform_ApplyUniformScale(0x40000);
          Render_TransformVertices(corners, buffer, 66);
          Render_DrawModelList(model);

          // The claw at the arc's end.
          if (frame <= 22) {
            model->mesh = gData_08199340;
            texture.logWidth = 6;
            texture.logHeight = 6;
            model->scrollU = 0;
            texture.pixels = work->sheet + 0x4000;
            SceneTransform_ApplyPosition3(endX << 16, endY << 16, 0);
            Transform_ApplyRoll(0x4000 - arc.head * 32);
            SceneTransform_ApplyUniformScale(width * 28 / 32);
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
#undef SLOT_BLITTER
#undef BLITTER
#undef HI
#undef FUME_FRAMES
#undef FUME_SEGMENTS
