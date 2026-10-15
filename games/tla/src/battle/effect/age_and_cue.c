#include "types.h"
#include "field_event.h"

struct TimedObject {
    u8 unk00[0x64];
    s16 age;
    u8 unk66[6];
    void *update;
};

void BattleFx_AgeAndCueAtEighty(struct TimedObject *object)
{
    if (object->age == 80) {
        object->update = 0;
        Audio_PlayCue(136);
    }
    object->age++;
}
