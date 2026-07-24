typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char u8;
typedef signed int s32;

struct QueueEntry { void *second; void *first; u32 control; };
extern u16 Data_02002090;
extern u16 Data_04000208;

void Func_080038fc(void *first, void *second)
{
    register u16 *count = &Data_02002090;
    u32 first_value = (u32)first;
    u32 second_value = (u32)second;
    u32 saved_ime;
    u32 index;

    second = &Data_04000208;
    saved_ime = *(volatile u16 *)second;

    *(volatile u16 *)second = (u16)(u32)second;
    index = *count;
    {
        if ((s32)index <= 31) {
            volatile u32 *entry = (u32 *)((u8 *)count + 4 + index * 12);
            *entry++ = second_value;
            *count = index + 1;
            *entry++ = first_value;
            *entry = 0x50000;
        }
    }
    *(volatile u16 *)second = saved_ime;
}
