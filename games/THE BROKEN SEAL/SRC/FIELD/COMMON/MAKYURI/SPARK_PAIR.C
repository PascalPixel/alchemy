#include "TYPES.H"
#include "FIELD_EVENT.H"

void Main_08009020(struct FieldSprite *sprite, s32 mode);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

/* A spawn point: an object's position and its first update value. */
struct SpawnPoint {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 value;
};

struct EventSpawns {
    u8 unknown_00[20];
    struct SpawnPoint *points[1];
};

extern union GameStateRows Data_02000240_t;
extern s32 MakyuriHeya_SparkScript[];

/* Spawns the two sparks at the event's spawn point, one on each sprite layer, and plays their cue. */
/* Spawns the two sparks at the event's spawn point, one on each sprite layer, and plays their cue. */
/* Spawns the two sparks at the event's spawn point, one on each sprite layer, and plays their cue. */
/* Spawns the two sparks at the event's spawn point, one on each sprite layer, and plays their cue. */
void MakyuriHeya_SpawnSparkPair(void)
{
    struct SpawnPoint *point;
    struct FieldActor *spark;
    struct FieldSprite *sprite;

    point = (*(struct EventSpawns **)0x03001ebc)->points[Data_02000240_t.words[125]];
    spark = Engine_ObjectCreate(26, point->x, point->y, point->z);
    if (spark != NULL) {
        *(s32 *)spark->unknown_14 = point->value;
        sprite = spark->sprite;
        Engine_ObjectSetScript(spark, MakyuriHeya_SparkScript);
        spark->motion_flags = 0;
        spark->unknown_64 = 0;
        *(struct SpawnPoint **)&spark->unknown_68 = point;
        if (sprite != NULL) {
            Main_08009020(sprite, 2);
            sprite->flags = 0;
            sprite->priority = 1;
        }
    }
    spark = Engine_ObjectCreate(26, point->x, point->y, point->z);
    if (spark != NULL) {
        *(s32 *)spark->unknown_14 = point->value;
        sprite = spark->sprite;
        Engine_ObjectSetScript(spark, MakyuriHeya_SparkScript);
        spark->motion_flags = 0;
        spark->unknown_64 = 0;
        *(struct SpawnPoint **)&spark->unknown_68 = point;
        spark->priority_flags = 2;
        if (sprite != NULL) {
            Main_08009020(sprite, 1);
            sprite->flags = 0;
        }
    }
    Engine_AudioPlayCue(130);
}
