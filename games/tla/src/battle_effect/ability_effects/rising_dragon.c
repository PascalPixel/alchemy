/// @file
/// Rising Dragon and Heartbreak.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

struct FxPartyList;
struct Model;
struct StripQuad;

/* VIEW(f178710): tag struct RenderModel as struct RenderModel__f178710;
 * render_model.h has another definition; tried with the definition the rest of
 * the object has, BattleEffect_RunRisingDragon moves: 1 of 882 instructions in
 * tla-ja. */
struct RenderModel__f178710 {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct ModelTexture *texture;
  long shade; // long: its store orders after texture's, as an s32 one does not
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
void Func_0815585c(struct BattleEffectArgument *effect, u32 packed, s32 *screen,
                   s32 *world);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyScale3(s32 x, s32 y, s32 z);
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets
void SceneTransform_ApplyRoll(s32 angle);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void BattleMotion_ApplyKnockbackFar(
    s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void BattlePlayback_StartFar(s32 phase);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);
/* VIEW(f178710): routine GetBattleObjectSlotFar as void
 * **GetBattleObjectSlotFar(s32 objectId); f175f74 has struct ActorPos
 * **GetBattleObjectSlotFar(s32); tried as the 11 declarations' void
 * **GetBattleObjectSlotFar(s32 objectId), it does not compile in tla-ja:
 * games/tla/src/battle_effect/ability_effects.c:25232: conflicting types for
 * `GetBattleObjectSlotFar'. */
extern void **
GetBattleObjectSlotFar__f178710(s32 objectId) __asm__("GetBattleObjectSlotFar");
void *Resource_GetTableEntry(s32 resource);

void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void Transform_Pop(void);

// On every strike after the first, the target's motion goes to this percent of
// its anchor's x, lowered from patch 1.
#if TLA_PATCH_1
#define TARGET_ANCHOR_PERCENT 200
#else
#define TARGET_ANCHOR_PERCENT 300
#endif

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1]) // the pixel half of a 16.16 value

extern char ResourceId_SmokeBandSheet;
extern char ResourceId_Data134;
extern char ResourceId_ClawSlashSheet;
extern char ResourceId_BlueFlameSheet;
extern u8 Data_02011000[];
extern u8 Data_02012000[];
extern u16 gBattleEffect_MoteCells[];
extern const u8 gData_08199340[]; // a head's mesh
extern const u8 Data_081991e0[];  // its corners

#define DRAGON_SEGMENTS 33

// A strip's vertex, as Render_TransformVertices reads it.
struct StripVertex {
  s8 x;
  s8 y;
  s8 z;
  s8 unk3;
};
/* VIEW(f178710): variable gMapCellBuffer as extern struct StripVertex
 * gMapCellBuffer[2][66]; f175f74 has extern u8 gMapCellBuffer[]; tried as the
 * rest of the object declares it, BattleEffect_RunRisingDragon moves: 93 of 882
 * instructions in tla-ja. */
extern struct StripVertex
    gMapCellBuffer__f178710[2][66] __asm__("gMapCellBuffer");

