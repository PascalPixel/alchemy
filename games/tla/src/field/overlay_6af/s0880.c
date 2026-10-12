#include "types.h"
#include "call.h"

struct Ov6afCell {
    s32 x;
    s32 z;
    s32 unknown_08;
    u16 flag;
    u16 unknown_0e;
};

extern struct Ov6afCell Data_0200117c[];
extern struct Ov6afCell Data_020011cc[];

void Func_02000f9c();
void Func_02000f94();
void GameFlag_ClearBit();

void Func_02000880(s32 unused, s32 frame, s32 id)
{
    struct Ov6afCell *cells;

    if (id < 50) {
        cells = Data_0200117c;
        id -= 40;
    } else {
        cells = Data_020011cc;
        id -= 50;
    }
    if (frame == 15) {
        Call6(Func_02000f9c, 3, 100, 3, 4, cells[id].x + 63, cells[id].z - 2);
    } else if (frame == 30) {
        Call6(Func_02000f9c, 6, 100, 3, 4, cells[id].x + 63, cells[id].z - 2);
    } else if (frame == 45) {
        Call6(Func_02000f9c, 9, 100, 3, 4, cells[id].x + 63, cells[id].z - 2);
        Call6(Func_02000f94, 0, 0, 1, 1, cells[id].x, cells[id].z);
        GameFlag_ClearBit(cells[id].flag);
    }
}
