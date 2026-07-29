typedef unsigned char u8;
typedef unsigned int u32;
typedef int s32;

typedef struct {
    u8 filler0[9];
    u8 f0 : 2;
    u8 f1 : 2;
    u8 f2 : 4;
} Sub;

typedef struct {
    u8 filler0[0x50];
    Sub *sub;
} Obj;

void Func_02000030(Obj *o, u32 v)
{
    o->sub->f1 = v;
}
