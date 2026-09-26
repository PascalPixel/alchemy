#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "DMA.H"

struct PillarSlot {
    u8 unknown_00[16];
    s32 flag;
};

extern struct PillarSlot Data_0200b6d0[];

#define DMA3 ((volatile u32 *)0x040000d4)

static __inline__ void Dma_Wait(volatile u32 *dma)
{
    while (dma[2] & 0x80000000)
        ;
}

void TakaraHashira_SortPillarActors(void)
{
    u32 i;
    u32 j;
    struct FieldActor *a;
    struct FieldActor *b;
    u8 actor[112];
    /* Holds the first 16 bytes of a slot; the copy runs over into actor, which is no longer needed. */
    s32 slot[3];

    for (i = 0; i <= 2; i++) {
        a = Engine_ActorGet(i + 8);
        for (j = i; j <= 3; j++) {
            b = Engine_ActorGet(j + 8);
            if (a->y.fixed <= b->y.fixed && a->z.fixed < b->z.fixed) {
                continue;
            }
            Dma_Set(b, actor, 0x8400001c, DMA3);
            Dma_Wait(DMA3);
            Dma_Set(a, b, 0x8400001c, DMA3);
            Dma_Wait(DMA3);
            Dma_Set(actor, a, 0x8400001c, DMA3);
            Dma_Wait(DMA3);
            Dma_Set(&Data_0200b6d0[j], slot, 0x84000004, DMA3);
            Dma_Wait(DMA3);
            Dma_Set(&Data_0200b6d0[i], &Data_0200b6d0[j], 0x84000004, DMA3);
            Dma_Wait(DMA3);
            Dma_Set(slot, &Data_0200b6d0[i], 0x84000004, DMA3);
            Dma_Wait(DMA3);
            if (Engine_GameFlagIsSet(Data_0200b6d0[i].flag) && !Engine_GameFlagIsSet(Data_0200b6d0[j].flag)) {
                Engine_GameFlagClear(Data_0200b6d0[i].flag);
                Engine_GameFlagSet(Data_0200b6d0[j].flag);
            } else if (!Engine_GameFlagIsSet(Data_0200b6d0[i].flag) && Engine_GameFlagIsSet(Data_0200b6d0[j].flag)) {
                Engine_GameFlagSet(Data_0200b6d0[i].flag);
                Engine_GameFlagClear(Data_0200b6d0[j].flag);
            }
        }
    }
}
