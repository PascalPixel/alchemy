#include "types.h"
#include "ram_buffer.h"
#include "call.h"

struct AlhafraEventWork {
    u8 unknown_000[0x15a];
    s16 value_15a;
    u8 unknown_15c[0x68];
    u16 value_1c4;
};

s32 GameFlag_Test();
void GameFlag_SetBit(s32 flag);
void Battle_WaitMode0(s32 frames);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void UiText_OpenMessageAtObject(s32 id, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 id, s32 flags);
void Func_02001c74(const u8 *message);
void Func_02001c84(s32 id, s32 a, s32 b);
void Func_02001c8c(s32 id, s32 a);
extern const u8 MsgAlhafra65[];
extern const u8 MsgAlhafra70[];
extern const u8 MsgAlhafra71[];

void Func_02000398(s32 id)
{
    if (!GameFlag_Test(0x8b5)) {
        ((struct AlhafraEventWork *)Ram_HeapSlots->event_work)->value_15a = 1;
        Func_02001c74(MsgAlhafra65);
        Func_02001c8c(id, 0);
    } else if (Value1(GameFlag_Test, 0x8b6)) {
        Func_02001c74(MsgAlhafra71);
        Func_02001c8c(id, 0);
    } else {
        Func_02001c74(MsgAlhafra70);
        Battle_WaitMode0(10);
        Motion_SetModeAndWaitAnimation(id, 3);
        Func_02001c84(id, 0, 10);
        Motion_SetVarCbAndRefresh(id, 2);
        UiText_OpenMessageAtObject(id, 0);
        Battle_WaitMode0(10);
        if (Inventory_PromptAndSetObjectMode(4, 0) == 0) {
            GameFlag_SetBit(0x8b6);
            Battle_WaitMode0(20);
            Func_02001c84(id, 0, 10);
            ((struct AlhafraEventWork *)Ram_HeapSlots->event_work)->value_1c4++;
        } else {
            Battle_WaitMode0(30);
            ((struct AlhafraEventWork *)Ram_HeapSlots->event_work)->value_1c4++;
            Func_02001c84(id, 0, 10);
        }
    }
}
