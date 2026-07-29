typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct OwnerRecord_080b5e14 {
    u8 name[15];
    u8 padding00f[0x11B];
    u8 field12a;
};

typedef char OwnerRecord_080b5e14_size[
    sizeof(struct OwnerRecord_080b5e14) == 0x12C ? 1 : -1
];

void Func_08002df0(void *);
void Func_080030f8(s32);
void *Func_08004970(s32);
s32 Func_08006408(void *);
void Func_08006488(void);
void Func_08015020(s32, u16 *);
void *Func_08077000(s32);
struct OwnerRecord_080b5e14 *Func_08077008(s32);

s32 Func_080b5e14(void)
{
    u16 tokens[24];
    struct OwnerRecord_080b5e14 *record;
    void *first_allocation;
    void *second_allocation;
    s32 owner;
    s32 length;
    s32 index;
    s32 qualifying;

    first_allocation = Func_08004970(0x154);
    qualifying = 0;

    for (owner = 0; owner <= 2; owner++) {
        record = Func_08077008(owner + 0x80);
        if (Func_08006408(record) == -1)
            break;

        Func_08006488();
        if (record->field12a != 0)
            qualifying++;
        Func_080030f8(2);

        Func_08015020(0x80C, tokens);
        length = 0;
        while (length < 5 && tokens[length] != 0)
            length++;

        index = 14;
        do {
            record->name[index] = record->name[index - length];
            index--;
        } while (index >= length);

        for (index = 0; index < length; index++)
            record->name[index] = tokens[index];
        record->name[14] = 0;
    }

    Func_08002df0(first_allocation);

    second_allocation = Func_08004970(0x140);
    record = Func_08077000(1);
    if (Func_08006408(record) != -1) {
        Func_08006488();
        Func_080030f8(2);
    }
    Func_08002df0(second_allocation);

    return qualifying;
}
