@ Reconstructed thumb disassembly of a call-graph-reached code region
@ (code interleaved with its data), verified byte-identical by build_asm.
.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_08002322, 0x08002322
	.set sub_080030f8, 0x080030f8
	.set sub_08003dec, 0x08003dec
	.set sub_08003f3c, 0x08003f3c
	.set sub_08016418, 0x08016418
	.set sub_0801671c, 0x0801671c
	.set sub_08017a64, 0x08017a64
	.set sub_08017aa4, 0x08017aa4
	.set sub_08026388, 0x08026388
	.set sub_080b50b8, 0x080b50b8
	.set sub_080b50e0, 0x080b50e0
	.set sub_080f9010, 0x080f9010
	.global Region_08026b44
Region_08026b44:
	lsls r2, r7, #1
	ldr r3, [pc, #48]
	strh r3, [r6, r2]
	adds r0, r6, #0
	bl sub_08017a64
	lsrs r2, r0, #31
	ldr r3, [r5, #0]
	adds r2, r0, r2
	asrs r2, r2, #1
	subs r3, r3, r2
	subs r3, #8
	str r3, [r5, #0]
	adds r3, r3, r0
	cmp r3, #224
	ble .L_08026b6a
	movs r3, #224
	subs r3, r3, r0
	str r3, [r5, #0]
.L_08026b6a:
	ldr r3, [r5, #0]
	cmp r3, #0
	bge .L_08026b7c
	movs r3, #0
	str r3, [r5, #0]
	b .L_08026b7c
	.2byte 0x0000
	.4byte 0x00000000
.L_08026b7c:
	bl sub_0801671c
	ldr r2, [r5, #0]
	adds r0, r6, #0
	ldr r1, [sp, #72]
	movs r3, #4
	bl sub_08017aa4
	ldr r1, [sp, #56]
	movs r3, #2
	negs r3, r3
	ands r1, r3
	str r1, [sp, #56]
	ldr r2, [sp, #48]
	cmp r2, #0
	bne .L_08026b9e
	b .L_08026cdc
.L_08026b9e:
	ldr r3, [sp, #64]
	movs r7, #1
	cmp r7, r3
	blt .L_08026ba8
	b .L_08026cdc
.L_08026ba8:
	mov r5, sp
	adds r5, #164
	movs r6, #96
	movs r4, #172
	str r5, [sp, #16]
	add r6, sp
	ldr r5, [sp, #32]
	movs r0, #2
	add r4, sp
	mov r8, r6
	str r0, [sp, #12]
	mov r10, r4
	adds r5, #12
	mov r4, r8
.L_08026bc4:
	ldr r1, [sp, #16]
	ldrb r3, [r1, r7]
	ldr r2, [sp, #36]
	lsls r3, r3, #2
	adds r3, r2, r3
	str r3, [sp, #4]
	ldr r3, [sp, #12]
	mov r6, r10
	adds r1, r4, #0
	ldrh r0, [r3, r6]
	str r4, [sp, #8]
	bl sub_080b50b8
	ldr r3, [pc, #180]
	ldr r0, [r3, #0]
	lsls r0, r0, #12
	bl sub_08002322
	ldr r4, [sp, #8]
	cmp r0, #0
	bge .L_08026bf2
	ldr r1, [pc, #168]
	adds r0, r0, r1
.L_08026bf2:
	ldr r3, [r4, #4]
	asrs r2, r0, #15
	adds r3, r3, r2
	str r3, [r4, #4]
	ldr r2, [sp, #32]
	mov lr, r5
	mov r12, r2
	mov r3, lr
	mov r6, r12
	ldmia r6!, {r0, r1, r2}
	stmia r3!, {r0, r1, r2}
	ldr r3, [sp, #4]
	movs r1, #1
	ldrb r2, [r3, #2]
	adds r3, r1, #0
	ands r3, r2
	cmp r3, #0
	beq .L_08026c3a
	ldr r6, [sp, #4]
	ldr r1, [r4, #0]
	ldrb r3, [r6, #0]
	adds r1, r1, r3
	lsrs r3, r1, #31
	ldrb r2, [r6, #1]
	adds r1, r1, r3
	ldr r3, [r4, #4]
	adds r3, r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r1, r1, #1
	asrs r3, r3, #1
	str r1, [r4, #0]
	strb r1, [r6, #0]
	str r3, [r4, #4]
	strb r3, [r6, #1]
	b .L_08026c50
.L_08026c3a:
	ldrh r3, [r5, #6]
	ldrb r2, [r5, #4]
	ldr r0, [sp, #4]
	lsls r3, r3, #23
	lsrs r3, r3, #23
	adds r2, #8
	strb r1, [r0, #2]
	str r3, [r4, #0]
	strb r3, [r0, #0]
	str r2, [r4, #4]
	strb r2, [r0, #1]
.L_08026c50:
	ldrb r2, [r5, #5]
	movs r1, #13
	negs r1, r1
	adds r3, r1, #0
	adds r0, r2, #0
	mov r2, r8
	ands r0, r3
	ldr r1, [r2, #0]
	movs r3, #4
	orrs r0, r3
	ldr r3, [pc, #36]
	subs r1, #8
	ands r1, r3
	ldr r2, [pc, #36]
	ldrh r3, [r5, #6]
	ands r3, r2
	orrs r3, r1
	mov r6, r8
	strh r3, [r5, #6]
	ldr r3, [r6, #4]
	subs r3, #12
	strb r0, [r5, #5]
	strb r3, [r5, #4]
	ldr r1, [sp, #84]
	cmp r1, #255
	bne .L_08026c9c
	movs r2, #4
	negs r2, r2
	ands r0, r2
	b .L_08026ca6
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x03001e40
	.4byte 0x00007fff
.L_08026c9c:
	movs r3, #4
	negs r3, r3
	ands r0, r3
	movs r3, #1
	orrs r0, r3
.L_08026ca6:
	strb r0, [r5, #5]
	ldr r2, [sp, #44]
	movs r3, #31
	movs r6, #63
	ands r2, r3
	negs r6, r6
	ldrb r3, [r5, #7]
	adds r1, r6, #0
	lsls r2, r2, #1
	ands r3, r1
	orrs r3, r2
	adds r0, r5, #0
	strb r3, [r5, #7]
	movs r1, #240
	str r4, [sp, #8]
	bl sub_08003dec
	ldr r0, [sp, #12]
	ldr r1, [sp, #64]
	adds r0, #2
	adds r7, #1
	adds r5, #12
	str r0, [sp, #12]
	ldr r4, [sp, #8]
	cmp r7, r1
	bge .L_08026cdc
	b .L_08026bc4
.L_08026cdc:
	ldr r3, [pc, #404]
	ldr r6, [r3, #0]
	ldr r3, [pc, #404]
	ldr r5, [r3, #0]
	ldr r3, [pc, #404]
	ldr r2, [r3, #0]
	adds r3, r2, #0
	adds r3, #216
	ldr r3, [r3, #0]
	cmp r3, #0
	beq .L_08026d0c
	adds r2, #220
	ldr r3, [r2, #0]
	movs r5, #0
	movs r6, #0
	cmp r3, #0
	bne .L_08026d08
	movs r3, #60
	str r3, [r2, #0]
	movs r5, #1
	movs r6, #1
	b .L_08026d0c
.L_08026d08:
	subs r3, #1
	str r3, [r2, #0]
.L_08026d0c:
	movs r3, #1
	ands r3, r6
	cmp r3, #0
	beq .L_08026d8e
	ldr r2, [sp, #28]
	ldr r3, [sp, #24]
	movs r5, #0
	ldrh r4, [r2, r3]
	ldr r7, [sp, #76]
	str r5, [sp, #56]
	movs r3, #88
	ldrsh r3, [r7, r3]
	movs r0, #1
	negs r0, r0
	movs r1, #0
	cmp r3, #255
	beq .L_08026d54
	cmp r3, r4
	bne .L_08026d38
	movs r0, #128
	lsls r0, r0, #1
	b .L_08026d54
.L_08026d38:
	adds r1, #1
	cmp r1, #5
	bgt .L_08026d54
	lsls r3, r1, #1
	ldr r5, [sp, #76]
	adds r3, #88
	ldrsh r3, [r5, r3]
	cmp r3, #255
	beq .L_08026d54
	cmp r3, r4
	bne .L_08026d38
	movs r0, #128
	lsls r0, r0, #1
	orrs r0, r1
.L_08026d54:
	cmp r0, #0
	bge .L_08026d8a
	ldr r2, [sp, #76]
	adds r2, #102
	movs r7, #0
	ldrsh r3, [r2, r7]
	movs r5, #192
	movs r1, #0
	lsls r5, r5, #1
	cmp r3, #255
	beq .L_08026d8a
	cmp r3, r4
	bne .L_08026d72
	adds r0, r5, #0
	b .L_08026d8a
.L_08026d72:
	adds r1, #1
	adds r2, #2
	cmp r1, #5
	bgt .L_08026d8a
	movs r7, #0
	ldrsh r3, [r2, r7]
	cmp r3, #255
	beq .L_08026d8a
	cmp r3, r4
	bne .L_08026d72
	adds r0, r5, #0
	orrs r0, r1
.L_08026d8a:
	str r0, [sp, #68]
	b .L_08026df6
.L_08026d8e:
	ldr r0, [sp, #84]
	cmp r0, #255
	beq .L_08026df6
	movs r3, #144
	ands r3, r5
	cmp r3, #0
	beq .L_08026dc4
	movs r0, #111
	bl sub_080f9010
.L_08026da2:
	ldr r1, [sp, #68]
	adds r1, #1
	str r1, [sp, #68]
	adds r0, r1, #0
	ldr r1, [sp, #60]
	bl sub_080022fc
	str r0, [sp, #68]
	ldr r4, [sp, #28]
	lsls r2, r0, #1
	ldrh r3, [r4, r2]
	cmp r3, #254
	beq .L_08026da2
	ldr r7, [sp, #56]
	movs r3, #1
	orrs r7, r3
	str r7, [sp, #56]
.L_08026dc4:
	movs r3, #96
	ands r3, r5
	cmp r3, #0
	beq .L_08026df6
	movs r0, #111
	bl sub_080f9010
.L_08026dd2:
	ldr r0, [sp, #68]
	ldr r1, [sp, #60]
	adds r3, r0, r1
	subs r3, #1
	adds r0, r3, #0
	str r3, [sp, #68]
	bl sub_080022fc
	str r0, [sp, #68]
	ldr r2, [sp, #28]
	lsls r3, r0, #1
	ldrh r3, [r2, r3]
	cmp r3, #254
	beq .L_08026dd2
	ldr r4, [sp, #56]
	movs r3, #1
	orrs r4, r3
	str r4, [sp, #56]
.L_08026df6:
	ldr r3, [pc, #132]
	ldr r3, [r3, #0]
	ldr r3, [r3, #76]
	cmp r3, #0
	beq .L_08026e08
	movs r3, #2
	ands r3, r6
	cmp r3, #0
	beq .L_08026e16
.L_08026e08:
	movs r0, #113
	bl sub_080f9010
	movs r5, #1
	negs r5, r5
	str r5, [sp, #68]
	b .L_08026e26
.L_08026e16:
	movs r0, #1
	bl sub_080030f8
	ldr r6, [sp, #56]
	cmp r6, #0
	beq .L_08026e26
	bl sub_08026388
.L_08026e26:
	movs r0, #1
	bl sub_080030f8
	mov r7, r9
	ldr r0, [sp, #52]
	bl sub_08003f3c
	cmp r7, #0
	beq .L_08026e40
	mov r0, r9
	movs r1, #1
	bl sub_08016418
.L_08026e40:
	ldr r0, [sp, #72]
	movs r1, #1
	bl sub_08016418
	movs r1, #0
	ldr r0, [sp, #28]
	bl sub_080b50e0
	ldr r3, [pc, #40]
	ldr r2, [r3, #0]
	movs r3, #0
	str r3, [r2, #40]
	movs r0, #1
	bl sub_080030f8
	ldr r0, [sp, #68]
	add sp, #324
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
