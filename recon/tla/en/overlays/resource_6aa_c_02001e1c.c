/* Near miss: Func_02001e1c differs 4, as overlay 6ad's Func_0200215c does: the part mask is built before the
 * sprite is loaded. sched2 ranks the sprite load (priority 72) above the mask constant, a QImode 3
 * (71), and they would tie on insn order anyway. Tried 2026-10-15: FieldSprite's priority bitfield
 * (same 4), u32 or u8 part, the part masked first in a local, the byte masked by hand four ways (9 to
 * 15), one-pass blocks and empty asm barriers around the stores (4 to 8); alchemy permute 3 minutes. */
#include "../../../../games/tla/src/field/overlay_6aa/room.h"

extern const u8 Data_020056a0[];

void Func_02001ae0(void);
void Func_02001cbc(void);
void Func_02001f90(struct ObjectRuntime *object);

void Func_02001e1c(struct ObjectRuntime *object, s32 part)
{
    object->motion_flags = 0;
    object->collision_flags = 0;
    ((struct SpriteParts *)object->animation)->part = part;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02004bd0(object, Data_020056a0);
    object->action = 0;
    object->speed_limit = 0;
    object->acceleration = 0;
}

