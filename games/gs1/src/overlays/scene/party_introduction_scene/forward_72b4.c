#include "types.h"

/*
 * Callback for resource_380 that forwards to Func_020072b4 and nothing else.
 * A script record holds its Thumb pointer; nothing calls it directly. The
 * twelve-byte owner includes the zero alignment halfword after the return and
 * has no literal pool. Func_020072b4 takes no arguments -- nothing is loaded
 * into r0 before the call.
 */

extern void Func_020072b4(void);

void FieldScene_Forward72b4(void)
{
    Func_020072b4();
}
