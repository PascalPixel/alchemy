#include "types.h"
#include "text/msg_ids.h"

void Object_SetModeById(s32 id, s32 mode);
void Func_020042dc(void);
void Func_020042e4(void);
void Func_0200437c(s32 message);
void Func_02004394(s32 speaker, s32 flags);
void Func_0200444c(s32 skip);

void Func_02000ba4(void)
{
    Func_020042dc();
    Func_0200444c(0);
    Object_SetModeById(14, 5);
    Func_0200437c((s32)MsgMarsStar03);
    Func_02004394(14, 0);
    Object_SetModeById(14, 9);
    Func_020042e4();
}
