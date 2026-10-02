/* 2026-10-02: the remaining difference is one allocation fact. In the
 * greg dump the actor (67 refs/222 insns) outranks the cell offset (10/79)
 * by priority, so it is not order: global.c's first pass only takes a hard
 * register already in use, and r5/r6 are held by block-local pointers while
 * the actor lives. The game's actor lands in r8 because r8 is already in use
 * (or r7 is held) when the actor is allocated; here r7 is the first free.
 * Pinning the actor to r8 defeats CSE of its coordinates (270 lines). */
/* NONMATCHING (2026-09-27): explicit selected-cell byte offset, shared
 * between the depth test and both CopyCells calls, gives 956/976 bytes,
 * 431 halfwords / 198 aligned edits. Actor remains r7, offset becomes sl,
 * frame grows from 16 to 20, flags spill and an extra table+4 pool word
 * appears. Full normalized diff read; actor-r8/cell-r7 admission fails.
 * Restore indexed SwitchCell accesses. No credit or compiler changes. */
/* 2026-09-27 lane stop: H3/H4 carrier trials are preserved in commits
 * 70d14bfc9 and 266293aa5. Both fail actor-r8/cell-r7 admission; restore
 * this typed canonical body rather than retain an unexplained one-edit
 * score gain. Current full baseline: 952/976 bytes, 414 halfwords/154 edits.
 * No DONE or alignment changes. A new allocation/interference fact is
 * required before reopening the pointer-phase axis. */
/* NONMATCHING H4 (2026-09-27): scalar byte-pointer carrier emits the
 * identical complete object to H3 (cmp), 952/976 bytes, 414 halfwords /
 * 153 aligned edits. Full normalized diff read; the completion address
 * still receives a separate pseudo. Thus union member expansion alone
 * does not explain the r7/r8 inversion. Both carrier representations fail
 * the mandatory actor-r8/cell-r7 admission. Stop this two-trial axis; keep
 * the typed H3 and scalar H4 in history, then restore the prior canonical
 * actor/effect model. No function or alignment credit.
 *
 * NONMATCHING H3 (2026-09-27): phased actor/priority/completion pointer
 * union, 952/976 bytes, 414 differing halfwords / 153 aligned edits.
 * Own listing reuses r8 across these three nonoverlapping pointer phases.
 * The union reconstructs the in-place priority-byte transition, but GCC
 * scalarizes actor SI33 (77 refs/242 insns/23 calls) and completion SI281
 * separately; actor remains r7 and cell offset r8. Full normalized diff
 * read: frame, calls and pool values retained; required r8 carrier fails.
 * Preserve this negative witness before the one scalar-carrier follow-up.
 * No function or alignment credit; all six owners remain not-yet-c.
 *
 * Prior NONMATCHING: 976 bytes, candidate 952, 414 differing halfwords, 154
 * halfword edits (2026-09-27). Scene_RunScene3c8SequenceA, meant for
 * FIELD/VINASU_HEYA/F_02B14.C as a single-overlay unit binding its names at
 * their runtime addresses (an import veneer's listing offset plus 0x8000).
 * Remaining: Rebuilt the complete pillar-trigger sequence from the
 * disassembly, correcting the table search and actor loops; remaining
 * differences are register lifetimes and scheduling.
 * New bounded trials (2026-09-26): coherent actor/effect union ownership
 * stayed at 190 edits; independent flag pointers gave 197, and separate
 * table-search/actor-loop counters 206. Restored the best whole candidate.
 * Remaining: actor versus loop-counter registers and pointer lifetimes.
 * 2026-09-27 audit: complete 02002b14..02002ee4, including ten final
 * pool words. Every emitted call target and sequence agrees with own ROM;
 * no missing call or misbound import found. Exact SETTLE_BLOCKS.C plus the
 * 0c5c constructor establish separate switch-effect script state: active
 * word +0 and completion byte +63. H1 owns these two effects in the final
 * phase rather than reusing actor pointers and the block flags pointer.
 * Baseline 960/976, 415 halfwords / 190 edits becomes 952/976, 414/154.
 * Full normalized diff: id=sl, flags=fp, priority=r9, scan=r6 and slot=r5
 * now match. Block=r7 versus r8 and cell offset=r8 versus r7 remain;
 * velocity stores still reverse, and completion pointer is r7 versus r8.
 * All calls, frame and pool values remain correct. This is not exact.
 * H2 transfers the sibling's unsigned SwitchCell { x, z } indexed table.
 * The full candidate is byte-identical to H1 (cmp); field ownership does
 * not change the cell-offset lifetime. Retain the evidenced views, stop
 * both axes here. No declaration or counter permutations; no DONE credit. */
#include "TYPES.H"
#include "FIELD_EFFECT.H"

struct SwitchCell {
    u32 x;
    u32 z;
};

extern struct SwitchCell Data_0200d164[8];
extern s32 Data_0200d77c[];
extern s32 Data_0200d7c8[];
extern s32 Data_0200dac8[];
extern s32 Data_0200dd3c[];

/* Exact SETTLE_BLOCKS.C and the local constructor share this script state. */
struct SwitchEffect {
    s32 active;
    u8 unknown_04[0x5f];
    u8 finished;
};

