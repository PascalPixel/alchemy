typedef unsigned short u16;
typedef unsigned int u32;

typedef struct {
    u32 value;
    u16 table_index;
} LookupKey;

void Func_080fa264(void *entry);
extern void *Data_080fc624[];
extern LookupKey Data_080fc684[];

void Func_080fa424(u16 index) {
    u32 offset = index;
    void * volatile *entries;
    LookupKey *keys;
    LookupKey *key;
    void *entry;
    u32 entry_value;
    u32 key_value;
    u32 table_index;
    u32 slot_index;

    offset <<= 16;
    entries = Data_080fc624;
    keys = Data_080fc684;
    offset >>= 13;
    key = (LookupKey *)((unsigned char *)keys + offset);
    table_index = key->table_index;
    slot_index = table_index << 1;
    slot_index += table_index;
    entry = entries[slot_index];
    entry_value = *(volatile u32 *)entry;
    key_value = key->value;

    if (entry_value == key_value)
        Func_080fa264(entry);
}
