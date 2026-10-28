// Draft, differ 743 of 1075 (draft 1058 instructions, no device): effect
// kind 125, written from the listing on The Broken Seal's canvas skeleton
// (twins: BattleEffect_RunTitanBlade 0.44 by shape, BattleEffect_RunHealingMotes 0.38
// by call order); its backdrop swap is the one BattleEffect_RunMegiddo (kind 124)
// also makes. Every call, cue and table read is in the native's order.
// The native stack frame is 136 bytes: to at sp+124, from sp+112, the spawn list
// sp+84, the blitter pair sp+76, the texture sp+68, effect sp+60, work
// sp+56, canvas sp+52, graphics sp+48, caster sp+44, transition sp+40, the
// saved scroll sp+36, &from sp+32, &to sp+28, &texture sp+24, the flash
// loop's 6k sp+20, the claw loop's frame * 10 - 240 - 160k sp+16, its
// start sp+12 and 32k sp+8. Here the stack frame is 112 bytes: the claw and
// flash loops keep those values in registers, so every stack offset and
// most registers differ.
// Not yet tried: the claw loop's three spilled values as the native's
// locals (x from frame * 10 rather than age * 10).
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "mem.h"
#include "motobj.h"
#include "bg_scroll.h"
#include "battle_display.h"
#include "battle_work.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The integer (pixel) half of a 16.16 value, read as the halfword it is.
#define HI(v) (((s16 *)&(v))[1])

#define KIND125_FRAMES 114

// A texture for the ARM model renderer: its size as powers of two and the
// address of its pixels.
struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const void *pixels;
};

