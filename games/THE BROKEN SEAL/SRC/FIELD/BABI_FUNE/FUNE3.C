/* Sets up the scene: actor collision and sprite priorities, the BG priorities,
 * the blend, and lowers map layers 6 and 7. The sprite priorities are the
 * FieldSprite bitfields read through an s32 view of actor->sprite; the BG
 * priorities 2 and 3 are one-halfword structs, whose short pool reach puts
 * the pool after the blend registers as in the ROM. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SERVICE.H"
#include "CALLBACK_SCHEDULER.H"
#include "DMA.H"
#include "IWRAM_CALL.H"

void Inventory_AddItemFar(s32 a, s32 b);
void FieldScene_ConfigureFixedPointValues(void);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

struct MapLayer {
    u8 unknown_00[12];
    s32 y;
    u8 unknown_10[32];
};

struct MapWork {
    u8 unknown_00[20];
    struct MapLayer layers[8];
};

/* The IWRAM field globals: the map work first, the event work at +0x4c. */
struct FieldGlobals {
    struct MapWork *map;
    u8 unknown_04[0x48];
    u8 *event;
};

extern struct FieldGlobals gMapWork;
#define SPRITE_BYTES(actor) ((u8 *)(actor)->sprite)
#define SPRITE_OF(actor) ((struct FieldSprite *)*(s32 *)((u8 *)(actor) + 0x50))
#define SET_PRIORITY(actor, n) ((n) == 9 ? (SPRITE_OF(actor)->priority = 1) : (SPRITE_OF(actor)->second_priority = 1))
#define REG_BG1CNT (*(volatile u16 *)0x0400000a)
#define REG_BG2CNT (*(volatile u16 *)0x0400000c)
#define REG_BG3CNT (*(volatile u16 *)0x0400000e)
#define REG_BLDCNT (*(volatile u16 *)0x04000050)
#define REG_BLDALPHA (*(volatile u16 *)0x04000052)

struct DisplayScrollState {
    u8 padding000[0xf00];
    u8 page;
};

extern struct DisplayScrollState *gHBlankScrollWork;

/* The hblank scroll work: two pages of BG3 offsets, a pair for each of the
 * 160 lines, the page the DMA reads, the wave's phase, and per axis the
 * wave's frequency, its step per line and its amplitude. */
struct ScrollWork {
    u16 pages[2][0x3c0];
    u8 page;
    u8 mode;
    u16 phase;
    u8 unknown_f04[4];
    s32 frequency[2];
    s32 step[2];
    s32 amplitude[2];
};

extern s16 gBgScroll[];
extern const s16 BabiFune_WaveSine[256];

/* One line's offset: the scroll swayed by the wave at this angle. */
static __inline__ s32 Wave_Offset(s32 angle, s32 amplitude, u16 base)
{
    return (u16)(Iwram_MulQ16(BabiFune_WaveSine[(angle >> 16) & 0xff], amplitude) / 256) + base;
}

extern u32 BabiFune_ShimmerActive;
extern u32 BabiFune_ShimmerPhase;
extern u32 BabiFune_CountTicks;
extern u32 BabiFune_DriftActive;
extern u32 BabiFune_SwellActive;
extern u32 BabiFune_SwellLayerSixY;
extern u32 BabiFune_SwellLayerSevenY;

void Battle_ResetEffectCounter(void);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void AudioCommand_WaitForStateByteClear(void);
void Scene_RunExtendedPresentationSequence(void);
void BabiFune_UpdateWaves(void);
void SceneState_CountDownEveryFortyTicks(void);
void BabiFune_CyclePalette(void);

#define REG_BACKDROP (*(volatile u16 *)0x05000000)
#define REG_BLDY (*(volatile u16 *)0x04000054)

/* The scheduler, which this overlay imports under its own name. */
static inline void BabiFune_AddTask(void (*task)(void), s32 order)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Scheduler_AddOrUpdateCallback((s32)task, order);
}

/* An engine service called with three arguments held in an inline's
 * parameters, which loads them in order. */
static inline void BabiFune_Call3(void (*service)(s32, s32, s32), s32 first, s32 second,
                                  s32 third)
{
    /* FAKEMATCH: the inline's parameters load the three arguments in order; a direct call builds the costly one first. */
    service(first, second, third);
}

/*
 * The ship's last scene: the backdrop warms to yellow, the lighthouse beam
 * flashes three times, the sea starts to swell and the ship sinks out of
 * view; then the screen fades to white and back and the next sequence runs.
 */
