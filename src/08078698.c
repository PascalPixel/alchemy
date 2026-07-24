typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

struct Global_08078698 {
    u8 padding[500];
    s32 owner;
};

s32 Func_08078664(s32, s32);
s32 Func_080796c4(s16 *);
extern struct Global_08078698 Data_02000240;

s32 Func_08078698(s32 value)
{
    s16 owners[10];
    s32 count;
    s32 index;
    s16 *owner;
    s16 v;

    if (Func_08078664(Data_02000240.owner, value) != -1)
        return Data_02000240.owner;
    count = Func_080796c4(owners);
    owner = owners;
    index = 0;
    if (index < count) {
        do {
            v = *owner++;
            if (Func_08078664(v, value) != -1)
                return v;
            index++;
        } while (index < count);
    }
    return -1;
}
