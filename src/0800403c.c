typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct Entry_0800403c {
    u16 value;
    u16 flags;
};

extern u8 Data_03001810[];
extern struct Entry_0800403c Data_03001b10[];

void Func_0800403c(void)
{
    u32 limit = 0x1ff;
    u8 *bytes = Data_03001810;
    u32 count = 0;
    u32 fill = 0xff;

    do {
        *bytes++ = fill;
        count++;
    } while (count <= limit);

    {
        struct Entry_0800403c *entry = Data_03001b10;

        count = 0;
        do {
            entry->flags |= 0xffff;
            entry->value = 0;
            entry++;
            count++;
        } while (count <= 95);
    }
}
