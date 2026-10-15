// near miss: differ 17 (listing 31 instructions, draft 30); the native keeps the constant 0xffff0000 in r5 and finishes the first offset before the second random call
#include "../../../../games/tla/src/scene/kibombo/kibombo.h"

extern u8 Data_0200635c[];
extern s16 Data_02007948[];
extern struct KibomboObject *Data_02007950;

void Func_02001318(void);
s32 Func_02002b08(s32 value);
void Func_02002b18(s32 index, s32 entry, s32 delta);
void Func_02002ba4(s32 index, s32 entry, s32 delta);

struct KibomboWork90 {
    u8 unk00[0x18];
    struct KibomboObject *leader;
};

s32 Func_02000054(struct KibomboObject *object)
{
    if (gFrameCount & 1) {
        s32 dx;
        s32 dy;

        dx = dy = -0x10000;
        dx += ((Random16Far() * 3) >> 16) << 16;
        dy += ((Random16Far() * 3) >> 16) << 16;

        Object_SetPositionAndResetMotion(object, object->field44 + dx, object->field48 + dy, object->z);
    }
    return 1;
}
