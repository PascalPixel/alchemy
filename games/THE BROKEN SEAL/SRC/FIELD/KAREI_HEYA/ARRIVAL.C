#include "TYPES.H"
#include "FIELD_EVENT.H"

#include "RESOURCE_3A9.H"

/* Table selection, dialogue and arrival scripts for resource_3a9. */
typedef struct Placement {
    u32 destination;
    u16 x;
    u16 y;
} Placement;

extern s16 Data_02000240[];     /* The shared work area, in RAM. */
extern u8 Value_00000064;
extern u8 Value_00000065;
extern u8 Data_020084d0[];
extern u8 Data_020086c8[];
extern u8 Data_020084a0[];
extern Placement Data_02008ef8[];   /* In-image placement table, four entries. */
extern s16 Data_02000240[];
extern u8 Data_020088d4[];
extern u8 Data_0200879c[];
extern u8 Data_02008a0c[];
extern u8 Data_02008784[];
extern u8 Data_02008c88[];
extern u8 Data_02008a48[];
extern u8 Data_02008eb0[];
extern u8 Data_02008a3c[];

u8 *Func_020005ba(s32);
u8 *Func_0200062a(int);
u8 *Func_020006e6();
void Func_0200065e(void);
void Func_020004d0(void *);

/* Old-style declarations where an overlay import varies in arity between its
 * call sites. */

/* Picks one of three scene tables by scene id. */
s32 SceneData_SelectTableBySceneId(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000064) {
        return (s32)Data_020084d0;
    }
    if (v == (s32)&Value_00000065) {
        return (s32)Data_020086c8;
    }
    return (s32)Data_020084a0;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

/*
 * Returns the in-image table at 0x02008728. The eight-byte owner includes its
 * one pool word, which holds that address and is returned without being
 * dereferenced.
 */
u8 *SceneData_GetTable8728(void)
{
    return (u8 *)0x02008728;
}

/* Picks a table by scene id and sub-state, and hands the chosen one to
 * Func_020004d0 before returning it. */
u8 *SceneData_SelectAndPrepareTable(void)
{
    s32 id = gGameState.scene;
    if (id == (s32)&Value_00000064) {
        s32 state = gGameState.entrance;
        u8 *tbl;
        switch (state) {
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 17:
            tbl = Data_020088d4;
            break;
        default:
            tbl = Data_0200879c;
            break;
        }
        Func_020004d0(tbl);
        return tbl;
    }
    if (id == (s32)&Value_00000065) {
        return Data_02008a0c;
    }
    return Data_02008784;
}

/* The same selection without the hand-off. Sub-state 16 falls to the default
 * arm even though it lies inside 9..17; that hole is deliberate. */
u8 *SceneData_SelectSubStateTable(void)
{
    s32 id = gGameState.scene;
    if (id == (s32)&Value_00000064) {
        s32 state = gGameState.entrance;
        switch (state) {
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 17:
            return Data_02008c88;
        default:
            return Data_02008a48;
        }
    }
    if (id == (s32)&Value_00000065) {
        return Data_02008eb0;
    }
    return Data_02008a3c;
}

void SceneDialogue_RunActor12DialogueAndSetFlag910(void)
{
    Event_Begin();
    Event_SetMessage(0x1ADD);
    Event_ShowMessage(0xC, 0);
    GameFlag_Set(0x910);
    Event_End();
}

void SceneDialogue_RunActor16Dialogue(void)
{
    Event_Begin();
    Event_SetMessage(0x1AE3);
    Event_AskYesNo(16, 0);
    Event_End();
}

/* Actor 8's dialogue, branched on flag 0x911. Engine_EventShowMessage and
 * Engine_EventSetMessage are two imports sharing one call word: the two-argument
 * gesture in the first arm, the one-argument message in the second. */
void SceneDialogue_RunActor8FlaggedDialogue(void)
{
    u8 *p = Func_020005ba(0);

    /* Band guard: facing in 0x6001..0x9fff. The test is spelled as the short
     * arm's condition, which is what reproduces the branch. */
    if ((u16)(*(u16 *)(p + 6) - 0x6001) <= 0x3FFE) {
        Inn_Open(7, 8);
    } else {
        Event_Begin();

        if (GameFlag_IsSet(0x911) != 0) {
            Event_SetMessage(0x1AFB);
            Event_ShowMessage(8, 0);
        } else {
            Event_SetMessage(0x1AD7);
            Event_AskYesNo(8, 0);
            GameFlag_Set(0x910);           /* 145 << 4 */
        }

        Event_End();
    }
}

