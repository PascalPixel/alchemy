.syntax unified
	.thumb
	.global Func_080f8708
	.thumb_func
Func_080f8708:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r2, #0
	adds r5, r1, #0
	mov r8, r0
	adds r1, r6, #0
	ldr r0, .L_080f8834
	movs r2, #0
	movs r3, #32
	sub sp, #4
	bl UiText_DrawCharacterAtOffsetFar
	movs r7, #40
	ldrh r0, [r5, #60]
	adds r2, r6, #0
	movs r3, #16
	movs r1, #3
	str r7, [sp, #0]
	bl UiText_DrawNumberAtOffsetFar
	mov r3, r8
	ldrh r2, [r3, #60]
	ldrh r3, [r5, #60]
	cmp r2, r3
	beq .L_080f8770
	adds r0, r2, #0
	movs r3, #64
	adds r2, r6, #0
	movs r1, #3
	str r7, [sp, #0]
	bl UiText_DrawNumberAtOffsetFar
	mov r3, r8
	ldrh r2, [r3, #60]
	ldrh r3, [r5, #60]
	cmp r2, r3
	bls .L_080f8764
	adds r0, r6, #0
	movs r1, #44
	movs r2, #36
	movs r3, #0
	bl Func_08104b58
	b .L_080f8770
.L_080f8764:
	adds r0, r6, #0
	movs r1, #44
	movs r2, #36
	movs r3, #1
	bl Func_08104b58
.L_080f8770:
	ldr r0, .L_080f8838
	adds r1, r6, #0
	movs r2, #0
	movs r3, #48
	bl UiText_DrawCharacterAtOffsetFar
	movs r7, #56
	ldrh r0, [r5, #62]
	adds r2, r6, #0
	movs r3, #16
	movs r1, #3
	str r7, [sp, #0]
	bl UiText_DrawNumberAtOffsetFar
	mov r3, r8
	ldrh r2, [r3, #62]
	ldrh r3, [r5, #62]
	cmp r2, r3
	beq .L_080f87c8
	adds r0, r2, #0
	movs r3, #64
	adds r2, r6, #0
	movs r1, #3
	str r7, [sp, #0]
	bl UiText_DrawNumberAtOffsetFar
	mov r3, r8
	ldrh r2, [r3, #62]
	ldrh r3, [r5, #62]
	cmp r2, r3
	bls .L_080f87bc
	adds r0, r6, #0
	movs r1, #44
	movs r2, #52
	movs r3, #0
	bl Func_08104b58
	b .L_080f87c8
.L_080f87bc:
	adds r0, r6, #0
	movs r1, #44
	movs r2, #52
	movs r3, #1
	bl Func_08104b58
.L_080f87c8:
	ldr r0, .L_080f883c
	adds r1, r6, #0
	movs r2, #0
	movs r3, #64
	bl UiText_DrawCharacterAtOffsetFar
	adds r7, r5, #0
	movs r3, #72
	adds r7, #64
	mov r5, r8
	ldrh r0, [r7]
	adds r2, r6, #0
	str r3, [sp, #0]
	mov r10, r3
	movs r1, #3
	movs r3, #16
	adds r5, #64
	bl UiText_DrawNumberAtOffsetFar
	ldrh r2, [r5]
	ldrh r3, [r7]
	cmp r2, r3
	beq .L_080f8828
	mov r3, r10
	adds r0, r2, #0
	str r3, [sp, #0]
	adds r2, r6, #0
	movs r3, #64
	movs r1, #3
	bl UiText_DrawNumberAtOffsetFar
	ldrh r2, [r5]
	ldrh r3, [r7]
	cmp r2, r3
	bls .L_080f881c
	adds r0, r6, #0
	movs r1, #44
	movs r2, #68
	movs r3, #0
	bl Func_08104b58
	b .L_080f8828
.L_080f881c:
	adds r0, r6, #0
	movs r1, #44
	movs r2, #68
	movs r3, #1
	bl Func_08104b58
.L_080f8828:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080f8834:
	.4byte 0x0000104b
.L_080f8838:
	.4byte 0x0000104c
.L_080f883c:
	.4byte 0x0000104f
