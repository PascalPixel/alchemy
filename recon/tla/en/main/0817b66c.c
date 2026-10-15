/* Draft (tla-en): listing 326 instructions, draft 326, common 318, differ 16.
 * The shape matches; what is left is the arm9tdmi scheduling leftover.
 * In this draft: BG2PA's 0x100 pinned to r3 behind a one-pass block (as
 * s2614.c), so it loads from the pool before the register address as the
 * native does, and the frame-80 block's "drift" temporary (alchemy
 * permute's), which puts velocityX's store after velocityY's.
 * Left:
 * - the one-pass block's barrier depends on every earlier use of r10, so
 *   sched2 now prefers "add r1, r10" to the resource id's pool load before
 *   Resource_LoadAndDecompress (a one-pass block around or before that
 *   call only moves the swap). f18284c.c's empty asm instead keeps the
 *   start but counts four estimated bytes, and the halfword pool (range
 *   64) is then placed before the loop with a jump around it.
 * - frame's 0 is set between "mov r3, sp" and "adds r3, #12"; the native
 *   stores &sprite at sp+8 first. Through a "source" local set before the
 *   loop the order is right but &sprite takes r2 and the 0 r3.
 * - the frame-0 block's setup (y, i, the zero and step) is in another
 *   order; the sprite mask's pool load comes after the stack load of
 *   sprite.bits (the native loads the pool word first, as if the load
 *   after model->unk14's store waited a cycle more); the step mask's
 *   pool load comes one row late.
 * The sprite source word is two byte fields written with masks: as u32
 *   bitfields the compiler stores bytes (differ 57).
 * Resource 313 (BattleFx_CrossedDaggersSheet) needs its name in
 *   system/resource/directory.s when this is adopted.
 * alchemy permute (22 minutes over three runs): differ 16 at best.
 */
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "effect_step.h"
#include "battle_effect_work.h"

struct FxWork {
    struct EffectStep steps[32];
    u8 unk0380[0x7780 - 0x380];
    s32 transferMode;
    s32 transferValue;
    u8 unk7788[0x20];
    s32 shakeFrames;
    u8 unk77ac[0x78e8 - 0x77ac];
    s32 transferPending;
};

struct SpriteSource {
    u32 bits;
    void *pixels;
};

struct RenderModel {
    s32 kind;
    s32 unk04;
    const void *mesh;
    void *vertices;
    struct SpriteSource *source;
    s32 unk14;
};

extern char ResourceId_CrossedDaggersSheet;
extern s32 gCameraSceneParameters[];
extern const u8 gData_08199450[];
extern const u8 gData_08199474[];

void BattleFx_BeginCanvasLayer(s32 bg_control);
void Resource_LoadAndDecompress(s32 resourceId, void *target, s32 unpack, s32 wait);
void Scheduler_AddOrUpdateCallback(s32 callback, s32 priority);
s32 Scheduler_RemoveCallback(s32 callback);
void *GetBattleObjectSlotFar(s32 id);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
u32 Random16(void);
void Render_ResetTransformState(void);
void Audio_PlayCue(s32 cue);
void Func_081180e8(s32 cue);
void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);
void BattleFx_StartActorMotion(s32 id, s32 force, s32 acceleration, s32 speedLimit, s32 velocityY, s32 percent);
void *Runtime_BumpAllocateAlternatePool(s32 size);
struct RenderModel *Func_081969f8(s32 kind);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void Func_080150e4(s32 angle);
void Func_08196958(const void *mesh, void *vertices, s32 count);
void Func_08196a7c(struct RenderModel *model);
void BattleFxKernels_IntegrateVector3(struct EffectStep *step, s32 damping, s32 gravity);
void MemFree(void *buffer);
void Func_08158ce0(s32 x, s32 y);
void Func_081434f8(void);
void WaitFrames(s32 frames);
void Func_08143000(void);
void Func_08143bb8(void);

