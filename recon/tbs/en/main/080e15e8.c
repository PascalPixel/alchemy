/* 080e15e8 BattleFx_InitializeMode12: draft, not matching.
   listing 1640, draft 1638, common 1510, differ 258 (frame 336 and
   every frame-loop slot as native, sp+24 included).
   0x1fe0000 for the camera goes through an inline DivQ16 (FAKEMATCH,
   as RESOLVE.C's matched call): a direct RatioMulQ14 call lets CSE
   share one 0x1fe0000 pseudo across the call, which local-alloc puts
   in r5; the native builds it twice.
   The ramp smoke calls' 32 (native: r6 for both calls): the second
   loop pass hoists the ramp loop's 32 out of the frame loop with the
   smoke's 32 matched to it (threshold 12 after the constant 2's move,
   12 * savings 2 * life 19 = 456 >= 448 insns), so it takes no
   register and is rebuilt at each use.
   n = count * 2 - i * 48 - 256 in one expression gives the native's
   sp+24: with frame * 4, GCSE's PRE gives the three-streams preheader's
   frame << 2 the same reaching register as the smoke block's second
   frame << 2 (copied after its first), so that register is set twice
   and is no giv of the frame loop; count << 1 is an expression of its
   own, PRE hoists it once, and the frame loop reduces it to sp+24 and
   the inner giv to count * 2 - i * 48, adding -256 inside, as native.
   Remaining (268): r8 and sl swapped in the ring, three-streams and
   ring-burst loops; 0x1fe0000 kept in r5 across the RatioMulQ14 call
   (native rebuilds it); the ramp smoke calls keep y in r6 and rebuild
   32 (native: y in r8, 32 in r6 for both); reload registers of the
   frame-loop latch; r5/r7 swapped in the particle draw loop.
   The ramp's two smoke calls take (frame - 151) * 20 - 12 for y, not
   h - 56: the native reloads the h giv from its slot for it (h - 56
   kept h in r8 through the ramp block).
   The native's frame-loop slots and how the draft reaches them:
   - sp+96, &scale held through the frame loop: matched by writing
     each scale branch as scale[0] = value; scale[1] = scale[0];
     scale[2] = scale[0]. Expanded that way, each branch sets its
     address before computing the value, so the two matching
     movables live long enough (threshold 15 * savings 2 * lifetime
     against the 484-insn loop) and loop.c hoists &scale to the
     preheader, where it spills just below count as in the native.
     The same 482 comes from scale[0] = s = value or a reversed chain
     scale[2] = scale[1] = scale[0] = value; a forward chain gives 490,
     an s local stored three times 701, a user sc = scale pointer 601
     (it spills above count, not below);
   - sp+24, frame * 4 as its own reduced giv of the frame loop, read
     by the three-streams preheader: not matched. n = frame * 4 - 256;
     n -= i * 48 (404) makes frame * 4 an invariant of the
     three-streams loop with a fourth inner slot (sp+44) instead.
     A reach = frame * 4 local set at the loop top gives the native's
     outer giv at sp+24 (482), but as a user variable it is not
     replaceable and keeps a copy in r8 through the loop, so the
     native's giv is likely not a user variable. In one expression,
     n = frame * 4 - i * 48 - 256, GCSE's PRE hoists frame << 2 to the
     three-streams preheader with lifetime 4 and loop.c rejects it
     ("not worth while, 170 vs 479": lifetime * benefit * 17 must reach
     the frame loop's insn count);
   - sp+60, sx in the smoke burst: matched by writing work->sheet +
     0x6000 in each smoke call, no smoke pointer (812 to 670). With the
     pointer, loop.c hoisted the constant 12 out of the frame loop:
     its movable threshold is 15, less 3 per earlier move, and 12 lived
     50 insns against 24's 49 in a 653-insn loop (15 * 50 >= 653), so
     12 took no register and sx took fp.
   Trials:
   - unscored first draft: differ 938 (frame 332).
   - pts[].y as (y >> 16) + 60, not a halfword cast: 929.
   - size = 5 after the three scale stores: 923.
   - n = frame * 4 - i * 48 - 256 without the reach local: 916.
   - n = count * 2 - i * 48 - 256: 916, no change.
   - ring store as sheet[offset + rings + j]: 914 (native adds offset to
     work, then rings); (sheet + rings)[offset + j]: 976;
     sheet[offset + j + rings]: 916.
   - scale used directly, no sc pointer: 844.
   - ring cell address as sheet + (lvl * 0x302 + cell) + rings: 812 (the
     native adds lvl * 0x302 to the cell offset, then work, then rings,
     inside the loop); (rings + lvl * 0x302 + cell): 832;
     &sheet[lvl * 0x302 + cell + rings]: 816; rings + (lvl * 0x302 +
     cell): 828.
   - on 812: n from count * 2, frame << 2, (frame - i * 12) * 4 or
     frame * 4 - (i * 48 + 256): 812, no change; reach = frame * 4 before
     the points loop (on 914): 921, user var rejected as above; blit[0] =
     SLOT(46) without cache: 817; sx after oy: 858; declaration order:
     no change.
   - on 812: literal 10000 for rings in both addresses: 816 to 834;
     ring store as (sheet + offset)[rings + j], (sheet + offset +
     rings)[j] or other orders: 812 to 816; ox += 60 before the smoke
     calls: 814; smoke set before or after the first LoadWork: 812;
     ApplyPosition before the size clamp: 902; count passed to the
     palette call: 812; count clamped and looped on directly, no n:
     840; sc pointer with count set before size, after the key test, in
     the palette call or clamped directly: 881 to 912.
   - on 812, rejected: count = frame * 2 just before the palette call:
     796, but the frame shrinks to 320 and the loop top no longer stores
     count first as the native does; n = 0 between the third smoke call
     and its release (permuter): 791, a register-allocation accident
     that moves the store away from the native's.
   - permuter, 5 minutes from 812: 638, all from temporaries, swapped
     compares and casts; no single rewrite carries over.
   - on 812: smoke pointer set after the first LoadWork: 812; a w = 12
     local for the smoke widths: 812; n as (frame - 64) * 4 - i * 48,
     (frame - 64 - i * 12) * 4, frame * 4 - 256 - i * 48 or
     frame * 4 - (i * 48 + 256): 812; (frame - i * 12 - 64) << 2: 841.
   - permuter restricted to the n line and the smoke block, 6 minutes
     from 812: 670, from smoke assigned inside the first call's
     arguments; the same 670 without any smoke pointer, adopted.
   - on 670: blit[0] = SLOT(46) without cache: 675; sx = 72 + ox,
     literal 10000 for rings in the ring address, or the burst's draw
     assigned after its declaration: 670; sc = scale pointer set before
     the frame loop for the three stores and the ApplyScale call: 738,
     for the call only: 744.
   - on 670: n split as n = frame * 4 - 256; n -= i * 48: 621, and
     other two-statement forms 633 to 638, but frame * 4 becomes an
     inner-loop invariant in a fourth inner slot, not the native's
     outer giv; reach = frame * 4 at the loop top: 701 (outer giv as
     native, but in r8); reach = frame * 4 - 256 at the top with
     n = reach - i * 48: 595, but reach takes &scale's slot at sp+100;
     sc set in the frame > 128 block, before the yaw call, at the loop
     top or before the if: 668 to 684 (never hoisted: a user var used
     outside its block is not a safe movable in a bottom-tested loop);
     reach at the loop top plus sc before the loop: 601, adopted (all
     24 declaration orders of size, count, reach and sc: 601 or 613).
   - on 601: reach before size or count, sc before the 0x784 store:
     603; sc before the ramp loop: 628; reach after the palette call,
     after Render_ResetTransformState or before the points block: 745
     to 763; count passed to the palette call: 601.
   - permuter, 6 minutes on 670, restricted to the loop top, the scale
     stores and the three-streams loop: no gain.
   - on 601: scale[0] = value; scale[1] = scale[0]; scale[2] =
     scale[0] in both branches, no sc pointer: 482 (&scale hoisted
     as native); only the first branch: 697; reversed chain or
     scale[0] = s = value: 482; forward chain: 490.
   - on 482: no reach, n = frame * 4 - 256; n -= i * 48 (or n = frame
     * 4; n -= 256; n -= i * 48): 404, adopted; n = frame * 4 then
     n -= i * 48 + 256 or n = n - i * 48 - 256: 414; n = frame * 4 -
     i * 48; n -= 256 or three steps: 419; one expression, no reach,
     or count * 2: 659; reach = frame * 4 - 256: 740; reach first at
     the loop top: 484; n = reach - 256; n -= i * 48: 691.
   - on 404: ramp smoke y as (frame - 151) * 20 - 12 (or + 44 - 56):
     378, adopted; the ramp loop bound written out: 845; h assigned
     just before its loop: 428.
   - on 378: n in one expression, as (frame - 64) * 4 - i * 48,
     (frame - i * 12) * 4 - 256, (frame - i * 12 - 64) * 4 or
     frame * 2 * 2 - i * 48 - 256: 633; n = (frame - 64) * 4;
     n -= i * 48: 378; n = (frame - i * 12) * 4; n -= 256: 670;
     n = frame - i * 12 - 64; n *= 4: 660; reach = frame * 4 at the
     loop top: 454 (outer giv as native, copy in r8), with n split:
     663 to 681; no count local (n = frame * 2 in the ring block,
     from the permuter on 601): 563; ring store as
     sheet[offset + (rings + j)]: 378.
   - permuter, 5 minutes on 601 over the whole function: 422, from
     the same scale rewrite this draft carries; nothing else held.
   - on 378: n = frame * 4 - i * 48 - 256 again: 633 (inner loop as
     native, but PRE's twice-set frame << 2 register is no outer giv);
     y as h - 56 on that: 659; n = count * 2 - i * 48 - 256: 268,
     adopted.
   - on 268: the camera's RatioMulQ14 through an inline DivQ16
     (FAKEMATCH, as RESOLVE.C): 258, adopted. */
