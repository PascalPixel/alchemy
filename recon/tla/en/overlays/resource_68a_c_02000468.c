/* Draft, differ 17 of 85: overlay 68a's rising object. The calls match through the call.h
 * forwarders (each keeps its own constant and the native argument order). Left, in the loop:
 * the native reads the scale again after the test (ldr r3, [r5, #24] at the join) where this
 * stores the register, and loads it between the two halves of the limit 0xffff (movs r2, #255;
 * ldr r3; lsls r2, #8). An empty one-pass block after the test does not bring the reload. */
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "call.h"

extern u8 MsgLemuriaStayExploring[];
extern u8 MsgLemuriaStayExploringAlternate[];

void				  GameFlag_SetBit(s32 flag);
void				  GameFlag_ClearBit(s32 flag);
s32					  GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void				  Object_SetModeById(u32 id, s32 mode);
void				  WaitFrames(s32 frames);
void				  Battle_WaitMode0(s32 frames);
void				  ObjectMotion_Launch(s32 id, s32 speed, s32 frames);
void				  ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void				  ObjectMotion_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void				  ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);
void				  ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void				  ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void				  ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void				  Func_02000080(s32 id);
void				  Func_02001914(void);
void				  Func_0200191c(void);
void				  Func_0200195c(s32 id, s32 x, s32 z);
void				  Func_02001994(u8 *message);
void				  Func_020019a4(s32 id, s32 a, s32 b);
void				  Func_020019ec(s32 value);
void				  Func_020019fc(s32 value);
void				  Func_02001a0c(s32 id, s32 a, s32 b, s32 c);
void				  Func_02001a2c(s32 value);

void Func_02000468(void)
{
	struct ObjectRuntime *object;
	s32					  i;

	if (Value1(GameFlag_Test, 0x96d))
		return;
	if (!GameFlag_Test(0x96c))
		return;
	object = Object_GetById(12);
	Call1(GameFlag_SetBit, 0x96d);
	GameFlag_ClearBit(0x96e);
	Func_02000080(13);
	Call3(Func_0200195c, 12, 270 << 16, 184 << 16);
	object->y = 22 << 16;
	ObjectMotion_Launch(12, 11, 0);
	object->scaleY = 0x8000;
	object->scaleX = 0x8000;
	Call3(ObjectMotion_SetSpeedParameters, 12, 0x23333, 0x11999);
	Call3(ObjectMotion_ResetAndSetPositionInMode2, 12, 312, 184);
	for (i = 23; i >= 0; i--)
	{
		object->angle += 0x2000;
		if (object->scaleX <= 0xffff)
			object->scaleX += 0xa00;
		object->scaleY = object->scaleX;
		WaitFrames(1);
	}
	object->angle = 0x3000;
	object->scaleY = 0x10000;
	object->scaleX = 0x10000;
	ObjectMotion_CommitCurrentPositionAndActivate(12);
}

