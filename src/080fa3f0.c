typedef unsigned short u16;
typedef unsigned int u32;

typedef struct {
    u32 value;
    u16 table_index;
} LookupKey;

void Func_080fab3c(void *entry);
extern void *Data_080fc624[];
extern LookupKey Data_080fc684[];

void Func_080fa3f0(u16 index)
{
    void **entries = Data_080fc624;
    LookupKey *keys = Data_080fc684;
    LookupKey *key = &keys[index];
    void *entry = entries[key->table_index * 3];

    if (*(u32 *)entry == key->value)
        Func_080fab3c(entry);
}
