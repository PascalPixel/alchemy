/* Near miss of Func_020000f4 (76 instructions, 14 differ): the native starts the z constant in
 * r2 before it stores x and keeps the motion flag address in r1; the draft reuses r3 for each
 * constant, and stores the priority byte before it loads the sprite attributes. An inline
 * position setter gives 20 differ. */
#include "types.h"
#include "party_state.h"
#include "object_runtime.h"
#include "field_sprite.h"

extern u32 Data_020007e4;
extern u32 gFrameCount;
extern const u8 Data_02000618[];

u32 Random16Far(void);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Func_02000540(struct ObjectRuntime *object, s32 mode);
void Func_02000548(struct ObjectRuntime *object, const void *script);
struct ObjectRuntime *Func_02000550(s32 type, s32 x, s32 y, s32 z);

void Func_020000f4(void)
{
    struct ObjectRuntime *object;
    struct FieldSprite *sprite;
    s32 phase;
    s32 x;

    ObjectMotion_SetActionVariant(8, 3);
    ObjectMotion_SetActionVariant(9, 3);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.current_owner), 0);
    phase = gFrameCount & 15;
    if (phase == 0) {
        object = Func_02000550(0x2a2, 0, 0, 0);
        if (object != NULL) {
            sprite = object->animation;
            x = Random16Far() * 40 + 0x32c0000;
            object->motion_flags = phase;
            object->x = x;
            object->y = 0xa00000;
            object->z = 0x2b80000;
            ObjectDispatch_SetSingleChildFlags(object, 0);
            object->priority_flags &= ~1;
            sprite->priority = 2;
            Func_02000548(object, Data_02000618);
            Func_02000540(object, 1);
        }
    }
    Data_020007e4++;
}