// A model for the ARM renderer (Render_DrawModelList). A textured model (kind 7)
// takes a shade; a flat one (kind 3) the address of its colour.
struct RenderModel {
  s32 kind;
  s32 unk04;
  const void *mesh;
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
extern struct EffectStep Data_02016000[];
extern u16 gData_08197410[];
extern const u16 gData_08199364[];
extern const u16 Data_0819919c[];
extern const u8 Data_081991e0[];
extern const u8 Data_081991a4[];
extern const s16 Data_08199f2a[];
extern const s16 Data_08199f2e[];

void BattleEffect_StartCanvasLayer(s32 layer);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_LoadBlitter(s32 slot, s32 value);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void Graphics_LoadGreyRamp(void);
void BattleEffect_BuildRepeatingTilemap(void);
void BattleEffect_SetApproachMotion(s32 id, s32 targetId, s32 frames, s32 rate);
void Resource_LoadAndDecompress(s32 id, void *target, s32 unpack, s32 wait);
s32 Task_Add(void (*callback)(void), s32 order);
s32 Task_Remove(void (*callback)(void));
struct BattleObj *GetBattleObjectSlotFar(s32 objectId);
void EffectPosition_ProjectScaledBattler(s32 id, struct EffectPosition *out);
void BattleActor_SpawnObjectsForListFar(s16 *targets, s32 mode);
void BattleActor_CommitPlacementFar(void);
void BattleBackground_LoadFar(s32 mode, s32 background, s32 level);
void Object_SetMode(struct MotionObject *object, s32 mode);
void ObjectDispatch_SetAnimationSpeedFar(struct MotionObject *object,
                                            s32 value);
void Runtime_ReleaseHeapBlock(s32 slot);
void WaitFrames(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
struct RenderModel *Render_AllocateModelList(s32 count);
void Render_ResetTransformState(void);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_080151e4(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_TransformVertices(const u8 *vertices, void *projected, s32 count);
void Render_DrawModelList(struct RenderModel *model);
void Func_0814cd48(s32 actor, s32 palette, s32 mode, s32 timer, s32 frames);
void Func_08164a4c(s32 blue, s32 green, s32 red);
void Camera_ApplyVerticalShake(s32 range);
void ObjectGroup_CountDownMemberTimers(void);
void Sound_PlayFar(s32 cue);

// Effect kind 125, 114 frames. The screen turns to a scrolling red haze
// with only the caster and the target on it (frame 2); two textured flashes
// rise from the caster, two claws strike the target from above and below
// (frames 24-89, sparks at each hit); at 84 the caster charges across, at 86
// a great slash crosses the screen and at 90 the target is struck and 128
// flat shards scatter, tumbling; from frame 100 everything fades to white,
// and at the end the battle backdrop returns.
void BattleEffect_RunOdyssey(struct BattleEffectArgument *effect) {
  struct EffectPosition from;
  struct EffectPosition to;
  s16 list[3];
  DrawRectangle draw[2];
  struct ModelTexture texture;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *graphics;
  struct MotionObject *caster;
  struct BattlePresentationTransition *transition;
  u16 savedScrollX;
  struct RenderModel *model;
  void *projected;
  struct EffectStep *step;
  u8 color;
  s32 frame;
  s32 i;
  s32 k;

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
        u32 red = *palette & 31;
        u32 blue = (*palette >> 10) & 31;

        if (red > 31)
          red = 31;
        if (blue > 31)
          blue = 31;
        *palette++ = red << 10 | (red >> 1) << 5 | blue;
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
      Object_SetMode(caster, 2);
      ObjectDispatch_SetAnimationSpeedFar(caster, 48);
      BattleEffect_SetApproachMotion(effect->actor, effect->actors[0], 7, 80);
    }

    // Frame 90: the hit, and 128 shards scatter away from the caster.
    if (frame == 90) {
      Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      Sound_PlayFar(144);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
      work->shakeFrames = 8;
      step = Data_02016000;
      for (i = 0; i != 128; i++) {
        s32 speed = Random16() & 0x1ff;
        s32 angle = Random16() & 0x7fff;

        speed += 32;
        if (effect->side == 1)
          angle += 0x8000;
        step->x = (to.x / 2) << 16;
        step->y = to.y << 16;
        step->velocityX = (speed * Trig_Sin(angle)) >> 6;
        step->velocityY = -(speed * Trig_Cos(angle)) >> 5;
        step->z = Random16();
        step->velocityZ = (Random16() & 0x3ff) + 0x400;
        step->variant = (Random16() & 0x3fff) + 0x2000;
        step++;
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
      Runtime_ReleaseHeapBlock(188);
    }

    // Until frame 89: the sparks of the claws' hits, shrinking.
    if (frame <= 89) {
      step = work->particles;
      for (i = 0; i != 64; i++) {
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
    for (k = 0; k != 2; k++) {
      s32 start = k * 16 + 24;

      if (frame == k * 16 + 28)
        Sound_PlayFar(212);
      if (frame == k * 16 + 32) {
        step = &work->particles[k * 32];
        for (i = 0; i != 16; i++) {
          s32 speed = Random16() & 255;
          s32 angle = Random16() & 0x3fff;

          speed += 128;
          if (effect->side == 0) {
            if (k != 0)
              angle += 0x4000;
          } else if (k == 0) {
            angle -= 0x4000;
          } else {
            angle += 0x8000;
          }
          step->x = (to.x / 2) << 16;
          step->y = to.y << 16;
          step->velocityX = (speed * Trig_Sin(angle)) >> 6;
          step->velocityY = -(speed * Trig_Cos(angle)) >> 5;
          step->variant = (Random16() & 7) + 17;
          step++;
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
          if (k == 0)
            y = age * 16 - 120;
          else
            y = 168 - age * 16;
        } else {
          x = to.x / 2;
          y = k == 0 ? 8 : 40;
        }
        if (k == 1) {
          if (effect->side == 0)
            BattleEffect_LoadBlitter(188, 1 << 4 | 11);
          else
            BattleEffect_LoadBlitter(188, 1 << 4 | 15);
          draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
        }
        draw[k](canvas, work->sheet, x - 20, y, 40, 80);
        if (k == 1)
          Runtime_ReleaseHeapBlock(188);
      }
    }

    projected = Runtime_BumpAllocateAlternatePool(32);
    model = Render_AllocateModelList(1);
    texture.logWidth = 7;
    texture.logHeight = 7;
    model->mesh = gData_08199364;
    texture.pixels = gMapCellBuffer;
    model->kind = 7;
    model->texture = &texture;
    model->vertices = projected;
    for (k = 0; k != 2; k++) {
      s32 start;

      // Two flashes rise from the caster (frames 2 and 8).
      start = k * 6 + 2;
      if (frame == start)
        Sound_PlayFar(212);
      if (frame >= start && frame < k * 6 + 18) {
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
          SceneTransform_ApplyPitch(Data_08199f2a[k]);
          SceneTransform_ApplyYaw(Data_08199f2e[k]);
          Render_TransformVertices(Data_081991e0, projected, 4);
          Render_DrawModelList(model);
        }
      }

      // Frames 64-82: the caster gathers itself, flashing.
      if (frame == k * 8 + 68)
        Sound_PlayFar(212);
      if (frame == k * 8 + 72)
        Func_0814cd48(effect->actor, 7, -1, 0, 0);
      else if (frame == k * 8 + 76)
        Func_0814cd48(effect->actor, 0, -1, 0, 0);
      if (frame >= k * 8 + 64 && frame < k * 8 + 75) {
        s32 scale = 0x30000 - ((frame - (k * 8 + 64)) << 14);

        model->look.shade = 0;
        Render_ResetTransformState();
        Func_08015160((from.x / 2 - 64) << 16, 0, 0);
        Func_080151e4(scale, scale * 2, scale);
        SceneTransform_ApplyPitch(Data_08199f2a[k]);
        SceneTransform_ApplyYaw(Data_08199f2e[k]);
        Render_TransformVertices(Data_081991e0, projected, 4);
        Render_DrawModelList(model);
      }

      // A flash at the target with each claw's hit.
      start = k * 16 + 32;
      if (frame >= start && frame < k * 16 + 48) {
        s32 age = frame - start;
        s32 shade = 64 - age * 8;

        if (shade > 0)
          shade = 0;
        if (shade > -64) {
          s32 scale;

          model->look.shade = shade;
          Render_ResetTransformState();
          scale = (age << 14) + 0x4000;
          Func_08015160((to.x / 2 - 64) << 16, 0, 0);
          Func_080151e4(scale, scale * 2, scale);
          SceneTransform_ApplyPitch(Data_08199f2a[k]);
          SceneTransform_ApplyYaw(Data_08199f2e[k]);
          Render_TransformVertices(Data_081991e0, projected, 4);
          Render_DrawModelList(model);
        }
      }
    }

    // From frame 90: the shards, flat triangles in eight colours; from 100
    // the screen fades to white.
    if (frame > 89) {
      model->mesh = Data_0819919c;
      model->kind = 3;
      if (frame > 99) {
        work->transferMode = 1;
        work->transferValue = 0x02020202;
        Func_08164a4c(2, 2, 2);
      }
      step = Data_02016000;
      for (i = 0; i != 128; i++) {
        color = (i & 7) + 32;
        model->look.color = &color;
        Render_ResetTransformState();
        Func_08015160(step->x - (64 << 16), step->y - (64 << 16), 0);
        Func_080151e4(step->variant * 3, step->variant * 6, step->variant);
        SceneTransform_ApplyPitch(step->z);
        SceneTransform_ApplyYaw(step->z);
        Render_TransformVertices(Data_081991a4, projected, 3);
        Render_DrawModelList(model);
        BattleEffectKernels_IntegrateVector3(step, 62, 0);
        step++;
      }
    }
    MemFree(model);
    MemFree(projected);

    Camera_ApplyVerticalShake(8);
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  // The battle's own scene again.
  BattleActor_CommitPlacementFar();
  gBgScroll[1].x = savedScrollX;
  transition->active = 0;
  BattleBackground_LoadFar(1, *(u16 *)Ram_HeapSlots->battleWork->unk670, 24);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(104);
  BattleEffect_EndCanvasLayer();
}
