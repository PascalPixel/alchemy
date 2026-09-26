#include "B5_CONTEXT.H"
#include "BATTLE_TYPES.H"
#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFFECT_WORK.H"

/* NONMATCHING: main [080d1350,080d1714), 964 bytes including 64-byte pool.
 * H1: registered resource/copy/decoder/scheduler interfaces preserve the
 * baseline bytes: candidate 940, 414 differing halfwords, 204 aligned edits.
 * H1 kept a 64-byte frame but misplaced runtime/destination/frame/screen
 * at sp+44/+40/+36/+48 rather than +48/+44/+40/+52.
 * H2: shared EffectStep, EffectPosition and two-entry rectangle cache emit
 * candidate 944, 412 differing halfwords, 193 aligned edits. Frame size and
 * all four slots now match. Cached phase still suppresses the reference's
 * second variant read; setup register roles, trail indexing, target loop
 * and final pool length/order still differ. No exact bytes or credit.
 * Score: alchemy score this file --owner main:080d1350 --size 964 --align.
 * Default registered symbols suffice; --reference-symbols is inadmissible
 * while the non-relocation core differs. No scoring TU or aliases required.
 * Bindings: RuntimeCells=03001eec, palette id=00000079, tiles id=00000073,
 * cell offsets=080ede48, radii=080ee158, trail work=02010000;
 * WordCopy=03001388 via _call_via_r3=080072f0; blit via r5=080072f8.
 * Resource_GetTableEntry=08002f40; Resource_DecodeType01=08005340;
 * Scheduler_AddOrUpdateCallback=080041d8; Scheduler_RemoveCallback=08004278;
 * BattlePresentation_ProcessPendingGraphicsTransfer=080cd260 (Thumb +1);
 * GetBattleObjectSlotFar=080b5098; Math_Div/Mod=080022ec/080022fc;
 * Trig_Cos/Sin=0800231c/08002322; Random16=08004458;
 * BattleFx_BeginCanvasLayer=080cd594; BattleFx_EndCanvasLayer=080cdbc0;
 * Graphics_UpdatePhasePalette=080d40ec; BattleEventRuntime_BeginPhaseFar=080b50e8;
 * Render_ResetTransformState=080049ac; Graphics_PrepareTransferInIwramWork=080051d8;
 * EffectPosition_ApplyBaseAndYOffset=080e3944; Audio_PlayCue=080f9010;
 * ObjectGroup_UpdateMembers=080d6888; ObjectGroup_TickMemberTimers=080cd52c;
 * Camera_ApplyShake=080e155c; WaitFrames=080030f8; Runtime_ReleaseHeapBlock=08002dd8;
 * BattleEffect_LoadWork=080ed408 through the existing reviewed BATTLE_EFX.H.
 */

typedef s32 (*WordCopy)(void *destination, const void *source, s32 size);

struct Position3d {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct EffectRuntime {
    u8 unknown_0000[0x7080];
    struct EffectStep points[64];
    s32 display_mode;
    s32 display_value;
    u8 unknown_7788[0x20];
    s32 impact_mode;
    u8 unknown_77ac[0x78];
    s32 frame_ready;
    struct BattleEffectArgument *argument;
};

struct RuntimeCells {
    struct EffectRuntime *runtime;
    void *draw_destination;
    u8 *graphics;
    u8 unknown_0c[16];
    DrawRectangle draw_rectangle;
};

extern struct RuntimeCells Data_03001eec;
extern u8 Data_00000073[];
extern u8 Data_00000079[];
extern u16 Data_080ede48[];
extern u8 Data_080ee158[];
extern struct EffectStep Data_02010000[];

void BattleFx_BeginCanvasLayer(s32 mode);
u32 Resource_GetTableEntry(u32 resource_id);
s32 Resource_DecodeType01(const void *source, void *destination);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void Graphics_UpdatePhasePalette(s32 frame, s32 red_phase, s32 green_phase, s32 blue_phase);
void BattleEventRuntime_BeginPhaseFar(s32 value);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 source, s32 destination);
void Audio_PlayCue(s32 value);
void Camera_ApplyShake(s32 random_mask, u32 shake_range);
void ObjectGroup_TickMemberTimers(void);
void BattleFx_EndCanvasLayer(void);
struct B5Context *GetBattleObjectSlotFar(s32 id);

