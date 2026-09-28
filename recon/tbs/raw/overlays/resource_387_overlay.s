.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/ENTRY.INC"
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
	bl 0x020090b4
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
	bl 0x020090c4
	adds r0, r5, #0
	movs r1, #14
	bl 0x020091bc
	adds r0, r5, #0
	movs r1, #1
	bl 0x020090cc
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
	bl 0x020090b4
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
	bl 0x020090c4
	adds r0, r5, #0
	movs r1, #15
	bl 0x020091bc
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
	bl 0x02009144
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
	bl 0x020090b4
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
	bl 0x020090a4
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl 0x020090ac
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
	bl 0x020091bc
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
	bl 0x02009084
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
	bl 0x02009084
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200013c_9:
	bl 0x02009084
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
	bl 0x020090a4
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl 0x020090ac
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
	.4byte 0x020092dc
	.4byte 0x02008105
	.4byte 0xffff0000
	.global Func_02000314
	.thumb_func
Func_02000314:
	push {r5, lr}
	adds r5, r1, #0
	movs r1, #0
	bl 0x020091cc
	adds r0, r5, #0
	bl 0x02009124
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200032c
	.thumb_func
Func_0200032c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020092f8
	.global Func_02000334
	.thumb_func
Func_02000334:
	movs r0, #0
	bx lr
	.global Func_02000338
	.thumb_func
Func_02000338:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009358
	.global Func_02000340
	.thumb_func
Func_02000340:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009368
	.global Func_02000348
	.thumb_func
