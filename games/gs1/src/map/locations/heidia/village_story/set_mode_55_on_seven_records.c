#include "types.h"

#define SceneActor_SetMode55OnSevenRecords Func_02000ad0

/*
 * resource_39d owner at 0x02000ad0, 82 bytes.
 *
 * Sets one byte on seven records in a row. Each record is fetched by id through
 * its own per-site veneer -- seven calls, seven veneers, as every other owner in
 * this overlay does -- and the byte at +85 is written immediately after.
 *
 * The ids are 0 and then 14 through 19, and the first record takes 3 where the
 * rest take 4. Nothing in the span says what the byte means, so it keeps an
 * offset-derived name.
 */

struct Record {
    u8 pad00[85];
    u8 mode55;              /* +85, 0x55 */
};

struct Record *Func_020041c6();
struct Record *Func_020041d2();
struct Record *Func_020041de();
struct Record *Func_020041e8();
struct Record *Func_020041f2();
struct Record *Func_020041fc();
struct Record *Func_02004206();

void SceneActor_SetMode55OnSevenRecords(void)
{
    Func_020041c6(0)->mode55 = 3;
    Func_020041d2(14)->mode55 = 4;
    Func_020041de(15)->mode55 = 4;
    Func_020041e8(16)->mode55 = 4;
    Func_020041f2(17)->mode55 = 4;
    Func_020041fc(18)->mode55 = 4;
    Func_02004206(19)->mode55 = 4;
}
