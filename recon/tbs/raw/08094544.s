.syntax unified
	.thumb
	.global DisplayScroll_BuildAndSwapHBlankPage
	.thumb_func
DisplayScroll_BuildAndSwapHBlankPage:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_08094710
	ldr r7, [r3]
	ldr r3, .L_08094714
	movs r4, #14
	ldrsh r2, [r3, r4]
	sub sp, #24
	movs r1, #12
	ldrsh r0, [r3, r1]
	str r2, [sp, #20]
	movs r2, #10
	ldrsh r1, [r3, r2]
	mov r12, r0
	movs r5, #8
	ldrsh r0, [r3, r5]
	str r1, [sp, #16]
	movs r2, #6
	ldrsh r5, [r3, r2]
	movs r4, #4
	ldrsh r1, [r3, r4]
	movs r4, #240
	str r5, [sp, #12]
	lsls r4, r4, #4
	adds r3, r7, r4
	ldrb r3, [r3]
	movs r2, #1
	eors r2, r3
	lsls r3, r2, #4
	subs r3, r3, r2
	movs r5, #241
	lsls r3, r3, #7
	lsls r5, r5, #4
	adds r4, r7, r3
	adds r3, r7, r5
	ldr r3, [r3]
	ldr r2, .L_08094718
	mov lr, r3
	adds r3, r7, r2
	ldrh r2, [r3]
	ldr r3, [sp, #20]
	lsls r3, r3, #16
	str r3, [sp, #8]
	subs r5, #8
	lsrs r3, r3, #16
	adds r2, r2, r3
	adds r3, r7, r5
	ldr r3, [r3]
	adds r6, r3, #0
	muls r6, r2
	mov r2, lr
	cmp r2, #0
	bne .L_080945ce
	movs r5, #0
	adds r3, r4, #0
.L_080945bc:
	mov r4, r12
	adds r5, #1
	strh r4, [r3]
	strh r0, [r3, #4]
	strh r1, [r3, #8]
	adds r3, #12
	cmp r5, #160
	bne .L_080945bc
	b .L_0809462c
.L_080945ce:
	ldr r5, .L_0809471c
	adds r3, r7, r5
	ldr r3, [r3]
	mov r2, r12
	mov r8, r3
	lsls r3, r2, #16
	lsls r1, r1, #16
	lsls r2, r0, #16
	lsrs r3, r3, #16
	ldr r0, .L_08094720
	lsrs r2, r2, #16
	lsrs r1, r1, #16
	str r3, [sp, #4]
	movs r5, #0
	mov r10, r0
	mov r11, r2
	mov r9, r1
.L_080945f0:
	movs r2, #255
	asrs r3, r6, #16
	ands r3, r2
	ldr r1, .L_08094724
	lsls r3, r3, #1
	ldrsh r0, [r1, r3]
	mov r1, r8
	movs r0, r0
	mov r12, pc
	bx r10
	cmp r0, #0
	bge .L_0809460a
	adds r0, #255
.L_0809460a:
	lsls r3, r0, #8
	ldr r0, [sp, #4]
	lsrs r3, r3, #16
	adds r2, r0, r3
	mov r1, r11
	strh r2, [r4]
	adds r4, #4
	adds r2, r1, r3
	strh r2, [r4]
	add r3, r9
	adds r4, #4
	adds r5, #1
	strh r3, [r4]
	add r6, lr
	adds r4, #4
	cmp r5, #160
	bne .L_080945f0
.L_0809462c:
	movs r2, #240
	lsls r2, r2, #4
	adds r3, r7, r2
	ldrb r3, [r3]
	movs r2, #1
	eors r2, r3
	lsls r3, r2, #4
	subs r3, r3, r2
	ldr r5, .L_08094728
	lsls r3, r3, #7
	adds r3, r7, r3
	adds r4, r3, #2
	adds r3, r7, r5
	ldr r3, [r3]
	ldr r0, .L_08094718
	mov lr, r3
	ldr r1, [sp, #8]
	adds r3, r7, r0
	ldrh r2, [r3]
	subs r5, #8
	lsrs r3, r1, #16
	adds r2, r2, r3
	adds r3, r7, r5
	ldr r3, [r3]
	mov r0, lr
	adds r6, r3, #0
	muls r6, r2
	cmp r0, #0
	bne .L_08094686
	movs r5, #0
	adds r3, r4, #0
.L_0809466a:
	add r1, sp, #20
	add r2, sp, #16
	add r4, sp, #12
	ldrh r1, [r1]
	ldrh r2, [r2]
	ldrh r4, [r4]
	adds r5, #1
	strh r1, [r3]
	strh r2, [r3, #4]
	strh r4, [r3, #8]
	adds r3, #12
	cmp r5, #160
	bne .L_0809466a
	b .L_080946e4
.L_08094686:
	ldr r5, .L_0809472c
	ldr r1, [sp, #12]
	adds r3, r7, r5
	ldr r3, [r3]
	ldr r0, [sp, #16]
	lsls r2, r1, #16
	ldr r1, [sp, #8]
	mov r8, r3
	lsrs r1, r1, #16
	lsls r3, r0, #16
	ldr r0, .L_08094720
	lsrs r3, r3, #16
	lsrs r2, r2, #16
	str r1, [sp, #0]
	movs r5, #0
	mov r10, r0
	mov r11, r3
	mov r9, r2
.L_080946aa:
	asrs r3, r6, #16
	movs r2, #255
	ands r3, r2
	ldr r2, .L_08094724
	lsls r3, r3, #1
	ldrsh r0, [r2, r3]
	mov r1, r8
	mov r12, pc
	bx r10
	cmp r0, #0
	bge .L_080946c2
	adds r0, #255
.L_080946c2:
	lsls r3, r0, #8
	ldr r0, [sp, #0]
	lsrs r3, r3, #16
	adds r2, r0, r3
	mov r1, r11
	strh r2, [r4]
	adds r4, #4
	adds r2, r1, r3
	strh r2, [r4]
	add r3, r9
	adds r4, #4
	adds r5, #1
	strh r3, [r4]
	add r6, lr
	adds r4, #4
	cmp r5, #160
	bne .L_080946aa
.L_080946e4:
	ldr r3, .L_08094718
	adds r2, r7, r3
	ldrh r3, [r2]
	movs r4, #240
	adds r3, #1
	strh r3, [r2]
	lsls r4, r4, #4
	adds r1, r7, r4
	ldrb r3, [r1]
	movs r2, #1
	eors r3, r2
	strb r3, [r1]
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_08094710:
	.4byte gHBlankScrollWork
.L_08094714:
	.4byte gBgScroll
.L_08094718:
	.4byte 0x00000f02
.L_0809471c:
	.4byte 0x00000f18
.L_08094720:
	.4byte IwramMulQ16ReturnIp
.L_08094724:
	.4byte Data_0809ed84
.L_08094728:
	.4byte 0x00000f14
.L_0809472c:
	.4byte 0x00000f1c
