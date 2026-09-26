#include "TYPES.H"
#include "FIELD_SCENE.H"

/* NONMATCHING (2026-09-26): 168 candidate / 148 owner bytes, 83 differing halfwords.
 * A streaming table cursor recovers the ldmia traversal. Independent signed
 * volatile reads and unsigned cached sprites do not recover the reference's
 * mixed read lowering: the direct branch emits ldrh/sign extension and the
 * ordinal branch retains redundant signed reads. The extra lifetime saves r8.
 * Four tables of 24-byte ScenePlacement records; signed sprite -1 ends
 * each table. Ordinal 8 persists across tables. Shared sentinel return
 * restored topology in the prior 144-byte candidate; loads and allocation still differed.
 * Signed and unsigned cached sprite trials emitted 136 and 152 bytes.
 * Three structural hypotheses exhausted; no adoption. */

struct ActionDescriptorTables {
    struct ScenePlacement *tables[4];
};

struct ScenePlacement *BattleAction_FindDescriptor(s32 id)
{
    struct ActionDescriptorTables *runtime =
        *(struct ActionDescriptorTables **)0x03001ebc;
    struct ScenePlacement **table = runtime->tables;
    s32 table_index;
    s32 group = 8;
    struct ScenePlacement *entry;
    u16 sprite;
    s32 signed_sprite;

    for (table_index = 0; table_index < 4; table_index++) {
        entry = *table++;
        if (entry == 0) {
            continue;
        }
        if (id <= 7) {
            /* FAKEMATCH: preserve the listing's independent signed and
               unsigned descriptor reads in the direct-owner search. */
            signed_sprite = *(volatile s16 *)&entry->sprite;
            sprite = (u16)entry->sprite;
            if (signed_sprite != SCENE_TABLE_END) {
                do {
                    if ((s16)sprite == id) {
                        goto found;
                    }
                    entry++;
                    signed_sprite = *(volatile s16 *)&entry->sprite;
                    sprite = (u16)entry->sprite;
                } while (signed_sprite != SCENE_TABLE_END);
            }
        } else {
            sprite = (u16)entry->sprite;
            if ((s16)sprite != SCENE_TABLE_END) {
                do {
                    if ((s16)sprite > 7) {
                        if (group == id) {
                            goto found;
                        }
                        group++;
                    }
                    entry++;
                    sprite = (u16)entry->sprite;
                } while ((s16)sprite != SCENE_TABLE_END);
            }
        }
    }
found:
    if (entry->sprite == SCENE_TABLE_END)
        entry = 0;
    return entry;
}
