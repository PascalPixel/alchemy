/// @file
/// Fire Puppet and Whiplash.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

struct FxPartyList;
struct Model;
struct StripQuad;

/* VIEW(f17bd88): tag struct ModelTexture as struct ModelTexture__f17bd88;
 * render_model.h has another definition; tried with the definition the rest of
 * the object has, BattleEffect_RunFirePuppet moves: 1 of 541 instructions in
 * tla-ja. */
struct ModelTexture__f17bd88 {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const u8 *pixels;
};

/* VIEW(f17bd88): tag struct RenderModel as struct RenderModel__f17bd88;
 * render_model.h has another definition; tried with the definition the rest of
 * the object has, BattleEffect_RunFirePuppet moves: 2 of 541 instructions in
 * tla-ja. */
struct RenderModel__f17bd88 {
  long kind; // long: see the tag at the model's stores
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct ModelTexture__f17bd88 *texture;
  long shade; // long: see the tag at the model's stores
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
void Picture_MirrorIntoQuadrants(u8 *src, u8 *dst, s32 width, s32 height);
void BattleEffect_RunDjinniEntrance(struct BattleEffectArgument *effect,
                                    u32 packed, s32 *screen, s32 *world);
void Canvas_BlitAtFoot(void *cells, s32 x, s32 y, s32 size);
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
void BattleMotion_ApplyKnockbackFar(
    s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void BattlePlayback_StartFar(s32 phase);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);
/* VIEW(f17bd88): routine BattleObject_GetFar as void
 * **BattleObject_GetFar(s32 objectId); motobj.h has struct BattleObj
 * *BattleObject_GetFar(s32 objectId); tried as the 11 declarations' void
 * **BattleObject_GetFar(s32 objectId), it does not compile in tla-ja:
 * games/tla/src/battle_effect/ability_effects.c:27753: conflicting types for
 * `BattleObject_GetFar'. */
extern void **
GetBattleObjectSlotFar__f17bd88(s32 objectId) __asm__("BattleObject_GetFar");

void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void Transform_Pop(void);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

#define BLITTER(mode, flags) ((mode) << 4 | (flags))

// The target's motion object, as the effect reads it.
struct PuppetTarget {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 unk14;
  s32 unk18;
  s32 unk1c;
};

extern char ResourceId_Data0D3;
extern char ResourceId_Data134;
extern char ResourceId_Data0F2;
extern char ResourceId_MercuryDjinnSheet;
extern u8 Data_02011000[];
extern u8 Data_02012000[];
extern const u8 gData_08199340[]; // the hand's mesh
extern const u8 Data_081991e0[];  // its corners

#define FIRE_PUPPET_FRAMES 103
#define PUPPET_SEGMENTS 33

// Abilities: Fire Puppet.
//
// From the caster rises a flaming arm (33 segments of two vertices each, a
// scrolling textured strip) that reaches up and over to 1424 units, with a
// hand at its end; at frame 54 it strikes, hitting every target, and from
// frame 74 it shrinks back. The target's position is kept in the work's
// last particle.
void BattleEffect_RunFirePuppet(struct BattleEffectArgument *effect) {
  u8 *buffer;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sprites;
  s32 *camera;
  struct PuppetTarget *target;
  struct EffectPosition pos;
  struct ModelTexture__f17bd88 texture;
  struct RenderModel__f17bd88 *model;
  s32 endX;
  s32 endY;
  s32 i;

  buffer = Data_02012000;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(
        SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  } else {
    BattleEffect_LoadBlitter(
        SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                            COMPOSITE_FLIP_X));
  }
  REG_BLDALPHA = 0x1010;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  target = *GetBattleObjectSlotFar__f17bd88(effect->actors[0]);
  // Default: set for real below, when j reaches 32.
  endX = 0;
  endY = 0;
  work->particles[63].x = target->x;
  work->particles[63].y = target->y;
  work->particles[63].z = target->z;
  work->particles[63].velocityX = target->unk18;
  work->particles[63].velocityY = target->unk1c;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0D3, work->sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0F2, work->sheet + 0x4000, 0,
                             0);
  Iwram_CopyWordsEntry(
      (void *)BG_PLTT,
      Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet), 128);
  REG_BG2PA = 0x80;
  Render_BuildQuadStrip(buffer, 4, 32, 32);

  for (frame = 0; frame != FIRE_PUPPET_FRAMES; frame++) {
    Transform_Reset();
    Transform_SetLookAt(camera, camera + 3);
    if (frame == 0) {
      BattleEffect_TintBattler(effect->actor, 6, 4, 0, -1);
    }
    if (frame == 8) {
      Sound_PlayFar(103);
    }
    if (frame == 100) {
      BattleEffect_TintBattler(effect->actor, 0, 0, 0, -1);
    }
    if (frame == 54) {
      Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
      Sound_PlayFar(212);
      BattlePlayback_StartFar(212);
      work->shakeFrames = 8;
      for (i = 0; i != effect->count; i++) {
        BattleEffect_StartActorMotion(effect->actors[i], 1, 0x20000, 0x40000,
                                      0x40000, 150);
        BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 8);
      }
    }
    if (frame > 15) {
      s32 length;
      s32 j;
      // The fixed address, not the symbol: loop.c steps the row offset alone.
      struct ModelVertex(*cells)[2] =
          (struct ModelVertex(*)[2])Ram_MapCellBuffer;
      u8 *projected = Data_02011000;
      s32 shade;

      length = frame * 44 - 704;
      if (length > 1424) {
        length = 1424;
      }
      if (frame > 73) {
        length = 4680 - frame * 44;
      }

      // The arm's vertices, thicker at its root.
      for (j = 0; j != PUPPET_SEGMENTS; j++) {
        s32 width = 50 - ((Trig_Sin(j * 862 + 4310) * 44) >> 16);
        s32 wobble = ((Trig_Sin((frame + j) << 11) << 4) >> 16) * j / 32;
        s32 outer;

        i = 0;
        outer = width + wobble;
        do {
          cells[j][i].x =
              ((Trig_Cos(j * length) * (i * width + wobble + 16)) >> 16) -
              outer / 2 - 16;
          cells[j][i].y =
              -((Trig_Sin(j * length) * (i * width + j + wobble + 32)) >> 16);
          cells[j][i].z = 0;
          i++;
        } while (i != 2);
        if (j == 32) {
          endX = ((Trig_Cos(length << 5) * (width / 2 + wobble + 16)) >> 16) -
                 (width + wobble) / 2 - 16;
          endY =
              -((Trig_Sin(length << 5) * (width / 2 + j + wobble + 32)) >> 16);
        }
      }

      model = Render_AllocateModelList(1);
      shade = 0;
      texture.logWidth = 7;
      texture.logHeight = 5;
      texture.pixels = work->sheet;
      /* FAKEMATCH(signature): RenderModel's kind and shade typed long, whose
       * alias set lets sched2 store mesh and vertices first; as s32, as every
       * other RenderModel has them, 4 of 543 instructions differ in tla-en. */
      model->kind = 6;
      model->mesh = buffer;
      model->vertices = projected;
      model->shade = shade;
      model->texture = &texture;
      EffectPosition_ProjectScaledBattler(effect->actor, &pos);
      model->scrollV = 0;
      model->scrollU = 127 & ~(frame << 2);
      Transform_Reset();
      SceneTransform_ApplyPosition3((pos.x / 2 - 64) << 16,
                                    (pos.y << 16) + 0xffd60000, 0);
      SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
      if (effect->side == 1) {
        Transform_ApplyYaw(0x8000);
      }
      Transform_ApplyRoll(0x4000);
      if (frame > 73) {
        SceneTransform_ApplyUniformScale((212 << 10) - (frame << 11));
      }
      // The buffers named directly, though cells and projected hold them:
      // with the locals, gcse's table changes size and 8 differ.
      Render_TransformVertices(Ram_MapCellBuffer, Data_02011000, 66);
      Render_DrawModelList(model);

      // The hand at the arm's end.
      model->mesh = gData_08199340;
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->vertices = projected;
      model->scrollU = 0;
      model->scrollV = 0;
      texture.pixels = work->sheet + 0x4000;
      model->shade = 0;
      SceneTransform_ApplyPosition3(endX << 16, endY << 16, 0);
      Transform_ApplyRoll(22384 - (length << 5));
      SceneTransform_ApplyUniformScale(length * 23 + 0x8000);
      Render_TransformVertices(Data_081991e0, projected, 4);
      Render_DrawModelList(model);
      Heap_Release(model);
    }
    BattleEffect_CountDownBattlerTints();
    BattleCamera_Shake(4, 4);
    work->transferPending = 1;
    wait(1);
  }
  HeapSlot_Release(SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#define HEAP_SLOT_BLITTER 104 // Ram_HeapSlots->unk64[1]

// The Lost Age's blitter modes run the other way from The Broken Seal's:
// additive 0, max 1, transparent 2, copy 3; 1 << 4 | 3 is max and clipped,
// and | 4 mirrors it.
#define BLITTER_MAX 1

#define CLIP_XY 3
#define FLIP_X 4

#define SWING_FRAMES 52
#define SWING_HIT_FRAME 18
#define SPARK_COUNT 256

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

extern char ResourceId_Data0F3;
extern char ResourceId_EffectPalette02;

// The offsets of the spark cels: cel n is n pixels wide and 2n tall.
extern u16 gBattleEffect_MoteCells[];

// A swing. A five-cel slash sweeps across the caster's front, six frames a
// cel, mirrored on the far side; at frame 18 the target is knocked back and
// flashes, the result phase starts, and 256 sparks spray from the slash's
// end toward the target, half of them falling and half rising as they burn
// down.
void BattleEffect_RunWhiplash(struct BattleEffectArgument *effect) {
  struct HeapSlots *heap;
  struct BattleEffectWork *work;
  u8 *canvas;
  struct EffectPosition caster;
  s32 frame;
  struct EffectStep *spark;
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  u8 *sparkCels;
  s32 i;

  heap = Ram_HeapSlots;
  work = (struct BattleEffectWork *)heap->battleFxWork[0];
  canvas = heap->battleFxWork[1];
  sparkCels = heap->unk64[0];

  BattleEffect_StartCanvasLayer(1);
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(HEAP_SLOT_BLITTER, BLITTER(BLITTER_MAX, CLIP_XY));
    draw[0] = (DrawRectangle)heap->unk64[1];
  } else {
    BattleEffect_LoadBlitter(HEAP_SLOT_BLITTER,
                             BLITTER(BLITTER_MAX, CLIP_XY | FLIP_X));
    draw[0] = (DrawRectangle)heap->unk64[1];
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sparkCels, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0F3, work->sheet, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_EffectPalette02),
                       128);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != SWING_FRAMES; frame++) {
    // The slash, its cels anchored to the caster's front.
    EffectPosition_ProjectBattler(effect->actor, &caster);
    if (effect->side == 0) {
      switch (frame / 6) {
      case 0:
        draw[0](canvas, work->sheet, caster.x / 2 - 48, caster.y - 120, 48, 96);
        break;
      case 1:
        draw[0](canvas, work->sheet + 0x1200, caster.x / 2 - 38, caster.y - 120,
                38, 92);
        break;
      case 2:
        draw[0](canvas, work->sheet + 0x1fa8, caster.x / 2 - 38, caster.y - 100,
                30, 76);
        break;
      case 3:
        draw[0](canvas, work->sheet + 0x2890, caster.x / 2 - 64, caster.y - 88,
                56, 96);
        break;
      case 4:
        draw[0](canvas, work->sheet + 0x3d90, caster.x / 2 - 64, caster.y - 88,
                56, 88);
        break;
      }
    } else {
      switch (frame / 6) {
      case 0:
        draw[0](canvas, work->sheet, caster.x / 2, caster.y - 120, 48, 96);
        break;
      case 1:
        draw[0](canvas, work->sheet + 0x1200, caster.x / 2, caster.y - 120, 38,
                92);
        break;
      case 2:
        draw[0](canvas, work->sheet + 0x1fa8, caster.x / 2, caster.y - 100, 30,
                76);
        break;
      case 3:
        draw[0](canvas, work->sheet + 0x2890, caster.x / 2, caster.y - 88, 56,
                96);
        break;
      case 4:
        draw[0](canvas, work->sheet + 0x3d90, caster.x / 2, caster.y - 88, 56,
                88);
        break;
      }
    }

    // The hit, and the sparks sprayed from 48 pixels out.
    if (frame == SWING_HIT_FRAME) {
      BattleMotion_ApplyKnockbackFar(effect->actors[0], 1);
      BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 16);
      BattlePlayback_StartFar(134);
      work->shakeFrames = 8;
      for (i = 0; i != SPARK_COUNT; i++) {
        s32 angle;
        s32 speed;

        spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];
        if (effect->side == 0)
          spark->x = (caster.x / 2 - 48) << 16;
        else
          spark->x = (caster.x / 2 + 48) << 16;
        spark->y = (caster.y - (Random16() & 7)) << 16;
        angle = (Random16() & 0x7fff) + 0x4000;
        speed = Random16() & 255;
        if (effect->side == 0)
          spark->velocityX = ((Trig_Sin(angle) * speed) >> 4) - (8 << 16);
        else
          spark->velocityX = ((Trig_Sin(angle) * speed) >> 4) + (8 << 16);
        spark->velocityY = (Trig_Cos(angle) * (speed + 64)) >> 5;
        spark->variant = (Random16() & 15) + 16;
      }
    }
    if (frame > SWING_HIT_FRAME - 1) {
      for (i = 0; i != SPARK_COUNT; i++) {
        spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];
        if (spark->variant >= 0) {
          s32 size = (spark->variant >> 3) + 1;

          draw[0](canvas, sparkCels + gBattleEffect_MoteCells[size - 1],
                  HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
          if (i & 1)
            BattleEffectKernels_IntegrateVector2(spark, 60, -0x2000);
          else
            BattleEffectKernels_IntegrateVector2(spark, 60, 0x1000);
          spark->variant--;
        }
      }
    }

    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
#undef SLOT_BLITTER
#undef BLITTER
#undef FIRE_PUPPET_FRAMES
#undef PUPPET_SEGMENTS
#undef HEAP_SLOT_BLITTER
#undef BLITTER_MAX
#undef CLIP_XY
#undef FLIP_X
#undef SWING_FRAMES
#undef SWING_HIT_FRAME
#undef SPARK_COUNT
#undef HI
