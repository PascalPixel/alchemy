typedef signed short s16;
typedef signed int s32;

extern s16 Data_0809e686[];

s32 Func_0808ddb8(s32 key)
{
    s16 *entry = Data_0809e686;
    s32 result = 16;
    s32 current = *entry;

    while (current != -1) {
        ++entry;
        if (key == current) {
            result = *entry;
            break;
        }
        ++entry;
        current = *entry;
    }
    return result;
}
