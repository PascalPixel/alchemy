/* 080ea0d8: differ 953 of 2455 (listing 2455, draft 2438, common 1970).
 * Remaining: register allocation, scheduling, constant hoisting, repeated
 * loads and pool placement. The first particle and dust blocks now use
 * native's pointer, counter and size registers; their spill offsets and
 * several scheduling decisions still differ.
 * Ramp: the eight clamped copies are a two-dimensional u8 [8][770] array at
 * work + 0x2710, indexed [i][j]: the constant then folds into the address
 * after the variable part, loop.c creates it in the inner loop's preheader
 * and cannot hoist it, and the whole ramp loop matches the native's.
 * Constants: the particle height is now built inside the loop, in the
 * native registers and order. loop.c still hoists the IO queue's 0,
 * &fill and 24; reload rematerializes them, so the 320-frame loop's
 * final preheader now contains only frame = 0 as native. The palette
 * loop now has the native halfword widening, separate red mask and
 * instruction/register sequence (pool placement still differs).
 * Locals: the native fill word sits at sp+140, between place and place2, so
 * it is a declared volatile word, not an addressed scalar (that put it at
 * sp+116 after pair). The frame is now the native's 184 bytes, and draw2
 * occupies the native's sp+92 slot and reloads before each call. The local
 * arrays consequently sit at their native offsets. In
 * the petal blocks the native keeps draw in sl and spills the petal cell
 * (sp+28), and builds d as frame * 2 then subtracts 164.
 * Register allocation and remaining stack slots
 * still differ widely; the BattleFx9 data names remain draft
 * declarations pending labels at their native bytes. BattleEffectWork, camera,
 * presentation, scroll and animation layouts now come from the shared headers.
 * RAM buffers, IWRAM entries, heap cells, callbacks and resources use existing
 * source names. The local IO queue macro is replaced by its common definition.
 * Trial: the named animation priority bitfield adds one instruction (2325);
 * its byte store is retained until the remaining allocation is understood.
 * Trial: a plain u32 fill (2324); volatile u32 fill (2257, kept).
 * Trial: draw2 declared last instead of after ctrl (2257, no change).
 * Trial: petal cell written in each call, not a block local: second petal
 * block 2253, both blocks 2252 (kept).
 * Trial: palette colour as s32, not u16 (u16 loads with ldrsh): 2238, kept;
 * u16 with red computed first: 2252. Through a volatile u16 pointer with
 * a u16 colour: ldrh and the pool-loaded 0x1f mask appear as native, but
 * red still masks the shifted colour (0x1f0000) instead of the loaded
 * one (2202, not kept); (u16)c for blue and green only: 2202.
 * Trial: spark velocities from their own zero variable, not the ramp
 * limit: the counter takes r7 and the zero r8 as native's (2207, kept);
 * literal 0 and 255 are not hoisted and 255 becomes lsls/subs (2264).
 * Trial: ramp limit as 64 - i * 7 in the inner loop: the native's copy of
 * it into ip appears (kept); limit and offset both from i: 2248; a 770
 * variable: 2217; &sheet[pos + 0x2710]: no change.
 * Trial: respawn x tests frame <= 279 first, as the native branches (2206).
 * Trial: frame > 85 and frame <= 213 as nested ifs: the native tests them
 * separately, not as one unsigned range (2201, kept).
 * Trial: one counter i for every loop: it spills (2485).
 * Trial: draw reloaded through slots: 2219; respawn y tests inverted: 2230.
 * Trial: draw2 read as gTransitionWork[3], the native literal's spelling:
 * still no spill, frame 180 (2231).
 * Permuter, 6 minutes from 2238: 2145 only through temporaries, do-while
 * loop forms and dropped parentheses, none carried.
 * Trial: ramp store as work->sheet[pos + j + 0x2710]: 2209;
 * (work->sheet + pos + j)[0x2710]: 2222; ramp[i][j] in a [8][770] array
 * at 0x2710: 2174, kept, and the petal cell as &ramp[n][offset]: 2174.
 * Trial: the petal ring radius frame * 10 - 1424 computed once in the frame
 * body, between n and the transfer value as the native orders them: it
 * becomes a reduced giv of the frame loop as native's sp+12 (2162, kept);
 * its preheader still stores it in a different slot.
 * Lead: the native's 320-frame loop preheader holds nothing but frame = 0;
 * loop.c moved nothing out of that loop (each block rebuilds &fill with
 * add sp, #140 and stores fill at [sp, #140] directly). Ours moves 0, &fill
 * and 24 out of it. Diagnostic only, not source: the loop as a label and
 * goto gives the native's 184-byte frame but 2294 overall; as do-while
 * the code is identical (2162).
 * Trial (2026-10-07): one counter for the 160-frame phase's seven
 * sequential subloops: 2162 -> 2126, kept. Also sharing the second
 * phase's counter: 2232, not kept. Volatile draw2: 2242; a one-pass
 * block around the second frame loop: 2160; neither kept. Narrowing
 * draw2 to frame scope and volatile hardware writes leave 2126 unchanged.
 * Petals (kept): the first four placements compute x and y before each
 * draw, reusing the left x for its lower petal and the right x for its
 * lower petal. This restores the 184-byte frame and draw2's sp+92 spill:
 * 2126 -> 1901. The same rewrite in the second petal block is 2521;
 * only reusing its lower y is 1906, neither kept.
 * Trial: a nonvolatile empty-asm height operand: 2126 -> 2116, or
 * 1901 -> 1897 after the coordinate rewrite; not kept. Its invariant
 * asm can itself hoist, so it does not restore the native loop form.
 * The goto frame loop with it was 2228.
 * Permuter, 3 minutes, two jobs, seed 47, heat, from 2126: weighted
 * 54790 -> 31510, differ 1916, but 63 rewrites remain (mostly unrelated
 * shared temporaries). Not carried; the ordinary coordinate rewrite
 * above recovers the native frame without those devices.
 * Continuation (2026-10-07): volatile palette access and u16 casts on
 * blue/green give 1894; extracting all channels before incrementing
 * them gives 1862. The only local difference is then r6/r7 swapped;
 * signed versus unsigned count is unchanged. A tagged r7 count gives
 * 1850 and the native loop. Sharing this count with i4 gave 1926; one
 * count for all object loops except ramp/rings gave 2257, not kept.
 * Particle height: a dependency on p prevents hoisting but swaps the
 * index/scale registers (1862); reading i22 in the operand instead
 * restores r7/r8 (1797). A volatile operand on the 255 pixel value,
 * with x loaded before the placement stores, gives native's mov 255,
 * load x, shift 16 order (1791, kept). An additional empty volatile
 * asm is unnecessary. Without the index input it is 1860. A one-pass
 * scale-store block was 1887; fixing i22 to r7 altered induction
 * structure (1892), neither kept. The particle placement's temporary
 * still spills at sp+12 instead of native's sp+8.
 * Counter continuation: the setup and cleanup loops now share i.
 * Reusing the spark setup counter in the final release loop gives 1785,
 * also sharing it with the intervening release/create/ramp loops gives
 * 1769 and their native r7 counters. The two initial counters can join
 * it without changing code. Named EffectPosition fields and correct
 * helper argument types likewise preserve code, and are kept.
 * Rejected counter trials: every counter fixed to r7 gave 2138, every
 * counter plain gave 2254; sharing the two phases gave 2052. Sharing
 * setup with the first phase gave 2033; sharing the ramp's inner j with
 * the first phase gave 1809. A fixed first-phase counter gave 1752 but
 * invalidated its reduced addresses, so it is not a usable improvement.
 * A fixed release counter likewise invalidates reduction unless the
 * pointer is incremented explicitly; ordinary counter reuse above
 * obtains the same native loop without a device. An input-only counter
 * asm does nothing. Sharing just the last two drawing counters is unchanged.
 * Position trials: a call-only pointer gives 1793, using it for fields
 * gives 1789; narrowing the count and explicit particle increments do
 * not improve that. A fixed pointer gives 1815; an empty pointer operand
 * after the setup calls gives 1774 from 1769, a constrained r8 operand
 * gives 1779. A constrained r7 counter with explicit particle increments
 * gives 1835 and a 180-byte frame; a narrower particle lifetime is unchanged.
 * Last drawing loops: moving -16 into the draw argument gives 1791,
 * a separate x -= 16 gives 1803; also sharing their counter with setup
 * gives 2259. Local x/y give 1986, or 2034 with -16 in the argument;
 * a fixed shared r7 counter gives 2041. None of these are kept.
 * Scroll mask: a signed random temporary alone leaves 1769 unchanged.
 * A mask operand depending on the random value gives 1754; putting the
 * operand after mask &= rnd gives 1752. Removing the operand still gives
 * 1752: assigning 3 to the mask and then updating it is sufficient, and
 * is kept without a device. The scroll calculation and closing release
 * loop now match locally, including removal of the extra pool jump.
 * DMA one-pass blocks inside the second phase gave 2215, not kept.
 * Sequential updates for the last loops' x calculation gave 2190 from
 * 1752, not kept. A focused one-minute, two-job permuter search (seed
 * 103) over the first particle block compiled 12 distinct candidates;
 * none beat weighted 36140 / differ 1752. Narrowing the palette counter
 * to its block preserves 1752 and is kept; removing its register binding
 * gives 1764 without improving the first phase. A high-register position
 * operand in the first particle loop hits an agscc reload ICE, not kept.
 * Scope continuation: a local depth copy preserving the clamp stores
 * gives 1769 from 1752. Separating the two size lifetimes gives 1674;
 * narrowing z and the dust size preserves it. A separate first counter
 * gives 1754, a < 128 bound gives 1675. Field-local coordinate copies
 * give 1731, complete local draw coordinates 1727; none are kept.
 * Burst offset before the resource call and base addition afterward,
 * including two pointer updates, leaves 1674 unchanged; an offset operand
 * delays the addition but gives 1693, not kept.
 * Local shard x/y give 1659. The native reloads shard width after cosine:
 * removing the draft's unproven const qualifier restores that reload and
 * gives 1573, with a 180-byte frame. Local final-loop coordinates give
 * 1510; sharing their counter with setup gives 1497 and native's arithmetic
 * registers. The callback still stays in fp. Narrowing it changes nothing;
 * volatile gives 1488 but a wrong 188-byte frame. An fp clobber after its
 * assignment spills it (1433); retaining the root declaration puts that
 * spill at native's sp+92 and restores the 184-byte frame (1215).
 * Local first-ring x/y give 1101, local second-ring x/y 1097, local arc
 * x/y 1073. Removing the callback constraint then gives 1372. Moving it
 * after the initial callback use gives 1071; merging the same clobber
 * into the existing particle-height operand preserves 1071 and is kept.
 * A plain palette count and explicit red-mask update give 1083, not kept.
 * Further lifetimes: first-petal x/y can be block locals without changing
 * 1071. Plain callback copies disappear; a second-petal callback operand
 * gives 1471, a fixed sl callback 1465, both petal callbacks bound 1463;
 * those forms make a 180-byte frame and are rejected. Persistent first-
 * petal top/left coordinates give 1498; second-petal x/y give 1173.
 * Sharing all second-phase counters with i4 gives 1850, not kept.
 * Second ring: high = 10, using high / 2, CellOffsets[high - 1] and
 * high * 2 preserves the native diameter relationship. A variable draw
 * width gives 1172; literal width 10 gives 1069. Sharing its count with
 * i4 gives 1030; a literal height 20 gives 1034. Kept the diameter form,
 * literal width and shared burst counter. Grouping cell offsets in array
 * indexes leaves 1030 unchanged for both the burst and first ring.
 * Circle: sharing its count with i4 gives 1636, with i gives 1397.
 * Separate half-radius locals give 1536, separate complete coordinate
 * scopes 1627. A bound r7 count gives 1022 but is invalid: the compiler
 * overwrites r7 with half, even with a trailing operand, so rejected.
 * An ordinary low-register count operand gives 1050, not kept.
 * Binding the two half-radius locals gives 1048; binding all three circle
 * state locals too gives 953. Only the three state bindings gives 994
 * with a shared half, 1518 with separate halves. The half bindings plus
 * slope give 1042; adding radius gives 978. Sharing the setup count then
 * gives 953. Removing the now-redundant slope binding preserves 953;
 * removing radius gives 1421, first half 964, second half 1371. Kept just
 * radius and the two half bindings, with the ordinary shared setup count.
 * An operand after the first half assignment gives 982, not kept. */
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
extern const u16 BattleFx9_ShardCells[];
extern u8 BattleFx9_ShardWidths[];
extern const u8 BattleFx9_ShardHeights[];

