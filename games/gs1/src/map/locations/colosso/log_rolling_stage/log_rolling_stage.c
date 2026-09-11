#include "types.h"
#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/log_rolling_stage.h"
#include "colosso_log_rolling_stage.h"

/* map/locations/colosso/log_rolling_stage/log_rolling_stage_tail.c */

typedef struct ActorSpriteRef {
    u8 filler00[0x28];
    s16 *entry;
} ActorSpriteRef;

typedef struct StageActor {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
    u8 filler14[0x3c];
    ActorSpriteRef *sprite;
    u8 state;
} StageActor;

typedef struct ScaledStageObject {
    u8 pad00[12];
    s32 y;                      /* +12 */
    u8 pad10[32];
    s32 scale_x;                /* +48 */
    s32 scale_z;                /* +52 */
    u8 pad38[35];
    u8 state;                   /* +91 */
} ScaledStageObject;

typedef struct StageEffect {
    s32 reserved_00;
    s32 reserved_04;
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[20];
    s32 vertical_motion;
    u8 reserved_2c[41];
    u8 state;
} StageEffect;

typedef struct StageEffect {
    s32 reserved_00;
    s32 reserved_04;
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[65];
    u8 state;
    u8 reserved_56[14];
    s16 linked_effect_slot;
} StageEffect;

typedef struct SceneControl {
    s16 enabled;
    s16 active;
    s16 scene_variant;
    s16 phase;
    s16 timer;
} SceneControl;

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

typedef struct ActiveSubjectSlot {
    u8 pad[500];
    void *handle;
} ActiveSubjectSlot;

extern void Map_Run23(void);       /* site 0x20024e4 -> Map_Run24 veneer */
extern void Map_Run25(void);       /* site 0x20024ea -> Map_Run26 veneer */
extern s32 Map_Run27(s32 actor, s32 slot); /* site 0x20024f2 -> Colosso_RunStateInteraction veneer */
extern void Map_Run28(s32 message);  /* site 0x20024fe -> Map_Run29 veneer (state==0 arm) */
extern void Map_Run30(s32 x, s32 z); /* site 0x200250a -> Map_Run31 veneer */
extern void Map_Run32(s32 x, s32 y, s32 z, s32 mode); /* site 0x200251c -> Map_Run33 veneer */
extern void Map_Run34(void);         /* site 0x2002520 -> Map_Run35 veneer */
extern void Map_Run36(s32 frames);   /* site 0x2002526 -> Map_Run37 veneer #1 */
extern void Map_Run38(s32 actor, s32 mode); /* site 0x200252e -> Map_Run39 veneer #1 */
extern void Map_Run40(s32 x, s32 z, s32 mode); /* site 0x2002538 -> Colosso_StartPaletteTask veneer */
extern void Map_Run41(s32 frames);   /* site 0x200253e -> Map_Run37 veneer #2 */
extern void Map_Run42(s32 actor, s32 mode); /* site 0x2002546 -> Map_Run39 veneer #2 */
extern void Map_Run43(s32 x, s32 z, s32 mode); /* site 0x2002550 -> Colosso_StartPaletteTaskFromState veneer #1 */
extern void Map_Run44(s32 frames);   /* site 0x2002556 -> Map_Run37 veneer #3 */
extern void Map_Run45(s32 actor, s32 mode); /* site 0x200255e -> Map_Run39 veneer #3 */
extern void Map_Run46(s32 x, s32 z, s32 mode); /* site 0x2002568 -> Colosso_StartPaletteTaskFromState veneer #2 */
extern void Map_Run47(s32 frames);   /* site 0x200256e -> Map_Run37 veneer #4 */
extern void Map_Run48(s32 actor, s32 mode); /* site 0x2002576 -> Map_Run39 veneer #4 */
extern void Map_Run49(void);         /* site 0x200257a -> Colosso_StopPaletteTask veneer */
extern void Map_Run50(s32 frames);   /* site 0x2002580 -> Map_Run37 veneer #5 */
extern void Map_Run51(s32 actor, s32 mode); /* site 0x2002588 -> Map_Run52 veneer */
extern void Map_Run53(s32 actor, s32 slot); /* site 0x2002590 -> Colosso_InitializeStateInteraction veneer */
extern void Map_Run54(s32 message);  /* site 0x200259c -> Map_Run29 veneer (state==1 arm) */
extern void Map_Run55(s32 actor, s32 mode); /* site 0x20025a4 -> Map_Run39 veneer #5 */
extern s32 Map_Run56(s32 state, s32 actor, s32 slot); /* site 0x20025ae -> Scene_RunMiddleSequence veneer */
extern void Map_Run57(void);         /* site 0x20025b2 -> Map_Run58 veneer */
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;
extern u8 Value_0000207c;
extern u16 gOv;
extern u16 gOv2;
extern u16 gOv3;
extern s32 gOv4;
extern u16 gOv5;
extern u16 gOv6;
extern s32 gOv7;
extern s32 gOv8;   /* default handler (mode not 2/3/4) */
extern s32 gOv9;   /* handler for mode == 2 */
extern s32 gOv10;   /* handler for mode == 4 */
extern s32 gOv11;   /* handler for mode == 3, parameter != 0 */
extern s32 gOv12;   /* handler for mode == 3, parameter == 0 */
extern void Map_Run59(void);              /* Map_Run26 veneer #1 (mode==0 arm) */
extern void Map_Run60(void);              /* Map_Run61 veneer #1 */
extern void Map_Run62(void);              /* Map_Run63 veneer #1 */
extern void Map_Run64(s32 frames);        /* Map_Run37 veneer #1 */
extern void Map_Run65(s32 cue);           /* Audio_PlayCue veneer #1 */
extern void Map_Run66(s32 mode);          /* Map_Run67 veneer #1 */
extern void Map_Run68(s32 style, s32 variant); /* Colosso_InitializeModeTask veneer #1 */
extern void Map_Run69(s32 frames);        /* Map_Run37 veneer #2 */
extern void Map_Run70(void);              /* Map_Run58 veneer #1 */
extern void Map_Run71(s32 cue);           /* Audio_PlayCue veneer #2 (main arm) */
extern void Map_Run72(void);              /* Map_Run26 veneer #2 */
extern void Map_Run73(void);              /* Map_Run61 veneer #2 */
extern void Map_Run74(void);              /* Map_Run63 veneer #2 */
extern void Map_Run75(s32 frames);        /* Map_Run37 veneer #3 */
extern void Map_Run76(s32 cue);           /* Audio_PlayCue veneer #3 */
extern void Map_Run77(s32 mode);          /* Map_Run67 veneer #2 */
extern void Map_Run78(s32 style, s32 variant); /* Colosso_InitializeModeTask veneer #2 */
extern void Map_Run79(s32 frames);        /* Map_Run37 veneer #4 */
extern void Map_Run80(s32 frames);        /* Map_Run81 veneer (loop body) */
extern s32 Map_Run82(void);               /* Map_Run83 veneer (loop check) */
extern void Map_Run84(s32 cue);           /* Audio_PlayCue veneer #4 */
extern void Map_Run85(s32 mode);          /* Map_Run67 veneer #3 */
extern void Map_Run86(s32 style, s32 variant); /* Colosso_InitializeModeTask veneer #3 */
extern void Map_Run87(s32 cue);           /* Audio_PlayCue veneer #5 */
extern void Map_Run88(s32 frames);        /* Map_Run37 veneer #5 */
extern void Map_Run89(s32 style, s32 variant); /* Colosso_InitializeModeTask veneer #4 */
extern void Map_Run90(s32 cue);           /* Audio_PlayCue veneer #6 */
extern void Map_Run91(s32 frames);        /* Map_Run37 veneer #6 */
extern void Map_Run92(s32 mode);          /* Map_Run67 veneer #4 */
extern void Map_Run93(s32 style, s32 variant); /* Colosso_InitializeModeTask veneer #5 */
extern void Map_Run94(s32 cue);           /* Audio_PlayCue veneer #7 */
extern void Map_Run95(s32 frames);        /* Map_Run37 veneer #7 */
extern void Map_Run96(s32 mode);          /* Map_Run67 veneer #5 */
extern void Map_Run97(s32 style, s32 variant); /* Colosso_InitializeModeTask veneer #6 */
extern void Map_Run98(s32 cue);           /* Audio_PlayCue veneer #8 */
extern void Map_Run99(void);              /* Map_Run100 veneer */
extern void Map_Run101(void);              /* Map_Run58 veneer #2 */
extern void Map_Run102(s32 flag);          /* GameFlag_Set veneer */
extern s16 gOv13;
extern u16 gOv14;
extern u16 gOv15;
extern u16 gOv16;
extern u16 gOv17;
extern u16 gOv18;
extern u16 gOv19;
extern u16 gOv20;
extern u16 gOv21;
extern u16 gOv22;
extern u16 gOv23;
extern u8 gOv24[];
extern u8 gOv25[];
extern u8 gOv26[];
extern u8 *gIw;
extern u8 gOv27[];
extern u32 gOv28[];

