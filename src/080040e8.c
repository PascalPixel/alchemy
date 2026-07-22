typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

struct Entry_080040e8 {
    u32 value;
    u16 flags;
    u8 extra;
    u8 filler7;
};

extern u8 Data_03001d34;
extern u8 Data_03001a10;
extern struct Entry_080040e8 Data_03001a20[];

void Func_080040e8(void)
{
    struct Entry_080040e8 *entry = Data_03001a20;
    /* Keep the initial zero and loop counter in the same register. */
    s32 remaining = ((u32)entry | ~(u32)entry) + 1;

    Data_03001d34 = remaining;
    Data_03001a10 = remaining;
    {
        u32 zero = 0;

        remaining = 19;
        do {
            entry->value = zero;
            entry->flags |= 0xffff;
            entry->extra = zero;
            entry++;
            remaining--;
        } while (remaining >= 0);
    }
    Data_03001d34 = 1;
}
