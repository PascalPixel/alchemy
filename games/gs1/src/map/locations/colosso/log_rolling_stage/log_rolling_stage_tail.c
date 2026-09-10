#include "types.h"

#define ColossoLogRollingStage_PositionAndActivateActor Func_020022f4
#define ColossoLogRollingStage_RunLogRollingInteraction Func_020024d0
#define ColossoLogRollingStage_RestoreActorPositions Func_020025c8
#define ColossoLogRollingStage_MarkSceneProgress Func_02002710
#define ColossoLogRollingStage_SelectNearestObstacle Func_02002758
#define ColossoLogRollingStage_ClearSavedActorPositions Func_02002a50
#define ColossoLogRollingStage_RunStateInteraction Func_02002a94
#define ColossoLogRollingStage_InitializeStateInteraction Func_02002b50
#define ColossoLogRollingStage_ApplyItemToMatchingSlots Func_02002e18
#define ColossoLogRollingStage_InitializeModeTask Func_020033d8
#define ColossoLogRollingStage_RunScriptedTransition Func_02003468
#define ColossoLogRollingStage_ResetActorMotion Func_020038dc
#define ColossoLogRollingStage_EnsurePaletteHandle Func_020038fc
#define ColossoLogRollingStage_StartPaletteTask Func_02003a58
#define ColossoLogRollingStage_StartPaletteTaskFromState Func_02003abc
#define ColossoLogRollingStage_StopPaletteTask Func_02003b18
#define ColossoLogRollingStage_PositionScaledObject Func_02003b40
#define ColossoLogRollingStage_SpawnPositionedObject Func_02003b80
#define ColossoLogRollingStage_SetBalanceStateReady Func_02003cc4
#define ColossoLogRollingStage_WaitForBalanceState Func_02003cd0
#define ColossoLogRollingStage_SpawnRandomSceneEffect Func_02003cf4
#define ColossoLogRollingStage_RaiseLinkedSceneEffect Func_02003d88
#define ColossoLogRollingStage_PositionActiveActor Func_02003ddc
#define ColossoLogRollingStage_SetupSceneDescriptor Func_02004494
#define ColossoLogRollingStage_InitializeSceneControl Func_0200457c
#define ColossoLogRollingStage_SetSceneControlValue Func_020045d0
#define ColossoLogRollingStage_PushStagedActor Func_02004628
#define ColossoLogRollingStage_FindActorAhead Func_02004790
#include "colosso_log_rolling_stage.h"

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

typedef struct StageEffect_02003d88 {
    s32 reserved_00;
    s32 reserved_04;
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[65];
    u8 state;
    u8 reserved_56[14];
    s16 linked_effect_slot;
} StageEffect_02003d88;

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

