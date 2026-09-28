#include "IMIRU_FUCHIN.H"

/*
 * The cave-mouth wind: four passes of the dust blower, scheduled as a
 * callback while the map opens.
 */
void FieldScene_RunFourPassCallbackSequence(void)
{
    s32 pass;
    s32 step;
    s32 span;
    s32 one;

    Audio_PlayCue(19);
    Audio_PlayCue(182);
    Event_Begin();
    Battle_ResetEffectCounter();

    /* 8, 7 and 1 are locals held across the loop, not literals: the first
     * call takes 8 as an immediate for argument 4 and from a register for
     * argument 5, which a literal cannot produce. */
    pass = 0;
    step = 8;
    span = 7;
    one = 1;
    do {
        ColorBuffer_ApplyTarget((s32)0x204318, 1);
        ColorBuffer_Interpolate(1);
        Task_Wait(2);
        if (pass == 0) {
            Map_CopyCellsTo(30, 8, 12, 8, step, span);
            Map_CopyCellsTo(30, 57, 19, 57, one, one);
        }
        ColorBuffer_ApplyTarget((s32)0x203108, 1);
        ColorBuffer_Interpolate(1);
        Task_Wait(2);
        /* The increment belongs to the loop test, not the body: `pass++;` as
         * a statement would not place it after the last call.  The compare is
         * unsigned against 3, so the body runs for pass 0 to 3. */
    } while ((unsigned int)++pass <= 3);

    Task_Wait(30);
    /* 0xc80 is built by shifting a small immediate, not loaded whole. */
    Engine_TaskAddCallback((void *)ImiruFuchin_BlowCaveMouthDust, (s32)0xc80);
    Task_Wait(40);
    ColorBuffer_ApplyTarget((s32)0x201090, 1);
    ColorBuffer_Interpolate(40);
    Task_Wait(80);
    Engine_TaskRemoveCallback((void *)ImiruFuchin_BlowCaveMouthDust);
    Task_Wait(20);
    /* 0x10000 is built by shifting a small immediate, not loaded whole. */
    ColorBuffer_ApplyTarget((s32)0x10000, 1);
    ColorBuffer_Interpolate(80);
    /* Same import as in the loop, one argument here. */
    Task_Wait(80);
    /* 0x820 is built by shifting a small immediate, not loaded whole. */
    GameFlag_Set((s32)0x820);
    PartyInventory_Discard(230);
    Audio_PlayCueFromEventWork();
    /* Same import as the first call, no argument register written here. */
    Event_End();
}

void SceneState_SetValue17e1(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_DRAGONS_FLAME_ILLUMINATES_PATH_TRUTH, 1);
    Event_End();
}

void SceneDialogue_RunLine17e2(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_SECRET_KI_SHALL_REVEALED_DISCIPLES, 1);
    Event_End();
}

void FieldScene_RunScriptedStep17E3(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_RAYS_LIGHT_GIVE_BIRTH_SHADOWS, 1);
    Event_End();
}

/*
 * Branch on flag 0x820 -- resource_39a. One arm sets a record flag; the other
 * sets a different flag and writes workspace halfword 370. Nothing is
 * returned, and the owner extends through the three pool words that follow
 * the epilogue.
 */
void SceneState_SetWorkspace370ByFlag820(void)
{

    Event_Begin();
    /* movs r0,#0x82 / lsls r0,#4 builds 0x820. */
    if (GameFlag_IsSet((s32)0x820) != 0) {
        Message_ShowCentered((s32)0x17e5, 1);
    } else {
        Message_ShowCentered((s32)0x17e4, 1);
        if (PartyInventory_FindOwner((s32)0xe6) != -1) {
            u8 *workspace = (u8 *)gEventWork;

            /* movs r1,#0xb9 / lsls r1,#1 gives the byte offset 370. */
            /*
             * The store goes through a pointer local and an s32 value local,
             * in that order. Storing the literal directly builds the constant
             * in HImode and loads it from the literal pool, costing a pool
             * word; splitting the address out first also fixes which register
             * holds it.
             */
            {
                u16 *slot = (u16 *)(workspace + 370);
                s32 one = 1;

                *slot = (u16)one;
            }
        }
    }
    Event_End();
}
