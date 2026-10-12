/* First drafts of overlay 6ad (the same pair is in overlays 6aa and 69e): Func_02003f18 and Func_02003fc8;
 * the registers of the loop and of the two flag bytes differ. */
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
