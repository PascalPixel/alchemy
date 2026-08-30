#include "types.h"

#define BattlePresentation_ProcessPendingGraphicsTransfer Func_080cd260

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

typedef void (*CopyWordsFn)(void *destination, const void *source, s32 size);
typedef void (*FillWordsFn)(void *destination, s32 size, s32 value);

static __inline__ void CopyWords(void *destination, const void *source, s32 size)
{
    ((CopyWordsFn)0x03001388)(destination, source, size);
}

static __inline__ void FillWords(void *destination, s32 size, s32 value)
{
    ((FillWordsFn)0x03000168)(destination, size, value);
}

void Func_08005534(void *source, void *destination, s32 size);
void Func_080054e4(void *source, void *destination, s32 size);
void Func_08005490(void *source, s32 amount, void *destination, s32 size);
void Func_0800543c(void *source, s32 amount, void *destination, s32 size);

void BattlePresentation_ProcessPendingGraphicsTransfer(void)
{
    void **heap_cache = (void **)0x03001eec;
    void *work = heap_cache[0];
    void *source;
    s32 *counter;
    s32 next_counter;

    if (FIELD(work, s32, 0x7824) == 1) {
        source = heap_cache[1];
        switch (FIELD(work, u32, 0x7780)) {
        case 0:
            CopyWords((void *)0x06004000, source, 0x4000);
            break;
        case 1:
            CopyWords((void *)0x06004000, source, 0x4000);
            FillWords(source, 0x4000, FIELD(work, s32, 0x7784));
            break;
        case 2:
            if (FIELD(work, s32, 0x7784) == 50) {
                Func_08005534(source, (void *)0x06004000, 0x4000);
            } else {
                Func_080054e4(source, (void *)0x06004000, 0x4000);
            }
            break;
        case 3:
            Func_08005490(source, FIELD(work, s32, 0x7784),
                (void *)0x06004000, 0x4000);
            break;
        case 4:
            Func_0800543c(source, FIELD(work, s32, 0x7784),
                (void *)0x06004000, 0x4000);
            break;
        }
        FIELD(work, s32, 0x7824) = 0;
        counter = (s32 *)((u8 *)work + 0x7820);
        next_counter = 1;
    } else {
        counter = (s32 *)((u8 *)work + 0x7820);
        next_counter = *counter + 1;
    }
    *counter = next_counter;
}
