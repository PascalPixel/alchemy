/* Draft of SceneState_SetRuntimeWord448To521, resource_3a9 at 0x02008308 (split from FIELD/KAREI_HEYA/ARRIVAL.C).
 * Remaining difference: it loads constants through address-derived symbols (Value_/Data_0000/LinkedMessage_ names) that no link defines, so the overlay keeps its listing rows. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "RESOURCE_3A9.H"

enum ArrivalMessage {
    MSG_CAN_LIVE_IN_PEACE_IN = 0x1a8f,
    MSG_GOING_TOLBI_ALSO = 0x1ad7,
    MSG_PLEASE_FINISH_EATING_IF_TAKING = 0x1add,
    MSG_DO_KNOW_ABOUT_CONTINENT_SOUTH = 0x1ae3,
    MSG_OUR_INN_FEELS_EMPTY_NOW = 0x1afb
};

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
void Func_020004d0(void *);

/* Publishes 0x209 at +448 of the runtime record, and calls SceneState_ClearSlotsBySubState for
 * scene 0x64. */
s32 SceneState_SetRuntimeWord448To521(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    if (gGameState.scene == (s32)(u32)&Value_00000064) {
        SceneState_ClearSlotsBySubState();
    }
    return 0;
}
void SceneState_ClearSlotsBySubState(void);
