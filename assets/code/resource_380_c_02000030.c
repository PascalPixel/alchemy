typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

typedef struct {
    u8 pad[9];
    u8 f0 : 2;
    u8 f1 : 2;
    u8 f2 : 4;
} S;

void Func_02000030(s32 obj, u32 v)
{
    S *p = *(S **)(obj + 0x50);
    p->f1 = v;
}
