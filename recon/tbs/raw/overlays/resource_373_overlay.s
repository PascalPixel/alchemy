.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/ENTRY.INC"
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
	bl 0x0200e160
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
	bl 0x0200dfe4
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
	bl 0x0200df44
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
	bl 0x0200def4
	ldr r5, [pc, #132]
	movs r0, #15
	bl 0x0200de8c
	movs r0, #185
	bl 0x0200e14c
	str r5, [r6, #48]
	str r5, [r6, #52]
	ldr r1, [r7]
.L_020000c4_5:
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl 0x0200df1c
	mov r0, r8
	str r5, [r0, #48]
	str r5, [r0, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl 0x0200df1c
	adds r0, r6, #0
	bl 0x0200df24
	bl 0x0200e144
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
	bl 0x0200def4
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
	.4byte 0x0200e190
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
	bl 0x0200df44
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
	.4byte 0x0200e190
	.4byte 0xffff0000
	.4byte 0x0200e1d0
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
	bl 0x0200dfe4
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
	.4byte 0x0200e1d0
	.4byte 0x0200e1e8
	.4byte 0x0200e190
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
	bl 0x0200df44
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
	.4byte 0x0200e1e8
	.4byte 0x0200e190
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
	bl 0x0200dfe4
	ldrh r3, [r0, #6]
	ldr r0, [sp, #76]
	lsrs r3, r3, #12
	mov r8, r3
	bl 0x0200dfe4
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
	bl 0x0200dfec
	movs r1, #8
	movs r0, #0
	bl 0x0200e044
	movs r0, #15
	bl 0x0200dfb4
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
	bl 0x0200e02c
	movs r0, #0
	bl 0x0200dfe4
	ldr r3, [pc, #424]
	str r3, [r0, #108]
	movs r0, #4
	bl 0x0200dfb4
	mov r3, r8
	subs r3, #6
	cmp r3, #7
	bhi .L_02000608_6
	adds r0, r7, #0
	movs r1, #3
	bl 0x0200def4
	b .L_02000608_7
.L_02000608_6:
	adds r0, r7, #0
	movs r1, #2
	bl 0x0200def4
.L_02000608_7:
	movs r0, #239
	bl 0x0200e14c
	adds r0, r7, #0
	ldr r1, [sp, #80]
	ldr r2, [sp, #84]
	ldr r3, [sp, #88]
	bl 0x0200df1c
	movs r0, #0
	bl 0x0200e034
	movs r0, #0
	movs r1, #2
	bl 0x0200e044
	movs r0, #0
	ldr r1, [pc, #360]
	ldr r2, [pc, #344]
	bl 0x0200dfec
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
	bl 0x0200e02c
	ldr r3, [sp, #92]
	cmp r3, #0
	beq .L_02000608_8
	bl 0x0200e160
.L_02000608_8:
	movs r0, #0
	bl 0x0200e034
	movs r1, #1
	movs r0, #0
	bl 0x0200e044
	movs r0, #0
	bl 0x0200dfe4
	movs r2, #0
	str r2, [r0, #108]
	adds r0, r7, #0
	bl 0x0200df24
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200e14c
	movs r0, #213
	bl 0x0200e14c
	ldr r3, [sp, #80]
	str r3, [r7, #8]
	ldr r3, [sp, #88]
	str r3, [r7, #16]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	adds r0, r7, #0
	movs r1, #1
	bl 0x0200def4
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
	bl 0x0200df3c
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
	bl 0x0200df3c
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
	bl 0x0200e144
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
	.2byte 0xe1e8
	.2byte 0x0200
	.2byte 0x1999
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0x0001
	.2byte 0x82a9
	.2byte 0x0200
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0xe190
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
	bl 0x0200dfe4
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
	bl 0x0200df3c
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
	.2byte 0xe1d0
	.2byte 0x0200
	.4byte 0x0200e1e8
	.global Func_020009dc
	.thumb_func
Func_020009dc:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200e708
	.global Func_020009e4
	.thumb_func
Func_020009e4:
	movs r0, #0
	bx lr
	.global Func_020009e8
	.thumb_func
Func_020009e8:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200e870
	.global Func_020009f0
	.thumb_func
Func_020009f0:
	push {lr}
	ldr r3, [pc, #52]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #16
	bne .L_020009f0_0
	ldr r0, [pc, #40]
	b .L_020009f0_1
.L_020009f0_0:
	ldr r0, [pc, #40]
	bl 0x0200df9c
	cmp r0, #0
	beq .L_020009f0_2
	ldr r0, [pc, #32]
	b .L_020009f0_1
.L_020009f0_2:
	ldr r0, [pc, #32]
	bl 0x0200df9c
	cmp r0, #0
	beq .L_020009f0_3
	ldr r0, [pc, #28]
	b .L_020009f0_1
.L_020009f0_3:
	ldr r0, [pc, #28]
.L_020009f0_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x0200ee48
	.4byte 0x0000087a
	.4byte 0x0200ecc8
	.4byte 0x00000815
	.4byte 0x0200eab8
	.4byte 0x0200e8a8
	.global Func_02000a44
	.thumb_func
Func_02000a44:
	push {lr}
	bl 0x0200dfbc
	movs r2, #0
	movs r1, #0
	movs r0, #26
	bl 0x0200e03c
	movs r0, #253
	lsls r0, r0, #4
	bl 0x0200dfa4
	movs r0, #181
	movs r1, #3
	bl 0x0200e11c
	movs r1, #0
	movs r0, #181
	bl 0x0200dfd4
	bl 0x0200dfc4
	pop {r0}
	bx r0
	.global Func_02000a74
	.thumb_func
Func_02000a74:
	push {lr}
	bl 0x0200dfbc
	movs r2, #0
	movs r1, #0
	movs r0, #20
.L_02000a80:
	bl 0x0200e03c
	movs r0, #253
	lsls r0, r0, #4
	bl 0x0200dfa4
	movs r0, #181
	movs r1, #3
	bl 0x0200e11c
	movs r1, #0
	movs r0, #181
	bl 0x0200dfd4
	bl 0x0200dfc4
	pop {r0}
	bx r0
	.global Func_02000aa4
	.thumb_func
Func_02000aa4:
	push {lr}
	ldr r0, [pc, #32]
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02000aa4_0
	ldr r0, [pc, #24]
	b .L_02000aa4_1
.L_02000aa4_0:
	ldr r0, [pc, #24]
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02000aa4_2
	ldr r0, [pc, #20]
	b .L_02000aa4_1
.L_02000aa4_2:
	ldr r0, [pc, #20]
.L_02000aa4_1:
	pop {r1}
	bx r1
	.4byte 0x0000087a
	.4byte 0x0200f334
	.4byte 0x00000815
	.4byte 0x0200f100
	.4byte 0x0200ef38
	.global Func_02000adc
	.thumb_func
Func_02000adc:
	push {lr}
	bl 0x0200dfbc
	ldr r0, [pc, #56]
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02000adc_0
	ldr r0, [pc, #48]
	bl 0x0200e084
	movs r0, #10
	movs r1, #0
	bl 0x0200e094
	b .L_02000adc_1
.L_02000adc_0:
	ldr r0, [pc, #36]
	bl 0x0200e084
	movs r0, #10
	movs r1, #0
	movs r2, #4
	bl 0x0200e074
	movs r0, #10
	movs r1, #0
	bl 0x0200e0a4
.L_02000adc_1:
	bl 0x0200dfc4
	pop {r0}
	bx r0
	.4byte 0x00000815
	.4byte 0x000011cc
	.4byte 0x00000f81
	.global Func_02000b28
	.thumb_func
Func_02000b28:
	push {lr}
	bl 0x0200dfbc
	ldr r0, [pc, #100]
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02000b28_0
	ldr r0, [pc, #92]
	bl 0x0200e084
	movs r0, #14
	movs r1, #0
	bl 0x0200e094
	b .L_02000b28_1
.L_02000b28_0:
	ldr r0, [pc, #80]
	bl 0x0200df9c
	cmp r0, #0
	bne .L_02000b28_2
	ldr r0, [pc, #72]
	bl 0x0200dfa4
	ldr r0, [pc, #68]
	bl 0x0200e084
	movs r0, #14
	movs r1, #0
	movs r2, #4
	bl 0x0200e074
	movs r0, #14
	movs r1, #0
	bl 0x0200e0a4
	b .L_02000b28_1
.L_02000b28_2:
	ldr r0, [pc, #48]
	bl 0x0200e084
	movs r0, #14
	movs r1, #0
	movs r2, #4
	bl 0x0200e074
	movs r0, #14
	movs r1, #0
	bl 0x0200e094
.L_02000b28_1:
	bl 0x0200dfc4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000815
	.4byte 0x000011c9
	.4byte 0x00000806
	.4byte 0x00000f7c
	.4byte 0x00000f7e
	.global Func_02000ba8
	.thumb_func
Func_02000ba8:
	push {lr}
	bl 0x0200dfbc
	ldr r0, [pc, #160]
	bl 0x0200df9c
	cmp r0, #0
	bne 0x02008c2e
	ldr r0, [pc, #148]
	bl 0x0200dfa4
	ldr r0, [pc, #148]
	bl 0x0200e084
	movs r0, #18
	ldr r1, [pc, #144]
	movs r2, #0
	bl 0x0200e0c4
	movs r0, #0
	movs r1, #18
	movs r2, #20
	bl 0x0200e074
	movs r0, #18
	movs r1, #0
	movs r2, #6
	bl 0x0200e09c
	movs r1, #128
	movs r0, #18
	lsls r1, r1, #8
	movs r2, #30
	bl 0x0200e0ac
	movs r0, #18
.L_02000bf0:
	movs r1, #2
	movs r2, #20
	bl 0x0200e054
	movs r0, #18
	movs r1, #0
	movs r2, #6
	bl 0x0200e09c
.L_02000c02:
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl 0x0200e074
	movs r0, #18
	ldr r1, [pc, #72]
	movs r2, #0
	bl 0x0200e0c4
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl 0x0200e09c
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200e0c4
	b .L_02000c02_0
	.2byte 0x490a
	.2byte 0x2200
	.2byte 0x2012
	.2byte 0xf005
	.2byte 0xfa46
	.2byte 0x4808
	.2byte 0xf005
	.2byte 0xfa23
	.2byte 0x2012
	.2byte 0x2100
	.2byte 0x2214
	.2byte 0xf005
	.2byte 0xfa2a
.L_02000c02_0:
	bl 0x0200dfc4
	pop {r0}
	bx r0
	.2byte 0x0807
	.2byte 0x0000
	.2byte 0x0f63
	.2byte 0x0000
	.4byte 0x00000103
	.2byte 0x0f66
	.2byte 0x0000
	.global Func_02000c60
	.thumb_func
Func_02000c60:
	push {lr}
	bl 0x0200dfbc
	ldr r0, [pc, #40]
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02000c60_0
	ldr r0, [pc, #32]
	bl 0x0200e084
	b .L_02000c60_1
.L_02000c60_0:
	ldr r0, [pc, #28]
	bl 0x0200e084
.L_02000c60_1:
	movs r0, #21
	movs r1, #0
	bl 0x0200e094
	bl 0x0200dfc4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000202
	.4byte 0x00000f68
	.4byte 0x00000f69
	.global Func_02000c9c
	.thumb_func
Func_02000c9c:
	push {lr}
	bl 0x0200dfbc
	movs r2, #20
	movs r1, #10
	movs r0, #0
	bl 0x0200e074
	ldr r0, [pc, #24]
	bl 0x0200e084
	movs r1, #0
	movs r0, #10
	bl 0x0200e094
	ldr r0, [pc, #16]
	bl 0x0200dfa4
	bl 0x0200dfc4
	pop {r0}
	bx r0
	.4byte 0x00001c8d
	.4byte 0x0000081f
	.global Func_02000cd0
	.thumb_func
Func_02000cd0:
	push {lr}
	bl 0x0200dfbc
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200df54
	movs r0, #10
	bl 0x0200de8c
	movs r0, #1
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #48]
	negs r0, r0
	bl 0x0200df54
	ldr r0, [pc, #44]
	bl 0x0200e084
	movs r0, #17
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r2, #20
	movs r0, #17
	movs r1, #0
	bl 0x0200e06c
	movs r0, #17
	movs r1, #0
	bl 0x0200e094
	bl 0x0200dfc4
	pop {r0}
	bx r0
	.4byte 0x0000e666
	.4byte 0x00001c9a
	.global Func_02000d2c
	.thumb_func
Func_02000d2c:
	push {lr}
	bl 0x0200dfbc
	movs r1, #2
	movs r0, #19
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r2, #20
	movs r1, #0
	movs r0, #19
	bl 0x0200e06c
	ldr r0, [pc, #28]
	bl 0x0200e084
	movs r1, #0
	movs r0, #19
	bl 0x0200e0a4
	ldr r0, [pc, #16]
	bl 0x0200dfa4
	bl 0x0200dfc4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001c9d
	.4byte 0x00000307
	.global Func_02000d70
	.thumb_func
Func_02000d70:
	push {lr}
	movs r0, #132
	lsls r0, r0, #2
	sub sp, #8
	bl 0x0200dfa4
	movs r3, #10
	movs r2, #84
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #40
	movs r1, #84
	movs r2, #7
	movs r3, #4
	bl 0x0200df3c
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000d98
	.thumb_func
Func_02000d98:
	push {lr}
	movs r0, #132
	lsls r0, r0, #2
	sub sp, #8
	bl 0x0200dfac
	movs r3, #10
	movs r2, #84
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #40
	movs r1, #89
	movs r2, #7
	movs r3, #4
	bl 0x0200df3c
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000dc0
	.thumb_func
Func_02000dc0:
	push {lr}
	movs r0, #188
	bl 0x0200e14c
	ldr r0, [pc, #28]
	movs r1, #45
	movs r2, #11
	bl 0x0200df2c
	movs r2, #210
	movs r0, #0
	ldr r1, [pc, #20]
	lsls r2, r2, #1
	bl 0x0200e01c
	movs r0, #11
	bl 0x0200e0fc
	pop {r0}
	bx r0
	.4byte 0x0200f544
	.4byte 0x00000101
	.global Func_02000df0
	.thumb_func
Func_02000df0:
	push {lr}
	ldr r0, [pc, #32]
	bl 0x0200df9c
	cmp r0, #0
	bne .L_02000df0_0
	bl 0x02008fec
	b .L_02000df0_1
.L_02000df0_0:
	movs r0, #123
	bl 0x0200e14c
	movs r0, #1
	bl 0x0200e0fc
.L_02000df0_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000801
	.global Func_02000e18
	.thumb_func
Func_02000e18:
	push {lr}
	movs r0, #123
	bl 0x0200e14c
	movs r0, #3
	bl 0x0200e0fc
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000e2c
	.thumb_func
Func_02000e2c:
	push {lr}
	movs r0, #123
	bl 0x0200e14c
	movs r0, #4
	bl 0x0200e0fc
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000e40
	.thumb_func
Func_02000e40:
	push {lr}
	movs r0, #123
	bl 0x0200e14c
	movs r0, #2
	bl 0x0200e0fc
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000e54
	.thumb_func
Func_02000e54:
	push {lr}
	movs r0, #158
	bl 0x0200e14c
	ldr r0, [pc, #28]
	movs r1, #54
	movs r2, #32
	bl 0x0200df2c
	movs r1, #203
	movs r0, #0
	lsls r1, r1, #1
	ldr r2, [pc, #16]
	bl 0x0200e01c
	movs r0, #5
	bl 0x0200e0fc
	pop {r0}
	bx r0
	.4byte 0x0200f55a
	.4byte 0x000002d7
	.global Func_02000e84
	.thumb_func
Func_02000e84:
	push {lr}
	movs r0, #158
	bl 0x0200e14c
	ldr r0, [pc, #28]
	movs r1, #45
	movs r2, #39
	bl 0x0200df2c
	movs r1, #131
	movs r0, #0
	lsls r1, r1, #1
	ldr r2, [pc, #16]
	bl 0x0200e01c
	movs r0, #6
	bl 0x0200e0fc
	pop {r0}
	bx r0
	.4byte 0x0200f570
	.4byte 0x00000325
	.global Func_02000eb4
	.thumb_func
Func_02000eb4:
	push {lr}
	ldr r0, [pc, #144]
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02000eb4_0
	ldr r0, [pc, #136]
	bl 0x0200df9c
	cmp r0, #0
	bne .L_02000eb4_0
	bl 0x0200dfbc
	ldr r0, [pc, #128]
	bl 0x0200e084
	movs r1, #0
	movs r0, #21
	bl 0x0200e08c
	movs r0, #0
	movs r1, #0
	bl 0x0200dfdc
	cmp r0, #0
	bne .L_02000eb4_1
	movs r0, #21
	movs r1, #0
	movs r2, #60
	bl 0x0200e09c
	movs r0, #21
	movs r1, #0
	bl 0x0200e094
	b .L_02000eb4_2
.L_02000eb4_1:
	ldr r3, [pc, #84]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	movs r0, #40
	strh r3, [r2]
	bl 0x0200dfb4
	movs r0, #21
	movs r1, #0
	bl 0x0200e094
.L_02000eb4_2:
	bl 0x0200dfc4
	b .L_02000eb4_3
.L_02000eb4_0:
	movs r0, #158
	bl 0x0200e14c
	ldr r0, [pc, #48]
	movs r1, #50
	movs r2, #44
	bl 0x0200df2c
	movs r1, #170
	movs r2, #222
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e01c
	movs r0, #7
	bl 0x0200e0fc
.L_02000eb4_3:
	pop {r0}
	bx r0
	.4byte 0x00000815
	.4byte 0x0000087a
	.4byte 0x000011b6
	.4byte 0x03001ebc
	.4byte 0x0200f55a
	.global Func_02000f5c
	.thumb_func
Func_02000f5c:
	push {lr}
	movs r0, #158
	bl 0x0200e14c
	ldr r0, [pc, #28]
	movs r1, #49
	movs r2, #69
	bl 0x0200df2c
	movs r1, #163
	movs r0, #0
	lsls r1, r1, #1
	ldr r2, [pc, #16]
	bl 0x0200e01c
	movs r0, #8
	bl 0x0200e0fc
	pop {r0}
	bx r0
	.4byte 0x0200f570
	.4byte 0x00000466
	.global Func_02000f8c
	.thumb_func
Func_02000f8c:
	push {lr}
	movs r0, #158
	bl 0x0200e14c
	ldr r0, [pc, #28]
	movs r1, #52
	movs r2, #76
	bl 0x0200df2c
	movs r1, #187
	movs r0, #0
	lsls r1, r1, #1
	ldr r2, [pc, #16]
	bl 0x0200e01c
	movs r0, #9
	bl 0x0200e0fc
	pop {r0}
	bx r0
	.4byte 0x0200f586
	.4byte 0x000004d6
	.global Func_02000fbc
	.thumb_func
Func_02000fbc:
	push {lr}
	movs r0, #158
	bl 0x0200e14c
	ldr r0, [pc, #28]
	movs r1, #35
	movs r2, #74
	bl 0x0200df2c
	movs r0, #0
	movs r1, #102
	ldr r2, [pc, #20]
	bl 0x0200e01c
	movs r0, #10
	bl 0x0200e0fc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200f55a
	.4byte 0x000004b6
	.global Func_02000fec
	.thumb_func
Func_02000fec:
	push {r5, r6, lr}
	movs r0, #0
	bl 0x0200dfe4
	adds r6, r0, #0
	movs r0, #5
	bl 0x0200dfe4
	adds r5, r0, #0
	bl 0x0200dfbc
	ldr r3, [r6, #8]
	str r3, [r5, #8]
	ldr r3, [r6, #12]
	str r3, [r5, #12]
	ldr r3, [r6, #16]
	str r3, [r5, #16]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #56]
	str r3, [r5, #60]
	str r3, [r5, #64]
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #40]
	str r3, [r5, #44]
	ldr r3, [r6, #12]
	movs r0, #1
	str r3, [r5, #20]
	bl 0x0200de8c
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200dfec
	movs r0, #5
	movs r1, #110
	ldr r2, [pc, #132]
	bl 0x0200e024
	movs r2, #2
	movs r0, #0
	movs r1, #5
	bl 0x0200e074
	ldr r0, [pc, #120]
	bl 0x0200e084
	ldr r2, [r6, #8]
	ldr r3, [r5, #8]
	cmp r2, r3
	bge .L_02000fec_0
	ldr r0, [pc, #112]
	movs r1, #0
	movs r2, #2
	bl 0x0200e09c
	b .L_02000fec_1
.L_02000fec_0:
	ldr r0, [pc, #104]
	movs r1, #0
	movs r2, #2
	bl 0x0200e09c
.L_02000fec_1:
	movs r1, #3
	movs r0, #0
	bl 0x0200e04c
	movs r0, #2
	bl 0x0200dfb4
	movs r0, #5
	movs r1, #2
	bl 0x0200e044
	movs r0, #0
	bl 0x0200dfe4
	cmp r0, #0
	beq .L_02000fec_2
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl 0x0200e00c
.L_02000fec_2:
	movs r0, #5
	bl 0x0200e034
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x0200e03c
	movs r0, #0
	movs r1, #110
	ldr r2, [pc, #32]
	bl 0x0200e024
	bl 0x0200dfc4
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000011b
	.4byte 0x00000f39
	.4byte 0x0000a005
	.4byte 0x00008005
	.4byte 0x0000012f
	.global Func_020010d8
	.thumb_func
Func_020010d8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r0, [pc, #220]
	sub sp, #12
	bl 0x0200df9c
	cmp r0, #0
	bne .L_020010d8_0
	ldr r3, [pc, #212]
	ldr r3, [r3]
	mov r8, r3
	bl 0x0200dfbc
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #0
	lsls r1, r1, #9
	bl 0x0200dfec
	movs r1, #1
	movs r0, #0
	bl 0x0200e044
	movs r0, #2
	bl 0x0200dfb4
	ldr r0, [pc, #180]
	bl 0x0200e084
	movs r0, #15
	movs r1, #0
	movs r2, #2
	bl 0x0200e09c
	movs r2, #2
	movs r0, #16
	movs r1, #0
	bl 0x0200e09c
	movs r0, #0
	bl 0x0200dfe4
	ldr r3, [r0, #8]
	mov r7, sp
	str r3, [r7]
	ldr r3, [r0, #12]
	str r3, [r7, #4]
	ldr r3, [r0, #16]
	mov r2, r8
	ldr r2, [r2]
	str r3, [r7, #8]
	mov r3, r8
	str r7, [r3]
	mov r10, r2
	movs r6, #0
	adds r5, r7, #0
.L_020010d8_1:
	ldr r3, [r5, #8]
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r5, #8]
	movs r0, #1
	adds r6, #1
	bl 0x0200dfb4
	bl 0x0200df14
	cmp r6, #40
	bne .L_020010d8_1
	movs r0, #60
	bl 0x0200dfb4
	ldr r0, [pc, #92]
	movs r1, #1
	bl 0x0200df7c
	movs r0, #6
	bl 0x0200dfb4
	movs r6, #0
	adds r5, r7, #0
.L_020010d8_2:
	ldr r3, [r5, #8]
	ldr r2, [pc, #76]
	adds r3, r3, r2
	str r3, [r5, #8]
	movs r0, #1
	adds r6, #1
	bl 0x0200dfb4
	bl 0x0200df14
	cmp r6, #40
	bne .L_020010d8_2
	mov r3, r10
	mov r2, r8
	str r3, [r2]
	movs r0, #60
	bl 0x0200dfb4
	movs r0, #0
	movs r1, #70
	ldr r2, [pc, #40]
	bl 0x0200e024
	bl 0x0200dfc4
.L_020010d8_0:
	sub sp, #-12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00000808
	.4byte 0x03001e70
	.4byte 0x00000f4d
	.4byte 0x00000f4f
	.4byte 0xfffe0000
	.4byte 0x000002e5
	.global Func_020011d8
	.thumb_func
Func_020011d8:
	push {r5, lr}
	ldr r0, [pc, #92]
	bl 0x0200df9c
	cmp r0, #0
	bne .L_020011d8_0
	bl 0x0200dfbc
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #0
	bl 0x0200dfec
	ldr r5, [pc, #68]
	adds r0, r5, #0
	bl 0x0200e084
	movs r0, #15
	movs r1, #0
	movs r2, #2
	bl 0x0200e09c
	adds r5, #2
	movs r2, #2
	movs r0, #16
	movs r1, #0
	bl 0x0200e09c
	movs r1, #1
	adds r0, r5, #0
	bl 0x0200df7c
	movs r0, #6
	bl 0x0200dfb4
	movs r0, #0
	movs r1, #69
	ldr r2, [pc, #24]
	bl 0x0200e024
	bl 0x0200dfc4
.L_020011d8_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000808
	.4byte 0x00000f4d
	.4byte 0x00000366
	.global Func_02001244
	.thumb_func
Func_02001244:
	push {lr}
	sub sp, #8
	bl 0x0200dfbc
	movs r3, #20
	movs r2, #50
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #53
	movs r2, #8
	movs r3, #4
	bl 0x0200df3c
	movs r1, #10
	movs r2, #11
	movs r3, #1
	movs r0, #0
	bl 0x0200b2b0
	movs r0, #129
	lsls r0, r0, #2
	bl 0x0200dfa4
	bl 0x0200dfc4
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001280
	.thumb_func
Func_02001280:
	push {lr}
	sub sp, #8
	bl 0x0200dfbc
	movs r1, #13
	movs r2, #10
	movs r3, #1
	movs r0, #0
	bl 0x0200b380
	movs r0, #129
	lsls r0, r0, #2
	bl 0x0200dfac
	movs r3, #20
	movs r2, #50
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #46
	movs r2, #8
	movs r3, #4
	bl 0x0200df3c
	bl 0x0200dfc4
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020012bc
	.thumb_func
Func_020012bc:
	push {r5, r6, lr}
	movs r0, #22
	bl 0x0200dfe4
	adds r5, r0, #0
	bl 0x0200dfbc
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #10
	bl 0x0200dfec
	movs r0, #0
	movs r1, #5
	movs r2, #0
	bl 0x0200e054
	adds r5, #90
	movs r0, #0
	movs r1, #215
	ldr r2, [pc, #320]
	bl 0x0200e01c
	ldrb r3, [r5]
	movs r6, #1
	orrs r3, r6
	movs r1, #166
	strb r3, [r5]
	movs r0, #22
	lsls r1, r1, #16
	ldr r2, [pc, #304]
	bl 0x0200e03c
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200e0ac
	ldrb r3, [r5]
	movs r1, #160
	eors r3, r6
	movs r2, #160
	strb r3, [r5]
	movs r0, #22
	lsls r1, r1, #10
	lsls r2, r2, #10
	bl 0x0200dfec
	movs r0, #22
	movs r1, #4
	movs r2, #0
	bl 0x0200e054
	ldr r2, [pc, #260]
	movs r0, #22
	movs r1, #202
	bl 0x0200e024
	movs r1, #1
	movs r0, #0
	bl 0x0200e044
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #176
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #192
	movs r0, #22
	lsls r1, r1, #6
	movs r2, #24
	bl 0x0200e0ac
	movs r1, #128
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r1, #2
	movs r0, #0
	bl 0x0200e05c
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	movs r0, #22
	lsls r1, r1, #9
	bl 0x0200dfec
	ldr r1, [pc, #176]
	movs r0, #0
	bl 0x0200dff4
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #0
	movs r0, #22
	ldr r1, [pc, #164]
	bl 0x0200e0c4
	ldr r1, [pc, #160]
	movs r0, #22
	bl 0x0200dff4
	movs r0, #0
	bl 0x0200dffc
	movs r1, #128
	movs r2, #237
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200e024
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200e0ac
	movs r0, #22
	bl 0x0200dffc
	movs r1, #128
	movs r2, #228
	lsls r2, r2, #1
	movs r0, #22
	lsls r1, r1, #1
	bl 0x0200e024
	movs r0, #0
	movs r1, #1
	bl 0x0200e044
	movs r1, #128
	movs r2, #20
	movs r0, #22
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r1, #2
	movs r0, #22
	bl 0x0200e05c
	movs r0, #20
	bl 0x0200dfb4
	ldr r0, [pc, #72]
	bl 0x0200e084
	movs r1, #0
	movs r0, #22
	bl 0x0200e0a4
	movs r0, #22
	bl 0x0200dfe4
	ldr r3, [pc, #56]
	ldr r1, [pc, #56]
	str r3, [r0, #108]
	movs r0, #22
	bl 0x0200dff4
	ldr r0, [pc, #52]
	bl 0x0200dfa4
	bl 0x0200dfc4
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000193
	.4byte 0x01770000
	.4byte 0x0000018b
	.4byte 0x0200f59c
	.4byte 0x00000103
	.4byte 0x0200f5ec
	.4byte 0x00000fce
	.4byte 0x0200d72d
	.4byte 0x0200e248
	.4byte 0x00000823
	.global Func_02001454
	.thumb_func
Func_02001454:
	push {r5, lr}
	movs r0, #22
	bl 0x0200dfe4
	adds r5, r0, #0
	ldr r0, [pc, #36]
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02001454_0
	adds r3, r5, #0
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_02001454_0
	ldr r0, [pc, #16]
	ldr r1, [pc, #20]
	bl 0x02009490
.L_02001454_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000823
	.4byte 0x0200f63c
	.4byte 0x0200f6cc
	.global Func_02001490
	.thumb_func
Func_02001490:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r5, r0, #0
	movs r0, #22
	mov r8, r1
	bl 0x0200dfe4
	adds r6, r0, #0
	bl 0x0200dfbc
	movs r0, #22
	movs r1, #2
	bl 0x0200e05c
	movs r1, #128
	movs r2, #20
	movs r0, #22
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r0, #0
	movs r1, #2
	bl 0x0200e05c
	movs r1, #129
	movs r2, #40
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200e0c4
	adds r1, r5, #0
	movs r0, #0
	bl 0x0200dff4
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #0
	movs r0, #22
	ldr r1, [pc, #96]
	bl 0x0200e0c4
	mov r1, r8
	movs r0, #22
	bl 0x0200e004
	movs r0, #0
	bl 0x0200dffc
	movs r5, #128
	movs r0, #20
	bl 0x0200dfb4
	lsls r5, r5, #9
	movs r1, #2
	movs r0, #22
	bl 0x0200e064
	str r5, [r6, #24]
	str r5, [r6, #28]
	movs r0, #0
	bl 0x0200dfe4
	str r5, [r0, #24]
	str r5, [r0, #28]
	ldr r0, [pc, #48]
	bl 0x0200e084
	movs r1, #0
	movs r0, #22
	bl 0x0200e0a4
	movs r0, #22
	bl 0x0200dfe4
	ldr r3, [pc, #32]
	ldr r1, [pc, #36]
	str r3, [r0, #108]
	movs r0, #22
	bl 0x0200dff4
	bl 0x0200dfc4
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000103
	.4byte 0x00000fce
	.4byte 0x0200d72d
	.4byte 0x0200e248
	.global Func_02001554
	.thumb_func
Func_02001554:
	push {r5, lr}
	movs r0, #22
	bl 0x0200dfe4
	adds r5, r0, #0
	ldr r0, [pc, #36]
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02001554_0
	adds r3, r5, #0
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_02001554_0
	ldr r0, [pc, #16]
	ldr r1, [pc, #20]
	bl 0x02009490
.L_02001554_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000823
	.4byte 0x0200f748
	.4byte 0x0200f7c4
	.global Func_02001590
	.thumb_func
Func_02001590:
	push {r5, lr}
	movs r0, #22
	bl 0x0200dfe4
	adds r5, r0, #0
	ldr r0, [pc, #48]
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02001590_0
	adds r3, r5, #0
	adds r3, #100
	movs r2, #0
	ldrsh r0, [r3, r2]
	cmp r0, #1
	bne .L_02001590_1
	ldr r0, [pc, #28]
	ldr r1, [pc, #32]
	bl 0x02009490
	b .L_02001590_0
.L_02001590_1:
	cmp r0, #2
	bne .L_02001590_0
	ldr r0, [pc, #16]
	ldr r1, [pc, #20]
	bl 0x02009490
.L_02001590_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000823
	.4byte 0x0200f748
	.4byte 0x0200f6cc
	.4byte 0x0200f7c4
	.global Func_020015dc
	.thumb_func
Func_020015dc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #36
	bl 0x0200dfbc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl 0x0200e0e4
	bl 0x0200e0f4
	movs r1, #0
	adds r6, r0, #0
	mov r8, r1
	adds r3, r6, #0
	adds r3, #85
	mov r2, r8
	movs r1, #160
	lsls r1, r1, #16
	strb r2, [r3]
	ldr r0, [pc, #1016]
	ldr r2, [pc, #1020]
	movs r3, #0
	bl 0x0200e0e4
	movs r0, #1
	bl 0x0200dfb4
	bl 0x0200df14
	movs r3, #20
	movs r2, #50
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #41
	movs r2, #7
	movs r3, #3
	bl 0x0200df3c
	movs r3, #2
	str r3, [sp, #0]
	movs r5, #1
	mov r9, r3
	movs r0, #2
	movs r1, #102
	movs r2, #84
	movs r3, #41
	str r5, [sp, #4]
	bl 0x0200df34
	movs r0, #1
	movs r1, #102
	movs r2, #83
	movs r3, #41
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200df34
	movs r1, #103
	movs r2, #82
	movs r3, #42
	movs r0, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200df34
	movs r0, #0
	bl 0x0200dfe4
	adds r6, r0, #0
	adds r1, r6, #0
	adds r1, #85
	str r1, [sp, #28]
	ldrb r2, [r1]
	mov r3, r8
	str r2, [sp, #32]
	movs r0, #0
	strb r3, [r1]
	ldr r2, [pc, #908]
	ldr r1, [pc, #908]
	bl 0x0200e03c
	movs r1, #196
	movs r2, #224
	movs r0, #21
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl 0x0200e03c
	movs r1, #149
	movs r2, #184
	movs r0, #1
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl 0x0200e03c
	movs r1, #149
	movs r2, #190
	movs r0, #5
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl 0x0200e03c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r0, #0
	movs r1, #11
	bl 0x0200e044
	ldr r1, [pc, #808]
	movs r0, #0
	mov r10, r1
	bl 0x0200dff4
	movs r0, #23
	movs r1, #2
	movs r2, #1
	bl 0x0200b45c
	ldr r2, [pc, #792]
	movs r1, #224
	mov r11, r2
	ldr r2, [r2]
	lsls r1, r1, #1
	adds r3, r2, r1
	mov r1, r8
	str r1, [r3]
	movs r3, #228
	lsls r3, r3, #1
	adds r2, r2, r3
	movs r3, #32
	str r3, [r2]
	bl 0x0200e104
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200dfec
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200dfec
	ldr r1, [pc, #740]
	movs r0, #5
	bl 0x0200dff4
	ldr r1, [pc, #736]
	movs r0, #1
	bl 0x0200dff4
	movs r7, #128
	movs r0, #0
	movs r1, #1
	bl 0x0200dff4
	lsls r7, r7, #9
	movs r1, #176
	movs r2, #40
	movs r0, #0
	lsls r1, r1, #8
	str r7, [r6, #24]
	str r7, [r6, #28]
	bl 0x0200e0ac
	movs r1, #3
	movs r0, #0
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #0
	ldr r1, [pc, #688]
	ldr r2, [pc, #688]
	bl 0x0200dfec
	movs r1, #202
	lsls r1, r1, #1
	ldr r2, [pc, #684]
	movs r0, #0
	bl 0x0200e024
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #192
	movs r2, #30
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #1
	movs r0, #0
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #128
	movs r2, #40
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #2
	movs r0, #0
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #0
	movs r1, #2
	bl 0x0200e07c
	movs r0, #23
	movs r1, #2
	bl 0x0200e07c
	movs r0, #23
	ldr r1, [pc, #592]
	ldr r2, [pc, #592]
	bl 0x0200dfec
	movs r1, #195
	movs r2, #208
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #23
	bl 0x0200e014
	movs r0, #80
	bl 0x0200dfb4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #201
	movs r2, #207
	lsls r2, r2, #2
	lsls r1, r1, #1
	movs r0, #23
	bl 0x0200e014
	movs r0, #80
	bl 0x0200dfb4
	movs r0, #0
	movs r1, #0
	bl 0x0200e07c
	movs r0, #23
	movs r1, #0
	bl 0x0200e07c
	movs r1, #195
	ldr r2, [pc, #524]
	movs r0, #23
	lsls r1, r1, #17
	bl 0x0200e03c
	movs r0, #0
	movs r1, #11
	bl 0x0200e044
	mov r1, r10
	movs r0, #0
	bl 0x0200dff4
	movs r0, #200
	bl 0x0200dfb4
	mov r1, r9
	str r1, [sp, #0]
	movs r3, #41
	movs r2, #84
	movs r0, #7
	movs r1, #102
	str r5, [sp, #4]
	bl 0x0200df34
	movs r0, #0
	movs r1, #1
	bl 0x0200dff4
	movs r1, #1
	movs r0, #0
	str r7, [r6, #24]
	str r7, [r6, #28]
	bl 0x0200e044
	movs r0, #30
	bl 0x0200dfb4
	movs r1, #3
	movs r0, #0
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	ldr r1, [pc, #440]
	ldr r2, [pc, #428]
	movs r0, #0
	bl 0x0200e024
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #30
	bl 0x0200e0ac
	movs r2, #20
	movs r0, #0
	movs r1, #0
	bl 0x0200e0ac
	movs r0, #0
	movs r1, #2
	bl 0x0200e07c
	movs r0, #23
	movs r1, #2
	bl 0x0200e07c
	movs r0, #23
	ldr r1, [pc, #368]
	ldr r2, [pc, #368]
	bl 0x0200dfec
	movs r1, #195
	movs r2, #208
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #23
	bl 0x0200e014
	movs r0, #80
	bl 0x0200dfb4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r2, #207
	lsls r2, r2, #2
	ldr r1, [pc, #340]
	movs r0, #23
	bl 0x0200e014
	movs r0, #80
	bl 0x0200dfb4
	movs r0, #0
	movs r1, #0
	bl 0x0200e07c
	movs r0, #23
	movs r1, #0
	bl 0x0200e07c
	movs r2, #0
	movs r0, #23
	movs r1, #0
	bl 0x0200e03c
	movs r0, #0
	movs r1, #11
	bl 0x0200e044
	mov r1, r10
	movs r0, #0
	bl 0x0200dff4
	movs r0, #200
	bl 0x0200dfb4
	movs r3, #41
	movs r2, #83
	movs r0, #6
	movs r1, #102
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200df34
	movs r0, #0
	movs r1, #1
	bl 0x0200dff4
	movs r1, #1
	movs r0, #0
	str r7, [r6, #24]
	str r7, [r6, #28]
	bl 0x0200e044
	movs r0, #30
	bl 0x0200dfb4
	movs r1, #3
	movs r0, #0
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #180
	movs r0, #0
	lsls r1, r1, #1
	ldr r2, [pc, #220]
	bl 0x0200e024
	movs r1, #176
	movs r0, #21
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200e0ac
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #30
	bl 0x0200e0ac
	movs r1, #208
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r0, #0
	movs r1, #2
	bl 0x0200e07c
	movs r0, #24
	movs r1, #2
	bl 0x0200e07c
	movs r0, #24
	ldr r1, [pc, #140]
	ldr r2, [pc, #140]
	bl 0x0200dfec
	movs r1, #195
	movs r2, #208
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #24
	bl 0x0200e014
	movs r0, #80
	bl 0x0200dfb4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #180
	ldr r2, [pc, #120]
	lsls r1, r1, #1
	movs r0, #24
	bl 0x0200e014
	movs r0, #80
	bl 0x0200dfb4
	movs r0, #0
	movs r1, #0
	bl 0x0200e07c
	movs r0, #24
	movs r1, #0
	bl 0x0200e07c
	movs r2, #0
	movs r0, #24
	movs r1, #0
	bl 0x0200e03c
	movs r0, #0
	movs r1, #11
	bl 0x0200e044
	mov r1, r10
	movs r0, #0
	bl 0x0200dff4
	movs r0, #200
	bl 0x0200dfb4
	b .L_020015dc_0
	.2byte 0x0000
	.4byte 0x017f0000
	.4byte 0x036d0000
	.4byte 0x02b20000
	.4byte 0x01970000
	.4byte 0x0200e590
	.4byte 0x03001ebc
	.4byte 0x0200e614
	.4byte 0x0200e5cc
	.4byte 0x00004ccc
	.4byte 0x00002666
	.4byte 0x0000034b
	.4byte 0x034a0000
	.4byte 0x00000179
	.4byte 0x00000357
	.4byte 0x0000034a
.L_020015dc_0:
	movs r3, #42
	movs r2, #82
	movs r0, #5
	movs r1, #103
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200df34
	movs r1, #1
	movs r0, #0
	bl 0x0200dff4
	ldr r0, [pc, #1004]
	str r7, [r6, #24]
	str r7, [r6, #28]
	bl 0x0200e084
	movs r0, #21
	movs r1, #2
	movs r2, #20
	bl 0x0200e054
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #5
	movs r2, #20
	bl 0x0200e0ac
	movs r3, #0
	movs r0, #21
	movs r1, #5
	movs r2, #6
	bl 0x0200b2b0
	movs r0, #21
	ldr r1, [pc, #952]
	ldr r2, [pc, #956]
	bl 0x0200dfec
	movs r2, #208
	ldr r1, [pc, #952]
	lsls r2, r2, #2
	movs r0, #21
	bl 0x0200e024
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #60
	bl 0x0200e0ac
	movs r1, #192
	movs r2, #60
	movs r0, #21
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #3
	bl 0x0200e04c
	movs r1, #3
	movs r0, #21
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #186
	movs r2, #208
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #21
	bl 0x0200e024
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #40
	bl 0x0200e0ac
	movs r1, #128
	movs r2, #40
	movs r0, #21
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #3
	bl 0x0200e04c
	movs r1, #3
	movs r0, #21
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #160
	movs r2, #30
	movs r0, #21
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r1, #3
	movs r0, #21
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r1, #2
	movs r0, #0
	bl 0x0200e064
	movs r0, #40
	bl 0x0200dfb4
	movs r1, #4
	movs r0, #21
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #0
	movs r0, #21
	bl 0x0200e08c
	movs r0, #0
	movs r1, #0
	bl 0x0200dfdc
	cmp r0, #0
	bne .L_020015dc_1
	mov r3, r11
	ldr r2, [r3]
	movs r1, #236
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_020015dc_1:
	movs r1, #4
	movs r0, #21
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #0
	movs r2, #20
	movs r0, #21
	bl 0x0200e09c
	ldr r0, [pc, #684]
	bl 0x0200e084
	movs r1, #193
	lsls r1, r1, #1
	ldr r2, [pc, #676]
	movs r0, #21
	bl 0x0200e024
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #208
	movs r2, #60
	movs r0, #21
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #2
	movs r0, #21
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #160
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #30
	bl 0x0200e0ac
	movs r1, #0
	movs r0, #21
	bl 0x0200e08c
	movs r0, #0
	movs r1, #0
	bl 0x0200dfdc
	cmp r0, #1
	bne .L_020015dc_2
	mov r3, r11
	ldr r2, [r3]
	movs r1, #236
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_020015dc_2:
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #208
	movs r2, #60
	movs r0, #21
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #2
	movs r0, #21
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	ldr r0, [pc, #540]
	bl 0x0200e084
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #193
	ldr r2, [pc, #528]
	lsls r1, r1, #1
	movs r0, #21
	bl 0x0200e024
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #4
	movs r0, #21
	bl 0x0200e04c
	movs r0, #60
	bl 0x0200dfb4
	movs r0, #21
	movs r1, #0
	movs r2, #60
	bl 0x0200e09c
	movs r1, #160
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #186
	movs r2, #208
	movs r0, #21
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r1, #160
	movs r2, #20
	movs r0, #21
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r1, #2
	movs r0, #0
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #3
	movs r0, #21
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #129
	movs r2, #60
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r1, #2
	movs r0, #21
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #0
	bl 0x0200e04c
	movs r0, #30
	bl 0x0200dfb4
	movs r1, #3
	movs r0, #21
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r2, #10
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	ldr r0, [pc, #332]
	ldr r1, [pc, #332]
	bl 0x0200e0dc
	movs r1, #160
	movs r2, #215
	movs r3, #1
	ldr r0, [pc, #324]
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200e0e4
	movs r2, #128
	adds r1, r7, #0
	movs r0, #5
	lsls r2, r2, #8
	bl 0x0200dfec
	movs r2, #128
	adds r1, r7, #0
	movs r0, #1
	lsls r2, r2, #8
	bl 0x0200dfec
	movs r2, #226
	movs r0, #1
	ldr r1, [pc, #292]
	lsls r2, r2, #2
	bl 0x0200e01c
	movs r1, #196
	movs r2, #226
	lsls r2, r2, #2
	movs r0, #5
	lsls r1, r1, #1
	bl 0x0200e024
	movs r0, #1
	movs r1, #1
	bl 0x0200e044
	movs r3, #0
	movs r0, #5
	movs r1, #10
	movs r2, #11
	bl 0x0200b2b0
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl 0x0200e09c
	movs r1, #2
	movs r0, #21
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #5
	movs r2, #30
	bl 0x0200e0ac
	movs r0, #5
	movs r1, #4
	movs r2, #0
	bl 0x0200e054
	movs r1, #196
	movs r0, #5
	lsls r1, r1, #1
	ldr r2, [pc, #176]
	bl 0x0200e024
	movs r1, #144
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #192
	movs r0, #21
.L_02001dea:
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #208
	movs r2, #40
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #3
	movs r0, #21
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #5
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r2, #20
	movs r0, #5
	movs r1, #0
	bl 0x0200e09c
	movs r0, #21
	movs r1, #3
	bl 0x0200e044
	movs r1, #3
	movs r0, #0
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r3, #0
	movs r0, #1
	movs r1, #10
	movs r2, #11
	b .L_02001dea_0
	.2byte 0x0f03
	.2byte 0x0000
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0x2666
	.2byte 0x0000
	.2byte 0x018d
	.2byte 0x0000
	.2byte 0x0f0a
	.2byte 0x0000
	.2byte 0x0349
	.2byte 0x0000
	.2byte 0x0f0e
	.2byte 0x0000
	.2byte 0x0339
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x0ccc
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0179
	.2byte 0x0171
	.2byte 0x0000
	.2byte 0x034b
	.2byte 0x0000
.L_02001dea_0:
	bl 0x0200b2b0
	movs r0, #5
	ldr r1, [pc, #1008]
	ldr r2, [pc, #1008]
	bl 0x0200dfec
	movs r0, #1
	ldr r1, [pc, #996]
	ldr r2, [pc, #1000]
	bl 0x0200dfec
	movs r1, #196
	lsls r1, r1, #1
	ldr r2, [pc, #992]
	movs r0, #1
	bl 0x0200e01c
	movs r0, #5
	bl 0x0200dfe4
	adds r0, #90
	ldrb r2, [r0]
	movs r7, #254
	adds r3, r7, #0
	ands r3, r2
	movs r1, #204
	strb r3, [r0]
	lsls r1, r1, #1
	ldr r2, [pc, #964]
	movs r0, #5
	bl 0x0200e024
	movs r0, #1
	bl 0x0200dfb4
	movs r0, #5
	bl 0x0200dfe4
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	movs r1, #128
	strb r3, [r0]
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #5
	bl 0x0200e0ac
	movs r0, #1
	bl 0x0200e034
	movs r0, #1
	movs r1, #1
	bl 0x0200e044
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #30
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #4
	movs r2, #30
	bl 0x0200e054
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #1
	bl 0x0200e04c
	movs r0, #30
	bl 0x0200dfb4
	movs r1, #160
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #30
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #208
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r2, #30
	movs r0, #0
	movs r1, #2
	bl 0x0200e054
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #0
	bl 0x0200e0cc
	movs r0, #60
	bl 0x0200dfb4
	movs r1, #192
	movs r2, #40
	movs r0, #21
	lsls r1, r1, #6
	bl 0x0200e0ac
	movs r0, #1
	movs r1, #2
	bl 0x0200e064
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r0, #21
	ldr r1, [pc, #772]
	movs r2, #80
	bl 0x0200e0c4
	movs r1, #160
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #30
	bl 0x0200e0ac
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #80
	bl 0x0200e0c4
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #208
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200e0ac
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #1
	bl 0x0200e04c
	movs r0, #100
	bl 0x0200dfb4
	movs r2, #30
	movs r0, #5
	movs r1, #1
	bl 0x0200e074
	movs r0, #1
	movs r1, #2
	bl 0x0200e05c
	movs r1, #2
	movs r0, #5
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #21
	ldr r1, [pc, #664]
	movs r2, #60
	bl 0x0200e0c4
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #128
	movs r2, #30
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #2
	movs r0, #5
	bl 0x0200e064
	movs r0, #30
	bl 0x0200dfb4
	movs r2, #20
	movs r0, #5
	movs r1, #0
	bl 0x0200e09c
	movs r1, #2
	movs r0, #21
	bl 0x0200e05c
	movs r0, #30
	bl 0x0200dfb4
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r0, #1
	movs r1, #3
	bl 0x0200e044
	movs r1, #3
	movs r0, #5
	bl 0x0200e04c
	movs r0, #30
	bl 0x0200dfb4
	movs r1, #4
	movs r0, #21
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r0, #1
	movs r1, #3
	bl 0x0200e044
	movs r1, #3
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #3
	movs r0, #21
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r0, #1
	movs r1, #3
	bl 0x0200e044
	movs r1, #3
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #2
	movs r0, #21
	bl 0x0200e064
	movs r0, #30
	bl 0x0200dfb4
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	ldr r0, [pc, #440]
	ldr r1, [pc, #440]
	bl 0x0200e0dc
	movs r1, #160
	movs r3, #1
	ldr r0, [pc, #436]
	lsls r1, r1, #16
	ldr r2, [pc, #436]
	bl 0x0200e0e4
	movs r1, #182
	movs r2, #204
	movs r0, #21
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r1, #208
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #6
	movs r2, #30
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #0
	movs r2, #40
	bl 0x0200e09c
	movs r2, #30
	movs r0, #5
	movs r1, #1
	bl 0x0200e074
	movs r0, #1
	movs r1, #2
	bl 0x0200e05c
	movs r1, #2
	movs r0, #5
	bl 0x0200e064
	movs r0, #30
	bl 0x0200dfb4
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #30
	bl 0x0200e0ac
	movs r1, #160
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #30
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #0
	movs r2, #30
	bl 0x0200e09c
	movs r2, #60
	movs r0, #0
	ldr r1, [pc, #284]
	bl 0x0200e0c4
	movs r1, #4
	movs r0, #21
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #0
	movs r0, #21
	bl 0x0200e08c
	movs r0, #0
	movs r1, #0
	movs r5, #0
	bl 0x0200dfdc
	cmp r0, #1
	bne .L_02001dea_1
	ldr r3, [pc, #260]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001dea_1:
	movs r0, #40
	bl 0x0200dfb4
	movs r1, #0
	movs r2, #20
	movs r0, #21
	bl 0x0200e09c
	ldr r0, [pc, #232]
	bl 0x0200e084
	movs r2, #0
	movs r0, #21
	ldr r1, [pc, #228]
	bl 0x0200e0c4
	movs r1, #3
	movs r0, #21
	bl 0x0200e064
	movs r0, #30
	bl 0x0200dfb4
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r2, #0
	movs r0, #21
	movs r1, #4
	bl 0x0200e054
	movs r0, #21
	movs r1, #3
	bl 0x0200e064
	movs r1, #7
	movs r0, #21
	bl 0x0200e044
	movs r0, #5
	bl 0x0200dfb4
	movs r3, #14
	movs r1, #1
	movs r0, #10
	movs r4, #4
	str r1, [sp, #4]
	str r0, [sp, #8]
	str r3, [sp, #12]
	str r3, [sp, #20]
	movs r2, #2
	movs r1, #14
	movs r3, #24
	movs r0, #21
	str r2, [sp, #0]
	str r5, [sp, #24]
	str r4, [sp, #16]
	bl 0x0200e0b4
	movs r0, #21
	bl 0x0200dfe4
	adds r6, r0, #0
	ldr r3, [r6, #80]
	adds r1, r6, #0
	adds r1, #90
	ldrb r2, [r1]
	adds r3, #38
	strb r5, [r3]
	adds r3, r7, #0
	ands r3, r2
	strb r3, [r1]
	movs r2, #192
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200dfec
	movs r1, #182
	movs r0, #21
	lsls r1, r1, #1
	ldr r2, [pc, #88]
	bl 0x0200e024
	movs r0, #4
	bl 0x0200dfb4
	movs r5, #0
.L_02001dea_3:
	ldr r3, [r6, #16]
	movs r1, #192
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r6, #16]
	ldr r2, [pc, #68]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	movs r0, #1
	adds r5, #1
	bl 0x0200dfb4
	b .L_02001dea_2
	.2byte 0x0000
	.4byte 0x00004ccc
	.4byte 0x00002666
	.4byte 0x0000034b
	.4byte 0x00000101
	.4byte 0x00000105
	.4byte 0x00009999
	.4byte 0x00001333
	.4byte 0x01750000
	.4byte 0x03450000
	.4byte 0x03001ebc
	.4byte 0x00000f27
	.4byte 0x00000103
	.4byte 0x0000032f
	.4byte 0xffffe667
.L_02001dea_2:
	cmp r5, #4
	bne .L_02001dea_3
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl 0x0200e03c
	movs r1, #192
	movs r2, #192
	movs r0, #1
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200dfec
	movs r0, #1
	movs r1, #6
	movs r2, #0
	bl 0x0200e054
	movs r1, #187
	ldr r2, [pc, #1012]
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200e024
	movs r0, #5
	movs r1, #0
	bl 0x0200e094
	movs r1, #176
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r0, #5
	movs r1, #2
	bl 0x0200e05c
	movs r1, #128
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r0, #1
	movs r1, #13
	bl 0x0200e044
	movs r0, #1
	movs r1, #2
	movs r2, #5
	bl 0x0200e054
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl 0x0200df54
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r2, #83
	movs r3, #41
	movs r1, #102
	movs r0, #1
	bl 0x0200df34
	movs r0, #1
	bl 0x0200dfe4
	movs r1, #0
	bl 0x0200df4c
	movs r1, #208
	movs r2, #10
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r0, #1
	movs r1, #3
	bl 0x0200e05c
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #864]
	bl 0x0200df54
	bl 0x0200df5c
	movs r1, #129
	movs r2, #80
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r0, #21
	movs r1, #8
	bl 0x0200e044
	movs r3, #128
	lsls r3, r3, #8
	movs r1, #182
	str r3, [r6, #28]
	movs r0, #21
	lsls r1, r1, #17
	ldr r2, [pc, #828]
	bl 0x0200e03c
	movs r5, #0
.L_02001dea_4:
	ldr r3, [r6, #28]
	ldr r1, [pc, #820]
	adds r3, r3, r1
	str r3, [r6, #28]
	movs r0, #1
	adds r5, #1
	bl 0x0200dfb4
	cmp r5, #5
	bne .L_02001dea_4
	movs r0, #60
	bl 0x0200dfb4
	movs r0, #1
	movs r1, #2
	bl 0x0200e064
	movs r1, #160
	movs r2, #30
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r0, #1
	movs r1, #2
	bl 0x0200e05c
	movs r1, #2
	movs r0, #5
	bl 0x0200e064
	movs r0, #60
	bl 0x0200dfb4
	movs r1, #2
	movs r0, #21
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	ldr r0, [pc, #736]
	ldr r1, [pc, #736]
	bl 0x0200e0dc
	movs r0, #186
	movs r1, #160
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #16
	ldr r2, [pc, #724]
	bl 0x0200e0e4
	movs r1, #192
	movs r2, #192
	movs r0, #21
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200dfec
	movs r0, #21
	movs r1, #6
	movs r2, #0
	bl 0x0200e054
	ldr r1, [pc, #700]
	ldr r2, [pc, #700]
	movs r0, #21
	bl 0x0200e024
	movs r0, #30
	bl 0x0200dfb4
	movs r1, #128
	movs r2, #30
	movs r0, #21
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r1, #2
	movs r0, #21
	bl 0x0200e064
	movs r0, #30
	bl 0x0200dfb4
	movs r0, #21
	bl 0x0200dfe4
	adds r6, r0, #0
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r1]
	movs r0, #21
	movs r1, #0
	movs r2, #80
	bl 0x0200e09c
	movs r0, #21
	ldr r1, [pc, #632]
	movs r2, #80
	bl 0x0200e0c4
	movs r2, #60
	movs r0, #21
	movs r1, #0
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #3
	bl 0x0200e064
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #21
	bl 0x0200e0cc
	movs r0, #80
	bl 0x0200dfb4
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #30
	bl 0x0200e0ac
	movs r1, #129
	movs r2, #80
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r1, #2
	movs r0, #1
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #128
	movs r2, #30
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #3
	movs r0, #1
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #3
	movs r0, #1
	bl 0x0200e064
	movs r0, #1
	bl 0x0200dfe4
	movs r1, #1
	bl 0x0200df4c
	movs r2, #0
	movs r0, #1
	movs r1, #6
	bl 0x0200e054
	movs r0, #1
	movs r1, #1
	bl 0x0200e044
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #10
	movs r0, #1
	bl 0x0200dfec
	movs r0, #1
	bl 0x0200dfe4
	adds r6, r0, #0
	adds r2, r6, #0
	adds r2, #90
	ldrb r3, [r2]
	ands r5, r3
	strb r5, [r2]
	movs r0, #1
	ldr r2, [pc, #396]
	ldr r1, [pc, #436]
	bl 0x0200e00c
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	bl 0x0200e0cc
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200e0ac
	movs r1, #0
	movs r2, #1
	movs r0, #5
	bl 0x0200e09c
	movs r0, #1
	bl 0x0200e034
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200e0ac
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #128
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r0, #1
	movs r1, #13
	bl 0x0200e044
	movs r2, #5
	movs r1, #2
	movs r0, #1
	bl 0x0200e054
	movs r0, #1
	bl 0x0200dfe4
	movs r1, #0
	bl 0x0200df4c
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #41
	movs r0, #2
	movs r1, #102
	movs r2, #84
	bl 0x0200df34
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	movs r0, #0
	lsls r1, r1, #11
	bl 0x0200df54
	movs r0, #1
	movs r1, #3
	bl 0x0200e064
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #240]
	bl 0x0200df54
	bl 0x0200df5c
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #30
	bl 0x0200e0c4
	movs r0, #5
	ldr r1, [pc, #228]
	ldr r2, [pc, #256]
	bl 0x0200dfec
	movs r1, #204
	ldr r2, [pc, #252]
	lsls r1, r1, #1
	movs r0, #5
	bl 0x0200e024
	movs r0, #60
	bl 0x0200dfb4
	movs r0, #1
	movs r1, #2
	bl 0x0200e064
	movs r2, #60
	movs r0, #21
	ldr r1, [pc, #228]
	bl 0x0200e0c4
	movs r0, #5
	movs r1, #3
	bl 0x0200e05c
	movs r1, #3
	movs r0, #0
	bl 0x0200e064
	movs r0, #80
	bl 0x0200dfb4
	movs r1, #128
	movs r2, #30
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r1, #3
	movs r0, #1
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #4
	movs r0, #5
	bl 0x0200e04c
	movs r0, #80
	bl 0x0200dfb4
	movs r1, #3
	movs r0, #21
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #176
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #0
	movs r2, #60
	bl 0x0200e0ac
	movs r1, #128
	movs r2, #60
	movs r0, #21
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r1, #4
	movs r0, #21
	bl 0x0200e04c
	movs r0, #60
	bl 0x0200dfb4
	movs r0, #21
	movs r1, #0
	movs r2, #80
	bl 0x0200e0ac
	movs r0, #21
	ldr r1, [pc, #64]
	movs r2, #80
	bl 0x0200e0c4
	movs r0, #21
	movs r1, #0
	b .L_02001dea_5
	.4byte 0x0000033b
	.4byte 0x0000e666
	.4byte 0x032b0000
	.4byte 0x00001999
	.4byte 0x00004ccc
	.4byte 0x00000999
	.4byte 0x035b0000
	.4byte 0x00000167
	.4byte 0x00000343
	.4byte 0x00000101
	.4byte 0x00000193
	.4byte 0x00002666
	.4byte 0x00000357
	.4byte 0x00000105
.L_02001dea_5:
	movs r2, #60
	bl 0x0200e09c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r0, #0
	ldr r1, [pc, #780]
	movs r2, #0
	bl 0x0200e0c4
	movs r0, #5
	ldr r1, [pc, #772]
	movs r2, #0
	bl 0x0200e0c4
	movs r2, #60
	movs r0, #1
	ldr r1, [pc, #760]
	bl 0x0200e0c4
	movs r1, #4
	movs r0, #21
	bl 0x0200e04c
	movs r0, #30
	bl 0x0200dfb4
	movs r2, #70
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r0, #1
	movs r1, #2
	bl 0x0200e05c
	movs r1, #2
	movs r0, #5
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #60
	bl 0x0200e0ac
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #30
	bl 0x0200e0ac
	movs r2, #30
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r2, #30
	movs r0, #21
	movs r1, #0
	bl 0x0200e0ac
	movs r1, #4
	movs r0, #21
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #5
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #3
	movs r0, #1
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #208
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #128
	movs r2, #0
	movs r0, #21
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r1, #3
	movs r0, #21
	bl 0x0200e064
	movs r0, #30
	bl 0x0200dfb4
	movs r2, #60
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r0, #1
	movs r1, #3
	bl 0x0200e064
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	lsls r1, r1, #9
	movs r0, #1
	bl 0x0200dfec
	movs r0, #1
	bl 0x0200dfe4
	movs r1, #0
	bl 0x0200df4c
	movs r0, #1
	movs r1, #4
	movs r2, #0
	bl 0x0200e054
	movs r1, #199
	movs r2, #207
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #1
	bl 0x0200e024
	movs r0, #60
	bl 0x0200dfb4
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #192
	movs r2, #60
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #3
	movs r0, #0
	bl 0x0200e04c
	movs r0, #60
	bl 0x0200dfb4
	movs r1, #3
	movs r0, #21
	bl 0x0200e04c
	movs r0, #60
	bl 0x0200dfb4
	movs r0, #1
	bl 0x0200dfe4
	adds r6, r0, #0
	adds r2, r6, #0
	adds r2, #90
	ldrb r3, [r2]
	movs r5, #1
	orrs r3, r5
	strb r3, [r2]
	movs r0, #5
	bl 0x0200dfe4
	adds r6, r0, #0
	adds r2, r6, #0
	adds r2, #90
	ldrb r3, [r2]
	orrs r3, r5
	strb r3, [r2]
	movs r0, #0
	bl 0x0200dfe4
	movs r1, #128
	movs r2, #128
	adds r6, r0, #0
	lsls r1, r1, #9
	movs r0, #1
	lsls r2, r2, #8
	bl 0x0200dfec
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200dfec
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl 0x0200e0ac
	movs r2, #10
	ldrsh r1, [r6, r2]
	movs r0, #5
	movs r3, #18
	ldrsh r2, [r6, r3]
	adds r1, #16
	bl 0x0200e01c
	movs r2, #10
	ldrsh r1, [r6, r2]
	movs r3, #18
	ldrsh r2, [r6, r3]
	adds r1, #16
	subs r2, #16
	movs r0, #1
.L_0200290a:
	bl 0x0200e024
	movs r0, #1
	bl 0x0200e034
	movs r1, #160
	movs r2, #30
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r0, #1
	movs r1, #3
	bl 0x0200e044
	movs r0, #5
	movs r1, #3
	bl 0x0200e044
	movs r1, #3
	movs r0, #0
	bl 0x0200e04c
	movs r0, #40
	bl 0x0200dfb4
	movs r2, #10
	ldrsh r1, [r6, r2]
	movs r0, #5
	movs r3, #18
	ldrsh r2, [r6, r3]
	bl 0x0200e024
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl 0x0200e03c
	movs r2, #10
	ldrsh r1, [r6, r2]
	movs r0, #1
	movs r3, #18
	ldrsh r2, [r6, r3]
	bl 0x0200e024
	movs r2, #0
	movs r0, #1
	movs r1, #0
	bl 0x0200e03c
	movs r0, #1
	movs r1, #5
	bl 0x0200dfcc
	movs r1, #160
	ldr r0, [pc, #192]
	lsls r1, r1, #16
	ldr r2, [pc, #192]
	movs r3, #1
	bl 0x0200e0e4
	movs r3, #0
	movs r0, #0
	movs r1, #13
	movs r2, #10
	bl 0x0200b380
	movs r1, #188
	movs r2, #228
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200e0ac
	movs r0, #21
	bl 0x0200dfe4
	adds r0, #90
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r3, #0
	movs r0, #21
	movs r1, #6
	movs r2, #5
	bl 0x0200b380
	movs r0, #21
	ldr r1, [pc, #124]
	ldr r2, [pc, #124]
	bl 0x0200e024
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #192
	movs r2, #40
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #3
	bl 0x0200e044
	movs r1, #3
	movs r0, #0
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #1
	movs r0, #0
	bl 0x0200e0d4
	bl 0x0200e0ec
	movs r0, #100
.L_02002a0a:
	bl 0x0200dfb4
	ldr r0, [pc, #60]
	bl 0x0200dfa4
	ldr r0, [pc, #56]
	bl 0x0200dfac
	add r1, sp, #32
	ldr r2, [sp, #28]
	ldrb r1, [r1]
	strb r1, [r2]
	bl 0x0200dfc4
	sub sp, #-36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0101
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0179
	.2byte 0x0000
	.2byte 0x0377
	.2byte 0x0175
	.2byte 0x0000
	.2byte 0x0377
	.2byte 0x0000
	.4byte 0x00000202
	.4byte 0x0000012f
.L_02002a54:
	.global Func_02002a54
	.thumb_func
Func_02002a54:
	push {r5, lr}
	ldr r2, [pc, #544]
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	sub sp, #8
	cmp r3, #16
	bne .L_02002a54_0
	ldr r1, [pc, #528]
	adds r3, r2, r1
	adds r1, #1
	ldrb r0, [r3]
	adds r3, r2, r1
	ldrb r1, [r3]
	bl 0x0200df94
	bl 0x0200b4c8
	b .L_02002a54_1
.L_02002a54_0:
	movs r0, #253
	lsls r0, r0, #4
	bl 0x0200df9c
	cmp r0, #0
	bne .L_02002a54_2
	ldr r0, [pc, #500]
	bl 0x0200df9c
	cmp r0, #0
	bne .L_02002a54_3
	movs r0, #26
	bl 0x0200db48
	b .L_02002a54_2
.L_02002a54_3:
	movs r0, #20
	bl 0x0200db48
.L_02002a54_2:
	movs r3, #2
	str r3, [sp, #0]
	movs r5, #1
	movs r0, #2
	movs r1, #102
	movs r2, #84
	movs r3, #41
	str r5, [sp, #4]
	bl 0x0200df34
	movs r1, #102
	movs r2, #83
	movs r3, #41
	movs r0, #1
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200df34
	ldr r0, [pc, #440]
	bl 0x0200df9c
	adds r3, r0, #0
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
	adds r0, #20
	bl 0x0200dfe4
	movs r1, #0
	adds r5, r0, #0
	bl 0x0200df4c
	movs r0, #197
	lsls r0, r0, #2
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02002a54_4
	movs r3, #181
	b .L_02002a54_5
.L_02002a54_4:
	ldr r0, [pc, #400]
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02002a54_6
	movs r3, #197
	b .L_02002a54_5
.L_02002a54_6:
	movs r3, #189
.L_02002a54_5:
	lsls r3, r3, #17
	str r3, [r5, #8]
	movs r3, #146
	lsls r3, r3, #18
	str r3, [r5, #16]
	movs r3, #192
	lsls r3, r3, #16
	str r3, [r5, #12]
	bl 0x0200d950
	adds r1, r5, #0
	adds r1, #34
	movs r3, #3
	strb r3, [r1]
	adds r3, r5, #0
	movs r2, #0
	adds r3, #85
	movs r1, #200
	strb r2, [r3]
	ldr r0, [pc, #348]
	lsls r1, r1, #4
	bl 0x0200de94
	ldr r0, [pc, #332]
	bl 0x0200df9c
	cmp r0, #0
	bne .L_02002a54_7
	ldr r0, [pc, #336]
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02002a54_8
	movs r0, #21
	bl 0x0200dfe4
	adds r5, r0, #0
	movs r0, #21
	bl 0x0200dfe4
	movs r1, #0
	bl 0x0200df4c
	ldr r3, [pc, #308]
	str r3, [r5, #24]
	str r3, [r5, #28]
.L_02002a54_8:
	ldr r0, [pc, #308]
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02002a54_9
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl 0x0200e03c
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl 0x0200e03c
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl 0x0200e03c
.L_02002a54_9:
	ldr r0, [pc, #260]
	bl 0x0200df9c
	adds r5, r0, #0
	cmp r5, #0
	bne .L_02002a54_10
	ldr r0, [pc, #260]
	bl 0x0200df9c
	cmp r0, #0
	bne .L_02002a54_11
	ldr r0, [pc, #252]
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02002a54_12
	movs r1, #128
	movs r2, #228
	lsls r1, r1, #17
	movs r0, #22
	lsls r2, r2, #17
	bl 0x0200e03c
	movs r0, #22
	bl 0x0200dfe4
	ldr r3, [pc, #228]
	ldr r1, [pc, #228]
	str r3, [r0, #108]
	movs r0, #22
	bl 0x0200dff4
	b .L_02002a54_12
.L_02002a54_11:
	movs r0, #22
	bl 0x0200dfe4
	adds r0, #91
	strb r5, [r0]
	ldr r0, [pc, #212]
	bl 0x0200dfac
.L_02002a54_12:
	ldr r3, [pc, #156]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #16
	beq .L_02002a54_10
	ldr r0, [pc, #148]
	bl 0x0200df9c
	cmp r0, #0
	bne .L_02002a54_10
	movs r1, #200
	ldr r0, [pc, #180]
	lsls r1, r1, #4
	bl 0x0200de94
.L_02002a54_10:
	movs r0, #194
	lsls r0, r0, #2
	bl 0x0200df9c
	cmp r0, #0
	bne .L_02002a54_7
	ldr r3, [pc, #108]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #17
	bne .L_02002a54_7
	bl 0x0200bfb0
	movs r0, #194
	lsls r0, r0, #2
	bl 0x0200dfa4
.L_02002a54_7:
	ldr r0, [pc, #112]
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02002a54_13
	movs r0, #129
	lsls r0, r0, #2
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02002a54_14
	movs r3, #20
	movs r2, #50
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #53
	movs r2, #8
	movs r3, #4
	bl 0x0200df3c
.L_02002a54_14:
	movs r0, #132
	lsls r0, r0, #2
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02002a54_13
	bl 0x02008d70
.L_02002a54_13:
	movs r0, #170
	bl 0x0200e13c
	bl 0x0200df14
	movs r0, #1
	bl 0x0200de8c
.L_02002a54_1:
	movs r0, #0
	sub sp, #-8
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000205
	.4byte 0x0000087a
	.4byte 0x00000316
	.4byte 0x0200da95
	.4byte 0x00000815
	.4byte 0x0000028f
	.4byte 0x00000808
	.4byte 0x00000109
	.4byte 0x00000823
	.4byte 0x0200d72d
	.4byte 0x0200e248
	.4byte 0x00000241
	.4byte 0x0200da41
	.global Func_02002cb0
	.thumb_func
Func_02002cb0:
	push {r5, lr}
	bl 0x0200dfbc
	movs r0, #0
	movs r1, #0
	bl 0x0200e07c
	movs r2, #20
	movs r1, #0
	movs r0, #8
	bl 0x0200e06c
	ldr r5, [pc, #548]
	adds r0, r5, #0
	bl 0x0200e084
	movs r0, #8
	movs r1, #2
	bl 0x0200e05c
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200e09c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200e0dc
	movs r0, #199
	movs r1, #1
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	ldr r2, [pc, #504]
	bl 0x0200e0e4
	movs r0, #0
	ldr r1, [pc, #500]
	ldr r2, [pc, #504]
	bl 0x0200dfec
	movs r0, #1
	ldr r1, [pc, #492]
	ldr r2, [pc, #492]
	bl 0x0200dfec
	movs r1, #210
	movs r2, #152
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200e0ac
	movs r0, #0
	bl 0x0200dfe4
	cmp r0, #0
	beq .L_02002cb0_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200e03c
.L_02002cb0_0:
	movs r1, #201
	movs r2, #152
	movs r0, #1
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r1, #208
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200e0ac
	ldr r0, [pc, #408]
	movs r1, #0
	bl 0x0200e094
	movs r1, #160
	movs r2, #20
	movs r0, #8
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r0, #8
	movs r1, #3
	bl 0x0200e04c
	ldr r0, [pc, #384]
	movs r1, #0
	bl 0x0200e094
	movs r0, #0
	movs r1, #3
	bl 0x0200e044
	movs r1, #3
	movs r0, #1
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #8
	movs r1, #2
	bl 0x0200e064
	movs r1, #0
	ldr r0, [pc, #344]
	bl 0x0200e08c
	movs r0, #0
	movs r1, #0
	bl 0x0200dfdc
	cmp r0, #1
	bne .L_02002cb0_1
	ldr r3, [pc, #328]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #8
	movs r1, #1
	bl 0x0200e05c
.L_02002cb0_1:
	ldr r0, [pc, #300]
	movs r1, #0
	movs r2, #40
	bl 0x0200e09c
	ldr r1, [pc, #300]
	movs r2, #60
	movs r0, #8
	bl 0x0200e0c4
	adds r0, r5, #6
	bl 0x0200e084
	movs r2, #20
	ldr r0, [pc, #272]
	movs r1, #0
	bl 0x0200e09c
	movs r1, #1
	movs r0, #1
	bl 0x0200e064
	movs r0, #40
	bl 0x0200dfb4
	movs r2, #40
	ldr r0, [pc, #244]
	movs r1, #0
	bl 0x0200e09c
	movs r0, #8
	movs r1, #1
	bl 0x0200e064
	movs r1, #208
	movs r2, #20
	movs r0, #8
	lsls r1, r1, #8
	bl 0x0200e0ac
	ldr r0, [pc, #220]
	movs r1, #0
	bl 0x0200e094
	movs r0, #1
	movs r1, #3
	bl 0x0200e04c
	ldr r0, [pc, #200]
	movs r1, #0
	movs r2, #120
	bl 0x0200e09c
	ldr r0, [pc, #196]
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r0, #1
	ldr r1, [pc, #192]
	movs r2, #40
	bl 0x0200e0c4
	movs r2, #40
	ldr r0, [pc, #168]
	movs r1, #0
	bl 0x0200e09c
	movs r0, #8
	movs r1, #4
	bl 0x0200e04c
	movs r2, #20
	ldr r0, [pc, #156]
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #1
	bl 0x0200e04c
	movs r0, #40
	bl 0x0200dfb4
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200e0ac
	movs r2, #10
	ldr r0, [pc, #120]
	movs r1, #0
	bl 0x0200e09c
	movs r0, #0
	movs r1, #3
	bl 0x0200e044
	movs r1, #3
	movs r0, #1
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #8
	movs r1, #3
	bl 0x0200e04c
	movs r0, #1
	movs r1, #2
	bl 0x0200e044
	movs r0, #0
	bl 0x0200dfe4
	cmp r0, #0
	beq .L_02002cb0_2
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200e00c
.L_02002cb0_2:
	movs r0, #1
	bl 0x0200e034
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x0200e03c
	ldr r0, [pc, #44]
	bl 0x0200dfa4
	bl 0x0200dfc4
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00001c45
	.4byte 0x02460000
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00001001
	.4byte 0x00004008
	.4byte 0x03001ebc
	.4byte 0x00000105
	.4byte 0x00000303
	.global Func_02002f14
	.thumb_func
Func_02002f14:
	push {lr}
	bl 0x0200dfbc
	movs r1, #1
	movs r3, #1
	ldr r0, [pc, #624]
	negs r1, r1
	ldr r2, [pc, #624]
	bl 0x0200e0e4
	movs r0, #0
	ldr r1, [pc, #620]
	ldr r2, [pc, #620]
	bl 0x0200e024
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r0, #0
	bl 0x0200dfe4
	cmp r0, #0
	beq .L_02002f14_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200e03c
.L_02002f14_0:
	movs r1, #173
	movs r0, #1
	lsls r1, r1, #1
	ldr r2, [pc, #576]
	bl 0x0200e024
	movs r1, #208
	movs r2, #20
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200e0ac
	ldr r0, [pc, #564]
	bl 0x0200e084
	movs r0, #1
	movs r1, #0
	bl 0x0200e094
	movs r0, #9
	movs r1, #2
	bl 0x0200e064
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200e0c4
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #10
	bl 0x0200e0ac
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200e0ac
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #40
	bl 0x0200e0ac
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl 0x0200e09c
	movs r0, #0
	movs r1, #3
	bl 0x0200e044
	movs r0, #1
	movs r1, #3
	bl 0x0200e04c
	movs r0, #9
	movs r1, #1
	bl 0x0200e064
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200e0ac
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl 0x0200e09c
	movs r0, #1
	movs r1, #1
	bl 0x0200e05c
	movs r0, #1
	ldr r1, [pc, #432]
	movs r2, #40
	bl 0x0200e0c4
	movs r2, #10
	movs r0, #1
	movs r1, #0
	bl 0x0200e09c
	movs r0, #9
	movs r1, #3
	bl 0x0200e04c
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #128
	movs r2, #40
	movs r0, #1
	lsls r1, r1, #5
	bl 0x0200e0ac
	movs r0, #9
	movs r1, #4
	bl 0x0200e04c
	movs r0, #9
	movs r1, #0
	bl 0x0200e094
	movs r1, #176
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #208
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #2
	movs r0, #1
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r2, #10
	movs r0, #1
	movs r1, #0
	bl 0x0200e09c
	movs r0, #9
	movs r1, #3
	bl 0x0200e04c
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200e0c4
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #80
	bl 0x0200e0c4
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200e09c
	movs r0, #9
	movs r1, #2
	bl 0x0200e064
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #5
	movs r2, #20
	bl 0x0200e0ac
	movs r1, #0
	movs r0, #1
	bl 0x0200e08c
	movs r0, #0
	movs r1, #0
	bl 0x0200dfdc
	cmp r0, #0
	bne .L_02002f14_1
	movs r0, #1
	ldr r1, [pc, #208]
	movs r2, #60
	bl 0x0200e0c4
	b .L_02002f14_2
.L_02002f14_1:
	ldr r3, [pc, #200]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02002f14_2:
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #208
	movs r2, #10
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200e0ac
	ldr r0, [pc, #168]
	bl 0x0200e084
	movs r0, #1
	movs r1, #0
	bl 0x0200e094
	movs r0, #9
	movs r1, #3
	bl 0x0200e04c
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #1
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #128
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #5
	bl 0x0200e0ac
	movs r0, #1
	movs r1, #3
	bl 0x0200e04c
	movs r0, #0
	movs r1, #3
	bl 0x0200e04c
	movs r0, #1
	movs r1, #2
	bl 0x0200e044
	movs r0, #0
	bl 0x0200dfe4
	cmp r0, #0
	beq .L_02002f14_3
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200e00c
.L_02002f14_3:
	movs r0, #1
	bl 0x0200e034
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x0200e03c
	movs r0, #193
	lsls r0, r0, #2
	bl 0x0200dfa4
	bl 0x0200dfc4
	pop {r0}
	bx r0
	.4byte 0x01650000
	.4byte 0x02e20000
	.4byte 0x0000016f
	.4byte 0x000002e9
	.4byte 0x00001c53
	.4byte 0x00000103
	.4byte 0x00000105
	.4byte 0x03001ebc
	.4byte 0x00001c60
	.global Func_020031b4
	.thumb_func
Func_020031b4:
	push {r5, lr}
	bl 0x0200dfbc
	movs r0, #12
	bl 0x0200dfe4
	movs r1, #0
	bl 0x0200df4c
	movs r0, #13
	bl 0x0200dfe4
	movs r1, #0
	bl 0x0200df4c
	movs r0, #14
	bl 0x0200dfe4
	movs r1, #0
	bl 0x0200df4c
	movs r0, #12
	movs r1, #0
	bl 0x0200e044
	movs r0, #13
	movs r1, #0
	bl 0x0200e044
	movs r1, #0
	movs r0, #14
	bl 0x0200e044
	movs r0, #20
	bl 0x0200de8c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl 0x0200df54
	ldr r5, [pc, #144]
	movs r0, #12
	adds r1, r5, #0
	bl 0x0200dff4
	movs r0, #10
	bl 0x0200de8c
	adds r1, r5, #0
	movs r0, #13
	bl 0x0200dff4
	movs r0, #1
	movs r1, #1
	ldr r2, [pc, #120]
	negs r1, r1
	negs r0, r0
	bl 0x0200df54
	movs r0, #20
	bl 0x0200de8c
	adds r1, r5, #0
	movs r0, #14
	bl 0x0200e004
	movs r1, #128
	movs r2, #40
	movs r0, #11
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r0, #11
	movs r1, #2
	bl 0x0200e064
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #10
	movs r0, #11
	bl 0x0200e0ac
	ldr r0, [pc, #68]
	bl 0x0200e084
	movs r0, #11
	movs r1, #0
	movs r2, #40
	bl 0x0200e09c
	movs r2, #20
	movs r0, #11
	movs r1, #0
	bl 0x0200e06c
	movs r0, #11
	movs r1, #0
	bl 0x0200e094
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #10
	movs r0, #11
	bl 0x0200e0ac
	ldr r0, [pc, #28]
	bl 0x0200dfa4
	bl 0x0200dfc4
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200e65c
	.4byte 0x0000e666
	.4byte 0x00001c90
	.4byte 0x00000305
	.global Func_020032b0
	.thumb_func
Func_020032b0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r7, r0, #0
	mov r9, r3
	mov r8, r1
	mov r10, r2
	bl 0x0200dfe4
	movs r1, #128
	movs r2, #128
	adds r6, r0, #0
	lsls r1, r1, #9
	adds r0, r7, #0
	lsls r2, r2, #8
	ldr r5, [r6, #80]
	bl 0x0200dfec
	movs r1, #196
	adds r0, r7, #0
	lsls r1, r1, #1
	ldr r2, [pc, #140]
	bl 0x0200e024
	movs r1, #192
	movs r2, #10
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200e0ac
	adds r5, #38
	movs r3, #0
	adds r6, #85
	strb r3, [r6]
	strb r3, [r5]
	adds r0, r7, #0
	mov r1, r8
	bl 0x0200e044
	adds r0, r7, #0
	ldr r1, [pc, #104]
	ldr r2, [pc, #108]
	bl 0x0200dfec
	movs r1, #196
	ldr r2, [pc, #104]
	lsls r1, r1, #1
	adds r0, r7, #0
	bl 0x0200e014
	movs r0, #10
	bl 0x0200dfb4
	adds r0, r7, #0
	mov r1, r10
	bl 0x0200e044
	movs r1, #128
	movs r2, #128
	adds r0, r7, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200dfec
	movs r1, #196
	adds r0, r7, #0
	lsls r1, r1, #1
	ldr r2, [pc, #64]
	bl 0x0200e014
	movs r3, #1
	strb r3, [r5]
	mov r3, r9
	cmp r3, #0
	beq .L_020032b0_0
	movs r3, #3
	strb r3, [r6]
.L_020032b0_0:
	movs r0, #10
	bl 0x0200dfb4
	adds r0, r7, #0
	movs r1, #1
	bl 0x0200e044
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000376
	.4byte 0x00004ccc
	.4byte 0x00002666
	.4byte 0x0000036b
	.4byte 0x0000035b
	.global Func_02003380
	.thumb_func
Func_02003380:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	mov r9, r3
	mov r8, r1
	mov r10, r2
	bl 0x0200dfe4
	movs r1, #128
	movs r2, #128
	adds r6, r0, #0
	lsls r1, r1, #9
	adds r0, r7, #0
	lsls r2, r2, #8
	ldr r5, [r6, #80]
	bl 0x0200dfec
	movs r1, #196
	adds r0, r7, #0
	lsls r1, r1, #1
	ldr r2, [pc, #148]
	bl 0x0200e024
	movs r1, #192
	adds r0, r7, #0
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200e0ac
	movs r2, #85
	movs r3, #0
	adds r2, r2, r6
	adds r5, #38
	strb r3, [r2]
	strb r3, [r5]
	adds r0, r7, #0
	mov r1, r8
	mov r11, r2
	bl 0x0200e044
	movs r1, #128
	movs r2, #128
	adds r0, r7, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200dfec
	movs r1, #196
	lsls r1, r1, #1
	ldr r2, [pc, #92]
	adds r0, r7, #0
	bl 0x0200e014
	movs r0, #10
	bl 0x0200dfb4
	ldr r2, [pc, #84]
	adds r0, r7, #0
	ldr r1, [pc, #84]
	bl 0x0200dfec
	adds r0, r7, #0
	mov r1, r10
	bl 0x0200e044
	movs r1, #196
	adds r0, r7, #0
	lsls r1, r1, #1
	ldr r2, [pc, #68]
	bl 0x0200e014
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r6, #40]
	movs r3, #1
	strb r3, [r5]
	mov r3, r9
	cmp r3, #0
	beq .L_02003380_0
	movs r3, #3
	mov r2, r11
	strb r3, [r2]
.L_02003380_0:
	adds r0, r7, #0
	movs r1, #1
	bl 0x0200e044
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000035b
	.4byte 0x0000036b
	.4byte 0x00002666
	.4byte 0x00004ccc
	.4byte 0x0000037a
	.global Func_0200345c
	.thumb_func
Func_0200345c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r1
	adds r6, r0, #0
	cmp r2, #0
	bne .L_0200345c_0
	movs r7, #0
	cmp r7, r8
	bcs .L_0200345c_1
.L_0200345c_2:
	adds r0, r6, #0
	bl 0x0200dfe4
	adds r5, r0, #0
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	movs r1, #0
	bl 0x0200df4c
	movs r3, #195
	lsls r3, r3, #17
	str r3, [r5, #8]
	movs r3, #160
	lsls r3, r3, #16
	str r3, [r5, #12]
	ldr r3, [pc, #48]
	adds r7, #1
	str r3, [r5, #16]
	adds r6, #1
	cmp r7, r8
	bcc .L_0200345c_2
	b .L_0200345c_1
.L_0200345c_0:
	movs r7, #0
	cmp r7, r8
	bcs .L_0200345c_1
.L_0200345c_3:
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0
	adds r7, #1
	bl 0x0200e03c
	adds r6, #1
	cmp r7, r8
	bcc .L_0200345c_3
.L_0200345c_1:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x034a0000
	.global Func_020034c8
	.thumb_func
Func_020034c8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #0
	sub sp, #28
	bl 0x0200dfe4
	mov r9, r0
	movs r0, #14
	bl 0x0200dfe4
	adds r6, r0, #0
	bl 0x0200dfbc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r1, r1
	negs r0, r0
	bl 0x0200e0e4
	movs r0, #1
	bl 0x0200de8c
	bl 0x0200e0f4
	movs r7, #0
	adds r0, #85
	movs r1, #0
	strb r7, [r0]
	movs r0, #1
	mov r10, r1
	bl 0x0200de8c
	movs r3, #20
	movs r2, #50
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #53
	movs r2, #8
	movs r3, #4
	bl 0x0200df3c
	movs r3, #2
	str r3, [sp, #0]
	movs r5, #1
	movs r0, #2
	movs r1, #102
	movs r2, #84
	movs r3, #41
	str r5, [sp, #4]
	bl 0x0200df34
	movs r0, #1
	movs r1, #102
	movs r2, #83
	movs r3, #41
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200df34
	movs r1, #103
	movs r2, #82
	movs r3, #42
	movs r0, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200df34
	movs r0, #11
	bl 0x0200dfe4
	adds r7, r0, #0
	adds r3, r7, #0
	mov r2, r10
	adds r3, #85
	strb r2, [r3]
	movs r3, #160
	lsls r3, r3, #16
	mov r8, r3
	str r3, [r7, #12]
	movs r5, #194
	movs r3, #210
	lsls r5, r5, #17
	lsls r3, r3, #18
	str r3, [r7, #16]
	str r5, [r7, #8]
	movs r1, #0
	bl 0x0200df4c
	movs r0, #12
	bl 0x0200dfe4
	adds r7, r0, #0
	adds r3, r7, #0
	mov r1, r10
	adds r3, #85
	strb r1, [r3]
	movs r3, #211
	mov r2, r8
	lsls r3, r3, #18
	str r2, [r7, #12]
	str r3, [r7, #16]
	str r5, [r7, #8]
	movs r1, #0
	bl 0x0200df4c
	movs r0, #13
	bl 0x0200dfe4
	adds r7, r0, #0
	adds r3, r7, #0
	mov r1, r10
	adds r3, #85
	strb r1, [r3]
	movs r3, #212
	lsls r3, r3, #18
	mov r2, r8
	str r3, [r7, #16]
	str r2, [r7, #12]
	str r5, [r7, #8]
	movs r1, #0
	bl 0x0200df4c
	movs r0, #11
	bl 0x0200dfe4
	movs r1, #0
	bl 0x0200df4c
	movs r0, #12
	bl 0x0200dfe4
	movs r1, #0
	bl 0x0200df4c
	movs r0, #0
	movs r1, #11
	bl 0x0200e044
	ldr r7, [pc, #1016]
	movs r0, #0
	adds r1, r7, #0
	bl 0x0200dff4
	bl 0x0200df64
	ldr r5, [pc, #1004]
	movs r1, #0
	adds r0, r5, #0
	movs r2, #0
	bl 0x0200df84
	bl 0x0200df6c
	ldr r2, [pc, #992]
	movs r3, #0
	mov r1, r8
	ldr r0, [pc, #992]
	bl 0x0200e0e4
	bl 0x0200df14
	movs r0, #1
	bl 0x0200de8c
	ldr r0, [pc, #980]
	ldr r1, [pc, #980]
	bl 0x0200e0dc
	movs r0, #148
	movs r3, #1
	lsls r0, r0, #17
	mov r1, r8
	ldr r2, [pc, #972]
	bl 0x0200e0e4
	ldr r1, [pc, #968]
	ldr r2, [pc, #972]
	movs r0, #5
	bl 0x0200e03c
	movs r0, #1
	bl 0x0200de8c
	movs r0, #5
	ldr r1, [pc, #960]
	ldr r2, [pc, #960]
	bl 0x0200dfec
	movs r1, #210
	movs r0, #5
	lsls r1, r1, #1
	ldr r2, [pc, #952]
	bl 0x0200e01c
	bl 0x0200e134
	ldr r3, [pc, #948]
	ldr r2, [r3]
	movs r3, #228
	lsls r3, r3, #1
	mov r10, r3
	mov r1, r10
	movs r3, #60
	str r3, [r2, r1]
	bl 0x0200e104
	movs r0, #5
	bl 0x0200e034
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200dfec
	movs r2, #133
	movs r0, #5
	ldr r1, [pc, #908]
	lsls r2, r2, #3
	bl 0x0200e024
	movs r0, #5
	ldr r1, [pc, #900]
	ldr r2, [pc, #904]
	bl 0x0200dfec
	movs r0, #5
	ldr r1, [pc, #900]
	ldr r2, [pc, #900]
	bl 0x0200e024
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200dfec
	movs r1, #159
	ldr r2, [pc, #884]
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200e01c
	movs r0, #8
	movs r1, #2
	bl 0x0200e044
	movs r1, #206
	movs r0, #5
	lsls r1, r1, #1
	ldr r2, [pc, #856]
	bl 0x0200e024
	movs r1, #206
	movs r0, #5
	lsls r1, r1, #1
	ldr r2, [pc, #852]
	bl 0x0200e024
	movs r1, #187
	movs r2, #252
	movs r0, #5
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r0, #5
	ldr r1, [pc, #832]
	ldr r2, [pc, #836]
	bl 0x0200e024
	movs r1, #159
	movs r0, #8
	lsls r1, r1, #1
	ldr r2, [pc, #812]
	bl 0x0200e024
	movs r2, #40
	movs r0, #5
	movs r1, #8
	bl 0x0200e074
	movs r0, #8
	movs r1, #2
	bl 0x0200e064
	movs r1, #3
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	ldr r2, [pc, #788]
	movs r0, #8
	ldr r1, [pc, #788]
	bl 0x0200e01c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl 0x0200e0dc
	movs r2, #230
	movs r0, #5
	ldr r1, [pc, #772]
	lsls r2, r2, #2
	bl 0x0200e024
	movs r2, #231
	ldr r1, [pc, #764]
	lsls r2, r2, #2
	movs r0, #5
	bl 0x0200e024
	bl 0x0200e0ec
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #240
	movs r2, #30
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #2
	movs r0, #5
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200e0dc
	movs r3, #1
	ldr r0, [pc, #708]
	mov r1, r8
	ldr r2, [pc, #708]
	bl 0x0200e0e4
	adds r5, #1
	bl 0x0200e0ec
	movs r1, #2
	movs r2, #20
	movs r0, #10
	bl 0x0200e054
	adds r0, r5, #0
	bl 0x0200e084
	ldr r0, [pc, #684]
	movs r1, #0
	movs r2, #10
	bl 0x0200e09c
	movs r3, #128
	lsls r3, r3, #9
	mov r2, r9
	str r3, [r2, #24]
	str r3, [r2, #28]
	movs r0, #0
	movs r1, #1
	bl 0x0200dff4
	movs r2, #40
	movs r0, #10
	movs r1, #0
	bl 0x0200e074
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200e0cc
	movs r1, #2
	movs r0, #0
	bl 0x0200e064
	movs r0, #40
	bl 0x0200dfb4
	movs r1, #2
	movs r0, #10
	bl 0x0200e05c
	movs r0, #40
	bl 0x0200dfb4
	movs r2, #40
	ldr r0, [pc, #608]
	movs r1, #0
	bl 0x0200e09c
	movs r0, #0
	movs r1, #11
	bl 0x0200e044
	movs r0, #0
	adds r1, r7, #0
	bl 0x0200dff4
	movs r1, #1
	movs r0, #5
	bl 0x0200e0d4
	bl 0x0200e0ec
	movs r0, #40
	bl 0x0200dfb4
	movs r0, #5
	movs r1, #3
	bl 0x0200e04c
	movs r1, #208
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200e0ac
	movs r1, #156
	movs r0, #5
	lsls r1, r1, #1
	ldr r2, [pc, #544]
	bl 0x0200e024
	movs r2, #190
	ldr r1, [pc, #540]
	lsls r2, r2, #2
	movs r0, #5
	bl 0x0200e024
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200e0ac
	movs r0, #5
	movs r1, #0
	movs r2, #40
	bl 0x0200e0ac
	ldr r0, [pc, #504]
	movs r1, #0
	movs r2, #10
	bl 0x0200e09c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200e0c4
	movs r0, #5
	movs r1, #4
	movs r2, #40
	bl 0x0200e054
	movs r1, #192
	movs r2, #30
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #2
	movs r0, #5
	bl 0x0200e064
	movs r0, #40
	bl 0x0200dfb4
	movs r2, #30
	movs r0, #5
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl 0x0200e0dc
	movs r0, #198
	movs r1, #1
	movs r2, #147
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #18
	bl 0x0200e0e4
	mov r1, r10
	ldr r2, [pc, #396]
	movs r0, #5
	bl 0x0200e024
	bl 0x0200e0ec
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #40
	movs r0, #1
	ldr r1, [pc, #376]
	bl 0x0200e0c4
	movs r1, #2
	movs r0, #1
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	bl 0x0200d594
	movs r0, #1
	movs r1, #17
	bl 0x0200e044
	ldr r0, [pc, #348]
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r0, #131
	bl 0x0200e14c
	movs r5, #0
.L_020034c8_0:
	movs r0, #1
	bl 0x0200dfe4
	bl 0x0200dc20
	adds r5, #1
	movs r0, #1
	bl 0x0200de8c
	cmp r5, #59
	bls .L_020034c8_0
	movs r0, #1
	movs r1, #1
	bl 0x0200e0bc
	ldr r3, [pc, #304]
	movs r1, #200
	mov r10, r3
	mov r0, r10
	lsls r1, r1, #4
	bl 0x0200de94
	ldr r1, [pc, #296]
	mov r8, r1
	movs r1, #200
	lsls r1, r1, #4
	mov r0, r8
	bl 0x0200de94
	movs r0, #14
	bl 0x0200dfe4
	movs r1, #0
	bl 0x0200df4c
	movs r2, #0
	mov r9, r2
	adds r3, r6, #0
	mov r1, r9
	adds r3, #85
	strb r1, [r3]
	movs r3, #214
	lsls r3, r3, #17
	str r3, [r6, #8]
	movs r3, #128
	lsls r3, r3, #8
	mov r11, r3
	movs r2, #208
	ldr r3, [pc, #248]
	movs r5, #146
	lsls r5, r5, #18
	mov r1, r11
	lsls r2, r2, #16
	str r3, [r6, #108]
	str r2, [r6, #12]
	strh r1, [r6, #6]
	str r5, [r6, #16]
	movs r0, #4
	bl 0x0200dfb4
	movs r1, #128
	movs r2, #128
	movs r0, #14
	lsls r1, r1, #10
	lsls r2, r2, #10
	bl 0x0200dfec
	movs r1, #204
	movs r2, #208
	adds r3, r5, #0
	adds r0, r6, #0
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200df1c
	movs r0, #40
	bl 0x0200dfb4
	movs r0, #9
	ldr r1, [pc, #188]
	ldr r2, [pc, #192]
	bl 0x0200dfec
	ldr r1, [pc, #180]
	ldr r2, [pc, #184]
.L_020039cc:
	movs r0, #14
	bl 0x0200dfec
	movs r0, #9
	bl 0x0200dfe4
	movs r1, #196
	movs r2, #208
	adds r3, r5, #0
	adds r0, r6, #0
	lsls r1, r1, #17
	lsls r2, r2, #16
	b .L_020039cc_0
	.2byte 0x0000
	.2byte 0xe590
	.2byte 0x0200
	.2byte 0x0ee8
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0495
	.2byte 0x0000
	.2byte 0x0153
	.2byte 0x547a
	.2byte 0x0000
	.2byte 0x0a8f
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0399
	.2byte 0x0000
	.2byte 0x0199
	.2byte 0x0000
	.2byte 0x046e
	.2byte 0xb333
	.2byte 0x0000
	.2byte 0x5999
	.2byte 0x0000
	.2byte 0x042c
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x0155
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x0167
	.2byte 0x0000
	.2byte 0x0409
	.2byte 0x0000
	.2byte 0x03b3
	.2byte 0x0000
	.2byte 0x03fb
	.2byte 0x0000
	.2byte 0x015b
	.2byte 0x0000
	.2byte 0x03bb
	.2byte 0x0000
	.2byte 0x03f9
	.2byte 0x0000
	.2byte 0x017b
	.2byte 0x0000
	.2byte 0x014d
	.2byte 0x0000
	.2byte 0x012b
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0183
	.2byte 0x0000
	.2byte 0x0362
	.2byte 0x100a
	.2byte 0x0000
	.2byte 0x02f7
	.2byte 0x0000
	.2byte 0x0169
	.2byte 0x0000
	.2byte 0x6001
	.2byte 0x0000
	.2byte 0x02e3
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0x2001
	.2byte 0x0000
	.2byte 0xd5b1
	.2byte 0x0200
	.2byte 0xd5d1
	.2byte 0x0200
	.2byte 0xd75d
	.2byte 0x0200
	.2byte 0x2666
	.2byte 0x0000
	.2byte 0x1333
	.2byte 0x0000
.L_020039cc_0:
	bl 0x0200df1c
	movs r1, #189
	movs r2, #146
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #9
	bl 0x0200e014
	movs r0, #20
	bl 0x0200dfb4
	ldr r0, [pc, #1016]
	movs r1, #0
	movs r2, #10
	bl 0x0200e09c
	mov r2, r9
	movs r1, #2
	str r2, [r6, #108]
	movs r0, #1
	bl 0x0200e0bc
	movs r0, #1
	bl 0x0200dfe4
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	mov r0, r10
	bl 0x0200de9c
	mov r0, r8
	bl 0x0200de9c
	movs r0, #1
	bl 0x0200de8c
	movs r0, #1
	movs r1, #0
	bl 0x0200e07c
	movs r0, #9
	movs r1, #0
	bl 0x0200e07c
	movs r1, #192
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #6
	bl 0x0200e0ac
	movs r1, #1
	movs r0, #1
	bl 0x0200e044
	adds r0, r6, #0
	bl 0x0200d7fc
	bl 0x0200d5a4
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #212
	movs r2, #156
	movs r0, #5
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r2, #60
	movs r0, #1
	movs r1, #5
	bl 0x0200e074
	movs r1, #2
	movs r0, #1
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #20
	ldr r0, [pc, #872]
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #129
	movs r2, #40
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r1, #4
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r2, #80
	movs r0, #1
	ldr r1, [pc, #804]
	bl 0x0200e0c4
	movs r1, #1
	movs r0, #5
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #129
	movs r2, #80
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r1, #4
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #20
	movs r0, #5
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #1
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #194
	movs r2, #151
	movs r0, #5
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r1, #160
	movs r2, #20
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #2
	movs r0, #5
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #20
	ldr r0, [pc, #688]
	movs r1, #0
	bl 0x0200e09c
	movs r1, #4
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #40
	movs r0, #1
	ldr r1, [pc, #664]
	bl 0x0200e0c4
	movs r1, #2
	movs r0, #1
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x0200e0ac
	mov r1, r11
	movs r0, #1
	movs r2, #40
	bl 0x0200e0ac
	movs r3, #14
	str r3, [sp, #12]
	str r3, [sp, #20]
	movs r1, #5
	mov r3, r9
	movs r0, #10
	movs r4, #4
	str r1, [sp, #4]
	str r0, [sp, #8]
	str r3, [sp, #24]
	movs r2, #2
	movs r3, #25
	movs r1, #1
	movs r0, #1
	str r2, [sp, #0]
	str r4, [sp, #16]
	bl 0x0200e0b4
	movs r0, #40
	bl 0x0200dfb4
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	bl 0x0200e0cc
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200e0cc
	movs r0, #80
	bl 0x0200dfb4
	movs r0, #5
	ldr r1, [pc, #548]
	movs r2, #40
	bl 0x0200e0c4
	movs r2, #20
	ldr r0, [pc, #540]
	movs r1, #0
	bl 0x0200e09c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200e0cc
	movs r0, #80
	bl 0x0200dfb4
	movs r1, #4
	movs r0, #1
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #5
	ldr r1, [pc, #496]
	movs r2, #40
	bl 0x0200e0c4
	movs r2, #20
	ldr r0, [pc, #492]
	movs r1, #0
	bl 0x0200e09c
	movs r1, #2
	movs r0, #1
	bl 0x0200e064
	movs r0, #40
	bl 0x0200dfb4
	movs r1, #4
	movs r0, #1
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r2, #80
	movs r0, #5
	ldr r1, [pc, #448]
	bl 0x0200e0c4
	movs r1, #4
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	ldr r0, [pc, #432]
	movs r1, #0
	movs r2, #10
	bl 0x0200e09c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #5
	movs r2, #40
	bl 0x0200e0ac
	movs r2, #40
	movs r0, #5
	movs r1, #1
	bl 0x0200e06c
	movs r1, #2
	movs r0, #1
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #190
	movs r2, #155
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #5
	bl 0x0200e024
	movs r0, #10
	bl 0x0200dfb4
	ldr r0, [pc, #364]
	movs r1, #0
	movs r2, #10
	bl 0x0200e09c
	movs r1, #160
	movs r2, #30
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200e0cc
	movs r0, #80
	bl 0x0200dfb4
	movs r1, #2
	movs r0, #1
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #20
	ldr r0, [pc, #304]
	movs r1, #0
	bl 0x0200e09c
	movs r1, #4
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #80
	movs r0, #1
	ldr r1, [pc, #280]
	bl 0x0200e0c4
	movs r1, #4
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #1
	ldr r1, [pc, #272]
	ldr r2, [pc, #272]
	bl 0x0200dfec
	movs r1, #206
	movs r2, #151
	movs r0, #1
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r1, #160
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r1, #1
	movs r0, #5
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	ldr r0, [pc, #216]
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r0, #1
	ldr r1, [pc, #200]
	movs r2, #80
	bl 0x0200e0c4
	movs r0, #5
	movs r1, #4
	movs r2, #30
	bl 0x0200e054
	movs r2, #20
	ldr r0, [pc, #184]
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #1
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #0
	movs r2, #20
	ldr r0, [pc, #148]
	bl 0x0200e09c
	movs r0, #30
	bl 0x0200dfb4
	movs r1, #224
	movs r2, #40
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r0, #5
	movs r1, #3
	bl 0x0200e04c
	ldr r0, [pc, #124]
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r0, #1
	ldr r1, [pc, #108]
	movs r2, #80
	bl 0x0200e0c4
	movs r0, #5
	ldr r1, [pc, #120]
	movs r2, #40
	bl 0x0200e0c4
	movs r0, #5
	ldr r1, [pc, #100]
	ldr r2, [pc, #104]
	bl 0x0200dfec
	movs r1, #214
	movs r2, #157
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #5
	bl 0x0200e01c
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #1
	bl 0x0200e0ac
	movs r0, #5
	bl 0x0200e034
	ldr r0, [pc, #68]
	movs r1, #0
	bl 0x0200e094
	movs r1, #1
	movs r0, #5
	bl 0x0200e044
	movs r0, #60
	bl 0x0200dfb4
	movs r1, #176
	movs r2, #30
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r0, #1
	b .L_020039cc_1
	.4byte 0x00002005
	.4byte 0x00006001
	.4byte 0x00000101
	.4byte 0x00001005
	.4byte 0x00000105
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00000103
	.4byte 0x00005001
.L_020039cc_1:
	movs r1, #2
	bl 0x0200e064
	movs r1, #214
	movs r2, #157
	movs r0, #5
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r1, #176
	movs r2, #20
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #3
	movs r0, #1
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #4
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #20
	ldr r0, [pc, #160]
	movs r1, #0
	bl 0x0200e09c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200e0cc
	movs r0, #40
	bl 0x0200dfb4
	movs r2, #20
	ldr r0, [pc, #136]
	movs r1, #0
	bl 0x0200e09c
	movs r0, #1
	movs r1, #3
	bl 0x0200e044
	movs r1, #3
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #128
	mov r1, r11
	movs r0, #5
	lsls r2, r2, #7
	bl 0x0200dfec
	movs r2, #128
	mov r1, r11
	movs r0, #1
	lsls r2, r2, #7
	bl 0x0200dfec
	movs r1, #225
	movs r0, #5
	lsls r1, r1, #1
	ldr r2, [pc, #80]
	bl 0x0200e01c
	movs r1, #225
	lsls r1, r1, #1
	ldr r2, [pc, #72]
	movs r0, #1
	bl 0x0200e01c
	movs r0, #60
	bl 0x0200dfb4
	ldr r3, [pc, #60]
	movs r1, #228
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #60
	str r2, [r3]
	bl 0x0200e10c
	bl 0x0200e114
	movs r0, #12
	bl 0x0200e0fc
	bl 0x0200dfc4
	sub sp, #-28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00002005
	.4byte 0x00005001
	.4byte 0x000002ee
	.4byte 0x03001ebc
	.global Func_02003fb0
	.thumb_func
Func_02003fb0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #0
	sub sp, #28
	bl 0x0200dfe4
	mov r11, r0
	bl 0x0200dfbc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl 0x0200e0e4
	movs r0, #1
	bl 0x0200de8c
	movs r3, #20
	movs r2, #50
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #53
	movs r2, #8
	movs r3, #4
	bl 0x0200df3c
	movs r3, #2
	str r3, [sp, #0]
	movs r5, #1
	movs r0, #2
	movs r1, #102
	movs r2, #84
	movs r3, #41
	str r5, [sp, #4]
	bl 0x0200df34
	movs r0, #1
	movs r1, #102
	movs r2, #83
	movs r3, #41
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200df34
	movs r3, #42
	movs r0, #0
	movs r1, #103
	movs r2, #82
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200df34
	movs r1, #196
	movs r2, #224
	lsls r1, r1, #17
	lsls r2, r2, #18
	movs r0, #21
	bl 0x0200e03c
	movs r0, #21
	bl 0x0200dfe4
	movs r3, #192
	lsls r3, r3, #8
	ldr r2, [pc, #60]
	mov r9, r3
	mov r8, r2
	mov r2, r9
	strh r2, [r0, #6]
	movs r1, #149
	movs r2, #184
	lsls r1, r1, #17
	lsls r2, r2, #18
	movs r0, #1
	bl 0x0200e03c
	movs r0, #1
	bl 0x0200dfe4
	movs r3, #128
	lsls r3, r3, #7
	mov r10, r3
	mov r2, r10
	strh r2, [r0, #6]
	movs r1, #149
	movs r2, #190
	lsls r2, r2, #18
	lsls r1, r1, #17
	movs r0, #5
	bl 0x0200e03c
	movs r0, #5
	bl 0x0200dfe4
	b .L_02003fb0_0
	.2byte 0x0000
	.4byte 0x00000000
.L_02003fb0_0:
	mov r3, r10
	strh r3, [r0, #6]
	movs r1, #11
	movs r0, #0
	bl 0x0200e044
	ldr r1, [pc, #1004]
	movs r0, #0
	bl 0x0200dff4
	movs r0, #23
	bl 0x0200dfe4
	adds r7, r0, #0
	adds r3, r7, #0
	mov r2, r8
	adds r3, #85
	strb r2, [r3]
	movs r5, #194
	movs r6, #160
	movs r3, #210
	lsls r5, r5, #17
	lsls r6, r6, #16
	lsls r3, r3, #18
	str r3, [r7, #16]
	str r5, [r7, #8]
	movs r1, #0
	str r6, [r7, #12]
	bl 0x0200df4c
	movs r0, #24
	bl 0x0200dfe4
	adds r7, r0, #0
	adds r3, r7, #0
	mov r2, r8
	adds r3, #85
	strb r2, [r3]
	movs r3, #211
	lsls r3, r3, #18
	str r3, [r7, #16]
	str r5, [r7, #8]
	movs r1, #0
	str r6, [r7, #12]
	bl 0x0200df4c
	movs r0, #25
	bl 0x0200dfe4
	adds r7, r0, #0
	adds r3, r7, #0
	mov r2, r8
	adds r3, #85
	strb r2, [r3]
	movs r3, #212
	lsls r3, r3, #18
	movs r1, #0
	str r3, [r7, #16]
	str r5, [r7, #8]
	str r6, [r7, #12]
	bl 0x0200df4c
	bl 0x0200e0f4
	mov r3, r8
	adds r0, #85
	strb r3, [r0]
	movs r0, #1
	bl 0x0200de8c
	adds r1, r6, #0
	ldr r2, [pc, #880]
	movs r3, #0
	ldr r0, [pc, #880]
	bl 0x0200e0e4
	bl 0x0200df14
	movs r0, #1
	bl 0x0200de8c
	ldr r3, [pc, #868]
	movs r2, #228
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #32
	str r2, [r3]
	bl 0x0200e104
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	mov r2, r10
	bl 0x0200dfec
	movs r1, #128
	mov r2, r10
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200dfec
	ldr r1, [pc, #828]
	movs r0, #5
	bl 0x0200dff4
	ldr r1, [pc, #824]
	movs r0, #1
	bl 0x0200dff4
	movs r0, #40
	bl 0x0200dfb4
	movs r0, #0
	movs r1, #1
	bl 0x0200dff4
	movs r3, #128
	lsls r3, r3, #9
	mov r2, r11
	movs r1, #176
	str r3, [r2, #24]
	str r3, [r2, #28]
	movs r0, #0
	movs r2, #40
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #3
	movs r0, #0
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #0
	ldr r1, [pc, #768]
	ldr r2, [pc, #772]
	bl 0x0200dfec
	movs r1, #200
	movs r2, #210
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #0
	bl 0x0200e024
	movs r0, #10
	bl 0x0200dfb4
	movs r2, #30
	movs r0, #0
	mov r1, r9
	bl 0x0200e0ac
	movs r1, #1
	movs r0, #0
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #128
	movs r2, #40
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200e0ac
	bl 0x0200d594
	movs r0, #0
	movs r1, #17
	bl 0x0200e044
	movs r1, #200
	ldr r0, [pc, #700]
	lsls r1, r1, #4
	bl 0x0200de94
	movs r5, #0
.L_02003fb0_1:
	mov r0, r11
	bl 0x0200dc20
	adds r5, #1
	movs r0, #1
	bl 0x0200de8c
	cmp r5, #39
	bls .L_02003fb0_1
	movs r0, #0
	movs r1, #1
	bl 0x0200e0bc
	ldr r6, [pc, #668]
	movs r1, #200
	lsls r1, r1, #4
	adds r0, r6, #0
	bl 0x0200de94
	ldr r5, [pc, #660]
	movs r1, #200
	adds r0, r5, #0
	lsls r1, r1, #4
	bl 0x0200de94
	movs r0, #23
	ldr r1, [pc, #648]
	ldr r2, [pc, #652]
	bl 0x0200dfec
	movs r1, #195
	movs r2, #208
	movs r0, #23
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e014
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #200
	lsls r1, r1, #1
	ldr r2, [pc, #620]
	movs r0, #23
	bl 0x0200e014
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #0
	bl 0x0200dfe4
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r1, #1
	strb r3, [r0]
	movs r0, #0
	bl 0x0200e044
	ldr r3, [pc, #560]
	mov r8, r3
	mov r0, r8
	bl 0x0200de9c
	adds r0, r6, #0
	bl 0x0200de9c
	adds r0, r5, #0
	bl 0x0200de9c
	movs r0, #1
	bl 0x0200de8c
	movs r0, #0
	movs r1, #0
	bl 0x0200e07c
	movs r0, #23
	movs r1, #0
	bl 0x0200e07c
	movs r2, #0
	movs r1, #0
	movs r0, #23
	bl 0x0200e03c
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #0
	movs r1, #11
	bl 0x0200e044
	ldr r1, [pc, #460]
	movs r0, #0
	bl 0x0200dff4
	movs r0, #120
	bl 0x0200dfb4
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #41
	movs r2, #84
	movs r0, #7
	movs r1, #102
	bl 0x0200df34
	movs r0, #0
	movs r1, #1
	bl 0x0200dff4
	movs r3, #128
	lsls r3, r3, #9
	mov r2, r11
	str r3, [r2, #24]
	str r3, [r2, #28]
	movs r1, #1
	movs r0, #0
	bl 0x0200e044
	movs r0, #40
	bl 0x0200dfb4
	movs r0, #0
	movs r1, #3
	bl 0x0200e04c
	movs r0, #0
	ldr r1, [pc, #440]
	ldr r2, [pc, #444]
	bl 0x0200e024
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200e0ac
	movs r2, #20
	movs r0, #0
	movs r1, #0
	bl 0x0200e0ac
	movs r0, #0
	movs r1, #17
	bl 0x0200e044
	movs r1, #200
	mov r0, r8
	lsls r1, r1, #4
	bl 0x0200de94
	movs r5, #0
.L_02003fb0_2:
	mov r0, r11
	bl 0x0200dc20
	adds r5, #1
	movs r0, #1
	bl 0x0200de8c
	cmp r5, #39
	bls .L_02003fb0_2
	movs r0, #0
	movs r1, #1
	bl 0x0200e0bc
	ldr r6, [pc, #344]
	movs r1, #200
	lsls r1, r1, #4
	adds r0, r6, #0
	bl 0x0200de94
	ldr r5, [pc, #360]
	movs r1, #200
	adds r0, r5, #0
	lsls r1, r1, #4
	bl 0x0200de94
	movs r0, #24
	ldr r1, [pc, #328]
	ldr r2, [pc, #328]
	bl 0x0200dfec
	movs r1, #195
	movs r2, #208
	movs r0, #24
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e014
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r2, #207
	ldr r1, [pc, #304]
	lsls r2, r2, #2
	movs r0, #24
	bl 0x0200e014
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #0
	bl 0x0200dfe4
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r1, #1
	strb r3, [r0]
	movs r0, #0
	bl 0x0200e044
	ldr r3, [pc, #240]
	mov r8, r3
	mov r0, r8
	bl 0x0200de9c
	adds r0, r6, #0
	bl 0x0200de9c
	adds r0, r5, #0
	bl 0x0200de9c
	movs r0, #1
	bl 0x0200de8c
	movs r0, #0
	movs r1, #0
	bl 0x0200e07c
	movs r0, #24
	movs r1, #0
	bl 0x0200e07c
	movs r2, #0
	movs r1, #0
	movs r0, #24
	bl 0x0200e03c
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #0
	movs r1, #11
	bl 0x0200e044
	ldr r1, [pc, #140]
	movs r0, #0
	bl 0x0200dff4
	movs r0, #120
	bl 0x0200dfb4
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r2, #83
	movs r3, #41
	movs r0, #6
	movs r1, #102
	bl 0x0200df34
	movs r0, #0
	movs r1, #1
	bl 0x0200dff4
	movs r3, #128
	lsls r3, r3, #9
	mov r2, r11
	str r3, [r2, #24]
	str r3, [r2, #28]
	movs r1, #1
	movs r0, #0
	bl 0x0200e044
	movs r0, #40
	bl 0x0200dfb4
	movs r0, #0
	movs r1, #3
	bl 0x0200e04c
	movs r1, #180
	movs r0, #0
	lsls r1, r1, #1
	ldr r2, [pc, #128]
	bl 0x0200e024
	movs r1, #176
	movs r0, #21
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200e0ac
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #30
	bl 0x0200e0ac
	movs r1, #208
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r0, #0
	movs r1, #17
	bl 0x0200e044
	movs r1, #200
	mov r0, r8
	lsls r1, r1, #4
	bl 0x0200de94
	movs r5, #0
	b .L_02003fb0_3
	.4byte 0x0200e590
	.4byte 0x036d0000
	.4byte 0x017f0000
	.4byte 0x03001ebc
	.4byte 0x0200e614
	.4byte 0x0200e5cc
	.4byte 0x00004ccc
	.4byte 0x00002666
	.4byte 0x0200da09
	.4byte 0x0200d5c1
	.4byte 0x0200d5e1
	.4byte 0x00003333
	.4byte 0x00001999
	.4byte 0x0000033a
	.4byte 0x00000179
	.4byte 0x0000034b
	.4byte 0x0200d5f1
	.4byte 0x00000357
.L_02003fb0_3:
	mov r0, r11
	bl 0x0200dc20
	adds r5, #1
	movs r0, #1
	bl 0x0200de8c
	cmp r5, #39
	bls .L_02003fb0_3
	movs r0, #0
	movs r1, #1
	bl 0x0200e0bc
	ldr r6, [pc, #516]
	movs r1, #200
	lsls r1, r1, #4
	adds r0, r6, #0
	bl 0x0200de94
	ldr r5, [pc, #508]
	movs r1, #200
	lsls r1, r1, #4
	adds r0, r5, #0
	bl 0x0200de94
	movs r0, #25
	ldr r1, [pc, #496]
	ldr r2, [pc, #500]
	bl 0x0200dfec
	movs r1, #195
	movs r2, #208
	movs r0, #25
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e014
	movs r1, #192
	movs r0, #0
.L_0200451a:
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #180
	lsls r1, r1, #1
	ldr r2, [pc, #468]
	movs r0, #25
	bl 0x0200e014
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #0
	bl 0x0200dfe4
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	movs r0, #0
	bl 0x0200e044
	ldr r0, [pc, #432]
	bl 0x0200de9c
	adds r0, r6, #0
	bl 0x0200de9c
	adds r0, r5, #0
	bl 0x0200de9c
	movs r0, #1
	bl 0x0200de8c
	movs r0, #0
	movs r1, #0
	bl 0x0200e07c
	movs r0, #25
	movs r1, #0
	bl 0x0200e07c
	movs r2, #0
	movs r1, #0
	movs r0, #25
	bl 0x0200e03c
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #0
	movs r1, #11
	bl 0x0200e044
	ldr r1, [pc, #372]
	movs r0, #0
	bl 0x0200dff4
	movs r0, #120
	bl 0x0200dfb4
	bl 0x0200d5a4
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r2, #82
	movs r3, #42
	movs r0, #5
	movs r1, #103
	bl 0x0200df34
	movs r0, #0
	movs r1, #1
	bl 0x0200dff4
	movs r3, #128
	lsls r3, r3, #9
	mov r2, r11
	str r3, [r2, #24]
	str r3, [r2, #28]
	movs r1, #2
	movs r2, #20
	movs r0, #21
	bl 0x0200e054
	ldr r0, [pc, #312]
	bl 0x0200e084
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl 0x0200e09c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #5
	movs r2, #10
	bl 0x0200e0ac
	movs r3, #0
	movs r0, #21
	movs r1, #5
	movs r2, #6
	bl 0x0200b2b0
	movs r0, #21
	ldr r1, [pc, #276]
	ldr r2, [pc, #276]
	bl 0x0200dfec
	movs r2, #208
	ldr r1, [pc, #272]
	lsls r2, r2, #2
	movs r0, #21
	bl 0x0200e024
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #128
.L_02004612:
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #60
	bl 0x0200e0ac
	movs r1, #192
	movs r2, #60
	movs r0, #21
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #3
	bl 0x0200e04c
	movs r0, #21
	movs r1, #3
	bl 0x0200e04c
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl 0x0200e09c
	movs r1, #186
	movs r2, #208
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #21
	bl 0x0200e024
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #40
	bl 0x0200e0ac
	movs r1, #128
	movs r2, #40
	movs r0, #21
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #3
	bl 0x0200e04c
	movs r0, #21
	movs r1, #3
	bl 0x0200e04c
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #160
	movs r2, #20
	movs r0, #21
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #3
	bl 0x0200e04c
	movs r2, #10
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r1, #2
	movs r0, #0
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #21
	movs r1, #4
	bl 0x0200e04c
	movs r1, #0
	movs r0, #21
	bl 0x0200e08c
	movs r0, #0
	movs r1, #0
	bl 0x0200dfdc
	cmp r0, #0
	bne .L_02004612_0
	movs r0, #21
	movs r1, #3
	bl 0x0200e04c
	ldr r3, [pc, #60]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02004612_1
	.2byte 0x0000
	.2byte 0xd5c1
	.2byte 0x0200
	.2byte 0xd601
	.2byte 0x0200
	.2byte 0x3333
	.2byte 0x0000
	.2byte 0x1999
	.2byte 0x0000
	.2byte 0x0345
	.2byte 0x0000
	.2byte 0xda09
	.2byte 0x0200
	.2byte 0xe590
	.2byte 0x0200
	.2byte 0x0f03
	.2byte 0x0000
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0x2666
	.2byte 0x0000
	.2byte 0x018d
	.2byte 0x0000
	.4byte 0x03001ebc
.L_02004612_0:
	movs r0, #21
	movs r1, #4
	bl 0x0200e04c
.L_02004612_1:
	movs r1, #0
	movs r2, #20
	movs r0, #21
	bl 0x0200e09c
	ldr r0, [pc, #1012]
	bl 0x0200e084
	movs r1, #193
	lsls r1, r1, #1
	ldr r2, [pc, #1004]
	movs r0, #21
	bl 0x0200e024
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #208
	movs r2, #60
	movs r0, #21
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #2
	bl 0x0200e064
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #160
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #30
	bl 0x0200e0ac
	movs r1, #0
	movs r0, #21
	bl 0x0200e08c
	movs r0, #0
	movs r1, #0
	bl 0x0200dfdc
	cmp r0, #1
	bne .L_02004612_2
	ldr r3, [pc, #932]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02004612_2:
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #208
	movs r2, #60
	movs r0, #21
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #2
	movs r0, #21
	bl 0x0200e064
	ldr r0, [pc, #892]
	bl 0x0200e084
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #193
	ldr r2, [pc, #876]
	lsls r1, r1, #1
	movs r0, #21
	bl 0x0200e024
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #4
	movs r0, #21
	bl 0x0200e04c
	movs r0, #60
	bl 0x0200dfb4
	movs r0, #21
	movs r1, #0
	movs r2, #60
	bl 0x0200e09c
	movs r1, #160
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl 0x0200e09c
	movs r1, #186
	movs r2, #208
	movs r0, #21
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r1, #160
	movs r2, #10
	movs r0, #21
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r1, #2
	movs r0, #0
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #3
	movs r0, #21
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #129
	movs r2, #60
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r0, #21
	movs r1, #2
	bl 0x0200e064
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r0, #0
	movs r1, #3
	bl 0x0200e04c
	movs r0, #21
	movs r1, #3
	bl 0x0200e04c
	movs r2, #10
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	ldr r0, [pc, #700]
	ldr r1, [pc, #700]
	bl 0x0200e0dc
	movs r1, #160
	movs r2, #215
	movs r3, #1
	ldr r0, [pc, #692]
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200e0e4
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200dfec
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200dfec
	movs r2, #226
	movs r0, #1
	ldr r1, [pc, #656]
	lsls r2, r2, #2
	bl 0x0200e01c
	movs r1, #196
	movs r2, #226
	lsls r2, r2, #2
	movs r0, #5
	lsls r1, r1, #1
	bl 0x0200e024
	movs r0, #1
	movs r1, #1
	bl 0x0200e044
	movs r3, #0
	movs r0, #5
	movs r1, #10
	movs r2, #11
	bl 0x0200b2b0
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl 0x0200e09c
	movs r1, #2
	movs r0, #21
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #5
	movs r2, #20
	bl 0x0200e0ac
	movs r0, #5
	movs r1, #4
	movs r2, #0
	bl 0x0200e054
	movs r1, #196
	movs r0, #5
	lsls r1, r1, #1
	ldr r2, [pc, #540]
	bl 0x0200e024
	movs r1, #144
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #208
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #3
	bl 0x0200e04c
	movs r2, #10
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r0, #5
	movs r1, #3
	bl 0x0200e04c
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl 0x0200e09c
	movs r0, #21
	movs r1, #3
	bl 0x0200e044
	movs r0, #0
	movs r1, #3
	bl 0x0200e04c
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r3, #0
	movs r0, #1
	movs r1, #10
	movs r2, #11
	bl 0x0200b2b0
	movs r0, #5
	ldr r1, [pc, #428]
	ldr r2, [pc, #428]
	bl 0x0200dfec
	movs r0, #1
	ldr r1, [pc, #416]
	ldr r2, [pc, #420]
	bl 0x0200dfec
	movs r1, #196
	lsls r1, r1, #1
	ldr r2, [pc, #400]
	movs r0, #1
	bl 0x0200e01c
	movs r0, #5
	bl 0x0200dfe4
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r1, #204
	strb r3, [r0]
	lsls r1, r1, #1
	ldr r2, [pc, #372]
	movs r0, #5
	bl 0x0200e024
	movs r0, #1
	bl 0x0200dfb4
	movs r0, #5
	bl 0x0200dfe4
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r1, #128
	strb r3, [r0]
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #5
	bl 0x0200e0ac
	movs r0, #1
	bl 0x0200e034
	movs r0, #1
	movs r1, #1
	bl 0x0200e044
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #30
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #4
	movs r2, #30
	bl 0x0200e054
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #1
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #160
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl 0x0200e09c
	movs r1, #208
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r2, #30
	movs r0, #0
	movs r1, #2
	bl 0x0200e054
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #0
	bl 0x0200e0cc
	movs r0, #60
	bl 0x0200dfb4
	movs r1, #192
	movs r2, #40
	movs r0, #21
	lsls r1, r1, #6
	bl 0x0200e0ac
	movs r0, #1
	movs r1, #2
	bl 0x0200e064
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r0, #21
	ldr r1, [pc, #188]
	movs r2, #80
	bl 0x0200e0c4
	movs r1, #160
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #30
	bl 0x0200e0ac
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200e0c4
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #208
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200e0ac
	movs r2, #10
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #1
	bl 0x0200e04c
	movs r0, #80
	bl 0x0200dfb4
	movs r2, #30
	movs r0, #5
	movs r1, #1
	bl 0x0200e074
	movs r0, #1
	movs r1, #2
	bl 0x0200e05c
	movs r1, #2
	movs r0, #5
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #21
	ldr r1, [pc, #80]
	movs r2, #60
	bl 0x0200e0c4
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl 0x0200e09c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	b .L_02004612_3
	.2byte 0x0000
	.4byte 0x00000f0a
	.4byte 0x00000349
	.4byte 0x03001ebc
	.4byte 0x00000f0e
	.4byte 0x00000339
	.4byte 0x00006666
	.4byte 0x00000ccc
	.4byte 0x01790000
	.4byte 0x00000171
	.4byte 0x0000034b
	.4byte 0x00004ccc
	.4byte 0x00002666
	.4byte 0x00000101
	.4byte 0x00000105
.L_02004612_3:
	bl 0x0200e0ac
	movs r1, #128
	movs r2, #10
	movs r0, #1
.L_02004b66:
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #2
	movs r0, #5
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl 0x0200e09c
	movs r1, #2
	movs r0, #21
	bl 0x0200e05c
	movs r0, #40
	bl 0x0200dfb4
	movs r2, #10
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r0, #1
	movs r1, #3
	bl 0x0200e044
	movs r1, #3
	movs r0, #5
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #21
	movs r1, #4
	bl 0x0200e04c
	movs r2, #10
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r0, #1
	movs r1, #3
	bl 0x0200e044
	movs r0, #5
	movs r1, #3
	bl 0x0200e04c
	movs r0, #21
	movs r1, #3
	bl 0x0200e04c
	movs r2, #10
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r0, #1
	movs r1, #3
	bl 0x0200e044
	movs r1, #3
	movs r0, #5
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r0, #21
	movs r1, #2
	bl 0x0200e064
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	ldr r0, [pc, #1004]
	ldr r1, [pc, #1004]
	bl 0x0200e0dc
	movs r1, #160
	movs r3, #1
	ldr r0, [pc, #1000]
	lsls r1, r1, #16
	ldr r2, [pc, #1000]
	bl 0x0200e0e4
	movs r1, #182
	movs r2, #204
	movs r0, #21
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r1, #208
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #6
	movs r2, #10
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #0
	movs r2, #40
	bl 0x0200e09c
	movs r2, #30
	movs r0, #5
	movs r1, #1
	bl 0x0200e074
	movs r0, #1
	movs r1, #2
	bl 0x0200e05c
	movs r1, #2
	movs r0, #5
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200e0ac
	movs r1, #160
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl 0x0200e09c
	movs r2, #60
	movs r0, #0
	ldr r1, [pc, #868]
	bl 0x0200e0c4
	movs r0, #21
	movs r1, #4
	bl 0x0200e04c
	movs r1, #0
	movs r0, #21
	bl 0x0200e08c
	movs r0, #0
	movs r1, #0
	movs r6, #0
	bl 0x0200dfdc
	cmp r0, #1
	bne .L_02004b66_0
	ldr r3, [pc, #836]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02004b66_0:
	movs r1, #0
	movs r2, #20
	movs r0, #21
	bl 0x0200e09c
	ldr r0, [pc, #812]
	bl 0x0200e084
	movs r2, #0
	movs r0, #21
	ldr r1, [pc, #808]
	bl 0x0200e0c4
	movs r0, #21
	movs r1, #3
	bl 0x0200e064
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl 0x0200e09c
	movs r2, #0
	movs r0, #21
	movs r1, #4
	bl 0x0200e054
	movs r0, #21
	movs r1, #3
	bl 0x0200e064
	movs r1, #7
	movs r0, #21
	bl 0x0200e044
	movs r0, #5
	bl 0x0200dfb4
	movs r3, #14
	movs r1, #1
	movs r0, #10
	movs r4, #4
	str r1, [sp, #4]
	str r0, [sp, #8]
	str r3, [sp, #12]
	str r3, [sp, #20]
	movs r2, #2
	movs r1, #14
	movs r3, #24
	movs r0, #21
	str r2, [sp, #0]
	str r4, [sp, #16]
	str r6, [sp, #24]
	bl 0x0200e0b4
	movs r0, #161
	bl 0x0200e14c
	movs r0, #21
	bl 0x0200dfe4
	adds r7, r0, #0
	ldr r3, [r7, #80]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	adds r3, #38
	strb r6, [r3]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r1]
	movs r2, #192
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200dfec
	movs r1, #182
	movs r0, #21
	lsls r1, r1, #1
	ldr r2, [pc, #668]
	bl 0x0200e024
	movs r0, #4
	bl 0x0200dfb4
	movs r5, #0
.L_02004b66_1:
	ldr r3, [r7, #16]
	movs r2, #192
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r7, #16]
	ldr r2, [pc, #648]
	ldr r3, [r7, #28]
	adds r3, r3, r2
	str r3, [r7, #28]
	movs r0, #1
	adds r5, #1
	bl 0x0200dfb4
	cmp r5, #4
	bne .L_02004b66_1
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl 0x0200e03c
	movs r1, #192
	movs r2, #192
	movs r0, #1
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200dfec
	movs r0, #1
	movs r1, #6
	movs r2, #0
	bl 0x0200e054
	movs r1, #187
	ldr r2, [pc, #596]
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200e024
	movs r0, #5
	movs r1, #0
	bl 0x0200e094
	movs r1, #176
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r0, #5
	movs r1, #2
	bl 0x0200e05c
	movs r1, #128
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r0, #1
	movs r1, #13
	bl 0x0200e044
	movs r1, #2
	movs r2, #5
	movs r0, #1
	bl 0x0200e054
	movs r0, #143
	bl 0x0200e14c
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl 0x0200df54
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r2, #83
	movs r3, #41
	movs r1, #102
	movs r0, #1
	bl 0x0200df34
	movs r0, #1
	bl 0x0200dfe4
	movs r1, #0
	bl 0x0200df4c
	movs r1, #208
	movs r2, #10
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r0, #1
	movs r1, #3
	bl 0x0200e05c
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #444]
	bl 0x0200df54
	bl 0x0200df5c
	movs r1, #129
	movs r2, #80
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r0, #21
	movs r1, #8
	bl 0x0200e044
	movs r3, #128
	lsls r3, r3, #8
	movs r1, #182
	str r3, [r7, #28]
	movs r0, #21
	lsls r1, r1, #17
	ldr r2, [pc, #404]
	bl 0x0200e03c
	movs r5, #0
.L_02004b66_2:
	ldr r3, [r7, #28]
	ldr r2, [pc, #400]
	adds r3, r3, r2
	str r3, [r7, #28]
	movs r0, #1
	adds r5, #1
	bl 0x0200dfb4
	cmp r5, #5
	bne .L_02004b66_2
	movs r0, #60
	bl 0x0200dfb4
	movs r0, #1
	movs r1, #2
	bl 0x0200e064
	movs r1, #160
	movs r2, #30
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r0, #1
	movs r1, #2
	bl 0x0200e05c
	movs r1, #2
	movs r0, #5
	bl 0x0200e064
	movs r0, #60
	bl 0x0200dfb4
	movs r1, #2
	movs r0, #21
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	ldr r0, [pc, #312]
	ldr r1, [pc, #316]
	bl 0x0200e0dc
	movs r0, #186
	movs r1, #160
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #16
	ldr r2, [pc, #304]
	bl 0x0200e0e4
	movs r1, #192
	movs r2, #192
	movs r0, #21
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200dfec
	movs r0, #21
	movs r1, #6
	movs r2, #0
	bl 0x0200e054
	ldr r1, [pc, #276]
	ldr r2, [pc, #280]
	movs r0, #21
	bl 0x0200e024
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #128
	movs r2, #20
	movs r0, #21
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #2
	bl 0x0200e064
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r1]
	movs r0, #21
	movs r1, #0
	movs r2, #80
	bl 0x0200e09c
	movs r0, #21
	ldr r1, [pc, #224]
	movs r2, #80
	bl 0x0200e0c4
	movs r2, #60
	movs r0, #21
	movs r1, #0
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #3
	bl 0x0200e064
	movs r2, #10
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #21
	bl 0x0200e0cc
	movs r0, #80
	bl 0x0200dfb4
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #40
	bl 0x0200e0ac
	movs r1, #129
	movs r2, #80
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r0, #1
	movs r1, #2
	bl 0x0200e064
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #128
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #3
	movs r0, #1
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #3
	movs r0, #1
	bl 0x0200e064
	movs r0, #1
	bl 0x0200dfe4
	movs r1, #1
	bl 0x0200df4c
	movs r2, #0
	movs r0, #1
	movs r1, #6
	b .L_02004b66_3
	.2byte 0x0000
	.4byte 0x00009999
	.4byte 0x00001333
	.4byte 0x01750000
	.4byte 0x03450000
	.4byte 0x00000105
	.4byte 0x03001ebc
	.4byte 0x00000f27
	.4byte 0x00000103
	.4byte 0x0000032f
	.4byte 0xffffe667
	.4byte 0x0000033b
	.4byte 0x0000e666
	.4byte 0x032b0000
	.4byte 0x00001999
	.4byte 0x00004ccc
	.4byte 0x00000999
	.4byte 0x035b0000
	.4byte 0x00000167
	.4byte 0x00000343
	.4byte 0x00000101
.L_02004b66_3:
	bl 0x0200e054
	movs r0, #1
	movs r1, #1
	bl 0x0200e044
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #10
	movs r0, #1
	bl 0x0200dfec
	movs r0, #1
	bl 0x0200dfe4
	adds r7, r0, #0
	adds r2, r7, #0
	adds r2, #90
	ldrb r3, [r2]
	ands r5, r3
	strb r5, [r2]
	movs r0, #1
	ldr r2, [pc, #1016]
	ldr r1, [pc, #1016]
	bl 0x0200e00c
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	bl 0x0200e0cc
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200e0ac
	movs r1, #0
	movs r2, #1
	movs r0, #5
	bl 0x0200e09c
	movs r0, #1
	bl 0x0200e034
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200e0ac
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #128
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r0, #1
	movs r1, #13
	bl 0x0200e044
	movs r2, #5
	movs r1, #2
	movs r0, #1
	bl 0x0200e054
	movs r0, #1
	bl 0x0200dfe4
	movs r1, #0
	bl 0x0200df4c
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #41
	movs r1, #102
	movs r2, #84
	movs r0, #2
	bl 0x0200df34
	movs r0, #143
	bl 0x0200e14c
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	movs r0, #0
	lsls r1, r1, #11
	bl 0x0200df54
	movs r0, #1
	movs r1, #3
	bl 0x0200e064
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #856]
	bl 0x0200df54
	bl 0x0200df5c
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #30
	bl 0x0200e0c4
	movs r0, #5
	ldr r1, [pc, #836]
	ldr r2, [pc, #840]
	bl 0x0200dfec
	movs r1, #204
	ldr r2, [pc, #836]
	lsls r1, r1, #1
	movs r0, #5
	bl 0x0200e024
	movs r0, #60
	bl 0x0200dfb4
	movs r0, #1
	movs r1, #2
	bl 0x0200e064
	movs r2, #60
	movs r0, #21
	ldr r1, [pc, #812]
	bl 0x0200e0c4
	movs r0, #5
	movs r1, #3
	bl 0x0200e05c
	movs r1, #3
	movs r0, #0
	bl 0x0200e064
	movs r0, #80
	bl 0x0200dfb4
	movs r1, #128
	movs r2, #30
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r1, #3
	movs r0, #1
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r1, #4
	movs r0, #5
	bl 0x0200e04c
	movs r0, #80
	bl 0x0200dfb4
	movs r1, #3
	movs r0, #21
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl 0x0200e09c
	movs r1, #176
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #0
	movs r2, #60
	bl 0x0200e0ac
	movs r1, #128
	movs r2, #60
	movs r0, #21
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r1, #4
	movs r0, #21
	bl 0x0200e04c
	movs r0, #60
	bl 0x0200dfb4
	movs r0, #21
	movs r1, #0
	movs r2, #80
	bl 0x0200e0ac
	movs r0, #21
	ldr r1, [pc, #648]
	movs r2, #80
	bl 0x0200e0c4
	movs r0, #21
	movs r1, #0
	movs r2, #60
	bl 0x0200e09c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r0, #0
	ldr r1, [pc, #620]
	movs r2, #0
	bl 0x0200e0c4
	movs r0, #5
	ldr r1, [pc, #608]
	movs r2, #0
	bl 0x0200e0c4
	movs r2, #60
	movs r0, #1
	ldr r1, [pc, #596]
	bl 0x0200e0c4
	movs r1, #4
	movs r0, #21
	bl 0x0200e04c
	movs r0, #30
	bl 0x0200dfb4
	movs r2, #60
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r0, #1
	movs r1, #2
	bl 0x0200e05c
	movs r1, #2
	movs r0, #5
	bl 0x0200e064
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #60
	bl 0x0200e0ac
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #30
	bl 0x0200e0ac
	movs r2, #30
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #5
	bl 0x0200e04c
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r2, #30
	movs r0, #21
	movs r1, #0
	bl 0x0200e0ac
	movs r1, #4
	movs r0, #21
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r1, #3
	movs r0, #5
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #3
	movs r0, #1
	bl 0x0200e064
	movs r0, #10
	bl 0x0200dfb4
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200e09c
	movs r1, #208
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #128
	movs r2, #0
	movs r0, #21
	lsls r1, r1, #1
	bl 0x0200e0c4
	movs r1, #3
	movs r0, #21
	bl 0x0200e064
	movs r0, #30
	bl 0x0200dfb4
	movs r2, #60
	movs r0, #21
	movs r1, #0
	bl 0x0200e09c
	movs r0, #1
	movs r1, #3
	bl 0x0200e064
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	lsls r1, r1, #9
	movs r0, #1
	bl 0x0200dfec
	movs r0, #1
	bl 0x0200dfe4
	movs r1, #0
	bl 0x0200df4c
	movs r0, #1
	movs r1, #4
	movs r2, #0
	bl 0x0200e054
	movs r1, #199
	movs r2, #207
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #1
	bl 0x0200e024
	movs r0, #60
	bl 0x0200dfb4
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #192
	movs r2, #60
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r1, #3
	movs r0, #0
	bl 0x0200e04c
	movs r0, #60
	bl 0x0200dfb4
	movs r1, #3
	movs r0, #21
	bl 0x0200e04c
	movs r0, #60
	bl 0x0200dfb4
	movs r0, #1
	bl 0x0200dfe4
	adds r7, r0, #0
	adds r2, r7, #0
	adds r2, #90
	ldrb r3, [r2]
	movs r5, #1
	orrs r3, r5
	strb r3, [r2]
	movs r0, #5
	bl 0x0200dfe4
	adds r7, r0, #0
	adds r2, r7, #0
	adds r2, #90
	ldrb r3, [r2]
	orrs r3, r5
	strb r3, [r2]
	movs r0, #0
	bl 0x0200dfe4
	movs r1, #128
	movs r2, #128
	adds r7, r0, #0
	lsls r1, r1, #9
	movs r0, #1
	lsls r2, r2, #8
	bl 0x0200dfec
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200dfec
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200e0ac
	movs r3, #10
	ldrsh r1, [r7, r3]
	movs r3, #18
	ldrsh r2, [r7, r3]
	adds r1, #16
	movs r0, #5
	bl 0x0200e01c
	movs r2, #10
	ldrsh r1, [r7, r2]
	movs r3, #18
	ldrsh r2, [r7, r3]
	adds r1, #16
	subs r2, #16
	movs r0, #1
	bl 0x0200e024
	movs r0, #1
	bl 0x0200e034
	movs r1, #160
	movs r2, #30
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200e0ac
	movs r0, #1
	movs r1, #3
	bl 0x0200e044
	movs r0, #5
	movs r1, #3
	bl 0x0200e044
	movs r1, #3
	movs r0, #0
	bl 0x0200e04c
	movs r0, #40
	bl 0x0200dfb4
	movs r2, #10
	ldrsh r1, [r7, r2]
	movs r0, #5
	movs r3, #18
	ldrsh r2, [r7, r3]
	bl 0x0200e024
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl 0x0200e03c
	movs r2, #10
	ldrsh r1, [r7, r2]
	movs r0, #1
	movs r3, #18
	ldrsh r2, [r7, r3]
	bl 0x0200e024
	movs r2, #0
	movs r0, #1
	movs r1, #0
	bl 0x0200e03c
	movs r0, #1
	movs r1, #5
	bl 0x0200dfcc
	b .L_02004b66_4
	.2byte 0x0000
	.4byte 0x0000033b
	.4byte 0x00000193
	.4byte 0x0000e666
	.4byte 0x00004ccc
	.4byte 0x00002666
	.4byte 0x00000357
	.4byte 0x00000105
	.4byte 0x00000101
.L_02004b66_4:
	movs r1, #160
	ldr r0, [pc, #228]
	lsls r1, r1, #16
	ldr r2, [pc, #228]
	movs r3, #1
	bl 0x0200e0e4
	movs r3, #0
	movs r0, #0
	movs r1, #13
	movs r2, #10
	bl 0x0200b380
	movs r1, #188
	movs r2, #228
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e024
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200e0ac
	movs r0, #21
	bl 0x0200dfe4
	adds r0, #90
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r3, #0
	movs r0, #21
	movs r1, #6
	movs r2, #5
	bl 0x0200b380
	movs r0, #21
	ldr r1, [pc, #156]
	ldr r2, [pc, #160]
	bl 0x0200e024
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200e0ac
	movs r1, #192
	movs r2, #40
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200e0ac
	movs r0, #21
	movs r1, #3
	bl 0x0200e044
	movs r1, #3
	movs r0, #0
	bl 0x0200e04c
	movs r0, #20
	bl 0x0200dfb4
	movs r1, #1
	movs r0, #0
	bl 0x0200e0d4
	bl 0x0200e0ec
	movs r0, #100
	bl 0x0200dfb4
	movs r3, #20
	movs r2, #50
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #46
	movs r2, #8
	movs r3, #4
	movs r0, #49
	bl 0x0200df3c
	ldr r0, [pc, #72]
	bl 0x0200dfa4
	ldr r0, [pc, #72]
	bl 0x0200dfac
	mov r2, r11
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	movs r3, #160
	mov r2, r11
	lsls r3, r3, #16
	str r3, [r2, #12]
	movs r3, #128
	lsls r3, r3, #24
	movs r6, #0
	str r3, [r2, #60]
	str r6, [r2, #40]
	bl 0x0200dfc4
	sub sp, #-28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x01790000
	.4byte 0x03770000
	.4byte 0x00000175
	.4byte 0x00000377
	.4byte 0x00000202
	.4byte 0x0000012f
	.global Func_02005594
	.thumb_func
Func_02005594:
	push {lr}
	movs r0, #140
	movs r1, #0
	bl 0x0200e124
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020055a4
	.thumb_func
Func_020055a4:
	push {lr}
	bl 0x0200e12c
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020055b0
	.thumb_func
Func_020055b0:
	push {lr}
	movs r0, #1
	bl 0x0200dfe4
	bl 0x0200dc5c
	pop {r0}
	bx r0
	.global Func_020055c0
	.thumb_func
Func_020055c0:
	push {lr}
	movs r0, #0
	bl 0x0200dfe4
	bl 0x0200dc5c
	pop {r0}
	bx r0
	.global Func_020055d0
	.thumb_func
Func_020055d0:
	push {lr}
	movs r0, #9
	bl 0x0200dfe4
	bl 0x0200dc98
	pop {r0}
	bx r0
	.global Func_020055e0
	.thumb_func
Func_020055e0:
	push {lr}
	movs r0, #23
	bl 0x0200dfe4
	bl 0x0200dc98
	pop {r0}
	bx r0
	.global Func_020055f0
	.thumb_func
Func_020055f0:
	push {lr}
	movs r0, #24
	bl 0x0200dfe4
	bl 0x0200dc98
	pop {r0}
	bx r0
	.global Func_02005600
	.thumb_func
Func_02005600:
	push {lr}
	movs r0, #25
	bl 0x0200dfe4
	bl 0x0200dc98
	pop {r0}
	bx r0
	.global Func_02005610
	.thumb_func
Func_02005610:
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
	bl 0x0200e160
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x030001d8
	.global Func_0200564c
	.thumb_func
Func_0200564c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	str r2, [sp, #4]
	movs r2, #0
	adds r5, r0, #0
	adds r6, r3, #0
	str r2, [sp, #0]
	movs r3, #91
	adds r3, r3, r5
	mov r10, r3
	ldrb r3, [r3]
	mov r11, r1
	cmp r3, #1
	bne .L_0200564c_0
	movs r2, #98
	adds r2, r2, r5
	ldrb r3, [r2]
	mov r9, r2
	cmp r3, #0
	bne .L_0200564c_1
	movs r1, #1
	bl 0x0200def4
	movs r0, #1
	b .L_0200564c_2
.L_0200564c_0:
	movs r3, #98
	adds r3, r3, r5
	mov r9, r3
.L_0200564c_1:
	movs r2, #8
	adds r2, r2, r5
	mov r7, r11
	mov r8, r2
	adds r7, #8
	adds r0, r7, #0
	mov r1, r8
	bl 0x0200d610
	ldr r3, [sp, #4]
	cmp r0, r3
	blt .L_0200564c_3
	cmp r6, #0
	beq .L_0200564c_4
.L_0200564c_3:
	mov r2, r11
	ldr r0, [r2, #16]
	ldr r3, [r5, #16]
	mov r2, r8
	subs r0, r0, r3
	ldr r1, [r7]
	ldr r3, [r2]
	subs r1, r1, r3
	bl 0x0200deac
	ldr r3, [pc, #100]
	lsls r0, r0, #16
	movs r2, #128
	lsrs r0, r0, #16
	lsls r2, r2, #5
	adds r4, r0, r3
	adds r1, r0, r2
	movs r3, #240
	ldrh r2, [r5, #6]
	lsls r3, r3, #8
	ands r4, r3
	ands r1, r3
	ands r0, r3
	ands r3, r2
	cmp r0, r3
	beq .L_0200564c_5
	cmp r1, r3
	beq .L_0200564c_5
	cmp r4, r3
	beq .L_0200564c_5
	cmp r6, #0
	beq .L_0200564c_4
.L_0200564c_5:
	movs r3, #1
	mov r2, r10
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200def4
	movs r3, #1
	mov r2, r9
	str r3, [sp, #0]
	strb r3, [r2]
	b .L_0200564c_6
.L_0200564c_4:
	mov r3, r10
	strb r6, [r3]
	adds r0, r5, #0
	movs r1, #2
	bl 0x0200def4
	mov r2, r9
	strb r6, [r2]
.L_0200564c_6:
	ldr r0, [sp, #0]
.L_0200564c_2:
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0xfffff000
	.global Func_0200572c
	.thumb_func
Func_0200572c:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #0
	bl 0x0200dfe4
	movs r3, #128
	ldr r2, [r5, #56]
	lsls r3, r3, #24
	adds r1, r0, #0
	cmp r2, r3
	bne .L_0200572c_0
	ldr r3, [r5, #64]
	movs r0, #0
	cmp r3, r2
	beq .L_0200572c_1
.L_0200572c_0:
	adds r0, r5, #0
	movs r2, #18
	movs r3, #0
	bl 0x0200d64c
	movs r0, #0
.L_0200572c_1:
	pop {r5}
	pop {r1}
	bx r1
	.global Func_0200575c
	.thumb_func
Func_0200575c:
	push {r5, r6, lr}
	adds r6, r0, #0
	sub sp, #12
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	bl 0x0200dea4
	ldr r3, [r6, #12]
	ldr r2, [pc, #124]
	lsls r0, r0, #4
	subs r3, r3, r0
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl 0x0200dea4
	adds r6, r0, #0
	bl 0x0200dea4
	adds r1, r0, #0
	lsls r0, r6, #1
	adds r0, r0, r6
	adds r2, r5, #0
	lsls r0, r0, #4
	bl 0x0200dec4
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	ldr r0, [pc, #84]
	bl 0x0200df04
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200575c_0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
	ldr r3, [pc, #68]
	adds r2, #9
	str r3, [r5, #72]
	movs r3, #12
	strh r3, [r2]
	movs r1, #0
	bl 0x0200df4c
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200def4
	ldr r1, [pc, #48]
	adds r0, r5, #0
	bl 0x0200defc
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
.L_0200575c_0:
	movs r0, #138
	bl 0x0200e14c
	sub sp, #-12
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0xfff80000
	.4byte 0x0000011d
	.4byte 0x00001999
	.4byte 0x0200e6e0
	.global Func_020057fc
	.thumb_func
Func_020057fc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r0, #154
	bl 0x0200e14c
	ldr r5, [pc, #212]
	movs r2, #30
	mov r8, r2
.L_020057fc_0:
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r2, #128
	ldrh r3, [r7, #6]
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r7, #6]
	ldr r3, [r7, #24]
	adds r3, r3, r5
	str r3, [r7, #24]
	ldr r3, [r7, #28]
	adds r3, r3, r5
	str r3, [r7, #28]
	movs r0, #1
	bl 0x0200de8c
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r2, r8
	cmp r2, #0
	bge .L_020057fc_0
	movs r3, #7
	mov r8, r3
.L_020057fc_2:
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	ldr r0, [pc, #148]
	bl 0x0200df04
	adds r6, r0, #0
	cmp r6, #0
	beq .L_020057fc_1
	movs r1, #0
	bl 0x0200df4c
	ldr r1, [pc, #136]
	adds r0, r6, #0
	bl 0x0200defc
	bl 0x0200dea4
	movs r3, #128
	lsls r3, r3, #9
	adds r2, r6, #0
	adds r2, #85
	adds r0, r0, r3
	str r3, [r6, #52]
	movs r3, #2
	str r0, [r6, #48]
	strb r3, [r2]
	ldr r3, [pc, #108]
	str r3, [r6, #72]
	bl 0x0200dea4
	adds r5, r0, #0
	bl 0x0200dea4
	subs r5, r5, r0
	str r5, [r6, #40]
	bl 0x0200dea4
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r2, #128
	lsls r2, r2, #12
	lsls r5, r5, #3
	adds r5, r5, r2
	bl 0x0200dea4
	adds r1, r5, #0
	adds r2, r0, #0
	adds r0, r6, #0
	bl 0x0200d8f0
.L_020057fc_1:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r2, r8
	cmp r2, #0
	bge .L_020057fc_2
	movs r0, #131
	bl 0x0200e14c
	movs r3, #128
	movs r2, #0
	lsls r3, r3, #24
	str r2, [r7, #8]
	str r2, [r7, #12]
	str r2, [r7, #16]
	str r3, [r7, #56]
	str r3, [r7, #60]
	str r3, [r7, #64]
	str r2, [r7, #36]
	str r2, [r7, #40]
	str r2, [r7, #44]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0xfffff800
	.4byte 0x0000011d
	.4byte 0x0200e6e4
	.4byte 0x00000a3d
	.global Func_020058f0
	.thumb_func
Func_020058f0:
	push {r5, r6, lr}
	adds r6, r0, #0
	sub sp, #12
	adds r0, r1, #0
	adds r1, r2, #0
	cmp r6, #0
	beq .L_020058f0_0
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	adds r2, r5, #0
	str r3, [r5, #8]
	bl 0x0200dec4
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	adds r0, r6, #0
	bl 0x0200df1c
.L_020058f0_0:
	sub sp, #-12
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02005928
	.thumb_func
Func_02005928:
	push {lr}
	sub sp, #8
	movs r3, #22
	movs r2, #36
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #17
	movs r1, #0
	movs r2, #3
	movs r3, #1
	bl 0x0200df3c
	bl 0x020080c4
	bl 0x0200d950
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02005950
	.thumb_func
Func_02005950:
	push {r5, lr}
	sub sp, #8
	movs r3, #22
	movs r2, #36
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #17
	movs r1, #0
	movs r2, #3
	movs r3, #1
	bl 0x0200df3c
	ldr r0, [pc, #144]
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02005950_0
	movs r0, #21
	b .L_02005950_1
.L_02005950_0:
	movs r0, #20
.L_02005950_1:
	bl 0x0200dfe4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02005950_2
	movs r0, #197
	lsls r0, r0, #2
	bl 0x0200dfac
	ldr r0, [pc, #116]
	bl 0x0200dfac
	ldr r0, [pc, #112]
	bl 0x0200dfac
	ldr r3, [r5, #8]
	asrs r0, r3, #20
	cmp r0, #22
	bne .L_02005950_3
	movs r3, #36
	str r0, [sp, #0]
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #1
	movs r2, #1
	movs r3, #1
	bl 0x0200df3c
	movs r0, #197
	lsls r0, r0, #2
	bl 0x0200dfa4
	b .L_02005950_2
.L_02005950_3:
	cmp r0, #23
	bne .L_02005950_4
	movs r3, #36
	str r0, [sp, #0]
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #1
	movs r2, #1
	movs r3, #1
	bl 0x0200df3c
	ldr r0, [pc, #44]
	bl 0x0200dfa4
	b .L_02005950_2
.L_02005950_4:
	movs r3, #24
	movs r2, #36
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #17
	movs r1, #1
	movs r2, #1
	movs r3, #1
	bl 0x0200df3c
	ldr r0, [pc, #20]
	bl 0x0200dfa4
.L_02005950_2:
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000087a
	.4byte 0x00000315
	.4byte 0x00000316
	.global Func_02005a08
	.thumb_func
Func_02005a08:
	push {lr}
	ldr r3, [pc, #20]
	ldr r3, [r3]
	movs r2, #15
	ands r3, r2
	cmp r3, #0
	bne .L_02005a08_0
	movs r0, #131
	bl 0x0200e14c
.L_02005a08_0:
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.global Func_02005a24
	.thumb_func
Func_02005a24:
	push {lr}
	bl 0x0200dfbc
	ldr r0, [pc, #16]
	movs r1, #1
	bl 0x0200df7c
	bl 0x0200dfc4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000ee4
	.global Func_02005a40
	.thumb_func
Func_02005a40:
	push {r5, lr}
	ldr r0, [pc, #76]
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02005a40_0
	movs r0, #131
	lsls r0, r0, #1
	bl 0x0200df9c
	adds r5, r0, #0
	cmp r5, #0
	bne .L_02005a40_1
	movs r0, #22
	bl 0x0200dfe4
	adds r0, #91
	strb r5, [r0]
	ldr r0, [pc, #40]
	bl 0x0200dfac
	b .L_02005a40_1
.L_02005a40_0:
	movs r0, #131
	lsls r0, r0, #1
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02005a40_1
	movs r0, #22
	bl 0x0200dfe4
	movs r3, #1
	adds r0, #91
	strb r3, [r0]
	ldr r0, [pc, #8]
	bl 0x0200dfa4
.L_02005a40_1:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000241
	.global Func_02005a94
	.thumb_func
Func_02005a94:
	push {r5, lr}
	movs r0, #0
	bl 0x0200dfe4
	adds r5, r0, #0
	ldr r0, [pc, #60]
	bl 0x0200df9c
	cmp r0, #0
	beq .L_02005a94_0
	movs r0, #21
	bl 0x0200dfe4
	b .L_02005a94_1
.L_02005a94_0:
	movs r0, #20
	bl 0x0200dfe4
.L_02005a94_1:
	cmp r0, #0
	beq .L_02005a94_2
	movs r2, #200
	ldr r3, [r5, #12]
	lsls r2, r2, #16
	cmp r3, r2
	ble .L_02005a94_3
	adds r2, r0, #0
	adds r2, #35
	movs r3, #3
	b .L_02005a94_4
.L_02005a94_3:
	adds r2, r0, #0
	adds r2, #35
	movs r3, #1
.L_02005a94_4:
	strb r3, [r2]
.L_02005a94_2:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000087a
	.global Func_02005ae0
	.thumb_func
Func_02005ae0:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r0, [r6, #48]
	ldr r7, [r6, #80]
	bl 0x0200deb4
	lsls r5, r0, #1
	cmp r5, #0
	ble .L_02005ae0_0
	negs r5, r5
.L_02005ae0_0:
	ldr r0, [r6, #48]
	bl 0x0200debc
	ldr r3, [r6, #56]
	lsls r0, r0, #1
	adds r3, r3, r0
	str r3, [r6, #8]
	ldr r0, [r6, #48]
	ldr r3, [r6, #60]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r5
	adds r0, r0, r2
	str r3, [r6, #12]
	bl 0x0200debc
	cmp r0, #0
	bge .L_02005ae0_1
	adds r0, #7
.L_02005ae0_1:
	asrs r3, r0, #3
	strh r3, [r7, #30]
	bl 0x0200dea4
	adds r5, r0, #0
	bl 0x0200dea4
	lsls r5, r5, #9
	lsls r0, r0, #9
	ldr r3, [r6, #48]
	lsrs r0, r0, #16
	lsrs r5, r5, #16
	adds r5, r5, r0
	movs r2, #128
	adds r3, r3, r5
	lsls r2, r2, #3
	adds r3, r3, r2
	str r3, [r6, #48]
	movs r0, #0
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02005b48
	.thumb_func
Func_02005b48:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	bl 0x0200dfe4
	adds r7, r0, #0
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
	strb r2, [r6, #9]
	movs r2, #0
	mov r8, r2
	adds r3, r6, #0
	adds r3, #39
	mov r2, r8
	strb r2, [r3]
	movs r1, #0
	bl 0x0200df4c
	movs r3, #92
	adds r3, r3, r7
	mov r2, r8
	strb r2, [r3]
	mov r10, r3
	adds r3, r7, #0
	adds r3, #85
	strb r2, [r3]
	ldr r0, [pc, #124]
	bl 0x0200df9c
	cmp r0, #0
	bne .L_02005b48_0
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #14
	adds r3, r3, r2
	str r3, [r7, #12]
.L_02005b48_0:
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
	bl 0x0200decc
	adds r5, r0, #0
	movs r0, #181
	bl 0x0200df8c
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	adds r2, r5, #0
	movs r1, #128
	ldrb r0, [r6, #28]
	bl 0x0200dee4
	movs r0, #17
	bl 0x0200ded4
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
	.4byte 0x00000109
	.4byte 0x0200dae1
	.global Func_02005c20
	.thumb_func
Func_02005c20:
	push {r5, lr}
	ldr r3, [pc, #52]
	ldr r3, [r3]
	movs r2, #2
	ands r3, r2
	adds r5, r0, #0
	cmp r3, #0
	beq .L_02005c20_0
	movs r1, #7
	bl 0x0200df74
	b .L_02005c20_1
.L_02005c20_0:
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200df74
.L_02005c20_1:
	ldr r3, [pc, #20]
	ldr r3, [r3]
	movs r2, #15
	ands r3, r2
	cmp r3, #0
	bne .L_02005c20_2
	adds r0, r5, #0
	bl 0x0200dd68
.L_02005c20_2:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.global Func_02005c5c
	.thumb_func
Func_02005c5c:
	push {r5, r6, lr}
	ldr r5, [pc, #52]
	ldr r3, [r5]
	movs r2, #1
	ands r3, r2
	adds r6, r0, #0
	cmp r3, #0
	beq .L_02005c5c_0
	ldr r0, [r5]
	movs r1, #6
	lsrs r0, r0, #1
	bl 0x0200de84
	adds r1, r0, #0
	adds r0, r6, #0
	bl 0x0200df74
.L_02005c5c_0:
	ldr r3, [r5]
	movs r2, #15
	ands r3, r2
	cmp r3, #0
	bne .L_02005c5c_1
	adds r0, r6, #0
	bl 0x0200dd68
.L_02005c5c_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.global Func_02005c98
	.thumb_func
Func_02005c98:
	push {r5, lr}
	adds r5, r0, #0
	ldr r0, [pc, #32]
	ldr r3, [r0]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02005c98_0
	ldr r0, [r0]
	movs r1, #6
	lsrs r0, r0, #1
	bl 0x0200de84
	adds r1, r0, #0
	adds r0, r5, #0
	bl 0x0200df74
.L_02005c98_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.global Func_02005cc4
	.thumb_func
Func_02005cc4:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #100
	ldrh r3, [r2]
	adds r3, #1
	ldr r6, [r5, #104]
	strh r3, [r2]
	lsls r3, r3, #16
	asrs r0, r3, #16
	cmp r0, #31
	ble .L_02005cc4_0
	adds r0, r5, #0
	bl 0x0200df0c
	b 0x0200dd0e
.L_02005cc4_0:
	lsls r0, r0, #10
	bl 0x0200deb4
	str r0, [r5, #24]
	str r0, [r5, #28]
	ldr r3, [r6, #8]
	movs r1, #128
	str r3, [r5, #8]
	ldr r3, [r5, #12]
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r5, #12]
	subs r1, r1, r0
	ldr r3, [r6, #16]
	lsls r2, r1, #2
	adds r2, r2, r1
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5, #16]
.L_02005d0e:
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02005d14
	.thumb_func
Func_02005d14:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #100
	ldrh r3, [r2]
	adds r3, #1
	ldr r6, [r5, #104]
	strh r3, [r2]
	lsls r3, r3, #16
	asrs r0, r3, #16
	cmp r0, #31
	ble .L_02005d14_0
	adds r0, r5, #0
	bl 0x0200df0c
	b 0x0200dd60
.L_02005d14_0:
	lsls r0, r0, #10
	bl 0x0200deb4
	negs r3, r0
	str r0, [r5, #24]
	str r3, [r5, #28]
	ldr r3, [r6, #8]
.L_02005d42:
	movs r1, #128
	str r3, [r5, #8]
	ldr r3, [r5, #12]
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r5, #12]
	subs r1, r1, r0
	ldr r3, [r6, #16]
	lsls r2, r1, #2
	adds r2, r2, r1
	subs r3, r3, r2
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r5, #16]
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02005d68
	.thumb_func
Func_02005d68:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #72]
	ldr r3, [r3]
	sub sp, #8
	movs r1, #63
	adds r6, r0, #0
	mov r11, r3
	movs r7, #0
	mov r10, sp
	mov r9, r1
.L_02005d68_2:
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	ldr r1, [r6, #8]
	movs r0, #26
	bl 0x0200df04
	lsls r3, r7, #2
	mov r2, r10
	str r0, [r3, r2]
	cmp r0, #0
	beq .L_02005d68_0
	ldr r3, [r6, #20]
	str r3, [r0, #20]
	adds r3, r0, #0
	ldr r5, [r0, #80]
	adds r3, #85
	movs r2, #0
	ldr r1, [pc, #16]
	strb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	mov r8, r1
	str r6, [r0, #104]
	cmp r5, #0
	beq .L_02005d68_0
	b .L_02005d68_1
	.4byte 0x00000000
	.4byte 0x03001f30
.L_02005d68_1:
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200deec
	adds r3, r5, #0
	adds r3, #38
	mov r2, r8
	strb r2, [r3]
	ldrb r0, [r5, #28]
	bl 0x0200dedc
	mov r3, r11
	adds r3, #70
	ldrh r3, [r3]
	strb r3, [r5, #28]
	ldrb r3, [r5, #29]
	movs r2, #1
	orrs r3, r2
	strb r3, [r5, #29]
	ldrb r3, [r5, #28]
	ldr r2, [pc, #64]
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r1, [r3, #2]
	ldr r2, [pc, #52]
	ldrh r3, [r5, #8]
	lsls r1, r1, #17
	lsrs r1, r1, #22
	ands r3, r2
	orrs r3, r1
	movs r1, #33
	negs r1, r1
	strh r3, [r5, #8]
	ldrb r3, [r5, #5]
	adds r2, r1, #0
	ands r3, r2
	mov r2, r9
	ands r3, r2
	movs r2, #64
	orrs r3, r2
	ldrb r2, [r5, #7]
	strb r3, [r5, #5]
	mov r3, r9
	ands r3, r2
	movs r2, #128
	orrs r3, r2
	strb r3, [r5, #7]
	ldr r3, [r5, #40]
	mov r1, r8
	strb r1, [r3, #22]
	b .L_02005d68_0
	.2byte 0x0000
	.4byte 0xfffffc00
	.4byte 0x03001b10
.L_02005d68_0:
	adds r7, #1
	cmp r7, #1
	ble .L_02005d68_2
	ldr r2, [sp, #0]
	ldr r3, [pc, #60]
	ldr r0, [r2, #80]
	str r3, [r2, #108]
	movs r2, #13
	ldrb r1, [r0, #9]
	negs r2, r2
	adds r3, r2, #0
	movs r4, #4
	ands r3, r1
	orrs r3, r4
	strb r3, [r0, #9]
	mov r3, r10
	ldr r1, [r3, #4]
	ldr r0, [r1, #80]
	ldrb r3, [r0, #9]
	ands r2, r3
	ldr r3, [pc, #32]
	orrs r2, r4
	str r3, [r1, #108]
	adds r1, #35
	movs r3, #2
	strb r2, [r0, #9]
	strb r3, [r1]
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200dd15
	.4byte 0x0200dcc5
	.include "games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/IMPORT.INC"
@ The compiler library links here from its licensed container.
	.section .text.part1,"ax",%progbits
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
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00004000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000002
	.4byte 0x80010000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01a40000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00d40000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d40000
	.4byte 0x00000000
	.4byte 0x01840000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00d40000
	.4byte 0x00000000
	.4byte 0x01ad0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00d40000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x000007ae
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x03700000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01410000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x010a0000
	.4byte 0x00000000
	.4byte 0x04ce0000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00001999
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00001999
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000f5c
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000147a
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000018
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001b
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00001000
	.4byte 0x0000001c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000001b
	.4byte 0xffff0000
	.4byte 0x000000a7
	.4byte 0x40000501
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000100
	.4byte 0x400001b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000071
	.4byte 0x4000012f
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000026
	.4byte 0x0000027c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x0000001d
	.4byte 0x00000318
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000001ca
	.4byte 0x80000571
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000196
	.4byte 0x400002e7
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00000106
	.4byte 0x40000335
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00000154
	.4byte 0x40000388
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x00000146
	.4byte 0x40000476
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000176
	.4byte 0x400004e6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x00000066
	.4byte 0x400004c6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0010
	.4byte 0x00000190
	.4byte 0xc0000354
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0011
	.4byte 0x00000190
	.4byte 0xc0000354
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00101013
	.4byte 0x00208005
	.4byte 0x00301006
	.4byte 0x00402006
	.4byte 0x00506007
	.4byte 0x00605007
	.4byte 0x00706008
	.4byte 0x00802007
	.4byte 0x00901007
	.4byte 0x00a02007
	.4byte 0x00b01009
	.4byte 0x00c11004
	.4byte 0x000001ff
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x038c0000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000003
	.4byte 0x005c0000
	.4byte 0x00000000
	.4byte 0x01fc0000
	.4byte 0x00004000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x026c0000
	.4byte 0x00008000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00a40000
	.4byte 0x00000000
	.4byte 0x04d40000
	.4byte 0x0001c000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x010c0000
	.4byte 0x00000000
	.4byte 0x04fc0000
	.4byte 0x00018000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x01140000
	.4byte 0x00000000
	.4byte 0x04140000
	.4byte 0x0000c000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00130000
	.4byte 0x00000000
	.4byte 0x02660000
	.4byte 0x00000000
	.4byte 0xffff0068
	.4byte 0x00000004
	.4byte 0x00320000
	.4byte 0x00000000
	.4byte 0x031b0000
	.4byte 0x00008000
	.4byte 0xffff0068
	.4byte 0x00000004
	.4byte 0x00410000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00008000
	.4byte 0xffff0068
	.4byte 0x00000004
	.4byte 0x00390000
	.4byte 0x00000000
	.4byte 0x03250000
	.4byte 0x00008000
	.4byte 0xffff002a
	.4byte 0x00000001
	.4byte 0x019c0000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00018000
	.4byte 0xffff0025
	.4byte 0x00000001
	.4byte 0x018e0000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0001c000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x017a0000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00020000
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x01750000
	.4byte 0x00000000
	.4byte 0x03790000
	.4byte 0x00004000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ce
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ce
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ce
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0fd00016
	.4byte 0x00000007
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x038c0000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000003
	.4byte 0x005c0000
	.4byte 0x00000000
	.4byte 0x01fc0000
	.4byte 0x00004000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x026c0000
	.4byte 0x00008000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00a40000
	.4byte 0x00000000
	.4byte 0x04d40000
	.4byte 0x0001c000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x010c0000
	.4byte 0x00000000
	.4byte 0x04fc0000
	.4byte 0x00018000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x01140000
	.4byte 0x00000000
	.4byte 0x04140000
	.4byte 0x0000c000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00130000
	.4byte 0x00000000
	.4byte 0x02660000
	.4byte 0x00000000
	.4byte 0xffff0068
	.4byte 0x00000004
	.4byte 0x00320000
	.4byte 0x00000000
	.4byte 0x031b0000
	.4byte 0x00008000
	.4byte 0xffff0068
	.4byte 0x00000004
	.4byte 0x00410000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00008000
	.4byte 0xffff0068
	.4byte 0x00000004
	.4byte 0x00390000
	.4byte 0x00000000
	.4byte 0x03250000
	.4byte 0x00008000
	.4byte 0xffff002a
	.4byte 0x00000001
	.4byte 0x019c0000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00038000
	.4byte 0xffff0025
	.4byte 0x00000001
	.4byte 0x018e0000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0003c000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x017a0000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00020000
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x014f0000
	.4byte 0x00000000
	.4byte 0x03650000
	.4byte 0x00004000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ce
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ce
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ce
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0fd00016
	.4byte 0x00000007
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff002a
	.4byte 0x00000001
	.4byte 0x019c0000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x0003d000
	.4byte 0xffff0025
	.4byte 0x00000001
	.4byte 0x01640000
	.4byte 0x00000000
	.4byte 0x02d70000
	.4byte 0x0000b000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00a40000
	.4byte 0x00000000
	.4byte 0x04d40000
	.4byte 0x0001c000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x010c0000
	.4byte 0x00000000
	.4byte 0x04fc0000
	.4byte 0x00028000
	.4byte 0xffff00d6
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00d6
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00d6
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x038c0000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000003
	.4byte 0x005c0000
	.4byte 0x00000000
	.4byte 0x01fc0000
	.4byte 0x00024000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x026c0000
	.4byte 0x00018000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x01140000
	.4byte 0x00000000
	.4byte 0x04140000
	.4byte 0x0002c000
	.4byte 0x0fd00016
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x017a0000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x01990000
	.4byte 0x00000000
	.4byte 0x046e0000
	.4byte 0x0000c000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x01ac0000
	.4byte 0x00000000
	.4byte 0x024c0000
	.4byte 0x00008000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00cc0000
	.4byte 0x00000000
	.4byte 0x039c0000
	.4byte 0x00000000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x018a0000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00024000
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x01750000
	.4byte 0x00000000
	.4byte 0x03790000
	.4byte 0x0000c000
	.4byte 0xffff00ce
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00ce
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00ce
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00d7
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00000f7f
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00000f80
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008add
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00000f79
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00000f7a
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00000f84
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008b29
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008ba9
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00000f67
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008c61
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00000fd1
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008dc1
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008df1
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008e19
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008e2d
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x02008e41
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008e55
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008e85
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008eb5
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008f5d
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x02008f8d
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x02008fbd
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x02008d71
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x02008d99
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x020090d9
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x020091d9
	.4byte 0x00000002
	.4byte 0x08230032
	.4byte 0x020092bd
	.4byte 0x00000002
	.4byte 0xffff0033
	.4byte 0x02009455
	.4byte 0x00000002
	.4byte 0xffff0034
	.4byte 0x02009555
	.4byte 0x00000002
	.4byte 0xffff0035
	.4byte 0x02009591
	.4byte 0x0000c602
	.4byte 0xffff0064
	.4byte 0x02009245
	.4byte 0x00004602
	.4byte 0xffff0065
	.4byte 0x02009281
	.4byte 0x00009415
	.4byte 0x0fd0001a
	.4byte 0x02008a45
	.4byte 0x00008c15
	.4byte 0xffff0014
	.4byte 0x0200d951
	.4byte 0x00000602
	.4byte 0xffff001e
	.4byte 0x0200d929
	.4byte 0x00008602
	.4byte 0xffff001f
	.4byte 0x0200d929
	.4byte 0x00000003
	.4byte 0xffff0018
	.4byte 0x0200da25
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000011ca
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000011cb
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008add
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000011c5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000011c6
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000011cd
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008b29
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000011b4
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000011b5
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x0000111d
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x0000111e
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000011e7
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000011e8
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000011f2
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000011f3
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000011f5
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000011f6
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000011f7
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000011f8
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000011f9
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008dc1
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008df1
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008e19
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008e2d
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x02008e41
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008e55
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008e85
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008eb5
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008f5d
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x02008f8d
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x02008fbd
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x02008d71
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x02008d99
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x020090d9
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x020091d9
	.4byte 0x00000002
	.4byte 0x08230032
	.4byte 0x020092bd
	.4byte 0x00000002
	.4byte 0xffff0033
	.4byte 0x02009455
	.4byte 0x00000002
	.4byte 0xffff0034
	.4byte 0x02009555
	.4byte 0x00000002
	.4byte 0xffff0035
	.4byte 0x02009591
	.4byte 0x0000c602
	.4byte 0xffff0064
	.4byte 0x02009245
	.4byte 0x00004602
	.4byte 0xffff0065
	.4byte 0x02009281
	.4byte 0x00009415
	.4byte 0x0fd0001a
	.4byte 0x02008a45
	.4byte 0x00008c15
	.4byte 0xffff0014
	.4byte 0x0200d951
	.4byte 0x00000602
	.4byte 0xffff001e
	.4byte 0x0200d929
	.4byte 0x00008602
	.4byte 0xffff001f
	.4byte 0x0200d929
	.4byte 0x00000003
	.4byte 0xffff0018
	.4byte 0x0200da25
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03030008
	.4byte 0x0200acb1
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001c62
	.4byte 0x00000000
	.4byte 0x03040009
	.4byte 0x0200af15
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001c63
	.4byte 0x00000000
	.4byte 0x081f000a
	.4byte 0x02008c9d
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001c8e
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001c90
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001c99
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008cd1
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001c9c
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x02008d2d
	.4byte 0x00008d15
	.4byte 0x03030408
	.4byte 0x0200acb1
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001c64
	.4byte 0x00008d15
	.4byte 0x03040409
	.4byte 0x0200af15
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001c65
	.4byte 0x00008d15
	.4byte 0x081f040a
	.4byte 0x02008c9d
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001c8f
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001c91
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001ca7
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001ca8
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001ca9
	.4byte 0x00008d15
	.4byte 0x03070413
	.4byte 0x02008d2d
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001caa
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008dc1
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008df1
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008e19
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008e2d
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x02008e41
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008e55
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008e85
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008eb5
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008f5d
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x02008f8d
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x02008fbd
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x02008d71
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x02008d99
	.4byte 0x0000c602
	.4byte 0xffff0064
	.4byte 0x02009245
	.4byte 0x00004602
	.4byte 0xffff0065
	.4byte 0x02009281
	.4byte 0x00009415
	.4byte 0x0fd00014
	.4byte 0x02008a75
	.4byte 0x00008c15
	.4byte 0xffff0015
	.4byte 0x0200d951
	.4byte 0x00000602
	.4byte 0xffff001e
	.4byte 0x0200d929
	.4byte 0x00008602
	.4byte 0xffff001f
	.4byte 0x0200d929
	.4byte 0x00000003
	.4byte 0xffff0018
	.4byte 0x0200da25
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00620000
	.4byte 0x00020002
	.4byte 0x00020002
	.4byte 0x00020062
	.4byte 0x00020002
	.4byte 0x0000ffff
	.4byte 0x00020060
	.4byte 0x00020002
	.4byte 0x00600002
	.4byte 0x00020002
	.4byte 0xffff0002
	.4byte 0x00620004
	.4byte 0x00020002
	.4byte 0x00040002
	.4byte 0x00020062
	.4byte 0x00020002
	.4byte 0x0004ffff
	.4byte 0x00020060
	.4byte 0x00020002
	.4byte 0x00600004
	.4byte 0x00020002
	.4byte 0xffff0002
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000003
	.4byte 0x00c70000
	.4byte 0x00000000
	.4byte 0x01be0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d70000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00ef0000
	.4byte 0x00000000
	.4byte 0x01cd0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c70000
	.4byte 0x00000000
	.4byte 0x01be0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d70000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00ef0000
	.4byte 0x00000000
	.4byte 0x01cd0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00020000
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000003
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01ca0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01ca0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01da0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01ca0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00020000
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01ca0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01da0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000010
