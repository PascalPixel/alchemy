/// @file
/// Routines first written as f190b74.c; none has a twin in The Broken Seal.
// Part of f18fefc.c.

#include "../f18fefc_internal.h"

struct Model;

// The integer (pixel) half of a 16.16 value, read as the halfword it is.
#define HI(v) (((s16 *)&(v))[1])

#define KIND125_FRAMES 114

// The battle background's id, read through a union view of the bytes that
// begin with it. Its s32 member gives the read the alias set of
// transition->active, so the read waits for that store; as
// *(u16 *)(battle)->unk670, 6 of 1075 instructions differ in tla-en.
union BattleBackgroundId {
  u16 id;
  s32 word;
};
#define BACKGROUND(battle) (((union BattleBackgroundId *)(battle)->unk670)->id)

// A texture for the ARM model renderer: its size as powers of two and the
// address of its pixels.

// A model for the ARM renderer (Render_DrawModelList). A textured model (kind
// 7) takes a shade; a flat one (kind 3) the address of its colour.
/* VIEW(f190b74): tag struct RenderModel as struct RenderModel__f190b74; render_model.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/battle_effect/f18fefc.c:1096: structure has no member named `look'. */
struct RenderModel__f190b74 {
  s32 kind;
  s32 unk04;
  const u16 *mesh;
  void *vertices;
  struct ModelTexture *texture;
  union {
    s32 shade;
    u8 *color;
  } look;
};

extern char ResourceId_Data134;
extern char ResourceId_Data102;
extern char ResourceId_DustBlobSheet;
extern char ResourceId_Data0C2;
extern char ResourceId_Data0E9;
extern char ResourceId_RedHazeBackdrop;
extern char ResourceId_Data101;
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern u8 Data_02016000[];
extern u16 gData_08197410[];
extern const u8 gData_08199364[];
extern const u8 Data_0819919c[];
extern const u8 Data_081991e0[];
extern const u8 Data_081991a4[];
extern const s16 Data_08199f2a[];
extern const s16 Data_08199f2e[];

void BattleEffect_StartCanvasLayer(s32 layer);
void BattleEffect_EndCanvasLayer(void);
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags);
void Graphics_LoadGreyRamp(void);
void BattleEffect_BuildRepeatingTilemap(void);
void BattleEffect_SetApproachMotion(s32 id, s32 targetId, s32 frames, s32 rate);

void BattleActor_SpawnObjectsForListFar(s16 *targets, s32 mode);
void BattleActor_CommitPlacementFar(void);
void BattleBackground_LoadFar(s32 mode, s32 background, s32 level);

void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_ResetTransformState(void);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_080151e4(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);
void Func_0814cd48(s32 actor, s32 palette, s32 mode, s32 timer, s32 frames);
void Func_08164a4c(s32 blue, s32 green, s32 red);
void Camera_ApplyVerticalShake(s32 range);
void BattleEffect_CountDownBattlerTints(void);

