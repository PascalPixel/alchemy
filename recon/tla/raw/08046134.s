.syntax unified
	.thumb
	.global Func_08046134
	.thumb_func
Func_08046134:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r4, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	ldr r7, [sp, #20]
	mov lr, r3
	movs r3, #1
	ands r7, r3
	adds r6, r2, #0
	lsls r7, r7, #12
	cmp r0, #0
	bge .L_08046156
	adds r6, r6, r0
	movs r0, #0
.L_08046156:
	adds r3, r0, r6
	cmp r3, #29
	ble .L_08046160
	movs r3, #30
	subs r6, r3, r0
.L_08046160:
	cmp r1, #0
	bge .L_08046168
	adds r4, r4, r1
	movs r1, #0
.L_08046168:
	adds r3, r1, r4
	cmp r3, #29
	ble .L_08046172
	movs r3, #20
	subs r4, r3, r1
.L_08046172:
	cmp r6, #0
	ble .L_080461be
	cmp r4, #0
	ble .L_080461be
	lsls r3, r0, #1
	lsls r2, r1, #6
	add r3, lr
	movs r0, #2
	adds r2, r2, r3
	mov r8, r0
	mov r12, r2
.L_08046188:
	mov r0, r12
	adds r2, r6, #0
	adds r0, #8
	cmp r2, #0
	beq .L_080461a4
	ldr r5, .L_080461c4
.L_08046194:
	ldrh r3, [r0]
	subs r2, #1
	ands r3, r5
	orrs r3, r7
	strh r3, [r0]
	adds r0, #2
	cmp r2, #0
	bne .L_08046194
.L_080461a4:
	lsrs r3, r1, #2
	mov r0, lr
	mov r2, r8
	lsls r2, r3
	ldrb r3, [r0, #3]
	orrs r2, r3
	strb r2, [r0, #3]
	subs r4, #1
	movs r2, #64
	add r12, r2
	adds r1, #1
	cmp r4, #0
	bne .L_08046188
.L_080461be:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080461c4:
	.4byte 0xffffefff
