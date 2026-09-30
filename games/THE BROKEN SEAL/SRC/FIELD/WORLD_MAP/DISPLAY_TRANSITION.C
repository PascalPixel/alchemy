/* Exact 1300-byte owner, resource_371:020039fc..02003f10.
 * Verified 2026-09-27: zero differing halfwords and equal topology.
 * The previous two-halfword residual was callback-address loading before
 * IME restoration. A one-pass scope around the final volatile restore keeps
 * that publication boundary intact; all queue entries and pools still match.
 * Reconstructed from our own ROM and the existing world-map queue model. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IO_REG.H"

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

extern struct DisplayTransferQueue gIoWriteQueue;
u16 gWorldMapBlend;
extern const u8 gWorldMapPalettes[];
extern const u8 gWorldMapPackedTiles[];
extern const u8 gWorldMapPackedFrames[];

void *Runtime_BumpAllocateAlternatePool(s32 size);
void Sys_Free(void *buffer);
s32 Resource_DecodeType01(const void *source, void *destination);
void Map_ResumeAnimation(void);
void Map_LoadAreaGraphics(void);
void SceneEffect_RestoreBlendRegisters(void);
void FieldScene_RunLateSequence(void);

/* FAKEMATCH: the one-pass read preserves the saved-IME copy before masking.
 * Each request preserves IME while publishing one complete DMA transfer.
 * The original writes 0x0208 to IME; its enable bit is clear.
 * FAKEMATCH: the one-pass restore keeps subsequent callback setup outside
 * the queue-publication control boundary. */
#define QueueTransfer(source, destination, control) \
{ \
    u32 saved; \
    u32 *p; \
    s32 n; \
    do { saved = *ime; } while (0); \
    *ime = (u16)(u32)ime; \
    n = *(u16 *)&gIoWriteQueue; \
    if (n < 32) { \
        p = (u32 *)&q->entries[n]; \
        *(u16 *)&gIoWriteQueue = n + 1; \
        *p++ = (u32)(source); \
        *p++ = (u32)(destination); \
        *p = (control); \
    } \
    do { *ime = saved; } while (0); \
}

#define QueueFrame(buffer, offset) QueueTransfer((buffer) + (offset), 0x06002000, 0x84000140)

static __inline__ void StartCallback(void (*callback)(void), s32 priority)
{
    /* FAKEMATCH: a single-pass call keeps the callback before its priority. */
    do {
        Engine_TaskAddCallback(callback, priority);
    } while (0);
}

void Scene_RunScene371SequenceA(s32 palette)
{
    struct DisplayTransferQueue *q;
    volatile u16 *ime;
    u8 *buffer = Runtime_BumpAllocateAlternatePool(0x4000);

    Engine_TaskWait(1);
    Engine_GameFlagClear(0x109);
    Map_ResumeAnimation();
    Resource_DecodeType01(gWorldMapPackedTiles, buffer);
    Resource_DecodeType01(gWorldMapPackedFrames, buffer + 0x1000);
    /* FAKEMATCH: one scope keeps the queue ahead of the IME pointer in
     * register allocation and the literal pool. */
    do {
        q = &gIoWriteQueue;
        ime = &REG_IME;
    } while (0);
    QueueTransfer(gWorldMapPalettes + palette * 32, (void *)0x050001c0, 0x80000010)
    QueueTransfer(buffer, (void *)0x06001000, 0x84000400)
    StartCallback(SceneEffect_RestoreBlendRegisters, 0xc80);
    Engine_EventBegin();
    QueueFrame(buffer, 0x3a80)
    Object_GetById(gGameState.selected_actor)->active = 0;
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
    Map_LoadAreaGraphics();
    Engine_TaskAddCallback(FieldScene_RunLateSequence, 0xc80);
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
    Sys_Free(buffer);
    Engine_GameFlagSet(0x101);
}