// Effect kind 125, 114 frames. The screen turns to a scrolling red haze
// with only the caster and the target on it (frame 2); two textured flashes
// rise from the caster, two claws strike the target from above and below
// (frames 24-89, sparks at each hit); at 84 the caster charges across, at 86
// a great slash crosses the screen and at 90 the target is struck and 128
// flat shards scatter, tumbling; from frame 100 everything fades to white,
// and at the end the battle backdrop returns.
void BattleEffect_RunOdyssey(struct BattleEffectArgument *effect) {
  struct EffectPosition to;
  struct EffectPosition from;
  s16 list[14];
  DrawRectangle draw[2];
  struct ModelTexture texture;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *graphics;
  struct MotionObject *caster;
  struct BattlePresentationTransition *transition;
  s32 savedScrollX;
  struct RenderModel__f190b74 *model;
  void *projected;
  struct EffectStep *step;
  s32 frame;
  s32 i;
  s32 k;
  s32 fillMode;
  s32 flatKind;
  u8 *pixels;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data102, work->sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_DustBlobSheet,
                             work->sheet + 0x2f10, 1, 0);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  // The blitter faces the caster's side.
  if (effect->side == 0)
    BattleEffect_LoadBlitter(104, 1 << 4 | 3);
  else
    BattleEffect_LoadBlitter(104, 1 << 4 | 7);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  caster = GetBattleObjectSlotFar(effect->actor)->object;
  transition = Ram_HeapSlots->transitionWork;
  savedScrollX = gBgScroll[1].x;

  for (frame = 0; frame != KIND125_FRAMES; frame++) {
    EffectPosition_ProjectScaledBattler(effect->actor, &from);
    EffectPosition_ProjectScaledBattler(effect->actors[0], &to);

    if (frame == 0) {
      Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, gMapCellBuffer, 0,
                                 0);
      Resource_LoadAndDecompress((s32)&ResourceId_Data0E9, gData_02014000, 0,
                                 0);
      Graphics_LoadGreyRamp();
      REG_BLDALPHA = 0x1010;
      for (i = 0; i != 64; i++)
        work->particles[i].variant = -1;
    }

    // Frame 2: only the caster and the target over the red haze, whose
    // colours (entries 96 on) lose their green and take red into blue.
    if (frame == 2) {
      u16 *palette;

      list[0] = effect->actor;
      list[1] = effect->actors[0];
      list[2] = 0xff;
      BattleActor_SpawnObjectsForListFar(list, 0);
      BattleBackground_LoadFar(1, (s32)&ResourceId_RedHazeBackdrop, 0);
      BattleEffect_BuildRepeatingTilemap();
      palette = (u16 *)BG_PLTT + 96;
      for (i = 0; i != 128; i++) {
        s32 entry = *palette;
        s32 red = 31;
        u16 green;
        s32 blue;

        red &= entry;
        /* FAKEMATCH(dead): green is taken and clamped, then never used, which
         * keeps blue's (u16) cast as a shift pair; as no green at all, 61 of
         * 1075 instructions differ in tla-en. */
        green = ((u16)entry >> 5) & 31;
        blue = ((u16)entry >> 10) & 31;

        if (red > 31)
          red = 31;
        if (blue > 31)
          blue = 31;
        green = green > 31 ? 31 : green;
        *palette++ = red << 10 | ((u16)red >> 1) << 5 | blue;
      }
      transition->active = 1;
    }
    if (frame > 1)
      gBgScroll[1].x += 8;

    if (frame == 8) {
      Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      step = work->particles;
      for (i = 0; i != 64; i++) {
        step->x = Random16() % 216 - 96;
        step->y = Random16() % 216;
        step++;
      }
    }
    if (frame == 86)
      Resource_LoadAndDecompress((s32)&ResourceId_Data101, work->sheet + 0xc80,
                                 1, 1);

    // Frame 84: the caster charges at the target.
    if (frame == 84) {
      Sound_PlayFar(212);
      Actor_SetMode(caster, 2);
      Actor_SetAnimationSpeedFar(caster, 48);
      BattleEffect_SetApproachMotion(effect->actor, effect->actors[0], 7, 80);
    }

    // Frame 90: the hit, and 128 shards scatter away from the caster.
    if (frame == 90) {
      struct EffectStep *shard;

      Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      Sound_PlayFar(144);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
      work->shakeFrames = 8;
      shard = Data_02016000;
      for (i = 0; i != 128; i++) {
        s32 speed = (Random16() & 0x1ff) + 32;
        s32 angle = Random16() & 0x7fff;

        if (effect->side == 1)
          angle += 0x8000;
        shard->x = (to.x / 2) << 16;
        shard->y = to.y << 16;
        shard->velocityX = (Trig_Sin(angle) * speed) >> 6;
        shard->velocityY = -(Trig_Cos(angle) * speed) >> 5;
        shard->z = Random16();
        shard->velocityZ = (Random16() & 0x3ff) + 0x400;
        shard->variant = (Random16() & 0x3fff) + 0x2000;
        shard++;
      }
    }

    // From frame 86: the slash crosses the screen in two mirrored halves.
    if (frame > 85) {
      s32 x;

      if (effect->side == 0) {
        x = 2872 - frame * 32;
        if (x < -38)
          x = -38;
      } else {
        x = frame * 32 - 2910;
        if (x > 0)
          x = 0;
      }
      draw[0](canvas, work->sheet + 0xc80, x, 8, 158, 56);
      if (effect->side == 0)
        BattleEffect_LoadBlitter(188, 1 << 4 | 11);
      else
        BattleEffect_LoadBlitter(188, 1 << 4 | 15);
      draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
      draw[1](canvas, work->sheet + 0xc80, x, 64, 158, 56);
      HeapSlot_Release(188);
    }

    // Until frame 89: the sparks of the claws' hits, shrinking.
    if (frame <= 89) {
      for (i = 0, step = work->particles; i != 64; i++) {
        if (step->variant >= 0) {
          s32 size = (step->variant >> 2) + 1;

          draw[0](canvas, graphics + gData_08197410[size - 1],
                  HI(step->x) - size / 2, HI(step->y) - size, size, size * 2);
          BattleEffectKernels_IntegrateVector2(step, 60, 0);
          step->variant--;
        }
        step++;
      }
    }

    // Two claws, from frames 24 and 40: one sweeps down onto the target,
    // the other (flipped) up from below; sixteen sparks at each hit.
    for (i = 0; i != 2; i++) {
      s32 start = i * 16 + 24;

      if (frame == i * 16 + 28)
        Sound_PlayFar(212);
      if (frame == i * 16 + 32) {
        for (k = 0; k != 16; k++) {
          struct EffectStep *spark;
          s32 speed = (Random16() & 255) + 128;
          s32 angle = Random16() & 0x3fff;

          if (effect->side == 0) {
            if (i != 0)
              angle += 0x4000;
          } else if (i == 0) {
            angle -= 0x4000;
          } else {
            angle += 0x8000;
          }
          spark = &work->particles[i * 32 + k];
          spark->x = (to.x / 2) << 16;
          spark->y = to.y << 16;
          spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
          spark->velocityY = -(Trig_Cos(angle) * speed) >> 5;
          spark->variant = (Random16() & 7) + 17;
        }
        Sound_PlayFar(133);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 64);
        work->shakeFrames = 4;
      }
      if (frame >= start && frame <= 89) {
        s32 age = frame - start;
        s32 x;
        s32 y;

        if (age <= 8) {
          if (effect->side == 0)
            x = to.x / 2 - age * 10 + 80;
          else
            x = to.x / 2 + age * 10 - 80;
          if (i == 0)
            y = age * 16 - 120;
          else
            y = 168 - age * 16;
        } else {
          // DEAD: both arms alike; the ROM keeps the test of effect->side.
          if (effect->side == 0)
            x = to.x / 2;
          else
            x = to.x / 2;
          y = i == 0 ? 8 : 40;
        }
        if (i == 1) {
          if (effect->side == 0)
            BattleEffect_LoadBlitter(188, 1 << 4 | 11);
          else
            BattleEffect_LoadBlitter(188, 1 << 4 | 15);
          draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
        }
        draw[i](canvas, work->sheet, x - 20, y, 40, 80);
        if (i == 1)
          HeapSlot_Release(188);
      }
    }

    projected = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    pixels = gMapCellBuffer;
    texture.logWidth = 7;
    texture.logHeight = 7;
    texture.pixels = pixels;
    model->mesh = gData_08199364;
    model->kind = 7;
    model->texture = &texture;
    model->vertices = projected;
    for (i = 0; i != 2; i++) {
      s32 start;

      // Two flashes rise from the caster (frames 2 and 8).
      start = i * 6 + 2;
      if (frame == start)
        Sound_PlayFar(212);
      if (frame >= start && frame < i * 6 + 18) {
        s32 age = frame - start;
        s32 shade = 64 - age * 8;

        if (shade > 0)
          shade = 0;
        if (shade > -64) {
          s32 scale = (age << 14) + 0x4000;

          model->look.shade = shade;
          Render_ResetTransformState();
          Func_08015160((from.x / 2 - 64) << 16, 0, 0);
          Func_080151e4(scale, scale * 2, scale);
          SceneTransform_ApplyPitch(Data_08199f2a[i]);
          SceneTransform_ApplyYaw(Data_08199f2e[i]);
          Render_TransformVertices(Data_081991e0, projected, 4);
          Render_DrawModelList(model);
        }
      }

      // Frames 64-82: the caster gathers itself, flashing.
      start = i * 8 + 64;
      if (frame == i * 8 + 68)
        Sound_PlayFar(212);
      if (frame == i * 8 + 72) {
        s32 none = 0;

        Func_0814cd48(effect->actor, 7, -1, 0, none);
      } else if (frame == i * 8 + 76)
        Func_0814cd48(effect->actor, 0, -1, 0, 0);
      if (frame >= start && frame < start + 11) {
        s32 scale = 0x30000 - ((frame - start) << 14);

        model->look.shade = 0;
        Render_ResetTransformState();
        Func_08015160((from.x / 2 - 64) << 16, 0, 0);
        Func_080151e4(scale, scale * 2, scale);
        SceneTransform_ApplyPitch(Data_08199f2a[i]);
        SceneTransform_ApplyYaw(Data_08199f2e[i]);
        Render_TransformVertices(Data_081991e0, projected, 4);
        Render_DrawModelList(model);
      }

      // A flash at the target with each claw's hit.
      start = i * 16 + 32;
      if (frame >= start && frame < i * 16 + 48) {
        s32 age = frame - start;
        s32 shade = 64 - age * 8;

        if (shade > 0)
          shade = 0;
        if (shade > -64) {
          s32 scale = (age << 14) + 0x4000;

          model->look.shade = shade;
          Render_ResetTransformState();
          Func_08015160((to.x / 2 - 64) << 16, 0, 0);
          Func_080151e4(scale, scale * 2, scale);
          SceneTransform_ApplyPitch(Data_08199f2a[i]);
          SceneTransform_ApplyYaw(Data_08199f2e[i]);
          Render_TransformVertices(Data_081991e0, projected, 4);
          Render_DrawModelList(model);
        }
      }
    }

    // From frame 90: the shards, flat triangles in eight colours; from 100
    // the screen fades to white.
    fillMode = 1;
    flatKind = 3;
    if (frame > 89) {
      u8 color[1];
      s32 center;
      s32 x;
      s32 y;

      model->mesh = Data_0819919c;
      model->kind = flatKind;
      if (frame > 99) {
        work->transferMode = fillMode;
        work->transferValue = 0x02020202;
        Func_08164a4c(2, 2, 2);
      }
      for (i = 0, step = Data_02016000; i != 128; i++) {
        color[0] = (i & 7) + 32;
        model->look.color = color;
        Render_ResetTransformState();
        x = step->x;
        y = step->y;
        center = -(64 << 16);
        Func_08015160(x + center, y + center, 0);
        Func_080151e4(step->variant * 3, step->variant * 6, step->variant);
        SceneTransform_ApplyPitch(step->z);
        SceneTransform_ApplyYaw(step->z);
        Render_TransformVertices(Data_081991a4, projected, 3);
        Render_DrawModelList(model);
        BattleEffectKernels_IntegrateVector3(step, 62, 0);
        step++;
      }
    }
    Heap_Release(model);
    Heap_Release(projected);

    Camera_ApplyVerticalShake(8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // The battle's own scene again.
  BattleActor_CommitPlacementFar();
  transition->active = 0;
  gBgScroll[1].x = savedScrollX;
  BattleBackground_LoadFar(1, BACKGROUND(Ram_HeapSlots->battleWork), 24);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}


