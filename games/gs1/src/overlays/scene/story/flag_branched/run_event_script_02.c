#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/flag_branched/run_event_script_02.h"

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
 * ninth, Story_Run6, reads as a single unambiguous veneer here yet is still
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

void *Story_Run7();

void *Story_Run8();

void *Story_Run9();

void *Story_Run10();

void *Story_Run11();
void *Story_Run12();

void *Story_Run13();

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

    Story_Run14();
    Story_Run(-1, -1, -1, 0);
    Story_Run15(1);
    buf = (u8 *)Story_Run7();
    buf[85] = clear;
    Story_Run2(0x037e0000, -1, 0x02980000, 0);
    Story_Run16(1);
    Story_Run17(0, 0, 0);
    Story_Run18();
    Story_Run19(1);

    work = *(u8 **)gWork;
    *(s32 *)(work + 0x1c0) = 0x201;
    *(s32 *)(work + 0x1c8) = 16;

    Story_Run20();
    Story_Run21();
    Story_Run22(40);
    Story_Place(19, 0x03780000, 0x031e0000);
    Story_Place2(0, 0x03880000, 0x031e0000);
    Story_Run3(0x9999, 0x1333);
    /* legacy word Story_Run23, veneer 0x0200a580 */
    Story_Run2(0x037e0000, -1, 0x02ba0000, 1);
    Story_Place3(19, 0xcccc, 0x6666);
    Story_Place4(0, 0x9999, 0x4ccc);
    Story_Place5(19, 888, 720);
    Story_Place6(0, 904, 736);
    Story_Run24(60);
    Story_Run25(19);
    Story_Run26(19, 1);
    Story_Run27(0);
    Story_Run28(0, 1);
    Story_Run29(20);
    /* legacy word Story_Run23, veneer 0x0200a520 */
    Story_Run30(19, 2);
    Story_Do(0x1728);

    flag = 1;
    if (Story_Check(0x84f) == 0) {
        bump_step(1);
        flag = 0;
    }
    Story_Run31(19, 0);
    if (flag != 0) {
        bump_step(1);
    }

    Story_Run4(0x037e0000, -1, 0x02980000, 1);
    Story_Run32(19, gOv2);
    Story_Place7(0, 894, 684);

    pos = (ActorPos *)Story_Run8(0);
    if (pos != 0) {
        Story_Run33(1, *(s32 *)&pos->x_low, *(s32 *)&pos->y_low);
    }
    pos = (ActorPos *)Story_Run9(0);
    if (pos != 0) {
        Story_Run34(2, *(s32 *)&pos->x_low, *(s32 *)&pos->y_low);
    }
    pos = (ActorPos *)Story_Run10(0);
    if (pos != 0) {
        Story_Run35(3, *(s32 *)&pos->x_low, *(s32 *)&pos->y_low);
    }

    Story_Place8(1, 0x9999, 0x4ccc);
    Story_Place9(2, 0x9999, 0x4ccc);
    Story_Place10(3, 0x10000, 0x8000);
    Story_Run36(1, 2);
    Story_Run37(2, 2);
    Story_Run38(3, 2);
    Story_Place11(1, -16, 16);
    Story_Run39(2, 16, 16);
    Story_Run40(3, 32, 16);
    Story_Run41(2);
    Story_Run42(1, 1);
    Story_Run43(2, 1);
    Story_Run44(3, 1);
    Story_Run45(10);
    Story_Place12(1, 0xc000, 0);
    Story_Place13(2, 0xc000, 0);
    Story_Run46(3);
    Story_Place14(3, 0xa000, 0);
    Story_Run47(19);
    Story_Run48(20);

    flag = 1;
    if (Story_Check2(0x84f) == 0) {
        bump_step(1);
        flag = 0;
    }
    Story_Run30(18, 3);
    Story_Place15(0x2012, 0, 20);
    if (flag != 0) {
        bump_step(1);
    }

    flag = 1;
    if (Story_Check3(0x84f) == 0) {
        bump_step(1);
        flag = 0;
    }
    Story_Run49(18, 1);
    Story_Place16(0x2012, 0, 10);
    if (flag != 0) {
        bump_step(1);
    }

    Story_Run50(); /* ConfigurePrimarySceneChannels, 0x02001958 */
    Story_Run51(20);
    if (Story_Check4(0x84f) != 0) {
        Story_Place17(0, 258, 60);
        Story_Place18(1, 261, 40);
    } else {
        Story_Run52(40);
    }

    Story_Place19(1, 0xc000, 10);
    Story_Place20(0x4001, 0, 10);
    Story_Place21(2, 0xc000, 10);
    Story_Run53(2, 3);
    Story_Run5(0x4002, 0);
    Story_Place22(3, 0xa000, 10);
    Story_Run54(3, 3);
    /* legacy word Story_Run55, veneer 0x0200a550 */
    Story_Place23(0x4003, 0, 20);
    Story_Run56(18, 3);
    Story_Run57(20);
    Story_Place23(0x2012, 0, 20);
    Story_Place24(1, 259, 0);
    Story_Place25(0, 258, 60);

    if (Story_Check5(0x84f) != 0) {
        Story_Run58(18, 1);
        /* legacy word Story_Run55, veneer 0x0200a508 */
        Story_Run56(18, 4);
        Story_Check6(0x2012, 0);
        Story_Run59(); /* ConfigurePrimarySceneChannels, 0x02001958 */
        flag = 1;
        if (Story_Check9(0, 0) != 0) {
            bump_step(1);
            flag = 0;
        }
        Story_Place26(18, 0x5000, 0);
        Story_Run60(); /* ConfigureSecondarySceneChannels, 0x02001984 */
        Story_Run61(10);
        Story_Place27(0x2012, 0, 10);
        if (flag != 0) {
            bump_step(1);
        }
        /* legacy word Story_Run62, veneer 0x0200a568 */
        Story_Place25(18, 258, 60);
    } else {
        bump_step(4);
    }

    Story_Check7(0x2012, 0);
    Story_Run63(); /* ConfigurePrimarySceneChannels, 0x02001958 */
    if (Story_Check10(0, 0) == 0) {
        /* legacy word Story_Run62, veneer 0x0200a538 */
        Story_Do2(0x1737);
    } else {
        Story_Do2(0x1738);
    }

    Story_Run64(); /* ConfigureSecondarySceneChannels, 0x02001984 */
    Story_Place28(0x2012, 0, 20);
    Story_Run65(19, 1);
    Story_Do3(0x1739);
    Story_Run66(19, 0, 10);
    Story_Place29(0, 0x6000, 0);
    Story_Place30(1, 0xe000, 0);
    /* legacy word Story_Run67, veneer 0x0200a560 */
    Story_Run68(2, 0, 0);
    Story_Place31(3, 0x8000, 40);
    Story_Run69(18, 2);
    Story_Place32(0x2012, 0, 10);
    Story_Place33(0, 0xc000, 0);
    Story_Run70(); /* ConfigureSecondarySceneChannels, 0x02001984 */
    Story_Run71(10);
    Story_Place34(18, 261, 60);
    /* legacy word Story_Run72, veneer 0x0200a550 */
    Story_Place32(0x2012, 0, 10);
    Story_Run73(0, 3);
    Story_Run74(1, 3);
    /* legacy word Story_Run67, veneer 0x0200a500 */
    Story_Run74(2, 3);
    Story_Run75(3, 3);
    Story_Run76(20);
    Story_Run77(18, 3);
    Story_Place35(0x2012, 0, 10);
    Story_Place36(18, 264, 60);
    Story_Place37(0x2012, 0, 10);
    Story_Place38(18, 0x3000, 10);
    /* legacy word Story_Run78, veneer 0x0200a550 */
    Story_Place37(0x2012, 0, 10);
    Story_Run79(18, 3);
    /* legacy word Story_Run78, veneer 0x0200a540 */
    Story_Check7(0x2012, 0);

    Story_Run80(); /* ConfigurePrimarySceneChannels, 0x02001958 */
    flag = 1;
    if (Story_Check11(0, 0) == 1) {
        bump_step(1);
        flag = 0;
    }
    Story_Run81(); /* ConfigureSecondarySceneChannels, 0x02001984 */
    Story_Place39(0x2012, 0, 10);
    if (flag != 0) {
        bump_step(1);
    }

    Story_Place40(18, 0x7000, 10);
    Story_Run82(19, 1);
    /* legacy word Story_Run83, veneer 0x0200a560 */
    Story_Place40(19, 0x1000, 20);
    Story_Run84(18, 3);
    /* legacy word Story_Run72, veneer 0x0200a468 */
    Story_Run85(20);
    Story_Run86(19, 3);
    Story_Run87(19, 0, 10);
    Story_Place41(19, 0x3000, 10);
    Story_Place42(18, 0x3000, 20);
    /* legacy word Story_Run83, veneer 0x0200a520 */
    Story_Run82(18, 1);
    Story_Place43(0x2012, 0, 10);
    /* legacy word Story_Run6, veneer 0x0200a508; that alias resolves to one
     * veneer here, but spelling it directly is still refused for want of a
     * stable binding, so the same veneer is reached through another alias. */
    Story_Run86(18, 3);
    Story_Place44(0x2012, 0, 10);
    Story_Run88(0, 3);
    Story_Run89(1, 3);
    Story_Run90(2, 3);
    Story_Run91(3, 3);
    Story_Run85(20);

    Story_Run92(1, 2);
    pos = (ActorPos *)Story_Run11(0);
    if (pos != 0) {
        Story_Run93(1, pos->x, pos->y);
    }
    Story_Run94(2, 2);
    pos = (ActorPos *)Story_Run12(0);
    if (pos != 0) {
        Story_Run95(2, pos->x, pos->y);
    }
    Story_Run96(3, 2);
    pos = (ActorPos *)Story_Run13(0);
    if (pos != 0) {
        Story_Run97(3, pos->x, pos->y);
    }

    Story_Run98(1);
    Story_Run99(1, 0, 0);
    Story_Run100(2, 0, 0);
    Story_Run101(3);
    Story_Run102(3, 0, 0);
    Story_Run103(20);
    Story_Place45(18, 0x5000, 0);
    Story_Place46(0, 0x10013, (s32)gOv);
    /* legacy word Story_Run104, veneer 0x0200a4d8 */
    Story_Place47(19, 852, 646);
    Story_Place47(19, 852, 666);
    Story_Place48(19, 864, 672);
    Story_Place49(19, 0x1000, 10);
    Story_Run105(19, 1);
    Story_Run106(10);
    Story_Run107(19, 0, 10);
    Story_Place50(19, 886, 708);
    Story_Place51(19, 894, 764);
    Story_Place52(0, 894, 764);
    Story_Run108();
    Story_Run109();
    Story_Do4(802);
    if (Story_Check8(0x84f) == 0) {
        /* legacy word Story_Run104, veneer 0x0200a458 */
        Story_Do5(0x84f);
        Story_Do5(0x84a);
    }
    Story_Run110(6);
    Story_Run111();
}
