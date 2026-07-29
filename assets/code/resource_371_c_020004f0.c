typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

struct Object {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
};

extern s16 Data_02000240[];

extern struct Object *Func_020047f6(u8 *);
extern struct Object *Func_02004806(s32);
extern void Func_020049d2(s32);

void Func_020004f0(u8 *o, s32 b, s32 c)
{
    u8 *state;
    s16 *table;
    struct Object *self;
    struct Object *other;

    self = Func_020047f6(o - 0x64);
    table = Data_02000240;
    other = Func_02004806(*(s32 *)&table[250]);
    state = *(u8 **)0x03001ebc;
    if (other->z < self->z) {
        *(u16 *)(state + 0x170) = b;
    } else {
        *(u16 *)(state + 0x170) = c;
    }
    Func_020049d2(0x7B);
}
