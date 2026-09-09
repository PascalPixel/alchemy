/*
 * Battle entry point.  Allocates the encounter work blocks, plays the opening
 * presentation, then runs the round loop until the encounter resolves and
 * returns the outcome word.
 *
 * Shape recovered from the reference at 0x080b63c8 (1688 bytes):
 *
 *   - Eight tagged blocks come from the bump allocator Func_080048f4.  Four
 *     of the pointers are kept (76-byte scene, 0x82c-byte encounter work,
 *     0x7c8-byte transfer buffer, 32-byte timer) and a fifth 12-byte block is
 *     kept only long enough to be cleared; the 0x280, 0xe00 and 0x600 blocks
 *     are allocated for their side effect alone.
 *   - The 76-byte scene block, the 0x82c-byte encounter work block and the
 *     12-byte block are cleared with three DMA3 fixed-source 32-bit fills
 *     whose word counts are exactly the block sizes.
 *   - The round loop re-arms the frame callback at 0x080b7739, clears the
 *     twenty sixteen-byte action slots at +0x2ec of the work block, rebuilds
 *     them, and replays them one at a time.
 *   - Five exits with distinct return values: the outcome word at +0x538 after
 *     a normal or aborted round, -1 when the player side has no living unit
 *     left, and 0x3e7 when a replayed entry reports 1.
 *
 * VENEER NOTE.  0x080072f0 is not a function: it is the GCC `__call_via_r3`
 * entry of the veneer bank that starts at 0x080072e4.  Both `bl 0x080072f0`
 * sites in this owner are indirect calls through r3 to the relocated IWRAM
 * routine at 0x03000164, and the pool load that reads like dead code is the
 * callee load.  This owner reaches it with two arguments - destination and
 * byte count - so it is spelled here as a typed indirect call, which is the
 * form the boundary rules ask for.  The register the compiler picks, and
 * therefore the veneer entry, is a byte-exact question this draft does not
 * settle; it happens to pick r3 here.
 *
 * DMA3 SPELLING.  Each fill is three ordinary field stores through the shared
 * `struct DmaChannel` of video_dma_family.h.  The reference emits them as one
 * `stmia r3!, {r0, r1, r2}` plus a compensating `subs r3, #12`; this route
 * cannot.  That is the already-investigated DMA3 "lone stmia" residual
 * recorded on Func_08004838 (08004838.c): the approved Thumb backend has no
 * pattern or peephole that groups three independently computed registers into
 * one store-multiple, so no ordinary source spelling reproduces it.  An
 * earlier revision of this draft walked a `u32 *` with post-increment and a
 * compensating `-= 3` to chase the writeback form; that was transcribed
 * assembly, it did not produce the grouped store either, and its claim that
 * struct-field stores broke topology equality is wrong - measured here, the
 * struct form keeps topology=equal and lowers differing halfwords from 790 to
 * 771.  Those three unreproducible foldings are most of the 20-byte shortfall.
 *
 * RESIDUAL (measured, not guessed).  candidate 1668 bytes against reference
 * 1688 (98.8%), topology equal, 360 wrong instructions, 771 differing
 * halfwords.  Every reference `bl` agrees in target and position, and every
 * substituted instruction carries the same mnemonic as its reference
 * counterpart - the differences are operands only.  Control flow, call
 * arguments, access widths, signedness and the pool constants agree.  What is
 * left is register allocation and address-carrier choice: the reference holds
 * the work block in r8 and the acting unit id in sl where this draft holds
 * them the other way round, its return value sits in r7 rather than r5, and it
 * keeps `&work->field_41`, `&work->field_44` and `&work->field_45` alive in
 * r9/[sp,#4]/fp where this draft re-derives them from plain member access.
 * The reference also reserves 68 bytes of stack where this draft needs 20; 48
 * of those bytes are never touched by any reference instruction, so what the
 * original source declared there is not recoverable from the reference alone.
 *
 * UNCERTAIN.  The flag ids passed to Func_080770c0 (0x16a, 0x16c, 0x16e) and
 * the cue and string ids stay numeric because nothing in the corpus names
 * them.  Func_08003f3c and Func_080b5b14 keep address spellings because the
 * project's labels for them (`reset_entry`, `reserved_no_op`) are shared
 * generic names, not unique external symbols.  Work-block fields keep
 * offset-based names except where a neighbouring recovered owner already fixes
 * the meaning: +0x2ec is the action slot array that
 * BattlePresentation_RunAction consumes as `s16 *`, and halfword 0 of a slot
 * is the acting unit id.  `actions[36]` spans +0x2ec to the next evidenced
 * field; only the first twenty slots are ever cleared or replayed here.
 */

