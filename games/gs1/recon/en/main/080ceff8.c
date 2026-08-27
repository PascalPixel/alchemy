#include "types.h"
#include "effect_step.h"

/*
 * Battle-presentation sub-effect in the same 0x03001eec "battle work"
 * family as games/gs1/src/battle/effects/member_orbit/run.c (owner
 * 0x080ce85c, family-matcher template).  Unlike that sibling, this owner
 * takes a second argument (`mode`, tested against 0/1/2 throughout) and
 * runs a fixed 48-frame loop rather than one sized from a party-member
 * count, so it is a related but distinct sub-effect, not the same body.
 *
 * `object` is republished at work + 0x7828, exactly as in the template.
 * Field 0x24 (36) of that object is read directly as a single s16
 * (a target id), not as the start of a member-id array.
 *
 * `Data_03001e50[46]` and `Data_03001e50[47]` are the same
 * "Data_03001e50[kind] holds kind's block address" heap-allocation cache
 * documented in games/gs1/recon/en/main/080e7404.c: this owner registers
 * two rectangle-blit routines through Func_080ed408(46, ...) and
 * Func_080ed408(47, ...) and then reads the resulting callbacks back out
 * of that cache by kind, exactly as 080e7404.c's own
 * `draw_rectangle = (DrawRectangle) Data_03001e50[46];` does.
 *
 * Value_0000007b, Value_0000008d, and Value_00000068 follow the
 * Value_<addr> convention already established for Func_08002f40's
 * resource-id argument (see games/gs1/src/map/heidia/prologue and
 * games/gs1/src/unidentified/overlays/state_update): every retained call
 * site loads the id through the literal pool rather than a movs
 * immediate, which an 8-bit integer constant cannot force but the
 * address of a link-time absolute byte symbol does.  Value_000000cc is
 * the same idiom applied to a direct BG2PA hardware-register write: 0xCC
 * fits an 8-bit movs immediate, yet the reference still routes it
 * through the literal pool.
 *
 * Both palette copies and the WordCopy call at 0x03001388 route through
 * the r6 slot of the _call_via_rN trampoline (games/gs1/asm/080072e4.s),
 * exactly as in the template.  The two per-frame rectangle draws route
 * through the r4 slot of the same trampoline, using function pointers
 * staged from Data_03001e50[46]/[47] before the loop and spilled across
 * it (r8/r9/sl/fp are already occupied by work, &pos, mode, and the
 * table_a base respectively, so canvas and the two draw callbacks live
 * on the stack for the whole function).
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*WordCopy)(void *dest, const void *src, s32 words);
typedef void (*DrawRectangle)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

extern void *Data_03001e50[];
extern u8 Value_0000007b;
extern u8 Value_0000008d;
extern u8 Value_00000068;
extern u8 Value_000000cc;

void Func_080cd594(s32 mode);
void *Func_08002f40(s32 id);
u32 Func_08005340(const void *source, void *destination);
s32 Func_080022ec(s32 numerator, s32 denominator);
void Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080041d8(void *callback, s32 interval);
void Func_080f9010(s32 value);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080b50e8(s32 value);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);
void EffectPosition_ApplyAlternateStepAndYOffset(
    s32 arg0, struct EffectPosition *position);

void Func_080ceff8(void *object, s32 mode)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *canvas;
    void *palette;
    struct EffectPosition pos;
    DrawRectangle draw_a;
    DrawRectangle draw_b;
    u8 *table_a;
    s32 frame;
    s32 idx;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    M2C_FIELD(work, void **, 0x7828) = object;
    Func_080cd594(0);

    palette = Func_08002f40((s32)&Value_0000007b);
    ((WordCopy)0x03001388)((void *)0x05000000, palette, 128);
    Func_08005340((u8 *)palette + 128, work);
    palette = Func_08002f40((s32)&Value_0000008d);
    ((WordCopy)0x03001388)((void *)0x05000000, palette, 128);
    if (mode == 2) {
        palette = Func_08002f40((s32)&Value_00000068);
        ((WordCopy)0x03001388)((void *)0x05000000, palette, 128);
    }

    EffectPosition_ApplyAlternateStepAndYOffset(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36), &pos);
    if (mode == 0) {
        M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x100;
        M2C_FIELD((void *)0x04000028, s32 *, 0) = (64 - pos.x) << 8;
    } else {
        M2C_FIELD((void *)0x04000020, s16 *, 0) = (s16)(s32)&Value_000000cc;
        M2C_FIELD((void *)0x04000028, s32 *, 0) =
            (Func_080022ec(-pos.x * 4, 5) + 64) << 8;
    }

    Func_080ed408(46, 7, 7, 3, 2);
    draw_a = (DrawRectangle)Data_03001e50[46];
    Func_080ed408(47, 7, 7, 7, 2);
    draw_b = (DrawRectangle)Data_03001e50[47];

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 50;
    Func_080041d8((void *)0x080CD261, 0x480);

    if (mode == 2) {
        M2C_FIELD(work, s32 *, 0x77A8) = 0;
        Func_080f9010(212);
    } else if (mode == 1) {
        M2C_FIELD(work, s32 *, 0x77A8) = 8;
        Func_080f9010(212);
    } else {
        M2C_FIELD(work, s32 *, 0x77A8) = 32;
    }

    table_a = (u8 *)0x080EE09F;
    for (frame = 0; frame != 48; frame++) {
        if (frame == 0) {
            if (mode == 2) {
                Func_080d6888(
                    M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36),
                    7, -1, 0, 32);
            } else {
                Func_080d6888(
                    M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36),
                    10, -1, 0, 32);
            }
        }
        if (frame == 24) {
            Func_080b50e8(0);
        }
        if (frame == 8 && mode == 0) {
            Func_080f9010(126);
        }
        if (frame <= 31) {
            idx = frame / 4;
            if (idx > 2) {
                idx = (idx & 1) + 1;
            }
            if (frame <= 27) {
                draw_a(canvas,
                    (u8 *)work + *(u16 *)((u8 *)0x080EE096 + idx * 2),
                    64 - *((u8 *)0x080EE09C + idx),
                    (pos.y - table_a[idx]) + 8,
                    *((u8 *)0x080EE09C + idx),
                    table_a[idx]);
                draw_b(canvas,
                    (u8 *)work + *(u16 *)((u8 *)0x080EE096 + idx * 2),
                    64,
                    (pos.y - table_a[idx]) + 8,
                    *((u8 *)0x080EE09C + idx),
                    table_a[idx]);
            }
        }
        if (mode == 0) {
            Func_080e155c(2, 2);
        } else {
            Func_080e155c(16, 16);
        }
        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
