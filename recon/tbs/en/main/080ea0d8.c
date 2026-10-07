/* 080ea0d8: differ 131 of 2455 (listing 2455, draft 2454, common 2389).
 * Frame: 184 as native, and every spill slot (sp+104 canvas down to sp+12,
 * the frame giv) holds what the native's holds, with r4's caller-save word
 * at sp+8.
 * Found in the 2026-10-07 session, in order of effect:
 * - One counter i serves every loop except the ramp's inner j: all 24 of
 *   the native's loop counters are r7. Separate counters each carry the
 *   doubled lifetime of a constant-initialised pseudo (update_equiv_regs
 *   doubles REG_LIVE_LENGTH for the REG_EQUAL 0 start before the
 *   increment clears the equivalence) and lost r7 loop by loop; one
 *   shared counter gets r7 once for the whole function. With the ring
 *   loop's two halves as separate variables (half, half2; one shared half
 *   outranked the counter for r7): 1426 -> 784.
 * - draw is DrawRectangle draw[2] (DImode, one 8-byte slot at sp+88):
 *   draw[0] the blitter, draw[1] the alternate one set each frame of the
 *   second loop. That alone explained the native's 184-byte frame (its
 *   "draw2" at sp+92 is draw[1]) and the copies of draw[0] the petal
 *   blocks call through (subreg reads CSE'd into one pseudo).
 * - IME: the native rematerialises 0x04000208 at every use and stores the
 *   constant 0x208, so ime is a function-level variable set once and
 *   spilled (as CANVAS.C's QUEUE_DISPLAY_CONTROL); the queue pointer stays
 *   per push (native keeps it in r1 across both pushes).
 * - x, y, size, z and amp are block locals of their loops (the native keeps
 *   size in r4 in one loop and r0 in another): function-wide, their
 *   combined lifetimes reordered every loop's registers. 2050 -> 1426 with
 *   the next two.
 * - The i18 ring draws use size = 10 (5 and 20 hoisted, 10 rematerialised)
 *   and multiply Trig_Sin(angle) * r2, which puts the radius first as the
 *   native's mul does; r2 * Trig_Sin(angle) puts the call result first.
 * - amp as a local of the spark loop body: loop.c then hoists 255 and the
 *   zero after the counter's start, as the native schedules them.
 * - Second petal block: d + 38 rather than frame * 2 - 126, so call four
 *   reuses d / 4 + 49 without a dangling d >= 0 branch that kept d alive.
 * - loop.c: ours hoists 0, &fill and 24 out of the 320-frame loop in its
 *   first pass and 22 and 44 in its second; the native keeps 44 in a
 *   register in both petal blocks (r10, r8), so it did not hoist 44.
 * - The frame <= 63 block calls ((DrawRectangle *)gWorkSlot)[47], the
 *   native's literal gWorkSlot+188 (listed as gTransitionWork+0xc), the
 *   same constant as draw[1]'s load: loop.c's second pass then hoists
 *   it before 22, and 44 stays in each petal block as the native's, so
 *   the second petal block spills its cell (sp+28) as the native does.
 * - The shard tables are not const: the native reloads ShardWidths[k]
 *   after Trig_Cos rather than keeping it across the call, and k then
 *   takes r4 under caller-save, not r1. With the previous: 784 -> 566.
 * - The 116..144 block calls through the same literal (566 -> 558).
 * - slots = gWorkSlot is set at the top of the first loop's body: loop.c
 *   hoists it after the scene addresses, in the native's order (536).
 * - The burst and petal cells are byte offsets computed before LoadWork and
 *   added to work->sheet at the call, as the native adds work after the
 *   call (518, 492); the petal draws pass high, high * 2 with high a local
 *   8 (rematerialised), the burst block its own high (441).
 * - The new objects clear part[0].priority through the bitfield: the native
 *   masks the byte with an SImode ~12 hoisted to r8 (441 -> 273).
 * - draw[0] after the first loop reads gWorkSlot[46] as an array element (base
 *   plus 184, as the native), not the folded literal (238).
 * - The particle placement writes place2[0] before place2[1] = 0xff0000:
 *   the constant's shorter life keeps loop.c from hoisting it (196).
 * - The frame <= 63 block picks its sheet with if-else (the then arm becomes
 *   the default, as the native's ble skips the other) and draws at
 *   top - 24 and top - 1 from top = frame - 4: the else join ends CSE's
 *   block, so top stays in r8 and top - 1 is (top - 24) + 23 (154).
 * - The shake y goes through an s32 local (no HImode narrowing of the & 3,
 *   whose constant would come from the pool) (137); the actor loop is a
 *   plain for, its zero start before the copied exit test (131).
 * - The reload registers of the prologue's constants (r1/r2/r4) match only
 *   when the function's set of spill registers matches: a quick global
 *   check after any change.
 * Earlier notes, still true:
 * Ramp: the eight clamped copies are a two-dimensional u8 [8][770] array at
 * work + 0x2710, indexed [i][j]: the constant then folds into the address
 * after the variable part, loop.c creates it in the inner loop's preheader
 * and cannot hoist it, and the whole ramp loop matches the native's.
 * The palette fade masks with a pool-loaded 0x1f and widens each colour as
 * a halfword (lsls 16 / lsrs 26), ours with asrs.
 * The native fill word sits at sp+140, between place and place2, so it is a
 * declared volatile word.
 * Trial: petal cell written in each call, not a block local (kept).
 * Trial: palette colour as s32, not u16 (u16 loads with ldrsh), kept.
 * Trial: spark velocities from their own zero variable, not the ramp
 * limit: the counter takes r7 and the zero r8 as native's (kept).
 * Trial: ramp limit as 64 - i * 7 in the inner loop (kept).
 * Trial: respawn x tests frame <= 279 first, as the native branches.
 * Trial: frame > 85 and frame <= 213 as nested ifs (kept).
 * Trial: one counter i for every loop: it spilled while x and y were
 * function-wide; with them local it is the 784 form above.
 * Trial: the petal ring radius frame * 10 - 1424 computed once in the frame
 * body: it becomes a reduced giv of the frame loop as native's sp+12 (kept).
 * Trial: loop 2 as a label and goto: frame 184 but worse overall; the
 * frame difference was draw[1], not loop.c. */
