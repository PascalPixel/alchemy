/* NONMATCHING: resource_372:020031ac; 2716 / 2716 bytes, 57 differing
 * halfwords, 36 wrong instructions, 48 halfword edits. Shared FieldSprite
 * and FieldActor ownership restores both pools and most store scheduling.
 * Visual reloads, two facing-store blocks and delay-address ordering remain. */
#include "FIELD_EVENT.H"

struct Half {
    u16 value;
};

struct FieldActor *Func_0200472c();
void Func_0200481c();
void Func_02004824();
void Func_0200473c();
void Func_02004744(s32 actorId, const s32 *actions);
void Func_020046f4();
void Func_02004864();
void Func_0200485c();
void Func_02000134();
void Func_0200477c();
void Func_020047fc();
void Func_0200474c();
void Func_0200475c(s32 actorId, const s32 *actions);
void Func_020046bc();
void Func_020048b4();
void Func_0200480c();
void Func_020047c4();
u32 Func_02004654();
s32 Func_02004634();
void Func_02004874();
void Func_0200487c();
void Func_0200463c();
void Func_020047cc();
void Func_020046b4();
struct FieldActor *Func_02004834();
void Func_02004684();
s32 Func_02004644();
void Func_020046c4();
s32 Func_0200464c();
void Func_02004754();
void Func_02004794();
void Func_0200478c();
void Func_02003c48();
void Func_020046e4();
void Func_020046a4();
void Func_020046ec();
void Func_0200469c();
void Func_020040f0();
void Func_020048ac();
void Func_0200471c();
void Func_0200486c();
extern const s32 Data_0200cd6c[];
extern const s32 Data_0200ce04[];
extern const s32 Data_0200ce30[];
extern const s32 Data_0200ce5c[];
extern const s32 Data_0200ce88[];
extern const s32 Data_0200ceb4[];
extern const s32 Data_0200cec8[];
extern const s32 Data_0200cedc[];
void Func_02003ce4(void);
void Func_02003e18(void);
void Func_020045b8(void);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ struct FieldActor *Pointer0(struct FieldActor *(*f)())
{
    return f();
}

static __inline__ struct FieldActor *Pointer1(struct FieldActor *(*f)(), s32 id)
{
    return f(id);
}

