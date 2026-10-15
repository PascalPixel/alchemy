/* Near miss: listing 25 instructions, draft 23, common 19, differ 10; the rest differs in instruction order. */
#include "types.h"
#include "ram_buffer.h"

struct CellTag {
    u8 key;
    s8 value;
    u8 unk02[2];
};

s32 Func_080eab98(s32 unused, s32 key, s8 value)
{
    struct CellTag *entry = (struct CellTag *)Ram_MapCollision;
    s32 i = 0;

    while (1) {
        if (entry->key == key) {
            if (entry->value == value)
                return i;
        } else if (entry->key == 0xff) {
            entry->key = key;
            entry->value = value;
            return i;
        }
        entry++;
        i++;
    }
}
