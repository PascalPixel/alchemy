#include "TYPES.H"

/*
 * Parks a scene actor's record: 0x80000000, a sentinel, in three
 * position-family fields, and zero in three more and in its halfword at
 * +100. The roles of +36, +40 and +44 are not established.
 */
void SceneActor_ParkRecord(u8 *record)
{
    /* The sentinel is built as 128 shifted left by 24, not pooled. */
    *(s32 *)(record + 56) = (s32)0x80000000;
    *(s32 *)(record + 60) = (s32)0x80000000;
    *(s32 *)(record + 64) = (s32)0x80000000;

    *(s32 *)(record + 36) = 0;
    *(s32 *)(record + 40) = 0;
    *(s32 *)(record + 44) = 0;

    *(u16 *)(record + 100) = 0;
}