#include "video_dma_family.h"

struct BattleTimerWork {
    s32 value;                  /* 0x00 */
    s32 frames;                 /* 0x04 */
    u8 unk_08[12];              /* 0x08 */
    s32 field_14;               /* 0x14 */
    s32 field_18;               /* 0x18 */
    s32 field_1c;               /* 0x1c */
};

struct BattleSceneVector {
    s32 field_00;
    s32 field_04;
    s32 field_08;
};

struct BattleSceneWork {
    s32 field_00;               /* 0x00 */
    s32 field_04;               /* 0x04 */
    s32 field_08;               /* 0x08 */
    struct BattleSceneVector sub_0c; /* 0x0c */
    u8 unk_18[8];               /* 0x18 */
    s32 field_20;               /* 0x20 */
    u8 unk_24[16];              /* 0x24 */
    u16 field_34;               /* 0x34 */
    u16 field_36;               /* 0x36 */
    u8 unk_38[20];              /* 0x38 .. 0x4b */
};

/* One replayed presentation entry; halfword 0 is the acting unit id. */
struct BattleActionSlot {
    s16 h[8];
};

struct BattleEncounterWork {
    s32 field_00;                       /* 0x000 */
    u8 unk_04[12];                      /* 0x004 */
    u16 msg_ids[22];                    /* 0x010 */
    u16 field_3c;                       /* 0x03c */
    u16 field_3e;                       /* 0x03e */
    u8 unk_40;                          /* 0x040 */
    u8 field_41;                        /* 0x041 */
    u8 field_42;                        /* 0x042 */
    u8 unk_43;                          /* 0x043 */
    u8 field_44;                        /* 0x044 */
    u8 field_45;                        /* 0x045 */
    u8 unk_46[10];                      /* 0x046 */
    u8 field_50;                        /* 0x050 */
    u8 unk_51;                          /* 0x051 */
    u8 field_52;                        /* 0x052 */
    u8 unk_53;                          /* 0x053 */
    s32 field_54;                       /* 0x054 */
    u8 unk_58[0x294];                   /* 0x058 */
    struct BattleActionSlot actions[36]; /* 0x2ec */
    u8 unk_52c[12];                     /* 0x52c */
    s32 field_538;                      /* 0x538 */
    u8 unk_53c[0x10c];                  /* 0x53c */
    u16 field_648;                      /* 0x648 */
    u8 unk_64a[0x1e2];                  /* 0x64a .. 0x82b */
};

/* DMA3 fixed-source 32-bit fill of one freshly allocated block. */
#define DMA3_REGISTERS ((struct DmaChannel *)0x040000d4)
#define DMA_FILL32 0x85000000

#define Dma3Fill(dma, cell, dest, bytes)                                      \
    ((cell) = 0, (dma)->source = &(cell), (dma)->destination = (dest),        \
     (dma)->control = DMA_FILL32 | ((bytes) >> 2))

extern u8 Data_02000240[];

/* The relocated IWRAM block clear this owner reaches through __call_via_r3. */
typedef void (*BlockClearProc)(void *dst, s32 len);

#define IWRAM_BLOCK_CLEAR ((BlockClearProc)0x03000164)

