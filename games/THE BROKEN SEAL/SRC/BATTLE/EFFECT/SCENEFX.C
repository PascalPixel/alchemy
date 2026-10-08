#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MEM.H"
#include "HEAP_STATE.H"
#include "ANIMSPR.H"
#include "MOTOBJ.H"
#include "CANVAS.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "DMA.H"
#include "EFFECT_STEP.H"
#include "RAM_BUFFER.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "BTLDISP.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
#include "TRANSFORM.H"
#include "IWRAM_CALL.H"
#include "MAP_SCROLL.H"
#include "PROJECT.H"
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *state, s32 flags);

#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)

#define Camera_ApplyShake ((void (*)(s32,s32))Camera_ApplyShake)

#define gBattleFxWork ((void *(*))&gBattleFxWork)

void AudioCommand_PlayFar(s32 cue);
void AudioCommand_PlayFar(s32 cue);

struct Scale {
    s32 x;
    s32 y;
};

extern const struct Scale CirclingScene_UnitScale;

void Object_ApplyProjectedPlacementFar(void *object, s32 *position, struct Scale *scale, s32 mode);

#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer)

#define HI(v) (((s16 *)&(v))[1])

extern s16 ParticleStreams_OrbitPoints[][3];
extern s16 ParticleStreams_DropPoints[][2];
extern u16 ParticleStreams_FlashSheetOffsets[];
extern u16 ParticleStreams_FlashSizes[];
void BattleFx_FlushPendingGraphicsTransfer(void);
void BattleFx_ArmPaletteHBlankDma(void);
void Camera_AdvanceBg2Reference(void);
void BattleActor_CommitPlacementFar(void);

#define FLASHES ((struct EffectStep *)Ram_MapCellBuffer)
#define EMBERS (&FLASHES[128])

