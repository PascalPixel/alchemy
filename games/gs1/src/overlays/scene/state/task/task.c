#include "types.h"

/* overlays/scene/state/task/actor_motion.c */
#define Actor_MovePairByTileOffset Func_02000310
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

extern s32 Data_02000240[];

Obj *Func_02004282(s32);
Obj *Func_0200428a(s32);
void Func_02004278(void);
void Func_020041f2(Obj *, s32, s32, s32);
void Func_020041ca(Obj *, s32);
void Func_0200421e(Obj *, s32, s32, s32);
void Func_02004202(Obj *, s32);
void Func_0200420c(Obj *, s32);
void Func_0200447a(s32);
void Func_02004250(Obj *);
void Func_02004228(Obj *, s32);
void Func_02004488(s32);
void Func_02004324(void);

void Actor_MovePairByTileOffset(s32 a0, s32 a1, s32 a2)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Func_02004282(Data_02000240[125]);
    q = Func_0200428a(a0);
    Func_02004278();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Func_020041f2(p, x, p->f0c, y);
    }
    Func_020041ca(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Func_0200421e(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Func_02004202(q, 4);
    } else {
        Func_0200420c(q, 3);
    }
    Func_0200447a(226);
    Func_02004250(p);
    Func_02004488(288);
    Func_02004228(q, 2);
    Func_02004324();
}
#undef Actor_MovePairByTileOffset

/* overlays/scene/state/task/actor_position.c */
#define Actor_PlaceWithScale20000 Func_020030a8
#define Actor_PlaceWithScale14000 Func_020030e8
s32 *Func_02007128(s32 selector);
void Func_02006f34(s32 *actor);
void Func_02006f1c(s32 *actor, s32 mode);
void Func_02006f58(s32 *actor, s32 x, s32 y, s32 z);
u8 *Func_02007168(s32 no);
void Func_02006f74(void);
void Func_02006f5c(u8 *obj, s32 x);
void Func_02006f98(u8 *obj, s32 x, s32 z, s32 arg3);
void Func_02006fa6(u8 *obj);
void Func_02006f76(u8 *obj, s32 x);

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
    s32 *actor = Func_02007128(selector);

    if (actor != 0) {
        s32 scale = 0x20000;
        u8 *p = (u8 *)actor;
        u8 zero = 0;
        *(s32 *)((u8 *)actor + 48) = scale;
        *(s32 *)((u8 *)actor + 52) = scale >> 1;
        p += 91;
        *p = zero;

        Func_02006f34(actor);
        Func_02006f1c(actor, 5);
        Func_02006f58(actor, x << 16, actor[3], z << 16);
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
    u8 *obj = Func_02007168(no);

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

    Func_02006f74();
    Func_02006f5c(obj, 5);
    Func_02006f98(obj, x << 16, *(s32 *)(obj + 12), z << 16);
    Func_02006fa6(obj);
    Func_02006f76(obj, 1);
}
#undef Actor_PlaceWithScale20000
#undef Actor_PlaceWithScale14000

/* overlays/scene/state/task/actor_search.c */
#define SceneData_FindSlotAtPosition Func_02003b48
#define Actor_FindOccupantAheadOfSubject Func_02003cf8
/* The active subject's handle sits 500 bytes into the shared table. */
typedef struct ActiveSubjectSlot {
    u8 pad[500];
    void *handle;
} ActiveSubjectSlot;

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

u8 *Func_02007c5c();
void Func_02007b0e();
s32 *Func_02007896();
void Func_02007b40();
s32 *Func_020078c8();

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay.
 * The occupancy lookups take the position block; the record pointer the call
 * sites also pass is not asserted as an argument. */

s32 *SceneData_FindSlotAtPosition(s32 *pos)
{
    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
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

    record = Func_02007c5c(((ActiveSubjectSlot *)Data_02000240)->handle);

    /* 128 << 6 = 0x2000 bias, then masked to bits 14-15 (192 << 8). */
    facing = (*(u16 *)(record + 6) + 0x2000) & 0xc000;

    position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
    position[1] = *(s32 *)(record + 12);
    position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
    Func_02007b0e(0x100000, facing, position);          /* 128 << 13 */

    occupant = Func_02007896(position, record);
    if (occupant == 0) {
        position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
        position[1] = *(s32 *)(record + 12);
        position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
        Func_02007b40(0x200000, facing, position);      /* 128 << 14 */

        occupant = Func_020078c8(position, record);
    }

    return occupant;
}
#undef SceneData_FindSlotAtPosition
#undef Actor_FindOccupantAheadOfSubject

/* overlays/scene/state/task/effect.c */
#define Effect_RemoveMarkerTask Func_02003080
extern s16 Data_0200cb46;

void Func_02006e36(void *callback);
void Func_02006e90(s32 slot);

/* Remove the marker task and release its selected display slot.
 * Per-site veneers (raw sub_ symbols from the overlay .s). */

void Effect_RemoveMarkerTask(void)
{
    Func_02006e36((void *)0x0200ae85);
    Func_02006e90(Data_0200cb46);
    Data_0200cb46 = -1;
}
#undef Effect_RemoveMarkerTask

/* overlays/scene/state/task/overlay_object.c */
#define OvObj_NotifyMatchingEntries Func_02002380
u16 *Func_0200625c(s32 no);
void Func_0200626e(s32 no, s32 val);
void Func_0200628a(s32 no, s32 index);

/*
 * resource_3bb: look up an object by arg0, then scan the first 15 halfword
 * entries of its table at offset 0xd8 for one equal to arg1, calling a handler
 * with each matching index.  The owner at 0x02002380 is 60 bytes and carries
 * no literal pool.  The three callees are identified by call shape only, and
 * the table's role is inferred from this scan alone.
 */

