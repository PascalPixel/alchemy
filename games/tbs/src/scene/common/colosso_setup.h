#include "types.h"
#include "field_obj.h"
#include "field_event.h"

void FieldScene_BuildSceneDescriptorAndInstallTask(
	s32 first, s32 second, s32 mode, s32 centre, s32 extra, s32 third, s32 fourth)
{

	u8				  *desc;
	struct FieldActor *rec0;
	struct FieldActor *rec1;
	s32				   handle;
	s32				   pal;

	desc = Runtime_AllocateBlock(59, 0x7170);
	handle = (s32)Runtime_BumpAllocateAlternatePool(512);

	*(u16 *)(desc + 222) = first;
	*(u16 *)(desc + 224) = second;
	*(u16 *)(desc + 226) = third;
	*(u16 *)(desc + 228) = fourth;
	*(u16 *)(desc + 230) = mode;
	*(s32 *)(desc + 232) = centre;
	*(s32 *)(desc + 236) = extra;

	rec0 = Object_GetById(first);
	rec1 = Object_GetById(second);

	if (Engine_GameFlagIsSet(0x109) == 0)
	{
		rec1->x.fixed = (centre << 1) - rec0->x.fixed;
		rec1->z.fixed = rec0->z.fixed;
	}

	*(u16 *)(desc + 218) = 0;
	*(u16 *)(desc + 220) = 0;

	FieldResource_DecodeType01(gColosso_GaugeGraphics, handle);

	pal = Resource_FindFreeEntry();
	*(u16 *)(desc + 216) = pal;
	Engine_VramLoad((s16)pal, 512, handle);

	FieldScheduler_AddCallback((s32)Colosso_DrawGauge + 1, 0xc76);

	Runtime_BumpFree(handle);
}

void SceneState_InitControlWhenFlag109Clear(s32 resource)
{
	u8					*work = gColossoWork;
	struct PathRecorder *recorder = (struct PathRecorder *)gSceneState;

	FieldResource_DecodeType01(Resource_GetTableEntry(resource), work + 240);
	if (Engine_GameFlagIsSet(0x109) == 0)
	{
		recorder->mode = 1;
		recorder->mirror = 1;
		recorder->actor = *(u16 *)(work + 224);
		recorder->still = 0;
		recorder->pos = 0;
	}
	FieldScheduler_AddCallback(Colosso_UpdatePathRival, 0xc85);
}
