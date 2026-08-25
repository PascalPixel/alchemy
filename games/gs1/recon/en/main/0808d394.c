#include "types.h"

#define BattleAction_FindDescriptor Func_0808d394

struct ActionDescriptor {
    s16 id;
    u8 data[22];
};

struct ActionDescriptorTables {
    struct ActionDescriptor *tables[4];
};

struct ActionDescriptor *BattleAction_FindDescriptor(s32 id)
{
    struct ActionDescriptorTables *runtime =
        *(struct ActionDescriptorTables **)0x03001ebc;
    s32 table_index;
    s32 group = 8;
    struct ActionDescriptor *entry = 0;

    for (table_index = 0; table_index < 4; table_index++) {
        entry = runtime->tables[table_index];
        if (entry == 0) {
            continue;
        }
        if (id <= 7) {
            while (entry->id != -1) {
                if (entry->id == id) {
                    return entry;
                }
                entry++;
            }
        } else {
            while (entry->id != -1) {
                if (entry->id > 7) {
                    if (group == id) {
                        return entry;
                    }
                    group++;
                }
                entry++;
            }
        }
    }
    return 0;
}
