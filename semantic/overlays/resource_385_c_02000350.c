typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * resource_385 owner at 0x02000350, 176 bytes: test whether a candidate is
 * close enough and lies in the subject's current facing sector.
 *
 * Complete owner: the extended-register prologue at 0x02000350 through the
 * interworking return at 0x020003ec-0x020003fb, plus the one referenced pool
 * word at 0x020003fc.  The next independent prologue is 0x02000400.
 *
 * First Func_02000314 measures the 3-D distance between the +8 coordinate
 * vectors.  When the candidate is at or beyond the supplied limit and the
 * override is clear, the subject's +91 byte is cleared and mode 2 is applied.
 * Otherwise Func_08000100 derives a direction from the +16 and +8 deltas.
 * The subject is accepted if its high facing nibble matches that direction,
 * either adjacent 0x1000 sector, or if the caller supplied the override.  An
 * accepted subject receives +91 = 1 and mode 1, and the function returns one.
 *
 * The pool word 0xf9fef001 at 0x020003fc is data despite being instruction-
 * shaped: the load at 0x02000396 consumes it for the wrapped adjacent-sector
 * addition, and execution returns before it.  The focused call-order bound is
 * therefore the code extent 0x0350..0x03fc: four real calls across three
 * independently resolved targets.
 */

struct Subject_02000350 {
    u8 reserved00[6];
    u16 facing;
    s32 x;
    u8 reserved0c[4];
    s32 y;
    u8 reserved14[0x47];
    u8 accepted;
};

extern s32 Func_02000314(s32 *left, s32 *right);
extern u16 Func_08000100(s32 y_delta, s32 x_delta);
extern void Func_08009080(struct Subject_02000350 *subject, s32 mode);

s32 Func_02000350(struct Subject_02000350 *subject,
    struct Subject_02000350 *candidate, s32 distance_limit, s32 override)
{
    u32 direction;
    u32 facing;

    if (Func_02000314(&candidate->x, &subject->x) < distance_limit ||
        override != 0) {
        direction = Func_08000100(candidate->y - subject->y,
            candidate->x - subject->x);
        facing = subject->facing & 0xf000;
        if ((direction & 0xf000) == facing ||
            ((direction + 0x1000) & 0xf000) == facing ||
            ((direction + 0xf9fef001) & 0xf000) == facing ||
            override != 0) {
            subject->accepted = 1;
            Func_08009080(subject, 1);
            return 1;
        }
    } else {
        subject->accepted = 0;
        Func_08009080(subject, 2);
    }

    return 0;
}
