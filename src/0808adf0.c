typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

typedef struct Entry {
    s16 first;
    s16 second;
    s16 third;
    s16 result;
} Entry;

extern Entry Data_0809d7a8[];

s32 Func_08009268(void *, s32 *);
s32 Func_080770c0(s32);
void Func_0808b2b0(s32);

s32 Func_0808adf0(void *arg0)
{
    s32 value;
    Entry *entry = Data_0809d7a8;
    s32 key = Func_08009268(arg0, &value);
    s32 result = 0;

    while (entry->first != -1) {
        if (entry->first == value &&
            (entry->second == -1 || entry->second == key) &&
            (entry->third == -1 || Func_080770c0(entry->third) == 0)) {
            result = entry->result;
            break;
        }
        entry++;
    }
    Func_0808b2b0(key);
    return result;
}
