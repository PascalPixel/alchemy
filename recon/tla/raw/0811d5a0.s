.syntax unified
	.thumb
	.global Func_0811d5a0
	.thumb_func
Func_0811d5a0:
	push {r5, r6, r7, lr}
	movs r1, #0
	adds r5, r0, #0
	movs r3, #15
	mov lr, r1
	movs r7, #31
.L_0811d5ac:
	lsls r3, r3, #4
	movs r6, #0
	mov r12, r3
.L_0811d5b2:
	mov r2, r12
	adds r3, r2, r6
	movs r1, #160
	lsls r1, r1, #19
	lsls r0, r3, #1
	adds r3, r0, r1
	ldrh r3, [r3]
	adds r1, r7, #0
	lsrs r4, r3, #10
	ands r4, r7
	lsrs r2, r3, #5
	ands r2, r7
	ands r1, r3
	adds r4, r4, r5
	adds r2, r2, r5
	adds r1, r1, r5
	cmp r4, #31
	ble .L_0811d5d8
	movs r4, #31
.L_0811d5d8:
	cmp r2, #31
	ble .L_0811d5de
	movs r2, #31
.L_0811d5de:
	cmp r1, #31
	ble .L_0811d5e4
	movs r1, #31
.L_0811d5e4:
	cmp r4, #0
	bge .L_0811d5ea
	movs r4, #0
.L_0811d5ea:
	cmp r2, #0
	bge .L_0811d5f0
	movs r2, #0
.L_0811d5f0:
	cmp r1, #0
	bge .L_0811d5f6
	movs r1, #0
.L_0811d5f6:
	lsls r2, r2, #5
	lsls r3, r4, #10
	orrs r3, r2
	orrs r3, r1
	ldr r1, .L_0811d618
	adds r6, #1
	adds r2, r0, r1
	strh r3, [r2]
	cmp r6, #15
	ble .L_0811d5b2
	movs r2, #1
	add lr, r2
	mov r1, lr
	movs r3, #5
	cmp r1, #1
	ble .L_0811d5ac
	pop {r5, r6, r7, pc}
.L_0811d618:
	.4byte 0x04ffffe0
