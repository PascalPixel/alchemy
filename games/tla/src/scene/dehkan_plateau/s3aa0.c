#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

extern const u8 Data_02003f92[];
extern const u8 Data_02003fb8[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_02000038(const void *data);
void Func_020001d0(const void *data);
void Func_02001ff4(void);
void Func_02002140(void);
void Func_02003764(void);

void Func_02003aa0(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 0x1ac) = 0x204;
    Object_GetById(gPartyState.current_owner)->priority_flags |= 32;
    if (GameFlag_Test(95) != 0)
        GameFlag_SetBit(521);
    Func_02000038(Data_02003fb8);
    Func_020001d0(Data_02003f92);
    if (GameFlag_Test(519) == 0) {
        Func_02002140();
        WaitFrames(10);
    }
    Func_02001ff4();
    if (GameFlag_Test(265) == 0) {
        if (gPartyState.entrance > 5) {
            if (gPartyState.entrance <= 39)
                Func_02003764();
        }
    }
}
