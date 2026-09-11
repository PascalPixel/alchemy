#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/message_branch.h"

#define WORKSPACE (*(u8 **) 0x03001EBC)

#include "resource_38c.h"

extern u8 gWork[];

u8 *Actor_Run2(s32);

u8 *Actor_Run3(s32);

u8 *Actor_Run4(s32);

/*
 * The eight-byte owner at 0x02000030 includes its one pool word, which holds
 * the returned table address 0x02008598.
 */

/*
 * The eight-byte owner at 0x0200003c includes its one pool word, which holds
 * the returned table address 0x02008688.
 */

/*
 * The eight-byte owner at 0x0200011c includes its one pool word, which holds
 * the returned table address 0x020088f0.
 */

/*
 * Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds, not a runtime address.
 */

/*
 * Call sites spelled through these wrappers pass their constants straight into
 * the argument registers; a direct call precomputes a costly constant into a
 * pseudo shared with later uses in the block.  A value-returning call also
 * sets r0 last of its arguments.
 */

/* The workspace pointer this overlay reaches through. */

static __inline__ void SetScale(s32 actor, s32 scale, s32 duration)
{
    Actor_Place6(actor, scale, duration);
}

static __inline__ void SetScale2(s32 actor, s32 scale, s32 duration)
{
    void Actor_Run5(s32, s32, s32);

    Actor_Run5(actor, scale, duration);
}

u8 *SceneData_GetPrimaryTable(void)
{
    return (u8 *)RESOURCE38C_PRIMARY_TABLE_ADDRESS;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetSecondaryTable(void)
{
    return (u8 *)RESOURCE38C_SECONDARY_TABLE_ADDRESS;
}

s32 SceneData_PrepareTable86b0(void)
{
    Actor_Do8(0x020086B0);
    return 0x020086B0;
}

void Scene_RunActor16MessageBranch(void)
{
    void Actor_Run6(s32, s32);

    u32 dir;

    dir = *(u16 *)(Actor_Run2(0) + 6);
    Actor_Run7();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Actor_Apply(7, 16);
    } else {
        if (Actor_Check9(0x845) == 0) {
            Actor_Do9(0x13E3);
        } else {
            Actor_Do10(0x16F5);
        }
        Actor_Apply2(16, 0);
    }

    Actor_Run8();
}

void Scene_RunActor18MessageBranch(void)
{
    void Actor_Run6(s32, s32);

    u32 dir;

    dir = *(u16 *)(Actor_Run3(0) + 6);
    Actor_Run9();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Actor_Run6(9, 18);
    } else {
        if (Actor_Check10(0x845) == 0) {
            Actor_Do11(0x13E9);
        } else {
            Actor_Do12(0x16F9);
        }
        Actor_Apply3(18, 0);
    }

    Actor_Run10();
}

u8 *SceneData_GetTertiaryTable(void)
{
    return (u8 *)RESOURCE38C_TERTIARY_TABLE_ADDRESS;
}

void Scene_RunActor17MessageBranch(void)
{
    s32 Actor_Run11();

    u32 i;
    s32 record;
    u8 *dir;

    record = Actor_Check(0);
    dir = *(volatile u16 *)(record + 6);
    Actor_Run12();
    if ((u32)((s32)dir + -0xa001) <= 0x3ffe) {
        Actor_Run13(8, 17);
    } else {
        if (Actor_Check2(0x845) == 0) {
            Actor_Do(0x13e5);
            Actor_Run14(17, 0, 0);
            Actor_Run15(10);
            Actor_Check3(17, 0);
            Actor_Place(17, 0x3000, 10);
        } else {
            Actor_Do2(0x16f7);
            Actor_Run16(17, 0);
        }
    }
    Actor_Run17();
}

void Scene_ConfigureActor21Scene(void)
{
    Actor_Run18();
    Actor_Do13(0x13ed);
    Actor_Place7(21, 0, 0);
    Actor_Apply4(21, 0);
    SetScale(21, 0xc000, 10);
    Actor_Run19();
}

void Scene_RunActor24Sequence(void)
{
    u32 i;
    s32 record;

    Actor_Run20();
    Actor_Do3(0x13f0);
    Actor_Run21(24, 0, 20);
    Actor_Run22(24, 0, 0);
    Actor_Run23(10);
    Actor_Check4(24, 0);
    if (Actor_Check5(0, 0) != 0) {
        bump_step(1);
    }
    Actor_Run24(24, 0);
    Actor_Place2(24, 0x4000, 10);
    Actor_Run25();
}

