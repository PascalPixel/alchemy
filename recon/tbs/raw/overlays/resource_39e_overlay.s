.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/ENTRY.INC"
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
	bl 0x0200c38c
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
	bl 0x0200c3a4
	adds r0, r5, #0
	movs r1, #14
	bl 0x0200c494
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200c3ac
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
	bl 0x0200c38c
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
	bl 0x0200c3a4
	adds r0, r5, #0
	movs r1, #15
	bl 0x0200c494
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
	bl 0x0200c404
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
	bl 0x0200c38c
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
	bl 0x0200c37c
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl 0x0200c384
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
	bl 0x0200c494
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
	bl 0x0200c33c
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
	bl 0x0200c33c
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200013c_9:
	bl 0x0200c33c
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
	bl 0x0200c37c
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl 0x0200c384
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
	.4byte 0x0200c62c
	.4byte 0x02008105
	.4byte 0xffff0000
	.global Func_02000314
	.thumb_func
Func_02000314:
	push {lr}
	movs r1, #0
	bl 0x0200c3a4
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02000324
	.thumb_func
Func_02000324:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #0
	bl 0x0200c404
	adds r2, r0, #0
	ldr r3, [r5, #16]
	ldr r0, [r2, #16]
	ldr r1, [r2, #8]
	subs r0, r0, r3
	ldr r3, [r5, #8]
	subs r1, r1, r3
	bl 0x0200c364
	strh r0, [r5, #6]
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_0200034c
	.thumb_func
Func_0200034c:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_0200034c_0
	ldr r0, [pc, #16]
	b .L_0200034c_1
.L_0200034c_0:
	ldr r0, [pc, #16]
.L_0200034c_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000003c
	.4byte 0x0200c7a8
	.4byte 0x0200c838
	.global Func_0200037c
	.thumb_func
Func_0200037c:
	movs r0, #0
	bx lr
	.global Func_02000380
	.thumb_func
Func_02000380:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200c8c8
	.global Func_02000388
	.thumb_func
Func_02000388:
	push {r5, lr}
	ldr r1, [pc, #112]
	movs r0, #224
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, [pc, #104]
	cmp r2, r3
	bne .L_02000388_0
	ldr r0, [pc, #100]
	b .L_02000388_1
.L_02000388_0:
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #3
	bne .L_02000388_2
	ldr r0, [pc, #88]
	b .L_02000388_1
.L_02000388_2:
	ldr r0, [pc, #88]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_02000388_3
	ldr r0, [pc, #80]
	ldr r1, [pc, #76]
	adds r3, r0, #0
	adds r3, #122
	strh r1, [r3]
	adds r3, #48
	strh r1, [r3]
	adds r2, r0, #0
	movs r3, #144
	adds r2, #200
	lsls r3, r3, #17
	str r3, [r2]
	movs r3, #248
	adds r2, #8
	lsls r3, r3, #16
	str r3, [r2]
	movs r2, #133
	lsls r2, r2, #1
	adds r3, r0, r2
	adds r2, #24
	strh r1, [r3]
	adds r3, r0, r2
	strh r1, [r3]
.L_02000388_3:
	ldr r5, [pc, #36]
	adds r0, r5, #0
	bl 0x0200c3ec
	adds r0, r5, #0
.L_02000388_1:
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000003c
	.4byte 0x0200c8f0
	.4byte 0x0200cae8
	.4byte 0x00000895
	.4byte 0x0200c998
	.global Func_02000414
	.thumb_func
Func_02000414:
	push {lr}
	bl 0x0200c3dc
	ldr r0, [pc, #96]
	bl 0x0200c49c
	movs r0, #137
	lsls r0, r0, #4
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_02000414_0
	ldr r3, [pc, #80]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #4
	strh r3, [r2]
.L_02000414_0:
	movs r1, #0
	movs r0, #8
	bl 0x0200c4a4
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f4
	cmp r0, #0
	bne .L_02000414_1
	movs r0, #137
	lsls r0, r0, #4
	bl 0x0200c3c4
	b .L_02000414_2
.L_02000414_1:
	ldr r3, [pc, #36]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000414_2:
	movs r0, #8
	movs r1, #0
	bl 0x0200c4ac
	bl 0x0200c3e4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000178a
	.4byte 0x03001ebc
	.global Func_02000484
	.thumb_func
Func_02000484:
	push {r5, lr}
	bl 0x0200c3dc
	ldr r0, [pc, #124]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_02000484_0
	ldr r0, [pc, #116]
	bl 0x0200c49c
	movs r1, #0
	movs r0, #12
	bl 0x0200c4bc
	bl 0x0200c3e4
	b .L_02000484_1
.L_02000484_0:
	ldr r0, [pc, #100]
	bl 0x0200c49c
	movs r1, #0
	movs r0, #12
	bl 0x0200c4a4
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f4
	cmp r0, #1
	bne .L_02000484_2
	ldr r5, [pc, #80]
	movs r2, #236
	ldr r3, [r5]
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r2, [r3]
	adds r2, #1
	movs r1, #0
	strh r2, [r3]
	movs r0, #12
	bl 0x0200c4a4
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f4
	cmp r0, #1
	bne .L_02000484_2
	ldr r2, [r5]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000484_2:
	movs r0, #12
	movs r1, #0
	bl 0x0200c4ac
	bl 0x0200c3e4
.L_02000484_1:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000088f
	.4byte 0x000017d6
	.4byte 0x00001794
	.4byte 0x03001ebc
	.global Func_02000518
	.thumb_func
Func_02000518:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	movs r0, #9
	sub sp, #56
	bl 0x0200c404
	adds r6, r0, #0
	bl 0x0200c3dc
	ldr r0, [pc, #280]
	bl 0x0200c49c
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r2, #196
	movs r0, #0
	movs r1, #168
	lsls r2, r2, #1
	bl 0x0200c434
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #20
	movs r0, #0
	bl 0x0200c4c4
	movs r0, #132
	bl 0x0200c57c
	movs r0, #9
	bl 0x0200c404
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r0, #40]
	movs r0, #9
	bl 0x0200c404
	movs r2, #128
	lsls r2, r2, #11
	mov r9, r2
	str r2, [r0, #72]
	movs r1, #192
	movs r2, #192
	movs r0, #9
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200c414
	movs r2, #196
	movs r1, #152
	lsls r2, r2, #1
	movs r0, #9
	bl 0x0200c424
	movs r0, #9
	bl 0x0200c444
	movs r0, #9
	bl 0x0200c404
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #72]
	movs r1, #0
	movs r2, #0
	movs r0, #9
	mov r10, r3
	bl 0x0200c4c4
	movs r0, #132
	bl 0x0200c57c
	add r4, sp, #16
	movs r3, #7
	str r3, [r4, #4]
	ldr r2, [r6, #16]
	mov r8, r4
	movs r3, #128
	mov r4, r10
	ldr r0, [r6, #8]
	ldr r1, [r6, #12]
	movs r5, #0
	str r4, [sp, #8]
	add r2, r9
	mov r4, r8
	lsls r3, r3, #8
	str r5, [sp, #0]
	str r5, [sp, #4]
	str r4, [sp, #12]
	bl 0x0200813c
	ldr r2, [r6, #16]
	mov r3, r10
	ldr r0, [r6, #8]
	ldr r1, [r6, #12]
	mov r4, r8
	str r3, [sp, #8]
	add r2, r9
	movs r3, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	str r4, [sp, #12]
	bl 0x0200813c
	ldr r2, [r6, #16]
	mov r4, r10
	ldr r1, [r6, #12]
	ldr r0, [r6, #8]
	add r2, r9
	str r4, [sp, #8]
	ldr r3, [pc, #72]
	mov r4, r8
	str r5, [sp, #0]
	str r5, [sp, #4]
	str r4, [sp, #12]
	bl 0x0200813c
	movs r0, #30
	bl 0x0200c3d4
	bl 0x0200c56c
	movs r3, #10
	movs r2, #22
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #24
	movs r2, #1
	movs r3, #1
	movs r0, #10
	bl 0x0200c39c
	ldr r0, [pc, #32]
	bl 0x0200c3c4
	bl 0x0200c3e4
	sub sp, #-56
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000017b4
	.4byte 0xffff8000
	.4byte 0x00000892
	.global Func_02000658
	.thumb_func
Func_02000658:
	push {lr}
	sub sp, #8
	bl 0x0200c3dc
	ldr r0, [pc, #176]
	bl 0x0200c3c4
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl 0x0200c47c
	movs r0, #10
	bl 0x0200c3d4
	ldr r0, [pc, #160]
	bl 0x0200c49c
	movs r1, #2
	movs r0, #9
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #128
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200c4c4
	movs r1, #0
	movs r0, #9
	bl 0x0200c4bc
	movs r0, #10
	bl 0x0200c3d4
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #80
	bl 0x0200c4cc
	movs r1, #208
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200c4c4
	movs r1, #2
	movs r0, #9
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl 0x0200c4c4
	movs r1, #3
	movs r0, #9
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r3, #10
	movs r2, #24
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #10
	movs r1, #26
	movs r2, #1
	movs r3, #1
	bl 0x0200c39c
	bl 0x0200c3e4
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x00000894
	.4byte 0x000017b7
	.global Func_0200071c
	.thumb_func
Func_0200071c:
	push {r5, lr}
	bl 0x0200c3dc
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200c3bc
	adds r5, r0, #0
	cmp r5, #0
	beq 0x02008782
	movs r2, #252
.L_02000732:
	movs r1, #168
	lsls r2, r2, #1
	movs r0, #0
	bl 0x0200c434
	movs r0, #5
	bl 0x0200c3d4
	movs r1, #192
	movs r2, #20
	lsls r1, r1, #8
	movs r0, #0
	bl 0x0200c4c4
	movs r0, #8
	bl 0x0200c404
	movs r3, #0
	adds r0, #91
	strb r3, [r0]
	movs r0, #152
	bl 0x0200c57c
	movs r0, #8
	bl 0x0200c404
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r1, #1
	movs r0, #8
	bl 0x0200c454
	movs r0, #30
	bl 0x0200c3d4
	ldr r0, [pc, #788]
	bl 0x0200c49c
	b .L_02000732_0
	.2byte 0x48c4
	.2byte 0xf003
	.2byte 0xfe8a
	.2byte 0x2108
	.2byte 0x2000
	.2byte 0xf003
	.2byte 0xfca6
	.2byte 0x201e
	.2byte 0xf003
	.2byte 0xfe1f
	.2byte 0x2100
	.2byte 0x2008
	.2byte 0xf003
	.2byte 0xfe87
	.2byte 0xf003
	.2byte 0xfcc3
	.2byte 0x2014
	.2byte 0xf003
	.2byte 0xfe16
	.2byte 0x22fc
	.2byte 0x21a8
	.2byte 0x0052
	.2byte 0x2000
	.2byte 0xf003
	.2byte 0xfe40
	.2byte 0x2005
	.2byte 0xf003
	.2byte 0xfe0d
	.2byte 0x21c0
	.2byte 0x2214
	.2byte 0x0209
	.2byte 0x2000
	.2byte 0xf003
	.2byte 0xfe7f
	.2byte 0x2098
	.2byte 0xf003
	.2byte 0xfed8
	.2byte 0x2008
	.2byte 0xf003
	.2byte 0xfe19
	.2byte 0x305b
	.2byte 0x7005
	.2byte 0x2008
	.2byte 0xf003
	.2byte 0xfe14
	.2byte 0x2380
	.2byte 0x031b
	.2byte 0x6283
	.2byte 0x2101
	.2byte 0x2008
	.2byte 0xf003
	.2byte 0xfe35
	.2byte 0x201e
	.2byte 0xf003
	.2byte 0xfdf2
	.2byte 0x2100
	.2byte 0x2008
	.2byte 0xf003
	.2byte 0xfe56
	.2byte 0x2000
	.2byte 0x2100
	.2byte 0xf003
	.2byte 0xfdfa
	.2byte 0x2801
	.2byte 0xd129
	.2byte 0x2102
	.2byte 0x2008
	.2byte 0xf003
	.2byte 0xfe34
	.2byte 0x2014
	.2byte 0xf003
	.2byte 0xfde1
	.2byte 0x2100
	.2byte 0x2008
	.2byte 0xf003
	.2byte 0xfe49
	.2byte 0x2014
	.2byte 0xf003
	.2byte 0xfdda
	.2byte 0x2100
	.2byte 0x2008
	.2byte 0xf003
	.2byte 0xfc5a
	.2byte 0x201e
	.2byte 0xf003
	.2byte 0xfdd3
	.2byte 0x2102
	.2byte 0x2000
	.2byte 0xf003
	.2byte 0xfe1f
	.2byte 0x2032
	.2byte 0xf003
	.2byte 0xfdcc
	.2byte 0xf003
	.2byte 0xfc74
	.2byte 0x201e
	.2byte 0xf003
	.2byte 0xfdc7
	.2byte 0x2008
	.2byte 0x2103
	.2byte 0xf003
	.2byte 0xfe07
	.2byte 0x2008
	.2byte 0x2100
	.2byte 0xf003
	.2byte 0xfe2b
	.2byte 0xe00b
	.2byte 0x4b8f
	.2byte 0x681a
	.2byte 0x23ec
	.2byte 0x005b
	.2byte 0x18d2
	.2byte 0x8813
	.2byte 0x3302
	.2byte 0x8013
	.2byte 0x2008
	.2byte 0x2100
	.2byte 0xf003
	.2byte 0xfe1e
	.2byte 0x2103
	.2byte 0x2008
	.2byte 0xf003
	.2byte 0xfdf2
	.2byte 0x201e
	.2byte 0xf003
	.2byte 0xfdab
	.2byte 0x2180
	.2byte 0x0049
	.2byte 0x223c
	.2byte 0x2008
	.2byte 0xf003
	.2byte 0xfe21
	.2byte 0x4884
	.2byte 0xf003
	.2byte 0xfe06
	.2byte 0x2100
	.2byte 0x2008
	.2byte 0xf003
	.2byte 0xfe06
	.2byte 0x2000
	.2byte 0x2100
	.2byte 0xf003
	.2byte 0xfdaa
	.2byte 0x2801
	.2byte 0xd124
	.2byte 0x223c
	.2byte 0x2008
	.2byte 0x497d
	.2byte 0xf003
	.2byte 0xfe0f
	.2byte 0x2100
	.2byte 0x2008
	.2byte 0xf003
	.2byte 0xfc13
	.2byte 0x201e
	.2byte 0xf003
	.2byte 0xfd8c
	.2byte 0x2102
	.2byte 0x2000
	.2byte 0xf003
	.2byte 0xfdd8
	.2byte 0x2032
	.2byte 0xf003
	.2byte 0xfd85
	.2byte 0xf003
	.2byte 0xfc2d
	.2byte 0x201e
	.2byte 0xf003
	.2byte 0xfd80
	.2byte 0x2008
	.2byte 0x2100
	.2byte 0xf003
	.2byte 0xfde8
	.2byte 0x4b6e
	.2byte 0x681a
	.2byte 0x23ec
	.2byte 0x005b
	.2byte 0x18d2
	.2byte 0x8813
	.2byte 0x3301
	.2byte 0x8013
	.2byte 0xe015
	.2byte 0x4b6a
	.2byte 0x681a
	.2byte 0x23ec
	.2byte 0x005b
	.2byte 0x18d2
	.2byte 0x8813
	.2byte 0x3301
	.2byte 0x8013
	.2byte 0x2014
	.2byte 0xf003
	.2byte 0xfd68
	.2byte 0x2103
	.2byte 0x2008
	.2byte 0xf003
	.2byte 0xfda8
	.2byte 0x2014
	.2byte 0xf003
	.2byte 0xfd61
	.2byte 0x2008
	.2byte 0x2100
	.2byte 0xf003
	.2byte 0xfdc9
	.2byte 0x2014
	.2byte 0xf003
	.2byte 0xfd5a
	.2byte 0x2104
	.2byte 0x2008
	.2byte 0xf003
	.2byte 0xfd9a
	.2byte 0x2014
	.2byte 0xf003
	.2byte 0xfd53
	.2byte 0x2008
	.2byte 0x2100
	.2byte 0xf003
	.2byte 0xfdbb
	.2byte 0x2102
	.2byte 0x2000
	.2byte 0xf003
	.2byte 0xfd9b
	.2byte 0x2014
	.2byte 0xf003
	.2byte 0xfd48
	.2byte 0x2100
	.2byte 0x2008
	.2byte 0xf003
	.2byte 0xfdb0
	.2byte 0x2014
	.2byte 0xf003
	.2byte 0xfd41
	.2byte 0x21c0
	.2byte 0x2008
	.2byte 0x0209
	.2byte 0x2214
	.2byte 0xf003
	.2byte 0xfdb3
	.2byte 0x2008
	.2byte 0x4950
	.2byte 0x4a51
	.2byte 0xf003
	.2byte 0xfd56
	.2byte 0x22e8
	.2byte 0x21a8
	.2byte 0x0052
	.2byte 0x2008
	.2byte 0xf003
	.2byte 0xfd60
	.2byte 0x203c
	.2byte 0xf003
	.2byte 0xfd2d
	.2byte 0x2180
	.2byte 0x2008
	.2byte 0x01c9
	.2byte 0x2228
	.2byte 0xf003
	.2byte 0xfd9f
	.2byte 0x2008
	.2byte 0x2100
	.2byte 0x220a
	.2byte 0xf003
	.2byte 0xfd92
	.2byte 0x2181
	.2byte 0x2000
	.2byte 0x0049
	.2byte 0x223c
	.2byte 0xf003
	.2byte 0xfd98
	.2byte 0x22ec
	.2byte 0x2008
	.2byte 0x21a8
	.2byte 0x0052
	.2byte 0xf003
	.2byte 0xfd46
.L_02000732_0:
	movs r1, #0
	movs r0, #8
	bl 0x0200c4a4
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f4
	cmp r0, #1
	bne .L_02000732_1
	ldr r0, [pc, #236]
	bl 0x0200c49c
	movs r0, #8
	movs r1, #0
	bl 0x0200c4ac
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200c3c4
	b .L_02000732_2
.L_02000732_1:
	ldr r0, [pc, #216]
	bl 0x0200c49c
	movs r0, #30
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl 0x0200c4e4
	movs r0, #8
	movs r1, #1
	bl 0x0200c4fc
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl 0x0200c514
	movs r1, #1
	ldr r0, [pc, #164]
	bl 0x0200c50c
	movs r0, #30
	bl 0x0200c51c
	bl 0x0200c534
	bl 0x0200c4f4
	bl 0x0200c140
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl 0x0200c50c
	movs r0, #30
	bl 0x0200c51c
	movs r0, #8
	movs r1, #0
	bl 0x0200c4ac
	movs r1, #2
	movs r0, #8
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	bl 0x0200c4ac
	movs r0, #0
	movs r1, #1
	bl 0x0200c46c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #0
	bl 0x0200c4d4
	movs r0, #60
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x0200c4b4
	ldr r0, [pc, #64]
	bl 0x0200c3c4
.L_02000732_2:
	movs r0, #8
	movs r1, #5
	bl 0x0200c454
	bl 0x0200c3e4
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000017ac
	.2byte 0x179f
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x17a4
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0x2666
	.2byte 0x0000
	.4byte 0x000017ab
	.4byte 0x000017ad
	.4byte 0x00010003
	.4byte 0x00000891
	.global Func_02000abc
	.thumb_func
Func_02000abc:
	push {lr}
	bl 0x0200c3dc
	ldr r0, [pc, #20]
	bl 0x0200c49c
	movs r1, #0
	movs r0, #8
	bl 0x0200c4bc
	bl 0x0200c3e4
	pop {r0}
	bx r0
	.4byte 0x000017b1
	.global Func_02000adc
	.thumb_func
Func_02000adc:
	push {lr}
	bl 0x0200c3dc
	ldr r0, [pc, #20]
	bl 0x0200c49c
	movs r1, #0
	movs r0, #9
	bl 0x0200c4bc
	bl 0x0200c3e4
	pop {r0}
	bx r0
	.4byte 0x00001825
	.global Func_02000afc
	.thumb_func
Func_02000afc:
	push {r5, lr}
	bl 0x0200c3dc
	movs r0, #12
	bl 0x0200c404
	movs r3, #0
	adds r0, #91
	strb r3, [r0]
	b .L_02000afc_0
.L_02000afc_1:
	movs r0, #1
	bl 0x0200c344
.L_02000afc_0:
	movs r0, #12
	bl 0x0200c404
	ldr r3, [r0, #12]
	cmp r3, #0
	bgt .L_02000afc_1
	movs r0, #12
	bl 0x0200c404
	movs r5, #0
	str r5, [r0, #12]
	movs r0, #12
	bl 0x0200c404
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r0, #60]
	movs r0, #12
	bl 0x0200c404
	str r5, [r0, #40]
	movs r0, #12
	bl 0x0200c404
	movs r3, #1
	adds r0, #91
	strb r3, [r0]
	movs r1, #0
	movs r0, #12
	movs r2, #0
	bl 0x0200c47c
	ldr r0, [pc, #36]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_02000afc_2
	ldr r0, [pc, #28]
	bl 0x0200c49c
	b .L_02000afc_3
.L_02000afc_2:
	ldr r0, [pc, #24]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_02000afc_4
	ldr r0, [pc, #20]
	bl 0x0200c49c
	b .L_02000afc_3
	.2byte 0x0000
	.4byte 0x00000895
	.4byte 0x00001a5b
	.4byte 0x0000089b
	.4byte 0x0000189e
.L_02000afc_4:
	ldr r0, [pc, #52]
	bl 0x0200c49c
.L_02000afc_3:
	movs r1, #0
	movs r0, #12
	bl 0x0200c4ac
	movs r0, #12
	bl 0x0200c404
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r0, #6]
	movs r0, #12
	bl 0x0200c404
	ldr r5, [pc, #16]
	adds r0, #91
	strb r5, [r0]
	ldr r1, [pc, #20]
	movs r0, #12
	bl 0x0200c41c
	bl 0x0200c3e4
	b .L_02000afc_5
	.4byte 0x00000000
	.4byte 0x0000182a
	.4byte 0x0200c638
.L_02000afc_5:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000bd4
	.thumb_func
Func_02000bd4:
	push {lr}
	bl 0x0200c3dc
	ldr r0, [pc, #20]
	bl 0x0200c49c
	movs r1, #0
	movs r0, #15
	bl 0x0200c4bc
	bl 0x0200c3e4
	pop {r0}
	bx r0
	.4byte 0x0000182d
	.global Func_02000bf4
	.thumb_func
Func_02000bf4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #19
	sub sp, #16
	bl 0x0200c404
	movs r6, #8
	adds r7, r0, #0
	mov r8, r6
.L_02000bf4_0:
	ldr r3, [r7, #80]
	lsls r5, r6, #12
	strh r5, [r3, #30]
	mov r0, r8
	bl 0x0200c344
	adds r0, r5, #0
	bl 0x0200c374
	lsls r2, r0, #1
	ldr r3, [r7, #8]
	adds r2, r2, r0
	lsls r2, r2, #1
	adds r3, r3, r2
	str r3, [r7, #8]
	adds r0, r5, #0
	bl 0x0200c36c
	lsls r2, r0, #1
	ldr r3, [r7, #16]
	adds r2, r2, r0
	lsls r2, r2, #1
	adds r3, r3, r2
	str r3, [r7, #16]
	movs r3, #2
	negs r3, r3
	subs r6, #1
	add r8, r3
	cmp r6, #3
	bhi .L_02000bf4_0
	movs r3, #144
	lsls r3, r3, #13
	str r3, [r7, #12]
	str r3, [r7, #60]
	movs r0, #227
	bl 0x0200c57c
	movs r6, #128
	ldr r0, [r7, #8]
	ldr r2, [r7, #16]
	ldr r3, [pc, #92]
	lsls r6, r6, #12
	ldr r4, [pc, #92]
	movs r5, #0
	ldr r1, [r7, #12]
	adds r0, r0, r3
	adds r2, r2, r6
	ldr r3, [pc, #88]
	str r4, [sp, #0]
	str r5, [sp, #4]
	str r5, [sp, #8]
	str r5, [sp, #12]
	bl 0x0200813c
	ldr r2, [r7, #16]
	ldr r4, [pc, #76]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r2, r2, r6
	ldr r3, [pc, #72]
	str r4, [sp, #0]
	str r5, [sp, #4]
	str r5, [sp, #8]
	str r5, [sp, #12]
	bl 0x0200813c
	ldr r0, [r7, #8]
	ldr r2, [r7, #16]
	movs r3, #160
	ldr r4, [pc, #56]
	lsls r3, r3, #12
	ldr r1, [r7, #12]
	adds r0, r0, r3
	adds r2, r2, r6
	ldr r3, [pc, #48]
	str r4, [sp, #0]
	str r5, [sp, #4]
	str r5, [sp, #8]
	str r5, [sp, #12]
	bl 0x0200813c
	sub sp, #-16
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xfff40000
	.4byte 0x00006666
	.4byte 0xffffcccd
	.4byte 0x00004ccc
	.4byte 0xffff3334
	.4byte 0x00003333
	.4byte 0xffff0000
	.global Func_02000cd4
	.thumb_func
Func_02000cd4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #19
	sub sp, #16
	bl 0x0200c404
	movs r6, #8
	adds r7, r0, #0
.L_02000cd4_0:
	ldr r3, [r7, #80]
	movs r0, #12
	lsls r5, r6, #12
	subs r0, r0, r6
	strh r5, [r3, #30]
	lsls r0, r0, #1
	bl 0x0200c344
	adds r0, r5, #0
	bl 0x0200c374
	lsls r2, r0, #1
	ldr r3, [r7, #8]
	adds r2, r2, r0
	lsls r2, r2, #1
	subs r3, r3, r2
	str r3, [r7, #8]
	adds r0, r5, #0
	bl 0x0200c36c
	lsls r2, r0, #1
	ldr r3, [r7, #16]
	adds r2, r2, r0
	lsls r2, r2, #1
	subs r3, r3, r2
	adds r6, #1
	str r3, [r7, #16]
	cmp r6, #12
	bls .L_02000cd4_0
	movs r3, #144
	lsls r3, r3, #13
	str r3, [r7, #12]
	str r3, [r7, #60]
	ldr r3, [pc, #112]
	movs r0, #227
	str r3, [r7, #24]
	bl 0x0200c57c
	ldr r0, [r7, #8]
	ldr r3, [pc, #104]
	ldr r2, [r7, #16]
	movs r6, #128
	lsls r6, r6, #12
	ldr r4, [pc, #100]
	adds r0, r0, r3
	movs r3, #128
	movs r5, #0
	ldr r1, [r7, #12]
	adds r2, r2, r6
	lsls r3, r3, #9
	str r4, [sp, #0]
	str r5, [sp, #4]
	str r5, [sp, #8]
	str r5, [sp, #12]
	mov r8, r4
	bl 0x0200813c
	ldr r2, [r7, #16]
	ldr r4, [pc, #76]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r2, r2, r6
	ldr r3, [pc, #72]
	str r4, [sp, #0]
	str r5, [sp, #4]
	str r5, [sp, #8]
	str r5, [sp, #12]
	bl 0x0200813c
	ldr r0, [r7, #8]
	movs r3, #160
	lsls r3, r3, #12
	ldr r2, [r7, #16]
	adds r0, r0, r3
	ldr r3, [pc, #52]
	ldr r1, [r7, #12]
	adds r2, r2, r6
	str r3, [sp, #0]
	mov r3, r8
	str r5, [sp, #4]
	str r5, [sp, #8]
	str r5, [sp, #12]
	bl 0x0200813c
	sub sp, #-16
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xffff3334
	.4byte 0xfff40000
	.4byte 0x00003333
	.4byte 0x00004ccc
	.4byte 0x0000cccc
	.4byte 0x00006666
	.global Func_02000db4
	.thumb_func
Func_02000db4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #19
	sub sp, #16
	bl 0x0200c404
	movs r2, #128
.L_02000dc6:
	lsls r2, r2, #24
	adds r7, r0, #0
	movs r6, #0
	movs r5, #8
	mov r8, r2
	adds r0, r5, #0
	bl 0x0200c344
	ldr r3, [r7, #16]
	ldr r2, [pc, #156]
	adds r3, r3, r2
	str r3, [r7, #16]
	adds r6, #1
	mov r3, r8
	str r3, [r7, #64]
.L_02000de4:
	subs r5, #2
	cmp r6, #3
	bls 0x02008dd0
	ldr r3, [r7, #80]
	movs r5, #0
	strh r5, [r3, #30]
	movs r0, #227
	bl 0x0200c57c
	ldr r3, [pc, #132]
	ldr r2, [r7, #16]
	mov r8, r3
	ldr r6, [pc, #128]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	add r2, r8
	ldr r3, [pc, #124]
	str r5, [sp, #0]
	str r6, [sp, #4]
	str r5, [sp, #8]
	str r5, [sp, #12]
	bl 0x0200813c
	ldr r2, [r7, #16]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	add r2, r8
	ldr r3, [pc, #108]
	str r5, [sp, #0]
	str r6, [sp, #4]
	str r5, [sp, #8]
	str r5, [sp, #12]
	bl 0x0200813c
	ldr r0, [r7, #8]
	ldr r2, [pc, #96]
	movs r3, #160
	adds r0, r0, r2
	lsls r3, r3, #12
	ldr r2, [r7, #16]
	mov r8, r3
	ldr r6, [pc, #64]
	ldr r3, [pc, #84]
	ldr r1, [r7, #12]
	add r2, r8
	str r5, [sp, #0]
	str r6, [sp, #4]
	str r5, [sp, #8]
	str r5, [sp, #12]
	mov r10, r3
	bl 0x0200813c
	ldr r0, [r7, #8]
	movs r2, #192
	lsls r2, r2, #11
	adds r0, r0, r2
	ldr r2, [r7, #16]
	ldr r1, [r7, #12]
	add r2, r8
	mov r3, r10
	str r5, [sp, #0]
	str r6, [sp, #4]
	str r5, [sp, #8]
	str r5, [sp, #12]
	bl 0x0200813c
	sub sp, #-16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xffff0000
	.4byte 0xfff80000
	.4byte 0xffffcccd
	.4byte 0xffff3334
	.4byte 0x0000cccc
	.4byte 0xfffa0000
	.4byte 0x00003333
	.global Func_02000e94
	.thumb_func
Func_02000e94:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #19
	sub sp, #16
	bl 0x0200c404
	movs r2, #128
	lsls r2, r2, #24
	adds r7, r0, #0
.L_02000eaa:
	movs r6, #0
	movs r5, #8
	mov r8, r2
	adds r0, r5, #0
	bl 0x0200c344
	ldr r3, [r7, #16]
	movs r4, #128
	lsls r4, r4, #9
	adds r3, r3, r4
	mov r2, r8
	adds r6, #1
	str r3, [r7, #16]
	str r2, [r7, #64]
	subs r5, #2
.L_02000ec8:
	cmp r6, #3
	bls 0x02008eb0
	ldr r3, [r7, #80]
	movs r5, #0
	strh r5, [r3, #30]
	ldr r3, [r7, #16]
	movs r4, #192
	lsls r4, r4, #13
	adds r3, r3, r4
	str r3, [r7, #16]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #64]
	movs r0, #227
	bl 0x0200c57c
	movs r6, #192
	ldr r2, [r7, #16]
	ldr r4, [pc, #124]
	lsls r6, r6, #12
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r2, r2, r6
	ldr r3, [pc, #120]
	str r5, [sp, #0]
	str r4, [sp, #4]
	str r5, [sp, #8]
	str r5, [sp, #12]
	mov r8, r4
	bl 0x0200813c
	ldr r2, [r7, #16]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	mov r4, r8
	adds r2, r2, r6
	ldr r3, [pc, #96]
	str r5, [sp, #0]
	str r4, [sp, #4]
	str r5, [sp, #8]
	str r5, [sp, #12]
	bl 0x0200813c
	ldr r0, [r7, #8]
	ldr r2, [pc, #84]
	ldr r3, [pc, #88]
	adds r0, r0, r2
	ldr r2, [r7, #16]
	mov r10, r3
	movs r6, #128
	ldr r1, [r7, #12]
	add r2, r10
	lsls r6, r6, #9
	mov r3, r8
	str r5, [sp, #0]
	str r6, [sp, #4]
	str r5, [sp, #8]
	str r5, [sp, #12]
	bl 0x0200813c
	ldr r0, [r7, #8]
	ldr r2, [r7, #16]
	movs r4, #192
	lsls r4, r4, #11
	ldr r1, [r7, #12]
	adds r0, r0, r4
.L_02000f4c:
	add r2, r10
	mov r3, r8
	str r5, [sp, #0]
	str r6, [sp, #4]
	str r5, [sp, #8]
	str r5, [sp, #12]
	bl 0x0200813c
	sub sp, #-16
	pop {r3, r5}
	mov r8, r3
.L_02000f62:
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x3333
	.2byte 0x0000
	.2byte 0x3334
	.2byte 0xffff
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfffa
	.2byte 0x0000
	.2byte 0xfff8
	.global Func_02000f80
	.thumb_func
Func_02000f80:
	push {lr}
	movs r0, #0
	bl 0x0200c404
	ldrh r2, [r0, #6]
	ldr r0, [pc, #144]
	adds r3, r2, r0
	ldr r0, [pc, #144]
	lsls r3, r3, #16
	ldr r1, [pc, #144]
	cmp r3, r0
	bhi .L_02000f80_0
	movs r0, #15
	movs r1, #216
	movs r2, #168
	bl 0x0200c434
	movs r0, #15
	movs r1, #224
	movs r2, #168
	bl 0x0200c434
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #6
	b .L_02000f80_1
.L_02000f80_0:
	ldr r0, [pc, #112]
	adds r3, r2, r0
	lsls r3, r3, #16
	lsrs r3, r3, #16
	cmp r3, r1
	bhi .L_02000f80_2
	movs r0, #15
	movs r1, #232
	movs r2, #160
	bl 0x0200c434
	movs r1, #160
	movs r0, #15
	lsls r1, r1, #7
	b .L_02000f80_1
.L_02000f80_2:
	movs r0, #192
	lsls r0, r0, #7
	adds r3, r2, r0
	lsls r3, r3, #16
	lsrs r3, r3, #16
	cmp r3, r1
	bhi .L_02000f80_3
	movs r0, #15
	movs r1, #216
	movs r2, #168
	bl 0x0200c434
	movs r0, #15
	movs r1, #224
	movs r2, #172
	bl 0x0200c434
	movs r1, #224
	movs r0, #15
	lsls r1, r1, #8
.L_02000f80_1:
	movs r2, #20
	bl 0x0200c4c4
	b .L_02000f80_4
.L_02000f80_3:
	movs r0, #15
	movs r1, #232
	movs r2, #160
	bl 0x0200c434
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200c4c4
.L_02000f80_4:
	pop {r0}
	bx r0
	.4byte 0xffffe000
	.4byte 0x3fff0000
	.4byte 0x00003fff
	.4byte 0xffffa000
	.global Func_0200102c
	.thumb_func
Func_0200102c:
	push {r5, r6, lr}
	adds r6, r0, #0
	ldr r1, [pc, #276]
	movs r0, #15
	ldr r2, [pc, #276]
	bl 0x0200c414
	movs r0, #60
	bl 0x0200c3d4
	ldr r5, [pc, #268]
	adds r0, r5, #0
	bl 0x0200c49c
	cmp r6, #0
	bne .L_0200102c_0
	subs r0, r5, #1
	bl 0x0200c49c
	movs r0, #15
	ldr r1, [pc, #252]
	movs r2, #60
	bl 0x0200c4cc
	movs r2, #20
	movs r0, #15
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #2
	movs r0, #15
	bl 0x0200c474
	ldr r0, [pc, #232]
	bl 0x0200c49c
	movs r2, #20
	movs r0, #15
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #4
	movs r0, #15
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #15
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #15
	movs r1, #3
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
.L_0200102c_0:
	cmp r6, #2
	bne .L_0200102c_1
	ldr r0, [pc, #176]
	bl 0x0200c49c
	movs r0, #15
	movs r1, #2
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
.L_0200102c_1:
	movs r2, #20
	movs r0, #15
	movs r1, #0
	bl 0x0200c4b4
	bl 0x02008f80
	movs r0, #15
	movs r1, #3
	bl 0x0200c474
	movs r1, #232
	movs r2, #168
	movs r0, #19
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200c44c
	movs r1, #232
	movs r2, #168
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #20
	bl 0x0200c44c
	movs r0, #19
	bl 0x0200c404
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #12]
	movs r0, #19
	bl 0x0200c404
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r0, #60]
	movs r0, #19
	bl 0x0200c404
	ldr r3, [pc, #56]
	str r3, [r0, #24]
	movs r0, #19
	bl 0x0200c404
	movs r3, #128
	ldr r2, [r0, #80]
	lsls r3, r3, #8
	strh r3, [r2, #30]
	movs r0, #124
	bl 0x0200c57c
	movs r0, #40
	bl 0x0200c3d4
	movs r0, #15
	movs r1, #216
	movs r2, #152
	bl 0x0200c434
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #6
	movs r2, #30
	bl 0x0200c4c4
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x0000183a
	.4byte 0x00000101
	.4byte 0x000018ae
	.4byte 0x000018ac
	.global Func_02001160
	.thumb_func
Func_02001160:
	push {lr}
	movs r0, #13
	movs r1, #19
	movs r2, #0
	bl 0x0200c47c
	movs r0, #14
	movs r1, #19
	movs r2, #0
	bl 0x0200c47c
	movs r0, #15
	movs r1, #19
	movs r2, #0
	bl 0x0200c47c
	movs r0, #16
	movs r1, #19
	movs r2, #0
	bl 0x0200c47c
	movs r2, #0
	movs r1, #19
	movs r0, #18
	bl 0x0200c47c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #2
	movs r0, #15
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	ldr r0, [pc, #284]
	bl 0x0200c49c
	movs r0, #15
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #16
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #18
	ldr r1, [pc, #260]
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #16
	ldr r1, [pc, #256]
	movs r2, #60
	bl 0x0200c4cc
	movs r2, #20
	movs r0, #16
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #4
	movs r0, #18
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #129
	movs r0, #16
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #16
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r2, #0
	movs r1, #18
	movs r0, #15
	bl 0x0200c484
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #18
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #15
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	ldr r2, [pc, #152]
	movs r0, #15
	ldr r1, [pc, #152]
	bl 0x0200c414
	bl 0x02008f80
	movs r0, #15
	movs r1, #3
	bl 0x0200c474
	movs r1, #232
	movs r2, #168
	movs r0, #19
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200c44c
	movs r1, #232
	movs r2, #168
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #20
	bl 0x0200c44c
	movs r0, #19
	bl 0x0200c404
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #12]
	movs r0, #19
	bl 0x0200c404
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r0, #60]
	movs r0, #19
	bl 0x0200c404
	ldr r3, [pc, #76]
	str r3, [r0, #24]
	movs r0, #19
	bl 0x0200c404
	movs r3, #128
	ldr r2, [r0, #80]
	lsls r3, r3, #8
	strh r3, [r2, #30]
	movs r0, #124
	bl 0x0200c57c
	movs r0, #40
	bl 0x0200c3d4
	movs r0, #15
	movs r1, #216
	movs r2, #152
	bl 0x0200c434
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #30
	bl 0x0200c4c4
	ldr r0, [pc, #28]
	bl 0x0200c3c4
	pop {r0}
	bx r0
	.4byte 0x0000187a
	.4byte 0x00000105
	.4byte 0x00000101
	.4byte 0x00006666
	.4byte 0x0000cccc
	.4byte 0x00000301
	.global Func_020012e0
	.thumb_func
Func_020012e0:
	push {lr}
	bl 0x0200c3dc
	movs r0, #0
	bl 0x0200c404
	movs r2, #128
	ldrh r3, [r0, #6]
	lsls r2, r2, #7
	cmp r3, r2
	bls .L_020012e0_0
	movs r0, #0
	bl 0x0200c404
	movs r2, #192
	ldrh r3, [r0, #6]
	lsls r2, r2, #8
	cmp r3, r2
	bcs .L_020012e0_0
	bl 0x02008bf4
	b .L_020012e0_1
.L_020012e0_0:
	bl 0x02008cd4
.L_020012e0_1:
	ldr r0, [pc, #28]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_020012e0_2
	bl 0x02009160
	b .L_020012e0_3
.L_020012e0_2:
	movs r0, #0
	bl 0x0200902c
.L_020012e0_3:
	bl 0x0200c3e4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000898
	.global Func_02001334
	.thumb_func
Func_02001334:
	push {r5, lr}
	movs r0, #0
	bl 0x0200c404
	adds r5, r0, #0
	bl 0x0200c3dc
	movs r1, #8
	movs r0, #0
	bl 0x0200c454
	movs r0, #20
	bl 0x0200c3d4
	ldr r0, [pc, #84]
	ldrh r2, [r5, #6]
	adds r3, r2, r0
	ldr r0, [pc, #84]
	lsls r3, r3, #16
	ldr r1, [pc, #84]
	cmp r3, r0
	bhi .L_02001334_0
	bl 0x02008e94
	b .L_02001334_1
.L_02001334_0:
	ldr r0, [pc, #76]
	adds r3, r2, r0
	lsls r3, r3, #16
	lsrs r3, r3, #16
	cmp r3, r1
	bhi .L_02001334_2
	bl 0x02008bf4
	b .L_02001334_1
.L_02001334_2:
	movs r0, #192
	lsls r0, r0, #7
	adds r3, r2, r0
	lsls r3, r3, #16
	lsrs r3, r3, #16
	cmp r3, r1
	bhi .L_02001334_3
	bl 0x02008db4
	b .L_02001334_1
.L_02001334_3:
	bl 0x02008cd4
.L_02001334_1:
	movs r1, #1
	movs r0, #0
	bl 0x0200c454
	movs r0, #1
	bl 0x0200902c
	bl 0x0200c3e4
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xffffe000
	.4byte 0x3fff0000
	.4byte 0x00003fff
	.4byte 0xffffa000
	.global Func_020013b8
	.thumb_func
Func_020013b8:
	push {r5, lr}
	movs r0, #0
	bl 0x0200c404
	adds r5, r0, #0
	bl 0x0200c3dc
	ldr r0, [pc, #180]
	ldrh r2, [r5, #6]
	adds r3, r2, r0
	ldr r0, [pc, #176]
	lsls r3, r3, #16
	ldr r1, [pc, #176]
	cmp r3, r0
	bhi .L_020013b8_0
	bl 0x02008e94
	b .L_020013b8_1
.L_020013b8_0:
	ldr r0, [pc, #168]
	adds r3, r2, r0
	lsls r3, r3, #16
	lsrs r3, r3, #16
	cmp r3, r1
	bhi .L_020013b8_2
	bl 0x02008bf4
	b .L_020013b8_1
.L_020013b8_2:
	movs r0, #192
	lsls r0, r0, #7
	adds r3, r2, r0
	lsls r3, r3, #16
	lsrs r3, r3, #16
	cmp r3, r1
	bhi .L_020013b8_3
	bl 0x02008db4
	b .L_020013b8_1
.L_020013b8_3:
	bl 0x02008cd4
.L_020013b8_1:
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200c4e4
	movs r1, #1
	movs r0, #20
	bl 0x0200c4fc
	bl 0x0200c4f4
	movs r1, #18
	ldrsh r3, [r5, r1]
	cmp r3, #209
	bgt .L_020013b8_4
	ldr r0, [pc, #100]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_020013b8_5
	ldr r0, [pc, #92]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_020013b8_6
.L_020013b8_5:
	movs r0, #0
	bl 0x0200902c
	b .L_020013b8_7
.L_020013b8_6:
	bl 0x02009160
.L_020013b8_7:
	bl 0x0200c3e4
	b .L_020013b8_8
.L_020013b8_4:
	ldr r0, [pc, #64]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_020013b8_9
	movs r0, #2
	bl 0x0200902c
	b .L_020013b8_10
.L_020013b8_9:
	ldr r0, [pc, #44]
	bl 0x0200c3bc
	cmp r0, #0
	bne .L_020013b8_11
	bl 0x02009494
	b .L_020013b8_10
.L_020013b8_11:
	bl 0x02009dbc
.L_020013b8_10:
	bl 0x0200c3e4
.L_020013b8_8:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xffffe000
	.4byte 0x3fff0000
	.4byte 0x00003fff
	.4byte 0xffffa000
	.4byte 0x0000089a
	.4byte 0x0000089b
	.global Func_02001494
	.thumb_func
Func_02001494:
	push {r5, lr}
	ldr r0, [pc, #1016]
	bl 0x0200c3c4
	movs r0, #30
	bl 0x0200c3d4
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x0200c47c
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl 0x0200c47c
	movs r1, #0
	movs r2, #0
	movs r0, #16
	bl 0x0200c47c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200c4cc
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200c4cc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #16
	bl 0x0200c4cc
	movs r0, #60
	bl 0x0200c3d4
	ldr r0, [pc, #928]
	bl 0x0200c49c
	movs r0, #13
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r2, #0
	movs r0, #0
	movs r1, #13
	bl 0x0200c47c
	movs r1, #1
	movs r0, #15
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #15
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r2, #0
	movs r0, #0
	movs r1, #15
	bl 0x0200c47c
	movs r1, #2
	movs r0, #16
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #0
	movs r0, #0
	movs r1, #16
	bl 0x0200c47c
	movs r1, #0
	movs r0, #16
	bl 0x0200c4bc
	movs r0, #50
	bl 0x0200c3d4
	movs r0, #16
	movs r1, #1
	bl 0x0200c4dc
	movs r0, #16
	ldr r1, [pc, #824]
	ldr r2, [pc, #828]
	bl 0x0200c414
	movs r0, #16
	movs r1, #176
	movs r2, #248
	bl 0x0200c434
	movs r1, #154
	movs r0, #16
	lsls r1, r1, #1
	movs r2, #248
	bl 0x0200c434
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #20
	movs r0, #16
	bl 0x0200c4c4
	movs r0, #158
	bl 0x0200c57c
	movs r2, #13
	ldr r0, [pc, #772]
	movs r1, #78
	bl 0x0200c394
	movs r1, #2
	movs r0, #16
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #192
	movs r2, #192
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #16
	bl 0x0200c414
	movs r0, #16
	bl 0x0200c404
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #154
	movs r2, #136
	strb r3, [r0]
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #16
	bl 0x0200c434
	movs r0, #1
	bl 0x0200c3d4
	movs r0, #16
	bl 0x0200c404
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r0, #16
	movs r2, #50
	bl 0x0200c4b4
	movs r1, #152
	movs r2, #216
	movs r0, #17
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200c44c
	movs r1, #152
	movs r0, #17
	lsls r1, r1, #1
	movs r2, #248
	bl 0x0200c434
	movs r0, #9
	movs r1, #17
	movs r2, #0
	bl 0x0200c47c
	movs r0, #10
	movs r1, #17
	movs r2, #0
	bl 0x0200c47c
	movs r0, #11
	movs r1, #17
	movs r2, #0
	bl 0x0200c47c
	movs r0, #12
	movs r1, #17
	movs r2, #0
	bl 0x0200c47c
	movs r0, #13
	movs r1, #17
	movs r2, #0
	bl 0x0200c47c
	movs r0, #14
	movs r1, #17
	movs r2, #0
	bl 0x0200c47c
	movs r0, #15
	movs r1, #17
	movs r2, #0
	bl 0x0200c47c
	movs r0, #16
	movs r1, #17
	movs r2, #0
	bl 0x0200c47c
	movs r2, #0
	movs r1, #17
	movs r0, #0
	bl 0x0200c47c
	movs r0, #10
	bl 0x0200c3d4
	movs r0, #9
	movs r1, #2
	bl 0x0200c46c
	movs r0, #10
	movs r1, #2
	bl 0x0200c46c
	movs r0, #11
	movs r1, #2
	bl 0x0200c46c
	movs r0, #12
	movs r1, #2
	bl 0x0200c46c
	movs r0, #13
	movs r1, #2
	bl 0x0200c46c
	movs r0, #14
	movs r1, #2
	bl 0x0200c46c
	movs r0, #15
	movs r1, #2
	bl 0x0200c46c
	movs r0, #16
	movs r1, #2
	bl 0x0200c474
	movs r0, #17
	ldr r1, [pc, #488]
	movs r2, #60
	bl 0x0200c4cc
	movs r1, #152
	movs r2, #216
	movs r0, #18
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200c44c
	movs r1, #152
	movs r0, #18
	lsls r1, r1, #1
	movs r2, #248
	bl 0x0200c42c
	movs r1, #140
	movs r2, #132
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #17
	bl 0x0200c42c
	movs r0, #18
	bl 0x0200c444
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #18
	bl 0x0200c4c4
	movs r0, #17
	bl 0x0200c444
	movs r0, #159
	bl 0x0200c57c
	ldr r0, [pc, #416]
	movs r1, #78
	movs r2, #13
	bl 0x0200c394
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #9
	movs r1, #17
	movs r2, #0
	bl 0x0200c47c
	movs r0, #10
	movs r1, #17
	movs r2, #0
	bl 0x0200c47c
	movs r0, #11
	movs r1, #17
	movs r2, #0
	bl 0x0200c47c
	movs r0, #12
	movs r1, #17
	movs r2, #0
	bl 0x0200c47c
	movs r0, #13
	movs r1, #17
	movs r2, #0
	bl 0x0200c47c
	movs r0, #14
	movs r1, #17
	movs r2, #0
	bl 0x0200c47c
	movs r0, #15
	movs r1, #17
	movs r2, #0
	bl 0x0200c47c
	movs r0, #16
	movs r1, #17
	movs r2, #0
	bl 0x0200c47c
	movs r2, #0
	movs r1, #17
	movs r0, #0
	bl 0x0200c47c
	movs r0, #10
	bl 0x0200c3d4
	movs r1, #2
	movs r0, #17
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #4
	movs r0, #18
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #18
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #17
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #1
	movs r0, #18
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #208
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200c4c4
	movs r0, #17
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #129
	movs r0, #18
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200c4cc
	movs r2, #20
	movs r0, #18
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #17
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #17
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #18
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #18
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #17
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #4
	movs r0, #18
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #18
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #2
	movs r0, #17
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #17
	movs r1, #0
	movs r2, #20
	bl 0x0200c4c4
	movs r0, #17
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #128
	movs r2, #20
	movs r0, #16
	lsls r1, r1, #8
	bl 0x0200c4c4
	movs r1, #3
	movs r0, #16
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #17
	bl 0x0200c45c
	movs r0, #20
	b .L_02001494_0
	.4byte 0x0000089a
	.4byte 0x0000183b
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x0200c77a
	.4byte 0x00000103
	.4byte 0x0200c790
.L_02001494_0:
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #17
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #16
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #16
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200c4c4
	movs r2, #20
	movs r0, #17
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #9
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #208
	movs r2, #20
	movs r0, #17
	lsls r1, r1, #8
	bl 0x0200c4c4
	movs r1, #1
	movs r0, #17
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #17
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #129
	movs r0, #18
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #17
	ldr r1, [pc, #900]
	movs r2, #60
	bl 0x0200c4cc
	movs r2, #20
	movs r0, #17
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #18
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200c4cc
	movs r2, #20
	movs r0, #17
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #4
	movs r0, #18
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #17
	ldr r1, [pc, #824]
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #17
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #128
	movs r0, #18
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200c4cc
	movs r2, #20
	movs r0, #18
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #4
	movs r0, #17
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #17
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #2
	movs r0, #18
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #18
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #2
	movs r0, #17
	bl 0x0200c474
	movs r0, #10
	bl 0x0200c3d4
	movs r1, #128
	movs r2, #140
	movs r0, #17
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200c434
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200c4c4
	movs r2, #0
	movs r1, #0
	movs r0, #17
	bl 0x0200c44c
	movs r0, #17
	bl 0x0200c40c
	movs r0, #30
	bl 0x0200c3d4
	movs r1, #2
	movs r0, #9
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #2
	movs r0, #15
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #15
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r2, #0
	movs r1, #18
	movs r0, #16
	bl 0x0200c47c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #2
	movs r0, #16
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #16
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #16
	movs r2, #0
	movs r0, #18
	bl 0x0200c47c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #18
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #4
	movs r0, #18
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #18
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #18
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #18
	ldr r1, [pc, #516]
	ldr r2, [pc, #516]
	bl 0x0200c414
	movs r1, #128
	movs r0, #18
	lsls r1, r1, #1
	movs r2, #248
	bl 0x0200c434
	movs r1, #192
	movs r2, #20
	movs r0, #18
	lsls r1, r1, #8
	bl 0x0200c4c4
	movs r0, #18
	movs r1, #1
	bl 0x0200c46c
	movs r1, #128
	movs r0, #18
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200c4cc
	movs r2, #184
	movs r0, #18
	movs r1, #240
	bl 0x0200c434
	movs r1, #2
	movs r0, #18
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #232
	movs r2, #168
	movs r0, #19
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200c44c
	movs r1, #232
	movs r2, #168
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #20
	bl 0x0200c44c
	movs r0, #19
	bl 0x0200c404
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #12]
	movs r0, #19
	bl 0x0200c404
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r0, #60]
	movs r0, #19
	bl 0x0200c404
	ldr r3, [pc, #380]
	str r3, [r0, #24]
	movs r0, #19
	bl 0x0200c404
	movs r3, #128
	ldr r2, [r0, #80]
	lsls r3, r3, #8
	strh r3, [r2, #30]
	movs r0, #124
	bl 0x0200c57c
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200c4c4
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #1
	movs r2, #240
	bl 0x0200c434
	movs r1, #176
	movs r2, #20
	movs r0, #16
	lsls r1, r1, #8
	bl 0x0200c4c4
	movs r0, #16
	movs r1, #1
	bl 0x0200c474
	movs r0, #16
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200c47c
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200c47c
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x0200c47c
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x0200c47c
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x0200c47c
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl 0x0200c47c
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl 0x0200c47c
	movs r2, #0
	movs r0, #16
	movs r1, #0
	bl 0x0200c47c
	movs r1, #2
	movs r0, #18
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #160
	movs r0, #18
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200c4c4
	movs r0, #18
	movs r1, #248
	movs r2, #208
	bl 0x0200c434
	movs r1, #160
	movs r0, #18
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200c4c4
	movs r1, #0
	movs r0, #18
	bl 0x0200c4a4
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f4
	cmp r0, #0
	bne .L_02001494_1
	movs r1, #1
	movs r0, #16
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #16
	b .L_02001494_2
.L_02001494_1:
	ldr r5, [pc, #136]
	movs r2, #236
	ldr r3, [r5]
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #18
	ldr r1, [pc, #116]
	movs r2, #60
	bl 0x0200c4cc
	movs r1, #128
	movs r2, #20
	movs r0, #18
	lsls r1, r1, #7
	bl 0x0200c4c4
	movs r1, #2
	movs r0, #16
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #0
	movs r0, #16
	bl 0x0200c4a4
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f4
	cmp r0, #0
	bne .L_02001494_3
	movs r1, #3
	movs r0, #16
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #176
	movs r0, #18
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200c4c4
	movs r0, #18
.L_02001494_2:
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	ldr r0, [pc, #28]
	bl 0x0200c3c4
	b .L_02001494_4
	.4byte 0x00000101
	.4byte 0x00000103
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x03001ebc
	.4byte 0x00000105
	.4byte 0x00000898
.L_02001494_3:
	ldr r2, [r5]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #4
	movs r0, #18
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	ldr r0, [pc, #56]
	bl 0x0200c3c4
.L_02001494_4:
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #128
	movs r2, #20
	movs r0, #11
	lsls r1, r1, #8
	bl 0x0200c4c4
	movs r0, #10
	movs r1, #5
	bl 0x0200c454
	movs r0, #11
	movs r1, #5
	bl 0x0200c454
	ldr r1, [pc, #16]
	movs r0, #12
	bl 0x0200c41c
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000899
	.4byte 0x0200c638
	.global Func_02001d50
	.thumb_func
Func_02001d50:
	push {lr}
	bl 0x0200c3dc
	ldr r0, [pc, #88]
	bl 0x0200c49c
	movs r1, #0
	movs r0, #18
	bl 0x0200c4a4
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f4
	cmp r0, #0
	bne .L_02001d50_0
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	ldr r0, [pc, #48]
	bl 0x0200c3c4
	bl 0x0200c3e4
	b .L_02001d50_1
.L_02001d50_0:
	ldr r3, [pc, #40]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	bl 0x0200c3e4
.L_02001d50_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000186e
	.4byte 0x00000898
	.4byte 0x03001ebc
	.global Func_02001dbc
	.thumb_func
Func_02001dbc:
	push {r5, lr}
	movs r0, #18
	bl 0x0200c404
	movs r5, #0
	str r5, [r0, #108]
	movs r0, #13
	bl 0x0200c404
	str r5, [r0, #108]
	movs r0, #14
	bl 0x0200c404
	str r5, [r0, #108]
	movs r0, #15
	bl 0x0200c404
	str r5, [r0, #108]
	movs r0, #16
	bl 0x0200c404
	movs r1, #1
	str r5, [r0, #108]
	movs r0, #11
	bl 0x0200c454
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl 0x0200c4e4
	movs r0, #232
	movs r1, #1
	movs r2, #200
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #16
	bl 0x0200c4ec
	bl 0x0200c4f4
	ldr r0, [pc, #1020]
	bl 0x0200c49c
	movs r0, #10
	ldr r1, [pc, #1016]
	ldr r2, [pc, #1016]
	bl 0x0200c414
	movs r0, #12
	ldr r1, [pc, #1004]
	ldr r2, [pc, #1008]
	bl 0x0200c414
	movs r0, #10
	movs r1, #152
	movs r2, #200
	bl 0x0200c42c
	movs r1, #144
	movs r2, #248
	movs r0, #12
	bl 0x0200c434
	movs r0, #10
	bl 0x0200c444
	movs r0, #9
	movs r1, #19
	movs r2, #0
	bl 0x0200c47c
	movs r0, #11
	movs r1, #19
	movs r2, #0
	bl 0x0200c47c
	movs r0, #13
	movs r1, #19
	movs r2, #0
	bl 0x0200c47c
	movs r0, #14
	movs r1, #19
	movs r2, #0
	bl 0x0200c47c
	movs r0, #15
	movs r1, #19
	movs r2, #0
	bl 0x0200c47c
	movs r0, #16
	movs r1, #19
	movs r2, #0
	bl 0x0200c47c
	movs r0, #18
	movs r1, #19
	movs r2, #0
	bl 0x0200c47c
	movs r1, #192
	movs r2, #192
	movs r0, #10
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c414
	movs r1, #128
	movs r2, #128
	movs r0, #12
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200c414
	movs r0, #10
	movs r1, #152
	movs r2, #200
	bl 0x0200c42c
	movs r0, #12
	movs r1, #144
	movs r2, #248
	bl 0x0200c434
	movs r1, #19
	movs r2, #0
	movs r0, #12
	bl 0x0200c47c
	movs r0, #10
	bl 0x0200c444
	movs r2, #0
	movs r0, #10
	movs r1, #19
	bl 0x0200c47c
	movs r1, #2
	movs r0, #18
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #18
	movs r1, #0
	movs r2, #40
	bl 0x0200c4b4
	movs r0, #9
	movs r1, #18
	movs r2, #0
	bl 0x0200c47c
	movs r0, #10
	movs r1, #18
.L_02001efc:
	movs r2, #0
	bl 0x0200c47c
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c4c4
	movs r0, #12
	movs r1, #18
	movs r2, #0
	bl 0x0200c47c
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c4c4
	movs r0, #14
	movs r1, #18
	movs r2, #0
	bl 0x0200c47c
	movs r0, #15
	movs r1, #18
	movs r2, #0
	bl 0x0200c47c
	movs r1, #18
	movs r2, #0
	movs r0, #16
	bl 0x0200c47c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #16
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #18
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #16
	bl 0x0200c45c
.L_02001f68:
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #16
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #18
	ldr r1, [pc, #672]
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #16
	ldr r1, [pc, #664]
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #16
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #129
	movs r0, #16
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #15
	ldr r1, [pc, #624]
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #15
	ldr r1, [pc, #600]
	ldr r2, [pc, #604]
	bl 0x0200c414
	movs r0, #15
	movs r1, #216
	movs r2, #176
	bl 0x0200c434
	movs r1, #192
	movs r0, #15
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200c4c4
	movs r0, #15
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #176
.L_02001fe2:
	movs r2, #20
	movs r0, #18
	lsls r1, r1, #8
	bl 0x0200c4c4
	movs r1, #4
	movs r0, #18
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #18
	movs r1, #0
	bl 0x0200c4b4
	movs r0, #9
	movs r1, #2
	bl 0x0200c46c
	movs r0, #10
	movs r1, #2
	bl 0x0200c46c
	movs r0, #11
	movs r1, #2
	bl 0x0200c46c
	movs r0, #12
	movs r1, #2
	bl 0x0200c46c
	movs r0, #13
	movs r1, #2
	bl 0x0200c46c
	movs r0, #14
	movs r1, #2
	bl 0x0200c46c
	movs r0, #15
	movs r1, #2
	bl 0x0200c46c
	movs r1, #2
	movs r0, #16
	bl 0x0200c46c
	movs r0, #40
	bl 0x0200c3d4
	movs r0, #13
	movs r1, #2
	bl 0x0200c474
	movs r2, #20
	movs r0, #13
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #18
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200c4c4
	movs r1, #160
	movs r2, #20
	movs r0, #18
	lsls r1, r1, #7
	bl 0x0200c4c4
	movs r1, #0
	movs r0, #18
	bl 0x0200c4bc
	ldr r1, [pc, #392]
	movs r2, #0
	movs r0, #9
	bl 0x0200c4cc
	movs r0, #5
	bl 0x0200c3d4
	ldr r1, [pc, #376]
	movs r2, #0
	movs r0, #10
	bl 0x0200c4cc
	movs r0, #5
	bl 0x0200c3d4
	ldr r1, [pc, #360]
	movs r2, #0
	movs r0, #11
	bl 0x0200c4cc
	movs r0, #5
	bl 0x0200c3d4
	ldr r1, [pc, #344]
	movs r2, #0
	movs r0, #12
	bl 0x0200c4cc
	movs r0, #5
	bl 0x0200c3d4
	ldr r1, [pc, #328]
	movs r2, #0
.L_020020d8:
	movs r0, #13
	bl 0x0200c4cc
	movs r0, #5
	bl 0x0200c3d4
	ldr r1, [pc, #312]
	movs r2, #0
	movs r0, #14
	bl 0x0200c4cc
	movs r0, #5
	bl 0x0200c3d4
	ldr r1, [pc, #296]
	movs r2, #0
	movs r0, #15
	bl 0x0200c4cc
	movs r0, #5
	bl 0x0200c3d4
	movs r2, #0
	ldr r1, [pc, #280]
	movs r0, #16
	bl 0x0200c4cc
	movs r0, #60
	bl 0x0200c3d4
	movs r0, #16
	movs r1, #2
	bl 0x0200c474
	movs r2, #20
	movs r0, #16
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #18
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #15
	ldr r1, [pc, #220]
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #15
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r2, #0
	movs r1, #15
	movs r0, #18
	bl 0x0200c47c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #4
	movs r0, #18
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #18
	movs r1, #0
	movs r2, #40
	bl 0x0200c4b4
	movs r0, #11
	movs r1, #10
	movs r2, #0
	bl 0x0200c484
	movs r0, #12
	movs r1, #14
	movs r2, #0
	bl 0x0200c484
	movs r1, #15
	movs r2, #0
	movs r0, #13
	bl 0x0200c484
	movs r0, #60
	bl 0x0200c3d4
	movs r0, #10
	movs r1, #18
	movs r2, #0
	bl 0x0200c47c
	movs r0, #11
	movs r1, #18
	movs r2, #0
	bl 0x0200c47c
.L_020021b2:
	movs r0, #12
	movs r1, #18
	movs r2, #0
	bl 0x0200c47c
	movs r0, #13
	movs r1, #18
	movs r2, #0
	bl 0x0200c47c
.L_020021c6:
	movs r0, #14
	movs r1, #18
	movs r2, #0
	bl 0x0200c47c
	movs r1, #18
	movs r2, #0
	movs r0, #15
	bl 0x0200c47c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #18
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #2
	movs r0, #18
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #9
	movs r1, #3
	bl 0x0200c454
	movs r0, #10
	movs r1, #3
	bl 0x0200c454
	movs r0, #11
	movs r1, #3
	b .L_020021c6_0
	.2byte 0x0000
	.2byte 0x1883
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0x0101
	.2byte 0x0000
.L_020021c6_0:
	bl 0x0200c454
	movs r0, #12
	movs r1, #3
	bl 0x0200c454
	movs r0, #13
	movs r1, #3
	bl 0x0200c454
	movs r0, #14
	movs r1, #3
	bl 0x0200c454
	movs r0, #15
	movs r1, #3
	bl 0x0200c454
	movs r1, #3
	movs r0, #16
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #160
	movs r2, #20
	movs r0, #18
	lsls r1, r1, #7
	bl 0x0200c4c4
	movs r1, #0
	movs r0, #18
	bl 0x0200c4bc
	movs r1, #3
.L_0200226c:
	movs r0, #18
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #18
	movs r1, #0
	bl 0x0200c4b4
	movs r0, #0
	movs r1, #3
	bl 0x0200c454
	movs r0, #9
	movs r1, #3
	bl 0x0200c454
	movs r0, #10
	movs r1, #3
	bl 0x0200c454
	movs r0, #11
	movs r1, #3
	bl 0x0200c454
	movs r0, #12
	movs r1, #3
	bl 0x0200c454
	movs r0, #13
	movs r1, #3
	bl 0x0200c454
	movs r0, #14
	movs r1, #3
	bl 0x0200c454
	movs r0, #15
	movs r1, #3
	bl 0x0200c454
	movs r1, #3
	movs r0, #16
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #2
	movs r0, #18
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r2, #20
	movs r0, #18
	movs r1, #0
	bl 0x0200c4b4
	movs r0, #0
	movs r1, #3
	bl 0x0200c454
	movs r1, #3
	movs r0, #18
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #128
	movs r2, #20
	movs r0, #18
	lsls r1, r1, #8
	bl 0x0200c4c4
	movs r1, #2
	movs r0, #18
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #18
	movs r1, #0
	bl 0x0200c4b4
	movs r0, #9
	movs r1, #3
	bl 0x0200c454
	movs r0, #10
	movs r1, #3
	bl 0x0200c454
	movs r0, #11
	movs r1, #3
	bl 0x0200c454
	movs r0, #12
	movs r1, #3
	bl 0x0200c454
	movs r0, #13
	movs r1, #3
.L_02002350:
	bl 0x0200c454
	movs r0, #14
	movs r1, #3
	bl 0x0200c454
	movs r0, #15
	movs r1, #3
	bl 0x0200c454
	movs r1, #3
	movs r0, #16
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #10
	movs r1, #120
	movs r2, #200
	bl 0x0200c42c
	movs r1, #120
	movs r2, #248
	movs r0, #12
	bl 0x0200c42c
	movs r0, #10
	bl 0x0200c444
	movs r1, #128
	movs r2, #20
	movs r0, #11
	lsls r1, r1, #8
	bl 0x0200c4c4
	movs r0, #10
	movs r1, #5
	bl 0x0200c454
	movs r1, #5
	movs r0, #11
	bl 0x0200c454
	movs r0, #12
	bl 0x0200c444
	ldr r1, [pc, #160]
	movs r0, #12
	bl 0x0200c41c
	movs r0, #15
	ldr r1, [pc, #152]
	ldr r2, [pc, #156]
	bl 0x0200c414
	movs r0, #15
	movs r1, #216
	movs r2, #168
	bl 0x0200c434
	movs r0, #15
	movs r1, #232
	movs r2, #168
	bl 0x0200c434
	movs r1, #192
	movs r2, #20
	movs r0, #15
	lsls r1, r1, #8
	bl 0x0200c4c4
	movs r0, #15
	movs r1, #3
	bl 0x0200c474
	movs r1, #232
	movs r2, #168
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #19
	bl 0x0200c44c
	movs r0, #19
	bl 0x0200c404
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #12]
	movs r0, #19
	bl 0x0200c404
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r0, #60]
	movs r0, #19
	bl 0x0200c404
	movs r3, #128
	ldr r2, [r0, #80]
	lsls r3, r3, #8
	strh r3, [r2, #30]
	movs r0, #124
	bl 0x0200c57c
	movs r0, #40
	bl 0x0200c3d4
	movs r0, #15
	movs r1, #216
	movs r2, #152
	bl 0x0200c434
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #30
	movs r0, #15
	bl 0x0200c4c4
	ldr r0, [pc, #28]
	bl 0x0200c3cc
	ldr r0, [pc, #24]
	bl 0x0200c3c4
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0200c638
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00000898
	.4byte 0x0000089b
	.global Func_02002464
	.thumb_func
Func_02002464:
	push {lr}
	bl 0x0200c3dc
.L_0200246a:
	ldr r0, [pc, #20]
	bl 0x0200c49c
	movs r1, #0
	movs r0, #11
	bl 0x0200c4bc
	bl 0x0200c3e4
.L_0200247c:
	pop {r0}
	bx r0
	.2byte 0x1a58
	.2byte 0x0000
	.global Func_02002484
	.thumb_func
Func_02002484:
	push {lr}
.L_02002486:
	bl 0x0200c3dc
	ldr r0, [pc, #108]
	bl 0x0200c3bc
	cmp r0, #0
	bne .L_02002486_0
	ldr r0, [pc, #100]
	bl 0x0200c3bc
	cmp r0, #0
	bne .L_02002486_0
	ldr r0, [pc, #96]
	movs r1, #1
	bl 0x0200c3b4
	bl 0x0200c3e4
	b .L_02002486_1
.L_02002486_0:
	movs r0, #158
	bl 0x0200c57c
	ldr r0, [pc, #80]
	movs r1, #78
	movs r2, #13
	bl 0x0200c394
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200c414
	movs r1, #153
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #248
	bl 0x0200c434
	movs r1, #152
	lsls r1, r1, #1
	movs r2, #216
	movs r0, #0
	bl 0x0200c42c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #4
	bl 0x0200c504
	bl 0x0200c3e4
.L_02002486_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000089a
	.4byte 0x00000895
	.4byte 0x000018ad
	.4byte 0x0200c77a
	.global Func_02002508
	.thumb_func
Func_02002508:
	push {r5, lr}
	movs r0, #0
	bl 0x0200c404
	ldrh r5, [r0, #6]
	bl 0x0200c3dc
	ldr r3, [pc, #40]
	adds r5, r5, r3
	ldr r3, [pc, #40]
	cmp r5, r3
	bhi .L_02002508_0
	movs r0, #13
	bl 0x0200c574
	b .L_02002508_1
.L_02002508_0:
	ldr r0, [pc, #28]
	bl 0x0200c49c
	movs r0, #13
	movs r1, #0
	bl 0x0200c4ac
.L_02002508_1:
	bl 0x0200c3e4
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x00001a1c
	.global Func_0200254c
	.thumb_func
Func_0200254c:
	push {lr}
	bl 0x0200c3dc
	movs r1, #2
	movs r0, #8
	bl 0x0200c474
	ldr r0, [pc, #20]
	bl 0x0200c49c
	movs r0, #8
	movs r1, #0
	bl 0x0200c4ac
	bl 0x0200c3e4
	pop {r0}
	bx r0
	.4byte 0x000017df
	.global Func_02002574
	.thumb_func
Func_02002574:
	push {lr}
	ldr r1, [pc, #44]
	movs r0, #224
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02002574_0
	ldr r0, [pc, #32]
	b .L_02002574_1
.L_02002574_0:
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #3
	bne .L_02002574_2
	ldr r0, [pc, #20]
	b .L_02002574_1
.L_02002574_2:
	ldr r0, [pc, #20]
.L_02002574_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x0000003c
	.4byte 0x0200cb90
	.4byte 0x0200d184
	.4byte 0x0200cd40
	.global Func_020025b8
	.thumb_func
Func_020025b8:
	push {r5, r6, r7, lr}
	movs r0, #0
	sub sp, #56
	bl 0x0200c404
	movs r5, #7
	add r6, sp, #16
	adds r7, r0, #0
	str r5, [r6, #4]
	bl 0x0200c35c
	lsls r3, r0, #3
	subs r3, r3, r0
	lsrs r3, r3, #16
	ands r3, r5
	cmp r3, #0
	bne .L_020025b8_0
	movs r3, #5
	str r3, [r6, #4]
.L_020025b8_0:
	ldr r3, [pc, #112]
	str r3, [r6, #8]
	ldr r3, [pc, #112]
	str r3, [r6, #12]
	bl 0x0200c35c
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r4, r0, #1
	adds r4, r4, r0
	ldr r5, [pc, #100]
	lsls r3, r4, #4
	adds r4, r4, r3
	ldr r2, [r5]
	lsls r3, r4, #8
	adds r4, r4, r3
	movs r3, #15
	ands r2, r3
	movs r3, #8
	ldr r0, [r7, #8]
	subs r3, r3, r2
	lsls r3, r3, #16
	ldr r1, [r7, #12]
	adds r0, r0, r3
	movs r3, #192
	lsls r3, r3, #13
	adds r1, r1, r3
	movs r3, #0
	ldr r2, [r7, #16]
	str r3, [sp, #4]
	movs r3, #144
	lsls r3, r3, #12
	negs r4, r4
	str r3, [sp, #8]
	movs r3, #0
	str r4, [sp, #0]
	str r6, [sp, #12]
	bl 0x0200813c
	ldr r3, [r5]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_020025b8_1
	movs r0, #0
	movs r1, #15
	bl 0x0200c48c
	b .L_020025b8_2
.L_020025b8_1:
	movs r0, #0
	movs r1, #1
	bl 0x0200c48c
.L_020025b8_2:
	sub sp, #-56
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0000b333
	.4byte 0x0000cccc
	.4byte 0x03001e40
	.global Func_0200265c
	.thumb_func
Func_0200265c:
	push {lr}
	movs r1, #32
	negs r1, r1
	movs r0, #0
	bl 0x0200a68c
	pop {r0}
	bx r0
	.global Func_0200266c
	.thumb_func
Func_0200266c:
	push {lr}
	movs r0, #0
	movs r1, #32
	bl 0x0200a68c
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200267c
	.thumb_func
Func_0200267c:
	push {lr}
	movs r0, #32
	negs r0, r0
	movs r1, #0
	bl 0x0200a68c
	pop {r0}
	bx r0
	.global Func_0200268c
	.thumb_func
Func_0200268c:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	bl 0x0200c3dc
	movs r1, #160
	movs r2, #160
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200c414
	adds r1, r5, #0
	adds r2, r6, #0
	movs r0, #0
	bl 0x0200c43c
	movs r2, #0
	movs r0, #0
	movs r1, #4
	bl 0x0200c464
	movs r1, #7
	movs r0, #0
	bl 0x0200c454
	movs r0, #0
	bl 0x0200c444
	movs r0, #0
	movs r1, #6
	bl 0x0200c454
	bl 0x0200c3e4
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_020026d8
	.thumb_func
Func_020026d8:
	push {r5, lr}
	bl 0x0200c3dc
	ldr r5, [pc, #116]
	movs r1, #200
	adds r0, r5, #0
	lsls r1, r1, #4
	bl 0x0200c34c
	ldr r1, [pc, #108]
	movs r0, #0
	ldr r2, [pc, #108]
	bl 0x0200c414
	ldr r3, [pc, #104]
	movs r2, #228
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #60
	str r2, [r3]
	bl 0x0200c52c
	movs r0, #154
	bl 0x0200c57c
	movs r0, #0
	movs r1, #2
	bl 0x0200c454
	movs r2, #6
	negs r2, r2
	movs r1, #0
	movs r0, #0
	bl 0x0200c43c
	movs r0, #0
	bl 0x0200c444
	movs r1, #15
	movs r0, #0
	bl 0x0200c48c
	movs r0, #0
	bl 0x0200c404
	movs r1, #0
	bl 0x0200c3a4
	adds r0, r5, #0
	bl 0x0200c354
	bl 0x0200c534
	movs r0, #3
	bl 0x0200c504
	bl 0x0200c3e4
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0200a5b9
	.4byte 0x00003333
	.4byte 0x00001999
	.4byte 0x03001ebc
	.global Func_02002764
	.thumb_func
Func_02002764:
	push {lr}
.L_02002766:
	movs r0, #123
	bl 0x0200c57c
	movs r0, #1
	bl 0x0200c504
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002778
	.thumb_func
Func_02002778:
	push {lr}
	bl 0x0200c3dc
	movs r0, #188
	bl 0x0200c57c
	movs r1, #77
	movs r2, #8
	ldr r0, [pc, #80]
	bl 0x0200c394
	movs r0, #0
	bl 0x0200c404
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	ldr r1, [pc, #68]
	movs r0, #0
	ldr r2, [pc, #68]
	bl 0x0200c414
	ldr r3, [pc, #64]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	movs r0, #0
	movs r1, #2
	bl 0x0200c454
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #0
	bl 0x0200c43c
	movs r0, #16
	bl 0x0200c3d4
	movs r0, #2
	bl 0x0200c504
	bl 0x0200c3e4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200c764
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x03001ebc
	.global Func_020027ec
	.thumb_func
Func_020027ec:
	push {r5, lr}
	bl 0x0200c3dc
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200c414
	movs r2, #252
	movs r1, #168
	movs r0, #0
	lsls r2, r2, #1
	bl 0x0200c42c
	ldr r5, [pc, #676]
	movs r2, #224
	ldr r3, [r5]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	bl 0x0200c524
	bl 0x0200c534
	movs r0, #0
	bl 0x0200c444
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #2
	bl 0x0200c46c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl 0x0200c4d4
	movs r0, #60
	bl 0x0200c3d4
	movs r0, #8
	bl 0x0200c404
	movs r3, #0
	adds r0, #91
	strb r3, [r0]
	movs r0, #152
	bl 0x0200c57c
	movs r0, #8
	bl 0x0200c404
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r1, #1
	movs r0, #8
	bl 0x0200c454
	ldr r0, [pc, #584]
	bl 0x0200c49c
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #0
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #0
	ldr r1, [pc, #524]
	movs r2, #60
	bl 0x0200c4cc
	movs r1, #0
	movs r0, #8
	bl 0x0200c4a4
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f4
	cmp r0, #0
	bne .L_020027ec_0
	movs r0, #10
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	ldr r2, [r5]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	b 0x0200a93c
.L_020027ec_0:
	movs r0, #10
	bl 0x0200c3d4
.L_020028fe:
	movs r1, #2
	movs r0, #8
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	ldr r2, [r5]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #8
	movs r2, #20
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #2
	movs r0, #8
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #0
	ldr r1, [pc, #344]
	movs r2, #60
	bl 0x0200c4cc
	movs r1, #0
	movs r0, #8
	bl 0x0200c4a4
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f4
	cmp r0, #1
	bne .L_020028fe_0
	movs r0, #10
	bl 0x0200c3d4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	movs r2, #60
	bl 0x0200c4cc
	ldr r0, [pc, #304]
	bl 0x0200c49c
	movs r0, #8
	movs r1, #0
	bl 0x0200c4a4
.L_020028fe_1:
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f4
	cmp r0, #1
	bne .L_020028fe_0
	movs r0, #10
	bl 0x0200c3d4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	movs r2, #60
	bl 0x0200c4cc
	ldr r0, [pc, #264]
	bl 0x0200c49c
	movs r0, #8
	movs r1, #0
	bl 0x0200c4a4
	b .L_020028fe_1
.L_020028fe_0:
	ldr r0, [pc, #252]
	bl 0x0200c49c
	movs r0, #10
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #0
	movs r0, #8
	bl 0x0200c4a4
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f4
	cmp r0, #0
	bne .L_020028fe_2
	movs r0, #10
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #0
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	ldr r3, [pc, #156]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020028fe_3
.L_020028fe_2:
	movs r0, #10
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #2
	bl 0x0200c474
	ldr r3, [pc, #124]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
.L_020028fe_3:
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #2
	movs r0, #8
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #0
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #5
	movs r0, #8
	bl 0x0200c454
	ldr r0, [pc, #36]
	bl 0x0200c3c4
	bl 0x0200c3e4
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.2byte 0x17be
	.2byte 0x0000
	.4byte 0x00000101
	.4byte 0x000017c8
	.4byte 0x000017e0
	.4byte 0x000017c9
	.4byte 0x00000893
	.global Func_02002ad0
	.thumb_func
Func_02002ad0:
	push {lr}
	bl 0x0200c3dc
	movs r0, #0
	ldr r1, [pc, #1004]
	ldr r2, [pc, #1008]
	bl 0x0200c414
	movs r1, #236
	movs r2, #134
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #0
	bl 0x0200c42c
	bl 0x0200c524
	bl 0x0200c534
	movs r0, #0
	bl 0x0200c444
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl 0x0200c4c4
	movs r1, #2
	movs r0, #9
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	ldr r0, [pc, #952]
	bl 0x0200c49c
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #128
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200c4c4
	movs r1, #3
	movs r0, #0
	bl 0x0200c45c
	movs r0, #30
	bl 0x0200c3d4
	movs r1, #128
	movs r2, #30
	movs r0, #8
	lsls r1, r1, #7
	bl 0x0200c4c4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #192
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200c4c4
	movs r1, #1
	movs r0, #0
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r0, #0
	bl 0x0200c404
	cmp r0, #0
	beq .L_02002ad0_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200c44c
.L_02002ad0_0:
	movs r0, #0
	bl 0x0200c404
	cmp r0, #0
	beq .L_02002ad0_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x0200c44c
.L_02002ad0_1:
	movs r0, #0
	bl 0x0200c404
	cmp r0, #0
	beq .L_02002ad0_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x0200c44c
.L_02002ad0_2:
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200c414
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200c414
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200c414
	movs r1, #128
	movs r2, #128
	movs r0, #3
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200c414
	movs r1, #232
	movs r2, #252
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200c42c
	movs r1, #240
	movs r2, #252
	movs r0, #2
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200c42c
	movs r1, #248
	movs r2, #248
	movs r0, #1
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200c42c
	movs r1, #224
	movs r2, #248
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #3
	bl 0x0200c42c
	movs r0, #0
	bl 0x0200c444
	movs r0, #2
	bl 0x0200c444
	movs r0, #3
	bl 0x0200c444
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #3
	bl 0x0200c4c4
	movs r0, #1
	bl 0x0200c444
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200c4c4
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #0
	ldr r1, [pc, #612]
	movs r2, #0
	bl 0x0200c4cc
	movs r0, #1
	ldr r1, [pc, #604]
	movs r2, #0
	bl 0x0200c4cc
	movs r0, #2
	ldr r1, [pc, #592]
	movs r2, #0
	bl 0x0200c4cc
	movs r0, #3
	ldr r1, [pc, #584]
	movs r2, #60
	bl 0x0200c4cc
	movs r1, #160
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200c4c4
	movs r1, #0
	movs r0, #1
	bl 0x0200c4bc
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200c4c4
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200c4cc
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200c4cc
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x0200c4b4
	movs r0, #8
	movs r1, #3
	bl 0x0200c45c
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200c4c4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #128
	movs r2, #0
	movs r0, #3
	lsls r1, r1, #6
	bl 0x0200c4c4
	movs r0, #2
	movs r1, #2
	bl 0x0200c46c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #2
	bl 0x0200c4d4
	movs r0, #60
	bl 0x0200c3d4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #3
	bl 0x0200c4c4
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #2
	movs r0, #2
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #2
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #0
	movs r1, #2
	movs r2, #50
	bl 0x0200c484
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #192
	movs r2, #30
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200c4c4
	movs r1, #3
	movs r0, #2
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #3
	ldr r1, [pc, #188]
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200c4c4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #1
	movs r0, #3
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #1
	bl 0x0200c4c4
	movs r0, #30
	bl 0x0200c3d4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200c4c4
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #4
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #2
	movs r2, #0
	movs r0, #8
	bl 0x0200c47c
	movs r0, #10
	bl 0x0200c3d4
	movs r0, #8
	b .L_02002ad0_3
	.2byte 0x0000
	.4byte 0x00006666
	.4byte 0x00003333
	.4byte 0x00001969
	.4byte 0x00000101
.L_02002ad0_3:
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x0200c47c
	movs r0, #1
	movs r1, #2
	movs r2, #0
	bl 0x0200c47c
	movs r0, #3
	movs r1, #2
	movs r2, #0
	bl 0x0200c47c
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200c4cc
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #2
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #2
	movs r0, #1
	bl 0x0200c46c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200c4cc
	movs r1, #1
	movs r2, #0
	movs r0, #8
	bl 0x0200c47c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #2
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #8
	movs r1, #1
	movs r2, #0
	bl 0x0200c47c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #3
	bl 0x0200c4c4
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #2
	movs r0, #2
	bl 0x0200c46c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #2
	movs r0, #3
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #160
	movs r2, #20
	movs r0, #8
	lsls r1, r1, #7
	bl 0x0200c4c4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r0, #0
	movs r1, #2
	bl 0x0200c46c
	movs r0, #1
	movs r1, #2
	bl 0x0200c46c
	movs r0, #2
	movs r1, #2
	bl 0x0200c46c
	movs r1, #2
	movs r0, #3
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #4
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #2
	movs r0, #8
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r0, #0
	movs r1, #3
	bl 0x0200c454
	movs r0, #1
	movs r1, #3
	bl 0x0200c454
	movs r0, #2
	movs r1, #3
	bl 0x0200c454
	movs r1, #3
	movs r0, #3
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r2, #0
	movs r0, #1
	ldr r1, [pc, #1016]
	bl 0x0200c4cc
	movs r1, #2
	movs r0, #1
	bl 0x0200c46c
	movs r0, #60
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #4
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #0
	ldr r1, [pc, #964]
	movs r2, #0
	bl 0x0200c4cc
	movs r0, #1
	ldr r1, [pc, #956]
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #129
	movs r2, #0
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200c4cc
	movs r1, #1
	movs r0, #8
	bl 0x0200c46c
	movs r0, #60
	bl 0x0200c3d4
	movs r1, #4
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r0, #0
	movs r1, #1
	bl 0x0200c46c
	movs r0, #1
	movs r1, #1
	bl 0x0200c46c
	movs r0, #2
	movs r1, #1
	bl 0x0200c46c
	movs r1, #1
	movs r0, #3
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #4
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #1
	ldr r1, [pc, #812]
	movs r2, #60
	bl 0x0200c4cc
	movs r1, #160
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200c4c4
	movs r1, #0
	movs r0, #1
	bl 0x0200c4bc
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #1
	movs r0, #2
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #2
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200c4c4
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200c4c4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r0, #0
	movs r1, #3
	bl 0x0200c454
	movs r0, #1
	movs r1, #3
	bl 0x0200c454
	movs r0, #2
	movs r1, #3
	bl 0x0200c454
	movs r1, #3
	movs r0, #3
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200c4c4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r0, #0
	movs r1, #2
	bl 0x0200c46c
	movs r0, #1
	movs r1, #2
	bl 0x0200c46c
	movs r0, #2
	movs r1, #2
	bl 0x0200c46c
	movs r1, #2
	movs r0, #3
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #4
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #2
	ldr r1, [pc, #596]
	movs r2, #60
	bl 0x0200c4cc
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200c4d4
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200c4d4
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	bl 0x0200c4d4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200c4d4
	movs r0, #60
	bl 0x0200c3d4
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #20
	movs r0, #8
	bl 0x0200c4c4
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #0
	ldr r1, [pc, #460]
	movs r2, #0
	bl 0x0200c4cc
	movs r0, #1
	ldr r1, [pc, #448]
	movs r2, #0
	bl 0x0200c4cc
	movs r0, #2
	ldr r1, [pc, #440]
	movs r2, #0
	bl 0x0200c4cc
	movs r2, #60
	movs r0, #3
	ldr r1, [pc, #428]
	bl 0x0200c4cc
	movs r1, #1
	movs r0, #1
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #0
	movs r1, #1
	movs r2, #0
	bl 0x0200c47c
	movs r0, #2
	movs r1, #1
	movs r2, #0
	bl 0x0200c47c
	movs r2, #0
	movs r1, #1
	movs r0, #3
	bl 0x0200c47c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #0
	movs r1, #3
	bl 0x0200c454
	movs r0, #2
	movs r1, #3
	bl 0x0200c454
	movs r1, #3
	movs r0, #3
	bl 0x0200c45c
	movs r0, #60
	bl 0x0200c3d4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #208
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #3
	bl 0x0200c4c4
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #0
	movs r0, #8
	bl 0x0200c4bc
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200c4cc
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200c4cc
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200c4cc
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #3
	bl 0x0200c4cc
	movs r0, #60
	bl 0x0200c3d4
	movs r1, #192
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200c4c4
	movs r1, #0
	movs r0, #1
	bl 0x0200c4bc
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200c4c4
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #224
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #30
	bl 0x0200c4c4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4c4
	movs r0, #8
	ldr r1, [pc, #148]
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #2
	ldr r1, [pc, #132]
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #2
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200c4c4
	movs r0, #8
	ldr r1, [pc, #104]
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #0
	ldr r1, [pc, #80]
	movs r2, #0
	bl 0x0200c4cc
	movs r0, #1
	ldr r1, [pc, #72]
	movs r2, #0
	bl 0x0200c4cc
	movs r0, #2
	ldr r1, [pc, #60]
	movs r2, #0
	bl 0x0200c4cc
	movs r0, #3
	ldr r1, [pc, #52]
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #0
	ldr r1, [pc, #32]
	movs r2, #0
	bl 0x0200c4cc
	movs r0, #1
	ldr r1, [pc, #20]
	movs r2, #0
	bl 0x0200c4cc
	movs r0, #2
	ldr r1, [pc, #12]
	movs r2, #0
	bl 0x0200c4cc
	b .L_02002ad0_4
	.4byte 0x00000103
	.4byte 0x00000101
	.4byte 0x00000105
.L_02002ad0_4:
	movs r0, #3
	ldr r1, [pc, #1020]
	movs r2, #60
	bl 0x0200c4cc
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r0, #0
	movs r1, #3
	bl 0x0200c454
	movs r0, #1
	movs r1, #3
	bl 0x0200c454
	movs r0, #2
	movs r1, #3
	bl 0x0200c454
	movs r1, #3
	movs r0, #3
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #1
	movs r0, #8
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #3
	bl 0x0200c4c4
	movs r0, #60
	bl 0x0200c3d4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #3
	bl 0x0200c4c4
	movs r0, #30
	bl 0x0200c3d4
	movs r1, #128
	movs r2, #20
	movs r0, #8
	lsls r1, r1, #7
	bl 0x0200c4c4
	movs r1, #4
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #236
	movs r2, #152
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl 0x0200c44c
	movs r0, #10
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200c4cc
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200c4cc
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200c4cc
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200c4cc
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200c4cc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #9
	bl 0x0200c4cc
	movs r0, #60
	bl 0x0200c3d4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #0
	movs r2, #0
	movs r0, #9
	bl 0x0200c4c4
	movs r0, #30
	bl 0x0200c3d4
	movs r0, #10
	ldr r1, [pc, #632]
	ldr r2, [pc, #636]
	bl 0x0200c414
	movs r1, #236
	movs r2, #134
	movs r0, #10
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200c434
	movs r0, #8
	ldr r1, [pc, #604]
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200c4c4
	movs r1, #244
	movs r2, #128
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #2
	bl 0x0200c434
	movs r0, #10
	bl 0x0200c3d4
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200c4c4
	movs r0, #0
	movs r1, #0
	movs r2, #30
	bl 0x0200c4c4
	movs r1, #228
	movs r2, #128
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #0
	bl 0x0200c434
	movs r0, #10
	bl 0x0200c3d4
	movs r1, #128
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #6
	bl 0x0200c4c4
	movs r1, #2
	movs r0, #10
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #10
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #8
	ldr r1, [pc, #476]
	movs r2, #60
	bl 0x0200c4cc
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r0, #10
	movs r1, #2
	bl 0x0200c46c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl 0x0200c4d4
	movs r0, #60
	bl 0x0200c3d4
	movs r0, #10
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #8
	ldr r1, [pc, #420]
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200c4cc
	movs r2, #20
	movs r0, #10
	movs r1, #0
	bl 0x0200c4b4
	movs r0, #8
	movs r1, #1
	bl 0x0200c46c
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #3
	bl 0x0200c4c4
	movs r0, #40
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #10
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #10
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #4
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #8
	ldr r1, [pc, #248]
	movs r2, #60
	bl 0x0200c4cc
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #1
	movs r0, #17
	bl 0x0200c46c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #10
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #8
	ldr r1, [pc, #216]
	movs r2, #60
	bl 0x0200c4cc
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #30
	bl 0x0200c4cc
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #0
	bl 0x0200c454
	movs r0, #2
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #2
	bl 0x0200c454
	movs r0, #1
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #3
	bl 0x0200c454
	movs r0, #5
	bl 0x0200c3d4
	movs r0, #1
	movs r1, #3
	bl 0x0200c45c
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl 0x0200c47c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #10
	ldr r1, [pc, #88]
	ldr r2, [pc, #92]
	bl 0x0200c414
	movs r1, #236
	movs r2, #252
	lsls r2, r2, #1
	lsls r1, r1, #1
	movs r0, #10
	bl 0x0200c434
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #1
	movs r0, #10
	bl 0x0200c46c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #10
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #8
	movs r1, #10
	movs r2, #0
	bl 0x0200c47c
	movs r0, #8
	ldr r1, [pc, #24]
	ldr r2, [pc, #28]
	bl 0x0200c414
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c4c4
	b .L_02002ad0_5
	.2byte 0x0000
	.4byte 0x00000101
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00000105
.L_02002ad0_5:
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #3
	bl 0x0200c4c4
	movs r0, #8
	bl 0x0200c444
	movs r0, #30
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #236
	movs r2, #142
	movs r0, #10
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200c42c
	movs r1, #236
	movs r2, #134
	lsls r2, r2, #2
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200c434
	movs r1, #2
	movs r0, #1
	bl 0x0200c474
	movs r0, #10
	bl 0x0200c444
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200c4b4
	movs r0, #8
	movs r1, #2
	bl 0x0200c46c
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200c4cc
	movs r1, #208
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #208
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200c4c4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r0, #0
	movs r1, #2
	bl 0x0200c46c
	movs r0, #1
	movs r1, #2
	bl 0x0200c46c
	movs r0, #2
	movs r1, #2
	bl 0x0200c46c
	movs r0, #3
	movs r1, #2
	bl 0x0200c474
	movs r1, #236
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200c434
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200c4c4
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #208
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200c4c4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #2
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #0
	bl 0x0200c404
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #224
	movs r2, #128
	lsls r1, r1, #1
	strb r3, [r0]
	lsls r2, r2, #2
	movs r0, #0
	bl 0x0200c434
	movs r0, #1
	bl 0x0200c3d4
	movs r0, #0
	bl 0x0200c404
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #20
	bl 0x0200c3d4
	bl 0x0200c1ec
	movs r0, #60
	bl 0x0200c3d4
	movs r0, #2
	movs r1, #144
	bl 0x0200c3fc
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r0, #0
	movs r1, #3
	bl 0x0200c454
	movs r0, #1
	movs r1, #3
	bl 0x0200c454
	movs r0, #2
	movs r1, #3
	bl 0x0200c454
	movs r1, #3
	movs r0, #3
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #9
	bl 0x0200c4c4
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #236
	movs r2, #138
	lsls r2, r2, #2
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200c434
	movs r1, #2
	movs r0, #8
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #208
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200c4c4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200c414
	movs r1, #240
	movs r2, #135
	movs r0, #8
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200c434
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #8
	ldr r1, [pc, #740]
	ldr r2, [pc, #744]
	bl 0x0200c414
	movs r0, #9
	ldr r1, [pc, #732]
	ldr r2, [pc, #732]
	bl 0x0200c414
	movs r1, #236
	movs r2, #152
	movs r0, #8
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200c42c
	movs r1, #236
	movs r2, #136
	movs r0, #9
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200c42c
	movs r1, #236
	movs r2, #152
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #10
	bl 0x0200c434
	movs r0, #9
	bl 0x0200c444
	movs r1, #236
	movs r2, #152
	movs r0, #9
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200c42c
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl 0x0200c44c
	movs r0, #8
	bl 0x0200c444
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl 0x0200c44c
	movs r0, #9
	bl 0x0200c444
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200c44c
	movs r1, #244
	movs r2, #130
	movs r0, #2
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200c434
	movs r2, #60
	movs r0, #2
	ldr r1, [pc, #608]
	bl 0x0200c4cc
	movs r1, #1
	movs r0, #3
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200c4c4
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #160
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200c4c4
	movs r1, #4
	movs r0, #2
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #2
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r2, #60
	movs r0, #3
	ldr r1, [pc, #520]
	bl 0x0200c4cc
	movs r0, #1
	movs r1, #1
	bl 0x0200c474
	movs r1, #192
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200c4c4
	movs r1, #0
	movs r0, #1
	bl 0x0200c4bc
	movs r1, #3
	movs r0, #0
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #2
	movs r0, #3
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c4c4
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #0
	ldr r1, [pc, #420]
	movs r2, #0
	bl 0x0200c4cc
	movs r2, #60
	movs r0, #1
	ldr r1, [pc, #408]
	bl 0x0200c4cc
	movs r1, #4
	movs r0, #3
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200c4cc
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200c4cc
	movs r1, #0
	movs r0, #3
	bl 0x0200c4a4
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f4
	cmp r0, #0
	bne .L_02002ad0_6
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #1
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200c4b4
	ldr r3, [pc, #312]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02002ad0_7
.L_02002ad0_6:
	movs r0, #20
	bl 0x0200c3d4
	ldr r3, [pc, #288]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r1, #3
	movs r0, #3
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
.L_02002ad0_7:
	movs r0, #1
	movs r1, #1
	bl 0x0200c474
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200c4b4
	movs r0, #0
	movs r1, #3
	bl 0x0200c454
	movs r1, #3
	movs r0, #3
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #224
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200c42c
	movs r1, #224
	movs r2, #128
	movs r0, #3
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200c434
	movs r1, #0
	movs r2, #0
	movs r0, #3
	bl 0x0200c44c
	movs r0, #1
	bl 0x0200c444
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x0200c44c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #2
	movs r2, #20
	movs r0, #0
	bl 0x0200c47c
	movs r0, #30
	bl 0x0200c3d4
	movs r0, #2
	movs r1, #0
	movs r2, #20
	bl 0x0200c47c
	movs r0, #2
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #129
	movs r2, #60
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200c4cc
	movs r0, #2
	movs r1, #2
	bl 0x0200c46c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #2
	bl 0x0200c4d4
	movs r0, #60
	bl 0x0200c3d4
	movs r1, #4
	movs r0, #2
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #2
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r1, #224
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200c434
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200c44c
	bl 0x0200c3e4
	ldr r0, [pc, #28]
	bl 0x0200c3c4
	pop {r0}
	bx r0
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00000101
	.4byte 0x00000105
	.4byte 0x03001ebc
	.4byte 0x00000895
	.global Func_02003e58
	.thumb_func
Func_02003e58:
	push {r5, lr}
	ldr r5, [pc, #576]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #568]
	sub sp, #8
	cmp r2, r3
	beq .L_02003e58_0
	b .L_02003e58_1
.L_02003e58_0:
	ldr r3, [pc, #560]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #225
	adds r2, #73
	str r2, [r3]
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_02003e58_2
	ldr r0, [pc, #536]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_02003e58_3
	movs r0, #8
	movs r1, #6
	bl 0x0200c454
	b .L_02003e58_4
.L_02003e58_3:
	movs r0, #8
	movs r1, #5
	bl 0x0200c454
	ldr r0, [pc, #512]
	bl 0x0200c3bc
	cmp r0, #0
	bne .L_02003e58_5
	b .L_02003e58_4
.L_02003e58_5:
	ldr r0, [pc, #504]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_02003e58_6
	b .L_02003e58_4
.L_02003e58_6:
	ldr r0, [pc, #496]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_02003e58_7
	b .L_02003e58_4
.L_02003e58_7:
	bl 0x0200a7ec
	b .L_02003e58_4
.L_02003e58_2:
	cmp r3, #2
	beq .L_02003e58_8
	cmp r3, #4
	bne .L_02003e58_9
.L_02003e58_8:
	ldr r0, [pc, #476]
	bl 0x0200c3cc
	ldr r0, [pc, #472]
	bl 0x0200c3bc
	adds r5, r0, #0
	cmp r5, #0
	bne .L_02003e58_10
	movs r0, #19
	bl 0x0200c404
	adds r3, r0, #0
	adds r3, #85
	strb r5, [r3]
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #12]
	str r3, [r0, #60]
	ldr r3, [pc, #444]
	movs r2, #128
	str r3, [r0, #24]
	ldr r3, [r0, #80]
	lsls r2, r2, #8
	str r2, [r0, #28]
	strh r2, [r3, #30]
	ldr r0, [pc, #436]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_02003e58_11
	movs r1, #248
	movs r2, #208
	movs r0, #18
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200c44c
	ldr r0, [pc, #416]
	bl 0x0200c3bc
	cmp r0, #0
	bne .L_02003e58_11
	movs r1, #128
	movs r2, #240
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #16
	bl 0x0200c44c
	movs r0, #18
	bl 0x0200c404
	ldr r5, [pc, #388]
	str r5, [r0, #108]
	movs r0, #13
	bl 0x0200c404
	str r5, [r0, #108]
	movs r0, #14
	bl 0x0200c404
	str r5, [r0, #108]
	movs r0, #15
	bl 0x0200c404
	str r5, [r0, #108]
	movs r0, #16
	bl 0x0200c404
	str r5, [r0, #108]
	b .L_02003e58_11
.L_02003e58_10:
	movs r0, #19
	bl 0x0200c404
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #12]
	str r3, [r0, #60]
	movs r1, #128
	ldr r3, [pc, #316]
	lsls r1, r1, #8
	str r3, [r0, #24]
	str r1, [r0, #28]
	movs r3, #89
	adds r3, r3, r0
	ldrb r2, [r3]
	mov r12, r3
	movs r3, #8
	orrs r3, r2
	mov r2, r12
	strb r3, [r2]
	ldr r3, [r0, #80]
	movs r2, #10
	strh r1, [r3, #30]
	movs r3, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl 0x0200c39c
.L_02003e58_11:
	movs r0, #152
	movs r1, #192
	movs r2, #224
	lsls r1, r1, #13
	lsls r2, r2, #16
	movs r3, #223
	lsls r0, r0, #17
	bl 0x02008048
	movs r0, #10
	movs r1, #5
	bl 0x0200c454
	movs r0, #11
	movs r1, #5
	bl 0x0200c454
	b .L_02003e58_4
.L_02003e58_9:
	cmp r3, #3
	bne .L_02003e58_4
	ldr r0, [pc, #220]
	bl 0x0200c3cc
	ldr r0, [pc, #216]
	bl 0x0200c3bc
	cmp r0, #0
	bne .L_02003e58_13
	bl 0x0200aad0
	b .L_02003e58_4
.L_02003e58_13:
	ldr r0, [pc, #220]
	bl 0x0200c3bc
	cmp r0, #0
	bne .L_02003e58_4
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200c44c
	movs r0, #9
	movs r1, #0
	movs r2, #0
.L_02003e58_12:
	bl 0x0200c44c
	b .L_02003e58_4
.L_02003e58_1:
	movs r0, #170
	bl 0x0200c564
	movs r0, #9
	bl 0x0200c404
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #16
	movs r1, #225
	orrs r3, r2
	lsls r1, r1, #1
	strb r3, [r0]
	adds r3, r5, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_02003e58_14
	ldr r0, [pc, #116]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_02003e58_14
	ldr r0, [pc, #148]
	bl 0x0200c3bc
	cmp r0, #0
	bne .L_02003e58_14
	movs r3, #10
	movs r2, #24
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #10
	movs r1, #84
	movs r2, #1
	movs r3, #1
	bl 0x0200c39c
.L_02003e58_14:
	ldr r0, [pc, #120]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_02003e58_4
	movs r1, #152
	movs r2, #196
	movs r0, #9
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x0200c44c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200c4c4
	movs r3, #10
	movs r2, #22
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #10
	movs r1, #26
	movs r2, #1
	movs r3, #1
	bl 0x0200c39c
.L_02003e58_4:
	movs r0, #0
	sub sp, #-8
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x0000003d
	.4byte 0x03001ebc
	.4byte 0x0000088f
	.4byte 0x00000f14
	.4byte 0x00000893
	.4byte 0x00000109
	.4byte 0x0000012f
	.4byte 0x00000895
	.4byte 0x0000cccc
	.4byte 0x0000089a
	.4byte 0x0000089b
	.4byte 0x02008325
	.4byte 0x000008b2
	.4byte 0x00000894
	.4byte 0x00000892
	.global Func_020040dc
	.thumb_func
Func_020040dc:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r6, r0, #0
	movs r0, #160
	lsls r0, r0, #1
	mov r8, r1
	bl 0x0200c3c4
	movs r0, #141
	movs r1, #1
	bl 0x0200c544
	ldr r3, [pc, #44]
	ldr r5, [r3]
	adds r0, r6, #0
	mov r1, r8
	bl 0x0200c54c
	adds r5, #35
	movs r3, #0
	strb r3, [r5]
	bl 0x0200c55c
	movs r0, #1
	bl 0x0200c53c
	movs r0, #1
	bl 0x0200c344
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001f30
	.global Func_02004128
	.thumb_func
Func_02004128:
	push {lr}
	movs r1, #1
	movs r0, #0
	bl 0x0200c454
	movs r0, #2
	bl 0x0200c53c
	bl 0x0200c554
	pop {r0}
	bx r0
	.global Func_02004140
	.thumb_func
Func_02004140:
	push {r5, r6, r7, lr}
	movs r0, #8
	sub sp, #56
	bl 0x0200c404
	movs r3, #1
	add r5, sp, #16
	str r3, [r5]
	ldr r3, [pc, #92]
	strh r3, [r5, #24]
	ldr r3, [pc, #92]
	str r3, [r5, #28]
	movs r3, #224
	lsls r3, r3, #10
	str r3, [r5, #16]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r5, #20]
	adds r7, r0, #0
	movs r6, #0
.L_02004140_1:
	movs r0, #10
	bl 0x0200c3d4
	movs r3, #1
	ands r3, r6
	cmp r3, #0
	beq .L_02004140_0
	movs r0, #130
	bl 0x0200c57c
.L_02004140_0:
	ldr r2, [r7, #16]
	ldr r3, [pc, #56]
	adds r2, r2, r3
	ldr r3, [pc, #56]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	ldr r3, [pc, #48]
	adds r6, #1
	str r3, [sp, #8]
	movs r3, #0
	str r5, [sp, #12]
	bl 0x0200813c
	cmp r6, #7
	bls .L_02004140_1
	movs r0, #60
	bl 0x0200c3d4
	sub sp, #-56
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000119
	.4byte 0x0200d1d8
	.4byte 0xffe80000
	.4byte 0x00009999
	.4byte 0x00360001
	.global Func_020041c4
	.thumb_func
Func_020041c4:
	push {lr}
	ldr r3, [pc, #32]
	ldr r3, [r3]
	movs r2, #1
	lsrs r3, r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_020041c4_0
	movs r1, #10
	bl 0x0200c494
	b .L_020041c4_1
.L_020041c4_0:
	movs r1, #9
	bl 0x0200c494
.L_020041c4_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e40
	.global Func_020041ec
	.thumb_func
Func_020041ec:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #131
	sub sp, #56
	bl 0x0200c57c
	movs r0, #8
	bl 0x0200c404
	ldr r5, [pc, #288]
	str r5, [r0, #108]
	movs r0, #40
	bl 0x0200c3d4
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl 0x0200c514
	movs r1, #1
	ldr r0, [pc, #272]
	bl 0x0200c50c
	movs r0, #60
	bl 0x0200c51c
	movs r0, #40
	bl 0x0200c3d4
	movs r0, #131
	bl 0x0200c57c
	movs r0, #2
	bl 0x0200c404
	str r5, [r0, #108]
	movs r0, #120
	bl 0x0200c3d4
	movs r0, #8
	bl 0x0200c404
	add r2, sp, #16
	movs r3, #1
	str r3, [r2]
	movs r3, #2
	str r3, [r2, #4]
	ldr r3, [pc, #220]
	strh r3, [r2, #24]
	mov r8, r0
	mov r10, r2
	movs r7, #0
.L_020041ec_1:
	movs r3, #3
	ands r3, r7
	cmp r3, #0
	bne .L_020041ec_0
	movs r0, #246
	bl 0x0200c57c
.L_020041ec_0:
	bl 0x0200c35c
	lsls r3, r0, #1
	adds r3, r3, r0
	mov r2, r8
	lsls r3, r3, #4
	ldr r6, [r2, #8]
	lsrs r3, r3, #16
	lsls r3, r3, #16
	adds r6, r6, r3
	ldr r3, [pc, #180]
	adds r6, r6, r3
	bl 0x0200c35c
	mov r2, r8
	lsls r0, r0, #5
	ldr r5, [r2, #12]
	lsrs r0, r0, #16
	ldr r3, [pc, #168]
	lsls r0, r0, #16
	adds r5, r5, r0
	adds r5, r5, r3
	bl 0x0200c35c
	lsls r0, r0, #2
	lsrs r0, r0, #16
	movs r2, #128
	lsls r2, r2, #8
	mov r3, r8
	lsls r0, r0, #15
	adds r0, r0, r2
	ldr r2, [r3, #16]
	movs r3, #0
	str r3, [sp, #4]
	mov r9, r3
	movs r3, #152
	lsls r3, r3, #13
	str r3, [sp, #8]
	mov r3, r10
	str r0, [sp, #0]
	str r3, [sp, #12]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r3, #0
	bl 0x0200813c
	adds r7, #1
	movs r0, #2
	bl 0x0200c344
	cmp r7, #63
	bls .L_020041ec_1
	movs r0, #220
	bl 0x0200c57c
.L_020042d6:
	movs r0, #30
	bl 0x0200c3d4
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl 0x0200c50c
	movs r0, #60
	bl 0x0200c51c
	movs r0, #40
	bl 0x0200c3d4
	movs r0, #8
	bl 0x0200c404
	mov r2, r9
	str r2, [r0, #108]
	movs r0, #2
	bl 0x0200c404
	mov r3, r9
	str r3, [r0, #108]
	movs r1, #0
	movs r0, #8
	bl 0x0200c48c
	movs r0, #2
	movs r1, #0
	bl 0x0200c48c
	sub sp, #-56
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0xc1c5
	.2byte 0x0200
	.2byte 0x5c54
	.2byte 0x0020
	.2byte 0x011d
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfff4
	.2byte 0x0000
	.2byte 0xfff0
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/IMPORT.INC"
	.section .rodata,"a",%progbits
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
	.4byte 0x0200c584
	.4byte 0x0200c5bc
	.4byte 0x0200c5f4
	.4byte 0x0000001c
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000015
	.4byte 0x00009999
	.4byte 0x80010000
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00030000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00050000
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x02008315
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x002e0042
	.4byte 0x00020003
	.4byte 0x003f0005
	.4byte 0x0003002e
	.4byte 0x00050002
	.4byte 0x003effff
	.4byte 0x00020013
	.4byte 0x00050002
	.4byte 0x00130040
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x0013003e
	.4byte 0x00020002
	.4byte 0x003c0005
	.4byte 0x00020013
	.4byte 0x00050002
	.4byte 0x0000ffff
	.4byte 0xffff0000
	.4byte 0x00000128
	.4byte 0xc00001e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000128
	.4byte 0xc00001f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000128
	.4byte 0x40000130
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x000000a8
	.4byte 0x40000168
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000000e8
	.4byte 0x40000148
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
	.4byte 0x000000a8
	.4byte 0xc0000228
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000a8
	.4byte 0xc0000228
	.4byte 0x00300000
	.4byte 0x01200180
	.4byte 0x00000240
	.4byte 0xffff0002
	.4byte 0x00000100
	.4byte 0xc0000108
	.4byte 0x00300000
	.4byte 0x01700050
	.4byte 0x00000120
	.4byte 0xffff0003
	.4byte 0x000001d8
	.4byte 0xc0000228
	.4byte 0x01600000
	.4byte 0x02500180
	.4byte 0x00000240
	.4byte 0xffff0004
	.4byte 0x00000130
	.4byte 0x400000f8
	.4byte 0x00300000
	.4byte 0x01700050
	.4byte 0x00000120
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0010c002
	.4byte 0x0020103d
	.4byte 0x0030103e
	.4byte 0x0000003d
	.4byte 0x0010203c
	.4byte 0x0020a048
	.4byte 0x00302058
	.4byte 0x00408049
	.4byte 0x000001ff
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00004000
	.4byte 0xffff0095
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff0097
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00004000
	.4byte 0xffff0097
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00008000
	.4byte 0xffff0097
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00004000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000033
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00024000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x0000c000
	.4byte 0x00000082
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00018000
	.4byte 0x00000083
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00018000
	.4byte 0x00000083
	.4byte 0x0200c638
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0x00000083
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00000066
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x0001c000
	.4byte 0x00000082
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00012000
	.4byte 0x00000082
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00004000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00008000
	.4byte 0xffff0016
	.4byte 0x0200c750
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00004000
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
	.4byte 0x0002c000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff003d
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x0001c000
	.4byte 0xffff0097
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200a779
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x0200a6d9
	.4byte 0x00000002
	.4byte 0x08940005
	.4byte 0x02008659
	.4byte 0x00000000
	.4byte 0x188f0008
	.4byte 0x000017d2
	.4byte 0x00000000
	.4byte 0x188f0009
	.4byte 0x000017d3
	.4byte 0x00000000
	.4byte 0x188f000a
	.4byte 0x000017d4
	.4byte 0x00000000
	.4byte 0x188f000b
	.4byte 0x000017d5
	.4byte 0x00000000
	.4byte 0x188f000c
	.4byte 0x02008485
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008415
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008415
	.4byte 0x00000000
	.4byte 0x08910009
	.4byte 0x00001791
	.4byte 0x00000000
	.4byte 0x08920009
	.4byte 0x02008519
	.4byte 0x00000000
	.4byte 0x0f140009
	.4byte 0x000017b5
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000017bc
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001792
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001793
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008485
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200a509
	.4byte 0x00000003
	.4byte 0x0350006e
	.4byte 0x00300000
	.4byte 0x00008d15
	.4byte 0x188f0008
	.4byte 0x000017d9
	.4byte 0x00008d15
	.4byte 0x188f0009
	.4byte 0x000017da
	.4byte 0x00008d15
	.4byte 0x188f000a
	.4byte 0x000017db
	.4byte 0x00008d15
	.4byte 0x188f000b
	.4byte 0x000017dc
	.4byte 0x00008d15
	.4byte 0x188f000c
	.4byte 0x000017dd
	.4byte 0x00008d15
	.4byte 0x08900008
	.4byte 0x0000178d
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001799
	.4byte 0x00008d15
	.4byte 0x08910009
	.4byte 0x0000179a
	.4byte 0x00008d15
	.4byte 0x08920009
	.4byte 0x000017b6
	.4byte 0x00008d15
	.4byte 0x0f140009
	.4byte 0x000017b6
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000017bd
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000179b
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000179c
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000179d
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001a1d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004402
	.4byte 0xffff0001
	.4byte 0x0200a765
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x0200a485
	.4byte 0x00000000
	.4byte 0x188f0008
	.4byte 0x000017de
	.4byte 0x00000000
	.4byte 0x18930008
	.4byte 0x000017ce
	.4byte 0x00000000
	.4byte 0x18910008
	.4byte 0x000017b0
	.4byte 0x00000000
	.4byte 0x13000008
	.4byte 0x0200871d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000179e
	.4byte 0x00008d15
	.4byte 0x188f0008
	.4byte 0x0200a54d
	.4byte 0x00008d15
	.4byte 0x18930008
	.4byte 0x000017cf
	.4byte 0x00008d15
	.4byte 0x18910408
	.4byte 0x02008abd
	.4byte 0x00008d15
	.4byte 0xffff0408
	.4byte 0x0200871d
	.4byte 0x00000000
	.4byte 0x18950009
	.4byte 0x00001a56
	.4byte 0x00000000
	.4byte 0x1895000a
	.4byte 0x00001a57
	.4byte 0x00000000
	.4byte 0x1895000b
	.4byte 0x0200a465
	.4byte 0x00000000
	.4byte 0x1895000c
	.4byte 0x02008afd
	.4byte 0x00000000
	.4byte 0x1895000e
	.4byte 0x00001a5c
	.4byte 0x00000000
	.4byte 0x18950010
	.4byte 0x00001a5d
	.4byte 0x00000000
	.4byte 0x189b0012
	.4byte 0x0000189a
	.4byte 0x00000000
	.4byte 0x189b0009
	.4byte 0x0000189b
	.4byte 0x00000000
	.4byte 0x189b000a
	.4byte 0x0000189c
	.4byte 0x00000000
	.4byte 0x189b000b
	.4byte 0x0000189d
	.4byte 0x00000000
	.4byte 0x189b000c
	.4byte 0x02008afd
	.4byte 0x00000000
	.4byte 0x189b000d
	.4byte 0x0000189f
	.4byte 0x00000000
	.4byte 0x189b000e
	.4byte 0x000018a0
	.4byte 0x00000000
	.4byte 0x189b000f
	.4byte 0x000018a1
	.4byte 0x00000000
	.4byte 0x189b0010
	.4byte 0x000018a2
	.4byte 0x00000000
	.4byte 0x13010012
	.4byte 0x0000187f
	.4byte 0x00000000
	.4byte 0x13010010
	.4byte 0x00001880
	.4byte 0x00000000
	.4byte 0x18980012
	.4byte 0x00001864
	.4byte 0x00000000
	.4byte 0x1898000d
	.4byte 0x00001865
	.4byte 0x00000000
	.4byte 0x1898000e
	.4byte 0x00001866
	.4byte 0x00000000
	.4byte 0x1898000f
	.4byte 0x00001867
	.4byte 0x00000000
	.4byte 0x18980010
	.4byte 0x00001868
	.4byte 0x00000000
	.4byte 0x18990012
	.4byte 0x02009d51
	.4byte 0x00000000
	.4byte 0x1899000d
	.4byte 0x00001871
	.4byte 0x00000000
	.4byte 0x1899000e
	.4byte 0x00001872
	.4byte 0x00000000
	.4byte 0x1899000f
	.4byte 0x00001873
	.4byte 0x00000000
	.4byte 0x18990010
	.4byte 0x00001874
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008add
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001828
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001829
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008afd
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000182b
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000182c
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008bd5
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001830
	.4byte 0x00008d15
	.4byte 0x18950009
	.4byte 0x00001a5e
	.4byte 0x00008d15
	.4byte 0x1895000a
	.4byte 0x00001a5f
	.4byte 0x00008d15
	.4byte 0x1895000b
	.4byte 0x00001a60
	.4byte 0x00008d15
	.4byte 0x1895000c
	.4byte 0x00001a61
	.4byte 0x00008d15
	.4byte 0x1895000e
	.4byte 0x00001a62
	.4byte 0x00008d15
	.4byte 0x18950010
	.4byte 0x00001a63
	.4byte 0x00008d15
	.4byte 0x189b0012
	.4byte 0x000018ab
	.4byte 0x00008d15
	.4byte 0x189b0009
	.4byte 0x000018a3
	.4byte 0x00008d15
	.4byte 0x189b000a
	.4byte 0x000018a4
	.4byte 0x00008d15
	.4byte 0x189b000b
	.4byte 0x000018a5
	.4byte 0x00008d15
	.4byte 0x189b000c
	.4byte 0x000018a6
	.4byte 0x00008d15
	.4byte 0x189b000d
	.4byte 0x000018a7
	.4byte 0x00008d15
	.4byte 0x189b000e
	.4byte 0x000018a8
	.4byte 0x00008d15
	.4byte 0x189b000f
	.4byte 0x000018a9
	.4byte 0x00008d15
	.4byte 0x189b0010
	.4byte 0x000018aa
	.4byte 0x00008d15
	.4byte 0x13010012
	.4byte 0x00001881
	.4byte 0x00008d15
	.4byte 0x13010010
	.4byte 0x00001882
	.4byte 0x00008d15
	.4byte 0x18980012
	.4byte 0x00001869
	.4byte 0x00008d15
	.4byte 0x1898000d
	.4byte 0x0000186a
	.4byte 0x00008d15
	.4byte 0x1898000e
	.4byte 0x0000186b
	.4byte 0x00008d15
	.4byte 0x1898000f
	.4byte 0x0000186c
	.4byte 0x00008d15
	.4byte 0x18980010
	.4byte 0x0000186d
	.4byte 0x00008d15
	.4byte 0x18990012
	.4byte 0x00001875
	.4byte 0x00008d15
	.4byte 0x1899000d
	.4byte 0x00001876
	.4byte 0x00008d15
	.4byte 0x1899000e
	.4byte 0x00001877
	.4byte 0x00008d15
	.4byte 0x1899000f
	.4byte 0x00001878
	.4byte 0x00008d15
	.4byte 0x18990010
	.4byte 0x00001879
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001831
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001832
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001833
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001834
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001835
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001836
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001837
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001838
	.4byte 0x00008602
	.4byte 0xffff000a
	.4byte 0x02009335
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x02009335
	.4byte 0x00000602
	.4byte 0xffff000b
	.4byte 0x02009335
	.4byte 0x00004602
	.4byte 0xffff000b
	.4byte 0x02009335
	.4byte 0x00008c15
	.4byte 0xffff0013
	.4byte 0x020092e1
	.4byte 0x00008e15
	.4byte 0xffff0014
	.4byte 0x020093b9
	.4byte 0x00000033
	.4byte 0x0f690064
	.4byte 0x001000bb
	.4byte 0x00000023
	.4byte 0x0f680065
	.4byte 0x0010010a
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001a09
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001a0a
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001a10
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001a11
	.4byte 0x00000023
	.4byte 0x0f6a0066
	.4byte 0x00200006
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
