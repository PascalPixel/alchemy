#include "TYPES.H"
#include "FIELD_EVENT.H"

/*
 * Suhara village's scene tables: the script and message tables and the
 * actor table the event flag 0x96f selects.
 */

/*
 * Returns the in-image table address 0x020082f0, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr.
 */
u8 *SceneData_GetScriptTable(void)
{
    return (u8 *)0x020082f0;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

/*
 * Returns the in-image table address 0x020083c8, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr.
 */
u8 *SceneData_GetMessageTable(void)
{
    return (u8 *)0x020083c8;
}

s32 SceneData_SelectActorTableByFlag96f(void)
{
    if (GameFlag_IsSet(0x96f) != 0) {
        return 0x020084e0;
    }
    return 0x020083f0;
}
