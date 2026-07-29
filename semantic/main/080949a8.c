typedef signed short s16;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct EffectState {
    u8 padding0000[0x1f80];
    s16 timer;
    s16 randomizeDelay;
    s16 alternateSound;
} EffectState;

typedef struct DisplayState {
    u8 padding0000[0x2a01];
    u8 mode;
    u8 pending;
} DisplayState;

typedef struct GlobalState {
    EffectState *effect;
    void *unused;
    DisplayState *display;
} GlobalState;

s32 Func_080770c0(s32);
u32 Func_08004458(void);
void Func_080f9010(s32);
void Func_08091200(void *, s32);

void Func_080949a8(void)
{
    GlobalState *globals = (GlobalState *)0x03001ec8;
    EffectState *effect = globals->effect;
    DisplayState *display = globals->display;
    s16 state;

    if (effect->timer < 0)
        return;

    if (Func_080770c0(0x166) != 0)
        effect->timer = 0x80;

    state = effect->timer--;
    switch (state) {
    case 0:
        if (effect->randomizeDelay != 0) {
            u32 first = Func_08004458();
            u32 second = Func_08004458();

            effect->timer =
                ((first * 400) >> 16) - ((second * 100) >> 16) + 150;
            if (effect->alternateSound != 0)
                Func_080f9010(172);
            else
                Func_080f9010(171);
        }
        /* fall through */
    case 5:
    case 10:
        {
            u32 *dma = (u32 *)0x040000d4;
            u32 source;
            u32 destination;
            u32 control;

            Func_08091200(effect, 1);
            source = (u32)((u8 *)effect + 0x1500);
            destination = (u32)((u8 *)display + 0x1880);
            control = 0x840002a0;
            dma[0] = source;
            dma[1] = destination;
            dma[2] = control;
            display->mode = 12;
        }
        break;

    case 1:
    case 6:
    case 11:
        Func_08091200((u8 *)effect + 0xa80, 1);
        display->mode = 1;
        break;

    default:
        return;
    }

    display->pending = 0;
}