extern void Func_02004d72(void);       /* site 0x20024e4 -> Func_0200288c veneer */
extern void Func_02006eec(void);       /* site 0x20024ea -> Func_0808a018 veneer */
extern s32 Func_02004f88(s32 actor, s32 slot); /* site 0x20024f2 -> Func_02002a94 veneer */
extern void Func_02006fb0(s32 message);  /* site 0x20024fe -> Func_0808a170 veneer (state==0 arm) */
extern void Func_02007004(s32 x, s32 z); /* site 0x200250a -> Func_0808a208 veneer */
extern void Func_0200701e(s32 x, s32 y, s32 z, s32 mode); /* site 0x200251c -> Func_0808a210 veneer */
extern void Func_0200702a(void);         /* site 0x2002520 -> Func_0808a218 veneer */
extern void Func_02006f20(s32 frames);   /* site 0x2002526 -> Func_0808a010 veneer #1 */
extern void Func_02006ff0(s32 actor, s32 mode); /* site 0x200252e -> Func_0808a180 veneer #1 */
extern void Func_02005f92(s32 x, s32 z, s32 mode); /* site 0x2002538 -> Func_02003a58 veneer */
extern void Func_02006f38(s32 frames);   /* site 0x200253e -> Func_0808a010 veneer #2 */
extern void Func_02007008(s32 actor, s32 mode); /* site 0x2002546 -> Func_0808a180 veneer #2 */
extern void Func_0200600e(s32 x, s32 z, s32 mode); /* site 0x2002550 -> Func_02003abc veneer #1 */
extern void Func_02006f50(s32 frames);   /* site 0x2002556 -> Func_0808a010 veneer #3 */
extern void Func_02007020(s32 actor, s32 mode); /* site 0x200255e -> Func_0808a180 veneer #3 */
extern void Func_02006026(s32 x, s32 z, s32 mode); /* site 0x2002568 -> Func_02003abc veneer #2 */
extern void Func_02006f68(s32 frames);   /* site 0x200256e -> Func_0808a010 veneer #4 */
extern void Func_02007038(s32 actor, s32 mode); /* site 0x2002576 -> Func_0808a180 veneer #4 */
extern void Func_02006094(void);         /* site 0x200257a -> Func_02003b18 veneer */
extern void Func_02006f7a(s32 frames);   /* site 0x2002580 -> Func_0808a010 veneer #5 */
extern void Func_0200707a(s32 actor, s32 mode); /* site 0x2002588 -> Func_0808a200 veneer */
extern void Func_020050e2(s32 actor, s32 slot); /* site 0x2002590 -> Func_02002b50 veneer */
extern void Func_0200704e(s32 message);  /* site 0x200259c -> Func_0808a170 veneer (state==1 arm) */
extern void Func_02007066(s32 actor, s32 mode); /* site 0x20025a4 -> Func_0808a180 veneer #5 */
extern s32 Func_0200515c(s32 state, s32 actor, s32 slot); /* site 0x20025ae -> Func_02002bac veneer */
extern void Func_02006fbc(void);         /* site 0x20025b2 -> Func_0808a020 veneer */
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;
extern u8 Value_0000207c;
extern u16 Data_0200dbd0;
extern u16 Data_0200dba4;
extern u16 Data_0200dbdc;
extern s32 Data_0200dbe0;
extern u16 Data_0200dc38;
extern u16 Data_0200dbac;
extern s32 Data_0200dbb0;
extern s32 Data_0200d9a6;   /* default handler (mode not 2/3/4) */
extern s32 Data_0200ccba;   /* handler for mode == 2 */
extern s32 Data_0200d9d2;   /* handler for mode == 4 */
extern s32 Data_0200cce2;   /* handler for mode == 3, parameter != 0 */
extern s32 Data_0200da50;   /* handler for mode == 3, parameter == 0 */
extern void Func_02007e72(void);              /* Func_0808a018 veneer #1 (mode==0 arm) */
extern void Func_02007fc6(void);              /* Func_0808a360 veneer #1 */
extern void Func_02007fda(void);              /* Func_0808a370 veneer #1 */
extern void Func_02007e78(s32 frames);        /* Func_0808a010 veneer #1 */
extern void Func_02008026(s32 cue);           /* Audio_PlayCue veneer #1 */
extern void Func_020062e0(s32 mode);          /* Func_02002e54 veneer #1 */
extern void Func_0200686c(s32 style, s32 variant); /* Func_020033d8 veneer #1 */
extern void Func_02007e92(s32 frames);        /* Func_0808a010 veneer #2 */
extern void Func_02007ea6(void);              /* Func_0808a020 veneer #1 */
extern void Func_02008046(s32 cue);           /* Audio_PlayCue veneer #2 (main arm) */
extern void Func_02007eaa(void);              /* Func_0808a018 veneer #2 */
extern void Func_02007ffe(void);              /* Func_0808a360 veneer #2 */
extern void Func_02008012(void);              /* Func_0808a370 veneer #2 */
extern void Func_02007eba(s32 frames);        /* Func_0808a010 veneer #3 */
extern void Func_0200806a(s32 cue);           /* Audio_PlayCue veneer #3 */
extern void Func_02006324(s32 mode);          /* Func_02002e54 veneer #2 */
extern void Func_020068b0(s32 style, s32 variant); /* Func_020033d8 veneer #2 */
extern void Func_02007ed6(s32 frames);        /* Func_0808a010 veneer #4 */
extern void Func_02007d26(s32 frames);        /* Func_080000c0 veneer (loop body) */
extern s32 Func_02008092(void);               /* Func_080f9048 veneer (loop check) */
extern void Func_02008094(s32 cue);           /* Audio_PlayCue veneer #4 */
extern void Func_0200634e(s32 mode);          /* Func_02002e54 veneer #3 */
extern void Func_020068da(s32 style, s32 variant); /* Func_020033d8 veneer #3 */
extern void Func_020080a8(s32 cue);           /* Audio_PlayCue veneer #5 */
extern void Func_02007f06(s32 frames);        /* Func_0808a010 veneer #5 */
extern void Func_020068ee(s32 style, s32 variant); /* Func_020033d8 veneer #4 */
extern void Func_020080bc(s32 cue);           /* Audio_PlayCue veneer #6 */
extern void Func_02007f1a(s32 frames);        /* Func_0808a010 veneer #6 */
extern void Func_0200637c(s32 mode);          /* Func_02002e54 veneer #4 */
extern void Func_02006908(s32 style, s32 variant); /* Func_020033d8 veneer #5 */
extern void Func_020080d6(s32 cue);           /* Audio_PlayCue veneer #7 */
extern void Func_02007f34(s32 frames);        /* Func_0808a010 veneer #7 */
extern void Func_02006396(s32 mode);          /* Func_02002e54 veneer #5 */
extern void Func_02006922(s32 style, s32 variant); /* Func_020033d8 veneer #6 */
extern void Func_020080f0(s32 cue);           /* Audio_PlayCue veneer #8 */
extern void Func_020080cc(void);              /* Func_0808a4f0 veneer */
extern void Func_02007f60(void);              /* Func_0808a020 veneer #2 */
extern void Func_02007f16(s32 flag);          /* GameFlag_Set veneer */
extern s16 Data_0200dace;
extern u16 Data_0200dc34;
extern u16 Data_0200dbc0;
extern u16 Data_0200db98;
extern u16 Data_0200dbb4;
extern u16 Data_0200dbcc;
extern u16 Data_0200dba0;
extern u16 Data_0200dc40;
extern u16 Data_0200dbe4;
extern u16 Data_0200dbfc;
extern u16 Data_0200db90;
extern u8 Data_0200ce50[];
extern u8 Data_0200ce74[];
extern u8 Data_0200cd80[];
extern u8 *Data_03001f3c;
extern u8 Data_0200bef1[];
extern u32 Data_0200cfc0[];