#include "CANVAS.H"
#include "RESOURCE.H"
#include "MEM.H"
#include "PROJECT.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "DMA.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BTLDISP.H"
#include "HEAP_STATE.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "RESOURCE_IDS.H"
#include "IO_WRITE_QUEUE.H"
#include "MAP_SCROLL.H"
#include "ANIMSPR.H"
void Resource_LoadAndDecompress(s32, void *, s32, s32);
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "FIXED_MATH.H"

void WaitFrames(s32);
u32 Random16(void);
void Audio_PlayCue(s32);
void BattlePres_ConfigureEffectDisplay(void);
void BattleEffect_WipeCanvas(s32, s32);
void BattleBackground_LoadFar(s32, s32, s32);
void BattlePresentation_ConfigurePaletteFadeFar(s32, s32, s32);
void Render_ResetTransformState(void);
void SceneTransform_ApplyRoll(s32);
void SceneTransform_ApplyPitch(s32);
void SceneTransform_ApplyYaw(s32);
void Graphics_PrepareTransferInIwramWork(void *, void *);
void AnimationObjects_SelectAnimationFar(void *, s32);
void Object_ApplyProjectedPlacementFar(void *, void *, void *, s32);
void BattleEffect_SetupBlendedDisplay(void);
void BattleEventRuntime_BeginPhaseFar(s32);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);

extern void *gBattleFxWork[];
extern void *gTransitionWork[];
extern volatile u32 gKeysRepeat;

typedef struct Scale {
    s32 x;
    s32 y;
} Scale;

struct BattleFx9Work {
    u8 sheet[0x2710];
    u8 ramp[8][770];
};

extern const u16 ParticleStreams_CellOffsets[];
extern const Scale BattleFx9_UnitScale;
extern const u8 BattleFx9_BurstHeights[];
extern const u16 BattleFx9_BurstCells[];
extern u16 BattleFx9_ShardCells[];
extern u8 BattleFx9_ShardWidths[];
extern u8 BattleFx9_ShardHeights[];

#define DUST ((struct EffectStep *)Ram_MapCellBuffer)
#define SPARKS (&DUST[128])

