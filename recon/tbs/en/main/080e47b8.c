/* 2026-09-29: five minutes of permutation (--function
 * BattleFx_RunCastingImpact): 2621 -> 2380 (45 register-only, 37 operand,
 * 13 reordered, 3 inserted, 3 deleted) with four natural rewrites, the
 * particle and fade reads assigned inside their tests and the image height
 * set after the blitter table is loaded; the function is in the permuter's
 * own formatting. Many resource and message numbers are still Value_
 * symbols. */
/* NONMATCHING: 7808-byte owner; complete casting and impact sequence.
 * The acting unit gathers particles, then launches the selected effect at
 * the first affected unit. All 217 calls follow the reference sequence.
 * Candidate 7808 bytes; 181 differing halfwords, 113 aligned edits.
 * Separate column/pair cursors canonicalize to the same 7804-byte draft
 * (189 aligned edits); loading the moving x before subtracting gives
 * 7808 bytes / 182 halfwords / 116 edits. The closer model is retained. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "FIXED_POINT_POSITION.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"

/* Image resources share this EWRAM scratch area for the duration of the effect. */
#define IMAGE_WORK ((u8 *)0x02010000)

typedef BattleEffectDrawRectangle RectangleBlit;
extern u8 gWorkSlot[];
extern u8 Value_0000008d;
extern u8 Value_000000a3;
extern u8 Value_000000a4;
extern u8 Value_000000a0;
extern u8 Value_000000bb;
extern u8 Value_000000b9;
extern u8 Value_000000c0;

extern u8 Value_00000053;
extern u8 Value_0000006f;
extern u8 Value_00000073;
extern u8 Value_00000079;
extern u8 Value_0000007d;
extern u8 Value_0000008e;
extern u8 Value_00000090;
extern u8 Value_00000092;
extern u8 Value_00000094;
extern u8 Value_00000096;
extern u8 Value_00000099;
extern u8 Value_0000009e;
extern u8 Value_000000a9;
extern u8 Value_000000ab;
extern u8 Value_000000ac;
extern u8 Value_000000ad;
extern u8 Value_000000ae;
extern u8 Value_000000b4;
extern u8 Value_000000b8;
extern u8 Value_000000c3;
extern u8 Value_000000c4;
extern u8 Value_000000ce;

void Render_ResetTransformState(void);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyRoll(s32 angle);
void Graphics_PrepareTransferInIwramWork(void *eye, void *target);
void Object_SetMode(void *object, s32 mode);
void ObjectDispatch_ApplyValueToChildrenFar(void *object, s32 value);
u32 Battle_GetObjectTableValueFar(s32 actor_id);
void BattleMotion_ApplyVariantMotionFar(s32 actor_id, s32 motion);
struct BattleObjectSlot *GetBattleObjectSlotFar(s32 actor_id);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void ObjectGroup_TickMemberTimers(void);
void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_BeginTiledCanvas(u32 display_control);
void BattleFx_EndCanvasLayer(void);
void BattleFx_FetchRectangleBlitters(s32 alternate, RectangleBlit *output);
s32 BattleFx_RunSparkGroups(void *effect, s32 mode);
void BattleFx_RenderMode5(void *effect);
void ObjectGroup_UpdateMembers(s32 set_id, s32 object_value, s32 group_value, s32 state_slot,
                               s32 state_value);
void BattleFx_RunPaletteRampMode1(s32 effect);
s32 BattleFx_RunProjectileVolley(void *effect, s32 mode);
/* This caller passes a height too; the packer itself uses 288 rows. */
void Graphics_PackTileRows();
void Camera_ApplyShake(s32 random_mask, u32 shake_range);
void BattleFx_StepPaletteToResource(s32 resource_id);

void Audio_PlayCue(s32 cue);

/* FAKEMATCH: Inline scope keeps each resource call's arguments local. */
static __inline__ void LoadResource(s32 id, void *dest, s32 skip, s32 copy)
{
    Resource_LoadAndDecompress(id, dest, skip, copy);
}

/* FAKEMATCH: Keep packing arguments within their own inline scope. */
static __inline__ void PackRows(void *src, void *dest, s32 width, s32 height)
{
    Graphics_PackTileRows(src, dest, width, height);
}

/* FAKEMATCH: Inline scope makes each clearing call reload its byte count. */
static __inline__ void ClearWords(void *dest, u32 size)
{
    ((s32 (*)(void *, u32))0x03000164)(dest, size);
}

/* FAKEMATCH: Inline scope reloads the fill routine before its arguments. */
static __inline__ void FillWords(void *dest, u32 size, u32 value)
{
    ((void (*)(void *, u32, u32))0x03000168)(dest, size, value);
}

/* Particle image offsets indexed by size; the remaining tables describe
 * individual impact images (offsets, dimensions and placement). */
extern u16 ParticleStreams_CellOffsets[];
extern u16 CastingImpact_OrbitCells[];
extern u8 PuffArc_CellWidths[];
extern u8 PuffArc_CellHeights[];
extern u8 PuffArc_CellBiasY[];
extern u16 PuffArc_CellSourceOffsets[];
extern u16 BattleFx12_SmokeCells[];
extern u8 BattleFx12_SmokeWidths[];
extern u8 BattleFx12_SmokeHeights[];
extern u8 CastingImpact_SmokeDrawFlags[];
extern u8 CastingImpact_ImageX[];
extern u8 CastingImpact_ImageY[];

/* FAKEMATCH: Inline scope reloads each image address at its drawing call. */
static __inline__ void DrawImage(void *canvas, const void *pixels, s32 x, s32 y, s32 width, s32 height,
                                 RectangleBlit *draw)
{
    (*draw)(canvas, pixels, x, y, width, height);
}

/* FAKEMATCH: Derive height inside the inline scope so the dimension stores
 * reuse one register instead of keeping two argument temporaries alive. */
static __inline__ void DrawTallImage(void *canvas, const void *pixels,
    s32 x, s32 y, s32 width, RectangleBlit *draw)
{
    (*draw)(canvas, pixels, x, y, width, width * 2);
}

