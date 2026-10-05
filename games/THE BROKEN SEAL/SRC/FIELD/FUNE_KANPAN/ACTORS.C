#include "GLOBAL.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KANPAN.H"

s32 FuneKanpan_SlotDepth[8];
void OverlayObject_ActivateSlotWithMode3(s32 actor);
void FuneKanpan_ChooseSlotModes(void);

union Slot {
    s32 w;
    s16 h[2];
};


void SceneEffect_InitSlotsEightToNineteen(void)
{
    Object_GetById(8)->collision_flags = 0;
    Object_GetById(9)->collision_flags = 0;
    Object_GetById(10)->collision_flags = 0;
    Object_GetById(11)->collision_flags = 0;
    OverlayObject_ActivateSlotWithMode3(8);
    OverlayObject_ActivateSlotWithMode3(9);
    OverlayObject_ActivateSlotWithMode3(10);
    OverlayObject_ActivateSlotWithMode3(11);
    OverlayObject_ActivateSlotWithMode3(12);
    OverlayObject_ActivateSlotWithMode3(13);
    OverlayObject_ActivateSlotWithMode3(14);
    OverlayObject_ActivateSlotWithMode3(15);
    FuneKanpan_SlotDepth[0] = Object_GetById(12)->z.fixed;
    FuneKanpan_SlotDepth[1] = Object_GetById(13)->z.fixed;
    FuneKanpan_SlotDepth[2] = Object_GetById(14)->z.fixed;
    FuneKanpan_SlotDepth[3] = Object_GetById(15)->z.fixed;
    OverlayObject_ActivateSlotWithMode3(16);
    OverlayObject_ActivateSlotWithMode3(17);
    OverlayObject_ActivateSlotWithMode3(18);
    OverlayObject_ActivateSlotWithMode3(19);
    Object_GetById(16)->scale_x = 0xffff0000;
    Object_GetById(17)->scale_x = 0xffff0000;
    Object_GetById(18)->scale_x = 0xffff0000;
    Object_GetById(19)->scale_x = 0xffff0000;
    FuneKanpan_SlotDepth[4] = Object_GetById(16)->z.fixed;
    FuneKanpan_SlotDepth[5] = Object_GetById(17)->z.fixed;
    FuneKanpan_SlotDepth[6] = Object_GetById(18)->z.fixed;
    FuneKanpan_SlotDepth[7] = Object_GetById(19)->z.fixed;
    FuneKanpan_ChooseSlotModes();
}

void OverlayObject_ActivateSlotWithMode3(s32 a)
{
    struct FieldActor *p = Object_GetById(a);

    if (p != 0) {
        Engine_ActorSetSpritePriority(a, 3);
        Engine_ActorSetSpriteFlags(p, 0);
        p->collision_flags = 0;
        {
            s32 c;
            c = 2 | p->priority_flags;
            p->priority_flags = c;
        }
    }
}
