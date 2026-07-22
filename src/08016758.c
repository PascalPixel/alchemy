typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;

struct EntryState {
    u8 padding00[0x14];
    u16 state;
};

struct EntrySlot {
    struct EntryState *entry;
    s16 field04;
    s16 field06;
    u8 padding08[0x0C];
    s16 field14;
    s16 field16;
    s16 field18;
    s16 field1a;
    u8 padding1c[0x0C];
};

extern u8 *Data_03001e8c;

void Func_0801671c(void);

void Func_08016758(void)
{
    struct EntrySlot *slot = (struct EntrySlot *)(Data_03001e8c + 0x620);
    struct EntrySlot *selected = 0;
    s32 index;

    for (index = 0; index != 3; slot++, index++) {
        if (slot->entry == 0 || slot->entry->state != 0) {
            selected = slot;
            break;
        }
    }
    if (selected != 0) {
        if (selected->entry != 0) {
            Func_0801671c();
            selected->field06 = 0;
        }
        selected->field04 = 0;
        selected->field14 = 0;
        selected->field16 = 0xF;
        selected->field18 = 0;
        selected->field1a = 0xA;
    }
}