void SceneDialogue_RunActor8FacingDialogue(void)
{
    void Event_SetMessage(int);

    u8 *p = Func_0200062a(0);

    /* Band guard: facing in 0xa001..0xdfff. The test is spelled as the short
     * arm's condition, which is what reproduces the branch. */
    if ((u16)(*(u16 *)(p + 6) + 0x5FFF) <= 0x3FFE) {
        Sanctum_Open(8);
    } else {
        Event_Begin();
        Event_SetMessage(0x1A8F);
        Event_ShowMessage(8, 0);
        Event_End();
    }
}

/*
 * Scene arrival: clears the residue byte at +85 of every scene slot from 8 to
 * 65, then looks the sub-state up in the placement table and places the
 * player from that entry. Sub-states other than 12, 13, 16 and 19 return
 * without touching anything. The loop skips a null record but the later clear
 * of the player's own +85 does not test for null; that asymmetry is real.
 * 158 is read as a cue id from its argument position and is not verified.
 */
void FieldScene_RunArrivalPlacement(void)
{
    u8 *Func_0200067a_a();

    u8 *work = *(u8 **)0x03001ebc;
    u32 slot;
    s32 idx;
    u8 *p;

    Event_Begin();

    for (slot = 8; slot <= 65; slot++) {
        u8 *rec = Func_0200067a_a(slot);

        if (rec != 0) {
            rec[85] = 0;
        }
    }

    /* The sub-state slot is read twice, here and for Engine_EventRequestExit below, and
     * both reads are kept. */
    switch (*(s16 *)(work + 364)) {         /* 182 << 1 */
    case 12: idx = 0; break;
    case 13: idx = 1; break;
    case 16: idx = 2; break;
    case 19: idx = 3; break;
    default: return;
    }

    Audio_PlayCue(158);

    {
        u32 x = Data_02008ef8[idx].x;
        u32 y = Data_02008ef8[idx].y;

        Map_AnimateCells(Data_02008ef8[idx].destination, x, y);
    }

    Actor_SetSpeed(0, 0x00008000, 0x00004000);

    p = Func_020006e6(0);
    p[85] = 0;

    Actor_SetAnimation(0, 2);
    Actor_CenterAndWalk(0, 3, -8);
    Event_Wait(10);

    Event_RequestExit(*(s16 *)(work + 364));
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
}

/* Publishes 0x209 at +448 of the runtime record, and calls Func_0200065e for
 * scene 0x64. */
s32 SceneState_SetRuntimeWord448To521(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    if (gGameState.scene == (s32)(u32)&Value_00000064) {
        Func_0200065e();
    }
    return 0;
}

/*
 * Clears the set of scene slots this sub-state leaves behind. Sub-state 16
 * takes the last arm even though it lies inside 9..17, so the test is not
 * written as a range. 0x911 is read as an event-flag id from its argument
 * position, and the six-argument call's argument meanings are not
 * established.
 */
void SceneState_ClearSlotsBySubState(void)
{
    s16 sub = gGameState.entrance;

    switch (sub) {
    case 3:
    {
        /* The last two arguments travel on the stack. */
        s32 fifth = 4;
        s32 sixth = 2;
        Map_CopyCellsTo(30, 14, 30, 16, fifth, sixth);
        return;
    }
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 17:
        break;
    default:
        goto other;
    }

    /* sub is 9..15 or 17. */
    if (GameFlag_IsSet(0x911) != 0) {
        /* Nine distinct call sites, not a loop; the trailing 15 is out of
         * order and is kept that way. */
        Actor_Destroy(10);
        Actor_Destroy(11);
        Actor_Destroy(12);
        Actor_Destroy(13);
        Actor_Destroy(14);
        Actor_Destroy(17);
        Actor_Destroy(18);
        Actor_Destroy(19);
        Actor_Destroy(15);
    } else {
        Actor_SetChildValue(13, 2);
    }
    return;

other:
    if (GameFlag_IsSet(0x911) != 0) {
        Actor_Destroy(16);
        Actor_Destroy(17);
    }
}
