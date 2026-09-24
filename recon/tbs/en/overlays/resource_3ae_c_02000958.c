/* NONMATCHING: 378 of 378 bytes, 3 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: the QImode zero from the pool (its 32-byte pool range forces the mid-function pool) lands in r6 as in the reference only when assigned before Engine_ActorGet(11), and then its load stays before that call; the reference loads it right after the call (3 halfwords). */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void FieldScene_PlaceSlots14And15(void);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;
extern u8 Data_00000000[];

/* The sprite's attribute bytes as the scene scripts write them. */
struct SpriteBytes {
    u8 unknown_00[9];
    u8 priority;
    u8 unknown_0a[11];
    u8 second_priority;
    u8 unknown_16[8];
    u16 rotation;
    u8 unknown_20[6];
    u8 flags;
};

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Local_02000958(void)
{
    struct FieldActor *actor;
    u8 zero;

    FieldScene_PlaceSlots14And15();
    if (Value1(Engine_GameFlagIsSet, 0x950)) {
        Engine_ActorSetChildValue(12, 2);
    }
    if (Data_02000240_t.halves[225][0] == 3) {
        Engine_GameFlagClear(0x12f);
    }
    if (Data_02000240_t.halves[225][0] == 1) {
        Engine_GameFlagClear(0x8aa);
    }
    if (Engine_GameFlagIsSet(0x8aa)) {
        Call3(Engine_ActorSetPosition, 8, 0x1980000, 0x1280000);
        Call3(Engine_ActorFaceDirection, 8, 0x8000, 0);
    }
    if (Engine_GameFlagIsSet(0x8ab)) {
        Call3(Engine_ActorSetPosition, 13, 0x1180000, 0x1280000);
        Call3(Engine_ActorFaceDirection, 13, 0xc000, 0);
        Call3(Engine_ActorSetPosition, 16, 0x1200000, 0x1180000);
        Call3(Engine_ActorFaceDirection, 16, 0xe000, 0);
        Call3(Engine_ActorSetPosition, 10, 0xe80000, 0x1300000);
        Call3(Engine_ActorFaceDirection, 10, 0x4000, 0);
        Call3(Engine_ActorSetPosition, 11, 0xf00000, 0x1380000);
        Call3(Engine_ActorFaceDirection, 11, 0xc000, 0);
        actor = Engine_ActorGet(10);
        actor->collision_flags = 0;
        actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
        ((u8 *)actor->sprite)[9] |= 12;
        ((u8 *)actor->sprite)[38] = 0;
        zero = (u32)Data_00000000;
        ((struct SpriteBytes *)actor->sprite)->rotation = 0xc000;
        actor = Engine_ActorGet(11);
        {

            actor->priority_flags = zero;
        }
        ((u8 *)actor->sprite)[9] |= 12;
        ((u8 *)actor->sprite)[21] |= 12;
    }
    if (Value1(Engine_GameFlagIsSet, 0x950)) {
        Call6(Engine_MapCopyCellAttributes, 18, 18, 1, 1, 14, 18);
        Call6(Engine_MapCopyCellAttributes, 18, 18, 1, 1, 15, 18);
    }
}