Func_02000348:
	push {r5, lr}
	movs r0, #9
	sub sp, #8
	bl 0x02009144
	ldr r3, [r0, #8]
	cmp r3, #0
	bge .L_02000348_0
	ldr r2, [pc, #104]
	adds r3, r3, r2
.L_02000348_0:
	ldr r0, [pc, #104]
	asrs r5, r3, #20
	bl 0x02009114
	ldr r0, [pc, #100]
	bl 0x02009114
	cmp r5, #15
	bne .L_02000348_1
	movs r3, #16
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #18
	movs r2, #1
	movs r3, #2
	bl 0x020090bc
	b .L_02000348_2
.L_02000348_1:
	cmp r5, #16
	bne .L_02000348_3
	movs r3, #18
	str r3, [sp, #4]
	movs r0, #48
	movs r1, #18
	movs r2, #1
	movs r3, #2
	str r5, [sp, #0]
	bl 0x020090bc
	ldr r0, [pc, #44]
	bl 0x0200910c
	b .L_02000348_2
.L_02000348_3:
	movs r3, #16
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #18
	movs r2, #1
	movs r3, #2
	bl 0x020090bc
	ldr r0, [pc, #20]
	bl 0x0200910c
.L_02000348_2:
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x000fffff
	.4byte 0x00000861
	.4byte 0x00000862
	.global Func_020003d0
	.thumb_func
Func_020003d0:
	push {r5, r6, lr}
	movs r0, #10
	sub sp, #8
	bl 0x02009144
	ldr r3, [r0, #8]
	cmp r3, #0
	bge .L_020003d0_0
	ldr r2, [pc, #80]
	adds r3, r3, r2
.L_020003d0_0:
	asrs r6, r3, #20
	cmp r6, #23
	bne .L_020003d0_1
	movs r0, #10
	bl 0x02009124
	movs r0, #10
	bl 0x02009144
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	movs r0, #10
	bl 0x02009144
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r0, #10
	bl 0x02009144
	movs r1, #0
	bl 0x020090c4
	movs r3, #17
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #17
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl 0x020090bc
	ldr r0, [pc, #16]
	bl 0x0200910c
.L_020003d0_1:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x000fffff
	.4byte 0x00000863
	.global Func_0200043c
	.thumb_func
Func_0200043c:
	push {r5, lr}
	ldr r3, [pc, #128]
	ldr r5, [r3]
	bl 0x0200912c
	movs r1, #8
	movs r0, #0
	bl 0x02009184
	movs r0, #20
	bl 0x02009124
	movs r0, #0
	ldr r1, [pc, #108]
	ldr r2, [pc, #108]
	bl 0x0200914c
	ldr r1, [pc, #100]
	ldr r2, [pc, #100]
	movs r0, #9
	bl 0x0200914c
	movs r0, #185
	bl 0x0200922c
	movs r2, #182
	lsls r2, r2, #1
	adds r5, r5, r2
	movs r2, #0
	ldrsh r3, [r5, r2]
	movs r5, #11
	lsls r3, r3, #1
	subs r5, r5, r3
	lsls r5, r5, #4
	adds r1, r5, #0
	movs r0, #0
	movs r2, #0
	bl 0x0200916c
	movs r2, #0
	adds r1, r5, #0
	movs r0, #9
	bl 0x0200916c
	movs r0, #0
	bl 0x02009174
	movs r0, #9
	bl 0x02009174
	movs r0, #20
	bl 0x02009124
	movs r0, #0
	movs r1, #1
	bl 0x02009184
	bl 0x02008348
	bl 0x02009224
	bl 0x02009134
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x00003333
	.4byte 0x00001999
	.global Func_020004cc
	.thumb_func
Func_020004cc:
	bx lr
	.2byte 0x0000
	.global Func_020004d0
	.thumb_func
Func_020004d0:
	bx lr
	.2byte 0x0000
	.global Func_020004d4
	.thumb_func
Func_020004d4:
	push {lr}
	bl 0x0200912c
	ldr r0, [pc, #12]
	bl 0x0200910c
	bl 0x02009134
	pop {r0}
	bx r0
	.4byte 0x00000866
	.global Func_020004ec
	.thumb_func
Func_020004ec:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009488
	.global Func_020004f4
	.thumb_func
Func_020004f4:
	push {r5, lr}
	movs r0, #162
	lsls r0, r0, #1
	sub sp, #8
	bl 0x0200910c
	movs r0, #10
	bl 0x02009124
	movs r0, #170
	bl 0x0200921c
	movs r1, #2
	movs r0, #11
	bl 0x02009184
	movs r0, #11
	bl 0x02009144
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	movs r0, #8
	bl 0x02009144
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #16
	orrs r3, r2
	strb r3, [r0]
	movs r0, #15
	bl 0x02009144
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #8
	orrs r3, r2
	strb r3, [r0]
	ldr r0, [pc, #296]
	bl 0x02009104
	cmp r0, #0
	beq .L_020004f4_0
	movs r3, #73
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #74
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl 0x020090bc
.L_020004f4_0:
	movs r0, #134
	lsls r0, r0, #4
	bl 0x02009104
	cmp r0, #0
	beq .L_020004f4_1
	movs r1, #136
	movs r2, #196
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #8
	bl 0x0200917c
	movs r0, #8
	bl 0x02009144
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #2
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #8
	bl 0x02009184
	movs r3, #8
	movs r5, #12
	str r3, [sp, #0]
	movs r0, #39
	movs r1, #12
	movs r2, #3
	movs r3, #1
	str r5, [sp, #4]
	bl 0x020090bc
	movs r3, #11
	str r3, [sp, #4]
	movs r0, #43
	movs r1, #11
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x020090bc
.L_020004f4_1:
	ldr r0, [pc, #184]
	bl 0x02009104
	cmp r0, #0
	beq .L_020004f4_2
	movs r1, #132
	movs r2, #156
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200917c
	movs r3, #16
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #48
	movs r1, #18
	movs r2, #1
	movs r3, #2
	bl 0x020090bc
	b .L_020004f4_3
.L_020004f4_2:
	ldr r0, [pc, #140]
	bl 0x02009104
	cmp r0, #0
	beq .L_020004f4_3
	movs r1, #140
	movs r2, #156
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200917c
	movs r3, #16
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #18
	movs r2, #1
	movs r3, #2
	bl 0x020090bc
.L_020004f4_3:
	ldr r0, [pc, #100]
	bl 0x02009104
	cmp r0, #0
	beq .L_020004f4_4
	movs r1, #188
	movs r2, #140
	lsls r2, r2, #17
	lsls r1, r1, #17
	movs r0, #10
	bl 0x0200917c
	movs r0, #10
	bl 0x02009144
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	movs r0, #10
	bl 0x02009144
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r0, #10
	bl 0x02009144
	movs r1, #0
	bl 0x020090c4
	movs r3, #23
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #54
	movs r1, #17
	movs r2, #1
	movs r3, #1
	bl 0x020090bc
.L_020004f4_4:
	movs r0, #0
	sub sp, #-8
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00000865
	.4byte 0x00000861
	.4byte 0x00000862
	.4byte 0x00000863
	.global Func_0200067c
	.thumb_func
Func_0200067c:
	push {lr}
	sub sp, #8
	bl 0x02009224
	bl 0x0200912c
	movs r0, #30
	bl 0x02009124
	ldr r0, [pc, #188]
	bl 0x020091c4
	movs r0, #0
	ldr r1, [pc, #184]
	ldr r2, [pc, #184]
	bl 0x0200914c
	movs r0, #1
	ldr r1, [pc, #172]
	ldr r2, [pc, #176]
	bl 0x0200914c
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl 0x020091dc
	movs r0, #0
	bl 0x02009144
	cmp r0, #0
	beq .L_0200067c_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200917c
.L_0200067c_0:
	movs r1, #132
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #168
	bl 0x02009164
	movs r1, #192
	movs r2, #20
	lsls r1, r1, #7
	movs r0, #1
	bl 0x020091dc
	movs r0, #20
	bl 0x02009124
	movs r1, #4
	movs r0, #1
	bl 0x0200918c
	movs r0, #20
	bl 0x02009124
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x020091d4
	movs r0, #0
	movs r1, #3
	bl 0x0200918c
	movs r0, #20
	bl 0x02009124
	ldr r0, [pc, #72]
	bl 0x02009104
	cmp r0, #0
	bne .L_0200067c_1
	movs r0, #1
	movs r1, #2
	bl 0x02009184
	movs r0, #0
	bl 0x02009144
	cmp r0, #0
	beq .L_0200067c_2
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x02009154
.L_0200067c_2:
	movs r0, #1
	bl 0x02009174
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200917c
	bl 0x02009134
	b .L_0200067c_3
	.4byte 0x0000138f
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00000855
.L_0200067c_1:
	movs r1, #180
	movs r2, #248
	movs r0, #2
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200917c
	movs r0, #2
	ldr r1, [pc, #1016]
	ldr r2, [pc, #1020]
	bl 0x0200914c
	movs r1, #136
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #248
	bl 0x02009164
	movs r1, #136
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #208
	bl 0x02009164
	movs r1, #160
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x020091dc
	movs r1, #2
	movs r0, #2
	bl 0x020091a4
	movs r0, #20
	bl 0x02009124
	movs r0, #2
	movs r1, #0
	movs r2, #20
	bl 0x020091d4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x020091dc
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #1
	bl 0x020091dc
	movs r0, #20
	bl 0x02009124
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x020091e4
	movs r1, #128
	movs r2, #60
	movs r0, #1
	lsls r1, r1, #1
	bl 0x020091e4
	movs r0, #2
	movs r1, #3
	bl 0x0200918c
	movs r1, #132
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #200
	bl 0x02009164
	movs r0, #0
	movs r1, #248
	movs r2, #168
	bl 0x0200915c
	movs r1, #248
	movs r2, #184
	movs r0, #2
	bl 0x02009164
	movs r0, #0
	bl 0x02009174
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x020091dc
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x020091dc
	movs r1, #232
	movs r2, #184
	movs r0, #2
	bl 0x02009164
	movs r0, #20
	bl 0x02009124
	movs r0, #2
	ldr r1, [pc, #816]
	movs r2, #60
	bl 0x020091e4
	movs r1, #224
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x020091dc
	movs r1, #4
	movs r0, #2
	bl 0x0200918c
	movs r0, #20
	bl 0x02009124
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x020091d4
	movs r0, #0
	movs r1, #3
	bl 0x02009184
	movs r1, #3
	movs r0, #1
	bl 0x0200918c
	movs r0, #20
	bl 0x02009124
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #20
	bl 0x020091dc
	movs r0, #2
	movs r1, #0
	movs r2, #120
	bl 0x020091d4
	movs r0, #0
	ldr r1, [pc, #724]
	movs r2, #0
	bl 0x020091e4
	movs r0, #1
	ldr r1, [pc, #716]
	movs r2, #60
	bl 0x020091e4
	movs r1, #1
	movs r2, #0
	movs r0, #0
	bl 0x020091b4
	movs r0, #60
	bl 0x02009124
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x020091dc
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #1
	bl 0x020091dc
	movs r0, #60
	bl 0x02009124
	movs r1, #131
	movs r2, #0
	movs r0, #2
	lsls r1, r1, #1
	bl 0x020091e4
	movs r1, #1
	movs r0, #2
	bl 0x0200919c
	movs r0, #30
	bl 0x02009124
	movs r0, #2
	movs r1, #0
	movs r2, #30
	bl 0x020091d4
	movs r1, #224
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x020091dc
	movs r0, #0
	movs r1, #2
	bl 0x0200919c
	movs r1, #2
	movs r0, #1
	bl 0x020091a4
	movs r0, #20
	bl 0x02009124
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x020091d4
	movs r0, #0
	movs r1, #3
	bl 0x02009184
	movs r1, #3
	movs r0, #1
	bl 0x0200918c
	movs r0, #20
	bl 0x02009124
	movs r1, #3
	movs r0, #2
	bl 0x0200918c
	movs r0, #20
	bl 0x02009124
	movs r1, #128
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x020091dc
	bl 0x02008d68
	movs r1, #1
	movs r0, #2
	bl 0x02009184
	movs r0, #20
	bl 0x02009124
	bl 0x02009224
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x020091e4
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #60
	bl 0x020091e4
	movs r2, #0
	movs r1, #2
	movs r0, #1
	bl 0x02009194
	movs r0, #20
	bl 0x02009124
	movs r0, #1
	movs r1, #20
	bl 0x02008314
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #184
	movs r0, #2
	bl 0x02009164
	movs r0, #10
	bl 0x02009124
	movs r0, #2
	movs r1, #1
	movs r2, #0
	bl 0x020091ac
	movs r0, #1
	movs r1, #2
	movs r2, #0
	bl 0x020091ac
	movs r2, #0
	movs r1, #2
	movs r0, #0
	bl 0x020091ac
	movs r0, #20
	bl 0x02009124
	movs r1, #3
	movs r0, #2
	bl 0x0200918c
	movs r0, #20
	bl 0x02009124
	movs r0, #2
	movs r1, #60
	bl 0x02008314
	movs r0, #0
	ldr r1, [pc, #384]
	movs r2, #0
	bl 0x020091e4
	movs r0, #1
	ldr r1, [pc, #376]
	movs r2, #60
	bl 0x020091e4
	movs r0, #0
	ldr r1, [pc, #368]
	movs r2, #0
	bl 0x020091e4
	ldr r1, [pc, #360]
	movs r2, #0
	movs r0, #1
	bl 0x020091e4
	movs r0, #60
	bl 0x02009124
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020091dc
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl 0x020091dc
	movs r0, #60
	bl 0x02009124
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x020091dc
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #0
	bl 0x020091dc
	movs r0, #10
	bl 0x02009124
	movs r0, #1
	movs r1, #20
	bl 0x02008314
	movs r2, #0
	ldr r1, [pc, #272]
	movs r0, #2
	bl 0x020091e4
	movs r0, #60
	bl 0x02009124
	movs r1, #4
	movs r0, #2
	bl 0x0200918c
	movs r0, #20
	bl 0x02009124
	movs r0, #2
	movs r1, #20
	bl 0x02008314
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x020091ac
	movs r0, #1
	movs r1, #2
	movs r2, #0
	bl 0x020091ac
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x020091e4
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #1
	bl 0x020091e4
	movs r0, #60
	bl 0x02009124
	movs r1, #2
	movs r0, #2
	bl 0x020091a4
	movs r0, #20
	bl 0x02009124
	movs r0, #2
	movs r1, #30
	bl 0x02008314
	movs r0, #0
	ldr r1, [pc, #168]
	movs r2, #0
	bl 0x020091e4
	movs r2, #0
	ldr r1, [pc, #156]
	movs r0, #1
	bl 0x020091e4
	movs r0, #80
	bl 0x02009124
	movs r1, #3
	movs r0, #2
	bl 0x0200918c
	movs r0, #20
	bl 0x02009124
	movs r0, #2
	movs r1, #20
	bl 0x02008314
	movs r0, #0
	movs r1, #1
	bl 0x0200919c
	movs r0, #1
	movs r1, #1
	bl 0x0200919c
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x020091ec
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x020091ec
	movs r0, #60
	bl 0x02009124
	movs r1, #4
	movs r0, #2
	bl 0x0200918c
	movs r0, #20
	bl 0x02009124
	movs r0, #2
	movs r1, #20
	bl 0x02008314
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x020091dc
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x020091dc
	movs r0, #80
	bl 0x02009124
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x020091ac
	movs r2, #0
	movs r1, #2
	movs r0, #1
	b .L_0200067c_6
	.2byte 0x0000
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00000105
	.4byte 0x00000101
.L_0200067c_6:
	bl 0x020091ac
	movs r0, #30
	bl 0x02009124
	movs r1, #3
	movs r0, #2
	bl 0x0200918c
	movs r0, #20
	bl 0x02009124
	movs r0, #2
	movs r1, #30
	bl 0x02008314
	movs r1, #4
	movs r0, #2
	bl 0x0200918c
	movs r0, #20
	bl 0x02009124
	movs r0, #2
	movs r1, #20
	bl 0x02008314
	movs r0, #0
	movs r1, #2
	bl 0x0200919c
	movs r1, #2
	movs r0, #1
	bl 0x020091a4
	movs r0, #20
	bl 0x02009124
	movs r1, #3
	movs r0, #2
	bl 0x0200918c
	movs r0, #20
	bl 0x02009124
	movs r0, #2
	movs r1, #40
	bl 0x02008314
	movs r0, #0
	movs r1, #3
	bl 0x02009184
	movs r1, #3
	movs r0, #1
	bl 0x0200918c
	movs r0, #20
	bl 0x02009124
	movs r1, #1
	movs r0, #2
	bl 0x0200913c
	movs r0, #60
	bl 0x02009124
	bl 0x02008fc8
	movs r1, #1
	movs r0, #2
	bl 0x020091a4
	movs r0, #20
	bl 0x02009124
	movs r2, #184
	movs r1, #248
	movs r0, #2
	bl 0x02009164
	movs r0, #20
	bl 0x02009124
	movs r0, #2
	movs r1, #20
	bl 0x02008314
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020091dc
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x020091dc
	movs r0, #120
	bl 0x02009124
	movs r0, #2
	movs r1, #30
	bl 0x02008314
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x020091ac
	movs r0, #1
	movs r1, #2
	movs r2, #0
	bl 0x020091ac
	movs r2, #0
	movs r1, #0
	movs r0, #2
	bl 0x020091ac
	movs r0, #20
	bl 0x02009124
	movs r0, #0
	movs r1, #3
	bl 0x02009184
	movs r0, #1
	movs r1, #3
	bl 0x02009184
	movs r1, #3
	movs r0, #2
	bl 0x0200918c
	movs r0, #50
	bl 0x02009124
	movs r0, #1
	ldr r1, [pc, #100]
	ldr r2, [pc, #100]
	bl 0x0200914c
	movs r0, #2
	ldr r1, [pc, #88]
	ldr r2, [pc, #92]
	bl 0x0200914c
	movs r0, #1
	movs r1, #248
	movs r2, #168
	bl 0x0200915c
	movs r0, #2
.L_0200067c_4:
	movs r1, #248
	movs r2, #168
	bl 0x02009164
	movs r1, #0
	movs r2, #0
	movs r0, #2
	bl 0x0200917c
	movs r0, #1
	bl 0x02009174
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200917c
	movs r3, #73
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #74
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl 0x020090bc
	ldr r0, [pc, #24]
	bl 0x0200910c
	bl 0x02009134
.L_0200067c_3:
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000cccc
.L_0200067c_5:
	.4byte 0x00006666
	.4byte 0x00000865
	.global Func_02000d04
	.thumb_func
Func_02000d04:
	push {r5, r6, lr}
	sub sp, #8
	bl 0x0200912c
	movs r0, #8
	bl 0x02009144
	ldr r3, [r0, #8]
	asrs r6, r3, #20
	cmp r6, #11
	bne .L_02000d04_0
	movs r0, #8
	bl 0x02008dd8
	movs r0, #8
	bl 0x02009144
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r3, #8
	str r3, [sp, #0]
	movs r5, #12
	movs r0, #39
	movs r1, #12
	movs r2, #3
	movs r3, #1
	str r5, [sp, #4]
	bl 0x020090bc
	movs r0, #43
	movs r1, #11
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x020090bc
	movs r0, #134
	lsls r0, r0, #4
	bl 0x0200910c
.L_02000d04_0:
	bl 0x02009134
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02000d68
	.thumb_func
Func_02000d68:
	push {r5, lr}
	ldr r3, [pc, #52]
	movs r0, #78
	movs r1, #1
	ldr r5, [r3]
	bl 0x020091fc
	movs r1, #15
	movs r0, #2
	bl 0x02009204
	ldr r3, [pc, #36]
	adds r5, r5, r3
	ldrb r2, [r5]
	movs r3, #8
	orrs r3, r2
	strb r3, [r5]
	bl 0x02009214
	movs r0, #1
	bl 0x020091f4
	bl 0x0200920c
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001f30
	.4byte 0x0000071c
	.global Func_02000da8
	.thumb_func
Func_02000da8:
	ldr r2, [r0, #80]
	ldr r1, [pc, #8]
	ldrh r3, [r2, #30]
	adds r3, r3, r1
	strh r3, [r2, #30]
	bx lr
	.4byte 0xfffff800
	.global Func_02000db8
	.thumb_func
Func_02000db8:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	adds r6, r1, #0
	movs r5, #60
.L_02000db8_1:
	cmp r5, #0
	beq .L_02000db8_0
	movs r0, #1
	bl 0x0200908c
	ldr r3, [r7, #12]
	subs r5, #1
	cmp r3, r6
	bgt .L_02000db8_1
.L_02000db8_0:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02000dd8
	.thumb_func
Func_02000dd8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #68
	mov r11, r0
	bl 0x02009144
	adds r6, r0, #0
	adds r5, r6, #0
	movs r3, #0
	adds r5, #85
	strb r3, [r5]
	mov r8, r3
.L_02000dd8_0:
	movs r0, #1
	bl 0x0200908c
	ldr r2, [r6, #80]
	ldr r1, [pc, #372]
	ldrh r3, [r2, #30]
	adds r3, r3, r1
	strh r3, [r2, #30]
	ldr r3, [r6, #80]
	ldrh r0, [r3, #30]
	bl 0x0200909c
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r3, [r6, #8]
	asrs r0, r0, #1
	subs r3, r3, r0
	str r3, [r6, #8]
	movs r2, #1
	movs r3, #128
	lsls r3, r3, #24
	add r8, r2
	str r3, [r6, #56]
	mov r3, r8
	cmp r3, #17
	bls .L_02000dd8_0
	ldr r3, [pc, #332]
	movs r1, #192
	movs r2, #192
	str r3, [r6, #108]
	mov r0, r11
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200914c
	movs r1, #160
	mov r0, r11
	movs r2, #192
	bl 0x02009154
	ldr r3, [pc, #308]
	str r3, [r6, #72]
	movs r3, #3
	strb r3, [r5]
	adds r3, r6, #0
	adds r3, #34
	movs r2, #0
	strb r2, [r3]
	mov r0, r11
	bl 0x02009174
	movs r1, #128
	lsls r1, r1, #14
	adds r0, r6, #0
	bl 0x02008db8
	movs r0, #188
	bl 0x0200922c
	movs r0, #160
	lsls r0, r0, #11
	movs r2, #128
	adds r1, r0, #0
	lsls r2, r2, #9
	bl 0x020090d4
	movs r0, #141
	bl 0x0200922c
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #244]
	bl 0x020090d4
	movs r4, #0
	add r7, sp, #56
	mov r8, r4
	mov r10, r7
	mov r9, r4
.L_02000dd8_2:
	mov r1, r8
	lsls r5, r1, #12
	adds r0, r5, #0
	bl 0x0200909c
	mov r2, r10
	mov r3, r9
	str r0, [r2]
	str r3, [r2, #4]
	adds r0, r5, #0
	bl 0x02009094
	mov r4, r10
	ldr r2, [r4]
	str r0, [r4, #8]
	adds r3, r2, #0
	cmp r2, #0
	bge .L_02000dd8_1
	adds r3, r2, #3
.L_02000dd8_1:
	lsrs r5, r0, #31
	adds r5, r0, r5
	asrs r3, r3, #2
	asrs r5, r5, #1
	subs r3, r2, r3
	subs r5, r0, r5
	str r3, [r7]
	str r5, [r7, #8]
	ldr r4, [r7, #4]
	ldr r1, [r6, #12]
	ldr r2, [r6, #16]
	ldr r0, [r6, #8]
	str r4, [sp, #0]
	mov r4, r9
	str r5, [sp, #4]
	str r4, [sp, #8]
	str r4, [sp, #12]
	bl 0x0200813c
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #16
	bls .L_02000dd8_2
	movs r3, #160
	lsls r3, r3, #11
	str r3, [r6, #40]
	movs r2, #196
	mov r0, r11
	movs r1, #139
	bl 0x02009154
	mov r0, r11
	bl 0x02009174
	movs r1, #128
	adds r0, r6, #0
	lsls r1, r1, #14
	bl 0x02008db8
	mov r3, r9
	str r3, [r6, #108]
	ldr r2, [r6, #80]
	movs r3, #128
	lsls r3, r3, #5
	strh r3, [r2, #30]
	add r4, sp, #16
	movs r3, #214
	strh r3, [r4, #24]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r4, #8]
	ldr r3, [pc, #84]
	str r3, [r4, #12]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r4, #16]
	ldr r3, [pc, #80]
	str r3, [r4, #20]
	mov r3, r9
	ldr r2, [r6, #16]
	ldr r1, [r6, #12]
	ldr r0, [r6, #8]
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r3, #224
	lsls r3, r3, #13
	str r3, [sp, #8]
	movs r3, #0
	str r4, [sp, #12]
	bl 0x0200813c
	movs r0, #154
	bl 0x0200922c
	mov r0, r11
	movs r1, #3
	bl 0x02009184
	bl 0x020090dc
	sub sp, #-68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xffffff00
	.4byte 0x02008da9
	.4byte 0x0000cccc
	.4byte 0x0000e666
	.4byte 0x00013333
	.global Func_02000f8c
	.thumb_func
Func_02000f8c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	mov r8, r1
	bl 0x020090e4
	mov r1, r8
	adds r5, r0, #0
	adds r0, r7, #0
	bl 0x020090f4
	movs r6, #0
	adds r5, #216
.L_02000f8c_1:
	ldrh r3, [r5]
	adds r5, #2
	cmp r3, r8
	bne .L_02000f8c_0
	adds r0, r7, #0
	adds r1, r6, #0
	bl 0x020090fc
.L_02000f8c_0:
	adds r6, #1
	cmp r6, #14
	ble .L_02000f8c_1
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02000fc8
	.thumb_func
Func_02000fc8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #65
	movs r0, #2
	mov r10, r3
	sub sp, #4
	bl 0x020090e4
	movs r3, #0
	adds r7, r0, #0
	mov r8, r3
.L_02000fc8_5:
	movs r3, #1
	add r8, r3
	movs r3, #250
	lsls r3, r3, #2
	cmp r8, r3
	ble .L_02000fc8_0
	adds r2, r7, #0
	adds r2, #244
	movs r3, #0
	strh r3, [r2]
.L_02000fc8_0:
	movs r0, #2
	movs r1, #65
	bl 0x020090f4
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_02000fc8_1
	adds r5, r7, #0
	movs r6, #0
	adds r5, #216
.L_02000fc8_3:
	ldrh r0, [r5]
	bl 0x020090ec
	ldrb r3, [r0, #2]
	adds r5, #2
	cmp r3, #1
	beq .L_02000fc8_2
	adds r6, #1
	cmp r6, #14
	ble .L_02000fc8_3
	adds r5, r7, #0
	ldr r2, [pc, #40]
	movs r6, #0
	adds r5, #216
.L_02000fc8_6:
	ldrh r0, [r5]
	str r2, [sp, #0]
	bl 0x020090ec
	ldrh r3, [r0, #2]
	ldr r2, [sp, #0]
	ands r3, r2
	cmp r3, #0
	bne .L_02000fc8_4
	ldrb r3, [r0, #12]
	cmp r3, #1
	bne .L_02000fc8_4
.L_02000fc8_2:
	movs r0, #2
	adds r1, r6, #0
	bl 0x0200911c
	b .L_02000fc8_5
	.2byte 0x0000
	.4byte 0x000008ff
.L_02000fc8_4:
	adds r6, #1
	adds r5, #2
	cmp r6, #14
	ble .L_02000fc8_6
	b .L_02000fc8_5
.L_02000fc8_1:
	adds r5, r7, #0
	movs r6, #0
	adds r5, #216
.L_02000fc8_8:
	ldrh r3, [r5]
	adds r5, #2
	cmp r3, r10
	bne .L_02000fc8_7
	movs r0, #2
	adds r1, r6, #0
	bl 0x020090fc
.L_02000fc8_7:
	adds r6, #1
	cmp r6, #14
	ble .L_02000fc8_8
	sub sp, #-4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.include "games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/IMPORT.INC"
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
	.4byte 0x02009234
	.4byte 0x0200926c
	.4byte 0x020092a4
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00019999
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x000000c0
	.4byte 0x400000c5
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000060
	.4byte 0xc00001b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000000c8
	.4byte 0x400000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000019
	.4byte 0x0010101c
	.4byte 0x00204002
	.4byte 0x000001ff
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0000c000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0000c000
	.4byte 0xffff00d4
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00d4
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00d7
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00028000
	.4byte 0xffff00d7
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01028000
	.4byte 0xffff00d4
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x086500f8
	.4byte 0x00000007
	.4byte 0x00bf0000
	.4byte 0x00000000
	.4byte 0x00c20000
	.4byte 0x0000c000
	.4byte 0x086600f8
	.4byte 0x020092e8
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00a80000
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
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000602
	.4byte 0xffff0005
	.4byte 0x0200843d
	.4byte 0x00008602
	.4byte 0xffff0006
	.4byte 0x0200843d
	.4byte 0x00008c15
	.4byte 0x08650008
	.4byte 0x0200867d
	.4byte 0x00008c15
	.4byte 0x08600008
	.4byte 0x02008d05
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte 0x02008349
	.4byte 0x00008c15
	.4byte 0x0863000a
	.4byte 0x020083d1
	.4byte 0x00004e15
	.4byte 0x08660010
	.4byte 0x020084d5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
