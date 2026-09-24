#include "TYPES.H"
#include "SCENE.H"

struct Object_080216b4 {
    struct Object_080216b4 *next;
    u8 filler4[4];
    u8 source;
    u8 filler9[11];
    u8 destination;
};

/* Frame counter advanced by the vertical-blank interrupt. */
extern volatile u32 gFrameTick;
extern const u8 gRom[];

void Ui_ApplyTableOffsetToPair(struct Object_080216b4 *obj)
{
    s32 value;

    value = obj->source + gRom[(gFrameTick >> 2) & 7];
    obj->destination = value;
    obj = obj->next;
    value = obj->source + gRom[(gFrameTick >> 2) & 7];
    obj->destination = value;
}
