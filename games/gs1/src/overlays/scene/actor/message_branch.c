#include "types.h"
#include "scene.h"

#define WORKSPACE (*(u8 **) 0x03001EBC)

#include "resource_38c.h"

extern u8 gWork[];

u8 *Actor_unk2_4(s32);

u8 *Actor_unk3_4(s32);

u8 *Actor_unk4_4(s32);

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
    Actor_unk6_3(actor, scale, duration);
}

static __inline__ void SetScale2(s32 actor, s32 scale, s32 duration)
{
    void Actor_unk5_4(s32, s32, s32);

    Actor_unk5_4(actor, scale, duration);
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
    Actor_unk8_2(0x020086B0);
    return 0x020086B0;
}

void Scene_RunActor16MessageBranch(void)
{
    void Actor_unk6_4(s32, s32);

    u32 dir;

    dir = *(u16 *)(Actor_unk2_4(0) + 6);
    Actor_unk7_4();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Actor_Apply(7, 16);
    } else {
        if (Actor_unk9(0x845) == 0) {
            Actor_unk9_2(0x13E3);
        } else {
            Actor_unk10_2(0x16F5);
        }
        Actor_Apply2(16, 0);
    }

    Actor_unk8_4();
}

void Scene_RunActor18MessageBranch(void)
{
    void Actor_unk6_4(s32, s32);

    u32 dir;

    dir = *(u16 *)(Actor_unk3_4(0) + 6);
    Actor_unk9_4();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Actor_unk6_4(9, 18);
    } else {
        if (Actor_unk10(0x845) == 0) {
            Actor_unk11_2(0x13E9);
        } else {
            Actor_unk12_2(0x16F9);
        }
        Actor_Apply3(18, 0);
    }

    Actor_unk10_4();
}

u8 *SceneData_GetTertiaryTable(void)
{
    return (u8 *)RESOURCE38C_TERTIARY_TABLE_ADDRESS;
}

void Scene_RunActor17MessageBranch(void)
{
    s32 Actor_unk11_4();

    u32 i;
    s32 record;
    u8 *dir;

    record = Actor_Check(0);
    dir = *(volatile u16 *)(record + 6);
    Actor_unk12_4();
    if ((u32)((s32)dir + -0xa001) <= 0x3ffe) {
        Actor_unk13_4(8, 17);
    } else {
        if (Actor_unk2(0x845) == 0) {
            Actor_Do(0x13e5);
            Actor_unk14_4(17, 0, 0);
            Actor_unk15_4(10);
            Actor_unk3(17, 0);
            Actor_Place(17, 0x3000, 10);
        } else {
            Actor_unk2_2(0x16f7);
            Actor_unk16_4(17, 0);
        }
    }
    Actor_unk17_4();
}

void Scene_ConfigureActor21Scene(void)
{
    Actor_unk18_4();
    Actor_unk13_2(0x13ed);
    Actor_unk7_3(21, 0, 0);
    Actor_Apply4(21, 0);
    SetScale(21, 0xc000, 10);
    Actor_unk19_4();
}

void Scene_RunActor24Sequence(void)
{
    u32 i;
    s32 record;

    Actor_unk20_4();
    Actor_unk3_2(0x13f0);
    Actor_unk21_4(24, 0, 20);
    Actor_unk22_4(24, 0, 0);
    Actor_unk23_4(10);
    Actor_unk4(24, 0);
    if (Actor_unk5(0, 0) != 0) {
        bump_step(1);
    }
    Actor_unk24_4(24, 0);
    Actor_unk2_3(24, 0x4000, 10);
    Actor_unk25_4();
}

void Scene_RunActor27Sequence(void)
{
    void Actor_unk26_4();

    u32 i;
    s32 record;

    Actor_unk27_4();
    Actor_unk4_2(0x13f6);
    Actor_unk28_4(27, 0, 0);
    Actor_unk29_4(10);
    Actor_unk6(27, 0);
    if (Actor_unk7(0, 0) != 0) {
        bump_step(1);
    }
    Actor_unk30_4(27, 0);
    Actor_unk3_3(27, 0x4000, 10);
    Actor_unk31_4();
}

