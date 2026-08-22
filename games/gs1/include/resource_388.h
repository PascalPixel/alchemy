#ifndef ALCHEMY_RESOURCE_388_H
#define ALCHEMY_RESOURCE_388_H

#include "types.h"

/* Behavior names for the address-stable overlay symbols. */
#define Resource388_GetPrimaryTable     Func_02000030
#define Resource388_NullCallback        Func_02000038
#define Resource388_GetSecondaryTable   Func_0200003c
#define Resource388_GetTertiaryTable    Func_02000044
#define Resource388_GetQuaternaryTable  Func_0200004c
#define Resource388_Run                 Func_02000054

#define RESOURCE388_PRIMARY_TABLE     ((u8 *)0x02008108)
#define RESOURCE388_SECONDARY_TABLE   ((u8 *)0x02008180)
#define RESOURCE388_TERTIARY_TABLE    ((u8 *)0x02008194)
#define RESOURCE388_QUATERNARY_TABLE  ((u8 *)0x020081c4)

#endif
