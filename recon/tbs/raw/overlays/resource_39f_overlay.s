.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {r5, lr}
	ldmia r0!, {r5}
	ldmia r1!, {r3}
	ldmia r0!, {r4}
	subs r5, r5, r3
	ldmia r1!, {r3}
	ldr r2, [r1]
	subs r4, r4, r3
	ldr r3, [r0]
	subs r3, r3, r2
	asrs r5, r5, #16
	asrs r4, r4, #16
	asrs r3, r3, #16
	adds r0, r5, #0
	muls r0, r5
	adds r2, r4, #0
	muls r2, r4
	adds r1, r3, #0
	muls r1, r3
	adds r0, r0, r2
	adds r3, r1, #0
	adds r0, r0, r3
	ldr r3, [pc, #8]
	bl 0x0200aec8
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x030001d8
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
	bl 0x0200adbc
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
	bl 0x0200ad54
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
	bl 0x0200ad24
	ldr r5, [pc, #132]
	movs r0, #15
	bl 0x0200acec
	movs r0, #185
	bl 0x0200aeb4
	str r5, [r6, #48]
	str r5, [r6, #52]
	ldr r1, [r7]
.L_020000c4_5:
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl 0x0200ad3c
	mov r0, r8
	str r5, [r0, #48]
	str r5, [r0, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl 0x0200ad3c
	adds r0, r6, #0
	bl 0x0200ad44
	bl 0x0200aeac
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
	bl 0x0200ad24
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
	.4byte 0x0200aef8
	.4byte 0xffff0000
	.4byte 0x00003333
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
	bl 0x0200ad54
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
	.4byte 0x0200aef8
	.4byte 0xffff0000
	.4byte 0x0200af38
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
	bl 0x0200adbc
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
	.4byte 0x0200af38
	.4byte 0x0200af50
	.4byte 0x0200aef8
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
	bl 0x0200ad54
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
	.4byte 0x0200af50
	.4byte 0x0200aef8
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
	bl 0x0200adbc
	ldrh r3, [r0, #6]
	ldr r0, [sp, #76]
	lsrs r3, r3, #12
	mov r8, r3
	bl 0x0200adbc
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
	bl 0x0200adc4
	movs r1, #8
	movs r0, #0
	bl 0x0200adfc
	movs r0, #15
	bl 0x0200ad9c
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
	bl 0x0200ade4
	movs r0, #0
	bl 0x0200adbc
	ldr r3, [pc, #424]
	str r3, [r0, #108]
	movs r0, #4
	bl 0x0200ad9c
	mov r3, r8
	subs r3, #6
	cmp r3, #7
	bhi .L_02000608_6
	adds r0, r7, #0
	movs r1, #3
	bl 0x0200ad24
	b .L_02000608_7
.L_02000608_6:
	adds r0, r7, #0
	movs r1, #2
	bl 0x0200ad24
.L_02000608_7:
	movs r0, #239
	bl 0x0200aeb4
	adds r0, r7, #0
	ldr r1, [sp, #80]
	ldr r2, [sp, #84]
	ldr r3, [sp, #88]
	bl 0x0200ad3c
	movs r0, #0
	bl 0x0200adec
	movs r0, #0
	movs r1, #2
	bl 0x0200adfc
	movs r0, #0
	ldr r1, [pc, #360]
	ldr r2, [pc, #344]
	bl 0x0200adc4
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
	bl 0x0200ade4
	ldr r3, [sp, #92]
	cmp r3, #0
	beq .L_02000608_8
	bl 0x0200aec8
.L_02000608_8:
	movs r0, #0
	bl 0x0200adec
	movs r1, #1
	movs r0, #0
	bl 0x0200adfc
	movs r0, #0
	bl 0x0200adbc
	movs r2, #0
	str r2, [r0, #108]
	adds r0, r7, #0
	bl 0x0200ad44
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200aeb4
	movs r0, #213
	bl 0x0200aeb4
	ldr r3, [sp, #80]
	str r3, [r7, #8]
	ldr r3, [sp, #88]
	str r3, [r7, #16]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	adds r0, r7, #0
	movs r1, #1
	bl 0x0200ad24
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
	bl 0x0200ad4c
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
	bl 0x0200ad4c
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
	bl 0x0200aeac
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
	.2byte 0xaf50
	.2byte 0x0200
	.2byte 0x1999
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0x0001
	.2byte 0x82a9
	.2byte 0x0200
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0xaef8
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
	bl 0x0200adbc
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
	bl 0x0200ad4c
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
	.2byte 0xaf38
	.2byte 0x0200
	.4byte 0x0200af50
	.global Func_020009dc
	.thumb_func
Func_020009dc:
	ldr r0, [r0, #80]
	movs r3, #3
	ldrb r2, [r0, #9]
	ands r1, r3
	movs r3, #13
	negs r3, r3
	lsls r1, r1, #2
	ands r3, r2
	orrs r3, r1
	strb r3, [r0, #9]
	bx lr
	.2byte 0x0000
	.global Func_020009f4
	.thumb_func
Func_020009f4:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl 0x0200ad34
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020009f4_0
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	adds r2, r5, #0
	strb r3, [r1, #9]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
	movs r3, #8
	strb r3, [r2]
	movs r1, #0
	bl 0x0200ad5c
	adds r0, r5, #0
	movs r1, #14
	bl 0x0200ae2c
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200ad64
	adds r0, r5, #0
	b .L_020009f4_1
.L_020009f4_0:
	movs r0, #0
.L_020009f4_1:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02000a4c
	.thumb_func
Func_02000a4c:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl 0x0200ad34
	adds r5, r0, #0
	cmp r5, #0
	beq 0x02008aa6
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
.L_02000a6e:
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	adds r2, r5, #0
	strb r3, [r1, #9]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
.L_02000a80:
	movs r3, #8
	strb r3, [r2]
	movs r1, #0
	bl 0x0200ad5c
	adds r0, r5, #0
	movs r1, #15
	bl 0x0200ae2c
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #2
	orrs r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	b .L_02000a80_0
	.2byte 0x2000
.L_02000a80_0:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02000ab0
	.thumb_func
Func_02000ab0:
	ldr r3, [r0, #8]
	ldr r2, [r0, #68]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, [r0, #72]
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r2, [r0, #76]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
	ldr r2, [r0, #48]
	ldr r3, [r0, #24]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [r0, #52]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	ldr r1, [r0, #80]
	adds r0, #100
	ldrh r3, [r1, #30]
	ldrh r2, [r0]
	adds r3, r3, r2
	strh r3, [r1, #30]
	bx lr
	.2byte 0x0000
	.global Func_02000ae8
	.thumb_func
Func_02000ae8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r6, r1, #0
	ldr r1, [sp, #48]
	adds r5, r0, #0
	movs r0, #0
	mov r8, r2
	str r3, [sp, #4]
	mov r10, r1
	ldr r7, [sp, #52]
	bl 0x0200adbc
	movs r3, #128
	lsls r3, r3, #13
	mov r2, r10
	ands r3, r2
	mov r9, r0
	cmp r3, #0
	beq .L_02000ae8_0
	cmp r7, #0
	beq .L_02000ae8_0
	movs r3, #24
	ldrsh r0, [r7, r3]
	adds r2, r6, #0
	b .L_02000ae8_1
.L_02000ae8_0:
	adds r2, r6, #0
	movs r0, #222
.L_02000ae8_1:
	adds r1, r5, #0
	mov r3, r8
	bl 0x0200ad34
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02000ae8_2
	b 0x02008ca2
.L_02000ae8_2:
	ldr r1, [r6, #80]
	mov r8, r1
	mov r1, r10
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	bl 0x0200ad24
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl 0x0200ad2c
	adds r3, r6, #0
	movs r0, #0
	adds r3, #85
	strb r0, [r3]
	mov r3, r8
	adds r3, #38
	strb r0, [r3]
	ldr r3, [pc, #328]
	str r3, [r6, #108]
	ldr r3, [sp, #4]
	str r3, [r6, #68]
	ldr r3, [sp, #40]
	str r3, [r6, #72]
	ldr r3, [sp, #44]
	mov r1, r9
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	mov r3, r8
	ldrb r1, [r3, #9]
	movs r3, #13
	negs r3, r3
	mov r9, r3
	ands r3, r1
	orrs r3, r2
	adds r2, r6, #0
	mov r1, r8
	adds r2, #100
	strb r3, [r1, #9]
	adds r3, r2, #0
	str r0, [r6, #48]
	str r0, [r6, #52]
	str r2, [sp, #0]
	strh r0, [r3]
	ldr r3, [pc, #276]
	mov r1, r10
	ands r3, r1
	movs r5, #3
	cmp r3, #0
	beq 0x02008ca2
	cmp r7, #0
	beq 0x02008ca2
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_02000ae8_3
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl 0x0200ae2c
.L_02000ae8_3:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq 0x02008bf4
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	mov r3, r8
	ldrb r2, [r7]
	ldrb r1, [r3, #9]
	ands r2, r5
	mov r3, r9
	ands r3, r1
	lsls r2, r2, #2
	orrs r3, r2
.L_02000bf0:
	mov r1, r8
	strb r3, [r1, #9]
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r10
	ands r2, r3
	cmp r2, #0
	beq 0x02008c08
	ldr r3, [r7, #8]
.L_02000c02:
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_02000c02_0
	ldr r3, [pc, #156]
	mov r1, r11
	ldr r5, [r3, r1]
	cmp r2, #0
	beq .L_02000c02_1
	ldr r0, [r7, #16]
	ldr r3, [r6, #24]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	bl 0x0200ace4
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_02000c02_2
.L_02000c02_1:
	ldr r0, [r7, #16]
	ldr r2, [pc, #128]
	ldr r1, [r5, #12]
	adds r0, r0, r2
	bl 0x0200ace4
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_02000c02_2:
	bl 0x0200ace4
	str r0, [r6, #52]
.L_02000c02_0:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_02000c02_3
	adds r0, r6, #0
	movs r1, #1
	bl 0x0200ad24
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl 0x0200ad2c
.L_02000c02_3:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02000c02_4
	ldrh r3, [r7, #32]
	mov r1, r8
	strh r3, [r1, #30]
.L_02000c02_4:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02000c02_5
	ldrh r3, [r7, #34]
	ldr r1, [sp, #0]
	strh r3, [r1]
.L_02000c02_5:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02000c02_6
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_02000c02_6:
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200b058
	.2byte 0x8ab1
	.2byte 0x0200
	.4byte 0xffff0000
	.global Func_02000cc0
	.thumb_func
Func_02000cc0:
	push {lr}
	movs r0, #18
	movs r1, #2
	bl 0x0200ae84
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000cd0
	.thumb_func
Func_02000cd0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r0, #0
	bl 0x0200adbc
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #85
	ldrb r3, [r6]
	adds r1, r7, #0
	mov r8, r3
	bl 0x0200ad54
	cmp r0, #0
	bne .L_02000cd0_0
	bl 0x0200ada4
	movs r1, #6
	adds r0, r5, #0
	bl 0x0200ad24
	movs r0, #6
	bl 0x0200acec
	movs r0, #152
	bl 0x0200aeb4
	adds r0, r5, #0
	movs r1, #7
	bl 0x0200ad24
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldrb r2, [r6]
	movs r3, #126
	ands r3, r2
	strb r3, [r6]
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200ad5c
	movs r3, #10
	ldrsh r2, [r7, r3]
	movs r3, #2
	ldrsh r1, [r7, r3]
	movs r0, #0
	bl 0x0200add4
	adds r0, r5, #0
	movs r1, #6
	bl 0x0200ad24
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200ad5c
	mov r3, r8
	strb r3, [r6]
	bl 0x0200adac
	movs r0, #1
	b .L_02000cd0_1
.L_02000cd0_0:
	movs r0, #0
.L_02000cd0_1:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02000d6c
	.thumb_func
Func_02000d6c:
	push {lr}
	adds r3, r0, #0
	adds r3, #100
	movs r2, #0
	ldrsh r1, [r3, r2]
	bl 0x0200ae2c
	movs r0, #0
	pop {r1}
	bx r1
	.global Func_02000d80
	.thumb_func
Func_02000d80:
	push {lr}
	movs r1, #0
	bl 0x0200ad5c
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02000d90
	.thumb_func
Func_02000d90:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	adds r6, r0, #0
	mov r10, r2
	mov r9, r3
	mov r8, r1
	bl 0x0200adbc
	movs r1, #1
	adds r5, r0, #0
	adds r0, r6, #0
	bl 0x0200ae3c
	movs r1, #192
	movs r2, #192
	lsls r2, r2, #9
	adds r0, r6, #0
	lsls r1, r1, #10
	bl 0x0200adc4
	movs r0, #152
	bl 0x0200aeb4
	mov r3, r9
	str r3, [r5, #40]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #72]
	movs r3, #0
	str r3, [r5, #68]
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200ad5c
	adds r0, r6, #0
	mov r1, r8
	mov r2, r10
	bl 0x0200add4
	mov r3, r8
	lsls r3, r3, #16
	mov r8, r3
	mov r3, r10
	lsls r3, r3, #16
	mov r10, r3
	adds r0, r6, #0
	mov r1, r8
	mov r2, r10
	bl 0x0200adf4
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200ad5c
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #72]
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02000e18
	.thumb_func
Func_02000e18:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #68
	bl 0x0200adbc
	mov r10, r0
	movs r0, #188
	bl 0x0200aeb4
	add r1, sp, #16
	movs r3, #1
	movs r2, #0
	str r3, [r1]
	mov r9, r1
	mov r8, r2
	add r7, sp, #56
.L_02000e18_0:
	mov r3, r8
	lsls r5, r3, #12
	adds r0, r5, #0
	bl 0x0200ad04
	movs r3, #0
	str r3, [r7, #4]
	str r0, [r7]
	adds r0, r5, #0
	bl 0x0200acfc
	ldr r5, [r7]
	adds r6, r0, #0
	movs r1, #3
	adds r0, r5, #0
	str r6, [r7, #8]
	bl 0x0200ace4
	adds r5, r5, r0
	str r5, [r7]
	mov r1, r10
	ldr r2, [r1, #16]
	ldr r0, [r1, #8]
	ldr r3, [r7, #4]
	ldr r1, [pc, #52]
	adds r3, r3, r1
	str r3, [sp, #0]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [sp, #8]
	movs r1, #128
	mov r3, r9
	str r3, [sp, #12]
	lsls r1, r1, #13
	adds r3, r5, #0
	str r6, [sp, #4]
	bl 0x02008ae8
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #16
	bls .L_02000e18_0
	sub sp, #-68
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00001999
	.global Func_02000ea8
	.thumb_func
Func_02000ea8:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200ae5c
	movs r1, #1
	adds r0, r5, #0
	bl 0x0200ae74
	bl 0x0200ae6c
	movs r0, #30
	bl 0x0200ad9c
	adds r0, r5, #0
	bl 0x02008e18
	adds r0, r5, #0
	adds r1, r6, #0
	bl 0x0200ae24
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02000ee0
	.thumb_func
Func_02000ee0:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000ee0_0
	ldr r0, [pc, #36]
	b .L_02000ee0_1
.L_02000ee0_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000ee0_2
	ldr r0, [pc, #36]
	b .L_02000ee0_1
.L_02000ee0_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000ee0_3
	ldr r0, [pc, #32]
	b .L_02000ee0_1
.L_02000ee0_3:
	ldr r0, [pc, #32]
.L_02000ee0_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000044
	.4byte 0x0200b0f4
	.4byte 0x00000045
	.4byte 0x0200b1e4
	.4byte 0x00000046
	.4byte 0x0200b334
	.4byte 0x0200b4b4
	.global Func_02000f34
	.thumb_func
Func_02000f34:
	movs r0, #0
	bx lr
	.global Func_02000f38
	.thumb_func
Func_02000f38:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200b5bc
	.global Func_02000f40
	.thumb_func
Func_02000f40:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000f40_0
	ldr r0, [pc, #36]
	b .L_02000f40_1
.L_02000f40_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000f40_2
	ldr r0, [pc, #36]
	b .L_02000f40_1
.L_02000f40_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000f40_3
	ldr r0, [pc, #32]
	b .L_02000f40_1
.L_02000f40_3:
	ldr r0, [pc, #32]
.L_02000f40_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000044
	.4byte 0x0200b6a0
	.4byte 0x00000045
	.4byte 0x0200b790
	.4byte 0x00000046
	.4byte 0x0200b8b0
	.4byte 0x0200ba30
	.global Func_02000f94
	.thumb_func
Func_02000f94:
	push {r5, lr}
	sub sp, #32
	bl 0x0200ada4
	add r5, sp, #8
	adds r0, r5, #0
	bl 0x02008474
	cmp r0, #0
	beq .L_02000f94_0
	mov r3, sp
	add r2, sp, #24
	ldmia r2!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r3, [r5, #12]
	ldr r0, [r5]
	ldr r1, [r5, #4]
	ldr r2, [r5, #8]
	bl 0x02008608
	ldr r3, [r5, #4]
	cmp r3, #9
	bne .L_02000f94_0
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #26
	bne .L_02000f94_0
	movs r0, #196
	lsls r0, r0, #2
	bl 0x0200ad94
	movs r0, #9
	movs r1, #3
	bl 0x0200adfc
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #7
	lsls r2, r2, #8
	bl 0x0200adc4
	movs r2, #16
	negs r2, r2
	movs r1, #0
	movs r0, #9
	bl 0x0200ade4
	movs r0, #45
	bl 0x0200ad9c
	movs r1, #8
	movs r0, #9
	bl 0x0200adfc
	movs r0, #240
	bl 0x0200aeb4
	movs r1, #1
	movs r0, #9
	bl 0x0200ae3c
	movs r0, #9
	bl 0x0200adbc
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	movs r2, #25
	movs r3, #31
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #27
	movs r2, #4
	movs r3, #2
	bl 0x0200ad4c
.L_02000f94_0:
	bl 0x0200adac
	sub sp, #-32
	pop {r5}
	pop {r0}
	bx r0
	.global Func_0200103c
	.thumb_func
Func_0200103c:
	push {lr}
	movs r0, #0
	sub sp, #12
	bl 0x0200adbc
	ldr r1, [pc, #40]
	ldr r3, [r0, #8]
	movs r4, #128
	lsls r4, r4, #12
	ands r3, r1
	mov r2, sp
	adds r3, r3, r4
	str r3, [r2]
	ldr r3, [r0, #12]
	str r3, [r2, #4]
	ldr r3, [r0, #16]
	ands r3, r1
	ldr r1, [pc, #20]
	adds r3, r3, r1
	str r3, [r2, #8]
	adds r0, r2, #0
	bl 0x02008cd0
	sub sp, #-12
	pop {r0}
	bx r0
	.4byte 0xfff00000
	.4byte 0xffe80000
	.global Func_02001078
	.thumb_func
Func_02001078:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #0
	sub sp, #12
	bl 0x0200adbc
	adds r6, r0, #0
	adds r7, r6, #0
	adds r7, #85
	ldrb r1, [r7]
	ldr r3, [r6, #8]
	ldr r2, [pc, #144]
	mov r8, r1
	movs r1, #128
	ands r3, r2
	lsls r1, r1, #12
	mov r0, sp
	adds r3, r3, r1
	str r3, [r0]
	ldr r3, [r6, #12]
	str r3, [r0, #4]
	ldr r3, [r6, #16]
	ands r3, r2
	movs r2, #160
	lsls r2, r2, #14
	adds r3, r3, r2
	str r3, [r0, #8]
	bl 0x02008cd0
	cmp r0, #0
	beq .L_02001078_0
	bl 0x0200ada4
	movs r3, #0
	movs r1, #7
	strb r3, [r7]
	movs r0, #9
	bl 0x0200adfc
	ldr r5, [pc, #92]
	ldr r3, [r6, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r6, #20]
	adds r3, r3, r5
	str r3, [r6, #20]
	movs r0, #2
	bl 0x0200acec
	ldr r3, [r6, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r6, #20]
	adds r3, r3, r5
	str r3, [r6, #20]
	movs r0, #10
	bl 0x0200acec
	movs r5, #128
	ldr r3, [r6, #12]
	lsls r5, r5, #9
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r6, #20]
	adds r3, r3, r5
	str r3, [r6, #20]
	movs r0, #4
	bl 0x0200acec
	ldr r3, [r6, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r6, #20]
	adds r3, r3, r5
	str r3, [r6, #20]
	mov r3, r8
	strb r3, [r7]
	bl 0x0200adac
.L_02001078_0:
	sub sp, #-12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0xfff00000
	.4byte 0xffff0000
	.global Func_0200112c
	.thumb_func
Func_0200112c:
	push {lr}
	bl 0x0200ada4
	movs r0, #0
	movs r1, #1
	bl 0x0200adfc
	ldr r0, [pc, #16]
	movs r1, #1
	bl 0x0200ad7c
	bl 0x0200adac
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000017e6
	.global Func_02001150
	.thumb_func
Func_02001150:
	push {r5, lr}
	movs r0, #10
	sub sp, #16
	bl 0x0200adbc
	adds r5, r0, #0
	bl 0x0200ada4
	movs r0, #10
	movs r1, #1
	bl 0x02008ea8
	movs r3, #192
	lsls r3, r3, #11
	movs r0, #10
	movs r1, #88
	movs r2, #120
	bl 0x02008d90
	ldr r2, [r5, #16]
	movs r3, #192
	lsls r3, r3, #13
	adds r2, r2, r3
	movs r3, #1
	movs r4, #0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	str r3, [sp, #8]
	movs r3, #0
	str r4, [sp, #0]
	str r4, [sp, #4]
	str r4, [sp, #12]
	bl 0x02008ae8
	movs r0, #10
	movs r1, #1
	bl 0x0200ae54
	movs r2, #0
	movs r1, #0
	movs r0, #10
	bl 0x0200ae1c
	movs r0, #20
	bl 0x0200ad9c
	movs r0, #10
	movs r1, #2
	bl 0x0200ae04
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	movs r5, #192
	bl 0x0200ae4c
	lsls r5, r5, #10
	movs r0, #60
	bl 0x0200ad9c
	adds r3, r5, #0
	movs r0, #10
	movs r1, #88
	movs r2, #152
	bl 0x02008d90
	movs r1, #10
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #10
	bl 0x0200ad9c
	adds r3, r5, #0
	movs r0, #10
	movs r1, #120
	movs r2, #192
	bl 0x02008d90
	movs r1, #10
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #10
	bl 0x0200ad9c
	adds r3, r5, #0
	movs r0, #10
	movs r1, #120
	movs r2, #240
	bl 0x02008d90
	movs r1, #10
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #10
	bl 0x0200ad9c
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200ad94
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x0200adf4
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200adf4
	bl 0x0200adac
	sub sp, #-16
	pop {r5}
	pop {r0}
	bx r0
	.global Func_02001244
	.thumb_func
Func_02001244:
	push {r5, lr}
	movs r0, #11
	sub sp, #16
	bl 0x0200adbc
	adds r5, r0, #0
	bl 0x0200ada4
	movs r0, #11
	movs r1, #0
	bl 0x02008ea8
	movs r1, #204
	movs r2, #228
	movs r3, #192
	lsls r1, r1, #1
	lsls r2, r2, #1
	lsls r3, r3, #11
	movs r0, #11
	bl 0x02008d90
	ldr r2, [r5, #16]
	movs r3, #192
	lsls r3, r3, #13
	adds r2, r2, r3
	movs r3, #1
	movs r4, #0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	str r3, [sp, #8]
	movs r3, #0
	str r4, [sp, #0]
	str r4, [sp, #4]
	str r4, [sp, #12]
	bl 0x02008ae8
	movs r0, #11
	movs r1, #1
	bl 0x0200ae54
	movs r2, #0
	movs r1, #0
	movs r0, #11
	bl 0x0200ae1c
	movs r0, #30
	bl 0x0200ad9c
	movs r0, #11
	movs r1, #2
	bl 0x0200ae04
	movs r2, #0
	ldr r1, [pc, #104]
	movs r0, #11
	bl 0x0200ae44
	movs r0, #147
	bl 0x0200aeb4
	movs r0, #60
	bl 0x0200ad9c
	movs r0, #0
	bl 0x0200adbc
	movs r2, #10
	ldrsh r5, [r0, r2]
	movs r0, #0
	bl 0x0200adbc
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r3, #128
	lsls r3, r3, #11
	adds r1, r5, #0
	movs r0, #11
	bl 0x02008d90
	movs r0, #10
	bl 0x0200ad9c
	ldr r0, [pc, #48]
	bl 0x0200ad94
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl 0x0200adf4
	ldr r3, [pc, #36]
	ldr r2, [pc, #40]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	movs r0, #53
	movs r1, #0
	bl 0x0200ae7c
	bl 0x0200adac
	sub sp, #-16
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000103
	.4byte 0x00000301
	.4byte 0x02000240
	.4byte 0x0000022b
	.global Func_02001328
	.thumb_func
Func_02001328:
	push {r5, r6, lr}
	movs r0, #12
	sub sp, #16
	bl 0x0200adbc
	movs r6, #172
	adds r5, r0, #0
	bl 0x0200ada4
	movs r0, #12
	movs r1, #1
	bl 0x02008ea8
	lsls r6, r6, #1
	movs r1, #134
	movs r3, #224
	adds r2, r6, #0
	lsls r1, r1, #2
	lsls r3, r3, #11
	movs r0, #12
	bl 0x02008d90
	ldr r2, [r5, #16]
	movs r3, #128
	lsls r3, r3, #13
	adds r2, r2, r3
	movs r3, #1
	movs r4, #0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	str r3, [sp, #8]
	movs r3, #0
	str r4, [sp, #0]
	str r4, [sp, #4]
	str r4, [sp, #12]
	bl 0x02008ae8
	movs r0, #12
	movs r1, #1
	bl 0x0200ae54
	movs r2, #0
	movs r1, #0
	movs r0, #12
	bl 0x0200ae1c
	movs r0, #20
	bl 0x0200ad9c
	movs r0, #12
	movs r1, #2
	bl 0x0200ae04
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #12
	bl 0x0200ae4c
	movs r5, #192
	movs r0, #60
	bl 0x0200ad9c
	lsls r5, r5, #10
	movs r1, #146
	adds r3, r5, #0
	adds r2, r6, #0
	lsls r1, r1, #2
	movs r0, #12
	bl 0x02008d90
	movs r1, #12
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	movs r1, #158
	adds r3, r5, #0
	adds r2, r6, #0
	lsls r1, r1, #2
	movs r0, #12
	bl 0x02008d90
	movs r1, #12
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	movs r1, #170
	adds r3, r5, #0
	adds r2, r6, #0
	lsls r1, r1, #2
	movs r0, #12
	bl 0x02008d90
	movs r1, #12
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	ldr r0, [pc, #24]
	bl 0x0200ad94
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl 0x0200adf4
	bl 0x0200adac
	sub sp, #-16
.L_02001416:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0302
	.2byte 0x0000
	.global Func_02001420
	.thumb_func
Func_02001420:
	push {lr}
	bl 0x0200ada4
	movs r2, #0
	movs r1, #0
	movs r0, #16
	bl 0x0200adf4
	ldr r0, [pc, #28]
	bl 0x0200ad94
	movs r0, #181
	movs r1, #3
	bl 0x0200aea4
	movs r1, #0
	movs r0, #181
	bl 0x0200adb4
	bl 0x0200adac
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000fd4
	.global Func_02001454
	.thumb_func
Func_02001454:
	push {r5, lr}
	sub sp, #32
	bl 0x0200ada4
	add r5, sp, #8
	adds r0, r5, #0
	bl 0x02008474
	cmp r0, #0
	beq .L_02001454_0
	mov r3, sp
	add r2, sp, #24
	ldmia r2!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r3, [r5, #12]
	ldr r0, [r5]
	ldr r1, [r5, #4]
	ldr r2, [r5, #8]
	bl 0x02008608
	ldr r3, [r5, #4]
	cmp r3, #8
	bne .L_02001454_1
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #23
	bne .L_02001454_1
	movs r3, #35
	movs r2, #68
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #35
	movs r1, #67
	movs r2, #4
	movs r3, #1
	bl 0x0200ad4c
	b .L_02001454_0
.L_02001454_1:
	ldr r3, [r5, #4]
	cmp r3, #10
	bne .L_02001454_0
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #35
	bne .L_02001454_0
	ldr r0, [pc, #108]
	bl 0x0200ad94
	movs r0, #10
	movs r1, #3
	bl 0x0200adfc
	movs r1, #16
	movs r2, #6
	negs r1, r1
	movs r0, #10
	bl 0x0200ade4
	movs r0, #30
	bl 0x0200ad9c
	movs r1, #8
	movs r0, #10
	bl 0x0200adfc
	movs r0, #240
	bl 0x0200aeb4
	movs r0, #10
	bl 0x0200adbc
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	movs r2, #30
	movs r3, #34
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #44
	movs r1, #30
	movs r2, #2
	movs r3, #4
	bl 0x0200ad4c
	movs r3, #4
	movs r5, #0
	str r3, [sp, #0]
	movs r0, #2
	movs r1, #35
	movs r2, #30
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02008244
.L_02001454_0:
	bl 0x0200adac
	sub sp, #-32
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000311
	.global Func_02001520
	.thumb_func
Func_02001520:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	bl 0x0200ada4
	movs r0, #11
	bl 0x0200adbc
	ldr r5, [r0, #8]
	movs r0, #11
	bl 0x0200adbc
	ldr r3, [r0, #16]
	asrs r5, r5, #20
	asrs r7, r3, #20
	movs r3, #255
	movs r6, #1
	str r3, [sp, #4]
	adds r1, r5, #0
	adds r2, r7, #0
	movs r3, #1
	movs r0, #2
	str r6, [sp, #0]
	bl 0x02008244
	movs r2, #0
	mov r8, r2
	str r2, [sp, #4]
	adds r1, r5, #1
	adds r2, r7, #0
	movs r3, #1
	movs r0, #2
	str r6, [sp, #0]
	bl 0x02008244
	mov r3, r8
	str r3, [sp, #4]
	subs r1, r5, #1
	adds r2, r7, #0
	movs r3, #1
	movs r0, #2
	str r6, [sp, #0]
	bl 0x02008244
	mov r3, r8
	adds r2, r7, #1
	str r3, [sp, #4]
	adds r1, r5, #0
	movs r3, #1
	movs r0, #2
	str r6, [sp, #0]
	bl 0x02008244
	mov r3, r8
	str r3, [sp, #4]
	subs r2, r7, #1
	movs r0, #2
	adds r1, r5, #0
	movs r3, #1
	str r6, [sp, #0]
	bl 0x02008244
	cmp r5, #36
	bne .L_02001520_0
	cmp r7, #24
	bne .L_02001520_0
	movs r0, #11
	bl 0x0200adbc
	adds r3, r0, #0
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	ldr r3, [pc, #20]
	str r3, [r0, #20]
	str r3, [r0, #12]
.L_02001520_0:
	bl 0x0200adac
	sub sp, #-8
	pop {r3}
.L_020015c2:
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfffe
	.global Func_020015d0
	.thumb_func
Func_020015d0:
	push {r5, r6, lr}
	movs r0, #12
	sub sp, #16
	bl 0x0200adbc
	adds r5, r0, #0
	bl 0x0200ada4
	movs r0, #12
	movs r1, #1
	bl 0x02008ea8
	movs r1, #196
	movs r3, #224
	lsls r1, r1, #1
	lsls r3, r3, #11
	movs r2, #104
	movs r0, #12
	bl 0x02008d90
	movs r0, #10
	bl 0x0200ad9c
	ldr r2, [r5, #16]
	movs r3, #128
	lsls r3, r3, #11
	adds r2, r2, r3
	movs r3, #1
	movs r4, #0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	str r3, [sp, #8]
	movs r3, #0
	str r4, [sp, #0]
	str r4, [sp, #4]
	str r4, [sp, #12]
	bl 0x02008ae8
	movs r0, #12
	movs r1, #1
	bl 0x0200ae54
	movs r2, #0
	movs r1, #0
	movs r0, #12
	bl 0x0200ae1c
	movs r0, #20
	bl 0x0200ad9c
	movs r0, #12
	movs r1, #2
	bl 0x0200ae04
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #12
	movs r5, #212
	movs r6, #192
	bl 0x0200ae4c
	lsls r5, r5, #1
	lsls r6, r6, #10
	movs r0, #60
	bl 0x0200ad9c
	adds r3, r6, #0
	adds r1, r5, #0
	movs r0, #12
	movs r2, #120
	bl 0x02008d90
	movs r1, #12
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	adds r3, r6, #0
	adds r1, r5, #0
	movs r0, #12
	movs r2, #168
	bl 0x02008d90
	movs r1, #12
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	adds r3, r6, #0
	adds r1, r5, #0
	movs r0, #12
	movs r2, #208
	bl 0x02008d90
	movs r1, #12
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	adds r3, r6, #0
	adds r1, r5, #0
	movs r0, #12
	movs r2, #232
	bl 0x02008d90
	movs r1, #12
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl 0x0200adf4
	ldr r0, [pc, #28]
	bl 0x0200ad94
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl 0x0200adf4
	bl 0x0200adac
	sub sp, #-16
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000303
	.global Func_020016f0
	.thumb_func
Func_020016f0:
	push {r5, lr}
	movs r0, #13
	sub sp, #16
	bl 0x0200adbc
	adds r5, r0, #0
	bl 0x0200ada4
	movs r0, #13
	movs r1, #1
	bl 0x02008ea8
	movs r1, #228
	movs r3, #224
	lsls r1, r1, #1
	lsls r3, r3, #11
	movs r2, #104
	movs r0, #13
	bl 0x02008d90
	movs r0, #10
	bl 0x0200ad9c
	ldr r2, [r5, #16]
	movs r3, #128
	lsls r3, r3, #11
	adds r2, r2, r3
	movs r3, #1
	movs r4, #0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	str r3, [sp, #8]
	movs r3, #0
	str r4, [sp, #0]
	str r4, [sp, #4]
	str r4, [sp, #12]
	bl 0x02008ae8
	movs r0, #13
	movs r1, #1
	bl 0x0200ae54
	movs r2, #0
	movs r1, #0
	movs r0, #13
	bl 0x0200ae1c
	movs r0, #20
	bl 0x0200ad9c
	movs r0, #13
	movs r1, #2
	bl 0x0200ae04
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #13
	bl 0x0200ae4c
	movs r0, #60
	bl 0x0200ad9c
	movs r1, #236
	movs r3, #192
	lsls r3, r3, #10
	lsls r1, r1, #1
	movs r0, #13
	movs r2, #136
	bl 0x02008d90
	movs r1, #13
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	movs r1, #252
	ldr r3, [pc, #132]
	lsls r1, r1, #1
	movs r0, #13
	movs r2, #136
.L_02001796:
	bl 0x02008d90
	movs r1, #13
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r5, #224
	movs r0, #6
	bl 0x0200ad9c
	lsls r5, r5, #10
	movs r1, #138
	adds r3, r5, #0
	lsls r1, r1, #2
	movs r0, #13
	movs r2, #136
	bl 0x02008d90
.L_020017bc:
	movs r1, #13
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	movs r1, #146
.L_020017ce:
	adds r3, r5, #0
	lsls r1, r1, #2
	movs r0, #13
	movs r2, #136
	bl 0x02008d90
	movs r1, #13
	movs r2, #0
	movs r0, #0
.L_020017e0:
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	movs r1, #0
	movs r2, #0
	movs r0, #13
	bl 0x0200adf4
	movs r0, #193
	lsls r0, r0, #2
	bl 0x0200ad94
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl 0x0200adf4
	bl 0x0200adac
	sub sp, #-16
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x3333
	.2byte 0x0003
	.global Func_02001818
	.thumb_func
Func_02001818:
	push {lr}
	bl 0x0200ada4
	movs r0, #14
	movs r1, #1
	bl 0x02008ea8
	movs r1, #212
	movs r2, #240
	ldr r3, [pc, #76]
	lsls r2, r2, #1
	lsls r1, r1, #1
	movs r0, #14
	bl 0x02008d90
	movs r0, #2
	bl 0x0200ad9c
	movs r0, #14
	bl 0x02008e18
	movs r1, #15
	movs r0, #14
	bl 0x0200ae24
	movs r0, #14
	bl 0x0200adbc
	movs r1, #0
	bl 0x0200ad5c
	movs r0, #30
	bl 0x0200ad9c
	ldr r0, [pc, #28]
	bl 0x0200ad94
	movs r1, #212
	movs r2, #240
	movs r0, #17
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200adf4
	bl 0x0200adac
	pop {r0}
	bx r0
	.4byte 0x00079999
	.4byte 0x00000305
	.global Func_02001880
	.thumb_func
Func_02001880:
	push {r5, r6, lr}
	movs r0, #14
	sub sp, #16
	bl 0x0200adbc
	adds r5, r0, #0
	bl 0x0200ada4
	movs r0, #14
	movs r1, #1
	bl 0x02008ea8
	movs r1, #196
	movs r2, #252
	movs r3, #192
	lsls r1, r1, #1
	lsls r2, r2, #1
	lsls r3, r3, #11
	movs r0, #14
	bl 0x02008d90
	movs r0, #10
	bl 0x0200ad9c
	ldr r2, [r5, #16]
	movs r3, #128
	lsls r3, r3, #11
	adds r2, r2, r3
	movs r3, #1
	movs r4, #0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	str r3, [sp, #8]
	movs r3, #0
	str r4, [sp, #0]
	str r4, [sp, #4]
	str r4, [sp, #12]
	bl 0x02008ae8
	movs r0, #14
	movs r1, #1
	bl 0x0200ae54
.L_020018d6:
	movs r2, #0
	movs r1, #0
	movs r0, #14
	bl 0x0200ae1c
	movs r0, #20
	bl 0x0200ad9c
	movs r0, #14
	movs r1, #2
	bl 0x0200ae04
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #14
	bl 0x0200ae4c
	movs r5, #132
	movs r0, #60
	movs r6, #192
	bl 0x0200ad9c
	lsls r5, r5, #2
	lsls r6, r6, #10
	movs r1, #180
	adds r3, r6, #0
	adds r2, r5, #0
	lsls r1, r1, #1
	movs r0, #14
	bl 0x02008d90
	movs r1, #14
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	movs r1, #164
	adds r3, r6, #0
	adds r2, r5, #0
	lsls r1, r1, #1
	movs r0, #14
	bl 0x02008d90
	movs r1, #14
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	movs r1, #144
	adds r3, r6, #0
	adds r2, r5, #0
	lsls r1, r1, #1
	movs r0, #14
	bl 0x02008d90
	movs r1, #14
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	movs r1, #128
	adds r3, r6, #0
	adds r2, r5, #0
	lsls r1, r1, #1
	movs r0, #14
	bl 0x02008d90
	movs r2, #0
.L_02001970:
	movs r1, #14
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	movs r0, #0
	movs r1, #1
	bl 0x0200ae54
	movs r1, #0
	movs r2, #0
	movs r0, #14
	bl 0x0200adf4
	movs r0, #30
	bl 0x0200ad9c
	ldr r0, [pc, #28]
	bl 0x0200ad94
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl 0x0200adf4
	bl 0x0200adac
	sub sp, #-16
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000306
	.global Func_020019b8
	.thumb_func
Func_020019b8:
	push {r5, r6, lr}
	sub sp, #32
	bl 0x0200ada4
	add r5, sp, #8
	adds r0, r5, #0
	movs r6, #0
	bl 0x02008474
	cmp r0, #0
	beq .L_020019b8_0
	mov r2, sp
	add r3, sp, #24
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r3, [r5, #12]
	ldr r0, [r5]
	ldr r1, [r5, #4]
	ldr r2, [r5, #8]
	bl 0x02008608
	ldr r3, [r5, #4]
	cmp r3, #9
	beq .L_020019b8_1
	cmp r3, #11
	bne .L_020019b8_2
	b .L_020019b8_3
.L_020019b8_1:
	ldr r3, [r5, #8]
	movs r2, #68
	asrs r3, r3, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #4
	movs r2, #1
	movs r0, #38
	movs r1, #68
	bl 0x0200ad4c
	ldr r3, [r5, #8]
	asrs r2, r3, #20
	cmp r2, #42
	bne .L_020019b8_4
	movs r3, #23
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #26
	movs r1, #20
	movs r2, #2
	movs r3, #4
	bl 0x0200ad4c
	movs r0, #9
	movs r1, #1
	bl 0x0200ae3c
	ldr r0, [pc, #152]
	movs r6, #1
	bl 0x0200ad94
	b .L_020019b8_4
.L_020019b8_3:
	ldr r3, [r5, #8]
	asrs r2, r3, #20
	cmp r2, #40
	bne .L_020019b8_4
	movs r3, #32
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #26
	movs r1, #20
	movs r2, #2
	movs r3, #4
	bl 0x0200ad4c
	movs r0, #11
	movs r1, #1
	bl 0x0200ae3c
	ldr r0, [pc, #112]
	movs r6, #1
	bl 0x0200ad94
.L_020019b8_4:
	cmp r6, #0
	bne .L_020019b8_5
	bl 0x0200adac
	b .L_020019b8_6
.L_020019b8_5:
	ldr r0, [r5, #4]
	movs r1, #3
	bl 0x0200adfc
	movs r2, #6
	movs r1, #18
	ldr r0, [r5, #4]
	bl 0x0200ade4
	movs r0, #30
	bl 0x0200ad9c
	movs r1, #8
	ldr r0, [r5, #4]
	bl 0x0200adfc
	movs r0, #240
	bl 0x0200aeb4
	ldr r0, [r5, #4]
	bl 0x0200adbc
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	b .L_020019b8_0
.L_020019b8_2:
	cmp r3, #8
	bne .L_020019b8_0
	ldr r3, [r5, #8]
	movs r2, #49
	asrs r3, r3, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #42
	movs r1, #49
	movs r2, #1
	movs r3, #4
	bl 0x0200ad4c
.L_020019b8_0:
	bl 0x0200adac
.L_020019b8_6:
	sub sp, #-32
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000312
	.4byte 0x00000313
	.global Func_02001ac8
	.thumb_func
Func_02001ac8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #0
	sub sp, #12
	bl 0x0200adbc
	adds r6, r0, #0
	adds r7, r6, #0
	adds r7, #85
	ldrb r2, [r7]
	movs r0, #0
	mov r8, r2
	bl 0x0200adbc
	ldr r2, [pc, #148]
	ldr r3, [r0, #8]
	mov r5, sp
	adds r3, r3, r2
	str r3, [r5]
	movs r0, #0
	bl 0x0200adbc
	ldr r3, [r0, #12]
	movs r0, #0
	str r3, [r5, #4]
	bl 0x0200adbc
	ldr r3, [r0, #16]
	adds r0, r5, #0
	str r3, [r5, #8]
	bl 0x02008cd0
	cmp r0, #0
	beq .L_02001ac8_0
	bl 0x0200ada4
	movs r3, #0
	movs r1, #7
	strb r3, [r7]
	movs r0, #11
	bl 0x0200adfc
	ldr r5, [pc, #96]
	ldr r3, [r6, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r6, #20]
	adds r3, r3, r5
	str r3, [r6, #20]
	movs r0, #2
	bl 0x0200acec
	ldr r3, [r6, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r6, #20]
	adds r3, r3, r5
	str r3, [r6, #20]
	movs r0, #10
	bl 0x0200acec
	movs r5, #128
	ldr r3, [r6, #12]
	lsls r5, r5, #9
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r6, #20]
	adds r3, r3, r5
	str r3, [r6, #20]
	movs r0, #4
	bl 0x0200acec
	ldr r3, [r6, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r6, #20]
	adds r3, r3, r5
	str r3, [r6, #20]
	mov r3, r8
	strb r3, [r7]
	bl 0x0200adac
.L_02001ac8_0:
	sub sp, #-12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xffe00000
	.4byte 0xffff0000
	.global Func_02001b84
	.thumb_func
Func_02001b84:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	bl 0x0200ada4
	movs r0, #13
	bl 0x0200adbc
	ldr r5, [r0, #8]
	movs r0, #13
	bl 0x0200adbc
	ldr r3, [r0, #16]
	asrs r5, r5, #20
	asrs r7, r3, #20
	movs r3, #255
	movs r6, #1
	str r3, [sp, #4]
	adds r1, r5, #0
	adds r2, r7, #0
	movs r3, #1
	movs r0, #2
	str r6, [sp, #0]
	bl 0x02008244
	movs r2, #0
	mov r8, r2
	str r2, [sp, #4]
	adds r1, r5, #1
	adds r2, r7, #0
	movs r3, #1
	movs r0, #2
	str r6, [sp, #0]
	bl 0x02008244
	mov r3, r8
	str r3, [sp, #4]
	subs r1, r5, #1
	adds r2, r7, #0
	movs r3, #1
	movs r0, #2
	str r6, [sp, #0]
	bl 0x02008244
	mov r3, r8
	adds r2, r7, #1
	str r3, [sp, #4]
	adds r1, r5, #0
	movs r3, #1
	movs r0, #2
	str r6, [sp, #0]
	bl 0x02008244
	mov r3, r8
	str r3, [sp, #4]
	subs r2, r7, #1
	movs r0, #2
	adds r1, r5, #0
	movs r3, #1
	str r6, [sp, #0]
	bl 0x02008244
	cmp r5, #45
	bne .L_02001b84_0
	cmp r7, #6
	bne .L_02001b84_0
	movs r0, #13
	bl 0x0200adbc
	adds r3, r0, #0
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	ldr r3, [pc, #20]
	str r3, [r0, #20]
	str r3, [r0, #12]
.L_02001b84_0:
	bl 0x0200adac
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xfffe0000
	.global Func_02001c34
	.thumb_func
Func_02001c34:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	bl 0x0200ada4
	movs r0, #14
	bl 0x0200adbc
	ldr r6, [r0, #8]
	movs r0, #14
	bl 0x0200adbc
	ldr r5, [r0, #16]
	movs r3, #1
	str r3, [sp, #0]
	asrs r6, r6, #20
	asrs r5, r5, #20
	mov r8, r3
	movs r3, #255
	str r3, [sp, #4]
	adds r2, r5, #0
	adds r1, r6, #0
	mov r10, r3
	movs r0, #2
	movs r3, #1
	bl 0x02008244
	movs r7, #0
	mov r3, r8
	adds r2, r5, #0
	adds r1, r6, #1
	movs r0, #2
	str r3, [sp, #0]
.L_02001c7a:
	str r7, [sp, #4]
	bl 0x02008244
	mov r3, r8
	adds r2, r5, #0
	subs r1, r6, #1
	movs r0, #2
	str r3, [sp, #0]
	str r7, [sp, #4]
	bl 0x02008244
	adds r2, r5, #1
	mov r3, r8
	adds r1, r6, #0
	movs r0, #2
	str r3, [sp, #0]
	str r7, [sp, #4]
	bl 0x02008244
	subs r5, #1
	mov r3, r8
	adds r1, r6, #0
	adds r2, r5, #0
	movs r0, #2
	str r3, [sp, #0]
	str r7, [sp, #4]
	bl 0x02008244
	movs r0, #14
	bl 0x0200adbc
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #27
	bne 0x02009cee
	movs r0, #14
	bl 0x0200adbc
	adds r3, r0, #0
	adds r3, #85
	strb r7, [r3]
	ldr r3, [pc, #48]
	str r3, [r0, #20]
	str r3, [r0, #12]
	movs r0, #133
	lsls r0, r0, #2
	bl 0x0200ad94
	mov r3, r8
	str r3, [sp, #0]
	mov r3, r10
	str r3, [sp, #4]
	movs r0, #2
	movs r1, #43
	movs r2, #23
.L_02001ce8:
	movs r3, #1
	bl 0x02008244
	bl 0x0200adac
	sub sp, #-8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0xfffe
	.global Func_02001d04
	.thumb_func
Func_02001d04:
	push {r5, r6, lr}
	movs r0, #15
	sub sp, #16
	bl 0x0200adbc
	movs r6, #128
	adds r5, r0, #0
	bl 0x0200ada4
	movs r0, #15
	movs r1, #0
	bl 0x02008ea8
	lsls r6, r6, #12
	movs r1, #236
	adds r3, r6, #0
	lsls r1, r1, #1
	movs r2, #104
	movs r0, #15
	bl 0x02008d90
	movs r0, #10
	bl 0x0200ad9c
	ldr r2, [r5, #16]
	movs r3, #1
.L_02001d38:
	movs r4, #0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	adds r2, r2, r6
	str r3, [sp, #8]
	movs r3, #0
	str r4, [sp, #0]
	str r4, [sp, #4]
	str r4, [sp, #12]
	bl 0x02008ae8
	movs r0, #15
	movs r1, #1
	bl 0x0200ae54
	movs r2, #0
	movs r1, #0
	movs r0, #15
	bl 0x0200ae1c
	movs r0, #30
	bl 0x0200ad9c
	movs r0, #15
	movs r1, #2
	bl 0x0200ae04
	movs r2, #0
	ldr r1, [pc, #92]
.L_02001d72:
	movs r0, #15
	bl 0x0200ae44
	movs r0, #147
	bl 0x0200aeb4
	movs r0, #60
	bl 0x0200ad9c
	movs r0, #0
	bl 0x0200adbc
	movs r2, #10
	ldrsh r5, [r0, r2]
	movs r0, #0
	bl 0x0200adbc
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r3, #192
	lsls r3, r3, #11
	adds r1, r5, #0
	movs r0, #15
	bl 0x02008d90
	movs r0, #10
	bl 0x0200ad9c
	ldr r0, [pc, #40]
	bl 0x0200ad94
	ldr r3, [pc, #36]
	ldr r2, [pc, #40]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	movs r0, #53
	movs r1, #0
	bl 0x0200ae7c
	bl 0x0200adac
	sub sp, #-16
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0103
	.2byte 0x0000
	.4byte 0x00000307
	.4byte 0x02000240
	.4byte 0x0000022b
	.global Func_02001de0
	.thumb_func
Func_02001de0:
	push {r5, r6, lr}
	movs r0, #16
	sub sp, #16
	bl 0x0200adbc
	adds r5, r0, #0
	bl 0x0200ada4
	movs r0, #16
	movs r1, #1
	bl 0x02008ea8
	movs r1, #228
.L_02001dfa:
	movs r3, #192
	lsls r1, r1, #1
	lsls r3, r3, #11
	movs r2, #152
	movs r0, #16
	bl 0x02008d90
	movs r0, #10
	bl 0x0200ad9c
.L_02001e0e:
	ldr r2, [r5, #16]
	movs r3, #128
	lsls r3, r3, #11
	adds r2, r2, r3
	movs r3, #1
	movs r4, #0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	str r3, [sp, #8]
.L_02001e20:
	movs r3, #0
	str r4, [sp, #0]
	str r4, [sp, #4]
	str r4, [sp, #12]
	bl 0x02008ae8
	movs r0, #16
	movs r1, #1
	bl 0x0200ae54
	movs r2, #0
	movs r1, #0
	movs r0, #16
	bl 0x0200ae1c
	movs r0, #20
	bl 0x0200ad9c
.L_02001e44:
	movs r0, #16
	movs r1, #2
	bl 0x0200ae04
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #16
	bl 0x0200ae4c
	movs r5, #192
.L_02001e58:
	movs r0, #60
	bl 0x0200ad9c
	lsls r5, r5, #10
	movs r1, #224
	adds r3, r5, #0
	lsls r1, r1, #1
	movs r0, #16
	movs r2, #192
	bl 0x02008d90
	movs r6, #212
	movs r1, #16
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	lsls r6, r6, #1
	movs r0, #6
	bl 0x0200ad9c
	adds r3, r5, #0
	adds r1, r6, #0
	movs r0, #16
	movs r2, #208
	bl 0x02008d90
	movs r1, #16
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	adds r3, r5, #0
	adds r1, r6, #0
	movs r0, #16
	movs r2, #224
	bl 0x02008d90
	movs r2, #0
	movs r1, #16
	movs r0, #0
.L_02001eb0:
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	movs r0, #0
	movs r1, #1
	bl 0x0200ae54
.L_02001ec2:
	movs r1, #0
	movs r2, #0
	movs r0, #16
	bl 0x0200adf4
	movs r0, #30
	bl 0x0200ad9c
.L_02001ed2:
	movs r0, #194
	lsls r0, r0, #2
	bl 0x0200ad94
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x0200adf4
	bl 0x0200adac
	sub sp, #-16
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02001ef0
	.thumb_func
Func_02001ef0:
	push {r5, r6, lr}
	movs r0, #17
.L_02001ef4:
	sub sp, #16
	bl 0x0200adbc
	movs r6, #192
	adds r5, r0, #0
	bl 0x0200ada4
	movs r0, #17
.L_02001f04:
	movs r1, #1
	bl 0x02008ea8
	lsls r6, r6, #11
	movs r1, #196
	adds r3, r6, #0
	lsls r1, r1, #1
	movs r2, #104
	movs r0, #17
	bl 0x02008d90
	movs r0, #10
	bl 0x0200ad9c
	ldr r2, [r5, #16]
	movs r3, #128
	lsls r3, r3, #11
	adds r2, r2, r3
	movs r3, #1
	movs r4, #0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
.L_02001f30:
	str r3, [sp, #8]
	movs r3, #0
	str r4, [sp, #0]
	str r4, [sp, #4]
	str r4, [sp, #12]
	bl 0x02008ae8
	movs r0, #17
	movs r1, #1
	bl 0x0200ae54
	movs r2, #0
	movs r1, #0
	movs r0, #17
	bl 0x0200ae1c
	movs r0, #20
	bl 0x0200ad9c
	movs r0, #17
	movs r1, #2
	bl 0x0200ae04
	movs r1, #129
	lsls r1, r1, #1
.L_02001f62:
	movs r0, #17
	bl 0x0200ae4c
	movs r0, #60
	bl 0x0200ad9c
	movs r1, #188
	adds r3, r6, #0
	lsls r1, r1, #1
	movs r0, #17
	movs r2, #152
	bl 0x02008d90
	movs r1, #17
.L_02001f7e:
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r5, #192
	movs r0, #10
	bl 0x0200ad9c
	lsls r5, r5, #10
	movs r1, #164
	adds r3, r5, #0
	lsls r1, r1, #1
	movs r0, #17
	movs r2, #160
.L_02001f9a:
	bl 0x02008d90
	movs r1, #17
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	movs r1, #148
	adds r3, r5, #0
	lsls r1, r1, #1
	movs r0, #17
	movs r2, #160
	bl 0x02008d90
	movs r2, #0
	movs r1, #17
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	movs r0, #0
	movs r1, #1
	bl 0x0200ae54
	movs r1, #0
	movs r2, #0
	movs r0, #17
	bl 0x0200adf4
	movs r0, #30
	bl 0x0200ad9c
	ldr r0, [pc, #24]
	bl 0x0200ad94
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl 0x0200adf4
	bl 0x0200adac
	sub sp, #-16
	pop {r5, r6}
.L_02001ffc:
	pop {r0}
	bx r0
	.2byte 0x0309
	.2byte 0x0000
	.global Func_02002004
	.thumb_func
Func_02002004:
	push {lr}
	bl 0x0200ada4
	movs r0, #18
	movs r1, #1
.L_0200200e:
	bl 0x02008ea8
	movs r0, #186
	movs r1, #1
	movs r2, #252
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	bl 0x0200ae64
	movs r1, #186
	movs r2, #252
	movs r3, #144
	lsls r3, r3, #12
	lsls r2, r2, #1
	lsls r1, r1, #2
	movs r0, #18
	bl 0x02008d90
	movs r0, #18
	bl 0x02008e18
	movs r1, #15
	movs r0, #18
	bl 0x0200ae24
	movs r0, #18
	bl 0x0200adbc
	movs r1, #0
	bl 0x0200ad5c
	movs r0, #30
	bl 0x0200ad9c
	ldr r0, [pc, #28]
	bl 0x0200ad94
	movs r1, #186
	movs r2, #252
	movs r0, #22
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200adf4
	bl 0x0200adac
	pop {r0}
.L_02002070:
	bx r0
	.2byte 0x0000
	.2byte 0x030a
	.2byte 0x0000
	.global Func_02002078
	.thumb_func
Func_02002078:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #18
	sub sp, #16
	bl 0x0200adbc
	movs r6, #178
	adds r5, r0, #0
	bl 0x0200ada4
	movs r0, #18
	movs r1, #1
	bl 0x02008ea8
	lsls r6, r6, #2
	movs r2, #134
	movs r3, #192
	lsls r3, r3, #11
	adds r1, r6, #0
	lsls r2, r2, #2
	movs r0, #18
	mov r8, r3
	bl 0x02008d90
	movs r0, #10
	bl 0x0200ad9c
	ldr r2, [r5, #16]
	movs r3, #128
	lsls r3, r3, #11
	adds r2, r2, r3
	movs r3, #1
	movs r4, #0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	str r3, [sp, #8]
	movs r3, #0
	str r4, [sp, #0]
	str r4, [sp, #4]
	str r4, [sp, #12]
	bl 0x02008ae8
	movs r0, #18
	movs r1, #1
	bl 0x0200ae54
	movs r2, #0
	movs r1, #0
	movs r0, #18
	bl 0x0200ae1c
	movs r0, #20
.L_020020e2:
	bl 0x0200ad9c
	movs r0, #18
	movs r1, #2
	bl 0x0200ae04
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #18
	bl 0x0200ae4c
	movs r0, #60
	bl 0x0200ad9c
	movs r2, #142
	mov r3, r8
.L_02002102:
	adds r1, r6, #0
	lsls r2, r2, #2
	movs r0, #18
	bl 0x02008d90
	movs r1, #18
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r5, #192
	movs r0, #10
	bl 0x0200ad9c
	lsls r5, r5, #10
	movs r2, #150
	adds r3, r5, #0
	adds r1, r6, #0
	lsls r2, r2, #2
	movs r0, #18
	bl 0x02008d90
	movs r1, #18
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	adds r6, #24
	movs r2, #160
.L_02002142:
	adds r3, r5, #0
	adds r1, r6, #0
	lsls r2, r2, #2
	movs r0, #18
	bl 0x02008d90
	movs r1, #18
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	movs r2, #176
.L_02002160:
	adds r3, r5, #0
	adds r1, r6, #0
	lsls r2, r2, #2
	movs r0, #18
	bl 0x02008d90
	movs r2, #0
	movs r1, #18
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	movs r0, #0
.L_0200217e:
	movs r1, #1
	bl 0x0200ae54
	movs r1, #0
	movs r2, #0
	movs r0, #18
	bl 0x0200adf4
	movs r0, #30
	bl 0x0200ad9c
	ldr r0, [pc, #20]
	bl 0x0200ad94
	bl 0x0200adac
	sub sp, #-16
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000030b
	.global Func_020021b0
	.thumb_func
Func_020021b0:
	push {r5, lr}
	movs r0, #18
	sub sp, #16
	bl 0x0200adbc
	adds r5, r0, #0
	bl 0x0200ada4
	movs r1, #136
	movs r2, #180
	lsls r2, r2, #17
	movs r0, #18
	lsls r1, r1, #16
	bl 0x0200adf4
	movs r0, #18
	movs r1, #1
	bl 0x02008ea8
	movs r2, #204
	movs r3, #128
	lsls r2, r2, #1
	lsls r3, r3, #12
	movs r1, #136
	movs r0, #18
	bl 0x02008d90
	movs r0, #10
.L_020021e8:
	bl 0x0200ad9c
	ldr r2, [r5, #16]
	movs r3, #128
	lsls r3, r3, #11
	adds r2, r2, r3
	movs r3, #1
	movs r4, #0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	str r3, [sp, #8]
	movs r3, #0
	str r4, [sp, #0]
	str r4, [sp, #4]
	str r4, [sp, #12]
	bl 0x02008ae8
	movs r1, #192
	movs r2, #40
	movs r0, #18
	lsls r1, r1, #8
	bl 0x0200ae34
	movs r1, #129
	movs r0, #18
	lsls r1, r1, #1
	bl 0x0200ae4c
	movs r0, #18
.L_02002222:
	movs r1, #2
	bl 0x0200ae0c
	movs r0, #18
	movs r1, #1
	bl 0x0200ae54
	movs r2, #220
	movs r3, #192
	lsls r3, r3, #11
	lsls r2, r2, #1
	movs r0, #18
	movs r1, #136
	bl 0x02008d90
	movs r1, #18
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r5, #192
	movs r0, #10
	bl 0x0200ad9c
	lsls r5, r5, #10
	movs r2, #236
	adds r3, r5, #0
	lsls r2, r2, #1
	movs r0, #18
	movs r1, #136
	bl 0x02008d90
	movs r1, #18
	movs r2, #0
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	movs r2, #252
	adds r3, r5, #0
	lsls r2, r2, #1
	movs r0, #18
	movs r1, #136
	bl 0x02008d90
	movs r2, #0
	movs r1, #18
	movs r0, #0
	bl 0x0200ae14
	movs r0, #6
	bl 0x0200ad9c
	movs r0, #0
	movs r1, #1
	bl 0x0200ae54
	movs r1, #0
	movs r2, #0
	movs r0, #18
	bl 0x0200adf4
	movs r0, #60
	bl 0x0200ad9c
	ldr r0, [pc, #16]
	bl 0x0200ad94
	bl 0x0200adac
	sub sp, #-16
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000089d
	.global Func_020022c0
	.thumb_func
Func_020022c0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #18
	sub sp, #28
	bl 0x0200adbc
	mov r10, r0
	bl 0x0200ada4
	movs r1, #15
	movs r0, #18
	bl 0x0200ae24
	movs r0, #18
	bl 0x0200adbc
	movs r1, #0
	bl 0x0200ad5c
	movs r1, #136
	movs r2, #180
	lsls r2, r2, #17
	movs r0, #18
	lsls r1, r1, #16
	bl 0x0200adf4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl 0x0200ae5c
	movs r0, #136
	movs r1, #1
	movs r2, #196
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #16
	bl 0x0200ae64
	bl 0x0200ae6c
	movs r0, #60
	bl 0x0200ad9c
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #11
	bl 0x0200ad6c
	movs r0, #18
	bl 0x02008e18
	movs r0, #0
	movs r1, #2
	bl 0x0200ae04
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #336]
	bl 0x0200ad6c
	bl 0x0200ad74
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #20
	movs r0, #0
	bl 0x0200ae34
	movs r0, #40
	bl 0x0200ad9c
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl 0x0200ad6c
	movs r0, #18
	bl 0x02008e18
	movs r0, #1
	movs r1, #1
	ldr r2, [pc, #280]
	negs r1, r1
	negs r0, r0
	bl 0x0200ad6c
	bl 0x0200ad74
	movs r0, #40
	bl 0x0200ad9c
	movs r0, #18
	bl 0x02008e18
	ldr r3, [pc, #260]
	mov r1, r10
	str r3, [r1, #24]
	str r3, [r1, #28]
	movs r0, #18
	movs r1, #5
	bl 0x0200ae24
	movs r2, #196
	movs r3, #240
	lsls r2, r2, #1
	lsls r3, r3, #12
	movs r1, #136
	movs r0, #18
	bl 0x02008d90
	movs r0, #15
	bl 0x0200ad9c
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl 0x0200ad6c
	movs r2, #0
	movs r7, #0
	add r6, sp, #16
	mov r8, r2
.L_020022c0_0:
	lsls r5, r7, #12
	adds r0, r5, #0
	bl 0x0200ad04
	mov r3, r8
	str r0, [r6]
	adds r0, r5, #0
	str r3, [r6, #4]
	bl 0x0200acfc
	ldr r3, [r6]
	lsrs r2, r3, #31
	adds r2, r3, r2
	asrs r2, r2, #1
	adds r3, r3, r2
	str r0, [r6, #8]
	str r3, [r6]
	mov r1, r10
	ldr r4, [r1, #8]
	ldr r2, [r1, #16]
	ldr r1, [r6, #4]
	str r1, [sp, #0]
	movs r1, #1
	str r1, [sp, #8]
	mov r1, r8
	str r0, [sp, #4]
	str r1, [sp, #12]
	adds r0, r4, #0
	movs r1, #0
	adds r7, #1
	bl 0x02008ae8
	cmp r7, #16
	bls .L_020022c0_0
	movs r0, #30
	bl 0x0200ad9c
	movs r0, #1
	movs r1, #1
	ldr r2, [pc, #116]
	negs r1, r1
	negs r0, r0
	bl 0x0200ad6c
	bl 0x0200ad74
	movs r0, #148
	bl 0x0200aeb4
	movs r1, #2
	movs r0, #18
	bl 0x0200ae0c
	movs r0, #20
	bl 0x0200ad9c
	movs r0, #0
	bl 0x0200adbc
	movs r2, #10
	ldrsh r5, [r0, r2]
	movs r0, #0
	bl 0x0200adbc
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r3, #128
	subs r2, #16
	lsls r3, r3, #12
	adds r1, r5, #0
	movs r0, #18
	bl 0x02008d90
	movs r0, #10
	bl 0x0200ad9c
	ldr r3, [pc, #48]
	ldr r1, [pc, #52]
	movs r2, #3
	adds r3, r3, r1
	strb r2, [r3]
	ldr r0, [pc, #48]
	movs r1, #15
	bl 0x0200ae8c
	movs r0, #53
	movs r1, #1
	bl 0x0200ae7c
	bl 0x0200adac
	sub sp, #-28
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0000e666
	.4byte 0x00013333
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x00000046
	.global Func_020024ac
	.thumb_func
Func_020024ac:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_020024ac_0
	ldr r0, [pc, #36]
	b .L_020024ac_1
.L_020024ac_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020024ac_2
	ldr r0, [pc, #36]
	b .L_020024ac_1
.L_020024ac_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020024ac_3
	ldr r0, [pc, #32]
	b .L_020024ac_1
.L_020024ac_3:
	ldr r0, [pc, #32]
.L_020024ac_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000044
	.4byte 0x0200ba48
	.4byte 0x00000045
	.4byte 0x0200bb20
	.4byte 0x00000046
	.4byte 0x0200bc1c
	.4byte 0x0200bd54
	.global Func_02002500
	.thumb_func
Func_02002500:
	push {r5, lr}
	ldr r3, [pc, #852]
	movs r2, #224
	ldr r1, [r3]
	movs r3, #129
.L_0200250a:
	lsls r3, r3, #2
	lsls r2, r2, #1
	str r3, [r1, r2]
	ldr r1, [pc, #840]
	ldrsh r2, [r1, r2]
	ldr r3, [pc, #840]
	sub sp, #8
	cmp r2, r3
	beq .L_0200250a_0
	b 0x0200a6fa
.L_0200250a_0:
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
.L_02002528:
	cmp r3, #1
	bge .L_02002528_0
	b 0x0200ab56
.L_02002528_0:
	cmp r3, #4
	ble .L_02002528_1
	cmp r3, #9
	ble .L_02002528_2
	b 0x0200ab56
.L_02002528_2:
	cmp r3, #7
	bge .L_02002528_3
	b 0x0200ab56
.L_02002528_3:
	b 0x0200a622
.L_02002528_1:
	ldr r0, [pc, #800]
	bl 0x0200ad8c
	cmp r0, #0
	bne 0x0200a5f0
.L_0200254a:
	bl 0x0200ada4
	movs r0, #1
	bl 0x0200acec
	movs r0, #10
	movs r1, #1
	bl 0x0200ae24
	movs r1, #184
	movs r2, #240
	movs r0, #10
	lsls r1, r1, #15
	lsls r2, r2, #15
	bl 0x0200adf4
	movs r1, #208
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ae34
	movs r0, #0
	ldr r1, [pc, #748]
	ldr r2, [pc, #752]
	bl 0x0200adc4
	movs r1, #136
	movs r2, #64
	movs r0, #0
	bl 0x0200addc
	bl 0x0200ae94
	bl 0x0200ae9c
	movs r0, #0
	bl 0x0200adec
	movs r0, #30
	bl 0x0200ad9c
	movs r1, #128
	movs r2, #0
	movs r0, #10
	lsls r1, r1, #1
	bl 0x0200ae44
	movs r1, #2
	movs r0, #10
.L_020025ae:
	bl 0x0200ae0c
	movs r0, #30
	bl 0x0200ad9c
	movs r3, #224
	lsls r3, r3, #11
	movs r2, #116
	movs r1, #136
	movs r0, #10
	bl 0x02008d90
	movs r0, #10
	bl 0x02008e18
	movs r1, #15
	movs r0, #10
	bl 0x0200ae24
	movs r0, #10
	bl 0x0200adbc
	movs r1, #0
	bl 0x0200ad5c
	ldr r0, [pc, #640]
	bl 0x0200ad94
	movs r0, #60
	bl 0x0200ad9c
	bl 0x0200adac
	ldr r0, [pc, #636]
	bl 0x0200ad8c
	cmp r0, #0
	bne .L_020025ae_0
	b 0x0200ab56
.L_020025ae_0:
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200ad8c
	cmp r0, #0
	beq .L_020025ae_1
	b 0x0200ab56
.L_020025ae_1:
	movs r0, #10
	movs r1, #15
	bl 0x0200ae24
	movs r1, #136
	movs r2, #232
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #15
	bl 0x0200adf4
	b 0x0200ab56
	.2byte 0x2000
	.2byte 0xf000
	.2byte 0xfbca
	.2byte 0x2800
	.2byte 0xd004
	.2byte 0x6881
	.2byte 0x6902
	.2byte 0x2010
	.2byte 0xf000
	.2byte 0xfbdf
	.2byte 0x2010
.L_02002638:
	bl 0x0200adbc
	movs r3, #0
	str r3, [r0, #108]
	ldr r0, [pc, #556]
	bl 0x0200ad8c
	cmp r0, #0
	beq .L_02002638_0
	movs r0, #16
	bl 0x0200adbc
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r0, #12]
.L_02002638_0:
	movs r0, #1
.L_02002658:
	bl 0x0200acec
	movs r1, #158
	movs r2, #220
	movs r0, #16
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200adf4
	ldr r0, [pc, #520]
	bl 0x0200ad8c
	cmp r0, #0
	bne .L_02002658_0
	movs r0, #16
	bl 0x0200ac0c
.L_02002658_0:
	movs r0, #11
	movs r1, #15
	bl 0x0200ae24
	movs r1, #15
	movs r0, #12
	bl 0x0200ae24
	movs r0, #11
	bl 0x0200adbc
	movs r1, #0
	bl 0x0200ad5c
	movs r0, #12
	bl 0x0200adbc
	movs r1, #0
	bl 0x0200ad5c
.L_020026a2:
	movs r0, #8
	bl 0x020088c0
	movs r0, #196
	lsls r0, r0, #2
	bl 0x0200ad8c
	cmp r0, #0
	bne .L_020026a2_0
	movs r0, #9
	bl 0x020088c0
	b 0x0200ab56
.L_020026a2_0:
	movs r0, #1
	bl 0x0200acec
	movs r1, #132
	movs r2, #204
	lsls r2, r2, #17
	movs r0, #9
	lsls r1, r1, #18
.L_020026cc:
	bl 0x0200adf4
	movs r0, #9
	movs r1, #4
	bl 0x0200adfc
	movs r3, #31
	movs r2, #25
	str r3, [sp, #0]
.L_020026de:
	str r2, [sp, #4]
	movs r3, #2
	movs r0, #38
	movs r1, #27
	movs r2, #4
	bl 0x0200ad4c
	movs r0, #9
	bl 0x0200adbc
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	b 0x0200ab56
	.2byte 0x4b5f
.L_020026fc:
	cmp r2, r3
	beq .L_020026fc_0
	b 0x0200a88c
.L_020026fc_0:
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bge .L_020026fc_1
	b 0x0200ab56
.L_020026fc_1:
	cmp r3, #6
	ble .L_020026fc_2
	cmp r3, #12
	ble .L_020026fc_3
	b 0x0200ab56
.L_020026fc_3:
	cmp r3, #10
	bge .L_020026fc_4
	b 0x0200ab56
.L_020026fc_4:
	b 0x0200a75c
.L_020026fc_2:
	ldr r0, [pc, #340]
	bl 0x0200ad8c
	cmp r0, #0
	bne 0x0200a742
	movs r1, #15
	movs r0, #12
	bl 0x0200ae24
	movs r0, #12
	bl 0x0200adbc
	movs r1, #0
.L_0200273e:
	bl 0x0200ad5c
	movs r0, #193
	lsls r0, r0, #2
	bl 0x0200ad8c
	cmp r0, #0
	beq .L_0200273e_0
	b 0x0200ab56
.L_0200273e_0:
	movs r1, #15
	movs r0, #13
	bl 0x0200ae24
	movs r0, #13
	b 0x0200a962
	.2byte 0x4848
	.2byte 0xf000
	.2byte 0xfb15
	.2byte 0x2800
	.2byte 0xd103
	.2byte 0x200a
	.2byte 0xf7fe
	.2byte 0xf8aa
	.2byte 0xe027
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xfabc
	.2byte 0x218a
	.2byte 0x22ff
	.2byte 0x0452
	.2byte 0x200a
	.2byte 0x0489
	.2byte 0xf000
	.2byte 0xfb39
	.2byte 0x2104
	.2byte 0x200a
	.2byte 0xf000
	.2byte 0xfb39
	.2byte 0x200a
	.2byte 0xf000
	.2byte 0xfb16
	.2byte 0x2302
	.2byte 0x3023
	.2byte 0x7003
	.2byte 0x221e
	.2byte 0x2322
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x202c
	.2byte 0x211e
	.2byte 0x2202
	.2byte 0x2304
	.2byte 0xf000
	.2byte 0xfad1
	.2byte 0x2304
	.2byte 0x2500
	.2byte 0x9300
	.2byte 0x2000
	.2byte 0x2123
	.2byte 0x221d
	.2byte 0x2301
	.2byte 0x9501
	.2byte 0xf7fd
	.2byte 0xfd43
	.2byte 0x2008
	.2byte 0xf7fe
	.2byte 0xf87e
	.2byte 0x2009
	.2byte 0xf7fe
	.2byte 0xf87b
	.2byte 0x200b
	.2byte 0xf000
	.2byte 0xfaf6
	.2byte 0x6885
	.2byte 0x200b
	.2byte 0xf000
	.2byte 0xfaf2
.L_020027d8:
	movs r3, #1
	ldr r2, [r0, #16]
	asrs r5, r5, #20
	str r3, [sp, #0]
	movs r3, #255
	asrs r2, r2, #20
	str r3, [sp, #4]
	adds r1, r5, #0
	movs r3, #1
	movs r0, #2
	bl 0x02008244
	movs r0, #1
	bl 0x0200acec
	movs r0, #11
	movs r1, #6
	bl 0x0200ae24
	movs r0, #8
	bl 0x0200adbc
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #8
	orrs r3, r2
	strb r3, [r0]
	ldr r0, [pc, #116]
	bl 0x0200ad8c
	cmp r0, #0
	beq .L_020027d8_0
	b 0x0200ab56
.L_020027d8_0:
	movs r1, #15
	movs r0, #14
	bl 0x0200ae24
	movs r0, #14
	bl 0x0200adbc
	movs r1, #0
	bl 0x0200ad5c
	ldr r0, [pc, #88]
	bl 0x0200ad8c
.L_02002834:
	cmp r0, #0
	bne .L_02002834_0
	b .L_02002834_1
.L_02002834_0:
	movs r1, #212
	movs r2, #240
	movs r0, #14
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200adf4
	movs r1, #212
	movs r2, #240
	movs r0, #17
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200adf4
	b .L_02002834_1
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x0044
	.2byte 0x0000
	.2byte 0x089c
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x3333
	.2byte 0x0000
	.2byte 0x0109
	.2byte 0x0000
	.2byte 0x0fd4
	.2byte 0x0000
	.2byte 0x0045
	.2byte 0x0000
	.2byte 0x0303
	.2byte 0x0000
	.2byte 0x0311
	.2byte 0x0000
	.2byte 0x0306
	.2byte 0x0000
	.2byte 0x0305
	.2byte 0x0000
	.2byte 0x4bb4
	.2byte 0x429a
	.2byte 0xd000
	.2byte 0xe160
	.2byte 0x22e1
	.2byte 0x0052
	.2byte 0x188b
	.2byte 0x2200
	.2byte 0x5e9b
	.2byte 0x3b03
	.2byte 0x2b0c
	.2byte 0xd900
	.2byte 0xe157
	.2byte 0x4aaf
	.2byte 0x009b
	.2byte 0x589b
	.2byte 0x469f
	.2byte 0x0000
	.2byte 0xa8e4
	.2byte 0x0200
	.2byte 0xa8e4
	.2byte 0x0200
	.2byte 0xa8e4
	.2byte 0x0200
	.2byte 0xa8e4
	.2byte 0x0200
	.2byte 0xa96e
	.2byte 0x0200
	.2byte 0xa9ba
	.2byte 0x0200
	.2byte 0xa9ba
	.2byte 0x0200
	.2byte 0xa9ba
	.2byte 0x0200
	.2byte 0xa9ba
	.2byte 0x0200
	.2byte 0xab2c
	.2byte 0x0200
	.2byte 0xab2c
	.2byte 0x0200
	.2byte 0xab56
	.2byte 0x0200
	.2byte 0xab50
	.2byte 0x0200
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xfa01
	.2byte 0x489f
	.2byte 0xf000
	.2byte 0xfa4e
	.2byte 0x2800
	.2byte 0xd10f
	.2byte 0x210f
	.2byte 0x200f
	.2byte 0xf000
	.2byte 0xfa94
	.2byte 0x200f
	.2byte 0xf000
	.2byte 0xfa5d
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xfa2a
	.2byte 0x2013
	.2byte 0xf000
	.2byte 0xfa57
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xfa24
	.2byte 0x20c2
	.2byte 0x0080
	.2byte 0xf000
	.2byte 0xfa38
	.2byte 0x2800
	.2byte 0xd10f
	.2byte 0x210f
	.2byte 0x2010
	.2byte 0xf000
	.2byte 0xfa7e
	.2byte 0x2010
	.2byte 0xf000
	.2byte 0xfa47
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xfa14
	.2byte 0x2014
	.2byte 0xf000
	.2byte 0xfa41
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xfa0e
	.2byte 0x488a
	.2byte 0xf000
	.2byte 0xfa23
	.2byte 0x2800
	.2byte 0xd000
	.2byte 0xe104
	.2byte 0x210f
	.2byte 0x2011
	.2byte 0xf000
	.2byte 0xfa68
	.2byte 0x2011
	.2byte 0xf000
	.2byte 0xfa31
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xf9fe
	.2byte 0x2015
	.2byte 0xf000
	.2byte 0xfa2b
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xf9f8
	.2byte 0xe0f3
	.2byte 0x200d
	.2byte 0xf000
	.2byte 0xfa24
	.2byte 0x6885
	.2byte 0x200d
	.2byte 0xf000
	.2byte 0xfa20
	.2byte 0x2301
	.2byte 0x6902
	.2byte 0x152d
	.2byte 0x9300
	.2byte 0x23ff
	.2byte 0x1512
	.2byte 0x9301
	.2byte 0x1c29
	.2byte 0x2301
	.2byte 0x2002
	.2byte 0xf7fd
	.2byte 0xfc58
	.2byte 0x2106
	.2byte 0x200d
	.2byte 0xf000
	.2byte 0xfa44
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xf9a5
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfa0a
	.2byte 0x3059
	.2byte 0x7802
	.2byte 0x2308
	.2byte 0x4313
	.2byte 0x7003
	.2byte 0x2008
	.2byte 0xf7fd
	.2byte 0xff84
	.2byte 0xe0cd
	.2byte 0x25b9
	.2byte 0x046d
	.2byte 0x2100
	.2byte 0x1c2a
	.2byte 0x23df
	.2byte 0x486a
	.2byte 0xf7fe
	.2byte 0xf841
	.2byte 0x2100
	.2byte 0x1c2a
	.2byte 0x23df
	.2byte 0x4868
	.2byte 0xf7fe
	.2byte 0xf83b
	.2byte 0x200a
	.2byte 0xf7fd
	.2byte 0xff72
	.2byte 0x200c
	.2byte 0xf7fd
	.2byte 0xff6f
	.2byte 0x4865
	.2byte 0xf000
	.2byte 0xf9d2
	.2byte 0x2800
	.2byte 0xd103
	.2byte 0x2009
	.2byte 0xf7fd
	.2byte 0xff67
	.2byte 0xe02f
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xf979
	.2byte 0x2009
	.2byte 0x2104
	.2byte 0xf000
	.2byte 0xf9fd
	.2byte 0x22c7
	.2byte 0x495d
	.2byte 0x0452
	.2byte 0x2009
	.2byte 0xf000
	.2byte 0xf9f3
	.2byte 0x2009
	.2byte 0xf000
	.2byte 0xf9d4
	.2byte 0x3023
	.2byte 0x7802
	.2byte 0x2302
	.2byte 0x4313
	.2byte 0x7003
	.2byte 0x2217
	.2byte 0x232a
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x2304
	.2byte 0x2114
	.2byte 0x2202
	.2byte 0x201a
	.2byte 0xf000
	.2byte 0xf98d
	.2byte 0x2085
	.2byte 0x0080
	.2byte 0xf000
	.2byte 0xf9ad
	.2byte 0x219e
	.2byte 0x22dc
	.2byte 0x0489
	.2byte 0x200e
	.2byte 0x0452
	.2byte 0xf000
	.2byte 0xf9d6
	.2byte 0x200e
	.2byte 0xf000
	.2byte 0xf9b7
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xf984
	.2byte 0x484a
	.2byte 0xf000
	.2byte 0xf999
	.2byte 0x2800
	.2byte 0xd103
	.2byte 0x200b
	.2byte 0xf7fd
	.2byte 0xff2e
	.2byte 0xe01b
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xf940
	.2byte 0x200b
	.2byte 0x2104
	.2byte 0xf000
	.2byte 0xf9c4
	.2byte 0x4943
	.2byte 0x4a44
	.2byte 0x200b
	.2byte 0xf000
	.2byte 0xf9bb
	.2byte 0x200b
	.2byte 0xf000
	.2byte 0xf99c
	.2byte 0x2302
	.2byte 0x3023
	.2byte 0x7003
	.2byte 0x2220
	.2byte 0x2328
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x201a
	.2byte 0x2114
	.2byte 0x2202
	.2byte 0x2304
	.2byte 0xf000
	.2byte 0xf957
	.2byte 0x200e
	.2byte 0xf000
	.2byte 0xf98c
	.2byte 0x6885
	.2byte 0x200e
	.2byte 0xf000
	.2byte 0xf988
	.2byte 0x2301
	.2byte 0x6902
	.2byte 0x152d
	.2byte 0x9300
	.2byte 0x23ff
	.2byte 0x1512
	.2byte 0x9301
	.2byte 0x1c29
	.2byte 0x2301
	.2byte 0x2002
	.2byte 0xf7fd
	.2byte 0xfbc0
	.2byte 0x2106
	.2byte 0x200e
	.2byte 0xf000
	.2byte 0xf9ac
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xf90d
	.2byte 0x2009
	.2byte 0xf000
	.2byte 0xf972
	.2byte 0x3059
	.2byte 0x7802
	.2byte 0x2308
	.2byte 0x4313
	.2byte 0x7003
	.2byte 0x482a
	.2byte 0xf000
	.2byte 0xf952
	.2byte 0x2800
	.2byte 0xd11c
	.2byte 0x210f
	.2byte 0x2012
	.2byte 0xf000
	.2byte 0xf998
	.2byte 0x2012
	.2byte 0xf000
	.2byte 0xf961
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xf92e
	.2byte 0x4823
	.2byte 0xf000
	.2byte 0xf943
	.2byte 0x2800
	.2byte 0xd00d
	.2byte 0x21ba
	.2byte 0x22fc
	.2byte 0x2016
	.2byte 0x0489
	.2byte 0x0452
	.2byte 0xf000
	.2byte 0xf96e
	.2byte 0x21ba
	.2byte 0x22fc
	.2byte 0x2012
	.2byte 0x0489
	.2byte 0x0452
	.2byte 0xf000
	.2byte 0xf967
	.2byte 0xf7ff
	.2byte 0xf885
	.2byte 0xe014
	.2byte 0x2012
	.2byte 0x4919
	.2byte 0xf000
	.2byte 0xf94c
	.2byte 0x4818
	.2byte 0xf000
	.2byte 0xf929
	.2byte 0x2800
	.2byte 0xd00b
	.2byte 0x4817
	.2byte 0xf000
	.2byte 0xf924
	.2byte 0x2800
	.2byte 0xd006
	.2byte 0x4815
	.2byte 0xf000
	.2byte 0xf923
	.2byte 0xe002
	.2byte 0x4812
	.2byte 0xf000
	.2byte 0xf91f
.L_02002834_1:
	movs r0, #0
	sub sp, #-8
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0046
	.2byte 0x0000
	.2byte 0xa8b0
	.2byte 0x0200
	.2byte 0x0307
	.2byte 0x0000
	.2byte 0x0309
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x02de
	.2byte 0x0000
	.2byte 0x02f2
	.2byte 0x0312
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x02ba
	.2byte 0x0313
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x029a
	.2byte 0x0000
	.2byte 0x0226
	.2byte 0x030b
	.2byte 0x0000
	.2byte 0x030a
	.2byte 0x0000
	.2byte 0xb084
	.2byte 0x0200
	.2byte 0x0893
	.2byte 0x0000
	.2byte 0x089e
	.2byte 0x0000
	.2byte 0x088f
	.2byte 0x0000
	.global Func_02002ba4
	.thumb_func
Func_02002ba4:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r0, [r6, #48]
	ldr r7, [r6, #80]
	bl 0x0200acfc
	lsls r5, r0, #1
	cmp r5, #0
.L_02002bb4:
	ble .L_02002bb4_0
	negs r5, r5
.L_02002bb4_0:
	ldr r0, [r6, #48]
	bl 0x0200ad04
	ldr r3, [r6, #56]
	lsls r0, r0, #1
	adds r3, r3, r0
.L_02002bc4:
	str r3, [r6, #8]
	ldr r0, [r6, #48]
	ldr r3, [r6, #60]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r5
	adds r0, r0, r2
	str r3, [r6, #12]
	bl 0x0200ad04
	cmp r0, #0
	bge .L_02002bc4_0
	adds r0, #7
.L_02002bc4_0:
	asrs r3, r0, #3
	strh r3, [r7, #30]
	bl 0x0200acf4
	adds r5, r0, #0
	bl 0x0200acf4
	lsls r5, r5, #9
	lsls r0, r0, #9
	ldr r3, [r6, #48]
	lsrs r0, r0, #16
	lsrs r5, r5, #16
	adds r5, r5, r0
	movs r2, #128
	adds r3, r3, r5
.L_02002bfc:
	lsls r2, r2, #3
	adds r3, r3, r2
	str r3, [r6, #48]
	movs r0, #0
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02002c0c
	.thumb_func
Func_02002c0c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	bl 0x0200adbc
	adds r7, r0, #0
.L_02002c1c:
	ldr r6, [r7, #80]
	movs r2, #13
	ldrb r3, [r6, #9]
	negs r2, r2
	ands r2, r3
	movs r3, #4
	ldrb r1, [r6, #5]
	orrs r2, r3
	movs r3, #33
	negs r3, r3
	ands r3, r1
	strb r3, [r6, #5]
	movs r3, #15
	ands r2, r3
.L_02002c38:
	strb r2, [r6, #9]
	movs r2, #0
	mov r8, r2
	adds r3, r6, #0
	adds r3, #39
	mov r2, r8
	strb r2, [r3]
	movs r1, #0
	bl 0x0200ad5c
	movs r3, #92
	adds r3, r3, r7
	mov r2, r8
	strb r2, [r3]
	mov r10, r3
	adds r3, r7, #0
	adds r3, #85
	strb r2, [r3]
	ldr r0, [pc, #124]
	bl 0x0200ad8c
	cmp r0, #0
	bne .L_02002c38_0
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #14
	adds r3, r3, r2
	str r3, [r7, #12]
.L_02002c38_0:
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #1
	strb r3, [r1]
	mov r9, r2
	adds r3, r7, #0
	mov r2, r9
	adds r3, #97
	movs r1, #193
	strb r2, [r3]
	lsls r1, r1, #3
	movs r0, #17
	bl 0x0200ad0c
	adds r5, r0, #0
	movs r0, #181
	bl 0x0200ad84
	movs r3, #128
	lsls r3, r3, #3
.L_02002c9e:
	adds r5, r5, r3
	adds r2, r5, #0
	movs r1, #128
	ldrb r0, [r6, #28]
	bl 0x0200ad1c
	movs r0, #17
	bl 0x0200ad14
	ldr r3, [r7, #8]
	str r3, [r7, #56]
	ldr r3, [r7, #12]
	mov r2, r8
	str r2, [r7, #48]
	str r3, [r7, #60]
	mov r2, r10
	mov r3, r9
	strb r3, [r2]
	ldr r3, [pc, #28]
	str r3, [r7, #108]
	adds r3, r7, #0
	adds r3, #86
	mov r2, r8
	strb r2, [r3]
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0109
	.2byte 0x0000
	.4byte 0x0200aba5
	.include "games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/IMPORT.INC"
@ The compiler library links here from its licensed container.
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
	.4byte 0x0200afb0
	.4byte 0x0200afe8
	.4byte 0x0200b020
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000006
	.4byte 0x00000022
	.4byte 0x02008d6d
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x0000000f
	.4byte 0x00000022
	.4byte 0x02008d6d
	.4byte 0x00000022
	.4byte 0x02008d81
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000012
	.4byte 0x80000000
	.4byte 0x00000022
	.4byte 0x02008d81
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000010
	.4byte 0xffff0001
	.4byte 0x00000088
	.4byte 0x40000018
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x000000e0
	.4byte 0xffff0002
	.4byte 0x00000018
	.4byte 0x00000080
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x000000e0
	.4byte 0xffff0003
	.4byte 0x000000f8
	.4byte 0x80000080
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x000000e0
	.4byte 0xffff0004
	.4byte 0x00000088
	.4byte 0xc00000c8
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x000000e0
	.4byte 0xffff0005
	.4byte 0x00000048
	.4byte 0x40000108
	.4byte 0x00000000
	.4byte 0x011000f0
	.4byte 0x000001d0
	.4byte 0xffff0006
	.4byte 0x000000c8
	.4byte 0xc00001b8
	.4byte 0x00000000
	.4byte 0x011000f0
	.4byte 0x000001d0
	.4byte 0xffff0007
	.4byte 0x000001a0
	.4byte 0x40000108
	.4byte 0x01200000
	.4byte 0x02a000f0
	.4byte 0x00000270
	.4byte 0xffff0008
	.4byte 0x00000230
	.4byte 0x40000108
	.4byte 0x01200000
	.4byte 0x02a000f0
	.4byte 0x00000270
	.4byte 0xffff0009
	.4byte 0x00000288
	.4byte 0x80000158
	.4byte 0x01200000
	.4byte 0x02a000f0
	.4byte 0x00000270
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000018
	.4byte 0x00000080
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x000000e0
	.4byte 0xffff0002
	.4byte 0x000000f8
	.4byte 0x80000080
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x000000e0
	.4byte 0xffff0003
	.4byte 0x000001a8
	.4byte 0x40000018
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff0004
	.4byte 0x00000138
	.4byte 0x00000088
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff0005
	.4byte 0x00000218
	.4byte 0x80000078
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff0006
	.4byte 0x000001a8
	.4byte 0xc00000c8
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff0007
	.4byte 0x000002e8
	.4byte 0x40000018
	.4byte 0x02400000
	.4byte 0x03500000
	.4byte 0x000000e0
	.4byte 0xffff0008
	.4byte 0x000002e8
	.4byte 0xc00000c8
	.4byte 0x02400000
	.4byte 0x03500000
	.4byte 0x000000e0
	.4byte 0xffff0009
	.4byte 0x00000018
	.4byte 0x00000170
	.4byte 0x00000000
	.4byte 0x011000f0
	.4byte 0x000001d0
	.4byte 0xffff000a
	.4byte 0x000001e8
	.4byte 0x40000108
	.4byte 0x01200000
	.4byte 0x02e000f0
	.4byte 0x00000270
	.4byte 0xffff000b
	.4byte 0x00000138
	.4byte 0x00000210
	.4byte 0x01200000
	.4byte 0x02e000f0
	.4byte 0x00000270
	.4byte 0xffff000c
	.4byte 0x00000168
	.4byte 0xc0000258
	.4byte 0x01200000
	.4byte 0x02e000f0
	.4byte 0x00000270
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000018
	.4byte 0x00000080
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x000000e0
	.4byte 0xffff0002
	.4byte 0x000000f8
	.4byte 0x80000080
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x000000e0
	.4byte 0xffff0003
	.4byte 0x000001a8
	.4byte 0x40000018
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff0004
	.4byte 0x00000138
	.4byte 0x000000a0
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff0005
	.4byte 0x00000218
	.4byte 0x800000a0
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff0006
	.4byte 0x000001a8
	.4byte 0xc00000c8
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff0007
	.4byte 0x00000338
	.4byte 0x80000088
	.4byte 0x02400000
	.4byte 0x03500000
	.4byte 0x000000e0
	.4byte 0xffff0008
	.4byte 0x000002d8
	.4byte 0x40000108
	.4byte 0x01900000
	.4byte 0x035000f0
	.4byte 0x00000280
	.4byte 0xffff0009
	.4byte 0x00000338
	.4byte 0x80000200
	.4byte 0x01900000
	.4byte 0x035000f0
	.4byte 0x00000280
	.4byte 0xffff000a
	.4byte 0x00000338
	.4byte 0x80000260
	.4byte 0x01900000
	.4byte 0x035000f0
	.4byte 0x00000280
	.4byte 0xffff000b
	.4byte 0x000002e0
	.4byte 0xc0000268
	.4byte 0x01900000
	.4byte 0x035000f0
	.4byte 0x00000280
	.4byte 0xffff000c
	.4byte 0x00000108
	.4byte 0x40000108
	.4byte 0x00000000
	.4byte 0x018000f0
	.4byte 0x000001f0
	.4byte 0xffff000d
	.4byte 0x00000088
	.4byte 0xc00001d8
	.4byte 0x00000000
	.4byte 0x018000f0
	.4byte 0x000001f0
	.4byte 0xffff000f
	.4byte 0x00000088
	.4byte 0xc00001b0
	.4byte 0x00000000
	.4byte 0x018000f0
	.4byte 0x000001f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000138
	.4byte 0x00000080
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff0002
	.4byte 0x000000f8
	.4byte 0x80000170
	.4byte 0x00000000
	.4byte 0x011000f0
	.4byte 0x000001d0
	.4byte 0xffff0003
	.4byte 0x000001a8
	.4byte 0xc00001b8
	.4byte 0x01200000
	.4byte 0x023000f0
	.4byte 0x000001d0
	.4byte 0xffff0004
	.4byte 0x000002c8
	.4byte 0x40000108
	.4byte 0x02400000
	.4byte 0x035000f0
	.4byte 0x000001d0
	.4byte 0xffff0005
	.4byte 0x00000088
	.4byte 0xc00002a8
	.4byte 0x00000000
	.4byte 0x011001e0
	.4byte 0x000002c0
	.4byte 0xffff0006
	.4byte 0x00000138
	.4byte 0x00000228
	.4byte 0x01200000
	.4byte 0x023001e0
	.4byte 0x000002c0
	.4byte 0xffff0007
	.4byte 0x00000138
	.4byte 0x00000288
	.4byte 0x01200000
	.4byte 0x023001e0
	.4byte 0x000002c0
	.4byte 0xffff0008
	.4byte 0x00000288
	.4byte 0xc00002a8
	.4byte 0x02400000
	.4byte 0x035001e0
	.4byte 0x000002c0
	.4byte 0xffff0009
	.4byte 0x00000218
	.4byte 0x80000080
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff000a
	.4byte 0x00000018
	.4byte 0x00000170
	.4byte 0x00000000
	.4byte 0x011000f0
	.4byte 0x000001d0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000044
	.4byte 0x0010d002
	.4byte 0x00202047
	.4byte 0x00301047
	.4byte 0x00405044
	.4byte 0x00504044
	.4byte 0x00607044
	.4byte 0x00706044
	.4byte 0x00808047
	.4byte 0x00901045
	.4byte 0x00000045
	.4byte 0x00109044
	.4byte 0x00204045
	.4byte 0x00303047
	.4byte 0x00402045
	.4byte 0x00509045
	.4byte 0x00607045
	.4byte 0x00706045
	.4byte 0x0080a045
	.4byte 0x00905045
	.4byte 0x00a08045
	.4byte 0x00b02046
	.4byte 0x00c04047
	.4byte 0x00000046
	.4byte 0x00105046
	.4byte 0x0020b045
	.4byte 0x00305047
	.4byte 0x00407046
	.4byte 0x00501046
	.4byte 0x00608046
	.4byte 0x00704046
	.4byte 0x00806046
	.4byte 0x00906047
	.4byte 0x00a07047
	.4byte 0x00b0c046
	.4byte 0x00c0b046
	.4byte 0x00d2b002
	.4byte 0x00000047
	.4byte 0x00103044
	.4byte 0x00202044
	.4byte 0x00303045
	.4byte 0x0040c045
	.4byte 0x00503046
	.4byte 0x00609046
	.4byte 0x0070a046
	.4byte 0x00808044
	.4byte 0x00b01047
	.4byte 0x00c02047
	.4byte 0x00d03047
	.4byte 0x00e04047
	.4byte 0x00f05047
	.4byte 0x01006047
	.4byte 0x01107047
	.4byte 0x01208047
	.4byte 0x01309047
	.4byte 0x0140a047
	.4byte 0x000001ff
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x02100000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00024000
	.4byte 0x030000bf
	.4byte 0x0200b084
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0x030100bf
	.4byte 0x0200b084
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x00024000
	.4byte 0x030200bf
	.4byte 0x0200b084
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0x03000016
	.4byte 0x0200b0bc
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0x03010016
	.4byte 0x0200b0bc
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x01024000
	.4byte 0x03020016
	.4byte 0x0200b0bc
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0x0fd40016
	.4byte 0x00000007
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x02500000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x02500000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00024000
	.4byte 0xffff0125
	.4byte 0x0200b064
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0x030300bf
	.4byte 0x0200b084
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00024000
	.4byte 0x030400bf
	.4byte 0x0200b084
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00024000
	.4byte 0x030600bf
	.4byte 0x0200b084
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01e00000
	.4byte 0x00024000
	.4byte 0x03030016
	.4byte 0x0200b0bc
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00024000
	.4byte 0x03040016
	.4byte 0x0200b0bc
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x01024000
	.4byte 0x03060016
	.4byte 0x0200b0bc
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01e00000
	.4byte 0x01024000
	.4byte 0x0032005a
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01e00000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x02200000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00024000
	.4byte 0xffff0125
	.4byte 0x0200b064
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff0125
	.4byte 0x0200b064
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0x030700bf
	.4byte 0x0200b084
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0x030800bf
	.4byte 0x0200b084
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0x030900bf
	.4byte 0x0200b084
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00024000
	.4byte 0x030b00bf
	.4byte 0x0200b084
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00024000
	.4byte 0x03070016
	.4byte 0x0200b0bc
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0x03080016
	.4byte 0x0200b0bc
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x01024000
	.4byte 0x03090016
	.4byte 0x0200b0bc
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x01024000
	.4byte 0x030b0016
	.4byte 0x0200b0bc
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0x00000202
	.4byte 0xffff0014
	.4byte 0x02008f95
	.4byte 0x0000c602
	.4byte 0xffff0019
	.4byte 0x0200903d
	.4byte 0x00004602
	.4byte 0xffff001a
	.4byte 0x02009079
	.4byte 0x00000003
	.4byte 0xffff001b
	.4byte 0x0200912d
	.4byte 0x00008e15
	.4byte 0x0300000d
	.4byte 0x02009151
	.4byte 0x00008e15
	.4byte 0x0301000e
	.4byte 0x02009245
	.4byte 0x00008e15
	.4byte 0x0302000f
	.4byte 0x02009329
	.4byte 0x00009415
	.4byte 0x0fd40010
	.4byte 0x02009421
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0x00000202
	.4byte 0xffff0014
	.4byte 0x02009455
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x02009521
	.4byte 0x00008e15
	.4byte 0x0303000f
	.4byte 0x020095d1
	.4byte 0x00008e15
	.4byte 0x03040010
	.4byte 0x020096f1
	.4byte 0x00008e15
	.4byte 0x03050011
	.4byte 0x02009819
	.4byte 0x00008e15
	.4byte 0x03060011
	.4byte 0x02009881
	.4byte 0x00000013
	.4byte 0x0f1b0064
	.4byte 0x001000c1
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008cc1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0x00000202
	.4byte 0xffff0014
	.4byte 0x020099b9
	.4byte 0x00008602
	.4byte 0xffff0019
	.4byte 0x02009ac9
	.4byte 0x00000002
	.4byte 0x089d0016
	.4byte 0x0200a1b1
	.4byte 0x00000002
	.4byte 0x089e0017
	.4byte 0x0200a2c1
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte 0x02009b85
	.4byte 0x00008c15
	.4byte 0x0214000e
	.4byte 0x02009c35
	.4byte 0x00008e15
	.4byte 0x03070013
	.4byte 0x02009d05
	.4byte 0x00008e15
	.4byte 0x03080014
	.4byte 0x02009de1
	.4byte 0x00008e15
	.4byte 0x03090015
	.4byte 0x02009ef1
	.4byte 0x00008e15
	.4byte 0x030a0016
	.4byte 0x0200a005
	.4byte 0x00008e15
	.4byte 0x030b0016
	.4byte 0x0200a079
	.4byte 0x00000013
	.4byte 0x0f1a0064
	.4byte 0x0010005d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0xffff000b
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x00000013
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
	.4byte 0x00000003
	.4byte 0x03500064
	.4byte 0x00300000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
