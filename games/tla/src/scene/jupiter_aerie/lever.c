#include "types.h"
#include "party_state.h"

struct JupiterAerieObject {
    u8 unk00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct JupiterAerieObject *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Battle_WaitMode0(s32 frames);
void Object_SetModeById(s32 actor, s32 mode);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void Object_AttachWorkTargetToObject(s32 actor, s32 target);
void Func_02000054(s32 step);
void Func_02001070(struct JupiterAerieObject *object);
void Func_020017a8(s32 a, s32 b, s32 c);
void Func_02002a78(s32 x, s32 z, s32 a, s32 b);
void Func_02002a88(void);
void Func_02002a90(void);
void Func_02002af0(void);
void Func_02002b18(s32 skip);

void Func_020001b4(void)
{
    GameFlag_SetBit(0xa0a);
    Func_02002a88();
    Func_02002b18(0);
    Object_SetModeById(gPartyState.currentOwner, 1);
    ObjectMotion_ArmCallback(gPartyState.currentOwner, 0, 0);
    Func_020017a8(11, 9, 0);
    Battle_WaitMode0(40);
    if (GameFlag_Test(0xa0b) == 0) {
        Func_02000054(0);
        Battle_WaitMode0(40);
    } else {
        Func_02000054(2);
    }
    Object_AttachWorkTargetToObject(gPartyState.currentOwner, 1);
    Battle_WaitMode0(40);
    Func_02001070(Object_GetById(9));
    Func_02001070(Object_GetById(10));
    Func_02001070(Object_GetById(11));
    Func_02001070(Object_GetById(12));
    Func_02002af0();
    Func_02002a90();
}

void Func_02000264(void)
{
    GameFlag_SetBit(0xa0b);
    Func_02002a88();
    Func_02002b18(0);
    Object_SetModeById(gPartyState.currentOwner, 1);
    ObjectMotion_ArmCallback(gPartyState.currentOwner, 0x8000, 0);
    Func_020017a8(12, 10, 1);
    Battle_WaitMode0(40);
    if (GameFlag_Test(0xa0a) == 0) {
        Func_02000054(1);
        Battle_WaitMode0(40);
    } else {
        Func_02000054(3);
    }
    Object_AttachWorkTargetToObject(gPartyState.currentOwner, 1);
    Battle_WaitMode0(40);
    Func_02001070(Object_GetById(9));
    Func_02001070(Object_GetById(10));
    Func_02001070(Object_GetById(11));
    Func_02001070(Object_GetById(12));
    Func_02002af0();
    Func_02002a90();
}

void Func_02000314(s32 unused, s32 actor)
{
    struct JupiterAerieObject *object = Object_GetById(actor);

    Func_02002a78(object->x, object->z, 0, 4);
}
