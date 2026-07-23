typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

struct Record_080fa350 {
    u32 value;
    u16 selector;
    u16 padding;
};

struct Entry_080fa350 {
    u32 value;
    s32 state;
    u32 padding;
};

void Func_080faa58(struct Entry_080fa350 *, u32);
extern struct Entry_080fa350 *Data_080fc624[];
extern struct Record_080fa350 Data_080fc684[];

void Func_080fa350(u16 index)
{
    struct Entry_080fa350 **entries = Data_080fc624;
    struct Record_080fa350 *records = Data_080fc684;
    struct Record_080fa350 *record = &records[index];
    struct Entry_080fa350 *entry = entries[record->selector * 3];

    if (entry->value != record->value) {
        Func_080faa58(entry, record->value);
    } else {
        s32 state = entry->state;
        u16 low = *(volatile u16 *)&entry->state;

        if (low == 0 || state < 0)
            Func_080faa58(entry, entry->value);
    }
}
