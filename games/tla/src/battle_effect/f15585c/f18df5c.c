/// @file
/// Meteor Blow.
// Part of f15585c.c.

#include "../f15585c_internal.h"

struct FxPartyList;
struct MeshTriangle;
struct Model;
struct StripQuad;

/* VIEW(f18df5c): tag struct ModelTexture as struct ModelTexture__f18df5c; render_model.h has another definition; tried with the definition the rest of the object has, BattleEffect_RunMeteorBlow moves: 1 of 1545 instructions in tla-ja. */
struct ModelTexture__f18df5c {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const u8 *pixels;
};

struct PointPair {
  s32 x;
  s32 y;
};

/* VIEW(f18df5c): tag struct RenderModel as struct RenderModel__f18df5c; render_model.h has another definition. */
struct RenderModel__f18df5c {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct ModelTexture__f18df5c *texture;
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
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);

void *Resource_GetTableEntry(s32 resource);

void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height, s32 count);
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

#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1])

// One star of the opening sky: a point on a sphere and its cell kind.
struct MeteorStar {
  s32 x;
  s32 y;
  s32 z;
  s32 kind;
};

extern char ResourceId_GlowOrbSetSheet;
extern char ResourceId_DustBlobSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_Data134;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_Data0DA;
extern char ResourceId_Data0C1;
extern u8 gMapCellBuffer[];
extern s32 Data_02010018[];         // the shards' variants, 28 bytes apart
extern u16 gData_08197410[]; // gBattleEffect_MoteCells
extern const struct PointPair Data_08196f20;
extern const u8 Data_08199e70[];
extern const u8 Data_08199e77[];
extern const u16 Data_08199e7e[];
extern const u8 Data_08199e8c[];
extern const u8 Data_08199e90[];
extern const u8 Data_08199e94[];
extern const u8 Data_08199e98[];
extern const u8 Data_08199e9c[];
extern const s8 Data_08199ea0[];
extern const u8 gData_081990d0[];
extern const u8 gData_08199090[];
extern const u8 Data_08198ec4[];
extern const u8 Data_08198cac[];

void BattleEffect_FlushPendingGraphicsTransfer(void);
void BattleEffect_ArmMeteorBlowPaletteDma(void);
void BattleEffect_ConfigureEffectDisplay(void);
void Func_08163c2c(s32 mode, s32 value);
void Graphics_LoadGreyRamp(void);
void Func_0814cca8(void);
void Func_0815f0a0(s32 palette);
void Func_08164abc(s32 red, s32 green, s32 blue);
void Func_0818de3c(struct MeshTriangle *triangles, s32 columns, s32 rows, s32 cellWidth, s32 cellHeight);
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags, u32 mode);
void SceneTransform_ApplyPosition(const s32 *pos);
s32 Render_ProjectPoint(const void *point, s32 *screen);


