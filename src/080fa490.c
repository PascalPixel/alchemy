typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

extern u8 Data_00000008[];
extern s32 Data_080fc624[];
void Func_080fa264(s32);

void Func_080fa490(void)
{
    u16 raw = (u32)Data_00000008;

    if (raw != 0) {
        s32 *entry = Data_080fc624;
        u32 count = raw;

        do {
            Func_080fa264(*entry);
            entry += 3;
            count--;
        } while (count != 0);
    }
}
