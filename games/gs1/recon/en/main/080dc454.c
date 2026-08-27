#include "types.h"

/*
 * Battle-presentation sub-effect at 0x080dc454, in the same 0x03001eec
 * "battle work" compiler-family cluster as the adopted template
 * games/gs1/src/battle/effects/member_orbit/run.c (owner 080ce85c).  This
 * owner is a smaller, simpler sibling: it registers only one draw routine
 * (heap kind 46, no kind 47) and only one periodic-update callback
 * (0x080CD261, no 0x080DBB9D), and it takes no argument -- unlike
 * 080ce85c, r0 is never read before being clobbered, and work's state
 * pointer at offset 0x7828 is read throughout without ever being written
 * here, so it must already be established by a prior call.
 *
 * Before the main loop it zeroes a 64-entry, 28-byte-stride record table
 * at work + 0x7080 (fields 0, 4, 0xC to zero, field 8 to 4).  Field 0xC of
 * each record is a persistent per-slot angle accumulator: the main loop
 * indexes it as member*3 + i for the i'th of 3 sprites orbiting each
 * listed party member, advances it by 0x200 every time that sprite is
 * drawn, and combines it with a per-sprite static offset (i * 0x5555, three
 * sprites spaced 120 degrees apart) to drive the sin/cos lookups -- unlike
 * 080ce85c's 4-sprites-per-member recomputed straight from the frame
 * counter, this owner keeps a running per-slot spin.
 *
 * `facing` is read once before the loop (matching the sibling
 * games/gs1/src/battle/effects/member_orbit/run.c 080d82b0.c's
 * `*(s32 *)((u8 *)heap_cache - 108)` idiom for the same fixed global) and
 * reused every frame, rather than re-read every iteration.
 *
 * Field offsets and callee signatures otherwise follow the 0x03001eec
 * subsystem already partly recovered in games/gs1/recon/en/main/080e7404.c
 * and games/gs1/recon/en/main/080d82b0.c.  Value_000000a9 is the
 * established Value_<hex> spelling for a small absolute link-time resource
 * id: the reference loads it from a literal pool rather than an immediate,
 * which an ordinary integer literal cannot produce (the same reasoning
 * 080ce85c's Value_000000af comment already records for this callee).
 *
 * The `screen2` output of Func_080e3944 is written (including its own
 * y_offset adjustment) but never read again in this owner; the call is
 * kept only for observable side effects, following the same
 * "load-bearing but unused result" pattern 080ce85c's own comment records
 * for its `status = ...` calls.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);
typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

extern u8 Value_000000a9;

void Func_080cd594(s32 mode);
void *Func_08002f40(s32 id);
u32 Func_08005340(const void *source, void *destination);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_080049ac(void);
void Func_080051d8(s32 a, s32 b);
void **Func_080b5098(s32 member_id);
void Func_080e396c(s32 member_id, s32 *screen);
void Func_080e3944(void *source, void *screen);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);

void Func_080dc454(void)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *canvas;
    void *palette;
    s32 status;
    void *rect_fn;
    s32 facing;
    s32 y_offset;
    s32 member;
    u32 window;
    s32 member_id_offset;
    s32 frame;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    facing = *(s32 *)((u8 *)heap_cache - 108);
    Func_080cd594(1);
    M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x100;
    M2C_FIELD((void *)0x04000020, s16 *, 50) = 0x1010;
    palette = Func_08002f40((s32)&Value_000000a9);
    status = ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);
    status = Func_08005340((u8 *)palette + 128, work);
    status = Func_080ed408(46, 7, 7, 3, 2);
    rect_fn = *(DrawRectangleFn *)((u8 *)heap_cache + 28);
    M2C_FIELD(work, s32 *, 0x7780) = 1;
    M2C_FIELD(work, s32 *, 0x7784) = 0;
    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    Func_080041d8((void *)0x080CD261, 0x480);
    y_offset = 0;
    if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) != 0) {
        y_offset = -112;
    }
    *(s32 *)0x04000028 = y_offset << 8;

    {
        void *rec;
        s32 n;

        rec = (u8 *)work + 0x7080;
        n = 0;
        do {
            n++;
            M2C_FIELD(rec, s32 *, 0) = 0;
            M2C_FIELD(rec, s32 *, 4) = 0;
            M2C_FIELD(rec, s32 *, 0xC) = 0;
            M2C_FIELD(rec, s32 *, 8) = 4;
            rec = (u8 *)rec + 0x1C;
        } while (n != 64);
    }

    frame = 0;
    if (frame
            != (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x14) * 16)
                + 64) {
        do {
            window = (u32)frame;
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x14)
                    != 0) {
                s32 screen[2];

                member = 0;
                member_id_offset = 0x24;
                do {
                    void *member_object;
                    s32 member_id;

                    member_id = M2C_FIELD(
                        M2C_FIELD(work, void **, 0x7828), s16 *,
                        member_id_offset);
                    member_object = *Func_080b5098(member_id);
                    member_id = M2C_FIELD(
                        M2C_FIELD(work, void **, 0x7828), s16 *,
                        member_id_offset);
                    Func_080e396c(member_id, screen);
                    screen[0] += y_offset;
                    if (window <= 63) {
                        s32 record[3];
                        s32 screen2[2];
                        s32 spin;
                        s32 idx_base;
                        u8 *src;
                        s32 i;

                        Func_080049ac();
                        Func_080051d8(facing, facing + 12);
                        record[0] = M2C_FIELD(member_object, s32 *, 8);
                        record[1] = M2C_FIELD(member_object, s32 *, 12);
                        record[2] = M2C_FIELD(member_object, s32 *, 16);
                        Func_080e3944(record, screen2);
                        screen2[0] += y_offset;

                        spin = 0;
                        src = (u8 *)work;
                        idx_base = member * 3;
                        for (i = 0; i != 3; i++) {
                            void *slot;
                            s32 x;
                            s32 y;

                            slot = (u8 *)work + 0x7080
                                + ((idx_base + i) * 0x1C);
                            x = screen[0]
                                + ((Func_08002322(
                                        M2C_FIELD(slot, s32 *, 0xC) + spin)
                                        << 3)
                                    >> 16);
                            y = screen[1]
                                + ((Func_0800231c(
                                        M2C_FIELD(slot, s32 *, 0xC) + spin)
                                        << 3)
                                    >> 16);
                            M2C_FIELD(slot, s32 *, 0xC) =
                                M2C_FIELD(slot, s32 *, 0xC) + 0x200;
                            ((DrawRectangleFn)rect_fn)(
                                canvas, src, x - 12, y - 28, 24, 24);
                            spin += 0x5555;
                            src += 0x240;
                        }
                    }
                    member_id_offset += 2;
                    member++;
                    window -= 16;
                } while (member
                    != M2C_FIELD(
                        M2C_FIELD(work, void **, 0x7828), s32 *, 0x14));
            }
            M2C_FIELD(work, s32 *, 0x7824) = 1;
            Func_080030f8(1);
            frame++;
        } while (frame
            != (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x14) * 16)
                + 64);
    }
    Func_08004278((void *)0x080CD261);
    Func_08002dd8(46);
    Func_080cdbc0();
}