// Abilities: Rising Dragon.
//
// One dragon (two at a higher level: effect->variant + 1) of smoke rises
// from the caster and arcs onto the target, each a scrolling textured strip
// of 33 segments with a clawed head; each strikes ten frames after the
// last, scattering 32 sparks.
void BattleEffect_RunRisingDragon(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  // Unused: blit[1] gives the stack frame its 8 bytes at sp+68 as one DImode
  // pseudo.
  DrawRectangle blit[2];
  s32 frame;
  s32 *camera;
  u8 *sprites;
  void *caster;
  s32 arcs[4];
  struct EffectPosition source;
  struct EffectPosition target;
  struct EffectPosition hit;
  struct ModelTexture texture;
  struct RenderModel__f178710 *model;
  void *vertices;
  struct EffectStep *spark;
  struct EffectStep *mote;
  s32 i;
  s32 j;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(
        SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  } else {
    BattleEffect_LoadBlitter(
        SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                            COMPOSITE_FLIP_X));
  }
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  REG_BLDALPHA = 0x1010;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  {
    s32 endX[2] = {0, 0};
    s32 endY[2] = {0, 0};
    caster = *GetBattleObjectSlotFar__f178710(effect->actor);
    Actor_SetAnimationSpeedFar(caster, 48);
    Actor_SetMode(caster, 2);
    EffectPosition_ProjectBattler(effect->actor, &source);
    EffectPosition_ProjectBattler(effect->actors[0], &target);
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeBandSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_ClawSlashSheet,
                               work->sheet + 0x4000, 1, 1);
    Iwram_CopyWordsEntry(
        (void *)BG_PLTT,
        Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet), 128);
    REG_BG2PA = 0x80;
    Render_BuildQuadStrip(Data_02012000, 4, 32, 32);

    for (frame = 0; frame != effect->variant * 10 + 60; frame++) {
      Render_ResetTransformState();
      Transform_SetLookAt(camera, camera + 3);
      if (frame == 0) {
        arcs[0] = 0;
        arcs[1] = 0;
        arcs[2] = 0;
        arcs[3] = 0;
        for (i = 0; i != 64; i++) {
          work->particles[i].variant = -1;
        }
      }

      // Each dragon's strike.
      for (i = 0; i != effect->variant + 1; i++) {
        if (frame != i * 10 + 28) {
          continue;
        }
        EffectPosition_ProjectBattler(effect->actors[0], &hit);
        for (j = 0; j != 32; j++) {
          s32 angle;
          s32 speed = 255;

          spark = &work->particles[i * 32 + j];

          spark->x = (hit.x / 2) << 16;
          spark->y = (hit.y - 16) << 16;
          angle = Random16();
          speed &= Random16();
          spark->velocityX = (Trig_Sin(angle) * (speed += 127)) >> 6;
          spark->velocityY = (Trig_Cos(angle) * speed) >> 6;
          spark->variant = (Random16() & 15) + 16;
        }
        Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
        work->shakeFrames = 8;
        if (frame == 28) {
          if (effect->variant == 0) {
            BattlePlayback_StartFar(144);
          } else {
            Sound_PlayFar(144);
          }
          BattleEffect_StartActorMotion(effect->actors[0], 1, 0x20000, 0x40000,
                                        0x40000, 150);
        } else {
          BattlePlayback_StartFar(144);
          BattleEffect_StartActorMotion(effect->actors[0], 1, 0x20000, 0x40000,
                                        0x70000, TARGET_ANCHOR_PERCENT);
        }
        BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 8);
      }
      for (i = 0; i != 64; i++) {
        mote = &work->particles[i];
        if (mote->variant >= 0) {
          s32 size = (mote->variant >> 3) + 2;

          blit[0](canvas, sprites + gBattleEffect_MoteCells[size - 1],
                  HI(mote->x) - size / 2, HI(mote->y) - size, size, size * 2);
          BattleEffectKernels_IntegrateVector2(mote, 60, 0x1000);
          mote->variant--;
        }
      }

      if (frame > 15) {
        struct StripVertex *strip0 = gMapCellBuffer__f178710[0];
        s32 scale = 20768;
        const u8 *stripMesh = Data_02012000;

        // The arcs' heads and tails, 44 units a frame.
        arcs[0] = frame * 44 - 704;
        if (arcs[0] > 950) {
          arcs[0] = 950;
        }
        if (frame > 31) {
          arcs[1] = frame * 44 - 1408;
          if (arcs[1] > 950) {
            arcs[1] = 950;
          }
        }
        if (frame > 29) {
          arcs[2] = frame * 44 - 1320;
          if (arcs[2] > 950) {
            arcs[2] = 950;
          }
        }
        if (frame > 45) {
          arcs[3] = frame * 44 - 2024;
          if (arcs[3] > 950) {
            arcs[3] = 950;
          }
        }

        // The first dragon arcs over, the second under.
        for (j = 0; j != DRAGON_SEGMENTS; j++) {
          s32 distance = source.x / 4 - target.x / 4;
          s32 angle = j * (arcs[0] - arcs[1]) + arcs[1] * 33;
          s32 height;
          s32 width;
          s32 start;

          if (distance < 0) {
            distance = -distance;
          }
          height = distance / 3;
          width = j * 2 + 16;
          if (width > 32) {
            width = 32;
          }
          width = width * scale / 0x10000;
          start = -(width / 2);
          for (i = 0; i != 2; i++) {
            struct StripVertex *corner = strip0 + (j * 2 + i);

            corner->x =
                ((Trig_Cos(angle) * (distance + start + i * width)) >> 16) -
                distance;
            corner->y =
                -((Trig_Sin(angle) * (height + start + i * width)) >> 16);
            corner->z = 0;
          }
          if (j == 32) {
            endX[0] = ((Trig_Cos(angle) * distance) >> 16) - distance;
            endY[0] = -((Trig_Sin(angle) * height) >> 16);
          }
        }
        for (j = 0; j != DRAGON_SEGMENTS; j++) {
          s32 distance = source.x / 4 - target.x / 4;
          s32 angle = j * (arcs[2] - arcs[3]) + arcs[3] * 33;
          s32 height;
          s32 width;
          s32 start;

          if (distance < 0) {
            distance = -distance;
          }
          height = distance * 32 / 36;
          width = j * 4;
          if (width > 24) {
            width = 24;
          }
          width = width * scale / 0x10000;
          start = -(width / 2);
          for (i = 0; i != 2; i++) {
            struct StripVertex *strip1 =
                (struct StripVertex *)Ram_MapCellBuffer + 66;
            struct StripVertex *corner = strip1 + (j * 2 + i);

            corner->x =
                ((Trig_Cos(angle) * (distance + start + i * width)) >> 16) -
                distance;
            corner->y = (Trig_Sin(angle) * (height + start + i * width)) >> 16;
            corner->z = 0;
          }
          if (j == 32) {
            endX[1] = ((Trig_Cos(angle) * distance) >> 16) - distance;
            endY[1] = (Trig_Sin(angle) * height) >> 16;
          }
        }

        model = Render_AllocateModelList(1);
        vertices = Data_02011000;
        model->scrollV = 0;
        model->kind = 6;
        model->texture = &texture;
        model->shade = 0;
        model->vertices = vertices;
        for (i = 0; i != effect->variant + 1; i++) {
          if (arcs[i * 2 + 1] >= arcs[i * 2]) {
            continue;
          }
          texture.logWidth = 6;
          texture.logHeight = 5;
          texture.pixels = work->sheet;
          model->mesh = stripMesh;
          model->scrollU = 127 & ~(frame << 3);
          Render_ResetTransformState();
          if (effect->side == 0) {
            Func_08015160((source.x / 2 - 68) << 16,
                          (source.y << 16) - 0x600000, 0);
          } else {
            Func_08015160((source.x / 2 - 52) << 16,
                          (source.y << 16) - 0x600000, 0);
          }
          SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
          if (effect->side == 1) {
            SceneTransform_ApplyYaw(0x8000);
          }
          SceneTransform_ApplyRoll(i * 14192 - 0x1000);
          Func_0801521c(0x40000);
          Render_TransformVertices(strip0 + i * 66, vertices, 66);
          Render_DrawModelList(model);
          if (frame < i * 16 + 40) {
            texture.logWidth = 6;
            texture.logHeight = 6;
            model->scrollU = 0;
            model->mesh = gData_08199340;
            texture.pixels = work->sheet + 0x4000;
            Func_08015160(endX[i] << 16, endY[i] << 16, 0);
            SceneTransform_ApplyRoll(0x3060 - (arcs[i * 2] << 5));
            Func_0801521c(20768);
            Render_TransformVertices(Data_081991e0, vertices, 4);
            Render_DrawModelList(model);
          }
        }
        Heap_Release(model);
      }
      BattleEffect_CountDownBattlerTints();
      BattleCamera_Shake(2, 2);
      work->transferPending = 1;
      wait(1);
    }
    Actor_SetAnimationSpeedFar(caster, 16);
    HeapSlot_Release(104);
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  }
  BattleEffect_EndCanvasLayer();
}

