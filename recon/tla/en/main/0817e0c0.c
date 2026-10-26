// 2026-10-26: differ 127 of 601, with Scheduler_AddOrUpdateCallback returning
// s32 as defined.
// Draft (tla-en): listing 601 instructions, draft 600, common 530, differ 141.
// The body of effect kinds 311, 314, 326, 339, 433 and 457 (variant_j.c's
// BattleEffect_RunBoneCharge to BattleEffect_RunLivingArmor pass modes 4, 1, 2, 3, 4 and 5). Written
// from the listing on ☀️'s skeleton; its nearest ☀️ shape is
// BattleEffect_RunRushingStrike (0.54), which is no twin.
// Written 2026-10-15 (lane N2). The frame matches (texture sp+44, the
// blitter pair sp+52, screen sp+60, caster sp+72, target sp+84; mode,
// canvas, frame, camera and the spark cels spilled at sp+40 down to sp+24).
// Left:
// - after the scheduler call the native copies the effect argument from
// fp into r5 once and loads its fields through r5 for the lunge, the
// target object and its place; here each load copies fp afresh.
// - the prologue loads work before camera and sets the sheet offset 0x700
// after the heap loads; the blitter pair's address goes straight to r1.
// - the streak quad's stores are scheduled in another order, the native
// setting brightness's 0 into r1 early.
// - the hit frame keeps frame in r5 for the flash's 8 (here r3), and a
// few argument moves elsewhere.
// Measured: the first draft 485; the flash's 128x128 size as two byte
// stores (the native strb through the hoisted &texture; the streak's is
// a masked word): 447; frame declared before camera (its spill slot
// then comes before camera's and the spark cels'): 435; the three
// EffectPositions declared target, caster, screen (their slots then
// match): 433; mode 5's cel computed before the projection call (the
// native rounds the division before the call): 349; the spark size
// computed before the call likewise: 201; a block-scoped pointer for
// each particle loop (the step pointer then takes r5 before the hoisted
// 255, as native): 145.
// alchemy permute, 10 minutes, two jobs: 127 with the streak's ~0xff mask
// forced into a temporary before the loop (not taken: a device for 18
// rows of register choice).
// 2026-10-19 (run 33): work declared before canvas (the native loads canvas
// into r2 and work into r1 for r9): 145 to 141.
// Resources 0x134 (Resource_Data134), 0xc2 (Resource_Data0C2), 0x181
// (Resource_Data181), 0x13e (BattleEffect_FlashBurstSheet), 0x186, 0x17d,
// 0x17f, 0x184 and 0x148 need names in system/resource/directory.s when
// this is adopted.
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_session.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "types.h"

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
struct Texture {
  u8 logWidth;
  u8 logHeight;
  u16 unk02;
  void *pixels;
};

#define TEXTURE_SIZE(texture, logWidth, logHeight)                             \
  do {                                                                         \
    *(u32 *)&(texture) = (*(u32 *)&(texture) & ~0xff) | (logWidth);            \
    *(u32 *)&(texture) = (*(u32 *)&(texture) & ~0xff00) | (logHeight) << 8;    \
  } while (0)

// One entry of the list Render_DrawModelList draws; kinds 6 and 7 are textured
// quads.
struct RenderPrimitive {
  s32 kind;
  s32 unk04;
  const void *faces;
  void *vertices;
  struct Texture *texture;
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
extern struct EffectStep gData_02014000[];

// The streak quad's faces (32x64) and the flash quad's (128x128), and the
// corners of a 64x64 square around the origin, which both scale.
extern const u8 Data_081992b0[];
extern const u8 gData_08199364[];
extern const u8 Data_081991e0[];

// The streak's scale per mode, 16.16.
extern const s32 Data_081994b8[];

// The offsets of the spark cels: cel n is n pixels wide and 2n tall.
extern const u16 gData_08197410[];

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_FetchRectangleBlitters(s32 side, DrawRectangle *pair);
void *Resource_GetTableEntry(s32 resourceId);
s32 Scheduler_AddOrUpdateCallback(s32 callback, s32 priority);
s32 Scheduler_RemoveCallback(s32 callback);
void BattleEffect_SetApproachMotion(s32 id, s32 targetId, s32 frames, s32 rate);
void ObjectDispatch_SetAnimationSpeedFar(struct MotionObject *object,
                                            s32 value);
void EffectPosition_ProjectBattler(
    s32 id, struct EffectPosition *position);
u32 Random16(void);
void *Runtime_BumpAllocateAlternatePool(s32 size);
struct RenderPrimitive *Render_AllocateModelList(s32 count);
void Render_ResetTransformState(void);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_080151e4(s32 x, s32 y, s32 z);
void SceneTransform_ApplyYaw(s32 angle);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct RenderPrimitive *list);
void MemFree(void *buffer);
void Func_0814cd48(s32 actor, s32 palette, s32 member, s32 timer, s32 frames);
void Func_08118088(s32 actor, s32 motion);
void Func_081180e8(s32 phase);
void Transform_SetLookAt(s32 *eye, s32 *target);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_08158ce0(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);
void Runtime_ReleaseHeapBlock(s32 slot);
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
  struct Texture texture;
  DrawRectangle draw[2];
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
  struct RenderPrimitive *quad;
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
  Scheduler_AddOrUpdateCallback((s32)BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The caster lunges at the target.
  BattleEffect_SetApproachMotion(effect->actor, effect->actors[0], 10, 80);
  ObjectDispatch_SetAnimationSpeedFar(
      GetBattleObjectSlotFar(effect->actor)->object, 48);
  targetObject = GetBattleObjectSlotFar(effect->actors[0])->object;
  EffectPosition_ProjectBattler(effect->actors[0], &target);

  for (frame = 0; frame != LUNGE_FRAMES; frame++) {
    // The first frame readies the sparks and mode 5's bursts at the
    // target, 8 pixels to the outside of it, flying back the other way.
    if (frame == 0) {
      struct EffectStep *spark = gData_02014000;
      struct EffectStep *burst;

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
      burst = work->particles;
      for (i = 0; i != SPARK_COUNT; i++, burst++) {
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
    vertices = Runtime_BumpAllocateAlternatePool(32);
    quad = Render_AllocateModelList(1);
    TEXTURE_SIZE(texture, 5, 6);
    texture.pixels = work->sheet;
    quad->texture = &texture;
    quad->faces = Data_081992b0;
    quad->kind = 6;
    quad->vertices = vertices;
    quad->brightness = 0;
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
      quad->kind = 7;
      texture.pixels = Ram_MapCellBuffer;
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
    MemFree(quad);
    MemFree(vertices);

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
      Transform_SetLookAt(camera, camera + 3);
      spark = gData_02014000;
      for (i = 0; i != SPARK_COUNT; i++, spark++) {
        s32 life = spark->variant;

        if (life >= 0) {
          s32 size = (life >> 4) + 2;

          EffectPosition_ProjectPoint(&spark->x, &screen);
          screen.x >>= 1;
          draw[i & 1](canvas, sparkCels + gData_08197410[size - 1],
                      screen.x - size / 2, screen.y - size, size, size * 2);
          BattleEffectKernels_IntegrateVector3(spark, 60, -0x200);
          spark->variant--;
        }
      }

      // Mode 5's flame bursts rise through six 32x64 cels in 24 frames.
      if (mode == 5) {
        struct EffectStep *burst = work->particles;

        for (i = 0; i != BURST_COUNT; i++, burst++) {
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

    Func_08158ce0(8, 8);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
