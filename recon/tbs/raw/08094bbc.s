.syntax unified
	.thumb
	.global Unnamed_08094bbc
	.thumb_func
Unnamed_08094bbc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_08094d14
	ldr r0, [r3]
	subs r3, #84
	ldr r3, [r3]
	sub sp, #16
	movs r1, #0
	mov r9, r0
	ldr r2, .L_08094d18
	mov r7, r9
	str r3, [sp, #12]
	str r1, [sp, #8]
	str r1, [sp, #4]
	adds r7, #8
	mov r11, r2
.L_08094be6:
	ldrh r3, [r7, #28]
	ldr r2, .L_08094d1c
	adds r1, r3, r2
	adds r3, r1, #0
	ands r3, r2
	strh r1, [r7, #28]
	cmp r3, r2
	bne .L_08094bf8
	b .L_08094d2c
.L_08094bf8:
	ldr r3, [sp, #12]
	adds r3, #228
	ldr r6, [r3]
	ldr r3, [r3, #4]
	movs r0, #179
	mov r8, r3
	lsls r3, r1, #16
	asrs r3, r3, #16
	lsls r0, r0, #1
	mov r10, r3
	bl Func_080770c0
	cmp r0, #0
	beq .L_08094c20
	ldrh r3, [r7, #28]
	adds r3, #1
	strh r3, [r7, #28]
	ldr r3, [r7, #24]
	subs r3, #1
	str r3, [r7, #24]
.L_08094c20:
	bl Random16
	adds r5, r0, #0
	bl Random16
	movs r3, #1
	ldr r1, [r7, #12]
	ands r0, r3
	ands r3, r5
	adds r3, r3, r0
	subs r1, r1, r6
	lsrs r3, r3, #1
	asrs r1, r1, #16
	adds r1, r1, r3
	ldr r2, [r7, #20]
	ldr r3, [r7, #16]
	mov r0, r10
	subs r2, r2, r3
	mov r3, r8
	subs r2, r2, r3
	lsls r3, r0, #16
	subs r4, r1, #1
	asrs r2, r2, #16
	lsrs r3, r3, #16
	adds r1, #15
	subs r0, r2, r3
	cmp r1, #255
	bhi .L_08094d28
	movs r1, #32
	negs r1, r1
	cmp r0, r1
	blt .L_08094d28
	cmp r0, #159
	bgt .L_08094d28
	ldrh r3, [r7, #28]
	cmp r3, #59
	bhi .L_08094c82
	mov r2, r9
	ldr r3, [r2, #4]
	ldr r1, .L_08094d20
	ldrh r2, [r7, #8]
	adds r3, #16
	ands r3, r1
	mov r1, r11
	ands r2, r1
	orrs r2, r3
	ldr r3, [r7, #24]
	adds r3, #3
	b .L_08094c9c
.L_08094c82:
	cmp r3, #89
	bhi .L_08094ca2
	mov r2, r9
	ldr r3, [r2, #4]
	ldr r1, .L_08094d20
	ldrh r2, [r7, #8]
	adds r3, #8
	ands r3, r1
	mov r1, r11
	ands r2, r1
	orrs r2, r3
	ldr r3, [r7, #24]
	adds r3, #1
.L_08094c9c:
	strh r2, [r7, #8]
	str r3, [r7, #24]
	b .L_08094cb4
.L_08094ca2:
	mov r3, r9
	ldr r2, [r3, #4]
	ldr r1, .L_08094d20
	ldrh r3, [r7, #8]
	ands r2, r1
	mov r1, r11
	ands r3, r1
	orrs r3, r2
	strh r3, [r7, #8]
.L_08094cb4:
	ldr r3, .L_08094d24
	ldr r3, [r3]
	movs r2, #1
	lsrs r3, r3, #3
	ands r3, r2
	cmp r3, #0
	beq .L_08094cd6
	ldrh r2, [r7, #8]
	lsls r3, r2, #22
	ldr r1, .L_08094d20
	lsrs r3, r3, #22
	adds r3, #4
	ands r3, r1
	mov r1, r11
	ands r2, r1
	orrs r2, r3
	strh r2, [r7, #8]
.L_08094cd6:
	ldr r3, .L_08094d0c
	ldr r2, .L_08094d10
	ands r4, r3
	ldrh r3, [r7, #6]
	ands r3, r2
	orrs r3, r4
	strh r3, [r7, #6]
	ldr r3, [r7, #24]
	asrs r3, r3, #2
	subs r3, r0, r3
	ldrb r1, [r7, #5]
	movs r2, #63
	strb r3, [r7, #4]
	adds r3, r2, #0
	ands r3, r1
	strb r3, [r7, #5]
	ldrb r3, [r7, #7]
	ands r2, r3
	movs r3, #64
	orrs r2, r3
	strb r2, [r7, #7]
	adds r0, r7, #0
	movs r1, #240
	bl Runtime_PushSlotEntry
	b .L_08094d2c
	.2byte 0x0000
.L_08094d0c:
	.4byte 0x000001ff
.L_08094d10:
	.4byte 0xfffffe00
.L_08094d14:
	.4byte gParticleWork
.L_08094d18:
	.4byte 0xfffffc00
.L_08094d1c:
	.4byte 0x0000ffff
.L_08094d20:
	.4byte 0x000003ff
.L_08094d24:
	.4byte gFrameCount
.L_08094d28:
	movs r3, #0
	strh r3, [r7, #28]
.L_08094d2c:
	ldr r2, [sp, #8]
	cmp r2, #7
	bhi .L_08094d7c
	ldrh r3, [r7, #28]
	mov r8, r3
	cmp r3, #0
	bne .L_08094d7c
	ldr r0, [sp, #12]
	ldr r6, [r0]
	bl Random16
	ldr r3, [r6]
	lsls r0, r0, #8
	ldr r5, .L_08094d9c
	adds r3, r3, r0
	adds r4, r3, r5
	str r4, [sp, #0]
	bl Random16
	ldr r3, [r6, #8]
	lsls r0, r0, #8
	adds r3, r3, r0
	ldr r4, [sp, #0]
	adds r0, r3, r5
	asrs r2, r0, #16
	str r0, [r7, #20]
	asrs r1, r4, #16
	str r4, [r7, #12]
	movs r0, #0
	bl Func_080091a8
	movs r3, #120
	lsls r0, r0, #16
	mov r1, r8
	str r0, [r7, #16]
	strh r3, [r7, #28]
	str r1, [r7, #24]
	ldr r2, [sp, #8]
	adds r2, #1
	str r2, [sp, #8]
.L_08094d7c:
	ldr r3, [sp, #4]
	adds r3, #1
	str r3, [sp, #4]
	adds r7, #32
	cmp r3, #31
	bhi .L_08094d8a
	b .L_08094be6
.L_08094d8a:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_08094d9c:
	.4byte 0xff800000
