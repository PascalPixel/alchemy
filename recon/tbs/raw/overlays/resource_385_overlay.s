.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_MURA_SAI/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
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
	.global Func_02000048
	.thumb_func
Func_02000048:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl 0x02009080
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000048_0
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
	bl 0x020090a0
	adds r0, r5, #0
	movs r1, #14
	bl 0x02009160
	adds r0, r5, #0
	movs r1, #1
	bl 0x020090a8
	adds r0, r5, #0
	b .L_02000048_1
.L_02000048_0:
	movs r0, #0
.L_02000048_1:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_020000a0
	.thumb_func
Func_020000a0:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl 0x02009080
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020000a0_0
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	adds r2, r5, #0
	strb r3, [r1, #9]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
	movs r3, #8
	strb r3, [r2]
	movs r1, #0
	bl 0x020090a0
	adds r0, r5, #0
	movs r1, #15
	bl 0x02009160
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #2
	orrs r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	b .L_020000a0_1
.L_020000a0_0:
	movs r0, #0
.L_020000a0_1:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02000104
	.thumb_func
Func_02000104:
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
	.global Func_0200013c
	.thumb_func
Func_0200013c:
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
	bl 0x02009100
	movs r3, #128
	lsls r3, r3, #13
	mov r2, r10
	ands r3, r2
	mov r9, r0
	cmp r3, #0
	beq .L_0200013c_0
	cmp r7, #0
	beq .L_0200013c_0
	movs r3, #24
	ldrsh r0, [r7, r3]
	adds r2, r6, #0
	b .L_0200013c_1
.L_0200013c_0:
	adds r2, r6, #0
	movs r0, #222
.L_0200013c_1:
	adds r1, r5, #0
	mov r3, r8
	bl 0x02009080
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200013c_2
	b .L_0200013c_3
.L_0200013c_2:
	ldr r1, [r6, #80]
	mov r8, r1
	mov r1, r10
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	bl 0x02009070
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl 0x02009078
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
	beq .L_0200013c_3
	cmp r7, #0
	beq .L_0200013c_3
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200013c_4
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl 0x02009160
.L_0200013c_4:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_5
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
	mov r1, r8
	strb r3, [r1, #9]
.L_0200013c_5:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r10
	ands r2, r3
	cmp r2, #0
	beq .L_0200013c_6
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_0200013c_6:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200013c_7
	ldr r3, [pc, #156]
	mov r1, r11
	ldr r5, [r3, r1]
	cmp r2, #0
	beq .L_0200013c_8
	ldr r0, [r7, #16]
	ldr r3, [r6, #24]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	bl 0x02009058
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200013c_9
.L_0200013c_8:
	ldr r0, [r7, #16]
	ldr r2, [pc, #128]
	ldr r1, [r5, #12]
	adds r0, r0, r2
	bl 0x02009058
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200013c_9:
	bl 0x02009058
	str r0, [r6, #52]
.L_0200013c_7:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200013c_10
	adds r0, r6, #0
	movs r1, #1
	bl 0x02009070
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl 0x02009078
.L_0200013c_10:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_11
	ldrh r3, [r7, #32]
	mov r1, r8
	strh r3, [r1, #30]
.L_0200013c_11:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_12
	ldrh r3, [r7, #34]
	ldr r1, [sp, #0]
	strh r3, [r1]
.L_0200013c_12:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_3
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200013c_3:
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200929c
	.4byte 0x02008105
	.4byte 0xffff0000
	.global Func_02000314
	.thumb_func
Func_02000314:
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
	bl 0x020091c4
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x030001d8
	.global Func_02000350
	.thumb_func
Func_02000350:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	mov r8, r1
	mov r7, r8
	adds r5, r6, #0
	adds r7, #8
	adds r5, #8
	mov r10, r2
	adds r0, r7, #0
	movs r2, #0
	adds r1, r5, #0
	mov r11, r3
	mov r9, r2
	bl 0x02008314
	cmp r0, r10
	blt .L_02000350_0
	mov r3, r11
	cmp r3, #0
	beq .L_02000350_1
.L_02000350_0:
	mov r2, r8
	ldr r0, [r2, #16]
	ldr r3, [r6, #16]
	ldr r1, [r7]
	subs r0, r0, r3
	ldr r3, [r5]
	subs r1, r1, r3
	bl 0x02009068
	ldr r3, [pc, #100]
	lsls r0, r0, #16
	movs r2, #128
	lsrs r0, r0, #16
	lsls r2, r2, #5
	adds r4, r0, r3
	adds r1, r0, r2
	movs r3, #240
	ldrh r2, [r6, #6]
	lsls r3, r3, #8
	ands r4, r3
	ands r1, r3
	ands r0, r3
	ands r3, r2
	cmp r0, r3
	beq .L_02000350_2
	cmp r1, r3
	beq .L_02000350_2
	cmp r4, r3
	beq .L_02000350_2
	mov r3, r11
	cmp r3, #0
	beq .L_02000350_3
.L_02000350_2:
	adds r2, r6, #0
	adds r2, #91
	movs r3, #1
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #1
	bl 0x02009070
	movs r2, #1
	mov r9, r2
	b .L_02000350_3
.L_02000350_1:
	adds r3, r6, #0
	adds r3, #91
	mov r2, r9
	strb r2, [r3]
	adds r0, r6, #0
	movs r1, #2
	bl 0x02009070
.L_02000350_3:
	mov r0, r9
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0xfffff000
	.global Func_02000400
	.thumb_func
Func_02000400:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, [pc, #132]
	adds r5, r0, #0
	ldr r2, [r3]
	adds r6, r5, #0
	mov r8, r2
	adds r6, #100
	movs r2, #18
	ldr r7, [r3, #48]
	mov r10, r2
	movs r3, #0
	ldrh r2, [r6]
	mov r9, r3
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02000400_0
	movs r0, #15
	b .L_02000400_1
.L_02000400_0:
	movs r0, #14
.L_02000400_1:
	bl 0x02009100
	adds r1, r0, #0
	adds r0, r5, #0
	movs r2, #32
	movs r3, #0
	bl 0x02008350
	cmp r0, #0
	bne .L_02000400_2
	movs r0, #0
	bl 0x02009100
	movs r2, #188
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r1, r0, #0
	cmp r3, #0
	bne .L_02000400_3
	ldr r3, [pc, #56]
	add r3, r8
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_02000400_4
.L_02000400_3:
	movs r3, #26
	ldrh r2, [r6]
	mov r10, r3
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_02000400_4
	movs r2, #1
	mov r9, r2
.L_02000400_4:
	adds r0, r5, #0
	mov r2, r10
	mov r3, r9
	bl 0x02008350
.L_02000400_2:
	movs r0, #0
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x03001e8c
	.4byte 0x00000ea4
	.global Func_02000498
	.thumb_func
Func_02000498:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009450
	.global Func_020004a0
	.thumb_func
Func_020004a0:
	movs r0, #0
	bx lr
	.global Func_020004a4
	.thumb_func
Func_020004a4:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020095a0
	.global Func_020004ac
	.thumb_func
Func_020004ac:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020095d8
	.global Func_020004b4
	.thumb_func
Func_020004b4:
	push {r5, lr}
	adds r5, r0, #0
	bl 0x020090e8
	adds r0, r5, #0
	movs r1, #1
	bl 0x02009138
	adds r0, r5, #0
	movs r1, #0
	bl 0x02009178
	bl 0x020090f0
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020004d8
	.thumb_func
Func_020004d8:
	push {lr}
	bl 0x020090e8
	movs r2, #2
	movs r1, #0
	movs r0, #8
	bl 0x02009150
	ldr r0, [pc, #24]
	bl 0x020090c8
	ldr r0, [pc, #24]
	bl 0x02009168
	movs r0, #8
	movs r1, #0
	bl 0x02009178
	bl 0x020090f0
	pop {r0}
	bx r0
	.4byte 0x00000305
	.4byte 0x00001cab
	.global Func_0200050c
	.thumb_func
Func_0200050c:
	push {lr}
	ldr r0, [pc, #24]
	bl 0x02009168
	movs r0, #11
	movs r1, #0
	movs r2, #2
	bl 0x02009158
	movs r0, #11
	bl 0x020084b4
	pop {r0}
	bx r0
	.4byte 0x00001cae
	.global Func_0200052c
	.thumb_func
Func_0200052c:
	push {lr}
	bl 0x020090e8
	movs r2, #2
	movs r1, #0
	movs r0, #12
	bl 0x02009150
	ldr r0, [pc, #32]
	bl 0x020090c8
	ldr r0, [pc, #32]
	bl 0x020090c8
	ldr r0, [pc, #28]
	bl 0x02009168
	movs r0, #12
	movs r1, #0
	bl 0x02009178
	bl 0x020090f0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000306
	.4byte 0x00000868
	.4byte 0x00001caf
	.global Func_0200056c
	.thumb_func
Func_0200056c:
	push {lr}
	ldr r0, [pc, #24]
	bl 0x02009168
	movs r0, #13
	movs r1, #0
	movs r2, #2
	bl 0x02009158
	movs r0, #13
	bl 0x020084b4
	pop {r0}
	bx r0
	.4byte 0x00001cb0
	.global Func_0200058c
	.thumb_func
Func_0200058c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #14
	bl 0x02009100
	adds r5, r0, #0
	movs r3, #6
	ldrsh r2, [r5, r3]
	adds r6, r5, #0
	adds r6, #100
	ldrh r3, [r6]
	mov r8, r2
	ldr r2, [pc, #48]
	orrs r2, r3
	strh r2, [r6]
	bl 0x020090e8
	ldr r7, [pc, #40]
	adds r0, r7, #0
	bl 0x02009168
	movs r0, #14
	movs r1, #0
	bl 0x02009138
	movs r0, #14
	movs r1, #0
	movs r2, #2
	bl 0x02009158
	movs r0, #192
	lsls r0, r0, #2
	bl 0x020090c0
	cmp r0, #0
	bne .L_0200058c_0
	b .L_0200058c_1
	.4byte 0x00000002
	.4byte 0x00001cb1
.L_0200058c_1:
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #60
	bl 0x02009190
	movs r0, #14
	movs r1, #0
	movs r2, #10
	bl 0x02009180
	movs r0, #14
	movs r1, #0
	movs r2, #10
	bl 0x02009180
	movs r0, #192
	lsls r0, r0, #2
	bl 0x020090c8
.L_0200058c_0:
	adds r0, r7, #2
	bl 0x02009168
	movs r1, #0
	movs r0, #14
	movs r2, #10
	bl 0x02009180
	mov r2, r8
	strh r2, [r5, #6]
	movs r0, #1
	bl 0x02009060
	bl 0x020090f0
	movs r3, #1
	strh r3, [r6]
	ldr r0, [pc, #16]
	bl 0x020090c8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000307
	.global Func_02000640
	.thumb_func
Func_02000640:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	movs r0, #15
	bl 0x02009100
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #100
	movs r2, #6
	ldrsh r1, [r5, r2]
	ldr r3, [pc, #56]
	ldrh r2, [r6]
	orrs r3, r2
	strh r3, [r6]
	mov r8, r1
	movs r1, #0
	mov r10, r1
	bl 0x020090e8
	ldr r0, [pc, #44]
	bl 0x02009168
	movs r0, #15
	movs r1, #0
	bl 0x02009138
	movs r0, #15
	movs r1, #0
	movs r2, #2
	bl 0x02009158
	movs r1, #0
	movs r0, #15
	movs r2, #10
	bl 0x02009180
.L_0200068c:
	mov r2, r8
	strh r2, [r5, #6]
	movs r0, #1
	b .L_0200068c_0
	.2byte 0x0002
	.2byte 0x0000
	.2byte 0x1cb4
	.2byte 0x0000
.L_0200068c_0:
	bl 0x02009060
	bl 0x020090f0
	mov r3, r10
	strh r3, [r6]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_020006b4
	.thumb_func
Func_020006b4:
	push {lr}
	bl 0x020090e8
	ldr r0, [pc, #76]
	bl 0x02009168
	movs r0, #16
	movs r1, #0
	movs r2, #2
	bl 0x02009158
	movs r1, #0
	movs r0, #16
	bl 0x02009170
	movs r0, #0
	movs r1, #0
	bl 0x020090f8
	cmp r0, #0
	beq .L_020006b4_0
	ldr r3, [pc, #44]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_020006b4_0:
	movs r1, #0
	movs r0, #16
	bl 0x02009178
	movs r0, #194
	lsls r0, r0, #2
	bl 0x020090c8
	bl 0x020090f0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001cb5
	.4byte 0x03001ebc
	.global Func_02000710
	.thumb_func
Func_02000710:
	push {lr}
	bl 0x020090e8
	movs r1, #1
	movs r0, #8
	bl 0x02009148
	movs r0, #20
	bl 0x020090e0
	movs r1, #0
	movs r2, #20
	movs r0, #8
	bl 0x02009150
	ldr r0, [pc, #28]
	bl 0x020090c8
	ldr r0, [pc, #24]
	bl 0x02009168
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x02009180
	bl 0x020090f0
	pop {r0}
	bx r0
	.4byte 0x00000305
	.4byte 0x00001cab
	.global Func_02000754
	.thumb_func
Func_02000754:
	push {r5, lr}
	ldr r0, [pc, #40]
	bl 0x02009168
	movs r0, #11
	bl 0x02009100
	movs r3, #1
	adds r0, #91
	strb r3, [r0]
	movs r0, #11
	bl 0x020084b4
	movs r0, #11
	bl 0x02009100
	movs r5, #0
	adds r0, #91
	strb r5, [r0]
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00001cbd
	.global Func_02000784
	.thumb_func
Func_02000784:
	push {lr}
	bl 0x020090e8
	movs r1, #1
	movs r0, #12
.L_0200078e:
	bl 0x02009148
	movs r0, #20
	bl 0x020090e0
	movs r1, #0
	movs r2, #20
	movs r0, #12
	bl 0x02009150
	ldr r0, [pc, #36]
	bl 0x020090c8
	ldr r0, [pc, #32]
	bl 0x020090c8
	ldr r0, [pc, #32]
	bl 0x02009168
	movs r0, #12
	movs r1, #0
	movs r2, #20
	bl 0x02009180
	bl 0x020090f0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000306
	.4byte 0x00000868
	.4byte 0x00001caf
	.global Func_020007d4
	.thumb_func
Func_020007d4:
	push {r5, lr}
	ldr r0, [pc, #40]
	bl 0x02009168
	movs r0, #13
	bl 0x02009100
	movs r3, #1
	adds r0, #91
	strb r3, [r0]
	movs r0, #13
	bl 0x020084b4
	movs r0, #13
	bl 0x02009100
	movs r5, #0
	adds r0, #91
	strb r5, [r0]
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00001cbf
	.global Func_02000804
	.thumb_func
Func_02000804:
	push {lr}
	movs r0, #14
	bl 0x02009100
	adds r0, #100
	ldrh r2, [r0]
	ldr r3, [pc, #32]
	orrs r3, r2
	strh r3, [r0]
	bl 0x020090e8
	ldr r0, [pc, #28]
	bl 0x020090c0
	cmp r0, #0
	beq .L_02000804_0
	ldr r0, [pc, #20]
	bl 0x02009168
	movs r0, #14
	bl 0x020084b4
	b .L_02000804_1
	.2byte 0x0000
	.4byte 0x00000002
	.4byte 0x00000307
	.4byte 0x00001cc0
.L_02000804_0:
	bl 0x0200858c
	ldr r0, [pc, #24]
	bl 0x020090c8
.L_02000804_1:
	bl 0x020090f0
	movs r0, #14
	bl 0x02009100
	movs r3, #1
	adds r0, #100
	strh r3, [r0]
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000307
	.global Func_02000864
	.thumb_func
Func_02000864:
	push {r5, lr}
	movs r0, #15
	bl 0x02009100
	adds r0, #100
	ldrh r2, [r0]
	ldr r3, [pc, #36]
	orrs r3, r2
	strh r3, [r0]
	bl 0x020090e8
	ldr r0, [pc, #32]
	bl 0x02009168
	movs r0, #15
	bl 0x020084b4
	bl 0x020090f0
	movs r0, #15
	bl 0x02009100
	movs r5, #0
	adds r0, #100
	strh r5, [r0]
	b .L_02000864_0
	.4byte 0x00000002
	.4byte 0x00001cc1
.L_02000864_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020008a8
	.thumb_func
Func_020008a8:
	push {r5, lr}
	movs r0, #194
	lsls r0, r0, #2
	bl 0x020090c0
	adds r5, r0, #0
	cmp r5, #0
	bne .L_020008a8_0
	bl 0x020090e8
	movs r0, #16
	bl 0x02009100
	movs r3, #1
	adds r0, #91
	strb r3, [r0]
	movs r1, #1
	movs r0, #16
	bl 0x02009138
	movs r1, #1
	movs r0, #16
	bl 0x02009148
	movs r0, #20
	bl 0x020090e0
	ldr r0, [pc, #132]
	bl 0x02009168
	movs r0, #16
	movs r1, #0
	movs r2, #2
	bl 0x02009158
	movs r1, #0
	movs r0, #16
	bl 0x02009170
	movs r0, #0
	movs r1, #0
	bl 0x020090f8
	cmp r0, #0
	beq .L_020008a8_1
	ldr r3, [pc, #100]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_020008a8_1:
	movs r1, #0
	movs r0, #16
	bl 0x02009178
	movs r0, #16
	bl 0x02009100
	adds r0, #91
	strb r5, [r0]
	movs r1, #2
	movs r0, #16
	bl 0x02009110
	bl 0x020090f0
	movs r0, #194
	lsls r0, r0, #2
	bl 0x020090c8
	b .L_020008a8_2
.L_020008a8_0:
	ldr r0, [pc, #48]
	bl 0x02009168
	movs r0, #16
	bl 0x02009100
	movs r3, #1
	adds r0, #91
	strb r3, [r0]
	movs r0, #16
	bl 0x020084b4
	movs r0, #16
	bl 0x02009100
	movs r5, #0
	adds r0, #91
	strb r5, [r0]
.L_020008a8_2:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00001cb5
	.4byte 0x03001ebc
	.4byte 0x00001cc2
	.global Func_02000970
	.thumb_func
Func_02000970:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	mov r8, r2
	adds r6, r1, #0
	mov r10, r3
	bl 0x02009100
	movs r1, #192
	movs r2, #192
	adds r7, r0, #0
	lsls r1, r1, #10
	adds r0, r5, #0
	lsls r2, r2, #9
	bl 0x02009108
	movs r3, #128
	lsls r3, r3, #8
	mov r2, r10
	str r3, [r7, #72]
	movs r3, #0
	str r3, [r7, #68]
	str r2, [r7, #40]
	adds r0, r7, #0
	movs r1, #0
	bl 0x020090a0
	adds r0, r5, #0
	adds r1, r6, #0
	mov r2, r8
	bl 0x02009118
	mov r3, r8
	lsls r3, r3, #16
	mov r8, r3
	lsls r6, r6, #16
	adds r0, r5, #0
	adds r1, r6, #0
	mov r2, r8
	bl 0x02009130
	movs r5, #60
	b .L_02000970_0
	.2byte 0x3d01
.L_02000970_0:
	cmp r5, #0
	beq 0x020089de
	movs r0, #1
	bl 0x02009060
.L_020009d6:
	movs r2, #42
	ldrsh r3, [r7, r2]
	cmp r3, #0
	bne 0x020089ca
	adds r0, r7, #0
	movs r1, #1
	bl 0x020090a0
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_020009f8
	.thumb_func
Func_020009f8:
	push {lr}
	bl 0x020090e8
	movs r0, #100
	bl 0x020091b0
	movs r0, #40
	bl 0x020090e0
	ldr r0, [pc, #112]
	bl 0x020090c0
	cmp r0, #0
	bne .L_020009f8_0
	movs r1, #129
	movs r0, #21
	lsls r1, r1, #1
	bl 0x02009198
	movs r1, #4
	movs r2, #0
	movs r0, #21
	bl 0x02009140
	movs r0, #12
	bl 0x020090e0
	movs r1, #4
	movs r2, #0
	movs r0, #21
	bl 0x02009140
	movs r0, #20
	bl 0x020090e0
	movs r1, #196
	movs r3, #224
	lsls r3, r3, #11
	lsls r1, r1, #1
	movs r2, #104
	movs r0, #21
	bl 0x02008970
	movs r0, #20
	bl 0x020090e0
	movs r1, #204
	movs r0, #21
	lsls r1, r1, #1
	movs r2, #104
	bl 0x02009128
	movs r1, #204
	movs r0, #21
	lsls r1, r1, #1
	movs r2, #120
	bl 0x02009128
	ldr r0, [pc, #12]
	bl 0x020090c8
.L_020009f8_0:
	bl 0x020090f0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000867
	.global Func_02000a80
	.thumb_func
Func_02000a80:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r6, r1, #0
	mov r8, r2
	movs r1, #128
	movs r2, #128
	adds r5, r0, #0
	lsls r1, r1, #8
	movs r0, #0
	lsls r2, r2, #7
	bl 0x02009108
	adds r2, r6, #0
	movs r0, #0
	adds r1, r5, #0
	bl 0x02009120
	ldr r3, [pc, #28]
	movs r2, #228
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #16
	str r2, [r3]
	mov r0, r8
	bl 0x020091a0
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.global Func_02000ac8
	.thumb_func
Func_02000ac8:
	push {lr}
	movs r0, #158
	bl 0x020091b0
	ldr r0, [pc, #24]
	movs r1, #56
	movs r2, #19
	bl 0x02009088
	movs r0, #204
	movs r1, #160
	lsls r0, r0, #1
	lsls r1, r1, #1
	movs r2, #5
	bl 0x02008a80
	pop {r0}
	bx r0
	.4byte 0x02009740
	.global Func_02000af0
	.thumb_func
Func_02000af0:
	push {lr}
	movs r0, #158
	bl 0x020091b0
	ldr r0, [pc, #24]
	movs r1, #50
	movs r2, #18
	bl 0x02009088
	movs r0, #156
	movs r1, #152
	lsls r0, r0, #1
	lsls r1, r1, #1
	movs r2, #6
	bl 0x02008a80
	pop {r0}
	bx r0
	.4byte 0x02009756
	.global Func_02000b18
	.thumb_func
Func_02000b18:
	push {lr}
	movs r0, #158
	bl 0x020091b0
	ldr r0, [pc, #24]
	movs r1, #44
	movs r2, #17
	bl 0x02009088
	movs r1, #144
	lsls r1, r1, #1
	movs r0, #216
	movs r2, #7
	bl 0x02008a80
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200976c
	.global Func_02000b40
	.thumb_func
Func_02000b40:
	push {r5, r6, lr}
	movs r0, #0
	sub sp, #8
	bl 0x02009100
	adds r5, r0, #0
	movs r0, #158
	ldr r6, [r5, #80]
	bl 0x020091b0
	ldr r0, [pc, #64]
	movs r1, #54
	movs r2, #13
	bl 0x02009088
	movs r3, #23
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r5, #35
	movs r0, #33
	movs r1, #20
	movs r2, #1
	movs r3, #3
	bl 0x02009098
	ldrb r2, [r5]
	movs r3, #254
	ands r3, r2
	strb r3, [r5]
	ldrb r3, [r6, #9]
	movs r2, #12
	orrs r3, r2
	movs r0, #188
	strb r3, [r6, #9]
	lsls r0, r0, #1
	movs r1, #224
	movs r2, #8
	bl 0x02008a80
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x02009782
	.global Func_02000b9c
	.thumb_func
Func_02000b9c:
	push {r5, r6, lr}
	movs r0, #0
	sub sp, #8
	bl 0x02009100
	adds r5, r0, #0
	movs r0, #158
	ldr r6, [r5, #80]
	bl 0x020091b0
	ldr r0, [pc, #64]
	movs r1, #49
	movs r2, #10
	bl 0x02009088
	movs r3, #18
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r5, #35
	movs r0, #33
	movs r1, #20
	movs r2, #1
	movs r3, #3
	bl 0x02009098
	ldrb r2, [r5]
	movs r3, #254
	ands r3, r2
	strb r3, [r5]
	ldrb r3, [r6, #9]
	movs r2, #12
	orrs r3, r2
	movs r0, #148
	strb r3, [r6, #9]
	lsls r0, r0, #1
	movs r1, #176
	movs r2, #9
	bl 0x02008a80
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x02009798
	.global Func_02000bf8
	.thumb_func
Func_02000bf8:
	push {lr}
	movs r0, #158
	bl 0x020091b0
	ldr r0, [pc, #20]
	movs r1, #38
	movs r2, #6
	bl 0x02009088
	movs r0, #120
	movs r1, #144
	movs r2, #10
	bl 0x02008a80
	pop {r0}
	bx r0
	.4byte 0x020097ae
	.global Func_02000c1c
	.thumb_func
Func_02000c1c:
	push {r5, r6, lr}
	mov r6, r8
.L_02000c20:
	push {r6}
	movs r0, #0
	sub sp, #8
	bl 0x02009100
	adds r6, r0, #0
	ldr r2, [r6, #80]
	movs r0, #188
	mov r8, r2
	bl 0x020091b0
	movs r5, #2
	movs r0, #42
	movs r1, #33
	movs r2, #34
	movs r3, #16
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009090
	movs r1, #35
	movs r2, #36
	movs r3, #16
	movs r0, #42
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009090
	movs r0, #4
	bl 0x020090e0
	movs r0, #40
	movs r1, #33
	movs r2, #34
	movs r3, #16
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009090
	movs r1, #35
	movs r2, #36
	movs r3, #16
	movs r0, #40
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009090
	movs r0, #4
	bl 0x020090e0
	movs r3, #3
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r6, #35
	movs r0, #33
	movs r1, #21
	movs r2, #2
	movs r3, #2
	bl 0x02009098
	ldrb r2, [r6]
	movs r3, #254
	ands r3, r2
.L_02000ca0:
	mov r2, r8
	strb r3, [r6]
	ldrb r3, [r2, #9]
	movs r2, #12
	orrs r3, r2
	movs r1, #136
	mov r2, r8
	strb r3, [r2, #9]
	lsls r1, r1, #1
	movs r0, #64
	movs r2, #11
	bl 0x02008a80
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000cc8
	.thumb_func
Func_02000cc8:
	push {r5, r6, lr}
	movs r0, #0
	sub sp, #8
	bl 0x02009100
	adds r5, r0, #0
	movs r0, #158
	ldr r6, [r5, #80]
	bl 0x020091b0
	ldr r0, [pc, #64]
	movs r1, #35
	movs r2, #9
.L_02000ce2:
	bl 0x02009088
	movs r3, #4
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r5, #35
	movs r0, #33
	movs r1, #20
	movs r2, #1
	movs r3, #3
	bl 0x02009098
	ldrb r2, [r5]
	movs r3, #254
	ands r3, r2
	strb r3, [r5]
	ldrb r3, [r6, #9]
	movs r2, #12
	orrs r3, r2
	strb r3, [r6, #9]
	movs r0, #72
	movs r1, #160
	movs r2, #12
	bl 0x02008a80
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x97c4
	.2byte 0x0200
	.global Func_02000d24
	.thumb_func
Func_02000d24:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	sub sp, #8
	bl 0x020090c8
	movs r3, #23
	movs r2, #26
	str r3, [sp, #0]
	str r2, [sp, #4]
.L_02000d38:
	movs r0, #55
	movs r1, #26
	movs r2, #4
	movs r3, #2
	bl 0x02009098
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000d4c
	.thumb_func
Func_02000d4c:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	sub sp, #8
	bl 0x020090d0
	movs r3, #23
	movs r2, #26
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #23
	movs r1, #23
	movs r2, #4
	movs r3, #2
	bl 0x02009098
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000d74
	.thumb_func
Func_02000d74:
	push {lr}
	movs r0, #21
	movs r1, #0
	movs r2, #4
	bl 0x020091a8
	pop {r0}
	bx r0
	.global Func_02000d84
	.thumb_func
Func_02000d84:
	push {lr}
	movs r0, #231
	bl 0x020090d8
	bl 0x020090e8
	movs r0, #10
	bl 0x020090e0
	movs r0, #18
	movs r1, #2
	bl 0x02009148
	movs r0, #18
	ldr r1, [pc, #128]
	ldr r2, [pc, #132]
	bl 0x02009108
	movs r2, #204
	movs r1, #216
	lsls r2, r2, #1
	movs r0, #18
	bl 0x02009128
	movs r0, #10
	bl 0x020090e0
	movs r1, #128
	movs r0, #18
	lsls r1, r1, #7
	movs r2, #20
	bl 0x02009188
	movs r1, #6
	movs r2, #0
	movs r0, #18
	bl 0x02009140
	movs r0, #30
	bl 0x020090e0
	movs r1, #6
	movs r2, #0
	movs r0, #18
	bl 0x02009140
	movs r0, #30
	bl 0x020090e0
	movs r1, #6
	movs r2, #0
	movs r0, #18
	bl 0x02009140
	movs r0, #30
	bl 0x020090e0
	movs r2, #196
	movs r1, #216
	lsls r2, r2, #1
	movs r0, #18
	bl 0x02009128
	movs r0, #10
.L_02000e04:
	bl 0x020090e0
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #20
	movs r0, #18
	bl 0x02009188
	ldr r0, [pc, #20]
	bl 0x020090c8
	bl 0x020090f0
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.4byte 0x00000858
	.global Func_02000e30
	.thumb_func
Func_02000e30:
	push {lr}
	sub sp, #8
	movs r3, #13
	movs r2, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #41
	movs r1, #43
	movs r2, #1
	movs r3, #1
	bl 0x02009098
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #40
	movs r1, #42
	movs r2, #12
	movs r3, #22
	bl 0x02009090
	sub sp, #-8
	pop {r0}
	bx r0
	.global Func_02000e60
	.thumb_func
Func_02000e60:
	push {lr}
	sub sp, #8
	movs r3, #13
	movs r2, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #37
	movs r1, #43
	movs r2, #1
	movs r3, #1
	bl 0x02009098
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #36
	movs r1, #42
	movs r2, #12
	movs r3, #22
	bl 0x02009090
	sub sp, #-8
	pop {r0}
	bx r0
	.global Func_02000e90
	.thumb_func
Func_02000e90:
	push {lr}
	bl 0x020090e8
	ldr r0, [pc, #92]
	bl 0x02009168
	movs r0, #18
	movs r1, #0
	bl 0x02009138
	movs r2, #0
	movs r1, #0
	movs r0, #18
	bl 0x02009158
	movs r0, #2
	bl 0x020090e0
	movs r0, #18
	movs r1, #0
	bl 0x02009178
	movs r1, #1
	movs r0, #18
	bl 0x02009138
	movs r0, #231
	bl 0x020090b8
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	beq .L_02000e90_0
	ldr r0, [pc, #36]
	bl 0x020090c0
	cmp r0, #0
	bne .L_02000e90_0
	ldr r3, [pc, #28]
	movs r1, #185
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
.L_02000e90_0:
	bl 0x020090f0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001342
	.4byte 0x00000858
	.4byte 0x03001ebc
	.global Func_02000f00
	.thumb_func
Func_02000f00:
	push {lr}
	bl 0x020090e8
	ldr r0, [pc, #24]
	movs r1, #1
	bl 0x020090b0
	ldr r0, [pc, #20]
	movs r1, #1
	bl 0x020090b0
	bl 0x020090f0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000947
	.4byte 0x000029dc
	.global Func_02000f28
	.thumb_func
Func_02000f28:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020097dc
	.global Func_02000f30
	.thumb_func
Func_02000f30:
	push {r5, r6, lr}
	movs r0, #128
	lsls r0, r0, #2
	sub sp, #8
	bl 0x020090c0
	cmp r0, #0
	beq .L_02000f30_0
	movs r3, #23
	movs r2, #26
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #55
	movs r1, #26
	movs r2, #4
	movs r3, #2
	bl 0x02009098
.L_02000f30_0:
	movs r0, #128
	movs r2, #210
	lsls r2, r2, #17
	movs r1, #0
	movs r3, #223
	lsls r0, r0, #16
	bl 0x020080a0
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r1, #41
	movs r2, #8
	movs r3, #45
	movs r0, #45
	bl 0x02009090
	movs r0, #1
	bl 0x02009060
	movs r0, #14
	bl 0x02009100
	ldr r5, [pc, #188]
	str r5, [r0, #108]
	movs r0, #14
	bl 0x02009100
	movs r3, #1
	adds r0, #100
	strh r3, [r0]
	movs r0, #15
	bl 0x02009100
	str r5, [r0, #108]
	movs r0, #15
	bl 0x02009100
	movs r6, #0
	adds r0, #100
	strh r6, [r0]
	ldr r0, [pc, #156]
	bl 0x020090c0
	cmp r0, #0
	beq .L_02000f30_2
	movs r1, #216
	movs r2, #196
	movs r0, #18
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x02009130
.L_02000f30_2:
	ldr r3, [pc, #136]
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bgt .L_02000f30_3
	movs r0, #52
	bl 0x020090c0
	cmp r0, #0
	bne .L_02000f30_3
	ldr r0, [pc, #112]
	bl 0x020090c0
	cmp r0, #0
	bne .L_02000f30_3
	ldr r0, [pc, #108]
	bl 0x020090d0
.L_02000f30_3:
	ldr r0, [pc, #100]
	bl 0x020090c0
	cmp r0, #0
	beq .L_02000f30_4
	movs r0, #52
	bl 0x020090c0
	cmp r0, #0
	bne .L_02000f30_4
	movs r1, #204
	movs r2, #240
	movs r0, #21
.L_02000f30_1:
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl 0x02009130
.L_02000f30_4:
	ldr r3, [pc, #60]
	movs r1, #225
	lsls r1, r1, #1
	adds r5, r3, r1
	movs r1, #0
	ldrsh r3, [r5, r1]
	ldrh r2, [r5]
	cmp r3, #11
	bne .L_02000f30_5
	ldr r0, [pc, #52]
	bl 0x020090d0
	ldrh r2, [r5]
.L_02000f30_5:
	lsls r3, r2, #16
	movs r2, #208
	lsls r2, r2, #12
	cmp r3, r2
	bne .L_02000f30_6
	movs r0, #144
	lsls r0, r0, #1
	bl 0x020090d0
.L_02000f30_6:
	movs r0, #0
	sub sp, #-8
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x02008401
	.4byte 0x00000858
	.4byte 0x02000240
	.4byte 0x00000109
	.4byte 0x00000867
	.4byte 0x0000012f
	.include "games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_MURA_SAI/IMPORT.INC"
@ The compiler library links here from its licensed container.
	.section .text.part1,"ax",%progbits
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
	.4byte 0x020091f4
	.4byte 0x0200922c
	.4byte 0x02009264
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x80010000
	.4byte 0x00000003
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x80010000
	.4byte 0x00000003
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000190
	.4byte 0xc00001d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000000e0
	.4byte 0x4000008a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000198
	.4byte 0x40000158
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000138
	.4byte 0x40000148
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x000000d8
	.4byte 0x40000138
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00000178
	.4byte 0x400000f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x00000128
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000078
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x00000040
	.4byte 0x40000126
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x00000046
	.4byte 0x400000b7
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000d
	.4byte 0x000000d9
	.4byte 0x40000199
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000e
	.4byte 0x000001a4
	.4byte 0x40000088
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00103002
	.4byte 0x0022c002
	.4byte 0x00505018
	.4byte 0x00606018
	.4byte 0x00707018
	.4byte 0x00808018
	.4byte 0x00909018
	.4byte 0x00a0a018
	.4byte 0x00b0b009
	.4byte 0x00c0a016
	.4byte 0x00d0a060
	.4byte 0x00e01062
	.4byte 0x000001ff
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x017d0000
	.4byte 0x00000000
	.4byte 0x01560000
	.4byte 0x00003000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x018c0000
	.4byte 0x00015000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00960000
	.4byte 0x0001b000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00f40000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00005000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x007c0000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00002000
	.4byte 0xffff006f
	.4byte 0x00000002
	.4byte 0x00f40000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x0000b000
	.4byte 0xffff006b
	.4byte 0x020092a8
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00028000
	.4byte 0xffff0066
	.4byte 0x0200937c
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x01a40000
	.4byte 0x00020000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x014c0000
	.4byte 0x00000000
	.4byte 0x019c0000
	.4byte 0x0000b000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x005c0000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00003000
	.4byte 0xffff0068
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00005000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00003000
	.4byte 0xffff00f8
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x0034005a
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00340023
	.4byte 0x00020002
	.4byte 0x00210004
	.4byte 0x00020034
	.4byte 0x00040002
	.4byte 0x0023ffff
	.4byte 0x00020032
	.4byte 0x00040002
	.4byte 0x00320021
	.4byte 0x00020002
	.4byte 0xffff0004
	.4byte 0x00300023
	.4byte 0x00020002
	.4byte 0x00210004
	.4byte 0x00020030
	.4byte 0x00040002
	.4byte 0x002affff
	.4byte 0x00020030
	.4byte 0x00040002
	.4byte 0x00300028
	.4byte 0x00020002
	.4byte 0xffff0004
	.4byte 0x002e002a
	.4byte 0x00020002
	.4byte 0x00280004
	.4byte 0x0002002e
	.4byte 0x00040002
	.4byte 0x0023ffff
	.4byte 0x0002002e
	.4byte 0x00040002
	.4byte 0x002e0021
	.4byte 0x00020002
	.4byte 0xffff0004
	.4byte 0x00210023
	.4byte 0x00020002
	.4byte 0x00210004
	.4byte 0x00020021
	.4byte 0x00040002
	.4byte 0x0000ffff
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0015
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0028
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff0029
	.4byte 0x0000000e
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02008d25
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008d4d
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008ac9
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008af1
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008b19
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008b41
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008b9d
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x02008bf9
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x02008c1d
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte 0x02008cc9
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020084d9
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001cac
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001cad
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0200850d
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0200852d
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200856d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0200858d
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008641
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x020086b5
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001cb8
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008e91
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001cf1
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008d75
	.4byte 0x00000003
	.4byte 0xffff001e
	.4byte 0x020089f9
	.4byte 0x00008d15
	.4byte 0x03050408
	.4byte 0x02008711
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001cba
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001cbb
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001cbc
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x02008755
	.4byte 0x00008d15
	.4byte 0x0306040c
	.4byte 0x02008785
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001cbe
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x020087d5
	.4byte 0x00008d15
	.4byte 0x0307040e
	.4byte 0x02008805
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x02008805
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x02008865
	.4byte 0x00008d15
	.4byte 0x03080410
	.4byte 0x020088a9
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x020088a9
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001cc3
	.4byte 0x00008d15
	.4byte 0x18580012
	.4byte 0x000013ab
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001cc4
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001cf5
	.4byte 0x00000003
	.4byte 0xffff005a
	.4byte 0x02008f01
	.4byte 0x000000d3
	.4byte 0x0f4a0065
	.4byte 0x001000b5
	.4byte 0x0000e714
	.4byte 0x08580012
	.4byte 0x02008d85
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008e61
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008e31
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
