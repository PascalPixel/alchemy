#ifndef ALCHEMY_RESOURCE_3CC_H
#define ALCHEMY_RESOURCE_3CC_H

/* Conservative behavior names for address-stable overlay symbols. */
#define Resource3cc_GetPrimaryTable     Func_02000030
#define Resource3cc_NullCallback        Func_02000038
#define Resource3cc_GetSecondaryTable   Func_0200003c
#define Resource3cc_GetTertiaryTable    Func_02000044
#define Resource3cc_Run0200004c         Func_0200004c
#define Resource3cc_Run02000064         Func_02000064
#define Resource3cc_Run0200007c         Func_0200007c
#define Resource3cc_Run02000094         Func_02000094
#define Resource3cc_GetQuaternaryTable  Func_020000ac
#define Resource3cc_ReturnZero          Func_020000b4

#define RESOURCE3CC_PRIMARY_TABLE_ADDRESS     0x020081a8
#define RESOURCE3CC_SECONDARY_TABLE_ADDRESS   0x020081d8
#define RESOURCE3CC_TERTIARY_TABLE_ADDRESS    0x020081ec
#define RESOURCE3CC_QUATERNARY_TABLE_ADDRESS  0x02008264

#endif
