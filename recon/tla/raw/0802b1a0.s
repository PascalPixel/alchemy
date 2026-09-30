.syntax unified
	.thumb
	.global Func_0802b1a0
	.thumb_func
Func_0802b1a0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r4, r3, #0
	mov r11, r2
	lsls r1, r1, #7
	ldr r2, .L_0802b2bc
	lsls r3, r4, #7
	adds r1, r1, r0
	add r3, r11
	lsls r1, r1, #2
	lsls r3, r3, #2
	sub sp, #36
	adds r3, r3, r2
	adds r1, r1, r2
	str r3, [sp, #4]
	str r1, [sp, #8]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r0, #132
	lsls r0, r0, #1
	adds r2, r3, r0
	add r0, sp, #12
	mov r9, r0
	movs r6, #2
.L_0802b1dc:
	ldr r3, [r2]
	subs r6, #1
	asrs r3, r3, #20
	str r3, [r0]
	ldr r3, [r2, #4]
	adds r2, #56
	asrs r3, r3, #20
	str r3, [r0, #4]
	adds r0, #8
	cmp r6, #0
	bge .L_0802b1dc
	ldr r3, [sp, #72]
	adds r7, r4, #0
	adds r3, r7, r3
	cmp r7, r3
	bcs .L_0802b2ae
	ldr r1, [sp, #68]
	str r3, [sp, #0]
	movs r3, #128
	subs r3, r3, r1
	lsls r3, r3, #2
	mov r8, r3
.L_0802b208:
	ldr r2, [sp, #68]
	mov r1, r11
	adds r3, r1, r2
	cmp r1, r3
	bcs .L_0802b29a
	mov r12, r7
	mov r4, r12
	mov lr, r3
	movs r3, #15
	ands r4, r3
	mov r10, r3
	mov r12, r4
.L_0802b220:
	ldr r2, [sp, #8]
	ldr r4, [sp, #4]
	ldmia r2!, {r5}
	movs r3, #240
	adds r0, r2, #0
	str r0, [sp, #8]
	lsls r3, r3, #4
	adds r3, #255
	ands r5, r3
	ldr r2, .L_0802b2c0
	ldr r3, [r4]
	movs r6, #0
	ands r3, r2
	orrs r3, r5
	stmia r4!, {r3}
	adds r2, r1, #0
	adds r0, r4, #0
	mov r3, r10
	mov r4, r12
	ands r2, r3
	lsls r3, r4, #5
	adds r3, r3, r2
	str r0, [sp, #4]
	lsls r4, r3, #2
	mov r0, r9
.L_0802b252:
	ldr r3, [r0]
	cmp r3, r1
	bgt .L_0802b286
	adds r3, #16
	cmp r3, r1
	ble .L_0802b286
	ldr r3, [r0, #4]
	cmp r3, r7
	bgt .L_0802b286
	adds r3, #12
	cmp r3, r7
	ble .L_0802b286
	lsls r3, r5, #3
	ldr r2, .L_0802b2c4
	ldr r5, .L_0802b2c8
	adds r0, r4, r2
	adds r2, r3, r5
	ldr r2, [r2]
	str r2, [r0]
	ldr r0, .L_0802b2cc
	adds r2, r3, r0
	ldr r3, .L_0802b2d0
	adds r0, r4, r3
	ldr r3, [r2]
	str r3, [r0]
	b .L_0802b294
.L_0802b286:
	movs r2, #128
	lsls r2, r2, #4
	adds r6, #1
	adds r4, r4, r2
	adds r0, #8
	cmp r6, #2
	ble .L_0802b252
.L_0802b294:
	adds r1, #1
	cmp r1, lr
	bcc .L_0802b220
.L_0802b29a:
	ldr r3, [sp, #8]
	ldr r4, [sp, #4]
	ldr r5, [sp, #0]
	add r3, r8
	add r4, r8
	adds r7, #1
	str r3, [sp, #8]
	str r4, [sp, #4]
	cmp r7, r5
	bcc .L_0802b208
.L_0802b2ae:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0802b2bc:
	.4byte gMapCellBuffer
.L_0802b2c0:
	.4byte 0xfffff000
.L_0802b2c4:
	.4byte 0x06002800
.L_0802b2c8:
	.4byte gMapBlocks
.L_0802b2cc:
	.4byte Data_02020004
.L_0802b2d0:
	.4byte 0x06002840
