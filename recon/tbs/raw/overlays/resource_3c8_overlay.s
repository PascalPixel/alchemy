.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
	movs r3, #8
	adds r6, r1, #0
	strb r3, [r2]
	movs r1, #0
	bl 0x0200ce44
	adds r0, r5, #0
	adds r1, r6, #0
	bl 0x0200cf3c
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02000058
	.thumb_func
Func_02000058:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl 0x0200cde4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000058_0
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	strb r3, [r1, #9]
	movs r1, #14
	bl 0x02008030
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200ce4c
	adds r0, r5, #0
	b .L_02000058_1
.L_02000058_0:
	movs r0, #0
.L_02000058_1:
	pop {r5, r6}
	pop {r1}
	bx r1
	.global Func_02000098
	.thumb_func
Func_02000098:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl 0x0200cde4
	adds r5, r0, #0
	cmp r5, #0
	beq 0x020080d8
.L_020000b2:
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	movs r1, #15
	bl 0x02008030
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	b .L_020000b2_0
	.2byte 0x2000
.L_020000b2_0:
	pop {r5, r6}
	pop {r1}
	bx r1
	.global Func_020000e0
	.thumb_func
Func_020000e0:
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
	.global Func_02000118
	.thumb_func
Func_02000118:
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
	bl 0x0200ceac
	movs r3, #128
	lsls r3, r3, #13
	mov r2, r10
	ands r3, r2
	mov r9, r0
	cmp r3, #0
	beq .L_02000118_0
	cmp r7, #0
	beq .L_02000118_0
	movs r3, #24
	ldrsh r0, [r7, r3]
	adds r2, r6, #0
	b .L_02000118_1
.L_02000118_0:
	adds r2, r6, #0
	movs r0, #222
.L_02000118_1:
	adds r1, r5, #0
	mov r3, r8
	bl 0x0200cde4
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02000118_2
	b .L_02000118_3
.L_02000118_2:
	ldr r1, [r6, #80]
	mov r8, r1
	mov r1, r10
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	bl 0x0200cdd4
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl 0x0200cddc
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
	beq .L_02000118_3
	cmp r7, #0
	beq .L_02000118_3
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_02000118_4
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl 0x0200cf3c
.L_02000118_4:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02000118_5
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
.L_02000118_5:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r10
	ands r2, r3
	cmp r2, #0
	beq .L_02000118_6
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02000118_6:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_02000118_7
	ldr r3, [pc, #156]
	mov r1, r11
	ldr r5, [r3, r1]
	cmp r2, #0
	beq .L_02000118_8
	ldr r0, [r7, #16]
	ldr r3, [r6, #24]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	bl 0x0200cd9c
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_02000118_9
.L_02000118_8:
	ldr r0, [r7, #16]
	ldr r2, [pc, #128]
	ldr r1, [r5, #12]
	adds r0, r0, r2
	bl 0x0200cd9c
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_02000118_9:
	bl 0x0200cd9c
	str r0, [r6, #52]
.L_02000118_7:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_02000118_10
	adds r0, r6, #0
	movs r1, #1
	bl 0x0200cdd4
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl 0x0200cddc
.L_02000118_10:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02000118_11
	ldrh r3, [r7, #32]
	mov r1, r8
	strh r3, [r1, #30]
.L_02000118_11:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02000118_12
	ldrh r3, [r7, #34]
	ldr r1, [sp, #0]
	strh r3, [r1]
.L_02000118_12:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02000118_3
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_02000118_3:
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200d1d4
	.4byte 0x020080e1
	.4byte 0xffff0000
	.global Func_020002f0
	.thumb_func
Func_020002f0:
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
	bl 0x0200d010
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x030001d8
	.global Func_0200032c
	.thumb_func
Func_0200032c:
	push {r5, r6, lr}
	ldr r3, [pc, #64]
	adds r4, r0, #0
	ldr r2, [r3]
	ldr r3, [r4]
	adds r1, r2, #0
	movs r5, #8
	asrs r6, r3, #20
	adds r1, #52
.L_0200032c_2:
	ldmia r1!, {r0}
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r6, r3
	bne .L_0200032c_0
	ldr r2, [r4, #4]
	ldr r3, [r0, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_0200032c_0
	ldr r2, [r4, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_0200032c_1
.L_0200032c_0:
	adds r5, #1
	cmp r5, #65
	bls .L_0200032c_2
	movs r0, #0
.L_0200032c_1:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.global Func_02000374
	.thumb_func
Func_02000374:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #0
	sub sp, #12
	bl 0x0200ceac
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
	bl 0x0200832c
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02000374_0
	b .L_02000374_1
.L_02000374_0:
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
	bl 0x0200832c
	cmp r0, #0
	beq .L_02000374_2
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_02000374_1
.L_02000374_2:
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
	bl 0x0200832c
	cmp r0, #0
	beq .L_02000374_3
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_02000374_1
.L_02000374_3:
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
	bl 0x0200ce3c
	cmp r0, #0
	bgt .L_02000374_1
	adds r3, r6, #0
	adds r3, #98
	ldrb r3, [r3]
	mov r10, r3
	cmp r3, #0
	bne .L_02000374_1
	movs r1, #8
	mov r0, r8
	bl 0x0200cdd4
	ldr r5, [pc, #132]
	movs r0, #15
	bl 0x0200cda4
	movs r0, #185
	bl 0x0200cffc
	str r5, [r6, #48]
	str r5, [r6, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl 0x0200ce04
	mov r0, r8
	str r5, [r0, #48]
	str r5, [r0, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl 0x0200ce04
	adds r0, r6, #0
	bl 0x0200ce0c
	bl 0x0200cff4
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
	bl 0x0200cdd4
.L_02000374_1:
	sub sp, #-12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200d0e8
	.4byte 0xffff0000
	.4byte 0x00003333
	.global Func_020004f4
	.thumb_func
Func_020004f4:
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
	beq .L_020004f4_0
	cmp r0, #2
	bhi .L_020004f4_1
	lsls r3, r0, #1
	adds r3, r3, r0
	movs r0, #152
	lsls r0, r0, #1
	lsls r3, r3, #4
	adds r3, r3, r0
	ldr r0, [r2, r3]
	b .L_020004f4_2
.L_020004f4_1:
	ldr r0, [pc, #52]
.L_020004f4_2:
	lsls r3, r1, #7
	adds r3, r6, r3
	lsls r3, r3, #2
	movs r1, #0
	adds r0, r0, r3
	cmp r1, r12
	bcs .L_020004f4_0
.L_020004f4_5:
	lsls r3, r1, #9
	movs r2, #0
	adds r3, r0, r3
	cmp r2, r4
	bcs .L_020004f4_3
.L_020004f4_4:
	adds r2, #1
	strb r5, [r3, #2]
	adds r3, #4
	cmp r2, r4
	bcc .L_020004f4_4
.L_020004f4_3:
	adds r1, #1
	cmp r1, r12
	bcc .L_020004f4_5
.L_020004f4_0:
	movs r0, #0
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x03001e70
	.4byte 0x02010000
	.global Func_02000558
	.thumb_func
Func_02000558:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #48]
	ldr r3, [r3]
	ldr r7, [pc, #48]
	adds r5, r3, #0
	movs r6, #8
	adds r5, #52
.L_02000558_1:
	ldmia r5!, {r0}
	adds r3, r0, #0
	adds r3, #100
	ldrh r2, [r3]
	lsls r3, r2, #16
	asrs r3, r3, #20
	cmp r3, r7
	bne .L_02000558_0
	movs r1, #15
	ands r1, r2
	bl 0x0200cf3c
.L_02000558_0:
	adds r6, #1
	cmp r6, #65
	bls .L_02000558_1
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000212
	.global Func_02000594
	.thumb_func
Func_02000594:
	push {lr}
	adds r3, r0, #0
	adds r3, #100
	ldrh r3, [r3]
	movs r1, #15
	ands r1, r3
	bl 0x0200cf3c
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_020005ac
	.thumb_func
Func_020005ac:
	push {lr}
	ldr r3, [pc, #48]
	ldr r3, [r3]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	bne .L_020005ac_0
	movs r1, #7
	bl 0x0200cf3c
	b .L_020005ac_1
.L_020005ac_0:
	movs r1, #0
	bl 0x0200cf3c
.L_020005ac_1:
	ldr r3, [pc, #20]
	ldr r3, [r3]
	movs r2, #7
	ands r3, r2
	cmp r3, #0
	bne .L_020005ac_2
	movs r0, #138
	bl 0x0200cffc
.L_020005ac_2:
	movs r0, #0
	pop {r1}
	bx r1
	.4byte 0x03001e40
	.global Func_020005e4
	.thumb_func
Func_020005e4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, [pc, #152]
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	sub sp, #56
	mov r10, r0
	cmp r7, #0
	bne .L_020005e4_0
	bl 0x0200cdb4
	lsls r0, r0, #1
	lsrs r0, r0, #16
	movs r2, #16
	movs r3, #3
	add r2, sp
	subs r3, r3, r0
	str r3, [r2]
	ldr r3, [pc, #124]
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #14
	str r3, [r2, #4]
	mov r8, r2
	bl 0x0200cdb4
	lsls r3, r0, #3
	adds r3, r3, r0
	mov r2, r10
	lsrs r3, r3, #16
	ldr r6, [r2, #8]
	subs r3, #4
	lsls r3, r3, #16
	adds r6, r6, r3
	bl 0x0200cdb4
	lsls r0, r0, #5
	mov r2, r10
	lsrs r0, r0, #16
	movs r3, #32
	ldr r5, [r2, #12]
	subs r3, r3, r0
	lsls r3, r3, #16
	adds r5, r5, r3
	bl 0x0200cdb4
	adds r3, r0, #0
	lsls r0, r3, #2
	adds r0, r0, r3
	lsrs r0, r0, #16
	movs r3, #160
	lsls r3, r3, #11
	lsls r0, r0, #16
	adds r0, r0, r3
	movs r1, #10
	bl 0x0200cd9c
	mov r3, r10
	ldr r2, [r3, #16]
	movs r3, #176
	lsls r3, r3, #12
	str r3, [sp, #8]
	mov r3, r8
	str r0, [sp, #0]
	str r3, [sp, #12]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r3, #0
	str r7, [sp, #4]
	bl 0x02008118
.L_020005e4_0:
	movs r0, #0
	sub sp, #-56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x03001e40
	.4byte 0x00006666
	.global Func_02000690
	.thumb_func
Func_02000690:
	push {lr}
	movs r1, #0
	bl 0x0200ce44
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_020006a0
	.thumb_func
Func_020006a0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #56
	ldr r3, [pc, #160]
	add r7, sp, #16
	str r3, [r7, #8]
	str r3, [r7, #12]
	movs r3, #0
	str r3, [r7]
	adds r5, r0, #0
	mov r8, r3
	bl 0x0200cdb4
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r4, r0, #1
	adds r4, r4, r0
	ldr r6, [pc, #136]
	lsls r3, r4, #4
	adds r4, r4, r3
	ldr r2, [r6]
	lsls r3, r4, #8
	adds r4, r4, r3
	movs r3, #15
	ands r2, r3
	mov r10, r3
	movs r3, #8
	subs r3, r3, r2
	ldr r0, [r5, #8]
	lsls r3, r3, #16
	adds r0, r0, r3
	ldr r1, [r5, #12]
	movs r3, #208
.L_020006e6:
	lsls r3, r3, #13
	adds r1, r1, r3
	mov r3, r8
	str r3, [sp, #4]
	movs r3, #160
	lsls r3, r3, #12
	negs r4, r4
	str r3, [sp, #8]
	ldr r2, [r5, #16]
	mov r8, r3
	movs r3, #0
	str r4, [sp, #0]
	str r7, [sp, #12]
	bl 0x02008118
	ldr r6, [r6]
	mov r3, r10
	ands r6, r3
	cmp r6, #0
	bne 0x0200873c
	movs r3, #128
	lsls r3, r3, #8
.L_02000712:
	str r3, [r7, #8]
	str r3, [r7, #12]
	bl 0x0200cdb4
	lsls r3, r0, #3
	adds r3, r3, r0
	lsrs r3, r3, #16
	subs r3, #4
	ldr r0, [r5, #8]
	lsls r3, r3, #16
	adds r0, r0, r3
	mov r3, r8
	str r3, [sp, #8]
	ldr r1, [r5, #12]
	ldr r2, [r5, #16]
	movs r3, #0
	str r6, [sp, #0]
	str r6, [sp, #4]
	str r7, [sp, #12]
	bl 0x02008118
	movs r0, #0
.L_0200073e:
	sub sp, #-56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x1e40
	.2byte 0x0300
	.global Func_02000754
	.thumb_func
Func_02000754:
	push {r5, r6, lr}
	ldr r3, [pc, #116]
	movs r0, #0
	ldr r6, [r3]
	bl 0x0200ceac
	adds r5, r0, #0
	bl 0x0200ce94
	movs r0, #228
	bl 0x0200cffc
	ldr r3, [pc, #96]
	str r3, [r5, #108]
	ldr r3, [pc, #96]
	movs r0, #0
	str r3, [r5, #48]
	movs r1, #2
	bl 0x0200cefc
	movs r2, #6
	negs r2, r2
	movs r1, #0
	movs r0, #0
	bl 0x0200cee4
	movs r0, #0
	bl 0x0200ceec
.L_0200078e:
	movs r1, #15
	movs r0, #0
	bl 0x0200cf34
	movs r0, #0
	bl 0x0200ceac
	movs r1, #0
	bl 0x0200ce44
	movs r3, #0
	str r3, [r5, #108]
	movs r0, #30
	bl 0x0200ce8c
	bl 0x0200cfcc
	bl 0x0200cfd4
	movs r3, #182
	lsls r3, r3, #1
	adds r6, r6, r3
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl 0x0200cfa4
	bl 0x0200ce9c
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x86a1
	.2byte 0x0200
	.2byte 0x3333
	.2byte 0x0000
	.global Func_020007d8
	.thumb_func
Func_020007d8:
	push {r5, r6, r7, lr}
	movs r0, #0
	bl 0x0200ceac
	adds r5, r0, #0
	ldr r0, [pc, #168]
	bl 0x0200ce74
	adds r6, r0, #0
	cmp r6, #0
	bne 0x02008886
	bl 0x0200ce94
	adds r7, r5, #0
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	adds r7, #85
	bl 0x0200cf84
	strb r6, [r7]
	movs r3, #18
	ldrsh r2, [r5, r3]
	movs r3, #10
	ldrsh r1, [r5, r3]
	ldr r3, [pc, #124]
	lsls r2, r2, #16
	adds r2, r2, r3
	lsls r1, r1, #16
.L_0200081a:
	movs r0, #0
	bl 0x0200cef4
	movs r1, #15
	movs r0, #0
	bl 0x0200cf34
	movs r0, #0
	bl 0x0200ceac
	movs r1, #0
	bl 0x0200ce44
	bl 0x0200cfc4
	bl 0x0200cfd4
	movs r0, #228
	bl 0x0200cffc
	ldr r3, [pc, #80]
	movs r0, #0
	str r3, [r5, #108]
	ldr r1, [pc, #76]
	ldr r2, [pc, #80]
	bl 0x0200ceb4
	movs r2, #8
.L_02000852:
	movs r0, #0
	movs r1, #0
	bl 0x0200cfe4
	movs r1, #0
	movs r0, #0
	bl 0x0200cf34
	movs r0, #0
	bl 0x0200ceac
	movs r1, #1
	bl 0x0200ce44
	movs r0, #0
	movs r1, #0
	movs r2, #8
	bl 0x0200cfe4
	movs r3, #3
	strb r3, [r7]
	str r6, [r5, #108]
	bl 0x0200cff4
	bl 0x0200ce9c
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0109
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0x86a1
	.2byte 0x0200
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x3333
	.2byte 0x0000
	.global Func_020008a0
	.thumb_func
Func_020008a0:
	push {lr}
	movs r0, #0
	bl 0x0200ceac
	ldr r3, [pc, #8]
	ldr r3, [r3]
	str r0, [r3, #24]
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ee0
	.global Func_020008b8
	.thumb_func
Func_020008b8:
	ldr r3, [pc, #8]
	ldr r2, [r3]
	movs r3, #0
	str r3, [r2, #24]
	bx lr
	.2byte 0x0000
	.4byte 0x03001ee0
	.global Func_020008c8
	.thumb_func
Func_020008c8:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #0
	bl 0x0200ceac
	movs r2, #35
	adds r2, r2, r5
	mov r12, r2
	movs r3, #2
	ldrb r2, [r2]
	adds r1, r3, #0
	orrs r1, r2
	mov r3, r12
	strb r1, [r3]
	ldr r2, [r0, #16]
	ldr r3, [r5, #16]
	cmp r2, r3
	bge .L_020008c8_0
	subs r3, r3, r2
	movs r2, #128
	lsls r2, r2, #11
	adds r3, r3, r2
	ldr r2, [r5, #12]
	adds r2, r2, r3
	ldr r3, [r0, #12]
	cmp r3, r2
	bgt .L_020008c8_0
	movs r3, #253
	ands r1, r3
	mov r3, r12
	strb r1, [r3]
.L_020008c8_0:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02000910
	.thumb_func
Func_02000910:
	push {r5, r6, lr}
	adds r5, r1, #0
	bl 0x0200ceac
	adds r6, r0, #0
	adds r0, r5, #0
	bl 0x0200ceac
	ldr r2, [r6, #16]
	ldr r3, [r0, #16]
	cmp r2, r3
	bgt .L_02000910_0
	ldr r3, [r0, #8]
	ldr r2, [r6, #8]
	str r3, [r6, #8]
	str r2, [r0, #8]
	ldr r3, [r0, #12]
	ldr r2, [r6, #12]
	str r3, [r6, #12]
	str r2, [r0, #12]
	ldr r3, [r0, #16]
	ldr r2, [r6, #16]
	str r3, [r6, #16]
	str r2, [r0, #16]
	movs r0, #1
	bl 0x0200cda4
.L_02000910_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_0200094c
	.thumb_func
Func_0200094c:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r5, #60
.L_0200094c_1:
	cmp r5, #0
	beq .L_0200094c_0
	movs r0, #1
	bl 0x0200cda4
	ldr r3, [r6, #40]
	subs r5, #1
	cmp r3, #0
	bne .L_0200094c_1
.L_0200094c_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200096c
	.thumb_func
Func_0200096c:
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
	bl 0x0200cd9c
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_0200096c_0
	adds r3, #15
.L_0200096c_0:
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
	.global Func_020009c8
	.thumb_func
Func_020009c8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #0
	sub sp, #68
	bl 0x0200ceac
	adds r7, r0, #0
	bl 0x0200ce94
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r1, r1
	movs r3, #0
	negs r0, r0
	bl 0x0200cf84
	bl 0x0200cdf4
	movs r0, #1
	bl 0x0200cda4
	movs r3, #130
	lsls r3, r3, #16
	str r3, [r7, #12]
	movs r3, #128
	lsls r3, r3, #8
	adds r5, r7, #0
	str r3, [r7, #72]
	adds r5, #85
	movs r3, #0
	str r3, [r7, #68]
	strb r3, [r5]
	bl 0x0200cfc4
	bl 0x0200cfd4
	movs r0, #30
	bl 0x0200ce8c
	movs r0, #204
	bl 0x0200cffc
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl 0x0200ce8c
	add r0, sp, #28
	movs r3, #7
	str r3, [r0, #4]
	ldr r3, [pc, #188]
	str r3, [r0, #36]
	ldr r3, [pc, #188]
	movs r2, #0
	str r3, [r0, #8]
	str r3, [r0, #12]
	mov r10, r0
	mov r8, r2
	add r6, sp, #16
.L_020009c8_0:
	mov r3, r8
	lsls r5, r3, #12
	adds r0, r5, #0
	bl 0x0200cdc4
	movs r3, #0
	str r0, [r6]
	adds r0, r5, #0
	str r3, [r6, #4]
	bl 0x0200cdbc
	ldr r3, [r6]
	lsrs r2, r3, #31
	adds r2, r3, r2
	asrs r2, r2, #1
	adds r3, r3, r2
	str r0, [r6, #8]
	str r3, [r6]
	ldr r5, [r7, #8]
	ldr r2, [r7, #16]
	ldr r1, [r7, #12]
	ldr r4, [r6, #4]
	str r0, [sp, #4]
	ldr r0, [pc, #132]
	str r0, [sp, #8]
	mov r0, r10
	str r0, [sp, #12]
	adds r0, r5, #0
	str r4, [sp, #0]
	bl 0x02008118
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #16
	bls .L_020009c8_0
	movs r0, #188
	bl 0x0200cffc
	movs r0, #0
	ldr r1, [pc, #104]
	bl 0x0200cf74
	movs r0, #0
	movs r1, #22
	bl 0x0200cefc
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl 0x0200ce54
	movs r0, #1
	movs r1, #1
	ldr r2, [pc, #72]
	negs r0, r0
	negs r1, r1
	bl 0x0200ce54
	bl 0x0200ce5c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200cf74
	bl 0x0200cff4
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #68]
	bl 0x0200ce9c
	sub sp, #-68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200896d
	.4byte 0x0000cccc
	.4byte 0x01090001
	.4byte 0x00000101
	.4byte 0x0000e666
	.global Func_02000b08
	.thumb_func
Func_02000b08:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #68
	bl 0x0200ceac
	add r2, sp, #16
	movs r3, #1
	str r3, [r2]
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, [pc, #108]
	str r3, [r2, #36]
	movs r3, #0
	mov r10, r0
	mov r9, r2
	mov r8, r3
	add r7, sp, #56
.L_02000b08_0:
	mov r2, r8
	lsls r5, r2, #12
	adds r0, r5, #0
	bl 0x0200cdc4
	movs r3, #0
	str r3, [r7, #4]
	str r0, [r7]
	adds r0, r5, #0
	bl 0x0200cdbc
	ldr r5, [r7]
	adds r6, r0, #0
	movs r1, #3
	adds r0, r5, #0
	str r6, [r7, #8]
	bl 0x0200cd9c
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
	bl 0x02008118
	movs r2, #2
	add r8, r2
	mov r3, r8
	cmp r3, #16
	bls .L_02000b08_0
	sub sp, #-68
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200896d
	.4byte 0x01030001
	.global Func_02000b98
	.thumb_func
Func_02000b98:
	push {r5, r6, r7, lr}
.L_02000b9a:
	mov r7, r8
	push {r7}
	ldr r3, [pc, #172]
	ldr r3, [r3]
	movs r2, #3
	ands r3, r2
	adds r7, r0, #0
	sub sp, #56
	movs r0, #0
	cmp r3, #0
	bne .L_02000b9a_0
	bl 0x0200cdb4
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #1
	lsrs r3, r3, #16
	cmp r3, #0
	bne .L_02000b9a_1
	movs r3, #128
	ldr r2, [r7, #56]
	lsls r3, r3, #24
	cmp r2, r3
	bne .L_02000b9a_2
	ldr r3, [r7, #64]
	cmp r3, r2
	beq .L_02000b9a_1
.L_02000b9a_2:
	movs r0, #246
	bl 0x0200cffc
.L_02000b9a_1:
	movs r3, #0
	mov r8, r3
	movs r3, #143
	add r5, sp, #16
	lsls r3, r3, #1
	strh r3, [r5, #24]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #8]
	str r3, [r5, #12]
	ldr r3, [pc, #100]
	str r3, [r5, #16]
	str r3, [r5, #20]
	bl 0x0200cdb4
	adds r3, r0, #0
	lsls r0, r3, #3
	adds r0, r0, r3
	lsrs r0, r0, #16
	subs r0, #4
	movs r1, #10
	lsls r0, r0, #16
	bl 0x0200cd9c
	adds r6, r0, #0
	bl 0x0200cdb4
	adds r3, r0, #0
	lsls r0, r3, #3
	adds r0, r0, r3
	lsrs r0, r0, #16
	subs r0, #4
	movs r1, #10
	lsls r0, r0, #16
	bl 0x0200cd9c
	ldr r2, [r7, #16]
	ldr r3, [pc, #48]
	adds r2, r2, r3
	mov r3, r8
	ldr r4, [r7, #8]
	ldr r1, [r7, #12]
	str r3, [sp, #0]
	ldr r3, [pc, #40]
	str r0, [sp, #4]
	str r3, [sp, #8]
	adds r0, r4, #0
	adds r3, r6, #0
	str r5, [sp, #12]
	bl 0x02008118
	movs r0, #0
.L_02000b9a_0:
	sub sp, #-56
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001e40
	.4byte 0xfffffeb9
	.4byte 0xffff0000
	.4byte 0x001c0001
	.global Func_02000c5c
	.thumb_func
Func_02000c5c:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r3, r1, #0
	lsls r4, r4, #16
	movs r0, #142
	adds r6, r2, #0
	lsls r3, r3, #16
	lsls r0, r0, #1
	adds r1, r4, #0
	movs r2, #0
	bl 0x0200cde4
	adds r5, r0, #0
	movs r0, #0
	cmp r5, #0
	beq .L_02000c5c_0
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #48]
	str r3, [r5, #52]
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200ce44
	adds r0, r5, #0
	movs r1, #7
	bl 0x0200cdd4
	adds r3, r5, #0
	movs r2, #0
	adds r3, #85
	strb r2, [r3]
	adds r1, r5, #0
	adds r3, #4
	str r2, [r5, #12]
	adds r1, #35
	strb r2, [r3]
	movs r3, #2
	strb r3, [r1]
	ldr r3, [pc, #24]
	str r3, [r5, #108]
	adds r3, r5, #0
	adds r3, #99
	adds r0, r5, #0
	strb r2, [r3]
	adds r1, r6, #0
	bl 0x0200cddc
	adds r0, r5, #0
.L_02000c5c_0:
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x02008b99
	.global Func_02000cc8
	.thumb_func
Func_02000cc8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #0
	sub sp, #12
	bl 0x0200ceac
	movs r2, #85
	adds r5, r0, #0
	adds r2, r2, r5
	ldrb r3, [r2]
	ldr r7, [pc, #284]
	mov r9, r3
	ldr r3, [r5, #8]
	mov r8, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r7
	mov r6, sp
	adds r3, r3, r2
	str r3, [r6]
	ldr r3, [r5, #12]
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	ands r3, r7
	adds r3, r3, r2
	str r3, [r6, #8]
	movs r2, #128
	ldrh r1, [r5, #6]
	lsls r2, r2, #6
	movs r3, #192
	lsls r3, r3, #8
	adds r1, r1, r2
	movs r0, #128
	ands r1, r3
	lsls r0, r0, #13
	adds r2, r6, #0
	mov r10, r3
	bl 0x0200cdcc
	adds r0, r5, #0
	adds r1, r6, #0
	bl 0x0200ce3c
	cmp r0, #1
	beq 0x02008dec
	adds r0, r6, #0
	adds r1, r5, #0
	bl 0x0200832c
	cmp r0, #0
	bne 0x02008dec
	ldr r3, [r5, #8]
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r7
	adds r3, r3, r2
	str r3, [r6]
	ldr r3, [r5, #12]
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	ands r3, r7
	adds r3, r3, r2
	str r3, [r6, #8]
	movs r3, #128
	ldrh r1, [r5, #6]
	lsls r3, r3, #6
.L_02000d52:
	adds r1, r1, r3
	mov r2, r10
	movs r0, #128
	ands r1, r2
	lsls r0, r0, #14
	adds r2, r6, #0
	bl 0x0200cdcc
	adds r0, r6, #0
	adds r1, r5, #0
	bl 0x0200832c
	cmp r0, #0
	bne .L_02000d52_0
	adds r0, r5, #0
	adds r1, r6, #0
	bl 0x0200ce3c
	cmp r0, #0
	bne .L_02000d52_0
	bl 0x0200ce94
	movs r1, #6
	adds r0, r5, #0
	bl 0x0200cdd4
	movs r0, #6
	bl 0x0200cda4
	movs r0, #152
	bl 0x0200cffc
	adds r0, r5, #0
	movs r1, #7
	bl 0x0200cdd4
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #40]
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #126
	ands r3, r2
	mov r2, r8
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200ce44
	movs r3, #10
	ldrsh r2, [r6, r3]
	movs r3, #2
	ldrsh r1, [r6, r3]
	movs r0, #0
	bl 0x0200ced4
	adds r0, r5, #0
	movs r1, #6
	bl 0x0200cdd4
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200ce44
	mov r2, r9
	mov r3, r8
	strb r2, [r3]
	bl 0x0200ce9c
	movs r0, #1
	b .L_02000d52_1
.L_02000d52_0:
	movs r0, #0
.L_02000d52_1:
	sub sp, #-12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfff0
	.global Func_02000e04
	.thumb_func
Func_02000e04:
	push {lr}
	ldr r3, [pc, #68]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_02000e04_0
	ldr r0, [pc, #56]
	b .L_02000e04_1
.L_02000e04_0:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02000e04_2
	ldr r0, [pc, #56]
	b .L_02000e04_1
.L_02000e04_2:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02000e04_3
	ldr r0, [pc, #52]
	b .L_02000e04_1
.L_02000e04_3:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02000e04_4
	ldr r0, [pc, #52]
	b .L_02000e04_1
.L_02000e04_4:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02000e04_5
	ldr r0, [pc, #48]
	b .L_02000e04_1
.L_02000e04_5:
	ldr r0, [pc, #48]
.L_02000e04_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000b5
	.4byte 0x0200dd68
	.4byte 0x000000b7
	.4byte 0x0200e020
	.4byte 0x000000b8
	.4byte 0x0200e230
	.4byte 0x000000b9
	.4byte 0x0200e350
	.4byte 0x000000ba
	.4byte 0x0200e548
	.4byte 0x0200ddc8
	.global Func_02000e7c
	.thumb_func
Func_02000e7c:
	movs r0, #0
	bx lr
	.global Func_02000e80
	.thumb_func
Func_02000e80:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200e740
	.global Func_02000e88
	.thumb_func
Func_02000e88:
	push {r5, lr}
	ldr r3, [pc, #88]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #80]
	cmp r2, r3
	bne .L_02000e88_0
	ldr r0, [pc, #76]
	b .L_02000e88_1
.L_02000e88_0:
	ldr r3, [pc, #76]
	cmp r2, r3
	bne .L_02000e88_2
	ldr r5, [pc, #76]
	b .L_02000e88_3
.L_02000e88_2:
	ldr r3, [pc, #76]
	cmp r2, r3
	bne .L_02000e88_4
	ldr r5, [pc, #72]
	b .L_02000e88_3
.L_02000e88_4:
	ldr r3, [pc, #72]
	cmp r2, r3
	bne .L_02000e88_5
	ldr r5, [pc, #72]
	b .L_02000e88_3
.L_02000e88_5:
	ldr r3, [pc, #72]
	cmp r2, r3
	bne .L_02000e88_6
	ldr r5, [pc, #68]
	b .L_02000e88_3
.L_02000e88_6:
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_02000e88_7
	ldr r5, [pc, #68]
.L_02000e88_3:
	adds r0, r5, #0
	bl 0x0200cea4
	adds r0, r5, #0
	b .L_02000e88_1
.L_02000e88_7:
	ldr r0, [pc, #60]
.L_02000e88_1:
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000b5
	.4byte 0x0200e904
	.4byte 0x000000b6
	.4byte 0x0200e9c4
	.4byte 0x000000b7
	.4byte 0x0200eb74
	.4byte 0x000000b8
	.4byte 0x0200ec04
	.4byte 0x000000b9
	.4byte 0x0200ec64
	.4byte 0x000000ba
	.4byte 0x0200ecf4
	.4byte 0x0200e8ec
	.global Func_02000f1c
	.thumb_func
Func_02000f1c:
	push {lr}
	bl 0x0200ce94
	ldr r0, [pc, #16]
	movs r1, #1
	bl 0x0200ce6c
	bl 0x0200ce9c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000953
	.global Func_02000f38
	.thumb_func
Func_02000f38:
	push {r5, r6, lr}
	sub sp, #8
	bl 0x0200ce94
	movs r1, #3
	movs r0, #8
	bl 0x0200cf14
	ldr r0, [pc, #104]
	bl 0x0200cf44
	movs r0, #8
	movs r1, #0
	movs r2, #20
	movs r6, #10
	movs r5, #8
	bl 0x0200cf54
.L_02000f38_1:
	movs r1, #15
	movs r0, #8
	bl 0x0200cf34
	movs r0, #2
	bl 0x0200cda4
	movs r0, #8
	movs r1, #0
	bl 0x0200cf34
	adds r0, r5, #0
	bl 0x0200cda4
	cmp r5, #3
	bls .L_02000f38_0
	subs r5, #1
.L_02000f38_0:
	subs r6, #1
	cmp r6, #0
	bne .L_02000f38_1
	ldr r0, [pc, #48]
	bl 0x0200ce7c
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200cef4
	movs r3, #7
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #7
	movs r1, #17
	movs r2, #2
	movs r3, #1
	bl 0x0200ce34
	bl 0x0200ce9c
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0000266d
	.4byte 0x00000981
	.global Func_02000fbc
	.thumb_func
Func_02000fbc:
	push {lr}
	bl 0x0200ce94
	ldr r0, [pc, #40]
	bl 0x0200cf44
	movs r2, #20
	movs r0, #11
	movs r1, #0
	bl 0x0200cf54
	movs r0, #11
	movs r1, #2
	bl 0x0200cf14
	movs r0, #11
	movs r1, #0
	bl 0x0200cf4c
	bl 0x0200ce9c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00002670
	.global Func_02000ff0
	.thumb_func
Func_02000ff0:
	push {r5, lr}
	movs r0, #12
	bl 0x0200ceac
	adds r5, r0, #0
	bl 0x0200ce94
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #54
	beq .L_02000ff0_0
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #6
	bne .L_02000ff0_1
.L_02000ff0_0:
	ldr r0, [pc, #16]
	bl 0x0200ce7c
.L_02000ff0_1:
	bl 0x0200ce9c
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000987
	.global Func_02001024
	.thumb_func
Func_02001024:
	push {lr}
	bl 0x0200ce94
	movs r0, #0
	movs r1, #1
	bl 0x0200cefc
	ldr r0, [pc, #16]
	movs r1, #1
	bl 0x0200ce6c
	bl 0x0200ce9c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00002682
	.global Func_02001048
	.thumb_func
Func_02001048:
	push {r5, r6, lr}
	ldr r0, [pc, #124]
	sub sp, #8
	bl 0x0200ce84
	movs r3, #23
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
.L_0200105a:
	movs r0, #35
	movs r1, #8
	movs r2, #1
	movs r3, #3
	bl 0x0200ce34
	movs r5, #3
	movs r6, #1
	movs r0, #35
	movs r1, #8
	movs r2, #23
	movs r3, #8
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200ce1c
	movs r0, #99
	movs r1, #8
	movs r2, #87
	movs r3, #8
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200ce1c
	movs r3, #46
	movs r2, #55
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #57
.L_02001094:
	movs r1, #55
	movs r2, #3
	movs r3, #3
	bl 0x0200ce34
	movs r0, #57
	movs r1, #55
	movs r2, #46
	movs r3, #55
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200ce1c
	movs r0, #121
	movs r1, #55
	movs r2, #110
	movs r3, #55
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200ce1c
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0161
	.2byte 0x0000
	.global Func_020010cc
	.thumb_func
Func_020010cc:
	push {r5, r6, lr}
	ldr r0, [pc, #124]
	sub sp, #8
	bl 0x0200ce7c
	movs r3, #23
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #36
	movs r1, #8
	movs r2, #1
	movs r3, #3
	bl 0x0200ce34
	movs r5, #3
	movs r6, #1
	movs r0, #36
	movs r1, #8
	movs r2, #23
	movs r3, #8
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200ce1c
	movs r0, #100
	movs r1, #8
	movs r2, #87
	movs r3, #8
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200ce1c
	movs r3, #46
	movs r2, #55
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #53
	movs r1, #55
	movs r2, #3
	movs r3, #3
	bl 0x0200ce34
	movs r0, #53
	movs r1, #55
	movs r2, #46
	movs r3, #55
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200ce1c
	movs r0, #117
	movs r1, #55
	movs r2, #110
	movs r3, #55
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200ce1c
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000161
	.global Func_02001150
	.thumb_func
Func_02001150:
	push {r5, lr}
	movs r0, #0
	bl 0x0200ceac
	adds r5, r0, #0
	bl 0x0200ce94
	ldr r1, [pc, #176]
	movs r0, #0
	bl 0x0200cebc
	movs r0, #0
	bl 0x0200cec4
	movs r0, #0
	movs r1, #6
	bl 0x0200cf34
	movs r1, #128
	lsls r1, r1, #11
	movs r2, #128
	str r1, [r5, #40]
	movs r0, #0
	lsls r2, r2, #10
	bl 0x0200ceb4
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #54
	bgt .L_02001150_0
	movs r0, #0
	bl 0x0200ceac
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r2, #210
	b .L_02001150_1
.L_02001150_0:
	movs r0, #0
	bl 0x0200ceac
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r2, #238
.L_02001150_1:
	movs r3, #10
	ldrsh r1, [r5, r3]
	lsls r2, r2, #2
	movs r0, #0
	bl 0x0200cedc
	movs r0, #1
	bl 0x0200ce8c
	movs r0, #0
	bl 0x0200ceac
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #20
	bl 0x0200ce8c
	ldr r3, [pc, #56]
	movs r1, #129
	str r3, [r5, #108]
	movs r2, #60
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200cf6c
	movs r0, #0
	movs r1, #4
	bl 0x0200cf0c
	movs r0, #0
	movs r1, #0
	bl 0x0200cf34
	movs r0, #0
	movs r1, #4
	bl 0x0200cf0c
	movs r3, #0
	str r3, [r5, #108]
	bl 0x0200ce9c
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0200d21c
	.4byte 0x020085e5
	.global Func_02001218
	.thumb_func
Func_02001218:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #684]
	ldr r7, [r3]
	ldr r3, [pc, #684]
	adds r2, r7, r3
	movs r3, #0
	strh r3, [r2]
	ldr r2, [pc, #680]
	movs r5, #1
	adds r3, r7, r2
	strh r5, [r3]
	sub sp, #8
	bl 0x0200ce94
	movs r0, #0
	movs r1, #1
	bl 0x0200cefc
	ldr r0, [pc, #660]
	movs r1, #1
	bl 0x0200ce6c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl 0x0200cfb4
	movs r1, #0
	ldr r0, [pc, #644]
	bl 0x0200cfac
	movs r0, #120
	bl 0x0200cfbc
	movs r0, #100
	bl 0x0200ce8c
	movs r0, #142
	bl 0x0200cffc
	movs r0, #30
	bl 0x0200ce8c
	movs r1, #0
	ldr r0, [pc, #616]
	bl 0x0200cfac
	movs r0, #60
	bl 0x0200cfbc
	movs r0, #70
	bl 0x0200ce8c
	ldr r0, [pc, #604]
	bl 0x0200ce74
	cmp r0, #0
	bne .L_02001218_0
	ldr r0, [pc, #596]
	bl 0x0200ce74
	cmp r0, #0
	bne .L_02001218_0
	ldr r3, [pc, #592]
	ldr r3, [r3]
	ands r3, r5
	cmp r3, #0
	beq .L_02001218_1
	ldr r0, [pc, #572]
	bl 0x0200ce7c
	b .L_02001218_0
.L_02001218_1:
	ldr r0, [pc, #568]
	bl 0x0200ce7c
.L_02001218_0:
	ldr r0, [pc, #560]
	bl 0x0200ce74
	cmp r0, #0
	bne .L_02001218_2
	ldr r0, [pc, #548]
	bl 0x0200ce7c
	ldr r0, [pc, #548]
	bl 0x0200ce84
	movs r3, #7
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #103
	movs r1, #27
	movs r2, #89
	movs r3, #27
	bl 0x0200ce1c
	movs r5, #3
	movs r6, #2
	movs r0, #41
	movs r1, #90
	movs r2, #27
	movs r3, #92
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #90
	movs r2, #29
	movs r3, #93
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #90
	movs r2, #27
	movs r3, #94
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #90
	movs r2, #27
	movs r3, #96
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #90
	movs r2, #29
	movs r3, #97
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #96
	movs r2, #25
	movs r3, #91
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #92
	movs r2, #25
	movs r3, #93
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #96
	movs r2, #25
	movs r3, #95
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #96
	movs r2, #25
	movs r3, #97
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #96
	movs r2, #27
	movs r3, #96
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #96
	movs r2, #29
	movs r3, #97
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	b .L_02001218_3
.L_02001218_2:
	ldr r0, [pc, #340]
	bl 0x0200ce7c
	ldr r0, [pc, #328]
	bl 0x0200ce84
	movs r3, #7
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #111
	movs r1, #27
	movs r2, #89
	movs r3, #27
	bl 0x0200ce1c
	movs r5, #3
	movs r6, #2
	movs r0, #41
	movs r1, #90
	movs r2, #25
	movs r3, #91
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #90
	movs r2, #25
	movs r3, #93
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #90
	movs r2, #25
	movs r3, #95
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #90
	movs r2, #25
	movs r3, #97
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #90
	movs r2, #27
	movs r3, #96
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #90
	movs r2, #29
	movs r3, #97
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #94
	movs r2, #27
	movs r3, #92
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #96
	movs r2, #29
	movs r3, #93
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #94
	movs r2, #27
	movs r3, #94
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #96
	movs r2, #27
	movs r3, #96
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
	movs r0, #41
	movs r1, #96
	movs r2, #29
	movs r3, #97
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200ce1c
.L_02001218_3:
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl 0x0200cfac
	movs r0, #20
	bl 0x0200cfbc
	movs r0, #40
	bl 0x0200ce8c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl 0x0200cf7c
	movs r0, #228
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #96]
	movs r3, #1
	lsls r0, r0, #17
	bl 0x0200cf84
	bl 0x0200cf8c
	movs r0, #50
	bl 0x0200ce8c
	movs r0, #228
	movs r1, #1
	lsls r0, r0, #17
	negs r1, r1
	ldr r2, [pc, #72]
	movs r3, #1
	bl 0x0200cf84
	bl 0x0200cf8c
	bl 0x0200ce9c
	ldr r3, [pc, #24]
	adds r2, r7, r3
	movs r3, #0
	strh r3, [r2]
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000cba
	.4byte 0x00000cb6
	.4byte 0x00002688
	.4byte 0x00010005
	.4byte 0x00007fff
	.4byte 0x00000982
	.4byte 0x00000983
	.4byte 0x03001e40
	.4byte 0x021e0000
	.4byte 0x01a70000
	.global Func_020014f4
	.thumb_func
Func_020014f4:
	push {lr}
	bl 0x0200ce94
	movs r0, #12
	bl 0x0200ceac
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #53
	beq .L_020014f4_0
	b .L_020014f4_1
.L_020014f4_0:
	ldr r0, [pc, #268]
	bl 0x0200ce74
	cmp r0, #0
	bne .L_020014f4_1
	ldr r0, [pc, #256]
	bl 0x0200ce7c
	movs r0, #0
	bl 0x0200ceac
	cmp r0, #0
	beq .L_020014f4_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200cef4
.L_020014f4_2:
	movs r0, #1
	ldr r1, [pc, #232]
	ldr r2, [pc, #236]
	bl 0x0200ceb4
	movs r1, #206
	movs r0, #1
	lsls r1, r1, #2
	movs r2, #88
	bl 0x0200cedc
	movs r1, #206
	movs r0, #1
	lsls r1, r1, #2
	movs r2, #104
	bl 0x0200cedc
	movs r2, #0
	movs r1, #0
	movs r0, #1
	bl 0x0200cf2c
	movs r0, #20
	bl 0x0200ce8c
	movs r1, #4
	movs r0, #1
	bl 0x0200cf0c
	movs r0, #20
	bl 0x0200ce8c
	ldr r0, [pc, #180]
	bl 0x0200cf44
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200cf54
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x0200cf5c
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200cf6c
	movs r2, #0
	movs r1, #0
	movs r0, #1
	bl 0x0200cf24
	movs r0, #20
	bl 0x0200ce8c
	movs r1, #2
	movs r0, #1
	bl 0x0200cf14
	movs r0, #20
	bl 0x0200ce8c
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200cf54
	movs r0, #0
	movs r1, #3
	bl 0x0200cefc
	movs r1, #3
	movs r0, #1
	bl 0x0200cf0c
	movs r0, #30
	bl 0x0200ce8c
	movs r1, #206
	movs r0, #1
	lsls r1, r1, #2
	movs r2, #88
	bl 0x0200cedc
	movs r0, #1
	movs r1, #2
	bl 0x0200cefc
	movs r0, #0
	bl 0x0200ceac
	cmp r0, #0
	beq .L_020014f4_3
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200cecc
.L_020014f4_3:
	movs r0, #1
	bl 0x0200ceec
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200cef4
	bl 0x0200ce9c
.L_020014f4_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000986
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00002691
	.global Func_02001628
	.thumb_func
Func_02001628:
	push {lr}
	bl 0x0200ce94
	bl 0x02008374
	movs r0, #20
	bl 0x0200ce8c
	bl 0x0200ce9c
	bl 0x020094f4
	pop {r0}
	bx r0
	.global Func_02001644
	.thumb_func
Func_02001644:
	push {r5, lr}
	movs r0, #13
	sub sp, #8
	bl 0x0200ceac
	adds r5, r0, #0
	bl 0x0200ce94
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #42
	bne .L_02001644_0
	movs r0, #30
	bl 0x0200ce8c
	movs r0, #188
	bl 0x0200cffc
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r3, [pc, #44]
	movs r0, #128
	str r3, [r5, #20]
	str r3, [r5, #12]
	lsls r0, r0, #2
	bl 0x0200ce7c
	movs r3, #3
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #44
	movs r1, #117
	movs r2, #41
	movs r3, #117
	bl 0x0200ce1c
.L_02001644_0:
	bl 0x0200ce9c
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xfffe0000
	.global Func_020016a4
	.thumb_func
Func_020016a4:
	push {r5, lr}
	movs r0, #0
	sub sp, #8
	bl 0x0200ceac
	adds r5, r0, #0
	movs r1, #10
	ldrsh r3, [r5, r1]
	movs r1, #18
	ldrsh r2, [r5, r1]
	ldr r1, [pc, #184]
	adds r3, r3, r1
	cmp r3, #7
	bhi .L_020016a4_0
	movs r3, #197
	lsls r3, r3, #2
	cmp r2, r3
	blt .L_020016a4_0
	movs r1, #199
	lsls r1, r1, #2
	cmp r2, r1
	blt .L_020016a4_1
.L_020016a4_0:
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #53
	movs r1, #50
	movs r2, #42
	movs r3, #49
	bl 0x0200ce1c
	movs r3, #3
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #117
	movs r2, #41
	movs r3, #117
	movs r0, #55
	bl 0x0200ce1c
	ldr r0, [pc, #128]
	bl 0x0200ce84
	adds r0, r5, #0
	adds r0, #85
	ldrb r1, [r0]
	movs r3, #1
	movs r2, #0
	orrs r3, r1
	strb r3, [r0]
	str r2, [r5, #20]
	str r2, [r5, #12]
	b .L_020016a4_2
.L_020016a4_1:
	ldr r0, [pc, #100]
	bl 0x0200ce74
	cmp r0, #0
	bne .L_020016a4_2
	bl 0x0200ce94
	movs r0, #5
	bl 0x0200ce8c
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #52
	movs r1, #50
	movs r2, #42
	movs r3, #49
	bl 0x0200ce1c
	movs r3, #3
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #117
	movs r2, #41
	movs r3, #117
	movs r0, #52
	bl 0x0200ce1c
	ldr r0, [pc, #44]
	bl 0x0200ce7c
	movs r0, #161
	bl 0x0200cffc
	adds r1, r5, #0
	adds r1, #85
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	ldr r3, [pc, #24]
	str r3, [r5, #20]
	str r3, [r5, #12]
	bl 0x0200ce9c
.L_020016a4_2:
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xfffffd5c
	.4byte 0x00000201
	.4byte 0xfffe0000
	.global Func_02001780
	.thumb_func
Func_02001780:
	push {lr}
	ldr r3, [pc, #116]
	ldr r2, [r3]
	ldr r3, [pc, #116]
	adds r1, r2, r3
	movs r3, #0
	strh r3, [r1]
	ldr r3, [pc, #112]
	adds r2, r2, r3
	movs r3, #1
	strh r3, [r2]
	bl 0x0200ce94
	ldr r0, [pc, #104]
	bl 0x0200cf44
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl 0x0200cf24
	movs r0, #10
	bl 0x0200ce8c
	movs r0, #10
	movs r1, #0
	movs r2, #20
	bl 0x0200cf54
	movs r1, #224
	movs r2, #0
	movs r0, #10
	lsls r1, r1, #8
	bl 0x0200cf5c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200cf7c
	movs r0, #224
	movs r1, #1
	movs r2, #216
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	bl 0x0200cf84
	bl 0x0200cf8c
	movs r0, #10
	movs r1, #0
	bl 0x0200cf4c
	bl 0x0200ce9c
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x00000cba
	.4byte 0x00000cb6
	.4byte 0x0000267d
	.global Func_02001808
	.thumb_func
Func_02001808:
	push {r5, r6, lr}
	movs r0, #0
	sub sp, #8
	bl 0x0200ceac
	adds r5, r0, #0
	bl 0x0200ce94
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r1, r1
	negs r0, r0
	bl 0x0200cf84
	movs r0, #0
	bl 0x0200ceac
	movs r1, #0
	bl 0x0200ce44
	movs r3, #128
	lsls r3, r3, #7
	movs r1, #192
	movs r2, #192
	strh r3, [r5, #6]
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200ceb4
	movs r2, #138
	lsls r2, r2, #2
	movs r3, #10
	ldrsh r1, [r5, r3]
	movs r0, #0
	bl 0x0200ced4
	movs r0, #10
	bl 0x0200ce8c
	movs r1, #22
	movs r0, #0
	bl 0x0200cefc
	movs r0, #30
	bl 0x0200ce8c
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200cf74
	movs r1, #2
	movs r0, #0
	bl 0x0200cf14
	movs r0, #20
	bl 0x0200ce8c
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r5, #6]
	movs r0, #0
	movs r1, #5
	bl 0x0200cefc
	movs r1, #24
	movs r0, #0
	bl 0x0200cf04
	movs r0, #40
	bl 0x0200ce8c
	ldr r3, [pc, #80]
	ldr r2, [r5, #16]
	str r3, [r5, #72]
	movs r3, #144
	movs r6, #0
	lsls r3, r3, #15
	adds r2, r2, r3
	str r6, [r5, #68]
	ldr r0, [r5, #8]
	movs r1, #0
	movs r3, #223
	bl 0x02008058
	movs r3, #34
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r2, #5
	movs r3, #1
	movs r1, #35
	movs r0, #34
	bl 0x0200ce34
	movs r0, #0
	bl 0x0200894c
	movs r1, #15
	movs r0, #0
	bl 0x0200cf34
	movs r0, #20
	bl 0x0200cfa4
	bl 0x0200cfcc
	bl 0x0200cfd4
	bl 0x0200ce9c
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00009999
	.global Func_020018f8
	.thumb_func
Func_020018f8:
	push {r5, lr}
	movs r0, #8
	sub sp, #8
	bl 0x0200ceac
	bl 0x0200ce94
	movs r3, #12
	movs r2, #44
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #19
.L_02001910:
	movs r1, #44
	movs r2, #4
	movs r3, #1
	bl 0x0200ce34
	movs r3, #11
	movs r2, #51
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #17
	movs r1, #51
	movs r2, #2
	movs r3, #2
	bl 0x0200ce34
	movs r5, #0
.L_02001910_0:
	adds r0, r5, #0
	adds r0, #8
	bl 0x0200ceac
	ldr r2, [r0, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #12
	movs r1, #50
	movs r2, #1
	movs r3, #1
	adds r5, #1
	bl 0x0200ce34
	cmp r5, #2
	bls .L_02001910_0
	movs r0, #10
	movs r1, #9
	bl 0x02008910
	bl 0x0200ce9c
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200196c
	.thumb_func
Func_0200196c:
	push {lr}
	sub sp, #8
	bl 0x0200ce94
	movs r3, #12
	movs r2, #44
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #19
	movs r1, #44
	movs r2, #4
	movs r3, #1
	bl 0x0200ce34
	bl 0x02008374
	bl 0x020098f8
	bl 0x0200ce9c
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200199c
	.thumb_func
Func_0200199c:
	push {r5, lr}
	movs r5, #0
	adds r0, r5, #0
	adds r0, #11
.L_020019a4:
	bl 0x0200ceac
	adds r5, #1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r3, #45
	movs r0, #0
	bl 0x0200ce64
	cmp r5, #1
	bls 0x020099a0
	pop {r5}
	pop {r0}
	bx r0
	.global Func_020019c0
	.thumb_func
Func_020019c0:
	push {r5, lr}
	movs r5, #0
.L_020019c0_1:
	adds r0, r5, #0
	adds r0, #11
	bl 0x0200ceac
	ldr r2, [pc, #28]
	ldr r3, [r0, #12]
	cmp r3, r2
	ble .L_020019c0_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r3, #255
	movs r0, #0
	bl 0x0200ce64
.L_020019c0_0:
	adds r5, #1
	cmp r5, #1
	bls .L_020019c0_1
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xfff00000
	.global Func_020019f0
	.thumb_func
Func_020019f0:
	push {lr}
	bl 0x0200ce94
	bl 0x02008cc8
	cmp r0, #0
	bne .L_020019f0_0
	bl 0x0200999c
	bl 0x02008374
	bl 0x020099c0
.L_020019f0_0:
	bl 0x0200ce9c
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001a14
	.thumb_func
Func_02001a14:
	push {lr}
	movs r2, #35
	adds r2, r2, r0
	mov r12, r2
	ldrb r2, [r2]
	movs r3, #2
	orrs r3, r2
	mov r2, r12
	strb r3, [r2]
	adds r3, r0, #0
	movs r1, #0
	adds r3, #85
	strb r1, [r3]
	ldr r2, [r0, #8]
	ldr r3, [r0, #16]
	sub sp, #8
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #9
	movs r1, #24
	movs r2, #1
	movs r3, #1
	bl 0x0200ce34
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001a50
	.thumb_func
Func_02001a50:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	ldr r1, [r7, #80]
	ldrb r2, [r1, #9]
	movs r3, #12
	ands r3, r2
	cmp r3, #12
	bne .L_02001a50_0
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	movs r5, #0
	movs r2, #128
	lsls r2, r2, #18
	movs r1, #0
	movs r3, #223
	str r5, [r7, #68]
	ldr r0, [r7, #8]
	bl 0x02008058
	adds r6, r0, #0
	adds r0, r7, #0
	bl 0x0200894c
	str r5, [r7, #8]
	str r5, [r7, #16]
	adds r0, r6, #0
	bl 0x0200cdec
	b .L_02001a50_1
.L_02001a50_0:
	bl 0x020099c0
.L_02001a50_1:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02001a9c
	.thumb_func
Func_02001a9c:
	push {r5, lr}
	bl 0x0200ce94
	movs r0, #11
	bl 0x0200ceac
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #8
	bne .L_02001a9c_0
	bl 0x0200894c
	adds r0, r5, #0
	bl 0x02009a14
	b .L_02001a9c_1
.L_02001a9c_0:
	adds r0, r5, #0
	bl 0x02009a50
.L_02001a9c_1:
	movs r0, #12
	bl 0x0200ceac
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #7
	bne 0x02009ae0
.L_02001ad4:
	bl 0x0200894c
	adds r0, r5, #0
	bl 0x02009a14
	b .L_02001ad4_0
	.2byte 0x1c28
	.2byte 0xf7ff
	.2byte 0xffb5
.L_02001ad4_0:
	bl 0x0200ce9c
	pop {r5}
	pop {r0}
	bx r0
	.global Func_02001af0
	.thumb_func
Func_02001af0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #532]
	movs r1, #178
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r1, r1, r3
	movs r0, #0
	mov r10, r1
	sub sp, #56
	bl 0x0200ceac
	ldr r1, [pc, #516]
	movs r2, #10
	ldrsh r5, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r3, #0
	str r3, [r0, #12]
	adds r3, r5, r1
	cmp r3, #7
	bls .L_02001af0_0
	b .L_02001af0_1
.L_02001af0_0:
	movs r3, #133
	lsls r3, r3, #2
	cmp r2, r3
	bge .L_02001af0_2
	b .L_02001af0_1
.L_02001af0_2:
	movs r1, #135
	lsls r1, r1, #2
	cmp r2, r1
	blt .L_02001af0_3
	b .L_02001af0_1
.L_02001af0_3:
	ldr r3, [pc, #476]
	str r3, [r0, #12]
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200ce74
	cmp r0, #0
	beq .L_02001af0_4
	b .L_02001af0_1
.L_02001af0_4:
	bl 0x0200ce94
	movs r0, #161
	bl 0x0200cffc
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200ce7c
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r1, #33
	movs r3, #33
	movs r2, #19
	movs r0, #26
	bl 0x0200ce1c
	movs r0, #30
	bl 0x0200ce8c
	movs r0, #239
	bl 0x0200cffc
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200ce54
	movs r0, #20
	bl 0x0200ce8c
	movs r2, #144
	lsls r2, r2, #17
	movs r3, #40
	movs r1, #4
	mov r8, r2
	movs r5, #29
	mov r9, r3
	movs r7, #0
	add r6, sp, #16
	mov r11, r1
.L_02001af0_8:
	mov r2, r10
	ldr r3, [r2, #8]
	ldr r1, [pc, #368]
	adds r3, r3, r1
	str r3, [r2, #8]
	ldr r2, [pc, #368]
	movs r3, #2
	add r8, r2
	str r3, [r6]
	bl 0x0200cdb4
	lsls r2, r0, #1
	adds r2, r2, r0
	lsrs r2, r2, #16
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r2, r3, #4
	adds r3, r3, r2
	ldr r1, [pc, #344]
	lsls r2, r3, #8
	adds r3, r3, r2
	adds r3, r3, r1
	str r3, [r6, #8]
	bl 0x0200cdb4
	lsls r2, r0, #1
	adds r2, r2, r0
	lsrs r2, r2, #16
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r2, r3, #4
	adds r3, r3, r2
	lsls r2, r3, #8
	adds r3, r3, r2
	ldr r2, [pc, #312]
	adds r3, r3, r2
	str r3, [r6, #12]
	bl 0x0200cdb4
	movs r3, #248
	lsls r0, r0, #12
	lsls r3, r3, #8
	lsrs r0, r0, #16
	adds r0, r0, r3
	strh r0, [r6, #34]
	ldr r3, [pc, #296]
	ldr r2, [r3]
	movs r3, #1
	ands r2, r3
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #16
	negs r3, r3
	str r3, [sp, #0]
	movs r3, #138
	lsls r3, r3, #16
	movs r2, #132
	movs r1, #0
	str r3, [sp, #8]
	mov r0, r8
	lsls r2, r2, #18
	movs r3, #0
	str r1, [sp, #4]
	str r6, [sp, #12]
	bl 0x02008118
	cmp r7, #240
	bne .L_02001af0_5
	ldr r2, [pc, #252]
	add r8, r2
.L_02001af0_5:
	mov r3, r9
	cmp r3, #0
	bne .L_02001af0_6
	movs r1, #40
	mov r9, r1
	cmp r7, #240
	bhi .L_02001af0_7
	movs r2, #3
	mov r3, r11
	subs r5, #4
	str r2, [sp, #0]
	str r3, [sp, #4]
	adds r0, r5, #0
	movs r1, #50
	movs r2, #15
	movs r3, #32
	bl 0x0200ce1c
	b .L_02001af0_6
.L_02001af0_7:
	movs r1, #3
	mov r2, r11
	adds r5, #4
	str r1, [sp, #0]
	str r2, [sp, #4]
	adds r0, r5, #0
	movs r1, #45
	movs r2, #9
	movs r3, #32
	bl 0x0200ce1c
.L_02001af0_6:
	movs r0, #1
	bl 0x0200cda4
	movs r3, #1
	ldr r1, [pc, #184]
	negs r3, r3
	adds r7, #1
	add r9, r3
	cmp r7, r1
	bls .L_02001af0_8
	mov r3, r10
	ldr r2, [r3, #8]
	movs r1, #128
	lsls r1, r1, #8
	adds r3, r2, r1
	mov r1, r10
	str r3, [r1, #8]
	cmp r3, #0
	bge .L_02001af0_9
	ldr r1, [pc, #160]
	adds r3, r2, r1
.L_02001af0_9:
	asrs r3, r3, #16
	lsls r3, r3, #16
	mov r2, r10
	str r3, [r2, #8]
	movs r3, #9
	str r3, [sp, #0]
	movs r5, #32
	movs r0, #15
	movs r1, #32
	movs r2, #3
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200ce34
	movs r3, #15
	str r3, [sp, #0]
	movs r1, #32
	movs r3, #1
	movs r2, #3
	movs r0, #12
	str r5, [sp, #4]
	bl 0x0200ce34
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200cffc
	movs r0, #188
	bl 0x0200cffc
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #92]
	bl 0x0200ce54
	bl 0x0200ce5c
	ldr r3, [pc, #84]
	movs r1, #224
	ldr r3, [r3]
	lsls r1, r1, #1
	ldr r2, [pc, #80]
	adds r3, r3, r1
	str r2, [r3]
	movs r0, #11
	bl 0x0200cfa4
	bl 0x0200ce9c
.L_02001af0_1:
	sub sp, #-56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e70
	.4byte 0xfffffecc
	.4byte 0xfffe0000
	.4byte 0x00003333
	.4byte 0xffffcccd
	.4byte 0x0000cccc
	.4byte 0x03001e40
	.4byte 0xffd00000
	.4byte 0x000001df
	.4byte 0x00017fff
	.4byte 0x0000e666
	.4byte 0x03001ebc
	.4byte 0x00000202
	.global Func_02001d48
	.thumb_func
Func_02001d48:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	movs r2, #0
	movs r0, #9
	str r2, [sp, #8]
	bl 0x0200ceac
	adds r6, r0, #0
	movs r0, #0
	bl 0x0200ceac
	mov r11, r0
	bl 0x0200ce94
	movs r3, #43
	str r3, [sp, #4]
	movs r2, #7
	movs r3, #5
	movs r5, #45
	movs r0, #109
	movs r1, #43
	str r5, [sp, #0]
	bl 0x0200ce34
	movs r3, #35
	adds r3, r3, r6
	ldrb r2, [r3]
	mov r10, r3
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_02001d48_0
	movs r3, #46
	str r3, [sp, #0]
	movs r0, #45
	movs r1, #45
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200ce34
	b .L_02001d48_1
.L_02001d48_0:
	ldr r2, [r6, #8]
	ldr r3, [r6, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #48
	movs r1, #42
	movs r2, #1
	movs r3, #1
	bl 0x0200ce34
.L_02001d48_1:
	ldr r3, [r6, #8]
	asrs r3, r3, #20
	mov r9, r3
	cmp r3, #46
	beq .L_02001d48_2
	b .L_02001d48_3
.L_02001d48_2:
	ldr r3, [r6, #16]
	asrs r3, r3, #20
	mov r8, r3
	cmp r3, #45
	beq .L_02001d48_4
	b .L_02001d48_3
.L_02001d48_4:
	ldr r0, [pc, #312]
	bl 0x0200ce74
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02001d48_5
	b .L_02001d48_3
.L_02001d48_5:
	mov r2, r11
	ldr r3, [r2, #16]
	asrs r3, r3, #20
	cmp r3, #45
	bgt .L_02001d48_6
	movs r0, #186
	movs r2, #176
	movs r1, #0
	lsls r0, r0, #18
	lsls r2, r2, #18
	movs r3, #20
	bl 0x02008098
	movs r1, #3
	str r0, [sp, #8]
	movs r0, #0
	bl 0x0200cf64
.L_02001d48_6:
	movs r0, #9
	bl 0x0200ceac
	movs r1, #0
	bl 0x0200ce44
	adds r3, r6, #0
	adds r3, #34
	adds r5, r6, #0
	strb r7, [r3]
	adds r5, #85
	movs r3, #3
	strb r3, [r5]
	ldr r3, [pc, #240]
	mov r2, r8
	str r3, [r6, #72]
	mov r3, r9
	movs r1, #45
	str r7, [r6, #68]
	movs r0, #43
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	bl 0x0200ce34
	adds r0, r6, #0
	bl 0x0200894c
	movs r0, #188
	bl 0x0200cffc
	ldr r3, [pc, #208]
	strb r7, [r5]
	movs r0, #9
	str r3, [r6, #12]
	movs r1, #3
	bl 0x0200cf64
	movs r3, #2
	mov r2, r10
	strb r3, [r2]
	mov r3, r9
	mov r2, r8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #45
	movs r2, #1
	movs r3, #1
	movs r0, #45
	bl 0x0200ce34
	movs r0, #0
	bl 0x0200ceac
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	ldr r0, [sp, #8]
	bl 0x0200cdec
	adds r3, r6, #0
	adds r3, #89
	strb r7, [r3]
	mov r2, r10
	ldrb r3, [r2]
	movs r5, #2
	orrs r3, r5
.L_02001d48_7:
	strb r3, [r2]
	movs r0, #10
	bl 0x0200ceac
	adds r0, #89
	strb r7, [r0]
	movs r0, #10
	bl 0x0200ceac
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #202
	orrs r5, r3
	movs r2, #182
	lsls r2, r2, #18
	strb r5, [r0]
	lsls r1, r1, #18
	movs r0, #10
	bl 0x0200cef4
	ldr r1, [pc, #96]
	movs r0, #10
	bl 0x0200cebc
	movs r0, #10
	bl 0x0200cec4
	movs r0, #30
	bl 0x0200ce8c
	movs r0, #158
	bl 0x0200cffc
	ldr r0, [pc, #72]
	movs r1, #110
	movs r2, #41
	bl 0x0200ce14
	movs r3, #42
	mov r2, r9
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #46
	movs r1, #41
	movs r2, #1
	movs r3, #1
	bl 0x0200ce34
	ldr r0, [pc, #28]
	bl 0x0200ce7c
.L_02001d48_3:
	bl 0x0200ce9c
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
	.4byte 0x00000301
	.4byte 0x00001999
	.4byte 0xfff00000
	.4byte 0x0200d3c4
	.4byte 0x0200dce8
	.global Func_02001f28
	.thumb_func
Func_02001f28:
	push {lr}
	sub sp, #8
	bl 0x0200ce94
	bl 0x02008cc8
	cmp r0, #0
	bne .L_02001f28_0
	movs r3, #45
	movs r2, #43
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #109
	movs r1, #43
	movs r2, #7
	movs r3, #5
	bl 0x0200ce34
	bl 0x02008374
.L_02001f28_0:
	bl 0x0200ce9c
	bl 0x02009d48
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001f60
	.thumb_func
Func_02001f60:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #20
	movs r0, #0
	str r0, [sp, #16]
	bl 0x0200ceac
	str r0, [sp, #12]
	bl 0x0200ce94
	movs r3, #44
	movs r2, #39
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #39
	movs r0, #108
	movs r2, #13
	movs r3, #7
	bl 0x0200ce34
	movs r1, #9
	mov r9, r1
.L_02001f60_22:
	mov r0, r9
	bl 0x0200ceac
	adds r6, r0, #0
	adds r2, r6, #0
	adds r2, #35
	str r2, [sp, #8]
	ldrb r3, [r2]
	cmp r3, #2
	beq .L_02001f60_0
	ldr r2, [r6, #8]
	ldr r3, [r6, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #47
	movs r1, #39
	movs r2, #1
	movs r3, #1
	bl 0x0200ce34
	b .L_02001f60_1
.L_02001f60_0:
	ldr r2, [r6, #8]
	ldr r3, [r6, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #46
	movs r1, #39
	movs r2, #1
	movs r3, #1
	bl 0x0200ce34
.L_02001f60_1:
	ldr r4, [pc, #616]
	movs r5, #0
	ldr r0, [r6, #8]
	ldr r3, [r4, r5]
	asrs r2, r0, #20
	movs r7, #5
	cmp r2, r3
	bne .L_02001f60_2
	ldr r3, [r6, #16]
	ldr r2, [r4, #4]
	asrs r3, r3, #20
	cmp r3, r2
	bne .L_02001f60_2
	ldr r3, [r6, #12]
	cmp r3, #0
	blt .L_02001f60_2
	movs r7, #0
	b .L_02001f60_3
.L_02001f60_2:
	adds r5, #1
	cmp r5, #3
	bhi .L_02001f60_3
	lsls r1, r5, #3
	ldr r3, [r4, r1]
	asrs r2, r0, #20
	cmp r2, r3
	bne .L_02001f60_2
	ldr r3, [r6, #16]
	adds r2, r1, #4
	ldr r2, [r4, r2]
	asrs r3, r3, #20
	cmp r3, r2
	bne .L_02001f60_2
	ldr r3, [r6, #12]
	cmp r3, #0
	blt .L_02001f60_2
	adds r7, r5, #0
.L_02001f60_3:
	cmp r7, #5
	bne .L_02001f60_4
	b .L_02001f60_5
.L_02001f60_4:
	movs r5, #9
	b .L_02001f60_6
.L_02001f60_8:
	adds r5, #1
.L_02001f60_6:
	cmp r5, #11
	bhi .L_02001f60_7
	adds r0, r5, #0
	bl 0x0200ceac
	cmp r9, r5
	beq .L_02001f60_8
	ldr r2, [r6, #8]
	ldr r3, [r0, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02001f60_8
	ldr r2, [r6, #16]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02001f60_8
	movs r7, #5
.L_02001f60_7:
	cmp r7, #5
	bne .L_02001f60_9
	b .L_02001f60_5
.L_02001f60_9:
	ldr r0, [sp, #12]
	ldr r3, [r0, #80]
	ldrb r3, [r3, #9]
	lsls r3, r3, #28
	lsrs r3, r3, #30
	ldr r1, [pc, #476]
	lsls r7, r7, #3
	mov r11, r3
	ldr r2, [r0, #16]
	adds r3, r7, #4
	mov r8, r3
	ldr r3, [r1, r3]
	asrs r2, r2, #20
	mov r10, r1
	cmp r2, r3
	bhi .L_02001f60_10
	ldr r2, [r6, #16]
	ldr r3, [pc, #456]
	ldr r1, [r6, #12]
	adds r2, r2, r3
	ldr r0, [r6, #8]
	movs r3, #20
	bl 0x02008098
	movs r1, #3
	str r0, [sp, #16]
	movs r0, #0
	bl 0x0200cf64
.L_02001f60_10:
	mov r0, r9
	bl 0x0200ceac
	movs r1, #0
	bl 0x0200ce44
	adds r3, r6, #0
	adds r3, #34
	movs r0, #0
	adds r5, r6, #0
	strb r0, [r3]
	adds r5, #85
	movs r3, #3
	strb r3, [r5]
	ldr r3, [pc, #408]
	movs r1, #0
	str r1, [r6, #68]
	str r3, [r6, #72]
	mov r2, r10
	mov r0, r8
	ldr r3, [r2, r7]
	ldr r2, [r2, r0]
	movs r1, #41
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	movs r0, #42
	bl 0x0200ce34
	adds r0, r6, #0
	bl 0x0200894c
	movs r0, #188
	bl 0x0200cffc
	adds r3, r6, #0
	movs r1, #0
	adds r3, #89
	strb r1, [r3]
	ldr r3, [pc, #360]
	strb r1, [r5]
	mov r0, r9
	str r3, [r6, #12]
	movs r1, #3
	bl 0x0200cf64
	ldr r2, [sp, #8]
	movs r3, #2
	strb r3, [r2]
	mov r0, r10
	mov r1, r8
	ldr r2, [r0, r1]
	ldr r3, [r0, r7]
	movs r1, #39
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	movs r0, #46
	bl 0x0200ce34
	movs r0, #0
	mov r1, r11
	bl 0x0200cf64
	movs r0, #0
	bl 0x0200ceac
	adds r0, #35
.L_02001f60_11:
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	ldr r2, [sp, #16]
	cmp r2, #0
	beq .L_02001f60_12
	adds r0, r2, #0
	bl 0x0200cdec
.L_02001f60_12:
	movs r0, #193
	lsls r0, r0, #2
	bl 0x0200ce74
	cmp r0, #0
	beq .L_02001f60_13
	bl 0x0200ce9c
	b .L_02001f60_14
.L_02001f60_13:
	movs r0, #9
	bl 0x0200ceac
	adds r6, r0, #0
	movs r0, #10
	bl 0x0200ceac
	adds r5, r0, #0
	movs r0, #11
	bl 0x0200ceac
	adds r6, #35
	adds r5, #35
	ldrb r2, [r6]
	ldrb r3, [r5]
	adds r0, #35
	ands r3, r2
	ldrb r2, [r0]
	movs r0, #2
	ands r3, r2
	ands r3, r0
	cmp r3, #0
	beq .L_02001f60_5
	movs r5, #222
	movs r6, #170
	lsls r5, r5, #2
	lsls r6, r6, #2
	adds r1, r6, #0
	adds r0, r5, #0
	ldr r2, [pc, #208]
	bl 0x02008c5c
	adds r1, r6, #0
	adds r7, r0, #0
	ldr r2, [pc, #204]
	adds r0, r5, #0
	bl 0x02008c5c
	ldr r3, [r7]
	adds r6, r7, #0
	adds r5, r0, #0
	adds r6, #99
	b .L_02001f60_15
.L_02001f60_20:
	ldrb r3, [r6]
	cmp r3, #0
	bne .L_02001f60_16
	adds r3, r5, #0
	adds r3, #99
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_02001f60_17
.L_02001f60_16:
	movs r0, #30
	bl 0x0200ce8c
	movs r0, #158
	bl 0x0200cffc
	ldr r0, [pc, #160]
	movs r1, #109
	movs r2, #37
	bl 0x0200ce14
	movs r3, #45
	movs r2, #38
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	movs r0, #45
	movs r1, #37
	bl 0x0200ce34
	movs r0, #9
	bl 0x0200ceac
	ldr r5, [pc, #100]
	ldr r2, [r0, #8]
	ldr r3, [r5]
	asrs r2, r2, #20
	cmp r2, r3
	bne .L_02001f60_18
	movs r0, #9
	bl 0x0200ceac
	ldr r3, [r0, #16]
	ldr r2, [r5, #4]
	asrs r3, r3, #20
	cmp r3, r2
	bne .L_02001f60_18
	ldr r0, [pc, #100]
	bl 0x0200ce7c
	b .L_02001f60_19
.L_02001f60_18:
	ldr r0, [pc, #96]
	bl 0x0200ce7c
.L_02001f60_19:
	movs r0, #193
	lsls r0, r0, #2
	bl 0x0200ce7c
	b .L_02001f60_5
.L_02001f60_17:
	movs r0, #1
	bl 0x0200cda4
	ldr r3, [r7]
.L_02001f60_15:
	cmp r3, #0
	bne .L_02001f60_20
	ldr r3, [r5]
	cmp r3, #0
	bne .L_02001f60_20
.L_02001f60_5:
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #11
	bhi .L_02001f60_21
	b .L_02001f60_22
.L_02001f60_21:
	bl 0x0200ce9c
.L_02001f60_14:
	sub sp, #-20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200d128
	.4byte 0xfffc0000
	.4byte 0x00001999
	.4byte 0xfff00000
	.4byte 0x0200d488
	.4byte 0x0200d508
	.4byte 0x0200dd12
	.4byte 0x00000302
	.4byte 0x00000303
.L_0200226c:
	.global Func_0200226c
	.thumb_func
Func_0200226c:
	push {lr}
	sub sp, #8
	bl 0x0200ce94
	bl 0x02008cc8
	cmp r0, #0
	bne .L_0200226c_0
	movs r3, #44
	movs r2, #39
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #108
	movs r1, #39
	movs r2, #13
	movs r3, #7
	bl 0x0200ce34
	bl 0x02008374
.L_0200226c_0:
	bl 0x0200ce9c
	bl 0x02009f60
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020022a4
	.thumb_func
Func_020022a4:
	push {r5, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #100
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl 0x0200ceac
	movs r2, #128
	ldr r3, [r0, #12]
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r5, #12]
	movs r0, #0
	pop {r5}
.L_020022c2:
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_020022c8
	.thumb_func
Func_020022c8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r9, r0
	movs r0, #8
	sub sp, #8
	bl 0x0200ceac
	adds r7, r0, #0
	movs r0, #9
	bl 0x0200ceac
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
	lsls r2, r2, #7
	mov r10, r0
	movs r0, #8
	bl 0x0200ceb4
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200ceb4
	mov r0, r9
	cmp r0, #0
	beq .L_020022c8_0
	movs r0, #180
	bl 0x0200cffc
.L_020022c8_0:
	movs r1, #100
	adds r1, r1, r7
	movs r2, #0
	ldrsh r3, [r1, r2]
	ldr r5, [pc, #176]
	lsls r3, r3, #2
	mov r6, r10
	ldr r2, [r5, r3]
	adds r0, r7, #0
	mov r8, r1
	ldr r3, [r7, #16]
	ldr r1, [r7, #8]
	adds r6, #100
	bl 0x0200ce04
	movs r0, #0
	ldrsh r3, [r6, r0]
	mov r2, r10
	mov r0, r10
	lsls r3, r3, #2
	ldr r1, [r2, #8]
	ldr r2, [r5, r3]
	ldr r3, [r0, #16]
	bl 0x0200ce04
	movs r0, #8
	bl 0x0200ceec
	movs r0, #9
	bl 0x0200ceec
	mov r2, r8
	movs r1, #0
	ldrsh r3, [r2, r1]
	lsls r3, r3, #2
	ldr r3, [r5, r3]
	str r3, [r7, #12]
	movs r0, #0
	ldrsh r3, [r6, r0]
	lsls r3, r3, #2
	ldr r3, [r5, r3]
	mov r1, r10
	mov r2, r9
	str r3, [r1, #12]
	cmp r2, #0
	beq .L_020022c8_1
	ldr r0, [pc, #96]
	bl 0x0200cffc
.L_020022c8_1:
	movs r5, #0
.L_020022c8_4:
	adds r0, r5, #0
	adds r0, #8
	bl 0x0200ceac
	ldr r3, [r0, #12]
	adds r2, r3, #0
	cmp r3, #0
	bge .L_020022c8_2
	ldr r1, [pc, #76]
	adds r2, r3, r1
.L_020022c8_2:
	asrs r2, r2, #16
	cmp r2, #0
	bge .L_020022c8_3
	movs r3, #30
	negs r3, r3
	cmp r2, r3
	ble .L_020022c8_3
	ldr r2, [r0, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #4
	movs r1, #19
	movs r2, #1
	movs r3, #1
	bl 0x0200ce34
.L_020022c8_3:
	adds r5, #1
	cmp r5, #4
	bls .L_020022c8_4
	mov r0, r9
	bl 0x0200ce8c
	sub sp, #-8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200d148
	.4byte 0x00000121
	.4byte 0x0000ffff
	.global Func_020023d4
	.thumb_func
Func_020023d4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r2, [pc, #152]
	sub sp, #4
	mov r8, r0
	mov r10, r2
	movs r7, #0
	movs r1, #0
	adds r6, r1, #0
	adds r6, #8
	cmp r6, r8
	beq 0x0200a430
	adds r0, r6, #0
	str r1, [sp, #0]
	bl 0x0200ceac
	adds r5, r0, #0
	mov r0, r8
	bl 0x0200ceac
.L_02002400:
	adds r7, r0, #0
	ldr r2, [r5, #8]
	ldr r3, [r7, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	ldr r1, [sp, #0]
	cmp r2, r3
	bne 0x0200a430
	ldr r2, [r5, #16]
	ldr r3, [r7, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne 0x0200a430
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #13
.L_02002422:
	adds r0, r3, r2
	cmp r10, r0
	bgt .L_02002422_0
	adds r3, r7, #0
	adds r3, #100
	strh r6, [r3]
	mov r10, r0
.L_02002422_0:
	adds r1, #1
	cmp r1, #5
	bls 0x0200a3e8
	movs r1, #128
	movs r2, #128
	mov r0, r8
	lsls r1, r1, #11
	lsls r2, r2, #10
	bl 0x0200ceb4
	ldr r1, [r7, #8]
	ldr r3, [r7, #16]
	mov r2, r10
	adds r0, r7, #0
	bl 0x0200ce04
	mov r0, r8
	bl 0x0200ceec
	movs r0, #188
	bl 0x0200cffc
	mov r0, r8
	bl 0x02008b08
	movs r0, #30
	bl 0x0200ce8c
	sub sp, #-4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffb0
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #132
	movs	r2, #0
	str	r2, [sp, #16]
	str	r2, [sp, #12]
	bl 0x0200ce94
	mov	r2, sp
	movs	r3, #0
	adds	r2, #20
	mov	r8, r3
	str	r2, [sp, #8]
	movs	r3, #10
	mov	fp, r3
.L_020024a4:
	mov	r0, fp
	bl 0x0200ceac
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	asrs	r3, r3, #20
	mov	r9, r3
	cmp	r3, #13
	bne.n	.L_0200250a
	ldr	r3, [r6, #16]
	asrs	r3, r3, #20
	mov	sl, r3
	cmp	r3, #7
	bne.n	.L_0200250a
	movs	r5, #128
	lsls	r5, r5, #2
	add	r5, r8
	adds	r0, r5, #0
	bl 0x0200ce74
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_0200250a
	adds	r0, r6, #0
	bl 0x0200894c
	adds	r0, r5, #0
	bl 0x0200ce7c
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #2
	orrs	r2, r3
	adds	r3, r6, #0
	adds	r3, #89
	strb	r2, [r1, #0]
	strb	r7, [r3, #0]
	subs	r3, #4
	strb	r7, [r3, #0]
	mov	r2, r9
	mov	r3, sl
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #4
	movs	r1, #19
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ce34
	b.n	.L_020026ca
.L_0200250a:
	ldr	r3, [r6, #80]
	ldrb	r2, [r3, #9]
	movs	r3, #12
	ands	r3, r2
	cmp	r3, #12
	bne.n	.L_020025b0
	movs	r7, #128
	lsls	r7, r7, #2
	add	r7, r8
	adds	r0, r7, #0
	bl 0x0200ce74
	cmp	r0, #0
	bne.n	.L_020025b0
	mov	r0, fp
	movs	r1, #1
	bl 0x0200cf64
	ldr	r3, [r6, #16]
	movs	r5, #0
	asrs	r3, r3, #20
	str	r5, [r6, #68]
	cmp	r3, #12
	bgt.n	.L_0200255a
	movs	r2, #224
	movs	r1, #0
	lsls	r2, r2, #16
	movs	r3, #253
	ldr	r0, [r6, #8]
	bl 0x02008058
	str	r0, [sp, #16]
	movs	r2, #240
	ldr	r0, [r6, #8]
	movs	r1, #0
	lsls	r2, r2, #16
	movs	r3, #253
	bl 0x02008058
	str	r0, [sp, #12]
.L_0200255a:
	adds	r0, r6, #0
	bl 0x0200894c
	movs	r1, #0
	movs	r2, #0
	mov	r0, fp
	bl 0x0200cef4
	ldr	r0, [sp, #16]
	bl 0x0200cdec
	ldr	r0, [sp, #12]
	bl 0x0200cdec
	adds	r0, r7, #0
	bl 0x0200ce7c
	b.n	.L_020026ca
.L_0200257e:
	adds	r0, r5, #0
	adds	r0, #10
	bl 0x0200ceac
	ldr	r3, [r6, #8]
	ldr	r2, [sp, #8]
	str	r3, [r2, #8]
	ldr	r3, [r6, #12]
	str	r3, [r2, #12]
	ldr	r3, [r6, #16]
	str	r3, [r2, #16]
	ldr	r3, [r0, #8]
	str	r3, [r6, #8]
	ldr	r3, [r0, #12]
	str	r3, [r6, #12]
	ldr	r3, [r0, #16]
.L_0200259e:
	str	r3, [r6, #16]
	ldr	r3, [r2, #8]
	str	r3, [r0, #8]
	ldr	r3, [r2, #12]
	str	r3, [r0, #12]
	ldr	r3, [r2, #16]
	adds	r7, r5, #0
	str	r3, [r0, #16]
.L_020025ae:
	b.n	.L_020025f8
.L_020025b0:
	ldr	r3, [r6, #16]
	asrs	r3, r3, #20
	cmp	r3, #19
	beq.n	.L_020025ba
	b.n	.L_020026bc
.L_020025ba:
	movs	r0, #128
	lsls	r0, r0, #2
	add	r0, r8
	bl 0x0200ce74
	cmp	r0, #0
	bne.n	.L_020026bc
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r6, #60]
	adds	r3, r6, #0
	adds	r3, #85
	str	r0, [r6, #20]
	str	r0, [r6, #40]
	movs	r5, #0
	strb	r0, [r3, #0]
	adds	r3, #15
	strh	r0, [r3, #0]
	mov	r7, r8
	cmp	r5, r8
	bge.n	.L_020025f8
.L_020025e4:
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r0, r5, r3
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_0200257e
	adds	r5, #1
	cmp	r5, r8
	blt.n	.L_020025e4
.L_020025f8:
	adds	r5, r7, #0
	adds	r5, #10
	adds	r0, r5, #0
	bl 0x0200ceac
	movs	r3, #128
	lsls	r3, r3, #24
.L_02002606:
	str	r3, [r0, #60]
	adds	r3, r0, #0
	movs	r2, #0
	adds	r3, #85
	str	r2, [r0, #20]
	str	r2, [r0, #40]
	movs	r1, #192
	strb	r2, [r3, #0]
	movs	r0, #192
	adds	r3, #15
	strh	r2, [r3, #0]
	lsls	r1, r1, #7
	lsls	r0, r0, #10
	bl 0x0200cf7c
	bl 0x0200cf9c
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #128
	movs	r0, #136
	movs	r2, #172
	movs	r3, #1
	lsls	r1, r1, #12
	lsls	r2, r2, #17
	lsls	r0, r0, #16
	bl 0x0200cf84
	bl 0x0200cf8c
	adds	r0, r5, #0
	.2byte 0xf7ff
	.2byte 0xfec5
	adds	r0, r5, #0
	bl 0x0200ceac
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, #6
	bne.n	.L_02002674
	movs	r0, #8
	bl 0x0200ceac
	adds	r0, #100
	ldrh	r3, [r0, #0]
	adds	r3, #1
	strh	r3, [r0, #0]
	movs	r0, #9
	bl 0x0200ceac
	adds	r0, #100
	ldrh	r3, [r0, #0]
	subs	r3, #1
	b.n	.L_0200268e
.L_02002674:
	movs	r0, #8
	bl 0x0200ceac
	adds	r0, #100
	ldrh	r3, [r0, #0]
	subs	r3, #1
	strh	r3, [r0, #0]
	movs	r0, #9
	bl 0x0200ceac
	adds	r0, #100
	ldrh	r3, [r0, #0]
	adds	r3, #1
.L_0200268e:
	strh	r3, [r0, #0]
	adds	r0, r5, #0
	bl 0x0200ceac
	ldr	r3, [pc, #72]
	str	r3, [r0, #108]
	movs	r0, #40
	bl 0x0200a2c8
	adds	r0, r5, #0
	bl 0x0200ceac
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	movs	r2, #128
	lsls	r2, r2, #2
	strb	r3, [r0, #0]
	adds	r0, r7, r2
	bl 0x0200ce7c
	b.n	.L_020026ca
.L_020026bc:
	movs	r3, #1
	add	r8, r3
.L_020026c0:
	mov	r2, r8
	add	fp, r3
	cmp	r2, #3
	bgt.n	.L_020026ca
	b.n	.L_020024a4
.L_020026ca:
	bl 0x0200ce9c
	add	sp, #132
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0xa2a5
	.2byte 0x0200
	.global Func_020026e4
	.thumb_func
Func_020026e4:
	push {lr}
	bl 0x0200ce94
	bl 0x02008374
	bl 0x0200ce9c
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020026f8
	.thumb_func
Func_020026f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #464]
	movs r1, #178
	ldr r3, [r3]
	lsls r1, r1, #1
	movs r0, #0
	adds r7, r3, r1
	sub sp, #56
	bl 0x0200ceac
	ldr r1, [pc, #448]
	movs r2, #10
	ldrsh r6, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r3, #0
	str r3, [r0, #12]
	adds r3, r6, r1
	cmp r3, #7
	bls .L_020026f8_0
	b .L_020026f8_1
.L_020026f8_0:
	movs r3, #162
	lsls r3, r3, #1
	cmp r2, r3
	bge .L_020026f8_2
	b .L_020026f8_1
.L_020026f8_2:
	movs r1, #166
	lsls r1, r1, #1
	cmp r2, r1
	blt .L_020026f8_3
	b .L_020026f8_1
.L_020026f8_3:
	ldr r3, [pc, #412]
	str r3, [r0, #12]
	ldr r0, [pc, #412]
	bl 0x0200ce74
	cmp r0, #0
	beq .L_020026f8_4
	b .L_020026f8_1
.L_020026f8_4:
	bl 0x0200ce94
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r1, #29
	movs r2, #33
	movs r3, #20
	movs r0, #63
	bl 0x0200ce1c
	movs r0, #161
	bl 0x0200cffc
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r1, #83
	movs r3, #80
	movs r2, #44
	movs r0, #44
	bl 0x0200ce1c
	movs r0, #30
	bl 0x0200ce8c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200ce54
	movs r0, #239
	bl 0x0200cffc
	movs r0, #20
	bl 0x0200ce8c
	movs r2, #154
	ldr r1, [pc, #320]
	lsls r2, r2, #18
	movs r6, #0
	movs r3, #60
	mov r9, r2
	mov r10, r3
	mov r8, r6
	add r5, sp, #16
	mov r11, r1
.L_020026f8_6:
	ldr r3, [r7, #8]
	ldr r2, [pc, #304]
	adds r3, r3, r2
	str r3, [r7, #8]
	ldr r3, [pc, #304]
	cmp r9, r3
	ble .L_020026f8_5
	mov r1, r8
	cmp r1, #40
	bls .L_020026f8_5
	ldr r2, [pc, #296]
	movs r3, #2
	add r9, r2
	str r3, [r5]
	bl 0x0200cdb4
	lsls r2, r0, #1
	adds r2, r2, r0
	lsrs r2, r2, #16
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r2, r3, #4
	adds r3, r3, r2
	lsls r2, r3, #8
	adds r3, r3, r2
	add r3, r11
	str r3, [r5, #8]
	bl 0x0200cdb4
	lsls r2, r0, #1
	adds r2, r2, r0
	lsrs r2, r2, #16
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r2, r3, #4
	adds r3, r3, r2
	lsls r2, r3, #8
	adds r3, r3, r2
	add r3, r11
	str r3, [r5, #12]
	bl 0x0200cdb4
	movs r3, #248
	lsls r0, r0, #12
	lsls r3, r3, #8
	lsrs r0, r0, #16
	adds r0, r0, r3
	strh r0, [r5, #34]
	ldr r3, [pc, #224]
	ldr r2, [r3]
	movs r3, #1
	ands r2, r3
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #16
	negs r3, r3
	str r3, [sp, #0]
	movs r3, #138
	lsls r3, r3, #16
	movs r2, #144
	movs r1, #0
	str r3, [sp, #8]
	lsls r2, r2, #17
	mov r0, r9
	movs r3, #0
	str r1, [sp, #4]
	str r5, [sp, #12]
	bl 0x02008118
	mov r2, r10
	cmp r2, #0
	bne .L_020026f8_5
	movs r3, #40
	movs r2, #4
	mov r10, r3
	adds r6, #4
	movs r3, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r0, r6, #0
	movs r1, #56
	movs r2, #36
	movs r3, #17
	bl 0x0200ce1c
.L_020026f8_5:
	movs r0, #1
	bl 0x0200cda4
	movs r1, #1
	movs r2, #1
	ldr r3, [pc, #144]
	negs r2, r2
	add r8, r1
	add r10, r2
	cmp r8, r3
	bls .L_020026f8_6
	ldr r2, [r7, #8]
	movs r1, #128
	lsls r1, r1, #8
	adds r3, r2, r1
	str r3, [r7, #8]
	cmp r3, #0
	bge .L_020026f8_7
	ldr r1, [pc, #120]
	adds r3, r2, r1
.L_020026f8_7:
	asrs r3, r3, #16
	lsls r3, r3, #16
	movs r0, #144
	str r3, [r7, #8]
	lsls r0, r0, #1
	bl 0x0200cffc
	movs r0, #188
	bl 0x0200cffc
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #92]
	bl 0x0200ce54
	bl 0x0200ce5c
	ldr r3, [pc, #88]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #66
	str r2, [r3]
	movs r0, #18
	bl 0x0200cfa4
	bl 0x0200ce9c
.L_020026f8_1:
	sub sp, #-56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001e70
	.4byte 0xfffffdec
	.4byte 0xfffe0000
	.4byte 0x00000306
	.4byte 0x0000cccc
	.4byte 0x00003333
	.4byte 0x023fffff
	.4byte 0xffffcccd
	.4byte 0x03001e40
	.4byte 0x0000013f
	.4byte 0x00017fff
	.4byte 0x0000e666
	.4byte 0x03001ebc
	.global Func_0200290c
	.thumb_func
Func_0200290c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #448]
	movs r1, #178
	ldr r3, [r3]
	lsls r1, r1, #1
	movs r0, #0
	adds r7, r3, r1
	sub sp, #56
	bl 0x0200ceac
	ldr r1, [pc, #432]
	movs r2, #10
	ldrsh r6, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r3, #0
	str r3, [r0, #12]
	adds r3, r6, r1
	cmp r3, #7
	bls .L_0200290c_0
	b 0x0200aaca
.L_0200290c_0:
	movs r3, #162
	lsls r3, r3, #1
	cmp r2, r3
	bge .L_0200290c_1
	b 0x0200aaca
.L_0200290c_1:
	movs r1, #166
	lsls r1, r1, #1
	cmp r2, r1
	blt 0x0200a956
	b 0x0200aaca
.L_02002956:
	ldr r3, [pc, #396]
	str r3, [r0, #12]
	ldr r0, [pc, #396]
	bl 0x0200ce74
	cmp r0, #0
	beq .L_02002956_0
	b 0x0200aaca
.L_02002956_0:
	bl 0x0200ce94
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r1, #29
	movs r3, #20
	movs r2, #49
	movs r0, #63
	bl 0x0200ce1c
	movs r0, #161
	bl 0x0200cffc
	movs r0, #30
	bl 0x0200ce8c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200ce54
	movs r0, #239
	bl 0x0200cffc
	movs r0, #20
	bl 0x0200ce8c
	movs r2, #176
	lsls r2, r2, #18
	mov r10, r2
	ldr r2, [pc, #320]
	movs r3, #60
	movs r1, #0
	movs r6, #61
	mov r8, r3
	mov r9, r1
	add r5, sp, #16
	mov r11, r2
	ldr r3, [r7, #8]
	ldr r1, [pc, #304]
	ldr r2, [pc, #308]
	adds r3, r3, r1
	str r3, [r7, #8]
	ldr r3, [pc, #304]
	add r10, r2
	ldr r1, [pc, #304]
	add r3, r10
	cmp r3, r1
	bhi 0x0200aa64
	movs r3, #2
	str r3, [r5]
	bl 0x0200cdb4
	lsls r2, r0, #1
	adds r2, r2, r0
	lsrs r2, r2, #16
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r2, r3, #4
	adds r3, r3, r2
	lsls r2, r3, #8
	adds r3, r3, r2
	add r3, r11
	str r3, [r5, #8]
	bl 0x0200cdb4
	lsls r2, r0, #1
	adds r2, r2, r0
	lsrs r2, r2, #16
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r2, r3, #4
	adds r3, r3, r2
	lsls r2, r3, #8
	adds r3, r3, r2
	add r3, r11
	str r3, [r5, #12]
	bl 0x0200cdb4
	movs r2, #248
	lsls r0, r0, #12
	lsls r2, r2, #8
	lsrs r0, r0, #16
	adds r0, r0, r2
	strh r0, [r5, #34]
	ldr r3, [pc, #228]
	ldr r2, [r3]
	movs r3, #1
	ands r2, r3
	lsls r3, r2, #1
	adds r3, r3, r2
.L_02002a24:
	lsls r3, r3, #16
	negs r3, r3
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #138
	lsls r3, r3, #16
	movs r2, #144
	str r3, [sp, #8]
	movs r1, #0
	mov r0, r10
	lsls r2, r2, #17
	movs r3, #0
	str r5, [sp, #12]
	bl 0x02008118
	mov r1, r8
	cmp r1, #0
	bne .L_02002a24_0
	movs r2, #40
	movs r3, #3
	mov r8, r2
	subs r6, #4
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r0, r6, #0
	movs r1, #56
	movs r2, #44
	movs r3, #17
	bl 0x0200ce1c
.L_02002a24_0:
	movs r0, #1
	bl 0x0200cda4
	movs r3, #1
	movs r1, #1
	ldr r2, [pc, #148]
	negs r1, r1
	add r9, r3
	add r8, r1
	cmp r9, r2
	bls 0x0200a9ba
	ldr r2, [r7, #8]
	movs r1, #128
	lsls r1, r1, #8
	adds r3, r2, r1
	str r3, [r7, #8]
	cmp r3, #0
	bge .L_02002a24_1
	ldr r1, [pc, #124]
	adds r3, r2, r1
.L_02002a24_1:
	asrs r3, r3, #16
	lsls r3, r3, #16
	movs r0, #144
	str r3, [r7, #8]
	lsls r0, r0, #1
	bl 0x0200cffc
	movs r0, #188
	bl 0x0200cffc
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #96]
	bl 0x0200ce54
	bl 0x0200ce5c
	ldr r3, [pc, #92]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #66
	str r2, [r3]
	movs r0, #19
	bl 0x0200cfa4
	bl 0x0200ce9c
	sub sp, #-56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x1e70
	.2byte 0x0300
	.2byte 0xfcec
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0xfffe
	.2byte 0x0307
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0xcccd
	.2byte 0xffff
	.2byte 0x3333
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfd38
	.2byte 0xffff
	.2byte 0x0027
	.2byte 0x1e40
	.2byte 0x0300
	.4byte 0x0000013f
	.4byte 0x00017fff
	.4byte 0x0000e666
	.4byte 0x03001ebc
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	movs	r1, #0
	movs	r0, #0
	str	r1, [sp, #12]
	bl 0x0200ceac
	str	r0, [sp, #8]
	bl 0x0200ce94
	movs	r3, #5
	movs	r2, #48
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #69
	movs	r1, #48
	movs	r2, #4
	movs	r3, #2
	bl 0x0200ce34
	movs	r3, #9
	movs	r2, #37
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #73
	movs	r2, #9
	movs	r1, #37
	movs	r3, #13
	bl 0x0200ce34
	movs	r2, #15
	mov	sl, r2
.L_02002b60:
	mov	r0, sl
	bl 0x0200ceac
	movs	r3, #35
	mov	r8, r0
	add	r3, r8
	mov	fp, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #2
	beq.n	.L_02002b8e
	ldr	r2, [r0, #8]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #72
	movs	r1, #48
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ce34
	b.n	.L_02002ba8
.L_02002b8e:
	mov	r1, r8
	ldr	r2, [r1, #8]
	ldr	r3, [r1, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #73
	movs	r1, #48
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ce34
.L_02002ba8:
	mov	r2, r8
	ldr	r4, [pc, #784]
	movs	r6, #0
	ldr	r0, [r2, #8]
	ldr	r3, [r4, r6]
	asrs	r2, r0, #20
	movs	r5, #8
	cmp	r2, r3
	bne.n	.L_02002bd0
	mov	r1, r8
	ldr	r3, [r1, #16]
	ldr	r2, [r4, #4]
	asrs	r3, r3, #20
	cmp	r3, r2
	bne.n	.L_02002bd0
	ldr	r3, [r1, #12]
	cmp	r3, #0
	blt.n	.L_02002bd0
	movs	r5, #0
	b.n	.L_02002bf8
.L_02002bd0:
	adds	r6, #1
	cmp	r6, #7
	bhi.n	.L_02002bf8
	lsls	r1, r6, #3
	ldr	r3, [r4, r1]
	asrs	r2, r0, #20
	cmp	r2, r3
	bne.n	.L_02002bd0
	mov	r2, r8
	ldr	r3, [r2, #16]
	adds	r2, r1, #4
	ldr	r2, [r4, r2]
	asrs	r3, r3, #20
	cmp	r3, r2
	bne.n	.L_02002bd0
	mov	r1, r8
	ldr	r3, [r1, #12]
	cmp	r3, #0
	blt.n	.L_02002bd0
	adds	r5, r6, #0
.L_02002bf8:
	cmp	r5, #8
.L_02002bfa:
	bne.n	.L_02002bfe
	b.n	.L_02002e98
.L_02002bfe:
	movs	r6, #15
	b.n	.L_02002c04
.L_02002c02:
	adds	r6, #1
.L_02002c04:
	cmp	r6, #18
	bhi.n	.L_02002c30
	adds	r0, r6, #0
	bl 0x0200ceac
	cmp	sl, r6
	beq.n	.L_02002c02
	mov	r3, r8
	ldr	r2, [r3, #8]
	ldr	r3, [r0, #8]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02002c02
	mov	r1, r8
	ldr	r2, [r1, #16]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02002c02
	movs	r5, #8
.L_02002c30:
	cmp	r5, #8
	bne.n	.L_02002c36
	b.n	.L_02002e98
.L_02002c36:
	ldr	r2, [sp, #8]
	ldr	r3, [r2, #80]
	ldrb	r3, [r3, #9]
	lsls	r3, r3, #28
	lsrs	r3, r3, #30
	lsls	r7, r5, #3
	ldr	r1, [pc, #632]
	mov	r9, r3
	ldr	r2, [r2, #16]
	adds	r3, r7, #4
	ldr	r3, [r1, r3]
	asrs	r2, r2, #20
	cmp	r2, r3
	bhi.n	.L_02002c6e
	mov	r2, r8
	ldr	r1, [r2, #12]
	ldr	r0, [r2, #8]
	ldr	r3, [pc, #612]
	ldr	r2, [r2, #16]
	adds	r2, r2, r3
	movs	r3, #20
	bl 0x02008098
	movs	r1, #3
	str	r0, [sp, #12]
	movs	r0, #0
	bl 0x0200cf64
.L_02002c6e:
	movs	r6, #15
.L_02002c70:
	adds	r0, r6, #0
	bl 0x0200ceac
	cmp	sl, r6
	beq.n	.L_02002c9e
	mov	r1, r8
	ldr	r2, [r1, #8]
	ldr	r3, [r0, #8]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02002c9e
	ldr	r2, [r1, #16]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	subs	r2, #1
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02002c9e
	adds	r0, r6, #0
	movs	r1, #3
	bl 0x0200cf64
.L_02002c9e:
	adds	r6, #1
	cmp	r6, #18
	bls.n	.L_02002c70
	mov	r0, sl
	bl 0x0200ceac
	movs	r1, #0
	bl 0x0200ce44
	mov	r3, r8
	adds	r3, #34
	movs	r2, #0
	mov	r6, r8
	strb	r2, [r3, #0]
	adds	r6, #85
	movs	r3, #3
	strb	r3, [r6, #0]
	ldr	r3, [pc, #512]
	mov	r1, r8
	movs	r2, #0
	str	r3, [r1, #72]
	str	r2, [r1, #68]
	ldr	r1, [pc, #496]
	adds	r5, r7, #4
	ldr	r3, [r1, r7]
	ldr	r2, [r1, r5]
	movs	r0, #6
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #44
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ce34
	mov	r0, r8
	bl 0x0200894c
	movs	r0, #188
	bl 0x0200cffc
	mov	r3, r8
	movs	r2, #0
	adds	r3, #89
	strb	r2, [r3, #0]
	ldr	r3, [pc, #464]
	mov	r1, r8
.L_02002cfa:
	strb	r2, [r6, #0]
	mov	r0, sl
	str	r3, [r1, #12]
	movs	r1, #3
	bl 0x0200cf64
	movs	r3, #2
	mov	r2, fp
	strb	r3, [r2, #0]
	ldr	r1, [pc, #428]
	ldr	r3, [r1, r7]
	ldr	r2, [r1, r5]
	movs	r0, #73
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r2, #1
	movs	r1, #48
	bl 0x0200ce34
	movs	r0, #0
	mov	r1, r9
	bl 0x0200cf64
	movs	r0, #0
	bl 0x0200ceac
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r6, #15
.L_02002d3c:
	adds	r0, r6, #0
	bl 0x0200ceac
	cmp	sl, r6
	beq.n	.L_02002d7c
	mov	r3, r8
	ldr	r2, [r3, #8]
	ldr	r3, [r0, #8]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02002d7c
	mov	r1, r8
	ldr	r2, [r1, #16]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	subs	r2, #1
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02002d7c
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200cf64
	adds	r0, r6, #0
	bl 0x0200ceac
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
.L_02002d7c:
	adds	r6, #1
	cmp	r6, #18
	bls.n	.L_02002d3c
	ldr	r0, [sp, #12]
	bl 0x0200cdec
	movs	r0, #194
	lsls	r0, r0, #2
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_02002d9a
	bl 0x0200ce9c
	b.n	.L_02002ea8
.L_02002d9a:
	movs	r0, #15
	bl 0x0200ceac
	mov	r8, r0
	movs	r0, #16
	bl 0x0200ceac
	adds	r5, r0, #0
	movs	r0, #17
	bl 0x0200ceac
	adds	r6, r0, #0
	movs	r0, #18
	bl 0x0200ceac
	movs	r2, #35
	add	r8, r2
	mov	r3, r8
	adds	r5, #35
	ldrb	r2, [r3, #0]
	ldrb	r3, [r5, #0]
	adds	r6, #35
	ands	r3, r2
	ldrb	r2, [r6, #0]
	adds	r0, #35
	ands	r3, r2
	ldrb	r2, [r0, #0]
	ands	r3, r2
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02002e98
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200cf7c
	movs	r0, #14
	movs	r1, #1
	bl 0x0200cf94
	bl 0x0200cf8c
	movs	r1, #194
	ldr	r2, [pc, #212]
	lsls	r1, r1, #2
	movs	r0, #136
	bl 0x02008c5c
	adds	r6, r0, #0
	movs	r0, #30
	bl 0x0200ce8c
	ldr	r0, [pc, #200]
	ldr	r1, [pc, #200]
	bl 0x0200cf7c
	movs	r0, #216
	movs	r1, #1
	movs	r2, #158
	movs	r3, #1
	lsls	r2, r2, #18
	negs	r1, r1
	lsls	r0, r0, #16
	bl 0x0200cf84
	adds	r0, r6, #0
.L_02002e22:
	bl 0x0200cdfc
	adds	r0, r6, #0
	ldr	r1, [pc, #172]
	bl 0x0200cddc
	movs	r1, #190
	lsls	r1, r1, #2
	movs	r0, #216
	ldr	r2, [pc, #164]
	.2byte 0xf7fd
	.2byte 0xff11
	movs	r1, #99
	ldr	r3, [r6, #0]
	adds	r1, r1, r6
	adds	r5, r0, #0
	mov	r8, r1
	b.n	.L_02002e8e
.L_02002e46:
	mov	r1, r8
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_02002e58
	adds	r3, r5, #0
	adds	r3, #99
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02002e86
.L_02002e58:
	movs	r0, #30
	bl 0x0200ce8c
	ldr	r0, [pc, #128]
	movs	r1, #77
	movs	r2, #35
	bl 0x0200ce14
	movs	r3, #13
	movs	r2, #36
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #13
	movs	r1, #35
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ce34
	movs	r0, #194
	lsls	r0, r0, #2
	bl 0x0200ce7c
	b.n	.L_02002e98
.L_02002e86:
	movs	r0, #1
	bl 0x0200cda4
	ldr	r3, [r6, #0]
.L_02002e8e:
	cmp	r3, #0
	bne.n	.L_02002e46
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_02002e46
.L_02002e98:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #18
	bhi.n	.L_02002ea4
	b.n	.L_02002b60
.L_02002ea4:
	bl 0x0200ce9c
.L_02002ea8:
	add	sp, #16
.L_02002eaa:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x0200d164
	.4byte 0xfffc0000
	.4byte 0x00001999
	.4byte 0xfff00000
	.4byte 0x0200d77c
	.4byte 0x00006666
	.4byte 0x00000ccc
	.4byte 0x0200d7c8
	.4byte 0x0200dac8
	.2byte 0xdd3c
	.2byte 0x0200
	.global Func_02002ee4
	.thumb_func
Func_02002ee4:
	push {lr}
	sub sp, #8
	bl 0x0200ce94
	bl 0x02008cc8
	cmp r0, #0
	bne .L_02002ee4_0
	movs r3, #5
	movs r2, #48
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #69
	movs r1, #48
	movs r2, #4
	movs r3, #2
	bl 0x0200ce34
	movs r3, #9
	movs r2, #37
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #73
	movs r1, #37
	movs r2, #9
	movs r3, #13
	bl 0x0200ce34
	bl 0x02008374
.L_02002ee4_0:
	bl 0x0200ce9c
	bl 0x0200ab14
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002f30
	.thumb_func
Func_02002f30:
	push {lr}
	bl 0x0200ce94
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #8
.L_02002f3e:
	lsls r2, r2, #7
	bl 0x0200ceb4
	movs r1, #130
	movs r2, #178
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200cedc
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200cf5c
	movs r0, #130
	movs r2, #196
	movs r3, #223
	lsls r2, r2, #18
	movs r1, #0
.L_02002f68:
	lsls r0, r0, #18
	bl 0x02008058
	movs r1, #6
	movs r2, #0
	movs r0, #0
	bl 0x0200cf1c
	movs r0, #60
	bl 0x0200ce8c
	movs r0, #20
	bl 0x0200cfa4
	bl 0x0200ce9c
	pop {r0}
	bx r0
	.global Func_02002f8c
	.thumb_func
Func_02002f8c:
	push {lr}
	ldr r3, [pc, #72]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #64]
	cmp r2, r3
	bne .L_02002f8c_0
	ldr r0, [pc, #60]
	b 0x0200afd4
.L_02002f8c_0:
	ldr r3, [pc, #60]
	cmp r2, r3
	beq 0x0200afd2
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_02002f8c_1
	ldr r0, [pc, #56]
	b 0x0200afd4
.L_02002f8c_1:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02002f8c_2
	ldr r0, [pc, #56]
	b 0x0200afd4
.L_02002f8c_2:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne 0x0200afc8
.L_02002fc4:
	ldr r0, [pc, #52]
	b .L_02002fc4_0
	.2byte 0x4b0d
	.2byte 0x429a
	.2byte 0xd101
	.2byte 0x480d
	.2byte 0xe000
	.2byte 0x480d
.L_02002fc4_0:
	pop {r1}
	bx r1
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x00b5
	.2byte 0x0000
	.2byte 0xee44
	.2byte 0x0200
	.2byte 0x00b6
	.2byte 0x0000
	.2byte 0x00b7
	.2byte 0x0000
	.2byte 0xf120
	.2byte 0x0200
	.2byte 0x00b8
	.2byte 0x0000
	.2byte 0xf300
	.2byte 0x0200
	.2byte 0x00b9
	.2byte 0x0000
	.4byte 0x0200f3b4
	.2byte 0x00ba
	.2byte 0x0000
	.2byte 0xf4f8
	.2byte 0x0200
	.2byte 0xef1c
	.2byte 0x0200
	.global Func_0200300c
	.thumb_func
Func_0200300c:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl 0x0200ceac
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r1, r5, #0
	adds r1, #89
	ldrb r2, [r1]
	movs r3, #252
	ands r3, r2
	strb r3, [r1]
	movs r1, #0
	bl 0x0200ce44
	adds r0, r5, #0
	movs r1, #5
	bl 0x0200cdd4
	adds r0, r6, #0
	movs r1, #3
	bl 0x0200cf64
	adds r5, #35
	ldrb r2, [r5]
	movs r3, #2
	orrs r3, r2
	strb r3, [r5]
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02003050
	.thumb_func
Func_02003050:
	push {r5, lr}
	movs r5, #15
.L_02003050_0:
	adds r0, r5, #0
	adds r5, #1
	bl 0x0200ceac
	cmp r5, #18
	bls .L_02003050_0
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #1
	sub	sp, #12
	bl 0x0200ce8c
	ldr	r0, [pc, #976]
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_0200308c
	bl 0x02008558
.L_0200308c:
	movs	r0, #136
	lsls	r0, r0, #1
	bl 0x0200ce7c
	ldr	r3, [pc, #956]
	movs	r2, #224
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	adds	r0, r3, r2
	movs	r3, #129
	lsls	r3, r3, #2
	str	r3, [r0, #0]
	ldr	r1, [pc, #944]
	ldrsh	r2, [r1, r2]
	ldr	r3, [pc, #944]
	cmp	r2, r3
	bne.n	.L_02003112
	movs	r3, #128
	lsls	r3, r3, #1
	str	r3, [r0, #0]
	ldr	r0, [pc, #936]
	bl 0x0200ce74
	cmp	r0, #0
	bne.n	.L_020030c6
	movs	r0, #8
	bl 0x0200b00c
	b.n	.L_020030da
.L_020030c6:
	movs	r3, #7
	movs	r2, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #7
	movs	r1, #17
	movs	r2, #2
	movs	r3, #1
	bl 0x0200ce34
.L_020030da:
	movs	r0, #9
	bl 0x0200b00c
	movs	r0, #10
	bl 0x0200b00c
	movs	r0, #11
	bl 0x0200b00c
	movs	r1, #2
	movs	r0, #11
	bl 0x0200cf64
	movs	r0, #12
	bl 0x0200b00c
	movs	r0, #12
	movs	r1, #2
	bl 0x0200cf64
	movs	r0, #13
	bl 0x0200b00c
	movs	r0, #14
	bl 0x0200b00c
	bl 0x0200bfa6
.L_02003112:
	ldr	r3, [pc, #848]
	cmp	r2, r3
	beq.n	.L_0200311a
	b.n	.L_020033a8
.L_0200311a:
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	subs	r3, #1
	cmp	r3, #25
	bls.n	.L_0200312e
	bl 0x0200bfa6
.L_0200312e:
	ldr	r2, [pc, #824]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x0200b1a0
	.4byte 0x0200b1a0
	.4byte 0x0200b1b2
	.4byte 0x0200b1b2
	.4byte 0x0200b1aa
	.4byte 0x0200b1b2
	.4byte 0x0200b376
	.4byte 0x0200b376
	.4byte 0x0200b376
	.4byte 0x0200b376
	.4byte 0x0200b376
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200b1bc
	.4byte 0x0200b1bc
	.4byte 0x0200b348
	.4byte 0x0200b348
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200b342
	.4byte 0xf7ff2008
	.4byte 0xf000ff33
	.4byte 0x2090fefe
	.4byte 0xf0010040
	.4byte 0x2009fe69
	.4byte 0xff2af7ff
	.4byte 0xfef5f000
	.4byte 0xf00148ab
	.4byte 0x2800fe59
	.4byte 0x2307d050
	.4byte 0x93002208
	.4byte 0x20679201
	.4byte 0x2259211b
	.4byte 0xf001231b
	.4byte 0x2503fe21
	.4byte 0x20292602
	.4byte 0x221b215a
	.4byte 0x9500235c
	.4byte 0xf0019601
	.4byte 0x2029fe17
	.4byte 0x221d215a
	.4byte 0x9500235d
	.4byte 0xf0019601
	.4byte 0x2029fe0f
	.4byte 0x221b215a
	.4byte 0x9500235e
	.4byte 0xf0019601
	.4byte 0x2029fe07
	.4byte 0x221b215a
	.4byte 0x95002360
	.4byte 0xf0019601
	.4byte 0x2029fdff
	.4byte 0x221d215a
	.4byte 0x95002361
	.4byte 0xf0019601
	.4byte 0x2029fdf7
	.4byte 0x22192160
	.4byte 0x9500235b
	.4byte 0xf0019601
	.4byte 0x2029fdef
	.4byte 0x2219215c
	.4byte 0x9500235d
	.4byte 0xf0019601
	.4byte 0x2029fde7
	.4byte 0x22192160
	.4byte 0x9500235f
	.4byte 0xf0019601
	.4byte 0x2029fddf
	.4byte 0x22192160
	.4byte 0xe0562361
	.4byte 0xf0014881
	.4byte 0x2800fe03
	.4byte 0xf000d101
	.4byte 0x2307fe98
	.4byte 0x93002208
	.4byte 0x206f9201
	.4byte 0x2259211b
	.4byte 0xf001231b
	.4byte 0x2503fdc9
	.4byte 0x20292602
	.4byte 0x2219215a
	.4byte 0x9500235b
	.4byte 0xf0019601
	.4byte 0x2029fdbf
	.4byte 0x2219215a
	.4byte 0x9500235d
	.4byte 0xf0019601
	.4byte 0x2029fdb7
	.4byte 0x2219215a
	.4byte 0x9500235f
	.4byte 0xf0019601
	.4byte 0x2029fdaf
	.4byte 0x2219215a
	.4byte 0x95002361
	.4byte 0xf0019601
	.4byte 0x2029fda7
	.4byte 0x221b215a
	.4byte 0x95002360
	.4byte 0xf0019601
	.4byte 0x2029fd9f
	.4byte 0x221d215a
	.4byte 0x95002361
	.4byte 0xf0019601
	.4byte 0x2029fd97
	.4byte 0x221b215e
	.4byte 0x9500235c
	.4byte 0xf0019601
	.4byte 0x2029fd8f
	.4byte 0x221d2160
	.4byte 0x9500235d
	.4byte 0xf0019601
	.4byte 0x2029fd87
	.4byte 0x221b215e
	.4byte 0x9500235e
	.4byte 0xf0019601
	.4byte 0x2029fd7f
	.4byte 0x221b2160
	.4byte 0x95002360
	.4byte 0xf0019601
	.4byte 0x2029fd77
	.4byte 0x221d2160
	.4byte 0x95002361
	.4byte 0xf0019601
	.4byte 0xf000fd6f
	.4byte 0x484cfe32
	.4byte 0xfd9ef001
	.4byte 0xf001484b
	.4byte 0x2080fd9b
	.4byte 0xf0010080
	.4byte 0x2800fd8f
	.4byte 0xf000d101
	.4byte 0x2303fe24
	.4byte 0x93002205
	.4byte 0x202c9201
	.4byte 0x22292175
	.4byte 0xf0012375
	.4byte 0xf000fd55
	.4byte 0x4841fe18
	.4byte 0xfd7cf001
	.4byte 0xd1012800
	.4byte 0xfe11f000
	.4byte 0x22b021da
	.4byte 0x0489200c
	.4byte 0xf00103d2
	.4byte 0x200cfdb1
	.4byte 0xfd8af001
	.4byte 0x1c074b39
	.4byte 0x238060fb
	.4byte 0x63fb061b
	.2byte 0xf000
	.2byte 0xfdff
.L_020033a8:
	ldr	r3, [pc, #216]
	cmp	r2, r3
	beq.n	.L_020033b0
	b.n	.L_020035b4
.L_020033b0:
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	subs	r3, #1
	cmp	r3, #20
	bls.n	.L_020033c4
	bl 0x0200bfa6
.L_020033c4:
	ldr	r2, [pc, #192]
	lsls	r3, r3, #2
.L_020033c8:
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0200bc86
	.4byte 0x0200bc86
	.4byte 0x0200bc86
	.4byte 0x0200bc86
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200b4dc
	.4byte 0x0200b4dc
	.4byte 0x0200b48c
	.2byte 0xb48c
	.2byte 0x0200
	push	{r2, r5, lr}
	lsls	r0, r0, #8
	push	{r2, r5, lr}
	lsls	r0, r0, #8
	push	{r2, r5, lr}
	lsls	r0, r0, #8
	push	{r2, r5, lr}
	lsls	r0, r0, #8
	push	{r2, r5, lr}
	lsls	r0, r0, #8
	push	{r5}
	lsls	r0, r0, #8
	.2byte 0xbfa6
	.2byte 0x0200
	.2byte 0xbfa6
	.2byte 0x0200
	.2byte 0xbc86
	.2byte 0x0200
	push	{r1, r4, r5}
	lsls	r0, r0, #8
	push	{r1, r3, r5}
	lsls	r0, r0, #8
	ldr	r0, [pc, #84]
	bl 0x0200ce84
	bl 0x0200bfa6
	bl 0x0200c2bc
	bl 0x0200bfa6
	movs	r0, #170
	bl 0x0200cfec
	ldr	r0, [pc, #20]
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_02003446
	bl 0x0200bfa6
.L_02003446:
	bl 0x020089c8
	bl 0x0200bfa6
	movs	r0, r0
	lsls	r1, r1, #4
	movs	r0, r0
.L_02003454:
	subs	r4, r7, #2
	lsls	r0, r0, #12
	lsls	r0, r0, #9
	lsls	r0, r0, #8
	lsls	r5, r6, #2
	movs	r0, r0
	lsrs	r1, r0, #6
	movs	r0, r0
	lsls	r6, r6, #2
	movs	r0, r0
	.2byte 0xb138
	lsls	r0, r0, #8
	lsrs	r2, r0, #6
	movs	r0, r0
	lsrs	r3, r0, #6
	movs	r0, r0
	lsls	r1, r4, #4
	movs	r0, r0
	lsls	r7, r5, #4
	movs	r0, r0
	lsrs	r7, r0, #6
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffe8
	.2byte 0x00b7
	movs	r0, r0
	.2byte 0xb3cc
	lsls	r0, r0, #8
	movs	r0, #11
	bl 0x0200ceac
	adds	r7, r0, #0
	bl 0x020099c0
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	r3, #8
	bne.n	.L_020034a6
	adds	r0, r7, #0
	bl 0x02009a14
.L_020034a6:
	movs	r0, #12
	bl 0x0200ceac
	adds	r7, r0, #0
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	r3, #7
	bne.n	.L_020034ba
	bl 0x02009a14
.L_020034ba:
	ldr	r5, [pc, #880]
	movs	r0, #206
	movs	r1, #0
	adds	r2, r5, #0
	movs	r3, #223
	lsls	r0, r0, #16
	bl 0x02008058
	movs	r0, #210
	lsls	r0, r0, #16
.L_020034ce:
	movs	r1, #0
	adds	r2, r5, #0
	movs	r3, #223
	bl 0x02008058
	bl 0x0200bfa6
	movs	r0, #0
	bl 0x0200cfdc
	movs	r0, #2
	bl 0x0200cda4
	movs	r0, #8
	bl 0x0200ceac
	adds	r7, r0, #0
.L_020034f0:
	ldr	r5, [pc, #828]
	adds	r3, r7, #0
	adds	r3, #85
	movs	r6, #0
	strb	r6, [r3, #0]
	movs	r0, #9
	str	r5, [r7, #108]
	bl 0x0200ceac
	adds	r7, r0, #0
	adds	r3, r7, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	movs	r0, #10
	str	r5, [r7, #108]
	bl 0x0200ceac
.L_02003512:
	adds	r7, r0, #0
	adds	r3, r7, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	str	r5, [r7, #108]
	bl 0x020098f8
.L_02003520:
	bl 0x0200bfa6
	movs	r0, #170
	bl 0x0200cfec
	movs	r0, #0
	bl 0x0200cfdc
	movs	r0, #2
	bl 0x0200cda4
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x0200ce74
	cmp	r0, #0
	bne.n	.L_02003546
	bl 0x0200bfa6
.L_02003546:
	movs	r5, #5
	movs	r6, #2
	movs	r0, #111
	movs	r1, #5
	movs	r2, #117
	movs	r3, #5
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ce1c
	movs	r0, #111
	movs	r1, #10
	movs	r2, #117
	movs	r3, #10
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ce1c
	movs	r0, #111
	movs	r1, #7
	movs	r2, #111
	movs	r3, #5
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ce1c
	movs	r0, #111
	movs	r1, #7
	movs	r2, #111
	movs	r3, #10
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ce1c
	movs	r3, #54
	movs	r5, #3
	str	r3, [sp, #0]
	movs	r0, #48
	movs	r1, #3
	movs	r2, #3
	movs	r3, #10
	str	r5, [sp, #4]
	bl 0x0200ce2c
	movs	r3, #48
	str	r3, [sp, #0]
	movs	r0, #55
	movs	r1, #26
	movs	r2, #3
	movs	r3, #10
	str	r5, [sp, #4]
	bl 0x0200ce2c
	bl 0x0200bfa6
.L_020035b4:
	ldr	r3, [pc, #636]
	cmp	r2, r3
	beq.n	.L_020035bc
	b.n	.L_020037bc
.L_020035bc:
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	subs	r3, #1
	cmp	r3, #10
	bls.n	.L_020035d0
	bl 0x0200bfa6
.L_020035d0:
	ldr	r2, [pc, #612]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0200bc86
	.4byte 0x0200b604
	.4byte 0x0200bfa6
	.4byte 0x0200b60a
	.4byte 0x0200bfa6
	.4byte 0x0200b60a
	.4byte 0x0200b706
	.4byte 0x0200b706
	.4byte 0x0200b614
	.4byte 0x0200b614
	.4byte 0x0200b6ee
	.4byte 0xf8e8f7fd
	.4byte 0x2000e33d
	.4byte 0xfce6f001
	.4byte 0xfcc9f000
	.4byte 0xf0012008
	.4byte 0x2200fc49
	.4byte 0x46901c07
	.4byte 0x33551c3b
	.4byte 0x70194641
	.4byte 0x60fa2009
	.4byte 0xfc3ef001
	.4byte 0x1c072355
	.4byte 0x464119db
	.4byte 0x469a7019
	.4byte 0x33591c3b
	.4byte 0x487e7019
	.4byte 0xfc16f001
	.4byte 0xd04c2800
	.4byte 0xf0012001
	.4byte 0x2301fba9
	.4byte 0x25029300
	.4byte 0x2129207c
	.4byte 0x2329226e
	.4byte 0xf0019501
	.4byte 0x232afbdb
	.4byte 0x262e9301
	.4byte 0x202e2301
	.4byte 0x22012129
	.4byte 0xf0019600
	.4byte 0x21bafbdd
	.4byte 0x200922b6
	.4byte 0x04920489
	.4byte 0xfc36f001
	.4byte 0x46534642
	.4byte 0x4b6c701a
	.4byte 0x60fb2009
	.4byte 0xf0012103
	.4byte 0x1c3bfc65
	.4byte 0x701d3323
	.4byte 0x9301232d
	.4byte 0x23012201
	.4byte 0x212d202d
	.4byte 0xf0019600
	.4byte 0x200afbc1
	.4byte 0xf0012107
	.4byte 0x2101fc21
	.4byte 0xf001200a
	.4byte 0x200afc51
	.4byte 0xfbf2f001
	.4byte 0x1c3b1c07
	.4byte 0x46413359
	.4byte 0x22ae7019
	.4byte 0x701d3b36
	.4byte 0x495a200a
	.4byte 0xf0010492
	.4byte 0x4b59fc09
	.4byte 0xf7fe66fb
	.4byte 0xf000fb2f
	.4byte 0x4b57fc5c
	.4byte 0x681b22e0
	.4byte 0x189b0052
	.4byte 0x601a3242
	.4byte 0xf0012000
	.4byte 0x4b53fbd5
	.4byte 0x20aa60c3
	.4byte 0xfc70f001
	.4byte 0xfb8af001
	.4byte 0x4a502300
	.4byte 0x20c08013
	.4byte 0xf0010080
	.4byte 0x2800fbab
	.4byte 0x2503d034
	.4byte 0x2160200f
	.4byte 0x23602209
	.4byte 0x95019500
	.4byte 0xfb74f001
	.4byte 0x2160200c
	.4byte 0x2360220f
	.4byte 0x95019500
	.4byte 0xfb6cf001
	.4byte 0x20052604
	.4byte 0x220f2132
	.4byte 0x95002320
	.4byte 0xf0019601
	.4byte 0x2019fb63
	.4byte 0x2209212d
	.4byte 0x95002320
	.4byte 0xf0019601
	.4byte 0x2309fb5b
	.4byte 0x93002520
	.4byte 0x2120200f
	.4byte 0x23012203
	.4byte 0xf0019501
	.4byte 0x230ffb5d
	.4byte 0x200c9300
	.4byte 0x22032120
	.4byte 0x95012301
	.4byte 0xfb54f001
	.4byte 0x21e14b32
	.4byte 0x185b0049
	.4byte 0x5e9b2200
	.4byte 0xd0012b0b
	.4byte 0xfc03f000
	.4byte 0xfc10f001
	.4byte 0xfc16f001
	.4byte 0x21e04b28
	.4byte 0x0049681b
	.4byte 0x185b2281
	.4byte 0x601a0092
	.2byte 0xf000
	.2byte 0xfbf5
.L_020037bc:
	ldr	r3, [pc, #156]
	cmp	r2, r3
	beq.n	.L_020037c4
	.2byte 0xe159
.L_020037c4:
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	subs	r3, #4
	cmp	r3, #16
	bls.n	.L_020037d6
	.2byte 0xe3e7
.L_020037d6:
	ldr	r2, [pc, #136]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x0200b910
	.4byte 0x0200b910
	.4byte 0x0200bc86
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200b864
	.4byte 0x0200b864
	.4byte 0x0200b864
	.4byte 0x0200b864
	.4byte 0x0200b864
	.4byte 0x0200b864
	.4byte 0x0200b916
	.4byte 0x0200b916
	.4byte 0x0200bfa6
	.4byte 0x0200bfa6
	.4byte 0x0200b824
	.4byte 0x0200b864
	.4byte 0xfef4f000
	.4byte 0x0000e3bd
	.4byte 0x01c10000
	.4byte 0x020088c9
	.2byte 0x00b8
	.2byte 0x0000
	push	{r3, r4, r6, r7, lr}
	lsls	r0, r0, #8
	lsls	r1, r0, #12
	movs	r0, r0
	movs	r0, r0
	.2byte 0xfff0
	.2byte 0x0000
	lsls	r7, r4, #11
	ldrh	r1, [r3, #28]
	lsls	r0, r0, #8
	subs	r4, r7, #2
	lsls	r0, r0, #12
	movs	r0, r0
	.2byte 0xfffe
	.2byte 0x0050
	lsls	r0, r0, #16
	lsls	r0, r0, #9
	lsls	r0, r0, #8
	lsls	r1, r7, #2
	movs	r0, r0
	.2byte 0xb7e0
	lsls	r0, r0, #8
	movs	r0, #170
	bl 0x0200cfec
	ldr	r0, [pc, #724]
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_020038ac
	movs	r3, #26
	movs	r2, #12
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #53
	movs	r1, #12
	movs	r2, #3
	movs	r3, #13
	bl 0x0200ce2c
	movs	r3, #9
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #41
	movs	r0, #81
	movs	r2, #89
	movs	r3, #14
	bl 0x0200ce1c
	movs	r0, #1
	bl 0x0200cda4
	movs	r1, #200
	ldr	r0, [pc, #668]
	lsls	r1, r1, #4
	bl 0x0200cdac
.L_020038ac:
	ldr	r0, [pc, #664]
	bl 0x0200ce74
	cmp	r0, #0
	beq.n	.L_020038ee
	movs	r3, #34
	movs	r2, #12
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #58
	movs	r1, #12
	movs	r2, #3
	movs	r3, #13
	bl 0x0200ce2c
	movs	r3, #5
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #41
	movs	r0, #81
	movs	r2, #97
	movs	r3, #14
	bl 0x0200ce1c
	movs	r0, #1
	bl 0x0200cda4
	movs	r1, #200
	ldr	r0, [pc, #612]
	lsls	r1, r1, #4
	bl 0x0200cdac
.L_020038ee:
	ldr	r3, [pc, #608]
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #11
	bne.n	.L_02003904
	bl 0x020087d8
	b.n	.L_02003fa6
.L_02003904:
	cmp	r3, #20
	beq.n	.L_0200390a
	b.n	.L_02003fa6
.L_0200390a:
	bl 0x0200c7c0
	b.n	.L_02003fa6
	.4byte 0xff62f7fc
	.4byte 0x2008e1b7
	.4byte 0xfac8f001
	.4byte 0x1c3b1c07
	.4byte 0x33552500
	.4byte 0x2009701d
	.4byte 0xf00160fd
	.4byte 0x3055fabf
	.4byte 0x200a7005
	.4byte 0xfabaf001
	.4byte 0x70053055
	.4byte 0xf001200b
	.4byte 0x3055fab5
	.4byte 0x20c17005
	.4byte 0xf0010080
	.4byte 0x2800fa93
	.4byte 0xe08ed100
	.4byte 0xf0012001
	.4byte 0x2301fa25
	.4byte 0x93002202
	.4byte 0x206f9201
	.4byte 0x226d213b
	.4byte 0xf0012325
	.4byte 0x232dfa57
	.4byte 0x93002226
	.4byte 0x202d9201
	.4byte 0x22012125
	.4byte 0xf0012301
	.4byte 0x4874fa59
	.4byte 0xfa76f001
	.4byte 0xd0152800
	.4byte 0x22a621c2
	.4byte 0x04892009
	.4byte 0xf0010492
	.4byte 0x21d2faad
	.4byte 0x200a22a6
	.4byte 0x04920489
	.4byte 0xfaa6f001
	.4byte 0x22ae21c2
	.4byte 0x0489200b
	.4byte 0xf0010492
	.4byte 0xe014fa9f
	.4byte 0x22a621d2
	.4byte 0x04892009
	.4byte 0xf0010492
	.4byte 0x21c2fa97
	.4byte 0x200a22ae
	.4byte 0x04920489
	.4byte 0xfa90f001
	.4byte 0x22ae21d2
	.4byte 0x0489200b
	.4byte 0xf0010492
	.4byte 0x2103fa89
	.4byte 0xf0012009
	.4byte 0x2009fabd
	.4byte 0xfa5ef001
	.4byte 0x4d591c07
	.4byte 0x33231c3b
	.4byte 0x22002602
	.4byte 0x210360fd
	.4byte 0x200a701e
	.4byte 0xf0014690
	.4byte 0x200afaad
	.4byte 0xfa4ef001
	.4byte 0x1c3b1c07
	.4byte 0x60fd3323
	.4byte 0x701e2103
	.4byte 0xf001200b
	.4byte 0x200bfaa1
	.4byte 0xfa42f001
	.4byte 0x1c3b1c07
	.4byte 0x60fd3323
	.4byte 0x701e2107
	.4byte 0xf001200c
	.4byte 0x200cfa61
	.4byte 0xfa36f001
	.4byte 0xf0012100
	.4byte 0x2101f9ff
	.4byte 0xf001200c
	.4byte 0x200cfa8b
	.4byte 0xfa2cf001
	.4byte 0x1c3b1c07
	.4byte 0x46413359
	.4byte 0x229e7019
	.4byte 0x701e3b36
	.4byte 0x493d200c
	.4byte 0xf0010492
	.4byte 0x4b3cfa43
	.4byte 0xf7fe66fb
	.2byte 0xfa75
	.2byte 0xe296
.L_02003a78:
	ldr	r3, [pc, #232]
	cmp	r2, r3
	beq.n	.L_02003a80
	b.n	.L_02003fa6
.L_02003a80:
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	subs	r3, #1
	cmp	r3, #19
	bls.n	.L_02003a92
	b.n	.L_02003fa6
.L_02003a92:
	ldr	r2, [pc, #212]
.L_02003a94:
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x0200baec
	.4byte 0x0200baec
	.4byte 0x0200bca6
	.4byte 0x0200bca6
	.4byte 0x0200bca6
	.4byte 0x0200bca6
	.4byte 0x0200bca6
	.4byte 0x0200bca6
	.4byte 0x0200bca6
	.4byte 0x0200bca6
	.4byte 0x0200bca6
	.4byte 0x0200bca6
	.4byte 0x0200bc86
	.4byte 0x0200bc86
	.4byte 0x0200be34
	.4byte 0x0200be34
	.4byte 0x0200bfa6
	.4byte 0x0200bc8e
	.4byte 0x0200bc8e
	.4byte 0x0200bca6
	.4byte 0xf001481f
	.4byte 0x2800f9c1
	.4byte 0x2000d03c
	.4byte 0xfbe6f7fe
	.4byte 0xf0012000
	.4byte 0xf7fcf9d5
	.4byte 0x2500ff23
	.4byte 0x300a1c28
	.4byte 0xf9cef001
	.4byte 0x68bb1c07
	.4byte 0x2c0d151c
	.4byte 0x693bd10d
	.4byte 0x2e07151e
	.4byte 0x2280d109
	.4byte 0x18a80092
	.4byte 0xf0019402
	.4byte 0x9c02f9a3
	.4byte 0xd0002800
	.4byte 0x3501e169
	.4byte 0xd9e52d03
	.4byte 0x0000e233
	.4byte 0x00000306
	.4byte 0x0200c5f1
	.4byte 0x00000307
	.4byte 0x0200c601
	.4byte 0x02000240
	.4byte 0x00000302
	.4byte 0xfff00000
	.4byte 0x02d70000
	.4byte 0x02008b99
	.4byte 0x000000ba
	.4byte 0x0200ba9c
	.4byte 0x00000109
	.4byte 0xf0012008
	.4byte 0x2100f99b
	.4byte 0x46891c07
	.4byte 0x33551c3b
	.4byte 0x701a464a
	.4byte 0x1c3a4b18
	.4byte 0x322360fb
	.4byte 0x7813469b
	.4byte 0x43332602
	.4byte 0x1c397013
	.4byte 0x780a3159
	.4byte 0x1c2b25fe
	.4byte 0x700b4013
	.4byte 0x490f2203
	.4byte 0x1c3b4692
	.4byte 0x33644688
	.4byte 0x80194651
	.4byte 0x21012008
	.4byte 0xf9d4f001
	.4byte 0xf0012009
	.4byte 0x1c07f975
	.4byte 0x33551c3b
	.4byte 0x701a4642
	.4byte 0x60fb465b
	.4byte 0x32231c3a
	.4byte 0x431e7813
	.4byte 0x32367016
	.4byte 0x401d7813
	.4byte 0xe0031c3b
	.4byte 0x00000000
	.4byte 0xffd00000
	.4byte 0x46513364
	.4byte 0x20097015
	.4byte 0x21018019
	.4byte 0xf9b4f001
	.4byte 0xf001200a
	.4byte 0x1c07f955
	.4byte 0x46421c3b
	.4byte 0x701a3355
	.4byte 0x330f4649
	.4byte 0x200a8019
	.4byte 0xf94af001
	.4byte 0xf0012100
	.4byte 0x200bf913
	.4byte 0xf944f001
	.4byte 0x1c3b1c07
	.4byte 0x33554642
	.4byte 0x4649701a
	.4byte 0x8019330f
	.4byte 0xf001200b
	.4byte 0x2100f939
	.4byte 0xf902f001
	.4byte 0xf001200c
	.4byte 0x1c07f933
	.4byte 0x46421c3b
	.4byte 0x701a3355
	.4byte 0x330f4649
	.4byte 0x200c8019
	.4byte 0xf928f001
	.4byte 0xf0012100
	.4byte 0x200df8f1
	.4byte 0xf922f001
	.4byte 0x1c3b1c07
	.4byte 0x46423355
	.4byte 0x4649701a
	.4byte 0x8019330f
	.4byte 0xf001200d
	.4byte 0x2100f917
	.4byte 0xf8e0f001
	.4byte 0x20aae18f
	.4byte 0xf9b0f001
	.4byte 0x4bcbe18b
	.4byte 0x681b22e0
	.4byte 0x189b0052
	.4byte 0x601a3242
	.4byte 0xf0012000
	.4byte 0x4bc7f905
	.4byte 0x201460c3
	.4byte 0xf900f001
	.4byte 0x30552704
	.4byte 0x20147007
	.4byte 0xf8faf001
	.4byte 0x78023023
	.4byte 0x43132302
	.4byte 0x20147003
	.4byte 0xf8f2f001
	.4byte 0x60c34bbe
	.4byte 0xf8aaf001
	.4byte 0x4bbd2500
	.4byte 0x48bd801d
	.4byte 0xf8ccf001
	.4byte 0xd0382800
	.4byte 0xf00120aa
	.4byte 0x2603f983
	.4byte 0x20242502
	.4byte 0x22202151
	.4byte 0x95012351
	.4byte 0xf0019600
	.4byte 0x2024f891
	.4byte 0x22242153
	.4byte 0x95012351
	.4byte 0xf0019600
	.4byte 0x2320f889
	.4byte 0x25119300
	.4byte 0x21112024
	.4byte 0x23012203
	.4byte 0xf0019501
	.4byte 0x2324f88b
	.4byte 0x20249300
	.4byte 0x22032112
	.4byte 0x95012301
	.4byte 0xf882f001
	.4byte 0x93002301
	.4byte 0x203f9301
	.4byte 0x2221211d
	.4byte 0xf0012314
	.4byte 0x2014f86d
	.4byte 0x22242138
	.4byte 0x96002311
	.4byte 0xf0019701
	.4byte 0x489ff865
	.4byte 0xf88ef001
	.4byte 0xd0352800
	.4byte 0x25022603
	.4byte 0x2151202c
	.4byte 0x23512230
	.4byte 0x96009501
	.4byte 0xf856f001
	.4byte 0x2153202c
	.4byte 0x2351222c
	.4byte 0x96009501
	.4byte 0xf84ef001
	.4byte 0x93002330
	.4byte 0x202c2511
	.4byte 0x22032111
	.4byte 0x95012301
	.4byte 0xf850f001
	.4byte 0x9300232c
	.4byte 0x2112202c
	.4byte 0x23012203
	.4byte 0xf0019501
	.4byte 0x2301f847
	.4byte 0x93019300
	.4byte 0x211d203f
	.4byte 0x23142231
	.4byte 0xf832f001
	.4byte 0x21382029
	.4byte 0x2311222c
	.4byte 0x97019600
	.4byte 0xf82af001
	.4byte 0x21e14b82
	.4byte 0x185d0049
	.4byte 0x1c13882a
	.4byte 0x21803b12
	.4byte 0x0249041b
	.4byte 0xd80b428b
	.4byte 0xf8f0f001
	.4byte 0xf8f6f001
	.4byte 0x22e04b74
	.4byte 0x0052681b
	.4byte 0x3244189b
	.4byte 0x882a601a
	.4byte 0x041321a0
	.4byte 0x428b0349
	.4byte 0xe0d0d000
	.4byte 0xfe12f000
	.4byte 0x1c38e0cd
	.4byte 0x78023023
	.4byte 0x43132302
	.4byte 0x1c3b7003
	.4byte 0x33592100
	.4byte 0x3b047019
	.4byte 0x20047019
	.4byte 0x22012113
	.4byte 0x94002301
	.4byte 0xf0019601
	.4byte 0xe0b8f801
	.4byte 0xf0002001
	.4byte 0x21c8ffb5
	.4byte 0x48660109
	.4byte 0xffb4f000
	.4byte 0xf001200e
	.4byte 0x2200f831
	.4byte 0x46901c07
	.4byte 0x46411c3b
	.4byte 0x70193355
	.4byte 0x60fa200f
	.4byte 0xf826f001
	.4byte 0x30554643
	.4byte 0x20107003
	.4byte 0xf820f001
	.4byte 0x30554641
	.4byte 0x20117001
	.4byte 0xf81af001
	.4byte 0x30554642
	.4byte 0x20127002
	.4byte 0xf814f001
	.4byte 0x30554643
	.4byte 0x20c27003
	.4byte 0xf0000080
	.4byte 0x2800fff1
	.4byte 0xe084d100
	.4byte 0xf0002001
	.4byte 0x2301ff83
	.4byte 0x26029300
	.4byte 0x2138205f
	.4byte 0x2323224d
	.4byte 0xf0009601
	.4byte 0x230dffb5
	.4byte 0x93002224
	.4byte 0x23019201
	.4byte 0x2123200d
	.4byte 0xf0002201
	.4byte 0x2184ffb7
	.4byte 0x049222ba
	.4byte 0x200f0449
	.4byte 0xf810f001
	.4byte 0xf000200f
	.4byte 0x1c07ffe9
	.4byte 0x1c3b4d3f
	.4byte 0x60fd3323
	.4byte 0x701e200f
	.4byte 0xf0012103
	.4byte 0x21b8f83b
	.4byte 0x0492229e
	.4byte 0x20100409
	.4byte 0xfffcf000
	.4byte 0xf0002010
	.4byte 0x1c07ffd5
	.4byte 0x33231c3b
	.4byte 0x201060fd
	.4byte 0x2103701e
	.4byte 0xf828f001
	.4byte 0x22ae21e8
	.4byte 0x04090492
	.4byte 0xf0002011
	.4byte 0x2011ffe9
	.4byte 0xffc2f000
	.4byte 0x1c3b1c07
	.4byte 0x60fd3323
	.4byte 0x701e2011
	.4byte 0xf0012103
	.4byte 0x21b8f815
	.4byte 0x049222a6
	.4byte 0x20120409
	.4byte 0xffd6f000
	.4byte 0xf0002012
	.4byte 0x1c07ffaf
	.4byte 0x33231c3b
	.4byte 0x201260fd
	.4byte 0x2103701e
	.4byte 0xf802f001
	.4byte 0x20132107
	.4byte 0xffcaf000
	.4byte 0xf0002013
	.4byte 0x2100ff9f
	.4byte 0xff68f000
	.4byte 0x20132101
	.4byte 0xfff4f000
	.4byte 0xf0002013
	.4byte 0x1c07ff95
	.4byte 0x33591c3b
	.4byte 0x70194641
	.4byte 0x3b362296
	.4byte 0x701e21d7
	.4byte 0x04092013
	.4byte 0xf0000492
	.4byte 0x4b10ffab
	.4byte 0xf7fe66fb
	.2byte 0xfdb7
.L_02003fa6:
	movs	r0, #0
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0xfffe0000
	.4byte 0xffef8000
	.4byte 0x04000050
	.4byte 0x00000306
	.4byte 0x00000307
	.4byte 0x02000240
	.4byte 0x0200b051
	.4byte 0xfff00000
	.2byte 0x8b99
	.2byte 0x0200
	.global Func_02003fe4
	.thumb_func
Func_02003fe4:
	push {lr}
	sub sp, #12
	mov r0, sp
	movs r3, #0
	str r3, [r0]
	subs r3, #1
	str r3, [r0, #8]
	bl 0x0200c048
	sub sp, #-12
	pop {r0}
	bx r0
	.global Func_02003ffc
	.thumb_func
Func_02003ffc:
	push {lr}
	sub sp, #12
	mov r0, sp
	movs r3, #0
	str r3, [r0]
	movs r3, #1
	str r3, [r0, #8]
	bl 0x0200c048
	sub sp, #-12
	pop {r0}
	bx r0
	.global Func_02004014
	.thumb_func
Func_02004014:
	push {lr}
	sub sp, #12
	movs r3, #1
	mov r0, sp
	negs r3, r3
	str r3, [r0]
	movs r3, #0
	str r3, [r0, #8]
	bl 0x0200c048
	sub sp, #-12
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02004030
	.thumb_func
Func_02004030:
	push {lr}
	sub sp, #12
	mov r0, sp
	movs r3, #1
	str r3, [r0]
	movs r3, #0
	str r3, [r0, #8]
	bl 0x0200c048
	sub sp, #-12
	pop {r0}
	bx r0
	.global Func_02004048
	.thumb_func
Func_02004048:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #584]
	mov r9, r0
	ldr r2, [r3]
	movs r0, #250
	ldr r3, [pc, #580]
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r3, [r3]
	sub sp, #88
	movs r1, #240
	str r3, [sp, #32]
	lsls r1, r1, #1
	adds r2, r2, r1
	ldr r2, [r2]
	adds r0, r3, #0
	str r2, [sp, #28]
	bl 0x0200ceac
	adds r7, r0, #0
	mov r0, r9
	ldr r3, [r0]
	add r2, sp, #76
	mov r11, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, [r7, #8]
	lsls r2, r2, #15
	mov r1, r11
	adds r3, r3, r2
	str r3, [r1]
	ldr r3, [r7, #12]
	str r3, [r1, #4]
	ldr r3, [r0, #8]
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, [r7, #16]
	lsls r2, r2, #15
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r7, #0
	bl 0x0200ce3c
	ldr r3, [pc, #508]
	ldr r3, [r3]
	str r3, [sp, #24]
	ldr r2, [sp, #24]
	movs r3, #4
	ands r2, r3
	mov r10, r0
	str r2, [sp, #24]
	cmp r2, #0
	bne .L_02004048_0
	bl 0x0200cdb4
	movs r1, #248
	mov r3, sp
	lsls r0, r0, #12
	adds r3, #36
	lsls r1, r1, #8
	lsrs r0, r0, #16
	adds r2, r3, #0
	adds r0, r0, r1
	str r3, [sp, #20]
	strh r0, [r2, #34]
	bl 0x0200cdb4
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #2
	ldr r5, [r7, #8]
	lsrs r3, r3, #16
	lsls r3, r3, #16
	adds r5, r5, r3
	ldr r3, [pc, #448]
	adds r5, r5, r3
	bl 0x0200cdb4
	lsls r3, r0, #2
	adds r3, r3, r0
	ldr r0, [pc, #440]
	lsrs r3, r3, #16
	mov r8, r0
	mov r1, r8
	muls r1, r3
	ldr r6, [pc, #436]
	mov r0, r9
	ldr r2, [r0]
	adds r3, r1, #0
	adds r3, r3, r6
	adds r1, r2, #0
	muls r1, r3
	str r1, [sp, #16]
	bl 0x0200cdb4
	lsls r3, r0, #2
	adds r3, r3, r0
	lsrs r3, r3, #16
	mov r2, r8
	muls r2, r3
	mov r0, r9
	adds r3, r2, #0
	ldr r2, [r0, #8]
	adds r3, r3, r6
	muls r3, r2
	ldr r1, [r7, #12]
	ldr r2, [r7, #16]
	str r3, [sp, #4]
	movs r3, #128
	lsls r3, r3, #16
	ldr r0, [sp, #24]
	str r3, [sp, #8]
	ldr r3, [sp, #20]
	str r0, [sp, #0]
	str r3, [sp, #12]
	adds r0, r5, #0
	ldr r3, [sp, #16]
	bl 0x02008118
.L_02004048_0:
	mov r0, r10
	cmp r0, #0
	bge .L_02004048_2
	movs r1, #129
	ldr r0, [sp, #32]
	lsls r1, r1, #1
	bl 0x0200cf74
	ldr r3, [r7, #16]
	movs r0, #128
	lsls r0, r0, #12
	adds r3, r3, r0
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	bl 0x0200ce04
	adds r0, r7, #0
	movs r1, #7
	bl 0x0200cdd4
	adds r0, r7, #0
	bl 0x0200ce0c
.L_02004048_3:
	movs r0, #1
	bl 0x0200cda4
	ldr r2, [r7, #12]
	ldr r3, [r7, #20]
	cmp r2, r3
	bne .L_02004048_3
	adds r0, r7, #0
	movs r1, #6
	bl 0x0200cdd4
	movs r0, #3
	bl 0x0200cda4
	b .L_02004048_4
.L_02004048_2:
	mov r1, r9
	ldr r2, [r1]
	ldr r3, [r7, #8]
	lsls r2, r2, #19
	adds r3, r3, r2
	mov r2, r11
	str r3, [r2]
	ldr r3, [r7, #12]
	str r3, [r2, #4]
	ldr r2, [r1, #8]
	ldr r3, [r7, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	mov r0, r11
	str r3, [r0, #8]
	mov r1, r11
	adds r0, r7, #0
	bl 0x0200ce3c
	mov r10, r0
	cmp r0, #0
	bgt .L_02004048_4
	mov r1, r9
	ldr r3, [r1]
	ldr r5, [pc, #244]
	mov r0, r9
	ldr r2, [r0, #8]
	adds r1, r3, #0
	muls r1, r5
	muls r2, r5
	ldr r3, [r7, #8]
	adds r3, r3, r1
	mov r0, r11
	subs r3, r3, r2
	str r3, [r0]
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	adds r3, r3, r2
	subs r3, r3, r1
	str r3, [r0, #8]
	mov r1, r11
	adds r0, r7, #0
	bl 0x0200ce3c
	mov r10, r0
	cmp r0, #0
	ble .L_02004048_5
	mov r1, r9
	ldr r3, [r1, #8]
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, [r7, #8]
	lsls r2, r2, #15
	adds r3, r3, r2
	str r3, [r7, #8]
	ldr r3, [r1]
	b .L_02004048_6
.L_02004048_5:
	mov r2, r9
	ldr r3, [r2]
	ldr r2, [r2, #8]
	adds r3, r3, r2
	adds r2, r3, #0
	muls r2, r5
	ldr r3, [r7, #8]
	mov r0, r11
	adds r3, r2, r3
	str r3, [r0]
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	adds r2, r2, r3
	str r2, [r0, #8]
	mov r1, r11
	adds r0, r7, #0
	bl 0x0200ce3c
	mov r10, r0
	cmp r0, #0
	ble .L_02004048_7
	mov r1, r9
	ldr r3, [r1, #8]
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, [r7, #8]
	lsls r2, r2, #15
	subs r3, r3, r2
	str r3, [r7, #8]
	ldr r3, [r1]
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, [r7, #16]
	lsls r2, r2, #15
	subs r3, r3, r2
	b .L_02004048_8
.L_02004048_7:
	mov r2, r9
	ldr r3, [r2]
	ldr r0, [sp, #28]
	lsls r2, r3, #1
	adds r2, r2, r3
.L_02004048_1:
	ldr r3, [r0, #8]
	lsls r2, r2, #15
	adds r3, r3, r2
	str r3, [r0, #8]
	mov r1, r9
	ldr r3, [r1]
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, [r7, #8]
	lsls r2, r2, #15
	adds r3, r3, r2
	str r3, [r7, #8]
	ldr r3, [r1, #8]
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, [r0, #16]
	lsls r2, r2, #15
	adds r3, r3, r2
	str r3, [r0, #16]
	ldr r3, [r1, #8]
.L_02004048_6:
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, [r7, #16]
	lsls r2, r2, #15
	adds r3, r3, r2
.L_02004048_8:
	str r3, [r7, #16]
.L_02004048_4:
	sub sp, #-88
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x03001e40
	.4byte 0xfffa0000
	.4byte 0x00001999
	.4byte 0x00007ffd
	.4byte 0x0005b333
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #580]
	movs	r2, #224
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #66
	str	r2, [r3, #0]
	sub	sp, #56
	bl 0x0200ce94
	movs	r0, #0
	bl 0x0200ceac
	movs	r1, #0
	bl 0x0200ce44
	movs	r1, #15
	movs	r0, #0
	bl 0x0200cf34
	movs	r0, #170
	bl 0x0200cfec
	bl 0x0200cfc4
	bl 0x0200cfd4
	movs	r0, #40
	bl 0x0200ce8c
	movs	r0, #162
	bl 0x0200cffc
	movs	r2, #16
	movs	r3, #0
	add	r2, sp
	mov	sl, r3
	mov	r8, r3
	mov	r9, r2
	mov	fp, r3
.L_0200431a:
	bl 0x0200cdb4
	ldr	r2, [pc, #500]
	lsls	r0, r0, #1
	lsrs	r0, r0, #16
	adds	r3, r0, #0
	muls	r3, r2
	ldr	r2, [pc, #492]
	adds	r3, r3, r2
	str	r3, [sp, #24]
	bl 0x0200cdb4
	ldr	r2, [pc, #480]
	lsls	r0, r0, #1
	lsrs	r0, r0, #16
	adds	r3, r0, #0
	muls	r3, r2
	ldr	r2, [pc, #472]
	adds	r3, r3, r2
	str	r3, [sp, #28]
	bl 0x0200cdb4
	movs	r3, #248
	lsls	r0, r0, #12
	lsrs	r0, r0, #16
	lsls	r3, r3, #8
	movs	r2, #50
	adds	r0, r0, r3
	add	r2, sp
	strh	r0, [r2, #0]
.L_02004356:
	mov	r3, r8
	movs	r6, #0
	cmp	r3, #7
	bhi.n	.L_020043a4
	movs	r5, #192
	lsls	r5, r5, #14
	movs	r7, #0
	add	r5, fp
.L_02004366:
	bl 0x0200cdb4
	adds	r3, r0, #0
	lsls	r0, r3, #3
	subs	r0, r0, r3
	movs	r3, #136
	lsls	r3, r3, #16
	lsrs	r0, r0, #16
	movs	r2, #216
	lsls	r2, r2, #18
	str	r3, [sp, #8]
	lsls	r0, r0, #19
	mov	r3, r9
	adds	r0, r0, r2
	str	r3, [sp, #12]
	adds	r2, r5, #0
	movs	r1, #0
	movs	r3, #0
	str	r7, [sp, #0]
	str	r7, [sp, #4]
	bl 0x02008118
	movs	r2, #128
	lsls	r2, r2, #11
	adds	r6, #1
	adds	r5, r5, r2
	cmp	r6, #3
	bhi.n	.L_020043a4
	mov	r3, r8
	cmp	r3, #7
	bls.n	.L_02004366
.L_020043a4:
	movs	r0, #3
	bl 0x0200cda4
	mov	r2, r8
	cmp	r2, #3
	bne.n	.L_020043bc
	mov	r3, sl
	cmp	r3, #2
	bhi.n	.L_020043bc
	movs	r2, #1
	add	sl, r2
	b.n	.L_02004356
.L_020043bc:
	mov	r3, r8
	adds	r3, #3
	movs	r2, #3
	movs	r1, #1
	str	r2, [sp, #0]
	str	r1, [sp, #4]
	movs	r2, #54
	adds	r1, r3, #0
	movs	r0, #48
	bl 0x0200ce1c
	movs	r3, #128
	movs	r2, #1
	lsls	r3, r3, #13
	add	r8, r2
	add	fp, r3
	mov	r3, r8
	cmp	r3, #9
	bls.n	.L_0200431a
	movs	r5, #5
	movs	r6, #2
	movs	r0, #111
	movs	r1, #5
	movs	r2, #117
	movs	r3, #5
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ce1c
	movs	r0, #111
	movs	r1, #10
	movs	r2, #117
	movs	r3, #10
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ce1c
	movs	r0, #111
	movs	r1, #7
	movs	r2, #111
	movs	r3, #5
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ce1c
	movs	r2, #111
	movs	r0, #111
	movs	r1, #7
	movs	r3, #10
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ce1c
	movs	r2, #0
	mov	r8, r2
	mov	sl, r9
	mov	fp, r2
.L_0200442e:
	bl 0x0200cdb4
	ldr	r2, [pc, #224]
	lsls	r0, r0, #1
	lsrs	r0, r0, #16
	adds	r3, r0, #0
	muls	r3, r2
	ldr	r2, [pc, #216]
	adds	r3, r3, r2
	mov	r2, sl
	str	r3, [r2, #8]
	bl 0x0200cdb4
	ldr	r2, [pc, #200]
	lsls	r0, r0, #1
	lsrs	r0, r0, #16
	adds	r3, r0, #0
	muls	r3, r2
	ldr	r2, [pc, #196]
	adds	r3, r3, r2
	mov	r2, sl
	str	r3, [r2, #12]
	bl 0x0200cdb4
	movs	r3, #248
	lsls	r0, r0, #12
	lsls	r3, r3, #8
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	mov	r2, sl
	mov	r3, r8
	strh	r0, [r2, #34]
	movs	r6, #0
	cmp	r3, #7
	bhi.n	.L_020044ba
	movs	r5, #192
	lsls	r5, r5, #14
	movs	r7, #0
	add	r5, fp
.L_0200447c:
	bl 0x0200cdb4
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsrs	r3, r3, #16
	movs	r2, #192
	lsls	r2, r2, #18
	lsls	r3, r3, #19
	adds	r3, r3, r2
	movs	r2, #136
	lsls	r2, r2, #16
	str	r2, [sp, #8]
	mov	r2, r9
	str	r2, [sp, #12]
	adds	r0, r3, #0
	adds	r2, r5, #0
	movs	r3, #0
	movs	r1, #0
	str	r7, [sp, #0]
	str	r7, [sp, #4]
	bl 0x02008118
.L_020044a8:
	movs	r3, #128
	lsls	r3, r3, #11
	adds	r6, #1
	adds	r5, r5, r3
	cmp	r6, #3
	bhi.n	.L_020044ba
	mov	r2, r8
	cmp	r2, #7
	bls.n	.L_0200447c
.L_020044ba:
	movs	r0, #3
	bl 0x0200cda4
	mov	r1, r8
	mov	r3, r8
	movs	r2, #3
	movs	r0, #1
	adds	r3, #3
	str	r2, [sp, #0]
	str	r0, [sp, #4]
	movs	r2, #48
	adds	r1, #26
	movs	r0, #55
	bl 0x0200ce1c
	movs	r3, #128
	movs	r2, #1
	lsls	r3, r3, #13
	add	r8, r2
	add	fp, r3
	mov	r3, r8
	cmp	r3, #9
	bls.n	.L_0200442e
	ldr	r0, [pc, #48]
	bl 0x0200cffc
	movs	r0, #60
	bl 0x0200ce8c
	movs	r0, #21
	bl 0x0200cfa4
	bl 0x0200ce9c
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.4byte 0x00004ccc
	.4byte 0x00017ffc
	.2byte 0x0121
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #184]
	sub	sp, #56
	add	r2, sp, #16
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	mov	r8, r2
	adds	r7, r0, #0
	mov	sl, r1
	bl 0x0200cdb4
	movs	r3, #248
	lsls	r0, r0, #12
	lsls	r3, r3, #8
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	mov	r2, r8
	ldr	r3, [pc, #156]
	strh	r0, [r2, #34]
	ldr	r6, [r3, #0]
	movs	r3, #3
	ands	r6, r3
	cmp	r6, #0
	bne.n	.L_020045d6
	bl 0x0200cdb4
	lsls	r0, r0, #1
	lsrs	r5, r0, #16
	cmp	r5, #0
	beq.n	.L_020045aa
	bl 0x0200cdb4
	adds	r5, r0, #0
	bl 0x0200cdb4
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsrs	r0, r0, #16
	movs	r3, #224
	lsls	r3, r3, #11
	lsls	r0, r0, #16
	adds	r0, r0, r3
	movs	r1, #10
	bl 0x0200cd9c
	lsls	r5, r5, #1
	mov	r3, sl
	lsrs	r5, r5, #16
	lsls	r5, r5, #4
	lsls	r2, r3, #19
	movs	r3, #136
	lsls	r3, r3, #16
	adds	r5, r7, r5
	lsls	r5, r5, #16
	str	r3, [sp, #8]
	mov	r3, r8
	str	r0, [sp, #4]
	str	r3, [sp, #12]
	adds	r0, r5, #0
	movs	r1, #0
	movs	r3, #0
	str	r6, [sp, #0]
	bl 0x02008118
	b.n	.L_020045d6
.L_020045aa:
	bl 0x0200cdb4
	adds	r3, r0, #0
	lsls	r0, r3, #4
	adds	r0, r0, r3
	ldr	r3, [pc, #52]
	lsls	r2, r7, #19
	adds	r2, r2, r3
	movs	r3, #136
	lsls	r3, r3, #16
.L_020045be:
	lsrs	r0, r0, #16
	adds	r0, r7, r0
	str	r3, [sp, #8]
	mov	r3, r8
	str	r3, [sp, #12]
	lsls	r0, r0, #16
	movs	r1, #0
	movs	r3, #0
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02008118
.L_020045d6:
	add	sp, #56
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0000b333
	.4byte 0x03001e40
	.2byte 0x0000
	.2byte 0xfffc
	.global Func_020045f0
	.thumb_func
Func_020045f0:
	push {lr}
	movs r0, #216
	lsls r0, r0, #1
	movs r1, #32
	bl 0x0200c520
	pop {r0}
	bx r0
	.global Func_02004600
	.thumb_func
Func_02004600:
	push {lr}
	movs r0, #140
	lsls r0, r0, #2
	movs r1, #44
	bl 0x0200c520
	pop {r0}
	bx r0
	.global Func_02004610
	.thumb_func
Func_02004610:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #396]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #66
	str r2, [r3]
	sub sp, #56
	bl 0x0200ce94
	movs r0, #0
	bl 0x0200ceac
	movs r1, #0
	bl 0x0200ce44
	movs r1, #15
	movs r0, #0
	bl 0x0200cf34
	movs r0, #170
	bl 0x0200cfec
	bl 0x0200cfc4
	bl 0x0200cfd4
	movs r0, #40
	bl 0x0200ce8c
	movs r0, #162
	bl 0x0200cffc
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl 0x0200cf7c
	movs r0, #220
	movs r1, #1
	movs r2, #180
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	bl 0x0200cf84
	movs r2, #16
	movs r3, #0
	add r2, sp
	mov r10, r3
	mov r8, r3
	mov r11, r2
	mov r9, r3
	bl 0x0200cdb4
	ldr r2, [pc, #284]
	lsls r0, r0, #1
	lsrs r0, r0, #16
	adds r3, r0, #0
	muls r3, r2
	ldr r2, [pc, #280]
	adds r3, r3, r2
	str r3, [sp, #24]
	bl 0x0200cdb4
	ldr r2, [pc, #264]
	lsls r0, r0, #1
	lsrs r0, r0, #16
	adds r3, r0, #0
	muls r3, r2
	ldr r2, [pc, #260]
	adds r3, r3, r2
	str r3, [sp, #28]
	bl 0x0200cdb4
	movs r3, #248
	lsls r0, r0, #12
	lsrs r0, r0, #16
.L_020046be:
	lsls r3, r3, #8
	movs r2, #50
	adds r0, r0, r3
	add r2, sp
	strh r0, [r2]
	movs r5, #192
	lsls r5, r5, #16
	movs r6, #0
	movs r7, #0
	add r5, r9
	bl 0x0200cdb4
	adds r3, r0, #0
	lsls r0, r3, #3
	subs r0, r0, r3
	lsrs r0, r0, #16
	movs r3, #208
	lsls r3, r3, #17
	lsls r0, r0, #19
	adds r0, r0, r3
	movs r3, #136
	lsls r3, r3, #16
	mov r2, r11
.L_020046ec:
	str r3, [sp, #8]
	str r2, [sp, #12]
	movs r3, #0
	adds r2, r5, #0
	movs r1, #0
	str r7, [sp, #0]
	str r7, [sp, #4]
	bl 0x02008118
	movs r3, #128
	lsls r3, r3, #11
	adds r6, #1
	adds r5, r5, r3
	cmp r6, #3
	bls 0x0200c6d2
	movs r0, #3
	bl 0x0200cda4
	mov r2, r8
	cmp r2, #3
	bne .L_020046ec_0
	mov r3, r10
	cmp r3, #2
	bhi .L_020046ec_1
	movs r2, #1
	add r10, r2
	b 0x0200c6c8
.L_020046ec_1:
	mov r3, r10
	cmp r3, #3
	bne .L_020046ec_0
	movs r1, #200
	ldr r0, [pc, #140]
	lsls r1, r1, #4
	bl 0x0200cdac
.L_020046ec_0:
	mov r3, r8
	adds r3, #12
	movs r2, #3
	movs r1, #1
	str r2, [sp, #0]
	str r1, [sp, #4]
	movs r2, #26
	adds r1, r3, #0
	movs r0, #53
	bl 0x0200ce1c
	movs r2, #128
	movs r3, #1
	lsls r2, r2, #13
	add r8, r3
	add r9, r2
	mov r2, r8
	cmp r2, #12
	bls 0x0200c68c
	movs r3, #9
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #81
	movs r1, #41
	movs r2, #89
	movs r3, #14
	bl 0x0200ce1c
	bl 0x0200cf8c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl 0x0200cf84
	movs r0, #60
	bl 0x0200ce8c
	ldr r0, [pc, #48]
	bl 0x0200ce7c
	movs r0, #19
	bl 0x0200cfa4
	bl 0x0200ce9c
	sub sp, #-56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0x7ffc
	.2byte 0x0001
	.4byte 0x0200c5f1
	.4byte 0x00000306
	.global Func_020047c0
	.thumb_func
Func_020047c0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #388]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #66
	str r2, [r3]
	sub sp, #56
	bl 0x0200ce94
	movs r0, #0
	bl 0x0200ceac
	movs r1, #0
	bl 0x0200ce44
	movs r1, #15
	movs r0, #0
	bl 0x0200cf34
	movs r0, #170
	bl 0x0200cfec
	bl 0x0200cfc4
	bl 0x0200cfd4
	movs r0, #40
	bl 0x0200ce8c
	movs r0, #162
	bl 0x0200cffc
	movs r0, #128
	movs r1, #128
.L_02004814:
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl 0x0200cf7c
	movs r0, #142
	movs r1, #1
	movs r2, #180
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	bl 0x0200cf84
	movs r2, #16
	movs r3, #0
	add r2, sp
	mov r10, r3
	mov r8, r3
	mov r11, r2
	mov r9, r3
.L_02004814_3:
	bl 0x0200cdb4
	ldr r2, [pc, #276]
	lsls r0, r0, #1
	lsrs r0, r0, #16
	adds r3, r0, #0
	muls r3, r2
	ldr r2, [pc, #272]
	adds r3, r3, r2
	str r3, [sp, #24]
	bl 0x0200cdb4
	ldr r2, [pc, #256]
	lsls r0, r0, #1
	lsrs r0, r0, #16
	adds r3, r0, #0
	muls r3, r2
	ldr r2, [pc, #252]
	adds r3, r3, r2
	str r3, [sp, #28]
	bl 0x0200cdb4
	movs r3, #248
	lsls r0, r0, #12
	lsrs r0, r0, #16
	lsls r3, r3, #8
	movs r2, #50
	adds r0, r0, r3
	add r2, sp
	strh r0, [r2]
.L_02004814_2:
	movs r5, #192
	lsls r5, r5, #16
	movs r6, #0
	movs r7, #0
	add r5, r9
.L_02004814_0:
	bl 0x0200cdb4
	adds r3, r0, #0
	lsls r0, r3, #3
	subs r0, r0, r3
	lsrs r0, r0, #16
	movs r3, #136
	lsls r3, r3, #18
	lsls r0, r0, #19
	adds r0, r0, r3
	movs r3, #136
	lsls r3, r3, #16
	mov r2, r11
	str r3, [sp, #8]
	str r2, [sp, #12]
	movs r3, #0
	adds r2, r5, #0
	movs r1, #0
	str r7, [sp, #0]
	str r7, [sp, #4]
	bl 0x02008118
	movs r3, #128
	lsls r3, r3, #11
	adds r6, #1
	adds r5, r5, r3
	cmp r6, #3
	bls .L_02004814_0
	movs r0, #3
	bl 0x0200cda4
	mov r2, r8
	cmp r2, #3
	bne .L_02004814_1
	mov r3, r10
	cmp r3, #2
	bhi .L_02004814_1
	movs r2, #1
	add r10, r2
	b .L_02004814_2
.L_02004814_1:
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #136]
	bl 0x0200cdac
	mov r3, r8
	adds r3, #12
	movs r2, #3
	movs r1, #1
	str r2, [sp, #0]
	str r1, [sp, #4]
	movs r2, #34
	adds r1, r3, #0
	movs r0, #58
	bl 0x0200ce1c
	movs r3, #128
	movs r2, #1
	lsls r3, r3, #13
	add r8, r2
	add r9, r3
	mov r3, r8
	cmp r3, #12
	bls .L_02004814_3
	movs r3, #5
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #86
	movs r1, #41
	movs r2, #97
	movs r3, #14
	bl 0x0200ce1c
	bl 0x0200cf8c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl 0x0200cf84
	movs r0, #60
	bl 0x0200ce8c
	ldr r0, [pc, #48]
	bl 0x0200ce7c
	movs r0, #20
	bl 0x0200cfa4
	bl 0x0200ce9c
	sub sp, #-56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x1ebc
	.2byte 0x0300
	.4byte 0x00004ccc
	.4byte 0x00017ffc
	.4byte 0x0200c601
	.4byte 0x00000307
	.global Func_02004968
	.thumb_func
Func_02004968:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #56
	add r2, sp, #16
	movs r3, #1
	str r3, [r2]
	movs r3, #5
	str r3, [r2, #4]
	movs r3, #143
	lsls r3, r3, #1
	strh r3, [r2, #24]
	ldr r3, [pc, #160]
	mov r10, r2
	str r3, [r2, #28]
	ldr r2, [pc, #156]
	ldr r7, [r2]
	movs r3, #3
	ands r7, r3
	adds r5, r0, #0
	cmp r7, #0
	bne 0x0200ca14
	ldr r3, [r2]
	movs r2, #7
	ands r3, r2
	cmp r3, #0
	bne .L_02004968_0
	movs r0, #246
	bl 0x0200cffc
.L_02004968_0:
	bl 0x0200cdb4
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	adds r3, r3, r0
	ldr r2, [r5, #8]
	lsrs r3, r3, #16
	subs r3, #24
	mov r8, r2
	lsls r3, r3, #16
	add r8, r3
	bl 0x0200cdb4
	lsls r3, r0, #1
.L_020049c4:
	adds r3, r3, r0
	lsls r3, r3, #4
	adds r3, r3, r0
	lsrs r3, r3, #16
	ldr r6, [r5, #12]
	subs r3, #24
	lsls r3, r3, #16
	adds r6, r6, r3
	bl 0x0200cdb4
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	adds r3, r3, r0
	lsrs r3, r3, #16
	ldr r5, [r5, #16]
	subs r3, #24
	lsls r3, r3, #16
	adds r5, r5, r3
	bl 0x0200cdb4
	lsls r0, r0, #2
	lsrs r0, r0, #16
	movs r3, #128
	lsls r3, r3, #8
	lsls r0, r0, #15
	adds r0, r0, r3
	movs r3, #204
	lsls r3, r3, #14
	mov r2, r10
	str r0, [sp, #0]
	str r3, [sp, #8]
	str r2, [sp, #12]
	mov r0, r8
	adds r1, r6, #0
	adds r2, r5, #0
	movs r3, #0
	str r7, [sp, #4]
	bl 0x02008118
	movs r0, #0
	sub sp, #-56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0xd2cc
	.2byte 0x0200
	.2byte 0x1e40
	.2byte 0x0300
	.global Func_02004a2c
	.thumb_func
Func_02004a2c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #0
	sub sp, #4
	bl 0x0200ceac
	adds r7, r0, #0
	movs r0, #20
	bl 0x0200ceac
	adds r6, r0, #0
	bl 0x0200ce94
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl 0x0200cf84
	bl 0x0200cdf4
	movs r0, #1
	bl 0x0200cda4
	movs r3, #130
	lsls r3, r3, #16
	str r3, [r7, #12]
	movs r3, #128
	movs r2, #0
	lsls r3, r3, #8
	str r3, [r7, #72]
	mov r10, r2
	movs r3, #85
	adds r3, r3, r7
	str r2, [r7, #68]
	mov r2, r10
	strb r2, [r3]
	mov r8, r3
	bl 0x0200cfc4
	bl 0x0200cfd4
	movs r0, #204
	bl 0x0200cffc
	movs r0, #30
	bl 0x0200ce8c
	movs r3, #3
	mov r2, r8
	strb r3, [r2]
	movs r0, #24
	bl 0x0200ce8c
	movs r0, #0
	ldr r1, [pc, #288]
	bl 0x0200cf74
	movs r1, #22
	movs r0, #0
	bl 0x0200cefc
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #254
	ands r3, r2
	mov r2, r8
	strb r3, [r2]
	ldr r2, [pc, #268]
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
	bl 0x0200cda4
	ldr r2, [pc, #244]
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
	bl 0x0200cda4
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
	bl 0x0200cda4
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
	bl 0x0200cda4
	movs r5, #128
	ldr r3, [r6, #12]
.L_02004b2a:
	lsls r5, r5, #9
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r5
	str r3, [r7, #12]
	ldr r3, [r7, #20]
	adds r3, r3, r5
	str r3, [r7, #20]
	mov r2, r8
	mov r3, r10
	strb r3, [r2]
	adds r3, r6, #0
	mov r2, r10
	adds r3, #85
	movs r1, #128
	strb r2, [r3]
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200cf74
	movs r1, #1
	movs r0, #0
	bl 0x0200cefc
	movs r0, #40
	bl 0x0200ce8c
	ldr r3, [pc, #112]
	movs r0, #60
	str r3, [r7, #108]
	bl 0x0200ce8c
	movs r0, #0
	movs r1, #1
	bl 0x0200cf64
	movs r1, #1
	movs r0, #20
	bl 0x0200cf64
	movs r0, #17
	bl 0x0200cffc
	movs r0, #154
	lsls r0, r0, #1
	bl 0x0200cffc
	ldr r0, [pc, #60]
	bl 0x0200ce7c
	movs r2, #0
.L_02004b2a_0:
	ldr r3, [r7, #12]
	adds r3, r3, r5
	str r3, [r7, #12]
	ldr r3, [r7, #20]
	adds r3, r3, r5
	str r3, [r7, #20]
	ldr r3, [r6, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	movs r0, #1
	str r2, [sp, #0]
	bl 0x0200cda4
	ldr r2, [sp, #0]
	adds r2, #1
	cmp r2, #127
	bls .L_02004b2a_0
	movs r0, #21
	bl 0x0200cfa4
	sub sp, #-4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00000101
	.2byte 0x0000
	.2byte 0xfffd
	.2byte 0x0000
	.2byte 0xfffe
	.4byte 0x0200c969
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	sub	sp, #12
	mov	r0, sp
	str	r3, [r0, #0]
	ldr	r1, [pc, #376]
	ldr	r3, [r5, #12]
	adds	r3, r3, r1
	str	r3, [r0, #4]
	ldr	r3, [r5, #16]
	movs	r1, #0
	str	r3, [r0, #8]
	bl 0x0200832c
	adds	r7, r0, #0
	ldr	r6, [r7, #80]
	ldr	r3, [r6, #40]
	movs	r1, #128
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r1, r1, #1
	cmp	r3, r1
	beq.n	.L_02004c0a
	b.n	.L_02004d4c
.L_02004c0a:
	ldr	r2, [r5, #36]
	adds	r4, r2, #0
	cmp	r2, #0
	bge.n	.L_02004c14
	negs	r4, r2
.L_02004c14:
	ldr	r3, [r5, #44]
	adds	r1, r3, #0
	cmp	r3, #0
	bge.n	.L_02004c1e
	negs	r1, r3
.L_02004c1e:
	cmp	r4, r1
	ble.n	.L_02004c38
	adds	r3, r2, #0
	cmp	r3, #0
	bge.n	.L_02004c2c
	ldr	r2, [pc, #312]
	adds	r3, r3, r2
.L_02004c2c:
	cmp	r3, #0
	bge.n	.L_02004c34
	ldr	r4, [pc, #308]
	b.n	.L_02004c4a
.L_02004c34:
	ldr	r4, [pc, #308]
	b.n	.L_02004c4a
.L_02004c38:
	cmp	r3, #0
	bge.n	.L_02004c40
	ldr	r1, [pc, #292]
	adds	r3, r3, r1
.L_02004c40:
	cmp	r3, #0
	bge.n	.L_02004c48
	ldr	r4, [pc, #296]
	b.n	.L_02004c4a
.L_02004c48:
	ldr	r4, [pc, #296]
.L_02004c4a:
	ldrb	r1, [r4, #0]
	adds	r0, r1, #0
	cmp	r0, #0
	beq.n	.L_02004c74
	adds	r2, r6, #0
	adds	r2, #36
	ldrb	r3, [r2, #0]
	cmp	r3, r0
	beq.n	.L_02004c6e
	adds	r6, r2, #0
.L_02004c5e:
	adds	r4, #1
	ldrb	r1, [r4, #0]
	adds	r2, r1, #0
	cmp	r2, #0
	beq.n	.L_02004c74
	ldrb	r3, [r6, #0]
	cmp	r3, r2
	bne.n	.L_02004c5e
.L_02004c6e:
	adds	r3, r1, #0
	cmp	r3, #0
	bne.n	.L_02004c7e
.L_02004c74:
	adds	r0, r5, #0
	ldr	r1, [pc, #256]
	bl 0x0200cddc
	b.n	.L_02004d54
.L_02004c7e:
	ldr	r3, [pc, #252]
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #244]
	cmp	r2, r3
	bne.n	.L_02004ce8
	ldr	r0, [pc, #240]
	movs	r4, #0
	ldr	r6, [r5, #8]
	ldr	r3, [r0, r4]
	asrs	r2, r6, #20
	cmp	r2, r3
	bne.n	.L_02004ca8
	ldr	r3, [r5, #16]
	ldr	r2, [r0, #4]
	asrs	r3, r3, #20
	cmp	r3, r2
	beq.n	.L_02004cc4
.L_02004ca8:
	adds	r4, #1
	cmp	r4, #3
	bhi.n	.L_02004cc4
	lsls	r1, r4, #3
	ldr	r3, [r0, r1]
	asrs	r2, r6, #20
	cmp	r2, r3
	bne.n	.L_02004ca8
	ldr	r3, [r5, #16]
	adds	r2, r1, #4
	ldr	r2, [r0, r2]
	asrs	r3, r3, #20
	cmp	r3, r2
	bne.n	.L_02004ca8
.L_02004cc4:
	movs	r6, #0
	lsls	r4, r4, #2
	b.n	.L_02004cd0
.L_02004cca:
	adds	r3, r1, #1
	str	r3, [r0, r4]
	adds	r6, #1
.L_02004cd0:
	ldr	r0, [pc, #180]
	ldr	r1, [r0, r4]
	ldrb	r2, [r1, #0]
	cmp	r2, #0
	beq.n	.L_02004c74
	ldr	r3, [r7, #80]
	adds	r3, #36
	ldrb	r3, [r3, #0]
	cmp	r2, r3
	bne.n	.L_02004cca
	ldr	r3, [pc, #164]
	b.n	.L_02004d3e
.L_02004ce8:
	ldr	r0, [pc, #164]
	movs	r4, #0
	ldr	r6, [r5, #8]
	ldr	r3, [r0, r4]
	asrs	r2, r6, #20
	cmp	r2, r3
	bne.n	.L_02004d00
	ldr	r3, [r5, #16]
	ldr	r2, [r0, #4]
	asrs	r3, r3, #20
	cmp	r3, r2
	beq.n	.L_02004d1c
.L_02004d00:
	adds	r4, #1
	cmp	r4, #7
	bhi.n	.L_02004d1c
	lsls	r1, r4, #3
	ldr	r3, [r0, r1]
	asrs	r2, r6, #20
	cmp	r2, r3
	bne.n	.L_02004d00
	ldr	r3, [r5, #16]
	adds	r2, r1, #4
	ldr	r2, [r0, r2]
	asrs	r3, r3, #20
	cmp	r3, r2
	bne.n	.L_02004d00
.L_02004d1c:
	movs	r6, #0
	lsls	r4, r4, #2
	b.n	.L_02004d28
.L_02004d22:
	adds	r3, r1, #1
	str	r3, [r0, r4]
	adds	r6, #1
.L_02004d28:
	ldr	r0, [pc, #104]
	ldr	r1, [r0, r4]
	ldrb	r2, [r1, #0]
	cmp	r2, #0
	beq.n	.L_02004c74
	ldr	r3, [r7, #80]
	adds	r3, #36
	ldrb	r3, [r3, #0]
	cmp	r2, r3
	bne.n	.L_02004d22
	ldr	r3, [pc, #88]
.L_02004d3e:
	ldr	r2, [r3, r4]
	lsls	r3, r6, #2
	ldr	r1, [r3, r2]
	adds	r0, r5, #0
	bl 0x0200cddc
	b.n	.L_02004d54
.L_02004d4c:
	ldr	r1, [pc, #40]
	adds	r0, r5, #0
	bl 0x0200cddc
.L_02004d54:
	movs	r0, #0
	add	sp, #12
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0xfff00000
	.4byte 0x0000ffff
	.4byte 0x0200d1a4
	.4byte 0x0200d1a8
	.4byte 0x0200d1ac
	.4byte 0x0200d1b0
	.4byte 0x0200d564
	.4byte 0x02000240
	.4byte 0x000000b9
	.4byte 0x0200d128
	.4byte 0x0200f72c
	.4byte 0x0200f77c
	.4byte 0x0200d164
	.4byte 0x0200f78c
	.4byte 0x0200f7ec
	.include "games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/IMPORT.INC"
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
	.4byte 0x00000030
	.4byte 0x00000029
	.4byte 0x00000034
	.4byte 0x00000029
	.4byte 0x00000030
	.4byte 0x0000002b
	.4byte 0x00000034
	.4byte 0x0000002b
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xffe00000
	.4byte 0xffd00000
	.4byte 0xffc00000
	.4byte 0xffb00000
	.4byte 0xffb00000
	.4byte 0x0000000b
	.4byte 0x00000027
	.4byte 0x0000000e
	.4byte 0x00000027
	.4byte 0x0000000b
	.4byte 0x00000029
	.4byte 0x00000010
	.4byte 0x0000002a
	.4byte 0x0000000a
	.4byte 0x0000002b
	.4byte 0x0000000e
	.4byte 0x0000002b
	.4byte 0x0000000a
	.4byte 0x0000002e
	.4byte 0x00000010
	.4byte 0x0000002e
	.4byte 0x00070605
	.4byte 0x00080604
	.4byte 0x00080703
	.4byte 0x00050403
	.4byte 0x00080706
	.4byte 0x00080706
	.4byte 0x00060504
	.4byte 0x00060504
	.4byte 0x05000007
	.4byte 0x05000800
	.4byte 0x00080700
	.4byte 0x00030003
	.4byte 0x0200d040
	.4byte 0x0200d078
	.4byte 0x0200d0b0
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00002126
	.4byte 0x00000015
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte 0x02008595
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00002126
	.4byte 0x00000022
	.4byte 0x02008595
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x0000000c
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte 0x020085ad
	.4byte 0x80010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00011999
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00011999
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000e666
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000e666
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000c
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000010
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
	.4byte 0x02008691
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000a000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00004000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000a000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x0000000c
	.4byte 0x00000004
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
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000004
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000006
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000007
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000008
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x02008691
	.4byte 0x0000001c
	.4byte 0x00000007
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte 0x02008b99
	.4byte 0x00000003
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000003
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000003
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02e70000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000003
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02e70000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000003
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03480000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03480000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000001b
	.4byte 0x00000003
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000003
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000015
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03080000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03080000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03480000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03080000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000003
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000015
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03480000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x03080000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x01080000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02e80000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00e80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00b80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x01080000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02a80000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00b80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02780000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000003
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000003
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000003
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00e80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02780000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00a80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02e80000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00a80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02b80000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000005
	.4byte 0x00b80000
	.4byte 0x00000015
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000022
	.4byte 0x0200cbd9
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000015
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0032007c
	.4byte 0x00020001
	.4byte 0x007c0006
	.4byte 0x0001002f
	.4byte 0x00060002
	.4byte 0x002c007c
	.4byte 0x00020001
	.4byte 0x007c0006
	.4byte 0x00010029
	.4byte 0x00060002
	.4byte 0x0075ffff
	.4byte 0x0001003b
	.4byte 0x00060002
	.4byte 0x003b0073
	.4byte 0x00020001
	.4byte 0x00710006
	.4byte 0x0001003b
	.4byte 0x00060002
	.4byte 0x003b006f
	.4byte 0x00020001
	.4byte 0xffff0006
	.4byte 0x003a0060
	.4byte 0x00020001
	.4byte 0x005f0006
	.4byte 0x0001003a
	.4byte 0x00060002
	.4byte 0x00380060
	.4byte 0x00020001
	.4byte 0x005f0006
	.4byte 0x00010038
	.4byte 0x00060002
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000028
	.4byte 0x00000108
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00000220
	.4byte 0xffff0002
	.4byte 0x00000178
	.4byte 0x40000058
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00000220
	.4byte 0xffff0003
	.4byte 0x000001e8
	.4byte 0x800001a8
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00000220
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000078
	.4byte 0xc00000c8
	.4byte 0x00000000
	.4byte 0x00f00038
	.4byte 0x000000d8
	.4byte 0xffff0002
	.4byte 0x00000078
	.4byte 0x40000068
	.4byte 0x00000000
	.4byte 0x00f00038
	.4byte 0x000000d8
	.4byte 0xffff0003
	.4byte 0x00000158
	.4byte 0xc00000e8
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0004
	.4byte 0x000001d8
	.4byte 0x400000b8
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0005
	.4byte 0x00000178
	.4byte 0x400000b8
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0006
	.4byte 0x00000158
	.4byte 0x40000068
	.4byte 0x01000000
	.4byte 0x02000030
	.4byte 0x000000f8
	.4byte 0xffff0007
	.4byte 0x00000348
	.4byte 0xc00000f8
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff0008
	.4byte 0x000003c8
	.4byte 0x400000a8
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff0009
	.4byte 0x00000358
	.4byte 0x40000058
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff000a
	.4byte 0x00000328
	.4byte 0x40000058
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff000b
	.4byte 0x000002c8
	.4byte 0x400000a8
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff000c
	.4byte 0x000000c8
	.4byte 0xc0000198
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000208
	.4byte 0xffff000d
	.4byte 0x00000068
	.4byte 0xc00001f8
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000208
	.4byte 0xffff000e
	.4byte 0x00000068
	.4byte 0x400001d8
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000208
	.4byte 0xffff0014
	.4byte 0x00000208
	.4byte 0x400002b8
	.4byte 0x01300000
	.4byte 0x02400158
	.4byte 0x000002d8
	.4byte 0xffff0015
	.4byte 0x00000158
	.4byte 0x40000238
	.4byte 0x01300000
	.4byte 0x02400158
	.4byte 0x000002d8
	.4byte 0xffff0016
	.4byte 0x000002d8
	.4byte 0x400003a8
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff0017
	.4byte 0x000002e8
	.4byte 0x40000318
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff0018
	.4byte 0x00000188
	.4byte 0x400003b8
	.4byte 0x01100000
	.4byte 0x02000328
	.4byte 0x000003d8
	.4byte 0xffff0019
	.4byte 0x00000188
	.4byte 0x40000358
	.4byte 0x01100000
	.4byte 0x02000328
	.4byte 0x000003d8
	.4byte 0xffff001a
	.4byte 0x000002f8
	.4byte 0x400003a8
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff001e
	.4byte 0x00000058
	.4byte 0x40000368
	.4byte 0x00100000
	.4byte 0x01000258
	.4byte 0x00000398
	.4byte 0xffff001f
	.4byte 0x000000b8
	.4byte 0x40000368
	.4byte 0x00100000
	.4byte 0x01000258
	.4byte 0x00000398
	.4byte 0xffff0020
	.4byte 0x000000b8
	.4byte 0x40000288
	.4byte 0x00100000
	.4byte 0x01000258
	.4byte 0x00000398
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000001c8
	.4byte 0x40000228
	.4byte 0x01780000
	.4byte 0x02e00158
	.4byte 0x00000270
	.4byte 0xffff0002
	.4byte 0x000001c8
	.4byte 0xc0000250
	.4byte 0x01780000
	.4byte 0x02e00158
	.4byte 0x00000270
	.4byte 0xffff0003
	.4byte 0x000002b8
	.4byte 0x400001d8
	.4byte 0x01780000
	.4byte 0x02e00158
	.4byte 0x00000270
	.4byte 0xffff0004
	.4byte 0x000002b8
	.4byte 0xc0000200
	.4byte 0x01780000
	.4byte 0x02e00158
	.4byte 0x00000270
	.4byte 0xffff0005
	.4byte 0x000001a8
	.4byte 0x40000088
	.4byte 0x01580000
	.4byte 0x02500030
	.4byte 0x000000d8
	.4byte 0xffff0006
	.4byte 0x00000208
	.4byte 0xc00000b0
	.4byte 0x01580000
	.4byte 0x02500030
	.4byte 0x000000d8
	.4byte 0xffff0007
	.4byte 0x00000068
	.4byte 0xc00002f0
	.4byte 0x00380000
	.4byte 0x01300248
	.4byte 0x00000370
	.4byte 0xffff0008
	.4byte 0x000000c8
	.4byte 0x400002a8
	.4byte 0x00380000
	.4byte 0x01300248
	.4byte 0x00000370
	.4byte 0xffff0009
	.4byte 0x00000058
	.4byte 0x40000158
	.4byte 0x00280000
	.4byte 0x01200118
	.4byte 0x000001f0
	.4byte 0xffff000a
	.4byte 0x000000f8
	.4byte 0xc00001b0
	.4byte 0x00280000
	.4byte 0x01200118
	.4byte 0x000001f0
	.4byte 0xffff000b
	.4byte 0x00000298
	.4byte 0x40000108
	.4byte 0x02680000
	.4byte 0x03e80028
	.4byte 0x00000168
	.4byte 0xffff000c
	.4byte 0x000002d8
	.4byte 0x40000088
	.4byte 0x02680000
	.4byte 0x03e80028
	.4byte 0x00000168
	.4byte 0xffff000d
	.4byte 0x00000348
	.4byte 0x40000088
	.4byte 0x02680000
	.4byte 0x03e80028
	.4byte 0x00000168
	.4byte 0xffff000e
	.4byte 0x000003b8
	.4byte 0x40000088
	.4byte 0x02680000
	.4byte 0x03e80028
	.4byte 0x00000168
	.4byte 0xffff000f
	.4byte 0x00000348
	.4byte 0xc0000168
	.4byte 0x02680000
	.4byte 0x03e80028
	.4byte 0x00000168
	.4byte 0xffff0010
	.4byte 0x00000058
	.4byte 0x40000098
	.4byte 0x00280000
	.4byte 0x01480028
	.4byte 0x000000d0
	.4byte 0xffff0011
	.4byte 0x000000b8
	.4byte 0x40000068
	.4byte 0x00280000
	.4byte 0x01480028
	.4byte 0x000000d0
	.4byte 0xffff0012
	.4byte 0x00000118
	.4byte 0x40000098
	.4byte 0x00280000
	.4byte 0x01480028
	.4byte 0x000000d0
	.4byte 0xffff0013
	.4byte 0x000001d8
	.4byte 0x40000308
	.4byte 0x01a80000
	.4byte 0x02a002b0
	.4byte 0x00000350
	.4byte 0xffff0014
	.4byte 0x00000258
	.4byte 0x40000308
	.4byte 0x01a80000
	.4byte 0x02a002b0
	.4byte 0x00000350
	.4byte 0xffff0015
	.4byte 0x00000348
	.4byte 0x40000088
	.4byte 0x02680000
	.4byte 0x03e80028
	.4byte 0x00000168
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000068
	.4byte 0x40000078
	.4byte 0x00380000
	.4byte 0x01700038
	.4byte 0x00000110
	.4byte 0xffff0002
	.4byte 0x00000138
	.4byte 0x400000c8
	.4byte 0x00380000
	.4byte 0x01700038
	.4byte 0x00000110
	.4byte 0xffff0003
	.4byte 0x00000258
	.4byte 0xc00000d8
	.4byte 0x02280000
	.4byte 0x03500058
	.4byte 0x00000220
	.4byte 0xffff0004
	.4byte 0x000002b8
	.4byte 0x40000138
	.4byte 0x02280000
	.4byte 0x03500058
	.4byte 0x00000220
	.4byte 0xffff0005
	.4byte 0x00000288
	.4byte 0xc0000218
	.4byte 0x02280000
	.4byte 0x03500058
	.4byte 0x00000220
	.4byte 0xffff0006
	.4byte 0x00000308
	.4byte 0x40000218
	.4byte 0x02280000
	.4byte 0x03500058
	.4byte 0x00000220
	.4byte 0xffff0007
	.4byte 0x00000068
	.4byte 0x40000208
	.4byte 0x00380000
	.4byte 0x01600188
	.4byte 0x00000260
	.4byte 0xffff0008
	.4byte 0x00000138
	.4byte 0xc0000248
	.4byte 0x00380000
	.4byte 0x01600188
	.4byte 0x00000260
	.4byte 0xffff0009
	.4byte 0x000002e8
	.4byte 0x400002b8
	.4byte 0x02880000
	.4byte 0x03800278
	.4byte 0x00000320
	.4byte 0xffff000a
	.4byte 0x00000328
	.4byte 0x400002b8
	.4byte 0x02880000
	.4byte 0x03800278
	.4byte 0x00000320
	.4byte 0xffff000b
	.4byte 0x00000138
	.4byte 0x40000218
	.4byte 0x00380000
	.4byte 0x01600188
	.4byte 0x00000260
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000088
	.4byte 0x40000088
	.4byte 0x00080000
	.4byte 0x01000038
	.4byte 0x000000e0
	.4byte 0xffff0002
	.4byte 0x00000088
	.4byte 0xc00000b8
	.4byte 0x00080000
	.4byte 0x01000038
	.4byte 0x000000e0
	.4byte 0xffff0003
	.4byte 0x00000088
	.4byte 0xc0000188
	.4byte 0x00080000
	.4byte 0x01000108
	.4byte 0x000001b0
	.4byte 0xffff0004
	.4byte 0x00000138
	.4byte 0x400002a8
	.4byte 0x00f80000
	.4byte 0x02000258
	.4byte 0x00000300
	.4byte 0xffff0005
	.4byte 0x00000188
	.4byte 0x400002a8
	.4byte 0x00f80000
	.4byte 0x02000258
	.4byte 0x00000300
	.4byte 0xffff0006
	.4byte 0x000001d8
	.4byte 0x400002a8
	.4byte 0x00f80000
	.4byte 0x02000258
	.4byte 0x00000300
	.4byte 0xffff0007
	.4byte 0x00000088
	.4byte 0x40000318
	.4byte 0x00080000
	.4byte 0x010002d8
	.4byte 0x000003a0
	.4byte 0xffff0008
	.4byte 0x00000088
	.4byte 0xc0000388
	.4byte 0x00080000
	.4byte 0x010002d8
	.4byte 0x000003a0
	.4byte 0xffff0009
	.4byte 0x00000138
	.4byte 0xc00000e8
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff000a
	.4byte 0x00000178
	.4byte 0x400000b8
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff000b
	.4byte 0x000001f8
	.4byte 0x400000f8
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff000c
	.4byte 0x000002a8
	.4byte 0x40000078
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff000d
	.4byte 0x00000138
	.4byte 0x400001b8
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff000e
	.4byte 0x00000278
	.4byte 0xc0000188
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff000f
	.4byte 0x000002d8
	.4byte 0x40000278
	.4byte 0x02a80000
	.4byte 0x03a00238
	.4byte 0x00000300
	.4byte 0xffff0010
	.4byte 0x00000378
	.4byte 0x40000278
	.4byte 0x02a80000
	.4byte 0x03a00238
	.4byte 0x00000300
	.4byte 0xffff0011
	.4byte 0x00000088
	.4byte 0x40000238
	.4byte 0x00080000
	.4byte 0x010001e0
	.4byte 0x00000288
	.4byte 0xffff0012
	.4byte 0x00000088
	.4byte 0xc0000258
	.4byte 0x00080000
	.4byte 0x010001e0
	.4byte 0x00000288
	.4byte 0xffff0013
	.4byte 0x000001b8
	.4byte 0xc0000108
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0xffff0014
	.4byte 0x00000238
	.4byte 0xc0000108
	.4byte 0x01080000
	.4byte 0x02d00038
	.4byte 0x000001e0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000078
	.4byte 0x400000f8
	.4byte 0x00480000
	.4byte 0x01400038
	.4byte 0x000001a0
	.4byte 0xffff0002
	.4byte 0x00000118
	.4byte 0xc0000118
	.4byte 0x00480000
	.4byte 0x01400038
	.4byte 0x000001a0
	.4byte 0xffff0003
	.4byte 0x000001d8
	.4byte 0x40000148
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0004
	.4byte 0x00000218
	.4byte 0x400000d8
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0005
	.4byte 0x00000298
	.4byte 0x40000058
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0006
	.4byte 0x000002e8
	.4byte 0x40000098
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0007
	.4byte 0x00000348
	.4byte 0x40000098
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0008
	.4byte 0x000003a8
	.4byte 0xc0000118
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0009
	.4byte 0x000001d8
	.4byte 0x400001d8
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff000a
	.4byte 0x00000268
	.4byte 0xc00001c8
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff000b
	.4byte 0x000002d8
	.4byte 0xc00001c8
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff000c
	.4byte 0x00000338
	.4byte 0xc00001c8
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff000d
	.4byte 0x00000318
	.4byte 0x40000288
	.4byte 0x02c80000
	.4byte 0x03d00248
	.4byte 0x00000360
	.4byte 0xffff000e
	.4byte 0x00000388
	.4byte 0x40000288
	.4byte 0x02c80000
	.4byte 0x03d00248
	.4byte 0x00000360
	.4byte 0xffff000f
	.4byte 0x00000068
	.4byte 0x40000308
	.4byte 0x00380000
	.4byte 0x01300218
	.4byte 0x00000350
	.4byte 0xffff0010
	.4byte 0x000000d8
	.4byte 0x40000258
	.4byte 0x00380000
	.4byte 0x01300218
	.4byte 0x00000350
	.4byte 0xffff0011
	.4byte 0x00000258
	.4byte 0x400002e8
	.4byte 0x01880000
	.4byte 0x02800288
	.4byte 0x00000340
	.4byte 0xffff0012
	.4byte 0x00000218
	.4byte 0x40000148
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0013
	.4byte 0x00000318
	.4byte 0x40000148
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0xffff0014
	.4byte 0x00000298
	.4byte 0x40000088
	.4byte 0x01a80000
	.4byte 0x03e00018
	.4byte 0x00000230
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000b5
	.4byte 0x00120002
	.4byte 0x002010b6
	.4byte 0x0033d002
	.4byte 0x000000b6
	.4byte 0x001020b5
	.4byte 0x002030b6
	.4byte 0x003020b6
	.4byte 0x0041f0b6
	.4byte 0x0051e0b6
	.4byte 0x006070b6
	.4byte 0x007060b6
	.4byte 0x008200b6
	.4byte 0x009160b6
	.4byte 0x00a0c0b6
	.4byte 0x00b0d0b6
	.4byte 0x00c0a0b6
	.4byte 0x00d0b0b6
	.4byte 0x00e180b6
	.4byte 0x0141a0b6
	.4byte 0x015190b6
	.4byte 0x016090b6
	.4byte 0x017100b7
	.4byte 0x0180e0b6
	.4byte 0x019150b6
	.4byte 0x01a140b6
	.4byte 0x01e050b6
	.4byte 0x01f040b6
	.4byte 0x020080b6
	.4byte 0x000000b7
	.4byte 0x001130b7
	.4byte 0x002090b7
	.4byte 0x003010b8
	.4byte 0x004050b7
	.4byte 0x005040b7
	.4byte 0x0060d0b7
	.4byte 0x0070e0b7
	.4byte 0x008040b8
	.4byte 0x009020b7
	.4byte 0x00a0b0b7
	.4byte 0x00b0a0b7
	.4byte 0x00c070b8
	.4byte 0x00d060b7
	.4byte 0x00e070b7
	.4byte 0x00f110b7
	.4byte 0x010170b6
	.4byte 0x0110f0b7
	.4byte 0x012100ad
	.4byte 0x013010b7
	.4byte 0x014140b7
	.4byte 0x0150b0b8
	.4byte 0x000000b8
	.4byte 0x001030b7
	.4byte 0x002030b8
	.4byte 0x003020b8
	.4byte 0x004080b7
	.4byte 0x0050a0b8
	.4byte 0x006110b9
	.4byte 0x0070c0b7
	.4byte 0x008090b8
	.4byte 0x009080b8
	.4byte 0x00a050b8
	.4byte 0x00b150b7
	.4byte 0x000000b9
	.4byte 0x001010ba
	.4byte 0x002040b9
	.4byte 0x003050b9
	.4byte 0x004020b9
	.4byte 0x005030b9
	.4byte 0x006090b9
	.4byte 0x007050ba
	.4byte 0x0080b0b9
	.4byte 0x009060b9
	.4byte 0x00a040ba
	.4byte 0x00b080b9
	.4byte 0x00c070ba
	.4byte 0x00d090ba
	.4byte 0x00e0f0b9
	.4byte 0x00f0e0b9
	.4byte 0x010120b9
	.4byte 0x011060b8
	.4byte 0x012100b9
	.4byte 0x013120ba
	.4byte 0x014130ba
	.4byte 0x000000ba
	.4byte 0x001010b9
	.4byte 0x002030ba
	.4byte 0x003020ba
	.4byte 0x0040a0b9
	.4byte 0x005070b9
	.4byte 0x006110ba
	.4byte 0x0070c0b9
	.4byte 0x008100ba
	.4byte 0x0090d0b9
	.4byte 0x00a0d0ba
	.4byte 0x00b0e0ba
	.4byte 0x00c0f0ba
	.4byte 0x00d0a0ba
	.4byte 0x00e0b0ba
	.4byte 0x00f0c0ba
	.4byte 0x010080ba
	.4byte 0x011060ba
	.4byte 0x012130b9
	.4byte 0x013140b9
	.4byte 0x014140ba
	.4byte 0x015010bb
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x09810098
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x010c0000
	.4byte 0x00024000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00024000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x01ac0000
	.4byte 0x00024000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x008c0000
	.4byte 0x00024000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x00024000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x019c0000
	.4byte 0x00024000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x016c0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x00000070
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x008c0000
	.4byte 0x00024000
	.4byte 0x00000071
	.4byte 0x00000001
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00024000
	.4byte 0x00000070
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x02500000
	.4byte 0x00000000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x0102c000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00024000
	.4byte 0x00000114
	.4byte 0x0200d29c
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00024000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00024000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01024000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01024000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000fd
	.4byte 0x0200d204
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00024000
	.4byte 0x000000fd
	.4byte 0x0200d204
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x01024000
	.4byte 0x000000fd
	.4byte 0x0200d204
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x01024000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0102c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x0200d37c
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x0002c000
	.4byte 0xffff011c
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x0200d3ac
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x0200d3a0
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x0200d388
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x0002c000
	.4byte 0xffff011c
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000101
	.4byte 0x00000007
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0002c000
	.4byte 0x00000101
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0102c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0102c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0102c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0102c000
	.4byte 0x000001f4
	.4byte 0x0200d1e0
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x0200d37c
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x0200d3ac
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x0200d3b8
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x0200d394
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0002c000
	.4byte 0xffff011c
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0x000000f2
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0002c000
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
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008f39
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000266e
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000266f
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008fbd
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002672
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002673
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002674
	.4byte 0x00008d15
	.4byte 0xffff0408
	.4byte 0x02008f39
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002675
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002676
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002677
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002678
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002679
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000267a
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
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000031
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000021
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
	.4byte 0x00000031
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000031
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000031
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000031
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000031
	.4byte 0xffff0018
	.4byte 0x00000018
	.4byte 0x00000021
	.4byte 0xffff0019
	.4byte 0x00000019
	.4byte 0x00000021
	.4byte 0xffff001a
	.4byte 0x0000001a
	.4byte 0x00000021
	.4byte 0xffff001e
	.4byte 0x0000001e
	.4byte 0x00000021
	.4byte 0xffff001f
	.4byte 0x0000001f
	.4byte 0x00000021
	.4byte 0xffff0020
	.4byte 0x00000020
	.4byte 0x00000002
	.4byte 0x0200002d
	.4byte 0x02009151
	.4byte 0x00000002
	.4byte 0xffff002e
	.4byte 0x020096a5
	.4byte 0x00000202
	.4byte 0xffff0032
	.4byte 0x02009629
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000267b
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000267c
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02009781
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000267f
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002680
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002681
	.4byte 0x00000003
	.4byte 0xffff0023
	.4byte 0x02009025
	.4byte 0x00000003
	.4byte 0xffff0029
	.4byte 0x02009219
	.4byte 0x00000013
	.4byte 0x0f370064
	.4byte 0x001000a1
	.4byte 0x00000013
	.4byte 0x0f380065
	.4byte 0x001000ce
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x020094f5
	.4byte 0x00008c15
	.4byte 0x0200000d
	.4byte 0x02009645
	.4byte 0x00009315
	.4byte 0xffff000c
	.4byte 0x02008ff1
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x020090cd
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02009049
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
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
	.4byte 0x00000021
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
	.4byte 0x00000021
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
	.4byte 0x00000021
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000031
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000021
	.4byte 0xffff0013
	.4byte 0x00000013
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x0200bffd
	.4byte 0x00000002
	.4byte 0xffff0023
	.4byte 0x02009809
	.4byte 0x00000202
	.4byte 0xffff001f
	.4byte 0x0200996d
	.4byte 0x00000202
	.4byte 0xffff0020
	.4byte 0x0200996d
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte 0x020088b9
	.4byte 0x00000002
	.4byte 0xffff0020
	.4byte 0x020088a1
	.4byte 0x00000202
	.4byte 0xffff002d
	.4byte 0x020099f1
	.4byte 0x00000013
	.4byte 0x0f340064
	.4byte 0x001000a2
	.4byte 0x00000003
	.4byte 0x0350006e
	.4byte 0x00300000
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x020098f9
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte 0x020098f9
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x020098f9
	.4byte 0x10008c15
	.4byte 0xffff000b
	.4byte 0x0200999d
	.4byte 0x10008c15
	.4byte 0xffff000c
	.4byte 0x0200999d
	.4byte 0x10009315
	.4byte 0xffff000b
	.4byte 0x0200999d
	.4byte 0x10009315
	.4byte 0xffff000c
	.4byte 0x0200999d
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x020099c1
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x020099c1
	.4byte 0x00009315
	.4byte 0xffff000b
	.4byte 0x02009a9d
	.4byte 0x00009315
	.4byte 0xffff000c
	.4byte 0x02009a9d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008755
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
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
	.4byte 0x00000002
	.4byte 0xffff0023
	.4byte 0x02009af1
	.4byte 0x00000202
	.4byte 0x03010024
	.4byte 0x02009f29
	.4byte 0x00000003
	.4byte 0xffff0019
	.4byte 0x02008f1d
	.4byte 0x00008c15
	.4byte 0x03010009
	.4byte 0x02009d49
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008755
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008755
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x02008755
	.4byte 0x00000021
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000021
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
	.4byte 0x00000031
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x0200bffd
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x0200bffd
	.4byte 0x00000202
	.4byte 0xffff0023
	.4byte 0x0200a26d
	.4byte 0x00000003
	.4byte 0xffff0019
	.4byte 0x02008f1d
	.4byte 0x00000013
	.4byte 0x0f350064
	.4byte 0x00100052
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte 0x02009f61
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x02009f61
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x02009f61
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
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
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000031
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
	.4byte 0x00000031
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00004602
	.4byte 0xffff0012
	.4byte 0x0200af31
	.4byte 0x00000202
	.4byte 0xffff0023
	.4byte 0x0200a6e5
	.4byte 0x00008602
	.4byte 0xffff0024
	.4byte 0x02008cc9
	.4byte 0x00000602
	.4byte 0xffff0024
	.4byte 0x02008cc9
	.4byte 0x00000202
	.4byte 0xffff0024
	.4byte 0x0200a6e5
	.4byte 0x00000602
	.4byte 0xffff0025
	.4byte 0x02008cc9
	.4byte 0x00000202
	.4byte 0xffff0025
	.4byte 0x0200a6e5
	.4byte 0x00000002
	.4byte 0xffff0028
	.4byte 0x0200a6f9
	.4byte 0x00000002
	.4byte 0xffff0029
	.4byte 0x0200a90d
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x0200bffd
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte 0x0200bfe5
	.4byte 0x00000002
	.4byte 0xffff0020
	.4byte 0x0200c015
	.4byte 0x00000002
	.4byte 0xffff0021
	.4byte 0x0200c031
	.4byte 0x00000202
	.4byte 0xffff002d
	.4byte 0x0200aee5
	.4byte 0x00000003
	.4byte 0xffff0019
	.4byte 0x02008f1d
	.4byte 0x00000013
	.4byte 0x0f360064
	.4byte 0x00100009
	.4byte 0x00000003
	.4byte 0x0351006e
	.4byte 0x00300000
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte 0x00000000
	.4byte 0x00009315
	.4byte 0xffff000a
	.4byte 0x0200a47d
	.4byte 0x00009315
	.4byte 0xffff000b
	.4byte 0x0200a47d
	.4byte 0x00009315
	.4byte 0xffff000c
	.4byte 0x0200a47d
	.4byte 0x00009315
	.4byte 0xffff000d
	.4byte 0x0200a47d
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte 0x0200ab15
	.4byte 0x00008c15
	.4byte 0xffff0010
	.4byte 0x0200ab15
	.4byte 0x00008c15
	.4byte 0xffff0011
	.4byte 0x0200ab15
	.4byte 0x00008c15
	.4byte 0xffff0012
	.4byte 0x0200ab15
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0200d1b4
	.4byte 0x0200d1b8
	.4byte 0x0200d1bc
	.4byte 0x0200d1c0
	.4byte 0x0200d57c
	.4byte 0x0200d5d4
	.4byte 0x0200d564
	.4byte 0x0200d564
	.4byte 0x0200d60c
	.4byte 0x0200d644
	.4byte 0x0200d564
	.4byte 0x0200d564
	.4byte 0x0200d6b4
	.4byte 0x0200d67c
	.4byte 0x0200d6b4
	.4byte 0x0200d564
	.4byte 0x0200d744
	.4byte 0x0200d70c
	.4byte 0x0200d744
	.4byte 0x0200d564
	.4byte 0x0200f73c
	.4byte 0x0200f74c
	.4byte 0x0200f75c
	.4byte 0x0200f76c
	.4byte 0x0200d1c4
	.4byte 0x0200d1c6
	.4byte 0x0200d1c7
	.4byte 0x0200d1c9
	.4byte 0x0200d1cb
	.4byte 0x0200d1cd
	.4byte 0x0200d1d0
	.4byte 0x0200d1d2
	.4byte 0x0200dc90
	.4byte 0x0200d564
	.4byte 0x0200d564
	.4byte 0x0200d980
	.4byte 0x0200d564
	.4byte 0x0200d9b8
	.4byte 0x0200d564
	.4byte 0x0200dc34
	.4byte 0x0200d564
	.4byte 0x0200d924
	.4byte 0x0200d8a4
	.4byte 0x0200d564
	.4byte 0x0200db90
	.4byte 0x0200d564
	.4byte 0x0200d824
	.4byte 0x0200d564
	.4byte 0x0200f7ac
	.4byte 0x0200f7b4
	.4byte 0x0200f7b8
	.4byte 0x0200f7c0
	.4byte 0x0200f7c8
	.4byte 0x0200f7d0
	.4byte 0x0200f7dc
	.4byte 0x0200f7e4
