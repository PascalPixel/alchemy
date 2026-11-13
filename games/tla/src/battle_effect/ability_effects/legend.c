/// @file
/// Legend, and the Bramble, Frost, Flame and Thunder Cards.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

// The Lost Age's heap slots, by byte offset into Ram_HeapSlots.
#define HEAP_SLOT_BLITTER 104

// The transfer task runs at this priority, where ☀️'s runs at 0x480.
#define TASK_PASS_FRAME 0xc80

// BattleEffect_LoadBlitter's packed argument: the mode in the high nibble.
#define BLITTER(flags, mode) ((mode) << 4 | (flags))

#define HI(v) (((s16 *)&(v))[1])

// The sparks live in the map cell buffer's second 16 KB, 128 for each sword.
#define SPARKS ((struct EffectStep *)(Ram_MapCellBuffer + 0x4000))
#define SPARK_COUNT 512

// A texture for the model kernels: its size as powers of two, then its
// pixels.

// One entry of a model list, as Render_AllocateModelList allocates them and
// Render_DrawModelList draws them.
/* VIEW(f189b14): tag struct Model as struct Model__f189b14; ability_effects.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/battle_effect/ability_effects.c:38301: structure has no member named `vertices'. */
struct Model__f189b14 {
  s32 kind;
  s32 next;
  void *mesh;
  void *vertices;
  struct ModelTexture *texture;
  s32 alpha;
  s32 tail;
};

extern char ResourceId_CornerRuneASheet;
extern char ResourceId_Data134;
extern char ResourceId_BladeSheet;
extern char ResourceId_Data0E9;
extern char ResourceId_Data0C2;

// The quad mesh and the corners of the rune, the jaw-like flash and the
// eight blades of the opening fan.
extern const u8 gData_08199364[];
// The quad meshes and corners: the card, the pictures and their spread.
extern u8 Data_081992b0[];
extern const u8 Data_081991e0[];
extern s32 Data_08199200[];

// The spark cells: offsets into the particle sheet by size.
extern u16 gData_08197410[];

void BattleEffect_StartCanvasLayer(s32 bgControl);
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags);
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);

