.syntax unified
	.thumb
	.global Func_08039ed0
	.thumb_func
Func_08039ed0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #60]
	movs r1, #161
	lsls r1, r1, #3
	movs r2, #0
	adds r5, r7, r1
	mov r8, r2
.L_08039ee6:
	ldrh r6, [r5, #22]
	cmp r6, #0
	beq .L_08039f14
	movs r4, #24
	ldrsh r3, [r5, r4]
	cmp r3, #0
	beq .L_08039f04
	adds r0, r5, #0
	movs r1, #0
	bl Func_08039f94
	ldrh r3, [r5, #24]
	subs r3, #1
	strh r3, [r5, #24]
	b .L_08039f82
.L_08039f04:
	movs r1, #26
	ldrsh r3, [r5, r1]
	cmp r3, #0
	beq .L_08039f82
	adds r0, r5, #0
	bl UiWork_DrawByAttributes
	b .L_08039f82
.L_08039f14:
	movs r3, #26
	ldrsh r2, [r5, r3]
	cmp r2, #0
	beq .L_08039f82
	movs r4, #24
	ldrsh r3, [r5, r4]
	cmp r3, r2
	beq .L_08039f4a
	movs r1, #28
	ldrsh r0, [r5, r1]
	movs r2, #30
	ldrsh r1, [r5, r2]
	movs r3, #32
	ldrsh r2, [r5, r3]
	movs r4, #34
	ldrsh r3, [r5, r4]
	bl Func_0803911c
	adds r0, r5, #0
	movs r1, #1
	bl Func_08039f94
	ldrh r3, [r5, #24]
	adds r3, #1
	strh r3, [r5, #24]
	movs r3, #1
	b .L_08039f80
.L_08039f4a:
	movs r1, #28
	ldrsh r0, [r5, r1]
	movs r2, #30
	ldrsh r1, [r5, r2]
	movs r3, #32
	ldrsh r2, [r5, r3]
	movs r4, #34
	ldrsh r3, [r5, r4]
	bl Func_0803911c
	movs r3, #1
	str r6, [r5]
	str r6, [r5, #4]
	strh r6, [r5, #8]
	strh r6, [r5, #10]
	strh r6, [r5, #12]
	strh r6, [r5, #14]
	strh r6, [r5, #16]
	strh r6, [r5, #18]
	strh r6, [r5, #20]
	strh r6, [r5, #22]
	strh r6, [r5, #24]
	strh r6, [r5, #26]
	strh r6, [r5, #28]
	strh r6, [r5, #30]
	strh r6, [r5, #32]
	strh r6, [r5, #34]
.L_08039f80:
	strb r3, [r7, #3]
.L_08039f82:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r5, #36
	cmp r2, #12
	bne .L_08039ee6
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
