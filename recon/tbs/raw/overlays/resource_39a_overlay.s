.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {lr}
	movs r0, #8
	movs r1, #2
	movs r2, #1
	bl 0x0200a408
	pop {r0}
	bx r0
	.global Func_02000040
	.thumb_func
Func_02000040:
	push {lr}
	movs r0, #11
	movs r1, #62
	bl 0x0200a3a8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000050
	.thumb_func
Func_02000050:
	push {lr}
	ldr r3, [pc, #88]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #80]
	cmp r2, r3
	bne .L_02000050_0
	ldr r0, [pc, #76]
	b .L_02000050_1
.L_02000050_0:
	ldr r3, [pc, #76]
	cmp r2, r3
	bne .L_02000050_2
	ldr r0, [pc, #76]
	b .L_02000050_1
.L_02000050_2:
	ldr r3, [pc, #76]
	cmp r2, r3
	bne .L_02000050_3
	ldr r0, [pc, #72]
	b .L_02000050_1
.L_02000050_3:
	ldr r3, [pc, #72]
	cmp r2, r3
	bne .L_02000050_4
	ldr r0, [pc, #72]
	b .L_02000050_1
.L_02000050_4:
	ldr r3, [pc, #72]
	cmp r2, r3
	bne .L_02000050_5
	ldr r0, [pc, #68]
	b .L_02000050_1
.L_02000050_5:
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_02000050_6
	ldr r0, [pc, #68]
	b .L_02000050_1
.L_02000050_6:
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_02000050_7
	ldr r0, [pc, #64]
	b .L_02000050_1
.L_02000050_7:
	ldr r0, [pc, #64]
.L_02000050_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000034
	.4byte 0x0200a4bc
	.4byte 0x0000003e
	.4byte 0x0200a504
	.4byte 0x0000003f
	.4byte 0x0200a5f4
	.4byte 0x00000040
	.4byte 0x0200a63c
	.4byte 0x00000041
	.4byte 0x0200a6cc
	.4byte 0x00000042
	.4byte 0x0200a744
	.4byte 0x00000043
	.4byte 0x0200a7bc
	.4byte 0x0200a48c
	.global Func_020000ec
	.thumb_func
Func_020000ec:
	movs r0, #0
	bx lr
	.global Func_020000f0
	.thumb_func
Func_020000f0:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200a8f4
	.global Func_020000f8
	.thumb_func
Func_020000f8:
	push {lr}
	ldr r3, [pc, #76]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_020000f8_0
	ldr r0, [pc, #64]
	b .L_020000f8_1
.L_020000f8_0:
	ldr r3, [pc, #64]
	cmp r2, r3
	bne .L_020000f8_2
	ldr r0, [pc, #64]
	b .L_020000f8_1
.L_020000f8_2:
	ldr r3, [pc, #64]
	cmp r2, r3
	bne .L_020000f8_3
	ldr r0, [pc, #60]
	b .L_020000f8_1
.L_020000f8_3:
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_020000f8_4
	ldr r0, [pc, #60]
	b .L_020000f8_1
.L_020000f8_4:
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_020000f8_5
	ldr r0, [pc, #56]
	b .L_020000f8_1
.L_020000f8_5:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_020000f8_6
	ldr r0, [pc, #56]
	b .L_020000f8_1
.L_020000f8_6:
	ldr r0, [pc, #56]
.L_020000f8_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000034
	.4byte 0x0200a9bc
	.4byte 0x0000003e
	.4byte 0x0200a9ec
	.4byte 0x0000003f
	.4byte 0x0200aa4c
	.4byte 0x00000040
	.4byte 0x0200aac4
	.4byte 0x00000041
	.4byte 0x0200ab3c
	.4byte 0x00000043
	.4byte 0x0200ab9c
	.4byte 0x0200a9a4
	.global Func_02000180
	.thumb_func
Func_02000180:
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
	bl 0x0200a350
	adds r6, r0, #0
	adds r0, r5, #0
	bl 0x0200a350
	adds r7, r0, #0
	bl 0x0200a338
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
	bl 0x0200a2c8
	adds r0, r6, #0
	movs r1, #27
	bl 0x0200a2a0
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
	bl 0x0200a2c8
	ldr r3, [sp, #4]
	cmp r3, #0
	blt .L_02000180_0
	ldr r2, [sp, #0]
	cmp r2, #0
	bge .L_02000180_1
.L_02000180_0:
	adds r0, r7, #0
	movs r1, #4
	bl 0x0200a2a0
	b .L_02000180_2
.L_02000180_1:
	adds r0, r7, #0
	movs r1, #3
	bl 0x0200a2a0
.L_02000180_2:
	adds r0, r6, #0
	bl 0x0200a2d0
	bl 0x0200a340
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
	.global Func_0200025c
	.thumb_func
Func_0200025c:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r0, #8
	movs r1, #112
	movs r2, #0
	bl 0x02008180
	movs r1, #112
	movs r2, #0
	movs r0, #8
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	ldr r0, [pc, #24]
	bl 0x0200a320
	movs r0, #2
	bl 0x0200a260
	bl 0x02009050
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000121
	.4byte 0x00000301
	.global Func_0200029c
	.thumb_func
Func_0200029c:
	push {r5, lr}
	movs r5, #112
	negs r5, r5
	movs r0, #241
	bl 0x0200a410
	adds r1, r5, #0
	movs r0, #8
	movs r2, #0
	bl 0x02008180
	adds r1, r5, #0
	movs r2, #0
	movs r0, #8
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	ldr r0, [pc, #24]
	bl 0x0200a328
	movs r0, #2
	bl 0x0200a260
	bl 0x02009050
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000121
	.4byte 0x00000301
	.global Func_020002e0
	.thumb_func
Func_020002e0:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r1, #0
	movs r2, #64
	movs r0, #9
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	ldr r0, [pc, #20]
	bl 0x0200a328
	movs r0, #2
	bl 0x0200a260
	bl 0x02009050
	pop {r0}
	bx r0
	.4byte 0x00000121
	.4byte 0x00000302
	.global Func_02000314
	.thumb_func
Func_02000314:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r2, #64
	negs r2, r2
	movs r1, #0
	movs r0, #9
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	ldr r0, [pc, #24]
	bl 0x0200a320
	movs r0, #2
	bl 0x0200a260
	bl 0x02009050
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000121
	.4byte 0x00000302
	.global Func_0200034c
	.thumb_func
Func_0200034c:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r1, #0
	movs r2, #64
	movs r0, #10
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	ldr r0, [pc, #20]
	bl 0x0200a328
	movs r0, #2
	bl 0x0200a260
	bl 0x02009050
	pop {r0}
	bx r0
	.4byte 0x00000121
	.4byte 0x00000303
	.global Func_02000380
	.thumb_func
Func_02000380:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r2, #64
	negs r2, r2
	movs r1, #0
	movs r0, #10
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	ldr r0, [pc, #24]
	bl 0x0200a320
	movs r0, #2
	bl 0x0200a260
	bl 0x02009050
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000121
	.4byte 0x00000303
	.global Func_020003b8
	.thumb_func
Func_020003b8:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r1, #144
	movs r2, #0
	movs r0, #8
	bl 0x02008180
	ldr r0, [pc, #28]
	bl 0x0200a410
	movs r0, #193
	lsls r0, r0, #2
	bl 0x0200a320
	movs r0, #2
	bl 0x0200a260
	bl 0x02009154
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000121
.L_020003ec:
	.global Func_020003ec
	.thumb_func
Func_020003ec:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r1, #144
.L_020003f6:
	negs r1, r1
	movs r2, #0
	movs r0, #8
	bl 0x02008180
	ldr r0, [pc, #32]
	bl 0x0200a410
	movs r0, #193
	lsls r0, r0, #2
	bl 0x0200a328
	ldr r0, [pc, #24]
	bl 0x0200a320
	movs r0, #2
	bl 0x0200a260
	bl 0x02009154
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000121
	.4byte 0x00000305
	.global Func_0200042c
	.thumb_func
Func_0200042c:
	push {lr}
	movs r0, #241
.L_02000430:
	bl 0x0200a410
	movs r1, #14
	negs r1, r1
	movs r2, #0
.L_0200043a:
	movs r0, #8
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	ldr r0, [pc, #24]
	bl 0x0200a320
	movs r0, #2
	bl 0x0200a260
	bl 0x02009154
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000121
	.4byte 0x00000305
	.global Func_02000464
	.thumb_func
Func_02000464:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	ldr r0, [pc, #64]
	bl 0x0200a318
	cmp r0, #0
	beq .L_02000464_0
	movs r0, #8
	movs r1, #16
	movs r2, #0
	bl 0x02008180
	ldr r0, [pc, #48]
	bl 0x0200a328
	b .L_02000464_1
.L_02000464_0:
	movs r0, #8
	movs r1, #144
	movs r2, #0
	bl 0x02008180
	movs r0, #193
	lsls r0, r0, #2
	bl 0x0200a320
.L_02000464_1:
	ldr r0, [pc, #28]
	bl 0x0200a410
	movs r0, #2
	bl 0x0200a260
.L_020004a6:
	bl 0x02009154
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0306
	.2byte 0x0000
	.2byte 0x0305
	.2byte 0x0000
	.2byte 0x0121
	.2byte 0x0000
	.global Func_020004bc
	.thumb_func
Func_020004bc:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r1, #0
	movs r2, #64
	movs r0, #9
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	ldr r0, [pc, #20]
	bl 0x0200a320
	movs r0, #2
.L_020004dc:
	bl 0x0200a260
	bl 0x02009154
	pop {r0}
	bx r0
	.2byte 0x0121
	.2byte 0x0000
	.2byte 0x0306
	.2byte 0x0000
	.global Func_020004f0
	.thumb_func
Func_020004f0:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r2, #64
	negs r2, r2
	movs r1, #0
	movs r0, #9
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	ldr r0, [pc, #24]
	bl 0x0200a328
	movs r0, #2
.L_02000512:
	bl 0x0200a260
	bl 0x02009154
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0121
	.2byte 0x0000
	.2byte 0x0306
	.2byte 0x0000
	.global Func_02000528
	.thumb_func
Func_02000528:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r0, #10
	movs r1, #0
	movs r2, #144
	bl 0x02008180
	movs r1, #0
	movs r2, #128
	movs r0, #10
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	ldr r0, [pc, #24]
	bl 0x0200a328
	movs r0, #2
	bl 0x0200a260
	bl 0x02009154
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000121
	.4byte 0x00000307
	.global Func_02000568
	.thumb_func
Func_02000568:
	push {r5, lr}
	movs r5, #96
	negs r5, r5
	movs r0, #241
	bl 0x0200a410
	adds r2, r5, #0
	movs r0, #10
	movs r1, #0
	bl 0x02008180
.L_0200057e:
	adds r2, r5, #0
	movs r0, #10
	movs r1, #0
	bl 0x02008180
	movs r2, #80
	negs r2, r2
	movs r1, #0
	movs r0, #10
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	ldr r0, [pc, #24]
	bl 0x0200a320
	movs r0, #2
	bl 0x0200a260
	bl 0x02009154
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000121
	.4byte 0x00000307
	.global Func_020005b8
	.thumb_func
Func_020005b8:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r0, #196
	lsls r0, r0, #2
	bl 0x0200a318
	cmp r0, #0
	bne .L_020005b8_0
	ldr r0, [pc, #80]
	bl 0x0200a318
	cmp r0, #0
	beq .L_020005b8_1
.L_020005b8_0:
	movs r1, #48
	negs r1, r1
	movs r0, #8
	movs r2, #0
	bl 0x02008180
	movs r0, #194
	lsls r0, r0, #2
	bl 0x0200a328
	ldr r0, [pc, #56]
	bl 0x0200a320
	b 0x0200860c
.L_020005b8_1:
	movs r1, #96
	negs r1, r1
	movs r0, #8
	movs r2, #0
	bl 0x02008180
.L_020005fe:
	movs r0, #194
	lsls r0, r0, #2
	bl 0x0200a320
	ldr r0, [pc, #28]
	bl 0x0200a328
	ldr r0, [pc, #24]
	bl 0x0200a410
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
	pop {r0}
	bx r0
	.2byte 0x030d
	.2byte 0x0000
	.4byte 0x00000309
	.4byte 0x00000121
	.global Func_0200062c
	.thumb_func
Func_0200062c:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r0, #194
	lsls r0, r0, #2
	bl 0x0200a328
	ldr r0, [pc, #32]
	bl 0x0200a328
	movs r1, #48
	movs r2, #0
	movs r0, #8
	bl 0x02008180
.L_0200064c:
	ldr r0, [pc, #20]
	bl 0x0200a410
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
	pop {r0}
	bx r0
	.2byte 0x0309
	.2byte 0x0000
	.4byte 0x00000121
	.global Func_02000668
	.thumb_func
Func_02000668:
	push {lr}
	movs r0, #196
	lsls r0, r0, #2
	bl 0x0200a318
	cmp r0, #0
	bne 0x020086b0
	ldr r0, [pc, #60]
	bl 0x0200a318
	cmp r0, #0
	bne 0x020086b0
	movs r0, #241
.L_02000682:
	bl 0x0200a410
	movs r0, #194
	lsls r0, r0, #2
	bl 0x0200a320
	ldr r0, [pc, #40]
	bl 0x0200a328
	movs r1, #48
	negs r1, r1
	movs r2, #0
	movs r0, #8
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
	pop {r0}
	bx r0
	.2byte 0x030d
	.2byte 0x0000
	.4byte 0x00000309
	.4byte 0x00000121
	.global Func_020006c0
	.thumb_func
Func_020006c0:
	push {lr}
.L_020006c2:
	movs r0, #241
	bl 0x0200a410
	movs r0, #194
	lsls r0, r0, #2
	bl 0x0200a328
	ldr r0, [pc, #32]
	bl 0x0200a328
	movs r1, #96
	movs r2, #0
	movs r0, #8
	bl 0x02008180
	ldr r0, [pc, #20]
	bl 0x0200a410
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
	pop {r0}
	bx r0
	.4byte 0x00000309
	.4byte 0x00000121
.L_020006fc:
	.global Func_020006fc
	.thumb_func
Func_020006fc:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r1, #32
.L_02000706:
	negs r1, r1
	movs r2, #0
	movs r0, #9
	bl 0x02008180
	ldr r0, [pc, #24]
.L_02000712:
	bl 0x0200a410
	ldr r0, [pc, #24]
	bl 0x0200a320
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0121
	.2byte 0x0000
	.4byte 0x0000030a
	.global Func_02000734
	.thumb_func
Func_02000734:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r1, #32
	movs r2, #0
	movs r0, #9
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	ldr r0, [pc, #20]
	bl 0x0200a328
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
	pop {r0}
	bx r0
	.4byte 0x00000121
	.4byte 0x0000030a
	.global Func_02000768
	.thumb_func
Func_02000768:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r0, #194
	lsls r0, r0, #2
	bl 0x0200a318
	cmp r0, #0
	beq 0x020087a4
.L_0200077c:
	movs r2, #64
	negs r2, r2
	movs r1, #0
	movs r0, #10
	bl 0x02008180
	ldr r0, [pc, #84]
	bl 0x0200a328
	movs r0, #195
	lsls r0, r0, #2
	bl 0x0200a320
	ldr r0, [pc, #76]
	bl 0x0200a328
	ldr r0, [pc, #72]
	bl 0x0200a328
	b 0x020087ca
	.2byte 0x2280
	.2byte 0x4252
	.2byte 0x2100
	.2byte 0x200a
	.2byte 0xf7ff
	.2byte 0xfce8
	.2byte 0x480b
	.2byte 0xf001
	.2byte 0xfdb5
	.2byte 0x20c3
	.2byte 0x0080
	.2byte 0xf001
	.2byte 0xfdb5
	.2byte 0x4809
	.2byte 0xf001
	.2byte 0xfdb2
	.2byte 0x4808
	.2byte 0xf001
	.2byte 0xfdaf
.L_020007ca:
	ldr r0, [pc, #32]
	bl 0x0200a410
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x030b
	.2byte 0x0000
	.2byte 0x030d
	.2byte 0x0000
	.2byte 0x030e
	.2byte 0x0000
	.4byte 0x00000121
	.global Func_020007f0
	.thumb_func
Func_020007f0:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r0, #194
	lsls r0, r0, #2
	bl 0x0200a318
	cmp r0, #0
	beq 0x02008824
	movs r1, #0
	movs r2, #16
	movs r0, #10
	bl 0x02008180
	ldr r0, [pc, #168]
	bl 0x0200a328
	movs r0, #195
	lsls r0, r0, #2
	bl 0x0200a320
	ldr r0, [pc, #156]
.L_0200081e:
	bl 0x0200a328
	b 0x02008878
	.2byte 0x20c4
	.2byte 0x0080
	.2byte 0xf001
	.2byte 0xfd76
	.2byte 0x2800
	.2byte 0xd00f
	.2byte 0x2100
	.2byte 0x2210
	.2byte 0x200a
	.2byte 0xf7ff
	.2byte 0xfca3
	.2byte 0x481f
	.2byte 0xf001
	.2byte 0xfd74
	.2byte 0x20c3
	.2byte 0x0080
	.2byte 0xf001
	.2byte 0xfd6c
	.2byte 0x481c
	.2byte 0xf001
	.2byte 0xfd6d
	.2byte 0xe013
	.2byte 0x481b
	.2byte 0xf001
	.2byte 0xfd61
	.2byte 0x2800
	.2byte 0xd012
	.2byte 0x2100
	.2byte 0x2240
.L_0200085e:
	movs r0, #10
	bl 0x02008180
	ldr r0, [pc, #80]
	bl 0x0200a328
	movs r0, #195
	lsls r0, r0, #2
	bl 0x0200a328
	ldr r0, [pc, #72]
	bl 0x0200a320
	ldr r0, [pc, #72]
	bl 0x0200a328
	b .L_0200085e_0
	.2byte 0x2100
	.2byte 0x2280
	.2byte 0x200a
	.2byte 0xf7ff
	.2byte 0xfc7b
	.2byte 0x480b
	.2byte 0xf001
	.2byte 0xfd4c
	.2byte 0x20c3
	.2byte 0x0080
	.2byte 0xf001
	.2byte 0xfd48
	.2byte 0x4808
	.2byte 0xf001
	.2byte 0xfd45
	.2byte 0x4809
	.2byte 0xf001
	.2byte 0xfd42
.L_0200085e_0:
	ldr r0, [pc, #32]
	bl 0x0200a410
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
	pop {r0}
	bx r0
	.4byte 0x0000030b
	.4byte 0x0000030d
	.2byte 0x0311
	.2byte 0x0000
	.4byte 0x0000030e
	.4byte 0x00000121
	.global Func_020008cc
	.thumb_func
Func_020008cc:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r2, #16
	negs r2, r2
	movs r1, #0
	movs r0, #10
	bl 0x02008180
	ldr r0, [pc, #44]
	bl 0x0200a410
	ldr r0, [pc, #44]
	bl 0x0200a320
	movs r0, #195
	lsls r0, r0, #2
	bl 0x0200a328
	ldr r0, [pc, #32]
	bl 0x0200a328
	ldr r0, [pc, #32]
	bl 0x0200a328
	movs r0, #2
	bl 0x0200a260
.L_02000906:
	bl 0x020092cc
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0121
	.2byte 0x0000
	.2byte 0x030b
	.2byte 0x0000
	.2byte 0x030d
	.2byte 0x0000
	.2byte 0x030e
	.2byte 0x0000
	.global Func_02000920
	.thumb_func
Func_02000920:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	ldr r0, [pc, #144]
	bl 0x0200a318
.L_0200092e:
	cmp r0, #0
	beq .L_0200092e_0
	movs r1, #0
	movs r2, #48
	movs r0, #10
	bl 0x02008180
	ldr r0, [pc, #128]
	bl 0x0200a328
	movs r0, #195
	lsls r0, r0, #2
	bl 0x0200a328
	ldr r0, [pc, #120]
	bl 0x0200a320
	b .L_0200092e_1
.L_0200092e_0:
	movs r0, #196
	lsls r0, r0, #2
	bl 0x0200a318
	cmp r0, #0
	beq .L_0200092e_2
	movs r1, #0
	movs r2, #32
	movs r0, #10
	bl 0x02008180
	ldr r0, [pc, #84]
	bl 0x0200a328
	movs r0, #195
	lsls r0, r0, #2
	bl 0x0200a320
	ldr r0, [pc, #76]
	bl 0x0200a328
.L_0200092e_1:
	ldr r0, [pc, #72]
	bl 0x0200a328
	b .L_0200092e_3
.L_0200092e_2:
	movs r1, #0
	movs r2, #112
	movs r0, #10
	bl 0x02008180
	ldr r0, [pc, #48]
	bl 0x0200a328
	movs r0, #195
	lsls r0, r0, #2
	bl 0x0200a328
	ldr r0, [pc, #36]
	bl 0x0200a328
	ldr r0, [pc, #36]
	bl 0x0200a328
.L_0200092e_3:
	ldr r0, [pc, #32]
	bl 0x0200a410
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
.L_020009b8:
	pop {r0}
	bx r0
	.2byte 0x0311
	.2byte 0x0000
	.2byte 0x030b
	.2byte 0x0000
	.2byte 0x030d
	.2byte 0x0000
	.2byte 0x030e
	.2byte 0x0000
	.2byte 0x0121
	.2byte 0x0000
	.global Func_020009d0
	.thumb_func
Func_020009d0:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r2, #64
	negs r2, r2
	movs r1, #0
	movs r0, #10
	bl 0x02008180
	ldr r0, [pc, #28]
	bl 0x0200a410
	ldr r0, [pc, #28]
	bl 0x0200a320
	ldr r0, [pc, #24]
	bl 0x0200a328
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
	pop {r0}
	bx r0
	.4byte 0x00000121
	.4byte 0x0000030b
	.4byte 0x0000030d
	.global Func_02000a10
	.thumb_func
Func_02000a10:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r1, #0
	movs r2, #64
	movs r0, #10
	bl 0x02008180
	ldr r0, [pc, #44]
	bl 0x0200a410
	ldr r0, [pc, #40]
	bl 0x0200a328
	movs r0, #195
	lsls r0, r0, #2
	bl 0x0200a328
	ldr r0, [pc, #32]
	bl 0x0200a328
	ldr r0, [pc, #28]
	bl 0x0200a328
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
	pop {r0}
	bx r0
	.4byte 0x00000121
	.4byte 0x0000030b
	.4byte 0x0000030d
	.4byte 0x0000030e
	.global Func_02000a60
	.thumb_func
Func_02000a60:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r2, #80
	negs r2, r2
	movs r1, #0
	movs r0, #10
	bl 0x02008180
	ldr r0, [pc, #44]
	bl 0x0200a410
	ldr r0, [pc, #44]
	bl 0x0200a320
	movs r0, #195
	lsls r0, r0, #2
	bl 0x0200a328
	ldr r0, [pc, #32]
	bl 0x0200a328
	ldr r0, [pc, #32]
	bl 0x0200a328
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000121
	.4byte 0x0000030b
	.4byte 0x0000030d
	.4byte 0x0000030e
	.global Func_02000ab4
	.thumb_func
Func_02000ab4:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r1, #0
	movs r2, #64
	movs r0, #10
	bl 0x02008180
	ldr r0, [pc, #44]
	bl 0x0200a410
	ldr r0, [pc, #40]
	bl 0x0200a328
	movs r0, #195
	lsls r0, r0, #2
	bl 0x0200a328
	ldr r0, [pc, #32]
	bl 0x0200a328
	ldr r0, [pc, #28]
	bl 0x0200a328
.L_02000ae6:
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
	pop {r0}
	bx r0
	.2byte 0x0121
	.2byte 0x0000
	.2byte 0x030b
	.2byte 0x0000
	.2byte 0x030d
	.2byte 0x0000
	.2byte 0x030e
	.2byte 0x0000
	.global Func_02000b04
	.thumb_func
Func_02000b04:
	push {lr}
	movs r0, #241
	bl 0x0200a410
.L_02000b0c:
	movs r0, #194
	lsls r0, r0, #2
	bl 0x0200a318
	cmp r0, #0
	bne .L_02000b0c_0
	ldr r0, [pc, #152]
	bl 0x0200a318
	cmp r0, #0
	beq .L_02000b0c_1
.L_02000b0c_0:
	movs r2, #64
	negs r2, r2
	movs r1, #0
	movs r0, #11
	bl 0x02008180
	ldr r0, [pc, #136]
	bl 0x0200a328
	movs r0, #196
	lsls r0, r0, #2
	bl 0x0200a328
	ldr r0, [pc, #124]
	bl 0x0200a320
	b 0x02008b70
.L_02000b0c_1:
	movs r0, #195
	lsls r0, r0, #2
	bl 0x0200a318
	cmp r0, #0
	beq 0x02008b78
	movs r2, #112
	negs r2, r2
	movs r1, #0
	movs r0, #11
	bl 0x02008180
	ldr r0, [pc, #88]
	bl 0x0200a328
.L_02000b62:
	movs r0, #196
	lsls r0, r0, #2
	bl 0x0200a320
	ldr r0, [pc, #80]
	bl 0x0200a328
	ldr r0, [pc, #76]
	bl 0x0200a328
	b .L_02000b62_0
	.2byte 0x2280
	.2byte 0x4252
	.2byte 0x2100
	.2byte 0x200b
	.2byte 0xf7ff
	.2byte 0xfafe
	.2byte 0x480c
	.2byte 0xf001
	.2byte 0xfbcb
	.2byte 0x20c4
	.2byte 0x0080
	.2byte 0xf001
	.2byte 0xfbcb
	.2byte 0x480a
	.2byte 0xf001
	.2byte 0xfbc8
	.2byte 0x4809
	.2byte 0xf001
	.2byte 0xfbc5
.L_02000b62_0:
	ldr r0, [pc, #36]
.L_02000ba0:
	bl 0x0200a410
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x030d
	.2byte 0x0000
	.2byte 0x030f
	.2byte 0x0000
	.2byte 0x0311
	.2byte 0x0000
	.2byte 0x0312
	.2byte 0x0000
	.2byte 0x0121
	.2byte 0x0000
	.global Func_02000bc8
	.thumb_func
Func_02000bc8:
	push {lr}
	ldr r0, [pc, #28]
	bl 0x0200a328
	movs r0, #196
	lsls r0, r0, #2
	bl 0x0200a328
	ldr r0, [pc, #16]
	bl 0x0200a328
	ldr r0, [pc, #16]
	bl 0x0200a328
	pop {r0}
	bx r0
	.4byte 0x0000030f
	.4byte 0x00000311
	.4byte 0x00000312
	.global Func_02000bf4
	.thumb_func
Func_02000bf4:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r1, #0
	movs r2, #128
	movs r0, #11
	bl 0x02008180
	bl 0x02008bc8
	ldr r0, [pc, #20]
	bl 0x0200a410
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000121
	.global Func_02000c24
	.thumb_func
Func_02000c24:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r2, #16
	negs r2, r2
	movs r1, #0
	movs r0, #11
	bl 0x02008180
	ldr r0, [pc, #44]
	bl 0x0200a410
	ldr r0, [pc, #44]
	bl 0x0200a320
.L_02000c44:
	movs r0, #196
	lsls r0, r0, #2
	bl 0x0200a328
	ldr r0, [pc, #32]
	bl 0x0200a328
	ldr r0, [pc, #32]
	bl 0x0200a328
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0121
	.2byte 0x0000
	.2byte 0x030f
	.2byte 0x0000
	.4byte 0x00000311
	.4byte 0x00000312
	.global Func_02000c78
	.thumb_func
Func_02000c78:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r1, #0
	movs r2, #112
	movs r0, #11
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	bl 0x02008bc8
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000121
	.global Func_02000ca8
	.thumb_func
Func_02000ca8:
	push {lr}
	movs r0, #241
.L_02000cac:
	bl 0x0200a410
	movs r1, #0
	movs r2, #64
	movs r0, #11
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	bl 0x02008bc8
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000121
	.global Func_02000cd8
	.thumb_func
Func_02000cd8:
	push {lr}
.L_02000cda:
	movs r0, #241
	bl 0x0200a410
	movs r1, #0
	movs r2, #80
	movs r0, #11
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	bl 0x02008bc8
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000121
	.global Func_02000d08
	.thumb_func
Func_02000d08:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r1, #0
	movs r2, #48
	movs r0, #11
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	bl 0x02008bc8
	movs r0, #2
	bl 0x0200a260
	bl 0x020092cc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000121
	.global Func_02000d38
	.thumb_func
Func_02000d38:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r1, #0
	movs r2, #112
	movs r0, #8
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	ldr r0, [pc, #20]
	bl 0x0200a320
	movs r0, #2
	bl 0x0200a260
	bl 0x020095dc
	pop {r0}
	bx r0
	.4byte 0x00000121
	.4byte 0x00000313
	.global Func_02000d6c
	.thumb_func
Func_02000d6c:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r2, #112
	negs r2, r2
	movs r1, #0
	movs r0, #8
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	ldr r0, [pc, #24]
	bl 0x0200a328
	movs r0, #2
	bl 0x0200a260
	bl 0x020095dc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000121
	.4byte 0x00000313
	.global Func_02000da4
	.thumb_func
Func_02000da4:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r1, #128
	negs r1, r1
	movs r2, #0
	movs r0, #9
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	movs r0, #197
	lsls r0, r0, #2
	bl 0x0200a320
	movs r0, #2
	bl 0x0200a260
	bl 0x020095dc
	pop {r0}
	bx r0
	.4byte 0x00000121
	.global Func_02000dd8
	.thumb_func
Func_02000dd8:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r1, #128
	movs r2, #0
	movs r0, #9
	bl 0x02008180
	ldr r0, [pc, #28]
	bl 0x0200a410
	movs r0, #197
	lsls r0, r0, #2
	bl 0x0200a328
	movs r0, #2
	bl 0x0200a260
	bl 0x020095dc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000121
	.global Func_02000e0c
	.thumb_func
Func_02000e0c:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r1, #160
	movs r2, #0
	movs r0, #10
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	ldr r0, [pc, #20]
	bl 0x0200a320
	movs r0, #2
	bl 0x0200a260
	bl 0x020095dc
	pop {r0}
	bx r0
	.4byte 0x00000121
	.4byte 0x00000315
	.global Func_02000e40
	.thumb_func
Func_02000e40:
	push {lr}
	movs r0, #241
	bl 0x0200a410
	movs r1, #160
	negs r1, r1
	movs r2, #0
	movs r0, #10
	bl 0x02008180
	ldr r0, [pc, #24]
	bl 0x0200a410
	ldr r0, [pc, #24]
	bl 0x0200a328
	movs r0, #2
	bl 0x0200a260
	bl 0x020095dc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000121
	.4byte 0x00000315
	.global Func_02000e78
	.thumb_func
Func_02000e78:
	push {lr}
	movs r0, #8
	movs r1, #1
	bl 0x0200a378
	movs r0, #8
	movs r1, #2
	bl 0x0200a378
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000e90
	.thumb_func
Func_02000e90:
	push {lr}
	movs r0, #9
	movs r1, #1
	bl 0x0200a378
	movs r0, #9
	movs r1, #2
	bl 0x0200a378
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000ea8
	.thumb_func
Func_02000ea8:
	push {lr}
	movs r0, #10
	movs r1, #1
	bl 0x0200a378
	movs r0, #10
	movs r1, #2
	bl 0x0200a378
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000ec0
	.thumb_func
Func_02000ec0:
	push {lr}
	movs r0, #11
	movs r1, #1
	bl 0x0200a378
	movs r0, #11
	movs r1, #2
	bl 0x0200a378
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000ed8
	.thumb_func
Func_02000ed8:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl 0x0200a2b8
	adds r5, r0, #0
	cmp r5, #0
	beq 0x02008f28
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
.L_02000efe:
	orrs r3, r2
	adds r2, r5, #0
	adds r2, #85
	strb r3, [r1, #9]
	movs r3, #0
	strb r3, [r2]
	movs r1, #0
	bl 0x0200a2f8
	adds r0, r5, #0
	movs r1, #15
	bl 0x0200a390
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	b .L_02000efe_0
	.2byte 0x2000
.L_02000efe_0:
	pop {r5, r6}
	pop {r1}
	bx r1
	.global Func_02000f30
	.thumb_func
Func_02000f30:
	push {lr}
	ldr r3, [pc, #88]
	movs r1, #224
.L_02000f36:
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #80]
	cmp r2, r3
	bne .L_02000f36_0
	ldr r0, [pc, #76]
	b 0x02008f86
.L_02000f36_0:
	ldr r3, [pc, #76]
	cmp r2, r3
	bne .L_02000f36_1
	ldr r0, [pc, #76]
	b 0x02008f86
.L_02000f36_1:
	ldr r3, [pc, #76]
	cmp r2, r3
	bne .L_02000f36_2
	ldr r0, [pc, #72]
	b 0x02008f86
.L_02000f36_2:
	ldr r3, [pc, #72]
	cmp r2, r3
	bne .L_02000f36_3
	ldr r0, [pc, #72]
	b 0x02008f86
.L_02000f36_3:
	ldr r3, [pc, #72]
.L_02000f68:
	cmp r2, r3
	bne .L_02000f68_0
	ldr r0, [pc, #68]
	b .L_02000f68_1
.L_02000f68_0:
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_02000f68_2
	ldr r0, [pc, #68]
	b .L_02000f68_1
.L_02000f68_2:
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_02000f68_3
	ldr r0, [pc, #64]
	b .L_02000f68_1
.L_02000f68_3:
	ldr r0, [pc, #64]
.L_02000f68_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x0034
	.2byte 0x0000
	.2byte 0xabd8
	.2byte 0x0200
	.2byte 0x003e
	.2byte 0x0000
	.2byte 0xac08
	.2byte 0x0200
	.2byte 0x003f
	.2byte 0x0000
	.2byte 0xad1c
	.2byte 0x0200
	.2byte 0x0040
	.2byte 0x0000
	.2byte 0xae24
	.2byte 0x0200
	.2byte 0x0041
	.2byte 0x0000
	.4byte 0x0200b058
	.4byte 0x00000042
	.4byte 0x0200b130
	.4byte 0x00000043
	.4byte 0x0200b184
	.4byte 0x0200abcc
	.global Func_02000fcc
	.thumb_func
Func_02000fcc:
	push {lr}
	bl 0x0200a338
.L_02000fd2:
	movs r0, #0
	bl 0x0200a350
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r0, #6]
	movs r0, #123
	bl 0x0200a410
	bl 0x0200a3d0
	bl 0x0200a3d8
	movs r0, #1
	bl 0x0200a3a0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000ff8
	.thumb_func
Func_02000ff8:
	push {lr}
	bl 0x0200a094
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001004
	.thumb_func
Func_02001004:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r7, r1, #0
	mov r8, r2
	bl 0x0200a350
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02001004_0
	movs r1, #3
	adds r0, r6, #0
	bl 0x0200a398
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
.L_02001004_0:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02001050
	.thumb_func
Func_02001050:
	push {r5, lr}
	sub sp, #8
	movs r3, #29
	str r3, [sp, #4]
	movs r0, #8
	movs r5, #8
	movs r1, #42
	movs r2, #15
	movs r3, #5
	str r5, [sp, #0]
	bl 0x0200a2f0
	ldr r0, [pc, #220]
	bl 0x0200a318
	cmp r0, #0
	beq .L_02001050_0
	movs r0, #8
	movs r1, #22
	movs r2, #31
	bl 0x02009004
	movs r3, #30
	str r3, [sp, #4]
	movs r0, #9
	movs r1, #30
	movs r2, #1
	movs r3, #3
	str r5, [sp, #0]
	bl 0x0200a2f0
	b .L_02001050_1
.L_02001050_0:
	movs r0, #8
	movs r1, #8
	movs r2, #31
	bl 0x02009004
	movs r3, #22
	movs r2, #30
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #9
	movs r1, #30
	movs r2, #1
	movs r3, #3
	bl 0x0200a2f0
.L_02001050_1:
	ldr r0, [pc, #156]
	bl 0x0200a318
	cmp r0, #0
	beq .L_02001050_2
	movs r0, #9
	movs r1, #12
	movs r2, #29
	bl 0x02009004
	movs r3, #11
	movs r2, #33
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #33
	movs r2, #3
	movs r3, #1
	bl 0x0200a2f0
	b .L_02001050_3
.L_02001050_2:
	movs r0, #9
	movs r1, #12
	movs r2, #33
	bl 0x02009004
	movs r3, #11
	movs r2, #29
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #29
	movs r2, #3
	movs r3, #1
	bl 0x0200a2f0
.L_02001050_3:
	ldr r0, [pc, #88]
	bl 0x0200a318
	cmp r0, #0
	beq .L_02001050_4
	movs r0, #10
	movs r1, #18
	movs r2, #29
	bl 0x02009004
	movs r3, #17
	movs r2, #33
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #33
	movs r2, #3
	movs r3, #1
	bl 0x0200a2f0
	b .L_02001050_5
.L_02001050_4:
	movs r0, #10
	movs r1, #18
	movs r2, #33
	bl 0x02009004
	movs r3, #17
	movs r2, #29
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #29
	movs r2, #3
	movs r3, #1
	bl 0x0200a2f0
.L_02001050_5:
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000301
	.4byte 0x00000302
	.4byte 0x00000303
	.global Func_02001154
	.thumb_func
Func_02001154:
	push {r5, r6, r7, lr}
	sub sp, #8
	movs r3, #12
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #28
	movs r2, #10
	movs r3, #18
	bl 0x0200a2f0
	movs r0, #193
	lsls r0, r0, #2
	bl 0x0200a318
	cmp r0, #0
	beq .L_02001154_0
	movs r0, #8
	movs r1, #21
	movs r2, #20
	bl 0x02009004
	movs r3, #13
	movs r2, #19
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #20
	movs r1, #19
	movs r2, #1
	movs r3, #3
	bl 0x0200a2f0
	b .L_02001154_1
.L_02001154_0:
	movs r0, #8
	movs r1, #13
	movs r2, #20
	bl 0x02009004
	movs r3, #21
	movs r2, #19
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #20
	movs r1, #19
	movs r2, #1
	movs r3, #3
	bl 0x0200a2f0
.L_02001154_1:
	ldr r0, [pc, #264]
	bl 0x0200a318
	cmp r0, #0
	beq .L_02001154_2
	movs r0, #8
	movs r1, #12
	movs r2, #20
	bl 0x02009004
	movs r5, #19
	movs r0, #5
	movs r1, #19
	movs r2, #1
	movs r3, #3
	movs r7, #12
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a2f0
	movs r6, #13
	movs r0, #20
	movs r1, #19
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a2f0
	movs r0, #193
	lsls r0, r0, #2
	bl 0x0200a318
	cmp r0, #0
	beq .L_02001154_2
	movs r0, #8
	movs r1, #21
	movs r2, #20
	bl 0x02009004
	movs r0, #20
	movs r1, #19
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a2f0
	movs r0, #20
	movs r1, #19
	movs r2, #1
	movs r3, #3
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a2f0
.L_02001154_2:
	ldr r0, [pc, #156]
	bl 0x0200a318
	cmp r0, #0
	beq .L_02001154_3
	movs r0, #9
	movs r1, #15
	movs r2, #21
	bl 0x02009004
	movs r3, #14
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #18
	movs r2, #3
	movs r3, #1
	bl 0x0200a2f0
	b .L_02001154_4
.L_02001154_3:
	movs r0, #9
	movs r1, #15
	movs r2, #17
	bl 0x02009004
	movs r3, #14
	movs r2, #21
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #18
	movs r2, #3
	movs r3, #1
	bl 0x0200a2f0
.L_02001154_4:
	ldr r0, [pc, #88]
	bl 0x0200a318
	cmp r0, #0
	beq .L_02001154_5
	movs r0, #10
	movs r1, #19
	movs r2, #8
	bl 0x02009004
	movs r3, #18
	movs r2, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #18
	movs r2, #3
	movs r3, #1
	bl 0x0200a2f0
	b .L_02001154_6
.L_02001154_5:
	movs r0, #10
	movs r1, #19
	movs r2, #25
	bl 0x02009004
	movs r3, #18
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #18
	movs r2, #3
	movs r3, #1
	bl 0x0200a2f0
.L_02001154_6:
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000305
	.4byte 0x00000306
	.4byte 0x00000307
	.global Func_020012cc
	.thumb_func
Func_020012cc:
	push {r5, lr}
	sub sp, #8
	movs r3, #12
	movs r2, #21
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #12
	movs r1, #3
.L_020012dc:
	movs r2, #9
	movs r3, #16
	bl 0x0200a2f0
	movs r0, #194
	lsls r0, r0, #2
	bl 0x0200a318
	cmp r0, #0
	beq .L_020012dc_0
	movs r0, #8
	movs r1, #14
	movs r2, #25
	bl 0x02009004
	movs r3, #20
	movs r2, #24
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #16
	movs r1, #24
	movs r2, #1
	movs r3, #3
	bl 0x0200a2f0
	b 0x0200937c
.L_020012dc_0:
	ldr r0, [pc, #680]
	bl 0x0200a318
	cmp r0, #0
	beq 0x0200935e
	movs r0, #8
	movs r1, #17
	movs r2, #25
.L_02001320:
	bl 0x02009004
	movs r3, #20
	movs r5, #24
	str r3, [sp, #0]
	movs r0, #18
	movs r1, #24
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl 0x0200a2f0
	movs r3, #14
	str r3, [sp, #0]
	movs r0, #18
	movs r1, #24
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl 0x0200a2f0
	movs r3, #17
	str r3, [sp, #0]
	movs r0, #8
	movs r1, #41
	movs r2, #1
	movs r3, #3
.L_02001356:
	str r5, [sp, #4]
	bl 0x0200a2f0
	b .L_02001356_0
	.2byte 0x2008
	.2byte 0x2114
	.2byte 0x2219
	.2byte 0xf7ff
	.2byte 0xfe4e
	.2byte 0x230e
	.2byte 0x2218
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x2010
	.2byte 0x2118
	.2byte 0x2201
	.2byte 0x2303
	.2byte 0xf000
	.2byte 0xffba
.L_02001356_0:
	ldr r0, [pc, #576]
	bl 0x0200a318
	cmp r0, #0
	beq 0x020093a6
	movs r0, #9
	movs r1, #13
	movs r2, #35
.L_0200138c:
	bl 0x02009004
	movs r3, #15
	movs r2, #34
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #34
	movs r2, #1
	movs r3, #3
	bl 0x0200a2f0
	b .L_0200138c_0
	.2byte 0x2009
	.2byte 0x210f
	.2byte 0x2223
	.2byte 0xf7ff
	.2byte 0xfe2a
	.2byte 0x230d
	.2byte 0x2222
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x200e
	.2byte 0x2122
	.2byte 0x2201
	.2byte 0x2303
	.2byte 0xf000
	.2byte 0xff96
.L_0200138c_0:
	ldr r0, [pc, #508]
	bl 0x0200a318
	cmp r0, #0
	beq .L_0200138c_1
	movs r0, #10
	movs r1, #15
	movs r2, #22
	bl 0x02009004
	movs r3, #30
	movs r5, #14
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #29
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200a2f0
	movs r3, #22
	str r3, [sp, #4]
	movs r0, #5
	movs r1, #41
	b 0x020094c0
.L_0200138c_1:
	movs r0, #195
.L_020013f8:
	lsls r0, r0, #2
	bl 0x0200a318
	cmp r0, #0
	beq .L_020013f8_0
	movs r0, #10
	movs r1, #15
	movs r2, #23
	bl 0x02009004
	movs r3, #23
	movs r5, #14
	str r3, [sp, #4]
	movs r0, #5
	movs r1, #42
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200a2f0
	movs r3, #30
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #29
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200a2f0
	movs r3, #21
	str r3, [sp, #4]
	movs r0, #10
	b .L_020013f8_1
.L_020013f8_0:
	ldr r0, [pc, #396]
	bl 0x0200a318
	cmp r0, #0
	beq .L_020013f8_2
	movs r0, #10
	movs r1, #15
	movs r2, #26
	bl 0x02009004
	movs r3, #22
	movs r5, #14
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #29
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200a2f0
	movs r3, #26
	str r3, [sp, #4]
	movs r0, #5
	movs r1, #43
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200a2f0
	movs r3, #30
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #29
	b .L_020013f8_3
.L_020013f8_2:
	ldr r0, [pc, #332]
	bl 0x0200a318
	cmp r0, #0
	beq .L_020013f8_4
	movs r0, #10
	movs r1, #15
	movs r2, #27
	bl 0x02009004
	movs r3, #22
	movs r5, #14
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #29
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200a2f0
	movs r3, #30
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #29
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200a2f0
	movs r3, #27
	str r3, [sp, #4]
	movs r0, #5
.L_020013f8_1:
	movs r1, #44
.L_020013f8_3:
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200a2f0
	b .L_020013f8_5
.L_020013f8_4:
	movs r0, #10
	movs r1, #15
	movs r2, #30
	bl 0x02009004
.L_020013f8_5:
	ldr r0, [pc, #248]
	bl 0x0200a318
	cmp r0, #0
	beq .L_020013f8_6
	movs r0, #11
	movs r1, #15
	movs r2, #23
	bl 0x02009004
	movs r3, #31
	movs r5, #14
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #29
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200a2f0
	movs r3, #23
	str r3, [sp, #4]
	movs r0, #10
	movs r1, #40
	b 0x0200959e
.L_020013f8_6:
	movs r0, #196
	lsls r0, r0, #2
	bl 0x0200a318
	cmp r0, #0
	beq 0x0200953c
	movs r0, #11
	movs r1, #15
	movs r2, #24
	bl 0x02009004
	movs r3, #31
	movs r5, #14
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #29
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200a2f0
	movs r3, #24
.L_02001534:
	str r3, [sp, #4]
	movs r0, #10
	movs r1, #41
	b 0x0200959e
	.2byte 0x4825
	.2byte 0xf000
	.2byte 0xfeeb
	.2byte 0x2800
	.2byte 0xd013
	.2byte 0x200b
	.2byte 0x210f
	.2byte 0x221b
	.2byte 0xf7ff
	.2byte 0xfd5a
	.2byte 0x231f
	.2byte 0x250e
	.2byte 0x9301
	.2byte 0x200e
	.2byte 0x211d
	.2byte 0x2203
	.2byte 0x2301
	.2byte 0x9500
	.2byte 0xf000
	.2byte 0xfec6
	.2byte 0x231b
	.2byte 0x9301
	.2byte 0x200a
	.2byte 0x212a
	.2byte 0xe017
	.2byte 0x481a
.L_02001570:
	bl 0x0200a318
	cmp r0, #0
	beq 0x020095aa
	movs r0, #11
	movs r1, #15
	movs r2, #28
	bl 0x02009004
	movs r3, #31
	movs r5, #14
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #29
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200a2f0
	movs r3, #28
	str r3, [sp, #4]
	movs r0, #10
	movs r1, #43
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200a2f0
.L_020015a8:
	b .L_020015a8_0
	.2byte 0x200b
	.2byte 0x210f
	.2byte 0x221f
	.2byte 0xf7ff
	.2byte 0xfd28
.L_020015a8_0:
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0309
	.2byte 0x0000
	.2byte 0x030a
	.2byte 0x0000
	.2byte 0x030b
	.2byte 0x0000
	.2byte 0x030d
	.2byte 0x0000
	.2byte 0x030e
	.2byte 0x0000
	.2byte 0x030f
	.2byte 0x0000
	.2byte 0x0311
	.2byte 0x0000
	.2byte 0x0312
	.2byte 0x0000
	.global Func_020015dc
	.thumb_func
Func_020015dc:
	push {lr}
	ldr r3, [pc, #288]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #1
	lsls r3, r3, #16
	lsls r2, r2, #9
	sub sp, #8
	cmp r3, r2
	bhi 0x0200960c
	movs r3, #14
	movs r2, #10
	str r3, [sp, #0]
.L_020015fc:
	str r2, [sp, #4]
	movs r0, #22
	movs r1, #20
	movs r2, #9
	movs r3, #8
	bl 0x0200a2f0
	b .L_020015fc_0
	.2byte 0x2307
	.2byte 0x222d
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x2014
	.2byte 0x212d
	.2byte 0x220b
	.2byte 0x2304
	.2byte 0xf000
	.2byte 0xfe68
.L_020015fc_0:
	ldr r0, [pc, #224]
	bl 0x0200a318
	cmp r0, #0
	beq 0x0200964a
	movs r0, #8
	movs r1, #20
	movs r2, #17
	bl 0x02009004
	movs r3, #19
.L_02001636:
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #19
	movs r1, #11
	movs r2, #3
	movs r3, #1
	bl 0x0200a2f0
	b .L_02001636_0
	.2byte 0x2008
	.2byte 0x2114
	.2byte 0x220a
	.2byte 0xf7ff
	.2byte 0xfcd8
	.2byte 0x2313
	.2byte 0x2211
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x2013
	.2byte 0x210b
	.2byte 0x2203
	.2byte 0x2301
	.2byte 0xf000
	.2byte 0xfe44
.L_02001636_0:
	movs r0, #197
	lsls r0, r0, #2
.L_0200166c:
	bl 0x0200a318
	cmp r0, #0
	beq .L_0200166c_0
	movs r0, #9
	movs r1, #14
	movs r2, #16
	bl 0x02009004
	movs r3, #22
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #16
	movs r1, #15
	movs r2, #1
	movs r3, #3
	bl 0x0200a2f0
	b 0x020096b2
.L_0200166c_0:
	movs r0, #9
	movs r1, #22
	movs r2, #16
	bl 0x02009004
	movs r3, #14
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #16
	movs r1, #15
	movs r2, #1
.L_020016ac:
	movs r3, #3
	bl 0x0200a2f0
	ldr r0, [pc, #84]
	bl 0x0200a318
	cmp r0, #0
	beq .L_020016ac_0
	movs r0, #10
	movs r1, #17
	movs r2, #46
	bl 0x02009004
	movs r3, #7
	movs r2, #45
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #15
	movs r1, #15
	movs r2, #1
	movs r3, #3
	bl 0x0200a2f0
	b .L_020016ac_1
.L_020016ac_0:
	movs r0, #10
	movs r1, #7
	movs r2, #46
	bl 0x02009004
	movs r3, #17
	movs r2, #45
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #15
	movs r1, #15
	movs r2, #1
	movs r3, #3
	bl 0x0200a2f0
.L_020016ac_1:
	sub sp, #-8
.L_020016fc:
	pop {r0}
	bx r0
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x0313
	.2byte 0x0000
	.2byte 0x0315
	.2byte 0x0000
	.global Func_0200170c
	.thumb_func
Func_0200170c:
	push {r5, lr}
	ldr r3, [pc, #24]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200170c_0
	ldr r3, [pc, #20]
	movs r0, #0
	ldr r5, [r3]
	bl 0x0200a350
	str r0, [r5, #24]
.L_0200170c_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0200b328
	.4byte 0x03001ee0
	.global Func_02001730
	.thumb_func
Func_02001730:
	push {lr}
	ldr r3, [pc, #20]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02001730_0
	ldr r3, [pc, #16]
	ldr r2, [r3]
	movs r3, #0
	str r3, [r2, #24]
.L_02001730_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200b328
	.4byte 0x03001ee0
	.global Func_02001750
	.thumb_func
Func_02001750:
	push {lr}
	ldr r3, [pc, #68]
	movs r1, #224
	ldr r3, [r3]
	lsls r1, r1, #1
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	str r2, [r3]
	ldr r0, [pc, #56]
	bl 0x0200a318
	cmp r0, #0
	bne .L_02001750_0
	ldr r3, [pc, #48]
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02001750_0
	movs r0, #162
	lsls r0, r0, #1
	bl 0x0200a320
	bl 0x020097a8
	b .L_02001750_1
.L_02001750_0:
	bl 0x020097e4
.L_02001750_1:
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000109
	.4byte 0x02000240
	.4byte 0x00000034
	.global Func_020017a8
	.thumb_func
Func_020017a8:
	push {lr}
	bl 0x0200a338
	movs r0, #8
	bl 0x0200a350
	movs r1, #0
	bl 0x0200a2f8
	bl 0x0200a3c8
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #10
	ldr r2, [pc, #24]
	bl 0x0200a358
	movs r1, #132
	movs r0, #0
	lsls r1, r1, #1
.L_020017d0:
	movs r2, #196
	bl 0x0200a360
	bl 0x0200a340
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x1999
	.2byte 0x0000
	.global Func_020017e4
	.thumb_func
Func_020017e4:
	push {r5, r6, r7, lr}
	sub sp, #8
	bl 0x02009948
	ldr r6, [pc, #312]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #304]
	cmp r2, r3
	bne .L_020017e4_0
	ldr r0, [pc, #304]
	bl 0x0200a318
	cmp r0, #0
	bne .L_020017e4_1
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #1
	bne .L_020017e4_1
	bl 0x02009b1c
.L_020017e4_1:
	ldr r3, [pc, #268]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r1, #192
	subs r3, #2
	lsls r3, r3, #16
	lsls r1, r1, #10
	cmp r3, r1
	bhi .L_020017e4_2
	movs r5, #226
	lsls r5, r5, #17
	movs r0, #156
	movs r1, #0
	adds r2, r5, #0
	movs r3, #223
	lsls r0, r0, #16
	bl 0x02008ed8
	movs r0, #188
	lsls r0, r0, #16
	movs r1, #0
	adds r2, r5, #0
	movs r3, #223
	bl 0x02008ed8
	b .L_020017e4_2
.L_020017e4_0:
	ldr r3, [pc, #224]
	cmp r2, r3
	bne .L_020017e4_2
	movs r0, #8
	bl 0x0200a350
	ldr r7, [pc, #216]
	adds r3, r0, #0
	adds r3, #85
	movs r5, #0
	str r5, [r7]
	movs r1, #1
	strb r5, [r3]
	str r5, [r0, #12]
	movs r0, #8
	bl 0x0200a398
	movs r1, #15
	movs r0, #8
	bl 0x0200a388
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #1
	blt .L_020017e4_3
	cmp r3, #2
	ble .L_020017e4_4
	cmp r3, #5
	beq .L_020017e4_5
	b .L_020017e4_3
.L_020017e4_4:
	movs r0, #0
	bl 0x0200a3f0
	movs r3, #1
	str r3, [r7]
	b .L_020017e4_3
.L_020017e4_5:
	movs r0, #0
	bl 0x0200a3f0
	movs r3, #1
	str r3, [r7]
	ldr r3, [pc, #144]
	ldr r5, [r3]
	movs r3, #0
	str r3, [r5, #24]
.L_020017e4_3:
	ldr r3, [pc, #116]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #6
	bgt .L_020017e4_2
	movs r0, #130
	lsls r0, r0, #4
	bl 0x0200a318
	cmp r0, #0
	beq .L_020017e4_6
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #30
	movs r1, #57
	movs r2, #19
	movs r3, #57
	bl 0x0200a2d8
	movs r2, #7
	movs r3, #8
	str r2, [sp, #4]
	movs r0, #30
	movs r1, #8
	movs r2, #12
	str r3, [sp, #0]
	bl 0x0200a2d8
	b .L_020017e4_2
.L_020017e4_6:
	ldr r3, [pc, #72]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	ldr r0, [pc, #64]
	movs r1, #1
	bl 0x0200a3b8
	ldr r0, [pc, #56]
	movs r1, #1
	bl 0x0200a3b0
	movs r0, #1
	bl 0x0200a3c0
	movs r0, #1
	bl 0x0200a260
.L_020017e4_2:
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000040
	.4byte 0x00000f13
	.4byte 0x00000043
	.4byte 0x0200b328
	.4byte 0x03001ee0
	.4byte 0x03001ebc
	.4byte 0x00203108
	.global Func_02001948
	.thumb_func
Func_02001948:
	push {lr}
	ldr r1, [pc, #212]
	movs r0, #224
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, [pc, #204]
	sub sp, #8
	cmp r2, r3
	bne .L_02001948_0
	movs r3, #8
	movs r2, #42
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #8
	movs r1, #29
	movs r2, #15
	movs r3, #5
	bl 0x0200a2f0
	bl 0x02009050
	b .L_02001948_1
.L_02001948_0:
	ldr r3, [pc, #172]
	cmp r2, r3
	bne .L_02001948_2
	movs r3, #0
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #12
	movs r1, #8
	movs r2, #10
	movs r3, #18
	bl 0x0200a2f0
	bl 0x02009154
	b .L_02001948_1
.L_02001948_2:
	ldr r3, [pc, #144]
	cmp r2, r3
	bne .L_02001948_3
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #1
	beq .L_02001948_3
	movs r3, #12
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #12
	movs r1, #21
	movs r2, #9
	movs r3, #16
	bl 0x0200a2f0
	bl 0x020092cc
	b .L_02001948_1
.L_02001948_3:
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, [pc, #92]
	cmp r2, r3
	bne .L_02001948_1
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	movs r0, #128
	subs r3, #1
	lsls r3, r3, #16
	lsls r0, r0, #9
	cmp r3, r0
	bhi .L_02001948_4
	movs r3, #22
	movs r2, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #10
	movs r2, #9
	movs r3, #8
	bl 0x0200a2f0
	b .L_02001948_5
.L_02001948_4:
	movs r3, #20
	movs r2, #45
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #7
	movs r1, #45
	movs r2, #11
	movs r3, #4
	bl 0x0200a2f0
.L_02001948_5:
	bl 0x020095dc
.L_02001948_1:
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000003e
	.4byte 0x0000003f
	.4byte 0x00000040
	.4byte 0x00000041
	.global Func_02001a34
	.thumb_func
Func_02001a34:
	push {r5, r6, lr}
	ldr r3, [pc, #100]
	movs r1, #182
	ldr r3, [r3]
	lsls r1, r1, #1
	ldr r5, [pc, #96]
	adds r3, r3, r1
	adds r1, #84
	movs r2, #0
	ldrsh r6, [r3, r2]
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #84]
	cmp r2, r3
	bne .L_02001a34_0
	cmp r6, #17
	bne .L_02001a34_1
	movs r1, #32
	negs r1, r1
	movs r0, #0
	bl 0x02009ad0
	b .L_02001a34_0
.L_02001a34_1:
	movs r0, #32
	negs r0, r0
	movs r1, #0
	bl 0x02009ad0
.L_02001a34_0:
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_02001a34_2
	cmp r6, #25
	bne .L_02001a34_2
	ldr r0, [pc, #40]
	bl 0x0200a318
	cmp r0, #0
	beq .L_02001a34_2
	movs r0, #0
	movs r1, #32
	bl 0x02009ad0
.L_02001a34_2:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000003f
	.4byte 0x00000040
	.4byte 0x00000309
	.global Func_02001ab0
	.thumb_func
Func_02001ab0:
	push {lr}
	movs r1, #32
	negs r1, r1
	movs r0, #0
	bl 0x02009ad0
	pop {r0}
	bx r0
	.global Func_02001ac0
	.thumb_func
Func_02001ac0:
	push {lr}
	movs r0, #32
	negs r0, r0
	movs r1, #0
	bl 0x02009ad0
	pop {r0}
	bx r0
	.global Func_02001ad0
	.thumb_func
Func_02001ad0:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	bl 0x0200a338
	movs r1, #160
	movs r2, #160
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200a358
	adds r1, r5, #0
	adds r2, r6, #0
	movs r0, #0
	bl 0x0200a368
	movs r2, #0
	movs r0, #0
	movs r1, #4
	bl 0x0200a380
	movs r1, #7
	movs r0, #0
	bl 0x0200a378
	movs r0, #0
	bl 0x0200a370
	movs r0, #0
	movs r1, #6
	bl 0x0200a378
	bl 0x0200a340
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02001b1c
	.thumb_func
Func_02001b1c:
	push {r5, r6, r7, lr}
	movs r1, #248
	movs r2, #128
	movs r3, #152
	movs r0, #22
	lsls r1, r1, #16
	lsls r2, r2, #12
	lsls r3, r3, #16
	bl 0x0200a2b8
	adds r7, r0, #0
	movs r5, #0
	cmp r7, #0
	beq 0x02009b90
	ldr r6, [r7, #80]
	adds r3, r6, #0
	adds r3, #38
	strb r5, [r3]
	adds r3, #1
	strb r5, [r3]
	movs r3, #33
	ldrb r2, [r6, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	strb r3, [r6, #9]
	adds r3, r7, #0
	adds r3, #85
	adds r2, r7, #0
	strb r5, [r3]
	adds r2, #92
	movs r3, #1
	movs r1, #193
	strb r3, [r2]
	lsls r1, r1, #3
	movs r0, #17
	bl 0x0200a288
	adds r5, r0, #0
	movs r0, #230
	bl 0x0200a308
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	movs r1, #128
.L_02001b7e:
	adds r2, r5, #0
	ldrb r0, [r6, #28]
	bl 0x0200a298
	movs r0, #17
	bl 0x0200a290
	ldr r3, [pc, #8]
	str r7, [r3]
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200a488
	.global Func_02001b9c
	.thumb_func
Func_02001b9c:
	push {r5, lr}
	bl 0x0200a338
	ldr r5, [pc, #48]
	ldr r0, [r5]
	cmp r0, #0
	beq .L_02001b9c_0
	movs r1, #3
	bl 0x0200a3e0
.L_02001b9c_0:
	movs r1, #0
	movs r0, #230
	bl 0x0200a348
	ldr r0, [pc, #28]
	bl 0x0200a320
	ldr r0, [r5]
	cmp r0, #0
	beq .L_02001b9c_1
	bl 0x0200a2c0
.L_02001b9c_1:
	bl 0x0200a340
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200a488
	.4byte 0x00000f13
	.global Func_02001bdc
	.thumb_func
Func_02001bdc:
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
	bx lr
	.2byte 0x0000
	.global Func_02001c08
	.thumb_func
Func_02001c08:
	push {lr}
	movs r1, #15
	bl 0x0200a390
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02001c18
	.thumb_func
Func_02001c18:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, [sp, #52]
	ldr r1, [sp, #56]
	mov r9, sp
	mov r11, r3
	ldr r3, [pc, #308]
	adds r6, r2, #0
	mov r2, r9
	mov r8, r0
	mov r10, r1
	ldmia r3!, {r0, r1, r7}
	stmia r2!, {r0, r1, r7}
	adds r3, r6, #0
	movs r0, #222
	adds r1, r4, #0
	adds r2, r5, #0
	bl 0x0200a2b8
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02001c18_0
	b .L_02001c18_1
.L_02001c18_0:
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	ldr r7, [r6, #80]
	bl 0x0200a2a0
	mov r3, r8
	ands r3, r5
	lsls r5, r3, #2
	mov r2, r9
	ldr r1, [r2, r5]
	adds r0, r6, #0
	bl 0x0200a2b0
	adds r3, r6, #0
	movs r2, #0
	adds r3, #85
	strb r2, [r3]
	adds r3, r7, #0
	adds r3, #38
	strb r2, [r3]
	ldr r3, [pc, #236]
	str r3, [r6, #108]
	mov r3, r11
	str r3, [r6, #68]
	ldr r3, [sp, #44]
	str r3, [r6, #72]
	ldr r3, [sp, #48]
	movs r0, #13
	str r3, [r6, #76]
	str r2, [r6, #48]
	str r2, [r6, #52]
	negs r0, r0
	ldrb r2, [r7, #9]
	mov r11, r0
	mov r3, r11
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r7, #9]
	ldr r3, [pc, #200]
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02001c18_1
	mov r2, r10
	cmp r2, #0
	beq .L_02001c18_1
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_02001c18_2
	ldr r1, [r2, #4]
	adds r0, r6, #0
	bl 0x0200a390
.L_02001c18_2:
	movs r3, #128
	lsls r3, r3, #10
	mov r0, r8
	ands r3, r0
	cmp r3, #0
	beq .L_02001c18_3
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	mov r1, r10
	ldrb r2, [r1]
	movs r3, #3
	ldrb r1, [r7, #9]
	ands r2, r3
	mov r3, r11
	lsls r2, r2, #2
	ands r3, r1
	orrs r3, r2
	strb r3, [r7, #9]
.L_02001c18_3:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_02001c18_4
	mov r7, r10
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02001c18_4:
	movs r3, #128
	lsls r3, r3, #11
	mov r0, r8
	ands r3, r0
	cmp r3, #0
	beq .L_02001c18_1
	mov r1, r9
	ldr r5, [r1, r5]
	cmp r2, #0
	beq .L_02001c18_5
	mov r2, r10
	ldr r0, [r2, #16]
	ldr r3, [r6, #24]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	bl 0x0200a258
	str r0, [r6, #48]
	mov r3, r10
	ldr r0, [r3, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_02001c18_6
.L_02001c18_5:
	mov r7, r10
	ldr r0, [r7, #16]
	ldr r1, [pc, #48]
	adds r0, r0, r1
	ldr r1, [r5, #12]
	bl 0x0200a258
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r2, [pc, #36]
	ldr r1, [r5, #12]
	adds r0, r0, r2
.L_02001c18_6:
	bl 0x0200a258
	str r0, [r6, #52]
.L_02001c18_1:
	sub sp, #-12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200a418
	.4byte 0x02009bdd
	.4byte 0xffff0000
	.global Func_02001d78
	.thumb_func
Func_02001d78:
	push {r5, r6, r7, lr}
	ldr r2, [pc, #124]
	ldr r7, [r2]
	movs r3, #3
	ands r7, r3
	sub sp, #40
	cmp r7, #0
	bne .L_02001d78_0
	add r6, sp, #16
	movs r3, #10
	str r3, [r6, #4]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r6, #8]
	str r3, [r6, #12]
	ldr r3, [pc, #100]
	str r3, [r6, #16]
	str r3, [r6, #20]
	ldr r3, [r2]
	movs r2, #7
	ands r3, r2
	cmp r3, #0
	bne .L_02001d78_1
	movs r0, #136
	bl 0x0200a410
.L_02001d78_1:
	bl 0x0200a278
	lsls r0, r0, #1
	lsrs r0, r0, #16
	ldr r5, [pc, #72]
	lsls r0, r0, #16
	subs r5, r5, r0
	bl 0x0200a278
	lsls r2, r0, #1
	adds r2, r2, r0
	lsrs r2, r2, #16
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r2, r3, #4
	adds r3, r3, r2
	lsls r2, r3, #8
	adds r3, r3, r2
	negs r3, r3
	str r3, [sp, #0]
	ldr r3, [pc, #44]
	movs r0, #154
	movs r1, #128
	movs r2, #222
	str r3, [sp, #8]
	lsls r0, r0, #17
	lsls r1, r1, #15
	lsls r2, r2, #16
	adds r3, r5, #0
	str r7, [sp, #4]
	str r6, [sp, #12]
	bl 0x02009c18
.L_02001d78_0:
	sub sp, #-40
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e40
	.4byte 0x0001cccc
	.4byte 0xffff0000
	.4byte 0x000d0001
	.global Func_02001e08
	.thumb_func
Func_02001e08:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #19
	sub sp, #8
	bl 0x0200a410
	movs r0, #182
	bl 0x0200a410
	bl 0x0200a338
	bl 0x0200a3f8
	movs r3, #8
	movs r5, #0
	mov r8, r3
	movs r7, #7
	movs r6, #1
	ldr r0, [pc, #196]
	movs r1, #1
	bl 0x0200a3b0
	movs r0, #1
	bl 0x0200a3c0
	movs r0, #2
	bl 0x0200a260
	cmp r5, #0
	bne .L_02001e08_0
	mov r3, r8
	str r3, [sp, #0]
	movs r0, #30
	movs r1, #8
	movs r2, #12
	movs r3, #8
	str r7, [sp, #4]
	bl 0x0200a2d8
	movs r0, #30
	movs r1, #57
	movs r2, #19
	movs r3, #57
	str r6, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200a2d8
.L_02001e08_0:
	movs r1, #1
	ldr r0, [pc, #140]
	bl 0x0200a3b0
	movs r0, #1
	bl 0x0200a3c0
	adds r5, #1
.L_02001e78:
	movs r0, #2
	bl 0x0200a260
	cmp r5, #3
	bls 0x02009e2e
	movs r0, #30
	bl 0x0200a260
	ldr r5, [pc, #112]
	movs r1, #200
	lsls r1, r1, #4
	adds r0, r5, #0
	bl 0x0200a268
	movs r0, #40
	bl 0x0200a260
	movs r1, #1
	ldr r0, [pc, #96]
	bl 0x0200a3b0
	movs r0, #40
	bl 0x0200a3c0
	movs r0, #80
	bl 0x0200a260
	adds r0, r5, #0
	bl 0x0200a270
	movs r0, #20
	bl 0x0200a260
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl 0x0200a3b0
	movs r0, #80
	bl 0x0200a3c0
	movs r0, #80
	bl 0x0200a260
	movs r0, #130
	lsls r0, r0, #4
	bl 0x0200a320
	movs r0, #230
	bl 0x0200a330
	bl 0x0200a400
	bl 0x0200a340
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x4318
	.2byte 0x0020
	.2byte 0x3108
	.2byte 0x0020
	.4byte 0x02009d79
	.4byte 0x00201090
	.global Func_02001f04
	.thumb_func
Func_02001f04:
	push {lr}
	bl 0x0200a338
	ldr r0, [pc, #16]
	movs r1, #1
	bl 0x0200a300
	bl 0x0200a340
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000017e1
	.global Func_02001f20
	.thumb_func
Func_02001f20:
	push {lr}
	bl 0x0200a338
	ldr r0, [pc, #16]
	movs r1, #1
	bl 0x0200a300
	bl 0x0200a340
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000017e2
	.global Func_02001f3c
	.thumb_func
Func_02001f3c:
	push {lr}
	bl 0x0200a338
	ldr r0, [pc, #16]
	movs r1, #1
	bl 0x0200a300
	bl 0x0200a340
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000017e3
	.global Func_02001f58
	.thumb_func
Func_02001f58:
	push {lr}
	bl 0x0200a338
	movs r0, #130
	lsls r0, r0, #4
	bl 0x0200a318
	cmp r0, #0
.L_02001f68:
	beq .L_02001f68_0
	ldr r0, [pc, #52]
	movs r1, #1
	bl 0x0200a300
	b 0x02009f98
.L_02001f68_0:
	movs r1, #1
	ldr r0, [pc, #44]
	bl 0x0200a300
	movs r0, #230
	bl 0x0200a310
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	beq 0x02009f98
	ldr r3, [pc, #28]
	movs r1, #185
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
.L_02001f98:
	bl 0x0200a340
	pop {r0}
	bx r0
	.2byte 0x17e5
	.2byte 0x0000
	.2byte 0x17e4
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.global Func_02001fac
	.thumb_func
Func_02001fac:
	push {lr}
	ldr r3, [pc, #80]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #6
	bgt .L_02001fac_0
	ldr r3, [pc, #68]
	ldr r2, [r3]
	movs r0, #1
	subs r3, #100
	adds r2, #52
	ldr r1, [r3]
	strb r0, [r2]
	ldr r2, [pc, #56]
	movs r4, #0
	adds r3, r1, r2
	subs r2, #2
	strb r4, [r3]
	adds r3, r1, r2
	strb r0, [r3]
	ldr r3, [pc, #48]
	adds r1, r1, r3
	strb r0, [r1]
	movs r0, #0
	movs r1, #1
	bl 0x0200a3b8
	ldr r0, [pc, #36]
	movs r1, #1
	bl 0x0200a3b0
	movs r0, #16
	bl 0x0200a3c0
	movs r0, #16
	bl 0x0200a260
.L_02001fac_0:
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x03001f30
	.4byte 0x0000053e
	.4byte 0x0000053d
	.4byte 0x00203108
	.global Func_02002014
	.thumb_func
Func_02002014:
	push {r5, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #100
	movs r0, #0
	ldrsh r1, [r2, r0]
	ldrh r3, [r2]
	cmp r1, #0
	beq .L_02002014_0
	subs r3, #1
	strh r3, [r2]
	b 0x0200a082
.L_02002014_0:
	adds r3, r5, #0
	adds r3, #90
	strb r1, [r3]
	ldr r3, [pc, #84]
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	ldr r1, [pc, #76]
	lsls r3, r3, #1
	movs r0, #1
	ldrsh r3, [r1, r3]
	negs r0, r0
	cmp r3, r0
	bne .L_02002014_1
	adds r0, r5, #0
	movs r1, #9
	bl 0x0200a2a0
	b 0x0200a082
.L_02002014_1:
	ldrh r1, [r5, #6]
	subs r3, r3, r1
	lsls r3, r3, #16
	movs r2, #128
	asrs r3, r3, #16
	lsls r2, r2, #5
	cmp r3, r2
	ble .L_02002014_2
	adds r3, r2, #0
.L_02002014_2:
	ldr r2, [pc, #40]
	cmp r3, r2
	bge .L_02002014_3
	adds r3, r2, #0
.L_02002014_3:
	adds r3, r1, r3
	adds r0, r5, #0
	movs r1, #2
	strh r3, [r5, #6]
	bl 0x0200a2a0
	adds r0, r5, #0
	movs r1, #48
.L_0200207e:
	bl 0x0200a2a8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x1ae8
	.2byte 0x0300
	.2byte 0xa424
	.2byte 0x0200
	.2byte 0xf000
	.2byte 0xffff
	.global Func_02002094
	.thumb_func
Func_02002094:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
.L_0200209c:
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #408]
	movs r0, #250
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #20
	bl 0x0200a3e8
	adds r6, r0, #0
	ldr r3, [pc, #392]
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	ldr r1, [pc, #388]
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
.L_020020c4:
	str r2, [sp, #4]
	lsls r3, r2, #16
	ldr r2, [pc, #380]
	cmp r3, r2
	bne .L_020020c4_0
	b 0x0200a22a
.L_020020c4_0:
	bl 0x0200a338
	ldr r2, [r6, #8]
	ldr r1, [pc, #372]
	movs r3, #128
	lsls r3, r3, #12
	mov r11, r3
	ands r2, r1
	add r5, sp, #8
	add r2, r11
.L_020020e4:
	str r2, [r5]
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	ands r3, r1
	add r3, r11
	str r3, [r5, #8]
	movs r0, #34
	mov r9, r3
	mov r10, r2
	adds r0, r0, r6
	mov r8, r0
	mov r1, r10
	mov r2, r9
	ldrb r0, [r0]
	bl 0x0200a2e8
	str r0, [sp, #0]
	movs r0, #128
	ldr r1, [sp, #4]
.L_0200210c:
	lsls r0, r0, #13
	adds r2, r5, #0
	bl 0x0200a280
	mov r2, r8
	ldrb r0, [r2]
	ldr r1, [r5]
	ldr r2, [r5, #8]
	bl 0x0200a2e8
	adds r7, r0, #0
	cmp r7, #255
	beq 0x0200a17c
	mov r3, r8
	ldrb r0, [r3]
	ldr r1, [r5]
.L_0200212c:
	ldr r2, [r5, #8]
	bl 0x0200a2e0
	ldr r3, [r6, #12]
	subs r0, r0, r3
	cmp r0, r11
	bgt .L_0200212c_0
	movs r3, #128
	mov r0, r10
	mov r2, r9
	lsls r3, r3, #10
	str r0, [r5]
	str r2, [r5, #8]
	str r3, [r6, #48]
	ldr r3, [pc, #260]
	adds r2, r6, #0
	str r3, [r6, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	adds r0, r6, #0
	mov r3, r9
	ldr r2, [r6, #12]
	mov r1, r10
	bl 0x0200a2c8
	adds r0, r6, #0
	movs r1, #2
	bl 0x0200a2a0
	adds r0, r6, #0
	movs r1, #48
	bl 0x0200a2a8
	adds r0, r6, #0
	bl 0x0200a2d0
	ldr r3, [pc, #220]
	str r3, [r6, #108]
	b 0x0200a1c6
.L_0200212c_0:
	add r3, sp, #4
	ldrh r3, [r3]
	strh r3, [r6, #6]
	b 0x0200a220
.L_02002184:
	mov r2, r8
	ldrb r0, [r2]
	ldr r1, [r5]
	ldr r2, [r5, #8]
	bl 0x0200a2e0
	ldr r3, [r6, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt 0x0200a1e4
	movs r3, #128
	lsls r3, r3, #10
	ldr r0, [r5]
	ldr r2, [r5, #8]
.L_020021a4:
	str r3, [r6, #48]
	ldr r3, [pc, #168]
	str r3, [r6, #52]
	mov r10, r0
	ldr r3, [r5, #8]
	ldr r1, [r5]
	adds r0, r6, #0
	mov r9, r2
	ldr r2, [r5, #4]
	bl 0x0200a2c8
	adds r0, r6, #0
	bl 0x0200a2d0
	ldr r3, [sp, #0]
	cmp r7, r3
	bne 0x0200a20a
	movs r0, #128
	ldr r1, [sp, #4]
	add r2, sp, #8
.L_020021cc:
	lsls r0, r0, #13
	bl 0x0200a280
	mov r2, r8
	ldrb r0, [r2]
	ldr r1, [r5]
	ldr r2, [r5, #8]
	bl 0x0200a2e8
	adds r7, r0, #0
	cmp r7, #255
	bne 0x0200a184
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r6, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #52]
	ldr r2, [r6, #12]
	adds r0, r6, #0
	mov r1, r10
	mov r3, r9
	bl 0x0200a2c8
	adds r0, r6, #0
	bl 0x0200a2d0
	movs r0, #2
	bl 0x0200a260
.L_02002208:
	b 0x0200a0b4
	.2byte 0x2300
	.2byte 0x66f3
	.2byte 0x1c31
	.2byte 0x315a
	.2byte 0x780a
	.2byte 0x2301
	.2byte 0x4313
	.2byte 0x700b
	.2byte 0x2380
	.2byte 0x01db
	.2byte 0x6373
	.2byte 0x200a
	.2byte 0xf000
	.2byte 0xf81d
	.2byte 0xf000
	.2byte 0xf88b
	.2byte 0xb005
	.2byte 0xbce8
	.2byte 0x4698
	.2byte 0x46a9
	.2byte 0x46b2
	.2byte 0x46bb
	.2byte 0xbce0
	.2byte 0xbc01
	.2byte 0x4700
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x1ae8
	.2byte 0x0300
	.2byte 0xa464
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0x1999
	.2byte 0x0000
	.2byte 0xa015
	.2byte 0x0200
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/IMPORT.INC"
	.4byte 0x0200b268
	.4byte 0x0200b2a8
	.4byte 0x0200b2e8
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xe000c000
	.4byte 0xc000a000
	.4byte 0x20004000
	.4byte 0x40006000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xffffc000
	.4byte 0xc000ffff
	.4byte 0xffff4000
	.4byte 0x4000ffff
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000c000
	.4byte 0xc0008000
	.4byte 0x00004000
	.4byte 0x40008000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00000100
	.4byte 0x40000258
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
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000108
	.4byte 0xc0000121
	.4byte 0x00870000
	.4byte 0x0178003e
	.4byte 0x00000148
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000f8
	.4byte 0x40000258
	.4byte 0x00400000
	.4byte 0x01a00040
	.4byte 0x00000290
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0xc0000278
	.4byte 0x00400000
	.4byte 0x01a00170
	.4byte 0x00000290
	.4byte 0xffff0002
	.4byte 0x00000178
	.4byte 0x400001e8
	.4byte 0x00400000
	.4byte 0x01a00170
	.4byte 0x00000290
	.4byte 0xffff0003
	.4byte 0x00000078
	.4byte 0x400001e8
	.4byte 0x00400000
	.4byte 0x01a00170
	.4byte 0x00000290
	.4byte 0xffff0004
	.4byte 0x000000e8
	.4byte 0x40000198
	.4byte 0x00400000
	.4byte 0x01a00170
	.4byte 0x00000290
	.4byte 0xffff0005
	.4byte 0x000000e8
	.4byte 0xc0000158
	.4byte 0x00400000
	.4byte 0x01a00030
	.4byte 0x00000178
	.4byte 0xffff0006
	.4byte 0x00000178
	.4byte 0xc0000098
	.4byte 0x00400000
	.4byte 0x01a00030
	.4byte 0x00000178
	.4byte 0xffff0007
	.4byte 0x00000078
	.4byte 0x400000c8
	.4byte 0x00400000
	.4byte 0x01a00030
	.4byte 0x00000178
	.4byte 0xffff0008
	.4byte 0x000000f8
	.4byte 0x40000068
	.4byte 0x00400000
	.4byte 0x01a00030
	.4byte 0x00000178
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0x40000078
	.4byte 0x00600000
	.4byte 0x01a00040
	.4byte 0x000001e0
	.4byte 0xffff0002
	.4byte 0x00000098
	.4byte 0xc00000c8
	.4byte 0x00600000
	.4byte 0x01a00040
	.4byte 0x000001e0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0x40000068
	.4byte 0x00800000
	.4byte 0x01700040
	.4byte 0x00000100
	.4byte 0xffff0002
	.4byte 0x000000b8
	.4byte 0x40000148
	.4byte 0x00600000
	.4byte 0x01a00100
	.4byte 0x00000290
	.4byte 0xffff0003
	.4byte 0x00000098
	.4byte 0x400001b0
	.4byte 0x00600000
	.4byte 0x01a00100
	.4byte 0x00000290
	.4byte 0xffff0004
	.4byte 0x000000b8
	.4byte 0x40000228
	.4byte 0x00600000
	.4byte 0x01a00100
	.4byte 0x00000290
	.4byte 0xffff0005
	.4byte 0x00000118
	.4byte 0xc0000268
	.4byte 0x00600000
	.4byte 0x01a00100
	.4byte 0x00000290
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000148
	.4byte 0x40000048
	.4byte 0x00400000
	.4byte 0x01a00010
	.4byte 0x000001a0
	.4byte 0xffff0002
	.4byte 0x000000f8
	.4byte 0xc0000188
	.4byte 0x00400000
	.4byte 0x01a00010
	.4byte 0x000001a0
	.4byte 0xffff0003
	.4byte 0x000000f8
	.4byte 0x400001d8
	.4byte 0x00400000
	.4byte 0x01a001a0
	.4byte 0x00000320
	.4byte 0xffff0004
	.4byte 0x00000158
	.4byte 0xc0000278
	.4byte 0x00400000
	.4byte 0x01a001a0
	.4byte 0x00000320
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000108
	.4byte 0xc00000e8
	.4byte 0x00800000
	.4byte 0x01800010
	.4byte 0x00000100
	.4byte 0xffff0002
	.4byte 0x000000f8
	.4byte 0x400001d8
	.4byte 0x00400000
	.4byte 0x01a00100
	.4byte 0x000002d0
	.4byte 0xffff0003
	.4byte 0x000000d8
	.4byte 0xc0000298
	.4byte 0x00400000
	.4byte 0x01a00100
	.4byte 0x000002d0
	.4byte 0xffff0004
	.4byte 0x00000118
	.4byte 0xc0000298
	.4byte 0x00400000
	.4byte 0x01a00100
	.4byte 0x000002d0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000118
	.4byte 0x40000078
	.4byte 0x00400000
	.4byte 0x01a00000
	.4byte 0x00000158
	.4byte 0xffff0002
	.4byte 0x000000d8
	.4byte 0x40000078
	.4byte 0x00400000
	.4byte 0x01a00000
	.4byte 0x00000158
	.4byte 0xffff0003
	.4byte 0x00000068
	.4byte 0x40000118
	.4byte 0x00400000
	.4byte 0x01a00000
	.4byte 0x00000158
	.4byte 0xffff0004
	.4byte 0x000000c8
	.4byte 0xc0000158
	.4byte 0x00400000
	.4byte 0x01a00010
	.4byte 0x00000158
	.4byte 0xffff0005
	.4byte 0x000000f8
	.4byte 0xc0000138
	.4byte 0x00400000
	.4byte 0x01a00010
	.4byte 0x00000158
	.4byte 0xffff0006
	.4byte 0x00000128
	.4byte 0xc0000158
	.4byte 0x00400000
	.4byte 0x01a00010
	.4byte 0x00000158
	.4byte 0xffff0007
	.4byte 0x000000c8
	.4byte 0x400001b8
	.4byte 0x00200000
	.4byte 0x01b00158
	.4byte 0x000002e0
	.4byte 0xffff0008
	.4byte 0x000000f8
	.4byte 0x400001b8
	.4byte 0x00200000
	.4byte 0x01b00158
	.4byte 0x000002e0
	.4byte 0xffff0009
	.4byte 0x00000128
	.4byte 0x400001a8
	.4byte 0x00200000
	.4byte 0x01b00158
	.4byte 0x000002e0
	.4byte 0xffff000a
	.4byte 0x00000178
	.4byte 0x400001d8
	.4byte 0x00200000
	.4byte 0x01b00158
	.4byte 0x000002e0
	.4byte 0xffff000b
	.4byte 0x000000f8
	.4byte 0xc0000218
	.4byte 0x00200000
	.4byte 0x01b00158
	.4byte 0x000002e0
	.4byte 0xffff000c
	.4byte 0x00000078
	.4byte 0x400002b8
	.4byte 0x00200000
	.4byte 0x01b00158
	.4byte 0x000002f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000034
	.4byte 0x00109032
	.4byte 0x0000003e
	.4byte 0x0010303c
	.4byte 0x0020203f
	.4byte 0x00304041
	.4byte 0x0040503e
	.4byte 0x0050403e
	.4byte 0x00604040
	.4byte 0x0070c043
	.4byte 0x0080b043
	.4byte 0x0000003f
	.4byte 0x00105040
	.4byte 0x0020203e
	.4byte 0x00000040
	.4byte 0x00103040
	.4byte 0x0020a043
	.4byte 0x00301040
	.4byte 0x0040603e
	.4byte 0x0050103f
	.4byte 0x00000041
	.4byte 0x00103043
	.4byte 0x00203041
	.4byte 0x00302041
	.4byte 0x0040303e
	.4byte 0x00000042
	.4byte 0x00102042
	.4byte 0x00201042
	.4byte 0x00302043
	.4byte 0x00401043
	.4byte 0x00000043
	.4byte 0x00103042
	.4byte 0x00204042
	.4byte 0x00301041
	.4byte 0x00407043
	.4byte 0x00508043
	.4byte 0x00609043
	.4byte 0x00704043
	.4byte 0x00805043
	.4byte 0x00906043
	.4byte 0x00a02040
	.4byte 0x00b0803e
	.4byte 0x00c0703e
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0059005c
	.4byte 0x00000001
	.4byte 0x01070000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00024000
	.4byte 0x006e005d
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00df
	.4byte 0x00000007
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00800000
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
	.4byte 0x02008fcd
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x02008ff9
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008031
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000602
	.4byte 0xffff000b
	.4byte 0x0200825d
	.4byte 0x00008602
	.4byte 0xffff000c
	.4byte 0x0200829d
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008e79
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x02008e79
	.4byte 0x00004602
	.4byte 0xffff000d
	.4byte 0x020082e1
	.4byte 0x0000c602
	.4byte 0xffff000e
	.4byte 0x02008315
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte 0x02008e91
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x02008e91
	.4byte 0x00004602
	.4byte 0xffff000f
	.4byte 0x0200834d
	.4byte 0x0000c602
	.4byte 0xffff0010
	.4byte 0x02008381
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x02008ea9
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte 0x02008ea9
	.4byte 0x00000001
	.4byte 0xffff0015
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0016
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0017
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0018
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0019
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff001a
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff001b
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff001c
	.4byte 0x00000008
	.4byte 0x00000003
	.4byte 0xffff0001
	.4byte 0x02009f05
	.4byte 0x00000013
	.4byte 0x0ef10064
	.4byte 0x00500001
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000602
	.4byte 0x0306000b
	.4byte 0x020083b9
	.4byte 0x00008602
	.4byte 0xffff000b
	.4byte 0x0200842d
	.4byte 0x0000c602
	.4byte 0xffff0011
	.4byte 0x02009a35
	.4byte 0x00000602
	.4byte 0xffff0011
	.4byte 0x02008465
	.4byte 0x00008602
	.4byte 0xffff000c
	.4byte 0x020083ed
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008e79
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x02008e79
	.4byte 0x00000002
	.4byte 0xffff0011
	.4byte 0x02008e79
	.4byte 0x00008602
	.4byte 0xffff000d
	.4byte 0x02009a35
	.4byte 0x00008602
	.4byte 0x0304000e
	.4byte 0x02009a35
	.4byte 0x00004602
	.4byte 0xffff000d
	.4byte 0x020084bd
	.4byte 0x0000c602
	.4byte 0xffff000e
	.4byte 0x020084f1
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte 0x02008e91
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x02008e91
	.4byte 0x00004602
	.4byte 0xffff000f
	.4byte 0x02008529
	.4byte 0x0000c602
	.4byte 0xffff0010
	.4byte 0x02008569
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x02008ea9
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte 0x02008ea9
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008041
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008602
	.4byte 0xffff000a
	.4byte 0x020085b9
	.4byte 0x00000602
	.4byte 0xffff000b
	.4byte 0x0200862d
	.4byte 0x00008602
	.4byte 0xffff000b
	.4byte 0x02008669
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x02009ab1
	.4byte 0x00000602
	.4byte 0xffff000c
	.4byte 0x020086c1
	.4byte 0x0000c602
	.4byte 0x130b000c
	.4byte 0x02009ab1
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x02008e79
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008e79
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x02008e79
	.4byte 0x00008602
	.4byte 0xffff000e
	.4byte 0x020086fd
	.4byte 0x00000602
	.4byte 0xffff000d
	.4byte 0x02008735
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte 0x02008e91
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x02008e91
	.4byte 0x0000c602
	.4byte 0xffff000f
	.4byte 0x02008769
	.4byte 0x00004602
	.4byte 0xffff0010
	.4byte 0x020087f1
	.4byte 0x0000c602
	.4byte 0xffff0011
	.4byte 0x020088cd
	.4byte 0x00004602
	.4byte 0x03080011
	.4byte 0x02008921
	.4byte 0x0000c602
	.4byte 0xffff0012
	.4byte 0x020089d1
	.4byte 0x00004602
	.4byte 0x03100012
	.4byte 0x02008a11
	.4byte 0x0000c602
	.4byte 0xffff0013
	.4byte 0x02008a61
	.4byte 0x00004602
	.4byte 0xffff0013
	.4byte 0x02008ab5
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x02008ea9
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte 0x02008ea9
	.4byte 0x00000002
	.4byte 0xffff0011
	.4byte 0x02008ea9
	.4byte 0x00000002
	.4byte 0xffff0012
	.4byte 0x02008ea9
	.4byte 0x00000002
	.4byte 0xffff0013
	.4byte 0x02008ea9
	.4byte 0x0000c602
	.4byte 0xffff0014
	.4byte 0x02008b05
	.4byte 0x00004602
	.4byte 0x03080015
	.4byte 0x02008bf5
	.4byte 0x0000c602
	.4byte 0xffff0016
	.4byte 0x02008c25
	.4byte 0x00004602
	.4byte 0xffff0016
	.4byte 0x02008c79
	.4byte 0x00008602
	.4byte 0xffff0017
	.4byte 0x02009ac1
	.4byte 0x00004602
	.4byte 0xffff0017
	.4byte 0x02008ca9
	.4byte 0x0000c602
	.4byte 0xffff0018
	.4byte 0x02008cd9
	.4byte 0x00004602
	.4byte 0xffff0018
	.4byte 0x02008d09
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x02008ec1
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x02008ec1
	.4byte 0x00000002
	.4byte 0xffff0016
	.4byte 0x02008ec1
	.4byte 0x00000002
	.4byte 0xffff0017
	.4byte 0x02008ec1
	.4byte 0x00000002
	.4byte 0xffff0018
	.4byte 0x02008ec1
	.4byte 0x00004602
	.4byte 0xffff0019
	.4byte 0x02009a35
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000003
	.4byte 0x0f13001e
	.4byte 0x02009b9d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004602
	.4byte 0xffff000b
	.4byte 0x02008d39
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte 0x02008d6d
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008e79
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x02008e79
	.4byte 0x00008602
	.4byte 0x0313000d
	.4byte 0x02008da5
	.4byte 0x00000602
	.4byte 0xffff000e
	.4byte 0x02008dd9
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte 0x02008e91
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x02008e91
	.4byte 0x00000602
	.4byte 0xffff000f
	.4byte 0x02008e0d
	.4byte 0x00008602
	.4byte 0xffff0010
	.4byte 0x02008e41
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x02008ea9
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte 0x02008ea9
	.4byte 0x00000021
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
	.4byte 0x00000013
	.4byte 0x0f6b0064
	.4byte 0x00100008
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
	.4byte 0x00000003
	.4byte 0xffff0007
	.4byte 0x02009f21
	.4byte 0x00000013
	.4byte 0x0f140064
	.4byte 0x001000c8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
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
	.4byte 0x00000031
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000031
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x0200970d
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte 0x02009731
	.4byte 0x00000003
	.4byte 0xffff0013
	.4byte 0x02009f3d
	.4byte 0x0000e604
	.4byte 0x08200014
	.4byte 0x02009e09
	.4byte 0x00000003
	.4byte 0xffff0014
	.4byte 0x02009f59
	.4byte 0x40009085
	.4byte 0x08200000
	.4byte 0x02009fad
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02009c09
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
	.4byte 0x00000022
	.4byte 0x02009c09
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
	.4byte 0x00000022
	.4byte 0x02009c09
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
