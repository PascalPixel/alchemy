.syntax unified
	.thumb
	.global Func_08043690
	.thumb_func
Func_08043690:
	push {r5, r6, r7, lr}
	movs r7, #0
	bl Func_0801596c
	cmp r0, #0
	beq .L_080436a8
	ldr r0, .L_0804375c
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	subs r7, #9
	b .L_08043752
.L_080436a8:
	bl Func_08016054
	ldr r3, .L_08043760
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #0
	bl Func_08043cd8
	movs r2, #1
	adds r5, r0, #0
	negs r2, r2
	cmp r5, r2
	bne .L_080436c6
	adds r7, r5, #0
	b .L_08043752
.L_080436c6:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #204
	movs r1, #192
	ldr r2, [r3]
	lsls r1, r1, #6
	lsls r3, r5, #6
	adds r1, #88
	adds r3, r3, r1
	ldrb r3, [r2, r3]
	cmp r3, #0
	beq .L_08043710
	ldr r0, .L_08043764
	movs r1, #13
	bl UiText_ShowPositionedMessageAndWait
	b .L_080436ee
.L_080436e8:
	movs r0, #1
	bl WaitFrames
.L_080436ee:
	bl UiWork_IsComplete
	cmp r0, #0
	beq .L_080436e8
	movs r0, #1
	movs r1, #0
	movs r2, #0
	movs r3, #1
	bl Menu_RunConfirmSelection
	cmp r0, #0
	beq .L_0804370c
	bl Func_0803ce1c
	b .L_08043752
.L_0804370c:
	bl Func_0803ce1c
.L_08043710:
	ldr r3, .L_08043760
	movs r0, #0
	strh r5, [r3]
	bl Func_08042e28
	adds r6, r0, #0
	bl Func_08043358
	bl Func_080c8628
	ldr r1, .L_08043768
	adds r0, r5, #0
	bl Func_08042f10
	adds r5, r0, #0
	adds r0, r6, #0
	bl Func_08042eec
	bl Func_0803ce1c
	cmp r5, #0
	beq .L_0804374a
	ldr r0, .L_0804376c
	movs r1, #1
	movs r7, #3
	bl UiText_ShowPositionedMessageAndWait
	negs r7, r7
	b .L_08043752
.L_0804374a:
	ldr r0, .L_08043770
	movs r1, #9
	bl UiText_ShowPositionedMessageAndWait
.L_08043752:
	bl Func_0801613c
	adds r0, r7, #0
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0804375c:
	.4byte 0x0000000b
.L_08043760:
	.4byte Data_020036d0
.L_08043764:
	.4byte 0x00000015
.L_08043768:
	.4byte Data_02000000
.L_0804376c:
	.4byte 0x0000000c
.L_08043770:
	.4byte 0x00000019
