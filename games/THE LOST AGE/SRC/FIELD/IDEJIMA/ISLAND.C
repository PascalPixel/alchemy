#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/*
 * Idejima after the tidal wave: the scene's tables, the scanline wave on the
 * sea, the drifting island and the steps of the scene where Felix wakes.
 */

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

struct FieldWork {
    u8 unknown_00[0x20];
    struct MapWork *map;
};

#define FIELD_WORK ((struct FieldWork *)0x03000000)

extern const u8 gSpawnedObjectScript[];
extern s16 gScanlineWavePage;
extern s32 gScanlineWaveCount;

void ScanlineWave_Reset(void)
{
    gScanlineWavePage = 0;
    gScanlineWaveCount = 0;
}

/* Before the island drifts: sets two map layer values, copies one map cell and
   gives actor 9 animation 2. */
void DriftScene_Prepare(void)
{
    struct MapLayer *layer = &FIELD_WORK->map->layer;

    layer->unknown_18 = 0x4000;
    layer->unknown_1c = 0x2000;
    Map_CopyCellsTo(72, 10, 75, 8, 1, 1);
    Actor_SetAnimation(9, 2);
}

/* Creates object 0x1e8 at a position, plays cue 0x97 and starts its script. */
void SceneObject_Spawn(s32 x, s32 y, s32 z, s16 value)
{
    struct FieldActor *object;
    struct FieldSprite *sprite;

    object = Object_Create(0x1e8, x, y, z);
    if (object != 0) {
        sprite = object->sprite;
        Audio_PlayCue(0x97);
        Object_SetMode(object, 1);
        Object_SetScript(object, gSpawnedObjectScript);
        object->unknown_55 = 0;
        sprite->unknown_1a = 0;
        sprite->unknown_12 = value;
    }
}
