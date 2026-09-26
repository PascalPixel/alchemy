#include "DMA.H"

extern void *Data_03001e50[];
extern u8 Value_00000140;

void *Runtime_AllocateHeapBlock(s32 slot, u32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void UiText_LookupMessage(void *reader, s32 message);

void UiText_DecodeMessage(s32 message, u16 *text, s32 capacity)
{
    u32 reader[3];
    void *original;
    s32 (*decode)(void *);
    u32 value;
    u16 delta;

    original = Data_03001e50[50];
    if (original == NULL) {
        u32 size;
        void *code;

        size = (u32)&Value_00000140;
        code = Runtime_AllocateHeapBlock(50, size);
        Dma_Set((const void *)0x08015430, code,
            0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    }
    decode = (s32 (*)(void *))Data_03001e50[50];
    UiText_LookupMessage(reader, message);
    delta = 0xffff;
    while ((value = decode(reader)) != 0) {
        switch (value) {
        case 14:
            capacity -= 3;
            if (capacity <= 0)
                goto done;
            *text++ = value;
            *text++ = decode(reader) + delta;
            value = decode(reader) + delta;
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 15:
            capacity--;
            if (capacity <= 0)
                goto done;
            *text++ = value;
            value = decode(reader) + 0xffff;
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