#define QUEUE_IO_WRITE(address, value, delay) do { \
        struct IoWriteQueue *q;                                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        q = &gIoWriteQueue;                                                 \
        do { \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (value);                                       \
            *destination++ = (address);                                     \
            *destination = (delay);                                         \
        }                                                                   \
        *ime = saved;                                                       \
    } while (0)

void Unnamed_080ea0d8(struct BattleEffectArgument *efx)
{
    s32 anchor[3];
    s32 scene[3];
    s32 place[4];
    volatile u32 fill;
    s32 place2[4];
    Scale pair;
    Scale scale;
    void *canvas;
    struct BattleEffectWork *work;
    struct BattlePresentationWork *ctrl;
    DrawRectangle draw[2];
    s32 frame;
    u8 *ramp;
    struct BattleCamera *camera;
    s32 depth;
    s32 zoom;
    s32 near;
    s32 far;
    void **cache;
    DrawRectangle *slots;
    s32 cx;
    s32 cy;
    s32 squash;
    s32 sx;
    s32 ring;
    s32 rings;
    s32 shift;
    s32 i;
    s32 j;
    struct EffectStep *p;
    DrawRectangle blit;
    volatile u16 *ime;

    cache = &gBattleFxWork[1];
    ime = (volatile u16 *)0x04000208;
    canvas = cache[0];
    work = cache[HEAP_SLOT_BATTLE_EFFECT - HEAP_SLOT_BATTLE_CANVAS];
    ctrl = cache[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE_CANVAS];
    ramp = cache[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_CANVAS];
    camera = cache[HEAP_SLOT_CAMERA - HEAP_SLOT_BATTLE_CANVAS];
    work->effect = efx;
    BattleFx_BeginCanvasLayer(0x2000);
    *(u16 *)0x04000020 = 0x100;
    BattlePres_ConfigureEffectDisplay();
    *(u16 *)0x05000000 = 0;
    *(u16 *)0x05000002 = 0;
    BattleEffect_LoadWork(46, 7, 7, 3, 3);
    draw[0] = (DrawRectangle)cache[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_CANVAS];
    work->transfer_mode = 0;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    BattleEffect_WipeCanvas(0, 0);
    BattleFx_SelectLivingTargets(work->effect);
    BattleFx_SpawnObjects(16, 0x17e, 1);
    gProjection.center_y = 240;
    WaitFrames(1);
    BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
    ctrl->initial_value = 1;
    gBgScroll[1].x = 0;
    BattleEffect_WipeCanvas(0, 1);
    *(u16 *)0x04000000 = 0x7741;
    *(u16 *)0x04000020 = 0x80;
    *(u16 *)0x04000052 = 0x1010;
    *(u16 *)0x04000050 = 0x3f44;
    depth = 0xffc00000;
    zoom = 0x80000;
    near = 0xffff;
    far = 0;
    Resource_LoadAndDecompress((s32)&ResourceId_PinkBurstSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_OverlayImageB, work->sheet + 0x600, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet + 0x95c, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, ramp, 0, 0);

    for (i = 0; i != 8; i++) {
        u8 *src = ramp;
        for (j = 0; j != 770; j++) {
            s32 v = *src++;
            if (v > 64 - i * 7) {
                v = 64 - i * 7;
            }
            if (v < 0) {
                v = 0;
            }
            ((struct BattleFx9Work *)work)->ramp[i][j] = v;
        }
    }

    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_BlueCrescentSheet), 128);

    for (i = 0; i != 128; i++) {
        DUST[i].x = (Random16() & 15) + 72;
        DUST[i].velocity_x = Random16() & 0xffff;
        DUST[i].velocity_y = Random16() & 0xffff;
        DUST[i].velocity_y = Random16() & 0xffff;
    }

    for (i = 0; i != 128; i++) {
        s32 angle = Random16() & 0xffff;
        s32 amp = 255;
        s32 speed = 0;
        SPARKS[i].x = (Trig_Sin(angle) * amp) >> 2;
        SPARKS[i].z = (Trig_Cos(angle) * amp) >> 2;
        SPARKS[i].y = ((Random16() & 63) + 32) << 16;
        SPARKS[i].velocity_x = speed;
        SPARKS[i].velocity_y = speed;
        SPARKS[i].variant = angle;
    }

    work->transfer_mode = 2;
    work->transfer_value = 50;
    *(u16 *)0x0400000c = 0x784;

    for (frame = 0; frame != 160 && (frame <= 4 || !(gKeysRepeat & (KEY_A | KEY_B))); frame++) {
        slots = (DrawRectangle *)gWorkSlot;
        if (frame == 143) {
            Iwram_FillWords(canvas, 0x4000, 0x2a2a2a2a);
            Audio_PlayCue(145);
        }
        if (frame == 80) {
            Audio_PlayCue(142);
        }

        if (frame < 0) {
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
            for (i = 0; i != 128; i++) {
                s32 z;
                s32 size;

                EffectStep_AdvanceWithGravity3D(&SPARKS[i], 60, -0x400);
                EffectPosition_ApplyBaseAndYOffset(&SPARKS[i], (struct EffectPosition *)scene);
                scene[0] >>= 1;
                scene[1] -= 120;
                if (scene[2] < 100) {
                    scene[2] = 100;
                }
                if (scene[2] > 900) {
                    scene[2] = 900;
                }
                z = scene[2] - 100;
                scene[2] = z;
                size = __divsi3(z, 100) + 1;
                if (near > z) {
                    near = z;
                }
                if (far < z) {
                    far = z;
                }
                draw[0](canvas, ramp + ParticleStreams_CellOffsets[size - 1], scene[0] - size / 2, scene[1] - size, size,
                    size * 2);
                if (frame == 64) {
                    SPARKS[i].velocity_x += (Trig_Sin(SPARKS[i].variant) * 255) >> 3;
                    SPARKS[i].velocity_y += (Trig_Cos(SPARKS[i].variant) * 255) >> 3;
                }
            }
        }

        if (frame == 72) {
            work->fade_frames = 24;
            work->fade_step = 0;
        }

        anchor[2] = 0;
        anchor[1] = 0;
        for (i = 0; i != 64; i++) {
            if (frame > i / 4 + 80 && DUST[i].x > 0) {
                s32 size;

                Render_ResetTransformState();
                SceneTransform_ApplyRoll(DUST[i].velocity_y);
                SceneTransform_ApplyPitch(DUST[i].velocity_x);
                SceneTransform_ApplyYaw(DUST[i].velocity_y + (frame << 9));
                anchor[0] = DUST[i].x;
                DUST[i].x -= 2;
                if (DUST[i].x < 0) {
                    DUST[i].x = 0;
                }
                EffectPosition_ApplyBaseAndYOffset((struct EffectStep *)anchor, (struct EffectPosition *)scene);
                if (scene[2] < -60) {
                    scene[2] = -60;
                }
                if (scene[2] > 60) {
                    scene[2] = 60;
                }
                scene[2] += 60;
                size = __divsi3(scene[2], 20) + 2;
                scene[0] += 60;
                scene[1] += 80;
                draw[0](canvas, ramp + ParticleStreams_CellOffsets[size - 1], scene[0] - size / 2, scene[1] - size, size,
                    size * 2);
            }
        }

        pair = BattleFx9_UnitScale;
        place[3] = 0;
        place[1] = 0;
        for (i = 0; i != 16; i++) {
            place[0] = ((i % 4) << 21) + (152 << 15);
            place[2] = ((i / 4) << 21) + depth;
            Object_ApplyProjectedPlacementFar(work->objects[i], place, &pair, 0);
        }

        depth += zoom;
        if (frame <= 47) {
            zoom -= 0x4000;
        }
        if (frame > 32) {
            zoom = zoom * 60 / 64;
        }
        if (frame == 144) {
            zoom = -0x80000;
        }
        if (frame == 146) {
            zoom = 0x10000;
        }

        if (frame == 72) {
            for (i = 0; i != 16; i++) {
                AnimationObjects_SelectAnimationFar(work->objects[i], i + 16);
            }
        }
        if (frame == 76) {
            for (i = 0; i != 16; i++) {
                AnimationObjects_SelectAnimationFar(work->objects[i], i + 32);
            }
        }

        if (frame >= 116 && frame < 144) {
            s32 high;
            s32 cell;
            s32 n = (frame - 116) / 4;
            if (n > 6) {
                n = 6;
            }
            high = BattleFx9_BurstHeights[n];
            cell = BattleFx9_BurstCells[n] + 0x600;

            BattleEffect_LoadWork(47, 7, 7, 3, 2);
            (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, work->sheet + cell, 60 - high, 80 - high * 2, high, high * 2);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
            (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, work->sheet + cell, 60, 80 - high * 2, high, high * 2);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 11, 2);
            (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, work->sheet + cell, 60 - high, 80, high, high * 2);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 15, 2);
            (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, work->sheet + cell, 60, 80, high, high * 2);
            Runtime_ReleaseHeapBlock(47);

            for (i = 0; i != 6; i++) {
                s32 k = i & 3;
                s32 angle = Random16() & 0xffff;
                s32 x = ((Trig_Sin(angle) * 16) >> 16) - BattleFx9_ShardWidths[k] / 2;
                s32 y = ((Trig_Cos(angle) * 16) >> 16) - BattleFx9_ShardHeights[k] / 2;
                draw[0](canvas, work->sheet + BattleFx9_ShardCells[k], x + 60, y + 80, BattleFx9_ShardWidths[k],
                    BattleFx9_ShardHeights[k]);
            }
        }

        if (frame > 143) {
            s32 top = (frame << 4) - 2272;
            s32 n;
            s32 r;
            BattleEffect_LoadWork(47, 7, 7, 3, 3);
            (slots[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, work, 36, top, 24, 64);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 7, 3);
            (slots[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, work, 60, top, 24, 64);
            Runtime_ReleaseHeapBlock(47);

            n = (frame - 144) / 2;
            r = frame * 10 - 1424;
            work->transfer_value = 75;
            if (n <= 6) {
                s32 cell = n * 770 + ParticleStreams_CellOffsets[7] + 0x2710;
                s32 high = 8;
                BattleEffect_LoadWork(47, 7, 7, 3, 2);
                blit = slots[HEAP_SLOT_BLITTER_ALTERNATE];
                for (i = 0; i != 128; i++) {
                    s32 angle = i << 9;
                    s32 x = ((r * Trig_Sin(angle)) >> 16) - high / 2;
                    s32 y = (r * Trig_Cos(angle)) >> 17;
                    blit(canvas, work->sheet + cell, x + 60, y + 72, high, high * 2);
                }
                Runtime_ReleaseHeapBlock(47);
            }
        }

        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Iwram_FillWords(canvas, 0x4000, 0);

    for (i = 0; i != 16; i++) {
        ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
    }

    for (i = 0; i != 16; i++) {
        struct AnimationObject *obj = (struct AnimationObject *)ResourceObject_CreateFar(0x186);
        work->objects[i] = obj;
        if (obj != 0) {
            obj->flags = 0;
            AnimationObjects_SelectAnimationFar(obj, __modsi3(i, 3));
            ((struct AnimationObject *)work->objects[i])->part[0].priority = 0;
        }
    }

    Runtime_ReleaseHeapBlock(46);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    Resource_LoadAndDecompress((s32)&ResourceId_BlueCrescentSheet, work->sheet + 0x4000, 1, 1);

    {
        u8 *c = ramp;
        for (i = 0; i != 770; i++, c++) {
            if (*c > 32) {
                *c += 224;
            } else if (*c != 0) {
                *c = 1;
            }
        }
    }

    work->fade_frames = 0;
    work->fade_step = 0;
    BattleBackground_LoadFar(1, (s32)&ResourceId_AerialViewBackdrop, 0);
    *(u16 *)0x04000020 = 0x100;
    *(u32 *)0x04000028 = -0x3c00;
    *(u16 *)0x0400000c = 0x784;

    fill = 0;
    Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);

    cx = 60;
    cy = 44;
    shift = 0;
    ring = 0;
    squash = 6;
    sx = 8;
    rings = 2;
    work->transfer_mode = 2;
    work->transfer_value = 75;

    fill = shift;
    Dma_Set(&fill, work, 0x85000e10, (volatile u32 *)0x040000d4);

    for (frame = 0; frame != 320; frame++) {
        if (frame == 66) {
            Audio_PlayCue(145);
            Audio_PlayCue(141);
        }
        if (frame == 155) {
            Audio_PlayCue(162);
        }
        if (frame == 217) {
            Audio_PlayCue(156);
        }
        if (frame == 280) {
            Audio_PlayCue(157);
        }
        if (frame == 300) {
            BattleEventRuntime_BeginPhaseFar(145);
        }

        if (gKeysRepeat & (KEY_A | KEY_B)) {
            if (frame >= 5 && frame < 150) {
                frame = 150;
                QUEUE_IO_WRITE(0x04000020, 0x80, 0x20000);
                QUEUE_IO_WRITE(0x04000028, 0, 0x30000);
                Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet, work->sheet + 0x4000, 1, 0);
            } else if (frame >= 155 && frame < 214) {
                frame = 214;
            } else if (frame >= 219 && frame < 280) {
                frame = 280;
            }
        }

        if (frame == 64) {
            QUEUE_IO_WRITE(0x04000020, 0x80, 0x20000);
            QUEUE_IO_WRITE(0x04000028, 0, 0x30000);
            fill = 0;
            Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
            Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet, work->sheet + 0x4000, 1, 0);
            Resource_LoadAndDecompress((s32)&ResourceId_OverlayImageA, work->sheet + 0x6000, 0, 0);
        }

        if (frame == 66) {
            u16 *pal = (u16 *)0x050000c0;
            for (i = 0; i != 128; i++, pal++) {
                *pal ^= 0x7fff;
            }
        }

        if (frame == 69) {
            fill = 0x3f3f3f3f;
            Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
        }

        if (frame == 70) {
            BattlePresentation_ConfigurePaletteFadeFar(1, (s32)&ResourceId_AerialViewBackdrop, 7);
        }

        if (frame == 150) {
            cx = 112;
            cy = 32;
            ring = 0;
            fill = 0;
            rings = 4;
            squash = 8;
            Dma_Set(&fill, work, 0x85000e10, (volatile u32 *)0x040000d4);
            fill = 0x3f3f3f3f;
            Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
            BattleBackground_LoadFar(1, (s32)&ResourceId_BeachBackdrop, 0);
        }

        if (frame == 214) {
            fill = 0;
            Dma_Set(&fill, work, 0x85000e10, (volatile u32 *)0x040000d4);
            fill = 0x3f3f3f3f;
            Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
            BattleBackground_LoadFar(1, (s32)&ResourceId_ForestBackdrop, 0);
            for (i = 0; i != 16; i++) {
                p = &work->particles[i];
                p->x = ((Random16() & 63) + 32) << 16;
                p->y = ((Random16() & 31) + 96) << 16;
                p->velocity_y = ((Random16() & 15) + 16) << 15;
                p->z = 0x8000 - (p->x >> 7);
                p->variant = 1;
            }
        }

        if (frame == 280) {
            BattleEffect_SetupBlendedDisplay();
            fill = 0;
            Dma_Set(&fill, work, 0x85000e10, (volatile u32 *)0x040000d4);
            fill = 0x01010101;
            Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
            *(u16 *)0x04000052 = 0x1010;
            ctrl->initial_value = 0;
            for (i = 0; i != 16; i++) {
                p = &work->particles[i];
                p->x = ((Random16() & 63) + 256) << 16;
                p->y = ((Random16() & 31) + 96) << 16;
                p->velocity_y = ((Random16() & 15) + 16) << 15;
                p->z = 0;
            }
            shift = 272;
            work->transfer_mode = 0;
            work->transfer_value = 0;
        }

        if (frame > 279) {
            u16 *pal = (u16 *)0x05000002;
            for (i = 0; i != 63; i++, pal++) {
                s32 c = *pal;
                s32 b = ((c >> 10) & 31) + 1;
                s32 g = ((c >> 5) & 31) + 1;
                s32 r = (c & 31) + 1;
                if (b > 31) {
                    b = 31;
                }
                if (g > 31) {
                    g = 31;
                }
                if (r > 31) {
                    r = 31;
                }
                *pal = (b << 10) | (g << 5) | r;
            }
        }

        if (frame == 182) {
            fill = 0x3f3f3f3f;
            Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
        }

        if (frame <= 63) {
            u8 *sheet;
            s32 top = frame - 4;
            if ((frame & 7) < 4) {
                sheet = work->sheet + 0x4000;
            } else {
                sheet = work->sheet + 0x4240;
            }
            BattleEffect_LoadWork(47, 7, 7, 3, 2);
            (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, sheet, 36, top - 24, 24, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
            (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, sheet, 59, top - 24, 24, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 11, 2);
            (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, sheet, 36, top - 1, 24, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 15, 2);
            (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, sheet, 59, top - 1, 24, 24);
            Runtime_ReleaseHeapBlock(47);
        }

        if (frame >= 64 && frame < 66) {
            draw[0](canvas, work->sheet + 0x6000, 52, 51, 16, 17);
        }
        if (frame >= 66 && frame < 68) {
            draw[0](canvas, work->sheet + 0x6110, 48, 40, 24, 41);
        }
        if (frame >= 68 && frame < 76) {
            s32 d = 76 - frame;
            draw[0](canvas, work->sheet + 0x64e8, 49 - d / 2, 38 - d, 22, 44);
            draw[0](canvas, work->sheet + 0x64e8, 49 - d / 2, d + 38, 22, 44);
            draw[0](canvas, work->sheet + 0x64e8, d / 2 + 49, 38 - d, 22, 44);
            draw[0](canvas, work->sheet + 0x64e8, d / 2 + 49, d + 38, 22, 44);
        }
        if (frame >= 78 && frame < 80) {
            draw[0](canvas, work->sheet + 0x6000, 52, 51, 16, 17);
        }
        if (frame >= 80 && frame < 82) {
            draw[0](canvas, work->sheet + 0x6110, 48, 40, 24, 41);
        }
        if (frame >= 82 && frame < 86) {
            s32 d = frame * 2 - 164;
            draw[0](canvas, work->sheet + 0x64e8, 49 - d / 4, 38 - d, 22, 44);
            draw[0](canvas, work->sheet + 0x64e8, 49 - d / 4, d + 38, 22, 44);
            draw[0](canvas, work->sheet + 0x64e8, d / 4 + 49, 38 - d, 22, 44);
            draw[0](canvas, work->sheet + 0x64e8, d / 4 + 49, d + 38, 22, 44);
        }

        if (frame >= 72 && frame < 76) {
            s32 r2 = (frame << 4) - 1136;
            s32 size = 10;
            for (i = 0; i != 256; i++) {
                s32 angle = i << 8;
                s32 x = ((Trig_Sin(angle) * r2) >> 16) - size / 2;
                s32 y = (Trig_Cos(angle) * r2) >> 17;
                draw[0](canvas, ramp + ParticleStreams_CellOffsets[size - 1], x + 60, y + 50, size, size * 2);
            }
        }

        if (frame > 85) {
            if (frame <= 213) {
                for (i = 0; i != rings; i++, ring++) {
                    s32 radius = ring;
                    s32 slope = 0;
                    s32 err = ring;
                    while (radius >= slope) {
                        s32 half;
                        s32 half2;
                        s32 left;
                        s32 right;
                        s32 top;
                        s32 bottom;
    
                        half = radius / 2;
                        right = cx + (half * sx) / 8;
                        left = cx - (half * sx) / 8;
                        bottom = cy + (slope * squash) / 8;
                        top = cy - (slope * sx) / 8;
                        if (top < 0) {
                            top = 0;
                        }
                        if (bottom > 119) {
                            bottom = 119;
                        }
                        if (left < 0) {
                            left = 0;
                        }
                        if (right > 119) {
                            right = 119;
                        }
                        work->sheet[bottom * 120 + right] = 20;
                        work->sheet[top * 120 + right] = 20;
                        work->sheet[bottom * 120 + left] = 20;
                        work->sheet[top * 120 + left] = 20;
                        right = cx + half + 1;
                        left = cx - half + 1;
                        if (left < 0) {
                            left = 0;
                        }
                        if (right > 119) {
                            right = 119;
                        }
                        work->sheet[bottom * 120 + right] = 20;
                        work->sheet[top * 120 + right] = 20;
                        work->sheet[bottom * 120 + left] = 20;
                        work->sheet[top * 120 + left] = 20;
    
                        half2 = slope / 2;
                        right = cx + (half2 * sx) / 8;
                        left = cx - (half2 * sx) / 8;
                        bottom = cy + (radius * squash) / 8;
                        top = cy - (radius * sx) / 8;
                        if (left < 0) {
                            left = 0;
                        }
                        if (right > 119) {
                            right = 119;
                        }
                        if (top < 0) {
                            top = 0;
                        }
                        if (bottom > 119) {
                            bottom = 119;
                        }
                        work->sheet[bottom * 120 + right] = 20;
                        work->sheet[top * 120 + right] = 20;
                        work->sheet[bottom * 120 + left] = 20;
                        work->sheet[top * 120 + left] = 20;
                        right = cx + half2 + 1;
                        left = cx - half2 + 1;
                        if (left < 0) {
                            left = 0;
                        }
                        if (right > 119) {
                            right = 119;
                        }
                        work->sheet[bottom * 120 + right] = 20;
                        work->sheet[top * 120 + right] = 20;
                        work->sheet[bottom * 120 + left] = 20;
                        work->sheet[top * 120 + left] = 20;
    
                        err = err - slope * 2 - 1;
                        if (err < 0) {
                            err = err + radius * 2 - 2;
                            radius--;
                        }
                        slope++;
                    }
                }
            }
        }

        if (frame >= 86 && frame < 150) {
            s32 angle = -0x4000;
            for (i = 0; i != 9; i++, angle += 0x1000) {
                s32 x = ((frame - 86) * Trig_Sin(angle)) >> 16;
                s32 y = ((frame - 70) * Trig_Cos(angle)) >> 16;
                draw[0](canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000, x + 44, y + 17, 32, 54);
            }
        }

        BattleEffect_LoadWork(47, 7, 7, 3, 3);
        draw[1] = ((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE];
        if (frame > 85) {
            draw[1](canvas, work, 0, 0, 120, 120);
        }

        if (frame > 279) {
            shift -= 8;
        }

        if (frame > 319) {
            for (i = 0; i != work->effect->count; i++) {
                ObjectGroup_UpdateMembers(work->effect->actors[i], 14, 5, -1, 0);
            }
        }

        if (frame == 238) {
            fill = 0x3f3f3f3f;
            Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
        }

        if (frame >= 214 && frame < 280) {
            gBgScroll[1].x += 8;
        }

        if (frame >= 246 && frame < 280) {
            shift += 8;
            work->transfer_mode = 0;
            work->transfer_value = 75;
        }

        if (frame > 213) {
            place2[3] = 0;
            place2[1] = 0;
            for (i = 0; i != 16; i++) {
                p = &work->particles[i];
                if (p->variant != 0) {
                    scale.x = p->z + (p->y >> 7);
                    if (scale.x < 0x800) {
                        scale.x = 0x800;
                    }
                    scale.y = scale.x;
                    place2[0] = p->x;
                    place2[1] = 0xff0000;
                    place2[2] = p->y + 0xff0000;
                    Object_ApplyProjectedPlacementFar(work->objects[i], place2, &scale, 0);
                    p->y -= p->velocity_y;
                    if (p->y < 0x100000) {
                        if (frame <= 279) {
                            p->x = ((Random16() & 63) + shift + 32) << 16;
                        } else {
                            p->x = ((Random16() & 63) + shift - 32) << 16;
                        }
                        if (frame > 301) {
                            p->variant = 0;
                        } else if (frame > 245) {
                            p->y = 0x600000;
                        } else {
                            p->y = 0xc00000 - p->x;
                        }
                    }
                }
            }

            if (frame <= 279) {
                for (i = 0; i != 8; i++) {
                    s32 x = (i & 1) * 20 - (Random16() & 15) + (i / 2) * 4 + shift / 2 - 16;
                    s32 y = (i / 2) * 32 - (Random16() & 15);
                    draw[1](canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000, x, y, 32, 54);
                }
            } else {
                for (i = 0; i != 8; i++) {
                    s32 x = (i & 1) * 20 + (Random16() & 15) - (i / 2) * 4 + shift / 2 - 16;
                    s32 y = (i / 2) * 32 - (Random16() & 15);
                    draw[1](canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000, x, y, 32, 54);
                }
            }
        }

        Runtime_ReleaseHeapBlock(47);

        if (frame > 63) {
            s32 shake = (Random16() & 3) + 30;

            gBgScroll[1].y = shake;
        }

        work->transfer_pending = 1;
        WaitFrames(1);
    }

    for (i = 0; i != 16; i++) {
        ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
    }

    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
