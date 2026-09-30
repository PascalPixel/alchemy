.syntax unified
	.thumb
	.global Func_080e2264
	.thumb_func
Func_080e2264:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r2, r3, #0
	adds r2, #224
	ldr r2, [r2]
	ldr r1, [r3, #108]
	mov r11, r2
	ldr r3, [r3, #92]
	movs r2, #204
	mov r8, r1
	lsls r2, r2, #4
	sub sp, #44
	add r2, r8
	str r3, [sp, #28]
	str r2, [sp, #24]
	movs r1, #0
	mov r3, r11
	ldr r3, [r3, #16]
	ldr r2, [sp, #28]
	str r1, [sp, #16]
	movs r1, #216
	lsls r1, r1, #5
	adds r1, #52
	mov r9, r3
	adds r3, r2, r1
	add r2, sp, #16
	ldrb r2, [r2]
	mov r1, r9
	strb r2, [r3]
	movs r2, #208
	lsls r2, r2, #4
	movs r3, #255
	adds r2, #60
	lsls r3, r3, #8
	add r2, r8
	adds r3, #255
	strh r3, [r2]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #62
	movs r3, #1
	negs r3, r3
	add r2, r8
	strh r3, [r2]
	movs r2, #212
	lsls r2, r2, #4
	add r2, r8
	strh r3, [r2]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #66
	add r2, r8
	strh r3, [r2]
	movs r3, #0
	str r3, [sp, #20]
	movs r3, #208
	lsls r3, r3, #4
	adds r1, #34
	adds r3, #60
	str r1, [sp, #8]
	add r3, r8
	movs r1, #0
	movs r2, #32
	str r3, [sp, #12]
	ldr r7, [sp, #24]
	str r1, [sp, #4]
	add r2, sp
	mov r10, r2
.L_080e22fa:
	ldr r2, [sp, #4]
	ldr r1, .L_080e2494
	ldrsb r3, [r2, r1]
	mov r1, r9
	ldr r2, [r1, #8]
	lsls r3, r3, #20
	adds r5, r2, r3
	ldr r3, [sp, #4]
	ldr r2, .L_080e2494
	adds r3, #1
	ldrsb r3, [r3, r2]
	ldr r2, [r1, #16]
	lsls r3, r3, #20
	adds r6, r2, r3
	ldr r3, [sp, #8]
	adds r1, r6, #0
	ldrb r2, [r3]
	adds r0, r5, #0
	bl Func_080dbda8
	mov r1, r10
	adds r4, r0, #0
	str r5, [r1]
	str r6, [r1, #8]
	cmp r4, #1
	bne .L_080e237a
	mov r3, r11
	movs r2, #30
	ldrsh r0, [r3, r2]
	mov r1, r9
	mov r2, r10
	str r4, [sp, #0]
	bl Func_080cda84
	movs r1, #1
	negs r1, r1
	ldr r4, [sp, #0]
	cmp r0, r1
	beq .L_080e237a
	adds r3, r5, #0
	cmp r5, #0
	bge .L_080e2352
	ldr r2, .L_080e2498
	adds r3, r5, r2
.L_080e2352:
	asrs r3, r3, #20
	strh r3, [r7]
	adds r2, r7, #2
	adds r3, r6, #0
	cmp r6, #0
	bge .L_080e2362
	ldr r1, .L_080e2498
	adds r3, r6, r1
.L_080e2362:
	asrs r3, r3, #20
	strh r3, [r2]
	ldr r2, [sp, #16]
	ldr r1, [sp, #28]
	adds r2, #1
	str r2, [sp, #16]
	movs r2, #216
	lsls r2, r2, #5
	adds r2, #52
	adds r3, r1, r2
	strb r4, [r3]
	adds r7, #4
.L_080e237a:
	cmp r4, #2
	bne .L_080e23f0
	mov r1, r11
	movs r3, #30
	ldrsh r0, [r1, r3]
	mov r2, r10
	mov r1, r9
	str r4, [sp, #0]
	bl Func_080cda84
	movs r2, #1
	negs r2, r2
	ldr r4, [sp, #0]
	cmp r0, r2
	beq .L_080e23f0
	ldr r1, [sp, #12]
	asrs r3, r5, #16
	strh r3, [r1]
	movs r1, #208
	lsls r1, r1, #4
	adds r1, #62
	add r1, r8
	asrs r3, r6, #16
	strh r3, [r1]
	ldr r3, [sp, #12]
	ldrh r2, [r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #240
	ands r3, r2
	ldr r2, [sp, #12]
	adds r3, #8
	strh r3, [r2]
	movs r3, #255
	ldrh r2, [r1]
	lsls r3, r3, #8
	adds r3, #240
	ands r3, r2
	adds r3, #8
	strh r3, [r1]
	adds r3, r5, #0
	cmp r5, #0
	bge .L_080e23d4
	ldr r1, .L_080e2498
	adds r3, r5, r1
.L_080e23d4:
	asrs r3, r3, #20
	strh r3, [r7]
	adds r2, r7, #2
	adds r3, r6, #0
	cmp r6, #0
	bge .L_080e23e4
	ldr r1, .L_080e2498
	adds r3, r6, r1
.L_080e23e4:
	asrs r3, r3, #20
	strh r3, [r2]
	ldr r2, [sp, #16]
	adds r7, #4
	adds r2, #1
	str r2, [sp, #16]
.L_080e23f0:
	cmp r4, #3
	bne .L_080e2462
	mov r1, r11
	movs r3, #30
	ldrsh r0, [r1, r3]
	mov r2, r10
	mov r1, r9
	bl Func_080cda84
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	beq .L_080e2462
	movs r1, #212
	movs r0, #208
	lsls r1, r1, #4
	lsls r0, r0, #4
	add r1, r8
	asrs r3, r5, #16
	adds r0, #66
	strh r3, [r1]
	add r0, r8
	asrs r3, r6, #16
	strh r3, [r0]
	movs r3, #255
	ldrh r2, [r1]
	lsls r3, r3, #8
	adds r3, #240
	ands r3, r2
	adds r3, #8
	strh r3, [r1]
	movs r3, #255
	ldrh r2, [r0]
	lsls r3, r3, #8
	adds r3, #240
	ands r3, r2
	adds r3, #8
	strh r3, [r0]
	adds r0, r5, #0
	cmp r0, #0
	bge .L_080e2446
	ldr r3, .L_080e2498
	adds r0, r0, r3
.L_080e2446:
	asrs r3, r0, #20
	adds r1, r6, #0
	strh r3, [r7]
	adds r2, r7, #2
	cmp r1, #0
	bge .L_080e2456
	ldr r3, .L_080e2498
	adds r1, r1, r3
.L_080e2456:
	asrs r3, r1, #20
	strh r3, [r2]
	ldr r1, [sp, #16]
	adds r7, #4
	adds r1, #1
	str r1, [sp, #16]
.L_080e2462:
	ldr r2, [sp, #4]
	ldr r3, [sp, #20]
	adds r2, #2
	adds r3, #1
	str r2, [sp, #4]
	str r3, [sp, #20]
	cmp r3, #20
	bhi .L_080e2474
	b .L_080e22fa
.L_080e2474:
	ldr r1, [sp, #16]
	ldr r2, [sp, #24]
	lsls r3, r1, #2
	adds r3, r3, r2
	ldr r2, .L_080e2490
	add sp, #44
	strh r2, [r3]
	strh r2, [r3, #2]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080e2490:
	.4byte 0xffffffff
.L_080e2494:
	.4byte Data_080f0f64
.L_080e2498:
	.4byte 0x000fffff
