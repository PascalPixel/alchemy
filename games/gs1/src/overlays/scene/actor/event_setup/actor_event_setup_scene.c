#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/event_setup/actor_event_setup_scene.h"


struct EventActor {
    u8 reserved_00[0x23];
    u8 flags;
    u8 reserved_24[0x2c];
    u8 *render_state;
};

struct Slot020008e0 {
    u8 head[6];
    u16 heading;
};

struct Actor02001060 {
    u8 head[12];
    s32 rank;
    u8 body[19];
    u8 flags;
};

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern s16 gCell[];
extern u8 Value_0000004a;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];

void *Actor_Run8(s32);

u8 *Actor_Run9();

u8 *Actor_Run10();

u8 *Actor_Run11();

u8 *Actor_Run12();

struct Actor02001060 *Actor_Run13(s32);
struct Actor *Actor_Run14(s32);
struct Actor *Actor_Run15(s32);

u8 *Actor_Run16();
u8 *Actor_Run17();
u8 *Actor_Run18();
u8 *Actor_Run19();

u8 *Actor_Run20();

/* Scene event steps and table getters for resource_3a2. */

/*
 * Table getter at 0x020000b0. The eight-byte owner includes its one pool word
 * at 0x020000b4, which holds 0x020098ec; the pc-relative load reads it. The
 * word is an address, returned without being dereferenced.
 */

/* Value-returning: the reference sets r1 before r0 at this site. */

/*
 * Prepare actor 14 at 0x020010b8: clear bit 1 of the bytes at +35 and +89,
 * clear the byte at +85, and install the callback at 0x02009061 in the record
 * at +0x6c -- that pool word is odd, so it is a Thumb entry and not data. The
 * zero stored at +85 is held in a local because a register carries it. The
 * callback drives the same bit-1 flag the clears here touch.
 */

/* Two sites reach this one symbol with different arities; old-style so both
 * calls are legal. */

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    void Actor_Run21(s32, s32, s32);

    Actor_Run21(actor, x, y);
}

