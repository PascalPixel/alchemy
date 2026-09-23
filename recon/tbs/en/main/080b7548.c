/* NONMATCHING: 19 differing halfwords. The ROM keeps the first loop as
 * placement + byte offset (ldrh [base, off]) where GCC here strength-reduces
 * it to a pointer; explicit offsets let CSE merge the pre-check and copy
 * loads instead. The second loop and the int return (pop {r1}) match. */
#include "TYPES.H"

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

#include "BATTLE_WORK.H"
#define BattlePlacement ((struct BattlePlacementState *)BattleWorkPtr)

void Func_080b7424(u16 *actor_ids, s32 count, s32 *x_positions, s32 *z_positions);
struct BattleMotionObject *Func_080b7dd0(s32 actor_id);

#define Summon_Refresh Func_080b7548

s32 Summon_Refresh(void)
{
    struct BattlePlacementPayload *placement = &BattlePlacement->placement;
    u16 actor_ids[14];
    s32 x_positions[6];
    s32 z_positions[6];
    s32 count = 0;

    while (count < 6 && (s16)placement->summon_slots[count] != 0xff) {
        actor_ids[count] = placement->summon_slots[count];
        count++;
    }

    Func_080b7424(actor_ids, count, x_positions, z_positions);

    if (count > 0) {
        s32 index = 0;
        s32 source_offset = 100;

        do {
            s32 actor_id = *(s16 *)((u8 *)placement + source_offset);

            if (actor_id != 0xfe) {
                struct BattleMotionObject *object = Func_080b7dd0(actor_id);

                object->x = x_positions[index] << 16;
                object->z = z_positions[index] << 16;
            }
            count--;
            index++;
            source_offset += 2;
        } while (count != 0);
    }
}
