/* NONMATCHING: current namespace/scalar repair, measured 2026-10-01.
 * Source hypothesis: Use the current IceShardSheet and SparkleDots resource rows and an ordinary 639 particle clipping threshold.
 * EN: 1340/1360 complete linked bytes, 768 differing byte positions, first +0x5b, including the complete literal pool.
 * All six ordinary TBS targets compile; text extents in bytes:
 * JA 1340; EN 1340; DE 1340; ES 1340; FR 1340; IT 1340.
 * The other five editions have no complete linked proof in this attempt.
 * Existing raw numeric callbacks/data pointers and provisional field views
 * still need owning source declarations before adoption. This is an
 * uncredited S4 draft; compiler options and production routing are unchanged.
 */
#include "BATTLE_TYPES.H"
#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "RESOURCE_IDS.H"


struct EffectArgument {
    u8 unknown_00[4];
    s32 direction;
    s32 source_id;
    u8 unknown_0c[8];
    s32 target_count;
    s32 variant;
    u8 unknown_1c[8];
    s16 target_ids[BATTLE_TARGET_CAPACITY];
};

struct Particle {
    s32 x;
    s32 y;
    u8 unknown_08[16];
    s32 frame;
};

struct EffectRuntime {
    u8 unknown_0000[0x7080];
    struct Particle falling[32];
    struct Particle bursts[32];
    s32 display_mode;
    s32 display_value;
    u8 unknown_7788[0x20];
    s32 impact_mode;
    u8 unknown_77ac[0x78];
    s32 frame_ready;
    struct EffectArgument *argument;
};

struct RuntimeCells {
    struct EffectRuntime *runtime;
    void *draw_destination;
    u8 *graphics;
};

struct DrawRegistry {
    u8 unknown_00[184];
    DrawRectangle rectangles[2];
};

extern struct RuntimeCells gBattleFxWork;
extern struct DrawRegistry gWorkSlot;
extern u16 BattleFx_PuffCells[];
extern u8 BattleFx_PuffSizes[];
extern u8 Data_080eded6[];

void BattleFx_BeginCanvasLayer(s32 mode);
void BattlePres_ConfigureEffectDisplay(void);
s32 Random16(void);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 interval);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleEventRuntime_BeginPhaseFar(s32 value);
s32 __divsi3(s32 numerator, s32 denominator);
void Audio_PlayCue(s32 value);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);
void ObjectGroup_TickMemberTimers(void);
void Camera_ApplyShake(s32, s32);
void WaitFrames(s32 frames);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 resource_id);
void BattleFx_EndCanvasLayer(void);

