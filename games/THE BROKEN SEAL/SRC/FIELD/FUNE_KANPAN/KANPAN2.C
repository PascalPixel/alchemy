/* Activate the deck's slot actors 8 to 19, keep the depth of the eight
   standing ones and fill the drifting slots' modes. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KANPAN.H"

/* The standing slots' depths, uninitialised work after the deck's block. */
s32 FuneKanpan_SlotDepth[8];
void OverlayObject_ActivateSlotWithMode3(s32 actor);
void FuneKanpan_ChooseSlotModes(void);

union Slot {
    s32 w;
    s16 h[2];
};

extern u8 LinkedMessage_TheresNothingWeCanDo[];
s32 BuildMotionCountdown(s32, s16);
u8 *Object_GetById(s32);

void SceneEffect_InitSlotsEightToNineteen(void)
{
    Engine_ActorGet(8)->collision_flags = 0;
    Engine_ActorGet(9)->collision_flags = 0;
    Engine_ActorGet(10)->collision_flags = 0;
    Engine_ActorGet(11)->collision_flags = 0;
    OverlayObject_ActivateSlotWithMode3(8);
    OverlayObject_ActivateSlotWithMode3(9);
    OverlayObject_ActivateSlotWithMode3(10);
    OverlayObject_ActivateSlotWithMode3(11);
    OverlayObject_ActivateSlotWithMode3(12);
    OverlayObject_ActivateSlotWithMode3(13);
    OverlayObject_ActivateSlotWithMode3(14);
    OverlayObject_ActivateSlotWithMode3(15);
    FuneKanpan_SlotDepth[0] = Engine_ActorGet(12)->z.fixed;
    FuneKanpan_SlotDepth[1] = Engine_ActorGet(13)->z.fixed;
    FuneKanpan_SlotDepth[2] = Engine_ActorGet(14)->z.fixed;
    FuneKanpan_SlotDepth[3] = Engine_ActorGet(15)->z.fixed;
    OverlayObject_ActivateSlotWithMode3(16);
    OverlayObject_ActivateSlotWithMode3(17);
    OverlayObject_ActivateSlotWithMode3(18);
    OverlayObject_ActivateSlotWithMode3(19);
    Engine_ActorGet(16)->scale_x = 0xffff0000;
    Engine_ActorGet(17)->scale_x = 0xffff0000;
    Engine_ActorGet(18)->scale_x = 0xffff0000;
    Engine_ActorGet(19)->scale_x = 0xffff0000;
    FuneKanpan_SlotDepth[4] = Engine_ActorGet(16)->z.fixed;
    FuneKanpan_SlotDepth[5] = Engine_ActorGet(17)->z.fixed;
    FuneKanpan_SlotDepth[6] = Engine_ActorGet(18)->z.fixed;
    FuneKanpan_SlotDepth[7] = Engine_ActorGet(19)->z.fixed;
    FuneKanpan_ChooseSlotModes();
}

void OverlayObject_ActivateSlotWithMode3(s32 a)
{
    u8 *p = Object_GetById(a);

    if (p != 0) {
        Actor_SetSpritePriority(a, 3);
        Actor_SetSpriteFlags(p, 0);
        p[89] = 0;
        {
            s32 c;
            c = 2 | p[35];
            p[35] = c;
        }
    }
}
