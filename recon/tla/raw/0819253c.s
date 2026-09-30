.syntax unified
	.thumb
	.global Func_0819253c
	.thumb_func
Func_0819253c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #176
	ldr r3, [r3]
	ldr r1, [r2, #92]
	mov r9, r3
	movs r2, #128
	ldr r3, .L_08192584
	lsls r2, r2, #19
	adds r2, #20
	strh r3, [r2]
	ldr r2, .L_08192588
	movs r3, #240
	str r3, [r2, #16]
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #240
	adds r1, r1, r2
	ldr r3, [r1]
	sub sp, #28
	ldr r3, [r3, #20]
	movs r7, #0
	cmp r3, #0
	beq .L_081925c8
	adds r6, r1, #0
	movs r3, #0
	movs r1, #36
	mov r10, r3
	mov r8, r1
	b .L_0819258c
.L_08192584:
	.4byte 0x00000000
.L_08192588:
	.4byte gCameraSceneParameters
.L_0819258c:
	ldr r3, [r6]
	mov r2, r8
	ldrsh r0, [r3, r2]
	bl Func_08118088 + 0x10
	ldr r5, [r0]
	mov r2, r10
	str r2, [r5, #8]
	bl Random16
	movs r2, #15
	ands r2, r0
	movs r3, #16
	subs r3, r3, r2
	lsls r3, r3, #16
	str r3, [r5, #12]
	ldr r3, [r5, #16]
	movs r1, #2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5, #16]
	mov r3, r10
	str r3, [r5, #72]
	ldr r3, [r6]
	adds r7, #1
	ldr r3, [r3, #20]
	add r8, r1
	cmp r7, r3
	bne .L_0819258c
.L_081925c8:
	mov r0, sp
	movs r3, #255
	strh r3, [r0]
	movs r1, #0
	bl Func_08118010
	ldr r1, .L_08192610
	movs r0, #1
	movs r2, #0
	bl Func_08118028 + 0x18
	ldr r5, .L_0819260c
	movs r4, #160
	lsls r4, r4, #19
	adds r4, #192
	movs r7, #0
.L_081925e8:
	ldrh r2, [r4]
	movs r0, #31
	lsls r3, r2, #16
	lsrs r1, r3, #26
	ands r0, r2
	lsrs r3, r3, #21
	ands r1, r5
	ands r3, r5
	subs r0, #16
	subs r3, #24
	subs r1, #20
	cmp r0, #0
	bge .L_08192604
	movs r0, #0
.L_08192604:
	cmp r3, #0
	bge .L_08192614
	movs r3, #0
	b .L_08192614
.L_0819260c:
	.4byte 0x0000001f
.L_08192610:
	.4byte 0x00000045
.L_08192614:
	cmp r1, #0
	bge .L_0819261a
	movs r1, #0
.L_0819261a:
	lsls r3, r3, #10
	lsls r2, r0, #5
	orrs r3, r2
	orrs r3, r1
	adds r7, #1
	strh r3, [r4]
	adds r4, #2
	cmp r7, #128
	bne .L_081925e8
	mov r2, r9
	movs r3, #1
	str r3, [r2, #16]
	ldr r2, .L_08192644
	movs r3, #0
	strh r3, [r2, #4]
	add sp, #28
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_08192644:
	.4byte Data_03001120
