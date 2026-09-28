.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/ENTRY.INC"
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
	bl 0x0200dc6c
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
	bl 0x0200dcac
	adds r0, r5, #0
	movs r1, #14
	bl 0x0200ddec
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200dcb4
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
	bl 0x0200dc6c
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
	bl 0x0200dcac
	adds r0, r5, #0
	movs r1, #15
	bl 0x0200ddec
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
	bl 0x0200dd4c
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
	bl 0x0200dc6c
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
	bl 0x0200dc5c
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl 0x0200dc64
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
	bl 0x0200ddec
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
	bl 0x0200dbec
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
	bl 0x0200dbec
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200013c_9:
	bl 0x0200dbec
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
	bl 0x0200dc5c
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl 0x0200dc64
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
	.4byte 0x0200dfb8
	.4byte 0x02008105
	.4byte 0xffff0000
	.global Func_02000314
	.thumb_func
Func_02000314:
	push {r5, lr}
	adds r5, r0, #0
	adds r5, #100
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #2
	beq .L_02000314_0
	cmp r3, #2
	bgt .L_02000314_1
	cmp r3, #0
	beq .L_02000314_2
	b .L_02000314_3
.L_02000314_1:
	cmp r3, #4
	beq .L_02000314_4
	cmp r3, #6
	bne .L_02000314_3
	ldr r3, [r0, #24]
	ldr r2, [pc, #92]
	adds r3, r3, r2
	str r3, [r0, #24]
	movs r2, #128
	ldr r3, [r0, #28]
	lsls r2, r2, #5
	b .L_02000314_5
.L_02000314_4:
	ldr r3, [r0, #24]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [pc, #72]
	b .L_02000314_6
.L_02000314_0:
	ldr r3, [r0, #24]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [pc, #60]
.L_02000314_6:
	ldr r3, [r0, #28]
.L_02000314_5:
	adds r3, r3, r2
	str r3, [r0, #28]
	b .L_02000314_3
.L_02000314_2:
	ldr r3, [r0, #24]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [pc, #40]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	bl 0x0200dc14
	movs r1, #80
	bl 0x0200dbf4
	adds r0, #80
	strh r0, [r5]
.L_02000314_3:
	ldrh r3, [r5]
	subs r3, #1
	strh r3, [r5]
	movs r0, #1
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0xffffe000
	.4byte 0xfffff800
	.4byte 0xfffffc00
	.global Func_020003a0
	.thumb_func
Func_020003a0:
	push {r5, r6, lr}
	adds r5, r0, #0
	ldr r4, [r5, #68]
	ldr r3, [r5, #8]
	adds r3, r3, r4
	ldr r2, [r5, #72]
	str r3, [r5, #8]
	ldr r3, [r5, #12]
	adds r3, r3, r2
	ldr r6, [r5, #76]
	str r3, [r5, #12]
	ldr r3, [r5, #16]
	adds r3, r3, r6
	sub sp, #4
	str r3, [r5, #16]
	adds r0, r4, #0
	movs r1, #22
	str r4, [sp, #0]
	bl 0x0200dbec
	ldr r4, [sp, #0]
	subs r4, r4, r0
	str r4, [r5, #68]
	adds r0, r6, #0
	movs r1, #20
	bl 0x0200dbec
	ldr r3, [r5, #24]
	ldr r2, [r5, #48]
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r2, [r5, #52]
	ldr r3, [r5, #28]
	subs r6, r6, r0
	adds r3, r3, r2
	str r6, [r5, #76]
	str r3, [r5, #28]
	ldr r1, [r5, #80]
	adds r5, #100
	ldrh r3, [r1, #30]
	ldrh r2, [r5]
	adds r3, r3, r2
	strh r3, [r1, #30]
	sub sp, #-4
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000400
	.thumb_func
Func_02000400:
	push {lr}
	ldr r1, [r0, #80]
	ldr r0, [pc, #20]
	ldrh r2, [r1, #30]
	adds r3, r2, r0
	lsls r3, r3, #16
	cmp r3, #0
	bge .L_02000400_0
	ldr r0, [pc, #12]
	adds r3, r2, r0
	strh r3, [r1, #30]
.L_02000400_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000ffff
	.4byte 0xfffff600
	.global Func_02000424
	.thumb_func
Func_02000424:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #102
	movs r1, #0
	ldrsh r3, [r6, r1]
	ldrh r2, [r6]
	cmp r3, #0
	bne .L_02000424_0
	bl 0x0200dc14
	ldrh r3, [r5, #6]
	lsls r0, r0, #15
	lsrs r0, r0, #16
	adds r3, r3, r0
	strh r3, [r5, #6]
	bl 0x0200dc14
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	strh r3, [r6]
	cmp r3, #0
	beq .L_02000424_1
	adds r2, r3, #0
.L_02000424_0:
	subs r3, r2, #1
	strh r3, [r6]
.L_02000424_1:
	movs r0, #1
	pop {r5, r6}
	pop {r1}
	bx r1
	.global Func_02000464
	.thumb_func
Func_02000464:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200e3d4
	.global Func_0200046c
	.thumb_func
Func_0200046c:
	movs r0, #0
	bx lr
	.global Func_02000470
	.thumb_func
Func_02000470:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200e464
	.global Func_02000478
	.thumb_func
Func_02000478:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200e478
	.global Func_02000480
	.thumb_func
Func_02000480:
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
	bl 0x0200dee0
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x030001d8
	.global Func_020004bc
	.thumb_func
Func_020004bc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, [pc, #76]
	movs r2, #0
	movs r0, #0
	ldr r5, [r3]
	mov r8, r2
	bl 0x0200dd4c
	movs r7, #160
	lsls r7, r7, #2
	mov r10, r0
	movs r6, #8
	adds r5, #52
.L_020004bc_1:
	ldmia r5!, {r1}
	cmp r1, #0
	beq .L_020004bc_0
	ldr r3, [r1, #80]
	ldr r3, [r3, #40]
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #242
	bne .L_020004bc_0
	mov r0, r10
	adds r1, #8
	adds r0, #8
	bl 0x02008480
	cmp r0, r7
	bge .L_020004bc_0
	adds r7, r0, #0
	mov r8, r6
.L_020004bc_0:
	adds r6, #1
	cmp r6, #65
	bls .L_020004bc_1
	mov r0, r8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.global Func_02000518
	.thumb_func
Func_02000518:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #0
	sub sp, #12
	bl 0x0200dd4c
	adds r5, r0, #0
	ldrh r1, [r5, #6]
	movs r3, #128
	lsls r3, r3, #5
	adds r1, r1, r3
	adds r7, r5, #0
	movs r3, #224
	lsls r3, r3, #8
	adds r7, #85
	ands r1, r3
	ldrb r3, [r7]
	ldr r0, [pc, #168]
	mov r8, r3
	ldr r3, [r5, #8]
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r0
	mov r6, sp
	adds r3, r3, r2
	str r3, [r6]
	ldr r3, [r5, #12]
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	ands r3, r0
	movs r0, #128
	adds r3, r3, r2
	lsls r0, r0, #14
	adds r2, r6, #0
	str r3, [r6, #8]
	bl 0x0200dc2c
	adds r0, r5, #0
	adds r1, r6, #0
	bl 0x0200dca4
	cmp r0, #0
	bne .L_02000518_0
	movs r0, #148
	lsls r0, r0, #2
	bl 0x0200dd1c
	bl 0x020086c0
	movs r1, #6
	adds r0, r5, #0
	bl 0x0200dc5c
	movs r0, #6
	bl 0x0200dbfc
	movs r1, #7
	adds r0, r5, #0
	bl 0x0200dc5c
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #52]
	movs r0, #152
	bl 0x0200decc
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldrb r2, [r7]
	movs r3, #126
	ands r3, r2
	strb r3, [r7]
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200dcac
	movs r3, #10
	ldrsh r2, [r6, r3]
	movs r3, #2
	ldrsh r1, [r6, r3]
	movs r0, #0
	bl 0x0200dd8c
	adds r0, r5, #0
	movs r1, #6
	bl 0x0200dc5c
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200dcac
	mov r3, r8
	strb r3, [r7]
.L_02000518_0:
	sub sp, #-12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0xfff00000
	.global Func_020005ec
	.thumb_func
Func_020005ec:
	push {r5, r6, r7, lr}
	movs r0, #0
	bl 0x0200dd4c
	adds r7, r0, #0
	bl 0x0200dd34
	bl 0x020084bc
	ldr r5, [pc, #180]
	str r0, [r5]
	cmp r0, #0
	beq .L_020005ec_0
	movs r0, #148
	lsls r0, r0, #2
	bl 0x0200dd14
	ldr r0, [r5]
	bl 0x0200dd4c
	adds r6, r0, #0
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r1, r7, #0
	adds r1, #85
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	ldr r2, [pc, #140]
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r3, [r7, #20]
	adds r3, r3, r2
	str r3, [r7, #20]
	movs r0, #2
	bl 0x0200dbfc
	ldr r2, [pc, #116]
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r3, [r7, #20]
	adds r3, r3, r2
	str r3, [r7, #20]
	movs r0, #10
	bl 0x0200dbfc
	movs r5, #128
	ldr r3, [r6, #12]
	lsls r5, r5, #10
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r5
	str r3, [r7, #12]
	ldr r3, [r7, #20]
	adds r3, r3, r5
	str r3, [r7, #20]
	movs r0, #4
	bl 0x0200dbfc
	ldr r3, [r6, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r5
	str r3, [r7, #12]
	ldr r3, [r7, #20]
	adds r3, r3, r5
	str r3, [r7, #20]
	movs r0, #4
	bl 0x0200dbfc
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r3, [r7, #20]
	adds r3, r3, r2
	str r3, [r7, #20]
.L_020005ec_0:
	bl 0x0200dd3c
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200e6e8
	.4byte 0xfffd0000
	.4byte 0xfffe0000
	.global Func_020006c0
	.thumb_func
Func_020006c0:
	push {r5, lr}
	movs r0, #0
	bl 0x0200dd4c
	movs r3, #3
	adds r0, #85
	strb r3, [r0]
	movs r0, #12
	bl 0x0200dd4c
	movs r5, #4
	adds r0, #85
	strb r5, [r0]
	movs r0, #13
	bl 0x0200dd4c
	adds r0, #85
	strb r5, [r0]
	movs r0, #14
	bl 0x0200dd4c
	adds r0, #85
	strb r5, [r0]
	movs r0, #15
	bl 0x0200dd4c
	adds r0, #85
	strb r5, [r0]
	movs r0, #16
	bl 0x0200dd4c
	adds r0, #85
	strb r5, [r0]
	movs r0, #17
	bl 0x0200dd4c
	adds r0, #85
	strb r5, [r0]
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000714
	.thumb_func
Func_02000714:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200e6ec
	.global Func_0200071c
	.thumb_func
Func_0200071c:
	push {r5, r6, r7, lr}
	movs r0, #162
	lsls r0, r0, #1
	bl 0x0200dd14
	movs r0, #1
	bl 0x0200dbfc
	movs r0, #136
	lsls r0, r0, #1
	bl 0x0200dd14
	movs r0, #8
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r0, #9
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r0, #10
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r0, #11
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r0, #10
	bl 0x0200dd4c
	ldr r5, [pc, #272]
	str r5, [r0, #24]
	movs r0, #11
	bl 0x0200dd4c
	str r5, [r0, #24]
	movs r6, #12
	movs r7, #0
.L_0200071c_0:
	adds r0, r6, #0
	bl 0x0200dd4c
	adds r5, r0, #0
	adds r0, r6, #0
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	adds r0, r6, #0
	movs r1, #1
	bl 0x0200de1c
	adds r2, r5, #0
	adds r2, #85
	movs r3, #4
	strb r3, [r2]
	adds r1, r5, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #2
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #8
	adds r6, #1
	str r3, [r5, #12]
	cmp r6, #17
	bls .L_0200071c_0
	ldr r3, [pc, #200]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	beq .L_0200071c_1
	cmp r3, #3
	bgt .L_0200071c_2
	cmp r3, #1
	beq .L_0200071c_3
	cmp r3, #2
	beq .L_0200071c_4
	b .L_0200071c_5
.L_0200071c_2:
	cmp r3, #9
	beq .L_0200071c_6
	cmp r3, #9
	bgt .L_0200071c_7
	cmp r3, #4
	beq .L_0200071c_8
	b .L_0200071c_5
.L_0200071c_7:
	cmp r3, #93
	beq .L_0200071c_9
	b .L_0200071c_5
.L_0200071c_3:
	ldr r0, [pc, #152]
	bl 0x0200dd0c
	cmp r0, #0
	bne .L_0200071c_5
	bl 0x020088b4
	b .L_0200071c_5
.L_0200071c_4:
	bl 0x0200a360
	b .L_0200071c_5
.L_0200071c_1:
	bl 0x0200b8c0
	b .L_0200071c_5
.L_0200071c_9:
	bl 0x0200b8dc
	b .L_0200071c_5
.L_0200071c_8:
	bl 0x0200b924
	b .L_0200071c_5
.L_0200071c_6:
	bl 0x0200dd34
	ldr r0, [pc, #112]
	bl 0x0200dd0c
	cmp r0, #0
	beq .L_0200071c_10
	movs r0, #0
	b .L_0200071c_11
.L_0200071c_10:
	ldr r0, [pc, #104]
	bl 0x0200dd0c
	cmp r0, #0
	beq .L_0200071c_12
	movs r0, #1
	b .L_0200071c_11
.L_0200071c_12:
	ldr r0, [pc, #92]
	bl 0x0200dd0c
	cmp r0, #0
	beq .L_0200071c_13
	movs r0, #2
.L_0200071c_11:
	movs r1, #65
	bl 0x0200dcf4
	b .L_0200071c_14
.L_0200071c_13:
	movs r0, #3
	movs r1, #65
	bl 0x0200dcf4
.L_0200071c_14:
	movs r0, #9
	bl 0x0200de5c
.L_0200071c_5:
	ldr r0, [pc, #48]
	bl 0x0200dd0c
	cmp r0, #0
	beq .L_0200071c_15
	bl 0x020084bc
	cmp r0, #0
	beq .L_0200071c_15
	bl 0x0200dd4c
	cmp r0, #0
	beq .L_0200071c_15
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_0200071c_15:
	movs r0, #0
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0xffff0000
	.4byte 0x02000240
	.4byte 0x00000109
	.4byte 0x00000345
	.4byte 0x00000346
	.4byte 0x00000347
	.global Func_02000894
	.thumb_func
Func_02000894:
	push {lr}
	movs r1, #0
	bl 0x0200de04
	movs r0, #10
	bl 0x0200dd2c
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020008a8
	.thumb_func
Func_020008a8:
	push {lr}
	movs r2, #10
	bl 0x0200de14
	pop {r0}
	bx r0
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #18
	bl 0x0200dd4c
	adds	r5, r0, #0
	bl 0x0200dd34
	movs	r0, #1
	movs	r1, #0
	bl 0x02009280
	movs	r0, #2
	movs	r1, #0
	bl 0x02009280
	movs	r0, #3
	movs	r1, #0
	bl 0x02009280
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200de3c
	movs	r0, #1
	bl 0x0200dbfc
	movs	r2, #0
	mov	r8, r2
	adds	r3, r5, #0
	mov	r2, r8
	adds	r3, #85
	strb	r2, [r3, #0]
	adds	r5, #35
	ldrb	r2, [r5, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r5, #0]
	movs	r0, #18
	bl 0x0200dd4c
	movs	r1, #0
	bl 0x0200dcac
	movs	r0, #18
	movs	r1, #1
	bl 0x0200de1c
	movs	r1, #145
	movs	r2, #169
	lsls	r2, r2, #17
	lsls	r1, r1, #18
	movs	r0, #18
	bl 0x0200ddac
	movs	r0, #0
	bl 0x0200dd4c
	adds	r6, r0, #0
	adds	r5, r6, #0
	mov	r3, r8
	adds	r5, #85
	strb	r3, [r5, #0]
	movs	r0, #0
	movs	r1, #1
	bl 0x0200de1c
	movs	r2, #144
	ldr	r1, [pc, #284]
	lsls	r2, r2, #17
	movs	r0, #0
	bl 0x0200ddac
	movs	r0, #1
	bl 0x0200dbfc
	bl 0x0200de94
	bl 0x0200dea4
	movs	r0, #20
	bl 0x0200dd2c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #18
	lsls	r1, r1, #9
.L_02000976:
	lsls	r2, r2, #8
	bl 0x0200dd5c
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #0
	bl 0x0200dd5c
	movs	r0, #0
	bl 0x0200dd4c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #145
	strb	r3, [r0, #0]
	lsls	r1, r1, #2
	movs	r0, #18
	movs	r2, #221
	bl 0x0200dd84
	movs	r0, #0
	ldr	r1, [pc, #200]
	movs	r2, #171
	bl 0x0200dd8c
	movs	r0, #18
	ldr	r1, [pc, #196]
	movs	r2, #211
	bl 0x0200dd84
	movs	r0, #0
	ldr	r1, [pc, #188]
	movs	r2, #161
	bl 0x0200dd8c
	movs	r1, #130
	movs	r0, #18
	lsls	r1, r1, #2
	movs	r2, #191
	bl 0x0200dd84
	movs	r2, #141
	ldr	r1, [pc, #172]
	movs	r0, #0
	bl 0x0200dd8c
	movs	r0, #18
	bl 0x0200dd4c
	movs	r1, #1
	bl 0x0200dcac
	movs	r0, #18
	ldr	r1, [pc, #152]
	movs	r2, #171
	bl 0x0200dd84
	movs	r1, #129
	movs	r2, #121
	lsls	r1, r1, #2
	movs	r0, #0
	bl 0x0200dd8c
	movs	r0, #144
	lsls	r0, r0, #1
	bl 0x0200decc
	movs	r0, #20
	bl 0x0200dd2c
	movs	r1, #6
	movs	r0, #0
	bl 0x0200ddb4
	movs	r0, #0
	bl 0x0200ddc4
	movs	r3, #129
	lsls	r3, r3, #18
	str	r3, [r6, #8]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r6, #12]
	movs	r3, #148
	lsls	r3, r3, #16
	movs	r7, #128
	lsls	r7, r7, #8
	ldr	r2, [pc, #60]
	str	r3, [r6, #16]
	movs	r3, #3
	strh	r7, [r6, #6]
	movs	r0, #152
	strb	r3, [r5, #0]
	mov	r8, r2
	bl 0x0200decc
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r6, #40]
	movs	r0, #152
	bl 0x0200decc
	movs	r0, #0
	bl 0x0200dd4c
	movs	r1, #1
	bl 0x0200dcac
	movs	r1, #252
	lsls	r1, r1, #1
	movs	r2, #148
	movs	r0, #0
	bl 0x0200dd8c
	movs	r0, #10
	bl 0x0200dd2c
	b.n	.L_02000a88
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x02450000
	.4byte 0x00000245
	.4byte 0x00000212
	.4byte 0x00000213
	.4byte 0x00000209
	.2byte 0x0203
	.2byte 0x0000
.L_02000a88:
	movs	r0, #0
	bl 0x0200dd4c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r5, #1
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	mov	r9, r3
	ldr	r3, [pc, #1016]
	mov	r2, r9
	str	r3, [r6, #12]
	strh	r2, [r6, #6]
	movs	r0, #20
	bl 0x0200dd2c
	movs	r0, #154
	lsls	r0, r0, #1
	bl 0x0200decc
	movs	r1, #131
	movs	r2, #191
.L_02000ab8:
	lsls	r1, r1, #2
	movs	r0, #18
	bl 0x0200dd8c
	movs	r0, #18
	bl 0x0200dd4c
	movs	r1, #0
	bl 0x0200dcac
	movs	r0, #18
	ldr	r1, [pc, #972]
	movs	r2, #211
	bl 0x0200dd8c
	movs	r1, #145
	movs	r0, #18
	lsls	r1, r1, #2
	movs	r2, #221
	bl 0x0200dd8c
	movs	r1, #145
	movs	r2, #169
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	movs	r0, #18
	bl 0x0200dd84
	movs	r0, #0
	bl 0x0200dd4c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r5, r3
	ldr	r2, [pc, #928]
	strb	r5, [r0, #0]
	ldr	r1, [pc, #928]
	movs	r0, #0
	bl 0x0200dd5c
	movs	r0, #1
	movs	r1, #1
	bl 0x02009280
	movs	r0, #2
	movs	r1, #1
	bl 0x02009280
	movs	r0, #3
	movs	r1, #1
	bl 0x02009280
	movs	r1, #246
	movs	r0, #0
	lsls	r1, r1, #1
	movs	r2, #164
	bl 0x0200dd94
	movs	r0, #1
	ldr	r1, [pc, #888]
	movs	r2, #164
	bl 0x0200dd94
	movs	r1, #246
	movs	r0, #2
	lsls	r1, r1, #1
	movs	r2, #140
	bl 0x0200dd94
	movs	r2, #140
	movs	r0, #3
	ldr	r1, [pc, #864]
	bl 0x0200dd9c
	movs	r0, #0
	movs	r1, #1
	bl 0x0200ddb4
	movs	r0, #1
	movs	r1, #1
	bl 0x0200ddb4
	movs	r0, #2
	movs	r1, #1
	bl 0x0200ddb4
	movs	r0, #0
	mov	r1, r9
	movs	r2, #0
	bl 0x0200de14
	movs	r0, #1
	mov	r1, r9
	movs	r2, #0
	bl 0x0200de14
	movs	r0, #2
	mov	r1, r9
	movs	r2, #0
	bl 0x0200de14
	mov	r1, r9
	movs	r2, #0
	movs	r0, #3
	bl 0x0200de14
	movs	r0, #18
	bl 0x0200dda4
	movs	r1, #0
	movs	r2, #0
	movs	r0, #18
	bl 0x0200ddac
	ldr	r6, [pc, #780]
	ldr	r0, [pc, #784]
	bl 0x0200decc
	ldr	r0, [pc, #780]
	bl 0x0200ddf4
	adds	r0, r6, #0
	bl 0x02008894
	movs	r0, #0
	adds	r1, r7, #0
	movs	r2, #0
	bl 0x0200de14
	movs	r0, #1
	adds	r1, r7, #0
	movs	r2, #0
	bl 0x0200de14
	movs	r2, #0
	movs	r0, #2
	adds	r1, r7, #0
	bl 0x0200de14
	adds	r1, r7, #0
	movs	r0, #3
	bl 0x020088a8
	bl 0x0200de4c
	mov	r3, r8
	adds	r0, #85
	strb	r3, [r0, #0]
	ldr	r1, [pc, #724]
	ldr	r0, [pc, #728]
	bl 0x0200de34
	movs	r0, #152
	movs	r1, #128
	movs	r2, #158
	movs	r3, #1
	lsls	r1, r1, #14
	lsls	r2, r2, #16
	lsls	r0, r0, #17
	bl 0x0200de3c
	bl 0x0200de44
	movs	r0, #20
	bl 0x0200dd2c
	movs	r2, #208
	lsls	r2, r2, #8
	mov	fp, r2
	movs	r0, #20
	mov	r1, fp
	ldr	r5, [pc, #688]
	bl 0x020088a8
	movs	r1, #1
	movs	r0, #20
	bl 0x0200dddc
	movs	r0, #61
	bl 0x0200decc
	adds	r0, r5, #0
	bl 0x02008894
	movs	r1, #4
	movs	r0, #19
	bl 0x0200ddb4
	adds	r0, r6, #0
	bl 0x02008894
	movs	r3, #176
	lsls	r3, r3, #8
.L_02000c3a:
	mov	sl, r3
	movs	r0, #20
	mov	r1, sl
	bl 0x020088a8
	ldr	r1, [pc, #636]
	movs	r2, #40
	movs	r0, #20
	bl 0x0200de24
	adds	r0, r5, #0
	bl 0x02008894
	movs	r0, #21
	bl 0x02008894
	movs	r1, #128
	movs	r0, #19
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #20
	lsls	r1, r1, #1
	movs	r2, #20
	bl 0x0200de24
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #160
	movs	r0, #19
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #160
	movs	r2, #20
	movs	r0, #20
	lsls	r1, r1, #7
	bl 0x0200de14
	ldr	r0, [pc, #560]
	ldr	r1, [pc, #560]
	bl 0x0200de34
	movs	r0, #147
	movs	r1, #1
	movs	r2, #194
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #16
	lsls	r0, r0, #17
	bl 0x0200de3c
	movs	r0, #20
	bl 0x0200dd2c
	movs	r0, #21
	ldr	r1, [pc, #488]
	ldr	r2, [pc, #484]
	bl 0x0200dd5c
	movs	r1, #136
	movs	r2, #200
	movs	r0, #21
	lsls	r1, r1, #1
	bl 0x0200dd9c
	movs	r1, #1
	movs	r0, #20
	bl 0x0200dddc
	movs	r0, #20
	bl 0x0200dd2c
	adds	r0, r5, #0
	bl 0x02008894
	movs	r2, #20
	ldr	r1, [pc, #492]
	movs	r0, #19
	bl 0x0200de24
	movs	r0, #19
	bl 0x02008894
	movs	r1, #3
	movs	r0, #21
	bl 0x0200ddbc
	movs	r0, #21
	bl 0x02008894
	movs	r2, #40
	ldr	r1, [pc, #464]
	movs	r0, #20
	bl 0x0200de24
	adds	r0, r5, #0
	bl 0x02008894
	movs	r1, #4
	movs	r0, #21
	bl 0x0200ddbc
	movs	r0, #21
	bl 0x02008894
	movs	r0, #19
	ldr	r1, [pc, #436]
	movs	r2, #60
	bl 0x0200de24
	movs	r0, #19
	movs	r1, #0
	movs	r2, #40
	bl 0x0200de0c
	movs	r1, #131
	movs	r2, #40
	movs	r0, #19
	lsls	r1, r1, #1
	bl 0x0200de24
	adds	r1, r7, #0
	movs	r0, #19
	bl 0x020088a8
	ldr	r2, [pc, #400]
	mov	r8, r2
	mov	r0, r8
	bl 0x02008894
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200de14
	ldr	r1, [pc, #372]
	movs	r2, #40
	movs	r0, #21
	bl 0x0200de24
	movs	r0, #21
	bl 0x02008894
	movs	r1, #192
	movs	r0, #19
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200de14
	movs	r2, #40
	movs	r0, #20
	mov	r1, sl
	bl 0x0200de14
	movs	r1, #1
	movs	r0, #21
	bl 0x0200dddc
	movs	r0, #21
	bl 0x02008894
	movs	r6, #160
	movs	r1, #160
	movs	r2, #0
	lsls	r6, r6, #7
	movs	r0, #20
	lsls	r1, r1, #7
	bl 0x0200de14
	adds	r1, r6, #0
	movs	r0, #19
	bl 0x020088a8
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #19
	bl 0x0200de24
	mov	r0, r8
	bl 0x02008894
	movs	r0, #21
	ldr	r1, [pc, #276]
	movs	r2, #20
	bl 0x0200de24
	movs	r0, #21
	movs	r1, #0
	movs	r2, #20
	bl 0x0200de0c
	movs	r2, #40
	movs	r0, #20
	adds	r1, r7, #0
	bl 0x0200de14
	movs	r0, #20
	movs	r1, #4
	bl 0x0200ddbc
	adds	r0, r5, #0
	movs	r2, #40
	movs	r1, #0
	bl 0x0200de0c
	movs	r0, #21
	movs	r1, #3
	bl 0x0200ddbc
	movs	r0, #21
	movs	r1, #0
	movs	r2, #20
	bl 0x0200de0c
	movs	r2, #20
	adds	r1, r6, #0
	movs	r0, #20
	bl 0x0200de14
	movs	r1, #2
	movs	r0, #21
	bl 0x0200ddd4
	movs	r0, #21
	bl 0x02008894
	movs	r0, #20
	ldr	r1, [pc, #176]
	movs	r2, #0
	bl 0x0200de24
	movs	r2, #80
	movs	r0, #19
	ldr	r1, [pc, #164]
	bl 0x0200de24
	movs	r1, #2
	movs	r0, #21
	bl 0x0200ddd4
	movs	r0, #21
	bl 0x02008894
	movs	r2, #60
	ldr	r1, [pc, #156]
	movs	r0, #19
	bl 0x0200de24
	mov	r0, r8
	bl 0x02008894
	movs	r1, #3
	movs	r0, #21
	bl 0x0200ddbc
	movs	r0, #21
	bl 0x02008894
	movs	r0, #19
	movs	r1, #1
	bl 0x0200ddd4
	movs	r1, #1
	movs	r0, #20
	bl 0x0200dddc
	movs	r0, #20
	bl 0x0200dd2c
	movs	r0, #21
	movs	r1, #4
	bl 0x0200ddbc
	movs	r0, #21
	movs	r1, #0
	movs	r2, #20
	bl 0x0200de0c
	movs	r0, #20
	ldr	r1, [pc, #72]
	movs	r2, #60
	bl 0x0200de24
	movs	r2, #20
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200de0c
	mov	r1, sl
	movs	r0, #21
	bl 0x020088a8
	movs	r0, #21
	b.n	.L_02000edc
	.4byte 0xffe00000
	.4byte 0x00000212
	.4byte 0x00006666
	.4byte 0x0000cccc
	.4byte 0x00000202
	.4byte 0x00004013
	.4byte 0x00000121
	.4byte 0x00002757
	.4byte 0x00009999
	.4byte 0x0004cccc
	.4byte 0x00002014
	.4byte 0x00000105
	.4byte 0x00019999
	.4byte 0x00003333
	.4byte 0x00000103
	.4byte 0x00000101
	.2byte 0x2013
	.2byte 0x0000
.L_02000edc:
	bl 0x02008894
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #6
	bl 0x020088a8
	movs	r1, #2
	movs	r0, #6
	bl 0x0200dddc
	movs	r0, #20
	bl 0x0200dd2c
	movs	r0, #21
	mov	r1, fp
	bl 0x020088a8
	movs	r1, #4
	movs	r0, #19
	bl 0x0200ddbc
	mov	r0, r8
	bl 0x02008894
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200de14
	movs	r1, #1
	movs	r0, #21
	bl 0x0200dddc
	movs	r0, #21
	bl 0x02008894
	movs	r1, #3
	movs	r0, #20
	bl 0x0200ddbc
	adds	r0, r5, #0
	bl 0x02008894
	movs	r1, #4
	movs	r0, #21
	bl 0x0200ddbc
	movs	r0, #21
	bl 0x02008894
	movs	r0, #20
	ldr	r1, [pc, #772]
	movs	r2, #0
	bl 0x0200de24
	movs	r0, #19
	ldr	r1, [pc, #764]
	movs	r2, #80
	bl 0x0200de24
	movs	r0, #19
	adds	r1, r7, #0
	movs	r2, #0
	bl 0x0200de14
	movs	r2, #0
	movs	r0, #20
	adds	r1, r7, #0
	bl 0x0200de14
	ldr	r0, [pc, #740]
	ldr	r1, [pc, #740]
	bl 0x0200de34
	movs	r0, #147
	movs	r1, #1
	movs	r2, #180
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200de3c
	movs	r1, #131
	movs	r0, #21
	lsls	r1, r1, #1
	movs	r2, #176
	bl 0x0200dd9c
	movs	r0, #21
	adds	r1, r7, #0
	movs	r2, #40
	ldr	r6, [pc, #704]
	bl 0x0200de14
	movs	r2, #20
	movs	r0, #21
	movs	r1, #0
	bl 0x0200de14
	movs	r1, #2
	movs	r0, #21
	bl 0x0200ddd4
	adds	r0, r6, #0
	bl 0x02008894
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #19
	bl 0x0200de24
	mov	r0, r8
	bl 0x02008894
	movs	r1, #2
	movs	r0, #21
	bl 0x0200ddd4
	adds	r0, r6, #0
	bl 0x02008894
	movs	r0, #20
	ldr	r1, [pc, #644]
	movs	r2, #40
	bl 0x0200de24
	ldr	r0, [pc, #640]
	movs	r1, #0
	movs	r2, #20
	bl 0x0200de0c
	ldr	r1, [pc, #632]
	movs	r2, #20
	movs	r0, #21
	bl 0x0200de24
	adds	r0, r6, #0
	bl 0x02008894
	ldr	r1, [pc, #608]
	movs	r2, #20
	movs	r0, #19
	bl 0x0200de24
	mov	r0, r8
	bl 0x02008894
	movs	r2, #40
	ldr	r1, [pc, #576]
	movs	r0, #21
	bl 0x0200de24
	ldr	r5, [pc, #588]
	adds	r0, r6, #0
	bl 0x02008894
	movs	r1, #4
	movs	r0, #20
	bl 0x0200ddbc
	adds	r0, r5, #0
	bl 0x02008894
	movs	r1, #3
	movs	r0, #19
	bl 0x0200ddbc
	mov	r0, r8
	bl 0x02008894
	movs	r0, #21
	ldr	r1, [pc, #544]
	movs	r2, #60
	bl 0x0200de24
	movs	r0, #21
	adds	r1, r7, #0
	movs	r2, #20
	bl 0x0200de14
	ldr	r0, [pc, #540]
	movs	r1, #0
	movs	r2, #40
	bl 0x0200de0c
	movs	r0, #6
	ldr	r1, [pc, #524]
	movs	r2, #120
	bl 0x0200de24
	ldr	r1, [pc, #516]
	movs	r2, #60
	movs	r0, #20
	bl 0x0200de24
	adds	r0, r5, #0
	bl 0x02008894
	movs	r2, #40
	movs	r0, #21
	movs	r1, #0
	bl 0x0200de14
	movs	r1, #3
	movs	r0, #19
	bl 0x0200ddb4
	mov	r0, r8
	bl 0x02008894
	movs	r0, #21
	movs	r1, #1
	bl 0x0200dddc
	adds	r0, r6, #0
	movs	r1, #0
	movs	r2, #20
	bl 0x0200de0c
	adds	r0, r5, #0
	bl 0x02008894
	movs	r1, #128
	movs	r2, #40
	movs	r0, #21
	lsls	r1, r1, #1
	bl 0x0200de24
	movs	r1, #4
	movs	r0, #19
	bl 0x0200ddb4
	mov	r0, r8
	bl 0x02008894
	movs	r0, #6
	ldr	r1, [pc, #424]
	movs	r2, #40
	bl 0x0200de24
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #20
	bl 0x0200de24
	adds	r0, r5, #0
	bl 0x02008894
	movs	r2, #20
	ldr	r1, [pc, #388]
	movs	r0, #19
	bl 0x0200de24
	mov	r0, r8
	bl 0x02008894
	movs	r1, #1
	movs	r0, #21
	bl 0x0200dddc
	movs	r0, #20
	bl 0x0200dd2c
	movs	r1, #2
	movs	r0, #20
	bl 0x0200ddd4
	adds	r0, r5, #0
	bl 0x02008894
	movs	r1, #4
	movs	r0, #19
	bl 0x0200ddb4
	mov	r0, r8
	bl 0x02008894
	ldr	r6, [pc, #348]
	movs	r2, #224
	ldr	r3, [r6, #0]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #66
	str	r2, [r3, #0]
	bl 0x0200de9c
	bl 0x0200dea4
	movs	r0, #252
	movs	r2, #168
	movs	r3, #0
	lsls	r2, r2, #16
	ldr	r1, [pc, #320]
	lsls	r0, r0, #17
	bl 0x0200de3c
	movs	r0, #1
	bl 0x0200dbfc
	bl 0x0200dc7c
	movs	r0, #1
	bl 0x0200dbfc
	ldr	r5, [pc, #300]
	bl 0x0200de94
	bl 0x0200dea4
	movs	r0, #20
	bl 0x0200dd2c
	movs	r1, #1
	movs	r0, #1
	bl 0x0200dddc
	adds	r0, r5, #0
	bl 0x02008894
	movs	r2, #40
	ldr	r1, [pc, #228]
	movs	r0, #3
	bl 0x0200de24
	movs	r0, #3
	bl 0x02008894
	movs	r0, #2
	movs	r1, #3
	bl 0x0200ddbc
	ldr	r0, [pc, #252]
	movs	r1, #0
	movs	r2, #40
	bl 0x0200de0c
	movs	r2, #20
	movs	r1, #2
	movs	r0, #1
	bl 0x0200ddcc
	adds	r0, r5, #0
	bl 0x02008894
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200ddfc
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200de14
	movs	r0, #2
	mov	r1, r9
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200de14
	movs	r0, #0
	movs	r1, #0
	bl 0x0200dd44
	cmp	r0, #1
	bne.n	.L_020011d6
	ldr	r2, [r6, #0]
	movs	r3, #236
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L_020011d6:
	movs	r0, #20
	bl 0x0200dd2c
	movs	r0, #1
	bl 0x02008894
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl 0x0200de14
	movs	r2, #20
	adds	r1, r7, #0
	movs	r0, #3
	bl 0x0200de14
	movs	r0, #0
	movs	r1, #3
	bl 0x0200ddb4
	movs	r0, #1
	movs	r1, #3
	bl 0x0200ddb4
	movs	r0, #2
	movs	r1, #3
	bl 0x0200ddb4
	movs	r0, #3
	movs	r1, #3
	bl 0x0200ddbc
	ldr	r5, [pc, #100]
	movs	r0, #1
	adds	r1, r5, #0
	bl 0x0200dd64
	adds	r1, r5, #0
	movs	r0, #2
	bl 0x0200dd64
	adds	r1, r5, #0
	movs	r0, #3
	bl 0x0200dd7c
	movs	r0, #20
	bl 0x0200dd2c
	bl 0x0200dd3c
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x00000101
	.4byte 0x00006666
	.4byte 0x00000ccc
	.4byte 0x00008015
	.4byte 0x00000103
	.4byte 0x0000a014
	.4byte 0x00000105
	.4byte 0x0000a015
	.4byte 0x03001ebc
	.4byte 0xffe80000
	.4byte 0x00008001
	.4byte 0x00001002
	.2byte 0xdfc4
	.2byte 0x0200
	.global Func_02001280
	.thumb_func
Func_02001280:
	push {r5, lr}
	adds r5, r0, #0
	cmp r1, #0
	beq .L_02001280_0
	movs r1, #0
	bl 0x0200dde4
	adds r0, r5, #0
	bl 0x0200dd4c
	movs r1, #1
	bl 0x0200dcac
	adds r0, r5, #0
	ldr r1, [pc, #32]
	ldr r2, [pc, #36]
	bl 0x0200dd5c
	b .L_02001280_1
.L_02001280_0:
	movs r1, #15
	adds r0, r5, #0
	bl 0x0200dde4
	adds r0, r5, #0
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
.L_02001280_1:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0000cccc
	.4byte 0x00006666
	.global Func_020012c8
	.thumb_func
Func_020012c8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	bl 0x0200dd34
	bl 0x0200de4c
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	ldr r1, [pc, #1000]
	ldr r0, [pc, #1000]
	bl 0x0200de34
	movs r0, #166
	movs r1, #128
	movs r2, #180
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #14
	lsls r2, r2, #16
	bl 0x0200de3c
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200dd5c
	movs r7, #128
	movs r1, #170
	movs r2, #184
	lsls r7, r7, #8
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200dd9c
	movs r0, #0
	adds r1, r7, #0
	bl 0x020088a8
	movs r1, #1
	movs r0, #21
	bl 0x0200dddc
	ldr r0, [pc, #936]
	bl 0x0200ddf4
	ldr r0, [pc, #932]
	bl 0x02008894
	bl 0x0200de4c
	adds r0, #85
	strb r5, [r0]
	ldr r1, [pc, #908]
	ldr r0, [pc, #908]
	bl 0x0200de34
	movs r0, #152
	movs r1, #128
	movs r2, #180
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #14
	lsls r2, r2, #16
	bl 0x0200de3c
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #9
	adds r2, r7, #0
	bl 0x0200dd5c
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #9
	adds r2, r7, #0
	bl 0x0200dd5c
	movs r0, #3
	ldr r1, [pc, #872]
	ldr r2, [pc, #872]
	bl 0x0200dd5c
	movs r0, #0
	bl 0x0200dd4c
	cmp r0, #0
	beq .L_020012c8_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200ddac
.L_020012c8_0:
	movs r0, #0
	bl 0x0200dd4c
	cmp r0, #0
	beq .L_020012c8_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x0200ddac
.L_020012c8_1:
	movs r0, #0
	bl 0x0200dd4c
	cmp r0, #0
	beq .L_020012c8_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x0200ddac
.L_020012c8_2:
	movs r1, #164
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #168
	bl 0x0200dd94
	movs r1, #170
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #196
	bl 0x0200dd94
	movs r1, #163
	movs r2, #204
	movs r0, #3
	lsls r1, r1, #1
	bl 0x0200dd9c
	movs r0, #1
	movs r1, #1
	bl 0x0200ddb4
	movs r0, #2
	movs r1, #1
	bl 0x0200ddb4
	movs r0, #1
	adds r1, r7, #0
	movs r2, #0
	bl 0x0200de14
	movs r2, #0
	movs r0, #2
	adds r1, r7, #0
	bl 0x0200de14
	movs r0, #3
	adds r1, r7, #0
	bl 0x020088a8
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x0200de14
	movs r2, #40
	movs r0, #19
	movs r1, #0
	bl 0x0200de14
	movs r1, #2
	movs r0, #20
	bl 0x0200dddc
	movs r0, #20
	bl 0x02008894
	adds r1, r7, #0
	movs r0, #19
	bl 0x020088a8
	ldr r2, [pc, #688]
	mov r8, r2
	mov r0, r8
	bl 0x02008894
	movs r2, #20
	ldr r1, [pc, #680]
	movs r0, #21
	bl 0x0200de24
	movs r0, #21
.L_02001442:
	bl 0x02008894
	movs r0, #21
	movs r1, #2
	bl 0x0200ddd4
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200de0c
	movs r1, #208
	movs r2, #40
	lsls r1, r1, #8
	movs r0, #21
	bl 0x0200de14
	movs r0, #21
	bl 0x02008894
	movs r0, #21
	movs r1, #4
	bl 0x0200ddbc
	movs r0, #21
	movs r1, #0
	bl 0x020088a8
	movs r2, #20
.L_0200147c:
	movs r0, #21
	movs r1, #0
	bl 0x0200de0c
	ldr r5, [pc, #608]
	movs r0, #20
	movs r1, #1
	bl 0x0200dddc
	adds r1, r7, #0
	movs r0, #20
	bl 0x020088a8
	adds r0, r5, #0
	bl 0x02008894
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	bl 0x0200de2c
	movs r1, #2
	movs r0, #3
	bl 0x0200ddd4
	movs r0, #3
	bl 0x02008894
	movs r1, #1
	movs r0, #2
	bl 0x0200dddc
	movs r0, #2
	bl 0x02008894
	movs r1, #0
	movs r0, #19
	bl 0x020088a8
	movs r0, #19
	bl 0x02008894
	movs r1, #131
	movs r0, #20
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200de24
	movs r1, #0
	movs r2, #20
	movs r0, #20
	bl 0x0200de14
.L_020014e6:
	movs r0, #20
	bl 0x02008894
.L_020014ec:
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200de14
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200de14
	movs r1, #224
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	movs r6, #192
	bl 0x0200de14
	lsls r6, r6, #6
	movs r2, #0
	movs r0, #21
	adds r1, r7, #0
	bl 0x0200de14
	movs r0, #19
	adds r1, r6, #0
	bl 0x020088a8
	movs r1, #129
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #19
	bl 0x0200de24
	movs r0, #19
	bl 0x02008894
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #20
	bl 0x020088a8
	adds r0, r5, #0
	bl 0x02008894
	adds r1, r7, #0
	movs r2, #20
	movs r0, #20
	bl 0x0200de14
	adds r0, r5, #0
	bl 0x02008894
	movs r0, #19
	adds r1, r7, #0
	movs r2, #0
	bl 0x0200de14
	movs r0, #0
	adds r1, r7, #0
.L_02001572:
	movs r2, #0
	bl 0x0200de14
	movs r0, #1
	adds r1, r7, #0
	movs r2, #0
	bl 0x0200de14
.L_02001582:
	movs r0, #2
	adds r1, r7, #0
	movs r2, #0
	bl 0x0200de14
	movs r0, #3
	adds r1, r7, #0
.L_02001590:
	movs r2, #0
	bl 0x0200de14
	movs r0, #21
	movs r1, #0
	movs r2, #20
	bl 0x0200de14
.L_020015a0:
	ldr r1, [pc, #328]
	movs r2, #40
	movs r0, #6
	bl 0x0200de24
	movs r0, #6
	bl 0x02008894
	movs r2, #20
	ldr r1, [pc, #304]
	movs r0, #20
	bl 0x0200de24
	adds r0, r5, #0
	bl 0x02008894
	movs r1, #2
	movs r0, #6
	bl 0x0200dddc
	movs r0, #20
	bl 0x0200dd2c
	movs r1, #3
	movs r0, #6
	bl 0x0200ddb4
	movs r0, #6
	bl 0x02008894
	movs r1, #3
	movs r0, #20
.L_020015e0:
	bl 0x0200ddbc
	adds r0, r5, #0
	bl 0x02008894
.L_020015ea:
	movs r0, #6
	ldr r1, [pc, #220]
	ldr r2, [pc, #256]
	bl 0x0200dd5c
	movs r1, #130
	movs r0, #6
	lsls r1, r1, #1
.L_020015fa:
	movs r2, #186
	bl 0x0200dd9c
	movs r0, #21
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200de14
	movs r1, #138
	movs r2, #192
	lsls r1, r1, #1
	movs r0, #6
	bl 0x0200dd9c
	movs r0, #19
	bl 0x0200dd4c
	movs r3, #160
.L_0200161e:
	lsls r3, r3, #7
	strh r3, [r0, #6]
	movs r0, #1
	bl 0x0200dbfc
	movs r1, #2
	movs r0, #19
	bl 0x0200ddd4
	mov r0, r8
	bl 0x02008894
	movs r0, #6
	movs r1, #2
	movs r2, #20
	bl 0x0200ddcc
	movs r0, #6
	ldr r1, [pc, #176]
	ldr r2, [pc, #176]
	bl 0x0200dd5c
.L_0200164a:
	movs r1, #130
	movs r0, #6
	lsls r1, r1, #1
	movs r2, #186
	bl 0x0200dd9c
	movs r0, #21
	adds r1, r7, #0
	movs r2, #0
	bl 0x0200de14
	movs r0, #6
	movs r1, #248
	movs r2, #172
	bl 0x0200dd9c
	movs r0, #19
	adds r1, r7, #0
	movs r2, #0
	bl 0x0200de14
	movs r0, #21
	adds r1, r7, #0
	movs r2, #0
	bl 0x0200de14
	movs r2, #20
	movs r0, #6
	movs r1, #0
	bl 0x0200de14
	movs r1, #3
	movs r0, #6
	bl 0x0200ddbc
	movs r0, #40
	bl 0x0200dd2c
	bl 0x0200a334
	movs r0, #1
	bl 0x0200dbfc
	movs r5, #0
.L_020016a2:
.L_020016a2_0:
	movs r0, #6
	bl 0x0200dd4c
	bl 0x0200a0dc
	adds r5, #1
	movs r0, #1
	bl 0x0200dbfc
	cmp r5, #39
	bls .L_020016a2_0
	ldr r5, [pc, #64]
	movs r1, #200
	lsls r1, r1, #4
	adds r0, r5, #0
	bl 0x0200dc04
.L_020016c4:
	movs r0, #80
	b .L_020016c4_0
	.2byte 0x1999
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x278e
	.2byte 0x0000
	.2byte 0x9015
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0001
	.2byte 0xb333
	.2byte 0x0000
	.2byte 0x2013
	.2byte 0x0000
	.2byte 0x0103
	.2byte 0x0000
	.2byte 0x2014
	.2byte 0x0000
	.2byte 0x0101
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0002
	.2byte 0x3333
	.2byte 0x0001
	.2byte 0xa351
	.2byte 0x0200
.L_020016c4_0:
	bl 0x0200dd2c
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200de14
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200de14
	movs r1, #224
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200de14
	movs r0, #21
	movs r1, #0
	movs r2, #40
.L_0200173a:
	bl 0x0200de14
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r1, #128
	movs r2, #0
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200de14
	movs r1, #128
	lsls r1, r1, #8
.L_02001772:
	movs r0, #21
	bl 0x020088a8
	movs r0, #20
	ldr r1, [pc, #692]
	movs r2, #40
	bl 0x0200de24
	ldr r0, [pc, #688]
	movs r1, #0
	movs r2, #20
	bl 0x0200de0c
	movs r2, #80
	movs r0, #6
.L_02001790:
	ldr r1, [pc, #676]
	bl 0x0200de24
	movs r1, #2
	movs r0, #19
	bl 0x0200ddd4
	ldr r3, [pc, #668]
.L_020017a0:
	mov r8, r3
	mov r0, r8
	bl 0x02008894
	adds r0, r5, #0
	bl 0x0200dc0c
	movs r0, #1
	bl 0x0200dbfc
	movs r1, #0
	movs r0, #6
.L_020017b8:
	bl 0x0200dde4
	movs r0, #10
	bl 0x0200dbfc
	bl 0x0200a344
.L_020017c6:
	movs r1, #2
	movs r2, #40
	movs r0, #6
	bl 0x0200ddcc
	movs r0, #6
	bl 0x02008894
	ldr r1, [pc, #616]
	movs r0, #20
	movs r2, #20
	bl 0x0200de24
	ldr r2, [pc, #592]
	mov r10, r2
	mov r0, r10
	bl 0x02008894
	movs r1, #2
	movs r0, #6
	bl 0x0200dddc
	movs r0, #6
	bl 0x0200dd4c
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #250
	movs r2, #176
	movs r0, #6
	bl 0x0200dd9c
	movs r0, #1
	bl 0x0200dd2c
	movs r0, #6
	bl 0x0200dd4c
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	strb r3, [r0]
	ldr r1, [pc, #536]
	movs r0, #21
	movs r2, #20
	bl 0x0200de24
	movs r2, #20
	movs r1, #0
	movs r0, #21
	bl 0x0200de14
	movs r0, #21
	bl 0x02008894
	movs r1, #4
	movs r0, #19
	bl 0x0200ddbc
.L_02001846:
	mov r0, r8
	bl 0x02008894
	movs r1, #2
	movs r0, #6
	bl 0x0200dddc
	movs r0, #40
	bl 0x0200dd2c
	ldr r1, [pc, #488]
	ldr r2, [pc, #488]
	movs r0, #6
	bl 0x0200dd5c
	movs r0, #6
.L_02001866:
	bl 0x0200dd4c
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	movs r2, #172
	movs r1, #248
	strb r5, [r0]
	movs r0, #6
	bl 0x0200dd9c
	movs r0, #1
	bl 0x0200dd2c
	movs r0, #6
	bl 0x0200dd4c
	adds r0, #90
.L_0200188a:
	ldrb r3, [r0]
	orrs r6, r3
	strb r6, [r0]
	movs r0, #20
	bl 0x0200dd2c
	movs r1, #3
	movs r0, #6
.L_0200189a:
	bl 0x0200ddbc
	movs r0, #20
	bl 0x0200dd2c
	movs r1, #3
	movs r0, #19
	bl 0x0200ddbc
.L_020018ac:
	mov r0, r8
	bl 0x02008894
	movs r1, #3
	movs r0, #6
	bl 0x0200ddbc
.L_020018ba:
	movs r0, #6
	bl 0x02008894
	movs r1, #192
	movs r0, #19
	lsls r1, r1, #6
	movs r2, #0
.L_020018c8:
	bl 0x0200de14
	movs r1, #176
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200de14
	movs r0, #19
	ldr r1, [pc, #348]
	movs r2, #0
	bl 0x0200de24
	movs r2, #60
	movs r0, #20
	ldr r1, [pc, #336]
	bl 0x0200de24
	movs r1, #4
	movs r0, #20
	bl 0x0200ddb4
	mov r0, r10
	bl 0x02008894
	ldr r1, [pc, #308]
	movs r2, #40
	movs r0, #19
.L_02001900:
	bl 0x0200de24
	movs r0, #19
	bl 0x02008894
	movs r2, #100
	movs r0, #20
	ldr r1, [pc, #296]
	bl 0x0200de24
	movs r1, #1
	movs r0, #20
.L_02001918:
	bl 0x0200dddc
	movs r0, #20
	bl 0x0200dd2c
	movs r0, #20
	movs r1, #0
	bl 0x020088a8
	movs r0, #19
	movs r1, #0
	bl 0x020088a8
	movs r1, #0
	movs r0, #20
	bl 0x0200ddfc
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200de14
	movs r1, #192
	movs r0, #2
.L_0200194a:
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r1, #224
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r0, #0
	movs r1, #0
	bl 0x0200dd44
.L_02001966:
	cmp r0, #0
	bne .L_02001966_0
	movs r0, #20
	bl 0x0200dd2c
	movs r0, #20
	movs r1, #3
	bl 0x0200ddbc
	movs r5, #1
	b 0x0200999c
.L_02001966_0:
	movs r0, #20
	bl 0x0200dd2c
	movs r0, #20
	movs r1, #4
	bl 0x0200ddbc
	ldr r3, [pc, #192]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
.L_02001992:
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r5, #0
	movs r0, #20
	bl 0x02008894
	cmp r5, #0
	beq .L_02001992_0
	ldr r3, [pc, #164]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001992_0:
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200de14
	movs r1, #132
	movs r2, #40
	movs r0, #20
	lsls r1, r1, #1
	bl 0x0200de24
	movs r1, #0
	movs r0, #20
	bl 0x0200ddfc
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
.L_020019f4:
	movs r2, #0
	bl 0x0200de14
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r1, #224
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r0, #0
	movs r1, #0
	bl 0x0200dd44
	cmp r0, #0
	bne 0x02009a50
	movs r0, #20
	bl 0x0200dd2c
	movs r0, #2
.L_02001a26:
	movs r1, #3
	bl 0x0200ddb4
	movs r5, #1
	b .L_02001a26_0
	.2byte 0x0101
	.2byte 0x0000
	.2byte 0x2014
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0x2013
	.2byte 0x0000
	.2byte 0x0103
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0000
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x2014
	.2byte 0xf004
	.2byte 0xf96b
	.2byte 0x2002
	.2byte 0x2104
	.2byte 0xf004
	.2byte 0xf9ab
	.2byte 0x4bfd
	.2byte 0x681a
	.2byte 0x23ec
	.2byte 0x005b
	.2byte 0x18d2
	.2byte 0x8813
	.2byte 0x3301
	.2byte 0x8013
	.2byte 0x2500
.L_02001a26_0:
	movs r0, #2
	bl 0x02008894
	cmp r5, #0
	beq .L_02001a26_1
	ldr r3, [pc, #984]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02001a26_1:
	movs r0, #1
	movs r1, #2
	bl 0x0200dddc
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #1
	bl 0x020088a8
	movs r0, #1
	bl 0x02008894
	movs r1, #1
	movs r0, #19
	bl 0x0200dddc
	movs r0, #19
	bl 0x02008894
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r2, #128
	lsls r2, r2, #8
	mov r10, r2
	mov r1, r10
	movs r0, #3
	bl 0x020088a8
	movs r0, #1
	movs r1, #4
	bl 0x0200ddbc
	movs r0, #20
	movs r1, #3
	bl 0x0200ddbc
	movs r2, #20
	movs r0, #20
	movs r1, #0
	bl 0x0200de0c
	movs r0, #0
	movs r1, #3
	bl 0x0200ddb4
	movs r0, #1
	movs r1, #3
	bl 0x0200ddb4
	movs r0, #2
	movs r1, #3
	bl 0x0200ddb4
	movs r0, #3
	movs r1, #3
	bl 0x0200ddbc
	movs r3, #192
	lsls r3, r3, #6
	mov r8, r3
	mov r1, r8
	movs r0, #19
	bl 0x020088a8
	movs r1, #3
	movs r0, #19
	bl 0x0200ddb4
	movs r0, #19
	bl 0x02008894
	movs r1, #176
	movs r2, #20
	movs r0, #20
	lsls r1, r1, #8
	bl 0x0200de14
	movs r1, #3
	movs r0, #20
	bl 0x0200ddbc
	ldr r6, [pc, #788]
	movs r0, #40
	bl 0x0200dd2c
	mov r1, r10
	movs r0, #20
	bl 0x020088a8
	adds r0, r6, #0
	bl 0x02008894
	movs r1, #2
	movs r0, #21
	bl 0x0200dddc
	movs r0, #20
	bl 0x0200dd2c
.L_02001b64:
	movs r1, #0
	movs r2, #40
	movs r0, #21
	bl 0x0200de0c
	adds r0, r6, #0
	bl 0x02008894
	movs r2, #60
	movs r0, #21
	ldr r1, [pc, #736]
	bl 0x0200de24
	mov r1, r10
	movs r0, #19
	bl 0x020088a8
	movs r1, #1
	movs r0, #19
	bl 0x0200dddc
	ldr r0, [pc, #720]
	bl 0x02008894
	movs r2, #60
	movs r0, #21
	ldr r1, [pc, #712]
	bl 0x0200de24
	movs r1, #3
	movs r0, #21
	bl 0x0200ddbc
	movs r0, #20
	bl 0x0200dd2c
	movs r0, #21
	ldr r1, [pc, #696]
	ldr r2, [pc, #696]
	bl 0x0200dd5c
	movs r1, #144
	movs r0, #21
	lsls r1, r1, #1
	movs r2, #192
	bl 0x0200dd9c
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl 0x0200de14
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x0200de14
	movs r1, #155
	movs r0, #21
	lsls r1, r1, #1
	movs r2, #192
	bl 0x0200dd9c
	movs r1, #164
	movs r2, #186
	lsls r1, r1, #1
	movs r0, #21
	bl 0x0200dd9c
	movs r0, #20
	bl 0x0200dd2c
	movs r0, #21
	movs r1, #2
	bl 0x0200dddc
	ldr r5, [pc, #624]
	movs r1, #1
	adds r0, r5, #0
	bl 0x0200dcdc
	movs r1, #155
	movs r0, #21
	lsls r1, r1, #1
	movs r2, #192
	bl 0x0200dd9c
	mov r1, r10
	movs r0, #19
	movs r2, #0
	bl 0x0200de14
	mov r1, r10
	movs r0, #20
	movs r2, #0
	bl 0x0200de14
	movs r1, #144
	movs r0, #21
	lsls r1, r1, #1
	movs r2, #192
	bl 0x0200dd9c
	movs r1, #131
	movs r0, #21
	lsls r1, r1, #1
	movs r2, #176
	bl 0x0200dd9c
	adds r5, #1
	movs r2, #40
	movs r1, #0
	movs r0, #21
	bl 0x0200de14
	adds r0, r5, #0
	bl 0x0200ddf4
	movs r0, #21
	bl 0x02008894
	movs r1, #3
	movs r0, #20
	bl 0x0200ddbc
	adds r0, r6, #0
	bl 0x02008894
	movs r0, #21
	movs r1, #3
	bl 0x0200ddbc
	movs r0, #20
	movs r1, #0
	bl 0x020088a8
	mov r1, r10
	movs r0, #21
	bl 0x020088a8
	movs r0, #21
	movs r1, #3
	bl 0x0200ddbc
	movs r0, #6
	movs r1, #3
	bl 0x0200ddbc
	movs r0, #6
	ldr r1, [pc, #472]
	ldr r2, [pc, #472]
	bl 0x0200dd5c
	movs r1, #130
	movs r0, #6
	lsls r1, r1, #1
	movs r2, #186
	bl 0x0200dd9c
	mov r1, r8
	movs r0, #21
	movs r2, #0
	bl 0x0200de14
	movs r1, #138
	movs r2, #192
	lsls r1, r1, #1
	movs r0, #6
	bl 0x0200dd9c
	movs r0, #19
	bl 0x0200decc
	movs r0, #19
	bl 0x0200dd4c
	movs r5, #160
	lsls r5, r5, #7
	strh r5, [r0, #6]
	movs r0, #1
	bl 0x0200dbfc
	movs r1, #1
	movs r0, #19
	bl 0x0200dddc
	movs r0, #19
	bl 0x02008894
	movs r0, #6
	movs r1, #2
	bl 0x0200dddc
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl 0x0200de14
	adds r1, r5, #0
	movs r0, #20
	movs r2, #0
	bl 0x0200de14
	movs r1, #208
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #20
	movs r6, #160
	bl 0x0200de14
	lsls r6, r6, #8
	movs r2, #20
	movs r0, #3
	movs r1, #2
	bl 0x0200ddcc
	adds r1, r6, #0
	movs r0, #3
	bl 0x020088a8
	movs r0, #3
	bl 0x02008894
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl 0x0200de14
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl 0x0200de14
	mov r1, r8
	movs r0, #19
	movs r2, #0
	bl 0x0200de14
	movs r1, #176
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #80
	bl 0x0200de14
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl 0x0200de14
	movs r1, #0
	movs r2, #40
	movs r0, #20
	bl 0x0200de14
	movs r0, #29
	bl 0x0200decc
	movs r0, #20
	bl 0x02008894
	mov r1, r8
	movs r0, #21
	movs r2, #0
	bl 0x0200de14
	movs r1, #176
	movs r2, #20
	movs r0, #6
	lsls r1, r1, #8
	bl 0x0200de14
	movs r1, #2
	movs r0, #2
	bl 0x0200ddd4
	movs r0, #2
	bl 0x02008894
	mov r1, r8
	movs r0, #19
	bl 0x020088a8
	movs r1, #3
	movs r0, #19
	bl 0x0200ddbc
	movs r5, #208
	movs r0, #19
	bl 0x02008894
	lsls r5, r5, #8
	movs r2, #0
	movs r0, #21
	movs r1, #0
	bl 0x0200de14
	adds r1, r5, #0
	movs r0, #6
	bl 0x020088a8
	movs r1, #3
	movs r0, #3
	bl 0x0200ddb4
	movs r0, #3
	bl 0x02008894
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #1
	movs r2, #20
	bl 0x0200de24
	mov r1, r8
	movs r2, #20
	movs r0, #20
	bl 0x0200de14
	movs r0, #20
	bl 0x02008894
	movs r1, #129
	movs r0, #6
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200de24
	movs r1, #129
	movs r0, #21
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200de24
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl 0x0200de14
	movs r0, #21
	movs r1, #0
	bl 0x020088a8
	movs r2, #40
	movs r0, #1
	ldr r1, [pc, #68]
	bl 0x0200de24
	movs r1, #2
	movs r0, #1
	bl 0x0200ddd4
	movs r0, #1
	bl 0x02008894
	movs r0, #19
	movs r1, #0
	bl 0x020088a8
	movs r1, #4
	movs r0, #19
	bl 0x0200ddbc
	movs r0, #19
	bl 0x02008894
	movs r0, #20
	movs r1, #0
	bl 0x020088a8
	movs r1, #4
	movs r0, #20
	bl 0x0200ddb4
	movs r0, #20
	b .L_02001b64_0
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x2014
	.2byte 0x0000
	.4byte 0x00000103
	.4byte 0x00002013
	.4byte 0x00000105
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x000027ba
.L_02001b64_0:
	bl 0x02008894
	adds r1, r5, #0
	movs r0, #6
	movs r2, #0
	bl 0x0200de14
	movs r0, #6
	ldr r1, [pc, #564]
	movs r2, #0
	bl 0x0200de24
	movs r0, #3
	ldr r1, [pc, #556]
	movs r2, #60
	bl 0x0200de24
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200de0c
	mov r1, r8
	movs r0, #19
	bl 0x020088a8
	movs r1, #4
	movs r0, #19
	bl 0x0200ddb4
	movs r0, #19
	bl 0x02008894
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	bl 0x0200de2c
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x0200de0c
	movs r0, #19
	movs r1, #0
	bl 0x020088a8
	movs r1, #3
	movs r0, #20
	bl 0x0200ddbc
	movs r0, #20
	bl 0x02008894
	movs r0, #0
	movs r1, #2
	bl 0x0200ddd4
	movs r0, #1
	movs r1, #2
	bl 0x0200ddd4
	movs r0, #2
	movs r1, #2
	bl 0x0200ddd4
	movs r0, #3
	movs r1, #2
	bl 0x0200dddc
	adds r1, r6, #0
	movs r0, #0
	movs r2, #0
	bl 0x0200de14
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200de14
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200de14
	movs r1, #224
	movs r2, #40
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200de14
	movs r0, #3
	movs r1, #1
	bl 0x0200dddc
	adds r1, r6, #0
	movs r0, #3
	bl 0x020088a8
	movs r0, #3
	bl 0x02008894
	mov r1, r10
	movs r0, #1
	movs r2, #20
	bl 0x0200de14
	movs r2, #20
	movs r0, #1
	ldr r1, [pc, #364]
	bl 0x0200de24
	movs r1, #2
	movs r0, #1
	bl 0x0200ddd4
	movs r0, #1
	bl 0x02008894
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl 0x0200de14
	mov r1, r10
	movs r0, #0
	movs r2, #0
	bl 0x0200de14
	mov r1, r10
	movs r0, #2
	movs r2, #40
	bl 0x0200de14
	movs r2, #40
	movs r0, #20
	ldr r1, [pc, #308]
	bl 0x0200de24
	movs r1, #2
	movs r0, #20
	bl 0x0200ddd4
	movs r0, #20
	bl 0x02008894
	movs r1, #1
	movs r0, #19
	bl 0x0200dddc
	movs r0, #19
	bl 0x02008894
	movs r0, #65
	bl 0x0200dcfc
	ldr r2, [pc, #272]
	adds r0, r0, r2
	bl 0x0200dd14
	movs r0, #65
	bl 0x0200dd04
	movs r0, #0
	bl 0x0200dd4c
	adds r0, #90
	movs r5, #254
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #1
	bl 0x0200dd4c
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #2
	bl 0x0200dd4c
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #3
	bl 0x0200dd4c
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #19
	bl 0x0200dd4c
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #20
	bl 0x0200dd4c
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #21
	bl 0x0200dd4c
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #6
	bl 0x0200dd4c
	adds r0, #90
	ldrb r3, [r0]
	ldr r6, [pc, #140]
	ands r5, r3
	strb r5, [r0]
	adds r1, r6, #0
	movs r0, #0
	bl 0x0200dd64
	adds r1, r6, #0
	movs r0, #1
	bl 0x0200dd64
	adds r1, r6, #0
	movs r0, #2
	bl 0x0200dd64
	adds r1, r6, #0
	movs r0, #3
	bl 0x0200dd64
	ldr r5, [pc, #104]
	movs r0, #19
	adds r1, r5, #0
	bl 0x0200dd64
	adds r1, r5, #0
	movs r0, #20
	bl 0x0200dd64
	adds r1, r6, #0
	movs r0, #21
	bl 0x0200dd64
	movs r0, #6
	adds r1, r6, #0
	bl 0x0200dd7c
	ldr r3, [pc, #76]
	ldr r2, [pc, #76]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	ldr r5, [pc, #72]
	movs r1, #2
	adds r0, r5, #0
	bl 0x0200de6c
	adds r0, r5, #0
	movs r1, #9
	bl 0x0200de74
	movs r0, #98
	movs r1, #1
	bl 0x0200de64
	movs r0, #212
	lsls r0, r0, #2
	bl 0x0200dd14
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000101
	.4byte 0x00000103
	.4byte 0x00000345
	.4byte 0x0200e004
	.4byte 0x0200e03c
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x000000bb
	.global Func_020020dc
	.thumb_func
Func_020020dc:
	push {r5, lr}
.L_020020de:
	ldr r3, [pc, #56]
	ldr r3, [r3]
	movs r2, #2
	ands r3, r2
	adds r5, r0, #0
	cmp r3, #0
	beq .L_020020de_0
	movs r1, #7
	bl 0x0200dcd4
	b .L_020020de_1
.L_020020de_0:
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200dcd4
.L_020020de_1:
	ldr r3, [pc, #24]
	movs r1, #15
	ldr r0, [r3]
	bl 0x0200dbf4
	cmp r0, #0
	bne .L_020020de_2
	adds r0, r5, #0
	bl 0x0200a200
.L_020020de_2:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e40
	.global Func_0200211c
	.thumb_func
Func_0200211c:
	push {r5, r6, lr}
	ldr r5, [pc, #56]
	ldr r3, [r5]
	movs r2, #1
	ands r3, r2
	adds r6, r0, #0
	cmp r3, #0
	beq .L_0200211c_0
	ldr r0, [r5]
	movs r1, #6
	lsrs r0, r0, #1
	bl 0x0200dbf4
	adds r1, r0, #0
	adds r0, r6, #0
	bl 0x0200dcd4
.L_0200211c_0:
	ldr r0, [r5]
	movs r1, #15
	bl 0x0200dbf4
	cmp r0, #0
	bne .L_0200211c_1
	adds r0, r6, #0
	bl 0x0200a200
.L_0200211c_1:
	pop {r5, r6}
.L_02002152:
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x1e40
	.2byte 0x0300
	.global Func_0200215c
	.thumb_func
Func_0200215c:
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
	ble .L_0200215c_0
	adds r0, r5, #0
	bl 0x0200dc74
	b 0x0200a1a6
.L_0200215c_0:
	lsls r0, r0, #10
	bl 0x0200dc1c
	str r0, [r5, #24]
	str r0, [r5, #28]
	ldr r3, [r6, #8]
	movs r1, #128
	str r3, [r5, #8]
.L_0200218c:
	ldr r3, [r5, #12]
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r5, #12]
	subs r1, r1, r0
	ldr r3, [r6, #16]
	lsls r2, r1, #2
	adds r2, r2, r1
.L_0200219c:
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5, #16]
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_020021ac
	.thumb_func
Func_020021ac:
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
	ble .L_020021ac_0
	adds r0, r5, #0
	bl 0x0200dc74
	b 0x0200a1f8
.L_020021ac_0:
	lsls r0, r0, #10
	bl 0x0200dc1c
	negs r3, r0
	str r0, [r5, #24]
	str r3, [r5, #28]
.L_020021d8:
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
	subs r3, r3, r2
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r5, #16]
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002200
	.thumb_func
Func_02002200:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #80]
	ldr r3, [r3]
	adds r6, r0, #0
	movs r0, #131
	sub sp, #8
	mov r11, r3
	bl 0x0200decc
	movs r1, #63
	movs r7, #0
	mov r10, sp
	mov r9, r1
.L_02002200_2:
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	ldr r1, [r6, #8]
	movs r0, #26
	bl 0x0200dc6c
	lsls r3, r7, #2
	mov r2, r10
	str r0, [r3, r2]
	cmp r0, #0
	beq .L_02002200_0
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
	beq .L_02002200_0
	b .L_02002200_1
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x03001f30
.L_02002200_1:
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200dc54
	adds r3, r5, #0
	adds r3, #38
	mov r2, r8
	strb r2, [r3]
	ldrb r0, [r5, #28]
	bl 0x0200dc44
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
	b .L_02002200_0
	.2byte 0x0000
	.4byte 0xfffffc00
	.4byte 0x03001b10
.L_02002200_0:
	adds r7, #1
	cmp r7, #1
	ble .L_02002200_2
	ldr r2, [sp, #0]
	ldr r3, [pc, #76]
	str r3, [r2, #108]
	ldr r3, [r6, #80]
	ldr r4, [r2, #80]
	ldrb r3, [r3, #9]
	movs r2, #13
	ldrb r0, [r4, #9]
	negs r2, r2
	movs r1, #12
	ands r1, r3
	adds r3, r2, #0
	ands r3, r0
	orrs r3, r1
	strb r3, [r4, #9]
	mov r3, r10
	ldr r0, [r3, #4]
	ldr r3, [r6, #80]
	ldr r4, [r0, #80]
	ldrb r1, [r3, #9]
	movs r3, #12
	ands r3, r1
	ldrb r1, [r4, #9]
	ands r2, r1
	orrs r2, r3
	ldr r3, [pc, #32]
	str r3, [r0, #108]
	adds r0, #35
	movs r3, #2
	strb r2, [r4, #9]
	strb r3, [r0]
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200a1ad
	.4byte 0x0200a15d
.L_02002334:
	.global Func_02002334
	.thumb_func
Func_02002334:
	push {lr}
	movs r0, #140
	movs r1, #0
	bl 0x0200deac
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002344
	.thumb_func
Func_02002344:
	push {lr}
	bl 0x0200deb4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002350
	.thumb_func
Func_02002350:
	push {lr}
	movs r0, #6
	bl 0x0200dd4c
	bl 0x0200a11c
	pop {r0}
	bx r0
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #136
	bl 0x0200dd34
	movs	r3, #17
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
.L_0200237c:
	movs	r1, #10
	movs	r2, #4
	movs	r3, #2
	movs	r0, #17
	bl 0x0200dc9c
	movs	r0, #1
	bl 0x0200dd4c
	movs	r3, #128
	lsls	r3, r3, #8
	ldr	r2, [pc, #60]
	mov	r8, r3
	mov	r4, r8
	mov	r9, r2
	movs	r1, #164
	movs	r2, #168
	strh	r4, [r0, #6]
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #1
	bl 0x0200ddac
	movs	r0, #2
	bl 0x0200dd4c
	mov	r2, r8
	strh	r2, [r0, #6]
	movs	r1, #170
	movs	r2, #196
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #2
.L_020023be:
	bl 0x0200ddac
	movs	r0, #3
.L_020023c4:
	bl 0x0200dd4c
	mov	r3, r8
	movs	r1, #163
	movs	r2, #204
	b.n	.L_020023d4
	.2byte 0x0000
	.2byte 0x0000
.L_020023d4:
	strh	r3, [r0, #6]
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #3
	bl 0x0200ddac
	movs	r0, #6
	bl 0x0200dd4c
	movs	r4, #192
.L_020023e8:
	lsls	r4, r4, #6
	mov	sl, r4
	mov	r2, sl
	strh	r2, [r0, #6]
	movs	r1, #134
	movs	r2, #154
	lsls	r1, r1, #17
.L_020023f6:
	lsls	r2, r2, #16
	movs	r0, #6
	bl 0x0200ddac
	movs	r0, #21
	bl 0x0200dd4c
	mov	r3, sl
.L_02002406:
	movs	r1, #134
	movs	r2, #164
	strh	r3, [r0, #6]
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #21
	bl 0x0200ddac
	movs	r0, #20
	bl 0x0200dd4c
	adds	r7, r0, #0
	adds	r3, r7, #0
	movs	r6, #10
	adds	r3, #100
	strh	r6, [r3, #0]
.L_02002426:
	movs	r3, #208
	lsls	r3, r3, #8
	movs	r1, #147
.L_0200242c:
	movs	r2, #212
	strh	r3, [r7, #6]
	lsls	r2, r2, #16
	movs	r0, #20
	lsls	r1, r1, #17
	bl 0x0200ddac
	movs	r0, #20
	movs	r1, #9
	bl 0x0200ddb4
	ldr	r4, [pc, #1004]
	mov	fp, r4
	mov	r1, fp
	movs	r0, #20
	bl 0x0200dd64
	movs	r0, #20
	bl 0x0200dd4c
	movs	r1, #0
	bl 0x0200dcac
	movs	r0, #19
	bl 0x0200dd4c
	adds	r7, r0, #0
	adds	r3, r7, #0
	adds	r3, #100
	movs	r5, #0
	movs	r1, #143
	movs	r2, #192
	strh	r6, [r3, #0]
	lsls	r2, r2, #16
	strh	r5, [r7, #6]
	movs	r0, #19
	lsls	r1, r1, #17
	bl 0x0200ddac
	movs	r0, #19
	movs	r1, #7
	bl 0x0200ddb4
	mov	r1, fp
	movs	r0, #19
	bl 0x0200dd64
	movs	r0, #19
	bl 0x0200dd4c
	movs	r1, #0
	bl 0x0200dcac
	bl 0x0200de4c
	mov	r2, r9
	adds	r0, #85
	strb	r2, [r0, #0]
	movs	r1, #128
	movs	r0, #152
	movs	r2, #180
	movs	r3, #0
	lsls	r1, r1, #14
	lsls	r2, r2, #16
	lsls	r0, r0, #17
	bl 0x0200de3c
	movs	r0, #1
	bl 0x0200dbfc
	bl 0x0200dc7c
	movs	r0, #1
	bl 0x0200dbfc
	movs	r6, #128
	bl 0x0200de94
	bl 0x0200dea4
	movs	r0, #80
	bl 0x0200dd2c
	lsls	r6, r6, #6
	movs	r2, #20
	movs	r0, #1
	movs	r1, #2
	bl 0x0200ddcc
	adds	r1, r6, #0
	movs	r0, #1
	bl 0x020088a8
	movs	r7, #160
.L_020024e8:
	ldr	r0, [pc, #840]
	bl 0x0200ddf4
	lsls	r7, r7, #8
	ldr	r0, [pc, #836]
	bl 0x02008894
.L_020024f6:
	adds	r1, r7, #0
	movs	r0, #3
	bl 0x020088a8
	movs	r0, #3
	bl 0x02008894
	movs	r2, #0
	movs	r0, #1
	mov	r1, r8
	bl 0x0200de14
	adds	r1, r7, #0
	movs	r0, #2
	bl 0x020088a8
	movs	r0, #21
	movs	r1, #2
.L_0200251a:
	bl 0x0200dddc
	ldr	r1, [pc, #796]
	ldr	r2, [pc, #796]
.L_02002522:
	movs	r0, #21
	bl 0x0200dd5c
	movs	r0, #21
	bl 0x0200dd4c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #140
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	movs	r2, #164
	movs	r0, #21
	bl 0x0200dd9c
	movs	r0, #1
	bl 0x0200dd2c
	movs	r0, #21
	bl 0x0200dd4c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #20
	bl 0x0200dd2c
	movs	r1, #129
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #2
	bl 0x0200de24
	movs	r0, #2
	bl 0x02008894
	movs	r1, #4
	movs	r0, #21
	bl 0x0200ddbc
	movs	r0, #21
	bl 0x02008894
	movs	r2, #20
	movs	r0, #1
	ldr	r1, [pc, #700]
	bl 0x0200de24
	movs	r1, #1
	movs	r0, #1
	bl 0x0200dddc
	movs	r0, #1
	bl 0x02008894
	movs	r0, #21
	movs	r1, #1
	bl 0x0200dddc
	movs	r1, #0
	movs	r0, #21
	bl 0x020088a8
	movs	r0, #21
	bl 0x02008894
	movs	r1, #2
	movs	r0, #3
	bl 0x0200ddd4
	movs	r0, #3
	bl 0x02008894
	movs	r1, #4
	movs	r0, #3
	bl 0x0200ddb4
	movs	r0, #3
	bl 0x02008894
	movs	r2, #40
	movs	r0, #21
	ldr	r1, [pc, #632]
	bl 0x0200de24
	mov	r1, sl
	movs	r0, #21
	bl 0x020088a8
	movs	r0, #21
	bl 0x02008894
	movs	r2, #20
	movs	r1, #2
	movs	r0, #2
	bl 0x0200ddcc
	movs	r0, #2
	bl 0x02008894
	movs	r1, #3
	movs	r0, #21
	bl 0x0200ddbc
	movs	r0, #21
	bl 0x02008894
	movs	r0, #1
	movs	r1, #1
	bl 0x0200dddc
	movs	r0, #1
	adds	r1, r6, #0
	bl 0x020088a8
	movs	r1, #0
	movs	r0, #1
	bl 0x0200ddfc
	movs	r1, #224
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #224
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r0, #0
	movs	r1, #0
	bl 0x0200dd44
	movs	r7, #1
.L_0200263a:
	cmp	r0, #0
	beq.n	.L_02002650
.L_0200263e:
	ldr	r3, [pc, #524]
	ldr	r2, [r3, #0]
	movs	r3, #236
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r7, #0
.L_02002650:
	movs	r0, #20
	bl 0x0200dbfc
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #8
.L_0200265c:
	movs	r2, #0
	bl 0x0200de14
.L_02002662:
	movs	r1, #160
	movs	r2, #0
	movs	r0, #2
	lsls	r1, r1, #8
	bl 0x0200de14
	movs	r4, #160
	lsls	r4, r4, #8
	mov	sl, r4
	movs	r0, #3
	mov	r1, sl
.L_02002678:
	bl 0x020088a8
	movs	r0, #21
	movs	r1, #0
	bl 0x020088a8
	movs	r0, #21
	movs	r1, #0
	bl 0x0200de04
	movs	r0, #17
	bl 0x0200decc
	movs	r0, #40
	bl 0x0200dd2c
	cmp	r7, #0
	beq.n	.L_020026ac
	ldr	r3, [pc, #428]
	movs	r0, #236
	ldr	r2, [r3, #0]
	lsls	r0, r0, #1
	adds	r2, r2, r0
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L_020026ac:
	movs	r0, #20
	bl 0x0200dd74
	movs	r0, #19
	bl 0x0200dd74
	movs	r0, #1
.L_020026ba:
	bl 0x0200dbfc
	movs	r0, #20
	bl 0x0200dd4c
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r7, r0, #0
	str	r2, [r7, #24]
	str	r2, [r7, #28]
	movs	r0, #19
.L_020026d0:
	mov	r8, r2
	bl 0x0200dd4c
.L_020026d6:
	mov	r3, r8
	adds	r7, r0, #0
	str	r3, [r7, #24]
	str	r3, [r7, #28]
	movs	r0, #1
	bl 0x0200dbfc
	movs	r0, #20
	bl 0x02008894
	movs	r1, #192
.L_020026ec:
	movs	r0, #21
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #192
	movs	r0, #1
	lsls	r1, r1, #7
	movs	r2, #0
.L_0200270a:
	bl 0x0200de14
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	movs	r2, #40
	movs	r0, #3
	lsls	r1, r1, #8
	bl 0x0200de14
	movs	r1, #1
	movs	r0, #20
	bl 0x0200ddb4
	movs	r0, #20
	bl 0x0200dd4c
	movs	r1, #1
	bl 0x0200dcac
	movs	r0, #20
	bl 0x0200dd2c
	movs	r0, #20
	ldr	r1, [pc, #268]
	ldr	r2, [pc, #268]
	bl 0x0200dd5c
	movs	r1, #150
	movs	r2, #206
.L_0200274e:
	lsls	r1, r1, #1
	movs	r0, #20
	bl 0x0200dd9c
	movs	r0, #20
	bl 0x0200dd2c
	movs	r1, #2
	movs	r0, #20
	bl 0x0200dddc
	movs	r0, #20
	bl 0x0200dd2c
	movs	r0, #20
	bl 0x0200dd4c
	ldr	r6, [pc, #228]
.L_02002772:
	str	r6, [r0, #24]
	movs	r0, #161
	bl 0x0200decc
	movs	r1, #8
	movs	r0, #20
	bl 0x0200ddb4
	movs	r0, #20
	bl 0x0200dd2c
	movs	r1, #1
	movs	r0, #19
	bl 0x0200ddb4
	movs	r0, #19
	bl 0x0200dd4c
	movs	r1, #1
	bl 0x0200dcac
	movs	r0, #19
	movs	r1, #4
	movs	r2, #40
	bl 0x0200ddcc
	ldr	r1, [pc, #168]
	ldr	r2, [pc, #168]
	movs	r0, #19
	bl 0x0200dd5c
	movs	r0, #19
	bl 0x0200dd4c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #148
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	movs	r0, #19
	movs	r2, #186
	bl 0x0200dd9c
	movs	r0, #19
	ldr	r1, [pc, #132]
	ldr	r2, [pc, #136]
	bl 0x0200dd5c
.L_020027d6:
	movs	r1, #146
	movs	r2, #186
	lsls	r1, r1, #1
	movs	r0, #19
	bl 0x0200dd9c
	movs	r0, #19
.L_020027e4:
	bl 0x0200dd4c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r4, #1
	orrs	r3, r4
	strb	r3, [r0, #0]
	movs	r0, #19
	bl 0x0200dd4c
.L_020027f8:
	str	r6, [r0, #24]
	movs	r0, #161
	bl 0x0200decc
	movs	r1, #5
	movs	r0, #19
	bl 0x0200ddb4
	movs	r0, #20
	bl 0x0200dd2c
	movs	r1, #2
	movs	r0, #19
	bl 0x0200dddc
	movs	r0, #20
	bl 0x0200dd2c
	movs	r1, #1
	movs	r0, #20
	bl 0x0200dddc
	movs	r0, #80
	bl 0x0200dd2c
	movs	r0, #3
	movs	r1, #4
.L_0200282e:
	b.n	.L_02002860
	.4byte 0x0200e074
	.4byte 0x000027cf
	.4byte 0x00001001
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00000103
	.4byte 0x00000105
	.4byte 0x03001ebc
	.4byte 0x00003333
	.4byte 0x00001999
	.4byte 0xffff0000
	.2byte 0x0ccc
	.2byte 0x0000
.L_02002860:
	bl 0x0200ddbc
	movs	r2, #20
	movs	r0, #3
	movs	r1, #0
	bl 0x0200de0c
	movs	r1, #2
	movs	r0, #19
	bl 0x0200dddc
	movs	r0, #40
	bl 0x0200dd2c
	movs	r0, #19
	bl 0x02008894
	movs	r0, #0
	mov	r1, sl
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #192
	movs	r0, #2
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #224
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200de14
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #192
	movs	r0, #1
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #20
	movs	r0, #3
	bl 0x0200de14
	movs	r0, #20
	bl 0x0200dd4c
	movs	r2, #192
	lsls	r2, r2, #6
	adds	r7, r0, #0
	mov	fp, r2
	ldr	r0, [pc, #60]
	mov	r4, r8
	mov	r3, fp
	strh	r3, [r7, #6]
	str	r4, [r7, #24]
	movs	r1, #1
	mov	r9, r0
	movs	r6, #208
	movs	r0, #20
	bl 0x0200ddb4
	lsls	r6, r6, #8
	movs	r0, #10
	bl 0x0200dd2c
	movs	r0, #20
	adds	r1, r6, #0
	bl 0x020088a8
	movs	r1, #6
	movs	r2, #0
	movs	r0, #20
	bl 0x0200ddcc
	movs	r0, #10
	bl 0x0200dd2c
	movs	r0, #0
	mov	r1, sl
	movs	r2, #0
	b.n	.L_02002930
	.2byte 0x0000
	.2byte 0x0000
.L_02002930:
	bl 0x0200de14
	movs	r0, #1
	mov	r1, sl
	movs	r2, #0
	bl 0x0200de14
	movs	r0, #2
	mov	r1, sl
	movs	r2, #0
	bl 0x0200de14
	movs	r0, #3
	mov	r1, sl
	movs	r2, #0
	bl 0x0200de14
	movs	r0, #21
	adds	r1, r6, #0
	movs	r2, #0
	bl 0x0200de14
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200de14
	movs	r0, #128
	ldr	r2, [r7, #12]
	lsls	r0, r0, #12
	mov	r8, r0
	ldr	r1, [r7, #8]
	add	r2, r8
	ldr	r3, [r7, #16]
	movs	r0, #22
	bl 0x0200dc6c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020029f2
	ldr	r6, [r5, #80]
	adds	r3, r6, #0
	adds	r3, #39
	mov	r2, r9
	strb	r2, [r3, #0]
	movs	r3, #33
	ldrb	r2, [r6, #5]
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r3, r2
	adds	r1, r5, #0
	movs	r2, #13
	adds	r1, #35
	negs	r2, r2
	ands	r3, r2
	ldrb	r2, [r1, #0]
	strb	r3, [r6, #9]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r5, #0
	mov	r4, r9
	adds	r3, #85
	adds	r2, r5, #0
	strb	r4, [r3, #0]
	adds	r2, #92
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r3, [pc, #664]
	str	r3, [r5, #48]
	ldr	r3, [pc, #664]
	movs	r1, #193
	str	r3, [r5, #52]
	lsls	r1, r1, #3
	movs	r0, #17
	bl 0x0200dc34
	str	r0, [sp, #40]
	movs	r0, #220
	bl 0x0200dcec
	movs	r3, #128
	ldr	r2, [sp, #40]
	lsls	r3, r3, #3
	adds	r3, r2, r3
	ldrb	r0, [r6, #28]
	movs	r1, #128
	adds	r2, r3, #0
	str	r3, [sp, #36]
	bl 0x0200dc4c
	movs	r0, #17
	bl 0x0200dc3c
.L_020029f2:
	movs	r1, #1
	movs	r0, #22
	bl 0x0200de1c
	movs	r0, #22
	bl 0x0200dd4c
	movs	r6, #128
	ldr	r3, [r7, #8]
	lsls	r6, r6, #14
	str	r3, [r0, #8]
	str	r6, [r0, #12]
	ldr	r3, [r7, #16]
	str	r3, [r0, #16]
	adds	r3, r0, #0
	adds	r3, #85
	movs	r1, #3
	strb	r1, [r3, #0]
	ldr	r3, [pc, #576]
	ldr	r2, [pc, #576]
	str	r3, [r0, #48]
	movs	r3, #192
	lsls	r3, r3, #8
	str	r2, [r0, #52]
	str	r3, [r0, #24]
	str	r3, [r0, #28]
	cmp	r5, #0
	beq.n	.L_02002a4a
	adds	r3, r5, #0
	adds	r3, #85
	strb	r1, [r3, #0]
	ldr	r3, [pc, #556]
	mov	r4, r8
	str	r3, [r5, #72]
	movs	r1, #154
	movs	r3, #164
	str	r2, [r5, #68]
	str	r4, [r5, #40]
	adds	r0, r5, #0
	lsls	r1, r1, #17
	adds	r2, r6, #0
	lsls	r3, r3, #16
	bl 0x0200dc84
.L_02002a4a:
	movs	r1, #154
	movs	r0, #22
	lsls	r1, r1, #1
	movs	r2, #164
	bl 0x0200dd8c
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ddac
	movs	r0, #21
	movs	r1, #0
	bl 0x0200de1c
	cmp	r5, #0
	beq.n	.L_02002afc
	ldr	r0, [pc, #500]
	bl 0x0200decc
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200dcac
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	movs	r1, #157
	movs	r3, #137
	lsls	r1, r1, #17
	adds	r2, r6, #0
	lsls	r3, r3, #16
	adds	r0, r5, #0
	bl 0x0200dc84
	adds	r0, r5, #0
	bl 0x0200dc8c
	ldr	r0, [pc, #460]
	bl 0x0200decc
	ldr	r1, [r5, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	movs	r3, #146
	ldr	r1, [pc, #432]
	adds	r2, r6, #0
	lsls	r3, r3, #16
	adds	r0, r5, #0
	bl 0x0200dc84
	adds	r0, r5, #0
	bl 0x0200dc8c
	ldr	r0, [pc, #412]
	bl 0x0200decc
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	movs	r1, #150
	movs	r3, #154
	lsls	r1, r1, #17
	adds	r2, r6, #0
	lsls	r3, r3, #16
	adds	r0, r5, #0
	bl 0x0200dc84
	adds	r0, r5, #0
	bl 0x0200dc8c
	movs	r0, #6
	bl 0x0200dbfc
	movs	r0, #0
	str	r0, [r5, #8]
	str	r0, [r5, #12]
	str	r0, [r5, #16]
	adds	r0, r5, #0
	bl 0x0200d688
.L_02002afc:
	movs	r1, #128
	movs	r0, #21
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r2, #30
	movs	r0, #3
	lsls	r1, r1, #1
	bl 0x0200de24
	movs	r1, #1
	movs	r0, #21
	bl 0x0200de1c
	movs	r0, #21
	bl 0x0200dd4c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
.L_02002b5a:
	strb	r3, [r0, #0]
	movs	r1, #2
	movs	r0, #2
	bl 0x0200ddd4
	movs	r0, #2
	bl 0x02008894
	movs	r0, #3
	movs	r1, #1
	bl 0x0200dddc
	movs	r2, #40
	movs	r0, #3
	movs	r1, #0
	bl 0x0200de0c
	movs	r1, #1
	movs	r0, #20
	bl 0x0200dddc
	movs	r0, #20
	bl 0x0200dd2c
	movs	r0, #20
	bl 0x02008894
	movs	r1, #160
	movs	r0, #0
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #160
	movs	r0, #1
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r2, #0
	movs	r0, #6
	mov	r1, fp
	bl 0x0200de14
	movs	r0, #21
	mov	r1, fp
	bl 0x020088a8
	movs	r0, #21
	ldr	r1, [pc, #148]
	movs	r2, #0
	bl 0x0200de24
	movs	r0, #6
	ldr	r1, [pc, #140]
	movs	r2, #0
	bl 0x0200de24
	movs	r0, #0
	ldr	r1, [pc, #128]
	movs	r2, #0
	bl 0x0200de24
	movs	r0, #1
	ldr	r1, [pc, #120]
	movs	r2, #0
	bl 0x0200de24
	movs	r0, #2
	ldr	r1, [pc, #108]
	movs	r2, #0
	bl 0x0200de24
	movs	r0, #3
	ldr	r1, [pc, #100]
	movs	r2, #60
	bl 0x0200de24
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #20
	bl 0x0200de24
	movs	r0, #20
	bl 0x02008894
	movs	r0, #0
	mov	r1, sl
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #6
	movs	r2, #40
	bl 0x0200de14
	movs	r1, #0
	movs	r0, #1
	bl 0x0200ddfc
	movs	r0, #0
	movs	r1, #0
	bl 0x0200dd44
	cmp	r0, #0
	bne.n	.L_02002c70
	movs	r0, #1
	movs	r1, #3
	bl 0x0200ddb4
	movs	r7, #1
	b.n	.L_02002c8a
	.2byte 0x0000
	.4byte 0x00019999
	.4byte 0x0000cccc
	.4byte 0x00009999
	.4byte 0x00000135
	.4byte 0x011d0000
	.2byte 0x0101
	.2byte 0x0000
.L_02002c70:
	ldr	r3, [pc, #1008]
	ldr	r2, [r3, #0]
	movs	r3, #236
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r0, #1
	movs	r1, #4
	bl 0x0200ddb4
	movs	r7, #0
.L_02002c8a:
	movs	r0, #1
	bl 0x02008894
	cmp	r7, #0
	beq.n	.L_02002ca4
	ldr	r3, [pc, #972]
	movs	r4, #236
	ldr	r2, [r3, #0]
	lsls	r4, r4, #1
	adds	r2, r2, r4
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L_02002ca4:
	movs	r0, #20
	bl 0x0200dd2c
	movs	r0, #24
	bl 0x0200dd4c
	movs	r1, #0
	bl 0x0200dcac
	movs	r1, #7
	movs	r0, #24
	bl 0x0200dde4
	movs	r0, #24
	bl 0x0200dd4c
	movs	r4, #0
	adds	r7, r0, #0
	ldr	r0, [pc, #924]
	mov	r8, r4
	ldr	r2, [pc, #924]
	adds	r3, r7, #0
	str	r0, [r7, #28]
	mov	r9, r0
	adds	r3, #85
	mov	r0, r8
	str	r2, [r7, #24]
	mov	sl, r2
	strb	r0, [r3, #0]
	movs	r6, #152
	movs	r3, #128
	movs	r2, #158
	lsls	r2, r2, #16
	lsls	r3, r3, #15
	lsls	r6, r6, #17
	str	r3, [r7, #12]
	str	r2, [r7, #16]
	str	r6, [r7, #8]
	movs	r0, #25
	mov	fp, r2
	bl 0x0200dd4c
	movs	r1, #0
	bl 0x0200dcac
	movs	r1, #7
	movs	r0, #25
	bl 0x0200dde4
	movs	r0, #25
	bl 0x0200dd4c
	mov	r3, r9
	adds	r7, r0, #0
	str	r3, [r7, #28]
	adds	r3, r7, #0
	mov	r4, sl
	mov	r0, r8
	adds	r3, #85
	str	r4, [r7, #24]
	strb	r0, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #15
	mov	r2, fp
	movs	r1, #128
	str	r3, [r7, #12]
	str	r6, [r7, #8]
	str	r2, [r7, #16]
	movs	r0, #21
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #160
	movs	r0, #1
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #160
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #160
.L_02002d8c:
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
.L_02002d96:
	movs	r1, #208
	movs	r0, #21
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r2, #0
.L_02002da4:
	movs	r0, #6
	movs	r1, #0
	bl 0x0200de14
	ldr	r6, [pc, #704]
	movs	r0, #24
	adds	r1, r6, #0
	bl 0x0200dd64
	adds	r1, r6, #0
	movs	r0, #25
	bl 0x0200dd64
	movs	r0, #145
	bl 0x0200decc
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	bl 0x0200dcbc
	movs	r1, #0
	ldr	r0, [pc, #668]
	bl 0x0200de7c
	movs	r0, #16
	bl 0x0200de8c
	movs	r0, #20
	bl 0x0200dbfc
	movs	r1, #0
	ldr	r0, [pc, #652]
	bl 0x0200de7c
	movs	r0, #24
	bl 0x0200de8c
	movs	r0, #60
	bl 0x0200dbfc
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #632]
	bl 0x0200dc04
	movs	r0, #141
	bl 0x0200decc
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
.L_02002e12:
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	bl 0x0200dcbc
	movs	r1, #0
	ldr	r0, [pc, #596]
	bl 0x0200de7c
	movs	r0, #120
	bl 0x0200de8c
.L_02002e2a:
	ldr	r6, [pc, #596]
	movs	r0, #24
	adds	r1, r6, #0
	bl 0x0200dd64
	adds	r1, r6, #0
	movs	r0, #25
	bl 0x0200dd64
	movs	r0, #120
	bl 0x0200dbfc
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	lsls	r1, r1, #10
.L_02002e4e:
	bl 0x0200dcbc
	movs	r1, #0
	ldr	r0, [pc, #556]
	bl 0x0200de7c
	movs	r0, #120
.L_02002e5c:
	bl 0x0200de8c
	movs	r0, #120
	bl 0x0200dbfc
	movs	r0, #63
	bl 0x0200decc
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
.L_02002e74:
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	bl 0x0200dcbc
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
.L_02002e82:
	bl 0x0200de7c
	movs	r0, #120
	bl 0x0200de8c
	movs	r0, #120
	bl 0x0200dbfc
.L_02002e92:
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	lsls	r1, r1, #9
	bl 0x0200dcbc
	movs	r0, #19
	movs	r1, #1
	bl 0x0200ddb4
	movs	r1, #0
	movs	r2, #0
	movs	r0, #19
	bl 0x0200de14
	movs	r0, #19
.L_02002eb6:
	bl 0x0200dd4c
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #24]
	movs	r2, #40
	movs	r0, #19
	movs	r1, #4
	bl 0x0200ddcc
	movs	r0, #19
	movs	r1, #1
	bl 0x0200dddc
	movs	r0, #19
	movs	r1, #0
	movs	r2, #20
	bl 0x0200de0c
	movs	r1, #192
	movs	r0, #0
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #192
	movs	r0, #1
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #192
	movs	r0, #21
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #192
	movs	r2, #20
	movs	r0, #6
	lsls	r1, r1, #6
	bl 0x0200de14
	movs	r1, #1
	movs	r0, #20
	bl 0x0200dddc
	movs	r6, #128
	movs	r0, #20
	lsls	r6, r6, #8
	bl 0x02008894
	movs	r0, #3
	movs	r1, #1
	bl 0x0200dddc
	adds	r1, r6, #0
	movs	r0, #3
	bl 0x020088a8
	movs	r0, #3
	bl 0x02008894
	movs	r0, #20
	movs	r1, #0
	bl 0x020088a8
	movs	r1, #4
	movs	r0, #20
	bl 0x0200ddbc
	movs	r0, #20
	bl 0x02008894
	movs	r2, #20
	movs	r0, #19
	ldr	r1, [pc, #288]
	bl 0x0200de24
	movs	r1, #2
	movs	r0, #19
	bl 0x0200ddd4
	movs	r0, #19
	bl 0x02008894
	movs	r1, #128
.L_02002f7c:
	movs	r2, #20
	movs	r0, #1
	lsls	r1, r1, #1
	bl 0x0200de24
	movs	r4, #192
	lsls	r4, r4, #7
	mov	fp, r4
	mov	r1, fp
	movs	r0, #1
	bl 0x020088a8
	movs	r0, #1
	bl 0x02008894
	movs	r1, #208
	movs	r0, #20
	lsls	r1, r1, #8
	bl 0x020088a8
	movs	r0, #20
	movs	r1, #3
	bl 0x0200ddbc
	movs	r2, #20
	movs	r0, #20
	movs	r1, #0
	bl 0x0200de0c
	movs	r1, #1
	movs	r0, #1
	bl 0x0200dddc
	movs	r0, #1
	bl 0x02008894
	movs	r1, #176
	movs	r2, #20
	movs	r0, #19
	lsls	r1, r1, #8
	bl 0x0200de14
	movs	r1, #1
	movs	r0, #19
	bl 0x0200dddc
	ldr	r0, [pc, #176]
	mov	r8, r0
	bl 0x02008894
	movs	r0, #0
	movs	r1, #1
	bl 0x0200ddd4
	movs	r0, #1
	movs	r1, #1
	bl 0x0200ddd4
	movs	r0, #2
	movs	r1, #1
	bl 0x0200ddd4
	movs	r0, #3
	movs	r1, #1
	bl 0x0200dddc
	adds	r1, r6, #0
	movs	r0, #0
	movs	r2, #0
	bl 0x0200de14
	adds	r1, r6, #0
	movs	r0, #1
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #160
	movs	r0, #2
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200de14
	movs	r2, #160
	lsls	r2, r2, #8
	mov	r9, r2
	mov	r1, r9
	movs	r0, #3
	bl 0x020088a8
	ldr	r1, [pc, #96]
	movs	r2, #80
	movs	r0, #21
	bl 0x0200de24
	movs	r0, #21
	bl 0x02008894
	mov	r1, r9
	movs	r0, #0
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200de14
	movs	r0, #2
	mov	r1, fp
	movs	r2, #0
	bl 0x0200de14
	movs	r1, #224
	movs	r2, #40
	b.n	.L_02003094
	.4byte 0x03001ebc
	.4byte 0xffff0000
	.4byte 0x00001999
	.4byte 0x0200e088
	.4byte 0x004063ff
	.4byte 0x00007fff
	.4byte 0x0200b6d1
	.4byte 0x0200e0ac
	.4byte 0x00203210
	.4byte 0x00000103
	.4byte 0x00002013
	.2byte 0x0101
	.2byte 0x0000
.L_02003094:
	movs	r0, #3
	lsls	r1, r1, #8
	bl 0x0200de14
	movs	r1, #1
	movs	r0, #20
	bl 0x0200dddc
	movs	r0, #20
	bl 0x02008894
	adds	r1, r6, #0
	movs	r0, #0
	movs	r2, #0
	bl 0x0200de14
	adds	r1, r6, #0
	movs	r0, #1
	movs	r2, #0
	bl 0x0200de14
	movs	r7, #176
	adds	r1, r6, #0
	movs	r0, #2
	movs	r2, #0
	bl 0x0200de14
	lsls	r7, r7, #8
	movs	r2, #20
	adds	r1, r6, #0
	movs	r0, #3
	bl 0x0200de14
	adds	r1, r7, #0
	movs	r0, #20
	bl 0x020088a8
	ldr	r3, [pc, #1008]
	mov	sl, r3
	mov	r0, sl
	bl 0x02008894
	movs	r0, #21
	movs	r1, #3
	bl 0x0200ddbc
	adds	r1, r7, #0
	movs	r0, #21
	movs	r2, #60
	bl 0x0200de14
	movs	r1, #192
	movs	r2, #40
	movs	r0, #21
	lsls	r1, r1, #6
	bl 0x0200de14
	movs	r1, #1
	movs	r0, #19
	bl 0x0200dddc
	mov	r0, r8
	bl 0x02008894
.L_02003114:
	movs	r1, #0
	movs	r2, #40
	movs	r0, #21
	bl 0x0200de14
	movs	r0, #21
	bl 0x02008894
	movs	r1, #208
	movs	r2, #40
	movs	r0, #20
	lsls	r1, r1, #8
	bl 0x0200de14
	adds	r1, r7, #0
	movs	r0, #20
	bl 0x020088a8
	mov	r0, sl
	bl 0x02008894
	movs	r1, #192
	movs	r0, #21
	lsls	r1, r1, #6
	movs	r2, #40
	bl 0x0200de14
	movs	r1, #192
	movs	r2, #20
	movs	r0, #21
	lsls	r1, r1, #6
	bl 0x0200de14
	movs	r0, #21
	movs	r1, #3
	bl 0x0200ddbc
	movs	r0, #19
	movs	r1, #0
	movs	r2, #40
	bl 0x0200de14
	adds	r1, r7, #0
	movs	r0, #19
	movs	r2, #20
	bl 0x0200de14
	movs	r2, #40
	ldr	r1, [pc, #860]
	movs	r0, #19
	bl 0x0200de24
	mov	r0, r8
	bl 0x02008894
	movs	r1, #192
	movs	r0, #21
	lsls	r1, r1, #6
	bl 0x020088a8
	ldr	r1, [pc, #836]
	movs	r2, #20
	movs	r0, #21
	bl 0x0200de24
	movs	r0, #21
	bl 0x02008894
	movs	r2, #20
	adds	r1, r7, #0
	movs	r0, #20
	bl 0x0200de14
	movs	r1, #4
	movs	r0, #19
	bl 0x0200ddb4
	mov	r0, r8
	bl 0x02008894
	movs	r2, #80
	ldr	r1, [pc, #800]
	movs	r0, #21
.L_020031ba:
	bl 0x0200de24
	movs	r0, #21
	bl 0x02008894
	movs	r1, #3
	movs	r0, #19
	bl 0x0200ddbc
	mov	r0, r8
	bl 0x02008894
	movs	r2, #20
	movs	r0, #21
	ldr	r1, [pc, #768]
	bl 0x0200de24
	movs	r1, #2
	movs	r0, #21
	bl 0x0200ddd4
	movs	r0, #21
	bl 0x02008894
	movs	r1, #4
	movs	r0, #19
	bl 0x0200ddb4
	mov	r0, r8
	bl 0x02008894
	movs	r1, #129
	movs	r2, #60
	movs	r0, #21
	lsls	r1, r1, #1
	bl 0x0200de24
	movs	r1, #1
	movs	r0, #21
	bl 0x0200dddc
	movs	r0, #21
	bl 0x02008894
	movs	r1, #132
	movs	r2, #40
	movs	r0, #20
	lsls	r1, r1, #1
	bl 0x0200de24
	movs	r0, #20
	movs	r1, #4
	bl 0x0200ddb4
	movs	r2, #40
	mov	r0, sl
	movs	r1, #0
	bl 0x0200de0c
	movs	r1, #1
	movs	r0, #21
	bl 0x0200dddc
	movs	r0, #20
	bl 0x0200dd2c
	movs	r0, #21
	bl 0x02008894
	movs	r1, #128
	movs	r0, #0
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #1
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	movs	r1, #128
	movs	r0, #3
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200de24
	mov	r1, r9
	movs	r0, #2
	movs	r2, #0
	bl 0x0200de14
	mov	r1, r9
	movs	r0, #3
	movs	r2, #20
	bl 0x0200de14
	movs	r1, #192
	movs	r2, #20
	movs	r0, #19
	lsls	r1, r1, #6
	bl 0x0200de14
	movs	r0, #19
	movs	r1, #3
	bl 0x0200ddb4
	movs	r0, #20
	movs	r1, #3
	bl 0x0200ddbc
	adds	r1, r7, #0
	movs	r2, #20
	movs	r0, #21
	bl 0x0200de14
	ldr	r0, [pc, #556]
	bl 0x02008894
	movs	r1, #128
	adds	r2, r6, #0
	movs	r0, #6
	lsls	r1, r1, #9
	bl 0x0200dd5c
	movs	r1, #128
	adds	r2, r6, #0
	movs	r0, #21
	lsls	r1, r1, #9
	bl 0x0200dd5c
	ldr	r7, [pc, #528]
	movs	r0, #21
	adds	r1, r7, #0
	bl 0x0200dd64
	movs	r0, #20
	bl 0x0200dd2c
	adds	r1, r7, #0
	movs	r0, #6
	bl 0x0200dd64
	movs	r0, #1
	mov	r1, fp
	bl 0x020088a8
	movs	r1, #2
	movs	r0, #1
	bl 0x0200ddd4
	movs	r0, #1
	bl 0x02008894
	movs	r0, #17
	bl 0x0200decc
	movs	r1, #1
	ldr	r0, [pc, #480]
	bl 0x0200de7c
	movs	r0, #40
	bl 0x0200de8c
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r0, #20
	bl 0x020088a8
	movs	r0, #20
	bl 0x02008894
	movs	r0, #0
	mov	r1, fp
	movs	r2, #0
	bl 0x0200de14
	adds	r1, r6, #0
	movs	r0, #2
	movs	r2, #0
	bl 0x0200de14
	movs	r2, #0
	adds	r1, r6, #0
	movs	r0, #3
	bl 0x0200de14
	bl 0x0200b5c4
	movs	r0, #20
	movs	r1, #7
	bl 0x0200dde4
	movs	r1, #7
	movs	r0, #19
	bl 0x0200dde4
	movs	r0, #20
	bl 0x0200dd2c
	movs	r1, #128
	movs	r0, #20
	lsls	r1, r1, #1
	bl 0x0200dde4
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r0, #19
	bl 0x0200dde4
	movs	r0, #20
	bl 0x0200dd2c
	bl 0x0200b5c4
	movs	r2, #20
	movs	r1, #2
	movs	r0, #3
	bl 0x0200ddcc
	movs	r0, #3
	bl 0x02008894
	movs	r1, #0
	movs	r0, #19
	bl 0x020088a8
	movs	r0, #19
	bl 0x02008894
	bl 0x0200b5c4
	movs	r0, #19
	bl 0x0200dd4c
	add	r4, sp, #96
	movs	r3, #7
	mov	sl, r0
	str	r3, [r4, #4]
	movs	r0, #128
	ldr	r3, [pc, #320]
	movs	r2, #84
	lsls	r0, r0, #9
	add	r2, sp
	mov	r9, r2
	str	r3, [r4, #36]
	str	r0, [r4, #8]
	str	r0, [r4, #12]
	mov	r8, r4
	movs	r7, #0
	mov	r6, r9
.L_020033bc:
	lsls	r3, r7, #12
	adds	r0, r3, #0
	str	r3, [sp, #32]
	bl 0x0200dc24
	movs	r3, #0
	str	r3, [r6, #4]
	str	r0, [r6, #0]
	ldr	r0, [sp, #32]
	bl 0x0200dc1c
	ldr	r3, [r6, #0]
	lsls	r2, r3, #1
	adds	r3, r3, r2
	lsls	r0, r0, #1
	str	r0, [r6, #8]
	str	r3, [r6, #0]
	mov	r4, sl
	ldr	r4, [r4, #8]
	str	r4, [sp, #28]
	mov	r2, sl
	ldr	r1, [r2, #12]
	ldr	r4, [r6, #4]
	ldr	r2, [r2, #16]
	str	r0, [sp, #4]
	ldr	r0, [pc, #252]
	str	r4, [sp, #0]
	str	r0, [sp, #8]
	mov	r4, r8
	ldr	r0, [sp, #28]
	adds	r7, #1
	str	r4, [sp, #12]
.L_020033fc:
	bl 0x0200813c
	cmp	r7, #16
	bls.n	.L_020033bc
	movs	r0, #212
	bl 0x0200decc
	movs	r0, #6
	bl 0x0200dbfc
	bl 0x0200b5c4
	movs	r0, #20
	bl 0x0200dd4c
	mov	sl, r0
	add	r0, sp, #44
	movs	r3, #7
	str	r3, [r0, #4]
.L_02003422:
	ldr	r3, [pc, #196]
	str	r3, [r0, #36]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #8]
	str	r3, [r0, #12]
	mov	r8, r0
	movs	r7, #0
	mov	r6, r9
.L_02003434:
	lsls	r2, r7, #12
	adds	r0, r2, #0
	str	r2, [sp, #24]
	bl 0x0200dc24
	movs	r3, #0
	str	r3, [r6, #4]
	str	r0, [r6, #0]
	ldr	r0, [sp, #24]
	bl 0x0200dc1c
	ldr	r3, [r6, #0]
	lsls	r2, r3, #1
	adds	r3, r3, r2
	lsls	r0, r0, #1
	str	r0, [r6, #8]
	str	r3, [r6, #0]
	mov	r4, sl
	ldr	r4, [r4, #8]
	str	r4, [sp, #20]
	mov	r2, sl
	ldr	r1, [r2, #12]
	ldr	r4, [r6, #4]
	ldr	r2, [r2, #16]
	str	r0, [sp, #4]
	ldr	r0, [pc, #132]
	str	r4, [sp, #0]
	str	r0, [sp, #8]
	mov	r4, r8
	ldr	r0, [sp, #20]
	adds	r7, #1
	str	r4, [sp, #12]
	bl 0x0200813c
.L_02003478:
	cmp	r7, #16
	bls.n	.L_02003434
	movs	r0, #212
	bl 0x0200decc
	movs	r2, #20
	movs	r1, #6
	movs	r0, #2
	bl 0x0200ddcc
	movs	r0, #54
	bl 0x0200decc
	movs	r0, #2
	bl 0x02008894
	movs	r1, #4
	movs	r0, #20
	bl 0x0200ddb4
	movs	r0, #20
	bl 0x02008894
	bl 0x0200b5c4
	movs	r0, #20
	ldr	r1, [pc, #64]
	ldr	r2, [pc, #68]
	bl 0x0200dd5c
	ldr	r1, [pc, #56]
	ldr	r2, [pc, #60]
	movs	r0, #19
	bl 0x0200dd5c
	movs	r0, #20
	bl 0x0200dd4c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r6, #254
	adds	r3, r6, #0
	b.n	.L_020034f8
	.2byte 0x0000
	.4byte 0x00002014
	.4byte 0x00000101
	.4byte 0x00000103
	.4byte 0x0000a015
	.4byte 0x0200e22c
	.4byte 0x0040250d
	.4byte 0x020083a1
	.4byte 0x01090000
	.4byte 0x00003333
	.2byte 0x1999
	.2byte 0x0000
.L_020034f8:
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #19
	bl 0x0200dd4c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #254
	ands	r2, r3
	movs	r1, #147
	str	r6, [sp, #16]
	lsls	r1, r1, #1
	strb	r2, [r0, #0]
	movs	r0, #20
	movs	r2, #196
	bl 0x0200dd84
	movs	r1, #147
	movs	r2, #196
	movs	r0, #19
	lsls	r1, r1, #1
.L_02003522:
	bl 0x0200dd84
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #128]
	bl 0x0200dc04
	movs	r0, #1
	movs	r1, #2
	bl 0x0200ddd4
	movs	r1, #0
.L_0200353a:
	movs	r0, #1
	bl 0x0200de04
	movs	r0, #141
	lsls	r0, r0, #2
	bl 0x0200dd14
	movs	r1, #0
	movs	r0, #2
	bl 0x0200de04
	ldr	r0, [pc, #92]
	bl 0x0200dd14
	bl 0x0200b5c4
	movs	r0, #20
	bl 0x0200dd2c
	bl 0x0200b5c4
	movs	r0, #20
	bl 0x0200dd2c
	ldr	r3, [pc, #72]
	ldr	r4, [pc, #72]
	movs	r2, #3
	adds	r3, r3, r4
	strb	r2, [r3, #0]
	ldr	r6, [pc, #68]
	movs	r1, #3
	adds	r0, r6, #0
	bl 0x0200de6c
	adds	r0, r6, #0
	movs	r1, #9
	bl 0x0200de74
	movs	r1, #0
	movs	r0, #98
	bl 0x0200de64
	bl 0x0200dd24
	ldr	r0, [pc, #44]
	bl 0x0200dd14
	add	sp, #136
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x0200b7c5
	.4byte 0x00000235
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x000000bb
	.2byte 0x0351
	.2byte 0x0000
	.global Func_020035c4
	.thumb_func
Func_020035c4:
	push {lr}
	movs r0, #187
	bl 0x0200decc
	movs r1, #1
	ldr r0, [pc, #40]
	bl 0x0200de7c
	movs r0, #1
	bl 0x0200de8c
	movs r0, #4
	bl 0x0200dbfc
	movs r1, #1
	ldr r0, [pc, #24]
	bl 0x0200de7c
	movs r0, #1
	bl 0x0200de8c
	movs r0, #1
	bl 0x0200dbfc
	pop {r0}
	bx r0
	.4byte 0x00007fff
	.4byte 0x0040250d
	.global Func_02003600
	.thumb_func
Func_02003600:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	adds r5, r0, #0
	movs r0, #24
	bl 0x0200dd4c
	adds r6, r5, #0
	adds r6, #100
	ldrh r1, [r6]
	mov r8, r1
	mov r10, r0
	mov r0, r8
	bl 0x0200dc24
	ldr r3, [r5, #48]
	adds r3, #3
	adds r2, r3, #0
	muls r2, r0
	mov r1, r10
	ldr r3, [r1, #8]
	mov r0, r8
	adds r3, r3, r2
	str r3, [r5, #8]
	bl 0x0200dc1c
	mov r2, r10
	ldr r3, [r2, #16]
	lsls r0, r0, #1
	ldr r2, [r5, #8]
	adds r3, r3, r0
	str r3, [r5, #16]
	str r2, [r5, #56]
	str r3, [r5, #64]
	ldr r1, [pc, #20]
	ldrh r3, [r6]
	adds r3, r3, r1
	strh r3, [r6]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xfffff800
	.global Func_02003660
	.thumb_func
Func_02003660:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r7, r0, #0
	movs r0, #23
	bl 0x0200dd4c
	adds r5, r7, #0
	adds r5, #100
	ldrh r6, [r5]
	mov r8, r0
	adds r0, r6, #0
	bl 0x0200dc24
	movs r1, #98
	adds r1, r1, r7
	ldrb r2, [r1]
	ldr r3, [r7, #48]
	adds r3, r3, r2
	adds r3, #6
	adds r2, r3, #0
	muls r2, r0
	mov r10, r1
	mov r1, r8
	ldr r3, [r1, #8]
	adds r3, r3, r2
	str r3, [r7, #8]
	adds r0, r6, #0
	bl 0x0200dc1c
	mov r2, r10
	ldrb r3, [r2]
	adds r3, #4
	adds r2, r3, #0
	muls r2, r0
	mov r1, r8
	ldr r3, [r1, #16]
	adds r3, r3, r2
	ldr r2, [r7, #8]
	str r3, [r7, #16]
	str r2, [r7, #56]
	str r3, [r7, #64]
	ldr r2, [pc, #20]
	ldrh r3, [r5]
	adds r3, r3, r2
	strh r3, [r5]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xfffff800
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r0, [pc, #52]
	bl 0x0200dd0c
	cmp	r0, #0
	bne.n	.L_020036ee
	ldr	r3, [pc, #44]
	movs	r1, #3
	ldr	r0, [r3, #0]
	bl 0x0200dbf4
	cmp	r0, #0
	bne.n	.L_020037b8
.L_020036ee:
	movs	r0, #24
	bl 0x0200dd4c
	adds	r5, r0, #0
	ldr	r0, [pc, #20]
	bl 0x0200dd0c
	cmp	r0, #0
	beq.n	.L_02003714
	bl 0x0200dc14
	adds	r2, r0, #0
	lsls	r2, r2, #8
	b.n	.L_0200371c
	.2byte 0x0000
	.4byte 0x00000236
	.2byte 0x1e40
	.2byte 0x0300
.L_02003714:
	bl 0x0200dc14
	adds	r2, r0, #0
	lsls	r2, r2, #6
.L_0200371c:
	ldr	r3, [r5, #12]
	lsrs	r2, r2, #16
	lsls	r2, r2, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #128]
	movs	r0, #142
	adds	r2, r2, r3
	ldr	r1, [r5, #8]
	ldr	r3, [r5, #16]
	lsls	r0, r0, #1
	bl 0x0200dc6c
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_020037b8
	ldr	r1, [pc, #112]
	adds	r0, r7, #0
	ldr	r6, [r7, #80]
	bl 0x0200dc64
	movs	r1, #1
	adds	r0, r7, #0
	bl 0x0200ddec
	adds	r3, r7, #0
	adds	r3, #85
	movs	r5, #0
	strb	r5, [r3, #0]
	bl 0x0200dc14
	ldr	r3, [pc, #84]
	adds	r2, r7, #0
	adds	r2, #100
	ands	r3, r0
	strh	r3, [r2, #0]
	adds	r3, r7, #0
	adds	r3, #102
	strh	r5, [r3, #0]
	ldr	r3, [pc, #72]
	ldr	r1, [pc, #56]
	str	r3, [r7, #108]
	mov	r8, r1
	bl 0x0200dc14
	adds	r3, r0, #0
	lsls	r0, r3, #16
	subs	r0, r0, r3
	lsrs	r0, r0, #20
	bl 0x0200dc1c
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
.L_02003786:
	asrs	r3, r3, #16
	str	r3, [r7, #48]
	adds	r3, r6, #0
	adds	r3, #38
	mov	r2, r8
	strb	r2, [r3, #0]
	movs	r3, #13
	ldrb	r2, [r6, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r6, #9]
	b.n	.L_020037b8
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xffe40000
	.4byte 0x0200e16c
	.4byte 0x0ffff000
	.2byte 0xb601
	.2byte 0x0200
.L_020037b8:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.global Func_020037c4
	.thumb_func
Func_020037c4:
	push {r5, r6, lr}
	movs r0, #20
.L_020037c8:
	bl 0x0200dd4c
	adds r5, r0, #0
	movs r0, #19
	bl 0x0200dd4c
	movs r2, #128
	ldr r3, [r5, #56]
	lsls r2, r2, #24
	adds r6, r0, #0
	cmp r3, r2
.L_020037de:
	bne .L_020037de_0
	ldr r2, [r5, #60]
.L_020037de_0:
	movs r2, #128
	ldr r3, [r6, #56]
	lsls r2, r2, #24
	cmp r3, r2
	bne 0x0200b7fa
.L_020037ec:
	ldr r2, [r6, #60]
	cmp r2, r3
	bne 0x0200b7fa
	ldr r3, [r6, #64]
.L_020037f4:
	movs r1, #1
	cmp r3, r2
	beq 0x0200b7fc
.L_020037fa:
	movs r1, #0
	cmp r1, #0
	beq 0x0200b8ae
	movs r3, #0
	strh r3, [r5, #6]
	ldr r0, [pc, #172]
	strh r3, [r6, #6]
	bl 0x0200dd0c
.L_0200380c:
	cmp r0, #0
	beq 0x0200b846
	movs r0, #20
	movs r1, #7
	bl 0x0200dde4
	movs r0, #19
	movs r1, #7
	bl 0x0200dde4
	movs r2, #160
	ldr r3, [r5, #24]
	lsls r2, r2, #9
	cmp r3, r2
	bge 0x0200b874
	movs r2, #128
.L_0200382c:
	lsls r2, r2, #2
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r2
	str r3, [r5, #28]
	ldr r3, [r6, #24]
.L_0200383a:
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	b 0x0200b874
	.2byte 0x4b1c
	.2byte 0x681b
.L_0200384a:
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq 0x0200b864
	movs r0, #20
	movs r1, #15
.L_02003856:
	bl 0x0200dde4
	movs r0, #19
	movs r1, #0
	bl 0x0200dde4
	b .L_02003856_0
	.2byte 0x2014
	.2byte 0x2100
	.2byte 0xf002
	.2byte 0xfabc
	.2byte 0x2013
	.2byte 0x210f
	.2byte 0xf002
	.2byte 0xfab8
.L_02003856_0:
	movs r0, #141
	lsls r0, r0, #2
	bl 0x0200dd0c
	cmp r0, #0
	beq .L_02003856_1
	movs r2, #156
	ldr r3, [r5, #8]
	lsls r2, r2, #17
	cmp r3, r2
	bge .L_02003856_2
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r5, #8]
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r6, #8]
.L_02003856_2:
	movs r2, #182
	ldr r3, [r5, #16]
	lsls r2, r2, #16
	cmp r3, r2
	ble .L_02003856_1
	ldr r2, [pc, #24]
	adds r3, r3, r2
	str r3, [r5, #16]
	ldr r3, [r6, #16]
	adds r3, r3, r2
	str r3, [r6, #16]
.L_02003856_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0235
	.2byte 0x0000
	.2byte 0x1e40
	.2byte 0x0300
	.4byte 0xfffff000
	.global Func_020038c0
	.thumb_func
Func_020038c0:
	push {lr}
	bl 0x0200dd34
	bl 0x0200bbc8
	bl 0x0200be9c
	bl 0x0200c23c
	bl 0x0200dd3c
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020038dc
	.thumb_func
Func_020038dc:
	push {lr}
	bl 0x0200dd34
	bl 0x0200c8d8
	bl 0x0200cb28
	movs r0, #141
	lsls r0, r0, #1
	bl 0x0200dd14
	ldr r3, [pc, #44]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #64
	str r3, [r2]
	subs r3, #56
	adds r2, r1, r3
	movs r3, #24
	str r3, [r2]
	bl 0x0200de9c
	bl 0x0200dea4
	movs r0, #1
	bl 0x0200de5c
	bl 0x0200dd3c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.global Func_02003924
	.thumb_func
Func_02003924:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	movs r0, #0
	sub sp, #8
	bl 0x0200dd4c
	mov r10, r0
	bl 0x0200dd34
	bl 0x0200de4c
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r2, #23
	movs r3, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #102
	movs r1, #4
	movs r2, #74
	movs r3, #4
	bl 0x0200dc94
	movs r3, #16
	movs r2, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #39
	movs r1, #72
	movs r2, #11
	movs r3, #72
	bl 0x0200dc94
	movs r2, #22
	str r2, [sp, #0]
	mov r8, r2
	movs r6, #6
	movs r0, #19
	movs r1, #6
	movs r2, #3
.L_0200397a:
	movs r3, #7
	str r6, [sp, #4]
	bl 0x0200dc9c
	movs r5, #13
.L_02003984:
	movs r0, #19
	movs r1, #6
	movs r2, #3
	movs r3, #7
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200dc9c
	mov r3, r8
	str r3, [sp, #0]
	movs r0, #19
	movs r1, #6
	movs r2, #3
	movs r3, #7
	str r5, [sp, #4]
	bl 0x0200dc9c
.L_020039a6:
	movs r1, #6
	movs r2, #3
	movs r3, #7
	movs r0, #19
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200dc9c
.L_020039b6:
	movs r0, #1
	bl 0x0200dbfc
	movs r0, #192
	movs r2, #238
	lsls r2, r2, #16
	movs r3, #0
	ldr r1, [pc, #492]
	lsls r0, r0, #16
	bl 0x0200de3c
.L_020039cc:
	movs r0, #1
	bl 0x0200dbfc
	bl 0x0200dc7c
	movs r0, #1
	bl 0x0200dbfc
	movs r0, #20
.L_020039de:
	bl 0x0200dd54
	movs r0, #19
	bl 0x0200dd54
	movs r1, #19
	movs r0, #0
	bl 0x0200ddb4
	movs r0, #0
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r3, #173
	mov r2, r10
	lsls r3, r3, #17
	str r3, [r2, #8]
	movs r3, #205
	lsls r3, r3, #16
	str r3, [r2, #16]
	movs r5, #128
	movs r3, #192
	lsls r3, r3, #7
	lsls r5, r5, #14
	str r5, [r2, #12]
.L_02003a14:
	strh r3, [r2, #6]
	mov r0, r10
	bl 0x0200d688
	movs r1, #18
	movs r0, #1
	bl 0x0200ddb4
	movs r0, #1
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r0, #1
.L_02003a32:
	bl 0x0200dd4c
	movs r3, #178
	lsls r3, r3, #17
	str r3, [r0, #8]
	movs r3, #192
	lsls r3, r3, #16
	str r3, [r0, #16]
	movs r3, #160
	lsls r3, r3, #8
.L_02003a46:
	strh r3, [r0, #6]
	str r5, [r0, #12]
	bl 0x0200d688
	movs r1, #18
	movs r0, #2
	bl 0x0200ddb4
.L_02003a56:
	movs r0, #2
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r0, #2
.L_02003a64:
	bl 0x0200dd4c
	movs r3, #180
	lsls r3, r3, #17
	str r3, [r0, #8]
	movs r6, #222
	movs r3, #128
	lsls r3, r3, #6
	lsls r6, r6, #16
	strh r3, [r0, #6]
	str r5, [r0, #12]
	str r6, [r0, #16]
.L_02003a7c:
	bl 0x0200d688
	movs r1, #18
	movs r0, #3
	bl 0x0200ddb4
	movs r0, #3
.L_02003a8a:
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r0, #3
	bl 0x0200dd4c
	movs r3, #167
	lsls r3, r3, #17
	str r3, [r0, #8]
	movs r3, #128
	lsls r3, r3, #8
.L_02003aa4:
	strh r3, [r0, #6]
	str r5, [r0, #12]
	str r6, [r0, #16]
	bl 0x0200d688
	movs r1, #196
	movs r2, #220
	lsls r2, r2, #16
	movs r0, #21
	lsls r1, r1, #16
	bl 0x0200ddac
	movs r0, #21
	movs r1, #5
	bl 0x0200ddb4
	movs r1, #188
	movs r2, #158
	lsls r2, r2, #17
	movs r0, #6
	lsls r1, r1, #16
	bl 0x0200ddac
	movs r1, #5
	movs r0, #6
.L_02003ad6:
	bl 0x0200ddb4
	movs r0, #6
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r0, #8
	bl 0x0200dd4c
	ldr r5, [pc, #200]
	ldr r3, [r0, #8]
	adds r3, r3, r5
	str r3, [r0, #8]
	bl 0x0200d688
	movs r0, #9
	bl 0x0200dd4c
	ldr r3, [r0, #8]
	adds r3, r3, r5
	str r3, [r0, #8]
	bl 0x0200d688
.L_02003b08:
	movs r0, #10
	bl 0x0200dd4c
	movs r5, #128
	ldr r3, [r0, #8]
	lsls r5, r5, #13
	adds r3, r3, r5
	str r3, [r0, #8]
	bl 0x0200d688
	movs r0, #11
	bl 0x0200dd4c
	ldr r3, [r0, #8]
	adds r3, r3, r5
	str r3, [r0, #8]
	bl 0x0200d688
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #9
	lsls r0, r0, #9
	bl 0x0200dcbc
	movs r0, #23
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r0, #23
	bl 0x0200dd4c
	movs r3, #4
	adds r0, #85
	strb r3, [r0]
	movs r1, #4
	movs r0, #23
	bl 0x0200dde4
	movs r0, #23
	bl 0x0200dd4c
	movs r3, #160
	lsls r3, r3, #14
	movs r1, #200
	str r3, [r0, #12]
	lsls r1, r1, #4
	ldr r0, [pc, #76]
	bl 0x0200dc04
	ldr r3, [pc, #76]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #64
	str r3, [r2]
	subs r3, #56
	adds r2, r1, r3
	movs r3, #24
	str r3, [r2]
	bl 0x0200de94
.L_02003b8c:
	bl 0x0200dea4
	movs r0, #40
.L_02003b92:
	bl 0x0200dd2c
	bl 0x0200cbec
	ldr r0, [pc, #40]
	bl 0x0200dd14
	movs r0, #2
	bl 0x0200de5c
	sub sp, #-8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
.L_02003bb0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0xffc0
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xda29
	.2byte 0x0200
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x09a7
	.2byte 0x0000
	.global Func_02003bc8
	.thumb_func
Func_02003bc8:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	movs r0, #141
	sub sp, #8
	bl 0x0200decc
	movs r3, #17
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #2
	movs r1, #10
	movs r2, #4
	movs r0, #17
	bl 0x0200dc9c
	movs r0, #0
	bl 0x0200dd4c
	movs r5, #160
	lsls r5, r5, #8
	ldr r2, [pc, #60]
	strh r5, [r0, #6]
	movs r0, #1
	mov r9, r2
	bl 0x0200dd4c
	movs r1, #164
	movs r2, #168
	strh r5, [r0, #6]
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #1
	bl 0x0200ddac
.L_02003c16:
	movs r0, #2
	bl 0x0200dd4c
	movs r1, #170
	movs r2, #196
	strh r5, [r0, #6]
	lsls r1, r1, #17
.L_02003c24:
	lsls r2, r2, #16
	movs r0, #2
	bl 0x0200ddac
	movs r0, #3
	bl 0x0200dd4c
.L_02003c32:
	movs r1, #163
	movs r2, #204
	b .L_02003c32_0
	.2byte 0x0000
	.2byte 0x0000
.L_02003c32_0:
	strh r5, [r0, #6]
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #3
	bl 0x0200ddac
	movs r0, #21
	bl 0x0200dd4c
	movs r5, #208
	lsls r5, r5, #8
	movs r1, #200
	movs r2, #216
	strh r5, [r0, #6]
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #21
	bl 0x0200ddac
	movs r0, #6
	bl 0x0200dd4c
	movs r1, #200
	movs r2, #216
	strh r5, [r0, #6]
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #6
	bl 0x0200ddac
	movs r0, #20
	bl 0x0200dd4c
	movs r5, #192
	lsls r5, r5, #6
	movs r1, #155
	movs r2, #158
	strh r5, [r0, #6]
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #20
	bl 0x0200ddac
	movs r0, #19
	bl 0x0200dd4c
	movs r1, #146
	movs r2, #158
	lsls r2, r2, #16
	strh r5, [r0, #6]
	lsls r1, r1, #17
	movs r0, #19
	bl 0x0200ddac
	movs r0, #24
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r0, #24
	movs r1, #7
	bl 0x0200dde4
	movs r1, #1
	movs r0, #24
	bl 0x0200de1c
	movs r0, #24
	bl 0x0200dd4c
	ldr r3, [pc, #448]
	ldr r6, [pc, #448]
	str r3, [r0, #28]
	mov r8, r3
	adds r3, r0, #0
	mov r2, r9
	adds r3, #85
	str r6, [r0, #24]
	strb r2, [r3]
	movs r3, #152
	lsls r3, r3, #17
	mov r10, r3
	str r3, [r0, #8]
	movs r5, #192
	movs r3, #128
	lsls r3, r3, #10
	lsls r5, r5, #15
	str r3, [r0, #12]
	str r5, [r0, #16]
	movs r0, #25
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r0, #25
	movs r1, #7
	bl 0x0200dde4
	movs r1, #1
	movs r0, #25
	bl 0x0200de1c
	movs r0, #25
	bl 0x0200dd4c
	mov r2, r8
	adds r3, r0, #0
	str r2, [r0, #28]
	adds r3, #85
	mov r2, r9
	str r6, [r0, #24]
	strb r2, [r3]
	mov r3, r10
	str r3, [r0, #8]
	movs r3, #136
	lsls r3, r3, #14
	movs r1, #200
.L_02003d2a:
	str r3, [r0, #12]
	str r5, [r0, #16]
	lsls r1, r1, #4
	ldr r0, [pc, #352]
	bl 0x0200dc04
	bl 0x0200de4c
	mov r2, r9
	adds r0, #85
	strb r2, [r0]
	movs r1, #128
	movs r2, #180
	movs r3, #0
	lsls r1, r1, #14
	lsls r2, r2, #16
	mov r0, r10
	bl 0x0200de3c
	movs r0, #1
	bl 0x0200dbfc
	bl 0x0200dc7c
	movs r0, #1
	bl 0x0200dbfc
	movs r0, #128
	lsls r0, r0, #9
	adds r1, r0, #0
	adds r2, r1, #0
	bl 0x0200dcbc
	bl 0x0200de94
	bl 0x0200dea4
	movs r0, #40
	bl 0x0200dd2c
	movs r1, #2
	movs r0, #20
	bl 0x0200dddc
	movs r0, #10
	bl 0x0200dd2c
	ldr r0, [pc, #268]
	bl 0x0200ddf4
	movs r0, #20
	bl 0x02008894
	movs r1, #3
	movs r0, #19
	bl 0x0200dddc
	movs r0, #20
	bl 0x0200dd2c
	movs r2, #40
	movs r1, #0
	movs r0, #19
	bl 0x0200de0c
	movs r0, #17
	bl 0x0200decc
	movs r0, #20
	bl 0x0200dd4c
	movs r3, #154
	adds r5, r0, #0
	lsls r3, r3, #17
	str r3, [r5, #8]
	movs r6, #152
	movs r3, #224
	lsls r6, r6, #16
	lsls r3, r3, #13
	str r3, [r5, #12]
	str r6, [r5, #16]
	movs r1, #10
	movs r0, #20
	mov r8, r3
	bl 0x0200ddb4
	movs r0, #20
	bl 0x0200dd2c
	movs r3, #153
	mov r2, r8
	lsls r3, r3, #17
	str r2, [r5, #12]
	str r3, [r5, #8]
	str r6, [r5, #16]
	movs r1, #11
	movs r0, #20
	bl 0x0200ddb4
	movs r0, #12
	bl 0x0200dd2c
	mov r3, r10
	str r3, [r5, #8]
	movs r3, #168
	lsls r3, r3, #13
	str r3, [r5, #12]
	movs r1, #12
	str r6, [r5, #16]
	movs r0, #20
	bl 0x0200ddb4
	movs r0, #8
	bl 0x0200dd2c
	movs r0, #20
	bl 0x0200dd54
	movs r0, #19
	bl 0x0200dd4c
	movs r3, #147
	adds r5, r0, #0
	mov r2, r8
	lsls r3, r3, #17
	str r2, [r5, #12]
	str r3, [r5, #8]
	movs r1, #8
	str r6, [r5, #16]
	movs r0, #19
	bl 0x0200ddb4
	movs r0, #20
	bl 0x0200dd2c
	movs r3, #150
	lsls r3, r3, #17
	str r3, [r5, #8]
	movs r3, #216
	lsls r3, r3, #13
	str r3, [r5, #12]
	movs r1, #9
	str r6, [r5, #16]
	movs r0, #19
	bl 0x0200ddb4
	movs r0, #12
	bl 0x0200dd2c
	mov r3, r10
	str r3, [r5, #8]
	movs r3, #136
	lsls r3, r3, #13
	str r3, [r5, #12]
	movs r1, #10
	str r6, [r5, #16]
	movs r0, #19
	bl 0x0200ddb4
	movs r0, #8
	bl 0x0200dd2c
	movs r0, #19
	bl 0x0200dd54
	movs r0, #160
	bl 0x0200dd2c
	sub sp, #-8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0x3333
	.2byte 0x0000
	.4byte 0x0200b6d1
	.4byte 0x00002809
	.global Func_02003e9c
	.thumb_func
Func_02003e9c:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	movs r1, #1
	movs r0, #1
	bl 0x0200dddc
	movs r0, #20
	bl 0x0200dd2c
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200de0c
	movs r1, #1
	movs r0, #2
	bl 0x0200dddc
	movs r0, #20
	bl 0x0200dd2c
	movs r1, #0
	movs r0, #2
	bl 0x0200ddfc
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #0
	bl 0x020088a8
	movs r0, #0
	movs r1, #0
	movs r5, #0
	bl 0x0200dd44
	cmp r0, #0
	bne .L_02003e9c_0
	movs r0, #20
	bl 0x0200dd2c
	movs r0, #2
	movs r1, #4
	bl 0x0200ddbc
	movs r5, #1
	b .L_02003e9c_1
.L_02003e9c_0:
	movs r0, #20
	bl 0x0200dd2c
	movs r0, #2
	movs r1, #1
	bl 0x0200dddc
	ldr r3, [pc, #756]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02003e9c_1:
	movs r0, #2
	bl 0x02008894
	cmp r5, #0
	beq .L_02003e9c_2
	ldr r3, [pc, #728]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02003e9c_2:
	movs r0, #1
	ldr r1, [pc, #716]
	ldr r2, [pc, #716]
	movs r6, #128
	bl 0x0200dd5c
	ldr r5, [pc, #712]
	lsls r6, r6, #6
	movs r2, #174
	movs r0, #1
	ldr r1, [pc, #708]
	bl 0x0200dd9c
	adds r1, r6, #0
	movs r0, #1
	bl 0x020088a8
	adds r0, r5, #0
	bl 0x02008894
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r1, #160
	movs r2, #0
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200de14
	movs r3, #192
	lsls r3, r3, #8
	mov r10, r3
	mov r1, r10
	movs r0, #3
	bl 0x020088a8
	movs r3, #128
	lsls r3, r3, #7
	mov r8, r3
	mov r1, r8
	movs r0, #1
	bl 0x020088a8
	adds r0, r5, #0
	bl 0x02008894
	movs r0, #1
	ldr r1, [pc, #632]
	movs r2, #40
	bl 0x0200de24
	adds r1, r6, #0
	movs r2, #20
	movs r0, #1
	bl 0x0200de14
	adds r0, r5, #0
	bl 0x02008894
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200de14
	ldr r0, [pc, #600]
	movs r1, #0
	movs r2, #40
	bl 0x0200de0c
	movs r2, #20
	adds r1, r6, #0
	movs r0, #1
	bl 0x0200de14
	adds r0, r5, #0
	bl 0x02008894
	movs r0, #17
	bl 0x0200decc
	movs r1, #4
	movs r0, #3
	bl 0x0200ddb4
	movs r0, #3
	bl 0x02008894
	mov r1, r8
	movs r0, #1
	movs r2, #80
	bl 0x0200de14
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #80
	bl 0x0200de14
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200de24
	movs r1, #192
	movs r2, #60
	movs r0, #0
	lsls r1, r1, #7
	bl 0x0200de14
	movs r0, #0
	movs r1, #2
	bl 0x0200dddc
	mov r1, r10
	movs r0, #2
	bl 0x020088a8
	movs r0, #2
	ldr r1, [pc, #496]
	movs r2, #60
	bl 0x0200de24
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #40
	bl 0x0200de14
	movs r2, #60
	movs r0, #2
	movs r1, #4
	bl 0x0200ddcc
	adds r1, r6, #0
	movs r0, #1
	bl 0x020088a8
	ldr r1, [pc, #456]
	movs r2, #40
	movs r0, #1
	bl 0x0200de24
	adds r0, r5, #0
	bl 0x02008894
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #40
	bl 0x0200de14
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200de24
	movs r1, #4
	movs r2, #40
	movs r0, #1
	bl 0x0200ddcc
	movs r0, #1
	bl 0x02008894
	movs r0, #21
	ldr r1, [pc, #380]
	ldr r2, [pc, #400]
	bl 0x0200dd5c
	movs r0, #21
	movs r1, #200
	movs r2, #188
	bl 0x0200dd84
	movs r2, #204
	movs r0, #6
	movs r1, #200
	bl 0x0200dd84
	ldr r0, [pc, #380]
	ldr r1, [pc, #372]
	bl 0x0200de34
	movs r0, #252
	movs r2, #190
	movs r3, #1
.L_020040c0:
	lsls r2, r2, #16
	movs r1, #0
	lsls r0, r0, #16
	bl 0x0200de3c
	bl 0x0200de44
	movs r0, #40
	bl 0x0200dd2c
	movs r0, #23
	bl 0x0200decc
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #21
	bl 0x0200de2c
	movs r0, #21
	bl 0x02008894
	movs r1, #1
	movs r0, #21
	bl 0x0200dddc
	movs r0, #20
	bl 0x0200dd2c
	movs r0, #21
	bl 0x02008894
	movs r1, #4
	movs r0, #21
	bl 0x0200ddbc
	movs r0, #21
	bl 0x02008894
	movs r1, #192
	movs r2, #20
	lsls r1, r1, #6
	movs r0, #21
	bl 0x0200de14
	movs r0, #21
	bl 0x02008894
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #6
	bl 0x0200de2c
	ldr r5, [pc, #264]
	movs r0, #20
	bl 0x0200dd2c
.L_02004130:
	movs r1, #2
	movs r0, #3
	bl 0x0200ddd4
	adds r0, r5, #0
	bl 0x02008894
	movs r1, #2
	movs r0, #21
	bl 0x0200dddc
	adds r6, #2
	movs r0, #20
	bl 0x0200dd2c
	adds r0, r6, #0
	bl 0x02008894
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #21
	bl 0x020088a8
	movs r1, #1
	movs r0, #1
	bl 0x0200dddc
	movs r0, #1
	bl 0x02008894
	movs r0, #21
	movs r1, #1
	bl 0x0200dddc
	movs r2, #20
	movs r1, #0
	movs r0, #21
	bl 0x0200de0c
	adds r0, r5, #0
	bl 0x02008894
	movs r1, #4
	movs r0, #21
	bl 0x0200ddbc
	movs r0, #21
	bl 0x02008894
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #2
	bl 0x0200de2c
	adds r0, r6, #0
	bl 0x02008894
	movs r1, #3
	movs r0, #21
	bl 0x0200ddb4
	movs r0, #21
	bl 0x02008894
	movs r2, #40
	movs r0, #1
	ldr r1, [pc, #128]
	bl 0x0200de24
	movs r1, #2
	movs r0, #1
	bl 0x0200ddd4
	movs r0, #1
	bl 0x02008894
	movs r1, #4
	movs r0, #21
	bl 0x0200ddb4
	movs r0, #21
	bl 0x02008894
	movs r1, #160
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200de14
	movs r0, #3
	movs r1, #4
	movs r2, #20
	bl 0x0200ddcc
	adds r0, r5, #0
	movs r1, #0
	movs r2, #20
	bl 0x0200de0c
	movs r0, #21
	ldr r1, [pc, #60]
	movs r2, #40
	bl 0x0200de24
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x9999
	.2byte 0x0001
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x1001
	.2byte 0x0000
	.2byte 0x0141
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0x8001
	.2byte 0x0000
	.2byte 0x0101
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x3333
	.2byte 0x0003
	.2byte 0x2003
	.2byte 0x0000
	.4byte 0x00000103
	.global Func_0200423c
	.thumb_func
Func_0200423c:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	movs r0, #19
	sub sp, #8
	bl 0x0200decc
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200decc
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200dcbc
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200de24
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200de24
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200de24
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200de24
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200de24
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #1
	movs r2, #10
	bl 0x0200de24
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r1, #208
	movs r0, #21
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r1, #208
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200dcbc
	movs r0, #10
	bl 0x0200dd2c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl 0x0200dcbc
	movs r0, #152
	movs r1, #128
	movs r2, #180
	movs r3, #1
	lsls r2, r2, #16
	lsls r1, r1, #14
	lsls r0, r0, #17
	bl 0x0200de3c
	bl 0x0200de44
	movs r0, #24
	bl 0x0200dd4c
	ldr r5, [pc, #1020]
	str r5, [r0, #24]
	movs r0, #25
	bl 0x0200dd4c
	str r5, [r0, #24]
	ldr r5, [pc, #1012]
	movs r0, #24
	adds r1, r5, #0
	bl 0x0200dd64
	adds r1, r5, #0
	movs r0, #25
	bl 0x0200dd64
	movs r0, #145
	bl 0x0200decc
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl 0x0200dcbc
	movs r1, #0
	ldr r0, [pc, #972]
	bl 0x0200de7c
	movs r0, #16
	bl 0x0200de8c
	movs r0, #20
	bl 0x0200dbfc
	movs r1, #0
	ldr r0, [pc, #956]
	bl 0x0200de7c
	movs r0, #24
	bl 0x0200de8c
	movs r0, #60
	bl 0x0200dbfc
	movs r0, #141
	bl 0x0200decc
	ldr r0, [pc, #936]
	bl 0x0200dd14
	movs r0, #24
	bl 0x0200dd4c
	ldr r3, [pc, #928]
	str r3, [r0, #12]
	movs r0, #25
	bl 0x0200dd4c
	ldr r3, [pc, #924]
	movs r1, #7
	str r3, [r0, #12]
	movs r0, #26
	bl 0x0200dde4
	movs r0, #26
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r0, #26
	bl 0x0200dd4c
	ldr r1, [pc, #896]
	adds r5, r0, #0
	str r1, [r5, #28]
	movs r0, #24
	mov r10, r1
	bl 0x0200dd4c
	ldr r3, [r0, #24]
	movs r2, #0
	mov r9, r2
	str r3, [r5, #24]
	adds r3, r5, #0
	adds r3, #85
	mov r1, r9
	strb r1, [r3]
	movs r2, #152
	ldr r3, [pc, #868]
	movs r6, #192
	lsls r2, r2, #17
	lsls r6, r6, #15
	str r2, [r5, #8]
	str r3, [r5, #12]
	str r6, [r5, #16]
	movs r1, #7
	movs r0, #27
	mov r8, r2
	bl 0x0200dde4
	movs r0, #27
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r0, #27
	bl 0x0200dd4c
	mov r3, r10
	adds r5, r0, #0
	str r3, [r5, #28]
	movs r0, #24
	bl 0x0200dd4c
	ldr r3, [r0, #24]
	str r3, [r5, #24]
	adds r3, r5, #0
	adds r3, #85
	mov r1, r9
	mov r2, r8
	strb r1, [r3]
	mov r3, r9
	str r2, [r5, #8]
	str r3, [r5, #12]
	str r6, [r5, #16]
	movs r1, #7
	movs r0, #28
	bl 0x0200dde4
	movs r0, #28
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r0, #28
	bl 0x0200dd4c
	mov r1, r10
	adds r5, r0, #0
	str r1, [r5, #28]
	movs r0, #24
	bl 0x0200dd4c
	ldr r3, [r0, #24]
	str r3, [r5, #24]
	adds r3, r5, #0
	adds r3, #85
	mov r2, r9
	strb r2, [r3]
	mov r3, r8
	str r3, [r5, #8]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r5, #12]
	movs r2, #23
	movs r3, #18
	str r6, [r5, #16]
	movs r0, #102
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #4
	movs r2, #74
	movs r3, #4
	bl 0x0200dc94
	movs r3, #16
	movs r2, #21
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #39
	movs r1, #72
	movs r2, #11
	movs r3, #72
	bl 0x0200dc94
	movs r1, #22
	str r1, [sp, #0]
	mov r8, r1
	movs r6, #6
	movs r0, #19
	movs r1, #6
	movs r2, #3
	movs r3, #7
	str r6, [sp, #4]
	bl 0x0200dc9c
	movs r5, #13
	movs r0, #19
	movs r1, #6
	movs r2, #3
	movs r3, #7
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200dc9c
	mov r2, r8
	str r2, [sp, #0]
	movs r0, #19
	movs r1, #6
	movs r2, #3
	movs r3, #7
	str r5, [sp, #4]
	bl 0x0200dc9c
	movs r1, #6
	movs r2, #3
	movs r3, #7
	movs r0, #19
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200dc9c
	movs r0, #1
	bl 0x0200dbfc
	movs r0, #8
	bl 0x0200dd4c
	ldr r5, [pc, #608]
	ldr r3, [r0, #8]
	adds r3, r3, r5
	str r3, [r0, #8]
	bl 0x0200d688
	movs r0, #9
	bl 0x0200dd4c
	ldr r3, [r0, #8]
	adds r3, r3, r5
	str r3, [r0, #8]
	bl 0x0200d688
	movs r0, #10
	bl 0x0200dd4c
	movs r5, #128
	ldr r3, [r0, #8]
	lsls r5, r5, #13
	adds r3, r3, r5
	str r3, [r0, #8]
	bl 0x0200d688
	movs r0, #11
	bl 0x0200dd4c
	ldr r3, [r0, #8]
	adds r3, r3, r5
	str r3, [r0, #8]
	bl 0x0200d688
	movs r0, #0
	bl 0x0200dd4c
	ldr r3, [r0, #8]
	adds r3, r3, r5
	str r3, [r0, #8]
	ldr r3, [r0, #16]
	adds r3, r3, r5
	str r3, [r0, #16]
	bl 0x0200d688
	movs r0, #1
	bl 0x0200dd4c
	ldr r3, [r0, #8]
	adds r3, r3, r5
	str r3, [r0, #8]
	ldr r3, [r0, #16]
	adds r3, r3, r5
	str r3, [r0, #16]
	bl 0x0200d688
	movs r0, #2
	bl 0x0200dd4c
	ldr r3, [r0, #8]
	adds r3, r3, r5
	str r3, [r0, #8]
	ldr r3, [r0, #16]
	adds r3, r3, r5
	str r3, [r0, #16]
	bl 0x0200d688
	movs r0, #3
	bl 0x0200dd4c
	ldr r3, [r0, #8]
	adds r3, r3, r5
	str r3, [r0, #8]
	ldr r3, [r0, #16]
	adds r3, r3, r5
	str r3, [r0, #16]
	bl 0x0200d688
	movs r1, #196
	movs r2, #220
	lsls r2, r2, #16
	movs r0, #21
	lsls r1, r1, #16
	bl 0x0200ddac
	movs r0, #21
	movs r1, #5
	bl 0x0200ddb4
	movs r1, #188
	movs r2, #158
	lsls r2, r2, #17
	movs r0, #6
	lsls r1, r1, #16
	bl 0x0200ddac
	movs r1, #5
	movs r0, #6
	bl 0x0200ddb4
	movs r0, #6
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl 0x0200dcbc
	movs r1, #0
	ldr r0, [pc, #356]
	bl 0x0200de7c
	movs r0, #120
	bl 0x0200de8c
	ldr r5, [pc, #376]
	movs r0, #24
	adds r1, r5, #0
	bl 0x0200dd64
	adds r1, r5, #0
	movs r0, #25
	bl 0x0200dd64
	adds r1, r5, #0
	movs r0, #26
	bl 0x0200dd64
	adds r1, r5, #0
	movs r0, #27
	bl 0x0200dd64
	adds r1, r5, #0
	movs r0, #28
	bl 0x0200dd64
	movs r0, #120
	bl 0x0200dbfc
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl 0x0200dcbc
	movs r1, #0
	ldr r0, [pc, #312]
	bl 0x0200de7c
	movs r0, #120
	bl 0x0200de8c
	movs r0, #120
	bl 0x0200dbfc
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl 0x0200dcbc
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl 0x0200de7c
	movs r0, #120
	bl 0x0200de8c
	movs r0, #120
	bl 0x0200dbfc
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #9
	lsls r0, r0, #9
	bl 0x0200dcbc
	movs r0, #24
	bl 0x0200dd4c
	ldr r3, [pc, #240]
	ldr r5, [pc, #240]
	str r3, [r0, #28]
	adds r1, r5, #0
	movs r0, #25
	bl 0x0200dd64
	adds r1, r5, #0
	movs r0, #26
	bl 0x0200dd64
	adds r1, r5, #0
	movs r0, #27
	bl 0x0200dd64
	adds r1, r5, #0
	movs r0, #28
	bl 0x0200dd7c
	ldr r0, [pc, #208]
	bl 0x0200decc
	movs r1, #15
	movs r0, #24
	bl 0x0200dde4
	movs r0, #20
	bl 0x0200dd2c
	ldr r1, [pc, #192]
	movs r0, #24
	bl 0x0200dd7c
	ldr r0, [pc, #188]
	bl 0x0200dc0c
	movs r1, #2
	movs r2, #20
	movs r0, #2
	bl 0x0200ddcc
	movs r0, #2
	bl 0x02008894
	movs r1, #192
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200de14
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200de2c
	movs r0, #20
	bl 0x0200dd2c
	movs r1, #2
	movs r0, #1
	bl 0x0200ddd4
	movs r0, #1
	bl 0x02008894
	movs r0, #3
	ldr r1, [pc, #128]
	ldr r2, [pc, #128]
	bl 0x0200dd5c
	movs r1, #163
	movs r2, #220
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200dd9c
	movs r0, #40
	bl 0x0200dd2c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200de2c
	movs r0, #3
	bl 0x02008894
	movs r0, #0
	bl 0x0200dd4c
	adds r2, r0, #0
	adds r3, r2, #0
	mov r1, r9
	adds r3, #98
	strb r1, [r3]
	b .L_0200423c_0
	.2byte 0x0000
	.4byte 0x00001999
	.4byte 0x0200e088
	.4byte 0x004063ff
	.4byte 0x00007fff
	.4byte 0x00000236
	.4byte 0xffa00000
	.4byte 0xffc00000
	.4byte 0xffff0000
	.4byte 0xffe00000
	.4byte 0xfff00000
	.4byte 0x0200e0d0
	.4byte 0x00203210
	.4byte 0x0000051e
	.4byte 0x0200e0f4
	.4byte 0x00000121
	.4byte 0x0200e130
	.4byte 0x0200b6d1
	.4byte 0x0000cccc
	.4byte 0x00006666
.L_0200423c_0:
	movs r6, #1
	adds r3, #1
	strb r6, [r3]
	ldr r3, [r2, #12]
	movs r5, #160
	lsls r5, r5, #8
	str r3, [r2, #76]
	ldr r3, [pc, #60]
	strh r5, [r2, #6]
	movs r0, #1
	mov r8, r3
	bl 0x0200dd4c
	adds r2, r0, #0
	adds r3, r2, #0
	mov r1, r8
	adds r3, #98
	strb r1, [r3]
	adds r3, #1
	strb r6, [r3]
	ldr r3, [r2, #12]
	strh r5, [r2, #6]
	str r3, [r2, #76]
	movs r0, #2
	bl 0x0200dd4c
	adds r2, r0, #0
	adds r3, r2, #0
	mov r1, r8
	adds r3, #98
	strb r1, [r3]
	adds r3, #1
	strb r6, [r3]
	ldr r3, [r2, #12]
	strh r5, [r2, #6]
	str r3, [r2, #76]
	movs r0, #3
	b .L_0200423c_1
	.4byte 0x00000000
.L_0200423c_1:
	bl 0x0200dd4c
	adds r2, r0, #0
	adds r3, r2, #0
	mov r1, r8
	adds r3, #98
	strb r1, [r3]
	adds r3, #1
	strb r6, [r3]
	ldr r3, [r2, #12]
	strh r5, [r2, #6]
	str r3, [r2, #76]
	movs r0, #21
	bl 0x0200dd4c
	adds r2, r0, #0
	adds r3, r2, #0
	mov r1, r8
	adds r3, #98
	strb r1, [r3]
	adds r3, #1
	strb r6, [r3]
	ldr r3, [r2, #12]
	movs r0, #6
	str r3, [r2, #76]
	bl 0x0200dd4c
	adds r2, r0, #0
	adds r3, r2, #0
	mov r1, r8
	adds r3, #98
	strb r1, [r3]
	adds r3, #1
	strb r6, [r3]
	ldr r3, [r2, #12]
	movs r0, #23
	str r3, [r2, #76]
	bl 0x0200dd4c
	mov r2, r8
	adds r0, #85
	strb r2, [r0]
	movs r0, #23
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r0, #23
	movs r1, #7
	bl 0x0200dde4
	movs r0, #23
	movs r1, #2
	bl 0x0200de1c
	ldr r3, [pc, #112]
	mov r1, r9
	ldr r2, [pc, #112]
	str r1, [r3]
	movs r3, #240
	movs r1, #200
	str r3, [r2]
	ldr r0, [pc, #108]
	lsls r1, r1, #4
	bl 0x0200dc04
.L_0200423c_2:
	movs r0, #1
	bl 0x0200dbfc
	ldr r0, [pc, #96]
	bl 0x0200dd0c
	cmp r0, #0
	beq .L_0200423c_2
	ldr r0, [pc, #92]
	bl 0x0200dd14
	movs r0, #30
	bl 0x0200dd2c
	movs r0, #141
	lsls r0, r0, #1
	bl 0x0200dd14
	bl 0x0200debc
	ldr r0, [pc, #72]
	movs r1, #91
	bl 0x0200de54
	ldr r2, [pc, #68]
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	ldr r3, [pc, #64]
	movs r2, #228
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #1
	str r2, [r3]
	bl 0x0200de9c
	bl 0x0200dea4
	sub sp, #-8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0200e764
	.4byte 0x0200e760
	.4byte 0x0200d6a1
	.4byte 0x00000237
	.4byte 0x00000101
	.4byte 0x00000002
	.4byte 0x00007fff
	.4byte 0x03001ebc
	.global Func_020048d8
	.thumb_func
Func_020048d8:
	push {r5, r6, lr}
.L_020048da:
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	movs r0, #20
	sub sp, #8
	bl 0x0200dd54
	movs r0, #19
	bl 0x0200dd54
	movs r0, #141
	bl 0x0200decc
	movs r3, #17
	movs r2, #8
	str r3, [sp, #0]
.L_020048fe:
	str r2, [sp, #4]
	movs r0, #17
.L_02004902:
	movs r1, #10
	movs r2, #4
	movs r3, #2
	bl 0x0200dc9c
	movs r3, #18
	movs r2, #23
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #102
	movs r1, #4
	movs r2, #74
	movs r3, #4
	bl 0x0200dc94
	movs r3, #16
	movs r2, #21
	str r3, [sp, #0]
.L_02004926:
	str r2, [sp, #4]
	movs r0, #39
	movs r1, #72
	movs r2, #11
	movs r3, #72
	bl 0x0200dc94
	movs r2, #22
	str r2, [sp, #0]
	mov r8, r2
	movs r6, #6
	movs r0, #19
	movs r1, #6
	movs r2, #3
	movs r3, #7
	str r6, [sp, #4]
	bl 0x0200dc9c
	movs r5, #13
	movs r0, #19
	movs r1, #6
	movs r2, #3
	movs r3, #7
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200dc9c
	mov r3, r8
	str r3, [sp, #0]
	movs r0, #19
	movs r1, #6
	movs r2, #3
	movs r3, #7
	str r5, [sp, #4]
	bl 0x0200dc9c
	movs r3, #7
	movs r0, #19
	movs r1, #6
	movs r2, #3
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200dc9c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200dcbc
	movs r0, #1
.L_02004990:
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r1, r1
	negs r0, r0
	bl 0x0200de3c
	bl 0x0200dec4
	bl 0x0200dc7c
	movs r0, #1
	bl 0x0200dbfc
	movs r0, #138
.L_020049b0:
	bl 0x0200decc
	movs r0, #0
	movs r1, #19
	bl 0x0200ddb4
	movs r0, #1
	movs r1, #18
	bl 0x0200ddb4
	movs r0, #2
	movs r1, #18
	bl 0x0200ddb4
	movs r1, #18
	movs r0, #3
.L_020049d0:
	bl 0x0200ddb4
	movs r0, #0
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r0, #1
	bl 0x0200dd4c
.L_020049e6:
	movs r1, #0
	bl 0x0200dcac
	movs r0, #2
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r0, #3
	bl 0x0200dd4c
.L_020049fe:
	movs r1, #0
	bl 0x0200dcac
.L_02004a04:
	movs r0, #0
	bl 0x0200dd4c
	movs r3, #173
	adds r5, r0, #0
	lsls r3, r3, #17
	str r3, [r5, #8]
	movs r2, #128
	movs r3, #205
.L_02004a16:
	lsls r2, r2, #14
	lsls r3, r3, #16
	str r2, [r5, #12]
	str r3, [r5, #16]
	mov r10, r2
	bl 0x0200d688
.L_02004a24:
	adds r3, r5, #0
	movs r6, #0
	adds r3, #99
	strb r6, [r3]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #40]
	movs r0, #1
.L_02004a34:
	mov r8, r3
	bl 0x0200dd4c
	movs r3, #178
	adds r5, r0, #0
	lsls r3, r3, #17
	str r3, [r5, #8]
.L_02004a42:
	movs r3, #192
	mov r2, r10
	lsls r3, r3, #16
	str r2, [r5, #12]
	str r3, [r5, #16]
	bl 0x0200d688
	adds r3, r5, #0
	adds r3, #99
	strb r6, [r3]
	mov r3, r8
	str r3, [r5, #40]
.L_02004a5a:
	movs r0, #2
	bl 0x0200dd4c
	movs r3, #180
	adds r5, r0, #0
	lsls r3, r3, #17
	str r3, [r5, #8]
.L_02004a68:
	movs r3, #222
	mov r2, r10
	lsls r3, r3, #16
	str r2, [r5, #12]
	str r3, [r5, #16]
	mov r9, r3
	bl 0x0200d688
	adds r3, r5, #0
	mov r2, r8
	adds r3, #99
	strb r6, [r3]
	movs r0, #3
	str r2, [r5, #40]
	bl 0x0200dd4c
	movs r3, #167
	adds r5, r0, #0
	lsls r3, r3, #17
	mov r2, r9
	str r3, [r5, #8]
	mov r3, r10
	str r2, [r5, #16]
	str r3, [r5, #12]
	bl 0x0200d688
	adds r3, r5, #0
	adds r3, #99
	strb r6, [r3]
	mov r3, r8
	str r3, [r5, #40]
	movs r0, #21
	bl 0x0200dd4c
	adds r0, #98
	strb r6, [r0]
	movs r0, #6
	bl 0x0200dd4c
	adds r0, #98
	strb r6, [r0]
	movs r0, #23
	bl 0x0200dd4c
	adds r0, #85
	ldrb r2, [r0]
	movs r3, #4
	orrs r3, r2
	strb r3, [r0]
	movs r1, #4
	movs r0, #23
	bl 0x0200dde4
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #68]
	bl 0x0200dc04
	ldr r3, [pc, #64]
	movs r2, #228
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #1
	str r2, [r3]
	bl 0x0200de94
	ldr r0, [pc, #52]
	movs r1, #0
	bl 0x0200de84
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl 0x0200de7c
	movs r0, #40
	bl 0x0200de8c
	movs r0, #60
	bl 0x0200dbfc
	sub sp, #-8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0200da29
	.4byte 0x03001ebc
	.4byte 0x00007fff
	.global Func_02004b28
	.thumb_func
Func_02004b28:
	push {lr}
	ldr r0, [pc, #172]
	bl 0x0200ddf4
	movs r0, #21
	bl 0x02008894
	movs r0, #62
	bl 0x0200decc
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #9
	lsls r1, r1, #9
	bl 0x0200dcbc
	ldr r0, [pc, #140]
	ldr r1, [pc, #144]
	bl 0x0200de34
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl 0x0200de34
	movs r0, #192
	movs r2, #238
	movs r3, #1
	lsls r2, r2, #16
	ldr r1, [pc, #120]
	lsls r0, r0, #16
	bl 0x0200de3c
	bl 0x0200de44
	movs r0, #40
	bl 0x0200dd2c
	movs r0, #21
	movs r1, #1
	bl 0x0200dddc
	movs r2, #40
	ldr r0, [pc, #96]
	movs r1, #0
	bl 0x0200de0c
	movs r1, #3
	movs r0, #6
	bl 0x0200dddc
	movs r0, #6
	bl 0x02008894
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #21
	bl 0x0200de2c
	movs r0, #60
	bl 0x0200dd2c
	movs r2, #80
	ldr r0, [pc, #56]
	movs r1, #0
	bl 0x0200de0c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #6
	bl 0x0200de2c
	movs r0, #40
	bl 0x0200dd2c
	movs r0, #6
	movs r1, #2
	bl 0x0200ddd4
	movs r0, #6
	bl 0x02008894
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00002829
	.4byte 0x0004cccc
	.4byte 0x00009999
	.4byte 0xffc00000
	.4byte 0x00002015
	.global Func_02004bec
	.thumb_func
Func_02004bec:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #0
	ldr r0, [pc, #1004]
	sub sp, #8
	mov r8, r1
	bl 0x0200ddf4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #21
	bl 0x0200de2c
	movs r0, #20
	bl 0x0200dd2c
	movs r2, #20
	ldr r0, [pc, #980]
	movs r1, #0
	bl 0x0200de0c
	movs r0, #6
	movs r1, #2
	bl 0x0200dddc
	movs r2, #20
	movs r0, #6
	movs r1, #0
	bl 0x0200de0c
	movs r1, #6
	movs r0, #6
	bl 0x0200ddb4
	movs r0, #10
	bl 0x0200dd2c
	movs r0, #21
	movs r1, #2
	bl 0x0200ddd4
	movs r2, #40
	ldr r0, [pc, #932]
	movs r1, #0
	bl 0x0200de0c
	movs r0, #6
	movs r1, #7
	bl 0x0200ddb4
	movs r2, #20
	movs r1, #0
	movs r0, #6
	bl 0x0200de0c
	movs r0, #17
	bl 0x0200decc
	movs r0, #6
	movs r1, #2
	bl 0x0200dddc
	ldr r1, [pc, #896]
	movs r0, #6
	bl 0x0200dd64
	movs r0, #20
	bl 0x0200dd2c
	movs r1, #129
	movs r0, #21
	lsls r1, r1, #1
	bl 0x0200de2c
	movs r1, #3
	movs r0, #21
	bl 0x0200ddd4
	movs r0, #6
	bl 0x0200dd6c
	movs r0, #160
	bl 0x0200dd2c
	movs r1, #1
	movs r0, #21
	bl 0x0200dddc
	movs r0, #20
	bl 0x0200dd2c
	movs r0, #21
	bl 0x0200dd4c
	movs r3, #160
	lsls r3, r3, #7
	strh r3, [r0, #6]
	movs r1, #0
	movs r0, #21
	bl 0x0200ddb4
	movs r0, #80
	bl 0x0200dd2c
	movs r1, #4
	movs r0, #21
	bl 0x0200ddbc
	movs r0, #40
	bl 0x0200dd2c
	movs r2, #40
	movs r0, #21
	movs r1, #0
	bl 0x0200de0c
	movs r1, #5
	movs r0, #21
	bl 0x0200ddb4
	movs r0, #10
	bl 0x0200dd2c
	movs r0, #21
	bl 0x0200dd4c
	movs r1, #0
	bl 0x0200dcac
	movs r0, #21
	movs r1, #0
	bl 0x0200ddb4
	movs r0, #21
	movs r1, #6
	movs r2, #0
	bl 0x0200ddcc
	movs r1, #192
	movs r2, #192
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #21
	bl 0x0200dd5c
	movs r0, #21
	bl 0x0200dd4c
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r0, #21
	bl 0x0200dd4c
	movs r5, #128
	ldr r3, [pc, #708]
	adds r7, r0, #0
	lsls r5, r5, #8
	str r3, [r7, #108]
	movs r2, #237
	strh r5, [r7, #6]
	movs r0, #21
	movs r1, #184
	bl 0x0200dd8c
	ldr r1, [pc, #692]
	movs r0, #21
	bl 0x0200dd7c
	movs r0, #120
	bl 0x0200dd2c
	movs r0, #0
	bl 0x02008894
	movs r0, #72
	bl 0x0200decc
	movs r0, #128
	adds r1, r5, #0
	lsls r0, r0, #11
	bl 0x0200de34
	movs r0, #171
	movs r1, #128
	movs r2, #212
	movs r3, #1
	lsls r2, r2, #16
	lsls r1, r1, #14
	lsls r0, r0, #17
	bl 0x0200de3c
	bl 0x0200de44
	movs r0, #20
	bl 0x0200dd2c
	movs r1, #2
	movs r0, #0
	bl 0x0200dddc
	movs r0, #40
	bl 0x0200dd2c
	movs r0, #0
	bl 0x0200dd4c
	movs r1, #1
	bl 0x0200dcac
	movs r0, #0
	movs r1, #1
	bl 0x0200ddb4
	movs r2, #60
	movs r1, #6
	movs r0, #0
	bl 0x0200ddcc
	movs r0, #0
	bl 0x0200dd4c
	movs r3, #10
	adds r0, #100
	strh r3, [r0]
	ldr r1, [pc, #576]
	movs r0, #0
	bl 0x0200dd64
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #80
	bl 0x0200de24
	movs r2, #40
	movs r0, #1
	movs r1, #0
	bl 0x0200de0c
	movs r1, #1
	movs r0, #2
	bl 0x0200dddc
	movs r0, #20
	bl 0x0200dd2c
	movs r0, #2
	bl 0x02008894
	movs r1, #129
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200de24
	movs r0, #3
	bl 0x02008894
	movs r0, #1
	bl 0x0200dd4c
	movs r1, #1
	bl 0x0200dcac
	movs r0, #1
	movs r1, #1
	bl 0x0200ddb4
	movs r2, #60
	movs r0, #1
	movs r1, #6
	bl 0x0200ddcc
	movs r1, #1
	movs r0, #2
	bl 0x0200dddc
	movs r0, #20
	bl 0x0200dd2c
	movs r0, #2
	bl 0x0200dd4c
	movs r1, #1
	bl 0x0200dcac
	movs r0, #2
	movs r1, #1
	bl 0x0200ddb4
	movs r2, #40
	movs r1, #6
	movs r0, #2
	bl 0x0200ddcc
	movs r0, #3
	bl 0x0200dd4c
	movs r1, #1
	bl 0x0200dcac
	movs r0, #3
	movs r1, #1
	bl 0x0200ddb4
	movs r0, #3
	movs r1, #6
	movs r2, #60
	bl 0x0200ddcc
	movs r1, #192
	movs r2, #60
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200de14
	movs r1, #4
	movs r0, #1
	bl 0x0200ddb4
	movs r0, #1
	bl 0x02008894
	movs r1, #192
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #7
	bl 0x0200de14
	movs r5, #192
	movs r1, #4
	movs r0, #2
	bl 0x0200ddb4
	lsls r5, r5, #7
	movs r0, #2
	bl 0x02008894
	movs r0, #3
	adds r1, r5, #0
	bl 0x020088a8
	movs r6, #128
	movs r0, #3
	movs r1, #4
	bl 0x0200ddb4
	lsls r6, r6, #7
	movs r2, #80
	movs r0, #3
	movs r1, #0
	bl 0x0200de0c
	movs r0, #1
	adds r1, r6, #0
	bl 0x020088a8
	movs r0, #1
	movs r1, #0
	movs r2, #40
	bl 0x0200de0c
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200de14
	movs r2, #40
	movs r0, #2
	movs r1, #0
	bl 0x0200de0c
	movs r0, #1
	movs r1, #1
	bl 0x0200ddd4
	movs r1, #1
	movs r0, #2
	bl 0x0200dddc
	movs r0, #20
	bl 0x0200dd2c
	movs r0, #1
	adds r1, r5, #0
	movs r2, #0
	bl 0x0200de14
	movs r2, #20
	movs r0, #2
	adds r1, r5, #0
	bl 0x0200de14
	movs r0, #3
	movs r1, #1
	bl 0x0200dddc
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #20
	movs r0, #3
	bl 0x0200de14
	movs r0, #3
	bl 0x02008894
	movs r1, #128
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200de24
	adds r1, r6, #0
	movs r0, #1
	bl 0x020088a8
	movs r0, #1
	bl 0x02008894
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r0, #1
	ldr r1, [pc, #176]
	ldr r2, [pc, #176]
	bl 0x0200dd5c
	movs r0, #1
	ldr r1, [pc, #172]
	movs r2, #194
.L_02004f60:
	bl 0x0200dd9c
	movs r0, #1
	ldr r1, [pc, #164]
	movs r2, #206
	bl 0x0200dd9c
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200de14
	movs r0, #1
	movs r1, #1
	bl 0x0200dddc
	movs r1, #0
	movs r2, #80
	movs r0, #1
	bl 0x0200de0c
.L_02004f8a:
	movs r0, #0
	bl 0x0200dd74
	movs r0, #1
	bl 0x0200dbfc
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #24]
	str r3, [r7, #28]
	movs r0, #40
	bl 0x0200dd2c
	movs r1, #128
	movs r2, #80
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200de24
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #1
.L_02004fb6:
	bl 0x020088a8
	movs r2, #0
	movs r1, #4
	movs r0, #1
	bl 0x0200ddcc
	movs r0, #1
	bl 0x02008894
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #1
	bl 0x020088a8
	movs r1, #0
	movs r0, #1
	bl 0x0200ddfc
	movs r0, #0
	movs r1, #0
	bl 0x0200dd44
	cmp r0, #0
	bne .L_02004fb6_0
	b .L_02004fb6_1
	.2byte 0x0000
	.2byte 0x282e
	.2byte 0x0000
	.2byte 0x2015
	.2byte 0x0000
	.2byte 0xe324
	.2byte 0x0200
	.2byte 0x8401
	.2byte 0x0200
	.2byte 0xe360
	.2byte 0x0200
	.2byte 0xe074
	.2byte 0x0200
	.2byte 0x9999
	.2byte 0x0001
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x014d
	.2byte 0x0000
.L_02004fb6_1:
	movs r0, #20
	bl 0x0200dd2c
	movs r1, #3
	movs r0, #1
	bl 0x0200ddbc
	movs r0, #1
	bl 0x02008894
	movs r1, #1
	movs r0, #2
	bl 0x0200dddc
	movs r0, #2
	bl 0x02008894
	movs r0, #3
	movs r1, #4
	bl 0x0200ddb4
	movs r0, #3
	bl 0x02008894
	movs r2, #1
	mov r8, r2
	b .L_02004fb6_2
.L_02004fb6_0:
	ldr r3, [pc, #1012]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #3
	strh r3, [r2]
	movs r0, #60
	bl 0x0200dd2c
	movs r0, #1
	bl 0x02008894
	movs r1, #1
	movs r0, #2
	bl 0x0200dddc
	movs r0, #2
	bl 0x02008894
	movs r0, #3
	movs r1, #4
	bl 0x0200ddb4
	movs r0, #3
	bl 0x02008894
.L_02004fb6_2:
	mov r1, r8
	cmp r1, #0
	beq .L_02004fb6_3
	ldr r3, [pc, #948]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #3
	strh r3, [r2]
.L_02004fb6_3:
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #0
	bl 0x020088a8
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #0
	bl 0x0200de2c
	movs r0, #80
	bl 0x0200dd2c
	movs r0, #17
	bl 0x0200decc
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #20
	bl 0x0200de24
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200de14
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200de14
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200de14
	ldr r1, [pc, #856]
	movs r2, #40
	movs r0, #2
	bl 0x0200de24
	movs r0, #2
	bl 0x02008894
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200de14
	movs r1, #128
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #6
	bl 0x0200de14
	movs r1, #4
	movs r0, #1
	bl 0x0200ddb4
	movs r0, #1
	bl 0x02008894
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200de14
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200de14
	movs r1, #192
	movs r2, #40
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200de14
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200de2c
	movs r0, #60
	bl 0x0200dd2c
	movs r0, #3
	bl 0x02008894
	movs r0, #141
	bl 0x0200decc
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl 0x0200dcbc
	movs r0, #40
	bl 0x0200dd2c
	movs r0, #145
	bl 0x0200decc
	movs r1, #18
	movs r2, #23
	str r1, [sp, #0]
	str r2, [sp, #4]
	mov r8, r2
	mov r9, r1
	movs r0, #110
	movs r1, #105
	movs r2, #74
	movs r3, #4
	bl 0x0200dc94
	mov r3, r8
	str r3, [sp, #4]
	movs r5, #8
	movs r0, #92
	movs r1, #86
	movs r2, #83
	movs r3, #4
	str r5, [sp, #0]
	bl 0x0200dc94
	mov r1, r8
	str r1, [sp, #4]
	movs r0, #75
	movs r1, #28
	movs r2, #75
	movs r3, #4
	str r5, [sp, #0]
	bl 0x0200dc94
	movs r2, #20
	movs r3, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	mov r11, r2
	movs r0, #92
	movs r1, #86
	movs r2, #11
	movs r3, #72
	bl 0x0200dc94
	movs r3, #21
	str r3, [sp, #4]
	movs r1, #92
	movs r2, #19
	movs r3, #68
	movs r0, #19
	str r5, [sp, #0]
	bl 0x0200dc94
	movs r0, #0
	bl 0x0200dd4c
	adds r7, r0, #0
	ldr r3, [r7, #16]
	ldr r6, [pc, #592]
	movs r1, #0
	adds r3, r3, r6
	mov r10, r1
	str r3, [r7, #16]
	adds r3, r7, #0
	mov r2, r10
	adds r3, #102
	strh r2, [r3]
	bl 0x0200d688
	movs r0, #1
	bl 0x0200dd4c
	adds r7, r0, #0
	ldr r3, [r7, #8]
	ldr r5, [pc, #564]
	adds r3, r3, r5
	str r3, [r7, #8]
	ldr r3, [r7, #16]
	adds r3, r3, r6
	str r3, [r7, #16]
	adds r3, r7, #0
	mov r1, r10
	adds r3, #102
	strh r1, [r3]
	bl 0x0200d688
	movs r0, #2
	bl 0x0200dd4c
	adds r7, r0, #0
	ldr r3, [r7, #8]
	adds r3, r3, r5
	str r3, [r7, #8]
	ldr r3, [r7, #16]
	adds r3, r3, r6
	str r3, [r7, #16]
	adds r3, r7, #0
	mov r2, r10
	adds r3, #102
	strh r2, [r3]
	bl 0x0200d688
	movs r0, #3
	bl 0x0200dd4c
	adds r7, r0, #0
	ldr r3, [r7, #8]
	adds r3, r3, r5
	str r3, [r7, #8]
	ldr r1, [pc, #496]
	ldr r3, [r7, #16]
	adds r3, r3, r1
	str r3, [r7, #16]
	adds r3, r7, #0
	mov r2, r10
	adds r3, #102
	strh r2, [r3]
	bl 0x0200d688
	movs r0, #23
	bl 0x0200dd4c
	movs r3, #224
	lsls r3, r3, #14
	str r3, [r0, #12]
	movs r1, #128
	movs r0, #169
	movs r2, #180
	movs r3, #0
	lsls r1, r1, #14
	lsls r2, r2, #16
	lsls r0, r0, #17
	bl 0x0200de3c
	bl 0x0200dc7c
	movs r0, #1
	bl 0x0200dbfc
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200de24
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200de24
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200de24
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200de24
	movs r0, #0
	movs r1, #6
	movs r2, #0
	bl 0x0200ddcc
	movs r0, #1
	movs r1, #6
	movs r2, #0
	bl 0x0200ddcc
	movs r0, #2
	movs r1, #6
	movs r2, #0
	bl 0x0200ddcc
	movs r2, #0
	movs r0, #3
	movs r1, #6
	bl 0x0200ddcc
	ldr r5, [pc, #352]
	movs r0, #0
	adds r1, r5, #0
	bl 0x0200dd64
	adds r1, r5, #0
	movs r0, #1
	bl 0x0200dd64
	adds r1, r5, #0
	movs r0, #2
	bl 0x0200dd64
	adds r1, r5, #0
	movs r0, #3
	bl 0x0200dd64
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200dcbc
	movs r0, #80
	bl 0x0200dd2c
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl 0x0200dcbc
	movs r0, #40
	bl 0x0200dd2c
	movs r0, #145
	bl 0x0200decc
	movs r3, #10
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #110
	movs r1, #106
	movs r2, #18
	movs r3, #14
	bl 0x0200dc9c
	mov r3, r9
	mov r1, r8
	str r3, [sp, #0]
	str r1, [sp, #4]
	movs r0, #110
	movs r1, #105
	movs r2, #74
	movs r3, #4
	bl 0x0200dc94
	mov r3, r11
	movs r2, #16
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #11
	movs r1, #86
	movs r3, #68
	movs r0, #92
	bl 0x0200dc94
	movs r0, #0
	bl 0x0200dd4c
	adds r7, r0, #0
	ldr r5, [pc, #204]
	ldr r3, [r7, #8]
	adds r3, r3, r5
	str r3, [r7, #8]
	bl 0x0200d688
	movs r0, #1
	bl 0x0200dd4c
	adds r7, r0, #0
	ldr r3, [r7, #8]
	adds r3, r3, r5
	str r3, [r7, #8]
	bl 0x0200d688
	movs r0, #2
	bl 0x0200dd4c
	adds r7, r0, #0
	ldr r3, [r7, #8]
	adds r3, r3, r5
	str r3, [r7, #8]
	bl 0x0200d688
	movs r0, #3
	bl 0x0200dd4c
	adds r7, r0, #0
	ldr r3, [r7, #8]
	adds r3, r3, r5
	str r3, [r7, #8]
	bl 0x0200d688
	movs r0, #8
.L_020053c8:
	bl 0x0200dd4c
	adds r7, r0, #0
	ldr r3, [r7, #8]
	movs r5, #128
	lsls r5, r5, #13
	adds r3, r3, r5
	str r3, [r7, #8]
	bl 0x0200d688
	movs r0, #9
	bl 0x0200dd4c
	adds r7, r0, #0
	ldr r3, [r7, #8]
	adds r3, r3, r5
	str r3, [r7, #8]
	bl 0x0200d688
	movs r0, #10
	bl 0x0200dd4c
	adds r7, r0, #0
	ldr r1, [pc, #92]
	ldr r3, [r7, #8]
	adds r3, r3, r1
	str r3, [r7, #8]
	bl 0x0200d688
	movs r0, #11
	bl 0x0200dd4c
	adds r7, r0, #0
	ldr r3, [r7, #8]
	ldr r2, [pc, #68]
	adds r3, r3, r2
	str r3, [r7, #8]
	bl 0x0200d688
	movs r0, #161
	movs r1, #128
	movs r2, #180
	movs r3, #0
	lsls r2, r2, #16
	lsls r1, r1, #14
	lsls r0, r0, #17
	bl 0x0200de3c
	bl 0x0200dc7c
.L_0200542c:
	movs r0, #1
	bl 0x0200dbfc
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	b .L_0200542c_0
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x0101
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffe0
	.2byte 0x0000
	.2byte 0xfffc
	.2byte 0x0000
	.2byte 0xffee
	.2byte 0xe3c0
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0xfff0
.L_0200542c_0:
	bl 0x0200de2c
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200de2c
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	bl 0x0200de2c
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	bl 0x0200de2c
	movs r0, #0
	movs r1, #6
	movs r2, #0
	bl 0x0200ddcc
	movs r0, #1
	movs r1, #6
	movs r2, #0
	bl 0x0200ddcc
	movs r0, #2
	movs r1, #6
	movs r2, #0
	bl 0x0200ddcc
	movs r0, #3
	movs r1, #6
	movs r2, #0
	bl 0x0200ddcc
	movs r0, #1
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #444]
	negs r0, r0
	bl 0x0200dcbc
	movs r0, #80
	bl 0x0200dd2c
	ldr r0, [pc, #436]
	bl 0x0200decc
	movs r0, #40
	bl 0x0200dd2c
	movs r0, #0
	bl 0x0200dd74
	movs r0, #1
	bl 0x0200dd74
	movs r0, #2
	bl 0x0200dd74
	movs r0, #3
	bl 0x0200dd74
	movs r0, #120
	bl 0x0200dd2c
	movs r2, #120
	movs r0, #2
	ldr r1, [pc, #392]
	bl 0x0200de24
	movs r0, #2
	movs r1, #1
	bl 0x0200dddc
	movs r1, #160
	movs r2, #20
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200de14
	movs r0, #2
	bl 0x02008894
	movs r0, #1
	movs r1, #3
	bl 0x0200ddbc
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200de14
	movs r0, #1
	bl 0x02008894
	movs r1, #224
	movs r2, #20
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200de14
	movs r1, #4
	movs r0, #3
	bl 0x0200ddb4
	movs r0, #3
	bl 0x02008894
	movs r2, #20
	movs r1, #2
	movs r0, #2
	bl 0x0200ddcc
	movs r0, #2
	bl 0x02008894
	movs r0, #1
	movs r1, #3
	bl 0x0200ddb4
	movs r0, #2
	movs r1, #3
	bl 0x0200ddb4
	movs r1, #3
	movs r0, #3
	bl 0x0200ddbc
	movs r0, #20
	bl 0x0200dd2c
	movs r1, #0
	movs r0, #1
	bl 0x020088a8
	movs r0, #1
	bl 0x02008894
	movs r1, #128
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200de14
	movs r1, #3
	movs r0, #0
	bl 0x0200ddbc
	movs r0, #20
	bl 0x0200dd2c
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200dd5c
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200dd5c
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #3
	lsls r1, r1, #9
	bl 0x0200dd5c
	ldr r5, [pc, #184]
	movs r0, #1
	adds r1, r5, #0
	bl 0x0200dd64
	adds r1, r5, #0
	movs r0, #2
	bl 0x0200dd64
	adds r1, r5, #0
	movs r0, #3
	bl 0x0200dd64
	movs r0, #60
	bl 0x0200dd2c
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200dd5c
	movs r1, #136
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #216
	bl 0x0200dd9c
	movs r1, #136
	lsls r1, r1, #1
	movs r2, #254
	movs r0, #0
	bl 0x0200dd94
	movs r0, #80
	bl 0x0200dd2c
	ldr r5, [pc, #112]
	movs r3, #224
	ldr r1, [r5]
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #65
	str r3, [r2]
	movs r2, #228
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r1, #16
	str r1, [r3]
	bl 0x0200de9c
	subs r5, #48
	bl 0x0200dea4
	movs r0, #80
	bl 0x0200dd2c
	bl 0x0200dcc4
	ldr r3, [r5]
	ldr r1, [pc, #72]
	adds r2, r3, r1
	mov r1, r10
	strh r1, [r2]
	ldr r2, [pc, #68]
	adds r3, r3, r2
	strh r1, [r3]
	movs r2, #0
	movs r1, #0
	ldr r0, [pc, #60]
	bl 0x0200dce4
	bl 0x0200dccc
	movs r0, #80
	bl 0x0200dd2c
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000e666
	.4byte 0x00000121
	.4byte 0x00000105
	.4byte 0x0200e39c
	.4byte 0x03001ebc
	.4byte 0x000012f4
	.4byte 0x000012f6
	.4byte 0x0000284f
	.global Func_02005688
	.thumb_func
Func_02005688:
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r0, #56]
	str r3, [r0, #60]
	str r3, [r0, #64]
	movs r3, #0
	str r3, [r0, #36]
	str r3, [r0, #40]
	str r3, [r0, #44]
	adds r0, #100
	strh r3, [r0]
	bx lr
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #23
	bl 0x0200dd4c
	ldr	r5, [pc, #512]
	ldr	r3, [r5, #0]
	adds	r6, r0, #0
	movs	r7, #0
	cmp	r3, #48
	bls.n	.L_020056bc
	b.n	.L_020058dc
.L_020056bc:
	ldr	r2, [pc, #500]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0200d788
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d7a2
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d7b4
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d7c6
	.4byte 0x0200d7ee
	.4byte 0x0200d84e
	.4byte 0x0200d896
	.4byte 0x0200d896
	.4byte 0x0200d896
	.4byte 0x0200d896
	.4byte 0x0200d896
	.4byte 0x0200d896
	.4byte 0x0200d896
	.4byte 0x0200d896
	.4byte 0x0200d8dc
	.4byte 0x0200d89a
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8dc
	.4byte 0x0200d8d0
	.4byte 0xf00020dc
	.4byte 0x20c0fb9f
	.4byte 0x228021c0
	.4byte 0x02c902c0
	.4byte 0xf0000252
	.4byte 0x4846fa8f
	.4byte 0x2080e001
	.4byte 0x21010240
	.4byte 0xfb68f000
	.4byte 0xf0002008
	.4byte 0xe093fb6d
	.4byte 0x21802080
	.4byte 0x02402280
	.4byte 0x02520249
	.4byte 0xfa7cf000
	.4byte 0x2398e08a
	.4byte 0x60b3045b
	.4byte 0x60f34b3b
	.4byte 0x041b23a4
	.4byte 0x23806133
	.4byte 0x1c30025b
	.4byte 0x61f361b3
	.4byte 0xff52f7ff
	.4byte 0x20174936
	.4byte 0xfabcf000
	.4byte 0x682be076
	.4byte 0x602b3b01
	.4byte 0x290068f1
	.4byte 0x2100dd3d
	.4byte 0xf0004831
	.4byte 0x2010fb3d
	.4byte 0xfb42f000
	.4byte 0x3301682b
	.4byte 0x2000602b
	.4byte 0xfa9cf000
	.4byte 0x30622501
	.4byte 0x20017005
	.4byte 0xfa96f000
	.4byte 0x70053062
	.4byte 0xf0002002
	.4byte 0x3062fa91
	.4byte 0x20037005
	.4byte 0xfa8cf000
	.4byte 0x70053062
	.4byte 0xf0002015
	.4byte 0x3062fa87
	.4byte 0x20067005
	.4byte 0xfa82f000
	.4byte 0x70053062
	.4byte 0x682be046
	.4byte 0x602b3b01
	.4byte 0x68f123a0
	.4byte 0x4299039b
	.4byte 0x2080dd0b
	.4byte 0x21000240
	.4byte 0xfb0af000
	.4byte 0xf0002028
	.4byte 0x682bfb0f
	.4byte 0x602b3301
	.4byte 0x4b14e032
	.4byte 0x2207681b
	.4byte 0x2b004013
	.4byte 0x20f6d103
	.4byte 0xfb22f000
	.4byte 0x229068f1
	.4byte 0x188b0292
	.4byte 0x60f32701
	.4byte 0x2701e022
	.4byte 0x20bbe020
	.4byte 0xfb16f000
	.4byte 0x2100480a
	.4byte 0xfaeaf000
	.4byte 0xf000200c
	.4byte 0xe015faef
	.4byte 0x0200e764
	.4byte 0x0200d6c4
	.4byte 0x002063ff
	.4byte 0xfe980000
	.4byte 0x0200e2d0
	.4byte 0x00203210
	.4byte 0x03001e40
	.4byte 0x00007fff
	.4byte 0xf0002017
	.4byte 0x4827fa4f
	.2byte 0xf000
	.2byte 0xfa1c
.L_020058dc:
	cmp	r7, #0
	beq.n	.L_0200599c
	bl 0x0200dc14
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #4
	ldr	r2, [r6, #12]
	lsrs	r3, r3, #16
	lsls	r3, r3, #16
	subs	r2, r2, r3
	ldr	r3, [pc, #132]
.L_020058f4:
	movs	r0, #142
	adds	r2, r2, r3
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	lsls	r0, r0, #1
	bl 0x0200dc6c
.L_02005902:
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_0200599c
	ldr	r1, [r7, #80]
	mov	sl, r1
	ldr	r1, [pc, #108]
	bl 0x0200dc64
	movs	r1, #1
	adds	r0, r7, #0
	bl 0x0200ddec
	adds	r3, r7, #0
	adds	r3, #85
	movs	r5, #0
	strb	r5, [r3, #0]
	bl 0x0200dc14
	ldr	r3, [pc, #88]
	adds	r2, r7, #0
	adds	r2, #100
	ands	r3, r0
	strh	r3, [r2, #0]
	adds	r3, r7, #0
	ldr	r1, [pc, #60]
	adds	r3, #102
	strh	r5, [r3, #0]
	mov	r8, r1
	bl 0x0200dc14
	adds	r3, r7, #0
	lsrs	r0, r0, #13
	adds	r3, #98
.L_02005944:
	strb	r0, [r3, #0]
	ldr	r3, [pc, #60]
	str	r3, [r7, #108]
	bl 0x0200dc14
	adds	r3, r0, #0
	lsls	r0, r3, #16
	subs	r0, r0, r3
	lsrs	r0, r0, #20
	bl 0x0200dc1c
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	str	r3, [r7, #48]
	movs	r2, #50
	ldrsh	r3, [r6, r2]
	str	r3, [r7, #48]
	mov	r3, sl
	adds	r3, #38
	mov	r1, r8
	b.n	.L_02005988
	.4byte 0x00000000
	.4byte 0x00000237
	.4byte 0xfff80000
	.4byte 0x0200e1cc
	.4byte 0x0ffff000
	.2byte 0xb661
	.2byte 0x0200
.L_02005988:
	strb	r1, [r3, #0]
	mov	r3, sl
	ldrb	r2, [r3, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	mov	r1, sl
	strb	r3, [r1, #9]
.L_0200599c:
	ldr	r2, [pc, #76]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	movs	r0, #0
	bl 0x0200dd4c
	bl 0x0200d9f0
	movs	r0, #1
	bl 0x0200dd4c
	bl 0x0200d9f0
	movs	r0, #2
	bl 0x0200dd4c
	bl 0x0200d9f0
	movs	r0, #3
	bl 0x0200dd4c
	bl 0x0200d9f0
	movs	r0, #21
	bl 0x0200dd4c
	bl 0x0200d9f0
	movs	r0, #6
	bl 0x0200dd4c
	bl 0x0200d9f0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0xe764
	.2byte 0x0200
	.global Func_020059f0
	.thumb_func
Func_020059f0:
	push {r5, lr}
.L_020059f2:
	adds r3, r0, #0
	adds r3, #99
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_020059f2_0
	adds r5, r0, #0
	adds r5, #98
	ldrb r2, [r5]
	ldr r3, [r0, #76]
	lsrs r2, r2, #2
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r0, #12]
	bl 0x0200d688
	ldrb r3, [r5]
	adds r2, r3, #0
	cmp r2, #0
	beq .L_020059f2_0
	cmp r2, #31
	bhi .L_020059f2_0
	adds r3, #1
	strb r3, [r5]
.L_020059f2_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r0, #23
	bl 0x0200dd4c
	ldr	r3, [pc, #100]
	mov	sl, r0
	ldr	r5, [r3, #0]
	bl 0x0200dc14
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #4
	lsrs	r3, r3, #16
	lsls	r3, r3, #16
	adds	r5, #232
	mov	r8, r3
	movs	r0, #2
	ldrsh	r3, [r5, r0]
	cmp	r3, #129
	bgt.n	.L_02005aac
	ldr	r3, [pc, #72]
	ldr	r3, [r3, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02005a7e
	movs	r1, #152
	movs	r2, #164
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #23
	bl 0x0200ddac
	movs	r0, #23
	bl 0x0200dd4c
	movs	r5, #128
	lsls	r5, r5, #9
	b.n	.L_02005a94
.L_02005a7e:
	movs	r1, #152
	movs	r2, #171
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r0, #23
	bl 0x0200ddac
	movs	r0, #23
	bl 0x0200dd4c
	ldr	r5, [pc, #20]
.L_02005a94:
	str	r5, [r0, #24]
	movs	r0, #23
	bl 0x0200dd4c
	str	r5, [r0, #28]
	b.n	.L_02005ab6
	.4byte 0x03001e70
	.4byte 0x03001e40
	.2byte 0x4ccc
	.2byte 0x0001
.L_02005aac:
	movs	r0, #23
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ddac
.L_02005ab6:
	mov	r1, sl
	cmp	r1, #0
	beq.n	.L_02005b80
	ldr	r3, [pc, #160]
	ldr	r6, [r3, #0]
	movs	r3, #15
	ands	r6, r3
	cmp	r6, #0
	bne.n	.L_02005b80
	mov	r0, sl
	ldr	r2, [r0, #12]
	ldr	r1, [r1, #8]
	movs	r3, #128
	lsls	r3, r3, #12
	add	r2, r8
	adds	r1, r1, r3
	adds	r2, r2, r3
	ldr	r3, [r0, #16]
	movs	r0, #142
	lsls	r0, r0, #1
	bl 0x0200dc6c
	movs	r1, #192
	lsls	r1, r1, #11
	adds	r7, r0, #0
	mov	r0, r8
	bl 0x0200dbec
	mov	r8, r0
	mov	r1, r8
	lsls	r1, r1, #16
	mov	r8, r1
	cmp	r7, #0
	beq.n	.L_02005b80
	ldr	r1, [pc, #104]
	adds	r0, r7, #0
	ldr	r5, [r7, #80]
	bl 0x0200dc64
	movs	r1, #5
	adds	r0, r7, #0
	bl 0x0200ddec
	adds	r3, r7, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	bl 0x0200dc14
	ldr	r3, [pc, #80]
	adds	r2, r7, #0
	ands	r3, r0
	adds	r2, #100
	ldr	r0, [pc, #60]
	strh	r3, [r2, #0]
	adds	r3, r7, #0
	mov	r9, r0
	adds	r3, #102
	ldr	r0, [pc, #64]
	strh	r6, [r3, #0]
	mov	r2, r8
	ldr	r3, [pc, #64]
	mov	r1, sl
	ands	r0, r2
	str	r1, [r7, #104]
	str	r3, [r7, #108]
	asrs	r0, r0, #4
	bl 0x0200dc1c
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	asrs	r3, r3, #16
	str	r3, [r7, #48]
	adds	r3, r5, #0
	adds	r3, #38
	mov	r0, r9
	strb	r0, [r3, #0]
	mov	r1, sl
	ldr	r3, [r1, #80]
	ldrb	r3, [r3, #9]
	movs	r2, #12
	ands	r2, r3
	b.n	.L_02005b74
	.4byte 0x00000000
	.4byte 0x03001e40
	.4byte 0x0200e734
	.4byte 0x0ffff000
	.4byte 0x000fffff
	.2byte 0xdb91
	.2byte 0x0200
.L_02005b74:
	ldrb	r1, [r5, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r5, #9]
.L_02005b80:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.global Func_02005b90
	.thumb_func
Func_02005b90:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #100
	ldrh r2, [r6]
	ldr r1, [r5, #104]
	mov r8, r2
	mov r0, r8
	mov r10, r1
	bl 0x0200dc24
	ldr r3, [r5, #48]
	adds r3, #28
	adds r2, r3, #0
	muls r2, r0
	mov r1, r10
	ldr r3, [r1, #8]
	mov r0, r8
	adds r3, r3, r2
	str r3, [r5, #8]
	bl 0x0200dc1c
	movs r2, #164
	ldr r3, [r5, #8]
	lsls r2, r2, #16
	lsls r0, r0, #4
	adds r0, r0, r2
	str r0, [r5, #16]
	str r3, [r5, #56]
	str r0, [r5, #64]
	ldr r1, [pc, #20]
	ldrh r3, [r6]
	adds r3, r3, r1
	strh r3, [r6]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xfffffe00
	.include "games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/IMPORT.INC"
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
	.4byte 0x0200df10
	.4byte 0x0200df48
	.4byte 0x0200df80
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01ec0000
	.4byte 0x00000000
	.4byte 0x00a40000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00003333
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00003333
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x02008315
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffc80
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000008c
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffc80
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x000000a0
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00020000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x000000a0
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000010
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000010
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x010c0000
	.4byte 0x00000000
	.4byte 0x00ba0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x010c0000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00de0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffa00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000004
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000600
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000004
	.4byte 0xc0020000
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffff4000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000078
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xfffb8000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000050
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x014a0000
	.4byte 0x00000000
	.4byte 0x00ad0000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x02008425
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00000130
	.4byte 0x300000d8
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x000001a4
	.4byte 0xffff0001
	.4byte 0x000001f8
	.4byte 0x300000a8
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x000001a4
	.4byte 0xffff0002
	.4byte 0x00000154
	.4byte 0x800000b8
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x000001a4
	.4byte 0xffff0003
	.4byte 0x00000154
	.4byte 0x800000b8
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x000001a4
	.4byte 0xffff0009
	.4byte 0x00000000
	.4byte 0x80000000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x000001a4
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000bb
	.4byte 0x00149002
	.4byte 0x002630b4
	.4byte 0x0090c0b3
	.4byte 0x000001ff
	.4byte 0xffff0102
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff0102
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0xffff0102
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0xffff0102
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00940000
	.4byte 0x00024000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00940000
	.4byte 0x00024000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00940000
	.4byte 0x00024000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00a40000
	.4byte 0x0002d000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00b40000
	.4byte 0x0002b000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x00020000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x0002d000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00f4
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000202
	.4byte 0xffff000b
	.4byte 0x02008519
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x020086c1
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x02008519
	.4byte 0x00000002
	.4byte 0x0250000a
	.4byte 0x020085ed
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x020092c9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016666
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016666
	.4byte 0x0000001c
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000001b
