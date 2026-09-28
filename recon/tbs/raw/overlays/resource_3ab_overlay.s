.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/ENTRY.INC"
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
	bl 0x02009978
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
	bl 0x02009998
	adds r0, r5, #0
	movs r1, #14
	bl 0x02009a68
	adds r0, r5, #0
	movs r1, #1
	bl 0x020099a0
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
	bl 0x02009978
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
	bl 0x02009998
	adds r0, r5, #0
	movs r1, #15
	bl 0x02009a68
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
	.section .text.x02008314,"ax",%progbits
	.global Func_02000314
	.thumb_func
Func_02000314:
	push {lr}
	ldr r3, [pc, #32]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #24]
	cmp r2, r3
	beq .L_02000314_0
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02000314_0
	ldr r0, [pc, #20]
	b .L_02000314_1
.L_02000314_0:
	ldr r0, [pc, #20]
.L_02000314_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000068
	.4byte 0x0000009f
	.4byte 0x02009d3c
	.4byte 0x02009bec
	.global Func_0200034c
	.thumb_func
Func_0200034c:
	movs r0, #0
	bx lr
	.global Func_02000350
	.thumb_func
Func_02000350:
	push {lr}
	ldr r3, [pc, #32]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #24]
	cmp r2, r3
	beq .L_02000350_0
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02000350_0
	ldr r0, [pc, #20]
	b .L_02000350_1
.L_02000350_0:
	ldr r0, [pc, #20]
.L_02000350_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000068
	.4byte 0x0000009f
	.4byte 0x02009e04
	.4byte 0x02009dcc
	.global Func_02000388
	.thumb_func
Func_02000388:
	push {lr}
	ldr r3, [pc, #32]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #24]
	cmp r2, r3
	beq .L_02000388_0
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02000388_0
	ldr r0, [pc, #20]
	b .L_02000388_1
.L_02000388_0:
	ldr r0, [pc, #20]
.L_02000388_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000068
	.4byte 0x0000009f
	.4byte 0x02009f64
	.4byte 0x02009e14
	.global Func_020003c0
	.thumb_func
Func_020003c0:
	push {lr}
	bl 0x020099e0
	movs r2, #0
	movs r1, #0
	movs r0, #20
	bl 0x02009a40
	ldr r0, [pc, #28]
	bl 0x020099c8
	movs r0, #181
	movs r1, #3
	bl 0x02009ae0
	movs r1, #0
	movs r0, #181
	bl 0x020099f0
	bl 0x020099e8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000fd1
	.global Func_020003f4
	.thumb_func
Func_020003f4:
	push {r5, r6, lr}
	movs r0, #8
	sub sp, #8
	bl 0x02009a00
	adds r6, r0, #0
	movs r0, #0
	bl 0x02009a00
	movs r3, #14
	movs r5, #4
	str r3, [sp, #0]
	movs r0, #17
	movs r1, #4
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009990
	movs r3, #15
	str r3, [sp, #0]
	movs r0, #15
	movs r1, #3
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009990
	movs r3, #13
	str r3, [sp, #0]
	movs r0, #15
	movs r1, #3
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009990
	cmp r6, #0
	beq .L_020003f4_0
	adds r0, r6, #0
	movs r1, #0
	bl 0x02009998
	adds r2, r6, #0
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
	subs r2, #50
	movs r3, #1
	strb r3, [r2]
.L_020003f4_0:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x020099c8
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02000468
	.thumb_func
Func_02000468:
	push {lr}
	movs r0, #0
	bl 0x02009a00
	movs r2, #128
	ldr r3, [r0, #12]
	lsls r2, r2, #13
	cmp r3, r2
	blt .L_02000468_0
	movs r0, #8
	bl 0x02009a00
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	b .L_02000468_1
.L_02000468_0:
	movs r0, #8
	bl 0x02009a00
	movs r3, #1
	adds r0, #35
.L_02000468_1:
	strb r3, [r0]
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200049c
	.thumb_func
Func_0200049c:
	push {r5, r6, r7, lr}
	movs r0, #0
	sub sp, #8
	bl 0x02009a00
	ldr r3, [r0, #8]
	cmp r3, #0
	bge .L_0200049c_0
	ldr r2, [pc, #296]
	adds r3, r3, r2
.L_0200049c_0:
	ldr r0, [r0, #16]
	asrs r6, r3, #20
	cmp r0, #0
	bge .L_0200049c_1
	ldr r3, [pc, #284]
	adds r0, r0, r3
.L_0200049c_1:
	asrs r5, r0, #20
	ldr r0, [pc, #284]
	bl 0x020099c0
	cmp r0, #0
	bne .L_0200049c_2
	cmp r6, #7
	bne .L_0200049c_3
	cmp r5, #16
	bne .L_0200049c_3
	movs r0, #0
	movs r1, #0
	movs r2, #16
	bl 0x02009b28
.L_0200049c_3:
	movs r1, #1
	movs r2, #1
	movs r0, #102
	negs r1, r1
	negs r2, r2
	bl 0x02009ae8
	movs r3, #7
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #28
	movs r1, #31
	movs r2, #1
	movs r3, #1
	bl 0x02009990
.L_0200049c_2:
	movs r3, #4
	str r3, [sp, #4]
	movs r0, #47
	movs r1, #4
	movs r2, #1
	movs r3, #1
	movs r7, #46
	str r7, [sp, #0]
	bl 0x02009988
	movs r3, #13
	str r3, [sp, #0]
	movs r2, #3
	movs r3, #3
	movs r0, #34
	movs r1, #37
	str r2, [sp, #4]
	bl 0x02009990
	movs r1, #232
	movs r2, #144
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #15
	bl 0x02009a40
	movs r0, #8
	bl 0x02009a00
	movs r3, #0
	str r3, [r0, #12]
	ldr r0, [pc, #164]
	bl 0x020099c0
	cmp r0, #0
	beq .L_0200049c_4
	movs r6, #1
	movs r5, #14
	movs r0, #41
	movs r1, #49
	movs r2, #3
	movs r3, #4
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009988
	movs r3, #33
	str r3, [sp, #0]
	movs r0, #44
	movs r1, #49
	movs r2, #3
	movs r3, #4
	str r5, [sp, #4]
	bl 0x02009988
	movs r0, #47
	movs r1, #49
	movs r2, #3
	movs r3, #4
	str r6, [sp, #0]
	str r7, [sp, #4]
	bl 0x02009988
	b .L_0200049c_5
.L_0200049c_4:
	movs r1, #224
	movs r2, #134
	lsls r1, r1, #14
	lsls r2, r2, #17
	movs r0, #19
	bl 0x02009a40
	movs r0, #19
	bl 0x02009a00
	movs r1, #0
	bl 0x02009998
	movs r0, #19
	bl 0x02009a00
	cmp r0, #0
	beq .L_0200049c_5
	adds r2, r0, #0
	adds r2, #85
	movs r3, #8
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r0, #12]
	subs r2, #50
	movs r3, #2
	strb r3, [r2]
	ldr r3, [pc, #44]
	str r3, [r0, #24]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r0, #28]
.L_0200049c_5:
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #36]
	bl 0x02009928
	ldr r0, [pc, #32]
	bl 0x020099d0
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000fffff
	.4byte 0x00000f27
	.4byte 0x00000202
	.4byte 0x00013333
	.4byte 0x02008469
	.4byte 0x00000201
	.global Func_020005f0
	.thumb_func
Func_020005f0:
	push {lr}
	movs r0, #0
	sub sp, #8
	bl 0x02009a00
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl 0x02009a40
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x02009a40
	movs r3, #46
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #38
	movs r2, #1
	movs r3, #1
	bl 0x02009988
	movs r3, #13
	str r3, [sp, #0]
	movs r2, #3
	movs r0, #37
	movs r1, #37
	movs r3, #3
	str r2, [sp, #4]
	bl 0x02009990
	movs r3, #14
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #37
	movs r1, #37
	movs r2, #1
	movs r3, #1
	bl 0x02009990
	movs r3, #7
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #8
	movs r1, #16
	movs r2, #1
	bl 0x02009990
	movs r0, #102
	movs r1, #0
	movs r2, #0
	bl 0x02009ae8
	movs r3, #1
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #2
	movs r2, #3
	movs r1, #42
	movs r0, #32
	bl 0x02009988
	movs r0, #128
	lsls r0, r0, #2
	bl 0x020099d0
	movs r1, #1
	movs r0, #8
	bl 0x02009a48
	movs r0, #8
	bl 0x02009a00
	movs r3, #0
	str r3, [r0, #108]
	movs r0, #8
	bl 0x02009a00
	movs r1, #0
	bl 0x020099a8
	ldr r0, [pc, #16]
	bl 0x02009930
	ldr r0, [pc, #16]
	bl 0x020099c8
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02008469
	.4byte 0x00000201
	.global Func_020006bc
	.thumb_func
Func_020006bc:
	push {lr}
	bl 0x020099e0
	ldr r0, [pc, #24]
	movs r1, #1
	bl 0x020099b0
	ldr r0, [pc, #20]
	movs r1, #1
	bl 0x020099b0
	bl 0x020099e8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000947
	.4byte 0x000029e1
	.global Func_020006e4
	.thumb_func
Func_020006e4:
	push {lr}
	ldr r3, [pc, #52]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_020006e4_0
	ldr r0, [pc, #40]
	bl 0x020099c0
	ldr r0, [pc, #40]
	b .L_020006e4_1
.L_020006e4_0:
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_020006e4_2
	ldr r0, [pc, #24]
	bl 0x020099c0
	cmp r0, #0
	beq .L_020006e4_2
	ldr r0, [pc, #28]
	b .L_020006e4_1
.L_020006e4_2:
	ldr r0, [pc, #28]
.L_020006e4_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x0000009f
	.4byte 0x00000941
	.4byte 0x0200a3b4
	.4byte 0x00000068
	.4byte 0x0200a1bc
	.4byte 0x02009fc4
	.global Func_02000738
	.thumb_func
Func_02000738:
	push {lr}
	bl 0x020099e0
	movs r1, #129
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #17
	bl 0x02009aa0
	ldr r0, [pc, #20]
	bl 0x02009a70
	movs r1, #0
	movs r0, #17
	bl 0x02009a88
	bl 0x020099e8
	pop {r0}
	bx r0
	.4byte 0x00001b9c
	.global Func_02000764
	.thumb_func
Func_02000764:
	push {r5, lr}
	bl 0x020099e0
	ldr r0, [pc, #120]
	bl 0x020099c0
	cmp r0, #0
	bne .L_02000764_0
	ldr r0, [pc, #112]
	bl 0x02009a70
	movs r0, #18
	movs r1, #0
	bl 0x02009a80
	b .L_02000764_1
.L_02000764_0:
	ldr r0, [pc, #100]
	bl 0x02009a70
	movs r1, #0
	movs r0, #18
	bl 0x02009a78
	movs r0, #0
	movs r1, #0
	bl 0x020099f8
	cmp r0, #0
	bne .L_02000764_2
	ldr r5, [pc, #80]
	movs r2, #236
	ldr r3, [r5]
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r2, [r3]
	adds r2, #1
	movs r1, #0
	strh r2, [r3]
	movs r0, #18
	bl 0x02009a78
	movs r0, #0
	movs r1, #0
	bl 0x020099f8
	cmp r0, #1
	bne .L_02000764_2
	ldr r2, [r5]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000764_2:
	movs r0, #18
	movs r1, #0
	bl 0x02009a80
.L_02000764_1:
	bl 0x020099e8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000085a
	.4byte 0x00001be1
	.4byte 0x00001b9f
	.4byte 0x03001ebc
	.global Func_020007f4
	.thumb_func
Func_020007f4:
	push {lr}
	ldr r0, [pc, #56]
	bl 0x020099c0
	cmp r0, #0
	bne .L_020007f4_0
	ldr r0, [pc, #48]
	bl 0x020099c0
	cmp r0, #0
	bne .L_020007f4_1
	ldr r0, [pc, #44]
	b .L_020007f4_2
.L_020007f4_1:
	ldr r0, [pc, #44]
.L_020007f4_2:
	bl 0x02009a70
	movs r0, #18
	movs r1, #0
	bl 0x02009a80
	b .L_020007f4_3
.L_020007f4_0:
	ldr r0, [pc, #32]
	bl 0x02009a70
	movs r0, #18
	movs r1, #0
	bl 0x02009a80
.L_020007f4_3:
	pop {r0}
	bx r0
	.4byte 0x00000941
	.4byte 0x0000085a
	.4byte 0x00001be2
	.4byte 0x00001ba5
	.4byte 0x0000250c
	.global Func_02000844
	.thumb_func
Func_02000844:
	push {lr}
	bl 0x020099e0
	ldr r0, [pc, #20]
	bl 0x02009a70
	movs r1, #0
	movs r0, #9
	bl 0x02009a88
	bl 0x020099e8
	pop {r0}
	bx r0
	.4byte 0x00001ba6
	.global Func_02000864
	.thumb_func
Func_02000864:
	push {lr}
	bl 0x020099e0
	ldr r0, [pc, #20]
	bl 0x02009a70
	movs r1, #0
	movs r0, #11
	bl 0x02009a88
	bl 0x020099e8
	pop {r0}
	bx r0
	.4byte 0x00001baa
	.global Func_02000884
	.thumb_func
Func_02000884:
	push {lr}
	bl 0x020099e0
	ldr r0, [pc, #20]
	bl 0x02009a70
	movs r1, #0
	movs r0, #15
	bl 0x02009a88
	bl 0x020099e8
	pop {r0}
	bx r0
	.4byte 0x00001bb0
	.global Func_020008a4
	.thumb_func
Func_020008a4:
	push {lr}
	bl 0x020099e0
	movs r1, #3
	movs r0, #10
	bl 0x02009a58
	ldr r0, [pc, #20]
	bl 0x02009a70
	movs r0, #10
	movs r1, #0
	bl 0x02009a80
	bl 0x020099e8
	pop {r0}
	bx r0
	.4byte 0x000024d1
	.global Func_020008cc
	.thumb_func
Func_020008cc:
	push {lr}
	bl 0x020099e0
	ldr r0, [pc, #20]
	bl 0x02009a70
	movs r1, #0
	movs r0, #12
	bl 0x02009a88
	bl 0x020099e8
	pop {r0}
	bx r0
	.4byte 0x000024d3
	.global Func_020008ec
	.thumb_func
Func_020008ec:
	push {r5, lr}
	ldr r3, [pc, #112]
	movs r2, #147
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_020008ec_0
	ldr r0, [pc, #100]
	bl 0x02009a70
	b .L_020008ec_1
.L_020008ec_0:
	ldr r0, [pc, #96]
	bl 0x020099c0
	cmp r0, #0
	beq .L_020008ec_2
	ldr r0, [pc, #88]
	bl 0x020099c0
	cmp r0, #0
	bne .L_020008ec_2
	movs r2, #60
	ldr r1, [pc, #80]
	movs r0, #8
	bl 0x02009aa0
	ldr r5, [pc, #76]
	adds r0, r5, #0
	bl 0x02009a70
	movs r0, #8
	movs r1, #0
	bl 0x02009a80
	adds r5, #1
	movs r0, #8
	movs r1, #1
	bl 0x02009a50
	adds r0, r5, #0
	bl 0x02009a70
	ldr r0, [pc, #48]
	bl 0x020099c8
	b .L_020008ec_1
.L_020008ec_2:
	ldr r0, [pc, #44]
	bl 0x02009a70
.L_020008ec_1:
	movs r0, #8
	movs r1, #0
	bl 0x02009a80
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x00002411
	.4byte 0x00000941
	.4byte 0x0000094d
	.4byte 0x00000101
	.4byte 0x000024db
	.4byte 0x000009af
	.4byte 0x00001bb5
	.global Func_02000980
	.thumb_func
Func_02000980:
	push {lr}
	ldr r3, [pc, #60]
	movs r2, #147
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_02000980_0
	ldr r0, [pc, #48]
	bl 0x02009a70
	b .L_02000980_1
.L_02000980_0:
	ldr r0, [pc, #44]
	bl 0x020099c0
	cmp r0, #0
	beq .L_02000980_2
	ldr r0, [pc, #36]
	bl 0x02009a70
	b .L_02000980_1
.L_02000980_2:
	ldr r0, [pc, #32]
	bl 0x02009a70
.L_02000980_1:
	movs r0, #9
	movs r1, #0
	bl 0x02009a80
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00002412
	.4byte 0x00000941
	.4byte 0x000024dd
	.4byte 0x00001bb6
	.global Func_020009d4
	.thumb_func
Func_020009d4:
	push {lr}
	ldr r3, [pc, #144]
	movs r2, #147
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020009d4_0
	ldr r0, [pc, #132]
	bl 0x020099c0
	cmp r0, #0
	beq .L_020009d4_0
	ldr r0, [pc, #124]
	bl 0x020099c0
	cmp r0, #0
	bne .L_020009d4_0
	ldr r0, [pc, #120]
	bl 0x020099c0
	cmp r0, #0
	bne .L_020009d4_1
	ldr r3, [pc, #112]
	movs r2, #191
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	strh r0, [r3]
	bl 0x02009b18
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x02009a60
	movs r0, #8
	ldr r1, [pc, #88]
	movs r2, #60
	bl 0x02009aa0
	ldr r0, [pc, #84]
	bl 0x02009a70
	ldr r0, [pc, #68]
	bl 0x020099c8
	b .L_020009d4_2
.L_020009d4_1:
	ldr r0, [pc, #76]
	bl 0x02009a70
.L_020009d4_2:
	movs r0, #8
	movs r1, #0
	bl 0x02009a80
	movs r0, #8
	movs r1, #1
	bl 0x02009a50
	ldr r0, [pc, #56]
	bl 0x02009a70
	b .L_020009d4_3
.L_020009d4_0:
	ldr r0, [pc, #52]
	bl 0x02009a70
.L_020009d4_3:
	movs r0, #8
	movs r1, #0
	bl 0x02009a80
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000941
	.4byte 0x0000094d
	.4byte 0x000009af
	.4byte 0x03001ebc
	.4byte 0x00000101
	.4byte 0x000024db
	.4byte 0x000024e7
	.4byte 0x000024dc
	.4byte 0x00001bbf
	.global Func_02000a90
	.thumb_func
Func_02000a90:
	push {lr}
	ldr r0, [pc, #36]
	bl 0x020099c0
	cmp r0, #0
	beq .L_02000a90_0
	ldr r0, [pc, #28]
	bl 0x02009a70
	b .L_02000a90_1
.L_02000a90_0:
	ldr r0, [pc, #24]
	bl 0x02009a70
.L_02000a90_1:
	movs r0, #9
	movs r1, #0
	bl 0x02009a80
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000941
	.4byte 0x000024e8
	.4byte 0x00001bc0
	.global Func_02000ac4
	.thumb_func
Func_02000ac4:
	push {r5, lr}
	movs r0, #0
	bl 0x02009a00
	ldr r3, [r0, #16]
	cmp r3, #0
	bge .L_02000ac4_0
	ldr r2, [pc, #44]
	adds r3, r3, r2
.L_02000ac4_0:
	ldr r0, [pc, #44]
	asrs r5, r3, #20
	bl 0x020099c0
	cmp r0, #0
	bne .L_02000ac4_1
	cmp r5, #10
	bne .L_02000ac4_1
	ldr r0, [pc, #28]
	bl 0x020099c8
	ldr r3, [pc, #24]
	movs r2, #182
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #20
	strh r2, [r3]
.L_02000ac4_1:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x000fffff
	.4byte 0x00000243
	.4byte 0x03001ebc
	.global Func_02000b0c
	.thumb_func
Func_02000b0c:
	push {lr}
	bl 0x020099e0
	movs r1, #1
	movs r0, #0
	bl 0x02009a48
	ldr r0, [pc, #72]
	bl 0x02009a70
	movs r0, #1
	movs r1, #0
	bl 0x02009a80
	movs r1, #129
	movs r2, #100
	movs r0, #0
	lsls r1, r1, #1
	bl 0x02009aa0
	movs r0, #0
	movs r1, #2
	bl 0x02009a48
	movs r2, #12
	movs r1, #0
	movs r0, #0
	bl 0x02009a30
	movs r0, #0
	bl 0x02009a38
	movs r1, #1
	movs r0, #0
	bl 0x02009a48
	ldr r0, [pc, #16]
	bl 0x020099d0
	bl 0x020099e8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000024cf
	.4byte 0x00000243
	.global Func_02000b6c
	.thumb_func
Func_02000b6c:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #152]
	ldr r7, [r3]
	bl 0x020099e0
	movs r5, #8
	movs r6, #0
.L_02000b6c_1:
	adds r0, r5, #0
	bl 0x02009a00
	cmp r0, #0
	beq .L_02000b6c_0
	adds r3, r0, #0
	adds r3, #85
	strb r6, [r3]
.L_02000b6c_0:
	adds r5, #1
	cmp r5, #65
	bls .L_02000b6c_1
	movs r0, #158
	bl 0x02009b30
	movs r3, #182
	lsls r3, r3, #1
	adds r6, r7, r3
	movs r3, #0
	ldrsh r5, [r6, r3]
	subs r5, #4
	lsls r4, r5, #3
	ldr r0, [pc, #100]
	adds r3, r4, #4
	ldrh r1, [r0, r3]
	adds r3, r3, r0
	ldrh r2, [r3, #2]
	ldr r0, [r0, r4]
	bl 0x02009980
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
	movs r0, #0
	lsls r2, r2, #7
	bl 0x02009a08
	movs r0, #0
	bl 0x02009a00
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #2
	movs r0, #0
	bl 0x02009a48
	cmp r5, #6
	beq .L_02000b6c_2
	movs r2, #8
	movs r0, #0
	movs r1, #2
	negs r2, r2
	bl 0x02009a28
	movs r0, #10
	bl 0x020099d8
.L_02000b6c_2:
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl 0x02009ab8
	bl 0x02009ad0
	bl 0x02009ad8
	bl 0x020099e8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0200a50c
	.global Func_02000c10
	.thumb_func
Func_02000c10:
	push {r5, lr}
	ldr r0, [pc, #80]
	bl 0x020099c0
	cmp r0, #0
	beq .L_02000c10_0
	ldr r0, [pc, #72]
	bl 0x020099c0
	adds r5, r0, #0
	cmp r5, #0
	bne .L_02000c10_0
	ldr r2, [pc, #64]
	ldr r1, [pc, #68]
	movs r0, #0
	bl 0x02009a08
	movs r0, #0
	bl 0x02009a00
	adds r0, #85
	strb r5, [r0]
	movs r1, #2
	movs r0, #0
	bl 0x02009a48
	movs r2, #8
	movs r0, #0
	movs r1, #2
	negs r2, r2
	bl 0x02009a28
	movs r0, #13
	bl 0x020099d8
	movs r0, #12
	bl 0x02009ab8
.L_02000c10_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000202
	.4byte 0x00000201
	.4byte 0x00001999
	.4byte 0x00003333
	.global Func_02000c74
	.thumb_func
Func_02000c74:
	push {r5, r6, lr}
	ldr r0, [pc, #132]
	sub sp, #8
	bl 0x020099c8
	bl 0x020099e0
	ldr r2, [pc, #124]
	ldr r1, [pc, #124]
	movs r0, #0
	bl 0x02009a08
	movs r0, #0
	bl 0x02009a00
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #2
	movs r0, #0
	bl 0x02009a48
	movs r2, #8
	movs r1, #0
	negs r2, r2
	movs r0, #0
	bl 0x02009b20
	movs r0, #158
	bl 0x02009b30
	movs r5, #41
	movs r6, #4
	movs r1, #4
	movs r2, #2
	movs r3, #2
	movs r0, #53
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009988
	movs r0, #10
	bl 0x020099d8
	movs r1, #6
	movs r2, #2
	movs r3, #2
	movs r0, #53
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009988
	movs r0, #10
	bl 0x020099d8
	movs r0, #1
	bl 0x02009ab8
	bl 0x02009ad0
	bl 0x02009ad8
	bl 0x020099e8
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00000242
	.4byte 0x00001999
	.4byte 0x00003333
	.global Func_02000d08
	.thumb_func
Func_02000d08:
	push {lr}
	bl 0x02009ac0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000d14
	.thumb_func
Func_02000d14:
	push {lr}
	ldr r0, [pc, #20]
	bl 0x020099c0
	cmp r0, #0
	bne .L_02000d14_0
	movs r0, #83
	bl 0x02009b30
.L_02000d14_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000201
	.global Func_02000d30
	.thumb_func
Func_02000d30:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r0, [pc, #364]
	sub sp, #8
	bl 0x020099c0
	cmp r0, #0
	beq .L_02000d30_0
	b .L_02000d30_1
.L_02000d30_0:
	ldr r0, [pc, #356]
	bl 0x020099c0
	cmp r0, #0
	beq .L_02000d30_2
	b .L_02000d30_1
.L_02000d30_2:
	movs r1, #0
	movs r2, #0
	movs r0, #19
	bl 0x02009a40
	movs r0, #210
	bl 0x02009b30
	movs r0, #1
	bl 0x02009920
	movs r6, #1
	movs r5, #14
	movs r0, #32
	movs r1, #45
	movs r2, #3
	movs r3, #4
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009988
	movs r3, #33
	str r3, [sp, #0]
	mov r8, r3
	movs r0, #35
	movs r1, #45
	movs r2, #3
	movs r3, #4
	str r5, [sp, #4]
	bl 0x02009988
	movs r3, #46
	str r3, [sp, #4]
	mov r10, r3
	movs r1, #45
	movs r2, #3
	movs r3, #4
	movs r0, #38
	str r6, [sp, #0]
	bl 0x02009988
	movs r0, #10
	bl 0x02009920
	movs r0, #41
	movs r1, #45
	movs r2, #3
	movs r3, #4
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009988
	mov r3, r8
	str r3, [sp, #0]
	movs r0, #44
	movs r1, #45
	movs r2, #3
	movs r3, #4
	str r5, [sp, #4]
	bl 0x02009988
	mov r3, r10
	str r3, [sp, #4]
	movs r1, #45
	movs r2, #3
	movs r3, #4
	movs r0, #47
	str r6, [sp, #0]
	bl 0x02009988
	movs r0, #10
	bl 0x02009920
	movs r0, #50
	movs r1, #45
	movs r2, #3
	movs r3, #4
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009988
	mov r3, r8
	str r3, [sp, #0]
	movs r0, #53
	movs r1, #45
	movs r2, #3
	movs r3, #4
	str r5, [sp, #4]
	bl 0x02009988
	mov r3, r10
	str r3, [sp, #4]
	movs r1, #45
	movs r2, #3
	movs r3, #4
	movs r0, #56
	str r6, [sp, #0]
	bl 0x02009988
	movs r0, #10
	bl 0x02009920
	movs r0, #32
	movs r1, #49
	movs r2, #3
	movs r3, #4
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009988
	mov r3, r8
	str r3, [sp, #0]
	movs r0, #35
	movs r1, #49
	movs r2, #3
	movs r3, #4
	str r5, [sp, #4]
	bl 0x02009988
	mov r3, r10
	str r3, [sp, #4]
	movs r1, #49
	movs r2, #3
	movs r3, #4
	movs r0, #38
	str r6, [sp, #0]
	bl 0x02009988
	movs r0, #10
	bl 0x02009920
	movs r0, #41
	movs r1, #49
	movs r2, #3
	movs r3, #4
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009988
	mov r3, r8
	str r3, [sp, #0]
	movs r0, #44
	movs r1, #49
	movs r2, #3
	movs r3, #4
	str r5, [sp, #4]
	bl 0x02009988
	mov r3, r10
	str r3, [sp, #4]
	movs r0, #47
	movs r1, #49
	movs r2, #3
	movs r3, #4
	str r6, [sp, #0]
	bl 0x02009988
	movs r0, #10
	bl 0x02009920
	ldr r0, [pc, #24]
	bl 0x020099c8
.L_02000d30_1:
	sub sp, #-8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000201
	.4byte 0x00000202
	.global Func_02000eb0
	.thumb_func
Func_02000eb0:
	push {r5, lr}
	ldr r3, [pc, #224]
	movs r2, #147
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	sub sp, #8
	cmp r3, #0
	bne .L_02000eb0_0
	bl 0x020099e0
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #2
	bl 0x02009aa0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #15
	movs r0, #9
	bl 0x02009aa0
	movs r0, #30
	bl 0x020099d8
	movs r0, #8
	movs r1, #152
	movs r2, #168
	bl 0x02009a20
	movs r2, #168
	movs r1, #168
	movs r0, #9
	bl 0x02009a20
	movs r0, #8
	bl 0x02009a38
	movs r0, #9
	bl 0x02009a38
	movs r0, #8
	bl 0x02009a10
	movs r0, #8
	movs r1, #0
	bl 0x02009a48
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #8
	bl 0x02009a90
	movs r0, #9
	bl 0x02009a10
	movs r0, #9
	movs r1, #0
	bl 0x02009a48
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #9
	bl 0x02009a90
	ldr r0, [pc, #92]
	bl 0x02009a70
	movs r1, #0
	movs r0, #8
	bl 0x02009a80
	movs r0, #144
	lsls r0, r0, #2
	bl 0x020099c8
	movs r3, #7
	str r3, [sp, #0]
	movs r5, #11
	movs r0, #6
	movs r1, #11
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009990
	movs r3, #8
	str r3, [sp, #0]
	movs r0, #6
	movs r1, #11
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009990
	movs r3, #9
	str r3, [sp, #0]
	movs r0, #6
	movs r1, #11
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009990
	bl 0x020099e8
.L_02000eb0_0:
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x000024da
	.global Func_02000f9c
	.thumb_func
Func_02000f9c:
	bx lr
	.2byte 0x0000
	.global Func_02000fa0
	.thumb_func
Func_02000fa0:
	push {r5, lr}
	sub sp, #8
	movs r3, #7
	str r3, [sp, #0]
	movs r5, #11
	movs r0, #6
	movs r1, #11
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009990
	movs r3, #8
	str r3, [sp, #0]
	movs r0, #6
	movs r1, #11
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009990
	movs r3, #9
	str r3, [sp, #0]
	movs r1, #11
	movs r2, #1
	movs r3, #1
	movs r0, #6
	str r5, [sp, #4]
	bl 0x02009990
	ldr r0, [pc, #12]
	bl 0x020099c8
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000241
	.global Func_02000ff0
	.thumb_func
Func_02000ff0:
	push {r5, r6, lr}
	movs r0, #145
	lsls r0, r0, #2
	bl 0x020099c0
	cmp r0, #0
	beq .L_02000ff0_0
	b .L_02000ff0_1
.L_02000ff0_0:
	movs r0, #145
	lsls r0, r0, #2
	bl 0x020099c8
	bl 0x020099e0
	movs r0, #0
	bl 0x02009a00
	movs r1, #0
	adds r6, r0, #0
	movs r2, #0
	movs r0, #8
	bl 0x02009a60
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl 0x02009a60
	movs r0, #8
	movs r1, #1
	bl 0x02009a50
	movs r1, #1
	movs r0, #9
	bl 0x02009a50
	movs r0, #20
	bl 0x020099d8
	movs r1, #129
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #8
	bl 0x02009aa0
	ldr r5, [pc, #308]
	adds r0, r5, #0
	bl 0x02009a70
	movs r0, #8
	movs r1, #0
	bl 0x02009a80
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x02009a08
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x02009a08
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	movs r0, #9
	lsls r1, r1, #10
	bl 0x02009a08
	movs r1, #4
	movs r0, #9
	bl 0x02009a48
	movs r0, #35
	bl 0x020099d8
	adds r0, r5, #1
	bl 0x02009a70
	movs r0, #9
	movs r1, #0
	bl 0x02009a80
	movs r2, #30
	ldr r1, [pc, #224]
	movs r0, #8
	bl 0x02009aa0
	adds r0, r5, #2
	bl 0x02009a70
	movs r0, #8
	movs r1, #0
	bl 0x02009a80
	movs r1, #3
	movs r0, #9
	bl 0x02009a48
	adds r5, #3
	movs r0, #25
	bl 0x020099d8
	adds r0, r5, #0
	bl 0x02009a70
	movs r0, #9
	movs r1, #0
	bl 0x02009a80
	movs r3, #10
	ldrsh r1, [r6, r3]
	movs r3, #18
	ldrsh r2, [r6, r3]
	subs r1, #1
	movs r0, #8
	bl 0x02009a20
	movs r0, #8
	bl 0x02009a38
	movs r0, #0
	movs r1, #160
	movs r2, #216
	bl 0x02009a20
	movs r0, #8
	movs r1, #152
	movs r2, #200
	bl 0x02009a20
	movs r1, #168
	movs r2, #200
	movs r0, #9
	bl 0x02009a20
	movs r0, #8
	bl 0x02009a38
	movs r0, #9
	bl 0x02009a38
	movs r0, #0
	bl 0x02009a38
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x02009a60
	movs r1, #0
	movs r2, #0
	movs r0, #9
	bl 0x02009a60
	movs r0, #12
	bl 0x020099d8
	movs r2, #136
	movs r0, #0
	movs r1, #160
	lsls r2, r2, #1
	bl 0x02009a20
	movs r2, #128
	movs r0, #8
	movs r1, #152
	lsls r2, r2, #1
	bl 0x02009a20
	movs r2, #128
	movs r1, #168
	lsls r2, r2, #1
	movs r0, #9
	bl 0x02009a20
	movs r0, #8
	bl 0x02009a38
	movs r0, #9
	bl 0x02009a38
	movs r0, #0
	bl 0x02009a38
	bl 0x020099e8
	movs r1, #200
	ldr r0, [pc, #20]
	lsls r1, r1, #4
	bl 0x02009928
.L_02000ff0_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00002409
	.4byte 0x00000103
	.4byte 0x02009241
	.global Func_0200118c
	.thumb_func
Func_0200118c:
	push {r5, lr}
	sub sp, #8
	movs r3, #7
	str r3, [sp, #0]
	movs r5, #11
	movs r0, #7
	movs r1, #12
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009990
	movs r3, #8
	str r3, [sp, #0]
	movs r0, #7
	movs r1, #12
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009990
	movs r3, #9
	str r3, [sp, #0]
	movs r0, #7
	movs r1, #12
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009990
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.global Func_020011d0
	.thumb_func
Func_020011d0:
	push {r5, lr}
	ldr r3, [pc, #88]
	ldr r0, [pc, #88]
	ldr r5, [r3]
	bl 0x020099d0
	movs r0, #144
	lsls r0, r0, #2
	bl 0x020099d0
	movs r0, #0
	bl 0x02009a00
	ldr r2, [pc, #72]
	ldr r3, [r0, #8]
	adds r3, r3, r2
	ldr r2, [pc, #68]
	cmp r3, r2
	bhi .L_020011d0_0
	movs r3, #160
	ldr r0, [r0, #16]
	lsls r3, r3, #16
	cmp r0, r3
	ble .L_020011d0_0
	movs r2, #248
	lsls r2, r2, #16
	cmp r0, r2
	bge .L_020011d0_0
	ldr r0, [pc, #48]
	bl 0x02009930
	movs r3, #193
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #91
	strh r3, [r2]
.L_020011d0_0:
	bl 0x0200918c
	movs r0, #145
	lsls r0, r0, #2
	bl 0x020099d0
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000241
	.4byte 0xff97ffff
	.4byte 0x0087fffe
	.4byte 0x02009241
	.global Func_02001240
	.thumb_func
Func_02001240:
	push {r5, lr}
	ldr r3, [pc, #80]
	movs r0, #0
	ldr r5, [r3]
	bl 0x02009a00
	ldr r3, [pc, #72]
	movs r2, #147
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02001240_0
	ldr r2, [pc, #60]
	ldr r3, [r0, #8]
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #14
	cmp r3, r2
	bhi .L_02001240_0
	movs r3, #168
	ldr r0, [r0, #16]
	lsls r3, r3, #16
	cmp r0, r3
	blt .L_02001240_0
	movs r2, #176
	lsls r2, r2, #16
	cmp r0, r2
	bge .L_02001240_0
	ldr r0, [pc, #32]
	bl 0x02009930
	movs r3, #193
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #91
	strh r3, [r2]
.L_02001240_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0xff700000
	.4byte 0x02009241
	.global Func_020012a4
	.thumb_func
Func_020012a4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #0
	sub sp, #28
	bl 0x02009a00
	ldr r3, [pc, #100]
	ldr r7, [r3]
	movs r3, #15
	ands r7, r3
	mov r8, r0
	cmp r7, #0
	bne .L_020012a4_0
	bl 0x02009938
	movs r3, #52
	muls r3, r0
	lsrs r3, r3, #16
	lsls r3, r3, #6
	adds r5, r3, #0
	adds r5, #230
	adds r0, r5, #0
	bl 0x02009948
	add r6, sp, #16
	cmp r0, #0
	bge .L_020012a4_1
	adds r0, #3
.L_020012a4_1:
	asrs r3, r0, #2
	adds r0, r5, #0
	str r3, [r6]
	str r7, [r6, #4]
	bl 0x02009940
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	str r0, [r6, #8]
	mov r3, r8
	ldr r5, [r3, #8]
	ldr r4, [r6, #4]
	ldr r1, [r3, #12]
	ldr r2, [r3, #16]
	ldr r3, [r6]
	str r0, [sp, #4]
	adds r0, r5, #0
	str r4, [sp, #0]
	str r7, [sp, #8]
	str r7, [sp, #12]
	bl 0x0200813c
.L_020012a4_0:
	sub sp, #-28
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.global Func_0200131c
	.thumb_func
Func_0200131c:
	push {r5, r6, lr}
	bl 0x020099e0
	movs r1, #160
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x02009a40
	movs r1, #152
	movs r2, #224
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #15
	bl 0x02009a40
	movs r1, #168
	movs r2, #224
	movs r0, #9
	lsls r1, r1, #16
	lsls r2, r2, #15
	bl 0x02009a40
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009a90
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	movs r2, #0
	bl 0x02009a90
	movs r1, #160
	movs r2, #0
	movs r0, #18
	lsls r1, r1, #7
	bl 0x02009a90
	movs r1, #0
	movs r0, #0
	bl 0x02009ab0
	bl 0x02009ac8
	movs r0, #30
	bl 0x020099d8
	movs r0, #0
	ldr r1, [pc, #548]
	ldr r2, [pc, #552]
	bl 0x02009a08
	movs r0, #8
	ldr r1, [pc, #540]
	ldr r2, [pc, #540]
	bl 0x02009a08
	movs r0, #9
	ldr r1, [pc, #528]
	ldr r2, [pc, #532]
	bl 0x02009a08
	movs r2, #144
	movs r0, #8
	movs r1, #152
	lsls r2, r2, #1
	bl 0x02009a20
	movs r2, #144
	lsls r2, r2, #1
	movs r0, #9
	movs r1, #168
	bl 0x02009a20
	movs r0, #0
	movs r1, #4
	bl 0x02009a48
	movs r2, #148
	lsls r2, r2, #1
	movs r0, #0
	movs r1, #160
	bl 0x02009a18
	ldr r6, [pc, #484]
	movs r1, #200
	lsls r1, r1, #4
	adds r0, r6, #0
	bl 0x02009928
	movs r0, #1
	bl 0x020099d8
	movs r0, #121
	bl 0x02009b30
	movs r0, #20
	bl 0x020099d8
	movs r0, #8
	movs r1, #3
	bl 0x02009a98
	movs r1, #3
	movs r0, #9
	bl 0x02009a98
	movs r0, #121
	bl 0x02009b30
	movs r0, #30
	bl 0x020099d8
	movs r0, #8
	bl 0x02009a00
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #9
	bl 0x02009a00
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	movs r1, #4
	strb r5, [r0]
	movs r0, #0
	bl 0x02009a48
	movs r0, #121
	bl 0x02009b30
	movs r0, #0
	bl 0x02009a38
	movs r0, #8
	movs r1, #1
	bl 0x02009a48
	movs r1, #1
	movs r0, #9
	bl 0x02009a48
	adds r0, r6, #0
	bl 0x02009930
	movs r0, #0
	bl 0x02009a00
	adds r0, #85
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #0
	bl 0x02009a00
	movs r5, #192
	lsls r5, r5, #11
	str r5, [r0, #40]
	movs r0, #0
	bl 0x02009a00
	str r5, [r0, #44]
	movs r0, #1
	bl 0x020099d8
	b .L_0200131c_0
.L_0200131c_1:
	movs r0, #1
	bl 0x020099d8
.L_0200131c_0:
	movs r0, #0
	bl 0x02009a00
	ldr r3, [r0, #12]
	cmp r3, #0
	bne .L_0200131c_1
	movs r1, #192
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #8
	bl 0x02009a90
	movs r1, #19
	movs r0, #0
	bl 0x02009a48
	movs r0, #127
	bl 0x02009b30
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x02009aa8
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #252]
	bl 0x02009928
	movs r0, #2
	bl 0x020099d8
	movs r0, #0
	bl 0x02009a00
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r0, #40]
	movs r0, #1
	bl 0x020099d8
	b .L_0200131c_2
.L_0200131c_3:
	movs r0, #1
	bl 0x020099d8
.L_0200131c_2:
	movs r0, #0
	bl 0x02009a00
	ldr r3, [r0, #12]
	cmp r3, #0
	bne .L_0200131c_3
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #0
	bl 0x02009aa8
	movs r0, #10
	bl 0x020099d8
	movs r1, #1
	movs r0, #0
	bl 0x02009a48
	ldr r0, [pc, #180]
	bl 0x02009930
	movs r0, #50
	bl 0x020099d8
	ldr r0, [pc, #172]
	bl 0x02009a70
	movs r1, #0
	movs r0, #8
	bl 0x02009a80
	movs r0, #8
	bl 0x02009a00
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #9
	bl 0x02009a00
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #128
	orrs r5, r3
	movs r2, #128
	strb r5, [r0]
	lsls r1, r1, #9
	movs r0, #8
	lsls r2, r2, #8
	bl 0x02009a08
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009a08
	movs r0, #8
	movs r1, #144
	movs r2, #200
	bl 0x02009a20
	movs r2, #200
	movs r1, #176
	movs r0, #9
	bl 0x02009a20
	movs r0, #8
	bl 0x02009a38
	movs r0, #9
	bl 0x02009a38
	movs r0, #8
	movs r1, #1
	bl 0x02009a48
	movs r1, #1
	movs r0, #9
	bl 0x02009a48
	movs r0, #30
	bl 0x020099d8
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #0
	bl 0x02009a90
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009a90
	bl 0x020099e8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0001cccc
	.4byte 0x0000e666
	.4byte 0x020092a5
	.4byte 0x00002410
	.global Func_020015bc
	.thumb_func
Func_020015bc:
	push {lr}
	bl 0x020099e0
	bl 0x02009ac8
	movs r2, #168
	movs r1, #152
	movs r0, #0
	bl 0x02009a20
	movs r0, #0
	bl 0x02009a38
	movs r0, #20
	bl 0x020099d8
	movs r0, #146
	movs r1, #1
	bl 0x02009af8
	movs r1, #0
	movs r0, #0
	bl 0x02009b00
	bl 0x02009b10
	movs r0, #1
	bl 0x02009af0
	bl 0x02009b08
	movs r1, #144
	movs r2, #184
	movs r0, #0
	bl 0x02009a20
	movs r0, #0
	bl 0x02009a38
	movs r1, #88
	movs r2, #184
	movs r0, #0
	bl 0x02009a20
	movs r0, #0
	bl 0x02009a38
	movs r1, #88
	movs r2, #200
	movs r0, #0
	bl 0x02009a20
	movs r0, #0
	bl 0x02009a38
	movs r1, #72
	movs r2, #200
	movs r0, #0
	bl 0x02009a20
	movs r0, #0
	bl 0x02009a38
	movs r2, #144
	movs r1, #72
	lsls r2, r2, #1
	movs r0, #0
	bl 0x02009a20
	movs r0, #0
	bl 0x02009a38
	movs r2, #144
	movs r1, #88
	lsls r2, r2, #1
	movs r0, #0
	bl 0x02009a20
	movs r0, #0
	bl 0x02009a38
	bl 0x020099e8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001668
	.thumb_func
Func_02001668:
	push {r5, lr}
	ldr r5, [pc, #276]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #268]
	cmp r2, r3
	bne .L_02001668_0
	ldr r3, [pc, #264]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	bl 0x020085f0
	ldr r0, [pc, #252]
	bl 0x020099c0
	cmp r0, #0
	bne .L_02001668_1
	movs r0, #20
	bl 0x02009840
.L_02001668_1:
	movs r0, #8
	bl 0x02009a00
	cmp r0, #0
	beq .L_02001668_2
	movs r1, #0
	bl 0x02009998
.L_02001668_2:
	ldr r0, [pc, #224]
	bl 0x020099c8
.L_02001668_0:
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #212]
	cmp r2, r3
	bne .L_02001668_3
	ldr r3, [pc, #192]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	ldr r3, [pc, #196]
	movs r1, #225
	adds r2, r5, r3
	movs r3, #10
	strh r3, [r2]
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #4
	bne .L_02001668_4
	ldr r0, [pc, #176]
	bl 0x020099c0
	cmp r0, #0
	bne .L_02001668_4
	bl 0x0200931c
.L_02001668_4:
	ldr r3, [pc, #136]
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_02001668_5
	ldr r0, [pc, #148]
	bl 0x020099c0
	cmp r0, #0
	bne .L_02001668_5
	bl 0x020095bc
.L_02001668_5:
	ldr r0, [pc, #136]
	bl 0x020099c0
	cmp r0, #0
	beq .L_02001668_6
	ldr r0, [pc, #132]
	bl 0x020099c0
	cmp r0, #0
	bne .L_02001668_6
	movs r1, #200
	ldr r0, [pc, #124]
	lsls r1, r1, #4
	bl 0x02009928
.L_02001668_6:
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #116]
	bl 0x02009928
	ldr r0, [pc, #112]
	bl 0x020099d0
	ldr r0, [pc, #112]
	bl 0x020099d0
	ldr r0, [pc, #108]
	bl 0x020099d0
	ldr r0, [pc, #108]
	bl 0x020099d0
	ldr r0, [pc, #104]
	bl 0x020099d0
	ldr r0, [pc, #104]
	bl 0x020099d0
	ldr r0, [pc, #100]
	bl 0x020099d0
	ldr r0, [pc, #100]
	bl 0x020099d0
	ldr r0, [pc, #96]
	bl 0x020099d0
	ldr r0, [pc, #96]
	bl 0x020099d0
.L_02001668_3:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000068
	.4byte 0x03001ebc
	.4byte 0x00000fd1
	.4byte 0x00000201
	.4byte 0x0000009f
	.4byte 0x00000242
	.4byte 0x00000109
	.4byte 0x00000941
	.4byte 0x0000094d
	.4byte 0x02008ac5
	.4byte 0x02009241
	.4byte 0x00000944
	.4byte 0x00000945
	.4byte 0x00000946
	.4byte 0x00000947
	.4byte 0x00000948
	.4byte 0x00000943
	.4byte 0x00000949
	.4byte 0x0000094a
	.4byte 0x0000094b
	.4byte 0x0000094c
	.global Func_020017d8
	.thumb_func
Func_020017d8:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r0, [r6, #48]
	ldr r7, [r6, #80]
	bl 0x02009940
	lsls r5, r0, #1
	cmp r5, #0
	ble .L_020017d8_0
	negs r5, r5
.L_020017d8_0:
	ldr r0, [r6, #48]
	bl 0x02009948
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
	bl 0x02009948
	cmp r0, #0
	bge .L_020017d8_1
	adds r0, #7
.L_020017d8_1:
	asrs r3, r0, #3
	strh r3, [r7, #30]
	bl 0x02009938
	adds r5, r0, #0
	bl 0x02009938
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
	.global Func_02001840
	.thumb_func
Func_02001840:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	bl 0x02009a00
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
	bl 0x02009998
	movs r3, #92
	adds r3, r3, r7
	mov r2, r8
	strb r2, [r3]
	mov r10, r3
	adds r3, r7, #0
	adds r3, #85
	strb r2, [r3]
	ldr r0, [pc, #124]
	bl 0x020099c0
	cmp r0, #0
	bne .L_02001840_0
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #14
	adds r3, r3, r2
	str r3, [r7, #12]
.L_02001840_0:
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
	bl 0x02009950
	adds r5, r0, #0
	movs r0, #181
	bl 0x020099b8
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	adds r2, r5, #0
	movs r1, #128
	ldrb r0, [r6, #28]
	bl 0x02009960
	movs r0, #17
	bl 0x02009958
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
	.4byte 0x020097d9
	.include "games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/IMPORT.INC"
	.section .rodata,"a",%progbits
AlchemyData_02001b38:
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
	.global gEffectScripts
gEffectScripts:
	.4byte 0x02009b38
	.4byte 0x02009b70
	.4byte 0x02009ba8
AlchemyData_02001bec:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000e0
	.4byte 0xc00001d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000000e8
	.4byte 0x40000020
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000078
	.4byte 0x40000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x000000f8
	.4byte 0x400000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000158
	.4byte 0x400000e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000158
	.4byte 0x40000148
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x000000e8
	.4byte 0x40000168
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00000148
	.4byte 0x40000078
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x00000068
	.4byte 0x400000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000030
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x000000d8
	.4byte 0x40000158
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x00000028
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
	.4byte 0x00000098
	.4byte 0xc0000148
	.4byte 0x00200000
	.4byte 0x01100030
	.4byte 0x00000158
	.4byte 0xffff0001
	.4byte 0x000000a0
	.4byte 0x40000068
	.4byte 0x00200000
	.4byte 0x01100030
	.4byte 0x00000158
	.4byte 0xffff0002
	.4byte 0x00000098
	.4byte 0xc0000148
	.4byte 0x00200000
	.4byte 0x01100030
	.4byte 0x00000158
	.4byte 0xffff0003
	.4byte 0x00000090
	.4byte 0x40000068
	.4byte 0x00200000
	.4byte 0x01100030
	.4byte 0x00000158
	.4byte 0xffff0004
	.4byte 0x000000a0
	.4byte 0x000000e0
	.4byte 0x00200000
	.4byte 0x01100030
	.4byte 0x00000158
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000068
	.4byte 0x00122002
	.4byte 0x0020209f
	.4byte 0x0030206a
	.4byte 0x00401069
	.4byte 0x00502069
	.4byte 0x00603069
	.4byte 0x00704069
	.4byte 0x00805069
	.4byte 0x00906069
	.4byte 0x00a07069
	.4byte 0x00b08069
	.4byte 0x00c02016
	.4byte 0x000001ff
	.4byte 0x0000009f
	.4byte 0x001010a0
	.4byte 0x00202068
	.4byte 0x000001ff
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00003000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00015000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00003000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0000b000
	.4byte 0xffff0067
	.4byte 0x00000002
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00008000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00005000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00280000
	.4byte 0x0001d000
	.4byte 0xffff0066
	.4byte 0x00000002
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00004000
	.4byte 0xffff0096
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x00013000
	.4byte 0xffff0096
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x00015000
	.4byte 0xffff011d
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00025000
	.4byte 0x0fd10016
	.4byte 0x00000007
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00024000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0096
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00013000
	.4byte 0xffff0096
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00015000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008b6d
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008b6d
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008b6d
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008b6d
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008b6d
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008b6d
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x02008b6d
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x02008b6d
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte 0x02008c11
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008845
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001ba9
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008865
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001bad
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001bae
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001baf
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008885
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001bb3
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008739
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008765
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001bb7
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001bb8
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001bb9
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001bba
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001bbb
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001bbc
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001bbd
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001bbe
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001ba4
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x020087f5
	.4byte 0x00001815
	.4byte 0x02000008
	.4byte 0x020083f5
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x0200849d
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x020085f1
	.4byte 0x00000003
	.4byte 0x02010014
	.4byte 0x02008d31
	.4byte 0x00000003
	.4byte 0x0f270066
	.4byte 0x00300000
	.4byte 0x00000083
	.4byte 0x0f880064
	.4byte 0x001000e3
	.4byte 0x00000033
	.4byte 0x0f890065
	.4byte 0x001000b5
	.4byte 0x00009415
	.4byte 0x0fd10014
	.4byte 0x020083c1
	.4byte 0x00000003
	.4byte 0xffff005a
	.4byte 0x020086bd
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
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008b6d
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008b6d
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008b6d
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008b6d
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008b6d
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008b6d
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x02008b6d
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x02008b6d
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte 0x02008c11
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000024d0
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020088a5
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000024d2
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020088cd
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000024d6
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000024d7
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000024d8
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000024d9
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002509
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0000250a
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000024df
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000024e0
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000024e1
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000024e2
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000024e3
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000024e4
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000024e5
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000024e6
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000250b
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x020087f5
	.4byte 0x00001815
	.4byte 0x02000008
	.4byte 0x020083f5
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x0200849d
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x020085f1
	.4byte 0x00000003
	.4byte 0x02010014
	.4byte 0x02008d31
	.4byte 0x00000003
	.4byte 0x0f270066
	.4byte 0x00300000
	.4byte 0x00000083
	.4byte 0x0f880064
	.4byte 0x001000e3
	.4byte 0x00000033
	.4byte 0x0f890065
	.4byte 0x001000b5
	.4byte 0x00009415
	.4byte 0x0fd10014
	.4byte 0x020083c1
	.4byte 0x00000003
	.4byte 0xffff005a
	.4byte 0x020086bd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00009285
	.4byte 0xffff0000
	.4byte 0x02008fa1
	.4byte 0x20009285
	.4byte 0xffff0000
	.4byte 0x020091d1
	.4byte 0x00000002
	.4byte 0x02420001
	.4byte 0x02008c75
	.4byte 0x00000002
	.4byte 0x0240000a
	.4byte 0x02008eb1
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x02008b0d
	.4byte 0x00000006
	.4byte 0xffff005b
	.4byte 0x02008ff1
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020088ed
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008981
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x020089d5
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x02008a91
	.4byte 0x00000003
	.4byte 0xffff005a
	.4byte 0x020086bd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00230026
	.4byte 0x00020002
	.4byte 0x00280005
	.4byte 0x00020023
	.4byte 0x00050002
	.4byte 0x002affff
	.4byte 0x00020023
	.4byte 0x00050002
	.4byte 0x0023002c
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x0021002e
	.4byte 0x00020002
	.4byte 0x00300005
	.4byte 0x00020021
	.4byte 0x00050002
	.4byte 0x002effff
	.4byte 0x00020023
	.4byte 0x00050002
	.4byte 0x00230030
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x00210032
	.4byte 0x00020002
	.4byte 0x00340005
	.4byte 0x00020021
	.4byte 0x00050002
	.4byte 0x0032ffff
	.4byte 0x00020023
	.4byte 0x00050002
	.4byte 0x00230034
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x0021002a
	.4byte 0x00020004
	.4byte 0x00260005
	.4byte 0x00040021
	.4byte 0x00050002
	.4byte 0x0036ffff
	.4byte 0x00020021
	.4byte 0x00050002
	.4byte 0x00210038
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x0200a45c
	.4byte 0x0009002e
	.4byte 0x0200a472
	.4byte 0x000c0034
	.4byte 0x0200a488
	.4byte 0x00120034
	.4byte 0x0200a49e
	.4byte 0x0014002d
	.4byte 0x0200a4b4
	.4byte 0x00050033
	.4byte 0x0200a4ca
	.4byte 0x00090025
	.4byte 0x0200a4e0
	.4byte 0x00060021
	.4byte 0x0200a4f6
	.4byte 0x0011002c
	.4byte 0x00000000
