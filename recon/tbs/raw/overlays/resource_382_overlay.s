.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_MURA/ENTRY.INC"
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
	bl 0x02009938
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
	bl 0x02009958
	adds r0, r5, #0
	movs r1, #14
	bl 0x02009a48
	adds r0, r5, #0
	movs r1, #1
	bl 0x02009960
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
	bl 0x02009938
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
	bl 0x02009958
	adds r0, r5, #0
	movs r1, #15
	bl 0x02009a48
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
	bl 0x020099e0
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
	bl 0x02009938
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
	bl 0x02009928
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl 0x02009930
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
	bl 0x02009a48
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
	bl 0x02009900
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
	bl 0x02009900
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200013c_9:
	bl 0x02009900
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
	bl 0x02009928
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl 0x02009930
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
	.4byte 0x02009bb4
	.4byte 0x02008105
	.4byte 0xffff0000
	.global Func_02000314
	.thumb_func
Func_02000314:
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
	beq .L_02000314_0
	movs r0, #15
	b .L_02000314_1
.L_02000314_0:
	movs r0, #14
.L_02000314_1:
	bl 0x020099e0
	adds r1, r0, #0
	adds r0, r5, #0
	movs r2, #32
	movs r3, #0
	bl 0x02009674
	cmp r0, #0
	bne .L_02000314_2
	movs r0, #0
	bl 0x020099e0
	movs r2, #188
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r1, r0, #0
	cmp r3, #0
	bne .L_02000314_3
	ldr r3, [pc, #56]
	add r3, r8
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_02000314_4
.L_02000314_3:
	movs r3, #26
	ldrh r2, [r6]
	mov r10, r3
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_02000314_4
	movs r2, #1
	mov r9, r2
.L_02000314_4:
	adds r0, r5, #0
	mov r2, r10
	mov r3, r9
	bl 0x02009674
.L_02000314_2:
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
	.global Func_020003ac
	.thumb_func
Func_020003ac:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, [pc, #88]
	adds r7, r0, #0
	movs r2, #0
	ldr r6, [r3]
	ldr r5, [r3, #48]
	movs r3, #18
	mov r10, r2
	mov r8, r3
	movs r2, #128
	ldr r3, [r7, #56]
	lsls r2, r2, #24
	movs r0, #0
	cmp r3, r2
	beq .L_020003ac_0
	bl 0x020099e0
	movs r2, #188
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r1, r0, #0
	cmp r3, #0
	bne .L_020003ac_1
	ldr r2, [pc, #44]
	adds r3, r6, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_020003ac_2
.L_020003ac_1:
	movs r3, #26
	movs r2, #1
	mov r8, r3
	mov r10, r2
.L_020003ac_2:
	adds r0, r7, #0
	mov r2, r8
	mov r3, r10
	bl 0x02009674
	movs r0, #0
.L_020003ac_0:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001e8c
	.4byte 0x00000ea4
	.global Func_02000418
	.thumb_func
Func_02000418:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009f5c
	.global Func_02000420
	.thumb_func
Func_02000420:
	movs r0, #0
	bx lr
	.global Func_02000424
	.thumb_func
Func_02000424:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200a094
	.global Func_0200042c
	.thumb_func
Func_0200042c:
	push {lr}
	ldr r0, [pc, #20]
	bl 0x02009988
	cmp r0, #0
	beq .L_0200042c_0
	ldr r0, [pc, #12]
	b .L_0200042c_1
.L_0200042c_0:
	ldr r0, [pc, #12]
.L_0200042c_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00000855
	.4byte 0x0200a27c
	.4byte 0x0200a0cc
	.global Func_02000450
	.thumb_func
Func_02000450:
	push {lr}
	movs r0, #2
	bl 0x02009970
	movs r3, #140
	lsls r3, r3, #1
	adds r0, r0, r3
	ldrb r0, [r0]
	pop {r1}
	bx r1
	.global Func_02000464
	.thumb_func
Func_02000464:
	push {lr}
	bl 0x02009ac0
	movs r0, #2
	bl 0x02009970
	adds r0, #248
	ldr r3, [r0]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02000464_0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	movs r0, #2
	bl 0x020099a0
	movs r0, #126
	bl 0x02009ac8
	movs r0, #0
	bl 0x02009978
	movs r0, #2
	bl 0x02009978
.L_02000464_0:
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_020004a0
	.thumb_func
Func_020004a0:
	push {r5, lr}
	ldr r3, [pc, #256]
	ldr r0, [pc, #256]
	ldr r5, [r3]
	bl 0x02009988
	cmp r0, #0
	bne .L_020004a0_0
	ldr r0, [pc, #248]
	bl 0x02009988
	cmp r0, #0
	bne .L_020004a0_1
.L_020004a0_0:
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	subs r0, #19
	bl 0x02009aa0
	b .L_020004a0_2
.L_020004a0_1:
	bl 0x020099b8
	movs r0, #0
	bl 0x020099e0
	cmp r0, #0
	beq .L_020004a0_3
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x02009a18
.L_020004a0_3:
	ldr r2, [pc, #200]
	movs r0, #2
	ldr r1, [pc, #200]
	bl 0x020099e8
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #20
	bne .L_020004a0_4
	movs r1, #200
	movs r2, #224
	movs r0, #2
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x02009a08
	b .L_020004a0_5
.L_020004a0_4:
	ldr r0, [pc, #164]
	ldr r1, [pc, #168]
	bl 0x02009a88
	movs r0, #224
	movs r1, #1
	movs r2, #162
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl 0x02009a90
	movs r0, #2
	movs r1, #224
	movs r2, #162
	bl 0x02009a08
	bl 0x02009a98
.L_020004a0_5:
	movs r1, #2
	movs r2, #0
	movs r0, #0
	bl 0x02009a40
	movs r0, #20
	bl 0x020099b0
	ldr r0, [pc, #116]
	bl 0x02009a50
	ldr r0, [pc, #116]
	movs r1, #0
	movs r2, #20
	bl 0x02009a68
	movs r0, #0
	movs r1, #3
	bl 0x02009a28
	bl 0x02008450
	cmp r0, #0
	beq .L_020004a0_6
	ldr r0, [pc, #92]
	bl 0x02009a50
	movs r0, #2
	movs r1, #0
	bl 0x02009a60
	bl 0x02008464
	movs r0, #20
	bl 0x02009910
.L_020004a0_6:
	movs r0, #2
	bl 0x020099d0
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	subs r0, #19
	bl 0x02009aa0
	bl 0x02009ab0
	bl 0x02009ab8
	bl 0x020099c0
.L_020004a0_2:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x00000855
	.4byte 0x00000856
	.4byte 0x00006666
	.4byte 0x0000cccc
	.4byte 0x00001999
	.4byte 0x00001327
	.4byte 0x00009002
	.4byte 0x0000132a
	.global Func_020005c8
	.thumb_func
Func_020005c8:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #24]
	movs r1, #1
	bl 0x02009968
	ldr r0, [pc, #20]
	movs r1, #1
	bl 0x02009968
	bl 0x020099c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000947
	.4byte 0x000029dc
	.global Func_020005f0
	.thumb_func
Func_020005f0:
	push {lr}
	ldr r0, [pc, #20]
	bl 0x02009988
	cmp r0, #0
	beq .L_020005f0_0
	ldr r0, [pc, #12]
	b .L_020005f0_1
.L_020005f0_0:
	ldr r0, [pc, #12]
.L_020005f0_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00000855
	.4byte 0x0200a630
	.4byte 0x0200a414
	.global Func_02000614
	.thumb_func
Func_02000614:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #80]
	bl 0x02009a50
	movs r0, #9
	movs r1, #0
	movs r2, #2
	bl 0x0200973c
	movs r1, #0
	movs r0, #9
	bl 0x02009a58
	movs r0, #0
	movs r1, #0
	bl 0x020099d8
	cmp r0, #0
	beq .L_02000614_0
	ldr r3, [pc, #48]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000614_0:
	movs r0, #9
	movs r1, #0
	bl 0x02009a60
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009a70
	bl 0x020099c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001223
	.4byte 0x03001ebc
	.global Func_02000674
	.thumb_func
Func_02000674:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #76]
	bl 0x02009a50
	movs r0, #13
	movs r1, #1
	bl 0x02009a20
	movs r0, #13
	movs r1, #0
	movs r2, #2
	bl 0x0200973c
	movs r1, #0
	movs r0, #13
	bl 0x02009a58
	movs r0, #0
	movs r1, #0
	bl 0x020099d8
	cmp r0, #0
	beq .L_02000674_0
	ldr r3, [pc, #36]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000674_0:
	movs r0, #13
	movs r1, #0
	bl 0x02009a60
	bl 0x020099c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001229
	.4byte 0x03001ebc
	.global Func_020006d0
	.thumb_func
Func_020006d0:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #68]
	bl 0x02009a50
	movs r0, #17
	movs r1, #0
	movs r2, #2
	bl 0x0200973c
	movs r1, #0
	movs r0, #17
	bl 0x02009a58
	movs r0, #0
	movs r1, #0
	bl 0x020099d8
	cmp r0, #0
	beq .L_020006d0_0
	ldr r3, [pc, #36]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_020006d0_0:
	movs r0, #17
	movs r1, #0
	bl 0x02009a60
	bl 0x020099c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000122f
	.4byte 0x03001ebc
	.global Func_02000724
	.thumb_func
Func_02000724:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #68]
	bl 0x02009a50
	movs r0, #18
	movs r1, #0
	movs r2, #2
	bl 0x0200973c
	movs r1, #0
	movs r0, #18
	bl 0x02009a58
	movs r0, #0
	movs r1, #0
	bl 0x020099d8
	cmp r0, #0
	beq .L_02000724_0
	ldr r3, [pc, #36]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000724_0:
	movs r0, #18
	movs r1, #0
	bl 0x02009a60
	bl 0x020099c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001232
	.4byte 0x03001ebc
	.global Func_02000778
	.thumb_func
Func_02000778:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #20]
	bl 0x02009a50
	movs r0, #11
	bl 0x0200890c
	bl 0x020099c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001227
	.global Func_02000798
	.thumb_func
Func_02000798:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #20]
	bl 0x02009a50
	movs r0, #16
	bl 0x0200890c
	bl 0x020099c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000122e
	.global Func_020007b8
	.thumb_func
Func_020007b8:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #40]
	bl 0x02009a50
	movs r0, #19
	movs r1, #0
	bl 0x02009a20
	movs r2, #2
	movs r0, #19
	movs r1, #0
	bl 0x0200973c
	movs r0, #19
	movs r1, #0
	bl 0x02009a60
	bl 0x020099c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001235
	.global Func_020007ec
	.thumb_func
Func_020007ec:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #14
	bl 0x020099e0
	adds r6, r0, #0
	movs r3, #6
	ldrsh r2, [r6, r3]
	adds r5, r6, #0
	adds r5, #100
	mov r8, r2
	ldr r3, [pc, #56]
	ldrh r2, [r5]
	orrs r3, r2
	strh r3, [r5]
	bl 0x020099b8
	ldr r0, [pc, #48]
	bl 0x02009a50
	movs r0, #14
	movs r1, #0
	bl 0x02009a20
	movs r2, #2
	movs r0, #14
	movs r1, #0
	bl 0x0200973c
	movs r1, #10
	movs r0, #14
	bl 0x02009724
	mov r2, r8
	strh r2, [r6, #6]
	movs r0, #1
	bl 0x02009910
	bl 0x020099c0
	b .L_020007ec_0
	.4byte 0x00000002
	.4byte 0x0000122c
.L_020007ec_0:
	ldrh r2, [r5]
	movs r3, #1
	ands r3, r2
	strh r3, [r5]
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200085c
	.thumb_func
Func_0200085c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #15
	bl 0x020099e0
	adds r6, r0, #0
	movs r3, #6
	ldrsh r2, [r6, r3]
	adds r5, r6, #0
	adds r5, #100
	mov r8, r2
	ldr r3, [pc, #56]
	ldrh r2, [r5]
	orrs r3, r2
	strh r3, [r5]
	bl 0x020099b8
	ldr r0, [pc, #48]
	bl 0x02009a50
	movs r0, #15
	movs r1, #0
	bl 0x02009a20
	movs r2, #2
	movs r0, #15
	movs r1, #0
	bl 0x0200973c
	movs r1, #10
	movs r0, #15
	bl 0x02009724
	mov r2, r8
	strh r2, [r6, #6]
	movs r0, #1
	bl 0x02009910
	bl 0x020099c0
	b .L_0200085c_0
	.4byte 0x00000002
	.4byte 0x0000122d
.L_0200085c_0:
	ldrh r2, [r5]
	movs r3, #1
	ands r3, r2
	strh r3, [r5]
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020008cc
	.thumb_func
Func_020008cc:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #48]
	bl 0x02009a50
	movs r0, #21
	movs r1, #0
	movs r2, #2
	bl 0x0200973c
	movs r2, #0
	ldr r1, [pc, #32]
	movs r0, #21
	bl 0x02009a78
	movs r0, #30
	bl 0x020099b0
	movs r1, #0
	movs r0, #21
	bl 0x02009a58
	bl 0x020099c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000012c0
	.4byte 0x00000103
	.global Func_0200090c
	.thumb_func
Func_0200090c:
	push {r5, lr}
	adds r5, r0, #0
	bl 0x020099b8
	adds r0, r5, #0
	movs r1, #1
	bl 0x02009a20
	adds r0, r5, #0
	movs r2, #2
	movs r1, #0
	bl 0x0200973c
	adds r0, r5, #0
	movs r1, #0
	bl 0x02009a60
	bl 0x020099c0
	pop {r5}
	pop {r0}
	bx r0
	.global Func_02000938
	.thumb_func
Func_02000938:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r6, r0, #0
	bl 0x020099e0
	movs r3, #0
	adds r5, r0, #0
	adds r5, #91
	mov r8, r3
	movs r3, #1
	strb r3, [r5]
	bl 0x020099b8
	adds r0, r6, #0
	movs r1, #1
	bl 0x02009a20
	movs r0, #2
	bl 0x020099b0
	adds r0, r6, #0
	movs r1, #0
	bl 0x02009a60
	bl 0x020099c0
	mov r3, r8
	strb r3, [r5]
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_0200097c
	.thumb_func
Func_0200097c:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #68]
	bl 0x02009a50
	movs r0, #8
	movs r1, #0
	movs r2, #2
	bl 0x0200973c
	movs r1, #0
	movs r0, #8
	bl 0x02009a58
	movs r0, #0
	movs r1, #0
	bl 0x020099d8
	cmp r0, #0
	beq 0x020089b6
	ldr r3, [pc, #36]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
.L_020009ae:
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #8
	movs r1, #0
	bl 0x02009a60
	bl 0x020099c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x1330
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.global Func_020009d0
	.thumb_func
Func_020009d0:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #20]
.L_020009d8:
	bl 0x02009a50
	movs r0, #11
	bl 0x0200890c
	bl 0x020099c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x1335
	.2byte 0x0000
	.global Func_020009f0
	.thumb_func
Func_020009f0:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #44]
	bl 0x02009a50
	movs r0, #2
	bl 0x02009988
	cmp r0, #0
	beq .L_020009f0_0
	ldr r3, [pc, #32]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_020009f0_0:
	movs r0, #12
	bl 0x0200890c
	bl 0x020099c0
	pop {r0}
	bx r0
	.4byte 0x00001336
	.4byte 0x03001ebc
	.global Func_02000a2c
	.thumb_func
Func_02000a2c:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #20]
	bl 0x02009a50
	movs r0, #13
	bl 0x0200890c
	bl 0x020099c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001338
	.global Func_02000a4c
	.thumb_func
Func_02000a4c:
	push {r5, r6, r7, lr}
	movs r0, #14
	bl 0x020099e0
	adds r6, r0, #0
	adds r5, r6, #0
	adds r5, #100
	ldrh r2, [r5]
	movs r3, #6
	ldrsh r7, [r6, r3]
	ldr r3, [pc, #40]
	orrs r3, r2
	strh r3, [r5]
	bl 0x020099b8
	ldr r0, [pc, #36]
	bl 0x02009a50
	movs r0, #2
	bl 0x02009988
	cmp r0, #0
	beq .L_02000a4c_0
	ldr r3, [pc, #24]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02000a4c_0
	.4byte 0x00000002
	.4byte 0x00001339
	.4byte 0x03001ebc
.L_02000a4c_0:
	movs r0, #14
	movs r1, #0
	bl 0x02009a20
	movs r2, #2
	movs r0, #14
	movs r1, #0
	bl 0x0200973c
	movs r1, #10
	movs r0, #14
	bl 0x02009724
	movs r0, #1
	strh r7, [r6, #6]
	bl 0x02009910
	bl 0x020099c0
	ldrh r2, [r5]
	movs r3, #1
	ands r3, r2
	strh r3, [r5]
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02000acc
	.thumb_func
Func_02000acc:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #15
	bl 0x020099e0
	adds r6, r0, #0
	movs r3, #6
	ldrsh r2, [r6, r3]
	adds r5, r6, #0
	adds r5, #100
	mov r8, r2
	ldr r3, [pc, #56]
	ldrh r2, [r5]
	orrs r3, r2
	strh r3, [r5]
	bl 0x020099b8
	ldr r0, [pc, #48]
	bl 0x02009a50
	movs r0, #15
	movs r1, #0
	bl 0x02009a20
	movs r2, #2
	movs r0, #15
	movs r1, #0
	bl 0x0200973c
	movs r1, #10
	movs r0, #15
	bl 0x02009724
	mov r2, r8
	strh r2, [r6, #6]
	movs r0, #1
	bl 0x02009910
	bl 0x020099c0
	b .L_02000acc_0
	.4byte 0x00000002
	.4byte 0x0000133b
.L_02000acc_0:
	ldrh r2, [r5]
	movs r3, #1
	ands r3, r2
	strh r3, [r5]
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000b3c
	.thumb_func
Func_02000b3c:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #76]
	bl 0x02009a50
	movs r0, #16
	movs r1, #1
	bl 0x02009a20
	movs r0, #16
	movs r1, #0
	movs r2, #2
	bl 0x0200973c
	movs r1, #0
	movs r0, #16
	bl 0x02009a58
	movs r0, #0
	movs r1, #0
	bl 0x020099d8
	cmp r0, #0
	beq .L_02000b3c_0
	ldr r3, [pc, #36]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000b3c_0:
	movs r0, #16
	movs r1, #0
	bl 0x02009a60
	bl 0x020099c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000133c
	.4byte 0x03001ebc
	.global Func_02000b98
	.thumb_func
Func_02000b98:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #68]
	bl 0x02009a50
	movs r0, #18
	movs r1, #0
	movs r2, #2
	bl 0x0200973c
	movs r1, #0
	movs r0, #18
	bl 0x02009a58
	movs r0, #0
	movs r1, #0
	bl 0x020099d8
	cmp r0, #0
	beq .L_02000b98_0
	ldr r3, [pc, #36]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000b98_0:
	movs r0, #18
	movs r1, #0
	bl 0x02009a60
	bl 0x020099c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000133f
	.4byte 0x03001ebc
	.global Func_02000bec
	.thumb_func
Func_02000bec:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #84]
	bl 0x02009a50
	movs r0, #19
	movs r1, #0
	bl 0x02009a20
	movs r2, #2
	movs r0, #19
	movs r1, #0
	bl 0x0200973c
	movs r0, #19
	movs r1, #0
	bl 0x02009a60
	movs r1, #1
	movs r0, #19
	bl 0x02009a20
	movs r0, #231
	bl 0x02009980
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	beq .L_02000bec_0
	ldr r0, [pc, #32]
	bl 0x02009988
	cmp r0, #0
	bne .L_02000bec_0
	ldr r3, [pc, #28]
	movs r1, #185
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
.L_02000bec_0:
	bl 0x020099c0
	pop {r0}
	bx r0
	.4byte 0x00001342
	.4byte 0x00000858
	.4byte 0x03001ebc
	.global Func_02000c54
	.thumb_func
Func_02000c54:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #44]
	bl 0x02009a50
	movs r2, #2
	movs r0, #20
	movs r1, #0
	bl 0x0200973c
	movs r1, #3
	movs r0, #20
	bl 0x02009a28
	movs r0, #20
	bl 0x020099b0
	movs r0, #20
	movs r1, #0
	bl 0x02009a60
	bl 0x020099c0
	pop {r0}
	bx r0
	.4byte 0x0000137f
	.global Func_02000c8c
	.thumb_func
Func_02000c8c:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #36]
	bl 0x02009988
	cmp r0, #0
	bne .L_02000c8c_0
	ldr r0, [pc, #28]
	bl 0x02009a50
	b .L_02000c8c_1
.L_02000c8c_0:
	ldr r0, [pc, #24]
	bl 0x02009a50
.L_02000c8c_1:
	movs r0, #11
	bl 0x02008938
	bl 0x020099c0
	pop {r0}
	bx r0
	.4byte 0x00000855
	.4byte 0x00001239
	.4byte 0x00001346
	.global Func_02000cc4
	.thumb_func
Func_02000cc4:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #36]
	bl 0x02009988
	cmp r0, #0
	bne .L_02000cc4_0
	ldr r0, [pc, #28]
	bl 0x02009a50
	b .L_02000cc4_1
.L_02000cc4_0:
	ldr r0, [pc, #24]
	bl 0x02009a50
.L_02000cc4_1:
	movs r0, #13
	bl 0x02008938
	bl 0x020099c0
	pop {r0}
	bx r0
	.4byte 0x00000855
	.4byte 0x0000123b
	.4byte 0x00001348
	.global Func_02000cfc
	.thumb_func
Func_02000cfc:
	push {lr}
	movs r0, #14
	bl 0x020099e0
	adds r0, #100
	ldrh r2, [r0]
	ldr r3, [pc, #24]
	orrs r3, r2
	strh r3, [r0]
	bl 0x020099b8
	ldr r0, [pc, #20]
	bl 0x02009988
	cmp r0, #0
	bne .L_02000cfc_0
	ldr r0, [pc, #12]
	bl 0x02009a50
	b .L_02000cfc_1
	.4byte 0x00000002
	.4byte 0x00000855
	.4byte 0x0000123c
.L_02000cfc_0:
	ldr r0, [pc, #60]
	bl 0x02009a50
	movs r0, #2
	bl 0x02009988
	cmp r0, #0
	beq .L_02000cfc_1
	ldr r3, [pc, #48]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000cfc_1:
	movs r0, #14
	bl 0x02008938
	bl 0x020099c0
	movs r0, #14
	bl 0x020099e0
	adds r0, #100
	ldrh r2, [r0]
	movs r3, #1
	ands r3, r2
	strh r3, [r0]
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001349
	.4byte 0x03001ebc
	.global Func_02000d78
	.thumb_func
Func_02000d78:
	push {lr}
	movs r0, #15
	bl 0x020099e0
	adds r0, #100
	ldrh r2, [r0]
	ldr r3, [pc, #24]
	orrs r3, r2
	strh r3, [r0]
	bl 0x020099b8
	ldr r0, [pc, #20]
	bl 0x02009988
	cmp r0, #0
	bne .L_02000d78_0
	ldr r0, [pc, #12]
	bl 0x02009a50
	b .L_02000d78_1
	.4byte 0x00000002
	.4byte 0x00000855
	.4byte 0x0000123d
.L_02000d78_0:
	ldr r0, [pc, #32]
	bl 0x02009a50
.L_02000d78_1:
	movs r0, #15
	bl 0x02008938
	bl 0x020099c0
	movs r0, #15
	bl 0x020099e0
	adds r0, #100
	ldrh r2, [r0]
	movs r3, #1
	ands r3, r2
	strh r3, [r0]
	pop {r0}
	bx r0
	.4byte 0x0000134b
	.global Func_02000dd4
	.thumb_func
Func_02000dd4:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #36]
	bl 0x02009988
	cmp r0, #0
	bne .L_02000dd4_0
	ldr r0, [pc, #28]
	bl 0x02009a50
	b .L_02000dd4_1
.L_02000dd4_0:
	ldr r0, [pc, #24]
	bl 0x02009a50
.L_02000dd4_1:
	movs r0, #16
	bl 0x02008938
	bl 0x020099c0
	pop {r0}
	bx r0
	.4byte 0x00000855
	.4byte 0x0000123e
	.4byte 0x0000134c
	.global Func_02000e0c
	.thumb_func
Func_02000e0c:
	push {r5, lr}
	movs r0, #19
	bl 0x020099e0
	adds r5, r0, #0
	movs r3, #1
	adds r5, #91
	strb r3, [r5]
	bl 0x020099b8
	ldr r0, [pc, #76]
	bl 0x02009988
	cmp r0, #0
	bne .L_02000e0c_0
	ldr r0, [pc, #72]
	bl 0x02009a50
	movs r0, #19
	movs r1, #0
	bl 0x02009a20
	movs r0, #2
	bl 0x020099b0
	b .L_02000e0c_1
.L_02000e0c_0:
	ldr r0, [pc, #52]
	bl 0x02009988
	cmp r0, #0
	beq .L_02000e0c_2
	ldr r0, [pc, #48]
	bl 0x02009a50
	b .L_02000e0c_1
.L_02000e0c_2:
	ldr r0, [pc, #44]
	bl 0x02009a50
.L_02000e0c_1:
	movs r0, #19
	movs r1, #0
	bl 0x02009a60
	bl 0x020099c0
	movs r3, #0
	strb r3, [r5]
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000855
	.4byte 0x00001241
	.4byte 0x00000858
	.4byte 0x000013ab
	.4byte 0x0000134e
	.global Func_02000e84
	.thumb_func
Func_02000e84:
	push {lr}
	bl 0x020099b8
	ldr r0, [pc, #20]
	bl 0x02009a50
	movs r0, #21
	bl 0x02008938
	bl 0x020099c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000012c1
	.global Func_02000ea4
	.thumb_func
Func_02000ea4:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	sub sp, #8
	bl 0x02009990
	movs r3, #23
	movs r2, #26
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #55
	movs r1, #26
	movs r2, #4
	movs r3, #2
	bl 0x02009950
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000ecc
	.thumb_func
Func_02000ecc:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	sub sp, #8
	bl 0x02009998
	movs r3, #23
	movs r2, #26
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #23
	movs r1, #23
	movs r2, #4
	movs r3, #2
	bl 0x02009950
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000ef4
	.thumb_func
Func_02000ef4:
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
	bl 0x020099e8
	adds r2, r6, #0
	movs r0, #0
	adds r1, r5, #0
	bl 0x02009a00
	ldr r3, [pc, #28]
	movs r2, #228
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #16
	str r2, [r3]
	mov r0, r8
	bl 0x02009aa0
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.global Func_02000f3c
	.thumb_func
Func_02000f3c:
	push {lr}
	movs r0, #158
	bl 0x02009ac8
	ldr r0, [pc, #24]
	movs r1, #56
	movs r2, #19
	bl 0x02009940
	movs r0, #204
	movs r1, #160
	lsls r0, r0, #1
	lsls r1, r1, #1
	movs r2, #5
	bl 0x02008ef4
	pop {r0}
	bx r0
	.4byte 0x0200a828
	.global Func_02000f64
	.thumb_func
Func_02000f64:
	push {lr}
	movs r0, #158
	bl 0x02009ac8
	ldr r0, [pc, #24]
	movs r1, #50
	movs r2, #18
	bl 0x02009940
	movs r0, #156
	movs r1, #152
	lsls r0, r0, #1
	lsls r1, r1, #1
	movs r2, #6
	bl 0x02008ef4
	pop {r0}
	bx r0
	.4byte 0x0200a83e
	.global Func_02000f8c
	.thumb_func
Func_02000f8c:
	push {lr}
	movs r0, #158
	bl 0x02009ac8
	ldr r0, [pc, #24]
	movs r1, #44
	movs r2, #17
	bl 0x02009940
	movs r1, #144
	lsls r1, r1, #1
	movs r0, #216
	movs r2, #7
	bl 0x02008ef4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200a854
	.global Func_02000fb4
	.thumb_func
Func_02000fb4:
	push {r5, r6, lr}
	movs r0, #0
	sub sp, #8
	bl 0x020099e0
	adds r5, r0, #0
	movs r0, #158
	ldr r6, [r5, #80]
	bl 0x02009ac8
	ldr r0, [pc, #64]
	movs r1, #54
	movs r2, #13
	bl 0x02009940
	movs r3, #23
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r5, #35
	movs r0, #33
	movs r1, #20
	movs r2, #1
	movs r3, #3
	bl 0x02009950
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
	bl 0x02008ef4
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0200a86a
	.global Func_02001010
	.thumb_func
Func_02001010:
	push {r5, r6, lr}
	movs r0, #0
	sub sp, #8
	bl 0x020099e0
	adds r5, r0, #0
	movs r0, #158
	ldr r6, [r5, #80]
	bl 0x02009ac8
	ldr r0, [pc, #64]
	movs r1, #49
	movs r2, #10
	bl 0x02009940
	movs r3, #18
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r5, #35
	movs r0, #33
	movs r1, #20
	movs r2, #1
	movs r3, #3
	bl 0x02009950
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
	bl 0x02008ef4
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0200a880
	.global Func_0200106c
	.thumb_func
Func_0200106c:
	push {lr}
	movs r0, #158
	bl 0x02009ac8
	ldr r0, [pc, #20]
	movs r1, #38
	movs r2, #6
	bl 0x02009940
	movs r0, #120
	movs r1, #144
	movs r2, #10
	bl 0x02008ef4
	pop {r0}
	bx r0
	.4byte 0x0200a896
	.global Func_02001090
	.thumb_func
Func_02001090:
	push {r5, r6, lr}
	mov r6, r8
.L_02001094:
	push {r6}
	movs r0, #0
	sub sp, #8
	bl 0x020099e0
	adds r6, r0, #0
	ldr r2, [r6, #80]
	movs r0, #188
	mov r8, r2
	bl 0x02009ac8
	movs r5, #2
	movs r0, #42
	movs r1, #33
	movs r2, #34
	movs r3, #16
.L_020010b4:
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009948
	movs r1, #35
	movs r2, #36
	movs r3, #16
	movs r0, #42
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009948
	movs r0, #4
	bl 0x020099b0
	movs r0, #40
	movs r1, #33
	movs r2, #34
	movs r3, #16
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009948
	movs r1, #35
	movs r2, #36
	movs r3, #16
	movs r0, #40
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009948
	movs r0, #4
	bl 0x020099b0
	movs r3, #3
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r6, #35
	movs r0, #33
	movs r1, #21
	movs r2, #2
	movs r3, #2
	bl 0x02009950
	ldrb r2, [r6]
	movs r3, #254
	ands r3, r2
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
	bl 0x02008ef4
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200113c
	.thumb_func
Func_0200113c:
	push {r5, r6, lr}
	movs r0, #0
	sub sp, #8
	bl 0x020099e0
	adds r5, r0, #0
	movs r0, #158
	ldr r6, [r5, #80]
	bl 0x02009ac8
	ldr r0, [pc, #64]
	movs r1, #35
	movs r2, #9
	bl 0x02009940
	movs r3, #4
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r5, #35
	movs r0, #33
	movs r1, #20
	movs r2, #1
	movs r3, #3
	bl 0x02009950
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
	bl 0x02008ef4
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200a8ac
	.global Func_02001198
	.thumb_func
Func_02001198:
	push {lr}
	movs r0, #123
	bl 0x02009ac8
	movs r1, #132
	lsls r1, r1, #1
	movs r0, #152
	movs r2, #13
	bl 0x02008ef4
	pop {r0}
	bx r0
	.global Func_020011b0
	.thumb_func
Func_020011b0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	mov r8, r2
	adds r6, r1, #0
	mov r10, r3
	bl 0x020099e0
	movs r1, #192
	movs r2, #192
	adds r7, r0, #0
	lsls r1, r1, #10
	adds r0, r5, #0
	lsls r2, r2, #9
	bl 0x020099e8
	movs r3, #128
	lsls r3, r3, #8
	mov r2, r10
	str r3, [r7, #72]
	movs r3, #0
	str r3, [r7, #68]
	str r2, [r7, #40]
	adds r0, r7, #0
	movs r1, #0
	bl 0x02009958
	adds r0, r5, #0
	adds r1, r6, #0
	mov r2, r8
	bl 0x020099f8
	mov r3, r8
	lsls r3, r3, #16
	mov r8, r3
	lsls r6, r6, #16
	adds r0, r5, #0
	adds r1, r6, #0
	mov r2, r8
	bl 0x02009a18
	movs r5, #60
	b .L_020011b0_0
.L_020011b0_2:
	subs r5, #1
.L_020011b0_0:
	cmp r5, #0
	beq .L_020011b0_1
	movs r0, #1
	bl 0x02009910
	movs r2, #42
	ldrsh r3, [r7, r2]
	cmp r3, #0
	bne .L_020011b0_2
.L_020011b0_1:
	adds r0, r7, #0
	movs r1, #1
	bl 0x02009958
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02001238
	.thumb_func
Func_02001238:
	push {lr}
	bl 0x020099b8
	movs r0, #100
	bl 0x02009ac8
	movs r0, #40
	bl 0x020099b0
	ldr r0, [pc, #112]
	bl 0x02009988
	cmp r0, #0
	bne .L_02001238_0
	movs r1, #129
	movs r0, #23
	lsls r1, r1, #1
	bl 0x02009a80
	movs r1, #4
	movs r2, #0
	movs r0, #23
	bl 0x02009a30
	movs r0, #12
	bl 0x020099b0
	movs r1, #4
	movs r2, #0
	movs r0, #23
	bl 0x02009a30
	movs r0, #20
	bl 0x020099b0
	movs r1, #196
	movs r3, #224
	lsls r3, r3, #11
	lsls r1, r1, #1
	movs r2, #104
	movs r0, #23
	bl 0x020091b0
	movs r0, #20
	bl 0x020099b0
	movs r1, #204
	movs r0, #23
	lsls r1, r1, #1
	movs r2, #104
	bl 0x02009a08
	movs r1, #204
	movs r0, #23
	lsls r1, r1, #1
	movs r2, #120
	bl 0x02009a08
	ldr r0, [pc, #12]
	bl 0x02009990
.L_02001238_0:
	bl 0x020099c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000867
	.global Func_020012c0
	.thumb_func
Func_020012c0:
	push {lr}
	movs r0, #231
	bl 0x020099a8
	bl 0x020099b8
	movs r0, #10
	bl 0x020099b0
	movs r0, #19
	movs r1, #2
	bl 0x02009a38
	movs r0, #19
	ldr r1, [pc, #128]
	ldr r2, [pc, #132]
	bl 0x020099e8
	movs r2, #204
	movs r1, #216
	lsls r2, r2, #1
	movs r0, #19
.L_020012ec:
	bl 0x02009a08
	movs r0, #10
	bl 0x020099b0
	movs r1, #128
	movs r0, #19
	lsls r1, r1, #7
	movs r2, #20
	bl 0x02009a70
	movs r1, #6
	movs r2, #0
	movs r0, #19
	bl 0x02009a30
	movs r0, #30
	bl 0x020099b0
	movs r1, #6
	movs r2, #0
	movs r0, #19
	bl 0x02009a30
	movs r0, #30
	bl 0x020099b0
	movs r1, #6
	movs r2, #0
.L_02001326:
	movs r0, #19
	bl 0x02009a30
	movs r0, #30
	bl 0x020099b0
	movs r2, #196
	movs r1, #216
	lsls r2, r2, #1
	movs r0, #19
	bl 0x02009a08
	movs r0, #10
	bl 0x020099b0
	movs r1, #128
	lsls r1, r1, #7
.L_02001348:
	movs r2, #20
	movs r0, #19
	bl 0x02009a70
	ldr r0, [pc, #20]
	bl 0x02009990
	bl 0x020099c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.4byte 0x00000858
	.global Func_0200136c
	.thumb_func
Func_0200136c:
	push {r5, r6, r7, lr}
	ldr r0, [pc, #656]
	sub sp, #8
	bl 0x02009988
	cmp r0, #0
	beq .L_0200136c_0
	movs r0, #14
	bl 0x02009aa0
.L_0200136c_0:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x02009988
	cmp r0, #0
	beq .L_0200136c_1
	movs r3, #23
	movs r2, #26
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #55
	movs r1, #26
	movs r2, #4
	movs r3, #2
	bl 0x02009950
.L_0200136c_1:
	movs r0, #128
	movs r2, #210
	lsls r2, r2, #17
	movs r1, #0
	movs r3, #223
	lsls r0, r0, #16
	bl 0x020080a0
	movs r0, #14
	bl 0x020099e0
	ldr r5, [pc, #588]
	adds r3, r0, #0
	adds r3, #100
	movs r7, #1
	strh r7, [r3]
	str r5, [r0, #108]
	movs r0, #15
	bl 0x020099e0
	adds r3, r0, #0
	movs r6, #0
	adds r3, #100
	strh r6, [r3]
	str r5, [r0, #108]
	ldr r0, [pc, #564]
	bl 0x02009988
	cmp r0, #0
	beq .L_0200136c_3
	movs r1, #216
	movs r2, #196
	movs r0, #19
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x02009a18
.L_0200136c_3:
	ldr r0, [pc, #544]
	bl 0x02009988
	adds r5, r0, #0
	ldr r0, [pc, #540]
	bl 0x02009988
	cmp r0, #0
	bne .L_0200136c_4
	adds r3, r7, #0
	ands r3, r5
	cmp r3, #0
	beq .L_0200136c_4
	movs r0, #21
	bl 0x020099e0
	ldr r3, [pc, #520]
	str r3, [r0, #108]
.L_0200136c_4:
	ldr r3, [pc, #520]
	movs r2, #225
	lsls r2, r2, #1
	adds r5, r3, r2
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #2
	bgt .L_0200136c_5
	ldr r0, [pc, #508]
	bl 0x02009988
	cmp r0, #0
	bne .L_0200136c_5
	ldr r0, [pc, #500]
	bl 0x02009998
	ldr r0, [pc, #480]
	bl 0x02009988
	cmp r0, #0
	bne .L_0200136c_5
	ldr r0, [pc, #488]
	bl 0x02009988
	cmp r0, #0
	beq .L_0200136c_5
	bl 0x020099b8
	movs r0, #0
	bl 0x020099e0
	cmp r0, #0
.L_0200136c_2:
	beq .L_0200136c_6
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x02009a18
.L_0200136c_6:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #1
	bne .L_0200136c_7
	movs r1, #200
	movs r2, #224
	movs r0, #2
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x02009a18
	b .L_0200136c_8
.L_0200136c_7:
	movs r1, #224
	movs r2, #162
	movs r0, #2
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x02009a18
.L_0200136c_8:
	movs r2, #0
	movs r1, #0
	movs r0, #2
	bl 0x02009a40
	bl 0x02009aa8
	bl 0x02009ab8
	movs r0, #30
	bl 0x020099b0
	movs r1, #2
	movs r0, #2
	bl 0x02009a38
	ldr r0, [pc, #388]
	bl 0x02009a50
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x02009a68
	movs r0, #0
	movs r1, #3
	bl 0x02009a28
	movs r0, #2
	ldr r1, [pc, #368]
	ldr r2, [pc, #368]
	bl 0x020099e8
	movs r0, #2
	movs r1, #2
	bl 0x02009a20
	movs r0, #0
	bl 0x020099e0
	cmp r0, #0
	beq .L_0200136c_9
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x020099f0
.L_0200136c_9:
	movs r0, #2
	bl 0x02009a10
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x02009a18
	movs r0, #2
	movs r1, #0
	bl 0x020099c8
	bl 0x020099c0
.L_0200136c_5:
	ldr r0, [pc, #288]
	bl 0x02009988
	cmp r0, #0
	beq .L_0200136c_10
	movs r1, #204
	movs r2, #240
	movs r0, #23
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl 0x02009a18
.L_0200136c_10:
	ldr r3, [pc, #256]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #11
	bne .L_0200136c_11
	ldr r0, [pc, #232]
	bl 0x02009988
	cmp r0, #0
	bne .L_0200136c_12
	ldr r0, [pc, #240]
	bl 0x02009988
	cmp r0, #0
	beq .L_0200136c_12
	movs r0, #2
	bl 0x02009988
	cmp r0, #0
	bne .L_0200136c_12
	bl 0x020099b8
	movs r1, #160
	movs r2, #155
	movs r0, #2
	lsls r1, r1, #14
	lsls r2, r2, #17
	bl 0x02009a18
	movs r2, #0
	movs r1, #0
	movs r0, #2
	bl 0x02009a40
	bl 0x02009aa8
	bl 0x02009ab8
	movs r0, #30
	bl 0x020099b0
	movs r1, #2
	movs r0, #2
	bl 0x02009a38
	ldr r0, [pc, #176]
	bl 0x02009a50
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x02009a68
	movs r0, #0
	movs r1, #3
	bl 0x02009a28
	movs r0, #2
	ldr r1, [pc, #152]
	ldr r2, [pc, #156]
	bl 0x020099e8
	movs r0, #2
	movs r1, #2
	bl 0x02009a20
	movs r0, #0
	bl 0x020099e0
	cmp r0, #0
	beq .L_0200136c_13
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x020099f0
.L_0200136c_13:
	movs r0, #2
	bl 0x02009a10
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x02009a18
	movs r0, #2
	movs r1, #0
	bl 0x020099c8
	bl 0x020099c0
.L_0200136c_12:
	ldr r0, [pc, #92]
	bl 0x02009998
	b .L_0200136c_14
.L_0200136c_11:
	cmp r3, #13
	bne .L_0200136c_14
	ldr r0, [pc, #44]
	bl 0x02009988
	cmp r0, #0
	beq .L_0200136c_14
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x02009a18
.L_0200136c_14:
	movs r0, #0
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0000087a
	.4byte 0x02008315
	.4byte 0x00000858
	.4byte 0x00000853
	.4byte 0x00000855
	.4byte 0x020083ad
	.4byte 0x02000240
	.4byte 0x00000109
	.4byte 0x00000867
	.4byte 0x00000856
	.4byte 0x00001328
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x0000012f
	.global Func_02001638
	.thumb_func
Func_02001638:
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
	bl 0x02009adc
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x030001d8
	.global Func_02001674
	.thumb_func
Func_02001674:
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
.L_0200168c:
	adds r5, #8
	mov r10, r2
	adds r0, r7, #0
	movs r2, #0
	adds r1, r5, #0
	mov r11, r3
	mov r9, r2
	bl 0x02009638
	cmp r0, r10
	blt .L_0200168c_0
	mov r3, r11
	cmp r3, #0
	beq 0x020096fe
.L_0200168c_0:
	mov r2, r8
	ldr r0, [r2, #16]
	ldr r3, [r6, #16]
	ldr r1, [r7]
	subs r0, r0, r3
	ldr r3, [r5]
	subs r1, r1, r3
	bl 0x02009920
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
	beq .L_0200168c_1
	cmp r1, r3
	beq .L_0200168c_1
	cmp r4, r3
	beq .L_0200168c_1
	mov r3, r11
	cmp r3, #0
	beq 0x0200970e
.L_0200168c_1:
	adds r2, r6, #0
	adds r2, #91
	movs r3, #1
.L_020016ee:
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #1
	bl 0x02009928
	movs r2, #1
	mov r9, r2
	b .L_020016ee_0
	.2byte 0x1c33
	.2byte 0x335b
	.2byte 0x464a
	.2byte 0x701a
	.2byte 0x1c30
	.2byte 0x2102
	.2byte 0xf000
	.2byte 0xf90d
.L_020016ee_0:
	mov r0, r9
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0xf000
	.2byte 0xffff
	.global Func_02001724
	.thumb_func
Func_02001724:
	push {r5, lr}
	adds r5, r1, #0
	movs r1, #0
	bl 0x02009a60
.L_0200172e:
	adds r0, r5, #0
	bl 0x020099b0
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200173c
	.thumb_func
Func_0200173c:
	push {r5, lr}
	adds r5, r2, #0
	movs r2, #0
	bl 0x02009a40
	adds r0, r5, #0
	bl 0x020099b0
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001754
	.thumb_func
Func_02001754:
	push {r5, r6, lr}
	adds r6, r0, #0
	ldr r3, [r6, #8]
	ldr r2, [r6, #48]
	adds r3, r3, r2
	str r3, [r6, #8]
	str r3, [r6, #56]
	adds r3, r6, #0
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_02001754_0
	ldr r3, [r6, #12]
	ldr r2, [r6, #52]
	b .L_02001754_1
.L_02001754_0:
	ldr r3, [r6, #16]
	ldr r2, [r6, #52]
	adds r3, r3, r2
	str r3, [r6, #16]
	str r3, [r6, #64]
	movs r2, #128
	ldr r3, [r6, #12]
	lsls r2, r2, #3
.L_02001754_1:
	adds r3, r3, r2
	str r3, [r6, #12]
	str r3, [r6, #60]
	ldr r5, [r6, #48]
	movs r1, #28
	adds r0, r5, #0
	bl 0x02009900
	subs r5, r5, r0
	str r5, [r6, #48]
	ldr r5, [r6, #52]
	movs r1, #28
	adds r0, r5, #0
	bl 0x02009900
	subs r5, r5, r0
	str r5, [r6, #52]
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_020017ac
	.thumb_func
Func_020017ac:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r7, r0, #0
	movs r0, #19
	bl 0x020099e0
	mov r8, r0
	cmp r0, #0
	bne .L_020017ac_0
	b .L_020017ac_1
.L_020017ac_0:
	bl 0x02009918
	adds r5, r0, #0
	bl 0x02009918
	lsls r5, r5, #3
	adds r3, r0, #0
	lsrs r5, r5, #16
	mov r0, r8
	lsls r3, r3, #3
	ldr r2, [r0, #8]
	subs r5, #4
	lsrs r3, r3, #16
	ldr r1, [r0, #16]
	subs r3, #4
	lsls r5, r5, #16
	adds r5, r5, r2
	lsls r3, r3, #16
	ldr r2, [r0, #12]
	adds r3, r3, r1
	movs r0, #172
	adds r1, r5, #0
	bl 0x02009938
	adds r6, r0, #0
	cmp r6, #0
	beq .L_020017ac_1
	ldr r1, [r6, #80]
	mov r10, r1
	bl 0x02009918
	movs r3, #1
	ands r0, r3
	cmp r0, #1
	bne .L_020017ac_2
	adds r0, r6, #0
	movs r1, #3
	bl 0x02009928
	ldr r1, [pc, #208]
	adds r0, r6, #0
	bl 0x02009930
	b .L_020017ac_3
.L_020017ac_2:
	adds r0, r6, #0
	movs r1, #2
	bl 0x02009928
	ldr r1, [pc, #196]
	adds r0, r6, #0
	bl 0x02009930
.L_020017ac_3:
	movs r2, #0
	adds r3, r6, #0
	mov r9, r2
	adds r3, #85
	mov r0, r9
	strb r0, [r3]
	movs r3, #2
	ands r3, r7
	cmp r3, #0
	beq .L_020017ac_4
	bl 0x02009918
	movs r1, #10
	bl 0x02009908
	movs r5, #1
	ands r7, r5
	adds r3, r7, #0
	eors r3, r5
	lsls r3, r3, #2
	adds r0, #5
	adds r0, r0, r3
	ldr r3, [pc, #148]
	muls r3, r7
	ldr r1, [pc, #148]
	adds r3, r3, r1
	muls r3, r0
	str r3, [r6, #52]
	bl 0x02009918
	movs r1, #15
	bl 0x02009908
	ldr r3, [pc, #132]
	subs r0, #7
	muls r3, r0
	str r3, [r6, #48]
	adds r3, r6, #0
	adds r3, #100
	mov r2, r9
	strh r2, [r3]
	b .L_020017ac_5
.L_020017ac_4:
	bl 0x02009918
	movs r1, #10
	bl 0x02009908
	ldr r3, [pc, #96]
	muls r3, r7
	ldr r1, [pc, #96]
	adds r0, #8
	adds r3, r3, r1
	muls r3, r0
	str r3, [r6, #48]
	bl 0x02009918
	movs r1, #14
	bl 0x02009908
	ldr r3, [pc, #80]
	adds r0, #1
	muls r3, r0
	adds r2, r6, #0
	str r3, [r6, #52]
	adds r2, #100
	movs r3, #1
	strh r3, [r2]
.L_020017ac_5:
	ldr r3, [pc, #68]
	mov r2, r10
	adds r2, #38
	str r3, [r6, #108]
	movs r3, #0
	strb r3, [r2]
	mov r2, r8
	ldr r3, [r2, #80]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	mov r3, r10
	ldrb r1, [r3, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	mov r0, r10
	strb r3, [r0, #9]
.L_020017ac_1:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200a8c4
	.4byte 0x0200a8dc
	.4byte 0x00003332
	.4byte 0xffffe667
	.4byte 0x00001999
	.4byte 0x02009755
	.include "games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_MURA/IMPORT.INC"
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
	.4byte 0x02009b0c
	.4byte 0x02009b44
	.4byte 0x02009b7c
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00006666
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x80010000
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000001c
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x000000f0
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000001c
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x000000f0
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00006666
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000015
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000015
	.4byte 0x00000008
	.4byte 0x00280000
	.4byte 0x80010000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000009
	.4byte 0x00080000
	.4byte 0x00100000
	.4byte 0x00180000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000028
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
	.4byte 0x000000df
	.4byte 0x40000087
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
	.4byte 0x00000098
	.4byte 0x40000110
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00103002
	.4byte 0x0022c002
	.4byte 0x00505015
	.4byte 0x00606015
	.4byte 0x00707015
	.4byte 0x00808015
	.4byte 0x00909015
	.4byte 0x00a0a015
	.4byte 0x00b0a009
	.4byte 0x00c01016
	.4byte 0x00d0d015
	.4byte 0x00e01017
	.4byte 0x000001ff
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00005000
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
	.4byte 0x00005000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00960000
	.4byte 0x0000b000
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
	.4byte 0x02009db4
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00028000
	.4byte 0xffff0066
	.4byte 0x02009e88
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
	.4byte 0x08540040
	.4byte 0x00000001
	.4byte 0x005c0000
	.4byte 0x00000000
	.4byte 0x011c0000
	.4byte 0x00005000
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
	.4byte 0x01880000
	.4byte 0x00005000
	.4byte 0x0853006c
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00003000
	.4byte 0x1853006a
	.4byte 0x02009bc0
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00005000
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
	.4byte 0x0000b000
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
	.4byte 0x02009db4
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00028000
	.4byte 0xffff0066
	.4byte 0x02009e88
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
	.4byte 0x08540016
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00005000
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
	.4byte 0x0855006a
	.4byte 0x02009bc0
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00005000
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
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x020084a1
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x020084a1
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02008ea5
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008ecd
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008f3d
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008f65
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008f8d
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008fb5
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02009011
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x0200906d
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x02009091
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte 0x0200913d
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte 0x02009199
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001222
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008615
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001226
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008779
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001228
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008675
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x020087ed
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0200885d
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008799
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x020086d1
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008725
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x020087b9
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x0000128f
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x020088cd
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001236
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001237
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001238
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x02008c8d
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000123a
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x02008cc5
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x02008cfd
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x02008d79
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x02008dd5
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000123f
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001240
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x02008e0d
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001295
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x02008e85
	.4byte 0x00000003
	.4byte 0xffff001e
	.4byte 0x02009239
	.4byte 0x00000003
	.4byte 0xffff005a
	.4byte 0x020085c9
	.4byte 0x000000d3
	.4byte 0x0f4a0065
	.4byte 0x001000b5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x020084a1
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x020084a1
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02008ea5
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008ecd
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008f3d
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008f65
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008f8d
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008fb5
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02009011
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x0200906d
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x02009091
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte 0x0200913d
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte 0x02009199
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200897d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001333
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001334
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020089d1
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020089f1
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008a2d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008a4d
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008acd
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008b3d
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008b99
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x02008bed
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x02008c55
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001343
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001344
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001345
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x02008c8d
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001347
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x02008cc5
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x02008cfd
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x02008d79
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x02008dd5
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x0000134d
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x02008e0d
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001383
	.4byte 0x00000003
	.4byte 0xffff001e
	.4byte 0x02009239
	.4byte 0x00000003
	.4byte 0xffff005a
	.4byte 0x020085c9
	.4byte 0x0000e714
	.4byte 0x08580013
	.4byte 0x020092c1
	.4byte 0x000000d3
	.4byte 0x0f4a0065
	.4byte 0x001000b5
	.4byte 0xffffffff
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
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000076
	.4byte 0x00000000
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x0000001b
