typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

struct Entry {
    u8 flags;
    u8 pad01[18];
    u8 value;
    u8 pad14[60];
};

struct Work {
    u8 pad00[8];
    u8 count;
    u8 pad09[35];
    struct Entry *entries;
    u8 pad30[4];
    s32 state;
};

void Func_080fb2cc(struct Work *arg0, u32 mask, u32 value)
{
    u8 *entry;
    s32 count;
    u32 bit;
    register u32 selected;
    register u32 shifted;

    selected = (mask << 16) >> 16;
    shifted = (unsigned short)value << 16;
    if (arg0->state == 0x68736d53) {
        arg0->state += 1;
        count = arg0->count;
        entry = (u8 *)arg0->entries;
        bit = 1;
        while (count > 0) {
            if ((selected & bit) && (entry[0] & 0x80)) {
                entry[19] = shifted >> 18;
                entry[0] |= 3;
            }
            count--;
            entry += 0x50;
            bit <<= 1;
        }
        arg0->state = 0x68736d53;
    }
}
