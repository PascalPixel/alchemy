#include "IMIRU_FUCHIN.H"

extern s32 ImiruFuchin_TrackLeader;
void ImiruFuchin_ApplyRoomLayout(void);
void ImiruFuchin_PlaceDragonsEye(void);
struct Actor_39a *OverlayObject_CreateAndInitialize(s32 x, s32 y, s32 z, s32 sprite);
void BattleFx_StartFadeOverlay(s32 mode);

void ImiruFuchin_ApplyEntrySetup(void)
{
    u8 *actor;
    s32 value;

    ImiruFuchin_ApplyRoomLayout();
    if (gGameState.scene == (s32)&SceneId_ImiruFuchin4) {
        if (!GameFlag_IsSet(0xf13) && gGameState.entrance == 1) {
            ImiruFuchin_PlaceDragonsEye();
        }
        if ((u16)(gGameState.entrance - 2) <= 3) {
            OverlayObject_CreateAndInitialize(0x9c0000, 0, 0x1c40000, 223);
            OverlayObject_CreateAndInitialize(0xbc0000, 0, 0x1c40000, 223);
        }
    } else if (gGameState.scene == (s32)&SceneId_ImiruFuchin7) {
        /* One zero clears the flag and both of actor 8's words, and the
         * variable is reused for the tracking work below: the reference keeps
         * the zero and then the work in the same register. */
        value = 0;
        actor = (u8 *)Actor_Get(8);
        ImiruFuchin_TrackLeader = value;
        actor[85] = value;
        *(s32 *)(actor + 12) = value;
        Actor_SetSpritePriority(8, 1);
        Actor_SetChildValue(8, 15);
        switch (gGameState.entrance) {
        case 1:
        case 2:
            BattleFx_StartFadeOverlay(0);
            ImiruFuchin_TrackLeader = 1;
            break;
        case 5:
            BattleFx_StartFadeOverlay(0);
            ImiruFuchin_TrackLeader = 1;
            value = *(s32 *)(gWorkSlot + 36);
            ((struct TrackingWork *)value)->actor = NULL;
            break;
        }
        if (gGameState.entrance <= 6) {
            if (GameFlag_IsSet(0x820)) {
                Map_CopyCellsTo(30, 57, 19, 57, 1, 1);
                Map_CopyCellsTo(30, 8, 12, 8, 8, 7);
            } else {
                gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
                ColorBuffer_ApplySource(0x203108, 1);
                ColorBuffer_ApplyTarget(0x203108, 1);
                ColorBuffer_Interpolate(1);
                Task_Wait(1);
            }
        }
    }
}
