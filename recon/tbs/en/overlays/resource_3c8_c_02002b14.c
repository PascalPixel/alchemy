/* NONMATCHING: 976 bytes, candidate 960, 415 differing halfwords, 190
 * halfword edits (2026-09-25). Scene_RunScene3c8SequenceA, meant for
 * FIELD/VINASU_HEYA/F_02B14.C as a single-overlay unit binding its names at
 * their runtime addresses (an import veneer's listing offset plus 0x8000).
 * Remaining: Rebuilt the complete pillar-trigger sequence from the
 * disassembly, correcting the table search and actor loops; remaining
 * differences are register lifetimes and scheduling.
 * New bounded trials (2026-09-26): coherent actor/effect union ownership
 * stayed at 190 edits; independent flag pointers gave 197, and separate
 * table-search/actor-loop counters 206. Restored the best whole candidate.
 * Remaining: actor versus loop-counter registers and pointer lifetimes. */
#include "TYPES.H"
#include "FIELD_EFFECT.H"

extern s32 Data_0200d164[8][2];
extern s32 Data_0200d77c[];
extern s32 Data_0200d7c8[];
extern s32 Data_0200dac8[];
extern s32 Data_0200dd3c[];

struct FieldActor *Local_02000098(s32 x, s32 y, s32 z, s32 type);
void Local_0200094c(struct FieldActor *actor);
struct FieldActor *Local_02000c5c(s32 x, s32 z, s32 *script);
void Main_080090d0(struct FieldActor *actor);
void Main_08009148(struct FieldActor *actor);
void Main_08009098(struct FieldActor *actor, s32 *script);
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
        actor = Engine_ActorGet(id);
        flags = &actor->priority_flags;
        if (*flags != 2)
            CopyCells(72, 48, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
        else
            CopyCells(73, 48, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);

        slot = 8;
        for (i = 0; i < 8; i++) {
            if ((actor->x.fixed >> 20) == Data_0200d164[i][0]
                && (actor->z.fixed >> 20) == Data_0200d164[i][1]
                && actor->y.fixed >= 0) {
                slot = i;
                break;
            }
        }
        if (slot == 8)
            continue;
        for (i = 15; i <= 18; i++) {
            other = Engine_ActorGet(i);
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
        if ((u32)(leader->z.fixed >> 20) <= (u32)Data_0200d164[slot][1]) {
            effect = Local_02000098(actor->x.fixed, actor->y.fixed,
                                   actor->z.fixed - 0x40000, 20);
            Main_0808a1e0(0, 3);
        }
        for (i = 15; i <= 18; i++) {
            other = Engine_ActorGet(i);
            if (id != i
                && (actor->x.fixed >> 20) == (other->x.fixed >> 20)
                && (actor->z.fixed >> 20) - 1 == (other->z.fixed >> 20))
                Main_0808a1e0(i, 3);
        }
        Engine_ActorSetSpriteFlags(Engine_ActorGet(id), 0);
        actor->unknown_22 = 0;
        motion = &actor->motion_flags;
        *motion = 3;
        ((union FieldObject *)actor)->effect.velocity_y = 0x1999;
        ((union FieldObject *)actor)->effect.velocity_x = 0;
        CopyCells(6, 44, 1, 1, Data_0200d164[slot][0], Data_0200d164[slot][1]);
        Local_0200094c(actor);
        Engine_AudioPlayCue(188);
        actor->collision_flags = 0;
        *motion = 0;
        actor->y.fixed = -0x100000;
        Main_0808a1e0(id, 3);
        *flags = 2;
        CopyCells(73, 48, 1, 1, Data_0200d164[slot][0], Data_0200d164[slot][1]);
        Main_0808a1e0(0, priority);
        Engine_ActorGet(0)->priority_flags |= 1;
        for (i = 15; i <= 18; i++) {
            other = Engine_ActorGet(i);
            if (id != i
                && (actor->x.fixed >> 20) == (other->x.fixed >> 20)
                && (actor->z.fixed >> 20) - 1 == (other->z.fixed >> 20)) {
                Main_0808a1e0(i, 1);
                Engine_ActorGet(i)->priority_flags |= 1;
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
        d = Engine_ActorGet(18);
        if ((actor->priority_flags & b->priority_flags & c->priority_flags & d->priority_flags) & 2) {
            Camera_SetSpeed(0x10000, 0x2000);
            Engine_CameraMoveToActor(14, 1);
            Engine_CameraWaitForMove();
            c = Local_02000c5c(136, 0x308, Data_0200d77c);
            Engine_EventWait(30);
            Camera_SetSpeed(0x6666, 0xccc);
            Camera_MoveTo(0xd80000, -1, 0x2780000, 1);
            Main_08009148(c);
            Main_08009098(c, Data_0200d7c8);
            b = Local_02000c5c(216, 0x2f8, Data_0200dac8);
            flags = &c->rise_enabled;
            while (*(s32 *)c != 0 || *(s32 *)b != 0) {
                if (*flags != 0 || b->rise_enabled != 0) {
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
