#include "TYPES.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "FIXED_MATH.H"

/* Three projected trail points form a closed triangle. Each edge receives
 * 24 interpolated particles, followed by a split 24x48 image. The optional
 * mode-1 image uses its own callback pair before the per-frame allocation.
 * Draft, not-yet-C: complete 1240-byte extent, pools included, is nonexact.
 * 2026-09-26 baseline: 1218 bytes, 128-byte frame versus reference 132,
 * equal topology, 386 aligned halfword edits (566 offset differences).
 * Shared EffectPosition stack/point records: 1212 bytes, still frame 128,
 * 402 aligned edits (560 offset differences). This type change did not
 * admit the required frame or stop induction-variable address replacement.
 * Indexed TriangleWork records and pre-division coordinate snapshots recover
 * frame 132: 1232 bytes, equal topology, 353 aligned edits (562 offset
 * differences). Coordinate capture now agrees with the reference; grouped
 * stack records still keep a common base instead of the screen pointer.
 */

typedef void (*WordCopy)(void *, const void *, s32);
typedef struct Effect {
    s32 kind, side, actor, unknown0c, unknown10, count, layers, mode, unknown20;
    s16 actors[8];
} Effect;
typedef struct TrailPoint {
    s32 unused[3];
    struct EffectPosition position;
    s32 unused18;
} TrailPoint;
enum {
    TRIANGLE_SCALE,
    TRIANGLE_SCREEN,
    TRIANGLE_VECTOR,
    TRIANGLE_ANCHOR,
    TRIANGLE_COORDINATE_COUNT
};
struct TriangleWork {
    u8 unknown_0000[0x7080];
    TrailPoint points[32];
    u8 unknown_7400[0x380];
    s32 transfer_mode;
    s32 transfer_value;
    u8 unknown_7788[0x9c];
    s32 transfer_pending;
    Effect *effect;
};
extern u8 Data_03001e50[];
extern u16 Data_080ede5c[];
extern s32 Data_080ee128[];
extern u8 Value_00000073, Value_00000076, Value_00000079, Value_0000008f;
void Func_080cd594(s32);
void Func_080de2f8(void *, s32, s32, s32, s32 *, s32 *);
u8 *Func_08002f40(s32);
void Func_08005340(const void *, void *);
s32 Func_080041d8(s32, s32);
void Func_080f9010(s32);
void Func_080b50e8(s32);
void Func_080d40ec(s32, s32, s32, s32);
s32 Func_08002322(s32);
s32 Func_0800231c(s32);
void Func_080cef64(s32, DrawRectangle *);
void Func_08002dd8(s32);
void **Func_080b5098(s32);
void Func_080049ac(void);
void Func_080051d8(const void *, const void *);
void Func_08004cf0(const s32 *);
void Func_08004c6c(s32);
void Func_08004c1c(s32);
void Func_080030f8(s32);
void Func_08004278(s32);
s32 Func_080cdbc0(void);
#define WORK_EFFECT (work->effect)