typedef DrawRectangle BlitterPair[2];

// The swirl runs until A is pressed, at most this many frames.
#define SWIRL_MAX_FRAMES 312000

extern char ResourceId_CloudSwirlSheet;
extern char ResourceId_EmberStreakSheet;
extern char ResourceId_BlueFlameSheet;

s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode);

void BattleEffect_FlushPendingGraphicsTransfer(void);

void Func_0815f0a0(s32 resource);

void *Resource_GetTableEntry(s32 id);
s32 Resource_DecodeType01(const void *source, void *destination);

static inline s32 Swirl_CopyPalette(const void *source) {
  return Iwram_CopyWords((void *)&BG_PLTT[96], source, 128);
}

static inline s32 Swirl_Draw(DrawRectangle draw, void *buffer,
                             const void *canvas, s32 x, s32 size) {
  return draw(buffer, canvas, x, 0, size, size);
}

// A turning cloud swirl on BG2, shown from Func_0813e7e0. The cloud sheet
// is drawn once into the canvas (its colours moved to palette entries 96
// on) and copied twice across the map buffer; then BG2 turns about the
// screen's centre while BG1 scrolls, the palette flashes white at the start,
// steps toward the swirl's own colours every fourth frame and from frame
// 120 dims. B starts it over and A ends it.
void Func_08191530(struct BattleEffectArgument *effect) {
  BlitterPair sheetDraw;
  s32 savedScrollX;
  u8 *canvas;
  struct BattleEffectWork *work;
  struct BattlePresentationTransition *transition;
  DrawRectangle draw;
  const u8 *palette;
  s32 frame;
  s32 enabled;
  s32 sine;
  s32 cosine;
  s32 i;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  savedScrollX = gBgScroll[1].x;
  transition = Ram_HeapSlots->transitionWork;
  BattleEffect_StartCanvasLayer(3);
  REG_DISPCNT = 0x7741;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f42;
  REG_BG2CNT = 0x787;
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 0;
  work->transferMode = 2;
  work->transferValue = 50;

  // The cloud sheet, recoloured to palette entries 96 on, drawn into the
  // canvas through the copy blitter.
  BattleEffect_LoadBlitter(104, 1 << 4 | 3);
  sheetDraw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  palette = Resource_GetTableEntry((s32)&ResourceId_CloudSwirlSheet);
  /* FAKEMATCH(wrapper): the copy through Swirl_CopyPalette; as a direct
   * Iwram_CopyWords, 22 of 468 instructions differ in tla-en. */
  Swirl_CopyPalette(palette);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet);
  palette = Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet);
  Iwram_CopyWords((void *)&BG_PLTT[96], palette, 128);
  for (i = 0; i != 0x4000; i++)
    work->sheet[i] += 96;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  sheetDraw[0](canvas, work->sheet, 0, 0, 128, 128);
  work->transferPending = 1;
  wait(1);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);

  // The swirl itself in the canvas (colour 0 made 1, so none is clear),
  // copied twice side by side into the map buffer.
  Resource_LoadAndDecompress((s32)&ResourceId_CloudSwirlSheet, canvas, 1, 1);
  for (i = 0; i != 0x4000; i++) {
    if (canvas[i] == 0)
      canvas[i] = 1;
  }
  {
    s32 mode = 3;

    enabled = 1;
    Func_08196404(104, 8, 7, mode, mode);
  }
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Task_Add(BattleEffect_FlushPendingGraphicsTransfer, 0xc80);
  transition->active = enabled;
  /* FAKEMATCH(wrapper): the blit through Swirl_Draw with the constant buffer;
   * as a direct draw, 58 of 468 instructions differ in tla-en. */
  Swirl_Draw(draw, Ram_MapCellBuffer, canvas, 0, 128);
  draw(Ram_MapCellBuffer, canvas, 128, 0, 128, 128);
  work->transferPending = enabled;
  wait(1);

  for (frame = 0; frame != SWIRL_MAX_FRAMES; frame++) {
    if (gInput.repeat & 2)
      frame = 0;
    if (gInput.repeat & 1)
      break;

    if (frame == 0)
      Iwram_FillWords((void *)BG_PLTT, 128, 0x7fff7fff);
    if (frame > 119)
      Func_08164a4c(-1, -1, -1);
    else if ((frame & 3) == 1)
      Func_0815f0a0((s32)&ResourceId_BlueFlameSheet);
    gBgScroll[1].x = frame / 2;

    // BG2 turns a 1024th of a circle a frame, at 1/532 of the sine's scale
    // (about half size), about the screen's centre (120, 72).
    sine = Trig_Sin(frame << 6) / 532;
    cosine = Trig_Cos(frame << 6) / 532;
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PA, cosine, 0x20000);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PB, sine, 0x20000);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PC, -sine, 0x20000);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PD, cosine, 0x20000);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2X,
                        (Iwram_MulQ16(-120 << 24, cosine) +
                         Iwram_MulQ16(-72 << 24, sine) + 0x400000) >>
                            8,
                        0x30000);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2Y,
                        (Iwram_MulQ16(-120 << 24, -sine) +
                         Iwram_MulQ16(-72 << 24, cosine) + 0x400000) >>
                            8,
                        0x30000);
    wait(1);
  }

  gBgScroll[1].x = savedScrollX;
  transition->active = 0;
  Task_Remove(BattleEffect_FlushPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

// Declared as The Broken Seal's five-argument blitter builder, which this game
// does not link; it serves only the never-taken call below.
s32 BattleEffect_BuildBlitterFar(s32 slot, s32 widthShift, s32 heightShift,
                                 s32 flags, u32 mode);

// Summon 25's screen wipe: a soft-edged grey band, drawn once into the
// canvas, slides across the screen on BG2 at speed pixels per frame, from
// the left or (when the low bit of side is set) from the right. Halfway
// through, while the band covers the screen, it calls midpoint to change
// the scene behind it.
void Func_08191958(u8 side, u8 speed, void (*midpoint)(void)) {
  s32 scrollX;
  u8 *canvas;
  struct BattleEffectWork *work;
  u16 ramp[63];
  s32 half;
  s32 x;
  s32 i;

  scrollX = (side & 1) * 192 - 64;
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  /* FAKEMATCH(dead): a never-taken five-argument call reserves the stack
   * frame's outgoing argument word at sp+0; as no call, 18 of 185
   * instructions differ in tla-en. */
  if (0)
    BattleEffect_BuildBlitterFar(104, 7, 7, 3, 3);
  BattleEffect_LoadBlitter(104, 3);
  work->transferMode = 0;
  work->transferValue = 75;
  REG_BG2PA = 0x48;

  // The band: colours 0 to 62 rising over the first 32 rows, 63 through the
  // middle 64 and falling again over the last 32, in 8x8 256-colour tiles.
  for (i = 0; i != 128; i++) {
    for (x = 0; x != 128; x++) {
      s32 line = i & 7;
      s32 color;
      s32 row;

      if (i <= 31)
        color = i * 2;
      else {
        color = 63 - ((i - 96) << 1);
        if (i <= 95)
          color = 63;
      }
      row = line + (i >> 3) * 64;
      canvas[row + ((x & 7) << 3) + ((x >> 3) << 10)] = color;
    }
  }

  // Colours 1 to 63: a grey ramp from black to half white.
  for (i = 0; i != 63; i++) {
    s32 level = i >> 1;

    ramp[i] = level << 10 | level << 5 | level;
  }
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT[1], (u32)ramp, 0x8000003f);

  REG_BG2X = scrollX << 8;
  // x, done with the band, now counts the slide's frames.
  x = 192 / ((s8)speed < 0 ? -(s8)speed : (s8)speed) + 1;
  half = (x - 1) >> 1;
  for (i = 0; i != x; i++) {
    scrollX += (s8)speed;
    REG_BG2X = scrollX << 8;
    work->transferPending = 1;
    wait(1);
    if (i == half && midpoint != NULL)
      midpoint();
  }
  HeapSlot_Release(104);
}

