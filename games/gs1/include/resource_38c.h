#ifndef ALCHEMY_RESOURCE_38C_H
#define ALCHEMY_RESOURCE_38C_H

/* Address-stable tables returned by the overlay's leaf accessors. */
#define RESOURCE38C_PRIMARY_TABLE_ADDRESS    0x02008598
#define RESOURCE38C_SECONDARY_TABLE_ADDRESS  0x02008688
#define RESOURCE38C_TERTIARY_TABLE_ADDRESS   0x020088f0

/* Behavior names for the address-stable leaf symbols. */
#define Resource38c_GetPrimaryTable    Func_02000030
#define Resource38c_NullCallback       Func_02000038
#define Resource38c_GetSecondaryTable  Func_0200003c
#define Resource38c_GetTertiaryTable   Func_0200011c

#endif