#define HEAP_SLOT_BLITTER 104           // Ram_HeapSlots->unk64[1]
#define HEAP_SLOT_BLITTER_ALTERNATE 188 // Ram_HeapSlots->unkb8[1]

// The Lost Age's blitter modes run the other way from The Broken Seal's:
// additive 0, max 1, transparent 2, copy 3; 2 << 4 | 3 is transparent and
// clipped, and | 4 mirrors it.
#define BLITTER_TRANSPARENT 2

#define CLIP_XY 3
#define FLIP_X 4

#define FIGURE_FRAMES 108
#define FIGURE_SWING_FRAME 30 // the first blow
#define FIGURE_HIT_FRAME 80   // the second, which starts the result phase

// The parts of the figure in Resource_Data0DC, unpacked into the map cells.
#define FIGURE_BODY 0             // 96x83
#define FIGURE_ARM_LOWERED 0x1f20 // 42x35
#define FIGURE_ARM_RAISED 0x24de  // 48x51
#define FIGURE_SWING 0x2e6e       // four cels, Data_081993d0 to d8
#define FIGURE_FIST 0x4e9d        // 32x29, then eight cels in Data_081993e0..f0

extern char ResourceId_Data0DC;

// The swing's cels: widths, heights and offsets; the fist's: heights,
// y offsets and offsets.
// Plain externs: const changes the code that reads them.
extern u8 gData_081993d0[];
extern u8 gData_081993d4[];
extern u16 gData_081993d8[];
extern u8 gData_081993e0[];
extern u8 gData_081993e8[];
extern u16 gData_081993f0[];

