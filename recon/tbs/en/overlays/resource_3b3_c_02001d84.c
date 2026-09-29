/* NONMATCHING: 588 of 592 bytes, 226 differing halfwords, 52 aligned edits.
 * 2026-09-27 pillars H1: exact SETUP's five-word slot table, retaining a
 * typed pointer only for its X/kind consumers, emits identical candidate
 * bytes to the scalar-record baseline. Full normalized diff read: the first
 * X and second Z loads still fold to pointer accesses; cell/offset/slot
 * remain fp/r9/sl instead of r9/sl/fp. Frame20 and all five pool words agree.
 * The table-view hypothesis is rejected; typed record restored for H2.
 * H2 tests independently advancing the slot pointer alongside the actor id.
 * Predict indexed X/Z loads, a separately advanced slot and frame20; one
 * trial, full extent/pool comparison mandatory before adoption.
 * H2 rejected: 596/592 bytes, 287 halfwords / 81 aligned edits. Indexed
 * first X and second Z loads are admitted, but slot spills at sp+12,
 * frame grows to 24 and the priority-bit one lives in fp across calls.
 * Full normalized diff read; this confirms pointer/index independence can
 * recover the load form but exceeds the reference's pressure budget. Stop
 * this axis rather than permuting declarations; 588-byte body restored.
 * H3 (2026-09-27): create slot immediately after the position guard instead
 * of before it. Both first indexed X/Z loads match, but slot spills and the
 * frame grows from 20 to 28 bytes. Complete score 600/592, 286 differing
 * halfwords / 89 aligned edits, all five pool words retained. This rejects
 * the earlier phase boundary; the stronger 588-byte body is restored below.
 * Own-ROM extent 0x02001d84..0x02001fd4 includes the five-word pool.
 * SETUP calls this four-pillar frame driver; the final call sorts the actors.
 * 2026-09-27 transfer from exact WORLD_MAP/LINKED_EFFECTS.C: access the four
 * priority-flag updates through a plain byte view. The old store_bit_field
 * expansion creates QImode zero 146, live for 416 insns across 19 calls.
 * Removing that synthetic zero restores the reference's 20-byte frame,
 * actor r6, flags r7 and separate zero after StepDownUntilClamp. The local
 * cell address no longer spills. This is a proved source-level correction;
 * preserve these facts while repairing the remaining indexed slot accesses,
 * cell/offset/pointer high-register roles and cell-kind store ordering.
 * Full normalized diff has equal topology and the same five pool words,
 * four bytes early. Diagnostic and ordinary compilation agree.
 * Before that transfer, moving MapCell cell inside its only using branch
 * produced the old 596/263/150 bytes unchanged; scope alone is not the fix.
 * On the new byte-view model, replacing x/y/z with pos[3] is byte-identical.
 * Delaying slot assignment until just before the second SetCellAttributes
 * restores indexed first loads, but spills another pointer and regresses
 * the frame to 24: 588/219/54. Reject that follow-up and close the slot-scope
 * axis; do not combine it with declaration permutations. No DONE yet.
 * A one-element MapCell buffer still becomes the same scalar addressof
 * producer, high-register assignments and 20-byte frame. Its only code
 * change moves the cell-address copy before the kind load, against the ROM:
 * 588/226/53. Reject it; retain the scalar cell, with no buffer-size sweep.
 *
 * Historical baseline: 596/592 bytes, 263 halfwords, 150 aligned edits.
 * 2026-09-26 bounded triage: indexed pos[3] plus delayed slot ownership restored
 * the initial indexed X load but grew the frame from 24 to 28 (reference 20):
 * 600 bytes, 258 halfwords, 142 edits. An inline position-clear helper emitted
 * identical bytes; zero still lives across StepDownUntilClamp. Both rejected.
 * The allocator decoder found no unique source repair; no permutation ran.
 * Those trials predate the proved byte-view transfer above. */
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

extern struct PillarSlot TakaraHashira_PillarSlots[];
extern s32 Data_0200b720[];

