/* Draft, differ 24 of 89: a scene prompt of overlay 693. The calls and branches match. Left:
 * the native builds each party state field offset in r2 and the address in r3 (movs r2, #128;
 * lsls; adds r2, #118; adds r3, r5, r2), and the first halfword index of the position in r2;
 * this takes r3 for the offset and the index. It also loads the second argument before the
 * first at UiText_OpenMessageAtObject(18, 0), through call.h too (the speed call matches
 * through it). The party state through a forced pointer measured 28. */
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "call.h"

extern u8 MsgAreWeSetting[];

void				  GameFlag_SetBit(s32 flag);
s32					  GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void				  Object_SetModeById(u32 id, s32 mode);
void				  UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32					  Inventory_PromptAndSetObjectMode(s32 actor, s32 flags);
void				  ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void				  ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);
void				  ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void				  Func_02001010(void);
void				  Func_02001018(void);
void				  Func_02001060(s32 id, s32 x, s32 z);
void				  Func_02001098(u8 *message);
void				  Func_020010a8(s32 id, s32 value);
void				  Func_020010f0(s32 value);
void				  Func_02001108(s32 value);

void Func_02000120(void)
{
	struct ObjectRuntime *object;

	Func_02001010();
	Func_02001108(0);
	Func_02001098(MsgAreWeSetting);
	Call2(UiText_OpenMessageAtObject, 18, 0);
	if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 0)
	{
		Object_SetModeById(18, 3);
		Func_020010a8(18, 0);
		gPartyState.owner_override = 1;
		if (GameFlag_Test(0x9d0))
		{
			GameFlag_SetBit(0x8df);
			GameFlag_SetBit(0x9b2);
		}
		Func_020010f0(4);
	}
	else
	{
		(*(u16 *)((u8 *)Ram_HeapSlots->event_work + 0x1c4))++;
		Func_020010a8(18, 0);
		Call3(ObjectMotion_SetSpeedParameters, 18, 0x19999, 0xcccc);
		Object_SetModeById(18, 2);
		object = Object_GetById(gPartyState.current_owner);
		if (object != NULL)
			ObjectMotion_ResetAndSetPosition(18, *(s16 *)((u8 *)object + 10), *(s16 *)((u8 *)object + 18));
		ObjectMotion_CommitCurrentPositionAndActivate(18);
		Func_02001060(18, 0, 0);
		GameFlag_SetBit(0x201);
		Func_02001018();
	}
}
