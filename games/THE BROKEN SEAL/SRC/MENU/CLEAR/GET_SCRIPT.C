#include "TYPES.H"
#include "SCENE.H"

/*
 * The eight-byte owner at 0x02000030 includes its one pool word, which holds
 * the returned table address 0x02009658.
 */

/*
 * The eight-byte owner at 0x0200003c includes its one pool word, which holds
 * the returned table address 0x02009688.
 */

/*
 * The eight-byte owner at 0x02000044 includes its one pool word, which holds
 * the returned table address 0x0200968c.
 */

/*
 * The eight-byte owner at 0x0200004c includes its one pool word, which holds
 * the returned table address 0x020096a4.
 */
u8 *SceneData_GetScriptTable(void)
{
    return (u8 *)0x02009658;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return (u8 *)0x02009688;
}

u8 *SceneData_GetActorTable(void)
{
    return (u8 *)0x0200968c;
}

u8 *SceneData_GetEffectTable(void)
{
    return (u8 *)0x020096a4;
}
