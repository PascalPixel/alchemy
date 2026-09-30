.syntax unified
	.thumb
	.global Func_080450fc
	.thumb_func
Func_080450fc:
	push {r5, lr}
	ldr r3, .L_08045188
	ldr r1, .L_0804518c
	ldr r3, [r3]
	movs r2, #7
	lsrs r3, r3, #1
	ands r3, r2
	lsls r3, r3, #2
	ldr r1, [r1, r3]
	sub sp, #8
	adds r5, r0, #0
	cmp r1, #0
	bge .L_08045118
	adds r1, #255
.L_08045118:
	asrs r1, r1, #8
	cmp r5, #0
	beq .L_08045198
	ldr r3, [sp, #0]
	ldr r4, .L_08045190
	lsls r1, r1, #16
	movs r2, #255
	lsrs r1, r1, #16
	ands r3, r4
	lsls r2, r2, #8
	adds r2, #255
	orrs r3, r1
	ands r3, r2
	lsls r1, r1, #16
	orrs r3, r1
	str r3, [sp, #0]
	mov r0, sp
	ldr r3, [r0, #4]
	ands r3, r4
	str r3, [r0, #4]
	bl Func_0801401c
	ldrb r2, [r5, #23]
	movs r3, #31
	ands r0, r3
	movs r3, #63
	negs r3, r3
	ands r3, r2
	lsls r0, r0, #1
	orrs r3, r0
	strb r3, [r5, #23]
	ldrb r3, [r5, #21]
	movs r2, #3
	orrs r3, r2
	strb r3, [r5, #21]
	ldrh r2, [r5, #6]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #240
	adds r2, r2, r3
	ldr r3, .L_08045184
	ldrh r1, [r5, #22]
	ands r2, r3
	ldr r3, .L_08045194
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #22]
	ldrb r3, [r5, #8]
	adds r3, #240
	strb r3, [r5, #20]
	movs r3, #252
	strb r3, [r5, #15]
	b .L_08045198
	.2byte 0x0000
.L_08045184:
	.4byte 0x000001ff
.L_08045188:
	.4byte gFrameTick
.L_0804518c:
	.4byte Data_0805f6e0
.L_08045190:
	.4byte 0xffff0000
.L_08045194:
	.4byte 0xfffffe00
.L_08045198:
	add sp, #8
	pop {r5, pc}
