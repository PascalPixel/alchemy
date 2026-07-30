#include "types.h"

#define OFFSET_OF(type, member) ((u32)&(((type *)0)->member))

struct Snapshot_0807a664 {
    u16 marker;
    u16 entries[4][15];
    s16 savedValue0;
    s16 savedValue1;
    u16 savedPair0;
    u16 savedPair1;
};

struct OwnerState_0807a664 {
    u8 padding00[0xd8];
    u16 entries[15];
};

struct ItemDefinition_0807a664 {
    u8 padding00[2];
    u8 category;
};

typedef char Assert_0807a664_snapshot_entries[
    OFFSET_OF(struct Snapshot_0807a664, entries) == 2 ? 1 : -1];
typedef char Assert_0807a664_snapshot_value0[
    OFFSET_OF(struct Snapshot_0807a664, savedValue0) == 0x7a ? 1 : -1];
typedef char Assert_0807a664_snapshot_pair0[
    OFFSET_OF(struct Snapshot_0807a664, savedPair0) == 0x7e ? 1 : -1];
typedef char Assert_0807a664_snapshot_pair1[
    OFFSET_OF(struct Snapshot_0807a664, savedPair1) == 0x80 ? 1 : -1];
typedef char Assert_0807a664_owner_entries[
    OFFSET_OF(struct OwnerState_0807a664, entries) == 0xd8 ? 1 : -1];
typedef char Assert_0807a664_item_category[
    OFFSET_OF(struct ItemDefinition_0807a664, category) == 2 ? 1 : -1];

struct OwnerState_0807a664 *Func_08077394(s32 owner);
struct ItemDefinition_0807a664 *Func_08078414(s32 item);
void Func_08079ae8(s32 owner);
s32 Func_08077428(s32 owner);
void Func_0807a628(s32 owner, s32 target);
s32 Func_08079358(s32 flag);
void Func_0807808c(s32 mode);

void Func_0807a664(void)
{
    struct Snapshot_0807a664 *snapshot =
        (struct Snapshot_0807a664 *)0x02001078;
    s32 owner;

    if (snapshot->marker != 0x6774) {
        snapshot->marker = 0x6774;

        for (owner = 0; owner < 4; owner++) {
            struct OwnerState_0807a664 *state =
                Func_08077394(owner);
            s32 index;
            s32 count;

            for (index = 0; index < 15; index++)
                snapshot->entries[owner][index] = state->entries[index];

            for (index = 0; index < 15; index++) {
                struct ItemDefinition_0807a664 *item =
                    Func_08078414(state->entries[index]);

                if (item->category != 6)
                    state->entries[index] = 0;
            }

            count = 0;
            for (index = 0; index < 15; index++) {
                u16 entry = state->entries[index];

                if (entry != 0) {
                    state->entries[count] = entry;
                    count++;
                }
            }
            while (count < 15) {
                state->entries[count] = 0;
                count++;
            }

            Func_08079ae8(owner);
            Func_08077428(owner);
        }

        snapshot->savedValue0 = *(s16 *)0x02000460;
        snapshot->savedValue1 = *(s16 *)0x02000462;
        snapshot->savedPair0 = *(u16 *)0x02000438;
        snapshot->savedPair1 = *(u16 *)0x0200043a;

        Func_0807a628(0, 16);
        Func_08079358(0x952);
    }

    Func_0807808c(1);
}
