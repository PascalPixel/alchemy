#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "HEAP_STATE.H"
#include "CANVAS.H"
#include "MAP_SCROLL.H"
#include "BATTLE_PRESENTATION.H"
#include "BATTLE_WORK.H"
#include "BATTLE_EFFECT_WORK.H"
#include "PROJECT.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SYSTEM.H"
#include "CALLBACK_SCHEDULER.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"
#include "RAM_BUFFER.H"

#define BattlePresentation_SetPaletteLevelFar ((void (*)(u16,s32))BattlePresentation_SetPaletteLevelFar)


typedef s32 (*ClearWordsFn)(void *destination, s32 size);

static __inline__ void ClearWords(ClearWordsFn clear, void *destination, s32 size)
{
    /* FAKEMATCH: direct calls share the size in r5 and move the routine to r6, changing the canvas tails. */
    clear(destination, size);
}

void Audio_PlayCue(s32);


#define QUEUE_DISPLAY_CONTROL(control) do { \
    /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        do { \
            /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (control);                                     \
            *destination++ = (u32)&REG_DISPCNT;                             \
            *destination = 0x20000;                                         \
        }                                                                   \
        *ime = saved;                                                       \
    } while (0)

void BattleFx_BeginCanvasLayer(s32 bg_control)
{
    /* FAKEMATCH: the one-pass IME read and u16 count store preserve queue scheduling and whole-function pointer lifetimes. */

    struct BattleEffectWork *work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
    struct BtlWork *battle = *(gWorkSlot + HEAP_SLOT_BATTLE);
    void *canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
    struct BattleBackgroundView *display = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
    volatile u16 *ime;
    struct IoWriteQueue *q;
    s32 row;
    s32 col;
    s32 offset;

    BattleFx_ClearActorTimers();
    display->second_mode = 1;
    WaitFrames(1);
    REG_BLDCNT = 0;
    q = &gIoWriteQueue;
    ime = &REG_IME;
    QUEUE_DISPLAY_CONTROL(0x7741);
    gBgScroll[1].y = 32;
    WaitFrames(1);
    BattlePresentation_ConfigurePaletteFadeFar(1, battle->background, 0);
    work->fade_frames = 0;
    work->fade_step = 0;
    Scheduler_AddOrUpdateCallback(Palette_StepFadeTransfer, 0xc80);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
    WaitFrames(1);
    REG_BG2CNT = bg_control | 0x784;
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
    BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
    display->second_mode = 0;
    WaitFrames(1);

    REG_BLDCNT = 0x3f44;
    REG_BLDALPHA = 0x100e;
    REG_BG2X = 0;
    REG_BG2Y = -0x1000;
    REG_BG2PA = 0x80;
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = 0x100;
    REG_WIN0H = 0xf0;
    REG_WIN0V = 0x1088;
    REG_WIN1H = 0xf0;
    REG_WIN1V = 0x1088;
    REG_WININ = 0x3537;
    REG_WINOUT = 0x3f21;
    QUEUE_DISPLAY_CONTROL(0x7741);

    offset = 0;
    for (row = 0; row != 16; row++) {
        for (col = 0; col != 8; col++, offset += 2) {
            s32 tile;
            s16 pair;

            tile = row * 16 + col * 2;
            pair = ((tile + 1) << 8) | tile;
            *(volatile s16 *)((u8 *)BG_CANVAS_MAP + offset) = pair;
        }
    }

    ClearWords(Iwram_ClearWords, canvas, 0x4000);
    ClearWords(Iwram_ClearWords, BG_CHAR_BLOCK(1), 0x4000);
    work->shake_frames = 0;
    work->saved_bg1_x = gBgScroll[1].x;
    work->saved_bg1_y = gBgScroll[1].y;
    WaitFrames(1);
}

