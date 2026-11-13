/// @file
/// Triple Chomp, Rolling Flame and Ghost Soldier.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

// The Lost Age's heap slots, by byte offset into Ram_HeapSlots.
#define HEAP_SLOT_BLITTER 104

// The transfer task runs at this priority, where The Broken Seal's runs at
// 0x480.
#define TASK_PASS_FRAME 0xc80

// BattleEffect_LoadBlitter's packed argument: the mode in the high nibble.
#define BLITTER(flags, mode) ((mode) << 4 | (flags))

// The sparks live in the map cell buffer's second 16 KB.
#define SPARKS ((struct EffectStep *)(Ram_MapCellBuffer + 0x4000))
#define SPARK_COUNT 512
#define HI(v) (((s16 *)&(v))[1])

// Each bite's anchor, where its jaw is drawn: the first three particles.
#define BITE_ANCHOR(bite) (&work->particles[bite])

// A texture for the model kernels: its size as powers of two, then its
// pixels.

// One entry of a model list, as Render_AllocateModelList allocates them and
// Render_DrawModelList draws them.
/* VIEW(f18b868): tag struct Model as struct Model__f18b868; ability_effects.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/battle_effect/ability_effects.c:39584: structure has no member
 * named `vertices'. */
struct Model__f18b868 {
  s32 kind;
  s32 next;
  const void *mesh;
  void *vertices;
  struct ModelTexture *texture;
  s32 alpha;
  s32 tail;
};

extern char ResourceId_Data0C2;
extern char ResourceId_Data134;
extern char ResourceId_BlueFlameSheet;

// The bite's quad: its mesh and its four corners, 192 x 128 around the
// origin.
extern const u8 gData_08199364[];
extern const u8 gData_08199210[];

// The three bites' frames: 0, 12 and 24.
extern u8 gData_08199d94[];

// The spark cells: offsets into the particle sheet by size.
extern u16 gBattleEffect_MoteCells[];

void BattleEffect_StartCanvasLayer(s32 bgControl);

void BattleEffect_SetApproachMotion(s32 actor, s32 target, s32 frames,
                                    s32 percent);
s32 BattleEffect_LoadBlitter(s32 slot, s32 packed);
struct PoolEntry *Render_AllocateModelList(s32 count);
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position);
void BattlePlayback_StartFar(s32 phase);
void BattleEffect_StartActorMotion(s32, s32, s32, s32, s32, s32);
void BattleEffect_TintBattler(s32 actor, s32 palette, s32 group, s32 timerSlot,
                              s32 frames);