void BabiFune_RunDeparture(void)
{
    struct MapWork *map;
    s32 i;
    s32 flash;
    s32 white;
    s32 dark;
    s32 sunk;
    volatile u16 cnt;

    map = gMapWork.map;

    Event_Begin();
    Battle_ResetEffectCounter();
    BabiFune_Call3(ObjectMotion_SetPositionAndReset, 0, 312, 232);
    BabiFune_Call3((void (*)(s32, s32, s32))Engine_ActorFaceDirection, 0, 0xc000, 0);
    Event_Wait(40);
    Audio_PlayCue(140);
    for (i = 0; i <= 15; i++) {
        /* FAKEMATCH: the one-pass block keeps the colour's two shifts in source order. */
        do {
            REG_BACKDROP = (i << 11) | (i << 5);
        } while (0);
        Event_Wait(10);
    }
    {
        /* FAKEMATCH: a word temporary builds the colour by a shift, not from the pool. */
        s32 color = 0x7e00;

        REG_BACKDROP = color;
    }
    {
        /* FAKEMATCH: word temporaries keep both blend values in registers across the loop. */
        s32 bright = 0x1010;
        s32 dim = 0x810;

        for (flash = 0; flash < 3; flash++) {
            Audio_PlayCue(212);
            REG_BLDALPHA = bright;
            Event_Wait(3);
            /* FAKEMATCH: the one-pass block places the counter step after the wait's argument. */
            do {
                REG_BLDALPHA = dim;
            } while (0);
            Event_Wait(65);
        }
    }
    BabiFune_ShimmerActive = 1;
    BabiFune_ShimmerPhase = 0;
    BabiFune_AddTask(BabiFune_UpdateWaves, TASK_PRIORITY_SCENE);
    BabiFune_DriftActive = 1;
    Event_Wait(20);
    Audio_PlayCue(163);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Event_Wait(60);
    BabiFune_DriftActive = 1;
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Event_Wait(60);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    BabiFune_CountTicks = 0;
    BabiFune_AddTask(SceneState_CountDownEveryFortyTicks, TASK_PRIORITY_SCENE);
    sunk = 0;
    do {
        map->layers[6].y += 0x3333;
        map->layers[7].y += 0x3333;
        sunk += 0x3333;
        Task_Wait(1);
    } while (sunk <= 0x59ffff);
    Engine_TaskRemoveCallback(SceneState_CountDownEveryFortyTicks);
    BabiFune_DriftActive = 0;
    REG_BG3CNT = cnt = (REG_BG3CNT & 0xfffc) | 3;
    REG_BG2CNT = cnt = (REG_BG2CNT & 0xfffc) | 3;
    REG_BG1CNT = cnt = (REG_BG1CNT & 0xfffc) | 2;
    BabiFune_ShimmerActive = 0;
    Audio_PlayCue(288);
    Task_Wait(1);
    Audio_PlayCue(145);
    {
        /* FAKEMATCH: a word temporary in a one-pass block loads the value before the register's address. */
        s32 blend = 191;

        do {
            REG_BLDCNT = blend;
        } while (0);
    }
    for (white = 0; white <= 16; white++) {
        REG_BLDY = white;
        Event_Wait(1);
    }
    Event_Wait(40);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    BabiFune_SwellLayerSixY = map->layers[6].y;
    BabiFune_SwellLayerSevenY = map->layers[7].y;
    BabiFune_SwellActive = 1;
    for (dark = 16; dark >= 0; dark--) {
        REG_BLDY = dark;
        Event_Wait(8);
    }
    BabiFune_AddTask(BabiFune_CyclePalette, TASK_PRIORITY_SCENE);
    Audio_PlayCue(80);
    AudioCommand_WaitForStateByteClear();
    Event_Wait(20);
    Event_End();
    Scene_RunExtendedPresentationSequence();
}