void BattleFx_OpenCanvasLayer(s32 bg_control)
{
    /* FAKEMATCH: the one-pass IME read and u16 count store preserve queue scheduling and whole-function pointer lifetimes. */

    struct BattleEffectWork *work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
    struct BtlWork *battle = *(gWorkSlot + HEAP_SLOT_BATTLE);
    void *canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
    struct BattleBackgroundView *display = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
    volatile u16 *ime;
    struct IoWriteQueue *q;
    s32 row;
    s32 col;
    s32 offset;

    BattleFx_ClearActorTimers();
    display->second_mode = 1;
    WaitFrames(1);
    REG_BLDCNT = 0;
    q = &gIoWriteQueue;
    ime = &REG_IME;
    QUEUE_DISPLAY_CONTROL(0x1741);
    gBgScroll[1].y = 32;
    WaitFrames(1);
    BattlePresentation_ConfigurePaletteFadeFar(1, battle->background, 128);
    work->fade_frames = 24;
    work->fade_step = 0;
    Scheduler_AddOrUpdateCallback(Palette_StepFadeTransfer, 0xc80);
    QUEUE_DISPLAY_CONTROL(0x1341);
    WaitFrames(1);
    REG_BG2CNT = bg_control | 0x784;
    QUEUE_DISPLAY_CONTROL(0x1341);
    BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
    display->second_mode = 0;
    WaitFrames(1);

    REG_BLDCNT = 0x3f44;
    REG_BLDALPHA = 0x100e;
    REG_BG2X = 0;
    REG_BG2Y = -0x1000;
    REG_BG2PA = 0x80;
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = 0x100;
    REG_WIN0H = 0xf0;
    REG_WIN0V = 0x1088;
    REG_WIN1H = 0xf0;
    REG_WIN1V = 0x1088;
    REG_WININ = 0x3537;
    REG_WINOUT = 0x3f21;
    QUEUE_DISPLAY_CONTROL(0x7741);
    WaitFrames(1);

    offset = 0;
    for (row = 0; row != 16; row++) {
        for (col = 0; col != 8; col++, offset += 2) {
            s32 tile;
            s16 pair;

            tile = row * 16 + col * 2;
            pair = ((tile + 1) << 8) | tile;
            *(volatile s16 *)((u8 *)BG_CANVAS_MAP + offset) = pair;
        }
    }

    ClearWords(Iwram_ClearWords, canvas, 0x4000);
    ClearWords(Iwram_ClearWords, BG_CHAR_BLOCK(1), 0x4000);
    WaitFrames(1);
}

#undef QUEUE_DISPLAY_CONTROL

void BattleFx_BeginTiledCanvas(s32 bg_control)
{
    s32 row;
    s32 col;
    s32 offset;

    BattleFx_BeginCanvasLayer(bg_control);
    REG_BG2CNT = bg_control | 0x6784;
    offset = 0;
    for (row = 0; row != 16; row++) {
        for (col = 0; col != 8; col++, offset += 2) {
            s32 tile;
            s16 pair;

            tile = row * 16 + col * 2;
            pair = ((tile + 1) << 8) | tile;
            *(volatile s16 *)((u8 *)BG_CANVAS_MAP + offset) = pair;
        }
        for (col = 0; col != 8; col++, offset += 2) {
            *(volatile u16 *)((u8 *)BG_CANVAS_MAP + offset) = 0;
        }
    }
}

void BattleFx_EndCanvasLayer(void)
{
    /* FAKEMATCH: the one-pass IME/queue scopes and u16 count store preserve scheduling, loading the queue before the preceding store. */

    struct BattleEffectWork *work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
    struct BtlWork *battle = *(gWorkSlot + HEAP_SLOT_BATTLE);
    s32 i;

    Audio_PlayCue(0x121);
    gBgScroll[1].x = work->saved_bg1_x;
    gBgScroll[1].y = work->saved_bg1_y;
    gProjection.center_x = 120;
    gProjection.center_y = 120;
    REG_BG2CNT = 0x787;
    Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
    Scheduler_RemoveCallback(Palette_StepFadeTransfer);
    gBgScroll[1].y = 32;
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
    REG_BLDCNT = 0;
    WaitFrames(1);

    BattlePresentation_ConfigurePaletteFadeFar(2, battle->background, 7);
    WaitFrames(1);
    for (i = 0; i != 8; i++) {
        BattlePresentation_SetPaletteLevelFar(battle->background, 21 - i * 3);
        WaitFrames(1);
    }

    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7541, 0x20000);
    WaitFrames(1);
}

void BattleFx_SetTransitionFlagAndDisplay(void)
{
  struct BtlWork *state;
  s32 one;
  s32 transfer;
  s32 *flag;

  flag = &((struct BattleBackgroundView *)Ram_WorkSlot[HEAP_SLOT_BATTLE_BACKGROUND])->second_mode;
  state = Ram_WorkSlot[HEAP_SLOT_BATTLE];
  *flag = 1;
  transfer = 0x1541;
  QueueIoWriteDelay2((u32)&REG_DISPCNT, transfer);
  one = 1;
  WaitFrames(one);
  BattlePresentation_ConfigurePaletteFadeFar(2, state->background, 0);
  transfer = one;
  WaitFrames(transfer);
}
