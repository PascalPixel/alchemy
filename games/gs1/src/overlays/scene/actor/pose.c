#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/pose.h"

extern s16 gCell[];
extern u8 Value_000000b3;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 Value_000000b4;
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 *gWork;
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 Value_00002880;

u8 *Actor_Run107(s32);

u8 *Actor_Run108(s32);

u8 *Actor_Run109(s32);

/*
 * The import's main-image target is Actor_Run110; its second argument is a
 * pose id.
 */

/*
 * The eight-byte owner at 0x02000044 includes its one pool word, which holds
 * the returned table address 0x02009438.
 */

/*
 * The eight-byte owner at 0x0200007c includes its one pool word, which holds
 * the returned table address 0x020096d0.
 */

/*
 * Select the scene script at 0x020004b0.  The owner runs to 0x020004e6 and
 * also owns the alignment halfword at 0x020004ea and the literal pool at
 * 0x020004ec-0x02000507.  The 0x9a7 predicate is evaluated on both sides of
 * the scene-id test, so the two calls stay separate -- they are two sites on
 * disjoint paths.  Index 224 of the scene-state block at 0x02000240 is the
 * signed scene id.
 */

/* Four scripts embedded in this overlay's own data. */

/*
 * Import veneer; used in a condition, so the return type is stated and the
 * arity left open.
 */

/*
 * The calls appear in machine order; that order is what reproduces the
 * reference.
 */

/*
 * Loader-relocated ROM calls: each site names the pre-relocation call word the
 * image holds, not a runtime address.
 */

/*
 * Import veneers, declared old-style: one import name takes different argument
 * counts at different sites in this overlay, so no arity is asserted and none
 * of these results is used.
 */
static __inline__ void ConfigureActorPose(s32 actor, s32 pose)
{
    SetActorPose(actor, pose);
}

s32 Actor_SetActor14Pose258(void)
{
    ConfigureActorPose(14, 258);
    return 0;
}

u8 *SceneData_GetTable9438(void)
{
    return (u8 *)0x02009438;
}

s32 SceneData_SelectTableByWord224(void)
{
    if (gCell[224] == (s32)&Value_000000b3) {
        return (s32)gOv;
    }
    return (s32)gOv2;
}

u8 *SceneData_GetTable96d0(void)
{
    return (u8 *)0x020096d0;
}

u8 *SceneData_SelectTableBySceneIdAndFlag9a7(void)
{
    if (gCell[224] == (s32)&Value_000000b4) {
        if (Actor_Check128(0x9A7) != 0) {
            return gOv4;
        }
        return gOv5;
    }
    return gOv3;
}

void Scene_RunSequenceA(void)
{
    s32 Actor_Run111(s32, s32);

    Actor_Do7(0x9BC);
    Actor_Run112();
    Actor_Run113();
    Actor_Do8(0xA);
    Actor_SetMode(0x780000, -1, 0x600000, 1);
    Actor_Run114();
    Actor_Do9(0x1E);
    Actor_Do10(0x288B);
    Actor_Apply2(0xC, 0);
    Actor_Do11(0xA);
    Actor_Place67(0, 0xC, 0);
    Actor_Do12(0x1E);
    Actor_Run111(0, 3);
    Actor_Do13(0x1E);
    Actor_Run115();
}

void Scene_RunThreeCallSequence(void)
{
    void Actor_Run116(s32, s32);

    Actor_Do14(0x9BC);
    Actor_Do15(0x288B);
    Actor_Run116(0xC, 0);
}

void State_ForwardWord16cAndApply7b(void)
{
    u8 *work = gWork;
    s16 *p = (s16 *)(work + 0x16C);

    Actor_Do16(*p);
    Actor_Do17(0x7B);
}

u8 *SceneData_SelectScriptBySceneIdAndFlag9a7(void)
{
    s32 Actor_Run117();

    if (gCell[224] == (s32)&Value_000000b4) {
        if (Actor_Run117(0x9a7) != 0) {
            return gOv6;
        }
        return gOv7;
    }
    if (Actor_Check129(0x9a7) != 0) {
        return gOv8;
    }
    return gOv9;
}

