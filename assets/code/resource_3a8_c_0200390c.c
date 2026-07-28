typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct RenderData {
    unsigned char pad00[0x1E];
    s16 rotation;
} RenderData;

typedef struct Effect {
    unsigned char pad00[8];
    s32 x;
    s32 y;
    unsigned char pad10[0x20];
    s32 angle;
    unsigned char pad34[4];
    s32 baseX;
    s32 baseY;
    unsigned char pad40[0x10];
    RenderData *render;
} Effect;

s32 Func_02007450(s32);
s32 Func_02007466(s32);
s32 Func_02007478(void);
s32 Func_0200747e(void);
s32 Func_02007480(s32);
Effect *Func_0200752e(s32);

void Func_0200390c(void)
{
    Effect *effect = Func_0200752e(26);
    RenderData *render = effect->render;
    s32 offset = Func_02007450(effect->angle) * 2;
    s32 first;

    if (offset > 0) {
        offset = -offset;
    }
    effect->x = effect->baseX + Func_02007466(effect->angle) * 2;
    effect->y = effect->baseY + offset;
    render->rotation = Func_02007480(effect->angle + 0x8000) >> 3;
    first = Func_02007478();
    effect->angle +=
        ((u32)(first << 9) >> 16)
        + ((u32)(Func_0200747e() << 9) >> 16)
        + 0x400;
}