s32 Func_020070e6(s32);
StageActor *Func_02007198(s32);
void Func_02007186(void);
StageActor *Func_020071a6(s32);
void Func_020072a0(s32);
void Func_020072b8(s32, s32);
void Func_02007210_a();
void Func_02007210_b();
void Func_02007374(void);
void Func_02007380(void);
void Func_02007200(s32, s32);
void Func_0200735a(s32);
void Func_0200720a(s32);
void Func_02004e9a(s32);
void Func_020073ae(void);
void Func_020073c2(void);
void Func_02007274(void);
void Func_0200742a(s32, s32);
void Func_02007434(s32, s32);
void Func_0200743e(s32, s32);
void Func_02007448(s32, s32);
void Func_02007452(s32, s32);
void Func_0200745c(s32, s32);
void Func_0200760c(void);
void Func_0200741c(s32, s32);
void Func_0200757a(s32);
void Func_02007592(s32, s32);
s32 Func_0200748c(s32);
s32 Func_0200749c(s32);
s32 Func_0200747e(s32);
void Func_020074cc(s32);
void Func_020075ca(s32);
s32 Func_020075da(s32, s32);
s32 Func_0200753a(s32, s32);
void Func_020074d2(s32, s32);
void Func_02007632(s32);
void Func_0200764a(s32, s32);
void Func_02007c38(s32 taskAddress, s32 frameBudget);
void Func_020082d4(s32 taskAddress, s32 frameBudget);
void Func_02008330(s32 taskAddress, s32 frameBudget);
void Func_0200b91c(void);
void Func_0200836e(void (*callback)(void));
void Func_020083c8(s32 slot);
ScaledStageObject *Func_020086b0();
void Func_02008464();
void Func_0200844c();
void Func_02008488();
u8 *Func_020086f0(s32 object_id);
void Func_020084a4(void);
void Func_0200848c(u8 *object, s32 mode);
void Func_020084c8(u8 *object, s32 x, s32 y, s32 z);
void Func_020084d6(u8 *object);
void Func_020084a6(u8 *object, s32 enabled);
u32 Func_02008568(void);
s32 Func_02008584(void);
s32 Func_0200858a(void);
void Func_020085a6(s32, s32, s32 *);
StageEffect *Func_0200863a(s32, s32, s32, s32);
void Func_020086b6(StageEffect *, s32);
void Func_0200864e_a(StageEffect *, s32);
void Func_0200864e_b(StageEffect *, s32);
void Func_02008656(StageEffect *, s32);
StageEffect *Func_020087b4(s16);
void Func_020086be(StageEffect *, s32, s32, s32);
void Func_020086a6(StageEffect *, s32);
void Func_02008964(s32);
u8 *Func_02008818_b();
u8 *Func_02008d2e();
s32 Func_02008d48();
u8 *Func_02008f08();
u8 *Func_02008f10();
s32 Func_02008eb0();
void Func_02008dbe();
s32 Func_02008de2();
s32 Func_02008dee();
void Func_02008d8e();
void Func_02008ddc();
void Func_0200c0d0(void);
s32 Func_02008e56(void);
void Func_02008e26(s32, s32);
s32 Func_02008f44(s32);
void Func_02008dfa(s32, s32);
SceneRecord *Func_0200905c();
SceneRecord *Func_02008c56(Position3 *, SceneRecord *);
SceneRecord *Func_02008c80(Position3 *, SceneRecord *);
SceneRecord *Func_02008cac(Position3 *, SceneRecord *);
s32 Func_0200905e(SceneRecord *, Position3 *);
void Func_02008ffc(SceneRecord *, s32);
void Func_02008f62(s32);
void Func_02009042(SceneRecord *, s32, s32, s32);
void Func_02009052(SceneRecord *, s32, s32, s32);
void Func_020092e8(s32);
void Func_02009066(SceneRecord *);
void Func_020092f6(s32);
void Func_0200904c(SceneRecord *, s32);
u8 *Func_020091bc();
void Func_02009046();
s32 *Func_02008dc6();
void Func_02009078();
s32 *Func_02008df8();

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

