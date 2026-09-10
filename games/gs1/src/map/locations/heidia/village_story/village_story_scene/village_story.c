#include "types.h"

#define SceneData_GetTableb9c8 Func_02000320
#define SceneData_GetTableB9d4AfterStateCheck Func_02000328
#define SceneData_GetTablebbe4 Func_02000354
#define SceneActor_SetMode55OnSevenRecords Func_02000ad0
#define FieldScene_ConfigureValue93Scene Func_02003438
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

extern u8 Data_0200b9c8[];
extern s16 Data_02000240[];
extern u8 Data_0200b9d4[];
extern u8 Data_0200bbe4[];

void Func_020039f6(s32);
struct Record *Func_020041c6();
struct Record *Func_020041d2();
struct Record *Func_020041de();
struct Record *Func_020041e8();
struct Record *Func_020041f2();
struct Record *Func_020041fc();
struct Record *Func_02004206();
void Func_02006c50(s32 value, s32 mode);
void Func_02006c60(s32 left, s32 right);
void Func_02006c7c(void);
void Func_02006c5a(s32 enabled);
void Func_02006c76(void);
void Func_02006c82(void);

/* Contiguous unnamed leaf-owner run for resource_39d. */

/* Configure and publish the scene's presentation layer. */

void *SceneData_GetTableb9c8(void)
{
    return Data_0200b9c8;
}

void *SceneData_GetTableB9d4AfterStateCheck(void)
{
    if (Data_02000240[225] != 1) {
        Func_020039f6(0x253);
    }
    return Data_0200b9d4;
}

void *SceneData_GetTablebbe4(void)
{
    return Data_0200bbe4;
}

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

void FieldScene_ConfigureValue93Scene(void)
{
    Func_02006c50(93, 1);
    Func_02006c60(24, 9);
    Func_02006c7c();
    Func_02006c5a(1);
    Func_02006c76();
    Func_02006c82();
}
