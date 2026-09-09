/*
 * Inner sanctum data getters, published in the descriptor table at the head
 * of overlay resource_37a. Sibling overlays share this layout but not the
 * addresses returned.
 */

#include "types.h"

#define GetSolShindenScriptData Func_02000030
#define GetSolShindenInitialState Func_02000038
#define GetSolShindenMessageData Func_0200003c
#define GetSolShindenActorData Func_02000044
#define GetSolShindenEffectData Func_0200004c
/*
 * Returns 0x0200aafc, an address inside this overlay's image; its contents
 * are not established. The eight-byte owner includes its one pool word,
 * which sits past the bx lr. The address is published in the descriptor
 * table, not reached by any call.
 */
u8 *GetSolShindenScriptData(void)
{
    return (u8 *)0x0200aafc;
}

/*
 * Returns zero. The four-byte owner carries no pool word, since a constant
 * that fits in an immediate needs none, unlike its eight-byte neighbours in
 * the same table. Whether the caller reads the zero as a null pointer or as
 * an integer is not established.
 */
s32 GetSolShindenInitialState(void)
{
    return 0;
}

/*
 * Returns 0x0200abec, an address inside this overlay's image; its contents
 * are not established. The eight-byte owner includes its one pool word,
 * which sits past the bx lr. The address is published in the descriptor
 * table, not reached by any call.
 */
u8 *GetSolShindenMessageData(void)
{
    return (u8 *)0x0200abec;
}

/*
 * Returns 0x0200ac14, an address inside this overlay's image; its contents
 * are not established. The eight-byte owner includes its one pool word,
 * which sits past the bx lr. The address is published in the descriptor
 * table, not reached by any call.
 */
u8 *GetSolShindenActorData(void)
{
    return (u8 *)0x0200ac14;
}

/*
 * Returns 0x0200ad34, an address inside this overlay's image; its contents
 * are not established. The eight-byte owner includes its one pool word,
 * which sits past the bx lr. The address is published in the descriptor
 * table, not reached by any call.
 */
u8 *GetSolShindenEffectData(void)
{
    return (u8 *)0x0200ad34;
}
