typedef signed short s16;
typedef signed int s32;

typedef struct {
    s32 unknown00[3];
    s32 current;
    s32 radius;
    s32 limit;
    s32 unknown18[3];
    s16 values[1];
} Data080da24c;

s32 Func_080da24c(Data080da24c *data, s16 *output) {
    s32 length = data->radius * 2 + 1;
    s32 count = 0;
    s32 center = 0;
    s32 index;

    for (index = 0; index != length; index++) {
        if (data->current == data->values[index]) {
            center = index;
            break;
        }
    }
    for (index = 0; index != length; index++) {
        s32 value = center + index - data->radius;

        if (value >= 0 && value < data->limit) {
            output[count++] = value;
        }
    }
    return count;
}
