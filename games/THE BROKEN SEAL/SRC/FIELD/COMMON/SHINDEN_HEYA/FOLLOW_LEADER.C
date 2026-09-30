#include "TYPES.H"
extern u32 gFrameCount;

s32 Engine_ActorGet();

void ShindenHeya_FollowLeaderOffset(u8 *obj)
{
    u8 *leader;

    leader = (u8 *)Engine_ActorGet(8);
    *(s32 *)(obj + 56) = *(s32 *)(obj + 8) = *(s32 *)(leader + 8);
    *(s32 *)(obj + 60) = *(s32 *)(obj + 12) = *(s32 *)(leader + 12);
    *(s32 *)(obj + 64) = *(s32 *)(obj + 16) = *(s32 *)(leader + 16) + -0x20000;
    switch (*(u32 *)&gFrameCount & 3) {
    case 0:
        *(s32 *)(obj + 56) = *(s32 *)(obj + 8) = *(s32 *)(leader + 8) + -0x38000;
        break;
    case 1:
        *(s32 *)(obj + 56) = *(s32 *)(obj + 8) = *(s32 *)(leader + 8) + 0x30000;
        break;
    case 2:
        *(s32 *)(obj + 60) = *(s32 *)(obj + 12) = *(s32 *)(leader + 12) + 0x20000;
        break;
    case 3:
        *(s32 *)(obj + 64) = *(s32 *)(obj + 16) = *(s32 *)(leader + 16);
        break;
    }
}
