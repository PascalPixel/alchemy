/* Presenting an item: its icon rises over the scene while the party member
   holding item 224 exchanges it for the presented item. */
#include "TYPES.H"

u8 *Engine_ObjectCreate(s32 type);
s32 PartyInventory_FindOwner(s32 item);
s32 Inventory_Find(s32 owner, s32 item);
void Engine_ObjectSetScript(u8 *object, const s32 *script);
u8 *Engine_HeapAllocate(s32 slot, s32 size);
void Engine_ItemLoadIcon(s32 item);
s32 Engine_VramLoad(s32 block, s32 size, const void *data);
void Engine_HeapRelease(s32 slot);
void Engine_AudioPlayCue(s32 cue);
void Engine_RunRisingObjectSequence(u8 *object, s32 mode);
s32 Inventory_Discard(s32 owner, s32 slot);
void Inventory_AddItem(s32 owner, s32 item);
void Engine_ObjectDispatchRelease(u8 *object);
void Engine_ActorSetAnimation(s32 actor, s32 animation);
extern const s32 SoruStar_PresentItemScript[];

s32 Scene_PresentItem(s32 item)
{
    register u8 *buf asm("r8"); /* FAKEMATCH: pins the buffer to r8 */
    u8 *obj;
    s32 owner;
    s32 slot;
    u8 *sprite;
    u8 *p;
    s32 mask;

    {
        register s32 zero asm("r0"); /* FAKEMATCH: builds the zero in r0 */

        asm("mov %0, #0" : "=l"(zero)); /* FAKEMATCH: the zero is not a reloadable constant */
        buf = (u8 *)zero;
    }
    obj = Engine_ObjectCreate(22);
    owner = PartyInventory_FindOwner(224);
    slot = Inventory_Find(owner, 224);

    if (obj == 0) {
        return owner;
    }
    {
        Engine_ObjectSetScript(obj, SoruStar_PresentItemScript);
        sprite = *(u8 **)(obj + 80);
        p = sprite + 38;
        *p = (u32)buf;
        p++;
        *p = (u32)buf;
        mask = 33;
        mask = -mask;
        sprite[5] &= mask;
        sprite[9] &= 0xf;
        *(s32 *)(obj + 40) = 163840;
        *(s32 *)(obj + 72) = 16384;
        buf = Engine_HeapAllocate(17, 1544);
        Engine_ItemLoadIcon(item);
        Engine_VramLoad(sprite[28], 128, buf + 1024);
        Engine_HeapRelease(17);
        Engine_AudioPlayCue(83);
        Engine_RunRisingObjectSequence(obj, 3);
        Inventory_Discard(owner, slot);
        Inventory_AddItem(owner, item);
        Engine_ObjectDispatchRelease(obj);
        Engine_ActorSetAnimation(0, 1);
    }
    return owner;
}
