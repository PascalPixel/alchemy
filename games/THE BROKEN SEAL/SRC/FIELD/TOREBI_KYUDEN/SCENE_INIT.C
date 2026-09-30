/* The palace's scene start, entry veneer 0. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"

void Map_SetLayerEntryFlagFar(s32 value);
s32 Party_ListActiveOwners(s16 *owners);
void Owner_RecalculateRatios(s32 owner);
void InventorySnapshot_Restore(void);
void Party_AddActiveOwner(s32 owner);
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

struct OwnerState *Owner_GetState(s32 owner);

/* Restore the whole party: every owner's HP and PP to their maximums, and
 * the three companions back into the active party. */
static __inline__ void Party_RestoreAll(void)
{
    s16 owners[8];
    struct OwnerState *state;
    s32 cnt;
    s32 i;

    cnt = Party_ListActiveOwners(owners);
    for (i = 0; i < cnt; i++) {
        state = Owner_GetState(owners[i]);
        state->hp = state->max_hp;
        state->pp = state->max_pp;
        Owner_RecalculateRatios(owners[i]);
    }
    Party_AddActiveOwner(1);
    Party_AddActiveOwner(2);
    Party_AddActiveOwner(3);
    InventorySnapshot_Restore();
}

/* Babi's Palace entry: record the entrance flags, then outside the second scene restore the lighthouse-item scene and the guards, set the entrance selector and, arriving by entrance 99 or 98, restore the party and run its scene. */
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
    if (gGameState.scene != (s32)&SceneId_TorebiKyuden2) {
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
                sprite->full_color = 0;
                sprite->palette = 0;
                buf = Engine_HeapAllocate(17, 0x608);
                Engine_ItemLoadIcon(205);
                Engine_VramLoad(sprite->vram_block, 128, buf + 0x400);
                Engine_HeapRelease(17);
            }
            if (gGameState.entrance == 33 && !Value1(Engine_GameFlagIsSet, 0x96f)) {
                Engine_GameFlagSet(0x96f);
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