void BattleBackground_LoadFar(s32 layer, s32 background, s32 frames);

// A great figure strikes. It sweeps down on the target from above the far
// side, swaying on a small circle and fading in through the blend; its arm
// is raised, and at frame 30 it swings down through four cels and the
// screen flashes white; from frame 40 its fist falls through eight cels,
// and at frame 80 the blow lands and the result phase starts. From frame
// 92 it fades out and rises away; the battle background is reloaded at the
// end. On the far side the whole figure is drawn mirrored.
void BattleEffect_RunHeartbreak(struct BattleEffectArgument *effect) {
  struct HeapSlots *heap;
  struct BattleEffectWork *work;
  u8 *canvas;
  u8 *parts;
  DrawRectangle draw[2];
  struct EffectPosition target;
  s32 frame;
  s32 x;
  s32 y;

  heap = Ram_HeapSlots;
  work = (struct BattleEffectWork *)heap->battleFxWork[0];
  canvas = heap->battleFxWork[1];
  parts = Ram_MapCellBuffer;
  BattleEffect_StartTiledCanvas(0x2001);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DC, parts, 1, 1);
  BattleEffect_LoadBlitter(HEAP_SLOT_BLITTER,
                           BLITTER(BLITTER_TRANSPARENT, CLIP_XY));
  draw[0] = (DrawRectangle)heap->unk64[1];
  BattleEffect_LoadBlitter(HEAP_SLOT_BLITTER_ALTERNATE,
                           BLITTER(BLITTER_TRANSPARENT, CLIP_XY | FLIP_X));
  draw[1] = ((DrawRectangle *)heap)[47];
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  EffectPosition_ProjectScaledBattler(effect->actors[0], &target);

  for (frame = 0; frame != FIGURE_FRAMES; frame++) {
    // It starts 100 pixels above the canvas and to the target's far side,
    // and comes down at it for 32 frames.
    if (frame == 0) {
      if (effect->side == 0)
        work->particles[0].x = (target.x + 40) << 16;
      else
        work->particles[0].x = (target.x - 168) << 16;
      work->particles[0].y = -100 << 16;
      if (effect->side == 0)
        work->particles[0].velocityX = -6 << 16;
      else
        work->particles[0].velocityX = 6 << 16;
      work->particles[0].velocityY = 8 << 16;
      REG_BLDALPHA = 0x1000;
    }
    if (frame < 32)
      BattleEffectKernels_IntegrateVector2(&work->particles[0], 60, 0);

    // In over 32 frames, out over the last 16, rising away.
    if (frame <= 32) {
      s32 level = frame / 2;

      REG_BLDALPHA = ((16 - level / 2) << 8) | level;
    }
    if (frame > 91) {
      s32 level = FIGURE_FRAMES - frame;

      REG_BLDALPHA = ((16 - level / 2) << 8) | level;
      BattleEffectKernels_IntegrateVector2(&work->particles[0], 60, -0x8000);
    }

    // The sway, and the canvas moved with the figure.
    x = HI(work->particles[0].x) + ((Trig_Sin(frame << 8) << 3) >> 16);
    y = HI(work->particles[0].y) + ((Trig_Cos(frame << 8) << 3) >> 16);
    REG_BG2X = -x << 8;

    // The body, and the arm raised until frame 48.
    if (effect->side == 0) {
      draw[effect->side](canvas, Ram_MapCellBuffer + FIGURE_BODY, 25, y + 2, 96,
                         83);
      if ((u32)frame < 48)
        draw[effect->side](canvas, Ram_MapCellBuffer + FIGURE_ARM_RAISED, 31,
                           y + 22, 48, 51);
      else
        draw[effect->side](canvas, Ram_MapCellBuffer + FIGURE_ARM_LOWERED, 64,
                           y + 16, 42, 35);
    } else {
      draw[effect->side](canvas, Ram_MapCellBuffer + FIGURE_BODY, 7, y + 2, 96,
                         83);
      if ((u32)frame < 48)
        draw[effect->side](canvas, Ram_MapCellBuffer + FIGURE_ARM_RAISED, 49,
                           y + 22, 48, 51);
      else
        draw[effect->side](canvas, Ram_MapCellBuffer + FIGURE_ARM_LOWERED, 22,
                           y + 16, 42, 35);
    }

    // The swing, three frames a cel from frame 28.
    if (frame >= 28 && frame < 40) {
      s32 cel = (frame - 28) / 3;

      if (effect->side == 0) {
        draw[effect->side](
            canvas, Ram_MapCellBuffer + FIGURE_SWING + gData_081993d8[cel],
            46 - gData_081993d0[cel] / 2, y - gData_081993d4[cel] / 2 + 61,
            gData_081993d0[cel], gData_081993d4[cel]);
      } else {
        draw[effect->side](
            canvas, Ram_MapCellBuffer + FIGURE_SWING + gData_081993d8[cel],
            gData_081993d0[cel] / 2 - gData_081993d0[cel] + 82,
            y - gData_081993d4[cel] / 2 + 61, gData_081993d0[cel],
            gData_081993d4[cel]);
      }
    }

    // The fist: raised to frame 47, then falling six frames a cel from 56.
    if (frame > 39) {
      if (frame <= 47) {
        if (effect->side == 0)
          draw[effect->side](canvas, Ram_MapCellBuffer + FIGURE_FIST, 30,
                             y + 46, 32, 29);
        else
          draw[effect->side](canvas, Ram_MapCellBuffer + FIGURE_FIST, 66,
                             y + 46, 32, 29);
      } else {
        s32 cel = (frame - 56) / 6;

        if (cel < 0)
          cel = 0;
        if (cel <= 7) {
          if (effect->side == 0)
            draw[effect->side](
                canvas, Ram_MapCellBuffer + FIGURE_FIST + gData_081993f0[cel],
                63, y + gData_081993e8[cel] - 8, 32, gData_081993e0[cel]);
          else
            draw[effect->side](
                canvas, Ram_MapCellBuffer + FIGURE_FIST + gData_081993f0[cel],
                33, y + gData_081993e8[cel] - 8, 32, gData_081993e0[cel]);
        }
      }
    }

    if (frame == FIGURE_HIT_FRAME) {
      work->shakeFrames = 6;
      BattlePlayback_StartFar(134);
      BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 16);
    }
    if (frame == FIGURE_SWING_FRAME) {
      Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
      work->shakeFrames = 3;
      Sound_PlayFar(212);
      BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 16);
    }

    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  BattleBackground_LoadFar(1, *(u16 *)((u8 *)Ram_HeapSlots->battleWork + 0x670),
                           24);
  HeapSlot_Release(HEAP_SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
#undef TARGET_ANCHOR_PERCENT
#undef SLOT_BLITTER
#undef BLITTER
#undef HI
#undef DRAGON_SEGMENTS
#undef HEAP_SLOT_BLITTER
#undef HEAP_SLOT_BLITTER_ALTERNATE
#undef BLITTER_TRANSPARENT
#undef CLIP_XY
#undef FLIP_X
#undef FIGURE_FRAMES
#undef FIGURE_SWING_FRAME
#undef FIGURE_HIT_FRAME
#undef FIGURE_BODY
#undef FIGURE_ARM_LOWERED
#undef FIGURE_ARM_RAISED
#undef FIGURE_SWING
#undef FIGURE_FIST
