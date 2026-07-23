typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

typedef void (*Callback)(u8);

extern u8 *Data_03007ff0;

void Func_080fa8d4(void)
{
    u8 *state = Data_03007ff0;
    u32 current = *(u32 *)state;
    s32 index;
    u8 *entry;

    if (current != 0x68736d53)
        return;

    *(u32 *)state = current + 1;

    index = 12;
    entry = state + 80;
    do {
        *entry = 0;
        index--;
        entry += 64;
    } while (index > 0);

    entry = *(u8 **)(state + 28);
    if (entry != 0) {
        index = 1;
        do {
            u8 id = (u8)index;
            Callback callback = *(Callback *)(state + 44);

            callback(id);
            *entry = 0;
            index++;
            entry += 64;
        } while (index <= 4);
    }

    *(u32 *)state = 0x68736d53;
}
