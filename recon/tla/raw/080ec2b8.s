.syntax unified
	.thumb
	.global Func_080ec2b8
	.thumb_func
Func_080ec2b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #24
	str r1, [sp, #16]
	str r2, [sp, #12]
	str r0, [sp, #20]
	bl Func_080cdf5c
	bl Func_080ed804
	mov r11, r0
	movs r0, #128
	lsls r0, r0, #19
	ldr r5, .L_080ec478
	str r0, [sp, #8]
	movs r4, #1
	movs r2, #6
	ldrsh r1, [r5, r2]
	movs r0, #10
	ldrsh r3, [r5, r0]
	mov r9, r1
	movs r1, #192
	lsls r1, r1, #3
	adds r1, #68
	adds r5, r5, r1
	ldr r5, [r5]
	negs r4, r4
	mov r10, r3
	cmp r5, #0
	beq .L_080ec30a
	ldr r3, .L_080ec47c
	asrs r5, r5, #16
	movs r0, #2
	ldrsh r2, [r3, r0]
	mov r9, r5
	mov r10, r2
.L_080ec30a:
	mov r7, r11
	ldrb r3, [r7]
	movs r1, #0
	mov r8, r1
	cmp r3, #0
	beq .L_080ec3aa
	adds r5, r7, #0
.L_080ec318:
	ldrb r3, [r7]
	movs r2, #192
	lsls r2, r2, #1
	adds r0, r7, #0
	str r4, [sp, #0]
	adds r6, r3, r2
	bl Func_080ec298
	ldr r4, [sp, #0]
	cmp r0, #0
	beq .L_080ec396
	cmp r0, #15
	beq .L_080ec396
	adds r0, r6, #0
	bl GameFlag_Test
	ldr r4, [sp, #0]
	cmp r0, #0
	beq .L_080ec396
	movs r3, #4
	ldrsh r2, [r7, r3]
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #85
	muls r2, r3
	cmp r2, #0
	bge .L_080ec356
	movs r0, #252
	lsls r0, r0, #6
	adds r0, #255
	adds r2, r2, r0
.L_080ec356:
	asrs r3, r2, #14
	ldr r2, .L_080ec480
	adds r1, r3, r2
	movs r3, #6
	ldrsh r2, [r7, r3]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r0, r3, #7
	cmp r0, #0
	bge .L_080ec372
	movs r2, #252
	lsls r2, r2, #6
	adds r2, #255
	adds r0, r0, r2
.L_080ec372:
	asrs r3, r0, #14
	mov r0, r9
	subs r2, r1, r0
	subs r3, #112
	mov r1, r10
	subs r3, r3, r1
	adds r0, r2, #0
	muls r0, r2
	adds r1, r3, #0
	muls r1, r3
	adds r2, r0, #0
	adds r3, r1, #0
	adds r0, r2, r3
	ldr r2, [sp, #8]
	cmp r2, r0
	ble .L_080ec396
	str r0, [sp, #8]
	mov r4, r8
.L_080ec396:
	movs r3, #1
	add r8, r3
	mov r0, r8
	adds r5, #20
	cmp r0, #127
	bgt .L_080ec3aa
	adds r7, r5, #0
	ldrb r3, [r7]
	cmp r3, #0
	bne .L_080ec318
.L_080ec3aa:
	movs r1, #1
	negs r1, r1
	cmp r4, r1
	bne .L_080ec3c0
	ldr r0, [sp, #20]
	ldr r1, [sp, #16]
	bl Func_080ec1d0
	b .L_080ec468
.L_080ec3bc:
	adds r7, r5, #0
	b .L_080ec428
.L_080ec3c0:
	lsls r3, r4, #2
	adds r3, r3, r4
	lsls r3, r3, #2
	mov r2, r11
	adds r7, r2, r3
	ldr r3, [sp, #8]
	cmp r3, #4
	bgt .L_080ec428
	movs r2, #0
.L_080ec3d2:
	ldr r0, [sp, #12]
	adds r4, r4, r0
	cmp r4, #0
	bge .L_080ec3e2
	mov r4, r8
	subs r4, #1
	adds r2, #1
	b .L_080ec424
.L_080ec3e2:
	lsls r3, r4, #2
	adds r3, r3, r4
	lsls r3, r3, #2
	mov r1, r11
	adds r5, r1, r3
	ldrb r3, [r5]
	cmp r3, #0
	bne .L_080ec3f8
	movs r4, #0
	adds r2, #1
	b .L_080ec424
.L_080ec3f8:
	ldrb r3, [r5]
	movs r0, #192
	lsls r0, r0, #1
	adds r6, r3, r0
	adds r0, r5, #0
	str r2, [sp, #4]
	str r4, [sp, #0]
	bl Func_080ec298
	ldr r2, [sp, #4]
	ldr r4, [sp, #0]
	cmp r0, #0
	beq .L_080ec424
	cmp r0, #15
	beq .L_080ec424
	adds r0, r6, #0
	bl GameFlag_Test
	ldr r2, [sp, #4]
	ldr r4, [sp, #0]
	cmp r0, #0
	bne .L_080ec3bc
.L_080ec424:
	cmp r2, #1
	ble .L_080ec3d2
.L_080ec428:
	movs r1, #4
	ldrsh r2, [r7, r1]
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #85
	muls r2, r3
	cmp r2, #0
	bge .L_080ec440
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	adds r2, r2, r3
.L_080ec440:
	ldr r0, .L_080ec480
	ldr r1, [sp, #20]
	asrs r3, r2, #14
	adds r3, r3, r0
	str r3, [r1]
	movs r3, #6
	ldrsh r2, [r7, r3]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r0, r3, #7
	cmp r0, #0
	bge .L_080ec460
	movs r1, #252
	lsls r1, r1, #6
	adds r1, #255
	adds r0, r0, r1
.L_080ec460:
	ldr r2, [sp, #16]
	asrs r3, r0, #14
	subs r3, #112
	str r3, [r2]
.L_080ec468:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ec478:
	.4byte Data_0202a000
.L_080ec47c:
	.4byte Data_0202a648
.L_080ec480:
	.4byte 0xfffffef3
