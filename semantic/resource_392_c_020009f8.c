/* Run the six-word placement query and forward a successful result. */
#include "types.h"
typedef struct { s32 lo, hi; } Pair;
typedef struct { s32 w0, w1, w2, w3; Pair tail; } Query;
void Func_0200181a(void); void Func_02001846(void);
s32 Func_02000e7a(Query *result);
void Func_02001026(s32,s32,s32,s32,Pair);
void Func_020009f8(void)
{
    Query result;
    Func_0200181a();
    if (Func_02000e7a(&result))
        Func_02001026(result.w0, result.w1, result.w2, result.w3, result.tail);
    Func_02001846();
}