void SceneTransform_ApplyScale3(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPosition3(s32 x, s32 y, s32 z);
void Transform_ApplyRoll(s32 angle);
void Transform_ApplyPitch(s32 angle);
void SceneTransform_ApplyUniformScale(s32 scale);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
/* VIEW(f18b868): routine Render_DrawModelList as void
 * Render_DrawModelList(struct Model__f18b868 *model); f18ad40 has void
 * Render_DrawModelList(struct Model *model). */
extern void Render_DrawModelList__f18b868(struct Model__f18b868 *model) __asm__(
    "Render_DrawModelList");
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_EndCanvasLayer(void);

// Triple Chomp (effect kind 426): the caster lunges 80% of the way to the
// target, and three bites close on it at frames 0, 12 and 24. Each bite
// throws 32 blue sparks that fly off away from the caster, jolts and flashes
// the target, and draws a jaw: a textured quad tilted back, turned a quarter
// further each bite, that grows and fades over 16 frames.
/// Triple Chomp's effect: the caster lunges, three bites close on the target.
void BattleEffect_RunTripleChomp(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2]; // Unused: [1] gives the stack frame its 4 bytes.
  u8 *graphics;
  void *vertices;
  struct Model__f18b868 *model;
  struct ModelTexture texture;
  struct EffectPosition position;
  struct EffectStep *drawn;
  s32 i;
  s32 j;
  u8 *pixels = Ram_MapCellBuffer;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up the canvas, then let the caster lunge.
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_SetApproachMotion(effect->actor, effect->actors[0], 8, 80);
  wait(8);

  // A clipped, mirrored MAX blitter for the sparks; the jaw's texture, the
  // spark sheet and the blue flame's colours.
  BattleEffect_LoadBlitter(
      HEAP_SLOT_BLITTER,
      BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
              BLIT_MAX));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, pixels, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Iwram_CopyWordsEntry((void *)0x05000000,
                       Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
                       128);

  for (frame = 0; frame != 50; frame++) {
    if (frame == 0) {
      for (i = 0; i != SPARK_COUNT; i++)
        SPARKS[i].variant = -1;
    }

    // This frame's jaw: one model drawing the quad with the 128 x 128
    // texture.
    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    EffectPosition_ProjectBattler(effect->actors[0], &position);
    texture.logWidth = 7;
    texture.logHeight = 7;
    texture.pixels = pixels;
    model->mesh = gData_08199364;
    model->kind = 7;
    model->texture = &texture;
    model->vertices = vertices;

    // The three bites; the native counts them in the same register as the
    // spark loops, as one variable.
    for (i = 0; i != 3; i++) {
      // The bite closes: 32 sparks fly from the target's middle, and the
      // target jolts and flashes. The last bite ends the action.
      if (frame == gData_08199d94[i]) {
        for (j = 0; j != 32; j++) {
          struct EffectStep *spark = &SPARKS[i * 128 + j];
          s32 angle;
          s32 force;

          spark->x = (position.x / 2) << 16;
          spark->y = (position.y - 24) << 16;
          angle = Random16();
          force = (Random16() & 255) + 32;
          spark->velocityX = (Trig_Sin(angle) * force) >> 8;
          spark->velocityY = (Trig_Cos(angle) * force) >> 6;
          if (effect->side == 0)
            spark->velocityX += -0x8000;
          else
            spark->velocityX += 0x8000;
          BITE_ANCHOR(i)->x = position.x;
          BITE_ANCHOR(i)->y = position.y - 24;
          spark->variant = (Random16() & 7) + 16;
        }
        if (i == 2)
          BattlePlayback_StartFar(134);
        else
          Sound_PlayFar(134);
        BattleEffect_StartActorMotion(effect->actors[0], 1, 0x20000, 0x80000, 0,
                                      i * 20 + 120);
        BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 8);
        work->shakeFrames = 8;
      }

      // For 16 frames the jaw grows from a quarter of its size and fades.
      if (frame >= gData_08199d94[i] && frame < gData_08199d94[i] + 16) {
        s32 age = frame - gData_08199d94[i];
        s32 alpha = 64 - age * 8;
        s32 scale = age * 10192 + 0x4000;

        if (alpha > 0)
          alpha = 0;
        if (alpha > -64) {
          model->alpha = alpha;
          Transform_Reset();
          SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
          SceneTransform_ApplyPosition3((BITE_ANCHOR(i)->x - 128) << 16,
                                        (BITE_ANCHOR(i)->y - 64) << 16, 0);
          Transform_ApplyRoll((i << 14) + 0x2000);
          Transform_ApplyPitch(-0x1000);
          SceneTransform_ApplyUniformScale(scale);
          Render_TransformVertices(gData_08199210, vertices, 4);
          Render_DrawModelList__f18b868(model);
        }
      }
    }

    // Each spark is drawn and moved twice a frame, shrinking as it dies.
    drawn = SPARKS;
    for (i = 0; i != SPARK_COUNT; i++) {
      s32 left = drawn->variant;

      if (left >= 0) {
        s32 size = (left >> 2) + 3;

        draw[0](canvas, graphics + gBattleEffect_MoteCells[size - 1],
                HI(drawn->x) - size / 2, HI(drawn->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(drawn, 62, 0);
        draw[0](canvas, graphics + gBattleEffect_MoteCells[size - 1],
                HI(drawn->x) - size / 2, HI(drawn->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(drawn, 62, 0);
        drawn->variant--;
      }
      drawn++;
    }

    Heap_Release(model);
    Heap_Release(vertices);
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  HeapSlot_Release(HEAP_SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

void BattleEffect_RunBreaths(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Beam, Cycle Beam, Searing Beam (kind 73).
DEFINE_EFFECT_KIND(BattleEffect_RunBeam, BattleEffect_RunBreaths, 0)

/// Abilities: Rolling Flame (kind 434).
void BattleEffect_RunRollingFlame(struct BattleEffectArgument *effect) {
  effect->variant = 2;
  BattleEffect_RunBreaths(effect, 1);
}

/// Abilities: Ghost Soldier, Pike Assault (kind 449).
void BattleEffect_RunGhostSoldier(struct BattleEffectArgument *effect) {
  effect->variant = 2;
  BattleEffect_RunBreaths(effect, 2);
}

/// Abilities: Blast Breath (kind 414).
DEFINE_EFFECT_KIND(BattleEffect_RunBlastBreath, BattleEffect_RunBreaths, 3)
#undef HEAP_SLOT_BLITTER
#undef BLITTER
#undef SPARKS
#undef SPARK_COUNT
#undef HI
#undef BITE_ANCHOR
