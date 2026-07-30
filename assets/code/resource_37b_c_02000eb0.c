typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define NULL ((void *)0)

struct Ent {
    s32 f00;
    u8 f04;
    u8 f05;
    u16 f06:9;
    u16 f07:7;
    s32 f08;
};

struct Cam {
    u8 pad[228];
    s32 x;
    s32 y;
};

extern struct Cam *Data_03001e70;
extern s32 Data_0200a974;
extern struct Ent Data_0200aa50[];

void Func_02003318(struct Ent *, s32);
void Func_02003358(struct Ent *, s32);
void Func_0200339c(struct Ent *, s32);

void Func_02000eb0(void) {
    s32 *cp = &Data_03001e70->x;
    struct Ent *e = Data_0200aa50;
    s32 sx = cp[0] / 65536;
    s32 sy = 80 - cp[1] / 65536;
    s32 v;
    u32 i;

    if ((u32)(sy + 16) <= 175) {
        v = (Data_0200a974 >> 10) - sx;
        v |= -32;
        {
            for (i = 0; i <= 8; i++) {
                e->f06 = v;
                e->f04 = sy;
                Func_02003318(e, 0);
                v += 32;
                e++;
            }
        }
        v = (Data_0200a974 >> 9) - sx;
        v |= -32;
        {
            for (i = 0; i <= 8; i++) {
                e->f06 = v;
                e->f04 = sy;
                Func_02003358(e, 0);
                v += 32;
                e++;
            }
        }
        v = (Data_0200a974 >> 8) - sx;
        v |= -32;
        {
            for (i = 0; i <= 8; i++) {
                e->f06 = v;
                e->f04 = sy + 8;
                Func_0200339c(e, 0);
                v += 32;
                e++;
            }
        }
    }
    Data_0200a974 += 0x80;
}