/* FAKEMATCH: Keep the cropped height inside the drawing scope. */
static __inline__ void DrawCroppedImage(void *canvas, const void *pixels,
    s32 x, s32 y, s32 width, RectangleBlit *draw)
{
    (*draw)(canvas, pixels, x, y, width, 91);
}

/* Two scrolling pieces form the column; the wider image anchors its base. */
static __inline__ void DrawRisingColumn(void *canvas, s32 origin_x, s32 rise,
    s32 scroll, RectangleBlit *draw)
{
    s32 width;
    s32 tile_height;
    s32 column_x;
    s32 column_y;

    width = 17;
    tile_height = 104;
    column_y = rise + scroll;
    column_x = origin_x - width / 2;
    DrawImage(canvas, IMAGE_WORK, column_x, column_y - tile_height, width,
              tile_height, draw);
    DrawImage(canvas, IMAGE_WORK, column_x, column_y, width,
              tile_height - scroll, draw);
    DrawImage(canvas, (IMAGE_WORK + 0x6e8), origin_x - width, rise + 47, width * 2, 65, draw);
}

void BattleFx_RunCastingImpact(struct BattleEffectArgument *command, s32 kind)
{
    void **slots;
    void **slot;
    s32 active_cnt;
    s32 image_offset;
    /* FAKEMATCH: Keep the shake timer address live into the shared store. */
    s32 *shake_addr;
    s32 speed;
    struct EffectPosition caster_pos;
    struct EffectPosition target_pos;
    struct FixedPointPosition moving_pos;
    s32 phase;
    s32 i;
    s32 *scanline;
    s32 cnt;
    s32 orb_x;
    RectangleBlit *work_blitters;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    u8 *camera;
    u8 *sprites;
    s32 scroll_pos;
    s32 scroll_speed;
    s32 duration;
    struct EffectPosition *target_screen;
    struct EffectPosition *caster_screen;
    RectangleBlit *draw_pair;
    s32 saved_velocity_x;
    s32 saved_velocity_y;
    s32 saved_velocity_z;
    s32 saved_acceleration;
    s32 saved_vertical_strength;
    struct MotionObject *target_actor;
    struct FixedPointPosition *motion;
    s32 kind_from_two;
    s32 kind_from_four;
    /* FAKEMATCH: Declaration order preserves the two temporary stack slots. */
    s32 origin_x;
    struct FixedPointPosition *position;
    struct MotionObject *caster_actor;
    struct EffectStep *particle;
    s32 pair_x;
    s32 strip_x;
    struct FixedPointPosition velocity;
    struct EffectPosition spark_screen;
    RectangleBlit blitters[2];
    struct EffectPosition projected;

    slots = (void **)(gWorkSlot + 39 * 4);
    slot = slots;
    work = (struct BattleEffectWork *)*slot++;
    canvas = *slot;
    camera = *(u8 **)((u8 *)slots - 0x6c);
    sprites = (u8 *)slots[2];
    work->effect = command;
    if (kind == 11 || kind == 8 || kind == 32) {
        BattleFx_BeginTiledCanvas(0);
    } else {
        BattleFx_BeginCanvasLayer(0);
    }
    *(volatile u16 *)0x04000052 = 0x1010;
    LoadResource((s32)&Value_00000073, sprites, 0, 0);
    LoadResource((s32)&Value_00000096, work, 1, 0);
    LoadResource((s32)&Value_00000099, IMAGE_WORK, 1, 0);
    /* FAKEMATCH: Local dimensions preserve the packing argument order. */
    {
        s32 width = 40;
        s32 height = 0x120;
        PackRows(IMAGE_WORK, (u8 *)work + 0x5100, width, height);
    }
    if (kind == 5 || kind == 23) {
        LoadResource((s32)&Value_0000007d, IMAGE_WORK, 1, 0);
    } else if (kind == 12) {
        LoadResource((s32)&Value_000000a9, IMAGE_WORK, 1, 0);
    } else if (kind == 6 || kind == 27) {
        LoadResource((s32)&Value_000000ce, IMAGE_WORK, 1, 0);
        LoadResource((s32)&Value_000000c4, (void *)0x02010c56, 1, 0);
    } else if (kind == 31) {
        LoadResource((s32)&Value_00000079, IMAGE_WORK, 1, 1);
    } else if (kind == 8) {
        LoadResource((s32)&Value_000000c3, IMAGE_WORK, 1, 1);
    } else if (kind == 14) {
        LoadResource((s32)&Value_0000006f, IMAGE_WORK, 1, 0);
    } else if (kind == 30) {
        LoadResource((s32)&Value_000000ce, IMAGE_WORK, 1, 0);
    } else if (kind == 16) {
        LoadResource((s32)&Value_000000b8, IMAGE_WORK, 1, 0);
    } else if (kind == 20) {
        LoadResource((s32)&Value_000000b4, IMAGE_WORK, 1, 0);
    } else if ((u32)(kind - 33) <= 1) {
        LoadResource((s32)&Value_00000053, IMAGE_WORK, 1, 0);
    } else if (kind != 11 && kind != 32) {
        LoadResource((s32)&Value_0000009e, IMAGE_WORK, 1, 0);
    }
    switch (kind) {
    case 0:
    case 4:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 33:
        LoadResource((s32)&Value_00000094, (void *)0x02013c56, 1, 1);
        break;
    case 2:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
        LoadResource((s32)&Value_00000092, (void *)0x02013c56, 1, 1);
        break;
    case 3:
    case 5:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 34:
    case 35:
        LoadResource((s32)&Value_0000008e, (void *)0x02013c56, 1, 1);
        break;
    case 1:
    case 6:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
        LoadResource((s32)&Value_00000090, (void *)0x02013c56, 1, 1);
        break;
    case 100:
        LoadResource((s32)&Value_00000092, (void *)0x02013c56, 1, 1);
        break;
    }
    work->transfer_mode = 2;
    if (kind == 12) {
        work->transfer_value = 75;
    } else {
        work->transfer_value = 50;
    }
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    /* FAKEMATCH: Retain each output pointer while preparing its call. */
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], target_screen = &target_pos);
    EffectPosition_ApplyStepAndYOffset(work->effect->actor, caster_screen = &caster_pos);
    BattleFx_FetchRectangleBlitters(work->effect->side, draw_pair = blitters);
    work->fade_frames = 24;
    work->fade_step = 0;
    /* Gather particles around the acting unit for 32 frames. The same
       * records later hold the sparks emitted at the target. */
    caster_actor = GetBattleObjectSlotFar(work->effect->actor)->object;
    {
        for (i = 0; i != 64; i++) {
            particle = &work->particles[i];
            particle->x = (Random16() & 63) + 32;
            particle->y = 0;
            particle->z = 0;
            /* Gathering reuses velocity fields as rotation angles. */
            particle->velocity_x = Random16() & 0xffff;
            particle->velocity_y = Random16() & 0xffff;
            particle->velocity_z = Random16() & 0xffff;
        }
    }
    ObjectDispatch_ApplyValueToChildrenFar(caster_actor, 0);
    position = &moving_pos;
    position->x = caster_actor->x;
    position->y = caster_actor->y + 0x500000;
    position->z = caster_actor->z;
    saved_velocity_x = caster_actor->velocity_x;
    saved_velocity_y = caster_actor->velocity_y;
    saved_velocity_z = caster_actor->velocity_z;
    saved_acceleration = caster_actor->acceleration;
    saved_vertical_strength = caster_actor->vertical_motion_strength;
    caster_actor->velocity_x = 0;
    caster_actor->velocity_y = 0;
    caster_actor->velocity_z = 0;
    caster_actor->acceleration = 0;
    caster_actor->vertical_motion_strength = 0;
    EffectPosition_ApplyStepAndYOffset(work->effect->actor, caster_screen);
    caster_screen->x = caster_screen->x / 2;
    Audio_PlayCue(212);
    frame = 0;
    do {
        cnt = 0;
        for (i = 0; i != 64; i++) {
            struct EffectStep *step = &work->particles[i];
            if (step->x >= 0) {
                if (frame >= i / 4) {
                    {
                        s32 size = 5;
                        Render_ResetTransformState();
                        SceneTransform_ApplyRoll(step->velocity_z);
                        SceneTransform_ApplyPitch(step->velocity_x);
                        SceneTransform_ApplyYaw(step->velocity_y);
                        EffectPosition_ApplyBaseAndYOffset((s32 *)step, &spark_screen);
                        spark_screen.x = spark_screen.x / 2 + caster_screen->x;
                        if (kind <= 7)
                            spark_screen.y = spark_screen.y + caster_screen->y - 8;
                        else if (kind == 35)
                            spark_screen.y = spark_screen.y + caster_screen->y + 44;
                        else
                            spark_screen.y = spark_screen.y + caster_screen->y + 12;
                        if (spark_screen.depth < -60)
                            spark_screen.depth = -60;
                        if (spark_screen.depth > 60)
                            spark_screen.depth = 60;
                        spark_screen.depth += 60;
                        blitters[1](canvas, sprites + ParticleStreams_CellOffsets[size - 1], spark_screen.x - size / 2, spark_screen.y - size, size, size * 2);
                    }
                    step->x -= 4;
                }
                cnt++;
            }
        }
        if (kind <= 7) {
            active_cnt = cnt;
            if (active_cnt <= 63) {
                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork(camera, camera + 12);
                EffectPosition_ApplyBaseAndYOffset((s32 *)&moving_pos, &spark_screen);
                orb_x = spark_screen.x;
                /* FAKEMATCH: Keep signed halving in the loaded coordinate register. */
                orb_x += (u32)orb_x >> 31;
                orb_x >>= 1;
                spark_screen.x = orb_x;
                blitters[0](canvas, IMAGE_WORK + 0x3c56, orb_x - 10, spark_screen.y - 4, 20, 40);
            }
        }
        work->transfer_pending = 1;
        WaitFrames(1);
        frame++;
    } while (frame != 32);
    /* FAKEMATCH: Restart the pointer's lifetime after gathering so the
       * projection above uses the stack position directly. */
    position = &moving_pos;
    if (kind == 11) {
        *(volatile u16 *)0x04000020 = 0x100;
        if (work->effect->side == 0) {
            *(volatile s32 *)0x04000028 = (frame - target_screen->x) << 8;
        } else {
            *(volatile s32 *)0x04000028 = (96 - target_screen->x) << 8;
        }
    } else {
        if (kind == 32) {
            *(volatile u16 *)0x04000020 = 0x100;
            if (work->effect->side == 0) {
                scroll_pos = -0x800000;
                scroll_speed = 0xc0000;
            } else {
                scroll_pos = 0x80000;
                scroll_speed = -0xc0000;
            }
            *(volatile s32 *)0x04000028 = (scroll_pos >> 16) << 8;
        }
    }
    if (kind == 8) {
        *(volatile u16 *)0x04000020 = 0x100;
        *(volatile s32 *)0x04000028 = (64 - target_screen->x) << 8;
        work->transfer_mode = 1;
        work->transfer_value = 0;
        ClearWords((void *)0x06004000, 0x4000);
        ClearWords(canvas, 0x4000);
        {
            s32 shown = 0;
            *(volatile u16 *)0x04000050 = shown;
        }
    }
    if (kind == 31) {
        *(volatile u16 *)0x04000020 = 0x100;
        if (work->effect->side == 0) {
            *(volatile s32 *)0x04000028 = (32 - target_screen->x) << 8;
        } else {
            *(volatile s32 *)0x04000028 = (96 - target_screen->x) << 8;
        }
    }
    if (kind == 15 || kind == 17 || kind == 24 || kind == 26) {
        ClearWords((void *)0x06004000, 0x4000);
        ClearWords(canvas, 0x4000);
        work->effect->unknown_001c = 0;
        Scheduler_RemoveCallback((u32)Palette_StepFadeTransfer);
        Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
        Runtime_ReleaseHeapBlock(47);
        Runtime_ReleaseHeapBlock(46);
        Object_SetMode(caster_actor, 3);
        if (kind == 15) {
            BattleFx_RunProjectileVolley(command, 9);
        }
        if (kind == 24) {
            BattleFx_RenderMode5(command);
        }
        if (kind != 26) {
            return;
        }
        BattleFx_RunProjectileVolley(command, 8);
        return;
    }
    ObjectDispatch_ApplyValueToChildrenFar(caster_actor, 16);
    caster_actor->velocity_x = saved_velocity_x;
    caster_actor->velocity_y = saved_velocity_y;
    caster_actor->velocity_z = saved_velocity_z;
    caster_actor->acceleration = saved_acceleration;
    caster_actor->vertical_motion_strength = saved_vertical_strength;
    if (kind == 35) {
        ClearWords((void *)0x06004000, 0x4000);
        ClearWords(canvas, 0x4000);
        work->effect->unknown_001c = 0;
        Scheduler_RemoveCallback((u32)Palette_StepFadeTransfer);
        Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
        Runtime_ReleaseHeapBlock(47);
        Runtime_ReleaseHeapBlock(46);
        command->variant = 3;
        BattleFx_RunSparkGroups(command, 2);
        return;
    }
    /* Restore the caster and aim the travelling orb at the first target. */
    target_actor = GetBattleObjectSlotFar(work->effect->actors[0])->object;
    motion = &velocity;
    speed = Math_Div(target_actor->x - position->x, 6);
    motion->x = speed;
    speed = Math_Div(target_actor->y - position->y + 0x1e0000, 6);
    motion->y = speed;
    speed = Math_Div(target_actor->z - position->z, 6);
    motion->z = speed;
    for (i = 0; i != 64; i++) {
        particle = &work->particles[i];
        particle->variant = 0;
    }
    if (kind != 14) {
        s32 height = (s32)Battle_GetObjectTableValueFar(work->effect->actors[0]) / 2;
        for (i = 0; i != 32; i++) {
            particle = &work->particles[i];
            particle->x = target_actor->x;
            particle->y = height;
            particle->z = target_actor->z;
            if (kind == 31) {
                particle->velocity_x = ((s32)(Random16() & 255) - 127) << 12;
                particle->velocity_y = ((s32)(Random16() & 255) - 64) << 10;
            } else {
                particle->velocity_x = ((s32)(Random16() & 255) - 127) << 12;
                particle->velocity_y = ((s32)(Random16() & 255) - 64) << 12;
            }
            /* FAKEMATCH: Keep the final velocity store ahead of the lifetime calculation. */
            do {
                particle->velocity_z = ((s32)(Random16() & 255) - 127) << 12;
            } while (0);
            particle->variant = i / 2 + 32;
        }
    }
    if (kind == 11) {
        LoadResource((s32)&Value_000000ab, work, 1, 1);
        LoadResource((s32)&Value_000000ac, IMAGE_WORK, 1, 0);
        *(volatile u16 *)0x04000052 = 0xe10;
    }
    if (kind == 32) {
        LoadResource((s32)&Value_000000ad, work, 1, 1);
        LoadResource((s32)&Value_000000ae, IMAGE_WORK, 1, 0);
        *(volatile u16 *)0x04000052 = 0xe10;
    }
    if (kind != 7 && kind != 13 && kind != 18 && kind != 11 && kind != 32 && kind != 19) {
        s32 height = 0;
        if (kind != 12)
            height = 0x140000;
        for (i = 0; i != 64; i++) {
            particle = &((struct EffectStep *)0x02014000)[i];
            particle->x = target_actor->x;
            particle->y = height;
            particle->z = target_actor->z;
            if (kind == 5 || kind == 23) {
                particle->velocity_x = ((s32)(Random16() & 255) - 127) << 11;
                particle->velocity_y = (Random16() & 255) << 11;
                particle->velocity_z = ((s32)(Random16() & 255) - 127) << 11;
            } else if (kind == 25) {
                particle->velocity_x = ((s32)(Random16() & 255) - 127) << 11;
                particle->velocity_y = (Random16() & 127) << 10;
                particle->velocity_z = ((s32)(Random16() & 255) - 127) << 11;
            } else {
                particle->velocity_x = ((s32)(Random16() & 255) - 127) << 10;
                particle->velocity_y = (Random16() & 127) << 10;
                particle->velocity_z = ((s32)(Random16() & 255) - 127) << 10;
            }
            particle->variant = 0;
        }
    }
    kind_from_two = kind - 2;
    if ((u32)kind_from_two <= 1 || kind == 12 || kind == 22 || kind == 29 || kind == 28) {
        Scheduler_AddOrUpdateCallback((s32)BattleFx_ArmBg2AffineHBlankDma, 0x480);
    }
    kind_from_four = kind - 4;
    if ((u32)kind_from_four <= 2 || kind == 23 || kind == 30 || kind == 27 || kind == 33 || kind == 34 || kind == 100) {
        duration = 32;
    } else if ((u32)kind <= 3 || kind == 8 || kind == 9 || kind == 10 || kind == 22 || kind == 25 || kind == 29 || kind == 31 || kind == 14) {
        duration = 48;
    } else if (kind == 21) {
        duration = 20;
    } else if (kind == 11 || kind == 32 || kind == 20) {
        duration = 40;
    } else if (kind == 28 || kind == 12) {
        duration = 64;
    } else {
        duration = 80;
    }
    /* Animate the selected impact and advance the battle event each frame. */
    frame = 0;
    while (frame != duration) {
        if (kind != 11) {
            if (kind != 32) {
                s32 base;
                scanline = work->bg2_x;
                i = 0;
                base = 0x40000;
                phase = frame << 12;
                for (; i != 160; i++) {
                    s32 offset = (base - (Trig_Sin(phase) << 2)) >> 10;
                    *scanline++ = offset;
                    phase += 0x800;
                }
            }
        }
        if (frame <= 2) {
            EffectPosition_ApplyStepAndYOffset(work->effect->actor, caster_screen);
            caster_screen->x = caster_screen->x / 2;
            caster_screen->y += 16;
        }
        if (kind != 11 && kind != 8 && kind != 32 && kind != 33 && kind != 34) {
            if (frame <= 11) {
                if (work->effect->side == 0) {
                    blitters[0](canvas, (u8 *)work + frame / 2 * 3456, caster_screen->x - 32, caster_screen->y - 40, 48, 72);
                } else {
                    blitters[0](canvas, (u8 *)work + frame / 2 * 3456, caster_screen->x, caster_screen->y - 40, 48, 72);
                }
            }
        }
        switch (kind) {
        case 33:
            BattleFx_StepPaletteToResource((s32)&Value_00000053);
            break;
        case 14:
            BattleFx_StepPaletteToResource((s32)&Value_0000006f);
            break;
        case 31:
            BattleFx_StepPaletteToResource((s32)&Value_00000079);
            break;
        case 8:
            BattleFx_StepPaletteToResource((s32)&Value_000000c3);
            break;
        case 0:
        case 10:
            BattleFx_StepPaletteToResource((s32)&Value_0000008d);
            break;
        case 12:
        case 13:
        case 25:
            BattleFx_StepPaletteToResource((s32)&Value_000000bb);
            break;
        case 18:
            BattleFx_StepPaletteToResource((s32)&Value_000000b9);
            break;
        case 19:
            BattleFx_StepPaletteToResource((s32)&Value_000000c0);
            break;
        case 2:
        case 29:
            BattleFx_StepPaletteToResource((s32)&Value_000000a4);
            break;
        case 1:
        case 28:
            BattleFx_StepPaletteToResource((s32)&Value_000000a3);
            break;
        case 3:
        case 20:
        case 22:
            BattleFx_StepPaletteToResource((s32)&Value_000000b4);
            break;
        case 9:
            BattleFx_StepPaletteToResource((s32)&Value_000000a0);
            break;
        case 5:
        case 23:
            BattleFx_StepPaletteToResource((s32)&Value_0000007d);
            break;
        }
        if (kind != 11 && kind != 8 && kind != 32) {
            if ((u32)(frame - 4) <= 11) {
                draw_pair[1](canvas, (u8 *)work + (frame - 4) / 2 * 960 + 0x5100, target_screen->x / 2 - 8, caster_screen->y - 24, 20, 48);
            }
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(camera, camera + 12);
            if (frame > 3) {
                for (i = 0; i != 128; i++) {
                    s32 index = i / 2;
                    struct EffectStep *step = &work->particles[index];
                    s32 size = step->variant;
                    if (size > 0) {
                        EffectPosition_ApplyBaseAndYOffset((s32 *)step, &projected);
                        /* Remaining lifetime determines the particle's image size. */
                        size >>= 4;
                        size++;
                        projected.x /= 2;
                        blitters[index & 1](canvas, sprites + ParticleStreams_CellOffsets[size - 1], projected.x - size / 2, projected.y - size, size, size * 2);
                        EffectStep_AdvanceWithGravity3D(step, 60, -0x1000);
                        step->variant--;
                    }
                }
            }
        }
        if (kind == 7 || kind == 13 || kind == 18 || kind == 19) {
            if (frame == 50) {
                ObjectGroup_UpdateMembers(work->effect->actor, 7, -1, -1, 0);
            }
            if (frame == 79) {
                ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
            }
            if (frame == 12) {
                particle = (struct EffectStep *)0x02014000;
                for (i = 0; i != 64; i++, particle++) {
                    particle->x = target_actor->x;
                    particle->y = 0x140000;
                    particle->z = target_actor->z;
                    particle->velocity_x = ((Random16() & 255) - 128) << 10;
                    particle->velocity_y = ((Random16() & 255) - 128) << 10;
                    particle->velocity_z = ((Random16() & 255) - 128) << 10;
                    particle->variant = 0;
                }
            }
            if (frame <= 11) {
                goto FinishFrame;
            }
            {
                struct EffectStep *step = (struct EffectStep *)0x02014000;
                struct MotionObject *caster;
                s32 height;
                caster = GetBattleObjectSlotFar(work->effect->actor)->object;
                height = (s32)Battle_GetObjectTableValueFar(work->effect->actor) / 2;
                for (i = 0; i != 32; i++, step++) {
                    if (step->variant >= 0) {
                        s32 size = (i & 1) + 6;
                        EffectPosition_ApplyBaseAndYOffset((s32 *)step, &projected);
                        projected.x >>= 1;
                        blitters[0](canvas, sprites + ParticleStreams_CellOffsets[size - 1], projected.x - (u32)size / 2, projected.y - size, size, size * 2);
                        EffectStep_AdvanceWithGravity3D(step, 62, 0);
                        if (frame > i + 22) {
                            s32 dx = (caster->x - step->x) >> 8;
                            s32 dy = (caster->y + height - step->y) >> 8;
                            s32 dz = (caster->z - step->z) >> 8;
                            step->velocity_x += dx;
                            step->velocity_y += dy;
                            step->velocity_z += dz;
                            if ((u32)(dx + 0xfff) <= 0x1ffe && (u32)(dz + 0xfff) <= 0x1ffe)
                                step->variant = -1;
                        }
                    }
                }
            }
            goto FinishFrame;
        }
        if (kind == 21) {
            goto FinishFrame;
        }
        if (kind == 6 || kind == 27) {
            if ((u32)(frame - 6) <= 13) {
                phase = frame;
                i = 0;
                do {
                    blitters[0](canvas, ((phase / 2) & 3) * 2880 + (IMAGE_WORK + 0xc56), target_screen->x / 2 - 8, 0, 24, 104);
                    i++;
                    phase += 3;
                } while (i != 2);
            }
            if ((u32)(frame - 8) > 15) {
                goto FinishFrame;
            }
            for (i = 0; i != 3; i++) {
                s32 image = i & 3;
                s32 angle = Random16() & 0xffff;
                s32 image_x;
                s32 image_y;
                s32 draw_flags;
                image_x = Trig_Sin(angle);
                image_x <<= 3;
                image_x >>= 16;
                image_x += target_screen->x / 2;
                image_x -= BattleFx12_SmokeWidths[image] / 2;
                image_y = Trig_Cos(angle);
                image_y <<= 5;
                image_y >>= 16;
                image_y -= BattleFx12_SmokeHeights[image] / 2;
                Runtime_ReleaseHeapBlock(47);
                Runtime_ReleaseHeapBlock(46);
                draw_flags = Random16();
                BattleEffect_LoadWork(47, 7, 7, CastingImpact_SmokeDrawFlags[draw_flags & 3] | 3, 2);
                ((RectangleBlit *)gWorkSlot)[47](canvas, IMAGE_WORK + BattleFx12_SmokeCells[image], image_x, image_y + 56, BattleFx12_SmokeWidths[image], BattleFx12_SmokeHeights[image]);
                Runtime_ReleaseHeapBlock(47);
                BattleFx_FetchRectangleBlitters(work->effect->side, blitters);
            }
            goto FinishFrame;
        }
        if (kind == 14) {
            s32 rise;
            s32 scroll;
            Runtime_ReleaseHeapBlock(47);
            Runtime_ReleaseHeapBlock(46);
            if ((u32)frame > 23) {
                goto RestoreBlitters;
            }
            origin_x = target_screen->x / 2;
            rise = frame * 32 - 232;
            scroll = frame * 16 - 48;
            if (rise > 0)
                rise = 0;
            while (scroll > 104)
                scroll -= 104;
            BattleEffect_LoadWork(47, 7, 7, 3, 2);
            /* FAKEMATCH: Share the drawing cursor with the paired-image branch. */
            work_blitters = (RectangleBlit *)gWorkSlot;
            work_blitters += 47;
            DrawRisingColumn(canvas, origin_x, rise, scroll, work_blitters);
            Runtime_ReleaseHeapBlock(47);
            if (frame == 8) {
                work->shake_frames = frame;
            }
            if (frame <= 1) {
                goto RestoreBlitters;
            }
            {
                s32 emitted = 0;
                struct EffectStep *step;
                for (i = 0; i != 64; i++) {
                    if ((step = &work->particles[i])->variant == 0) {
                        step->x = target_actor->x;
                        step->y = 0x140000;
                        step->z = target_actor->z;
                        step->velocity_x = ((Random16() & 255) - 127) << 12;
                        step->velocity_y = ((Random16() & 255) - 64) << 10;
                        /* FAKEMATCH: Finish velocity before deriving the particle lifetime. */
                        do {
                            step->velocity_z = ((Random16() & 255) - 127) << 12;
                        } while (0);
                        step->variant = i / 2 + 32;
                        emitted++;
                        if (emitted == 4)
                            break;
                    }
                }
            }
            goto RestoreBlitters;
        }
        if (kind == 31) {
            s32 height;
            Runtime_ReleaseHeapBlock(47);
            Runtime_ReleaseHeapBlock(46);
            if ((u32)(frame - 4) <= 19) {
                pair_x = target_screen->x / 2;
                BattleEffect_LoadWork(47, 7, 7, 3, 2);
                work_blitters = (RectangleBlit *)gWorkSlot;
                height = 48;
                DrawImage(canvas, IMAGE_WORK, pair_x - 24, 48, 24, height, work_blitters + 47);
                Runtime_ReleaseHeapBlock(47);
                BattleEffect_LoadWork(47, 7, 7, 7, 2);
                DrawImage(canvas, IMAGE_WORK, pair_x, 48, 24, height, work_blitters + 47);
                Runtime_ReleaseHeapBlock(47);
            }
        RestoreBlitters:
            ;
            BattleFx_FetchRectangleBlitters(work->effect->side, blitters);
            goto FinishFrame;
        }
        if (kind == 30) {
            s32 image;
            if (frame > 15) {
                *(volatile u16 *)0x04000052 = (0x20 - frame) | 0x1000;
            }
            if (frame <= 5) {
                goto FinishFrame;
            }
            strip_x = target_screen->x / 2 - 20;
            image = Math_Mod(frame / 2, 3);
            image_offset = image * 2560;
            blitters[0](canvas, IMAGE_WORK + 0xc56 + image_offset, strip_x, 16, 40, 32);
            blitters[0](canvas, image * 1280 + (IMAGE_WORK + 0x2a56), strip_x, 48, 40, 32);
            blitters[0](canvas, image_offset + (IMAGE_WORK + 0x1156), strip_x, 80, 40, 32);
            goto FinishFrame;
        }
        if (kind != 5) {
            if (kind != 23) {
                goto DrawLargeImages;
            }
        }
        {
            struct EffectStep *step = (struct EffectStep *)0x02014000;
            for (i = 0; i != 16; i++, step++) {
                if (frame >= i / 2 + 4) {
                    s32 age = step->variant;
                    if (age <= 11) {
                        s32 image = age / 2;
                        EffectPosition_ApplyBaseAndYOffset((s32 *)step, &projected);
                        projected.x /= 2;
                        blitters[0](canvas, IMAGE_WORK + (image << 11), projected.x - 16, projected.y - 32, 32, 64);
                        EffectStep_AdvanceWithGravity3D(step, 60, 0x1000);
                        step->variant++;
                    }
                }
            }
        }
        goto FinishFrame;
    DrawLargeImages:
        ;
        if (kind == 4)
            goto FinishFrame;
        if (kind == 11) {
            s32 image_y;
            s32 sway;
            Trig_Sin(frame << 9);
            sway = Trig_Cos(frame << 9);
            sway <<= 2;
            sway >>= 16;
            image_y = (target_screen->y >> 16) + sway + 16;
            if (frame <= 3) {
                blitters[0](canvas, work, CastingImpact_ImageX[work->effect->side * 7], image_y + CastingImpact_ImageY[0], 57, 98);
                goto FinishFrame;
            }
            if (frame <= 7) {
                blitters[0](canvas, work, CastingImpact_ImageX[work->effect->side * 7], image_y + CastingImpact_ImageY[0], 57, 98);
            }
            blitters[0](canvas, (u8 *)work + 0x15d2, CastingImpact_ImageX[work->effect->side * 7 + 1], image_y + CastingImpact_ImageY[1], 99, 69);
            if ((u32)(frame - 4) <= 1) {
                FillWords(canvas, 0x4000, 0x3f3f3f3f);
            }
            if ((u32)(frame - 6) <= 1) {
                blitters[0](canvas, (u8 *)work + 0x3081, CastingImpact_ImageX[work->effect->side * 7 + 2], image_y + CastingImpact_ImageY[2], 128, 91);
            }
            if ((u32)(frame - 8) <= 1) {
                DrawCroppedImage(canvas, IMAGE_WORK, CastingImpact_ImageX[work->effect->side * 7 + 3], image_y + CastingImpact_ImageY[3], 128, &blitters[0]);
            }
            if ((u32)(frame - 10) <= 1) {
                blitters[0](canvas, IMAGE_WORK + 0x2d80, CastingImpact_ImageX[work->effect->side * 7 + 4], image_y + CastingImpact_ImageY[4], 128, 59);
            }
            if ((u32)(frame - 12) <= 1) {
                blitters[0](canvas, IMAGE_WORK + 0x4b00, CastingImpact_ImageX[work->effect->side * 7 + 5], image_y + CastingImpact_ImageY[5], 122, 29);
            }
            if ((u32)(frame - 14) > 1) {
                goto FinishFrame;
            }
            blitters[0](canvas, IMAGE_WORK + 0x58d2, CastingImpact_ImageX[work->effect->side * 7 + 6], image_y + CastingImpact_ImageY[6], 76, 25);
            goto FinishFrame;
        } else if (kind == 32) {
            scroll_pos = scroll_pos + scroll_speed;
            if (frame > 6) {
                scroll_speed = (s32)((u32)scroll_speed * 48) / 64;
            }
            {
                s32 x = scroll_pos;
                x >>= 16;
                x <<= 8;
                *(volatile s32 *)0x04000028 = x;
            }
            {
                s32 fade;
                if ((u32)(fade = frame - 16) <= 15) {
                    *(volatile u16 *)0x04000052 = (16 - fade) | 0x1000;
                }
            }
            if ((u32)(frame - 4) <= 1) {
                FillWords(canvas, 0x4000, 0x3f3f3f3f);
            }
            if (frame <= 3) {
                if (work->effect->side == 1) {
                    blitters[0](canvas, work, 0, 24, 80, 104);
                } else {
                    blitters[0](canvas, work, 48, 24, 80, 104);
                }
                goto FinishFrame;
            }
            if (frame <= 7) {
                if (work->effect->side == 1) {
                    blitters[0](canvas, work, 0, 24, 80, 104);
                } else {
                    blitters[0](canvas, work, 48, 24, 80, 104);
                }
            }
            if (work->effect->side == 1) {
                blitters[0](canvas, (u8 *)work + 0x1e00, 16, 16, 80, 104);
            } else {
                blitters[0](canvas, (u8 *)work + 0x1e00, 32, 16, 80, 104);
            }
            if ((u32)(frame - 6) <= 1) {
                blitters[0](canvas, (u8 *)work + 0x3e80, 0, 16, 128, 91);
            }
            if ((u32)(frame - 8) <= 1) {
                DrawCroppedImage(canvas, IMAGE_WORK, 0, 16, 128, &blitters[0]);
            }
            if ((u32)(frame - 10) <= 1) {
                blitters[0](canvas, IMAGE_WORK + 0x2d80, 0, 16, 128, 59);
            }
            if ((u32)(frame - 12) <= 1) {
                blitters[0](canvas, IMAGE_WORK + 0x4b00, 0, 16, 128, 29);
            }
            if ((u32)(frame - 14) > 1) {
                goto FinishFrame;
            }
            blitters[0](canvas, IMAGE_WORK + 0x5980, 0, 16, 128, 26);
            goto FinishFrame;
        } else if (kind == 20) {
            for (i = 0; i != 12; i++) {
                if (frame >= i + 6 && frame < i + 18) {
                    s32 image = (frame - i - 6) / 2;
                    s32 image_x = target_screen->x / 2 - PuffArc_CellWidths[image] / 2;
                    s32 mirrored;
                    if (i & 1)
                        image_x += (1 + i) / 2 * 3;
                    else
                        image_x -= (i + 1) / 2 * 3;
                    mirrored = 1;
                    if (i != 0) {
                        mirrored = 0;
                        if (((i - 1) & 3) > 1)
                            mirrored = 1;
                    }
                    blitters[mirrored](canvas, IMAGE_WORK + PuffArc_CellSourceOffsets[image], image_x, PuffArc_CellBiasY[image] + 48, PuffArc_CellWidths[image], PuffArc_CellHeights[image]);
                }
            }
        } else if (kind == 16) {
            if (frame == 0) {
                for (i = 0; i != 64; i++) {
                    particle = &((struct EffectStep *)0x02014000)[i];
                    particle->x = (Random16() & 127) + 32;
                    particle->y = 0;
                    particle->z = 0;
                    particle->velocity_x = Random16() & 0xffff;
                    particle->velocity_y = Random16() & 0xffff;
                    particle->velocity_z = Random16() & 0xffff;
                }
                ((struct EffectStep *)0x02014000)[63].y = 159;
            }
            {
                struct EffectStep *step = (struct EffectStep *)0x02014000;
                /* FAKEMATCH: The wide snapshot keeps this origin in a saved
                   * register; a pointer alias merges with the spilled pointer. */
                u64 origin_addr = (u32)target_screen;
                for (i = 0; i != 64; i++, step++) {
                    if (step->x >= 0 && frame >= i / 2) {
                        s32 image = i & 3;
                        Render_ResetTransformState();
                        SceneTransform_ApplyPitch(step->velocity_x);
                        SceneTransform_ApplyYaw(step->velocity_y);
                        EffectPosition_ApplyBaseAndYOffset((s32 *)step, &projected);
                        projected.x = projected.x / 2 + ((struct EffectPosition *)(u32)origin_addr)->x / 2;
                        projected.y += ((struct EffectPosition *)(u32)origin_addr)->y + 32;
                        blitters[1](canvas, IMAGE_WORK + CastingImpact_OrbitCells[image], projected.x - 4, projected.y - 4, 8, 8);
                        step->x -= 6;
                        if (step->x < 0 && ((i & 7) == 0 || i == 63)) {
                            Audio_PlayCue(133);
                            ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
                        }
                    }
                }
            }
            goto FinishFrame;
        } else if (kind == 8) {
            s32 height;
            if ((u32)(frame - 5) > 44) {
                goto FinishFrame;
            }
            if (frame > 25) {
                height = 196 - (frame << 2);
            } else {
                height = (frame << 4) - 64;
            }
            if (height > 96) {
                height = 96;
            }
            DrawImage(canvas, IMAGE_WORK, 48, 104 - height, 32, height, &blitters[0]);
        } else if ((u32)(kind - 33) <= 1) {
            s32 offset;
            s32 image_x;
            s32 image_y;
            if (frame > 5)
                goto FinishFrame;
            /* FAKEMATCH: Calculate each center before the shared centered draw;
               * spell signed halving as an in-place rounding bias and shift. */
            if (work->effect->side == 0) {
                image_x = target_screen->x;
                image_x += (u32)image_x >> 31;
                image_x >>= 1;
                offset = (6 - frame) * 3;
                image_x += offset * 2;
                image_y = target_screen->y - offset * 4 + 24;
            } else {
                image_x = target_screen->x;
                image_x += (u32)image_x >> 31;
                image_x >>= 1;
                offset = (6 - frame) * 3;
                image_x -= offset * 2;
                image_y = target_screen->y - offset * 4 + 24;
            }
            DrawTallImage(canvas, IMAGE_WORK, image_x - 16, image_y - 32, 32, &blitters[1]);
        } else if (kind == 12) {
            if (frame > 47) {
                *(volatile u16 *)0x04000052 = (0x40 - frame) | 0x1000;
            }
            {
                struct EffectStep *step = (struct EffectStep *)0x02014000;
                s32 mask;
                /* FAKEMATCH: Initialize the mask after the loop counter. */
                for (i = 0, mask = 3; i != 16; i++, step++) {
                    s32 image = Math_Mod(i, 3);
                    EffectPosition_ApplyBaseAndYOffset((s32 *)step, &projected);
                    projected.x /= 2;
                    blitters[i & 1](canvas, IMAGE_WORK + image * 576, projected.x - 12, projected.y - 12, 24, 24);
                    EffectStep_AdvanceWithGravity3D(step, 60, 1 << ((i & mask) + 11));
                    step->variant++;
                }
            }
        } else if (kind != 100) {
            struct EffectStep *step = (struct EffectStep *)0x02014000;
            for (i = 0; i != 16; i++, step++) {
                if (frame >= i + 4) {
                    s32 age = step->variant;
                    if (age <= 23) {
                        s32 image = age / 4;
                        EffectPosition_ApplyBaseAndYOffset((s32 *)step, &projected);
                        projected.x /= 2;
                        blitters[i & 1](canvas, IMAGE_WORK + image * 1152, projected.x - 12, projected.y - 24, 24, 48);
                        if (kind == 25)
                            EffectStep_AdvanceWithGravity3D(step, 60, 0x400);
                        else
                            EffectStep_AdvanceWithGravity3D(step, 60, 0x1000);
                        step->variant++;
                    }
                }
            }
        }
    FinishFrame:
        ;
        if (kind <= 7) {
            if (frame <= 5) {
                EffectPosition_ApplyBaseAndYOffset((s32 *)position, &projected);
                orb_x = projected.x;
                /* FAKEMATCH: Keep signed halving in the loaded coordinate register. */
                orb_x += (u32)orb_x >> 31;
                orb_x >>= 1;
                projected.x = orb_x;
                draw_pair[1](canvas, IMAGE_WORK + 0x3c56, orb_x - 10, projected.y - 4, 20, 40);
                position->x += motion->x;
                position->y += motion->y;
                position->z += motion->z;
            }
        }
        if (frame == 3) {
            BattleEventRuntime_BeginPhaseFar(-1);
        }
        if (frame == 4) {
            Audio_PlayCue(134);
        }
        if (frame == 6) {
            if ((u32)kind_from_four > 1 && kind != 7 && kind != 13 && kind != 18 && kind != 19 && kind != 23 && kind != 34 && kind != 100) {
                goto SelectActorMotion;
            }
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
            shake_addr = &work->shake_frames;
            goto SetShake8;
        SelectActorMotion:
            ;
            if (kind == 20 || kind == 14 || kind == 33) {
                BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
                shake_addr = &work->shake_frames;
                *shake_addr = 2;
                goto ActorMotionDone;
            }
            if (kind != 30) {
                if (kind != 8) {
                    goto ActorMotionDone;
                }
            }
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 3);
            shake_addr = &work->shake_frames;
        SetShake8:
            ;
            *shake_addr = 8;
        ActorMotionDone:
            ;
        }
        if (frame == 6) {
            ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
        }
        if (frame == 14) {
            ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
        }
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
        frame++;
    }
    /* Hand off the follow-up effect, or release the canvas layer. */
    if (kind == 21) {
        ClearWords((void *)0x06004000, 0x4000);
        ClearWords(canvas, 0x4000);
        work->effect->unknown_001c = 0;
        Scheduler_RemoveCallback((u32)Palette_StepFadeTransfer);
        Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
        Runtime_ReleaseHeapBlock(47);
        Runtime_ReleaseHeapBlock(46);
        BattleFx_RunPaletteRampMode1((s32)command);
    } else {
        if ((u32)kind_from_two <= 1 || kind == 12 || kind == 22 || kind == 28 || kind == 29) {
            Scheduler_RemoveCallback((u32)BattleFx_ArmBg2AffineHBlankDma);
        }
        Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
        Runtime_ReleaseHeapBlock(47);
        Runtime_ReleaseHeapBlock(46);
        BattleFx_EndCanvasLayer();
    }
}
