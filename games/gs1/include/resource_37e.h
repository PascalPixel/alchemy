#ifndef ALCHEMY_RESOURCE_37E_H
#define ALCHEMY_RESOURCE_37E_H

#include "types.h"

/* Behavior names for the address-stable overlay symbols. */
#define Resource37e_GetPrimaryTable     Func_02000030
#define Resource37e_NullCallback        Func_02000038
#define Resource37e_GetSecondaryTable   Func_0200003c
#define Resource37e_GetTertiaryTable    Func_02000044
#define Resource37e_GetQuaternaryTable  Func_0200004c
#define Resource37e_Run                 Func_02000054

#define RESOURCE37E_PRIMARY_TABLE     ((u8 *)0x020080c0)
#define RESOURCE37E_SECONDARY_TABLE   ((u8 *)0x02008120)
#define RESOURCE37E_TERTIARY_TABLE    ((u8 *)0x02008130)
#define RESOURCE37E_QUATERNARY_TABLE  ((u8 *)0x02008148)

#endif
