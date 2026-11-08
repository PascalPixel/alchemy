// Differ 12 of 1150 (tla-en, 2026-11-08, -mtune=gba): order only, the first loop's movs r4
// (line) one place early, and the texture block's r0/r2 and r1/r0 reloads (the native loads
// gData_02014000 into r0 after moving the model to r6). 8 November, second round: the mote
// loop's i = 0 set before cell as its own statement (14 to 12; after cell 14); line or j
// set outside the for (12 to 20); pixels first, between the bitfields, before the allocation,
// through a local or in a once-only block, and the model stores reordered (12 to 119).
// 8 November: the texture block's empty-asm barrier replaced by a once-only block around the
// vertices store (14, no device; without either 16). The wrapper still measures 2 (direct call,
// a constant local or a block all 16 to 18). No gain: the motes loop's init orders (14 to 22),
// the texture stores reordered (16 to 24).
// Differ 22 of 1150 (tla-en, 2026-11-06). Left: order only: movs r4/add r2,fp
// at the start, movs r3 #58 twice, and the texture block's r0/r2 and r1/r0 reloads.

// Draft of BattleEffect_RunZagan, differ 46 of 1150, skeleton gap 21 (tla-en, 2026-11-01)
// Same calls, control flow, frame and spill slots. What is left is instruction
// order and two reload registers:
// - The three work->objects loops (start, frame 157, end): the listing places
//   the movs/lsls/adds/add that builds the pointer after the block's stores,
//   here it starts first (priority 4 in sched2). Writing the pointer as a user
//   variable changes nothing. The same late `add r2, fp` shows at the top of
//   the mote loop, around the `str r0, [sp, #8]` of the fade copy.
// - The texture block: the listing loads gData_02014000 into r0, which forces
//   `adds r6, r0, #0` early and lets `mov r1, fp` reuse r1; here it lands in
//   r2. Reordering the field stores only made it worse.
// - frame <= 59 and the else branch: `movs r3, #58` falls one place early.
// What the shape rests on, each measured: position and velocity are 8-byte
// structs held in registers, so each spills as a pair and every field reloads;
// `j * 7` stays an expression so its hoisted copy takes the [sp, #8] slot; the
// table stays non-const and indexed through a variable, which keeps its load
// in the loop and the 6 in a register; cell starts as the offset and adds
// sprites second; the shake's offsets are computed before the BG1 store.
// `none` is a guess at the zero r5 holds across the first calls.

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"

struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const void *pixels;
};

struct RequestPair {
  s32 x, y;
};

struct RenderModel {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct ModelTexture *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // BattleEffect_TintBattler
void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void Func_0815585c(struct BattleEffectArgument *effect, s32 packed, s32 *screen,
                   s32 *otherPoint);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct RenderModel *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct RenderModel *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void EffectPosition_ProjectBattler(s32 id,
                                                 struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 count, s32 resource, s32 mode);   // SpawnObjects
void BattleEffect_SelectLivingTargets(struct BattleEffectArgument *effect); // SelectLivingTargets
void Func_080150e4(s32 angle); // SceneTransform_ApplyRoll
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void Render_ApplyProjectedPlacementFar(void *object, s32 *request, s32 *source,
                                       s32 flags);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void Func_08158ce0(s32 randomMask, u32 shakeRange); // Camera_ApplyShake
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                      s32 drag);
void **GetBattleObjectSlotFar(s32 objectId);
void *Heap_AllocEwram(s32 size);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);
void Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void Render_BuildQuadStrip(void *mesh, s32 columns, s32 width, s32 height);
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

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1])

#include "bg_scroll.h"
#include "input.h"
static __inline__ s32 Through2_(s32 (*routine)(), s32 a0, s32 a1) { return routine(a0, a1); }

void Func_08179e6c(void);
void Animation_ApplyChildArgumentFar(void *object, s32 animation);
void ResourceObject_ReleaseFar(void *object);

extern char ResourceId_Data134;
extern char ResourceId_PetalSheet;
extern char ResourceId_CrescentRowSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_Data0C1;
extern u32 gCameraSceneParameters[];
extern u8 gData_02014000[];
extern u16 gData_08197410[];
extern const struct RequestPair Data_08196e0c;
extern const u16 Data_08197a38[][2]; // the charging Zagan's parts
extern const u16 Data_08197a64[][2]; // the galloping Zagan's
extern const u8 Data_08197a94[];     // each ring's start
extern const u8 Data_08197a98[];     // growth
extern const s8 Data_08197a9c[];     // height
extern const u8 Data_08198cac[];
extern const u8 Data_08198ec4[];

