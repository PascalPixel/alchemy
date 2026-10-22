// Draft: listing 1235 instructions, draft 1251, differ 882. No close ☀️ twin
// (call order BattleEffect_RunTitanBlade 0.459): ⚓️'s own effect on ☀️'s frame,
// its card and its pictures drawn as textured quads by ⚓️'s model kernels, as
// in Triple Chomp (BattleEffect_RunTripleChomp) and Legend (BattleEffect_RunLegend). The native frame
// (216) holds effect at sp+84, work 80, canvas 76, frame 72, the blitter 64,
// graphics 60, the card's scale 56, the card's position's address 52, the hit's
// speed 48, the texture's address 32, the member's actor offset 28, the
// member's sparks' offset 20 and 100 + (frame - 68) * 3 less 100 at 24; the
// texture is at sp+88, the aim at 96, the targets' positions at 108 and the
// card at 204. The mode stays in r9. Written from the listing (2026-10-15,
// fx-n3), in the native's statement order. Left: this frame is 208, two slots
// short below the blitter pair (every spill sits 8 lower), the mode takes r11
// for r9, and the aim's and the particle's addresses are kept in registers
// where the native reloads them, which adds 18 instructions. Next to try: the
// strike block's temporaries (the native spills the actor offset, the sparks'
// offset and the hit's speed as separate induction variables) and the order of
// the scalar declarations.
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The Lost Age's heap slots, by byte offset into Ram_HeapSlots.
#define HEAP_SLOT_BLITTER 104

// The transfer callback runs at this order, where ☀️'s runs at 0x480.
#define TRANSFER_CALLBACK_ORDER 0xc80

// ⚓️'s blitter modes count the other way from ☀️'s COMPOSITE_* modes.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };

// BattleEffect_LoadBlitter's packed argument: the mode in the high nibble.
#define BLITTER(flags, mode) ((mode) << 4 | (flags))

#define HI(v) (((s16 *)&(v))[1])

// The sparks live in the map cell buffer's second 16 KB, 64 for each
// member.
#define SPARKS ((struct EffectStep *)(Ram_MapCellBuffer + 0x4000))
#define SPARK_COUNT 512

// The four cards, as the wrappers pass them.
enum {
  CARD_BRAMBLE, // Bramble Card, kind 88: vines grow from the card.
  CARD_FROST,   // Frost Card, kind 87: the card throws crystal shards.
  CARD_FLAME,   // Flame Card, kind 79: the card throws smoke and flashes.
  CARD_THUNDER, // Thunder Card, kind 89: bolts strike from the card.
};

// A texture for the model kernels: its size as powers of two, then its
// pixels.
struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  void *pixels;
};

// One entry of a model list, as Render_AllocateModelList allocates them and
// Render_DrawModelList draws them.
struct Model {
  s32 kind;
  s32 next;
  void *mesh;
  void *vertices;
  struct ModelTexture *texture;
  s32 alpha;
  s32 tail;
};

// How each card's sparks move: the kernel's gravity and damping, and how
// many steps (each drawn) a frame.
struct SparkMotion {
  s32 gravity;
  s32 damping;
  s32 steps;
};

extern char ResourceId_Data134;
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

// The quad meshes and corners: the card, the pictures and their spread.
extern u8 Data_081992b0[];
extern u8 gData_08199268[];
extern s32 Data_081991e0[];
extern s32 Data_081991c0[];

// The spark cells: offsets into the particle sheet by size.
extern u16 gData_08197410[];

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_LoadBlitter(s32 slot, s32 packed);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Audio_PlayCue(s32 cue);
void EffectPosition_ProjectSprite(s32 id,
                                             struct EffectPosition *position);
