#include "metadata_lookup.h"
#include "types.h"

#define ResourceMetadata_SumCommandLengths Func_0800be20

struct EventInfo {
    u8 pad0[5];
    u8 count;
    u8 pad1[10];
    u8 **table;
};

s32 ResourceMetadata_SumCommandLengths(s32 id, u32 no, s32 cnt)
{
    struct EventInfo *info;
    u8 *p;
    u8 op;
    u8 val;
    s32 sum = 0;

    info = Func_08185000(id);
    if (no >= info->count) {
        return 0;
    }
    p = info->table[no];
    for (;;) {
        op = p[0];
        val = p[1];
        p += 2;
        /* 終端命令は長さへ含めない。 */
        if (op == 254 || op == 241 || op == 253 || op == 239) {
            break;
        }
        if (op == 245 || op == 255 || op <= 238) {
            sum += val;
            cnt--;
            if (cnt == 0) {
                break;
            }
        }
    }
    return sum;
}
