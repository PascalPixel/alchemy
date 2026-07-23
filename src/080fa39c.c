typedef unsigned short u16;
typedef signed int s32;

typedef struct {
    s32 value;
    u16 table_index;
} LookupKey;

extern s32 *Data_080fc624[];
extern LookupKey Data_080fc684[];

void Func_080faa58(void *, s32);
void Func_080fa264(void *);

void Func_080fa39c(u16 index)
{
    s32 **entries = Data_080fc624;
    LookupKey *keys = Data_080fc684;
    LookupKey *key = &keys[index];
    s32 *entry = entries[key->table_index * 3];
    s32 current = entry[0];
    s32 value = key->value;

    if (current != value) {
        Func_080faa58(entry, value);
    } else {
        s32 state = entry[1];

        if ((u16)state == 0)
            Func_080faa58(entry, current);
        else if (state < 0)
            Func_080fa264(entry);
    }
}
