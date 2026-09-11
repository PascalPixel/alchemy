#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/state/task/task.h"

/* overlays/scene/state/task/actor_motion.c */
typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj;

extern s32 gCell[];

Obj *State_Run15(s32);
Obj *State_Run16(s32);

void Actor_MovePairByTileOffset(s32 a0, s32 a1, s32 a2)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = State_Run15(gCell[125]);
    q = State_Run16(a0);
    State_Run17();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        State_SetMode(p, x, p->f0c, y);
    }
    State_Apply(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        State_SetMode2(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        State_Apply2(q, 4);
    } else {
        State_Apply3(q, 3);
    }
    State_Do6(226);
    State_Do7(p);
    State_Do8(288);
    State_Apply4(q, 2);
    State_Run18();
}

/* overlays/scene/state/task/actor_position.c */
s32 *State_Run19(s32 selector);

u8 *State_Run20(s32 no);

/* STILL-OPEN: adopt --where differing_bytes=11/64. alchemist.ts refused
 * (tiers: class, priority) -- matches the documented DEAD-END FINGERPRINT
 * (adjacent register swap in an asrs/str block), genuinely unfixed. */

/* Per-site veneers (raw sub_ symbols from the overlay .s). */

/*
 * One symbol per call site.  These names stand for loader-relocated call
 * words in the overlay's veneer table, not runtime addresses.
 */

void Actor_PlaceWithScale20000(s32 selector, s32 x, s32 z)
{
    s32 *actor = State_Run19(selector);

    if (actor != 0) {
        s32 scale = 0x20000;
        u8 *p = (u8 *)actor;
        u8 zero = 0;
        *(s32 *)((u8 *)actor + 48) = scale;
        *(s32 *)((u8 *)actor + 52) = scale >> 1;
        p += 91;
        *p = zero;

        State_Do9(actor);
        State_Apply5(actor, 5);
        State_SetMode3(actor, x << 16, actor[3], z << 16);
    }
}

/*
 * resource_3bb owner at 0x020030e8, 80 bytes plus one alignment halfword:
 * spawn an object and run a short fixed setup sequence on it.  `no' is
 * forwarded unchanged -- it is never freshly loaded before the first call.
 * The object's fields (0x30 and 0x34 taking the fixed 0x14000/0xa000 pair,
 * 0x5b cleared, 0xc read back for the position call) are named by position
 * from call shape alone and are not verified.
 */
void Actor_PlaceWithScale14000(s32 no, s32 x, s32 z)
{
    u8 *obj = State_Run20(no);

    if (obj == 0) {
        return;
    }

    {
        s32 scale = 0x14000;
        u8 *p = obj;
        u8 zero = 0;
        *(s32 *)(obj + 0x30) = scale;
        *(s32 *)(obj + 0x34) = scale >> 1;
        p += 0x5b;
        *p = zero;
    }

    State_Run21();
    State_Apply6(obj, 5);
    State_SetMode4(obj, x << 16, *(s32 *)(obj + 12), z << 16);
    State_Do10(obj);
    State_Apply7(obj, 1);
}

/* overlays/scene/state/task/actor_search.c */
/* The active subject's handle sits 500 bytes into the shared table. */
typedef struct ActiveSubjectSlot {
    u8 pad[500];
    void *handle;
} ActiveSubjectSlot;

extern u8 *gWork;
extern s16 gCell[];

u8 *State_Run22();

s32 *State_Run23();

s32 *State_Run24();

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay.
 * The occupancy lookups take the position block; the record pointer the call
 * sites also pass is not asserted as an argument. */

