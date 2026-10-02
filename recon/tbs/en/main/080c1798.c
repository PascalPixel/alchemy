/* BattleFx_PlayUnitElementEffect: show one of four effects on a unit. Mode 0
   is the star field, faded in over 25 frames and followed for 45; mode 1 the
   prepared scene, followed for 40; modes 2 and others launch an effect with
   an argument block built here.

   2026-10-01 slice-6: rewritten from the listing, 49 instructions off (was
   212): 31 register choices, 15 orderings, one instruction short. The
   frame, every callee-saved register, the entry and both loops agree in
   shape. What made them agree:
   - one pointer variable holds the gCameraWork slot address at entry (the
     session is read 12 bytes below it) and the IME address in mode 0. It
     is assigned the IME address at the top of the mode 0 branch (the
     scheduler sinks the load to the loop), which makes it live long enough
     to rank below the hoisted position address: r7 and r6 as in the ROM.
   - the brightness pointer is taken before the loop and spilled.
   - the default launch stores kind first: kind then dies three
     instructions earlier and outranks the hoisted constant 64 for r11.
     Fragile: the two differ by 0.002.
   Remaining:
   - mode 0 loop: the reference reads the work pointer as gWorkSlot plus
     156 (base loaded, offset added) and rebuilds 64 in r4; this draft
     loads the one address of slot 39 and rebuilds 64 in r1, which rotates
     the temporaries r0 to r4 through the loop body. The loop pass hoists
     both the address and the 64 here and seemingly neither there.
   - both preheaders: the same loads in other temporaries and order.
   - the unused 40 bytes at the top of the frame are a guess.
   Pinning the pointer to r7 miscompiles (the allocator reuses r7).
   SparkWork's last three words are the BG2 origin and its pending flag;
   EFFECT3.C names them padding13c4 and unknown_13cc. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"
#include "IO_WRITE_QUEUE.H"
#include "EFFECT_STEP.H"
#include "BATTLE_WORK.H"

struct Spark {
    s32 pos[6];
    s32 life;
};

struct Ring {
    s32 x;
    s32 y;
    s32 vel_x;
    s32 vel_y;
    s32 age;
};

struct SparkWork {
    u8 padding0000[0x11c0];
    struct Spark sparks[16];
    struct Ring rings[3];
    s32 frames;
    s32 ready;
    s32 origin_x;
    s32 origin_y;
    s32 origin_pending;
};

struct State {
    u8 unknown[156];
    struct SparkWork *context;
    u32 source;
};

/* The argument block the effect launchers take. */
struct EffectLaunch {
    s32 kind;
    s32 side;
    s32 actor;
    s32 target;
    s32 unknown_0010;
    s32 count;
    s32 variant;
    s32 unknown_001c;
    s32 unknown_0020;
    s16 actors[24];
};

extern struct State gWorkSlot;
extern u8 gCameraWork[];
extern struct SparkWork *gBattleFxWork[2];

void BattlePresentation_ConfigurePaletteFade(s32 mode, u16 value, s32 fade);
s32 BattleFx_InitializeStarField(s32 mode);
void Graphics_ResetVramBlockAndReleaseHeapBlocks(s32);
s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale, s32 count);
void BattleMotion_ProjectScaledPosition(s32 unit, struct EffectPosition *position);
void Func_080c9020(struct EffectLaunch *launch);
void Func_080c9030(struct EffectLaunch *launch);
void BattlePresentation_PrepareSceneFar(s32);
void BattleFx_ScheduleCallbacksAndReleaseBlocksFar(void);

#define Io_Write16(reg, value) \
    do { \
        s32 value_ = (value); \
        (reg) = value_; \
    } while (0)

/* Queue the two-word BG2 origin transfer for the next frame, with IME off. */
#define QUEUE_BG2_ORIGIN(origin) \
    { \
        volatile u16 *ime; \
        struct IoWriteQueue *q; \
        u32 saved; \
        s32 count; \
 \
        q = &gIoWriteQueue; \
        do { \
            ime = (volatile u16 *)0x04000208; \
            saved = *ime; \
        } while (0); \
        *ime = (u16)ime; \
        count = q->count; \
        if (count <= 31) { \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4); \
            *(u16 *)&q->count = count + 1; \
            *destination++ = (u32)(origin); \
            *destination++ = 0x04000028; \
            *destination = 0x84000002; \
        } \
        *ime = saved; \
    }

