#include "types.h"

/*
 * Battle-presentation sub-effect at 0x080ce85c: entry 34 of the effect
 * callback table at 0x080EE2B4, so the single argument is the effect state
 * pointer that the dispatcher republishes at work + 0x7828.
 *
 * The owner sets the BG2 affine scale, loads its palette and 4KB of 32x32
 * sprite frames into the kind-39 work block, prepares the two generated
 * rectangle-blit routines (heap kinds 46 and 47), and then runs
 * member_count * 16 + 48 frames.  Each frame rewrites the 160-entry
 * per-scanline table at work + 0x6980 from a sine sweep - two different
 * curves depending on state field 4 - and, for every listed party member
 * whose 60-frame window is open, draws four sprites orbiting that member's
 * projected screen position.  The sprite frame and the blit routine are both
 * selected from the frame counter.
 *
 * Field offsets and callee signatures follow the 0x03001eec "battle work"
 * subsystem already partly recovered in games/gs1/recon/en/main/080e7404.c
 * and games/gs1/recon/en/main/080d82b0.c.  Value_000000af is the established
 * spelling for a small absolute link-time constant: every one of the 124
 * retained call sites of Func_08002f40 loads its resource id from a literal
 * pool rather than an immediate, which an ordinary integer literal cannot
 * produce, and the same spelling is already used by
 * games/gs1/src/overlays/scene_state_interaction/.
 *
 * The `status` assignments are load-bearing, not leftovers.  Binding a
 * callee's result makes the compiler emit the call as a set of r0, which
 * gives the following argument setup an output dependency on r0 and is what
 * puts the reference's `mov r0, #46` and `mov r0, #47` last in their
 * argument groups.  Dropping them costs seven differing halfwords.
 */
#define FIELD_AT_OFFSET(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);
typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

extern u8 Value_000000af;

void Func_080cd594(s32 mode);
void *Func_08002f40(s32 id);
u32 Func_08005340(const void *source, void *destination);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_080049ac(void);
void Func_080051d8(s32 a, s32 b);
void **Func_080b5098(s32 member_id);
void Func_080e3944(void *source, void *screen);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);

void Func_080ce85c(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *canvas;
    void *palette;
    s32 status;
    void *rectangle[2];
    s32 record[3];
    s32 screen[3];
    s32 member;
    s32 y_offset;
    void **rectangle_slot;
    void *second_rectangle;
    s32 *record_slot;
    s32 frame;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    FIELD_AT_OFFSET(work, void **, 0x7828) = object;
    Func_080cd594(0);
    FIELD_AT_OFFSET((void *)0x04000020, s16 *, 0) = 0x100;
    palette = Func_08002f40((s32)&Value_000000af);
    status = ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);
    status = Func_08005340((u8 *)palette + 128, work);
    status = Func_080ed408(46, 7, 7, 3, 2);
    rectangle[0] = heap_cache[7];
    status = Func_080ed408(47, 7, 7, 15, 2);
    second_rectangle = heap_cache[8];
    rectangle_slot = rectangle;
    rectangle_slot[1] = second_rectangle;
    Func_080041d8((void *)0x080DBB9D, 0x480);
    FIELD_AT_OFFSET(work, s32 *, 0x7780) = 2;
    FIELD_AT_OFFSET(work, s32 *, 0x7784) = 50;
    Func_080041d8((void *)0x080CD261, 0x480);
    if (FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828), s32 *, 4) == 1) {
        FIELD_AT_OFFSET((void *)0x04000028, s32 *, 0) = -0x6800;
        y_offset = -112;
    } else {
        y_offset = 0;
    }
    for (frame = 0;
            frame != (FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828), s32 *, 20)
                * 16) + 48;
            frame++) {
        s32 facing;
        s32 *scanline;
        s32 i;
        s32 member_id_offset;

        facing = *(s32 *)0x03001E80;
        scanline = (s32 *)((u8 *)work + 0x6980);
        if (FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828), s32 *, 4) == 0) {
            s32 angle;
            s32 ceiling;

            for (i = 0, ceiling = 0x80000, angle = frame << 10;
                    i != 160; i++) {
                *scanline++ = (ceiling - (Func_08002322(angle) << 3)) >> 10;
                angle += 1024;
            }
        } else {
            s32 angle;

            for (i = 0, angle = frame << 10; i != 160; i++) {
                *scanline++ = ((Func_08002322(angle) << 3) >> 10) - 0x7000;
                angle += 1024;
            }
        }
        Func_080049ac();
        Func_080051d8(facing, facing + 12);
        member = 0;
        if (FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828), s32 *, 20) != 0) {
            record_slot = record;
            member_id_offset = 36;
            while (member
                != FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828), s32 *, 20)) {
                void *member_object;

                member_object = *Func_080b5098(
                    FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828), s16 *,
                        member_id_offset));
                if (frame > member * 16 && frame < (member * 16) + 60) {
                    s32 spin;

                    if (frame == (member * 16) + 32) {
                        Func_080d6888(
                            FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828),
                                s16 *, member_id_offset),
                            0, 5, -1, 0);
                    }
                    record_slot[0] = FIELD_AT_OFFSET(member_object, s32 *, 8);
                    record_slot[1] = 0x280000;
                    record_slot[2] = FIELD_AT_OFFSET(member_object, s32 *, 16);
                    Func_080e3944(record_slot, screen);
                    for (i = 0; i != 4; i++) {
                        s32 x;
                        s32 y;
                        s32 slot;

                        spin = (frame << 9) + (i << 14);
                        x = (screen[0] + ((Func_08002322(spin) << 4) >> 16))
                            + y_offset;
                        y = screen[1] + ((Func_0800231c(spin) << 4) >> 16);
                        slot = frame / 16;
                        ((DrawRectangleFn)rectangle_slot[slot & 1])(
                            canvas,
                            (u8 *)work + (((frame / 4) - (slot * 4)) << 10),
                            x - 16, y - 16, 32, 32);
                    }
                }
                member_id_offset += 2;
                member++;
            }
        }
        FIELD_AT_OFFSET(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
    }
    Func_08004278((void *)0x080CD261);
    Func_08004278((void *)0x080DBB9D);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