void *Func_080048f4(s32 tag, s32 size);
void Scheduler_ResetTaskTable(void);
void Render_ResetTransformState(void);
void Func_080770c8(s32 id);
s32 Func_0808a4a0(void);
void Func_08009078(s32 mode);
s32 Func_080770c0(s32 flag);
void Func_08015008(s32 mode);
s32 Func_080c1ffc(s32 value);
void Func_080030f8(s32 frames);
s32 Func_080770e0(s32 id);
void BattleParty_AssignMemberSlots(void);
void Scheduler_AddOrUpdateCallback(s32 callback, s32 order);
void Func_080f9010(s32 cue);
void Func_080037d4(s32 value);
void BattleParty_CollectUnitList(void);
void BattleUnit_RefreshPlacement(void);
void BattlePlacement_UpdateEntries(void);
void BattleSummon_UpdateAvailability(void);
s32 *Func_08077000(s32 index);
void Func_08015128(s32 value);
void Camera_InitDefaultTransform(void);
void BattleActor_CommitPlacement(void);
void Func_080c08a8(void);
void Func_080c08ec(s32 a, s32 b, s32 c);
void Func_080c0a24(s32 a, s32 b, s32 c, s32 d, s32 e);
void BattlePresentation_SetupTransitionScene(s32 a, s32 b, s32 c, s32 d);
void Func_080b5b14(s32 value);
void Summon_ClearWorkFields(void);
s32 Resource_LoadIntoFreeSlot(s32 size);
s32 Func_080771a0(void);
void Func_080c02a4(s32 object, s32 arg);
void BattleRuntime_ReservedNoOp9B2C(void);
void Func_08015130(s32 mode);
void Func_08003f3c(s32 entry);
void Runtime_GetRemainingIwram(void);
void Runtime_GetRemainingEwram(void);
s32 BattlePresentation_BuildActions(struct BattleActionSlot *slots);
s32 BattlePresentation_BuildSortedUnitEntries(struct BattleActionSlot *slots);
s32 BattlePresentation_DispatchAction(struct BattleActionSlot *slot, s32 delay);
s32 BattlePresentation_RunAction(struct BattleActionSlot *slot);
s32 BattleParty_ListLivingUnits(s32 side, s32 mode);
s32 Func_080b6148(void);
void BattleRuntime_ReservedNoOpF674(void);
void Func_080bf678(void);
void BattleMotion_DestroyAllSlotObjects(void);
s32 Func_08015038(s32 id, s32 a, s32 b, s32 c);
s32 Func_08015048(void);
void Func_08015018(s32 handle, s32 mode);
void Func_080bb7c0(s32 a, s32 b);
void Battle_ApplyValueToWork2224(void);
void Func_08077140(s32 a, s32 b, s32 c);
void Func_08015118(void);
void Func_08015120(s32 a, s32 b);
void Func_080151c8(s32 id);
void BattlePresentation_WaitForAdvance(void);
void Func_080c2724(void);
void Blend_SetDarkenTarget16(s32 value);
void Blend_WaitForTransition(void);
void Scheduler_EnableCallbacks(s32 value);
s32 BattleParty_PrepareActiveOwners(s32 side);
void Func_080b5b18(void);
void BattlePlacement_UpdateTimedEntries(void);
void Scheduler_RemoveCallback(s32 callback);
void Runtime_ReleaseHeapBlock10(void);

#define Battle_RunEncounter Func_080b63c8

