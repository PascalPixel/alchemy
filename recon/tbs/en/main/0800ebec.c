/* Draft, not exact (2026-09-24): 1,790 of 1,804 bytes, 77.5% aligned
 * similarity; every call site is in the ROM's order. Rewritten from the ROM:
 * the four delay loops are goto loops (a do/while is unrolled away); the
 * timer and footprint-phase stores go through the actor's members so the
 * constants become movs; FieldObject_Create takes the kind first; the ungated
 * path squares the x and z velocity through Iwram_MulQ16, takes FixedSqrt
 * and redirects it along the facing. Remaining: the ROM keeps angle << 16 in
 * [sp+4] and derives (u32)angle >> 16 afresh after each join, where GCSE
 * here keeps one copy (so r6/r8/fp/r9 roles shift); the footprint phase is
 * read twice (ldrsh for == 2, ldrh for the flip) with the flip's zero from
 * the pool; mode and facing trade [sp+8]/[sp+12] with declaration order.
 * Tried: angle = the raw s16 table value with every use spelled (u16)angle
 * (and the clamp's difference through its own local) reproduces [sp+4] =
 * angle << 16 and the 104-byte frame, but the u16 is still carried in r6
 * across the first probe (758 halfwords); (void)&angle, an s16 angle and
 * u16 or s16 spellings of the first test all regress. The search loop exits
 * with cmp #6; blt in the ROM where combine gives cmp #5; ble here; i <= 5,
 * i - 6 < 0, (u32)i < 6, a sizeof bound and a goto loop all keep ble.
 * 2026-09-29 (alchemy permute scorer): the draft scored 6285 (73
 * register-only, 19 operand, 23 reordered, 19 inserted, 22 deleted). This body
 * is the permuter's best after a 300-second search (about 40,000 candidates):
 * 5300 (67 register-only, 14 operand, 24 reordered, 12 inserted, 20
 * deleted). Its rewrites are search output, not a
 * reading of the ROM; the allocation above is still the difference.
 */
#include "TYPES.H"
#include "IWRAM_CALL.H"

/* One 0x70-byte field object: the player's actor and the entries of the
 * object table it is tested against. */
struct FieldActor {
    void *data;                 /* 0x00 */
    u16 tick;                   /* 0x04 */
    u16 facing;                 /* 0x06 */
    s32 pos[6];                 /* 0x08 x, y, z, ... */
    u16 radius;                 /* 0x20 */
    u8 unknown_22;
    u8 kind;                    /* 0x23 */
    s32 velocity_x;             /* 0x24 */
    s32 velocity_y;             /* 0x28 */
    s32 velocity_z;             /* 0x2c */
    s32 speed;                  /* 0x30 */
    s32 accel;                  /* 0x34 */
    s32 target[3];              /* 0x38 */
    u8 unknown_44[0x0c];
    struct FieldSprite *sprite; /* 0x50 */
    u8 unknown_54;
    u8 blocked;                 /* 0x55 */
    u8 unknown_56[2];
    union {
        u32 word;               /* 0x58 */
        u8 bytes[4];            /* 0x59 bit 0: solid */
    } flags;
    u8 unknown_5c[8];
    s16 step_timer;             /* 0x64 */
    u16 step_phase;             /* 0x66 */
    u8 unknown_68[8];
};

struct FieldSprite {
    u8 unknown_00[9];
    u8 flags;                   /* 0x09 */
    u8 unknown_0a[0x14];
    u16 angle;                  /* 0x1e */
    u8 unknown_20[6];
    u8 blocked;                 /* 0x26 */
};

struct CameraState {
    u8 unknown_00[0x17];
    u8 footprints;              /* 0x17 */
};

struct PlayerState {
    u8 unknown_000[0x1f4];
    s32 leader;                 /* 0x1f4 */
    u8 unknown_1f8[0x24];
    u16 dash_keys;              /* 0x21c */
};

struct OwnerState {
    u8 unknown_00[0x38];
    s16 hp;                     /* 0x38 */
};

extern volatile u8 gDebugMode;
extern u8 ResourceBlockOwners[];
extern volatile u32 gKeysHeld;
extern struct PlayerState gGameState;
extern s16 Data_08013254[];
extern s16 Data_08013274[];
extern struct FieldActor *gObjectSlots;
extern u16 *gEventWork;
extern struct CameraState *gMapWork;

