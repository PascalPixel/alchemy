#include "local.h"

#define LEADER_HOP()                                    \
    do                                                  \
    {                                                   \
        Func_0200119c();                                \
        Func_0200125c(0);                               \
        Func_02001204(gPartyState.currentOwner, 0x102); \
    } while (0)

void Func_02000038(void)
{
    Func_0200122c(8, 10);
}

void Func_02000044(void)
{
    Func_02001274_(7, 63, 1, 16, 16, 57, 63);
}

void Func_02000060(void)
{
    Func_02001274_(22, 15, 0, 5, 5, 63, 52);
}

void Func_02000080(void)
{
    Func_02001274_(7, 22, 0, 3, 3, 56, 58);
}

void Func_020000a0(void)
{
    Func_02001274_(44, 25, 0, 4, 3, 66, 57);
}

void Func_020000c0(void)
{
    Func_02001274_(16, 13, 0, 4, 8, 61, 52);
}

void Func_020000e0(void)
{
    Func_02001274_(21, 9, 0, 2, 2, 57, 52);
}

void Func_02000100(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);

    Func_02001274_(22, 15, 0, 5, 5, 63, 52);
    if (GameFlag_Test(0xf53) == 0)
    {
        Func_020011cc_(64, 0x1880000, 0x1580000);
        if (leader->x >> 20 == 24 && leader->z >> 20 == 21)
        {
            LEADER_HOP();
            ObjectMotion_OffsetPositionAndResetMotion(gPartyState.currentOwner, 16, 0);
            leader->velocityY = 0x60000;
            ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
            Func_020011a4();
        }
    }
}

void Func_0200018c(void)
{
    Func_0200116c_(51, 83, 3, 1, 9, 65);
    Func_0200121c(3);
}

void Func_020001b0(void)
{
    Func_0200116c_(51, 83, 3, 1, 18, 66);
    Func_0200121c(4);
}

void Func_020001d4(void)
{
    Func_0200121c(5);
}

void Func_020001e0(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);

    Func_02001274_(7, 22, 0, 3, 3, 56, 58);
    Func_020011cc_(8, 0x880000, 0x1780000);
    if (leader->x >> 20 == 8 && leader->z >> 20 == 23)
    {
        LEADER_HOP();
        ObjectMotion_OffsetPositionAndResetMotion(gPartyState.currentOwner, 16, 0);
        leader->velocityY = 0x60000;
        ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
        Func_020011a4();
    }
}

void Func_02000260(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);

    Func_02001274_(5, 13, 0, 5, 3, 56, 52);
    Func_020011cc_(9, 0x880000, 0xf80000);
    if (leader->x >> 20 == 8 && leader->z >> 20 == 15)
    {
        LEADER_HOP();
        ObjectMotion_OffsetPositionAndResetMotion(gPartyState.currentOwner, 16, 0);
        leader->velocityY = 0x60000;
        ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
        Func_020011a4();
    }
}

void Func_020002e0(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);

    Func_02001274_(5, 13, 0, 5, 3, 56, 52);
    Func_020011cc_(10, 0x980000, 0x1080000);
    if (leader->x >> 20 == 9 && leader->z >> 20 == 16)
    {
        Func_0200119c();
        Func_0200125c_(0);
        Func_02001204_(gPartyState.currentOwner, 0x102);
        ObjectMotion_OffsetPositionAndResetMotion_(gPartyState.currentOwner, -16, 0);
        leader->velocityY = 0x60000;
        ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
        Func_020011a4();
    }
}

void Func_02000364(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);

    Func_02001274_(5, 13, 0, 5, 3, 56, 52);
    Func_020011cc_(11, 0x880000, 0x1180000);
    if (leader->x >> 20 == 8 && leader->z >> 20 == 17)
    {
        LEADER_HOP();
        ObjectMotion_OffsetPositionAndResetMotion(gPartyState.currentOwner, 16, 0);
        leader->velocityY = 0x60000;
        ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
        Func_020011a4();
    }
}

void Func_020003e4(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);

    Func_02001274_(5, 13, 0, 5, 3, 56, 52);
    Func_020011cc_(12, 0x680000, 0x1180000);
    if (leader->x >> 20 == 6 && leader->z >> 20 == 17)
    {
        LEADER_HOP();
        ObjectMotion_OffsetPositionAndResetMotion(gPartyState.currentOwner, 16, 0);
        leader->velocityY = 0x60000;
        ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
        Func_020011a4();
    }
}

