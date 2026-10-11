/* Draft, differ 0: overlay 699's Func_02000560 with this overlay's addresses, which needs its asm barrier on the height (the native loads the ground height before it clears the word). Held back: the device ratchet (tla asm-barrier 197) may not rise. */
#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 Data_020025f0;
extern s32 Data_020025f4;
extern u16 Data_0200261c[];
extern u16 Data_0200263c[];
extern u16 Data_02002640[];

s32 GameFlag_Test();
void Scheduler_AddOrUpdateCallback();
struct FieldActor *Object_GetById();
s32 Map_GetTerrainHeight();
void Func_02001bd8();
void Func_02001c98();
void Func_020003a4(void);

void Func_020004e4(s32 id)
{
    struct FieldActor *object;
    s32 height;

    object = Object_GetById(id);
    object->motion_flags = 4;
    height = object->terrain_height;
    asm volatile("" : "+l"(height)); /* FAKEMATCH: ⚓️ loads the ground height before it clears the word */
    WORD_AT(object, 68) = 0;
    object->y = height + 0x200000;
    {
        u8 *flags = &object->priority_flags;
        s32 bit = 128; /* FAKEMATCH: ⚓️ builds the bit in the register it ORs into */

        *flags = bit | *flags;
    }
    height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
    Func_02001bd8(object->x, object->z, 0, (height >> 19) + 6);
    Func_02001c98(object->x, object->z, 0, 128);
}
