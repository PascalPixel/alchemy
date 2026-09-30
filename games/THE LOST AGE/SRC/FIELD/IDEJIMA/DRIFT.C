#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "RAM_BUFFER.H"

/* The drifting island: its map set-up and the objects it throws up. */

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

extern const u8 gIdejimaSpawnScript[];

void Engine_MapCopyCellsTo(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y,
                           s32 width, s32 height);
struct FieldActor *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
void Engine_ObjectSetMode(struct FieldActor *object, s32 mode);
void Engine_ObjectSetScript(struct FieldActor *object, const void *script);

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

/* Creates object 0x1e8 at a position, plays cue 0x97 and starts its script. */
void DriftScene_SpawnObject(s32 x, s32 y, s32 z, s16 value)
{
    struct FieldActor *object;
    struct FieldSprite *sprite;

    object = Engine_ObjectCreate(0x1e8, x, y, z);
    if (object != 0) {
        sprite = object->sprite;
        Engine_AudioPlayCue(0x97);
        Engine_ObjectSetMode(object, 1);
        Engine_ObjectSetScript(object, gIdejimaSpawnScript);
        object->motion_flags = 0;
        sprite->unknown_1a = 0;
        sprite->unknown_12 = value;
    }
}
