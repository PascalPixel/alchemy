.syntax unified
	.thumb
	.global Func_080397e0
	.thumb_func
Func_080397e0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	movs r2, #215
	lsls r2, r2, #3
	adds r5, r3, r2
	movs r3, #1
	movs r7, #0
	mov r8, r3
.L_080397f8:
	ldr r2, [r5]
	cmp r2, #0
	beq .L_08039856
	ldr r3, [r2, #24]
	cmp r3, #0
	bne .L_08039856
	ldrh r3, [r2, #22]
	cmp r3, #0
	bne .L_0803980e
	str r3, [r5]
	b .L_08039856
.L_0803980e:
	ldrh r6, [r2, #18]
	cmp r6, #0
	beq .L_0803981c
	adds r0, r5, #0
	bl Func_0803cbf8
	b .L_08039856
.L_0803981c:
	adds r0, r5, #0
	bl Func_08039864
	cmp r0, #8
	beq .L_0803982c
	cmp r0, #9
	beq .L_08039832
	b .L_08039856
.L_0803982c:
	ldr r3, [r5]
	mov r2, r8
	b .L_08039854
.L_08039832:
	ldr r0, [r5]
	movs r1, #2
	ldrh r3, [r0, #22]
	ands r1, r3
	lsls r1, r1, #16
	lsrs r1, r1, #16
	bl UiWork_Finalize
	ldr r3, [r5]
	mov r2, r8
	strh r6, [r5, #4]
	strh r6, [r5, #6]
	strh r6, [r5, #18]
	strh r6, [r5, #20]
	strh r6, [r5, #22]
	strh r6, [r5, #24]
	strh r6, [r5, #26]
.L_08039854:
	strh r2, [r3, #20]
.L_08039856:
	adds r7, #1
	adds r5, #40
	cmp r7, #3
	bne .L_080397f8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
