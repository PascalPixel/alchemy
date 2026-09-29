/* Korima's tree: the overlay's first entry stages the tree actors and sets
 * Retreat to return the party to the Korima bridge. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"

void PaletteScene_AdjustPaletteWindow(s32 step);

/* Stage the tree actors: sizes, sprite flags and priorities, heights and collision. */
s32 KorimaKi_PrepareActors(void)
{
    struct FieldActor *first;
    struct FieldActor *third;
    struct FieldActor *second;
    u8 zero;

    first = Engine_ActorGet(10);
    third = Engine_ActorGet(14);
    second = Engine_ActorGet(11);
    Engine_TaskWait(1);
    Engine_ActorSetChildValue(14, 15);
    gEventWork->start_transition = 0x204;
    gGameState.retreat_scene = (s32)&SceneId_KorimaHashi;
    gGameState.retreat_entrance = 4;
    /* FAKEMATCH: the heights are cleared through a u8 local zero, which the
     * compiler loads from the pool. */
    zero = 0;
    if (!Engine_GameFlagIsSet(0x845))
        PaletteScene_AdjustPaletteWindow(3);
    Engine_ActorGet(8)->radius = 6;
    Engine_ActorGet(9)->radius = 6;
    Engine_ActorGet(12)->radius = 6;
    Engine_ActorGet(13)->radius = 6;
    Engine_ActorSetSpriteFlags(Engine_ActorGet(14), 0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(10), 0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(11), 0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(8), 0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(9), 0);
    Engine_ActorSetSpritePriority(8, 2);
    Engine_ActorSetSpritePriority(14, 2);
    Engine_ActorSetSpritePriority(9, 2);
    first->motion_flags = zero;
    first->y.fixed = 0x1c0000;
    second->motion_flags = zero;
    second->y.fixed = 0x1c0000;
    third->motion_flags = zero;
    third->y.fixed = 0x1c0000;
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorGet(8)->collision_flags |= 8;
    Engine_ActorGet(9)->collision_flags |= 8;
    Engine_ActorGet(10)->collision_flags |= 8;
    Engine_ActorGet(11)->collision_flags |= 8;
    Engine_ActorGet(14)->collision_flags |= 8;
    return 0;
}
