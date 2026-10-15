/* Draft, differ 2 of 30: the listing loads the zero for arrived from the literal pool (ldr r2, =0) before it moves the frames, the mark of a constant held in a halfword register; this draft builds it after. alchemy permute matched only by truncating the update pointer through a short, which is no source form. Earlier tries: an asm barrier (6), a u8 zero local (5), the byte store first (20). Also tried: a u16 zero or (u16)0 (2), frames as u32, s16, u16 or u8 (2 to 22), a barrier or one-pass block around either store (4 to 24), three more minutes of permute (nothing better). sched2 ties the zero's load and the frames copy (mov r0, r8) at priority 69 and takes the copy, which has one more dependent. */
#include "types.h"

struct Walker {
    u8 unk00[0x63];
    u8 arrived;
    u16 frames;
    u8 unk66[6];
    void (*update)(void);
};

struct Walker *Object_GetById(s32 id);
void Func_020044cc(s32 actor, s32 x, s32 z);
void Func_020003bc(void);

void Func_0200279c(s32 actor, s32 x, s32 z, s32 frames)
{
    struct Walker *object = Object_GetById(actor);

    object->frames = frames;
    object->arrived = 0;
    object->update = Func_020003bc;
    Func_020044cc(actor, x << 16, z << 16);
}
