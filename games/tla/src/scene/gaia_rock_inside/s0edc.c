#include "types.h"
#define FIELD_AT(base, type, off) \
    (((struct {                   \
        u8   pad[off];            \
        type v;                   \
    } *)(base))                   \
            ->v)

#include "call.h"
void      Func_02004e5c(void);
void      Func_02004fcc(s32 a);
void      Func_02004f0c(s32 a);
void      Func_02004f14(s32 a, s32 b);
void      Func_02004e64(void);
extern u8 MsgGaiaRockInside05[];

void Func_02000edc(void)
{
    Func_02004e5c();
    Call1(Func_02004fcc, 0);
    Call1(Func_02004f0c, (s32)MsgGaiaRockInside05);
    Call2(Func_02004f14, 0xa, 0);
    Func_02004e64();
}
