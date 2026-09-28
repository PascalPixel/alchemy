/* Depth flags and slot ranks. */
#include "BABI.H"

s32 SceneActor_SetFlagBitByRelativeDepth(struct Actor_02000ec8 *actor)
{
    struct Actor_02000ec8 *ref;
    u8 *fp;
    u8 flag;
    ref = Actor_Get(ACTOR_PARTY_LEADER);
    fp = &actor->flatla3;
    flag = *fp | 2;
    *fp = flag;
    if (ref->z < actor->z) {
        s32 lim = actor->z - ref->z;
        s32 ay;
        lim += 0x00040000;
        ay = actor->y;
        ay += lim;
        if (ref->y <= ay) {
            flag &= 0xfd;
            *fp = flag;
        }
    }
    return 0;
}

void SceneState_SwapSlotPairByRank(s32 first, s32 second)
{
    struct Slot02000f10 *a = Actor_Get(first);
    struct Slot02000f10 *b = Actor_Get(second);

    if (a->rank <= b->rank) {
        s32 t;

        t = a->x;    a->x    = b->x;    b->x    = t;
        t = a->y;    a->y    = b->y;    b->y    = t;
        t = a->rank; a->rank = b->rank; b->rank = t;
        Task_Wait(1);
    }
}
