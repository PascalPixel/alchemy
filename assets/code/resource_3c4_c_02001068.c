/* Byte-exact reconstruction of resource_3c4's per-frame object step at
 * 0x02001068. Routed -fno-sched-depend-count: without it the `ldr` of the
 * +76 field is hoisted above the store to +12. */
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

extern s32 Func_0200409e(s32, s32);

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