s32 BabiFune_SetupScene(void)
{
    struct MapWork *map;
    volatile u16 cnt;

    map = gMapWork.map;
    if (((union GameStateRows *)&gGameState)->halves[225][0] == 99) {
        Inventory_AddItemFar(0, 242);
    }
    *(s32 *)(gMapWork.event + 0x1c0) = 0x100;
    Object_GetById(8)->collision_flags = 0;
    Object_GetById(8)->priority_flags = 2;
    Object_GetById(9)->collision_flags = 0;
    Object_GetById(9)->priority_flags = 2;
    {
        struct FieldActor *actor = Object_GetById(8);

        SET_PRIORITY(actor, 9);
    }
    {
        struct FieldActor *actor = Object_GetById(9);

        SET_PRIORITY(actor, 9);
    }
    {
        struct FieldActor *actor = Object_GetById(0);

        SET_PRIORITY(actor, 9);
        SET_PRIORITY(actor, 21);
    }
    {
        struct FieldActor *actor = Object_GetById(1);

        SET_PRIORITY(actor, 9);
        SET_PRIORITY(actor, 21);
    }
    {
        struct FieldActor *actor = Object_GetById(2);

        SET_PRIORITY(actor, 9);
        SET_PRIORITY(actor, 21);
    }
    {
        struct FieldActor *actor = Object_GetById(3);

        SET_PRIORITY(actor, 9);
        SET_PRIORITY(actor, 21);
    }
    {
        struct Half { u16 v; } two, three;

        cnt = REG_BG3CNT & 0xfffc;
        two.v = 2;
        cnt |= two.v;
        REG_BG3CNT = cnt;
        cnt = REG_BG2CNT & 0xfffc;
        three.v = 3;
        cnt |= three.v;
        REG_BG2CNT = cnt;
        cnt = (REG_BG1CNT & 0xfffc) | three.v;
        REG_BG1CNT = cnt;
    }
    {
        s32 v = 0x2648;

        /* FAKEMATCH: a do-while(0) keeps the BG1CNT store ahead of the
           BLDCNT address load, as in the reference */
        do {
            REG_BLDCNT = v;
            v = 0x810;
        } while (0);
        REG_BLDALPHA = v;
    }
    {
        struct MapLayer *layer = map->layers + 6;

        layer->y += -0x5a0000;
        layer = map->layers + 7;
        layer->y += -0x5a0000;
    }
    Engine_MapRedraw();
    FieldScene_ConfigureFixedPointValues();
    return 0;
}

/*
 * Babi Fune: arm the BG3 hblank scroll DMA. It copies the scroll line for the
 * current page into the hblank DMA source, clears the DMA enable and done
 * bits in the status register, and fires a single 32-bit transfer. This is
 * the overlay's own copy of the routine the shared scroll initialiser
 * (GRAPHICS/DISPLAY/SCROLL_INITIALIZE_HBLANK_DMA.C) arms by this name.
 */
void DisplayScroll_ArmHBlankDma(void)
{
    volatile u32 *dma;
    struct DisplayScrollState *state;
    u32 *source;
    u16 status;
    u32 control;
    volatile u32 *destination;

    state = gHBlankScrollWork;
    source = (u32 *)((u8 *)state + state->page * 0x780);

    dma = (volatile u32 *)0x040000b0;
    destination = (volatile u32 *)0x0400001c;
    status = *(volatile u16 *)((u8 *)dma + 10);
    control = 0xc5ff & status;
    *(volatile u16 *)((u8 *)dma + 10) = control;
    status = *(volatile u16 *)((u8 *)dma + 10);
    control = 0x7fff & status;
    *(volatile u16 *)((u8 *)dma + 10) = control;
    (void)*(volatile u16 *)((u8 *)dma + 10);

    *destination = *source++;
    control = 0xa6600001;
    Dma_Set(source, destination, control, dma);
}

/* Babi Fune: fill the page the DMA is not reading with this frame's wave,
 * each line's horizontal and vertical BG3 offset swayed from the scroll by
 * a sine of the line, then flip pages and advance the phase. */
void DisplayScroll_BuildAndSwapHBlankPage(void)
{
    struct ScrollWork *work;
    u16 *line;
    s32 angle;
    u16 base;
    s32 amplitude;
    s32 step;
    u16 y;
    s32 i;

    work = ((struct ScrollWork *)gHBlankScrollWork);
    y = gBgScroll[7];
    line = work->pages[work->page ^ 1];
    step = work->step[0];
    angle = (work->phase + y) * work->frequency[0];
    amplitude = work->amplitude[0];
    base = gBgScroll[6];
    for (i = 0; i != 160; i++) {
        *line = Wave_Offset(angle, amplitude, base);
        angle += step;
        line += 2;
    }
    line = work->pages[work->page ^ 1] + 1;
    step = work->step[1];
    angle = (work->phase + y) * work->frequency[1];
    amplitude = work->amplitude[1];
    for (i = 0; i != 160; i++) {
        *line = Wave_Offset(angle, amplitude, y);
        angle += step;
        line += 2;
    }
    work->phase++;
    work->page ^= 1;
}