static __inline__ void Scene_AdvanceStep(s32 amount)
{
    extern u8 gWork[];

    u8 *work = *(u8 **)gWork;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

s32 EventScript_PrepareActorRenderFlags(struct EventActor *actor)
{
    actor->flags &= ~1;
    actor->render_state[9] |= 0xc;
    actor->render_state[21] |= 0xc;
    return 0;
}

s32 OvObj_SetFacingTowardObject10(void *self)
{
    void *obj;

    obj = Actor_Run8(0xA);
    FIELD_AT_OFFSET(self, s16 *, 6) = Actor_Apply(FIELD_AT_OFFSET(obj, s32 *, 0x10) - FIELD_AT_OFFSET(self, s32 *, 0x10), FIELD_AT_OFFSET(obj, s32 *, 8) - FIELD_AT_OFFSET(self, s32 *, 8));
    return 0;
}

s32 SceneData_SelectTable97b4ByState(void)
{
    if (gCell[224] == (s32)&Value_0000004a) {
        return (s32)gOv;
    }
    return (s32)gOv2;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTable98ec(void)
{
    return (u8 *)0x020098ec;
}

s32 SceneData_SelectTable9918ByState(void)
{
    if (gCell[224] == (s32)&Value_0000004a) {
        return (s32)gOv3;
    }
    return (s32)gOv4;
}

void Dialogue_RunMessage1958Step(void)
{
    extern u8 *gWork;

    u8 *work;

    Actor_Run22();
    Actor_Do9(0x1958);
    Actor_Apply2(10, 0);

    if (Actor_Apply3(0, 0) == 1) {
        Actor_Do10(20);
        Actor_Apply4(10, 0);
    } else {
        work = gWork;
        *(u16 *)(work + 472) += 1;
        Actor_Apply5(10, 0);
    }

    Actor_Run23();
}

void Dialogue_RunActor11Message195d(void)
{
    Actor_Run24();
    Actor_Do11(0x195D);
    Actor_Apply6(11, 0);
    Actor_Run25();
}

void Dialogue_RunActor13Message1961(void)
{
    Actor_Run26();
    Actor_Do12(0x1961);
    Actor_Apply7(13, 0);
    Actor_Run27();
}

void Scene_RunPrimaryScript(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Actor_Run28(188);
    Actor_Place(0x2009788, 67, 6);
    *(u8 *)(Actor_Check9(0) + 85) = 0;
    Actor_Place2(0, 0xcccc, 0x6666);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    Actor_Run29(0, 2);
    Actor_Place3(0, 0, -16);
    Actor_Run30(16);
    Actor_Run31(2);
}

void Scene_RunScene3a2SequenceA(void)
{
    extern u8 gWork[];
    void Actor_Run32();

    u32 i;
    s32 record;

    Actor_Run33();
    Actor_Place4(8, 0x880000, 0xa80000);
    Actor_Place5(8, 0x5000, 0);
    Actor_Place6(0, 0x900000, 0xc80000);
    Actor_Place7(1, 0xa00000, 0xc00000);
    Actor_Place8(2, 0x800000, 0xc80000);
    Actor_Place9(3, 0x700000, 0xc00000);
    Actor_Place10(0, 0xc000, 0);
    Actor_Place11(1, 0xa000, 0);
    Actor_Place12(2, 0xc000, 0);
    Actor_Place13(3, 0xe000, 0);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x201;
    Actor_Run34();
    Actor_Run35();
    Actor_Run36(60);
    Actor_Run37(8, 3);
    Actor_Run38(20);
    Actor_Do(0x19e9);
    Actor_Run39(8, 0, 20);
    Actor_Run40(0, 3);
    Actor_Run41(1, 3);
    Actor_Run42(2, 3);
    Actor_Run43(3, 3);
    Actor_Run44(20);
    Actor_Run45(8, 4);
    Actor_Run46(20);
    Actor_Run47(8, 0, 20);
    Actor_Run48(3, 4);
    Actor_Run49(20);
    Actor_Run50(3, 0, 20);
    Actor_Run51(2, 2);
    Actor_Run52(20);
    Actor_Run53(2, 0, 20);
    Actor_Run54(8, 2);
    Actor_Place14(8, 0x5000, 20);
    Actor_Run55(8, 0, 20);
    Actor_Place15(2, 0x102, 60);
    Actor_Run56(120);
    Actor_Run57(1, 2);
    Actor_Run58(20);
    Actor_Place16(1, 0x6000, 20);
    Actor_Check(1, 0);
    if (Actor_Check2(0, 0) == 0) {
        Actor_Run59(20);
        Actor_Run60(3, 0, 20);
        Actor_Place17(3, 0x101, 60);
        Actor_Run61(3, 0, 20);
        Actor_Place18(0, 0xe000, 0);
        Actor_Place19(2, 0xe000, 0);
        Actor_Run62(1, 2);
        Actor_Run(1, 0x102);
        Actor_Run63(60);
        Actor_Place20(1, 0x8000, 20);
        Actor_Run64(1, 0, 20);
        bump_step(2);
    } else {
        bump_step(2);
        Actor_Run65(20);
        Actor_Run66(3, 0, 20);
        Actor_Run67(3, 3);
        Actor_Run68(20);
        Actor_Run69(3, 0, 20);
        Actor_Place21(1, 0x102, 60);
        Actor_Place22(0, 0xe000, 0);
        Actor_Place23(2, 0xe000, 0);
        Actor_Run70(1, 0, 20);
    }
    Actor_Run71(8, 2);
    Actor_Run72(20);
    Actor_Place24(0, 0xc000, 0);
    Actor_Place25(1, 0xa000, 0);
    Actor_Place26(2, 0xc000, 0);
    Actor_Place27(3, 0xe000, 0);
    Actor_Run73(20);
    Actor_Place28(8, 0x3000, 20);
    Actor_Run74(8, 0, 20);
    Actor_Run75(1, 2);
    Actor_Run76(20);
    Actor_Run77(1, 3);
    Actor_Run78(30);
    Actor_Run79(8, 3);
    Actor_Run80(20);
    Actor_Run81(8, 0, 20);
    Actor_Run82(8, 2);
    Actor_Run83(20);
    Actor_Run84(8, 0, 20);
    Actor_Run85(0, 3);
    Actor_Run86(1, 3);
    Actor_Run87(2, 3);
    Actor_Run88(3, 3);
    Actor_Run89(20);
    Actor_Run90(8, 3);
    Actor_Run91(20);
    Actor_Place29(8, 0xc000, 30);
    Actor_Run92(188);
    Actor_Place30(0x2009788, 67, 6);
    Actor_Place31(8, 0xcccc, 0x6666);
    Actor_Run93(8, 136, 136);
    Actor_Run94(8, 0, 0);
    Actor_Run95(188);
    Actor_Place32(0x200979e, 67, 6);
    Actor_Run96(60);
    Actor_Run97();
    Actor_Run98(1, 2);
    Actor_Run99(20);
    Actor_Place33(1, 0x6000, 20);
    Actor_Run100(1, 0, 20);
    Actor_Place34(0, 0x8000, 20);
    Actor_Run101(2, 2);
    Actor_Run102(20);
    Actor_Place35(3, 0x2000, 20);
    Actor_Run103(3, 0, 20);
    Actor_Run104(1, 1);
    Actor_Place36(0, 0xe000, 20);
    Actor_Run105(1, 0);
    Actor_Run106(20);
    Actor_Run107(2, 2);
    Actor_Run108(20);
    Actor_Place37(0, 0x8000, 20);
    Actor_Run109(2, 0, 20);
    Actor_Run110(0, 1);
    Actor_Run111(1, 1);
    Actor_Run112(3, 1);
    Actor_Run113(20);
    Actor_Run114(2, 4);
    Actor_Run115(20);
    Actor_Run116(2, 0, 20);
    Actor_Run117(3, 2);
    Actor_Run118(20);
    Actor_Run119(3, 0, 20);
    Actor_Place38(2, 0xb000, 20);
    Actor_Run120(2, 3);
    Actor_Run121(20);
    Actor_Run122(2, 3);
    Actor_Run123(20);
    Actor_Run124(2, 0, 20);
    Actor_Place39(1, 0x102, 60);
    Actor_Run125(1, 0, 20);
    Actor_Place40(2, 0xe000, 20);
    Actor_Run126(2, 4);
    Actor_Run127(20);
    Actor_Run128(2, 0, 20);
    Actor_Place41(0, 0x102, 0);
    Actor_Place42(3, 0x102, 60);
    Actor_Run129(3, 0, 20);
    Actor_Place43(2, 0xa000, 20);
    Actor_Run130(2, 3);
    Actor_Run131(20);
    Actor_Run132(2, 0, 20);
    Actor_Run133(0, 1);
    Actor_Run134(1, 1);
    Actor_Run135(3, 1);
    Actor_Run136(20);
    Actor_Place44(2, 0x8000, 0x4000);
    Actor_Run137(2, 128, 184);
    Actor_Place45(2, 0x4000, 20);
    Actor_Place46(0, 0xc000, 0);
    Actor_Place47(1, 0xa000, 0);
    Actor_Place48(3, 0xe000, 20);
    Actor_Run138(2, 3);
    Actor_Run139(20);
    Actor_Run32(2, 0, 20);
    Actor_Run140(0, 3);
    Actor_Run141(1, 3);
    Actor_Run142(3, 3);
    Actor_Run143(20);
    Actor_Place49(1, 0x8000, 0x4000);
    Actor_Place50(3, 0x8000, 0x4000);
    Actor_Run144(1, 144, 200);
    Actor_Run145(2, 144, 200);
    Actor_Run146(3, 144, 200);
    Actor_Run147(1);
    Actor_Run148(1, 0, 0);
    Actor_Run149(2);
    Actor_Run150(2, 0, 0);
    Actor_Run151(3);
    Actor_Run152(3, 0, 0);
    Actor_Run153();
}

void Dialogue_RunLine1956(void)
{
    void Actor_Run154(void);

    Actor_Run154();
    Actor_Apply8(0, 1);
    Actor_Apply9(0x1956, 1);
    Actor_Run155();
}

void ConfigureAndPlaceActorFourteen(void)
{
    void Actor_Run21(s32, s32, s32);

    s32 a = 21, b = 9;
    Actor_Run156(85, 9, 1, 1, a, b);
    Actor_Run157(100, 0, 0);
    PlaceActor(14, 0x01580000, 0x00980000);
}

void Scene_RunScene3a2(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Actor_SetRect(21, 73, 1, 1, 21, 9);
    Actor_Place51(100, -1, -1);
    Actor_Run158(14, 0, 0);
}

void Dialogue_RunActorFifteenByLeaderHeading(void)
{
    struct Slot020008e0 *Actor_Run159(s32);

    u32 heading = Actor_Run159(0)->heading;

    Actor_Run160();
    if (heading - 0xA001 <= 0x3FFE) {
        Actor_Do13(15);
    } else {
        Actor_Do14(0x1A1E);
        Actor_Apply10(15, 0);
    }
    Actor_Run161();
}

void Scene_RunEventTransition(void)
{
    u8 *record;
    s32 none;

    if (Actor_Check3(0x89a) == 0) {
    } else {
        Actor_Run162();
        Actor_Place52(10, 0x2180000, 0xd80000);
        Actor_Do2(0x18b5);
        Actor_Run163(10, 0, 20);
        Actor_Run164(0, 2);
        Actor_Run165(20);
        record = Actor_Run9(0);
        *(s32 *)((s32)record + 108) = 0x2008055;
        record = Actor_Check4(0);
        if ((*(s32 *)((s32)record + 16) >> 20) == 13) {
            Actor_Place53(0, 0x1b8, 200);
        }
        Actor_Place54(10, 0x20000, 0x10000);
        Actor_Run166(10, 2);
        Actor_Place55(10, 0x198, 216);
        {
            u8 *record = Actor_Run10(10);
            u32 flag = 1;

            flag = flag | record[35];
            record[35] = (u8)flag;
        }
        Actor_Run167(10);
        Actor_Place56(10, 0x8000, 20);
        Actor_Run168(10, 0, 20);
        Actor_Run169(10, 2);
        Actor_Run2(10, 0x102);
        Actor_Run170(60);
        Actor_Run171(10, 0, 20);
        Actor_Check5(10, 0x200962c);
        Actor_Run3(0x1280000, -1, 0x1580000, 1);
        Actor_Do3(0x8b0);
        Actor_Run172(10);
        Actor_Run173();
        Actor_Place57(0, 0x10000, 0x8000);
        Actor_Run4(0, 0x20096b8);
        Actor_Run174(0);
        Actor_Run175(10);
        none = 0;
        record = Actor_Run11(0);
        *(s32 *)((s32)record + 108) = none;
        Actor_Run176(30);
        Actor_Run177(10, 2);
        Actor_Run178(20);
        Actor_Place58(10, 0x5000, 120);
        Actor_Place59(10, 0x105, 60);
        Actor_Place60(0, 0x101, 60);
        Actor_Run179(10, 4);
        Actor_Run180(20);
        Actor_Run181(10, 0, 20);
        Actor_Run182();
    }
}

void Scene_RunActorCue(void)
{
    extern u8 gWork[];

    Actor_Run183();
    Actor_Do4(0x18b9);
    Actor_Place61(10, 0x105, 60);
    Actor_Check6(10, 0);
    if (Actor_Check7(0, 0) == 1) {
        Scene_AdvanceStep(1);
    }
    Actor_Run184(20);
    Actor_Run185(10, 4);
    Actor_Run186(20);
    Actor_Run187(10, 0, 20);
    Actor_Run188();
}

void Scene_RunActorExchange(void)
{
    Actor_Run189();
    Actor_Run190();
    Actor_Run191();
    Actor_Run192(30);
    Actor_Place62(0, 0x6000, 0);
    Actor_Run193(9, 1);
    Actor_Run194();
    Actor_Run195(8, 2);
    Actor_Run196(20);
    Actor_Do5(0x19cf);
    Actor_Run197(8, 0, 20);
    Actor_Run198(9, 1);
    Actor_Run199(20);
    Actor_Run200(9, 0, 20);
    Actor_Run201(8, 3);
    Actor_Run202(20);
    Actor_Run203(8, 0, 20);
    Actor_Place63(10, 0xd000, 20);
    Actor_Run204(10, 0, 20);
    Actor_Run205(8, 3);
    Actor_Run206(60);
    Actor_Place64(8, 0x102, 60);
    Actor_Run207(8, 4);
    Actor_Run208(20);
    Actor_Run209(8, 0, 20);
    Actor_Run210(10, 2);
    Actor_Run5(10, 0x102);
    Actor_Run211(60);
    Actor_Place65(10, 0xb000, 20);
    Actor_Run212(9, 5);
    Actor_Run213();
    Actor_Do6(0x8b1);
}

void Scene_RunActorSequence(void)
{
    extern u8 gWork[];
    void Actor_Run214();

    s32 mask;

    Actor_Run214();
    Actor_Run6(8, 0x102);
    Actor_Run215(8, 2);
    Actor_Run216(60);
    Actor_Do7(0x19da);
    Actor_Run217(8, 0, 20);
    Actor_Run7(10, 0x102);
    Actor_Run218(10, 4, 0);
    Actor_Run219(60);
    Actor_Run220(10, 0, 20);
    Actor_Run221(8, 1);
    Actor_Run222(20);
    Actor_Run223(8, 0, 20);
    Actor_Place66(10, 0xd000, 20);
    Actor_Run224(10, 3);
    Actor_Run225(20);
    Actor_Place67(8, 178, 0x114);
    Actor_Place68(10, 172, 0x11c);
    Actor_Run226(8);
    Actor_Place69(8, 0x5000, 0);
    Actor_Place70(10, 0xb000, 0);
    Actor_Run227(20);
    Actor_Run228(8, 2);
    Actor_Run229(20);
    mask = 254;
    Actor_Run230(8, 0, 20);
    *(u8 *)(Actor_Check10(8) + 90) &= mask;
    *(u8 *)(Actor_Check11(10) + 90) &= mask;
    Actor_Place71(8, 0x3333, 0x1999);
    Actor_Place72(10, 0x3333, 0x1999);
    Actor_Run231(8, 5);
    Actor_Run232(10, 6);
    Actor_Run233(20);
    Actor_Run234(125);
    Actor_Run235(8, 2, 0);
    Actor_Run236(9, 2, 0);
    Actor_Run237(10, 2, 0);
    Actor_Run238(10);
    Actor_Run239(30);
    Actor_Run240(8, 5);
    Actor_Run241(10, 6);
    Actor_Run242(20);
    Actor_Run243(125);
    Actor_Run244(8, 4, 0);
    Actor_Run245(9, 4, 0);
    Actor_Run246(10, 4, 0);
    Actor_Run247(10);
    Actor_Run248(9);
    Actor_Run249(8, 1);
    Actor_Run250(10, 1);
    Actor_Run251(50);
    Actor_Run252(10, 2, 0);
    Actor_Run253(20);
    Actor_Run254(10, 0, 20);
    Actor_Run255(8, 3);
    Actor_Run256(20);
    Actor_Check8(8, 0);
    Actor_Run257(8, 0, 20);
    Actor_Run258(8, 5);
    Actor_Run259(10, 6);
    Actor_Run260(20);
    Actor_Run261(125);
    Actor_Run262(8, 2, 0);
    Actor_Run263(9, 2, 0);
    Actor_Run264(10, 2, 0);
    Actor_Run265(10);
    Actor_Run266(30);
    Actor_Run267(8, 5);
    Actor_Run268(10, 6);
    Actor_Run269(20);
    Actor_Run270(125);
    Actor_Run271(8, 4, 0);
    Actor_Run272(9, 4, 0);
    Actor_Run273(10, 4, 0);
    Actor_Run274(10);
    Actor_Run275(40);
    Actor_Run276(8, 1);
    Actor_Run277(10, 1);
    Actor_Run278(10, 2, 0);
    Actor_Run279(20);
    Actor_Place73(10, 0xd000, 20);
    Actor_Run280(10, 0, 20);
    Actor_Run281(8, 3);
    Actor_Run282(20);
    Actor_Run283(8, 0, 30);
    Actor_Run284(8, 0, 20);
    {
        u8 *record = Actor_Run12(10);
        u32 flag = 1;

        flag = flag | record[90];
        record[90] = (u8)flag;
    }
    Actor_Place74(10, 0xcccc, 0x6666);
    Actor_Place75(10, 168, 0x128);
    Actor_Place76(10, 0xd000, 20);
    Actor_Run285(10, 5);
    Actor_Run286(8, 3);
    Actor_Run287(20);
    Actor_Run288(8, 0, 20);
    Actor_Run289(10, 3);
    Actor_Run290(20);
    Actor_Run291(10, 0, 20);
    Actor_Run292(8, 3);
    Actor_Run293(20);
    Actor_Run294(8, 0, 20);
    Actor_Run295(0, 3);
    Actor_Run296(20);
    Actor_Run297();
    Actor_Do8(0x8b2);
    *(s32 *)((*(u8 **)gWork + 0x1c0)) = 0x201;
    Actor_Run298(6);
}

void Scene_RunScriptedSteps0And1A12(void)
{
    Actor_Run299();
    Actor_Apply11(0, 1);
    Actor_Apply12(0x1A12, 1);
    Actor_Run300();
}

void Scene_RunPairedLayoutStepsThenSetOne(void)
{
    {
        s32 fifth = 1;
        s32 sixth = 2;

        Actor_SetRect2(5, 28, 5, 13, fifth, sixth);
    }
    {
        s32 fifth = 5;
        s32 sixth = 13;

        Actor_SetRect3(5, 28, 1, 2, fifth, sixth);
    }
    Actor_Do15(1);
}

void State_RunRect6x28Step(void)
{
    {
        s32 fifth = 1;
        s32 sixth = 2;

        Actor_SetRect4(6, 28, 5, 13, fifth, sixth);
    }
    {
        s32 fifth = 5;
        s32 sixth = 13;

        Actor_SetRect5(6, 28, 1, 2, fifth, sixth);
    }
    Actor_Do16(1);
}

s32 Actor_SetFlagBitByRankAgainstActorZero(struct Actor02001060 *actor)
{
    if (Actor_Run13(0)->rank > actor->rank) {
        actor->flags |= 2;
    } else {
        actor->flags &= 0xFD;
    }
}

void Actor_UpdateActorFourteenByDepth(void)
{
    struct Actor *current = Actor_Run14(0);
    struct Actor *other = Actor_Run15(14);

    if (current->f10 <= other->f10) {
        Actor_Apply13(14, 1);
    }
}

void ActorDraw_PrepareActorFourteenWithCallback(void)
{
    u8 zero;

    zero = 0;
    Actor_Run301();

    Actor_Run16(14)[35] &= 0xfd;
    Actor_Run17(14)[89] &= 0xfd;
    Actor_Run18(14)[85] = zero;
    *(void **)(Actor_Run19(14) + 108) = (void *)0x02009061;

    Actor_Run302(55, 16, 1, 1, 56, 18);
    Actor_Run303(55, 16, 1, 1, 20, 18);

    Actor_Run304(1);
    Actor_Run305(512);
    Actor_Run306(14, 2);
    Actor_Run307();
}

void Scene_SetSlot15Byte89AndRunStep(void)
{
    u8 *slot;

    Actor_Run308();
    {
        s32 fifth = 21;
        s32 sixth = 11;

        Actor_SetRect6(14, 6, 1, 2, fifth, sixth);
    }
    slot = Actor_Run20(15) + 89;
    *slot = 254;
    Actor_Do17(0x201);
    Actor_Run309();
}

s32 SceneData_SelectTableByWord224(void)
{
    if (gCell[224] == (s32)&Value_0000004a) {
        return (s32)gOv5;
    }
    return (s32)gOv6;
}
