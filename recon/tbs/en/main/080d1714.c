/* 080d1714: differ 123 of 1481 instructions (1482 generated, common
 * 1420). Battle effect mode 8 (DISPATCH.C): the targets spin and
 * rise in a whirlwind for 160 frames, then nine comet objects sway in,
 * 32 particles converge on their centre, sparks burst four times and
 * four marks fly out with a screen shake; the targets are put back at
 * frame 400. Name at adoption, e.g. BtlEfxWhirlComet.
 * BattleEffectWork, BattleEffectArgument, MotionObject and EffectStep
 * replace local partial layouts. The control flow already
 * follows the listing; what remains is register allocation and spill
 * slots: the native frame is 356 bytes and keeps index in r9 and frame
 * at sp+80; it strength-reduces frame - threshold into its own slot and
 * hoists (wave_x >> 17) + 32 out of the threshold loop. Loops the native
 * addresses from a literal or runtime + constant base are written by
 * index; loops it walks with a pointer keep one.
 * The native writes the random z velocity over velocity_y and uses that
 * value for both y and z advancement; preserve this behavior.
 * Trials:
 * - baseline 1479; types 1475; approved IWRAM calls 1477.
 * - AudioCommand_PlayFar, the listing's callee, for the cues: 1477.
 * - threshold as index * 12 + 330, no biv: 1490.
 * - mark pointer for particles[60 + index] outside the inner loop: 1486
 *   (frame 352); also inside it: 1473 (frame 336).
 * - if (frame - threshold >= 0): 1494.
 * - particles[step + 32].variant = 4 indexed, as the native's
 *   runtime + 0x7418 shows: 1471.
 * - map cell buffer cleared by index, the native's literal base: 1404.
 * - first particle loop by index: 1348; burst setup loop by index: 1338;
 *   distance loop by index: 1324; spark spawn loop by index: 1320.
 * - first-phase particle loop by index: 1581; spark update loop by
 *   index: 1370 (both kept as pointers).
 * - (a parallel session, on its 1191 base) the inner spark loop reading
 *   the mark position as runtime->particles[index + 60]: 1191, frame
 *   356; the 64-step init loop by pointer with its own counter: 1249,
 *   point = &runtime->particles[index] each iteration: 1184; register
 *   on mark, step and alive: 1184; permuter 4 minutes with
 *   PERM_RANDOMIZE over the frame loop's actor loop: none below 1184;
 *   mark x and velocity_x stored before y: 1182. Its greg dump: index
 *   (158 refs) is allocated after about 60 shorter allocnos and loop.c
 *   hoists runtime + 0x7828 out of the actor loop (savings 3) into r9,
 *   the register the native gives index.
 * - 64-particle setup loop through point = &particles[index] each
 *   iteration: the native's runtime + 0x7080 base, 1313 (a walking
 *   pointer hoists 120 and 63 out of the loop: 1376).
 * - screen.y >> 16 and screen.depth >>= 16 for the halfword reads:
 *   combine narrows them to ldrsh off the &screen register as the
 *   native does; ((s16 *)&screen)[3] made loop-invariant address
 *   pseudos that took r9-r11 before index: 1182.
 * - register s32 index asm("r9"): 1718 (a hard register is no biv).
 * - the burst loop reads particles[index + 60].velocity_x and _y by
 *   index, as the native's two runtime-relative givs of 28 per mark
 *   show (it keeps mark for x and y): 992, frame 356 as the native.
 * - burst angle as step * 2340, no angle variable: 1130 (the burst
 *   loop then allocates as the native: &work r8, &screen sl, step fp,
 *   the angle giv spilled; but the frame drops to 352); with
 *   center_x = (wave_x >> 17) + 32 before the threshold loop: 1137.
 * - the distance loop's three divisions as / dist: the libcall keeps
 *   each coordinate in a register across it, as the native does: 956;
 *   the other __divsi3, __modsi3 and __umodsi3 calls as / and %: 956.
 * - distance loop through point = &particles[index] each iteration,
 *   one giv as the native's r6: 909. The same in the burst setup loop
 *   and spark spawn loop: 909 (spawn kept for reading).
 * - threshold = index * 12 + 330 each iteration, no biv: 1095 (frame
 *   364, threshold + 8 its own giv).
 * - burst angle as step * 2340 again on 909: 1080, but index now
 *   gets r9 as the native: angle no longer holds r8 there, so &work
 *   takes r8 and &screen sl. The frame was 352: the native's missing
 *   slot is the hoisted (wave_x >> 17) + 32. center_x assigned before
 *   the threshold loop: 1085 (gcse reuses the earlier wave_x >> 17,
 *   frame 360); center_x = (wave_x >> 17) + 32 as the loop body's
 *   first statement, which loop hoists as the native: 892, frame 356.
 * - threshold declared in the frame > 255 block, a later pseudo as
 *   the native's low slot suggests: 888; center_x there too: 888.
 * - the per-frame actor loop as for (index = 0; index != count;
 *   index++): the native recomputes runtime + 0x7828 at the test, the
 *   top and the latch from one constant pseudo (r2) and never hoists
 *   the address; as a do-while loop moved it out (savings 3): 1023
 *   alone (frame 352, index * 2 then took fp instead of its sp+56
 *   slot).
 * - step merged into slot, one counter for every inner loop as the
 *   native's fp shows: 887, frame 356.
 * - the 9-object placement loop counts with index, the native's r9:
 *   793 (slot's priority now falls below index's, so global.c gives
 *   index r9 and slot fp as the native).
 * - draw sources as &runtime->sheet[frame / 4 % 3 * 3072 + ...]:
 *   runtime + offset in the native's operand order, 791.
 * - burst setup stores variant = 0 after z, as sched2 places it
 *   between the umodsi3 result and the z store: 792 (1471 generated).
 * - threshold = index * 12 + 330 (or 330 + index * 12) each
 *   iteration again, on 792: 1004, but 1481 instructions as the native
 *   and its loop end matches exactly: four reduced givs (0x7720 +
 *   28i, 28i, frame - threshold, threshold) incremented in the
 *   native's order. The frame grows to 364: wave_x >> 17 is hoisted
 *   out of the distance loop into its own slot (the native shifts
 *   wave_x inside each iteration; 792 hoists it too), and one more
 *   slot sits at sp+12. Ignoring stack and literal offsets it is 410
 *   lines against 792's 404: fix the hoist and the extra slot there.
 *   The hoist is gcse's PRE: the unconditional center_x in the
 *   threshold loop makes wave_x >> 17 anticipatable from the distance
 *   loop on, so PRE computes it once before that loop (expression
 *   reg 1148) and the distance loop, the alive draw and center_x reuse
 *   it.
 * - (wave_x >> 17) + 32 written inside if (frame == threshold), no
 *   center_x, with threshold from index: 732, frame 356 (PRE no longer
 *   hoists; the distance loop and alive draw shift wave_x as the
 *   native). Loop.c keeps the conditional x sum in the loop ("not
 *   desirable", life 3) where the native hoists it to sp+52; one extra
 *   slot remains at the low end, so the givs sit 4 bytes above the
 *   native's sp+16..28. center_x assigned before the loop again: 994.
 * - mark stores in the order x, y, velocity_x, velocity_y: the x sum
 *   lives long enough for loop.c to hoist it (sp+56, frame 360): 972;
 *   y, x, velocity_y, velocity_x: 980.
 * - the extra low slot was a caller-save: the final restore loop used
 *   the function-wide object pseudo (r5 everywhere), so base_angle's
 *   walking pointer lost r7 and was caller-saved in r2 (sp+12). A
 *   block-local obj there takes r2 as the native: 872 alone (frame
 *   352); with the y, x, vy, vx stores: 646, frame 356; with x, y,
 *   vx, vy: 638, the threshold loop head and loop end as the native.
 * - the frame == 160 actor loop as a for loop too: its pre-test no
 *   longer lends its runtime + 0x7828 register to the hoisted one,
 *   which the native recomputes: 632.
 * - Trig_Sin(angle) * speed, the call first, in the burst setup: the
 *   native ties speed to the mul output (mov rD, speed; mul rD, result):
 *   628; the same in wave_y (Trig_Sin(...) * size): 624. The angle as
 *   two statements (and, then += 0x9fff): 639.
 * - alive = 0 before the transform calls, sharing the shake zero as the
 *   native's mov sl, r2 shows: 624 (x, y, z store order: 633; alive
 *   first: 631).
 * - drift_x, drift_y, then shake_frames = 8 (all six orders tried, 598
 *   to 622): 598.
 * - index = 0 before mark = &runtime->particles[60]: the native shares
 *   the zero of index and the giv at sp+24: 567.
 * - slot = 0 before spark = &runtime->particles[32] likewise: 517
 *   (index = 0 before point or the spark update pointer: 521, 517).
 * - speed and angle block-local in the spark spawn and burst setup
 *   blocks: one pseudo across both loops had swapped speed and the
 *   walking pointer (r7/r6 where the native has r6/r7): 455 for
 *   speed, 421 with angle. span block-local: 449 in all four blocks,
 *   425 in one.
 * - alive = 0 right after shake.z, a permuter hint: 394 (index = 0
 *   before the placement setup: 429).
 * - size block-local in the mark draw (if (frame >= threshold)): 380;
 *   also in the first draw block or the wave block: 380; point,
 *   object or spark block-local, or the drift adds interleaved with
 *   the halvings: 380.
 * - the frame == 160 background id is the resource id
 *   ResourceId_VioletSkyBackdrop (entry 59), which the native loads
 *   from the literal pool once for both calls: 379.
 * - on 379: index = 0 before the wave and drift inits: 595 (drift
 *   inits first: 595); wave_x -= then += drift_x before wave_y: 388;
 *   alive = 0 after shake.x: 404; a block-local copy of draw_rectangle
 *   for the two-draw block: 379 (copy propagated away);
 *   threshold declared in the mark loop body: 379.
 * - width = 48 set once before the frame loop, for the four draw
 *   widths: a pseudo set once to a constant that global.c leaves
 *   without a register, so reload rematerializes 48 at every use as
 *   the native: 404; with (width - size) * 48 in the first draw too:
 *   378, both single draws now exact.
 * - on 378: drift adds or halvings in other orders, drift + wave
 *   operand order, the sine terms through a temporary: 378 to 382; a
 *   block-local draw_rectangle copy, src block-local or from
 *   runtime->sheet, width set elsewhere: 378; alive = 0 after shake.y,
 *   after shake.x or after the calls: 398 to 403.
 * - wave and drift as two-field structs (struct ScalePair wave, drift,
 *   in that order): 243 (drift before wave: 297). An 8-byte struct
 *   that is never addressed is a DImode pseudo; each field store
 *   computes into a new SImode pseudo and moves it into the subreg, so
 *   the native's sine and drift results land in the operand's register
 *   and wave.y is stored twice, and the spilled field reloads are not
 *   inherited, as the native reloads drift.x and drift.y twice. The
 *   rotation of reload registers after this block then matches too.
 * - the two draw functions as one struct DrawPair draw (rect, then
 *   rect_alt): 182, 1481 instructions as the native (rect_alt first:
 *   204). A call through a field of the spilled DImode pseudo copies
 *   the field into a new pseudo, which the two-draw block shares
 *   across its first call: loaded once into r4 and caller-saved at
 *   sp+8, as the native.
 * - on 182: the frame == 160 radius and spin loop as a for or while
 *   loop: 190. The distance sum as x * x + (y * y + z * z): 173, the
 *   squares now in r0, r2 and r1 as the native; z * z first: 179;
 *   y * y first: 186; the sum assigned to dist before Iwram_Sqrt: 230.
 * - on 173: index = 0 before point = runtime->particles in the
 *   frame <= 159 loop, as before the mark and spark pointers: 169;
 *   point = &runtime->particles[0]: 173.
 * - on 169: the frame == 160 radius and spin loop as a for loop: 133
 *   (jump.c's copied exit test is the guard, so cse no longer shares
 *   its runtime->effect load with the loop and loop.c hoists a fresh
 *   one, as the native's guard ldr [r5, r3] then preheader reload
 *   shows); the final restore loop as a for loop too: 127; the first
 *   actor loop as a for loop: 127, kept for one loop shape throughout.
 * - on 127: the distance sum in source order x * x + y * y + z * z:
 *   180 (expand interleaves products and adds); the three squares as
 *   statements xx, yy, zz, then Iwram_Sqrt(xx + yy + zz): 123, the
 *   native's three products, x + y, then z through its copy in r3.
 * Remaining (123): two spill slots swapped (view + 12 at sp+36 and
 * &base_angle at sp+40 in the native); the frame <= 159 particle loop
 * preheader and the mark and spark loop preheaders choose other
 * reload registers (reload's rotation: &work through r0 and slot 0
 * through r7 in the native's spark preheader); the threshold loop
 * preheader sets mark (runtime + 0x7710) before the threshold givs,
 * where the draft sets it last, and loads one more literal (1482
 * generated). */