s32 GameFlag_TestFar(s32 flag);
void Audio_PlayCue(s32 cue);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *pos);
s32 Func_080120dc(struct FieldActor *actor, s32 *pos);
s32 Runtime_CheckRadiusOverlap(s32 *a, s32 ra, s32 *b, s32 rb);
s32 ScriptObject_CheckOverlap(struct FieldActor *actor, s32 *pos);
s32 ArcTan2(s32 y, s32 x);
void ObjectDispatch_ApplyArgumentToChildren(struct FieldActor *actor, s32 argument);
void ObjectDispatch_Initialize(struct FieldActor *actor, s16 *table);
struct FieldActor *FieldObject_Create(s32 kind, s32 x, s32 y, s32 z);
void AnimationObjects_SelectAnimation(struct FieldSprite *sprite, s32 animation);
void Object_SetMoveTarget(struct FieldActor *actor, s32 x, s32 y, s32 z);
struct OwnerState *Owner_GetStateFar(s32 id);
s32 FixedSqrt(s32 value);
void Field_CheckConfiguredKeys(void);

s32 FieldObject_UpdatePlayerControl(struct FieldActor *actor)
{
    /* FAKEMATCH: permuter found a cast to the operand's own type, a cast of a cast or (*p).member */
    s32 posA[3];
    register s32 posB[3];
    s32 posC[3];
    s32 unused[8];
    s16 deltas[6];
    s32 blocked;
    s32 handled;
    s16 facing;
    s32 angle;
    s32 mode;
    s32 dir;
    s32 i;
    struct FieldActor *entry;
    s16 *timer;
    u16 tmp;
    u16 tmp2;

    blocked = 0;
    handled = 0;
    if (gDebugMode != 0 && GameFlag_TestFar(350) != 0) {
        u8 *p;
        s32 count;
        s32 n;
        count = 0;
        p = ResourceBlockOwners;
        n = 512;
        while (1) {
            u8 v;
            v = *p;
            p++;
            if (v == 0xff)
                count++;
            --n;
            if (n == 0)
                break;
        }
        if (count - 136 < 0)
            Audio_PlayCue(135);
    }
    if (gDebugMode != 0) {
        s32 mask;
        mask = 0x200;
        if (gKeysHeld & mask) {
            s32 count;
            count = mask;
        wait_a:
            --count;
            if (count != 0)
                goto wait_a;
            count = 95;
        wait_b:
            count--;
            if (count >= 0)
                goto wait_b;
            count = 63;
        wait_c:
            count--;
            if (count >= 0)
                goto wait_c;
            count = 63;
        wait_d:
            count--;
            if (count >= 0)
                goto wait_d;
        }
    }
    if (gKeysHeld & gGameState.dash_keys) {
        actor->speed = 0x18000;
        actor->accel = 0x4000;
        mode = 5;
    } else {
        actor->speed = 0x10000;
        actor->accel = 0x4000;
        mode = 2;
    }
    if (GameFlag_TestFar(0x17f) != 0 && (gKeysHeld & 2)) {
        actor->speed = 0x40000;
        actor->accel = 0x10000;
        mode = 5;
    }
    angle = Data_08013254[(gKeysHeld >> 4) & 15] << 16;
    tmp2 = (u16)((u32)angle >> 16);
    if (tmp2 == 0xffff) {
        blocked |= 4;
        goto tail;
    }
    posA[0] = actor->pos[0];
    posA[1] = actor->pos[1];
    posA[2] = actor->pos[2];
    blocked = 0;
    tmp = (u16)((u32)angle >> 16);
    Vector_AddPolarOffset(0x80000, tmp, posA);
    if (gDebugMode != 0) {
        facing = angle >> 16;
        if (gKeysHeld & 0x200)
            goto tail;
    }
    if (Func_080120dc(actor, posA) != 0)
        goto search;
    posB[0] = actor->pos[0];
    posB[1] = actor->pos[1];
    posB[2] = actor->pos[2];
    Vector_AddPolarOffset(0x80000, 0x1000 + (u16)((u32)angle >> 16), posB);
    if (Func_080120dc(actor, posB) != 0)
        goto search;
    posB[0] = actor->pos[0];
    posB[1] = actor->pos[1];
    posB[2] = actor->pos[2];
    Vector_AddPolarOffset(0x80000, (u16)((u32)angle >> 16) - 0x1000, posB);
    if (Func_080120dc(actor, posB) != 0)
        goto search;
    posB[0] = actor->pos[0];
    posB[1] = actor->pos[1];
    posB[2] = actor->pos[2];
    Vector_AddPolarOffset(0x80000, (u16)((u32)angle >> 16) + 0x2000, posB);
    if (Func_080120dc(actor, posB) != 0)
        goto search;
    posB[0] = actor->pos[0];
    posB[1] = actor->pos[1];
    posB[2] = ((s32 *)actor->pos)[2];
    Vector_AddPolarOffset(0x80000, (u16)((u32)angle >> 16) - 0x2000, posB);
    if (Func_080120dc(actor, posB) != 0)
        goto search;
    facing = angle >> 16;
    goto move;
search:
    dir = (u16)((u32)angle >> 16);
    deltas[0] = dir + 0x1000;
    deltas[1] = dir - 0x1000;
    deltas[2] = dir + 0x2000;
    deltas[3] = dir - 0x2000;
    deltas[4] = dir + 0x3000;
    deltas[5] = dir - 0x3000;
    for (i = 0; i < 6; i++) {
        facing = deltas[i];
        posA[0] = actor->pos[0];
        posA[1] = actor->pos[1];
        posA[2] = actor->pos[2];
        Vector_AddPolarOffset(0x80000, (u16)facing, posA);
        if (Func_080120dc(actor, posA) != 0)
            continue;
        posB[0] = actor->pos[0];
        posB[1] = actor->pos[1];
        posB[2] = actor->pos[2];
        Vector_AddPolarOffset(0x80000, (u16)facing + 0x1000, posB);
        if (Func_080120dc(actor, posB) != 0)
            continue;
        posB[0] = actor->pos[0];
        posB[1] = actor->pos[1];
        posB[2] = actor->pos[2];
        Vector_AddPolarOffset(0x80000, (u16)facing - 0x1000, posB);
        if (Func_080120dc(actor, posB) != 0)
            continue;
        posB[0] = actor->pos[0];
        posB[1] = actor->pos[1];
        posB[2] = actor->pos[2];
        Vector_AddPolarOffset(0x80000, (u16)facing + 0x2000, posB);
        if (Func_080120dc(actor, posB) != 0)
            continue;
        posB[0] = actor->pos[0];
        posB[1] = actor->pos[1];
        posB[2] = actor->pos[2];
        Vector_AddPolarOffset(0x80000, (u16)facing - 0x2000, posB);
        if (Func_080120dc(actor, posB) == 0)
            goto move;
    }
    posA[0] = actor->pos[0];
    posA[1] = actor->pos[1];
    posA[2] = actor->pos[2];
    blocked |= 1;
move:
    posC[0] = actor->pos[0];
    posC[1] = actor->pos[1];
    posC[2] = actor->pos[2];
    Vector_AddPolarOffset(0x40000, (u16)facing, posC);
    entry = gObjectSlots;
    for (i = 63; i >= 0; i -= 1, entry++) {
        s32 push;
        s32 radius;
        radius = actor->radius - 2;
        if (entry->data == 0)
            continue;
        if (!(entry->flags.bytes[1] & 1))
            continue;
        if (entry == actor)
            continue;
        if (Runtime_CheckRadiusOverlap(entry->pos, radius, posC, entry->radius - 2) < 0)
            continue;
        if ((entry->flags.word & 0xff000200) != 0x200)
            goto push_blocked;
        push = ArcTan2(entry->pos[2] - actor->pos[2], entry->pos[0] - actor->pos[0]);
        posB[0] = entry->pos[0];
        posB[1] = entry->pos[1];
        facing = push;
        push = (u16)push;
        posB[2] = entry->pos[2];
        Vector_AddPolarOffset(0x4000, push, posB);
        if (ScriptObject_CheckOverlap(entry, posB))
            goto push_blocked;
        posB[0] = entry->pos[0];
        posB[1] = entry->pos[1];
        posB[2] = entry->pos[2];
        Vector_AddPolarOffset(0xa0000, push, posB);
        if (Func_080120dc(entry, posB) != 0)
            goto push_blocked;
        posB[0] = entry->pos[0];
        posB[1] = entry->pos[1];
        posB[2] = entry->pos[2];
        Vector_AddPolarOffset(0xa0000, push + 0x1000, posB);
        if (Func_080120dc(entry, posB) != 0)
            goto push_blocked;
        if (Func_080120dc(entry, posB) != 0)
            goto push_blocked;
        posB[0] = entry->pos[0];
        posB[1] = (*entry).pos[1];
        posB[2] = entry->pos[2];
        Vector_AddPolarOffset(0xa0000, push - 0x1000, posB);
        if (Func_080120dc(entry, posB) != 0)
            goto push_blocked;
        Vector_AddPolarOffset(0x4000, push, entry->pos);
        entry->target[0] = 0x80000000;
        entry->target[1] = 0x80000000;
        entry->target[2] = 0x80000000;
        handled |= 1;
        continue;
    push_blocked:
        blocked |= 2;
    }
    if (blocked == 0 && handled != 0) {
        actor->speed = 0x4000;
        actor->accel = 0x2000;
    }
tail:
    if (gEventWork != 0) {
        if (blocked & 3)
            ++gEventWork[206];
        else
            gEventWork[206] = 0;
    }
    if (handled != 0) {
        ObjectDispatch_ApplyArgumentToChildren(actor, 8);
    } else if (blocked != 0) {
        s32 kind;
        kind = 9;
        if (Owner_GetStateFar(gGameState.leader)->hp == 0)
            kind = 22;
        ObjectDispatch_ApplyArgumentToChildren(actor, kind);
    } else {
        ObjectDispatch_ApplyArgumentToChildren(actor, mode);
    }
    if (blocked != 0) {
        actor->target[0] = 0x80000000;
        actor->target[1] = 0x80000000;
        actor->target[2] = 0x80000000;
        actor->velocity_x = 0;
        actor->velocity_z = 0;
        if (blocked & 3) {
            s32 diff;
            if ((diff = (s16)((u16)((u32)angle >> 16) - actor->facing)) > 0x1000)
                diff = 0x1000;
            if (diff < -0x1000)
                diff = -0x1000;
            actor->facing += diff;
        }
        actor->step_timer = 0;
        actor->step_phase = 2;
    } else {
        s32 speed;
        Object_SetMoveTarget(actor, posA[0], posA[1], posA[2]);
        speed = FixedSqrt(Iwram_MulQ16(actor->velocity_x, actor->velocity_x) + Iwram_MulQ16(actor->velocity_z, actor->velocity_z));
        actor->velocity_x = 0;
        actor->velocity_z = 0;
        Vector_AddPolarOffset(speed, (u16)facing, &actor->velocity_x);
        if (actor->step_timer != 0)
            actor->step_timer--;
    }
    dir = (u16)((u32)angle >> 16);
    if (gMapWork->footprints != 0 && actor->step_timer == 0 && blocked == 0) {
        struct FieldActor *print;
        print = FieldObject_Create(25, actor->pos[0], actor->pos[1], actor->pos[2]);
        if (print != 0) {
            u16 *phase;
            struct FieldSprite *sprite;
            u16 flip;
            print->pos[3] = actor->pos[3];
            sprite = print->sprite;
            ObjectDispatch_Initialize(print, Data_08013274);
            print->kind = 2;
            print->blocked = blocked;
            if (sprite != 0) {
                AnimationObjects_SelectAnimation(sprite, 1);
                sprite->blocked = blocked;
                sprite->angle = dir + 0x4000;
                sprite->flags |= 0xc;
            }
            flip = actor->step_phase;
            if ((s16)actor->step_phase == 2) {
                AnimationObjects_SelectAnimation(sprite, 2);
                actor->step_phase = blocked;
                flip = 0;
            }
            if (flip != 0)
                print->facing = 0x8000;
            if (mode == 5)
                actor->step_timer = 12;
            else
                actor->step_timer = 18;
            actor->step_phase ^= 1;
        }
    }
    Field_CheckConfiguredKeys();
    actor->tick++;
    return 1;
}
