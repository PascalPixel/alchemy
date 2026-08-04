#include "types.h"

/*
 * resource_39c owner at 0x02000cd0, 136 bytes through its pool: derive a
 * small four-word setup record from the low runtime-state bits, generate
 * a wrapped negative random displacement, and forward the object's
 * adjusted position plus that record to the overlay's general eight-
 * argument configuration routine.  The owner always returns zero.
 *
 * m2c_guard independently measures 126 code bytes from the r8-saving
 * prologue at 0x02000cd0 through the real-value return at 0x02000d4c and
 * finds no seeding hazard.  Alignment at 0x02000d4e and two referenced
 * pool words at 0x02000d50-0x02000d57 extend the whole owner to the next
 * prologue at 0x02000d58.  Its two calls resolve to Func_080000f8 and
 * Func_0200013c.
 *
 * The setup record's game-level fields are not established here.  Its
 * second word is 7 when runtime bit 0 is set and 5 otherwise; its final
 * two words are both 0xcccc.  The random arithmetic is kept explicitly
 * wrapped because the assembly negates the resulting 32-bit word.
 */

extern u32 Data_03001e40;

extern u32 Func_080000f8(void);
extern void Func_0200013c(s32, s32, s32, s32, s32, s32, u32, u8 *);

s32 Func_02000cd0(u8 *object)
{
    s32 extra[4];
    u32 randomUnit;
    u32 displacement;
    s32 x;
    s32 y;
    s32 z;

    extra[0] = 0;
    extra[1] = (Data_03001e40 & 1) != 0 ? 7 : 5;
    extra[2] = 0xcccc;
    extra[3] = 0xcccc;

    randomUnit = (Func_080000f8() << 3) >> 16;
    displacement = randomUnit * 3;
    displacement += displacement << 4;
    displacement += displacement << 8;

    x = *(s32 *)(object + 8) + (8 - (Data_03001e40 & 0xf)) * 0x10000;
    y = *(s32 *)(object + 12) + 0x1a0000;
    z = *(s32 *)(object + 16);

    Func_0200013c(x, y, z, 0, (s32)(0u - displacement), 0,
                  0xb0000, (u8 *)extra);
    return 0;
}
