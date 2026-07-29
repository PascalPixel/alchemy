typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern u8 *Data_03001e60;
extern u16 Data_03001b10[];

u8 *Func_08185000(s32 arg0);
s32 Func_08004080(void);
s32 Func_08003fa4(s32, s32, s32);
s32 Func_0800b8ac(void *, s32);

struct Slot {
    s32 word0;      /* 0x00 */
    s32 word1;      /* 0x04 */
    s32 word2;      /* 0x08 */
    s32 word3;      /* 0x0C */
    s32 word4;      /* 0x10 */
    s32 word5;      /* 0x14 */
    u8 pad18[4];    /* 0x18 */
    u8 field1C;     /* 0x1C */
    u8 pad1D;       /* 0x1D */
    u16 field1E;    /* 0x1E */
    u8 first;       /* 0x20 */
    u8 pad21[5];    /* 0x21 */
    u8 field26;     /* 0x26 */
    u8 pad27[0x38 - 0x27]; /* 0x27 */
};

void *Func_0800bc70(s32 arg0)
{
    u8 *header;
    u8 *table;
    struct Slot *slot;
    s32 id;
    s32 i;
    s32 flags;
    s32 threshold;
    u32 headerValue;
    s32 code;

    slot = NULL;
    header = Func_08185000(arg0);
    id = Func_08004080();
    table = Data_03001e60;
    if (header[0] == 0)
        return NULL;

    for (i = 0; i <= 63; i++) {
        if (M2C_FIELD(table, u8, 0x20) == 0) {
            slot = (struct Slot *) table;
            break;
        }
        table += 0x38;
    }

    if (slot == NULL)
        return NULL;
    if (id == 0x60)
        return NULL;

    if ((code = Func_08003fa4(id, 0, 0)) == 0)
        return NULL;

    slot->field1C = id;
    slot->field1E = 0;
    slot->field26 = 1;

    headerValue = (header[0] << 8) + header[1];
    flags = 0x80008000;
    if (headerValue != 0x1020) {
        if (headerValue <= 0x1020) {
            flags = 0x8000;
            if (headerValue != 0x810) {
                if (headerValue <= 0x810) {
                    threshold = 0x808;
                    flags = 0;
                    goto check_low;
                }
                flags = 0x4000;
                if (headerValue != 0x1008) {
                    flags = 0x40000000;
                    if (headerValue != 0x1010) {
                        goto no_match;
                    }
                }
            }
        } else {
            flags = 0xC0008000;
            if (headerValue != 0x2040) {
                if (headerValue <= 0x2040) {
                    flags = 0x80004000;
                    if (headerValue != 0x2010) {
                        flags = 0x80000000;
                        if (headerValue != 0x2020) {
                            goto no_match;
                        }
                    }
                } else {
                    flags = 0xC0004000;
                    if (headerValue != 0x4020) {
                        threshold = 0x4040;
                        flags = 0xC0000000;
                    check_low:
                        if (headerValue != threshold) {
                        no_match:
                            flags = 0;
                        }
                    }
                }
            }
        }
    }

    M2C_FIELD(table, s32, 0x00) = 0;
    M2C_FIELD(table, s32, 0x04) = flags | 0x2000;
    M2C_FIELD(table, s32, 0x08) = code | 0x800;
    M2C_FIELD(table, s32, 0x0C) = 0;
    M2C_FIELD(table, s32, 0x10) = 0x6000;
    M2C_FIELD(table, s32, 0x14) = (Data_03001b10[187] >> 5) | 0x800;

    Func_0800b8ac(table, arg0);
    return slot;
}
