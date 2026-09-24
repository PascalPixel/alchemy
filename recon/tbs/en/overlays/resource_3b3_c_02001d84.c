/* NONMATCHING: 596 of 592 bytes, 263 differing halfwords (2026-09-24).
 * Hand-written from the disassembly: the per-frame driver for the four
 * pillars (actors 8-11) and their slots at 0x0200b6d0 (cell x, y, z, the
 * saved map cell, the flag), then TakaraHashira_Func02001be8 sorts them. The
 * statements and calls are in the reference's order; loop.c's address givs
 * differ: the reference indexes the slot table by a byte offset in sl off a
 * base in r8, keeps a second pointer giv in fp and &cell in r9, and spills
 * only the id, the index and the z offset. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "DMA.H"

struct MapCell {
    u32 tile : 12;
    u32 layer : 2;
    u32 kind : 2;
    u32 height : 8;
    u32 event : 8;
};

struct PillarSlot {
    s32 x;
    s32 y;
    s32 z;
    struct MapCell cell;
    s32 flag;
};

extern struct PillarSlot Data_0200b6d0[];
extern s32 Data_0200b720[];

s32 Main_080091d8(struct FieldActor *object, s32 *pos);
s32 Main_080091b0(s32 layer, s32 x, s32 z);
void TakaraHashira_SetCellAttributes(s32 layer, s32 x, s32 y, struct MapCell *src);
void TakaraHashira_ReadMapCell(s32 layer, s32 x, s32 y, struct MapCell *dst);
s32 Func_02001074(s32 id, s32 far);
void SceneActor_WaitHeightBelowLimit(struct FieldActor *actor, s32 limit);
void StagedActor_StepDownUntilClamp(s32 id);
void OverlayObject_WaitUntilSettledAndReset(struct FieldActor *actor);
void TakaraHashira_Func02001be8(void);

static __inline__ void Dma_Wait(volatile u32 *dma)
{
    while (dma[2] & 0x80000000)
        ;
}

void Func_02001d84(void)
{
    u32 id;
    struct FieldActor *actor;
    struct PillarSlot *slot;
    u8 *flags;
    u32 i;
    u32 k;
    struct FieldActor *other;
    struct MapCell cell;

    for (id = 8; id <= 11; id++) {
        actor = Engine_ActorGet(id);
        actor->unknown_22 = 2;
        i = id - 8;
        slot = &Data_0200b6d0[i];
        if ((actor->x.fixed >> 20) == Data_0200b6d0[i].x && (actor->z.fixed >> 20) == Data_0200b6d0[i].z
            && actor->velocity_y == 0) {
            continue;
        }
        Dma_Set(&actor->x, Data_0200b720, 0x84000003, (volatile u32 *)0x040000d4);
        Dma_Wait((volatile u32 *)0x040000d4);
        if (Main_080091d8(actor, Data_0200b720) == -1) {
            actor->motion_flags = 3;
        }
        flags = &actor->motion_flags;
        TakaraHashira_SetCellAttributes(0, Data_0200b6d0[i].x, Data_0200b6d0[i].z, &Data_0200b6d0[i].cell);
        TakaraHashira_SetCellAttributes(2, slot->x, Data_0200b6d0[i].z, &Data_0200b6d0[i].cell);
        if (*flags & 1) {
            if (Main_080091b0(2, actor->x.fixed, actor->z.fixed) == 50) {
                Engine_AudioPlayCue(189);
                actor->priority_flags &= 254;
                Func_02001074(id, 1);
                actor->priority_flags |= 1;
            } else if (Main_080091b0(2, actor->x.fixed, actor->z.fixed) == 51) {
                SceneActor_WaitHeightBelowLimit(actor, 0);
                Engine_AudioPlayCue(189);
                actor->y.fixed = 0;
                actor->priority_flags &= 254;
                StagedActor_StepDownUntilClamp(id);
                actor->x.fixed = 0;
                actor->y.fixed = 0;
                actor->z.fixed = 0;
                actor->priority_flags |= 1;
            } else {
                OverlayObject_WaitUntilSettledAndReset(actor);
            }
            *flags = 0;
        }
        TakaraHashira_ReadMapCell(0, actor->x.fixed >> 20, actor->z.fixed >> 20, &Data_0200b6d0[i].cell);
        if (actor->y.fixed >= 0) {
            TakaraHashira_ReadMapCell(0, 27, (actor->y.fixed >> 20) + 6, &cell);
            TakaraHashira_SetCellAttributes(0, actor->x.fixed >> 20, actor->z.fixed >> 20, &cell);
            cell.kind = slot->cell.kind;
            TakaraHashira_SetCellAttributes(2, actor->x.fixed >> 20, actor->z.fixed >> 20, &cell);
        }
        Data_0200b6d0[i].x = actor->x.fixed >> 20;
        Data_0200b6d0[i].y = actor->y.fixed >> 20;
        Data_0200b6d0[i].z = actor->z.fixed >> 20;
        for (k = 0; k <= 3; k++) {
            if (k == i) {
                continue;
            }
            Engine_GameFlagClear(Data_0200b6d0[k].flag);
            other = Engine_ActorGet(k + 8);
            if ((actor->x.fixed >> 20) == (other->x.fixed >> 20) && (actor->z.fixed >> 20) == (other->z.fixed >> 20)
                && actor->y.fixed > other->y.fixed) {
                Engine_GameFlagSet(Data_0200b6d0[k].flag);
            }
        }
    }
    TakaraHashira_Func02001be8();
}
