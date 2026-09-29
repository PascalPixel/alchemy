#include "KAREI.H"
#include "SCENE_IDS.H"

extern const u16 KareiTorebi_LeaveCells1[];
extern const u16 KareiTorebi_LeaveCells3[];

void FieldScene_RunScene3ae_020007dc(void)
{
    u8 *work;

    Event_Begin();
    Audio_PlayCue(158);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
    work = (u8 *)Data_02000240;
    if (*(s16 *)(work + 0x1c0) == (s32)&SceneId_KareiTorebi1) {
        Actor_WalkTo(ACTOR_PARTY_LEADER, 0x130, 0x570);
        Map_AnimateCells(KareiTorebi_LeaveCells1, 78, 86);
    } else {
        if (*(s16 *)(work + 0x1c0) == (s32)&SceneId_KareiTorebi3) {
            Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 192);
            Map_AnimateCells(KareiTorebi_LeaveCells3, 74, 9);
        }
    }
    Event_Wait(16);
    Event_RequestExit(3);
    Event_End();
}