void Scene_RunActor27Sequence(void)
{
    void Actor_Run26();

    u32 i;
    s32 record;

    Actor_Run27();
    Actor_Do4(0x13f6);
    Actor_Run28(27, 0, 0);
    Actor_Run29(10);
    Actor_Check6(27, 0);
    if (Actor_Check7(0, 0) != 0) {
        bump_step(1);
    }
    Actor_Run30(27, 0);
    Actor_Place3(27, 0x4000, 10);
    Actor_Run31();
}

void Scene_RunActor8Message(void)
{
    Actor_Run32();
    Actor_Do14(0x16E1);
    Actor_Apply5(8, 0);
    Actor_Run33();
}

void Scene_RunActor13Message(void)
{
    Actor_Run34();
    Actor_Do15(0x16EC);
    Actor_Apply6(13, 0);
    Actor_Run35();
}

void Scene_RunActor19MessageBranch(void)
{
    s32 Actor_Run36(s32);

    u32 dir;

    dir = *(u16 *)(Actor_Run4(0) + 6);
    Actor_Run37();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Actor_Apply7(2, 19);
    } else if (Actor_Run36(0x845) != 0) {
        Actor_Do16(0x16FB);
        Actor_Apply8(19, 0);
    } else {
        Actor_Do17(0x13EB);
        Actor_Apply9(19, 0);
    }

    Actor_Run38();
}

void Scene_RunActor21SequenceOnFlag300(void)
{
    void Actor_Run39();

    u32 i;
    s32 record;

    Actor_Run40();
    if (Actor_Check8(0x300) == 0) {
        Actor_Do5(0x16ff);
        Actor_Run41(21, 0);
        Actor_Place4(21, 0x8000, 20);
        Actor_Run42(21, 0);
        Actor_Run43(22, 2);
        Actor_Run(22, 0x102);
        Actor_Run44(60);
        Actor_Run45(22, 0);
        Actor_Run46(10);
        Actor_Do6(0x300);
    }
    Actor_Run47(21, 0, 0);
    Actor_Do7(0x1702);
    Actor_Run48(21, 0);
    Actor_Place5(21, 0xc000, 10);
    Actor_Run49();
}

void Scene_ConfigureActor22Scene(void)
{
    void Actor_Run50(s32, s32, s32);
    void Actor_Run51(s32, s32, s32);

    Actor_Run52();
    Actor_Do18(0x1703);
    Actor_Apply10(0x16, 0);
    Actor_Run50(0x16, 0, 0);
    Actor_Apply11(0x16, 0);
    Actor_Run51(0x16, 0, 0xA);
    Actor_Run53();
}

void Scene_ConfigureActor23Scene(void)
{
    void Actor_Run5(s32, s32, s32);

    Actor_Run54();
    Actor_Do19(0x1705);
    Actor_Apply12(23, 0);
    Actor_Place8(23, 0, 0);
    Actor_Apply13(23, 0);
    SetScale2(23, 0xc000, 10);
    Actor_Run55();
}

void Scene_RunActor27Message(void)
{
    s32 Actor_Run51(s32);
    void Actor_Run5(void);
    void Actor_Run56(s32, s32);

    Actor_Run57();
    Actor_Do20(0x170A);
    Actor_Apply14(27, 0);
    Actor_Run58();
}

void Scene_RunActor10MessageBranch(void)
{
    s32 Actor_Run59(s32);
    void Actor_Run60(void);
    void Actor_Run56(s32, s32);

    Actor_Run61();
    if (Actor_Run59(3) != 0) {
        Actor_Do21(0x146F);
    } else {
        Actor_Do22(0x13D9);
    }
    Actor_Run56(10, 0);
    Actor_Run60();
}

s32 Scene_SetupActor27OnEntry(void)
{
    u8 *Actor_Run62(s32);

    u8 *actor;
    u8 *record;
    s32 bits;

    *(s32 *)(WORKSPACE + 448) = 521;
    actor = Actor_Run62(27);
    /*
     * The stored zero is also the mask's starting value: -13 is built by
     * subtracting from the register the strb already set to zero, not by
     * materializing 0xf3 or negating 13.
     */
    actor[0x23] = bits = 0;
    record = *(u8 **)(actor + 0x50);
    bits -= 13;
    bits &= record[9];
    bits |= 8;
    record[9] = bits;
    return 0;
}
