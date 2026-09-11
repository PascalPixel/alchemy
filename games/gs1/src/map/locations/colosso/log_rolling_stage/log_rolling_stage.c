#include "types.h"
#include "scene.h"
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

extern void Map_unk23_4(void);       /* site 0x20024e4 -> Map_unk24_4 veneer */
extern void Map_unk25_4(void);       /* site 0x20024ea -> Map_unk26_4 veneer */
extern s32 Map_unk27_4(s32 actor, s32 slot); /* site 0x20024f2 -> Colosso_RunStateInteraction veneer */
extern void Map_unk28_4(s32 message);  /* site 0x20024fe -> Map_unk29_4 veneer (state==0 arm) */
extern void Map_unk30_4(s32 x, s32 z); /* site 0x200250a -> Map_unk31_4 veneer */
extern void Map_unk32_4(s32 x, s32 y, s32 z, s32 mode); /* site 0x200251c -> Map_unk33_4 veneer */
extern void Map_unk34_4(void);         /* site 0x2002520 -> Map_unk35_4 veneer */
extern void Map_unk36_4(s32 frames);   /* site 0x2002526 -> Map_unk37_4 veneer #1 */
extern void Map_unk38_4(s32 actor, s32 mode); /* site 0x200252e -> Map_unk39_4 veneer #1 */
extern void Map_unk40_4(s32 x, s32 z, s32 mode); /* site 0x2002538 -> Colosso_StartPaletteTask veneer */
extern void Map_unk41_4(s32 frames);   /* site 0x200253e -> Map_unk37_4 veneer #2 */
extern void Map_unk42_4(s32 actor, s32 mode); /* site 0x2002546 -> Map_unk39_4 veneer #2 */
extern void Map_unk43_4(s32 x, s32 z, s32 mode); /* site 0x2002550 -> Colosso_StartPaletteTaskFromState veneer #1 */
extern void Map_unk44_4(s32 frames);   /* site 0x2002556 -> Map_unk37_4 veneer #3 */
extern void Map_unk45_4(s32 actor, s32 mode); /* site 0x200255e -> Map_unk39_4 veneer #3 */
extern void Map_unk46_4(s32 x, s32 z, s32 mode); /* site 0x2002568 -> Colosso_StartPaletteTaskFromState veneer #2 */
extern void Map_unk47_4(s32 frames);   /* site 0x200256e -> Map_unk37_4 veneer #4 */
extern void Map_unk48_4(s32 actor, s32 mode); /* site 0x2002576 -> Map_unk39_4 veneer #4 */
extern void Map_unk49_4(void);         /* site 0x200257a -> Colosso_StopPaletteTask veneer */
extern void Map_unk50_4(s32 frames);   /* site 0x2002580 -> Map_unk37_4 veneer #5 */
extern void Map_unk51_4(s32 actor, s32 mode); /* site 0x2002588 -> Map_unk52_4 veneer */
extern void Map_unk53_4(s32 actor, s32 slot); /* site 0x2002590 -> Colosso_InitializeStateInteraction veneer */
extern void Map_unk54_4(s32 message);  /* site 0x200259c -> Map_unk29_4 veneer (state==1 arm) */
extern void Map_unk55_4(s32 actor, s32 mode); /* site 0x20025a4 -> Map_unk39_4 veneer #5 */
extern s32 Map_unk56_4(s32 state, s32 actor, s32 slot); /* site 0x20025ae -> Scene_RunMiddleSequence veneer */
extern void Map_unk57_4(void);         /* site 0x20025b2 -> Map_unk58_4 veneer */
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
extern void Map_unk59_4(void);              /* Map_unk26_4 veneer #1 (mode==0 arm) */
extern void Map_unk60_4(void);              /* Map_unk61_4 veneer #1 */
extern void Map_unk62_4(void);              /* Map_unk63_4 veneer #1 */
extern void Map_unk64_4(s32 frames);        /* Map_unk37_4 veneer #1 */
extern void Map_unk65_4(s32 cue);           /* Audio_PlayCue veneer #1 */
extern void Map_unk66_4(s32 mode);          /* Map_unk67_4 veneer #1 */
extern void Map_unk68_4(s32 style, s32 variant); /* Colosso_InitializeModeTask veneer #1 */
extern void Map_unk69_4(s32 frames);        /* Map_unk37_4 veneer #2 */
extern void Map_unk70_4(void);              /* Map_unk58_4 veneer #1 */
extern void Map_unk71_4(s32 cue);           /* Audio_PlayCue veneer #2 (main arm) */
extern void Map_unk72_4(void);              /* Map_unk26_4 veneer #2 */
extern void Map_unk73_4(void);              /* Map_unk61_4 veneer #2 */
extern void Map_unk74_4(void);              /* Map_unk63_4 veneer #2 */
extern void Map_unk75_4(s32 frames);        /* Map_unk37_4 veneer #3 */
extern void Map_unk76_4(s32 cue);           /* Audio_PlayCue veneer #3 */
extern void Map_unk77_4(s32 mode);          /* Map_unk67_4 veneer #2 */
extern void Map_unk78_4(s32 style, s32 variant); /* Colosso_InitializeModeTask veneer #2 */
extern void Map_unk79_4(s32 frames);        /* Map_unk37_4 veneer #4 */
extern void Map_unk80_4(s32 frames);        /* Map_unk81_4 veneer (loop body) */
extern s32 Map_unk82_3(void);               /* Map_unk83_3 veneer (loop check) */
extern void Map_unk84_3(s32 cue);           /* Audio_PlayCue veneer #4 */
extern void Map_unk85_3(s32 mode);          /* Map_unk67_4 veneer #3 */
extern void Map_unk86_3(s32 style, s32 variant); /* Colosso_InitializeModeTask veneer #3 */
extern void Map_unk87_3(s32 cue);           /* Audio_PlayCue veneer #5 */
extern void Map_unk88_3(s32 frames);        /* Map_unk37_4 veneer #5 */
extern void Map_unk89_3(s32 style, s32 variant); /* Colosso_InitializeModeTask veneer #4 */
extern void Map_unk90_3(s32 cue);           /* Audio_PlayCue veneer #6 */
extern void Map_unk91_3(s32 frames);        /* Map_unk37_4 veneer #6 */
extern void Map_unk92_3(s32 mode);          /* Map_unk67_4 veneer #4 */
extern void Map_unk93_3(s32 style, s32 variant); /* Colosso_InitializeModeTask veneer #5 */
extern void Map_unk94_3(s32 cue);           /* Audio_PlayCue veneer #7 */
extern void Map_unk95_3(s32 frames);        /* Map_unk37_4 veneer #7 */
extern void Map_unk96_3(s32 mode);          /* Map_unk67_4 veneer #5 */
extern void Map_unk97_3(s32 style, s32 variant); /* Colosso_InitializeModeTask veneer #6 */
extern void Map_unk98_3(s32 cue);           /* Audio_PlayCue veneer #8 */
extern void Map_unk99_3(void);              /* Map_unk100_3 veneer */
extern void Map_unk101_3(void);              /* Map_unk58_4 veneer #2 */
extern void Map_unk102_3(s32 flag);          /* GameFlag_Set veneer */
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

