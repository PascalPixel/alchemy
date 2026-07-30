typedef signed int s32;
typedef signed short s16;

typedef struct {
    s16 filler0[50];
    s32 unk64;
} Struct_22a4;

typedef struct {
    s32 filler0[3];
    s32 unkC;
} Struct_22a4b;

extern Struct_22a4b *Func_0200715e(s32);

s32 Func_020022a4(Struct_22a4 *p)
{
    Struct_22a4b *q;

    q = Func_0200715e(((s16 *)p)[50]);
    ((s32 *)p)[3] = q->unkC + 0x100000;
    return 0;
}