s32 Engine_CheckMovementCollision(struct FieldActor *object, s32 *pos);
s32 Engine_MapQueryPosition(s32 layer, s32 x, s32 z);
void TakaraHashira_SetCellAttributes(s32 layer, s32 x, s32 y, struct MapCell *src);
void TakaraHashira_ReadMapCell(s32 layer, s32 x, s32 y, struct MapCell *dst);
s32 TakaraHashira_LowerActorToLedge(s32 id, s32 far);
void SceneActor_WaitHeightBelowLimit(struct FieldActor *actor, s32 limit);
void StagedActor_StepDownUntilClamp(s32 id);
void OverlayObject_WaitUntilSettledAndReset(struct FieldActor *actor);
void TakaraHashira_SortPillarActors(void);

static __inline__ void Dma_Wait(volatile u32 *dma)
{
    while (dma[2] & 0x80000000)
        ;
}

void TakaraHashira_UpdatePillarActors(void)
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
        slot = &TakaraHashira_PillarSlots[i];
        if ((actor->x.fixed >> 20) == TakaraHashira_PillarSlots[i].x && (actor->z.fixed >> 20) == TakaraHashira_PillarSlots[i].z
            && actor->velocity_y == 0) {
            continue;
        }
        Dma_Set(&actor->x, Data_0200b720, 0x84000003, (volatile u32 *)0x040000d4);
        Dma_Wait((volatile u32 *)0x040000d4);
        if (Engine_CheckMovementCollision(actor, Data_0200b720) == -1) {
            actor->motion_flags = 3;
        }
        flags = &actor->motion_flags;
        TakaraHashira_SetCellAttributes(0, TakaraHashira_PillarSlots[i].x, TakaraHashira_PillarSlots[i].z, &TakaraHashira_PillarSlots[i].cell);
        TakaraHashira_SetCellAttributes(2, slot->x, TakaraHashira_PillarSlots[i].z, &TakaraHashira_PillarSlots[i].cell);
        if (*flags & 1) {
            if (Engine_MapQueryPosition(2, actor->x.fixed, actor->z.fixed) == 50) {
                Engine_AudioPlayCue(189);
                /* FAKEMATCH: plain byte stores omit the synthetic narrow
                 * zero, as in the matched linked-effect family. */
                *(u8 *)&actor->priority_flags &= 254;
                TakaraHashira_LowerActorToLedge(id, 1);
                *(u8 *)&actor->priority_flags |= 1;
            } else if (Engine_MapQueryPosition(2, actor->x.fixed, actor->z.fixed) == 51) {
                SceneActor_WaitHeightBelowLimit(actor, 0);
                Engine_AudioPlayCue(189);
                actor->y.fixed = 0;
                *(u8 *)&actor->priority_flags &= 254;
                StagedActor_StepDownUntilClamp(id);
                actor->x.fixed = 0;
                actor->y.fixed = 0;
                actor->z.fixed = 0;
                *(u8 *)&actor->priority_flags |= 1;
            } else {
                OverlayObject_WaitUntilSettledAndReset(actor);
            }
            *flags = 0;
        }
        TakaraHashira_ReadMapCell(0, actor->x.fixed >> 20, actor->z.fixed >> 20, &TakaraHashira_PillarSlots[i].cell);
        if (actor->y.fixed >= 0) {
            TakaraHashira_ReadMapCell(0, 27, (actor->y.fixed >> 20) + 6, &cell);
            TakaraHashira_SetCellAttributes(0, actor->x.fixed >> 20, actor->z.fixed >> 20, &cell);
            cell.kind = slot->cell.kind;
            TakaraHashira_SetCellAttributes(2, actor->x.fixed >> 20, actor->z.fixed >> 20, &cell);
        }
        TakaraHashira_PillarSlots[i].x = actor->x.fixed >> 20;
        TakaraHashira_PillarSlots[i].y = actor->y.fixed >> 20;
        TakaraHashira_PillarSlots[i].z = actor->z.fixed >> 20;
        for (k = 0; k <= 3; k++) {
            if (k == i) {
                continue;
            }
            Engine_GameFlagClear(TakaraHashira_PillarSlots[k].flag);
            other = Engine_ActorGet(k + 8);
            if ((actor->x.fixed >> 20) == (other->x.fixed >> 20) && (actor->z.fixed >> 20) == (other->z.fixed >> 20)
                && actor->y.fixed > other->y.fixed) {
                Engine_GameFlagSet(TakaraHashira_PillarSlots[k].flag);
            }
        }
    }
    TakaraHashira_SortPillarActors();
}
