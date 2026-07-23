typedef unsigned char u8;
typedef signed int s32;

void Func_08015078(s32, s32, s32, s32);
extern u8 Data_00000bdc[][2];
extern u8 Data_00000bda[];

void Func_080a8508(s32 target, s32 selected, u8 *entries) {
    s32 count = 0;
    s32 index = 0;

    do {
        if (entries[index] != 0) {
            if (selected == count) {
                s32 message = (s32)Data_00000bdc[index];
                Func_08015078(message, target, 0, -1);
                Func_08015078(message + 1, target, 0, 15);
            }
            count++;
        }
        index++;
    } while (index <= 4);

    if (count == 0)
        Func_08015078((s32)Data_00000bda, target, 0, 0);
}
