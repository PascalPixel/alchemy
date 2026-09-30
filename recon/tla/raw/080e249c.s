.syntax unified
	.thumb
	.global Func_080e249c
	.thumb_func
Func_080e249c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #224
	ldr r3, [r3]
	movs r1, #204
	mov r10, r3
	ldr r3, [r2, #108]
	lsls r1, r1, #4
	adds r0, r3, r1
	movs r4, #0
	adds r3, r0, #0
	mov r2, r10
	mov r8, r4
	movs r4, #0
	ldrsh r3, [r3, r4]
	ldr r1, [r2, #16]
	movs r2, #1
	negs r2, r2
	sub sp, #56
	cmp r3, r2
	beq .L_080e2536
	adds r1, #34
	add r7, sp, #8
	mov r9, r1
	mov r11, r2
	adds r6, r0, #0
	adds r4, r7, #0
.L_080e24e2:
	movs r1, #0
	ldrsh r0, [r6, r1]
	movs r2, #2
	ldrsh r1, [r6, r2]
	mov r3, r9
	lsls r0, r0, #20
	lsls r1, r1, #20
	ldrb r2, [r3]
	str r4, [sp, #4]
	bl Func_080dbdc8
	movs r5, #0
	lsls r0, r0, #16
	asrs r0, r0, #16
	ldr r4, [sp, #4]
	cmp r5, r8
	bge .L_080e251e
	movs r1, #0
	ldrsh r3, [r7, r1]
	cmp r3, r0
	beq .L_080e251e
	adds r2, r7, #0
.L_080e250e:
	adds r5, #1
	cmp r5, r8
	bge .L_080e251e
	adds r2, #2
	movs r1, #0
	ldrsh r3, [r2, r1]
	cmp r3, r0
	bne .L_080e250e
.L_080e251e:
	cmp r5, r8
	bne .L_080e252a
	adds r5, #1
	strh r0, [r4]
	mov r8, r5
	adds r4, #2
.L_080e252a:
	adds r6, #4
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, r11
	bne .L_080e24e2
	b .L_080e2538
.L_080e2536:
	add r7, sp, #8
.L_080e2538:
	mov r3, r8
	lsls r2, r3, #1
	ldr r3, .L_080e2568
	mov r4, r8
	strh r3, [r7, r2]
	cmp r4, #1
	ble .L_080e259a
	mov r2, r8
	movs r3, #0
	subs r2, #1
	cmp r3, r2
	bge .L_080e259a
	mov r9, r2
.L_080e2552:
	adds r6, r3, #1
	cmp r6, r8
	bge .L_080e2594
	lsls r3, r3, #1
	mov r12, r3
	mov r0, r8
	adds r4, r7, #0
	mov lr, r12
	lsls r2, r6, #1
	subs r5, r0, r6
	b .L_080e256c
.L_080e2568:
	.4byte 0xffffffff
.L_080e256c:
	mov r3, lr
	ldrsh r1, [r4, r3]
	ldrh r3, [r2, r4]
	movs r0, #2
	add r0, sp
	strh r3, [r0]
	ldrsh r3, [r2, r4]
	mov r11, r3
	cmp r1, r11
	ble .L_080e258c
	movs r3, #2
	add r3, sp
	ldrh r0, [r3]
	mov r3, r12
	strh r0, [r7, r3]
	strh r1, [r2, r7]
.L_080e258c:
	subs r5, #1
	adds r2, #2
	cmp r5, #0
	bne .L_080e256c
.L_080e2594:
	adds r3, r6, #0
	cmp r3, r9
	blt .L_080e2552
.L_080e259a:
	movs r4, #0
	ldrsh r3, [r7, r4]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	beq .L_080e25d8
	mov r8, r2
	adds r6, r7, #0
	movs r5, #0
.L_080e25ac:
	ldrsh r2, [r5, r6]
	movs r0, #160
	mov r4, r10
	lsls r0, r0, #23
	movs r3, #30
	ldrsh r1, [r4, r3]
	adds r0, #5
	bl Func_080ce458
	cmp r0, #0
	beq .L_080e25d0
	mov r3, r10
	movs r2, #24
	ldrsh r1, [r3, r2]
	movs r4, #26
	ldrsh r2, [r3, r4]
	bl Func_080ceafc
.L_080e25d0:
	adds r5, #2
	ldrsh r3, [r5, r6]
	cmp r3, r8
	bne .L_080e25ac
.L_080e25d8:
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
