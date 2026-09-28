.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/ENTRY.INC"
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
	bl 0x02009a70
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
	bl 0x02009aa0
	adds r0, r5, #0
	movs r1, #14
	bl 0x02009b48
	adds r0, r5, #0
	movs r1, #1
	bl 0x02009aa8
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
	bl 0x02009a70
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
	bl 0x02009aa0
	adds r0, r5, #0
	movs r1, #15
	bl 0x02009b48
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
	bl 0x02009af8
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
	bl 0x02009a70
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
	bl 0x02009a60
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl 0x02009a68
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
	bl 0x02009b48
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
	bl 0x02009a10
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
	bl 0x02009a10
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200013c_9:
	bl 0x02009a10
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
	bl 0x02009a60
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl 0x02009a68
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
	.4byte 0x02009da8
	.4byte 0x02008105
	.4byte 0xffff0000
	.global Func_02000314
	.thumb_func
Func_02000314:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200a5cc
	.global Func_0200031c
	.thumb_func
Func_0200031c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200a9d4
	.global Func_02000324
	.thumb_func
Func_02000324:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200aa14
	.global Func_0200032c
	.thumb_func
Func_0200032c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200aa58
	.global Func_02000334
	.thumb_func
Func_02000334:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	str r1, [sp, #8]
	mov r11, r2
	movs r1, #238
	ldr r2, [pc, #96]
	str r0, [sp, #12]
	lsls r1, r1, #1
	mov r9, r3
	adds r3, r2, r1
	ldr r3, [r3]
	asrs r3, r3, #20
	adds r3, #64
	adds r1, #8
	mov r10, r3
	adds r3, r2, r1
	ldr r3, [r3]
	adds r1, #16
	asrs r3, r3, #20
	mov r8, r3
	adds r3, r2, r1
	ldr r7, [r3]
	adds r0, r7, #0
	bl 0x02009af8
	ldr r5, [pc, #60]
	ldr r2, [r5]
	movs r1, #0
	ldrsh r3, [r2, r1]
	adds r6, r0, #0
	cmp r9, r3
	bne .L_02000334_0
	b .L_02000334_1
.L_02000334_0:
	mov r3, r9
	strh r3, [r2]
	ldr r0, [sp, #12]
	bl 0x02009ac8
	cmp r0, #0
	bne .L_02000334_2
	mov r1, r10
	mov r3, r8
	str r1, [sp, #0]
	str r3, [sp, #4]
	ldr r0, [sp, #8]
	mov r1, r11
	movs r2, #1
	movs r3, #1
	bl 0x02009a90
	ldr r0, [sp, #12]
	bl 0x02009ad0
	b .L_02000334_1
	.4byte 0x02000240
	.4byte 0x0200add0
.L_02000334_2:
	ldr r2, [r5]
	ldr r3, [pc, #60]
	strh r3, [r2]
	mov r3, r10
	str r3, [sp, #0]
	mov r1, r11
	mov r3, r8
	str r3, [sp, #4]
	ldr r0, [sp, #8]
	movs r2, #1
	movs r3, #1
	adds r1, #1
	bl 0x02009a90
	movs r0, #206
	bl 0x02009bf8
	bl 0x02009ae0
	ldr r0, [pc, #28]
	mov r1, r9
	bl 0x02009ba8
	adds r0, r7, #0
	movs r1, #27
	bl 0x02009b20
	adds r0, r7, #0
	bl 0x02009af8
	movs r1, #0
	b .L_02000334_3
	.4byte 0xffffffff
	.4byte 0x0000002d
.L_02000334_3:
	bl 0x02009aa0
	adds r0, r7, #0
	ldr r1, [pc, #116]
	bl 0x02009b88
	movs r0, #30
	bl 0x02009ad8
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	negs r0, r0
	movs r3, #0
	bl 0x02009b98
	adds r3, r6, #0
	movs r5, #2
	adds r3, #85
	strb r5, [r3]
	ldr r3, [pc, #80]
	str r3, [r6, #20]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r6, #72]
	movs r0, #204
	bl 0x02009bf8
	movs r0, #3
	bl 0x02009ad8
	adds r3, r6, #0
	adds r3, #34
	strb r5, [r3]
	adds r0, r7, #0
	movs r1, #3
	bl 0x02009b78
	ldr r7, [pc, #36]
	movs r5, #29
.L_02000334_4:
	ldrh r3, [r6, #6]
	adds r3, r3, r7
	strh r3, [r6, #6]
	movs r0, #1
	subs r5, #1
	bl 0x02009a20
	cmp r5, #0
	bge .L_02000334_4
	mov r1, r9
	cmp r1, #50
	beq .L_02000334_1
	movs r0, #145
	lsls r0, r0, #1
	bl 0x02009ad0
	b .L_02000334_1
	.2byte 0x0000
	.4byte 0x00002000
	.4byte 0x00000101
	.4byte 0xff600000
.L_02000334_1:
	sub sp, #-16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000494
	.thumb_func
Func_02000494:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	movs r1, #64
	movs r2, #35
	movs r3, #21
	bl 0x02008334
	pop {r0}
	bx r0
	.global Func_020004a8
	.thumb_func
Func_020004a8:
	push {lr}
	ldr r0, [pc, #16]
	movs r1, #65
	movs r2, #35
	movs r3, #22
	bl 0x02008334
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000201
	.global Func_020004c0
	.thumb_func
Func_020004c0:
	push {lr}
	ldr r0, [pc, #16]
	movs r1, #66
	movs r2, #35
	movs r3, #23
	bl 0x02008334
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000202
	.global Func_020004d8
	.thumb_func
Func_020004d8:
	push {lr}
	ldr r0, [pc, #16]
	movs r1, #67
	movs r2, #35
	movs r3, #24
	bl 0x02008334
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000203
	.global Func_020004f0
	.thumb_func
Func_020004f0:
	push {lr}
	movs r0, #129
	lsls r0, r0, #2
	movs r1, #68
	movs r2, #35
	movs r3, #25
	bl 0x02008334
	pop {r0}
	bx r0
	.global Func_02000504
	.thumb_func
Func_02000504:
	push {lr}
	ldr r0, [pc, #16]
	movs r1, #69
	movs r2, #35
	movs r3, #26
	bl 0x02008334
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000205
	.global Func_0200051c
	.thumb_func
Func_0200051c:
	push {lr}
	ldr r0, [pc, #16]
	movs r1, #70
	movs r2, #35
	movs r3, #27
	bl 0x02008334
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000206
	.global Func_02000534
	.thumb_func
Func_02000534:
	push {lr}
	ldr r0, [pc, #16]
	movs r1, #71
	movs r2, #35
	movs r3, #28
	bl 0x02008334
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000207
	.global Func_0200054c
	.thumb_func
Func_0200054c:
	push {lr}
	movs r0, #130
	lsls r0, r0, #2
	movs r1, #72
	movs r2, #35
	movs r3, #29
	bl 0x02008334
	pop {r0}
	bx r0
	.global Func_02000560
	.thumb_func
Func_02000560:
	push {lr}
	ldr r0, [pc, #16]
	movs r1, #73
	movs r2, #35
	movs r3, #31
	bl 0x02008334
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000209
	.global Func_02000578
	.thumb_func
Func_02000578:
	push {lr}
	ldr r0, [pc, #16]
	movs r1, #74
	movs r2, #35
	movs r3, #32
	bl 0x02008334
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000020a
	.global Func_02000590
	.thumb_func
Func_02000590:
	push {lr}
	ldr r0, [pc, #16]
	movs r1, #79
	movs r2, #35
	movs r3, #50
	bl 0x02008334
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000020b
	.global Func_020005a8
	.thumb_func
Func_020005a8:
	push {lr}
	movs r0, #131
	lsls r0, r0, #2
	movs r1, #75
	movs r2, #35
	movs r3, #51
	bl 0x02008334
	pop {r0}
	bx r0
	.global Func_020005bc
	.thumb_func
Func_020005bc:
	push {lr}
	ldr r0, [pc, #16]
	movs r1, #76
	movs r2, #35
	movs r3, #52
	bl 0x02008334
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000020d
	.global Func_020005d4
	.thumb_func
Func_020005d4:
	push {lr}
	ldr r0, [pc, #16]
	movs r1, #77
	movs r2, #35
	movs r3, #53
	bl 0x02008334
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000020e
	.global Func_020005ec
	.thumb_func
Func_020005ec:
	push {lr}
	ldr r0, [pc, #16]
	movs r1, #78
	movs r2, #35
	movs r3, #54
	bl 0x02008334
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000020f
	.global Func_02000604
	.thumb_func
Func_02000604:
	push {lr}
	movs r0, #132
	lsls r0, r0, #2
	movs r1, #80
	movs r2, #35
	movs r3, #55
	bl 0x02008334
	pop {r0}
	bx r0
	.global Func_02000618
	.thumb_func
Func_02000618:
	push {lr}
	ldr r0, [pc, #16]
	movs r1, #81
	movs r2, #35
	movs r3, #56
	bl 0x02008334
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000211
	.global Func_02000630
	.thumb_func
Func_02000630:
	push {lr}
	ldr r0, [pc, #16]
	movs r1, #82
	movs r2, #35
	movs r3, #57
	bl 0x02008334
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000212
	.global Func_02000648
	.thumb_func
Func_02000648:
	push {lr}
	ldr r0, [pc, #16]
	movs r1, #83
	movs r2, #35
	movs r3, #58
	bl 0x02008334
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000213
	.global Func_02000660
	.thumb_func
Func_02000660:
	push {lr}
	movs r0, #133
	lsls r0, r0, #2
	movs r1, #84
	movs r2, #35
	movs r3, #59
	bl 0x02008334
	pop {r0}
	bx r0
	.global Func_02000674
	.thumb_func
Func_02000674:
	push {lr}
	bl 0x02009bc0
	cmp r0, #0
	bne .L_02000674_0
	ldr r3, [pc, #16]
	ldr r2, [r3]
	ldr r3, [pc, #8]
	strh r3, [r2]
.L_02000674_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xffffffff
	.4byte 0x0200add0
	.global Func_02000694
	.thumb_func
Func_02000694:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200aae8
	.global Func_0200069c
	.thumb_func
Func_0200069c:
	push {r5, r6, lr}
	movs r0, #3
	bl 0x02009ac8
	adds r6, r0, #0
	bl 0x02009ae0
	movs r0, #17
	bl 0x02009bf8
	ldr r0, [pc, #568]
	bl 0x02009b50
	movs r1, #0
	movs r2, #20
	ldr r0, [pc, #564]
	bl 0x02009b68
	movs r0, #29
	bl 0x02009bf8
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009b00
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009b00
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009b00
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #3
	bl 0x02009b00
	movs r0, #3
	bl 0x02009af8
	adds r0, #35
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #3
	bl 0x02009b78
	movs r0, #0
	bl 0x02009af8
	adds r0, #35
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r1, #2
	movs r0, #0
	bl 0x02009b78
	movs r0, #0
	bl 0x02009af8
	cmp r0, #0
	beq .L_0200069c_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x02009b18
.L_0200069c_0:
	movs r0, #0
	bl 0x02009af8
	cmp r0, #0
	beq .L_0200069c_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x02009b18
.L_0200069c_1:
	cmp r6, #0
	beq .L_0200069c_2
	movs r0, #0
	bl 0x02009af8
	cmp r0, #0
	beq .L_0200069c_3
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x02009b18
.L_0200069c_3:
	ldr r1, [pc, #388]
	movs r0, #3
	bl 0x02009b08
.L_0200069c_2:
	ldr r1, [pc, #384]
	movs r0, #0
	bl 0x02009b08
	ldr r1, [pc, #380]
	movs r0, #1
	bl 0x02009b08
	ldr r1, [pc, #376]
	movs r0, #2
	bl 0x02009b10
	movs r0, #10
	bl 0x02009ad8
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b70
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b70
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b70
	movs r1, #192
	movs r2, #40
	movs r0, #2
	lsls r1, r1, #8
	bl 0x02009b70
	movs r1, #11
	movs r0, #8
	bl 0x02009b20
	movs r0, #10
	bl 0x02009ad8
	movs r1, #8
	movs r0, #8
	bl 0x02009b20
	movs r0, #20
	bl 0x02009ad8
	movs r0, #8
	bl 0x02009424
	ldr r0, [pc, #284]
	movs r1, #0
	bl 0x02009b60
	movs r0, #0
	movs r1, #2
	bl 0x02009b30
	movs r0, #1
	movs r1, #2
	bl 0x02009b30
	movs r0, #3
	movs r1, #2
	bl 0x02009b30
	movs r0, #2
	movs r1, #2
	bl 0x02009b30
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x02009b80
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x02009b80
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #0
	bl 0x02009b80
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #60
	movs r0, #2
	bl 0x02009b80
	movs r0, #11
	bl 0x02009424
	movs r2, #10
	ldr r0, [pc, #188]
	movs r1, #0
	bl 0x02009b68
	movs r0, #0
	movs r1, #1
	bl 0x02009b30
	movs r0, #1
	movs r1, #1
	bl 0x02009b30
	movs r0, #3
	movs r1, #1
	bl 0x02009b30
	movs r0, #2
	movs r1, #1
	bl 0x02009b38
	ldr r0, [pc, #148]
	movs r1, #0
	bl 0x02009b60
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x02009b88
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x02009b88
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	bl 0x02009b88
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #2
	bl 0x02009b88
	movs r0, #40
	bl 0x02009ad8
	movs r0, #11
	bl 0x02009424
	ldr r0, [pc, #88]
	movs r1, #0
	bl 0x02009b60
	ldr r3, [pc, #84]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #64
	str r3, [r2]
	subs r3, #56
	adds r2, r1, r3
	movs r3, #64
	str r3, [r2]
	ldr r3, [pc, #64]
	ldr r2, [pc, #68]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	ldr r0, [pc, #64]
	movs r1, #19
	bl 0x02009bb8
	movs r0, #36
	movs r1, #0
	bl 0x02009bb0
	bl 0x02009ae8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x000014ce
	.4byte 0x00008009
	.4byte 0x02009e2c
	.4byte 0x02009db4
	.4byte 0x02009ddc
	.4byte 0x02009e04
	.4byte 0x00008008
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x0000002d
	.global Func_02000918
	.thumb_func
Func_02000918:
	push {r5, r6, r7, lr}
	movs r0, #3
	sub sp, #8
	bl 0x02009ac8
	adds r7, r0, #0
	movs r0, #3
	bl 0x02009af8
	adds r0, #35
.L_0200092c:
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #3
	bl 0x02009b78
	movs r0, #0
	bl 0x02009af8
	adds r0, #35
.L_02000946:
	ldrb r3, [r0]
	ands r5, r3
	movs r1, #2
	strb r5, [r0]
	movs r0, #0
	bl 0x02009b78
	movs r0, #184
	bl 0x02009ac0
	movs r0, #17
	bl 0x02009bf8
	bl 0x02009ae0
	movs r0, #0
	ldr r1, [pc, #1008]
	ldr r2, [pc, #1008]
	bl 0x02009b00
	movs r0, #1
.L_02000970:
	ldr r1, [pc, #996]
	ldr r2, [pc, #1000]
	bl 0x02009b00
	movs r0, #2
	ldr r1, [pc, #988]
	ldr r2, [pc, #988]
	bl 0x02009b00
	movs r0, #3
	ldr r1, [pc, #976]
	ldr r2, [pc, #980]
.L_02000988:
	bl 0x02009b00
	movs r1, #166
	movs r2, #160
	lsls r1, r1, #16
	lsls r2, r2, #15
	movs r0, #0
	bl 0x02009b18
	movs r0, #0
	bl 0x02009af8
	movs r6, #192
.L_020009a2:
	lsls r6, r6, #8
	movs r1, #148
	movs r2, #180
	lsls r1, r1, #16
	lsls r2, r2, #15
	strh r6, [r0, #6]
	movs r0, #1
	bl 0x02009b18
	movs r0, #1
	bl 0x02009af8
	movs r1, #182
	movs r2, #180
	strh r6, [r0, #6]
	lsls r1, r1, #16
	movs r0, #2
	lsls r2, r2, #15
	bl 0x02009b18
	movs r0, #2
	bl 0x02009af8
	strh r6, [r0, #6]
	cmp r7, #0
	beq .L_020009a2_0
	movs r1, #166
	movs r2, #208
	movs r0, #3
	lsls r1, r1, #16
	lsls r2, r2, #15
	bl 0x02009b18
	movs r0, #3
	bl 0x02009af8
	strh r6, [r0, #6]
.L_020009a2_0:
	movs r0, #0
	bl 0x02009424
	movs r0, #10
	bl 0x02009a20
	ldr r3, [pc, #868]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	adds r3, #200
	adds r2, r1, r3
	movs r3, #48
	str r3, [r2]
	bl 0x02009be0
	bl 0x02009be8
	movs r0, #20
	bl 0x02009ad8
	ldr r0, [pc, #836]
	ldr r1, [pc, #840]
	bl 0x02009b90
	movs r0, #168
	movs r1, #1
	movs r2, #152
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #16
	bl 0x02009b98
	bl 0x02009ba0
	movs r0, #10
	bl 0x02009ad8
	movs r0, #123
	bl 0x02009bf8
	movs r3, #8
	str r3, [sp, #4]
	movs r5, #10
	movs r0, #26
	movs r1, #3
	movs r2, #1
	movs r3, #2
	str r5, [sp, #0]
	bl 0x02009a98
	movs r3, #43
	str r3, [sp, #4]
	movs r1, #38
	movs r2, #1
	movs r3, #1
	movs r0, #26
	str r5, [sp, #0]
	bl 0x02009a90
	movs r0, #4
	bl 0x02009a20
	movs r3, #42
	str r3, [sp, #4]
	movs r1, #37
	movs r2, #1
	movs r3, #2
	movs r0, #26
	str r5, [sp, #0]
	bl 0x02009a90
	movs r0, #4
	bl 0x02009a20
	movs r3, #41
	str r3, [sp, #4]
	movs r1, #36
	movs r2, #1
	movs r3, #3
	movs r0, #26
	str r5, [sp, #0]
	bl 0x02009a90
	movs r0, #4
	bl 0x02009a20
	movs r3, #40
	str r3, [sp, #4]
	movs r1, #35
	movs r3, #4
	movs r2, #1
	movs r0, #26
	str r5, [sp, #0]
	bl 0x02009a90
	movs r0, #80
	bl 0x02009a20
	ldr r0, [pc, #688]
	bl 0x02009b50
	movs r2, #20
	ldr r0, [pc, #684]
	movs r1, #0
	bl 0x02009b68
	movs r0, #0
	movs r1, #2
	bl 0x02009b30
	movs r0, #1
	movs r1, #2
	bl 0x02009b30
	movs r0, #3
	movs r1, #2
	bl 0x02009b30
	movs r1, #2
	movs r0, #2
	bl 0x02009b38
	movs r0, #20
	bl 0x02009ad8
	movs r0, #168
	movs r1, #1
	movs r2, #180
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #15
	lsls r0, r0, #16
	bl 0x02009b98
	bl 0x02009ba0
	movs r0, #40
	bl 0x02009ad8
	movs r0, #1
	bl 0x02009424
	movs r0, #60
	bl 0x02009ad8
	movs r0, #21
	bl 0x02009bf8
	movs r0, #4
	bl 0x02009424
	ldr r0, [pc, #584]
	movs r1, #0
	movs r2, #20
	bl 0x02009b68
	movs r0, #0
	ldr r1, [pc, #576]
	movs r2, #0
	bl 0x02009b80
	movs r0, #1
	ldr r1, [pc, #568]
	movs r2, #0
	bl 0x02009b80
	movs r0, #3
	ldr r1, [pc, #556]
	movs r2, #0
	bl 0x02009b80
	movs r2, #80
	movs r0, #2
	ldr r1, [pc, #544]
	bl 0x02009b80
	movs r1, #0
	ldr r0, [pc, #532]
	bl 0x02009b60
	movs r0, #40
	bl 0x02009ad8
	movs r2, #20
	ldr r0, [pc, #520]
	movs r1, #0
	bl 0x02009b68
	movs r0, #0
	movs r1, #3
	bl 0x02009b20
	movs r0, #1
	movs r1, #3
	bl 0x02009b20
	movs r0, #3
	movs r1, #3
	bl 0x02009b20
	movs r1, #3
	movs r0, #2
	bl 0x02009b28
	movs r0, #20
	bl 0x02009ad8
	movs r1, #0
	movs r2, #20
	ldr r0, [pc, #468]
	bl 0x02009b68
	movs r0, #0
	bl 0x02009424
	movs r0, #40
	bl 0x02009ad8
	movs r2, #20
	ldr r0, [pc, #448]
	movs r1, #0
	bl 0x02009b68
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x02009b88
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x02009b88
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	bl 0x02009b88
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #2
	bl 0x02009b88
	movs r0, #60
	bl 0x02009ad8
	movs r0, #1
	movs r1, #2
	bl 0x02009b38
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #10
	bl 0x02009b70
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #10
	bl 0x02009b70
	movs r2, #10
	ldr r0, [pc, #368]
	movs r1, #0
	bl 0x02009b68
	movs r0, #2
	movs r1, #4
	bl 0x02009b28
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x02009b70
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b70
	movs r1, #0
	movs r2, #20
	ldr r0, [pc, #328]
	bl 0x02009b68
	movs r0, #0
	bl 0x02009424
	movs r0, #40
	bl 0x02009ad8
	movs r2, #10
	ldr r0, [pc, #296]
	movs r1, #0
	bl 0x02009b68
	movs r0, #0
	movs r1, #2
	bl 0x02009b30
	movs r0, #1
	movs r1, #2
	bl 0x02009b30
	movs r0, #3
	movs r1, #2
	bl 0x02009b30
	movs r0, #2
	movs r1, #2
	bl 0x02009b38
	movs r0, #0
	adds r1, r6, #0
	movs r2, #0
	bl 0x02009b70
	movs r0, #1
	adds r1, r6, #0
	movs r2, #0
	bl 0x02009b70
	movs r2, #40
	adds r1, r6, #0
	movs r0, #2
	bl 0x02009b70
	movs r0, #4
	bl 0x02009424
	movs r1, #0
	ldr r0, [pc, #220]
	bl 0x02009b58
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b70
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b70
	movs r0, #0
	movs r1, #0
	bl 0x02009af0
	cmp r0, #0
	beq .L_020009a2_1
	movs r2, #20
	movs r0, #1
	ldr r1, [pc, #188]
	bl 0x02009b80
	movs r1, #4
	movs r0, #1
	bl 0x02009b20
	ldr r0, [pc, #180]
	bl 0x02009b50
	ldr r0, [pc, #160]
	movs r1, #0
	bl 0x02009b60
	movs r0, #2
	ldr r1, [pc, #160]
	movs r2, #10
	bl 0x02009b80
	movs r0, #2
	movs r1, #3
	bl 0x02009b20
	ldr r0, [pc, #140]
	movs r1, #0
	bl 0x02009b60
.L_020009a2_1:
	movs r0, #20
	bl 0x02009ad8
	movs r0, #4
	bl 0x02009424
	ldr r0, [pc, #132]
	bl 0x02009b50
	ldr r0, [pc, #100]
	movs r1, #0
	movs r2, #20
	bl 0x02009b68
	movs r2, #10
	movs r1, #0
	ldr r0, [pc, #88]
	bl 0x02009b68
	movs r0, #0
	bl 0x02009424
	movs r0, #20
	bl 0x02009ad8
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl 0x02009bd0
	movs r1, #1
	ldr r0, [pc, #84]
	bl 0x02009bc8
	movs r0, #20
	bl 0x02009bd8
	movs r0, #40
	bl 0x02009a20
	movs r0, #0
	movs r1, #2
	bl 0x02009b30
	movs r0, #1
	movs r1, #2
	bl 0x02009b30
	b .L_020009a2_2
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00013333
	.4byte 0x00002666
	.4byte 0x000014d3
	.4byte 0x00008009
	.4byte 0x00000101
	.4byte 0x00008001
	.4byte 0x00008002
	.4byte 0x00000103
	.4byte 0x000014dd
	.4byte 0x000014df
	.4byte 0x00406218
.L_020009a2_2:
	movs r0, #3
	movs r1, #2
	bl 0x02009b30
	movs r0, #2
	movs r1, #2
	bl 0x02009b38
	movs r0, #1
	adds r1, r6, #0
	movs r2, #0
	bl 0x02009b70
	adds r1, r6, #0
	movs r2, #20
	movs r0, #2
	bl 0x02009b70
	movs r0, #20
	bl 0x02009ad8
	ldr r2, [pc, #368]
	movs r3, #0
	str r3, [r2]
	ldr r2, [pc, #364]
	movs r3, #168
	lsls r3, r3, #16
	str r3, [r2]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r2, #4]
	ldr r5, [pc, #356]
	movs r3, #208
	lsls r3, r3, #14
	movs r1, #200
	str r3, [r2, #8]
	lsls r1, r1, #4
	adds r0, r5, #0
	bl 0x02009a28
	movs r0, #220
	bl 0x02009ad8
	adds r0, r5, #0
	bl 0x02009a30
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl 0x02009bc8
	movs r0, #20
	bl 0x02009bd8
	movs r0, #40
	bl 0x02009a20
	movs r0, #4
	bl 0x02009424
	movs r0, #20
	bl 0x02009ad8
	movs r2, #10
	movs r1, #0
	ldr r0, [pc, #292]
	bl 0x02009b68
	movs r0, #0
	bl 0x02009424
	ldr r0, [pc, #280]
	movs r1, #0
	bl 0x02009b60
	ldr r1, [pc, #276]
	movs r0, #8
	bl 0x02009b10
	movs r0, #40
	bl 0x02009ad8
	movs r1, #129
	movs r2, #60
	movs r0, #1
	lsls r1, r1, #1
	bl 0x02009b80
	ldr r0, [pc, #252]
	movs r1, #0
	bl 0x02009b60
	movs r1, #129
	movs r2, #10
	movs r0, #2
	lsls r1, r1, #1
	bl 0x02009b80
	ldr r0, [pc, #236]
	movs r1, #0
	bl 0x02009b60
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009b70
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #10
	bl 0x02009b70
	movs r1, #128
	movs r2, #10
	movs r0, #0
	lsls r1, r1, #7
	bl 0x02009b70
	movs r0, #1
	movs r1, #1
	bl 0x02009b38
	movs r2, #10
	ldr r0, [pc, #180]
	movs r1, #0
	bl 0x02009b68
	movs r0, #2
	movs r1, #1
	bl 0x02009b38
	ldr r0, [pc, #168]
	movs r1, #0
	movs r2, #10
	bl 0x02009b68
	cmp r7, #0
	beq .L_020009a2_3
	movs r0, #3
	movs r1, #1
	bl 0x02009b38
	ldr r0, [pc, #148]
	movs r1, #0
	movs r2, #10
	bl 0x02009b68
.L_020009a2_3:
	movs r0, #0
	movs r1, #3
	bl 0x02009b20
	movs r0, #1
	movs r1, #3
	bl 0x02009b20
	movs r0, #3
	movs r1, #3
	bl 0x02009b20
	movs r0, #2
	movs r1, #3
	bl 0x02009b28
	ldr r5, [pc, #112]
	movs r0, #1
	adds r1, r5, #0
	bl 0x02009b08
	cmp r7, #0
	beq .L_020009a2_4
	movs r0, #3
	adds r1, r5, #0
	bl 0x02009b08
.L_020009a2_4:
	adds r1, r5, #0
	movs r0, #2
	bl 0x02009b10
	movs r0, #20
	bl 0x02009ad8
	movs r0, #0
	bl 0x02009af8
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	ldr r0, [pc, #64]
	bl 0x02009ad0
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #56]
	bl 0x02009a28
	bl 0x02009ae8
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200adcc
	.4byte 0x0200adc0
	.4byte 0x0200962d
	.4byte 0x00008009
	.4byte 0x02009e54
	.4byte 0x00008001
	.4byte 0x00008002
	.4byte 0x00008003
	.4byte 0x02009ec8
	.4byte 0x00000844
	.4byte 0x02009245
	.global Func_02000f58
	.thumb_func
Func_02000f58:
	push {r5, r6, lr}
	sub sp, #8
	cmp r0, #0
	beq .L_02000f58_0
	ldr r0, [pc, #152]
	bl 0x02009ac8
	cmp r0, #0
	bne .L_02000f58_0
	bl 0x02008918
.L_02000f58_0:
	movs r0, #1
	bl 0x02009a20
	ldr r0, [pc, #136]
	bl 0x02009ac8
	cmp r0, #0
	beq .L_02000f58_1
	movs r3, #93
	str r3, [sp, #0]
	movs r6, #10
	movs r0, #121
	movs r1, #34
	movs r2, #3
	movs r3, #1
	str r6, [sp, #4]
	bl 0x02009a90
	movs r3, #43
	str r3, [sp, #4]
	movs r5, #30
	movs r0, #46
	movs r1, #38
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02009a90
	movs r3, #9
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #2
	str r5, [sp, #0]
	bl 0x02009a98
	movs r3, #8
	str r3, [sp, #4]
	movs r0, #26
	movs r1, #3
	movs r2, #1
	movs r3, #2
	str r6, [sp, #0]
	bl 0x02009a98
	movs r3, #40
	str r3, [sp, #4]
	movs r0, #26
	movs r1, #35
	movs r2, #1
	movs r3, #4
	str r6, [sp, #0]
	bl 0x02009a90
	b .L_02000f58_2
.L_02000f58_1:
	movs r3, #10
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #11
	movs r1, #8
	movs r2, #1
	movs r3, #2
	bl 0x02009a98
.L_02000f58_2:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000109
	.4byte 0x00000844
	.global Func_02001004
	.thumb_func
Func_02001004:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #8
	bl 0x02009af8
	ldr r2, [pc, #456]
	ldr r3, [pc, #460]
	mov r8, r0
	str r3, [r2]
	bl 0x02009224
	ldr r5, [pc, #452]
	mov r3, r8
	mov r1, r8
	adds r3, #85
	movs r6, #0
	strb r6, [r3]
	movs r0, #9
	str r5, [r1, #12]
	bl 0x02009af8
	adds r3, r0, #0
	adds r3, #85
	strb r6, [r3]
	movs r1, #15
	str r5, [r0, #12]
	movs r0, #9
	bl 0x02009b40
	movs r0, #0
	bl 0x02009424
	ldr r7, [pc, #416]
	movs r2, #225
	lsls r2, r2, #1
	adds r2, r2, r7
	movs r1, #0
	ldrsh r3, [r2, r1]
	mov r8, r2
	cmp r3, #19
	beq .L_02001004_0
	movs r1, #200
	ldr r0, [pc, #400]
	lsls r1, r1, #4
	bl 0x02009a28
.L_02001004_0:
	ldr r0, [pc, #396]
	bl 0x02009ac8
	cmp r0, #0
	beq .L_02001004_1
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x02009b18
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x02009b18
.L_02001004_1:
	ldr r0, [pc, #368]
	bl 0x02009ac8
	cmp r0, #0
	beq .L_02001004_2
	bl 0x020097ec
.L_02001004_2:
	ldr r6, [pc, #360]
	ldr r2, [r6]
	mov lr, r2
	mov r3, lr
	adds r3, #236
	ldr r0, [r3]
	movs r5, #130
	movs r3, #160
	lsls r3, r3, #16
	lsls r5, r5, #1
	add r5, lr
	ldr r4, [pc, #340]
	adds r0, r0, r3
	ldr r1, [pc, #340]
	movs r0, r0
	mov r12, pc
	bx r4
	.2byte 0x68ab
	.2byte 0x181b
	.2byte 0x60ab
	.2byte 0x4673
	.2byte 0x33f0
	.2byte 0x6818
	.2byte 0x2188
	.2byte 0x0409
	.2byte 0x1840
	.2byte 0x494f
	.2byte 0x46fc
	.2byte 0x4720
	.2byte 0x68eb
	.2byte 0x181b
	.2byte 0x60eb
	.2byte 0x4b4d
	.2byte 0x484d
	.2byte 0x612b
	.2byte 0x616b
	.2byte 0xf000
	.2byte 0xfcfb
	.2byte 0x484c
	.2byte 0xf000
	.2byte 0xfcf8
	.2byte 0x484b
	.2byte 0xf000
	.2byte 0xfcf5
	.2byte 0x484b
	.2byte 0xf000
	.2byte 0xfcf2
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xfc97
	.2byte 0x2000
	.2byte 0xf7ff
	.2byte 0xff30
	.2byte 0x6cf3
	.2byte 0x22e0
	.2byte 0x0052
	.2byte 0x189b
	.2byte 0x3242
	.2byte 0x601a
	.2byte 0x4641
	.2byte 0x3a0e
	.2byte 0x2300
	.2byte 0x5ece
	.2byte 0x18bb
	.2byte 0x681b
	.2byte 0x4698
	.2byte 0x4640
	.2byte 0xf000
	.2byte 0xfcf0
	.2byte 0x1c07
	.2byte 0x2e32
	.2byte 0xd005
	.2byte 0x2e28
	.2byte 0xd003
	.2byte 0x2e1e
	.2byte 0xd001
	.2byte 0x2e14
	.2byte 0xd142
	.2byte 0xf000
	.2byte 0xfd59
	.2byte 0x211b
	.2byte 0x4640
	.2byte 0xf000
	.2byte 0xfcf5
	.2byte 0x4640
	.2byte 0xf000
	.2byte 0xfcde
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xfcaf
	.2byte 0x4640
	.2byte 0x4934
	.2byte 0xf000
	.2byte 0xfd1f
	.2byte 0x2001
	.2byte 0x2101
	.2byte 0x2201
	.2byte 0x4252
	.2byte 0x4249
	.2byte 0x4240
	.2byte 0x2300
	.2byte 0xf000
	.2byte 0xfd1e
	.2byte 0x1c3b
	.2byte 0x3355
	.2byte 0x2502
	.2byte 0x701d
	.2byte 0x23c8
	.2byte 0x03db
	.2byte 0x60fb
	.2byte 0x4b2c
	.2byte 0x617b
	.2byte 0x2380
	.2byte 0x021b
	.2byte 0x64bb
	.2byte 0x20cc
	.2byte 0xf000
	.2byte 0xfd3f
	.2byte 0x1c31
	.2byte 0x390a
	.2byte 0x4828
	.2byte 0xf000
	.2byte 0xfd12
	.2byte 0x2014
	.2byte 0xf000
	.2byte 0xfca7
	.2byte 0x1c3b
	.2byte 0x3322
	.2byte 0x701d
	.2byte 0x2103
	.2byte 0x4640
	.2byte 0xf000
	.2byte 0xfcf0
	.2byte 0x2002
	.2byte 0xf000
	.2byte 0xfc9d
	.2byte 0x2180
	.2byte 0x4640
	.2byte 0x0049
	.2byte 0xf000
	.2byte 0xfcf0
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfc95
	.2byte 0xe00e
	.2byte 0x2e0a
	.2byte 0xd107
	.2byte 0x480f
	.2byte 0xf000
	.2byte 0xfc87
	.2byte 0x2800
	.2byte 0xd107
	.2byte 0xf000
	.2byte 0xfb7b
	.2byte 0xe004
	.2byte 0x2e13
	.2byte 0xd102
	.2byte 0x2001
	.2byte 0xf7ff
	.2byte 0xfec5
	.2byte 0x2000
	.2byte 0xbc08
	.2byte 0x4698
	.2byte 0xbce0
	.2byte 0xbc02
	.2byte 0x4708
	.2byte 0x0000
	.4byte 0x0200add0
	.4byte 0x02001000
	.4byte 0xfff60000
	.4byte 0x02000240
	.4byte 0x02009245
	.4byte 0x00000844
	.4byte 0x00000109
	.4byte 0x03001e70
	.4byte 0x03000118
	.4byte 0x00001999
	.2byte 0xe666
	.2byte 0x0000
	.2byte 0x0201
	.2byte 0x0000
	.2byte 0x020d
	.2byte 0x0000
	.2byte 0x020f
	.2byte 0x0000
	.2byte 0x0213
	.2byte 0x0000
	.2byte 0x0101
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xff60
	.2byte 0x002d
	.2byte 0x0000
	.global Func_02001224
	.thumb_func
Func_02001224:
	ldr r2, [pc, #16]
	movs r0, #160
	ldr r1, [r2]
	ldr r3, [pc, #16]
	lsls r0, r0, #19
	ldr r2, [pc, #16]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bx lr
	.2byte 0x0000
	.4byte 0x03001ed0
	.4byte 0x040000d4
	.4byte 0x84000070
	.global Func_02001244
	.thumb_func
Func_02001244:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, [pc, #448]
	movs r1, #191
	ldr r2, [r3]
	ldr r3, [r3, #20]
	lsls r1, r1, #1
	mov r8, r3
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	sub sp, #4
	cmp r3, #0
	beq .L_02001244_0
	b .L_02001244_1
.L_02001244_0:
	ldr r3, [pc, #424]
	ldr r3, [r3]
	movs r2, #31
	ands r3, r2
	cmp r3, #0
	beq .L_02001244_2
	b .L_02001244_1
.L_02001244_2:
	ldr r1, [pc, #416]
	movs r3, #32
	movs r2, #0
	add r8, r3
	mov r9, r1
	mov r10, r2
.L_02001244_19:
	ldr r3, [pc, #408]
	ldr r3, [r3]
	ldr r1, [pc, #408]
	lsls r3, r3, #2
	adds r2, r3, #4
	ldr r4, [r1, r3]
	adds r3, #8
	ldr r6, [r1, r3]
	mov r3, r10
	ldr r7, [r1, r2]
	cmp r3, #47
	bls .L_02001244_3
	lsrs r5, r4, #31
	adds r0, r4, #0
	movs r1, #3
	str r4, [sp, #0]
	adds r5, r4, r5
	bl 0x02009a10
	asrs r5, r5, #1
	ldr r4, [sp, #0]
	adds r5, r5, r0
	subs r4, r4, r5
	adds r0, r7, #0
	movs r1, #3
	str r4, [sp, #0]
	bl 0x02009a10
	lsrs r5, r7, #31
	adds r5, r7, r5
	asrs r5, r5, #1
	adds r5, r5, r0
	movs r1, #3
	adds r0, r6, #0
	bl 0x02009a10
	subs r7, r7, r5
	lsrs r5, r6, #31
	adds r5, r6, r5
	asrs r5, r5, #1
	adds r5, r5, r0
	subs r6, r6, r5
	ldr r4, [sp, #0]
	b .L_02001244_4
.L_02001244_3:
	mov r1, r10
	cmp r1, #31
	bls .L_02001244_5
	adds r0, r4, #0
	movs r1, #3
	str r4, [sp, #0]
	bl 0x02009a10
	ldr r4, [sp, #0]
	adds r3, r4, #0
	cmp r4, #0
	bge .L_02001244_6
	adds r3, r4, #3
.L_02001244_6:
	asrs r3, r3, #2
	adds r3, r0, r3
	subs r4, r4, r3
	adds r0, r7, #0
	movs r1, #3
	str r4, [sp, #0]
	bl 0x02009a10
	adds r3, r7, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bge .L_02001244_7
	adds r3, r7, #3
.L_02001244_7:
	asrs r3, r3, #2
	adds r3, r0, r3
	movs r1, #3
	adds r0, r6, #0
	subs r7, r7, r3
	str r4, [sp, #0]
	bl 0x02009a10
	adds r3, r6, #0
	ldr r4, [sp, #0]
	cmp r6, #0
	bge .L_02001244_8
	adds r3, r6, #3
.L_02001244_8:
	asrs r3, r3, #2
	adds r3, r0, r3
	subs r6, r6, r3
	b .L_02001244_4
.L_02001244_5:
	mov r2, r10
	cmp r2, #15
	bls .L_02001244_4
	adds r5, r4, #0
	cmp r4, #0
	bge .L_02001244_9
	adds r5, r4, #3
.L_02001244_9:
	adds r0, r4, #0
	movs r1, #5
	str r4, [sp, #0]
	bl 0x02009a10
	asrs r5, r5, #2
	ldr r4, [sp, #0]
	adds r5, r5, r0
	subs r4, r4, r5
	adds r5, r7, #0
	cmp r7, #0
	bge .L_02001244_10
	adds r5, r7, #3
.L_02001244_10:
	adds r0, r7, #0
	movs r1, #5
	str r4, [sp, #0]
	bl 0x02009a10
	asrs r5, r5, #2
	adds r5, r5, r0
	subs r7, r7, r5
	ldr r4, [sp, #0]
	adds r5, r6, #0
	cmp r6, #0
	bge .L_02001244_11
	adds r5, r6, #3
.L_02001244_11:
	adds r0, r6, #0
	movs r1, #5
	str r4, [sp, #0]
	bl 0x02009a10
	asrs r5, r5, #2
	adds r5, r5, r0
	ldr r4, [sp, #0]
	subs r6, r6, r5
.L_02001244_4:
	mov r1, r8
	ldrh r3, [r1]
	movs r2, #31
	adds r0, r3, #0
	lsrs r1, r3, #5
	ands r0, r2
	lsrs r3, r3, #10
	ands r1, r2
	ands r3, r2
	adds r0, r0, r4
	adds r1, r1, r7
	adds r3, r3, r6
	cmp r0, #31
	ble .L_02001244_12
	movs r0, #31
.L_02001244_12:
	cmp r1, #31
	ble .L_02001244_13
	movs r1, #31
.L_02001244_13:
	cmp r3, #31
	ble .L_02001244_14
	movs r3, #31
.L_02001244_14:
	cmp r0, #0
	bge .L_02001244_15
	movs r0, #0
.L_02001244_15:
	cmp r1, #0
	bge .L_02001244_16
	movs r1, #0
.L_02001244_16:
	cmp r3, #0
	bge .L_02001244_17
	movs r3, #0
.L_02001244_17:
	lsls r2, r1, #5
	lsls r3, r3, #10
	orrs r3, r2
	movs r1, #1
	mov r2, r9
	orrs r3, r0
	add r10, r1
	strh r3, [r2]
	movs r3, #2
	mov r2, r10
	add r9, r3
	add r8, r3
	cmp r2, #62
	bhi .L_02001244_18
	b .L_02001244_19
.L_02001244_18:
	ldr r5, [pc, #60]
	bl 0x02009a38
	movs r3, #7
	ands r0, r3
	lsls r2, r0, #1
	ldr r3, [r5]
	adds r2, r2, r0
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [pc, #44]
	lsls r3, r3, #2
	ldr r3, [r2, r3]
	cmp r3, #99
	bne .L_02001244_1
	movs r3, #0
	str r3, [r5]
.L_02001244_1:
	sub sp, #-4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x03001e40
	.4byte 0x05000020
	.4byte 0x0200adb8
	.4byte 0x02009f00
	.global Func_02001424
	.thumb_func
Func_02001424:
	push {lr}
	cmp r0, #12
	bls .L_02001424_0
	b .L_02001424_1
.L_02001424_0:
	ldr r2, [pc, #376]
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
	str r4, [sp, #416]
	lsls r0, r0, #8
	str r4, [sp, #928]
	lsls r0, r0, #8
	str r4, [sp, #512]
	lsls r0, r0, #8
	str r4, [sp, #608]
	lsls r0, r0, #8
	str r4, [sp, #704]
	lsls r0, r0, #8
	str r4, [sp, #968]
	lsls r0, r0, #8
	str r5, [sp, #624]
	lsls r0, r0, #8
	str r5, [sp, #40]
	lsls r0, r0, #8
	str r5, [sp, #552]
	lsls r0, r0, #8
	str r5, [sp, #136]
	lsls r0, r0, #8
	str r5, [sp, #232]
	lsls r0, r0, #8
	str r5, [sp, #328]
	lsls r0, r0, #8
	str r5, [sp, #592]
	lsls r0, r0, #8
	movs r1, #1
	movs r0, #8
	bl 0x02009b20
	movs r0, #6
	bl 0x02009a20
	movs r0, #8
	movs r1, #3
	bl 0x02009b20
	b .L_02001424_1
	.2byte 0x2101
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfb4c
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfac9
	.2byte 0x2008
	.2byte 0x2105
	.2byte 0xf000
	.2byte 0xfb45
	.2byte 0xe081
	.2byte 0x2101
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfb40
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfabd
	.2byte 0x2008
	.2byte 0x2104
	.2byte 0xf000
	.2byte 0xfb39
	.2byte 0xe075
	.2byte 0x2101
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfb34
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfab1
	.2byte 0x2103
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfb2d
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfaaa
	.2byte 0x2101
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfb26
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfaa3
	.2byte 0x2103
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfb1f
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa9c
	.2byte 0x2008
	.2byte 0x2101
	.2byte 0xf000
	.2byte 0xfb18
	.2byte 0xe054
	.2byte 0x2101
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfb13
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa90
	.2byte 0x2008
	.2byte 0x2102
	.2byte 0xf000
	.2byte 0xfb0c
	.2byte 0xe048
	.2byte 0x2106
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfb07
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa84
	.2byte 0x2008
	.2byte 0x2108
	.2byte 0xf000
	.2byte 0xfb00
	.2byte 0xe03c
	.2byte 0x2106
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfafb
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa78
	.2byte 0x2008
	.2byte 0x2109
	.2byte 0xf000
	.2byte 0xfaf4
	.2byte 0xe030
	.2byte 0x2106
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfaef
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa6c
	.2byte 0x2008
	.2byte 0x210a
	.2byte 0xf000
	.2byte 0xfae8
	.2byte 0xe024
	.2byte 0x2106
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfae3
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa60
	.2byte 0x2108
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfadc
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa59
	.2byte 0x2106
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xfad5
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa52
	.2byte 0x2108
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xface
	.2byte 0x2006
	.2byte 0xf000
	.2byte 0xfa4b
	.2byte 0x2008
	.2byte 0x2106
	.2byte 0xf000
	.2byte 0xfac7
	.2byte 0xe003
	.2byte 0x2008
	.2byte 0x2106
	.2byte 0xf000
	.2byte 0xfac2
.L_02001424_1:
	movs r0, #12
	bl 0x02009a20
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02009434
	.global Func_020015ac
	.thumb_func
Func_020015ac:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	adds r7, r6, #0
	adds r7, #100
	movs r1, #0
	ldrsh r2, [r7, r1]
	sub sp, #12
	cmp r2, #79
	bgt .L_020015ac_0
	ldr r3, [r6, #56]
	mov r5, sp
	str r3, [r5]
	ldr r3, [r6, #60]
	str r3, [r5, #4]
	ldr r3, [r6, #64]
	str r3, [r5, #8]
	adds r3, r6, #0
	adds r3, #102
	movs r1, #0
	ldrsh r3, [r3, r1]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #8
	adds r1, r1, r3
	lsls r0, r2, #16
	adds r2, r5, #0
	bl 0x02009a50
	ldr r3, [r5]
	str r3, [r6, #8]
	ldr r3, [r5, #4]
	str r3, [r6, #12]
	ldr r3, [r5, #8]
	str r3, [r6, #16]
	movs r1, #0
	ldrsh r3, [r7, r1]
	ldrh r2, [r7]
	cmp r3, #39
	bgt .L_020015ac_1
	ldr r2, [pc, #44]
	ldr r3, [r6, #24]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	ldrh r2, [r7]
.L_020015ac_1:
	adds r3, r2, #1
	strh r3, [r7]
	b .L_020015ac_2
.L_020015ac_0:
	ldr r3, [r6, #80]
	ldrb r0, [r3, #28]
	bl 0x02009a58
	adds r0, r6, #0
	bl 0x02009a78
.L_020015ac_2:
	sub sp, #-12
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xfffffae2
	.global Func_0200162c
	.thumb_func
Func_0200162c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r7, [pc, #404]
	ldr r5, [r7]
	movs r0, #0
	mov r11, r0
	movs r1, #10
	adds r0, r5, #0
	bl 0x02009a10
	adds r6, r0, #0
	cmp r5, #44
	bls .L_0200162c_0
	b .L_0200162c_1
.L_0200162c_0:
	ldr r2, [pc, #384]
	lsls r3, r5, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	str r7, [sp, #64]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #64]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #64]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #64]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #64]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #712]
	lsls r0, r0, #8
	str r7, [sp, #680]
	lsls r0, r0, #8
	movs r0, #220
	bl 0x02009bf8
	movs r3, #6
	movs r7, #0
	subs r1, r3, r6
	cmp r7, r1
	bcs .L_0200162c_2
	movs r3, #180
	movs r2, #0
	lsls r3, r3, #1
	ldr r6, [pc, #176]
	mov r9, r2
	mov r8, r1
	mov r10, r3
.L_0200162c_4:
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	ldr r0, [pc, #164]
	bl 0x02009a70
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200162c_3
	mov r1, r11
	ldr r0, [r5, #80]
	bl 0x02009bf0
	adds r3, r5, #0
	adds r3, #85
	mov r11, r0
	mov r0, r9
	strb r0, [r3]
	ldr r1, [r5, #80]
	movs r0, #13
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	strb r3, [r1, #9]
	adds r0, r5, #0
	movs r1, #0
	bl 0x02009aa0
	adds r0, r5, #0
	movs r1, #1
	bl 0x02009a60
	adds r3, r5, #0
	adds r3, #100
	mov r2, r9
	strh r2, [r3]
	mov r1, r8
	mov r0, r10
	bl 0x02009a18
	muls r0, r7
	mov r1, r10
	lsls r0, r0, #16
	bl 0x02009a18
	adds r3, r5, #0
	adds r3, #102
	strh r0, [r3]
	ldr r3, [r6]
	str r3, [r5, #56]
	ldr r3, [r6, #4]
	str r3, [r5, #60]
	ldr r3, [r6, #8]
	str r3, [r5, #64]
	ldr r3, [pc, #64]
	str r3, [r5, #48]
	ldr r3, [pc, #64]
	str r3, [r5, #108]
.L_0200162c_3:
	adds r7, #1
	cmp r7, r8
	bcc .L_0200162c_4
.L_0200162c_2:
	ldr r0, [pc, #60]
	bl 0x02009bf8
	ldr r7, [pc, #28]
.L_0200162c_1:
	ldr r3, [r7]
	adds r3, #1
	str r3, [r7]
	cmp r3, #120
	ble .L_0200162c_5
	movs r3, #0
	str r3, [r7]
.L_0200162c_5:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200adcc
	.4byte 0x0200965c
	.4byte 0x0200adc0
	.4byte 0x0000011d
	.4byte 0x00019999
	.4byte 0x020095ad
	.4byte 0x00000121
	.global Func_020017ec
	.thumb_func
Func_020017ec:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r6, [pc, #84]
	movs r2, #0
	ldrsh r3, [r6, r2]
	movs r2, #1
	negs r2, r2
	sub sp, #8
	cmp r3, r2
	beq .L_020017ec_0
	mov r8, r2
	movs r5, #0
	movs r7, #1
.L_020017ec_2:
	ldrsh r0, [r6, r5]
	bl 0x02009ac8
	cmp r0, #0
	beq .L_020017ec_1
	adds r3, r5, #2
	ldrsh r3, [r6, r3]
	cmp r3, #0
	beq .L_020017ec_1
	adds r3, r5, #4
	ldrsh r0, [r6, r3]
	adds r3, r5, #6
	ldrsh r1, [r6, r3]
	adds r3, #2
	ldrsh r2, [r6, r3]
	adds r3, #2
	ldrsh r3, [r6, r3]
	str r7, [sp, #0]
	str r7, [sp, #4]
	bl 0x02009a88
.L_020017ec_1:
	ldr r6, [pc, #20]
	adds r5, #12
	ldrsh r3, [r6, r5]
	cmp r3, r8
	bne .L_020017ec_2
.L_020017ec_0:
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02009ca8
	.global Func_0200184c
	.thumb_func
Func_0200184c:
	push {lr}
	ldr r0, [pc, #8]
	bl 0x02009a30
	pop {r0}
	bx r0
	.4byte 0x02009245
	.global Func_0200185c
	.thumb_func
Func_0200185c:
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
	bl 0x02009a10
	subs r5, r5, r0
	str r5, [r6, #68]
.L_02001884:
	adds r3, r7, #0
	cmp r7, #0
	bge .L_02001884_0
	adds r3, #15
.L_02001884_0:
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
	.global Func_020018b8
	.thumb_func
Func_020018b8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #68
	bl 0x02009ae0
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl 0x02009b98
	bl 0x02009a80
	movs r0, #1
	bl 0x02009a20
	movs r0, #0
	bl 0x02009af8
	movs r3, #130
	lsls r3, r3, #16
	str r3, [r0, #12]
	movs r0, #0
	bl 0x02009af8
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #0
	bl 0x02009af8
	movs r5, #0
	str r5, [r0, #68]
	movs r0, #0
	bl 0x02009af8
	adds r0, #85
	strb r5, [r0]
	bl 0x02009be0
	bl 0x02009be8
	movs r0, #204
	bl 0x02009bf8
	movs r0, #0
	bl 0x02009af8
	movs r3, #3
	adds r0, #85
	strb r3, [r0]
	movs r0, #24
	bl 0x02009ad8
	movs r0, #0
	bl 0x02009af8
	mov r10, r0
	add r0, sp, #28
	movs r3, #7
	str r3, [r0, #4]
	ldr r3, [pc, #188]
	str r3, [r0, #36]
	ldr r3, [pc, #188]
	str r3, [r0, #8]
	str r3, [r0, #12]
	mov r8, r0
	movs r7, #0
	add r6, sp, #16
.L_020018b8_0:
	lsls r5, r7, #12
	adds r0, r5, #0
	bl 0x02009a48
	movs r3, #0
	str r0, [r6]
	adds r0, r5, #0
	str r3, [r6, #4]
	bl 0x02009a40
	ldr r3, [r6]
	lsrs r2, r3, #31
	adds r2, r3, r2
	asrs r2, r2, #1
	adds r3, r3, r2
	str r0, [r6, #8]
	str r3, [r6]
	mov r2, r10
	ldr r5, [r2, #8]
	ldr r1, [r2, #12]
	ldr r4, [r6, #4]
	ldr r2, [r2, #16]
	str r0, [sp, #4]
	ldr r0, [pc, #136]
	str r4, [sp, #0]
	str r0, [sp, #8]
	mov r4, r8
	adds r0, r5, #0
	adds r7, #1
	str r4, [sp, #12]
	bl 0x0200813c
	cmp r7, #16
	bls .L_020018b8_0
	movs r0, #188
	bl 0x02009bf8
	movs r0, #0
	ldr r1, [pc, #108]
	bl 0x02009b88
	movs r0, #0
	movs r1, #22
	bl 0x02009b20
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl 0x02009ab0
	movs r0, #1
	movs r1, #1
	ldr r2, [pc, #80]
	negs r0, r0
	negs r1, r1
	bl 0x02009ab0
	bl 0x02009ab8
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #0
	bl 0x02009b88
	movs r0, #0
	bl 0x02009af8
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #72]
	movs r0, #0
	bl 0x02009af8
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r0, #68]
	bl 0x02009ae8
	sub sp, #-68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200985d
	.4byte 0x0000cccc
	.4byte 0x01090001
	.4byte 0x00000101
	.4byte 0x0000e666
	.include "games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/IMPORT.INC"
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
	.4byte 0x00010200
	.4byte 0x00230040
	.4byte 0x00060071
	.4byte 0x00000201
	.4byte 0x00230041
	.4byte 0x00060073
	.4byte 0x00010202
	.4byte 0x00230042
	.4byte 0x0008006e
	.4byte 0x00010203
	.4byte 0x00230043
	.4byte 0x000a006e
	.4byte 0x00010204
	.4byte 0x00230044
	.4byte 0x000c006f
	.4byte 0x00010205
	.4byte 0x00230045
	.4byte 0x000c0071
	.4byte 0x00010206
	.4byte 0x00230046
	.4byte 0x000e0070
	.4byte 0x00010207
	.4byte 0x00230047
	.4byte 0x000e0072
	.4byte 0x00010208
	.4byte 0x00230048
	.4byte 0x000e0074
	.4byte 0x00010209
	.4byte 0x00230049
	.4byte 0x00170047
	.4byte 0x0001020a
	.4byte 0x0023004a
	.4byte 0x0018004d
	.4byte 0x0001020c
	.4byte 0x0023004b
	.4byte 0x00180070
	.4byte 0x0000020d
	.4byte 0x0023004c
	.4byte 0x00180072
	.4byte 0x0001020e
	.4byte 0x0023004d
	.4byte 0x00180074
	.4byte 0x0000020f
	.4byte 0x0023004e
	.4byte 0x001a0070
	.4byte 0x0001020b
	.4byte 0x0023004f
	.4byte 0x001a0072
	.4byte 0x00010210
	.4byte 0x00230050
	.4byte 0x001a0074
	.4byte 0x00010211
	.4byte 0x00230051
	.4byte 0x001c0070
	.4byte 0x00010212
	.4byte 0x00230052
	.4byte 0x001c0072
	.4byte 0x00000213
	.4byte 0x00230053
	.4byte 0x001c0074
	.4byte 0x00010214
	.4byte 0x00230054
	.4byte 0x001c0076
	.4byte 0x0000ffff
	.4byte 0x02009c00
	.4byte 0x02009c38
	.4byte 0x02009c70
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a60000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00940000
	.4byte 0x00000000
	.4byte 0x005a0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00b60000
	.4byte 0x00000000
	.4byte 0x005a0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a60000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffb20000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffff000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000c
	.4byte 0x00000008
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
	.4byte 0x00a60000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0x00000063
	.4byte 0xffff0000
	.4byte 0x00000088
	.4byte 0x40000088
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000001e8
	.4byte 0xc00000f8
	.4byte 0x01600000
	.4byte 0x02800000
	.4byte 0x00000110
	.4byte 0xffff0002
	.4byte 0x000002c8
	.4byte 0x00000098
	.4byte 0x02a00000
	.4byte 0x03b00000
	.4byte 0x00000110
	.4byte 0xffff0003
	.4byte 0x00000398
	.4byte 0x80000098
	.4byte 0x02a00000
	.4byte 0x03b00000
	.4byte 0x00000110
	.4byte 0xffff0004
	.4byte 0x000000a8
	.4byte 0xc0000208
	.4byte 0x00200000
	.4byte 0x01300110
	.4byte 0x00000220
	.4byte 0xffff0005
	.4byte 0x00000128
	.4byte 0x800001a8
	.4byte 0x00200000
	.4byte 0x01300110
	.4byte 0x00000220
	.4byte 0xffff0006
	.4byte 0x00000178
	.4byte 0x000001a8
	.4byte 0x01600000
	.4byte 0x02800110
	.4byte 0x00000220
	.4byte 0xffff0007
	.4byte 0x000001e8
	.4byte 0xc0000208
	.4byte 0x01600000
	.4byte 0x02800110
	.4byte 0x00000220
	.4byte 0xffff0008
	.4byte 0x000002c8
	.4byte 0x000001a8
	.4byte 0x02a00000
	.4byte 0x03c00110
	.4byte 0x00000220
	.4byte 0xffff0009
	.4byte 0x00000398
	.4byte 0x800001a8
	.4byte 0x02a00000
	.4byte 0x03c00110
	.4byte 0x00000220
	.4byte 0xffff000a
	.4byte 0x000000a8
	.4byte 0x40000098
	.4byte 0x00200000
	.4byte 0x01300000
	.4byte 0x00000110
	.4byte 0xffff000b
	.4byte 0x001801a8
	.4byte 0x40000048
	.4byte 0x01600000
	.4byte 0x02800000
	.4byte 0x00000110
	.4byte 0xffff000c
	.4byte 0x000002e8
	.4byte 0x40000058
	.4byte 0x02a00000
	.4byte 0x03b00000
	.4byte 0x00000110
	.4byte 0xffff000d
	.4byte 0x001800e8
	.4byte 0x40000158
	.4byte 0x00200000
	.4byte 0x01300110
	.4byte 0x00000220
	.4byte 0xffff000e
	.4byte 0x00000228
	.4byte 0x40000168
	.4byte 0x01600000
	.4byte 0x02800110
	.4byte 0x00000220
	.4byte 0xffff000f
	.4byte 0x001800a8
	.4byte 0x40000098
	.4byte 0x00200000
	.4byte 0x01300000
	.4byte 0x00000110
	.4byte 0xffff0010
	.4byte 0x000001e8
	.4byte 0x400000a8
	.4byte 0x01600000
	.4byte 0x02800000
	.4byte 0x00000110
	.4byte 0xffff0015
	.4byte 0x000001d8
	.4byte 0x40000068
	.4byte 0x01600000
	.4byte 0x02800000
	.4byte 0x00000110
	.4byte 0xffff0016
	.4byte 0x000001f8
	.4byte 0x40000068
	.4byte 0x01600000
	.4byte 0x02800000
	.4byte 0x00000110
	.4byte 0xffff0017
	.4byte 0x000001a8
	.4byte 0x40000088
	.4byte 0x01600000
	.4byte 0x02800000
	.4byte 0x00000110
	.4byte 0xffff0018
	.4byte 0x000001a8
	.4byte 0x400000a8
	.4byte 0x01600000
	.4byte 0x02800000
	.4byte 0x00000110
	.4byte 0xffff0019
	.4byte 0x000001b8
	.4byte 0x400000c8
	.4byte 0x01600000
	.4byte 0x02800000
	.4byte 0x00000110
	.4byte 0xffff001a
	.4byte 0x000001e8
	.4byte 0x400000c8
	.4byte 0x01600000
	.4byte 0x02800000
	.4byte 0x00000110
	.4byte 0xffff001b
	.4byte 0x000001c8
	.4byte 0x400000e8
	.4byte 0x01600000
	.4byte 0x02800000
	.4byte 0x00000110
	.4byte 0xffff001c
	.4byte 0x000001e8
	.4byte 0x400000e8
	.4byte 0x01600000
	.4byte 0x02800000
	.4byte 0x00000110
	.4byte 0xffff001d
	.4byte 0x00000208
	.4byte 0x400000e8
	.4byte 0x01600000
	.4byte 0x02800000
	.4byte 0x00000110
	.4byte 0xffff001f
	.4byte 0x000002f8
	.4byte 0x40000068
	.4byte 0x02a00000
	.4byte 0x03b00000
	.4byte 0x00000110
	.4byte 0xffff0020
	.4byte 0x00000358
	.4byte 0x40000078
	.4byte 0x02a00000
	.4byte 0x03b00000
	.4byte 0x00000110
	.4byte 0xffff0033
	.4byte 0x000001c8
	.4byte 0x40000188
	.4byte 0x01600000
	.4byte 0x02800110
	.4byte 0x00000220
	.4byte 0xffff0034
	.4byte 0x000001e8
	.4byte 0x40000188
	.4byte 0x01600000
	.4byte 0x02800110
	.4byte 0x00000220
	.4byte 0xffff0035
	.4byte 0x00000208
	.4byte 0x40000188
	.4byte 0x01600000
	.4byte 0x02800110
	.4byte 0x00000220
	.4byte 0xffff0036
	.4byte 0x000001c8
	.4byte 0x400001a8
	.4byte 0x01600000
	.4byte 0x02800110
	.4byte 0x00000220
	.4byte 0xffff0037
	.4byte 0x00000208
	.4byte 0x400001a8
	.4byte 0x01600000
	.4byte 0x02800110
	.4byte 0x00000220
	.4byte 0xffff0038
	.4byte 0x000001c8
	.4byte 0x400001a8
	.4byte 0x01600000
	.4byte 0x02800110
	.4byte 0x00000220
	.4byte 0xffff0039
	.4byte 0x000001e8
	.4byte 0x400001c8
	.4byte 0x01600000
	.4byte 0x02800110
	.4byte 0x00000220
	.4byte 0xffff003a
	.4byte 0x00000208
	.4byte 0x400001c8
	.4byte 0x01600000
	.4byte 0x02800110
	.4byte 0x00000220
	.4byte 0xffff003b
	.4byte 0x00000228
	.4byte 0x400001c8
	.4byte 0x01600000
	.4byte 0x02800110
	.4byte 0x00000220
	.4byte 0xffff0032
	.4byte 0x000001e8
	.4byte 0x400001a8
	.4byte 0x01600000
	.4byte 0x02800110
	.4byte 0x00000220
	.4byte 0xffff0028
	.4byte 0x000000a8
	.4byte 0x400001a8
	.4byte 0x00200000
	.4byte 0x01300110
	.4byte 0x00000220
	.4byte 0xffff001e
	.4byte 0x00000328
	.4byte 0x40000098
	.4byte 0x02a00000
	.4byte 0x03b00000
	.4byte 0x00000110
	.4byte 0xffff0014
	.4byte 0x000001e8
	.4byte 0x40000098
	.4byte 0x01600000
	.4byte 0x02800000
	.4byte 0x00000110
	.4byte 0xffff0013
	.4byte 0x000000a8
	.4byte 0x40000048
	.4byte 0x00200000
	.4byte 0x01300000
	.4byte 0x00000110
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x001c01a4
	.4byte 0x01ac0044
	.4byte 0x004c0024
	.4byte 0x000affff
	.4byte 0x001c00e4
	.4byte 0x00ec0152
	.4byte 0x015a0024
	.4byte 0x000cffff
	.4byte 0x001c00a3
	.4byte 0x00ab0094
	.4byte 0x009c0024
	.4byte 0x000effff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000002d
	.4byte 0x0010202c
	.4byte 0x0020102e
	.4byte 0x0030202e
	.4byte 0x0040302e
	.4byte 0x0050402e
	.4byte 0x0060602e
	.4byte 0x0070502e
	.4byte 0x0080702e
	.4byte 0x0090802e
	.4byte 0x00a0c02d
	.4byte 0x00b0b02d
	.4byte 0x00c0e02d
	.4byte 0x00d0d02d
	.4byte 0x00e1002d
	.4byte 0x00f0f02d
	.4byte 0x000001ff
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0037
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00300000
	.4byte 0x00024000
	.4byte 0xffff0047
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00300000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004401
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00008401
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000401
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00004401
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000401
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00008401
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00004401
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00008401
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000401
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000202
	.4byte 0xffff0015
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff0016
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff0017
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff0018
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff0019
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff001a
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff001b
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff001c
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff001d
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff001f
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff0020
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff0032
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff0033
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff0034
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff0035
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff0036
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff0037
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff0038
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff0039
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff003a
	.4byte 0x02008675
	.4byte 0x00000202
	.4byte 0xffff003b
	.4byte 0x02008675
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x02008495
	.4byte 0x00000002
	.4byte 0xffff0016
	.4byte 0x020084a9
	.4byte 0x00000002
	.4byte 0xffff0017
	.4byte 0x020084c1
	.4byte 0x00000002
	.4byte 0xffff0018
	.4byte 0x020084d9
	.4byte 0x00000002
	.4byte 0xffff0019
	.4byte 0x020084f1
	.4byte 0x00000002
	.4byte 0xffff001a
	.4byte 0x02008505
	.4byte 0x00000002
	.4byte 0xffff001b
	.4byte 0x0200851d
	.4byte 0x00000002
	.4byte 0xffff001c
	.4byte 0x02008535
	.4byte 0x00000002
	.4byte 0xffff001d
	.4byte 0x0200854d
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte 0x02008561
	.4byte 0x00000002
	.4byte 0xffff0020
	.4byte 0x02008579
	.4byte 0x00000002
	.4byte 0xffff0032
	.4byte 0x02008591
	.4byte 0x00000002
	.4byte 0xffff0033
	.4byte 0x020085a9
	.4byte 0x00000002
	.4byte 0xffff0034
	.4byte 0x020085bd
	.4byte 0x00000002
	.4byte 0xffff0035
	.4byte 0x020085d5
	.4byte 0x00000002
	.4byte 0xffff0036
	.4byte 0x020085ed
	.4byte 0x00000002
	.4byte 0xffff0037
	.4byte 0x02008605
	.4byte 0x00000002
	.4byte 0xffff0038
	.4byte 0x02008619
	.4byte 0x00000002
	.4byte 0xffff0039
	.4byte 0x02008631
	.4byte 0x00000002
	.4byte 0xffff003a
	.4byte 0x02008649
	.4byte 0x00000002
	.4byte 0xffff003b
	.4byte 0x02008661
	.4byte 0x00000002
	.4byte 0x08440028
	.4byte 0x0200869d
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x0200984d
	.4byte 0x00000013
	.4byte 0x0f5b0064
	.4byte 0x00100109
	.4byte 0x00000013
	.4byte 0x0f5c0065
	.4byte 0x001000b5
	.4byte 0x00000003
	.4byte 0x03500066
	.4byte 0x00300000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
