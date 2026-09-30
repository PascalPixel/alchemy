#include "TYPES.H"

/* One 24-byte descriptor; a sprite of -1 ends each table. */
struct ActionDescriptor {
    u16 sprite;
    s16 condition;
    s32 behavior;
    s32 x;
    s32 y;
    s32 z;
    u16 facing;
    u8 talk_facing;
    u8 flags;
};

struct ActionDescriptorTables {
    struct ActionDescriptor *tables[4];
};

extern struct ActionDescriptorTables *gEventWork;

/*
 * Finds the descriptor for a battle action across the event work's four
 * tables. Ids up to 7 name a descriptor by its sprite; larger ids count,
 * from 8, the descriptors whose sprite is above 7. Returns NULL when the
 * id is not found.
 */

struct ActionDescriptor *BattleAction_FindDescriptor(s32 id)
{
    struct ActionDescriptor *entry;
    s32 i;
    s32 group = 8;

    for (i = 0; i < 4; i++) {
        entry = gEventWork->tables[i];
        if (entry == NULL)
            continue;
        if (id <= 7) {
            for (; (s16)entry->sprite != -1; entry++) {
                if ((s16)entry->sprite == id)
                    goto found;
            }
        } else {
            for (; (s16)entry->sprite != -1; entry++) {
                if ((s16)entry->sprite > 7) {
                    if (group == id)
                        goto found;
                    group++;
                }
            }
        }
    }
found:
    if ((s16)entry->sprite == -1)
        return NULL;
    return entry;
}
