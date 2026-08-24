#include "types.h"

typedef u16 (*Callback_08005904)(u16);
extern Callback_08005904 Data_02004c14;

u16 Func_08005904(u16 value)
{
    return Data_02004c14(value);
}
