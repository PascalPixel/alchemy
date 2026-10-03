.syntax unified
	.thumb
	.global Func_080d2cc4
	.thumb_func
Func_080d2cc4:
	push {r5, lr}
	adds r5, r0, #0
	bl EventRuntime_Begin
	movs r0, #0
	bl EventRuntime_ResolvePendingActions
	movs r1, #1
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWaitFar
	movs r0, #161
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	bl EventRuntime_End
	movs r0, #136
	bl Func_080ad2e8
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	beq .L_080d2d04
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #173
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
.L_080d2d04:
	pop {r5, pc}
	.2byte 0x0000
