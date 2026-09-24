/* NONMATCHING: 380:0200478c and its twin 381:0200330c, 200 of 200 bytes, 3
 * halfword edits (2026-09-24). Written as an instanced unit source with
 * absolute symbols (Engine_* at each overlay's runtime import veneer; the
 * offset tables at 380 0x0200d140/0x0200d168, 381 0x0200ba68/0x0200ba90;
 * the update callback at 380 0x0200c49c, 381 0x0200b01c). Remaining: the
 * X table constant loads into r2 in the reference, r3 here, which flips
 * both indexed-load operand orders. */
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
s32 Engine_ScheduleCallback(void *callback, s32 priority);

/* Mt. Aleph (Sol Sanctum and the crater): claim heap block 33 for up to ten
 * ring entries, one per actor from first, each blended, placed at its fixed
 * offset, and schedule the ring's update. The same function sits in both
 * overlays. */
void Soru_StartActorRing(s32 first, u32 count)
{
    struct SoruRingEntry *list;
    struct SoruRingEntry *entry;
    struct SoruRingObject *obj;
    u32 i;
    s32 k;
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
        k = 0;
        do {
            obj = Engine_ActorGet(first);
            {
                u8 *frame = &obj->anim[38];

                entry->obj = obj;
                *frame = cleared;
            }
            obj->state = cleared;
            Engine_ObjectSetBlendMode(Engine_ActorGet(first), 1);
            entry->x = *(s32 *)(k + (u8 *)Soru_RingOffsetX);
            entry->z = -*(s32 *)((u8 *)Soru_RingOffsetZ + k);
            entry->kind = 3;
            i++;
            k += 4;
            entry++;
            first++;
        } while (i != count);
    }
    *(u16 *)((u8 *)list + 0x190) = count;
    Engine_ScheduleCallback((void *)Soru_UpdateRing, 0xc80);
}
