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
extern s32 Func_08019ed0(void);
extern void Func_0801a5a4(FontTransfer *transfer, s32 slot);
extern s32 Func_08004080(void);
extern s32 Func_08003fa4(s32 index, s32 size, u8 *destination);
extern void Func_08002dd8(s32 id);
extern s32 Data_08029a10[];
extern s32 Data_0802de88[];

void Func_0801a404(u32 glyph, s32 includeBase, s32 *sourceIndex,
                   s32 *result, s32 reuseSource)
{
    FontTransfer *transfer;
    s32 slot;

    transfer = Func_080048b0(0x11, 0x608);
    slot = 0;

    if (glyph >= Func_08019ed0())
        glyph = 0;

    if (includeBase != 0) {
        transfer->f604 = Data_08029a10[2];
        transfer->f600 = 2;
        transfer->f602 = 2;
        Func_0801a5a4(transfer, 0);
        slot = 1;
    }

    transfer->f604 = Data_0802de88[glyph];
    transfer->f600 = 2;
    transfer->f602 = 2;
    Func_0801a5a4(transfer, slot);

    if (reuseSource == 0)
        *sourceIndex = Func_08004080();

    *result = Func_08003fa4(*sourceIndex, 0x80, &transfer->f400);
    Func_08002dd8(0x11);
}
