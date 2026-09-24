#include "TYPES.H"
#include "SYSTEM.H"

struct ObjectSlot_080babdc {
    void *object;
};

struct Runtime_080babdc {
    u8 padding_00[65];
    u8 mode;
};

extern struct Runtime_080babdc *gBattleWork;

void *Owner_GetStateFar(s32 id);
struct ObjectSlot_080babdc *GetBattleObjectSlot(s32 id);
void Object_SetMode(void *object, s32 mode);
void BattleLayout_HighlightPartyPanelsFar(u16 *selection);
void Unnamed_080ba918(void *object, s32 value);
s32 BattleMotion_GetSlotField14(s32 id);
void UiWindow_DrawPartyStatusContentsFar(s32 mode);

void BattleMotion_RunValueSequence(s32 id)
{
    u16 selection[2];
    u16 *sel;
    s32 target;
    s32 remaining;

    Owner_GetStateFar(id);
    Object_SetMode(GetBattleObjectSlot(id)->object, 5);

    sel = selection;
    remaining = 1;
    do {
        target = 0xff;
        sel[1] = target;
        sel[0] = id;
        BattleLayout_HighlightPartyPanelsFar(sel);
        Unnamed_080ba918(GetBattleObjectSlot(id)->object, 7);
        WaitFrames(2);

        sel[0] = id;
        BattleLayout_HighlightPartyPanelsFar(sel);
        Unnamed_080ba918(GetBattleObjectSlot(id)->object, BattleMotion_GetSlotField14(id));
        WaitFrames(2);
        remaining--;
    } while (remaining >= 0);

    UiWindow_DrawPartyStatusContentsFar(gBattleWork->mode);
}