void Unnamed_080d1350(struct BattleEffectArgument *argument)
{
    struct RuntimeCells *cells;
    struct EffectRuntime *runtime;
    struct Position3d *source;
    struct Position3d *target;
    struct B5Context *context;
    struct EffectStep *point;
    void *draw_destination;
    s32 frame;
    s32 vertex;
    s32 target_index;
    /* FAKEMATCH: entry 1 is unused; the family cache preserves stack slots. */
    DrawRectangle draw_rectangle[2];
    s32 point_index;
    u8 *graphics;
    void *view;
    s32 trail_base;
    struct EffectStep *trail;
    struct BattleEffectArgument **argument_cell;
    void **cell_cursor;

    cells = &Data_03001eec;
    cell_cursor = (void **)cells;
    runtime = *cell_cursor++;
    draw_destination = *cell_cursor;
    graphics = cells->graphics;
    view = *(void **)((u8 *)cells - 108);
    argument_cell = &runtime->argument;
    *argument_cell = argument;

    BattleFx_BeginCanvasLayer(1);
    ((WordCopy)0x03001388)(
        (void *)0x05000000,
        (const void *)Resource_GetTableEntry((u32)Data_00000079),
        0x80);
    Resource_DecodeType01((const void *)Resource_GetTableEntry((u32)Data_00000073), graphics);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);

    runtime->display_mode = 2;
    runtime->display_value = 50;
    draw_rectangle[0] = cells->draw_rectangle;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    context = GetBattleObjectSlotFar((*argument_cell)->actor);
    source = context->object;
    context = GetBattleObjectSlotFar((*argument_cell)->actors[0]);
    target = context->object;

    point_index = 0;
    point = runtime->points;
    do {
        point->x = source->x / 2;
        point->y = source->y + 0x780000;
        point->z = source->z;
        point->velocity_x = Math_Div(
            target->x + (((Random16() & 0x7f) - 0x40) << 16)
                - point->x,
            12);
        point->velocity_y = Math_Div(
            target->y - point->y + 0x140000,
            12);
        point->velocity_z = Math_Div(target->z - point->z, 12);
        point->variant = (Random16() & 0xf) + point_index * 8;
        point_index++;
        point++;
    } while (point_index != 8);

    frame = 0;
    do {
        Graphics_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
        if (frame == 96)
            BattleEventRuntime_BeginPhaseFar(134);

        point_index = 0;
        trail_base = 0;
        point = runtime->points;
point_loop:
        if (frame < point->variant)
            goto next_point;
        {
                struct EffectPosition screen;

                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork((s32)view, (s32)view + 12);
                EffectPosition_ApplyBaseAndYOffset(&point->x, &screen);
                screen.x >>= 1;

                if ((u32)(screen.x + 8) <= 135) {
                    if (screen.y <= 127) {
                        if (screen.y >= -8) {
                    vertex = 0;
                    trail = &Data_02010000[point_index * 10];
                    do {
                        s32 angle;

                        angle = vertex * 0x199a
                            - ((frame - point->variant) << 11);
                        trail->velocity_x = screen.x
                            + ((Data_080ee158[vertex & 1]
                                * Trig_Sin(angle)) / 2 >> 16);
                        trail->velocity_y = screen.y
                            - (Data_080ee158[vertex & 1]
                                * Trig_Cos(angle) >> 16);
                        vertex++;
                        trail++;
                    } while (vertex != 10);

                    vertex = 0;
                    do {
                        struct EffectStep *current;
                        struct EffectStep *next;
                        s32 step;

                        current = &((struct EffectStep *)0x02010000)[
                            trail_base + vertex];
                        next = &((struct EffectStep *)0x02010000)[
                            trail_base + Math_Mod(vertex + 1, 10)];
                        step = 0;
                        do {
                            s32 x;
                            s32 y;

                            x = current->velocity_x;
                            x += Math_Div(
                                step * (next->velocity_x - x), 12);
                            y = current->velocity_y;
                            y += Math_Div(
                                step * (next->velocity_y - y), 12);
                            draw_rectangle[0](
                                draw_destination,
                                graphics + Data_080ede48[1],
                                x - 1,
                                y - 2,
                                2,
                                4);
                            step++;
                        } while (step != 12);
                        vertex++;
                    } while (vertex != 10);
                        }
                    }
                }

                if (point->y <= 0x1dffff) {
                    point->velocity_y = -point->velocity_y;
                    point->velocity_x /= 2;
                    point->velocity_z /= 2;
                    runtime->impact_mode = 4;
                    Audio_PlayCue(134);

                    target_index = 0;
                    if (runtime->argument->count != 0) {
                        do {
                            ObjectGroup_UpdateMembers(
                                runtime->argument->actors[target_index],
                                7,
                                5,
                                target_index,
                                8);
                            target_index++;
                        } while (target_index != runtime->argument->count);
                    }
                }

                point->x += point->velocity_x;
                point->y += point->velocity_y;
                point->z += point->velocity_z;
        }

next_point:
        trail_base += 10;
        point_index++;
        point++;
        if (point_index != 8)
            goto point_loop;

        Camera_ApplyShake(4, 4);
        ObjectGroup_TickMemberTimers();
        runtime->frame_ready = 1;
        WaitFrames(1);
        frame++;
    } while (frame != 128);

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
