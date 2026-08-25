#include "types.h"

struct BattleMotionObject {
    u8 reserved_00[12];
    s32 x;
    s32 z;
};

struct BattlePlacementPayload {
    u8 reserved_00[100];
    u16 summon_slots[6];
};

struct BattlePlacementState {
    u16 reserved_00;
    struct BattlePlacementPayload placement;
} __attribute__((packed));

extern struct BattlePlacementState *Data_03001e74;

void Func_080b7424(u16 *actor_ids, s32 count, s32 *x_positions, s32 *z_positions);
struct BattleMotionObject *Func_080b7dd0(s32 actor_id);

#define RefreshSummonPositions Func_080b7548

void RefreshSummonPositions(void)
{
    s32 slot;
    struct BattlePlacementPayload *placement = &Data_03001e74->placement;
    u16 actor_ids[14];
    s32 x_positions[6];
    s32 z_positions[6];
    s32 count = 0;
    s32 first_actor_id;

    slot = 0;
    first_actor_id = (s16)placement->summon_slots[slot];
    if (first_actor_id != 0xff) {
        do {
            actor_ids[count] = placement->summon_slots[count];
            count++;
        } while (count <= 5 && (s16)placement->summon_slots[count] != 0xff);
    }

    Func_080b7424(actor_ids, count, x_positions, z_positions);

    if (count > 0) {
        s32 position_offset = 0;
        s32 source_offset = 100;

        do {
            s32 actor_id = *(s16 *)((u8 *)placement + source_offset);

            if (actor_id != 0xfe) {
                struct BattleMotionObject *object = Func_080b7dd0(actor_id);

                object->x = *(s32 *)((u8 *)x_positions + position_offset) << 16;
                object->z = *(s32 *)((u8 *)z_positions + position_offset) << 16;
            }
            count--;
            position_offset += 4;
            source_offset += 2;
        } while (count != 0);
    }
}
