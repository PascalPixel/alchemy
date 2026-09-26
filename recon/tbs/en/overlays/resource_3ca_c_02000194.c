/* NONMATCHING: 556 bytes, candidate 558, 244 differing halfwords, 43 aligned
 * edits (2026-09-26). Reconstructed both Q16 waves and all four actor-height
 * blocks. The complete topology now agrees. Remaining: halfword scroll
 * reload and stack-slot ownership, saved position pointer, and spawn stores.
 * Corrected BLDALPHA and all six neighbouring base-height addresses from
 * the raw listing's literal words. An addressable scroll array restores the
 * +18 slot and its address scheduling. Reusing bob for the reload keeps
 * an unwanted r6 copy; position-pointer and x lifetimes still differ.
 * Earlier structural attempts are preserved in draft commits. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IWRAM_CALL.H"

struct MapLayer {
    s32 unknown_00[3];
    s32 y;
    s32 unknown_10[8];
};

struct MapWork {
    u8 unknown_00[20];
    struct MapLayer layers[8];
};

struct Position {
    s32 x, y, z;
};

struct PositionWork {
    struct Position *pos;
};

struct Half {
    u16 v;
};

extern struct MapWork *Data_03001e70;
extern s32 Data_020097e8;
extern s32 Data_020097ec;
extern s32 Data_020097f0;
extern s32 Data_020097f8;
extern s32 Data_020097fc;
extern s32 Data_02009800;
extern s32 Data_02009804;
extern s32 Data_02009808;
extern s32 Data_0200980c;
extern s32 Data_02009810;
extern s32 Data_02009814;
extern s32 Data_02009818;

void BabiFune_UpdateDriftingObject(u8 *obj);

void BabiFune_UpdateWaves(void)
{
    volatile u16 scroll[1];
    struct Half zero;
    struct Position buf;
    struct PositionWork work;
    struct MapWork *map;
    struct FieldActor *actor;
    s32 bob;
    s32 x, z;

    map = Data_03001e70;
    if (Data_020097e8 != 0) {
        bob = Iwram_MulQ16(Engine_MathSin(Data_020097ec << 9), 3);
        /* FAKEMATCH: explicit halfword accesses retain truncation before I/O. */
        scroll[0] = Data_020097f0 + ((bob + 8) << 8);
        bob = scroll[0];
        *(volatile u16 *)0x04000052 = bob;
        Data_020097ec++;
    }
    if (Data_020097fc != 0) {
        bob = Iwram_MulQ16(Engine_MathSin(Data_02009800 << 9), 2) << 16;
        map->layers[6].y = Data_02009804 + bob;
        map->layers[7].y = Data_02009808 + bob;
        if (Data_0200980c != -0x10000) {
            actor = Engine_ActorGet(0);
            actor->y.fixed = Data_0200980c + bob;
            *(s32 *)actor->unknown_14 = Data_0200980c + bob;
            actor->motion_flags = 0;
        }
        if (Data_02009810 != -0x10000) {
            actor = Engine_ActorGet(1);
            actor->y.fixed = Data_02009810 + bob;
            *(s32 *)actor->unknown_14 = Data_0200980c + bob;
            actor->motion_flags = 0;
        }
        if (Data_02009814 != -0x10000) {
            actor = Engine_ActorGet(3);
            actor->y.fixed = Data_02009814 + bob;
            *(s32 *)actor->unknown_14 = Data_0200980c + bob;
            actor->motion_flags = 0;
        }
        if (Data_02009818 != -0x10000) {
            actor = Engine_ActorGet(2);
            actor->y.fixed = Data_02009818 + bob;
            *(s32 *)actor->unknown_14 = Data_0200980c + bob;
            actor->motion_flags = 0;
        }
        Data_02009800++;
    }
    if (Data_020097f8 != 0 && (gFrameCount & 1) != 0) {
        x = map->layers[4].unknown_10[0] & -0x10000;
        z = map->layers[4].unknown_10[1] & -0x10000;
        x += Engine_RandomNext() * 240;
        work.pos = &buf;
        work.pos->y = 0;
        work.pos->x = x;
        z += Engine_RandomNext() * 160;
        z += 0x1e0000;
        work.pos->z = z;
        actor = Engine_ObjectCreate(0x1f7, work.pos->x, work.pos->y, z);
        if (actor != 0) {
            actor->update = (void (*)(union FieldObject *))BabiFune_UpdateDriftingObject;
            actor->unknown_64 = 60;
            /* FAKEMATCH: a halfword zero retains the short literal-pool reach. */
            zero.v = 0;
            actor->unknown_66 = 1;
            actor->motion_flags = zero.v;
            actor->priority_flags = 2;
            actor->sprite->priority = 2;
            Engine_ObjectSetBlendMode(actor, 0);
            Engine_ObjectSetAnimation(actor, 0);
        }
    }
}
