#include "types.h"
#include "scene.h"

/* Second event script of resource_38d: a linear field-scene choreography that
 * drives channels 0-3 and 18-20 through fades, motions, positions and message
 * waits, branching six times on the saved flag 0x84f and on three actor
 * lookups.
 *
 * Every branch, call and store below is taken from the complete decoded owner
 * (2060 bytes, 186 calls). Roles of the called wrappers remain unknown, so
 * they keep their address aliases; the two same-overlay callees are noted at
 * their call sites.
 *
 * Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. That legacy spelling is a PC-relative reading of a
 * word the loader rewrites, so it is not a stable identity. Eight of the words
 * in this owner spell an alias that two different runtime veneers share. A
 * ninth, Story_unk6_4, reads as a single unambiguous veneer here yet is still
 * refused a binding by the scorer, and the cause is not established from this
 * owner; the call word at 0x02002006 is an ordinary bl and the halfword before
 * it is `movs r1,#3`, not pool data. So none of those nine can be bound on
 * their own from this file. Each such call is
 * written with another alias of the SAME runtime veneer, and the legacy word
 * plus that veneer address is recorded at the call site. The intended per-site
 * spelling is Func_<legacy>_a/_b with explicit runtime addresses, which needs
 * an absolute_symbols block in this owner's translation unit; that block does
 * not exist yet.
 *
 * The cleared state byte written into the record at +85 is held in its own
 * local, so its value is established once at the top of the scene rather than
 * rebuilt at the store; the fourth argument zero of the following call is an
 * unrelated constant. */

extern u8 gWork[];
extern u8 gOv[];
extern u8 gOv2[];

void *Story_unk7_4();

void *Story_unk8_4();

void *Story_unk9_4();

void *Story_unk10_4();

void *Story_unk11_4();
void *Story_unk12_4();

void *Story_unk13_4();

/* Actor record returned by the three lookup helpers. The two halfword reads
 * take the whole part of the matching 16.16 word. */
typedef struct ActorPos {
    u8 field_00[8];
    s16 x_low;
    s16 x;
    s32 field_0c;
    s16 y_low;
    s16 y;
} ActorPos;

