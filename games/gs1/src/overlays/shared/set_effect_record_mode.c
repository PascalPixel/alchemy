#include "types.h"

/* Shared 22-byte head leaf proved identical for this overlay family. */
struct Rec_02000030 {
    u8 pad00[9];
    u8 lo : 2;
    u8 mode : 2;
    u8 hi : 4;
};

struct Work_02000030 {
    u8 pad00[80];
    struct Rec_02000030 *rec;
};

void Func_02000030(struct Work_02000030 *work, s32 value)
{
    work->rec->mode = value;
}