StageActor *Map_unk103_3(s32);

StageActor *Map_unk104_3(s32);

ScaledStageObject *Map_unk105_3();

u8 *Map_unk106_3(s32 object_id);

StageEffect *Map_unk107_3(s32, s32, s32, s32);

StageEffect *Map_unk108_3(s16);

u8 *Map_unk109_3();
u8 *Map_unk110_3();

u8 *Map_unk111_3();
u8 *Map_unk112_3();

SceneRecord *Map_unk113_3();
SceneRecord *Map_unk114_3(Position3 *, SceneRecord *);
SceneRecord *Map_unk115_3(Position3 *, SceneRecord *);
SceneRecord *Map_unk116_3(Position3 *, SceneRecord *);

u8 *Map_unk117_3();

s32 *Map_unk118_3();

s32 *Map_unk119_3();

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

s32 *Map_unk120_3();          /* entity by selector, established */

void Map_unk121_3();          /* unestablished */

void Map_unk122_3();          /* established (record, mode) */

void Map_unk123_3();          /* established (record, x, y, z) */

void Map_unk124_3();          /* unestablished, single argument */

void Map_unk125_3();          /* established (record, mode) */

s32 Map_unk126_3();           /* Map_unk127_3 veneer #1 */

s32 Map_unk128_3();           /* Map_unk127_3 veneer #2 */

void Map_unk129_3();          /* Map_unk130_3 veneer #1 */

s32 Map_unk131_3();           /* Map_unk127_3 veneer #3 */

s32 Map_unk132_3();           /* Map_unk127_3 veneer #4 */

void Map_unk133_3();          /* Map_unk130_3 veneer #2 */

s32 Map_unk134_3();           /* Map_unk127_3 veneer #5 */

s32 Map_unk135_3();           /* Map_unk127_3 veneer #6 */

void Map_unk136_3();          /* Map_unk130_3 veneer #3 */

u8 *Map_unk137_3();           /* veneer, item/party record by id, established */

s32 Map_unk138_3();           /* veneer, established (handle, item) */

void Map_unk139_3();          /* veneer, established (handle, slot) */

u8 *Map_unk140_3();           /* scene-record accessor, established (veneer to Scene_GetRecord) */

void Map_unk141_3();          /* unestablished */

s32 Map_unk142_3(void);       /* established (veneer to Map_unk143_3) */

void Map_unk144_3();          /* sibling item-28 owner, via per-site veneer */

void Map_unk145_3();          /* advance the task scheduler, established (veneer to Map_unk81_4) */

s32 Map_unk146_3();           /* veneer to GameFlag_IsSet */

u8 *Map_unk147_3();         /* veneer to Scene_GetRecord */

void Map_unk148_3();          /* veneer to Object_SetPosition */

void Map_unk149_3();          /* veneer to Object_SetCallback */

void Map_unk150_3();          /* veneer to Map_unk81_4 */

s32 Map_unk151_3();           /* local thunk to Map_unk152_3, site A */

s32 Map_unk153_3();           /* local thunk to Map_unk152_3, site B */

void Map_unk154_3();          /* veneer to UiText_DrawQuantity, site A */

void Map_unk155_3();          /* veneer to UiText_DrawQuantity, site B */

