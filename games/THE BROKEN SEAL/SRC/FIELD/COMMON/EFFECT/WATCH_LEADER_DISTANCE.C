#include "IWRAM_CALL.H"

struct FieldActor {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct FieldPartyState {
    u8 unknown_000[500];
    s32 leader;
};

extern u8 *gEventWork;
extern u8 Value_00002090[];
extern struct FieldPartyState gGameState;

struct FieldActor *ObjectTable_Get(s32 index);

/* Counts down the scene timer while it runs and raises request 0x2090 once
   the leader strays 60 units from the anchor or the timer runs out. */
void FieldEffect_WatchLeaderDistance(void)
{
    u8 *work;
    struct FieldActor *actor;
    s16 *timer;
    s32 dx;
    s32 dz;

    work = gEventWork;
    actor = ObjectTable_Get(gGameState.leader);
    if (*(s16 *)(work + 0xcc0) != 0) {
        timer = (s16 *)(work + 0xcba);
        if (*timer != 0)
            (*timer)--;
    }
    dx = Iwram_MulQ16(*(s16 *)(work + 0xcbc) - actor->x / 0x10000, 0xd105);
    dz = *(s16 *)(work + 0xcbe) - (actor->z - actor->y) / 0x10000;
    if (dx * dx + dz * dz >= 3600 || *(s16 *)(work + 0xcba) == 0)
        *(u16 *)(work + 382) = (u32)Value_00002090;
}
