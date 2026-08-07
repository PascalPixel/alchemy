#include "types.h"

struct Object_080b5d3c {
    u8 padding00[0x118];
    u8 values[4];
};

struct Requirement_080b5d3c {
    u8 padding00[4];
    u8 values[4];
};

struct Result_080b5d3c {
    u32 mask;
};

struct Totals_080b5d3c {
    u8 values[4];
};

s32 Func_080b6a60(u16 *);
struct Object_080b5d3c *Func_08077008(s32);
const struct Requirement_080b5d3c *Func_080771e0(s32);
struct Result_080b5d3c *Func_08077000(s32);

struct Result_080b5d3c *Func_080b5d3c(void)
{
    struct Totals_080b5d3c totals;
    u16 object_ids[10];
    s32 object_count;
    s32 index;
    u32 mask;

    object_count = Func_080b6a60(object_ids);
    mask = 0;

    index = 0;
    do {
        totals.values[index] = 0;
        if (object_count > 0) {
            u16 *object_id = object_ids;
            s32 remaining = object_count;

            do {
                struct Object_080b5d3c *object =
                    Func_08077008(*object_id++);

                remaining--;
                totals.values[index] += object->values[index];
            } while (remaining != 0);
        }
        index++;
    } while (index <= 3);

    index = 0;
    do {
        const struct Requirement_080b5d3c *requirement =
            Func_080771e0(index);
        s32 matched;

        if (requirement != 0) {
            const u8 *needed = requirement->values;

            matched = 0;
            if (totals.values[0] >= needed[0]) {
                u8 *total = totals.values;

                do {
                    matched++;
                    if (matched > 3)
                        break;
                    total++;
                    needed++;
                } while (*total >= *needed);
            }

            if (matched == 4)
                mask |= 1u << index;
        }
        index++;
    } while (index <= 31);

    {
        struct Result_080b5d3c *result = Func_08077000(0);
        result->mask = mask;
        return result;
    }
}

void Func_080b5e10(void)
{
}
