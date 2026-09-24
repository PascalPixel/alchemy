/* NONMATCHING: 832 bytes, candidate 832, 1 differing halfword (2026-09-24).
 * Hand-written; twin of resource_39c:020055c0. Unit symbols beyond the resolved
 * Engine_ veneers: gPushAngles = 0x0200a7f4, Data_03001c94 (keys newly
 * pressed) and Data_03001ae8 (data). Reading the key latch through the
 * Data_03001c94 symbol fixed the spark-loop 15/address order. Remaining: the
 * collision call passes pos as add r1, sp, #16 in the reference, here as the
 * fp copy of &pos: PRE (gcse) turns insn "r131 = sfp-12" into a copy of the
 * first &pos pseudo, then cse2 propagates it (-fno-gcse removes the copy but
 * changes the allocation). Pointer variables, casts and every shape of the
 * early-return condition leave it; the reference's later pos uses (the
 * spawn position, the motion commit) are merged into fp as here. */
#include "TYPES.H"
#include "FIELD_EFFECT.H"
#include "IWRAM_CALL.H"

extern u8 Data_00000000[];

void Main_08000128(s32 radius, s32 angle, union FieldCoordinate *pos);
s32 Main_080091d8(struct FieldActor *actor, union FieldCoordinate *pos);
s32 Main_08009020(struct FieldSprite *sprite, s32 animation);
void OverlayObject_SpawnKind24AtActor(struct FieldActor *actor);

/* One cell of the height map: the third byte is the cell's floor level. */
struct MapCell {
    u8 unknown_0[2];
    u8 level;
    u8 unknown_3;
};

struct EventActors {
    u8 unknown_00[20];
    struct FieldActor *actors[1];
};

extern s16 gPushAngles[];
extern u32 Data_03001c94;
extern u32 Data_03001ae8;

struct PushState {
    s32 count;
    s32 level;
    s32 active;
    s32 target_x;
    s32 target_z;
    struct FieldActor *source;
    struct FieldActor *effect;
};

#define MAP_CELLS ((struct MapCell *)0x02010000)

void Local_02002030(void)
{
    u8 *work;
    struct PushState *state;
    struct FieldActor *actor;
    struct FieldActor *source;
    struct FieldActor *obj;
    struct FieldSprite *sprite;
    struct MapCell *from;
    struct MapCell *to;
    u8 saved;
    u8 *flags;
    s32 angle;
    s32 n;
    s32 i;
    u8 zero;
    union FieldCoordinate pos[3];

    state = **(struct PushState ***)((u8 *)&gEventWork + 32);
    work = (u8 *)gEventWork;
    actor = ((struct EventActors *)work)->actors[gGameState.selected_actor];
    flags = &actor->motion_flags;
    saved = *flags;
    angle = (u16)gPushAngles[(Data_03001ae8 >> 4) & 15];
    if (gPushAngles[(Data_03001ae8 >> 4) & 15] == -1)
        return;
    pos[0].fixed = (actor->x.fixed & -0x100000) + 0x80000;
    pos[1].fixed = *(s32 *)((u8 *)actor + 20);
    pos[2].fixed = (actor->z.fixed & -0x100000) + 0x80000;
    from = &MAP_CELLS[(pos[2].fixed / 0x100000 << 7) + pos[0].fixed / 0x100000];
    Main_08000128(0x200000, angle, pos);
    to = &MAP_CELLS[(pos[2].fixed / 0x100000 << 7) + pos[0].fixed / 0x100000];
    if (from->level != state->level && to->level == state->level && state->count == 0)
        return;

    Engine_EventBegin();
    if (Main_080091d8(actor, pos) != 0)
        return;
    obj = state->effect;
    if (obj != NULL) {
        ((union FieldObject *)obj)->effect.spin = 0;
        Engine_ObjectSetScript(obj, (const s32 *)0x0200a7dc);
        Engine_ObjectSetAnimation(obj, 7);
        state->effect = NULL;
    }
    if (to->level == state->level && state->count != 0) {
        source = state->source;
        obj = Engine_ObjectCreate(26, source->x.fixed, source->y.fixed, source->z.fixed);
        if (obj != NULL) {
            sprite = obj->sprite;
            *(s32 *)((u8 *)obj + 20) = *(s32 *)((u8 *)source + 20);
            Engine_ObjectSetScript(obj, (const s32 *)0x0200a7d0);
            obj->motion_flags = 0;
            ((union FieldObject *)obj)->effect.spin = 0;
            obj->priority_flags = 2;
            obj->speed = 0x40000;
            obj->acceleration = 0x20000;
            Engine_ObjectSetPosition(obj, pos[0].fixed, pos[1].fixed, pos[2].fixed);
            if (sprite != NULL) {
                Main_08009020(sprite, 6);
                zero = 0;
                ((u8 *)sprite)[38] = zero;
            }
            state->effect = obj;
        }
        n = state->count - 1;
        state->count = n;
        if (n == 0) {
            Engine_ObjectDispatchRelease(state->source);
            state->source = NULL;
            Engine_GameFlagClear(0x161);
        } else if (state->source != NULL) {
            Engine_ObjectSetAnimation(state->source, 6 - n);
        }
    }
    Engine_ObjectSetAnimation(actor, 6);
    Engine_TaskWait(3);
    Engine_AudioPlayCue(152);
    Engine_ObjectSetAnimation(actor, 7);
    actor->speed = 0x30000;
    actor->acceleration = 0x20000;
    actor->velocity_y = 0x40000;
    *flags &= 0x7e;
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ObjectMotionSetPositionAndCommit(0, pos[0].part.pixel, pos[2].part.pixel);
    Engine_ObjectSetAnimation(actor, 6);
    Engine_TaskWait(2);
    if (to->level != state->level)
        Engine_ActorSetSpriteFlags(actor, 1);
    else
        Engine_AudioPlayCue(215);
    Engine_TaskWait(1);
    *flags = saved;
    if (to->level == state->level && state->effect == NULL) {
        Engine_ObjectSetAnimation(actor, 18);
        Engine_AudioPlayCue(241);
        for (i = 0;; i++) {
            if ((i & 15) == 0)
                OverlayObject_SpawnKind24AtActor(actor);
            if (i > 31 && Data_03001c94 != 0)
                break;
            Engine_TaskWait(1);
        }
        Engine_AudioPlayCue(0x120);
        Engine_TaskWait(1);
        actor->x.fixed = state->target_x;
        actor->z.fixed = state->target_z;
        Engine_ActorSetSpriteFlags(actor, 1);
    }
    state->active = 0;
    Engine_EventEnd();
    *(s32 *)(work + 0x1b4) += Iwram_MulQ16(*(s32 *)(work + 0x1b0), 0x200000);
}
