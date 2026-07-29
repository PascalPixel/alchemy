typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

struct S0104 {
    u8 pad[0x1e];
    u16 h1e;
};

struct T0104 {
    u8 pad0[8];
    s32 x;      /* 0x08 */
    s32 y;      /* 0x0c */
    s32 z;      /* 0x10 */
    s32 pad14;
    s32 a18;    /* 0x18 */
    s32 a1c;    /* 0x1c */
    u8 pad20[0x10];
    s32 v18;    /* 0x30 */
    s32 v1c;    /* 0x34 */
    u8 pad38[0xc];
    s32 vx;     /* 0x44 */
    s32 vy;     /* 0x48 */
    s32 vz;     /* 0x4c */
    struct S0104 *p; /* 0x50 */
    u8 pad54[0x10];
    u16 h64;    /* 0x64 */
};

void Func_02000104(struct T0104 *a)
{
    struct S0104 *p;

    a->x += a->vx;
    a->y += a->vy;
    a->z += a->vz;
    a->a18 += a->v18;
    *(s32 *)((u32)a + 0x1c) = *(s32 *)((u32)a + 0x1c) + a->v1c;
    p = (struct S0104 *)*(s32 *)((u32)a + 0x50);
    p->h1e += a->h64;
}
