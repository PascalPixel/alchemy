typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

extern u8 Data_00000008[];
extern u8 Data_080fc624[];
void Func_080fab3c(void *);

void Func_080fa458(void)
{
    u32 value = (u16)(u32)Data_00000008;

    if (value != 0) {
        u8 *entry = Data_080fc624;
        u32 count = value;

        do {
            Func_080fab3c(*(void **)entry);
            entry += 12;
            count--;
        } while (count != 0);
    }
}
