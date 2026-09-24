#include "TYPES.H"
#include "FIELD_EVENT.H"

void Main_08009190(s32 value);
s32 Main_08077158(s16 *owners);
void Main_08077128(s32 owner);
void Main_08077318(void);
void Engine_PartyAddActiveOwner(s32 owner);
void FieldScene_RunMainCutsceneSequence(void);
void FieldScene_RunBranchingActorSequence(void);
void FieldScene_RunScene3b8SequenceB(void);

/* The stats an owner keeps: current HP and PP follow their maximums. */
struct OwnerState {
    u8 unknown_00[0x34];
    u16 max_hp;
    u16 max_pp;
    u16 hp;
    u16 pp;
};

struct OwnerState *Engine_OwnerGetState(s32 owner);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;
extern u8 Data_0000008b[];

/* Restore the whole party: every owner's HP and PP to their maximums, and
 * the three companions back into the active party. */
static __inline__ void Party_RestoreAll(void)
{
    s16 owners[8];
    struct OwnerState *state;
    s32 cnt;
    s32 i;

    cnt = Main_08077158(owners);
    for (i = 0; i < cnt; i++) {
        state = Engine_OwnerGetState(owners[i]);
        state->hp = state->max_hp;
        state->pp = state->max_pp;
        Main_08077128(owners[i]);
    }
    Engine_PartyAddActiveOwner(1);
    Engine_PartyAddActiveOwner(2);
    Engine_PartyAddActiveOwner(3);
    Main_08077318();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Babi's Palace entry: record the entrance flags, then outside area 0x8b restore the lighthouse-item scene and the guards, set the entrance selector and, arriving by entrance 99 or 98, restore the party and run its scene. */
s32 TorebiKyuden_ApplyEntryState(void)
{
    struct FieldActor *actor;
    struct FieldSprite *sprite;
    u8 *buf;
    s32 set;

    Main_08009190(1);
    Main_08009190(2);
    Main_08009190(4);
    if (Data_02000240_t.halves[225][0] == 90) {
        Engine_GameFlagSet(0x962);
    }
    if (Data_02000240_t.halves[225][0] == 91) {
        Engine_GameFlagSet(0x962);
        Engine_GameFlagSet(0x950);
    }
    if (Data_02000240_t.halves[224][0] != (s32)Data_0000008b) {
        if (Data_02000240_t.halves[225][0] == 11) {
            Engine_GameFlagClear(0x12f);
        }
        if (Engine_GameFlagIsSet(0x950)) {
            set = Engine_GameFlagIsSet(0xf31);
            if (set) {
                Engine_ActorSetPosition(16, 0, 0);
            } else {
                actor = Engine_ActorGet(16);
                actor->unknown_5c = 1;
                actor->motion_flags = set;
                sprite = actor->sprite;
                actor->y.fixed = 0x40000;
                sprite->part_count = set;
                {
                    s32 v = -33;

                    v &= ((u8 *)sprite)[5];
                    ((u8 *)sprite)[5] = v;
                }
                sprite->palette = 0;
                buf = Engine_HeapAllocate(17, 0x608);
                Engine_ItemLoadIcon(205);
                Engine_VramLoad(sprite->vram_block, 128, buf + 0x400);
                Engine_HeapRelease(17);
            }
            if (Data_02000240_t.halves[225][0] == 33 && !Value1(Engine_GameFlagIsSet, 0x96f)) {
                Call1(Engine_GameFlagSet, 0x96f);
                Call3(Engine_ActorSetPosition, 14, 0xd00000, 0x2c00000);
                FieldScene_RunMainCutsceneSequence();
            }
            Engine_ActorSetAnimation(14, 5);
            Engine_ActorSetSpriteFlags(Engine_ActorGet(14), 0);
        } else if (Engine_GameFlagIsSet(0x962) && !Engine_GameFlagIsSet(0x966)) {
            Call3(Engine_ActorSetPosition, 10, 0x780000, 0x480000);
        }
        gEventWork->start_transition = 0x209;
        Engine_ActorGet(9)->collision_flags |= 4;
        if (Data_02000240_t.halves[225][0] == 99) {
            Party_RestoreAll();
            FieldScene_RunBranchingActorSequence();
            Data_02000240_t.halves[225][0] = 8;
        }
        if (Data_02000240_t.halves[225][0] == 98) {
            Party_RestoreAll();
            Engine_GameFlagSet(0x966);
            Engine_GameFlagSet(0x967);
            Call3(Engine_ActorSetPosition, 10, 0x380000, 0x780000);
            Engine_ActorFaceDirection(10, 0xf000, 0);
            FieldScene_RunScene3b8SequenceB();
            Data_02000240_t.halves[225][0] = 8;
        }
    }
    return 0;
}