void OvObj_NotifyMatchingEntries(s32 no, s32 val)
{
    u16 *tbl = Func_0200625c(no);
    s32 i;

    Func_0200626e(no, val);

    tbl = (u16 *)((char *)tbl + 0xd8);
    for (i = 0; i <= 14; i++) {
        if (tbl[i] == val) {
            Func_0200628a(no, i);
        }
    }
}
#undef OvObj_NotifyMatchingEntries

/* overlays/scene/state/task/party.c */
#define GetPartyInteractionRecord Func_02005e6a
#define FinishPartyInteractionRecordRead Func_02005e5e
#define GetPartyMemberCount Func_02005e3a
#define SetInteractionCue Func_02005f04
#define CheckActorInteraction Func_02005f24
#define PlaceSelectedActor Func_02005ee0
#define SetInteractionStep Func_02005e9e
#define PlaceActorZero Func_02005ef8
#define PlaceSupportActor Func_02005f04_a
#define StartInteractionPhase Func_02005ff0
#define AdvanceInteractionPhase Func_02005ffc
#define SelectInteractionStep Func_02005fc2
#define SetLargePartyInteractionCue Func_02005f6a
#define RunLargePartyInteraction Func_02005f82
#define FinishPartyCountInteraction Func_02005ee6
#define RunPartyCountInteractionCopyA Func_02001f14

typedef struct PartyInteractionRecord {
    u8 padding_00[10];
    s16 x;
    u8 padding_0c[6];
    s16 y;
} PartyInteractionRecord;

PartyInteractionRecord *Func_02005e6a(void);
void Func_02005e5e(void);
s32 Func_02005e3a(void);
void Func_02005f04();
void Func_02005f04_a();
s32 Func_02005f24(s32 actorId, s32 mode);
void Func_02005eae(s32 actorId, s32 horizontalRate, s32 verticalRate);
void Func_02005ebc(s32 actorId, s32 horizontalRate, s32 verticalRate);
void Func_02005ee0(s32 actorId, s32 x, s32 y);
void Func_02005e9e(s32 step);
void Func_02005ef8(s32 actorId, s32 x, s32 y);
void Func_02005ff0(void);
void Func_02005ffc(void);
void Func_02005fc2(s32 step);
void Func_02005f6a(s32 cue);
void Func_02005f82(s32 actorId, s32 mode);
void Func_02005ee6(void);

