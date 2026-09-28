.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/GOMA_HASHIRA/ENTRY.INC"
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
	bl 0x02009524
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
	bl 0x02009450
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
	bl 0x020093f8
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
	bl 0x020093c8
	ldr r5, [pc, #132]
	movs r0, #15
	bl 0x020093a8
	movs r0, #185
	bl 0x02009510
	str r5, [r6, #48]
	str r5, [r6, #52]
	ldr r1, [r7]
.L_020000c4_5:
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl 0x020093e0
	mov r0, r8
	str r5, [r0, #48]
	str r5, [r0, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl 0x020093e0
	adds r0, r6, #0
	bl 0x020093e8
	bl 0x02009508
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
	bl 0x020093c8
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
	.4byte 0x02009554
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
	bl 0x020093f8
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
	.4byte 0x02009554
	.4byte 0xffff0000
	.4byte 0x02009594
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
	bl 0x02009450
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
	.4byte 0x02009594
	.4byte 0x020095ac
	.4byte 0x02009554
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
	bl 0x020093f8
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
	.4byte 0x020095ac
	.4byte 0x02009554
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
	bl 0x02009450
	ldrh r3, [r0, #6]
	ldr r0, [sp, #76]
	lsrs r3, r3, #12
	mov r8, r3
	bl 0x02009450
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
	bl 0x02009458
	movs r1, #8
	movs r0, #0
	bl 0x02009490
	movs r0, #15
	bl 0x02009438
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
	bl 0x02009478
	movs r0, #0
	bl 0x02009450
	ldr r3, [pc, #424]
	str r3, [r0, #108]
	movs r0, #4
	bl 0x02009438
	mov r3, r8
	subs r3, #6
	cmp r3, #7
	bhi .L_02000608_6
	adds r0, r7, #0
	movs r1, #3
	bl 0x020093c8
	b .L_02000608_7
.L_02000608_6:
	adds r0, r7, #0
	movs r1, #2
	bl 0x020093c8
.L_02000608_7:
	movs r0, #239
	bl 0x02009510
	adds r0, r7, #0
	ldr r1, [sp, #80]
	ldr r2, [sp, #84]
	ldr r3, [sp, #88]
	bl 0x020093e0
	movs r0, #0
	bl 0x02009480
	movs r0, #0
	movs r1, #2
	bl 0x02009490
	movs r0, #0
	ldr r1, [pc, #360]
	ldr r2, [pc, #344]
	bl 0x02009458
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
	bl 0x02009478
	ldr r3, [sp, #92]
	cmp r3, #0
	beq .L_02000608_8
	bl 0x02009524
.L_02000608_8:
	movs r0, #0
	bl 0x02009480
	movs r1, #1
	movs r0, #0
	bl 0x02009490
	movs r0, #0
	bl 0x02009450
	movs r2, #0
	str r2, [r0, #108]
	adds r0, r7, #0
	bl 0x020093e8
	movs r0, #144
	lsls r0, r0, #1
	bl 0x02009510
	movs r0, #213
	bl 0x02009510
	ldr r3, [sp, #80]
	str r3, [r7, #8]
	ldr r3, [sp, #88]
	str r3, [r7, #16]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	adds r0, r7, #0
	movs r1, #1
	bl 0x020093c8
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
	bl 0x020093f0
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
	bl 0x020093f0
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
	bl 0x02009508
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
	.2byte 0x95ac
	.2byte 0x0200
	.2byte 0x1999
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0x0001
	.2byte 0x82a9
	.2byte 0x0200
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0x9554
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
	bl 0x02009450
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
	bl 0x020093f0
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
	.2byte 0x9594
	.2byte 0x0200
	.4byte 0x020095ac
	.global Func_020009dc
	.thumb_func
Func_020009dc:
	push {lr}
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #1
	movs r2, #0
	bl 0x020094e0
	movs r0, #13
	movs r1, #2
	movs r2, #0
	bl 0x020094a0
	movs r0, #12
	movs r1, #40
	bl 0x020094e8
	pop {r0}
	bx r0
	.global Func_02000a00
	.thumb_func
Func_02000a00:
	push {lr}
	adds r3, r0, #0
	adds r3, #102
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	beq .L_02000a00_0
	cmp r3, #1
	bgt .L_02000a00_1
	cmp r3, #0
	beq .L_02000a00_2
	b .L_02000a00_3
.L_02000a00_1:
	cmp r3, #2
	beq .L_02000a00_4
	b .L_02000a00_3
.L_02000a00_2:
	ldr r3, [r0, #8]
	ldr r2, [r0, #48]
	adds r3, r3, r2
	str r3, [r0, #8]
	str r3, [r0, #56]
	ldr r2, [r0, #52]
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	str r3, [r0, #60]
	b .L_02000a00_3
.L_02000a00_0:
	ldr r3, [r0, #8]
	ldr r2, [r0, #48]
	adds r3, r3, r2
	str r3, [r0, #8]
	str r3, [r0, #56]
	b .L_02000a00_5
.L_02000a00_4:
	ldr r3, [r0, #12]
	ldr r2, [r0, #48]
	adds r3, r3, r2
	str r3, [r0, #12]
	str r3, [r0, #60]
.L_02000a00_5:
	ldr r2, [r0, #52]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
	str r3, [r0, #64]
.L_02000a00_3:
	pop {r0}
	bx r0
	.global Func_02000a58
	.thumb_func
Func_02000a58:
	push {lr}
	movs r1, #15
	bl 0x020094c0
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02000a68
	.thumb_func
Func_02000a68:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #0
	adds r6, r1, #0
	sub sp, #12
	mov r8, r2
	mov r9, r3
	bl 0x02009450
	ldr r3, [pc, #192]
	mov r10, sp
	mov r2, r10
	mov r11, r0
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	adds r2, r6, #0
	movs r0, #222
	adds r1, r5, #0
	mov r3, r8
	bl 0x020093d8
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02000a68_0
	ldr r1, [sp, #48]
	movs r5, #15
	adds r1, #1
	ands r1, r5
	ldr r7, [r6, #80]
	bl 0x020093c8
	ldr r3, [sp, #48]
	ands r3, r5
	lsls r3, r3, #2
	mov r0, r10
	ldr r1, [r0, r3]
	adds r0, r6, #0
	bl 0x020093d0
	ldr r2, [sp, #48]
	lsrs r1, r2, #16
	adds r0, r6, #0
	ands r1, r5
	bl 0x020094c0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	adds r2, r7, #0
	adds r2, #38
	strb r3, [r2]
	ldr r3, [pc, #108]
	str r3, [r6, #108]
	mov r3, r9
	str r3, [r6, #48]
	ldr r3, [sp, #44]
	str r3, [r6, #52]
	add r4, sp, #52
	ldrh r4, [r4]
	adds r3, r6, #0
	adds r3, #102
	strh r4, [r3]
	ldr r0, [sp, #52]
	lsrs r4, r0, #16
	cmp r4, #0
	beq .L_02000a68_1
	cmp r4, #3
	bhi .L_02000a68_0
	b .L_02000a68_2
.L_02000a68_1:
	mov r1, r11
	ldr r3, [r1, #80]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	ldrb r1, [r7, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	b .L_02000a68_3
.L_02000a68_2:
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r3, #3
	ands r4, r3
	ldrb r2, [r7, #9]
	movs r3, #13
	negs r3, r3
	lsls r1, r4, #2
	ands r3, r2
	orrs r3, r1
.L_02000a68_3:
	strb r3, [r7, #9]
.L_02000a68_0:
	sub sp, #-12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200960c
	.4byte 0x02008a01
	.global Func_02000b50
	.thumb_func
Func_02000b50:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009690
	.global Func_02000b58
	.thumb_func
Func_02000b58:
	movs r0, #0
	bx lr
	.global Func_02000b5c
	.thumb_func
Func_02000b5c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009750
	.global Func_02000b64
	.thumb_func
Func_02000b64:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200976c
	.global Func_02000b6c
	.thumb_func
Func_02000b6c:
	push {r5, lr}
	movs r0, #8
	sub sp, #8
	bl 0x02009450
	ldr r5, [r0, #8]
	cmp r5, #0
	bge .L_02000b6c_0
	ldr r3, [pc, #72]
	adds r5, r5, r3
.L_02000b6c_0:
	asrs r5, r5, #20
	bl 0x02009440
	cmp r5, #20
	bne .L_02000b6c_1
	movs r3, #18
	movs r2, #6
	str r3, [sp, #0]
	movs r0, #18
	movs r1, #40
	movs r3, #3
	str r2, [sp, #4]
	bl 0x020093f0
	ldr r0, [pc, #44]
	bl 0x02009430
	b .L_02000b6c_2
.L_02000b6c_1:
	movs r3, #18
	movs r2, #6
	str r3, [sp, #0]
	movs r0, #24
	movs r1, #40
	movs r3, #3
	str r2, [sp, #4]
	bl 0x020093f0
	ldr r0, [pc, #20]
	bl 0x02009428
.L_02000b6c_2:
	bl 0x02009448
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x000fffff
	.4byte 0x00000302
	.global Func_02000bd0
	.thumb_func
Func_02000bd0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, [pc, #256]
	ldr r7, [r3]
	mov r9, r3
	movs r3, #7
	ands r7, r3
	sub sp, #12
	cmp r7, #0
	bne 0x02008cca
	movs r0, #9
	bl 0x02009450
.L_02000bf0:
	adds r5, r0, #0
	bl 0x020093c0
	lsls r3, r0, #1
	adds r3, r3, r0
	ldr r5, [r5, #8]
	lsls r3, r3, #2
	lsrs r3, r3, #16
	lsls r3, r3, #16
.L_02000c02:
	mov r8, r5
	movs r0, #9
	add r8, r3
	bl 0x02009450
	mov r10, r0
	movs r0, #9
	bl 0x02009450
	movs r3, #192
	ldr r6, [r0, #16]
	lsls r3, r3, #11
	adds r6, r6, r3
	bl 0x020093c0
	lsls r2, r0, #2
	adds r2, r2, r0
	lsrs r2, r2, #16
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r2
	lsls r5, r3, #6
	subs r5, r5, r3
	lsls r5, r5, #3
	adds r5, r5, r2
	bl 0x020093c0
	lsls r0, r0, #1
	lsrs r0, r0, #16
	mov r3, r10
	ldr r1, [r3, #12]
	negs r5, r5
	str r0, [sp, #4]
	movs r3, #0
	mov r0, r8
	adds r2, r6, #0
	str r7, [sp, #8]
	str r5, [sp, #0]
	bl 0x02008a68
	mov r3, r9
	ldr r7, [r3]
	movs r3, #15
	ands r7, r3
	cmp r7, #0
	bne .L_02000c02_0
	movs r0, #9
	bl 0x02009450
	adds r5, r0, #0
	bl 0x020093c0
	lsls r3, r0, #1
	adds r3, r3, r0
	ldr r5, [r5, #8]
	lsls r3, r3, #2
	lsrs r3, r3, #16
	lsls r3, r3, #16
	mov r8, r5
	movs r0, #9
	add r8, r3
	bl 0x02009450
	mov r10, r0
	movs r0, #9
	bl 0x02009450
	movs r3, #192
	ldr r6, [r0, #16]
	lsls r3, r3, #11
	adds r6, r6, r3
	bl 0x020093c0
	lsls r2, r0, #2
	adds r2, r2, r0
	lsrs r2, r2, #16
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r2
	lsls r5, r3, #6
	subs r5, r5, r3
	lsls r5, r5, #3
	adds r5, r5, r2
	bl 0x020093c0
	lsls r0, r0, #1
	lsrs r0, r0, #16
	mov r3, r10
	ldr r1, [r3, #12]
	negs r5, r5
	str r0, [sp, #4]
	adds r2, r6, #0
	mov r0, r8
	movs r3, #0
	str r5, [sp, #0]
	str r7, [sp, #8]
	bl 0x02008a68
.L_02000c02_0:
	sub sp, #-12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x1e40
	.2byte 0x0300
	.global Func_02000ce0
	.thumb_func
Func_02000ce0:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	movs r0, #9
	sub sp, #12
	bl 0x02009450
	ldr r5, [r0, #8]
	cmp r5, #0
	bge .L_02000ce0_0
	ldr r3, [pc, #448]
	adds r5, r5, r3
.L_02000ce0_0:
	asrs r5, r5, #20
	bl 0x02009440
	cmp r5, #25
	beq .L_02000ce0_1
	b .L_02000ce0_2
.L_02000ce0_1:
	movs r0, #11
	bl 0x02009450
	movs r3, #0
	mov r9, r3
	adds r0, #34
	movs r3, #1
	strb r3, [r0]
	movs r0, #11
	bl 0x02009450
	movs r1, #0
	bl 0x02009400
	movs r1, #14
	movs r0, #11
	bl 0x020094b8
	movs r0, #11
	bl 0x02009450
	movs r1, #1
	bl 0x02009408
	movs r1, #207
	movs r2, #240
	lsls r2, r2, #16
	lsls r1, r1, #17
	movs r0, #11
	bl 0x02009488
	movs r0, #10
	bl 0x02009438
	ldr r5, [pc, #368]
	movs r1, #200
	lsls r1, r1, #4
	adds r0, r5, #0
	bl 0x020093b0
	movs r0, #141
	bl 0x02009510
	movs r1, #1
	movs r2, #0
	movs r0, #9
	bl 0x02009478
	movs r0, #9
	bl 0x02009480
	movs r0, #10
	bl 0x02009438
	movs r1, #2
	movs r2, #0
	movs r0, #9
	bl 0x02009478
	movs r0, #9
	bl 0x02009480
	movs r0, #9
	bl 0x02009450
	mov r3, r9
	str r3, [r0, #68]
	movs r0, #9
	bl 0x02009450
	ldr r3, [pc, #300]
	str r3, [r0, #72]
	movs r0, #3
	bl 0x02009438
	movs r1, #160
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #7
	movs r0, #9
	bl 0x02009458
	movs r0, #144
	lsls r0, r0, #1
	bl 0x02009510
	movs r1, #208
	movs r2, #200
	lsls r1, r1, #1
	movs r0, #9
	bl 0x02009460
	movs r0, #9
	bl 0x02009450
	movs r1, #0
	bl 0x02009400
	adds r0, r5, #0
	bl 0x020093b8
	movs r0, #12
	bl 0x02009438
	movs r0, #189
	bl 0x02009510
	movs r0, #9
	bl 0x02009450
	adds r5, r0, #0
	bl 0x020093c0
	lsls r3, r0, #1
	adds r3, r3, r0
	ldr r5, [r5, #8]
	lsls r3, r3, #2
	lsrs r3, r3, #16
	lsls r3, r3, #16
	mov r8, r5
	movs r0, #9
	add r8, r3
	bl 0x02009450
	mov r10, r0
	movs r0, #9
	bl 0x02009450
	movs r3, #192
	ldr r6, [r0, #16]
	lsls r3, r3, #11
	adds r6, r6, r3
	bl 0x020093c0
	lsls r2, r0, #2
	adds r2, r2, r0
	lsrs r2, r2, #16
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r2
	lsls r5, r3, #6
	subs r5, r5, r3
	lsls r5, r5, #3
	adds r5, r5, r2
	bl 0x020093c0
	lsls r0, r0, #1
	lsrs r0, r0, #16
	mov r3, r10
	ldr r1, [r3, #12]
	adds r2, r6, #0
	mov r3, r9
	str r0, [sp, #4]
	negs r5, r5
	mov r0, r8
	str r3, [sp, #8]
	str r5, [sp, #0]
	bl 0x02008a68
	movs r0, #20
	bl 0x02009438
	movs r0, #154
	bl 0x02009510
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl 0x02009410
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #88]
	bl 0x02009410
	bl 0x02009418
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x02009488
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl 0x02009488
	movs r0, #192
	lsls r0, r0, #2
	bl 0x02009428
	movs r3, #21
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #21
	movs r1, #45
	movs r2, #4
	movs r3, #2
	bl 0x020093f0
.L_02000ce0_2:
	bl 0x02009448
	sub sp, #-12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x000fffff
	.4byte 0x02008bd1
	.4byte 0x00009999
	.4byte 0x0000e666
	.global Func_02000ecc
	.thumb_func
Func_02000ecc:
	push {r5, r6, lr}
	mov r6, r11
	mov r5, r10
	push {r5, r6}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	movs r0, #10
	sub sp, #12
	bl 0x02009450
	ldr r3, [r0, #8]
	cmp r3, #0
	bge .L_02000ecc_0
	ldr r0, [pc, #388]
	adds r3, r3, r0
.L_02000ecc_0:
	movs r0, #10
	asrs r5, r3, #20
	bl 0x02009450
	ldr r3, [r0, #16]
	cmp r3, #0
	bge .L_02000ecc_1
	ldr r2, [pc, #372]
	adds r3, r3, r2
.L_02000ecc_1:
	asrs r3, r3, #20
	cmp r5, #38
	beq .L_02000ecc_2
	b .L_02000ecc_3
.L_02000ecc_2:
	cmp r3, #14
	beq .L_02000ecc_4
	b .L_02000ecc_3
.L_02000ecc_4:
	movs r0, #10
	bl 0x02009450
	ldr r3, [pc, #352]
	str r3, [r0, #12]
	movs r0, #10
	bl 0x02009450
	adds r5, r0, #0
	movs r0, #10
	bl 0x02009450
	ldr r3, [r0, #12]
	movs r0, #188
	str r3, [r5, #60]
	bl 0x02009510
	movs r0, #10
	bl 0x02009450
	adds r6, r0, #0
	movs r0, #10
	bl 0x02009450
	adds r5, r0, #0
	movs r0, #10
	bl 0x02009450
	ldr r4, [r6, #8]
	ldr r2, [r0, #16]
	movs r0, #0
	ldr r1, [r5, #12]
	mov r8, r0
	str r0, [sp, #0]
	str r0, [sp, #4]
	movs r3, #128
	movs r0, #1
	str r0, [sp, #8]
	mov r11, r0
	lsls r3, r3, #8
	adds r0, r4, #0
	bl 0x02008a68
	movs r0, #10
	bl 0x02009450
	adds r6, r0, #0
	movs r0, #10
	bl 0x02009450
	adds r5, r0, #0
	movs r0, #10
	bl 0x02009450
	ldr r2, [r0, #16]
	ldr r0, [pc, #252]
	ldr r1, [r5, #12]
	ldr r3, [r6, #8]
	mov r9, r0
	str r0, [sp, #0]
	mov r0, r8
	str r0, [sp, #4]
	mov r0, r11
	str r0, [sp, #8]
	adds r0, r3, #0
	mov r3, r9
	bl 0x02008a68
	movs r0, #10
	bl 0x02009450
	adds r6, r0, #0
	movs r0, #10
	bl 0x02009450
	adds r5, r0, #0
	movs r0, #10
	bl 0x02009450
	ldr r2, [r0, #16]
	ldr r0, [pc, #204]
	mov r10, r0
	mov r0, r9
	ldr r1, [r5, #12]
	ldr r3, [r6, #8]
	str r0, [sp, #0]
	mov r0, r8
	str r0, [sp, #4]
	mov r0, r11
	str r0, [sp, #8]
	adds r0, r3, #0
	mov r3, r10
	bl 0x02008a68
	movs r0, #10
	bl 0x02009450
	adds r6, r0, #0
	movs r0, #10
	bl 0x02009450
	adds r5, r0, #0
	movs r0, #10
	bl 0x02009450
	ldr r4, [r6, #8]
	ldr r2, [r0, #16]
	mov r0, r8
	ldr r1, [r5, #12]
	str r0, [sp, #0]
	str r0, [sp, #4]
	mov r0, r11
	str r0, [sp, #8]
	ldr r3, [pc, #144]
	adds r0, r4, #0
	bl 0x02008a68
	movs r0, #10
	bl 0x02009450
	adds r6, r0, #0
	movs r0, #10
	bl 0x02009450
	adds r5, r0, #0
	movs r0, #10
	bl 0x02009450
	ldr r2, [r0, #16]
	mov r0, r10
	ldr r1, [r5, #12]
	ldr r3, [r6, #8]
	str r0, [sp, #0]
	mov r0, r8
	str r0, [sp, #4]
	mov r0, r11
	str r0, [sp, #8]
	adds r0, r3, #0
	mov r3, r9
	bl 0x02008a68
	movs r0, #10
	bl 0x02009450
	adds r6, r0, #0
	movs r0, #10
	bl 0x02009450
	adds r5, r0, #0
	movs r0, #10
	bl 0x02009450
	ldr r2, [r0, #16]
	mov r0, r10
	ldr r3, [r6, #8]
	ldr r1, [r5, #12]
	str r0, [sp, #0]
	mov r0, r8
	str r0, [sp, #4]
	mov r0, r11
	str r0, [sp, #8]
	adds r0, r3, #0
	mov r3, r10
	bl 0x02008a68
	ldr r0, [pc, #44]
	bl 0x02009428
.L_02000ecc_3:
	sub sp, #-12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r3}
	mov r11, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x000fffff
	.4byte 0xfffe0000
	.4byte 0x00006666
	.4byte 0xffff999a
	.4byte 0xffff8000
	.4byte 0x00000301
	.global Func_02001088
	.thumb_func
Func_02001088:
	push {lr}
	bl 0x02009440
	bl 0x02008ecc
	bl 0x02009448
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200109c
	.thumb_func
Func_0200109c:
	push {lr}
	bl 0x02009440
	bl 0x020080c4
	bl 0x02008ecc
	bl 0x02009448
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020010b4
	.thumb_func
Func_020010b4:
	push {lr}
	bl 0x020094f0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020010c0
	.thumb_func
Func_020010c0:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009814
	.global Func_020010c8
	.thumb_func
Func_020010c8:
	push {r5, lr}
	ldr r3, [pc, #132]
	ldr r2, [pc, #132]
	ldr r5, [r3]
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	cmp r3, #240
	beq .L_020010c8_0
	cmp r3, #240
	bgt .L_020010c8_1
	cmp r3, #60
	beq .L_020010c8_2
	cmp r3, #180
	beq .L_020010c8_3
	b .L_020010c8_4
.L_020010c8_1:
	movs r2, #135
	lsls r2, r2, #1
	cmp r3, r2
	beq .L_020010c8_0
	adds r2, #210
	cmp r3, r2
	beq .L_020010c8_5
	b .L_020010c8_4
.L_020010c8_2:
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #0
	bl 0x020094d8
	movs r0, #13
	movs r1, #2
	movs r2, #0
	bl 0x020094e0
	b .L_020010c8_4
.L_020010c8_3:
	movs r0, #13
	movs r1, #3
	bl 0x020094a8
	b .L_020010c8_4
.L_020010c8_0:
	movs r0, #13
	movs r1, #4
	movs r2, #0
	bl 0x020094a0
	b .L_020010c8_4
.L_020010c8_5:
	movs r0, #13
	movs r1, #4
	bl 0x02009490
.L_020010c8_4:
	ldr r3, [pc, #40]
	movs r2, #141
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020010c8_6
	movs r3, #193
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #99
	strh r3, [r2]
.L_020010c8_6:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x020098ec
	.4byte 0x02000240
	.global Func_0200115c
	.thumb_func
Func_0200115c:
	push {lr}
	ldr r0, [pc, #176]
	bl 0x020093b8
	bl 0x02009440
.L_02001168:
	movs r1, #128
	movs r2, #30
	movs r0, #13
	lsls r1, r1, #1
	bl 0x020094e0
	movs r0, #13
	movs r1, #2
	bl 0x020094b0
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #0
	bl 0x020094d8
	ldr r0, [pc, #136]
	bl 0x020094c8
	movs r0, #13
	movs r1, #0
	bl 0x020094d0
	movs r1, #3
	movs r0, #13
	bl 0x02009498
	movs r0, #30
	bl 0x02009438
	movs r0, #10
	bl 0x02009450
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #253
	ands r3, r2
	movs r1, #128
	movs r2, #128
	strb r3, [r0]
	lsls r1, r1, #10
	movs r0, #13
	lsls r2, r2, #9
	bl 0x02009458
	movs r1, #150
	movs r0, #13
	lsls r1, r1, #2
	movs r2, #216
	bl 0x02009470
	movs r1, #150
	movs r0, #13
	lsls r1, r1, #2
	movs r2, #248
	bl 0x02009470
	movs r1, #142
	movs r2, #148
	movs r0, #13
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x02009470
	movs r1, #0
	movs r2, #0
	movs r0, #13
	bl 0x02009488
	movs r0, #10
	bl 0x02009450
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	ldr r0, [pc, #20]
	bl 0x02009428
	bl 0x02009448
	pop {r0}
	bx r0
	.2byte 0x90c9
	.2byte 0x0200
	.4byte 0x0000132f
	.4byte 0x00000869
	.global Func_0200121c
	.thumb_func
Func_0200121c:
	push {r5, r6, lr}
	ldr r3, [pc, #368]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #68
	str r3, [r2]
	subs r3, #60
	adds r2, r1, r3
	movs r3, #24
	str r3, [r2]
	movs r0, #9
	sub sp, #8
	bl 0x02009450
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #16
	orrs r3, r2
	strb r3, [r0]
	ldr r0, [pc, #332]
	bl 0x02009420
	cmp r0, #0
	beq .L_0200121c_0
	movs r1, #172
	movs r2, #208
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl 0x02009488
	movs r3, #18
	movs r2, #6
	str r3, [sp, #0]
	movs r0, #24
	movs r1, #40
	movs r3, #3
	str r2, [sp, #4]
	bl 0x020093f0
	b .L_0200121c_1
.L_0200121c_0:
	movs r3, #18
	movs r2, #6
	str r3, [sp, #0]
	movs r0, #18
	movs r1, #40
	movs r3, #3
	str r2, [sp, #4]
	bl 0x020093f0
.L_0200121c_1:
	movs r0, #192
	lsls r0, r0, #2
	bl 0x02009420
	cmp r0, #0
	beq .L_0200121c_2
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x02009488
	movs r3, #21
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #21
	movs r1, #45
	movs r2, #4
	movs r3, #2
	bl 0x020093f0
.L_0200121c_2:
	ldr r0, [pc, #232]
	bl 0x02009420
	cmp r0, #0
	beq .L_0200121c_3
	movs r1, #154
	movs r2, #232
	lsls r2, r2, #16
	movs r0, #10
	lsls r1, r1, #18
	bl 0x02009488
	ldr r6, [pc, #212]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r6, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #2
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_0200121c_4
	movs r0, #10
	bl 0x02009450
	movs r3, #2
	adds r0, #34
	strb r3, [r0]
	movs r0, #10
	bl 0x02009450
	ldr r3, [r0, #12]
	subs r3, #1
	str r3, [r0, #12]
	movs r0, #10
	bl 0x02009450
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #2
	orrs r5, r3
	movs r2, #14
	movs r3, #36
	strb r5, [r0]
	movs r1, #48
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #36
	movs r2, #5
	movs r3, #1
	bl 0x020093f0
	b .L_0200121c_4
.L_0200121c_3:
	ldr r6, [pc, #128]
.L_0200121c_4:
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #99
	bne .L_0200121c_5
	bl 0x020094f8
	bl 0x02009500
	movs r1, #192
	movs r2, #192
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #9
	bl 0x02009488
	movs r0, #60
	bl 0x02009438
	movs r0, #9
	bl 0x02009450
	movs r3, #2
	adds r0, #34
	movs r1, #204
	strb r3, [r0]
	lsls r1, r1, #1
	movs r0, #9
	movs r2, #192
	bl 0x02009468
	movs r0, #60
	bl 0x02009438
	bl 0x02008ce0
.L_0200121c_5:
	movs r2, #141
	lsls r2, r2, #2
	adds r3, r6, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200121c_6
	ldr r3, [pc, #40]
	movs r2, #0
	movs r1, #200
	str r2, [r3]
	ldr r0, [pc, #36]
	lsls r1, r1, #4
	bl 0x020093b0
.L_0200121c_6:
	movs r0, #0
	sub sp, #-8
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x00000302
	.4byte 0x00000301
	.4byte 0x02000240
	.4byte 0x020098ec
	.4byte 0x020090c9
	.include "games/THE BROKEN SEAL/SRC/FIELD/GOMA_HASHIRA/IMPORT.INC"
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
	.4byte 0x02009618
	.4byte 0x02009640
	.4byte 0x02009668
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02008a59
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02008a59
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02008a59
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000058
	.4byte 0x400000b8
	.4byte 0x00080000
	.4byte 0x02d80008
	.4byte 0x000001f8
	.4byte 0xffff0002
	.4byte 0x00000248
	.4byte 0x400000b8
	.4byte 0x00080000
	.4byte 0x02d80008
	.4byte 0x000001f8
	.4byte 0xffff0003
	.4byte 0x00000208
	.4byte 0x400000e8
	.4byte 0x00080000
	.4byte 0x02d80008
	.4byte 0x000001f8
	.4byte 0xffff0004
	.4byte 0x000000e8
	.4byte 0x400000b8
	.4byte 0x00080000
	.4byte 0x02d80008
	.4byte 0x000001f8
	.4byte 0xffff0005
	.4byte 0x00000218
	.4byte 0x40000198
	.4byte 0x00080000
	.4byte 0x02d80008
	.4byte 0x000001f8
	.4byte 0xffff0063
	.4byte 0x00000178
	.4byte 0x400000c8
	.4byte 0x00080000
	.4byte 0x02d80008
	.4byte 0x000001f8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001b
	.4byte 0x0010201c
	.4byte 0x0020101a
	.4byte 0x0030201a
	.4byte 0x0040301a
	.4byte 0x0050301c
	.4byte 0x000001ff
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00024000
	.4byte 0xffff00d4
	.4byte 0x00000001
	.4byte 0x01800000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00df
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x0058005c
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00004000
	.4byte 0x08690086
	.4byte 0x00000001
	.4byte 0x02740000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00012000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0008
	.4byte 0x00000004
	.4byte 0x00000021
	.4byte 0xffff0009
	.4byte 0x00000005
	.4byte 0x00000202
	.4byte 0x0301000a
	.4byte 0x0200909d
	.4byte 0x00008602
	.4byte 0xffff0014
	.4byte 0x020090b5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020089dd
	.4byte 0x00000000
	.4byte 0x00a8000d
	.4byte 0x0000132b
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000132c
	.4byte 0x00008d15
	.4byte 0x00a8000d
	.4byte 0x0000132d
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000132e
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x02008b6d
	.4byte 0x00008c15
	.4byte 0x03000009
	.4byte 0x02008ce1
	.4byte 0x00008c15
	.4byte 0x0301000a
	.4byte 0x02009089
	.4byte 0x00000013
	.4byte 0x0f4f0065
	.4byte 0x001000e5
	.4byte 0x00000006
	.4byte 0xffff0063
	.4byte 0x0200915d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
