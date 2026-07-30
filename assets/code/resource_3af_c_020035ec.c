typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

extern s32 Func_0200790e(s32);
extern void Func_02007a40(s32, s32);
extern void Func_02007a54(s32, s32);
extern void Func_02007a68(s32, s32);
extern void Func_02007a7c(s32, s32);
extern s32 Func_0200791c(s32);
extern s32 Func_02007940(s32);
extern s32 Func_02007948(s32);

union Slot {
    s32 w;
    s16 h[2];
};

extern u32 Data_0200db70[];
extern u16 Data_0200db30[];
extern u16 Data_0200db40[];
extern s32 Data_0200db90[];

void Func_020035ec(s32 a, s32 b, s32 c)
{
    s32 *p = (s32 *)Func_0200790e(a);
    union Slot *q = (union Slot *)p[20];
    s32 t;

    if ((c & 2) == 0) {
        switch (Data_0200db70[b]) {
        case 1:
            Data_0200db40[b] = Data_0200db30[0];
            Func_02007a40(a, 8);
            break;
        case 2:
            Data_0200db40[b] = Data_0200db30[1];
            Func_02007a54(a, 9);
            break;
        case 3:
            Data_0200db40[b] = Data_0200db30[2];
            Func_02007a68(a, 10);
            break;
        case 4:
            Data_0200db40[b] = Data_0200db30[3];
            Func_02007a7c(a, 11);
            break;
        }
    }
    if ((c & 1) != 0) {
        t = Func_0200790e(Data_0200db40[b]);
        q[7].h[1] = Func_0200791c(Data_0200db40[b] + 0x8000) >> 5;
        p[4] = Data_0200db90[b] - (t << 2) - (t << 1);
    } else {
        t = Func_02007940(Data_0200db40[b] + 0x8000);
        q[7].h[1] = Func_02007948(Data_0200db40[b]) >> 5;
        p[4] = Data_0200db90[b] + (t << 2) + (t << 1);
    }
}
