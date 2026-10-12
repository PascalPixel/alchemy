#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

extern const u8 Data_02003f44[];
extern const u8 Data_02003f52[];
extern const u8 Data_02003f80[];
extern const u8 Data_02003f92[];
extern const u8 Data_02003f98[];
extern const u8 Data_02003fa0[];
extern const u8 Data_02003fa8[];
extern const u8 Data_02003fb0[];
extern const u8 Data_02003fb8[];
extern const u8 Data_02003fc0[];
extern const u8 Data_02003fc4[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_020000a4(const void *data, s32 a, s32 b);
void Func_020002f8(const void *data);
void Func_02000708(const void *data);
void Func_02001474(s32 show);
void Func_02002180(void);
void Func_02003bf0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003d78(s32 skip);
void Func_02003dc0(const void *data);
void Func_02003dc8(void);
void Func_02003df8(s32 cue);

static __inline__ s32 Flag_Test(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return GameFlag_Test(flag);
}

static __inline__ void Flag_Set(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    GameFlag_SetBit(flag);
}

static __inline__ void Flag_Clear(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    GameFlag_ClearBit(flag);
}

static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02003bf0(a, b, c, d, e, f);
}

static __inline__ void Wait(s32 frames)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    WaitFrames(frames);
}

void Func_02001368(void)
{
    s32 y;

    Func_02000708(Data_02003f52);
    Func_020012d8();
    y = Object_GetById(gPartyState.current_owner)->y;
    /* FAKEMATCH: ⚓️ loads the height before it builds the limit; the plain test starts the constant first (one pair of instructions swapped). */
    __asm__ volatile("");
    if (y < 0x80000) {
        ObjectMotion_SetActionVariant(gPartyState.current_owner, 2);
        Object_GetById(gPartyState.current_owner)->priority_flags |= 1;
    }
}
