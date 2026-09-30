.syntax unified
	.thumb
	.global Func_0811ccf0
	.thumb_func
Func_0811ccf0:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl Owner_GetState
	movs r2, #56
	ldrsh r3, [r0, r2]
	cmp r3, #0
	bgt .L_0811cd2e
	adds r0, r6, #0
	bl GetBattleObjectSlot
	ldr r3, [r0]
	movs r1, #5
	ldr r5, [r3, #80]
	adds r0, r5, #0
	bl Animation_ApplyChildArgumentFar
	ldr r2, [r5, #40]
	movs r3, #6
	strb r3, [r2, #5]
	movs r3, #255
	strb r3, [r2, #22]
	movs r0, #4
	bl WaitFrames
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotionFar + 0x10
	adds r0, r6, #0
	bl Func_0811bc64
.L_0811cd2e:
	pop {r5, r6, pc}