#include "CANVAS.H"
#include "MOTION_OBJECT.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "PROJECT.H"
#include "B5_CONTEXT.H"
#include "BATTLE_TYPES.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "EFFECT_STEP.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "RESOURCE_IDS.H"


struct Vector3 {
    s32 x;
    s32 y;
    s32 z;
};

struct Placement {
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_0c;
};

struct DrawPair {
    DrawRectangle rect;
    DrawRectangle rect_alt;
};

struct ScalePair {
    s32 x;
    s32 y;
};

extern void *gBattleFxWork[];

extern u32 gKeysRepeat;
extern u16 ParticleStreams_CellOffsets[];
extern const struct ScalePair Data_080eda78;
extern u8 Data_080ee15a[];
extern u8 Data_080ee163[];
extern u16 Data_080ee16c[];
extern u8 Data_080ee174[];
extern u8 Data_080ee177[];
extern u8 Data_080ee17a[];
extern u16 Data_080ee17e[];

s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 interval);
void Scheduler_RemoveCallback(void (*callback)(void));
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
s32 Random16(void);
s32 ArcTan2(s32 x, s32 z);
s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(void *source, void *destination);
void SceneTransform_ApplyYaw(s32 yaw);
void SceneTransform_ApplyRoll(s32 pitch);
void SceneTransform_ApplyPosition(struct Vector3 *position);
void AudioCommand_PlayFar(s32 cue);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);
void BattlePresentation_ConfigurePaletteFadeFar(s32, s32, s32);
void BattleBackground_LoadFar(s32, s32, s32);
void BattleEventRuntime_BeginPhaseFar(s32 value);
void Object_ApplyProjectedPlacementFar(void *handle, struct Placement *place, struct ScalePair *scale,
    s32 mode);
