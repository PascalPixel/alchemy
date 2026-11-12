/// @file
/// Routines whose twins are in The Broken Seal's
/// battle_effect/battle_effect/mirrored_wall.c.
// Part of f15585c.c.

#include "../f15585c_internal.h"

struct Model;

#define HEAP_SLOT_BLITTER 104           // Ram_HeapSlots->unk64[1]
#define HEAP_SLOT_BLITTER_ALTERNATE 188 // Ram_HeapSlots->unkb8[1]

#define LUNGE_FRAMES 60
#define LUNGE_STREAK_FRAMES 15 // the streak follows the caster
#define LUNGE_HIT_FRAME 8      // the caster reaches the target
#define LUNGE_FLASH_FRAMES 12  // the flash at the target grows and fades
#define SPARK_COUNT 64
#define BURST_COUNT 16 // mode 5's flame bursts

// A texture: log2 of its width and height in the first word, then its
// pixels.
/* VIEW(f17e0c0): tag struct Texture as struct Texture__f17e0c0; f15585c.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/battle_effect/f15585c.c:29798: structure has no member named `logWidth'. */
struct Texture__f17e0c0 {
  u8 logWidth;
  u8 logHeight;
  u16 unk02;
  void *pixels;
};

// One entry of the list Render_DrawModelList draws; kinds 6 and 7 are textured
// quads.
/* VIEW(f17e0c0): tag struct RenderPrimitive as struct RenderPrimitive__f17e0c0; f15585c.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/battle_effect/f15585c.c:29770: structure has no member named `brightness'. */
struct RenderPrimitive__f17e0c0 {
  s32 kind;
  s32 unk04;
  const void *faces;
  void *vertices;
  struct Texture__f17e0c0 *texture;
  s32 brightness;
};

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

extern char ResourceId_Data134;
extern char ResourceId_Data0C2;
extern char ResourceId_Data181;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_FirePillarSheetB;
extern char ResourceId_IceBlockSheet;
extern char ResourceId_PinkBurstSheet;
extern char ResourceId_BlastSheet;
extern char ResourceId_OrangePaletteB;

// The 64 sparks, past the map cells' first 16 KB.
extern u8 gData_02014000[];

// The streak quad's faces (32x64) and the flash quad's (128x128), and the
// corners of a 64x64 square around the origin, which both scale.
extern u8 Data_081992b0[];
extern const u8 gData_08199364[];
extern const u8 Data_081991e0[];

// The streak's scale per mode, 16.16.
extern const s32 Data_081994b8[];

// The offsets of the spark cels: cel n is n pixels wide and 2n tall.
extern u16 gData_08197410[];

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void *Resource_GetTableEntry(s32 resourceId);

void BattleEffect_SetApproachMotion(s32 id, s32 targetId, s32 frames, s32 rate);

void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position);
u32 Random16(void);
void *Heap_AllocEwram(s32 size);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_ResetTransformState(void);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_080151e4(s32 x, s32 y, s32 z);
void SceneTransform_ApplyYaw(s32 angle);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);
void Heap_Release(void *buffer);
void Func_0814cd48(s32 actor, s32 palette, s32 member, s32 timer, s32 frames);
void Func_08118088(s32 actor, s32 motion);
void Func_081180e8(s32 phase);
void Transform_SetLookAt(s32 *eye, s32 *target);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffect_CountDownBattlerTints(void);
void wait(s32 frames);
void HeapSlot_Release(s32 slot);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_EndCanvasLayer(void);