#include "CANVAS.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "PROJECT.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "FIXED_MATH.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "SYSTEM.H"

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_AdvanceScrollOnInterval(void);
void AudioCommand_PlayFar(s32);
void BattlePres_ConfigureEffectDisplay(void);
void BattleEffect_WipeCanvas(s32, s32);
void BattleBackground_LoadFar(s32, s32, s32);
void Graphics_UpdatePhasePalette(s32, s32, s32, s32);
void Camera_StoreSceneParameters(s32, s32, s32);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(s32 *);
void SceneTransform_ApplyPitch(s32);
void SceneTransform_ApplyRoll(s32);
void SceneTransform_ApplyYaw(s32);
void SceneTransform_ApplyScale(s32 *);
void BattleEffect_SetupBlendedDisplay(void);
void BattleEventRuntime_BeginPhaseFar(s32);
void BattleMotion_ApplyVariantMotionFar(s32, s32);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *, s32, s32);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);
void ObjectGroup_TickMemberTimers(void);
void Camera_ApplyShake(s32, s32);
void BattleFx_PlaceFormationObjects(s32, s32, s32);
void BattleEffect_RunImpactBurst(s32, s32, s32);

extern u8 gWorkSlot[];
extern volatile u32 gKeysRepeat;

extern s16 Mode12_Points[][2];
extern u16 ParticleStreams_CellOffsets[];
extern u8 Mode12_Animations[];
extern u8 Mode12_SmokeFlips[];
extern u16 BattleFx_GlintCellOffsets[];
extern u8 BattleFx_GlintCellWidths[];
extern u8 BattleFx_GlintCellHeights[];