/// Meteor Blow.
/// ★ Named from the game: "Meteor Blow" (message 02130)
void BattleEffect_RunMeteorBlow(struct BattleEffectArgument *effect) {
  u8 *canvas;
  struct BattleEffectWork *work;
  u8 *map;
  // Unused: blit[1] gives the stack frame its 4 bytes.
  DrawRectangle blit[2];
  u8 *sprites;
  u8 *projected;
  u8 *meshA;
  u8 *triangles;
  s32 *transition;
  struct MeteorStar *stars;
  s32 pitch;
  s32 yaw;
  s32 x;
  s32 z;
  s32 size;
  s32 scale;
  s32 hx;
  s32 hz;
  void *verts;
  s32 x0;
  s32 half;
  s32 blank;
  s32 mode;
  s32 offset;
  s32 frame;
  s32 angle0;
  s32 j;
  s32 star;
  s32 i;
  s32 *starKind;
  struct MeteorStar *stars2;
  s32 position[3];
  s32 screen[3];
  s32 point[3];
  s32 place[4];
  s32 place2[4];
  struct ModelTexture__f18df5c texture2;
  struct PointPair req2;
  s32 req[2];
  struct ModelTexture__f18df5c texture;

  canvas = Ram_HeapSlots->battleFxWork[1];
  map = gMapCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  work->effect = effect;
  BattleEffect_StartCanvasLayer(0x2000);
  REG_BG2PA = 0x100;
  BattleEffect_ConfigureEffectDisplay();
  ((volatile u16 *)BG_PLTT)[0] = 0;
  ((volatile u16 *)BG_PLTT)[1] = 0;
  work->transferMode = 0;
  blank = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  Func_08163c2c(0, 0);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSetSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_DustBlobSheet,
                             work->sheet + 0x2000, 1, 1);
  Iwram_CopyWordsEntry(sprites, work->sheet + 0x1c2, 30);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
  Graphics_LoadGreyRamp();
  gCameraSceneParameters[4] = 240;
  wait(1);
  /* FAKEMATCH(wrapper): both fills forwarded through FillWords_, which leaves
   * 0x4000 without a pseudo of its own; as direct Iwram_FillWords calls, 286
   * of 1556 instructions differ in tla-en. */
  // A once-only block: it closes the fill's block before the copy.
  do {
    Iwram_FillWords(map, 0x8000, 0x01010101);
  } while (0);
  Iwram_CopyWordsEntry((void *)0x06008000, map, 0x7800);
  BattleEffect_SelectLivingTargets(work->effect);
  BattleEffect_SpawnObjects(1, 0x28a, 1);
  REG_BLDCNT = blank;
  REG_BG2PA = 0x100;
  REG_BG1CNT = 0x1f81;
  REG_BG2CNT = 0x2787;
  // A once-only block, as for the map's fill.
  do {
    Iwram_FillWords(canvas, 0x4000, 0);
  } while (0);
  Iwram_CopyWordsEntry((void *)0x06004000, canvas, 0x4000);
  Task_Add(BattleEffect_FlushPendingGraphicsTransfer, 3200);
  REG_BLDCNT = 0x3f46;
  REG_BLDALPHA = 0x1010;
  projected = work->sheet + 0x5740;
  meshA = work->sheet + 0x5f40;
  triangles = work->sheet + 0x6340;
  transition = Ram_HeapSlots->transitionWork;
  stars = (struct MeteorStar *)(work->sheet + 0x7a4);
  // Default: the flight sets these at frame 0.
  pitch = 0;
  yaw = 0;
  x = 0;
  z = 0;
  size = 0;
  scale = 0x10000;
  Func_0818de3c(triangles, 2, 16, 32, 2);
  for (offset = 0, frame = 0, angle0 = 0; frame != 16;
       angle0 += 1092, offset += 8, frame++) {
    s32 y = (-Trig_Cos(angle0) * 48) >> 16;
    s32 r = (Trig_Sin(angle0) * 48) >> 16;
    u8 *v;

    j = 0;
    v = meshA + offset;
    for (; j != 2; v += 4) {
      s32 c;

      v[0] = (Trig_Sin(j << 13) * r) >> 16;
      v[1] = y;
      c = (Trig_Cos(j << 13) * r) >> 16;
      j++;
      // A once-only block keeps j's reload after the multiply.
      do {
        v[2] = c;
      } while (0);
    }
  }
  mode = 3;
  Func_08196404(SLOT_BLITTER, 8, 7, mode, 1);
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  // DEAD: both fields are set again before anything reads them.
  work->transferMode = mode;
  work->transferValue = 0x04040404;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ArmMeteorBlowPaletteDma, 0xcfe);
  gBgScroll[1].x = 0;
  transition[4] = 1;
  starKind = &stars->kind;
  stars2 = stars;
  for (star = 0; star != 256; star++, starKind += 4, stars2++) {
    s32 a = (star & 255) << 7;
    s32 r = 160;
    s32 b;

    stars2->y = Trig_Sin(a) * r;
    r = (Trig_Cos(a) * r) >> 16;
    b = Random16() & 0x7fff;
    stars2->x = Trig_Sin(b) * r;
    stars2->z = Trig_Cos(b) * r;
    if ((star & 31) == 0) {
      *starKind = (star / 32) % 7;
    } else {
      *starKind = 7;
    }
  }

  // The flight through the stars.
  for (frame = 0; frame != 160 && !(gInput.repeat & 1) && !(gInput.repeat & 2);
       frame++) {
    s32 fade;
    s32 speed;

    if (frame == 0) {
      x = 0x1100000;
      z = 0x580000;
      size = 0x400;
      pitch = 0xd00;
      yaw = 0x2500;
      work->frame = 8;
      Sound_PlayFar(162);
    }
    if (frame == 120) {
      Sound_PlayFar(163);
    }
    fade = frame - 96;
    if ((u32)fade <= 16) {
      REG_BLDALPHA = (16 - fade) | 0x1000;
    }
    if (frame == 115) {
      REG_BLDALPHA = 0x1010;
    }
    if (frame == 116) {
      Task_Remove(BattleEffect_FlushPendingGraphicsTransfer);
      Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
      IO_WRITE_QUEUE_PUSH(0x04000020, 128, 0x20000);
      IO_WRITE_QUEUE_PUSH(0x0400000c, 0x784, 0x20000);
    }
    if (frame > 128) {
      Func_0815f0a0((s32)&ResourceId_OrangePaletteB);
    }
    if (frame == 117) {
      work->frame = 45;
      for (i = 0; i != 64; i++) {
        work->particles[i].variant = -1;
      }
      scale = 0x10000;
    }
    // DEAD: frame never reaches 1600 here; the source keeps the test.
    if (frame <= 1599) {
      u16 *dst = (u16 *)((u8 *)work + 0x7640);
      s32 value = work->frame;
      s32 base = value / 2 + 16;

      if (frame > 79) {
        work->frame++;
        if (work->frame > 236) {
          work->frame = 236;
        }
      }
      for (i = 0; i != 15; i++) {
        *dst++ = 0;
      }
      for (; i != 135; i++) {
        s32 g = i - 16;
        s32 b = g / 4;

        value = b + base;
        b = value - 32;
        g = value - 80;

        if (b < 0)
          b = 0;
        if (b > 31)
          b = 31;
        if (g < 0)
          g = 0;
        if (g > 31)
          g = 31;
        *dst++ = b << 10 | g << 5 | g >> 1;
      }
      for (; i != 160; i++) {
        *dst++ = 0;
      }
    }

    if (frame > 115) {
      struct RenderModel__f18df5c *model;
      s32 shade;
      s32 scroll;
      s32 angle;

      model = Render_AllocateModelList(1);
      shade = frame * 4 - 528;
      if (shade > 0) {
        shade = 0;
      }
      texture.logWidth = 5;
      texture.logHeight = 5;
      texture.pixels = work->sheet + 0x2000;
      model->kind = 6;
      model->texture = &texture;
      model->mesh = triangles;
      model->vertices = projected;
      model->shade = shade;
      model->scrollU = 0;
      if (frame > 127) {
        s32 d = (frame << 6) - 0x2000;

        if (d > 1023) {
          d = 1024;
        }
        scale += d;
        Func_080151e4(scale, scale, scale);
      }
      i = 0;
      angle = -0x4000;
      scroll = frame * 2;
      for (; i != 5; i++) {
        model->scrollV = ~scroll & 127;
        Render_ResetTransformState();
        Func_08015160(-0x20000, -0x400000, 0);
        Func_080151e4(42000, 0x24650, 0x10000);
        Func_080151e4(scale, scale, scale);
        Func_080150e4(0x8000);
        SceneTransform_ApplyYaw(angle);
        Render_TransformVertices(meshA, projected, 32);
        Render_DrawModelList(model);
        angle += 0x1800;
        scroll += 20;
      }
      Heap_Release(model);
    } else {
      // DEAD: frame is at most 115 here, so the second arm never runs; the
      // source keeps both tests.
      if (frame <= 159) {
        pitch = ((pitch << 16) + (256 << 16)) >> 16;
      } else if (frame <= 191) {
        pitch = (((u16)pitch - frame * 8 + 1216) << 16) >> 16;
      }
      if (frame <= 47) {
        yaw = ((yaw << 16) + (256 << 16)) >> 16;
      } else if (frame <= 111) {
        yaw = (((u16)yaw - frame * 4 + 448) << 16) >> 16;
      }
      position[0] = 120;
      position[1] = 64;
      position[2] = 0;
      Render_ResetTransformState();
      SceneTransform_ApplyPosition(position);
      SceneTransform_ApplyPitch((u16)pitch);
      SceneTransform_ApplyYaw((u16)yaw);
      if (frame <= 112) {
        struct MeteorStar *s = stars;

        for (i = 0; i != 256; i++, s++) {
          s32 kind = s->kind;

          point[0] = HI(s->x);
          point[1] = HI(s->y);
          point[2] = HI(s->z);
          Render_ProjectPoint(point, screen);
          if (screen[2] > 0) {
            if (kind == 7) {
              blit[0](gMapCellBuffer, sprites + i % 30, screen[0], screen[1], 1,
                      1);
            } else {
              blit[0](gMapCellBuffer, work->sheet + Data_08199e7e[kind],
                      screen[0], screen[1], Data_08199e70[kind],
                      Data_08199e77[kind]);
            }
          }
        }
      } else {
        Iwram_ClearWordsEntry(gMapCellBuffer, 0x7800);
      }
    }

    req[1] = size;
    req[0] = size;
    place[3] = 0;
    place[1] = 0xff0000;
    place[0] = x;
    place[2] = z;
    Render_ApplyProjectedPlacementFar(work->objects[0], place, req, 0);
    speed = 0x800;
    {
      s32 a = 0x6000;

      if (frame > 23) {
        a = a - frame * 286 + 0x11e0;
        if (a < 0) {
          a = 0;
        }
      }
      if (frame > 87) {
        speed = speed - (frame << 6) + 0x1600;
        if (speed < 0) {
          speed = 0;
        }
      }
      x -= (Trig_Sin(a) * speed) >> 10;
      z += (Trig_Cos(a) * speed) >> 10;
      size += 512;
    }
    if (frame > 127) {
      speed = (frame << 13) - 0x100000;
      if (speed > 0x1ffff) {
        speed = 0x20000;
      }
      z += speed;
      size += 512;
    } else if (size > 0x10000) {
      size = 0x10000;
    }
    work->transferPending = 1;
    wait(1);
  }

  wait(1);
  transition[4] = 0;
  Task_Remove(BattleEffect_ArmMeteorBlowPaletteDma);
  if (frame <= 115) {
    Task_Remove(BattleEffect_FlushPendingGraphicsTransfer);
  } else {
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  }
  HeapSlot_Release(SLOT_BLITTER);
  Func_0814cca8();
  // DEAD: frame 0 sets hx and hz again before any read.
  hx = 200;
  hz = 0;
  REG_BG2PA = 0x80;
  REG_BG2X = 0;
  REG_BG2Y = -0x1000;
  REG_BLDALPHA = 0x1010;
  REG_BG2CNT = 0x784;
  REG_WININ = 0x2737;
  REG_WINOUT = 0x721;
  REG_WIN0V = 0x107c;
  REG_WIN1V = 0x88;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, work->sheet + 0x4000, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, work->sheet + 0x5000, 0,
                             0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
  {
    s32 *q = (s32 *)Data_02010018;

    for (i = 0; i != 256; i++, q += 7) {
      *q = 0;
    }
  }
  Func_08164abc(5, -4, -12);

  // The impact: the meteor lands, rings of fire and shards fly.
  for (frame = 0; frame != 50; frame++) {
    struct RenderModel__f18df5c *model;
    s32 angle;
    s32 back;
    s32 shade;
    s32 step;

    if (frame == 0) {
      hx = 144;
      hz = 0;
    }
    if (frame <= 33) {
      req2 = Data_08196f20;
      place2[3] = 0;
      place2[1] = 0xff0000;
      place2[0] = hx << 16;
      place2[2] = hz << 16;
      if (frame <= 15) {
        hx -= 1;
        hz += 9;
      } else {
        hz += 2;
      }
      Render_ApplyProjectedPlacementFar(work->objects[0], place2, (s32 *)&req2,
                                        0);
    }
    if (frame == 34) {
      Func_081180e8(134);
    }
    if (frame == 16) {
      struct EffectStep *p;
      s32 count = 256;

      p = (struct EffectStep *)gMapCellBuffer;
      i = 0;
      half = (hx / 2) << 16;
      /* FAKEMATCH(dead): updates of hx that cancel out end the shard's halving
       * here, so GCSE does not carry its sign on to the hoop's hx / 2;
       * without them, 392 of 1556 instructions differ in tla-en. */
      hx++;
      hx--;
      for (; i != count; i++, p++) {
        s32 r;
        s32 a;

        /* FAKEMATCH(dead): updates of count, the shard count and the radius's
         * base, that cancel out and keep GCSE from folding 256 into the radius
         * add; without them, 74 of 1556 instructions differ in tla-en. */
        count++;
        count--;
        r = (Random16() & 511) + count;
        a = (Random16() & 0x7fff) + 0x4000;

        p->x = half;
        p->y = 0x720000;
        p->velocityX = (Trig_Sin(a) * r) >> 6;
        p->velocityY = (Trig_Cos(a) * r * 2) >> 6;
        p->variant = (Random16() & 7) + 24;
      }
      work->shakeFrames = 4;
      Sound_PlayFar(145);
      for (i = 0; i != effect->count; i++) {
        Func_08118088(effect->actors[i], 4);
        Func_0814cd48(effect->actors[i], 7, 5, i, 8);
      }
    }

    verts = Heap_AllocEwram(1024);
    model = Render_AllocateModelList(1);
    shade = 0;
    model->texture = &texture2;
    model->vertices = verts;
    model->shade = shade;
    texture2.logWidth = 6;
    texture2.logHeight = 6;
    texture2.pixels = work->sheet + 0x4000;
    model->kind = 7;
    model->mesh = gData_081990d0;
    x0 = hx / 2 - 60;
    step = frame << 11;
    i = 0;
    back = -step;
    angle = step;
    for (; i != 4; i++, back += 0x2000, angle += 0x2000) {
      s32 start = Data_08199e8c[i] + 16;

      if (frame > start) {
        shade = (start - frame) * 8 + 56;

        if (shade > -16) {
          shade = -16;
        }
        if (shade > -64) {
          s32 s = (frame - start) * Data_08199e90[i] * 2000 + 42768;

          model->shade = shade;
          Render_ResetTransformState();
          Func_08015160(x0 << 16, 0x280000, 0);
          Func_080151e4(s, s * Data_08199e94[i], s / 4);
          SceneTransform_ApplyPitch(1000);
          if (i & 1) {
            SceneTransform_ApplyYaw(angle);
          } else {
            SceneTransform_ApplyYaw(back);
          }
          Render_TransformVertices(gData_08199090, verts, 32);
          Render_DrawModelList(model);
        }
      }
    }
    model->mesh = Data_08198ec4;
    texture2.pixels = work->sheet + 0x5000;
    texture2.logWidth = 7;
    texture2.logHeight = 7;
    // hx / 2 written out: as hx / 2 it would share the hoop's sign through
    // GCSE, and through a copy of hx combine would add the sign first.
    half = ((hx + (s32)((u32)hx >> 31)) >> 1) - 64;
    for (i = 0; i != 3; i++) {
      s32 start = Data_08199e98[i] + 8;

      if (frame > start) {
        shade = (start - frame) * 8 + 40;

        if (shade > 0) {
          shade = 0;
        }
        if (shade > -64) {
          s32 s = (frame - start) * Data_08199e9c[i] * 1500 + 0x4180;

          model->shade = shade;
          model->scrollU = ((frame + i * 5) & 7) << 4;
          Render_ResetTransformState();
          Func_08015160(half << 16, Data_08199ea0[i] << 16, 0);
          Func_080151e4(s, s * 2, s);
          SceneTransform_ApplyPitch(2000);
          Render_TransformVertices(Data_08198cac, verts, 32);
          Render_DrawModelList(model);
        }
      }
    }
    Heap_Release(model);
    Heap_Release(verts);

    {
      struct EffectStep *p;

      p = (struct EffectStep *)gMapCellBuffer;
      for (i = 0; i != 256; i++, p++) {
        if (p->variant >= 0) {
          s32 n = (p->variant >> 2) + 1;

          blit[0](canvas, sprites + gData_08197410[n - 1], HI(p->x) - n / 2,
                  HI(p->y) - n, n, n * 2);
          p->variant--;
          BattleEffectKernels_IntegrateVector2(p, 56, 0x2000);
        }
      }
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  ResourceObject_ReleaseFar(work->objects[0]);
  BattleEffect_EndCanvasLayer();
}
#undef SLOT_BLITTER
#undef BLITTER
#undef HI
