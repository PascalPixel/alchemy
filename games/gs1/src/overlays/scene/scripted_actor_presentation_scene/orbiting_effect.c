#include "types.h"

#define SceneEffect_UpdateCounterDrivenOrbit Func_02003660
/*
 * These three symbols name the call words each site holds, not the imports
 * they eventually reach. Respelling them as the semantic imports changes the
 * call encodings, so leave the addresses as they are.
 */

u8 *Func_020093ba(s32 index);   /* scene-record accessor (Scene_GetRecord) */

s32 Func_020092b8(s32 angle);   /* sine of a binary angle (Func_08000118) */

s32 Func_020092a0(s32 angle);   /* cosine of a binary angle (Func_08000120) */

/*
 * Per-frame orbit step for one actor: read the binary angle at +100, place
 * the actor on a circle around scene record 23, mirror the placement into
 * +56/+64, and advance the angle by -0x800, a thirty-second of a turn. The
 * two arms use deliberately different radius terms -- the +98 counter enters
 * both -- and must not be unified. The actor layout is raw offsets: nothing
 * establishes which of +8 and +16 is which world axis, so they are unnamed.
 */
void SceneEffect_UpdateCounterDrivenOrbit(u8 *actor)
{
    u8 *anchor = Func_020093ba(23);
    u16 *pangle = (u16 *)(actor + 100);
    s32 angle = *pangle;
    s32 cosine;
    s32 sine;
    s32 along;
    s32 across;

    cosine = Func_020092a0(angle);
    along = *(s32 *)(anchor + 8)
          + cosine *(*(s32 *)(actor + 48) + *(u8 *)(actor + 98) + 6);
    *(s32 *)(actor + 8) = along;

    sine = Func_020092b8(angle);
    across = *(s32 *)(anchor + 16)
           + sine *(*(u8 *)(actor + 98) + 4);
    *(s32 *)(actor + 16) = across;

    *(s32 *)(actor + 56) = *(s32 *)(actor + 8);
    *(s32 *)(actor + 64) = across;

    {
        s32 next = *pangle;
        next = next + (s32)0xfffff800;
        *pangle = (u16)next;
    }
}