StageActor *Map_Run103(s32);

StageActor *Map_Run104(s32);

ScaledStageObject *Map_Run105();

u8 *Map_Run106(s32 object_id);

StageEffect *Map_Run107(s32, s32, s32, s32);

StageEffect *Map_Run108(s16);

u8 *Map_Run109();
u8 *Map_Run110();

u8 *Map_Run111();
u8 *Map_Run112();

SceneRecord *Map_Run113();
SceneRecord *Map_Run114(Position3 *, SceneRecord *);
SceneRecord *Map_Run115(Position3 *, SceneRecord *);
SceneRecord *Map_Run116(Position3 *, SceneRecord *);

u8 *Map_Run117();

s32 *Map_Run118();

s32 *Map_Run119();

/* Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence) -- declared/named as the literal per-site
 * targets, not the shared ultimate-destination symbol. */

/*
 * The two rate words and state byte remain byte-offset accesses because this
 * owner needs the temporary pointer walk to preserve register allocation.
 * Its six imported calls are per-site veneers whose semantics are unproven.
 */

/* Per-site veneers (raw sub_ symbols from the overlay .s). */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */

                                /* allocate/fetch a record by (id, size) */

                                /* reserve a graphics handle */

                                /* release a graphics handle */

                                /* upload image data to a handle */

                                /* upload a palette ramp */

                                /* next palette slot index */

                                /* install a per-frame task (callback, rate) */

                                /* test a story flag */

                                /* scene record for an actor selector */

/* In-image data at file offset 0x3f14 (0x0200cd80 - 0x8000). */

/* The per-frame task this owner installs; in-image code at file offset
 * 0x33a0 (0x0200c0d0 - 0x8000). */

/* Store the caller's halfword in the scene workspace field at offset 220. */

                                /* scene record for a subject handle */

                                /* select presentation mode (record, mode) */

                                /* place the record at (x, y, z) */

                                /* re-attach the camera to a record */

                                /* terrain probe; positive result blocks */

                                /* wait n frames */

                                /* play a cue */

/* This overlay's own byte-exact occupancy lookup. */

/* In-image direction table at file offset 0x4154 (0x0200cfc0 - 0x8000):
 * sixteen packed steps, high half x, low half z. */

s32 *Map_Run120();          /* entity by selector, established */

void Map_Run121();          /* unestablished */

void Map_Run122();          /* established (record, mode) */

void Map_Run123();          /* established (record, x, y, z) */

void Map_Run124();          /* unestablished, single argument */

void Map_Run125();          /* established (record, mode) */

s32 Map_Run126();           /* Map_Run127 veneer #1 */

s32 Map_Run128();           /* Map_Run127 veneer #2 */

void Map_Run129();          /* Map_Run130 veneer #1 */

s32 Map_Run131();           /* Map_Run127 veneer #3 */

s32 Map_Run132();           /* Map_Run127 veneer #4 */

void Map_Run133();          /* Map_Run130 veneer #2 */

s32 Map_Run134();           /* Map_Run127 veneer #5 */

s32 Map_Run135();           /* Map_Run127 veneer #6 */

void Map_Run136();          /* Map_Run130 veneer #3 */

u8 *Map_Run137();           /* veneer, item/party record by id, established */

s32 Map_Run138();           /* veneer, established (handle, item) */

void Map_Run139();          /* veneer, established (handle, slot) */

u8 *Map_Run140();           /* scene-record accessor, established (veneer to Scene_GetRecord) */

void Map_Run141();          /* unestablished */

s32 Map_Run142(void);       /* established (veneer to Map_Run143) */

void Map_Run144();          /* sibling item-28 owner, via per-site veneer */

void Map_Run145();          /* advance the task scheduler, established (veneer to Map_Run81) */

s32 Map_Run146();           /* veneer to GameFlag_IsSet */

u8 *Map_Run147();         /* veneer to Scene_GetRecord */

void Map_Run148();          /* veneer to Object_SetPosition */

void Map_Run149();          /* veneer to Object_SetCallback */

void Map_Run150();          /* veneer to Map_Run81 */

s32 Map_Run151();           /* local thunk to Map_Run152, site A */

s32 Map_Run153();           /* local thunk to Map_Run152, site B */

void Map_Run154();          /* veneer to UiText_DrawQuantity, site A */

void Map_Run155();          /* veneer to UiText_DrawQuantity, site B */

void Map_Run156();          /* shared veneer, selector refresh + 0x96a */

void Map_Run157();          /* shared veneer, selector refresh + 0x96a */

void Map_Run158();          /* veneer to Map_Run159 */

void Colosso_PositionAndActivateActor(s32 selector, s32 x, s32 z)
{
    s32 *record;

    record = Map_Run120(selector);
    if (record != 0) {
        Map_Run121();
        Map_Run122(record, 5);
        Map_Run123(record, x << 16, record[3], z << 16);
        Map_Run124(record);
        Map_Run125(record, 1);
    }
}

void Colosso_RunLogRollingInteraction(s32 actor)
{
    extern s16 gCell[];

    s32 state;

    if (gCell[225] == 2) {
        Map_Run23();
        return;
    }

    Map_Run25();
    state = Map_Run27(actor, 6);

    if (state == 0) {
        Map_Run28(0x20c7);
        Map_Run30(0x30000, 0x6000);
        Map_Run32(0x5080000, -1, 0x980000, 1);
        Map_Run34();
        Map_Run36(30);
        Map_Run38(actor, 0);
        Map_Run40(0xb4, 0x58, 0);
        Map_Run41(60);
        Map_Run42(actor, 0);
        Map_Run43(0x20, 0x54, 10);
        Map_Run44(30);
        Map_Run45(actor, 0);
        Map_Run46(0x60, 0x54, 30);
        Map_Run47(60);
        Map_Run48(actor, 0);
        Map_Run49();
        Map_Run50(2);
        Map_Run51(0, 0);
        Map_Run53(actor, 6);
    } else if (state == 1) {
        Map_Run54(0x20c6);
        Map_Run55(actor, 0);
    }

    Map_Run56(state, actor, 6);
    Map_Run57();
}

void Colosso_RestoreActorPositions(void)
{
    s32 center;

    {
        s32 x = Map_Run126(896);
        s32 z = Map_Run128(904);
        center = 0x80000;
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Map_Run129(1, x, z);
    }
    {
        s32 x = Map_Run131(912);
        s32 z = Map_Run132(920);
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Map_Run133(2, x, z);
    }
    {
        s32 x = Map_Run134(928);
        s32 z = Map_Run135(936);
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Map_Run136(3, x, z);
    }
}

