#include "types.h"

typedef struct {
    u8 pad0[0x400];
    u8 f400;
    u8 pad401[0x600 - 0x401];
    s16 f600;
    s16 f602;
    s32 f604;
} FontTransfer;

extern FontTransfer *Func_080048b0(s32 arg0, s32 arg1);
extern s32 Func_08019ebc(void);
extern void Func_0801a5a4(FontTransfer *transfer, s32 slot);
extern s32 Func_08004080(void);
extern s32 Func_08003fa4(s32 index, s32 size, u8 *destination);
extern void Func_08002dd8(s32 id);
extern s32 Data_08029a10[];
extern s32 Data_08029ee4[];

void Func_08019fcc(u32 glyph, s32 includeBase, s32 *sourceIndex,
                   s32 *result, s32 reuseSource)
{
    FontTransfer *transfer;
    s32 slot;

    slot = 0;
    transfer = Func_080048b0(0x11, 0x608);

    if (glyph >= Func_08019ebc())
        glyph = 0;

    if (includeBase != 0) {
        transfer->f604 = Data_08029a10[2];
        transfer->f600 = 2;
        transfer->f602 = 2;
        Func_0801a5a4(transfer, 0);
        slot = 1;
    }

    transfer->f604 = Data_08029ee4[glyph];
    transfer->f600 = 2;
    transfer->f602 = 2;
    Func_0801a5a4(transfer, slot);

    if (reuseSource == 0)
        *sourceIndex = Func_08004080();

    *result = Func_08003fa4(*sourceIndex, 0x80, &transfer->f400);
    Func_08002dd8(0x11);
}
