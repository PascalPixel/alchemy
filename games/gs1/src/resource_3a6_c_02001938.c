#include "types.h"
/* resource_3a6 owner at 0x02001938, 76 bytes.
 * Fetches a record for the caller's id and, when it exists, marks it and
 * writes two 20.12 fixed-point coordinates biased by half a unit. */
struct Rec_3a6 {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
    u8 pad14[14];
    u8 f34;                     /* +34 */
    u8 f35;                     /* +35 */
};
struct Rec_3a6 *Func_02003602();
void Func_02003670();
void Func_02001938(s32 id, s32 x, s32 y)
{
    struct Rec_3a6 *rec = Func_02003602(id);

    if (rec != 0) {
        Func_02003670(id, 3);
        rec->f34 = 2;
        rec->f35 |= 2;
        rec->f8 = (x << 20) + 0x80000;
        rec->f16 = (y << 20) + 0x80000;
    }
}