void Colosso_MarkSceneProgress(void)
{
    extern s16 gCell[];

    u8 *state;
    s16 *table;
    s32 slotValue;
    s32 value;
    u16 *field;

    state = *(u8 **)0x03001ebc;
    table = gCell;
    slotValue = *(s32 *)&table[250];
    if (slotValue != 0) {
        if ((s16)*(u16 *)(state + 382) >> 10 == slotValue) {
            if (Map_Check24(0x141) != 0) {
                field = (u16 *)(state + 386);
                value = 99;
                *field = value;
            }
        }
    }
}

void Colosso_SelectNearestObstacle(void)
{
    extern s16 gCell[];

    u8 *state;
    s16 *table;
    StageActor *target;
    StageActor *actor;
    s32 *frame;
    s32 active_slot;
    s32 slot;
    s32 best;
    s32 best_slot;
    s32 dx;
    s32 adx;
    s32 dz;
    s32 base;
    s32 z;

    state = *(u8 **)0x03001ebc;
    best_slot = 8;
    best = 0x100000;
    table = gCell;
    active_slot = *(s32 *)&table[250];
    target = Map_Run103(active_slot);
    Map_Run160();
    for (slot = 8; slot <= 66; slot++) {
        actor = Map_Run104(slot);
        if (actor == 0) {
            continue;
        }
        if (actor->state != 1) {
            continue;
        }
        if (*actor->sprite->entry != 165) {
            continue;
        }
        dx = (target->x - actor->x) / 65536;
        dz = (target->z - actor->z) / 65536;
        if (dz > 0) {
            continue;
        }
        adx = dx;
        if (adx < 0) {
            adx = -adx;
        }
        if (dz < 0) {
            dz = -dz;
        }
        if (adx + dz < best) {
            best_slot = slot;
            best = adx + dz;
        }
    }
    Map_Do12(0x2085);
    Map_Apply(best_slot, 0);
    frame = (s32 *)(state + 448);
    *frame = 0x200;
    *(s32 *)(state + 456) = 15;
    Map_Run161(20);
    Map_Run162();
    Map_Run163();
    base = active_slot * 16;
    Map_Apply2(base + 880, target->x >> 20);
    z = target->z >> 20;
    Map_Run164(base + 888, z);
    active_slot = active_slot + 1;
    if (active_slot > 3) {
        Map_Do13(10);
        Map_Do14(282);
    } else {
        Map_Do15(active_slot);
        Map_Run165();
        Map_Run166();
        *frame = 0;
    }
    Map_Run167();
}

void Colosso_ClearSavedActorPositions(void)
{
    Map_Apply3(896, 0);
    Map_Apply4(904, 0);
    Map_Apply5(912, 0);
    Map_Apply6(920, 0);
    Map_Apply7(928, 0);
    Map_Apply8(936, 0);
}

s32 Colosso_RunStateInteraction(s32 actor_handle, s32 interaction_base)
{
    extern s16 gCell[];

    s32 stage_variant;
    s32 script_id;
    s32 result;

    Map_Run168();
    Map_Apply9(interaction_base, 5);
    stage_variant = gCell[224];
    if (stage_variant == (s32)&Value_0000008f) {
        script_id = (s32)&Value_00002076;
    } else if (stage_variant == (s32)&Value_00000090) {
        script_id = (s32)&Value_00002078;
    } else {
        script_id = (s32)&Value_0000207a;
    }
    Map_Do16(script_id);
    Map_Apply10(actor_handle, 0);
    if (Map_Check25(interaction_base + 512) != 0) {
        return 2;
    }
    if (Map_Check26(interaction_base + 520) != 0) {
        result = Map_Check27(0);
        if (result == 1) {
            return 2;
        }
        if (result == 2 || result == -1) {
            return 3;
        }
        return result;
    }
    Map_Do17(interaction_base + 520);
    Map_Do18((s32)&Value_0000207c);
    Map_Apply11(actor_handle, 0);
    return Map_Apply12(0, 0);
}

void Colosso_InitializeStateInteraction(s32 actor_handle, s32 interaction_base)
{
    extern s16 gCell[];

    s32 stage_variant;
    s32 script_id;

    Map_Apply13(interaction_base, 5);
    stage_variant = gCell[224];
    if (stage_variant == (s32)&Value_0000008f) {
        script_id = (s32)&Value_00002076;
    } else if (stage_variant == (s32)&Value_00000090) {
        script_id = (s32)&Value_00002078;
    } else {
        script_id = (s32)&Value_0000207a;
    }
    Map_Do19(script_id + 1);
    Map_Apply14(actor_handle, 0);
}

void Colosso_ApplyItemToMatchingSlots(s32 handle, s32 item)
{
    u8 *record;
    s32 slot;

    record = Map_Run137(handle);
    Map_Run138(handle, item);

    for (slot = 0; slot <= 14; slot++) {
        if (*(u16 *)(record + 216 + slot * 2) == item) {
            Map_Run139(handle, slot);
        }
    }
}

void Colosso_InitializeModeTask(u32 mode, u32 parameter)
{
    s32 handler;

    gOv = (u16)mode;
    gOv2 = (u16)(parameter << 4);

    {
        s32 budget = 0xc80;
        s32 task = 0x0200aee9;
        Map_Apply15(task, budget);
    }

    handler = (s32)&gOv8;
    if (mode == 2) {
        handler = (s32)&gOv9;
    }
    if (mode == 4) {
        handler = (s32)&gOv10;
    }
    if (mode == 3) {
        if (parameter != 0) {
            handler = (s32)&gOv11;
        } else {
            handler = (s32)&gOv12;
        }
    }

    gOv3 = 0;
    gOv4 = handler;
    gOv5 = 0;
    gOv6 = 0;
    gOv7 = 0;
}

/*
 * resource_3bc scripted transition owner at 0x02003468, 268 bytes including
 * alignment and its three-word pool.  Mode zero is the short opening; every
 * other mode runs the complete multi-stage transition and publishes flag
 * 0x123 when it closes.
 *
 * Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence, including every repeated Map_Run37,
 * Audio_PlayCue, Map_Run67, Colosso_InitializeModeTask, Map_Run26/360/370/020
 * call) -- declared/named as the literal per-site targets, not the shared
 * ultimate-destination symbol.
 */
void Colosso_RunScriptedTransition(s32 mode)
{
    if (mode == 0) {
        Map_Run59();
        Map_Run60();
        Map_Run62();
        Map_Run64(30);
        Map_Run65(0x59);
        Map_Run66(0);
        Map_Run68(1, 0);
        Map_Run69(120);
        Map_Run70();
        return;
    }

    Map_Run71(0xf7);
    Map_Run72();
    Map_Run73();
    Map_Run74();
    {
        s16 *base = (s16 *)0x0200d9a6;
        *(s16 *)((u8 *)base + 30) = (s16)(mode * 60);
    }
    Map_Run75(30);
    Map_Run76(mode + 0x5a);
    Map_Run77(mode);
    Map_Run78(1, 0);
    Map_Run79(120);

    goto check_transition;
wait_transition:
    Map_Run80(1);
check_transition:
    if (Map_Run82() != 0)
        goto wait_transition;

    Map_Run84(0x121);
    Map_Run85(5);
    Map_Run86(2, 0);
    Map_Run87(0xec);
    Map_Run88(60);
    Map_Run89(2, 1);
    Map_Run90(0xec);
    Map_Run91(60);
    Map_Run92(6);
    Map_Run93(2, 0);
    Map_Run94(0xec);
    Map_Run95(60);
    Map_Run96(7);
    Map_Run97(4, 0);
    Map_Run98(0xed);
    Map_Run99();
    Map_Run101();
    Map_Run102(0x123);
}

void Colosso_ResetActorMotion(s32 selector)
{
    u8 *record;

    record = Map_Run140(selector);
    Map_Run141();

    *(u32 *)(record + 36) = 0;
    *(u32 *)(record + 44) = 0;
    *(u32 *)(record + 56) = 0x80000000;
    *(u32 *)(record + 64) = 0x80000000;
}

