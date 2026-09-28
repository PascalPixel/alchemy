.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_HEYA/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {r5, lr}
	adds r5, r0, #0
	ldr r1, [r5, #104]
	cmp r1, #0
	beq .L_02000030_0
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	ldr r0, [r1, #16]
	ldr r3, [r5, #16]
	ldr r1, [r1, #8]
	subs r0, r0, r3
	ldr r3, [r5, #8]
	subs r1, r1, r3
	bl 0x02009164
	ldrh r3, [r5, #6]
	lsls r0, r0, #16
	lsrs r0, r0, #16
	subs r0, r0, r3
	lsls r0, r0, #16
	asrs r0, r0, #16
	cmp r0, #0
	beq .L_02000030_0
	movs r2, #128
	lsls r2, r2, #5
	cmp r0, r2
	ble .L_02000030_1
	adds r0, r2, #0
.L_02000030_1:
	ldr r2, [pc, #20]
	cmp r0, r2
	bge .L_02000030_2
	adds r0, r2, #0
.L_02000030_2:
	adds r3, r3, r0
	strh r3, [r5, #6]
.L_02000030_0:
	movs r0, #1
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0xfffff000
	.global Func_02000088
	.thumb_func
Func_02000088:
	adds r1, r0, #0
	adds r1, #100
	movs r3, #0
	ldrsh r2, [r1, r3]
	ldr r3, [r0, #8]
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r0, #8]
	movs r4, #102
	adds r4, r4, r0
	movs r3, #0
	ldrsh r2, [r4, r3]
	ldr r3, [r0, #12]
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r2, [pc, #28]
	ldr r3, [r0, #24]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	ldrh r3, [r1]
	adds r3, #5
	strh r3, [r1]
	ldrh r3, [r4]
	subs r3, #1
	strh r3, [r4]
	movs r0, #0
	bx lr
	.2byte 0x0000
	.4byte 0x00000666
	.global Func_020000cc
	.thumb_func
Func_020000cc:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009478
	.global Func_020000d4
	.thumb_func
Func_020000d4:
	movs r0, #0
	bx lr
	.global Func_020000d8
	.thumb_func
Func_020000d8:
	push {lr}
	ldr r0, [pc, #20]
	bl 0x02009184
	cmp r0, #0
	beq .L_020000d8_0
	ldr r0, [pc, #12]
	b .L_020000d8_1
.L_020000d8_0:
	ldr r0, [pc, #12]
.L_020000d8_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00000834
	.4byte 0x02009590
	.4byte 0x02009568
	.global Func_020000fc
	.thumb_func
Func_020000fc:
	push {r5, lr}
	ldr r0, [pc, #44]
	bl 0x02009184
	cmp r0, #0
	beq .L_020000fc_0
	ldr r5, [pc, #36]
	b .L_020000fc_1
.L_020000fc_0:
	ldr r0, [pc, #36]
	bl 0x02009184
	cmp r0, #0
	beq .L_020000fc_2
	ldr r5, [pc, #32]
	b .L_020000fc_1
.L_020000fc_2:
	ldr r5, [pc, #32]
.L_020000fc_1:
	adds r0, r5, #0
	bl 0x020091ac
	adds r0, r5, #0
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0000087a
	.4byte 0x020098b8
	.4byte 0x00000815
	.4byte 0x02009738
	.4byte 0x020095b8
	.global Func_02000140
	.thumb_func
Func_02000140:
	push {lr}
	ldr r0, [pc, #48]
	bl 0x02009184
	cmp r0, #0
	beq .L_02000140_0
	ldr r0, [pc, #40]
	b .L_02000140_1
.L_02000140_0:
	ldr r0, [pc, #40]
	bl 0x02009184
	cmp r0, #0
	beq .L_02000140_2
	ldr r0, [pc, #36]
	b .L_02000140_1
.L_02000140_2:
	ldr r0, [pc, #36]
	bl 0x02009184
	cmp r0, #0
	beq .L_02000140_3
	ldr r0, [pc, #28]
	b .L_02000140_1
.L_02000140_3:
	ldr r0, [pc, #28]
.L_02000140_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00000834
	.4byte 0x02009ac8
	.4byte 0x0000087a
	.4byte 0x02009ffc
	.4byte 0x00000815
	.4byte 0x02009da4
	.4byte 0x02009c00
	.global Func_02000190
	.thumb_func
Func_02000190:
	push {lr}
	bl 0x0200919c
	ldr r0, [pc, #32]
	bl 0x02009244
	movs r2, #6
	movs r0, #0
	movs r1, #15
	bl 0x02009234
	movs r1, #0
	movs r0, #15
	bl 0x02009264
	bl 0x020091a4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000f6d
	.global Func_020001bc
	.thumb_func
Func_020001bc:
	push {lr}
	bl 0x0200919c
	ldr r0, [pc, #32]
	bl 0x02009244
	movs r2, #6
	movs r0, #0
	movs r1, #19
	bl 0x02009234
	movs r1, #0
	movs r0, #19
	bl 0x02009264
	bl 0x020091a4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000f73
	.global Func_020001e8
	.thumb_func
Func_020001e8:
	push {r5, lr}
	bl 0x0200919c
	ldr r0, [pc, #88]
	bl 0x02009184
	cmp r0, #0
	beq .L_020001e8_0
	ldr r0, [pc, #80]
	bl 0x02009244
	movs r0, #20
	movs r1, #0
	bl 0x02009254
	movs r1, #128
	ldr r2, [pc, #68]
	movs r0, #20
	lsls r1, r1, #9
	bl 0x0200923c
	b .L_020001e8_1
.L_020001e8_0:
	ldr r5, [pc, #60]
	adds r0, r5, #0
	bl 0x02009244
	adds r5, #1
	movs r2, #20
	movs r0, #20
	movs r1, #0
	bl 0x0200925c
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200917c
	movs r0, #180
	movs r1, #0
	bl 0x020091b4
	ldr r0, [pc, #12]
	bl 0x0200918c
.L_020001e8_1:
	bl 0x020091a4
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0000081b
	.4byte 0x000011a6
	.4byte 0x020092fc
	.4byte 0x000011a4
	.global Func_02000258
	.thumb_func
Func_02000258:
	push {lr}
	bl 0x0200919c
	ldr r0, [pc, #20]
	bl 0x02009244
	movs r1, #0
	movs r0, #16
	bl 0x02009264
	bl 0x020091a4
	pop {r0}
	bx r0
	.4byte 0x000011be
	.global Func_02000278
	.thumb_func
Func_02000278:
	push {lr}
	bl 0x0200919c
	ldr r0, [pc, #20]
	bl 0x02009244
	movs r1, #0
	movs r0, #10
	bl 0x02009264
	bl 0x020091a4
	pop {r0}
	bx r0
	.4byte 0x00001c3d
	.global Func_02000298
	.thumb_func
Func_02000298:
	push {lr}
	bl 0x0200919c
	ldr r0, [pc, #20]
	bl 0x02009244
	ldr r0, [pc, #16]
	movs r1, #0
	bl 0x02009254
	bl 0x020091a4
	pop {r0}
	bx r0
	.4byte 0x00001c40
	.4byte 0x0000800b
	.global Func_020002bc
	.thumb_func
Func_020002bc:
	push {r5, lr}
	adds r5, r0, #0
	ldr r0, [pc, #60]
	bl 0x02009184
	cmp r0, #0
	beq .L_020002bc_0
	bl 0x020092bc
.L_020002bc_0:
	movs r0, #123
	bl 0x020092f4
	ldr r3, [pc, #44]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #73
	str r3, [r2]
	subs r3, #65
	adds r2, r1, r3
	movs r3, #16
	str r3, [r2]
	bl 0x020092dc
	bl 0x020092e4
	adds r0, r5, #0
	bl 0x020092ac
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000834
	.4byte 0x03001ebc
	.global Func_02000308
	.thumb_func
Func_02000308:
	push {lr}
	movs r0, #1
	bl 0x020082bc
	pop {r0}
	bx r0
	.global Func_02000314
	.thumb_func
Func_02000314:
	push {lr}
	movs r0, #2
	bl 0x020082bc
	pop {r0}
	bx r0
	.global Func_02000320
	.thumb_func
Func_02000320:
	push {lr}
	movs r0, #3
	bl 0x020082bc
	pop {r0}
	bx r0
	.global Func_0200032c
	.thumb_func
Func_0200032c:
	push {lr}
	movs r0, #4
	bl 0x020082bc
	pop {r0}
	bx r0
	.global Func_02000338
	.thumb_func
Func_02000338:
	push {lr}
	movs r0, #5
	bl 0x020082bc
	pop {r0}
	bx r0
	.global Func_02000344
	.thumb_func
Func_02000344:
	push {lr}
	movs r0, #6
	bl 0x020082bc
	pop {r0}
	bx r0
	.global Func_02000350
	.thumb_func
Func_02000350:
	push {lr}
	movs r0, #7
	bl 0x020082bc
	pop {r0}
	bx r0
	.global Func_0200035c
	.thumb_func
Func_0200035c:
	push {lr}
	movs r0, #8
	bl 0x020082bc
	pop {r0}
	bx r0
	.global Func_02000368
	.thumb_func
Func_02000368:
	push {r5, lr}
	ldr r5, [pc, #340]
	movs r2, #224
	ldr r3, [r5]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #73
	str r2, [r3]
	ldr r0, [pc, #328]
	bl 0x02009184
	cmp r0, #0
	beq .L_02000368_0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x02009204
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x02009204
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x02009204
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x02009204
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x02009204
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x02009204
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl 0x02009204
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl 0x02009204
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl 0x02009204
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl 0x02009204
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl 0x02009204
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl 0x02009204
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x02009204
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl 0x02009204
	movs r1, #0
	movs r0, #22
	movs r2, #0
	bl 0x02009204
	bl 0x020092b4
	ldr r3, [r5, #12]
	ldr r2, [pc, #168]
	adds r3, r3, r2
	movs r2, #1
	strh r2, [r3]
	bl 0x020092c4
	movs r0, #30
	bl 0x0200914c
	bl 0x020092d4
	bl 0x020092e4
	bl 0x020092cc
.L_02000368_0:
	ldr r0, [pc, #140]
	bl 0x02009184
	cmp r0, #0
	beq .L_02000368_1
	ldr r3, [pc, #136]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #6
	bne .L_02000368_2
	ldr r0, [pc, #124]
	bl 0x02009184
	cmp r0, #0
	bne .L_02000368_2
	bl 0x02008658
.L_02000368_2:
	movs r0, #10
	bl 0x020091c4
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
.L_02000368_1:
	ldr r3, [pc, #88]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_02000368_3
	ldr r0, [pc, #80]
	bl 0x02009184
	cmp r0, #0
	beq .L_02000368_3
	movs r1, #227
	movs r2, #150
	lsls r2, r2, #16
	lsls r1, r1, #17
	movs r0, #13
	bl 0x02009204
	movs r0, #13
	bl 0x020091c4
	movs r1, #0
	bl 0x02009174
	movs r0, #13
	movs r1, #5
	bl 0x0200920c
	movs r0, #4
	bl 0x0200916c
.L_02000368_3:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000834
	.4byte 0x00001f84
	.4byte 0x0000087a
	.4byte 0x02000240
	.4byte 0x0000081d
	.4byte 0x00000815
	.global Func_020004dc
	.thumb_func
Func_020004dc:
	push {lr}
	movs r0, #0
	bl 0x020091c4
	ldr r2, [pc, #88]
	ldrh r3, [r0, #6]
	adds r3, r3, r2
	ldr r2, [pc, #88]
	cmp r3, r2
	bhi .L_020004dc_0
	movs r0, #1
	movs r1, #21
	bl 0x020092ec
	b .L_020004dc_1
.L_020004dc_0:
	bl 0x0200919c
	ldr r0, [pc, #72]
	bl 0x02009184
	cmp r0, #0
	beq .L_020004dc_2
	ldr r0, [pc, #64]
	bl 0x02009244
	movs r0, #21
	movs r1, #0
	bl 0x02009264
	b .L_020004dc_3
.L_020004dc_2:
	ldr r0, [pc, #52]
	bl 0x02009184
	cmp r0, #0
	beq .L_020004dc_4
	ldr r0, [pc, #48]
	bl 0x02009244
	b .L_020004dc_5
.L_020004dc_4:
	ldr r0, [pc, #44]
	bl 0x02009244
.L_020004dc_5:
	movs r0, #21
	movs r1, #0
	bl 0x02009254
.L_020004dc_3:
	bl 0x020091a4
.L_020004dc_1:
	pop {r0}
	bx r0
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x0000087a
	.4byte 0x00001c06
	.4byte 0x00000815
	.4byte 0x000011a2
	.4byte 0x00000f53
	.global Func_0200055c
	.thumb_func
Func_0200055c:
	push {lr}
	movs r0, #0
	bl 0x020091c4
	ldr r2, [pc, #80]
	ldrh r3, [r0, #6]
	adds r3, r3, r2
	ldr r2, [pc, #80]
	cmp r3, r2
	bhi .L_0200055c_0
	movs r0, #2
	movs r1, #22
	bl 0x020092ec
	b .L_0200055c_1
.L_0200055c_0:
	bl 0x0200919c
	ldr r0, [pc, #64]
	bl 0x02009184
	cmp r0, #0
	beq .L_0200055c_2
	ldr r0, [pc, #56]
	bl 0x02009244
	b .L_0200055c_3
.L_0200055c_2:
	ldr r0, [pc, #52]
	bl 0x02009184
	cmp r0, #0
	beq .L_0200055c_4
	ldr r0, [pc, #48]
	bl 0x02009244
	b .L_0200055c_3
.L_0200055c_4:
	ldr r0, [pc, #44]
	bl 0x02009244
.L_0200055c_3:
	movs r0, #22
	movs r1, #0
	bl 0x02009254
	bl 0x020091a4
.L_0200055c_1:
	pop {r0}
	bx r0
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x0000087a
	.4byte 0x00001c09
	.4byte 0x00000815
	.4byte 0x000011a3
	.4byte 0x00000f54
	.global Func_020005d4
	.thumb_func
Func_020005d4:
	push {lr}
.L_020005d6:
	movs r0, #0
	bl 0x020091c4
	ldr r2, [pc, #96]
	ldrh r3, [r0, #6]
	adds r3, r3, r2
.L_020005e2:
	ldr r2, [pc, #96]
	cmp r3, r2
	bhi .L_020005e2_0
	movs r0, #3
	movs r1, #20
	bl 0x020092ec
	b 0x0200863a
.L_020005e2_0:
	ldr r0, [pc, #84]
	bl 0x02009184
	cmp r0, #0
.L_020005fa:
	beq 0x02008614
	bl 0x0200919c
	ldr r0, [pc, #72]
	bl 0x02009244
.L_02000606:
	movs r0, #20
	movs r1, #0
	bl 0x02009254
	bl 0x020091a4
.L_02000612:
	b 0x0200863a
	.2byte 0x480e
	.2byte 0xf000
	.2byte 0xfdb5
	.2byte 0x2800
	.2byte 0xd002
.L_0200061e:
	bl 0x020081e8
	b .L_0200061e_0
	.2byte 0xf000
	.2byte 0xfdba
	.2byte 0x480a
	.2byte 0xf000
	.2byte 0xfe0b
	.2byte 0x2014
	.2byte 0x2100
	.2byte 0xf000
	.2byte 0xfe0f
	.2byte 0xf000
	.2byte 0xfdb5
.L_0200061e_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x5fff
	.2byte 0xffff
	.2byte 0x3ffe
	.2byte 0x0000
	.2byte 0x087a
	.2byte 0x0000
	.2byte 0x1c0a
	.2byte 0x0000
	.2byte 0x0815
	.2byte 0x0000
	.2byte 0x0f55
	.2byte 0x0000
	.global Func_02000658
	.thumb_func
Func_02000658:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #28
	bl 0x0200919c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r2, r2
	negs r1, r1
	negs r0, r0
	bl 0x02009294
	movs r0, #1
	bl 0x0200914c
	movs r0, #3
	movs r1, #1
	bl 0x0200927c
	movs r0, #0
	ldr r1, [pc, #1004]
	ldr r2, [pc, #1004]
	bl 0x020091cc
	movs r0, #1
	ldr r1, [pc, #992]
	ldr r2, [pc, #996]
	bl 0x020091cc
	movs r0, #2
	ldr r1, [pc, #984]
	ldr r2, [pc, #984]
	bl 0x020091cc
	ldr r2, [pc, #980]
	movs r0, #3
	ldr r1, [pc, #972]
	bl 0x020091cc
	movs r0, #8
	movs r1, #5
	bl 0x0200920c
	movs r1, #202
	movs r2, #254
	lsls r2, r2, #1
	lsls r1, r1, #2
	movs r0, #0
	bl 0x020091ec
	movs r0, #23
	bl 0x020091c4
	movs r1, #0
	bl 0x02009174
	movs r0, #24
	bl 0x020091c4
	movs r1, #0
	bl 0x02009174
	movs r0, #25
	bl 0x020091c4
	movs r1, #0
	bl 0x02009174
	movs r0, #23
	bl 0x020091c4
	movs r6, #0
	adds r0, #85
	strb r6, [r0]
	movs r0, #24
	bl 0x020091c4
	adds r0, #85
	strb r6, [r0]
	movs r0, #25
	bl 0x020091c4
	ldr r7, [pc, #888]
	adds r0, #85
	movs r1, #200
	lsls r1, r1, #4
	strb r6, [r0]
	adds r0, r7, #0
	bl 0x02009154
	movs r0, #1
	bl 0x0200914c
	ldr r2, [pc, #868]
	ldr r3, [r2]
	mov r8, r2
	movs r2, #228
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #32
	str r2, [r3]
	bl 0x020092d4
	bl 0x020092e4
	movs r0, #0
	bl 0x020091fc
	movs r0, #0
	movs r1, #1
	bl 0x0200920c
	movs r0, #0
	bl 0x020091c4
	cmp r0, #0
	beq .L_02000658_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x02009204
.L_02000658_0:
	movs r0, #0
	bl 0x020091c4
	cmp r0, #0
	beq .L_02000658_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x02009204
.L_02000658_1:
	movs r0, #0
	bl 0x020091c4
	cmp r0, #0
	beq .L_02000658_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x02009204
.L_02000658_2:
	movs r1, #198
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x020091ec
	movs r1, #206
	movs r2, #252
	movs r0, #2
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x020091ec
	movs r2, #131
	lsls r2, r2, #2
	movs r0, #3
	ldr r1, [pc, #740]
	bl 0x020091f4
	movs r0, #1
	movs r1, #1
	bl 0x0200920c
	movs r1, #1
	movs r0, #2
	bl 0x0200920c
	movs r0, #10
	bl 0x02009194
	ldr r5, [pc, #716]
	movs r0, #0
	ldr r1, [pc, #716]
	adds r2, r5, #0
	bl 0x0200923c
	movs r0, #1
	ldr r1, [pc, #708]
	adds r2, r5, #0
	bl 0x0200923c
	movs r0, #2
	ldr r1, [pc, #696]
	adds r2, r5, #0
	bl 0x0200923c
	adds r2, r5, #0
	ldr r1, [pc, #688]
	movs r0, #3
	bl 0x0200923c
	movs r0, #150
	lsls r0, r0, #1
	bl 0x02009194
	bl 0x020092a4
	adds r0, #85
	strb r6, [r0]
	ldr r1, [pc, #668]
	ldr r0, [pc, #668]
	bl 0x0200928c
	movs r2, #215
	movs r3, #1
	movs r1, #0
	lsls r2, r2, #17
	ldr r0, [pc, #660]
	bl 0x02009294
.L_02000808:
	movs r0, #240
	bl 0x02009194
	movs r0, #10
	bl 0x020091dc
	movs r1, #129
	movs r0, #10
	lsls r1, r1, #1
	movs r2, #80
	bl 0x02009284
	ldr r2, [pc, #632]
	ldr r1, [pc, #620]
	movs r0, #10
	bl 0x020091f4
	movs r0, #40
	bl 0x02009194
	movs r1, #4
	movs r0, #10
	bl 0x02009214
	movs r0, #40
	bl 0x02009194
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #20
	movs r0, #10
	bl 0x0200926c
	ldr r0, [pc, #596]
	bl 0x02009244
	movs r1, #0
	movs r2, #20
	ldr r0, [pc, #588]
	bl 0x0200925c
	movs r0, #0
	bl 0x020091dc
	movs r0, #1
	bl 0x020091dc
	movs r0, #2
	bl 0x020091dc
	movs r0, #3
	bl 0x020091dc
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #40
	bl 0x02009284
	movs r2, #20
	ldr r0, [pc, #548]
	movs r1, #0
	bl 0x0200925c
	movs r1, #2
	movs r0, #10
	bl 0x0200922c
	movs r0, #40
	bl 0x02009194
	ldr r0, [pc, #524]
	movs r1, #0
	movs r2, #10
	bl 0x0200925c
	movs r1, #160
	movs r0, #11
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200926c
	movs r2, #40
	ldr r0, [pc, #504]
	movs r1, #0
	bl 0x0200925c
	movs r1, #2
	movs r0, #10
	bl 0x0200922c
	movs r0, #20
	bl 0x02009194
	ldr r0, [pc, #476]
	movs r1, #0
	movs r2, #20
	bl 0x0200925c
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #80
	bl 0x02009284
	movs r1, #131
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #40
	bl 0x02009284
	movs r2, #40
	ldr r0, [pc, #444]
	movs r1, #0
	bl 0x0200925c
	movs r0, #10
	movs r1, #2
	bl 0x02009224
	movs r1, #129
	movs r2, #20
	movs r0, #10
	lsls r1, r1, #1
	bl 0x02009284
	movs r0, #10
	movs r1, #4
	bl 0x0200920c
	movs r2, #10
	ldr r0, [pc, #404]
	movs r1, #0
	bl 0x0200925c
	movs r0, #11
	movs r1, #1
	bl 0x02009224
	movs r1, #3
	movs r0, #11
	bl 0x02009214
	movs r0, #20
	bl 0x02009194
	movs r0, #10
	movs r1, #1
	bl 0x02009224
	movs r0, #10
	movs r1, #4
	bl 0x02009214
	movs r0, #11
	movs r1, #1
	bl 0x02009224
	movs r0, #11
	movs r1, #3
	bl 0x02009214
	movs r0, #10
	movs r1, #1
	bl 0x02009224
	movs r0, #10
	movs r1, #4
	bl 0x02009214
	movs r2, #0
	movs r0, #9
	ldr r1, [pc, #328]
	bl 0x02009284
	movs r1, #1
	movs r0, #9
	bl 0x0200922c
	movs r0, #20
	bl 0x02009194
	movs r1, #128
	movs r2, #40
	movs r0, #9
	lsls r1, r1, #5
	bl 0x0200926c
	movs r1, #2
	movs r0, #9
	bl 0x0200922c
	movs r0, #60
	bl 0x02009194
	movs r1, #3
	movs r0, #9
	bl 0x0200922c
	movs r0, #40
	bl 0x02009194
	movs r2, #40
	ldr r0, [pc, #272]
	movs r1, #0
	bl 0x0200925c
	movs r0, #11
	movs r1, #0
	bl 0x0200920c
	movs r0, #11
	movs r1, #2
	bl 0x0200922c
	movs r2, #10
	ldr r0, [pc, #236]
	movs r1, #0
	bl 0x0200925c
	movs r0, #9
	movs r1, #4
	bl 0x02009214
	movs r0, #9
	movs r1, #2
	bl 0x0200922c
	ldr r0, [pc, #220]
	movs r1, #0
	movs r2, #10
	bl 0x0200925c
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #1
	movs r2, #20
	bl 0x02009284
	movs r1, #160
	movs r2, #40
	movs r0, #10
	lsls r1, r1, #7
	bl 0x0200926c
	movs r0, #10
	movs r1, #3
	bl 0x02009214
	movs r2, #10
	ldr r0, [pc, #180]
	movs r1, #0
	bl 0x0200925c
	movs r0, #9
	movs r1, #4
	bl 0x02009214
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200926c
	movs r2, #0
	movs r0, #9
	movs r1, #2
	bl 0x0200921c
	movs r0, #9
	movs r1, #4
	bl 0x0200920c
	ldr r0, [pc, #132]
	movs r1, #0
	movs r2, #10
	bl 0x0200925c
	movs r0, #11
	ldr r1, [pc, #128]
	movs r2, #0
	bl 0x02009284
	movs r0, #10
	ldr r1, [pc, #116]
	movs r2, #40
	bl 0x02009284
	movs r1, #208
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #80
	bl 0x0200926c
	movs r1, #160
	movs r2, #60
	movs r0, #10
	lsls r1, r1, #7
	bl 0x0200926c
	movs r0, #10
	movs r1, #2
	bl 0x02009224
	movs r0, #11
	movs r1, #2
	bl 0x02009224
	movs r3, #1
	b .L_02000808_0
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x3333
	.2byte 0x0000
	.2byte 0x90c1
	.2byte 0x0200
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x0332
	.2byte 0x0000
	.2byte 0x92fc
	.2byte 0x0200
	.2byte 0x000a
	.2byte 0x0001
	.4byte 0x00000333
	.2byte 0x1999
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0312
	.4byte 0x00000195
	.4byte 0x00001c1e
	.4byte 0x0000900a
	.4byte 0x0000200b
	.4byte 0x00000105
	.4byte 0x00004009
	.4byte 0x0000400a
	.4byte 0x00000101
.L_02000808_0:
	movs r0, #12
	movs r4, #7
	str r0, [sp, #8]
	str r3, [sp, #12]
	str r3, [sp, #20]
	movs r2, #6
	movs r1, #11
	movs r3, #6
	movs r0, #10
	str r2, [sp, #0]
	str r1, [sp, #4]
	str r4, [sp, #16]
	str r6, [sp, #24]
	bl 0x02009274
	movs r0, #20
	bl 0x02009194
	ldr r0, [pc, #1008]
	ldr r1, [pc, #1012]
	bl 0x0200928c
	movs r2, #234
	movs r3, #1
	lsls r2, r2, #17
	movs r1, #0
	ldr r0, [pc, #1000]
	bl 0x02009294
	bl 0x0200929c
	movs r0, #40
	bl 0x02009194
	movs r0, #1
	movs r1, #3
	bl 0x02009214
	movs r2, #20
	ldr r0, [pc, #980]
	movs r1, #0
	bl 0x0200925c
	movs r1, #2
	movs r0, #8
	bl 0x0200922c
	adds r0, r7, #0
	bl 0x0200915c
	movs r0, #40
	bl 0x02009194
	movs r1, #6
	movs r0, #8
	bl 0x02009214
	movs r0, #20
	bl 0x02009194
	ldr r0, [pc, #940]
	movs r1, #0
	movs r2, #20
	bl 0x0200925c
	movs r3, #1
	movs r1, #0
	ldr r2, [pc, #932]
	ldr r0, [pc, #932]
	bl 0x02009294
	movs r0, #20
	bl 0x02009194
	movs r1, #160
	movs r0, #11
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200926c
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200926c
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #5
	movs r2, #40
	bl 0x0200926c
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #40
	bl 0x02009284
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200926c
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #5
	movs r2, #20
	bl 0x0200926c
	movs r1, #192
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #6
	bl 0x0200926c
	movs r1, #6
	movs r0, #8
	bl 0x02009214
	movs r0, #60
	bl 0x02009194
	movs r0, #8
	movs r1, #6
	movs r2, #0
	bl 0x0200921c
	ldr r0, [pc, #804]
	movs r1, #0
	movs r2, #20
	bl 0x0200925c
	movs r0, #1
	ldr r1, [pc, #776]
	ldr r2, [pc, #804]
	bl 0x020091cc
	movs r0, #1
	ldr r1, [pc, #800]
	ldr r2, [pc, #800]
	bl 0x020091f4
	movs r1, #224
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200926c
	movs r0, #1
	movs r1, #3
	bl 0x02009214
	ldr r0, [pc, #780]
	movs r1, #0
	movs r2, #10
	bl 0x0200925c
	movs r1, #128
	movs r2, #20
	movs r0, #8
	lsls r1, r1, #5
	bl 0x0200926c
	movs r0, #8
	movs r1, #3
	bl 0x02009214
	movs r1, #0
	ldr r0, [pc, #724]
	bl 0x0200924c
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200926c
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #5
	movs r2, #0
	bl 0x0200926c
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200926c
	movs r1, #224
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200926c
	movs r1, #176
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200926c
	movs r0, #0
	movs r1, #0
	bl 0x020091bc
	cmp r0, #1
	bne .L_02000808_1
	mov r3, r8
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000808_1:
	movs r2, #214
	lsls r2, r2, #17
	movs r3, #1
	movs r1, #0
	ldr r0, [pc, #612]
	bl 0x02009294
	movs r0, #20
	bl 0x02009194
	movs r0, #10
	movs r1, #2
	bl 0x0200922c
	movs r0, #10
	movs r1, #0
	bl 0x02009254
	movs r1, #4
	movs r0, #11
	bl 0x02009214
	movs r0, #20
	bl 0x02009194
	ldr r0, [pc, #608]
	bl 0x02009244
	ldr r0, [pc, #604]
	movs r1, #0
	bl 0x02009254
	movs r2, #234
	movs r3, #1
	movs r1, #0
	lsls r2, r2, #17
	ldr r0, [pc, #548]
	bl 0x02009294
	movs r0, #20
	bl 0x02009194
	movs r1, #208
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200926c
	movs r1, #3
	movs r0, #1
	bl 0x02009214
	movs r0, #20
	bl 0x02009194
	movs r0, #9
	movs r1, #4
	bl 0x02009214
	movs r1, #208
	movs r2, #10
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200926c
	ldr r0, [pc, #536]
	movs r1, #0
	bl 0x02009254
	movs r0, #8
	movs r1, #3
	bl 0x02009214
	ldr r0, [pc, #484]
	movs r1, #0
	bl 0x02009254
	movs r1, #224
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200926c
	movs r0, #1
	movs r1, #3
	bl 0x02009214
	movs r1, #128
	movs r2, #10
	movs r0, #9
	lsls r1, r1, #5
	bl 0x0200926c
	movs r0, #11
	movs r1, #3
	bl 0x0200920c
	movs r0, #10
	movs r1, #3
	bl 0x0200920c
	movs r0, #9
	movs r1, #3
	bl 0x0200920c
	movs r1, #3
	movs r0, #8
	bl 0x02009214
	movs r0, #20
	bl 0x02009194
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200926c
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #80
	bl 0x02009284
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200926c
	ldr r0, [pc, #392]
	movs r1, #0
	movs r2, #20
	bl 0x0200925c
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #10
	bl 0x0200926c
	movs r2, #40
	movs r0, #0
	movs r1, #0
	bl 0x0200926c
	movs r0, #0
	movs r1, #3
	bl 0x0200920c
	movs r1, #3
	movs r0, #1
	bl 0x02009214
	movs r0, #20
	bl 0x02009194
	movs r1, #128
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #7
	bl 0x0200926c
	movs r1, #3
	movs r0, #3
	bl 0x02009214
	movs r0, #20
	bl 0x02009194
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #5
	movs r2, #0
	bl 0x0200926c
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200926c
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x020091cc
	movs r0, #2
	ldr r1, [pc, #288]
	ldr r2, [pc, #292]
	bl 0x020091f4
	movs r1, #176
	movs r2, #40
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200926c
	movs r0, #2
	movs r1, #2
	bl 0x0200922c
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x0200925c
	movs r0, #2
	movs r1, #3
	bl 0x02009214
	movs r0, #8
	movs r1, #3
	bl 0x0200920c
	movs r0, #9
	movs r1, #3
	bl 0x0200920c
	movs r0, #10
	movs r1, #3
	bl 0x0200920c
	movs r1, #3
	movs r0, #9
	bl 0x02009214
	movs r0, #3
	bl 0x020091c4
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #1
	movs r0, #3
	bl 0x0200927c
	movs r1, #128
	movs r2, #128
	movs r0, #3
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x020091cc
	movs r2, #130
	movs r0, #3
	ldr r1, [pc, #176]
	lsls r2, r2, #2
	bl 0x020091f4
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200926c
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200926c
	movs r1, #196
	movs r2, #248
	movs r0, #3
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x020091f4
	movs r1, #144
	lsls r1, r1, #8
	movs r2, #10
	movs r0, #3
	bl 0x0200926c
	movs r0, #3
	bl 0x020091c4
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x0200925c
	movs r0, #8
	movs r1, #3
	bl 0x0200920c
	movs r0, #9
	movs r1, #3
	bl 0x0200920c
	movs r0, #10
	movs r1, #3
	bl 0x0200920c
	movs r1, #3
	movs r0, #9
	b .L_02000808_2
	.2byte 0x0000
	.4byte 0x00019999
	.4byte 0x00003333
	.4byte 0x03090000
	.4byte 0x00001001
	.4byte 0x00004008
	.4byte 0x01c30000
	.4byte 0x02ee0000
	.4byte 0x0000cccc
	.4byte 0x00000315
	.4byte 0x000001d9
	.4byte 0x00004001
	.4byte 0x00001c33
	.4byte 0x0000200b
	.4byte 0x00004009
	.4byte 0x00000333
	.4byte 0x000001e9
	.4byte 0x0000031a
.L_02000808_2:
	bl 0x02009214
	movs r0, #20
	bl 0x02009194
	movs r2, #214
	movs r3, #1
	movs r1, #0
	lsls r2, r2, #17
	ldr r0, [pc, #368]
	bl 0x02009294
	movs r0, #20
	bl 0x02009194
	movs r0, #11
	ldr r1, [pc, #356]
	ldr r2, [pc, #360]
	bl 0x020091cc
	movs r2, #194
	movs r0, #11
	ldr r1, [pc, #352]
	lsls r2, r2, #1
	bl 0x020091f4
	movs r1, #160
	movs r0, #11
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200926c
	movs r1, #132
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #40
	bl 0x02009284
	ldr r0, [pc, #324]
	movs r1, #0
	movs r2, #20
	bl 0x0200925c
	movs r2, #234
	movs r3, #1
	movs r1, #0
	lsls r2, r2, #17
	ldr r0, [pc, #292]
	bl 0x02009294
	movs r0, #40
	bl 0x02009194
	movs r1, #224
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200926c
	movs r1, #240
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200926c
	movs r1, #144
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200926c
	movs r1, #208
	movs r2, #20
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200926c
	movs r0, #2
	movs r1, #3
	bl 0x0200920c
	movs r1, #3
	movs r0, #3
	bl 0x02009214
	movs r0, #20
	bl 0x02009194
	movs r1, #1
	movs r0, #10
	bl 0x0200922c
	movs r0, #20
	bl 0x02009194
	movs r0, #10
	movs r1, #3
	bl 0x0200920c
	movs r0, #10
	movs r1, #0
	movs r2, #20
	bl 0x0200925c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200926c
	movs r1, #208
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200926c
	movs r1, #176
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200926c
	movs r1, #208
	movs r2, #40
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200926c
	movs r0, #0
	movs r1, #3
	bl 0x0200920c
	movs r0, #1
	movs r1, #3
	bl 0x0200920c
	movs r0, #2
	movs r1, #3
	bl 0x0200920c
	movs r1, #3
	movs r0, #3
	bl 0x02009214
	movs r0, #20
	bl 0x02009194
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #2
	lsls r1, r1, #9
	bl 0x020091cc
	ldr r5, [pc, #96]
	movs r0, #1
	adds r1, r5, #0
	bl 0x020091d4
	adds r1, r5, #0
	movs r0, #2
	bl 0x020091d4
	adds r1, r5, #0
	movs r0, #3
	bl 0x020091e4
	ldr r1, [pc, #76]
	movs r0, #10
	bl 0x020091d4
	movs r2, #188
	movs r0, #11
	ldr r1, [pc, #68]
	lsls r2, r2, #1
	bl 0x020091f4
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #20
	movs r0, #11
	bl 0x0200926c
	ldr r0, [pc, #52]
	bl 0x0200918c
	bl 0x020091a4
	sub sp, #-28
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03090000
	.4byte 0x00006666
	.4byte 0x00003333
	.4byte 0x00000343
	.4byte 0x0000200b
	.4byte 0x02009400
	.4byte 0x02009310
	.4byte 0x00000345
	.4byte 0x0000081d
	.global Func_020010c0
	.thumb_func
Func_020010c0:
	push {r5, r6, lr}
	ldr r3, [pc, #116]
	movs r1, #180
	ldr r0, [r3]
	bl 0x02009144
	movs r6, #23
	cmp r0, #20
	beq .L_020010c0_0
	cmp r0, #20
	bhi .L_020010c0_1
	cmp r0, #10
	beq .L_020010c0_2
	b .L_020010c0_3
.L_020010c0_1:
	cmp r0, #30
	beq .L_020010c0_4
	b .L_020010c0_3
.L_020010c0_0:
	movs r6, #24
	b .L_020010c0_2
.L_020010c0_4:
	movs r6, #25
.L_020010c0_2:
	adds r0, r6, #0
	bl 0x020091c4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020010c0_3
	movs r0, #8
	bl 0x020091c4
	cmp r0, #0
	beq .L_020010c0_5
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	adds r0, r6, #0
	bl 0x02009204
.L_020010c0_5:
	ldr r3, [pc, #48]
	movs r2, #192
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldr r3, [r5, #12]
	lsls r2, r2, #13
	adds r3, r3, r2
	adds r2, r5, #0
	str r3, [r5, #12]
	str r3, [r5, #60]
	adds r2, #100
	movs r3, #25
	strh r3, [r2]
	adds r2, #2
	movs r3, #128
	strh r3, [r2]
	ldr r1, [pc, #20]
	adds r0, r6, #0
	bl 0x020091d4
.L_020010c0_3:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e40
	.4byte 0x00006666
	.4byte 0x02009440
	.include "games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_HEYA/IMPORT.INC"
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00004ccc
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03100000
	.4byte 0x00000000
	.4byte 0x01950000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000010
	.4byte 0xc0010000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x80030000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000010
	.4byte 0xc0030000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03330000
	.4byte 0x00000000
	.4byte 0x01950000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x80020000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000010
	.4byte 0xc0020000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01fc0000
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
	.4byte 0x00000022
	.4byte 0x02008089
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000064
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000060
	.4byte 0xc00000de
	.4byte 0x00040000
	.4byte 0x00f40004
	.4byte 0x00000124
	.4byte 0xffff0002
	.4byte 0x00000151
	.4byte 0xc00000c7
	.4byte 0x00fc0000
	.4byte 0x02240004
	.4byte 0x000000ec
	.4byte 0xffff0003
	.4byte 0x00000290
	.4byte 0xc00000f4
	.4byte 0x022c0000
	.4byte 0x03840004
	.4byte 0x00000114
	.4byte 0xffff0004
	.4byte 0x0000009f
	.4byte 0xc000020d
	.4byte 0x00040000
	.4byte 0x014c0144
	.4byte 0x00000234
	.4byte 0xffff0005
	.4byte 0x00000210
	.4byte 0xc00001e6
	.4byte 0x015c0000
	.4byte 0x0264011c
	.4byte 0x00000204
	.4byte 0xffff0006
	.4byte 0x00000330
	.4byte 0xc000020a
	.4byte 0x02740000
	.4byte 0x0374011c
	.4byte 0x00000224
	.4byte 0xffff0007
	.4byte 0x000001cf
	.4byte 0xc0000353
	.4byte 0x01440000
	.4byte 0x024c025c
	.4byte 0x00000374
	.4byte 0xffff0008
	.4byte 0x00000301
	.4byte 0xc0000359
	.4byte 0x02440000
	.4byte 0x03540274
	.4byte 0x00000374
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000007
	.4byte 0x0010a004
	.4byte 0x00209004
	.4byte 0x00303005
	.4byte 0x00405005
	.4byte 0x00507004
	.4byte 0x00606004
	.4byte 0x00704005
	.4byte 0x00801005
	.4byte 0x000001ff
	.4byte 0x00000007
	.4byte 0x0010a003
	.4byte 0x00209003
	.4byte 0x00303005
	.4byte 0x00405005
	.4byte 0x00507003
	.4byte 0x00606003
	.4byte 0x00704005
	.4byte 0x00801005
	.4byte 0x000001ff
	.4byte 0x00000075
	.4byte 0x00000001
	.4byte 0x02b90000
	.4byte 0x00000000
	.4byte 0x01d40000
	.4byte 0x00013000
	.4byte 0x0000006f
	.4byte 0x00000001
	.4byte 0x02d60000
	.4byte 0x00000000
	.4byte 0x01e60000
	.4byte 0x0001b000
	.4byte 0x00000036
	.4byte 0x00000002
	.4byte 0x032f0000
	.4byte 0x00000000
	.4byte 0x01820000
	.4byte 0x00000000
	.4byte 0x00000026
	.4byte 0x00000001
	.4byte 0x03490000
	.4byte 0x00000000
	.4byte 0x01c70000
	.4byte 0x0000c000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x01930000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00008000
	.4byte 0x0000006f
	.4byte 0x00000001
	.4byte 0x014b0000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00020000
	.4byte 0x00000073
	.4byte 0x00000001
	.4byte 0x01e60000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0x00000065
	.4byte 0x00000001
	.4byte 0x01ef0000
	.4byte 0x00000000
	.4byte 0x01c50000
	.4byte 0x0000d000
	.4byte 0x00000073
	.4byte 0x00000001
	.4byte 0x01950000
	.4byte 0x00000000
	.4byte 0x01a50000
	.4byte 0x00018000
	.4byte 0x0000006c
	.4byte 0x00000002
	.4byte 0x00630000
	.4byte 0x00000000
	.4byte 0x007d0000
	.4byte 0x0000c000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x03000000
	.4byte 0x00000000
	.4byte 0x03000000
	.4byte 0x00008000
	.4byte 0x00000074
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02b90000
	.4byte 0x0001c000
	.4byte 0x00000076
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0x0000007c
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00004000
	.4byte 0x0000007d
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000075
	.4byte 0x00000001
	.4byte 0x02b90000
	.4byte 0x00000000
	.4byte 0x01d40000
	.4byte 0x00033000
	.4byte 0x0000006f
	.4byte 0x00000001
	.4byte 0x02d60000
	.4byte 0x00000000
	.4byte 0x01e60000
	.4byte 0x0001b000
	.4byte 0x00000036
	.4byte 0x00000002
	.4byte 0x032f0000
	.4byte 0x00000000
	.4byte 0x01820000
	.4byte 0x00000000
	.4byte 0x00000026
	.4byte 0x00000001
	.4byte 0x03490000
	.4byte 0x00000000
	.4byte 0x01c70000
	.4byte 0x0000c000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x01930000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00008000
	.4byte 0x0000006f
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x009c0000
	.4byte 0x00020000
	.4byte 0x00000073
	.4byte 0x00000001
	.4byte 0x01e60000
	.4byte 0x00000000
	.4byte 0x00760000
	.4byte 0x0000c000
	.4byte 0x00000065
	.4byte 0x00000001
	.4byte 0x01ef0000
	.4byte 0x00000000
	.4byte 0x01c50000
	.4byte 0x0000d000
	.4byte 0x00000073
	.4byte 0x00000001
	.4byte 0x01950000
	.4byte 0x00000000
	.4byte 0x01a50000
	.4byte 0x00018000
	.4byte 0x0000006c
	.4byte 0x00000002
	.4byte 0x00630000
	.4byte 0x00000000
	.4byte 0x007d0000
	.4byte 0x0000c000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x03000000
	.4byte 0x00000000
	.4byte 0x03000000
	.4byte 0x00008000
	.4byte 0x00000074
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02b90000
	.4byte 0x0001c000
	.4byte 0x00000076
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00034000
	.4byte 0x0000007c
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00004000
	.4byte 0x0000007d
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01b80000
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
	.4byte 0x0000c000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x00000075
	.4byte 0x00000001
	.4byte 0x02b90000
	.4byte 0x00000000
	.4byte 0x01d40000
	.4byte 0x00003000
	.4byte 0x0000006f
	.4byte 0x00000001
	.4byte 0x02d60000
	.4byte 0x00000000
	.4byte 0x01e60000
	.4byte 0x0000b000
	.4byte 0x00000036
	.4byte 0x02009310
	.4byte 0x03330000
	.4byte 0x00000000
	.4byte 0x01950000
	.4byte 0x00009000
	.4byte 0x00000026
	.4byte 0x00000001
	.4byte 0x03490000
	.4byte 0x00000000
	.4byte 0x01720000
	.4byte 0x0000d000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x01930000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00008000
	.4byte 0x0000006f
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x009c0000
	.4byte 0x00000000
	.4byte 0x00000073
	.4byte 0x00000001
	.4byte 0x01ee0000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x0000c000
	.4byte 0x00000065
	.4byte 0x00000001
	.4byte 0x01ef0000
	.4byte 0x00000000
	.4byte 0x01c50000
	.4byte 0x0000d000
	.4byte 0x00000073
	.4byte 0x00000001
	.4byte 0x01950000
	.4byte 0x00000000
	.4byte 0x01a50000
	.4byte 0x00018000
	.4byte 0x0000006c
	.4byte 0x00000002
	.4byte 0x00630000
	.4byte 0x00000000
	.4byte 0x007d0000
	.4byte 0x0000c000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x03000000
	.4byte 0x00000000
	.4byte 0x03000000
	.4byte 0x00008000
	.4byte 0x00000074
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02b90000
	.4byte 0x0001c000
	.4byte 0x00000076
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00004000
	.4byte 0x0000007c
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00004000
	.4byte 0x0000007d
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00004000
	.4byte 0xffff0118
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0118
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0xffff0118
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02008309
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008315
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008321
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x0200832d
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x02008339
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte 0x02008345
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte 0x02008351
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x0200835d
	.4byte 0x000000f3
	.4byte 0xffff00c8
	.4byte 0x004029c4
	.4byte 0x000000f3
	.4byte 0xffff00c9
	.4byte 0x004029c5
	.4byte 0x0000c4f3
	.4byte 0xffff00ca
	.4byte 0x004029c6
	.4byte 0x0000c4f3
	.4byte 0xffff00cb
	.4byte 0x004029c7
	.4byte 0x0000c4f3
	.4byte 0xffff00cc
	.4byte 0x004029c8
	.4byte 0x0000c4f3
	.4byte 0xffff00cd
	.4byte 0x004029c9
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x004029ca
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x004029cb
	.4byte 0x000000f3
	.4byte 0xffff00d0
	.4byte 0x004029cc
	.4byte 0x000000f3
	.4byte 0xffff00d1
	.4byte 0x004029cd
	.4byte 0x000000f3
	.4byte 0xffff00d2
	.4byte 0x004029ce
	.4byte 0x000000f3
	.4byte 0xffff00d3
	.4byte 0x004029cf
	.4byte 0x00000033
	.4byte 0xffff0064
	.4byte 0x0040094b
	.4byte 0x00000023
	.4byte 0xffff0065
	.4byte 0x0040094a
	.4byte 0x00000023
	.4byte 0xffff0066
	.4byte 0x0040094a
	.4byte 0x00000023
	.4byte 0xffff0067
	.4byte 0x0040094a
	.4byte 0x00000023
	.4byte 0xffff0068
	.4byte 0x0040094a
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00000f5f
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00000f60
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00000f61
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00000f62
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00000f6a
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00000f6b
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00000f6c
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008191
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00000f70
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00000f71
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00000f72
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x020081bd
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x020084dd
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x0200855d
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x020085d5
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02008309
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008315
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008321
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x0200832d
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x02008339
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte 0x02008345
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte 0x02008351
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x0200835d
	.4byte 0x000000f3
	.4byte 0xffff00c8
	.4byte 0x004029c4
	.4byte 0x000000f3
	.4byte 0xffff00c9
	.4byte 0x004029c5
	.4byte 0x0000c4f3
	.4byte 0xffff00ca
	.4byte 0x004029c6
	.4byte 0x0000c4f3
	.4byte 0xffff00cb
	.4byte 0x004029c7
	.4byte 0x0000c4f3
	.4byte 0xffff00cc
	.4byte 0x004029c8
	.4byte 0x0000c4f3
	.4byte 0xffff00cd
	.4byte 0x004029c9
	.4byte 0x00000033
	.4byte 0x0f420064
	.4byte 0x001000bb
	.4byte 0x00000023
	.4byte 0x0f430065
	.4byte 0x00200006
	.4byte 0x00000023
	.4byte 0x0f440066
	.4byte 0x00200002
	.4byte 0x00000023
	.4byte 0x0f450067
	.4byte 0x001000e2
	.4byte 0x00000023
	.4byte 0x0f460068
	.4byte 0x00200003
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000011b0
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000011b1
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000011b2
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000011b3
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000011ba
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000011bb
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000011bc
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000011bd
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008259
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000011c1
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000011c2
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000011c3
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x020084dd
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x0200855d
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x020085d5
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000011db
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x000011dc
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000011dd
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000011e3
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000011e4
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000011e5
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000011e6
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000011e9
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000011ea
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000011eb
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000011ec
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000011ed
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000011ee
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000011ef
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000011f0
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02008309
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008315
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008321
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x0200832d
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x02008339
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte 0x02008345
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte 0x02008351
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x0200835d
	.4byte 0x00000033
	.4byte 0x0f420064
	.4byte 0x001000bb
	.4byte 0x00000023
	.4byte 0x0f430065
	.4byte 0x00200006
	.4byte 0x00000023
	.4byte 0x0f440066
	.4byte 0x00200002
	.4byte 0x00000023
	.4byte 0x0f450067
	.4byte 0x001000e2
	.4byte 0x00000023
	.4byte 0x0f460068
	.4byte 0x00200003
	.4byte 0x000000f3
	.4byte 0xffff00c8
	.4byte 0x004029c4
	.4byte 0x000000f3
	.4byte 0xffff00c9
	.4byte 0x004029c5
	.4byte 0x0000c4f3
	.4byte 0xffff00ca
	.4byte 0x004029c6
	.4byte 0x0000c4f3
	.4byte 0xffff00cb
	.4byte 0x004029c7
	.4byte 0x0000c4f3
	.4byte 0xffff00cc
	.4byte 0x004029c8
	.4byte 0x0000c4f3
	.4byte 0xffff00cd
	.4byte 0x004029c9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001c3b
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001c3c
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008279
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008299
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001c7c
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001c7d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001c7e
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001c82
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001c83
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001c86
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001c88
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001c8a
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x020084dd
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x0200855d
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x020085d5
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001c41
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001c42
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001c43
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001c44
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001c0d
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001c0e
	.4byte 0x00008d15
	.4byte 0x081b0014
	.4byte 0x00001c12
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001c0f
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001c7f
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001c80
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001c81
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001c84
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001c85
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001c87
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001c89
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001c8b
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02008309
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008315
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008321
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x0200832d
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x02008339
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte 0x02008345
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte 0x02008351
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x0200835d
	.4byte 0x00000033
	.4byte 0x0f420064
	.4byte 0x001000bb
	.4byte 0x00000023
	.4byte 0x0f430065
	.4byte 0x00200006
	.4byte 0x00000023
	.4byte 0x0f440066
	.4byte 0x00200002
	.4byte 0x00000023
	.4byte 0x0f450067
	.4byte 0x001000e2
	.4byte 0x00000023
	.4byte 0x0f460068
	.4byte 0x00200003
	.4byte 0x000000f3
	.4byte 0xffff00c8
	.4byte 0x004029c4
	.4byte 0x000000f3
	.4byte 0xffff00c9
	.4byte 0x004029c5
	.4byte 0x0000c4f3
	.4byte 0xffff00ca
	.4byte 0x004029c6
	.4byte 0x0000c4f3
	.4byte 0xffff00cb
	.4byte 0x004029c7
	.4byte 0x0000c4f3
	.4byte 0xffff00cc
	.4byte 0x004029c8
	.4byte 0x0000c4f3
	.4byte 0xffff00cd
	.4byte 0x004029c9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
