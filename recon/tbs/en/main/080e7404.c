/* Stubborn (2026-10-06, sun100 queue-2 rescore on staging 2fb23197f0): parent differ 322 (listing 1532, draft 1634, common 1422), above the ~50 first-try bar; AddFlame/AddMote stay locked. Not first-try; parked. */
/* 080e7404 BattleEffect_RunParticleStreams: differ 2 of 1632 (draft 1632, common 1631), session 3. The loop top and the palette now match through fixed registers (count r2, work's copy r0, quarter r3, level r2) and a non-volatile asm through base that reads count. Only left: direction = side == 1 ? mode : -1 folds mode to 1 (movs r4, #1 for ldr r4, [sp, #76]); cse1 records mode == 1 at the if and the reference keeps the load. Earlier state (differ 38): heat now sits in r4 with its sp+8 caller-save (FAKEMATCH hold/clobber around the 141 call), the second loop's size is (frame + 0x1d0) << 8 so every stack slot matches, and an asm read of count after heat and line keeps the work->frame load apart from the division temp, which also fixes the reload rotation through the line loops and the instruction count. Left: at the first effect loop's top the reference holds work in r0, the 0x778c offset in r1, count in r2 and the temp in r3; ours has work r1, offset r0, count r3, temp r2. The palette's level is r3 instead of r2 (a swap of the same kind), and direction = mode still folds to 1 (movs r4, #1 for ldr r4, [sp, #76]).
 * Stubborn (2026-10-06): parent remains differ 222 after prior trials; not first-try. Nested AddFlame (080e7338) and AddMote (080e73a0) match byte-exact as the draft's leading 100 instructions (alchemy diff THIS 080e7338,080e73a0 → common 100 of 100) but adopt only with this parent (W notes). Score+parked from sun100 queue-3. */
