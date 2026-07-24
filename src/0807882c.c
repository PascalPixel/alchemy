typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct Container_0807882c {
    u8 padding[216];
    u16 arr[15];
};

struct Item_0807882c {
    u8 padding[2];
    u8 type;
};

struct Item_0807882c *Func_08078414(s32);

void *Func_0807882c(struct Container_0807882c *arg0, s32 arg1)
{
    s32 i;
    struct Item_0807882c *item;

    for (i = 0; i <= 14; i++) {
        if (arg0->arr[i] & 0x200) {
            item = Func_08078414(arg0->arr[i]);
            if (item->type == arg1) {
                return item;
            }
        }
    }
    return 0;
}