s32 Battle_RunEncounter(s32 arg)
{
    struct BattleSceneWork *scene;
    struct BattleSceneVector *cam;
    struct BattleEncounterWork *work;
    struct BattleTimerWork *timer;
    struct DmaChannel *dma;
    u8 *buf;
    u8 *src;
    u8 *dst;
    s32 object;
    s32 cnt;
    s32 i;
    s32 delay;
    s32 handle;
    s32 ret;
    s32 wait;
    u32 pos;
    s32 off;
    u32 actor;
    s16 cue;
    u32 fill;

    scene = (struct BattleSceneWork *)Func_080048f4(12, 76);
    work = (struct BattleEncounterWork *)Func_080048f4(9, 0x82c);
    buf = (u8 *)Func_080048f4(54, 0x7c8);
    timer = (struct BattleTimerWork *)Func_080048f4(44, 32);
    Func_080048f4(11, 0x280);
    cam = &scene->sub_0c;
    /* __call_via_r3 -> the IWRAM block clear at 0x03000164. */
    IWRAM_BLOCK_CLEAR(buf, 0x7c8);
    Scheduler_ResetTaskTable();
    timer->frames = 0;
    timer->value = 0x2000;
    timer->field_14 = 1;
    timer->field_18 = 0;
    timer->field_1c = 0;
    *(u16 *)0x04000000 = 1;
    Func_080770c8(0x103);
    Func_080770c8(0x169);
    Render_ResetTransformState();

    dma = DMA3_REGISTERS;
    Dma3Fill(dma, fill, scene, 76);
    Dma3Fill(dma, fill, work, 0x82c);
    work->field_54 = -1;
    work->field_00 = arg;
    dst = (u8 *)Func_080048f4(37, 12);
    Dma3Fill(dma, fill, dst, 12);
    work->field_648 = (u16)Func_0808a4a0();
    Func_080048f4(4, 0xe00);
    Func_080048f4(3, 0x600);
    Func_08009078(4);
    if (Func_080770c0(0x16e) != 0)
        Func_08015008(1);
    else
        Func_08015008(0);

    cam->field_04 = 0x400000;
    cam->field_00 = 0;
    cam->field_08 = 0;
    scene->field_04 = 0xb40000;
    scene->field_08 = 0x400000;
    scene->field_00 = 0;
    scene->field_36 = 0x2800;
    scene->field_34 = 0x5000;
    scene->field_20 = 0x1000000;
    object = Func_080c1ffc(work->field_00);

    if (Func_080770c0(0x16c) != 0) {
        work->field_44 = 1;
        Data_02000240[0x22b] = 4;
    }
    if (work->field_44 != 0) {
        *(s32 *)0x020023a8 = 0;
        wait = 0;
        do {
            if ((*(u16 *)0x03001f64 & 3) == 3)
                goto linked;
            wait++;
            Func_080030f8(1);
        } while (wait <= 24);
        work->field_52 = 1;
    linked:
        work->field_50 = (u8)((*(u32 *)0x04000128 << 26) >> 30);
        src = (u8 *)0x02018000;
        dst = *(u8 **)0x03001f28;
        pos = 0;
        do {
            pos++;
            *dst = *src;
            src++;
            dst++;
        } while (pos <= 0x7c7);
        object = Func_080770e0(0x3f0);
        BattleParty_AssignMemberSlots();
        work->field_42 = 0;
    }
    Scheduler_AddOrUpdateCallback(0x080b5865, 0xc7f);

    {
        off = 494;
        cue = *(s16 *)(Data_02000240 + off);
    }
    if (cue != 0) {
        Func_080f9010(cue);
        if (Func_080770c0(0x16c) != 0) {
            Func_080f9010(55);
            Func_080037d4(4);
        }
    } else {
        Func_080f9010(51);
        Func_080f9010(76);
    }

    BattleParty_CollectUnitList();
    BattleUnit_RefreshPlacement();
    BattlePlacement_UpdateEntries();
    BattleSummon_UpdateAvailability();
    if (*Func_08077000(0) != 0)
        work->field_41 = 3;
    else
        work->field_41 = 1;
    Func_08015128(9);
    Camera_InitDefaultTransform();
    BattleActor_CommitPlacement();
    Func_080c08a8();
    Func_080c08ec(1, work->field_648, 0);
    Func_080c0a24(0xa00000, 0x500000, 0, 0, 0x20000);
    BattlePresentation_SetupTransitionScene(0, 0, 0, 190);
    Func_080b5b14(1);
    *(u16 *)0x04000050 = 0;
    Summon_ClearWorkFields();
    work->field_54 = Resource_LoadIntoFreeSlot(128);
    work->field_45 = 0;
    if (Func_080770c0(0x16e) != 0) {
        work->field_45 = 1;
    } else if (Data_02000240[0x22b] == 0) {
        if ((Func_080771a0() & 15) == 0)
            work->field_45 = 1;
        else if ((Func_080771a0() & 31) == 0)
            work->field_45 = 2;
    }
    Func_080c02a4(object, arg);
    timer->field_14 = 0;
    *(u8 *)0x03001f58 = 0;
    Scheduler_AddOrUpdateCallback(0x080b7739, 0xc80);

    for (;;) {
        BattleRuntime_ReservedNoOp9B2C();
        BattleSummon_UpdateAvailability();
        if (*Func_08077000(0) != 0)
            work->field_41 = 3;
        else
            work->field_41 = 1;
        timer->value = 0x2800;
        timer->frames = 60;
        Func_08015130(work->field_41);
        /* __call_via_r3: clear the twenty action slots. */
        IWRAM_BLOCK_CLEAR(work->actions, 0x140);
        Func_08003f3c(work->field_54);
        if (Func_080770c0(0x16a) == 0) {
            Runtime_GetRemainingIwram();
            Runtime_GetRemainingEwram();
            cnt = BattlePresentation_BuildActions(work->actions);
            Runtime_GetRemainingIwram();
            Runtime_GetRemainingEwram();
        } else {
            cnt = BattlePresentation_BuildSortedUnitEntries(work->actions);
        }
        work->field_54 = Resource_LoadIntoFreeSlot(128);
        Func_08015130(work->field_41);
        if (cnt < 0)
            goto aborted;

        for (i = 0; i < cnt; i++) {
            actor = work->actions[i].h[0];
            Runtime_GetRemainingIwram();
            Runtime_GetRemainingEwram();
            if (Func_080770c0(0x16a) == 0) {
                delay = 10;
                if (i != 0)
                    delay = 0;
                if (BattlePresentation_DispatchAction(&work->actions[i], delay) == 1)
                    goto interrupted;
            } else {
                if (BattlePresentation_RunAction(&work->actions[i]) == 1)
                    goto interrupted;
            }
            Runtime_GetRemainingIwram();
            Runtime_GetRemainingEwram();
            if (BattleParty_ListLivingUnits(1, 0) == 0)
                goto party_lost;
            if (BattleParty_ListLivingUnits(2, 0) == 0) {
                if (actor <= 7 && work->field_538 == 1)
                    work->field_3e = 3;
                goto resolved;
            }
            if (Func_080b6148() < 0)
                goto aborted;
        }

        work->field_45 = 0;
        BattleRuntime_ReservedNoOpF674();
        Func_080bf678();
        BattleMotion_DestroyAllSlotObjects();
        if (work->field_44 != 0) {
            if (Func_080b6148() < 0)
                goto aborted;
        } else {
            Func_080030f8(20);
        }
        if (Func_080770c0(0x16e) == 0)
            continue;

        handle = Func_08015038(0xc47, 0, 4, 1);
        while (Func_08015048() == 0)
            Func_080030f8(1);
        Func_08015018(handle, 1);
        Func_080030f8(1);
        handle = Func_08015038(0xc48, 10, 4, 1);
        Func_080bb7c0(92, 24);
        Func_08015018(handle, 1);
        Func_080030f8(1);
    }

resolved:
    Battle_ApplyValueToWork2224();
    if (Func_080770c0(0x16e) == 0) {
        if (work->field_44 != 0)
            Func_080f9010(58);
        if (work->field_538 != 0) {
            Func_080f9010(58);
            if (work->field_3e <= 1) {
                Func_08077140(128, work->msg_ids[work->field_3c], 26);
                Func_08015118();
                Func_08015120(128, 1);
                Func_080151c8(work->field_3e + 0x838);
                BattlePresentation_WaitForAdvance();
            }
        }
        Func_080c2724();
    }
    Func_080f9010(17);
    Blend_SetDarkenTarget16(30);
    Blend_WaitForTransition();
    ret = work->field_538;
    goto finished;

aborted:
    Battle_ApplyValueToWork2224();
    Scheduler_EnableCallbacks(0);
    *(u16 *)0x04000000 = 1;
    ret = work->field_538;
    Func_080770c8(0x3e8);
    goto finished;

party_lost:
    Battle_ApplyValueToWork2224();
    Func_080f9010(59);
    Func_08015118();
    off = 504;
    src = Data_02000240 + off;
    Func_08015120(*src, 1);
    if (BattleParty_PrepareActiveOwners(0) == 1)
        Func_080151c8(0x83d);
    else
        Func_080151c8(0x837);
    BattlePresentation_WaitForAdvance();
    Func_080f9010(17);
    Blend_SetDarkenTarget16(30);
    ret = -1;
    Blend_WaitForTransition();
    goto finished;

interrupted:
    Func_080f9010(17);
    Blend_SetDarkenTarget16(30);
    Blend_WaitForTransition();
    ret = 0x3e7;

finished:
    Func_080b5b18();
    BattleRuntime_ReservedNoOpF674();
    BattlePlacement_UpdateTimedEntries();
    Data_02000240[0x22b] = 0;
    Scheduler_RemoveCallback(0x080b7739);
    Runtime_ReleaseHeapBlock10();
    return ret;
}