#define ZAGAN_PARTS 12

// Abilities: Zagan.
//
// The screen slides over to a white field and petals blow across it; Zagan
// (twelve objects) gallops in from the right as the backdrop darkens, rears,
// and at frame 157 swings its axe across the targets, striking four times
// while the screen shakes and rings of dust spread. Holding A or B draws
// only every other frame.
void BattleEffect_RunZagan(struct BattleEffectArgument *effect) {
  u8 *battle;
  void *canvas;
  struct BattleEffectWork *work;
  u8 *sprites;
  s32 *transition;
  s32 savedScroll;
  DrawRectangle *blit;
  struct RequestPair position;
  struct RequestPair velocity;
  s32 skip;
  DrawRectangle blitters[2];
  struct RequestPair request;
  s32 placement[4];
  struct ModelTexture texture;
  struct RenderModel *model;
  void *vertices;
  struct EffectStep *p;
  s32 none;
  s32 line;
  s32 motes;
  s32 scroll;
  s32 frame;
  s32 i;
  s32 j;

  none = 0;
  battle = (u8 *)Ram_HeapSlots->battleWork;
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  transition = (s32 *)Ram_HeapSlots->transitionWork;
  savedScroll = gBgScroll[1].x;
  Func_0813ba50();
  BattleEffect_StartCanvasLayer(0);
  Func_08179e6c();
  work->transferMode = none;
  /* FAKEMATCH(wrapper): the call through an inline forwarder, whose parameter leaves the constant set in argument order; as a direct call, 2 of 1150 instructions differ in tla-en. */
  Through2_((s32 (*)())Task_Add, (s32)(BattleEffect_ProcessPendingGraphicsTransfer), (s32)(0xc80));

  // The screen slides over to the field.
  for (scroll = -80, frame = 0; frame != 28; frame++) {
    if (frame == 27) {
      gCameraSceneParameters[4] = 240;
      BattleEffect_SelectLivingTargets(work->effect);
      work->scrollStepX = -8;
      transition[4] = 1;
      Iwram_FillWords((void *)(BG_PLTT + 96), 256, 0x7fff7fff);
      Resource_LoadAndDecompress((s32)&ResourceId_Data134, work->sheet, 0, 0);
      // Ten ever-fainter copies of one mote.
      motes = 3;
      for (j = 0, line = 0; j != 10; line += 16, j++) {
        u8 *cell;

        i = 0;
        cell = (u8 *)(line * 2);
        cell += (s32)sprites;

        for (; i != 32; i++) {
          s32 offset = gData_08197410[motes] + i + 0x700;
          s32 value = ((u8 *)work)[offset];

          if (value != 0) {
            value -= j * 7;
            if (value <= 0) {
              value = 1;
            }
          }
          *cell++ = value;
        }
      }
      Resource_LoadAndDecompress((s32)&ResourceId_PetalSheet, work->sheet, 1,
                                 1);
      for (i = 0; i != 32; i++) {
        work->sheet[0x5dc0 + i] = i + 16;
        work->sheet[0x5de0 + i] = i + 16;
        work->sheet[0x5e00 + i] = i + 16;
      }
    }
    scroll += 4;
    REG_BG2X = scroll << 8;
    work->transferPending = 1;
    wait(1);
  }
  blit = blitters;
  BattleEffect_FetchRectangleBlitters(0, (void **)blit);
  work->transferMode = 2;
  work->transferValue = 50;
  skip = 0;
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
  BattleEffect_SelectLivingTargets(work->effect);
  wait(1);
  BattleEffect_SpawnObjects(ZAGAN_PARTS, 658, 1);
  REG_BG2X = skip;
  REG_BG2PA = 0x80;

  for (frame = 0; frame != 200; frame++) {
    if (gInput.repeat & 3) {
      skip = 1;
    }
    if (frame == 0) {
      Sound_PlayFar(212);
    }
    if (frame == 32) {
      Sound_PlayFar(191);
    }
    if ((u32)(frame - 78) <= 31) {
      REG_BLDALPHA = (16 - (frame - 78) / 2) | 0x1000;
    }
    if (frame == 140) {
      REG_BLDALPHA = 0x1010;
    }
    if (frame == 157) {
      Func_081180e8(145);
      goto strike;
    }
    if (frame == 161 || frame == 165 || frame == 169) {
    strike:
      for (i = 0; i != effect->count; i++) {
        Func_08118088(effect->actors[i], 4);
        Func_0814cd48(effect->actors[i], 7, -1, i, 4);
      }
    }
    if (frame == 0) {
      transition[4] = 1;
      position.x = 240 << 16;
      position.y = 0x400000;
      velocity.x = -0x140000;
      velocity.y = 0;
      for (i = 0; i != ZAGAN_PARTS; i++) {
        Animation_ApplyChildArgumentFar(work->objects[i], i + 12);
      }
      REG_BLDALPHA = 0x1010;
      Iwram_FillWords((void *)(BG_PLTT + 96), 256, 0x7fff7fff);
      work->scrollStepX = -8;
      gCameraSceneParameters[3] = -968;
      gCameraSceneParameters[4] = 120;
      gBgScroll[1].x = savedScroll + 1128;
      Resource_LoadAndDecompress((s32)&ResourceId_PetalSheet, work->sheet, 1,
                                 1);
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                      128);
      work->transferMode = 2;
      work->transferValue = 50;
    }
    gCameraSceneParameters[3] -= work->scrollStepX;
    gBgScroll[1].x += work->scrollStepX;

    // The backdrop darkens toward the battle's palette.
    if (!(frame & 1) && (u32)(frame - 32) <= 111) {
      u16 *from = Resource_GetTableEntry(*(u16 *)(battle + 0x670));
      u16 *to = (u16 *)(BG_PLTT + 96);

      for (i = 0; i != 128; i++, from++, to++) {
        s32 b = (*to >> 10) & 31;
        s32 g = (*to >> 5) & 31;
        s32 r = *to & 31;
        s32 blue = ((*from >> 10) & 31) - 2;
        s32 green = ((*from >> 5) & 31) - 10;
        s32 red = (*from & 31) - 6;

        if (blue < 0) {
          blue = 0;
        }
        if (green < 0) {
          green = 0;
        }
        if (red < 0) {
          red = 0;
        }
        if (b > blue) {
          b--;
        }
        if (g > green) {
          g--;
        }
        if (r > red) {
          r--;
        }
        *to = (b << 10) | (g << 5) | r;
      }
    }
    if ((u32)(frame - 64) <= 35) {
      velocity.x += 0x8000;
      velocity.y += -0x1000;
    }
    if (frame > 177) {
      velocity.x += -0x20000;
      velocity.y += -0x10000;
    } else if (frame > 139) {
      velocity.x += -40000;
      velocity.y += 0x2000;
    } else if (frame > 99) {
      velocity.x += -0x2000;
      velocity.y += 0x800;
    }
    if (frame == 156) {
      Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
    }
    if (frame == 157) {
      Resource_LoadAndDecompress((s32)&ResourceId_CrescentRowSheet, work->sheet,
                                 1, 0);
      work->shakeFrames = 16;
      for (i = 0; i != ZAGAN_PARTS - 1; i++) {
        Animation_ApplyChildArgumentFar(work->objects[i], i + 24);
      }
      work->transferMode = 2;
      work->transferValue = 50;
    }
    if ((u32)(frame - 156) <= 7) {
      blitters[0](canvas, work->sheet, 12, 0, 58, 120);
    }
    for (i = 0; i != 8; i++) {
      if (frame == i * 6 + 114) {
        work->scrollStepX++;
      }
    }
    if (frame == 163) {
      gBgScroll[1].x += -20;
    }
    if (frame == 167) {
      gBgScroll[1].x += -10;
    }
    if (frame == 171) {
      gBgScroll[1].x += -10;
    }

    // Petals on the wind.
    if (frame == 0) {
      s32 row;

      for (i = 0, row = 3, p = work->particles; i != 20; i++, p++, row += 6) {
        p->x = (Random16() & 127) - 160;
        p->y = row;
        p->variant = Random16() & 1;
        p->velocityX = (Random16() & 3) + p->variant * 8 + 8;
      }
    }
    if ((u32)frame <= 119) {
      s32 odd;

      for (i = 0, odd = frame & 1, p = work->particles; i != 20; i++, p++) {
        if (!odd || !skip) {
          blitters[0](canvas, work->sheet + 0x5dc0, p->x, p->y, 32,
                      p->variant + 1);
        }
        p->x += p->velocityX;
        if (p->x > 128) {
          p->x -= 160;
        }
      }
    }
    if ((u32)frame > 195) {
      goto shake;
    }

    // Zagan.
    request = Data_08196e0c;
    placement[3] = 0;
    placement[1] = 0;
    if (!(frame & 1) || !skip) {
      if (frame > 156) {
        for (i = 0; i != ZAGAN_PARTS - 1; i++) {
          placement[0] = (Data_08197a38[i][0] << 16) + position.x - 0x200000;
          placement[2] = (Data_08197a38[i][1] << 16) + position.y + 0x100000;
          Render_ApplyProjectedPlacementFar(work->objects[i], placement,
                                            (s32 *)&request, 0);
        }
      } else {
        for (i = 0; i != ZAGAN_PARTS; i++) {
          placement[0] = (Data_08197a64[i][0] << 16) + position.x;
          placement[2] = (Data_08197a64[i][1] << 16) + position.y - 0x100000;
          Render_ApplyProjectedPlacementFar(work->objects[i], placement,
                                            (s32 *)&request, 0);
        }
      }
    }
    if (frame > 177) {
      position.x += velocity.x;
      position.y += velocity.y;
      velocity.x = velocity.x * 63 / 64;
      velocity.y = velocity.y * 63 / 64;
    } else if (frame > 149) {
      if (position.y > 0x37ffff) {
        velocity.x = 0;
        velocity.y = 0;
      }
      position.x += velocity.x;
      position.y += velocity.y;
    } else if (frame <= 59) {
      position.x += velocity.x;
      position.y += velocity.y;
      velocity.x = velocity.x * 58 / 64;
      velocity.y = velocity.y * 62 / 64;
    } else {
      position.x += velocity.x;
      position.y += velocity.y;
      velocity.x = velocity.x * 58 / 64;
      velocity.y = velocity.y * 61 / 64;
    }
    if ((!(frame & 1) || !skip) && frame <= 119) {
      blit[1](canvas, work->sheet + ((frame / 3) & 3) * 6000,
                  (position.x / 2 >> 16) - 12, (position.y >> 16) - 58, 60, 100);
    }
    if (frame == 157) {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, gData_02014000, 0,
                                 0);
    }
    if ((frame & 1) && skip) {
      goto shake;
    }

    // Rings of dust from the blow.
    if (frame <= 156) {
      goto next;
    }
    {
      s32 k;

      vertices = Heap_AllocEwram(0x400);
      model = Render_AllocateModelList(1);
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = gData_02014000;
      model->texture = &texture;
      model->kind = 7;
      model->mesh = Data_08198ec4;
      // A once-only block: it keeps the store after the mesh's.
      do {
        model->vertices = vertices;
      } while (0);
      model->scrollU = 0;
      model->scrollV = 0;
      for (k = 0; k != 4; k++) {
        s32 start = Data_08197a94[k] + 157;
        s32 scale;
        s32 shade;

        if (frame <= start) {
          continue;
        }
        scale = (((frame - start) * Data_08197a98[k] * 125) << 4) + 0x8000;
        shade = (start - frame) * 8 + 56;
        if (shade > 0) {
          shade = 0;
        }
        if (shade <= -64) {
          continue;
        }
        model->shade = shade;
        Render_ResetTransformState();
        Func_08015160(-0x100000, Data_08197a9c[k] << 16, 0);
        Func_080151e4(scale, scale * 4 / 3, scale);
        SceneTransform_ApplyPitch(3000);
        model->scrollU = ((frame + k * 5) & 7) * 16;
        Render_TransformVertices(Data_08198cac, vertices, 32);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
  shake:
    if (frame > 156) {
      if (work->shakeFrames > 0) {
        s32 offsetX = (Random16() & 3) - 8;
        s32 randomY = Random16() & 15;
        s32 offsetY = randomY - 8;

        gBgScroll[1].y = randomY + 24;
        gCameraSceneParameters[3] = 120 - offsetX;
        gCameraSceneParameters[4] = 120 - offsetY;
        work->shakeFrames--;
      } else {
        gBgScroll[1].y = work->savedBg1Y;
        gCameraSceneParameters[3] = 120;
        gCameraSceneParameters[4] = 120;
      }
    }
  next:
    BattleEffect_CountDownBattlerTints();
    if (!(frame & 1) || !skip) {
      work->transferPending = 1;
      wait(1);
    }
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  transition[4] = 0;
  for (i = 0; i != ZAGAN_PARTS; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  BattleEffect_EndCanvasLayer();
}
