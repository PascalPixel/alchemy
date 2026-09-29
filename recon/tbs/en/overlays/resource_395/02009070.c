/* Draft of resource_395 0x02009070..0x020091e8 (376 bytes with pool),
 * KorimaKi_PrepareActors, the overlay's first entry; the listing keeps the
 * rows. Remaining difference: the reference loads 40 and 0 from its literal
 * pool, link-time values; integers are immediates (372 bytes, 175 differ
 * from +0x31). */
#include "TYPES.H"
#include "FIELD_EVENT.H"


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
    *(s32 *)(*(u8 **)&gEventWork + 0x1c0) = 0x204;
    ((u16 *)&gGameState)[288] = 40;
    ((u16 *)&gGameState)[289] = 4;
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