/* Trials: const/register heat, outer heat, declaration order and a sheet-field
 * pointer leave differ 222. Keeping the undivided frame until palette use gives
 * 231. Moving heat before angle gives 606. Forcing r4 omits its required call
 * spill; clobber barriers did not improve the draft and were removed. A scoped
 * r4 quotient copied back to an ordinary local gives 221 with an extra move.
 * Reusing size/x/y for heat gives 573/681/223; initializing direction before
 * its branch gives 445. Mode read/write barriers with the second loop's
 * factored size give 414 (register) or 513 (memory). All were removed.
 * 2026-10-06 rescore on sun100 51bbf7161e: still differ 222 (listing 1632,
 * draft 1634, common 1522). break for goto skipped 569; size from frame << 8
 * without angle 574; second loop size (frame + 0x1d0) << 8 alone 403; heat
 * loaded whole then heat /= 4 227; a count local divided into heat 222. The
 * reference also copies the loaded work->frame (r2) to the division temp (r3)
 * and computes the second loop's grow from fp without a giv; ours folds both.
 * 2026-10-06 session (sun100 b666ade248), RTL: heat is pseudo 393 (5 refs, 50
 * insns, 1 call, LO_REGS), 89th of 115 in global's order; line (402) has r5,
 * and size (407, r6) and x (408, r7) never overlap heat, so it takes r6. The
 * native r4 + sp+8 is caller-save, which global.c picks only when r5-r7 are all
 * unavailable to heat; nothing visible in the native holds r6/r7 there, so look
 * for a pseudo live only from the loop top to the palette loop, or a reload
 * eviction. The frame size is equal because our second-loop grow giv (sp+16)
 * fills heat's slot. Combine folds the loaded frame (400) into the division
 * temp (401); the native keeps 400, so 400 must stay live past the copy.
 * Trials: volatile read of work->frame 222 (still folded); asm clobber of
 * r1/r6/r7 at the loop top or before the palette loop 222, heat moves to r7 and
 * its division vanishes; frame == 0 test as work->frame == 0 206 but compares
 * the field where the native compares fp; direction after the join as
 * mode == 1 && side == 1 ? mode : -1 247, still folds. All removed.
 * 2026-10-06 session 2 (on f96ab443dd), still 222. Most of the first loop's
 * other lines follow from heat: reload1.c hands out reload registers round
 * robin, and the native's run r0, r1, r2, r3, r0, r1 from the frame > 16 test
 * on, skipping r4 while heat holds it; ours runs r3, r4, r0, r1, r2, r4. The
 * first zero loop's shared movs r3, #0 is the same effect: loop.c hoists an
 * HImode 0 (a pool load) in both builds, and the native's reload for i = 0
 * lands on r3, the hoisted 0's register, so reload_cse drops the pool load;
 * ours lands on r1. global.c (lines 500-515) drops every hard-register
 * conflict with r7, the Thumb frame pointer, so an asm clobber cannot keep
 * heat out of r7: heat stays in r7 and flow2 deletes its division as dead
 * (wrong code). To reach caller-save r4, heat needs pseudos, not clobbers, in
 * r6 and r7 across its range. reload's find_reg takes a free register before
 * it spills, and r0-r4 are mostly free there, so a reload eviction of heat is
 * unlikely. Trials: do { AudioCommand_PlayFar(145); } while (0) 475 (the
 * loop note splits cse, but loop.c and cse2 reshape the branch and mode
 * still folds); u8 mode in header and draft 224 (adds lsls/lsrs, still
 * folds); asm clobber of r6/r7 before frame == 0 222 (division deleted);
 * work->frame == 0 206 (heat still r6, only the rotation moves); first zero
 * loop storing x = 0 655. All removed. Kept: an asm-defined pseudo held
 * across the 141 call (it takes r7) plus an r6 clobber puts heat in r4 with
 * the sp+8 save, as in the reference (418 alone, its grow giv slot still
 * shifting the frame); with the second loop's size as (frame + 0x1d0) << 8
 * the frame matches: 89. Also tried: count = work->frame passed through an
 * asm "+r" before the division, 89, the load still folds; an asm read of
 * count after the division with line computed after the asm 54; the same
 * read before the division 57; line in its declaration before the read 38
 * (kept).
 * 2026-10-06 session 3 (on 5d63ee8208), 38 -> 2. Palette: local-alloc ties
 * level to the quotient (both local, the quotient dies in the add), so level
 * took r3 and red r2; quotient fixed r3 and level fixed r2 gives 20 (level
 * alone 30, quotient alone 38). Loop top: as a hard register count leaves the
 * division temp canonical in cse (make_regs_eqv puts the pseudo first), so the
 * compare and +3 use the temp as in the reference; count in r2 with the
 * volatile read before line and work copied to base before it 12; base fixed
 * r0 4 (the 0x778c reload then takes r1); the volatile read was a sched2
 * barrier after which the mode reload (true dependence, latency 1, class 3)
 * beat movs r2, #252 (anti dependence on the read of r2, class 2); passing base
 * through a non-volatile asm that also reads count 2 (kept). Also tried:
 * switch (mode) for if (mode == 1) 38, still folds; (void)&mode 100, mode
 * moves to a local slot and still folds; count r2 alone 176; count r2 without
 * any read 89 (combine merges the load into the temp); "=r"(count) : "0"(count)
 * before the division 89, after it 13; volatile read before the division 19
 * (the temp takes r2 by copy preference); level = heat + green / 4 38 (same
 * RTL); r3 clobber on the read 4. Mode fold: an asm copy of mode ("=l", "0")
 * for the ternary's arm leaves 8 (copy r2, the 0x778c offset r4; reference r4
 * and r2), with the copy fixed r4 212, with side loaded first 218, inside the
 * arm as a statement expression 134; the first mode == 1 test on an asm copy
 * of mode ("=l", "0") 260. Left: direction = mode folds to movs r4,
 * #1 in cse1 (record_jump_equiv of mode == 1); the reference reloads mode. */
/* 080e7338 AddFlame and 080e73a0 AddMote are nested functions of this one; they match (differ 0) inside this draft. Score with alchemy diff THIS 080e7338,080e73a0,080e7404 */
#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "MAP_SCROLL.H"
#include "PROJECT.H"
#include "ANIMSPR.H"
#include "TRANSFORM.H"

struct Scale {
    s32 x;
    s32 y;
};

