#include "TYPES.H"
#include "MAKYURI.H"

void AnimationObjects_SelectAnimation(struct FieldSprite *sprite, s32 mode);

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

extern s32 MakyuriHeya_SparkScript[];

/* Spawns the two sparks at the event's spawn point, one on each sprite layer,
 * and plays their cue. */
void MakyuriHeya_SpawnSparkPair(void)
{
    struct SpawnPoint *point;
    struct FieldActor *spark;
    struct FieldSprite *sprite;

    point = (*(struct EventSpawns **)&gEventWork)->points[gGameState.selected_actor];
    spark = Engine_ObjectCreate(26, point->x, point->y, point->z);
    if (spark != NULL) {
        *(s32 *)spark->unknown_14 = point->value;
        sprite = spark->sprite;
        Engine_ObjectSetScript(spark, MakyuriHeya_SparkScript);
        spark->motion_flags = 0;
        spark->unknown_64 = 0;
        *(struct SpawnPoint **)&spark->unknown_68 = point;
        if (sprite != NULL) {
            AnimationObjects_SelectAnimation(sprite, 2);
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
            AnimationObjects_SelectAnimation(sprite, 1);
            sprite->flags = 0;
        }
    }
    Audio_PlayCue(130);
}
