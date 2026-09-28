.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #76]
	movs r2, #142
	ldr r6, [r3]
	ldr r3, [pc, #72]
	lsls r2, r2, #2
	adds r7, r3, r2
	movs r3, #214
	lsls r3, r3, #1
	adds r5, r6, r3
	ldr r3, [r5]
	lsls r0, r3, #3
	adds r0, r0, r3
	movs r1, #10
	bl 0x0200c174
	ldr r3, [r7]
	cmp r3, r0
	blt .L_02000030_0
	bl 0x0200c19c
	movs r2, #128
	lsls r2, r2, #8
	cmp r0, r2
	bcs .L_02000030_1
	ldr r0, [pc, #36]
	movs r1, #3
	bl 0x0200c41c
	movs r3, #212
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #0
	str r3, [r2]
	b .L_02000030_0
.L_02000030_1:
	ldr r3, [r5]
	str r3, [r7]
.L_02000030_0:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000808
	.global Func_0200008c
	.thumb_func
Func_0200008c:
	push {r5, r6, lr}
	ldr r3, [pc, #56]
	ldr r5, [pc, #56]
	movs r2, #142
	lsls r2, r2, #2
	ldr r6, [r3]
	adds r5, r5, r2
	subs r2, #140
	adds r3, r6, r2
	ldr r3, [r3]
	lsls r0, r3, #3
	adds r0, r0, r3
	movs r1, #10
	bl 0x0200c174
	ldr r3, [r5]
	cmp r3, r0
	blt .L_0200008c_0
	ldr r0, [pc, #28]
	movs r1, #42
	bl 0x0200c41c
	movs r3, #212
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #0
	str r3, [r2]
.L_0200008c_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000809
	.global Func_020000d4
	.thumb_func
Func_020000d4:
	push {r5, r6, lr}
	ldr r3, [pc, #56]
	ldr r5, [pc, #56]
	movs r2, #142
	lsls r2, r2, #2
	ldr r6, [r3]
	adds r5, r5, r2
	subs r2, #140
	adds r3, r6, r2
	ldr r3, [r3]
	lsls r0, r3, #3
	adds r0, r0, r3
	movs r1, #10
	bl 0x0200c174
	ldr r3, [r5]
	cmp r3, r0
	blt .L_020000d4_0
	ldr r0, [pc, #28]
	movs r1, #24
	bl 0x0200c41c
	movs r3, #212
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #0
	str r3, [r2]
.L_020000d4_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000080a
	.global Func_0200011c
	.thumb_func
Func_0200011c:
	push {lr}
	adds r2, r0, #0
	adds r2, #100
	movs r4, #0
	ldrsh r3, [r2, r4]
	ldrh r1, [r2]
	cmp r3, #0
	bgt .L_0200011c_0
	adds r3, r1, #1
	strh r3, [r2]
	b .L_0200011c_1
.L_0200011c_0:
	bl 0x0200c214
.L_0200011c_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200013c
	.thumb_func
Func_0200013c:
	push {r5, r6, lr}
	ldr r3, [pc, #116]
	ldr r3, [r3]
	movs r2, #4
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_0
	ldr r3, [pc, #108]
	b .L_0200013c_1
.L_0200013c_0:
	movs r3, #128
	lsls r3, r3, #9
.L_0200013c_1:
	str r3, [r0, #24]
	str r3, [r0, #28]
	ldr r3, [pc, #92]
	ldr r3, [r3]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	ldr r0, [pc, #80]
	bl 0x0200c20c
	adds r6, r0, #0
	movs r0, #246
	bl 0x0200c4a4
	cmp r6, #0
	beq .L_0200013c_2
	adds r3, r6, #0
	adds r3, #85
	movs r5, #0
	strb r5, [r3]
	ldr r1, [r6, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200c244
	adds r0, r6, #0
	movs r1, #1
	bl 0x0200c1fc
	adds r3, r6, #0
	adds r3, #100
	strh r5, [r3]
	ldr r3, [pc, #20]
	str r3, [r6, #108]
.L_0200013c_2:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e40
	.4byte 0x00014ccc
	.4byte 0x0000011d
	.4byte 0x0200811d
	.global Func_020001c4
	.thumb_func
Func_020001c4:
	push {r5, r6, lr}
	ldr r3, [pc, #124]
	ldr r3, [r3]
	movs r2, #2
	ands r3, r2
	adds r6, r0, #0
	cmp r3, #0
	beq .L_020001c4_0
	movs r1, #10
	bl 0x0200c25c
	b .L_020001c4_1
.L_020001c4_0:
	adds r0, r6, #0
	movs r1, #7
	bl 0x0200c25c
.L_020001c4_1:
	adds r3, r6, #0
	adds r3, #102
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020001c4_2
	ldr r3, [pc, #84]
	adds r5, r6, #0
	str r3, [r6, #8]
	adds r5, #100
	movs r3, #0
	ldrsh r0, [r5, r3]
	lsls r0, r0, #3
	bl 0x0200c1a4
	movs r1, #128
	ldr r3, [pc, #68]
	lsls r1, r1, #11
	mov r12, pc
	bx r3
	.2byte 0x2480
	.2byte 0x0364
	.2byte 0x23a6
	.2byte 0x1900
	.2byte 0x04db
	.2byte 0x60f0
	.2byte 0x6133
	.2byte 0x2200
	.2byte 0x5ea9
	.2byte 0x1c32
	.2byte 0x3208
	.2byte 0x1c20
	.2byte 0xf003
	.2byte 0xffc2
	.2byte 0x882b
	.2byte 0x2280
	.2byte 0x01d2
	.2byte 0x189b
	.2byte 0x80f3
	.2byte 0x2280
	.2byte 0x882b
	.2byte 0x00d2
	.2byte 0x189b
	.2byte 0x802b
.L_020001c4_2:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e40
	.4byte 0x15d00000
	.4byte 0x03000118
	.global Func_02000250
	.thumb_func
Func_02000250:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #48
	bl 0x0200c28c
	cmp r0, #0
	bne .L_02000250_0
	movs r0, #183
	lsls r0, r0, #1
	bl 0x0200c28c
	cmp r0, #0
	bne .L_02000250_0
	ldr r3, [pc, #32]
	str r3, [r5, #108]
	adds r3, r5, #0
	adds r3, #85
	strb r0, [r3]
	adds r3, #15
	strh r0, [r3]
	adds r3, #2
	strh r0, [r3]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #24]
	str r3, [r5, #28]
.L_02000250_0:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x020081c5
	.global Func_02000290
	.thumb_func
Func_02000290:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200d27c
	.global Func_02000298
	.thumb_func
Func_02000298:
	movs r0, #0
	bx lr
	.global Func_0200029c
	.thumb_func
Func_0200029c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200da2c
	.global Func_020002a4
	.thumb_func
Func_020002a4:
	push {r5, lr}
	adds r5, r0, #0
	bl 0x020082cc
	ldr r0, [pc, #24]
	bl 0x0200c28c
	cmp r0, #0
	beq .L_020002a4_0
	adds r0, r5, #0
	movs r1, #2
	bl 0x0200c1fc
.L_020002a4_0:
	movs r0, #1
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00000847
	.global Func_020002cc
	.thumb_func
Func_020002cc:
	ldr r3, [pc, #28]
	movs r1, #141
	ldr r2, [r3]
	lsls r1, r1, #1
	adds r2, r2, r1
	ldr r3, [r0, #80]
	ldrh r2, [r2]
	ldr r1, [pc, #12]
	strh r2, [r3, #30]
	adds r3, #38
	strb r1, [r3]
	movs r0, #1
	bx lr
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x03001e70
	.global Func_020002f0
	.thumb_func
Func_020002f0:
	ldr r3, [pc, #28]
	ldr r4, [r0, #80]
	adds r0, #89
	ldrb r2, [r0]
	ldr r1, [r3]
	movs r3, #1
	orrs r3, r2
	movs r2, #141
	lsls r2, r2, #1
	strb r3, [r0]
	adds r3, r1, r2
	ldrh r3, [r3]
	movs r0, #1
	strh r3, [r4, #30]
	bx lr
	.2byte 0x0000
	.4byte 0x03001e70
	.global Func_02000314
	.thumb_func
Func_02000314:
	push {r5, r6, lr}
	adds r5, r0, #0
	movs r1, #0
	bl 0x0200c244
	adds r0, r5, #0
	movs r1, #10
	bl 0x0200c25c
	adds r3, r5, #0
	adds r3, #89
	movs r6, #0
	movs r0, #138
	strb r6, [r3]
	lsls r0, r0, #4
	bl 0x0200c28c
	cmp r0, #0
	beq .L_02000314_0
	ldr r0, [pc, #16]
	bl 0x0200c294
	str r6, [r5, #8]
	str r6, [r5, #12]
.L_02000314_0:
	movs r0, #0
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x000002f1
	.global Func_02000350
	.thumb_func
Func_02000350:
	push {lr}
	adds r0, #84
	ldrb r2, [r0]
	movs r1, #1
	adds r3, r1, #0
	ands r3, r2
	cmp r3, #0
	beq .L_02000350_0
	ldr r3, [pc, #20]
	ldr r3, [r3]
	ands r3, r1
	cmp r3, #0
	beq .L_02000350_0
	movs r3, #1
	eors r3, r2
	strb r3, [r0]
.L_02000350_0:
	movs r0, #1
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001e40
	.global Func_0200037c
	.thumb_func
Func_0200037c:
	push {lr}
	ldr r3, [pc, #228]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #49
	cmp r3, #31
	bhi .L_0200037c_0
	ldr r2, [pc, #212]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	strh r0, [r3, #32]
	lsls r0, r0, #8
	strh r6, [r2, #34]
	lsls r0, r0, #8
	strh r6, [r2, #34]
	lsls r0, r0, #8
	strh r6, [r2, #34]
	lsls r0, r0, #8
	strh r6, [r2, #34]
	lsls r0, r0, #8
	strh r6, [r2, #34]
	lsls r0, r0, #8
	strh r6, [r2, #34]
	lsls r0, r0, #8
	strh r6, [r2, #34]
	lsls r0, r0, #8
	strh r6, [r2, #34]
	lsls r0, r0, #8
	strh r6, [r2, #34]
	lsls r0, r0, #8
	strh r6, [r2, #34]
	lsls r0, r0, #8
	strh r6, [r2, #34]
	lsls r0, r0, #8
	strh r6, [r2, #34]
	lsls r0, r0, #8
	strh r6, [r2, #34]
	lsls r0, r0, #8
	strh r6, [r2, #34]
	lsls r0, r0, #8
	strh r0, [r6, #32]
	lsls r0, r0, #8
	strh r6, [r7, #32]
	lsls r0, r0, #8
	strh r6, [r1, #34]
	lsls r0, r0, #8
	strh r6, [r1, #34]
	lsls r0, r0, #8
	strh r6, [r1, #34]
	lsls r0, r0, #8
	strh r6, [r1, #34]
	lsls r0, r0, #8
	strh r6, [r7, #32]
	lsls r0, r0, #8
	strh r2, [r0, #34]
	lsls r0, r0, #8
	strh r6, [r0, #34]
	lsls r0, r0, #8
	strh r2, [r1, #34]
	lsls r0, r0, #8
	strh r6, [r2, #34]
	lsls r0, r0, #8
	strh r6, [r1, #34]
	lsls r0, r0, #8
	strh r6, [r2, #34]
	lsls r0, r0, #8
	strh r6, [r2, #34]
	lsls r0, r0, #8
	strh r6, [r2, #34]
	lsls r0, r0, #8
	strh r6, [r2, #34]
	lsls r0, r0, #8
	strh r2, [r2, #34]
	lsls r0, r0, #8
	ldr r0, [pc, #80]
	bl 0x0200c28c
	cmp r0, #0
	bne .L_0200037c_0
	ldr r0, [pc, #76]
	bl 0x0200c28c
	cmp r0, #0
	beq .L_0200037c_0
	ldr r0, [pc, #68]
	b .L_0200037c_1
	.2byte 0x4811
	.2byte 0xf003
	.2byte 0xff2b
	.2byte 0x2800
	.2byte 0xd10d
	.2byte 0x4810
	.2byte 0xe00f
	.2byte 0x4810
	.2byte 0xe00d
	.2byte 0x4810
	.2byte 0xe00b
	.2byte 0x4810
	.2byte 0xe009
	.2byte 0x4810
	.2byte 0xe007
	.2byte 0x4810
	.2byte 0xe005
	.2byte 0x4810
	.2byte 0xe003
.L_0200037c_0:
	ldr r0, [pc, #64]
	bl 0x0200c294
	ldr r0, [pc, #60]
.L_0200037c_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02008398
	.4byte 0x0000094f
	.4byte 0x00000941
	.4byte 0x0200e154
	.2byte 0x085a
	.2byte 0x0000
	.2byte 0xe04c
	.2byte 0x0200
	.2byte 0xe1e4
	.2byte 0x0200
	.2byte 0xe28c
	.2byte 0x0200
	.2byte 0xe394
	.2byte 0x0200
	.2byte 0xe3c4
	.2byte 0x0200
	.2byte 0xe25c
	.2byte 0x0200
	.2byte 0xe2ec
	.2byte 0x0200
	.4byte 0x00000235
	.4byte 0x0200db84
	.global Func_020004a0
	.thumb_func
Func_020004a0:
	push {r5, r6, r7, lr}
	subs r0, #100
	adds r7, r1, #0
	adds r6, r2, #0
	bl 0x0200c2fc
	ldr r3, [pc, #56]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl 0x0200c2fc
	ldr r3, [pc, #44]
	ldr r2, [r0, #8]
	ldr r1, [r3]
	ldr r3, [r5, #8]
	cmp r2, r3
	bge .L_020004a0_0
	movs r2, #184
	lsls r2, r2, #1
	adds r3, r1, r2
	strh r7, [r3]
	b .L_020004a0_1
.L_020004a0_0:
	movs r2, #184
	lsls r2, r2, #1
	adds r3, r1, r2
	strh r6, [r3]
.L_020004a0_1:
	movs r0, #123
	bl 0x0200c4a4
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x03001ebc
	.global Func_020004f0
	.thumb_func
Func_020004f0:
	push {r5, r6, r7, lr}
	subs r0, #100
	adds r7, r1, #0
	adds r6, r2, #0
	bl 0x0200c2fc
	ldr r3, [pc, #56]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl 0x0200c2fc
	ldr r3, [pc, #44]
	ldr r2, [r0, #16]
	ldr r1, [r3]
	ldr r3, [r5, #16]
	cmp r2, r3
	bge .L_020004f0_0
	movs r2, #184
	lsls r2, r2, #1
	adds r3, r1, r2
	strh r7, [r3]
	b .L_020004f0_1
.L_020004f0_0:
	movs r2, #184
	lsls r2, r2, #1
	adds r3, r1, r2
	strh r6, [r3]
.L_020004f0_1:
	movs r0, #123
	bl 0x0200c4a4
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x03001ebc
	.global Func_02000540
	.thumb_func
Func_02000540:
	push {lr}
	movs r0, #130
	movs r1, #6
	movs r2, #47
	bl 0x020084a0
	pop {r0}
	bx r0
	.global Func_02000550
	.thumb_func
Func_02000550:
	push {lr}
	movs r0, #150
	movs r1, #46
	movs r2, #11
	bl 0x020084f0
	pop {r0}
	bx r0
	.global Func_02000560
	.thumb_func
Func_02000560:
	push {lr}
	movs r0, #116
	movs r1, #56
	movs r2, #21
	bl 0x020084f0
	pop {r0}
	bx r0
	.global Func_02000570
	.thumb_func
Func_02000570:
	push {lr}
	movs r0, #151
	movs r1, #25
.L_02000576:
	movs r2, #54
	bl 0x020084f0
	pop {r0}
	bx r0
	.global Func_02000580
	.thumb_func
Func_02000580:
	push {lr}
	movs r0, #125
	movs r1, #59
	movs r2, #30
	bl 0x020084f0
	pop {r0}
	bx r0
	.global Func_02000590
	.thumb_func
Func_02000590:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200e3f4
	.global Func_02000598
	.thumb_func
Func_02000598:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #244]
	movs r1, #250
	lsls r1, r1, #1
	adds r3, r3, r1
	ldr r0, [r3]
	sub sp, #8
	bl 0x0200c2fc
	adds r7, r0, #0
	ldr r3, [r7, #80]
	ldrh r2, [r7, #32]
	ldr r3, [r3, #24]
	adds r1, r2, #0
	muls r1, r3
	ldr r3, [pc, #220]
	str r1, [sp, #0]
	ldr r3, [r3]
	mov r8, r3
	movs r3, #240
	lsls r3, r3, #1
	add r3, r8
	ldr r5, [r3]
	ldr r2, [pc, #208]
	ldr r3, [r5, #8]
	movs r1, #160
	adds r2, r3, r2
	str r2, [sp, #4]
	lsls r1, r1, #16
	adds r1, r1, r3
	mov r11, r1
	ldr r3, [r5, #16]
	ldr r2, [pc, #192]
	movs r1, #200
	lsls r1, r1, #16
	adds r2, r2, r3
	adds r1, r1, r3
	mov r9, r2
	mov r10, r1
	movs r6, #8
.L_02000598_7:
	adds r0, r6, #0
	bl 0x0200c46c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000598_0
	ldr r3, [r5, #8]
	ldr r1, [sp, #4]
	ldr r2, [r5, #16]
	cmp r3, r1
	blt .L_02000598_1
	cmp r3, r11
	bgt .L_02000598_1
	cmp r2, r9
	blt .L_02000598_1
	cmp r2, r10
	ble .L_02000598_2
.L_02000598_1:
	adds r2, r5, #0
	adds r2, #84
	movs r3, #0
	strb r3, [r2]
	b .L_02000598_0
.L_02000598_2:
	adds r2, r5, #0
	movs r3, #1
	adds r2, #84
	strb r3, [r2]
	ldr r3, [pc, #128]
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_02000598_3
	ldr r0, [pc, #124]
	bl 0x0200c28c
	cmp r0, #0
	bne .L_02000598_0
.L_02000598_3:
	ldr r3, [r5, #80]
	ldr r2, [r5, #8]
	ldr r1, [r3, #24]
	ldr r3, [r7, #8]
	subs r4, r2, r3
	cmp r4, #0
	bge .L_02000598_4
	subs r4, r3, r2
.L_02000598_4:
	ldrh r3, [r5, #32]
	muls r3, r1
	ldr r1, [sp, #0]
	ldr r0, [r5, #16]
	adds r2, r1, r3
	ldr r1, [r7, #16]
	subs r3, r0, r1
	cmp r3, #0
	blt .L_02000598_5
	adds r3, r4, r3
	cmp r3, r2
	blt .L_02000598_6
	b .L_02000598_0
.L_02000598_5:
	subs r3, r1, r0
	adds r3, r4, r3
	cmp r3, r2
	bge .L_02000598_0
.L_02000598_6:
	movs r0, #130
	lsls r0, r0, #1
	bl 0x0200c28c
	cmp r0, #0
	bne .L_02000598_0
	movs r3, #182
	adds r2, r6, #0
	lsls r3, r3, #1
	adds r2, #100
	add r3, r8
	strh r2, [r3]
.L_02000598_0:
	adds r6, #1
	cmp r6, #65
	bls .L_02000598_7
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x03001ebc
	.4byte 0xff600000
	.4byte 0xfed40000
	.4byte 0x03001f54
	.4byte 0x00000163
	.global Func_020006b4
	.thumb_func
Func_020006b4:
	push {lr}
	ldr r0, [pc, #32]
	bl 0x0200c28c
	cmp r0, #0
	bne .L_020006b4_0
	movs r0, #101
	bl 0x0200c40c
	b .L_020006b4_1
.L_020006b4_0:
	movs r0, #123
	bl 0x0200c4a4
	movs r0, #3
	bl 0x0200c40c
.L_020006b4_1:
	pop {r0}
	bx r0
	.4byte 0x0000085a
	.global Func_020006dc
	.thumb_func
Func_020006dc:
	push {lr}
	bl 0x0200c2dc
	movs r0, #74
	bl 0x0200c40c
	pop {r0}
	bx r0
	.global Func_020006ec
	.thumb_func
Func_020006ec:
	push {r5, lr}
	ldr r3, [pc, #836]
	movs r1, #225
	lsls r1, r1, #1
	adds r5, r3, r1
	movs r1, #0
	ldrsh r3, [r5, r1]
	ldrh r2, [r5]
	cmp r3, #99
	bne .L_020006ec_0
	movs r0, #176
	lsls r0, r0, #1
	bl 0x0200c294
	ldr r0, [pc, #812]
	bl 0x0200c294
	ldr r0, [pc, #812]
	bl 0x0200c294
	ldrh r2, [r5]
.L_020006ec_0:
	lsls r3, r2, #16
	asrs r3, r3, #16
	cmp r3, #90
	bne .L_020006ec_1
	movs r0, #0
	bl 0x0200b9fc
	ldr r0, [pc, #792]
	movs r1, #1
	bl 0x0200c404
	b .L_020006ec_2
.L_020006ec_1:
	cmp r3, #91
	bne .L_020006ec_3
	movs r0, #1
	bl 0x0200b9fc
	ldr r0, [pc, #776]
	movs r1, #93
	bl 0x0200c404
	b .L_020006ec_2
.L_020006ec_3:
	cmp r3, #78
	bne .L_020006ec_4
	bl 0x0200c2dc
	movs r0, #242
	bl 0x0200c2cc
	movs r0, #112
	bl 0x0200c40c
	b .L_020006ec_2
.L_020006ec_4:
	movs r0, #162
	lsls r0, r0, #1
	bl 0x0200c294
	ldr r3, [pc, #740]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #128
	lsls r3, r3, #3
	str r3, [r2]
	movs r3, #228
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #16
	str r3, [r2]
	movs r0, #1
	bl 0x0200c184
	movs r0, #128
	movs r1, #128
	lsls r1, r1, #9
	lsls r0, r0, #12
	bl 0x0200c3ec
	ldr r0, [pc, #700]
	bl 0x0200c29c
	movs r1, #200
	ldr r0, [pc, #696]
	lsls r1, r1, #4
	bl 0x0200c18c
	ldr r0, [pc, #692]
	bl 0x0200c28c
	cmp r0, #0
	bne .L_020006ec_5
	movs r1, #128
	movs r0, #128
	lsls r1, r1, #1
	movs r2, #176
	movs r3, #56
	bl 0x0200c23c
.L_020006ec_5:
	movs r1, #0
	ldrsh r3, [r5, r1]
	subs r3, #1
	cmp r3, #79
	bls .L_020006ec_6
	b .L_020006ec_7
.L_020006ec_6:
	ldr r2, [pc, #660]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	ldrh r0, [r1, #8]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r2, [r4, #8]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r0, #12]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r2, [r3, #12]
	lsls r0, r0, #8
	ldrh r2, [r5, #12]
	lsls r0, r0, #8
	ldrh r0, [r6, #12]
	lsls r0, r0, #8
	ldrh r6, [r6, #12]
	lsls r0, r0, #8
	ldrh r4, [r7, #12]
	lsls r0, r0, #8
	ldrh r2, [r0, #14]
	lsls r0, r0, #8
	ldrh r0, [r1, #14]
	lsls r0, r0, #8
	ldrh r6, [r1, #14]
	lsls r0, r0, #8
	ldrh r4, [r2, #14]
	lsls r0, r0, #8
	ldrh r2, [r3, #14]
	lsls r0, r0, #8
	ldrh r0, [r4, #14]
	lsls r0, r0, #8
	ldrh r4, [r1, #16]
	lsls r0, r0, #8
	ldrh r0, [r4, #14]
	lsls r0, r0, #8
	ldrh r0, [r4, #14]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r0, [r3, #16]
	lsls r0, r0, #8
	ldrh r2, [r2, #16]
	lsls r0, r0, #8
	ldr r0, [pc, #336]
	bl 0x0200c28c
	cmp r0, #0
	beq .L_020006ec_8
	b .L_020006ec_2
.L_020006ec_8:
	ldr r0, [pc, #324]
	bl 0x0200c294
	ldr r0, [pc, #324]
	bl 0x0200c294
	b .L_020006ec_2
	.2byte 0x4850
	.2byte 0xf003
	.2byte 0xfcb2
	.2byte 0x2800
	.2byte 0xd01c
	.2byte 0x484e
	.2byte 0xf003
	.2byte 0xfcad
	.2byte 0x2800
	.2byte 0xd17a
	.2byte 0x208d
	.2byte 0x0080
	.2byte 0xf003
	.2byte 0xfca7
	.2byte 0x2800
	.2byte 0xd074
	.2byte 0x4d4a
	.2byte 0x2337
	.2byte 0x602b
	.2byte 0x2037
	.2byte 0x4949
	.2byte 0x4a49
	.2byte 0xf003
	.2byte 0xfd05
	.2byte 0x6828
	.2byte 0xf003
	.2byte 0xfcd2
	.2byte 0x23c0
	.2byte 0x019b
	.2byte 0x80c3
	.2byte 0x6828
	.2byte 0xf001
	.2byte 0xff02
	.2byte 0xe062
	.2byte 0x4840
	.2byte 0xf003
	.2byte 0xfc90
	.2byte 0x2800
	.2byte 0xd15d
	.2byte 0x4841
	.2byte 0xf003
	.2byte 0xfc8b
	.2byte 0x2800
	.2byte 0xd158
	.2byte 0xf001
	.2byte 0xfd95
	.2byte 0xe055
	.2byte 0x483e
	.2byte 0xf003
	.2byte 0xfc83
	.2byte 0x2800
	.2byte 0xd150
	.2byte 0x483d
	.2byte 0xf003
	.2byte 0xfc7e
	.2byte 0x2800
	.2byte 0xd04b
	.2byte 0xf000
	.2byte 0xf87a
	.2byte 0xe048
	.2byte 0x483a
	.2byte 0xf003
	.2byte 0xfc76
	.2byte 0x2800
	.2byte 0xd143
	.2byte 0xf000
	.2byte 0xf940
	.2byte 0xe040
	.2byte 0xf000
	.2byte 0xff27
	.2byte 0xe03d
	.2byte 0xf000
	.2byte 0xff6a
	.2byte 0xe03a
	.2byte 0xf000
	.2byte 0xffbf
	.2byte 0xe037
	.2byte 0xf001
	.2byte 0xf814
	.2byte 0xe034
	.2byte 0xf001
	.2byte 0xf869
	.2byte 0xe031
	.2byte 0xf001
	.2byte 0xf8c8
	.2byte 0xe02e
	.2byte 0xf001
	.2byte 0xfc51
	.2byte 0xe02b
	.2byte 0xf002
	.2byte 0xfdd2
	.2byte 0xe028
	.2byte 0xf002
	.2byte 0xfba9
	.2byte 0xe025
	.2byte 0x208e
	.2byte 0x0040
	.2byte 0xf003
	.2byte 0xfc56
	.2byte 0x20be
	.2byte 0x0080
	.2byte 0xf003
	.2byte 0xfc5a
	.2byte 0x2800
	.2byte 0xd01b
	.2byte 0x4b0f
	.2byte 0x21f9
	.2byte 0x0049
	.2byte 0x185a
	.2byte 0x2302
	.2byte 0x21c8
	.2byte 0x7013
	.2byte 0x4821
	.2byte 0x0109
	.2byte 0xf003
	.2byte 0xfbc1
	.2byte 0xe00f
	.2byte 0xf002
	.2byte 0xfc70
	.2byte 0xe00c
	.2byte 0xf002
	.2byte 0xf94f
	.2byte 0xe009
.L_020006ec_7:
	movs r0, #53
	bl 0x0200c2fc
	movs r5, #160
	lsls r5, r5, #9
	str r5, [r0, #24]
	movs r0, #53
	bl 0x0200c2fc
	str r5, [r0, #28]
.L_020006ec_2:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000161
	.4byte 0x00000163
	.4byte 0x0000003a
	.4byte 0x000000bb
	.4byte 0x03001ebc
	.4byte 0x0000012f
	.4byte 0x02008599
	.4byte 0x0000090a
	.4byte 0x020087c8
	.4byte 0x00000815
	.4byte 0x0000085c
	.2byte 0x0109
	.2byte 0x0000
	.2byte 0x085d
	.2byte 0x0000
	.2byte 0xe79c
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0x1794
	.2byte 0x0000
	.2byte 0x0d48
	.2byte 0x09b8
	.2byte 0x0000
	.2byte 0x094f
	.2byte 0x0000
	.2byte 0x0941
	.2byte 0x0000
	.2byte 0x085a
	.2byte 0x0000
	.2byte 0xb679
	.2byte 0x0200
	.global Func_02000a8c
	.thumb_func
Func_02000a8c:
	push {lr}
	bl 0x0200c2dc
	bl 0x0200c43c
	bl 0x0200c44c
	bl 0x0200c45c
	ldr r0, [pc, #380]
	bl 0x0200c294
	movs r1, #183
	movs r0, #11
	lsls r1, r1, #21
	ldr r2, [pc, #372]
	bl 0x0200c35c
	movs r1, #24
	movs r2, #8
	movs r0, #11
	bl 0x0200c34c
	movs r0, #11
	bl 0x0200c354
	movs r0, #60
	bl 0x0200c2d4
	movs r1, #183
	movs r0, #12
	lsls r1, r1, #21
	ldr r2, [pc, #336]
	bl 0x0200c35c
	movs r1, #12
	movs r2, #24
	movs r0, #12
	bl 0x0200c34c
	movs r0, #30
	bl 0x0200c2d4
	movs r1, #160
	movs r0, #11
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c3c4
	movs r1, #208
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #12
	bl 0x0200c3c4
	movs r0, #60
	bl 0x0200c2d4
	movs r0, #11
	movs r1, #3
	bl 0x0200c364
	movs r1, #3
	movs r0, #12
	bl 0x0200c364
	movs r0, #120
	bl 0x0200c2d4
	movs r2, #151
	ldr r1, [pc, #264]
	lsls r2, r2, #19
	movs r0, #8
	bl 0x0200c35c
	movs r0, #60
	bl 0x0200c2d4
	movs r0, #12
	movs r1, #2
	bl 0x0200c364
	movs r0, #8
	bl 0x0200c2fc
	cmp r0, #0
	beq .L_02000a8c_0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #12
	bl 0x0200c32c
.L_02000a8c_0:
	movs r0, #12
	bl 0x0200c354
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl 0x0200c35c
	movs r0, #60
	bl 0x0200c2d4
	movs r0, #11
	movs r1, #2
	bl 0x0200c364
	movs r0, #8
	bl 0x0200c2fc
	cmp r0, #0
	beq .L_02000a8c_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #11
	bl 0x0200c32c
.L_02000a8c_1:
	movs r0, #11
	bl 0x0200c354
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl 0x0200c35c
	movs r0, #60
	bl 0x0200c2d4
	movs r0, #0
	movs r1, #2
	bl 0x0200c364
	movs r0, #8
	bl 0x0200c2fc
	cmp r0, #0
	beq .L_02000a8c_2
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #0
	bl 0x0200c32c
.L_02000a8c_2:
	movs r0, #0
	bl 0x0200c354
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl 0x0200c35c
	movs r0, #60
	bl 0x0200c2d4
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200c30c
	movs r1, #56
	movs r2, #8
	movs r0, #8
	bl 0x0200c34c
	movs r0, #8
	bl 0x0200c354
	movs r1, #40
	movs r2, #40
	movs r0, #8
	bl 0x0200c34c
	movs r0, #8
	bl 0x0200c354
	movs r1, #8
	movs r2, #88
	movs r0, #8
	bl 0x0200c34c
	movs r0, #8
	bl 0x0200c354
	bl 0x0200c444
	movs r0, #108
	bl 0x0200c40c
	bl 0x0200c2e4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000094f
	.4byte 0x049c0000
	.4byte 0x16f80000
	.global Func_02000c28
	.thumb_func
Func_02000c28:
	push {r5, r6, lr}
	movs r0, #15
	bl 0x0200c2fc
	adds r6, r0, #0
	bl 0x0200c2dc
	movs r0, #160
	movs r1, #1
	lsls r0, r0, #9
	bl 0x0200c48c
	movs r0, #4
	bl 0x0200c184
	bl 0x0200c43c
	bl 0x0200c44c
	bl 0x0200c45c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl 0x0200c3f4
	movs r0, #1
	bl 0x0200c184
	movs r0, #0
	ldr r1, [pc, #1012]
	ldr r2, [pc, #1016]
	bl 0x0200c30c
	movs r0, #1
	ldr r1, [pc, #1004]
	ldr r2, [pc, #1004]
	bl 0x0200c30c
	movs r2, #197
	movs r0, #0
	ldr r1, [pc, #1000]
	lsls r2, r2, #3
	bl 0x0200c344
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c3c4
	movs r2, #197
	lsls r2, r2, #19
	ldr r1, [pc, #980]
	movs r0, #8
	bl 0x0200c35c
	movs r0, #1
	bl 0x0200c184
	movs r1, #15
	movs r0, #8
	bl 0x0200c394
	movs r0, #8
	bl 0x0200c2fc
	movs r1, #0
	bl 0x0200c244
	movs r0, #10
	ldr r1, [pc, #932]
	ldr r2, [pc, #944]
	bl 0x0200c30c
	movs r0, #11
	ldr r1, [pc, #920]
	ldr r2, [pc, #936]
	bl 0x0200c30c
	movs r0, #12
	ldr r1, [pc, #912]
	ldr r2, [pc, #924]
	bl 0x0200c30c
	ldr r2, [pc, #920]
	ldr r1, [pc, #900]
	movs r0, #13
	bl 0x0200c30c
	movs r0, #141
	bl 0x0200c4a4
	ldr r1, [pc, #908]
	movs r0, #10
	bl 0x0200c314
	movs r0, #20
	bl 0x0200c2d4
	ldr r1, [pc, #896]
	movs r0, #11
	bl 0x0200c314
	movs r0, #20
	bl 0x0200c2d4
	ldr r1, [pc, #888]
	movs r0, #12
	bl 0x0200c314
	movs r0, #20
	bl 0x0200c2d4
	ldr r1, [pc, #876]
	movs r0, #13
	bl 0x0200c324
	ldr r0, [pc, #872]
	bl 0x0200c4a4
	movs r0, #0
	bl 0x0200c2fc
	cmp r0, #0
	beq .L_02000c28_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200c35c
.L_02000c28_0:
	movs r2, #200
	movs r0, #1
	ldr r1, [pc, #848]
	lsls r2, r2, #3
	bl 0x0200c344
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c3c4
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200c3c4
	movs r0, #0
	ldr r1, [pc, #816]
	movs r2, #0
	bl 0x0200c3d4
	movs r0, #1
	ldr r1, [pc, #808]
	movs r2, #60
	bl 0x0200c3d4
	movs r2, #199
	ldr r1, [pc, #764]
	lsls r2, r2, #19
	movs r0, #8
	bl 0x0200c35c
	movs r0, #1
	bl 0x0200c184
	ldr r0, [pc, #784]
	bl 0x0200c3a4
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x0200c3bc
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c3c4
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #40
	bl 0x0200c3c4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c3c4
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #60
	bl 0x0200c3c4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c3c4
	movs r1, #192
	movs r2, #10
	lsls r1, r1, #7
	movs r0, #1
	bl 0x0200c3c4
	movs r0, #141
	bl 0x0200c4a4
	ldr r1, [pc, #696]
	movs r0, #10
	bl 0x0200c314
	movs r0, #20
	bl 0x0200c2d4
	ldr r1, [pc, #684]
	movs r0, #11
	bl 0x0200c314
	movs r0, #20
	bl 0x0200c2d4
	ldr r1, [pc, #676]
	movs r0, #12
	bl 0x0200c314
	movs r0, #10
	bl 0x0200c2d4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c3c4
	movs r1, #128
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200c3c4
	ldr r1, [pc, #640]
	movs r0, #13
	bl 0x0200c324
	ldr r0, [pc, #604]
	bl 0x0200c4a4
	movs r0, #20
	bl 0x0200c2d4
	bl 0x0200c464
	movs r0, #10
	movs r1, #1
	bl 0x0200c364
	movs r0, #11
	movs r1, #1
	bl 0x0200c364
	movs r0, #12
	movs r1, #1
	bl 0x0200c364
	movs r0, #13
	movs r1, #1
	bl 0x0200c364
	movs r1, #1
	movs r2, #223
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #19
	ldr r0, [pc, #580]
	bl 0x0200c3f4
	bl 0x0200c3fc
	movs r0, #20
	bl 0x0200c2d4
	bl 0x0200c45c
	movs r2, #219
	ldr r1, [pc, #556]
	lsls r2, r2, #19
	movs r0, #9
	bl 0x0200c35c
	movs r0, #1
	bl 0x0200c184
	movs r0, #9
	ldr r1, [pc, #544]
	ldr r2, [pc, #544]
	bl 0x0200c30c
	movs r2, #217
	movs r0, #9
	ldr r1, [pc, #540]
	lsls r2, r2, #3
	bl 0x0200c344
	movs r2, #217
	movs r0, #9
	ldr r1, [pc, #532]
	lsls r2, r2, #3
	bl 0x0200c344
	movs r2, #223
	lsls r2, r2, #3
	ldr r1, [pc, #520]
	movs r0, #9
	bl 0x0200c344
	movs r0, #20
	bl 0x0200c2d4
	movs r1, #2
	movs r0, #9
	bl 0x0200c384
	movs r0, #20
	bl 0x0200c2d4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl 0x0200c3dc
	movs r0, #60
	bl 0x0200c2d4
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl 0x0200c3c4
	movs r0, #9
	movs r1, #3
	bl 0x0200c37c
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x0200c3bc
	movs r2, #223
	lsls r2, r2, #19
	ldr r1, [pc, #448]
	movs r0, #8
	bl 0x0200c35c
	movs r0, #1
	bl 0x0200c184
	movs r1, #0
	movs r0, #8
	bl 0x0200c394
	movs r0, #8
	bl 0x0200c2fc
	movs r1, #1
	bl 0x0200c244
	movs r0, #8
	ldr r1, [pc, #328]
	ldr r2, [pc, #336]
	bl 0x0200c30c
	movs r2, #223
	lsls r2, r2, #3
	ldr r1, [pc, #392]
	movs r0, #8
	bl 0x0200c344
	movs r0, #20
	bl 0x0200c2d4
	movs r0, #8
	movs r1, #2
	bl 0x0200c37c
	ldr r0, [pc, #380]
	movs r1, #0
	movs r2, #10
	bl 0x0200c3bc
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #60
	bl 0x0200c3c4
	movs r1, #128
	movs r2, #10
	movs r0, #8
	lsls r1, r1, #8
	bl 0x0200c3c4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl 0x0200c3dc
	movs r0, #60
	bl 0x0200c2d4
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c3c4
	movs r1, #192
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #6
	bl 0x0200c3c4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl 0x0200c3dc
	movs r0, #60
	bl 0x0200c2d4
	movs r0, #8
	movs r1, #2
	bl 0x0200c37c
	movs r2, #40
	ldr r0, [pc, #280]
	movs r1, #0
	bl 0x0200c3bc
	movs r0, #9
	movs r1, #1
	bl 0x0200c384
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200c3c4
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200c3bc
	movs r0, #8
	ldr r1, [pc, #248]
	movs r2, #60
	bl 0x0200c3d4
	movs r2, #10
	ldr r0, [pc, #232]
	movs r1, #0
	bl 0x0200c3bc
	movs r0, #8
	movs r1, #1
	bl 0x0200c384
	movs r0, #8
	movs r1, #3
	bl 0x0200c36c
	ldr r0, [pc, #208]
	movs r1, #0
	movs r2, #10
	bl 0x0200c3bc
	movs r0, #9
	ldr r1, [pc, #148]
	movs r2, #60
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl 0x0200c3bc
	movs r1, #3
	movs r0, #8
	bl 0x0200c36c
	bl 0x0200c464
	movs r0, #107
	bl 0x0200c4a4
	movs r0, #128
	movs r1, #128
	lsls r1, r1, #11
	lsls r0, r0, #11
	bl 0x0200c3ec
	bl 0x0200955c
	ldr r0, [pc, #88]
	bl 0x0200c4a4
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200c3d4
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200c3d4
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c3c4
	movs r0, #9
	movs r1, #0
	movs r2, #40
	bl 0x0200c3c4
	b .L_02000c28_1
	.2byte 0x0000
	.4byte 0x00019999
	.4byte 0x0000cccc
	.4byte 0x000016fc
	.4byte 0x16d80000
	.4byte 0x00006666
	.4byte 0x0200c9e4
	.4byte 0x0200cb50
	.4byte 0x0200cc74
	.4byte 0x0200cd98
	.4byte 0x00000121
	.4byte 0x00001704
	.4byte 0x00000101
	.4byte 0x00001215
	.4byte 0x0200ca78
	.4byte 0x0200cbe4
	.4byte 0x0200cd08
	.4byte 0x0200ce2c
	.4byte 0x16080000
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x00001608
	.4byte 0x000015f8
	.4byte 0x16180000
	.4byte 0x00002008
	.4byte 0x00000105
.L_02000c28_1:
	movs r1, #176
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c3c4
	movs r1, #176
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200c3c4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200c3ec
	movs r1, #1
	movs r2, #217
	movs r3, #1
	ldr r0, [pc, #1008]
	negs r1, r1
	lsls r2, r2, #19
	bl 0x0200c3f4
	bl 0x0200c3fc
	movs r2, #213
	ldr r1, [pc, #996]
	lsls r2, r2, #19
	movs r0, #14
	bl 0x0200c35c
	movs r0, #1
	bl 0x0200c184
	ldr r2, [pc, #984]
	movs r0, #14
	ldr r1, [pc, #984]
	bl 0x0200c30c
	ldr r1, [pc, #980]
	movs r0, #14
	bl 0x0200c314
	movs r0, #160
	bl 0x0200c2d4
	ldr r3, [pc, #972]
	str r3, [r6, #72]
	str r3, [r6, #68]
	movs r3, #192
	lsls r3, r3, #9
	adds r5, r6, #0
	str r3, [r6, #24]
	str r3, [r6, #28]
	adds r5, #100
	movs r3, #0
	strh r3, [r5]
	movs r3, #128
	lsls r3, r3, #15
	ldr r2, [r6, #80]
	str r3, [r6, #12]
	movs r3, #240
	lsls r3, r3, #8
	strh r3, [r2, #30]
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200c244
	movs r1, #2
	adds r0, r6, #0
	bl 0x0200c1fc
	movs r0, #1
	bl 0x0200c184
	movs r0, #15
	bl 0x0200c2fc
	movs r1, #0
	bl 0x0200c244
	movs r1, #200
	ldr r0, [pc, #904]
	lsls r1, r1, #4
	bl 0x0200c18c
.L_02000c28_2:
	movs r0, #1
	bl 0x0200c184
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	beq .L_02000c28_2
	movs r0, #15
	bl 0x0200c2fc
	movs r1, #0
	bl 0x0200c244
	movs r0, #14
	bl 0x0200c2fc
	movs r1, #0
	bl 0x0200c244
	movs r0, #10
	bl 0x0200c2d4
	movs r0, #9
	bl 0x0200c2fc
	movs r5, #192
	lsls r5, r5, #11
	str r5, [r0, #40]
	movs r0, #8
	bl 0x0200c2fc
	str r5, [r0, #40]
	movs r0, #145
	bl 0x0200c4a4
	movs r0, #128
	movs r1, #128
	lsls r1, r1, #11
	lsls r0, r0, #11
	bl 0x0200c3ec
	bl 0x02009680
	bl 0x02009680
	movs r0, #60
	bl 0x0200c2d4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200c3ec
	movs r1, #1
	movs r2, #223
	movs r3, #1
	lsls r2, r2, #19
	ldr r0, [pc, #784]
	negs r1, r1
	bl 0x0200c3f4
	bl 0x0200c3fc
	bl 0x0200c45c
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	bl 0x0200c3dc
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl 0x0200c3dc
	movs r0, #60
	bl 0x0200c2d4
	ldr r0, [pc, #736]
	bl 0x0200c194
	movs r0, #1
	bl 0x0200c184
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl 0x0200c35c
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl 0x0200c35c
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200c3c4
	movs r0, #8
	movs r1, #4
	movs r2, #40
	bl 0x0200c374
	ldr r0, [pc, #692]
	movs r1, #0
	movs r2, #10
	bl 0x0200c3bc
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200c3c4
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x0200c3bc
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200c3c4
	ldr r0, [pc, #648]
	movs r1, #0
	movs r2, #20
	bl 0x0200c3bc
	movs r0, #9
	movs r1, #4
	movs r2, #20
	bl 0x0200c374
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl 0x0200c3bc
	movs r0, #8
	movs r1, #1
	bl 0x0200c384
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200c3c4
	movs r2, #10
	ldr r0, [pc, #596]
	movs r1, #0
	bl 0x0200c3bc
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl 0x0200c3dc
	movs r0, #80
	bl 0x0200c2d4
	movs r1, #3
	movs r0, #8
	bl 0x0200c36c
	movs r0, #20
	bl 0x0200c2d4
	movs r0, #9
	movs r1, #1
	bl 0x0200c384
	movs r0, #9
	movs r1, #3
	bl 0x0200c36c
	movs r2, #223
	movs r0, #8
	ldr r1, [pc, #544]
	lsls r2, r2, #3
	bl 0x0200c344
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200c35c
	movs r2, #217
	movs r0, #9
	ldr r1, [pc, #524]
	lsls r2, r2, #3
	bl 0x0200c344
	movs r2, #217
	movs r0, #9
	ldr r1, [pc, #516]
	lsls r2, r2, #3
	bl 0x0200c344
	movs r2, #219
	movs r0, #9
	ldr r1, [pc, #504]
	lsls r2, r2, #3
	bl 0x0200c344
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl 0x0200c35c
	movs r0, #141
	bl 0x0200c4a4
	ldr r1, [pc, #484]
	movs r0, #10
	bl 0x0200c314
	ldr r1, [pc, #480]
	movs r0, #11
	bl 0x0200c314
	movs r0, #40
	bl 0x0200c2d4
	ldr r1, [pc, #472]
	movs r0, #12
	bl 0x0200c314
	movs r0, #40
	bl 0x0200c2d4
	ldr r1, [pc, #460]
	movs r0, #13
	bl 0x0200c324
	bl 0x0200c464
	movs r2, #197
	movs r0, #0
	ldr r1, [pc, #448]
	lsls r2, r2, #19
	bl 0x0200c35c
	movs r2, #200
	lsls r2, r2, #19
	movs r0, #1
	ldr r1, [pc, #440]
	bl 0x0200c35c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl 0x0200c3ec
	movs r1, #1
	movs r2, #201
	lsls r2, r2, #19
	movs r3, #1
	ldr r0, [pc, #416]
	negs r1, r1
	bl 0x0200c3f4
	bl 0x0200c3fc
	ldr r5, [pc, #408]
	movs r0, #10
	adds r1, r5, #0
	bl 0x0200c314
	movs r0, #20
	bl 0x0200c2d4
	ldr r0, [pc, #396]
	ldr r1, [pc, #400]
	bl 0x0200c3ec
	movs r1, #1
	movs r2, #193
	movs r3, #1
	lsls r2, r2, #19
	ldr r0, [pc, #372]
	negs r1, r1
	bl 0x0200c3f4
	adds r1, r5, #0
	movs r0, #11
	bl 0x0200c314
	movs r0, #20
	bl 0x0200c2d4
	adds r1, r5, #0
	movs r0, #12
	bl 0x0200c314
	movs r0, #20
	bl 0x0200c2d4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c3c4
	movs r1, #128
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200c3c4
	adds r1, r5, #0
	movs r0, #13
	bl 0x0200c314
	movs r0, #40
	bl 0x0200c2d4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c3c4
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200c3c4
	movs r0, #13
	bl 0x0200c31c
	ldr r0, [pc, #284]
	bl 0x0200c4a4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl 0x0200c3ec
	movs r1, #1
	movs r2, #201
	movs r3, #1
	ldr r0, [pc, #264]
	negs r1, r1
	lsls r2, r2, #19
	bl 0x0200c3f4
	bl 0x0200c3fc
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c3c4
	movs r1, #160
	movs r2, #80
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200c3c4
	movs r0, #0
	movs r1, #3
	bl 0x0200c364
	movs r1, #3
	movs r0, #1
	bl 0x0200c36c
	movs r0, #20
	bl 0x0200c2d4
	movs r0, #1
	movs r1, #2
	bl 0x0200c364
	movs r0, #0
	bl 0x0200c2fc
	cmp r0, #0
	beq .L_02000c28_3
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200c32c
.L_02000c28_3:
	movs r0, #1
	bl 0x0200c354
	movs r2, #0
	movs r0, #1
	movs r1, #0
	bl 0x0200c35c
	ldr r0, [pc, #160]
	ldr r1, [pc, #84]
	bl 0x0200c3ec
	movs r1, #1
	movs r2, #201
	movs r3, #1
	ldr r0, [pc, #124]
	negs r1, r1
	lsls r2, r2, #19
	bl 0x0200c3f4
	movs r0, #0
	ldr r1, [pc, #136]
	ldr r2, [pc, #116]
	bl 0x0200c30c
	movs r2, #197
	ldr r1, [pc, #128]
	lsls r2, r2, #3
	movs r0, #0
	bl 0x0200c344
	bl 0x0200c444
	bl 0x0200c44c
	ldr r0, [pc, #116]
	bl 0x0200c294
	movs r0, #3
	bl 0x0200c40c
	b .L_02000c28_4
	.4byte 0x15e80000
	.4byte 0x15a80000
	.4byte 0x00002666
	.4byte 0x00004ccc
	.4byte 0x0200cebc
	.4byte 0x00001999
	.4byte 0x020097a5
	.4byte 0x16080000
	.4byte 0x00002008
	.4byte 0x00001618
	.4byte 0x000015f8
	.4byte 0x00001608
	.4byte 0x0200cac4
	.4byte 0x0200cc30
	.4byte 0x0200cd54
	.4byte 0x0200ce78
	.4byte 0x170c0000
	.4byte 0x17140000
	.4byte 0x16d80000
	.4byte 0x0200caf4
	.4byte 0x00006666
	.4byte 0x00000ccc
	.4byte 0x00000121
	.4byte 0x16f80000
	.4byte 0x0000cccc
	.4byte 0x000016d8
	.4byte 0x0000085a
.L_02000c28_4:
	bl 0x0200c2e4
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200155c
	.thumb_func
Func_0200155c:
	push {lr}
	movs r1, #1
	movs r2, #223
	negs r1, r1
	lsls r2, r2, #19
	movs r3, #1
	ldr r0, [pc, #240]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #228]
	movs r3, #1
	ldr r0, [pc, #228]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #216]
	movs r3, #1
	ldr r0, [pc, #200]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #188]
	movs r3, #1
	ldr r0, [pc, #180]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #176]
	movs r3, #1
	ldr r0, [pc, #168]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	movs r2, #223
	negs r1, r1
	lsls r2, r2, #19
	movs r3, #1
	ldr r0, [pc, #156]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	movs r2, #223
	negs r1, r1
	lsls r2, r2, #19
	movs r3, #1
	ldr r0, [pc, #136]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #124]
	movs r3, #1
	ldr r0, [pc, #124]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #112]
	movs r3, #1
	ldr r0, [pc, #96]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #84]
	movs r3, #1
	ldr r0, [pc, #76]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #72]
	movs r3, #1
	ldr r0, [pc, #64]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	movs r2, #223
	ldr r0, [pc, #36]
	negs r1, r1
	lsls r2, r2, #19
	movs r3, #1
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x160c0000
	.4byte 0x06fc0000
	.4byte 0x16040000
	.4byte 0x06f40000
	.4byte 0x16080000
	.4byte 0x160a0000
	.4byte 0x06fa0000
	.4byte 0x16060000
	.4byte 0x06f60000
	.global Func_02001680
	.thumb_func
Func_02001680:
	push {lr}
	movs r1, #1
	movs r2, #217
	negs r1, r1
	lsls r2, r2, #19
	movs r3, #1
	ldr r0, [pc, #240]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #228]
	movs r3, #1
	ldr r0, [pc, #228]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #216]
	movs r3, #1
	ldr r0, [pc, #200]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #188]
	movs r3, #1
	ldr r0, [pc, #180]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #176]
	movs r3, #1
	ldr r0, [pc, #168]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	movs r2, #217
	negs r1, r1
	lsls r2, r2, #19
	movs r3, #1
	ldr r0, [pc, #156]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	movs r2, #217
	negs r1, r1
	lsls r2, r2, #19
	movs r3, #1
	ldr r0, [pc, #136]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #124]
	movs r3, #1
	ldr r0, [pc, #124]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #112]
	movs r3, #1
	ldr r0, [pc, #96]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #84]
	movs r3, #1
	ldr r0, [pc, #76]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #72]
	movs r3, #1
	ldr r0, [pc, #64]
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	movs r1, #1
	movs r2, #217
	ldr r0, [pc, #36]
	negs r1, r1
	lsls r2, r2, #19
	movs r3, #1
	bl 0x0200c3f4
	movs r0, #4
	bl 0x0200c184
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x15ec0000
	.4byte 0x06cc0000
	.4byte 0x15e40000
	.4byte 0x06c40000
	.4byte 0x15e80000
	.4byte 0x15ea0000
	.4byte 0x06ca0000
	.4byte 0x15e60000
	.4byte 0x06c60000
	.global Func_020017a4
	.thumb_func
Func_020017a4:
	push {r5, lr}
	movs r0, #15
	bl 0x0200c2fc
	adds r5, r0, #0
	movs r0, #14
	bl 0x0200c2fc
	ldr r3, [r0, #8]
	str r3, [r5, #8]
	ldr r3, [r0, #16]
	movs r2, #160
	str r3, [r5, #16]
	ldr r3, [r5, #12]
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_020017a4_0
	movs r3, #160
	lsls r3, r3, #12
	movs r0, #128
	str r3, [r5, #12]
	lsls r0, r0, #2
	bl 0x0200c28c
	cmp r0, #0
	bne .L_020017a4_0
	movs r0, #145
	bl 0x0200c4a4
	adds r0, r5, #0
	movs r1, #3
	bl 0x0200c1fc
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200c294
	adds r2, r5, #0
	adds r2, #100
	movs r3, #1
	strh r3, [r2]
.L_020017a4_0:
	pop {r5}
	pop {r0}
	bx r0
	.global Func_020017fc
	.thumb_func
Func_020017fc:
	push {r5, lr}
	movs r0, #8
	bl 0x0200c2fc
	adds r5, r0, #0
	bl 0x0200c2dc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl 0x0200c3f4
	movs r0, #1
	bl 0x0200c184
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl 0x0200c35c
	movs r3, #160
	lsls r3, r3, #9
	str r3, [r5, #28]
	str r3, [r5, #24]
	movs r0, #8
	movs r1, #1
	bl 0x0200c3e4
	bl 0x0200c43c
	movs r0, #8
	ldr r1, [pc, #48]
	ldr r2, [pc, #48]
	bl 0x0200c30c
	ldr r1, [pc, #48]
	ldr r2, [pc, #48]
	movs r0, #8
	bl 0x0200c344
	bl 0x0200c444
	bl 0x0200c44c
	ldr r0, [pc, #36]
	bl 0x0200c294
	movs r0, #102
	bl 0x0200c40c
	bl 0x0200c2e4
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00006666
	.4byte 0x00003333
	.4byte 0x000014a8
	.4byte 0x00000918
	.4byte 0x00000927
	.global Func_02001888
	.thumb_func
Func_02001888:
	push {r5, lr}
	movs r0, #8
	bl 0x0200c2fc
	adds r5, r0, #0
	bl 0x0200c2dc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl 0x0200c3f4
	movs r0, #1
	bl 0x0200c184
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200c35c
	movs r2, #200
	lsls r2, r2, #16
	ldr r1, [pc, #100]
	movs r0, #8
	bl 0x0200c35c
	movs r3, #160
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r0, #1
	bl 0x0200c184
	movs r0, #8
	movs r1, #1
	bl 0x0200c3e4
	bl 0x0200c43c
	movs r0, #8
	ldr r1, [pc, #68]
	ldr r2, [pc, #72]
	bl 0x0200c30c
	adds r5, #100
	movs r3, #0
	strh r3, [r5]
	ldr r1, [pc, #64]
	movs r0, #8
	bl 0x0200c314
.L_02001888_0:
	movs r0, #1
	bl 0x0200c184
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	beq .L_02001888_0
	bl 0x0200c444
	bl 0x0200c44c
	ldr r0, [pc, #36]
	bl 0x0200c294
	movs r0, #103
	bl 0x0200c40c
	bl 0x0200c2e4
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x1f080000
	.4byte 0x00009999
	.4byte 0x00004ccc
	.4byte 0x0200d158
	.4byte 0x00000927
	.global Func_02001938
	.thumb_func
Func_02001938:
	push {r5, lr}
	movs r0, #8
	bl 0x0200c2fc
	adds r5, r0, #0
	bl 0x0200c2dc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl 0x0200c3f4
	movs r0, #1
	bl 0x0200c184
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200c35c
	movs r2, #200
	lsls r2, r2, #16
	ldr r1, [pc, #100]
	movs r0, #8
	bl 0x0200c35c
	movs r3, #160
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r0, #1
	bl 0x0200c184
	movs r0, #8
	movs r1, #1
	bl 0x0200c3e4
	bl 0x0200c43c
	movs r0, #8
	ldr r1, [pc, #68]
	ldr r2, [pc, #72]
	bl 0x0200c30c
	adds r5, #100
	movs r3, #0
	strh r3, [r5]
	ldr r1, [pc, #64]
	movs r0, #8
	bl 0x0200c314
.L_02001938_0:
	movs r0, #1
	bl 0x0200c184
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	beq .L_02001938_0
	bl 0x0200c444
	bl 0x0200c44c
	ldr r0, [pc, #36]
	bl 0x0200c294
	movs r0, #104
	bl 0x0200c40c
	bl 0x0200c2e4
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x1f080000
	.4byte 0x00009999
	.4byte 0x00004ccc
	.4byte 0x0200d158
	.4byte 0x00000927
	.global Func_020019e8
	.thumb_func
Func_020019e8:
	push {r5, lr}
	movs r0, #8
	bl 0x0200c2fc
	adds r5, r0, #0
	bl 0x0200c2dc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl 0x0200c3f4
	movs r0, #1
	bl 0x0200c184
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200c35c
	movs r2, #200
	lsls r2, r2, #16
	ldr r1, [pc, #100]
	movs r0, #8
	bl 0x0200c35c
	movs r3, #160
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r0, #1
	bl 0x0200c184
	movs r0, #8
	movs r1, #1
	bl 0x0200c3e4
	bl 0x0200c43c
	movs r0, #8
	ldr r1, [pc, #68]
	ldr r2, [pc, #72]
	bl 0x0200c30c
	adds r5, #100
	movs r3, #0
	strh r3, [r5]
	ldr r1, [pc, #64]
	movs r0, #8
	bl 0x0200c314
.L_020019e8_0:
	movs r0, #1
	bl 0x0200c184
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	beq .L_020019e8_0
	bl 0x0200c444
	bl 0x0200c44c
	ldr r0, [pc, #36]
	bl 0x0200c294
	movs r0, #105
	bl 0x0200c40c
	bl 0x0200c2e4
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x1f080000
	.4byte 0x00009999
	.4byte 0x00004ccc
	.4byte 0x0200d158
	.4byte 0x00000927
	.global Func_02001a98
	.thumb_func
Func_02001a98:
	push {r5, lr}
	movs r0, #8
	bl 0x0200c2fc
	adds r5, r0, #0
	bl 0x0200c2dc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl 0x0200c3f4
	movs r0, #1
	bl 0x0200c184
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200c35c
	movs r2, #200
	lsls r2, r2, #16
	ldr r1, [pc, #116]
	movs r0, #8
	bl 0x0200c35c
	movs r3, #160
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r0, #1
	bl 0x0200c184
	movs r0, #8
	movs r1, #1
	bl 0x0200c3e4
	bl 0x0200c43c
	movs r0, #8
	ldr r1, [pc, #84]
	ldr r2, [pc, #88]
	bl 0x0200c30c
	adds r5, #100
	movs r3, #0
	strh r3, [r5]
	bl 0x0200b41c
	cmp r0, #11
	bne .L_02001a98_0
	ldr r1, [pc, #72]
	movs r0, #8
	bl 0x0200c314
	b .L_02001a98_1
.L_02001a98_0:
	ldr r1, [pc, #64]
	movs r0, #8
	bl 0x0200c314
.L_02001a98_1:
	movs r0, #1
	bl 0x0200c184
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	beq .L_02001a98_1
	bl 0x0200c444
	bl 0x0200c44c
	ldr r0, [pc, #40]
	bl 0x0200c294
	movs r0, #106
	bl 0x0200c40c
	bl 0x0200c2e4
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x1f080000
	.4byte 0x00009999
	.4byte 0x00004ccc
	.4byte 0x0200d1b8
	.4byte 0x0200d158
	.4byte 0x00000927
	.global Func_02001b5c
	.thumb_func
Func_02001b5c:
	push {r5, lr}
	movs r0, #8
	bl 0x0200c2fc
	adds r5, r0, #0
	bl 0x0200c2dc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl 0x0200c3f4
	movs r0, #1
	bl 0x0200c184
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200c35c
	ldr r2, [pc, #88]
	ldr r1, [pc, #92]
	movs r0, #8
	bl 0x0200c35c
	movs r3, #160
	lsls r3, r3, #9
	str r3, [r5, #28]
	str r3, [r5, #24]
	movs r0, #1
	bl 0x0200c184
	movs r0, #8
	movs r1, #1
	bl 0x0200c3e4
	bl 0x0200c43c
	movs r0, #8
	ldr r1, [pc, #60]
	ldr r2, [pc, #60]
	bl 0x0200c30c
	ldr r1, [pc, #60]
	ldr r2, [pc, #60]
	movs r0, #8
	bl 0x0200c344
	bl 0x0200c444
	bl 0x0200c44c
	ldr r0, [pc, #48]
	bl 0x0200c294
	ldr r0, [pc, #48]
	bl 0x0200c29c
	movs r0, #107
	bl 0x0200c40c
	bl 0x0200c2e4
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x09180000
	.4byte 0x13e80000
	.4byte 0x00006666
	.4byte 0x00003333
	.4byte 0x000013c8
	.4byte 0x00000918
	.4byte 0x0000093e
	.4byte 0x00000927
	.global Func_02001c08
	.thumb_func
Func_02001c08:
	push {lr}
	bl 0x0200c464
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #6
	bl 0x0200c48c
	bl 0x0200c494
	bl 0x0200c45c
	movs r1, #2
	movs r0, #8
	bl 0x0200c384
	ldr r0, [pc, #104]
	bl 0x0200c3a4
	movs r1, #0
	movs r0, #8
	bl 0x0200c3b4
	movs r0, #30
	bl 0x0200c2d4
	movs r0, #111
	bl 0x0200c4a4
	movs r1, #2
	movs r0, #0
	bl 0x0200c284
	ldr r0, [pc, #76]
	bl 0x0200c29c
	ldr r0, [pc, #72]
	bl 0x0200c29c
	bl 0x0200c49c
	movs r2, #30
	movs r1, #4
	movs r0, #8
	bl 0x0200c374
	ldr r0, [pc, #56]
	bl 0x0200c3a4
	movs r1, #0
	movs r0, #8
	bl 0x0200c3b4
	ldr r0, [pc, #36]
	bl 0x0200c29c
	ldr r0, [pc, #32]
	bl 0x0200c294
	bl 0x0200c49c
	movs r0, #30
	bl 0x0200c2d4
	movs r0, #12
	movs r1, #6
	bl 0x0200c414
	pop {r0}
	bx r0
	.4byte 0x00000c66
	.4byte 0x0000016f
	.4byte 0x00000171
	.4byte 0x00000c67
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #8
	bl 0x0200c2fc
	adds	r6, r0, #0
	movs	r0, #0
	bl 0x0200c2fc
	ldr	r1, [pc, #360]
	ldr	r3, [r0, #8]
	adds	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	ldr	r2, [pc, #356]
	asrs	r3, r3, #1
	mov	fp, r2
	add	r3, fp
	mov	r9, r3
	ldr	r1, [pc, #348]
	ldr	r3, [r0, #16]
	adds	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	movs	r7, #166
	asrs	r3, r3, #1
	lsls	r7, r7, #19
	movs	r0, #183
	adds	r3, r3, r7
	lsls	r0, r0, #1
	movs	r5, #0
	mov	sl, r3
	bl 0x0200c28c
	cmp	r0, #0
	beq.n	.L_02001cf8
	b.n	.L_02002080
.L_02001cf8:
	movs	r0, #1
	bl 0x0200c2c4
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200c294
	bl 0x0200c2dc
	movs	r0, #0
	bl 0x0200c2fc
	cmp	r0, #0
	beq.n	.L_02001d1e
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #16]
	movs	r0, #8
	bl 0x0200c35c
.L_02001d1e:
	movs	r1, #0
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c2b4
	movs	r1, #0
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c2bc
	bl 0x0200c45c
	movs	r1, #8
	movs	r2, #0
	movs	r0, #0
	bl 0x0200c38c
	movs	r0, #10
	bl 0x0200c2d4
	movs	r0, #0
	ldr	r1, [pc, #236]
	movs	r2, #60
	bl 0x0200c3d4
	adds	r2, r6, #0
	movs	r3, #1
	adds	r2, #102
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #0
	movs	r0, #8
	bl 0x0200c38c
	movs	r0, #16
	bl 0x0200c184
	ldr	r0, [pc, #208]
	bl 0x0200c3a4
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c3b4
	bl 0x0200c464
	ldr	r0, [pc, #196]
	movs	r1, #6
	bl 0x0200c48c
	bl 0x0200c494
	bl 0x0200c45c
	movs	r2, #85
	adds	r2, r2, r6
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r6, #72]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #48]
	str	r3, [r6, #52]
	str	r5, [r6, #40]
	str	r5, [r6, #20]
	adds	r3, r7, #0
	mov	r8, r2
	adds	r0, r6, #0
	mov	r1, fp
	movs	r2, #0
	bl 0x0200c224
	movs	r7, #128
	lsls	r7, r7, #4
	movs	r5, #15
.L_02001db8:
	ldr	r3, [r6, #24]
	adds	r3, r3, r7
	str	r3, [r6, #24]
	ldr	r3, [r6, #28]
	adds	r3, r3, r7
	str	r3, [r6, #28]
	movs	r0, #1
	subs	r5, #1
	bl 0x0200c184
	cmp	r5, #0
	bge.n	.L_02001db8
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c38c
	movs	r2, #0
	movs	r1, #8
	movs	r0, #0
	bl 0x0200c38c
	movs	r0, #16
	bl 0x0200c184
	movs	r3, #0
	str	r3, [r6, #108]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200c25c
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #72]
	movs	r1, #0
	movs	r0, #8
	bl 0x0200c3b4
	movs	r0, #131
	bl 0x0200c4a4
	movs	r0, #140
	movs	r1, #0
	bl 0x0200c474
	ldr	r7, [pc, #48]
	movs	r5, #59
.L_02001e16:
	ldr	r3, [r7, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001e48
	adds	r0, r6, #0
	movs	r1, #7
	bl 0x0200c25c
	b.n	.L_02001e50
	.2byte 0x0000
	.4byte 0xea300000
	.4byte 0x15d00000
	.4byte 0xfad00000
	.4byte 0x00000101
	.4byte 0x00000c4f
	.4byte 0x00013333
	.2byte 0x1e40
	.2byte 0x0300
.L_02001e48:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200c25c
.L_02001e50:
	ldr	r3, [r7, #0]
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02001e60
	adds	r0, r6, #0
	bl 0x0200c058
.L_02001e60:
	movs	r0, #1
	subs	r5, #1
	bl 0x0200c184
	cmp	r5, #0
	bge.n	.L_02001e16
	bl 0x0200c47c
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200c25c
	movs	r0, #8
	movs	r1, #2
	bl 0x0200c384
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c3b4
	movs	r1, #129
	movs	r2, #30
	movs	r0, #0
	lsls	r1, r1, #1
	bl 0x0200c3d4
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c3b4
	movs	r0, #0
	ldr	r1, [pc, #208]
	movs	r2, #30
	bl 0x0200c3d4
	mov	r3, r9
	asrs	r1, r3, #16
	mov	r3, sl
	asrs	r2, r3, #16
	movs	r0, #8
	bl 0x0200c344
	movs	r0, #0
	movs	r1, #22
	bl 0x0200c364
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c3b4
	movs	r0, #0
	ldr	r1, [pc, #168]
	movs	r2, #40
	bl 0x0200c3d4
	movs	r2, #30
	movs	r0, #8
	movs	r1, #4
	bl 0x0200c374
	movs	r0, #150
	lsls	r0, r0, #1
	movs	r1, #4
	bl 0x0200c26c
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c3b4
	movs	r1, #128
	movs	r2, #30
	movs	r0, #0
	lsls	r1, r1, #1
	bl 0x0200c3d4
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c3b4
	movs	r0, #0
	movs	r1, #2
	bl 0x0200c384
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c3b4
	movs	r2, #30
	movs	r0, #8
	movs	r1, #2
	bl 0x0200c374
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c3b4
	movs	r5, #0
	mov	r1, r8
	movs	r2, #128
	strb	r5, [r1, #0]
	adds	r0, r6, #0
	mov	r1, r9
	lsls	r2, r2, #13
	mov	r3, sl
	bl 0x0200c224
	ldr	r7, [pc, #52]
	movs	r5, #15
.L_02001f38:
	ldrh	r3, [r6, #6]
	adds	r3, r3, r7
	strh	r3, [r6, #6]
	movs	r0, #1
	subs	r5, #1
	bl 0x0200c184
	cmp	r5, #0
	bge.n	.L_02001f38
	movs	r0, #0
	movs	r1, #1
	bl 0x0200c364
	movs	r1, #0
	movs	r0, #8
	bl 0x0200c3b4
	movs	r2, #0
	movs	r3, #2
	mov	r1, r8
	strb	r3, [r1, #0]
	ldr	r7, [pc, #8]
	str	r2, [r6, #40]
	str	r2, [r6, #20]
	b.n	.L_02001f74
	.2byte 0x0000
	.4byte 0x00001000
	.2byte 0x0101
	.2byte 0x0000
.L_02001f74:
	movs	r5, #7
.L_02001f76:
	ldrh	r3, [r6, #6]
	adds	r3, r3, r7
	strh	r3, [r6, #6]
	movs	r0, #1
	subs	r5, #1
	bl 0x0200c184
	cmp	r5, #0
	bge.n	.L_02001f76
	movs	r0, #0
	movs	r1, #22
	bl 0x0200c364
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c3b4
	movs	r1, #129
	movs	r0, #8
	lsls	r1, r1, #1
	movs	r2, #30
	bl 0x0200c3d4
	movs	r2, #0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c38c
	movs	r0, #8
	movs	r1, #2
	bl 0x0200c384
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c3b4
	movs	r0, #8
	movs	r1, #2
	movs	r2, #30
	bl 0x0200c374
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c3ac
	movs	r5, #0
.L_02001fd2:
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c2f4
	cmp	r0, #1
	bne.n	.L_0200201a
	movs	r0, #8
	movs	r1, #2
	movs	r2, #20
	bl 0x0200c374
	movs	r0, #8
	movs	r1, #2
	movs	r2, #20
	bl 0x0200c374
	cmp	r5, #6
	bne.n	.L_02002006
	ldr	r0, [pc, #596]
	bl 0x0200c3a4
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c3b4
	b.n	.L_02002044
.L_02002006:
	ldr	r0, [pc, #584]
	adds	r0, r5, r0
	bl 0x0200c3a4
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c3ac
	adds	r5, #1
	b.n	.L_02001fd2
.L_0200201a:
	movs	r0, #0
	movs	r1, #22
	bl 0x0200c364
	movs	r0, #8
	movs	r1, #2
	movs	r2, #20
	bl 0x0200c374
	movs	r1, #4
	movs	r0, #8
	movs	r2, #20
	bl 0x0200c374
	ldr	r0, [pc, #540]
	bl 0x0200c3a4
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c3b4
.L_02002044:
	movs	r0, #150
	movs	r1, #4
	lsls	r0, r0, #1
	bl 0x0200c26c
	movs	r0, #81
	bl 0x0200c4a4
	ldr	r5, [pc, #512]
	movs	r1, #3
	adds	r0, r5, #0
	adds	r5, #1
	bl 0x0200c264
	adds	r0, r5, #0
	bl 0x0200c3a4
	movs	r2, #20
	movs	r0, #8
	movs	r1, #2
	bl 0x0200c374
	movs	r1, #0
	movs	r0, #8
	bl 0x0200c3b4
	movs	r0, #9
	bl 0x0200c4a4
	b.n	.L_020021a6
.L_02002080:
	bl 0x0200c2dc
	movs	r0, #0
	bl 0x0200c2fc
	cmp	r0, #0
	beq.n	.L_02002098
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #16]
	movs	r0, #8
	bl 0x0200c35c
.L_02002098:
	movs	r3, #160
	lsls	r3, r3, #12
	mov	r1, r9
	movs	r2, #0
	str	r3, [r6, #40]
	adds	r0, r6, #0
	mov	r3, sl
	bl 0x0200c224
	movs	r0, #30
	bl 0x0200c2d4
	bl 0x0200c45c
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c38c
	movs	r2, #0
	movs	r0, #0
	movs	r1, #8
	bl 0x0200c38c
	movs	r1, #22
	movs	r0, #0
	bl 0x0200c364
	ldr	r0, [pc, #392]
	bl 0x0200c3a4
	movs	r0, #8
	movs	r1, #2
	movs	r2, #20
	bl 0x0200c374
	movs	r2, #20
	movs	r0, #8
	movs	r1, #2
	bl 0x0200c374
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c3b4
	movs	r0, #8
	movs	r1, #2
	bl 0x0200c384
	movs	r1, #0
	movs	r0, #8
	bl 0x0200c3b4
	movs	r0, #111
	bl 0x0200c4a4
	movs	r1, #2
	movs	r0, #0
	bl 0x0200c284
	ldr	r0, [pc, #332]
	bl 0x0200c294
	ldr	r0, [pc, #332]
	bl 0x0200c29c
	bl 0x0200c49c
	ldr	r0, [pc, #324]
	bl 0x0200c3a4
	adds	r3, r7, #0
	movs	r2, #0
	mov	r1, fp
	adds	r0, r6, #0
	bl 0x0200c224
	movs	r0, #30
	bl 0x0200c2d4
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c3b4
	movs	r2, #0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c38c
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c3b4
	movs	r1, #0
	movs	r0, #8
	bl 0x0200c3ac
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c2f4
	cmp	r0, #1
	bne.n	.L_020021b0
	movs	r0, #0
	movs	r1, #22
	bl 0x0200c364
	movs	r1, #2
	movs	r0, #8
	bl 0x0200c384
	ldr	r0, [pc, #244]
	bl 0x0200c3a4
	movs	r1, #0
	movs	r0, #8
	bl 0x0200c3ac
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c2f4
	cmp	r0, #1
	beq.n	.L_020021b0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c3b4
	mov	r3, sl
	mov	r2, r9
	asrs	r1, r2, #16
	movs	r0, #8
	asrs	r2, r3, #16
	bl 0x0200c344
.L_020021a6:
	bl 0x02009c08
	bl 0x0200c464
	b.n	.L_0200223c
.L_020021b0:
	movs	r1, #22
	movs	r0, #0
	bl 0x0200c364
	ldr	r0, [pc, #180]
	bl 0x0200c3a4
	movs	r0, #8
	movs	r1, #2
	movs	r2, #20
	bl 0x0200c374
	movs	r2, #20
	movs	r0, #8
	movs	r1, #2
	bl 0x0200c374
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c36c
	movs	r1, #128
	movs	r2, #30
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x0200c3d4
	movs	r1, #0
	movs	r0, #8
	bl 0x0200c3b4
	ldr	r0, [pc, #112]
	bl 0x0200c294
	ldr	r0, [pc, #108]
	bl 0x0200c294
	bl 0x0200c49c
	movs	r2, #20
	movs	r0, #8
	movs	r1, #2
	bl 0x0200c374
	movs	r0, #8
	movs	r1, #0
	bl 0x0200c3b4
	bl 0x0200c464
	movs	r1, #0
	movs	r2, #0
	movs	r0, #8
	bl 0x0200c484
	movs	r0, #42
	bl 0x0200c4a4
	bl 0x0200c2e4
	movs	r0, #183
	lsls	r0, r0, #1
	bl 0x0200c29c
	ldr	r0, [pc, #44]
	bl 0x0200c29c
	ldr	r0, [pc, #44]
	bl 0x0200c29c
.L_0200223c:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00000c62
	.4byte 0x00000c5c
	.4byte 0x00000c63
	.4byte 0x00000c64
	.4byte 0x00000c68
	.4byte 0x0000016f
	.4byte 0x00000171
	.4byte 0x00000c6a
	.4byte 0x00000c6d
	.2byte 0x0c6f
	.2byte 0x0000
	.global Func_02002274
	.thumb_func
Func_02002274:
	push {r5, lr}
	movs r0, #10
	bl 0x0200c2fc
	adds r5, r0, #0
	bl 0x0200c2dc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl 0x0200c3f4
	movs r0, #1
	bl 0x0200c184
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl 0x0200c35c
	movs r0, #1
	bl 0x0200c184
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	bl 0x0200c43c
	bl 0x0200c44c
	movs r0, #20
	bl 0x0200c2d4
	ldr r1, [pc, #372]
	ldr r2, [pc, #372]
	movs r0, #10
	bl 0x0200c35c
	movs r0, #1
	bl 0x0200c184
	movs r0, #141
	bl 0x0200c4a4
	ldr r2, [pc, #356]
	movs r0, #10
	ldr r1, [pc, #356]
	bl 0x0200c30c
	movs r0, #10
	movs r1, #2
	bl 0x0200c364
	ldr r2, [pc, #348]
	movs r0, #10
	ldr r1, [pc, #348]
	bl 0x0200c334
	ldr r0, [pc, #328]
	ldr r1, [pc, #344]
	bl 0x0200c3ec
	movs r1, #1
	movs r3, #1
	ldr r0, [pc, #336]
	negs r1, r1
	ldr r2, [pc, #336]
	bl 0x0200c3f4
	movs r0, #10
	ldr r1, [pc, #332]
	ldr r2, [pc, #312]
	bl 0x0200c334
	movs r0, #10
	ldr r1, [pc, #328]
	ldr r2, [pc, #328]
	bl 0x0200c334
	ldr r2, [pc, #328]
	movs r0, #10
	ldr r1, [pc, #328]
	bl 0x0200c334
	movs r1, #1
	movs r0, #10
	bl 0x0200c364
	ldr r0, [pc, #316]
	bl 0x0200c4a4
	movs r0, #20
	bl 0x0200c2d4
	ldr r1, [pc, #308]
	ldr r2, [pc, #312]
	movs r0, #0
	bl 0x0200c35c
	movs r0, #1
	bl 0x0200c184
	movs r0, #0
	movs r1, #6
	movs r2, #0
	bl 0x0200c374
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200c30c
	movs r0, #0
	ldr r1, [pc, #276]
	ldr r2, [pc, #252]
	bl 0x0200c344
	movs r2, #40
	movs r1, #0
	movs r0, #0
	bl 0x0200c3c4
	movs r0, #141
	bl 0x0200c4a4
	movs r0, #10
	movs r1, #2
	bl 0x0200c364
	movs r0, #10
	ldr r1, [pc, #244]
	ldr r2, [pc, #220]
	bl 0x0200c334
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c3c4
	movs r0, #10
	ldr r1, [pc, #224]
	ldr r2, [pc, #224]
	bl 0x0200c334
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c3c4
	movs r0, #10
	ldr r1, [pc, #208]
	ldr r2, [pc, #204]
	bl 0x0200c334
	movs r0, #10
	ldr r1, [pc, #204]
	ldr r2, [pc, #204]
	bl 0x0200c334
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c3c4
	movs r0, #10
	ldr r1, [pc, #188]
	ldr r2, [pc, #112]
	bl 0x0200c334
	movs r0, #10
	ldr r1, [pc, #184]
	ldr r2, [pc, #160]
	bl 0x0200c334
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl 0x0200c35c
	ldr r0, [pc, #124]
	bl 0x0200c4a4
	movs r0, #40
	bl 0x0200c2d4
	movs r1, #1
	movs r3, #1
	ldr r0, [pc, #112]
	negs r1, r1
	ldr r2, [pc, #80]
	bl 0x0200c3f4
	movs r0, #0
	ldr r1, [pc, #140]
	ldr r2, [pc, #48]
	bl 0x0200c30c
	ldr r1, [pc, #136]
	ldr r2, [pc, #48]
	movs r0, #0
	bl 0x0200c344
	bl 0x0200c444
	bl 0x0200c44c
	movs r0, #20
	bl 0x0200c40c
	bl 0x0200c2e4
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x15680000
	.4byte 0x08380000
	.4byte 0x00006666
	.4byte 0x00019999
	.4byte 0x00000858
	.4byte 0x0000156d
	.4byte 0x00000ccc
	.4byte 0x15b80000
	.4byte 0x08580000
	.4byte 0x0000159e
	.4byte 0x000015a8
	.4byte 0x0000086e
	.4byte 0x00000878
	.4byte 0x000015e8
	.4byte 0x00000121
	.4byte 0x15d80000
	.4byte 0x08780000
	.4byte 0x000015c8
	.4byte 0x000015f8
	.4byte 0x00000838
	.4byte 0x000015bd
	.4byte 0x000015b8
	.4byte 0x00000853
	.4byte 0x00001572
	.4byte 0x00001568
	.4byte 0x0000cccc
	.4byte 0x000015d8
	.global Func_020024a8
	.thumb_func
Func_020024a8:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	bl 0x0200c2dc
	ldr r6, [pc, #620]
	movs r3, #55
	str r3, [r6]
	movs r0, #55
	bl 0x0200a768
	bl 0x0200c43c
	bl 0x0200c44c
	bl 0x0200c45c
	movs r0, #0
	bl 0x0200c2fc
	adds r3, r0, #0
	cmp r3, #0
	beq .L_020024a8_0
	ldr r0, [r6]
	ldr r1, [r3, #8]
	ldr r2, [r3, #16]
	bl 0x0200c35c
.L_020024a8_0:
	ldr r0, [r6]
	ldr r1, [pc, #576]
	ldr r2, [pc, #576]
	bl 0x0200c30c
	ldr r0, [r6]
	ldr r1, [pc, #572]
	ldr r2, [pc, #576]
	bl 0x0200c344
	ldr r0, [r6]
	movs r1, #0
	movs r2, #20
	bl 0x0200c3c4
	movs r1, #128
	movs r2, #60
	ldr r0, [r6]
	lsls r1, r1, #1
	bl 0x0200c3d4
	ldr r0, [r6]
	movs r1, #2
	bl 0x0200c37c
	ldr r0, [pc, #544]
	bl 0x0200c3a4
	ldr r0, [r6]
	movs r3, #128
	lsls r3, r3, #5
	orrs r0, r3
	movs r1, #0
	movs r2, #10
	bl 0x0200c3bc
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl 0x0200c3c4
	bl 0x0200c464
	ldr r0, [pc, #512]
	movs r1, #10
	bl 0x0200c48c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #12
	lsls r1, r1, #9
	bl 0x0200c3ec
	movs r1, #1
	movs r3, #1
	ldr r2, [pc, #492]
	negs r1, r1
	ldr r0, [pc, #492]
	bl 0x0200c3f4
	bl 0x0200c3fc
	movs r0, #40
	bl 0x0200c2d4
	bl 0x0200c45c
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl 0x0200c3dc
	movs r0, #60
	bl 0x0200c2d4
	ldr r0, [r6]
	movs r1, #1
	bl 0x0200c384
	movs r5, #192
	ldr r0, [r6]
	lsls r5, r5, #6
	orrs r0, r5
	movs r1, #0
	movs r2, #40
	bl 0x0200c3bc
	ldr r0, [r6]
	ldr r1, [pc, #408]
	ldr r2, [pc, #432]
	bl 0x0200c344
	ldr r0, [r6]
	ldr r1, [pc, #428]
	ldr r2, [pc, #420]
	bl 0x0200c344
	ldr r0, [r6]
	adds r1, r5, #0
	movs r2, #20
	bl 0x0200c3c4
	ldr r2, [pc, #412]
	movs r0, #0
	ldr r1, [pc, #372]
	bl 0x0200c30c
	ldr r1, [pc, #408]
	movs r0, #0
	bl 0x0200c314
	movs r0, #20
	bl 0x0200c2d4
	ldr r0, [r6]
	movs r1, #1
	bl 0x0200c384
	ldr r0, [r6]
	movs r1, #0
	movs r2, #10
	bl 0x0200c3bc
	b .L_020024a8_1
.L_020024a8_2:
	movs r0, #1
	bl 0x0200c184
.L_020024a8_1:
	movs r0, #0
	bl 0x0200c2fc
	adds r0, #100
	movs r2, #0
	ldrsh r3, [r0, r2]
	cmp r3, #0
	beq .L_020024a8_2
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200c3c4
	ldr r6, [pc, #292]
	movs r1, #131
	ldr r0, [r6]
	movs r2, #0
	lsls r1, r1, #1
	bl 0x0200c3d4
	ldr r0, [r6]
	movs r1, #2
	bl 0x0200c384
	ldr r0, [r6]
	movs r1, #0
	movs r2, #10
	bl 0x0200c3bc
	movs r1, #128
	ldr r0, [r6]
	movs r2, #10
	lsls r1, r1, #8
	bl 0x0200c3c4
	ldr r0, [r6]
	movs r1, #0
	bl 0x0200c3b4
	movs r1, #0
	movs r2, #20
	movs r0, #0
	bl 0x0200c3c4
	ldr r0, [r6]
	bl 0x0200c2fc
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	ldr r1, [pc, #264]
	ldr r2, [pc, #244]
	ldr r0, [r6]
	bl 0x0200c344
	movs r0, #1
	bl 0x0200c2d4
	ldr r0, [r6]
	bl 0x0200c2fc
	movs r2, #1
	adds r0, #90
	ldrb r3, [r0]
	mov r8, r2
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r0, #20
	bl 0x0200c2d4
	ldr r0, [r6]
	bl 0x0200c2fc
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	ldr r1, [pc, #196]
	ldr r2, [pc, #188]
	ldr r0, [r6]
	bl 0x0200c344
	movs r0, #1
	bl 0x0200c2d4
	ldr r0, [r6]
	bl 0x0200c2fc
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r2, r3
	strb r2, [r0]
	movs r1, #3
	movs r0, #242
	mov r8, r2
	bl 0x0200c454
	movs r1, #0
	movs r0, #242
	bl 0x0200c2ec
	ldr r0, [r6]
	movs r1, #1
	bl 0x0200c384
	ldr r0, [pc, #152]
	bl 0x0200c3a4
	ldr r0, [r6]
	movs r1, #0
	movs r2, #10
	bl 0x0200c3bc
	movs r1, #192
	movs r2, #0
	ldr r0, [r6]
	lsls r1, r1, #6
	bl 0x0200c3c4
	bl 0x0200c464
	movs r0, #128
	movs r1, #10
	lsls r0, r0, #9
	bl 0x0200c48c
	movs r0, #20
	bl 0x0200c2d4
	movs r0, #141
	lsls r0, r0, #2
	bl 0x0200c294
	ldr r0, [pc, #100]
	bl 0x0200c294
	ldr r1, [pc, #100]
	movs r0, #226
	ldr r2, [pc, #100]
	lsls r0, r0, #1
	adds r3, r1, r0
	strh r2, [r3]
	movs r3, #227
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #78
	strh r3, [r2]
	bl 0x0200c2e4
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200e79c
	.4byte 0x00019999
	.4byte 0x0000cccc
	.4byte 0x00001768
	.4byte 0x00000d78
	.4byte 0x0000263c
	.4byte 0x00016666
	.4byte 0x0d680000
	.4byte 0x17880000
	.4byte 0x00000d48
	.4byte 0x00001794
	.4byte 0x00006666
	.4byte 0x0200cf20
	.4byte 0x0000178c
	.4byte 0x00002642
	.4byte 0x000009bf
	.4byte 0x02000240
	.4byte 0x00000002
	.global Func_02002768
	.thumb_func
Func_02002768:
	push {r5, r6, r7, lr}
	ldr r4, [pc, #92]
	movs r3, #1
	negs r3, r3
	ldr r6, [pc, #88]
	adds r0, r4, #0
	movs r7, #2
	mov r12, r4
	movs r1, #4
	movs r2, #0
	mov lr, r3
.L_02002768_2:
	ldr r3, [r2, r4]
	cmp r3, #1
	bne .L_02002768_0
	ldrsh r3, [r0, r1]
	cmp r3, #138
	bne .L_02002768_0
	adds r3, r1, #4
	str r7, [r0, r2]
	str r6, [r0, r3]
	mov r0, r12
.L_02002768_0:
	ldr r3, [r2, r4]
	cmp r3, lr
	beq .L_02002768_1
	adds r1, #12
	adds r2, #12
	b .L_02002768_2
.L_02002768_1:
	ldr r0, [pc, #48]
	movs r6, #192
	adds r2, r0, #0
	ldr r4, [pc, #44]
	ldr r5, [pc, #48]
	lsls r6, r6, #6
	adds r2, #8
	movs r1, #0
.L_02002768_4:
	ldrsh r3, [r1, r0]
	cmp r3, #57
	bne .L_02002768_3
	str r4, [r2]
	str r5, [r2, #8]
	strh r6, [r2, #12]
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_02002768_3:
	adds r2, #24
	adds r1, #24
	b .L_02002768_4
	.2byte 0x0000
	.4byte 0x0200e3f4
	.4byte 0x0200a859
	.4byte 0x0200db84
	.4byte 0x17940000
	.4byte 0x0d480000
	.global Func_020027dc
	.thumb_func
Func_020027dc:
	push {r5, r6, r7, lr}
	ldr r0, [pc, #56]
	movs r3, #33
	movs r4, #1
	adds r7, r0, #0
	movs r6, #1
	adds r1, r0, #4
	movs r2, #0
	mov r12, r3
	negs r4, r4
.L_020027dc_2:
	ldr r3, [r2, r0]
	cmp r3, #2
	bne .L_020027dc_0
	movs r5, #0
	ldrsh r3, [r1, r5]
	cmp r3, #138
	bne .L_020027dc_0
	mov r3, r12
	str r6, [r2, r7]
	str r3, [r1, #4]
	b .L_020027dc_1
.L_020027dc_0:
	ldr r3, [r2, r0]
	cmp r3, r4
	beq .L_020027dc_1
	adds r1, #12
	adds r2, #12
	b .L_020027dc_2
.L_020027dc_1:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200e3f4
	.global Func_0200281c
	.thumb_func
Func_0200281c:
	push {lr}
	bl 0x0200c2dc
	movs r2, #0
	movs r1, #0
	movs r0, #55
	bl 0x0200c38c
	ldr r0, [pc, #32]
	bl 0x0200c3a4
	ldr r3, [pc, #32]
	movs r1, #0
	ldr r0, [r3]
	bl 0x0200c3b4
	movs r1, #192
	movs r0, #55
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c3c4
	bl 0x0200c2e4
	pop {r0}
.L_0200284e:
	bx r0
	.2byte 0x2642
	.2byte 0x0000
	.2byte 0xe79c
	.2byte 0x0200
	.global Func_02002858
	.thumb_func
Func_02002858:
	push {lr}
	bl 0x0200c2dc
	bl 0x0200c45c
	ldr r0, [pc, #52]
	bl 0x0200c3a4
	ldr r3, [pc, #48]
	movs r1, #0
	ldr r0, [r3]
	bl 0x0200c3b4
	bl 0x0200c464
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c30c
	movs r0, #0
	ldr r1, [pc, #24]
	ldr r2, [pc, #24]
	bl 0x0200c344
	bl 0x0200c2e4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00002643
	.4byte 0x0200e79c
	.4byte 0x00001778
	.4byte 0x00000d48
	.global Func_020028a8
	.thumb_func
Func_020028a8:
	push {lr}
	bl 0x0200c2dc
	bl 0x0200c45c
	ldr r0, [pc, #44]
	movs r1, #1
	bl 0x0200c264
	movs r0, #141
	lsls r0, r0, #2
	bl 0x0200c28c
	cmp r0, #0
	beq .L_020028a8_0
	ldr r3, [pc, #28]
	movs r1, #185
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
.L_020028a8_0:
	bl 0x0200c464
	bl 0x0200c2e4
	pop {r0}
	bx r0
	.4byte 0x0000264c
	.4byte 0x03001ebc
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #0
	bl 0x0200c2fc
	adds	r5, r0, #0
	bl 0x0200c2dc
	bl 0x0200c464
	ldr	r0, [pc, #872]
	movs	r1, #6
	bl 0x0200c48c
	movs	r0, #192
	movs	r1, #192
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x0200c3ec
	movs	r1, #1
	movs	r3, #1
	ldr	r0, [pc, #852]
	negs	r1, r1
	ldr	r2, [pc, #852]
	bl 0x0200c3f4
	ldr	r2, [pc, #852]
	movs	r0, #0
	ldr	r1, [pc, #852]
	bl 0x0200c30c
	movs	r0, #0
	movs	r1, #2
	bl 0x0200c364
	adds	r3, r5, #0
	adds	r3, #91
	movs	r7, #0
	strb	r7, [r3, #0]
	adds	r0, r5, #0
	bl 0x0200c21c
	ldr	r3, [r5, #16]
	ldr	r2, [pc, #812]
	cmp	r3, r2
	ble.n	.L_02002964
	ldr	r3, [r5, #8]
	ldr	r1, [pc, #816]
	cmp	r3, r1
	ble.n	.L_0200297c
	adds	r0, r5, #0
	ldr	r2, [r5, #12]
	ldr	r3, [pc, #812]
	bl 0x0200c224
	adds	r0, r5, #0
	bl 0x0200c22c
	b.n	.L_0200297c
.L_02002964:
	ldr	r3, [r5, #8]
	ldr	r1, [pc, #800]
	cmp	r3, r1
	ble.n	.L_0200297c
	adds	r0, r5, #0
	ldr	r2, [r7, #12]
	ldr	r3, [pc, #792]
	bl 0x0200c224
	adds	r0, r5, #0
	bl 0x0200c22c
.L_0200297c:
	ldr	r3, [pc, #756]
	movs	r2, #0
	ldr	r1, [pc, #780]
	adds	r0, r5, #0
	bl 0x0200c224
	adds	r0, r5, #0
	bl 0x0200c22c
	movs	r0, #0
	movs	r1, #1
	bl 0x0200c364
	movs	r2, #40
	movs	r0, #0
	movs	r1, #0
	bl 0x0200c3c4
	bl 0x0200c45c
	movs	r1, #2
	movs	r0, #0
	bl 0x0200c384
	movs	r0, #20
	bl 0x0200c2d4
	movs	r0, #0
	movs	r1, #28
	bl 0x0200c364
	ldr	r1, [r5, #8]
	movs	r3, #128
	lsls	r3, r3, #10
	movs	r2, #152
	adds	r1, r1, r3
	movs	r0, #22
	ldr	r3, [r5, #16]
	lsls	r2, r2, #14
	bl 0x0200c20c
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02002a30
	movs	r1, #0
	adds	r0, #85
	strb	r1, [r0, #0]
	ldr	r6, [r7, #80]
	adds	r3, r6, #0
	adds	r3, #38
	strb	r1, [r3, #0]
	adds	r3, #1
	strb	r1, [r3, #0]
	movs	r3, #33
	ldrb	r2, [r6, #5]
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r3, r2
	movs	r1, #193
	strb	r3, [r6, #9]
	lsls	r1, r1, #3
	movs	r0, #17
	bl 0x0200c1b4
	adds	r5, r0, #0
	movs	r0, #242
	bl 0x0200c274
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r5, r5, r2
	movs	r1, #128
	adds	r2, r5, #0
	ldrb	r0, [r6, #28]
	bl 0x0200c1e4
	movs	r0, #17
	bl 0x0200c1bc
	movs	r0, #20
	bl 0x0200c2d4
	ldr	r3, [pc, #620]
	movs	r0, #80
	str	r3, [r7, #108]
	bl 0x0200c2d4
.L_02002a30:
	ldr	r6, [pc, #612]
	ldr	r0, [r6, #0]
	bl 0x0200c2fc
	movs	r3, #192
	lsls	r3, r3, #6
	mov	r8, r3
	mov	r2, r8
	strh	r2, [r0, #6]
	movs	r1, #128
	movs	r2, #0
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x0200c3d4
	movs	r1, #2
	ldr	r0, [r6, #0]
	bl 0x0200c384
	ldr	r3, [pc, #580]
	mov	sl, r3
	mov	r0, sl
	bl 0x0200c3a4
	ldr	r0, [r6, #0]
	movs	r1, #0
	movs	r2, #80
	bl 0x0200c3bc
	cmp	r7, #0
	beq.n	.L_02002a74
	adds	r0, r7, #0
	bl 0x0200c214
.L_02002a74:
	movs	r1, #1
	movs	r0, #0
	bl 0x0200c364
	movs	r0, #40
	bl 0x0200c2d4
	ldr	r0, [r6, #0]
	movs	r1, #6
	movs	r2, #40
	bl 0x0200c374
	ldr	r0, [r6, #0]
	movs	r1, #0
	movs	r2, #20
	bl 0x0200c3bc
	movs	r1, #224
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c3c4
	movs	r1, #208
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200c3c4
	movs	r5, #144
	ldr	r0, [r6, #0]
	lsls	r5, r5, #8
	movs	r2, #40
	orrs	r0, r5
	movs	r1, #0
	bl 0x0200c3bc
	ldr	r0, [r6, #0]
	movs	r1, #4
	bl 0x0200c36c
	ldr	r0, [r6, #0]
	movs	r1, #0
	orrs	r0, r5
	movs	r2, #20
	bl 0x0200c3bc
	ldr	r0, [r6, #0]
	mov	r1, r8
	movs	r2, #20
	bl 0x0200c3c4
	ldr	r0, [r6, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x0200c3bc
	ldr	r2, [pc, #400]
	ldr	r0, [r6, #0]
	ldr	r1, [pc, #400]
	bl 0x0200c30c
	movs	r1, #2
	ldr	r0, [r6, #0]
	bl 0x0200c364
	movs	r0, #55
	bl 0x0200c2fc
	adds	r7, r0, #0
	ldr	r2, [r7, #12]
	ldr	r1, [pc, #388]
	ldr	r3, [pc, #388]
	bl 0x0200c224
	adds	r0, r7, #0
	bl 0x0200c22c
	ldr	r3, [pc, #396]
	movs	r2, #0
	ldr	r1, [pc, #396]
	adds	r0, r7, #0
	bl 0x0200c224
	adds	r0, r7, #0
	bl 0x0200c22c
	movs	r0, #55
	movs	r1, #1
	bl 0x0200c364
	movs	r1, #160
	movs	r2, #10
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
	bl 0x0200c3c4
	ldr	r0, [r6, #0]
	movs	r1, #1
	bl 0x0200c384
	ldr	r0, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #5
	orrs	r0, r3
	movs	r1, #0
	movs	r2, #20
	bl 0x0200c3bc
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	ldr	r0, [r6, #0]
	bl 0x0200c30c
	movs	r0, #55
	bl 0x0200c2fc
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #2
	movs	r0, #55
	bl 0x0200c364
	movs	r3, #214
	movs	r2, #0
	lsls	r3, r3, #20
	ldr	r1, [pc, #300]
	adds	r0, r7, #0
	bl 0x0200c224
	adds	r0, r7, #0
	bl 0x0200c22c
	movs	r1, #1
	movs	r0, #55
	bl 0x0200c364
	movs	r0, #10
	bl 0x0200c2d4
	movs	r0, #55
	movs	r1, #2
	bl 0x0200c364
	movs	r2, #0
	ldr	r3, [pc, #252]
	ldr	r1, [pc, #256]
	adds	r0, r7, #0
	bl 0x0200c224
	adds	r0, r7, #0
	bl 0x0200c22c
	movs	r0, #55
	movs	r1, #1
	bl 0x0200c364
	mov	r0, sl
	movs	r1, #1
	adds	r0, #6
	bl 0x0200c264
	ldr	r3, [pc, #232]
	ldr	r2, [r3, #0]
	movs	r3, #236
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r0, #242
	bl 0x0200c2cc
	movs	r0, #20
	bl 0x0200c2d4
	ldr	r0, [r6, #0]
	movs	r1, #4
	bl 0x0200c364
	movs	r2, #10
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x0200c3bc
	movs	r0, #0
	movs	r1, #3
	bl 0x0200c36c
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200c36c
	ldr	r0, [r6, #0]
	movs	r1, #2
	bl 0x0200c364
	movs	r0, #0
	bl 0x0200c2fc
	cmp	r0, #0
	beq.n	.L_02002c20
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	ldr	r0, [r6, #0]
	bl 0x0200c32c
.L_02002c20:
	ldr	r0, [r6, #0]
	bl 0x0200c354
	movs	r2, #0
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x0200c35c
	bl 0x0200c464
	movs	r0, #128
	movs	r1, #6
	lsls	r0, r0, #9
	bl 0x0200c48c
	movs	r0, #20
	bl 0x0200c2d4
	bl 0x0200a7dc
	ldr	r0, [r6, #0]
	bl 0x0200c304
	movs	r0, #141
	lsls	r0, r0, #2
	bl 0x0200c29c
	ldr	r0, [pc, #88]
	bl 0x0200c294
	bl 0x0200c2e4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00016666
	.4byte 0x17880000
	.4byte 0x0d680000
	.4byte 0x00006666
	.4byte 0x0000cccc
	.4byte 0x176e0000
	.4byte 0x0d7d0000
	.4byte 0x177a0000
	.4byte 0x0d480000
	.4byte 0x17690000
	.4byte 0x0200813d
	.4byte 0x0200e79c
	.4byte 0x00002644
	.4byte 0x0d580000
	.4byte 0x17710000
	.4byte 0x176d0000
	.4byte 0x03001ebc
	.2byte 0x085d
	.2byte 0x0000
	.global Func_02002cb4
	.thumb_func
Func_02002cb4:
	push {r5, lr}
	ldr r3, [pc, #996]
	ldr r1, [pc, #996]
	adds r2, r3, r1
	ldrb r0, [r2]
	ldr r2, [pc, #996]
	adds r3, r3, r2
	ldrb r1, [r3]
	sub sp, #28
	bl 0x0200c27c
	bl 0x0200c2dc
	movs r0, #128
	movs r1, #150
	lsls r0, r0, #9
	lsls r1, r1, #1
	bl 0x0200c48c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r2, r2
	negs r0, r0
	negs r1, r1
	bl 0x0200c3f4
	movs r0, #5
	movs r1, #19
	bl 0x0200c364
	movs r0, #8
	movs r1, #5
	bl 0x0200c364
	movs r2, #0
	movs r1, #0
	movs r0, #0
	bl 0x0200c35c
	movs r0, #1
	bl 0x0200c184
	movs r0, #192
	lsls r0, r0, #9
	movs r1, #16
	bl 0x0200c48c
	ldr r5, [pc, #912]
	movs r1, #224
	ldr r3, [r5]
	lsls r1, r1, #1
	movs r2, #128
	adds r3, r3, r1
	lsls r2, r2, #1
	movs r1, #1
	ldr r0, [pc, #900]
	str r2, [r3]
	bl 0x0200c424
	ldr r3, [r5]
	movs r2, #228
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #16
	str r2, [r3]
	bl 0x0200c43c
	bl 0x0200c494
	bl 0x0200c45c
	movs r0, #40
	bl 0x0200c2d4
	movs r1, #1
	movs r0, #5
	bl 0x0200c384
	movs r0, #20
	bl 0x0200c2d4
	ldr r0, [pc, #852]
	bl 0x0200c3a4
	movs r2, #20
	movs r0, #5
	movs r1, #0
	bl 0x0200c3bc
	movs r1, #2
	movs r0, #8
	bl 0x0200c384
	movs r0, #20
	bl 0x0200c2d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c3bc
	movs r0, #5
	ldr r1, [pc, #812]
	movs r2, #20
	bl 0x0200c3d4
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl 0x0200c3bc
	movs r0, #8
	ldr r1, [pc, #796]
	movs r2, #80
	bl 0x0200c3d4
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x0200c3bc
	movs r0, #5
	movs r1, #2
	bl 0x0200c384
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl 0x0200c3bc
	movs r0, #8
	ldr r1, [pc, #760]
	movs r2, #100
	bl 0x0200c3d4
	movs r0, #5
	ldr r1, [pc, #748]
	movs r2, #40
	bl 0x0200c3d4
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl 0x0200c3bc
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x0200c3bc
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #20
	bl 0x0200c3d4
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl 0x0200c3bc
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl 0x0200c3dc
	movs r0, #80
	bl 0x0200c2d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c3bc
	movs r0, #5
	ldr r1, [pc, #672]
	movs r2, #80
	bl 0x0200c3d4
	movs r2, #120
	movs r0, #8
	movs r1, #0
	bl 0x0200c3bc
	movs r0, #5
	movs r1, #1
	bl 0x0200c384
	movs r0, #5
	movs r1, #0
	movs r2, #40
	bl 0x0200c3bc
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c3bc
	movs r0, #5
	ldr r1, [pc, #624]
	movs r2, #40
	bl 0x0200c3d4
	movs r0, #5
	movs r1, #0
	movs r2, #120
	bl 0x0200c3bc
	movs r0, #9
	ldr r1, [pc, #608]
	ldr r2, [pc, #608]
	bl 0x0200c30c
	movs r0, #9
	ldr r1, [pc, #604]
	ldr r2, [pc, #608]
	bl 0x0200c35c
	movs r0, #9
	ldr r1, [pc, #604]
	ldr r2, [pc, #604]
	bl 0x0200c344
	movs r2, #218
	ldr r1, [pc, #600]
	lsls r2, r2, #4
	movs r0, #9
	bl 0x0200c344
	movs r0, #20
	bl 0x0200c2d4
	ldr r0, [pc, #588]
	movs r1, #0
	movs r2, #20
	bl 0x0200c3bc
	movs r0, #8
	ldr r1, [pc, #580]
	movs r2, #0
	bl 0x0200c3d4
	movs r2, #60
	movs r0, #5
	ldr r1, [pc, #568]
	bl 0x0200c3d4
	movs r0, #9
	movs r1, #3
	bl 0x0200c36c
	movs r1, #0
	ldr r0, [pc, #548]
	bl 0x0200c3b4
	bl 0x0200c234
	movs r0, #20
	bl 0x0200c2d4
	ldr r1, [pc, #540]
	movs r0, #9
	bl 0x0200c314
	movs r0, #80
	bl 0x0200c2d4
	movs r0, #8
	movs r1, #1
	bl 0x0200c364
	movs r2, #40
	movs r0, #8
	movs r1, #4
	bl 0x0200c374
	movs r0, #5
	movs r1, #1
	bl 0x0200c364
	movs r0, #5
	movs r1, #4
	movs r2, #60
	bl 0x0200c374
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c3c4
	movs r1, #176
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200c3c4
	movs r0, #8
	ldr r1, [pc, #468]
	ldr r2, [pc, #472]
	bl 0x0200c30c
	ldr r2, [pc, #464]
	movs r0, #5
	ldr r1, [pc, #456]
	bl 0x0200c30c
	ldr r1, [pc, #460]
	movs r0, #8
	bl 0x0200c314
	movs r0, #20
	bl 0x0200c2d4
	ldr r0, [pc, #448]
	ldr r1, [pc, #452]
	bl 0x0200c3ec
	movs r1, #1
	ldr r0, [pc, #448]
	negs r1, r1
	ldr r2, [pc, #448]
	movs r3, #1
	bl 0x0200c3f4
	ldr r1, [pc, #444]
	movs r0, #5
	bl 0x0200c314
.L_02002cb4_0:
	movs r0, #10
	movs r1, #6
	bl 0x0200c364
	movs r1, #8
	movs r0, #6
	bl 0x0200c364
	movs r0, #1
	bl 0x0200c184
	movs r0, #5
	bl 0x0200c2fc
	adds r0, #100
	movs r1, #0
	ldrsh r3, [r0, r1]
	cmp r3, #0
	beq .L_02002cb4_0
	movs r0, #20
	bl 0x0200c2d4
	movs r1, #128
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200c3c4
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200c3dc
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #5
	bl 0x0200c3dc
	movs r0, #40
	bl 0x0200c2d4
	bl 0x0200c45c
	movs r3, #4
	movs r2, #12
	movs r1, #8
	movs r0, #9
	movs r4, #3
	str r2, [sp, #0]
	str r1, [sp, #4]
	str r0, [sp, #8]
	str r3, [sp, #12]
	str r3, [sp, #16]
	movs r2, #13
	movs r3, #2
	movs r1, #7
	movs r5, #0
	movs r0, #5
	str r4, [sp, #20]
	str r5, [sp, #24]
	bl 0x0200c3cc
	movs r0, #20
	bl 0x0200c2d4
	bl 0x0200c234
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200c3ec
	movs r1, #1
	movs r3, #1
	ldr r0, [pc, #288]
	negs r1, r1
	ldr r2, [pc, #276]
	bl 0x0200c3f4
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c3c4
	movs r0, #8
	ldr r1, [pc, #268]
	ldr r2, [pc, #272]
	bl 0x0200c30c
	movs r0, #5
	ldr r1, [pc, #260]
	ldr r2, [pc, #260]
	bl 0x0200c30c
	movs r0, #8
	ldr r1, [pc, #256]
	ldr r2, [pc, #260]
	bl 0x0200c33c
	ldr r2, [pc, #256]
	movs r0, #5
	ldr r1, [pc, #256]
	bl 0x0200c344
	movs r1, #1
	movs r0, #8
	bl 0x0200c364
	bl 0x0200c45c
	movs r0, #80
	bl 0x0200c2d4
	movs r0, #8
	movs r1, #1
	bl 0x0200c384
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c3bc
	movs r0, #5
	movs r1, #2
	bl 0x0200c384
	ldr r0, [pc, #212]
	movs r1, #0
	movs r2, #40
	bl 0x0200c3bc
	movs r1, #128
	movs r2, #20
	movs r0, #8
	lsls r1, r1, #8
	bl 0x0200c3c4
	movs r0, #8
	movs r1, #2
	bl 0x0200c37c
	movs r2, #60
	movs r1, #0
	movs r0, #8
	bl 0x0200c3bc
	bl 0x0200c234
	movs r0, #17
	bl 0x0200c4a4
	movs r1, #0
	movs r0, #0
	bl 0x0200c424
	movs r0, #120
	bl 0x0200c434
	movs r0, #120
	bl 0x0200c184
	ldr r0, [pc, #144]
	movs r1, #10
	bl 0x0200c404
	sub sp, #-28
	b .L_02002cb4_1
	.4byte 0x02000240
	.4byte 0x00000205
	.4byte 0x00000206
	.4byte 0x03001ebc
	.4byte 0x00010003
	.4byte 0x00002913
	.4byte 0x00000107
	.4byte 0x00000105
	.4byte 0x00006666
	.4byte 0x00003333
	.4byte 0x1ddc0000
	.4byte 0x0d840000
	.4byte 0x00001d94
	.4byte 0x00000d8c
	.4byte 0x00001d88
	.4byte 0x00006009
	.4byte 0x00000101
	.4byte 0x0200cf7c
	.4byte 0x00009999
	.4byte 0x00004ccc
	.4byte 0x0200d01c
	.4byte 0x0000b333
	.4byte 0x00001666
	.4byte 0x1e380000
	.4byte 0x0dc80000
	.4byte 0x0200d0a8
	.4byte 0x1e580000
	.4byte 0x00019999
	.4byte 0x0000cccc
	.4byte 0x00001e7c
	.4byte 0x00000db8
	.4byte 0x00000dd8
	.4byte 0x00001e6c
	.4byte 0x00001005
	.4byte 0x00000000
.L_02002cb4_1:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02003130
	.thumb_func
Func_02003130:
	push {lr}
	bl 0x0200c2dc
	movs r0, #141
	bl 0x0200c4a4
	movs r0, #0
	movs r1, #0
	bl 0x0200c42c
	movs r1, #0
	movs r0, #0
	bl 0x0200c424
	movs r0, #1
	bl 0x0200c434
	movs r0, #2
	bl 0x0200c184
	ldr r3, [pc, #148]
	movs r2, #228
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #1
	str r2, [r3]
	bl 0x0200c43c
	bl 0x0200c44c
	movs r1, #15
	movs r0, #0
	bl 0x0200c394
	movs r0, #0
	bl 0x0200c2fc
	movs r1, #0
	bl 0x0200c244
	movs r0, #1
	bl 0x0200c184
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl 0x0200c3ec
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #88]
	bl 0x0200c18c
	movs r0, #0
	movs r1, #0
	bl 0x0200c42c
	ldr r0, [pc, #80]
	movs r1, #1
	bl 0x0200c424
	movs r0, #128
	movs r1, #2
	lsls r0, r0, #9
	bl 0x0200c424
	movs r0, #40
	bl 0x0200c434
	movs r0, #240
	bl 0x0200c2d4
	movs r1, #0
	movs r0, #0
	bl 0x0200c424
	movs r0, #80
	bl 0x0200c434
	movs r0, #90
	bl 0x0200c184
	movs r0, #109
	bl 0x0200c40c
	movs r0, #141
	lsls r0, r0, #1
	bl 0x0200c294
	bl 0x0200c2e4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0200b1fd
	.4byte 0x00010004
	.global Func_020031fc
	.thumb_func
Func_020031fc:
	push {r5, r6, lr}
	bl 0x0200c19c
	lsls r5, r0, #2
	adds r5, r5, r0
	bl 0x0200c19c
	lsls r5, r5, #3
	lsls r3, r0, #4
	subs r3, r3, r0
	ldr r2, [pc, #184]
	lsrs r5, r5, #16
	lsls r3, r3, #1
	lsls r5, r5, #16
	adds r5, r5, r2
	lsrs r3, r3, #16
	ldr r2, [pc, #176]
	lsls r3, r3, #16
	adds r3, r3, r2
	movs r0, #222
	adds r1, r5, #0
	movs r2, #0
	bl 0x0200c20c
	adds r6, r0, #0
	cmp r6, #0
	beq .L_020031fc_0
	ldr r5, [r6, #80]
	bl 0x0200c19c
	ldr r3, [pc, #152]
	lsls r0, r0, #15
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r5, #0
	movs r1, #0
	adds r3, #38
	strb r1, [r3]
	movs r3, #13
	ldrb r2, [r5, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r5, #9]
	adds r3, r6, #0
	adds r3, #85
	strb r1, [r3]
	str r0, [r6, #24]
	str r0, [r6, #28]
	movs r1, #1
	adds r0, r6, #0
	bl 0x0200c1fc
	ldr r1, [pc, #108]
	adds r0, r6, #0
	bl 0x0200c204
.L_020031fc_0:
	ldr r3, [pc, #104]
	movs r1, #3
	ldr r0, [r3]
	bl 0x0200c17c
	cmp r0, #0
	bne .L_020031fc_1
	bl 0x0200c19c
	lsls r0, r0, #2
	lsrs r0, r0, #16
	cmp r0, #1
	beq .L_020031fc_2
	cmp r0, #1
	bcc .L_020031fc_3
	cmp r0, #2
	beq .L_020031fc_4
	cmp r0, #3
	beq .L_020031fc_5
	b .L_020031fc_1
.L_020031fc_3:
	movs r1, #1
	ldr r0, [pc, #68]
	b .L_020031fc_6
.L_020031fc_2:
	movs r1, #1
	ldr r0, [pc, #64]
	negs r1, r1
	ldr r2, [pc, #64]
	b .L_020031fc_7
.L_020031fc_4:
	movs r1, #1
	ldr r0, [pc, #56]
.L_020031fc_6:
	negs r1, r1
	ldr r2, [pc, #60]
.L_020031fc_7:
	movs r3, #1
	bl 0x0200c3f4
	b .L_020031fc_1
.L_020031fc_5:
	movs r1, #1
	ldr r0, [pc, #36]
	negs r1, r1
	ldr r2, [pc, #40]
	movs r3, #1
	bl 0x0200c3f4
.L_020031fc_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x17b00000
	.4byte 0x0c4c0000
	.4byte 0x00013333
	.4byte 0x0200d14c
	.4byte 0x03001e40
	.4byte 0x17c70000
	.4byte 0x17c90000
	.4byte 0x0c670000
	.4byte 0x0c690000
	.global Func_020032f0
	.thumb_func
Func_020032f0:
	push {lr}
	bl 0x0200c2dc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r2, r2
	negs r1, r1
	negs r0, r0
	bl 0x0200c3f4
	movs r0, #1
	bl 0x0200c184
	movs r0, #8
	movs r1, #2
	bl 0x0200c364
	movs r2, #202
	lsls r2, r2, #18
	ldr r1, [pc, #208]
	movs r0, #8
	bl 0x0200c35c
	movs r0, #8
	bl 0x0200c2fc
	movs r3, #160
	lsls r3, r3, #8
	strh r3, [r0, #6]
	movs r0, #1
	bl 0x0200c184
	ldr r0, [pc, #184]
	movs r1, #1
	bl 0x0200c48c
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl 0x0200c35c
	movs r1, #1
	movs r0, #8
	bl 0x0200c3e4
	movs r0, #1
	bl 0x0200c184
	ldr r3, [pc, #156]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	bl 0x0200c43c
	movs r0, #8
	ldr r1, [pc, #140]
	ldr r2, [pc, #144]
	bl 0x0200c30c
	movs r2, #178
	movs r0, #8
	ldr r1, [pc, #136]
	lsls r2, r2, #2
	bl 0x0200c334
	movs r2, #154
	movs r0, #8
	ldr r1, [pc, #128]
	lsls r2, r2, #2
	bl 0x0200c334
	movs r0, #8
	ldr r1, [pc, #124]
	ldr r2, [pc, #124]
	bl 0x0200c30c
	movs r2, #236
	movs r0, #8
	ldr r1, [pc, #108]
	lsls r2, r2, #1
	bl 0x0200c334
	movs r0, #8
	ldr r1, [pc, #88]
	ldr r2, [pc, #108]
	bl 0x0200c30c
	movs r2, #228
	movs r0, #8
	ldr r1, [pc, #100]
	lsls r2, r2, #1
	bl 0x0200c334
	movs r0, #8
	ldr r1, [pc, #88]
	ldr r2, [pc, #92]
	bl 0x0200c30c
	movs r2, #220
	lsls r2, r2, #1
	movs r0, #8
	ldr r1, [pc, #76]
	bl 0x0200c334
	movs r1, #1
	movs r0, #8
	bl 0x0200c364
	movs r0, #40
	bl 0x0200c2d4
	bl 0x0200c444
	bl 0x0200c44c
	movs r0, #110
	bl 0x0200c40c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x13080000
	.4byte 0x00013333
	.4byte 0x03001ebc
	.4byte 0x00006666
	.4byte 0x00003333
	.4byte 0x000012d8
	.4byte 0x000012a8
	.4byte 0x00004ccc
	.4byte 0x00002666
	.4byte 0x00001999
	.4byte 0x00001298
	.4byte 0x00000ccc
	.global Func_0200341c
	.thumb_func
Func_0200341c:
	push {r5, r6, lr}
	movs r0, #0
	bl 0x0200b448
	adds r6, r0, #0
	movs r0, #2
	bl 0x0200b448
	adds r6, r6, r0
	movs r0, #1
	bl 0x0200b448
	adds r5, r0, #0
	movs r0, #3
	bl 0x0200b448
	adds r5, r5, r0
	subs r6, r6, r5
	adds r0, r6, #0
	pop {r5, r6}
	pop {r1}
	bx r1
	.global Func_02003448
	.thumb_func
Func_02003448:
	push {r5, r6, lr}
	movs r6, #0
	cmp r0, #1
	beq .L_02003448_0
	cmp r0, #1
	bcc .L_02003448_1
	cmp r0, #2
	beq .L_02003448_2
	cmp r0, #3
	beq .L_02003448_3
	b .L_02003448_4
.L_02003448_1:
	ldr r6, [pc, #48]
	b .L_02003448_4
.L_02003448_0:
	ldr r6, [pc, #48]
	b .L_02003448_4
.L_02003448_2:
	ldr r6, [pc, #48]
	b .L_02003448_4
.L_02003448_6:
	ldr r3, [pc, #48]
	lsls r2, r5, #2
	ldr r0, [r3, r2]
	b .L_02003448_5
.L_02003448_3:
	movs r6, #153
	lsls r6, r6, #4
.L_02003448_4:
	movs r5, #0
.L_02003448_7:
	adds r0, r6, r5
	bl 0x0200c28c
	cmp r0, #0
	bne .L_02003448_6
	adds r5, #1
	cmp r5, #8
	bls .L_02003448_7
	movs r0, #0
.L_02003448_5:
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x0000092c
	.4byte 0x00000935
	.4byte 0x00000917
	.4byte 0x0200e718
	.global Func_020034a0
	.thumb_func
Func_020034a0:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #200]
	ldr r3, [r3]
	movs r2, #15
	ands r3, r2
	cmp r3, #0
	bne .L_020034a0_0
	movs r0, #8
	bl 0x0200c2fc
	adds r6, r0, #0
	ldr r1, [r6, #8]
	ldr r0, [pc, #180]
	ldr r3, [r6, #16]
	adds r1, r1, r0
	ldr r0, [pc, #180]
	ldr r2, [r6, #12]
	adds r3, r3, r0
	movs r0, #222
	bl 0x0200c20c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020034a0_0
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldr r7, [r5, #80]
	bl 0x0200c19c
	lsls r0, r0, #1
	lsrs r0, r0, #16
	cmp r0, #0
	beq .L_020034a0_1
	bl 0x0200c19c
	lsls r2, r0, #1
	adds r2, r2, r0
	lsls r2, r2, #4
	lsrs r2, r2, #16
	lsls r2, r2, #16
	ldr r3, [r5, #8]
	asrs r1, r2, #1
	subs r3, r3, r1
	str r3, [r5, #8]
	ldr r3, [r5, #16]
	subs r3, r3, r2
	b .L_020034a0_2
.L_020034a0_1:
	bl 0x0200c19c
	lsls r0, r0, #5
	ldr r3, [r5, #8]
	lsrs r0, r0, #16
	lsls r0, r0, #16
	adds r3, r3, r0
	str r3, [r5, #8]
	ldr r3, [r5, #16]
	asrs r0, r0, #1
	adds r3, r3, r0
.L_020034a0_2:
	str r3, [r5, #16]
	adds r2, r7, #0
	adds r2, #38
	movs r3, #0
	strb r3, [r2]
	ldr r3, [r6, #80]
	ldrb r3, [r3, #9]
	movs r2, #12
	ldrb r1, [r7, #9]
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	adds r1, r5, #0
	adds r1, #35
	orrs r3, r2
	ldrb r2, [r1]
	strb r3, [r7, #9]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	adds r3, r6, #0
	adds r3, #85
	ldrb r3, [r3]
	adds r2, r5, #0
	adds r2, #85
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #9
	bl 0x0200c39c
	adds r0, r5, #0
	movs r1, #2
	bl 0x0200c1fc
	ldr r1, [pc, #24]
	adds r0, r5, #0
	bl 0x0200c204
.L_020034a0_0:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.4byte 0xffe00000
	.4byte 0xfff00000
	.4byte 0x0200e73c
	.global Func_0200357c
	.thumb_func
Func_0200357c:
	push {r5, r6, lr}
	movs r0, #8
	bl 0x0200c2fc
	adds r5, r0, #0
	movs r0, #60
	bl 0x0200c2d4
	bl 0x0200c2dc
	ldr r0, [pc, #184]
	movs r1, #1
	bl 0x0200c48c
	ldr r3, [pc, #180]
	movs r1, #1
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r0, #8
	bl 0x0200c3e4
	movs r0, #1
	bl 0x0200c184
	movs r1, #15
	movs r0, #0
	bl 0x0200c394
	movs r0, #0
	bl 0x0200c2fc
	movs r1, #0
	bl 0x0200c244
	movs r0, #8
	bl 0x0200c2fc
	movs r1, #0
	bl 0x0200c244
	ldr r2, [pc, #132]
	movs r0, #8
	ldr r1, [pc, #132]
	bl 0x0200c30c
	adds r5, #100
	movs r3, #0
	strh r3, [r5]
	ldr r1, [pc, #124]
	movs r0, #8
	bl 0x0200c314
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #116]
	bl 0x0200c18c
	ldr r5, [pc, #116]
	movs r2, #224
	ldr r3, [r5]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	movs r1, #1
	str r2, [r3]
	ldr r0, [pc, #104]
	bl 0x0200c424
	movs r6, #228
	ldr r2, [r5]
	movs r3, #32
	lsls r6, r6, #1
	str r3, [r2, r6]
	bl 0x0200c43c
	movs r0, #120
	bl 0x0200c2d4
	movs r1, #150
	lsls r1, r1, #1
	ldr r0, [pc, #76]
	bl 0x0200c48c
	movs r0, #135
	lsls r0, r0, #1
	bl 0x0200c2d4
	ldr r2, [r5]
	movs r3, #16
	str r3, [r2, r6]
	ldr r3, [pc, #20]
	movs r2, #160
	lsls r2, r2, #19
	strh r3, [r2]
	bl 0x0200c444
	bl 0x0200c44c
	movs r0, #111
	bl 0x0200c40c
	b .L_0200357c_0
	.4byte 0x00007fff
	.4byte 0x00009999
	.4byte 0x00013333
	.4byte 0x00003333
	.4byte 0x00006666
	.4byte 0x0200d218
	.4byte 0x0200b4a1
	.4byte 0x03001ebc
	.4byte 0x00010003
	.4byte 0x00016666
.L_0200357c_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02003678
	.thumb_func
Func_02003678:
	push {r5, r6, lr}
	ldr r3, [pc, #84]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r0, [r3]
	bl 0x0200c2fc
	ldr r3, [pc, #72]
	ldr r6, [r3]
	ldr r3, [pc, #72]
	ldr r3, [r3]
	lsls r3, r3, #12
	strh r3, [r0, #6]
	movs r0, #190
	lsls r0, r0, #2
	bl 0x0200c2a4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02003678_0
	cmp r5, #1
	bne .L_02003678_1
	movs r3, #193
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #99
	strh r3, [r2]
	b .L_02003678_0
.L_02003678_1:
	movs r0, #131
	lsls r0, r0, #1
	bl 0x0200c28c
	cmp r0, #0
	bne .L_02003678_0
	subs r5, #1
.L_02003678_0:
	movs r0, #190
	lsls r0, r0, #2
	adds r1, r5, #0
	bl 0x0200c2ac
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x03001ebc
	.4byte 0x03001e40
	.global Func_020036dc
	.thumb_func
Func_020036dc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r2, [pc, #196]
	movs r3, #250
	mov r10, r2
	lsls r3, r3, #1
	add r3, r10
	ldr r6, [r3]
	adds r7, r0, #0
	adds r0, r6, #0
	bl 0x0200c2fc
	adds r5, r0, #0
	movs r0, #188
	lsls r0, r0, #2
	bl 0x0200c28c
	mov r8, r0
	cmp r0, #0
	bne .L_020036dc_0
	bl 0x0200c2dc
	adds r0, r6, #0
	ldr r1, [pc, #160]
	bl 0x0200c3dc
	adds r0, r6, #0
	movs r1, #9
	bl 0x0200c364
	adds r0, r7, #0
	bl 0x0200c2fc
	cmp r0, #0
	beq .L_020036dc_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	adds r0, r6, #0
	bl 0x0200c32c
.L_020036dc_1:
	adds r0, r6, #0
	bl 0x0200c354
	movs r0, #244
	bl 0x0200c4a4
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #108]
	adds r7, r5, #0
	bl 0x0200c18c
	adds r7, #85
	mov r2, r8
	strb r2, [r7]
	movs r3, #128
	ldr r2, [r5, #12]
	lsls r3, r3, #14
	ldr r1, [r5, #8]
	adds r2, r2, r3
	adds r0, r5, #0
	ldr r3, [r5, #16]
	bl 0x0200c224
	adds r0, r6, #0
	bl 0x0200c354
	mov r2, r8
	str r2, [r5, #40]
	movs r2, #249
	movs r3, #4
	lsls r2, r2, #1
	add r2, r10
	strb r3, [r7]
	movs r0, #188
	movs r3, #2
	strb r3, [r2]
	lsls r0, r0, #2
	bl 0x0200c294
	movs r0, #190
	lsls r0, r0, #2
	movs r1, #180
	bl 0x0200c2ac
	bl 0x0200c2e4
	ldr r3, [pc, #36]
	movs r2, #190
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r2, r8
	strh r2, [r3]
.L_020036dc_0:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x00000101
	.4byte 0x0200b679
	.4byte 0x03001ebc
	.global Func_020037bc
	.thumb_func
Func_020037bc:
	push {lr}
	movs r0, #54
	bl 0x0200b6dc
	pop {r0}
	bx r0
	.global Func_020037c8
	.thumb_func
Func_020037c8:
	ldr r3, [pc, #8]
	ldr r3, [r3]
	movs r2, #1
	adds r3, #52
	strb r2, [r3]
	bx lr
	.4byte 0x03001f30
	.global Func_020037d8
	.thumb_func
Func_020037d8:
	push {r5, lr}
	ldr r3, [pc, #96]
	movs r1, #193
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r1, #0
	ldrsh r3, [r2, r1]
	cmp r3, #99
	bne .L_020037d8_0
	movs r3, #0
	strh r3, [r2]
.L_020037d8_0:
	movs r0, #188
	lsls r0, r0, #2
	bl 0x0200c29c
	ldr r0, [pc, #68]
	bl 0x0200c294
	movs r0, #190
	lsls r0, r0, #2
	movs r1, #0
	bl 0x0200c2ac
	movs r0, #98
	movs r1, #5
	bl 0x0200c414
	ldr r5, [pc, #48]
	ldr r3, [pc, #52]
	adds r2, r5, r3
	movs r3, #3
	strb r3, [r2]
	movs r0, #98
	movs r1, #7
	bl 0x0200c414
	movs r1, #250
	lsls r1, r1, #1
	adds r5, r5, r1
	ldr r0, [r5]
	bl 0x0200c2fc
	movs r3, #2
	adds r0, #85
	strb r3, [r0]
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x000002f1
	.4byte 0x02000240
	.4byte 0x0000022b
	.global Func_0200384c
	.thumb_func
Func_0200384c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #156]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r0, [r3]
	bl 0x0200c2fc
	adds r5, r0, #0
	movs r0, #54
	bl 0x0200c2fc
	mov r8, r0
	bl 0x0200c2dc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r1, r1
	negs r0, r0
	bl 0x0200c3f4
	movs r0, #219
	bl 0x0200c4a4
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200c244
	mov r2, r8
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	adds r2, r5, #0
	adds r2, #85
	strb r3, [r2]
	str r3, [r5, #40]
	adds r2, #12
	movs r3, #1
	strb r3, [r2]
	mov r2, r8
	adds r2, #97
	strb r3, [r2]
	ldr r7, [pc, #72]
	movs r6, #59
.L_0200384c_0:
	ldr r3, [r5, #40]
	adds r3, r3, r7
	str r3, [r5, #40]
	mov r2, r8
	ldr r3, [r2, #40]
	adds r3, r3, r7
	str r3, [r2, #40]
	movs r0, #1
	subs r6, #1
	bl 0x0200c184
	cmp r6, #0
	bge .L_0200384c_0
	bl 0x0200c444
	bl 0x0200c44c
	bl 0x0200c2e4
	movs r0, #145
	lsls r0, r0, #1
	bl 0x0200c294
	ldr r0, [pc, #24]
	movs r1, #27
	bl 0x0200c404
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00003333
	.4byte 0x00000002
	.global Func_020038fc
	.thumb_func
Func_020038fc:
	push {lr}
	ldr r0, [pc, #40]
	ldr r1, [pc, #40]
	bl 0x0200c1ec
	ldr r3, [pc, #40]
	ldr r3, [r3]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_020038fc_0
	ldr r3, [pc, #32]
	ldr r1, [pc, #12]
	ldrh r3, [r3]
	ldr r0, [pc, #28]
	orrs r1, r3
	bl 0x0200c1ec
	b .L_020038fc_1
	.2byte 0x0000
	.4byte 0x0000000c
	.4byte 0x04000050
	.4byte 0x00003f41
	.4byte 0x03001e40
	.4byte 0x0200e7a0
	.4byte 0x04000052
.L_020038fc_0:
	ldr r3, [pc, #20]
	ldr r1, [pc, #16]
	ldrh r3, [r3]
	ldr r0, [pc, #20]
	orrs r1, r3
	bl 0x0200c1ec
.L_020038fc_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000010
	.4byte 0x0200e7a0
	.4byte 0x04000052
	.global Func_0200395c
	.thumb_func
Func_0200395c:
	push {r5, r6, lr}
	ldr r3, [pc, #144]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r0, [r3]
	bl 0x0200c2fc
	movs r3, #10
	ldrsh r5, [r0, r3]
	ldr r3, [pc, #128]
	movs r2, #18
	ldrsh r6, [r0, r2]
	movs r1, #3
	ldr r0, [r3]
	bl 0x0200c17c
	cmp r0, #0
	bne .L_0200395c_0
	bl 0x0200c19c
	lsls r0, r0, #2
	lsrs r0, r0, #16
	cmp r0, #1
	beq .L_0200395c_1
	cmp r0, #1
	bcc .L_0200395c_2
	cmp r0, #2
	beq .L_0200395c_3
	cmp r0, #3
	beq .L_0200395c_4
	b .L_0200395c_0
.L_0200395c_2:
	ldr r3, [pc, #88]
	lsls r0, r5, #16
	adds r0, r0, r3
	movs r3, #128
	lsls r3, r3, #9
	lsls r2, r6, #16
	movs r1, #1
	b .L_0200395c_5
.L_0200395c_1:
	movs r2, #128
	lsls r2, r2, #9
	ldr r3, [pc, #68]
	lsls r0, r5, #16
	adds r0, r0, r2
	movs r1, #1
	lsls r2, r6, #16
	b .L_0200395c_5
.L_0200395c_3:
	movs r3, #128
	lsls r3, r3, #9
	lsls r0, r5, #16
	lsls r2, r6, #16
	movs r1, #1
	adds r0, r0, r3
.L_0200395c_5:
	adds r2, r2, r3
	negs r1, r1
	movs r3, #1
	bl 0x0200c3f4
	b .L_0200395c_0
.L_0200395c_4:
	ldr r3, [pc, #32]
	lsls r0, r5, #16
	lsls r2, r6, #16
	movs r1, #1
	adds r0, r0, r3
	adds r2, r2, r3
	negs r1, r1
	movs r3, #1
	bl 0x0200c3f4
.L_0200395c_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x03001e40
	.4byte 0xffff0000
	.global Func_020039fc
	.thumb_func
Func_020039fc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r8, r0
	movs r0, #128
	lsls r0, r0, #7
	bl 0x0200c1c4
	adds r7, r0, #0
	movs r0, #1
	bl 0x0200c184
	ldr r0, [pc, #280]
	bl 0x0200c29c
	bl 0x0200c24c
	adds r1, r7, #0
	ldr r0, [pc, #272]
	bl 0x0200c1d4
	movs r2, #128
	lsls r2, r2, #5
	adds r1, r7, r2
	ldr r0, [pc, #264]
	bl 0x0200c1d4
	ldr r6, [pc, #264]
	ldr r5, [pc, #264]
	ldrh r3, [r5]
	adds r0, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_020039fc_0
	lsls r3, r2, #1
	adds r3, r3, r2
	mov r1, r8
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r6]
	lsls r2, r1, #5
	ldr r1, [pc, #240]
	adds r3, r3, r6
	adds r3, #4
	adds r2, r2, r1
	stmia r3!, {r2}
	ldr r2, [pc, #236]
	stmia r3!, {r2}
	ldr r2, [pc, #236]
	str r2, [r3]
.L_020039fc_0:
	strh r0, [r5]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_020039fc_1
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r6
	adds r3, #4
	adds r2, #1
	stmia r3!, {r7}
	strh r2, [r6]
	ldr r2, [pc, #204]
	stmia r3!, {r2}
	ldr r2, [pc, #204]
	str r2, [r3]
.L_020039fc_1:
	strh r1, [r5]
	ldr r0, [pc, #204]
	movs r1, #200
	lsls r1, r1, #4
	bl 0x0200c18c
	bl 0x0200c2dc
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_020039fc_2
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #234
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, [pc, #160]
	stmia r3!, {r2}
	ldr r2, [pc, #160]
	str r2, [r3]
.L_020039fc_2:
	strh r1, [r5]
	ldr r3, [pc, #156]
	movs r1, #250
	lsls r1, r1, #1
	adds r3, r3, r1
	ldr r0, [r3]
	bl 0x0200c2fc
	movs r3, #0
	adds r0, #84
	strb r3, [r0]
	ldr r3, [pc, #140]
	movs r2, #228
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #16
	str r2, [r3]
	bl 0x0200c43c
	bl 0x0200c44c
	movs r0, #246
	bl 0x0200c4a4
	ldr r3, [pc, #116]
	mov r8, r3
	ldr r3, [pc, #48]
	mov r0, r8
	strh r3, [r0]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_020039fc_3
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #210
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, [pc, #56]
	stmia r3!, {r2}
	ldr r2, [pc, #56]
	str r2, [r3]
	b .L_020039fc_3
	.2byte 0x0000
	.4byte 0x00000e00
	.4byte 0x00000109
	.4byte 0x0200c7a6
	.4byte 0x0200c4ec
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x0200c4ac
	.4byte 0x050001c0
	.4byte 0x80000010
	.4byte 0x06001000
	.4byte 0x84000400
	.4byte 0x0200b8fd
	.4byte 0x06002000
	.4byte 0x84000140
	.4byte 0x02000240
	.4byte 0x03001ebc
	.4byte 0x0200e7a0
.L_020039fc_3:
	strh r1, [r5]
	movs r0, #2
	bl 0x0200c2d4
	ldr r3, [pc, #48]
	mov r1, r8
	strh r3, [r1]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_020039fc_4
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #186
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, [pc, #12]
	stmia r3!, {r2}
	ldr r2, [pc, #12]
	str r2, [r3]
	b .L_020039fc_4
	.2byte 0x0000
	.4byte 0x00000d00
	.4byte 0x06002000
	.4byte 0x84000140
.L_020039fc_4:
	strh r1, [r5]
	movs r0, #2
	bl 0x0200c2d4
	ldr r3, [pc, #48]
	mov r1, r8
	strh r3, [r1]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_020039fc_5
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #162
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, [pc, #12]
	stmia r3!, {r2}
	ldr r2, [pc, #12]
	str r2, [r3]
	b .L_020039fc_5
	.2byte 0x0000
	.4byte 0x00000c00
	.4byte 0x06002000
	.4byte 0x84000140
.L_020039fc_5:
	strh r1, [r5]
	movs r0, #2
	bl 0x0200c2d4
	movs r1, #176
	lsls r1, r1, #4
	mov r11, r1
	mov r2, r11
	mov r3, r8
	strh r2, [r3]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_020039fc_6
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #138
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, [pc, #192]
	stmia r3!, {r2}
	ldr r2, [pc, #192]
	str r2, [r3]
.L_020039fc_6:
	strh r1, [r5]
	movs r0, #2
	bl 0x0200c2d4
	movs r1, #160
	lsls r1, r1, #4
	mov r9, r1
	mov r2, r9
	mov r3, r8
	strh r2, [r3]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_020039fc_7
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #228
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #5
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, [pc, #128]
	stmia r3!, {r2}
	ldr r2, [pc, #128]
	str r2, [r3]
.L_020039fc_7:
	strh r1, [r5]
	movs r0, #2
	bl 0x0200c2d4
	movs r1, #144
	lsls r1, r1, #4
	mov r10, r1
	mov r2, r10
	mov r3, r8
	strh r2, [r3]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_020039fc_8
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #180
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #5
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, [pc, #68]
	stmia r3!, {r2}
	ldr r2, [pc, #68]
	str r2, [r3]
.L_020039fc_8:
	strh r1, [r5]
	movs r0, #2
	bl 0x0200c2d4
	ldr r3, [pc, #48]
	mov r1, r8
	strh r3, [r1]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_020039fc_9
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #132
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #5
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, [pc, #12]
	stmia r3!, {r2}
	ldr r2, [pc, #12]
	str r2, [r3]
	b .L_020039fc_9
	.4byte 0x00000800
	.4byte 0x06002000
	.4byte 0x84000140
.L_020039fc_9:
	strh r1, [r5]
	movs r0, #140
	bl 0x0200c2d4
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_020039fc_10
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #180
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #5
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, [pc, #336]
	stmia r3!, {r2}
	ldr r2, [pc, #336]
	str r2, [r3]
.L_020039fc_10:
	strh r1, [r5]
	movs r0, #4
	bl 0x0200c2d4
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_020039fc_11
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #228
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #5
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, [pc, #284]
	stmia r3!, {r2}
	ldr r2, [pc, #284]
	str r2, [r3]
.L_020039fc_11:
	strh r1, [r5]
	movs r0, #4
	bl 0x0200c2d4
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_020039fc_12
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #138
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, [pc, #236]
	stmia r3!, {r2}
	ldr r2, [pc, #236]
	str r2, [r3]
.L_020039fc_12:
	strh r1, [r5]
	movs r0, #4
	bl 0x0200c2d4
	mov r1, r10
	mov r2, r8
	strh r1, [r2]
	ldrh r3, [r5]
	adds r1, r3, #0
.L_02003db0:
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_02003db0_0
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #162
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, [pc, #180]
	stmia r3!, {r2}
	ldr r2, [pc, #180]
	str r2, [r3]
.L_02003db0_0:
	strh r1, [r5]
	movs r0, #4
	bl 0x0200c2d4
	mov r1, r9
	mov r2, r8
	strh r1, [r2]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_02003db0_1
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #186
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, [pc, #124]
	stmia r3!, {r2}
	ldr r2, [pc, #124]
	str r2, [r3]
.L_02003db0_1:
	strh r1, [r5]
	movs r0, #4
	bl 0x0200c2d4
	mov r1, r11
	mov r2, r8
	strh r1, [r2]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_02003db0_2
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #210
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, [pc, #68]
	stmia r3!, {r2}
	ldr r2, [pc, #68]
	str r2, [r3]
.L_02003db0_2:
	strh r1, [r5]
	movs r0, #4
	bl 0x0200c2d4
	ldr r3, [pc, #48]
	mov r1, r8
	strh r3, [r1]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_02003db0_3
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #234
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, [pc, #12]
	stmia r3!, {r2}
	ldr r2, [pc, #12]
	str r2, [r3]
	b .L_02003db0_3
	.4byte 0x00000c00
	.4byte 0x06002000
	.4byte 0x84000140
.L_02003db0_3:
	strh r1, [r5]
	bl 0x0200c254
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #80]
	bl 0x0200c18c
	movs r0, #141
	bl 0x0200c4a4
	ldr r3, [pc, #52]
	mov r1, r8
	strh r3, [r1]
	movs r0, #4
	bl 0x0200c2d4
	ldr r3, [pc, #44]
	mov r2, r8
	strh r3, [r2]
	movs r0, #4
	bl 0x0200c2d4
	ldr r3, [pc, #36]
	mov r0, r8
	strh r3, [r0]
	movs r0, #4
	bl 0x0200c2d4
	ldr r3, [pc, #28]
	mov r1, r8
	strh r3, [r1]
	movs r0, #45
	bl 0x0200c2d4
	bl 0x0200c444
	b .L_02003db0_4
	.4byte 0x00000d00
	.4byte 0x00000e00
	.4byte 0x00000f00
	.4byte 0x00001000
	.4byte 0x0200b95d
.L_02003db0_4:
	bl 0x0200c44c
	adds r0, r7, #0
	bl 0x0200c1cc
	ldr r0, [pc, #20]
	bl 0x0200c294
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00000101
	.global Func_02003f10
	.thumb_func
Func_02003f10:
	push {r5, lr}
	ldr r3, [pc, #52]
	ldr r3, [r3]
	movs r2, #2
	ands r3, r2
	adds r5, r0, #0
	cmp r3, #0
	beq .L_02003f10_0
	movs r1, #7
	bl 0x0200c25c
	b .L_02003f10_1
.L_02003f10_0:
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200c25c
.L_02003f10_1:
	ldr r3, [pc, #20]
	ldr r3, [r3]
	movs r2, #15
	ands r3, r2
	cmp r3, #0
	bne .L_02003f10_2
	adds r0, r5, #0
	bl 0x0200c058
.L_02003f10_2:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.global Func_02003f4c
	.thumb_func
Func_02003f4c:
	push {r5, r6, lr}
	ldr r5, [pc, #52]
	ldr r3, [r5]
	movs r2, #1
	ands r3, r2
	adds r6, r0, #0
	cmp r3, #0
	beq .L_02003f4c_0
	ldr r0, [r5]
	movs r1, #6
	lsrs r0, r0, #1
	bl 0x0200c17c
	adds r1, r0, #0
	adds r0, r6, #0
	bl 0x0200c25c
.L_02003f4c_0:
	ldr r3, [r5]
	movs r2, #15
	ands r3, r2
	cmp r3, #0
	bne .L_02003f4c_1
	adds r0, r6, #0
	bl 0x0200c058
.L_02003f4c_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.global Func_02003f88
	.thumb_func
Func_02003f88:
	push {r5, lr}
	adds r5, r0, #0
	ldr r0, [pc, #32]
	ldr r3, [r0]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02003f88_0
	ldr r0, [r0]
	movs r1, #6
	lsrs r0, r0, #1
	bl 0x0200c17c
	adds r1, r0, #0
	adds r0, r5, #0
	bl 0x0200c25c
.L_02003f88_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.global Func_02003fb4
	.thumb_func
Func_02003fb4:
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
	ble .L_02003fb4_0
	adds r0, r5, #0
	bl 0x0200c214
	b .L_02003fb4_1
.L_02003fb4_0:
	lsls r0, r0, #10
	bl 0x0200c1a4
	str r0, [r5, #24]
	str r0, [r5, #28]
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
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5, #16]
.L_02003fb4_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02004004
	.thumb_func
Func_02004004:
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
	ble .L_02004004_0
	adds r0, r5, #0
	bl 0x0200c214
	b .L_02004004_1
.L_02004004_0:
	lsls r0, r0, #10
	bl 0x0200c1a4
	negs r3, r0
	str r0, [r5, #24]
	str r3, [r5, #28]
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
.L_02004004_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02004058
	.thumb_func
Func_02004058:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #72]
	ldr r3, [r3]
	sub sp, #8
	movs r1, #63
	adds r6, r0, #0
	mov r11, r3
	movs r7, #0
	mov r10, sp
	mov r9, r1
.L_02004058_2:
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	ldr r1, [r6, #8]
	movs r0, #26
	bl 0x0200c20c
	lsls r3, r7, #2
	mov r2, r10
	str r0, [r3, r2]
	cmp r0, #0
	beq .L_02004058_0
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
	beq .L_02004058_0
	b .L_02004058_1
	.4byte 0x00000000
	.4byte 0x03001f30
.L_02004058_1:
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200c1f4
	adds r3, r5, #0
	adds r3, #38
	mov r2, r8
	strb r2, [r3]
	ldrb r0, [r5, #28]
	bl 0x0200c1dc
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
	b .L_02004058_0
	.2byte 0x0000
	.4byte 0xfffffc00
	.4byte 0x03001b10
.L_02004058_0:
	adds r7, #1
	cmp r7, #1
	ble .L_02004058_2
	ldr r2, [sp, #0]
	ldr r3, [pc, #60]
	ldr r0, [r2, #80]
	str r3, [r2, #108]
	movs r2, #13
	ldrb r1, [r0, #9]
	negs r2, r2
	adds r3, r2, #0
	movs r4, #4
	ands r3, r1
	orrs r3, r4
	strb r3, [r0, #9]
	mov r3, r10
	ldr r1, [r3, #4]
	ldr r0, [r1, #80]
	ldrb r3, [r0, #9]
	ands r2, r3
	ldr r3, [pc, #32]
	orrs r2, r4
	str r3, [r1, #108]
	adds r1, #35
	movs r3, #2
	strb r2, [r0, #9]
	strb r3, [r1]
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200c005
	.4byte 0x0200bfb5
	.include "games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/IMPORT.INC"
	.4byte 0x7c1f7c1f
	.4byte 0x20a21861
	.4byte 0x45643503
	.4byte 0x6a2659c5
	.4byte 0x7ecd7e87
	.4byte 0x7f997f33
	.4byte 0x7c1f7fff
	.4byte 0x00007c1f
	.4byte 0x7c1f7c1f
	.4byte 0x013300ce
	.4byte 0x01b80176
	.4byte 0x023b01fa
	.4byte 0x02de027d
	.4byte 0x337f033f
	.4byte 0x613d7fff
	.4byte 0x00006dff
	.4byte 0xe8802b01
	.4byte 0xec020200
	.4byte 0x0596020b
	.4byte 0x03280008
	.4byte 0x103c00c4
	.4byte 0xec870201
	.4byte 0x2c400086
	.4byte 0x873fe086
	.4byte 0x2b4000e0
	.4byte 0x4000be06
	.4byte 0x00be0826
	.4byte 0x120d2340
	.4byte 0x60800088
	.4byte 0x0192e091
	.4byte 0xe491e402
	.4byte 0x25400082
	.4byte 0xe0a1e0a0
	.4byte 0xe40201a2
	.4byte 0xa0e4a108
	.4byte 0x234000e4
	.4byte 0x21b1e0b0
	.4byte 0x0201b2e0
	.4byte 0xb0e4b1e4
	.4byte 0x09254000
	.4byte 0xb2e8b1e8
	.4byte 0xb1ec0201
	.4byte 0x00840a01
	.4byte 0xa1e825c0
	.4byte 0x0201a2e8
	.4byte 0x01cfa1ec
	.4byte 0x2740100a
	.4byte 0x020192e8
	.4byte 0x40200601
	.4byte 0x020e202c
	.4byte 0xff4000f7
	.4byte 0x007f4000
	.4byte 0x40000750
	.4byte 0x8000e049
	.4byte 0x0e40106e
	.4byte 0xe2ff0060
	.4byte 0x081b4000
	.4byte 0x628000be
	.4byte 0x019de09c
	.4byte 0xbf41e402
	.4byte 0xab264000
	.4byte 0xade0ace0
	.4byte 0xe4080201
	.4byte 0x50abe4ac
	.4byte 0xe0bb24c0
	.4byte 0xbde021bc
	.4byte 0xbce40201
	.4byte 0x4000bbe4
	.4byte 0xbce80925
	.4byte 0x0201bde8
	.4byte 0x0a01bcec
	.4byte 0x25c00084
	.4byte 0xade8ace8
	.4byte 0xacec0201
	.4byte 0x100a01cf
	.4byte 0x9de82740
	.4byte 0x06010201
	.4byte 0x0f278010
	.4byte 0x4000fa14
	.4byte 0xeb40609f
	.4byte 0x502f4050
	.4byte 0x40003c80
	.4byte 0x0060e04e
	.4byte 0x60aae02e
	.4byte 0x60e02e00
	.4byte 0x60e02e00
	.4byte 0x50e02e00
	.4byte 0xc4e02e80
	.4byte 0x002e0060
	.4byte 0xe08e8880
	.4byte 0xe402018f
	.4byte 0x00e4448e
	.4byte 0xe08c2740
	.4byte 0xe402018d
	.4byte 0x40009a8c
	.4byte 0x018de829
	.4byte 0xec060102
	.4byte 0xe828c000
	.4byte 0x02018f7f
	.4byte 0x00300601
	.4byte 0xff400030
	.4byte 0xb0ce4000
	.4byte 0x40001f40
	.4byte 0x00c0f8af
	.4byte 0x8700c0ff
	.4byte 0x00174400
	.4byte 0x40000780
	.4byte 0x9ae0ba20
	.4byte 0xe4020188
	.4byte 0x4000e4ba
	.4byte 0xaae0b927
	.4byte 0xe4020193
	.4byte 0x294000b9
	.4byte 0x0201aae8
	.4byte 0xec4f0601
	.4byte 0xe828c000
	.4byte 0x0102019a
	.4byte 0x2c401006
	.4byte 0xfe228490
	.4byte 0x40b34000
	.4byte 0x80002b44
	.4byte 0x2f045073
	.4byte 0x00373c00
	.4byte 0xb610a340
	.4byte 0x00888800
	.4byte 0xec822c40
	.4byte 0x2c4000b5
	.4byte 0xbe82e0b5
	.4byte 0x882c0060
	.4byte 0x082c0010
	.4byte 0x264000be
	.4byte 0x4000be08
	.4byte 0x40e18826
	.4byte 0xbe0a2c00
	.4byte 0x83244000
	.4byte 0x00e483e0
	.4byte 0x85112940
	.4byte 0x020193e0
	.4byte 0x00e485e4
	.4byte 0xe8392840
	.4byte 0x01020193
	.4byte 0x2bc00006
	.4byte 0xc05083e8
	.4byte 0xc030f6b9
	.4byte 0xff400028
	.4byte 0x001e3050
	.4byte 0x00e04f40
	.4byte 0x40106e40
	.4byte 0xec22b70e
	.4byte 0x2c4000b6
	.4byte 0x00a6eca7
	.4byte 0x3fa62c40
	.4byte 0x0060a7e0
	.4byte 0x00be082a
	.4byte 0xbe082640
	.4byte 0x08264000
	.4byte 0x4000febe
	.4byte 0x003e0a24
	.4byte 0xfc0a2240
	.4byte 0x0e268000
	.4byte 0x20c04042
	.4byte 0xb3e013b3
	.4byte 0x2c4000e4
	.4byte 0x40a0b3e8
	.4byte 0x1f309035
	.4byte 0xab4000f5
	.4byte 0x00ff0200
	.4byte 0x0060ff02
	.4byte 0x80709820
	.2byte 0x972e
.L_02004752:
	ands	r0, r0
	ldrb	r4, [r5, #28]
	sub	sp, #92
	cmp	r4, #64
	.2byte 0xbe08
	strh	r0, [r0, #0]
	movs	r1, r5
	cmp	r4, #64
	.2byte 0xbe08
	strh	r0, [r2, #2]
	lsrs	r6, r4, #32
	sub	sp, #248
	strh	r0, [r0, #0]
	.2byte 0xec27
	.2byte 0x8000
	str	r0, [r7, r0]
	movs	r2, #192
	.2byte 0xe081
	.2byte 0xe481
	.4byte 0x2c40009f
	.4byte 0x006081e8
	.4byte 0xff0200ff
	.4byte 0x00ff0200
	.4byte 0xa2412102
	.4byte 0xff0200df
	.4byte 0xb77a0200
	.4byte 0x00ff1210
	.4byte 0x0200ff02
	.4byte 0xff0200ff
	.4byte 0x80100200
	.4byte 0xff000000
	.4byte 0x1141387d
	.4byte 0x7ffe6715
	.4byte 0xe6889329
	.2byte 0xaad0
	.2byte 0x65ff
	push	{r1, r4, r6, lr}
	strb	r3, [r7, #11]
	.2byte 0xd968
	ldrh	r3, [r1, #50]
	adds	r3, #117
	.2byte 0xfbff
	.2byte 0x82bc
	ldr	r0, [sp, #188]
	strh	r1, [r4, #46]
	ldr	r7, [pc, #612]
	stmia	r5!, {r0, r3}
	str	r4, [sp, #492]
	lsrs	r1, r6, #7
	subs	r6, #28
	stmia	r0!, {r0, r1, r2, r3, r4, r5, r6, r7}
	asrs	r7, r4, #21
	subs	r1, #148
	.2byte 0xea94
	.2byte 0x99cb
	add	r3, sp, #272
	rors	r4, r3
	ldr	r4, [pc, #724]
	adds	r5, r3, #6
	str	r0, [sp, #820]
	str	r5, [sp, #464]
	adds	r3, r7, r0
	pop	{r1, r3, r4, r6, r7, pc}
	.2byte 0x465b
	.4byte 0x4519b111
	.4byte 0x630f76cc
	.4byte 0x71176d4e
	.4byte 0x3177ddea
	.4byte 0xbaa666eb
	.4byte 0xc39531cc
	.4byte 0xbc9f60a7
	.4byte 0xb2350a43
	.4byte 0x3521ccf8
	.4byte 0x45c80c78
	.4byte 0x19ccf490
	.4byte 0x44c62e2a
	.4byte 0x4aac5489
	.4byte 0x2ad56a94
	.4byte 0x41d8658a
	.4byte 0x0547ccd1
	.4byte 0x611c0ab3
	.4byte 0x10c0a336
	.4byte 0x2a05033e
	.4byte 0x500a11c0
	.4byte 0x00a11c84
	.4byte 0x892fc845
	.4byte 0x905596e4
	.4byte 0x483bd970
	.4byte 0xa9e670f8
	.4byte 0xa6ab3176
	.4byte 0x5251886a
	.4byte 0x0eebacbd
	.4byte 0x7cb3bf9f
	.4byte 0xa59f8176
	.4byte 0x52c78d30
	.4byte 0x06a7ca38
	.4byte 0x8f9b3306
	.2byte 0xc682
	tst	r0, r7
	adds	r3, #96
.L_0200487a:
	.2byte 0xf847
	.2byte 0x7104
	asrs	r2, r4, #15
	.2byte 0xb370
	bne.n	.L_020048b8
	.2byte 0xf139
	.2byte 0x9890
	ldr	r0, [pc, #608]
	str	r4, [r1, #68]
	str	r1, [sp, #376]
	ldmia	r5!, {r1, r3, r6}
.L_02004890:
	adds	r5, #42
	asrs	r0, r7, #3
	adds	r2, #205
	ldmia	r5, {r0, r2, r4, r5}
	strb	r4, [r2, #3]
	cmp	r3, #56
	lsls	r2, r6
	lsls	r6, r3, #17
	ldr	r7, [r2, r4]
	ldr	r6, [pc, #960]
	.2byte 0xf524
	.2byte 0x854e
	strh	r2, [r0, #60]
	ldrb	r1, [r2, #20]
	.2byte 0xf4fe
	.2byte 0xd3e2
.L_020048b0:
	ldmia	r4!, {r2, r3}
	add	r0, pc, #224
	ldr	r4, [pc, #172]
	.2byte 0xdb03
.L_020048b8:
	adds	r2, #192
	.2byte 0xb870
	adds	r3, #152
	cmp	r4, #33
.L_020048c0:
	add	r2, sp, #944
	.2byte 0xfc99
	.2byte 0x0a8d
	.2byte 0xb37b
	str	r3, [r0, #124]
	str	r3, [r4, #84]
	ldrh	r6, [r3, #56]
	ldr	r3, [r0, #44]
	stmia	r2!, {r2, r3, r6, r7}
	strh	r0, [r5, r1]
	stmia	r1!, {r0, r3, r5, r7}
	lsls	r3, r0, #19
	movs	r2, #39
	.2byte 0x1d3f
	.2byte 0xf02e
	.2byte 0x1c9a
	lsls	r0, r3
	add	r0, pc, #4
	add	r3, pc, #608
	lsrs	r0, r0, #9
	ldrh	r0, [r1, #16]
	ldrh	r1, [r7, #16]
	lsrs	r7, r1, #26
	strb	r0, [r3, #20]
	lsrs	r0, r4, #13
	lsrs	r6, r4, #15
	strh	r6, [r7, #60]
	ldr	r0, [pc, #880]
	str	r0, [r5, #112]
	ldrh	r7, [r0, #4]
	ldr	r5, [sp, #708]
.L_020048fe:
	b.n	.L_02004752
	.4byte 0x02e81d9d
	.4byte 0xc10ee0a0
	.4byte 0x7687ba82
	.4byte 0x42227088
	.4byte 0xd267118f
	.4byte 0x40899ccc
	.4byte 0x834aacce
	.4byte 0x1be0706f
	.4byte 0x61b62448
	.4byte 0x6571c787
	.4byte 0x05d814c3
	.4byte 0xc4c327c8
	.4byte 0x7c808af9
	.4byte 0x4e5ea933
	.4byte 0xe7d551d5
	.4byte 0x103e350f
	.4byte 0xe5ce7182
	.4byte 0xe7107013
	.4byte 0x0f87be1e
	.4byte 0x127ccbc6
	.4byte 0xf80c066e
	.4byte 0x65cfa21d
	.4byte 0xb264e43a
	.4byte 0x7989e132
	.4byte 0x29e70e8a
	.4byte 0x942246e6
	.4byte 0x622eb888
	.4byte 0x27e0a182
	.4byte 0x827e2e98
	.4byte 0xfc18fa90
	.4byte 0x041687c4
	.4byte 0xfa0421f0
	.4byte 0xd7b0ebc2
	.4byte 0x3f863edd
	.4byte 0xea6f07c0
	.4byte 0xfe7d8873
	.4byte 0xf03fcf81
	.4byte 0x953e07f9
	.4byte 0xf86b8fc0
	.4byte 0x00ff0e38
	.4byte 0xc3bc61fa
	.4byte 0xfe7cf0ff
	.4byte 0x6cbfe050
	.4byte 0x00737848
	.4byte 0xd7f76b76
	.4byte 0xd001ce01
	.4byte 0x1f780049
	.4byte 0x183c1f85
	.4byte 0xfcfb9bc0
	.4byte 0xe07f9f03
	.4byte 0x3e1df763
	.4byte 0x1fe7c0ff
	.4byte 0x9f03fcf8
	.4byte 0x0ff3e07f
	.4byte 0xcf81fe7c
	.4byte 0x04e1f03f
	.4byte 0x0000003e
	.4byte 0x00000002
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06280000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a30000
	.4byte 0x00000000
	.4byte 0x06080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a80000
	.4byte 0x00000000
	.4byte 0x064e0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16e80000
	.4byte 0x00000000
	.4byte 0x06580000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16ec0000
	.4byte 0x00000000
	.4byte 0x06580000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16ec0000
	.4byte 0x00000000
	.4byte 0x06c80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x16180000
	.4byte 0x00000000
	.4byte 0x07080000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16a80000
	.4byte 0x00000000
	.4byte 0x07080000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000002
	.4byte 0x16ec0000
	.4byte 0x00000000
	.4byte 0x06a80000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16f00000
	.4byte 0x00000000
	.4byte 0x05f80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16f80000
	.4byte 0x00000000
	.4byte 0x05480000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000002
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06280000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a30000
	.4byte 0x00000000
	.4byte 0x06080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a80000
	.4byte 0x00000000
	.4byte 0x065e0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16c80000
	.4byte 0x00000000
	.4byte 0x06680000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16e70000
	.4byte 0x00000000
	.4byte 0x06680000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16ec0000
	.4byte 0x00000000
	.4byte 0x06c80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x16080000
	.4byte 0x00000000
	.4byte 0x06e80000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16080000
	.4byte 0x00000000
	.4byte 0x07030000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a80000
	.4byte 0x00000000
	.4byte 0x07080000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000002
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06280000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a30000
	.4byte 0x00000000
	.4byte 0x06080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a80000
	.4byte 0x00000000
	.4byte 0x064e0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16c80000
	.4byte 0x00000000
	.4byte 0x06580000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16e70000
	.4byte 0x00000000
	.4byte 0x06580000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16ec0000
	.4byte 0x00000000
	.4byte 0x06c80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x16180000
	.4byte 0x00000000
	.4byte 0x06e80000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16180000
	.4byte 0x00000000
	.4byte 0x07030000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a80000
	.4byte 0x00000000
	.4byte 0x07080000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000002
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06280000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a30000
	.4byte 0x00000000
	.4byte 0x06080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a80000
	.4byte 0x00000000
	.4byte 0x063e0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16c80000
	.4byte 0x00000000
	.4byte 0x06480000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x16e70000
	.4byte 0x00000000
	.4byte 0x06480000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16ec0000
	.4byte 0x00000000
	.4byte 0x06c80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x16280000
	.4byte 0x00000000
	.4byte 0x06e80000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x16280000
	.4byte 0x00000000
	.4byte 0x07030000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x16a80000
	.4byte 0x00000000
	.4byte 0x07080000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00001999
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00001999
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x15e80000
	.4byte 0x00000000
	.4byte 0x06b80000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000028f
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000028f
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x000000a0
	.4byte 0xc0010000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x17780000
	.4byte 0x00000000
	.4byte 0x0d480000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x17840000
	.4byte 0x00000000
	.4byte 0x0d480000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x1d8c0000
	.4byte 0x00000000
	.4byte 0x0d940000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1d980000
	.4byte 0x00000000
	.4byte 0x0d880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1dd80000
	.4byte 0x00000000
	.4byte 0x0d880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1de80000
	.4byte 0x00000000
	.4byte 0x0d980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1e280000
	.4byte 0x00000000
	.4byte 0x0d980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1e480000
	.4byte 0x00000000
	.4byte 0x0da80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x1d8c0000
	.4byte 0x00000000
	.4byte 0x0d940000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1d980000
	.4byte 0x00000000
	.4byte 0x0d880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1dd80000
	.4byte 0x00000000
	.4byte 0x0d880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1de80000
	.4byte 0x00000000
	.4byte 0x0d980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1e180000
	.4byte 0x00000000
	.4byte 0x0db80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x1d8c0000
	.4byte 0x00000000
	.4byte 0x0d940000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1d980000
	.4byte 0x00000000
	.4byte 0x0d880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1dd80000
	.4byte 0x00000000
	.4byte 0x0d880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1de80000
	.4byte 0x00000000
	.4byte 0x0d980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x1e080000
	.4byte 0x00000000
	.4byte 0x0dc80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x0000001b
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000028
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0xfffe0000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000028
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0xfffe0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0xfffe0000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000004
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x02008251
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x00001618
	.4byte 0x400004e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00001630
	.4byte 0x400004ca
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00001638
	.4byte 0x400004c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x000016d8
	.4byte 0x40000648
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00001918
	.4byte 0x40000528
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000019b8
	.4byte 0x400004c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00001b10
	.4byte 0x80000558
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00001ca0
	.4byte 0x800004d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00001be0
	.4byte 0x8000041e
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x00001728
	.4byte 0x40000128
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00001788
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x00001d90
	.4byte 0x40000588
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x00001ec8
	.4byte 0x40000638
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000d
	.4byte 0x00001eac
	.4byte 0xc0000698
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000e
	.4byte 0x00001d78
	.4byte 0x400007d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000f
	.4byte 0x00001be8
	.4byte 0x000006c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0010
	.4byte 0x00001b60
	.4byte 0x00000658
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0011
	.4byte 0x00001ae8
	.4byte 0x400006c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0012
	.4byte 0x00001ae8
	.4byte 0x00000724
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0013
	.4byte 0x000015b8
	.4byte 0x80000858
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0014
	.4byte 0x000015d8
	.4byte 0x40000878
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0015
	.4byte 0x00001530
	.4byte 0x400008d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0016
	.4byte 0x0000138c
	.4byte 0x80000918
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0017
	.4byte 0x00001328
	.4byte 0x40000908
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0018
	.4byte 0x000012e8
	.4byte 0x400007c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0019
	.4byte 0x0000134c
	.4byte 0xc0000a58
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff001a
	.4byte 0x00001618
	.4byte 0x40000978
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff001b
	.4byte 0x0000150c
	.4byte 0x80000b18
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff001c
	.4byte 0x00001548
	.4byte 0xc0000b68
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff001d
	.4byte 0x000016e8
	.4byte 0x40000d48
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff001e
	.4byte 0x0000176c
	.4byte 0x40000b72
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff001f
	.4byte 0x0000176c
	.4byte 0x40000b18
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0020
	.4byte 0x00001790
	.4byte 0x80000c78
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0021
	.4byte 0x00001758
	.4byte 0x40000d68
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0022
	.4byte 0x00001708
	.4byte 0x40000488
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0023
	.4byte 0x00001766
	.4byte 0x40000324
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0026
	.4byte 0x00001cd0
	.4byte 0x000004d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0027
	.4byte 0x00001744
	.4byte 0x0000010a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0028
	.4byte 0x00001954
	.4byte 0x400004bc
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0029
	.4byte 0x00001568
	.4byte 0x40000a48
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff002a
	.4byte 0x00001728
	.4byte 0x00000cc8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff002b
	.4byte 0x00001e70
	.4byte 0x40000838
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff002c
	.4byte 0x000016d8
	.4byte 0xc0000608
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff002d
	.4byte 0x00001868
	.4byte 0x40000208
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff002e
	.4byte 0x00001d90
	.4byte 0xc0000548
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff002f
	.4byte 0x00001b40
	.4byte 0x00000550
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0030
	.4byte 0x00001854
	.4byte 0x40000228
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0031
	.4byte 0x000016e4
	.4byte 0x400004bc
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0032
	.4byte 0x00001b90
	.4byte 0xc0000678
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0033
	.4byte 0x00001b58
	.4byte 0x800006c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0034
	.4byte 0x00001b08
	.4byte 0x400006a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0035
	.4byte 0x000017ca
	.4byte 0x400008b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0036
	.4byte 0x0000134c
	.4byte 0x40000aa8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0037
	.4byte 0x00001528
	.4byte 0x00000cc8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0038
	.4byte 0x00001528
	.4byte 0xc0000af8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0039
	.4byte 0x00001544
	.4byte 0x00000b18
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff003a
	.4byte 0x00001530
	.4byte 0xc0000898
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff003b
	.4byte 0x0000176c
	.4byte 0xc0000b28
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff003c
	.4byte 0x00001568
	.4byte 0x40000848
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff003d
	.4byte 0x00001808
	.4byte 0x00000c8c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0040
	.4byte 0x000016d8
	.4byte 0x40000648
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0041
	.4byte 0x00001508
	.4byte 0x400008c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0042
	.4byte 0x00001f08
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0043
	.4byte 0x00001f08
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0044
	.4byte 0x00001f08
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0045
	.4byte 0x00001f08
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffff0080
	.4byte 0x0000ffff
	.4byte 0xffff0046
	.4byte 0x000013e8
	.4byte 0x40000918
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0047
	.4byte 0x00001598
	.4byte 0x40000848
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0048
	.4byte 0x00001d28
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0049
	.4byte 0x000017c8
	.4byte 0x80000c68
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff004a
	.4byte 0x00001278
	.4byte 0x800001a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff004b
	.4byte 0x00001308
	.4byte 0x80000328
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff004c
	.4byte 0x000011e0
	.4byte 0x400001fc
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff004d
	.4byte 0x00001128
	.4byte 0x40000148
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff004e
	.4byte 0x00000008
	.4byte 0x40000008
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0050
	.4byte 0x00001d80
	.4byte 0x40000db0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff005a
	.4byte 0x00001788
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff005b
	.4byte 0x000017c8
	.4byte 0x40000c60
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0061
	.4byte 0x00001868
	.4byte 0x40000368
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0062
	.4byte 0x00001098
	.4byte 0x40000978
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0063
	.4byte 0x000015c8
	.4byte 0x40000828
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0010a005
	.4byte 0x00202002
	.4byte 0x00301014
	.4byte 0x00401019
	.4byte 0x0050101e
	.4byte 0x00601023
	.4byte 0x00701024
	.4byte 0x00804028
	.4byte 0x00901032
	.4byte 0x00a0b039
	.4byte 0x00b01027
	.4byte 0x00c0103c
	.4byte 0x00d01044
	.4byte 0x00e01048
	.4byte 0x00f0104a
	.4byte 0x0100104b
	.4byte 0x01101058
	.4byte 0x01201059
	.4byte 0x01313002
	.4byte 0x01409063
	.4byte 0x0150106b
	.4byte 0x01602070
	.4byte 0x01701087
	.4byte 0x01801092
	.4byte 0x01901098
	.4byte 0x01a0109e
	.4byte 0x01b0a0a7
	.4byte 0x01c010a4
	.4byte 0x01d010a9
	.4byte 0x01e140b2
	.4byte 0x01f010b1
	.4byte 0x020010b5
	.4byte 0x021040ab
	.4byte 0x02201068
	.4byte 0x02301031
	.4byte 0x0280201d
	.4byte 0x0290609e
	.4byte 0x02a050a9
	.4byte 0x02b0d046
	.4byte 0x02c03014
	.4byte 0x02d01031
	.4byte 0x02e02027
	.4byte 0x02f02023
	.4byte 0x0300302f
	.4byte 0x0310106a
	.4byte 0x0320204a
	.4byte 0x0330304a
	.4byte 0x03404057
	.4byte 0x0350405c
	.4byte 0x03602098
	.4byte 0x037020a5
	.4byte 0x0380406b
	.4byte 0x03b150b2
	.4byte 0x03c02099
	.4byte 0x03d030b5
	.4byte 0x04a0606d
	.4byte 0x04b01071
	.4byte 0x06401002
	.4byte 0x06540002
	.4byte 0x0660b06e
	.4byte 0x0670c06e
	.4byte 0x0680d06e
	.4byte 0x0691506f
	.4byte 0x06a0e06e
	.4byte 0x06b1206d
	.4byte 0x06c0a099
	.4byte 0x06d040bb
	.4byte 0x06e1706f
	.4byte 0x06f50002
	.4byte 0x0700a069
	.4byte 0x000001ff
	.4byte 0x00000022
	.4byte 0x020082a5
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x020082cd
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x020082f1
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x02008315
	.4byte 0x00000022
	.4byte 0x02008351
	.4byte 0x00000010
	.4byte 0x0033005a
	.4byte 0x0200d270
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x005a005c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0048005b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0105
	.4byte 0x0200db58
	.4byte 0x162e0000
	.4byte 0x00000000
	.4byte 0x04b00000
	.4byte 0x00024000
	.4byte 0xffff0105
	.4byte 0x0200db58
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06280000
	.4byte 0x00024000
	.4byte 0xffff0105
	.4byte 0x0200db58
	.4byte 0x1b400000
	.4byte 0x00000000
	.4byte 0x06580000
	.4byte 0x00024000
	.4byte 0xffff0105
	.4byte 0x0200db58
	.4byte 0x15280000
	.4byte 0x00000000
	.4byte 0x0b180000
	.4byte 0x00024000
	.4byte 0xffff0105
	.4byte 0x0200db58
	.4byte 0x17040000
	.4byte 0x00000000
	.4byte 0x04680000
	.4byte 0x00024000
	.4byte 0xffff0115
	.4byte 0x0200db58
	.4byte 0x152e0000
	.4byte 0x00000000
	.4byte 0x08b80000
	.4byte 0x00020000
	.4byte 0xffff0115
	.4byte 0x0200db58
	.4byte 0x13ac0000
	.4byte 0x00000000
	.4byte 0x09180000
	.4byte 0x00028000
	.4byte 0xffff0109
	.4byte 0x0200db58
	.4byte 0x17280000
	.4byte 0x00000000
	.4byte 0x010e0000
	.4byte 0x00024000
	.4byte 0xffff0108
	.4byte 0x0200db58
	.4byte 0x1cb80000
	.4byte 0x00000000
	.4byte 0x04d80000
	.4byte 0x00024000
	.4byte 0xffff010a
	.4byte 0x0200db58
	.4byte 0x1d780000
	.4byte 0x00000000
	.4byte 0x07b80000
	.4byte 0x00024000
	.4byte 0xffff010b
	.4byte 0x0200db58
	.4byte 0x1ae80000
	.4byte 0x00000000
	.4byte 0x06b00000
	.4byte 0x00024000
	.4byte 0xffff010b
	.4byte 0x0200db58
	.4byte 0x1ec80000
	.4byte 0x00000000
	.4byte 0x06180000
	.4byte 0x00024000
	.4byte 0xffff0107
	.4byte 0x0200db58
	.4byte 0x19b80000
	.4byte 0x00000000
	.4byte 0x04a80000
	.4byte 0x00024000
	.4byte 0xffff0107
	.4byte 0x0200db58
	.4byte 0x15d80000
	.4byte 0x00000000
	.4byte 0x08580000
	.4byte 0x00024000
	.4byte 0xffff0107
	.4byte 0x0200db58
	.4byte 0x176c0000
	.4byte 0x00000000
	.4byte 0x0b4e0000
	.4byte 0x00024000
	.4byte 0xffff0107
	.4byte 0x0200db58
	.4byte 0x13280000
	.4byte 0x00000000
	.4byte 0x08e80000
	.4byte 0x00024000
	.4byte 0xffff010d
	.4byte 0x0200db58
	.4byte 0x17880000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff010f
	.4byte 0x0200db58
	.4byte 0x16180000
	.4byte 0x00000000
	.4byte 0x04580000
	.4byte 0x00024000
	.4byte 0xffff0114
	.4byte 0x0200db58
	.4byte 0x17c80000
	.4byte 0x00000000
	.4byte 0x0c680000
	.4byte 0x00024000
	.4byte 0xffff010e
	.4byte 0x0200db58
	.4byte 0x1b280000
	.4byte 0x00000000
	.4byte 0x05520000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x0200db58
	.4byte 0x176c0000
	.4byte 0x00000000
	.4byte 0x0af80000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte 0x0200db58
	.4byte 0x19180000
	.4byte 0x00000000
	.4byte 0x05160000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte 0x0200db58
	.4byte 0x19540000
	.4byte 0x00000000
	.4byte 0x04a00000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte 0x0200db58
	.4byte 0x12e80000
	.4byte 0x00000000
	.4byte 0x07a80000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte 0x0200db58
	.4byte 0x16180000
	.4byte 0x00000000
	.4byte 0x09580000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte 0x0200db58
	.4byte 0x15680000
	.4byte 0x00000000
	.4byte 0x0a280000
	.4byte 0x00024000
	.4byte 0xffff0113
	.4byte 0x0200db58
	.4byte 0x1e6e0000
	.4byte 0x00000000
	.4byte 0x08200000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte 0x0200db58
	.4byte 0x17580000
	.4byte 0x00000000
	.4byte 0x0d480000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte 0x0200db58
	.4byte 0x17640000
	.4byte 0x00000000
	.4byte 0x03060000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte 0x0200db58
	.4byte 0x18500000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte 0x0200db58
	.4byte 0x16e00000
	.4byte 0x00000000
	.4byte 0x04960000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte 0x0200db58
	.4byte 0x1b080000
	.4byte 0x00000000
	.4byte 0x06800000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte 0x0200db58
	.4byte 0x17cc0000
	.4byte 0x00000000
	.4byte 0x08980000
	.4byte 0x00024000
	.4byte 0xffff0113
	.4byte 0x0200db58
	.4byte 0x1bfe0000
	.4byte 0x00000000
	.4byte 0x041e0000
	.4byte 0x00024000
	.4byte 0xffff0113
	.4byte 0x0200db58
	.4byte 0x1eb00000
	.4byte 0x00000000
	.4byte 0x06c80000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte 0x0200db58
	.4byte 0x15680000
	.4byte 0x00000000
	.4byte 0x08280000
	.4byte 0x00024000
	.4byte 0xffff0112
	.4byte 0x0200db58
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0112
	.4byte 0x0200db58
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0112
	.4byte 0x0200db58
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0116
	.4byte 0x0200db4c
	.4byte 0x1d900000
	.4byte 0x00000000
	.4byte 0x05680000
	.4byte 0x00024000
	.4byte 0xffff0117
	.4byte 0x0200db58
	.4byte 0x134e0000
	.4byte 0x00000000
	.4byte 0x0a820000
	.4byte 0x00024000
	.4byte 0xffff01f5
	.4byte 0x0200db58
	.4byte 0x17940000
	.4byte 0x00000000
	.4byte 0x0d820000
	.4byte 0x00028000
	.4byte 0x18a000a1
	.4byte 0x0200db58
	.4byte 0x12980000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0002c000
	.4byte 0x02f10121
	.4byte 0x0200db70
	.4byte 0x11280000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0039
	.4byte 0x0200db64
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x0200db58
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
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0031
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff009a
	.4byte 0x0200db58
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff009a
	.4byte 0x0200db58
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff009a
	.4byte 0x0200db58
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff009a
	.4byte 0x0200db58
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0105
	.4byte 0x0200db58
	.4byte 0x16d80000
	.4byte 0x00000000
	.4byte 0x06280000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff009a
	.4byte 0x0200db58
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0105
	.4byte 0x0200db58
	.4byte 0x17040000
	.4byte 0x00000000
	.4byte 0x04680000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte 0x0200db58
	.4byte 0x16e00000
	.4byte 0x00000000
	.4byte 0x04960000
	.4byte 0x00024000
	.4byte 0xffff0044
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0031
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
	.4byte 0xffff00a1
	.4byte 0x0200db58
	.4byte 0x15080000
	.4byte 0x00000000
	.4byte 0x08c80000
	.4byte 0x00028000
	.4byte 0xffff0107
	.4byte 0x0200db58
	.4byte 0x15d80000
	.4byte 0x00000000
	.4byte 0x08580000
	.4byte 0x00024000
	.4byte 0xffff0115
	.4byte 0x0200db58
	.4byte 0x152e0000
	.4byte 0x00000000
	.4byte 0x08b80000
	.4byte 0x00020000
	.4byte 0xffff0115
	.4byte 0x0200db58
	.4byte 0x13a40000
	.4byte 0x00000000
	.4byte 0x09180000
	.4byte 0x01028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00a1
	.4byte 0x0200db58
	.4byte 0x15380000
	.4byte 0x00000000
	.4byte 0x09080000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0107
	.4byte 0x0200db58
	.4byte 0x15d80000
	.4byte 0x00000000
	.4byte 0x08580000
	.4byte 0x00024000
	.4byte 0xffff010c
	.4byte 0x0200db58
	.4byte 0x15680000
	.4byte 0x00000000
	.4byte 0x08280000
	.4byte 0x00024000
	.4byte 0xffff009a
	.4byte 0x0200db58
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0005
	.4byte 0x0200db58
	.4byte 0x1d880000
	.4byte 0x00000000
	.4byte 0x0db80000
	.4byte 0x00015000
	.4byte 0xffff0006
	.4byte 0x0200db58
	.4byte 0x1e880000
	.4byte 0x00000000
	.4byte 0x0dc80000
	.4byte 0x00010000
	.4byte 0xffff001e
	.4byte 0x0200db58
	.4byte 0x1d780000
	.4byte 0x00000000
	.4byte 0x0da80000
	.4byte 0x0001d000
	.4byte 0xffff002b
	.4byte 0x0200db58
	.4byte 0x1e1c0000
	.4byte 0x00000000
	.4byte 0x0d800000
	.4byte 0x00015000
	.4byte 0xffff0023
	.4byte 0x0200db58
	.4byte 0x1e780000
	.4byte 0x00000000
	.4byte 0x0dd80000
	.4byte 0x0001b000
	.4byte 0xffff01f5
	.4byte 0x0200db58
	.4byte 0x1db40000
	.4byte 0x00000000
	.4byte 0x0dc20000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff01f8
	.4byte 0x0200db58
	.4byte 0x1d280000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0114
	.4byte 0x0200db58
	.4byte 0x17c00000
	.4byte 0x00000000
	.4byte 0x0c600000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0030005a
	.4byte 0x02009ca5
	.4byte 0x00000002
	.4byte 0x02f1005f
	.4byte 0x0200b7bd
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008031
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0200808d
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020080d5
	.4byte 0x00000002
	.4byte 0x0033005c
	.4byte 0x02008031
	.4byte 0x00000002
	.4byte 0x005a005b
	.4byte 0x0200808d
	.4byte 0x00000002
	.4byte 0x0048005d
	.4byte 0x020080d5
	.4byte 0x00000003
	.4byte 0xffff0060
	.4byte 0x0200a8a9
	.4byte 0x0000f204
	.4byte 0x085d0060
	.4byte 0x0200a8e9
	.4byte 0x00000053
	.4byte 0x0fcf0064
	.4byte 0x00100103
	.4byte 0x00000000
	.4byte 0xffff0037
	.4byte 0x0200a81d
	.4byte 0x00008d15
	.4byte 0xffff0437
	.4byte 0x0200a81d
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0032
	.4byte 0x00000032
	.4byte 0x00000001
	.4byte 0xffff0033
	.4byte 0x00000033
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000001
	.4byte 0xffff001c
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0xffff0037
	.4byte 0x00000037
	.4byte 0x00000001
	.4byte 0xffff001d
	.4byte 0x0000001d
	.4byte 0x00000001
	.4byte 0xffff002a
	.4byte 0x0000002a
	.4byte 0x00000001
	.4byte 0xffff0020
	.4byte 0x00000020
	.4byte 0x00000001
	.4byte 0xffff003d
	.4byte 0x0000003d
	.4byte 0x00000001
	.4byte 0xffff002f
	.4byte 0x0000002f
	.4byte 0x00000001
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0xffff0070
	.4byte 0x020086b5
	.4byte 0x00000001
	.4byte 0xffff0084
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0085
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0xffff007b
	.4byte 0x00000005
	.4byte 0x00000002
	.4byte 0xffff0082
	.4byte 0x02008541
	.4byte 0x00000001
	.4byte 0xffff0077
	.4byte 0x00000007
	.4byte 0x00000002
	.4byte 0xffff0096
	.4byte 0x02008551
	.4byte 0x00000001
	.4byte 0xffff0090
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff008b
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0xffff008c
	.4byte 0x00000030
	.4byte 0x00000001
	.4byte 0xffff0076
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff007f
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff007a
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff0091
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff0089
	.4byte 0x0000002b
	.4byte 0x00000001
	.4byte 0xffff0078
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff0071
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff008e
	.4byte 0x00000034
	.4byte 0x00000001
	.4byte 0xffff0079
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff008f
	.4byte 0x00000035
	.4byte 0x00000001
	.4byte 0xffff007c
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff0073
	.4byte 0x00000022
	.4byte 0x00000001
	.4byte 0xffff008d
	.4byte 0x00000031
	.4byte 0x00000002
	.4byte 0xffff0074
	.4byte 0x02008561
	.4byte 0x00000001
	.4byte 0xffff0075
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0xffff007e
	.4byte 0x00000017
	.4byte 0x00000001
	.4byte 0xffff0086
	.4byte 0x00000018
	.4byte 0x00000002
	.4byte 0xffff0097
	.4byte 0x02008571
	.4byte 0x00000001
	.4byte 0xffff0092
	.4byte 0x0000003c
	.4byte 0x00000001
	.4byte 0xffff0087
	.4byte 0x0000001a
	.4byte 0x00000001
	.4byte 0xffff0088
	.4byte 0x00000029
	.4byte 0x00000001
	.4byte 0xffff0072
	.4byte 0x0000001b
	.4byte 0x00000001
	.4byte 0xffff008a
	.4byte 0x00000021
	.4byte 0x00000001
	.4byte 0xffff0083
	.4byte 0x0000001f
	.4byte 0x00000002
	.4byte 0xffff007d
	.4byte 0x02008581
	.4byte 0x00000602
	.4byte 0x18a0004a
	.4byte 0x020086dd
	.4byte 0x0000c401
	.4byte 0xffff004b
	.4byte 0x0000004b
	.4byte 0x10002115
	.4byte 0x12f00036
	.4byte 0x0200b7c9
	.4byte 0x00002115
	.4byte 0x12f00036
	.4byte 0x0200b7d9
	.4byte 0x00000006
	.4byte 0xffff0063
	.4byte 0x0200b84d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000007
	.4byte 0x00000005
	.4byte 0x00000008
	.4byte 0x00000004
	.4byte 0x00000009
	.4byte 0x0000000c
	.4byte 0x00000006
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000005
	.4byte 0xffffe667
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000e00
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffe80
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000030
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