void Map_unk156_3();          /* shared veneer, selector refresh + 0x96a */

void Map_unk157_3();          /* shared veneer, selector refresh + 0x96a */

void Map_unk158_3();          /* veneer to Map_unk159_3 */

void Colosso_PositionAndActivateActor(s32 selector, s32 x, s32 z)
{
    s32 *record;

    record = Map_unk120_3(selector);
    if (record != 0) {
        Map_unk121_3();
        Map_unk122_3(record, 5);
        Map_unk123_3(record, x << 16, record[3], z << 16);
        Map_unk124_3(record);
        Map_unk125_3(record, 1);
    }
}

void Colosso_RunLogRollingInteraction(s32 actor)
{
    extern s16 gCell[];

    s32 state;

    if (gCell[225] == 2) {
        Map_unk23_4();
        return;
    }

    Map_unk25_4();
    state = Map_unk27_4(actor, 6);

    if (state == 0) {
        Map_unk28_4(0x20c7);
        Map_unk30_4(0x30000, 0x6000);
        Map_unk32_4(0x5080000, -1, 0x980000, 1);
        Map_unk34_4();
        Map_unk36_4(30);
        Map_unk38_4(actor, 0);
        Map_unk40_4(0xb4, 0x58, 0);
        Map_unk41_4(60);
        Map_unk42_4(actor, 0);
        Map_unk43_4(0x20, 0x54, 10);
        Map_unk44_4(30);
        Map_unk45_4(actor, 0);
        Map_unk46_4(0x60, 0x54, 30);
        Map_unk47_4(60);
        Map_unk48_4(actor, 0);
        Map_unk49_4();
        Map_unk50_4(2);
        Map_unk51_4(0, 0);
        Map_unk53_4(actor, 6);
    } else if (state == 1) {
        Map_unk54_4(0x20c6);
        Map_unk55_4(actor, 0);
    }

    Map_unk56_4(state, actor, 6);
    Map_unk57_4();
}