void Func_0817b66c(struct BattleEffectArgument *effect)
{
    struct FxWork *work = (struct FxWork *)Ram_HeapSlots->battleFxWork[0];
    struct SpriteSource sprite;
    struct EffectPosition pos;
    struct EffectStep *step;
    struct RenderModel *model;
    void *vertices;
    s32 frame;
    s32 i;
    s32 cue;

    BattleFx_BeginCanvasLayer(0);
    Resource_LoadAndDecompress((s32)&ResourceId_CrossedDaggersSheet, (u8 *)work + 0x700, 1, 1);
    work->transferMode = 1;
    work->transferValue = 0;
    REG_BLDCNT = 0;
    Scheduler_AddOrUpdateCallback((s32)Func_08143000, 0xc80);
    GetBattleObjectSlotFar(effect->actors[0]);
    EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], &pos);
    {
        register u16 scale __asm__("r3") = 0x100;

        do { } while (0);
        REG_BG2PA = scale;
    }
    REG_BG2X = (64 - pos.x) << 8;
    for (frame = 0; frame != 96; frame++) {
        if (frame == 0) {
            s32 y = 0x800000;

            for (i = 0, step = work->steps; i != 32; i++, step++) {
                step->x = ((Random16() & 63) + 32) << 16;
                step->y = y;
                step->z = Random16();
                step->velocityX = 0;
                step->velocityY = -((Random16() & 31) + 32) << 10;
                step->velocityZ = (Random16() & 0x3ff) + 0x400;
                if (i & 1)
                    step->velocityZ = -step->velocityZ;
                step->variant = 0;
                y += 0x80000;
            }
        }
        Render_ResetTransformState();
        gCameraSceneParameters[4] = 104;
        for (i = 0, cue = 20; i != 8; i++, cue += 6) {
            if (frame == cue) {
                work->shakeFrames = 4;
                Audio_PlayCue(133);
                Func_0814cd48(effect->actors[0], 7, 5, 0, 6);
                BattleFx_StartActorMotion(effect->actors[0], 1, 0, 0, 0x30000, 100);
            }
        }
        if (frame == 90) {
            work->shakeFrames = 4;
            Func_081180e8(134);
            Func_0814cd48(effect->actors[0], 7, 5, 0, 10);
            BattleFx_StartActorMotion(effect->actors[0], 1, 0, 0, 0x30000, 100);
        }
        vertices = Runtime_BumpAllocateAlternatePool(32);
        model = Func_081969f8(1);
        model->unk14 = 0;
        sprite.bits = (sprite.bits & ~0xff) | 5;
        sprite.bits = (sprite.bits & ~0xff00) | 0x500;
        model->source = &sprite;
        sprite.pixels = (u8 *)work + 0x700;
        model->kind = 9;
        model->mesh = gData_08199450;
        model->vertices = vertices;
        for (i = 0, step = work->steps; i != 8; i++, step++) {
            if (frame == 80) {
                s32 speed;
                s32 drift;

                step->y = 0xffd00000;
                speed = (i << 10) + (Random16() & 0x3ff) - 0x1200;
                step->x = (speed << 10) + 0x400000;
                step->z = speed;
                drift = -speed << 5;
                step->velocityY = 0xc0000;
                step->velocityX = drift;
                step->velocityZ = 0;
            }
            Render_ResetTransformState();
            Func_08015160(step->x - 0x400000, step->y - 0x400000, 0);
            Func_0801521c(0x8000);
            Func_080150e4(step->z);
            Func_08196958(gData_08199474, vertices, 4);
            Func_08196a7c(model);
            if (frame > 79)
                BattleFxKernels_IntegrateVector3(step, 64, 0);
            else
                BattleFxKernels_IntegrateVector3(step, 64, -0x1000);
        }
        MemFree(model);
        MemFree(vertices);
        gCameraSceneParameters[3] = 120;
        gCameraSceneParameters[4] = 120;
        Func_08158ce0(4, 4);
        Func_081434f8();
        work->transferPending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((s32)Func_08143000);
    Func_08143bb8();
}
