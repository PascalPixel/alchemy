.syntax unified
	.thumb
	.global Func_080eaf98
	.thumb_func
Func_080eaf98:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	mov r8, r1
	mov r10, r2
	adds r6, r3, #0
	ldr r7, [sp, #24]
	cmp r5, #0
	beq .L_080eb002
	ldr r3, .L_080eb010
	movs r1, #40
	movs r2, #0
	mov lr, r3
	.2byte 0xf800
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #20]
	str r3, [r5, #24]
	movs r3, #255
	strh r3, [r5, #30]
	mov r3, r8
	strh r3, [r5, #36]
	mov r3, r10
	strh r3, [r5, #38]
	lsrs r2, r6, #12
	lsrs r3, r6, #30
	orrs r3, r2
	ldr r1, .L_080eb014
	movs r2, #15
	ands r3, r2
	lsls r3, r3, #1
	ldrb r2, [r1, r3]
	adds r3, #1
	ldrb r3, [r1, r3]
	strh r2, [r5, #32]
	strh r3, [r5, #34]
	adds r3, r5, #4
	stmia r3!, {r6}
	movs r0, #0
	str r0, [r3]
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	strb r3, [r5, #5]
	ldr r3, .L_080eb00c
	ldrh r2, [r5, #8]
	ands r7, r3
	ldr r3, .L_080eb018
	ands r3, r2
	orrs r3, r7
	strh r3, [r5, #8]
.L_080eb002:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080eb00c:
	.4byte 0x000003ff
.L_080eb010:
	.4byte IwramFillWords
.L_080eb014:
	.4byte Data_080f3bc4
.L_080eb018:
	.4byte 0xfffffc00
