#include "types.h"
#include "scene.h"
#include "battle_party.h"

/* battle/presentation/misc/wait_sync.c */
struct LinkWork {
    u8 pad0[0x44];
    u8 enabled;
    u8 pad1[0x0b];
    u8 side;
    u8 pad2;
    u8 paused;
};

#define LINK_WORK (*(struct LinkWork **)0x03001e74)
#define LINK_REC 0x02002024
#define LINK_LOCAL ((u16 *)0x02002224)
#define LINK_STAT (*(u16 *)0x03001f64)

s32 BattlePres_WaitSync(void)
{
    struct LinkWork *work = LINK_WORK;
    u16 *peer;
    u16 *sync;
    s32 miss = 0;
    s32 i;

    if (work->enabled != 0) {
        u32 side = work->side;
        u32 other = 1;

        other ^= side;
        side = other << 1;
        side += other;
        side <<= 3;
        peer = (u16 *)(LINK_REC + side);
        sync = LINK_LOCAL;
        if (work->paused == 0) {
            sync[0] = 'E';
            sync[1] = 'X';
            sync[2] = 'E';
            sync[3] = 'C';

            for (i = 0; i <= 29; i++) {
                if ((LINK_STAT & 3) != 3) {
                    miss++;
                    if (miss > 24) {
                        return -1;
                    }
                } else {
                    miss = 0;
                    if (sync[2] == peer[2] && sync[3] == peer[3]) {
                        return 0;
                    }
                }
                WaitFrames(1);
            }
        }
        return -1;
    }
    return 0;
}

/* battle/party/assign_member_slots.c */
extern u8 *gBattleWork;

s32 BattleParty_AssignMemberSlots(void)
{
    u16 active_members[8];
    u8 *battle_state = gBattleWork;
    s32 party_size = BattleParty_ListActiveMembers(active_members);
    s32 member_slot;
    s32 unit_id;

    for (member_slot = 0; member_slot < party_size; member_slot++) {
        unit_id = active_members[member_slot];
        unit_id += 72;
        battle_state[unit_id] = (s8)(member_slot - 128);
    }
}

/* battle/runtime/apply_value_to_work_2224.c */
/*
 * Apply a value to the battle work record at 0x02002224.
 */

/*
 * Battle_SetMode names a `bx rN` slot: the call is indirect through the
 * register that slot selects, and the trailing argument is the callee
 * address at 0x03000164. That routine is reached with two arguments at
 * some sites and three at others, so its shape is not established.
 */

/*
 * The third argument reads val before val is written, so it carries
 * whatever the register already holds; it must not be respelled as a fresh
 * load. The two assignments that follow the call keep that order.
 */
char Battle_ApplyValueToWork2224(s16 arg2)
{
  s16 val;
  s16 val2;
  Battle_SetMode(0x02002224, 0x10, val, 0x03000164);
  val2 = arg2;
  val = val2;
}
