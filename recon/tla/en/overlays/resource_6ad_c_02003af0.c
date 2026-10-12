/* Near misses of overlay 6ad (the same four are in overlays 6aa and 69e): Func_02003af0 differ 2 (the listing
 * saves the object after it loads x and z), Func_02003b78 differ 2 (the bit copy and the x load swap).
 * Func_02003f18 and Func_02003fc8 are first drafts: the registers of the loop and of the two flag bytes differ. */
#include "../../../../games/tla/src/field/overlay_6ad/room.h"

struct WarpEntry {
    s16 x;
    s16 z;
    s16 flag;
    s16 unknown_6;
};

struct WarpWork {
    s16 unknown_00;
    s16 position_flag;
    s16 unknown_04;
    s16 source_x;
    s16 source_z;
    s16 count;
    s16 current;
    s16 busy;
    u8 unknown_10[0x10];
    struct WarpEntry entries[1];
};

extern struct WarpWork Data_020070a8;

void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
struct ObjectRuntime *Func_020055e8(void);
void Func_020054b0(void);
void Func_02003af0(void);
void Func_02003b78(struct WarpWork *work, struct WarpEntry *entry);
void Func_02003be8(struct WarpWork *work, struct WarpEntry *entry);
void Func_02003d80(void);

void Func_02003af0(void)
{
    struct PartyState *party = &gPartyState;
    struct ObjectRuntime *found = Object_GetById(party->current_owner);
    s32 x = found->x;
    s32 z = found->z;
    struct ObjectRuntime *object = found;

    ObjectMotion_SetSpeedParameters(party->current_owner, 0x40000, 0x20000);
    x >>= 20;
    z >>= 20;
    ObjectMotion_SetPositionAndCommit(party->current_owner, (x << 4) + 8, (z << 4) + 8);
    WaitFrames(1);
    Object_SetPositionAndResetMotion(object, (x << 20) + 0x80000, object->y, (z << 20) + 0x80000);
    Battle_WaitMode0(4);
    Func_020055e8()->y += 0xfff80000;
}

void Func_02003b78(struct WarpWork *work, struct WarpEntry *entry)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    s32 x = (object->x >> 20) | (s16)(work->source_x & 64);
    s32 z = (object->z >> 20) | (s16)(work->source_z & 64);

    Func_02003af0();
    Func_020056c0(161);
    Func_020054e0_(work->source_x, work->source_z + 1, 1, 1, x, z);
    Battle_WaitMode0(12);
}

s32 Func_02003f18(void)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;
    struct WarpWork *work = &Data_020070a8;
    struct WarpEntry *entry = work->entries;
    s32 index;

    for (index = 0; index < work->count; index++, entry++) {
        if (entry->x == x && entry->z == z) {
            if (!GameFlag_Test(entry->flag)) {
                Func_02003b78(work, entry);
                GameFlag_SetBit(entry->flag);
                work->current = index;
                break;
            }
            if (index == work->current)
                break;
            work->current = index;
            Func_02003be8(work, entry);
            GameFlag_SetByte(work->position_flag, x);
            GameFlag_SetByte(work->position_flag + 8, z);
            return 1;
        }
    }
    return 0;
}

void Func_02003fc8(s32 dx, s32 dz, s32 value)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    u8 *event = Ram_HeapSlots->event_work;
    struct WarpWork *work = &Data_020070a8;
    s32 x = GameFlag_GetByte(work->position_flag);
    s32 z = GameFlag_GetByte(work->position_flag + 8);

    if (x != 0 || z != 0) {
        struct ObjectRuntime *other;

        GameFlag_SetByte(work->position_flag, 0);
        GameFlag_SetByte(work->position_flag + 8, 0);
        object->z = ((z + dz) << 20) + 0x80000;
        object->x = ((x + dx) << 20) + 0x80000;
        other = *(struct ObjectRuntime **)(event + 460);
        other->x = object->x;
        other->z = object->z;
        Func_020054b0();
        Func_02003d80();
        WORD_AT(Ram_HeapSlots->event_work, 428) = value;
    }
}
