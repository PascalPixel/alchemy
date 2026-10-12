#include "ov660.h"

void GameFlag_ClearBit(s32 flag);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void ObjectMotion_EnableActionAndSetCallback(s32 id, s32 action);
void Func_02000710(void);
void Func_020007dc(s32 value);
void Func_0200081c(s32 id, s32 x, s32 z);
void Func_02000824(s32 id, s32 x, s32 z, s32 angle);
void Func_0200087c(s32 value);
void Func_02000894(s32 id, s32 angle);

s32 Func_02000580(void)
{
	s32 turn;

	*(s32 *)((u8 *)Ram_HeapSlots->event_work + 0x1ac) = 521;
	Object_GetById(gPartyState.current_owner)->priority_flags |= 32;
	Func_0200087c(0);
	if (gPartyState.scene == (s32)&SceneId_Ov660Room1)
	{
		if (GameFlag_Test(0x91b) || gPartyState.entrance == 99)
		{
			Func_0200081c(19, 0, 0);
			Func_0200081c(20, 0, 0);
			ObjectMotion_EnableActionAndSetCallback(18, 3);
		}
		else
		{
			{
				s32 x;
				s32 z;
				s32 id;

				turn = 0x2000;
				/* FAKEMATCH: the native builds the angle 0x2000, which both objects take, before the other arguments; unforced it is built after the id (differ 6). */
				asm volatile("" : "+l"(turn));
				x = 153;
				/* FAKEMATCH: the native loads the id between the two halves of the x 306 << 16; written whole, the x is finished first (differ 2). */
				__asm__("" : "+l"(x));
				id = 17;
				/* FAKEMATCH: and this forced id keeps its load right after that half; unforced it loads last. */
				asm volatile("" : "+l"(id));
				x <<= 17;
				z = 335 << 16;
				/* FAKEMATCH: and the finished x and the z keep their loads before the angle is copied; unforced the copy comes first. */
				asm volatile("" : "+l"(x), "+l"(z));
				Func_02000824(id, x, z, turn);
			}
			{
				s32 angle = 0x6000;
				s32 z = 336 << 16;

				/* FAKEMATCH: the native starts the angle before the z; the plain call starts the z first (differ 4). */
				asm volatile("" : "+l"(angle), "+l"(z));
				Func_02000824(18, 345 << 16, z, angle);
			}
			ObjectDispatch_SetSingleChildFlags(Object_GetById(19), 0);
			ObjectDispatch_SetSingleChildFlags(Object_GetById(20), 0);
			Object_GetById(17)->motion_flags = 0;
			Object_GetById(17)->y = 0;
			Object_GetById(18)->motion_flags = 0;
			Object_GetById(18)->y = 0;
			Object_GetById(19)->motion_flags = 0;
			Object_GetById(19)->y = 0;
			Object_GetById(20)->motion_flags = 0;
			Object_GetById(20)->y = 0;
			Func_02000894(17, 0xe000);
			ObjectDispatch_SetSingleChildFlags(Object_GetById(17), 0);
			Func_02000894(18, turn);
			ObjectDispatch_SetSingleChildFlags(Object_GetById(18), 0);
		}
		if (gPartyState.entrance == 99 && !GameFlag_Test(0x91b))
			Func_02000710();
	}
	if (GameFlag_Test(0x91b))
		GameFlag_ClearBit(0x310);
	if (GameFlag_Test(0x310))
		Func_020007dc(0xa00);
	return 0;
}