static inline void InitializeActorZero(void)
{
    Func_02005eae(0, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Func_02005ebc(actorId, 0x10000, 0x8000);
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
#undef GetPartyInteractionRecord
#undef FinishPartyInteractionRecordRead
#undef GetPartyMemberCount
#undef SetInteractionCue
#undef CheckActorInteraction
#undef PlaceSelectedActor
#undef SetInteractionStep
#undef PlaceActorZero
#undef PlaceSupportActor
#undef StartInteractionPhase
#undef AdvanceInteractionPhase
#undef SelectInteractionStep
#undef SetLargePartyInteractionCue
#undef RunLargePartyInteraction
#undef FinishPartyCountInteraction
#undef RunPartyCountInteractionCopyA

/* overlays/scene/state/task/run_scene_middle_sequence.c */
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
void Func_02005f2c();
s32 Func_02006054();
s32 Func_02006060();
s32 Func_0200607e();
void Func_020060e0();
s32 Func_020060ec();
s32 Func_020060ec_a();
void Func_020060f4();
void Func_0200611e();
void Func_02006130();
void Func_02006164();
s32 Func_02006174();
void Func_020061b8();
void Func_020061c6();
s32 Func_020061ca();
void Func_020061d4();
void Func_020061fa();
void Func_02006212();
void Func_02006212_a();
void Func_02006218();
void Func_0200621a();
void Func_0200621c();
s32 Func_02006224();
void Func_0200622a();
void Func_02006240();
void Func_0200624a();
s32 Func_0200624c();
void Func_0200624e();
void Func_0200624e_a();
void Func_0200625c();
void Func_02006260();
void Func_02006266();
void Func_0200626e();
s32 Func_02006282();
void Func_02006286();
void Func_02006286_a();
s32 Func_0200628e();
void Func_02006290();
void Func_0200629a();
void Func_020062ea();
void Func_02006374();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

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

    rec = Value1(Func_0200607e, owner);
    p9 = *(s16 *)(rec + 10);
    p11 = *(s16 *)(rec + 18);
    if (mode != 3) {
        count = Value0(Func_02006054);
        for (i = 0; i < count; i++) {
            buf[i] = Data_02000240[504 + i];
        }
        if (count <= 1) {
            Func_020061fa(0x2083);
            Func_02006212(owner, 0);
            return;
        }
        if (Value1(Func_02006060, base + 512) != 0) {
            Func_020061fa(0x2084);
            Func_02006212(owner, 0);
            return;
        }
        if (mode == 2) {
            state = 0;
            Func_02005f2c(6);
        } else {
            Call1(Func_02006164, 0x207d);
            Value2(Func_02006174, owner, 0);
            state = Value2(Func_020060ec, 0, 0);
        }
        if (state == 0) {
            if (state < count) {
                for (i = 0; i < count; i++) {
                    Func_020060e0((s32)(s8)buf[i]);
                }
            }
            for (i = 0; i < count; i++) {
                if ((s32)(s8)buf[i] != 0) {
                    Func_020060f4((s32)(s8)buf[i]);
                }
            }
            obj = Value0(Func_0200628e);
            for (i = 0; i < count; i++) {
                Func_0200611e((s32)(s8)buf[i]);
            }
            for (i = 0; i < count; i++) {
                Func_02006130((s32)(s8)buf[i]);
            }
            if (obj != -1) {
                goto L_main;
            }
        }
    }
    Func_020061fa(0x207e);
    Func_02006212(owner, 0);
    return;
L_main:
    ((void (*)())Func_020060ec_a)(obj, 1);
    Call1(Func_02006212_a, 0x207f);
    Func_0200622a(owner, 0);
    Call3(Func_020061b8, 0, 0x10000, 0x8000);
    Call3(Func_020061c6, obj, 0x10000, 0x8000);
    Call3(Func_020061d4, owner, 0x10000, 0x8000);
    record = Value1(Func_020061ca, 0);
    if (record != 0) {
        Func_02006218(obj, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    hi = p11 + 16;
    Func_0200621a(obj, p9, hi);
    lo = p9 + 16;
    Value3(Func_02006224, 0, lo, hi);
    Func_0200626e(obj, 0, 30);
    Func_0200624e(obj, 3);
    tail = hi - 32;
    Func_02006260(0, 3);
    Func_0200624a(owner, p9, tail);
    Value3(Func_0200624c, owner, lo, tail);
    Func_02006374(0, obj);
    Func_02006266(obj, p9, tail);
    Func_02006286(owner, 1);
    Call3(Func_020062ea, owner, 0x8000, 0);
    Func_02006286_a(obj, p9, p11 - 48);
    Func_02006290(owner, p9, tail);
    Func_0200629a(owner, p9, p11);
    Func_02006240(obj);
    Func_0200621c(base + 512);
    rec = Value1(Func_02006282, obj);
    sx = *(volatile s32 *)(rec + 8) >> 20;
    Func_0200624e_a((obj << 4) + 880, sx);
    sy = *(volatile s32 *)(rec + 16) >> 20;
    Func_0200625c((obj << 4) + 888, sy);
}

/* overlays/scene/state/task/scene_data.c */
#define SceneData_GetTableC414 Func_02000030
#define SceneData_GetTableC474 Func_0200003c
#define SceneData_GetTableC48c Func_02000044
#define SceneData_GetTableC83c Func_02000b30
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
#undef SceneData_GetTableC414
#undef SceneData_GetTableC474
#undef SceneData_GetTableC48c
#undef SceneData_GetTableC83c

/* overlays/scene/state/task/scene_motion.c */
#define State_StartMarkerMove Func_02003024
extern u16 Data_0200cc00;
extern u16 Data_0200cca0;
extern u16 Data_0200cc94;
extern u16 Data_0200cc44;
extern u16 Data_0200cc20;
extern u16 Data_0200cc5c;
extern u16 Data_0200cc2c;
extern u16 Data_0200cbf0;

void Func_02006df8(s32 task_address, s32 frame_budget);

/* Start an interpolated marker move from the current position. */

/* Per-site veneer (raw sub_ symbol from the overlay .s). */

void State_StartMarkerMove(u32 x, u32 y, u32 duration)
{
    Data_0200cc00 = (u16)x;
    Data_0200cca0 = (u16)y;
    Data_0200cc44 = Data_0200cc94;
    Data_0200cc5c = Data_0200cc20;
    Data_0200cc2c = (u16)duration;
    Data_0200cbf0 = 0;

    {
        s32 frame_budget = 0xc80;
        Func_02006df8(0x0200ae85, frame_budget);
    }
}
#undef State_StartMarkerMove

/* overlays/scene/state/task/scene_primary_script_head.c */
#define Scene_RunSupplementalSequenceOne Func_02000714
#define Scene_RunScene3bbSequenceA Func_020008ec
#define FieldScene_RunScene3bb_02000b38 Func_02000b38
#define Scene_RunSecondActorInteraction Func_02001424
extern u8 Data_02000240[];
extern u8 Data_0200c834[];
extern u8 Data_0200c838[];
extern u8 Data_03001ebc[];

void Func_02004650();
s32 Func_0200465c();
void Func_02004666();
s32 Func_02004674();
void Func_02004678();
s32 Func_0200467e();
void Func_02004692();
void Func_020046a6();
void Func_020046ba();
void Func_020046ce();
void Func_020046ec();
void Func_02004700();
void Func_02004714();
void Func_02004728();
s32 Func_0200475c();
s32 Func_02004770();
s32 Func_02004784();
s32 Func_02004798();
s32 Func_020047b2();
s32 Func_020047b6();
s32 Func_020047ca();
s32 Func_020047de();
s32 Func_020047f2();
void Func_020047f6();
void Func_02004850();
void Func_020046ac();
void Func_0200489a();
void Func_020048a8();
void Func_020048b6();
void Func_020048c4();
void Func_020048d4();
void Func_020036a2();
void Func_02004950();
void Func_02004a74();
void Func_02004a88();
void Func_02004a98();
void Func_02004a9e();
void Func_02004aee();
void Func_02004afc();
void Func_02004b0a();
void Func_02004b28();
void Func_02004b30();
void Func_02004b30_a();
void Func_02004b3a();
void Func_02004b3c();
void Func_02004b42();
void Func_02004be0();
void Func_02001d44();
void Func_02001dd8();
void Func_02001e3c();
void Func_0200322e();
s32 Func_02003444();
void Func_020035b8();
s32 Func_02003632();
void Func_02004046();
void Func_02004274();
s32 Func_020045a4();
s32 Func_020045b0();
void Func_02005378();
void Func_020053d4();
void Func_02005412();
void Func_02005424();
s32 Func_02005462();
s32 Func_02005462_a();
void Func_02005470();
void Func_0200547c();
void Func_02005496();
void Func_020054a2();
void Func_020054ca();
void Func_020054dc();
void Func_020054f4();
void Func_02005502();
void Func_02005508();
void Func_02005520();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern s32 Data_02000240_t[][1];

    return f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern s32 Data_02000240_t[][1];

    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern s32 Data_02000240_t[][1];

    f(a0, a1, a2, a3, a4, a5);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000b38(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern s16 Data_02000240_t[][1];

    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern s16 Data_02000240_t[][1];

    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern s16 Data_02000240_t[][1];

    return f(a0, a1, a2);
}

void Scene_RunSupplementalSequenceOne(void)
{
    extern s32 Data_02000240_t[][1];

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

    rec8 = Func_02004674(Data_02000240_t[125][0]);
    for (i = 22; i <= 25; i++) {
        rec7 = Value1(Func_0200467e, i);
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
        if (Value1(Func_0200465c, 0x104) != 0) {
            yb = *(s32 *)(rec7 + 16);
        } else {
            yb = *(s32 *)(rec8 + 16);
            yd = *(s32 *)(rec7 + 44);
            yb = yb + yd;
        }
        *(s32 *)(rec8 + 16) = yb;
    }
    if (*(volatile s32 *)Data_0200c838 != 0
        && *(s32 *)(rec7 + 56) == (s32)0x80000000) {
        if (*(volatile s32 *)Data_0200c834 == 0) {
            Call6(Func_02004650, 58, 28, 7, 1, 58, 13);
        } else {
            Call6(Func_02004650, 58, 10, 1, 1, 58, 11);
        }
    } else {
        Call6(Func_02004666, 57, 11, 1, 1, 58, 11);
        Call6(Func_02004678, 58, 14, 7, 1, 58, 13);
    }
    base5 = (s32)Data_0200c838;
    if (*(volatile s32 *)base5 == 0) {
        flag = *(volatile s32 *)Data_0200c834 ^ 1;
        *(volatile s32 *)Data_0200c834 = flag;
        if (flag != 0) {
            record = Func_0200475c(22);
            Call4(Func_02004692, record, 0x3a80000, 0, 0xb80000);
            record = Func_02004770(23);
            Call4(Func_020046a6, record, 0x3c80000, 0, 0xf80000);
            record = Func_02004784(24);
            Call4(Func_020046ba, record, 0x3e80000, 0, 0xb80000);
            record = Func_02004798(25);
            Call4(Func_020046ce, record, 0x4080000, 0, 0xf80000);
            Func_020047f6(31, 11);
        } else {
            record = Func_020047b6(22);
            Call4(Func_020046ec, record, 0x3a80000, 0, 0xd80000);
            record = Func_020047ca(23);
            Call4(Func_02004700, record, 0x3c80000, 0, 0xd80000);
            record = Func_020047de(24);
            Call4(Func_02004714, record, 0x3e80000, 0, 0xd80000);
            record = Func_020047f2(25);
            Call4(Func_02004728, record, 0x4080000, 0, 0xd80000);
            Func_02004850(31, 10);
        }
    }
    count = *(volatile s32 *)base5 + 1;
    *(volatile s32 *)base5 = count;
    if ((u32)count > 119) {
        record = Value1(Func_020047b2, 0x104);
        if (record == 0) {
            *(volatile s32 *)base5 = record;
        }
    }
}

void Scene_RunScene3bbSequenceA(void)
{
    u32 i;
    s32 record;

    *(volatile s32 *)Data_0200c838 = 0;
    *(volatile s32 *)Data_0200c834 = 0;
    Call1(Func_020046ac, 0x2008715);
    Call3(Func_0200489a, 22, 0x3a80000, 0xd80000);
    Call3(Func_020048a8, 23, 0x3c80000, 0xd80000);
    Call3(Func_020048b6, 24, 0x3e80000, 0xd80000);
    Call3(Func_020048c4, 25, 0x4080000, 0xd80000);
    Func_020048d4(31, 10);
}

void FieldScene_RunScene3bb_02000b38(s32 a0)
{
    u32 i;
    s32 record;

    Func_02004a98(40);
    Func_02004a9e(41);
    Func_02004a74(1);
    Func_02004a88();
    Call3_02000b38(Func_02004aee, 8, 0x580000, 0x1000000);
    Call3_02000b38(Func_02004afc, 0, 0x780000, 0x1000000);
    Call3_02000b38(Func_02004b30, 8, 0x4000, 0);
    Call3_02000b38(Func_02004b3c, 0, 0x4000, 0);
    if (a0 < 0) {
        Func_02004b28(8, 10);
        Func_02004b30_a(0, 35);
    } else {
        Func_02004b3a(8, 8);
        Func_02004b42(0, 28);
    }
    Func_02004950(1);
    Call4(Func_02004be0, 0x680000, 0, 0xc00000, 0);
    Func_020036a2(a0);
    Func_02004b0a();
}

void Scene_RunSecondActorInteraction(s32 a0)
{
    extern s16 Data_02000240_t[][1];

    u32 i;
    s32 rec;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        Func_0200322e();
    } else {
        Func_02005378();
        rec = Value2(Func_02003444, a0, 2);
        if (rec == 0) {
            Call1(Func_02005424, 0x20a2);
            Func_02001d44();
            Call2(Func_0200547c, 0x30000, 0x6000);
            Call4(Func_02005496, 0x3d80000, -1, 0xe80000, 1);
            Func_020054a2();
            Value2(Func_02005462, a0, 0);
            Func_02001dd8();
            Func_02005470(a0, 0);
            Func_02004046(0, 0x438, 0x108);
            Func_020053d4(15);
            Call3(Func_02005412, 0, 0x18000, 0xc000);
            Value3(Func_020045a4, 0, 0x438, 216);
            Value3(Func_020045b0, 0, 0x428, 216);
            Func_02001e3c();
            Func_02005520();
            Call4(Func_02005502, -1, -1, -1, 0);
            Func_020054ca(a0, 0);
            Func_02004274(0);
            Func_02005508(0, 0);
            Func_020035b8(a0, 2);
        } else {
            if (rec == 1) {
                Call1(Func_020054dc, 0x20a1);
                Func_020054f4(a0, 0);
            }
        }
        Value3(Func_02003632, rec, a0, 2);
        ((void (*)())Func_02005462_a)();
    }
}
#undef Scene_RunSupplementalSequenceOne
#undef Scene_RunScene3bbSequenceA
#undef FieldScene_RunScene3bb_02000b38
#undef Scene_RunSecondActorInteraction

/* overlays/scene/state/task/scene_setup.c */
#define State_ConfigureRegionByActorElevenColumn Func_0200005c
#define Scene_RunTwoStepSequence Func_020000b0
#define State_ApplyRectsForActors15To17 Func_0200062c
#define Scene_RunStep15At29By26 Func_0200069c
#define Scene_RunStep15At33By26 Func_020006b0
#define Scene_RunStep16At45By26 Func_020006c4
#define Scene_RunStep16At49By26 Func_020006d8
#define Scene_RunStep17At40By23 Func_020006ec
#define Scene_RunStep17At40By25 Func_02000700
#define Scene_RunSixSteps896To936 Func_02001fb8
#define Scene_BuildDescriptorAndInstallTask Func_020039fc
#define State_InitControlRecordAndStartTask Func_02003ae4
typedef struct Ctl {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 f6;
    s16 f8;
} Ctl;

extern u8 Data_0200c194[];
extern Ctl Data_02001000;
extern u8 *Data_03001f3c;
extern u8 Data_0200b459[];

u8 Func_02003f0e(s32, s32, s32, s32, s32, s32);
u8 Func_02003f2a(s32, s32, s32, s32, s32, s32);
u8 Func_02003f62(s32);
u8 Func_02003f86(s32);
void *Func_02003fb4(s32);
void Func_02003c44(void);
void Func_02000114(void);
void Func_020044ca();
s32 *Func_02004598();
void Func_020044e4();
s32 *Func_020045b2();
void Func_020044fe();
s32 *Func_020045cc();
void Func_0200451a();
void Func_02000b66(s32, s32, s32);
void Func_02000cd6(void);
void Func_02000b7a(s32, s32, s32);
void Func_02000cea(void);
void Func_02000b8e(s32, s32, s32);
void Func_02000cfe(void);
void Func_02000ba2(s32, s32, s32);
void Func_02000d12(void);
void Func_02000bb6(s32, s32, s32);
void Func_02000d26(void);
void Func_02000bca(s32, s32, s32);
void Func_02000d3a(void);
void Func_02005eca(s32 arg0, s32 arg1);
void Func_02005ed4(s32 arg0, s32 arg1);
void Func_02005ede(s32 arg0, s32 arg1);
void Func_02005ee8(s32 arg0, s32 arg1);
void Func_02005ef2(s32 arg0, s32 arg1);
void Func_02005efc(s32 arg0, s32 arg1);
u8 *Func_020077f6();
s32 Func_02007810();
u8 *Func_020079a8();
u8 *Func_020079b0();
s32 Func_02007950();
void Func_02007886();
s32 Func_020078aa();
s32 Func_020078b6();
void Func_02007856();
void Func_020078a4();
void Func_0200b638();
s32 Func_0200791e(void);
void Func_020078ee(s32, s32);
s32 Func_020079e4(s32);
void Func_020078c2(s32, s32);

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

    work = Func_02003fb4(11);
    if ((*(s32 *)(work + 8) >> 20) == 36) {
        Func_02003f62(0x335);
        v0 = 0x23;
        v1 = 0x4D;
        Func_02003f0e(0x23, 0x4E, 1, 1, v0, v1);
    } else {
        Func_02003f86(0x335);
        v0 = 0x23;
        v1 = 0x4D;
        Func_02003f2a(0x22, 0x4D, 1, 1, v0, v1);
    }
}

void Scene_RunTwoStepSequence(void)
{
    Func_02003c44();
    Func_02000114();
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

    Func_020044ca(100, 11, 12, 4, 14, 11);

    field = Func_02004598(15)[2] >> 20;
    Func_020044e4(13, 28, 1, 4, field, 11);

    field = Func_020045b2(16)[2] >> 20;
    Func_020044fe(13, 28, 1, 4, field, 11);

    field = Func_020045cc(17)[4] >> 20;
    Func_0200451a(13, 28, 4, 1, 18, field);
}

void Scene_RunStep15At29By26(void)
{
    Func_02000b66(15, 29, 26);
    Func_02000cd6();
}

void Scene_RunStep15At33By26(void)
{
    Func_02000b7a(15, 33, 26);
    Func_02000cea();
}

void Scene_RunStep16At45By26(void)
{
    Func_02000b8e(16, 45, 26);
    Func_02000cfe();
}

void Scene_RunStep16At49By26(void)
{
    Func_02000ba2(16, 49, 26);
    Func_02000d12();
}

void Scene_RunStep17At40By23(void)
{
    Func_02000bb6(17, 40, 23);
    Func_02000d26();
}

void Scene_RunStep17At40By25(void)
{
    Func_02000bca(17, 40, 25);
    Func_02000d3a();
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
    Func_02005eca(896, 0);
    Func_02005ed4(904, 0);
    Func_02005ede(912, 0);
    Func_02005ee8(920, 0);
    Func_02005ef2(928, 0);
    Func_02005efc(936, 0);
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

    descriptor = Func_020077f6(59, 0x7170);
    handle = Func_02007810(512);                /* 128 << 2 */

    *(u16 *)(descriptor + 222) = (u16)first;
    *(u16 *)(descriptor + 224) = (u16)second;
    *(u16 *)(descriptor + 226) = (u16)third;
    *(u16 *)(descriptor + 228) = (u16)fourth;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = Func_020079a8(first);
    second_record = Func_020079b0(second);

    if (Func_02007950(0x109) == 0) {
        *(s32 *)(second_record + 8) =
            (centre << 1) - *(s32 *)(first_record + 8);
        *(s32 *)(second_record + 16) = *(s32 *)(first_record + 16);
    }

    *(u16 *)(descriptor + 218) = 0;
    *(u16 *)(descriptor + 220) = 0;

    Func_02007886(Data_0200c194, handle);

    extent = Func_020078aa();
    *(u16 *)(descriptor + 216) = (u16)extent;
    Func_020078b6((s16)extent, 512, handle);

    /* The task is published as its entry address plus the Thumb bit. */
    Func_02007856((s32)Func_0200b638 + 1, 0xc76);

    Func_020078a4(handle);
}

void State_InitControlRecordAndStartTask(void)
{
    u8 *state = Data_03001f3c;
    Ctl *m = &Data_02001000;

    Func_020078ee(Func_0200791e(), (s32)(state + 240));
    if (Func_020079e4(0x109) == 0) {
        m->f0 = 1;
        m->f2 = 1;
        m->f4 = *(u16 *)(state + 224);
        m->f8 = 0;
        m->f6 = 0;
    }
    {
        s32 e = 0xc85;

        Func_020078c2((s32)Data_0200b459, e);
    }
}
#undef State_ConfigureRegionByActorElevenColumn
#undef Scene_RunTwoStepSequence
#undef State_ApplyRectsForActors15To17
#undef Scene_RunStep15At29By26
#undef Scene_RunStep15At33By26
#undef Scene_RunStep16At45By26
#undef Scene_RunStep16At49By26
#undef Scene_RunStep17At40By23
#undef Scene_RunStep17At40By25
#undef Scene_RunSixSteps896To936
#undef Scene_BuildDescriptorAndInstallTask
#undef State_InitControlRecordAndStartTask

/* overlays/scene/state/task/scene_state_interaction_head.c */
#define State_SetFlag331AndConfigureRegion46_17 Func_02000258
#define Scene_SetFlag332AndDrawTiles Func_0200028c
#define Scene_SetFlag333AndDrawTiles Func_020002c0
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

extern u8 *Data_03001ebc;
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;
extern u8 Value_0000207c;

void Func_02004150(s32);
u8 *Func_020041b6(s32);
void Func_02004108(s32, s32, s32, s32, s32, s32);
void Func_02004184(s32);
u8 *Func_020041ea(s32);
void Func_0200413c(s32, s32, s32, s32, s32, s32);
void Func_020041b8(s32);
void Func_02004164(s32, s32, s32, s32, s32, s32);
s32 Func_02005a38(s32);
s32 Func_02005a42(s32);
void Func_02005ae8(s32, s32, s32);
s32 Func_02005a60(s32);
s32 Func_02005a6a(s32);
void Func_02005b0c(s32, s32, s32);
s32 Func_02005a84(s32);
s32 Func_02005a8e(s32);
void Func_02005b30(s32, s32, s32);
s32 Func_02005b86(s32);
Obj *Func_02005c38(s32);
void Func_02005c26(void);
Obj *Func_02005c46(s32);
void Func_02005d28(s32);
void Func_02005d40(s32, s32);
void Func_02005cb0();
void Func_02005cb0_a();
void Func_02005dec(void);
void Func_02005df8(void);
void Func_02005ca0(s32, s32);
void Func_02005de2(s32);
void Func_02005caa(s32);
void Func_0200396a(s32);
void Func_02005e26(void);
void Func_02005e3a_a(void);
void Func_02005d14(void);
void Func_02006084(void);
void Func_02005ebc_a(s32, s32);
void Func_02006002(s32);
void Func_0200601a(s32, s32);
s32 Func_02005f2c(s32);
s32 Func_02005f3c(s32);
s32 Func_02005f1e(s32);
void Func_02005f6c(s32);
void Func_02006052(s32);
s32 Func_02006062(s32, s32);
s32 Func_02005fda(s32, s32);

void State_SetFlag331AndConfigureRegion46_17(void)
{
    u8 *p;

    Func_02004150(0x331);
    p = Func_020041b6(20) + 85;
    *p = 0;
    {
        s32 p5 = 44;
        s32 p6 = 17;

        Func_02004108(46, 17, 1, 1, p5, p6);
    }
}

void Scene_SetFlag332AndDrawTiles(void)
{
    u8 *slot;

    Func_02004184(0x332);
    slot = Func_020041ea(21) + 85;
    *slot = 0;
    {
        s32 v5 = 50;
        s32 v6 = 17;

        Func_0200413c(46, 17, 1, 1, v5, v6);
    }
}

void Scene_SetFlag333AndDrawTiles(void)
{
    Func_020041b8(0x333);
    {
        s32 width = 32;
        s32 height = 77;

        Func_02004164(32, 37, 1, 4, width, height);
    }
}

void Func_02001b30(void)
{
    {
        s32 x = Func_02005a38(896);
        s32 y = Func_02005a42(904);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Func_02005ae8(1, x, y);
    }
    {
        s32 x = Func_02005a60(912);
        s32 y = Func_02005a6a(920);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Func_02005b0c(2, x, y);
    }
    {
        s32 x = Func_02005a84(928);
        s32 y = Func_02005a8e(936);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Func_02005b30(3, x, y);
    }
}

void Func_02001c78(void)
{
    extern s32 Data_02000240[];

    u8 *state = Data_03001ebc;
    s32 v = Data_02000240[125];

    if (v != 0 && ((s32)(s16)*(u16 *)(state + 382) >> 10) == v
        && Func_02005b86(321) != 0) {
        u16 *p = (u16 *)(state + 386);
        s32 t = 99;

        *p = t;
    }
}

void Func_02001cc0(void)
{
    extern s32 Data_02000240[];

    u8 *state = Data_03001ebc;
    s32 best = 8;
    s32 bestd = 0x100000;
    s32 n = Data_02000240[125];
    Obj *p = Func_02005c38(n);
    s32 i;
    s32 *q;
    s32 base;

    Func_02005c26();
    for (i = 8; i <= 66; i++) {
        Obj *o = Func_02005c46(i);

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
    Func_02005d28(0x2085);
    Func_02005d40(best, 0);
    q = (s32 *)(state + 448);
    *q = 512;
    *(s32 *)(state + 456) = 15;
    Func_02005cb0(20);
    Func_02005dec();
    Func_02005df8();
    base = n << 4;
    Func_02005ca0(base + 880, p->f08 >> 20);
    {
        s32 v = p->f10 >> 20;

        Func_02005cb0_a(base + 888, v);
    }
    n++;
    if (n > 3) {
        Func_02005de2(10);
        Func_02005caa(282);
    } else {
        Func_0200396a(n);
        Func_02005e26();
        Func_02005e3a_a();
        *q = 0;
    }
    Func_02005d14();
}

s32 Func_02001ffc(s32 a, s32 b)
{
    extern s16 Data_02000240[];

    s32 v;
    s32 id;
    s32 r;

    Func_02006084();
    Func_02005ebc_a(b, 5);
    v = Data_02000240[224];
    if (v == (s32)&Value_0000008f) {
        id = (s32)&Value_00002076;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)&Value_00002078;
    } else {
        id = (s32)&Value_0000207a;
    }
    Func_02006002(id);
    Func_0200601a(a, 0);
    if (Func_02005f2c(b + 512) != 0) {
        return 2;
    }
    if (Func_02005f3c(b + 520) != 0) {
        r = Func_02005f1e(0);
        if (r == 1) {
            return 2;
        }
        if (r == 2 || r == -1) {
            return 3;
        }
        return r;
    }
    Func_02005f6c(b + 520);
    Func_02006052((s32)&Value_0000207c);
    Func_02006062(a, 0);
    return Func_02005fda(0, 0);
}
#undef State_SetFlag331AndConfigureRegion46_17
#undef Scene_SetFlag332AndDrawTiles
#undef Scene_SetFlag333AndDrawTiles

/* overlays/scene/state/task/shared.c */
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define HexDigits Data_0200c250
#define Scene_RunSingleStep Func_02000050
#define Scene_Forward4358 Func_02000304
#define Text_WriteU32AsHex Func_020031fc

extern u8 Data_0200c250[];

void Func_02003be4();
void Func_02004358(void);

void Scene_RunSingleStep(void)
{
    Func_02003be4();
}

void Scene_Forward4358(void)
{
    Func_02004358();
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
#undef NULL
#undef FIELD_AT_OFFSET
#undef HexDigits
#undef Scene_RunSingleStep
#undef Scene_Forward4358
#undef Text_WriteU32AsHex

/* overlays/scene/state/task/staged_actor.c */
#define StagedActor_ResetMotionAfterRefresh Func_02002e44
u8 *Func_02006d98();
void Func_02006cb6();

/* Reset the selected actor's motion fields after refreshing it. */

void StagedActor_ResetMotionAfterRefresh(s32 slot)
{
    u8 *actor = Func_02006d98(slot);
    Func_02006cb6(actor);
    *(s32 *)(actor + 36) = 0;
    *(s32 *)(actor + 44) = 0;
    *(s32 *)(actor + 56) = (s32)0x80000000;
    *(s32 *)(actor + 64) = (s32)0x80000000;
}
#undef StagedActor_ResetMotionAfterRefresh

/* overlays/scene/state/task/staged_actor_movement.c */
#define StagedActor_PushActorAhead Func_02003b90
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

extern u32 Data_0200c3d4[];
extern s16 Data_02000240[];

SceneRecord *Func_02007726(Position3 *, SceneRecord *);
SceneRecord *Func_02007750(Position3 *, SceneRecord *);
SceneRecord *Func_0200777c(Position3 *, SceneRecord *);

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */

/* This overlay's own occupancy lookup for a cell. The record pointer the call
 * sites also load is spelled here, although the lookup itself uses only the
 * position. */

/* In-image direction table: sixteen packed steps, high half x, low half z. */

SceneRecord *Func_02007afc();   /* scene record for a subject handle */

s32 Func_02007b0e_a(SceneRecord *, Position3 *);  /* terrain probe */

void Func_02007acc(SceneRecord *, s32);         /* select presentation mode */

void Func_02007a2a(s32);                        /* wait n frames */

void Func_02007b12(SceneRecord *, s32, s32, s32);   /* place at (x, y, z) */

void Func_02007b22(SceneRecord *, s32, s32, s32);   /* place at (x, y, z) */

void Func_02007d60(s32);                        /* play a cue */

void Func_02007b36(SceneRecord *);              /* re-attach the camera */

void Func_02007d6e(s32);                        /* play a cue */

void Func_02007b1c(SceneRecord *, s32);         /* select presentation mode */

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

    handle = *(s32 *)((u8 *)Data_02000240 + (idx << 1));
    subject = Func_02007afc(handle);

    dir = subject->facing >> 12;

    step = Data_0200c3d4[dir];
    pos.x = subject->x + (s32)(step & 0xffff0000);
    pos.y = subject->y;
    step <<= 16;
    pos.z = subject->z + (s32)step;

    target = Func_02007726(&pos, subject);
    if (target == 0) {
        return;
    }

    step = Data_0200c3d4[dir];
    pos.x = target->x + (s32)(step & 0xffff0000);
    pos.y = target->y;
    step <<= 16;
    pos.z = target->z + (s32)step;

    blocker = Func_02007750(&pos, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    pos.x = target->x;
    pos.y = target->y + 0x100000;
    pos.z = target->z;

    blocker = Func_0200777c(&pos, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    target->state = 2;
    zero = 0;

    step = Data_0200c3d4[dir];
    pos.x = target->x + (s32)(step & 0xffff0000);
    pos.y = target->y;
    step <<= 16;
    pos.z = target->z + (s32)step;

    /* Terrain probe: signed, so only a positive code refuses the move. */
    if (Func_02007b0e_a(target, &pos) > 0) {
        return;
    }

    Func_02007acc(subject, 8);
    Func_02007a2a(15);

    target->rate_x = 0x3333;
    target->rate_z = 0x3333;
    Func_02007b12(target, pos.x, pos.y, pos.z);

    /* The same destination block, moved onto the subject this time. */
    subject->rate_x = 0x3333;
    subject->rate_z = 0x3333;
    Func_02007b22(subject, pos.x, pos.y, pos.z);

    Func_02007d60(0xee);
    Func_02007b36(target);
    Func_02007d6e(0x120);

    target->x = pos.x;
    target->z = pos.z;
    target->motion_24 = zero;
    target->motion_2c = zero;

    Func_02007b1c(subject, 1);
}
#undef StagedActor_PushActorAhead

/* overlays/scene/state/task/state_update.c */
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define State_StoreSlotTileXToWork832To848 Func_020000c0
#define State_SetWorkByte35 Func_02000150
#define State_SendWord250With6 Func_020002e8
#define State_ApplyTable8715AndValue104 Func_02000950
#define State_WaitForStatusWords Func_02000970
#define State_InstallTask8714AndApplyTwoRects Func_020009b0
#define State_InitCursorWhenUnset Func_02002e64
#define State_SetHalfword1000To9 Func_0200322c
#define State_WaitUntilStatusNine Func_02003238

extern s16 Data_02000240[];
extern u8 Value_0000000a;
extern volatile s32 Data_0200c834;
extern volatile s32 Data_0200c838;
extern s16 Data_0200cb46;

void Func_02003f5e(s32, s32, s32, s32, s32, s32);
s32 *Func_0200402c(s32);
void Func_02003ff2(s32, s32);
void Func_02003f84(s32, s32, s32, s32, s32, s32);
s32 *Func_02004052(s32);
void Func_02004018(s32, s32);
void Func_02003fa8(s32, s32, s32, s32, s32, s32);
s32 *Func_02004076(s32);
void Func_0200403c(s32, s32);
void Func_02003fcc(s32, s32, s32, s32, s32, s32);
void Func_020042aa(s32, s32, s32);
s32 Func_02004700_a(s32, s32);
void Func_02004858(s32);
void Func_02004716();
void Func_02004724();
void Func_02004a58(s32);
void Func_020048b0(s32);
void Func_02004774(s32);
void Func_0200477a(s32);
void Func_02004790(s32);
void Func_0200487c(s32, s32, s32, s32, s32, s32);
void Func_0200488e(s32, s32, s32, s32, s32, s32);
void Func_02008714();
s32 Func_02006d3e(void);
void Func_02006fe8(s32 arg0);

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

    Func_02003f5e(100, 11, 12, 4, fifth, sixth);

    record = Func_0200402c(12);
    value = record[2] >> 20;
    Func_02003ff2(832, value);
    Func_02003f84(71, 16, 1, 1, value, 16);

    record = Func_02004052(13);
    value = record[2] >> 20;
    Func_02004018(840, value);
    Func_02003fa8(71, 16, 1, 1, value, 16);

    record = Func_02004076(14);
    value = record[2] >> 20;
    Func_0200403c(848, value);
    Func_02003fcc(71, 16, 1, 1, value, 16);
}

void State_SetWorkByte35(void)
{
    FIELD_AT_OFFSET(*(void **)0x03001F30, s8 *, 0x35) = 1;
}

void State_SendWord250With6(void)
{
    s16 *tbl = Data_02000240;

    Func_020042aa(*(s32 *)(tbl + 250), 6, 0);
}

void State_ApplyTable8715AndValue104(void)
{
    Func_02004700_a(0x2008715, 0xC85);
    Func_02004858(0x104);
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
    Func_02004716(10);

    cnt = 0;
    while (Data_0200c834 != 0 || Data_0200c838 != 75) {
        Func_02004724(1);
        cnt++;
        if (cnt >= 600) {
            return;
        }
    }
}

void State_InstallTask8714AndApplyTwoRects(void)
{
    Func_02004a58(31);
    Func_020048b0(820);                 /* 205 << 2 */

    if (Data_0200c834 != 0) {
        Data_0200c838 = 0;
    }

    Func_02004774(30);
    Func_0200477a(1);

    /* The task is published as its entry address with the Thumb bit set. The
     * `.thumb_set` alias the exact reconstruction emits for a Thumb symbol already carries
     * bit 0, so adding it again here overshoots by one. */
    Func_02004790((s32)Func_02008714);

    Func_0200487c(58, 28, 7, 1, 58, 13);
    Func_0200488e(57, 11, 1, 1, 58, 11);
}

void State_InitCursorWhenUnset(void)
{
    s16 *cursor = &Data_0200cb46;

    if (*cursor == -1) {
        *cursor = Func_02006d3e();
    }
}

/* Complete eight-byte state setter plus its sole four-byte pool word. */
void State_SetHalfword1000To9(void)
{
    extern u16 Data_02001000;

    u16 *p = &Data_02001000;
    u16 v = 9;
    *p = v;
}

void State_WaitUntilStatusNine(void)
{
    extern s16 Data_02001000;

    s16 *status = &Data_02001000;

    while (*status != 9) {
        Func_02006fe8(1);
    }
}
#undef NULL
#undef FIELD_AT_OFFSET
#undef State_StoreSlotTileXToWork832To848
#undef State_SetWorkByte35
#undef State_SendWord250With6
#undef State_ApplyTable8715AndValue104
#undef State_WaitForStatusWords
#undef State_InstallTask8714AndApplyTwoRects
#undef State_InitCursorWhenUnset
#undef State_SetHalfword1000To9
#undef State_WaitUntilStatusNine