s32 *Func_02006e64();          /* entity by selector, established */

void Func_02006c06();          /* unestablished */

void Func_02006bee();          /* established (record, mode) */

void Func_02006c2a();          /* established (record, x, y, z) */

void Func_02006c38();          /* unestablished, single argument */

void Func_02006c08();          /* established (record, mode) */

s32 Func_02006f98();           /* Func_080770e0 veneer #1 */

s32 Func_02006fa2();           /* Func_080770e0 veneer #2 */

void Func_02007058();          /* Func_0808a0f0 veneer #1 */

s32 Func_02006fc0();           /* Func_080770e0 veneer #3 */

s32 Func_02006fca();           /* Func_080770e0 veneer #4 */

void Func_0200707c();          /* Func_0808a0f0 veneer #2 */

s32 Func_02006fe4();           /* Func_080770e0 veneer #5 */

s32 Func_02006fee();           /* Func_080770e0 veneer #6 */

void Func_020070a0();          /* Func_0808a0f0 veneer #3 */

u8 *Func_020077bc();           /* veneer, item/party record by id, established */

s32 Func_020077ce();           /* veneer, established (handle, item) */

void Func_020077ea();          /* veneer, established (handle, slot) */

u8 *Func_020082f8();           /* scene-record accessor, established (veneer to Scene_GetRecord) */

void Func_020081e6();          /* unestablished */

s32 Func_0200829e(void);       /* established (veneer to Func_080153b8) */

void Func_02007362();          /* sibling item-28 owner, via per-site veneer */

void Func_02008520();          /* advance the task scheduler, established (veneer to Func_080000c0) */

s32 Func_020087a0();           /* veneer to GameFlag_IsSet */

u8 *Func_02008818_a();         /* veneer to Scene_GetRecord */

void Func_0200876a();          /* veneer to Object_SetPosition */

void Func_02008750();          /* veneer to Object_SetCallback */

void Func_020086b6_a();          /* veneer to Func_080000c0 */

s32 Func_02006ca2();           /* local thunk to Func_020020e8, site A */

s32 Func_02006cb4();           /* local thunk to Func_020020e8, site B */

void Func_0200880a();          /* veneer to UiText_DrawQuantity, site A */

void Func_0200881c();          /* veneer to UiText_DrawQuantity, site B */

void Func_0200882c_a();          /* shared veneer, selector refresh + 0x96a */

void Func_0200882c_b();          /* shared veneer, selector refresh + 0x96a */

void Func_020087ca();          /* veneer to Func_08009148 */

void ColossoLogRollingStage_PositionAndActivateActor(s32 selector, s32 x, s32 z)
{
    s32 *record;

    record = Func_02006e64(selector);
    if (record != 0) {
        Func_02006c06();
        Func_02006bee(record, 5);
        Func_02006c2a(record, x << 16, record[3], z << 16);
        Func_02006c38(record);
        Func_02006c08(record, 1);
    }
}

void ColossoLogRollingStage_RunLogRollingInteraction(s32 actor)
{
    extern s16 Data_02000240[];

    s32 state;

    if (Data_02000240[225] == 2) {
        Func_02004d72();
        return;
    }

    Func_02006eec();
    state = Func_02004f88(actor, 6);

    if (state == 0) {
        Func_02006fb0(0x20c7);
        Func_02007004(0x30000, 0x6000);
        Func_0200701e(0x5080000, -1, 0x980000, 1);
        Func_0200702a();
        Func_02006f20(30);
        Func_02006ff0(actor, 0);
        Func_02005f92(0xb4, 0x58, 0);
        Func_02006f38(60);
        Func_02007008(actor, 0);
        Func_0200600e(0x20, 0x54, 10);
        Func_02006f50(30);
        Func_02007020(actor, 0);
        Func_02006026(0x60, 0x54, 30);
        Func_02006f68(60);
        Func_02007038(actor, 0);
        Func_02006094();
        Func_02006f7a(2);
        Func_0200707a(0, 0);
        Func_020050e2(actor, 6);
    } else if (state == 1) {
        Func_0200704e(0x20c6);
        Func_02007066(actor, 0);
    }

    Func_0200515c(state, actor, 6);
    Func_02006fbc();
}

