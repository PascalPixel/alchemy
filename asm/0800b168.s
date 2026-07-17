@ Reconstructed thumb disassembly of a call-graph-reached code region
@ (code interleaved with its data), verified byte-identical by build_asm.
.syntax unified
	.thumb
	.set sub_08003d28, 0x08003d28
	.set sub_08003dec, 0x08003dec
	.set sub_0800aa0c, 0x0800aa0c
	.global Region_0800b168
Region_0800b168:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	adds r0, #32
	ldrb r0, [r0, #0]
	sub sp, #68
	lsrs r0, r0, #1
	str r0, [sp, #28]
	adds r0, r7, #0
	adds r0, #33
	str r0, [sp, #20]
	ldrb r0, [r0, #0]
	lsrs r0, r0, #1
	str r0, [sp, #24]
	movs r4, #8
	movs r0, #4
	str r4, [sp, #16]
	str r0, [sp, #12]
	ldmia r2!, {r4}
	ldmia r1!, {r0}
	ldr r2, [r2, #0]
	mov r9, r2
	ldmia r1!, {r2}
	str r2, [sp, #4]
	mov r8, r4
	lsls r3, r3, #16
	ldmia r1!, {r4}
	lsrs r3, r3, #16
	ldr r6, [r1, #0]
	mov r10, r0
	adds r1, r3, #0
	adds r0, r7, #0
	mov r11, r4
	bl sub_0800aa0c
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0800b1da
	movs r0, #128
	lsls r0, r0, #9
	cmp r8, r0
	bne .L_0800b1da
	cmp r9, r8
	bne .L_0800b1da
	ldrh r2, [r7, #30]
	adds r3, r2, #0
	cmp r3, #0
	bne .L_0800b1dc
	movs r1, #0
	str r1, [sp, #8]
	str r1, [sp, #32]
	b .L_0800b222
.L_0800b1da:
	ldrh r2, [r7, #30]
.L_0800b1dc:
	movs r3, #1
	str r3, [sp, #8]
	add r0, sp, #36
	ldr r3, [r0, #4]
	ldr r4, [pc, #224]
	ands r3, r4
	orrs r3, r2
	str r3, [r0, #4]
	mov r1, r8
	lsls r3, r1, #8
	ldr r1, [sp, #36]
	lsrs r3, r3, #16
	mov r2, r9
	ands r1, r4
	orrs r1, r3
	lsls r3, r2, #8
	ldr r2, [pc, #204]
	lsrs r3, r3, #16
	lsls r3, r3, #16
	ands r1, r2
	orrs r1, r3
	str r1, [sp, #36]
	cmp r5, #0
	beq .L_0800b21c
	ldrh r3, [r0, #0]
	negs r3, r3
	lsls r3, r3, #16
	adds r2, r4, #0
	lsrs r3, r3, #16
	ands r2, r1
	orrs r2, r3
	str r2, [sp, #36]
.L_0800b21c:
	bl sub_08003d28
	str r0, [sp, #32]
.L_0800b222:
	movs r3, #128
	lsls r3, r3, #9
	cmp r8, r3
	bgt .L_0800b22e
	cmp r9, r3
	ble .L_0800b246
.L_0800b22e:
	ldr r4, [sp, #28]
	ldr r0, [sp, #24]
	movs r3, #3
	lsls r4, r4, #1
	lsls r0, r0, #1
	movs r1, #16
	movs r2, #8
	str r3, [sp, #8]
	str r4, [sp, #28]
	str r0, [sp, #24]
	str r1, [sp, #16]
	str r2, [sp, #12]
.L_0800b246:
	ldr r3, [sp, #4]
	ldr r4, [pc, #132]
	cmp r3, r4
	bgt .L_0800b256
	movs r0, #1
	str r0, [sp, #0]
	movs r4, #0
	b .L_0800b260
.L_0800b256:
	mov r1, r11
	asrs r3, r1, #17
	adds r3, #10
	str r3, [sp, #0]
	movs r4, #2
.L_0800b260:
	mov r2, r11
	subs r3, r2, r6
	ldr r0, [sp, #12]
	asrs r3, r3, #16
	subs r6, r3, r0
	adds r3, r7, #0
	adds r3, #38
	ldrb r2, [r3, #0]
	movs r3, #1
	mov r1, r10
	ands r3, r2
	asrs r5, r1, #16
	cmp r3, #0
	beq .L_0800b2dc
	cmp r6, #159
	bgt .L_0800b2d8
	adds r0, r7, #0
	adds r0, #12
	ldrb r2, [r0, #5]
	movs r3, #4
	negs r3, r3
	ands r3, r2
	ldr r2, [sp, #8]
	orrs r3, r2
	strb r3, [r0, #5]
	ldr r2, [sp, #32]
	movs r3, #31
	ldrb r1, [r0, #7]
	ands r2, r3
	movs r3, #63
	negs r3, r3
	lsls r2, r2, #1
	ands r3, r1
	orrs r3, r2
	strb r3, [r0, #7]
	ldr r1, [sp, #16]
	ldr r3, [pc, #24]
	subs r2, r5, r1
	ands r2, r3
	ldrh r1, [r0, #6]
	ldr r3, [pc, #32]
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #6]
	strb r6, [r0, #4]
	adds r1, r4, #0
	bl sub_08003dec
	b .L_0800b2dc
	.2byte 0x0000
	.4byte 0x000001ff
	.4byte 0xffff0000
	.4byte 0x0000ffff
	.4byte 0xff9c0000
	.4byte 0xfffffe00
.L_0800b2d8:
	mov r2, r10
	asrs r5, r2, #16
.L_0800b2dc:
	ldr r3, [sp, #28]
	subs r2, r5, r3
	adds r3, r7, #0
	adds r3, #34
	ldrb r3, [r3, #0]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r4, r8
	muls r4, r3
	ldr r0, [pc, #120]
	adds r3, r4, #0
	adds r3, r3, r0
	asrs r3, r3, #16
	adds r4, r2, r3
	ldr r3, [sp, #4]
	mov r2, r11
	subs r1, r2, r3
	ldr r2, [sp, #24]
	asrs r1, r1, #16
	subs r1, r1, r2
	ldr r2, [sp, #20]
	ldrb r3, [r2, #0]
	adds r2, r7, #0
	adds r2, #35
	ldrb r2, [r2, #0]
	lsls r2, r2, #24
	asrs r2, r2, #24
	lsrs r3, r3, #1
	subs r3, r3, r2
	mov r2, r9
	muls r2, r3
	adds r3, r2, #0
	adds r3, r3, r0
	asrs r3, r3, #16
	subs r6, r1, r3
	cmp r4, #239
	bgt .L_0800b374
	cmp r6, #159
	bgt .L_0800b374
	ldr r3, [pc, #56]
	adds r0, r7, #0
	ldrh r2, [r0, #6]
	ands r4, r3
	ldr r3, [pc, #56]
	ands r3, r2
	orrs r3, r4
	strh r3, [r0, #6]
	strb r6, [r0, #4]
	ldrb r2, [r0, #5]
	movs r3, #4
	ldr r4, [sp, #8]
	negs r3, r3
	ands r3, r2
	orrs r3, r4
	strb r3, [r0, #5]
	ldr r1, [sp, #32]
	movs r3, #31
	ands r1, r3
	str r1, [sp, #32]
	movs r3, #63
	ldrb r2, [r0, #7]
	negs r3, r3
	lsls r1, r1, #1
	ands r3, r2
	orrs r3, r1
	strb r3, [r0, #7]
	ldr r1, [sp, #0]
	b .L_0800b370
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0xfffffe00
.L_0800b370:
	bl sub_08003dec
.L_0800b374:
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
