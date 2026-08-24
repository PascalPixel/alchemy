#include "metadata_lookup.h"
#include "types.h"

struct EventInfo {
    u8 pad0[5];
    u8 count;
    u8 pad1[10];
    u8 **table;
};

s32 Func_0800be20(s32 arg0, u32 arg1, s32 arg2)
{
    struct EventInfo *info;
    u8 *ptr;
    u8 op;
    u8 val;
    s32 sum = 0;

    info = Func_08185000(arg0);
    if (arg1 >= info->count) {
        return 0;
    }
    ptr = info->table[arg1];
    for (;;) {
        op = ptr[0];
        val = ptr[1];
        ptr += 2;
        /* 終端命令は長さへ含めない。 */
        if (op == 254 || op == 241 || op == 253 || op == 239) {
            break;
        }
        if (op == 245 || op == 255 || op <= 238) {
            sum += val;
            arg2--;
            if (arg2 == 0) {
                break;
            }
        }
    }
    return sum;
}
