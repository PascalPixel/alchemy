#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "callback_scheduler.h"

extern const u8 gData_02003fb0[];
extern struct ObjectRuntime *gData_02004420;
extern s32 gData_02005678;
extern s32 gData_0200567c;

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_02000038(const void *data);
void Func_02001804(void);
void Func_020019b8(void);
void Func_02001ee8(void);
void Func_02002c14(void);
void Func_020035ec(void);
void Func_02003764(void);
void Func_02003a30(void);
void Func_02003bf0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003c68(s32 id, s32 x, s32 z);

static __inline__ s32 Flag_Test(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return GameFlag_Test(flag);
}

static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02003bf0(a, b, c, d, e, f);
}

static __inline__ void Actor_SetPosition(struct ObjectRuntime *object, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Object_SetPositionAndResetMotion(object, x, y, z);
}

void Func_0200390c(void)
{
    struct ObjectRuntime *owner = Object_GetById(gPartyState.currentOwner);
    s32 entrance;

    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
    owner->priorityFlags |= 32;
    if (Flag_Test(0x878) == 0) {
        Func_02003a30();
        if (Flag_Test(0x871) != 0) {
            Map_CopyAttributes(0, 0, 1, 1, 15, 5);
            if (gData_02004420 != NULL) {
                Actor_SetPosition(gData_02004420, 248 << 16, 0, 176 << 15);
                WaitFrames(1);
            }
        }
    }
    Func_02000038(gData_02003fb0);
    Func_020019b8();
    entrance = gPartyState.entrance;
    if (entrance > 10) {
        Func_02001804();
        GameFlag_SetBit(entrance + 789);
    }
    if (Flag_Test(0x871) != 0)
        Func_02003c68(9, 0, 0);
    Func_020035ec();
    if (Flag_Test(265) == 0) {
        if (gPartyState.entrance > 16) {
            if (gPartyState.entrance == 37 && Flag_Test(0x871) == 0)
                Func_02002c14();
            else
                Func_02003764();
        }
    }
    gData_0200567c = 0;
    gData_02005678 = 0;
    Scheduler_AddOrUpdateCallback(Func_02001ee8, 0x480);
}
