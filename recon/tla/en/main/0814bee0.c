// Draft, differ 2 of 1150 (tla-en, 2026-11-08); no device left. Every edition
// differs by the same 2.
// Devices pass (2026-11-09), none below 2. sched2 (verbose) after j's zero:
// the 6's copy to ip and line's zero tie on priority (1), class and
// dependents (0 each), so LUID decides. Tried: line = j * 16 as a giv behind
// a once-only block, line << 1 or line + line, or an asm operand hiding it
// from CSE (213 to 702: line stops being reduced or j * 32 is); the table's
// 6 set in the for init (256).
// Left: one swap in the mote loop's preheader. The listing sets line (r4) to 0
// after `mov ip, r1` (the hoisted 6 of gData_08197410[motes]); here before it.
// sched2 breaks that tie by RTL order, so in the original r4's 0 is emitted
// after loop.c's hoisted invariants, where strength reduction puts a reduced
// giv's start (the j * 7 copy's `movs r0, #0` sits there too): the for's own
// init cannot get there. A reduced j * 16 would, but every form tried either
// lets CSE fold line * 2 into j << 5 or reduces j * 32 instead (6 to 703):
// line = j * 16 at the top of the body, in the for's step, as u16, s16 or u8;
// the store as sprites[line * 2 + i] in five spellings or through a u16
// pointer; cell = sprites + line * 2 in three spellings. j * size (a local
// 16) reduces the right giv and orders r4 right, but keeps 16 in r8 (105).
// line + line, line scoped in the frame-27 block, u32, the inits swapped: 2
// or worse. What would settle it: a source form whose 16-step register is a
// reduced giv while its doubling stays outside every giv.
// 8 November, third round (12 to 2): the rings' texture pointer is a local,
// ringPixels, holding gData_02014000 for the whole function; left without a
// register it is reloaded at each use, which loads it into r0 after the
// model's copy and moves the vertices' reload into r1 (12 to 4); the
// once-only block around the vertices store then goes (4 to 2). Task_Add is
// declared from task.h, returning s32 as it does: its call sets r0, which is
// all the wrapper did, so the wrapper and its tag are gone (a void Task_Add
// measures 4). Earlier rounds: the three objects loops, `movs r3, #58` and
// the texture's barrier were settled by 6 November.
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
#include "task.h"

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
  u8 *ringPixels;
  s32 line;
  s32 motes;
  s32 scroll;
  s32 frame;
  s32 i;
  s32 j;

  none = 0;
  ringPixels = gData_02014000; // the rings' texture, for the whole function
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
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

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
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, ringPixels, 0, 0);
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
      texture.pixels = ringPixels;
      model->texture = &texture;
      model->kind = 7;
      model->mesh = Data_08198ec4;
      model->vertices = vertices;
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
