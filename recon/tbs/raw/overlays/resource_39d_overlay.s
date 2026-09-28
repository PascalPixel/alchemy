.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/ENTRY.INC"
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
	bl 0x0200b670
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
	bl 0x0200b690
	adds r0, r5, #0
	movs r1, #14
	bl 0x0200b760
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200b698
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
	bl 0x0200b670
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
	bl 0x0200b690
	adds r0, r5, #0
	movs r1, #15
	bl 0x0200b760
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
	bl 0x0200b6f0
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
	bl 0x0200b670
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
	bl 0x0200b660
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl 0x0200b668
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
	bl 0x0200b760
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
	bl 0x0200b628
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
	bl 0x0200b628
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200013c_9:
	bl 0x0200b628
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
	bl 0x0200b660
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl 0x0200b668
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
	.4byte 0x0200b92c
	.4byte 0x02008105
	.4byte 0xffff0000
	.global Func_02000314
	.thumb_func
Func_02000314:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200b938
	.global Func_0200031c
	.thumb_func
Func_0200031c:
	movs r0, #0
	bx lr
	.global Func_02000320
	.thumb_func
Func_02000320:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200b9c8
	.global Func_02000328
	.thumb_func
Func_02000328:
	push {lr}
	ldr r3, [pc, #28]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	beq .L_02000328_0
	ldr r0, [pc, #16]
	bl 0x0200b6b8
.L_02000328_0:
	ldr r0, [pc, #12]
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000253
	.4byte 0x0200b9d4
	.global Func_02000354
	.thumb_func
Func_02000354:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200bbe4
	.global Func_0200035c
	.thumb_func
Func_0200035c:
	push {r5, r6, lr}
	ldr r0, [pc, #484]
	sub sp, #8
	bl 0x0200b6b8
	ldr r3, [pc, #480]
	movs r2, #224
	ldr r1, [r3]
	movs r3, #129
	lsls r2, r2, #1
	lsls r3, r3, #2
	str r3, [r1, r2]
	ldr r3, [pc, #468]
	ldrsh r2, [r3, r2]
	ldr r3, [pc, #468]
	cmp r2, r3
	beq .L_0200035c_0
	b .L_0200035c_1
.L_0200035c_0:
	movs r0, #162
	lsls r0, r0, #1
	bl 0x0200b6b8
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #452]
	bl 0x0200b638
	movs r0, #0
	movs r1, #1
	bl 0x0200b790
	movs r0, #1
	movs r1, #1
	bl 0x0200b790
	movs r0, #2
	movs r1, #1
	bl 0x0200b790
	movs r0, #3
	movs r1, #1
	bl 0x0200b790
	movs r0, #5
	movs r1, #1
	bl 0x0200b790
	movs r0, #20
	movs r1, #1
	bl 0x0200b790
	movs r0, #21
	movs r1, #1
	bl 0x0200b790
	movs r0, #22
	movs r1, #1
	bl 0x0200b790
	movs r0, #23
	movs r1, #1
	bl 0x0200b790
	movs r0, #24
	movs r1, #1
	bl 0x0200b790
	movs r0, #8
	movs r1, #1
	bl 0x0200b790
	movs r0, #9
	movs r1, #1
	bl 0x0200b790
	movs r0, #10
	movs r1, #1
	bl 0x0200b790
	movs r0, #11
	movs r1, #1
	bl 0x0200b790
	movs r0, #12
	movs r1, #1
	bl 0x0200b790
	movs r0, #13
	movs r1, #1
	bl 0x0200b790
	movs r5, #14
	movs r6, #0
.L_0200035c_2:
	movs r1, #1
	adds r0, r5, #0
	bl 0x0200b790
	adds r0, r5, #0
	bl 0x0200b6f0
	movs r3, #4
	adds r0, #85
	strb r3, [r0]
	adds r0, r5, #0
	bl 0x0200b6f0
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #2
	orrs r3, r2
	strb r3, [r0]
	adds r0, r5, #0
	bl 0x0200b6f0
	ldr r3, [pc, #276]
	adds r5, #1
	str r3, [r0, #12]
	cmp r5, #19
	bls .L_0200035c_2
	ldr r0, [pc, #272]
	bl 0x0200b6b0
	cmp r0, #0
	beq .L_0200035c_3
	bl 0x020088cc
	cmp r0, #0
	beq .L_0200035c_3
	bl 0x0200b6f0
	cmp r0, #0
	beq .L_0200035c_3
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_0200035c_3:
	movs r0, #9
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r0, #10
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r0, #11
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r0, #12
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r0, #13
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r0, #12
	bl 0x0200b6f0
	ldr r5, [pc, #176]
	str r5, [r0, #24]
	movs r0, #13
	bl 0x0200b6f0
	ldr r3, [pc, #144]
	movs r2, #225
	str r5, [r0, #24]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #1
	bne .L_0200035c_4
	ldr r0, [pc, #144]
	bl 0x0200b6b0
	cmp r0, #0
	bne .L_0200035c_1
	bl 0x0200856c
	b .L_0200035c_1
.L_0200035c_4:
	cmp r3, #2
	bne .L_0200035c_5
	ldr r0, [pc, #132]
	bl 0x0200b6b0
	cmp r0, #0
	bne .L_0200035c_1
	ldr r3, [pc, #124]
	movs r2, #178
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #19
	str r2, [r3, #12]
	bl 0x0200b678
	movs r0, #1
	bl 0x0200b630
	movs r3, #5
	movs r2, #4
	str r3, [sp, #0]
	movs r0, #4
	movs r1, #70
	movs r3, #74
	str r2, [sp, #4]
	bl 0x0200b680
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200b728
	ldr r0, [pc, #60]
	bl 0x0200b6b0
	cmp r0, #0
	bne .L_0200035c_1
	bl 0x02009af0
	b .L_0200035c_1
.L_0200035c_5:
	cmp r3, #5
	bne .L_0200035c_1
	ldr r0, [pc, #48]
	bl 0x0200b6b8
.L_0200035c_1:
	movs r0, #0
	sub sp, #-8
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00000111
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000003a
	.4byte 0x0200b4bd
	.4byte 0xffcd8000
	.4byte 0x00000109
	.4byte 0xffff0000
	.4byte 0x00000251
	.4byte 0x03001e70
	.global Func_0200056c
	.thumb_func
Func_0200056c:
	push {lr}
	bl 0x0200b6d8
	bl 0x0200b6c8
	movs r0, #0
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r0, #1
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r0, #2
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r0, #3
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r0, #152
	movs r1, #1
	movs r2, #240
	movs r3, #0
	negs r1, r1
	lsls r2, r2, #15
	lsls r0, r0, #17
	bl 0x0200b7b0
	movs r0, #1
	bl 0x0200b630
	bl 0x0200b678
	movs r0, #1
	bl 0x0200b630
	movs r0, #141
	bl 0x0200b840
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl 0x0200b6a0
	ldr r0, [pc, #652]
	bl 0x0200b840
	movs r0, #1
	movs r1, #1
	negs r1, r1
	negs r0, r0
	ldr r2, [pc, #640]
	bl 0x0200b6a0
	ldr r3, [pc, #640]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	bl 0x0200b7f8
	bl 0x0200b800
	bl 0x0200b6a8
	movs r0, #30
	bl 0x0200b6d0
	bl 0x0200b7c0
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	ldr r1, [pc, #600]
	ldr r0, [pc, #604]
	bl 0x0200b7a8
	movs r0, #128
	movs r2, #160
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #18
	ldr r1, [pc, #592]
	bl 0x0200b7b0
	bl 0x0200b7b8
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl 0x0200b7e8
	movs r1, #0
	ldr r0, [pc, #572]
	bl 0x0200b7e0
	movs r0, #50
	bl 0x0200b7f0
	movs r0, #50
	bl 0x0200b6d0
	movs r1, #0
	ldr r0, [pc, #556]
	bl 0x0200b7e0
	movs r0, #30
	bl 0x0200b7f0
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #252
	movs r2, #168
	movs r0, #0
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200b728
	movs r1, #132
	movs r2, #144
	movs r0, #1
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl 0x0200b728
	movs r1, #244
	movs r2, #144
	movs r0, #2
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200b728
	movs r1, #128
	movs r2, #152
	lsls r2, r2, #16
	movs r0, #3
	lsls r1, r1, #18
	bl 0x0200b728
	movs r0, #0
	movs r1, #19
	bl 0x0200b730
	movs r0, #1
	movs r1, #19
	bl 0x0200b730
	movs r0, #2
	movs r1, #19
	bl 0x0200b730
	movs r1, #19
	movs r0, #3
	bl 0x0200b730
	movs r0, #10
	bl 0x0200b6d0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl 0x0200b7e0
	movs r0, #30
	bl 0x0200b7f0
	movs r0, #30
	bl 0x0200b6d0
	movs r0, #80
	bl 0x0200b6d0
	movs r0, #0
	bl 0x0200b6f0
	movs r1, #1
	bl 0x0200b690
	movs r1, #1
	movs r0, #0
	bl 0x0200b730
	movs r0, #30
	bl 0x0200b6d0
	movs r0, #0
	movs r1, #4
	bl 0x0200b738
	movs r1, #192
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200b788
	movs r1, #2
	movs r0, #0
	bl 0x0200b740
	movs r0, #60
	bl 0x0200b6d0
	movs r0, #1
	bl 0x0200b6f0
	movs r1, #1
	bl 0x0200b690
	movs r1, #1
	movs r0, #1
	bl 0x0200b730
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b788
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #1
	bl 0x0200b788
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #0
	movs r1, #0
	movs r0, #1
	bl 0x0200b788
	movs r0, #2
	bl 0x0200b6f0
	movs r1, #1
	bl 0x0200b690
	movs r0, #2
	movs r1, #1
	bl 0x0200b730
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #1
	bl 0x0200b788
	movs r0, #40
	bl 0x0200b6d0
	movs r0, #3
	bl 0x0200b6f0
	movs r1, #1
	bl 0x0200b690
	movs r1, #1
	movs r0, #3
	bl 0x0200b730
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #3
	movs r0, #3
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #1
	ldr r1, [pc, #192]
	ldr r2, [pc, #208]
	bl 0x0200b6f8
	movs r0, #2
	ldr r1, [pc, #184]
	ldr r2, [pc, #196]
	bl 0x0200b6f8
	ldr r2, [pc, #192]
	movs r0, #3
	ldr r1, [pc, #172]
	bl 0x0200b6f8
	movs r0, #1
	movs r1, #2
	bl 0x0200b730
	movs r0, #2
	movs r1, #2
	bl 0x0200b730
	movs r0, #3
	movs r1, #2
	bl 0x0200b730
	movs r0, #0
	bl 0x0200b6f0
	cmp r0, #0
	beq .L_0200056c_0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200b700
.L_0200056c_0:
	movs r0, #0
	bl 0x0200b6f0
	cmp r0, #0
	beq .L_0200056c_1
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x0200b700
.L_0200056c_1:
	movs r0, #0
	bl 0x0200b6f0
	cmp r0, #0
	beq .L_0200056c_2
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl 0x0200b700
.L_0200056c_2:
	movs r0, #3
	bl 0x0200b720
	movs r1, #0
	movs r2, #0
	movs r0, #3
	bl 0x0200b728
	movs r0, #2
	bl 0x0200b720
	movs r1, #0
	movs r2, #0
	movs r0, #2
	bl 0x0200b728
	movs r0, #1
	bl 0x0200b720
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200b728
	bl 0x0200b6e0
	pop {r0}
	bx r0
	.4byte 0x00000121
	.4byte 0x0000e666
	.4byte 0x03001ebc
	.4byte 0x00001999
	.4byte 0x0000cccc
	.4byte 0xffe80000
	.4byte 0x00010005
	.4byte 0x00007fff
	.4byte 0x00006666
	.global Func_02000890
	.thumb_func
Func_02000890:
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
	bl 0x0200b854
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x030001d8
	.global Func_020008cc
	.thumb_func
Func_020008cc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, [pc, #76]
	movs r2, #0
	movs r0, #0
	ldr r5, [r3]
	mov r8, r2
	bl 0x0200b6f0
	movs r7, #160
	lsls r7, r7, #2
	mov r10, r0
	movs r6, #8
	adds r5, #52
.L_020008cc_1:
	ldmia r5!, {r1}
	cmp r1, #0
	beq .L_020008cc_0
	ldr r3, [r1, #80]
	ldr r3, [r3, #40]
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #242
	bne .L_020008cc_0
	mov r0, r10
	adds r1, #8
	adds r0, #8
	bl 0x02008890
	cmp r0, r7
	bge .L_020008cc_0
	adds r7, r0, #0
	mov r8, r6
.L_020008cc_0:
	adds r6, #1
	cmp r6, #65
	bls .L_020008cc_1
	mov r0, r8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.global Func_02000928
	.thumb_func
Func_02000928:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #0
	sub sp, #12
	bl 0x0200b6f0
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
	bl 0x0200b658
	adds r0, r5, #0
	adds r1, r6, #0
	bl 0x0200b688
	cmp r0, #0
	bne .L_02000928_0
	movs r0, #148
	lsls r0, r0, #2
	bl 0x0200b6c0
	bl 0x02008ad0
	movs r1, #6
	adds r0, r5, #0
	bl 0x0200b660
	movs r0, #6
	bl 0x0200b630
	movs r0, #152
	bl 0x0200b840
	adds r0, r5, #0
	movs r1, #7
	bl 0x0200b660
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
	bl 0x0200b690
	movs r3, #10
	ldrsh r2, [r6, r3]
	movs r3, #2
	ldrsh r1, [r6, r3]
	movs r0, #0
	bl 0x0200b708
	adds r0, r5, #0
	movs r1, #6
	bl 0x0200b660
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200b690
	mov r3, r8
	strb r3, [r7]
.L_02000928_0:
	sub sp, #-12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0xfff00000
	.global Func_020009fc
	.thumb_func
Func_020009fc:
	push {r5, r6, r7, lr}
	movs r0, #0
	bl 0x0200b6f0
	adds r7, r0, #0
	bl 0x0200b6d8
	bl 0x020088cc
	ldr r5, [pc, #180]
	str r0, [r5]
	cmp r0, #0
	beq 0x02008ab8
	movs r0, #148
	lsls r0, r0, #2
	bl 0x0200b6b8
	ldr r0, [r5]
	bl 0x0200b6f0
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
.L_02000a42:
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r3, [r7, #20]
	adds r3, r3, r2
	str r3, [r7, #20]
	movs r0, #2
	bl 0x0200b630
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
	bl 0x0200b630
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
	bl 0x0200b630
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
	bl 0x0200b630
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
	bl 0x0200b6e0
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0xbc50
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0xfffd
	.4byte 0xfffe0000
	.global Func_02000ad0
	.thumb_func
Func_02000ad0:
	push {r5, lr}
	movs r0, #0
	bl 0x0200b6f0
	movs r3, #3
	adds r0, #85
	strb r3, [r0]
	movs r0, #14
	bl 0x0200b6f0
	movs r5, #4
	adds r0, #85
	strb r5, [r0]
	movs r0, #15
	bl 0x0200b6f0
	adds r0, #85
	strb r5, [r0]
	movs r0, #16
	bl 0x0200b6f0
	adds r0, #85
	strb r5, [r0]
	movs r0, #17
	bl 0x0200b6f0
	adds r0, #85
	strb r5, [r0]
	movs r0, #18
	bl 0x0200b6f0
	adds r0, #85
	strb r5, [r0]
	movs r0, #19
	bl 0x0200b6f0
	adds r0, #85
	strb r5, [r0]
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000b24
	.thumb_func
Func_02000b24:
	push {r5, r6, lr}
	bl 0x0200b6d8
	movs r0, #17
	bl 0x0200b6f0
	adds r0, #85
	ldrb r2, [r0]
	movs r3, #250
	ands r3, r2
	strb r3, [r0]
	ldr r1, [pc, #672]
	movs r0, #0
	ldr r2, [pc, #672]
	bl 0x0200b6f8
	movs r0, #1
	ldr r1, [pc, #660]
	ldr r2, [pc, #660]
	bl 0x0200b6f8
	movs r0, #2
	ldr r1, [pc, #648]
	ldr r2, [pc, #652]
	bl 0x0200b6f8
	movs r0, #3
	ldr r1, [pc, #640]
	ldr r2, [pc, #640]
	bl 0x0200b6f8
	movs r0, #0
	bl 0x0200b6f0
	cmp r0, #0
	beq .L_02000b24_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200b728
.L_02000b24_0:
	movs r0, #0
	bl 0x0200b6f0
	cmp r0, #0
	beq .L_02000b24_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x0200b728
.L_02000b24_1:
	movs r0, #0
	bl 0x0200b6f0
	cmp r0, #0
	beq .L_02000b24_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x0200b728
.L_02000b24_2:
	movs r0, #1
	bl 0x0200b630
	movs r1, #172
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #232
	bl 0x0200b710
	movs r1, #164
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #232
	bl 0x0200b710
	movs r1, #172
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #248
	bl 0x0200b710
	movs r1, #164
	lsls r1, r1, #1
	movs r2, #248
	movs r0, #3
	bl 0x0200b710
	movs r0, #0
	bl 0x0200b720
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200b788
	movs r0, #1
	bl 0x0200b720
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b788
	movs r0, #2
	bl 0x0200b720
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #2
	bl 0x0200b788
	movs r0, #3
	bl 0x0200b720
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #3
	bl 0x0200b788
	movs r0, #50
	bl 0x0200b6d0
	movs r1, #2
	movs r0, #1
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200b7a8
	movs r0, #164
	movs r1, #160
	movs r2, #176
	lsls r1, r1, #14
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #17
	bl 0x0200b7b0
	bl 0x0200b7c0
	movs r3, #0
	adds r0, #85
	movs r1, #192
	movs r2, #192
	strb r3, [r0]
	lsls r1, r1, #9
	movs r0, #1
	lsls r2, r2, #8
	bl 0x0200b6f8
	movs r1, #164
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #216
	bl 0x0200b718
	bl 0x0200b7b8
	ldr r1, [pc, #356]
	ldr r2, [pc, #356]
	movs r0, #1
	bl 0x0200b6f8
	movs r0, #60
	bl 0x0200b6d0
	movs r0, #172
	movs r1, #192
	movs r2, #232
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #16
	bl 0x0200b7b0
	movs r1, #128
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #6
	bl 0x0200b788
	bl 0x0200b7b8
	movs r1, #4
	movs r0, #1
	bl 0x0200b738
	ldr r0, [pc, #304]
	bl 0x0200b768
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200b780
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200b7a0
	movs r0, #60
	bl 0x0200b6d0
	movs r1, #192
	movs r2, #192
	movs r0, #3
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200b6f8
	movs r1, #164
	lsls r1, r1, #1
	movs r2, #232
	movs r0, #3
	bl 0x0200b718
	movs r0, #10
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200b780
	movs r1, #4
	movs r0, #3
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #3
	movs r1, #0
	movs r2, #30
	bl 0x0200b780
	movs r1, #0
	movs r2, #0
	movs r0, #2
	bl 0x0200b750
	movs r0, #60
	bl 0x0200b6d0
.L_02000d22:
	movs r1, #168
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #248
	bl 0x0200b718
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200b788
	movs r0, #30
	bl 0x0200b6d0
	movs r0, #2
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r0, #3
	movs r1, #0
	movs r2, #30
	bl 0x0200b788
	movs r1, #0
	movs r0, #3
	bl 0x0200b770
	movs r0, #0
	movs r1, #0
	bl 0x0200b6e8
	cmp r0, #0
	bne .L_02000d22_0
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200b798
	movs r1, #128
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200b788
	movs r0, #1
	movs r1, #3
	bl 0x0200b738
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #192
	movs r2, #0
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200b788
	movs r1, #4
	movs r0, #1
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200b780
	ldr r3, [pc, #28]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	b .L_02000d22_1
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x159c
	.2byte 0x0000
	.4byte 0x03001ebc
.L_02000d22_0:
	ldr r3, [pc, #1016]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #3
	ldr r1, [pc, #996]
	movs r2, #60
	bl 0x0200b798
	movs r1, #128
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200b788
	movs r0, #1
	movs r1, #4
	bl 0x0200b738
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #192
	movs r2, #0
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200b788
	movs r1, #2
	movs r0, #1
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
.L_02000d22_1:
	movs r0, #3
	ldr r1, [pc, #920]
	movs r2, #60
	bl 0x0200b798
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #1
	movs r2, #0
	movs r0, #0
	bl 0x0200b750
	movs r0, #60
	bl 0x0200b6d0
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #128
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200b788
	movs r0, #0
	movs r1, #3
	bl 0x0200b730
	movs r1, #3
	movs r0, #1
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200b780
	movs r0, #3
	movs r1, #2
	bl 0x0200b748
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200b780
	movs r0, #0
	movs r1, #3
	bl 0x0200b730
	movs r0, #1
	movs r1, #3
	bl 0x0200b730
	movs r1, #3
	movs r0, #2
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #4
	movs r0, #3
	bl 0x0200b738
	movs r0, #10
	bl 0x0200b6d0
	movs r2, #10
	movs r0, #3
	movs r1, #0
	bl 0x0200b788
	movs r0, #3
	movs r1, #16
	bl 0x0200b730
	movs r1, #0
	movs r2, #60
	movs r0, #3
	bl 0x0200b780
	movs r0, #17
	bl 0x0200b840
	movs r1, #216
	movs r2, #200
	movs r0, #5
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200b728
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #240
	movs r2, #160
	movs r0, #5
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl 0x0200b728
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl 0x0200b788
	movs r0, #3
	movs r1, #1
	bl 0x0200b730
	movs r0, #0
	movs r1, #1
	bl 0x0200b740
	movs r0, #1
	movs r1, #1
	bl 0x0200b748
	movs r0, #0
	movs r1, #0
	movs r2, #10
	bl 0x0200b788
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200b788
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200b788
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200b788
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #5
	bl 0x0200b788
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200b788
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #5
	bl 0x0200b788
	movs r1, #128
	movs r2, #5
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200b788
	movs r0, #0
	movs r1, #2
	bl 0x0200b740
	movs r1, #128
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200b788
	movs r1, #2
	movs r0, #1
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200b7a8
	movs r0, #240
	movs r2, #168
	movs r3, #1
	lsls r2, r2, #16
	ldr r1, [pc, #520]
	lsls r0, r0, #15
	bl 0x0200b7b0
	bl 0x0200b7b8
	movs r0, #40
	bl 0x0200b6d0
	movs r1, #1
	movs r0, #21
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #61
	bl 0x0200b840
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200b780
	movs r1, #1
	movs r0, #23
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #23
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #192
	movs r0, #23
	lsls r1, r1, #8
	movs r2, #60
	bl 0x0200b788
	movs r0, #23
	movs r1, #0
	movs r2, #20
	bl 0x0200b788
	movs r0, #23
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #132
	movs r2, #144
	movs r0, #1
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200b728
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #164
	movs r2, #216
	lsls r2, r2, #16
	movs r0, #1
	lsls r1, r1, #17
	bl 0x0200b728
	movs r1, #4
	movs r0, #23
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #23
	movs r1, #0
	bl 0x0200b780
	movs r1, #3
	movs r0, #23
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #23
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #160
	movs r0, #23
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200b788
	movs r1, #128
	movs r2, #10
	movs r0, #21
	lsls r1, r1, #6
	bl 0x0200b788
	movs r1, #1
	movs r0, #21
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #30
	bl 0x0200b788
	movs r2, #30
	movs r0, #21
	movs r1, #0
	bl 0x0200b788
	movs r0, #5
	movs r1, #2
	bl 0x0200b748
	movs r1, #4
	movs r0, #21
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #128
	movs r2, #10
	movs r0, #21
	lsls r1, r1, #6
	bl 0x0200b788
	movs r1, #3
	movs r0, #21
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #23
	movs r1, #3
	bl 0x0200b738
	movs r0, #21
	ldr r1, [pc, #204]
	ldr r2, [pc, #204]
	bl 0x0200b6f8
	movs r1, #104
	movs r2, #168
	movs r0, #21
	bl 0x0200b718
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #23
	movs r1, #0
	movs r2, #20
	bl 0x0200b788
	movs r0, #20
	movs r1, #0
	movs r2, #10
	bl 0x0200b788
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b798
	movs r1, #129
	movs r0, #20
	lsls r1, r1, #1
	movs r2, #70
	bl 0x0200b798
	movs r1, #132
	movs r2, #144
	movs r0, #22
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200b728
	movs r0, #22
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #148
	movs r2, #240
	movs r0, #22
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl 0x0200b728
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
.L_02001196:
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x0200b788
	movs r2, #0
	movs r0, #20
	movs r1, #0
	bl 0x0200b788
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200b7a8
	movs r0, #232
	movs r1, #160
	b .L_02001196_0
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x0101
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffe8
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
.L_02001196_0:
	movs r2, #152
	movs r3, #1
	lsls r0, r0, #16
	lsls r1, r1, #14
	lsls r2, r2, #16
	bl 0x0200b7b0
	movs r0, #22
	ldr r1, [pc, #1016]
	ldr r2, [pc, #1020]
	bl 0x0200b6f8
	movs r1, #136
	movs r0, #22
	lsls r1, r1, #1
	movs r2, #128
	bl 0x0200b718
	movs r1, #132
	movs r0, #22
	lsls r1, r1, #1
	movs r2, #152
	bl 0x0200b718
	movs r1, #140
	movs r0, #22
	lsls r1, r1, #1
	movs r2, #168
	bl 0x0200b718
	bl 0x0200b7b8
	movs r1, #160
	movs r2, #20
	lsls r1, r1, #7
	movs r0, #22
	bl 0x0200b788
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #23
	bl 0x0200b778
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #3
	movs r0, #22
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #22
	movs r1, #0
	bl 0x0200b778
	movs r0, #148
	movs r1, #160
	movs r2, #216
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #14
	lsls r2, r2, #16
	bl 0x0200b7b0
	movs r1, #152
	movs r0, #22
	lsls r1, r1, #1
	movs r2, #176
	bl 0x0200b718
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #22
	bl 0x0200b788
	bl 0x0200b7b8
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #0
	movs r1, #1
	bl 0x0200b740
	movs r0, #1
	movs r1, #1
	bl 0x0200b740
	movs r0, #2
	movs r1, #1
	bl 0x0200b740
	movs r1, #1
	movs r0, #3
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #3
	movs r0, #22
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #22
	bl 0x0200b778
	movs r0, #10
	bl 0x0200b6d0
	movs r0, #0
	ldr r1, [pc, #796]
	movs r2, #0
	bl 0x0200b798
	movs r0, #1
	ldr r1, [pc, #788]
	movs r2, #0
	bl 0x0200b798
	movs r0, #3
	ldr r1, [pc, #776]
	movs r2, #0
	bl 0x0200b798
	movs r2, #70
	movs r0, #2
	ldr r1, [pc, #764]
	bl 0x0200b798
	movs r1, #1
	movs r0, #22
	bl 0x0200b748
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #22
	bl 0x0200b778
	movs r0, #10
	bl 0x0200b6d0
	movs r0, #0
	ldr r1, [pc, #732]
	movs r2, #0
	bl 0x0200b798
	movs r0, #1
	ldr r1, [pc, #724]
	movs r2, #0
	bl 0x0200b798
	movs r0, #2
	ldr r1, [pc, #712]
	movs r2, #0
	bl 0x0200b798
	movs r2, #70
	movs r0, #3
	ldr r1, [pc, #700]
	bl 0x0200b798
	movs r1, #3
	movs r0, #22
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #22
	bl 0x0200b778
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #23
	bl 0x0200b778
	movs r0, #10
	bl 0x0200b6d0
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #22
	bl 0x0200b788
	movs r0, #40
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #22
	bl 0x0200b778
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #23
	bl 0x0200b778
	movs r0, #10
	bl 0x0200b6d0
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #22
	bl 0x0200b788
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #22
	bl 0x0200b778
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #1
	bl 0x0200b6f0
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r1, #164
	strb r3, [r0]
	lsls r1, r1, #1
	movs r0, #1
	movs r2, #224
	bl 0x0200b710
	movs r1, #172
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #224
	bl 0x0200b710
	movs r1, #172
	lsls r1, r1, #1
	movs r2, #232
	movs r0, #2
	bl 0x0200b710
	movs r0, #1
	bl 0x0200b720
	movs r0, #1
	bl 0x0200b6f0
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	movs r1, #192
	strb r3, [r0]
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b788
	movs r0, #0
	bl 0x0200b720
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x0200b788
	movs r0, #2
	bl 0x0200b720
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #2
	bl 0x0200b788
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #168
	movs r2, #200
	movs r0, #23
.L_0200145a:
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200b728
	movs r0, #23
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #208
	movs r2, #200
	lsls r2, r2, #16
	movs r0, #23
	lsls r1, r1, #15
	bl 0x0200b728
	movs r1, #1
	movs r0, #22
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #22
	movs r1, #0
	bl 0x0200b780
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200b7a8
	movs r0, #240
	movs r2, #168
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #15
	ldr r1, [pc, #364]
	bl 0x0200b7b0
	bl 0x0200b7b8
	movs r1, #3
	movs r0, #23
	bl 0x0200b738
	movs r0, #10
	bl 0x0200b6d0
	movs r1, #208
	movs r2, #20
	movs r0, #23
	lsls r1, r1, #8
	bl 0x0200b788
	movs r1, #3
	movs r0, #23
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #23
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #20
	lsls r1, r1, #6
	movs r2, #70
	bl 0x0200b788
	movs r1, #20
	movs r2, #0
	movs r0, #5
	bl 0x0200b750
	movs r0, #50
	bl 0x0200b6d0
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r2, #20
	movs r0, #20
	lsls r1, r1, #6
	bl 0x0200b788
	movs r1, #4
	movs r0, #5
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #128
	movs r0, #23
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200b798
	movs r0, #23
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r0, #5
	ldr r1, [pc, #200]
	movs r2, #60
	bl 0x0200b798
	movs r1, #208
	movs r2, #20
	movs r0, #21
	lsls r1, r1, #8
	bl 0x0200b788
	movs r0, #21
	movs r1, #1
	bl 0x0200b748
	movs r1, #3
	movs r0, #23
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #23
	movs r1, #0
	bl 0x0200b780
	movs r1, #4
	movs r0, #23
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #23
	movs r1, #0
	bl 0x0200b780
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #21
	bl 0x0200b7a0
	movs r0, #60
	bl 0x0200b6d0
	movs r1, #4
	movs r0, #21
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200b780
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #5
	bl 0x0200b7a0
	movs r0, #60
	bl 0x0200b6d0
	movs r2, #40
	movs r0, #5
	movs r1, #0
	bl 0x0200b780
	movs r1, #3
	movs r0, #21
	bl 0x0200b738
	movs r0, #60
	bl 0x0200b6d0
	movs r1, #160
	movs r2, #10
	movs r0, #5
	lsls r1, r1, #8
	bl 0x0200b788
	movs r1, #3
	movs r0, #20
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #20
	movs r1, #0
	b .L_0200145a_0
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x0107
	.2byte 0x0000
	.4byte 0x00000105
	.4byte 0xffe80000
.L_0200145a_0:
	movs r2, #30
	bl 0x0200b780
	movs r0, #5
	ldr r1, [pc, #908]
	ldr r2, [pc, #912]
	bl 0x0200b6f8
	movs r0, #20
	ldr r1, [pc, #900]
	ldr r2, [pc, #900]
	bl 0x0200b6f8
	movs r0, #5
	movs r1, #128
	movs r2, #144
	bl 0x0200b710
	movs r0, #20
	movs r1, #120
	movs r2, #136
	bl 0x0200b718
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #20
	bl 0x0200b788
	movs r0, #5
	bl 0x0200b720
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl 0x0200b788
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #20
	movs r0, #21
	bl 0x0200b788
	movs r0, #21
	bl 0x0200b6f0
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #88
	movs r2, #152
	movs r0, #21
	bl 0x0200b718
	movs r0, #21
	bl 0x0200b6f0
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #176
	orrs r3, r6
	strb r3, [r0]
	movs r2, #20
	movs r0, #23
	lsls r1, r1, #8
	bl 0x0200b788
	movs r0, #23
	movs r1, #3
	bl 0x0200b730
	movs r1, #3
	movs r0, #21
	bl 0x0200b738
	movs r0, #40
	bl 0x0200b6d0
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #10
	movs r0, #23
	bl 0x0200b6f8
	movs r0, #23
	bl 0x0200b6f0
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r0, #40]
	movs r0, #152
	bl 0x0200b840
	movs r0, #23
	bl 0x0200b6f0
	adds r0, #85
	ldrb r2, [r0]
	movs r3, #126
	ands r3, r2
	strb r3, [r0]
	movs r0, #23
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r0, #17
	bl 0x0200b6f0
	movs r3, #4
	adds r0, #85
	strb r3, [r0]
	movs r2, #168
	movs r1, #104
	movs r0, #23
	bl 0x0200b708
	movs r0, #23
	bl 0x0200b6f0
	movs r1, #1
	bl 0x0200b690
	movs r0, #23
	bl 0x0200b6f0
	movs r3, #3
	adds r0, #85
	strb r3, [r0]
	movs r1, #0
	movs r0, #23
	movs r2, #30
	bl 0x0200b788
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl 0x0200b788
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200b780
	movs r0, #5
	movs r1, #2
	bl 0x0200b748
	movs r1, #4
	movs r0, #5
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r2, #20
	movs r0, #23
	movs r1, #0
	bl 0x0200b780
	movs r0, #17
	movs r1, #0
	bl 0x0200b790
	movs r0, #18
	movs r1, #0
	bl 0x0200b790
	bl 0x0200b208
	movs r0, #17
	movs r1, #1
	bl 0x0200b790
	movs r0, #18
	movs r1, #1
	bl 0x0200b790
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r0, #152
	movs r1, #128
	movs r2, #216
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #14
	lsls r2, r2, #16
	bl 0x0200b7b0
	bl 0x0200b7b8
	movs r1, #136
	movs r2, #140
	movs r0, #20
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200b728
	movs r0, #20
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r2, #0
	movs r0, #20
	movs r1, #0
	bl 0x0200b728
	movs r1, #1
	movs r0, #1
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #192
	movs r2, #192
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200b6f8
	movs r1, #156
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #216
	bl 0x0200b718
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200b788
	movs r0, #22
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #156
	movs r0, #22
	lsls r1, r1, #1
	movs r2, #184
	bl 0x0200b718
	movs r1, #192
	movs r0, #22
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200b788
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200b798
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #1
	bl 0x0200b6f8
	movs r0, #1
	bl 0x0200b6f0
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #164
	ands r5, r3
	movs r2, #224
	lsls r1, r1, #1
	strb r5, [r0]
	movs r0, #1
	bl 0x0200b718
	movs r0, #1
	bl 0x0200b6d0
	movs r0, #1
	bl 0x0200b6f0
	adds r0, #90
	ldrb r3, [r0]
	orrs r6, r3
	strb r6, [r0]
	movs r1, #1
	movs r0, #0
	bl 0x0200b740
	movs r0, #1
	movs r1, #1
	bl 0x0200b740
	movs r0, #2
	movs r1, #1
	bl 0x0200b740
	movs r1, #1
	movs r0, #3
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #1
	movs r0, #22
	bl 0x0200b748
	movs r0, #10
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #22
	movs r1, #0
	bl 0x0200b780
	movs r1, #2
	movs r0, #1
	bl 0x0200b748
	movs r0, #10
	bl 0x0200b6d0
	movs r1, #4
	movs r0, #1
	bl 0x0200b738
	movs r0, #10
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200b780
	movs r1, #1
	movs r0, #2
	bl 0x0200b748
	movs r0, #10
	bl 0x0200b6d0
	movs r1, #3
	movs r0, #2
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #2
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r0, #22
	ldr r1, [pc, #80]
	movs r2, #60
	bl 0x0200b798
	movs r1, #0
	movs r0, #22
	bl 0x0200b770
	movs r0, #0
	movs r1, #0
	bl 0x0200b6e8
	cmp r0, #0
	bne .L_0200145a_1
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #4
	movs r0, #22
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #22
	movs r1, #0
	bl 0x0200b780
	ldr r3, [pc, #28]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200145a_2
	.4byte 0x0000b333
	.4byte 0x00005999
	.4byte 0x00000101
	.4byte 0x03001ebc
.L_0200145a_1:
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #4
	movs r0, #22
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	ldr r3, [pc, #256]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #22
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
.L_0200145a_2:
	movs r1, #2
	movs r0, #22
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #22
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #164
	movs r2, #200
	movs r0, #22
	lsls r1, r1, #1
	bl 0x0200b718
	movs r0, #22
	movs r1, #2
	bl 0x0200b748
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #20
	movs r0, #22
	bl 0x0200b788
	movs r0, #22
	bl 0x0200b6f0
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #168
	strb r3, [r0]
	lsls r1, r1, #1
	movs r2, #208
	movs r0, #22
	bl 0x0200b718
	movs r0, #1
	bl 0x0200b6d0
	movs r0, #22
	bl 0x0200b6f0
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r1, #129
	strb r3, [r0]
	lsls r1, r1, #1
	movs r0, #22
	movs r2, #60
	bl 0x0200b798
	movs r0, #3
	ldr r1, [pc, #116]
	movs r2, #60
	bl 0x0200b798
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r0, #22
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #160
	movs r2, #20
	movs r0, #22
	lsls r1, r1, #7
	bl 0x0200b788
	movs r1, #2
	movs r0, #22
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #22
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #168
	movs r2, #216
	movs r0, #22
	lsls r1, r1, #1
	bl 0x0200b718
	ldr r0, [pc, #44]
	movs r1, #2
	bl 0x0200b7d8
	ldr r3, [pc, #40]
	ldr r2, [pc, #40]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	movs r0, #36
	movs r1, #2
	bl 0x0200b7d0
	bl 0x0200b6e0
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000101
	.4byte 0x0000003a
	.4byte 0x02000240
	.4byte 0x0000022b
	.global Func_02001af0
	.thumb_func
Func_02001af0:
	push {r5, r6, r7, lr}
	sub sp, #8
	bl 0x0200b6d8
	movs r0, #9
	bl 0x0200b6f0
	movs r6, #0
	adds r0, #85
	movs r1, #172
	movs r2, #224
	strb r6, [r0]
	lsls r1, r1, #17
	movs r0, #0
	lsls r2, r2, #16
	bl 0x0200b728
	movs r1, #164
	movs r2, #224
	movs r0, #1
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200b728
	movs r1, #172
	movs r2, #232
	movs r0, #2
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200b728
	movs r1, #164
	movs r2, #232
	movs r0, #3
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200b728
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #168
	movs r2, #176
	lsls r2, r2, #16
	movs r0, #22
	lsls r1, r1, #17
	bl 0x0200b728
	movs r1, #9
	movs r0, #22
	bl 0x0200b730
	movs r0, #22
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r0, #168
	movs r1, #1
	movs r2, #208
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #0
	lsls r0, r0, #17
	bl 0x0200b7b0
	movs r0, #1
	bl 0x0200b630
	bl 0x0200b678
	movs r0, #1
	bl 0x0200b630
	ldr r7, [pc, #896]
	movs r2, #224
	ldr r3, [r7]
	lsls r2, r2, #1
	movs r5, #128
	adds r3, r3, r2
	lsls r5, r5, #1
	str r5, [r3]
	bl 0x0200b7f8
	bl 0x0200b800
	movs r0, #60
	bl 0x0200b6d0
	movs r1, #2
	movs r0, #1
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #0
	movs r2, #20
	movs r0, #1
	bl 0x0200b788
	ldr r0, [pc, #848]
	bl 0x0200b768
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #128
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200b788
	movs r1, #3
	movs r0, #0
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #2
	movs r0, #22
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #22
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200b788
	movs r0, #2
	ldr r1, [pc, #760]
	movs r2, #60
	bl 0x0200b798
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x0200b780
	movs r1, #2
	movs r0, #22
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #22
	movs r1, #0
	movs r2, #30
	bl 0x0200b780
	movs r1, #156
	movs r2, #224
	lsls r1, r1, #17
	lsls r2, r2, #15
	movs r0, #24
	bl 0x0200b728
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200b840
	movs r0, #24
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r0, #3
	adds r1, r5, #0
	movs r2, #60
	bl 0x0200b798
	movs r2, #30
	movs r1, #0
	movs r0, #3
	bl 0x0200b780
	movs r0, #29
	bl 0x0200b840
	ldr r1, [pc, #664]
	ldr r0, [pc, #664]
	bl 0x0200b7a8
	bl 0x0200b7c0
	adds r0, #85
	strb r6, [r0]
	movs r1, #1
	movs r0, #168
	movs r2, #168
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #17
	bl 0x0200b7b0
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #24
	ldr r1, [pc, #624]
	ldr r2, [pc, #628]
	bl 0x0200b6f8
	movs r1, #172
	movs r0, #24
	lsls r1, r1, #1
	movs r2, #136
	bl 0x0200b718
	movs r1, #160
	movs r0, #24
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200b788
	bl 0x0200b7b8
	movs r0, #3
	movs r1, #0
	movs r2, #30
	bl 0x0200b780
	movs r0, #168
	movs r1, #1
	movs r2, #184
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200b7b0
	movs r1, #172
	movs r0, #24
	lsls r1, r1, #1
	movs r2, #160
	bl 0x0200b718
	movs r1, #164
	movs r0, #24
	lsls r1, r1, #1
	movs r2, #168
	bl 0x0200b718
	movs r1, #156
	movs r0, #24
	lsls r1, r1, #1
	movs r2, #176
	bl 0x0200b718
	movs r1, #192
	movs r2, #20
	movs r0, #24
	lsls r1, r1, #6
	bl 0x0200b788
	bl 0x0200b7b8
	movs r1, #3
	movs r0, #24
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #24
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r1, #128
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200b788
	movs r1, #2
	movs r0, #3
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200b780
	movs r1, #4
	movs r0, #24
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #24
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200b788
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #224
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #60
	bl 0x0200b788
	movs r1, #176
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200b788
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200b788
	movs r2, #20
	movs r0, #24
	movs r1, #0
	bl 0x0200b780
	movs r1, #4
	movs r0, #3
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200b780
	movs r0, #24
	movs r1, #2
	bl 0x0200b740
	movs r0, #24
	adds r1, r5, #0
	movs r2, #60
	bl 0x0200b798
	movs r1, #192
	movs r0, #24
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200b788
	movs r0, #24
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r2, #60
	movs r0, #3
	ldr r1, [pc, #220]
	bl 0x0200b798
	movs r1, #4
	movs r0, #24
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #24
	movs r1, #0
	bl 0x0200b780
	movs r1, #2
	movs r0, #3
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200b780
	movs r1, #3
	movs r0, #24
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #24
	movs r1, #0
	bl 0x0200b780
	movs r1, #1
	movs r0, #3
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #156
	movs r0, #24
	lsls r1, r1, #1
	movs r2, #184
	bl 0x0200b718
	movs r1, #192
	movs r0, #24
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200b788
	movs r2, #20
	movs r0, #24
	movs r1, #0
	bl 0x0200b780
	movs r1, #2
	movs r0, #1
	bl 0x0200b748
	movs r0, #10
	bl 0x0200b6d0
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200b788
	movs r1, #0
	movs r0, #1
	bl 0x0200b770
	movs r0, #0
	movs r1, #0
	bl 0x0200b6e8
	cmp r0, #0
	bne .L_02001af0_0
	movs r0, #1
	movs r1, #2
	bl 0x0200b748
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200b780
	ldr r2, [r7]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02001af0_1
	.4byte 0x03001ebc
	.4byte 0x000015d4
	.4byte 0x00000101
	.4byte 0x00001999
	.4byte 0x0000cccc
	.4byte 0x00006666
.L_02001af0_0:
	ldr r2, [r7]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #1
	movs r1, #1
	bl 0x0200b748
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
.L_02001af0_1:
	movs r0, #2
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #192
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200b788
	movs r1, #3
	movs r0, #24
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #24
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #208
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200b788
	movs r0, #24
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #192
	movs r2, #20
	movs r0, #24
	lsls r1, r1, #6
	bl 0x0200b788
	movs r1, #4
	movs r0, #24
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #24
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r0, #3
	ldr r1, [pc, #752]
	movs r2, #60
	bl 0x0200b798
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200b780
	movs r1, #4
	movs r0, #24
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #24
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #176
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200b788
	movs r2, #20
	movs r1, #0
	movs r0, #24
	bl 0x0200b780
	movs r0, #10
	bl 0x0200b6d0
	movs r1, #2
	movs r0, #24
	bl 0x0200b748
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #24
	bl 0x0200b778
	movs r0, #10
	bl 0x0200b6d0
	movs r1, #2
	movs r0, #1
	bl 0x0200b748
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #1
	bl 0x0200b778
	movs r0, #10
	bl 0x0200b6d0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #24
	bl 0x0200b788
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #4
	movs r0, #24
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #24
	bl 0x0200b778
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #2
	movs r0, #24
	bl 0x0200b748
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #24
	bl 0x0200b778
	movs r0, #10
	bl 0x0200b6d0
	movs r1, #128
	movs r2, #60
	movs r0, #2
	lsls r1, r1, #1
	bl 0x0200b798
	movs r1, #0
	movs r0, #2
	bl 0x0200b778
	movs r0, #10
	bl 0x0200b6d0
	movs r1, #3
	movs r0, #24
	bl 0x0200b738
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #24
	bl 0x0200b778
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #2
	movs r0, #24
	bl 0x0200b748
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #24
	bl 0x0200b778
	movs r0, #10
	bl 0x0200b6d0
	movs r1, #128
	lsls r1, r1, #5
	movs r2, #0
	movs r0, #24
	bl 0x0200b788
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #132
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #24
	bl 0x0200b798
	movs r0, #10
	bl 0x0200b6d0
	movs r1, #2
	movs r0, #22
	bl 0x0200b748
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #8
	movs r0, #22
	bl 0x0200b730
	movs r0, #45
	bl 0x0200b6d0
	movs r1, #1
	movs r0, #22
	bl 0x0200b730
	movs r0, #22
	bl 0x0200b6f0
	movs r1, #1
	bl 0x0200b690
	movs r0, #40
	bl 0x0200b6d0
	movs r1, #129
	movs r2, #60
	movs r0, #2
	lsls r1, r1, #1
	bl 0x0200b798
	movs r1, #0
	movs r0, #2
	bl 0x0200b778
	movs r0, #10
	bl 0x0200b6d0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #22
	bl 0x0200b788
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #4
	movs r0, #22
	bl 0x0200b738
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #22
	bl 0x0200b778
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #24
	bl 0x0200b788
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #24
	bl 0x0200b778
	movs r0, #10
	bl 0x0200b6d0
	movs r1, #2
	movs r0, #3
	bl 0x0200b748
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #3
	bl 0x0200b778
	movs r0, #10
	bl 0x0200b6d0
	movs r2, #0
	movs r1, #22
	movs r0, #24
	bl 0x0200b750
	movs r0, #35
	bl 0x0200b6d0
	movs r1, #3
	movs r0, #22
	bl 0x0200b738
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #3
	movs r0, #24
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #164
	movs r0, #24
	lsls r1, r1, #1
	movs r2, #176
	bl 0x0200b718
	movs r1, #192
	movs r2, #20
	movs r0, #24
	lsls r1, r1, #6
	bl 0x0200b788
	movs r0, #24
	movs r1, #5
	bl 0x0200b730
	movs r1, #7
	movs r0, #22
	bl 0x0200b730
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #1
	movs r0, #1
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #1
	bl 0x0200b778
	movs r0, #10
	bl 0x0200b6d0
	movs r2, #60
	movs r0, #24
	ldr r1, [pc, #124]
	bl 0x0200b798
	movs r1, #0
	movs r0, #24
	bl 0x0200b770
	movs r0, #30
	bl 0x0200b6d0
	movs r0, #1
	movs r1, #2
	bl 0x0200b740
	movs r0, #2
	movs r1, #2
	bl 0x0200b740
	movs r0, #3
	movs r1, #2
	bl 0x0200b748
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #3
	movs r2, #0
	bl 0x0200b788
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #0
	movs r1, #0
	bl 0x0200b6e8
	cmp r0, #0
	bne .L_02001af0_2
	movs r0, #30
	bl 0x0200b6d0
	movs r0, #24
	movs r1, #0
	bl 0x0200b778
	ldr r3, [pc, #20]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02001af0_3
	.4byte 0x00000101
	.4byte 0x03001ebc
.L_02001af0_2:
	movs r0, #30
	bl 0x0200b6d0
	ldr r3, [pc, #1004]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #24
	movs r1, #0
	bl 0x0200b778
.L_02001af0_3:
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #4
	movs r0, #24
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #3
	bl 0x0200b788
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #24
	movs r1, #0
	bl 0x0200b778
	movs r1, #1
	movs r0, #22
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r1, #0
	movs r0, #22
	bl 0x0200b780
	bl 0x0200af18
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #0
	movs r1, #2
	bl 0x0200b740
	movs r0, #1
	movs r1, #2
	bl 0x0200b740
	movs r0, #2
	movs r1, #2
	bl 0x0200b740
	movs r0, #3
	movs r1, #2
	bl 0x0200b748
	movs r0, #0
	movs r1, #0
	movs r2, #5
	bl 0x0200b788
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #5
	bl 0x0200b788
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #5
	bl 0x0200b788
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #5
	bl 0x0200b788
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #5
	bl 0x0200b788
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #5
	bl 0x0200b788
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #5
	bl 0x0200b788
	movs r1, #0
	movs r2, #5
	movs r0, #3
	bl 0x0200b788
	bl 0x0200b7c0
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r1, #1
	movs r0, #140
	movs r2, #232
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #17
	negs r1, r1
	bl 0x0200b7b0
	bl 0x0200b7b8
	movs r0, #22
	movs r1, #15
	bl 0x0200b758
	movs r0, #24
	movs r1, #15
	bl 0x0200b758
	movs r1, #240
	movs r2, #208
	movs r0, #22
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200b728
	movs r1, #232
	movs r2, #208
	movs r0, #24
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200b728
	movs r1, #160
	movs r0, #22
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #24
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200b788
	bl 0x0200b060
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200b788
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #15
	bl 0x0200b788
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200b788
	movs r1, #164
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #208
	bl 0x0200b710
	movs r1, #168
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #224
	bl 0x0200b710
	movs r1, #156
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #216
	bl 0x0200b718
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b788
	movs r0, #2
	bl 0x0200b720
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #2
	bl 0x0200b788
	movs r0, #0
	bl 0x0200b720
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #30
	bl 0x0200b788
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200b780
	movs r1, #2
	movs r0, #3
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200b788
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #128
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200b788
	movs r0, #2
	movs r1, #1
	bl 0x0200b748
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200b788
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x0200b780
	movs r1, #4
	movs r0, #3
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200b788
	movs r2, #20
	movs r0, #24
	movs r1, #0
	bl 0x0200b780
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200b7a8
	movs r0, #152
	movs r1, #1
	movs r2, #216
	negs r1, r1
	lsls r0, r0, #16
	lsls r2, r2, #16
	movs r3, #1
	bl 0x0200b7b0
	ldr r6, [pc, #344]
	movs r2, #178
	ldr r3, [r6]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #224
	lsls r2, r2, #18
	str r2, [r3, #12]
	bl 0x0200b678
	movs r0, #1
	bl 0x0200b630
	movs r0, #9
	bl 0x0200b6f0
	movs r1, #208
	adds r0, #85
	movs r2, #132
	lsls r1, r1, #15
	lsls r2, r2, #17
	strb r5, [r0]
	movs r0, #9
	bl 0x0200b728
	movs r0, #9
	bl 0x0200b6f0
	ldr r5, [pc, #292]
	str r5, [r0, #12]
	movs r0, #9
	bl 0x0200b6f0
	movs r3, #5
	movs r2, #4
	str r5, [r0, #60]
	movs r1, #74
	str r3, [sp, #0]
	movs r0, #29
	movs r3, #74
	str r2, [sp, #4]
	bl 0x0200b680
	movs r0, #17
	movs r1, #0
	bl 0x0200b790
	movs r0, #18
	movs r1, #0
	bl 0x0200b790
	bl 0x0200b324
	movs r0, #17
	movs r1, #1
	bl 0x0200b790
	movs r1, #1
	movs r0, #18
	bl 0x0200b790
	bl 0x0200b7b8
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #128
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200b788
	movs r0, #24
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r0, #140
	movs r1, #1
	movs r2, #216
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200b7b0
	bl 0x0200b7b8
	movs r1, #192
	movs r2, #20
	movs r0, #24
	lsls r1, r1, #6
	bl 0x0200b788
	movs r0, #24
	movs r1, #5
	bl 0x0200b730
	movs r2, #20
	movs r0, #24
	movs r1, #0
	bl 0x0200b780
	movs r1, #2
	movs r0, #2
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200b788
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x0200b780
	movs r1, #1
	movs r0, #1
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r0, #24
	ldr r1, [pc, #84]
	movs r2, #60
	bl 0x0200b798
	movs r1, #0
	movs r0, #24
	bl 0x0200b770
	movs r0, #0
	movs r1, #0
	bl 0x0200b6e8
	cmp r0, #0
	bne .L_02001af0_4
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #2
	movs r0, #22
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #22
	movs r1, #0
	bl 0x0200b780
	ldr r2, [r6, #76]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02001af0_5
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x03001e70
	.4byte 0xffe00000
	.4byte 0x00000101
.L_02001af0_4:
	ldr r2, [r6, #76]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #22
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
.L_02001af0_5:
	movs r1, #2
	movs r0, #24
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #24
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r0, #0
	ldr r1, [pc, #840]
	movs r2, #0
	bl 0x0200b798
	movs r0, #1
	ldr r1, [pc, #832]
	movs r2, #0
	bl 0x0200b798
	movs r0, #2
	ldr r1, [pc, #820]
	movs r2, #0
	bl 0x0200b798
	movs r0, #3
	ldr r1, [pc, #812]
	movs r2, #60
	bl 0x0200b798
	movs r1, #0
	movs r0, #24
	bl 0x0200b770
	movs r0, #0
	movs r1, #0
	bl 0x0200b6e8
	cmp r0, #0
	bne .L_02001af0_6
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #2
	movs r0, #24
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #24
	movs r1, #0
	bl 0x0200b780
	ldr r3, [pc, #756]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02001af0_7
.L_02001af0_6:
	ldr r3, [pc, #740]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #1
	movs r0, #24
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #24
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
.L_02001af0_7:
	movs r0, #20
	bl 0x0200b6d0
	bl 0x0200af18
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #128
	movs r1, #1
	movs r2, #200
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #16
	negs r1, r1
	bl 0x0200b7b0
	bl 0x0200b7b8
	movs r0, #22
	movs r1, #1
	bl 0x0200b730
	movs r0, #24
	movs r1, #1
	bl 0x0200b730
	movs r0, #22
	movs r1, #15
	bl 0x0200b758
	movs r0, #24
	movs r1, #15
	bl 0x0200b758
	movs r1, #240
	movs r2, #152
	movs r0, #22
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl 0x0200b728
	movs r1, #224
	movs r2, #160
	movs r0, #24
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl 0x0200b728
	movs r1, #160
	movs r0, #22
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #24
	bl 0x0200b788
	bl 0x0200b060
	movs r0, #30
	bl 0x0200b6d0
	movs r0, #24
	movs r1, #0
	movs r2, #20
	bl 0x0200b788
	movs r1, #0
	movs r0, #24
	bl 0x0200b770
	movs r0, #0
	movs r1, #0
	bl 0x0200b6e8
	cmp r0, #0
	bne .L_02001af0_8
	movs r1, #3
	movs r0, #24
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #24
	movs r1, #0
	bl 0x0200b780
	ldr r3, [pc, #508]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02001af0_9
.L_02001af0_8:
	ldr r3, [pc, #488]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r1, #4
	movs r0, #24
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #24
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
.L_02001af0_9:
	movs r2, #20
	movs r0, #24
	movs r1, #0
	bl 0x0200b780
	movs r1, #1
	movs r0, #22
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #22
	movs r1, #0
	movs r2, #20
	bl 0x0200b788
	movs r1, #0
	movs r2, #20
	movs r0, #22
	bl 0x0200b780
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #24
	movs r1, #0
	bl 0x0200b788
	movs r0, #17
	movs r1, #0
	bl 0x0200b790
	movs r0, #18
	movs r1, #0
	bl 0x0200b790
	bl 0x0200b208
	movs r0, #17
	movs r1, #1
	bl 0x0200b790
	movs r1, #1
	movs r0, #18
	bl 0x0200b790
	movs r0, #17
	bl 0x0200b840
	movs r0, #168
	movs r1, #1
	movs r2, #216
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #17
	negs r1, r1
	bl 0x0200b7b0
	bl 0x0200b7b8
	movs r1, #4
	movs r0, #2
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	bl 0x0200b838
	movs r0, #2
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r2, #22
	movs r0, #1
	lsls r1, r1, #6
	bl 0x0200b788
	movs r1, #2
	movs r0, #1
	bl 0x0200b748
	movs r0, #10
	bl 0x0200b6d0
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r0, #3
	ldr r1, [pc, #252]
	movs r2, #60
	bl 0x0200b798
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200b788
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #128
	movs r2, #20
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200b788
	movs r1, #16
	movs r0, #3
	bl 0x0200b730
	movs r0, #3
	bl 0x0200b6f0
	ldr r3, [pc, #188]
	str r3, [r0, #24]
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #40
	movs r0, #3
	movs r1, #0
	bl 0x0200b780
	movs r1, #2
	movs r0, #2
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #160
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200b788
	movs r1, #4
	movs r0, #2
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #0
	movs r0, #2
	bl 0x0200b770
	movs r1, #1
	movs r0, #3
	bl 0x0200b730
	movs r0, #3
	bl 0x0200b6f0
	movs r3, #128
	lsls r3, r3, #9
	movs r1, #128
	str r3, [r0, #24]
	lsls r1, r1, #7
	movs r0, #0
	movs r2, #0
	bl 0x0200b788
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200b788
	movs r0, #0
	movs r1, #0
	bl 0x0200b6e8
	cmp r0, #0
	bne .L_02001af0_10
	movs r1, #3
	movs r0, #2
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x0200b780
	ldr r3, [pc, #20]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02001af0_11
	.2byte 0x0000
	.4byte 0x00000101
	.4byte 0x03001ebc
	.4byte 0x00000105
	.4byte 0xffff0000
.L_02001af0_10:
	ldr r3, [pc, #856]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r1, #2
	movs r0, #2
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #2
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
.L_02001af0_11:
	movs r1, #192
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #7
	bl 0x0200b788
	movs r1, #3
	movs r0, #2
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #2
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #224
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #30
	bl 0x0200b788
	movs r1, #128
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #6
	bl 0x0200b788
	movs r1, #1
	movs r0, #1
	bl 0x0200b748
	movs r0, #10
	bl 0x0200b6d0
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #160
	movs r2, #20
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200b788
	movs r1, #3
	movs r0, #3
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200b780
	movs r0, #3
	movs r1, #1
	bl 0x0200b748
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r0, #0
	ldr r1, [pc, #680]
	movs r2, #0
	bl 0x0200b798
	movs r0, #1
	ldr r1, [pc, #672]
	movs r2, #0
	bl 0x0200b798
	movs r0, #2
	ldr r1, [pc, #660]
	movs r2, #60
	bl 0x0200b798
	movs r0, #3
	ldr r1, [pc, #656]
	ldr r2, [pc, #656]
	bl 0x0200b6f8
	movs r0, #1
	ldr r1, [pc, #644]
	ldr r2, [pc, #648]
	bl 0x0200b6f8
	movs r1, #164
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #216
	bl 0x0200b718
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #2
	bl 0x0200b788
	movs r0, #1
	bl 0x0200b6f0
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #156
	strb r3, [r0]
	lsls r1, r1, #1
	movs r0, #1
	movs r2, #200
	bl 0x0200b710
	movs r1, #140
	lsls r1, r1, #1
	movs r2, #216
	movs r0, #3
	bl 0x0200b710
	movs r0, #1
	bl 0x0200b720
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #1
	bl 0x0200b788
	movs r0, #3
	bl 0x0200b720
	movs r1, #2
	movs r0, #3
	bl 0x0200b748
	movs r0, #30
	bl 0x0200b6d0
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200b788
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200b780
	movs r1, #1
	movs r0, #1
	bl 0x0200b748
	movs r0, #20
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x0200b780
	movs r0, #3
	movs r1, #3
	bl 0x0200b738
	movs r0, #0
	movs r1, #2
	bl 0x0200b740
	movs r0, #2
	movs r1, #2
	bl 0x0200b740
	movs r0, #1
	movs r1, #2
	bl 0x0200b740
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200b7a0
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200b7a0
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #2
	bl 0x0200b7a0
	movs r0, #60
	bl 0x0200b6d0
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x0200b780
	movs r1, #4
	movs r0, #3
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #224
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200b788
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200b788
	movs r1, #4
	movs r0, #3
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #3
	movs r1, #0
	movs r2, #30
	bl 0x0200b780
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200b788
	movs r1, #160
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200b788
	movs r0, #0
	movs r1, #3
	bl 0x0200b730
	movs r0, #1
	movs r1, #3
	bl 0x0200b730
	movs r1, #3
	movs r0, #2
	bl 0x0200b738
	movs r0, #30
	bl 0x0200b6d0
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b788
	movs r1, #192
	movs r2, #40
	movs r0, #2
	lsls r1, r1, #7
	bl 0x0200b788
	movs r0, #0
	movs r1, #3
	bl 0x0200b730
	movs r0, #1
	movs r1, #3
	bl 0x0200b730
	movs r1, #3
	movs r0, #2
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r1, #3
	movs r0, #3
	bl 0x0200b738
	movs r0, #20
	bl 0x0200b6d0
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200b780
	movs r1, #156
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #216
	bl 0x0200b710
	movs r1, #156
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #216
	bl 0x0200b710
	movs r1, #156
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #216
	bl 0x0200b710
	movs r1, #156
	lsls r1, r1, #1
	movs r2, #216
	movs r0, #3
	bl 0x0200b710
	movs r0, #0
	bl 0x0200b720
	movs r0, #1
	bl 0x0200b720
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x0200b728
	movs r0, #2
	bl 0x0200b720
	movs r1, #0
	movs r2, #0
	movs r0, #2
	bl 0x0200b728
	movs r0, #3
	bl 0x0200b720
	movs r1, #0
	movs r0, #3
	movs r2, #0
	bl 0x0200b728
	ldr r3, [pc, #84]
	movs r2, #178
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #19
	str r2, [r3, #12]
	bl 0x0200b678
	movs r0, #1
	bl 0x0200b630
	movs r3, #5
	movs r2, #4
	str r3, [sp, #0]
	movs r1, #70
	movs r3, #74
	movs r0, #4
	str r2, [sp, #4]
	bl 0x0200b680
	movs r0, #136
	lsls r0, r0, #4
	bl 0x0200b6b8
	ldr r0, [pc, #36]
	bl 0x0200b6b8
	bl 0x0200b6e0
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x00000101
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x03001e70
	.4byte 0x00000881
	.global Func_02002ddc
	.thumb_func
Func_02002ddc:
	push {r5, r6, lr}
	ldr r3, [pc, #196]
	ldr r5, [r3]
	movs r3, #178
	lsls r3, r3, #1
	sub sp, #8
	adds r5, r5, r3
	bl 0x0200b6d8
	movs r3, #224
	lsls r3, r3, #18
	str r3, [r5, #12]
	bl 0x0200b678
	movs r0, #1
	bl 0x0200b630
	movs r0, #9
	bl 0x0200b6f0
	movs r6, #0
	adds r0, #85
	movs r1, #208
	movs r2, #132
	lsls r2, r2, #17
	lsls r1, r1, #15
	strb r6, [r0]
	movs r0, #9
	bl 0x0200b728
	movs r0, #9
	bl 0x0200b6f0
	ldr r5, [pc, #136]
	str r5, [r0, #12]
	movs r0, #9
	bl 0x0200b6f0
	str r5, [r0, #60]
	bl 0x0200b7c0
	adds r0, #85
	strb r6, [r0]
	ldr r1, [pc, #120]
	ldr r0, [pc, #120]
	bl 0x0200b7a8
	movs r0, #128
	movs r1, #1
	movs r2, #184
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #16
	bl 0x0200b7b0
	bl 0x0200b7b8
	movs r0, #30
	bl 0x0200b6d0
	movs r3, #5
	movs r2, #4
	str r3, [sp, #0]
	movs r0, #29
	movs r3, #74
	movs r1, #74
	str r2, [sp, #4]
	bl 0x0200b680
	movs r0, #17
	movs r1, #0
	bl 0x0200b790
	movs r0, #18
	movs r1, #0
	bl 0x0200b790
	bl 0x0200b324
	movs r0, #17
	movs r1, #1
	bl 0x0200b790
	movs r1, #1
	movs r0, #18
	bl 0x0200b790
	movs r0, #20
	bl 0x0200b6d0
	ldr r0, [pc, #32]
	bl 0x0200b6b8
	bl 0x0200b6e0
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001e70
	.4byte 0xffe00000
	.4byte 0x00001999
	.4byte 0x0000cccc
	.4byte 0x00000251
	.global Func_02002eb8
	.thumb_func
Func_02002eb8:
	push {lr}
	bl 0x0200b6d8
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200b6f8
	movs r0, #0
	movs r1, #104
	movs r2, #152
	bl 0x0200b718
	movs r1, #128
	movs r2, #60
	movs r0, #0
	lsls r1, r1, #7
	bl 0x0200b788
	movs r0, #17
	movs r1, #0
	bl 0x0200b790
	movs r0, #18
	movs r1, #0
	bl 0x0200b790
	bl 0x0200b208
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl 0x0200b7b0
	movs r0, #1
	bl 0x0200b7c8
	bl 0x0200b6e0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002f18
	.thumb_func
Func_02002f18:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #22
	sub sp, #56
	bl 0x0200b6f0
	mov r9, r0
	movs r0, #24
	bl 0x0200b6f0
	mov r10, r0
	movs r0, #190
	bl 0x0200b840
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #1
	bl 0x0200b758
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #24
	bl 0x0200b758
	movs r0, #22
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r0, #24
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r3, #1
	add r7, sp, #16
	str r3, [r7]
	movs r3, #5
	str r3, [r7, #4]
	movs r3, #142
	lsls r3, r3, #1
	strh r3, [r7, #24]
	ldr r3, [pc, #220]
	str r3, [r7, #8]
	movs r3, #192
	lsls r3, r3, #10
	movs r2, #0
	str r3, [r7, #12]
	mov r8, r2
.L_02002f18_2:
	movs r0, #1
	bl 0x0200b6d0
	movs r6, #1
	mov r3, r8
	ands r6, r3
	cmp r6, #0
	beq .L_02002f18_0
	bl 0x0200b640
	lsls r3, r0, #1
	adds r3, r3, r0
	mov r2, r9
	lsls r3, r3, #3
	ldr r5, [r2, #8]
	lsrs r3, r3, #16
	lsls r3, r3, #16
	adds r5, r5, r3
	ldr r3, [pc, #172]
	adds r5, r5, r3
	bl 0x0200b640
	mov r2, r9
	lsls r0, r0, #5
	ldr r1, [r2, #12]
	lsrs r0, r0, #16
	lsls r0, r0, #16
	ldr r3, [pc, #160]
	adds r1, r1, r0
	adds r1, r1, r3
	movs r3, #128
	lsls r3, r3, #11
	ldr r2, [r2, #16]
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #216
	lsls r3, r3, #13
	str r3, [sp, #8]
	adds r0, r5, #0
	movs r3, #0
	str r7, [sp, #12]
	bl 0x0200813c
	b .L_02002f18_1
.L_02002f18_0:
	bl 0x0200b640
	lsls r3, r0, #1
	adds r3, r3, r0
	mov r2, r10
	lsls r3, r3, #3
	ldr r5, [r2, #8]
	lsrs r3, r3, #16
	lsls r3, r3, #16
	adds r5, r5, r3
	ldr r3, [pc, #100]
	adds r5, r5, r3
	bl 0x0200b640
	mov r2, r10
	lsls r0, r0, #5
	ldr r1, [r2, #12]
	lsrs r0, r0, #16
	lsls r0, r0, #16
	ldr r3, [pc, #84]
	adds r1, r1, r0
	adds r1, r1, r3
	movs r3, #128
	lsls r3, r3, #11
	ldr r2, [r2, #16]
	str r3, [sp, #0]
	movs r3, #216
	lsls r3, r3, #13
	str r3, [sp, #8]
	adds r0, r5, #0
	movs r3, #0
	str r6, [sp, #4]
	str r7, [sp, #12]
	bl 0x0200813c
.L_02002f18_1:
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #31
	bls .L_02002f18_2
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl 0x0200b728
	movs r0, #24
	movs r1, #0
	movs r2, #0
	bl 0x0200b728
	sub sp, #-56
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00006666
	.4byte 0xfff40000
	.4byte 0xfff00000
	.global Func_02003060
	.thumb_func
Func_02003060:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #22
	sub sp, #56
	bl 0x0200b6f0
	mov r9, r0
	movs r0, #24
	bl 0x0200b6f0
	mov r10, r0
	movs r0, #190
	bl 0x0200b840
	movs r0, #22
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r0, #24
	bl 0x0200b6f0
	movs r1, #0
	bl 0x0200b690
	movs r3, #1
	add r7, sp, #16
	str r3, [r7]
	movs r3, #5
	str r3, [r7, #4]
	movs r3, #142
	lsls r3, r3, #1
	strh r3, [r7, #24]
	ldr r3, [pc, #264]
	str r3, [r7, #8]
	movs r3, #192
	lsls r3, r3, #10
	movs r2, #0
	str r3, [r7, #12]
	mov r8, r2
.L_02003060_3:
	movs r0, #1
	bl 0x0200b6d0
	movs r6, #1
	mov r3, r8
	ands r6, r3
	cmp r6, #0
	beq .L_02003060_0
	bl 0x0200b640
	lsls r3, r0, #1
	adds r3, r3, r0
	mov r2, r9
	lsls r3, r3, #3
	ldr r5, [r2, #8]
	lsrs r3, r3, #16
	lsls r3, r3, #16
	adds r5, r5, r3
	ldr r3, [pc, #216]
	adds r5, r5, r3
	bl 0x0200b640
	mov r2, r9
	lsls r0, r0, #5
	ldr r1, [r2, #12]
	lsrs r0, r0, #16
	lsls r0, r0, #16
	movs r3, #128
	adds r1, r1, r0
	lsls r3, r3, #14
	adds r1, r1, r3
	ldr r3, [pc, #196]
	ldr r2, [r2, #16]
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #216
	lsls r3, r3, #13
	str r3, [sp, #8]
	adds r0, r5, #0
	movs r3, #0
	str r7, [sp, #12]
	bl 0x0200813c
	b .L_02003060_1
.L_02003060_0:
	bl 0x0200b640
	lsls r3, r0, #1
	adds r3, r3, r0
	mov r2, r10
	lsls r3, r3, #3
	ldr r5, [r2, #8]
	lsrs r3, r3, #16
	lsls r3, r3, #16
	adds r5, r5, r3
	ldr r3, [pc, #144]
	adds r5, r5, r3
	bl 0x0200b640
	mov r2, r10
	lsls r0, r0, #5
	ldr r1, [r2, #12]
	lsrs r0, r0, #16
	lsls r0, r0, #16
	movs r3, #128
	adds r1, r1, r0
	lsls r3, r3, #14
	adds r1, r1, r3
	ldr r3, [pc, #120]
	ldr r2, [r2, #16]
	str r3, [sp, #0]
	movs r3, #216
	lsls r3, r3, #13
	str r3, [sp, #8]
	adds r0, r5, #0
	movs r3, #0
	str r6, [sp, #4]
	str r7, [sp, #12]
	bl 0x0200813c
.L_02003060_1:
	mov r2, r8
	cmp r2, #20
	bne .L_02003060_2
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #1
	bl 0x0200b758
	movs r1, #128
	movs r0, #24
	lsls r1, r1, #1
	bl 0x0200b758
.L_02003060_2:
	movs r3, #1
	add r8, r3
	mov r2, r8
	cmp r2, #31
	bls .L_02003060_3
	movs r0, #22
	movs r1, #0
	bl 0x0200b758
	movs r1, #0
	movs r0, #24
	bl 0x0200b758
	movs r0, #22
	bl 0x0200b6f0
	movs r1, #1
	bl 0x0200b690
	movs r0, #24
	bl 0x0200b6f0
	movs r1, #1
	bl 0x0200b690
	sub sp, #-56
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00006666
	.4byte 0xfff40000
	.4byte 0xfffc0000
	.global Func_020031c0
	.thumb_func
Func_020031c0:
	push {r5, r6, lr}
	ldr r3, [pc, #64]
	adds r2, r1, #0
	asrs r2, r2, #20
	ldr r1, [r3]
	movs r3, #64
	subs r2, r3, r2
	adds r6, r2, #0
	adds r5, r2, #0
	movs r4, #0
	adds r6, #8
	adds r5, #11
	adds r1, #20
.L_020031c0_1:
	ldmia r1!, {r3}
	cmp r3, #0
	beq .L_020031c0_0
	ldr r2, [r3, #8]
	ldr r3, [r3, #16]
	asrs r2, r2, #20
	subs r2, #4
	asrs r3, r3, #20
	cmp r2, #4
	bhi .L_020031c0_0
	cmp r6, r3
	bgt .L_020031c0_0
	cmp r3, r5
	bge .L_020031c0_0
	stmia r0!, {r4}
.L_020031c0_0:
	adds r4, #1
	cmp r4, #65
	bls .L_020031c0_1
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.global Func_02003208
	.thumb_func
Func_02003208:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #256]
	movs r1, #178
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r1, r1, r3
	sub sp, #24
	mov r11, r1
	movs r1, #4
	ldr r2, [pc, #244]
	add r1, sp
	movs r3, #0
	mov r8, r1
	mov r10, r2
	mov r9, r3
	movs r2, #0
	movs r1, #66
	mov r3, r8
.L_02003208_0:
	adds r2, #1
	stmia r3!, {r1}
	cmp r2, #4
	bls .L_02003208_0
	mov r2, r11
	ldr r1, [r2, #12]
	mov r0, r8
	bl 0x0200b1c0
	mov r1, r8
	ldr r3, [r1]
	movs r2, #0
	cmp r3, #66
	beq .L_02003208_1
	movs r6, #0
	movs r5, #0
.L_02003208_2:
	mov r3, r8
	ldr r0, [r5, r3]
	str r2, [sp, #0]
	bl 0x0200b6f0
	ldr r2, [sp, #0]
	adds r0, #85
	movs r1, #1
	adds r2, #1
	strb r6, [r0]
	add r9, r1
	adds r5, #4
	cmp r2, #4
	bhi .L_02003208_1
	mov r1, r8
	ldr r3, [r5, r1]
	cmp r3, #66
	bne .L_02003208_2
.L_02003208_1:
	movs r0, #223
	bl 0x0200b840
	movs r2, #0
.L_02003208_7:
	mov r1, r11
	ldr r3, [r1, #12]
	mov r1, r10
	subs r3, r3, r1
	movs r7, #0
	mov r1, r11
	str r3, [r1, #12]
	cmp r7, r9
	bcs .L_02003208_3
	mov r6, r8
.L_02003208_4:
	ldr r0, [r6]
	str r2, [sp, #0]
	bl 0x0200b6f0
	ldr r3, [r0, #16]
	add r3, r10
	str r3, [r0, #16]
	ldr r0, [r6]
	bl 0x0200b6f0
	adds r5, r0, #0
	ldmia r6!, {r0}
	bl 0x0200b6f0
	ldr r3, [r0, #16]
	adds r7, #1
	str r3, [r5, #64]
	ldr r2, [sp, #0]
	cmp r7, r9
	bcc .L_02003208_4
.L_02003208_3:
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	bne .L_02003208_5
	ldr r3, [pc, #80]
	add r10, r3
.L_02003208_5:
	ldr r1, [pc, #80]
	cmp r10, r1
	ble .L_02003208_6
	movs r3, #192
	lsls r3, r3, #9
	mov r10, r3
.L_02003208_6:
	movs r0, #1
	str r2, [sp, #0]
	bl 0x0200b630
	ldr r2, [sp, #0]
	adds r2, #1
	cmp r2, #227
	bls .L_02003208_7
	movs r2, #0
	cmp r2, r9
	bcs .L_02003208_8
	movs r6, #0
	mov r5, r8
.L_02003208_9:
	ldmia r5!, {r0}
	str r2, [sp, #0]
	bl 0x0200b6f0
	ldr r2, [sp, #0]
	adds r0, #85
	adds r2, #1
	strb r6, [r0]
	cmp r2, r9
	bcc .L_02003208_9
.L_02003208_8:
	sub sp, #-24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001e70
	.4byte 0x00001999
	.4byte 0x00017fff
	.global Func_02003324
	.thumb_func
Func_02003324:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #244]
	movs r1, #178
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r1, r1, r3
	sub sp, #24
	mov r11, r1
	add r1, sp, #4
	movs r2, #192
	lsls r2, r2, #9
	movs r3, #0
	mov r8, r1
	mov r10, r2
	mov r9, r3
	movs r2, #0
	movs r1, #66
	mov r3, r8
.L_02003324_0:
	adds r2, #1
	stmia r3!, {r1}
	cmp r2, #4
	bls .L_02003324_0
	mov r2, r11
	ldr r1, [r2, #12]
	mov r0, r8
	bl 0x0200b1c0
	mov r1, r8
	ldr r3, [r1]
	movs r2, #0
	cmp r3, #66
	beq .L_02003324_1
	movs r6, #0
	movs r5, #0
.L_02003324_2:
	mov r3, r8
	ldr r0, [r5, r3]
	str r2, [sp, #0]
	bl 0x0200b6f0
	ldr r2, [sp, #0]
	adds r0, #85
	movs r1, #1
	adds r2, #1
	strb r6, [r0]
	add r9, r1
	adds r5, #4
	cmp r2, #4
	bhi .L_02003324_1
	mov r1, r8
	ldr r3, [r5, r1]
	cmp r3, #66
	bne .L_02003324_2
.L_02003324_1:
	movs r0, #223
	bl 0x0200b840
	movs r2, #0
.L_02003324_7:
	mov r1, r11
	ldr r3, [r1, #12]
	movs r7, #0
	add r3, r10
	str r3, [r1, #12]
	cmp r7, r9
	bcs .L_02003324_3
	mov r6, r8
.L_02003324_4:
	ldr r0, [r6]
	str r2, [sp, #0]
	bl 0x0200b6f0
	ldr r3, [r0, #16]
	mov r1, r10
	subs r3, r3, r1
	str r3, [r0, #16]
	ldr r0, [r6]
	bl 0x0200b6f0
	adds r5, r0, #0
	ldmia r6!, {r0}
	bl 0x0200b6f0
	ldr r3, [r0, #16]
	adds r7, #1
	str r3, [r5, #64]
	ldr r2, [sp, #0]
	cmp r7, r9
	bcc .L_02003324_4
.L_02003324_3:
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	bne .L_02003324_5
	cmp r2, #75
	bls .L_02003324_5
	ldr r3, [pc, #68]
	add r10, r3
.L_02003324_5:
	ldr r1, [pc, #68]
	cmp r10, r1
	bgt .L_02003324_6
	ldr r3, [pc, #64]
	mov r10, r3
.L_02003324_6:
	movs r0, #1
	str r2, [sp, #0]
	bl 0x0200b630
	ldr r2, [sp, #0]
	adds r2, #1
	cmp r2, #85
	bls .L_02003324_7
	movs r3, #128
	lsls r3, r3, #19
	mov r1, r11
	str r3, [r1, #12]
	bl 0x0200b678
	movs r0, #2
	bl 0x0200b630
	sub sp, #-24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001e70
	.4byte 0xffffcccd
	.4byte 0x00000ccb
	.4byte 0x00000ccc
	.global Func_02003438
	.thumb_func
Func_02003438:
	push {lr}
	movs r0, #93
	movs r1, #1
	bl 0x0200b810
	movs r1, #9
	movs r0, #24
	bl 0x0200b818
	bl 0x0200b830
	movs r0, #1
	bl 0x0200b808
	bl 0x0200b820
	bl 0x0200b828
	pop {r0}
	bx r0
	.global Func_02003460
	.thumb_func
Func_02003460:
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
	bl 0x0200b650
	ldr r3, [r5, #48]
	adds r3, #28
	adds r2, r3, #0
	muls r2, r0
	mov r1, r10
	ldr r3, [r1, #8]
	mov r0, r8
	adds r3, r3, r2
	str r3, [r5, #8]
	bl 0x0200b648
	movs r2, #144
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
	.global Func_020034bc
	.thumb_func
Func_020034bc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #8
	bl 0x0200b6f0
	ldr r3, [pc, #100]
	mov r10, r0
	ldr r5, [r3]
	bl 0x0200b640
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	lsls r3, r3, #16
	adds r5, #232
	mov r8, r3
	movs r0, #2
	ldrsh r3, [r5, r0]
	cmp r3, #129
	bgt .L_020034bc_0
	ldr r3, [pc, #72]
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_020034bc_1
	movs r1, #152
	movs r2, #144
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #8
	bl 0x0200b728
	movs r0, #8
	bl 0x0200b6f0
	movs r5, #128
	lsls r5, r5, #9
	b .L_020034bc_2
.L_020034bc_1:
	movs r1, #152
	movs r2, #151
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #8
	bl 0x0200b728
	movs r0, #8
	bl 0x0200b6f0
	ldr r5, [pc, #20]
.L_020034bc_2:
	str r5, [r0, #24]
	movs r0, #8
	bl 0x0200b6f0
	str r5, [r0, #28]
	b .L_020034bc_3
	.4byte 0x03001e70
	.4byte 0x03001e40
	.4byte 0x00014ccc
.L_020034bc_0:
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #12
	lsls r2, r2, #12
	bl 0x0200b728
.L_020034bc_3:
	mov r1, r10
	cmp r1, #0
	beq .L_020034bc_4
	ldr r3, [pc, #160]
	ldr r6, [r3]
	movs r3, #15
	ands r6, r3
	cmp r6, #0
	bne .L_020034bc_4
	mov r0, r10
	ldr r2, [r0, #12]
	ldr r1, [r1, #8]
	movs r3, #128
	lsls r3, r3, #12
	add r2, r8
	adds r1, r1, r3
	adds r2, r2, r3
	ldr r3, [r0, #16]
	movs r0, #142
	lsls r0, r0, #1
	bl 0x0200b670
	movs r1, #192
	lsls r1, r1, #11
	adds r7, r0, #0
	mov r0, r8
	bl 0x0200b628
	mov r8, r0
	mov r1, r8
	lsls r1, r1, #16
	mov r8, r1
	cmp r7, #0
	beq .L_020034bc_4
	ldr r1, [pc, #104]
	adds r0, r7, #0
	ldr r5, [r7, #80]
	bl 0x0200b668
	movs r1, #3
	adds r0, r7, #0
	bl 0x0200b760
	adds r3, r7, #0
	adds r3, #85
	strb r6, [r3]
	bl 0x0200b640
	ldr r3, [pc, #80]
	adds r2, r7, #0
	ands r3, r0
	adds r2, #100
	ldr r0, [pc, #60]
	strh r3, [r2]
	adds r3, r7, #0
	mov r9, r0
	adds r3, #102
	ldr r0, [pc, #64]
	strh r6, [r3]
	mov r2, r8
	ldr r3, [pc, #64]
	mov r1, r10
	ands r0, r2
	str r1, [r7, #104]
	str r3, [r7, #108]
	asrs r0, r0, #4
	bl 0x0200b648
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	asrs r3, r3, #16
	str r3, [r7, #48]
	adds r3, r5, #0
	adds r3, #38
	mov r0, r9
	strb r0, [r3]
	mov r1, r10
	ldr r3, [r1, #80]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	b .L_020034bc_5
	.4byte 0x00000000
	.4byte 0x03001e40
	.4byte 0x0200bc54
	.4byte 0x0ffff000
	.4byte 0x000fffff
	.4byte 0x0200b461
.L_020034bc_5:
	ldrb r1, [r5, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r5, #9]
.L_020034bc_4:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.include "games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/IMPORT.INC"
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
	.4byte 0x0200b884
	.4byte 0x0200b8bc
	.4byte 0x0200b8f4
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000008
	.4byte 0x40000008
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x000001a8
	.4byte 0xffff0002
	.4byte 0x00000008
	.4byte 0x40000008
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x000001a8
	.4byte 0xffff0005
	.4byte 0x000001f8
	.4byte 0x40000098
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x000001a8
	.4byte 0xffff0033
	.4byte 0x000001f8
	.4byte 0x400000a8
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x000001a8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000003a
	.4byte 0x0010f039
	.4byte 0x000001ff
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00026000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00022000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x02530005
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00020000
	.4byte 0xffff00f4
	.4byte 0x00000007
	.4byte 0x01300000
	.4byte 0x00280000
	.4byte 0x00900000
	.4byte 0x00024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0x0253001e
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0x02530023
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00020000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x02530021
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00020000
	.4byte 0xffff002b
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x08800005
	.4byte 0x02008b25
	.4byte 0x00000002
	.4byte 0x02510006
	.4byte 0x0200addd
	.4byte 0x00000202
	.4byte 0xffff000b
	.4byte 0x02008929
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008ad1
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x02008929
	.4byte 0x00000002
	.4byte 0x0250000a
	.4byte 0x020089fd
	.4byte 0x00000003
	.4byte 0x03500064
	.4byte 0x00300000
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x0200aeb9
	.4byte 0xffffffff
	.4byte 0x00000000
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