void Scene_RunActor8Message(void)
{
    Actor_unk32_4();
    Actor_unk14_2(0x16E1);
    Actor_Apply5(8, 0);
    Actor_unk33_4();
}

void Scene_RunActor13Message(void)
{
    Actor_unk34_4();
    Actor_unk15_2(0x16EC);
    Actor_Apply6(13, 0);
    Actor_unk35_4();
}

void Scene_RunActor19MessageBranch(void)
{
    s32 Actor_unk36_4(s32);

    u32 dir;

    dir = *(u16 *)(Actor_unk4_4(0) + 6);
    Actor_unk37_4();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Actor_Apply7(2, 19);
    } else if (Actor_unk36_4(0x845) != 0) {
        Actor_unk16_2(0x16FB);
        Actor_Apply8(19, 0);
    } else {
        Actor_unk17_2(0x13EB);
        Actor_Apply9(19, 0);
    }

    Actor_unk38_4();
}

void Scene_RunActor21SequenceOnFlag300(void)
{
    void Actor_unk39_4();

    u32 i;
    s32 record;

    Actor_unk40_4();
    if (Actor_unk8(0x300) == 0) {
        Actor_unk5_2(0x16ff);
        Actor_unk41_4(21, 0);
        Actor_unk4_3(21, 0x8000, 20);
        Actor_unk42_4(21, 0);
        Actor_unk43_4(22, 2);
        Actor_Run(22, 0x102);
        Actor_unk44_4(60);
        Actor_unk45_4(22, 0);
        Actor_unk46_4(10);
        Actor_unk6_2(0x300);
    }
    Actor_unk47_4(21, 0, 0);
    Actor_unk7_2(0x1702);
    Actor_unk48_4(21, 0);
    Actor_unk5_3(21, 0xc000, 10);
    Actor_unk49_4();
}

void Scene_ConfigureActor22Scene(void)
{
    void Actor_unk50_4(s32, s32, s32);
    void Actor_unk51_4(s32, s32, s32);

    Actor_unk52_4();
    Actor_unk18_2(0x1703);
    Actor_Apply10(0x16, 0);
    Actor_unk50_4(0x16, 0, 0);
    Actor_Apply11(0x16, 0);
    Actor_unk51_4(0x16, 0, 0xA);
    Actor_unk53_4();
}

void Scene_ConfigureActor23Scene(void)
{
    void Actor_unk5_4(s32, s32, s32);

    Actor_unk54_4();
    Actor_unk19_2(0x1705);
    Actor_Apply12(23, 0);
    Actor_unk8_3(23, 0, 0);
    Actor_Apply13(23, 0);
    SetScale2(23, 0xc000, 10);
    Actor_unk55_4();
}

void Scene_RunActor27Message(void)
{
    s32 Actor_unk51_4(s32);
    void Actor_unk5_4(void);
    void Actor_unk56_4(s32, s32);

    Actor_unk57_4();
    Actor_unk20_2(0x170A);
    Actor_Apply14(27, 0);
    Actor_unk58_4();
}

void Scene_RunActor10MessageBranch(void)
{
    s32 Actor_unk59_4(s32);
    void Actor_unk60_4(void);
    void Actor_unk56_4(s32, s32);

    Actor_unk61_4();
    if (Actor_unk59_4(3) != 0) {
        Actor_unk21_2(0x146F);
    } else {
        Actor_unk22_2(0x13D9);
    }
    Actor_unk56_4(10, 0);
    Actor_unk60_4();
}

s32 Scene_SetupActor27OnEntry(void)
{
    u8 *Actor_unk62_4(s32);

    u8 *actor;
    u8 *record;
    s32 bits;

    *(s32 *)(WORKSPACE + 448) = 521;
    actor = Actor_unk62_4(27);
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
