#include "../../../../../games/tla/src/field/overlay_685/scene.h"
#include "scene_ids.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct EventCounters {
    u8 unknown_000[0x1c4];
    u16 count;
};

struct Palette5 {
    u16 colour[5];
};

struct Position {
    s32 x;
    s32 y;
    s32 z;
};

void ObjectDispatch_SetSingleChildFlags(struct FieldActor *object, s32 flags);
void Motion_SetVarCbAndRefresh(s32 actor, s32 variant);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Motion_CamBounds(s32 a, s32 b, s32 c, s32 d);
void ObjectMotion_CommitPositionAndActivate(s32 actor, s32 dx, s32 dz);
void ObjectMotion_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);
void Party_SetFields1f2And1f4(s32 scene, s32 entrance);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Func_020034d4(s32 actor, s32 x, s32 z);
s32 Func_02003544(s32 actor, s32 value, s32 frames);
void Func_0200351c(s32 actor, s32 value, s32 frames);
void Func_02003564(void);
void Func_020035bc(void);
void Func_0200343c(s32 x, s32 z, s32 a, s32 b);
void Func_02003434(s32 a, s32 b, s32 c);
void Func_0200357c(s32 actor, s32 value);
void Func_02000734(struct Palette5 *from, struct Palette5 *to, s32 frames);
void Func_02002f58(struct Position *position);
void Func_02001138(void);

