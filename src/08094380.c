typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct {
    u8 unknown00[5];
    u8 kind;
} Effect08094380;

typedef struct {
    u8 unknown00[38];
    u8 first_flag;
    u8 second_flag;
    u8 unknown28[4];
    void *effect;
} Context08094380;

typedef struct {
    u8 unknown00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown14[28];
    s32 speed30;
    s32 speed34;
    u8 unknown38[24];
    Context08094380 *context;
} Object08094380;

Object08094380 *Func_0808ba1c(s32);
Effect08094380 *Func_08009048(Context08094380 *, s32);
void Func_08009080(Object08094380 *, s32);
void Func_080030f8(s32);
void Func_08009060(void *);
void Func_08009150(Object08094380 *, s32, s32, s32);
void Func_08009158(Object08094380 *);

extern s32 Data_02000240[];

void Func_08094380(s32 arg0)
{
    s32 zero;
    s32 mask;
    Object08094380 *object = Func_0808ba1c(Data_02000240[125]);
    Context08094380 *context = object->context;
    Effect08094380 *effect = Func_08009048(context, 27);

    zero = 0;
    mask = 0xfff00000;
    *(u8 *)((u8 *)context + 38) = zero;
    effect->kind = 15;
    object->x = (object->x & mask) + 0x80000;
    object->z &= mask;
    Func_08009080(object, arg0);
    Func_080030f8(30);
    context->second_flag = 1;
    Func_08009060(context->effect);
    context->effect = (void *)zero;
    *(u8 *)((u8 *)context + 38) = 1;
    object->speed34 = 0x10000;
    object->speed30 = 0x10000;
    Func_08009150(object,
        object->x, object->y, object->z + 0x80000);
    Func_08009158(object);
}
