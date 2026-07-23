typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *Func_0808ba1c(u32);
s32 Func_080044d0(s32, s32);
void Func_0800447c(s32, u16, void *);

struct GlobalData {
    u8 filler[0x1F4];
    u32 value_1F4;
};

extern struct GlobalData Data_02000240;
extern char Value_fff80000;
extern char Value_001ffffe;

void Func_0808ee0c(void)
{
    register s32 saved_x;
    register s32 saved_y;
    register s32 x_offset;
    register s32 y_offset;
    register s32 x_delta;
    register s32 y_delta;
    register s32 object_x;
    register s32 object_y;
    register s32 negative_center;
    register u32 maximum;
    register s32 center;
    register s32 index;
    register u8 *object;
    register u8 *entry;

    object = Func_0808ba1c(Data_02000240.value_1F4);
    entry = *(u8 **)0x03001EBC + 0x11C;
    index = 0;
    if (entry[4] != 0) {
        object_x = FIELD(object, s32, 8);
        object_y = FIELD(object, s32, 0x10);
        negative_center = (s32)&Value_fff80000;
        maximum = (u32)&Value_001ffffe;
        center = 128;
        center <<= 12;
loop:
        x_offset = entry[6] << 20;
        x_delta = object_x - x_offset;
        saved_x = x_delta + negative_center;
        y_offset = entry[7] << 20;
        y_delta = object_y - y_offset;
        saved_y = y_delta + negative_center;
        if ((u32)(x_delta + 0x7FFFF) <= maximum &&
            (u32)(y_delta + 0x7FFFF) <= maximum) {
            FIELD(object, s32, 8) = x_offset + center;
            FIELD(object, s32, 0x10) = y_offset + center;
            Func_0800447c(0x140000,
                          (u16)Func_080044d0(saved_y, saved_x),
                          object + 8);
            FIELD(object, s32, 0x38) = 0x80000000;
            FIELD(object, s32, 0x3C) = 0x80000000;
            FIELD(object, s32, 0x40) = 0x80000000;
            return;
        }
        index++;
        entry += 8;
        if (index <= 9 && entry[4] != 0)
            goto loop;
    }
}
