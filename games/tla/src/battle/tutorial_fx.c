#include "types.h"

struct BattleFxRequest {
    s32 field_00;
    s32 field_04;
    s32 field_08;
    s32 field_0c;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    u8 unknown_1c[8];
    s16 field_24;
    u8 unknown_26[0x32];
};

void Func_08138050(struct BattleFxRequest *request, s32 value);

void Func_081197a8(s32 value)
{
    struct BattleFxRequest request;

    request.field_10 = 0;
    request.field_08 = 128;
    request.field_0c = 0;
    request.field_14 = 1;
    request.field_24 = 128;
    request.field_04 = 0;
    request.field_00 = 0;
    request.field_18 = 0;
    Func_08138050(&request, value);
}
