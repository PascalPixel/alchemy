#include "types.h"
#include "scene.h"

/* overlays/scene/state/task/task.c */
/* overlays/scene/state/task/task.c */
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

Obj *State_unk15_4(s32);
Obj *State_unk16_4(s32);

void Actor_MovePairByTileOffset(s32 a0, s32 a1, s32 a2)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = State_unk15_4(gCell[125]);
    q = State_unk16_4(a0);
    State_unk17_4();
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
    State_unk6_2(226);
    State_unk7_2(p);
    State_unk8_2(288);
    State_Apply4(q, 2);
    State_unk18_4();
}

/* overlays/scene/state/task/actor_position.c */
s32 *State_unk19_4(s32 selector);

u8 *State_unk20_4(s32 no);

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
    s32 *actor = State_unk19_4(selector);

    if (actor != 0) {
        s32 scale = 0x20000;
        u8 *p = (u8 *)actor;
        u8 zero = 0;
        *(s32 *)((u8 *)actor + 48) = scale;
        *(s32 *)((u8 *)actor + 52) = scale >> 1;
        p += 91;
        *p = zero;

        State_unk9_2(actor);
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
    u8 *obj = State_unk20_4(no);

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

    State_unk21_4();
    State_Apply6(obj, 5);
    State_SetMode4(obj, x << 16, *(s32 *)(obj + 12), z << 16);
    State_unk10_2(obj);
    State_Apply7(obj, 1);
}

/* overlays/scene/state/task/actor_search.c */
/* The active subject's handle sits 500 bytes into the shared table. */
typedef struct ActiveSubjectSlot {
    u8 pad[500];
    void *handle;
} ActiveSubjectSlot;

extern u8 gWork[];

u8 *State_unk22_4();

s32 *State_unk23_4();

s32 *State_unk24_4();

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

    record = State_unk22_4(((ActiveSubjectSlot *)gCell)->handle);

    /* 128 << 6 = 0x2000 bias, then masked to bits 14-15 (192 << 8). */
    facing = (*(u16 *)(record + 6) + 0x2000) & 0xc000;

    position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
    position[1] = *(s32 *)(record + 12);
    position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
    State_unk25_4(0x100000, facing, position);          /* 128 << 13 */

    occupant = State_unk23_4(position, record);
    if (occupant == 0) {
        position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
        position[1] = *(s32 *)(record + 12);
        position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
        State_unk26_4(0x200000, facing, position);      /* 128 << 14 */

        occupant = State_unk24_4(position, record);
    }

    return occupant;
}

/* overlays/scene/state/task/effect.c */
extern s16 gOv;

/* Remove the marker task and release its selected display slot.
 * Per-site veneers (raw sub_ symbols from the overlay .s). */

void Effect_RemoveMarkerTask(void)
{
    State_unk11_2((void *)0x0200ae85);
    State_unk12_2(gOv);
    gOv = -1;
}

/* overlays/scene/state/task/overlay_object.c */
u16 *State_unk27_4(s32 no);

/*
 * resource_3bb: look up an object by arg0, then scan the first 15 halfword
 * entries of its table at offset 0xd8 for one equal to arg1, calling a handler
 * with each matching index.  The owner at 0x02002380 is 60 bytes and carries
 * no literal pool.  The three callees are identified by call shape only, and
 * the table's role is inferred from this scan alone.
 */

void OvObj_NotifyMatchingEntries(s32 no, s32 val)
{
    u16 *tbl = State_unk27_4(no);
    s32 i;

    State_unk28_4(no, val);

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
    State_unk19_3(0, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    State_unk20_3(actorId, 0x10000, 0x8000);
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
            State_unk29_4(0x2083);
            State_unk30_4(owner, 0);
            return;
        }
        if (State_unk2(base + 512) != 0) {
            State_unk29_4(0x2084);
            State_unk30_4(owner, 0);
            return;
        }
        if (mode == 2) {
            state = 0;
            State_unk12(6);
        } else {
            State_Do(0x207d);
            State_unk3(owner, 0);
            state = State_unk4(0, 0);
        }
        if (state == 0) {
            if (state < count) {
                for (i = 0; i < count; i++) {
                    State_unk31_4((s32)(s8)buf[i]);
                }
            }
            for (i = 0; i < count; i++) {
                if ((s32)(s8)buf[i] != 0) {
                    State_unk32_4((s32)(s8)buf[i]);
                }
            }
            obj = State_unk2_4();
            for (i = 0; i < count; i++) {
                State_unk33_4((s32)(s8)buf[i]);
            }
            for (i = 0; i < count; i++) {
                State_unk34_4((s32)(s8)buf[i]);
            }
            if (obj != -1) {
                goto L_main;
            }
        }
    }
    State_unk29_4(0x207e);
    State_unk30_4(owner, 0);
    return;
