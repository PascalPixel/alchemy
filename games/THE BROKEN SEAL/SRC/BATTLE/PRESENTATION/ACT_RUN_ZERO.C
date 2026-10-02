#include "TYPES.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"
#include "IO_WRITE_QUEUE.H"
#include "EFFECT_STEP.H"
#include "BATTLE_WORK.H"

/* The tail of the work block in heap slot 39 that both the star field and
   the prepared scene end with: where BG2's reference point should be, and
   word that it has changed. */
struct Bg2OriginWork {
    u8 unknown_0000[0x13c4];
    s32 origin_x;
    s32 origin_y;
    s32 origin_pending;
};

/* The argument block the effect launchers take. The frame gives each of the
   two here 84 bytes. */
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

extern void *gWorkSlot[];

void BattlePresentation_ConfigurePaletteFade(s32 mode, u16 value, s32 fade);
s32 BattleFx_InitializeStarField(s32 kind);
void Graphics_ResetVramBlockAndReleaseHeapBlocks(s32 kind);
s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale, s32 count);
void BattleMotion_ProjectScaledPosition(s32 unit, struct EffectPosition *position);
void Func_080c9020(struct EffectLaunch *launch);
void Func_080c9030(struct EffectLaunch *launch);
void BattlePresentation_PrepareSceneFar(s32 kind);
void BattleFx_ScheduleCallbacksAndReleaseBlocksFar(void);

/* FAKEMATCH: the write goes through its own int, which loads each value as a
   word, and the do/while (0) ends a scheduling region after it, as in the
   slideshow's register writes. */
#define Io_Write16(reg, value) \
    do { \
        /* FAKEMATCH: the one-pass register-write boundary preserves measured instruction scheduling. */ \
        /* FAKEMATCH: the register-write word temporary preserves measured value allocation. */ \
        s32 value_ = (value); \
        (reg) = value_; \
    } while (0)

/* Queue the two-word copy of the origin to BG2's reference point for the
   next frame, with interrupts off. */
#define BG2_QUEUE_ORIGIN(ime, find_ime, origin) \
    { \
        struct IoWriteQueue *q; \
        u32 saved; \
        s32 count; \
 \
        q = &gIoWriteQueue; \
        /* FAKEMATCH: the one-pass block keeps the saved copy ahead of the IME store, as in the IO write queue. */ \
        do { \
            find_ime; \
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
        /* FAKEMATCH: the one-pass block keeps the IME restore ahead of the next statement. */ \
        do { \
            *ime = saved; \
        } while (0); \
    }

/*
 * Show one of four effects on a unit. Mode 0 is the star field, faded in
 * over 25 frames and followed for 45; mode 1 the prepared scene, followed
 * for 40; mode 2 and the others launch an effect with an argument block
 * built here.
 */
void BattleFx_PlayUnitElementEffect(s32 unit, s32 kind, s32 mode, s32 variant)
{
    u8 unused[40]; /* FAKEMATCH: unused; it only reserves the 40 bytes the reference frame has above the positions. */
    struct EffectPosition position;
    struct EffectPosition position2;
    struct EffectLaunch launch;
    struct EffectLaunch launch2;
    /* FAKEMATCH: the reference holds the address of slot 12 in the register
       that later holds the IME address; only a variable assigned twice does
       that. The session is slot 9, read three words below it. */
    volatile u16 *ime = (volatile u16 *)&gWorkSlot[12];
    struct BattleSession *session = *(struct BattleSession **)((u8 *)ime - 12);
    struct Bg2OriginWork *work;
    s32 frame;
    s32 *brightness;
    /* Left to the allocator the IME address takes r6 and the position r7,
       which exchanges the two registers in 14 instructions. */
    /* FAKEMATCH: the position pointer is pinned to r6, as the reference keeps it. */
    register struct EffectPosition *pos __asm__("r6");

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
        for (frame = 0, pos = &position; frame <= 44; frame++) {
            brightness = &session->brightness;
            ime = (volatile u16 *)0x04000208;
            work = gWorkSlot[39];
            if (frame <= 24)
                Graphics_ScaleRgb555Clamped(session->palette, (u16 *)0x050000c0, *brightness = 0x10000 - frame * 1092, 128);
            {
                /* The reference loads the second argument before the first;
                   a variable in r1 is loaded where it is assigned. */
                /* FAKEMATCH: the second argument is pinned to r1 to load it first. */
                register struct EffectPosition *out __asm__("r1");

                out = pos;
                BattleMotion_ProjectScaledPosition(unit, out);
            }
            work->origin_x = (64 - pos->x) << 8;
            work->origin_y = (64 - pos->y) << 8;
            BG2_QUEUE_ORIGIN(ime, (void)0, &work->origin_x);
            work->origin_pending = 1;
            WaitFrames(1);
        }
        Graphics_ResetVramBlockAndReleaseHeapBlocks(kind);
    } else if (mode == 1) {
        BattlePresentation_PrepareSceneFar(kind);
        for (frame = 0; frame < 40; frame++) {
            work = gWorkSlot[39];
            BattleMotion_ProjectScaledPosition(unit, &position2);
            work->origin_x = (64 - position2.x) << 8;
            work->origin_y = (64 - position2.y) << 8;
            {
                volatile u16 *reg;

                BG2_QUEUE_ORIGIN(reg, reg = (volatile u16 *)0x04000208, &work->origin_x);
            }
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

#include "SCENE.H"
volatile unsigned long long BattlePresentation_SetPaletteLevel(s32, s32);

void BattlePres_RunWithZeroArguments(void)
{
  BattlePresentation_SetPaletteLevel((unsigned long) 0, 0);
}
