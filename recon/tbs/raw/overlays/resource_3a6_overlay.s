.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/ENTRY.INC"
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
	bl 0x02009c44
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
	bl 0x02009c7c
	adds r0, r5, #0
	movs r1, #14
	bl 0x02009d0c
	adds r0, r5, #0
	movs r1, #1
	bl 0x02009c84
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
	bl 0x02009c44
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
	bl 0x02009c7c
	adds r0, r5, #0
	movs r1, #15
	bl 0x02009d0c
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
	bl 0x02009cbc
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
	bl 0x02009c44
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
	bl 0x02009c34
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl 0x02009c3c
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
	bl 0x02009d0c
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
	bl 0x02009c14
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
	bl 0x02009c14
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200013c_9:
	bl 0x02009c14
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
	bl 0x02009c34
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl 0x02009c3c
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
	.4byte 0x02009f60
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
	bl 0x02009d70
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x030001d8
	.global Func_02000350
	.thumb_func
Func_02000350:
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
.L_02000350_4:
	ldmia r1!, {r0}
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r7, r3
	bne .L_02000350_0
	ldr r3, [r4, #4]
	cmp r3, #0
	bge .L_02000350_1
	adds r3, r3, r6
.L_02000350_1:
	asrs r2, r3, #16
	ldr r3, [r0, #12]
	cmp r3, #0
	bge .L_02000350_2
	adds r3, r3, r6
.L_02000350_2:
	asrs r3, r3, #16
	cmp r2, r3
	bne .L_02000350_0
	ldr r2, [r4, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_02000350_3
.L_02000350_0:
	adds r5, #1
	cmp r5, #65
	bls .L_02000350_4
	movs r0, #0
.L_02000350_3:
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x0000ffff
	.global Func_020003a8
	.thumb_func
Func_020003a8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #0
	sub sp, #12
	bl 0x02009cbc
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
	bl 0x02008350
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020003a8_0
	b .L_020003a8_1
.L_020003a8_0:
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
	bl 0x02008350
	cmp r0, #0
	beq .L_020003a8_2
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_020003a8_1
.L_020003a8_2:
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
	bl 0x02008350
	cmp r0, #0
	beq .L_020003a8_3
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_020003a8_1
.L_020003a8_3:
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
	bl 0x02009c74
	cmp r0, #0
	bgt .L_020003a8_1
	adds r3, r6, #0
	adds r3, #98
	ldrb r3, [r3]
	mov r10, r3
	cmp r3, #0
	bne .L_020003a8_1
	movs r1, #8
	mov r0, r8
	bl 0x02009c34
	ldr r5, [pc, #132]
	movs r0, #15
	bl 0x02009c1c
	movs r0, #185
	bl 0x02009d5c
	str r5, [r6, #48]
	str r5, [r6, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl 0x02009c54
	mov r0, r8
	str r5, [r0, #48]
	str r5, [r0, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl 0x02009c54
	adds r0, r6, #0
	bl 0x02009c5c
	bl 0x02009d54
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
	bl 0x02009c34
.L_020003a8_1:
	sub sp, #-12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02009e48
	.4byte 0xffff0000
	.4byte 0x00003333
	.global Func_02000528
	.thumb_func
Func_02000528:
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
	beq .L_02000528_0
	cmp r0, #2
	bhi .L_02000528_1
	lsls r3, r0, #1
	adds r3, r3, r0
	movs r0, #152
	lsls r0, r0, #1
	lsls r3, r3, #4
	adds r3, r3, r0
	ldr r0, [r2, r3]
	b .L_02000528_2
.L_02000528_1:
	ldr r0, [pc, #52]
.L_02000528_2:
	lsls r3, r1, #7
	adds r3, r6, r3
	lsls r3, r3, #2
	movs r1, #0
	adds r0, r0, r3
	cmp r1, r12
	bcs .L_02000528_0
.L_02000528_5:
	lsls r3, r1, #9
	movs r2, #0
	adds r3, r0, r3
	cmp r2, r4
	bcs .L_02000528_3
.L_02000528_4:
	adds r2, #1
	strb r5, [r3, #2]
	adds r3, #4
	cmp r2, r4
	bcc .L_02000528_4
.L_02000528_3:
	adds r1, #1
	cmp r1, r12
	bcc .L_02000528_5
.L_02000528_0:
	movs r0, #0
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x03001e70
	.4byte 0x02010000
	.global Func_0200058c
	.thumb_func
Func_0200058c:
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
	bl 0x02008350
	cmp r0, #0
	beq .L_0200058c_0
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldr r2, [pc, #96]
	movs r1, #0
.L_0200058c_2:
	ldmia r2!, {r3}
	cmp r0, r3
	beq .L_0200058c_1
	adds r1, #1
	cmp r1, #5
	bls .L_0200058c_2
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #56]
	str r3, [r5, #64]
.L_0200058c_0:
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
	bl 0x02009c74
	cmp r0, #0
	ble .L_0200058c_1
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #56]
	str r3, [r5, #64]
.L_0200058c_1:
	movs r0, #0
	sub sp, #-12
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x02009e48
	.4byte 0xffff0000
	.4byte 0x02009e88
	.global Func_02000630
	.thumb_func
Func_02000630:
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
	bl 0x02009cbc
	ldrh r3, [r0, #6]
	ldr r1, [sp, #20]
	lsrs r3, r3, #12
	movs r2, #8
	str r3, [r1]
	adds r5, #52
	str r2, [sp, #8]
	mov r9, r0
	mov r11, r5
.L_02000630_4:
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
.L_02000630_3:
	ldr r1, [sp, #0]
	ldmia r1!, {r3}
	ldr r2, [sp, #4]
	adds r4, r1, #0
	str r4, [sp, #0]
	cmp r2, r3
	bne .L_02000630_0
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
	bgt .L_02000630_0
	cmp r7, r1
	bge .L_02000630_0
	cmp r4, r5
	bgt .L_02000630_0
	cmp r5, r2
	bge .L_02000630_0
	movs r3, #1
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02000630_1
	mov r2, lr
	asrs r3, r2, #20
	cmp r6, r3
	beq .L_02000630_0
	ldr r3, [sp, #8]
	ldr r4, [sp, #16]
	mov r0, r10
	str r3, [r4]
	b .L_02000630_2
.L_02000630_1:
	mov r1, r12
	asrs r3, r1, #20
	cmp r4, r3
	beq .L_02000630_0
	ldr r2, [sp, #8]
	ldr r3, [sp, #16]
	mov r0, r10
	str r2, [r3]
	b .L_02000630_2
.L_02000630_0:
	movs r4, #1
	add r8, r4
	mov r1, r8
	adds r0, #16
	cmp r1, #5
	bls .L_02000630_3
	ldr r3, [sp, #8]
	movs r2, #4
	adds r3, #1
	add r11, r2
	str r3, [sp, #8]
	cmp r3, #65
	bls .L_02000630_4
	movs r0, #0
.L_02000630_2:
	sub sp, #-24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
.L_0200073e:
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x9e88
	.2byte 0x0200
	.2byte 0x9ea0
	.2byte 0x0200
	.2byte 0x9e48
	.2byte 0x0200
	.global Func_02000758
	.thumb_func
Func_02000758:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	sub sp, #32
.L_0200076a:
	movs r3, #0
	add r0, sp, #16
	adds r1, r6, #4
	adds r2, r6, #0
	str r3, [r6, #20]
	bl 0x02008630
	mov r10, r0
	cmp r0, #0
	bne .L_0200076a_0
	movs r0, #0
	b 0x020088cc
.L_0200076a_0:
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
.L_02000796:
	adds r3, r1, #4
	ldr r2, [r5, r3]
	cmp r2, #0
	bge .L_02000796_0
	negs r2, r2
.L_02000796_0:
	adds r3, r1, #0
	adds r3, #12
	ldr r3, [r5, r3]
	cmp r3, #0
	bge .L_02000796_1
	negs r3, r3
.L_02000796_1:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #8]
	ldr r2, [r5, r1]
	cmp r2, #0
	bge .L_02000796_2
	negs r2, r2
.L_02000796_2:
	adds r3, r1, #0
	adds r3, #8
	ldr r3, [r5, r3]
	cmp r3, #0
	bge .L_02000796_3
	negs r3, r3
.L_02000796_3:
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
.L_02000796_9:
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
	bge .L_02000796_4
.L_02000796_8:
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
	bge .L_02000796_5
.L_02000796_7:
	adds r1, r4, #0
	mov r0, r10
	str r4, [sp, #0]
	bl 0x02009c74
	ldr r4, [sp, #0]
	cmp r0, #2
	beq .L_02000796_6
	ldr r3, [r4]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	adds r5, #1
	str r3, [r4]
	cmp r5, r9
	blt .L_02000796_7
.L_02000796_5:
	ldr r3, [r6, #16]
	movs r0, #128
	lsls r0, r0, #13
	ldr r1, [sp, #8]
	adds r3, r3, r0
	adds r7, #1
	str r3, [r6, #16]
	cmp r7, r1
	blt .L_02000796_8
.L_02000796_4:
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
	b .L_02000796_9
.L_02000796_6:
	ldr r2, [sp, #4]
	movs r3, #0
	strb r3, [r2]
	ldr r3, [sp, #12]
	movs r0, #0
	cmp r3, #0
	beq .L_02000796_10
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
.L_02000796_10:
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
	.4byte 0x02009ea0
	.4byte 0x02009e48
	.4byte 0xffff0000
	.global Func_020008ec
	.thumb_func
Func_020008ec:
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
.L_0200090a:
	movs r0, #0
	str r3, [sp, #12]
	bl 0x02009cbc
	ldrh r3, [r0, #6]
	ldr r0, [sp, #76]
	lsrs r3, r3, #12
	mov r8, r3
	bl 0x02009cbc
	ldr r3, [sp, #72]
	ldr r4, [pc, #616]
	lsls r1, r3, #4
	adds r3, r1, #4
	ldr r2, [r4, r3]
	adds r7, r0, #0
	cmp r2, #0
	bge .L_0200090a_0
	negs r2, r2
.L_0200090a_0:
	adds r3, r1, #0
	adds r3, #12
	ldr r3, [r4, r3]
	cmp r3, #0
	bge .L_0200090a_1
	negs r3, r3
.L_0200090a_1:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #8]
	ldr r2, [r4, r1]
	cmp r2, #0
	bge .L_0200090a_2
	negs r2, r2
.L_0200090a_2:
	adds r3, r1, #0
	adds r3, #8
	ldr r3, [r4, r3]
	cmp r3, #0
	bge .L_0200090a_3
	negs r3, r3
.L_0200090a_3:
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
	bl 0x02008528
	adds r2, r5, #0
	movs r0, #0
	adds r1, r6, #0
	bl 0x02009cc4
	movs r1, #8
	movs r0, #0
	bl 0x02009cfc
	movs r0, #15
	bl 0x02009ca4
	mov r4, r11
	ldr r2, [sp, #80]
	ldr r3, [r4]
	subs r1, r2, r3
	cmp r1, #0
	bge .L_0200090a_4
	ldr r0, [pc, #456]
	adds r1, r1, r0
.L_0200090a_4:
	mov r4, r11
	ldr r2, [sp, #88]
	ldr r3, [r4, #8]
	subs r2, r2, r3
	asrs r1, r1, #17
	cmp r2, #0
	bge .L_0200090a_5
	ldr r0, [pc, #440]
	adds r2, r2, r0
.L_0200090a_5:
	asrs r2, r2, #17
	movs r0, #0
	bl 0x02009ce4
	movs r0, #0
	bl 0x02009cbc
	ldr r3, [pc, #424]
	str r3, [r0, #108]
	movs r0, #4
	bl 0x02009ca4
	mov r3, r8
	subs r3, #6
	cmp r3, #7
	bhi .L_0200090a_6
	adds r0, r7, #0
	movs r1, #3
	bl 0x02009c34
	b .L_0200090a_7
.L_0200090a_6:
	adds r0, r7, #0
	movs r1, #2
	bl 0x02009c34
.L_0200090a_7:
	movs r0, #239
	bl 0x02009d5c
	adds r0, r7, #0
	ldr r1, [sp, #80]
	ldr r2, [sp, #84]
	ldr r3, [sp, #88]
	bl 0x02009c54
	movs r0, #0
	bl 0x02009cec
	movs r0, #0
	movs r1, #2
	bl 0x02009cfc
	movs r0, #0
	ldr r1, [pc, #360]
	ldr r2, [pc, #344]
	bl 0x02009cc4
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
	bl 0x02009ce4
	ldr r3, [sp, #92]
	cmp r3, #0
	beq .L_0200090a_8
	bl 0x02009d70
.L_0200090a_8:
	movs r0, #0
	bl 0x02009cec
	movs r1, #1
	movs r0, #0
	bl 0x02009cfc
	movs r0, #0
	bl 0x02009cbc
	movs r2, #0
	str r2, [r0, #108]
	adds r0, r7, #0
	bl 0x02009c5c
	movs r0, #144
	lsls r0, r0, #1
	bl 0x02009d5c
	movs r0, #213
	bl 0x02009d5c
	ldr r3, [sp, #80]
	str r3, [r7, #8]
	ldr r3, [sp, #88]
	str r3, [r7, #16]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	adds r0, r7, #0
	movs r1, #1
	bl 0x02009c34
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
	bl 0x02009c6c
	ldr r0, [sp, #8]
	ldr r1, [sp, #80]
	ldr r2, [sp, #88]
	str r0, [sp, #0]
	movs r5, #255
	mov r3, r9
	movs r0, #0
	str r5, [sp, #4]
	bl 0x02008528
	ldr r3, [sp, #8]
	ldr r1, [sp, #80]
	ldr r2, [sp, #88]
	str r3, [sp, #0]
	movs r0, #2
	mov r3, r9
	str r5, [sp, #4]
	bl 0x02008528
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
	bl 0x02009c6c
	ldr r3, [sp, #8]
	mov r2, r11
	ldr r1, [r2]
	movs r4, #0
	ldr r2, [r2, #8]
	movs r0, #2
	str r3, [sp, #0]
	mov r3, r9
	str r4, [sp, #4]
	bl 0x02008528
	bl 0x02009d54
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
	.4byte 0x02009ea0
	.4byte 0x00001999
	.4byte 0x0001ffff
	.4byte 0x0200858d
	.4byte 0x00004ccc
	.4byte 0x02009e48
	.global Func_02000ba4
	.thumb_func
Func_02000ba4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, [pc, #260]
	ldr r3, [r3]
	sub sp, #32
	mov r10, r3
	bl 0x02009cbc
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r1, [pc, #244]
	movs r5, #0
	ldr r3, [r1, r5]
	cmp r2, r3
	bne .L_02000ba4_0
	add r7, sp, #8
	b .L_02000ba4_1
.L_02000ba4_0:
	add r7, sp, #8
	mov r12, r7
	movs r6, #7
	adds r4, r1, #0
.L_02000ba4_3:
	mov r3, r12
	adds r5, #1
	str r6, [r3]
	cmp r5, #5
	bhi .L_02000ba4_2
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	adds r4, #4
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [r4]
	cmp r2, r3
	bne .L_02000ba4_3
.L_02000ba4_1:
	str r5, [r7]
.L_02000ba4_2:
	ldr r2, [r7]
	cmp r2, #6
	bls .L_02000ba4_4
	movs r0, #0
	b .L_02000ba4_5
.L_02000ba4_4:
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
	bge .L_02000ba4_6
	negs r2, r2
.L_02000ba4_6:
	adds r3, r1, #0
	adds r3, #12
	ldr r3, [r4, r3]
	cmp r3, #0
	bge .L_02000ba4_7
	negs r3, r3
.L_02000ba4_7:
	adds r3, r2, r3
	ldr r0, [r4, r1]
	asrs r3, r3, #4
	mov r8, r3
	adds r6, r0, #0
	cmp r0, #0
	bge .L_02000ba4_8
	negs r6, r0
.L_02000ba4_8:
	adds r3, r1, #0
	adds r3, #8
	ldr r3, [r4, r3]
	cmp r3, #0
	bge .L_02000ba4_9
	negs r3, r3
.L_02000ba4_9:
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
	bl 0x02009c6c
	mov r3, r8
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r5, #255
	str r3, [sp, #0]
	movs r0, #0
	adds r3, r6, #0
	str r5, [sp, #4]
	bl 0x02008528
	mov r3, r8
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r0, #2
	str r3, [sp, #0]
	adds r3, r6, #0
	str r5, [sp, #4]
	bl 0x02008528
	movs r0, #1
.L_02000ba4_5:
	sub sp, #-32
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x03001e70
	.4byte 0x02009e88
	.4byte 0x02009ea0
	.global Func_02000cc0
	.thumb_func
Func_02000cc0:
	push {lr}
	movs r0, #8
	movs r1, #3
	movs r2, #4
	bl 0x02009d4c
	pop {r0}
	bx r0
	.global Func_02000cd0
	.thumb_func
Func_02000cd0:
	push {r5, r6, lr}
	adds r5, r0, #0
	movs r6, #60
.L_02000cd0_1:
	cmp r6, #0
	beq .L_02000cd0_0
	movs r0, #1
	bl 0x02009c1c
	ldr r3, [r5, #12]
	ldr r2, [r5, #20]
	subs r6, #1
	cmp r3, r2
	bgt .L_02000cd0_1
	b .L_02000cd0_2
.L_02000cd0_0:
	ldr r2, [r5, #20]
.L_02000cd0_2:
	str r2, [r5, #12]
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000cf8
	.thumb_func
Func_02000cf8:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #0
	bl 0x02009cbc
	adds r2, r0, #0
	ldr r3, [r5, #16]
	ldr r0, [r2, #16]
	ldr r1, [r2, #8]
	subs r0, r0, r3
	ldr r3, [r5, #8]
	subs r1, r1, r3
	bl 0x02009c2c
	strh r0, [r5, #6]
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02000d20
	.thumb_func
Func_02000d20:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000d20_0
	ldr r0, [pc, #36]
	b .L_02000d20_1
.L_02000d20_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000d20_2
	ldr r0, [pc, #36]
	b .L_02000d20_1
.L_02000d20_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000d20_3
	ldr r0, [pc, #32]
	b .L_02000d20_1
.L_02000d20_3:
	ldr r0, [pc, #32]
.L_02000d20_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000005d
	.4byte 0x02009f9c
	.4byte 0x0000005e
	.4byte 0x0200a014
	.4byte 0x0000005f
	.4byte 0x0200a134
	.4byte 0x02009f6c
	.global Func_02000d74
	.thumb_func
Func_02000d74:
	movs r0, #0
	bx lr
	.global Func_02000d78
	.thumb_func
Func_02000d78:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200a1dc
	.global Func_02000d80
	.thumb_func
Func_02000d80:
	push {lr}
	ldr r3, [pc, #44]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #36]
	cmp r2, r3
	beq 0x02008da8
	ldr r3, [pc, #32]
	cmp r2, r3
	bne .L_02000d80_0
	ldr r0, [pc, #32]
	b 0x02008daa
.L_02000d80_0:
	ldr r3, [pc, #32]
	cmp r2, r3
	bne 0x02008da8
	ldr r0, [pc, #28]
.L_02000da6:
	b .L_02000da6_0
	.2byte 0x4807
.L_02000da6_0:
	pop {r1}
	bx r1
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x005d
	.2byte 0x0000
	.2byte 0x005e
	.2byte 0x0000
	.2byte 0xa2c4
	.2byte 0x0200
	.2byte 0x005f
	.2byte 0x0000
	.2byte 0xa39c
	.2byte 0x0200
	.2byte 0xa234
	.2byte 0x0200
	.global Func_02000dcc
	.thumb_func
Func_02000dcc:
	push {lr}
	sub sp, #8
	movs r3, #15
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #16
	movs r1, #15
	movs r2, #1
	movs r3, #1
	bl 0x02009c64
	sub sp, #-8
	pop {r0}
	bx r0
	.global Func_02000de8
	.thumb_func
Func_02000de8:
	push {lr}
	sub sp, #8
	movs r3, #15
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #16
	movs r1, #17
	movs r2, #1
	movs r3, #1
	bl 0x02009c64
	sub sp, #-8
	pop {r0}
	bx r0
	.global Func_02000e04
	.thumb_func
Func_02000e04:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #56
	bl 0x02009cac
	movs r0, #9
	bl 0x02009cbc
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #23
	beq .L_02000e04_0
	b .L_02000e04_1
.L_02000e04_0:
	movs r1, #180
	movs r2, #166
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x02009ccc
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #10
	movs r0, #0
	bl 0x02009d14
	movs r0, #9
	bl 0x02009cbc
	movs r2, #128
	ldr r3, [r0, #8]
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r0, #8]
	movs r0, #9
	bl 0x02009cbc
	adds r5, r0, #0
	movs r0, #9
	bl 0x02009cbc
	movs r6, #208
	ldr r2, [r0, #16]
	lsls r6, r6, #14
	adds r2, r2, r6
	movs r1, #0
	movs r3, #241
	ldr r0, [r5, #8]
	bl 0x02008048
	mov r10, r0
	movs r0, #9
	bl 0x02009cbc
	movs r3, #128
	ldr r5, [r0, #8]
	lsls r3, r3, #13
	movs r0, #9
	adds r5, r5, r3
	bl 0x02009cbc
	ldr r2, [r0, #16]
	movs r1, #0
	adds r2, r2, r6
	movs r3, #241
	adds r0, r5, #0
	bl 0x02008048
	mov r8, r0
	movs r0, #9
	bl 0x02009cbc
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	ldr r3, [pc, #204]
	add r6, sp, #16
	str r3, [r6, #8]
	str r3, [r6, #12]
	movs r3, #7
	str r3, [r6, #4]
	movs r0, #216
	bl 0x02009d5c
	movs r7, #0
.L_02000e04_2:
	bl 0x02009c24
	lsls r5, r0, #4
	adds r5, r5, r0
	lsrs r5, r5, #16
	movs r2, #184
	lsls r2, r2, #17
	lsls r5, r5, #16
	adds r5, r5, r2
	bl 0x02009c24
	lsls r2, r0, #3
	subs r2, r2, r0
	lsls r2, r2, #1
	lsrs r2, r2, #16
	movs r3, #156
	lsls r3, r3, #18
	lsls r2, r2, #16
.L_02000e04_3:
	adds r2, r2, r3
	movs r3, #0
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r3, #144
	lsls r3, r3, #12
	str r3, [sp, #8]
	movs r1, #0
	movs r3, #0
	adds r0, r5, #0
	str r6, [sp, #12]
	bl 0x0200813c
	movs r0, #9
	bl 0x02009cbc
	ldr r2, [pc, #120]
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	adds r7, #1
	movs r0, #1
	bl 0x02009ca4
	cmp r7, #67
	bls .L_02000e04_2
	movs r3, #23
	movs r2, #39
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #41
	movs r2, #1
	movs r3, #1
	movs r0, #23
	bl 0x02009c6c
	movs r0, #9
	bl 0x02009cbc
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #128
	lsls r0, r0, #2
	bl 0x02009c94
	movs r0, #9
	bl 0x02009cbc
	ldr r3, [pc, #52]
	movs r1, #2
	str r3, [r0, #12]
	movs r0, #9
	bl 0x02009cfc
	mov r0, r10
	bl 0x02009c4c
	mov r0, r8
	bl 0x02009c4c
	movs r0, #30
	bl 0x02009ca4
.L_02000e04_1:
	bl 0x02009cb4
	sub sp, #-56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00009999
	.4byte 0xffff8000
	.4byte 0xfff80000
	.global Func_02000f78
	.thumb_func
Func_02000f78:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #56
	bl 0x02009cac
	movs r0, #10
	bl 0x02009cbc
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #27
	beq .L_02000f78_0
	b 0x020090a6
.L_02000f78_0:
	movs r0, #10
	bl 0x02009cbc
	ldr r2, [pc, #280]
	ldr r5, [r0, #8]
	movs r0, #10
	adds r5, r5, r2
	bl 0x02009cbc
	movs r6, #208
	ldr r2, [r0, #16]
	lsls r6, r6, #14
	adds r2, r2, r6
	movs r1, #0
	movs r3, #241
	adds r0, r5, #0
	bl 0x02008048
	mov r10, r0
	movs r0, #10
	bl 0x02009cbc
	movs r3, #128
	ldr r5, [r0, #8]
	lsls r3, r3, #12
	movs r0, #10
	adds r5, r5, r3
	bl 0x02009cbc
	ldr r2, [r0, #16]
	movs r1, #0
	adds r2, r2, r6
	movs r3, #241
	adds r0, r5, #0
	bl 0x02008048
	mov r8, r0
	movs r0, #9
	bl 0x02009cbc
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	ldr r3, [pc, #204]
	add r6, sp, #16
	str r3, [r6, #8]
	str r3, [r6, #12]
	movs r3, #7
	str r3, [r6, #4]
	movs r0, #216
	bl 0x02009d5c
	movs r7, #0
.L_02000f78_1:
	bl 0x02009c24
	lsls r5, r0, #4
	adds r5, r5, r0
	lsrs r5, r5, #16
	movs r2, #216
	lsls r2, r2, #17
	lsls r5, r5, #16
	adds r5, r5, r2
	bl 0x02009c24
	lsls r2, r0, #3
	subs r2, r2, r0
	lsls r2, r2, #1
	lsrs r2, r2, #16
	movs r3, #164
	lsls r3, r3, #18
	lsls r2, r2, #16
.L_02000f78_2:
	adds r2, r2, r3
	movs r3, #0
	str r3, [sp, #0]
.L_0200102a:
	str r3, [sp, #4]
	movs r3, #144
	lsls r3, r3, #12
	str r3, [sp, #8]
	movs r1, #0
	movs r3, #0
	adds r0, r5, #0
	str r6, [sp, #12]
	bl 0x0200813c
	movs r0, #10
	bl 0x02009cbc
	ldr r2, [pc, #120]
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	adds r7, #1
	movs r0, #1
	bl 0x02009ca4
	cmp r7, #67
	bls 0x02009000
	movs r3, #27
	movs r2, #41
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #39
	movs r2, #2
	movs r3, #1
	movs r0, #31
	bl 0x02009c6c
	movs r0, #10
	bl 0x02009cbc
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	ldr r0, [pc, #68]
	bl 0x02009c94
	movs r0, #10
	bl 0x02009cbc
	ldr r3, [pc, #44]
	movs r1, #2
	str r3, [r0, #12]
	movs r0, #10
	bl 0x02009cfc
	mov r0, r10
	bl 0x02009c4c
	mov r0, r8
	bl 0x02009c4c
	movs r0, #30
	bl 0x02009ca4
	bl 0x02009cb4
	sub sp, #-56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0xfff80000
	.2byte 0x9999
	.2byte 0x0000
	.4byte 0xffff8000
	.4byte 0x00000201
	.global Func_020010c8
	.thumb_func
Func_020010c8:
	push {lr}
	bl 0x02009cac
	bl 0x020083a8
	bl 0x02009cb4
	bl 0x02008f78
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020010e0
	.thumb_func
Func_020010e0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #0
	sub sp, #12
	bl 0x02009cbc
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #85
	ldrb r1, [r7]
	ldr r3, [r5, #8]
	ldr r2, [pc, #160]
	mov r8, r1
	movs r1, #128
	ands r3, r2
	lsls r1, r1, #12
	mov r6, sp
	adds r3, r3, r1
	str r3, [r6]
	ldr r3, [r5, #12]
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	ands r3, r2
	movs r2, #160
	lsls r2, r2, #14
	adds r3, r3, r2
	adds r1, r6, #0
	str r3, [r6, #8]
	bl 0x02009c74
	cmp r0, #0
	bne .L_020010e0_0
	bl 0x02009cac
	movs r1, #6
	adds r0, r5, #0
	bl 0x02009c34
	movs r0, #6
	bl 0x02009c1c
	movs r0, #152
	bl 0x02009d5c
	adds r0, r5, #0
	movs r1, #7
	bl 0x02009c34
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
	bl 0x02009c7c
	movs r3, #10
	ldrsh r2, [r6, r3]
	movs r3, #2
	ldrsh r1, [r6, r3]
	movs r0, #0
	bl 0x02009ccc
	adds r0, r5, #0
	movs r1, #6
	bl 0x02009c34
	adds r0, r5, #0
	movs r1, #1
	bl 0x02009c7c
	mov r1, r8
	strb r1, [r7]
	bl 0x02009cb4
	movs r0, #1
	b .L_020010e0_1
.L_020010e0_0:
	movs r0, #0
.L_020010e0_1:
	sub sp, #-12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0xfff00000
	.global Func_020011a0
	.thumb_func
Func_020011a0:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	bl 0x02009c8c
	cmp r0, #0
	bne 0x02009252
	movs r0, #128
	lsls r0, r0, #2
	bl 0x02009c94
	bl 0x02009cac
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x02009d34
	movs r1, #1
	movs r0, #8
.L_020011ca:
	bl 0x02009d2c
	bl 0x02009d3c
	movs r0, #60
	bl 0x02009ca4
	movs r1, #192
	movs r2, #20
	movs r0, #8
	lsls r1, r1, #8
	bl 0x02009d14
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	bl 0x02009d24
	movs r1, #2
	movs r0, #8
	bl 0x02009d04
	movs r0, #20
	bl 0x02009ca4
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009cc4
	movs r1, #198
	lsls r1, r1, #2
	movs r2, #248
	movs r0, #8
	bl 0x02009cdc
	movs r0, #152
	bl 0x02009d5c
	movs r0, #8
	bl 0x02009cbc
	movs r3, #128
	lsls r3, r3, #12
	movs r1, #198
	movs r2, #140
	str r3, [r0, #40]
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #8
	bl 0x02009cdc
	movs r0, #20
	bl 0x02009ca4
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009d14
	movs r0, #30
	bl 0x02009ca4
	bl 0x02009cb4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001258
	.thumb_func
Func_02001258:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	bl 0x02009c8c
	cmp r0, #0
	beq .L_02001258_0
	ldr r0, [pc, #140]
	bl 0x02009c8c
	cmp r0, #0
	bne .L_02001258_0
	ldr r0, [pc, #128]
	bl 0x02009c94
	ldr r0, [pc, #128]
	bl 0x02009c94
	bl 0x02009cac
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	bl 0x02009d24
	movs r1, #2
	movs r0, #8
	bl 0x02009d04
	movs r0, #20
	bl 0x02009ca4
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x02009cc4
	movs r1, #190
	movs r2, #140
	movs r0, #8
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x02009cdc
	movs r1, #190
	movs r2, #156
	movs r0, #8
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x02009cdc
	movs r1, #198
	movs r2, #156
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #8
	bl 0x02009cdc
	movs r0, #10
	bl 0x02009ca4
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009d14
	movs r0, #8
	bl 0x02009cbc
	ldr r3, [pc, #16]
	str r3, [r0, #108]
	bl 0x02009cb4
.L_02001258_0:
	pop {r0}
	bx r0
	.4byte 0x00000201
	.4byte 0x00000302
	.4byte 0x02008cf9
	.global Func_02001300
	.thumb_func
Func_02001300:
	push {r5, r6, lr}
	ldr r3, [pc, #96]
	ldr r0, [pc, #96]
	ldr r5, [r3]
	bl 0x02009c8c
	cmp r0, #0
	beq .L_02001300_0
	ldr r2, [pc, #88]
	ldr r3, [pc, #92]
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #8
	beq .L_02001300_0
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r0, #8
	movs r2, #0
	ldrsh r5, [r3, r2]
	bl 0x02009cbc
	adds r6, r0, #0
	movs r0, #0
	bl 0x02009cbc
	ldr r3, [r0, #48]
	movs r0, #8
	str r3, [r6, #48]
	bl 0x02009cbc
	adds r6, r0, #0
	movs r0, #0
	bl 0x02009cbc
	ldr r3, [r0, #52]
	subs r5, #45
	str r3, [r6, #52]
	ldr r2, [pc, #36]
	lsls r5, r5, #3
	adds r3, r5, #4
	ldr r1, [r2, r5]
	movs r0, #8
	ldr r2, [r2, r3]
	bl 0x02009cd4
.L_02001300_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x00000302
	.4byte 0x0000024a
	.4byte 0x02000240
	.4byte 0x02009f00
	.global Func_02001378
	.thumb_func
Func_02001378:
	push {lr}
	bl 0x02009cac
	movs r0, #8
	movs r1, #0
	bl 0x02009cfc
	bl 0x02009cb4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001390
	.thumb_func
Func_02001390:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #32
	bl 0x02009cac
	add r7, sp, #8
	adds r0, r7, #0
	bl 0x02008758
	cmp r0, #0
	beq .L_02001390_0
	mov r3, sp
	add r2, sp, #24
	ldmia r2!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r3, [r7, #12]
	ldr r0, [r7]
	ldr r1, [r7, #4]
	ldr r2, [r7, #8]
	bl 0x020088ec
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r3, #17
	bne .L_02001390_0
	movs r1, #3
	ldr r0, [r7, #4]
	bl 0x02009cfc
	ldr r0, [r7, #4]
	bl 0x02009cbc
	movs r6, #0
	adds r0, #85
	strb r6, [r0]
	ldr r0, [r7, #4]
	bl 0x02009cbc
	movs r1, #12
	str r6, [r0, #68]
	movs r2, #0
	negs r1, r1
	ldr r0, [r7, #4]
	bl 0x02009ce4
	ldr r0, [r7, #4]
	bl 0x02009cec
	ldr r0, [r7, #4]
	movs r1, #3
	bl 0x02009cfc
	movs r1, #3
	movs r0, #10
	bl 0x02009d1c
	ldr r0, [r7, #4]
	bl 0x02009cbc
	movs r3, #3
	adds r0, #85
	strb r3, [r0]
	movs r1, #6
	movs r2, #0
	negs r1, r1
	ldr r0, [r7, #4]
	bl 0x02009ce4
	ldr r0, [r7, #4]
	bl 0x02009cbc
	bl 0x02008cd0
	movs r1, #8
	ldr r0, [r7, #4]
	bl 0x02009cfc
	ldr r0, [r7, #4]
	bl 0x02009cbc
	movs r3, #2
	mov r8, r3
	adds r0, #35
	mov r1, r8
	strb r1, [r0]
	ldr r2, [r7, #16]
	ldr r1, [r7, #8]
	asrs r2, r2, #20
	movs r5, #4
	asrs r1, r1, #20
	subs r2, #2
	movs r3, #1
	movs r0, #0
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02008528
	ldr r2, [r7, #16]
	ldr r1, [r7, #8]
	asrs r2, r2, #20
	asrs r1, r1, #20
	subs r2, #2
	movs r3, #1
	movs r0, #2
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02008528
	mov r3, r8
	str r3, [sp, #0]
	movs r1, #16
	movs r2, #18
	movs r3, #1
	movs r0, #2
	str r6, [sp, #4]
	bl 0x02008528
	movs r1, #16
	movs r2, #16
	movs r3, #1
	movs r0, #0
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02008528
	ldr r0, [pc, #24]
	bl 0x02009c94
	movs r0, #240
	bl 0x02009d5c
.L_02001390_0:
	bl 0x02009cb4
	sub sp, #-32
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00000203
	.global Func_020014ac
	.thumb_func
Func_020014ac:
	push {r5, r6, lr}
	sub sp, #8
	bl 0x02009cac
	movs r0, #10
	bl 0x02009ca4
	movs r1, #128
	ldr r2, [pc, #260]
	movs r0, #0
	lsls r1, r1, #8
	bl 0x02009cc4
	movs r1, #8
	movs r0, #0
	bl 0x02009cfc
	movs r0, #15
	bl 0x02009ca4
	movs r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x02009ce4
	movs r0, #4
	bl 0x02009ca4
	movs r0, #144
	lsls r0, r0, #1
	bl 0x02009d5c
	movs r0, #239
	bl 0x02009d5c
	movs r1, #128
	ldr r2, [pc, #204]
	movs r0, #9
	lsls r1, r1, #8
	bl 0x02009cc4
	movs r1, #2
	movs r0, #9
	bl 0x02009cfc
	movs r0, #9
	bl 0x02009cbc
	movs r6, #0
	adds r0, #85
	strb r6, [r0]
	movs r0, #9
	bl 0x02009cbc
	movs r2, #0
	str r6, [r0, #68]
	movs r1, #12
	movs r0, #9
	bl 0x02009ce4
	movs r0, #0
	bl 0x02009cec
	movs r1, #1
	movs r0, #0
	bl 0x02009cfc
	movs r0, #9
	bl 0x02009cec
	movs r0, #144
	lsls r0, r0, #1
	bl 0x02009d5c
	movs r0, #213
	bl 0x02009d5c
	movs r1, #3
	movs r0, #9
	bl 0x02009cfc
	movs r0, #9
	bl 0x02009cbc
	movs r3, #3
	adds r0, #85
	strb r3, [r0]
	movs r2, #0
	movs r1, #6
	movs r0, #9
	bl 0x02009ce4
	movs r0, #9
	bl 0x02009cbc
	bl 0x02008cd0
	movs r0, #9
	movs r1, #8
	bl 0x02009cfc
	movs r1, #3
	movs r0, #9
	bl 0x02009d1c
	movs r0, #9
	bl 0x02009cbc
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	movs r5, #4
	movs r1, #12
	movs r2, #16
	movs r3, #1
	movs r0, #0
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02008528
	movs r1, #13
	movs r2, #16
	movs r3, #1
	movs r0, #0
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02008528
	ldr r0, [pc, #24]
	bl 0x02009c94
	movs r0, #240
	bl 0x02009d5c
	bl 0x02009cb4
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00001999
	.4byte 0x00000202
	.global Func_020015cc
	.thumb_func
Func_020015cc:
	push {r5, lr}
	sub sp, #32
	bl 0x02009cac
	add r5, sp, #8
	adds r0, r5, #0
	bl 0x02008758
	cmp r0, #0
	beq .L_020015cc_0
	mov r2, sp
	add r3, sp, #24
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r3, [r5, #12]
	ldr r2, [r5, #8]
	ldr r0, [r5]
	ldr r1, [r5, #4]
	bl 0x020088ec
	movs r0, #11
	movs r1, #3
	bl 0x02009cfc
	movs r1, #128
	movs r2, #128
	movs r0, #11
	lsls r1, r1, #7
	lsls r2, r2, #8
	bl 0x02009cc4
	movs r2, #16
	negs r2, r2
	movs r1, #0
	movs r0, #11
	bl 0x02009ce4
	movs r0, #45
	bl 0x02009ca4
	movs r0, #240
	bl 0x02009d5c
	movs r1, #8
	movs r0, #11
	bl 0x02009cfc
	movs r0, #11
	bl 0x02009cbc
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	ldr r2, [r5, #16]
	movs r1, #0
	asrs r2, r2, #20
	str r3, [sp, #0]
	str r1, [sp, #4]
	movs r3, #4
	subs r2, #1
	movs r0, #0
	movs r1, #13
	bl 0x02008528
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #20
	bne .L_020015cc_1
	ldr r0, [pc, #64]
	bl 0x02009c94
	b .L_020015cc_0
.L_020015cc_1:
	movs r0, #129
	lsls r0, r0, #2
	bl 0x02009c94
	movs r3, #16
	movs r5, #14
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #17
	movs r2, #2
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02009c6c
	movs r3, #15
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #13
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02009c6c
.L_020015cc_0:
	bl 0x02009cb4
	sub sp, #-32
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000205
	.global Func_0200169c
	.thumb_func
Func_0200169c:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_0200169c_0
	ldr r0, [pc, #36]
	b .L_0200169c_1
.L_0200169c_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_0200169c_2
	ldr r0, [pc, #36]
	b .L_0200169c_1
.L_0200169c_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_0200169c_3
	ldr r0, [pc, #32]
	b .L_0200169c_1
.L_0200169c_3:
	ldr r0, [pc, #32]
.L_0200169c_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000005d
	.4byte 0x0200a420
	.4byte 0x0000005e
	.4byte 0x0200a450
	.4byte 0x0000005f
	.4byte 0x0200a624
	.4byte 0x0200a414
	.global Func_020016f0
	.thumb_func
Func_020016f0:
	push {lr}
	ldr r3, [pc, #68]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r1, r3, r2
	movs r3, #129
	lsls r3, r3, #2
	str r3, [r1]
	ldr r3, [pc, #56]
	ldrsh r2, [r3, r2]
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_020016f0_0
	movs r3, #128
	lsls r3, r3, #1
	str r3, [r1]
	movs r0, #1
	bl 0x02009c1c
	movs r0, #11
	movs r1, #3
	bl 0x02009d1c
	movs r0, #12
	movs r1, #3
	bl 0x02009d1c
	ldr r0, [pc, #24]
	bl 0x02009c9c
.L_020016f0_0:
	bl 0x02009984
	movs r0, #0
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000005d
	.4byte 0x0000012f
	.global Func_02001748
	.thumb_func
Func_02001748:
	push {r5, lr}
	ldr r3, [pc, #32]
	movs r2, #182
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r5, [r3, r2]
	movs r0, #123
	bl 0x02009d5c
	adds r0, r5, #0
	bl 0x02009d44
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.global Func_02001770
	.thumb_func
Func_02001770:
	push {r5, r6, lr}
	sub sp, #8
	movs r0, #0
	movs r6, #23
	movs r5, #34
	movs r1, #34
	movs r2, #13
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009c6c
	ldr r0, [pc, #72]
	bl 0x02009c8c
	cmp r0, #0
	beq .L_02001770_0
	movs r0, #11
	movs r1, #35
	movs r2, #35
	bl 0x02009938
	movs r0, #24
	movs r1, #34
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009c6c
	b .L_02001770_1
.L_02001770_0:
	movs r0, #11
	movs r1, #23
	movs r2, #35
	bl 0x02009938
	movs r3, #35
	str r3, [sp, #0]
	movs r0, #24
	movs r1, #34
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl 0x02009c6c
.L_02001770_1:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000301
	.global Func_020017d8
	.thumb_func
Func_020017d8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	str r2, [sp, #0]
	ldr r3, [pc, #192]
	movs r2, #250
	str r1, [sp, #4]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl 0x02009cbc
	adds r6, r0, #0
	adds r0, r5, #0
	bl 0x02009cbc
	adds r7, r0, #0
	bl 0x02009cac
	ldr r3, [sp, #4]
	lsls r3, r3, #16
	mov r11, r3
	ldr r3, [r6, #8]
	ldr r2, [pc, #156]
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
	bl 0x02009c54
	adds r0, r6, #0
	movs r1, #27
	bl 0x02009c34
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
	bl 0x02009c54
	ldr r3, [sp, #4]
	cmp r3, #0
	blt .L_020017d8_0
	ldr r2, [sp, #0]
	cmp r2, #0
	bge .L_020017d8_1
.L_020017d8_0:
	adds r0, r7, #0
	movs r1, #4
	bl 0x02009c34
	b .L_020017d8_2
.L_020017d8_1:
	adds r0, r7, #0
	movs r1, #3
	bl 0x02009c34
.L_020017d8_2:
	adds r0, r6, #0
	bl 0x02009c5c
	bl 0x02009cb4
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
	.4byte 0x02000240
	.4byte 0xfff00000
	.global Func_020018b4
	.thumb_func
Func_020018b4:
	push {lr}
	movs r0, #241
	bl 0x02009d5c
	movs r0, #11
	movs r1, #112
	movs r2, #0
	bl 0x020097d8
	movs r1, #80
	movs r2, #0
	movs r0, #11
	bl 0x020097d8
	ldr r0, [pc, #24]
	bl 0x02009c94
	movs r0, #2
	bl 0x02009c1c
	bl 0x02009770
	ldr r0, [pc, #12]
	bl 0x02009d5c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000301
	.4byte 0x00000121
	.global Func_020018f4
	.thumb_func
Func_020018f4:
	push {lr}
	movs r0, #241
	bl 0x02009d5c
	movs r1, #112
	negs r1, r1
	movs r0, #11
	movs r2, #0
	bl 0x020097d8
	movs r1, #80
	negs r1, r1
	movs r2, #0
	movs r0, #11
	bl 0x020097d8
	ldr r0, [pc, #24]
	bl 0x02009c9c
	movs r0, #2
	bl 0x02009c1c
	bl 0x02009770
	ldr r0, [pc, #12]
	bl 0x02009d5c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000301
	.4byte 0x00000121
	.global Func_02001938
	.thumb_func
Func_02001938:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r7, r1, #0
	mov r8, r2
	bl 0x02009cbc
	adds r5, r0, #0
	cmp r5, #0
	beq 0x0200997a
	movs r1, #3
	adds r0, r6, #0
	bl 0x02009d1c
	adds r1, r5, #0
	adds r1, #34
	movs r3, #2
	strb r3, [r1]
	adds r1, #1
	ldrb r3, [r1]
	movs r2, #2
	orrs r2, r3
	strb r2, [r1]
	movs r2, #128
	lsls r2, r2, #12
	lsls r3, r7, #20
	adds r3, r3, r2
	mov r1, r8
	str r3, [r5, #8]
	lsls r3, r1, #20
	adds r3, r3, r2
	str r3, [r5, #16]
.L_0200197a:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02001984
	.thumb_func
Func_02001984:
	push {r5, r6, lr}
	ldr r2, [pc, #584]
	movs r0, #224
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r0, #0
	ldrsh r1, [r3, r0]
	ldr r3, [pc, #576]
	sub sp, #8
	cmp r1, r3
	beq .L_02001984_0
	b .L_02001984_1
.L_02001984_0:
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #9
	bls .L_02001984_2
	b .L_02001984_3
.L_02001984_2:
	ldr r2, [pc, #552]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	ldr r1, [sp, #896]
	lsls r0, r0, #8
	ldr r1, [sp, #896]
	lsls r0, r0, #8
	ldr r1, [sp, #896]
	lsls r0, r0, #8
	ldr r1, [sp, #896]
	lsls r0, r0, #8
	ldr r2, [sp, #8]
	lsls r0, r0, #8
	ldr r2, [sp, #8]
	lsls r0, r0, #8
	ldr r2, [sp, #8]
	lsls r0, r0, #8
	ldr r2, [sp, #400]
	lsls r0, r0, #8
	ldr r2, [sp, #400]
	lsls r0, r0, #8
	ldr r2, [sp, #400]
	lsls r0, r0, #8
	movs r0, #15
	movs r1, #3
	bl 0x02009d1c
	movs r0, #13
	movs r1, #3
	bl 0x02009d1c
	movs r0, #240
	movs r2, #232
	lsls r0, r0, #15
	lsls r2, r2, #16
	movs r1, #0
	movs r3, #223
	bl 0x020080a0
	b .L_02001984_3
	.2byte 0x2070
	.2byte 0xf000
	.2byte 0xf942
	.2byte 0x2800
	.2byte 0xd000
	.2byte 0xe0db
	.2byte 0x4873
	.2byte 0xf000
	.2byte 0xf93c
	.2byte 0x2800
	.2byte 0xd100
	.2byte 0xe0d5
	.2byte 0x2080
	.2byte 0x0080
	.2byte 0xf000
	.2byte 0xf939
	.2byte 0x4b6b
	.2byte 0x20e1
	.2byte 0x0040
	.2byte 0x181b
	.2byte 0x2100
	.2byte 0x5e5b
	.2byte 0x2b05
	.2byte 0xd102
	.2byte 0x303f
	.2byte 0xf000
	.2byte 0xf92e
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xf8ef
	.2byte 0x4868
	.2byte 0xf000
	.2byte 0xf924
	.2byte 0x2800
	.2byte 0xd000
	.2byte 0xe0bd
	.2byte 0x21c6
	.2byte 0x228c
	.2byte 0x2008
	.2byte 0x0489
	.2byte 0x0452
	.2byte 0xf000
	.2byte 0xf94e
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xf92f
	.2byte 0x4b61
	.2byte 0x66c3
	.2byte 0xe0b0
	.2byte 0x228a
	.2byte 0x0492
	.2byte 0x2100
	.2byte 0x2314
	.2byte 0x485e
	.2byte 0xf7fe
	.2byte 0xfb17
	.2byte 0x2300
	.2byte 0x2222
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x2017
	.2byte 0x2122
	.2byte 0x220d
	.2byte 0x2303
	.2byte 0xf000
	.2byte 0xf8f3
	.2byte 0xf7ff
	.2byte 0xfe73
	.2byte 0x2080
	.2byte 0x0080
	.2byte 0xf000
	.2byte 0xf8fd
	.2byte 0x2800
	.2byte 0xd009
	.2byte 0x2317
	.2byte 0x2227
	.2byte 0x9300
	.2byte 0x9201
.L_02001984_4:
	.2byte 0x2017
	.2byte 0x2129
	.2byte 0x2201
	.2byte 0x2301
	.2byte 0xf000
	.2byte 0xf8e1
	.2byte 0x4850
	.2byte 0xf000
	.2byte 0xf8ee
	.2byte 0x2800
	.2byte 0xd100
	.2byte 0xe087
	.2byte 0x231b
	.2byte 0x2229
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x201f
	.2byte 0x2127
	.2byte 0x2202
	.2byte 0x2301
	.2byte 0xf000
	.2byte 0xf8d1
	.2byte 0xe07c
.L_02001984_1:
	ldr r3, [pc, #288]
	cmp r1, r3
	bne .L_02001984_3
	movs r0, #225
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #3
	bgt .L_02001984_3
	cmp r3, #1
	blt .L_02001984_3
	adds r0, #64
	bl 0x02009c8c
	cmp r0, #0
	beq .L_02001984_5
	movs r6, #4
	movs r5, #0
	movs r1, #12
	movs r2, #16
	movs r3, #1
	movs r0, #0
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02008528
	movs r0, #0
	movs r1, #13
	movs r2, #16
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02008528
	b .L_02001984_6
.L_02001984_5:
	movs r0, #9
	bl 0x02008ba4
.L_02001984_6:
	ldr r0, [pc, #216]
	bl 0x02009c8c
	cmp r0, #0
	beq .L_02001984_7
	movs r6, #4
	movs r5, #0
	movs r1, #16
	movs r2, #16
	movs r3, #1
	movs r0, #2
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02008528
	movs r0, #0
	movs r1, #16
	movs r2, #16
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02008528
	b .L_02001984_8
.L_02001984_7:
	movs r0, #10
	bl 0x02008ba4
.L_02001984_8:
	ldr r0, [pc, #164]
	bl 0x02009c8c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02001984_9
	movs r3, #2
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #13
	movs r2, #19
	movs r3, #4
	bl 0x02008528
	b .L_02001984_3
.L_02001984_9:
	movs r0, #129
	lsls r0, r0, #2
	bl 0x02009c8c
	cmp r0, #0
	beq .L_02001984_10
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #13
	movs r2, #15
	movs r3, #4
	movs r0, #0
	str r5, [sp, #4]
	bl 0x02008528
	movs r3, #16
	movs r5, #14
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #17
	movs r2, #2
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02009c6c
	movs r3, #15
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #13
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02009c6c
	b .L_02001984_3
.L_02001984_10:
	movs r0, #11
	bl 0x02008ba4
	movs r0, #11
	movs r1, #3
	bl 0x02009d1c
.L_02001984_3:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000005e
	.4byte 0x020099b8
	.2byte 0x0302
	.2byte 0x0000
	.2byte 0x0109
	.2byte 0x0000
	.2byte 0x8cf9
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0x0282
	.2byte 0x0201
	.2byte 0x0000
	.4byte 0x0000005f
	.4byte 0x00000203
	.4byte 0x00000205
	.global Func_02001bfc
	.thumb_func
Func_02001bfc:
	push {lr}
	movs r0, #11
	movs r1, #1
	bl 0x02009cfc
	movs r0, #11
	movs r1, #2
	bl 0x02009cfc
	pop {r0}
	bx r0
	.2byte 0x0000
	.include "games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/IMPORT.INC"
@ The compiler library links here from its licensed container.
	.section .rodata.part1,"a",%progbits
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
	.4byte 0x000002f8
	.4byte 0x00000128
	.4byte 0x000002f8
	.4byte 0x00000118
	.4byte 0x00000308
	.4byte 0x00000118
	.4byte 0x00000318
	.4byte 0x00000118
	.4byte 0x00000328
	.4byte 0x00000118
	.4byte 0x00000338
	.4byte 0x00000118
	.4byte 0x00000338
	.4byte 0x00000128
	.4byte 0x00000338
	.4byte 0x00000138
	.4byte 0x00000328
	.4byte 0x00000138
	.4byte 0x00000318
	.4byte 0x00000138
	.4byte 0x00000308
	.4byte 0x00000138
	.4byte 0x000002f8
	.4byte 0x00000138
	.4byte 0x02009da0
	.4byte 0x02009dd8
	.4byte 0x02009e10
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
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000098
	.4byte 0x00100000
	.4byte 0x01500000
	.4byte 0x00000170
	.4byte 0xffff0001
	.4byte 0x00000068
	.4byte 0xc0000158
	.4byte 0x00100000
	.4byte 0x01500000
	.4byte 0x00000170
	.4byte 0xffff0002
	.4byte 0x00000088
	.4byte 0x400000b8
	.4byte 0x00100000
	.4byte 0x01500000
	.4byte 0x00000170
	.4byte 0xffff0003
	.4byte 0x00000088
	.4byte 0xc0000178
	.4byte 0x00100000
	.4byte 0x01500000
	.4byte 0x00000170
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000148
	.4byte 0xc0000188
	.4byte 0x00300000
	.4byte 0x02200020
	.4byte 0x000001a0
	.4byte 0xffff0002
	.4byte 0x00000138
	.4byte 0x400000a0
	.4byte 0x00300000
	.4byte 0x02200020
	.4byte 0x000001a0
	.4byte 0xffff0003
	.4byte 0x00000078
	.4byte 0x40000078
	.4byte 0x00300000
	.4byte 0x02200020
	.4byte 0x000001a0
	.4byte 0xffff0004
	.4byte 0x000001f8
	.4byte 0x40000088
	.4byte 0x00300000
	.4byte 0x02200020
	.4byte 0x000001a0
	.4byte 0xffff0005
	.4byte 0x00000318
	.4byte 0xc0000168
	.4byte 0x02700000
	.4byte 0x03b00010
	.4byte 0x00000180
	.4byte 0xffff0006
	.4byte 0x000002a8
	.4byte 0x400000f8
	.4byte 0x02700000
	.4byte 0x03b00010
	.4byte 0x00000180
	.4byte 0xffff0007
	.4byte 0x00000388
	.4byte 0x400000e8
	.4byte 0x02700000
	.4byte 0x03b00010
	.4byte 0x00000180
	.4byte 0xffff0008
	.4byte 0x00000198
	.4byte 0xc0000368
	.4byte 0x01380000
	.4byte 0x02c001d8
	.4byte 0x00000380
	.4byte 0xffff0009
	.4byte 0x000001e8
	.4byte 0xc00002e8
	.4byte 0x01380000
	.4byte 0x02c001d8
	.4byte 0x00000380
	.4byte 0xffff000a
	.4byte 0x00000278
	.4byte 0x40000228
	.4byte 0x01380000
	.4byte 0x02c001d8
	.4byte 0x00000380
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000c8
	.4byte 0x400001b0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000c8
	.4byte 0xc00001b0
	.4byte 0x00400000
	.4byte 0x01a00018
	.4byte 0x000001d0
	.4byte 0xffff0002
	.4byte 0x00000158
	.4byte 0x40000078
	.4byte 0x00400000
	.4byte 0x01a00018
	.4byte 0x000001d0
	.4byte 0xffff0003
	.4byte 0x00000078
	.4byte 0xc0000108
	.4byte 0x00400000
	.4byte 0x01a00018
	.4byte 0x000001d0
	.4byte 0xffff0004
	.4byte 0x00000218
	.4byte 0xc0000308
	.4byte 0x01900000
	.4byte 0x038001b8
	.4byte 0x00000320
	.4byte 0xffff0005
	.4byte 0x00000348
	.4byte 0xc0000308
	.4byte 0x01900000
	.4byte 0x038001b8
	.4byte 0x00000320
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000005d
	.4byte 0x00104006
	.4byte 0x0020105e
	.4byte 0x00306006
	.4byte 0x0000005e
	.4byte 0x0010205d
	.4byte 0x0020505e
	.4byte 0x0030805e
	.4byte 0x0040105f
	.4byte 0x0050205e
	.4byte 0x0060905e
	.4byte 0x0070305f
	.4byte 0x0080305e
	.4byte 0x0090605e
	.4byte 0x00a0405f
	.4byte 0x0000005f
	.4byte 0x0010405e
	.4byte 0x0020505f
	.4byte 0x0030705e
	.4byte 0x0040a05e
	.4byte 0x0050205f
	.4byte 0x000001ff
	.4byte 0xffff00f1
	.4byte 0x00000007
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff00f1
	.4byte 0x00000007
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff00f1
	.4byte 0x00000007
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0070005d
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00004000
	.4byte 0xffff00f1
	.4byte 0x00000007
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00024000
	.4byte 0xffff00f1
	.4byte 0x00000007
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x00024000
	.4byte 0xffff00f1
	.4byte 0x00000007
	.4byte 0x01a60000
	.4byte 0x00000000
	.4byte 0x00ce0000
	.4byte 0x00024000
	.4byte 0xffff00f1
	.4byte 0x00000007
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00ce0000
	.4byte 0x01024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x01800000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0070005d
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00004000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02009749
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02009749
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02009749
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
	.4byte 0x00000602
	.4byte 0xffff0014
	.4byte 0x020098b5
	.4byte 0x00008602
	.4byte 0xffff0015
	.4byte 0x020098f5
	.4byte 0x00004602
	.4byte 0xffff0015
	.4byte 0x020090e1
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x02009bfd
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x02009bfd
	.4byte 0x00000202
	.4byte 0x02010018
	.4byte 0x020090c9
	.4byte 0x00000002
	.4byte 0x00700028
	.4byte 0x020091a1
	.4byte 0x00000002
	.4byte 0x00700029
	.4byte 0x02009259
	.4byte 0x00000002
	.4byte 0x0070002d
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x0070002e
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x0070002f
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x00700030
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x00700031
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x00700032
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x00700033
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x00700034
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x00700035
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x00700036
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x00700037
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x00700038
	.4byte 0x02009301
	.4byte 0x00000007
	.4byte 0xffff0008
	.4byte 0x02008cc1
	.4byte 0x00000013
	.4byte 0x0fc20064
	.4byte 0x001000cc
	.4byte 0x00000013
	.4byte 0x0fc30065
	.4byte 0x001000b5
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008de9
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008dcd
	.4byte 0x00008c15
	.4byte 0x02000009
	.4byte 0x02008e05
	.4byte 0x00008c15
	.4byte 0x0201000a
	.4byte 0x02008f79
	.4byte 0x00009115
	.4byte 0xffff0008
	.4byte 0x02009379
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
	.4byte 0x00000202
	.4byte 0xffff0014
	.4byte 0x02009391
	.4byte 0x00000602
	.4byte 0xffff0015
	.4byte 0x020094ad
	.4byte 0x00000202
	.4byte 0xffff0016
	.4byte 0x020095cd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