void BattleEffect_RunFallingParticles(struct EffectArgument *argument)
{
    struct RuntimeCells *cells;
    void **cell;
    struct EffectRuntime *runtime;
    struct Particle *particle;
    struct Particle *burst;
    DrawRectangle rectangles[2];
    DrawRectangle *draw_functions;
    void *draw_destination;
    u16 *map;
    s32 particle_index;
    s32 burst_index;
    s32 frame;
    s32 x_offset;
    s32 y;
    s32 color;
    u8 *graphics;

    cells = &gBattleFxWork;
    cell = (void **)cells;
    runtime = *cell++;
    draw_destination = *cell;
    graphics = cells->graphics;
    runtime->argument = argument;

    BattleFx_BeginCanvasLayer(0x2001);
    *(u16 *)0x04000020 = 0x100;
    Resource_LoadAndDecompress(&ResourceId_IceShardSheet, runtime, 1, 1);
    Resource_LoadAndDecompress(&ResourceId_SparkleDots, graphics, 0, 0);
    BattlePres_ConfigureEffectDisplay();
    *(u16 *)0x04000050 = 0x3f44;
    *(u16 *)0x04000048 = 0x3337;

    particle_index = 0;
    y = -128;
    x_offset = -16;
    particle = runtime->falling;
    do {
        s32 x;

        x = (Random16() & 0x3f)
            + (Random16() & 7)
            + 24;
        if (runtime->argument->direction == 1)
            x += x_offset + 24;
        else
            x += -x_offset + 80;
        particle->y = y;
        particle->x = x * 8;
        particle_index++;
        y -= 64;
        particle->frame = -1;
        x_offset -= 8;
        particle++;
    } while (particle_index != 32);

    particle_index = 0;
    do {
        runtime->bursts[particle_index].frame = -1;
        particle_index++;
    } while (particle_index != 32);

    if (runtime->argument->direction == 0) {
        BattleEffect_LoadWork(46, 7, 7, 2, 2);
        BattleEffect_LoadWork(47, 7, 7, 2, 3);
    } else {
        BattleEffect_LoadWork(46, 7, 7, 6, 2);
        BattleEffect_LoadWork(47, 7, 7, 6, 3);
    }

    rectangles[0] = gWorkSlot.rectangles[0];
    rectangles[1] = gWorkSlot.rectangles[1];
    draw_functions = rectangles;

    if (runtime->argument->direction == 0) {
        map = (u16 *)0x02010000;
        particle_index = 0;
        color = 0x7000;
        do {
            if ((u32)(particle_index - 8) <= 95)
                *map = (0xf0 - particle_index) | color;
            else if (particle_index <= 135)
                *map = 0x888;
            else
                *map = 0x100;
            particle_index++;
            map++;
            color -= 0x100;
        } while (particle_index != 160);
    } else {
        map = (u16 *)0x02010000;
        particle_index = 0;
        color = 0x1800;
        do {
            if ((u32)(particle_index - 8) <= 87)
                *map = (particle_index - 8 + 160) | color;
            else if (particle_index <= 135)
                *map = 0x78f8;
            else
                *map = 0x100;
            particle_index++;
            map++;
            color += 0x100;
        } while (particle_index != 160);
    }

    Scheduler_AddOrUpdateCallback((void (*)(void))0x080c91a5, 0x480);
    runtime->display_mode = 2;
    if (runtime->argument->variant == 1)
        runtime->display_value = 75;
    else
        runtime->display_value = 50;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    frame = 0;
    if (Data_080eded6[runtime->argument->variant * 2 + 1] != 0) {
        do {
            if (frame
                == Data_080eded6[runtime->argument->variant * 2 + 1] - 16) {
                BattleEventRuntime_BeginPhaseFar(132);
            }

            particle_index = 0;
            if (Data_080eded6[runtime->argument->variant * 2] != 0) {
                particle = runtime->falling;
                do {
                    if (particle->frame == -1) {
                        s32 draw_x;
                        s32 draw_y;
                        DrawRectangle draw;

                        draw_x = particle->x / 8;
                        draw_y = particle->y / 8;
                        draw = draw_functions[runtime->argument->variant == 2];
                        draw(
                            draw_destination,
                            runtime,
                            draw_x,
                            draw_y,
                            32,
                            32);
                        if (particle->y <= 639) {
                            if (runtime->argument->direction == 0)
                                particle->x -= 64;
                            else
                                particle->x += 64;
                            particle->y += 64;
                        } else {
                            if ((particle_index & 3) == 0)
                                Audio_PlayCue(115);
                            runtime->impact_mode = 2;
                            particle->frame = 0;

                            burst_index = 0;
                            if (runtime->argument->target_count != 0) {
                                do {
                                    ObjectGroup_UpdateMembers(
                                        runtime->argument->target_ids[burst_index],
                                        9,
                                        5,
                                        burst_index,
                                        8);
                                    burst_index++;
                                } while (burst_index
                                    != runtime->argument->target_count);
                            }
                        }
                    }

                    if (particle->frame != -1) {
                        s32 draw_x;
                        s32 draw_y;

                        draw_x = particle->x / 8;
                        draw_y = particle->y / 8;
                        if ((u32)(particle->frame - 1) <= 13) {
                            s32 draw_index;
                            const void *image;
                            DrawRectangle draw;

                            draw_index = runtime->argument->variant == 2;
                            image = (u8 *)runtime + 0x400
                                + (__divsi3(particle->frame, 3) << 10);
                            draw = draw_functions[draw_index];
                            draw(
                                draw_destination,
                                image,
                                draw_x,
                                draw_y,
                                32,
                                32);
                        }

                        if ((u32)(particle->frame - 9) <= 2) {
                            burst_index = 0;
                            burst = runtime->bursts;
                            do {
                                if (burst->frame == -1) {
                                    s32 burst_y;

                                    burst->frame = 18;
                                    burst->x = ((Random16() & 31)
                                        + particle->x / 8) * 8 + 8;
                                    burst_y = (Random16() & 15)
                                        + particle->y / 8 - 15;
                                    burst->y = burst_y * 8;
                                    break;
                                }
                                burst_index++;
                                burst++;
                            } while (burst_index != 32);
                        }

                        if (particle->frame <= 14)
                            particle->frame++;
                    }
                    particle_index++;
                    particle++;
                } while (particle_index
                    != Data_080eded6[runtime->argument->variant * 2]);
            }

            particle_index = 0;
            burst = runtime->bursts;
            do {
                if (burst->frame != -1) {
                    if (burst->frame <= 17) {
                        s32 image;
                        s32 size;
                        s32 half_size;
                        s32 draw_x;
                        s32 draw_y;
                        DrawRectangle draw;

                        image = burst->frame / 2;
                        draw_x = burst->x / 8;
                        size = BattleFx_PuffSizes[image];
                        half_size = (u32)size >> 1;
                        draw_x -= half_size;
                        draw_y = burst->y / 8 - half_size;
                        draw = draw_functions[runtime->argument->variant == 2];
                        draw(
                            draw_destination,
                            graphics + BattleFx_PuffCells[image],
                            draw_x,
                            draw_y,
                            size,
                            size);
                    }
                    if (burst->frame > -1)
                        burst->frame--;
                }
                particle_index++;
                burst++;
            } while (particle_index != 32);

            ObjectGroup_TickMemberTimers();
            Camera_ApplyShake(4, 4);
            runtime->frame_ready = 1;
            WaitFrames(1);
            frame++;
        } while (frame
            != Data_080eded6[runtime->argument->variant * 2 + 1]);
    }

    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Scheduler_RemoveCallback((void (*)(void))0x080c91a5);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
    BattlePres_ConfigureEffectDisplay();
}
