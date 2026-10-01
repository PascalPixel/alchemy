/* BattleFx_PlayUnitElementEffect: show one of four effects on a unit. Mode 0
   is the star field, faded in over 25 frames and followed for 45; mode 1 the
   prepared scene, followed for 40; modes 2 and others launch an effect with
   an argument block built here.

   2026-10-01 slice-6: rewritten from the listing, 64 instructions off (was
   212). The 240-byte frame, the spilled session and brightness pointer, the
   register-write macro and both loops' bodies agree in shape. Remaining:
   - entry: the reference loads slot 12's address (gCameraWork) into r7 and
     reads the session 12 bytes below it; this draft reads gBattleWork
     directly. A cursor variable used once is folded to the one address, so
     the cursor must have another use this draft does not have.
   - mode 0 loop: the reference keeps the IME address in r7 and the position
     in r6 across the loop, saved IME in r4, the origin address in r0; here
     the IME address is rebuilt every frame (r0), the position is in r7 and
     saved IME in r6. The work pointer is read as gWorkSlot plus 156 (base
     reloaded, offset added) where this draft folds it to slot 39's address.
   - mode 1 loop: the registers agree; the three preheader moves and one
     temporary differ.
   Tried: one pointer variable set to the gCameraWork symbol at entry (the
     session read 12 bytes below it) and to the IME address before the mode
     0 loop, the loop then using it without reloading. That gives the
     reference shape (shared register at entry and in the loop, saved IME
     in r4, origin address in r0) but the allocator ranks it just above the
     hoisted position address (11 references over 118 against 7 over 54), so
     it takes r6 and the position r7, the reverse of the reference, and the
     hoisted 64 then outranks kind, which spills. Pinning that pointer to r7
     miscompiles (the allocator reuses r7 for the work pointer).
   - the unused 40 bytes at the top of the frame are a guess.
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

void BattleFx_PlayUnitElementEffect(s32 unit, s32 kind, s32 mode, s32 variant)
{
    u8 unused[40];
    struct EffectPosition position;
    struct EffectPosition position2;
    struct EffectLaunch launch;
    struct EffectLaunch launch2;
    struct BattleSession *session = gBattleWork;
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
        QueueIoWriteDelay2(0x04000052, 0x100e);
        BattleFx_InitializeStarField(kind);
        brightness = &session->brightness;
        for (frame = 0, fade = 0; frame <= 44; frame++, fade += 0x444) {
            work = gWorkSlot.context;
            if (frame <= 24)
                Graphics_ScaleRgb555Clamped(session->palette, (u16 *)0x050000c0, *brightness = 0x10000 - fade, 128);
            BattleMotion_ProjectScaledPosition(unit, &position);
            work->origin_x = (64 - position.x) << 8;
            work->origin_y = (64 - position.y) << 8;
            QUEUE_BG2_ORIGIN(&work->origin_x);
            work->origin_pending = 1;
            WaitFrames(1);
        }
        Graphics_ResetVramBlockAndReleaseHeapBlocks(kind);
    } else if (mode == 1) {
        BattlePresentation_PrepareSceneFar(kind);
        for (frame = 39; frame >= 0; frame--) {
            work = gBattleFxWork[0];
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
        launch2.unknown_001c = 0;
        launch2.variant = 0;
        launch2.actor = unit;
        launch2.target = unit;
        launch2.kind = kind;
        launch2.actors[0] = unit;
        launch2.count = 1;
        launch2.unknown_0010 = 1;
        Func_080c9030(&launch2);
    }
}