L_main:
    ((void (*)())State_unk13)(obj, 1);
    State_unk2_2(0x207f);
    State_unk35_4(owner, 0);
    State_Place(0, 0x10000, 0x8000);
    State_unk2_3(obj, 0x10000, 0x8000);
    State_unk3_3(owner, 0x10000, 0x8000);
    record = State_unk5(0);
    if (record != 0) {
        State_unk36_4(obj, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    hi = p11 + 16;
    State_unk37_4(obj, p9, hi);
    lo = p9 + 16;
    State_unk4_3(0, lo, hi);
    State_unk28_4(obj, 0, 30);
    State_unk38_4(obj, 3);
    tail = hi - 32;
    State_unk39_4(0, 3);
    State_unk40_4(owner, p9, tail);
    State_unk5_3(owner, lo, tail);
    State_unk41_3(0, obj);
    State_unk42_3(obj, p9, tail);
    State_unk43_3(owner, 1);
    State_unk6_3(owner, 0x8000, 0);
    State_unk44_3(obj, p9, p11 - 48);
    State_unk45_3(owner, p9, tail);
    State_unk46_3(owner, p9, p11);
    State_unk47_3(obj);
    State_unk48_3(base + 512);
    rec = State_unk6(obj);
    sx = *(volatile s32 *)(rec + 8) >> 20;
    State_unk49_3((obj << 4) + 880, sx);
    sy = *(volatile s32 *)(rec + 16) >> 20;
    State_unk27_4((obj << 4) + 888, sy);
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
extern u8 gOv10[];
extern u8 gOv11[];

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

    rec8 = State_unk14(gCell2[125][0]);
    for (i = 22; i <= 25; i++) {
        rec7 = State_unk7(i);
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
        if (State_unk8(0x104) != 0) {
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
        State_unk2_5(57, 11, 1, 1, 58, 11);
        State_unk3_5(58, 14, 7, 1, 58, 13);
    }
    base5 = (s32)gOv11;
    if (*(volatile s32 *)base5 == 0) {
        flag = *(volatile s32 *)gOv10 ^ 1;
        *(volatile s32 *)gOv10 = flag;
        if (flag != 0) {
            record = State_unk15(22);
            State_unk3_4(record, 0x3a80000, 0, 0xb80000);
            record = State_unk16(23);
            State_unk4_4(record, 0x3c80000, 0, 0xf80000);
            record = State_unk17(24);
            State_unk5_4(record, 0x3e80000, 0, 0xb80000);
            record = State_unk18(25);
            State_unk6_4(record, 0x4080000, 0, 0xf80000);
            State_unk50_3(31, 11);
        } else {
            record = State_unk19(22);
            State_unk7_4(record, 0x3a80000, 0, 0xd80000);
            record = State_unk20(23);
            State_unk8_4(record, 0x3c80000, 0, 0xd80000);
            record = State_unk21(24);
            State_unk9_4(record, 0x3e80000, 0, 0xd80000);
            record = State_unk22(25);
            State_unk10_4(record, 0x4080000, 0, 0xd80000);
            State_unk51_3(31, 10);
        }
    }
    count = *(volatile s32 *)base5 + 1;
    *(volatile s32 *)base5 = count;
    if ((u32)count > 119) {
        record = State_unk9(0x104);
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
    State_unk3_2(0x2008715);
    State_unk7_3(22, 0x3a80000, 0xd80000);
    State_unk8_3(23, 0x3c80000, 0xd80000);
    State_unk9_3(24, 0x3e80000, 0xd80000);
    State_unk10_3(25, 0x4080000, 0xd80000);
    State_unk52_3(31, 10);
}

void Scene_RunScene3bb(s32 a0)
{
    u32 i;
    s32 record;

    State_unk53_3(40);
    State_unk54_3(41);
    State_unk55_3(1);
    State_unk56_2();
    State_unk11_3(8, 0x580000, 0x1000000);
    State_unk12_3(0, 0x780000, 0x1000000);
    State_unk13_3(8, 0x4000, 0);
    State_unk14_3(0, 0x4000, 0);
    if (a0 < 0) {
        State_unk57_2(8, 10);
        State_unk58_2(0, 35);
    } else {
        State_unk59(8, 8);
        State_unk60(0, 28);
    }
    State_unk61(1);
    State_unk11_4(0x680000, 0, 0xc00000, 0);
    State_unk62(a0);
    State_unk63();
}

void Scene_RunSecondActorInteraction(s32 a0)
{
    extern s16 gCell2[][1];

    u32 i;
    s32 rec;
    s32 record;

    if (gCell2[225][0] == 2) {
        State_unk64();
    } else {
        State_unk65();
        rec = State_unk10(a0, 2);
        if (rec == 0) {
            State_unk4_2(0x20a2);
            State_unk66();
            State_unk12_4(0x30000, 0x6000);
            State_unk13_4(0x3d80000, -1, 0xe80000, 1);
            State_unk67();
            State_unk11(a0, 0);
            State_unk68();
            State_unk69(a0, 0);
            State_unk70(0, 0x438, 0x108);
            State_unk71(15);
            State_unk15_3(0, 0x18000, 0xc000);
            State_unk16_3(0, 0x438, 216);
            State_unk17_3(0, 0x428, 216);
            State_unk72();
            State_unk73();
            ObjectDispatch_WaitForValue16(-1, -1, -1, 0);
            State_unk74(a0, 0);
            State_unk75(0);
            State_unk76(0, 0);
            State_unk77(a0, 2);
        } else {
            if (rec == 1) {
                State_unk5_2(0x20a1);
                State_unk78(a0, 0);
            }
        }
        State_unk18_3(rec, a0, 2);
        ((void (*)())State_unk23)();
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

void *State_unk79(s32);

s32 *State_unk80();

s32 *State_unk81();

s32 *State_unk82();

u8 *State_unk83();

u8 *State_unk84();
u8 *State_unk85();

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

    work = State_unk79(11);
    if ((*(s32 *)(work + 8) >> 20) == 36) {
        State_unk13_2(0x335);
        v0 = 0x23;
        v1 = 0x4D;
        State_unk4_5(0x23, 0x4E, 1, 1, v0, v1);
    } else {
        State_unk14_2(0x335);
        v0 = 0x23;
        v1 = 0x4D;
        State_unk5_5(0x22, 0x4D, 1, 1, v0, v1);
    }
}

void Scene_RunTwoStepSequence(void)
{
    State_unk86();
    State_unk87();
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

    State_unk88(100, 11, 12, 4, 14, 11);

    field = State_unk80(15)[2] >> 20;
    State_unk89(13, 28, 1, 4, field, 11);

    field = State_unk81(16)[2] >> 20;
    State_unk90(13, 28, 1, 4, field, 11);

    field = State_unk82(17)[4] >> 20;
    State_unk91(13, 28, 4, 1, 18, field);
}

void Scene_RunStep15At29By26(void)
{
    State_unk21_3(15, 29, 26);
    State_unk92();
}

void Scene_RunStep15At33By26(void)
{
    State_unk22_3(15, 33, 26);
    State_unk93();
}

void Scene_RunStep16At45By26(void)
{
    State_unk23_3(16, 45, 26);
    State_unk94();
}

void Scene_RunStep16At49By26(void)
{
    State_unk24_3(16, 49, 26);
    State_unk95();
}

void Scene_RunStep17At40By23(void)
{
    State_unk25_3(17, 40, 23);
    State_unk96();
}

void Scene_RunStep17At40By25(void)
{
    State_unk26_3(17, 40, 25);
    State_unk97();
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

    descriptor = State_unk83(59, 0x7170);
    handle = State_unk24(512);                /* 128 << 2 */

    *(u16 *)(descriptor + 222) = (u16)first;
    *(u16 *)(descriptor + 224) = (u16)second;
    *(u16 *)(descriptor + 226) = (u16)third;
    *(u16 *)(descriptor + 228) = (u16)fourth;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = State_unk84(first);
    second_record = State_unk85(second);

    if (State_unk25(0x109) == 0) {
        *(s32 *)(second_record + 8) =
            (centre << 1) - *(s32 *)(first_record + 8);
        *(s32 *)(second_record + 16) = *(s32 *)(first_record + 16);
    }

    *(u16 *)(descriptor + 218) = 0;
    *(u16 *)(descriptor + 220) = 0;

    State_unk98(gOv12, handle);

    extent = State_unk26();
    *(u16 *)(descriptor + 216) = (u16)extent;
    State_unk27((s16)extent, 512, handle);

    /* The task is published as its entry address plus the Thumb bit. */
    State_unk99((s32)State_unk100 + 1, 0xc76);

    State_unk101(handle);
}

void State_InitControlRecordAndStartTask(void)
{
    u8 *state = gIw;
    Ctl *m = &gOv13;

    State_Apply16(State_unk28(), (s32)(state + 240));
    if (State_unk29(0x109) == 0) {
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

extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;
extern u8 Value_0000207c;

u8 *State_unk102(s32);

u8 *State_unk103(s32);

Obj *State_unk104(s32);

Obj *State_unk105(s32);

void State_SetFlag331AndConfigureRegion46_17(void)
{
    u8 *p;

    State_unk15_2(0x331);
    p = State_unk102(20) + 85;
    *p = 0;
    {
        s32 p5 = 44;
        s32 p6 = 17;

        State_unk6_5(46, 17, 1, 1, p5, p6);
    }
}

void Scene_SetFlag332AndDrawTiles(void)
{
    u8 *slot;

    State_unk16_2(0x332);
    slot = State_unk103(21) + 85;
    *slot = 0;
    {
        s32 v5 = 50;
        s32 v6 = 17;

        State_unk7_5(46, 17, 1, 1, v5, v6);
    }
}

void Scene_SetFlag333AndDrawTiles(void)
{
    State_unk17_2(0x333);
    {
        s32 width = 32;
        s32 height = 77;

        State_unk8_5(32, 37, 1, 4, width, height);
    }
}

        s32 y = State_unk30(904);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        State_unk27_3(1, x, y);
    }
    {
        s32 x = State_unk31(912);
        s32 y = State_unk32(920);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        State_unk28_3(2, x, y);
    }
    {
        s32 x = State_unk33(928);
        s32 y = State_unk34(936);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        State_unk29_3(3, x, y);
    }
}

void mark_scene_progress(void)
{
    extern s32 gCell[];

    u8 *state = gWork;
    s32 v = gCell[125];

    if (v != 0 && ((s32)(s16)*(u16 *)(state + 382) >> 10) == v
        && State_unk35(321) != 0) {
        u16 *p = (u16 *)(state + 386);
        s32 t = 99;

        *p = t;
    }
}

void select_nearest_scene_actor(void)
{
    extern s32 gCell[];

    u8 *state = gWork;
    s32 best = 8;
    s32 bestd = 0x100000;
    s32 n = gCell[125];
    Obj *p = State_unk104(n);
    s32 i;
    s32 *q;
    s32 base;

    State_unk108();
    for (i = 8; i <= 66; i++) {
        Obj *o = State_unk105(i);

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
    State_unk18_2(0x2085);
    State_Apply18(best, 0);
    q = (s32 *)(state + 448);
    *q = 512;
    *(s32 *)(state + 456) = 15;
    State_unk109(20);
    State_unk110();
    State_unk111();
    base = n << 4;
    State_Apply19(base + 880, p->f08 >> 20);
    {
        s32 v = p->f10 >> 20;

        State_unk112(base + 888, v);
    }
    n++;
    if (n > 3) {
        State_unk19_2(10);
        State_unk20_2(282);
    } else {
        State_unk21_2(n);
        State_unk113();
        GetPartyMemberCount();
        *q = 0;
    }
    State_unk115();
}

s32 run_state_interaction(s32 a, s32 b)
{
    extern s16 gCell[];

    s32 v;
    s32 id;
    s32 r;

    State_unk117();
    State_Apply20(b, 5);
    v = gCell[224];
    if (v == (s32)&Value_0000008f) {
        id = (s32)&Value_00002076;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)&Value_00002078;
    } else {
        id = (s32)&Value_0000207a;
    }
    State_unk22_2(id);
    State_Apply21(a, 0);
    if (State_unk12(b + 512) != 0) {
        return 2;
    }
    if (State_unk36(b + 520) != 0) {
        r = State_unk37(0);
        if (r == 1) {
            return 2;
        }
        if (r == 2 || r == -1) {
            return 3;
        }
        return r;
    }
    State_unk23_2(b + 520);
    State_unk24_2((s32)&Value_0000207c);
    State_Apply22(a, 0);
    return State_Apply23(0, 0);
}

/* overlays/scene/state/task/shared.c */

extern u8 HexDigits[];

void Scene_RunSingleStep(void)
{
    State_unk118();
}

void Scene_Forward(void)
{
    State_unk119();
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
u8 *State_unk120();

/* Reset the selected actor's motion fields after refreshing it. */

void StagedActor_ResetMotionAfterRefresh(s32 slot)
{
    u8 *actor = State_unk120(slot);
    State_unk121(actor);
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

SceneRecord *State_unk122(Position3 *, SceneRecord *);
SceneRecord *State_unk123(Position3 *, SceneRecord *);
SceneRecord *State_unk124(Position3 *, SceneRecord *);

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */

/* This overlay's own occupancy lookup for a cell. The record pointer the call
 * sites also load is spelled here, although the lookup itself uses only the
 * position. */

/* In-image direction table: sixteen packed steps, high half x, low half z. */

SceneRecord *State_unk125();   /* scene record for a subject handle */

s32 State_unk126(SceneRecord *, Position3 *);  /* terrain probe */

void State_unk127(SceneRecord *, s32);         /* select presentation mode */

void State_unk128(s32);                        /* wait n frames */

void State_unk129(SceneRecord *, s32, s32, s32);   /* place at (x, y, z) */

void State_unk130(SceneRecord *, s32, s32, s32);   /* place at (x, y, z) */

void State_unk131(s32);                        /* play a cue */

void State_unk132(SceneRecord *);              /* re-attach the camera */

void State_unk133(s32);                        /* play a cue */

void State_unk134(SceneRecord *, s32);         /* select presentation mode */

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
    subject = State_unk125(handle);

    dir = subject->facing >> 12;

    step = gOv15[dir];
    pos.x = subject->x + (s32)(step & 0xffff0000);
    pos.y = subject->y;
    step <<= 16;
    pos.z = subject->z + (s32)step;

    target = State_unk122(&pos, subject);
    if (target == 0) {
        return;
    }

    step = gOv15[dir];
    pos.x = target->x + (s32)(step & 0xffff0000);
    pos.y = target->y;
    step <<= 16;
    pos.z = target->z + (s32)step;

    blocker = State_unk123(&pos, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    pos.x = target->x;
    pos.y = target->y + 0x100000;
    pos.z = target->z;

    blocker = State_unk124(&pos, target);
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
    if (State_unk126(target, &pos) > 0) {
        return;
    }

    State_unk127(subject, 8);
    State_unk128(15);

    target->rate_x = 0x3333;
    target->rate_z = 0x3333;
    State_unk129(target, pos.x, pos.y, pos.z);

    /* The same destination block, moved onto the subject this time. */
    subject->rate_x = 0x3333;
    subject->rate_z = 0x3333;
    State_unk130(subject, pos.x, pos.y, pos.z);

    State_unk131(0xee);
    State_unk132(target);
    State_unk133(0x120);

    target->x = pos.x;
    target->z = pos.z;
    target->motion_24 = zero;
    target->motion_2c = zero;

    State_unk134(subject, 1);
}

/* overlays/scene/state/task/state_update.c */

extern u8 Value_0000000a;
extern volatile s32 gOv10;
extern volatile s32 gOv11;

s32 *State_unk135(s32);

s32 *State_unk136(s32);

s32 *State_unk137(s32);

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

    State_unk9_5(100, 11, 12, 4, fifth, sixth);

    record = State_unk135(12);
    value = record[2] >> 20;
    State_Apply24(832, value);
    State_unk10_5(71, 16, 1, 1, value, 16);

    record = State_unk136(13);
    value = record[2] >> 20;
    State_Apply25(840, value);
    State_unk11_5(71, 16, 1, 1, value, 16);

    record = State_unk137(14);
    value = record[2] >> 20;
    State_Apply26(848, value);
    State_unk12_5(71, 16, 1, 1, value, 16);
}

void State_SetWorkByte35(void)
{
    FIELD_AT_OFFSET(*(void **)0x03001F30, s8 *, 0x35) = 1;
}

void State_SendWord250With6(void)
{
    s16 *tbl = gCell;

    State_unk30_3(*(s32 *)(tbl + 250), 6, 0);
}

void State_ApplyTable8715AndValue104(void)
{
    State_Apply27(0x2008715, 0xC85);
    State_unk25_2(0x104);
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
    State_unk138(10);

    cnt = 0;
    while (gOv10 != 0 || gOv11 != 75) {
        State_unk139(1);
        cnt++;
        if (cnt >= 600) {
            return;
        }
    }
}

void State_InstallTask8714AndApplyTwoRects(void)
{
    State_unk26_2(31);
    State_unk27_2(820);                 /* 205 << 2 */

    if (gOv10 != 0) {
        gOv11 = 0;
    }

    State_unk28_2(30);
    State_unk29_2(1);

    /* The task is published as its entry address with the Thumb bit set. The
     * `.thumb_set` alias the exact reconstruction emits for a Thumb symbol already carries
     * bit 0, so adding it again here overshoots by one. */
    State_unk30_2((s32)State_unk140);

    State_unk13_5(58, 28, 7, 1, 58, 13);
    State_unk14_5(57, 11, 1, 1, 58, 11);
}

void State_InitCursorWhenUnset(void)
{
    s16 *cursor = &gOv;

    if (*cursor == -1) {
        *cursor = State_unk38();
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
        State_unk31_2(1);
    }
}

/* overlays/scene/state/task/no_op_handler.c */
/*
 * resource_3bb owner at 0x0200004c: two bytes, `bx lr', with no prologue and
 * no pool.  Two data-table slots install it as a handler, so the empty body
 * is deliberate rather than padding.  Whether those slots expect a void
 * handler or an identity is not settled here -- `bx lr' leaves r0 untouched,
 * so a caller reading a result gets back whatever it passed in.
 */

void Resource3bb_NoOpHandler(void)
{
}

/* overlays/scene/state/task/run_scene_scene_three_coordinator.c */
extern s16 gCell2[][1];

void Scene_RunSceneThreeCoordinator(s32 a0)
{
    u32 i;
    s32 rec2;
    s32 record;

    if (gCell2[225][0] == 2) {
        State_unk6_4ThreeCoordinator();
    } else {
        State_unk7_4ThreeCoordinator();
        rec2 = State_Check(a0, 3);
        if (rec2 != 0) {
        } else {
            State_Do(0x20a6);
            State_Run(0x30000, 0x6000);
            State_unk2_4(0x2f00000, -1, 0xc00000, 1);
            State_unk8_4ThreeCoordinator();
            State_unk9_4ThreeCoordinator(60);
            State_unk3_4(0x10000, 0x2000);
            State_unk4_4(0x2f00000, -1, 0xe00000, 1);
            State_unk10_4ThreeCoordinator();
            State_unk11_4ThreeCoordinator(a0, 0);
            State_unk12_4ThreeCoordinator(0, 0x358, 0x108);
            State_unk13_4ThreeCoordinator(10);
            State_Place(0, 0x18000, 0xc000);
            State_unk2_3(0, 0x358, 0x108);
            State_unk3_3(0, 0x358, 232);
            State_unk14_4ThreeCoordinator(a0, 0);
            State_unk4_3(0, 0x348, 232);
            State_unk15_4ThreeCoordinator(10);
            State_unk5_3(33, -64, 0);
            State_unk5_4(0x2f00000, -1, 0xd80000, 1);
            State_unk16_4ThreeCoordinator(0, 1);
            State_unk17_4ThreeCoordinator(10);
            State_unk6_3(0, 0x10000, 0x8000);
            State_unk7_3(0, 0x2f8, 232);
            State_unk18_4ThreeCoordinator(10);
            State_unk8_3(0, 0x4000, 30);
            State_unk19_4ThreeCoordinator(a0, 0);
            State_unk20_4ThreeCoordinator(0);
            State_unk21_4ThreeCoordinator(0, 0);
            State_unk9_3(33, 0x3480000, 0xe80000);
            State_unk22_4ThreeCoordinator(a0, 3);
            goto L_020016b0;
        }
        if (rec2 == 1) {
            State_unk2_2(0x20a5);
            State_unk23_4ThreeCoordinator(a0, 0);
        }
        L_020016b0:;
        State_unk10_3(rec2, a0, 3);
        State_unk24_4ThreeCoordinator();
    }
}

/* overlays/scene/state/task/initialize_state_interaction.c */

void State_RunInitializeStateInteraction(s32 a, s32 b)
{
    s32 v;
    s32 id;

    State_ApplyInitializeStateInteraction(b, 5);
    v = gCell[224];
    if (v == (s32)&Value_0000008f) {
        id = (s32)&Value_00002076;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)&Value_00002078;
    } else {
        id = (s32)&Value_0000207a;
    }
    State_Do(id + 1);
    State_Apply2InitializeStateInteraction(a, 0);
}

/* overlays/scene/state/task/initialize_mode_task.c */
/* overlays/scene/state/task/initialize_mode_task.c */
/*
 * Mode task setup for resource_3bb.  It seeds a second per-instance task's
 * state and picks which of five addresses that task dispatches through.  The
 * 144-byte owner includes its 15-word literal pool.
 */

extern struct ModeRecord gOvInitializeModeTask;
extern struct ModeRecord gOv2InitializeModeTask;
extern u8 gOv3InitializeModeTask[];
extern u8 gOv4InitializeModeTask[];
extern u8 gOv5InitializeModeTask[];
extern u8 gOv6InitializeModeTask[];
extern s32 gOv7InitializeModeTask;

extern s32 gOv8InitializeModeTask;   /* Default handler. */
extern s32 gOv9InitializeModeTask;   /* Handler for mode 2. */
extern s32 gOv10InitializeModeTask;   /* Handler for mode 4. */
extern s32 gOv11InitializeModeTask;   /* Handler for mode 3 with param set. */
extern s32 gOv12InitializeModeTask;   /* Handler for mode 3 with param zero. */

/*
 * 0x0200a451 is the installed callback, a routine in this overlay.  The branch
 * chain picks one of five addresses by mode, consulting param only when mode is
 * 3; whether those five are code or per-mode records is not resolved, so they
 * are typed as plain addresses.  The four stores that follow reset the rest of
 * the task's state block, three as halfwords and one as a word.
 */
void State_RunInitializeModeTask(u32 mode, u32 param)
{
    s32 handler;

    gOvInitializeModeTask = (u16)mode;
    gOv2InitializeModeTask = (u16)(param << 4);

    {
        s32 budget = 0xc80;
        s32 task = 0x0200a451;
        State_ApplyInitializeModeTask(task, budget);
    }

    handler = (s32)&gOv8InitializeModeTask;
    if (mode == 2) {
        handler = (s32)&gOv9InitializeModeTask;
    }
    if (mode == 4) {
        handler = (s32)&gOv10InitializeModeTask;
    }
    if (mode == 3) {
        if (param != 0) {
            handler = (s32)&gOv11InitializeModeTask;
        } else {
            handler = (s32)&gOv12InitializeModeTask;
        }
    }

    gOv3InitializeModeTask = 0;
    gOv4InitializeModeTask = handler;
    gOv5InitializeModeTask = 0;
    gOv6InitializeModeTask = 0;
    gOv7InitializeModeTask = 0;
}

/* overlays/scene/state/task/run_scripted_transition.c */
/*
 * resource_3bb scripted transition owner at 0x020029d0, 268 bytes including
 * alignment and its three-word pool.  Mode zero is the short opening; every
 * other mode runs the complete multi-stage transition and publishes flag
 * 0x123 when it closes.
 *
 * Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence, including every repeated State_RunInitializeModeTask,
 * Audio_PlayCue, State_unk2_4, State_unk3_4InitializeModeTask, State_unk4_4/360/370/020
 * call) -- declared/named as the literal per-site targets, not the shared
 * ultimate-destination symbol.
 */

extern void State_unk5_4(void);              /* State_unk4_4 veneer #1 (mode==0 arm) */
extern void State_unk6_4InitializeModeTask(void);              /* State_unk7_4InitializeModeTask veneer #1 */
extern void State_unk8_4InitializeModeTask(void);              /* State_unk9_4InitializeModeTask veneer #1 */
extern void State_unk10_4InitializeModeTask(s32 frames);        /* State_RunInitializeModeTask veneer #1 */
extern void State_unk11_4InitializeModeTask(s32 cue);           /* Audio_PlayCue veneer #1 */
extern void State_unk12_4InitializeModeTask(s32 mode);          /* State_unk2_4 veneer #1 */
extern void State_unk13_4InitializeModeTask(s32 style, s32 variant); /* State_unk3_4InitializeModeTask veneer #1 */
extern void State_unk14_4InitializeModeTask(s32 frames);        /* State_RunInitializeModeTask veneer #2 */
extern void State_unk15_4InitializeModeTask(void);              /* State_unk16_4InitializeModeTask veneer #1 */

extern void State_unk17_4InitializeModeTask(s32 cue);           /* Audio_PlayCue veneer #2 (main arm) */
extern void State_unk18_4InitializeModeTask(void);              /* State_unk4_4 veneer #2 */
extern void State_unk19_4InitializeModeTask(void);              /* State_unk7_4InitializeModeTask veneer #2 */
extern void State_unk20_4InitializeModeTask(void);              /* State_unk9_4InitializeModeTask veneer #2 */
extern void State_unk21_4InitializeModeTask(s32 frames);        /* State_RunInitializeModeTask veneer #3 */
extern void State_unk22_4InitializeModeTask(s32 cue);           /* Audio_PlayCue veneer #3 */
extern void State_unk23_4InitializeModeTask(s32 mode);          /* State_unk2_4 veneer #2 */
extern void State_unk24_4InitializeModeTask(s32 style, s32 variant); /* State_unk3_4InitializeModeTask veneer #2 */
extern void State_unk25_4InitializeModeTask(s32 frames);        /* State_RunInitializeModeTask veneer #4 */

extern void State_unk26_4InitializeModeTask(s32 frames);        /* State_unk27_4InitializeModeTask veneer (loop body) */
extern s32 State_unk28_4InitializeModeTask(void);               /* State_unk29_4InitializeModeTask veneer (loop check) */

extern void State_unk30_4InitializeModeTask(s32 cue);           /* Audio_PlayCue veneer #4 */
extern void State_unk31_4InitializeModeTask(s32 mode);          /* State_unk2_4 veneer #3 */
extern void State_unk32_4InitializeModeTask(s32 style, s32 variant); /* State_unk3_4InitializeModeTask veneer #3 */
extern void State_unk33_4InitializeModeTask(s32 cue);           /* Audio_PlayCue veneer #5 */
extern void State_unk34_4InitializeModeTask(s32 frames);        /* State_RunInitializeModeTask veneer #5 */
extern void State_unk35_4InitializeModeTask(s32 style, s32 variant); /* State_unk3_4InitializeModeTask veneer #4 */
extern void State_unk36_4InitializeModeTask(s32 cue);           /* Audio_PlayCue veneer #6 */
extern void State_unk37_4InitializeModeTask(s32 frames);        /* State_RunInitializeModeTask veneer #6 */
extern void State_unk38_4InitializeModeTask(s32 mode);          /* State_unk2_4 veneer #4 */
extern void State_unk39_4InitializeModeTask(s32 style, s32 variant); /* State_unk3_4InitializeModeTask veneer #5 */
extern void State_unk40_4InitializeModeTask(s32 cue);           /* Audio_PlayCue veneer #7 */
extern void State_unk41_3InitializeModeTask(s32 frames);        /* State_RunInitializeModeTask veneer #7 */
extern void State_unk42_3InitializeModeTask(s32 mode);          /* State_unk2_4 veneer #5 */
extern void State_unk43_3InitializeModeTask(s32 style, s32 variant); /* State_unk3_4InitializeModeTask veneer #6 */
extern void State_unk44_3InitializeModeTask(s32 cue);           /* Audio_PlayCue veneer #8 */
extern void State_unk45_3InitializeModeTask(void);              /* Audio_PlayCueFromEventWork veneer */
extern void State_unk47_3InitializeModeTask(void);              /* State_unk16_4InitializeModeTask veneer #2 */
extern void State_unk48_3InitializeModeTask(s32 flag);          /* GameFlag_Set veneer */

        State_unk6_4InitializeModeTask();
        State_unk8_4InitializeModeTask();
        State_unk10_4InitializeModeTask(30);
        State_unk11_4InitializeModeTask(0x59);
        State_unk12_4InitializeModeTask(0);
        State_unk13_4InitializeModeTask(1, 0);
        State_unk14_4InitializeModeTask(120);
        State_unk15_4InitializeModeTask();
        return;
    }

    State_unk17_4InitializeModeTask(0xf7);
    State_unk18_4InitializeModeTask();
    State_unk19_4InitializeModeTask();
    State_unk20_4InitializeModeTask();
    {
        s16 *base = (s16 *)0x0200ca1e;
        *(s16 *)((u8 *)base + 30) = (s16)(mode * 60);
    }
    State_unk21_4InitializeModeTask(30);
    State_unk22_4InitializeModeTask(mode + 0x5a);
    State_unk23_4InitializeModeTask(mode);
    State_unk24_4InitializeModeTask(1, 0);
    State_unk25_4InitializeModeTask(120);

    goto check_transition;
wait_transition:
    State_unk26_4InitializeModeTask(1);
check_transition:
    if (State_unk28_4InitializeModeTask() != 0)
        goto wait_transition;

    State_unk30_4InitializeModeTask(0x121);
    State_unk31_4InitializeModeTask(5);
    State_unk32_4InitializeModeTask(2, 0);
    State_unk33_4InitializeModeTask(0xec);
    State_unk34_4InitializeModeTask(60);
    State_unk35_4InitializeModeTask(2, 1);
    State_unk36_4InitializeModeTask(0xec);
    State_unk37_4InitializeModeTask(60);
    State_unk38_4InitializeModeTask(6);
    State_unk39_4InitializeModeTask(2, 0);
    State_unk40_4InitializeModeTask(0xec);
    State_unk41_3InitializeModeTask(60);
    State_unk42_3InitializeModeTask(7);
    State_unk43_3InitializeModeTask(4, 0);
    State_unk44_3InitializeModeTask(0xed);
    State_unk45_3InitializeModeTask();
    State_unk47_3InitializeModeTask();
    State_unk48_3InitializeModeTask(0x123);
}

/* overlays/scene/state/task/run_scene_late_sequence.c */
extern u8 gVal[];

/* The two mode records the entry point seeds; the halfword at +26 holds the
 * per-mode span in sixtieths. */
struct ModeRecord {
    u8 pad[26];
    u16 span;
};
extern u8 gCell2[][2];

u8 *State_unk2_4();

u8 *State_unk3_4InitializeModeTask();

void Scene_RunLateSequence(s32 a0)
{
    s32 kind;

    State_unk4_4(247);
    State_unk5_4();
    State_unk6_4InitializeModeTask();
    gOvInitializeModeTask.span = a0 * 60;
    gOv2InitializeModeTask.span = (a0 < 0 ? -a0 : a0) * 60;
    if (a0 < 0) {
        State_unk7_4InitializeModeTask(30);
        State_unk8_4InitializeModeTask(86);
        State_unk9_4InitializeModeTask(8);
        State_Check(3, 1);
        State_unk10_4InitializeModeTask(-a0 * 60 + 60);
        kind = 0;
    } else {
        State_unk11_4InitializeModeTask(30);
        State_unk12_4InitializeModeTask(a0 + 90);
        State_unk13_4InitializeModeTask(4);
        State_unk2(3, 0);
        State_unk14_4InitializeModeTask(a0 * 60 + 60);
        kind = 8;
    }
    State_Place(kind, 0x105, 0);
    while (State_RunInitializeModeTask()!= 0) {
        State_unk15_4InitializeModeTask(1);
    }
    State_unk16_4InitializeModeTask(19);
    State_unk17_4InitializeModeTask(30);
    State_Do(0x121);
    State_unk18_4InitializeModeTask();
    State_unk19_4InitializeModeTask();
}

void State_unk20_4InitializeModeTask(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p11;
    s32 p8;
    s32 p8b;
    u8 *p9;
    s32 p9b;
    u8 *rec;
    s32 record;
    s32 v5;
    s32 base6_4000208;
    s32 v1;
    s32 base4_2002090;
    s32 v4;
    s32 v0;
    s32 slot0;
    u8 *p6;

    p6 = *(volatile s32 *)0x03001e68;
    p11 = a0;
    p8 = a1;
    p10 = a2;
    rec = State_unk2_4();
    p6[6] = 1;
    p6[7] = 4;
    *(volatile s32 *)gOv6InitializeModeTask = *(volatile s32 *)((s32)rec + 8);
    *(volatile s32 *)gOv4InitializeModeTask = *(volatile s32 *)((s32)rec + 16);
    p9 = *(volatile s32 *)((s32)rec + 80);
    *(volatile s32 *)gOv5InitializeModeTask = *(volatile u16 *)((s32)rec + 6);
    v5 = 1;
    State_unk21_4InitializeModeTask(p11, 2);
    {
        u8 value = *(volatile u8 *)&rec[35];

        rec[35] = (u8)(value | v5);
    }
    {
        s32 shown = 0x4000;

        *(volatile u16 *)((s32)rec + 6) = shown;
    }
    State_unk22_4InitializeModeTask((s32)rec, 3);
    State_unk23_4InitializeModeTask((s32)rec, 0);
    State_unk24_4InitializeModeTask((s32)rec, 1);
    p10b = ((s32)p10 << 16);
    p8b = ((s32)p8 << 16);
    State_unk25_4InitializeModeTask(p11, p8b, p10b);
    State_unk26_4InitializeModeTask(0, 0x4000, 0);
    base6_4000208 = 0x4000208;
    base4_2002090 = (s32)gOv3InitializeModeTask;
    v1 = *(volatile u16 *)base6_4000208;
    *(volatile u16 *)base6_4000208 = base6_4000208;
    if (*(volatile u16 *)base4_2002090 <= 31) {
        *(volatile u16 *)base4_2002090 += 1;
        *(volatile s32 *)((((((*(volatile u16 *)base4_2002090 << 1) + *(volatile u16 *)base4_2002090) << 2) + base4_2002090) + 4)) = 0xf00;
        *(volatile s32 *)(((((((*(volatile u16 *)base4_2002090 << 1) + *(volatile u16 *)base4_2002090) << 2) + base4_2002090) + 4) + 4)) = 0x4000050;
        *(volatile s32 *)(((((((*(volatile u16 *)base4_2002090 << 1) + *(volatile u16 *)base4_2002090) << 2) + base4_2002090) + 4) + 4) + 4) = 0x20000;
    }
    *(volatile u16 *)base6_4000208 = v1;
    p9[5] = ((s32)(-13 & p9[5]) | 4);
    p9[17] = ((s32)(-13 & p9[17]) | 4);
    slot0 = base4_2002090;
    v5 = 0;
    State_unk27_4InitializeModeTask(252);
    v4 = slot0;
    do {
        *(volatile s32 *)((s32)rec + 24) = ((v5 << 12) + 0x1000);
        *(volatile s32 *)((s32)rec + 28) = (0x1f000 - (v5 << 12));
        v0 = *(volatile u16 *)base6_4000208;
        *(volatile u16 *)base6_4000208 = base6_4000208;
        if (*(volatile u16 *)(v4) <= 31) {
            *(volatile u16 *)(v4) += 1;
            *(volatile s32 *)(((v4 + (((*(volatile u16 *)(v4) << 1) + *(volatile u16 *)(v4)) << 2)) + 4)) = (((15 - v5) << 8) | (v5 + 1));
            *(volatile s32 *)((((v4 + (((*(volatile u16 *)(v4) << 1) + *(volatile u16 *)(v4)) << 2)) + 4) + 4)) = 0x4000052;
            *(volatile s32 *)((((v4 + (((*(volatile u16 *)(v4) << 1) + *(volatile u16 *)(v4)) << 2)) + 4) + 4) + 4) = 0x20000;
        }
        *(volatile u16 *)base6_4000208 = v0;
        slot0 = v4;
        State_unk28_4InitializeModeTask(1);
        v5 = (v5 + 2);
        v4 = slot0;
    } while (v5 <= 15);
    v4 = *(volatile u16 *)0x04000208;
    *(volatile u16 *)0x04000208 = 0x4000208;
    if (*(volatile u16 *)gOv3InitializeModeTask <= 31) {
        *(volatile u16 *)gOv3InitializeModeTask += 1;
        *(volatile s32 *)((((((*(volatile u16 *)gOv3InitializeModeTask << 1) + *(volatile u16 *)gOv3InitializeModeTask) << 2) + 0x2002090) + 4)) = 16;
        *(volatile s32 *)(((((((*(volatile u16 *)gOv3InitializeModeTask << 1) + *(volatile u16 *)gOv3InitializeModeTask) << 2) + 0x2002090) + 4) + 4)) = 0x4000052;
        *(volatile s32 *)(((((((*(volatile u16 *)gOv3InitializeModeTask << 1) + *(volatile u16 *)gOv3InitializeModeTask) << 2) + 0x2002090) + 4) + 4) + 4) = 0x20000;
    }
    *(volatile u16 *)0x04000208 = v4;
    *(volatile s32 *)((s32)rec + 24) = 0x11000;
    *(volatile s32 *)((s32)rec + 28) = 0xf000;
    State_unk29_4InitializeModeTask(1);
    *(volatile s32 *)((s32)rec + 24) = 0x10000;
    *(volatile s32 *)((s32)rec + 28) = 0x10000;
    State_unk30_4InitializeModeTask(13);
    p9[5] &= -13;
    p9[17] &= -13;
    State_unk31_4InitializeModeTask(p11, 3);
    State_unk32_4InitializeModeTask(20);
    p9b = v5;
}

void State_unk33_4InitializeModeTask(s32 a0)
{
    u32 i;
    u8 *rec7;
    s32 record;
    u8 *p7;

    p7 = *(volatile s32 *)0x03001e68;
    rec7 = State_unk3_4InitializeModeTask();
    if (gCell2[249][0] == 1) {
        gCell2[249][0] = 0;
        State_unk34_4InitializeModeTask(a0, 1);
    } else {
        State_unk2_3(a0, 0x4000, 30);
        State_unk35_4InitializeModeTask(a0, 3);
        State_unk36_4InitializeModeTask(30);
    }
    p7[7] = 0;
    p7[6] = 15;
    *(volatile s32 *)((s32)rec7 + 8) = *(volatile s32 *)gOv6InitializeModeTask;
    *(volatile s32 *)((s32)rec7 + 16) = *(volatile s32 *)gOv4InitializeModeTask;
    *(volatile u16 *)((s32)rec7 + 6) = *(volatile s32 *)gOv5InitializeModeTask;
    *(volatile s32 *)((s32)rec7 + 56) = -0x80000000;
    *(volatile s32 *)((s32)rec7 + 64) = -0x80000000;
    *(volatile s32 *)((s32)rec7 + 36) = 0;
    *(volatile s32 *)((s32)rec7 + 44) = 0;
    rec7[85] = 3;
    rec7[34] = (s32)gVal;
    *(volatile s32 *)((s32)rec7 + 12) = 0;
    *(volatile s32 *)((s32)rec7 + 20) = 0;
    State_unk37_4InitializeModeTask((s32)rec7, 1);
    State_unk38_4InitializeModeTask((s32)rec7, 0);
    State_unk39_4InitializeModeTask((s32)rec7, 1);
    State_unk8_4InitializeModeTask(1);
}

/* overlays/scene/state/task/seed_marker_and_install_task.c */
/* Seed the marker cycle directly from caller coordinates and install it. */
extern u16 gOvAndInstallTask;
extern u16 gOv2AndInstallTask;
extern u16 gOv3AndInstallTask;
extern u16 gOv4AndInstallTask;
extern u16 gOv5AndInstallTask;

/* Per-site veneers (raw sub_ symbols from the overlay .s). */

void Effect_SeedMarkerAndInstallTask(u32 x, u32 y, u32 style)
{
    State_PlaceAndInstallTask(x, y, style);

    gOvAndInstallTask = (u16)x;
    gOv2AndInstallTask = (u16)y;
    gOv3AndInstallTask = (u16)(style & 3);
    gOv4AndInstallTask = 0;
    gOv5AndInstallTask = 0;

    {
        s32 budget = 0xc80;
        State_ApplyAndInstallTask(0x0200ae85, budget);
    }
}

/* overlays/scene/state/task/empty_hook_d.c */
/* Complete two-byte empty hook plus its alignment halfword. */
void Resource3bb_EmptyHook(void)
{
}

/* overlays/scene/state/task/spawn_random_scene_effect.c */
typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[20];
    s32 f28;
    u8 f2c[41];
    u8 f55;
} Obj;

extern u8 gOvSpawnRandomEffect[];

extern Obj *State_RunSpawnRandomEffect(s32, s32, s32, s32);

void spawn_random_scene_effect(Obj *a)
{
    s32 t[3];
    u32 n;

    if (a->f28 >= -255 && a->f28 <= 255) {
        a->f55 = 0;
    }
    n = State_unk3_4SpawnRandomEffect();
    if (n * 100 >> 16 <= 9) {
        Obj *o;
        s32 u;
        s32 w;

        t[0] = a->f08;
        t[1] = a->f0c;
        t[2] = a->f10;
        u = State_Check();
        w = State_unk2();
        State_PlaceSpawnRandomEffect(u << 4, w, t);
        {
            s32 x = t[0];
            s32 y = t[1];
            s32 z = t[2];

            o = State_RunSpawnRandomEffect(285, x, y, z);
        }
        if (o != 0) {
            o->f55 = 0;
            State_ApplySpawnRandomEffect(o, 0);
            State_Apply2SpawnRandomEffect(o, (s32)gOvSpawnRandomEffect);
            State_Apply3SpawnRandomEffect(o, 1);
            State_Apply4SpawnRandomEffect(o, 0);
        }
    }
}

/* overlays/scene/state/task/raise_linked_scene_effect.c */
/* overlays/scene/state/task/raise_linked_scene_effect.c */
typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[65];
    u8 f55;
    u8 f56[14];
    s16 f64;
} Obj;