extern void *gBattleFxWork[];
extern void *gTransitionWork[];
extern DrawRectangle gWorkSlot[];
extern struct BattleCamera *gCameraWork;
extern volatile u32 gKeysRepeat;
extern u16 ParticleStreams_CellOffsets[];
extern s16 ParticleStreams_OrbitPoints[][3];
extern s16 ParticleStreams_DropPoints[][2];
extern u16 ParticleStreams_FlashSheetOffsets[];
extern u16 ParticleStreams_FlashSizes[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_FlushPendingGraphicsTransfer(void);
void BattleFx_ArmPaletteHBlankDma(void);
void Camera_AdvanceBg2Reference(void);
void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_EndCanvasLayer(void);
void BattlePres_ConfigureEffectDisplay(void);
void BattleEffect_WipeCanvas(s32 mode, s32 layer);
void BattleFx_SelectLivingTargets(struct BattleEffectArgument *effect);
void BattleFx_SpawnObjects(s32 count, s32 kind, s32 variant);
void BattleEffect_SetupBlendedDisplay(void);
struct BattleObjectSlot *GetBattleObjectSlotFar(s32 id);
struct AnimationObject *GetBattleEffectObject(s32 kind);
void Object_InitializeMode(struct AnimationObject *object, s32 animation);
void Object_ApplyProjectedPlacementFar(void *object, s32 *position, struct Scale *scale, s32 mode);
void BattleActor_CommitPlacementFar(void);
void AudioCommand_PlayFar(s32 cue);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Palette_BrightenBgEntries(s32 red, s32 green, s32 blue);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyRoll(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Camera_ApplyShake(s32 x, s32 y);

#define REG_DMA3 ((volatile u32 *)0x040000d4)

#define FLASHES ((struct EffectStep *)Ram_MapCellBuffer)
#define EMBERS (&FLASHES[128])

#define HI(v) (((s16 *)&(v))[1])

static __inline__ void CopyWords(void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: called directly, the constant arguments are loaded before
       the routine's address and the size's lsls lands ahead of it (2 of
       1631 lines at each of the two VRAM copies); through the inline
       parameters the routine is loaded first, as in the reference. */
    Iwram_CopyWords(destination, source, size);
}

void BattleEffect_RunParticleStreams(struct BattleEffectArgument *effect, s32 mode)
{
    volatile u32 fill;
    u8 offsets[128];
    void **cache;
    void *canvas;
    DrawRectangle draw;
    s32 direction;
    struct BattleEffectWork *work;
    void *colors = (void *)0x05000000;

    void AddFlame(s32 x, s32 y, s32 velocity_x)
    {
        s32 i;

        for (i = 0; i != 16; i++) {
            struct EffectStep *flame = &work->particles[32 + i];

            if (flame->variant == -1) {
                flame->variant = 0;
                flame->x = x;
                flame->y = y;
                flame->velocity_x = velocity_x;
                break;
            }
        }
    }

    void AddMote(s32 x, s32 y)
    {
        s32 i;

        for (i = 0; i != 32; i++) {
            struct EffectStep *mote = &work->particles[i];

            if (mote->variant == -1) {
                mote->variant = 0;
                mote->x = x;
                mote->y = y;
                break;
            }
        }
    }

    s32 position[4];
    s32 vector[3];
    struct EffectPosition spot;
    struct EffectPosition *pos;
    struct EffectPosition point;
    struct Scale scale;
    s32 frame;
    s32 i;

    cache = &gBattleFxWork[1];
    canvas = cache[0];
    work = cache[-1];

    work->effect = effect;
    BattleFx_BeginCanvasLayer(0x2000);
    *(volatile u16 *)0x04000020 = 0x100;
    if (mode == 1) {
        struct MotionObject *object = GetBattleObjectSlotFar(work->effect->actor)->object;

        object->velocity_y = 0xa0000;
        object->vertical_motion_strength = 0x91eb;
        ObjectGroup_UpdateMembers(work->effect->actor, -1, 2, -1, 0);
        AudioCommand_PlayFar(145);
        direction = work->effect->side == 1 ? mode : -1;
    } else {
        direction = -1;
    }
    BattlePres_ConfigureEffectDisplay();
    *(u16 *)0x05000000 = 0;
    *(u16 *)0x05000002 = 0;
    work->transfer_mode = 0;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    BattleEffect_WipeCanvas(0, 0);
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    if (mode == 1) {
        for (i = 0; i != 2; i++) {
            struct AnimationObject *object = GetBattleEffectObject(483 + i * 0x2001);

            work->objects[i] = object;
            if (object != 0) {
                object->flags = 0;
                Object_InitializeMode(object, 2);
                ((struct AnimationObject *)work->objects[i])->part[0].priority = 3;
            }
        }
    } else {
        BattleFx_SpawnObjects(1, 381, 3);
    }
    Resource_LoadAndDecompress((s32)&ResourceId_FlameSheetB, work, 1, 1);
    if (mode == 1)
        Iwram_CopyWords(colors, Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet), 128);
    fill = 0x01010101;
    Dma_Set((void *)&fill, Ram_MapCellBuffer, 0x85002000, REG_DMA3);
    CopyWords((void *)0x06008000, Ram_MapCellBuffer, 0x7800);
    WaitFrames(1);
    *(volatile u16 *)0x04000050 = 0;
    *(volatile u16 *)0x04000020 = 0x100;
    *(volatile u16 *)0x0400000a = 0x1f80;
    *(volatile u16 *)0x0400000c = 0x2787;
    for (i = 0; i != 63; i++) {
        s32 red = (Random16() & 15) + 16;
        s32 green = (Random16() & 15) + 16;
        s32 blue = (Random16() & 15) + 16;

        ((u16 *)0x05000100)[i] = (blue << 10) | (green << 5) | red;
    }
    fill = 0;
    Dma_Set((void *)&fill, canvas, 0x85001000, REG_DMA3);
    for (i = 0; i != 256; i++) {
        s32 x = Random16() & 127;
        s32 y = Random16() & 127;
        s32 shade = (Random16() & 63) + 64;

        ((u8 *)canvas)[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = shade;
    }
    CopyWords((void *)0x06004000, canvas, 0x4000);
    gProjection.center_y = 240;
    BattleFx_SelectLivingTargets(work->effect);
    work->bg2x = 0;
    work->bg2y = 0;
    work->scroll_timer = 0;
    work->scroll_interval = 2;
    work->scroll_step_x = direction << 7;
    work->scroll_step_y = 256;
    Scheduler_AddOrUpdateCallback(Camera_AdvanceBg2Reference, 0x4ff);
    Scheduler_AddOrUpdateCallback(BattleFx_FlushPendingGraphicsTransfer, 0x480);
    for (i = 0; i != 128; i++)
        offsets[i] = Random16() & 63;
    {
        s32 limit = 0;
        s32 step = 1;
        s32 row = 0;

        do {
            limit += step / 2;
            step += 4;
            for (; row != limit; row++) {
                s32 x;

                for (x = 0; x != 256; x++) {
                    s32 y = row - offsets[x & 127];

                    if (y >= 0)
                        if (y <= 127)
                            Ram_MapCellBuffer[((y / 8 * 32 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = 0;
                }
            }
            work->transfer_pending = 1;
            WaitFrames(1);
        } while (limit <= 191);
    }
    *(volatile u16 *)0x04000050 = 0x3f42;
    *(volatile u16 *)0x04000052 = 0x1010;
    {
        s32 saved_x = gBgScroll[1].x;
        s32 saved_y = gBgScroll[1].y;
        struct BattlePresentationWork *presentation = gTransitionWork[0];

        gBgScroll[1].x = 0;
        gBgScroll[1].y = 32;
        BattleEffect_LoadWork(46, 8, 7, 3, 2);
        draw = gTransitionWork[2];
        work->transfer_mode = 3;
        work->transfer_value = (s32)(Ram_MapBlocks + 0x202);
        Scheduler_AddOrUpdateCallback(BattleFx_ArmPaletteHBlankDma, 0x4fe);
        for (i = 0; i != 64; i++)
            work->particles[i].variant = -1;
        presentation->initial_value = 1;
        work->frame = 0;
        for (frame = 0; frame != 192; frame++, work->frame++) {
            s32 angle = frame << 8;
            /* As a hard register count leaves the division temp canonical in
               cse, so the compare and +3 use the temp as in the reference. */
            /* FAKEMATCH: count fixed in r2, as in the reference. */
            register s32 count asm("r2") = work->frame;
            /* FAKEMATCH: work's copy in r0; else it and the 0x778c reload swap r0/r1. */
            register u8 *base asm("r0") = (u8 *)work;
            s32 heat = count / 4;
            u16 *line;
            s32 size;
            s32 x;
            s32 y;

            /* Reading count after the division keeps its load apart from the
               division temp (no combine); through base the asm is not
               volatile, so no sched2 barrier, and the mode reload lands after
               movs r2, #252 as in the reference (38 -> 2). */
            /* FAKEMATCH: count read and base passed through, see above. */
            asm("" : "=r"(base) : "0"(base), "r"(count));
            line = (u16 *)(base + 0x1f80);
            if (mode == 1) {
                if ((gKeysRepeat & 3) && frame > 16)
                    goto skipped;
            } else {
                if ((gKeysRepeat & 3) && frame > 4)
                    goto skipped;
            }
            {
                s32 hold;

                /* FAKEMATCH: with line in r5, hold in r7 and r6 clobbered,
                   heat takes r4 and its sp+8 caller-save as in the
                   reference (222 -> 89); global.c ignores hard conflicts
                   with r7, the frame pointer, so r7 needs a pseudo. */
                /* FAKEMATCH: hold takes r7 across the call. */
                asm("" : "=r"(hold));
                /* FAKEMATCH: r6 is clobbered across the call. */
                asm("" : : : "r6");
                if (frame == 0)
                    AudioCommand_PlayFar(141);
                /* FAKEMATCH: hold's use after the call. */
                asm("" : : "r"(hold));
            }
            for (i = 0; i != 15; i++)
                *line++ = 0;
            for (; i != 135; i++) {
                s32 green = i - 16;
                /* Local-alloc otherwise ties level to the quotient and swaps
                   level and red (16 lines). */
                /* FAKEMATCH: the quotient in r3, as in the reference. */
                register s32 quarter asm("r3") = green / 4;
                /* FAKEMATCH: level in r2, see quarter. */
                register s32 level asm("r2") = quarter + heat;
                s32 red = level - 32;

                green = level - 80;

                if (red < 0)
                    red = 0;
                if (red > 31)
                    red = 31;
                if (green < 0)
                    green = 0;
                if (green > 31)
                    green = 31;
                *line++ = (red << 10) | (green << 5) | (green >> 1);
            }
            for (; i != 160; i++)
                *line++ = 0;
            if (direction == 1)
                x = frame / 4;
            else
                x = 64 - frame / 4;
            y = 96 - frame;
            position[3] = 0;
            position[1] = 0xff0000;
            if (mode == 1) {
                size = angle + 0xa000;
                scale.x = size;
                scale.y = size;
                position[0] = (x << 16) + 0x500000;
                position[2] = (64 - y) << 16;
                Object_ApplyProjectedPlacementFar(work->objects[0], position, &scale, 0);
                Object_ApplyProjectedPlacementFar(work->objects[1], position, &scale, 0);
            } else {
                size = angle + 0x10000;
                scale.x = size;
                scale.y = size;
                position[0] = (x << 16) + 0x600000;
                position[2] = (96 - y) << 16;
                Object_ApplyProjectedPlacementFar(work->objects[0], position, &scale, 0);
            }
            for (i = 0, y = 32 - y; i != 32; i++) {
                struct EffectStep *mote = &work->particles[i];

                if (mote->variant == -1) {
                    s32 angle = (Random16() & 0x7fff) + 0x4000;

                    mote->variant = 0;
                    mote->x = ((x + 96) << 16) + Trig_Sin(angle) * 30 / 65536 * size;
                    mote->y = (y << 16) - Trig_Cos(angle) * 30 / 65536 * size;
                    break;
                }
            }
            point.x = 0;
            point.y = 0;
            point.depth = 0x2000000;
            Render_ResetTransformState();
            SceneTransform_ApplyPosition(&point.x);
            SceneTransform_ApplyRoll(0x800);
            SceneTransform_ApplyYaw(frame << 8);
            for (i = 0; i != 7; i++) {
                vector[0] = (ParticleStreams_OrbitPoints[i][0] / 2) << 16;
                vector[1] = (ParticleStreams_OrbitPoints[i][1] + frame) << 16;
                vector[2] = (ParticleStreams_OrbitPoints[i][2] / 2) << 16;
                EffectPosition_ApplyBaseAndYOffset(vector, pos = &spot);
                pos->x = HI(pos->x) + 128;
                pos->y = HI(pos->y) + 60;
                draw(Ram_MapCellBuffer, (u8 *)work + 0x1f40, pos->x - 4, pos->y - 4, 8, 8);
            }
            if (direction == 1)
                x = frame / 4 - 16;
            else
                x = 16 - frame / 4;
            y = frame - 96;
            for (i = 0; i != 7; i++) {
                s32 drop = ParticleStreams_DropPoints[i][1] + y;

                if (drop <= 93)
                    draw(Ram_MapCellBuffer, (u8 *)work + 0x1c80, ParticleStreams_DropPoints[i][0] + x - 12, drop - 12, 24, 24);
                else if (drop <= 95)
                    AddFlame((ParticleStreams_DropPoints[i][0] + x) << 16, drop << 16, 1);
            }
            for (i = 0; i != 32; i++) {
                struct EffectStep *mote = &work->particles[i];

                if (mote->variant >= 0) {
                    draw(Ram_MapCellBuffer, (u8 *)work + (mote->variant << 10),
                        HI(mote->x) - 16, HI(mote->y) - 16, 32, 32);
                    mote->x -= direction * 0x14000;
                    mote->y -= 0x50000;
                    mote->variant++;
                    if (mote->variant == 6)
                        mote->variant = -1;
                }
            }
            work->transfer_pending = 1;
            WaitFrames(1);
        }
    skipped:
        WaitFrames(1);
        presentation->initial_value = 0;
        Scheduler_RemoveCallback(Camera_AdvanceBg2Reference);
        Scheduler_RemoveCallback(BattleFx_ArmPaletteHBlankDma);
        Scheduler_RemoveCallback(BattleFx_FlushPendingGraphicsTransfer);
        gBgScroll[1].x = saved_x;
        gBgScroll[1].y = saved_y;
    }
    Runtime_ReleaseHeapBlock(46);
    BattleEffect_SetupBlendedDisplay();
    *(volatile u16 *)0x04000020 = 0x80;
    *(volatile u32 *)0x04000028 = 0;
    *(volatile u32 *)0x0400002c = 0xfffff000;
    *(volatile u16 *)0x04000052 = 0x1010;
    *(volatile u16 *)0x0400000c = 0x2784;
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw = gWorkSlot[46];
    Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work, 1, 0);
    for (i = 0; i != 32; i++) {
        struct EffectStep *mote = &work->particles[i];

        mote->x = Random16() & 127;
        mote->y = (Random16() & 127) + 127;
    }
    for (i = 0; i != 128; i++) {
        struct EffectStep *flash = &FLASHES[i];

        flash->x = 0;
        flash->y = 0;
        flash->z = 0;
        flash->velocity_x = ((Random16() & 255) - 127) << 12;
        flash->velocity_y = (Random16() & 255) << 11;
        flash->velocity_z = ((Random16() & 255) - 127) << 12;
        flash->variant = 0;
    }
    for (i = 0; i != 512; i++) {
        struct EffectStep *ember = &EMBERS[i];

        ember->x = 0;
        ember->y = 0;
        ember->z = 0;
        ember->velocity_x = ((Random16() & 255) - 128) << 13;
        ember->velocity_y = (Random16() & 255) << 11;
        ember->velocity_z = ((Random16() & 255) - 128) << 13;
        ember->variant = 0;
    }
    work->transfer_mode = 1;
    work->transfer_value = 0x10101010;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (frame = 0; frame != 54; frame++) {
        s32 size = (frame + 0x1d0) << 8;
        struct BattleCamera *camera = gCameraWork;
        s32 elapsed = frame - 16;
        struct Scale scale;
        s32 x;
        s32 y;
        s32 grow;

        if (elapsed > 19)
            Palette_BrightenBgEntries(2, 2, 2);
        if (frame == 0)
            AudioCommand_PlayFar(156);
        if (frame == 40)
            AudioCommand_PlayFar(145);
        if (frame == 48) {
            if (mode == 1) {
                ResourceObject_ReleaseFar(work->objects[0]);
                ResourceObject_ReleaseFar(work->objects[1]);
                BattleActor_CommitPlacementFar();
            }
            BattleEventRuntime_BeginPhaseFar(134);
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
        for (i = 0; i != 64; i++) {
            struct EffectStep *ember = &EMBERS[i];

            if (ember->y >= 0) {
                s32 size;

                EffectPosition_ApplyBaseAndYOffset(&ember->x, &point);
                point.x >>= 1;
                if (point.depth <= 159)
                    point.depth = 160;
                if (point.depth > 799)
                    point.depth = 799;
                size = 9 - (point.depth - 160) / 64;
                draw(canvas, (u8 *)work + (ParticleStreams_CellOffsets[size - 1] + (i & 1) * 770) + 0x3200,
                    point.x - size / 2, point.y - size, size, size * 2);
                EffectStep_AdvanceWithGravity3D(ember, 64, -0x2000);
                if (ember->y <= 0x140000) {
                    ember->x = 0;
                    ember->z = 0;
                    ember->y = 0x140000;
                    ember->velocity_x = ((Random16() & 63) - 32) << 15;
                    ember->velocity_y = (Random16() & 63) << 13;
                    ember->velocity_z = ((Random16() & 63) - 32) << 15;
                }
            }
        }
        for (i = 0; i != 64; i++) {
            struct EffectStep *mote = &work->particles[i];
            u32 rise = i & 7;
            s32 size = rise + 3;

            draw(canvas, (u8 *)work + (ParticleStreams_CellOffsets[size - 1] + (i & 1) * 770) + 0x3200,
                mote->x - size / 2, mote->y - size, size, size * 2);
            mote->y = mote->y - rise - 8;
            if (mote->y < -10)
                mote->y = 128;
        }
        for (i = 0; i != 64; i++) {
            struct EffectStep *flash = &FLASHES[i];

            if (i / 3 < elapsed && flash->y >= 0) {
                EffectPosition_ApplyBaseAndYOffset(&flash->x, &point);
                point.x >>= 1;
                if ((u32)flash->variant <= 13) {
                    s32 cell = flash->variant / 2;

                    draw(canvas, (u8 *)work + ParticleStreams_FlashSheetOffsets[cell],
                        point.x - ParticleStreams_FlashSizes[cell] / 2,
                        point.y - ParticleStreams_FlashSizes[cell] / 2,
                        ParticleStreams_FlashSizes[cell], ParticleStreams_FlashSizes[cell]);
                }
                flash->variant++;
                if (flash->variant == 14) {
                    flash->y = 0x140000;
                    flash->x = 0;
                    flash->z = ((Random16() & 255) - 127) << 16;
                    flash->velocity_x = 0;
                    flash->velocity_y = (Random16() & 255) << 11;
                    flash->velocity_z = 0;
                    flash->variant = 0;
                } else {
                    EffectStep_AdvanceWithGravity3D(flash, 64, 1);
                }
            }
        }
        if (direction == 1)
            x = frame / 2 + 24;
        else
            x = 56 - frame / 2;
        y = 64 - frame * 2;
        grow = (frame << 8) + 0x20000;
        position[3] = 0;
        position[1] = 0xff0000;
        if (mode == 1) {
            scale.x = size;
            scale.y = size;
            position[0] = (x << 16) + 0x600000;
            position[2] = (96 - y) << 16;
            Object_ApplyProjectedPlacementFar(work->objects[0], position, &scale, 0);
            Object_ApplyProjectedPlacementFar(work->objects[1], position, &scale, 0);
        } else {
            scale.x = grow;
            scale.y = grow;
            position[0] = (x << 16) + 0x600000;
            position[2] = (96 - y) << 16;
            Object_ApplyProjectedPlacementFar(work->objects[0], position, &scale, 0);
        }
        work->shake_frames = 1;
        Camera_ApplyShake(8, 8);
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    if (mode == 0)
        ResourceObject_ReleaseFar(work->objects[0]);
    BattleFx_EndCanvasLayer();
}
