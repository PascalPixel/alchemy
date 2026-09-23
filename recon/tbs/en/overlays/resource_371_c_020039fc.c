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
extern const u8 gWorldMapPalettes[];
extern const u8 gWorldMapPackedTiles[];
extern const u8 gWorldMapPackedFrames[];

void *Func_0200c1c4(s32 size);
void Func_0200c1cc(void *buffer);
s32 Func_0200c1d4(const void *source, void *destination);
void Func_0200c184(s32 mode);
void Func_0200c24c(void);
void Func_0200c254(void);
void Func_0200c43c(void);
void Func_0200c444(void);
void Func_0200c44c(void);
void WorldMap_RestoreBlend(void);
void WorldMap_UpdateBlend(void);

/* Each request preserves IME while publishing one complete DMA transfer.
 * The original writes 0x0208 to IME; its enable bit is clear. */
static inline void QueueTransfer(const void *source, void *destination, u32 control)
{
    volatile u16 *ime = (volatile u16 *)0x04000208;
    u16 saved = *ime;
    struct DisplayTransfer *entry;

    *ime = 0x0208;
    if (gWorldMapTransferQueue.count < 32) {
        entry = &gWorldMapTransferQueue.entries[gWorldMapTransferQueue.count++];
        entry->source = source;
        entry->destination = destination;
        entry->control = control;
    }
    *ime = saved;
}

static inline void QueueFrame(const u8 *buffer, s32 offset)
{
    QueueTransfer(buffer + offset, (void *)0x06002000, 0x84000140);
}

void Scene_RunScene371SequenceA(s32 palette)
{
    u8 *buffer = Func_0200c1c4(0x4000);

    Func_0200c184(1);
    Engine_GameFlagClear(0x109);
    Func_0200c24c();
    Func_0200c1d4(gWorldMapPackedTiles, buffer);
    Func_0200c1d4(gWorldMapPackedFrames, buffer + 0x1000);
    QueueTransfer(gWorldMapPalettes + palette * 32, (void *)0x050001c0, 0x80000010);
    QueueTransfer(buffer, (void *)0x06001000, 0x84000400);
    Engine_TaskAddCallback(WorldMap_RestoreBlend, 0xc80);
    Engine_EventEnd();
    QueueFrame(buffer, 0x3a80);
    Engine_ActorLookup(gGameState.selected_actor)->active = 0;
    gEventWork->transition_frames = 16;
    Func_0200c43c();
    Func_0200c44c();
    Engine_AudioPlayCue(246);

    gWorldMapBlend = 0xe00;
    QueueFrame(buffer, 0x3480);
    Engine_EventWait(2);
    gWorldMapBlend = 0xd00;
    QueueFrame(buffer, 0x2e80);
    Engine_EventWait(2);
    gWorldMapBlend = 0xc00;
    QueueFrame(buffer, 0x2880);
    Engine_EventWait(2);
    gWorldMapBlend = 0xb00;
    QueueFrame(buffer, 0x2280);
    Engine_EventWait(2);
    gWorldMapBlend = 0xa00;
    QueueFrame(buffer, 0x1c80);
    Engine_EventWait(2);
    gWorldMapBlend = 0x900;
    QueueFrame(buffer, 0x1680);
    Engine_EventWait(2);
    gWorldMapBlend = 0x800;
    QueueFrame(buffer, 0x1080);
    Engine_EventWait(140);

    QueueFrame(buffer, 0x1680);
    Engine_EventWait(4);
    QueueFrame(buffer, 0x1c80);
    Engine_EventWait(4);
    QueueFrame(buffer, 0x2280);
    Engine_EventWait(4);
    gWorldMapBlend = 0x900;
    QueueFrame(buffer, 0x2880);
    Engine_EventWait(4);
    gWorldMapBlend = 0xa00;
    QueueFrame(buffer, 0x2e80);
    Engine_EventWait(4);
    gWorldMapBlend = 0xb00;
    QueueFrame(buffer, 0x3480);
    Engine_EventWait(4);
    gWorldMapBlend = 0xc00;
    QueueFrame(buffer, 0x3a80);
    Func_0200c254();
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
    Func_0200c444();
    Func_0200c44c();
    Func_0200c1cc(buffer);
    Engine_GameFlagSet(0x101);
}