#define QUEUE_BG2_ORIGIN_AT(ime, origin) \
    { \
        struct IoWriteQueue *q; \
        u32 saved; \
        s32 count; \
 \
        q = &gIoWriteQueue; \
        do { \
            saved = *ime; \
        } while (0); \
        *ime = (u16)(u32)ime; \
        count = q->count; \
        if (count <= 31) { \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4); \
            *(u16 *)&q->count = count + 1; \
            *destination++ = (u32)(origin); \
            *destination++ = 0x04000028; \
            *destination = 0x84000002; \
        } \
        do { \
            *ime = saved; \
        } while (0); \
    }

void BattleFx_PlayUnitElementEffect(s32 unit, s32 kind, s32 mode, s32 variant)
{
    u8 unused[40];
    struct EffectPosition position;
    struct EffectPosition position2;
    struct EffectLaunch launch;
    struct EffectLaunch launch2;
    volatile u16 *reg = (volatile u16 *)gCameraWork;
    struct BattleSession *session = *(struct BattleSession **)((u8 *)reg - 12);
    struct SparkWork *work;
    s32 frame;
    s32 fade;
    s32 *brightness;

    WaitFrames(1);
    BattlePresentation_ConfigurePaletteFade(1, session->background, 0);
    Iwram_ClearWords((void *)0x06004000, 0x4000);
    QueueIoWriteDelay2(0x04000000, 0x3741);
    QueueIoWriteDelay2(0x0400000c, 0x784);
    QueueIoWriteDelay2(0x04000050, 0x3f44);
    WaitFrames(1);
    Io_Write16(*(u16 *)0x04000040, 240);
    Io_Write16(*(u16 *)0x04000044, 0x1088);
    Io_Write16(*(u16 *)0x04000048, 63);
    Io_Write16(*(u16 *)0x0400004a, 17);
    if (mode == 0) {
        reg = (volatile u16 *)0x04000208;
        QueueIoWriteDelay2(0x04000052, 0x100e);
        BattleFx_InitializeStarField(kind);
        for (frame = 0; frame <= 44; frame++) {
            work = gWorkSlot.context;
            if (frame <= 24)
                Graphics_ScaleRgb555Clamped(session->palette, (u16 *)0x050000c0, session->brightness = 0x10000 - frame * 1092, 128);
            BattleMotion_ProjectScaledPosition(unit, &position);
            work->origin_x = (64 - position.x) << 8;
            work->origin_y = (64 - position.y) << 8;
            QUEUE_BG2_ORIGIN_AT(reg, &work->origin_x);
            work->origin_pending = 1;
            WaitFrames(1);
        }
        Graphics_ResetVramBlockAndReleaseHeapBlocks(kind);
    } else if (mode == 1) {
        BattlePresentation_PrepareSceneFar(kind);
        for (frame = 0; frame < 40; frame++) {
            work = gWorkSlot.context;
            BattleMotion_ProjectScaledPosition(unit, &position2);
            work->origin_x = (64 - position2.x) << 8;
            work->origin_y = (64 - position2.y) << 8;
            QUEUE_BG2_ORIGIN(&work->origin_x);
            work->origin_pending = 1;
            WaitFrames(1);
        }
        BattleFx_ScheduleCallbacksAndReleaseBlocksFar();
    } else if (mode == 2) {
        launch.unknown_001c = 0;
        launch.kind = kind;
        launch.variant = variant;
        launch.actor = unit;
        launch.actors[0] = unit;
        launch.target = unit;
        launch.count = 1;
        launch.unknown_0010 = 1;
        Func_080c9020(&launch);
    } else {
        launch2.kind = kind;
        launch2.unknown_001c = 0;
        launch2.variant = 0;
        launch2.actor = unit;
        launch2.actors[0] = unit;
        launch2.target = unit;
        launch2.count = 1;
        launch2.unknown_0010 = 1;
        Func_080c9030(&launch2);
    }
}