struct Point {
    s32 x;
    s32 y;
    s32 z;
};

struct BlitterPair {
    DrawRectangle upper;
    DrawRectangle lower;
};

#define PARTICLES ((struct EffectStep *)(Ram_MapCellBuffer + 0xc58))
#define SLOT(n) (((DrawRectangle *)gWorkSlot)[n])

static __inline__ s32 DivQ16(s32 divisor, s32 value)
{
    /* FAKEMATCH: a direct call keeps 0x1fe0000 in r5 for the camera call; the native builds it twice. */
    return Iwram_RatioMulQ14(divisor, value);
}

void BattleFx_InitializeMode12(struct BattleEffectArgument *efx)
{
    u8 ramp[32];
    s32 vec[3];
    struct Point pts[11];
    s32 pos[3];
    s32 scale[3];
    void **slots;
    s32 *ctrl;
    struct BattleEffectWork *work;
    void *dst;
    u8 *aux;
    s32 frame;
    DrawRectangle blit[2];
    s32 size;
    s32 count;
    s32 i;
    s32 j;
    s32 n;
    struct EffectStep *p;
    DrawRectangle *cache;
    s32 rings = 0x2710;

    slots = (void **)(gWorkSlot + 44 * 4);
    ctrl = slots[0];
    work = slots[-5];
    dst = slots[-4];
    aux = slots[-3];
    work->effect = efx;
    BattleFx_BeginCanvasLayer(0x2000);
    *(u16 *)0x04000020 = 0x100;
    Resource_LoadAndDecompress((s32)&ResourceId_LightFanSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_LightFanSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesC, work->sheet + 0x1800, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, aux, 0, 0);

    {
    s32 offset;
    for (i = 0, offset = 0; i != 8; i++) {
        for (j = 0; j != 0x302; j++) {
            s32 v = aux[j];

            if (v > 64 - i * 7) {
                v = 64 - i * 7;
            }
            if (v < 0) {
                v = 0;
            }
            work->sheet[offset + rings + j] = v;
        }
        offset += j;
    }
    }

    BattlePres_ConfigureEffectDisplay();
    *(u16 *)0x05000000 = 0;
    *(u16 *)0x05000002 = 0;
    *(u16 *)0x0400000c = 0x2784;
    work->scroll_timer = 0;
    work->scroll_interval = 2;
    work->scroll_step_x = 1;
    work->scroll_step_y = 0;
    ctrl[4] = 1;
    Scheduler_AddOrUpdateCallback((s32)BattleFx_AdvanceScrollOnInterval, 0x480);
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    work->transfer_mode = 0;
    BattleEffect_WipeCanvas(0, 0);
    BattleBackground_LoadFar(1, (s32)&ResourceId_DuskCloudsBackdrop, 0);
    gProjection.center_y = 240;
    BattleEffect_WipeCanvas(0, 1);
    *(u16 *)0x04000020 = 0x80;
    BattleEffect_LoadWork(46, 7, 7, 3, 3);
    cache = (DrawRectangle *)gWorkSlot;
    blit[0] = cache[46];
    *(u16 *)0x04000000 = 0x7741;
    *(u16 *)0x04000020 = 0x80;
    *(u16 *)0x04000052 = 0x100f;
    *(u16 *)0x04000050 = 0x3f44;
    work->transfer_mode = 2;
    work->transfer_value = 75;

    for (i = 0; i != 16; i++) {
        ramp[i] = i * 8 + 3;
        ramp[31 - i] = i * 8 + 3;
        if (ramp[i] > 63) {
            ramp[i] = 63;
        }
        if (ramp[31 - i] > 63) {
            ramp[31 - i] = 63;
        }
    }
    *(u16 *)0x0400000c = 0x784;

    for (frame = 0; frame != 170; frame++) {
        size = 2;
        count = frame * 2;
        if (frame == 16) {
            AudioCommand_PlayFar(140);
        }
        if (frame == 132) {
            AudioCommand_PlayFar(131);
        }
        if (frame == 151) {
            AudioCommand_PlayFar(145);
        }
        if (gKeysRepeat & (KEY_A | KEY_B)) {
            break;
        }
        Graphics_UpdatePhasePalette(frame * 2, 0xaaab, 0x5555, 0);
        if (frame == 150) {
            work->transfer_mode = 1;
            work->transfer_value = 0x1a1a1a1a;
        } else {
            work->transfer_mode = size;
            work->transfer_value = 75;
        }
        Camera_StoreSceneParameters(0x1fe0000, DivQ16(0x1fe0000, 0xc000), 0x7fff0000);
        Render_ResetTransformState();
        if (frame > 128) {
            s32 a = frame - 128;
            if (a > 22) {
                a = 20;
            }
            pos[0] = 0;
            pos[1] = -a << 17;
            pos[2] = 0x2000000;
            size = (a >> 2) + 2;
            if (size > 8) {
                size = 8;
            }
            SceneTransform_ApplyPosition(pos);
            SceneTransform_ApplyPitch(-0x1000);
            SceneTransform_ApplyRoll(0x8000);
            SceneTransform_ApplyPitch(0x8000);
            SceneTransform_ApplyYaw(a << 12);
            if (frame > 150) {
                scale[0] = frame * 0x1800 - 0xd9400;
                scale[1] = scale[0];
                scale[2] = scale[0];
                size = 5;
            } else {
                scale[0] = 0x10000 - a * 3 * 0x400;
                scale[1] = scale[0];
                scale[2] = scale[0];
            }
            SceneTransform_ApplyScale(scale);
        } else {
            pos[0] = 0;
            pos[1] = 0;
            pos[2] = 0x1000000;
            SceneTransform_ApplyPosition(pos);
            SceneTransform_ApplyPitch(-0x1000);
            SceneTransform_ApplyRoll(frame << 8);
            SceneTransform_ApplyPitch(frame << 8);
        }

        if (frame <= 149) {
            for (i = 0; i != 6; i++) {
                vec[0] = (Mode12_Points[i][0] - 96) << 16;
                vec[1] = 0;
                vec[2] = (Mode12_Points[i][1] - 96) << 16;
                EffectPosition_ApplyBaseAndYOffset(vec, (struct EffectPosition *)&pts[i]);
                pts[i].x = (pts[i].x >> 17) + 64;
                pts[i].y = (pts[i].y >> 16) + 60;
            }
            for (i = 0; i != 3; i++) {
                n = count * 2 - i * 48 - 256;
                if (n > 48) {
                    n = 48;
                }
                if (n >= 0) {
                    for (j = 0; j != n; j++) {
                        s32 x = pts[i * 2].x + Iwram_MulQ16(j * (pts[i * 2 + 1].x - pts[i * 2].x), 0x555);
                        s32 y = pts[i * 2].y + Iwram_MulQ16(j * (pts[i * 2 + 1].y - pts[i * 2].y), 0x555);
                        blit[0](dst, aux + ParticleStreams_CellOffsets[size - 1], x - size / 2, y - size, size, size * 2);
                    }
                }
            }
        }

        if (frame <= 179) {
            DrawRectangle blit47;
            s32 lvl = 0;

            if (frame > 155) {
                lvl = frame - 156;
            }
            if (lvl > 7) {
                lvl = 7;
            }
            if (frame <= 139) {
                BattleEffect_LoadWork(47, 7, 7, 3, 3);
                blit47 = SLOT(47);
            } else {
                BattleEffect_LoadWork(47, 7, 7, 3, 2);
                blit47 = SLOT(47);
            }
            n = count;
            if (n > 128) {
                n = 128;
            }
            for (i = 0; i != n; i++) {
                s32 s;
                vec[0] = Trig_Sin(i << 9) * 96;
                vec[2] = -(Trig_Cos(i << 9) * 96);
                EffectPosition_ApplyBaseAndYOffset(vec, (struct EffectPosition *)&pts[0]);
                pts[0].x = (pts[0].x >> 17) + 64;
                pts[0].y = (pts[0].y >> 16) + 60;
                s = size + 1;
                blit47(dst, work->sheet + (lvl * 0x302 + ParticleStreams_CellOffsets[s - 1]) + rings,
                    pts[0].x - s / 2, pts[0].y - s, s, s * 2);
            }
            Runtime_ReleaseHeapBlock(47);
        }

        if ((u32)(frame - 151) <= 16) {
            s32 w = 0;
            s32 h = (frame - 151) * 20 + 44;
            if (frame > 151) {
                w = frame - 152;
            }
            if (w > 15) {
                w = 15;
            }
            for (i = 1; i != 10; i++) {
                if (w + i <= 15) {
                    blit[0](dst, &ramp[w + i], w + 48 + i, 16 - i, 32 - w * 2 - i * 2, 1);
                }
            }
            for (i = 0; i != h; i++) {
                blit[0](dst, ramp + w, w + 48, i + 16, 32 - w * 2, 1);
            }
            BattleEffect_LoadWork(47, 7, 7, 3, 2);
            SLOT(47)(dst, work, 32, (frame - 151) * 20 - 12, 32, 96);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
            SLOT(47)(dst, work, 64, (frame - 151) * 20 - 12, 32, 96);
            Runtime_ReleaseHeapBlock(47);
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    ctrl[4] = 0;
    Scheduler_RemoveCallback((u32)BattleFx_AdvanceScrollOnInterval);
    BattleEffect_SetupBlendedDisplay();
    BattleFx_SelectLivingTargets(work->effect);
    BattleFx_SpawnObjects(9, 0x173, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, Ram_MapCellBuffer, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_LightningPillarSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_VioletMoteSheet, work->sheet + 0x6000, 1, 0);
    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_CrystalSheet), 128);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesB, aux, 0, 0);
    *(u16 *)0x04000052 = 0x1010;
    {
    s32 cam_x;
    s32 cam_y;
    s32 vel_x;
    s32 vel_y;
    s32 cx;
    s32 cy;

    cam_x = 0x500000;
    cam_y = 0x400000;
    vel_x = 0;
    vel_y = 0;
    for (i = 0; i != 910; i++) {
        PARTICLES[i].variant = 0;
    }
    for (i = 0; i != 16; i++) {
        work->particles[i].x = (Random16() & 31) - 16;
        work->particles[i].y = Random16() & 63;
        work->particles[i].variant = -(Random16() & 15);
    }
    work->transfer_mode = 2;
    work->transfer_value = 50;
    AudioCommand_PlayFar(145);
    cx = 60;
    cy = 112;

    for (frame = 0; frame != 192; frame++) {
        s32 d;
        if (frame == 20) {
            Resource_LoadAndDecompress((s32)&ResourceId_VioletLightningSheetB, work, 1, 0);
        }
        d = frame - 80;
        if ((u32)d <= 59 && !(frame & 7)) {
            AudioCommand_PlayFar(134);
        }
        if (frame == 140) {
            BattleEventRuntime_BeginPhaseFar(134);
        }
        for (j = 0; j != 1; j++) {
            s32 start = j * 128 + 8;
            if (frame >= start && frame < j * 128 + 17) {
                if (frame >= j * 128 + 9 && frame < j * 128 + 12) {
                    blit[0](dst, work->sheet, 36, 0, 48, 112);
                }
                if (frame >= j * 128 + 12 && frame < start + 8) {
                    blit[0](dst, work->sheet + 0x1500, 36, 0, 48, 112);
                }
                if (frame == start + 2) {
                    n = 0;
                    for (i = 0, p = PARTICLES; i != 910; i++, p++) {
                        if (p->variant == 0) {
                            s32 speed = 0x3ff;
                            s32 angle;
                            speed &= Random16();
                            angle = (Random16() & 0x7fff) - 0x4000;
                            p->x = cx << 16;
                            p->y = cy << 16;
                            speed += 32;
                            p->velocity_x = (Trig_Sin(angle) * speed) >> 7;
                            p->velocity_y = -((Trig_Cos(angle) * speed) << 1) >> 7;
                            p->variant = (Random16() & 7) + 32;
                            if (++n == 512) {
                                break;
                            }
                        }
                    }
                    work->shake_frames = 8;
                }
            }
        }
        if (frame == 10) {
            for (i = 0; i != work->effect->count; i++) {
                ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 8);
                BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
            }
        }
        if ((u32)d <= 63) {
            s32 ox;
            s32 oy;
            s32 sx;
            for (i = 0; i != 6; i++) {
                if (frame == i * 8 + 80) {
                    Iwram_FillWords(dst, 0x4000, 0x10101010);
                }
            }
            for (i = 0; i != 30; i++) {
                if (frame >= i * 2 + 80) {
                    if (frame < i * 2 + 82) {
                        s32 rx = Random16() & 3;
                        s32 ry = Random16() & 3;
                        BattleEffect_LoadWork(47, 7, 7, 3, 2);
                        SLOT(47)(dst, work->sheet + i % 3 * 0x1440, rx - 3, ry + 32, 72, 72);
                        Runtime_ReleaseHeapBlock(47);
                    }
                    if (frame == i * 2 + 80) {
                        n = 0;
                        for (j = 0, p = PARTICLES; j != 910; j++, p++) {
                            if (p->variant == 0) {
                                s32 speed = 0x3ff;
                                s32 angle;
                                speed &= Random16();
                                angle = Random16() & 0xffff;
                                p->x = 0x100000;
                                p->y = 0x500000;
                                speed += 32;
                                p->velocity_x = (Trig_Sin(angle) * speed) >> 7;
                                p->velocity_y = -((Trig_Cos(angle) * speed) << 1) >> 7;
                                p->variant = (Random16() & 7) + 32;
                                if (++n == 16) {
                                    break;
                                }
                            }
                        }
                        work->shake_frames = 8;
                        for (j = 0; j != work->effect->count; j++) {
                            ObjectGroup_UpdateMembers(work->effect->actors[j],
                                Mode12_Animations[Random16() & 3], 5, j, 4);
                            BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 4);
                        }
                    }
                }
            }
            {
                s32 r = 7;
                s32 angle;
                r &= Random16();
                angle = Random16() & 0xffff;
                ox = (Trig_Sin(angle) * (r + 8)) >> 16;
                sx = ox + 72;
                oy = 32 - ((Trig_Cos(angle) * (r + 8)) >> 16);
            }
            BattleEffect_LoadWork(47, 7, 7, 3, 2);
            SLOT(47)(dst, work->sheet + 0x6000, ox + 60, oy - 24, 12, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
            SLOT(47)(dst, work->sheet + 0x6000, sx, oy - 24, 12, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 11, 2);
            SLOT(47)(dst, work->sheet + 0x6000, ox + 60, oy, 12, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 15, 2);
            SLOT(47)(dst, work->sheet + 0x6000, sx, oy, 12, 24);
            Runtime_ReleaseHeapBlock(47);
            n = 0;
            for (i = 0, p = PARTICLES; i != 910; i++, p++) {
                if (p->variant == 0) {
                    s32 speed = 63;
                    s32 angle;
                    speed &= Random16();
                    angle = Random16() & 0xffff;
                    p->x = sx << 16;
                    p->y = oy << 16;
                    speed += 64;
                    p->velocity_x = (Trig_Sin(angle) * speed) >> 7;
                    p->velocity_y = -((Trig_Cos(angle) * speed) << 1) >> 7;
                    p->variant = (Random16() & 7) + 16;
                    if (++n == 4) {
                        break;
                    }
                }
            }
        }

        BattleEffect_LoadWork(47, 7, 7, 15, 2);
        {
            DrawRectangle draw = SLOT(47);
            for (i = 0, p = PARTICLES; i != 910; i++, p++) {
                if (p->variant > 0) {
                    p->variant--;
                    EffectStep_AdvanceWithGravity2D(p, 60, 0);
                    if (p->y > 0x780000) {
                        p->velocity_y = -p->velocity_y / 2;
                    } else if ((u32)p->x <= 0x7effff && p->y >= 0) {
                        s32 x = p->x >> 16;
                        s32 y = p->y >> 16;
                        s32 s = p->variant / 16 + 1;
                        draw(dst, aux + ParticleStreams_CellOffsets[s - 1], x - s / 2, y - s, s, s * 2);
                    }
                }
            }
        }
        Runtime_ReleaseHeapBlock(47);

        if (frame > 15) {
            if (frame == 32) {
                vel_x = 0x40000;
                vel_y = -0x4000;
            }
            if (frame > 31) {
                cam_x += vel_x;
                cam_y += vel_y;
                vel_x = vel_x * 60 / 64;
                vel_y = vel_y * 60 / 64;
            }
            BattleFx_PlaceFormationObjects(0, cam_x, cam_y);
        }
        if ((u32)(frame - 16) <= 127) {
            s32 base = cam_x >> 17;
            for (i = 0; i != 3; i++) {
                s32 k = i & 3;
                s32 angle = Random16() & 0xffff;
                s32 x = ((Trig_Sin(angle) << 3) >> 16) + base - (BattleFx_GlintCellWidths[k] >> 1);
                s32 y = ((Trig_Cos(angle) * 40) >> 16) - (BattleFx_GlintCellHeights[k] >> 1);
                BattleEffect_LoadWork(47, 7, 7, 3 | Mode12_SmokeFlips[Random16() & 3], 2);
                SLOT(47)(dst, Ram_MapCellBuffer + BattleFx_GlintCellOffsets[k], x, y + 56,
                    BattleFx_GlintCellWidths[k], BattleFx_GlintCellHeights[k]);
                Runtime_ReleaseHeapBlock(47);
            }
        }
        if (frame <= 31) {
            Camera_ApplyShake(8, 16);
        } else {
            Camera_ApplyShake(4, 4);
        }
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Runtime_ReleaseHeapBlock(46);
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleEffect_RunImpactBurst(0, cam_x, cam_y);
    }
    for (i = 0; i != 9; i++) {
        ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
    }
    BattleFx_EndCanvasLayer();
}
