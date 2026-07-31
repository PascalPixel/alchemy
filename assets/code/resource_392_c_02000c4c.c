typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

extern s32 Func_020019fa(s32);
extern s32 Func_02001a10(s32);
extern s32 Func_02001a2a(s32);
/* The no-argument source is reached from two sites, so it carries two names. */
extern s32 Func_02001a28(void);
extern s32 Func_02001a2e(void);

struct Particle_02000c4c {
    u8 unknown_00[8];
    s32 x;                  /* +0x08 */
    s32 y;                  /* +0x0c */
    u8 unknown_10[0x20];
    s32 angle;              /* +0x30, 0x10000 to the turn */
    u8 unknown_34[4];
    s32 base_x;             /* +0x38 */
    s32 base_y;             /* +0x3c */
    u8 unknown_40[0x10];
    u16 *sprite;            /* +0x50 */
};

s32 Func_02000c4c(struct Particle_02000c4c *record)
{
    u16 *sprite = record->sprite;
    s32 lift;
    s32 tilt;
    s32 jitter;

    lift = Func_020019fa(record->angle) * 2;
    if (lift > 0)
        lift = -lift;

    record->x = record->base_x + Func_02001a10(record->angle) * 2;
    record->y = record->base_y + lift;

    /* Signed divide by 8, spelled `if (v < 0) v += 7; v >>= 3`. */
    tilt = Func_02001a2a(record->angle + 0x8000);
    if (tilt < 0)
        tilt += 7;
    sprite[15] = (u16)(tilt >> 3);          /* +0x1e */

    jitter = (s32)(((u32)Func_02001a28() << 9) >> 16);
    jitter += (s32)(((u32)Func_02001a2e() << 9) >> 16);
    record->angle += jitter + 1024;

    return 0;
}
