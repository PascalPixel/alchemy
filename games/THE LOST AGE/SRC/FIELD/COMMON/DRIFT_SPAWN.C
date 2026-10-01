#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The drifting island throws these up; overlay 696 carries the same routine
   and script. */
extern const u8 DriftScene_SpawnScript[];

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
        Engine_ObjectSetScript(object, DriftScene_SpawnScript);
        object->motion_flags = 0;
        sprite->unknown_1a = 0;
        sprite->unknown_12 = value;
    }
}