void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position);
void Func_081180e8(s32 phase);
void BattleEffect_StartActorMotion(s32, s32, s32, s32, s32, s32);
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Func_080151e4(s32 x, s32 y, s32 z);
void Func_08015160(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Func_080150e4(s32 angle);
void Func_0801521c(s32 scale);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
/* VIEW(f189b14): routine Render_DrawModelList as void Render_DrawModelList(struct Model__f189b14 *model); f188714 has void Render_DrawModelList(struct Model *model). */
extern void Render_DrawModelList__f189b14(struct Model__f189b14 *model) __asm__("Render_DrawModelList");
void Heap_Release(void *block);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_EndCanvasLayer(void);

// Legend (effect kind 122): a fan of eight blades opens over the target for
// 50 frames while a rune, mirrored from one corner, rises and turns behind
// it. Then one sword for each step of the variant (variant + 1 in all, 16
// frames apart) falls on the target from above the user's side: it strikes
// at 59 frames plus its delay, throwing 128 sparks that bounce on the ground,
// while a flash grows and fades where it lands. The effect lasts 86 frames
// and 16 more for each further sword.
void BattleEffect_RunLegend(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  u8 *graphics;
  struct EffectPosition start;
  struct EffectPosition target;
  struct ModelTexture texture;
  struct EffectStep *step;
  s32 i;
  s32 j;
  u8 *cells = Ram_MapCellBuffer;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up the canvas and a clipped MAX blitter, mirrored to face the
  // target; mirror the rune's corner into the sheet; load the sparks, the
  // sword and the flash.
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  if (effect->side == 0)
    BattleEffect_LoadBlitter(
        HEAP_SLOT_BLITTER,
        BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, BLIT_ADDITIVE));
  else
    BattleEffect_LoadBlitter(
        HEAP_SLOT_BLITTER,
        BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                BLIT_ADDITIVE));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_CornerRuneASheet, cells, 1, 1);
  Func_0816ae40(cells, work->sheet, 64, 64);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BladeSheet, work->sheet + 0x4000,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0E9, work->sheet + 0x5000, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, cells, 0, 0);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != effect->variant * 16 + 86; frame++) {
    // Frame 0: the swords set out from 80 pixels above and beside the
    // target, on the user's side, toward it.
    if (frame == 0) {
      EffectPosition_ProjectBattler(effect->actors[0], &target);
      if (effect->side == 0)
        start.x = target.x + 80;
      else
        start.x = target.x - 80;
      start.y = target.y - 80;
      for (i = 0; i != 3; i++) {
        step = &work->particles[i];
        step->x = start.x << 15;
        step->y = start.y << 16;
        step->velocityX = (target.x - start.x) << 11;
        step->velocityY = (target.y - start.y) << 12;
        step->variant = 0;
      }
      for (i = 0; i != SPARK_COUNT; i++)
        SPARKS[i].variant = -1;
    }

    // The swords, 16 frames apart.
    for (i = 0; i != effect->variant + 1; i++) {
      step = &work->particles[i];
      if (frame == i * 16 + 54)
        Sound_PlayFar(212);

      // The strike: the target reels and flashes, and 128 sparks fly up
      // from it. The last sword ends the action.
      if (frame == i * 16 + 59) {
        if (i == effect->variant)
          Func_081180e8(144);
        else
          Sound_PlayFar(144);
        BattleEffect_StartActorMotion(effect->actors[0], 1, 0, 0, 0, 100);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
        work->shakeFrames = 4;
        for (j = 0; j != 128; j++) {
          struct EffectStep *spark = &SPARKS[i * 128 + j];
          s32 angle;
          s32 force;

          spark->x = target.x << 15;
          spark->y = 96 << 16;
          angle = Random16() % 0xc000 + 0x2000;
          force = Random16() & 127;
          spark->velocityX = (Trig_Sin(angle) * force) >> 4;
          spark->velocityY = (Trig_Cos(angle) * force) >> 3;
          spark->variant = (Random16() & 7) + 16;
        }
      }

      // From frame 50 plus its delay the sword falls, faster each frame,
      // until it sticks in the ground 12 frames later.
      if (frame >= i * 16 + 50) {
        s32 x = HI(step->x) - 20;
        s32 y = HI(step->y) - 50;
        s32 height = 80;

        if (y > 24)
          height = 104 - y;
        draw[0](canvas, work->sheet + 0x4000, x, y, 40, height);
        if (step->variant <= 12) {
          step->x += step->velocityX;
          step->y += step->velocityY;
          step->velocityX += (target.x - start.x) << 9;
          step->velocityY += (target.y - start.y) << 10;
        }
        step->variant++;
      }
    }

    // The sparks bounce once on the ground at y = 104.
    for (i = 0; i != SPARK_COUNT; i++) {
      step = &SPARKS[i];
      if (step->variant >= 0) {
        s32 size = (step->variant >> 2) + 1;

        draw[0](canvas, graphics + gData_08197410[size - 1],
                HI(step->x) - size / 2, HI(step->y) - size, size, size * 2);
        if (step->y > 104 << 16) {
          step->y = 104 << 16;
          step->velocityY = -step->velocityY / 2;
        }
        BattleEffectKernels_IntegrateVector2(step, 62, 0x4000);
        step->variant--;
      }
    }

    // This frame's models: one quad, retextured for each part.
    {
      struct Model__f189b14 *model;
      void *vertices;
      s32 scale;

      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = work->sheet;
      // Stored as an integer: as a pointer this store follows the kind store.
      *(s32 *)&model->mesh = (s32)gData_08199364;
      model->kind = 7;
      model->texture = &texture;
      model->vertices = vertices;

      // Frames 16-107: the rune rises behind the target, turning, then fades.
      if (frame >= 16 && frame <= 107) {
        s32 height = frame * 2 - 96;
        s32 alpha = 0;

        scale = (frame << 13) - 0x20000;
        if (height > -48)
          height = -48;
        if (frame >= effect->variant * 16 + 60)
          // Spelled with + -240 so that the negation is not folded into 240 -
          // x.
          alpha = -((frame - effect->variant * 16) * 4 + -240);
        if (scale > 0x20000)
          scale = 0x20000;
        model->alpha = alpha;
        Render_ResetTransformState();
        Func_080151e4(0x8000, 0x10000, 0x8000);
        Func_08015160((start.x - 128) << 16, height << 16, 0);
        SceneTransform_ApplyPitch(0x5900);
        if (effect->side == 0)
          SceneTransform_ApplyYaw(0x1900);
        else
          SceneTransform_ApplyYaw(-0x1900);
        Func_080150e4(frame << 9);
        Func_0801521c(scale);
        Func_080151e4(0x10000, 0x10000, 0x10000);
        Render_TransformVertices(Data_081991e0, vertices, 4);
        Render_DrawModelList__f189b14(model);
      }

      // Each sword's flash grows where it lands and fades over 16 frames.
      texture.pixels = cells;
      for (i = 0; i != effect->variant + 1; i++) {
        if (frame >= i * 16 + 50) {
          s32 age = frame - i * 16 - 50;
          s32 alpha = 64 - age * 8;

          if (alpha > 0)
            alpha = 0;
          if (alpha > -64) {
            scale = age * 12384 + 0x8000;
            model->alpha = alpha;
            Render_ResetTransformState();
            Func_080151e4(0x8000, 0x10000, 0x8000);
            Func_08015160((start.x - 128) << 16, -48 << 16, 0);
            SceneTransform_ApplyPitch(0x5900);
            if (effect->side == 0)
              SceneTransform_ApplyYaw(0x1900);
            else
              SceneTransform_ApplyYaw(-0x1900);
            Func_080150e4(frame << 9);
            Func_0801521c(scale);
            Render_TransformVertices(Data_081991e0, vertices, 4);
            Render_DrawModelList__f189b14(model);
          }
        }
      }

      // Frames 0-49: the fan of eight blades opens, pulsing, and closes.
      if (frame <= 49) {
        texture.logWidth = 5;
        texture.logHeight = 6;
        texture.pixels = work->sheet + 0x5000;
        model->mesh = Data_081992b0;
        for (i = 0; i != 8; i++) {
          model->alpha = 0;
          Render_ResetTransformState();
          Func_080151e4(0x8000, 0x10000, 0x8000);
          Func_08015160((start.x - 128) << 16, -80 << 16, 0);
          SceneTransform_ApplyPitch(0x47d0);
          Func_080150e4((frame + i * 16) << 9);
          scale = (frame << 12) - 0x4000;
          if (scale > 0x10000)
            scale = 0x10000;
          if (frame > 31)
            scale = 0x30000 - (frame << 12);
          Func_080151e4(0x8000, 0x10000,
                        (Trig_Sin((frame + i * 8) << 9) / 8 + scale) * 2);
          Render_TransformVertices(Data_08199200, vertices, 4);
          Render_DrawModelList__f189b14(model);
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

void Func_0818a220(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Bramble Card (kind 88).
DEFINE_EFFECT_KIND(BattleEffect_RunBrambleCard, Func_0818a220, 0)

/// Abilities: Frost Card (kind 87).
DEFINE_EFFECT_KIND(BattleEffect_RunFrostCard, Func_0818a220, 1)

/// Abilities: Flame Card (kind 79).
DEFINE_EFFECT_KIND(BattleEffect_RunFlameCard, Func_0818a220, 2)

/// Abilities: Thunder Card (kind 89).
DEFINE_EFFECT_KIND(BattleEffect_RunThunderCard, Func_0818a220, 3)

// The four cards, as the wrappers pass them.
enum {
  CARD_BRAMBLE, // Bramble Card, kind 88: vines grow from the card.
  CARD_FROST,   // Frost Card, kind 87: the card throws crystal shards.
  CARD_FLAME,   // Flame Card, kind 79: the card throws smoke and flashes.
  CARD_THUNDER, // Thunder Card, kind 89: bolts strike from the card.
};

// How each card's sparks move: the kernel's gravity and damping, and how
// many steps (each drawn) a frame.
struct SparkMotion {
  s32 gravity;
  s32 damping;
  s32 steps;
};

extern char ResourceId_VineCardSheet;
extern char ResourceId_DragonCardSheet;
extern char ResourceId_CrystalSheet;
extern char ResourceId_SunCardSheet;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_SmokeSheet;
extern char ResourceId_BoltCardSheet;
extern char ResourceId_Data0F8;
extern char ResourceId_EffectPalette02;
extern char ResourceId_OrangePaletteB;

// Each card's length in frames: 124, 140, 132 and 130.
extern u8 Data_08199d60[];
extern struct SparkMotion Data_08199d64[];

// The crystal shards' cells.
extern s32 gData_08197834[];
extern u8 gData_0819781a[];
extern u8 gData_08197826[];

extern const u8 gData_08199268[];

extern const u8 Data_081991c0[];

void EffectPosition_ProjectSprite(s32 id, struct EffectPosition *position);
void Func_0815f0a0(s32 resource);

// The four cards (Bramble, Frost, Flame and Thunder Card): a card rises from
// the user, spinning and growing, for 48 frames, then shows its picture.
// From frame 48 the background takes the card's colours; the Frost and
// Flame cards throw shards or smoke at the targets, and the Bramble and
// Thunder cards spread four pictures over the field. Each target is struck
// every five frames for a while, reeling, flashing and throwing up sparks.
// From frame 96 the card shrinks away.
void Func_0818a220(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  u8 *graphics;
  s32 scale;
  struct EffectPosition card;
  struct EffectPosition targets[8];
  struct EffectPosition aim;
  struct ModelTexture texture;
  struct Model__f189b14 *model;
  void *vertices;
  s32 i;
  s32 firstHit;
  s32 lastHit;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up the canvas, a clipped additive blitter, the sparks, and the
  // card's picture and its extras.
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(
      HEAP_SLOT_BLITTER,
      BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, BLIT_ADDITIVE));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  scale = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  if (mode == CARD_BRAMBLE) {
    Resource_LoadAndDecompress((s32)&ResourceId_VineCardSheet, work->sheet, 1,
                               1);
  } else if (mode == CARD_FROST) {
    Resource_LoadAndDecompress((s32)&ResourceId_DragonCardSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet,
                               work->sheet + 0x800, 1, 0);
  } else if (mode == CARD_FLAME) {
    Resource_LoadAndDecompress((s32)&ResourceId_SunCardSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                               work->sheet + 0x800, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet + 0x800,
                               1, 0);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_BoltCardSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0F8, work->sheet + 0x800, 0,
                               0);
  }
  REG_BLDCNT = 0;
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != Data_08199d60[mode]; frame++) {
    // Frame 0: clear the particles, give each member a strike countdown, and
    // find the user and every target.
    if (frame == 0) {
      Sound_PlayFar(190);
      for (i = 0; i != 56; i++)
        work->particles[i].variant = -1;
      for (i = 0; i != effect->count; i++)
        work->particles[56 + i].variant = (Random16() & 3) + 2;
      for (i = 0; i != SPARK_COUNT; i++)
        SPARKS[i].variant = -1;
      EffectPosition_ProjectSprite(effect->actor, &card);
      for (i = 0; i != effect->count; i++)
        EffectPosition_ProjectScaledBattler(effect->actors[i], &targets[i]);
      REG_BLDCNT = 0;
      work->transferMode = 1;
      work->transferValue = 0;
      scale = 0x4000;
    }

    // The card rises and grows for 48 frames, and from frame 96 rises
    // faster and shrinks.
    if (frame <= 47) {
      card.y--;
      scale += 0x200;
      if (scale > 0x8000)
        scale = 0x8000;
    }
    if (frame > 95) {
      scale += -0x200;
      card.y -= 2;
    }

    // Frame 48: the card shows its picture.
    if (frame == 48) {
      if (mode == CARD_BRAMBLE)
        Sound_PlayFar(148);
      if (mode == CARD_FROST)
        Sound_PlayFar(144);
      if (mode == CARD_FLAME)
        Sound_PlayFar(104);
      if (mode == CARD_THUNDER)
        Sound_PlayFar(221);
      if (mode != CARD_BRAMBLE) {
        work->transferMode = 2;
        work->transferValue = 50;
        REG_BLDCNT = 0x3f44;
      }
    }

    if (frame > 47) {
      // The background steps toward the card's colours.
      if (mode != CARD_BRAMBLE) {
        if (mode == CARD_FROST)
          Func_0815f0a0((s32)&ResourceId_CrystalSheet);
        else if (mode == CARD_THUNDER)
          Func_0815f0a0((s32)&ResourceId_EffectPalette02);
        else
          Func_0815f0a0((s32)&ResourceId_OrangePaletteB);
      }

      // The Frost and Flame cards throw one particle a frame at a target in
      // turn, until frame 95.
      if (mode == CARD_FROST || mode == CARD_FLAME) {
        s32 member = frame % effect->count;

        aim.x = targets[member].x + (Random16() & 63) - 32;
        if (mode == CARD_FROST)
          aim.y = targets[member].y + (Random16() & 255) - 96;
        else if (mode == CARD_FLAME)
          aim.y = targets[member].y + (Random16() & 63) + 32;
        if (frame <= 95) {
          struct EffectStep *step = &work->particles[frame % 56];

          step->x = card.x << 15;
          step->y = card.y << 16;
          step->velocityX = (aim.x - card.x) << 11;
          step->velocityY = (aim.y - card.y) << 11;
          step->z = 32;
          step->variant = 0;
        }
      }

      if (mode == CARD_FROST) {
        // The shards tumble through their three cells, bounce and slow.
        for (i = 0; i != 56; i++) {
          struct EffectStep *step = &work->particles[i];

          if ((u32)step->variant <= 1023) {
            s32 cell = i % 5 * 3 + step->variant / 96 % 3;

            draw[0](canvas, work->sheet + gData_08197834[cell] + 0x1000,
                    HI(step->x) - gData_0819781a[cell] / 2,
                    HI(step->y) - gData_08197826[cell] / 2,
                    gData_0819781a[cell], gData_08197826[cell]);
            BattleEffectKernels_IntegrateVector2(step, 64, 0);
            if (step->y > 96 << 16)
              step->velocityY = -step->velocityY / 2;
            step->variant += step->z;
            if (step->z > 1 && (frame & 1))
              step->z--;
          }
        }
      } else if (mode == CARD_FLAME) {
        // The smoke flies to the ground and plays its six cells.
        for (i = 0; i != 56; i++) {
          struct EffectStep *step = &work->particles[i];

          if (step->variant >= 0) {
            if (step->variant > 1)
              draw[0](canvas, work->sheet + 0x800 + step->variant / 4 * 0x480,
                      HI(step->x) - 12, HI(step->y) - 24, 24, 48);
            step->x += step->velocityX;
            step->y += step->velocityY;
            if (step->y > 96 << 16) {
              step->y = 96 << 16;
              step->velocityY = 0;
            }
            step->variant++;
            if (step->variant == 24)
              step->variant = -1;
          }
        }
      }
    }

    // The strikes: every five frames each target in turn reels, flashes and
    // throws up 16 sparks.
    if (mode == CARD_THUNDER)
      firstHit = 52;
    else if (mode == CARD_BRAMBLE)
      firstHit = 60;
    else
      firstHit = 68;
    if (mode == CARD_BRAMBLE)
      lastHit = 96;
    else
      lastHit = 110;
    if (frame >= firstHit && frame < lastHit) {
      s32 speed = (frame - 68) * 3;

      for (i = 0; i != effect->count; i++) {
        struct EffectStep *step = &work->particles[56 + i];

        if (--step->variant == 0) {
          s32 burst;
          s32 j;

          step->variant = 5;
          work->shakeFrames = 4;
          if (mode == CARD_FROST)
            Sound_PlayFar(134);
          else if (mode == CARD_THUNDER)
            Sound_PlayFar(177);
          else
            Sound_PlayFar(133);
          if (i == 0)
            Func_081180e8(134);
          BattleEffect_StartActorMotion(effect->actors[i], 1, 0x20000, 0x80000,
                                        0x10000, 100 + speed);
          Func_0814cd48(effect->actors[i], 7, -1, i, 8);
          burst = 0;
          EffectPosition_ProjectScaledBattler(effect->actors[i], &aim);
          for (j = 0; j != 64; j++) {
            struct EffectStep *spark = &SPARKS[i * 64 + j];

            if (spark->variant < 0) {
              s32 angle;
              s32 force;

              spark->x = aim.x << 15;
              spark->y = (aim.y + 8) << 16;
              angle = Random16() % 0xc000 + 0x2000;
              force = Random16() & 127;
              spark->velocityX = (Trig_Sin(angle) * force) >> 5;
              spark->velocityY = (Trig_Cos(angle) * force) >> 4;
              if (mode == CARD_THUNDER) {
                spark->velocityX /= 2;
                spark->velocityY /= 2;
              }
              spark->variant = (Random16() & 7) + 16;
              if (++burst == 16)
                break;
            }
          }
        }
      }
    }

    // This frame's models: the card, then the pictures.
    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    texture.logWidth = 5;
    texture.logHeight = 6;
    texture.pixels = work->sheet;
    model->kind = 8;
    model->texture = &texture;
    model->mesh = Data_081992b0;
    model->vertices = vertices;
    if (frame >= 0) {
      // The card: it swings from side to side as it rises, tilted back.
      Render_ResetTransformState();
      Func_080151e4(0x8000, 0x10000, 0x8000);
      Func_08015160((card.x << 16) - (128 << 16), (card.y << 16) - (64 << 16),
                    0);
      if (frame <= 47) {
        SceneTransform_ApplyPitch((48 - frame) << 10);
        if (effect->side == 0)
          Func_08015160(-Trig_Sin(frame * 682) << 7, 0, 0);
        else
          Func_08015160(Trig_Sin(frame * 682) << 7, 0, 0);
      }
      SceneTransform_ApplyYaw(0x1770);
      Func_080150e4(frame << 10);
      Func_0801521c(scale);
      Func_080151e4(0x10000, 0x20000, 0x10000);
      Render_TransformVertices(Data_081991e0, vertices, 4);
      Render_DrawModelList__f189b14(model);
      if (mode == CARD_BRAMBLE || mode == CARD_THUNDER) {
        Func_0801521c(0x4000);
        Render_TransformVertices(Data_081991e0, vertices, 4);
      }
    }

    // The Bramble and Thunder cards spread four pictures from the card's
    // corners for 64 frames; the Thunder Card's fade from frame 96.
    if ((mode == CARD_THUNDER || mode == CARD_BRAMBLE) &&
        (frame >= 48 && frame <= 111)) {
      s32 alpha = 0;

      if (mode == CARD_THUNDER && frame > 95)
        alpha = frame * -4 + 384;
      texture.logWidth = 5;
      texture.logHeight = 7;
      model->alpha = alpha;
      model->texture = &texture;
      model->mesh = gData_08199268;
      model->vertices = vertices;
      if (mode == CARD_BRAMBLE) {
        // The vines sway, each from its own phase.
        model->kind = 9;
        if (frame == 48) {
          s32 first = Random16();

          for (i = 0; i != 4; i++) {
            struct EffectStep *step = &work->particles[i];

            step->velocityY = i << 13;
            step->z = ((first + i) % 3 << 12) + 0x800;
          }
        }
        for (i = 0; i != 4; i++) {
          struct EffectStep *step = &work->particles[i];

          step->velocityZ =
              Trig_Sin(step->velocityY + (frame << 10)) / 16 + 12192;
        }
      } else {
        // The bolts flicker every other frame.
        model->kind = 7;
        if ((frame & 1) == 0) {
          s32 first = Random16();

          for (i = 0; i != 4; i++) {
            struct EffectStep *step = &work->particles[i];

            step->velocityZ = (Random16() & 0x1fff) + 8096;
            step->z = ((first + i) % 3 << 12) + 0x800;
          }
        }
      }
      for (i = 0; i != 4; i++) {
        work->particles[i].x = ((s16 *)vertices)[(i & 3) * 4];
        work->particles[i].y = ((s16 *)vertices)[(i & 3) * 4 + 1];
      }
      for (i = 0; i != 4; i++) {
        struct EffectStep *step = &work->particles[i];

        texture.pixels = work->sheet + step->z;
        Render_ResetTransformState();
        Func_08015160((step->x - 60) << 16, (step->y - 64) << 16, 0);
        Func_080151e4(0x8000, 0x10000, 0x8000);
        if (effect->side == 1)
          SceneTransform_ApplyYaw(0x8000);
        Func_080150e4(step->velocityZ);
        if (mode == CARD_BRAMBLE) {
          s32 length = (frame << 13) - 0x60000;

          if (length > 0x20000)
            length = 0x20000;
          if (frame > 95)
            length = length - (frame << 13) + 0xc0000;
          Func_080151e4(0x40000, length, 0x10000);
        } else {
          Func_080151e4(0x20000, 0x20000, 0x10000);
        }
        Render_TransformVertices(Data_081991c0, vertices, 4);
        Render_DrawModelList__f189b14(model);
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);

    // The sparks bounce once on the ground at y = 104.
    for (i = 0; i != SPARK_COUNT; i++) {
      struct EffectStep *step = &SPARKS[i];

      if (step->variant >= 0) {
        s32 size = step->variant >> 3;
        s32 j;

        if (mode == CARD_BRAMBLE || mode == CARD_THUNDER)
          size += 1;
        else
          size += 3;
        for (j = 0; j != Data_08199d64[mode].steps; j++) {
          draw[0](canvas, graphics + gData_08197410[size - 1],
                  HI(step->x) - size / 2, HI(step->y) - size, size, size * 2);
          if (step->y > 104 << 16) {
            step->y = 104 << 16;
            step->velocityY = -step->velocityY / 2;
          }
          BattleEffectKernels_IntegrateVector2(
              step, Data_08199d64[mode].damping, Data_08199d64[mode].gravity);
        }
        step->variant--;
      }
    }
    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

void Func_0818ad40(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Sleep Card (kind 92).
DEFINE_EFFECT_KIND(BattleEffect_RunSleepCard, Func_0818ad40, 0)

/// Abilities: Baffle Card (kind 90).
DEFINE_EFFECT_KIND(BattleEffect_RunBaffleCard, Func_0818ad40, 1)

/// Abilities: Death Card (kind 93).
DEFINE_EFFECT_KIND(BattleEffect_RunDeathCard, Func_0818ad40, 2)

/// Abilities: Sword Card (kind 91).
DEFINE_EFFECT_KIND(BattleEffect_RunSwordCard, Func_0818ad40, 3)
#undef HEAP_SLOT_BLITTER
#undef BLITTER
#undef HI
#undef SPARKS
#undef SPARK_COUNT
