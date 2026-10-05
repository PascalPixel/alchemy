#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/*
 * The Luna and Sol rooms deep in Sol Sanctum. Sukureta wonders what the two
 * rooms mean and asks Robin to look farther up the passage. Lighting the four
 * statue lights turns the picture of Luna to Sol, and the trap that follows
 * shakes the statue hall.
 */

enum InnerSanctumActor {
    ACTOR_SUKURETA = ACTOR_FIRST_PLACED + 8
};

enum InnerSanctumFlag {
    FLAG_INNER_SANCTUM_ENTERED = 0x80a,
    /* The four statue lights. */
    FLAG_STATUE_LIGHT_1 = 0x80b,
    FLAG_STATUE_LIGHT_2,
    FLAG_STATUE_LIGHT_3,
    FLAG_STATUE_LIGHT_4,
    FLAG_LUNA_PICTURE_CHANGED_TO_SOL = 0x810,
    FLAG_STATUE_TRAP_SPRUNG = 0x813,
    /* Robin agreed to look farther up the passage for Sukureta. */
    FLAG_ROBIN_SEARCHING_FOR_SUKURETA = 0x896
};

void SetSolShindenActorStep();
void SetSolShindenActorStep();

s32 CheckAllStatueLights(void);

void SetSolShindenActorStep(s32 actor_step, s32 wait_frames);

#include "TYPES.H"

/* Message IDs used by Engine_EventSetMessage. */

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))
void FieldScene_RunScene37aSequenceA();
void SetSolShindenActorStep();

#include "TYPES.H"

#include "TYPES.H"

void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();

/* The scene step counter at 0x1d8 of the shared scene work record. */

/*
 * Inner sanctum scene state: clear the backdrop colour, light the statue
 * groups whose progress flags are set, and step an actor. Overlay
 * resource_37a.
 */

#include "TYPES.H"

#include "TYPES.H"

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} EntA;

typedef struct {
    u8 filler0[10];
    s16 unkA;
    u8 fillerC[6];
    s16 unk12;
} EntB;

extern u16 SoruNichigetsu_Light1Timer;
extern s32 SoruNichigetsu_FlashState;
extern u16 SoruNichigetsu_Light2Timer;
extern u16 SoruNichigetsu_Light3Timer;
extern u16 SoruNichigetsu_Light4Timer;

#include "TYPES.H"

void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();
void UpdateStatueLight1(void);
void UpdateStatueLight2(void);
void UpdateStatueLight3(void);
void UpdateStatueLight4(void);

/* The scene step counter at 0x1d8 of the shared scene work record. */

#include "TYPES.H"

void SetSolShindenActorStep();
void SetSolShindenActorStep();
