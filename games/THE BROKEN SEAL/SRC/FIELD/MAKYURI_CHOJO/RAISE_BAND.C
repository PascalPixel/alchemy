#include "TYPES.H"

void MakyuriChojo_CollectBandSlots();
u8 * Engine_ActorGet();
void Engine_AudioPlayCue();
void Engine_TaskWait();
void Engine_MapRedraw();


struct Flags85 {
    u8 pad[85];
    u8 flags;
};

/* Slide the work position up while lowering the collected actors, easing
 * the speed off over the last ten frames, then redraw the map. */
void MakyuriChojo_RaiseCollectedActors(void)
{
    s32 ids[5];
    u8 *work;
    u32 i;
    u32 j;
    u32 n;
    s32 speed;
    u8 *actor;

    work = *(u8 **)0x03001e70 + 0x164;
    speed = 0x18000;
    n = 0;
    for (i = 0; i <= 4; i++)
        ids[i] = 66;
    MakyuriChojo_CollectBandSlots(ids, *(s32 *)(work + 12));
    for (i = 0; i <= 4; i++) {
        if (ids[i] == 66)
            break;
        ((struct Flags85 *)Engine_ActorGet(ids[i]))->flags = 0;
        n++;
    }
    Engine_AudioPlayCue(223);
    for (i = 0; i <= 85; i++) {
        *(s32 *)(work + 12) += speed;
        for (j = 0; j < n; j++) {
            *(s32 *)(Engine_ActorGet(ids[j]) + 16) -= speed;
            actor = Engine_ActorGet(ids[j]);
            *(s32 *)(actor + 64) = *(s32 *)(Engine_ActorGet(ids[j]) + 16);
        }
        if ((i & 3) == 3 && i > 75)
            speed += -0x3333;
        if (speed < 0xccc)
            speed = 0xccc;
        Engine_TaskWait(1);
    }
    *(s32 *)(work + 12) = 0x4000000;
    Engine_MapRedraw();
    Engine_TaskWait(2);
}