void Func_0815f0a0(s32 resource);
void Func_081180e8(s32 phase);
void BattleEffect_StartActorMotion(s32, s32, s32, s32, s32, s32);
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void *Runtime_BumpAllocateAlternatePool(s32 size);
struct Model *Render_AllocateModelList(s32 count);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z);
void Func_08015160(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Func_080150e4(s32 angle);
void Func_0801521c(s32 scale);
void Render_TransformVertices(void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);
void MemFree(void *block);
void Func_08158ce0(s32 randomMask, u32 shakeRange);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);
void Runtime_ReleaseHeapBlock(s32 slot);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);

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
  struct EffectStep *step;
  struct Model *model;
  void *vertices;
  s32 i;
  s32 j;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up the canvas, a clipped additive blitter, the sparks, and the
  // card's picture and its extras.
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(HEAP_SLOT_BLITTER,
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
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, TRANSFER_CALLBACK_ORDER);

  for (frame = 0; frame != Data_08199d60[mode]; frame++) {
    // Frame 0: clear the particles, give each member a strike countdown, and
    // find the user and every target.
    if (frame == 0) {
      Audio_PlayCue(190);
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
        Audio_PlayCue(148);
      if (mode == CARD_FROST)
        Audio_PlayCue(144);
      if (mode == CARD_FLAME)
        Audio_PlayCue(104);
      if (mode == CARD_THUNDER)
        Audio_PlayCue(221);
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
          step = &work->particles[frame % 56];
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
          step = &work->particles[i];
          if (step->variant >= 0 && step->variant <= 767) {
            s32 cell = i % 5 * 3 + step->variant / 96 % 3;

            draw[0](canvas, work->sheet + 0x1000 + gData_08197834[cell],
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
          step = &work->particles[i];
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
    if (frame >= (mode == CARD_THUNDER   ? 52
                  : mode == CARD_BRAMBLE ? 60
                                         : 68) &&
        frame < (mode == CARD_BRAMBLE ? 96 : 110)) {
      for (i = 0; i != effect->count; i++) {
        step = &work->particles[56 + i];
        if (--step->variant == 0) {
          s32 burst;

          step->variant = 5;
          work->shakeFrames = 4;
          if (mode == CARD_FROST)
            Audio_PlayCue(134);
          else if (mode == CARD_THUNDER)
            Audio_PlayCue(177);
          else
            Audio_PlayCue(133);
          if (i == 0)
            Func_081180e8(134);
          BattleEffect_StartActorMotion(effect->actors[i], 1, 0x20000, 0x80000,
                                    0x10000, 100 + (frame - 68) * 3);
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
    vertices = Runtime_BumpAllocateAlternatePool(32);
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
      Func_08015160((card.x - 128) << 16, (card.y - 64) << 16, 0);
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
      Render_DrawModelList(model);
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
        alpha = -(frame * 4) + 384;
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
            work->particles[i].velocityY = i << 13;
            work->particles[i].z = ((first + i) % 3 << 12) + 0x800;
          }
        }
        for (i = 0; i != 4; i++)
          work->particles[i].velocityZ =
              Trig_Sin(work->particles[i].velocityY + (frame << 10)) / 16 +
              12192;
      } else {
        // The bolts flicker every other frame.
        model->kind = 7;
        if ((frame & 1) == 0) {
          s32 first = Random16();

          for (i = 0; i != 4; i++) {
            work->particles[i].velocityZ = (Random16() & 0x1fff) + 8096;
            work->particles[i].z = ((first + i) % 3 << 12) + 0x800;
          }
        }
      }
      for (i = 0; i != 4; i++) {
        work->particles[i].x = ((s16 *)vertices)[(i & 3) * 4];
        work->particles[i].y = ((s16 *)vertices)[(i & 3) * 4 + 1];
      }
      for (i = 0; i != 4; i++) {
        step = &work->particles[i];
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
        Render_DrawModelList(model);
      }
    }
    MemFree(model);
    MemFree(vertices);

    // The sparks bounce once on the ground at y = 104.
    for (i = 0; i != SPARK_COUNT; i++) {
      step = &SPARKS[i];
      if (step->variant >= 0) {
        s32 size = step->variant >> 3;

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
          BattleEffectKernels_IntegrateVector2(step, Data_08199d64[mode].damping,
                                           Data_08199d64[mode].gravity);
        }
        step->variant--;
      }
    }
    Func_08158ce0(4, 4);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
