/* resource_3b8:0200c0b4..0200c340 (652 bytes with pool), still linked from
 * the listing. Remaining difference: the scene test loads 0x8b from the
 * literal pool (ldr r3, =0x8b; cmp r2, r3), a link-time value; an integer
 * scene compares with an immediate, and the function comes out 648 bytes
 * with 377 differing from +0x4f on. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void Map_SetLayerEntryFlagFar(s32 value);
s32 Party_ListActiveOwnersFar(s16 *owners);
void Owner_RecalculateRatiosFar(s32 owner);
void InventorySnapshot_RestoreFar(void);
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


/* Restore the whole party: every owner's HP and PP to their maximums, and
 * the three companions back into the active party. */
static __inline__ void Party_RestoreAll(void)
{
    s16 owners[8];
    struct OwnerState *state;
    s32 cnt;
    s32 i;

    cnt = Party_ListActiveOwnersFar(owners);
    for (i = 0; i < cnt; i++) {
        state = Engine_OwnerGetState(owners[i]);
        state->hp = state->max_hp;
        state->pp = state->max_pp;
        Owner_RecalculateRatiosFar(owners[i]);
    }
    Engine_PartyAddActiveOwner(1);
    Engine_PartyAddActiveOwner(2);
    Engine_PartyAddActiveOwner(3);
    InventorySnapshot_RestoreFar();
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

    Map_SetLayerEntryFlagFar(1);
    Map_SetLayerEntryFlagFar(2);
    Map_SetLayerEntryFlagFar(4);
    if (gGameState.entrance == 90) {
        Engine_GameFlagSet(0x962);
    }
    if (gGameState.entrance == 91) {
        Engine_GameFlagSet(0x962);
        Engine_GameFlagSet(0x950);
    }
    if (gGameState.scene != 0x8b) {
        if (gGameState.entrance == 11) {
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
            if (gGameState.entrance == 33 && !Value1(Engine_GameFlagIsSet, 0x96f)) {
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
        if (gGameState.entrance == 99) {
            Party_RestoreAll();
            FieldScene_RunBranchingActorSequence();
            gGameState.entrance = 8;
        }
        if (gGameState.entrance == 98) {
            Party_RestoreAll();
            Engine_GameFlagSet(0x966);
            Engine_GameFlagSet(0x967);
            Call3(Engine_ActorSetPosition, 10, 0x380000, 0x780000);
            Engine_ActorFaceDirection(10, 0xf000, 0);
            FieldScene_RunScene3b8SequenceB();
            gGameState.entrance = 8;
        }
    }
    return 0;
}
