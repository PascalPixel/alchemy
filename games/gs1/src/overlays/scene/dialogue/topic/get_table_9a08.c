#include "types.h"
#include "scene.h"

/*
 * resource_3b7 owner at 0x02000030: a leaf that loads its literal pool word
 * and returns it. The eight-byte owner includes that pool word at
 * 0x02000034, holding the address 0x02009a08 -- image offset 0x1a08 --
 * returned without being dereferenced. Many rows across the tree share this
 * body but each returns a different address.
 */

u8 *SceneData_GetTable9A08(void)
{
    return (u8 *)0x02009a08;
}
