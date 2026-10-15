#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "scene_call.h"
#include "text/msg_ids.h"
#include "daila.h"

struct DailaActor {
    u8 unknown_00[0xc];
    s32 y;
    u8 unknown_10[8];
    s32 speed_18;
    s32 speed_1c;
    u8 unknown_20[8];
    s32 unknown_28;
    u8 unknown_2c[0x1c];
    s32 unknown_48;
    u8 unknown_4c[9];
    u8 kind_55;
    u8 unknown_56[3];
    u8 flags_59;
    u8 unknown_5a[0x12];
    s32 unknown_6c;
};

struct DailaActor *Object_GetById(s32 id);
s32 PartyInventory_Add(s32 item);
s32 Func_02000d14(s32 item);
void Func_02000da4(struct DailaActor *object, s32 mode);
void Func_02000dfc(s32 cue);
void Func_02000ccc(s32 value, s32 slot);
void Func_02000c9c(struct DailaActor *object);
s32 Func_02000878(s32 item, s32 mode);

void Func_02000730(void)
{
    s32 saved_48;
    s32 saved_28;
    s32 saved_y;
    s32 item;

    saved_48 = Object_GetById(9)->unknown_48;
    item = 458;
    saved_28 = Object_GetById(9)->unknown_28;
    saved_y = Object_GetById(9)->y;
    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    if (Func_02000d14(item) >= 0) {
        Func_02000da4(Object_GetById(9), 3);
        Func_02000dfc(83);
        Func_02000ccc(item, 2);
        UiText_ShowPositionedMessageAndWait((s32)MsgItemReceived, 1);
        PartyInventory_Add(item);
        Func_02000d4c(9, 0, 0);
        Func_02000d4c(13, 232 << 16, 194 << 16);
        GameFlag_SetBit(0xa20);
    } else {
        Func_02000da4(Object_GetById(9), 3);
        Func_02000dfc(83);
        if (Func_02000878(item, 0) != -1) {
            Func_02000d4c(9, 0, 0);
            Func_02000d4c(13, 232 << 16, 196 << 16);
            GameFlag_SetBit(0xa20);
        } else {
            Func_02000d4c(9, 0, 0);
            Battle_WaitMode0(1);
            Object_GetById(9)->unknown_28 = saved_28;
            Object_GetById(9)->y = saved_y;
            Object_GetById(9)->unknown_48 = saved_48;
            Object_GetById(9)->unknown_6c = 0;
            Func_02000c9c(Object_GetById(9));
            Engine_ActorSetSpritePriority(9, 1);
            Object_GetById(9)->kind_55 = 2;
            Object_GetById(9)->speed_1c = 0x13333;
            Object_GetById(9)->speed_18 = 0x13333;
            Object_GetById(9)->flags_59 |= 4;
            Daila_ObjPlace(9, 232 << 16, 196 << 16);
        }
    }
    Engine_EventEnd();
}