#define DUST ((struct EffectStep *)Ram_MapCellBuffer)
#define SPARKS (&DUST[128])

void Unnamed_080ea0d8(struct BattleEffectArgument *efx)
{
    s32 anchor[3];
    struct EffectPosition scene;
    s32 place[4];
    volatile u32 fill;
    s32 place2[4];
    Scale pair;
    Scale scale;
    void *canvas;
    struct BattleEffectWork *work;
    struct BattlePresentationWork *ctrl;
    DrawRectangle draw2;
    DrawRectangle draw;
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
    s32 i4;
    s32 i14;
    s32 i15;
    s32 i16;
    s32 i20;
    s32 i21;
    s32 i22;
    s32 j;
    s32 high;
    s32 speed;
    struct EffectStep *p;
    DrawRectangle blit;
    s32 amp;

    cache = &gBattleFxWork[1];
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
    draw = (DrawRectangle)cache[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_CANVAS];
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

    amp = 255;
    speed = 0;
    for (i = 0; i != 128; i++) {
        s32 angle = Random16() & 0xffff;
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

    slots = (DrawRectangle *)gWorkSlot;
    for (frame = 0; frame != 160 && (frame <= 4 || !(gKeysRepeat & (KEY_A | KEY_B))); frame++) {
        if (frame == 143) {
            Iwram_FillWords(canvas, 0x4000, 0x2a2a2a2a);
            Audio_PlayCue(145);
        }
        if (frame == 80) {
            Audio_PlayCue(142);
        }

        if (frame < 0) {
            s32 size;
            s32 z;
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
            for (i4 = 0; i4 != 128; i4++) {
                EffectStep_AdvanceWithGravity3D(&SPARKS[i4], 60, -0x400);
                EffectPosition_ApplyBaseAndYOffset((s32 *)&SPARKS[i4], &scene);
                scene.x >>= 1;
                scene.y -= 120;
                if (scene.depth < 100) {
                    scene.depth = 100;
                }
                if (scene.depth > 900) {
                    scene.depth = 900;
                }
                z = scene.depth - 100;
                scene.depth = z;
                size = __divsi3(z, 100) + 1;
                if (near > z) {
                    near = z;
                }
                if (far < z) {
                    far = z;
                }
                draw(canvas, ramp + ParticleStreams_CellOffsets[size - 1], scene.x - size / 2, scene.y - size, size,
                    size * 2);
                if (frame == 64) {
                    SPARKS[i4].velocity_x += (Trig_Sin(SPARKS[i4].variant) * 255) >> 3;
                    SPARKS[i4].velocity_y += (Trig_Cos(SPARKS[i4].variant) * 255) >> 3;
                }
            }
        }

        if (frame == 72) {
            work->fade_frames = 24;
            work->fade_step = 0;
        }

        anchor[2] = 0;
        anchor[1] = 0;
        for (i4 = 0; i4 != 64; i4++) {
            if (frame > i4 / 4 + 80 && DUST[i4].x > 0) {
                s32 size;
                Render_ResetTransformState();
                SceneTransform_ApplyRoll(DUST[i4].velocity_y);
                SceneTransform_ApplyPitch(DUST[i4].velocity_x);
                SceneTransform_ApplyYaw(DUST[i4].velocity_y + (frame << 9));
                anchor[0] = DUST[i4].x;
                DUST[i4].x -= 2;
                if (DUST[i4].x < 0) {
                    DUST[i4].x = 0;
                }
                EffectPosition_ApplyBaseAndYOffset(anchor, &scene);
                if (scene.depth < -60) {
                    scene.depth = -60;
                }
                if (scene.depth > 60) {
                    scene.depth = 60;
                }
                scene.depth += 60;
                size = __divsi3(scene.depth, 20) + 2;
                scene.x += 60;
                scene.y += 80;
                draw(canvas, ramp + ParticleStreams_CellOffsets[size - 1], scene.x - size / 2, scene.y - size, size,
                    size * 2);
            }
        }

        pair = BattleFx9_UnitScale;
        place[3] = 0;
        place[1] = 0;
        for (i4 = 0; i4 != 16; i4++) {
            place[0] = ((i4 % 4) << 21) + (152 << 15);
            place[2] = ((i4 / 4) << 21) + depth;
            Object_ApplyProjectedPlacementFar(work->objects[i4], place, &pair, 0);
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
            for (i4 = 0; i4 != 16; i4++) {
                AnimationObjects_SelectAnimationFar(work->objects[i4], i4 + 16);
            }
        }
        if (frame == 76) {
            for (i4 = 0; i4 != 16; i4++) {
                AnimationObjects_SelectAnimationFar(work->objects[i4], i4 + 32);
            }
        }

        if (frame >= 116 && frame < 144) {
            u8 *cell;
            s32 n = (frame - 116) / 4;
            if (n > 6) {
                n = 6;
            }
            high = BattleFx9_BurstHeights[n];
            cell = work->sheet + BattleFx9_BurstCells[n] + 0x600;

            BattleEffect_LoadWork(47, 7, 7, 3, 2);
            ((DrawRectangle)gTransitionWork[3])(canvas, cell, 60 - high, 80 - high * 2, high, high * 2);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
            ((DrawRectangle)gTransitionWork[3])(canvas, cell, 60, 80 - high * 2, high, high * 2);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 11, 2);
            ((DrawRectangle)gTransitionWork[3])(canvas, cell, 60 - high, 80, high, high * 2);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 15, 2);
            ((DrawRectangle)gTransitionWork[3])(canvas, cell, 60, 80, high, high * 2);
            Runtime_ReleaseHeapBlock(47);

            for (i4 = 0; i4 != 6; i4++) {
                s32 x, y;
                s32 k = i4 & 3;
                s32 angle = Random16() & 0xffff;
                x = ((Trig_Sin(angle) * 16) >> 16) - BattleFx9_ShardWidths[k] / 2;
                y = ((Trig_Cos(angle) * 16) >> 16) - BattleFx9_ShardHeights[k] / 2;
                draw(canvas, work->sheet + BattleFx9_ShardCells[k], x + 60, y + 80, BattleFx9_ShardWidths[k],
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
                u8 *cell = &((struct BattleFx9Work *)work)->ramp[n][ParticleStreams_CellOffsets[7]];
                high = 8;
                BattleEffect_LoadWork(47, 7, 7, 3, 2);
                blit = slots[HEAP_SLOT_BLITTER_ALTERNATE];
                for (i4 = 0; i4 != 128; i4++) {
                    s32 x, y;
                    s32 angle = i4 << 9;
                    x = ((r * Trig_Sin(angle)) >> 16) - high / 2;
                    y = (r * Trig_Cos(angle)) >> 17;
                    blit(canvas, cell, x + 60, y + 72, 8, 16);
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
            ((u8 *)work->objects[i])[9] &= ~12;
        }
    }

    Runtime_ReleaseHeapBlock(46);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw = ((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER];
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
                IO_WRITE_QUEUE_PUSH(0x04000020, 0x80, 0x20000);
                IO_WRITE_QUEUE_PUSH(0x04000028, 0, 0x30000);
                Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet, work->sheet + 0x4000, 1, 0);
            } else if (frame >= 155 && frame < 214) {
                frame = 214;
            } else if (frame >= 219 && frame < 280) {
                frame = 280;
            }
        }

        if (frame == 64) {
            IO_WRITE_QUEUE_PUSH(0x04000020, 0x80, 0x20000);
            IO_WRITE_QUEUE_PUSH(0x04000028, 0, 0x30000);
            fill = 0;
            Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
            Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet, work->sheet + 0x4000, 1, 0);
            Resource_LoadAndDecompress((s32)&ResourceId_OverlayImageA, work->sheet + 0x6000, 0, 0);
        }

        if (frame == 66) {
            u16 *pal = (u16 *)0x050000c0;
            for (i14 = 0; i14 != 128; i14++, pal++) {
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
            for (i15 = 0; i15 != 16; i15++) {
                p = &work->particles[i15];
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
            for (i16 = 0; i16 != 16; i16++) {
                p = &work->particles[i16];
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
            volatile u16 *pal = (volatile u16 *)0x05000002;
            /* FAKEMATCH: the palette counter uses r7 in the native loop; ordinary signed and unsigned counters swap r6/r7 with the shared mask (1862 -> 1850). */
            register s32 i17 asm("r7");
            for (i17 = 0; i17 != 63; i17++, pal++) {
                s32 c = *pal;
                s32 b = ((u16)c >> 10) & 31;
                s32 g = ((u16)c >> 5) & 31;
                s32 r = c & 31;
                b++;
                g++;
                r++;
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
            u8 *sheet = work->sheet + 0x4000;
            s32 top = frame - 28;
            if ((frame & 7) > 3) {
                sheet = work->sheet + 0x4240;
            }
            BattleEffect_LoadWork(47, 7, 7, 3, 2);
            ((DrawRectangle)gTransitionWork[3])(canvas, sheet, 36, top - 24 + 24, 24, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
            ((DrawRectangle)gTransitionWork[3])(canvas, sheet, 59, top, 24, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 11, 2);
            ((DrawRectangle)gTransitionWork[3])(canvas, sheet, 36, top + 23, 24, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 15, 2);
            ((DrawRectangle)gTransitionWork[3])(canvas, sheet, 59, top + 23, 24, 24);
            Runtime_ReleaseHeapBlock(47);
        }

        if (frame >= 64 && frame < 66) {
            draw(canvas, work->sheet + 0x6000, 52, 51, 16, 17);
        }
        if (frame >= 66 && frame < 68) {
            draw(canvas, work->sheet + 0x6110, 48, 40, 24, 41);
        }
        if (frame >= 68 && frame < 76) {
            s32 x, y;
            s32 d = 76 - frame;
            x = 49 - d / 2;
            y = 38 - d;
            draw(canvas, work->sheet + 0x64e8, x, y, 22, 44);
            y = d + 38;
            draw(canvas, work->sheet + 0x64e8, x, y, 22, 44);
            x = d / 2 + 49;
            y = 38 - d;
            draw(canvas, work->sheet + 0x64e8, x, y, 22, 44);
            y = d + 38;
            draw(canvas, work->sheet + 0x64e8, x, y, 22, 44);
        }
        if (frame >= 78 && frame < 80) {
            draw(canvas, work->sheet + 0x6000, 52, 51, 16, 17);
        }
        if (frame >= 80 && frame < 82) {
            draw(canvas, work->sheet + 0x6110, 48, 40, 24, 41);
        }
        if (frame >= 82 && frame < 86) {
            s32 d = frame * 2 - 164;
            draw(canvas, work->sheet + 0x64e8, 49 - d / 4, 38 - d, 22, 44);
            draw(canvas, work->sheet + 0x64e8, 49 - d / 4, frame * 2 - 126, 22, 44);
            draw(canvas, work->sheet + 0x64e8, d / 4 + 49, 38 - d, 22, 44);
            draw(canvas, work->sheet + 0x64e8, d / 4 + 49, frame * 2 - 126, 22, 44);
        }

        if (frame >= 72 && frame < 76) {
            s32 r2 = (frame << 4) - 1136;
            high = 10;
            for (i4 = 0; i4 != 256; i4++) {
                s32 x, y;
                s32 angle = i4 << 8;
                x = ((r2 * Trig_Sin(angle)) >> 16) - high / 2;
                y = (r2 * Trig_Cos(angle)) >> 17;
                draw(canvas, ramp + ParticleStreams_CellOffsets[high - 1], x + 60, y + 50, 10, high * 2);
            }
        }

        if (frame > 85) {
            if (frame <= 213) {
                for (i = 0; i != rings; i++, ring++) {
                    /* FAKEMATCH: native keeps the circle radius in lr across both mirrored halves; ordinary locals choose ip or r7 (1421 -> 953 with the retained half bindings). */
                    register s32 radius asm("r14") = ring;
                    s32 slope = 0;
                    s32 err = ring;
                    while (radius >= slope) {
                        /* FAKEMATCH: native keeps radius/2 in r9; separate ordinary half-radius locals choose low registers (964 -> 953 with the other retained bindings). */
                        register s32 half asm("r9");
                        /* FAKEMATCH: native keeps slope/2 in ip while drawing the mirrored half (1371 -> 953 with the other retained bindings). */
                        register s32 half2 asm("r12");
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
            for (i20 = 0; i20 != 9; i20++, angle += 0x1000) {
                s32 x, y;
                x = ((frame - 86) * Trig_Sin(angle)) >> 16;
                y = ((frame - 70) * Trig_Cos(angle)) >> 16;
                draw(canvas, work->sheet + ((frame / 2 + i20) % 4) * 1728 + 0x4000, x + 44, y + 17, 32, 54);
            }
        }

        BattleEffect_LoadWork(47, 7, 7, 3, 3);
        draw2 = ((DrawRectangle *)gWorkSlot)[47];
        if (frame > 85) {
            draw2(canvas, work, 0, 0, 120, 120);
        }

        if (frame > 279) {
            shift -= 8;
        }

        if (frame > 319) {
            if (work->effect->count != 0) {
                for (i21 = 0; i21 != work->effect->count; i21++) {
                    ObjectGroup_UpdateMembers(work->effect->actors[i21], 14, 5, -1, 0);
                }
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
            for (i22 = 0; i22 != 16; i22++) {
                p = &work->particles[i22];
                if (p->variant != 0) {
                    s32 h;
                    s32 px;
                    scale.x = p->z + (p->y >> 7);
                    if (scale.x < 0x800) {
                        scale.x = 0x800;
                    }
                    scale.y = scale.x;
                    h = 255;
                    /* FAKEMATCH: native rebuilds 255 per particle, loads x, then shifts by 16; the index input preserves its r7 and scale's r8 (1860 -> 1791). The fp clobber spills draw2 at native's sp+92 with a 184-byte frame; scopes alone keep it in fp (1372), volatile makes 188 bytes, this form gives 1071. */
                    asm volatile("" : "+r"(h) : "r"(i22) : "r11");
                    px = p->x;
                    place2[1] = h << 16;
                    place2[0] = px;
                    place2[2] = p->y + (h << 16);
                    Object_ApplyProjectedPlacementFar(work->objects[i22], place2, &scale, 0);
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
                    s32 x, y;

                    x = (i & 1) * 20 - (Random16() & 15) + (i / 2) * 4 + shift / 2 - 16;
                    y = (i / 2) * 32 - (Random16() & 15);
                    draw2(canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000, x, y, 32, 54);
                }
            } else {
                for (i = 0; i != 8; i++) {
                    s32 x, y;

                    x = (i & 1) * 20 + (Random16() & 15) - (i / 2) * 4 + shift / 2 - 16;
                    y = (i / 2) * 32 - (Random16() & 15);
                    draw2(canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000, x, y, 32, 54);
                }
            }
        }

        Runtime_ReleaseHeapBlock(47);

        if (frame > 63) {
            s32 rnd = Random16();
            s32 mask = 3;
            mask &= rnd;
            gBgScroll[1].y = mask + 30;
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
