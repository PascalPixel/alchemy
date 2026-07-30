#include "types.h"

struct Record_080b9470 {
    s16 owner;
    u16 unknown_02;
    u16 sort_key;
    s16 kind;
    u16 action;
    u8 unknown_0a[6];
};

struct ActionDefinition_080b9470 {
    u8 unknown_00[3];
    u8 type;
    u8 unknown_04[12];
};

void *Func_08077008(s32 owner);
s32 Func_080771e8(s32 group, s32 index);
struct ActionDefinition_080b9470 *Func_08077080(s32 action);
void *Func_08007304(void *destination, const void *source, u32 size);

void Func_080b9470(struct Record_080b9470 *records, s32 count)
{
    s32 index;
    s32 swapped;
    struct Record_080b9470 temporary;

    for (index = 0; index < count; index++) {
        struct Record_080b9470 *record = &records[index];

        if (record->kind == 5) {
            struct ActionDefinition_080b9470 *definition;
            s32 action_group;
            s32 action_index;

            Func_08077008(record->owner);
            action_group = (record->action >> 8) & 0xf;
            action_index = record->action & 0xff;
            definition =
                Func_08077080(Func_080771e8(action_group, action_index));

            if (definition->type == 46 || definition->type == 47 ||
                definition->type == 53) {
                record->sort_key += 10000;
            }
        }
    }

    do {
        swapped = 0;
        for (index = count - 1; index > 0; index--) {
            if ((s16)records[index].sort_key >
                (s16)records[index - 1].sort_key) {
                Func_08007304(&temporary, &records[index], 16);
                Func_08007304(&records[index], &records[index - 1], 16);
                Func_08007304(&records[index - 1], &temporary, 16);
                swapped++;
            }
        }
    } while (swapped != 0);
}
