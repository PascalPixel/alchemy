.syntax unified
	.thumb
	.global Func_080fb554
	.thumb_func
Func_080fb554:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #15
	adds r6, r1, #0
	bl UiText_DrawNumberInWindowFar + 0x8
	movs r2, #1
	movs r3, #0
	ldrsb r3, [r5, r3]
	negs r2, r2
	mov r8, r2
	cmp r3, r8
	bne .L_080fb578
	movs r0, #14
	bl UiText_DrawNumberInWindowFar + 0x8
.L_080fb578:
	ldr r7, .L_080fb634
	movs r3, #24
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r0, #15
	bl UiText_DrawNumberInWindowFar + 0x8
	movs r3, #1
	ldrsb r3, [r5, r3]
	cmp r3, r8
	bne .L_080fb59a
	movs r0, #14
	bl UiText_DrawNumberInWindowFar + 0x8
.L_080fb59a:
	movs r3, #24
	adds r0, r7, #1
	adds r1, r6, #0
	movs r2, #40
	bl UiText_DrawCharacterAtOffsetFar
	movs r0, #15
	bl UiText_DrawNumberInWindowFar + 0x8
	movs r3, #3
	ldrsb r3, [r5, r3]
	cmp r3, r8
	bne .L_080fb5ba
	movs r0, #14
	bl UiText_DrawNumberInWindowFar + 0x8
.L_080fb5ba:
	movs r3, #32
	adds r0, r7, #2
	adds r1, r6, #0
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r0, #15
	bl UiText_DrawNumberInWindowFar + 0x8
	movs r3, #5
	ldrsb r3, [r5, r3]
	cmp r3, r8
	bne .L_080fb5da
	movs r0, #14
	bl UiText_DrawNumberInWindowFar + 0x8
.L_080fb5da:
	movs r3, #32
	adds r0, r7, #3
	adds r1, r6, #0
	movs r2, #80
	bl UiText_DrawCharacterAtOffsetFar
	movs r0, #15
	bl UiText_DrawNumberInWindowFar + 0x8
	movs r3, #2
	ldrsb r3, [r5, r3]
	cmp r3, r8
	bne .L_080fb5fa
	movs r0, #14
	bl UiText_DrawNumberInWindowFar + 0x8
.L_080fb5fa:
	movs r3, #24
	adds r0, r7, #4
	adds r1, r6, #0
	movs r2, #80
	bl UiText_DrawCharacterAtOffsetFar
	movs r0, #15
	bl UiText_DrawNumberInWindowFar + 0x8
	movs r3, #4
	ldrsb r3, [r5, r3]
	cmp r3, r8
	bne .L_080fb61a
	movs r0, #14
	bl UiText_DrawNumberInWindowFar + 0x8
.L_080fb61a:
	adds r0, r7, #5
	adds r1, r6, #0
	movs r2, #40
	movs r3, #32
	bl UiText_DrawCharacterAtOffsetFar
	movs r0, #15
	bl UiText_DrawNumberInWindowFar + 0x8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080fb634:
	.4byte 0x00001063