void Scene_RunPrimaryScript(void)
{
    void Actor_Run118();

    Actor_Run();
    Actor_Check(1);
    Actor_Place(0, 6291456, 12058624);
    Actor_Place2(0, 49152, 0);
    Actor_Place3(11, 0, 0);
    Actor_Place4(12, 32768, 0);
    Actor_Do(10320);
    Actor_Run2();
    Actor_Run3();
    Actor_Check2(10);
    Actor_Place5(0, 65536, 32768);
    Actor_Place6(0, 0, -16);
    Actor_Place7(0, 104, 136);
    Actor_Check3(10);
    Actor_Run4(1, -16, 16, 49152);
    Actor_Check4(3, 0, 24, 49152);
    Actor_Run5(2, 16, 16, 49152);
    Actor_Check5(1);
    Actor_Check6(20);
    Actor_Check7(10);
    Actor_Run6(2, 2);
    Actor_Check8(20);
    Actor_Run7(2, 0);
    Actor_Check9(10);
    Actor_Place8(11, 16384, 0);
    Actor_Place9(12, 16384, 0);
    Actor_Check10(30);
    Actor_Place10(11, 261, 70);
    Actor_Run8(11, 0);
    Actor_Check11(10);
    Actor_Run9(11, 4);
    Actor_Check12(20);
    Actor_Run10(11, 0);
    Actor_Check13(20);
    Actor_Run11(11, 2);
    Actor_Check14(20);
    Actor_Run12(11, 0);
    Actor_Check15(20);
    Actor_Place11(12, 258, 40);
    Actor_Run13(12, 0);
    Actor_Check16(10);
    Actor_Run14(12, 4);
    Actor_Check17(20);
    Actor_Run15(12, 0);
    Actor_Check18(20);
    Actor_Do2(10);
    Actor_Place12(11, 261, 50);
    Actor_Run16(11, 0);
    if (Actor_Check19(2495) == 0) {
        Actor_Run17();
    } else {
    Actor_Do3(20);
    Actor_Run18(11, 2);
    Actor_Check20(20);
    Actor_Run19(11, 0);
    Actor_Check21(10);
    Actor_Place13(1, 0, 50);
    Actor_Place14(0, 261, 60);
    Actor_Check22(10);
    Actor_Place15(1, 49152, 0);
    Actor_Check23(20);
    Actor_Run20(1, 0);
    Actor_Check24(10);
    Actor_Run21(3, 4);
    Actor_Check25(20);
    Actor_Run22(3, 0);
    Actor_Check26(10);
    Actor_Run23(2, 2);
    Actor_Check27(20);
    Actor_Run24(2, 0);
    Actor_Check28(20);
    Actor_Place16(12, 257, 40);
    Actor_Run25(12, 0);
    Actor_Run26(0, 0);
    Actor_Check29(20);
    Actor_Run27(12, 2);
    Actor_Check30(20);
    Actor_Run28(12, 0);
    Actor_Check31(20);
    Actor_Run29(0, 3);
    Actor_Check32(30);
    Actor_Check33(10);
    Actor_Run30(1, 2);
    Actor_Check34(20);
    Actor_Run31(1, 0);
    Actor_Check35(10);
    Actor_Run32(3, 4);
    Actor_Check36(20);
    Actor_Run33(3, 0);
    Actor_Check37(10);
    Actor_Run34(2, 3);
    Actor_Check38(30);
    Actor_Run35(2, 0);
    Actor_Check39(10);
    Actor_Place17(2, 0, 30);
    Actor_Run36(8194, 0);
    }
    Actor_Do4(10337);
    Actor_Check40(10);
    Actor_Place18(0, 16384, 0);
    Actor_Place19(1, 0, 20);
    if (Actor_Apply(0, 0) == 0) {
    Actor_Check41(20);
    Actor_Place20(0, 49152, 0);
    Actor_Place21(1, 49152, 0);
    Actor_Place22(2, 49152, 0);
    Actor_Check42(30);
    Actor_Check43(10);
    Actor_Run37(11, 3);
    Actor_Check44(30);
    Actor_Run38(11, 0);
    (*(u16 *)(*(u8 **)0x03001ebc + 0x1d8))++;
    } else {
    Actor_Do5(20);
    Actor_Place23(0, 49152, 0);
    Actor_Place24(1, 49152, 0);
    Actor_Place25(2, 49152, 0);
    Actor_Check45(30);
    Actor_Check46(10);
    Actor_Run39(11, 4);
    Actor_Check47(20);
    (*(u16 *)(*(u8 **)0x03001ebc + 0x1d8))++;
    Actor_Run40(11, 0);
    }
    Actor_Check48(10);
    Actor_Run41(11, 2);
    Actor_Check49(20);
    Actor_Run42(11, 0);
    Actor_Check50(10);
    Actor_Place26(1, 0, 0);
    Actor_Place27(3, 2, 0);
    Actor_Check51(40);
    Actor_Check52(10);
    Actor_Place28(11, 258, 40);
    Actor_Run43(11, 0);
    Actor_Check53(10);
    Actor_Place29(0, 49152, 0);
    Actor_Place30(1, 49152, 0);
    Actor_Place31(2, 49152, 0);
    Actor_Place32(3, 49152, 0);
    Actor_Check54(50);
    Actor_Check55(10);
    Actor_Run44(11, 3);
    Actor_Check56(30);
    Actor_Run45(11, 0);
    Actor_Check57(10);
    Actor_Run46(11, 2);
    Actor_Check58(20);
    Actor_Run47(11, 0);
    Actor_Check59(10);
    Actor_Place33(3, 256, 40);
    Actor_Run48(3, 0);
    Actor_Check60(20);
    Actor_Place34(11, 0, 0);
    Actor_Check61(20);
    Actor_Run49(11, 0);
    Actor_Check62(10);
    Actor_Run50(12, 2);
    Actor_Check63(20);
    Actor_Place35(12, 32768, 0);
    Actor_Check64(20);
    Actor_Run51(12, 0);
    Actor_Check65(10);
    Actor_Run52(11, 4);
    Actor_Check66(20);
    Actor_Run53(11, 0);
    Actor_Check67(10);
    Actor_Run54(2, 2);
    Actor_Check68(20);
    Actor_Run55(2, 0);
    Actor_Check69(10);
    Actor_Place36(11, 16384, 0);
    Actor_Check70(30);
    Actor_Run56(11, 0);
    Actor_Check71(10);
    Actor_Place37(12, 258, 50);
    Actor_Run57(12, 0);
    Actor_Check72(20);
    Actor_Place38(11, 257, 65);
    Actor_Place39(11, 0, 0);
    Actor_Check73(20);
    Actor_Run58(11, 0);
    Actor_Check74(20);
    Actor_Place40(12, 256, 40);
    Actor_Run59(12, 0);
    Actor_Check75(20);
    Actor_Place41(12, 16384, 0);
    Actor_Do6(40);
    Actor_Run60(12, 0);
    Actor_Check76(10);
    Actor_Place42(11, 16384, 0);
    Actor_Check77(50);
    Actor_Place43(11, 262, 60);
    Actor_Run61(11, 0);
    Actor_Check78(10);
    Actor_Run62(11, 3);
    Actor_Check79(30);
    Actor_Run63(11, 0);
    Actor_Check80(10);
    Actor_Run64(0, 2);
    Actor_Run65(1, 2);
    Actor_Run66(3, 2);
    Actor_Run67(2, 2);
    Actor_Check81(30);
    Actor_Check82(10);
    Actor_Place44(11, 0, 0);
    Actor_Check83(20);
    Actor_Run68(11, 0);
    Actor_Check84(10);
    Actor_Run69(12, 2);
    Actor_Check85(30);
    Actor_Place45(12, 32768, 0);
    Actor_Check86(20);
    Actor_Run70(12, 0);
    Actor_Check87(10);
    Actor_Place46(11, 257, 0);
    Actor_Check88(10);
    Actor_Place47(1, 57344, 0);
    Actor_Check89(30);
    Actor_Run71(1, 4);
    Actor_Check90(20);
    Actor_Run72(1, 0);
    Actor_Check91(10);
    Actor_Place48(12, 16384, 0);
    Actor_Check92(30);
    Actor_Place49(12, 261, 60);
    Actor_Run73(12, 0);
    Actor_Check93(10);
    Actor_Run74(1, 2);
    Actor_Check94(20);
    Actor_Run75(1, 0);
    Actor_Check95(10);
    Actor_Place50(11, 16384, 0);
    Actor_Check96(40);
    Actor_Place51(11, 258, 40);
    Actor_Run76(11, 0);
    Actor_Check97(10);
    Actor_Place52(2, 32768, 0);
    Actor_Check98(30);
    Actor_Place53(2, 263, 60);
    Actor_Place54(2, 49152, 0);
    Actor_Check99(30);
    Actor_Run77(2, 4);
    Actor_Check100(20);
    Actor_Run78(2, 0);
    Actor_Check101(10);
    Actor_Run79(11, 4);
    Actor_Check102(20);
    Actor_Run80(11, 0);
    Actor_Check103(10);
    Actor_Run81(0, 2);
    Actor_Run82(1, 2);
    Actor_Run83(3, 2);
    Actor_Run84(2, 2);
    Actor_Check104(30);
    Actor_Check105(10);
    Actor_Run85(11, 3);
    Actor_Check106(30);
    Actor_Run86(11, 0);
    Actor_Check107(20);
    Actor_Run87(0, 3);
    Actor_Run88(1, 3);
    Actor_Run89(3, 3);
    Actor_Run90(2, 3);
    Actor_Check108(30);
    Actor_Check109(10);
    Actor_Run91(11, 2);
    Actor_Check110(20);
    Actor_Run92(11, 0);
    Actor_Check111(10);
    Actor_Run93(1, 2);
    Actor_Check112(20);
    Actor_Run94(1, 0);
    Actor_Check113(10);
    Actor_Run95(11, 3);
    Actor_Check114(30);
    Actor_Run96(11, 0);
    Actor_Check115(20);
    Actor_Run97(3, 3);
    Actor_Check116(30);
    Actor_Run98(3, 0);
    Actor_Check117(20);
    Actor_Place55(0, 16384, 0);
    Actor_Place56(1, 57344, 0);
    Actor_Place57(2, 40960, 0);
    Actor_Check118(30);
    Actor_Run99(0, 3);
    Actor_Check119(10);
    Actor_Run100(1, 3);
    Actor_Run101(2, 3);
    Actor_Run102(3, 3);
    Actor_Check120(30);
    Actor_Place58(1, 78643, 39321);
    Actor_Place59(3, 78643, 39321);
    Actor_Place60(2, 78643, 39321);
    Actor_Run103(1, 2);
    {
        u8 *rec = Actor_Check121(0);
        if (rec != 0) {
            s16 y = *(s16 *)(rec + 10);
            s16 x = *(s16 *)(rec + 18);
            Actor_Place61(1, y, x);
        }
    }
    Actor_Check122(1);
    Actor_Place62(1, 0, 0);
    Actor_Run104(3, 2);
    {
        u8 *rec = Actor_Check123(0);
        if (rec != 0) {
            s16 y = *(s16 *)(rec + 10);
            s16 x = *(s16 *)(rec + 18);
            Actor_Place63(3, y, x);
        }
    }
    Actor_Check124(3);
    Actor_Place64(3, 0, 0);
    Actor_Run105(2, 2);
    {
        u8 *rec = Actor_Check125(0);
        if (rec != 0) {
            s16 y = *(s16 *)(rec + 10);
            s16 x = *(s16 *)(rec + 18);
            Actor_Place65(2, y, x);
        }
    }
    Actor_Check126(2);
    Actor_Place66(2, 0, 0);
    Actor_Check127(10);
    Actor_Run106();
}