struct ChildObject {
  u8 unk00[5];
  u8 mode;
  u8 unk06[10];
  s32 active;
};

struct ChildGroup {
  u8 unk00[25];
  u8 dirty;
  u8 unk1a;
  u8 count;
  u8 unk1c[12];
  struct ChildObject *children[1];
};

/// Sets the draw parameter (byte 5) of each active child of group to mode
/// and marks the group for redrawing.
void Actor_SetActiveChildParam(struct ChildGroup *group, s32 mode) {
  s32 i;

  for (i = 0; i < group->count; i++) {
    struct ChildObject *child = group->children[i];

    if (child != NULL && child->active != 0)
      *((u8 *)child + 5) = mode;
  }
  group->dirty = 1;
}

/// Fills count bytes of table with a rising shade ramp (0 to 63, by a step
/// scaled by scale), then repeats it seven more times.
void BattleEffect_BuildShadeRamp(u8 *table, u32 count, s32 scale) {
  u32 step = count * 2;
  s32 total = 0;
  u32 i;
  s32 j;

  if (step > 63)
    step = 63;
  step = 63 / step;
  if (step < 6)
    step = 6;
  for (i = 0; i != count; i++) {
    s32 value;

    total += step;
    value = (scale * total) >> 16;
    if (value > 63)
      value = 63;
    table[i] = value;
  }
  for (j = 1; j != 8; j++) {
    for (i = 0; i != count; i++)
      table[j * count + i] = table[i];
  }
}

