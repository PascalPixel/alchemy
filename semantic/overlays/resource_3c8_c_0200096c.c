typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * Resource 3c8 at 0x0200096c: per-frame integration of a projectile-style
 * scene object - advance position by velocity, damp the velocity, advance a
 * secondary pair, and spin the attached sprite.
 *
 * The owner is complete: `push {r5, r6, r7, lr}` at 0x0200096c and the
 * matching `pop {r5, r6, r7} ; pop {r0} ; bx r0` at
 * 0x020009c0..0x020009c4.  No literal pool sits inside the span and no live
 * frame or register state escapes the row: the next prologue begins at
 * 0x020009c8.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.
 *
 * The single `bl 0x0200572a` is reached with r0 = the pre-update x velocity
 * and r1 = 18, and its result is subtracted from that velocity, so it is a
 * damping/step helper.  Note that r0 is loaded at 0x02000984, three
 * instructions before the branch, and is NOT the object pointer.
 *
 * `0x0200099c` is a signed division by 16 emitted as
 * `cmp #0 ; bge ; adds #15 ; asrs #4`, i.e. C truncating division; the z
 * velocity loses one sixteenth of itself each frame.
 *
 * Field offsets 8/12/16 are the x/y/z triple proven by the byte-exact
 * siblings 0x020009c8 and 0x02000b08.  UNCERTAINTY: the halfword at 100 sits
 * where the byte-exact siblings 0x02000594 and 0x020022a4 read a *kind* word
 * on the scene-actor record; here it is added into the attached sprite's
 * halfword at +30 every frame, which reads as a rotation step.  The two uses
 * are only reconciled if this record is a different family, so the field is
 * left unnamed.  The remaining offsets are named descriptively and their
 * meaning beyond "added into the position triple" is not claimed.
 */

struct Sprite_0200096c {
    u8 unknown_00[30];
    u16 angle;              /* offset 30 */
};

struct Object_0200096c {
    u8 unknown_00[8];
    s32 x;                  /* 0x08 */
    s32 y;                  /* 0x0c */
    s32 z;                  /* 0x10 */
    u8 unknown_14[4];       /* 0x14 - not touched here */
    s32 accumulator18;      /* 0x18 */
    s32 accumulator1c;      /* 0x1c */
    u8 unknown_20[0x10];
    s32 rate30;             /* 0x30 */
    s32 rate34;             /* 0x34 */
    u8 unknown_38[0xc];
    s32 velocityX;          /* 0x44 */
    s32 velocityY;          /* 0x48 */
    s32 velocityZ;          /* 0x4c */
    struct Sprite_0200096c *sprite; /* 0x50 */
    u8 unknown_54[0x10];
    u16 word64;             /* 0x64 = 100 */
};

/* Old-style declaration: used for its return value. */
s32 Func_0200572a();

void Func_0200096c(struct Object_0200096c *object)
{
    s32 velocityX;
    s32 velocityZ;

    velocityX = object->velocityX;
    object->x += velocityX;
    object->y += object->velocityY;
    velocityZ = object->velocityZ;
    object->z += velocityZ;

    object->velocityX = velocityX - Func_0200572a(velocityX, 18);
    object->velocityZ = velocityZ - velocityZ / 16;

    object->accumulator18 += object->rate30;
    object->accumulator1c += object->rate34;

    object->sprite->angle += object->word64;
}
