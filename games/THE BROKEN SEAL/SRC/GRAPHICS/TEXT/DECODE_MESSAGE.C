#include "RUNTIME_MEM.H"
#include "TEXT_READER.H"
#include "HEAP_STATE.H"
#include "DMA.H"

extern const u8 Func_08015430[];

extern u8 Text_DecodeSymbolCodeSize[];


/* The European decoder also passes code 29's one argument through, and
   biases each argument by a plain constant. */
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define DECODE_EUROPEAN
#endif

void UiText_DecodeMessage(s32 message, u16 *text, s32 capacity)
{
    struct TextReader reader;
    void *original;
    s32 (*decode)(struct TextReader *);
    u32 value;
#if !defined(DECODE_EUROPEAN)
    u16 delta;
#endif

    original = ((union HeapState *)&gWorkSlot)->slots[50];
    if (original == NULL) {
        u32 size;
        void *code;

        size = (u32)Text_DecodeSymbolCodeSize;
        code = Runtime_AllocateHeapBlock(50, size);
        Dma_Set((const void *)Func_08015430, code,
            0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    }
    decode = (s32 (*)(struct TextReader *))((union HeapState *)&gWorkSlot)->slots[50];
    UiText_LookupMessage(&reader, message);
#if !defined(DECODE_EUROPEAN)
    delta = 0xffff;
#endif
    while ((value = decode(&reader)) != 0) {
        switch (value) {
        case 14:
            capacity -= 3;
            if (capacity <= 0)
                goto done;
            *text++ = value;
#if defined(DECODE_EUROPEAN)
            value = decode(&reader) + 0xffff;
            *text++ = value;
            value = decode(&reader) + 0xffff;
#else
            *text++ = decode(&reader) + delta;
            value = decode(&reader) + delta;
#endif
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 15:
#if defined(DECODE_EUROPEAN)
        case 29:
#endif
            capacity--;
            if (capacity <= 0)
                goto done;
            *text++ = value;
            value = decode(&reader) + 0xffff;
            break;
        default:
            capacity--;
            if (capacity <= 0)
                goto done;
            break;
        }
        *text++ = value;
    }
done:
    if (original == NULL)
        Runtime_ReleaseHeapBlock(50);
    *text = 0;
}