void Scene_RunActorGroupDepartureSequence(void)
{
    struct FieldSprite *actorVisual;
    struct FieldSprite *groupVisual;
    struct FieldActor *actor;
    struct FieldActor *fieldActor;
    struct FieldActor *groupActor;
    struct FieldActor *work;
    u32 random;
    const s32 *entryActions;
    s32 zero;
    const s32 *moveActions;
    s32 phase;
    const s32 *departureActions;
    u8 *step;

    actor = Pointer1(Func_0200472c, 19);
    groupActor = Pointer1(Func_0200472c, 27);
    groupVisual = groupActor->sprite;
    actorVisual = actor->sprite;
    Call2(Func_0200481c, 0x10000, 0x2000);
    Call4(Func_02004824, 0x6e0000, -1, 0x58b0000, 1);
    Call3(Func_0200473c, 8, 0x13333, 0x9999);
    Call3(Func_0200473c, 26, 0x13333, 0x9999);
    Call3(Func_0200473c, 0, 0x13333, 0x9999);
    Call3(Func_0200473c, 22, 0x13333, 0x9999);
    entryActions = Data_0200cd6c;
    Func_02004744(8, entryActions);
    Func_020046f4(10);
    Func_02004744(26, entryActions);
    Func_02004864();
    Func_020046f4(10);
    Func_02004744(0, entryActions);
    Func_020046f4(10);
    Func_0200485c();
    Func_02004744(22, entryActions);
    Func_020046f4(128);
    Func_02000134();
    Call4(Func_02004824, 0xae0000, -1, 0x5940000, 1);
    Func_020046f4(104);
    Call4(Func_02004824, 0x990000, -1, 0x52d0000, 1);
    Call3(Func_0200477c, 9, 158, 0x4f8);
    Call3(Func_020047fc, 9, 0x2000, 0);
    Func_0200474c(8);
    Func_02004744(8, Data_0200ce04);
    Func_02004744(26, Data_0200ce30);
    Func_02004744(0, Data_0200ce5c);
    Func_0200475c(22, Data_0200ce88);
    Call3(Func_020046bc, 0x40000, 0x40000, 0x10000);
    Func_020048b4(145);
    Func_020046f4(20);
    Call3(Func_020046bc, 0x10000, 0x10000, 0x10000);
    Func_020046f4(60);
    Call3(Func_0200480c, 0, 0x101, 0);
    Call3(Func_0200480c, 26, 0x101, 0);
    Call3(Func_0200480c, 22, 0x101, 0);
    Call3(Func_0200480c, 8, 0x101, 0);
    Call3(Func_0200480c, 9, 0x101, 60);
    Func_020047c4(26, 8, 0);
    Func_020047c4(22, 0, 0);
    Func_020046f4(20);
    fieldActor = Pointer1(Func_0200472c, 0);
    random = Func_02004654();
    random = Func_02004634(random, 20) + 20;
    {
    /* FAKEMATCH: keep the byte initialization as a short-reach pool load. */
    struct Half initialStep = { 0 };

    zero = 0;
    *(u16 *)(((u8 *)fieldActor + 100)) = random;
    fieldActor = Pointer1(Func_0200472c, 22);
    random = Func_02004654();
    *(u16 *)(((u8 *)fieldActor + 100)) = (Func_02004634(random, 20) + 20);
    fieldActor = Pointer1(Func_0200472c, 26);
    random = Func_02004654();
    *(u16 *)(((u8 *)fieldActor + 100)) = (Func_02004634(random, 20) + 20);
    fieldActor = Pointer1(Func_0200472c, 8);
    random = Func_02004654();
    *(u16 *)(((u8 *)fieldActor + 100)) = (Func_02004634(random, 20) + 20);
    fieldActor = Pointer1(Func_0200472c, 9);
    random = Func_02004654();
    *(u16 *)(((u8 *)fieldActor + 100)) = (Func_02004634(random, 20) + 20);
    moveActions = Data_0200ceb4;
    Func_02004744(9, moveActions);
    Func_020046f4(30);
    Func_02004744(0, moveActions);
    Func_02004744(26, moveActions);
    Func_02004744(22, moveActions);
    Func_02004744(8, moveActions);
    Func_020046f4(10);
    Func_020048b4(17);
    Call3(Func_020046bc, 0x30000, 0x30000, 0x10000);
    Func_020048b4(145);
    Func_020046f4(30);
    Call3(Func_020046bc, 0x10000, 0x10000, 0x10000);
    Func_020046f4(120);
    Call3(Func_020046bc, 0x30000, 0x30000, 0x10000);
    Func_020048b4(145);
    Func_020046f4(40);
    Call3(Func_020046bc, 0x20000, 0x20000, 0x10000);
    Func_020046f4(60);
    Call3(Func_020046bc, 0x40000, 0x40000, 0x10000);
    Func_020048b4(145);
    Func_020046f4(20);
    Call3(Func_020046bc, 0x10000, 0x10000, 0x10000);
    Func_020046f4(60);
    Call3(Func_020046bc, 0x30000, 0x30000, 0x10000);
    Func_020048b4(145);
    Func_020046f4(40);
    Call3(Func_020046bc, 0x10000, 0x10000, 0x10000);
    Func_020046f4(60);
    Func_0200485c();
    Call3(Func_020046bc, 0x10000, 0x10000, 0x10000);
    Func_020046f4(1);
    Call3(Func_020046bc, -1, -1, 0xe666);
    Call2(Func_0200481c, 0x80000, 0x80000);
    Call4(Func_02004824, 0xd90000, -1, 0x43c0000, 1);
    Func_02004874(0, 0);
    Func_0200487c(40);
    Func_0200463c(40);
    Func_020047cc(19, 0);
    work = Func_0200472c(19);
    Func_020046b4(work, 0);
    work = Func_0200472c(27);
    Func_020046b4(work, 0);
    groupActor->scale_x = 0xcccc;
    groupActor->scale_y = 0xcccc;
    groupActor->priority_flags &= 254;
    groupVisual->priority = 1;
    actor->x.fixed = 0xc80000;
    actor->y.fixed = 0xc80000;
    actor->target_x = 0xc80000;
    actor->target_y = 0xc80000;
    actor->z.fixed = 0x3820000;
    actor->target_z = 0x3820000;
    step = ((u8 *)actor + 85);
    actor->motion_flags = initialStep.value;
    actor->priority_flags &= 254;
    actorVisual->priority = 0;
    work = Func_02004834();
    work->target_x = 0x80000000;
    work = Pointer0(Func_02004834);
    work->target_y = 0x80000000;
    work = Pointer0(Func_02004834);
    work->target_z = 0x80000000;
    work = Func_02004834();
    work->velocity_x = zero;
    work = Func_02004834();
    work->velocity_y = zero;
    work = Func_02004834();
    work->velocity_z = zero;
    Func_0200463c(1);
    Call4(Func_02004824, 0xf70000, 0x800000, 0x3950000, 0);
    Func_02004684();
    Func_0200463c(1);
    Call2(Func_02004874, 0x10003, 1);
    Call2(Func_02004874, 0x10000, 2);
    Func_0200487c(30);
    Func_0200463c(30);
    Value2(Func_02004644, (s32)Func_02003ce4, 0xc80);
    Func_02004744(19, Data_0200cedc);
    Call2(Func_0200481c, 0x20000, 0x7ae);
    Call4(Func_02004824, 0xaf0000, 0x600000, 0x43e0000, 1);
    do {
        Func_0200463c(1);
    } while (*(s16 *)(((u8 *)actor + 102)) != 8);
    Func_02004874(0, 0);
    Func_0200487c(60);
    Func_0200463c(60);
    Func_020046c4();
    work = Func_02004834();
    work->target_x = 0x80000000;
    work = Func_02004834();
    work->target_y = 0x80000000;
    work = Func_02004834();
    work->target_z = 0x80000000;
    phase = 0;
    work = Func_02004834();
    work->velocity_x = phase;
    work = Pointer0(Func_02004834);
    work->velocity_y = phase;
    work = Pointer0(Func_02004834);
    work->velocity_z = phase;
    Value1(Func_0200464c, (s32)Func_02003ce4);
    Func_02004754(19);
    Func_0200463c(1);
    Func_02004794(19, 0);
    groupActor->scale_x = 0x14000;
    groupActor->scale_y = 0x14000;
    groupVisual->unknown_20[3] = 2;
    groupVisual->scale = 0x14000;
    actor->scale_x = 0x20000;
    actor->scale_y = 0x20000;
    actor->x.fixed = phase;
    actor->z.fixed = phase;
    actor->target_x = phase;
    actor->target_z = phase;
    Func_0200463c(1);
    Func_02004794(23, 8);
    Call3(Func_0200478c, 9, 0xa90000, 0x4f00000);
    Call3(Func_020047fc, 9, 0xc000, 0);
    Func_02004794(9, 9);
    Call3(Func_0200478c, 26, 0x970000, 0x50c0000);
    Call3(Func_020047fc, 26, 0x8000, 0);
    Call2(Func_02004794, 26, 5);
    Call3(Func_0200478c, 8, 0xaa0000, 0x5210000);
    Call3(Func_020047fc, 8, 0x6000, 0);
    Func_02004794(8, 5);
    Call3(Func_0200478c, 0, 0xb90000, 0x5350000);
    Call3(Func_020047fc, 0, 0x2000, 0);
    Func_02004794(0, 17);
    Call3(Func_0200478c, 22, 0xa90000, 0x5680000);
    Call3(Func_020047fc, 22, 0x4000, 0);
    Func_02004794(22, 0);
    Call4(Func_02004824, 0xa60000, 0, 0x5390000, 0);
    Func_02004684();
    step[0] = phase;
    actor->target_x = 0x80000000;
    actor->target_y = 0x80000000;
    actor->target_z = 0x80000000;
    Func_02003c48();
    Call3(Func_0200478c, 27, 0xda0000, 0x4980000);
    Call4(Func_02004824, 0xd20000, 0, 0x4ac0000, 0);
    Func_02004684();
    groupActor->scale_x = 0x20000;
    groupActor->scale_y = 0x20000;
    Value2(Func_02004644, (s32)Func_02003e18, 0xc80);
    Func_02004754(10);
    Func_02004754(24);
    Func_02004754(25);
    Func_0200463c(1);
    groupActor = Pointer1(Func_0200472c, 10);
    groupVisual = groupActor->sprite;
    groupActor->priority_flags &= 254;
    groupActor->scale_x = 0x10000;
    groupActor->scale_y = 0x10000;
    {
        s32 shown = 0xd000;

        groupActor->facing = shown;
    }
    groupVisual->priority = 0;
    Func_02004794(10, 0);
    groupActor = Pointer1(Func_0200472c, 24);
    groupVisual = groupActor->sprite;
    groupActor->priority_flags &= 254;
    groupActor->scale_x = 0x10000;
    groupActor->scale_y = 0x10000;
    groupVisual->priority = 0;
    {
        s32 shown = 0xb000;

        groupActor->facing = shown;
    }
    Func_02004794(24, 5);
    groupActor = Pointer1(Func_0200472c, 25);
    groupVisual = groupActor->sprite;
    groupActor->priority_flags &= 254;
    groupActor->scale_x = 0x10000;
    groupActor->scale_y = 0x10000;
    {
        s32 shown = 0xb000;

        groupActor->facing = shown;
    }
    groupVisual->priority = 0;
    Func_02004794(25, 5);
    groupActor = Pointer1(Func_0200472c, 27);
    groupVisual = groupActor->sprite;
    Func_02003c48();
    actor->y.fixed = 0x300000;
    actor->x.fixed = 0xd60000;
    actor->z.fixed = 0x4c00000;
    actor->target_x = 0x80000000;
    actor->target_y = 0x80000000;
    actor->target_z = 0x80000000;
    groupVisual->priority = 1;
    Func_0200478c(27, 0xd60000, 0x4c00000);
    Call3(Func_020047fc, 24, 0xc000, 0);
    Call3(Func_020047fc, 25, 0xc000, 20);
    Call1(Func_020046e4, 0x166);
    Func_020046a4(0);
    Func_020046a4(1);
    Func_020046a4(2);
    Func_020046a4(3);
    Func_020046a4(4);
    Func_020046a4(5);
    Call2(Func_02004874, 0x10003, 1);
    Call2(Func_02004874, 0x10000, 2);
    Func_0200487c(120);
    Func_0200463c(160);
    Call2(Func_02004874, 0x7fff, 1);
    Call2(Func_02004874, 0x7fff, 2);
    Func_0200487c(80);
    Func_020046f4(80);
    Func_020046f4(100);
    Value1(Func_0200464c, (s32)Func_02003e18);
    groupVisual->scale = groupActor->scale_x;
    Call1(Func_020046ec, 0x166);
    Func_0200469c(0);
    Func_0200469c(1);
    Func_0200469c(2);
    Func_0200469c(3);
    Func_0200469c(4);
    Func_0200469c(5);
    Func_020040f0();
    Call3(Func_0200478c, 9, 0xa50000, 0x4cd0000);
    Func_02004794(9, 1);
    actor = Func_0200472c(9);
    {
        s32 shown = 0xe000;

        actor->facing = shown;
    }
    random = Func_02004654();
    *(u16 *)(((u8 *)actor + 100)) = (Func_02004634(random, 90) + 60);
    departureActions = Data_0200cec8;
    {
        s32 shown = 1;

        *(u16 *)(((u8 *)actor + 102)) = shown;
    }
    Func_02004744(9, departureActions);
    Call3(Func_0200478c, 26, 0xa50000, 0x4e60000);
    Func_02004794(26, 1);
    actor = Pointer1(Func_0200472c, 26);
    {
        s32 shown = 0xe000;

        actor->facing = shown;
    }
    random = Func_02004654();
    *(u16 *)(((u8 *)actor + 100)) = (Func_02004634(random, 90) + 60);
    *(u16 *)((((u8 *)actor + 100)) + 2) = (s32)2;
    Func_02004744(26, departureActions);
    Call3(Func_0200478c, 22, 0x980000, 0x5050000);
    Func_02004794(22, 1);
    actor = Func_0200472c(22);
    {
        s32 shown = 0xe000;

        actor->facing = shown;
    }
    random = Func_02004654();
    *(u16 *)(((u8 *)actor + 100)) = (Func_02004634(random, 90) + 60);
    {
        s32 shown = 3;

        *(u16 *)(((u8 *)actor + 102)) = shown;
    }
    Func_02004744(22, departureActions);
    Call3(Func_0200478c, 8, 0xb40000, 0x51f0000);
    actor = Func_0200472c(8);
    {
        s32 shown = 0xe000;

        actor->facing = shown;
    }
    random = Func_02004654();
    *(u16 *)(((u8 *)actor + 100)) = (Func_02004634(random, 90) + 60);
    {
        s32 shown = 4;

        *(u16 *)(((u8 *)actor + 102)) = shown;
    }
    Func_02004744(8, departureActions);
    Func_02004794(8, 6);
    Func_0200472c(22)->priority_flags &= 254;
    Func_0200472c(8)->priority_flags &= 254;
    Value2(Func_02004644, (s32)Func_020045b8, 0xc80);
    Call3(Func_0200478c, 0, 0xb50000, 0x4f90000);
    work = Func_0200472c(0);
    {
        s32 shown = 0xe000;

        work->facing = shown;
    }
    Func_02004794(0, 1);
    Call4(Func_02004824, 0xb50000, 0, 0x4f90000, 0);
    Func_02004684();
    Func_0200478c(10, 0, 0);
    Func_0200478c(19, 0, 0);
    Func_0200478c(24, 0, 0);
    Func_0200478c(25, 0, 0);
    Func_0200478c(23, 0, 0);
    Func_0200478c(27, 0, 0);
    Call3(Func_0200478c, 17, 0x900000, 0x42e0000);
    Call3(Func_0200478c, 18, 0x1140000, 0x4f60000);
    Func_0200463c(60);
    Call2(Func_02004874, 0x10003, 1);
    Call2(Func_02004874, 0x10000, 2);
    Func_0200487c(80);
    Func_020046f4(60);
    Func_020048ac();
    Func_020046f4(60);
    Func_0200471c(1);
    Func_0200486c();
    }
}
