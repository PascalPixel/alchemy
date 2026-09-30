.syntax unified
	.thumb
	.global Func_080e11bc
	.thumb_func
Func_080e11bc:
	push {r5, lr}
	ldr r5, .L_080e1208
	movs r3, #153
	lsls r3, r3, #2
	adds r2, r5, r3
	movs r3, #0
	str r3, [r2]
	bl Func_080cb82c
	movs r2, #154
	lsls r2, r2, #2
	adds r3, r5, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080e11f0
	movs r0, #150
	movs r1, #4
	bl UiText_DrawQuantity
	ldr r0, .L_080e120c
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	b .L_080e1200
.L_080e11f0:
	movs r0, #236
	movs r1, #2
	bl UiText_DrawQuantity
	ldr r0, .L_080e1210
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
.L_080e1200:
	bl Func_080cb8a4
	pop {r5, pc}
	.2byte 0x0000
.L_080e1208:
	.4byte gPartyState
.L_080e120c:
	.4byte 0x00000dbf
.L_080e1210:
	.4byte 0x00000dc1