/// Fills table with a shade ramp that rises over count bytes and falls back,
/// repeated repeats times.
void BattleEffect_BuildShadeRampWave(u8 *table, u32 count, u32 repeats,
                                     s32 scale) {
  s32 total = 0;
  u32 step = count * 2;
  u32 i;
  u32 j;

  if (step > 63)
    step = 63;
  step = 63 / step;
  if (step < 6)
    step = 6;
  for (i = 0; i != count; i++) {
    s32 value;

    total += step;
    value = (scale * total) >> 16;
    if (value > 63)
      value = 63;
    table[i] = value;
  }
  for (i = 0; i != count; i++)
    table[count + i] = table[count - 1 - i];
  for (j = 1; j != repeats; j++) {
    for (i = 0; i != count * 2; i++)
      table[j * count * 2 + i] = table[i];
  }
}

/// As BattleEffect_BuildShadeRampWave, at twice the step.
void BattleEffect_BuildSteepShadeRampWave(u8 *table, u32 count, u32 repeats,
                                          s32 scale) {
  s32 total = 0;
  u32 step = count * 2;
  u32 i;
  u32 j;

  if (step > 63)
    step = 63;
  step = 63 / step;
  if (step < 6)
    step = 6;
  step *= 2;
  for (i = 0; i != count; i++) {
    s32 value;

    total += step;
    value = (scale * total) >> 16;
    if (value > 63)
      value = 63;
    table[i] = value;
  }
  for (i = 0; i != count; i++)
    table[count + i] = table[count - 1 - i];
  for (j = 1; j != repeats; j++) {
    for (i = 0; i != count * 2; i++)
      table[j * count * 2 + i] = table[i];
  }
}


void Func_08118088(s32 actor, s32 mode);
void Palette_LoadBasicObjColors(void);

// Summon 25's teardown: sets each target back on the ground at the battle
// origin with the usual bounce (The Broken Seal's 0xab85 motion strength)
// and restarts its idle motion, ends the presentation transition, centres the
// camera at (120, 120), reloads the battle palette and redraws the battle's
// own backdrop over the summon's.
void Func_08191cc4(void) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  struct BattlePresentationTransition *transition =
      Ram_HeapSlots->transitionWork;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 i;

  for (i = 0; i != work->effect->count; i++) {
    struct MotionObject *object =
        GetBattleObjectSlotFar(work->effect->actors[i])->object;

    object->x = 0;
    object->y = 120 << 16;
    object->z = 0;
    object->verticalMotionStrength = 0xab85;
    Func_08118088(work->effect->actors[i], 1);
  }
  transition->active = 0;
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 120;
  Palette_LoadBasicObjColors();
  // The battle's background (The Broken Seal's battle->background).
  BattleBackground_LoadFar(1, *(u16 *)battle->unk670, 24);
}
#undef HI
#undef KIND125_FRAMES
#undef BACKGROUND
#undef SWIRL_MAX_FRAMES