extern u8 gOvRaiseLinkedEffect[];

extern Obj *State_Run(s16);

    State_SetModeRaiseLinkedEffect(o, a->f08, a->f0c + 0x240000, a->f10);
    o->f55 = 0;
    State_ApplyRaiseLinkedEffect(o, (s32)gOvRaiseLinkedEffect);
    State_DoRaiseLinkedEffect(83);
    a->f64 = 0;
    return 0;
}

/* overlays/scene/state/task/position_active_actor.c */
#if defined(GS1_EDITION_JA)
#define SCENE_TEXT_VALUE 0x98f
#elif defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define SCENE_TEXT_VALUE 0xa5a
#else
#define SCENE_TEXT_VALUE 0x96a
#endif
#if defined(GS1_EDITION_DE)
#define SCENE_WORK_PTR 0x03001f4c
#else
#define SCENE_WORK_PTR 0x03001f3c
#endif
#if defined(GS1_EDITION_FR)
#define SCENE_CALLBACK 0x0200cbcc
#else
#define SCENE_CALLBACK 0x0200cb9c
#endif


s32 State_RunPositionActiveActor();           /* veneer to GameFlag_IsSet */
u8 *Scene_GetRecord();           /* veneer to Scene_GetRecord */

void State_unk3_4();          /* veneer to Object_SetPosition */
void Object_SetCallback();          /* veneer to Object_SetCallback */
void State_unk5_4RaiseLinkedEffect();          /* veneer to State_unk6_4 */
s32 State_unk7_4();           /* local thunk to State_unk8_4, site A */
s32 State_unk9_4();           /* local thunk to State_unk8_4, site B */
void State_unk10_4();          /* veneer to UiText_DrawQuantity, site A */
void State_unk11_4();          /* veneer to UiText_DrawQuantity, site B */
void State_unk12_4();          /* shared veneer, selector refresh + 0x96a */

