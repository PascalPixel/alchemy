#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The item object once it has been placed. */
extern struct FieldActor *Data_0200e968;

static __inline__ s32 Value1(s32 (*f)(s32), s32 a0)
{
    return f(a0);
}

static __inline__ void Call1(s32 (*f)(s32), s32 a0)
{
    f(a0);
}

/* Places the Anchor Charm (item 232) as object 22, drawing its item icon
 * into the object's VRAM block, the first time only (flag 0x200); returns
 * the object. */
struct FieldActor *FuneHeya_Func020045d0(void)
{
    s32 set = Value1(Engine_GameFlagIsSet, 0x200);
    struct FieldActor *object;
    struct FieldSprite *sprite;
    u8 *buffer;

    if (set != 0) {
        return Data_0200e968;
    }
    object = Engine_ObjectCreate(22, 0x1c70000, 0x40000, 0x2200000);
    object->motion_flags = set;
    object->unknown_5c = 1;
    sprite = object->sprite;
    sprite->part_count = set;
    sprite->full_color = 0;
    sprite->palette = 0;
    buffer = Engine_HeapAllocate(17, 0x608);
    Engine_ItemLoadIcon(232);
    Engine_VramLoad(sprite->vram_block, 128, buffer + 0x400);
    Engine_HeapRelease(17);
    Call1(Engine_GameFlagSet, 0x200);
    Data_0200e968 = object;
    return object;
}
