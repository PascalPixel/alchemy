#include "types.h"

typedef struct DmaChannel {
    const void *source;
    void *destination;
    u32 control;
} DmaChannel;

void *Func_080048f4(s32, s32);
s32 Func_080022fc(s32, s32);
s32 Func_080022ec(s32, s32);
void Func_080b06ec(s32, void *, u32);
s32 Func_08004080(void);
void Func_08003fa4(s32, s32, void *);
s32 Func_080150c8(s32, u32, s32, s32, s32);
void Func_08002dd8(s32);

s32 Func_080b0744(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *buffer = Func_080048f4(14, 0x400);
    s32 result = 0;
    s32 item1;
    s32 item2;
    s32 item3;
    s32 item4;

    *(volatile DmaChannel *)0x040000D4 = (DmaChannel) {
        (const void *)0x080B3E80,
        buffer,
        0x84000040,
    };

    Func_080b06ec(Func_080022fc(arg0, 10), buffer, 0);
    item1 = Func_080022ec(arg0, 10);
    if (item1 != 0) {
        Func_080b06ec(Func_080022fc(item1, 10), buffer, 1);
        item2 = Func_080022ec(item1, 10);
        if (item2 != 0) {
            Func_080b06ec(Func_080022fc(item2, 10), buffer, 2);
            item3 = Func_080022ec(item2, 10);
            if (item3 != 0) {
                Func_080b06ec(Func_080022fc(item3, 10), buffer, 3);
                item4 = Func_080022ec(item3, 10);
                if (item4 != 0) {
                    Func_080b06ec(Func_080022fc(item4, 10), buffer, 4);
                }
            }
        }
    }

    item1 = Func_08004080();
    if (item1 != 0x60) {
        Func_08003fa4(item1, 0x100, buffer);
        result = Func_080150c8(item1, 0x80008000, arg1, arg2, arg3);
    }
    Func_08002dd8(14);
    return result;
}
