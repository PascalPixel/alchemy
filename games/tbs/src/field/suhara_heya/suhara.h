#ifndef SUHARA_H
#define SUHARA_H

#include "types.h"
#include "field_service.h"
#include "field_scene.h"
#include "scene.h"

struct SceneRecord
{
	u8	unk_00[9];
	u8	flags;
	u8	unk_0a[20];
	u16 angle;
	u8	unk_20[6];
	u8	field_26;
};

union SceneActor
{
	struct
	{
		u8					unk_00[12];
		s32					y;
		u8					unk_10[64];
		struct SceneRecord *record;
		u8					unk_54;
		u8					mode;
		u8					unk_56[3];
		u8					flags;
	} fields;
	u8 bytes[90];
};

extern struct SceneEntrance	 gSuharaHeyaEntrances[];
extern s16					 gSuharaHeyaRegions[];
extern u32					 gSuharaHeyaExits[];
extern struct ScenePlacement gSuharaHeyaPlacements[];
extern struct ScenePlacement gSuharaHeyaPlacements96f[];
extern struct SceneEvent	 gSuharaHeyaEvents[];
extern struct SceneEvent	 gSuharaHeyaEvents96f[];

void Audio_PlayCueFromEventWork(void);
void Engine_InnCheckIn(s32 first, s32 second);
void Engine_ShopConfirmAct(s32 first);
#endif