void ColossoLogRollingStage_RestoreActorPositions(void)
{
    s32 center;

    {
        s32 x = Func_02006f98(896);
        s32 z = Func_02006fa2(904);
        center = 0x80000;
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Func_02007058(1, x, z);
    }
    {
        s32 x = Func_02006fc0(912);
        s32 z = Func_02006fca(920);
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Func_0200707c(2, x, z);
    }
    {
        s32 x = Func_02006fe4(928);
        s32 z = Func_02006fee(936);
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Func_020070a0(3, x, z);
    }
}

void ColossoLogRollingStage_MarkSceneProgress(void)
{
    extern s16 Data_02000240[];

    u8 *state;
    s16 *table;
    s32 slotValue;
    s32 value;
    u16 *field;

    state = *(u8 **)0x03001ebc;
    table = Data_02000240;
    slotValue = *(s32 *)&table[250];
    if (slotValue != 0) {
        if ((s16)*(u16 *)(state + 382) >> 10 == slotValue) {
            if (Func_020070e6(0x141) != 0) {
                field = (u16 *)(state + 386);
                value = 99;
                *field = value;
            }
        }
    }
}

void ColossoLogRollingStage_SelectNearestObstacle(void)
{
    extern s16 Data_02000240[];

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
    table = Data_02000240;
    active_slot = *(s32 *)&table[250];
    target = Func_02007198(active_slot);
    Func_02007186();
    for (slot = 8; slot <= 66; slot++) {
        actor = Func_020071a6(slot);
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
    Func_020072a0(0x2085);
    Func_020072b8(best_slot, 0);
    frame = (s32 *)(state + 448);
    *frame = 0x200;
    *(s32 *)(state + 456) = 15;
    Func_02007210_a(20);
    Func_02007374();
    Func_02007380();
    base = active_slot * 16;
    Func_02007200(base + 880, target->x >> 20);
    z = target->z >> 20;
    Func_02007210_b(base + 888, z);
    active_slot = active_slot + 1;
    if (active_slot > 3) {
        Func_0200735a(10);
        Func_0200720a(282);
    } else {
        Func_02004e9a(active_slot);
        Func_020073ae();
        Func_020073c2();
        *frame = 0;
    }
    Func_02007274();
}

void ColossoLogRollingStage_ClearSavedActorPositions(void)
{
    Func_0200742a(896, 0);
    Func_02007434(904, 0);
    Func_0200743e(912, 0);
    Func_02007448(920, 0);
    Func_02007452(928, 0);
    Func_0200745c(936, 0);
}

s32 ColossoLogRollingStage_RunStateInteraction(s32 actor_handle, s32 interaction_base)
{
    extern s16 Data_02000240[];

    s32 stage_variant;
    s32 script_id;
    s32 result;

    Func_0200760c();
    Func_0200741c(interaction_base, 5);
    stage_variant = Data_02000240[224];
    if (stage_variant == (s32)&Value_0000008f) {
        script_id = (s32)&Value_00002076;
    } else if (stage_variant == (s32)&Value_00000090) {
        script_id = (s32)&Value_00002078;
    } else {
        script_id = (s32)&Value_0000207a;
    }
    Func_0200757a(script_id);
    Func_02007592(actor_handle, 0);
    if (Func_0200748c(interaction_base + 512) != 0) {
        return 2;
    }
    if (Func_0200749c(interaction_base + 520) != 0) {
        result = Func_0200747e(0);
        if (result == 1) {
            return 2;
        }
        if (result == 2 || result == -1) {
            return 3;
        }
        return result;
    }
    Func_020074cc(interaction_base + 520);
    Func_020075ca((s32)&Value_0000207c);
    Func_020075da(actor_handle, 0);
    return Func_0200753a(0, 0);
}

void ColossoLogRollingStage_InitializeStateInteraction(s32 actor_handle, s32 interaction_base)
{
    extern s16 Data_02000240[];

    s32 stage_variant;
    s32 script_id;

    Func_020074d2(interaction_base, 5);
    stage_variant = Data_02000240[224];
    if (stage_variant == (s32)&Value_0000008f) {
        script_id = (s32)&Value_00002076;
    } else if (stage_variant == (s32)&Value_00000090) {
        script_id = (s32)&Value_00002078;
    } else {
        script_id = (s32)&Value_0000207a;
    }
    Func_02007632(script_id + 1);
    Func_0200764a(actor_handle, 0);
}

void ColossoLogRollingStage_ApplyItemToMatchingSlots(s32 handle, s32 item)
{
    u8 *record;
    s32 slot;

    record = Func_020077bc(handle);
    Func_020077ce(handle, item);

    for (slot = 0; slot <= 14; slot++) {
        if (*(u16 *)(record + 216 + slot * 2) == item) {
            Func_020077ea(handle, slot);
        }
    }
}

void ColossoLogRollingStage_InitializeModeTask(u32 mode, u32 parameter)
{
    s32 handler;

    Data_0200dbd0 = (u16)mode;
    Data_0200dba4 = (u16)(parameter << 4);

    {
        s32 budget = 0xc80;
        s32 task = 0x0200aee9;
        Func_02007c38(task, budget);
    }

    handler = (s32)&Data_0200d9a6;
    if (mode == 2) {
        handler = (s32)&Data_0200ccba;
    }
    if (mode == 4) {
        handler = (s32)&Data_0200d9d2;
    }
    if (mode == 3) {
        if (parameter != 0) {
            handler = (s32)&Data_0200cce2;
        } else {
            handler = (s32)&Data_0200da50;
        }
    }

    Data_0200dbdc = 0;
    Data_0200dbe0 = handler;
    Data_0200dc38 = 0;
    Data_0200dbac = 0;
    Data_0200dbb0 = 0;
}

/*
 * resource_3bc scripted transition owner at 0x02003468, 268 bytes including
 * alignment and its three-word pool.  Mode zero is the short opening; every
 * other mode runs the complete multi-stage transition and publishes flag
 * 0x123 when it closes.
 *
 * Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence, including every repeated Func_0808a010,
 * Audio_PlayCue, Func_02002e54, Func_020033d8, Func_0808a018/360/370/020
 * call) -- declared/named as the literal per-site targets, not the shared
 * ultimate-destination symbol.
 */
void ColossoLogRollingStage_RunScriptedTransition(s32 mode)
{
    if (mode == 0) {
        Func_02007e72();
        Func_02007fc6();
        Func_02007fda();
        Func_02007e78(30);
        Func_02008026(0x59);
        Func_020062e0(0);
        Func_0200686c(1, 0);
        Func_02007e92(120);
        Func_02007ea6();
        return;
    }

    Func_02008046(0xf7);
    Func_02007eaa();
    Func_02007ffe();
    Func_02008012();
    {
        s16 *base = (s16 *)0x0200d9a6;
        *(s16 *)((u8 *)base + 30) = (s16)(mode * 60);
    }
    Func_02007eba(30);
    Func_0200806a(mode + 0x5a);
    Func_02006324(mode);
    Func_020068b0(1, 0);
    Func_02007ed6(120);

    goto check_transition;
wait_transition:
    Func_02007d26(1);
check_transition:
    if (Func_02008092() != 0)
        goto wait_transition;

    Func_02008094(0x121);
    Func_0200634e(5);
    Func_020068da(2, 0);
    Func_020080a8(0xec);
    Func_02007f06(60);
    Func_020068ee(2, 1);
    Func_020080bc(0xec);
    Func_02007f1a(60);
    Func_0200637c(6);
    Func_02006908(2, 0);
    Func_020080d6(0xec);
    Func_02007f34(60);
    Func_02006396(7);
    Func_02006922(4, 0);
    Func_020080f0(0xed);
    Func_020080cc();
    Func_02007f60();
    Func_02007f16(0x123);
}

void ColossoLogRollingStage_ResetActorMotion(s32 selector)
{
    u8 *record;

    record = Func_020082f8(selector);
    Func_020081e6();

    *(u32 *)(record + 36) = 0;
    *(u32 *)(record + 44) = 0;
    *(u32 *)(record + 56) = 0x80000000;
    *(u32 *)(record + 64) = 0x80000000;
}

void ColossoLogRollingStage_EnsurePaletteHandle(void)
{
    s16 *cursor = &Data_0200dace;

    if (*cursor == -1) {
        *cursor = Func_0200829e();
    }
}

void ColossoLogRollingStage_StartPaletteTask(u32 first_value, u32 second_value, u32 mode)
{
    Func_02007362(first_value, second_value, mode);

    Data_0200dc34 = (u16)first_value;
    Data_0200dbc0 = (u16)second_value;
    Data_0200db98 = (u16)(mode & 3);
    Data_0200dbb4 = 0;
    Data_0200dbcc = 0;

    {
        s32 budget = 0xc80;
        s32 task = 0x0200b91d;
        Func_020082d4(task, budget);
    }
}

void ColossoLogRollingStage_StartPaletteTaskFromState(u32 first_value, u32 second_value, u32 mode)
{
    Data_0200dba0 = (u16)first_value;
    Data_0200dc40 = (u16)second_value;
    Data_0200dbe4 = Data_0200dc34;
    Data_0200dbfc = Data_0200dbc0;
    Data_0200dbcc = (u16)mode;
    Data_0200db90 = 0;

    {
        s32 budget = 0xc80;
        s32 task = 0x0200b91d;
        Func_02008330(task, budget);
    }
}

void ColossoLogRollingStage_StopPaletteTask(void)
{
    Func_0200836e(Func_0200b91c);
    Func_020083c8(Data_0200dace);
    Data_0200dace = -1;
}

void ColossoLogRollingStage_PositionScaledObject(s32 id, s32 x, s32 z)
{
    ScaledStageObject *object = Func_020086b0(id);
    s32 scale;

    if (object != 0) {
        scale = 0x20000;
        object->scale_x = scale;
        object->scale_z = scale / 2;
        object->state = 0;
        Func_02008464(object);
        Func_0200844c(object, 5);
        Func_02008488(object, x << 16, object->y, z << 16);
    }
}

void ColossoLogRollingStage_SpawnPositionedObject(s32 object_id, s32 x, s32 z)
{
    u8 *object = Func_020086f0(object_id);

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

    Func_020084a4();
    Func_0200848c(object, 5);
    Func_020084c8(object, x << 16, *(s32 *)(object + 12), z << 16);
    Func_020084d6(object);
    Func_020084a6(object, 1);
}

/* Complete eight-byte state setter plus its sole four-byte pool word. */
void ColossoLogRollingStage_SetBalanceStateReady(void)
{
    u16 *p = (u16 *)0x02001000;
    u16 v = 9;
    *p = v;
}

void ColossoLogRollingStage_WaitForBalanceState(void)
{
    extern s16 Data_02001000;

    s16 *status = &Data_02001000;

    while (*status != 9) {
        Func_02008520(1);
    }
}

void ColossoLogRollingStage_SpawnRandomSceneEffect(StageEffect *source)
{
    s32 position[3];
    u32 random_value;

    if (source->vertical_motion >= -255 && source->vertical_motion <= 255) {
        source->state = 0;
    }
    random_value = Func_02008568();
    if (random_value * 100 >> 16 <= 9) {
        StageEffect *effect;
        s32 angle;
        s32 radius;

        position[0] = source->x;
        position[1] = source->y;
        position[2] = source->z;
        angle = Func_02008584();
        radius = Func_0200858a();
        Func_020085a6(angle << 4, radius, position);
        {
            s32 x = position[0];
            s32 y = position[1];
            s32 z = position[2];

            effect = Func_0200863a(285, x, y, z);
        }
        if (effect != 0) {
            effect->state = 0;
            Func_020086b6(effect, 0);
            Func_0200864e_b(effect, (s32)Data_0200ce50);
            Func_0200864e_a(effect, 1);
            Func_02008656(effect, 0);
        }
    }
}

s32 ColossoLogRollingStage_RaiseLinkedSceneEffect(StageEffect_02003d88 *source)
{
    StageEffect_02003d88 *effect = Func_020087b4(source->linked_effect_slot);

    Func_020086be(effect, source->x, source->y + 0x240000, source->z);
    effect->state = 0;
    Func_020086a6(effect, (s32)Data_0200ce74);
    Func_02008964(83);
    source->linked_effect_slot = 0;
    return 0;
}

s32 ColossoLogRollingStage_PositionActiveActor(s32 first_handle, s32 second_handle)
{
    extern u8 Data_02000240[];

    u8 *workspace = *(u8 **)0x03001f3c;
    u8 *shared;
    u8 *record;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = Func_020087a0(0x211);

    shared = Data_02000240;
    record = Func_02008818_a(*(s32 *)(shared + 500));

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

    Func_0200876a(record, x, 0, z);
    Func_02008818_b(0x211);
    Func_02008750(record, (void *)0x0200db24);

    while (*waitp != 0) {
        Func_020086b6_a(1);
    }

    if (flag == 0) {
        Func_02006ca2(0, first_handle);
        Func_0200880a(first_handle, 2);
    } else {
        Func_02006cb4(0, second_handle);
        Func_0200881c(second_handle, 2);
    }

    shared = Data_02000240;
    Func_0200882c_a(*(s32 *)(shared + 500), 1);
    Func_0200882c_b(0x96a, 3);
    Func_020087ca(record);

    return flag;
}

void ColossoLogRollingStage_SetupSceneDescriptor(s32 first_actor, s32 second_actor,
                   s32 mode, s32 centre, s32 extra, s32 third_actor,
                   s32 fourth_actor)
{
    u8 *descriptor;
    u8 *first_record;
    u8 *second_record;
    s32 handle;
    s32 extent;

    descriptor = Func_02008d2e(59, 0x7170);
    handle = Func_02008d48(512);

    *(u16 *)(descriptor + 222) = (u16)first_actor;
    *(u16 *)(descriptor + 224) = (u16)second_actor;
    *(u16 *)(descriptor + 226) = (u16)third_actor;
    *(u16 *)(descriptor + 228) = (u16)fourth_actor;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = Func_02008f08(first_actor);
    second_record = Func_02008f10(second_actor);

    if (Func_02008eb0(0x109) == 0) {
        *(s32 *)(second_record + 8) =
            (centre << 1) - *(s32 *)(first_record + 8);
        *(s32 *)(second_record + 16) = *(s32 *)(first_record + 16);
    }

    *(u16 *)(descriptor + 218) = 0;
    *(u16 *)(descriptor + 220) = 0;

    Func_02008dbe(Data_0200cd80, handle);

    extent = Func_02008de2();
    *(u16 *)(descriptor + 216) = (u16)extent;
    Func_02008dee((s16)extent, 512, handle);

    Func_02008d8e((s32)Func_0200c0d0 + 1, 0xc76);

    Func_02008ddc(handle);
}

void ColossoLogRollingStage_InitializeSceneControl(void)
{
    extern SceneControl Data_02001000;

    u8 *scene_state = Data_03001f3c;
    SceneControl *control = &Data_02001000;

    Func_02008e26(Func_02008e56(), (s32)(scene_state + 240));
    if (Func_02008f44(0x109) == 0) {
        control->enabled = 1;
        control->active = 1;
        control->scene_variant = *(u16 *)(scene_state + 224);
        control->timer = 0;
        control->phase = 0;
    }
    {
        s32 event_id = 0xc85;

        Func_02008dfa((s32)Data_0200bef1, event_id);
    }
}

void ColossoLogRollingStage_SetSceneControlValue(u16 value)
{
    u8 *workspace = *(u8 **)0x03001f3c;
    *(u16 *)(workspace + 220) = value;
}

void ColossoLogRollingStage_PushStagedActor(void)
{
    extern s16 Data_02000240[];

    SceneRecord *subject;
    SceneRecord *target;
    SceneRecord *blocker;
    u32 step;
    u32 direction;
    Position3 position;
    u32 data_index = 250;
    s32 zero;
    s32 subject_handle;

    subject_handle = *(s32 *)((u8 *)Data_02000240 + (data_index << 1));
    subject = Func_0200905c(subject_handle);

    direction = subject->facing >> 12;

    step = Data_0200cfc0[direction];
    position.x = subject->x + (s32)(step & 0xffff0000);
    position.y = subject->y;
    step <<= 16;
    position.z = subject->z + (s32)step;

    target = Func_02008c56(&position, subject);
    if (target == 0) {
        return;
    }

    /* Is the cell one step beyond the target already taken? */
    step = Data_0200cfc0[direction];
    position.x = target->x + (s32)(step & 0xffff0000);
    position.y = target->y;
    step <<= 16;
    position.z = target->z + (s32)step;

    blocker = Func_02008c80(&position, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    /* ...and the cell directly above the target? */
    position.x = target->x;
    position.y = target->y + 0x100000;      /* 128 << 13 */
    position.z = target->z;

    blocker = Func_02008cac(&position, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    target->state = 2;
    zero = 0;

    step = Data_0200cfc0[direction];
    position.x = target->x + (s32)(step & 0xffff0000);
    position.y = target->y;
    step <<= 16;
    position.z = target->z + (s32)step;

    if (Func_0200905e(target, &position) > 0) {
        return;
    }

    Func_02008ffc(subject, 8);
    Func_02008f62(15);

    target->rate_x = 0x3333;
    target->rate_z = 0x3333;
    Func_02009042(target, position.x, position.y, position.z);

    subject->rate_x = 0x3333;
    subject->rate_z = 0x3333;
    Func_02009052(subject, position.x, position.y, position.z);

    Func_020092e8(0xee);
    Func_02009066(target);
    Func_020092f6(0x120);                                /* 144 << 1 */

    target->x = position.x;
    target->z = position.z;
    target->motion_24 = zero;
    target->motion_2c = zero;

    Func_0200904c(subject, 1);
}

/* This overlay's own byte-exact occupancy lookup. */
s32 *ColossoLogRollingStage_FindActorAhead(void)
{
    extern s16 Data_02000240[];

    u8 *record;
    s32 facing;
    s32 position[3];
    s32 *occupant;

    record = Func_020091bc(((ActiveSubjectSlot *)Data_02000240)->handle);

    /* 128 << 6 = 0x2000 bias, then masked to bits 14-15 (192 << 8). */
    facing = (*(u16 *)(record + 6) + 0x2000) & 0xc000;

    position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
    position[1] = *(s32 *)(record + 12);
    position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
    Func_02009046(0x100000, facing, position);          /* 128 << 13 */

    occupant = Func_02008dc6(position, record);
    if (occupant == 0) {
        position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
        position[1] = *(s32 *)(record + 12);
        position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
        Func_02009078(0x200000, facing, position);      /* 128 << 14 */

        occupant = Func_02008df8(position, record);
    }

    return occupant;
}