void Colosso_RestoreActorPositions(void)
{
    s32 center;

    {
        s32 x = Map_unk126_3(896);
        s32 z = Map_unk128_3(904);
        center = 0x80000;
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Map_unk129_3(1, x, z);
    }
    {
        s32 x = Map_unk131_3(912);
        s32 z = Map_unk132_3(920);
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Map_unk133_3(2, x, z);
    }
    {
        s32 x = Map_unk134_3(928);
        s32 z = Map_unk135_3(936);
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Map_unk136_3(3, x, z);
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
            if (Map_unk24(0x141) != 0) {
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
    target = Map_unk103_3(active_slot);
    Map_unk160_3();
    for (slot = 8; slot <= 66; slot++) {
        actor = Map_unk104_3(slot);
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
    Map_unk12_2(0x2085);
    Map_Apply(best_slot, 0);
    frame = (s32 *)(state + 448);
    *frame = 0x200;
    *(s32 *)(state + 456) = 15;
    Map_unk161_3(20);
    Map_unk162_3();
    Map_unk163_3();
    base = active_slot * 16;
    Map_Apply2(base + 880, target->x >> 20);
    z = target->z >> 20;
    Map_unk164_3(base + 888, z);
    active_slot = active_slot + 1;
    if (active_slot > 3) {
        Map_unk13_2(10);
        Map_unk14_2(282);
    } else {
        Map_unk15_2(active_slot);
        Map_unk165_3();
        Map_unk166_3();
        *frame = 0;
    }
    Map_unk167_3();
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

    Map_unk168_3();
    Map_Apply9(interaction_base, 5);
    stage_variant = gCell[224];
    if (stage_variant == (s32)&Value_0000008f) {
        script_id = (s32)&Value_00002076;
    } else if (stage_variant == (s32)&Value_00000090) {
        script_id = (s32)&Value_00002078;
    } else {
        script_id = (s32)&Value_0000207a;
    }
    Map_unk16_2(script_id);
    Map_Apply10(actor_handle, 0);
    if (Map_unk25(interaction_base + 512) != 0) {
        return 2;
    }
    if (Map_unk26(interaction_base + 520) != 0) {
        result = Map_unk27(0);
        if (result == 1) {
            return 2;
        }
        if (result == 2 || result == -1) {
            return 3;
        }
        return result;
    }
    Map_unk17_2(interaction_base + 520);
    Map_unk18_2((s32)&Value_0000207c);
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
    Map_unk19_2(script_id + 1);
    Map_Apply14(actor_handle, 0);
}

void Colosso_ApplyItemToMatchingSlots(s32 handle, s32 item)
{
    u8 *record;
    s32 slot;

    record = Map_unk137_3(handle);
    Map_unk138_3(handle, item);

    for (slot = 0; slot <= 14; slot++) {
        if (*(u16 *)(record + 216 + slot * 2) == item) {
            Map_unk139_3(handle, slot);
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
 * target at every occurrence, including every repeated Map_unk37_4,
 * Audio_PlayCue, Map_unk67_4, Colosso_InitializeModeTask, Map_unk26_4/360/370/020
 * call) -- declared/named as the literal per-site targets, not the shared
 * ultimate-destination symbol.
 */
void Colosso_RunScriptedTransition(s32 mode)
{
    if (mode == 0) {
        Map_unk59_4();
        Map_unk60_4();
        Map_unk62_4();
        Map_unk64_4(30);
        Map_unk65_4(0x59);
        Map_unk66_4(0);
        Map_unk68_4(1, 0);
        Map_unk69_4(120);
        Map_unk70_4();
        return;
    }

    Map_unk71_4(0xf7);
    Map_unk72_4();
    Map_unk73_4();
    Map_unk74_4();
    {
        s16 *base = (s16 *)0x0200d9a6;
        *(s16 *)((u8 *)base + 30) = (s16)(mode * 60);
    }
    Map_unk75_4(30);
    Map_unk76_4(mode + 0x5a);
    Map_unk77_4(mode);
    Map_unk78_4(1, 0);
    Map_unk79_4(120);

    goto check_transition;
wait_transition:
    Map_unk80_4(1);
check_transition:
    if (Map_unk82_3() != 0)
        goto wait_transition;

    Map_unk84_3(0x121);
    Map_unk85_3(5);
    Map_unk86_3(2, 0);
    Map_unk87_3(0xec);
    Map_unk88_3(60);
    Map_unk89_3(2, 1);
    Map_unk90_3(0xec);
    Map_unk91_3(60);
    Map_unk92_3(6);
    Map_unk93_3(2, 0);
    Map_unk94_3(0xec);
    Map_unk95_3(60);
    Map_unk96_3(7);
    Map_unk97_3(4, 0);
    Map_unk98_3(0xed);
    Map_unk99_3();
    Map_unk101_3();
    Map_unk102_3(0x123);
}

void Colosso_ResetActorMotion(s32 selector)
{
    u8 *record;

    record = Map_unk140_3(selector);
    Map_unk141_3();

    *(u32 *)(record + 36) = 0;
    *(u32 *)(record + 44) = 0;
    *(u32 *)(record + 56) = 0x80000000;
    *(u32 *)(record + 64) = 0x80000000;
}

void Colosso_EnsurePaletteHandle(void)
{
    s16 *cursor = &gOv13;

    if (*cursor == -1) {
        *cursor = Map_unk142_3();
    }
}

void Colosso_StartPaletteTask(u32 first_value, u32 second_value, u32 mode)
{
    Map_unk144_3(first_value, second_value, mode);

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
    Map_unk20_2(Map_unk169_3);
    Map_unk21_2(gOv13);
    gOv13 = -1;
}

void Colosso_PositionScaledObject(s32 id, s32 x, s32 z)
{
    ScaledStageObject *object = Map_unk105_3(id);
    s32 scale;

    if (object != 0) {
        scale = 0x20000;
        object->scale_x = scale;
        object->scale_z = scale / 2;
        object->state = 0;
        Map_unk170_3(object);
        Map_unk171_3(object, 5);
        Map_unk172_3(object, x << 16, object->y, z << 16);
    }
}

void Colosso_SpawnPositionedObject(s32 object_id, s32 x, s32 z)
{
    u8 *object = Map_unk106_3(object_id);

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

    Map_unk173_3();
    Map_Apply18(object, 5);
    Map_SetMode(object, x << 16, *(s32 *)(object + 12), z << 16);
    Map_unk22_2(object);
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
        Map_unk145_3(1);
    }
}

void Colosso_SpawnRandomSceneEffect(StageEffect *source)
{
    s32 position[3];
    u32 random_value;

    if (source->vertical_motion >= -255 && source->vertical_motion <= 255) {
        source->state = 0;
    }
    random_value = Map_unk174_3();
    if (random_value * 100 >> 16 <= 9) {
        StageEffect *effect;
        s32 angle;
        s32 radius;

        position[0] = source->x;
        position[1] = source->y;
        position[2] = source->z;
        angle = Map_unk28();
        radius = Map_unk29();
        Map_unk23_3(angle << 4, radius, position);
        {
            s32 x = position[0];
            s32 y = position[1];
            s32 z = position[2];

            effect = Map_unk107_3(285, x, y, z);
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
    StageEffect *effect = Map_unk108_3(source->linked_effect_slot);

    Map_SetMode2(effect, source->x, source->y + 0x240000, source->z);
    effect->state = 0;
    Map_Apply24(effect, (s32)gOv25);
    Map_unk23_2(83);
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

    flag = Map_unk146_3(0x211);

    shared = gCell;
    record = Map_unk147_3(*(s32 *)(shared + 500));

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

    Map_unk148_3(record, x, 0, z);
    Map_unk109_3(0x211);
    Map_unk149_3(record, (void *)0x0200db24);

    while (*waitp != 0) {
        Map_unk150_3(1);
    }

    if (flag == 0) {
        Map_unk151_3(0, first_handle);
        Map_unk154_3(first_handle, 2);
    } else {
        Map_unk153_3(0, second_handle);
        Map_unk155_3(second_handle, 2);
    }

    shared = gCell;
    Map_unk156_3(*(s32 *)(shared + 500), 1);
    Map_unk157_3(0x96a, 3);
    Map_unk158_3(record);

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

    descriptor = Map_unk110_3(59, 0x7170);
    handle = Map_unk30(512);

    *(u16 *)(descriptor + 222) = (u16)first_actor;
    *(u16 *)(descriptor + 224) = (u16)second_actor;
    *(u16 *)(descriptor + 226) = (u16)third_actor;
    *(u16 *)(descriptor + 228) = (u16)fourth_actor;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = Map_unk111_3(first_actor);
    second_record = Map_unk112_3(second_actor);

    if (Map_unk31(0x109) == 0) {
        *(s32 *)(second_record + 8) =
            (centre << 1) - *(s32 *)(first_record + 8);
        *(s32 *)(second_record + 16) = *(s32 *)(first_record + 16);
    }

    *(u16 *)(descriptor + 218) = 0;
    *(u16 *)(descriptor + 220) = 0;

    Map_unk175_3(gOv26, handle);

    extent = Map_unk32();
    *(u16 *)(descriptor + 216) = (u16)extent;
    Map_unk33((s16)extent, 512, handle);

    Map_unk176_3((s32)Map_unk177_3 + 1, 0xc76);

    Map_unk178_3(handle);
}

void Colosso_InitializeSceneControl(void)
{
    extern SceneControl gOv29;

    u8 *scene_state = gIw;
    SceneControl *control = &gOv29;

    Map_Apply25(Map_unk34(), (s32)(scene_state + 240));
    if (Map_unk35(0x109) == 0) {
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
    subject = Map_unk113_3(subject_handle);

    direction = subject->facing >> 12;

    step = gOv28[direction];
    position.x = subject->x + (s32)(step & 0xffff0000);
    position.y = subject->y;
    step <<= 16;
    position.z = subject->z + (s32)step;

    target = Map_unk114_3(&position, subject);
    if (target == 0) {
        return;
    }

    /* Is the cell one step beyond the target already taken? */
    step = gOv28[direction];
    position.x = target->x + (s32)(step & 0xffff0000);
    position.y = target->y;
    step <<= 16;
    position.z = target->z + (s32)step;

    blocker = Map_unk115_3(&position, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    /* ...and the cell directly above the target? */
    position.x = target->x;
    position.y = target->y + 0x100000;      /* 128 << 13 */
    position.z = target->z;

    blocker = Map_unk116_3(&position, target);
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
    Map_unk24_2(15);

    target->rate_x = 0x3333;
    target->rate_z = 0x3333;
    Map_SetMode3(target, position.x, position.y, position.z);

    subject->rate_x = 0x3333;
    subject->rate_z = 0x3333;
    Map_SetMode4(subject, position.x, position.y, position.z);

    Map_unk25_2(0xee);
    Map_unk26_2(target);
    Map_unk27_2(0x120);                                /* 144 << 1 */

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

    record = Map_unk117_3(((ActiveSubjectSlot *)gCell)->handle);

    /* 128 << 6 = 0x2000 bias, then masked to bits 14-15 (192 << 8). */
    facing = (*(u16 *)(record + 6) + 0x2000) & 0xc000;

    position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
    position[1] = *(s32 *)(record + 12);
    position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
    Map_unk179_3(0x100000, facing, position);          /* 128 << 13 */

    occupant = Map_unk118_3(position, record);
    if (occupant == 0) {
        position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
        position[1] = *(s32 *)(record + 12);
        position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
        Map_unk180_3(0x200000, facing, position);      /* 128 << 14 */

        occupant = Map_unk119_3(position, record);
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
    Map_unk24_3(0, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Map_unk25_3(actorId, 0x10000, 0x8000);
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
        Map_unk181_3();
        v6 = r10;
    } else {
        Map_unk182_3();
        rec5 = Map_Check(a0, 4);
        if (rec5 != 0) {
            v6 = r10;
        } else {
            Map_Do(0x20bf);
            Map_Run(0x30000, 0x6000);
            Map_unk2_4(0x3580000, -1, 0xa80000, 1);
            Map_unk183_3();
            Map_unk184_3(30);
            Map_unk185_3(a0, 0);
            Map_unk186_3(a0, 0);
            Map_Place(0, 0x330, 200);
            Map_unk2_3(0, 0x10000, 0x8000);
            Map_unk3_3(0, 0x348, 200);
            Map_unk4_3(0, 0xc000, 20);
            Map_unk187_3();
            Map_unk3_4(-1, -1, -1, 0);
            Map_unk5_3(0, 0x8000, 0x4000);
            rec7 = Map_unk2(0);
            p8 = *(volatile s32 *)(rec7 + 12);
            p6 = *(volatile s32 *)(rec7 + 8);
            Map_unk6_3(0, 0x8000, 0x4000);
            Map_unk188_3(0, 10);
            p9 = (0x60000 + p8);
            Map_unk189_3(rec7, (s32)p6, p9, *(volatile s32 *)(rec7 + 16));
            Map_unk190_3(rec7);
            Map_unk191_3(0, 14);
            p10 = (0x400000 + (s32)p6);
            Map_unk192_3(rec7, p10, p9, *(volatile s32 *)(rec7 + 16));
            Map_unk193_3(rec7);
            Map_unk194_3(0, 10);
            Map_unk195_3(rec7, p10, (p8 + 0x360000), *(volatile s32 *)(rec7 + 16));
            Map_unk196_3(rec7);
            Map_unk197_3(0, 15);
            v6 = ((s32)p6 + 0x300000);
            v6 = ((s32)p6 + 0x300000);
            Map_unk198_3(rec7, ((s32)p6 + 0x300000), (p8 + 0x360000), *(volatile s32 *)(rec7 + 16));
            Map_unk199_3(rec7);
            Map_unk200_3(0, 12);
            Map_unk201_3(a0, 0);
            Map_unk202_3(0);
            Map_unk203_3(0, 0);
            Map_unk204_3(a0, 4);
            goto L_02002298;
        }
        if (rec5 == 1) {
            Map_unk2_2(0x20be);
            Map_unk205_3(a0, 0);
        }
        L_02002298:;
        Map_unk7_3(rec5, a0, 4);
        Map_unk206_2();
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

    rec = Map_unk3(owner);
    p9 = *(s16 *)(rec + 10);
    p11 = *(s16 *)(rec + 18);
    if (mode != 3) {
        count = Map_unk4_4();
        for (i = 0; i < count; i++) {
            buf[i] = gCell[504 + i];
        }
        if (count <= 1) {
            Map_unk207_2(0x2083);
            Map_unk208_2(owner, 0);
            return;
        }
        if (Map_unk4(base + 512) != 0) {
            Map_unk207_2(0x2084);
            Map_unk208_2(owner, 0);
            return;
        }
        if (mode == 2) {
            state = 0;
            Map_unk209_2(6);
        } else {
            Map_unk3_2(0x207d);
            Map_unk5(owner, 0);
            state = Map_unk6(0, 0);
        }
        if (state == 0) {
            if (state < count) {
                for (i = 0; i < count; i++) {
                    Map_unk210_2((s32)(s8)buf[i]);
                }
            }
            for (i = 0; i < count; i++) {
                if ((s32)(s8)buf[i] != 0) {
                    Map_unk211_2((s32)(s8)buf[i]);
                }
            }
            obj = Map_unk5_4();
            for (i = 0; i < count; i++) {
                Map_unk212_2((s32)(s8)buf[i]);
            }
            for (i = 0; i < count; i++) {
                Map_unk213_2((s32)(s8)buf[i]);
            }
            if (obj != -1) {
                goto L_main;
            }
        }
    }
    Map_unk207_2(0x207e);
    Map_unk208_2(owner, 0);
    return;
L_main:
    ((void (*)())Map_unk36)(obj, 1);
    Map_unk4_2(0x207f);
    Map_unk214_2(owner, 0);
    Map_unk8_3(0, 0x10000, 0x8000);
    Map_unk9_3(obj, 0x10000, 0x8000);
    Map_unk10_3(owner, 0x10000, 0x8000);
    record = Map_unk7(0);
    if (record != 0) {
        Map_unk215_2(obj, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    hi = p11 + 16;
    Map_unk216_2(obj, p9, hi);
    lo = p9 + 16;
    Map_unk11_3(0, lo, hi);
    Map_unk217_2(obj, 0, 30);
    Map_unk218_2(obj, 3);
    tail = hi - 32;
    Map_unk219_2(0, 3);
    Map_unk220_2(owner, p9, tail);
    Map_unk12_3(owner, lo, tail);
    Map_unk221_2(0, obj);
    Map_unk138_3(obj, p9, tail);
    Map_unk222_2(owner, 1);
    Map_unk13_3(owner, 0x8000, 0);
    Map_unk223_2(obj, p9, p11 - 48);
    Map_unk224_2(owner, p9, tail);
    Map_unk225_2(owner, p9, p11);
    Map_unk226_2(obj);
    Map_unk227_2(base + 512);
    rec = Map_unk8(obj);
    sx = *(volatile s32 *)(rec + 8) >> 20;
    Map_unk228_2((obj << 4) + 880, sx);
    sy = *(volatile s32 *)(rec + 16) >> 20;
    Map_unk137_3((obj << 4) + 888, sy);
}

/* map/locations/colosso/log_rolling_stage/run_second_arrival_sequence.c */
/* AUDITED GENERATED CALL SCRIPT for Scene_RunSecondArrivalSequence:
 * state-routed scene setup and all 40 calls with their scene arguments. */

extern s16 gCell[];

void Scene_RunSecondArrivalSequence(s32 scene)
{
    s32 state;

    if (gCell[225] == 2) {
        Map_unk229_2();
        return;
    }
    Map_unk230_2();
    state = Map_unk37(scene, 2);
    if (state == 0) {
    Map_unk5_2(8375);
    Map_unk6_4(196608, 24576);
    Map_unk7_4(24641536, -1, 9961472, 1);
    Map_unk231_2();
    Map_unk232_2(30);
    Map_unk233_2(scene, 0);
    Map_unk38(0, 280, 200);
    Map_unk14_3(0, 98304, 49152);
    Map_unk39(0, 280, 152);
    Map_unk15_3(0, 296, 152);
    Map_unk234_2(10);
    Map_unk235_2();
    Map_unk8_4(-1, -1, -1, 0);
    Map_unk16_3(0, 49152, 15);
    Map_unk236_2();
    Map_unk9_4(-1, -1, -1, 0);
    Map_unk17_3(0, 0, 15);
    Map_unk237_2();
    Map_unk10_4(-1, -1, -1, 0);
    Map_unk18_3(0, 16384, 15);
    Map_unk238_2(scene, 0);
    Map_unk19_3(96, 40, 0);
    Map_unk239_2(128, 40, 10);
    Map_unk240_2(30);
    Map_unk241_2(160, 40, 10);
    Map_unk242_2(30);
    Map_unk243_2(160, 72, 10);
    Map_unk244_2(30);
    Map_unk245_2(scene, 0);
    Map_unk246_2();
    Map_unk247_2(0);
    Map_unk248_2(0, 0);
    Map_unk249_2(scene, 2);
    } else if (state == 1) {
        Map_unk6_2(0x20b6);
        Map_unk250_2(scene, 0);
    }
    Map_unk20_3(state, scene, 2);
    Map_unk251_2();
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

u8 *Map_unk252_2();

u8 *Map_unk253_2();

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
    rec = Map_unk9(0x362);
    if (rec == 0) {
        record = Map_unk10(10);
        if ((s32)record != 0) {
            Map_unk254_2((s32)p6, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Map_unk255_2((s32)p6);
        record = Map_unk11(11);
        record[85] = rec;
        *(s32 *)((s32)record + 52) = 0x6666;
        *(s32 *)((s32)record + 48) = 0xcccc;
        Map_unk11_4((s32)record, *(s32 *)((s32)record + 8), 0x200000, *(s32 *)((s32)record + 16));
        record = Map_unk12(10);
        record[85] = rec;
        *(s32 *)((s32)record + 52) = 0x6666;
        *(s32 *)((s32)record + 48) = 0xcccc;
        Map_unk12_4((s32)record, *(s32 *)((s32)record + 8), 0x40000, *(s32 *)((s32)record + 16));
        rec7 = Map_unk40((s32)p6);
        p9 = rec7 + 85;
        *p9 = rec;
        *(s32 *)(rec7 + 52) = 0x6666;
        *(s32 *)(rec7 + 48) = 0xcccc;
        Map_unk13_4(rec7, *(s32 *)(rec7 + 8), 0x40000, *(s32 *)(rec7 + 16));
        Map_unk256_2(rec7, 1);
        Map_unk257_2((s32)p6);
        Map_SetRect(0, 24, 1, 1, 9, 12);
        Map_unk258_2(2);
        Map_unk259_2(rec7, 1);
        *p9 = 3;
        *(s32 *)(rec7 + 20) = *(s32 *)(rec7 + 12);
        Map_unk7_2(0x367);
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

    rec = (u8 *)Map_unk13(12);
    a = (*(s32 *)((s32)rec + 8) >> 20);
    if (a == 9) {
        b = (*(s32 *)((s32)rec + 16) >> 20);
        if (b == 12) {
            b1 = Map_unk252_2(12);
            Map_unk260_2((s32)b1, 0);
            t = b1 + 35;
            zero = 0;
            two = 2;
            *t = two;
            t += 50;
            *t = zero;
            *(s32 *)((s32)b1 + 52) = 0x6666;
            *(s32 *)((s32)b1 + 48) = 0xcccc;
            Map_unk14_4((s32)b1, *(s32 *)((s32)b1 + 8), 0x40000, *(s32 *)((s32)b1 + 16));
            b2 = (u8 *)Map_unk14(11);
            b2[35] = two;
            *(s32 *)((s32)b2 + 52) = 0x6666;
            *(s32 *)((s32)b2 + 48) = 0xcccc;
            Map_unk15_4((s32)b2, *(s32 *)((s32)b2 + 8), 0x200000, *(s32 *)((s32)b2 + 16));
            b3 = Map_unk253_2(10);
            *(s32 *)((s32)b3 + 52) = 0x6666;
            *(s32 *)((s32)b3 + 48) = 0xcccc;
            Map_unk16_4((s32)b3, *(s32 *)((s32)b3 + 8), 0x40000, *(s32 *)((s32)b3 + 16));
            Map_unk8_2(0x368);
            Map_unk2_5(15, 12, 1, 1, 13, b);
            Map_unk3_5(1, 25, 1, 1, a, b);
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
    rec4 = Map_unk15(*(volatile s32 *)base5_2000434);
    rec2 = Map_unk16(31);
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
    Map_unk21_3(0x100000, tile, (s32)q);
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
            if (Map_unk17(v5, p6) != 0) {
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
        if (Map_unk18(v5, p6) == 0) {
            v9 = (v9 + 1);
            v7 = v5;
            goto L_02000848;
        }
        L_02000862:;
        if (v9 == 0) {
            goto L_020009f2;
        }
        Map_unk4_5(74, 8, 1, 4, (*(volatile s32 *)(rec2 + 8) >> 20), 9);
        Map_unk5_5(120, 60, 8, 5, 74, 60);
        Map_unk261_2();
        Map_unk262_2(p5, 8);
        Map_unk263_2(6);
        *(volatile s32 *)(rec2 + 48) = 0x8000;
        base5_3333 = (s32)gVal;
        *(volatile s32 *)(rec2 + 52) = base5_3333;
        Map_unk19(rec2, slot16);
        Map_unk264_2(rec2, v7, 0, p6);
        Map_unk265_2(6);
        Map_unk266_2(p5, 2);
        record = Map_unk20(27, 0xccc);
        Map_unk267_2(*(volatile s32 *)((record + 0x1e0)), rec2);
        Map_unk22_3(p5, 0x8000, base5_3333);
        Map_unk268_2(239);
        Map_unk269_2(rec4, 2);
        Map_unk270_2(rec4, (((v9 *slot12) << 16) + *(volatile s32 *)(rec4 + 8)), 0, *(volatile s32 *)(rec4 + 16));
        Map_unk271_2(rec4);
        Map_unk21(rec4, 1);
        Map_unk272_2(rec2);
        if (v7 >= 0x5300000) {
            Map_unk9_2(0x369);
            Map_unk273_2(31, 3);
            Map_unk274_2(31, 18, 6);
            Map_unk275_2(30);
            Map_unk276_2(rec2, 8);
            Map_unk277_2(rec2);
            *(u8 *)(rec2 + 35) = 2;
            v5 = 84;
            Map_unk6_5(86, 10, 1, 2, v5, 10);
            Map_unk7_5(86, 9, 1, 1, v5, 12);
            Map_unk10_2(0x120);
            Map_unk278_2(240);
        } else {
            Map_unk279_2(rec2, 1);
            Map_unk11_2(0x120);
            Map_unk280_2(213);
            v5 = (v7 >> 20);
            Map_unk8_5(85, 9, 1, 4, v5, 9);
            Map_unk9_5(85, 9, 1, 4, v5, 61);
        }
        Map_unk281_2(15);
        Map_unk282_2();
        goto L_020009f2;
    }
    L_020009ea:;
    Map_unk283_2();
    Map_unk284_2();
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
        Map_unk285_2();
        base7_2073 = (s32)gVal3;
        threea0 = (a0 << 1) + a0;
        Map_unk286_2(threea0 + base7_2073);
        Map_unk22(a0, 0);
        if (Map_unk23(p8, 0) == 0) {
            s32 t1 = base7_2073 + 1;
            Map_unk287_2(threea0 + t1);
            Map_unk288_2(a0, 0);
            *(volatile s32 *)((0x1c0 + p10)) = 0x200;
            *(volatile s32 *)((0x1c8 + p10)) = 15;
            Map_unk289_2();
            Map_unk290_2();
            Map_unk291_2(a0);
            Map_unk292_2();
            Map_unk293_2();
        } else {
            s32 t2 = base7_2073 + 2;
            Map_unk294_2(threea0 + t2);
            Map_unk295_2(a0, 0);
        }
        Map_unk296_2();
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
    Map_unk297_2(1);
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
    Map_unk298_2(1);
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
    Map_unk299_2(record, 0);
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
    callback_target = (s32)Map_unk300_2;
    PENDING_CALLBACK_FLAG = 9;
    Map_unk17_4(callback_target, 0xc80);
    Battle_WaitMode0_18(5);
    Map_unk301_2(callback_target);
    Battle_WaitMode0_19(55);
    BattleFx_SpawnLinked_3(1, 0x101, 60);
    Map_unk18_4(callback_target, 0xc80);
    Battle_WaitMode0_20(20);
    Map_unk302_2(callback_target);
    Battle_WaitMode0_21(40);
    BattleFx_SpawnLinked_4(2, 0x101, 60);
    PENDING_CALLBACK_FLAG = 9;
    Map_unk19_4(callback_target, 0xc80);
    Battle_WaitMode0_22(35);
    Map_unk303_2(callback_target);
    Battle_WaitMode0_23(25);
    BattleFx_SpawnLinked_5(3, 0x102, 60);
    PENDING_CALLBACK_FLAG = 9;
    Map_unk20_4(callback_target, 0xc80);
    Battle_WaitMode0_24(35);
    Map_unk304_2(callback_target);
    Battle_WaitMode0_25(25);
    BattleFx_SpawnLinked_6(2, 0x102, 60);
    Object_LinkObjectAndSetCallback_14(3, 2);
    Object_LinkObjectAndSetCallback_15(2, 3);
    Battle_WaitMode0_26(60);
    Object_LinkObjectAndSetCallback_16(3, 0);
    Object_LinkObjectAndSetCallback_17(2, 0);
    PENDING_CALLBACK_FLAG = 9;
    Map_unk21_4(callback_target, 0xc80);
    Battle_WaitMode0_27(35);
    Map_unk305_2(callback_target);
    Battle_WaitMode0_28(25);
    BattleFx_SpawnLinked_7(3, 0x108, 60);
    Motion_SetVarCb_1(1, 3);
    Motion_SetVarCb_2(2, 3);
    Motion_SetVarCbObj_6(3, 3);
    Object_LinkObjectAndSetCallback_18(3, 2);
    Object_LinkObjectAndSetCallback_19(1, 2);
    PENDING_CALLBACK_FLAG = 9;
    Map_unk22_4(callback_target, 0xc80);
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
    Map_unk306_2();
    Motion_SetSpeed_5();
    Battle_WaitMode0_33(30);
    Motion_SetHPosTerrain_6(1, 0x5e80000, 0xb00000);
    Motion_SetHPosTerrain_7(2, 0x5b80000, 0xc00000);
    Motion_SetHPosTerrain_8(3, 0x6180000, 0xc80000);
    Map_unk307_2();
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