struct FieldActor *Local_02000098(s32 x, s32 y, s32 z, s32 type);
void Local_0200094c(struct FieldActor *actor);
struct SwitchEffect *Local_02000c5c(s32 x, s32 z, s32 *script);
void Main_080090d0(struct FieldActor *actor);
void Main_08009148(struct SwitchEffect *effect);
void Main_08009098(struct SwitchEffect *effect, s32 *script);
void Main_08009178(s32 *script, s32 x, s32 z);
void Main_0808a1e0(s32 actor, s32 priority);
void Main_080091c0(s32 sx, s32 sy, s32 w, s32 h, s32 x, s32 z);
void Engine_CameraMoveTo(s32 x, s32 y, s32 z, s32 pan);
void Engine_CameraSetSpeed(s32 speed, s32 acceleration);
s32 Engine_GameFlagIsSet(s32 flag);
s32 Engine_GameFlagSet(s32 flag);

static __inline__ void CopyCells(s32 sx, s32 sy, s32 w, s32 h, s32 x, s32 z)
{
    Main_080091c0(sx, sy, w, h, x, z);
}

void Scene_RunScene3c8SequenceA(void)
{
    struct FieldActor *effect;
    struct FieldActor *actor;
    struct FieldActor *other;
    struct FieldActor *leader;
    struct FieldActor *b;
    struct FieldActor *c;
    struct FieldActor *d;
    u8 *flags;
    u8 *motion;
    u32 id;
    u32 i;
    u32 slot;
    u32 priority;

    effect = 0;
    leader = Actor_Get(0);
    Engine_EventBegin();
    CopyCells(69, 48, 4, 2, 5, 48);
    CopyCells(73, 37, 9, 13, 9, 37);
    for (id = 15; id <= 18; id++) {
        actor = Object_GetById(id);
        flags = &actor->priority_flags;
        if (*flags != 2)
            CopyCells(72, 48, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
        else
            CopyCells(73, 48, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);

        slot = 8;
        for (i = 0; i < 8; i++) {
            if ((actor->x.fixed >> 20) == Data_0200d164[i].x
                && (actor->z.fixed >> 20) == Data_0200d164[i].z
                && actor->y.fixed >= 0) {
                slot = i;
                break;
            }
        }
        if (slot == 8)
            continue;
        for (i = 15; i <= 18; i++) {
            other = Object_GetById(i);
            if (id != i
                && (actor->x.fixed >> 20) == (other->x.fixed >> 20)
                && (actor->z.fixed >> 20) == (other->z.fixed >> 20)) {
                slot = 8;
                break;
            }
        }
        if (slot == 8)
            continue;

        priority = leader->sprite->priority;
        if ((u32)(leader->z.fixed >> 20) <= Data_0200d164[slot].z) {
            effect = Local_02000098(actor->x.fixed, actor->y.fixed,
                                   actor->z.fixed - 0x40000, 20);
            Main_0808a1e0(0, 3);
        }
        for (i = 15; i <= 18; i++) {
            other = Object_GetById(i);
            if (id != i
                && (actor->x.fixed >> 20) == (other->x.fixed >> 20)
                && (actor->z.fixed >> 20) - 1 == (other->z.fixed >> 20))
                Main_0808a1e0(i, 3);
        }
        Engine_ActorSetSpriteFlags(Object_GetById(id), 0);
        actor->unknown_22 = 0;
        motion = &actor->motion_flags;
        *motion = 3;
        ((union FieldObject *)actor)->effect.velocity_y = 0x1999;
        ((union FieldObject *)actor)->effect.velocity_x = 0;
        CopyCells(6, 44, 1, 1, Data_0200d164[slot].x, Data_0200d164[slot].z);
        Local_0200094c(actor);
        Engine_AudioPlayCue(188);
        actor->collision_flags = 0;
        *motion = 0;
        actor->y.fixed = -0x100000;
        Main_0808a1e0(id, 3);
        *flags = 2;
        CopyCells(73, 48, 1, 1, Data_0200d164[slot].x, Data_0200d164[slot].z);
        Main_0808a1e0(0, priority);
        Object_GetById(0)->priority_flags |= 1;
        for (i = 15; i <= 18; i++) {
            other = Object_GetById(i);
            if (id != i
                && (actor->x.fixed >> 20) == (other->x.fixed >> 20)
                && (actor->z.fixed >> 20) - 1 == (other->z.fixed >> 20)) {
                Main_0808a1e0(i, 1);
                Object_GetById(i)->priority_flags |= 1;
            }
        }
        Main_080090d0(effect);
        if (GameFlag_IsSet(0x308)) {
            Engine_EventEnd();
            return;
        }
        actor = Actor_Get(15);
        b = Actor_Get(16);
        c = Actor_Get(17);
        d = Object_GetById(18);
        if ((actor->priority_flags & b->priority_flags & c->priority_flags & d->priority_flags) & 2) {
            struct SwitchEffect *first;
            struct SwitchEffect *second;
            u8 *finished;

            Camera_SetSpeed(0x10000, 0x2000);
            Engine_CameraMoveToActor(14, 1);
            Engine_CameraWaitForMove();
            first = Local_02000c5c(136, 0x308, Data_0200d77c);
            Engine_EventWait(30);
            Camera_SetSpeed(0x6666, 0xccc);
            Camera_MoveTo(0xd80000, -1, 0x2780000, 1);
            Main_08009148(first);
            Main_08009098(first, Data_0200d7c8);
            second = Local_02000c5c(216, 0x2f8, Data_0200dac8);
            finished = &first->finished;
            while (first->active != 0 || second->active != 0) {
                if (*finished != 0 || second->finished != 0) {
                    Engine_EventWait(30);
                    Main_08009178(Data_0200dd3c, 77, 35);
                    CopyCells(13, 35, 1, 1, 13, 36);
                    GameFlag_Set(0x308);
                    break;
                }
                Engine_TaskWait(1);
            }
        }
    }
    Engine_EventEnd();
}