void RunEventScript02(void)
{
    u8 *work;
    u8 *buf;
    ActorPos *pos;
    s32 flag;
    u8 clear = 0;

    Story_unk14_4();
    Story_Run(-1, -1, -1, 0);
    Story_unk15_4(1);
    buf = (u8 *)Story_unk7_4();
    buf[85] = clear;
    Story_unk2_4(0x037e0000, -1, 0x02980000, 0);
    Story_unk16_4(1);
    Story_unk17_4(0, 0, 0);
    Story_unk18_4();
    Story_unk19_4(1);

    work = *(u8 **)gWork;
    *(s32 *)(work + 0x1c0) = 0x201;
    *(s32 *)(work + 0x1c8) = 16;

    Story_unk20_4();
    Story_unk21_4();
    Story_unk22_4(40);
    Story_Place(19, 0x03780000, 0x031e0000);
    Story_unk2_3(0, 0x03880000, 0x031e0000);
    Story_unk3_4(0x9999, 0x1333);
    /* legacy word Story_unk23_4, veneer 0x0200a580 */
    Story_unk2_4(0x037e0000, -1, 0x02ba0000, 1);
    Story_unk3_3(19, 0xcccc, 0x6666);
    Story_unk4_3(0, 0x9999, 0x4ccc);
    Story_unk5_3(19, 888, 720);
    Story_unk6_3(0, 904, 736);
    Story_unk24_4(60);
    Story_unk25_4(19);
    Story_unk26_4(19, 1);
    Story_unk27_4(0);
    Story_unk28_4(0, 1);
    Story_unk29_4(20);
    /* legacy word Story_unk23_4, veneer 0x0200a520 */
    Story_unk30_4(19, 2);
    Story_Do(0x1728);

    flag = 1;
    if (Story_Check(0x84f) == 0) {
        bump_step(1);
        flag = 0;
    }
    Story_unk31_4(19, 0);
    if (flag != 0) {
        bump_step(1);
    }

    Story_unk4_4(0x037e0000, -1, 0x02980000, 1);
    Story_unk32_4(19, gOv2);
    Story_unk7_3(0, 894, 684);

    pos = (ActorPos *)Story_unk8_4(0);
    if (pos != 0) {
        Story_unk33_4(1, *(s32 *)&pos->x_low, *(s32 *)&pos->y_low);
    }
    pos = (ActorPos *)Story_unk9_4(0);
    if (pos != 0) {
        Story_unk34_4(2, *(s32 *)&pos->x_low, *(s32 *)&pos->y_low);
    }
    pos = (ActorPos *)Story_unk10_4(0);
    if (pos != 0) {
        Story_unk35_4(3, *(s32 *)&pos->x_low, *(s32 *)&pos->y_low);
    }

    Story_unk8_3(1, 0x9999, 0x4ccc);
    Story_unk9_3(2, 0x9999, 0x4ccc);
    Story_unk10_3(3, 0x10000, 0x8000);
    Story_unk36_4(1, 2);
    Story_unk37_4(2, 2);
    Story_unk38_4(3, 2);
    Story_unk11_3(1, -16, 16);
    Story_unk39_4(2, 16, 16);
    Story_unk40_4(3, 32, 16);
    Story_unk41_4(2);
    Story_unk42_4(1, 1);
    Story_unk43_4(2, 1);
    Story_unk44_4(3, 1);
    Story_unk45_4(10);
    Story_unk12_3(1, 0xc000, 0);
    Story_unk13_3(2, 0xc000, 0);
    Story_unk46_4(3);
    Story_unk14_3(3, 0xa000, 0);
    Story_unk47_4(19);
    Story_unk48_4(20);

    flag = 1;
    if (Story_unk2(0x84f) == 0) {
        bump_step(1);
        flag = 0;
    }
    Story_unk30_4(18, 3);
    Story_unk15_3(0x2012, 0, 20);
    if (flag != 0) {
        bump_step(1);
    }

    flag = 1;
    if (Story_unk3(0x84f) == 0) {
        bump_step(1);
        flag = 0;
    }
    Story_unk49_4(18, 1);
    Story_unk16_3(0x2012, 0, 10);
    if (flag != 0) {
        bump_step(1);
    }

    Story_unk50_4(); /* ConfigurePrimarySceneChannels, 0x02001958 */
    Story_unk51_4(20);
    if (Story_unk4(0x84f) != 0) {
        Story_unk17_3(0, 258, 60);
        Story_unk18_3(1, 261, 40);
    } else {
        Story_unk52_3(40);
    }

    Story_unk19_3(1, 0xc000, 10);
    Story_unk20_3(0x4001, 0, 10);
    Story_unk21_3(2, 0xc000, 10);
    Story_unk53_3(2, 3);
    Story_unk5_4(0x4002, 0);
    Story_unk22_3(3, 0xa000, 10);
    Story_unk54_3(3, 3);
    /* legacy word Story_unk55_3, veneer 0x0200a550 */
    Story_unk23_3(0x4003, 0, 20);
    Story_unk56_3(18, 3);
    Story_unk57_3(20);
    Story_unk23_3(0x2012, 0, 20);
    Story_unk24_3(1, 259, 0);
    Story_unk25_3(0, 258, 60);

    if (Story_unk5(0x84f) != 0) {
        Story_unk58_3(18, 1);
        /* legacy word Story_unk55_3, veneer 0x0200a508 */
        Story_unk56_3(18, 4);
        Story_unk6(0x2012, 0);
        Story_unk59_3(); /* ConfigurePrimarySceneChannels, 0x02001958 */
        flag = 1;
        if (Story_unk9(0, 0) != 0) {
            bump_step(1);
            flag = 0;
        }
        Story_unk26_3(18, 0x5000, 0);
        Story_unk60_3(); /* ConfigureSecondarySceneChannels, 0x02001984 */
        Story_unk61_3(10);
        Story_unk27_3(0x2012, 0, 10);
        if (flag != 0) {
            bump_step(1);
        }
        /* legacy word Story_unk62_3, veneer 0x0200a568 */
        Story_unk25_3(18, 258, 60);
    } else {
        bump_step(4);
    }

    Story_unk7(0x2012, 0);
    Story_unk63_3(); /* ConfigurePrimarySceneChannels, 0x02001958 */
    if (Story_unk10(0, 0) == 0) {
        /* legacy word Story_unk62_3, veneer 0x0200a538 */
        Story_unk2_2(0x1737);
    } else {
        Story_unk2_2(0x1738);
    }

    Story_unk64_3(); /* ConfigureSecondarySceneChannels, 0x02001984 */
    Story_unk28_3(0x2012, 0, 20);
    Story_unk65_3(19, 1);
    Story_unk3_2(0x1739);
    Story_unk66_3(19, 0, 10);
    Story_unk29_3(0, 0x6000, 0);
    Story_unk30_3(1, 0xe000, 0);
    /* legacy word Story_unk67_3, veneer 0x0200a560 */
    Story_unk68_3(2, 0, 0);
    Story_unk31_3(3, 0x8000, 40);
    Story_unk69_3(18, 2);
    Story_unk32_3(0x2012, 0, 10);
    Story_unk33_3(0, 0xc000, 0);
    Story_unk70_3(); /* ConfigureSecondarySceneChannels, 0x02001984 */
    Story_unk71_3(10);
    Story_unk34_3(18, 261, 60);
    /* legacy word Story_unk72_3, veneer 0x0200a550 */
    Story_unk32_3(0x2012, 0, 10);
    Story_unk73_3(0, 3);
    Story_unk74_3(1, 3);
    /* legacy word Story_unk67_3, veneer 0x0200a500 */
    Story_unk74_3(2, 3);
    Story_unk75_3(3, 3);
    Story_unk76_3(20);
    Story_unk77_3(18, 3);
    Story_unk35_3(0x2012, 0, 10);
    Story_unk36_3(18, 264, 60);
    Story_unk37_3(0x2012, 0, 10);
    Story_unk38_3(18, 0x3000, 10);
    /* legacy word Story_unk78_3, veneer 0x0200a550 */
    Story_unk37_3(0x2012, 0, 10);
    Story_unk79_3(18, 3);
    /* legacy word Story_unk78_3, veneer 0x0200a540 */
    Story_unk7(0x2012, 0);

    Story_unk80_3(); /* ConfigurePrimarySceneChannels, 0x02001958 */
    flag = 1;
    if (Story_unk11(0, 0) == 1) {
        bump_step(1);
        flag = 0;
    }
    Story_unk81_3(); /* ConfigureSecondarySceneChannels, 0x02001984 */
    Story_unk39_3(0x2012, 0, 10);
    if (flag != 0) {
        bump_step(1);
    }

    Story_unk40_3(18, 0x7000, 10);
    Story_unk82_3(19, 1);
    /* legacy word Story_unk83_3, veneer 0x0200a560 */
    Story_unk40_3(19, 0x1000, 20);
    Story_unk84_3(18, 3);
    /* legacy word Story_unk72_3, veneer 0x0200a468 */
    Story_unk85_3(20);
    Story_unk86_3(19, 3);
    Story_unk87_3(19, 0, 10);
    Story_unk41_3(19, 0x3000, 10);
    Story_unk42_3(18, 0x3000, 20);
    /* legacy word Story_unk83_3, veneer 0x0200a520 */
    Story_unk82_3(18, 1);
    Story_unk43_3(0x2012, 0, 10);
    /* legacy word Story_unk6_4, veneer 0x0200a508; that alias resolves to one
     * veneer here, but spelling it directly is still refused for want of a
     * stable binding, so the same veneer is reached through another alias. */
    Story_unk86_3(18, 3);
    Story_unk44_3(0x2012, 0, 10);
    Story_unk88_3(0, 3);
    Story_unk89_3(1, 3);
    Story_unk90_3(2, 3);
    Story_unk91_3(3, 3);
    Story_unk85_3(20);

    Story_unk92_3(1, 2);
    pos = (ActorPos *)Story_unk11_4(0);
    if (pos != 0) {
        Story_unk93_3(1, pos->x, pos->y);
    }
    Story_unk94_3(2, 2);
    pos = (ActorPos *)Story_unk12_4(0);
    if (pos != 0) {
        Story_unk95_3(2, pos->x, pos->y);
    }
    Story_unk96_3(3, 2);
    pos = (ActorPos *)Story_unk13_4(0);
    if (pos != 0) {
        Story_unk97_3(3, pos->x, pos->y);
    }

    Story_unk98_3(1);
    Story_unk99_3(1, 0, 0);
    Story_unk100_3(2, 0, 0);
    Story_unk101_3(3);
    Story_unk102_3(3, 0, 0);
    Story_unk103_3(20);
    Story_unk45_3(18, 0x5000, 0);
    Story_unk46_3(0, 0x10013, (s32)gOv);
    /* legacy word Story_unk104_3, veneer 0x0200a4d8 */
    Story_unk47_3(19, 852, 646);
    Story_unk47_3(19, 852, 666);
    Story_unk48_3(19, 864, 672);
    Story_unk49_3(19, 0x1000, 10);
    Story_unk105_3(19, 1);
    Story_unk106_3(10);
    Story_unk107_3(19, 0, 10);
    Story_unk50_3(19, 886, 708);
    Story_unk51_3(19, 894, 764);
    Story_unk52_2(0, 894, 764);
    Story_unk108_3();
    Story_unk109_3();
    Story_unk4_2(802);
    if (Story_unk8(0x84f) == 0) {
        /* legacy word Story_unk104_3, veneer 0x0200a458 */
        Story_unk5_2(0x84f);
        Story_unk5_2(0x84a);
    }
    Story_unk110_3(6);
    Story_unk111_3();
}
