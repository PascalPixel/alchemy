/* Near misses of overlay 679, one draft (alchemy diff --overlay resource_679 --symbol NAME):
 * Func_02000ef8, differ 40: the native shares one tail store between the two arms and
 *   keeps the offset and blend values in separate registers; untried beyond the plain form.
 * Func_02000f58, differ 4: two pool loads sit one instruction earlier in the native
 *   (ldr r0 before movs r3, #181; ldr r3 before movs r1, #2).
 * Func_02001054, differ 2 with the tagged devices: the native finishes the 0x200 argument
 *   (lsls r1) after the two small constants (movs r2, #8; movs r3, #9); here it follows
 *   movs r1 at once. sched2 breaks that tie by RTL order, and the precomputed argument
 *   comes first in ours.
 * Func_020010d8, differ 8: the terrain store and the sprite load, and the two priority
 *   byte loads, are in the other order.
 * Func_020017bc, differ 2 with the devices: adds r1, #4 comes before the ClearWords
 *   address load, where the native loads the address first (equal priority and
 *   dependent count; RTL order decides). Without the "+l" barrier that tie falls the
 *   native way but the size is started before the entry is stored. */
#include "../../../../games/tla/src/scene/apojii_islands/local.h"
#include "map.h"

void Func_02000ef8(void)
{
    if (REG_VCOUNT < gData_02002a98) {
        REG_BG1HOFS = gData_02002a9c;
        REG_BLDCNT = 0;
    } else {
        REG_BG1HOFS = gData_02002a9e;
        REG_BLDALPHA = 0x100c;
        REG_BLDCNT = 0x3f42;
    }
}

void Func_02000f58(void)
{
    struct MapWork *map = Ram_HeapSlots->mapWork;
    s16 *scroll = (s16 *)&map->layers[2];
    s32 hofs;

    gData_02002a98 = 0x5a8 - scroll[3];
    gData_02002a9c = hofs = scroll[1];
    gData_02002a9e = hofs - (gFrameCount >> 2);
}

void Func_02001054(void)
{
    struct PartyState *party;
    s16 *entrance;

    Scheduler_AddOrUpdateCallback(Func_020003e8, 0x480);
    Func_02001dfc();
    Func_02001e04(0, 0x200, 8, 9);
    Func_02001dec(170);
    Func_02000f98();
    party = &gPartyState;
    /* FAKEMATCH: the native loads the party state before it builds the entrance offset. */
    __asm__ volatile("" : "+l"(party));
    entrance = &party->entrance;
    if (*entrance == 3)
        Func_02001500();
    /* FAKEMATCH: the native reloads the entrance for each test; gcse shares one halfword load without the barrier. */
    __asm__ volatile("" : "+l"(entrance));
    if (*entrance == 4)
        Func_02001500();
    /* FAKEMATCH: the native reloads the entrance for each test; gcse shares one halfword load without the barrier. */
    __asm__ volatile("" : "+l"(entrance));
    if (*entrance == 5)
        Func_02001500();
}

void Func_020010d8(struct ObjectRuntime *object)
{
    struct ObjectRuntime *child = Engine_ObjectCreate(14, object->x, object->y + 0x40000, object->z + 0x40000);
    struct FieldSprite *parentSprite = object->animation;

    if (child != 0) {
        struct FieldSprite *sprite = child->animation;

        child->terrainHeight = object->terrainHeight;
        Func_02001c74_(child, gData_02001ee8);
        child->motionFlags = 0;
        if (sprite != 0) {
            Animation_ApplyChildArgument(sprite, 1);
            sprite->nextFrame = 0;
            sprite->priority = parentSprite->priority;
        }
    }
}

void Func_020017bc(void)
{
    s32 priority;

    u16 *work = gData_02002aa0;

    Iwram_ClearWords(work, 0x1004);
    *work = Resource_FindFreeEntry();
    /* FAKEMATCH: the native stores the entry before it starts the size (differ 2 without the barrier). */
    __asm__("" : "+m"(*(u8 *)work));
    VramCache_Load(*work, 0x200, 0);
    priority = 0x480;
    /* FAKEMATCH: the native builds the priority before loading the callback. */
    __asm__ volatile("" : "+r"(priority));
    Scheduler_AddOrUpdateCallback(Func_020015f8, priority);
}