static __inline__ void CopyWords(void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: needed to match: called directly, the constant arguments are loaded before the routine's address and the size's lsls lands ahead of it */
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
        /* FAKEMATCH: cse forgets mode == 1 from the test; the arm reloads mode (ldr r4, [sp, #76]) for movs r4, #1 (2 -> 0). */
        asm ("" : "+rm" (mode));
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
            struct AnimationObject *object = ResourceObject_CreateFar(483 + i * 0x2001);

            work->objects[i] = object;
            if (object != 0) {
                object->flags = 0;
                AnimationObjects_SelectAnimationFar(object, 2);
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
        struct BattlePresentationWork *presentation = ((void **)&gTransitionWork)[0];

        gBgScroll[1].x = 0;
        gBgScroll[1].y = 32;
        BattleEffect_LoadWork(46, 8, 7, 3, 2);
        draw = ((void **)&gTransitionWork)[2];
        work->transfer_mode = 3;
        work->transfer_value = (s32)(Ram_MapBlocks + 0x202);
        Scheduler_AddOrUpdateCallback(BattleFx_ArmPaletteHBlankDma, 0x4fe);
        for (i = 0; i != 64; i++)
            work->particles[i].variant = -1;
        presentation->initial_value = 1;
        work->frame = 0;
        for (frame = 0; frame != 192; frame++, work->frame++) {
            s32 angle = frame << 8;
            /* FAKEMATCH: count fixed in r2, as in the reference; as a hard register it leaves the division temp canonical in cse, so the compare and +3 use the temp as in the reference. */
            register s32 count asm("r2") = work->frame;
            /* FAKEMATCH: work's copy in r0; else it and the 0x778c reload swap r0/r1. */
            register u8 *base asm("r0") = (u8 *)work;
            s32 heat = count / 4;
            u16 *line;
            s32 size;
            s32 x;
            s32 y;

            /* FAKEMATCH: count read and base passed through; reading count after the division keeps its load apart from the division temp (no combine); through base the asm is not volatile, so no sched2 barrier, and the mode reload lands after movs r2, #252 as in the reference (38 -> 2). */
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

                /* FAKEMATCH: hold takes r7 across the call; with line in r5, hold in r7 and r6 clobbered, heat takes r4 and its sp+8 caller-save as in the reference (222 -> 89); global.c ignores hard conflicts with r7, the frame pointer, so r7 needs a pseudo. */
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
                /* FAKEMATCH: the quotient in r3, as in the reference; local-alloc otherwise ties level to the quotient and swaps level and red (16 lines). */
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

void BattleEffect_RunCirclingFallingScene(struct BattleEffectArgument *effect)
{
    s32 position[4];
    DrawRectangle draw[2];
    struct Scale scale;
    void **cache;
    void *canvas;
    struct BattleEffectWork *work;
    s32 frame;
    u8 *sheet;
    struct Scale base;
    struct Scale orb;
    s32 i;
    s32 k;

    cache = &gBattleFxWork[1];
    canvas = cache[0];
    work = cache[-1];
    sheet = cache[1];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    BattlePres_ConfigureEffectDisplay();
    PLTT_BACKDROP = 0;
    *(u16 *)0x05000002 = 0;
    work->transfer_mode = 0;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    BattleEffect_WipeCanvas(1, 0);
    BattleFx_SelectLivingTargets(work->effect);
    BattleFx_SpawnObjects(9, 379, 2);
    for (i = 0; i != 6; i++) {
        struct AnimationObject *object = ResourceObject_CreateFar(390);

        work->objects[9 + i] = object;
        if (object != 0) {
            object->flags = 0;
            AnimationObjects_SelectAnimationFar(object, i % 3);
            ((struct AnimationObject *)work->objects[9 + i])->part[0].priority = 1;
        }
    }
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
    draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
    draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    REG_WININ = 0x2737;
    REG_WIN0H = 0xf0;
    REG_WIN1V = 0x1088;
    WaitFrames(1);
    BattleBackground_LoadFar(1, (s32)&ResourceId_DesertBackdrop, 0);
    BattleEffect_WipeCanvas(1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet, 1, 1);
    REG_DISPCNT = 0x7741;
    REG_BG2PA = 0x80;
    REG_BLDALPHA = 0x1010;
    REG_BLDCNT = 0x3f44;
    work->transfer_mode = 2;
    work->transfer_value = 50;
    base.x = 0xbc0000;
    base.y = 0x5c0000;
    orb.x = 0xa00000;
    orb.y = 0x5c0000;
    for (i = 0; i != 6; i++) {
        struct EffectStep *rock = &work->particles[i];

        rock->x = (Random16() & 127) << 16;
        rock->y = i * -0x100000;
        rock->velocity_x = 0;
        rock->velocity_y = 0;
        rock->variant = 0;
    }
    for (i = 0; i != 58; i++)
        work->particles[6 + i].variant = 24;
    for (i = 0; i != 1024; i++)
        SPARKS[i].variant = -1;
    work->fade_frames = 24;
    work->fade_step = 0;

    for (frame = 0; frame != 320 && !(gKeysRepeat & (KEY_A | KEY_B)); frame++) {
        if (frame == 94)
            AudioCommand_PlayFar(156);
        if (frame == 136)
            AudioCommand_PlayFar(156);
        if (frame == 178)
            AudioCommand_PlayFar(156);
        if (frame == 260)
            AudioCommand_PlayFar(145);
        scale = CirclingScene_UnitScale;
        if (frame >= 96 && frame <= 251)
            work->shake_frames = 1;
        else if (frame >= 260 && frame <= 263)
            work->shake_frames = 1;
        position[3] = 0;
        position[1] = 0;
        for (i = 0; i != 7; i++) {
            position[0] = (CirclingScene_ObjectColumns[i] << 16) + base.x - 0x200000;
            position[2] = (CirclingScene_ObjectRows[i] << 16) + base.y - 0x200000;
            Object_ApplyProjectedPlacementFar(work->objects[i], position, &scale, 0);
        }
        if (frame <= 90) {
            orb.x = Trig_Sin(frame << 9) * 16 + 0x9c0000;
            orb.y = Trig_Cos(frame << 9) * 16 + 0x5c0000;
        }
        if (frame <= 196) {
            s32 start;

            for (i = 0, start = 91; i != 3; start += 40, i++) {
                if (frame >= start && frame < start + 4)
                    orb.y += 0x80000;
                if (frame == start + 3) {
                    for (k = 0; k != 4; k++) {
                        struct EffectStep *puff = &work->particles[6 + i * 8 + k];

                        puff->x = 0x400000;
                        puff->y = 0x600000;
                        puff->velocity_x = ((Random16() & 255) - 127) << 10;
                        puff->velocity_y = ((Random16() & 255) - 127) << 10;
                        puff->variant = Random16() & 15;
                    }
                }
                if (frame >= start + 20 && frame < start + 36)
                    orb.y -= 0x20000;
            }
        }
        if (frame >= 244 && frame <= 251)
            orb.x -= 0x10000;
        if (frame >= 252 && frame <= 275)
            orb.x -= (frame - 250) << 16;
        if (frame <= 259) {
            position[0] = orb.x;
            position[1] = -0x1000000;
            position[2] = orb.y - 0x1000000;
            Object_ApplyProjectedPlacementFar(work->objects[7], position, &scale, 0);
            position[0] = orb.x + 0x200000;
            Object_ApplyProjectedPlacementFar(work->objects[8], position, &scale, 0);
        }
        position[1] = 0;
        for (i = 0; i != 6; i++) {
            struct EffectStep *rock = &work->particles[i];

            if (rock->variant != 2) {
                position[0] = rock->x;
                position[2] = rock->y;
                Object_ApplyProjectedPlacementFar(work->objects[9 + i], position, &scale, 0);
                rock->x += rock->velocity_x;
                rock->y += rock->velocity_y;
                if (frame > 96)
                    rock->velocity_y += 0x4000;
                if (rock->y > 0x780000) {
                    rock->variant++;
                    if (rock->variant == 1) {
                        rock->velocity_y = -rock->velocity_y / 2;
                        for (k = 0; k != 2; k++) {
                            struct EffectStep *puff = &work->particles[30 + i * 2 + k];

                            puff->x = rock->x / 2;
                            puff->y = rock->y - 0x200000;
                            puff->velocity_x = ((Random16() & 255) - 127) << 10;
                            puff->velocity_y = ((Random16() & 255) - 127) << 10;
                            puff->variant = Random16() & 15;
                        }
                    } else if (frame <= 199) {
                        rock->y = 0;
                        rock->velocity_y = 0;
                        rock->variant = 0;
                    }
                }
            }
        }
        for (i = 0; i != 56; i++) {
            struct EffectStep *puff = &work->particles[6 + i];

            if (puff->variant >= 0) {
                if (puff->variant >= 0 && puff->variant <= 23) {
                    s32 cell = puff->variant / 6 + 3;

                    draw[0](canvas, work->sheet + CirclingScene_PuffSheetOffsets[cell],
                        HI(puff->x) - CirclingScene_PuffSizes[cell] / 2,
                        HI(puff->y) - CirclingScene_PuffSizes[cell] / 2,
                        CirclingScene_PuffSizes[cell], CirclingScene_PuffSizes[cell]);
                }
                EffectStep_AdvanceWithGravity2D(puff, 60, -0x4000);
                puff->variant++;
            }
        }
        if (frame == 260) {
            for (i = 0; i != work->effect->count; i++) {
                BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
                ObjectGroup_UpdateMembers(work->effect->actors[i], 7, -1, i, 8);
            }
            work->shake_frames = 8;
        }
        if (frame == 260) {
            for (i = 0; i != 512; i++) {
                struct EffectStep *spark = &SPARKS[i];
                s32 speed;
                s32 angle;

                speed = 0x3ff;
                speed &= Random16();
                angle = Random16() & 0xffff;
                spark->x = 0x200000;
                spark->y = 0x5c0000;
                spark->velocity_x = Trig_Sin(angle) * (speed + 32) >> 7;
                spark->velocity_y = -(Trig_Cos(angle) * (speed + 32) << 1) >> 7;
                spark->variant = (Random16() & 15) + 32;
            }
        }
        for (i = 0; i != 512; i++) {
            struct EffectStep *spark = &SPARKS[i];

            if (spark->variant >= 0) {
                s32 size = (spark->variant >> 3) + 1;

                draw[i & 1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
                    HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
                EffectStep_AdvanceWithGravity2D(spark, 62, 0x1000);
                spark->variant--;
            }
        }
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    BattleEventRuntime_BeginPhaseFar(134);
    BattleEffect_SetupBlendedDisplay();
    for (i = 0; i != 15; i++)
        ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}

struct TrailPoint {
    s32 x;
    s32 y;
};

void BattleEffect_RunDualParticleStream(struct BattleEffectArgument *effect)
{
    struct TrailPoint trail[20];
    struct EffectPosition position;
    DrawRectangle draw[2];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    s32 drift;
    s8 *path;
    s32 x;
    s32 y;
    u8 *sheet;
    struct BattleCamera *camera;
    s32 i;
    s32 j;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    drift = 0;
    x = 0;
    y = 0;
    sheet = heap_cache[2];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    REG_BLDALPHA = 0x1010;
    Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetB, work->sheet, 1, 1);
    for (i = 1; i != 20; i++) {
        for (j = 0; j != 936; j++) {
            s32 shade = work->sheet[0x1680 + j];

            if (i > 10) {
                shade = shade - i * 4 + 40;
                if (shade < 0)
                    shade = 0;
                work->sheet[0x1680 + (i - 10) * 936 + j] = shade;
            }
        }
    }
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, &work->sheet[0x3c00], 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, Ram_MapCellBuffer, 1, 0);
    BattleFx_FetchRectangleBlitters(work->effect->side, draw);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (j = 0; j != 40; j++) {
        struct EffectStep *ember = &work->particles[8 + j];

        if (work->effect->side == 0)
            ember->x = -0x380000;
        else
            ember->x = 0x380000;
        ember->y = 0;
        ember->z = 0;
        ember->velocity_x = ((Random16() & 63) - 32) << 14;
        ember->velocity_y = (Random16() & 63) << 13;
        ember->velocity_z = ((Random16() & 63) - 32) << 14;
        ember->variant = 1;
    }
    for (j = 0; j != 16; j++) {
        struct EffectStep *flash = &work->particles[48 + j];

        if (work->effect->side == 0)
            flash->x = -0x380000;
        else
            flash->x = 0x380000;
        flash->y = 0x140000;
        flash->z = 0;
        flash->velocity_x = ((Random16() & 63) - 32) << 14;
        flash->velocity_y = (Random16() & 63) << 12;
        flash->velocity_z = ((Random16() & 63) - 32) << 14;
        flash->variant = 0;
    }
    for (i = 0, j = -0x4000; i != 8; i++, j += 0x1000) {
        struct EffectStep *puff = &work->particles[i];

        if (work->effect->side == 1)
            puff->x = (Trig_Sin(j) * 24 >> 16) + 88;
        else
            puff->x = (-(Trig_Sin(j) * 24) >> 16) + 16;
        puff->y = (Trig_Cos(j) * 16 >> 16) + 40;
        puff->variant = -(i * 2);
    }
    path = Resource_GetTableEntry((s32)&ResourceId_DualStreamPath);

    for (frame = 0; frame != 150; frame++) {
        camera = gCameraWork;
        if (frame == 83)
            BattleEventRuntime_BeginPhaseFar(134);
        if (frame == 0)
            AudioCommand_PlayFar(136);
        if (frame == 50)
            AudioCommand_PlayFar(136);
        if (work->effect->side == 0) {
            if (frame <= 63)
                camera->yaw -= 0x100;
        } else {
            if (frame <= 63)
                camera->yaw += 0x100;
        }
        BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
        if (frame <= 17) {
            s32 cell = frame / 3;

            draw[0](canvas, &work->sheet[0x3c00] + PuffArc_CellSourceOffsets[cell],
                48, PuffArc_CellBiasY[cell] + 60,
                PuffArc_CellWidths[cell], PuffArc_CellHeights[cell]);
            draw[1](canvas, &work->sheet[0x3c00] + PuffArc_CellSourceOffsets[cell],
                56, PuffArc_CellBiasY[cell] + 60,
                PuffArc_CellWidths[cell], PuffArc_CellHeights[cell]);
        }
        if (frame >= 18 && frame <= 58) {
            if (frame == 18) {
                x = (path[0] << 8) + (u8)path[1];
                y = (path[2] << 8) + (u8)path[3] + 16;
                path += 4;
            } else {
                x += path[0];
                y += path[1];
                path += 2;
            }
        }
        if (frame >= 78 && frame <= 118) {
            if (frame == 78) {
                x = -56;
                y = 48;
            } else {
                y -= 16;
            }
        }
        for (j = 19; j != 0; j--) {
            if (frame > j + 18 && frame <= j + 83) {
                trail[j].x = trail[j - 1].x;
                trail[j].y = trail[j - 1].y;
                if (j > 10)
                    draw[0](canvas, &work->sheet[0x1680 + (j - 10) * 936],
                        trail[j].x, trail[j].y, 24, 39);
                else
                    draw[0](canvas, &work->sheet[0x1680], trail[j].x, trail[j].y, 24, 39);
            }
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
        if (frame >= 18 && frame <= 83) {
            s32 lift;

            if (work->effect->side == 1)
                position.x = 64 - x / 2;
            else
                position.x = x / 2 + 64;
            position.y = 60 - y;
            lift = (position.y - trail[0].y - 24) / 2;
            if (lift > 2)
                lift = 2;
            if (lift < -2)
                lift = -2;
            drift += lift;
            if (drift > 8)
                drift = 8;
            if (drift < -8)
                drift = -8;
            lift = drift / 4 + 2;
            trail[0].x = position.x - 12;
            trail[0].y = position.y - 20;
            draw[0](canvas, &work->sheet[lift * 0x480],
                trail[0].x - 6, trail[0].y - 2, 24, 48);
        }
        if (frame == 83) {
            work->shake_frames = 8;
            ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
        }
        if (frame > 83) {
            for (i = 0; i != 56; i++) {
                struct EffectStep *ember = &work->particles[8 + i];

                if (ember->y >= 0) {
                    s32 size;

                    EffectPosition_ApplyBaseAndYOffset(&ember->x, &position);
                    position.x >>= 1;
                    if (position.depth <= 159)
                        position.depth = 160;
                    if (position.depth > 799)
                        position.depth = 799;
                    size = 9 - (position.depth - 160) / 64;
                    if (i > 47) {
                        if (ember->variant <= 11) {
                            draw[0](canvas, Ram_MapCellBuffer + ember->variant / 2 * 0x800,
                                position.x - 16, position.y - 32, 32, 64);
                            ember->variant++;
                        }
                    } else {
                        draw[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
                            position.x - size / 2, position.y - size, size, size * 2);
                    }
                    ember->x += ember->velocity_x;
                    ember->y += ember->velocity_y;
                    ember->z += ember->velocity_z;
                    ember->velocity_y -= 0x2000;
                }
            }
        }
        if (frame == 50) {
            work->shake_frames = 12;
            ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
        }
        if (frame > 49) {
            for (i = 0; i != 8; i++) {
                struct EffectStep *puff = &work->particles[i];

                if (puff->variant >= 0 && puff->variant <= 11) {
                    s32 cell = puff->variant / 2;

                    draw[1](canvas, &work->sheet[0x3c00] + PuffArc_CellSourceOffsets[cell],
                        puff->x - PuffArc_CellWidths[cell] / 2,
                        puff->y + PuffArc_CellBiasY[cell],
                        PuffArc_CellWidths[cell], PuffArc_CellHeights[cell]);
                }
                puff->variant++;
            }
        }
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}

void BattleFx_RunFireBurstShards(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    u8 *cells;
    struct BattleCamera *view;
    struct MotionObject *slot;
    struct EffectStep *shard;
    struct EffectPosition position;
    struct EffectPosition origin;
    struct EffectPosition out;
    volatile s32 fill;
    DrawRectangle routine[2];
    s32 i;
    s32 frame;
    s32 size;
    u8 *burst;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    cells = heap_cache[2];
    view = heap_cache[HEAP_SLOT_CAMERA - HEAP_SLOT_BATTLE_EFFECT];
    burst = Ram_MapCellBuffer;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    Resource_LoadAndDecompress((s32)&ResourceId_FireStreakSheet, work->sheet, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_FireBurstSheet, burst, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, cells, 0, 0);
    BattleMotion_ApproachTargetFar(work->effect->actor, work->effect->actors[0], 4, 0);
    WaitFrames(1);
    slot = GetBattleObjectSlotFar(work->effect->actors[0])->object;

    i = 0;
    do {
        struct EffectStep *seed = &work->particles[i];

        seed->x = slot->x;
        seed->y = slot->y;
        seed->z = slot->z;
        seed->velocity_x = (Random16() & 255) << 11;
        seed->velocity_y = ((Random16() & 255) - 127) << 12;
        seed->velocity_z = ((Random16() & 255) - 127) << 12;
        if (seed->x > 0)
            seed->velocity_x = -seed->velocity_x;
        seed->variant = i / 4 * 2 + 16;
        i++;
    } while (i != 64);

    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &origin);
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    work->transfer_mode = 2;
    work->transfer_value = 75;

    for (frame = 0; frame != 64; frame++) {
        if (frame == 8)
            BattleEventRuntime_BeginPhaseFar(0x86);
        if (work->effect->variant != 0 && frame == 8)
            AudioCommand_PlayFar(0xd4);
        EffectPosition_ApplyStepAndYOffset(work->effect->actor, &position);
        if (frame >= 6 && frame <= 11) {
            if (work->effect->side == 0)
                BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
            else
                BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 3);
            routine[0] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
            if (work->effect->side == 0)
                routine[0](canvas, work->sheet + (frame - 6) * 0xd80, position.x / 2 - 24, position.y - 24, 48, 72);
            else
                routine[0](canvas, work->sheet + (frame - 6) * 0xd80, position.x / 2, position.y - 24, 48, 72);
            Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        }
        if (frame >= 16 && frame <= 47) {
            s32 step = (frame - 16) / 2;
            s32 strip;

            BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
            routine[0] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
            if (step > 2)
                step = 2;
            if (work->effect->variant == 0)
                strip = 0;
            else
                strip = 0x2580;
            routine[0](canvas, Ram_MapCellBuffer + strip + step * 0xc80, origin.x / 2 - 20, origin.y - 48, 40, 80);
            BattleFx_RunNoEffectFrames(10000);
            Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        }
        if (frame == 8) {
            fill = 0x3f3f3f3f;
            Dma_Set((void *)&fill, canvas, 0x85001000, REG_DMA3);
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(view->eye, view->pos);
        if (frame > 3) {
            BattleFx_FetchRectangleBlitters(work->effect->side, routine);
            for (i = 0; i != 64; i++) {
                s32 life;

                shard = &work->particles[i];
                life = shard->variant;
                if (life > 0) {
                    EffectPosition_ApplyBaseAndYOffset((s32 *)shard, &out);
                    out.x >>= 1;
                    out.y = out.y + origin.y - 112;
                    size = (life >> 3) + 2;
                    routine[(i / 2) & 1](canvas, cells + ParticleStreams_CellOffsets[size - 1], out.x - size / 2, out.y - size,
                        size, size * 2);
                    EffectStep_AdvanceWithGravity3D(shard, 60, -0x400);
                    shard->variant--;
                }
            }
            Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
            Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        }
        if (frame == 8) {
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
            work->shake_frames = 4;
        }
        if (frame == 6)
            ObjectGroup_UpdateMembers(work->effect->actors[0], 10, -1, -1, 0);
        if (frame == 14)
            ObjectGroup_UpdateMembers(work->effect->actors[0], 10, -1, -1, 0);
        Camera_ApplyShake(16, 16);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}
