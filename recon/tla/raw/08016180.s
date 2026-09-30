.syntax unified
	.thumb
	.global Func_08016180
	.thumb_func
Func_08016180:
	push {r5, r6, r7, lr}
	ldr r6, .L_080161cc
	sub sp, #4
	ldrh r3, [r6]
	adds r7, r3, #0
	strh r6, [r6]
	ldr r5, .L_080161d0
	movs r0, #7
	movs r1, #0
	adds r2, r5, #0
	bl Func_08013438
	movs r0, #6
	movs r1, #0
	adds r2, r5, #0
	bl Func_08013438
	ldr r4, .L_080161c8
	adds r3, r6, #0
	strh r4, [r3]
	ldr r1, .L_080161d4
	movs r3, #255
	ldrh r2, [r1]
	lsls r3, r3, #8
	adds r3, #63
	ands r3, r2
	strh r3, [r1]
	adds r1, #2
	ldrh r2, [r1]
	movs r0, #128
	adds r3, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_080161d8
	strh r0, [r1]
	b .L_080161d8
.L_080161c8:
	.4byte 0x00000000
.L_080161cc:
	.4byte 0x04000208
.L_080161d0:
	.4byte Func_08016694
.L_080161d4:
	.4byte 0x04000200
.L_080161d8:
	ldrh r2, [r1]
	movs r0, #64
	adds r3, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_080161e6
	strh r0, [r1]
.L_080161e6:
	ldr r2, .L_0801622c
	ldr r3, .L_08016220
	ldr r1, .L_08016230
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #5
	strh r4, [r2]
	str r3, [r1]
	movs r3, #128
	lsls r3, r3, #6
	strh r4, [r2]
	str r3, [r1]
	ldr r2, .L_08016224
	ldrh r3, [r1]
	mov r0, sp
	orrs r3, r2
	strh r3, [r1]
	ldr r3, .L_08016228
	ldr r2, .L_08016234
	strh r3, [r6]
	movs r3, #0
	mov r12, r2
	str r3, [r0]
	movs r2, #133
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	b .L_08016238
.L_08016220:
	.4byte 0x00008000
.L_08016224:
	.4byte 0x00004003
.L_08016228:
	.4byte 0x00000001
.L_0801622c:
	.4byte 0x04000134
.L_08016230:
	.4byte 0x04000128
.L_08016234:
	.4byte gSerialRuntime
.L_08016238:
	mov r1, r12
	adds r2, #88
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #1
	mov r2, r12
	negs r3, r3
	str r3, [r2, #20]
	mov r3, r12
	adds r3, #96
	str r3, [r2, #40]
	mov r4, r12
	adds r3, #32
	str r3, [r2, #44]
	adds r4, #224
	adds r3, #64
	adds r2, #48
	adds r1, #160
	movs r0, #1
.L_0801625e:
	subs r0, #1
	str r1, [r2]
	str r3, [r2, #16]
	str r4, [r2, #32]
	adds r3, #96
	adds r4, #96
	adds r2, #4
	adds r1, #96
	cmp r0, #0
	bge .L_0801625e
	ldr r5, .L_080162b8
	movs r0, #0
	strh r0, [r5]
	ldr r1, .L_080162bc
	ldr r2, .L_080162ac
	ldrh r3, [r1]
	ldr r4, .L_080162b0
	orrs r3, r2
	strh r3, [r1]
	ldr r2, .L_080162b4
	ldr r3, .L_080162c0
	strh r2, [r5]
	strh r2, [r3]
	ldr r3, .L_080162c4
	strb r4, [r3]
	ldr r3, .L_080162c8
	str r0, [r3]
	ldr r3, .L_080162cc
	strh r0, [r3]
	ldr r3, .L_080162d0
	str r0, [r3]
	ldr r3, .L_080162d4
	strh r0, [r3]
	bl Func_08016950
	strh r7, [r5]
	add sp, #4
	b .L_080162d8
	.2byte 0x0000
.L_080162ac:
	.4byte 0x00000080
.L_080162b0:
	.4byte 0x00000000
.L_080162b4:
	.4byte 0x00000001
.L_080162b8:
	.4byte 0x04000208
.L_080162bc:
	.4byte 0x04000200
.L_080162c0:
	.4byte Data_030011b8
.L_080162c4:
	.4byte Data_020054c0
.L_080162c8:
	.4byte Data_020038d0
.L_080162cc:
	.4byte Data_020036d4
.L_080162d0:
	.4byte Data_020055d0
.L_080162d4:
	.4byte Data_02005354
.L_080162d8:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