// A lunge. The caster runs 80 percent of the way to the target in ten
// frames behind a glowing streak (a 32x32 flare and its reflection); on
// frame 8 it strikes: the target flashes and is knocked back, a 128x128
// flash stretched four times taller grows over it and fades in twelve
// frames, and 64 sparks fly out of it to the caster's side and burn down.
// Mode 5 adds sixteen rising flame bursts. The mode also picks the palette:
// fire pillar, ice block, pink burst, blast (modes 3 and 5) or orange.
void Func_0817e0c0(struct BattleEffectArgument *effect, s32 mode) {
  struct Texture__f17e0c0 texture;
  DrawRectangle draw[2];
  u8 *flashPixels = Ram_MapCellBuffer;
  s32 height;
  struct EffectPosition target;
  struct EffectPosition caster;
  struct EffectPosition screen;
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  s32 frame;
  s32 *camera = Ram_HeapSlots->cameraWork;
  u8 *sparkCels = Ram_HeapSlots->unk64[0];
  struct MotionObject *targetObject;
  struct RenderPrimitive__f17e0c0 *quad;
  void *vertices;
  s32 i;
  s32 row;
  s32 column;
  s32 scale;
  s32 flashFrame;
  s32 brightness;
  s32 resource;

  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sparkCels, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, Ram_MapCellBuffer, 0, 0);

  // The streak: the 32x32 flare, then the same flare upside down below it.
  Resource_LoadAndDecompress((s32)&ResourceId_Data181, work->sheet, 0, 0);
  for (row = 0; row != 32; row++)
    for (column = 0; column != 32; column++)
      work->sheet[0x400 + (31 - row) * 32 + column] =
          work->sheet[row * 32 + column];
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x800, 1, 0);
  switch (mode) {
  case 0:
    resource = (s32)&ResourceId_FirePillarSheetB;
    break;
  case 1:
    resource = (s32)&ResourceId_IceBlockSheet;
    break;
  case 2:
    resource = (s32)&ResourceId_PinkBurstSheet;
    break;
  case 3:
  case 5:
    resource = (s32)&ResourceId_BlastSheet;
    break;
  default:
    resource = (s32)&ResourceId_OrangePaletteB;
    break;
  }
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The caster lunges at the target.
  BattleEffect_SetApproachMotion(effect->actor, effect->actors[0], 10, 80);
  Actor_SetAnimationSpeedFar(GetBattleObjectSlotFar(effect->actor)->object, 48);
  targetObject = GetBattleObjectSlotFar(effect->actors[0])->object;
  EffectPosition_ProjectBattler(effect->actors[0], &target);

  for (frame = 0; frame != LUNGE_FRAMES; frame++) {
    // The first frame readies the sparks and mode 5's bursts at the
    // target, 8 pixels to the outside of it, flying back the other way.
    if (frame == 0) {
      struct EffectStep *spark = gData_02014000;

      for (i = 0; i != SPARK_COUNT; i++, spark++) {
        spark->x = targetObject->x;
        if (spark->x < 0)
          spark->x += -8 << 16;
        else
          spark->x += 8 << 16;
        spark->y = targetObject->y + (14 << 16);
        spark->z = targetObject->z;
        spark->velocityX = (Random16() & 511) << 11;
        spark->velocityY = ((Random16() & 255) - 64) << 11;
        spark->velocityZ = ((Random16() & 255) - 128) << 11;
        if (spark->x > 0)
          spark->velocityX = -spark->velocityX;
        spark->variant = i / 2 + 16;
      }
      for (i = 0; i != SPARK_COUNT; i++) {
        struct EffectStep *burst = &work->particles[i];
        burst->x = targetObject->x;
        if (burst->x < 0)
          burst->x += -8 << 16;
        else
          burst->x += 8 << 16;
        burst->y = targetObject->y + (14 << 16);
        burst->z = targetObject->z;
        burst->velocityX = ((Random16() & 511) - 256) << 10;
        burst->velocityY = ((Random16() & 255) - 64) << 10;
        burst->velocityZ = ((Random16() & 255) - 128) << 10;
        if (burst->x > 0)
          burst->velocityX = -burst->velocityX;
        burst->variant = 0;
      }
    }

    // The streak, on the caster for its first 15 frames.
    EffectPosition_ProjectBattler(effect->actor, &caster);
    vertices = Heap_AllocEwram(32);
    quad = Render_AllocateModelList(1);
    *(u32 *)&texture = (*(u32 *)&texture & ~0xff) | 5;
    brightness = 0;
    *(u32 *)&texture = (*(u32 *)&texture & ~0xff00) | 6 << 8;
    texture.pixels = work->sheet;
    quad->texture = &texture;
    quad->faces = Data_081992b0;
    quad->kind = 6;
    quad->vertices = vertices;
    quad->brightness = brightness;
    if (frame < LUNGE_STREAK_FRAMES) {
      Render_ResetTransformState();
      Func_080151e4(0x8000, 0x10000, 0x8000);
      Func_08015160((caster.x - 128) << 16, (caster.y - 88) << 16, 0);
      Func_080151e4(Data_081994b8[mode] << 1, Data_081994b8[mode] << 1,
                    Data_081994b8[mode]);
      if (effect->side == 1)
        SceneTransform_ApplyYaw(0x8000);
      Render_TransformVertices(Data_081991e0, vertices, 4);
      Render_DrawModelList(quad);
    }

    // The flash at the target grows for twelve frames from the hit and
    // darkens from its fifth.
    flashFrame = frame - LUNGE_HIT_FRAME;
    if (frame >= LUNGE_HIT_FRAME &&
        frame < LUNGE_HIT_FRAME + LUNGE_FLASH_FRAMES) {
      if (mode == 4)
        scale = (flashFrame << 12) + 0x1000;
      else
        scale = flashFrame * 3048 + 3048;
      brightness = 32 - flashFrame * 8;
      if (brightness > 0)
        brightness = 0;
      quad->brightness = brightness;
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = flashPixels;
      quad->kind = 7;
      quad->faces = gData_08199364;
      Render_ResetTransformState();
      Func_080151e4(0x8000, 0x10000, 0x8000);
      if (effect->side == 0)
        Func_08015160((target.x - 144) << 16, (target.y - 84) << 16, 0);
      else
        Func_08015160((target.x - 112) << 16, (target.y - 84) << 16, 0);
      Func_080151e4(scale, scale << 2, scale);
      Render_TransformVertices(Data_081991e0, vertices, 4);
      Render_DrawModelList(quad);
    }
    Heap_Release(quad);
    Heap_Release(vertices);

    // The hit.
    if (frame == LUNGE_HIT_FRAME) {
      Func_0814cd48(effect->actors[0], 7, 5, 0, LUNGE_HIT_FRAME);
      Func_08118088(effect->actors[0], 4);
      if (mode == 5)
        Func_081180e8(145);
      else
        Func_081180e8(134);
      work->shakeFrames = 8;
    }

    // From the hit on, the sparks fly and burn down, drawn by turns with
    // the two blitters; their cel shrinks as they burn.
    if (frame >= LUNGE_HIT_FRAME && frame < 64) {
      struct EffectStep *spark;

      Render_ResetTransformState();
      spark = gData_02014000;
      /* FAKEMATCH(barrier): an empty asm on spark, which ranks it over the
       * screen pointer; with spark set after the look-at and no asm, 2 of 601
       * instructions differ in tla-en. */
      __asm__("" : "=r"(spark) : "0"(spark));
      Transform_SetLookAt(camera, camera + 3);
      for (i = 0; i != SPARK_COUNT; i++, spark++) {
        s32 life = spark->variant;

        if (life >= 0) {
          s32 size = (life >> 4) + 2;

          EffectPosition_ProjectPoint(&spark->x, &screen);
          screen.x >>= 1;
          height = size * 2;
          draw[i & 1](canvas, sparkCels + gData_08197410[size - 1],
                      screen.x - size / 2, screen.y - size, size, height);
          BattleEffectKernels_IntegrateVector3(spark, 60, -0x200);
          spark->variant--;
        }
      }

      // Mode 5's flame bursts rise through six 32x64 cels in 24 frames.
      if (mode == 5) {

        for (i = 0; i != BURST_COUNT; i++) {
          struct EffectStep *burst = &work->particles[i];
          s32 life = burst->variant;

          if (life >= 0 && life < 24) {
            s32 cel = life / 4;

            EffectPosition_ProjectPoint(&burst->x, &screen);
            screen.x >>= 1;
            draw[0](canvas, work->sheet + 0x800 + (cel << 11), screen.x - 16,
                    screen.y - 32, 32, 64);
            BattleEffectKernels_IntegrateVector3(burst, 60, 0x2000);
            burst->variant++;
          }
        }
      }
    }

    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

struct BattleEffectArgument;

void Func_0817e698(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Spasm (kind 348).
void BattleEffect_RunSpasm(s32 a0) { Func_0817e698(a0, 0); }

/// Abilities: Berserk (kind 353).
void BattleEffect_RunBerserk(s32 a0) { Func_0817e698(a0, 1); }

/// Abilities: Recovery (kind 389).
void BattleEffect_RunRecovery(s32 a0) { Func_0817e698(a0, 2); }

/// Abilities: Sack (kind 431).
void BattleEffect_RunSack(s32 a0) { Func_0817e698(a0, 3); }
#undef HEAP_SLOT_BLITTER
#undef HEAP_SLOT_BLITTER_ALTERNATE
#undef LUNGE_FRAMES
#undef LUNGE_STREAK_FRAMES
#undef LUNGE_HIT_FRAME
#undef LUNGE_FLASH_FRAMES
#undef SPARK_COUNT
#undef BURST_COUNT
#undef HI