void Colosso_EnsurePaletteHandle(void)
{
    s16 *cursor = &gOv13;

    if (*cursor == -1) {
        *cursor = Map_Run142();
    }
}

void Colosso_StartPaletteTask(u32 first_value, u32 second_value, u32 mode)
{
    Map_Run144(first_value, second_value, mode);

    gOv14 = (u16)first_value;
    gOv15 = (u16)second_value;
    gOv16 = (u16)(mode & 3);
    gOv17 = 0;
    gOv18 = 0;

    {
        s32 budget = 0xc80;
        s32 task = 0x0200b91d;
        Map_Apply16(task, budget);
    }
}

void Colosso_StartPaletteTaskFromState(u32 first_value, u32 second_value, u32 mode)
{
    gOv19 = (u16)first_value;
    gOv20 = (u16)second_value;
    gOv21 = gOv14;
    gOv22 = gOv15;
    gOv18 = (u16)mode;
    gOv23 = 0;

    {
        s32 budget = 0xc80;
        s32 task = 0x0200b91d;
        Map_Apply17(task, budget);
    }
}

void Colosso_StopPaletteTask(void)
{
    Map_Do20(Map_Run169);
    Map_Do21(gOv13);
    gOv13 = -1;
}

void Colosso_PositionScaledObject(s32 id, s32 x, s32 z)
{
    ScaledStageObject *object = Map_Run105(id);
    s32 scale;

    if (object != 0) {
        scale = 0x20000;
        object->scale_x = scale;
        object->scale_z = scale / 2;
        object->state = 0;
        Map_Run170(object);
        Map_Run171(object, 5);
        Map_Run172(object, x << 16, object->y, z << 16);
    }
}

void Colosso_SpawnPositionedObject(s32 object_id, s32 x, s32 z)
{
    u8 *object = Map_Run106(object_id);

    if (object == 0) {
        return;
    }

    {
        s32 move_rate = 0x14000;
        u8 *state_byte = object;
        u8 zero = 0;
        *(s32 *)(object + 0x30) = move_rate;
        *(s32 *)(object + 0x34) = move_rate >> 1;
        state_byte += 0x5b;
        *state_byte = zero;
    }

    Map_Run173();
    Map_Apply18(object, 5);
    Map_SetMode(object, x << 16, *(s32 *)(object + 12), z << 16);
    Map_Do22(object);
    Map_Apply19(object, 1);
}

/* Complete eight-byte state setter plus its sole four-byte pool word. */
void Colosso_SetBalanceStateReady(void)
{
    u16 *p = (u16 *)0x02001000;
    u16 v = 9;
    *p = v;
}

void Colosso_WaitForBalanceState(void)
{
    extern s16 gOv29;

    s16 *status = &gOv29;

    while (*status != 9) {
        Map_Run145(1);
    }
}

void Colosso_SpawnRandomSceneEffect(StageEffect *source)
{
    s32 position[3];
    u32 random_value;

    if (source->vertical_motion >= -255 && source->vertical_motion <= 255) {
        source->state = 0;
    }
    random_value = Map_Run174();
    if (random_value * 100 >> 16 <= 9) {
        StageEffect *effect;
        s32 angle;
        s32 radius;

        position[0] = source->x;
        position[1] = source->y;
        position[2] = source->z;
        angle = Map_Check28();
        radius = Map_Check29();
        Map_Place23(angle << 4, radius, position);
        {
            s32 x = position[0];
            s32 y = position[1];
            s32 z = position[2];

            effect = Map_Run107(285, x, y, z);
        }
        if (effect != 0) {
            effect->state = 0;
            Map_Apply20(effect, 0);
            Map_Apply21(effect, (s32)gOv24);
            Map_Apply22(effect, 1);
            Map_Apply23(effect, 0);
        }
    }
}

s32 Colosso_RaiseLinkedSceneEffect(StageEffect *source)
{
    StageEffect *effect = Map_Run108(source->linked_effect_slot);

    Map_SetMode2(effect, source->x, source->y + 0x240000, source->z);
    effect->state = 0;
    Map_Apply24(effect, (s32)gOv25);
    Map_Do23(83);
    source->linked_effect_slot = 0;
    return 0;
}

s32 Colosso_PositionActiveActor(s32 first_handle, s32 second_handle)
{
    extern u8 gCell[];

    u8 *workspace = *(u8 **)0x03001f3c;
    u8 *shared;
    u8 *record;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = Map_Run146(0x211);

    shared = gCell;
    record = Map_Run147(*(s32 *)(shared + 500));

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

    Map_Run148(record, x, 0, z);
    Map_Run109(0x211);
    Map_Run149(record, (void *)0x0200db24);

    while (*waitp != 0) {
        Map_Run150(1);
    }

    if (flag == 0) {
        Map_Run151(0, first_handle);
        Map_Run154(first_handle, 2);
    } else {
        Map_Run153(0, second_handle);
        Map_Run155(second_handle, 2);
    }

    shared = gCell;
    Map_Run156(*(s32 *)(shared + 500), 1);
    Map_Run157(0x96a, 3);
    Map_Run158(record);

    return flag;
}

void Colosso_SetupSceneDescriptor(s32 first_actor, s32 second_actor,
                   s32 mode, s32 centre, s32 extra, s32 third_actor,
                   s32 fourth_actor)
{
    u8 *descriptor;
    u8 *first_record;
    u8 *second_record;
    s32 handle;
    s32 extent;

    descriptor = Map_Run110(59, 0x7170);
    handle = Map_Check30(512);

    *(u16 *)(descriptor + 222) = (u16)first_actor;
    *(u16 *)(descriptor + 224) = (u16)second_actor;
    *(u16 *)(descriptor + 226) = (u16)third_actor;
    *(u16 *)(descriptor + 228) = (u16)fourth_actor;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = Map_Run111(first_actor);
    second_record = Map_Run112(second_actor);

    if (Map_Check31(0x109) == 0) {
        *(s32 *)(second_record + 8) =
            (centre << 1) - *(s32 *)(first_record + 8);
        *(s32 *)(second_record + 16) = *(s32 *)(first_record + 16);
    }

    *(u16 *)(descriptor + 218) = 0;
    *(u16 *)(descriptor + 220) = 0;

    Map_Run175(gOv26, handle);

    extent = Map_Check32();
    *(u16 *)(descriptor + 216) = (u16)extent;
    Map_Check33((s16)extent, 512, handle);

    Map_Run176((s32)Map_Run177 + 1, 0xc76);

    Map_Run178(handle);
}

void Colosso_InitializeSceneControl(void)
{
    extern SceneControl gOv29;

    u8 *scene_state = gIw;
    SceneControl *control = &gOv29;

    Map_Apply25(Map_Check34(), (s32)(scene_state + 240));
    if (Map_Check35(0x109) == 0) {
        control->enabled = 1;
        control->active = 1;
        control->scene_variant = *(u16 *)(scene_state + 224);
        control->timer = 0;
        control->phase = 0;
    }
    {
        s32 event_id = 0xc85;

        Map_Apply26((s32)gOv27, event_id);
    }
}

void Colosso_SetSceneControlValue(u16 value)
{
    u8 *workspace = *(u8 **)0x03001f3c;
    *(u16 *)(workspace + 220) = value;
}

