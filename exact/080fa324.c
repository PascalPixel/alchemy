typedef unsigned short u16;
typedef signed int s32;

typedef struct {
    s32 value;
    u16 table_index;
} LookupKey;

extern s32 Data_080fc624[];
extern LookupKey Data_080fc684[];

void Func_080faa58(s32, s32);

void Func_080fa324(u16 index)
{
    s32 *records = Data_080fc624;
    LookupKey *keys = Data_080fc684;
    LookupKey *key = &keys[index];
    s32 *record = &records[key->table_index * 3];

    Func_080faa58(record[0], key->value);
}
