#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"

/*
 * Battle-presentation sub-effect at 0x080da2ac, sharing the 0x03001eec
 * "battle work" subsystem shape with games/THE BROKEN SEAL/src/battle/effects/
 * member_orbit/run.c (owner 080ce85c, the assigned family template) and
 * recon/tbs/en/main/080d82b0.c / 080e01e4.c / 080e7404.c, but a
 * genuinely different sub-effect: a variant-driven shrinking/expanding bar
 * wipe read from a per-variant 3-byte table at 0x080eea38 (iteration
 * count, threshold extra, and a trailing id), not an orbiting-sprite
 * animation.  Field offsets (work+0x7780/0x7784/0x7824/0x7828), the
 * (*(type_ptr)((u8 *)(expr) + (offset))) macro, the DrawRectangleFn/WordCopyFn
 * typedefs, and the heap_cache/cursor prologue idiom are all established
 * by that family.
 */

typedef s32 (*WordCopyFn)(void *dest, void *src, s32 size);

typedef struct {
    s32 a, b, c, d;
} Bounds4;

extern const u8 Data_080eea38[];
extern const Bounds4 Data_080eda88;

void BattleFx_BeginCanvasLayer(s32 mode);
void _call_via_r3(void *dest, void *src, s32 size, WordCopyFn copier);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 interval);
void Audio_PlayCue(s32 id);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void BattleEventRuntime_BeginPhaseFar(s32 id);
void **GetBattleObjectSlotFar(s32 member_id);
void EffectPosition_ApplyBaseAndYOffset(void *source, void *screen);
s32 Trig_Sin(s32 angle);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Camera_ApplyShake(s32 a, s32 b);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);
void Scheduler_RemoveCallback(void *callback);
void Runtime_ReleaseHeapBlock(s32 id);
s32 BattleFx_EndCanvasLayer(void);

