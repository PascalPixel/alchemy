typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct {
    void *object;
    s32 value;
    u8 mode;
    u8 pad9;
    u16 tag;
} Entry;

void Func_08006864(s32, s32, s32);
void Func_080fa55c(s32);
void Func_080fa6a0(s32);
void Func_080fa83c(s32);
void Func_080fa9e0(void *, s32, u8);
extern u8 Data_080f9675;
extern u32 Value_00000008;
extern u8 Data_02004350;

void Func_080fa2a0(void)
{
    u16 count;

    Func_08006864((s32)&Data_080f9675 & ~1, 0x03007000, 0x04000100);
    Func_080fa6a0(0x02003050);
    Func_080fa55c(0x02004090);
    Func_080fa83c(0x0097F800);

    count = (u32)&Value_00000008;
    if (count != 0) {
        Entry *entry = (Entry *)0x080FC624;
        u32 remaining = count;
        do {
            u8 *object = entry->object;
            Func_080fa9e0(object, entry->value, entry->mode);
            object[11] = entry->tag;
            *(void **)(object + 24) = &Data_02004350;
            entry++;
            remaining--;
        } while (remaining != 0);
    }
}
