#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "call.h"

extern u8 MsgLemuriaStayExploring[];
extern u8 MsgLemuriaStayExploringAlternate[];

s32					  GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void				  Object_SetModeById(u32 id, s32 mode);
void				  Battle_WaitMode0(s32 frames);
void				  ObjectMotion_SetSpeedParameters();
void				  ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);
void				  ObjectMotion_SetPositionAndReset();
void				  ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void				  ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void				  Func_02001914(void);
void				  Func_0200191c(void);
void				  Func_0200195c(s32 id, s32 x, s32 z);
void				  Func_02001994(u8 *message);
void				  Func_020019a4(s32 id, s32 a, s32 b);
void				  Func_020019ec(s32 value);
void				  Func_020019fc(s32 value);
void				  Func_02001a0c();
void				  Func_02001a2c(s32 value);

void Func_02000534(void)
{
	struct ObjectRuntime *object;

	if (!GameFlag_Test(0x961) || GameFlag_Test(0x96a))
	{
		Func_02001a2c(123);
		Func_020019ec(2);
	}
	else
	{
		Func_02001914();
		Func_020019fc(0);
		Call4(Func_02001a0c, 11, 0, -16, 0x5000);
		ObjectMotion_ArmCallback(4, 0xc000, 0);
		ObjectMotion_CommitCurrentPositionAndActivate(11);
		Battle_WaitMode0(20);
		if (GameFlag_Test(7))
		{
			Func_02001994(MsgLemuriaStayExploring);
			Func_020019a4(11, 0, 10);
		}
		else
		{
			Func_02001994(MsgLemuriaStayExploringAlternate);
			Func_020019a4(11, 0, 10);
		}
		Call3(ObjectMotion_SetSpeedParameters, 11, 0x13333, 0x9999);
		Object_SetModeById(11, 2);
		object = Object_GetById(gPartyState.currentOwner);
		if (object != NULL)
			ObjectMotion_ResetAndSetPosition(11, *(s16 *)((u8 *)object + 10), *(s16 *)((u8 *)object + 18));
		ObjectMotion_CommitCurrentPositionAndActivate(11);
		Func_0200195c(11, 0, 0);
		Call3(ObjectMotion_SetPositionAndReset, 4, 168, 0x340);
		Func_0200191c();
	}
}
