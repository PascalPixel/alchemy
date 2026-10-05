#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The fields of an effect object this step reads. */
struct TimedObject {
    u8 unknown_00[0x64];
    s16 age;
    u8 unknown_66[6];
    void *update;
};

/* Counts the object's age; at 80 frames it stops this step and plays cue
   136. */
void BattleFx_AgeAndCueAtEighty(struct TimedObject *object)
{
    if (object->age == 80) {
        object->update = 0;
        Audio_PlayCue(136);
    }
    object->age++;
}
