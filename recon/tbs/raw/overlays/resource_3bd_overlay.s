.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/ENTRY.INC"
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
	bl 0x0200be84
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
	bl 0x0200bcc8
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
	bl 0x0200bc58
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
	bl 0x0200bc28
	ldr r5, [pc, #132]
	movs r0, #15
	bl 0x0200bc00
	movs r0, #185
	bl 0x0200be70
	str r5, [r6, #48]
	str r5, [r6, #52]
	ldr r1, [r7]
.L_020000c4_5:
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl 0x0200bc38
	mov r0, r8
	str r5, [r0, #48]
	str r5, [r0, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl 0x0200bc38
	adds r0, r6, #0
	bl 0x0200bc40
	bl 0x0200be58
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
	bl 0x0200bc28
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
	.4byte 0x0200beb4
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
	bl 0x0200bc58
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
	.4byte 0x0200beb4
	.4byte 0xffff0000
	.4byte 0x0200bef4
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
	bl 0x0200bcc8
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
	.4byte 0x0200bef4
	.4byte 0x0200bf0c
	.4byte 0x0200beb4
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
	bl 0x0200bc58
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
	.4byte 0x0200bf0c
	.4byte 0x0200beb4
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
	bl 0x0200bcc8
	ldrh r3, [r0, #6]
	ldr r0, [sp, #76]
	lsrs r3, r3, #12
	mov r8, r3
	bl 0x0200bcc8
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
	bl 0x0200bcd0
	movs r1, #8
	movs r0, #0
	bl 0x0200bd20
	movs r0, #15
	bl 0x0200bca8
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
	bl 0x0200bd08
	movs r0, #0
	bl 0x0200bcc8
	ldr r3, [pc, #424]
	str r3, [r0, #108]
	movs r0, #4
	bl 0x0200bca8
	mov r3, r8
	subs r3, #6
	cmp r3, #7
	bhi .L_02000608_6
	adds r0, r7, #0
	movs r1, #3
	bl 0x0200bc28
	b .L_02000608_7
.L_02000608_6:
	adds r0, r7, #0
	movs r1, #2
	bl 0x0200bc28
.L_02000608_7:
	movs r0, #239
	bl 0x0200be70
	adds r0, r7, #0
	ldr r1, [sp, #80]
	ldr r2, [sp, #84]
	ldr r3, [sp, #88]
	bl 0x0200bc38
	movs r0, #0
	bl 0x0200bd10
	movs r0, #0
	movs r1, #2
	bl 0x0200bd20
	movs r0, #0
	ldr r1, [pc, #360]
	ldr r2, [pc, #344]
	bl 0x0200bcd0
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
	bl 0x0200bd08
	ldr r3, [sp, #92]
	cmp r3, #0
	beq .L_02000608_8
	bl 0x0200be84
.L_02000608_8:
	movs r0, #0
	bl 0x0200bd10
	movs r1, #1
	movs r0, #0
	bl 0x0200bd20
	movs r0, #0
	bl 0x0200bcc8
	movs r2, #0
	str r2, [r0, #108]
	adds r0, r7, #0
	bl 0x0200bc40
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200be70
	movs r0, #213
	bl 0x0200be70
	ldr r3, [sp, #80]
	str r3, [r7, #8]
	ldr r3, [sp, #88]
	str r3, [r7, #16]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	adds r0, r7, #0
	movs r1, #1
	bl 0x0200bc28
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
	bl 0x0200bc48
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
	bl 0x0200bc48
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
	bl 0x0200be58
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
	.2byte 0xbf0c
	.2byte 0x0200
	.2byte 0x1999
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0x0001
	.2byte 0x82a9
	.2byte 0x0200
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0xbeb4
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
	bl 0x0200bcc8
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
	bl 0x0200bc48
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
	.2byte 0xbef4
	.2byte 0x0200
	.4byte 0x0200bf0c
	.global Func_020009dc
	.thumb_func
Func_020009dc:
	push {lr}
	movs r0, #13
	movs r1, #65
	bl 0x0200bdb0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020009ec
	.thumb_func
Func_020009ec:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200bf70
	.global Func_020009f4
	.thumb_func
Func_020009f4:
	movs r0, #0
	bx lr
	.global Func_020009f8
	.thumb_func
Func_020009f8:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200c138
	.global Func_02000a00
	.thumb_func
Func_02000a00:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000a00_0
	ldr r0, [pc, #36]
	b .L_02000a00_1
.L_02000a00_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000a00_2
	ldr r0, [pc, #36]
	b .L_02000a00_1
.L_02000a00_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000a00_3
	ldr r0, [pc, #32]
	b .L_02000a00_1
.L_02000a00_3:
	ldr r0, [pc, #32]
.L_02000a00_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000093
	.4byte 0x0200c1b0
	.4byte 0x00000095
	.4byte 0x0200c270
	.4byte 0x00000097
	.4byte 0x0200c318
	.4byte 0x0200c198
	.global Func_02000a54
	.thumb_func
Func_02000a54:
	push {lr}
	ldr r3, [pc, #84]
	ldrb r2, [r3]
	ldr r1, [pc, #84]
	ldr r3, [pc, #84]
	strh r1, [r3]
	lsls r2, r2, #24
	asrs r2, r2, #24
	cmp r2, #0
	bne .L_02000a54_0
	movs r2, #128
	lsls r2, r2, #5
	adds r3, #2
	b .L_02000a54_1
.L_02000a54_0:
	cmp r2, #1
	bne .L_02000a54_2
	movs r2, #224
	ldr r3, [pc, #64]
	lsls r2, r2, #4
	b .L_02000a54_1
.L_02000a54_2:
	cmp r2, #2
	bne .L_02000a54_3
	movs r2, #192
	ldr r3, [pc, #52]
	lsls r2, r2, #4
	b .L_02000a54_1
.L_02000a54_3:
	cmp r2, #3
	bne .L_02000a54_4
	movs r2, #160
	ldr r3, [pc, #40]
	lsls r2, r2, #4
	b .L_02000a54_1
.L_02000a54_4:
	cmp r2, #4
	bne .L_02000a54_5
	movs r2, #128
	ldr r3, [pc, #28]
	lsls r2, r2, #4
	b .L_02000a54_1
.L_02000a54_5:
	movs r2, #192
	ldr r3, [pc, #20]
	lsls r2, r2, #3
.L_02000a54_1:
	strh r2, [r3]
	pop {r0}
	bx r0
	.4byte 0x02001004
	.4byte 0x00003f42
	.4byte 0x04000050
	.4byte 0x04000052
	.global Func_02000abc
	.thumb_func
Func_02000abc:
	push {lr}
	ldr r2, [pc, #28]
	ldr r3, [pc, #28]
	ldr r3, [r3]
	strb r0, [r2]
	ldr r2, [pc, #28]
	adds r3, r3, r2
	movs r2, #0
.L_02000acc:
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02000acc_0
	bl 0x02008a54
.L_02000acc_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x1004
	.2byte 0x0200
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x0cb8
	.2byte 0x0000
	.global Func_02000ae8
	.thumb_func
Func_02000ae8:
	push {lr}
	movs r0, #0
	bl 0x02008abc
	pop {r0}
	bx r0
	.global Func_02000af4
	.thumb_func
Func_02000af4:
	push {lr}
	movs r0, #1
	bl 0x02008abc
	pop {r0}
	bx r0
	.global Func_02000b00
	.thumb_func
Func_02000b00:
	push {lr}
	movs r0, #2
	bl 0x02008abc
	pop {r0}
	bx r0
	.global Func_02000b0c
	.thumb_func
Func_02000b0c:
	push {lr}
	movs r0, #3
	bl 0x02008abc
	pop {r0}
	bx r0
	.global Func_02000b18
	.thumb_func
Func_02000b18:
	push {lr}
	movs r0, #4
	bl 0x02008abc
	pop {r0}
	bx r0
	.global Func_02000b24
	.thumb_func
Func_02000b24:
	push {lr}
	movs r0, #5
	bl 0x02008abc
	pop {r0}
	bx r0
	.global Func_02000b30
	.thumb_func
Func_02000b30:
	push {r5, lr}
	ldr r5, [pc, #108]
	movs r3, #224
	ldr r1, [r5]
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	adds r3, #200
	adds r2, r1, r3
	movs r3, #24
	str r3, [r2]
	movs r0, #1
	bl 0x0200bc00
	movs r0, #77
	bl 0x0200bdb8
	ldr r3, [pc, #76]
	ldr r5, [r5, #16]
	adds r2, r5, r3
	movs r3, #5
	strh r3, [r2]
	ldr r0, [pc, #72]
	bl 0x0200bc88
	cmp r0, #0
	beq .L_02000b30_0
	ldr r3, [pc, #64]
	adds r2, r5, r3
	ldr r3, [pc, #64]
	strh r3, [r2]
	ldr r3, [pc, #64]
	adds r2, r5, r3
	movs r3, #63
	strh r3, [r2]
	bl 0x02008a54
	b .L_02000b30_1
.L_02000b30_0:
	ldr r3, [pc, #44]
	adds r2, r5, r3
	ldr r3, [pc, #52]
	strh r3, [r2]
	ldr r3, [pc, #44]
	adds r2, r5, r3
	movs r3, #31
	strh r3, [r2]
	ldr r2, [pc, #44]
	ldr r3, [pc, #44]
	strh r2, [r3]
	ldr r2, [pc, #44]
	adds r3, #2
	strh r2, [r3]
.L_02000b30_1:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x0000052a
	.4byte 0x00000201
	.4byte 0x00000534
	.4byte 0x00001d1d
	.4byte 0x00000536
	.4byte 0x00003f3f
	.4byte 0x00003f42
	.4byte 0x04000050
	.4byte 0x00000c04
	.global Func_02000bc8
	.thumb_func
Func_02000bc8:
	push {lr}
	ldr r3, [pc, #80]
	movs r0, #224
	lsls r0, r0, #1
	adds r3, r3, r0
	ldrh r1, [r3]
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_02000bc8_0
	movs r2, #128
	ldr r3, [pc, #64]
	lsls r2, r2, #5
	strh r2, [r3]
.L_02000bc8_0:
	lsls r3, r1, #16
	ldr r2, [pc, #60]
	asrs r3, r3, #16
	cmp r3, r2
	bne .L_02000bc8_1
	movs r0, #16
	movs r1, #1
	bl 0x0200bd58
	movs r0, #17
	movs r1, #4
	bl 0x0200bd58
	movs r0, #18
	movs r1, #11
	bl 0x0200bd58
	movs r0, #19
	movs r1, #2
	bl 0x0200bd58
	movs r0, #20
	movs r1, #3
	bl 0x0200bd58
.L_02000bc8_1:
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x00000092
	.4byte 0x04000052
	.4byte 0x00000097
	.global Func_02000c2c
	.thumb_func
Func_02000c2c:
	push {r5, lr}
	ldr r3, [pc, #92]
	movs r0, #128
	lsls r0, r0, #2
	ldr r5, [r3]
	bl 0x0200bc88
	cmp r0, #0
	beq .L_02000c2c_0
	bl 0x02008b30
	adds r2, r5, #0
	adds r2, #52
	movs r3, #1
	strb r3, [r2]
.L_02000c2c_0:
	ldr r3, [pc, #68]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_02000c2c_1
	movs r0, #16
	movs r1, #6
	bl 0x0200bd58
	movs r0, #17
	movs r1, #6
	bl 0x0200bd58
	movs r0, #18
	movs r1, #6
	bl 0x0200bd58
	movs r0, #19
	movs r1, #6
	bl 0x0200bd58
	movs r0, #20
	movs r1, #6
	bl 0x0200bd58
.L_02000c2c_1:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001f30
	.4byte 0x02000240
	.4byte 0x00000097
	.global Func_02000c98
	.thumb_func
Func_02000c98:
	push {r5, r6, lr}
	ldr r3, [pc, #80]
	ldr r6, [r3]
	movs r1, #128
	ldr r5, [r6, #16]
	movs r2, #0
	movs r3, #24
	ldrsh r0, [r6, r3]
	lsls r1, r1, #7
	bl 0x0200bd78
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200bc78
	movs r0, #20
	bl 0x0200bca8
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	movs r0, #0
	bl 0x0200bc30
	adds r4, r0, #0
	cmp r4, #0
	beq .L_02000c98_0
	ldr r3, [pc, #32]
	adds r0, r5, #0
	adds r1, r4, #0
	ldr r2, [pc, #28]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r3, r5, #0
	movs r2, #0
	adds r3, #84
	str r2, [r5, #108]
	str r4, [r6, #16]
	strb r2, [r3]
.L_02000c98_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001f30
	.4byte 0x040000d4
	.4byte 0x8400001c
	.global Func_02000cf8
	.thumb_func
Func_02000cf8:
	push {lr}
	movs r0, #12
	sub sp, #8
	bl 0x0200bcc8
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #30
	bne .L_02000cf8_0
	ldr r3, [r0, #16]
	asrs r4, r3, #20
	cmp r4, #20
	bne .L_02000cf8_0
	adds r1, r0, #0
	movs r2, #2
	adds r1, #85
	movs r3, #0
	strb r2, [r1]
	str r3, [r0, #20]
	adds r3, r0, #0
	adds r3, #35
	strb r2, [r3]
	movs r3, #32
	str r3, [sp, #0]
	movs r0, #30
	movs r1, #20
	movs r2, #1
	movs r3, #1
	str r4, [sp, #4]
	bl 0x0200bc48
	ldr r0, [pc, #12]
	bl 0x0200bc90
.L_02000cf8_0:
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000212
	.global Func_02000d48
	.thumb_func
Func_02000d48:
	push {lr}
	bl 0x020080c4
	bl 0x02008cf8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000d58
	.thumb_func
Func_02000d58:
	push {r5, lr}
	ldr r3, [pc, #48]
	movs r0, #11
	sub sp, #12
	ldr r5, [r3]
	bl 0x0200bcc8
	ldr r3, [r0, #8]
	mov r1, sp
	str r3, [r1]
	ldr r3, [r0, #12]
	str r3, [r1, #4]
	ldr r3, [r0, #16]
	str r3, [r1, #8]
	bl 0x0200bc58
	cmp r0, #0
	ble .L_02000d58_0
	adds r2, r5, #0
	adds r2, #53
	movs r3, #1
	strb r3, [r2]
.L_02000d58_0:
	sub sp, #-12
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001f30
	.global Func_02000d90
	.thumb_func
Func_02000d90:
	push {r5, r6, lr}
	ldr r3, [pc, #76]
	movs r0, #11
	ldr r5, [r3]
	sub sp, #8
	bl 0x0200bcc8
	adds r5, #53
	ldrb r5, [r5]
	lsls r5, r5, #24
	asrs r5, r5, #24
	adds r6, r0, #0
	cmp r5, #0
	bne .L_02000d90_0
	movs r3, #73
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #76
	movs r1, #16
	movs r2, #1
	movs r3, #1
	bl 0x0200bc48
	cmp r6, #0
	beq .L_02000d90_1
	adds r2, r6, #0
	movs r3, #2
	adds r2, #85
	strb r3, [r2]
	adds r3, r6, #0
	adds r3, #35
	strb r5, [r3]
.L_02000d90_1:
	ldr r0, [pc, #16]
	bl 0x0200bc90
.L_02000d90_0:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001f30
	.4byte 0x00000211
	.global Func_02000de8
	.thumb_func
Func_02000de8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #184]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #12
	bl 0x0200bcc8
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #85
	ldrb r3, [r7]
	mov r8, r3
	ldr r3, [r5, #8]
	mov r6, sp
	str r3, [r6]
	ldr r3, [r5, #12]
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	str r3, [r6, #8]
	movs r1, #240
	ldrh r3, [r5, #6]
	lsls r1, r1, #8
	movs r0, #128
	ands r1, r3
	lsls r0, r0, #14
	adds r2, r6, #0
	bl 0x0200bc20
	adds r0, r5, #0
	adds r1, r6, #0
	bl 0x0200bc58
	cmp r0, #0
	bne .L_02000de8_0
	bl 0x0200bcb0
	movs r1, #6
	adds r0, r5, #0
	bl 0x0200bc28
	movs r0, #6
	bl 0x0200bc00
	movs r0, #152
	bl 0x0200be70
	adds r0, r5, #0
	movs r1, #7
	bl 0x0200bc28
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldrb r2, [r7]
	movs r3, #126
	ands r3, r2
	strb r3, [r7]
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bc60
	movs r2, #2
	ldrsh r1, [r6, r2]
	movs r0, #0
	movs r3, #10
	ldrsh r2, [r6, r3]
	bl 0x0200bcf8
	adds r0, r5, #0
	movs r1, #6
	bl 0x0200bc28
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200bc60
	mov r2, r8
	strb r2, [r7]
	bl 0x0200bcb8
.L_02000de8_0:
	sub sp, #-12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.global Func_02000eac
	.thumb_func
Func_02000eac:
	push {r5, lr}
	sub sp, #32
	bl 0x0200bcb0
	add r5, sp, #8
	adds r0, r5, #0
	bl 0x02008474
	cmp r0, #0
	beq .L_02000eac_0
	mov r2, sp
	add r3, sp, #24
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [r5]
	ldr r1, [r5, #4]
	ldr r2, [r5, #8]
	ldr r3, [r5, #12]
	bl 0x02008608
.L_02000eac_0:
	bl 0x0200bcb8
	sub sp, #-32
	pop {r5}
	pop {r0}
	bx r0
	.global Func_02000ee0
	.thumb_func
Func_02000ee0:
	adds r1, r0, #0
	adds r1, #100
	ldrh r3, [r1]
	ldr r2, [pc, #28]
	lsls r3, r3, #16
	asrs r3, r3, #18
	ldr r4, [pc, #24]
	ands r3, r2
	lsls r3, r3, #2
	ldr r3, [r4, r3]
	str r3, [r0, #24]
	str r3, [r0, #28]
	ldrh r3, [r1]
	movs r2, #15
	adds r3, #1
	ands r3, r2
	strh r3, [r1]
	b .L_02000ee0_0
	.4byte 0x00000003
	.4byte 0x0200c468
.L_02000ee0_0:
	bx lr
	.2byte 0x0000
	.global Func_02000f10
	.thumb_func
Func_02000f10:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #252
	lsls r3, r3, #17
	mov r8, r3
	movs r3, #192
	lsls r3, r3, #13
	sub sp, #12
	mov r9, r1
	mov r11, r2
	mov r10, r3
	bl 0x0200bcc8
	mov r3, r8
	mov r5, sp
	adds r6, r0, #0
	str r3, [r5]
	mov r0, r9
	mov r3, r10
	mov r1, r11
	adds r2, r5, #0
	str r3, [r5, #8]
	bl 0x0200bc20
	ldr r3, [r5]
	str r3, [r6, #8]
	movs r7, #144
	ldr r3, [r5, #8]
	lsls r7, r7, #16
	str r3, [r6, #12]
	str r7, [r6, #16]
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
	.global Func_02000f6c
	.thumb_func
Func_02000f6c:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r5, #0
.L_02000f6c_0:
	adds r0, r5, #0
	movs r1, #192
	adds r2, r6, #0
	adds r0, #11
	lsls r1, r1, #13
	bl 0x02008f10
	ldr r3, [pc, #12]
	adds r5, #1
	adds r6, r6, r3
	cmp r5, #4
	ble .L_02000f6c_0
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0xffffcccd
	.global Func_02000f94
	.thumb_func
Func_02000f94:
	push {r5, r6, r7, lr}
	ldr r4, [pc, #228]
	ldr r5, [r4]
	movs r0, #0
	ldrsh r2, [r5, r0]
	sub sp, #4
	movs r7, #1
	ldrh r1, [r5]
	cmp r2, #0
	bne .L_02000f94_0
	ldrh r3, [r5, #8]
	ldr r0, [pc, #212]
	adds r3, #16
	strh r3, [r5, #8]
	lsls r3, r3, #16
	cmp r3, r0
	bls .L_02000f94_1
	adds r3, r1, #1
	strh r3, [r5]
	strh r2, [r5, #2]
	b .L_02000f94_1
.L_02000f94_0:
	cmp r2, #1
	bne .L_02000f94_2
	movs r2, #2
	ldrsh r3, [r5, r2]
	cmp r3, #30
	bne .L_02000f94_1
	b .L_02000f94_3
.L_02000f94_2:
	cmp r2, #2
	bne .L_02000f94_4
	ldrh r3, [r5, #8]
	ldr r0, [pc, #176]
	ldr r2, [pc, #176]
	adds r3, r3, r0
	strh r3, [r5, #8]
	lsls r3, r3, #16
	cmp r3, r2
	bhi .L_02000f94_1
.L_02000f94_3:
	adds r3, r1, #1
	strh r3, [r5]
	b .L_02000f94_1
.L_02000f94_4:
	cmp r2, #3
	bne .L_02000f94_5
	ldr r3, [pc, #160]
	movs r6, #0
	ldrsb r6, [r3, r6]
	movs r1, #5
	lsls r0, r6, #16
	str r4, [sp, #0]
	bl 0x0200bbe8
	ldrh r3, [r5, #6]
	ldr r2, [pc, #144]
	subs r3, r3, r0
	lsls r3, r3, #16
	adds r3, r3, r2
	ldr r2, [pc, #140]
	ldr r4, [sp, #0]
	cmp r3, r2
	bhi .L_02000f94_1
	movs r2, #128
	lsls r2, r2, #7
	adds r3, r0, r2
	strh r3, [r5, #6]
	movs r2, #0
	movs r3, #99
	adds r0, r6, #0
	strh r3, [r5]
	strh r2, [r5, #8]
	adds r0, #11
	bl 0x0200bcc8
	ldr r3, [pc, #112]
	ldr r4, [sp, #0]
	str r3, [r0, #108]
	b .L_02000f94_1
.L_02000f94_5:
	cmp r2, #99
	bne .L_02000f94_1
	movs r7, #0
.L_02000f94_1:
	cmp r7, #0
	beq .L_02000f94_6
	ldr r2, [r4]
	ldrh r3, [r2, #6]
	ldrh r1, [r2, #8]
	adds r3, r3, r1
	strh r3, [r2, #6]
	ldrh r0, [r2, #6]
	str r4, [sp, #0]
	bl 0x02008f6c
	ldr r4, [sp, #0]
	ldr r1, [r4]
	ldrh r3, [r1, #10]
	ldrh r2, [r1, #8]
	movs r0, #192
	adds r3, r3, r2
	strh r3, [r1, #10]
	lsls r0, r0, #22
	lsls r3, r3, #16
	cmp r3, r0
	bls .L_02000f94_6
	movs r3, #0
	strh r3, [r1, #10]
	movs r0, #135
	bl 0x0200be70
	ldr r4, [sp, #0]
.L_02000f94_6:
	ldr r2, [r4]
	ldrh r3, [r2, #2]
	adds r3, #1
	strh r3, [r2, #2]
	sub sp, #-4
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200bf6c
	.4byte 0x0bff0000
	.4byte 0x0000fff8
	.4byte 0x02ff0000
	.4byte 0x02001002
	.4byte 0xc2ff0000
	.4byte 0x05fe0000
	.4byte 0x02008ee1
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #0
	sub	sp, #4
	mov	r9, r2
	adds	r7, r0, #0
	bl 0x0200bcb0
	bl 0x0200bde8
	ldr	r0, [pc, #792]
	bl 0x0200bd60
	movs	r0, #16
	movs	r1, #0
	bl 0x0200bd70
	movs	r6, #128
	bl 0x0200bcb8
	movs	r4, #0
	mov	r8, r4
	lsls	r6, r6, #9
.L_020010d4:
	adds	r0, r4, #0
	adds	r0, #11
	str	r4, [sp, #0]
	bl 0x0200bcc8
	ldr	r4, [sp, #0]
	adds	r5, r0, #0
	mov	r3, r8
	adds	r4, #1
	str	r3, [r5, #108]
	str	r6, [r5, #24]
	str	r6, [r5, #28]
	cmp	r4, #4
	ble.n	.L_020010d4
	ldr	r3, [pc, #740]
	ldrb	r2, [r3, #0]
	mov	r8, r2
	ldr	r2, [pc, #740]
	movs	r6, #1
	ldrsb	r6, [r3, r6]
	ldr	r5, [r2, #0]
	mov	sl, r2
	ldrb	r2, [r3, #1]
	lsls	r0, r6, #16
	movs	r1, #5
	mov	fp, r2
	bl 0x0200bbe8
	movs	r3, #128
	lsls	r3, r3, #7
	mov	r2, r8
	adds	r0, r0, r3
	lsls	r3, r2, #24
	asrs	r3, r3, #24
	strh	r0, [r5, #6]
	cmp	r3, #0
	bne.n	.L_0200113c
	cmp	r7, #16
	bne.n	.L_0200112e
	movs	r3, #1
	movs	r0, #110
	mov	r8, r3
	bl 0x0200be70
	b.n	.L_02001134
.L_0200112e:
	movs	r0, #114
	bl 0x0200be70
.L_02001134:
	ldr	r2, [pc, #680]
	movs	r3, #0
	strb	r3, [r2, #0]
	b.n	.L_02001256
.L_0200113c:
	cmp	r3, #1
	bne.n	.L_020011da
	cmp	r7, #16
	bne.n	.L_0200114c
	movs	r0, #110
	bl 0x0200be70
	b.n	.L_02001256
.L_0200114c:
	cmp	r7, #20
	bne.n	.L_020011ce
	movs	r2, #2
	movs	r0, #110
	mov	r8, r2
	bl 0x0200be70
	movs	r0, #30
	bl 0x0200bc00
	mov	r2, sl
	ldr	r3, [r2, #0]
	movs	r2, #6
	ldrsh	r7, [r3, r2]
	ldr	r3, [pc, #632]
	movs	r4, #0
	mov	sl, r3
.L_0200116e:
	adds	r5, r4, #0
	adds	r5, #11
	lsls	r7, r7, #16
	movs	r1, #192
	lsrs	r2, r7, #16
	adds	r0, r5, #0
	lsls	r1, r1, #13
	str	r4, [sp, #0]
	bl 0x02008f10
	movs	r0, #151
	bl 0x0200be70
	adds	r0, r5, #0
	bl 0x0200bcc8
	movs	r3, #0
	adds	r5, r0, #0
	str	r3, [r5, #24]
	ldr	r6, [pc, #592]
	ldr	r4, [sp, #0]
.L_02001198:
	str	r6, [r5, #28]
	str	r6, [r5, #24]
	movs	r0, #1
	str	r4, [sp, #0]
	bl 0x0200bc00
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r6, r6, r2
	ldr	r3, [r5, #24]
	ldr	r2, [pc, #572]
	ldr	r4, [sp, #0]
	cmp	r3, r2
	ble.n	.L_02001198
	lsrs	r3, r7, #16
	add	r3, sl
	lsls	r3, r3, #16
	adds	r4, #1
	asrs	r7, r3, #16
	cmp	r4, #4
	ble.n	.L_0200116e
	movs	r0, #30
	bl 0x0200bc00
	movs	r3, #1
	mov	r9, r3
	b.n	.L_02001256
.L_020011ce:
	movs	r0, #114
	bl 0x0200be70
	movs	r2, #0
	mov	r8, r2
	b.n	.L_02001256
.L_020011da:
	cmp	r3, #2
	bne.n	.L_02001256
	adds	r3, r6, #0
	adds	r3, #16
	cmp	r7, r3
	beq.n	.L_02001246
	movs	r3, #0
	movs	r0, #114
	mov	r8, r3
	bl 0x0200be70
	movs	r0, #30
	bl 0x0200bc00
	movs	r4, #0
.L_020011f8:
	adds	r7, r4, #0
	adds	r7, #11
	adds	r0, r7, #0
	str	r4, [sp, #0]
	bl 0x0200bcc8
	adds	r5, r0, #0
	movs	r0, #151
	bl 0x0200be70
	ldr	r6, [r5, #24]
	ldr	r2, [pc, #472]
	ldr	r4, [sp, #0]
	cmp	r6, r2
	ble.n	.L_02001230
.L_02001216:
	str	r6, [r5, #28]
	str	r6, [r5, #24]
	movs	r0, #1
	str	r4, [sp, #0]
	bl 0x0200bc00
	ldr	r3, [pc, #460]
	ldr	r2, [pc, #448]
	adds	r6, r6, r3
	ldr	r3, [r5, #24]
	ldr	r4, [sp, #0]
	cmp	r3, r2
	bgt.n	.L_02001216
.L_02001230:
	adds	r0, r7, #0
	movs	r1, #0
	movs	r2, #0
	str	r4, [sp, #0]
	bl 0x0200bd18
	ldr	r4, [sp, #0]
	adds	r4, #1
	cmp	r4, #4
	ble.n	.L_020011f8
	b.n	.L_02001256
.L_02001246:
	movs	r0, #110
	bl 0x0200be70
	movs	r3, #1
	movs	r0, #30
	mov	r9, r3
	bl 0x0200bc00
.L_02001256:
	ldr	r7, [pc, #384]
	mov	r2, r8
	mov	r3, r9
	strb	r2, [r7, #0]
	cmp	r3, #0
	bne.n	.L_02001264
	b.n	.L_020013c0
.L_02001264:
	subs	r3, r7, #1
	ldrb	r5, [r3, #0]
	adds	r5, #1
	strb	r5, [r3, #0]
	bl 0x0200bc18
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	add	r0, fp
	adds	r0, #1
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	movs	r1, #5
	adds	r0, #5
	bl 0x0200bbf0
	ldr	r6, [pc, #340]
	strb	r0, [r7, #1]
	ldr	r2, [r6, #0]
	movs	r3, #0
	strh	r3, [r2, #0]
	strh	r3, [r2, #2]
	movs	r3, #128
	lsls	r3, r3, #2
	strh	r3, [r2, #8]
	movs	r3, #192
	lsls	r5, r5, #24
	lsls	r3, r3, #6
	movs	r1, #200
	lsrs	r5, r5, #24
	strh	r3, [r2, #10]
	ldr	r0, [pc, #336]
	lsls	r1, r1, #4
	bl 0x0200bc08
	cmp	r5, #2
	bhi.n	.L_020012d8
	ldr	r3, [r6, #0]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #99
	beq.n	.L_020012ca
	adds	r5, r6, #0
.L_020012ba:
	movs	r0, #1
	bl 0x0200bc00
	ldr	r3, [r5, #0]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #99
	bne.n	.L_020012ba
.L_020012ca:
	movs	r0, #10
	bl 0x0200bc00
	movs	r0, #110
	bl 0x0200be70
	b.n	.L_020013ba
.L_020012d8:
	movs	r3, #99
	strb	r3, [r7, #0]
	ldr	r3, [r6, #0]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #2
	beq.n	.L_020012f8
	adds	r5, r6, #0
.L_020012e8:
	movs	r0, #1
	bl 0x0200bc00
	ldr	r3, [r5, #0]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #2
	bne.n	.L_020012e8
.L_020012f8:
	ldr	r2, [r6, #0]
	movs	r3, #2
	movs	r1, #0
	strh	r3, [r2, #0]
	strh	r1, [r2, #2]
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200bc68
	movs	r0, #20
	bl 0x0200bca8
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200bc68
	ldr	r2, [r6, #0]
	movs	r3, #99
	strh	r3, [r2, #0]
.L_0200132e:
	movs	r0, #190
	bl 0x0200be70
	movs	r3, #192
	lsls	r3, r3, #13
	mov	r8, r3
	ldr	r3, [r6, #0]
	movs	r2, #6
	ldrsh	r7, [r3, r2]
	movs	r3, #192
	lsls	r3, r3, #4
	mov	sl, r3
.L_02001346:
	movs	r4, #0
.L_02001348:
	adds	r6, r4, #0
	adds	r6, #11
	adds	r0, r6, #0
	str	r4, [sp, #0]
	bl 0x0200bcc8
	adds	r5, r0, #0
	ldr	r3, [r5, #24]
	subs	r3, #16
	str	r3, [r5, #24]
	ldr	r3, [r5, #28]
	subs	r3, #16
	str	r3, [r5, #28]
	lsls	r5, r7, #16
	lsrs	r5, r5, #16
	adds	r2, r5, #0
	adds	r0, r6, #0
	mov	r1, r8
	bl 0x02008f10
	ldr	r2, [pc, #112]
	ldr	r4, [sp, #0]
	adds	r5, r5, r2
	lsls	r5, r5, #16
	adds	r4, #1
	asrs	r7, r5, #16
	cmp	r4, #4
	ble.n	.L_02001348
	lsls	r3, r7, #16
	lsrs	r3, r3, #16
	add	r3, sl
	lsls	r3, r3, #16
	add	r8, r2
	movs	r0, #1
	asrs	r7, r3, #16
	bl 0x0200bc00
	mov	r3, r8
	cmp	r3, #0
	bgt.n	.L_02001346
	movs	r4, #0
.L_0200139a:
	adds	r0, r4, #0
	adds	r0, #11
	movs	r1, #0
	movs	r2, #0
	str	r4, [sp, #0]
	bl 0x0200bd18
	ldr	r4, [sp, #0]
	adds	r4, #1
	cmp	r4, #4
	ble.n	.L_0200139a
	bl 0x0200bad4
	movs	r0, #80
	bl 0x0200be70
.L_020013ba:
	ldr	r0, [pc, #56]
	bl 0x0200bc10
.L_020013c0:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x000021db
	.4byte 0x02001001
	.4byte 0x0200bf6c
	.4byte 0x02001000
	.4byte 0xffffcccd
	.4byte 0x00006666
	.4byte 0x0000ffff
	.4byte 0xfffff400
	.2byte 0x8f95
	.2byte 0x0200
	.global Func_020013f8
	.thumb_func
Func_020013f8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r0, [pc, #1008]
	bl 0x0200bc90
	movs r0, #237
	bl 0x0200bca0
	bl 0x0200bcb0
	bl 0x0200bde8
	movs r1, #2
	movs r0, #8
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	ldr r0, [pc, #976]
	bl 0x0200bd60
	movs r0, #8
	movs r1, #0
	bl 0x0200bd70
	movs r0, #0
	ldr r1, [pc, #964]
	ldr r2, [pc, #968]
	bl 0x0200bcd0
	movs r0, #0
	movs r1, #232
	movs r2, #160
	bl 0x0200bd00
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200bd78
	movs r0, #50
	bl 0x0200bca8
	movs r0, #10
	bl 0x0200bca8
	movs r0, #132
	movs r1, #1
	movs r2, #200
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200bd98
	movs r0, #0
	ldr r1, [pc, #904]
	ldr r2, [pc, #904]
	bl 0x0200bcd0
	movs r1, #132
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #208
	bl 0x0200bd00
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #16
	movs r3, #192
	movs r0, #1
	negs r1, r1
	movs r2, #16
	lsls r3, r3, #8
	bl 0x0200be38
	movs r3, #192
	movs r0, #3
	movs r1, #0
	movs r2, #16
	lsls r3, r3, #8
	bl 0x0200be38
	movs r3, #192
	lsls r3, r3, #8
	movs r1, #16
	movs r2, #16
	movs r0, #2
	bl 0x0200be38
	movs r0, #1
	bl 0x0200bd10
	movs r0, #20
	bl 0x0200bca8
	movs r0, #10
	bl 0x0200bca8
	movs r1, #132
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r1, #0
	movs r0, #8
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r0, #3
	movs r1, #2
	movs r2, #50
	bl 0x0200bd50
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200bd78
	movs r0, #20
	bl 0x0200bca8
	movs r0, #8
	bl 0x0200bcc8
	adds r7, r0, #0
	movs r0, #30
	bl 0x0200bca8
	movs r1, #2
	movs r0, #8
	bl 0x0200bd40
.L_02001528:
	movs r0, #30
	bl 0x0200bca8
	movs r1, #2
	movs r0, #8
	bl 0x0200bd40
	movs r0, #30
	bl 0x0200bca8
	movs r1, #2
	movs r0, #8
	bl 0x0200bd40
	movs r0, #60
	bl 0x0200bca8
	movs r0, #17
	bl 0x0200be70
	ldr r2, [r7, #80]
	movs r3, #0
	strh r3, [r2, #30]
	movs r0, #8
	movs r1, #10
	movs r2, #70
	bl 0x0200bd30
	movs r6, #13
	negs r6, r6
	movs r5, #29
.L_02001528_0:
	ldr r1, [r7, #80]
	ldrb r2, [r1, #9]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r1, #9]
	movs r0, #2
	bl 0x0200bca8
	ldr r1, [r7, #80]
	ldrb r2, [r1, #9]
	adds r3, r6, #0
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r1, #9]
	movs r0, #2
	subs r5, #1
	bl 0x0200bca8
	cmp r5, #0
	bge .L_02001528_0
	movs r0, #40
	bl 0x0200bca8
	movs r0, #8
	bl 0x0200bcc8
	movs r2, #10
	ldrsh r6, [r0, r2]
	movs r0, #8
	bl 0x0200bcc8
	movs r3, #18
	ldrsh r5, [r0, r3]
.L_020015aa:
	movs r1, #0
	movs r0, #8
	movs r2, #0
	lsls r5, r5, #16
	lsls r6, r6, #16
	bl 0x0200bd18
	adds r1, r6, #0
	adds r2, r5, #0
	movs r0, #9
	bl 0x0200bd18
.L_020015c2:
	ldr r3, [r7, #80]
	movs r2, #0
	adds r3, #38
	strb r2, [r3]
	movs r0, #30
	bl 0x0200be70
	movs r0, #9
	ldr r1, [pc, #552]
	ldr r2, [pc, #552]
	bl 0x0200bcd0
.L_020015da:
	movs r0, #9
	movs r1, #32
	movs r2, #32
	bl 0x0200be48
	movs r1, #128
.L_020015e6:
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl 0x0200bd78
	movs r0, #20
	bl 0x0200bca8
	movs r0, #0
	bl 0x0200bcc8
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #1
	bl 0x0200bcc8
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #3
	bl 0x0200bcc8
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #2
	bl 0x0200bcc8
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r1, #0
	movs r0, #0
	movs r2, #16
	bl 0x0200be40
	movs r0, #1
	movs r1, #0
	movs r2, #16
	bl 0x0200be40
	movs r0, #3
	movs r1, #0
	movs r2, #16
	bl 0x0200be40
	movs r2, #16
	movs r1, #0
	movs r0, #2
	bl 0x0200be48
	movs r0, #0
	bl 0x0200bcc8
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #1
	bl 0x0200bcc8
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #3
	bl 0x0200bcc8
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #2
	bl 0x0200bcc8
	adds r0, #90
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r1, #1
	movs r0, #0
	bl 0x0200bd20
	movs r0, #1
	movs r1, #1
	bl 0x0200bd20
	movs r0, #3
	movs r1, #1
	bl 0x0200bd20
	movs r1, #1
	movs r0, #2
	bl 0x0200bd20
	movs r0, #1
	bl 0x0200bc00
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r0, #0
	movs r1, #2
	bl 0x0200bd38
	movs r0, #1
	movs r1, #2
	bl 0x0200bd38
	movs r0, #3
	movs r1, #2
	bl 0x0200bd38
	movs r1, #2
	movs r0, #2
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r0, #10
	bl 0x0200bca8
	movs r2, #40
	movs r0, #9
	ldr r1, [pc, #220]
	bl 0x0200bd80
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200bd80
	movs r0, #1
	movs r1, #4
	movs r2, #13
	bl 0x0200bd30
	movs r2, #30
	movs r0, #1
	movs r1, #4
	bl 0x0200bd30
	movs r0, #1
	movs r1, #0
	bl 0x0200bd70
	movs r0, #2
	movs r1, #1
	movs r2, #30
	bl 0x0200bd48
	movs r2, #40
	movs r0, #2
	ldr r1, [pc, #148]
.L_02001770:
	bl 0x0200bd80
	movs r0, #2
	movs r1, #0
	bl 0x0200bd70
	movs r0, #1
	movs r1, #2
	movs r2, #30
	bl 0x0200bd48
	movs r1, #129
	movs r2, #40
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r0, #1
	movs r1, #0
	bl 0x0200bd70
	movs r1, #4
	movs r0, #9
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #9
	bl 0x0200bd68
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r0, #0
	movs r1, #0
	bl 0x0200bcc0
	cmp r0, #0
	bne .L_02001770_0
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #9
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	ldr r0, [pc, #28]
	bl 0x0200bd60
	movs r0, #9
	movs r1, #0
	b .L_02001770_1
	.2byte 0x0962
	.2byte 0x0000
	.2byte 0x2183
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.4byte 0x0000218a
.L_02001770_1:
	bl 0x0200bd70
	b .L_02001770_2
.L_02001770_0:
	ldr r0, [pc, #1020]
	bl 0x0200bd60
	movs r0, #9
	movs r1, #0
	bl 0x0200bd70
.L_02001770_2:
	ldr r0, [pc, #1008]
	bl 0x0200bd60
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #3
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #3
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #2
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #2
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r2, #40
	movs r0, #9
	ldr r1, [pc, #940]
	bl 0x0200bd80
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200bd50
	movs r0, #3
	movs r1, #2
	movs r2, #50
	bl 0x0200bd50
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #1
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #1
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #9
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r2, #40
	movs r0, #2
	ldr r1, [pc, #780]
	bl 0x0200bd80
	movs r1, #0
	movs r0, #2
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #2
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #2
	bl 0x0200bd70
	movs r0, #20
	bl 0x0200bca8
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcd0
	movs r2, #48
	movs r0, #2
	movs r1, #0
	negs r2, r2
	bl 0x0200be48
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200bd78
	movs r0, #40
	bl 0x0200bca8
	movs r0, #141
	movs r1, #1
	bl 0x0200bdc8
	movs r1, #9
	movs r0, #2
	bl 0x0200bdd0
	bl 0x0200bde0
	movs r0, #1
	bl 0x0200bdc0
	movs r0, #150
	bl 0x0200bca8
	movs r0, #2
	bl 0x0200bdc0
	bl 0x0200bdd8
	movs r1, #1
	movs r0, #2
	bl 0x0200bd20
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #2
	bl 0x0200bd40
	movs r0, #40
	bl 0x0200bca8
	movs r0, #10
	bl 0x0200bca8
	movs r2, #40
	movs r0, #3
	ldr r1, [pc, #608]
	bl 0x0200bd80
	movs r1, #0
	movs r0, #3
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #129
	movs r2, #40
	movs r0, #2
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r1, #0
	movs r0, #2
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200bd80
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200bd80
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #3
	bl 0x0200bd80
	movs r0, #10
	bl 0x0200bca8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #2
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #2
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r0, #9
	movs r1, #4
	movs r2, #13
	bl 0x0200bd30
	movs r0, #9
	movs r1, #4
	movs r2, #30
	bl 0x0200bd30
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #50
	bl 0x0200bd80
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #9
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #20
	bl 0x0200bca8
	movs r1, #128
	movs r2, #40
	movs r0, #9
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #1
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #1
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r2, #40
	movs r0, #3
	ldr r1, [pc, #336]
	bl 0x0200bd80
	movs r1, #0
	movs r0, #3
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r2, #40
	movs r0, #9
	ldr r1, [pc, #312]
	bl 0x0200bd80
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #50
	bl 0x0200bca8
	movs r1, #3
	movs r0, #0
	bl 0x0200bd28
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #1
	bl 0x0200bd28
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #3
	bl 0x0200bd28
	movs r0, #10
	bl 0x0200bca8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200bd78
	movs r0, #20
	bl 0x0200bca8
	movs r1, #3
	movs r0, #2
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #9
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r0, #20
	bl 0x0200bca8
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcd0
	movs r1, #0
	movs r2, #48
	movs r0, #2
	bl 0x0200be48
	movs r0, #10
	bl 0x0200bca8
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200bd50
	movs r0, #3
	movs r1, #2
	movs r2, #50
	bl 0x0200bd50
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #2
	bl 0x0200bd78
	movs r0, #50
	bl 0x0200bca8
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #0
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #4
	movs r0, #9
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #129
	movs r2, #40
	movs r0, #2
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r1, #0
	movs r0, #2
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #9
	bl 0x0200bd40
	b .L_02001770_3
	.2byte 0x0000
	.4byte 0x0000218b
	.4byte 0x0000218c
	.4byte 0x00000101
.L_02001770_3:
	movs r0, #30
	bl 0x0200bca8
	movs r1, #3
	movs r0, #9
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r0, #10
	bl 0x0200bca8
	movs r1, #4
	movs r0, #1
	bl 0x0200bd28
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #1
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #4
	movs r0, #9
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200bd80
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200bd80
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200bd80
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #80
	movs r0, #2
	bl 0x0200bd80
	movs r0, #20
	bl 0x0200bca8
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl 0x0200bd78
	movs r0, #50
	bl 0x0200bca8
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r2, #40
	movs r0, #2
	ldr r1, [pc, #128]
	bl 0x0200bd80
	movs r1, #0
	movs r0, #2
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #9
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r0, #10
	bl 0x0200bca8
	movs r2, #40
	movs r0, #3
	ldr r1, [pc, #84]
	bl 0x0200bd80
	movs r1, #0
	movs r0, #3
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #9
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r0, #10
	bl 0x0200bca8
	movs r0, #1
	ldr r1, [pc, #44]
	movs r2, #40
	bl 0x0200bd80
	movs r1, #0
	movs r0, #1
	bl 0x0200bd68
	movs r0, #0
	movs r1, #0
	bl 0x0200bcc0
	cmp r0, #0
	bne .L_02001770_4
	ldr r0, [pc, #20]
	bl 0x0200bd60
	movs r0, #1
	movs r1, #0
	bl 0x0200bd70
	b .L_02001770_5
	.2byte 0x0000
	.4byte 0x00000101
	.4byte 0x000021a4
.L_02001770_4:
	ldr r0, [pc, #1012]
	bl 0x0200bd60
	movs r0, #1
	movs r1, #0
	bl 0x0200bd70
.L_02001770_5:
	ldr r0, [pc, #1004]
	bl 0x0200bd60
	movs r0, #10
	bl 0x0200bca8
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200bd80
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #9
	bl 0x0200bd78
	movs r0, #10
	bl 0x0200bca8
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #1
	bl 0x0200bd80
	movs r0, #10
	bl 0x0200bca8
	movs r0, #1
	movs r1, #0
	movs r2, #50
	bl 0x0200bd50
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r0, #10
	bl 0x0200bca8
	movs r0, #0
	movs r1, #3
	bl 0x0200bd20
	movs r1, #3
	movs r0, #1
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #9
	bl 0x0200bd68
	ldr r0, [pc, #856]
	bl 0x0200bd60
	movs r0, #0
	movs r1, #0
	bl 0x0200bcc0
	ldr r3, [pc, #848]
	ldr r3, [r3]
	ldr r2, [pc, #848]
	mov r9, r3
	add r2, r9
	movs r3, #32
	strh r3, [r2]
	ldr r2, [pc, #840]
	ldr r3, [pc, #844]
	ldr r7, [pc, #844]
	movs r5, #0
	movs r6, #16
	mov r10, r2
	mov r8, r3
.L_02001770_6:
	mov r2, r10
	mov r3, r8
	strh r2, [r3]
	lsls r3, r5, #8
	orrs r3, r6
	strh r3, [r7]
	movs r0, #7
	adds r5, #1
	bl 0x0200bc00
	subs r6, #1
	cmp r5, #16
	ble .L_02001770_6
	ldr r2, [pc, #812]
	ldr r3, [pc, #816]
	add r2, r9
	strh r3, [r2]
	movs r1, #140
	movs r2, #152
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200bd18
	movs r1, #148
	movs r2, #152
	lsls r2, r2, #17
	movs r0, #11
	lsls r1, r1, #17
	bl 0x0200bd18
	movs r0, #10
	movs r1, #0
	bl 0x0200bd70
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #2
	bl 0x0200bd78
	movs r0, #10
	bl 0x0200bca8
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #3
	bl 0x0200bd78
	movs r0, #10
	bl 0x0200bca8
	movs r1, #128
.L_02001e8e:
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #1
	bl 0x0200bd78
	movs r0, #10
	bl 0x0200bca8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #0
	bl 0x0200bd78
	movs r0, #10
	bl 0x0200bca8
	movs r0, #10
	bl 0x0200bca8
	movs r1, #1
	movs r0, #10
	bl 0x0200bd90
	bl 0x0200bda0
	movs r0, #70
	bl 0x0200bca8
	movs r1, #129
	movs r2, #40
	movs r0, #11
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r0, #11
	movs r1, #0
	bl 0x0200bd70
	movs r0, #0
	movs r1, #10
	bl 0x0200be50
	movs r0, #1
	movs r1, #10
	bl 0x0200be50
	movs r0, #3
	movs r1, #10
	bl 0x0200be50
	movs r0, #2
	movs r1, #10
	bl 0x0200be50
	movs r0, #10
	ldr r1, [pc, #628]
	ldr r2, [pc, #628]
	bl 0x0200bcd0
	ldr r2, [pc, #624]
	movs r0, #11
	ldr r1, [pc, #616]
	bl 0x0200bcd0
	ldr r5, [pc, #616]
	movs r0, #10
	adds r1, r5, #0
	bl 0x0200bcd8
	movs r0, #3
	bl 0x0200bca8
	movs r1, #16
	movs r2, #0
	movs r0, #11
	negs r1, r1
	bl 0x0200be48
	adds r1, r5, #0
	movs r0, #11
	bl 0x0200bcd8
	movs r0, #10
	bl 0x0200bce0
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #10
	bl 0x0200be48
	movs r0, #11
	bl 0x0200bce0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200bd78
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl 0x0200bd78
	movs r0, #0
	bl 0x0200bce8
	movs r0, #1
	bl 0x0200bce8
	movs r0, #3
	bl 0x0200bce8
	movs r0, #2
	bl 0x0200bce8
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r0, #248
	movs r1, #1
	movs r2, #216
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #16
	bl 0x0200bd98
	bl 0x0200bda0
	movs r0, #20
	bl 0x0200bca8
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #10
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #10
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #9
	bl 0x0200bd78
	movs r0, #20
	bl 0x0200bca8
	movs r1, #2
	movs r0, #9
	bl 0x0200bd40
	movs r0, #50
	bl 0x0200bca8
	movs r0, #10
	bl 0x0200bca8
	movs r2, #40
	movs r0, #11
	ldr r1, [pc, #352]
	bl 0x0200bd80
	movs r1, #0
	movs r0, #11
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #9
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl 0x0200bd78
	movs r0, #10
	bl 0x0200bca8
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #129
	movs r2, #40
	movs r0, #10
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r1, #0
	movs r0, #10
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #129
	movs r2, #40
	movs r0, #9
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r0, #9
	movs r1, #0
	bl 0x0200bd70
	movs r2, #0
	movs r1, #16
.L_02002090:
	movs r0, #11
	bl 0x0200be48
	movs r0, #20
	bl 0x0200bca8
	movs r1, #2
	movs r0, #11
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #11
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #9
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r0, #9
	movs r1, #0
	bl 0x0200bd70
	movs r2, #0
	movs r1, #16
	movs r0, #10
	bl 0x0200be48
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #10
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #128
	movs r2, #40
	movs r0, #9
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r0, #11
	movs r1, #4
	movs r2, #13
	bl 0x0200bd30
	movs r2, #30
	movs r0, #11
	movs r1, #4
	bl 0x0200bd30
	movs r1, #0
	movs r0, #11
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #129
	movs r2, #40
	movs r0, #9
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #10
	bl 0x0200bd28
	b .L_02002090_0
	.2byte 0x21a5
	.2byte 0x0000
	.2byte 0x21a6
	.2byte 0x0000
	.2byte 0x21a8
	.2byte 0x0000
	.2byte 0x1ecc
	.2byte 0x0300
	.2byte 0x052a
	.2byte 0x0000
	.2byte 0x3f42
	.2byte 0x0000
	.2byte 0x0050
	.2byte 0x0400
	.2byte 0x0052
	.2byte 0x0400
	.2byte 0x0536
	.2byte 0x0000
	.2byte 0x3f3f
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0001
	.2byte 0xb333
	.2byte 0x0000
	.2byte 0xc478
	.2byte 0x0200
	.2byte 0x0101
	.2byte 0x0000
.L_02002090_0:
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #10
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #9
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl 0x0200bd78
	movs r0, #10
	bl 0x0200bca8
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #11
	bl 0x0200bd78
	movs r0, #70
	bl 0x0200bca8
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl 0x0200bd78
	movs r0, #10
	bl 0x0200bca8
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r0, #10
	bl 0x0200bca8
	movs r2, #40
	movs r0, #11
	ldr r1, [pc, #1020]
	bl 0x0200bd80
	movs r1, #0
	movs r0, #11
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #9
	bl 0x0200bd80
	movs r0, #20
	bl 0x0200bca8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r0, #10
	movs r1, #11
	movs r2, #70
	bl 0x0200bd50
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200bd78
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #11
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #3
	movs r0, #10
	bl 0x0200bd28
	movs r0, #30
.L_0200227e:
	bl 0x0200bca8
	movs r1, #3
	movs r0, #11
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #11
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #11
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200bd50
	movs r0, #3
	movs r1, #2
	movs r2, #50
	bl 0x0200bd50
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r0, #10
	bl 0x0200bca8
	movs r0, #0
	movs r1, #3
	bl 0x0200bd20
	movs r0, #1
	movs r1, #3
	bl 0x0200bd20
	movs r0, #3
	movs r1, #3
	bl 0x0200bd20
	movs r1, #3
	movs r0, #2
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r0, #10
	bl 0x0200bca8
	movs r1, #132
	movs r2, #40
	movs r0, #11
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r1, #0
	movs r0, #11
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl 0x0200bd78
	movs r0, #10
	bl 0x0200bca8
	movs r2, #0
	movs r1, #0
	movs r0, #11
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #2
	movs r0, #10
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #10
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #129
	movs r2, #40
	movs r0, #9
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r0, #10
	movs r1, #3
	bl 0x0200bd20
	movs r1, #3
	movs r0, #11
	bl 0x0200bd28
	movs r0, #20
	bl 0x0200bca8
	movs r1, #3
	movs r0, #9
	bl 0x0200bd28
	movs r0, #40
	bl 0x0200bca8
	movs r0, #0
	movs r1, #9
	bl 0x0200be50
	movs r0, #1
	movs r1, #9
	bl 0x0200be50
	movs r0, #3
	movs r1, #9
	bl 0x0200be50
	movs r0, #2
	movs r1, #9
	bl 0x0200be50
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcd0
	movs r1, #16
	movs r0, #9
	negs r1, r1
	movs r2, #0
	bl 0x0200be48
	movs r0, #10
	movs r1, #0
	movs r2, #48
	bl 0x0200be40
	movs r0, #11
	movs r1, #0
	movs r2, #48
	bl 0x0200be40
	movs r1, #16
	movs r0, #9
	negs r1, r1
	movs r2, #0
	bl 0x0200be48
	movs r2, #32
	movs r0, #9
	movs r1, #0
	bl 0x0200be48
	movs r0, #10
	movs r1, #1
	bl 0x0200bd20
	movs r1, #1
	movs r0, #11
	bl 0x0200bd20
	movs r0, #0
	bl 0x0200bce8
	movs r0, #1
	bl 0x0200bce8
	movs r0, #3
	bl 0x0200bce8
	movs r0, #2
	bl 0x0200bce8
	movs r0, #10
	bl 0x0200bca8
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200bd80
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl 0x0200bd78
	movs r0, #20
	bl 0x0200bca8
	movs r0, #9
	movs r1, #0
	bl 0x0200bd70
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #11
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #2
	movs r0, #1
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #1
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r2, #30
	movs r0, #1
	movs r1, #0
	bl 0x0200bd48
	movs r1, #2
	movs r0, #1
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r2, #30
	movs r0, #1
	movs r1, #9
	bl 0x0200bd48
	movs r1, #0
	movs r0, #1
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #2
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #2
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #3
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #3
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #9
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #20
	bl 0x0200bca8
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl 0x0200bd78
	movs r0, #70
	bl 0x0200bca8
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl 0x0200bd78
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #20
	bl 0x0200bca8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #9
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #3
	movs r0, #9
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	ldr r1, [pc, #128]
	movs r0, #11
	bl 0x0200bcd8
	ldr r1, [pc, #124]
	movs r0, #10
	bl 0x0200bcd8
	ldr r1, [pc, #120]
	movs r0, #9
	bl 0x0200bcd8
	movs r0, #10
	bl 0x0200bca8
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200bd50
	movs r2, #0
	movs r1, #2
	movs r0, #3
	bl 0x0200bd50
	movs r0, #9
	bl 0x0200bce0
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200bd80
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200bd80
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200bd80
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200bd80
	movs r1, #128
	movs r0, #0
	b .L_0200227e_0
	.2byte 0x0000
	.2byte 0x0101
	.2byte 0x0000
	.4byte 0x0200c4c8
	.4byte 0x0200c518
	.4byte 0x0200c57c
.L_0200227e_0:
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bd78
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bd78
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bd78
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #2
	bl 0x0200bd78
	movs r0, #20
	bl 0x0200bca8
	movs r0, #248
	movs r1, #1
	movs r2, #248
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #16
	bl 0x0200bd98
	bl 0x0200bda0
	movs r0, #20
	bl 0x0200bca8
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl 0x0200bd78
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #10
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl 0x0200bd78
	movs r0, #10
	bl 0x0200bca8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #11
	bl 0x0200bd78
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #10
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #4
	movs r0, #11
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #11
	bl 0x0200bd70
	movs r0, #20
	bl 0x0200bca8
	movs r0, #9
	ldr r1, [pc, #968]
	movs r2, #40
	bl 0x0200bd80
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl 0x0200bd78
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #129
	movs r0, #10
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200bd80
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #11
	bl 0x0200bd80
	movs r0, #30
	bl 0x0200bca8
	movs r0, #10
	movs r1, #11
	movs r2, #60
	bl 0x0200bd48
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #10
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #3
	movs r0, #10
	bl 0x0200bd28
	movs r0, #40
	bl 0x0200bca8
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #3
	movs r0, #9
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #9
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #9
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #3
	movs r0, #9
	bl 0x0200bd28
	movs r0, #20
	bl 0x0200bca8
	movs r0, #10
	movs r1, #3
	bl 0x0200bd20
	movs r1, #3
	movs r0, #11
	bl 0x0200bd28
	movs r0, #40
	bl 0x0200bca8
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl 0x0200bd78
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl 0x0200bd78
	movs r0, #40
	bl 0x0200bca8
	movs r0, #9
	movs r1, #32
	movs r2, #0
	bl 0x0200be40
	movs r0, #11
	movs r1, #0
	movs r2, #64
	bl 0x0200be40
	movs r0, #10
	movs r1, #16
	movs r2, #0
	bl 0x0200be48
	movs r1, #0
	movs r2, #64
	movs r0, #10
	bl 0x0200be40
	movs r0, #9
	bl 0x0200bd10
	movs r0, #9
	movs r1, #0
	movs r2, #64
	bl 0x0200be48
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200bd18
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200bd18
	movs r2, #0
	movs r0, #11
	movs r1, #0
	bl 0x0200bd18
	movs r0, #20
	bl 0x0200bca8
	ldr r2, [pc, #628]
	ldr r3, [pc, #628]
	ldr r7, [pc, #632]
	movs r5, #0
	movs r6, #16
	mov r10, r2
	mov r8, r3
.L_0200227e_1:
	mov r2, r10
	mov r3, r8
	strh r2, [r3]
	lsls r3, r6, #8
	orrs r3, r5
	strh r3, [r7]
	movs r0, #7
	adds r5, #1
	bl 0x0200bc00
	subs r6, #1
	cmp r5, #16
	ble .L_0200227e_1
	ldr r2, [pc, #600]
	movs r3, #5
	add r2, r9
	strh r3, [r2]
	ldr r2, [pc, #596]
	movs r3, #31
	add r2, r9
	strh r3, [r2]
	movs r0, #1
	bl 0x0200bc00
	ldr r2, [pc, #564]
	ldr r3, [pc, #568]
	strh r2, [r3]
	ldr r2, [pc, #580]
	adds r3, #2
	strh r2, [r3]
	movs r0, #0
	movs r1, #1
	bl 0x0200bda8
	bl 0x0200bda0
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #1
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #1
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #2
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #2
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #129
	movs r2, #40
	movs r0, #3
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r1, #0
	movs r0, #3
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r2, #0
	movs r1, #0
	movs r0, #1
	bl 0x0200bd48
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #1
	bl 0x0200bd68
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200bd48
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200bd48
	movs r0, #0
	movs r1, #0
	bl 0x0200bcc0
	cmp r0, #0
	bne .L_0200227e_2
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #1
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	ldr r0, [pc, #400]
	bl 0x0200bd60
	movs r0, #1
	movs r1, #0
	bl 0x0200bd70
	b .L_0200227e_3
.L_0200227e_2:
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #1
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	ldr r0, [pc, #368]
	bl 0x0200bd60
	movs r0, #1
	movs r1, #0
	bl 0x0200bd70
.L_0200227e_3:
	ldr r0, [pc, #360]
	bl 0x0200bd60
	movs r0, #10
	bl 0x0200bca8
	movs r1, #4
	movs r0, #2
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #2
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #3
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #3
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #129
	movs r2, #40
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r1, #0
	movs r0, #1
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #2
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #2
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #3
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #3
	bl 0x0200bd70
	movs r0, #20
	bl 0x0200bca8
	movs r1, #128
	movs r2, #40
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r0, #1
	movs r1, #0
	bl 0x0200bd70
	movs r1, #0
	movs r0, #1
	bl 0x0200bd68
	movs r0, #0
	movs r1, #0
	bl 0x0200bcc0
	cmp r0, #0
	bne .L_0200227e_4
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #1
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	ldr r0, [pc, #152]
	bl 0x0200bd60
	movs r1, #0
	movs r0, #1
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r0, #3
	movs r1, #2
	movs r2, #50
	bl 0x0200bd50
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #2
	bl 0x0200bd70
	movs r0, #20
	bl 0x0200bca8
	movs r0, #0
	movs r1, #3
	bl 0x0200bd20
	movs r0, #1
	movs r1, #3
	bl 0x0200bd20
	movs r0, #3
	movs r1, #3
	bl 0x0200bd20
	movs r0, #2
	movs r1, #3
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	b .L_0200227e_5
	.2byte 0x0000
	.4byte 0x00000107
	.4byte 0x00003f42
	.4byte 0x04000050
	.4byte 0x04000052
	.4byte 0x0000052a
	.4byte 0x00000536
	.4byte 0x00000c04
	.4byte 0x000021ce
	.4byte 0x000021cf
	.4byte 0x000021d0
	.4byte 0x000021d7
.L_0200227e_4:
	movs r0, #10
	bl 0x0200bca8
	movs r1, #4
	movs r0, #1
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	ldr r0, [pc, #328]
	bl 0x0200bd60
	movs r1, #0
	movs r0, #1
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r0, #3
	movs r1, #2
	movs r2, #50
	bl 0x0200bd50
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #2
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #0
	bl 0x0200bd28
	movs r0, #20
	bl 0x0200bca8
	movs r1, #3
	movs r0, #2
	bl 0x0200bd28
	movs r0, #20
	bl 0x0200bca8
	movs r1, #3
	movs r0, #3
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #70
	bl 0x0200bd80
.L_0200227e_5:
	movs r0, #17
	bl 0x0200be70
	movs r0, #1
	ldr r1, [pc, #196]
	ldr r2, [pc, #200]
	bl 0x0200bcd0
	movs r0, #2
	ldr r1, [pc, #188]
	ldr r2, [pc, #188]
	bl 0x0200bcd0
	movs r0, #3
	ldr r1, [pc, #176]
	ldr r2, [pc, #180]
	bl 0x0200bcd0
	movs r0, #1
	movs r1, #2
	bl 0x0200bd20
	movs r0, #0
	bl 0x0200bcc8
	cmp r0, #0
	beq .L_0200227e_6
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200bcf0
.L_0200227e_6:
	movs r0, #1
	bl 0x0200bd10
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200bd18
	movs r0, #2
	movs r1, #2
	bl 0x0200bd20
	movs r0, #0
	bl 0x0200bcc8
	cmp r0, #0
	beq .L_0200227e_7
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x0200bcf0
.L_0200227e_7:
	movs r0, #2
	bl 0x0200bd10
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200bd18
	movs r0, #3
	movs r1, #2
	bl 0x0200bd20
	movs r0, #0
	bl 0x0200bcc8
	cmp r0, #0
	beq .L_0200227e_8
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl 0x0200bcf0
.L_0200227e_8:
	movs r0, #3
	bl 0x0200bd10
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200bd18
	bl 0x0200bdf0
	bl 0x0200bcb8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000021d9
	.4byte 0x00013333
	.4byte 0x00009999
	.global Func_02002c44
	.thumb_func
Func_02002c44:
	push {r5, lr}
	movs r0, #150
	lsls r0, r0, #4
	bl 0x0200bc90
	movs r0, #24
	bl 0x0200be70
	bl 0x0200bcb0
	bl 0x0200bde8
	ldr r0, [pc, #880]
	bl 0x0200bd60
	movs r1, #2
	movs r0, #8
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #8
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r0, #248
	movs r1, #1
	movs r2, #184
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200bd98
	movs r0, #0
	ldr r1, [pc, #832]
	ldr r2, [pc, #832]
	bl 0x0200bcd0
	movs r0, #0
	movs r1, #248
	movs r2, #192
	bl 0x0200bd00
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #16
	movs r3, #192
	movs r0, #1
	negs r1, r1
	movs r2, #16
	lsls r3, r3, #8
	bl 0x0200be38
	movs r3, #192
	movs r0, #3
	movs r1, #0
	movs r2, #16
	lsls r3, r3, #8
	bl 0x0200be38
	movs r3, #192
	lsls r3, r3, #8
	movs r1, #16
	movs r2, #16
	movs r0, #2
	bl 0x0200be38
	movs r0, #1
	bl 0x0200bd10
	movs r0, #20
	bl 0x0200bca8
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #2
	bl 0x0200bd80
	movs r0, #40
	bl 0x0200bca8
	movs r0, #2
	movs r1, #0
	bl 0x0200bd70
	movs r1, #2
	movs r0, #3
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #3
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #128
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r1, #0
	movs r0, #8
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r0, #1
	movs r1, #2
	movs r2, #50
	bl 0x0200bd50
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #1
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200bd88
	movs r1, #2
	movs r0, #8
	bl 0x0200bd40
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #8
	bl 0x0200bd70
	movs r0, #20
	bl 0x0200bca8
	movs r0, #3
	movs r1, #2
	movs r2, #50
	bl 0x0200bd50
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #2
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #128
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r1, #0
	movs r0, #8
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #4
	movs r0, #3
	bl 0x0200bd28
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #3
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r2, #40
	movs r0, #8
	ldr r1, [pc, #468]
	bl 0x0200bd80
	movs r1, #0
	movs r0, #8
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r0, #1
	movs r1, #0
	movs r2, #30
	bl 0x0200bd48
	movs r2, #40
	movs r0, #1
	ldr r1, [pc, #440]
	bl 0x0200bd80
	movs r0, #1
	movs r1, #0
	bl 0x0200bd70
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200bd78
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #1
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #129
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r0, #8
	movs r1, #0
	bl 0x0200bd70
	movs r1, #2
	movs r0, #8
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r1, #4
	movs r0, #3
	bl 0x0200bd28
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #3
	bl 0x0200bd70
	movs r0, #30
	bl 0x0200bca8
	movs r1, #131
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r1, #0
	movs r0, #8
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r0, #0
	ldr r1, [pc, #304]
	movs r2, #0
	bl 0x0200bd80
	movs r0, #1
	ldr r1, [pc, #292]
	movs r2, #0
	bl 0x0200bd80
	movs r0, #3
	ldr r1, [pc, #284]
	movs r2, #0
	bl 0x0200bd80
	movs r2, #0
	ldr r1, [pc, #272]
	movs r0, #2
	bl 0x0200bd80
	movs r0, #60
	bl 0x0200bca8
	movs r1, #2
	movs r0, #2
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #2
	bl 0x0200bd70
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #8
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #129
	movs r2, #40
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r1, #0
	movs r0, #1
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #8
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r0, #8
	movs r1, #0
	bl 0x0200bd70
	movs r1, #2
	movs r0, #3
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #3
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #129
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r1, #0
	movs r0, #8
	bl 0x0200bd68
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200bd48
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200bd48
	movs r0, #0
	movs r1, #0
	bl 0x0200bcc0
	cmp r0, #0
	bne .L_02002c44_0
	ldr r0, [pc, #92]
	bl 0x0200bd60
	movs r0, #20
	bl 0x0200bca8
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200bd80
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200bcd0
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #1
	bl 0x0200be48
	movs r0, #10
	bl 0x0200bca8
	movs r0, #1
	movs r1, #0
	movs r2, #30
	bl 0x0200bd50
	movs r0, #1
	movs r1, #0
	bl 0x0200bd70
	b .L_02002c44_1
	.4byte 0x0000214f
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00000105
	.4byte 0x00000101
	.4byte 0x00002164
.L_02002c44_0:
	ldr r0, [pc, #1016]
	bl 0x0200bd60
	movs r0, #10
	bl 0x0200bca8
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcd0
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #1
	bl 0x0200be48
	movs r0, #10
	bl 0x0200bca8
	movs r2, #30
	movs r0, #1
	movs r1, #0
	bl 0x0200bd50
	movs r1, #3
	movs r0, #1
	bl 0x0200bd28
	movs r0, #20
	bl 0x0200bca8
	movs r0, #1
	movs r1, #0
	bl 0x0200bd70
.L_02002c44_1:
	ldr r5, [pc, #944]
	adds r0, r5, #0
	bl 0x0200bd60
	movs r0, #2
	ldr r1, [pc, #940]
	movs r2, #40
	bl 0x0200bd80
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200bcd0
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #2
	bl 0x0200be48
	movs r0, #10
	bl 0x0200bca8
	movs r2, #30
	movs r0, #2
	movs r1, #0
	bl 0x0200bd50
	movs r0, #2
	movs r1, #0
	bl 0x0200bd70
	movs r1, #3
	movs r0, #3
	bl 0x0200bd28
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #3
	bl 0x0200bd28
	movs r0, #20
	bl 0x0200bca8
	movs r0, #0
	movs r1, #3
	movs r2, #0
	bl 0x0200bd48
	movs r0, #1
	movs r1, #3
	movs r2, #0
	bl 0x0200bd48
	movs r2, #0
	movs r1, #3
	movs r0, #2
	bl 0x0200bd48
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #3
	bl 0x0200bd70
	movs r0, #20
	bl 0x0200bca8
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcd0
	movs r2, #16
	movs r1, #0
	movs r0, #1
	bl 0x0200be48
	movs r0, #30
	bl 0x0200bca8
	movs r1, #4
	movs r0, #1
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r2, #0
	movs r1, #0
	movs r0, #1
	bl 0x0200bd48
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #1
	bl 0x0200bd70
	movs r0, #30
	bl 0x0200bca8
	movs r2, #30
	movs r0, #3
	movs r1, #2
	bl 0x0200bd50
	movs r0, #2
	movs r1, #3
	bl 0x0200bd28
	movs r1, #3
	movs r0, #3
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcd0
	movs r1, #0
	movs r2, #16
	movs r0, #2
	bl 0x0200be48
	movs r0, #10
	bl 0x0200bca8
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #2
	adds r5, #7
	bl 0x0200bd78
	adds r0, r5, #0
	bl 0x0200bd60
	movs r0, #30
	bl 0x0200bca8
	movs r1, #128
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r1, #0
	movs r0, #8
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #2
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #2
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #132
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r1, #0
	movs r0, #8
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #2
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #8
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #3
	movs r0, #3
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #8
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r0, #1
	movs r1, #0
	movs r2, #50
	bl 0x0200bd50
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #8
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r2, #40
	movs r0, #3
	ldr r1, [pc, #416]
	bl 0x0200bd80
	movs r1, #0
	movs r0, #3
	bl 0x0200bd70
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #8
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r0, #1
	movs r1, #0
	movs r2, #50
	bl 0x0200bd50
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #8
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r0, #3
	movs r1, #2
	movs r2, #50
	bl 0x0200bd50
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd78
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200bd78
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #8
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #1
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #1
	bl 0x0200bd70
	movs r0, #30
	bl 0x0200bca8
	movs r1, #0
	movs r0, #8
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #2
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #2
	bl 0x0200bd70
	movs r0, #20
	bl 0x0200bca8
	movs r1, #132
	movs r2, #50
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r1, #0
	movs r0, #8
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r2, #40
	movs r0, #1
	ldr r1, [pc, #172]
	bl 0x0200bd80
	movs r1, #0
	movs r0, #1
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #129
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200bd80
	movs r0, #8
	movs r1, #0
	bl 0x0200bd70
	movs r1, #2
	movs r0, #8
	bl 0x0200bd40
	movs r0, #20
	bl 0x0200bca8
	movs r0, #10
	bl 0x0200bca8
	movs r1, #4
	movs r0, #3
	bl 0x0200bd28
	movs r0, #20
	bl 0x0200bca8
	movs r1, #0
	movs r0, #3
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r2, #40
	movs r0, #0
	movs r1, #3
	bl 0x0200bd48
	movs r1, #3
	movs r0, #0
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r0, #1
	movs r1, #3
	movs r2, #0
	bl 0x0200bd48
	movs r2, #0
	movs r1, #3
	movs r0, #2
	bl 0x0200bd48
	movs r0, #20
	bl 0x0200bca8
	movs r0, #1
	movs r1, #3
	bl 0x0200bd20
	movs r1, #3
	movs r0, #2
	bl 0x0200bd28
	movs r0, #30
	bl 0x0200bca8
	movs r0, #17
	b .L_02002c44_2
	.4byte 0x00002168
	.4byte 0x00002165
	.4byte 0x00000103
	.4byte 0x00000101
.L_02002c44_2:
	bl 0x0200be70
	movs r0, #1
	ldr r1, [pc, #184]
	ldr r2, [pc, #184]
	bl 0x0200bcd0
	movs r0, #2
	ldr r1, [pc, #172]
	ldr r2, [pc, #176]
	bl 0x0200bcd0
	movs r0, #3
	ldr r1, [pc, #164]
	ldr r2, [pc, #164]
	bl 0x0200bcd0
	movs r0, #1
	movs r1, #2
	bl 0x0200bd20
	movs r0, #0
	bl 0x0200bcc8
	cmp r0, #0
	beq .L_02002c44_3
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200bcf0
.L_02002c44_3:
	movs r0, #1
	bl 0x0200bd10
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200bd18
	movs r0, #2
	movs r1, #2
	bl 0x0200bd20
	movs r0, #0
	bl 0x0200bcc8
	cmp r0, #0
	beq .L_02002c44_4
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x0200bcf0
.L_02002c44_4:
	movs r0, #2
	bl 0x0200bd10
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200bd18
	movs r0, #3
	movs r1, #2
	bl 0x0200bd20
	movs r0, #0
	bl 0x0200bcc8
	cmp r0, #0
	beq .L_02002c44_5
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl 0x0200bcf0
.L_02002c44_5:
	movs r0, #3
	bl 0x0200bd10
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200bd18
	bl 0x0200bdf0
	bl 0x0200bcb8
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00013333
	.4byte 0x00009999
	.global Func_020034bc
	.thumb_func
Func_020034bc:
	push {r5, lr}
	bl 0x0200bcb0
	ldr r5, [pc, #80]
	adds r0, r5, #0
	bl 0x0200bd60
	movs r1, #0
	movs r0, #8
	bl 0x0200bd68
	movs r0, #0
	movs r1, #0
	bl 0x0200bcc0
	cmp r0, #0
	bne .L_020034bc_0
	movs r0, #20
	bl 0x0200bca8
	adds r0, r5, #1
	bl 0x0200bd60
	movs r0, #8
	movs r1, #0
	bl 0x0200bd70
	b .L_020034bc_1
.L_020034bc_0:
	movs r0, #20
	bl 0x0200bca8
	adds r0, r5, #2
	bl 0x0200bd60
	movs r0, #8
	movs r1, #0
	bl 0x0200bd70
.L_020034bc_1:
	bl 0x0200bcb8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000217f
	.global Func_02003518
	.thumb_func
Func_02003518:
	push {lr}
	movs r0, #150
	lsls r0, r0, #4
	bl 0x0200bc88
	cmp r0, #0
	beq .L_02003518_0
	ldr r0, [pc, #100]
	bl 0x0200bc88
	cmp r0, #0
	bne .L_02003518_0
	ldr r0, [pc, #92]
	bl 0x0200bc90
	bl 0x0200bcb0
	ldr r0, [pc, #88]
	bl 0x0200bd60
	movs r1, #0
	movs r0, #8
	bl 0x0200bd70
	movs r0, #10
	bl 0x0200bca8
	movs r1, #2
	movs r0, #0
	bl 0x0200bd40
	movs r0, #30
	bl 0x0200bca8
	movs r2, #0
	movs r1, #8
	movs r0, #0
	bl 0x0200bd48
	movs r0, #30
	bl 0x0200bca8
	movs r0, #8
	movs r1, #0
	bl 0x0200bd70
	movs r0, #0
	movs r1, #3
	bl 0x0200bd28
	movs r0, #20
	bl 0x0200bca8
	bl 0x0200bcb8
.L_02003518_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000962
	.4byte 0x00000961
	.4byte 0x0000217d
	.global Func_02003598
	.thumb_func
Func_02003598:
	push {lr}
	ldr r3, [pc, #68]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_02003598_0
	ldr r0, [pc, #56]
	b .L_02003598_1
.L_02003598_0:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02003598_2
	ldr r0, [pc, #56]
	b .L_02003598_1
.L_02003598_2:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02003598_3
	ldr r0, [pc, #52]
	b .L_02003598_1
.L_02003598_3:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02003598_4
	ldr r0, [pc, #52]
	b .L_02003598_1
.L_02003598_4:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02003598_5
	ldr r0, [pc, #48]
	b .L_02003598_1
.L_02003598_5:
	ldr r0, [pc, #48]
.L_02003598_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000093
	.4byte 0x0200c688
	.4byte 0x00000094
	.4byte 0x0200c724
	.4byte 0x00000095
	.4byte 0x0200c76c
	.4byte 0x00000096
	.4byte 0x0200c808
	.4byte 0x00000097
	.4byte 0x0200c850
	.4byte 0x0200c5e0
	.global Func_02003610
	.thumb_func
Func_02003610:
	push {r5, lr}
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02003610_0
	adds r2, r5, #0
	adds r2, #35
	movs r3, #0
	strb r3, [r2]
	movs r0, #0
	bl 0x0200bcc8
	ldr r3, [r0, #80]
	ldr r4, [r5, #80]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	ldrb r1, [r4, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r4, #9]
.L_02003610_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02003644
	.thumb_func
Func_02003644:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r2, [pc, #876]
	movs r0, #225
	lsls r0, r0, #1
	adds r0, r0, r2
	movs r1, #0
	ldrsh r3, [r0, r1]
	sub sp, #12
	mov r12, r0
	cmp r3, #0
	bne .L_02003644_0
	movs r3, #224
	lsls r3, r3, #1
	adds r0, r2, r3
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [pc, #848]
	ldrh r1, [r0]
	cmp r2, r3
	bne .L_02003644_1
	movs r3, #10
	mov r2, r12
	strh r3, [r2]
.L_02003644_1:
	lsls r3, r1, #16
	ldr r2, [pc, #836]
	asrs r3, r3, #16
	cmp r3, r2
	bne .L_02003644_2
	mov r1, r12
	movs r3, #20
	strh r3, [r1]
	ldrh r1, [r0]
.L_02003644_2:
	lsls r3, r1, #16
	ldr r2, [pc, #824]
	asrs r3, r3, #16
	cmp r3, r2
	bne .L_02003644_3
	movs r3, #30
	mov r2, r12
	strh r3, [r2]
	ldrh r1, [r0]
.L_02003644_3:
	lsls r3, r1, #16
	ldr r2, [pc, #808]
	asrs r3, r3, #16
	cmp r3, r2
	bne .L_02003644_4
	mov r1, r12
	movs r3, #40
	strh r3, [r1]
	ldrh r1, [r0]
.L_02003644_4:
	lsls r3, r1, #16
	ldr r2, [pc, #796]
	asrs r3, r3, #16
	cmp r3, r2
	bne .L_02003644_0
	movs r3, #50
	mov r2, r12
	strh r3, [r2]
.L_02003644_0:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bc90
	ldr r0, [pc, #776]
	bl 0x0200bc98
	ldr r6, [pc, #748]
	movs r0, #224
	lsls r0, r0, #1
	adds r3, r6, r0
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #764]
	cmp r2, r3
	bne .L_02003644_5
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #1
	bne .L_02003644_6
	ldr r0, [pc, #748]
	bl 0x0200bc88
	cmp r0, #0
	bne .L_02003644_7
	ldr r3, [pc, #740]
	strb r0, [r3]
.L_02003644_7:
	ldr r0, [pc, #724]
	bl 0x0200bc90
.L_02003644_6:
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_02003644_5
	ldr r0, [pc, #712]
	bl 0x0200bc88
	cmp r0, #0
	bne .L_02003644_8
	ldr r2, [pc, #708]
	movs r3, #5
	strb r3, [r2]
.L_02003644_8:
	ldr r0, [pc, #688]
	bl 0x0200bc90
.L_02003644_5:
	movs r0, #224
	lsls r0, r0, #1
	adds r3, r6, r0
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #652]
	cmp r2, r3
	bne .L_02003644_9
	ldr r0, [pc, #684]
	bl 0x0200bc88
	cmp r0, #0
	beq .L_02003644_10
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200bd18
	b .L_02003644_9
.L_02003644_10:
	movs r0, #8
	bl 0x0200bcc8
	ldr r1, [r0, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r3, [r0, #80]
	movs r2, #2
	adds r3, #38
	strb r2, [r3]
	movs r3, #128
	ldr r2, [r0, #80]
	lsls r3, r3, #7
	strh r3, [r2, #30]
.L_02003644_9:
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, [pc, #584]
	cmp r2, r3
	bne .L_02003644_11
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bc98
	movs r0, #8
	bl 0x020088c0
	movs r0, #9
	bl 0x020088c0
	movs r0, #10
	bl 0x020088c0
	ldr r0, [pc, #584]
	bl 0x0200bc88
	cmp r0, #0
	beq .L_02003644_12
	movs r0, #11
	movs r1, #5
	bl 0x0200bd20
	movs r3, #73
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #76
	movs r1, #16
	movs r2, #1
	movs r3, #1
	bl 0x0200bc48
	b .L_02003644_13
.L_02003644_12:
	movs r0, #11
	bl 0x0200bcc8
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
.L_02003644_13:
	movs r0, #11
	bl 0x0200bcc8
	movs r1, #0
	bl 0x0200bc60
	ldr r0, [pc, #520]
	bl 0x0200bc88
	cmp r0, #0
	beq .L_02003644_14
	movs r3, #32
	movs r2, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #30
	movs r1, #20
	movs r2, #1
	movs r3, #1
	bl 0x0200bc48
.L_02003644_14:
	ldr r6, [pc, #444]
.L_02003644_11:
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, [pc, #452]
	cmp r2, r3
	beq .L_02003644_15
	b .L_02003644_16
.L_02003644_15:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bc98
	movs r0, #8
	bl 0x020088c0
	movs r0, #9
	bl 0x020088c0
	movs r0, #10
	bl 0x020088c0
	movs r0, #8
	bl 0x0200bcc8
	ldr r5, [pc, #444]
	str r5, [r0, #108]
	movs r0, #9
	bl 0x0200bcc8
	str r5, [r0, #108]
	movs r0, #10
	bl 0x0200bcc8
	movs r1, #225
	str r5, [r0, #108]
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #52
	bne .L_02003644_17
	ldr r2, [pc, #412]
	add r0, sp, #8
	movs r3, #0
	str r3, [r0]
	ldr r1, [r2]
	ldr r3, [pc, #408]
	ldr r2, [pc, #408]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, [pc, #372]
	bl 0x0200bc88
	cmp r0, #0
	bne .L_02003644_17
	ldr r3, [pc, #396]
	movs r2, #4
	strb r0, [r3]
	strb r0, [r3, #1]
	strb r2, [r3, #2]
.L_02003644_17:
	ldr r5, [pc, #392]
	movs r3, #0
	ldrsb r3, [r5, r3]
	ldrb r2, [r5]
	cmp r3, #99
	bne .L_02003644_18
	movs r3, #30
	movs r2, #55
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #41
	movs r1, #55
	movs r2, #3
	movs r3, #2
	bl 0x0200bc50
	movs r3, #31
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #42
	movs r2, #1
	movs r1, #8
	movs r3, #1
	bl 0x0200bc48
	ldrb r2, [r5]
.L_02003644_18:
	movs r0, #128
	lsls r3, r2, #24
	lsls r0, r0, #18
	cmp r3, r0
	bne .L_02003644_19
	movs r0, #1
	ldrsb r0, [r5, r0]
	movs r1, #5
	lsls r0, r0, #16
	bl 0x0200bbe8
	movs r1, #128
	lsls r1, r1, #7
	adds r0, r0, r1
	bl 0x02008f6c
.L_02003644_19:
	movs r6, #0
	movs r7, #128
	mov r8, r6
	lsls r7, r7, #9
.L_02003644_20:
	adds r5, r6, #0
	adds r5, #11
	adds r0, r5, #0
	bl 0x0200bcc8
	adds r3, r0, #0
	mov r2, r8
	adds r3, #85
	strb r2, [r3]
	adds r3, #4
	strb r2, [r3]
	str r7, [r0, #24]
	str r7, [r0, #28]
	adds r0, r5, #0
	bl 0x0200bcc8
	adds r6, #1
	movs r1, #0
	bl 0x0200bc60
	adds r0, r5, #0
	adds r1, r6, #0
	bl 0x0200bd20
	cmp r6, #4
	ble .L_02003644_20
	movs r0, #11
	movs r1, #1
	bl 0x0200bd58
	movs r0, #12
	movs r1, #4
	bl 0x0200bd58
	movs r0, #13
	movs r1, #11
	bl 0x0200bd58
	movs r0, #14
	movs r1, #2
	bl 0x0200bd58
	movs r0, #15
	movs r1, #3
	bl 0x0200bd58
	movs r0, #16
	movs r1, #6
	bl 0x0200bd58
	movs r0, #17
	movs r1, #6
	bl 0x0200bd58
	movs r0, #18
	movs r1, #6
	bl 0x0200bd58
	movs r0, #19
	movs r1, #6
	bl 0x0200bd58
	movs r1, #6
	movs r0, #20
	bl 0x0200bd58
	movs r0, #16
	bl 0x0200bcc8
	ldr r2, [r0, #80]
	ldrb r3, [r2, #9]
	movs r5, #12
	orrs r3, r5
	strb r3, [r2, #9]
	movs r0, #20
	bl 0x0200bcc8
	ldr r2, [r0, #80]
	ldrb r3, [r2, #9]
	orrs r3, r5
	strb r3, [r2, #9]
	movs r0, #16
	bl 0x0200bcc8
	movs r5, #2
	adds r0, #35
	strb r5, [r0]
	movs r0, #20
	bl 0x0200bcc8
	adds r0, #35
	strb r5, [r0]
	movs r0, #16
	bl 0x0200bcc8
	movs r1, #0
	bl 0x0200bc60
	movs r0, #20
	bl 0x0200bcc8
	movs r1, #0
	bl 0x0200bc60
.L_02003644_16:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bc88
	cmp r0, #0
	beq .L_02003644_21
	bl 0x02008b30
	b .L_02003644_22
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000093
	.4byte 0x00000094
	.4byte 0x00000095
	.4byte 0x00000096
	.4byte 0x00000097
	.4byte 0x00000201
	.4byte 0x00000092
	.4byte 0x00000109
	.4byte 0x02001004
	.4byte 0x00000962
	.4byte 0x00000211
	.4byte 0x00000212
	.4byte 0x0200b611
	.4byte 0x0200bf6c
	.4byte 0x040000d4
	.4byte 0x85000003
	.4byte 0x02001000
	.4byte 0x02001001
.L_02003644_21:
	ldr r3, [pc, #36]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	movs r0, #228
	adds r2, r1, r3
	lsls r0, r0, #1
	adds r3, #68
	str r3, [r2]
	adds r2, r1, r0
	movs r3, #24
	str r3, [r2]
.L_02003644_22:
	movs r0, #0
	sub sp, #-12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.global Func_02003a30
	.thumb_func
Func_02003a30:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	movs r2, #64
	adds r2, r2, r6
	movs r7, #0
	ldrsb r7, [r2, r7]
	sub sp, #12
	mov r8, r2
	cmp r7, #0
	bne .L_02003a30_0
	ldr r3, [r6, #24]
	ldr r2, [r6, #20]
	mov r5, sp
	str r3, [r6, #8]
	str r3, [r5, #8]
	str r2, [r6, #4]
	str r2, [r5]
	bl 0x0200bc18
	adds r1, r0, #0
	movs r0, #240
	adds r2, r5, #0
	lsls r0, r0, #15
	bl 0x0200bc20
	ldr r3, [r5]
	str r3, [r6, #12]
	ldr r3, [r5, #8]
	str r3, [r6, #16]
	movs r3, #160
	lsls r3, r3, #11
	str r3, [r6, #36]
	str r3, [r6, #32]
	adds r3, r6, #0
	adds r3, #66
	strb r7, [r3]
	mov r2, r8
	ldrb r3, [r2]
	adds r3, #1
	strb r3, [r2]
	ldr r3, [pc, #72]
	ldr r3, [r3]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	bne .L_02003a30_1
	movs r0, #134
	bl 0x0200be70
	b .L_02003a30_1
.L_02003a30_0:
	cmp r7, #1
	bne .L_02003a30_2
	adds r0, r6, #0
	bl 0x0200bdf8
	cmp r0, #0
	bne .L_02003a30_1
	mov r2, r8
	ldrb r3, [r2]
	subs r3, #1
	strb r3, [r2]
	b .L_02003a30_1
.L_02003a30_2:
	cmp r7, #2
	bne .L_02003a30_1
	adds r0, r6, #0
	bl 0x0200bdf8
	cmp r0, #0
	bne .L_02003a30_1
	adds r0, r6, #0
	bl 0x0200be18
.L_02003a30_1:
	sub sp, #-12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001800
	.global Func_02003ad4
	.thumb_func
Func_02003ad4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #20
	bl 0x0200be28
	ldr r3, [pc, #244]
	ldr r3, [r3]
	ldr r0, [pc, #244]
	mov r8, r3
	bl 0x0200be60
	movs r3, #252
	add r6, sp, #8
	lsls r3, r3, #17
	str r3, [r6]
	movs r3, #192
	lsls r3, r3, #13
	str r3, [r6, #4]
	movs r3, #144
	lsls r3, r3, #16
	adds r0, r6, #0
	str r3, [r6, #8]
	bl 0x0200be20
	mov r5, r8
	adds r5, #88
	movs r7, #23
.L_02003ad4_0:
	movs r1, #142
	ldr r2, [r6]
	ldr r3, [r6, #8]
	adds r0, r5, #0
	lsls r1, r1, #1
	bl 0x0200be10
	adds r0, r5, #0
	ldr r1, [pc, #192]
	bl 0x0200be08
	adds r0, r5, #0
	movs r1, #7
	bl 0x0200be00
	bl 0x0200bc18
	lsls r1, r0, #3
	subs r1, r1, r0
	lsrs r1, r1, #16
	ldr r0, [r5]
	bl 0x0200bc80
	bl 0x0200bc18
	movs r1, #3
	bl 0x0200bbf8
	movs r3, #192
	lsls r3, r3, #9
	adds r0, r0, r3
	str r0, [r5, #44]
	str r0, [r5, #40]
	subs r7, #1
	movs r0, #1
	bl 0x0200bc00
	adds r5, #72
	cmp r7, #0
	bge .L_02003ad4_0
	movs r0, #80
	bl 0x0200bc00
	movs r3, #30
	movs r2, #55
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #41
	movs r1, #55
	movs r2, #3
	movs r3, #2
	bl 0x0200bc50
	movs r3, #31
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r1, #8
	movs r2, #1
	movs r0, #42
	bl 0x0200bc48
	movs r0, #50
	bl 0x0200bc00
	movs r0, #1
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #76]
	negs r0, r0
	bl 0x0200bc68
	movs r0, #30
	bl 0x0200bc00
	mov r2, r8
	movs r1, #2
	adds r2, #152
	movs r7, #23
.L_02003ad4_2:
	movs r3, #5
	ldrsb r3, [r2, r3]
	cmp r3, #0
	beq .L_02003ad4_1
	strb r1, [r2]
.L_02003ad4_1:
	subs r7, #1
	adds r2, #72
	cmp r7, #0
	bge .L_02003ad4_2
	bl 0x0200bc70
	bl 0x0200be68
	bl 0x0200be30
	sub sp, #-20
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001f30
	.4byte 0x00202108
	.4byte 0x0200ba31
	.4byte 0x0000e666
	.include "games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/IMPORT.INC"
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
	.4byte 0x0200c904
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000001c8
	.4byte 0xc00001f8
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00000210
	.4byte 0xffff0002
	.4byte 0x00000118
	.4byte 0x40000168
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00000210
	.4byte 0xffff0003
	.4byte 0x00000078
	.4byte 0xc00002b8
	.4byte 0x00000000
	.4byte 0x00f00228
	.4byte 0x000002d0
	.4byte 0xffff0004
	.4byte 0x00000078
	.4byte 0x40000258
	.4byte 0x00000000
	.4byte 0x00f00228
	.4byte 0x000002d0
	.4byte 0xffff000a
	.4byte 0x00000188
	.4byte 0x400001f8
	.4byte 0x00300000
	.4byte 0x01f00010
	.4byte 0x000002a0
	.4byte 0xffff000b
	.4byte 0x00000088
	.4byte 0x40000068
	.4byte 0x00300000
	.4byte 0x01f00010
	.4byte 0x000002a0
	.4byte 0xffff0014
	.4byte 0x000000e8
	.4byte 0x400000d8
	.4byte 0x00400000
	.4byte 0x02500030
	.4byte 0x00000240
	.4byte 0xffff0015
	.4byte 0x00000148
	.4byte 0x40000108
	.4byte 0x00400000
	.4byte 0x02500030
	.4byte 0x00000240
	.4byte 0xffff001e
	.4byte 0x000000c8
	.4byte 0x400001a8
	.4byte 0x00300000
	.4byte 0x02400050
	.4byte 0x000001e0
	.4byte 0xffff001f
	.4byte 0x00000208
	.4byte 0x40000148
	.4byte 0x00300000
	.4byte 0x02400050
	.4byte 0x000001e0
	.4byte 0xffff0028
	.4byte 0x000002a8
	.4byte 0x40000098
	.4byte 0x00000000
	.4byte 0x03000000
	.4byte 0x00000200
	.4byte 0xffff0029
	.4byte 0x00000028
	.4byte 0x400000f8
	.4byte 0x00000000
	.4byte 0x03000000
	.4byte 0x00000200
	.4byte 0xffff0032
	.4byte 0x00000128
	.4byte 0x40000188
	.4byte 0x00a00000
	.4byte 0x02400130
	.4byte 0x000002c0
	.4byte 0xffff0033
	.4byte 0x000001a8
	.4byte 0x40000188
	.4byte 0x00a00000
	.4byte 0x02400130
	.4byte 0x000002c0
	.4byte 0xffff0034
	.4byte 0x000001f8
	.4byte 0xc00000e8
	.4byte 0x01800000
	.4byte 0x02700040
	.4byte 0x000000f0
	.4byte 0xffff0035
	.4byte 0x000001f8
	.4byte 0x40000098
	.4byte 0x01800000
	.4byte 0x02700040
	.4byte 0x000000f0
	.4byte 0xffff0036
	.4byte 0x000000b8
	.4byte 0xc00000d8
	.4byte 0x00400000
	.4byte 0x01300040
	.4byte 0x000000f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000092
	.4byte 0x00118002
	.4byte 0x00203092
	.4byte 0x00302092
	.4byte 0x0040a093
	.4byte 0x00000093
	.4byte 0x00a04092
	.4byte 0x00b14094
	.4byte 0x00000094
	.4byte 0x0140b093
	.4byte 0x0151e095
	.4byte 0x00000095
	.4byte 0x01e15094
	.4byte 0x01f28096
	.4byte 0x00000096
	.4byte 0x0281f095
	.4byte 0x02932097
	.4byte 0x00000097
	.4byte 0x03229096
	.4byte 0x03334097
	.4byte 0x03433097
	.4byte 0x03536097
	.4byte 0x03635097
	.4byte 0x000001ff
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
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0041
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00024000
	.4byte 0xffff0034
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00024000
	.4byte 0xffff00a3
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x0002c000
	.4byte 0xffff00a3
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0129
	.4byte 0x00000007
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0x0071005d
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00006000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0129
	.4byte 0x00000007
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff0129
	.4byte 0x00000007
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01e00000
	.4byte 0x00024000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00080000
	.4byte 0x06d80000
	.4byte 0x00004000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00280000
	.4byte 0x06d80000
	.4byte 0x00004000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00380000
	.4byte 0x06d80000
	.4byte 0x00004000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00280000
	.4byte 0x06d80000
	.4byte 0x00004000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00080000
	.4byte 0x06d80000
	.4byte 0x00004000
	.4byte 0xffff0125
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00be0000
	.4byte 0x00004000
	.4byte 0xffff0125
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00ae0000
	.4byte 0x00004000
	.4byte 0xffff0125
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00ae0000
	.4byte 0x00004000
	.4byte 0xffff0125
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00ae0000
	.4byte 0x00004000
	.4byte 0xffff0125
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00be0000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00010000
	.4byte 0x00011999
	.4byte 0x00013333
	.4byte 0x00011999
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00100000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00200000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0xffff003c
	.4byte 0x02008ae9
	.4byte 0x00000002
	.4byte 0xffff003d
	.4byte 0x02008af5
	.4byte 0x00000002
	.4byte 0xffff003e
	.4byte 0x02008b01
	.4byte 0x00000002
	.4byte 0xffff003f
	.4byte 0x02008b0d
	.4byte 0x00000002
	.4byte 0xffff0040
	.4byte 0x02008b19
	.4byte 0x00000002
	.4byte 0xffff0041
	.4byte 0x02008b25
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008bc9
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008c2d
	.4byte 0x10009585
	.4byte 0xffff0000
	.4byte 0x02008c99
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000031
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x09600008
	.4byte 0x0200ac45
	.4byte 0x00000000
	.4byte 0x0f300008
	.4byte 0x0200b4bd
	.4byte 0x00000000
	.4byte 0x09620008
	.4byte 0x020093f9
	.4byte 0x00000002
	.2byte 0x0013
	.2byte 0x0961
	push	{r0, r3, r4, lr}
	lsls	r0, r0, #8
	ldrh	r5, [r2, #40]
	movs	r0, r0
	lsls	r0, r1, #16
	lsrs	r0, r4, #5
	add	r4, sp, #276
	lsls	r0, r0, #8
	ldrh	r5, [r2, #40]
	movs	r0, r0
	movs	r0, r1
	lsrs	r0, r6, #28
.L_020046e4:
	movs	r1, #130
	movs	r0, r0
	ldrh	r5, [r2, #40]
	movs	r0, r0
	lsls	r0, r1, #16
	lsrs	r2, r4, #5
	str	r3, [sp, #996]
	lsls	r0, r0, #8
	str	r0, [sp, #532]
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x8c2d
	lsls	r0, r0, #8
	str	r5, [sp, #532]
	asrs	r0, r0, #32
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x8c99
	lsls	r0, r0, #8
	movs	r3, r2
	movs	r0, r0
	lsls	r4, r4, #1
	lsrs	r5, r6, #27
	movs	r5, r0
	lsls	r0, r2, #1
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r1, r4
	movs	r0, r0
	movs	r4, r2
	.2byte 0xffff
	.2byte 0x0014
	movs	r0, r0
	movs	r1, r6
	movs	r0, r0
	movs	r5, r2
	.2byte 0xffff
	.2byte 0x0015
	movs	r0, r0
	str	r0, [sp, #532]
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x8c2d
	lsls	r0, r0, #8
	str	r5, [sp, #532]
	asrs	r0, r0, #32
.L_0200474c:
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x8c99
	lsls	r0, r0, #8
	movs	r3, r2
	movs	r0, r0
	lsls	r4, r4, #1
	lsrs	r4, r3, #30
	lsls	r6, r6, #2
	movs	r0, r2
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r1, r4
	movs	r0, r0
	movs	r6, r3
	.2byte 0xffff
	.2byte 0x001e
	movs	r0, r0
	movs	r1, r6
	movs	r0, r0
	movs	r7, r3
	.2byte 0xffff
	.2byte 0x001f
	movs	r0, r0
	.2byte 0x4602
	movs	r0, r0
	lsls	r0, r1, #1
	lsls	r1, r2, #8
	ldrh	r1, [r5, #46]
	lsls	r0, r0, #8
	strh	r2, [r0, #48]
	movs	r0, r0
.L_02004794:
	lsls	r1, r1, #1
	lsls	r1, r2, #8
	ldrh	r1, [r5, #46]
	lsls	r0, r0, #8
	stmia	r6!, {r1}
	movs	r0, r0
	lsls	r2, r1, #1
	lsls	r1, r2, #8
	ldrh	r1, [r5, #46]
	lsls	r0, r0, #8
	lsls	r2, r0, #24
	movs	r0, r0
	lsls	r3, r1, #1
	lsls	r1, r2, #8
	ldrh	r1, [r5, #46]
	lsls	r0, r0, #8
	lsls	r2, r0, #8
	movs	r0, r0
	lsls	r7, r0, #1
	.2byte 0xffff
	.2byte 0x8ead
	lsls	r0, r0, #8
	lsls	r2, r0, #24
	movs	r0, r0
	lsls	r5, r1, #1
	.2byte 0xffff
	.2byte 0x8d49
	lsls	r0, r0, #8
	ldrh	r5, [r2, #32]
	movs	r0, r0
	movs	r4, r1
	.2byte 0xffff
	.2byte 0x8cf9
	lsls	r0, r0, #8
	adds	r5, r2, r0
	asrs	r0, r0, #32
	movs	r3, r1
	lsls	r1, r2, #8
	ldrh	r1, [r3, #42]
	lsls	r0, r0, #8
	adds	r5, r2, r0
	movs	r0, r0
	movs	r3, r1
.L_020047ea:
	lsls	r1, r2, #8
	ldrh	r1, [r2, #44]
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r5, r1
	.2byte 0xffff
	.2byte 0x89dd
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r1, r4
	movs	r0, r0
	movs	r0, r5
	.2byte 0xffff
	.2byte 0x0028
	movs	r0, r0
	movs	r1, r6
	movs	r0, r0
	movs	r1, r5
	.2byte 0xffff
	.2byte 0x0029
	movs	r0, r0
	str	r0, [sp, #532]
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x8c2d
	lsls	r0, r0, #8
	str	r5, [sp, #532]
	asrs	r0, r0, #32
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x8c99
.L_02004836:
	lsls	r0, r0, #8
	movs	r3, r2
	movs	r0, r0
	lsls	r4, r4, #1
	lsrs	r5, r3, #30
	lsls	r0, r0, #3
	movs	r0, r2
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r1, r4
	movs	r0, r0
	movs	r2, r6
	.2byte 0xffff
	.2byte 0x0032
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r3, r6
	.2byte 0xffff
	.2byte 0x0033
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r4, r6
	.2byte 0xffff
	.2byte 0x0034
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r5, r6
	.2byte 0xffff
	.2byte 0x0035
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r6, r6
	.2byte 0xffff
	.2byte 0x0036
	movs	r0, r0
	lsls	r2, r0, #8
	movs	r0, r0
	lsls	r6, r0, #1
	.2byte 0xffff
	.2byte 0x8ead
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r0, r2
	.2byte 0xffff
	.2byte 0x909d
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r1, r2
	.2byte 0xffff
	.2byte 0x909d
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r2, r2
	.2byte 0xffff
	.2byte 0x909d
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
.L_020048c0:
	movs	r3, r2
	.2byte 0xffff
	.2byte 0x909d
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r4, r2
	.2byte 0xffff
	.2byte 0x909d
	lsls	r0, r0, #8
	movs	r3, r2
	movs	r0, r0
	lsls	r4, r4, #1
	lsrs	r0, r6, #28
	lsls	r5, r5, #3
	movs	r0, r2
	str	r0, [sp, #532]
	str	r0, [r0, r0]
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x8bc9
	lsls	r0, r0, #8
	str	r0, [sp, #532]
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x8c2d
.L_020048f6:
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
