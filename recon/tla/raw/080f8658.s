.syntax unified
	.thumb
	.global Func_080f8658
	.thumb_func
Func_080f8658:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r2, #0
	ldr r0, .L_080f86fc
	movs r3, #40
	adds r6, r1, #0
	bl UiText_DrawStringAtOffsetFar
	ldr r3, .L_080f8700
	adds r1, r6, #0
	mov r8, r3
	mov r0, r8
	movs r3, #40
	movs r2, #48
	bl UiText_DrawStringInWindowFar
	movs r3, #52
	ldrsh r5, [r7, r3]
	adds r1, r6, #0
	movs r3, #40
	adds r0, r5, #0
	movs r2, #88
	bl UiText_DrawNumberRightAlignedFar
	movs r3, #56
	ldrsh r5, [r7, r3]
	ldrh r3, [r7, #52]
	lsls r3, r3, #16
	asrs r3, r3, #18
	cmp r5, r3
	bge .L_080f86a0
	movs r0, #4
	bl Func_080380b8
.L_080f86a0:
	cmp r5, #0
	bne .L_080f86aa
	movs r0, #2
	bl Func_080380b8
.L_080f86aa:
	adds r1, r6, #0
	adds r0, r5, #0
	movs r2, #48
	movs r3, #40
	bl UiText_DrawNumberRightAlignedFar
	movs r0, #15
	bl Func_080380b8
	adds r1, r6, #0
	ldr r0, .L_080f8704
	movs r2, #0
	movs r3, #48
	bl UiText_DrawStringAtOffsetFar
	mov r0, r8
	adds r1, r6, #0
	movs r3, #48
	movs r2, #48
	bl UiText_DrawStringInWindowFar
	movs r3, #58
	ldrsh r5, [r7, r3]
	adds r1, r6, #0
	adds r0, r5, #0
	movs r3, #48
	movs r2, #48
	bl UiText_DrawNumberRightAlignedFar
	movs r3, #54
	ldrsh r5, [r7, r3]
	adds r1, r6, #0
	adds r0, r5, #0
	movs r2, #88
	movs r3, #48
	bl UiText_DrawNumberRightAlignedFar
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080f86fc:
	.4byte Data_0810593c
.L_080f8700:
	.4byte Data_08105940
.L_080f8704:
	.4byte Data_08105944
