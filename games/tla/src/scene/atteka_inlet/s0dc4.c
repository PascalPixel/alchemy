#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"

struct MapBits
{
    u8 unk00[23];
    u8 low : 2;
    u8 mode : 2;
};

void                  GameFlag_SetBit(s32 flag);
s32                   GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void                  Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void                  ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void                  Object_LinkObjectAndSetCallback(s32 id, s32 target);
void                  Func_02001060(s32 id, s32 x, s32 z);
void                  Func_02001068(s32 id, s32 x, s32 z, s32 speed);
void                  Func_02000c14(void);
void                  Func_02000bb8(void);

void Func_02000bf0(void);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void Func_02000ff8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

static __inline__ void Scheduler_AddOrUpdateCallback_(void (*callback)(void), s32 priority)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as The Broken Seal's scene layer does. */
    Scheduler_AddOrUpdateCallback(callback, priority);
}

static __inline__ void Func_02001060_(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline loads the arguments in the native order (differ 2 as a direct call). */
    Func_02001060(id, x, z);
}

static __inline__ void Func_02001068_(s32 id, s32 x, s32 z, s32 speed)
{
    /* FAKEMATCH: forwarding through the inline loads the arguments in the native order (differ 6 as a direct call). */
    Func_02001068(id, x, z, speed);
}

void Func_02000dc4(void)
{
    struct MapBits    *map = (struct MapBits *)Ram_HeapSlots->mapWork;
    struct PartyState *party;

    GameFlag_SetBit(0x9b4);
    if (!GameFlag_Test(0x9ad))
        Func_02001060(65, 0, 0);
    map->mode = 1;
    party = &gPartyState;
    /* FAKEMATCH: the native loads the party state's address before it starts the field offset; unforced the offset starts first (differ 2). */
    asm volatile("" : "+l"(party));
    Object_GetById(party->currentOwner)->priorityFlags |= 32;
    Scheduler_AddOrUpdateCallback_(Func_02000c14, 0x480);
    Scheduler_AddOrUpdateCallback_(Func_02000bb8, 0x480);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(17), 0);
    *((u8 *)Object_GetById(17) + 85) = 0;
    if (GameFlag_Test(0x9b2))
    {
        Func_02001060(19, 0, 0);
        Func_02001060(20, 0, 0);
        Func_02001060(21, 0, 0);
        Func_02001060(22, 0, 0);
        Func_02001060(23, 0, 0);
    }
    else
    {
        if (GameFlag_Test(0x9b1))
        {
            s32 speed = 0xc0;

            Func_02001068_(23, 0x1f80000, 0x1680000, speed << 6);
        }
        Object_LinkObjectAndSetCallback(19, party->currentOwner);
        Object_LinkObjectAndSetCallback(20, party->currentOwner);
        Object_LinkObjectAndSetCallback(21, party->currentOwner);
        Object_LinkObjectAndSetCallback(22, party->currentOwner);
    }
}

void Func_02000ed0(void)
{
    if (gPartyState.entrance == 3)
        GameFlag_ClearBit(0x12f);
    if (GameFlag_Test(0x9b2))
    {
        Func_02001060(9, 0, 0);
        Func_02001060(10, 0, 0);
        Func_02001060(11, 0, 0);
    }
    else if (GameFlag_Test(0x9b0))
    {
        Func_02001068_(9, 70 << 16, 308 << 16, 0);
        Func_02001068_(10, 80 << 16, 316 << 16, 0);
        Func_02000ff8(1, 16, 1, 1, 3, 19);
        Func_02000ff8(1, 16, 1, 1, 4, 19);
        Func_02000ff8(1, 16, 1, 1, 4, 20);
        Func_02000ff8(1, 16, 1, 1, 4, 21);
        WaitFrames(1);
    }
    if (GameFlag_Test(0x300))
        Func_02001060_(14, 120 << 16, 376 << 16);
    {
        s32 priority = 0x480;

        Scheduler_AddOrUpdateCallback(Func_02000bf0, priority);
    }
}
