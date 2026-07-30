/*
 * resource_3c4 @ 0x02001068 (90 bytes).
 *
 * One integration step of a moving object.  The three velocity words at +68,
 * +72 and +76 are added into the position words at +8, +12 and +16, then two
 * of the velocities are damped:
 *
 *   - the +68 velocity loses whatever the service at 0x0200409e returns for
 *     (velocity, 18) — a per-step friction lookup;
 *   - the +76 velocity loses a sixteenth of itself, computed with the classic
 *     signed `x < 0 ? x + 15 : x` bias before `asrs #4`, i.e. a division that
 *     truncates toward zero rather than an arithmetic shift.
 *
 * Two further accumulators at +24 and +28 take their own increments from +48
 * and +52, and the unsigned halfword counter at +30 of the object referenced
 * through +80 is advanced by the unsigned halfword at +100.
 *
 * `pop {r5, r6, r7} ; pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef struct Linked_02001068 {
    u8 pad[30];
    u16 counter;           /* +30 */
} Linked_02001068;

typedef struct Object_02001068 {
    s32 pad0[2];           /* +0  */
    s32 x;                 /* +8  */
    s32 y;                 /* +12 */
    s32 z;                 /* +16 */
    s32 pad20;             /* +20 */
    s32 a;                 /* +24 */
    s32 b;                 /* +28 */
    s32 pad32[4];          /* +32 */
    s32 aStep;             /* +48 */
    s32 bStep;             /* +52 */
    s32 pad56[3];          /* +56 */
    s32 vx;                /* +68 */
    s32 vy;                /* +72 */
    s32 vz;                /* +76 */
    Linked_02001068 *link; /* +80 */
    s32 pad84[4];          /* +84 */
    u16 tick;              /* +100 */
} Object_02001068;

s32 Func_0200409e(s32 velocity, s32 kind);

void Func_02001068(Object_02001068 *object)
{
    s32 vx = object->vx;
    s32 vz;
    s32 bias;

    object->x += vx;
    object->y += object->vy;
    vz = object->vz;
    object->z += vz;

    object->vx = vx - Func_0200409e(vx, 18);

    bias = vz;
    if (vz < 0) {
        bias += 15;
    }
    object->vz = vz - (bias >> 4);

    object->a += object->aStep;
    object->b += object->bStep;
    object->link->counter = (u16)(object->link->counter + object->tick);
}
