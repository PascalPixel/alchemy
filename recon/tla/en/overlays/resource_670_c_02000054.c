// near miss: differ 17 (listing 31 instructions, draft 30); the native keeps the constant 0xffff0000 in r5 and finishes the first offset before the second random call
#include "../../../../games/tla/src/field/overlay_670/ov670.h"

extern u8 Data_0200635c[];
extern s16 Data_02007948[];
extern struct Ov670Object *Data_02007950;

void Func_02001318(void);
s32 Func_02002b08(s32 value);
void Func_02002b18(s32 index, s32 entry, s32 delta);
void Func_02002ba4(s32 index, s32 entry, s32 delta);

struct Ov670Work90 {
    u8 unknown_00[0x18];
    struct Ov670Object *leader;
};

s32 Func_02000054(struct Ov670Object *object)
{
    if (gFrameCount & 1) {
        s32 dx;
        s32 dy;

        dx = dy = -0x10000;
        dx += ((Random16Far() * 3) >> 16) << 16;
        dy += ((Random16Far() * 3) >> 16) << 16;

        Object_SetPositionAndResetMotion(object, object->field_44 + dx, object->field_48 + dy, object->z);
    }
    return 1;
}