s32 *SceneData_FindSlotAtPosition(s32 *pos)
{
    s32 **slots = (s32 **)(gWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if ((pos[0] >> 20) == (p[2] >> 20)
            && (pos[1] >> 20) == (p[3] >> 20)
            && (pos[2] >> 20) == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

/*
 * Probe the two cells ahead of the active subject and return what occupies the
 * nearer one, else the further one, else zero. The 160-byte owner includes its
 * alignment bytes and two-word literal pool. Facing is the biased quadrant of
 * the halfword at +6, with no sign extension; each probe rounds x and z down to
 * whole units and re-centres them by half a unit, carrying y unrounded. Only
 * the record fields at +6, +8, +12 and +16 are asserted.
 */
s32 *Actor_FindOccupantAheadOfSubject(void)
{
    u8 *record;
    s32 facing;
    s32 position[3];
    s32 *occupant;

    record = State_Run22(((ActiveSubjectSlot *)gCell)->handle);

    /* 128 << 6 = 0x2000 bias, then masked to bits 14-15 (192 << 8). */
    facing = (*(u16 *)(record + 6) + 0x2000) & 0xc000;

    position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
    position[1] = *(s32 *)(record + 12);
    position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
    State_Run25(0x100000, facing, position);          /* 128 << 13 */

    occupant = State_Run23(position, record);
    if (occupant == 0) {
        position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
        position[1] = *(s32 *)(record + 12);
        position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
        State_Run26(0x200000, facing, position);      /* 128 << 14 */

        occupant = State_Run24(position, record);
    }

    return occupant;
}

/* overlays/scene/state/task/effect.c */
extern s16 gOv;

/* Remove the marker task and release its selected display slot.
 * Per-site veneers (raw sub_ symbols from the overlay .s). */

void Effect_RemoveMarkerTask(void)
{
    State_Do11((void *)0x0200ae85);
    State_Do12(gOv);
    gOv = -1;
}

/* overlays/scene/state/task/overlay_object.c */
u16 *State_Run27(s32 no);

/*
 * resource_3bb: look up an object by arg0, then scan the first 15 halfword
 * entries of its table at offset 0xd8 for one equal to arg1, calling a handler
 * with each matching index.  The owner at 0x02002380 is 60 bytes and carries
 * no literal pool.  The three callees are identified by call shape only, and
 * the table's role is inferred from this scan alone.
 */

void OvObj_NotifyMatchingEntries(s32 no, s32 val)
{
    u16 *tbl = State_Run27(no);
    s32 i;

    State_Run28(no, val);

    tbl = (u16 *)((char *)tbl + 0xd8);
    for (i = 0; i <= 14; i++) {
        if (tbl[i] == val) {
            State_Apply8(no, i);
        }
    }
}

/* overlays/scene/state/task/party.c */

typedef struct PartyInteractionRecord {
    u8 padding_00[10];
    s16 x;
    u8 padding_0c[6];
    s16 y;
} PartyInteractionRecord;

PartyInteractionRecord *GetPartyInteractionRecord(void);

static inline void InitializeActorZero(void)
{
    State_Place19(0, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    State_Place20(actorId, 0x10000, 0x8000);
}

void RunPartyCountInteractionCopyA(s32 actorId)
{
    PartyInteractionRecord *record;
    s32 x;
    s32 y;

    record = GetPartyInteractionRecord();
    x = record->x;
    y = record->y;
    FinishPartyInteractionRecordRead();

    if (GetPartyMemberCount() <= 1) {
        SetInteractionCue(0x20e5);
        if (CheckActorInteraction(actorId, 0) == 0) {
            InitializeActorZero();
            InitializeSelectedActor(actorId);
            PlaceSelectedActor(actorId, x, y + 0x40);
            SetInteractionStep(15);
            PlaceActorZero(0, x, y);
            PlaceSupportActor(0, x, y + 0x20);
            StartInteractionPhase();
            AdvanceInteractionPhase();
            SelectInteractionStep(11);
        }
    } else {
        SetLargePartyInteractionCue(0x20e8);
        RunLargePartyInteraction(actorId, 0);
    }

    FinishPartyCountInteraction();
}

/* overlays/scene/state/task/run_scene_middle_sequence.c */
extern u8 gCell[];

void Scene_RunMiddleSequence(s32 mode, s32 owner, s32 base)
{
    s32 rec;
    s32 record;
    s32 p9;
    s32 p11;
    s32 count;
    s32 state;
    s32 obj;
    s32 hi;
    s32 lo;
    s32 tail;
    s32 sx;
    s32 sy;
    s32 i;
    u8 buf[8];

    rec = State_Check(owner);
    p9 = *(s16 *)(rec + 10);
    p11 = *(s16 *)(rec + 18);
    if (mode != 3) {
        count = State_Run();
        for (i = 0; i < count; i++) {
            buf[i] = gCell[504 + i];
        }
        if (count <= 1) {
            State_Run29(0x2083);
            State_Run30(owner, 0);
            return;
        }
        if (State_Check2(base + 512) != 0) {
            State_Run29(0x2084);
            State_Run30(owner, 0);
            return;
        }
        if (mode == 2) {
            state = 0;
            State_Check12(6);
        } else {
            State_Do(0x207d);
            State_Check3(owner, 0);
            state = State_Check4(0, 0);
        }
        if (state == 0) {
            if (state < count) {
                for (i = 0; i < count; i++) {
                    State_Run31((s32)(s8)buf[i]);
                }
            }
            for (i = 0; i < count; i++) {
                if ((s32)(s8)buf[i] != 0) {
                    State_Run32((s32)(s8)buf[i]);
                }
            }
            obj = State_Run2();
            for (i = 0; i < count; i++) {
                State_Run33((s32)(s8)buf[i]);
            }
            for (i = 0; i < count; i++) {
                State_Run34((s32)(s8)buf[i]);
            }
            if (obj != -1) {
                goto L_main;
            }
        }
    }
    State_Run29(0x207e);
    State_Run30(owner, 0);
    return;
L_main:
    ((void (*)())State_Check13)(obj, 1);
    State_Do2(0x207f);
    State_Run35(owner, 0);
    State_Place(0, 0x10000, 0x8000);
    State_Place2(obj, 0x10000, 0x8000);
    State_Place3(owner, 0x10000, 0x8000);
    record = State_Check5(0);
    if (record != 0) {
        State_Run36(obj, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    hi = p11 + 16;
    State_Run37(obj, p9, hi);
    lo = p9 + 16;
    State_Place4(0, lo, hi);
    State_Run28(obj, 0, 30);
    State_Run38(obj, 3);
    tail = hi - 32;
    State_Run39(0, 3);
    State_Run40(owner, p9, tail);
    State_Place5(owner, lo, tail);
    State_Run41(0, obj);
    State_Run42(obj, p9, tail);
    State_Run43(owner, 1);
    State_Place6(owner, 0x8000, 0);
    State_Run44(obj, p9, p11 - 48);
    State_Run45(owner, p9, tail);
    State_Run46(owner, p9, p11);
    State_Run47(obj);
    State_Run48(base + 512);
    rec = State_Check6(obj);
    sx = *(volatile s32 *)(rec + 8) >> 20;
    State_Run49((obj << 4) + 880, sx);
    sy = *(volatile s32 *)(rec + 16) >> 20;
    State_Run27((obj << 4) + 888, sy);
}

/* overlays/scene/state/task/scene_data.c */
/* Return this overlay's state block. */

/*
 * Table getter for resource_3bb, published from the overlay's header.
 *
 * The eight-byte owner at 0x02000030 includes its one pool word at
 * 0x02000034; the load reads that word and returns it as an address,
 * without dereferencing it.
 */
u8 *SceneData_GetTableC414(void)
{
    return (u8 *)0x0200c414;
}

/*
 * Overlay resource_3bb. Exported getter for an in-image table, published
 * from the overlay header.
 */

/*
 * The eight-byte owner at 0x0200003c includes its one pool word, which holds
 * the returned address; the table is returned without being dereferenced.
 */
u8 *SceneData_GetTableC474(void)
{
    return (u8 *)0x0200c474;
}

/*
 * Overlay resource_3bb. Exported getter for an in-image table, published
 * from the overlay header.
 */

/*
 * The eight-byte owner at 0x02000044 includes its one pool word, which holds
 * the returned address; the table is returned without being dereferenced.
 */
u8 *SceneData_GetTableC48c(void)
{
    return (u8 *)0x0200c48c;
}

u8 *SceneData_GetTableC83c(void) { return (u8 *)0x0200c83c; }

/* overlays/scene/state/task/scene_motion.c */
extern u16 gOv2;
extern u16 gOv3;
extern u16 gOv4;
extern u16 gOv5;
extern u16 gOv6;
extern u16 gOv7;
extern u16 gOv8;
extern u16 gOv9;

/* Start an interpolated marker move from the current position. */

/* Per-site veneer (raw sub_ symbol from the overlay .s). */

void State_StartMarkerMove(u32 x, u32 y, u32 duration)
{
    gOv2 = (u16)x;
    gOv3 = (u16)y;
    gOv5 = gOv4;
    gOv7 = gOv6;
    gOv8 = (u16)duration;
    gOv9 = 0;

    {
        s32 frame_budget = 0xc80;
        State_Apply9(0x0200ae85, frame_budget);
    }
}

/* overlays/scene/state/task/scene_primary_script_head.c */
extern u8 gCell[];
extern u8 gOv10[];
extern u8 gOv11[];
extern u8 gWork[];

void Scene_RunSupplementalSequenceOne(void)
{
    extern s32 gCell2[][1];

    s32 i;
    s32 rec8;
    s32 rec7;
    s32 xa;
    s32 xb;
    s32 xd;
    s32 ya;
    s32 yb;
    s32 yd;
    s32 flag;
    s32 count;
    s32 record;
    s32 base5;

    rec8 = State_Check14(gCell2[125][0]);
    for (i = 22; i <= 25; i++) {
        rec7 = State_Check7(i);
        *(u8 *)(rec7 + 91) = 0;
        xa = *(s32 *)(rec7 + 8);
        xb = *(s32 *)(rec8 + 8);
        xd = xa - xb;
        if (xd >= 0) {
            if (xd > 0x9ffff) {
                continue;
            }
        } else {
            xb = xb - xa;
            if (xb > 0x9ffff) {
                continue;
            }
        }
        ya = *(s32 *)(rec7 + 16);
        yb = *(s32 *)(rec8 + 16);
        yd = ya - yb;
        if (yd >= 0) {
            if (yd > 0x9ffff) {
                continue;
            }
        } else {
            yb = yb - ya;
            if (yb > 0x9ffff) {
                continue;
            }
        }
        if (State_Check8(0x104) != 0) {
            yb = *(s32 *)(rec7 + 16);
        } else {
            yb = *(s32 *)(rec8 + 16);
            yd = *(s32 *)(rec7 + 44);
            yb = yb + yd;
        }
        *(s32 *)(rec8 + 16) = yb;
    }
    if (*(volatile s32 *)gOv11 != 0
        && *(s32 *)(rec7 + 56) == (s32)0x80000000) {
        if (*(volatile s32 *)gOv10 == 0) {
            State_SetRect(58, 28, 7, 1, 58, 13);
        } else {
            State_SetRect(58, 10, 1, 1, 58, 11);
        }
    } else {
        State_SetRect2(57, 11, 1, 1, 58, 11);
        State_SetRect3(58, 14, 7, 1, 58, 13);
    }
    base5 = (s32)gOv11;
    if (*(volatile s32 *)base5 == 0) {
        flag = *(volatile s32 *)gOv10 ^ 1;
        *(volatile s32 *)gOv10 = flag;
        if (flag != 0) {
            record = State_Check15(22);
            State_Run3(record, 0x3a80000, 0, 0xb80000);
            record = State_Check16(23);
            State_Run4(record, 0x3c80000, 0, 0xf80000);
            record = State_Check17(24);
            State_Run5(record, 0x3e80000, 0, 0xb80000);
            record = State_Check18(25);
            State_Run6(record, 0x4080000, 0, 0xf80000);
            State_Run50(31, 11);
        } else {
            record = State_Check19(22);
            State_Run7(record, 0x3a80000, 0, 0xd80000);
            record = State_Check20(23);
            State_Run8(record, 0x3c80000, 0, 0xd80000);
            record = State_Check21(24);
            State_Run9(record, 0x3e80000, 0, 0xd80000);
            record = State_Check22(25);
            State_Run10(record, 0x4080000, 0, 0xd80000);
            State_Run51(31, 10);
        }
    }
    count = *(volatile s32 *)base5 + 1;
    *(volatile s32 *)base5 = count;
    if ((u32)count > 119) {
        record = State_Check9(0x104);
        if (record == 0) {
            *(volatile s32 *)base5 = record;
        }
    }
}

void Scene_RunScene3bbSequenceA(void)
{
    u32 i;
    s32 record;

    *(volatile s32 *)gOv11 = 0;
    *(volatile s32 *)gOv10 = 0;
    State_Do3(0x2008715);
    State_Place7(22, 0x3a80000, 0xd80000);
    State_Place8(23, 0x3c80000, 0xd80000);
    State_Place9(24, 0x3e80000, 0xd80000);
    State_Place10(25, 0x4080000, 0xd80000);
    State_Run52(31, 10);
}

void Scene_RunScene3bb(s32 a0)
{
    u32 i;
    s32 record;

    State_Run53(40);
    State_Run54(41);
    State_Run55(1);
    State_Run56();
    State_Place11(8, 0x580000, 0x1000000);
    State_Place12(0, 0x780000, 0x1000000);
    State_Place13(8, 0x4000, 0);
    State_Place14(0, 0x4000, 0);
    if (a0 < 0) {
        State_Run57(8, 10);
        State_Run58(0, 35);
    } else {
        State_Run59(8, 8);
        State_Run60(0, 28);
    }
    State_Run61(1);
    State_Run11(0x680000, 0, 0xc00000, 0);
    State_Run62(a0);
    State_Run63();
}

void Scene_RunSecondActorInteraction(s32 a0)
{
    extern s16 gCell2[][1];

    u32 i;
    s32 rec;
    s32 record;

    if (gCell2[225][0] == 2) {
        State_Run64();
    } else {
        State_Run65();
        rec = State_Check10(a0, 2);
        if (rec == 0) {
            State_Do4(0x20a2);
            State_Run66();
            State_Run12(0x30000, 0x6000);
            State_Run13(0x3d80000, -1, 0xe80000, 1);
            State_Run67();
            State_Check11(a0, 0);
            State_Run68();
            State_Run69(a0, 0);
            State_Run70(0, 0x438, 0x108);
            State_Run71(15);
            State_Place15(0, 0x18000, 0xc000);
            State_Place16(0, 0x438, 216);
            State_Place17(0, 0x428, 216);
            State_Run72();
            State_Run73();
            State_Run14(-1, -1, -1, 0);
            State_Run74(a0, 0);
            State_Run75(0);
            State_Run76(0, 0);
            State_Run77(a0, 2);
        } else {
            if (rec == 1) {
                State_Do5(0x20a1);
                State_Run78(a0, 0);
            }
        }
        State_Place18(rec, a0, 2);
        ((void (*)())State_Check23)();
    }
}

/* overlays/scene/state/task/scene_setup.c */
typedef struct Ctl {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 f6;
    s16 f8;
} Ctl;

extern u8 gOv12[];
extern Ctl gOv13;
extern u8 *gIw;
extern u8 gOv14[];

void *State_Run79(s32);

s32 *State_Run80();

s32 *State_Run81();

s32 *State_Run82();

u8 *State_Run83();

u8 *State_Run84();
u8 *State_Run85();

/* Rect setup for resource_3bb.  Each call site is spelled with its own import
 * name even though several of them reach the same import. */

/* Contiguous unnamed leaf-owner run for resource_3bb. */

/*
 * Scene setup for resource_3bb: allocates a descriptor, stamps its parameter
 * block, binds the in-image data to a graphics handle, and installs the
 * per-frame task.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */

/* This overlay's own routine at file offset 0x3638, installed as a task. */

/* In-image descriptor at file offset 0x4194. */

void State_ConfigureRegionByActorElevenColumn(void)
{
    u8 *work;
    s32 v0;
    s32 v1;

    work = State_Run79(11);
    if ((*(s32 *)(work + 8) >> 20) == 36) {
        State_Do13(0x335);
        v0 = 0x23;
        v1 = 0x4D;
        State_SetRect4(0x23, 0x4E, 1, 1, v0, v1);
    } else {
        State_Do14(0x335);
        v0 = 0x23;
        v1 = 0x4D;
        State_SetRect5(0x22, 0x4D, 1, 1, v0, v1);
    }
}

void Scene_RunTwoStepSequence(void)
{
    State_Run86();
    State_Run87();
}

/*
 * One fixed line, then three whose fifth or sixth argument is a field of the
 * record fetched for participants 15, 16 and 17.  Records 15 and 16 contribute
 * their word at +8, record 17 its word at +16, which moves to the sixth
 * argument slot while a literal 18 takes the fifth.  The shift is arithmetic,
 * so the fields are signed fixed-point with 20 fractional bits.  Only those two
 * fields are asserted; what the six arguments mean is not established here.
 */
void State_ApplyRectsForActors15To17(void)
{
    s32 field;

    State_Run88(100, 11, 12, 4, 14, 11);

    field = State_Run80(15)[2] >> 20;
    State_Run89(13, 28, 1, 4, field, 11);

    field = State_Run81(16)[2] >> 20;
    State_Run90(13, 28, 1, 4, field, 11);

    field = State_Run82(17)[4] >> 20;
    State_Run91(13, 28, 4, 1, 18, field);
}

void Scene_RunStep15At29By26(void)
{
    State_Place21(15, 29, 26);
    State_Run92();
}

void Scene_RunStep15At33By26(void)
{
    State_Place22(15, 33, 26);
    State_Run93();
}

void Scene_RunStep16At45By26(void)
{
    State_Place23(16, 45, 26);
    State_Run94();
}

void Scene_RunStep16At49By26(void)
{
    State_Place24(16, 49, 26);
    State_Run95();
}

void Scene_RunStep17At40By23(void)
{
    State_Place25(17, 40, 23);
    State_Run96();
}

void Scene_RunStep17At40By25(void)
{
    State_Place26(17, 40, 25);
    State_Run97();
}

/*
 * resource_3bb: six steps, each calling its own near-identical handler stub
 * with a fixed id -- 896 through 936, eight apart -- and a second argument of
 * zero.  The owner at 0x02001fb8 is 66 bytes: its code plus one trailing
 * alignment halfword.  It reads no incoming argument, so it takes none.  The
 * six callees are identified by call shape only.
 */
void Scene_RunSixSteps896To936(void)
{
    State_Apply10(896, 0);
    State_Apply11(904, 0);
    State_Apply12(912, 0);
    State_Apply13(920, 0);
    State_Apply14(928, 0);
    State_Apply15(936, 0);
}

/*
 * Seven arguments: four in registers, three from the caller's stack; five are
 * stored as halfwords and two as words, which is how they are typed here. The
 * 232-byte owner includes the seven-word literal pool the body branches over.
 * When the story flag is clear, `second` is mirrored about `centre`. The
 * extent stored at +216 is passed back sign-extended from sixteen bits.
 * Descriptor layout is asserted only for the fields written here.
 */
void Scene_BuildDescriptorAndInstallTask(s32 first, s32 second, s32 mode, s32 centre,
                   s32 extra, s32 third, s32 fourth)
{
    u8 *descriptor;
    u8 *first_record;
    u8 *second_record;
    s32 handle;
    s32 extent;

    descriptor = State_Run83(59, 0x7170);
    handle = State_Check24(512);                /* 128 << 2 */

    *(u16 *)(descriptor + 222) = (u16)first;
    *(u16 *)(descriptor + 224) = (u16)second;
    *(u16 *)(descriptor + 226) = (u16)third;
    *(u16 *)(descriptor + 228) = (u16)fourth;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = State_Run84(first);
    second_record = State_Run85(second);

    if (State_Check25(0x109) == 0) {
        *(s32 *)(second_record + 8) =
            (centre << 1) - *(s32 *)(first_record + 8);
        *(s32 *)(second_record + 16) = *(s32 *)(first_record + 16);
    }

    *(u16 *)(descriptor + 218) = 0;
    *(u16 *)(descriptor + 220) = 0;

    State_Run98(gOv12, handle);

    extent = State_Check26();
    *(u16 *)(descriptor + 216) = (u16)extent;
    State_Check27((s16)extent, 512, handle);

    /* The task is published as its entry address plus the Thumb bit. */
    State_Run99((s32)State_Run100 + 1, 0xc76);

    State_Run101(handle);
}

void State_InitControlRecordAndStartTask(void)
{
    u8 *state = gIw;
    Ctl *m = &gOv13;

    State_Apply16(State_Check28(), (s32)(state + 240));
    if (State_Check29(0x109) == 0) {
        m->f0 = 1;
        m->f2 = 1;
        m->f4 = *(u16 *)(state + 224);
        m->f8 = 0;
        m->f6 = 0;
    }
    {
        s32 e = 0xc85;

        State_Apply17((s32)gOv14, e);
    }
}

/* overlays/scene/state/task/scene_state_interaction_head.c */
typedef struct Sub {
    u8 pad00[0x28];
    s16 *f28;
} Sub;

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[0x3c];
    Sub *f50;
    u8 f54;
} Obj;

extern u8 *gWork;
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;
extern u8 Value_0000207c;

u8 *State_Run102(s32);

u8 *State_Run103(s32);

Obj *State_Run104(s32);

Obj *State_Run105(s32);

void State_SetFlag331AndConfigureRegion46_17(void)
{
    u8 *p;

    State_Do15(0x331);
    p = State_Run102(20) + 85;
    *p = 0;
    {
        s32 p5 = 44;
        s32 p6 = 17;

        State_SetRect6(46, 17, 1, 1, p5, p6);
    }
}

void Scene_SetFlag332AndDrawTiles(void)
{
    u8 *slot;

    State_Do16(0x332);
    slot = State_Run103(21) + 85;
    *slot = 0;
    {
        s32 v5 = 50;
        s32 v6 = 17;

        State_SetRect7(46, 17, 1, 1, v5, v6);
    }
}

void Scene_SetFlag333AndDrawTiles(void)
{
    State_Do17(0x333);
    {
        s32 width = 32;
        s32 height = 77;

        State_SetRect8(32, 37, 1, 4, width, height);
    }
}

        s32 y = State_Check30(904);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        State_Place27(1, x, y);
    }
    {
        s32 x = State_Check31(912);
        s32 y = State_Check32(920);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        State_Place28(2, x, y);
    }
    {
        s32 x = State_Check33(928);
        s32 y = State_Check34(936);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        State_Place29(3, x, y);
    }
}

void State_Run106(void)
{
    extern s32 gCell[];

    u8 *state = gWork;
    s32 v = gCell[125];

    if (v != 0 && ((s32)(s16)*(u16 *)(state + 382) >> 10) == v
        && State_Check35(321) != 0) {
        u16 *p = (u16 *)(state + 386);
        s32 t = 99;

        *p = t;
    }
}

void State_Run107(void)
{
    extern s32 gCell[];

    u8 *state = gWork;
    s32 best = 8;
    s32 bestd = 0x100000;
    s32 n = gCell[125];
    Obj *p = State_Run104(n);
    s32 i;
    s32 *q;
    s32 base;

    State_Run108();
    for (i = 8; i <= 66; i++) {
        Obj *o = State_Run105(i);

        if (o != 0 && o->f54 == 1 && *o->f50->f28 == 165) {
            s32 dx = (p->f08 - o->f08) / 65536;
            s32 dy = (p->f10 - o->f10) / 65536;

            if (dy <= 0) {
                s32 a = dx;
                s32 d;

                if (a < 0) a = -a;
                if (dy < 0) dy = -dy;
                d = a + dy;
                if (d < bestd) {
                    best = i;
                    bestd = d;
                }
            }
        }
    }
    State_Do18(0x2085);
    State_Apply18(best, 0);
    q = (s32 *)(state + 448);
    *q = 512;
    *(s32 *)(state + 456) = 15;
    State_Run109(20);
    State_Run110();
    State_Run111();
    base = n << 4;
    State_Apply19(base + 880, p->f08 >> 20);
    {
        s32 v = p->f10 >> 20;

        State_Run112(base + 888, v);
    }
    n++;
    if (n > 3) {
        State_Do19(10);
        State_Do20(282);
    } else {
        State_Do21(n);
        State_Run113();
        State_Run114();
        *q = 0;
    }
    State_Run115();
}

s32 State_Run116(s32 a, s32 b)
{
    extern s16 gCell[];

    s32 v;
    s32 id;
    s32 r;

    State_Run117();
    State_Apply20(b, 5);
    v = gCell[224];
    if (v == (s32)&Value_0000008f) {
        id = (s32)&Value_00002076;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)&Value_00002078;
    } else {
        id = (s32)&Value_0000207a;
    }
    State_Do22(id);
    State_Apply21(a, 0);
    if (State_Check12(b + 512) != 0) {
        return 2;
    }
    if (State_Check36(b + 520) != 0) {
        r = State_Check37(0);
        if (r == 1) {
            return 2;
        }
        if (r == 2 || r == -1) {
            return 3;
        }
        return r;
    }
    State_Do23(b + 520);
    State_Do24((s32)&Value_0000207c);
    State_Apply22(a, 0);
    return State_Apply23(0, 0);
}

/* overlays/scene/state/task/shared.c */

extern u8 HexDigits[];

void Scene_RunSingleStep(void)
{
    State_Run118();
}

void Scene_Forward(void)
{
    State_Run119();
}

void Text_WriteU32AsHex(u8 *buf, u32 value)
{
    s32 i;

    buf += 8;
    *buf = 0;
    buf--;
    for (i = 7; i >= 0; i--) {
        *buf = HexDigits[value & 15];
        value >>= 4;
        buf--;
    }
}

/* overlays/scene/state/task/staged_actor.c */
u8 *State_Run120();

/* Reset the selected actor's motion fields after refreshing it. */

void StagedActor_ResetMotionAfterRefresh(s32 slot)
{
    u8 *actor = State_Run120(slot);
    State_Run121(actor);
    *(s32 *)(actor + 36) = 0;
    *(s32 *)(actor + 44) = 0;
    *(s32 *)(actor + 56) = (s32)0x80000000;
    *(s32 *)(actor + 64) = (s32)0x80000000;
}

/* overlays/scene/state/task/staged_actor_movement.c */
typedef struct SceneRecord {
    u8 pad_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 pad_14[14];
    u8 state;
    u8 pad_23;
    s32 motion_24;
    u8 pad_28[4];
    s32 motion_2c;
    s32 rate_x;
    s32 rate_z;
    u8 pad_38[33];
    u8 flags;
} SceneRecord;

typedef struct Position3 {
    s32 x;
    s32 y;
    s32 z;
} Position3;

extern u32 gOv15[];
extern s16 gCell[];

SceneRecord *State_Run122(Position3 *, SceneRecord *);
SceneRecord *State_Run123(Position3 *, SceneRecord *);
SceneRecord *State_Run124(Position3 *, SceneRecord *);

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */

/* This overlay's own occupancy lookup for a cell. The record pointer the call
 * sites also load is spelled here, although the lookup itself uses only the
 * position. */

/* In-image direction table: sixteen packed steps, high half x, low half z. */

SceneRecord *State_Run125();   /* scene record for a subject handle */

s32 State_Run126(SceneRecord *, Position3 *);  /* terrain probe */

void State_Run127(SceneRecord *, s32);         /* select presentation mode */

void State_Run128(s32);                        /* wait n frames */

void State_Run129(SceneRecord *, s32, s32, s32);   /* place at (x, y, z) */

void State_Run130(SceneRecord *, s32, s32, s32);   /* place at (x, y, z) */

void State_Run131(s32);                        /* play a cue */

void State_Run132(SceneRecord *);              /* re-attach the camera */

void State_Run133(s32);                        /* play a cue */

void State_Run134(SceneRecord *, s32);         /* select presentation mode */

/*
 * The push interaction: probe the tile one step ahead of the subject and, if
 * something occupies it, slide it and the subject one step on. Only the
 * occupant's position is committed. The 360-byte owner includes its four-word
 * literal pool. Record fields are asserted only where written: facing at +6,
 * position at +8/+12/+16, state at +34, occupancy flag bit 0 at +89, speeds
 * at +48/+52, and the words cleared at +36/+44.
 */
void StagedActor_PushActorAhead(void)
{
    SceneRecord *subject;
    SceneRecord *target;
    SceneRecord *blocker;
    u32 step;
    u32 dir;
    Position3 pos;
    u32 idx = 250;
    s32 zero;
    s32 handle;

    handle = *(s32 *)((u8 *)gCell + (idx << 1));
    subject = State_Run125(handle);

    dir = subject->facing >> 12;

    step = gOv15[dir];
    pos.x = subject->x + (s32)(step & 0xffff0000);
    pos.y = subject->y;
    step <<= 16;
    pos.z = subject->z + (s32)step;

    target = State_Run122(&pos, subject);
    if (target == 0) {
        return;
    }

    step = gOv15[dir];
    pos.x = target->x + (s32)(step & 0xffff0000);
    pos.y = target->y;
    step <<= 16;
    pos.z = target->z + (s32)step;

    blocker = State_Run123(&pos, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    pos.x = target->x;
    pos.y = target->y + 0x100000;
    pos.z = target->z;

    blocker = State_Run124(&pos, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    target->state = 2;
    zero = 0;

    step = gOv15[dir];
    pos.x = target->x + (s32)(step & 0xffff0000);
    pos.y = target->y;
    step <<= 16;
    pos.z = target->z + (s32)step;

    /* Terrain probe: signed, so only a positive code refuses the move. */
    if (State_Run126(target, &pos) > 0) {
        return;
    }

    State_Run127(subject, 8);
    State_Run128(15);

    target->rate_x = 0x3333;
    target->rate_z = 0x3333;
    State_Run129(target, pos.x, pos.y, pos.z);

    /* The same destination block, moved onto the subject this time. */
    subject->rate_x = 0x3333;
    subject->rate_z = 0x3333;
    State_Run130(subject, pos.x, pos.y, pos.z);

    State_Run131(0xee);
    State_Run132(target);
    State_Run133(0x120);

    target->x = pos.x;
    target->z = pos.z;
    target->motion_24 = zero;
    target->motion_2c = zero;

    State_Run134(subject, 1);
}

/* overlays/scene/state/task/state_update.c */

extern s16 gCell[];
extern u8 Value_0000000a;
extern volatile s32 gOv10;
extern volatile s32 gOv11;
extern s16 gOv;

s32 *State_Run135(s32);

s32 *State_Run136(s32);

s32 *State_Run137(s32);

/* Per-site veneers: both reach the same main-image import, but each names its
 * own loader-relocated call word rather than a runtime address. */

/* In-image status words. The 64-byte owner includes its alignment bytes and
 * the two-word literal pool that holds these two addresses. */

/* Address-taken rather than called. Overlays are LINKED at 0x02008000 and run
 * at 0x02000000, so a pooled function address carries the link-base form: the
 * reference word is 0x02008715, which is this symbol plus the Thumb bit that
 * `.thumb_set` supplies. A called symbol uses the raw run-time annotation; an
 * address-taken one does not. */

void State_StoreSlotTileXToWork832To848(void)
{
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 14;
    s32 sixth = 11;
    s32 *record;
    s32 value;

    State_SetRect9(100, 11, 12, 4, fifth, sixth);

    record = State_Run135(12);
    value = record[2] >> 20;
    State_Apply24(832, value);
    State_SetRect10(71, 16, 1, 1, value, 16);

    record = State_Run136(13);
    value = record[2] >> 20;
    State_Apply25(840, value);
    State_SetRect11(71, 16, 1, 1, value, 16);

    record = State_Run137(14);
    value = record[2] >> 20;
    State_Apply26(848, value);
    State_SetRect12(71, 16, 1, 1, value, 16);
}

void State_SetWorkByte35(void)
{
    FIELD_AT_OFFSET(*(void **)0x03001F30, s8 *, 0x35) = 1;
}

void State_SendWord250With6(void)
{
    s16 *tbl = gCell;

    State_Place30(*(s32 *)(tbl + 250), 6, 0);
}

void State_ApplyTable8715AndValue104(void)
{
    State_Apply27(0x2008715, 0xC85);
    State_Do25(0x104);
}

/*
 * Spin until the first status word reaches zero with the second equal to 75,
 * giving up after 600 polls. Both words are re-read on every pass, so they are
 * volatile and their reads must not be hoisted or merged across the poll call.
 * The plain while loop is the spelling that reproduces the reference. What the
 * two words mean is not established here -- only that another task publishes
 * them while this owner spins.
 */
void State_WaitForStatusWords(void)
{
    s32 cnt;

    /* The frame count is a literal ten. */
    State_Run138(10);

    cnt = 0;
    while (gOv10 != 0 || gOv11 != 75) {
        State_Run139(1);
        cnt++;
        if (cnt >= 600) {
            return;
        }
    }
}

void State_InstallTask8714AndApplyTwoRects(void)
{
    State_Do26(31);
    State_Do27(820);                 /* 205 << 2 */

    if (gOv10 != 0) {
        gOv11 = 0;
    }

    State_Do28(30);
    State_Do29(1);

    /* The task is published as its entry address with the Thumb bit set. The
     * `.thumb_set` alias the exact reconstruction emits for a Thumb symbol already carries
     * bit 0, so adding it again here overshoots by one. */
    State_Do30((s32)State_Run140);

    State_SetRect13(58, 28, 7, 1, 58, 13);
    State_SetRect14(57, 11, 1, 1, 58, 11);
}

void State_InitCursorWhenUnset(void)
{
    s16 *cursor = &gOv;

    if (*cursor == -1) {
        *cursor = State_Check38();
    }
}

/* Complete eight-byte state setter plus its sole four-byte pool word. */
void State_SetHalfword1000To9(void)
{
    extern u16 gOv13;

    u16 *p = &gOv13;
    u16 v = 9;
    *p = v;
}

void State_WaitUntilStatusNine(void)
{
    extern s16 gOv13;

    s16 *status = &gOv13;

    while (*status != 9) {
        State_Do31(1);
    }
}