void Func_080da2ac(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    DrawRectangleFn draw_rectangle_fn;
    s32 facing;
    s32 y_offset;
    s32 variant;
    s32 count;
    s32 frame;
    s32 pass;
    s32 lower_x;
    s32 upper_x_base;
    s32 limit;
    s32 band;
    s32 b_arg;
    s32 bounds[4];
    s32 *bounds_ptr;
    s32 position[3];
    s32 screen[3];
    s32 i;
    s32 j;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    facing = *(s32 *)((u8 *)heap_cache - 108);
    (*(void **)((u8 *)(work) + 0x7828)) = object;
    BattleFx_BeginCanvasLayer(1);
    (*(s16 *)((u8 *)((void *)0x04000020) + 0)) = 0x100;
    (*(s16 *)((u8 *)((void *)0x04000020) + 0x30)) = 0;
    Resource_LoadAndDecompress((s32)&ResourceId_RockWallSheet, (u8 *)work + 0x1E00, 1, 1);
    _call_via_r3(
        (void *)0x02010000, (void *)0x06008000, 0x8000,
        (WordCopyFn)0x03001388);

    for (i = 0; i != 16; i++) {
        s32 row;
        u8 *dest;

        row = i + 0x60;
        dest = (u8 *)work + i * 40;
        for (j = 0; j != 40; j++) {
            s32 col;

            col = ((j + 0x20) & 7) + (((j + 0x20) / 8) << 6);
            *dest = *((const u8 *)0x02010000 + col + ((row / 8) << 11));
            dest++;
        }
    }

    if ((*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 4)) == 1) {
        *(s32 *)0x04000028 = 0xFFFF9000;
        y_offset = -112;
    } else {
        y_offset = 0;
    }

    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    draw_rectangle_fn = *(DrawRectangleFn *)((u8 *)heap_cache + 28);

    count = (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 0x14));
    if (count != 0) {
        s32 *record;
        

        record = (s32 *)((u8 *)work + 0x7098);
        i = 0;
        do {
            *record = 0;
            record += 7;
            i++;
        } while (i != count);
    }

    (*(s32 *)((u8 *)(work) + 0x7780)) = 1;
    (*(s32 *)((u8 *)(work) + 0x7784)) = 0;
    Scheduler_AddOrUpdateCallback((void *)0x080CD261, 0x480);

    *(Bounds4 *)bounds = Data_080eda88;
    bounds_ptr = bounds;
    (*(s32 *)((u8 *)(work) + 0x77A8)) = 0x80;
    Audio_PlayCue(141);

    variant = (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 0x18));
    count = Data_080eea38[variant * 3];
    frame = 0;
    if (count != 0) {
        b_arg = facing + 12;

        do {
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(facing, b_arg);

            variant = (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 0x18));
            if (frame == Data_080eea38[variant * 3] - 16) {
                BattleEventRuntime_BeginPhaseFar(133);
            }

            lower_x = 0;
            upper_x_base = 8;
            limit = 16;
            band = 0;
            pass = 0;
            do {
                if (((frame & 0x1F) + 0x20) == limit) {
                    *(s32 *)((u8 *)bounds_ptr + band) += 0x20;
                }

                if (frame >= limit) {
                    variant = (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 0x18));
                    if (frame
                            < limit
                                + Data_080eea38[variant * 3 + 1]) {
                        s32 half_size;
                        s32 min_y;

                        half_size = (*(s32 *)((u8 *)bounds_ptr + band)
                            * Trig_Sin((frame - limit) << 10)) >> 16;
                        if (half_size < 0) {
                            half_size = -half_size;
                        }
                        min_y = 0x70 - half_size;
                        draw_rectangle_fn(
                            draw_destination, (u8 *)work + 0x1E00,
                            upper_x_base, min_y, 40, half_size);
                        draw_rectangle_fn(
                            draw_destination, work, upper_x_base,
                            0x60 - half_size, 40, 16);

                        count = (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 0x14));
                        if (count != 0) {
                            s32 member_id_offset;
                            s32 lower_bound;
                            s32 upper_bound;
                            

                            lower_bound = lower_x;
                            upper_bound = upper_x_base + 40;
                            member_id_offset = 36;
                            i = 0;
                            do {
                                void *member_object;
                                s32 member_id;

                                member_id = (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + (member_id_offset)));
                                member_object = *GetBattleObjectSlotFar(member_id);
                                position[0] = (*(s32 *)((u8 *)(member_object) + 8));
                                position[1] = (*(s32 *)((u8 *)(member_object) + 12));
                                position[2] = (*(s32 *)((u8 *)(member_object) + 16));
                                EffectPosition_ApplyBaseAndYOffset(position, screen);
                                screen[0] += y_offset;
                                if (screen[0] >= lower_bound + 8
                                        && screen[0] <= upper_bound
                                        && screen[1] >= min_y) {
                                    (*(s32 *)((u8 *)(member_object) + 0x28)) = 0xC0000;
                                    (*(s32 *)((u8 *)(member_object) + 0x48)) = 0xAB85;
                                }
                                if ((*(s32 *)((u8 *)(member_object) + 12)) < 0) {
                                    member_id = (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + (member_id_offset)));
                                    ObjectGroup_UpdateMembers(member_id, 0, 5, -1, 0);
                                }
                                member_id_offset += 2;
                                i++;
                                count = (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 0x14));
                            } while (i != count);
                        }
                    }
                }

                lower_x += 40;
                upper_x_base += 40;
                limit += 4;
                band += 4;
                pass++;
            } while (pass != 3);

            count = (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 0x14));
            if (count != 0) {
                s32 *record;
                s32 member_id_offset;
                

                record = (s32 *)((u8 *)work + 0x7080);
                member_id_offset = 36;
                i = 0;
                do {
                    void *member_object;
                    s32 member_id;

                    member_id = (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + (member_id_offset)));
                    member_object = *GetBattleObjectSlotFar(member_id);
                    if (record[6] == 0 && (*(s32 *)((u8 *)(member_object) + 12)) <= 0
                            && (*(s32 *)((u8 *)(member_object) + 0x28)) < 0) {
                        record[6] = 1;
                        member_id = (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + (member_id_offset)));
                        ObjectGroup_UpdateMembers(member_id, 7, 5, i, 5);
                    }
                    member_id_offset += 2;
                    record += 7;
                    i++;
                    count = (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 0x14));
                } while (i != count);
            }

            variant = (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 0x18));
            Camera_ApplyShake(
                Data_080eea38[variant * 3 + 2],
                Data_080eea38[variant * 3 + 2]);
            ObjectGroup_TickMemberTimers();
            (*(s32 *)((u8 *)(work) + 0x7824)) = 1;
            WaitFrames(1);
            frame++;

            variant = (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 0x18));
        } while (frame != Data_080eea38[variant * 3]);
    }

    Scheduler_RemoveCallback((void *)0x080CD261);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
