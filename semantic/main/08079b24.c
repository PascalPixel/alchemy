typedef signed short s16;
typedef signed int s32;

struct Pair_08079b24 {
    s16 input;
    s16 output;
};

extern struct Pair_08079b24 Data_08089258[];
#define FIELD(base, offset) (*(s16 *)((char *)(base) + (offset)))
s32 Func_080022ec(s32, s32);

s32 Func_08079b24(s32 value, s32 halve)
{
    s32 table = (s32)Data_08089258;
    s32 mode = halve;
    s32 clamped;
    s32 minimum;
    s32 maximum;
    s32 count;
    s32 index;
    s32 offset;
    s32 result;

    index = 0;
    minimum = FIELD(table, index);
    offset = 16;
    maximum = FIELD(table, offset);
    clamped = value;
    count = 5;
    if (clamped > minimum)
        clamped = minimum;
    else if (clamped < maximum)
        clamped = maximum;

    index = 0;
    offset = 0;
    if (index < count) {
        if (clamped > FIELD(table, offset)) {
            offset = 0;
        } else {
            s32 cursor = 0;
            do {
                index++;
                cursor += 4;
                if (index >= count) {
                    offset = index * 4;
                    break;
                }
                offset = cursor;
            } while (clamped <= FIELD(table, offset));
        }
    }

    if (index == count) {
        result = FIELD(table, offset - 2);
    } else {
        s32 previousInput = FIELD(table, offset - 4);
        s32 currentInput = FIELD(table, offset);
        s32 previousOutput = FIELD(table, offset - 2);
        s32 currentOutput = FIELD(table, offset + 2);
        s32 denominator = previousInput - currentInput;
        s32 numerator = previousOutput - currentOutput;
        result = Func_080022ec((clamped - currentInput) * numerator, denominator);
        result += currentOutput;
    }

    if (mode != 0) {
        if (mode == 1)
            result /= 2;
    }

    return result + 0x100;
}
