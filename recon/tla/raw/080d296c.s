.syntax unified
	.thumb
	.global Func_080d296c
	.thumb_func
Func_080d296c:
	push {r5, r6, r7, lr}
	adds r5, r1, #0
	adds r6, r0, #0
	adds r0, r5, #0
	adds r7, r2, #0
	bl Func_080ad2f0
	cmp r0, #0
	bge .L_080d29d6
	adds r0, r6, #0
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #24]
	movs r1, #3
	adds r0, r5, #0
	bl Func_080cf424
	adds r0, r5, #0
	movs r1, #2
	bl UiText_DrawQuantity
	ldr r5, .L_080d2a04
	movs r1, #1
	adds r0, r5, #0
	adds r5, #4
	bl UiText_ShowPositionedMessageAndWaitFar
	movs r1, #1
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWaitFar
	adds r0, r6, #0
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #24]
	ldr r3, .L_080d2a08
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	adds r1, r6, #0
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl EventRuntime_Wait
	movs r0, #1
	negs r0, r0
	b .L_080d2a00
.L_080d29d6:
	movs r2, #0
	adds r0, r6, #0
	movs r1, #0
	bl Func_080d3118
	adds r0, r5, #0
	movs r1, #3
	bl Func_080cf424
	adds r0, r5, #0
	movs r1, #0
	bl PartyInventory_GiveItem
	movs r3, #1
	negs r3, r3
	cmp r7, r3
	beq .L_080d29fe
	adds r0, r7, #0
	bl GameFlag_SetBit
.L_080d29fe:
	movs r0, #0
.L_080d2a00:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d2a04:
	.4byte 0x00000e0f
.L_080d2a08:
	.4byte gPartyState
