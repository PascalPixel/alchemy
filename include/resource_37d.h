#ifndef ALCHEMY_RESOURCE_37D_H
#define ALCHEMY_RESOURCE_37D_H

#include "types.h"

/* Behavior names for the address-stable overlay symbols. */
#define Resource37d_GetPrimaryTable     Func_02000030
#define Resource37d_NullCallback        Func_02000038
#define Resource37d_GetSecondaryTable   Func_0200003c
#define Resource37d_GetTertiaryTable    Func_02000044
#define Resource37d_GetQuaternaryTable  Func_0200004c
#define Resource37d_Run                 Func_02000054

#define RESOURCE37D_PRIMARY_TABLE     ((u8 *)0x020080c0)
#define RESOURCE37D_SECONDARY_TABLE   ((u8 *)0x02008120)
#define RESOURCE37D_TERTIARY_TABLE    ((u8 *)0x02008130)
#define RESOURCE37D_QUATERNARY_TABLE  ((u8 *)0x02008148)

#endif
