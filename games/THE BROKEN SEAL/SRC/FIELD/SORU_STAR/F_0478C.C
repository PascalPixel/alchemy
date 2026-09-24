/* Instanced unit: 380:0200478c and its twin 381:0200330c. Engine_* bind at
 * each overlay's runtime import veneer; the offset tables and the update
 * callback are per-overlay absolute symbols. */
#include "DMA.H"

struct SoruRingObject {
    u8 pad00[80];
    u8 *anim;
    u8 pad54;
    u8 state;
};

struct SoruRingEntry {
    struct SoruRingObject *obj;
    u8 pad04[24];
    s32 x;
    s32 z;
    u8 kind;
    u8 pad25[3];
};

extern s32 Soru_RingOffsetX[];
extern s32 Soru_RingOffsetZ[];
void Soru_UpdateRing(void);
struct SoruRingEntry *Engine_AllocateBlock(s32 slot, s32 size);
struct SoruRingObject *Engine_ActorGet(s32 actor);
void Engine_ObjectSetBlendMode(struct SoruRingObject *obj, s32 mode);
s32 Engine_TaskAddCallback(void *callback, s32 priority);

/* Mt. Aleph (Sol Sanctum and the crater): claim heap block 33 for up to ten
 * ring entries, one per actor from first, each blended, placed at its fixed
 * offset, and schedule the ring's update. The same function sits in both
 * overlays. */
void SoruStar_Func0200478c(s32 first, u32 count)
{
    struct SoruRingEntry *list;
    struct SoruRingEntry *entry;
    struct SoruRingObject *obj;
    u32 i;
    s32 none;
    s32 cleared;
    volatile u32 zero;

    list = Engine_AllocateBlock(33, 0x194);
    zero = 0;
    entry = list;
    Dma_Set((const void *)&zero, list, 0x85000065, (volatile u32 *)0x040000d4);
    if (count > 10)
        count = 10;
    none = 0;
    i = none;
    if (count != 0) {
        cleared = none;
        do {
            obj = Engine_ActorGet(first);
            {
                u8 *frame = &obj->anim[38];

                entry->obj = obj;
                *frame = cleared;
            }
            obj->state = cleared;
            Engine_ObjectSetBlendMode(Engine_ActorGet(first), 1);
            entry->x = Soru_RingOffsetX[i];
            entry->z = -Soru_RingOffsetZ[i];
            entry->kind = 3;
            i++;
            entry++;
            first++;
        } while (i != count);
    }
    *(u16 *)((u8 *)list + 0x190) = count;
    Engine_TaskAddCallback((void *)Soru_UpdateRing, 0xc80);
}
