#include "TYPES.H"
#include "FIELD_SCENE.H"

/* NONMATCHING: complete 148-byte owner, including its pointer pool.
 * Four tables of 24-byte ScenePlacement records; signed sprite -1 ends
 * each table. Ordinal 8 persists across tables. Shared sentinel return
 * restores topology (144-byte candidate); loads and allocation still differ.
 * Signed and unsigned cached sprite trials emitted 136 and 152 bytes.
 * Three structural hypotheses exhausted; no adoption. */

struct ActionDescriptorTables {
    struct ScenePlacement *tables[4];
};

struct ScenePlacement *BattleAction_FindDescriptor(s32 id)
{
    struct ActionDescriptorTables *runtime =
        *(struct ActionDescriptorTables **)0x03001ebc;
    s32 table_index;
    s32 group = 8;
    struct ScenePlacement *entry;

    for (table_index = 0; table_index < 4; table_index++) {
        entry = runtime->tables[table_index];
        if (entry == 0) {
            continue;
        }
        if (id <= 7) {
            if (entry->sprite != SCENE_TABLE_END) {
                do {
                    if (entry->sprite == id) {
                        goto found;
                    }
                    entry++;
                } while (entry->sprite != SCENE_TABLE_END);
            }
        } else {
            if (entry->sprite != SCENE_TABLE_END) {
                do {
                    if (entry->sprite > 7) {
                        if (group == id) {
                            goto found;
                        }
                        group++;
                    }
                    entry++;
                } while (entry->sprite != SCENE_TABLE_END);
            }
        }
    }
found:
    if (entry->sprite == SCENE_TABLE_END)
        entry = 0;
    return entry;
}
