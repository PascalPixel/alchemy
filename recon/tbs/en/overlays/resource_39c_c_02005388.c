/* NONMATCHING: 200 of 208 bytes, 29 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: instructions match; the reference loads the first flags zero from a pool it dumps mid-function (after the second ObjectSetScript) while this draft pools at the end, and one byte store is scheduled before str r7,[r5,#104]. Twin of resource_39b:02001df8 (29 edits). */
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
extern u8 Data_00000000[];
extern s32 MakyuriHeya_SparkScript[];

void Local_02005388(void)
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
            sprite->flags = (u32)Data_00000000;
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
        spark->priority_flags = 2;
        *(struct SpawnPoint **)&spark->unknown_68 = point;
        if (sprite != NULL) {
            Main_08009020(sprite, 1);
            sprite->flags = (u32)Data_00000000;
        }
    }
    Engine_AudioPlayCue(130);
}
