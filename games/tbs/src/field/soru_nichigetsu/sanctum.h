#include "types.h"
#include "field_event.h"
#include "field_scene.h"

enum InnerSanctumActor
{
	ACTOR_SUKURETA = ACTOR_FIRST_PLACED + 8
};

enum InnerSanctumFlag
{
	FLAG_INNER_SANCTUM_ENTERED = 0x80a,
	FLAG_STATUE_LIGHT_1 = 0x80b,
	FLAG_STATUE_LIGHT_2,
	FLAG_STATUE_LIGHT_3,
	FLAG_STATUE_LIGHT_4,
	FLAG_LUNA_PICTURE_CHANGED_TO_SOL = 0x810,
	FLAG_STATUE_TRAP_SPRUNG = 0x813,
	FLAG_ROBIN_SEARCHING_FOR_SUKURETA = 0x896
};

void SetSolShindenActorStep();
void SetSolShindenActorStep();

s32 CheckAllStatueLights(void);

void SetSolShindenActorStep(s32 actor_step, s32 wait_frames);

#include "types.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))
void FieldScene_RunScene37aSequenceA();
void SetSolShindenActorStep();

#include "types.h"

#include "types.h"

void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();

#include "types.h"

#include "types.h"

typedef struct
{
	u8	filler0[8];
	s32 unk8;
	u8	fillerC[4];
	s32 unk10;
} EntA;

typedef struct
{
	u8	filler0[10];
	s16 unkA;
	u8	fillerC[6];
	s16 unk12;
} EntB;

extern u16 SoruNichigetsu_Light1Timer;
extern s32 SoruNichigetsu_FlashState;
extern u16 SoruNichigetsu_Light2Timer;
extern u16 SoruNichigetsu_Light3Timer;
extern u16 SoruNichigetsu_Light4Timer;

#include "types.h"

void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();
void UpdateStatueLight1(void);
void UpdateStatueLight2(void);
void UpdateStatueLight3(void);
void UpdateStatueLight4(void);

#include "types.h"

void SetSolShindenActorStep();
void SetSolShindenActorStep();
