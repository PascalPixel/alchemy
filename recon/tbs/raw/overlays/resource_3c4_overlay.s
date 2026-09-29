.syntax unified
	.thumb
	.section .text.x0200806c,"ax",%progbits
	.global Func_0200006c
	.thumb_func
Func_0200006c:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #76]
	adds r4, r0, #0
	ldr r2, [r3]
	ldr r3, [r4]
	adds r1, r2, #0
	ldr r6, [pc, #68]
	movs r5, #8
	asrs r7, r3, #20
	adds r1, #52
.L_0200006c_4:
	ldmia r1!, {r0}
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r7, r3
	bne .L_0200006c_0
	ldr r3, [r4, #4]
	cmp r3, #0
	bge .L_0200006c_1
	adds r3, r3, r6
.L_0200006c_1:
	asrs r2, r3, #16
	ldr r3, [r0, #12]
	cmp r3, #0
	bge .L_0200006c_2
	adds r3, r3, r6
.L_0200006c_2:
	asrs r3, r3, #16
	cmp r2, r3
	bne .L_0200006c_0
	ldr r2, [r4, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_0200006c_3
.L_0200006c_0:
	adds r5, #1
	cmp r5, #65
	bls .L_0200006c_4
	movs r0, #0
.L_0200006c_3:
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x0000ffff
	.global StagedActor_AdvancePair
	.thumb_func
StagedActor_AdvancePair:
	.global Func_020000c4
	.thumb_func
Func_020000c4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #0
	sub sp, #12
	bl 0x0200b10c
	ldrh r3, [r0, #6]
	mov r8, r0
	lsrs r3, r3, #12
	ldr r0, [pc, #344]
	lsls r5, r3, #2
	ldr r2, [pc, #344]
	ldr r1, [r0, r5]
	mov r10, r2
	mov r3, r10
	adds r2, r1, #0
	mov r9, r0
	mov r0, r8
	ands r2, r3
	ldr r3, [r0, #8]
	mov r7, sp
	adds r3, r3, r2
	str r3, [r7]
	ldr r3, [r0, #12]
	str r3, [r7, #4]
	ldr r3, [r0, #16]
	lsls r1, r1, #16
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r0, r7, #0
	mov r1, r8
	bl 0x0200806c
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020000c4_1
	b .L_020000c4_2
.L_020000c4_1:
	mov r2, r9
	ldr r1, [r2, r5]
	mov r3, r10
	adds r2, r1, #0
	ands r2, r3
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r7]
	ldr r3, [r6, #12]
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	lsls r1, r1, #16
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r0, r7, #0
	adds r1, r6, #0
	bl 0x0200806c
	cmp r0, #0
	beq .L_020000c4_4
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_020000c4_2
.L_020000c4_4:
	ldr r3, [r6, #8]
	str r3, [r7]
	movs r0, #128
	ldr r3, [r6, #12]
	lsls r0, r0, #13
	adds r3, r3, r0
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	adds r0, r7, #0
	str r3, [r7, #8]
	adds r1, r6, #0
	bl 0x0200806c
	cmp r0, #0
	beq .L_020000c4_0
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_020000c4_2
.L_020000c4_0:
	adds r2, r6, #0
	adds r2, #34
	movs r3, #2
	strb r3, [r2]
	mov r2, r9
	ldr r1, [r2, r5]
	mov r3, r10
	adds r2, r1, #0
	ands r2, r3
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r7]
	ldr r3, [r6, #12]
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	lsls r1, r1, #16
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r0, r6, #0
	adds r1, r7, #0
	bl 0x0200b0a4
.L_020000c4_3:
	cmp r0, #0
	bgt .L_020000c4_2
	adds r3, r6, #0
	adds r3, #98
	ldrb r3, [r3]
	mov r10, r3
	cmp r3, #0
	bne .L_020000c4_2
	movs r1, #8
	mov r0, r8
	bl 0x0200b054
	ldr r5, [pc, #132]
	movs r0, #15
	bl 0x0200b024
	movs r0, #185
	bl 0x0200b1ac
	str r5, [r6, #48]
	str r5, [r6, #52]
	ldr r1, [r7]
.L_020000c4_5:
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl 0x0200b074
	mov r0, r8
	str r5, [r0, #48]
	str r5, [r0, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl 0x0200b074
	adds r0, r6, #0
	bl 0x0200b07c
	bl 0x0200b1a4
	ldr r3, [r7]
	str r3, [r6, #8]
	ldr r3, [r7, #8]
	mov r1, r10
	str r3, [r6, #16]
	str r1, [r6, #36]
	str r1, [r6, #44]
	movs r3, #128
	mov r2, r8
	lsls r3, r3, #24
	str r3, [r2, #56]
	str r3, [r2, #64]
	movs r0, #10
	ldrsh r3, [r2, r0]
	lsls r3, r3, #16
	str r1, [r2, #36]
	str r1, [r2, #44]
	str r3, [r2, #8]
	movs r1, #18
	ldrsh r3, [r2, r1]
	lsls r3, r3, #16
	str r3, [r2, #16]
	mov r0, r8
	movs r1, #1
	bl 0x0200b054
.L_020000c4_2:
	sub sp, #-12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200b1f0
	.4byte 0xffff0000
	.4byte 0x00003333
	.global StagedActor_FillGridAttributeRectangle
	.thumb_func
StagedActor_FillGridAttributeRectangle:
	.global Func_02000244
	.thumb_func
Func_02000244:
	push {r5, r6, lr}
	adds r4, r3, #0
	ldr r3, [sp, #12]
	mov r12, r3
	ldr r3, [pc, #80]
	adds r6, r1, #0
	adds r1, r2, #0
	ldr r2, [r3]
	ldr r5, [sp, #16]
	cmp r2, #0
	beq .L_02000244_0
	cmp r0, #2
	bhi .L_02000244_1
	lsls r3, r0, #1
	adds r3, r3, r0
	movs r0, #152
	lsls r0, r0, #1
	lsls r3, r3, #4
	adds r3, r3, r0
	ldr r0, [r2, r3]
	b .L_02000244_2
.L_02000244_1:
	ldr r0, [pc, #52]
.L_02000244_2:
	lsls r3, r1, #7
	adds r3, r6, r3
	lsls r3, r3, #2
	movs r1, #0
	adds r0, r0, r3
	cmp r1, r12
	bcs .L_02000244_0
.L_02000244_5:
	lsls r3, r1, #9
	movs r2, #0
	adds r3, r0, r3
	cmp r2, r4
	bcs .L_02000244_3
.L_02000244_4:
	adds r2, #1
	strb r5, [r3, #2]
	adds r3, #4
	cmp r2, r4
	bcc .L_02000244_4
.L_02000244_3:
	adds r1, #1
	cmp r1, r12
	bcc .L_02000244_5
.L_02000244_0:
	movs r0, #0
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x03001e70
	.4byte 0x02010000
	.global Func_020002a8
	.thumb_func
Func_020002a8:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	ldrh r3, [r5, #6]
	ldr r2, [pc, #144]
	lsrs r3, r3, #12
	lsls r7, r3, #2
	ldr r1, [r2, r7]
	ldr r2, [pc, #140]
	ldr r3, [r5, #8]
	ands r2, r1
	sub sp, #12
	mov r6, sp
	adds r3, r3, r2
	str r3, [r6]
	ldr r3, [r5, #12]
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	lsls r1, r1, #16
	adds r3, r3, r1
	adds r0, r6, #0
	adds r1, r5, #0
	str r3, [r6, #8]
	bl 0x0200806c
	cmp r0, #0
	beq .L_020002a8_0
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldr r2, [pc, #96]
	movs r1, #0
.L_020002a8_2:
	ldmia r2!, {r3}
	cmp r0, r3
	beq .L_020002a8_1
	adds r1, #1
	cmp r1, #5
	bls .L_020002a8_2
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #56]
	str r3, [r5, #64]
.L_020002a8_0:
	ldr r3, [pc, #60]
	ldr r2, [pc, #60]
	ldr r1, [r3, r7]
	ldr r3, [r5, #8]
	ands r2, r1
	adds r3, r3, r2
	str r3, [r6]
	ldr r3, [r5, #12]
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	lsls r1, r1, #16
	adds r3, r3, r1
	adds r0, r5, #0
	adds r1, r6, #0
	str r3, [r6, #8]
	bl 0x0200b0a4
	cmp r0, #0
	ble .L_020002a8_1
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #56]
	str r3, [r5, #64]
.L_020002a8_1:
	movs r0, #0
	sub sp, #-12
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x0200b1f0
	.4byte 0xffff0000
	.4byte 0x0200b230
	.global Func_0200034c
	.thumb_func
Func_0200034c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #24
	str r0, [sp, #20]
	str r1, [sp, #16]
	str r2, [sp, #12]
	ldr r3, [pc, #256]
	movs r0, #0
	ldr r5, [r3]
	bl 0x0200b10c
	ldrh r3, [r0, #6]
	ldr r1, [sp, #20]
	lsrs r3, r3, #12
	movs r2, #8
	str r3, [r1]
	adds r5, #52
	str r2, [sp, #8]
	mov r9, r0
	mov r11, r5
.L_0200034c_4:
	mov r3, r11
	ldr r3, [r3]
	mov r10, r3
	ldr r3, [r3, #80]
	ldr r3, [r3, #40]
	movs r2, #0
	ldrsh r1, [r3, r2]
	ldr r3, [pc, #216]
	movs r4, #0
	str r1, [sp, #4]
	ldr r0, [pc, #216]
	str r3, [sp, #0]
	mov r8, r4
.L_0200034c_3:
	ldr r1, [sp, #0]
	ldmia r1!, {r3}
	ldr r2, [sp, #4]
	adds r4, r1, #0
	str r4, [sp, #0]
	cmp r2, r3
	bne .L_0200034c_0
	ldr r4, [sp, #12]
	mov r3, r8
	str r3, [r4]
	ldr r2, [sp, #20]
	ldr r3, [r2]
	ldr r4, [pc, #188]
	lsls r3, r3, #2
	mov r1, r9
	ldr r2, [r4, r3]
	ldr r1, [r1, #8]
	asrs r3, r2, #16
	mov lr, r1
	asrs r1, r1, #16
	adds r1, r1, r3
	asrs r7, r1, #4
	mov r1, r9
	ldr r1, [r1, #16]
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r3, r1, #16
	adds r3, r3, r2
	asrs r5, r3, #4
	mov r3, r10
	mov r12, r1
	movs r2, #10
	ldrsh r1, [r3, r2]
	ldr r3, [r0]
	adds r3, r1, r3
	asrs r6, r3, #4
	mov r3, r10
	movs r4, #18
	ldrsh r2, [r3, r4]
	ldr r3, [r0, #4]
	adds r3, r2, r3
	asrs r4, r3, #4
	ldr r3, [r0, #8]
	adds r1, r1, r3
	ldr r3, [r0, #12]
	adds r2, r2, r3
	asrs r1, r1, #4
	asrs r2, r2, #4
	cmp r6, r7
	bgt .L_0200034c_0
	cmp r7, r1
	bge .L_0200034c_0
	cmp r4, r5
	bgt .L_0200034c_0
	cmp r5, r2
	bge .L_0200034c_0
	movs r3, #1
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200034c_1
	mov r2, lr
	asrs r3, r2, #20
	cmp r6, r3
	beq .L_0200034c_0
	ldr r3, [sp, #8]
	ldr r4, [sp, #16]
	mov r0, r10
	str r3, [r4]
	b .L_0200034c_2
.L_0200034c_1:
	mov r1, r12
	asrs r3, r1, #20
	cmp r4, r3
	beq .L_0200034c_0
	ldr r2, [sp, #8]
	ldr r3, [sp, #16]
	mov r0, r10
	str r2, [r3]
	b .L_0200034c_2
.L_0200034c_0:
	movs r4, #1
	add r8, r4
	mov r1, r8
	adds r0, #16
	cmp r1, #5
	bls .L_0200034c_3
	ldr r3, [sp, #8]
	movs r2, #4
	adds r3, #1
	add r11, r2
	str r3, [sp, #8]
	cmp r3, #65
	bls .L_0200034c_4
	movs r0, #0
.L_0200034c_2:
	sub sp, #-24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x0200b230
	.4byte 0x0200b248
	.4byte 0x0200b1f0
	.global Func_02000474
	.thumb_func
Func_02000474:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	sub sp, #32
	movs r3, #0
	add r0, sp, #16
	adds r1, r6, #4
	adds r2, r6, #0
	str r3, [r6, #20]
	bl 0x0200834c
	mov r10, r0
	cmp r0, #0
	bne .L_02000474_0
	movs r0, #0
	b .L_02000474_1
.L_02000474_0:
	mov r0, r10
	adds r0, #34
	movs r3, #2
	str r0, [sp, #4]
	strb r3, [r0]
	ldr r3, [r6]
	movs r1, #0
	str r1, [sp, #12]
	ldr r5, [pc, #332]
	lsls r1, r3, #4
	adds r3, r1, #4
	ldr r2, [r5, r3]
	cmp r2, #0
	bge .L_02000474_2
	negs r2, r2
.L_02000474_2:
	adds r3, r1, #0
	adds r3, #12
	ldr r3, [r5, r3]
	cmp r3, #0
	bge .L_02000474_3
	negs r3, r3
.L_02000474_3:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #8]
	ldr r2, [r5, r1]
	cmp r2, #0
	bge .L_02000474_4
	negs r2, r2
.L_02000474_4:
	adds r3, r1, #0
	adds r3, #8
	ldr r3, [r5, r3]
	cmp r3, #0
	bge .L_02000474_5
	negs r3, r3
.L_02000474_5:
	adds r3, r2, r3
	asrs r3, r3, #4
	mov r9, r3
	ldr r3, [sp, #16]
	ldr r1, [pc, #276]
	add r2, sp, #20
	lsls r3, r3, #2
	mov r8, r2
	ldr r2, [r1, r3]
	ldr r3, [pc, #268]
	mov r4, r10
	ands r2, r3
	ldr r3, [r4, #8]
	mov r0, r8
	adds r3, r3, r2
	str r3, [r0]
	ldr r0, [r4, #12]
	mov r2, r8
	str r0, [r2, #4]
	ldr r3, [sp, #16]
	lsls r3, r3, #2
	ldr r2, [r1, r3]
	ldr r3, [r4, #16]
	lsls r2, r2, #16
	mov r4, r8
	adds r3, r3, r2
	str r3, [r4, #8]
	adds r4, r6, #0
	str r0, [r6, #12]
	adds r4, #8
	mov r11, r8
.L_02000474_11:
	ldr r3, [r6]
	ldr r0, [pc, #216]
	lsls r3, r3, #4
	adds r3, #4
	ldr r2, [r0, r3]
	mov r1, r8
	ldr r3, [r1, #8]
	lsls r2, r2, #16
	adds r3, r3, r2
	ldr r2, [sp, #8]
	movs r7, #0
	str r3, [r6, #16]
	cmp r7, r2
	bge .L_02000474_6
.L_02000474_10:
	ldr r3, [r6]
	ldr r0, [pc, #188]
	lsls r3, r3, #4
	ldr r2, [r0, r3]
	mov r1, r8
	ldr r3, [r1]
	lsls r2, r2, #16
	adds r3, r3, r2
	movs r5, #0
	str r3, [r6, #8]
	cmp r5, r9
	bge .L_02000474_7
.L_02000474_9:
	adds r1, r4, #0
	mov r0, r10
	str r4, [sp, #0]
	bl 0x0200b0a4
	ldr r4, [sp, #0]
	cmp r0, #2
	beq .L_02000474_8
	ldr r3, [r4]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	adds r5, #1
	str r3, [r4]
	cmp r5, r9
	blt .L_02000474_9
.L_02000474_7:
	ldr r3, [r6, #16]
	movs r0, #128
	lsls r0, r0, #13
	ldr r1, [sp, #8]
	adds r3, r3, r0
	adds r7, #1
	str r3, [r6, #16]
	cmp r7, r1
	blt .L_02000474_10
.L_02000474_6:
	ldr r2, [sp, #12]
	ldr r3, [sp, #16]
	adds r2, #1
	str r2, [sp, #12]
	ldr r0, [pc, #112]
	lsls r3, r3, #2
	ldr r2, [r0, r3]
	ldr r3, [pc, #108]
	mov r1, r11
	ands r2, r3
	ldr r3, [r1]
	adds r3, r3, r2
	str r3, [r1]
	ldr r3, [sp, #16]
	lsls r3, r3, #2
	ldr r2, [r0, r3]
	ldr r3, [r1, #8]
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r1, #8]
	b .L_02000474_11
.L_02000474_8:
	ldr r2, [sp, #4]
	movs r3, #0
	strb r3, [r2]
	ldr r3, [sp, #12]
	movs r0, #0
	cmp r3, #0
	beq .L_02000474_1
	ldr r3, [sp, #16]
	ldr r2, [pc, #60]
	lsls r3, r3, #2
	ldr r2, [r2, r3]
	ldr r3, [pc, #60]
	ldr r4, [sp, #12]
	ands r3, r2
	adds r1, r4, #0
	muls r1, r3
	lsls r2, r2, #16
	muls r2, r4
	mov r0, r10
	ldr r3, [r0, #8]
	adds r3, r3, r1
	str r3, [r6, #8]
	ldr r3, [r0, #12]
	str r3, [r6, #12]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r6, #16]
	movs r0, #1
.L_02000474_1:
	sub sp, #-32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0200b248
	.4byte 0x0200b1f0
	.4byte 0xffff0000
	.global Func_02000608
	.thumb_func
Func_02000608:
	sub sp, #16
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #40
	str r0, [sp, #72]
	str r1, [sp, #76]
	str r2, [sp, #80]
	str r3, [sp, #84]
	ldr r3, [pc, #640]
	ldr r3, [r3]
	movs r0, #0
	str r3, [sp, #12]
	bl 0x0200b10c
	ldrh r3, [r0, #6]
	ldr r0, [sp, #76]
	lsrs r3, r3, #12
	mov r8, r3
	bl 0x0200b10c
	ldr r3, [sp, #72]
	ldr r4, [pc, #616]
	lsls r1, r3, #4
	adds r3, r1, #4
	ldr r2, [r4, r3]
	adds r7, r0, #0
	cmp r2, #0
	bge .L_02000608_0
	negs r2, r2
.L_02000608_0:
	adds r3, r1, #0
	adds r3, #12
	ldr r3, [r4, r3]
	cmp r3, #0
	bge .L_02000608_1
	negs r3, r3
.L_02000608_1:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #8]
	ldr r2, [r4, r1]
	cmp r2, #0
	bge .L_02000608_2
	negs r2, r2
.L_02000608_2:
	adds r3, r1, #0
	adds r3, #8
	ldr r3, [r4, r3]
	cmp r3, #0
	bge .L_02000608_3
	negs r3, r3
.L_02000608_3:
	adds r3, r2, r3
	asrs r3, r3, #4
	mov r9, r3
	ldr r5, [pc, #560]
	ldr r3, [r7, #8]
	movs r6, #128
	add r0, sp, #28
	lsls r6, r6, #8
	str r6, [r7, #48]
	str r5, [r7, #52]
	str r3, [r0]
	ldr r3, [r7, #16]
	str r3, [r0, #8]
	ldr r2, [sp, #72]
	lsls r2, r2, #4
	ldr r3, [r4, r2]
	ldr r1, [r7, #8]
	lsls r3, r3, #16
	adds r1, r1, r3
	mov r11, r0
	add r0, sp, #16
	str r1, [r0]
	adds r2, #4
	ldr r3, [r4, r2]
	ldr r2, [r7, #16]
	lsls r3, r3, #16
	adds r2, r2, r3
	asrs r1, r1, #20
	asrs r2, r2, #20
	str r1, [r0]
	str r2, [r0, #8]
	ldr r3, [sp, #8]
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r0, #0
	mov r3, r9
	bl 0x02008244
	adds r2, r5, #0
	movs r0, #0
	adds r1, r6, #0
	bl 0x0200b114
	movs r1, #8
	movs r0, #0
	bl 0x0200b144
	movs r0, #15
	bl 0x0200b0ec
	mov r4, r11
	ldr r2, [sp, #80]
	ldr r3, [r4]
	subs r1, r2, r3
	cmp r1, #0
	bge .L_02000608_4
	ldr r0, [pc, #456]
	adds r1, r1, r0
.L_02000608_4:
	mov r4, r11
	ldr r2, [sp, #88]
	ldr r3, [r4, #8]
	subs r2, r2, r3
	asrs r1, r1, #17
	cmp r2, #0
	bge .L_02000608_5
	ldr r0, [pc, #440]
	adds r2, r2, r0
.L_02000608_5:
	asrs r2, r2, #17
	movs r0, #0
	bl 0x0200b12c
	movs r0, #0
	bl 0x0200b10c
	ldr r3, [pc, #424]
	str r3, [r0, #108]
	movs r0, #4
	bl 0x0200b0ec
	mov r3, r8
	subs r3, #6
	cmp r3, #7
	bhi .L_02000608_6
	adds r0, r7, #0
	movs r1, #3
	bl 0x0200b054
	b .L_02000608_7
.L_02000608_6:
	adds r0, r7, #0
	movs r1, #2
	bl 0x0200b054
.L_02000608_7:
	movs r0, #239
	bl 0x0200b1ac
	adds r0, r7, #0
	ldr r1, [sp, #80]
	ldr r2, [sp, #84]
	ldr r3, [sp, #88]
	bl 0x0200b074
	movs r0, #0
	bl 0x0200b134
	movs r0, #0
	movs r1, #2
	bl 0x0200b144
	movs r0, #0
	ldr r1, [pc, #360]
	ldr r2, [pc, #344]
	bl 0x0200b114
	ldr r2, [pc, #356]
	mov r1, r8
	lsls r3, r1, #2
	ldr r0, [r2, r3]
	asrs r3, r0, #16
	lsls r3, r3, #16
	lsls r0, r0, #16
	asrs r1, r3, #16
	asrs r2, r0, #16
	lsrs r3, r3, #31
	lsrs r0, r0, #31
	adds r1, r1, r3
	adds r2, r2, r0
	asrs r1, r1, #1
	asrs r2, r2, #1
	movs r0, #0
	bl 0x0200b12c
	ldr r3, [sp, #92]
	cmp r3, #0
	beq .L_02000608_8
	bl 0x0200b1c0
.L_02000608_8:
	movs r0, #0
	bl 0x0200b134
	movs r1, #1
	movs r0, #0
	bl 0x0200b144
	movs r0, #0
	bl 0x0200b10c
	movs r2, #0
	str r2, [r0, #108]
	adds r0, r7, #0
	bl 0x0200b07c
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200b1ac
	movs r0, #213
	bl 0x0200b1ac
	ldr r3, [sp, #80]
	str r3, [r7, #8]
	ldr r3, [sp, #88]
	str r3, [r7, #16]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	adds r0, r7, #0
	movs r1, #1
	bl 0x0200b054
	ldr r2, [sp, #72]
	ldr r4, [pc, #220]
	lsls r2, r2, #4
	ldr r3, [r4, r2]
	ldr r0, [sp, #80]
	lsls r3, r3, #16
	adds r2, #4
	adds r0, r0, r3
	ldr r3, [r4, r2]
	ldr r1, [sp, #88]
	lsls r3, r3, #16
	adds r1, r1, r3
.L_020007de:
	ldr r2, [sp, #12]
	asrs r0, r0, #20
	asrs r1, r1, #20
	mov r10, r4
	movs r4, #158
	str r0, [sp, #80]
	str r1, [sp, #88]
	lsls r4, r4, #1
	adds r3, r2, r4
	ldr r3, [r3]
	mov r8, r3
	mov r2, r8
	asrs r2, r2, #20
	ldr r4, [sp, #12]
	mov r8, r2
	movs r2, #160
	lsls r2, r2, #1
	adds r3, r4, r2
	ldr r6, [r3]
	mov r4, r8
	asrs r6, r6, #20
	adds r3, r4, r0
	adds r2, r6, r1
	str r3, [sp, #0]
	str r2, [sp, #4]
	ldr r3, [sp, #8]
	mov r2, r9
	bl 0x0200b09c
	ldr r0, [sp, #8]
	ldr r1, [sp, #80]
	ldr r2, [sp, #88]
	str r0, [sp, #0]
	movs r5, #255
	mov r3, r9
	movs r0, #0
	str r5, [sp, #4]
	bl 0x02008244
	ldr r3, [sp, #8]
	ldr r1, [sp, #80]
	ldr r2, [sp, #88]
	str r3, [sp, #0]
	movs r0, #2
	mov r3, r9
	str r5, [sp, #4]
	bl 0x02008244
	ldr r2, [sp, #72]
	mov r4, r10
	lsls r2, r2, #4
	ldr r3, [r4, r2]
	mov r0, r11
	ldr r1, [r0]
	lsls r3, r3, #16
	adds r2, #4
	adds r1, r1, r3
	ldr r3, [r4, r2]
	ldr r2, [r0, #8]
	lsls r3, r3, #16
	adds r2, r2, r3
	asrs r1, r1, #20
	asrs r2, r2, #20
	str r1, [r0]
	str r2, [r0, #8]
	add r8, r1
	adds r6, r6, r2
	str r1, [sp, #0]
	str r2, [sp, #4]
	ldr r3, [sp, #8]
	mov r0, r8
	adds r1, r6, #0
	mov r2, r9
	bl 0x0200b09c
	ldr r3, [sp, #8]
	mov r2, r11
	ldr r1, [r2]
	movs r4, #0
	ldr r2, [r2, #8]
	movs r0, #2
	str r3, [sp, #0]
	mov r3, r9
	str r4, [sp, #4]
	bl 0x02008244
	bl 0x0200b1a4
	sub sp, #-40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r3}
	sub sp, #-16
	bx r3
	.2byte 0x0000
	.2byte 0x1e70
	.2byte 0x0300
	.2byte 0xb248
	.2byte 0x0200
	.2byte 0x1999
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0x0001
	.2byte 0x82a9
	.2byte 0x0200
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0xb1f0
	.2byte 0x0200
	.global Func_020008c0
	.thumb_func
Func_020008c0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, [pc, #260]
	ldr r3, [r3]
	sub sp, #32
	mov r10, r3
	bl 0x0200b10c
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r1, [pc, #244]
	movs r5, #0
	ldr r3, [r1, r5]
	cmp r2, r3
	bne .L_020008c0_0
	add r7, sp, #8
	b 0x0200890c
.L_020008c0_0:
	add r7, sp, #8
	mov r12, r7
	movs r6, #7
	adds r4, r1, #0
	mov r3, r12
	adds r5, #1
	str r6, [r3]
	cmp r5, #5
	bhi 0x0200890e
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	adds r4, #4
.L_02000902:
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [r4]
	cmp r2, r3
	bne 0x020088f2
	str r5, [r7]
	ldr r2, [r7]
	cmp r2, #6
	bls .L_02000902_0
	movs r0, #0
	b .L_02000902_1
.L_02000902_0:
	ldr r3, [r0, #8]
	str r3, [r7, #8]
	mov r12, r3
	ldr r3, [r0, #12]
	str r3, [r7, #12]
	ldr r0, [r0, #16]
	lsls r1, r2, #4
	str r0, [r7, #16]
	ldr r4, [pc, #172]
	adds r5, r1, #4
	ldr r2, [r4, r5]
	mov lr, r0
	cmp r2, #0
	bge .L_02000902_2
	negs r2, r2
.L_02000902_2:
	adds r3, r1, #0
	adds r3, #12
	ldr r3, [r4, r3]
	cmp r3, #0
	bge .L_02000902_3
	negs r3, r3
.L_02000902_3:
	adds r3, r2, r3
	ldr r0, [r4, r1]
	asrs r3, r3, #4
	mov r8, r3
	adds r6, r0, #0
	cmp r0, #0
	bge .L_02000902_4
	negs r6, r0
.L_02000902_4:
	adds r3, r1, #0
	adds r3, #8
	ldr r3, [r4, r3]
	cmp r3, #0
	bge .L_02000902_5
	negs r3, r3
.L_02000902_5:
	lsls r0, r0, #16
	add r0, r12
	str r0, [r7, #8]
	ldr r1, [r4, r5]
	lsls r1, r1, #16
	add r1, lr
	asrs r0, r0, #20
	asrs r1, r1, #20
	adds r6, r6, r3
	movs r3, #158
	str r0, [r7, #8]
	str r1, [r7, #16]
	lsls r3, r3, #1
	add r3, r10
	ldr r3, [r3]
	asrs r5, r3, #20
	movs r3, #160
	lsls r3, r3, #1
	add r3, r10
	ldr r3, [r3]
	asrs r3, r3, #20
	adds r2, r5, r0
	adds r3, r3, r1
	asrs r6, r6, #4
	str r2, [sp, #0]
	str r3, [sp, #4]
	adds r2, r6, #0
	mov r3, r8
	bl 0x0200b09c
	mov r3, r8
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r5, #255
	str r3, [sp, #0]
	movs r0, #0
	adds r3, r6, #0
	str r5, [sp, #4]
	bl 0x02008244
	mov r3, r8
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r0, #2
	str r3, [sp, #0]
	adds r3, r6, #0
	str r5, [sp, #4]
	bl 0x02008244
	movs r0, #1
.L_02000902_1:
	sub sp, #-32
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x1e70
	.2byte 0x0300
	.2byte 0xb230
	.2byte 0x0200
	.4byte 0x0200b248
	.section .text.x020090c2,"ax",%progbits
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #0
	sub	sp, #56
	bl 0x0200b10c
	adds	r6, r0, #0
	bl 0x0200b0f4
	movs	r1, #6
	adds	r0, r6, #0
	bl 0x0200b054
	movs	r0, #0
	bl 0x0200b14c
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200b054
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200b0ac
	movs	r0, #85
	adds	r0, r0, r6
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	mov	sl, r0
	movs	r0, #152
	bl 0x0200b1ac
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r6, #40]
	movs	r0, #192
	ldr	r3, [r6, #16]
	lsls	r0, r0, #12
	adds	r3, r3, r0
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	adds	r0, r6, #0
	bl 0x0200b074
	movs	r0, #6
	bl 0x0200b024
	add	r3, sp, #16
	mov	r8, r3
	ldr	r3, [pc, #156]
	mov	r2, sl
	mov	r0, r8
	movs	r5, #0
	strb	r5, [r2, #0]
	str	r3, [r0, #36]
	movs	r0, #127
	bl 0x0200b1ac
	movs	r7, #0
.L_02001142:
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #136]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	str	r3, [r6, #60]
	movs	r0, #1
	bl 0x0200b024
	movs	r3, #1
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_020011a6
	bl 0x0200b034
	movs	r1, #10
	bl 0x0200b01c
	ldr	r3, [pc, #108]
	subs	r0, #5
	adds	r5, r0, #0
	muls	r5, r3
	bl 0x0200b034
	movs	r1, #10
	bl 0x0200b01c
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #2
	adds	r3, r3, r0
	lsls	r4, r3, #6
	subs	r4, r4, r3
	lsls	r4, r4, #3
	adds	r4, r4, r0
	ldr	r3, [pc, #80]
	negs	r4, r4
	adds	r4, r4, r3
	movs	r3, #0
	ldr	r0, [r6, #8]
	ldr	r1, [r6, #12]
	ldr	r2, [r6, #16]
	str	r3, [sp, #0]
	ldr	r3, [pc, #68]
	str	r3, [sp, #8]
	mov	r3, r8
	str	r3, [sp, #12]
	adds	r3, r5, #0
	str	r4, [sp, #4]
	bl Effect_Spawn
.L_020011a6:
	adds	r7, #1
	cmp	r7, #7
	bls.n	.L_02001142
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200b0ac
	movs	r3, #3
	mov	r0, sl
	strb	r3, [r0, #0]
	bl 0x0200b0fc
	add	sp, #56
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte Effect_AdvanceMotion
	.4byte 0xfffe0000
	.4byte 0x00003332
	.4byte 0xffff8003
	.2byte 0x0001
	.2byte 0x0100
	.section .rodata.part1,"a",%progbits
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.4byte 0x000000cf
	.4byte 0x000000cd
	.4byte 0x000000e4
	.4byte 0x000000e5
	.4byte 0x0000012a
	.4byte 0x00000129
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff0
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global Data_0200b350
Data_0200b350:
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xffe00000
	.4byte 0xffd00000
	.4byte 0xffc00000
	.4byte 0xffb00000
	.4byte 0xffb00000
	.global gEffectScripts
gEffectScripts:
	.4byte 0x0200b2a8
	.4byte 0x0200b2e0
	.4byte 0x0200b318
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00009999
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00009999
	.4byte 0x00000015
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte SceneState_ApplyArgMode0AndReturnZero
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00019999
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000023
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte BabiChika_UpdateFlickerEffect
	.4byte 0x00000000
	.4byte 0x00000082
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000010
	.global Data_0200b3ec
Data_0200b3ec:
	.4byte 0x003b001c
	.4byte 0x00020001
	.4byte 0x001a0004
	.4byte 0x0001003b
	.4byte 0x00040002
	.4byte 0x003b0018
	.4byte 0x00020001
	.4byte 0xffff0004
	.global Data_0200b40c
Data_0200b40c:
	.4byte 0x003b001a
	.4byte 0x00020001
	.4byte 0x001c0004
	.4byte 0x0001003b
	.4byte 0x00040002
	.4byte 0x003b001e
	.4byte 0x00020001
	.4byte 0xffff0004
	.global gBabiChikaEntrancesOther
gBabiChikaEntrancesOther:
	.4byte 0xffff0000
	.4byte 0x000002e8
	.4byte 0x40000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x000001b8
	.4byte 0x40000228
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiChikaEntrances1
gBabiChikaEntrances1:
	.4byte 0xffff0000
	.4byte 0x000002e8
	.4byte 0x40000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000068
	.4byte 0x40000098
	.4byte 0x00300000
	.4byte 0x01200030
	.4byte 0x00000170
	.4byte 0xffff0002
	.4byte 0x000000c8
	.4byte 0xc0000148
	.4byte 0x00300000
	.4byte 0x01200030
	.4byte 0x00000170
	.4byte 0xffff0003
	.4byte 0x000000b8
	.4byte 0x40000228
	.4byte 0x00400000
	.4byte 0x013001d0
	.4byte 0x00000330
	.4byte 0xffff0004
	.4byte 0x000000a8
	.4byte 0xc0000330
	.4byte 0x00400000
	.4byte 0x013001d0
	.4byte 0x00000330
	.4byte 0xffff0005
	.4byte 0x00000208
	.4byte 0x40000068
	.4byte 0x01500000
	.4byte 0x02400030
	.4byte 0x00000120
	.4byte 0xffff0006
	.4byte 0x00000188
	.4byte 0xc00000c0
	.4byte 0x01500000
	.4byte 0x02400030
	.4byte 0x00000120
	.4byte 0xffff0007
	.4byte 0x00000208
	.4byte 0xc0000100
	.4byte 0x01500000
	.4byte 0x02400030
	.4byte 0x00000120
	.4byte 0xffff0008
	.4byte 0x00000238
	.4byte 0x400001b8
	.4byte 0x01800000
	.4byte 0x02700160
	.4byte 0x00000280
	.4byte 0xffff0009
	.4byte 0x000001b8
	.4byte 0xc0000288
	.4byte 0x01800000
	.4byte 0x02700160
	.4byte 0x00000280
	.4byte 0xffff000a
	.4byte 0x000002e8
	.4byte 0x40000068
	.4byte 0x02700000
	.4byte 0x03600028
	.4byte 0x00000120
	.4byte 0xffff000b
	.4byte 0x000002a8
	.4byte 0xc0000100
	.4byte 0x02700000
	.4byte 0x03600028
	.4byte 0x00000120
	.4byte 0xffff000c
	.4byte 0x000001f8
	.4byte 0x40000318
	.4byte 0x01700000
	.4byte 0x026002d0
	.4byte 0x000003d0
	.4byte 0xffff000d
	.4byte 0x00000238
	.4byte 0x40000368
	.4byte 0x01700000
	.4byte 0x026002d0
	.4byte 0x000003d0
	.4byte 0xffff000e
	.4byte 0x00000308
	.4byte 0xc00001b0
	.4byte 0x02b00000
	.4byte 0x03a00170
	.4byte 0x00000280
	.4byte 0xffff000f
	.4byte 0x00000348
	.4byte 0xc0000260
	.4byte 0x02b00000
	.4byte 0x03a00170
	.4byte 0x00000280
	.4byte 0xffff0010
	.4byte 0x000002e8
	.4byte 0x40000318
	.4byte 0x02b00000
	.4byte 0x03a002e0
	.4byte 0x000003d0
	.4byte 0xffff0011
	.4byte 0x000002e8
	.4byte 0xc00003b0
	.4byte 0x02b00000
	.4byte 0x03a002e0
	.4byte 0x000003d0
	.4byte 0xffff0012
	.4byte 0x00000368
	.4byte 0xc0000370
	.4byte 0x02b00000
	.4byte 0x03a002e0
	.4byte 0x000003d0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiChikaEntrances2
gBabiChikaEntrances2:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000278
	.4byte 0x40000158
	.4byte 0x02500000
	.4byte 0x03700040
	.4byte 0x00000190
	.4byte 0xffff0002
	.4byte 0x00000348
	.4byte 0x40000078
	.4byte 0x02500000
	.4byte 0x03700040
	.4byte 0x00000190
	.4byte 0xffff0003
	.4byte 0x00000338
	.4byte 0x40000158
	.4byte 0x02500000
	.4byte 0x03700040
	.4byte 0x00000190
	.4byte 0xffff0004
	.4byte 0x000000d8
	.4byte 0x40000068
	.4byte 0x00200000
	.4byte 0x01100030
	.4byte 0x00000140
	.4byte 0xffff0005
	.4byte 0x000000d8
	.4byte 0xc0000128
	.4byte 0x00200000
	.4byte 0x01100030
	.4byte 0x00000140
	.4byte 0xffff0006
	.4byte 0x00000168
	.4byte 0x40000068
	.4byte 0x01300000
	.4byte 0x02200030
	.4byte 0x00000160
	.4byte 0xffff0007
	.4byte 0x000001e8
	.4byte 0xc0000148
	.4byte 0x01300000
	.4byte 0x02200030
	.4byte 0x00000160
	.4byte 0xffff0008
	.4byte 0x00000078
	.4byte 0xc0000238
	.4byte 0x00500000
	.4byte 0x01c00190
	.4byte 0x00000250
	.4byte 0xffff0009
	.4byte 0x000000d8
	.4byte 0x400001c8
	.4byte 0x00500000
	.4byte 0x01c00190
	.4byte 0x00000250
	.4byte 0xffff000a
	.4byte 0x00000138
	.4byte 0xc0000238
	.4byte 0x00500000
	.4byte 0x01c00190
	.4byte 0x00000250
	.4byte 0xffff000b
	.4byte 0x00000198
	.4byte 0x400001c8
	.4byte 0x00500000
	.4byte 0x01c00190
	.4byte 0x00000250
	.4byte 0xffff000c
	.4byte 0x00000088
	.4byte 0x400002c8
	.4byte 0x00100000
	.4byte 0x01000290
	.4byte 0x00000370
	.4byte 0xffff000d
	.4byte 0x00000188
	.4byte 0x400002c8
	.4byte 0x01200000
	.4byte 0x02300290
	.4byte 0x00000360
	.4byte 0xffff000e
	.4byte 0x00000208
	.4byte 0x40000308
	.4byte 0x01200000
	.4byte 0x02300290
	.4byte 0x00000360
	.4byte 0xffff000f
	.4byte 0x00000288
	.4byte 0xc00003b8
	.4byte 0x02600000
	.4byte 0x03500330
	.4byte 0x000003d0
	.4byte 0xffff0010
	.4byte 0x00000308
	.4byte 0x40000388
	.4byte 0x02600000
	.4byte 0x03500330
	.4byte 0x000003d0
	.4byte 0xffff0011
	.4byte 0x000002b8
	.4byte 0x400002a8
	.4byte 0x02700000
	.4byte 0x03600250
	.4byte 0x00000300
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiChikaRegions2
gBabiChikaRegions2:
	.4byte 0x00100270
	.4byte 0x02800150
	.4byte 0x01600020
	.4byte 0x000fffff
	.4byte 0x00100330
	.4byte 0x03400150
	.4byte 0x01600020
	.4byte 0x0010ffff
	.4byte 0x00100340
	.4byte 0x03500070
	.4byte 0x00800020
	.4byte 0x0011ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200b85c
Data_0200b85c:
	.4byte 0x000000ac
	.4byte 0x001010ae
	.4byte 0x002030ac
	.4byte 0x003020ac
	.4byte 0x004050ac
	.4byte 0x005040ac
	.4byte 0x006080ac
	.4byte 0x0070a0ac
	.4byte 0x008060ac
	.4byte 0x0090c0ac
	.4byte 0x00a070ac
	.4byte 0x00b100ac
	.4byte 0x00c090ac
	.4byte 0x00d0e0ac
	.4byte 0x00e0d0ac
	.4byte 0x00f040ad
	.4byte 0x0100b0ac
	.4byte 0x011060ad
	.4byte 0x012110ad
	.4byte 0x000000ad
	.4byte 0x0010f0ac
	.4byte 0x002090ad
	.4byte 0x003110ac
	.4byte 0x0040b0ad
	.4byte 0x005050ad
	.4byte 0x006070ad
	.4byte 0x0070c0ad
	.4byte 0x0080d0ad
	.4byte 0x009080ad
	.4byte 0x00a0a0ad
	.4byte 0x00b0f0ad
	.4byte 0x00c120ac
	.4byte 0x00d0e0ad
	.4byte 0x00e120b7
	.4byte 0x00f040b0
	.4byte 0x010050b0
	.4byte 0x011060b0
	.4byte 0x000001ff
	.global Data_0200b8f4
Data_0200b8f4:
	.4byte 0x000000fd
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0x000000fd
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x01024000
	.4byte 0x000000fd
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0x000000fd
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x01024000
	.4byte 0x000000fe
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00024000
	.4byte 0x000000fe
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x01024000
	.4byte 0x000000fe
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x01024000
	.4byte 0x000000f8
	.4byte 0x0200b3a8
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x031a0000
	.4byte 0x0000c000
	.4byte 0x000000f8
	.4byte 0x0200b3a8
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x031a0000
	.4byte 0x0000c000
	.4byte 0x097000f8
	.4byte 0x0200b3a8
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x03580000
	.4byte 0x0000c000
	.4byte 0x000000ff
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0002c000
	.4byte 0x000000ff
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0002c000
	.4byte 0x000000e3
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0x000000e3
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0x00000114
	.4byte 0x0200b378
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200ba74
Data_0200ba74:
	.4byte 0x000000ff
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0002c000
	.4byte 0x000000e3
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0x00000101
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0002c000
	.4byte 0x00000101
	.4byte 0x00000007
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0102c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0102c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0102c000
	.4byte 0x00000102
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00024000
	.4byte 0x00000102
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00024000
	.4byte 0x000000fd
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0x000000fd
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0x000001f4
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00024000
	.4byte 0x000001f4
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00024000
	.4byte 0x000001f4
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00024000
	.4byte 0x000001f4
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00024000
	.4byte 0x000001f4
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200bc0c
Data_0200bc0c:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00004602
	.4byte 0xffff0017
	.4byte FieldScene_RunStepWith6
	.4byte 0x00008602
	.4byte 0xffff0017
	.4byte SceneActor_ApplyPointLeftOfActorZero
	.4byte 0x00000202
	.4byte 0xffff0018
	.4byte 0x02009319
	.4byte 0x00004602
	.4byte 0xffff0019
	.4byte 0x020090c5
	.4byte 0x00000202
	.4byte 0xffff001a
	.4byte 0x02009375
	.4byte 0x00004602
	.4byte 0xffff001b
	.4byte FieldScene_RunStepWith6
	.4byte 0x00000602
	.4byte 0xffff001b
	.4byte SceneActor_PassPointTwoRightOfActorZero
	.4byte 0x00008602
	.4byte 0xffff0032
	.4byte SceneState_RunUnlessActorZeroAtTile32x50
	.4byte 0x00000602
	.4byte 0xffff0032
	.4byte SceneActor_PassPointTwoRightOfActorZero
	.4byte 0x0000c602
	.4byte 0xffff0032
	.4byte SceneState_RunUnlessActorZeroAt30_52
	.4byte 0x00004602
	.4byte 0xffff0032
	.4byte SceneActor_PassRaisedPointOfActorZero
	.4byte 0x00004602
	.4byte 0xffff001c
	.4byte SceneActor_CheckTwoUnitsAboveActorZero
	.4byte 0x00000202
	.4byte 0xffff001c
	.4byte 0x020093e1
	.4byte 0x00000202
	.4byte 0x0971001d
	.4byte FieldScene_RunFourCallSequenceB
	.4byte 0x00000602
	.4byte 0x0200001d
	.4byte SceneActor_PassPointTwoRightOfActorZero
	.4byte 0x00004602
	.4byte 0x0200001d
	.4byte SceneActor_PassRaisedPointOfActorZero
	.4byte 0x0000c602
	.4byte 0x0200001d
	.4byte SceneActor_PassActorZeroOffsetPoint
	.4byte 0x00008602
	.4byte 0x0200001f
	.4byte SceneActor_ApplyPointLeftOfActorZero
	.4byte 0x00000602
	.4byte 0xffff001f
	.4byte SceneActor_PassPointTwoRightOfActorZero
	.4byte 0x00000202
	.4byte 0x0972001e
	.4byte FieldScene_RunFourStepSequenceA
	.4byte 0x00008602
	.4byte 0x0201001e
	.4byte SceneActor_ApplyPointLeftOfActorZero
	.4byte 0x00004602
	.4byte 0x0201001e
	.4byte SceneActor_PassRaisedPointOfActorZero
	.4byte 0x0000c602
	.4byte 0x0201001e
	.4byte SceneActor_PassActorZeroOffsetPoint
	.4byte 0x00000602
	.4byte 0x02020020
	.4byte FieldScene_RunFourStepSequenceA
	.4byte 0x00000602
	.4byte 0x02010020
	.4byte SceneActor_PassPointTwoRightOfActorZero
	.4byte 0x00008602
	.4byte 0xffff0020
	.4byte SceneActor_ApplyPointLeftOfActorZero
	.4byte 0x00000003
	.4byte 0xffff0028
	.4byte SceneDialogue_RunFlag982Or983Dialogue
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte FieldScene_PlaceAndPinSlots8And9
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte FieldScene_PlaceAndPinSlots8And9
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte FieldScene_PlaceAndPinSlots10And11
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte FieldScene_PlaceAndPinSlots10And11
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte FieldScene_RunScene3c4_02002480
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte FieldScene_RunScene3c4_02002480
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte FieldScene_RunScene3c4_02002480
	.4byte 0x00004e15
	.4byte 0x0200000f
	.4byte SceneActor_MirrorFlag201IntoSlot14
	.4byte 0x00004e15
	.4byte 0x02010010
	.4byte SceneActor_SetActor14Field98ByFlag200
	.4byte 0x00004e15
	.4byte 0x09700011
	.4byte SceneState_ApplyFlag970
	.4byte 0x00008c15
	.4byte 0x09710012
	.4byte FieldScene_RunSupplementalSequenceTwo
	.4byte 0x00001815
	.4byte 0x02000014
	.4byte ActorPresentation_ConfigureActorTwentyAndFlag200
	.4byte 0x00008c15
	.4byte 0x09720013
	.4byte FieldScene_RunSupplementalSequenceOne
	.4byte 0x00001815
	.4byte 0x02010015
	.4byte SceneActor_ConfigureSlot21AndSetFlag201
	.4byte 0x10002115
	.4byte 0x02020013
	.4byte FieldScene_SetActor19TableB3B8
	.4byte 0x00002115
	.4byte 0x02020013
	.4byte SceneState_SetValue202ThenCall
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200bef4
Data_0200bef4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000021
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000202
	.4byte 0xffff0019
	.4byte FieldScene_RunFourStepSequenceB
	.4byte 0x00004602
	.4byte 0xffff001a
	.4byte FieldScene_RunStepWith6
	.4byte 0x00008602
	.4byte 0xffff001a
	.4byte SceneActor_ApplyPointLeftOfActorZero
	.4byte 0x00004602
	.4byte 0xffff001b
	.4byte 0x020090c5
	.4byte 0x00000202
	.4byte 0xffff001c
	.4byte FieldScene_RunThreeStepSequence
	.4byte 0x00000602
	.4byte 0xffff001e
	.4byte SceneActor_PassPointTwoRightOfActorZero
	.4byte 0x00008602
	.4byte 0xffff001e
	.4byte SceneActor_ApplyPointLeftOfActorZero
	.4byte 0x00000202
	.4byte 0xffff001e
	.4byte FieldScene_RunThreeStepSequence
	.4byte 0x00000602
	.4byte 0xffff001d
	.4byte FieldScene_RunFourCallSequence
	.4byte 0x00004602
	.4byte 0xffff001d
	.4byte SceneActor_PassRaisedPointOfActorZero
	.4byte 0x0000c602
	.4byte 0xffff001d
	.4byte SceneActor_PassActorZeroOffsetPoint
	.4byte 0x00000202
	.4byte 0xffff0029
	.4byte FieldScene_RunLayoutAt83By45
	.4byte 0x00000003
	.4byte 0xffff0014
	.4byte FieldScene_RunScriptedStep953
	.4byte 0x00000003
	.4byte 0xffff002a
	.4byte SceneState_SetValue268bInScene
	.4byte 0x00000013
	.4byte 0x0f320064
	.4byte 0x00100071
	.4byte 0x00000013
	.4byte 0x0f330065
	.4byte 0x00100054
	.4byte 0x00008c15
	.4byte 0x02040008
	.4byte FieldScene_RunTwoStepSequence
	.4byte 0x10002115
	.4byte 0x02030008
	.4byte SceneActor_InstallSlotNineHandler
	.4byte 0x00001815
	.4byte 0x02040009
	.4byte SceneActor_SetupSlotNineAndInstallHandler
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte 0x00000000
	.4byte 0x00009315
	.4byte 0xffff000c
	.4byte FieldScene_RunMiddleSequence
	.4byte 0x00009315
	.4byte 0xffff000d
	.4byte FieldScene_RunMiddleSequence
	.4byte 0x00009315
	.4byte 0xffff000e
	.4byte FieldScene_RunMiddleSequence
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte FieldScene_RunLateSequenceSecond
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte FieldScene_RunLateSequenceHead
	.4byte 0x00008c15
	.4byte 0xffff0011
	.4byte SceneState_SetSlot17And18Selectors
	.4byte 0x00008c15
	.4byte 0xffff0012
	.4byte SceneState_SetSlot17And18Selectors
	.4byte 0x00008c15
	.4byte 0xffff0013
	.4byte FieldScene_RunScene3c4SequenceA
	.4byte 0x00008c15
	.4byte 0xffff0014
	.4byte FieldScene_RunScene3c4SequenceA
	.4byte 0x00008c15
	.4byte 0xffff0015
	.4byte FieldScene_RunScene3c4SequenceA
	.4byte 0x00008c15
	.4byte 0xffff0016
	.4byte FieldScene_RunScene3c4SequenceA
	.4byte 0x00008c15
	.4byte 0xffff0017
	.4byte FieldScene_RunScene3c4SequenceA
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x0000268c
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x0000268d
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x0000268e
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x0000268f
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002690
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
