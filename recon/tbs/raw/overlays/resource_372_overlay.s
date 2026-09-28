.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {r5, r6, lr}
	adds r6, r0, #0
	adds r5, r6, #0
	adds r5, #100
	movs r2, #0
	ldrsh r3, [r5, r2]
	adds r0, r3, #0
	cmp r3, #0
	bne .L_02000030_0
	bl 0x0200c654
	strh r0, [r6, #6]
	bl 0x0200c654
	movs r1, #20
	bl 0x0200c634
	adds r0, #20
	strh r0, [r5]
.L_02000030_0:
	subs r3, r0, #1
	strh r3, [r5]
	movs r0, #1
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02000064
	.thumb_func
Func_02000064:
	push {r5, lr}
	adds r5, r0, #0
	adds r5, #100
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #2
	beq .L_02000064_0
	cmp r3, #2
	bgt .L_02000064_1
	cmp r3, #0
	beq .L_02000064_2
	b .L_02000064_3
.L_02000064_1:
	cmp r3, #4
	beq .L_02000064_4
	cmp r3, #6
	bne .L_02000064_3
	ldr r3, [r0, #24]
	ldr r2, [pc, #84]
	adds r3, r3, r2
	str r3, [r0, #24]
	movs r2, #128
	ldr r3, [r0, #28]
	lsls r2, r2, #6
	b .L_02000064_5
.L_02000064_4:
	ldr r3, [r0, #24]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [pc, #64]
	b .L_02000064_6
.L_02000064_0:
	ldr r3, [r0, #24]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [pc, #52]
.L_02000064_6:
	ldr r3, [r0, #28]
.L_02000064_5:
	adds r3, r3, r2
	str r3, [r0, #28]
	b .L_02000064_3
.L_02000064_2:
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #24]
	str r3, [r0, #28]
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	adds r0, #60
	strh r0, [r5]
.L_02000064_3:
	ldrh r3, [r5]
	subs r3, #1
	strh r3, [r5]
	movs r0, #1
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0xffffc000
	.4byte 0xfffff000
	.4byte 0xfffff800
	.global Func_020000e8
	.thumb_func
Func_020000e8:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200d0e4
	.global Func_020000f0
	.thumb_func
Func_020000f0:
	movs r0, #0
	bx lr
	.global Func_020000f4
	.thumb_func
Func_020000f4:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200d27c
	.global Func_020000fc
	.thumb_func
Func_020000fc:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200d2b8
	.global Func_02000104
	.thumb_func
Func_02000104:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200d558
	.global Func_0200010c
	.thumb_func
Func_0200010c:
	push {lr}
	movs r0, #132
	lsls r0, r0, #2
	sub sp, #8
	bl 0x0200c6e4
	movs r3, #10
	movs r2, #84
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #40
	movs r1, #84
	movs r2, #7
	movs r3, #4
	bl 0x0200c6ac
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000134
	.thumb_func
Func_02000134:
	push {lr}
	movs r0, #132
	lsls r0, r0, #2
	sub sp, #8
	bl 0x0200c6ec
	movs r3, #10
	movs r2, #84
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #40
	movs r1, #89
	movs r2, #7
	movs r3, #4
	bl 0x0200c6ac
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200015c
	.thumb_func
Func_0200015c:
	push {r5, lr}
	adds r5, r0, #0
	ldr r0, [pc, #44]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_0200015c_0
	bl 0x0200c85c
.L_0200015c_0:
	ldr r3, [pc, #36]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	adds r3, #200
	adds r2, r1, r3
	movs r3, #16
	str r3, [r2]
	adds r0, r5, #0
	bl 0x0200c83c
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000834
	.4byte 0x03001ebc
	.global Func_02000198
	.thumb_func
Func_02000198:
	push {lr}
	movs r0, #158
	bl 0x0200c8b4
	ldr r0, [pc, #36]
	movs r1, #45
	movs r2, #11
	bl 0x0200c68c
	movs r2, #210
	ldr r1, [pc, #28]
	lsls r2, r2, #1
	movs r0, #0
	bl 0x0200c774
	movs r0, #3
	bl 0x0200c6f4
	movs r0, #11
	bl 0x0200815c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200d774
	.4byte 0x00000101
	.global Func_020001d0
	.thumb_func
Func_020001d0:
	push {lr}
	movs r0, #123
	bl 0x0200c8b4
	movs r0, #1
	bl 0x0200815c
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020001e4
	.thumb_func
Func_020001e4:
	push {lr}
	movs r0, #123
	bl 0x0200c8b4
	movs r0, #3
	bl 0x0200815c
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020001f8
	.thumb_func
Func_020001f8:
	push {lr}
	movs r0, #123
	bl 0x0200c8b4
	movs r0, #4
	bl 0x0200815c
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200020c
	.thumb_func
Func_0200020c:
	push {lr}
	movs r0, #123
	bl 0x0200c8b4
	ldr r0, [pc, #32]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_0200020c_0
	ldr r0, [pc, #28]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_0200020c_0
	bl 0x0200bfb0
.L_0200020c_0:
	movs r0, #2
	bl 0x0200815c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000841
	.4byte 0x00000842
	.global Func_02000240
	.thumb_func
Func_02000240:
	push {lr}
	movs r0, #158
	bl 0x0200c8b4
	ldr r0, [pc, #36]
	movs r1, #54
	movs r2, #32
	bl 0x0200c68c
	movs r1, #203
	lsls r1, r1, #1
	ldr r2, [pc, #28]
	movs r0, #0
	bl 0x0200c774
	movs r0, #3
	bl 0x0200c6f4
	movs r0, #5
	bl 0x0200815c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200d78a
	.4byte 0x000002d7
	.global Func_02000278
	.thumb_func
Func_02000278:
	push {lr}
	ldr r0, [pc, #84]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_02000278_0
	movs r0, #158
	bl 0x0200c8b4
	ldr r0, [pc, #72]
	movs r1, #45
	movs r2, #39
	bl 0x0200c68c
.L_02000278_0:
	ldr r0, [pc, #64]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_02000278_1
	ldr r0, [pc, #60]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_02000278_1
	bl 0x0200950c
	ldr r0, [pc, #32]
	bl 0x0200c6e4
	b .L_02000278_2
.L_02000278_1:
	movs r1, #131
	movs r0, #0
	lsls r1, r1, #1
	ldr r2, [pc, #36]
	bl 0x0200c774
	movs r0, #3
	bl 0x0200c6f4
	movs r0, #6
	bl 0x0200815c
.L_02000278_2:
	pop {r0}
	bx r0
	.4byte 0x00000206
	.4byte 0x0200d7a0
	.4byte 0x00000835
	.4byte 0x00000831
	.4byte 0x00000325
	.global Func_020002e4
	.thumb_func
Func_020002e4:
	push {lr}
	ldr r0, [pc, #56]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_020002e4_0
	movs r0, #158
	bl 0x0200c8b4
	ldr r0, [pc, #44]
	movs r1, #50
	movs r2, #44
	bl 0x0200c68c
.L_020002e4_0:
	movs r1, #170
	movs r2, #222
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #0
	bl 0x0200c774
	movs r0, #3
	bl 0x0200c6f4
	movs r0, #7
	bl 0x0200815c
	pop {r0}
.L_0200031c:
	bx r0
	.2byte 0x0000
	.2byte 0x0205
	.2byte 0x0000
	.2byte 0xd78a
	.2byte 0x0200
	.global Func_02000328
	.thumb_func
Func_02000328:
	push {lr}
	movs r0, #158
	bl 0x0200c8b4
	ldr r0, [pc, #36]
	movs r1, #49
	movs r2, #69
	bl 0x0200c68c
	movs r1, #163
	lsls r1, r1, #1
	ldr r2, [pc, #28]
	movs r0, #0
	bl 0x0200c774
	movs r0, #3
	bl 0x0200c6f4
.L_0200034c:
	movs r0, #8
	bl 0x0200815c
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0xd7a0
	.2byte 0x0200
	.2byte 0x0466
	.2byte 0x0000
.L_02000360:
	.global Func_02000360
	.thumb_func
Func_02000360:
	push {lr}
	movs r0, #158
	bl 0x0200c8b4
	ldr r0, [pc, #36]
	movs r1, #52
	movs r2, #76
	bl 0x0200c68c
	movs r1, #187
	lsls r1, r1, #1
	ldr r2, [pc, #28]
	movs r0, #0
	bl 0x0200c774
	movs r0, #3
	bl 0x0200c6f4
	movs r0, #9
	bl 0x0200815c
	pop {r0}
.L_0200038c:
	bx r0
	.2byte 0x0000
	.2byte 0xd7b6
	.2byte 0x0200
	.2byte 0x04d6
	.2byte 0x0000
	.global Func_02000398
	.thumb_func
Func_02000398:
	push {lr}
	movs r0, #158
	bl 0x0200c8b4
	ldr r0, [pc, #32]
	movs r1, #35
	movs r2, #74
	bl 0x0200c68c
	movs r1, #102
	ldr r2, [pc, #24]
	movs r0, #0
	bl 0x0200c774
	movs r0, #3
	bl 0x0200c6f4
	movs r0, #10
	bl 0x0200815c
	pop {r0}
	bx r0
	.4byte 0x0200d78a
	.4byte 0x000004b6
	.global Func_020003cc
	.thumb_func
Func_020003cc:
	push {lr}
	movs r0, #158
	bl 0x0200c8b4
	ldr r0, [pc, #32]
	movs r1, #35
	movs r2, #73
	bl 0x0200c68c
	movs r1, #102
	ldr r2, [pc, #24]
	movs r0, #0
	bl 0x0200c774
	movs r0, #3
	bl 0x0200c6f4
	movs r0, #12
	bl 0x0200815c
	pop {r0}
	bx r0
	.4byte 0x0200d78a
	.4byte 0x000004b6
	.global Func_02000400
	.thumb_func
Func_02000400:
	push {lr}
	movs r0, #158
	bl 0x0200c8b4
	ldr r0, [pc, #32]
	movs r1, #38
	movs r2, #72
	bl 0x0200c68c
	movs r1, #146
	ldr r2, [pc, #24]
	movs r0, #0
	bl 0x0200c774
	movs r0, #3
	bl 0x0200c6f4
	movs r0, #13
	bl 0x0200815c
	pop {r0}
	bx r0
	.4byte 0x0200d7a0
	.4byte 0x0000049e
	.global Func_02000434
	.thumb_func
Func_02000434:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #170
	sub sp, #8
	bl 0x0200c8a4
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl 0x0200c78c
	ldr r0, [pc, #920]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_02000434_0
	ldr r0, [pc, #916]
	bl 0x0200c6ec
	ldr r0, [pc, #912]
	bl 0x0200c6ec
.L_02000434_0:
	movs r0, #131
	lsls r0, r0, #4
	bl 0x0200c6dc
	cmp r0, #0
	beq 0x02008480
	movs r1, #167
	movs r2, #233
	movs r0, #11
.L_02000474:
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl 0x0200c78c
	bl 0x02008ec4
	ldr r0, [pc, #880]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_02000474_0
	movs r1, #224
	movs r2, #218
	movs r0, #12
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200c78c
	bl 0x020090a4
.L_02000474_0:
	ldr r0, [pc, #856]
	bl 0x0200c6dc
	cmp r0, #0
	beq 0x020084b6
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #15
.L_020004ac:
	ldr r2, [pc, #844]
	bl 0x0200c78c
	bl 0x020092f0
	ldr r0, [pc, #840]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_020004ac_0
	movs r1, #216
	movs r0, #14
	lsls r1, r1, #17
	ldr r2, [pc, #828]
	bl 0x0200c78c
	bl 0x02009498
.L_020004ac_0:
	movs r0, #11
	bl 0x0200c72c
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #4
	orrs r3, r5
	strb r3, [r0]
	movs r0, #12
	bl 0x0200c72c
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #13
	bl 0x0200c72c
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #14
	bl 0x0200c72c
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #15
	bl 0x0200c72c
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #16
.L_0200051a:
	bl 0x0200c72c
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #17
	bl 0x0200c72c
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #18
	bl 0x0200c72c
	adds r0, #89
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	ldr r0, [pc, #708]
	bl 0x0200c6dc
	cmp r0, #0
	beq 0x02008556
	movs r0, #22
.L_0200054e:
	movs r1, #0
	movs r2, #0
	bl 0x0200c78c
	movs r0, #19
	bl 0x0200c72c
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r0, #24]
	str r3, [r0, #28]
	ldr r0, [pc, #676]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_0200054e_0
	movs r1, #228
	movs r0, #19
	lsls r1, r1, #15
	ldr r2, [pc, #664]
	bl 0x0200c78c
	b .L_0200054e_1
.L_0200054e_0:
	ldr r1, [pc, #660]
	movs r0, #19
	bl 0x0200c744
.L_0200054e_1:
	ldr r0, [pc, #656]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_0200054e_2
	b .L_0200054e_3
.L_0200054e_2:
	bl 0x0200c0f0
	movs r1, #165
	ldr r2, [pc, #644]
	lsls r1, r1, #16
	movs r0, #9
	bl 0x0200c78c
	movs r0, #9
	bl 0x0200c72c
	movs r6, #224
	adds r5, r0, #0
	lsls r6, r6, #8
	strh r6, [r5, #6]
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	ldr r2, [pc, #612]
	adds r3, r5, #0
	adds r0, #60
	adds r3, #100
	mov r8, r2
	strh r0, [r3]
	adds r5, #102
	movs r3, #1
	strh r3, [r5]
	movs r0, #9
	mov r1, r8
	bl 0x0200c744
	movs r1, #165
	ldr r2, [pc, #588]
	lsls r1, r1, #16
	movs r0, #26
	bl 0x0200c78c
	movs r0, #26
	bl 0x0200c72c
	adds r5, r0, #0
	strh r6, [r5, #6]
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	adds r3, r5, #0
	adds r0, #60
	adds r3, #100
	strh r0, [r3]
	adds r5, #102
	movs r3, #2
	strh r3, [r5]
	movs r0, #26
	mov r1, r8
	bl 0x0200c744
	movs r1, #152
	ldr r2, [pc, #540]
	lsls r1, r1, #16
	movs r0, #22
	bl 0x0200c78c
	movs r0, #22
	bl 0x0200c72c
	adds r5, r0, #0
	strh r6, [r5, #6]
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	adds r3, r5, #0
	adds r0, #60
	adds r3, #100
	strh r0, [r3]
	adds r5, #102
	movs r3, #3
	strh r3, [r5]
	movs r0, #22
	mov r1, r8
	bl 0x0200c744
	movs r1, #184
	movs r2, #163
	lsls r2, r2, #19
	lsls r1, r1, #16
	movs r0, #8
	bl 0x0200c78c
	movs r0, #8
	bl 0x0200c72c
	adds r5, r0, #0
	strh r6, [r5, #6]
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	adds r3, r5, #0
	adds r0, #60
	adds r3, #100
	strh r0, [r3]
	adds r5, #102
	movs r3, #4
	strh r3, [r5]
	movs r0, #8
	mov r1, r8
	bl 0x0200c744
	movs r1, #6
	movs r0, #8
	bl 0x0200c794
	movs r0, #22
	bl 0x0200c72c
	adds r0, #35
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #8
	bl 0x0200c72c
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #200
	ands r5, r3
	strb r5, [r0]
	lsls r1, r1, #4
	ldr r0, [pc, #392]
	bl 0x0200c644
	movs r0, #24
	movs r1, #0
	movs r2, #0
	bl 0x0200c78c
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl 0x0200c78c
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl 0x0200c78c
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl 0x0200c78c
	ldr r0, [pc, #348]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_0200054e_4
	b .L_0200054e_5
.L_0200054e_4:
	movs r0, #22
	b .L_0200054e_6
.L_0200054e_3:
	ldr r0, [pc, #336]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_0200054e_7
	b .L_0200054e_8
.L_0200054e_7:
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #16
	ldr r2, [pc, #324]
	bl 0x0200c78c
	movs r1, #128
	movs r2, #0
	movs r0, #10
	lsls r1, r1, #6
	bl 0x0200c7fc
	movs r1, #5
	movs r0, #10
	bl 0x0200c794
	movs r0, #10
	bl 0x0200c72c
	adds r5, r0, #0
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	ldr r6, [pc, #256]
	adds r0, #60
	adds r5, #100
	strh r0, [r5]
	adds r1, r6, #0
	movs r0, #10
	bl 0x0200c744
	movs r1, #227
	movs r0, #24
	lsls r1, r1, #16
	ldr r2, [pc, #256]
	bl 0x0200c78c
	movs r1, #128
	movs r2, #0
	movs r0, #24
	lsls r1, r1, #7
	bl 0x0200c7fc
	movs r1, #6
	movs r0, #24
	bl 0x0200c794
	movs r0, #24
	bl 0x0200c72c
	adds r5, r0, #0
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	adds r5, #100
	adds r0, #60
	strh r0, [r5]
	adds r1, r6, #0
	movs r0, #24
	bl 0x0200c744
	movs r1, #247
	movs r0, #25
	lsls r1, r1, #16
	ldr r2, [pc, #192]
	bl 0x0200c78c
	movs r1, #128
	movs r2, #0
	movs r0, #25
	lsls r1, r1, #7
	bl 0x0200c7fc
	movs r1, #6
	movs r0, #25
	bl 0x0200c794
	movs r0, #25
	bl 0x0200c72c
	adds r5, r0, #0
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	adds r5, #100
	adds r0, #60
	strh r0, [r5]
	adds r1, r6, #0
	movs r0, #25
	bl 0x0200c744
	movs r1, #243
	movs r0, #23
	lsls r1, r1, #16
	ldr r2, [pc, #132]
	bl 0x0200c78c
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #23
	bl 0x0200c7fc
	movs r0, #23
	bl 0x0200c72c
	movs r1, #0
	bl 0x0200c6b4
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl 0x0200c78c
	movs r0, #18
.L_0200054e_6:
	movs r1, #0
	movs r2, #0
	bl 0x0200c78c
	b .L_0200054e_5
	.2byte 0x0109
	.2byte 0x0000
	.2byte 0x0205
	.2byte 0x0000
	.2byte 0x0206
	.2byte 0x0000
	.2byte 0x0831
	.2byte 0x0000
	.2byte 0x0832
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x02bf
	.2byte 0x0833
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x047b
	.2byte 0x0837
	.2byte 0x0000
	.4byte 0x00000838
	.4byte 0x014d0000
	.4byte 0x0200c9f4
	.4byte 0x00000841
	.4byte 0x04cd0000
	.4byte 0x0200cec8
	.4byte 0x04e60000
	.4byte 0x05050000
	.4byte 0x0200c5b9
	.4byte 0x00000842
	.4byte 0x0000083a
	.4byte 0x04be0000
	.4byte 0x04fd0000
.L_0200054e_8:
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl 0x0200c78c
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl 0x0200c78c
.L_0200054e_5:
	ldr r3, [pc, #404]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #15
	bne .L_0200054e_9
	ldr r0, [pc, #392]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_0200054e_10
.L_0200054e_9:
	bl 0x0200c84c
	bl 0x0200c854
.L_0200054e_10:
	movs r0, #132
	lsls r0, r0, #2
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_0200054e_11
	bl 0x0200810c
.L_0200054e_11:
	ldr r0, [pc, #364]
	bl 0x0200c6e4
	movs r3, #26
	str r3, [sp, #0]
	movs r5, #46
	movs r0, #29
	movs r1, #24
	movs r2, #1
	movs r3, #2
	str r5, [sp, #4]
	bl 0x0200c6ac
	movs r3, #27
	str r3, [sp, #0]
	movs r0, #29
	movs r1, #25
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200c6ac
	movs r3, #28
	str r3, [sp, #0]
	movs r0, #29
	movs r1, #25
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200c6ac
	movs r3, #88
	str r3, [sp, #4]
	movs r5, #20
	movs r0, #19
	movs r1, #90
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200c6ac
	movs r3, #89
	str r3, [sp, #4]
	movs r1, #90
	movs r2, #1
	movs r3, #1
	movs r0, #19
	str r5, [sp, #0]
	bl 0x0200c6ac
	movs r0, #21
	bl 0x0200c72c
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #192
	lsls r3, r3, #16
	str r3, [r0, #12]
	adds r2, #4
	movs r3, #8
	strb r3, [r2]
	movs r1, #0
	bl 0x0200c6b4
	movs r0, #1
	bl 0x0200c63c
	ldr r0, [pc, #220]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_0200054e_12
	ldr r3, [pc, #208]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #15
	bne .L_0200054e_12
	bl 0x02008a10
	b .L_0200054e_13
.L_0200054e_12:
	movs r0, #23
	movs r1, #7
	bl 0x0200c794
	ldr r0, [pc, #188]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_0200054e_14
	bl 0x0200c6fc
	movs r0, #22
	ldr r1, [pc, #176]
	bl 0x0200c814
	movs r1, #200
	movs r0, #22
	lsls r1, r1, #17
	ldr r2, [pc, #168]
	bl 0x0200c78c
	movs r1, #212
	movs r0, #21
	lsls r1, r1, #17
	ldr r2, [pc, #160]
	bl 0x0200c78c
	movs r1, #200
	movs r0, #22
	lsls r1, r1, #1
	ldr r2, [pc, #152]
	bl 0x0200c774
	movs r1, #212
	ldr r2, [pc, #144]
	movs r0, #21
	lsls r1, r1, #1
	bl 0x0200c77c
	movs r0, #21
	movs r1, #2
	bl 0x0200c794
	movs r0, #22
	movs r1, #5
	bl 0x0200c794
	bl 0x0200c704
	b .L_0200054e_15
.L_0200054e_14:
	bl 0x0200c6fc
	movs r1, #212
	movs r0, #21
	lsls r1, r1, #17
	ldr r2, [pc, #100]
	bl 0x0200c78c
	movs r1, #212
	movs r0, #21
	lsls r1, r1, #1
	ldr r2, [pc, #92]
	bl 0x0200c77c
	movs r0, #21
	movs r1, #3
	bl 0x0200c794
	bl 0x0200c704
.L_0200054e_15:
	ldr r3, [pc, #76]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	adds r3, #200
	adds r2, r1, r3
	movs r3, #24
	str r3, [r2]
	bl 0x0200c884
	bl 0x0200c88c
	bl 0x0200c86c
.L_0200054e_13:
	movs r0, #0
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x0000087b
	.4byte 0x00000834
	.4byte 0x00000837
	.4byte 0x00000101
	.4byte 0x02630000
	.4byte 0x02730000
	.4byte 0x0000026b
	.4byte 0x03001ebc
	.global Func_02000a10
	.thumb_func
Func_02000a10:
	push {r5, lr}
	bl 0x0200c6fc
	bl 0x0200c84c
	bl 0x0200c854
	bl 0x0200c864
	movs r0, #60
	bl 0x0200c63c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #7
	lsls r1, r1, #4
	bl 0x0200c81c
	movs r0, #158
	movs r1, #160
	movs r2, #220
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200c824
	movs r1, #147
	movs r2, #217
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl 0x0200c78c
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200c78c
	ldr r3, [pc, #688]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	adds r3, #200
	adds r2, r1, r3
	movs r3, #16
	str r3, [r2]
	bl 0x0200c884
	bl 0x0200c88c
	bl 0x0200c86c
	movs r0, #158
	bl 0x0200c8b4
	movs r2, #44
	ldr r0, [pc, #648]
	movs r1, #50
	bl 0x0200c68c
	movs r0, #22
	ldr r1, [pc, #644]
	bl 0x0200c814
	movs r0, #9
	ldr r1, [pc, #640]
	ldr r2, [pc, #640]
	bl 0x0200c73c
	movs r0, #0
	ldr r1, [pc, #628]
	ldr r2, [pc, #632]
	bl 0x0200c73c
	movs r0, #10
	ldr r1, [pc, #620]
	ldr r2, [pc, #620]
	bl 0x0200c73c
	movs r1, #171
	movs r0, #9
	lsls r1, r1, #17
	ldr r2, [pc, #612]
	bl 0x0200c78c
	movs r1, #171
	movs r0, #9
	lsls r1, r1, #1
	ldr r2, [pc, #604]
	bl 0x0200c77c
	bl 0x0200c8ac
	movs r1, #148
	movs r0, #9
	lsls r1, r1, #1
	ldr r2, [pc, #588]
	bl 0x0200c774
	movs r1, #171
	movs r0, #0
	lsls r1, r1, #17
	ldr r2, [pc, #572]
	bl 0x0200c78c
	movs r1, #171
	movs r0, #0
	lsls r1, r1, #1
	ldr r2, [pc, #568]
	bl 0x0200c774
	movs r1, #171
	movs r0, #0
	lsls r1, r1, #1
	ldr r2, [pc, #552]
	bl 0x0200c77c
	movs r1, #159
	ldr r2, [pc, #544]
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200c77c
	movs r0, #9
	movs r1, #1
	bl 0x0200c794
	movs r0, #9
	movs r1, #1
	bl 0x0200c7b4
	movs r1, #192
	movs r2, #60
	lsls r1, r1, #8
	movs r0, #9
	bl 0x0200c7fc
	ldr r5, [pc, #516]
	adds r0, r5, #0
	bl 0x0200c7d4
	movs r0, #9
	movs r1, #0
	bl 0x0200c7e4
	movs r1, #147
	ldr r2, [pc, #500]
	lsls r1, r1, #1
	movs r0, #10
	bl 0x0200c77c
	movs r0, #40
	bl 0x0200c6f4
	movs r0, #10
	movs r1, #4
	bl 0x0200c79c
	movs r0, #10
	movs r1, #0
	bl 0x0200c7e4
	movs r1, #9
	movs r2, #0
	movs r0, #0
	bl 0x0200c7c4
	movs r0, #40
	bl 0x0200c6f4
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c7fc
	movs r0, #10
	movs r1, #0
	movs r2, #20
	bl 0x0200c7ec
	movs r0, #9
	ldr r1, [pc, #400]
	movs r2, #20
	bl 0x0200c80c
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200c7fc
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl 0x0200c7ec
	movs r0, #10
	movs r1, #4
	bl 0x0200c79c
	movs r0, #10
	movs r1, #0
	bl 0x0200c7e4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl 0x0200c814
	movs r0, #30
	bl 0x0200c6f4
	movs r0, #9
	movs r1, #0
	movs r2, #50
	bl 0x0200c7fc
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200c7fc
	movs r1, #192
	movs r2, #192
	movs r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c73c
	movs r0, #9
	ldr r1, [pc, #332]
	ldr r2, [pc, #336]
	bl 0x0200c77c
	movs r1, #224
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r0, #9
	movs r1, #0
	bl 0x0200c7e4
	movs r0, #10
	movs r1, #2
	bl 0x0200c7b4
	movs r0, #10
	movs r1, #0
	bl 0x0200c7e4
	movs r0, #9
	movs r1, #4
	bl 0x0200c79c
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r1, #128
	movs r2, #10
	lsls r1, r1, #6
	movs r0, #9
	adds r5, #8
	bl 0x0200c7fc
	adds r0, r5, #0
	bl 0x0200c7d4
	movs r1, #0
	movs r0, #9
	bl 0x0200c7dc
	movs r1, #151
	movs r0, #0
	lsls r1, r1, #1
	ldr r2, [pc, #220]
	bl 0x0200c77c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c7fc
	b .L_02000a10_0
.L_02000a10_1:
	movs r1, #1
	movs r0, #9
	bl 0x0200c7b4
	ldr r0, [pc, #216]
	bl 0x0200c7d4
	movs r0, #9
	movs r1, #0
	bl 0x0200c7dc
.L_02000a10_0:
	movs r0, #0
	movs r1, #0
	bl 0x0200c724
	cmp r0, #1
	beq .L_02000a10_1
	movs r1, #3
	movs r0, #9
	bl 0x0200c79c
	ldr r0, [pc, #188]
	bl 0x0200c7d4
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl 0x0200c7ec
	movs r0, #0
	movs r1, #3
	bl 0x0200c79c
	movs r1, #192
	movs r2, #192
	movs r0, #10
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c73c
	ldr r1, [pc, #152]
	ldr r2, [pc, #156]
	movs r0, #10
	bl 0x0200c774
	movs r0, #10
	bl 0x0200c6f4
	movs r0, #9
	ldr r1, [pc, #136]
	ldr r2, [pc, #136]
	bl 0x0200c77c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200c78c
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl 0x0200c78c
	movs r0, #10
	movs r1, #1
	bl 0x0200c794
	movs r0, #21
	movs r1, #2
	bl 0x0200c794
	movs r1, #5
	movs r0, #22
	bl 0x0200c794
	ldr r0, [pc, #92]
	bl 0x0200c6ec
	ldr r0, [pc, #88]
	bl 0x0200c6e4
	ldr r0, [pc, #88]
	bl 0x0200c6e4
	bl 0x0200c704
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0200d78a
	.4byte 0x00000101
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x037a0000
	.4byte 0x00000389
	.4byte 0x0000037a
	.4byte 0x00000e5c
	.4byte 0x00000346
	.4byte 0x00000121
	.4byte 0x00000373
	.4byte 0x00000e65
	.4byte 0x00000e66
	.4byte 0x00000129
	.4byte 0x000002ee
	.4byte 0x0000012f
	.4byte 0x0000087b
	.4byte 0x00000205
	.global Func_02000d5c
	.thumb_func
Func_02000d5c:
	push {r5, r6, r7, lr}
	movs r0, #196
	lsls r0, r0, #2
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_02000d5c_0
	b .L_02000d5c_1
.L_02000d5c_0:
	bl 0x0200c6fc
	movs r0, #131
	lsls r0, r0, #4
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_02000d5c_2
	movs r0, #11
	bl 0x0200c72c
	movs r1, #128
	adds r5, r0, #0
	movs r2, #128
	movs r0, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	ldr r6, [r5, #80]
	bl 0x0200c6bc
	movs r0, #141
	bl 0x0200c8b4
	adds r7, r5, #0
	movs r0, #40
	bl 0x0200c63c
	adds r7, #35
	movs r0, #145
	bl 0x0200c8b4
	ldrb r2, [r7]
	movs r3, #254
	ands r3, r2
	strb r3, [r7]
	ldrb r2, [r6, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	movs r2, #233
	strb r3, [r6, #9]
	movs r0, #11
	ldr r1, [pc, #212]
	lsls r2, r2, #18
	bl 0x0200c78c
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r5, #48]
	str r3, [r5, #52]
	movs r2, #240
	ldr r3, [r5, #12]
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r5, #12]
	str r3, [r5, #60]
	ldr r3, [pc, #188]
	movs r1, #172
	movs r2, #233
	lsls r1, r1, #1
	str r3, [r5, #68]
	movs r0, #11
	lsls r2, r2, #2
	bl 0x0200c77c
	ldrb r3, [r6, #9]
	movs r2, #12
	orrs r3, r2
	ldrb r2, [r7]
	strb r3, [r6, #9]
	movs r3, #1
	orrs r3, r2
	strb r3, [r7]
	movs r0, #40
	bl 0x0200c6f4
	ldr r0, [pc, #152]
	bl 0x0200c8b4
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #140]
	bl 0x0200c6bc
	bl 0x0200c6c4
	bl 0x0200c8ac
	movs r0, #131
	lsls r0, r0, #4
	bl 0x0200c6e4
.L_02000d5c_2:
	bl 0x02008ec4
	movs r0, #196
	lsls r0, r0, #2
	bl 0x0200c6e4
	ldr r0, [pc, #112]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_02000d5c_3
	ldr r0, [pc, #104]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_02000d5c_3
	movs r0, #195
	lsls r0, r0, #2
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_02000d5c_3
	movs r0, #0
	bl 0x0200c72c
	movs r2, #128
	ldr r3, [r0, #12]
	lsls r2, r2, #16
	cmp r3, r2
	ble .L_02000d5c_4
	ldr r5, [pc, #72]
	movs r0, #163
	lsls r0, r0, #1
	adds r1, r5, #0
	bl 0x02009a64
	movs r0, #0
	ldr r1, [pc, #60]
	adds r2, r5, #0
	bl 0x0200c77c
	b .L_02000d5c_5
.L_02000d5c_4:
	ldr r0, [pc, #56]
	ldr r1, [pc, #56]
	bl 0x02009a64
.L_02000d5c_5:
	movs r0, #195
	lsls r0, r0, #2
	bl 0x0200c6e4
.L_02000d5c_3:
	bl 0x0200c704
.L_02000d5c_1:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x01d90000
	.4byte 0x00006666
	.4byte 0x00000121
	.4byte 0x0000e666
	.4byte 0x00000837
	.4byte 0x00000841
	.4byte 0x00000396
	.4byte 0x00000123
	.4byte 0x0000014f
	.4byte 0x000003bd
	.global Func_02000ec4
	.thumb_func
Func_02000ec4:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #8
	movs r3, #57
	str r3, [sp, #4]
	movs r6, #21
	mov r8, r3
	movs r0, #29
	movs r1, #64
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl 0x0200c6ac
	movs r5, #58
	movs r0, #29
	movs r1, #64
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200c6ac
	movs r3, #22
	str r3, [sp, #0]
	movs r0, #29
	movs r1, #64
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200c6ac
	movs r6, #20
	movs r0, #29
	movs r1, #64
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200c6ac
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #28
	movs r1, #20
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl 0x0200c6ac
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000f38
	.thumb_func
Func_02000f38:
	push {r5, lr}
	ldr r0, [pc, #316]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_02000f38_0
	b .L_02000f38_1
.L_02000f38_0:
	bl 0x0200c6fc
	ldr r0, [pc, #304]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_02000f38_2
	movs r0, #12
	bl 0x0200c72c
	movs r1, #128
	adds r5, r0, #0
	movs r2, #128
	movs r0, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl 0x0200c6bc
	movs r0, #141
	bl 0x0200c8b4
	movs r0, #40
	bl 0x0200c63c
	movs r0, #145
	bl 0x0200c8b4
	movs r2, #202
	movs r0, #12
	ldr r1, [pc, #252]
	lsls r2, r2, #18
	bl 0x0200c78c
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r5, #48]
	str r3, [r5, #52]
	movs r2, #128
	ldr r3, [r5, #12]
	lsls r2, r2, #17
	adds r3, r3, r2
	str r3, [r5, #12]
	str r3, [r5, #60]
	movs r3, #128
	lsls r3, r3, #8
	movs r1, #145
	str r3, [r5, #68]
	ldr r2, [pc, #220]
	movs r0, #12
	lsls r1, r1, #1
	bl 0x0200c77c
	movs r0, #12
	movs r1, #1
	bl 0x0200c804
	movs r1, #129
	movs r2, #213
	lsls r2, r2, #2
	movs r0, #12
	lsls r1, r1, #1
	bl 0x0200c77c
	movs r0, #12
	movs r1, #2
	bl 0x0200c804
	movs r2, #218
	movs r1, #224
	lsls r2, r2, #2
	movs r0, #12
	bl 0x0200c77c
	movs r0, #40
	bl 0x0200c6f4
	ldr r0, [pc, #164]
	bl 0x0200c8b4
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #156]
	bl 0x0200c6bc
	bl 0x0200c6c4
	bl 0x0200c8ac
	ldr r0, [pc, #124]
	bl 0x0200c6e4
.L_02000f38_2:
	bl 0x020090a4
	ldr r0, [pc, #112]
	bl 0x0200c6e4
	ldr r0, [pc, #128]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_02000f38_3
	ldr r0, [pc, #124]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_02000f38_3
	movs r0, #195
	lsls r0, r0, #2
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_02000f38_3
	movs r0, #0
	bl 0x0200c72c
	movs r2, #128
	ldr r3, [r0, #12]
	lsls r2, r2, #16
	cmp r3, r2
	ble .L_02000f38_4
	ldr r1, [pc, #88]
	movs r0, #219
	bl 0x02009a64
	movs r0, #0
	movs r1, #179
	ldr r2, [pc, #80]
	bl 0x0200c77c
	b .L_02000f38_5
.L_02000f38_4:
	movs r1, #227
	lsls r1, r1, #2
	movs r0, #214
	bl 0x02009a64
	movs r0, #0
	movs r1, #219
	ldr r2, [pc, #64]
	bl 0x0200c77c
.L_02000f38_5:
	movs r0, #195
	lsls r0, r0, #2
	bl 0x0200c6e4
.L_02000f38_3:
	bl 0x0200c704
.L_02000f38_1:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000311
	.4byte 0x00000831
	.4byte 0x017d0000
	.4byte 0x00000341
	.4byte 0x00000121
	.4byte 0x0000e666
	.4byte 0x00000837
	.4byte 0x00000841
	.4byte 0x0000034b
	.4byte 0x0000033d
	.4byte 0x0000038f
	.global Func_020010a4
	.thumb_func
Func_020010a4:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	sub sp, #8
	movs r2, #15
	str r2, [sp, #0]
	mov r10, r2
	movs r5, #53
	movs r0, #29
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200c6ac
	movs r6, #14
	movs r0, #29
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200c6ac
	movs r3, #13
	str r3, [sp, #0]
	mov r8, r3
	movs r0, #29
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200c6ac
	movs r3, #52
	str r3, [sp, #4]
	movs r0, #26
	movs r1, #20
	movs r2, #2
	movs r3, #1
	str r6, [sp, #0]
	bl 0x0200c6ac
	mov r2, r8
	str r2, [sp, #0]
	movs r5, #54
	movs r0, #25
	movs r1, #21
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200c6ac
	mov r3, r10
	str r3, [sp, #0]
	movs r0, #25
	movs r1, #21
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200c6ac
	movs r0, #14
	movs r1, #53
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200c6ac
	mov r2, r10
	movs r3, #55
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #13
	movs r1, #55
	movs r2, #1
	movs r3, #1
	bl 0x0200c6ac
	sub sp, #-8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02001154
	.thumb_func
Func_02001154:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r0, [pc, #348]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_02001154_0
	b .L_02001154_1
.L_02001154_0:
	bl 0x0200c6fc
	ldr r0, [pc, #336]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_02001154_2
	movs r0, #13
	bl 0x0200c72c
	adds r6, r0, #0
	movs r0, #0
	bl 0x0200c72c
	movs r2, #35
	ldr r3, [r0, #80]
	mov r8, r0
	add r8, r2
	ldrb r5, [r3, #9]
	mov r3, r8
	ldrb r3, [r3]
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #11
	mov r10, r3
	bl 0x0200c6bc
	movs r0, #141
	bl 0x0200c8b4
	movs r0, #40
	bl 0x0200c63c
	movs r0, #145
	bl 0x0200c8b4
	movs r1, #3
	movs r0, #0
	bl 0x0200c804
	movs r0, #0
	bl 0x0200c72c
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r0, #13
	ldr r2, [pc, #240]
	bl 0x0200c78c
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r6, #48]
	str r3, [r6, #52]
	movs r2, #160
	ldr r3, [r6, #12]
	lsls r2, r2, #15
	adds r3, r3, r2
	str r3, [r6, #12]
	str r3, [r6, #60]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r6, #68]
	movs r1, #64
	ldr r2, [pc, #208]
	movs r0, #13
	bl 0x0200c77c
	movs r0, #40
	bl 0x0200c6f4
	ldr r0, [pc, #200]
	bl 0x0200c8b4
	movs r0, #1
	movs r1, #1
	ldr r2, [pc, #192]
	negs r1, r1
	negs r0, r0
	lsls r5, r5, #28
	bl 0x0200c6bc
	lsrs r5, r5, #30
	bl 0x0200c6c4
	bl 0x0200c8ac
	ldr r0, [pc, #156]
	bl 0x0200c6e4
	movs r0, #0
	adds r1, r5, #0
	bl 0x0200c804
	movs r0, #0
	bl 0x0200c72c
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	mov r2, r8
	mov r3, r10
	strb r3, [r2]
.L_02001154_2:
	bl 0x020092f0
	ldr r0, [pc, #112]
	bl 0x0200c6e4
	ldr r0, [pc, #128]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_02001154_3
	ldr r0, [pc, #124]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_02001154_3
	movs r0, #195
	lsls r0, r0, #2
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_02001154_3
	movs r0, #0
	bl 0x0200c72c
	ldr r2, [pc, #100]
	ldr r3, [r0, #16]
	cmp r3, r2
	bgt .L_02001154_4
	ldr r1, [pc, #96]
	movs r0, #62
	bl 0x02009a64
	movs r0, #0
	movs r1, #27
	ldr r2, [pc, #88]
	bl 0x0200c77c
	b .L_02001154_5
.L_02001154_4:
	ldr r1, [pc, #84]
	movs r0, #75
	bl 0x02009a64
	movs r0, #0
	movs r1, #67
	ldr r2, [pc, #76]
	bl 0x0200c77c
.L_02001154_5:
	movs r0, #195
	lsls r0, r0, #2
	bl 0x0200c6e4
.L_02001154_3:
	bl 0x0200c704
.L_02001154_1:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00000312
	.4byte 0x00000832
	.4byte 0x02bf0000
	.4byte 0x000002bf
	.4byte 0x00000121
	.4byte 0x0000e666
	.4byte 0x00000837
	.4byte 0x00000841
	.4byte 0x02b4ffff
	.4byte 0x0000029d
	.4byte 0x00000273
	.4byte 0x000002cb
	.4byte 0x000002f5
	.global Func_020012f0
	.thumb_func
Func_020012f0:
	push {r5, r6, lr}
	sub sp, #8
	movs r3, #3
	str r3, [sp, #0]
	movs r5, #42
	movs r0, #29
	movs r1, #22
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200c6ac
	movs r6, #2
	movs r0, #29
	movs r1, #21
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200c6ac
	movs r3, #4
	str r3, [sp, #0]
	movs r0, #29
	movs r1, #21
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200c6ac
	movs r3, #43
	str r3, [sp, #4]
	movs r0, #23
	movs r1, #20
	movs r2, #3
	movs r3, #1
	str r6, [sp, #0]
	bl 0x0200c6ac
	sub sp, #-8
	pop {r5, r6}
.L_02001342:
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001348
	.thumb_func
Func_02001348:
	push {r5, lr}
	ldr r0, [pc, #276]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_02001348_0
	b .L_02001348_1
.L_02001348_0:
	bl 0x0200c6fc
	ldr r0, [pc, #264]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_02001348_2
	movs r0, #14
	bl 0x0200c72c
	movs r1, #128
	adds r5, r0, #0
	movs r2, #128
	movs r0, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl 0x0200c6bc
	movs r0, #141
	bl 0x0200c8b4
	movs r0, #40
	bl 0x0200c63c
	movs r0, #145
	bl 0x0200c8b4
	movs r1, #237
	movs r0, #14
	lsls r1, r1, #17
	ldr r2, [pc, #208]
	bl 0x0200c78c
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #48]
	str r3, [r5, #52]
	movs r2, #144
	ldr r3, [r5, #12]
	lsls r2, r2, #15
	adds r3, r3, r2
	str r3, [r5, #12]
	str r3, [r5, #60]
	movs r3, #128
	lsls r3, r3, #8
	movs r1, #216
	str r3, [r5, #68]
	lsls r1, r1, #1
	ldr r2, [pc, #176]
	movs r0, #14
	bl 0x0200c77c
	movs r0, #40
	bl 0x0200c6f4
	ldr r0, [pc, #168]
	bl 0x0200c8b4
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #156]
	bl 0x0200c6bc
	bl 0x0200c6c4
	bl 0x0200c8ac
	ldr r0, [pc, #128]
	bl 0x0200c6e4
.L_02001348_2:
	bl 0x02009498
	ldr r0, [pc, #112]
	bl 0x0200c6e4
	ldr r0, [pc, #132]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_02001348_3
	ldr r0, [pc, #124]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_02001348_3
	movs r0, #195
	lsls r0, r0, #2
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_02001348_3
	movs r0, #0
	bl 0x0200c72c
	ldr r2, [pc, #100]
	ldr r3, [r0, #16]
	cmp r3, r2
	bgt .L_02001348_4
	movs r0, #206
	movs r1, #140
	lsls r0, r0, #1
	lsls r1, r1, #3
	bl 0x02009a64
	movs r1, #207
	movs r0, #0
	lsls r1, r1, #1
	ldr r2, [pc, #80]
	bl 0x0200c77c
	b .L_02001348_5
.L_02001348_4:
	ldr r0, [pc, #76]
	ldr r1, [pc, #76]
	bl 0x02009a64
	movs r0, #0
	ldr r1, [pc, #72]
	ldr r2, [pc, #76]
	bl 0x0200c77c
.L_02001348_5:
	movs r0, #195
	lsls r0, r0, #2
	bl 0x0200c6e4
.L_02001348_3:
	bl 0x0200c704
.L_02001348_1:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000313
	.4byte 0x00000833
	.4byte 0x047b0000
	.4byte 0x0000047b
	.4byte 0x00000121
	.4byte 0x0000e666
	.4byte 0x00000837
	.4byte 0x00000841
	.4byte 0x0479ffff
	.4byte 0x0000042c
	.4byte 0x000001bd
	.4byte 0x00000494
	.4byte 0x000001bf
	.4byte 0x000004cb
	.global Func_02001498
	.thumb_func
Func_02001498:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #8
	movs r3, #71
	str r3, [sp, #4]
	movs r5, #26
	mov r8, r3
	movs r0, #29
	movs r1, #20
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200c6ac
	movs r6, #70
	movs r0, #29
	movs r1, #20
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200c6ac
	movs r5, #27
	movs r0, #29
	movs r1, #20
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200c6ac
	movs r3, #28
	str r3, [sp, #0]
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #28
	movs r1, #21
	movs r2, #1
	movs r3, #1
	bl 0x0200c6ac
	movs r3, #72
	str r3, [sp, #4]
	movs r0, #28
	movs r1, #22
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200c6ac
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_0200150c
	.thumb_func
Func_0200150c:
	push {r5, lr}
	bl 0x0200c6fc
	movs r1, #131
	movs r0, #0
	lsls r1, r1, #1
	ldr r2, [pc, #200]
	bl 0x0200c77c
	movs r1, #131
	movs r0, #20
	lsls r1, r1, #17
	ldr r2, [pc, #192]
	bl 0x0200c78c
	movs r1, #131
	movs r0, #20
	lsls r1, r1, #1
	ldr r2, [pc, #184]
	bl 0x0200c774
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
.L_0200153e:
	lsls r2, r2, #9
	bl 0x0200c73c
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x0200c7a4
	movs r1, #141
	ldr r2, [pc, #156]
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200c77c
	movs r0, #20
	movs r1, #1
	bl 0x0200c794
	movs r0, #0
	movs r1, #4
	movs r2, #0
	bl 0x0200c7a4
	movs r2, #0
	movs r1, #20
	movs r0, #0
	bl 0x0200c7c4
	bl 0x0200c8ac
	movs r0, #30
	bl 0x0200c6f4
	movs r0, #0
	movs r1, #2
	bl 0x0200c7b4
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #20
	bl 0x0200c80c
	ldr r5, [pc, #92]
	adds r0, r5, #0
	bl 0x0200c7d4
	movs r1, #0
	movs r0, #20
	bl 0x0200c7e4
	movs r0, #20
	bl 0x0200c6f4
	movs r1, #0
	movs r0, #20
	bl 0x0200c7f4
	adds r5, #4
	movs r1, #2
	movs r0, #20
	bl 0x0200c7b4
	adds r0, r5, #0
	bl 0x0200c7d4
	movs r2, #20
	movs r0, #20
	movs r1, #0
	bl 0x0200c7ec
	ldr r1, [pc, #40]
	movs r0, #20
	bl 0x0200c75c
	ldr r0, [pc, #36]
	bl 0x0200c6e4
	bl 0x0200c704
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x032a
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0325
	.2byte 0x0339
	.2byte 0x0000
	.4byte 0x00000357
	.4byte 0x00000e67
	.4byte 0x0200c8c0
	.4byte 0x00000835
	.global Func_02001600
	.thumb_func
Func_02001600:
	push {lr}
	ldr r0, [pc, #108]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_02001600_0
	ldr r0, [pc, #100]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_02001600_0
	bl 0x0200c6fc
	ldr r0, [pc, #92]
	bl 0x0200c7d4
	movs r0, #22
	movs r1, #0
	movs r2, #20
	bl 0x0200c7ec
	movs r0, #0
	ldr r1, [pc, #76]
	movs r2, #40
	bl 0x0200c80c
	movs r1, #191
	movs r0, #0
	lsls r1, r1, #1
	ldr r2, [pc, #68]
	bl 0x0200c77c
	movs r2, #0
	movs r0, #0
	movs r1, #22
	bl 0x0200c7bc
	movs r1, #2
	movs r0, #0
	bl 0x0200c7b4
	movs r0, #30
	bl 0x0200c6f4
	movs r0, #22
	movs r1, #0
	bl 0x0200c7e4
	ldr r0, [pc, #12]
	bl 0x0200c6e4
	bl 0x0200c704
.L_02001600_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000836
	.4byte 0x00000837
	.4byte 0x00000e6c
	.4byte 0x00000101
	.4byte 0x0000026b
	.global Func_02001684
	.thumb_func
Func_02001684:
	push {lr}
	ldr r0, [pc, #56]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_02001684_0
	ldr r0, [pc, #48]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_02001684_0
	bl 0x0200c6fc
	movs r1, #2
	movs r0, #22
	bl 0x0200c7b4
	movs r0, #20
	bl 0x0200c6f4
	ldr r0, [pc, #24]
	bl 0x0200c7d4
	bl 0x0200973c
	bl 0x0200c704
.L_02001684_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000837
	.4byte 0x00000836
	.4byte 0x00000e71
	.global Func_020016cc
	.thumb_func
Func_020016cc:
	push {lr}
	ldr r0, [pc, #92]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_020016cc_0
	bl 0x0200c6fc
	movs r2, #0
	movs r1, #0
	movs r0, #22
	bl 0x0200c7bc
	movs r0, #20
	bl 0x0200c6f4
	ldr r0, [pc, #64]
	bl 0x0200c7d4
	movs r0, #22
	movs r1, #0
	bl 0x0200c7e4
	movs r1, #224
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200c7fc
	bl 0x0200c704
	b .L_020016cc_1
.L_020016cc_0:
	ldr r0, [pc, #36]
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_020016cc_1
	bl 0x0200c6fc
	ldr r0, [pc, #28]
	bl 0x0200c7d4
	bl 0x0200973c
	bl 0x0200c704
.L_020016cc_1:
	pop {r0}
	bx r0
	.4byte 0x00000841
	.4byte 0x00000ed0
	.4byte 0x00000837
	.4byte 0x00000e6e
	.global Func_0200173c
	.thumb_func
Func_0200173c:
	push {r5, lr}
	movs r1, #0
	movs r0, #22
	bl 0x0200c7dc
	movs r0, #0
	movs r1, #22
	movs r2, #0
	bl 0x0200c7c4
	movs r0, #0
	movs r1, #0
	movs r5, #0
	bl 0x0200c724
	cmp r0, #0
	bne .L_0200173c_0
	ldr r0, [pc, #180]
	bl 0x0200c7d4
	movs r5, #1
	b .L_0200173c_1
.L_0200173c_0:
	ldr r0, [pc, #172]
	bl 0x0200c7d4
.L_0200173c_1:
	movs r0, #20
	bl 0x0200c6f4
	movs r2, #40
	movs r0, #22
	movs r1, #0
	bl 0x0200c7ec
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #1
	bl 0x0200c814
	movs r0, #21
	movs r1, #3
	bl 0x0200c794
	movs r1, #1
	movs r0, #22
	bl 0x0200c794
	movs r0, #40
	bl 0x0200c6f4
	movs r1, #0
	movs r0, #22
	movs r2, #0
	bl 0x0200c7bc
	movs r0, #20
	bl 0x0200c6f4
	movs r0, #22
	movs r1, #3
	bl 0x0200c79c
	cmp r5, #0
	beq .L_0200173c_2
	ldr r0, [pc, #96]
	bl 0x0200c7d4
	b .L_0200173c_3
.L_0200173c_2:
	ldr r0, [pc, #92]
	bl 0x0200c7d4
.L_0200173c_3:
	movs r0, #22
	movs r1, #0
	bl 0x0200c7e4
	movs r0, #22
	movs r1, #2
	bl 0x0200c794
	movs r0, #0
	bl 0x0200c72c
	cmp r0, #0
	beq .L_0200173c_4
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #22
	bl 0x0200c764
.L_0200173c_4:
	movs r0, #22
	bl 0x0200c784
	movs r2, #0
	movs r0, #22
	movs r1, #0
	bl 0x0200c78c
	movs r0, #1
	movs r1, #1
	bl 0x0200c714
	ldr r0, [pc, #24]
	bl 0x0200c6e4
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000ee5
	.4byte 0x00000ee6
	.4byte 0x00000e70
	.4byte 0x00000ee7
	.4byte 0x00000837
	.global Func_02001828
	.thumb_func
Func_02001828:
	push {r5, r6, lr}
	ldr r0, [pc, #328]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_02001828_0
	b .L_02001828_1
.L_02001828_0:
	bl 0x0200c6fc
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #22
	bl 0x0200c814
	ldr r5, [pc, #304]
	adds r0, r5, #0
	bl 0x0200c7d4
	movs r0, #22
	movs r1, #0
	bl 0x0200c7e4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #20
	bl 0x0200c80c
	movs r1, #128
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #7
	bl 0x0200c7fc
	ldr r0, [pc, #268]
	ldr r1, [pc, #272]
	bl 0x0200c81c
	movs r0, #128
	movs r1, #1
	movs r2, #147
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #18
	bl 0x0200c824
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	movs r0, #22
	lsls r1, r1, #10
	bl 0x0200c73c
	ldr r1, [pc, #236]
	movs r0, #22
	bl 0x0200c75c
	movs r2, #0
	movs r1, #22
	movs r0, #0
	bl 0x0200c7c4
	movs r0, #30
	bl 0x0200c6f4
	ldr r1, [pc, #216]
	movs r0, #22
	bl 0x0200c744
	movs r1, #0
	movs r0, #22
	bl 0x0200c7e4
	movs r0, #22
	bl 0x0200c72c
	movs r6, #128
	lsls r6, r6, #9
	movs r1, #1
	str r6, [r0, #28]
	movs r0, #22
	bl 0x0200c7b4
	movs r0, #20
	bl 0x0200c6f4
	movs r1, #0
	movs r0, #22
	bl 0x0200c7f4
	movs r0, #40
	bl 0x0200c6f4
	adds r5, #5
	movs r1, #1
	movs r0, #22
	bl 0x0200c7b4
	adds r0, r5, #0
	bl 0x0200c7d4
	movs r2, #20
	movs r0, #22
	movs r1, #0
	bl 0x0200c7ec
	movs r0, #0
	movs r1, #3
	bl 0x0200c79c
	movs r0, #22
	movs r1, #3
	bl 0x0200c79c
	movs r0, #22
	movs r1, #0
	bl 0x0200c7e4
	movs r2, #128
	movs r0, #22
	adds r1, r6, #0
	lsls r2, r2, #8
	bl 0x0200c73c
	movs r0, #22
	movs r1, #2
	bl 0x0200c794
	movs r0, #0
	bl 0x0200c72c
	cmp r0, #0
	beq .L_02001828_2
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #22
	bl 0x0200c764
.L_02001828_2:
	movs r0, #22
	bl 0x0200c784
	movs r2, #0
	movs r0, #22
	movs r1, #0
	bl 0x0200c78c
	movs r0, #1
	movs r1, #1
	bl 0x0200c714
	movs r0, #21
	movs r1, #3
	bl 0x0200c794
	ldr r0, [pc, #16]
	bl 0x0200c6e4
	bl 0x0200c704
.L_02001828_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000837
	.4byte 0x00000e74
	.4byte 0x00006666
	.4byte 0x00000ccc
	.4byte 0x0200c934
	.4byte 0x0200c984
	.global Func_0200198c
	.thumb_func
Func_0200198c:
	push {lr}
	bl 0x0200c6fc
	movs r0, #0
	bl 0x0200c72c
	cmp r0, #0
	beq .L_0200198c_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #22
	bl 0x0200c78c
.L_0200198c_0:
	movs r1, #128
	movs r2, #128
	movs r0, #22
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c73c
	movs r0, #22
	ldr r1, [pc, #156]
	ldr r2, [pc, #156]
	bl 0x0200c77c
	movs r2, #0
	movs r1, #0
	movs r0, #22
	bl 0x0200c7c4
	movs r0, #30
	bl 0x0200c6f4
	ldr r0, [pc, #140]
	bl 0x0200c7d4
	movs r0, #22
	movs r1, #0
	bl 0x0200c7e4
	movs r2, #0
	movs r1, #22
	movs r0, #0
	bl 0x0200c7bc
	movs r0, #10
	bl 0x0200c6f4
	movs r1, #1
	movs r0, #0
	bl 0x0200c7b4
	movs r0, #20
	bl 0x0200c6f4
	movs r1, #128
	movs r2, #0
	movs r0, #22
	lsls r1, r1, #7
	bl 0x0200c7fc
	movs r0, #22
	movs r1, #0
	bl 0x0200c7e4
	movs r0, #22
	movs r1, #2
	bl 0x0200c794
	movs r0, #0
	bl 0x0200c72c
	cmp r0, #0
	beq .L_0200198c_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #22
	bl 0x0200c764
.L_0200198c_1:
	movs r0, #22
	bl 0x0200c784
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl 0x0200c78c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	ldr r2, [pc, #24]
	bl 0x0200c77c
	bl 0x0200c704
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000119
	.4byte 0x000001fb
	.4byte 0x00000e7b
	.4byte 0x00000205
	.global Func_02001a64
	.thumb_func
Func_02001a64:
	push {r5, r6, lr}
	adds r5, r0, #0
	movs r0, #0
	adds r6, r1, #0
	bl 0x0200c72c
	cmp r0, #0
	beq .L_02001a64_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #22
	bl 0x0200c78c
.L_02001a64_0:
	movs r1, #128
	movs r2, #128
	movs r0, #22
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c73c
	movs r0, #22
	adds r1, r5, #0
	adds r2, r6, #0
	bl 0x0200c77c
	movs r2, #0
	movs r1, #22
	movs r0, #0
	bl 0x0200c7c4
	movs r0, #20
	bl 0x0200c6f4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #0
	bl 0x0200c814
	movs r0, #40
	bl 0x0200c6f4
	ldr r0, [pc, #92]
	bl 0x0200c7d4
	movs r0, #22
	movs r1, #0
	bl 0x0200c7e4
	movs r0, #22
	movs r1, #2
	bl 0x0200c7b4
	movs r0, #22
	movs r1, #0
	bl 0x0200c7e4
	movs r0, #0
	movs r1, #3
	bl 0x0200c79c
	movs r0, #22
	movs r1, #2
	bl 0x0200c794
	movs r0, #0
	bl 0x0200c72c
	cmp r0, #0
	beq .L_02001a64_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #22
	bl 0x0200c764
.L_02001a64_1:
	movs r0, #22
	bl 0x0200c784
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl 0x0200c78c
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000e7d
	.global Func_02001b18
	.thumb_func
Func_02001b18:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, [pc, #1008]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_02001b18_0
	b 0x0200a07e
.L_02001b18_0:
	bl 0x0200c6fc
	ldr r0, [pc, #996]
	bl 0x0200c70c
	bl 0x0200c550
	movs r0, #1
	bl 0x0200c63c
	movs r0, #141
	bl 0x0200c8b4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200c6bc
	movs r0, #30
	bl 0x0200c6f4
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200c6bc
	movs r0, #145
	bl 0x0200c8b4
	movs r0, #30
	bl 0x0200c6f4
	movs r0, #0
	bl 0x0200c72c
	cmp r0, #0
	beq .L_02001b18_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #22
	bl 0x0200c78c
.L_02001b18_1:
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200c73c
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	movs r0, #22
	lsls r1, r1, #10
	bl 0x0200c73c
	ldr r1, [pc, #880]
	movs r0, #0
	bl 0x0200c744
	ldr r1, [pc, #876]
	movs r0, #22
	bl 0x0200c75c
	movs r0, #0
	bl 0x0200c74c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200c80c
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #1
	movs r2, #30
	bl 0x0200c80c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl 0x0200c6bc
	movs r0, #145
	bl 0x0200c8b4
	movs r0, #40
	bl 0x0200c6f4
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #11
	bl 0x0200c6bc
	movs r0, #145
	bl 0x0200c8b4
	movs r0, #20
	bl 0x0200c6f4
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200c814
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #22
	bl 0x0200c814
	movs r0, #40
	bl 0x0200c6f4
	movs r0, #32
	movs r1, #5
	bl 0x0200c794
	movs r0, #33
	movs r1, #5
	bl 0x0200c794
	movs r0, #30
	movs r1, #8
	bl 0x0200c794
	movs r1, #8
	movs r0, #29
	bl 0x0200c794
	movs r0, #30
	bl 0x0200c72c
	ldr r3, [pc, #720]
	movs r1, #2
	str r3, [r0, #24]
	movs r0, #32
	bl 0x0200c804
	movs r0, #33
	movs r1, #2
	bl 0x0200c804
	movs r0, #30
	movs r1, #3
	bl 0x0200c804
	movs r1, #3
	movs r0, #29
	bl 0x0200c804
	ldr r0, [pc, #688]
	bl 0x0200c7d4
	movs r0, #28
	movs r1, #0
	movs r2, #20
	bl 0x0200c7ec
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #192
	movs r2, #20
	movs r0, #22
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl 0x0200c81c
	movs r0, #224
	movs r1, #1
	lsls r0, r0, #15
	negs r1, r1
	ldr r2, [pc, #632]
	movs r3, #1
	bl 0x0200c824
	bl 0x0200c82c
	movs r5, #0
	movs r0, #32
	bl 0x0200c72c
	bl 0x0200c2bc
	movs r0, #33
	bl 0x0200c72c
	bl 0x0200c2bc
	movs r0, #30
	bl 0x0200c72c
	bl 0x0200c2bc
	movs r0, #29
	bl 0x0200c72c
	bl 0x0200c2bc
	adds r5, #1
	movs r0, #1
	bl 0x0200c63c
	cmp r5, #39
.L_02001cf4:
	bls 0x02009cc2
	ldr r5, [pc, #572]
	ldr r2, [pc, #572]
	ldr r7, [pc, #576]
	movs r6, #0
	movs r1, #200
	str r6, [r2]
	lsls r1, r1, #4
	str r6, [r5]
	adds r0, r7, #0
	mov r9, r2
	bl 0x0200c644
	ldr r3, [pc, #560]
	movs r1, #200
	mov r11, r3
	lsls r1, r1, #4
	mov r0, r11
	bl 0x0200c644
	movs r0, #40
	bl 0x0200c6f4
	movs r2, #1
	str r2, [r5]
	movs r0, #30
	mov r10, r2
	bl 0x0200c6f4
	movs r1, #228
	movs r2, #145
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r0, #19
	bl 0x0200c78c
	movs r0, #19
	bl 0x0200c72c
	movs r2, #128
	ldr r3, [r0, #12]
	lsls r2, r2, #15
	adds r3, r3, r2
	str r3, [r0, #12]
	str r3, [r0, #60]
	ldr r1, [pc, #500]
	ldr r2, [pc, #500]
	movs r0, #19
	bl 0x0200c73c
	movs r0, #145
	bl 0x0200c8b4
	ldr r2, [pc, #492]
	movs r0, #19
	movs r1, #114
	bl 0x0200c76c
	movs r0, #19
	movs r1, #2
	bl 0x0200c794
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200c6bc
	movs r0, #145
	bl 0x0200c8b4
	movs r0, #19
	ldr r1, [pc, #444]
	ldr r2, [pc, #452]
	str r6, [r5]
	bl 0x0200c73c
	movs r2, #150
	lsls r2, r2, #1
	movs r0, #19
	movs r1, #114
	bl 0x0200c76c
	movs r0, #19
	movs r1, #2
	bl 0x0200c794
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl 0x0200c6bc
	movs r0, #145
	bl 0x0200c8b4
	movs r3, #2
	str r3, [r5]
	movs r0, #19
	ldr r1, [pc, #384]
	ldr r2, [pc, #384]
	mov r8, r3
	bl 0x0200c73c
	ldr r2, [pc, #380]
	movs r0, #19
	movs r1, #114
	bl 0x0200c76c
	movs r0, #19
	movs r1, #2
	bl 0x0200c794
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200c6bc
	movs r0, #145
	bl 0x0200c8b4
	movs r0, #19
	ldr r1, [pc, #336]
	ldr r2, [pc, #340]
	str r6, [r5]
	bl 0x0200c73c
	movs r2, #150
	lsls r2, r2, #1
	movs r0, #19
	movs r1, #114
	bl 0x0200c76c
	movs r0, #19
	movs r1, #2
	bl 0x0200c794
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl 0x0200c6bc
	movs r0, #145
	bl 0x0200c8b4
	mov r2, r8
	str r2, [r5]
	movs r0, #19
	ldr r1, [pc, #272]
	ldr r2, [pc, #276]
	bl 0x0200c73c
	ldr r2, [pc, #272]
	movs r0, #19
	movs r1, #114
	bl 0x0200c76c
	movs r0, #19
	movs r1, #2
	bl 0x0200c794
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #10
	lsls r0, r0, #10
	bl 0x0200c6bc
	movs r0, #145
	bl 0x0200c8b4
	mov r3, r10
	str r3, [r5]
	movs r0, #20
	bl 0x0200c6f4
	movs r1, #129
	movs r0, #32
	lsls r1, r1, #1
	bl 0x0200c814
	movs r0, #32
	movs r1, #2
	bl 0x0200c7b4
	movs r0, #31
	movs r1, #0
	bl 0x0200c7e4
	movs r1, #128
	movs r2, #0
	movs r0, #33
	lsls r1, r1, #1
	bl 0x0200c80c
	movs r0, #33
	movs r1, #2
	bl 0x0200c7b4
	movs r2, #40
	movs r0, #28
	movs r1, #0
	bl 0x0200c7ec
	movs r1, #129
	movs r0, #30
	lsls r1, r1, #1
	bl 0x0200c814
	movs r0, #30
	movs r1, #2
	bl 0x0200c7b4
	movs r0, #30
	movs r1, #0
	bl 0x0200c7e4
	mov r3, r9
	mov r2, r10
	str r2, [r3]
	movs r1, #1
	movs r0, #29
	bl 0x0200c794
	movs r0, #1
	bl 0x0200c63c
	movs r0, #29
	movs r1, #0
	bl 0x0200c7cc
	movs r0, #29
	ldr r1, [pc, #120]
	movs r2, #20
	bl 0x0200c80c
	movs r1, #128
	movs r0, #29
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200c7fc
	movs r0, #29
	movs r1, #0
	movs r2, #20
	bl 0x0200c7fc
	movs r1, #128
	movs r0, #29
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200c7fc
	movs r1, #128
	movs r0, #29
	lsls r1, r1, #7
	movs r2, #40
	bl 0x0200c7fc
	movs r1, #128
	movs r2, #0
	movs r0, #29
	b .L_02001cf4_0
	.2byte 0x0838
	.2byte 0x0000
	.2byte 0xd4b0
	.2byte 0x0200
	.2byte 0xca00
	.2byte 0x0200
	.2byte 0xca3c
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0x0e7f
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x014b
	.4byte 0x0200d7fc
	.4byte 0x0200d7f8
	.4byte 0x0200c56d
	.4byte 0x0200c5a9
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x0000014d
	.4byte 0x00003333
	.4byte 0x00000105
.L_02001cf4_0:
	lsls r1, r1, #1
	bl 0x0200c80c
	movs r0, #29
	movs r1, #2
	bl 0x0200c7b4
	movs r2, #40
	movs r0, #29
	movs r1, #4
	bl 0x0200c7a4
	movs r1, #9
	movs r0, #29
	bl 0x0200c794
	movs r0, #10
	bl 0x0200c6f4
	movs r1, #0
	movs r2, #20
	movs r0, #29
	bl 0x0200c7ec
	ldr r0, [pc, #260]
	bl 0x0200c8b4
	movs r0, #1
	movs r1, #1
	ldr r2, [pc, #256]
	negs r0, r0
	negs r1, r1
	bl 0x0200c6bc
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl 0x0200c81c
	movs r0, #168
	movs r1, #1
	movs r2, #141
	movs r3, #1
	lsls r0, r0, #15
	negs r1, r1
	lsls r2, r2, #18
	bl 0x0200c824
	bl 0x0200c82c
	bl 0x0200c8ac
	movs r2, #0
	movs r1, #0
	movs r0, #22
	bl 0x0200c7bc
	movs r0, #20
	bl 0x0200c6f4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #22
	bl 0x0200c814
	movs r0, #30
	bl 0x0200c6f4
	adds r0, r7, #0
	bl 0x0200c64c
	mov r0, r11
	bl 0x0200c64c
	movs r0, #22
	movs r1, #0
	bl 0x0200c7e4
	movs r2, #0
	movs r1, #22
	movs r0, #0
	bl 0x0200c7bc
	movs r0, #20
	bl 0x0200c6f4
	bl 0x0200c560
	movs r0, #0
	movs r1, #3
	bl 0x0200c794
	movs r1, #3
	movs r0, #22
	bl 0x0200c79c
	movs r0, #20
	bl 0x0200c6f4
	movs r0, #22
	movs r1, #2
	bl 0x0200c794
	movs r0, #0
	bl 0x0200c72c
	cmp r0, #0
	beq .L_02001cf4_1
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #22
	bl 0x0200c764
.L_02001cf4_1:
	movs r0, #22
	bl 0x0200c784
	movs r1, #0
	movs r2, #0
	movs r0, #22
	bl 0x0200c78c
	movs r0, #31
	bl 0x0200c734
	movs r0, #28
	bl 0x0200c734
	movs r0, #30
	bl 0x0200c734
	movs r0, #29
	bl 0x0200c734
	movs r0, #32
	bl 0x0200c734
	movs r0, #33
	bl 0x0200c734
	ldr r0, [pc, #32]
	bl 0x0200c6e4
	bl 0x0200c704
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000121
	.4byte 0x0000e666
	.4byte 0x00000838
	.global Func_0200209c
	.thumb_func
Func_0200209c:
	push {lr}
	adds r3, r0, #0
	adds r3, #84
	ldrb r3, [r3]
	adds r2, r1, #0
	movs r1, #15
.L_020020a8:
	ands r1, r3
	cmp r1, #1
	bne .L_020020a8_0
	ldr r0, [r0, #80]
	subs r4, r2, #1
	mov r12, r0
	cmp r2, #0
	bne .L_020020a8_1
	ldr r3, [pc, #56]
	ldr r3, [r3]
	ldr r2, [pc, #56]
	lsrs r3, r3, #1
	ands r3, r1
	ldrb r4, [r2, r3]
.L_020020a8_1:
	mov r3, r12
	adds r3, #39
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_020020a8_2
	mov r0, r12
	adds r0, #40
	adds r1, r3, #0
.L_020020a8_4:
	ldmia r0!, {r2}
	cmp r2, #0
	beq .L_020020a8_3
	ldr r3, [r2, #16]
	cmp r3, #0
	beq .L_020020a8_3
	strb r4, [r2, #5]
.L_020020a8_3:
	subs r1, #1
	cmp r1, #0
	bne .L_020020a8_4
.L_020020a8_2:
	mov r2, r12
	adds r2, #37
	movs r3, #1
	strb r3, [r2]
.L_020020a8_0:
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.4byte 0x0200c8bc
	.global Func_020020fc
	.thumb_func
Func_020020fc:
	push {lr}
	ldr r3, [pc, #120]
	ldr r2, [pc, #120]
	ldr r3, [r3]
	ldr r2, [r2]
	lsrs r3, r2
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	beq .L_020020fc_0
	movs r0, #32
	bl 0x0200c72c
	movs r1, #1
	bl 0x0200a09c
	movs r0, #33
	bl 0x0200c72c
	movs r1, #1
	bl 0x0200a09c
	movs r0, #30
	bl 0x0200c72c
	movs r1, #1
	bl 0x0200a09c
	movs r0, #29
	bl 0x0200c72c
	movs r1, #1
	bl 0x0200a09c
	b .L_020020fc_1
.L_020020fc_0:
	movs r0, #32
	bl 0x0200c72c
	movs r1, #8
	bl 0x0200a09c
	movs r0, #33
	bl 0x0200c72c
	movs r1, #8
	bl 0x0200a09c
	movs r0, #30
	bl 0x0200c72c
	movs r1, #8
	bl 0x0200a09c
	movs r0, #29
	bl 0x0200c72c
	movs r1, #8
	bl 0x0200a09c
.L_020020fc_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e40
	.4byte 0x0200d7fc
	.global Func_02002180
	.thumb_func
Func_02002180:
	push {r5, r6, lr}
	ldr r0, [pc, #1012]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_02002180_0
	b 0x0200a87e
.L_02002180_0:
	bl 0x0200c6fc
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #16
	ldr r2, [pc, #992]
	bl 0x0200c78c
	movs r1, #128
	movs r2, #0
	movs r0, #10
	lsls r1, r1, #6
	bl 0x0200c7fc
	movs r1, #5
	movs r0, #10
	bl 0x0200c794
	movs r0, #10
	bl 0x0200c72c
	adds r5, r0, #0
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	ldr r6, [pc, #952]
	adds r0, #60
	adds r5, #100
	strh r0, [r5]
	adds r1, r6, #0
	movs r0, #10
	bl 0x0200c744
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #16
	ldr r2, [pc, #936]
	bl 0x0200c78c
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #227
	movs r0, #24
	lsls r1, r1, #16
	ldr r2, [pc, #904]
	bl 0x0200c78c
	movs r1, #128
	movs r2, #0
	movs r0, #24
	lsls r1, r1, #7
	bl 0x0200c7fc
	movs r1, #6
	movs r0, #24
	bl 0x0200c794
	movs r0, #24
	bl 0x0200c72c
	adds r5, r0, #0
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	adds r5, #100
	adds r0, #60
	strh r0, [r5]
	adds r1, r6, #0
	movs r0, #24
	bl 0x0200c744
	movs r1, #250
	movs r0, #25
	lsls r1, r1, #16
	ldr r2, [pc, #840]
	bl 0x0200c78c
	movs r1, #128
	movs r2, #0
	movs r0, #25
	lsls r1, r1, #7
	bl 0x0200c7fc
	movs r1, #6
	movs r0, #25
	bl 0x0200c794
	movs r0, #25
	bl 0x0200c72c
	adds r5, r0, #0
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	adds r5, #100
	adds r0, #60
	strh r0, [r5]
	adds r1, r6, #0
	movs r0, #25
	bl 0x0200c744
	movs r1, #227
	movs r0, #26
	lsls r1, r1, #16
	ldr r2, [pc, #784]
	bl 0x0200c78c
	movs r1, #192
	movs r0, #26
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #243
	movs r0, #23
	lsls r1, r1, #16
	ldr r2, [pc, #764]
	bl 0x0200c78c
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #23
	bl 0x0200c7fc
	movs r0, #23
	bl 0x0200c72c
	movs r1, #0
	bl 0x0200c6b4
	movs r0, #3
	bl 0x0200c63c
	ldr r0, [pc, #732]
	bl 0x0200c7d4
	ldr r0, [pc, #728]
	movs r1, #0
	bl 0x0200c7e4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #20
	bl 0x0200c80c
	movs r0, #0
	movs r1, #150
	ldr r2, [pc, #708]
	bl 0x0200c77c
	movs r0, #0
	bl 0x0200c72c
	cmp r0, #0
	beq .L_02002180_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #22
	bl 0x0200c78c
.L_02002180_1:
	movs r0, #22
	movs r1, #132
	ldr r2, [pc, #680]
	bl 0x0200c77c
	movs r1, #22
	movs r2, #0
	movs r0, #0
	bl 0x0200c7c4
	movs r0, #40
	bl 0x0200c6f4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #128
	movs r2, #20
	movs r0, #22
	lsls r1, r1, #7
	bl 0x0200c7fc
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl 0x0200c81c
	movs r0, #216
	movs r1, #1
	movs r2, #154
	movs r3, #1
	lsls r2, r2, #19
	negs r1, r1
	lsls r0, r0, #16
	bl 0x0200c824
	bl 0x0200c82c
	movs r0, #40
	bl 0x0200c6f4
	movs r0, #10
	movs r1, #2
	bl 0x0200c7b4
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl 0x0200c7ec
	movs r0, #23
	movs r1, #3
	bl 0x0200c7b4
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl 0x0200c7fc
	movs r0, #9
	movs r1, #3
	bl 0x0200c79c
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r1, #192
	movs r2, #10
	movs r0, #9
	lsls r1, r1, #6
	bl 0x0200c7fc
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200c81c
	movs r0, #232
	movs r1, #1
	movs r3, #1
	negs r1, r1
	ldr r2, [pc, #512]
	lsls r0, r0, #16
	bl 0x0200c824
	bl 0x0200c82c
	movs r0, #20
	bl 0x0200c6f4
	movs r0, #134
	bl 0x0200c8b4
	movs r2, #0
	movs r0, #23
	movs r1, #4
	bl 0x0200c7a4
	movs r1, #6
	movs r0, #23
	bl 0x0200c794
	movs r0, #10
	bl 0x0200c6f4
	movs r2, #0
	movs r1, #0
	movs r0, #23
	bl 0x0200c78c
	movs r0, #60
	bl 0x0200c6f4
	bl 0x0200c8ac
	movs r1, #1
	movs r0, #10
	bl 0x0200c794
	movs r0, #10
	bl 0x0200c72c
	movs r5, #128
	lsls r5, r5, #9
	str r5, [r0, #24]
	str r5, [r0, #28]
	movs r1, #1
	movs r0, #24
	bl 0x0200c794
	movs r0, #24
	bl 0x0200c72c
	movs r1, #1
	str r5, [r0, #24]
	str r5, [r0, #28]
	movs r0, #25
	bl 0x0200c794
	movs r0, #25
	bl 0x0200c72c
	movs r1, #2
	str r5, [r0, #24]
	str r5, [r0, #28]
	movs r0, #10
	bl 0x0200c7ac
	movs r0, #9
	movs r1, #2
	bl 0x0200c7ac
	movs r0, #24
	movs r1, #2
	bl 0x0200c7ac
	movs r0, #25
	movs r1, #2
	bl 0x0200c7ac
	movs r0, #26
	movs r1, #2
	bl 0x0200c7b4
	ldr r0, [pc, #348]
	ldr r1, [pc, #352]
	bl 0x0200c81c
	movs r0, #216
	movs r1, #1
	movs r2, #154
	movs r3, #1
	lsls r2, r2, #19
	lsls r0, r0, #16
	negs r1, r1
	bl 0x0200c824
	bl 0x0200c82c
	movs r1, #129
	movs r0, #26
	lsls r1, r1, #1
	bl 0x0200c814
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl 0x0200c814
	movs r0, #60
	bl 0x0200c6f4
	movs r0, #26
	movs r1, #2
	bl 0x0200c7b4
	movs r0, #26
	movs r1, #3
	bl 0x0200c7ac
	movs r0, #26
	movs r1, #0
	bl 0x0200c7e4
	movs r0, #25
	movs r1, #2
	movs r2, #0
	bl 0x0200c7a4
	movs r0, #25
	movs r1, #234
	ldr r2, [pc, #264]
	bl 0x0200c764
	movs r0, #26
	movs r1, #2
	movs r2, #0
	bl 0x0200c7a4
	movs r1, #227
	ldr r2, [pc, #248]
	movs r0, #26
	bl 0x0200c764
	movs r0, #90
	bl 0x0200c6f4
	movs r0, #232
	movs r1, #1
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	ldr r2, [pc, #208]
	bl 0x0200c824
	bl 0x0200c82c
	movs r1, #243
	ldr r2, [pc, #180]
	lsls r1, r1, #16
	movs r0, #23
	bl 0x0200c78c
	movs r0, #1
	bl 0x0200c63c
	movs r0, #106
	bl 0x0200c8b4
	movs r0, #23
	bl 0x0200c72c
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r0, #40]
	movs r0, #6
	bl 0x0200c6f4
	movs r1, #7
	movs r0, #23
	bl 0x0200c794
	movs r0, #20
	bl 0x0200c6f4
	bl 0x0200c8ac
	movs r0, #20
	bl 0x0200c6f4
	ldr r0, [pc, #152]
	ldr r1, [pc, #156]
	bl 0x0200c81c
	movs r0, #216
	movs r1, #1
	movs r2, #154
	movs r3, #1
	lsls r2, r2, #19
	lsls r0, r0, #16
	negs r1, r1
	bl 0x0200c824
	bl 0x0200c82c
	movs r1, #2
	movs r0, #24
	bl 0x0200c7b4
	movs r0, #20
.L_02002538:
	bl 0x0200c6f4
	movs r0, #24
	ldr r1, [pc, #116]
	movs r2, #40
	bl 0x0200c80c
	movs r2, #0
	movs r1, #10
	movs r0, #24
	bl 0x0200c7c4
	movs r0, #10
	bl 0x0200c6f4
	movs r0, #10
	movs r1, #2
	bl 0x0200c7b4
	movs r1, #0
	ldr r0, [pc, #84]
	bl 0x0200c7e4
	movs r0, #25
	bl 0x0200c72c
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	b .L_02002538_0
	.2byte 0x083a
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x04be
	.2byte 0xcec8
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0x04a5
	.2byte 0x0000
	.2byte 0x04fd
	.2byte 0x0e8c
	.2byte 0x0000
	.2byte 0x201a
	.2byte 0x0000
	.2byte 0x0446
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x04e5
	.2byte 0x9999
	.2byte 0x0000
	.2byte 0x1333
	.2byte 0x0000
	.2byte 0x04b5
	.2byte 0x0000
	.2byte 0x04b1
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0001
	.2byte 0x3333
	.2byte 0x0000
	.4byte 0x00000105
	.4byte 0x0000800a
.L_02002538_0:
	strb r3, [r0]
	movs r0, #26
	bl 0x0200c72c
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	ldr r1, [pc, #692]
	movs r0, #25
	ldr r2, [pc, #692]
	bl 0x0200c73c
	movs r0, #26
	ldr r1, [pc, #680]
	ldr r2, [pc, #684]
	bl 0x0200c73c
	movs r0, #25
	movs r1, #247
	ldr r2, [pc, #676]
	bl 0x0200c764
	movs r1, #227
	ldr r2, [pc, #672]
	movs r0, #26
	bl 0x0200c76c
	movs r0, #25
	bl 0x0200c72c
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #26
	bl 0x0200c72c
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #192
	orrs r5, r3
	strb r5, [r0]
	lsls r1, r1, #7
	movs r0, #26
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #128
	movs r2, #10
	movs r0, #25
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r0, #24
	movs r1, #4
	bl 0x0200c794
	ldr r0, [pc, #608]
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200c7fc
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl 0x0200c7fc
	movs r0, #10
	movs r1, #4
	bl 0x0200c794
	ldr r0, [pc, #572]
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r0, #24
	ldr r1, [pc, #564]
	movs r2, #0
	bl 0x0200c80c
	movs r0, #10
	ldr r1, [pc, #552]
	movs r2, #60
	bl 0x0200c80c
	movs r1, #131
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #20
	bl 0x0200c80c
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200c7fc
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200c7fc
	movs r0, #9
	movs r1, #0
	movs r2, #30
	bl 0x0200c7fc
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200c7fc
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #144
	movs r0, #25
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #160
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #128
	movs r2, #10
	movs r0, #26
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r0, #10
	movs r1, #1
	bl 0x0200c7b4
	movs r2, #10
	ldr r0, [pc, #416]
	movs r1, #0
	bl 0x0200c7ec
	movs r0, #9
	movs r1, #4
	bl 0x0200c794
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r0, #10
	ldr r1, [pc, #392]
	movs r2, #0
	bl 0x0200c80c
	movs r0, #24
	ldr r1, [pc, #380]
	movs r2, #0
	bl 0x0200c80c
	movs r0, #25
	ldr r1, [pc, #372]
	movs r2, #0
	bl 0x0200c80c
	movs r0, #26
	ldr r1, [pc, #360]
	movs r2, #40
	bl 0x0200c80c
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200c7fc
	movs r1, #25
	movs r2, #0
	movs r0, #24
	bl 0x0200c7c4
	movs r0, #20
	bl 0x0200c6f4
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200c7fc
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl 0x0200c7fc
	movs r1, #128
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #128
	movs r2, #10
	movs r0, #25
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r0, #24
	movs r1, #3
	bl 0x0200c794
	movs r0, #25
	movs r1, #3
	bl 0x0200c79c
	movs r2, #0
	movs r1, #9
	movs r0, #10
	bl 0x0200c7c4
	movs r0, #20
	bl 0x0200c6f4
	movs r0, #10
	movs r1, #1
	bl 0x0200c7b4
	movs r2, #10
	ldr r0, [pc, #236]
	movs r1, #0
	bl 0x0200c7ec
	movs r0, #9
	movs r1, #3
	bl 0x0200c79c
	movs r1, #208
	movs r2, #10
	movs r0, #24
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r0, #24
	movs r1, #1
	bl 0x0200c7ac
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200c7fc
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl 0x0200c7fc
	movs r0, #26
	movs r1, #1
	bl 0x0200c7b4
	movs r1, #128
	movs r0, #26
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200c7fc
	movs r1, #160
	movs r2, #20
	movs r0, #25
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r0, #25
	movs r1, #3
	bl 0x0200c79c
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl 0x0200c7ec
	movs r1, #3
	movs r0, #26
	bl 0x0200c79c
	movs r0, #20
	bl 0x0200c6f4
	movs r0, #9
	movs r1, #2
	bl 0x0200c7b4
	movs r0, #9
	movs r1, #3
	bl 0x0200c79c
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r1, #128
	movs r2, #10
	movs r0, #26
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r1, #3
	movs r0, #26
	bl 0x0200c79c
	movs r0, #20
	bl 0x0200c6f4
	movs r0, #9
	movs r1, #1
	bl 0x0200c7b4
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	bl 0x0200a8a4
	ldr r0, [pc, #40]
	bl 0x0200c6e4
	bl 0x0200c704
	pop {r5, r6}
	pop {r0}
.L_02002882:
	bx r0
	.2byte 0x9999
	.2byte 0x0000
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0x04ba
	.2byte 0x0000
	.2byte 0x04a5
	.2byte 0x0000
	.2byte 0x8018
	.2byte 0x0000
	.2byte 0x800a
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0x083a
	.2byte 0x0000
	.global Func_020028a4
	.thumb_func
Func_020028a4:
	push {r5, lr}
	movs r1, #192
	movs r0, #26
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #208
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #176
	movs r0, #25
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #208
.L_020028d8:
	movs r2, #20
	movs r0, #10
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r0, #26
	movs r1, #3
	bl 0x0200c794
	movs r0, #24
.L_020028ec:
	movs r1, #3
	bl 0x0200c794
	movs r0, #25
	movs r1, #3
	bl 0x0200c794
	movs r0, #9
	movs r1, #3
	bl 0x0200c794
	movs r1, #3
	movs r0, #25
	bl 0x0200c79c
	movs r0, #20
	bl 0x0200c6f4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200c81c
	movs r0, #134
	movs r1, #1
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	ldr r2, [pc, #920]
	bl 0x0200c824
	movs r0, #26
	ldr r1, [pc, #916]
	ldr r2, [pc, #916]
	bl 0x0200c73c
	ldr r2, [pc, #912]
	movs r0, #9
	ldr r1, [pc, #904]
	bl 0x0200c73c
	ldr r1, [pc, #904]
	movs r0, #26
	bl 0x0200c744
	ldr r1, [pc, #900]
	movs r0, #9
	bl 0x0200c75c
	movs r0, #158
	bl 0x0200c8b4
	movs r1, #38
	movs r2, #72
	ldr r0, [pc, #888]
	bl 0x0200c68c
	movs r0, #10
	bl 0x0200c6f4
	movs r0, #9
	movs r1, #149
	ldr r2, [pc, #876]
	bl 0x0200c77c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200c78c
	movs r0, #25
	movs r1, #250
	ldr r2, [pc, #860]
	bl 0x0200c77c
	bl 0x0200c8ac
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #192
	movs r0, #24
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #192
	movs r2, #0
	movs r0, #25
	lsls r1, r1, #6
	bl 0x0200c7fc
	movs r0, #10
	movs r1, #5
	bl 0x0200c794
	movs r0, #24
	movs r1, #6
	bl 0x0200c794
	movs r1, #6
	movs r0, #25
	bl 0x0200c794
	movs r0, #10
	bl 0x0200c72c
	adds r5, r0, #0
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	adds r5, #100
	adds r0, #60
	strh r0, [r5]
	movs r0, #24
	bl 0x0200c72c
	adds r5, r0, #0
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	adds r5, #100
	adds r0, #60
	strh r0, [r5]
	movs r0, #25
	bl 0x0200c72c
	adds r5, r0, #0
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	adds r5, #100
	adds r0, #60
	strh r0, [r5]
	ldr r5, [pc, #720]
	movs r0, #10
	adds r1, r5, #0
	bl 0x0200c744
	adds r1, r5, #0
	movs r0, #24
	bl 0x0200c744
	adds r1, r5, #0
	movs r0, #25
	bl 0x0200c744
	movs r0, #26
	bl 0x0200c74c
	movs r0, #10
	bl 0x0200c6f4
	movs r0, #159
	bl 0x0200c8b4
	movs r1, #38
	movs r2, #72
	ldr r0, [pc, #676]
	bl 0x0200c68c
	movs r0, #30
	bl 0x0200c6f4
	bl 0x0200c8ac
	movs r0, #224
	movs r1, #1
	movs r3, #1
	negs r1, r1
	ldr r2, [pc, #656]
	lsls r0, r0, #15
	bl 0x0200c824
	movs r0, #158
	bl 0x0200c8b4
	movs r2, #73
	movs r1, #35
	ldr r0, [pc, #644]
	bl 0x0200c68c
	movs r0, #20
	bl 0x0200c6f4
	bl 0x0200c8ac
	ldr r1, [pc, #632]
	movs r0, #9
	bl 0x0200c744
	movs r0, #20
	bl 0x0200c6f4
	ldr r1, [pc, #620]
	movs r0, #26
	bl 0x0200c744
	movs r0, #40
	bl 0x0200c6f4
	movs r0, #159
	bl 0x0200c8b4
	movs r1, #35
	movs r2, #73
	ldr r0, [pc, #600]
	bl 0x0200c68c
	movs r0, #26
	bl 0x0200c74c
	bl 0x0200c8ac
	movs r0, #40
	bl 0x0200c6f4
	ldr r5, [pc, #584]
	adds r0, r5, #0
	bl 0x0200c7d4
	movs r2, #20
.L_02002abc:
	movs r0, #9
	movs r1, #0
	bl 0x0200c7ec
	movs r0, #26
	movs r1, #3
	bl 0x0200c79c
	movs r2, #40
	ldr r0, [pc, #560]
	movs r1, #0
	bl 0x0200c7ec
	movs r0, #9
	movs r1, #3
	bl 0x0200c794
	movs r1, #3
	movs r0, #26
	bl 0x0200c79c
	movs r0, #30
	bl 0x0200c6f4
	ldr r1, [pc, #532]
	movs r0, #9
	bl 0x0200c744
	ldr r1, [pc, #528]
	movs r0, #26
	bl 0x0200c744
	movs r0, #40
	bl 0x0200c6f4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200c81c
	movs r0, #210
	movs r1, #1
	movs r3, #1
	negs r1, r1
	ldr r2, [pc, #500]
	lsls r0, r0, #15
	bl 0x0200c824
	movs r0, #9
	bl 0x0200c74c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200c80c
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200c7fc
	ldr r2, [pc, #432]
	movs r0, #9
	movs r1, #105
	bl 0x0200c77c
	movs r0, #9
	movs r1, #2
	bl 0x0200c7b4
	movs r1, #0
	ldr r0, [pc, #416]
	bl 0x0200c7dc
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl 0x0200c7fc
	movs r0, #0
	movs r1, #0
	bl 0x0200c724
	cmp r0, #0
	bne .L_02002abc_0
	movs r0, #9
	movs r1, #3
	bl 0x0200c79c
	adds r0, r5, #4
	bl 0x0200c7d4
	b .L_02002abc_1
.L_02002abc_0:
	movs r0, #9
	movs r1, #2
	bl 0x0200c7b4
	adds r0, r5, #5
	bl 0x0200c7d4
.L_02002abc_1:
	ldr r0, [pc, #360]
	movs r1, #0
	bl 0x0200c7e4
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200c7fc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl 0x0200c80c
	ldr r5, [pc, #332]
	adds r0, r5, #0
	bl 0x0200c7d4
	movs r1, #0
	ldr r0, [pc, #316]
	bl 0x0200c7dc
	movs r0, #0
	movs r1, #0
	bl 0x0200c724
	cmp r0, #0
	bne .L_02002abc_2
	movs r1, #3
	movs r0, #9
	bl 0x0200c79c
	adds r0, r5, #1
	bl 0x0200c7d4
	ldr r0, [pc, #284]
	movs r1, #0
	movs r2, #30
	bl 0x0200c7ec
	movs r1, #128
	movs r2, #20
	movs r0, #22
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r0, #0
	movs r1, #3
	bl 0x0200c794
	movs r0, #22
	movs r1, #3
	bl 0x0200c794
	movs r0, #9
	movs r1, #3
	bl 0x0200c79c
	movs r0, #40
	bl 0x0200c6f4
	b .L_02002abc_3
.L_02002abc_2:
	movs r0, #9
	ldr r1, [pc, #236]
	movs r2, #90
	bl 0x0200c80c
	movs r2, #40
	movs r0, #9
	ldr r1, [pc, #228]
	bl 0x0200c80c
	movs r1, #4
	movs r0, #9
	bl 0x0200c794
	adds r0, r5, #2
	bl 0x0200c7d4
	ldr r0, [pc, #196]
	movs r1, #0
	bl 0x0200c7e4
.L_02002abc_3:
	ldr r1, [pc, #204]
	movs r0, #9
	bl 0x0200c744
	movs r0, #90
	bl 0x0200c6f4
	movs r2, #0
	movs r1, #22
	movs r0, #0
	bl 0x0200c7c4
	movs r0, #40
	bl 0x0200c6f4
	movs r0, #0
	movs r1, #3
	bl 0x0200c794
	movs r1, #3
	movs r0, #22
	bl 0x0200c79c
	movs r0, #20
	bl 0x0200c6f4
	movs r0, #22
	movs r1, #2
	bl 0x0200c794
	movs r0, #0
	bl 0x0200c72c
	cmp r0, #0
	beq .L_02002abc_4
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #22
	bl 0x0200c764
.L_02002abc_4:
	movs r0, #22
	bl 0x0200c784
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl 0x0200c78c
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x04ab
	.2byte 0x9999
	.2byte 0x0001
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0xcab4
	.2byte 0x0200
	.2byte 0xca78
	.2byte 0x0200
	.2byte 0xd7a0
	.2byte 0x0200
	.2byte 0x0497
	.2byte 0x0000
	.2byte 0x04be
	.2byte 0x0000
	.2byte 0xcec8
	.2byte 0x0200
	.2byte 0xd7e2
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0x04c9
	.2byte 0xd78a
	.2byte 0x0200
	.2byte 0xcb28
	.2byte 0x0200
	.2byte 0xcb9c
	.2byte 0x0200
	.2byte 0xd7cc
	.2byte 0x0200
	.2byte 0x0e9b
	.2byte 0x0000
	.4byte 0x0000201a
	.4byte 0x0200cc0c
	.4byte 0x0200cc5c
	.4byte 0x043e0000
	.4byte 0x0000043e
	.4byte 0x00008009
	.4byte 0x00000ea1
	.4byte 0x00000105
	.4byte 0x00000103
	.4byte 0x0200cca8
	.global Func_02002d28
	.thumb_func
Func_02002d28:
	push {r5, lr}
	bl 0x0200c6fc
	movs r1, #1
	movs r0, #10
	bl 0x0200c794
	movs r0, #10
	bl 0x0200c6f4
	movs r0, #10
	movs r1, #0
	movs r2, #20
	bl 0x0200c7c4
	ldr r0, [pc, #148]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_02002d28_0
	ldr r0, [pc, #140]
	bl 0x0200c7d4
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	b .L_02002d28_1
.L_02002d28_0:
	ldr r0, [pc, #128]
	bl 0x0200c7d4
	movs r0, #10
	movs r1, #1
	bl 0x0200c7ac
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl 0x0200c7ec
	movs r0, #10
	movs r1, #2
	bl 0x0200c7ac
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
.L_02002d28_1:
	movs r1, #128
	movs r2, #20
	movs r0, #10
	lsls r1, r1, #6
	bl 0x0200c7fc
	movs r1, #5
	movs r0, #10
	bl 0x0200c794
	movs r0, #10
	bl 0x0200c6f4
	movs r0, #10
	bl 0x0200c72c
	adds r5, r0, #0
	bl 0x0200c654
	movs r1, #90
	bl 0x0200c634
	adds r5, #100
	adds r0, #60
	ldr r1, [pc, #40]
	strh r0, [r5]
	movs r0, #10
	bl 0x0200c744
	movs r0, #20
	bl 0x0200c6f4
	ldr r0, [pc, #12]
	bl 0x0200c6e4
	bl 0x0200c704
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0000030d
	.4byte 0x00000ea5
	.4byte 0x00000ea4
	.4byte 0x0200cec8
	.global Func_02002dec
	.thumb_func
Func_02002dec:
	push {r5, r6, r7, lr}
	movs r0, #132
	lsls r0, r0, #4
	bl 0x0200c6dc
	cmp r0, #0
	bne .L_02002dec_0
	b .L_02002dec_1
.L_02002dec_0:
	ldr r0, [pc, #876]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_02002dec_2
	b .L_02002dec_1
.L_02002dec_2:
	bl 0x0200c6fc
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c73c
	movs r1, #128
	movs r2, #128
	movs r0, #22
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c73c
	movs r1, #128
	movs r2, #128
	movs r0, #26
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c73c
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c73c
	movs r0, #0
	movs r1, #217
	ldr r2, [pc, #804]
	bl 0x0200c77c
	movs r0, #0
	bl 0x0200c72c
	cmp r0, #0
	beq .L_02002dec_3
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #22
	bl 0x0200c78c
.L_02002dec_3:
	movs r0, #22
	movs r1, #235
	ldr r2, [pc, #776]
	bl 0x0200c77c
	movs r1, #176
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c7fc
	movs r0, #0
	bl 0x0200c72c
	cmp r0, #0
	beq .L_02002dec_4
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #26
	bl 0x0200c78c
.L_02002dec_4:
	movs r0, #26
	movs r1, #199
	ldr r2, [pc, #732]
	bl 0x0200c77c
	movs r1, #208
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200c7fc
	movs r1, #247
	movs r0, #25
	lsls r1, r1, #16
	ldr r2, [pc, #712]
	bl 0x0200c78c
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #25
	bl 0x0200c7fc
	movs r0, #8
	bl 0x0200c72c
	adds r1, r0, #0
	adds r1, #35
	ldr r4, [r0, #80]
	ldrb r2, [r1]
	movs r6, #254
	adds r3, r6, #0
	movs r5, #13
	ands r3, r2
	negs r5, r5
	ldrb r2, [r4, #9]
	strb r3, [r1]
	adds r3, r5, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r4, #9]
	movs r0, #0
	bl 0x0200c72c
	adds r7, r0, #0
	adds r7, #35
	ldr r4, [r0, #80]
	ldrb r3, [r7]
	ands r6, r3
	ldrb r3, [r4, #9]
	ands r5, r3
	movs r3, #8
	orrs r5, r3
	strb r6, [r7]
	strb r5, [r4, #9]
	movs r0, #0
	bl 0x0200c72c
	cmp r0, #0
	beq .L_02002dec_5
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #8
	bl 0x0200c78c
.L_02002dec_5:
	movs r0, #8
	movs r1, #221
	ldr r2, [pc, #608]
	bl 0x0200c77c
	movs r1, #176
	movs r2, #60
	movs r0, #8
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r1, #2
	movs r0, #26
	bl 0x0200c7b4
	ldr r0, [pc, #588]
	bl 0x0200c7d4
	movs r0, #26
	movs r1, #0
	movs r2, #40
	bl 0x0200c7ec
	movs r1, #202
	movs r0, #9
	lsls r1, r1, #15
	ldr r2, [pc, #568]
	bl 0x0200c78c
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200c7fc
	ldr r0, [pc, #556]
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r1, #160
	movs r2, #0
	movs r0, #26
	lsls r1, r1, #8
	bl 0x0200c7fc
	ldr r0, [pc, #536]
	ldr r1, [pc, #540]
	bl 0x0200c81c
	movs r0, #202
	movs r1, #1
	movs r3, #1
	lsls r0, r0, #15
	negs r1, r1
	ldr r2, [pc, #512]
	bl 0x0200c824
	ldr r2, [pc, #520]
	movs r0, #9
	ldr r1, [pc, #520]
	bl 0x0200c73c
	ldr r1, [pc, #520]
	movs r0, #9
	bl 0x0200c744
	movs r0, #60
	bl 0x0200c6f4
	ldr r0, [pc, #508]
	ldr r1, [pc, #512]
	bl 0x0200c81c
	movs r0, #187
	movs r1, #1
	movs r2, #166
	movs r3, #1
	lsls r2, r2, #19
	negs r1, r1
	lsls r0, r0, #16
	bl 0x0200c824
	bl 0x0200c82c
	movs r0, #40
	bl 0x0200c6f4
	movs r0, #26
	movs r1, #2
	bl 0x0200c7b4
	movs r2, #20
	movs r0, #26
	movs r1, #0
	bl 0x0200c7ec
	movs r0, #9
	movs r1, #2
	bl 0x0200c7b4
	movs r2, #20
	ldr r0, [pc, #452]
	movs r1, #0
	bl 0x0200c7ec
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200c81c
	movs r0, #221
	movs r1, #1
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	ldr r2, [pc, #428]
	bl 0x0200c824
	movs r0, #0
	movs r1, #8
	movs r2, #0
	bl 0x0200c7bc
	movs r0, #22
	movs r1, #8
	movs r2, #0
	bl 0x0200c7bc
	movs r1, #192
	movs r0, #26
	lsls r1, r1, #6
	movs r2, #80
	bl 0x0200c7fc
	movs r0, #182
	movs r1, #1
	movs r2, #170
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #19
	bl 0x0200c824
	movs r2, #173
	movs r0, #8
	movs r1, #182
	lsls r2, r2, #3
	bl 0x0200c77c
	movs r2, #0
	movs r1, #9
	movs r0, #8
	bl 0x0200c7bc
	movs r0, #30
	bl 0x0200c6f4
	movs r1, #3
	movs r0, #8
	bl 0x0200c79c
	movs r0, #10
	bl 0x0200c6f4
	movs r0, #0
	movs r1, #9
	movs r2, #0
	bl 0x0200c7bc
	movs r0, #22
	movs r1, #9
	movs r2, #0
	bl 0x0200c7bc
	movs r2, #0
	movs r0, #26
	movs r1, #9
	bl 0x0200c7bc
	movs r0, #9
	movs r1, #3
	bl 0x0200c79c
	movs r0, #9
	movs r1, #0
	bl 0x0200c7e4
	movs r0, #26
	movs r1, #2
	bl 0x0200c7b4
	movs r0, #26
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r1, #224
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200c7fc
	movs r1, #192
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #6
	bl 0x0200c7fc
	movs r0, #9
	movs r1, #3
	bl 0x0200c79c
	movs r0, #9
	movs r1, #0
	bl 0x0200c7e4
	movs r0, #26
	movs r1, #8
	movs r2, #0
	bl 0x0200c7c4
	movs r1, #0
	movs r2, #0
	movs r0, #22
	bl 0x0200c7c4
	movs r0, #40
	bl 0x0200c6f4
	movs r0, #0
	movs r1, #9
	movs r2, #0
	bl 0x0200c7bc
	movs r0, #22
	movs r1, #9
	movs r2, #0
	bl 0x0200c7bc
	movs r0, #26
	movs r1, #9
	movs r2, #0
	bl 0x0200c7bc
	movs r2, #0
	movs r0, #8
	movs r1, #9
	bl 0x0200c7bc
	movs r1, #2
	movs r0, #9
	bl 0x0200c7b4
	movs r0, #20
	bl 0x0200c6f4
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl 0x0200c7ec
	movs r0, #0
	movs r1, #3
	bl 0x0200c794
	movs r0, #26
	movs r1, #3
	bl 0x0200c794
	movs r0, #22
	movs r1, #3
	bl 0x0200c794
	movs r1, #3
	movs r0, #8
	bl 0x0200c79c
	ldrb r3, [r7]
	movs r5, #1
	orrs r3, r5
	strb r3, [r7]
	movs r0, #8
	bl 0x0200c72c
	adds r2, r0, #0
	adds r2, #35
	ldrb r3, [r2]
	orrs r3, r5
	strb r3, [r2]
	bl 0x0200b1ac
	ldr r0, [pc, #16]
	bl 0x0200c6e4
	bl 0x0200c704
.L_02002dec_1:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000841
	.4byte 0x00000557
	.4byte 0x04ba0000
	.4byte 0x00000569
	.4byte 0x00000ec6
	.4byte 0x04ad0000
	.4byte 0x00001009
	.4byte 0x00013333
	.4byte 0x00002666
	.4byte 0x0000b333
	.4byte 0x00016666
	.4byte 0x0200cd1c
	.4byte 0x00009999
	.4byte 0x00001333
	.4byte 0x00004009
	.4byte 0x05690000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #19
	sub	sp, #4
	bl 0x0200c72c
	adds	r7, r0, #0
	movs	r0, #27
	bl 0x0200c72c
	adds	r6, r0, #0
	ldr	r1, [r6, #80]
	ldr	r0, [r7, #80]
	mov	fp, r1
	mov	sl, r0
	movs	r1, #128
	movs	r0, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200c81c
	movs	r0, #220
	movs	r1, #1
	movs	r3, #1
	lsls	r0, r0, #15
	negs	r1, r1
	ldr	r2, [pc, #428]
	bl 0x0200c824
	movs	r0, #8
	ldr	r1, [pc, #424]
	ldr	r2, [pc, #424]
	bl 0x0200c73c
	movs	r0, #26
	ldr	r1, [pc, #412]
	ldr	r2, [pc, #416]
	bl 0x0200c73c
	movs	r0, #0
	ldr	r1, [pc, #404]
	ldr	r2, [pc, #404]
	bl 0x0200c73c
	ldr	r2, [pc, #400]
	movs	r0, #22
	ldr	r1, [pc, #392]
	bl 0x0200c73c
	ldr	r5, [pc, #392]
	movs	r0, #8
	adds	r1, r5, #0
	bl 0x0200c744
	movs	r0, #10
	bl 0x0200c6f4
	adds	r1, r5, #0
	movs	r0, #26
	bl 0x0200c744
	bl 0x0200c864
	movs	r0, #10
	bl 0x0200c6f4
	adds	r1, r5, #0
	movs	r0, #0
	bl 0x0200c744
	movs	r0, #10
	bl 0x0200c6f4
	bl 0x0200c85c
	adds	r1, r5, #0
	movs	r0, #22
	bl 0x0200c744
	movs	r0, #128
	bl 0x0200c6f4
	bl 0x02008134
	movs	r0, #174
	movs	r1, #1
	negs	r1, r1
	ldr	r2, [pc, #320]
	movs	r3, #1
	lsls	r0, r0, #16
	bl 0x0200c824
	movs	r0, #104
	bl 0x0200c6f4
	movs	r0, #153
	movs	r1, #1
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	ldr	r2, [pc, #300]
	bl 0x0200c824
	movs	r2, #159
	movs	r0, #9
	movs	r1, #158
	lsls	r2, r2, #3
	bl 0x0200c77c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #9
	bl 0x0200c7fc
	movs	r0, #8
	bl 0x0200c74c
	ldr	r1, [pc, #268]
	movs	r0, #8
	bl 0x0200c744
	ldr	r1, [pc, #264]
	movs	r0, #26
	bl 0x0200c744
	ldr	r1, [pc, #260]
	movs	r0, #0
	bl 0x0200c744
	ldr	r1, [pc, #256]
	movs	r0, #22
	bl 0x0200c75c
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	bl 0x0200c6bc
	movs	r0, #145
	bl 0x0200c8b4
	movs	r0, #20
	bl 0x0200c6f4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200c6bc
	movs	r0, #60
	bl 0x0200c6f4
	movs	r0, #0
	ldr	r1, [pc, #200]
	movs	r2, #0
	bl 0x0200c80c
	movs	r0, #26
	ldr	r1, [pc, #188]
	movs	r2, #0
	bl 0x0200c80c
	movs	r0, #22
	ldr	r1, [pc, #180]
	movs	r2, #0
	bl 0x0200c80c
	movs	r0, #8
	ldr	r1, [pc, #168]
	movs	r2, #0
	bl 0x0200c80c
	movs	r0, #9
	ldr	r1, [pc, #160]
	movs	r2, #60
	bl 0x0200c80c
	movs	r0, #26
	movs	r1, #8
	movs	r2, #0
	bl 0x0200c7c4
	movs	r2, #0
	movs	r1, #0
	movs	r0, #22
	bl 0x0200c7c4
	movs	r0, #20
	bl 0x0200c6f4
	movs	r0, #0
	bl 0x0200c72c
	adds	r5, r0, #0
	bl 0x0200c654
	movs	r1, #20
	bl 0x0200c634
	adds	r5, #100
	ldr	r2, [pc, #60]
	adds	r0, #20
	movs	r3, #0
	strh	r0, [r5, #0]
	movs	r0, #22
	mov	r9, r3
	mov	r8, r2
	bl 0x0200c72c
	adds	r5, r0, #0
	bl 0x0200c654
	movs	r1, #20
	bl 0x0200c634
	adds	r5, #100
	adds	r0, #20
	strh	r0, [r5, #0]
	movs	r0, #26
	bl 0x0200c72c
	adds	r5, r0, #0
	bl 0x0200c654
	movs	r1, #20
	bl 0x0200c634
	adds	r5, #100
	adds	r0, #20
	strh	r0, [r5, #0]
.L_02003390:
	b.n	.L_020033c4
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x058b0000
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x0200cd6c
	.4byte 0x05940000
	.4byte 0x052d0000
	.4byte 0x0200ce04
	.4byte 0x0200ce30
	.4byte 0x0200ce5c
	.4byte 0x0200ce88
	.2byte 0x0101
	.2byte 0x0000
.L_020033c4:
	movs	r0, #8
	bl 0x0200c72c
	adds	r5, r0, #0
	bl 0x0200c654
	movs	r1, #20
	bl 0x0200c634
	adds	r5, #100
	adds	r0, #20
	strh	r0, [r5, #0]
	movs	r0, #9
	bl 0x0200c72c
	adds	r5, r0, #0
	bl 0x0200c654
	movs	r1, #20
	bl 0x0200c634
	adds	r5, #100
	adds	r0, #20
	strh	r0, [r5, #0]
	ldr	r5, [pc, #1012]
	movs	r0, #9
	adds	r1, r5, #0
	bl 0x0200c744
	movs	r0, #30
	bl 0x0200c6f4
	adds	r1, r5, #0
	movs	r0, #0
	bl 0x0200c744
	adds	r1, r5, #0
	movs	r0, #26
	bl 0x0200c744
	adds	r1, r5, #0
	movs	r0, #22
	bl 0x0200c744
	adds	r1, r5, #0
	movs	r0, #8
	bl 0x0200c744
	movs	r0, #10
	bl 0x0200c6f4
	movs	r0, #17
	bl 0x0200c8b4
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200c6bc
	movs	r0, #145
	bl 0x0200c8b4
	movs	r0, #30
	bl 0x0200c6f4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200c6bc
	movs	r0, #120
	bl 0x0200c6f4
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200c6bc
	movs	r0, #145
	bl 0x0200c8b4
	movs	r0, #40
	bl 0x0200c6f4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200c6bc
	movs	r0, #60
	bl 0x0200c6f4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	bl 0x0200c6bc
	movs	r0, #145
	bl 0x0200c8b4
	movs	r0, #20
	bl 0x0200c6f4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200c6bc
	movs	r0, #60
	bl 0x0200c6f4
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200c6bc
	movs	r0, #145
	bl 0x0200c8b4
	movs	r0, #40
	bl 0x0200c6f4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200c6bc
	movs	r0, #60
	bl 0x0200c6f4
	bl 0x0200c85c
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200c6bc
	movs	r0, #1
	bl 0x0200c6f4
	movs	r0, #1
	movs	r1, #1
	ldr	r2, [pc, #728]
	negs	r0, r0
	negs	r1, r1
	bl 0x0200c6bc
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #12
	lsls	r1, r1, #12
	bl 0x0200c81c
	movs	r0, #217
	movs	r1, #1
	ldr	r2, [pc, #704]
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	bl 0x0200c824
	movs	r1, #0
	movs	r0, #0
	bl 0x0200c874
	movs	r0, #40
	bl 0x0200c87c
	movs	r0, #40
	bl 0x0200c63c
	movs	r1, #0
	movs	r0, #19
	bl 0x0200c7cc
	movs	r0, #19
	bl 0x0200c72c
	movs	r1, #0
	bl 0x0200c6b4
	movs	r0, #27
	bl 0x0200c72c
	movs	r1, #0
	bl 0x0200c6b4
	ldr	r3, [pc, #644]
	adds	r1, r6, #0
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r0, #254
	adds	r3, r0, #0
	ands	r3, r2
	strb	r3, [r1, #0]
	mov	r1, fp
	ldrb	r2, [r1, #9]
	movs	r1, #13
	negs	r1, r1
	adds	r3, r1, #0
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	mov	r2, fp
	strb	r3, [r2, #9]
	movs	r3, #200
	lsls	r3, r3, #16
	ldr	r2, [pc, #604]
	str	r3, [r7, #8]
	str	r3, [r7, #12]
	str	r3, [r7, #56]
	str	r3, [r7, #60]
	adds	r3, r7, #0
	str	r2, [r7, #16]
	str	r2, [r7, #64]
	adds	r3, #85
	mov	r2, r8
	str	r3, [sp, #0]
	strb	r2, [r3, #0]
	adds	r2, r7, #0
	adds	r2, #35
	ldrb	r3, [r2, #0]
	ands	r0, r3
	strb	r0, [r2, #0]
	mov	r0, sl
	ldrb	r3, [r0, #9]
	ands	r1, r3
	strb	r1, [r0, #9]
	bl 0x0200c834
	movs	r5, #128
	lsls	r5, r5, #24
	str	r5, [r0, #56]
	bl 0x0200c834
	str	r5, [r0, #60]
	bl 0x0200c834
	str	r5, [r0, #64]
	bl 0x0200c834
	mov	r1, r9
	str	r1, [r0, #36]
	bl 0x0200c834
	mov	r2, r9
	str	r2, [r0, #40]
	bl 0x0200c834
	mov	r3, r9
	str	r3, [r0, #44]
	movs	r0, #1
	bl 0x0200c63c
	movs	r0, #247
	movs	r1, #128
	ldr	r2, [pc, #512]
	movs	r3, #0
	lsls	r1, r1, #16
	lsls	r0, r0, #16
	bl 0x0200c824
	bl 0x0200c684
	movs	r0, #1
	bl 0x0200c63c
	ldr	r0, [pc, #492]
	movs	r1, #1
	bl 0x0200c874
	movs	r0, #128
	movs	r1, #2
	lsls	r0, r0, #9
	bl 0x0200c874
	movs	r0, #30
	bl 0x0200c87c
	movs	r0, #30
	bl 0x0200c63c
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #464]
	bl 0x0200c644
	ldr	r1, [pc, #460]
	movs	r0, #19
	bl 0x0200c744
	movs	r0, #128
	lsls	r0, r0, #10
	ldr	r1, [pc, #452]
	bl 0x0200c81c
	movs	r0, #175
	movs	r1, #192
	lsls	r0, r0, #16
	lsls	r1, r1, #15
	ldr	r2, [pc, #444]
	movs	r3, #1
	bl 0x0200c824
	adds	r5, r7, #0
	adds	r5, #102
.L_02003662:
	movs	r0, #1
	bl 0x0200c63c
	movs	r0, #0
	ldrsh	r3, [r5, r0]
	cmp	r3, #8
	bne.n	.L_02003662
	movs	r1, #0
	movs	r0, #0
	bl 0x0200c874
	movs	r0, #60
	bl 0x0200c87c
	movs	r0, #60
	bl 0x0200c63c
	bl 0x0200c6c4
	bl 0x0200c834
	movs	r1, #128
	lsls	r1, r1, #24
	str	r1, [r0, #56]
	mov	r8, r1
	bl 0x0200c834
	mov	r2, r8
	str	r2, [r0, #60]
	bl 0x0200c834
	mov	r3, r8
	str	r3, [r0, #64]
	bl 0x0200c834
	movs	r5, #0
	str	r5, [r0, #36]
	bl 0x0200c834
	str	r5, [r0, #40]
	bl 0x0200c834
	str	r5, [r0, #44]
	ldr	r0, [pc, #332]
	bl 0x0200c64c
	movs	r0, #19
	bl 0x0200c754
	movs	r0, #1
	bl 0x0200c63c
	movs	r0, #19
	movs	r1, #0
	bl 0x0200c794
	mov	r2, fp
	movs	r3, #160
	lsls	r3, r3, #9
	adds	r2, #35
	movs	r0, #2
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	strb	r0, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #10
	mov	r1, fp
	str	r3, [r1, #24]
	movs	r0, #1
	str	r2, [r7, #24]
	str	r2, [r7, #28]
	str	r5, [r7, #8]
	str	r5, [r7, #16]
	str	r5, [r7, #56]
	str	r5, [r7, #64]
	mov	sl, r2
	bl 0x0200c63c
	movs	r0, #23
	movs	r1, #8
	bl 0x0200c794
	movs	r1, #169
	movs	r2, #158
	movs	r0, #9
	lsls	r1, r1, #16
	lsls	r2, r2, #19
	bl 0x0200c78c
	movs	r1, #192
	movs	r2, #0
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200c7fc
	movs	r0, #9
	movs	r1, #9
	bl 0x0200c794
	movs	r1, #151
	movs	r0, #26
	lsls	r1, r1, #16
	ldr	r2, [pc, #232]
	bl 0x0200c78c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200c7fc
	movs	r0, #26
	movs	r1, #5
	bl 0x0200c794
	movs	r1, #170
	movs	r0, #8
	lsls	r1, r1, #16
	ldr	r2, [pc, #204]
	bl 0x0200c78c
	movs	r1, #192
	movs	r2, #0
	movs	r0, #8
	lsls	r1, r1, #7
	bl 0x0200c7fc
	movs	r0, #8
	movs	r1, #5
	bl 0x0200c794
	movs	r1, #185
	movs	r0, #0
	lsls	r1, r1, #16
	ldr	r2, [pc, #176]
	bl 0x0200c78c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #0
	lsls	r1, r1, #6
	bl 0x0200c7fc
	movs	r0, #0
	movs	r1, #17
	bl 0x0200c794
	movs	r1, #169
	movs	r2, #173
	movs	r0, #22
	lsls	r1, r1, #16
	lsls	r2, r2, #19
	bl 0x0200c78c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #22
	lsls	r1, r1, #7
	bl 0x0200c7fc
	movs	r0, #22
	movs	r1, #0
	bl 0x0200c794
	movs	r0, #166
	movs	r1, #0
	ldr	r2, [pc, #116]
	lsls	r0, r0, #16
	movs	r3, #0
	bl 0x0200c824
	bl 0x0200c684
	ldr	r3, [sp, #0]
	mov	r0, r8
	strb	r5, [r3, #0]
	str	r0, [r7, #56]
	str	r0, [r7, #60]
	str	r0, [r7, #64]
	bl 0x0200bc48
	movs	r1, #218
	movs	r2, #147
	movs	r0, #27
	lsls	r1, r1, #16
	lsls	r2, r2, #19
	bl 0x0200c78c
	movs	r0, #210
	ldr	r2, [pc, #72]
	movs	r3, #0
	lsls	r0, r0, #16
	movs	r1, #0
	bl 0x0200c824
	b.n	.L_0200382c
	.2byte 0x0000
	.4byte 0x0200ceb4
	.4byte 0x0000e666
	.4byte 0x043c0000
	.4byte 0x0000cccc
	.4byte 0x03820000
	.4byte 0x03950000
	.4byte 0x00010003
	.4byte 0x0200bce5
	.4byte 0x0200cedc
	.4byte 0x000007ae
	.4byte 0x043e0000
	.4byte 0x050c0000
	.4byte 0x05210000
	.4byte 0x05350000
	.4byte 0x05390000
	.2byte 0x0000
	.2byte 0x04ac
.L_0200382c:
	bl 0x0200c684
	mov	r1, sl
	str	r1, [r6, #24]
	str	r1, [r6, #28]
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #644]
	bl 0x0200c644
	movs	r0, #10
	bl 0x0200c754
	movs	r0, #24
	bl 0x0200c754
	movs	r0, #25
	bl 0x0200c754
	movs	r0, #1
	bl 0x0200c63c
	movs	r0, #10
	bl 0x0200c72c
	adds	r6, r0, #0
	ldr	r2, [r6, #80]
	adds	r1, r6, #0
	adds	r1, #35
	mov	fp, r2
	ldrb	r2, [r1, #0]
	movs	r3, #254
	movs	r0, #128
	mov	sl, r3
	lsls	r0, r0, #9
	ands	r3, r2
	strb	r3, [r1, #0]
	str	r0, [r6, #24]
	str	r0, [r6, #28]
	mov	r1, fp
	movs	r3, #208
	ldrb	r2, [r1, #9]
	subs	r5, #13
	lsls	r3, r3, #8
	strh	r3, [r6, #6]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r1, #9]
	movs	r0, #10
	movs	r1, #0
	bl 0x0200c794
	movs	r0, #24
	bl 0x0200c72c
	adds	r6, r0, #0
	ldr	r2, [r6, #80]
	adds	r1, r6, #0
	adds	r1, #35
	mov	fp, r2
	ldrb	r2, [r1, #0]
	mov	r3, sl
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	movs	r0, #176
	mov	r3, fp
	ldrb	r2, [r3, #9]
	lsls	r0, r0, #8
	mov	r9, r0
	adds	r3, r5, #0
	ands	r3, r2
	mov	r1, r9
	mov	r0, fp
	strb	r3, [r0, #9]
	strh	r1, [r6, #6]
	movs	r0, #24
	movs	r1, #5
	bl 0x0200c794
	movs	r0, #25
	bl 0x0200c72c
	adds	r6, r0, #0
	ldr	r1, [r6, #80]
	mov	fp, r1
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	mov	r3, sl
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #9
	strb	r3, [r1, #0]
	str	r2, [r6, #24]
	str	r2, [r6, #28]
	mov	r0, fp
	ldrb	r2, [r0, #9]
	mov	r3, r9
	strh	r3, [r6, #6]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #9]
	movs	r1, #5
	movs	r0, #25
	bl 0x0200c794
	movs	r0, #27
	bl 0x0200c72c
	adds	r6, r0, #0
	ldr	r1, [r6, #80]
	mov	fp, r1
	bl 0x0200bc48
	movs	r3, #192
	lsls	r3, r3, #14
	movs	r1, #214
	movs	r2, #152
	str	r3, [r7, #12]
	lsls	r1, r1, #16
	mov	r3, r8
	lsls	r2, r2, #19
	str	r1, [r7, #8]
	str	r2, [r7, #16]
	str	r3, [r7, #56]
	str	r3, [r7, #60]
	str	r3, [r7, #64]
	mov	r0, fp
	ldrb	r3, [r0, #9]
	ands	r5, r3
	movs	r3, #4
	orrs	r5, r3
	strb	r5, [r0, #9]
	movs	r0, #27
	bl 0x0200c78c
	movs	r1, #192
	movs	r0, #24
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200c7fc
	movs	r1, #192
	movs	r2, #20
	lsls	r1, r1, #8
	movs	r0, #25
	bl 0x0200c7fc
	movs	r0, #179
	lsls	r0, r0, #1
	bl 0x0200c6e4
	movs	r0, #0
	bl 0x0200c6a4
	movs	r0, #1
	bl 0x0200c6a4
	movs	r0, #2
	bl 0x0200c6a4
	movs	r0, #3
	bl 0x0200c6a4
	movs	r0, #4
	bl 0x0200c6a4
	movs	r0, #5
	bl 0x0200c6a4
	ldr	r0, [pc, #312]
	movs	r1, #1
	bl 0x0200c874
	movs	r0, #128
	movs	r1, #2
	lsls	r0, r0, #9
	bl 0x0200c874
	movs	r0, #120
	bl 0x0200c87c
	movs	r0, #160
	bl 0x0200c63c
	ldr	r0, [pc, #288]
	movs	r1, #1
	bl 0x0200c874
	movs	r1, #2
	ldr	r0, [pc, #276]
	bl 0x0200c874
	movs	r0, #80
	bl 0x0200c87c
	movs	r0, #80
	bl 0x0200c6f4
	movs	r0, #100
	bl 0x0200c6f4
	ldr	r0, [pc, #244]
	bl 0x0200c64c
	ldr	r3, [r6, #24]
	mov	r1, fp
	movs	r0, #179
	str	r3, [r1, #24]
	lsls	r0, r0, #1
	bl 0x0200c6ec
	movs	r0, #0
	bl 0x0200c69c
	movs	r0, #1
	bl 0x0200c69c
	movs	r0, #2
	bl 0x0200c69c
	movs	r0, #3
	bl 0x0200c69c
	movs	r0, #4
	bl 0x0200c69c
	movs	r0, #5
	bl 0x0200c69c
	bl 0x0200c0f0
	movs	r1, #165
	ldr	r2, [pc, #196]
	movs	r0, #9
	lsls	r1, r1, #16
	bl 0x0200c78c
	movs	r1, #1
	movs	r0, #9
	bl 0x0200c794
	movs	r0, #9
	bl 0x0200c72c
	movs	r2, #224
	lsls	r2, r2, #8
	mov	r8, r2
	adds	r7, r0, #0
	mov	r3, r8
	strh	r3, [r7, #6]
	bl 0x0200c654
	movs	r1, #90
	bl 0x0200c634
	adds	r3, r7, #0
	ldr	r5, [pc, #152]
	adds	r0, #60
	adds	r3, #100
	adds	r2, r7, #0
	strh	r0, [r3, #0]
	adds	r2, #102
	movs	r3, #1
	strh	r3, [r2, #0]
	adds	r1, r5, #0
	movs	r0, #9
	bl 0x0200c744
	movs	r1, #165
	ldr	r2, [pc, #128]
	movs	r0, #26
	lsls	r1, r1, #16
	bl 0x0200c78c
	movs	r1, #1
	movs	r0, #26
	bl 0x0200c794
	movs	r0, #26
	bl 0x0200c72c
	adds	r7, r0, #0
	mov	r0, r8
	strh	r0, [r7, #6]
	bl 0x0200c654
	movs	r1, #90
	bl 0x0200c634
	adds	r3, r7, #0
	adds	r0, #60
	adds	r3, #100
	ldr	r1, [pc, #60]
	strh	r0, [r3, #0]
	adds	r3, #2
	strh	r1, [r3, #0]
	movs	r0, #26
	adds	r1, r5, #0
	bl 0x0200c744
	movs	r1, #152
	ldr	r2, [pc, #68]
	movs	r0, #22
	lsls	r1, r1, #16
	bl 0x0200c78c
	movs	r1, #1
	movs	r0, #22
	bl 0x0200c794
	movs	r0, #22
	bl 0x0200c72c
	mov	r2, r8
	adds	r7, r0, #0
	strh	r2, [r7, #6]
	bl 0x0200c654
	movs	r1, #90
	bl 0x0200c634
	adds	r3, r7, #0
	b.n	.L_02003adc
	.4byte 0x00000002
	.4byte 0x0200be19
	.4byte 0x00010003
	.4byte 0x00007fff
	.4byte 0x04cd0000
	.4byte 0x0200cec8
	.4byte 0x04e60000
	.2byte 0x0000
	.2byte 0x0505
.L_02003adc:
	adds	r0, #60
	adds	r3, #100
	adds	r2, r7, #0
	strh	r0, [r3, #0]
	adds	r2, #102
	movs	r3, #3
	strh	r3, [r2, #0]
	adds	r1, r5, #0
	movs	r0, #22
	bl 0x0200c744
	movs	r1, #180
	ldr	r2, [pc, #312]
	lsls	r1, r1, #16
	movs	r0, #8
	bl 0x0200c78c
	movs	r0, #8
	bl 0x0200c72c
	mov	r3, r8
	adds	r7, r0, #0
	strh	r3, [r7, #6]
	bl 0x0200c654
	movs	r1, #90
	bl 0x0200c634
	adds	r3, r7, #0
	adds	r0, #60
	adds	r3, #100
	adds	r2, r7, #0
	adds	r2, #102
	strh	r0, [r3, #0]
	movs	r3, #4
	strh	r3, [r2, #0]
	adds	r1, r5, #0
	movs	r0, #8
	bl 0x0200c744
	movs	r1, #6
	movs	r0, #8
	bl 0x0200c794
	movs	r0, #22
	bl 0x0200c72c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	mov	r3, sl
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #8
	bl 0x0200c72c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	mov	r1, sl
	ands	r1, r3
	strb	r1, [r0, #0]
	mov	sl, r1
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #216]
	bl 0x0200c644
	movs	r1, #181
	lsls	r1, r1, #16
	ldr	r2, [pc, #208]
	movs	r0, #0
	bl 0x0200c78c
	movs	r0, #0
	bl 0x0200c72c
	mov	r2, r8
	strh	r2, [r0, #6]
	movs	r1, #1
	movs	r0, #0
	bl 0x0200c794
	movs	r0, #181
	movs	r3, #0
	lsls	r0, r0, #16
	movs	r1, #0
	ldr	r2, [pc, #176]
	bl 0x0200c824
	bl 0x0200c684
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c78c
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c78c
	movs	r0, #24
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c78c
	movs	r0, #25
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c78c
	movs	r0, #23
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c78c
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200c78c
	movs	r1, #144
	movs	r0, #17
	lsls	r1, r1, #16
	ldr	r2, [pc, #104]
	bl 0x0200c78c
	movs	r1, #138
	ldr	r2, [pc, #100]
	lsls	r1, r1, #17
	movs	r0, #18
	bl 0x0200c78c
	movs	r0, #60
	bl 0x0200c63c
	ldr	r0, [pc, #88]
	movs	r1, #1
	bl 0x0200c874
	movs	r0, #128
	movs	r1, #2
	lsls	r0, r0, #9
	bl 0x0200c874
	movs	r0, #80
	bl 0x0200c87c
	movs	r0, #60
	bl 0x0200c6f4
	bl 0x0200c8ac
	movs	r0, #60
	bl 0x0200c6f4
	movs	r0, #1
	bl 0x0200c71c
	bl 0x0200c86c
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x051f0000
	.4byte 0x0200c5b9
	.4byte 0x04f90000
	.4byte 0x042e0000
	.4byte 0x04f60000
	.2byte 0x0003
	.2byte 0x0001
	.global Func_02003c48
	.thumb_func
Func_02003c48:
	push {lr}
	movs r0, #20
	bl 0x0200c63c
	movs r0, #179
	lsls r0, r0, #1
	bl 0x0200c6e4
	movs r0, #0
	bl 0x0200c6a4
	movs r0, #1
	bl 0x0200c6a4
	movs r0, #2
	bl 0x0200c6a4
	movs r0, #3
	bl 0x0200c6a4
	movs r0, #4
	bl 0x0200c6a4
	movs r0, #5
	bl 0x0200c6a4
	ldr r0, [pc, #96]
	movs r1, #1
	bl 0x0200c874
	movs r0, #128
	movs r1, #2
	lsls r0, r0, #9
	bl 0x0200c874
	movs r0, #1
	bl 0x0200c87c
	movs r0, #120
	bl 0x0200c63c
	movs r1, #0
	movs r0, #0
	bl 0x0200c874
	movs r0, #60
	bl 0x0200c87c
	movs r0, #60
	bl 0x0200c63c
	movs r0, #179
	lsls r0, r0, #1
	bl 0x0200c6ec
	movs r0, #0
	bl 0x0200c69c
	movs r0, #1
	bl 0x0200c69c
	movs r0, #2
	bl 0x0200c69c
	movs r0, #3
	bl 0x0200c69c
	movs r0, #4
	bl 0x0200c69c
	movs r0, #5
	bl 0x0200c69c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00010003
	.global Func_02003ce4
	.thumb_func
Func_02003ce4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #19
	bl 0x0200c72c
	adds r7, r0, #0
	movs r0, #27
	bl 0x0200c72c
	adds r5, r0, #0
	ldr r1, [r5, #80]
	adds r6, r7, #0
	adds r6, #100
	mov r8, r1
	movs r1, #0
	ldrsh r3, [r6, r1]
	ldrh r2, [r6]
	cmp r3, #0
	beq .L_02003ce4_0
	cmp r3, #60
	bne .L_02003ce4_1
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl 0x0200c6bc
	ldrh r2, [r6]
.L_02003ce4_1:
	movs r1, #160
	lsls r3, r2, #16
	lsls r1, r1, #14
	cmp r3, r1
	bne .L_02003ce4_2
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl 0x0200c6bc
	ldrh r2, [r6]
.L_02003ce4_2:
	movs r1, #240
	lsls r3, r2, #16
	lsls r1, r1, #13
	cmp r3, r1
	bne .L_02003ce4_3
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl 0x0200c6bc
	ldrh r2, [r6]
.L_02003ce4_3:
	movs r1, #160
	lsls r3, r2, #16
	lsls r1, r1, #13
	cmp r3, r1
	bne .L_02003ce4_4
	movs r0, #1
	movs r1, #1
	ldr r2, [pc, #148]
	negs r0, r0
	negs r1, r1
	bl 0x0200c6bc
	ldrh r2, [r6]
.L_02003ce4_4:
	subs r3, r2, #1
	strh r3, [r6]
.L_02003ce4_0:
	ldr r2, [r7, #8]
	str r2, [r5, #8]
	ldr r3, [r7, #16]
	str r2, [r5, #56]
	mov r2, r8
	str r3, [r5, #16]
	adds r2, #35
	movs r3, #10
	strb r3, [r2]
	ldr r3, [pc, #120]
	ldr r2, [r3]
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	beq .L_02003ce4_5
	adds r3, r7, #0
	adds r3, #102
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r0, r3, #1
	cmp r0, #8
	bhi .L_02003ce4_6
	ldr r2, [pc, #96]
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
	pop {r4, r6, r7, pc}
	.2byte 0x0200
	.2byte 0xbddc
	.2byte 0x0200
	.2byte 0xbddc
	.2byte 0x0200
	.2byte 0xbdd6
	.2byte 0x0200
	.2byte 0xbdd0
	.2byte 0x0200
	.2byte 0xbddc
	.2byte 0x0200
	.2byte 0xbddc
	.2byte 0x0200
	.2byte 0xbddc
	.2byte 0x0200
	.2byte 0xbddc
	.2byte 0x0200
	.2byte 0x69ab
	.2byte 0x4a0e
	.2byte 0xe004
	.2byte 0x69ab
	.2byte 0x4a0d
	.2byte 0xe001
	.2byte 0x69ab
	.2byte 0x4a0d
	.2byte 0x189b
	.2byte 0x61ab
	.2byte 0x69eb
	.2byte 0x189b
	.2byte 0x61eb
.L_02003ce4_6:
	ldr r3, [r5, #24]
	mov r1, r8
	str r3, [r1, #24]
	b .L_02003ce4_7
.L_02003ce4_5:
	mov r3, r8
	str r2, [r3, #24]
.L_02003ce4_7:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0000e666
	.4byte 0x03001e40
	.4byte 0x0200bdac
	.2byte 0x0a3d
	.2byte 0x0000
	.2byte 0x051e
	.2byte 0x0000
	.2byte 0xf852
	.2byte 0xffff
	.global Func_02003e18
	.thumb_func
Func_02003e18:
	push {lr}
	movs r0, #27
	bl 0x0200c72c
	ldr r3, [pc, #32]
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	ldr r1, [r0, #80]
	cmp r3, #0
	beq .L_02003e18_0
	adds r2, r1, #0
	adds r2, #35
	movs r3, #2
	b .L_02003e18_1
.L_02003e18_0:
	adds r2, r1, #0
	adds r2, #35
	movs r3, #64
.L_02003e18_1:
	strb r3, [r2]
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e40
	.global Func_02003e48
	.thumb_func
Func_02003e48:
	push {r5, r6, lr}
	movs r0, #0
	bl 0x0200c72c
	adds r6, r0, #0
	movs r0, #8
	bl 0x0200c72c
	adds r5, r0, #0
	bl 0x0200c6fc
	ldr r0, [pc, #320]
	bl 0x0200c6dc
	cmp r0, #0
	beq .L_02003e48_0
	movs r0, #8
	bl 0x0200c754
	movs r0, #10
	bl 0x0200c6f4
	movs r0, #8
	movs r1, #2
	bl 0x0200c7b4
	movs r0, #40
	bl 0x0200c6f4
	movs r2, #6
	ldrsh r3, [r6, r2]
	cmp r3, #0
	blt .L_02003e48_1
	movs r0, #8
	movs r1, #7
	bl 0x0200c794
	b .L_02003e48_2
.L_02003e48_1:
	movs r0, #8
	movs r1, #8
	bl 0x0200c794
.L_02003e48_2:
	movs r1, #2
	movs r0, #8
	bl 0x0200c7b4
	movs r0, #20
	bl 0x0200c6f4
	ldr r0, [pc, #248]
	bl 0x0200c7d4
	movs r0, #8
	movs r1, #0
	bl 0x0200c7e4
	ldr r1, [pc, #236]
	movs r0, #8
	bl 0x0200c744
	movs r0, #8
	movs r1, #6
	bl 0x0200c794
	b .L_02003e48_3
.L_02003e48_0:
	movs r0, #8
	bl 0x0200c754
	movs r3, #128
	lsls r3, r3, #9
	movs r1, #128
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r2, #0
	movs r0, #8
	lsls r1, r1, #5
	bl 0x0200c7fc
	movs r2, #6
	ldrsh r3, [r6, r2]
	cmp r3, #0
	blt .L_02003e48_4
	movs r0, #8
	movs r1, #7
	bl 0x0200c794
	b .L_02003e48_5
.L_02003e48_4:
	movs r0, #8
	movs r1, #8
	bl 0x0200c794
.L_02003e48_5:
	movs r0, #20
	bl 0x0200c6f4
	ldr r0, [pc, #164]
	bl 0x0200c7d4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c7ec
	movs r0, #8
	movs r1, #1
	bl 0x0200c794
	movs r2, #0
	movs r1, #4
	movs r0, #8
	bl 0x0200c7a4
	movs r0, #80
	bl 0x0200c6f4
	movs r0, #8
	movs r1, #2
	bl 0x0200c7b4
	movs r0, #40
	bl 0x0200c6f4
	movs r2, #6
	ldrsh r3, [r6, r2]
	cmp r3, #0
	blt .L_02003e48_6
	movs r0, #8
	movs r1, #7
	bl 0x0200c794
	b .L_02003e48_7
.L_02003e48_6:
	movs r0, #8
	movs r1, #8
	bl 0x0200c794
.L_02003e48_7:
	movs r0, #2
	bl 0x0200c6f4
	movs r2, #0
	movs r1, #2
	movs r0, #8
	bl 0x0200c7a4
	movs r0, #60
	bl 0x0200c6f4
	movs r1, #2
	movs r0, #8
	bl 0x0200c7b4
	movs r0, #20
	bl 0x0200c6f4
	movs r0, #8
	movs r1, #0
	bl 0x0200c7e4
	ldr r1, [pc, #36]
	movs r0, #8
	bl 0x0200c744
	movs r0, #8
	movs r1, #6
	bl 0x0200c794
	ldr r0, [pc, #12]
	bl 0x0200c6e4
.L_02003e48_3:
	bl 0x0200c704
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00000305
	.4byte 0x00000ed2
	.4byte 0x0200cec8
	.4byte 0x00000ed1
	.global Func_02003fb0
	.thumb_func
Func_02003fb0:
	push {lr}
	bl 0x0200c6fc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl 0x0200c824
	movs r0, #22
	bl 0x0200c754
	ldr r0, [pc, #260]
	bl 0x0200c64c
	movs r1, #240
	movs r2, #174
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #3
	bl 0x0200c77c
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200c78c
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #20
	movs r0, #22
	bl 0x0200c7fc
	movs r0, #22
	bl 0x0200c72c
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r1, #249
	movs r2, #155
	strb r3, [r0]
	lsls r2, r2, #19
	lsls r1, r1, #16
	movs r0, #22
	bl 0x0200c78c
	movs r0, #1
	bl 0x0200c63c
	ldr r0, [pc, #184]
	bl 0x0200c7d4
	ldr r0, [pc, #184]
	movs r1, #0
	bl 0x0200c7e4
	movs r1, #172
	ldr r2, [pc, #176]
	lsls r1, r1, #16
	movs r0, #22
	bl 0x0200c78c
	movs r0, #1
	bl 0x0200c63c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl 0x0200c81c
	movs r0, #162
	movs r3, #1
	ldr r2, [pc, #148]
	movs r1, #0
	lsls r0, r0, #16
	bl 0x0200c824
	bl 0x0200c82c
	movs r0, #40
	bl 0x0200c6f4
	movs r0, #22
	movs r1, #4
	bl 0x0200c79c
	ldr r0, [pc, #112]
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r1, #192
	movs r2, #20
	movs r0, #22
	lsls r1, r1, #8
	bl 0x0200c7fc
	movs r0, #22
	movs r1, #2
	bl 0x0200c7b4
	ldr r0, [pc, #84]
	movs r1, #0
	movs r2, #10
	bl 0x0200c7ec
	movs r1, #128
	movs r2, #20
	movs r0, #22
	lsls r1, r1, #5
	bl 0x0200c7fc
	movs r0, #22
	movs r1, #3
	bl 0x0200c79c
	movs r1, #128
	movs r2, #128
	movs r0, #22
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200c73c
	movs r0, #22
	movs r1, #165
	ldr r2, [pc, #48]
	bl 0x0200c77c
	movs r2, #179
	movs r0, #22
	movs r1, #195
	lsls r2, r2, #3
	bl 0x0200c77c
	ldr r0, [pc, #32]
	bl 0x0200c6e4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200c5b9
	.4byte 0x00000ed3
	.4byte 0x00001016
	.4byte 0x04fe0000
	.4byte 0x05050000
	.4byte 0x00000514
	.4byte 0x00000842
	.global Func_020040f0
	.thumb_func
Func_020040f0:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	sub sp, #8
	movs r2, #6
	movs r3, #3
	str r2, [sp, #0]
	str r3, [sp, #4]
	mov r9, r2
	mov r10, r3
	movs r0, #16
	movs r1, #96
	movs r2, #11
	movs r3, #73
	bl 0x0200c694
	movs r2, #10
	str r2, [sp, #4]
	movs r6, #14
	mov r8, r2
	movs r0, #16
	movs r1, #96
	movs r2, #34
	movs r3, #68
	str r6, [sp, #0]
	bl 0x0200c694
	movs r5, #7
	movs r0, #16
	movs r1, #96
	movs r2, #64
	movs r3, #68
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200c694
	mov r3, r9
	mov r2, r10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #9
	movs r1, #95
	movs r2, #11
	movs r3, #73
	bl 0x0200c694
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #40
	movs r1, #94
	movs r2, #34
	movs r3, #68
	str r6, [sp, #0]
	bl 0x0200c694
	movs r2, #8
	str r2, [sp, #0]
	mov r8, r2
	movs r0, #54
	movs r1, #94
	movs r2, #64
	movs r3, #68
	str r5, [sp, #4]
	bl 0x0200c694
	movs r5, #1
	movs r0, #72
	movs r1, #75
	movs r2, #72
	movs r3, #76
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200c694
	movs r0, #72
	movs r1, #75
	movs r2, #74
	movs r3, #76
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200c694
	mov r2, r9
	movs r3, #75
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #7
	movs r1, #75
	movs r2, #1
	movs r3, #1
	bl 0x0200c6ac
	mov r2, r8
	movs r3, #71
	str r2, [sp, #0]
	str r3, [sp, #4]
.L_020041b6:
	movs r0, #8
	movs r1, #70
	movs r2, #3
	movs r3, #1
	bl 0x0200c6ac
.L_020041c2:
	movs r3, #72
	str r3, [sp, #4]
	movs r6, #9
	movs r0, #8
	movs r1, #70
	movs r2, #2
.L_020041ce:
	movs r3, #1
	str r6, [sp, #0]
	bl 0x0200c6ac
	movs r5, #73
	movs r0, #8
.L_020041da:
	movs r1, #70
	movs r2, #2
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200c6ac
.L_020041e8:
	mov r3, r8
	str r3, [sp, #0]
	movs r0, #11
	movs r1, #66
	movs r2, #1
	movs r3, #1
.L_020041f4:
	str r5, [sp, #4]
	bl 0x0200c6ac
	movs r3, #11
	str r3, [sp, #0]
	movs r0, #12
.L_02004200:
	movs r1, #66
	movs r2, #1
	movs r3, #4
	str r5, [sp, #4]
	bl 0x0200c6ac
.L_0200420c:
	mov r2, r9
	movs r3, #74
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #25
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200c6ac
	bl 0x0200c684
	sub sp, #-8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02004234
	.thumb_func
Func_02004234:
	push {lr}
	bl 0x0200c6fc
	ldr r3, [pc, #52]
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
	ldr r0, [pc, #32]
	bl 0x0200c6e4
	movs r1, #2
	movs r0, #12
	bl 0x0200c844
	movs r0, #144
	lsls r0, r0, #4
	bl 0x0200c6e4
	bl 0x0200c704
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0000087c
	.global Func_02004278
	.thumb_func
Func_02004278:
	push {lr}
	bl 0x0200c6fc
	ldr r3, [pc, #52]
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
	ldr r0, [pc, #32]
	bl 0x0200c6e4
	movs r1, #3
	movs r0, #12
	bl 0x0200c844
	movs r0, #144
	lsls r0, r0, #4
	bl 0x0200c6e4
	bl 0x0200c704
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0000087f
	.global Func_020042bc
	.thumb_func
Func_020042bc:
	push {r5, lr}
	ldr r3, [pc, #60]
	ldr r3, [r3]
	movs r2, #2
	ands r3, r2
	adds r5, r0, #0
	cmp r3, #0
	beq .L_020042bc_0
	movs r1, #7
	bl 0x0200c6cc
	b .L_020042bc_1
.L_020042bc_0:
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200c6cc
.L_020042bc_1:
	ldr r3, [pc, #32]
	ldr r1, [r3]
	ldr r2, [pc, #24]
	lsls r1, r1, #3
	adds r1, #16
	ldr r0, [r2]
	bl 0x0200c634
	cmp r0, #0
	bne .L_020042bc_2
	adds r0, r5, #0
	bl 0x0200c41c
.L_020042bc_2:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.4byte 0x0200d7fc
	.global Func_02004304
	.thumb_func
Func_02004304:
	push {r5, r6, lr}
	ldr r5, [pc, #60]
	ldr r3, [r5]
	movs r2, #1
	ands r3, r2
	adds r6, r0, #0
	cmp r3, #0
	beq .L_02004304_0
	ldr r0, [r5]
	movs r1, #6
	lsrs r0, r0, #1
	bl 0x0200c634
	adds r1, r0, #0
	adds r0, r6, #0
	bl 0x0200c6cc
.L_02004304_0:
	ldr r3, [pc, #32]
	ldr r1, [r3]
	lsls r1, r1, #3
	adds r1, #16
	ldr r0, [r5]
	bl 0x0200c634
	cmp r0, #0
	bne .L_02004304_1
	adds r0, r6, #0
	bl 0x0200c41c
.L_02004304_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.4byte 0x0200d7fc
	.global Func_0200434c
	.thumb_func
Func_0200434c:
	push {r5, lr}
	adds r5, r0, #0
	ldr r0, [pc, #32]
	ldr r3, [r0]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200434c_0
	ldr r0, [r0]
	movs r1, #6
	lsrs r0, r0, #1
	bl 0x0200c634
	adds r1, r0, #0
	adds r0, r5, #0
	bl 0x0200c6cc
.L_0200434c_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.global Func_02004378
	.thumb_func
Func_02004378:
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
	ble .L_02004378_0
	adds r0, r5, #0
	bl 0x0200c67c
	b .L_02004378_1
.L_02004378_0:
	lsls r0, r0, #10
	bl 0x0200c65c
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
.L_02004378_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_020043c8
	.thumb_func
Func_020043c8:
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
	ble .L_020043c8_0
	adds r0, r5, #0
	bl 0x0200c67c
	b .L_020043c8_1
.L_020043c8_0:
	lsls r0, r0, #10
	bl 0x0200c65c
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
.L_020043c8_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200441c
	.thumb_func
Func_0200441c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #80]
	ldr r3, [r3]
	adds r6, r0, #0
	movs r0, #152
	sub sp, #8
	mov r11, r3
	bl 0x0200c8b4
	movs r1, #63
	movs r7, #0
	mov r10, sp
	mov r9, r1
.L_0200441c_2:
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	ldr r1, [r6, #8]
	movs r0, #26
	bl 0x0200c674
	lsls r3, r7, #2
	mov r2, r10
	str r0, [r3, r2]
	cmp r0, #0
	beq .L_0200441c_0
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
	beq .L_0200441c_0
	b .L_0200441c_1
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x03001f30
.L_0200441c_1:
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200c66c
	adds r3, r5, #0
	adds r3, #38
	mov r2, r8
	strb r2, [r3]
	ldrb r0, [r5, #28]
	bl 0x0200c664
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
	b .L_0200441c_0
	.2byte 0x0000
	.4byte 0xfffffc00
	.4byte 0x03001b10
.L_0200441c_0:
	adds r7, #1
	cmp r7, #1
	ble .L_0200441c_2
	ldr r2, [sp, #0]
	ldr r3, [pc, #76]
	str r3, [r2, #108]
	ldr r3, [r6, #80]
	ldr r4, [r2, #80]
	ldrb r3, [r3, #9]
	movs r2, #13
	ldrb r0, [r4, #9]
	negs r2, r2
	movs r1, #12
	ands r1, r3
	adds r3, r2, #0
	ands r3, r0
	orrs r3, r1
	strb r3, [r4, #9]
	mov r3, r10
	ldr r0, [r3, #4]
	ldr r3, [r6, #80]
	ldr r4, [r0, #80]
	ldrb r1, [r3, #9]
	movs r3, #12
	ands r3, r1
	ldrb r1, [r4, #9]
	ands r2, r1
	orrs r2, r3
	ldr r3, [pc, #32]
	str r3, [r0, #108]
	adds r0, #35
	movs r3, #2
	strb r2, [r4, #9]
	strb r3, [r0]
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
	.4byte 0x0200c3c9
	.4byte 0x0200c379
	.global Func_02004550
	.thumb_func
Func_02004550:
	push {lr}
	movs r0, #140
	movs r1, #0
	bl 0x0200c894
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02004560
	.thumb_func
Func_02004560:
	push {lr}
	bl 0x0200c89c
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200456c
	.thumb_func
Func_0200456c:
	push {lr}
	movs r0, #32
	bl 0x0200c72c
	bl 0x0200c304
	movs r0, #33
	bl 0x0200c72c
	bl 0x0200c304
	movs r0, #30
	bl 0x0200c72c
	bl 0x0200c304
	ldr r3, [pc, #20]
	ldr r3, [r3]
	cmp r3, #0
	bne .L_0200456c_0
	movs r0, #29
	bl 0x0200c72c
	bl 0x0200c304
.L_0200456c_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200d7f8
	.global Func_020045a8
	.thumb_func
Func_020045a8:
	push {lr}
	movs r0, #19
	bl 0x0200c72c
	bl 0x0200c34c
	pop {r0}
	bx r0
	.global Func_020045b8
	.thumb_func
Func_020045b8:
	push {r5, r6, lr}
	movs r0, #0
	bl 0x0200c72c
	ldr r6, [r0, #80]
	movs r0, #22
	bl 0x0200c72c
	ldr r0, [r0, #80]
	ldrb r3, [r6, #9]
	movs r5, #13
	ldrb r1, [r0, #9]
	negs r5, r5
	movs r2, #12
	ands r2, r3
	adds r3, r5, #0
	ands r3, r1
	orrs r3, r2
	strb r3, [r0, #9]
	movs r0, #8
	bl 0x0200c72c
	ldrb r2, [r6, #9]
	ldr r1, [r0, #80]
	movs r3, #12
	ands r3, r2
	ldrb r2, [r1, #9]
	ands r5, r2
	orrs r5, r3
	strb r5, [r1, #9]
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020045fc
	.thumb_func
Func_020045fc:
	push {lr}
	bl 0x0200c6fc
	ldr r0, [pc, #16]
	movs r1, #1
	bl 0x0200c6d4
	bl 0x0200c704
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000ee4
	.global Func_02004618
	.thumb_func
Func_02004618:
	push {lr}
	bl 0x0200c6fc
	ldr r0, [pc, #16]
	movs r1, #1
	bl 0x0200c6d4
	bl 0x0200c704
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001120
	.include "games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/IMPORT.INC"
	.4byte 0x00000007
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00b90000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b30000
	.4byte 0x00000000
	.4byte 0x03440000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00e20000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00e20000
	.4byte 0x00000000
	.4byte 0x03b00000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01590000
	.4byte 0x00000000
	.4byte 0x02640000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01370000
	.4byte 0x00000000
	.4byte 0x027e0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x000000c0
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000180
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000018
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffe80
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffd00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000000c
	.4byte 0xc0020000
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x005e0000
	.4byte 0x00000000
	.4byte 0x02340000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x004c0000
	.4byte 0x00000000
	.4byte 0x02340000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00b20000
	.4byte 0x00000000
	.4byte 0x04a60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00950000
	.4byte 0x00000000
	.4byte 0x04aa0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c60000
	.4byte 0x00000000
	.4byte 0x04980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00aa0000
	.4byte 0x00000000
	.4byte 0x04ab0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00950000
	.4byte 0x00000000
	.4byte 0x04aa0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00950000
	.4byte 0x00000000
	.4byte 0x04970000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000002
	.4byte 0x00660000
	.4byte 0x00000000
	.4byte 0x04a50000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00660000
	.4byte 0x00000000
	.4byte 0x04c20000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00540000
	.4byte 0x00000000
	.4byte 0x04cc0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000002
	.4byte 0x00660000
	.4byte 0x00000000
	.4byte 0x04a50000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00660000
	.4byte 0x00000000
	.4byte 0x04c20000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x04cc0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00320000
	.4byte 0x00000000
	.4byte 0x04be0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x002e0000
	.4byte 0x00000000
	.4byte 0x046a0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00430000
	.4byte 0x00000000
	.4byte 0x044c0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00990000
	.4byte 0x00000000
	.4byte 0x04e30000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b10000
	.4byte 0x00000000
	.4byte 0x054d0000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x007b0000
	.4byte 0x00000000
	.4byte 0x04360000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00940000
	.4byte 0x00000000
	.4byte 0x04360000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00940000
	.4byte 0x00000000
	.4byte 0x04280000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00230000
	.4byte 0x00000000
	.4byte 0x04280000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00660000
	.4byte 0x00000000
	.4byte 0x04d40000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00940000
	.4byte 0x00000000
	.4byte 0x04e80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b10000
	.4byte 0x00000000
	.4byte 0x05170000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00890000
	.4byte 0x00000000
	.4byte 0x05630000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00720000
	.4byte 0x00000000
	.4byte 0x05880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x005f0000
	.4byte 0x00000000
	.4byte 0x05880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00530000
	.4byte 0x00000000
	.4byte 0x05930000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x009a0000
	.4byte 0x00000000
	.4byte 0x05960000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b30000
	.4byte 0x00000000
	.4byte 0x05760000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b30000
	.4byte 0x00000000
	.4byte 0x05510000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x051d0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00a50000
	.4byte 0x00000000
	.4byte 0x051a0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00b70000
	.4byte 0x00000000
	.4byte 0x052d0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00a60000
	.4byte 0x00000000
	.4byte 0x052d0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02008065
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00030000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00020000
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00800000
	.4byte 0x03a30000
	.4byte 0x00000001
	.4byte 0x0000001e
	.4byte 0x00000091
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x0000003c
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00e40000
	.4byte 0x00950000
	.4byte 0x03bd0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00c90000
	.4byte 0x00a00000
	.4byte 0x03d80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00ad0000
	.4byte 0x008a0000
	.4byte 0x03f30000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000005
	.4byte 0x00000003
	.4byte 0x00920000
	.4byte 0x00600000
	.4byte 0x040e0000
	.4byte 0x00000001
	.4byte 0x0000001e
	.4byte 0x00000091
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00008000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x0000003c
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000006
	.4byte 0x00000003
	.4byte 0x00a10000
	.4byte 0x006b0000
	.4byte 0x041a0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00009999
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000007
	.4byte 0x00000003
	.4byte 0x00b10000
	.4byte 0x00600000
	.4byte 0x04260000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00028000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000008
	.4byte 0x00000003
	.4byte 0x00c90000
	.4byte 0x004a0000
	.4byte 0x04320000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00030000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000009
	.4byte 0x00000003
	.4byte 0x00e10000
	.4byte 0xffc00000
	.4byte 0x043e0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x000000a7
	.4byte 0x40000501
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000101
	.4byte 0x400001c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000071
	.4byte 0x4000012f
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x0000001b
	.4byte 0x0000026d
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x0000001d
	.4byte 0x00000318
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000001ca
	.4byte 0x80000571
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000196
	.4byte 0x400002e7
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00000106
	.4byte 0x40000335
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00000154
	.4byte 0x40000388
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x00000146
	.4byte 0x40000476
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000176
	.4byte 0x400004e6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x00000066
	.4byte 0x400004c6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x00000065
	.4byte 0x400004c0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000d
	.4byte 0x00000092
	.4byte 0x400004a9
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000f
	.4byte 0x0000014f
	.4byte 0xc000038c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0010
	.4byte 0x000000b5
	.4byte 0xe00004f9
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00208005
	.4byte 0x00301006
	.4byte 0x00402006
	.4byte 0x00506007
	.4byte 0x00605007
	.4byte 0x00706008
	.4byte 0x00802007
	.4byte 0x00901007
	.4byte 0x00a01007
	.4byte 0x00b01007
	.4byte 0x00c0b008
	.4byte 0x00d0c008
	.4byte 0x01410003
	.4byte 0x000001ff
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0035
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x01b50000
	.4byte 0x00000000
	.4byte 0x02fa0000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x01420000
	.4byte 0x00000000
	.4byte 0x05990000
	.4byte 0x01024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x042e0000
	.4byte 0x01024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x01140000
	.4byte 0x00000000
	.4byte 0x04f60000
	.4byte 0x01024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00d2
	.4byte 0x00000001
	.4byte 0x01a40000
	.4byte 0x00000000
	.4byte 0x026b0000
	.4byte 0x00004000
	.4byte 0xffff0013
	.4byte 0x00000001
	.4byte 0x018c0000
	.4byte 0x00000000
	.4byte 0x026b0000
	.4byte 0x00000000
	.4byte 0xffff0024
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff003b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff003c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0030
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0120
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x007c0000
	.4byte 0x00000000
	.4byte 0x01770000
	.4byte 0x0002b000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00550000
	.4byte 0x00000000
	.4byte 0x01690000
	.4byte 0x0002d000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00870000
	.4byte 0x00000000
	.4byte 0x016b0000
	.4byte 0x0002b000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00640000
	.4byte 0x00000000
	.4byte 0x01770000
	.4byte 0x0002d000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x007c0000
	.4byte 0x00000000
	.4byte 0x01770000
	.4byte 0x0002b000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x00640000
	.4byte 0x00000000
	.4byte 0x01770000
	.4byte 0x0002d000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x020081d1
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x020081e5
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x020081f9
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x0200820d
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008241
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008279
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x020082e5
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008329
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x02008361
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x02008399
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte 0x020083cd
	.4byte 0x0000c602
	.4byte 0xffff000d
	.4byte 0x02008401
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x020096cd
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0200ad29
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00000ea6
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00000ea7
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00000ece
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00000ecf
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200be49
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001121
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x0200810d
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x02008135
	.4byte 0x00000002
	.4byte 0xffff0029
	.4byte 0x02008d5d
	.4byte 0x00000002
	.4byte 0xffff002a
	.4byte 0x02008f39
	.4byte 0x00000002
	.4byte 0xffff002b
	.4byte 0x02009155
	.4byte 0x00000002
	.4byte 0xffff002c
	.4byte 0x02009349
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x02009601
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte 0x02009685
	.4byte 0x00000002
	.4byte 0xffff0020
	.4byte 0x02009829
	.4byte 0x00000002
	.4byte 0xffff0021
	.4byte 0x0200998d
	.4byte 0x00000002
	.4byte 0xffff0022
	.4byte 0x02009b19
	.4byte 0x00000002
	.4byte 0xffff0023
	.4byte 0x0200a181
	.4byte 0x00000002
	.4byte 0xffff0024
	.4byte 0x0200aded
	.4byte 0x00000002
	.4byte 0x087c0010
	.4byte 0x0200c235
	.4byte 0x00000002
	.4byte 0x087f0011
	.4byte 0x0200c279
	.4byte 0x00000003
	.4byte 0xffff0018
	.4byte 0x0200c5fd
	.4byte 0x00000003
	.4byte 0xffff0013
	.4byte 0x0200c619
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00620000
	.4byte 0x00020002
	.4byte 0x00020002
	.4byte 0x00020062
	.4byte 0x00020002
	.4byte 0x0000ffff
	.4byte 0x00020060
	.4byte 0x00020002
	.4byte 0x00600002
	.4byte 0x00020002
	.4byte 0xffff0002
	.4byte 0x00620004
	.4byte 0x00020002
	.4byte 0x00060002
	.4byte 0x00020062
	.4byte 0x00020002
	.4byte 0x0004ffff
	.4byte 0x00020060
	.4byte 0x00020002
	.4byte 0x00600006
	.4byte 0x00020002
	.4byte 0xffff0002
	.4byte 0x00600000
	.4byte 0x00020002
	.4byte 0x00320002
	.4byte 0x0002002c
	.4byte 0x00020002
	.4byte 0x0004ffff
	.4byte 0x00020062
	.4byte 0x00020002
	.4byte 0x006c002c
	.4byte 0x00020002
	.4byte 0xffff0002
