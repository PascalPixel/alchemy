typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

typedef struct {
    u8 unknown00[9];
    u8 unknown09_0 : 2;
    u8 variant : 2;
    u8 unknown09_4 : 4;
    u8 unknown0a[28];
    u8 enabled;
} Object080dbb24;

extern u32 Data_03001eec;
extern u8 Value_000077d8;

Object080dbb24 *Func_08009030(s32);
void Func_08009020(Object080dbb24 *, s32);

void Func_080dbb24(s32 count, s32 kind, u32 variant) {
    u32 base = Data_03001eec;
    s32 index = 0;
    u32 offset;

    if (count == 0) {
        return;
    }
    offset = (u32)&Value_000077d8;
    do {
        Object080dbb24 *object = Func_08009030(kind);

        *(Object080dbb24 **)(offset + base) = object;
        if (object != 0) {
            object->enabled = 0;
            Func_08009020(object, index);
            (*(Object080dbb24 **)(offset + base))->variant = variant;
        }
        index++;
        offset += 4;
    } while (index != count);
}
