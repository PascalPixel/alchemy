.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/ENTRY.INC"
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
	bl 0x0200b12c
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
	bl 0x0200b088
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
	bl 0x0200b030
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
	bl 0x0200aff8
	ldr r5, [pc, #132]
	movs r0, #15
	bl 0x0200afb8
	movs r0, #185
	bl 0x0200b118
	str r5, [r6, #48]
	str r5, [r6, #52]
	ldr r1, [r7]
.L_020000c4_5:
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl 0x0200b010
	mov r0, r8
	str r5, [r0, #48]
	str r5, [r0, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl 0x0200b010
	adds r0, r6, #0
	bl 0x0200b018
	bl 0x0200b110
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
	bl 0x0200aff8
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
	.4byte 0x0200b15c
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
	bl 0x0200b030
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
	.4byte 0x0200b15c
	.4byte 0xffff0000
	.4byte 0x0200b19c
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
	bl 0x0200b088
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
	.4byte 0x0200b19c
	.4byte 0x0200b1b4
	.4byte 0x0200b15c
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
	bl 0x0200b030
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
	.4byte 0x0200b1b4
	.4byte 0x0200b15c
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
	bl 0x0200b088
	ldrh r3, [r0, #6]
	ldr r0, [sp, #76]
	lsrs r3, r3, #12
	mov r8, r3
	bl 0x0200b088
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
	bl 0x0200b090
	movs r1, #8
	movs r0, #0
	bl 0x0200b0c0
	movs r0, #15
	bl 0x0200b068
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
	bl 0x0200b0a8
	movs r0, #0
	bl 0x0200b088
	ldr r3, [pc, #424]
	str r3, [r0, #108]
	movs r0, #4
	bl 0x0200b068
	mov r3, r8
	subs r3, #6
	cmp r3, #7
	bhi .L_02000608_6
	adds r0, r7, #0
	movs r1, #3
	bl 0x0200aff8
	b .L_02000608_7
.L_02000608_6:
	adds r0, r7, #0
	movs r1, #2
	bl 0x0200aff8
.L_02000608_7:
	movs r0, #239
	bl 0x0200b118
	adds r0, r7, #0
	ldr r1, [sp, #80]
	ldr r2, [sp, #84]
	ldr r3, [sp, #88]
	bl 0x0200b010
	movs r0, #0
	bl 0x0200b0b0
	movs r0, #0
	movs r1, #2
	bl 0x0200b0c0
	movs r0, #0
	ldr r1, [pc, #360]
	ldr r2, [pc, #344]
	bl 0x0200b090
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
	bl 0x0200b0a8
	ldr r3, [sp, #92]
	cmp r3, #0
	beq .L_02000608_8
	bl 0x0200b12c
.L_02000608_8:
	movs r0, #0
	bl 0x0200b0b0
	movs r1, #1
	movs r0, #0
	bl 0x0200b0c0
	movs r0, #0
	bl 0x0200b088
	movs r2, #0
	str r2, [r0, #108]
	adds r0, r7, #0
	bl 0x0200b018
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200b118
	movs r0, #213
	bl 0x0200b118
	ldr r3, [sp, #80]
	str r3, [r7, #8]
	ldr r3, [sp, #88]
	str r3, [r7, #16]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	adds r0, r7, #0
	movs r1, #1
	bl 0x0200aff8
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
	bl 0x0200b028
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
	bl 0x0200b028
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
	bl 0x0200b110
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
	.2byte 0xb1b4
	.2byte 0x0200
	.2byte 0x1999
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0x0001
	.2byte 0x82a9
	.2byte 0x0200
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0xb15c
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
	bl 0x0200b088
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
	bl 0x0200b028
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
	.2byte 0xb19c
	.2byte 0x0200
	.4byte 0x0200b1b4
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
	bl 0x0200b008
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
	bl 0x0200b038
	adds r0, r5, #0
	movs r1, #14
	bl 0x0200b0c8
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200b040
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
	bl 0x0200b008
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
	bl 0x0200b038
	adds r0, r5, #0
	movs r1, #15
	bl 0x0200b0c8
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
	bl 0x0200b088
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
	bl 0x0200b008
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
	bl 0x0200aff8
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl 0x0200b000
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
	bl 0x0200b0c8
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
	bl 0x0200afb0
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
	bl 0x0200afb0
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_02000c02_2:
	bl 0x0200afb0
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
	bl 0x0200aff8
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl 0x0200b000
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
	.4byte 0x0200b2d4
	.2byte 0x8ab1
	.2byte 0x0200
	.4byte 0xffff0000
	.global Func_02000cc0
	.thumb_func
Func_02000cc0:
	bx lr
	.2byte 0x0000
	.global Func_02000cc4
	.thumb_func
Func_02000cc4:
	push {lr}
	ldr r3, [pc, #68]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_02000cc4_0
	ldr r0, [pc, #56]
	b .L_02000cc4_1
.L_02000cc4_0:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02000cc4_2
	ldr r0, [pc, #56]
	b .L_02000cc4_1
.L_02000cc4_2:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02000cc4_3
	ldr r0, [pc, #52]
	b .L_02000cc4_1
.L_02000cc4_3:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02000cc4_4
	ldr r0, [pc, #52]
	b .L_02000cc4_1
.L_02000cc4_4:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02000cc4_5
	ldr r0, [pc, #48]
	b .L_02000cc4_1
.L_02000cc4_5:
	ldr r0, [pc, #48]
.L_02000cc4_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000071
	.4byte 0x0200b310
	.4byte 0x00000072
	.4byte 0x0200b358
	.4byte 0x0000007b
	.4byte 0x0200b3a0
	.4byte 0x0000007c
	.4byte 0x0200b400
	.4byte 0x0000007d
	.4byte 0x0200b448
	.4byte 0x0200b478
	.global Func_02000d3c
	.thumb_func
Func_02000d3c:
	movs r0, #0
	bx lr
	.global Func_02000d40
	.thumb_func
Func_02000d40:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200b508
	.global Func_02000d48
	.thumb_func
Func_02000d48:
	push {lr}
	ldr r3, [pc, #52]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_02000d48_0
	ldr r0, [pc, #40]
	b .L_02000d48_1
.L_02000d48_0:
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000d48_2
	ldr r0, [pc, #40]
	b .L_02000d48_1
.L_02000d48_2:
	ldr r3, [pc, #40]
	cmp r2, r3
	bgt .L_02000d48_3
	ldr r3, [pc, #36]
	cmp r2, r3
	blt .L_02000d48_3
	ldr r0, [pc, #36]
	b .L_02000d48_1
.L_02000d48_3:
	ldr r0, [pc, #36]
.L_02000d48_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000071
	.4byte 0x0200b610
	.4byte 0x0000007b
	.4byte 0x0200b718
	.4byte 0x00000086
	.4byte 0x0000007e
	.4byte 0x0200b850
	.4byte 0x0200b5f8
	.global Func_02000da4
	.thumb_func
Func_02000da4:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r5, [r6, #68]
	ldr r3, [r6, #8]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r2, [r6, #72]
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r7, [r6, #76]
	ldr r3, [r6, #16]
	adds r0, r5, #0
	adds r3, r3, r7
	movs r1, #18
	str r3, [r6, #16]
	bl 0x0200afb0
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_02000da4_0
	adds r3, #15
.L_02000da4_0:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r1, [r6, #80]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #30]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #30]
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000e00
	.thumb_func
Func_02000e00:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #68
	bl 0x0200b088
	ldr r3, [pc, #108]
	add r2, sp, #16
	str r3, [r2, #36]
	movs r3, #0
	mov r10, r0
	mov r9, r2
	mov r8, r3
	add r7, sp, #56
.L_02000e00_0:
	mov r2, r8
	lsls r5, r2, #12
	adds r0, r5, #0
	bl 0x0200afd0
	movs r3, #0
	str r3, [r7, #4]
	str r0, [r7]
	adds r0, r5, #0
	bl 0x0200afc8
	ldr r5, [r7]
	adds r6, r0, #0
	movs r1, #3
	adds r0, r5, #0
	str r6, [r7, #8]
	bl 0x0200afb0
	adds r5, r5, r0
	str r5, [r7]
	mov r3, r10
	ldr r2, [r3, #16]
	ldr r0, [r3, #8]
	ldr r1, [r3, #12]
	ldr r3, [r7, #4]
	str r3, [sp, #0]
	ldr r3, [pc, #44]
	str r3, [sp, #8]
	mov r3, r9
	str r3, [sp, #12]
	adds r3, r5, #0
	str r6, [sp, #4]
	bl 0x02008ae8
	movs r2, #2
	add r8, r2
	mov r3, r8
	cmp r3, #16
	bls .L_02000e00_0
	sub sp, #-68
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02008da5
	.4byte 0x01000001
	.global Func_02000e88
	.thumb_func
Func_02000e88:
	push {r5, lr}
	bl 0x0200b070
	ldr r5, [pc, #44]
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	movs r1, #5
	bl 0x0200b0e8
	ldr r3, [pc, #28]
	adds r5, r5, r3
	movs r3, #3
	strb r3, [r5]
	movs r0, #84
	movs r1, #5
	bl 0x0200b0e0
	bl 0x0200b078
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000022b
	.global Func_02000ec4
	.thumb_func
Func_02000ec4:
	push {lr}
	ldr r3, [pc, #68]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_02000ec4_0
	ldr r0, [pc, #56]
	b .L_02000ec4_1
.L_02000ec4_0:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02000ec4_2
	ldr r0, [pc, #56]
	b .L_02000ec4_1
.L_02000ec4_2:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02000ec4_3
	ldr r0, [pc, #52]
	b .L_02000ec4_1
.L_02000ec4_3:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02000ec4_4
	ldr r0, [pc, #52]
	b .L_02000ec4_1
.L_02000ec4_4:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02000ec4_5
	ldr r0, [pc, #48]
	b .L_02000ec4_1
.L_02000ec4_5:
	ldr r0, [pc, #48]
.L_02000ec4_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000071
	.4byte 0x0200b904
	.4byte 0x00000072
	.4byte 0x0200b8e0
	.4byte 0x0000007b
	.4byte 0x0200b9f4
	.4byte 0x0000007c
	.4byte 0x0200bd48
	.4byte 0x0000007d
	.4byte 0x0200bd6c
	.4byte 0x0200b880
	.global Func_02000f3c
	.thumb_func
Func_02000f3c:
	push {lr}
	bl 0x0200b070
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl 0x0200b0b8
	ldr r0, [pc, #28]
	bl 0x0200b060
	movs r0, #181
	movs r1, #3
	bl 0x0200b100
	movs r1, #0
	movs r0, #181
	bl 0x0200b080
	bl 0x0200b078
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000fd7
	.global Func_02000f70
	.thumb_func
Func_02000f70:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #612]
	movs r2, #224
	ldr r1, [r3]
	movs r3, #129
	lsls r2, r2, #1
	lsls r3, r3, #2
	ldr r6, [pc, #604]
	str r3, [r1, r2]
	adds r2, r2, r6
	movs r1, #0
	ldrsh r7, [r2, r1]
	ldr r3, [pc, #596]
	sub sp, #8
	mov r8, r2
	cmp r7, r3
	bne .L_02000f70_0
	bl 0x0200991c
	b .L_02000f70_1
.L_02000f70_0:
	ldr r3, [pc, #584]
	cmp r7, r3
	bne .L_02000f70_2
	ldr r0, [pc, #584]
	bl 0x0200b058
	cmp r0, #0
	bne .L_02000f70_3
	movs r3, #13
	str r3, [sp, #0]
	movs r5, #40
	movs r0, #0
	movs r1, #3
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200b028
	movs r3, #15
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #2
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200b028
	movs r1, #216
	movs r2, #162
	movs r0, #101
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200b108
.L_02000f70_3:
	mov r1, r8
	movs r2, #0
	ldrsh r3, [r1, r2]
	cmp r3, r7
	bne .L_02000f70_2
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #5
	beq .L_02000f70_4
	ldr r0, [pc, #500]
	bl 0x0200b058
	cmp r0, #0
	bne .L_02000f70_4
	b .L_02000f70_1
.L_02000f70_4:
	ldr r0, [pc, #488]
	bl 0x0200b060
	movs r3, #13
	movs r2, #30
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #1
	movs r2, #1
	movs r3, #1
	bl 0x0200b028
	movs r1, #216
	movs r2, #244
	movs r0, #100
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x0200b108
	b .L_02000f70_1
.L_02000f70_2:
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #440]
	cmp r2, r3
	beq .L_02000f70_5
	b .L_02000f70_6
.L_02000f70_5:
	bl 0x0200967c
	movs r0, #8
	bl 0x0200b088
	movs r3, #129
	lsls r3, r3, #16
	str r3, [r0, #56]
	movs r0, #9
	bl 0x020088c0
	movs r0, #10
	bl 0x020088c0
	movs r0, #144
	lsls r0, r0, #2
	bl 0x0200b058
	cmp r0, #0
	beq .L_02000f70_7
	movs r0, #11
	bl 0x0200b088
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000f70_8
	adds r2, r5, #0
	adds r2, #89
	movs r3, #0
	movs r1, #4
	strb r3, [r2]
	bl 0x0200aff8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200b038
.L_02000f70_8:
	movs r1, #152
	movs r2, #184
	movs r0, #0
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r3, #253
	bl 0x0200b048
.L_02000f70_7:
	ldr r0, [pc, #348]
	bl 0x0200b058
	cmp r0, #0
	beq .L_02000f70_9
	movs r0, #12
	bl 0x0200b088
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000f70_10
	adds r2, r5, #0
	adds r2, #89
	movs r3, #0
	movs r1, #4
	strb r3, [r2]
	bl 0x0200aff8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200b038
.L_02000f70_10:
	movs r1, #160
	movs r2, #184
	movs r0, #0
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #253
	bl 0x0200b048
.L_02000f70_9:
	ldr r0, [pc, #292]
	bl 0x0200b058
	cmp r0, #0
	beq .L_02000f70_11
	movs r0, #13
	bl 0x0200b088
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000f70_12
	adds r2, r5, #0
	adds r2, #89
	movs r3, #0
	movs r1, #4
	strb r3, [r2]
	bl 0x0200aff8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200b038
.L_02000f70_12:
	movs r1, #192
	movs r2, #168
	movs r0, #0
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #253
	bl 0x0200b048
.L_02000f70_11:
	ldr r0, [pc, #236]
	bl 0x0200b058
	cmp r0, #0
	beq .L_02000f70_13
	movs r0, #14
	bl 0x0200b088
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000f70_14
	adds r2, r5, #0
	adds r2, #89
	movs r3, #0
	movs r1, #4
	strb r3, [r2]
	bl 0x0200aff8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200b038
.L_02000f70_14:
	movs r1, #144
	movs r2, #160
	movs r0, #0
	lsls r1, r1, #16
	lsls r2, r2, #17
	movs r3, #253
	bl 0x0200b048
	movs r1, #188
	movs r2, #160
	movs r0, #0
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r3, #253
	bl 0x0200b048
.L_02000f70_13:
	ldr r0, [pc, #164]
	bl 0x0200b058
	cmp r0, #0
	bne .L_02000f70_1
	movs r0, #8
	bl 0x0200aed8
	b .L_02000f70_1
.L_02000f70_6:
	ldr r5, [pc, #148]
	cmp r2, r5
	bne .L_02000f70_15
	ldr r0, [pc, #148]
	bl 0x0200b058
	cmp r0, #0
	bne .L_02000f70_15
	movs r3, #37
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200b028
	movs r1, #150
	movs r2, #168
	movs r0, #100
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl 0x0200b108
.L_02000f70_15:
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	cmp r2, r5
	blt .L_02000f70_1
	ldr r3, [pc, #92]
	cmp r2, r3
	bgt .L_02000f70_1
	bl 0x02009214
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #5
	bne .L_02000f70_1
	bl 0x02009494
.L_02000f70_1:
	movs r0, #0
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000007b
	.4byte 0x0000007d
	.4byte 0x00000ef7
	.4byte 0x000008d1
	.4byte 0x00000071
	.4byte 0x00000241
	.4byte 0x00000242
	.4byte 0x00000243
	.4byte 0x00000fd7
	.4byte 0x0000007e
	.4byte 0x00000ef4
	.4byte 0x00000086
	.global Func_02001214
	.thumb_func
Func_02001214:
	push {r5, r6, lr}
	movs r0, #8
	sub sp, #8
	bl 0x0200b088
	ldr r3, [pc, #128]
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r6, r0, #0
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldr r2, [pc, #116]
	ldr r3, [pc, #120]
	subs r3, r3, r2
	adds r0, r0, r3
	bl 0x0200b058
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02001214_0
	movs r2, #168
	lsls r2, r2, #16
	ldr r1, [pc, #104]
	movs r0, #8
	bl 0x0200b0b8
	ldr r3, [pc, #100]
	movs r0, #8
	str r3, [r6, #12]
	bl 0x0200b088
	movs r1, #0
	bl 0x0200b038
	movs r0, #8
	movs r1, #3
	bl 0x0200b0d0
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r2, #10
	movs r3, #40
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #42
	movs r1, #10
	movs r2, #1
	movs r3, #1
	bl 0x0200b028
	b .L_02001214_1
.L_02001214_0:
	movs r0, #8
	bl 0x0200b088
	adds r0, #85
	strb r5, [r0]
.L_02001214_1:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000007e
	.4byte 0x000008d2
	.4byte 0x028a0000
	.4byte 0xffe00000
	.global Func_020012b4
	.thumb_func
Func_020012b4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #8
	sub sp, #8
	bl 0x0200b088
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	mov r8, r3
	cmp r3, #40
	bne .L_020012b4_0
	ldr r3, [pc, #148]
	movs r2, #224
	lsls r2, r2, #1
	adds r2, r2, r3
	movs r3, #0
	ldrsh r0, [r2, r3]
	mov r9, r2
	ldr r3, [pc, #136]
	ldr r2, [pc, #140]
	subs r2, r2, r3
	mov r10, r2
	add r0, r10
	bl 0x0200b058
	adds r7, r0, #0
	cmp r7, #0
	bne .L_020012b4_0
	adds r6, r5, #0
	movs r3, #3
	adds r6, #85
	strb r3, [r6]
	movs r0, #8
	bl 0x0200b068
	movs r0, #8
	bl 0x02008e00
	movs r0, #136
	bl 0x0200b118
	movs r0, #40
	bl 0x0200b068
	movs r0, #8
	bl 0x0200b088
	movs r1, #0
	bl 0x0200b038
	movs r0, #8
	movs r1, #3
	bl 0x0200b0d0
	strb r7, [r6]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	mov r2, r8
	movs r3, #10
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #42
	movs r2, #1
	movs r3, #1
	movs r1, #10
	bl 0x0200b028
	mov r2, r9
	movs r3, #0
	ldrsh r0, [r2, r3]
	add r0, r10
	bl 0x0200b060
.L_020012b4_0:
	sub sp, #-8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000007e
	.4byte 0x000008d2
	.global Func_02001374
	.thumb_func
Func_02001374:
	push {lr}
	bl 0x0200b070
	bl 0x020080c4
	bl 0x0200b078
	bl 0x020092b4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200138c
	.thumb_func
Func_0200138c:
	push {lr}
	movs r1, #37
	movs r2, #7
	ldr r0, [pc, #20]
	bl 0x0200b020
	movs r0, #183
	bl 0x0200b118
	movs r0, #4
	bl 0x0200b0d8
	pop {r0}
	bx r0
	.4byte 0x0200b2bc
	.global Func_020013ac
	.thumb_func
Func_020013ac:
	push {r5, r6, r7, lr}
	ldr r6, [pc, #204]
	movs r2, #224
	lsls r2, r2, #1
	adds r5, r6, r2
	movs r3, #0
	ldrsh r0, [r5, r3]
	ldr r7, [pc, #196]
	ldr r3, [pc, #196]
	subs r3, r3, r7
	adds r0, r0, r3
	bl 0x0200b058
	cmp r0, #0
	bne .L_020013ac_0
	bl 0x0200b070
	movs r2, #0
	ldrsh r0, [r5, r2]
	movs r1, #5
	bl 0x0200b0e8
	ldr r3, [pc, #172]
	adds r2, r6, r3
	movs r3, #3
	strb r3, [r2]
	movs r2, #0
	ldrsh r3, [r5, r2]
	subs r3, r3, r7
	cmp r3, #8
	bhi .L_020013ac_1
	ldr r2, [pc, #160]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	str r4, [sp, #96]
	lsls r0, r0, #8
	str r4, [sp, #112]
	lsls r0, r0, #8
	str r4, [sp, #128]
	lsls r0, r0, #8
	str r4, [sp, #144]
	lsls r0, r0, #8
	str r4, [sp, #160]
	lsls r0, r0, #8
	str r4, [sp, #200]
	lsls r0, r0, #8
	str r4, [sp, #240]
	lsls r0, r0, #8
	str r4, [sp, #280]
	lsls r0, r0, #8
	str r4, [sp, #320]
	lsls r0, r0, #8
	movs r0, #63
	b .L_020013ac_2
	.2byte 0x203f
	.2byte 0xe009
	.2byte 0x203f
	.2byte 0xe00c
	.2byte 0x203f
	.2byte 0xe00f
	.2byte 0x2054
.L_020013ac_2:
	movs r1, #0
	bl 0x0200b0e0
	b .L_020013ac_1
	.2byte 0x2054
	.2byte 0x2101
	.2byte 0xf001
	.2byte 0xfe53
	.2byte 0xe00d
	.2byte 0x2054
	.2byte 0x2102
	.2byte 0xf001
	.2byte 0xfe4e
	.2byte 0xe008
	.2byte 0x2054
	.2byte 0x2103
	.2byte 0xf001
	.2byte 0xfe49
	.2byte 0xe003
	.2byte 0x2054
	.2byte 0x2104
	.2byte 0xf001
	.2byte 0xfe44
.L_020013ac_1:
	bl 0x0200b078
	b .L_020013ac_3
.L_020013ac_0:
	ldr r0, [pc, #48]
	movs r1, #44
	movs r2, #7
	bl 0x0200b020
	movs r0, #183
	bl 0x0200b118
	movs r0, #3
	bl 0x0200b0d8
.L_020013ac_3:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000007e
	.4byte 0x000008c8
	.4byte 0x0000022b
	.4byte 0x020093f4
	.4byte 0x0200b2bc
	.global Func_02001494
	.thumb_func
Func_02001494:
	push {lr}
	bl 0x0200b070
	ldr r1, [pc, #84]
	movs r0, #0
	ldr r2, [pc, #84]
	bl 0x0200b090
	bl 0x0200b0f0
	bl 0x0200b0f8
	ldr r3, [pc, #72]
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldr r2, [pc, #64]
	ldr r3, [pc, #68]
	subs r3, r3, r2
	adds r0, r0, r3
	bl 0x0200b060
	movs r0, #30
	bl 0x0200b068
	ldr r0, [pc, #56]
	movs r1, #44
	movs r2, #7
	bl 0x0200b020
	movs r2, #16
	movs r1, #3
	negs r2, r2
	movs r0, #0
	bl 0x0200b0a0
	movs r0, #3
	bl 0x0200b0d8
	bl 0x0200b078
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00006666
	.4byte 0x00003333
	.4byte 0x02000240
	.4byte 0x0000007e
	.4byte 0x000008c8
	.4byte 0x0200b2bc
	.global Func_02001508
	.thumb_func
Func_02001508:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #11
	bl 0x0200b088
	cmp r0, #0
	beq .L_02001508_0
	adds r2, r0, #0
	adds r2, #89
	movs r3, #0
	strb r3, [r2]
.L_02001508_0:
	adds r0, r5, #0
	bl 0x0200b088
	movs r1, #0
	bl 0x0200b038
	movs r1, #152
	movs r2, #184
	movs r0, #0
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r3, #253
	bl 0x0200b048
	movs r0, #144
	lsls r0, r0, #2
	bl 0x0200b060
	pop {r5}
	pop {r0}
	bx r0
	.global Func_02001548
	.thumb_func
Func_02001548:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #12
	bl 0x0200b088
	cmp r0, #0
	beq .L_02001548_0
	adds r2, r0, #0
	adds r2, #89
	movs r3, #0
	strb r3, [r2]
.L_02001548_0:
	adds r0, r5, #0
	bl 0x0200b088
	movs r1, #0
	bl 0x0200b038
	movs r1, #160
	movs r2, #184
	movs r0, #0
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #253
	bl 0x0200b048
	ldr r0, [pc, #12]
	bl 0x0200b060
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000241
	.global Func_0200158c
	.thumb_func
Func_0200158c:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #13
	bl 0x0200b088
	cmp r0, #0
	beq .L_0200158c_0
	adds r2, r0, #0
	adds r2, #89
	movs r3, #0
	strb r3, [r2]
.L_0200158c_0:
	adds r0, r5, #0
	bl 0x0200b088
	movs r1, #0
	bl 0x0200b038
	movs r1, #192
	movs r2, #168
	movs r0, #0
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #253
	bl 0x0200b048
	ldr r0, [pc, #12]
	bl 0x0200b060
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000242
	.global Func_020015d0
	.thumb_func
Func_020015d0:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #14
	bl 0x0200b088
	cmp r0, #0
	beq .L_020015d0_0
	adds r2, r0, #0
	adds r2, #89
	movs r3, #0
	strb r3, [r2]
.L_020015d0_0:
	adds r0, r5, #0
	bl 0x0200b088
	movs r1, #0
	bl 0x0200b038
	movs r1, #144
	movs r2, #160
	movs r0, #0
	lsls r1, r1, #16
	lsls r2, r2, #17
	movs r3, #253
	bl 0x0200b048
	movs r1, #188
	movs r2, #160
	movs r0, #0
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r3, #253
	bl 0x0200b048
	ldr r0, [pc, #12]
	bl 0x0200b060
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000243
	.global Func_02001624
	.thumb_func
Func_02001624:
	push {lr}
	ldr r0, [pc, #32]
	sub sp, #8
	bl 0x0200b060
	movs r3, #8
	movs r2, #21
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200b028
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x000008c4
	.global Func_0200164c
	.thumb_func
Func_0200164c:
	push {lr}
	ldr r0, [pc, #8]
	bl 0x0200b060
	pop {r0}
	bx r0
	.4byte 0x000008c5
	.global Func_0200165c
	.thumb_func
Func_0200165c:
	push {lr}
	ldr r0, [pc, #8]
	bl 0x0200b060
	pop {r0}
	bx r0
	.4byte 0x000008c6
	.global Func_0200166c
	.thumb_func
Func_0200166c:
	push {lr}
	ldr r0, [pc, #8]
	bl 0x0200b060
	pop {r0}
	bx r0
	.4byte 0x000008c7
	.global Func_0200167c
	.thumb_func
Func_0200167c:
	push {lr}
	ldr r0, [pc, #172]
	sub sp, #8
	bl 0x0200b058
	cmp r0, #0
	beq .L_0200167c_0
	movs r3, #8
	movs r2, #21
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200b028
	movs r1, #242
	movs r2, #242
	movs r0, #15
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl 0x0200b0b8
	b .L_0200167c_1
.L_0200167c_0:
	movs r0, #15
	bl 0x0200b088
	ldr r3, [pc, #120]
	str r3, [r0, #28]
.L_0200167c_1:
	ldr r0, [pc, #120]
	bl 0x0200b058
	cmp r0, #0
	beq .L_0200167c_2
	movs r1, #242
	movs r2, #242
	movs r0, #16
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl 0x0200b0b8
	b .L_0200167c_3
.L_0200167c_2:
	movs r0, #16
	bl 0x0200b088
	ldr r3, [pc, #84]
	str r3, [r0, #28]
.L_0200167c_3:
	ldr r0, [pc, #88]
	bl 0x0200b058
	cmp r0, #0
	beq .L_0200167c_4
	movs r1, #242
	movs r2, #242
	movs r0, #17
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl 0x0200b0b8
	b .L_0200167c_5
.L_0200167c_4:
	movs r0, #17
	bl 0x0200b088
	ldr r3, [pc, #48]
	str r3, [r0, #28]
.L_0200167c_5:
	ldr r0, [pc, #56]
	bl 0x0200b058
	cmp r0, #0
	beq .L_0200167c_6
	movs r1, #242
	movs r2, #242
	movs r0, #18
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl 0x0200b0b8
	b .L_0200167c_7
.L_0200167c_6:
	movs r0, #18
	bl 0x0200b088
	ldr r3, [pc, #12]
	str r3, [r0, #28]
.L_0200167c_7:
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000008c4
	.4byte 0x00019999
	.4byte 0x000008c5
	.4byte 0x000008c6
	.4byte 0x000008c7
	.global Func_02001740
	.thumb_func
Func_02001740:
	push {r5, lr}
	sub sp, #32
	bl 0x0200b070
	add r5, sp, #8
	adds r0, r5, #0
	bl 0x02008474
	cmp r0, #0
	beq .L_02001740_0
	mov r2, sp
	add r3, sp, #24
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [r5]
	ldr r1, [r5, #4]
	ldr r2, [r5, #8]
	ldr r3, [r5, #12]
	bl 0x02008608
.L_02001740_0:
	bl 0x0200b078
	sub sp, #-32
	pop {r5}
	pop {r0}
	bx r0
	.global Func_02001774
	.thumb_func
Func_02001774:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	str r2, [sp, #0]
	ldr r3, [pc, #204]
	movs r2, #250
	str r1, [sp, #4]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl 0x0200b088
	adds r6, r0, #0
	adds r0, r5, #0
	bl 0x0200b088
	adds r7, r0, #0
	bl 0x0200b070
	ldr r3, [sp, #4]
	lsls r3, r3, #16
	mov r11, r3
	ldr r3, [r6, #8]
	ldr r2, [pc, #168]
	add r3, r11
	movs r5, #128
	lsls r5, r5, #12
	ands r3, r2
	adds r1, r3, r5
	ldr r3, [sp, #0]
	lsls r3, r3, #16
	mov r9, r3
	ldr r3, [r6, #16]
	add r3, r9
	mov r10, r2
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #9
	str r2, [r6, #48]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r5
	mov r8, r2
	str r2, [r6, #52]
	adds r0, r6, #0
	ldr r2, [r6, #12]
	bl 0x0200b010
	adds r0, r6, #0
	movs r1, #27
	bl 0x0200aff8
	ldr r3, [r7, #8]
	mov r2, r10
	add r3, r11
	ands r3, r2
	adds r1, r3, r5
	ldr r3, [r7, #16]
	add r3, r9
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #9
	str r2, [r7, #48]
	mov r2, r8
	adds r3, r3, r5
	str r2, [r7, #52]
	adds r0, r7, #0
	ldr r2, [r7, #12]
	bl 0x0200b010
	ldr r3, [sp, #4]
	cmp r3, #0
	blt .L_02001774_0
	ldr r2, [sp, #0]
	cmp r2, #0
	bge .L_02001774_1
.L_02001774_0:
	adds r0, r7, #0
	movs r1, #4
	bl 0x0200aff8
	b .L_02001774_2
.L_02001774_1:
	adds r0, r7, #0
	movs r1, #3
	bl 0x0200aff8
.L_02001774_2:
	movs r0, #226
	bl 0x0200b118
	adds r0, r6, #0
	bl 0x0200b018
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200b118
	bl 0x0200b078
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0xfff00000
	.global Func_0200185c
	.thumb_func
Func_0200185c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r6, r0, #0
	adds r7, r1, #0
	mov r8, r2
	bl 0x0200b088
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200185c_0
	adds r0, r6, #0
	movs r1, #3
	bl 0x0200b0d0
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	ldr r3, [r5, #8]
	ldr r2, [r5, #16]
	asrs r3, r3, #20
	asrs r2, r2, #20
	subs r3, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r0, r7, #0
	mov r1, r8
	movs r2, #3
	movs r3, #1
	bl 0x0200b028
.L_0200185c_0:
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020018b0
	.thumb_func
Func_020018b0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r6, r0, #0
	adds r7, r1, #0
	mov r8, r2
	bl 0x0200b088
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020018b0_0
	adds r0, r6, #0
	movs r1, #3
	bl 0x0200b0d0
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	ldr r3, [r5, #16]
	ldr r2, [r5, #8]
	asrs r3, r3, #20
	asrs r2, r2, #20
	subs r3, #1
	str r2, [sp, #0]
	str r3, [sp, #4]
	adds r0, r7, #0
	mov r1, r8
	movs r2, #1
	movs r3, #3
	bl 0x0200b028
.L_020018b0_0:
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001904
	.thumb_func
Func_02001904:
	push {r5, lr}
	adds r5, r0, #0
	movs r1, #1
	bl 0x0200b0c0
	adds r0, r5, #0
	movs r1, #2
	bl 0x0200b0c0
	pop {r5}
	pop {r0}
.L_0200191a:
	bx r0
	.global Func_0200191c
	.thumb_func
Func_0200191c:
	push {lr}
	movs r0, #8
	movs r1, #17
	movs r2, #30
	movs r3, #21
	bl 0x0200985c
	movs r0, #10
	movs r1, #17
	movs r2, #31
	movs r3, #22
	bl 0x0200985c
	movs r0, #11
	movs r1, #20
	movs r2, #30
	movs r3, #23
	bl 0x020098b0
	movs r0, #12
	movs r1, #21
	movs r2, #30
	movs r3, #24
.L_0200194a:
	bl 0x020098b0
	movs r0, #13
	movs r1, #22
	movs r2, #30
	movs r3, #25
	bl 0x020098b0
	movs r0, #15
	movs r1, #23
	movs r2, #30
	movs r3, #26
	bl 0x020098b0
	movs r0, #17
	movs r1, #0
	movs r2, #30
	movs r3, #31
	bl 0x0200985c
	movs r0, #18
	movs r1, #0
	movs r2, #31
	movs r3, #32
	bl 0x0200985c
	movs r0, #9
	movs r1, #0
	movs r2, #32
	movs r3, #33
	bl 0x0200985c
	movs r0, #19
	movs r1, #4
	movs r2, #30
	movs r3, #34
	bl 0x020098b0
	movs r0, #14
	movs r1, #5
	movs r2, #30
	movs r3, #35
	bl 0x020098b0
	movs r0, #16
	movs r1, #6
	movs r2, #30
	movs r3, #36
	bl 0x020098b0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020019b4
	.thumb_func
Func_020019b4:
	push {lr}
	movs r0, #8
	bl 0x02009904
	pop {r0}
	bx r0
	.global Func_020019c0
	.thumb_func
Func_020019c0:
	push {lr}
	movs r0, #10
	bl 0x02009904
	pop {r0}
	bx r0
	.global Func_020019cc
	.thumb_func
Func_020019cc:
	push {lr}
	movs r0, #11
	bl 0x02009904
	pop {r0}
	bx r0
	.global Func_020019d8
	.thumb_func
Func_020019d8:
	push {lr}
	movs r0, #12
	bl 0x02009904
	pop {r0}
	bx r0
	.global Func_020019e4
	.thumb_func
Func_020019e4:
	push {lr}
	movs r0, #13
	bl 0x02009904
	pop {r0}
	bx r0
	.global Func_020019f0
	.thumb_func
Func_020019f0:
	push {lr}
	movs r0, #15
	bl 0x02009904
	pop {r0}
	bx r0
	.global Func_020019fc
	.thumb_func
Func_020019fc:
	push {lr}
	movs r0, #17
	bl 0x02009904
	pop {r0}
	bx r0
	.global Func_02001a08
	.thumb_func
Func_02001a08:
	push {lr}
	movs r0, #18
	bl 0x02009904
	pop {r0}
	bx r0
	.global Func_02001a14
	.thumb_func
Func_02001a14:
	push {lr}
	movs r0, #9
	bl 0x02009904
	pop {r0}
	bx r0
	.global Func_02001a20
	.thumb_func
Func_02001a20:
	push {lr}
	movs r0, #19
	bl 0x02009904
	pop {r0}
	bx r0
	.global Func_02001a2c
	.thumb_func
Func_02001a2c:
	push {lr}
	movs r0, #14
	bl 0x02009904
	pop {r0}
	bx r0
	.global Func_02001a38
	.thumb_func
Func_02001a38:
	push {lr}
	movs r0, #16
	bl 0x02009904
	pop {r0}
	bx r0
	.global Func_02001a44
	.thumb_func
Func_02001a44:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #85
	ldrb r3, [r7]
	ldr r1, [pc, #188]
	mov r8, r3
	ldr r3, [r5, #8]
	movs r2, #128
	lsls r2, r2, #12
	sub sp, #12
	ands r3, r1
	mov r6, sp
	adds r3, r3, r2
	str r3, [r6]
	ldr r3, [r5, #12]
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [r6, #8]
	ldrh r1, [r5, #6]
	movs r3, #128
	lsls r3, r3, #6
	adds r1, r1, r3
	movs r3, #192
	lsls r3, r3, #8
	movs r0, #128
	ands r1, r3
	lsls r0, r0, #14
	adds r2, r6, #0
	bl 0x0200afd8
	adds r0, r5, #0
	adds r1, r6, #0
	bl 0x0200b030
	cmp r0, #0
	bne .L_02001a44_0
	bl 0x0200b070
	movs r1, #6
	adds r0, r5, #0
	bl 0x0200aff8
	movs r0, #6
	bl 0x0200afb8
	movs r0, #152
	bl 0x0200b118
	adds r0, r5, #0
	movs r1, #7
	bl 0x0200aff8
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
	bl 0x0200b038
	movs r3, #10
	ldrsh r2, [r6, r3]
	movs r3, #2
	ldrsh r1, [r6, r3]
	movs r0, #0
	bl 0x0200b098
	adds r0, r5, #0
	movs r1, #6
	bl 0x0200aff8
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200b038
	mov r3, r8
	strb r3, [r7]
	bl 0x0200b078
	movs r0, #1
	b .L_02001a44_1
.L_02001a44_0:
	movs r0, #0
.L_02001a44_1:
	sub sp, #-12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0xfff00000
	.global Func_02001b14
	.thumb_func
Func_02001b14:
	push {lr}
	movs r0, #0
	sub sp, #12
	bl 0x0200b088
	ldr r3, [r0, #8]
	mov r1, sp
	str r3, [r1]
	ldr r3, [r0, #12]
	str r3, [r1, #4]
	ldr r2, [pc, #16]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r1, #8]
	bl 0x02009a44
	sub sp, #-12
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0xffe00000
	.global Func_02001b40
	.thumb_func
Func_02001b40:
	push {lr}
	movs r0, #0
	sub sp, #12
	bl 0x0200b088
	ldr r3, [r0, #8]
	mov r1, sp
	str r3, [r1]
	ldr r3, [r0, #12]
	str r3, [r1, #4]
	movs r2, #128
	ldr r3, [r0, #16]
	lsls r2, r2, #14
	adds r3, r3, r2
	str r3, [r1, #8]
	bl 0x02009a44
	sub sp, #-12
	pop {r1}
	bx r1
	.global Func_02001b68
	.thumb_func
Func_02001b68:
	push {lr}
	movs r0, #0
	sub sp, #12
	bl 0x0200b088
	ldr r2, [pc, #28]
	ldr r3, [r0, #8]
	mov r1, sp
	adds r3, r3, r2
	str r3, [r1]
	ldr r3, [r0, #12]
	str r3, [r1, #4]
	ldr r3, [r0, #16]
	str r3, [r1, #8]
	bl 0x02009a44
	sub sp, #-12
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0xffe00000
	.global Func_02001b94
	.thumb_func
Func_02001b94:
	push {lr}
	movs r0, #0
	sub sp, #12
	bl 0x0200b088
	movs r2, #128
	ldr r3, [r0, #8]
	lsls r2, r2, #14
	mov r1, sp
	adds r3, r3, r2
	str r3, [r1]
	ldr r3, [r0, #12]
	str r3, [r1, #4]
	ldr r3, [r0, #16]
	str r3, [r1, #8]
	bl 0x02009a44
	sub sp, #-12
	pop {r1}
	bx r1
	.global Func_02001bbc
	.thumb_func
Func_02001bbc:
	push {r5, r6, r7, lr}
	movs r0, #8
	sub sp, #8
.L_02001bc2:
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #8
	asrs r7, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #12
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #15
	asrs r5, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r6, #19
	bne .L_02001bc2_0
	cmp r5, #24
	bne .L_02001bc2_1
	movs r2, #80
	b .L_02001bc2_2
.L_02001bc2_1:
	cmp r3, #24
	bne .L_02001bc2_3
	movs r2, #112
	negs r2, r2
	movs r0, #8
	movs r1, #0
	bl 0x02009774
	movs r2, #32
	b .L_02001bc2_2
.L_02001bc2_3:
	movs r2, #80
	negs r2, r2
	movs r0, #8
	movs r1, #0
	bl 0x02009774
	movs r2, #112
	b .L_02001bc2_2
.L_02001bc2_0:
	cmp r6, #14
	bne .L_02001bc2_4
	cmp r5, #24
	beq .L_02001bc2_5
	cmp r3, #24
	bne .L_02001bc2_6
	movs r2, #64
	b .L_02001bc2_2
.L_02001bc2_6:
	movs r2, #112
	b .L_02001bc2_2
.L_02001bc2_4:
	cmp r6, #10
	bne .L_02001bc2_7
	cmp r3, #24
	beq .L_02001bc2_5
	movs r2, #48
.L_02001bc2_2:
	negs r2, r2
	movs r0, #8
	movs r1, #0
	bl 0x02009774
	b .L_02001bc2_8
.L_02001bc2_7:
	bl 0x02009b14
	b .L_02001bc2_5
.L_02001bc2_8:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #8
	bl 0x0200b088
	ldr r3, [r0, #16]
	subs r5, r7, #1
	asrs r3, r3, #20
	str r3, [sp, #4]
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200b028
.L_02001bc2_5:
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001c84
	.thumb_func
Func_02001c84:
	push {r5, r6, lr}
	movs r0, #8
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #8
	asrs r5, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #12
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r6, #7
	bne .L_02001c84_0
	cmp r3, #24
	bne .L_02001c84_1
	movs r0, #8
	movs r1, #0
	movs r2, #48
	bl 0x02009774
	b .L_02001c84_2
.L_02001c84_1:
	movs r0, #8
	movs r1, #0
	movs r2, #80
	bl 0x02009774
	movs r0, #8
	movs r1, #0
	movs r2, #112
	bl 0x02009774
	b .L_02001c84_2
.L_02001c84_0:
	cmp r6, #10
	bne .L_02001c84_3
	cmp r3, #24
	beq .L_02001c84_4
	movs r0, #8
	movs r1, #0
	movs r2, #144
	bl 0x02009774
	b .L_02001c84_2
.L_02001c84_3:
	cmp r6, #14
	bne .L_02001c84_4
	movs r0, #8
	movs r1, #0
	movs r2, #80
	bl 0x02009774
.L_02001c84_2:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #8
	bl 0x0200b088
	ldr r3, [r0, #16]
	subs r5, #1
	asrs r3, r3, #20
	str r3, [sp, #4]
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200b028
.L_02001c84_4:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02001d2c
	.thumb_func
Func_02001d2c:
	push {r5, r6, r7, lr}
	movs r0, #10
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #10
	asrs r7, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #13
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #15
	asrs r5, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r6, #18
	bne 0x02009d82
	subs r3, #31
	cmp r3, #2
	bhi .L_02001d2c_0
	movs r2, #128
	b 0x02009d96
.L_02001d2c_0:
	adds r3, r5, #0
	subs r3, #31
	cmp r3, #2
	bhi 0x02009d72
.L_02001d6e:
	movs r2, #128
	b .L_02001d6e_0
	.2byte 0x2270
	.2byte 0x4252
	.2byte 0x200a
	.2byte 0x2100
	.2byte 0xf7ff
	.2byte 0xfcfb
	.2byte 0x2240
	.2byte 0xe009
	.2byte 0x2e0a
	.2byte 0xd10d
	.2byte 0x3b1f
	.2byte 0x2b02
	.2byte 0xd925
	.2byte 0x1c2b
	.2byte 0x3b1f
	.2byte 0x2b02
	.2byte 0xd921
	.2byte 0x2230
.L_02001d6e_0:
	negs r2, r2
	movs r0, #10
	movs r1, #0
	bl 0x02009774
	b .L_02001d6e_1
	.2byte 0x2e07
	.2byte 0xd018
.L_02001d6e_1:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #10
	bl 0x0200b088
	ldr r3, [r0, #16]
	subs r5, r7, #1
	asrs r3, r3, #20
	str r3, [sp, #4]
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200b028
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02001de0
	.thumb_func
Func_02001de0:
	push {r5, r6, lr}
	movs r0, #10
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #10
	asrs r5, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	asrs r6, r3, #20
	cmp r6, #18
	beq .L_02001de0_0
	cmp r6, #10
	bne .L_02001de0_1
	movs r0, #10
	movs r1, #0
	movs r2, #128
	bl 0x02009774
	b .L_02001de0_2
.L_02001de0_1:
	movs r0, #10
	movs r1, #0
	movs r2, #112
	bl 0x02009774
	movs r0, #10
	movs r1, #0
	movs r2, #64
	bl 0x02009774
.L_02001de0_2:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #10
	bl 0x0200b088
	ldr r3, [r0, #16]
	subs r5, #1
	asrs r3, r3, #20
	str r3, [sp, #4]
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200b028
.L_02001de0_0:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001e5c
	.thumb_func
Func_02001e5c:
	push {r5, r6, lr}
	movs r0, #11
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #11
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	asrs r5, r3, #20
	cmp r6, #30
	beq .L_02001e5c_0
	cmp r6, #34
	bne .L_02001e5c_1
	movs r0, #10
	bl 0x0200b088
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #18
	beq .L_02001e5c_0
	movs r1, #64
	b .L_02001e5c_2
.L_02001e5c_1:
	cmp r6, #36
	bne .L_02001e5c_3
	movs r0, #10
	bl 0x0200b088
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #18
	bne .L_02001e5c_4
	movs r1, #32
.L_02001e5c_2:
	negs r1, r1
	movs r0, #11
	movs r2, #0
	bl 0x02009774
	b .L_02001e5c_3
.L_02001e5c_4:
	movs r1, #96
	negs r1, r1
	movs r0, #11
	movs r2, #0
	bl 0x02009774
.L_02001e5c_3:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #11
	bl 0x0200b088
	ldr r3, [r0, #8]
	subs r5, #1
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b028
.L_02001e5c_0:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02001ef4
	.thumb_func
Func_02001ef4:
	push {r5, r6, lr}
	movs r0, #11
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #11
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	asrs r5, r3, #20
	cmp r6, #36
	beq .L_02001ef4_0
	cmp r6, #30
	bne .L_02001ef4_1
	movs r0, #10
	bl 0x0200b088
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #18
	beq .L_02001ef4_0
	movs r0, #11
	movs r1, #96
	movs r2, #0
	bl 0x02009774
	b .L_02001ef4_2
.L_02001ef4_1:
	cmp r6, #34
	bne .L_02001ef4_2
	movs r0, #11
	movs r1, #32
	movs r2, #0
	bl 0x02009774
.L_02001ef4_2:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #11
	bl 0x0200b088
	ldr r3, [r0, #8]
	subs r5, #1
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b028
.L_02001ef4_0:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001f78
	.thumb_func
Func_02001f78:
	push {r5, r6, r7, lr}
	movs r0, #12
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #12
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	asrs r7, r3, #20
	cmp r6, #36
	bne .L_02001f78_0
	movs r5, #96
	negs r5, r5
	movs r0, #12
	adds r1, r5, #0
	movs r2, #0
	bl 0x02009774
	movs r0, #12
	adds r1, r5, #0
	b .L_02001f78_1
.L_02001f78_0:
	cmp r6, #34
	bne .L_02001f78_2
	movs r1, #96
	negs r1, r1
	movs r0, #12
	movs r2, #0
	bl 0x02009774
	movs r1, #64
	negs r1, r1
	movs r0, #12
.L_02001f78_1:
	movs r2, #0
	bl 0x02009774
	b .L_02001f78_3
.L_02001f78_2:
	cmp r6, #24
	beq .L_02001f78_4
.L_02001f78_3:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #12
	bl 0x0200b088
	ldr r3, [r0, #8]
	subs r5, r7, #1
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b028
.L_02001f78_4:
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02002004
	.thumb_func
Func_02002004:
	push {r5, r6, lr}
	movs r0, #12
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #12
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	asrs r5, r3, #20
	cmp r6, #24
	bne .L_02002004_0
	movs r0, #12
	movs r1, #96
	movs r2, #0
	bl 0x02009774
	movs r0, #12
	movs r1, #96
	b .L_02002004_1
.L_02002004_0:
	cmp r6, #34
	bne .L_02002004_2
	movs r0, #12
	movs r1, #32
.L_02002004_1:
	movs r2, #0
	bl 0x02009774
	b .L_02002004_3
.L_02002004_2:
	cmp r6, #36
	beq .L_02002004_4
.L_02002004_3:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #12
	bl 0x0200b088
	ldr r3, [r0, #8]
	subs r5, #1
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b028
.L_02002004_4:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002080
	.thumb_func
Func_02002080:
	push {r5, r6, r7, lr}
	movs r0, #13
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #13
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #10
	asrs r7, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #15
	asrs r5, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r6, #36
	bne .L_02002080_0
	cmp r3, #34
	bne .L_02002080_1
	movs r1, #16
	b 0x0200a122
.L_02002080_1:
	cmp r5, #7
	bne .L_02002080_2
	movs r1, #32
	b 0x0200a122
.L_02002080_2:
	cmp r3, #30
	bne .L_02002080_3
	movs r1, #80
	b 0x0200a122
.L_02002080_3:
	movs r1, #96
	negs r1, r1
	movs r0, #13
	movs r2, #0
	bl 0x02009774
	movs r1, #80
	b 0x0200a122
.L_02002080_0:
	cmp r6, #35
	bne .L_02002080_4
	cmp r3, #34
	beq 0x0200a164
	cmp r5, #7
	bne .L_02002080_5
	movs r1, #16
	b 0x0200a122
.L_02002080_5:
	cmp r3, #30
	bne .L_02002080_6
	movs r1, #64
	b 0x0200a122
.L_02002080_6:
	movs r5, #80
	negs r5, r5
	movs r0, #13
	adds r1, r5, #0
	movs r2, #0
	bl 0x02009774
	movs r0, #13
	adds r1, r5, #0
	b 0x0200a126
.L_02002080_4:
	cmp r6, #34
.L_02002106:
	bne .L_02002106_0
	cmp r5, #7
	beq .L_02002106_1
	cmp r3, #30
	bne .L_02002106_2
	movs r1, #48
	b .L_02002106_3
.L_02002106_2:
	movs r1, #144
	b .L_02002106_3
.L_02002106_0:
	cmp r6, #31
	bne .L_02002106_4
	cmp r3, #30
	beq .L_02002106_1
	movs r1, #96
.L_02002106_3:
	negs r1, r1
	movs r0, #13
	movs r2, #0
	bl 0x02009774
	b .L_02002106_5
.L_02002106_4:
	cmp r6, #25
	beq .L_02002106_1
.L_02002106_5:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #13
	bl 0x0200b088
	ldr r3, [r0, #8]
	subs r5, r7, #1
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b028
.L_02002106_1:
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_0200216c
	.thumb_func
Func_0200216c:
	push {r5, r6, lr}
	movs r0, #13
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #13
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #15
	asrs r5, r3, #20
	bl 0x0200b088
	cmp r6, #25
	bne .L_0200216c_0
	movs r0, #13
	movs r1, #96
	movs r2, #0
	bl 0x02009774
	movs r0, #13
	movs r1, #80
	b .L_0200216c_1
.L_0200216c_0:
	cmp r6, #31
	bne .L_0200216c_2
	movs r0, #13
	movs r1, #80
	b .L_0200216c_1
.L_0200216c_2:
	cmp r6, #34
	bne .L_0200216c_3
	movs r0, #13
	movs r1, #32
	b .L_0200216c_1
.L_0200216c_3:
	cmp r6, #35
	bne .L_0200216c_4
	movs r0, #13
	movs r1, #16
.L_0200216c_1:
	movs r2, #0
	bl 0x02009774
	b .L_0200216c_5
.L_0200216c_4:
	cmp r6, #36
	beq .L_0200216c_6
.L_0200216c_5:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #13
	bl 0x0200b088
	ldr r3, [r0, #8]
	subs r5, #1
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b028
.L_0200216c_6:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02002200
	.thumb_func
Func_02002200:
	push {r5, r6, r7, lr}
	movs r0, #15
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #15
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #8
	asrs r7, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #10
	asrs r5, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r6, #35
	bne .L_02002200_0
	cmp r3, #7
	bne .L_02002200_1
	movs r1, #16
	b .L_02002200_2
.L_02002200_1:
	cmp r5, #7
	bne .L_02002200_3
	movs r1, #112
	b .L_02002200_2
.L_02002200_3:
	movs r1, #96
	negs r1, r1
	movs r0, #15
	movs r2, #0
	bl 0x02009774
	movs r1, #80
	b .L_02002200_2
.L_02002200_0:
	cmp r6, #34
	bne .L_02002200_4
	cmp r3, #7
	beq .L_02002200_5
	movs r1, #96
	negs r1, r1
	movs r0, #15
	movs r2, #0
	bl 0x02009774
	movs r1, #64
	b .L_02002200_2
.L_02002200_4:
	cmp r6, #33
	bne .L_02002200_6
	movs r1, #144
	b .L_02002200_2
.L_02002200_6:
	cmp r6, #31
	bne .L_02002200_7
	movs r1, #80
	b .L_02002200_2
.L_02002200_7:
	cmp r6, #30
	bne .L_02002200_8
	movs r1, #96
.L_02002200_2:
	negs r1, r1
	movs r0, #15
	movs r2, #0
	bl 0x02009774
	b .L_02002200_9
.L_02002200_8:
	cmp r6, #24
	beq .L_02002200_5
.L_02002200_9:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #15
	bl 0x0200b088
	ldr r3, [r0, #8]
	subs r5, r7, #1
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b028
.L_02002200_5:
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_020022c8
	.thumb_func
Func_020022c8:
	push {r5, r6, r7, lr}
	movs r0, #15
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #15
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #10
	asrs r7, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #13
	asrs r5, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r6, #24
	bne .L_020022c8_0
	cmp r5, #7
	beq .L_020022c8_1
	cmp r3, #31
	bne .L_020022c8_2
.L_020022c8_1:
	movs r0, #15
	movs r1, #96
	b .L_020022c8_3
.L_020022c8_2:
	cmp r3, #34
	bne .L_020022c8_4
	movs r0, #15
	movs r1, #64
	movs r2, #0
	bl 0x02009774
	movs r0, #15
	movs r1, #80
	b .L_020022c8_3
.L_020022c8_4:
	cmp r3, #35
	bne .L_020022c8_5
	movs r0, #15
	movs r1, #80
	movs r2, #0
	bl 0x02009774
	movs r0, #15
	movs r1, #80
	b .L_020022c8_3
.L_020022c8_5:
	movs r0, #15
	movs r1, #80
	movs r2, #0
	bl 0x02009774
	movs r0, #15
	movs r1, #96
	b .L_020022c8_3
.L_020022c8_0:
	cmp r6, #30
	beq .L_020022c8_6
	cmp r3, #31
	bne .L_020022c8_7
.L_020022c8_6:
	cmp r5, #7
	beq .L_020022c8_8
	cmp r3, #34
	bne .L_020022c8_9
	movs r0, #15
	movs r1, #48
	b .L_020022c8_3
.L_020022c8_9:
	cmp r3, #35
	bne .L_020022c8_10
	movs r0, #15
	movs r1, #64
	b .L_020022c8_3
.L_020022c8_10:
	movs r0, #15
	movs r1, #80
	b .L_020022c8_3
.L_020022c8_7:
	cmp r6, #33
	bne .L_020022c8_11
	cmp r3, #34
	beq .L_020022c8_8
	cmp r3, #35
	beq .L_020022c8_12
	movs r0, #15
	movs r1, #32
	b .L_020022c8_3
.L_020022c8_11:
	cmp r6, #34
	bne .L_020022c8_13
.L_020022c8_12:
	movs r0, #15
	movs r1, #16
.L_020022c8_3:
	movs r2, #0
	bl 0x02009774
	b .L_020022c8_14
.L_020022c8_13:
	cmp r6, #35
	beq .L_020022c8_8
.L_020022c8_14:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #15
	bl 0x0200b088
	ldr r3, [r0, #8]
	subs r5, r7, #1
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b028
.L_020022c8_8:
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_020023c4
	.thumb_func
Func_020023c4:
	push {r5, r6, lr}
	movs r0, #17
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #17
	asrs r5, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #19
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r6, #19
	bne .L_020023c4_0
	subs r3, #3
	cmp r3, #2
	bhi .L_020023c4_1
	movs r2, #16
	b .L_020023c4_2
.L_020023c4_1:
	movs r2, #64
	b .L_020023c4_2
.L_020023c4_0:
	cmp r6, #18
	bne .L_020023c4_3
	subs r3, #3
	cmp r3, #2
	bls .L_020023c4_4
	movs r2, #48
.L_020023c4_2:
	negs r2, r2
	movs r0, #17
	movs r1, #0
	bl 0x02009774
	b .L_020023c4_5
.L_020023c4_3:
	cmp r6, #15
	beq .L_020023c4_4
.L_020023c4_5:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #17
	bl 0x0200b088
	ldr r3, [r0, #16]
	subs r5, #1
	asrs r3, r3, #20
	str r3, [sp, #4]
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200b028
.L_020023c4_4:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002450
	.thumb_func
Func_02002450:
	push {r5, r6, lr}
	movs r0, #17
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #17
	asrs r5, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	asrs r6, r3, #20
	cmp r6, #15
	bne .L_02002450_0
	movs r0, #17
	movs r1, #0
	movs r2, #64
	bl 0x02009774
	b .L_02002450_1
.L_02002450_0:
	cmp r6, #18
	bne .L_02002450_2
	movs r0, #17
	movs r1, #0
	movs r2, #16
	bl 0x02009774
	b .L_02002450_1
.L_02002450_2:
	cmp r6, #19
	beq .L_02002450_3
.L_02002450_1:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #17
	bl 0x0200b088
	ldr r3, [r0, #16]
	subs r5, #1
	asrs r3, r3, #20
	str r3, [sp, #4]
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200b028
.L_02002450_3:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020024c8
	.thumb_func
Func_020024c8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #18
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #18
	asrs r3, r3, #20
	mov r8, r3
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #19
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #14
	asrs r7, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #16
	asrs r5, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #8]
	asrs r2, r3, #20
	cmp r6, #19
	bne .L_020024c8_0
	subs r3, r7, #6
	cmp r3, #2
	bhi .L_020024c8_1
	movs r2, #16
	b .L_020024c8_2
.L_020024c8_1:
	subs r3, r5, #6
	cmp r3, #2
	bhi .L_020024c8_3
	movs r2, #64
	b .L_020024c8_2
.L_020024c8_3:
	subs r3, r2, #6
	cmp r3, #2
	bhi .L_020024c8_4
	movs r2, #112
	b .L_020024c8_2
.L_020024c8_4:
	movs r2, #64
	negs r2, r2
	movs r0, #18
	movs r1, #0
	bl 0x02009774
	movs r2, #96
	b .L_020024c8_2
.L_020024c8_0:
	cmp r6, #18
	bne .L_020024c8_5
	subs r3, r7, #6
	cmp r3, #2
	bls .L_020024c8_6
	subs r3, r5, #6
	cmp r3, #2
	bhi .L_020024c8_7
	movs r2, #48
	b .L_020024c8_2
.L_020024c8_7:
	subs r3, r2, #6
	cmp r3, #2
	bhi .L_020024c8_8
	movs r2, #96
	b .L_020024c8_2
.L_020024c8_8:
	movs r2, #144
	b .L_020024c8_2
.L_020024c8_5:
	cmp r6, #15
	bne .L_020024c8_9
	subs r3, r5, #6
	cmp r3, #2
	bls .L_020024c8_6
	subs r3, r2, #6
	cmp r3, #2
	bhi .L_020024c8_10
	movs r2, #48
	b .L_020024c8_2
.L_020024c8_10:
	movs r2, #96
	b .L_020024c8_2
.L_020024c8_9:
	cmp r6, #14
	bne .L_020024c8_11
	subs r3, r5, #6
	cmp r3, #2
	bls .L_020024c8_6
	subs r3, r2, #6
	cmp r3, #2
	bls .L_020024c8_12
	movs r2, #80
	b .L_020024c8_2
.L_020024c8_11:
	cmp r6, #12
	bne .L_020024c8_13
	subs r3, r2, #6
	cmp r3, #2
	bls .L_020024c8_6
	movs r2, #48
	b .L_020024c8_2
.L_020024c8_13:
	cmp r6, #11
	bne .L_020024c8_14
	subs r3, r2, #6
	cmp r3, #2
	bls .L_020024c8_6
.L_020024c8_12:
	movs r2, #32
.L_020024c8_2:
	negs r2, r2
	movs r0, #18
	movs r1, #0
	bl 0x02009774
	b .L_020024c8_15
.L_020024c8_14:
	cmp r6, #9
	beq .L_020024c8_6
.L_020024c8_15:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #18
	bl 0x0200b088
	ldr r3, [r0, #16]
	mov r5, r8
	subs r5, #1
	asrs r3, r3, #20
	str r3, [sp, #4]
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200b028
.L_020024c8_6:
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020025f0
	.thumb_func
Func_020025f0:
	push {r5, r6, r7, lr}
	movs r0, #18
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #18
	asrs r7, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #19
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #14
	asrs r5, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r6, #9
	bne 0x0200a642
	subs r3, #6
	cmp r3, #2
	bls 0x0200a674
	subs r3, r5, #6
	cmp r3, #2
	bls 0x0200a696
	movs r0, #18
	movs r1, #0
	movs r2, #64
	bl 0x02009774
	movs r0, #18
.L_02002638:
	movs r1, #0
	movs r2, #96
	bl 0x02009774
	b 0x0200a6c6
	.2byte 0x2e0b
	.2byte 0xd111
	.2byte 0x3b06
	.2byte 0x2b02
	.2byte 0xd955
	.2byte 0x1fab
	.2byte 0x2b02
	.2byte 0xd805
	.2byte 0x2012
	.2byte 0x2100
	.2byte 0x2230
	.2byte 0xf7ff
	.2byte 0xf88c
	.2byte 0xe033
.L_0200265e:
	movs r0, #18
	movs r1, #0
	movs r2, #128
	bl 0x02009774
	b .L_0200265e_0
	.2byte 0x2e0c
	.2byte 0xd10e
	.2byte 0x1fab
	.2byte 0x2b02
	.2byte 0xd805
	.2byte 0x2012
	.2byte 0x2100
	.2byte 0x2220
	.2byte 0xf7ff
	.2byte 0xf87b
	.2byte 0xe022
	.2byte 0x2012
	.2byte 0x2100
	.2byte 0x2270
	.2byte 0xf7ff
	.2byte 0xf875
	.2byte 0xe01c
	.2byte 0x2e0e
	.2byte 0xd108
	.2byte 0x1fab
	.2byte 0x2b02
	.2byte 0xd930
	.2byte 0x2012
	.2byte 0x2100
	.2byte 0x2250
	.2byte 0xf7ff
	.2byte 0xf86a
	.2byte 0xe011
	.2byte 0x2e0f
	.2byte 0xd105
	.2byte 0x2012
	.2byte 0x2100
	.2byte 0x2240
	.2byte 0xf7ff
	.2byte 0xf862
	.2byte 0xe009
	.2byte 0x2e12
	.2byte 0xd105
	.2byte 0x2012
	.2byte 0x2100
	.2byte 0x2210
	.2byte 0xf7ff
	.2byte 0xf85a
	.2byte 0xe001
	.2byte 0x2e13
	.2byte 0xd018
.L_0200265e_0:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #18
	bl 0x0200b088
	ldr r3, [r0, #16]
	subs r5, r7, #1
	asrs r3, r3, #20
	str r3, [sp, #4]
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200b028
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02002700
	.thumb_func
Func_02002700:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #9
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #9
	asrs r3, r3, #20
	mov r8, r3
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #19
	asrs r7, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #14
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #16
	asrs r5, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #8]
	asrs r2, r3, #20
	cmp r7, #19
	bne .L_02002700_0
	adds r3, r6, #0
	subs r3, #9
	cmp r3, #2
	bhi .L_02002700_1
	movs r2, #16
	b .L_02002700_2
.L_02002700_1:
	adds r3, r5, #0
	subs r3, #9
	cmp r3, #2
	bhi .L_02002700_3
	movs r2, #64
	b .L_02002700_2
.L_02002700_3:
	adds r3, r2, #0
	subs r3, #9
	cmp r3, #2
	bhi .L_02002700_4
	movs r2, #112
	b .L_02002700_2
.L_02002700_4:
	movs r2, #80
	negs r2, r2
	movs r0, #9
	movs r1, #0
	bl 0x02009774
	movs r2, #96
	b .L_02002700_2
.L_02002700_0:
	cmp r7, #18
	bne .L_02002700_5
	adds r3, r6, #0
	subs r3, #9
	cmp r3, #2
	bls .L_02002700_6
	adds r3, r5, #0
	subs r3, #9
	cmp r3, #2
	bls .L_02002700_7
	adds r3, r2, #0
	subs r3, #9
	cmp r3, #2
	bhi .L_02002700_8
	movs r2, #96
	b .L_02002700_2
.L_02002700_8:
	movs r2, #96
	negs r2, r2
	movs r0, #9
	movs r1, #0
	bl 0x02009774
	movs r2, #64
	b .L_02002700_2
.L_02002700_5:
	cmp r7, #15
	bne .L_02002700_9
	adds r3, r5, #0
	subs r3, #9
	cmp r3, #2
	bls .L_02002700_6
	adds r3, r2, #0
	subs r3, #9
	cmp r3, #2
	bls .L_02002700_7
	movs r2, #112
	b .L_02002700_2
.L_02002700_9:
	cmp r7, #14
	bne .L_02002700_10
	adds r3, r5, #0
	subs r3, #9
	cmp r3, #2
	bls .L_02002700_6
	adds r3, r2, #0
	subs r3, #9
	cmp r3, #2
	bhi .L_02002700_11
	movs r2, #32
	b .L_02002700_2
.L_02002700_11:
	movs r2, #96
	b .L_02002700_2
.L_02002700_10:
	cmp r7, #12
	bne .L_02002700_12
	adds r3, r2, #0
	subs r3, #9
	cmp r3, #2
	bls .L_02002700_6
	movs r2, #64
	b .L_02002700_2
.L_02002700_12:
	cmp r7, #11
	bne .L_02002700_13
	adds r3, r2, #0
	subs r3, #9
	cmp r3, #2
	bls .L_02002700_6
.L_02002700_7:
	movs r2, #48
.L_02002700_2:
	negs r2, r2
	movs r0, #9
	movs r1, #0
	bl 0x02009774
	b .L_02002700_14
.L_02002700_13:
	cmp r7, #9
	bls .L_02002700_6
.L_02002700_14:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #9
	bl 0x0200b088
	ldr r3, [r0, #16]
	mov r5, r8
	subs r5, #1
	asrs r3, r3, #20
	str r3, [sp, #4]
	adds r0, r5, #0
	adds r1, r7, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	str r7, [sp, #4]
	bl 0x0200b028
.L_02002700_6:
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002848
	.thumb_func
Func_02002848:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #9
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #9
	asrs r3, r3, #20
	mov r8, r3
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #19
	asrs r7, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #14
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #16
	asrs r5, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r7, #8
	bne 0x0200a8b4
	subs r3, #9
	cmp r3, #2
	bls 0x0200a976
	adds r3, r5, #0
	subs r3, #9
	cmp r3, #2
	bls 0x0200a8c8
	adds r3, r6, #0
	subs r3, #9
	cmp r3, #2
	bls .L_02002848_0
	movs r0, #9
	movs r1, #0
	movs r2, #80
	bl 0x02009774
.L_02002848_0:
	movs r0, #9
	movs r1, #0
	movs r2, #96
	bl 0x02009774
	b 0x0200a942
.L_020028b4:
	cmp r7, #11
	bne .L_020028b4_0
	adds r3, r5, #0
	subs r3, #9
	cmp r3, #2
	bls .L_020028b4_1
	adds r3, r6, #0
	subs r3, #9
	cmp r3, #2
	bhi .L_020028b4_2
	movs r0, #9
	movs r1, #0
	movs r2, #48
	bl 0x02009774
	b .L_020028b4_3
.L_020028b4_2:
	movs r0, #9
	movs r1, #0
	movs r2, #128
	bl 0x02009774
	b .L_020028b4_3
.L_020028b4_0:
	cmp r7, #12
	bne .L_020028b4_4
	adds r3, r5, #0
	subs r3, #9
	cmp r3, #2
	bls .L_020028b4_1
	adds r3, r6, #0
	subs r3, #9
	cmp r3, #2
	bhi .L_020028b4_5
	movs r0, #9
	movs r1, #0
	movs r2, #32
	bl 0x02009774
	b .L_020028b4_3
.L_020028b4_5:
	movs r0, #9
	movs r1, #0
	movs r2, #112
	bl 0x02009774
	b .L_020028b4_3
.L_020028b4_4:
	cmp r7, #14
	bne .L_020028b4_6
	adds r3, r6, #0
	subs r3, #9
	cmp r3, #2
	bls .L_020028b4_1
	movs r0, #9
	movs r1, #0
	movs r2, #80
	bl 0x02009774
	b .L_020028b4_3
.L_020028b4_6:
	cmp r7, #15
	bne .L_020028b4_7
	movs r0, #9
	movs r1, #0
	movs r2, #64
	bl 0x02009774
	b .L_020028b4_3
.L_020028b4_7:
	cmp r7, #18
	bne .L_020028b4_3
	movs r0, #9
	movs r1, #0
	movs r2, #16
	bl 0x02009774
.L_020028b4_3:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #9
	bl 0x0200b088
	ldr r3, [r0, #16]
	mov r5, r8
	subs r5, #1
	asrs r3, r3, #20
	str r3, [sp, #4]
	adds r0, r5, #0
	adds r1, r7, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	str r7, [sp, #4]
	bl 0x0200b028
.L_020028b4_1:
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002984
	.thumb_func
Func_02002984:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #19
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #19
	asrs r7, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #17
	asrs r3, r3, #20
	mov r8, r3
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #18
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #9
	asrs r5, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r7, #3
	beq .L_02002984_0
	cmp r7, #13
	bne .L_02002984_1
	cmp r3, #15
	bne .L_02002984_2
	movs r1, #16
	b .L_02002984_3
.L_02002984_2:
	cmp r5, #15
	bne .L_02002984_4
	movs r1, #64
	b .L_02002984_3
.L_02002984_4:
	cmp r6, #15
	bne .L_02002984_5
	movs r1, #112
	b .L_02002984_3
.L_02002984_5:
	movs r1, #112
	negs r1, r1
	movs r0, #19
	movs r2, #0
	bl 0x02009774
	movs r1, #48
	b .L_02002984_3
.L_02002984_1:
	cmp r7, #6
	bne .L_02002984_6
	cmp r6, #15
	beq .L_02002984_0
	movs r1, #48
	b .L_02002984_3
.L_02002984_6:
	cmp r7, #5
	bne .L_02002984_7
	movs r1, #32
	b .L_02002984_3
.L_02002984_7:
	cmp r7, #8
	bne .L_02002984_8
	cmp r5, #15
	beq .L_02002984_0
	cmp r6, #15
	bne .L_02002984_9
	movs r1, #32
	b .L_02002984_3
.L_02002984_9:
	movs r1, #80
	b .L_02002984_3
.L_02002984_8:
	cmp r7, #9
	bne .L_02002984_10
	cmp r5, #15
	beq .L_02002984_0
	cmp r6, #15
	bne .L_02002984_11
	movs r1, #48
	b .L_02002984_3
.L_02002984_10:
	cmp r7, #12
	bne .L_02002984_12
	cmp r3, #15
	beq .L_02002984_0
	cmp r5, #15
	bne .L_02002984_13
	movs r1, #48
	b .L_02002984_3
.L_02002984_13:
	cmp r6, #15
	bne .L_02002984_14
.L_02002984_11:
	movs r1, #96
.L_02002984_3:
	negs r1, r1
	movs r0, #19
	movs r2, #0
	bl 0x02009774
	b .L_02002984_12
.L_02002984_14:
	movs r1, #144
	negs r1, r1
	movs r0, #19
	movs r2, #0
	bl 0x02009774
.L_02002984_12:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #19
	bl 0x0200b088
	ldr r3, [r0, #8]
	mov r5, r8
	subs r5, #1
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r7, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b028
.L_02002984_0:
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002a98
	.thumb_func
Func_02002a98:
	push {r5, r6, r7, lr}
	movs r0, #19
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #19
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #18
	asrs r7, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #9
	asrs r5, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r6, #3
	bne .L_02002a98_0
	cmp r5, #15
	bne .L_02002a98_1
	movs r0, #19
	movs r1, #32
	b .L_02002a98_2
.L_02002a98_1:
	cmp r3, #15
	bne .L_02002a98_3
	movs r0, #19
	movs r1, #80
	b .L_02002a98_2
.L_02002a98_3:
	movs r0, #19
	movs r1, #112
	movs r2, #0
	bl 0x02009774
	movs r0, #19
	movs r1, #48
	b .L_02002a98_2
.L_02002a98_0:
	cmp r6, #5
	bne .L_02002a98_4
	cmp r5, #15
	beq .L_02002a98_5
	cmp r3, #15
	bne .L_02002a98_6
	movs r0, #19
	movs r1, #48
	b .L_02002a98_2
.L_02002a98_6:
	movs r0, #19
	movs r1, #128
	b .L_02002a98_2
.L_02002a98_4:
	cmp r6, #6
	bne .L_02002a98_7
	cmp r3, #15
	bne .L_02002a98_8
	movs r0, #19
	movs r1, #32
	b .L_02002a98_2
.L_02002a98_8:
	movs r0, #19
	movs r1, #112
	b .L_02002a98_2
.L_02002a98_7:
	cmp r6, #8
	bne .L_02002a98_9
	cmp r3, #15
	beq .L_02002a98_5
	movs r0, #19
	movs r1, #80
	b .L_02002a98_2
.L_02002a98_9:
	cmp r6, #9
	bne .L_02002a98_10
	movs r0, #19
	movs r1, #64
.L_02002a98_2:
	movs r2, #0
	bl 0x02009774
	b .L_02002a98_11
.L_02002a98_10:
	cmp r6, #12
	bne .L_02002a98_11
	movs r0, #19
	movs r1, #16
	movs r2, #0
	bl 0x02009774
.L_02002a98_11:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #19
	bl 0x0200b088
	ldr r3, [r0, #8]
	subs r5, r7, #1
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b028
.L_02002a98_5:
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002b80
	.thumb_func
Func_02002b80:
	push {r5, r6, r7, lr}
	movs r0, #14
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #14
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #18
	asrs r7, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #9
	asrs r5, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r6, #13
	bne .L_02002b80_0
	subs r3, #12
	cmp r3, #2
	bhi .L_02002b80_1
	movs r1, #16
	b .L_02002b80_2
.L_02002b80_1:
	adds r3, r5, #0
	subs r3, #12
	cmp r3, #2
	bhi .L_02002b80_3
	movs r1, #64
	b .L_02002b80_2
.L_02002b80_3:
	movs r1, #112
	b .L_02002b80_2
.L_02002b80_0:
	cmp r6, #12
	bne .L_02002b80_4
	subs r3, #12
	cmp r3, #2
	bls .L_02002b80_5
	adds r3, r5, #0
	subs r3, #12
	cmp r3, #2
	bhi .L_02002b80_6
	movs r1, #48
	b .L_02002b80_2
.L_02002b80_6:
	movs r1, #96
	b .L_02002b80_2
.L_02002b80_4:
	cmp r6, #9
	bne .L_02002b80_7
	adds r3, r5, #0
	subs r3, #12
	cmp r3, #2
	bls .L_02002b80_5
	movs r1, #48
	b .L_02002b80_2
.L_02002b80_7:
	cmp r6, #8
	bne .L_02002b80_8
	adds r3, r5, #0
	subs r3, #12
	cmp r3, #2
	bls .L_02002b80_5
	movs r1, #32
.L_02002b80_2:
	negs r1, r1
	movs r0, #14
	movs r2, #0
	bl 0x02009774
	b .L_02002b80_9
.L_02002b80_8:
	cmp r6, #6
	beq .L_02002b80_5
.L_02002b80_9:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #14
	bl 0x0200b088
	ldr r3, [r0, #8]
	subs r5, r7, #1
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b028
.L_02002b80_5:
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02002c4c
	.thumb_func
Func_02002c4c:
	push {r5, r6, r7, lr}
	movs r0, #14
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #14
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #18
	asrs r7, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #9
	asrs r5, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r6, #6
	bne .L_02002c4c_0
	subs r3, #12
	cmp r3, #2
	bhi .L_02002c4c_1
	movs r0, #14
	movs r1, #32
	b .L_02002c4c_2
.L_02002c4c_1:
	adds r3, r5, #0
	subs r3, #12
	cmp r3, #2
	bhi .L_02002c4c_3
	movs r0, #14
	movs r1, #64
	b .L_02002c4c_2
.L_02002c4c_3:
	movs r0, #14
	movs r1, #112
	b .L_02002c4c_2
.L_02002c4c_0:
	cmp r6, #8
	bne .L_02002c4c_4
	subs r3, #12
	cmp r3, #2
	bls .L_02002c4c_5
	movs r0, #14
	movs r1, #80
	b .L_02002c4c_2
.L_02002c4c_4:
	cmp r6, #9
	bne .L_02002c4c_6
	subs r3, #12
	cmp r3, #2
	bls .L_02002c4c_5
	movs r0, #14
	movs r1, #64
	b .L_02002c4c_2
.L_02002c4c_6:
	cmp r6, #12
	bne .L_02002c4c_7
	movs r0, #14
	movs r1, #16
.L_02002c4c_2:
	movs r2, #0
	bl 0x02009774
	b .L_02002c4c_8
.L_02002c4c_7:
	cmp r6, #13
	beq .L_02002c4c_5
.L_02002c4c_8:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #14
	bl 0x0200b088
	ldr r3, [r0, #8]
	subs r5, r7, #1
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b028
.L_02002c4c_5:
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002d0c
	.thumb_func
Func_02002d0c:
	push {r5, r6, r7, lr}
	movs r0, #16
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #16
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #18
	asrs r7, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #9
	asrs r5, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r6, #13
	bne .L_02002d0c_0
	subs r3, #9
	cmp r3, #2
	bhi .L_02002d0c_1
	movs r1, #16
	b .L_02002d0c_2
.L_02002d0c_1:
	adds r3, r5, #0
	subs r3, #9
	cmp r3, #2
	bhi .L_02002d0c_3
	movs r1, #64
	b .L_02002d0c_2
.L_02002d0c_3:
	movs r1, #112
	b .L_02002d0c_2
.L_02002d0c_0:
	cmp r6, #12
	bne .L_02002d0c_4
	subs r3, #9
	cmp r3, #2
	bls .L_02002d0c_5
	adds r3, r5, #0
	subs r3, #9
	cmp r3, #2
	bhi .L_02002d0c_6
	movs r1, #48
	b .L_02002d0c_2
.L_02002d0c_6:
	movs r1, #96
	b .L_02002d0c_2
.L_02002d0c_4:
	cmp r6, #9
	bne .L_02002d0c_7
	adds r3, r5, #0
	subs r3, #9
	cmp r3, #2
	bls .L_02002d0c_5
	movs r1, #48
	b .L_02002d0c_2
.L_02002d0c_7:
	cmp r6, #8
	bne .L_02002d0c_8
	movs r1, #32
.L_02002d0c_2:
	negs r1, r1
	movs r0, #16
	movs r2, #0
	bl 0x02009774
	b .L_02002d0c_9
.L_02002d0c_8:
	cmp r6, #6
	beq .L_02002d0c_5
.L_02002d0c_9:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #16
	bl 0x0200b088
	ldr r3, [r0, #8]
	subs r5, r7, #1
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b028
.L_02002d0c_5:
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02002dd0
	.thumb_func
Func_02002dd0:
	push {r5, r6, lr}
	movs r0, #16
	sub sp, #8
	bl 0x0200b088
	ldr r3, [r0, #8]
	movs r0, #16
	asrs r6, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	movs r0, #9
	asrs r5, r3, #20
	bl 0x0200b088
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r6, #6
	bne .L_02002dd0_0
	subs r3, #9
	cmp r3, #2
	bhi .L_02002dd0_1
	movs r0, #16
	movs r1, #32
	b .L_02002dd0_2
.L_02002dd0_1:
	movs r0, #16
	movs r1, #112
	b .L_02002dd0_2
.L_02002dd0_0:
	cmp r6, #8
	bne .L_02002dd0_3
	subs r3, #9
	cmp r3, #2
	bls .L_02002dd0_4
	movs r0, #16
	movs r1, #80
	b .L_02002dd0_2
.L_02002dd0_3:
	cmp r6, #9
	bne .L_02002dd0_5
	movs r0, #16
	movs r1, #64
	b .L_02002dd0_2
.L_02002dd0_5:
	cmp r6, #12
	bne .L_02002dd0_6
	movs r0, #16
	movs r1, #16
.L_02002dd0_2:
	movs r2, #0
	bl 0x02009774
	b .L_02002dd0_7
.L_02002dd0_6:
	cmp r6, #13
	beq .L_02002dd0_4
.L_02002dd0_7:
	movs r0, #2
	bl 0x0200afb8
	movs r0, #16
	bl 0x0200b088
	ldr r3, [r0, #8]
	subs r5, #1
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl 0x0200b028
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200b028
.L_02002dd0_4:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02002e70
	.thumb_func
Func_02002e70:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r0, [r6, #48]
	ldr r7, [r6, #80]
	bl 0x0200afc8
	lsls r5, r0, #1
	cmp r5, #0
	ble .L_02002e70_0
	negs r5, r5
.L_02002e70_0:
	ldr r0, [r6, #48]
	bl 0x0200afd0
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
	bl 0x0200afd0
	cmp r0, #0
	bge .L_02002e70_1
	adds r0, #7
.L_02002e70_1:
	asrs r3, r0, #3
	strh r3, [r7, #30]
	bl 0x0200afc0
	adds r5, r0, #0
	bl 0x0200afc0
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
	.global Func_02002ed8
	.thumb_func
Func_02002ed8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	bl 0x0200b088
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
	bl 0x0200b038
	movs r3, #92
	adds r3, r3, r7
	mov r2, r8
	strb r2, [r3]
	mov r10, r3
	adds r3, r7, #0
	adds r3, #85
	strb r2, [r3]
	ldr r0, [pc, #124]
	bl 0x0200b058
	cmp r0, #0
	bne .L_02002ed8_0
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #14
	adds r3, r3, r2
	str r3, [r7, #12]
.L_02002ed8_0:
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
	bl 0x0200afe0
	adds r5, r0, #0
	movs r0, #181
	bl 0x0200b050
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	adds r2, r5, #0
	movs r1, #128
	ldrb r0, [r6, #28]
	bl 0x0200aff0
	movs r0, #17
	bl 0x0200afe8
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
	.4byte 0x0200ae71
	.include "games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/IMPORT.INC"
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
	.4byte 0x0017003a
	.4byte 0x00020001
	.4byte 0x003b0005
	.4byte 0x00010017
	.4byte 0x00050002
	.4byte 0x0000ffff
	.4byte 0x0200b214
	.4byte 0x0200b24c
	.4byte 0x0200b284
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000118
	.4byte 0xc0000220
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00000230
	.4byte 0xffff0002
	.4byte 0x00000088
	.4byte 0x40000168
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00000230
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000138
	.4byte 0xc0000208
	.4byte 0x00100000
	.4byte 0x01a00040
	.4byte 0x00000220
	.4byte 0xffff0002
	.4byte 0x00000118
	.4byte 0x00000090
	.4byte 0x00100000
	.4byte 0x01a00040
	.4byte 0x00000220
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000138
	.4byte 0xc0000198
	.4byte 0x00000000
	.4byte 0x02800010
	.4byte 0x000001d0
	.4byte 0xffff0002
	.4byte 0x000000c8
	.4byte 0xc0000198
	.4byte 0x00000000
	.4byte 0x02800010
	.4byte 0x000001d0
	.4byte 0xffff0063
	.4byte 0x00000110
	.4byte 0x400000e8
	.4byte 0x00000000
	.4byte 0x02800010
	.4byte 0x000001d0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000000b8
	.4byte 0x80000088
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000028
	.4byte 0x000000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000138
	.4byte 0x80000290
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000000d8
	.4byte 0xc0000220
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000348
	.4byte 0x800000b0
	.4byte 0x01b00000
	.4byte 0x03700020
	.4byte 0x00000160
	.4byte 0xffff0002
	.4byte 0x000001e8
	.4byte 0x000000b0
	.4byte 0x01b00000
	.4byte 0x03700020
	.4byte 0x00000160
	.4byte 0xffff0003
	.4byte 0x000002c8
	.4byte 0x40000098
	.4byte 0x01b00000
	.4byte 0x03700020
	.4byte 0x00000160
	.4byte 0xffff0004
	.4byte 0x00000258
	.4byte 0x40000098
	.4byte 0x01b00000
	.4byte 0x03700020
	.4byte 0x00000160
	.4byte 0xffff0005
	.4byte 0x000002c8
	.4byte 0xc0000098
	.4byte 0x01b00000
	.4byte 0x03700020
	.4byte 0x00000160
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000071
	.4byte 0x0014c002
	.4byte 0x00201072
	.4byte 0x00000072
	.4byte 0x00102071
	.4byte 0x0020107e
	.4byte 0x0000007b
	.4byte 0x00103086
	.4byte 0x00204086
	.4byte 0x0000007c
	.4byte 0x00102086
	.4byte 0x0020107d
	.4byte 0x0000007d
	.4byte 0x0010207c
	.4byte 0x0000007e
	.4byte 0x00102072
	.4byte 0x0020107f
	.4byte 0x00301073
	.4byte 0x00402073
	.4byte 0x0000007f
	.4byte 0x0010207e
	.4byte 0x00201080
	.4byte 0x00301074
	.4byte 0x00402074
	.4byte 0x00000080
	.4byte 0x0010207f
	.4byte 0x00201081
	.4byte 0x00301075
	.4byte 0x00402075
	.4byte 0x00000081
	.4byte 0x00102080
	.4byte 0x00201082
	.4byte 0x00301076
	.4byte 0x00402076
	.4byte 0x00000082
	.4byte 0x00102081
	.4byte 0x00201083
	.4byte 0x00301077
	.4byte 0x00402077
	.4byte 0x00000083
	.4byte 0x00102082
	.4byte 0x00201084
	.4byte 0x00301078
	.4byte 0x00402078
	.4byte 0x00000084
	.4byte 0x00102083
	.4byte 0x00201085
	.4byte 0x00301079
	.4byte 0x00402079
	.4byte 0x00000085
	.4byte 0x00102084
	.4byte 0x00201086
	.4byte 0x0030107a
	.4byte 0x0040207a
	.4byte 0x00000086
	.4byte 0x00102085
	.4byte 0x0020107c
	.4byte 0x0030107b
	.4byte 0x0040207b
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0fd70016
	.4byte 0x00000007
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00024000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00024000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00024000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00024000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
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
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x020093ad
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x0200938d
	.4byte 0x00000202
	.4byte 0xffff0014
	.4byte 0x02009375
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x020092b5
	.4byte 0x00000013
	.4byte 0x0ef40064
	.4byte 0x00500004
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00009415
	.4byte 0x0fd70008
	.4byte 0x02008f3d
	.4byte 0x00000c15
	.4byte 0x0240000b
	.4byte 0x02009509
	.4byte 0x00000c15
	.4byte 0x0241000c
	.4byte 0x02009549
	.4byte 0x00000c15
	.4byte 0x0242000d
	.4byte 0x0200958d
	.4byte 0x00000c15
	.4byte 0x0243000e
	.4byte 0x020095d1
	.4byte 0x00004e15
	.4byte 0x08c4000f
	.4byte 0x02009625
	.4byte 0x00004e15
	.4byte 0x08c50010
	.4byte 0x0200964d
	.4byte 0x00004e15
	.4byte 0x08c60011
	.4byte 0x0200965d
	.4byte 0x00004e15
	.4byte 0x08c70012
	.4byte 0x0200966d
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000202
	.4byte 0xffff0014
	.4byte 0x02009741
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x02009741
	.4byte 0x00008413
	.4byte 0x0ec00064
	.4byte 0x00200001
	.4byte 0x0000c413
	.4byte 0x0ec00064
	.4byte 0x00200001
	.4byte 0x0000a413
	.4byte 0x0ec00064
	.4byte 0x00200001
	.4byte 0x00008413
	.4byte 0x0ec10065
	.4byte 0x001000c3
	.4byte 0x0000c413
	.4byte 0x0ec10065
	.4byte 0x001000c3
	.4byte 0x0000a413
	.4byte 0x0ec10065
	.4byte 0x001000c3
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00008602
	.4byte 0xffff0028
	.4byte 0x02009b69
	.4byte 0x00000602
	.4byte 0xffff0029
	.4byte 0x02009b95
	.4byte 0x00004602
	.4byte 0xffff002a
	.4byte 0x02009b41
	.4byte 0x0000c602
	.4byte 0xffff002b
	.4byte 0x02009b15
	.4byte 0x0000c602
	.4byte 0xffff0015
	.4byte 0x02009bbd
	.4byte 0x00004602
	.4byte 0xffff0015
	.4byte 0x02009c85
	.4byte 0x00000602
	.4byte 0xffff0015
	.4byte 0x02009b95
	.4byte 0x00008602
	.4byte 0xffff0015
	.4byte 0x02009b69
	.4byte 0x0000c602
	.4byte 0xffff0016
	.4byte 0x02009d2d
	.4byte 0x00004602
	.4byte 0xffff0016
	.4byte 0x02009de1
	.4byte 0x00000602
	.4byte 0xffff0016
	.4byte 0x02009b95
	.4byte 0x00008602
	.4byte 0xffff0016
	.4byte 0x02009b69
	.4byte 0x00008602
	.4byte 0xffff0017
	.4byte 0x02009e5d
	.4byte 0x00000602
	.4byte 0xffff0017
	.4byte 0x02009ef5
	.4byte 0x0000c602
	.4byte 0xffff0017
	.4byte 0x02009b15
	.4byte 0x00004602
	.4byte 0xffff0017
	.4byte 0x02009b41
	.4byte 0x00008602
	.4byte 0xffff0018
	.4byte 0x02009f79
	.4byte 0x00000602
	.4byte 0xffff0018
	.4byte 0x0200a005
	.4byte 0x0000c602
	.4byte 0xffff0018
	.4byte 0x02009b15
	.4byte 0x00004602
	.4byte 0xffff0018
	.4byte 0x02009b41
	.4byte 0x00008602
	.4byte 0xffff0019
	.4byte 0x0200a081
	.4byte 0x00000602
	.4byte 0xffff0019
	.4byte 0x0200a16d
	.4byte 0x0000c602
	.4byte 0xffff0019
	.4byte 0x02009b15
	.4byte 0x00004602
	.4byte 0xffff0019
	.4byte 0x02009b41
	.4byte 0x00008602
	.4byte 0xffff001a
	.4byte 0x0200a201
	.4byte 0x00000602
	.4byte 0xffff001a
	.4byte 0x0200a2c9
	.4byte 0x0000c602
	.4byte 0xffff001a
	.4byte 0x02009b15
	.4byte 0x00004602
	.4byte 0xffff001a
	.4byte 0x02009b41
	.4byte 0x0000c602
	.4byte 0xffff001f
	.4byte 0x0200a3c5
	.4byte 0x00004602
	.4byte 0xffff001f
	.4byte 0x0200a451
	.4byte 0x00000602
	.4byte 0xffff001f
	.4byte 0x02009b95
	.4byte 0x00008602
	.4byte 0xffff001f
	.4byte 0x02009b69
	.4byte 0x0000c602
	.4byte 0xffff0020
	.4byte 0x0200a4c9
	.4byte 0x00004602
	.4byte 0xffff0020
	.4byte 0x0200a5f1
	.4byte 0x00000602
	.4byte 0xffff0020
	.4byte 0x02009b95
	.4byte 0x00008602
	.4byte 0xffff0020
	.4byte 0x02009b69
	.4byte 0x0000c602
	.4byte 0xffff0021
	.4byte 0x0200a701
	.4byte 0x00004602
	.4byte 0xffff0021
	.4byte 0x0200a849
	.4byte 0x00000602
	.4byte 0xffff0021
	.4byte 0x02009b95
	.4byte 0x00008602
	.4byte 0xffff0021
	.4byte 0x02009b69
	.4byte 0x00008602
	.4byte 0xffff0022
	.4byte 0x0200a985
	.4byte 0x00000602
	.4byte 0xffff0022
	.4byte 0x0200aa99
	.4byte 0x0000c602
	.4byte 0xffff0022
	.4byte 0x02009b15
	.4byte 0x00004602
	.4byte 0xffff0022
	.4byte 0x02009b41
	.4byte 0x00008602
	.4byte 0xffff0023
	.4byte 0x0200ab81
	.4byte 0x00000602
	.4byte 0xffff0023
	.4byte 0x0200ac4d
	.4byte 0x0000c602
	.4byte 0xffff0023
	.4byte 0x02009b15
	.4byte 0x00004602
	.4byte 0xffff0023
	.4byte 0x02009b41
	.4byte 0x00008602
	.4byte 0xffff0024
	.4byte 0x0200ad0d
	.4byte 0x00000602
	.4byte 0xffff0024
	.4byte 0x0200add1
	.4byte 0x0000c602
	.4byte 0xffff0024
	.4byte 0x02009b15
	.4byte 0x00004602
	.4byte 0xffff0024
	.4byte 0x02009b41
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x020099b5
	.4byte 0x00000002
	.4byte 0xffff0016
	.4byte 0x020099c1
	.4byte 0x00000002
	.4byte 0xffff0017
	.4byte 0x020099cd
	.4byte 0x00000002
	.4byte 0xffff0018
	.4byte 0x020099d9
	.4byte 0x00000002
	.4byte 0xffff0019
	.4byte 0x020099e5
	.4byte 0x00000002
	.4byte 0xffff001a
	.4byte 0x020099f1
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte 0x020099fd
	.4byte 0x00000002
	.4byte 0xffff0020
	.4byte 0x02009a09
	.4byte 0x00000002
	.4byte 0xffff0021
	.4byte 0x02009a15
	.4byte 0x00000002
	.4byte 0xffff0022
	.4byte 0x02009a21
	.4byte 0x00000002
	.4byte 0xffff0023
	.4byte 0x02009a2d
	.4byte 0x00000002
	.4byte 0xffff0024
	.4byte 0x02009a39
	.4byte 0x00000013
	.4byte 0x0ee20064
	.4byte 0x002003e7
	.4byte 0x00000013
	.4byte 0x0ee30065
	.4byte 0x001000bd
	.4byte 0x00000013
	.4byte 0x0ee40066
	.4byte 0x0010000b
	.4byte 0x00000013
	.4byte 0x0ee50067
	.4byte 0x001000e3
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x08d1000a
	.4byte 0x02008e89
	.4byte 0x00004602
	.4byte 0xffff0014
	.4byte 0x02009b41
	.4byte 0x00000013
	.4byte 0x0ee60064
	.4byte 0x00100053
	.4byte 0x00000013
	.4byte 0x0ef70065
	.4byte 0x00500007
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