void BattleEffect_SetupBlendedDisplay(void);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 count);

void Unnamed_080d1714(struct BattleEffectArgument *argument)
{
    void **cells;
    void *draw_destination;
    void *view;
    struct BattleEffectWork *runtime;
    struct DrawPair draw;
    s32 frame;
    u8 *graphics;
    struct ScalePair wave;
    struct ScalePair drift;
    struct MotionObject *object;
    struct BattleObjectSlot *context;
    struct EffectStep *point;
    struct EffectStep *spark;
    struct EffectStep *mark;
    u8 *tbl;
    u8 *src;
    s32 index;
    s32 slot;
    s32 size;
    s32 span;
    s32 dist;
    s32 alive;
    s32 width;
    s32 heading[8];
    s32 radius[8];
    s32 spin[8];
    struct ScalePair home[8];
    s32 base_angle[8];
    struct Vector3 work;
    struct EffectPosition screen;
    struct Vector3 shake;
    struct Placement place;
    struct ScalePair scale;

    cells = gBattleFxWork;
    draw_destination = cells[0];
    view = *(void **)((u8 *)cells - 112);
    runtime = cells[-1];
    graphics = (u8 *)cells[1];
    runtime->effect = argument;

    BattleFx_BeginCanvasLayer(0);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw.rect = (DrawRectangle)cells[6];
    BattleEffect_LoadWork(47, 7, 7, 3, 3);
    draw.rect_alt = (DrawRectangle)cells[7];

    tbl = Resource_GetTableEntry((s32)&ResourceId_WhirlwindSheet);
    Iwram_CopyWords((void *)0x05000000, tbl, 0x80);
    tbl += 0x80;
    Resource_DecodeType01(tbl, runtime);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);

    runtime->transfer_mode = 2;
    runtime->transfer_value = 50;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    wave.x = 0x800000;
    wave.y = 0x280000;
    drift.x = 0;
    drift.y = 0;

    for (index = 0; index != runtime->effect->count; index++) {
        context = GetBattleObjectSlotFar(runtime->effect->actors[index]);
        object = (struct MotionObject *)context->object;
        home[index].x = object->x;
        home[index].y = object->z;
        base_angle[index] = object->angle;
        heading[index] = (u16)ArcTan2(object->x, object->z);
        radius[index] = Iwram_Sqrt(
            (object->x >> 8) * (object->x >> 8)
                + (object->z >> 8) * (object->z >> 8)) >> 7;
        spin[index] = 0;
        object->vertical_motion_strength = 0;
    }

    index = 0;
    do {
        point = &runtime->particles[index];
        point->z = 120;
        point->y = 0;
        Random16();
        point->velocity_y = 0;
        point->velocity_x = 0;
        point->variant = Random16() & 63;
        index++;
    } while (index != 64);

    width = 48;
    frame = 0;
    do {
        if ((gKeysRepeat & (KEY_A | KEY_B)) != 0) {
            if (frame <= 159)
                frame = 160;
            else if (frame <= 394)
                frame = 395;
        }

        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(view, (u8 *)view + 12);

        if (frame == 16)
            AudioCommand_PlayFar(141);
        if (frame == 256)
            AudioCommand_PlayFar(140);
        if (frame == 334)
            AudioCommand_PlayFar(212);
        if (frame == 347)
            AudioCommand_PlayFar(212);
        if (frame == 359)
            AudioCommand_PlayFar(212);
        if (frame == 372)
            AudioCommand_PlayFar(212);

        for (index = 0; index != runtime->effect->count; index++) {
            if (frame > index * 16) {
                context = GetBattleObjectSlotFar(
                    runtime->effect->actors[index]);
                object = (struct MotionObject *)context->object;
                object->x =
                    (radius[index] * Trig_Sin(heading[index])) >> 1;
                object->z =
                    (radius[index] * Trig_Cos(heading[index])) >> 1;

                if (frame <= 159) {
                    if (frame > index * 16 + 16)
                        spin[index] += 48;
                    if (radius[index] <= 31)
                        object->y += 0x60000;
                    else
                        object->y += 0x8000;
                    if (object->y > 0x7c0000)
                        object->y = 0x7c0000;
                }

                if (frame <= 490) {
                    heading[index] += spin[index];
                    if (heading[index] > 0x10000)
                        heading[index] -= 0x10000;
                    object->angle += spin[index] / 4;
                }

                if (frame == 395)
                    object->vertical_motion_strength = 0xab85;

                slot = 0;
                do {
                    if (frame == Data_080ee16c[slot]) {
                        object->velocity_y = 0;
                        ObjectGroup_UpdateMembers(
                            runtime->effect->actors[index],
                            7,
                            5,
                            index,
                            8);
                    }
                    slot++;
                } while (slot != 4);

                if (radius[index] > 16)
                    radius[index] -= 2;
            }
        }

        if ((u32)(frame - 16) <= 143) {
            size = frame * 2 - 32;
            if (size > 48)
                size = 48;
            draw.rect(
                draw_destination,
                &runtime->sheet[frame / 4 % 3 * 3072
                    + (width - size) * 48],
                32,
                112 - size,
                width,
                size);
        }

        if ((u32)(frame - 48) <= 111) {
            size = frame * 2 - 96;
            if (size > 64)
                size = 64;
            draw.rect(
                draw_destination,
                &runtime->sheet[frame / 4 % 3 * 3072
                    + (64 - size) * 48],
                32,
                64 - size,
                width,
                size);
        }

        if ((u32)(frame - 160) <= 239) {
            src = (u8 *)runtime + frame / 4 % 3 * 3072;
            draw.rect(draw_destination, src, 8, 0, width, 64);
            draw.rect(draw_destination, src, 8, 64, width, 64);
        }

        if (frame <= 159) {
            index = 0;
            point = runtime->particles;
            do {
                if (point->variant == 0) {
                    work.x = point->z * Trig_Sin(point->velocity_y);
                    work.y = point->y;
                    work.z = point->z * Trig_Cos(point->velocity_y);
                    EffectPosition_ApplyBaseAndYOffset(&work.x, &screen);
                    screen.x >>= 1;
                    if (point->y <= 0x3fffff) {
                        draw.rect(
                            draw_destination,
                            (u8 *)runtime + 0x2400,
                            screen.x - 12,
                            screen.y - 12,
                            16,
                            16);
                    }
                    if (point->z > 24)
                        point->z -= 4;
                    point->velocity_y += point->velocity_x * 2;
                    if (point->velocity_y > 0x10000)
                        point->velocity_y -= 0x10000;
                    point->velocity_x += 50;
                    if (point->velocity_x > 4096)
                        point->velocity_x = 4096;
                    point->velocity_y += 2048;
                    point->y += point->velocity_y * 2;
                    if (point->y > 0x300000) {
                        point->y = 0;
                        point->z = 100;
                        point->velocity_y = 0;
                        point->velocity_x = 0;
                    }
                } else {
                    point->variant--;
                }
                index++;
                point++;
            } while (index != 24);
        }

        if (frame == 160) {
            s32 speed;
            s32 angle;

            BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
            BattlePresentation_ConfigurePaletteFadeFar(1, (s32)&ResourceId_VioletSkyBackdrop, 8);
            BattleFx_SelectLivingTargets(runtime->effect);
            BattleFx_SpawnObjects(9, 376, 2);

            tbl = Resource_GetTableEntry((s32)&ResourceId_VioletCometSheet);
            Iwram_CopyWords((void *)0x05000000, tbl, 0x80);
            tbl += 0x80;
            Resource_DecodeType01(tbl, (u8 *)runtime + 0x3600);

            index = 0;
            do {
                ((struct EffectStep *)Ram_MapCellBuffer)[index].variant = 0;
                index++;
            } while (index != 512);

            for (index = 0; index != runtime->effect->count; index++) {
                context = GetBattleObjectSlotFar(
                    runtime->effect->actors[index]);
                object = (struct MotionObject *)context->object;
                object->y = ((Random16() & 15) + 8) << 16;
            }

            gProjection.center_x = 72;

            index = 0;
            do {
                speed = Random16() & 127;
                angle = (Random16() & 0x7fff) + 0x9fff;
                runtime->particles[index].x = Trig_Sin(angle) * speed;
                runtime->particles[index].y = Trig_Cos(angle) * speed;
                runtime->particles[index].z =
                    ((u32)Random16() % 200 - 100) << 16;
                runtime->particles[index].variant = 0;
                index++;
            } while (index != 32);

            for (index = 0; index != runtime->effect->count; index++) {
                radius[index] = 16;
                spin[index] = 6000;
            }
        }

        if (frame > 159) {
            scale = Data_080eda78;
            size = 16;
            if (frame - 160 <= 64)
                size = 32;

            wave.x -= (Trig_Sin((frame - 160) << 7) * 32) >> 6;
            wave.y += (Trig_Sin((frame - 160) << 9) * size) >> 6;
            wave.x += drift.x;
            wave.y += drift.y;
            drift.x /= 2;
            drift.y /= 2;

            scale.x = 0x10000;
            scale.y = 0x10000;
            place.unknown_0c = 0;
            index = 0;
            do {
                place.x = (Data_080ee15a[index] << 16) + wave.x + 0x280000;
                place.y = 0xff0000 + index * 0x400000;
                place.z =
                    ((Data_080ee163[index] + index * 64) << 16) + wave.y;
                Object_ApplyProjectedPlacementFar(runtime->objects[index], &place, &scale, 0);
                index++;
            } while (index != 9);

            if (frame > 255) {
                s32 threshold;

                shake.z = 0x1000000;
                alive = 0;
                shake.x = 0;
                shake.y = 0;
                Render_ResetTransformState();
                SceneTransform_ApplyPosition(&shake);

                if (frame <= 329) {
                    index = 0;
                    do {
                        s32 xx;
                        s32 yy;
                        s32 zz;

                        point = &runtime->particles[index];
                        xx = (point->x >> 8) * (point->x >> 8);
                        yy = (point->y >> 8) * (point->y >> 8);
                        zz = (point->z >> 8) * (point->z >> 8);
                        dist = Iwram_Sqrt(xx + yy + zz) >> 9;
                        if (dist != 0) {
                            EffectPosition_ApplyBaseAndYOffset(&point->x, &screen);
                            screen.x =
                                (screen.x >> 17) + (wave.x >> 17) + 32;
                            screen.y = (screen.y >> 16)
                                + (wave.y >> 16) - 4;
                            screen.depth >>= 16;
                            if (screen.depth <= 169)
                                screen.depth = 170;
                            if (screen.depth > 350)
                                screen.depth = 350;
                            span = 6 - (screen.depth - 170) / 36;
                            draw.rect_alt(
                                draw_destination,
                                graphics + ParticleStreams_CellOffsets[span - 1],
                                screen.x - span / 2,
                                screen.y - span,
                                span,
                                span * 2);
                            point->x -=
                                point->x / dist;
                            point->y -=
                                point->y / dist;
                            point->z -=
                                point->z / dist;
                        } else {
                            alive++;
                        }
                        index++;
                    } while (index != 32);

                    if (alive > 0) {
                        span = alive / 10 + 1;
                        draw.rect_alt(
                            draw_destination,
                            graphics + ParticleStreams_CellOffsets[span - 1],
                            (wave.x >> 17) - span / 2 + 32,
                            (wave.y >> 16) - span - 4,
                            span,
                            span * 2);
                    }
                }

                slot = 0;
                do {
                    if (frame == Data_080ee16c[slot]) {
                        s32 speed;
                        s32 angle;

                        index = 0;
                        do {
                            speed = (Random16() & 127) + 16;
                            angle = Random16() & 0xffff;
                            spark = &((struct EffectStep *)Ram_MapCellBuffer)[slot * 128 + index];
                            spark->velocity_x = (Trig_Sin(angle) * speed) >> 6;
                            spark->velocity_y = -(Trig_Cos(angle) * speed) >> 6;
                            spark->velocity_y = ((Random16() & 255) - 128) << 10;
                            spark->x = 0;
                            spark->y = 0;
                            spark->z = 0;
                            spark->variant = (Random16() & 15) + 64;
                            index++;
                        } while (index != 96);
                    }

                    if (frame >= Data_080ee16c[0]) {
                        spark = ((struct EffectStep *)Ram_MapCellBuffer);
                        index = 0;
                        do {
                            if (spark->variant > 0) {
                                EffectPosition_ApplyBaseAndYOffset(&spark->x, &screen);
                                screen.x = (screen.x >> 17) + 32;
                                screen.y = (screen.y >> 16) + 56;
                                screen.depth >>= 16;
                                if (screen.depth <= 169)
                                    screen.depth = 170;
                                if (screen.depth > 350)
                                    screen.depth = 350;
                                span =
                                    3 - (screen.depth - 170) / 90;
                                draw.rect(
                                    draw_destination,
                                    graphics + ParticleStreams_CellOffsets[span - 1],
                                    screen.x - span / 2,
                                    screen.y - span,
                                    span,
                                    span * 2);
                                spark->x += spark->velocity_x;
                                spark->y += spark->velocity_y;
                                spark->z += spark->velocity_y;
                                spark->variant--;
                            }
                            index++;
                            spark++;
                        } while (index != 512);
                    }
                    slot++;
                } while (slot != 4);

                index = 0;
                mark = &runtime->particles[60];
                do {
                    threshold = index * 12 + 330;
                    if (frame == threshold) {
                        mark->x = (wave.x >> 17) + 32;
                        mark->y = (wave.y >> 16) - 4;
                        mark->velocity_x = (wave.x >> 17) + 32;
                        mark->velocity_y = (wave.y >> 16) - 4;

                        slot = 0;
                        do {
                            runtime->particles[slot + 32].variant = 4;
                            slot++;
                        } while (slot != 28);

                        drift.x = 0x80000;
                        drift.y = -0x20000;
                        runtime->shake_frames = 8;
                    }

                    if (frame >= threshold) {
                        s32 size;

                        size = (frame - threshold) / 2;
                        if (size > 2)
                            size = 2;
                        draw.rect_alt(
                            draw_destination,
                            (u8 *)runtime + Data_080ee17e[size],
                            mark->x,
                            mark->y - Data_080ee17a[size],
                            Data_080ee174[size],
                            Data_080ee177[size]);
                        mark->x -= 8;
                        mark->y += 2;

                        if (frame < threshold + 8) {
                            SceneTransform_ApplyRoll(-2048);
                            SceneTransform_ApplyYaw(-4096);

                            slot = 0;
                            spark = &runtime->particles[32];
                            do {
                                work.x = 0;
                                work.y = spark->variant
                                    * Trig_Cos(slot * 2340);
                                work.z = spark->variant
                                    * Trig_Sin(slot * 2340);
                                spark->variant += 2;
                                EffectPosition_ApplyBaseAndYOffset(&work.x, &screen);
                                screen.x =
                                    (screen.x >> 17) + runtime->particles[index + 60].velocity_x;
                                screen.y = (screen.y >> 16)
                                    + runtime->particles[index + 60].velocity_y;
                                screen.depth >>= 16;
                                if (screen.depth <= 169)
                                    screen.depth = 170;
                                if (screen.depth > 350)
                                    screen.depth = 350;
                                span = 3
                                    - (screen.depth - 170) / 90;
                                draw.rect(
                                    draw_destination,
                                    graphics + ParticleStreams_CellOffsets[span - 1],
                                    screen.x - span / 2,
                                    screen.y - span,
                                    span,
                                    span * 2);
                                slot++;
                                spark++;
                            } while (slot != 28);
                        }
                    }
                    index++;
                    mark++;
                } while (index != 4);
            }
        }

        ObjectGroup_TickMemberTimers();
        runtime->transfer_pending = 1;
        WaitFrames(1);
        frame++;
    } while (frame != 400);

    BattleEventRuntime_BeginPhaseFar(134);

    for (index = 0; index != runtime->effect->count; index++) {
        struct MotionObject *obj;

        context = GetBattleObjectSlotFar(runtime->effect->actors[index]);
        obj = (struct MotionObject *)context->object;
        obj->x = home[index].x;
        obj->z = home[index].y;
        obj->angle = base_angle[index];
    }

    gProjection.center_x = 120;

    BattleEffect_SetupBlendedDisplay();

    index = 0;
    do {
        ResourceObject_ReleaseFar((struct ResourceObjectWork *)runtime->objects[index]);
        index++;
    } while (index != 9);

    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
