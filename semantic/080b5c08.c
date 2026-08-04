#include "layout_guard.h"
#include "types.h"

typedef struct RegistrationEntry_080b5c08 {
    u8 index_00;
    u8 bit_01;
    u8 owner_02;
    s8 status_03;
} RegistrationEntry_080b5c08;

typedef struct RegistrationTable_080b5c08 {
    u8 padding_000[8];
    RegistrationEntry_080b5c08 entries_008[64];
    s32 count_108;
} RegistrationTable_080b5c08;

LAYOUT_SIZE_GUARD(
    RegistrationEntry080b5c08_Size,
    RegistrationEntry_080b5c08,
    4);
LAYOUT_OFFSET_GUARD(
    RegistrationEntry080b5c08_Index,
    RegistrationEntry_080b5c08,
    index_00,
    0);
LAYOUT_OFFSET_GUARD(
    RegistrationEntry080b5c08_Bit,
    RegistrationEntry_080b5c08,
    bit_01,
    1);
LAYOUT_OFFSET_GUARD(
    RegistrationEntry080b5c08_Owner,
    RegistrationEntry_080b5c08,
    owner_02,
    2);
LAYOUT_OFFSET_GUARD(
    RegistrationEntry080b5c08_Status,
    RegistrationEntry_080b5c08,
    status_03,
    3);
LAYOUT_OFFSET_GUARD(
    RegistrationTable080b5c08_Entries,
    RegistrationTable_080b5c08,
    entries_008,
    8);
LAYOUT_OFFSET_GUARD(
    RegistrationTable080b5c08_Count,
    RegistrationTable_080b5c08,
    count_108,
    0x108);

RegistrationTable_080b5c08 *Func_08077000(s32);
s32 Func_080770c0(s32);
void Func_080771b0(s32, s32, s32);
void Func_080771c0(s32, s32, s32);
void Func_080771c8(s32, s32, s32);
s32 Func_08077210(s32, s32, s32);
s32 Func_080b6a60(u16 *);
void *Func_080b7dd0(s32);

/*
 * Reconcile each active owner's available index/bit registrations, then
 * initialize orphaned pending entries in the primary registration table.
 */
void Func_080b5c08(void)
{
    u16 owners[5];
    s32 owner_count = Func_080b6a60(owners);
    s32 owner_number;

    for (owner_number = 0; owner_number < owner_count; owner_number++) {
        u16 owner = owners[owner_number];
        s32 index;

        for (index = 0; index <= 3; index++) {
            s32 bit;

            for (bit = 0; bit <= 19; bit++) {
                RegistrationTable_080b5c08 *table;
                s32 entry_number;

                if (Func_08077210(owner, index, bit) == 0)
                    continue;

                table = Func_08077000((u32)owner > 7);
                entry_number = 0;
                while (entry_number < table->count_108) {
                    RegistrationEntry_080b5c08 *entry =
                        &table->entries_008[entry_number];

                    if (entry->index_00 == index &&
                        entry->bit_01 == bit) {
                        break;
                    }
                    entry_number++;
                }

                if (entry_number == table->count_108)
                    Func_080771c8(owner, index, bit);
            }
        }
    }

    if (Func_080770c0(0x016c) == 0) {
        RegistrationTable_080b5c08 *table = Func_08077000(0);
        s32 entry_number;

        for (entry_number = 0;
             entry_number < table->count_108;
             entry_number++) {
            RegistrationEntry_080b5c08 *entry =
                &table->entries_008[entry_number];

            if (entry->status_03 == -1 &&
                Func_080b7dd0(entry->owner_02) == NULL) {
                Func_080771b0(
                    entry->owner_02,
                    entry->index_00,
                    entry->bit_01);
                Func_080771c0(
                    entry->owner_02,
                    entry->index_00,
                    entry->bit_01);
            }
        }
    }
}
