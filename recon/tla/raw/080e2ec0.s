.syntax unified
	.thumb
	.global Func_080e2ec0
	.thumb_func
Func_080e2ec0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #224
	ldr r6, [r2, #92]
	ldr r1, [r3]
	movs r0, #216
	movs r3, #216
	lsls r0, r0, #5
	lsls r3, r3, #5
	adds r0, #18
	adds r3, #16
	adds r0, r0, r6
	adds r3, r3, r6
	mov r10, r0
	movs r2, #0
	ldrsh r7, [r0, r2]
	mov r8, r3
	movs r0, #0
	ldrsh r3, [r3, r0]
	sub sp, #16
	ldr r4, [r1, #16]
	cmp r3, #1
	beq .L_080e2fd6
	cmp r3, #1
	bgt .L_080e2f02
	cmp r3, #0
	beq .L_080e2f0a
	b .L_080e302e
.L_080e2f02:
	cmp r3, #2
	bne .L_080e2f08
	b .L_080e3014
.L_080e2f08:
	b .L_080e302e
.L_080e2f0a:
	lsls r0, r7, #8
	str r4, [sp, #0]
	bl Trig_Sin
	movs r2, #216
	lsls r2, r2, #5
	adds r2, #24
	adds r2, r2, r6
	lsls r0, r0, #5
	movs r3, #216
	str r0, [r2]
	lsls r3, r3, #5
	adds r3, #20
	adds r1, r6, r3
	ldr r3, [r1]
	movs r0, #160
	lsls r0, r0, #3
	adds r3, r3, r0
	str r3, [r1]
	movs r0, #216
	movs r3, #216
	lsls r3, r3, #5
	lsls r0, r0, #5
	adds r3, #28
	adds r0, #40
	mov r12, r2
	adds r2, r6, r3
	adds r3, r6, r0
	ldr r3, [r3]
	ldr r2, [r2]
	add r5, sp, #4
	subs r3, r3, r2
	muls r3, r7
	ldr r4, [sp, #0]
	cmp r3, #0
	bge .L_080e2f54
	adds r3, #127
.L_080e2f54:
	asrs r3, r3, #7
	adds r3, r2, r3
	movs r0, #216
	str r3, [r5]
	lsls r0, r0, #5
	movs r3, #217
	lsls r3, r3, #5
	adds r0, #44
	adds r2, r6, r3
	adds r3, r6, r0
	ldr r3, [r3]
	ldr r2, [r2]
	subs r3, r3, r2
	muls r3, r7
	cmp r3, #0
	bge .L_080e2f76
	adds r3, #127
.L_080e2f76:
	asrs r3, r3, #7
	adds r3, r2, r3
	str r3, [r5, #4]
	movs r0, #216
	movs r3, #216
	lsls r3, r3, #5
	lsls r0, r0, #5
	adds r3, #36
	adds r0, #48
	adds r2, r6, r3
	adds r3, r6, r0
	ldr r3, [r3]
	ldr r2, [r2]
	subs r3, r3, r2
	muls r3, r7
	cmp r3, #0
	bge .L_080e2f9a
	adds r3, #127
.L_080e2f9a:
	asrs r3, r3, #7
	adds r3, r2, r3
	str r3, [r5, #8]
	mov r2, r12
	ldr r0, [r2]
	ldr r1, [r1]
	adds r2, r5, #0
	str r4, [sp, #0]
	bl Vector_AddPolarOffset
	ldr r3, [r5]
	ldr r4, [sp, #0]
	str r3, [r4, #8]
	ldr r3, [r5, #4]
	str r3, [r4, #12]
	ldr r3, [r5, #8]
	str r3, [r4, #16]
	cmp r7, #127
	ble .L_080e302e
	mov r0, r8
	ldrh r3, [r0]
	mov r2, r8
	adds r3, #1
	strh r3, [r2]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	mov r0, r10
	strh r3, [r0]
	b .L_080e302e
.L_080e2fd6:
	ldr r3, [r4, #12]
	ldr r2, .L_080e305c
	adds r3, r3, r2
	str r3, [r4, #12]
	adds r3, r1, #0
	adds r3, #52
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #1
	bne .L_080e2ffc
	cmp r7, #40
	bne .L_080e302e
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	mov r0, r8
	strh r3, [r0]
	b .L_080e302e
.L_080e2ffc:
	cmp r7, #40
	bne .L_080e302e
	mov r2, r8
	ldrh r3, [r2]
	mov r0, r8
	adds r3, #1
	strh r3, [r0]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	mov r2, r10
	b .L_080e302c
.L_080e3014:
	ldr r3, [r4, #12]
	movs r0, #243
	lsls r0, r0, #10
	adds r0, #204
	adds r3, r3, r0
	str r3, [r4, #12]
	cmp r7, #40
	bne .L_080e302e
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	mov r2, r8
.L_080e302c:
	strh r3, [r2]
.L_080e302e:
	movs r3, #216
	lsls r3, r3, #5
	adds r3, #18
	adds r2, r6, r3
	ldrh r3, [r2]
	movs r0, #216
	adds r3, #1
	strh r3, [r2]
	lsls r0, r0, #5
	adds r0, #12
	adds r3, r6, r0
	ldr r2, [r3]
	ldr r3, [r4, #8]
	add sp, #16
	str r3, [r2, #8]
	ldr r3, [r4, #12]
	str r3, [r2, #12]
	ldr r3, [r4, #16]
	str r3, [r2, #16]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080e305c:
	.4byte 0xffff999a
