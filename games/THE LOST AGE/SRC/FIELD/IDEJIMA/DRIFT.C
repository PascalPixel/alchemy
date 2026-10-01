#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "RAM_BUFFER.H"

/* The drifting island: its map set-up; DRIFT_SPAWN.C throws up its objects. */

struct MapLayer {
    u8 unknown_00[6];
    s16 y;
    u8 unknown_08[0x10];
    s32 unknown_18;
    s32 unknown_1c;
};

struct MapWork {
    u8 unknown_000[0x108];
    struct MapLayer layer;
};

/* Before the island drifts: sets two map layer values, copies one map cell and
   gives actor 9 animation 2. */
void DriftScene_Prepare(void)
{
    struct MapLayer *layer = &Ram_HeapSlots->map_work->layer;

    layer->unknown_18 = 0x4000;
    layer->unknown_1c = 0x2000;
    Engine_MapCopyCellsTo(72, 10, 75, 8, 1, 1);
    Engine_ActorSetAnimation(9, 2);
}