/*
 * Cutscene script at 0x02000eec.  The owner runs to 0x0200103a and also owns
 * the alignment halfword at 0x0200103e and the literal pool at
 * 0x02001040-0x0200104b; the body is straight-line, with no branch.  The
 * script is a sequence of "act on channel N, then wait k frames" beats.  The
 * channel ids and beat constants are transcribed literally: what each channel
 * drives is not established, and the middle argument 0x105 is unidentified.
 */
void Scene_RunSecondaryScript(void)
{
    void Actor_Run119(s32);
    void Actor_Run120(s32);
    void Actor_Run121(s32);
    void Actor_Run122(s32);

    Actor_Run123(((s32)&Value_00002880));
    Actor_Run124(20);

    Actor_Run125(11, 2);
    Actor_Run126(20);
    Actor_Run127(11, 0);
    Actor_Run128(10);

    Actor_Run129(1, 0, 50);
    Actor_Run130(0, 0x105, 60);
    Actor_Run131(10);
    Actor_Run132(1, 0xc000, 0);
    Actor_Run133(20);
    Actor_Run134(1, 0);
    Actor_Run135(10);

    Actor_Run136(3, 4);
    Actor_Run137(20);
    Actor_Run138(3, 0);
    Actor_Run139(10);

    Actor_Run140(2, 2);
    Actor_Run141(20);
    Actor_Run142(2, 0);
    Actor_Run143(10);

    Actor_Run144(12, 2);
    Actor_Run145(20);
    Actor_Run146(12, 0);
    Actor_Run147(20);

    Actor_Run148(0, 2);
    Actor_Run149(20);
    Actor_Run119(25);

    Actor_Run150(1, 2);
    Actor_Run151(30);
    Actor_Run152(1, 0);
    Actor_Run153(30);

    Actor_Run154(1, 4);
    Actor_Run120(20);
    Actor_Run155(1, 0);
    Actor_Run156(10);

    Actor_Run157(3, 4);
    Actor_Run121(20);
    Actor_Run158(3, 0);
    Actor_Run159(10);

    Actor_Run160(2, 3);
    Actor_Run122(30);
    Actor_Run161(2, 0);
    Actor_Run162(10);

    Actor_Run163(2, 0, 30);
    Actor_Check130(0x2002, 0);
}