void Func_02000464(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);

    Func_02001274_(5, 13, 0, 5, 3, 56, 52);
    Func_020011cc_(13, 0x580000, 0x1080000);
    if (leader->x >> 20 == 5 && leader->z >> 20 == 16)
    {
        LEADER_HOP();
        ObjectMotion_OffsetPositionAndResetMotion(gPartyState.currentOwner, 16, 0);
        leader->velocityY = 0x60000;
        ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
        Func_020011a4();
    }
}

void Func_020004e4(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);

    Func_02001274_(5, 13, 0, 5, 3, 56, 52);
    Func_020011cc_(14, 0x680000, 0xf80000);
    if (leader->x >> 20 == 6 && leader->z >> 20 == 15)
    {
        LEADER_HOP();
        ObjectMotion_OffsetPositionAndResetMotion(gPartyState.currentOwner, 16, 0);
        leader->velocityY = 0x60000;
        ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
        Func_020011a4();
    }
}

void Func_02000564(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);

    Func_02001274_(16, 13, 0, 4, 8, 61, 52);
    Func_020011cc_(8, 0x1180000, 0x1280000);
    if (leader->x >> 20 == 17 && leader->z >> 20 == 18)
    {
        Func_0200119c();
        Func_0200125c_(0);
        Func_02001204_(gPartyState.currentOwner, 0x102);
        ObjectMotion_OffsetPositionAndResetMotion_(gPartyState.currentOwner, 0, -16);
        leader->velocityY = 0x60000;
        ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
        Func_020011a4();
    }
}

void Func_020005e8(void)
{
    Func_02001274_(7, 23, 0, 4, 5, 60, 56);
}

void Func_02000608(void)
{
    Func_02001274_(7, 23, 0, 4, 5, 60, 56);
    if (GameFlag_Test(0x9b5) == 0)
    {
        Func_0200127c_(249, 168, 424);
        ((struct GabombaCatacombsParty *)&gPartyState)->value254 = 0x19b5;
        gPartyState.battleRule24b = 2;
        Func_02001224(106, 0);
    }
}

void Func_0200066c(void)
{
    Func_0200116c_(0, 0, 1, 2, 7, 11);
    Func_02001164_(2, 4, 1, 1, 7, 14);
}

void Func_0200069c(void)
{
    Func_0200116c_(1, 0, 1, 2, 7, 11);
    Func_02001164_(2, 5, 1, 1, 7, 14);
}

void Func_020006cc(void)
{
    struct ObjectRuntime *object = Object_GetById(8);

    GameFlag_SetBit(0x200);
    object->motionFlags = 0;
    object->priorityFlags = 0;
    Func_02001164_(6, 23, 1, 1, 8, 23);
}

void Func_02000704(void)
{
    struct ObjectRuntime *object = Object_GetById(9);

    GameFlag_SetBit(0x201);
    object->motionFlags = 0;
    object->priorityFlags = 0;
    Func_02001164_(0, 4, 1, 1, 8, 15);
}

void Func_02000740(void)
{
    struct ObjectRuntime *object = Object_GetById(10);

    GameFlag_SetBit(0x202);
    object->motionFlags = 0;
    object->priorityFlags = 0;
    Func_02001164_(0, 4, 1, 1, 9, 16);
}

void Func_0200077c(void)
{
    struct ObjectRuntime *object = Object_GetById(11);

    GameFlag_SetBit(0x203);
    object->motionFlags = 0;
    object->priorityFlags = 0;
    Func_02001164_(0, 4, 1, 1, 8, 17);
}

void Func_020007b8(void)
{
    struct ObjectRuntime *object = Object_GetById(12);

    GameFlag_SetBit(0x204);
    object->motionFlags = 0;
    object->priorityFlags = 0;
    Func_02001164_(0, 4, 1, 1, 6, 17);
}

void Func_020007f0(void)
{
    struct ObjectRuntime *object = Object_GetById(13);

    GameFlag_SetBit(0x205);
    object->motionFlags = 0;
    object->priorityFlags = 0;
    Func_02001164_(0, 4, 1, 1, 5, 16);
}

void Func_0200082c(void)
{
    struct ObjectRuntime *object = Object_GetById(14);

    GameFlag_SetBit(0x206);
    object->motionFlags = 0;
    object->priorityFlags = 0;
    Func_02001164_(0, 4, 1, 1, 6, 15);
}

void Func_02000868(void)
{
    struct ObjectRuntime *object = Object_GetById(8);

    GameFlag_SetBit(0x200);
    object->motionFlags = 0;
    object->priorityFlags = 0;
    Func_02001164_(15, 18, 1, 1, 17, 18);
}
