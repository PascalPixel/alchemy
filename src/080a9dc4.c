typedef unsigned char u8;
typedef signed int s32;

struct Entry_080a9dc4 {
    u8 padding[14];
    u8 value;
};

struct State_080a9dc4 {
    u8 padding[200];
    struct Entry_080a9dc4 *entries[5];
};

extern struct State_080a9dc4 *Data_03001f2c;
extern void Func_08015298(s32, s32, s32, s32);

s32 Func_080a9dc4(const u8 *enabled)
{
    struct State_080a9dc4 *state = Data_03001f2c;
    s32 index = 0;

    do {
        if (enabled[index] != 0) {
            s32 kind;

            switch (index) {
            case 0:
                kind = 16;
                break;
            case 1:
                kind = 1;
                break;
            case 2:
                kind = 2;
                break;
            case 3:
                kind = 15;
                break;
            case 4:
                kind = 7;
                break;
            default:
                kind = 0;
                break;
            }
            Func_08015298(8, kind, state->entries[index]->value, 0);
        }
        index++;
    } while (index <= 4);
    return 1;
}
