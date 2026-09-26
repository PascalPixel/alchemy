/* NONMATCHING: 1300 bytes, candidate 1300, 3 differing halfwords, 2 halfword
 * edits (2026-09-25). Scene_RunScene371SequenceA, meant for
 * FIELD/WORLD_MAP/F_039FC.C as a single-overlay unit binding its names at
 * their runtime addresses (an import veneer's listing offset plus 0x8000).
 * Remaining: Whole-function queue pointers and one-pass IME reads reproduce
 * every queued transfer; only the first task callback argument order
 * differs.
 * WALL: scheduling: first callback address load follows priority instead of
 * preceding it */
#include "TYPES.H"
#include "FIELD_EVENT.H"

struct DisplayTransfer {
    const void *source;
    void *destination;
    u32 control;
};

struct DisplayTransferQueue {
    u16 count;
    u16 unknown_02;
    struct DisplayTransfer entries[32];
};

extern struct DisplayTransferQueue gWorldMapTransferQueue;
extern u16 gWorldMapBlend;
extern volatile u16 Value_04000208;
extern const u8 gWorldMapPalettes[];
extern const u8 gWorldMapPackedTiles[];
extern const u8 gWorldMapPackedFrames[];

void *Main_08000170(s32 size);
void Main_08000178(void *buffer);
s32 Main_080001a8(const void *source, void *destination);
void Main_08009230(void);
void Main_08009238(void);
void WorldMap_RestoreBlend(void);
void WorldMap_UpdateBlend(void);

/* FAKEMATCH: the one-pass read preserves the saved-IME copy before masking.
 * Each request preserves IME while publishing one complete DMA transfer.
 * The original writes 0x0208 to IME; its enable bit is clear. */
#define QueueTransfer(source, destination, control) \
{ \
    u32 saved; \
    u32 *p; \
    s32 n; \
    do { saved = *ime; } while (0); \
    *ime = (u16)(u32)ime; \
    n = *(u16 *)&gWorldMapTransferQueue; \
    if (n < 32) { \
        p = (u32 *)&q->entries[n]; \
        *(u16 *)&gWorldMapTransferQueue = n + 1; \
        *p++ = (u32)(source); \
        *p++ = (u32)(destination); \
        *p = (control); \
    } \
    *ime = saved; \
}

#define QueueFrame(buffer, offset) QueueTransfer((buffer) + (offset), 0x06002000, 0x84000140)

void Scene_RunScene371SequenceA(s32 palette)
{
    struct DisplayTransferQueue *q;
    volatile u16 *ime;
    u8 *buffer = Main_08000170(0x4000);

    Engine_TaskWait(1);
    Engine_GameFlagClear(0x109);
    Main_08009230();
    Main_080001a8(gWorldMapPackedTiles, buffer);
    Main_080001a8(gWorldMapPackedFrames, buffer + 0x1000);
    q = &gWorldMapTransferQueue;
    ime = &Value_04000208;
    QueueTransfer(gWorldMapPalettes + palette * 32, (void *)0x050001c0, 0x80000010)
    QueueTransfer(buffer, (void *)0x06001000, 0x84000400)
    Engine_TaskAddCallback(WorldMap_RestoreBlend, 0xc80);
    Engine_EventBegin();
    QueueFrame(buffer, 0x3a80)
    Engine_ActorGet(gGameState.selected_actor)->active = 0;
    gEventWork->transition_frames = 16;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_AudioPlayCue(246);

    gWorldMapBlend = 0xe00;
    QueueFrame(buffer, 0x3480)
    Engine_EventWait(2);
    gWorldMapBlend = 0xd00;
    QueueFrame(buffer, 0x2e80)
    Engine_EventWait(2);
    gWorldMapBlend = 0xc00;
    QueueFrame(buffer, 0x2880)
    Engine_EventWait(2);
    gWorldMapBlend = 0xb00;
    QueueFrame(buffer, 0x2280)
    Engine_EventWait(2);
    gWorldMapBlend = 0xa00;
    QueueFrame(buffer, 0x1c80)
    Engine_EventWait(2);
    gWorldMapBlend = 0x900;
    QueueFrame(buffer, 0x1680)
    Engine_EventWait(2);
    gWorldMapBlend = 0x800;
    QueueFrame(buffer, 0x1080)
    Engine_EventWait(140);

    QueueFrame(buffer, 0x1680)
    Engine_EventWait(4);
    QueueFrame(buffer, 0x1c80)
    Engine_EventWait(4);
    QueueFrame(buffer, 0x2280)
    Engine_EventWait(4);
    gWorldMapBlend = 0x900;
    QueueFrame(buffer, 0x2880)
    Engine_EventWait(4);
    gWorldMapBlend = 0xa00;
    QueueFrame(buffer, 0x2e80)
    Engine_EventWait(4);
    gWorldMapBlend = 0xb00;
    QueueFrame(buffer, 0x3480)
    Engine_EventWait(4);
    gWorldMapBlend = 0xc00;
    QueueFrame(buffer, 0x3a80)
    Main_08009238();
    Engine_TaskAddCallback(WorldMap_UpdateBlend, 0xc80);
    Engine_AudioPlayCue(141);
    gWorldMapBlend = 0xd00;
    Engine_EventWait(4);
    gWorldMapBlend = 0xe00;
    Engine_EventWait(4);
    gWorldMapBlend = 0xf00;
    Engine_EventWait(4);
    gWorldMapBlend = 0x1000;
    Engine_EventWait(45);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Main_08000178(buffer);
    Engine_GameFlagSet(0x101);
}
