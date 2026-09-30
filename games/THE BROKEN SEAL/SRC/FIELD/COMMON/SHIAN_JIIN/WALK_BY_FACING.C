#include "TYPES.H"
#include "CALL.H"

s32 Engine_ActorGet();
void Engine_ActorWalkToAndWait();
void Engine_ActorFaceDirection();

/* Walk actor 15 around the leader by the quadrant it faces, then turn it. */
void ShianJiin_WalkByFacing(void)
{
    u32 dir;
    u32 limit;

    dir = *(u16 *)(Engine_ActorGet(0) + 6);
    limit = 0x3fff;
    if ((u32)((dir + -0x2000) << 16) <= 0x3fff0000) {
        Engine_ActorWalkToAndWait(15, 216, 168);
        Engine_ActorWalkToAndWait(15, 224, 168);
        Call3(Engine_ActorFaceDirection, 15, 0x2000, 20);
    } else if ((u16)(dir - 0x6000) <= limit) {
        Engine_ActorWalkToAndWait(15, 232, 160);
        Call3(Engine_ActorFaceDirection, 15, 0x5000, 20);
    } else if ((u16)(dir + 0x6000) <= limit) {
        Engine_ActorWalkToAndWait(15, 216, 168);
        Engine_ActorWalkToAndWait(15, 224, 172);
        Call3(Engine_ActorFaceDirection, 15, 0xe000, 20);
    } else {
        Engine_ActorWalkToAndWait(15, 232, 160);
        Call3(Engine_ActorFaceDirection, 15, 0x2000, 20);
    }
}