void Func_080d05fc(Effect *effect)
{
    u32 *cache, *entry;
    struct TriangleWork *work;
    u8 *sheet, *resource, *camera;
    void *dst;
    DrawRectangle draw[2];
    s32 origin_x, origin_y, shift;
    struct EffectPosition pos[TRIANGLE_COORDINATE_COUNT];
    s32 frame, member, tick, point_base, scale_phase;
    s32 x, y, radius, width, angle, scale_value, rotation;
    s32 point, sample;
    TrailPoint *trail, *from, *to;
    s32 *actor;

    cache = (u32 *)(Data_03001e50 + 39 * 4);
    entry = cache;
    work = (struct TriangleWork *)*entry++;
    dst = (void *)*entry;
    camera = (u8 *)cache[-27];
    sheet = (u8 *)cache[2];
    WORK_EFFECT = effect;
    Func_080cd594(1);
    if (WORK_EFFECT->mode == 1)
        Func_080de2f8(effect, 3, WORK_EFFECT->side, 0, &origin_x, &origin_y);
    *(s16 *)0x04000020 = 0x100;
    resource = Func_08002f40((s32)&Value_00000079);
    ((WordCopy)0x03001388)((void *)0x05000000, resource, 128);
    Func_08005340(resource + 128, work);
    Func_08005340(Func_08002f40((s32)&Value_00000073), sheet);
    Func_08005340(Func_08002f40((s32)&Value_00000076), (u8 *)work + 0x1000);
    resource = Func_08002f40((s32)&Value_0000008f);
    Func_08005340(resource + 128, (u8 *)work + 0x2000);
    work->transfer_mode = 3;
    work->transfer_value = 0x04040404;
    Func_080041d8(0x080cd261, 0x480);
    EffectPosition_ApplyStepAndYOffset(
        WORK_EFFECT->actors[0], &pos[TRIANGLE_ANCHOR]);
    shift = 64 - pos[TRIANGLE_ANCHOR].x;
    *(s32 *)0x04000028 = shift << 8;
    Func_080f9010(142);
    frame = 0;
    while (frame != WORK_EFFECT->count * 20 + 72) {
        if (frame == 64) Func_080b50e8(0);
        Func_080d40ec(frame, 0xaaab, 0x5555, 0);
        if (WORK_EFFECT->mode == 1) {
            x = ((Func_08002322(frame << 11) * 20) >> 16) + origin_x + shift - 20;
            y = ((Func_0800231c(frame << 11) << 2) >> 16) + origin_y;
            Func_080cef64(WORK_EFFECT->side, draw);
            y -= 24;
            if (frame > 32) y = y - (frame << 1) + 64;
            draw[0](dst, (u8 *)work + 0x2000, x, y, 40, 40);
            if (frame <= 3) draw[1](dst, (u8 *)work + 0x2000, x, y, 40, 40);
            Func_08002dd8(47);
            Func_08002dd8(46);
        }
        BattleEffect_LoadWork(46, 7, 7, 3, 2);
        draw[0] = (DrawRectangle)((u32 *)Data_03001e50)[46];
        BattleEffect_LoadWork(47, 7, 7, 7, 2);
        draw[1] = (DrawRectangle)((u32 *)Data_03001e50)[47];
        if (frame > 16 && (frame & 15) == 0)
            work->transfer_value += 0x01010101;
        member = 0;
        point_base = 0;
        scale_phase = frame * 3 << 9;
        tick = frame;
        do {
            actor = (s32 *)*Func_080b5098(WORK_EFFECT->actors[member]);
            if ((u32)tick <= 95) {
                Func_080049ac();
                Func_080051d8(camera, camera + 12);
                pos[TRIANGLE_VECTOR].x = actor[2];
                pos[TRIANGLE_VECTOR].y = actor[3];
                pos[TRIANGLE_VECTOR].depth = actor[4];
                EffectPosition_ApplyBaseAndYOffset(
                    (s32 *)&pos[TRIANGLE_VECTOR], &pos[TRIANGLE_SCREEN]);
                pos[TRIANGLE_SCREEN].x = pos[TRIANGLE_ANCHOR].x + shift;
                pos[TRIANGLE_SCREEN].y -= 24;
                if (tick <= 67) {
                    angle = 0;
                    scale_value = 0x2a000 - scale_phase;
                    rotation = (64 - tick) << 9;
                    point = 0;
                    do {
                        Func_080049ac();
                        if (tick <= 63) {
                            pos[TRIANGLE_SCALE].x = scale_value;
                            pos[TRIANGLE_SCALE].y = scale_value;
                            pos[TRIANGLE_SCALE].depth = scale_value;
                            Func_08004cf0((s32 *)&pos[TRIANGLE_SCALE]);
                            Func_08004c6c(rotation);
                            Func_08004c1c(rotation);
                        }
                        Func_08004c6c(angle);
                        EffectPosition_ApplyBaseAndYOffset(
                            Data_080ee128, &pos[TRIANGLE_VECTOR]);
                        trail = &work->points[point_base + point];
                        trail->position.x = pos[TRIANGLE_VECTOR].x + pos[TRIANGLE_SCREEN].x;
                        trail->position.y = pos[TRIANGLE_VECTOR].y + pos[TRIANGLE_SCREEN].y + 16;
                        point++; angle += 0x5555;
                    } while (point != 3);
                    point = 0;
                    do {
                        from = &work->points[point_base + point];
                        point++;
                        to = &work->points[point_base + Math_Mod(point, 3)];
                        radius = 5 - tick / 16;
                        sample = 0;
                        width = radius << 1;
                        do {
                            x = from->position.x;
                            x += Math_Div(sample * (to->position.x - x), 24);
                            y = from->position.y;
                            y += Math_Div(sample * (to->position.y - y), 24);
                            x -= radius; y -= radius;
                            draw[0](dst, (u8 *)work + Data_080ede5c[radius - 1] + 0x1000, x, y, width, width);
                            sample++;
                        } while (sample != 24);
                    } while (point != 3);
                }
                if (tick > 63) {
                    draw[0](dst, work, pos[TRIANGLE_SCREEN].x - 24, pos[TRIANGLE_SCREEN].y - 24, 24, 48);
                    draw[1](dst, work, pos[TRIANGLE_SCREEN].x, pos[TRIANGLE_SCREEN].y - 24, 24, 48);
                }
            }
            scale_phase -= 0x3000; point_base += 32; member++; tick -= 8;
        } while (member != 1);
        Func_08002dd8(47);
        Func_08002dd8(46);
        work->transfer_pending = member;
        Func_080030f8(1);
        frame++;
    }
    Func_08004278(0x080cd261);
    Func_080cdbc0();
}
