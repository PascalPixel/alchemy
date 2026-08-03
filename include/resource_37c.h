#ifndef ALCHEMY_RESOURCE_37C_H
#define ALCHEMY_RESOURCE_37C_H

#include "types.h"

/* Behavior names for the address-stable overlay symbols. */
#define Resource37c_GetPrimaryTable     Func_02000030
#define Resource37c_NullCallback        Func_02000038
#define Resource37c_GetSecondaryTable   Func_0200003c
#define Resource37c_GetTertiaryTable    Func_02000044
#define Resource37c_GetQuaternaryTable  Func_0200004c
#define Resource37c_Run                 Func_02000054

#define RESOURCE37C_PRIMARY_TABLE     ((u8 *)0x020080c8)
#define RESOURCE37C_SECONDARY_TABLE   ((u8 *)0x02008110)
#define RESOURCE37C_TERTIARY_TABLE    ((u8 *)0x0200811c)
#define RESOURCE37C_QUATERNARY_TABLE  ((u8 *)0x02008134)

#endif
