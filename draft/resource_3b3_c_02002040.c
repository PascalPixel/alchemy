#include "types.h"
/* resource_3b3 owner at 0x02002040, 64 bytes.
 * Copies a record's three coordinates into a stack triple, lifting the third
 * by 0x100000, and offers it; on refusal it falls back once. */
struct Rec_3b3 {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    s32 f12;                    /* +12 */
    s32 f16;                    /* +16 */
};
struct Pos_3b3 { s32 x, y, z; };
struct Rec_3b3 *Func_02004b64();
s32 Func_020023b2();
void Func_0200403e();
s32 Func_020032d8();
void Func_02004c0c();
void Func_02002040(void)
{
    struct Rec_3b3 *rec = Func_02004b64(0);
    struct Pos_3b3 pos;

    pos.x = rec->f8;
    pos.y = rec->f12;
    pos.z = rec->f16 + 0x100000;

    if (Func_020023b2(&pos) != 0) {
        Func_0200403e();
    } else if (Func_020032d8() == 0) {
        Func_02004c0c();
    }
}