void Func_0200115c(void)
{
    struct Position position;
    struct Palette5 target;
    struct Palette5 current;
    struct FieldActor *object = Object_GetById(13);

    if (GameFlag_Test(0x97e))
        return;
    Func_0200345c();
    Func_020035b4(0);
    ObjectMotion_SetPositionAndReset(gPartyState.current_owner, 584, 248);
    if (GameFlag_Test(0x974)) {
        Func_02003534(gPartyState.current_owner, 0);
        Func_0200350c((s32)MsgBackForMore);
        Func_0200351c(11, 0, 5);
        Func_0200362c(36);
        Motion_CamBounds(664 << 16, -1, 0x1000000, 1);
        Func_02003564();
        Battle_WaitMode0(5);
        Motion_SetModeAndWaitAnimation(8, 4);
        Func_0200351c(0x2008, 0, 5);
        Func_02003544(8, 265, 30);
        Func_0200351c(0x2008, 0, 5);
    } else {
        GameFlag_SetBit(0x974);
        Func_0200350c((s32)MsgHoldItRight);
        Func_0200351c(11, 0, 5);
        Func_0200362c(36);
        ObjectMotion_ArmCallback(gPartyState.current_owner, 0, 0);
        Motion_CamBounds(664 << 16, -1, 0x1000000, 1);
        Func_02003564();
        Battle_WaitMode0(5);
        ObjectMotion_Launch(11, 6, 15);
        ObjectMotion_Launch(11, 6, 23);
        Func_0200351c(11, 0, 5);
        Func_02003544(11, 263, 30);
        Func_0200351c(11, 0, 5);
        Func_02003544(8, 257, 30);
        Func_02003534(8, 0x3000);
        Battle_WaitMode0(10);
        Func_0200351c(8, 0, 5);
        Func_02003544(11, 256, 30);
        Func_02003534(11, 0xb000);
        Battle_WaitMode0(10);
        Func_0200351c(11, 0, 5);
        Func_02003544(11, 263, 30);
        Func_0200351c(11, 0, 5);
        Motion_SetModeAndWaitAnimation(8, 3);
        Func_0200351c(8, 0, 5);
        Func_02003534(8, 0x8000);
        Battle_WaitMode0(5);
        Motion_SetModeAndWaitAnimation(8, 4);
        Func_0200351c(8, 0, 5);
        Func_02003534(11, 0x8000);
        Battle_WaitMode0(30);
        Func_02003534(11, 0xb000);
        Battle_WaitMode0(10);
        Func_02003544(11, 258, 30);
        Func_0200351c(11, 0, 5);
        Func_02003534(11, 0x8000);
        Battle_WaitMode0(20);
        Motion_SetVarCbAndRefresh(11, 2);
        Battle_WaitMode0(5);
        Func_0200351c(11, 0, 5);
        Func_02003544(8, 257, 30);
        Func_0200351c(8, 0, 5);
        Func_02003534(11, 0xb000);
        Battle_WaitMode0(10);
        Func_02003544(11, 265, 45);
        Func_02003534(11, 0x300a);
        Battle_WaitMode0(10);
        Func_0200351c(11, 0, 5);
        Func_02003544(8, 258, 30);
        Func_02003534(8, 0x3000);
        Battle_WaitMode0(10);
        Func_0200351c(8, 0, 5);
        Func_02003534(11, 0xb000);
        Battle_WaitMode0(10);
        Motion_SetModeAndWaitAnimation(8, 4);
        Battle_WaitMode0(5);
        Func_02003534(8, 0x8000);
        Battle_WaitMode0(5);
        Func_0200351c(8, 0, 5);
        Motion_SetModeAndWaitAnimation(11, 3);
        Battle_WaitMode0(10);
        Motion_SetModeAndWaitAnimation(11, 3);
        Battle_WaitMode0(5);
        Func_0200351c(11, 0, 5);
        Func_02003534_(11, 0x8000);
        Battle_WaitMode0(5);
    }
    Func_0200350c((s32)MsgOhAllRightListen);
    Func_02003534(8, 0xb000);
    Battle_WaitMode0(20);
    Object_GetById(8)->facing = 0x5000;
    Object_SetModeById(8, 5);
    Battle_WaitMode0(30);
    Object_SetModeById(8, 6);
    Func_020034d4(12, 724 << 16, 252 << 16);
    WORD_AT(Object_GetById(12), 24) = 0x13333;
    WORD_AT(Object_GetById(12), 28) = 0x13333;
    ObjectDispatch_SetSingleChildFlags(Object_GetById(12), 0);
    UiText_OpenMessageAtObject(0x2008, 0);
    if (Inventory_PromptAndSetObjectMode(4, 0) == 0) {
        Func_020035bc();
        Battle_WaitMode0(15);
        Func_020034d4(12, 0, 0);
        Object_SetModeById(8, 1);
        Object_GetById(8)->facing = 0x8000;
        Func_0200351c(0x2008, 0, 5);
        ((struct EventCounters *)Ram_HeapSlots->event_work)->count++;
        ObjectMotion_CommitPositionAndActivate(4, -16, 0);
        Battle_WaitMode0(5);
    } else {
        Battle_WaitMode0(25);
        ((struct EventCounters *)Ram_HeapSlots->event_work)->count++;
        Func_0200351c(0x2008, 0, 5);
        Func_02003544(8, 263, 30);
        Func_0200351c(0x2008, 0, 5);
        Func_0200362c(78);
        Func_0200343c(656 << 16, 208 << 16, 0, -4);
        Func_0200343c(656 << 16, 224 << 16, 0, -4);
        Func_0200343c(656 << 16, 240 << 16, 0, -4);
        Func_0200343c(656 << 16, 256 << 16, 0, -4);
        object->velocity_y = 0x4000;
        object->motion_flags = 2;
        object->terrain_height = 0xfc190000;
        ObjectMotion_Launch(12, 10, 0);
        Func_0200362c(146);
        ObjectMotion_SetActionVariant(12, 1);
        ObjectMotion_SetSpeedParameters(12, 0x30000, 0x18000);
        ObjectMotion_ResetAndSetPositionInMode2(12, 644, 248);
        Battle_WaitMode0(21);
        Func_020034d4(12, 0, 0);
        Battle_WaitMode0(50);
        Func_02003434(0x20000, 0x20000, 0x10000);
        Battle_WaitMode0(70);
        Func_0200362c(148);
        Battle_WaitMode0(25);
        current = *(struct Palette5 *)0x050000e4;
        target = *(struct Palette5 *)0x050001a4;
        Func_02000734(&current, &target, 48);
        Battle_WaitMode0(20);
        Func_0200362c(194);
        position.x = 0x02960000;
        position.y = 0;
        position.z = 0xee0000;
        Func_02002f58(&position);
        Func_0200362c(147);
        Func_020034d4(13, 664 << 16, 248 << 16);
        object->velocity_y = 0x10000;
        object->motion_flags = 2;
        object->terrain_height = 0;
        object->y = 0x200000;
        WORD_AT(object, 24) = 0x4000;
        WORD_AT(object, 108) = (s32)Func_02001138;
        WORD_AT(object, 28) = 0x4000;
        ObjectMotion_Launch(13, 11, 0);
        Func_0200362c(147);
        ObjectMotion_SetSpeedParameters(13, 0x33333, 0x19999);
        ObjectMotion_SetPositionAndReset(13, 584, 248);
        Party_SetFields1f2And1f4((s32)&SceneId_Ov685Room4, 98);
        Party_SetFields1eeAnd1f0((s32)&SceneId_Ov685Room4, 99);
        gPartyState.battle_rule_24b = 2;
        Func_0200357c(12, 5);
    }
    Func_02003464();
}