void State_unk13_4();          /* veneer to ObjectDispatch_WaitForValue16 */

s32 position_active_actor(s32 handleA, s32 handleB)
{
    u8 *workspace = *(u8 **)SCENE_WORK_PTR;
    u8 *shared;
    u8 *record;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = State_RunPositionActiveActor(0x211);

    shared = gCell;
    record = Scene_GetRecord(*(s32 *)(shared + 500));

    if (*(s32 *)(workspace + 232) < *(s32 *)(record + 8)) {
        x = *(s32 *)(workspace + 232) + 0xc0000;
    } else {
        x = *(s32 *)(workspace + 232) - 0xc0000;
    }

    if (flag != 0) {
        z = *(s32 *)(workspace + 236) + 0x100000;
        cuep = (u16 *)(workspace + 228);
    } else {
        z = *(s32 *)(workspace + 236) - 0x100000;
        cuep = (u16 *)(workspace + 226);
    }

    waitp = (s16 *)(record + 100);
    *waitp = *cuep;
    *(s32 *)(record + 52) = 0x4000;
    *(s32 *)(record + 48) = 0x10000;

    State_unk3_4(record, x, 0, z);
    Scene_GetRecord(0x211);
    Object_SetCallback(record, (void *)SCENE_CALLBACK);

    while (*waitp != 0) {
        State_unk5_4RaiseLinkedEffect(1);
    }

    if (flag == 0) {
        State_unk7_4(0, handleA);
        State_unk10_4(handleA, 2);
    } else {
        State_unk9_4(0, handleB);
        State_unk11_4(handleB, 2);
    }

    shared = gCell;
    State_unk12_4(*(s32 *)(shared + 500), 1);
    State_unk17_4RaiseLinkedEffect(SCENE_TEXT_VALUE, 3);
    State_unk13_4(record);

    return flag;
}

/* overlays/scene/state/task/set_work_halfword_dc.c */
void State_SetWorkHalfwordDc(s16 v)
{
    FIELD_AT_OFFSET(*(void **)0x03001F3C, s16 *, 0xDC) = v;
}