void Colosso_PushStagedActor(void)
{
    extern s16 gCell[];

    SceneRecord *subject;
    SceneRecord *target;
    SceneRecord *blocker;
    u32 step;
    u32 direction;
    Position3 position;
    u32 data_index = 250;
    s32 zero;
    s32 subject_handle;

    subject_handle = *(s32 *)((u8 *)gCell + (data_index << 1));
    subject = Map_Run113(subject_handle);

    direction = subject->facing >> 12;

    step = gOv28[direction];
    position.x = subject->x + (s32)(step & 0xffff0000);
    position.y = subject->y;
    step <<= 16;
    position.z = subject->z + (s32)step;

    target = Map_Run114(&position, subject);
    if (target == 0) {
        return;
    }

    /* Is the cell one step beyond the target already taken? */
    step = gOv28[direction];
    position.x = target->x + (s32)(step & 0xffff0000);
    position.y = target->y;
    step <<= 16;
    position.z = target->z + (s32)step;

    blocker = Map_Run115(&position, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    /* ...and the cell directly above the target? */
    position.x = target->x;
    position.y = target->y + 0x100000;      /* 128 << 13 */
    position.z = target->z;

    blocker = Map_Run116(&position, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    target->state = 2;
    zero = 0;

    step = gOv28[direction];
    position.x = target->x + (s32)(step & 0xffff0000);
    position.y = target->y;
    step <<= 16;
    position.z = target->z + (s32)step;

    if (Map_Apply27(target, &position) > 0) {
        return;
    }

    Map_Apply28(subject, 8);
    Map_Do24(15);

    target->rate_x = 0x3333;
    target->rate_z = 0x3333;
    Map_SetMode3(target, position.x, position.y, position.z);

    subject->rate_x = 0x3333;
    subject->rate_z = 0x3333;
    Map_SetMode4(subject, position.x, position.y, position.z);

    Map_Do25(0xee);
    Map_Do26(target);
    Map_Do27(0x120);                                /* 144 << 1 */

    target->x = position.x;
    target->z = position.z;
    target->motion_24 = zero;
    target->motion_2c = zero;

    Map_Apply29(subject, 1);
}

/* This overlay's own byte-exact occupancy lookup. */
s32 *Colosso_FindActorAhead(void)
{
    extern s16 gCell[];

    u8 *record;
    s32 facing;
    s32 position[3];
    s32 *occupant;

    record = Map_Run117(((ActiveSubjectSlot *)gCell)->handle);

    /* 128 << 6 = 0x2000 bias, then masked to bits 14-15 (192 << 8). */
    facing = (*(u16 *)(record + 6) + 0x2000) & 0xc000;

    position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
    position[1] = *(s32 *)(record + 12);
    position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
    Map_Run179(0x100000, facing, position);          /* 128 << 13 */

    occupant = Map_Run118(position, record);
    if (occupant == 0) {
        position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
        position[1] = *(s32 *)(record + 12);
        position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
        Map_Run180(0x200000, facing, position);      /* 128 << 14 */

        occupant = Map_Run119(position, record);
    }

    return occupant;
}

/* map/locations/colosso/log_rolling_stage/party.c */

typedef struct PartyInteractionRecord {
    u8 padding_00[10];
    s16 x;
    u8 padding_0c[6];
    s16 y;
} PartyInteractionRecord;

PartyInteractionRecord *GetPartyInteractionRecord(void);

static inline void InitializeActorZero(void)
{
    Map_Place24(0, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Map_Place25(actorId, 0x10000, 0x8000);
}

void RunPartyCountInteractionCopyB(s32 actorId)
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

/* map/locations/colosso/log_rolling_stage/run_scene_four_step_actor_motion.c */
extern u8 gCell[];
extern u8 gWork[];
extern s16 gCell2[][1];

void Scene_RunFourStepActorMotion(s32 a0)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p11;
    s32 p8;
    s32 p9;
    s32 rec5;
    s32 rec7;
    s32 record;
    s32 r10;
    s32 v6;
    u8 *p6;

    if (gCell2[225][0] == 2) {
        Map_Run181();
        v6 = r10;
    } else {
        Map_Run182();
        rec5 = Map_Check(a0, 4);
        if (rec5 != 0) {
            v6 = r10;
        } else {
            Map_Do(0x20bf);
            Map_Run(0x30000, 0x6000);
            Map_Run2(0x3580000, -1, 0xa80000, 1);
            Map_Run183();
            Map_Run184(30);
            Map_Run185(a0, 0);
            Map_Run186(a0, 0);
            Map_Place(0, 0x330, 200);
            Map_Place2(0, 0x10000, 0x8000);
            Map_Place3(0, 0x348, 200);
            Map_Place4(0, 0xc000, 20);
            Map_Run187();
            Map_Run3(-1, -1, -1, 0);
            Map_Place5(0, 0x8000, 0x4000);
            rec7 = Map_Check2(0);
            p8 = *(volatile s32 *)(rec7 + 12);
            p6 = *(volatile s32 *)(rec7 + 8);
            Map_Place6(0, 0x8000, 0x4000);
            Map_Run188(0, 10);
            p9 = (0x60000 + p8);
            Map_Run189(rec7, (s32)p6, p9, *(volatile s32 *)(rec7 + 16));
            Map_Run190(rec7);
            Map_Run191(0, 14);
            p10 = (0x400000 + (s32)p6);
            Map_Run192(rec7, p10, p9, *(volatile s32 *)(rec7 + 16));
            Map_Run193(rec7);
            Map_Run194(0, 10);
            Map_Run195(rec7, p10, (p8 + 0x360000), *(volatile s32 *)(rec7 + 16));
            Map_Run196(rec7);
            Map_Run197(0, 15);
            v6 = ((s32)p6 + 0x300000);
            v6 = ((s32)p6 + 0x300000);
            Map_Run198(rec7, ((s32)p6 + 0x300000), (p8 + 0x360000), *(volatile s32 *)(rec7 + 16));
            Map_Run199(rec7);
            Map_Run200(0, 12);
            Map_Run201(a0, 0);
            Map_Run202(0);
            Map_Run203(0, 0);
            Map_Run204(a0, 4);
            goto L_02002298;
        }
        if (rec5 == 1) {
            Map_Do2(0x20be);
            Map_Run205(a0, 0);
        }
        L_02002298:;
        Map_Place7(rec5, a0, 4);
        Map_Run206();
    }
    p10b = v6;
    p11 = a0;
}

/* map/locations/colosso/log_rolling_stage/run_scene_late_sequence.c */

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

    rec = Map_Check3(owner);
    p9 = *(s16 *)(rec + 10);
    p11 = *(s16 *)(rec + 18);
    if (mode != 3) {
        count = Map_Run4();
        for (i = 0; i < count; i++) {
            buf[i] = gCell[504 + i];
        }
        if (count <= 1) {
            Map_Run207(0x2083);
            Map_Run208(owner, 0);
            return;
        }
        if (Map_Check4(base + 512) != 0) {
            Map_Run207(0x2084);
            Map_Run208(owner, 0);
            return;
        }
        if (mode == 2) {
            state = 0;
            Map_Run209(6);
        } else {
            Map_Do3(0x207d);
            Map_Check5(owner, 0);
            state = Map_Check6(0, 0);
        }
        if (state == 0) {
            if (state < count) {
                for (i = 0; i < count; i++) {
                    Map_Run210((s32)(s8)buf[i]);
                }
            }
            for (i = 0; i < count; i++) {
                if ((s32)(s8)buf[i] != 0) {
                    Map_Run211((s32)(s8)buf[i]);
                }
            }
            obj = Map_Run5();
            for (i = 0; i < count; i++) {
                Map_Run212((s32)(s8)buf[i]);
            }
            for (i = 0; i < count; i++) {
                Map_Run213((s32)(s8)buf[i]);
            }
            if (obj != -1) {
                goto L_main;
            }
        }
    }
    Map_Run207(0x207e);
    Map_Run208(owner, 0);
    return;
L_main:
    ((void (*)())Map_Check36)(obj, 1);
    Map_Do4(0x207f);
    Map_Run214(owner, 0);
    Map_Place8(0, 0x10000, 0x8000);
    Map_Place9(obj, 0x10000, 0x8000);
    Map_Place10(owner, 0x10000, 0x8000);
    record = Map_Check7(0);
    if (record != 0) {
        Map_Run215(obj, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    hi = p11 + 16;
    Map_Run216(obj, p9, hi);
    lo = p9 + 16;
    Map_Place11(0, lo, hi);
    Map_Run217(obj, 0, 30);
    Map_Run218(obj, 3);
    tail = hi - 32;
    Map_Run219(0, 3);
    Map_Run220(owner, p9, tail);
    Map_Place12(owner, lo, tail);
    Map_Run221(0, obj);
    Map_Run138(obj, p9, tail);
    Map_Run222(owner, 1);
    Map_Place13(owner, 0x8000, 0);
    Map_Run223(obj, p9, p11 - 48);
    Map_Run224(owner, p9, tail);
    Map_Run225(owner, p9, p11);
    Map_Run226(obj);
    Map_Run227(base + 512);
    rec = Map_Check8(obj);
    sx = *(volatile s32 *)(rec + 8) >> 20;
    Map_Run228((obj << 4) + 880, sx);
    sy = *(volatile s32 *)(rec + 16) >> 20;
    Map_Run137((obj << 4) + 888, sy);
}

/* map/locations/colosso/log_rolling_stage/run_second_arrival_sequence.c */
/* AUDITED GENERATED CALL SCRIPT for Scene_RunSecondArrivalSequence:
 * state-routed scene setup and all 40 calls with their scene arguments. */

extern s16 gCell[];

void Scene_RunSecondArrivalSequence(s32 scene)
{
    s32 state;

    if (gCell[225] == 2) {
        Map_Run229();
        return;
    }
    Map_Run230();
    state = Map_Check37(scene, 2);
    if (state == 0) {
    Map_Do5(8375);
    Map_Run6(196608, 24576);
    Map_Run7(24641536, -1, 9961472, 1);
    Map_Run231();
    Map_Run232(30);
    Map_Run233(scene, 0);
    Map_Check38(0, 280, 200);
    Map_Place14(0, 98304, 49152);
    Map_Check39(0, 280, 152);
    Map_Place15(0, 296, 152);
    Map_Run234(10);
    Map_Run235();
    Map_Run8(-1, -1, -1, 0);
    Map_Place16(0, 49152, 15);
    Map_Run236();
    Map_Run9(-1, -1, -1, 0);
    Map_Place17(0, 0, 15);
    Map_Run237();
    Map_Run10(-1, -1, -1, 0);
    Map_Place18(0, 16384, 15);
    Map_Run238(scene, 0);
    Map_Place19(96, 40, 0);
    Map_Run239(128, 40, 10);
    Map_Run240(30);
    Map_Run241(160, 40, 10);
    Map_Run242(30);
    Map_Run243(160, 72, 10);
    Map_Run244(30);
    Map_Run245(scene, 0);
    Map_Run246();
    Map_Run247(0);
    Map_Run248(0, 0);
    Map_Run249(scene, 2);
    } else if (state == 1) {
        Map_Do6(0x20b6);
        Map_Run250(scene, 0);
    }
    Map_Place20(state, scene, 2);
    Map_Run251();
}

/* map/locations/colosso/log_rolling_stage/scene_primary_script_head.c */
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))
#define PENDING_CALLBACK_FLAG (*(s32 *)0x0200db80)

extern u8 gCell[];
extern u8 gWork[];
extern u8 gVal[];
extern u8 gVal2[];
extern u8 gVal3[];
extern u8 gOv30[];
extern u8 gOv31[];

u8 *Map_Run252();

u8 *Map_Run253();

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* Slot at 0x0200db80 set just before installing one of the callbacks below. */

void Scene_RunClosingAuxiliarySequence(void)
{
    u32 i;
    u8 *p9;
    s32 rec;
    s32 rec7;
    u8 *record;
    u8 *p6;

    u8 *base = gCell;

    p6 = *(u8 **)(base + 500);
    rec = Map_Check9(0x362);
    if (rec == 0) {
        record = Map_Check10(10);
        if ((s32)record != 0) {
            Map_Run254((s32)p6, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Map_Run255((s32)p6);
        record = Map_Check11(11);
        record[85] = rec;
        *(s32 *)((s32)record + 52) = 0x6666;
        *(s32 *)((s32)record + 48) = 0xcccc;
        Map_Run11((s32)record, *(s32 *)((s32)record + 8), 0x200000, *(s32 *)((s32)record + 16));
        record = Map_Check12(10);
        record[85] = rec;
        *(s32 *)((s32)record + 52) = 0x6666;
        *(s32 *)((s32)record + 48) = 0xcccc;
        Map_Run12((s32)record, *(s32 *)((s32)record + 8), 0x40000, *(s32 *)((s32)record + 16));
        rec7 = Map_Check40((s32)p6);
        p9 = rec7 + 85;
        *p9 = rec;
        *(s32 *)(rec7 + 52) = 0x6666;
        *(s32 *)(rec7 + 48) = 0xcccc;
        Map_Run13(rec7, *(s32 *)(rec7 + 8), 0x40000, *(s32 *)(rec7 + 16));
        Map_Run256(rec7, 1);
        Map_Run257((s32)p6);
        Map_SetRect(0, 24, 1, 1, 9, 12);
        Map_Run258(2);
        Map_Run259(rec7, 1);
        *p9 = 3;
        *(s32 *)(rec7 + 20) = *(s32 *)(rec7 + 12);
        Map_Do7(0x367);
    }
}

void Scene_RunFinalAuxiliarySequence(void)
{
    u8 *rec;
    u8 *b1;
    u8 *t;
    u8 *b2;
    u8 *b3;
    s32 two;
    s32 zero;
    s32 a;
    s32 b;

    rec = (u8 *)Map_Check13(12);
    a = (*(s32 *)((s32)rec + 8) >> 20);
    if (a == 9) {
        b = (*(s32 *)((s32)rec + 16) >> 20);
        if (b == 12) {
            b1 = Map_Run252(12);
            Map_Run260((s32)b1, 0);
            t = b1 + 35;
            zero = 0;
            two = 2;
            *t = two;
            t += 50;
            *t = zero;
            *(s32 *)((s32)b1 + 52) = 0x6666;
            *(s32 *)((s32)b1 + 48) = 0xcccc;
            Map_Run14((s32)b1, *(s32 *)((s32)b1 + 8), 0x40000, *(s32 *)((s32)b1 + 16));
            b2 = (u8 *)Map_Check14(11);
            b2[35] = two;
            *(s32 *)((s32)b2 + 52) = 0x6666;
            *(s32 *)((s32)b2 + 48) = 0xcccc;
            Map_Run15((s32)b2, *(s32 *)((s32)b2 + 8), 0x200000, *(s32 *)((s32)b2 + 16));
            b3 = Map_Run253(10);
            *(s32 *)((s32)b3 + 52) = 0x6666;
            *(s32 *)((s32)b3 + 48) = 0xcccc;
            Map_Run16((s32)b3, *(s32 *)((s32)b3 + 8), 0x40000, *(s32 *)((s32)b3 + 16));
            Map_Do8(0x368);
            Map_SetRect2(15, 12, 1, 1, 13, b);
            Map_SetRect3(1, 25, 1, 1, a, b);
        }
    }
}

void Scene_RunEarlySequence(void)
{
    s32 rec4;
    s32 rec2;
    s32 base5_2000434;
    s32 tile;
    s32 v3;
    s32 v7;
    s32 v9;
    s32 v5;
    s32 base5_3333;
    s32 slot16;
    s32 slot12;
    s32 record;
    s32 p5;
    s32 p6;
    s32 *q;
    u8 *base0;
    u8 *tbl;
    s32 off;
    s32 t;
    s32 a8;
    volatile s32 *fl;
    s32 slot20[3];

    base0 = gCell;
    base5_2000434 = (s32)(base0 + 500);
    rec4 = Map_Check15(*(volatile s32 *)base5_2000434);
    rec2 = Map_Check16(31);
    tbl = (u8 *)0x0200cc38;
    v9 = 0;
    off = (s32)(((u32)*(volatile u16 *)(rec4 + 6) >> 13) << 1);
    tile = *(volatile u16 *)(tbl + off);
    a8 = *(volatile s32 *)(rec4 + 8);
    p5 = *(volatile s32 *)base5_2000434;
    q = slot20;
    q[0] = ((a8 & -0x100000) + 0x80000);
    q[1] = *(volatile s32 *)(rec4 + 12);
    q[2] = ((*(volatile s32 *)(rec4 + 16) & -0x100000) + 0x80000);
    Map_Place21(0x100000, tile, (s32)q);
    v7 = *(volatile s32 *)(rec2 + 8);
    v3 = q[0] - v7;
    p6 = *(volatile s32 *)(rec2 + 16);
    if (v3 < 0) {
        v3 = v7 - q[0];
        if (v3 > 0x80000) {
            goto L_020009ea;
        }
        t = q[2];
    } else {
        if (v3 > 0x80000) {
            goto L_020009ea;
        }
        t = q[2];
    }
    {
        if ((t - p6) >= 0) {
            if ((t - p6) > 0x200000) {
                goto L_020009ea;
            }
            fl = (volatile s32 *)0x03001ae8;
        } else {
            if ((p6 - t) > 0x200000) {
                goto L_020009ea;
            }
            fl = (volatile s32 *)0x03001ae8;
        }
        if ((*fl & 32) != 0) {
            slot16 = 2;
            slot12 = -8;
            L_0200081c:;
            v5 = (v7 + -0x100000);
            if (Map_Check17(v5, p6) != 0) {
                goto L_02000862;
            }
            v9 = (v9 + 1);
            v7 = v5;
            goto L_0200081c;
        }
        if ((*fl & 16) == 0) {
            goto L_020009f2;
        }
        slot16 = 3;
        slot12 = 8;
        L_02000848:;
        v5 = (v7 + 0x100000);
        if (Map_Check18(v5, p6) == 0) {
            v9 = (v9 + 1);
            v7 = v5;
            goto L_02000848;
        }
        L_02000862:;
        if (v9 == 0) {
            goto L_020009f2;
        }
        Map_SetRect4(74, 8, 1, 4, (*(volatile s32 *)(rec2 + 8) >> 20), 9);
        Map_SetRect5(120, 60, 8, 5, 74, 60);
        Map_Run261();
        Map_Run262(p5, 8);
        Map_Run263(6);
        *(volatile s32 *)(rec2 + 48) = 0x8000;
        base5_3333 = (s32)gVal;
        *(volatile s32 *)(rec2 + 52) = base5_3333;
        Map_Check19(rec2, slot16);
        Map_Run264(rec2, v7, 0, p6);
        Map_Run265(6);
        Map_Run266(p5, 2);
        record = Map_Check20(27, 0xccc);
        Map_Run267(*(volatile s32 *)((record + 0x1e0)), rec2);
        Map_Place22(p5, 0x8000, base5_3333);
        Map_Run268(239);
        Map_Run269(rec4, 2);
        Map_Run270(rec4, (((v9 *slot12) << 16) + *(volatile s32 *)(rec4 + 8)), 0, *(volatile s32 *)(rec4 + 16));
        Map_Run271(rec4);
        Map_Check21(rec4, 1);
        Map_Run272(rec2);
        if (v7 >= 0x5300000) {
            Map_Do9(0x369);
            Map_Run273(31, 3);
            Map_Run274(31, 18, 6);
            Map_Run275(30);
            Map_Run276(rec2, 8);
            Map_Run277(rec2);
            *(u8 *)(rec2 + 35) = 2;
            v5 = 84;
            Map_SetRect6(86, 10, 1, 2, v5, 10);
            Map_SetRect7(86, 9, 1, 1, v5, 12);
            Map_Do10(0x120);
            Map_Run278(240);
        } else {
            Map_Run279(rec2, 1);
            Map_Do11(0x120);
            Map_Run280(213);
            v5 = (v7 >> 20);
            Map_SetRect8(85, 9, 1, 4, v5, 9);
            Map_SetRect9(85, 9, 1, 4, v5, 61);
        }
        Map_Run281(15);
        Map_Run282();
        goto L_020009f2;
    }
    L_020009ea:;
    Map_Run283();
    Map_Run284();
    L_020009f2:;
}

void Scene_RunSupplementalSequenceOne(s32 a0)
{
    u8 *base;
    s32 p10;
    s32 p8;
    s32 base7_2073;
    s32 threea0;
    s32 mode;

    p10 = *(volatile s32 *)gWork;
    base = gCell;
    p8 = *(volatile s32 *)(base + 500);
    mode = *(s16 *)(base + 450);
    if (mode == 2) {
        Map_Run285();
        base7_2073 = (s32)gVal3;
        threea0 = (a0 << 1) + a0;
        Map_Run286(threea0 + base7_2073);
        Map_Check22(a0, 0);
        if (Map_Check23(p8, 0) == 0) {
            s32 t1 = base7_2073 + 1;
            Map_Run287(threea0 + t1);
            Map_Run288(a0, 0);
            *(volatile s32 *)((0x1c0 + p10)) = 0x200;
            *(volatile s32 *)((0x1c8 + p10)) = 15;
            Map_Run289();
            Map_Run290();
            Map_Run291(a0);
            Map_Run292();
            Map_Run293();
        } else {
            s32 t2 = base7_2073 + 2;
            Map_Run294(threea0 + t2);
            Map_Run295(a0, 0);
        }
        Map_Run296();
    }
}

/* Sets up actors 0-3 and a batch of movement/scale/rotation animation
 * records, then plays them through a long fixed sequence of moves,
 * rotations, and callback-driven steps. */
void Scene_RunMultiPhaseActorSequence(s32 a0)
{
    s32 record;
    s32 data_table_addr;
    s32 callback_target;

    ObjectTable_DestroyById_1(39);
    ObjectTable_DestroyById_2(40);
    Map_Run297(1);
    Audio_PlayCue_1(17);
    Battle_Reset_1();
    Motion_SetHPosTerrain_1(8, 0x6080000, 0xc00000);
    if (a0 < 0) {
        Object_SetModeById_1(8, 10);
    } else {
        Object_SetModeById_2(8, 8);
    }
    Motion_EnableActCb_1(8, 0x200d668);
    Motion_SetHPosTerrain_2(0, 0x5e00000, 0xc00000);
    record = Scene_GetRecord_1(0);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    Motion_EnableActCb_2(0, 0x200d738);
    Object_SetModeById_3(0, 35);
    Motion_SetSpeed_1(1, 0x10000, 0x8000);
    Motion_SetSpeed_2(2, 0x10000, 0x8000);
    Motion_SetSpeed_3(3, 0x10000, 0x8000);
    Motion_SetHPosTerrain_3(1, 0x5b80000, 0xb80000);
    Motion_SetHPosTerrain_4(2, 0x5b80000, 0xc80000);
    Motion_SetHPosTerrain_5(3, 0x5a80000, 0xc00000);
    record = Scene_GetRecord_2(1);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    record = Scene_GetRecord_3(2);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    record = Scene_GetRecord_4(3);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    Map_Run298(1);
    Motion_SetPosReset_1(0, 0);
    SCENE_PHASE = 0x100;
    Battle_WaitMode0_1(0x10001, 1);
    Battle_WaitMode0_2();
    Motion_SetSpeed_4();
    SceneWork_SetStepValue_1(0x20f1);
    Battle_WaitMode0_3(60);
    data_table_addr = (s32)gOv31;
    Motion_EnableActCb_3(0, data_table_addr);
    record = Scene_GetRecord_5(0);
    *(s32 *)(record + 24) = 0x10000;
    record = Scene_GetRecord_6(0);
    *(s32 *)(record + 28) = 0x10000;
    Motion_CallWaitAnim_1(0, 36);
    record = Scene_GetRecord_7(0);
    *(s32 *)(record + 8) += 0x30000;
    Battle_WaitMode0_4(10);
    record = Scene_GetRecord_8(0);
    Map_Run299(record, 0);
    Battle_WaitMode0_5(20);
    Motion_EnableActCb_4(0, 0x200d808);
    BattleEv_RunWait_1(1, 0);
    Battle_WaitMode0_6(20);
    Motion_SetPosReset_2(1, 0x5e0, 176);
    Motion_ArmCb_1(1, 0x4000, 10);
    BattleFx_SpawnLinked_1(1, 0x100, 20);
    BattleEv_RunWait_2(2, 0);
    Object_LinkObjectAndSetCallback_1(1, 2);
    Battle_WaitMode0_7(30);
    Motion_SetPosReset_3(2, 0x5d0, 176);
    Motion_ResetPosMode2_1(1, 0x5f0, 184);
    Motion_SetPosReset_4(2, 0x5e0, 176);
    Object_SetModeById_4(1, 1);
    Motion_ArmCb_2(1, 0x6000, 0);
    Motion_ArmCb_3(2, 0x4000, 10);
    Motion_SetVarCbObj_1(2, 2);
    Battle_WaitMode0_8(10);
    Object_LinkObjectAndSetCallback_2(2, 1);
    Battle_WaitMode0_9(30);
    Motion_CallWaitAnim_2(1, 4);
    Battle_WaitMode0_10(30);
    BattleEv_RunWait_3(3, 0);
    Object_LinkObjectAndSetCallback_3(1, 3);
    Object_LinkObjectAndSetCallback_4(2, 3);
    Motion_SetPosReset_5(3, 0x5d0, 184);
    Object_LinkObjectAndSetCallback_5(2, 0);
    Object_LinkObjectAndSetCallback_6(1, 0);
    BattleFx_SpawnLinked_2(1, 0x102, 60);
    Object_LinkObjectAndSetCallback_7(2, 1);
    Object_LinkObjectAndSetCallback_8(1, 2);
    Battle_WaitMode0_11(40);
    Motion_SetVarCbObj_2(3, 2);
    Battle_WaitMode0_12(10);
    Object_LinkObjectAndSetCallback_9(2, 3);
    Object_LinkObjectAndSetCallback_10(1, 3);
    Battle_WaitMode0_13(20);
    BattleEv_RunWait_4(3, 0);
    Motion_EnableActCb_5(0, 0x200d8ac);
    Motion_SetVarCbObj_3(1, 2);
    Object_LinkObjectAndSetCallback_11(1, 0);
    Battle_WaitMode0_14(20);
    BattleEv_RunWait_5(1, 0);
    Motion_SetVarCbObj_4(2, 2);
    Object_LinkObjectAndSetCallback_12(2, 0);
    Battle_WaitMode0_15(20);
    BattleEv_RunWait_6(2, 0);
    Motion_SetVarCbObj_5(3, 2);
    Object_LinkObjectAndSetCallback_13(3, 0);
    Battle_WaitMode0_16(20);
    BattleEv_RunWait_7(3, 0);
    Motion_EnableActCb_6(0, data_table_addr);
    Battle_WaitMode0_17(60);
    callback_target = (s32)Map_Run300;
    PENDING_CALLBACK_FLAG = 9;
    Map_Run17(callback_target, 0xc80);
    Battle_WaitMode0_18(5);
    Map_Run301(callback_target);
    Battle_WaitMode0_19(55);
    BattleFx_SpawnLinked_3(1, 0x101, 60);
    Map_Run18(callback_target, 0xc80);
    Battle_WaitMode0_20(20);
    Map_Run302(callback_target);
    Battle_WaitMode0_21(40);
    BattleFx_SpawnLinked_4(2, 0x101, 60);
    PENDING_CALLBACK_FLAG = 9;
    Map_Run19(callback_target, 0xc80);
    Battle_WaitMode0_22(35);
    Map_Run303(callback_target);
    Battle_WaitMode0_23(25);
    BattleFx_SpawnLinked_5(3, 0x102, 60);
    PENDING_CALLBACK_FLAG = 9;
    Map_Run20(callback_target, 0xc80);
    Battle_WaitMode0_24(35);
    Map_Run304(callback_target);
    Battle_WaitMode0_25(25);
    BattleFx_SpawnLinked_6(2, 0x102, 60);
    Object_LinkObjectAndSetCallback_14(3, 2);
    Object_LinkObjectAndSetCallback_15(2, 3);
    Battle_WaitMode0_26(60);
    Object_LinkObjectAndSetCallback_16(3, 0);
    Object_LinkObjectAndSetCallback_17(2, 0);
    PENDING_CALLBACK_FLAG = 9;
    Map_Run21(callback_target, 0xc80);
    Battle_WaitMode0_27(35);
    Map_Run305(callback_target);
    Battle_WaitMode0_28(25);
    BattleFx_SpawnLinked_7(3, 0x108, 60);
    Motion_SetVarCb_1(1, 3);
    Motion_SetVarCb_2(2, 3);
    Motion_SetVarCbObj_6(3, 3);
    Object_LinkObjectAndSetCallback_18(3, 2);
    Object_LinkObjectAndSetCallback_19(1, 2);
    PENDING_CALLBACK_FLAG = 9;
    Map_Run22(callback_target, 0xc80);
    Object_SetModeById_5(1, 3);
    Object_SetModeById_6(2, 3);
    Object_SetModeById_7(3, 3);
    Battle_WaitMode0_29(60);
    Motion_ResetPosMode2_2(3, 0x5b8, 200);
    Battle_WaitMode0_30(5);
    Motion_ResetPosMode2_3(2, 0x558, 184);
    Battle_WaitMode0_31(3);
    Motion_SetPosReset_6(1, 0x5e8, 184);
    Motion_ResetPosMode2_4(1, 0x558, 184);
    Motion_CommitPos_1(3);
    Object_SetModeById_8(3, 1);
    Object_LinkObjectAndSetCallback_20(3, 0);
    Battle_WaitMode0_32(60);
    Motion_SetPosReset_7(3, 0x598, 200);
    Motion_ResetPosMode2_5(3, 0x558, 184);
    Map_Run306();
    Motion_SetSpeed_5();
    Battle_WaitMode0_33(30);
    Motion_SetHPosTerrain_6(1, 0x5e80000, 0xb00000);
    Motion_SetHPosTerrain_7(2, 0x5b80000, 0xc00000);
    Motion_SetHPosTerrain_8(3, 0x6180000, 0xc80000);
    Map_Run307();
    Battle_WaitMode0_34(0x10000, 2);
    Battle_WaitMode0_35(1);
    SceneWork_SetStepValue_2(0x214c);
    BattleEv_RunWait_8(1, 0);
    BattleEv_RunWait_9(2, 0);
    BattleEv_RunWait_10(3, 0);
    Battle_WaitMode0_36(60);
    Battle_SchedShoulder_1();
}

/* map/locations/colosso/log_rolling_stage/shared.c */

extern u8 HexDigitCharacters[];
extern u8 *gWork;

void WriteU32AsHex(u8 *hex_text, u32 value)
{
    s32 digit_index;

    hex_text += 8;
    *hex_text = 0;
    hex_text--;
    for (digit_index = 7; digit_index >= 0; digit_index--) {
        *hex_text = HexDigitCharacters[value & 15];
        value >>= 4;
        hex_text--;
    }
}

s32 *FindActorAtWholeTilePosition(s32 *position)
{
    s32 **slots = (s32 **)(gWork + 0x14);
    u32 actor_index;

    for (actor_index = 8; actor_index <= 65; actor_index++) {
        s32 *actor = slots[actor_index];

        if ((position[0] >> 20) == (actor[2] >> 20)
            && (position[1] >> 20) == (actor[3] >> 20)
            && (position[2] >> 20) == (actor[4] >> 20)) {
            return actor;
        }
    }
    return 0;
}
