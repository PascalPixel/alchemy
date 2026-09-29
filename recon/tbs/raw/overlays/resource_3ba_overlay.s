.syntax unified
	.thumb
	.section .text.x02008540,"ax",%progbits
	.p2align 2
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #700]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r5, [r3, #0]
	adds	r0, r5, #0
	sub	sp, #12
	bl 0x0200bcb8
	str	r0, [sp, #8]
	movs	r0, #12
	bl 0x0200bcb8
	adds	r7, r0, #0
	ldr	r0, [pc, #676]
	bl 0x0200bc60
	bl 0x0200bca0
	movs	r1, #8
	adds	r0, r5, #0
	bl 0x0200bd00
	movs	r0, #6
	bl 0x0200bc98
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #48]
	ldr	r3, [pc, #648]
	movs	r0, #239
	str	r3, [r7, #52]
	mov	r8, r3
	bl 0x0200bdf8
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200bba8
	ldr	r1, [r7, #8]
	ldr	r2, [pc, #632]
	ldr	r3, [r7, #16]
	adds	r1, r1, r2
	adds	r0, r7, #0
	movs	r2, #0
	bl 0x0200bbd8
	movs	r0, #6
	bl 0x0200bc98
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200bd00
	ldr	r1, [pc, #608]
	movs	r0, #27
	bl 0x0200bb40
	movs	r3, #240
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldr	r0, [r0, #0]
	adds	r1, r7, #0
	bl 0x0200bbc0
	adds	r0, r5, #0
	ldr	r1, [pc, #588]
	mov	r2, r8
	bl 0x0200bcc8
	ldr	r2, [sp, #8]
	ldr	r3, [pc, #580]
	ldr	r1, [r2, #8]
	adds	r0, r2, #0
	adds	r1, r1, r3
	ldr	r3, [r2, #16]
	movs	r2, #0
	bl 0x0200bbd8
	adds	r0, r5, #0
	bl 0x0200bcf0
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200bd00
	adds	r0, r7, #0
	bl 0x0200bbe0
	movs	r1, #1
	adds	r0, r7, #0
	bl 0x0200bba8
	movs	r0, #144
	lsls	r0, r0, #1
	bl 0x0200bdf8
	movs	r0, #213
	bl 0x0200bdf8
	movs	r0, #15
	bl 0x0200bc98
	bl 0x0200bca8
	movs	r5, #7
	movs	r0, #37
	movs	r1, #7
	movs	r2, #1
	movs	r3, #4
	movs	r6, #34
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf8
	movs	r3, #37
	str	r3, [sp, #0]
	movs	r0, #36
	movs	r1, #7
	movs	r2, #1
	movs	r3, #4
	str	r5, [sp, #4]
	bl 0x0200bbf8
	ldr	r0, [pc, #480]
	bl 0x0200bc58
	mov	sl, r0
	cmp	r0, #0
	beq.n	.L_020006ea
	bl 0x0200bca0
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x0200bd70
	movs	r0, #138
	movs	r1, #1
	movs	r2, #200
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	bl 0x0200bd78
	bl 0x0200bd80
	movs	r5, #38
	movs	r1, #29
	movs	r2, #1
	movs	r3, #3
	movs	r0, #96
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #3
	bl 0x0200bc98
	movs	r1, #29
	movs	r2, #1
	movs	r3, #3
	movs	r0, #97
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #3
	bl 0x0200bc98
	movs	r1, #29
	movs	r2, #1
	movs	r3, #3
	movs	r0, #98
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #3
	bl 0x0200bc98
	movs	r1, #29
	movs	r2, #1
	movs	r3, #3
	movs	r0, #99
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #3
	bl 0x0200bc98
	movs	r0, #100
	movs	r1, #29
	movs	r2, #1
	movs	r3, #3
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #15
	bl 0x0200bc98
	bl 0x0200bca8
	b.n	.L_020007f8
.L_020006ea:
	ldr	r0, [pc, #312]
	bl 0x0200bc60
	bl 0x0200bca0
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x0200bd70
	movs	r0, #150
	movs	r1, #1
	movs	r2, #200
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #18
	bl 0x0200bd78
	bl 0x0200bd80
	movs	r0, #13
	bl 0x0200bcb8
	adds	r7, r0, #0
	adds	r3, r7, #0
	adds	r3, #85
	mov	r2, sl
	strb	r2, [r3, #0]
	ldr	r2, [pc, #256]
	ldr	r3, [pc, #256]
	str	r2, [r7, #48]
	mov	r9, r2
	movs	r2, #128
	str	r3, [r7, #52]
	ldr	r1, [r7, #8]
	lsls	r2, r2, #12
	mov	r8, r3
	ldr	r3, [r7, #16]
	bl 0x0200bbd8
	adds	r0, r7, #0
	movs	r1, #3
	bl 0x0200bba8
	movs	r5, #38
	movs	r1, #29
	movs	r2, #1
	movs	r3, #3
	movs	r0, #96
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #3
	bl 0x0200bc98
	movs	r1, #29
	movs	r2, #1
	movs	r3, #3
	movs	r0, #97
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #3
	bl 0x0200bc98
	movs	r1, #29
	movs	r2, #1
	movs	r3, #3
	movs	r0, #98
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #3
	bl 0x0200bc98
	movs	r1, #29
	movs	r2, #1
	movs	r3, #3
	movs	r0, #99
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #3
	bl 0x0200bc98
	movs	r1, #29
.L_020007a2:
	movs	r2, #1
	movs	r3, #3
	movs	r0, #100
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #14
	bl 0x0200bcb8
	adds	r7, r0, #0
	adds	r3, r7, #0
	adds	r3, #85
	mov	r2, sl
	strb	r2, [r3, #0]
	mov	r2, r9
	mov	r3, r8
	str	r2, [r7, #48]
	movs	r2, #128
	ldr	r1, [r7, #8]
	lsls	r2, r2, #14
	str	r3, [r7, #52]
	ldr	r3, [r7, #16]
	bl 0x0200bbd8
	adds	r0, r7, #0
	bl 0x0200bbe0
	movs	r0, #15
	bl 0x0200bc98
	bl 0x0200bca8
	movs	r3, #41
	movs	r2, #12
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #43
	movs	r1, #12
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bbf8
.L_020007f8:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0x00000302
	.4byte 0x00003333
	.4byte 0xffd00000
	.4byte 0x00000ccc
	.4byte 0x00004ccc
	.4byte 0xffe80000
	.4byte 0x00000301
	.4byte 0x0000cccc
	.2byte 0x6666
	.2byte 0x0000
	.section .text.x02008840,"ax",%progbits
	.p2align 2
	.global KorosseoKawa_RunStageStart
	.thumb_func
KorosseoKawa_RunStageStart:
	push {r5, r6, lr}
	bl 0x0200af90
	bl 0x0200bca0
	movs r1, #127
	movs r0, #120
	bl 0x0200b0ac
	adds r6, r0, #0
	bl 0x0200afa0
	movs r5, #9
.L_02000840_0:
	movs r0, #8
	subs r5, #1
	bl 0x0200bcd0
	cmp r5, #0
	bge .L_02000840_0
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcc8
	movs r1, #165
	movs r0, #8
	lsls r1, r1, #3
	movs r2, #192
	bl 0x0200bce0
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcc8
	movs r1, #161
	movs r2, #192
	movs r0, #0
	lsls r1, r1, #3
	bl 0x0200bce8
	movs r0, #8
	movs r1, #1
	bl 0x0200bd00
	movs r2, #0
	movs r1, #8
	movs r0, #0
	bl 0x0200bd28
	movs r0, #10
	bl 0x0200bc98
	movs r0, #8
	movs r1, #3
	bl 0x0200bd00
	movs r1, #3
	movs r0, #0
	bl 0x0200bd08
	movs r0, #20
	bl 0x0200bc98
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200bcc8
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200bcc8
	movs r1, #162
	movs r0, #0
	lsls r1, r1, #3
	movs r2, #192
	bl 0x0200bce0
	movs r1, #164
	movs r2, #192
	movs r0, #8
	lsls r1, r1, #3
	bl 0x0200bce8
	movs r0, #0
	movs r1, #16
	bl 0x0200bd00
	movs r1, #9
	movs r0, #8
	bl 0x0200bd00
	movs r0, #10
	bl 0x0200bc98
	movs r1, #0
	subs r1, r1, r6
	adds r1, #1
	movs r0, #72
	bl 0x0200bd90
	ldr r3, [pc, #40]
	ldr r2, [pc, #40]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	ldr r5, [pc, #36]
	movs r1, #4
	adds r0, r5, #0
	bl 0x0200bd98
	adds r0, r5, #0
	movs r1, #5
	bl 0x0200bda0
	movs r0, #141
	lsls r0, r0, #1
	bl 0x0200bc60
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x0000008f
	.section .text.x02008a3c,"ax",%progbits
	.p2align 2
	.global Func_02000a3c
	.thumb_func
Func_02000a3c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, [pc, #832]
	movs r1, #224
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	movs r0, #162
	str r2, [r3]
	lsls r0, r0, #1
	sub sp, #12
	bl 0x0200bc60
	movs r0, #9
	bl 0x0200bcb8
	adds r5, r0, #0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl 0x0200bbe8
	ldr r3, [r5, #12]
	cmp r3, #0
	bne .L_02000a3c_0
	cmp r0, #0
	bne .L_02000a3c_0
	adds r2, r5, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	adds r3, r5, #0
	adds r3, #85
	strb r0, [r3]
	ldr r2, [r5, #8]
	ldr r3, [r5, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl 0x0200bbf8
.L_02000a3c_0:
	movs r0, #196
	lsls r0, r0, #2
	bl 0x0200bc68
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02000a3c_1
	movs r6, #25
.L_02000a3c_1:
	movs r0, #10
	bl 0x0200bcb8
	movs r2, #128
	lsls r2, r2, #12
	mov r9, r2
	lsls r3, r6, #20
	adds r5, r0, #0
	add r3, r9
	movs r1, #0
	str r3, [r5, #8]
	mov r8, r1
	adds r3, r5, #0
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	movs r7, #2
	subs r3, #50
	strb r7, [r3]
	movs r3, #12
	str r3, [sp, #4]
	movs r2, #1
	movs r0, #14
	movs r1, #13
	mov r10, r3
	movs r3, #1
	str r6, [sp, #0]
	bl 0x0200bbf8
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #668]
	bl 0x0200bb10
	movs r0, #15
	bl 0x0200bcb8
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #34
	movs r3, #1
	strb r3, [r2]
	ldr r0, [pc, #648]
	bl 0x0200bc58
	cmp r0, #0
	beq .L_02000a3c_2
	adds r0, r5, #0
	movs r1, #4
	bl 0x0200bba8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bc10
	adds r3, r5, #0
	adds r3, #89
	mov r1, r8
	strb r1, [r3]
	movs r2, #3
	subs r3, #54
	strb r2, [r3]
	movs r3, #47
	mov r2, r10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #24
	movs r2, #1
	movs r3, #1
	bl 0x0200bbf8
.L_02000a3c_2:
	movs r0, #17
	bl 0x0200bcb8
	adds r5, r0, #0
	ldr r3, [r5, #16]
	asrs r2, r3, #20
	adds r3, r5, #0
	adds r3, #85
	mov r1, r8
	strb r1, [r3]
	subs r3, #50
	strb r7, [r3]
	movs r3, #64
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #24
	movs r2, #3
	movs r3, #1
	movs r0, #64
	bl 0x0200bbf8
	movs r0, #18
	bl 0x0200bcb8
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r2, r3, #20
	adds r3, r5, #0
	adds r3, #85
	mov r1, r8
	strb r1, [r3]
	subs r3, #50
	strb r7, [r3]
	movs r3, #9
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #63
	movs r1, #25
	movs r2, #1
	movs r3, #3
	bl 0x0200bbf8
	ldr r0, [pc, #508]
	bl 0x0200bc58
	cmp r0, #0
	beq .L_02000a3c_3
	movs r6, #34
	movs r5, #7
	movs r0, #37
	movs r1, #7
	movs r2, #1
	movs r3, #4
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200bbf8
	movs r3, #37
	str r3, [sp, #0]
	movs r0, #36
	movs r1, #7
	movs r2, #1
	movs r3, #4
	str r5, [sp, #4]
	bl 0x0200bbf8
	movs r3, #38
	str r3, [sp, #4]
	movs r0, #100
	movs r1, #29
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	bl 0x0200bbf0
.L_02000a3c_3:
	movs r0, #13
	bl 0x0200bcb8
	adds r5, r0, #0
	ldr r0, [pc, #440]
	bl 0x0200bc58
	cmp r0, #0
	beq .L_02000a3c_4
	movs r3, #41
	mov r2, r10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #43
	movs r1, #12
	movs r2, #1
	movs r3, #1
	bl 0x0200bbf8
	adds r3, r5, #0
	adds r3, #85
	mov r1, r8
	strb r1, [r3]
	ldr r3, [pc, #404]
	str r3, [r5, #52]
	ldr r3, [pc, #404]
	mov r2, r9
	str r3, [r5, #48]
	str r2, [r5, #12]
	adds r0, r5, #0
	movs r1, #3
	bl 0x0200bba8
	b .L_02000a3c_5
.L_02000a3c_4:
	adds r0, r5, #0
	movs r1, #2
	bl 0x0200bba8
.L_02000a3c_5:
	movs r0, #14
	bl 0x0200bcb8
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	movs r1, #120
	movs r0, #24
	bl 0x0200aea0
	movs r1, #127
	movs r0, #25
	bl 0x0200aea0
	ldr r3, [pc, #356]
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #4
	bls .L_02000a3c_6
	b .L_02000a3c_7
.L_02000a3c_6:
	ldr r2, [pc, #340]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	ldrh r0, [r6, #34]
	lsls r0, r0, #8
	ldrh r0, [r2, #40]
	lsls r0, r0, #8
	ldrh r2, [r0, #42]
	lsls r0, r0, #8
	ldrh r0, [r3, #42]
	lsls r0, r0, #8
	ldrh r6, [r4, #42]
	lsls r0, r0, #8
	movs r2, #192
	lsls r2, r2, #16
	str r2, [sp, #0]
	movs r2, #24
	str r2, [sp, #4]
	movs r3, #163
	movs r2, #25
	str r2, [sp, #8]
	lsls r3, r3, #19
	movs r1, #8
	movs r2, #4
	movs r0, #0
	bl 0x0200b764
	movs r3, #19
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #2
	movs r0, #127
	bl 0x0200bc00
	movs r0, #19
	bl 0x0200bcc0
	movs r0, #20
	bl 0x0200bcc0
	movs r0, #21
	bl 0x0200bcc0
	movs r0, #22
	bl 0x0200bcc0
	movs r0, #23
	bl 0x0200bcc0
	ldr r0, [pc, #236]
	bl 0x0200bc58
	cmp r0, #0
	bne .L_02000a3c_8
	movs r0, #17
	bl 0x0200bdf8
	movs r0, #0
	bl 0x02009910
	bl 0x02009898
	movs r0, #1
	bl 0x02008a10
	movs r0, #2
	bl 0x02008a10
	movs r0, #3
	bl 0x02008a10
	movs r0, #1
	bl 0x0200a738
.L_02000a3c_8:
	movs r0, #1
	movs r1, #0
	bl 0x0200bde8
	movs r0, #2
	movs r1, #0
	bl 0x0200bde8
	movs r0, #3
	movs r1, #0
	bl 0x0200bde8
	ldr r0, [pc, #164]
	bl 0x0200b84c
	b .L_02000a3c_7
	.2byte 0x21c8
	.2byte 0x0109
	.2byte 0x4827
	.2byte 0xf002
	.2byte 0xfefb
	.2byte 0x2018
	.2byte 0xf002
	.2byte 0xffd0
	.2byte 0x2019
	.2byte 0xf002
	.2byte 0xffcd
	.2byte 0x4821
	.2byte 0xf002
	.2byte 0xff96
	.2byte 0x2800
	.2byte 0xd121
	.2byte 0xf000
	.2byte 0xfdb2
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xfdeb
	.2byte 0x2000
	.2byte 0xf001
	.2byte 0xfcfc
	.2byte 0xe018
	.2byte 0x481a
	.2byte 0xf002
	.2byte 0xff88
	.2byte 0x2800
	.2byte 0xd113
	.2byte 0x2013
	.2byte 0xf000
	.2byte 0xf833
	.2byte 0xf000
	.2byte 0xffe5
	.2byte 0xe00d
	.2byte 0x2001
	.2byte 0xf7ff
	.2byte 0xfe0b
	.2byte 0x2004
	.2byte 0xf003
	.2byte 0xf812
	.2byte 0xe006
	.2byte 0x2001
	.2byte 0x4240
	.2byte 0xf7ff
	.2byte 0xfe03
	.2byte 0x2005
	.2byte 0xf003
	.2byte 0xf80a
.L_02000a3c_7:
	movs r0, #0
	sub sp, #-12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0200804d
	.4byte 0x00000303
	.4byte 0x00000302
	.4byte 0x00000301
	.4byte 0x00006666
	.4byte 0x0000cccc
	.4byte 0x02000240
	.4byte 0x02008c5c
	.4byte 0x00000109
	.4byte 0x000000e4
	.2byte 0x99e1
	.2byte 0x0200
	.section .text.x0200a124,"ax",%progbits
	.p2align 2
	.global Func_02002124
	.thumb_func
Func_02002124:
	.global Korosseo_LoadPortrait
	.thumb_func
Korosseo_LoadPortrait:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #229
	lsls r0, r0, #5
	bl 0x0200bb50
	ldr r7, [pc, #104]
	movs r2, #0
	ldrsh r3, [r7, r2]
	movs r2, #1
	negs r2, r2
	adds r6, r0, #0
	cmp r3, r2
	bne .L_02002124_0
	bl 0x0200bb80
	strh r0, [r7]
.L_02002124_0:
	ldr r3, [pc, #88]
	ldrb r3, [r3, r5]
	mov r8, r3
	cmp r5, #8
	bne .L_02002124_1
	movs r5, #4
.L_02002124_1:
	ldr r0, [pc, #80]
	bl 0x0200bb98
	adds r1, r6, #0
	bl 0x0200bb60
	mov r2, r8
	adds r0, r6, r2
	ldr r3, [pc, #68]
	ldr r1, [pc, #68]
	ldr r2, [pc, #72]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	lsls r2, r5, #10
	adds r2, r2, r6
	movs r1, #128
	adds r2, #160
	lsls r1, r1, #3
	movs r3, #0
	ldrsh r0, [r7, r3]
	bl 0x0200bb78
	movs r2, #128
	ldr r1, [pc, #36]
	lsls r2, r2, #24
.L_02002124_2:
	ldr r3, [r1, #8]
	ands r3, r2
	cmp r3, #0
	bne .L_02002124_2
	adds r0, r6, #0
	bl 0x0200bb58
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200c57c
	.4byte 0x0200be44
	.4byte 0x000000e7
	.4byte 0x040000d4
	.4byte 0x050003e0
	.4byte 0x84000008
	.section .text.x0200a6a8,"ax",%progbits
	.global Func_020026a8
	.thumb_func
Func_020026a8:
	.global SceneData_SelectBlockAndResetCounters
	.thumb_func
SceneData_SelectBlockAndResetCounters:
	push {r5, r6, lr}
	ldr r3, [pc, #48]
	adds r5, r0, #0
	adds r6, r1, #0
	ldr r2, [pc, #44]
	strh r5, [r3]
	movs r1, #200
	lsls r3, r6, #4
	lsls r1, r1, #4
	strh r3, [r2]
	ldr r0, [pc, #36]
	bl 0x0200bb10
	ldr r1, [pc, #36]
	cmp r5, #2
	bne .L_020026a8_0
	ldr r1, [pc, #32]
.L_020026a8_0:
	cmp r5, #4
	bne .L_020026a8_1
	ldr r1, [pc, #32]
.L_020026a8_1:
	cmp r5, #3
	bne .L_020026a8_2
	cmp r6, #0
	beq .L_020026a8_3
	ldr r1, [pc, #24]
	b .L_020026a8_2
	.4byte 0x0200c790
	.4byte 0x0200c764
	.4byte 0x0200a1b9
	.4byte 0x0200c57e
	.4byte 0x0200be4e
	.4byte 0x0200c5aa
	.4byte 0x0200be76
.L_020026a8_3:
	ldr r1, [pc, #28]
.L_020026a8_2:
	ldr r2, [pc, #24]
	ldr r3, [pc, #28]
	strh r2, [r3]
	ldr r3, [pc, #28]
	str r1, [r3]
	ldr r3, [pc, #28]
	strh r2, [r3]
	ldr r3, [pc, #28]
	strh r2, [r3]
	ldr r2, [pc, #28]
	movs r3, #0
	str r3, [r2]
	b .L_020026a8_4
	.4byte 0x00000000
	.4byte 0x0200c628
	.4byte 0x0200c79c
	.4byte 0x0200c7a0
	.4byte 0x0200c7f8
	.4byte 0x0200c76c
	.4byte 0x0200c770
.L_020026a8_4:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.section .text.x0200ad28,"ax",%progbits
	.global Func_02002d28
	.thumb_func
Func_02002d28:
	.global SceneState_StoreParamsAndInitTable
	.thumb_func
SceneState_StoreParamsAndInitTable:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r6, r0, #0
	mov r8, r1
	adds r5, r2, #0
	bl 0x0200abcc
	ldr r3, [pc, #44]
	strh r6, [r3]
	ldr r3, [pc, #44]
	mov r2, r8
	strh r2, [r3]
	ldr r3, [pc, #28]
	ldr r2, [pc, #40]
	ands r5, r3
	strh r5, [r2]
	ldr r3, [pc, #40]
	ldr r2, [pc, #20]
	strh r2, [r3]
	ldr r3, [pc, #36]
	movs r1, #200
	strh r2, [r3]
	lsls r1, r1, #4
	ldr r0, [pc, #32]
	bl 0x0200bb10
	b .L_02002d28_0
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x0200c7f4
	.4byte 0x0200c780
	.4byte 0x0200c758
	.4byte 0x0200c774
	.4byte 0x0200c78c
	.4byte 0x0200abed
.L_02002d28_0:
	pop {r3}
	mov r8, r3
	pop {r5, r6}
.L_02002d86:
	pop {r0}
	bx r0
	.2byte 0x0000
	.global SceneState_InitTableWordsAndLoad3200
	.thumb_func
SceneState_InitTableWordsAndLoad3200:
	push {lr}
	ldr r3, [pc, #52]
	strh r0, [r3]
	ldr r3, [pc, #52]
	strh r1, [r3]
	ldr r3, [pc, #52]
	ldr r1, [pc, #52]
	ldrh r3, [r3]
	strh r3, [r1]
	ldr r3, [pc, #52]
	ldr r1, [pc, #52]
	ldrh r3, [r3]
	strh r3, [r1]
	ldr r3, [pc, #52]
	strh r2, [r3]
	ldr r2, [pc, #52]
	ldr r3, [pc, #16]
	movs r1, #200
	strh r3, [r2]
	lsls r1, r1, #4
	ldr r0, [pc, #44]
	bl 0x0200bb10
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200c760
	.4byte 0x0200c800
	.4byte 0x0200c7f4
	.4byte 0x0200c7a4
	.4byte 0x0200c780
	.4byte 0x0200c7bc
	.4byte 0x0200c78c
	.4byte 0x0200c750
	.4byte 0x0200abed
	.section .text.x0200af94,"ax",%progbits
	.p2align 2
	.global Func_02002f94
	.thumb_func
Func_02002f94:
	ldr r2, [pc, #4]
	movs r3, #9
	strh r3, [r2]
	bx lr
	.4byte 0x02001000
	.global Func_02002fa0
	.thumb_func
Func_02002fa0:
	push {r5, lr}
	ldr r5, [pc, #28]
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #9
	beq .L_02002fa0_0
.L_02002fa0_1:
	movs r0, #1
	bl 0x0200bb08
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #9
	bne .L_02002fa0_1
.L_02002fa0_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x02001000
	.section .text.x0200b3a0,"ax",%progbits
	.p2align 2
	.global Scene_RunScene3baSequenceA
	.thumb_func
Scene_RunScene3baSequenceA:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #916]
	ldr	r3, [r3, #0]
	adds	r1, r3, #0
	sub	sp, #20
	mov	r8, r1
.L_020033b8:
	str	r3, [sp, #12]
	str	r3, [sp, #16]
	mov	r7, r8
	adds	r7, #216
	movs	r1, #0
	ldrsh	r3, [r7, r1]
	ldr	r2, [pc, #896]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	lsrs	r3, r3, #5
	str	r3, [sp, #8]
	mov	r3, r8
	adds	r3, #230
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	subs	r3, #10
	mov	fp, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_020033ee
	mov	r6, r8
	adds	r6, #218
	movs	r3, #2
	strh	r3, [r6, #0]
	b.n	.L_0200345c
.L_020033ee:
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200bc58
	cmp	r0, #0
	beq.n	.L_0200340e
	mov	r6, r8
	adds	r6, #218
	movs	r1, #0
	ldrsh	r3, [r6, r1]
	ldrh	r2, [r6, #0]
	cmp	r3, #0
	ble.n	.L_0200345c
	subs	r3, r2, #1
	strh	r3, [r6, #0]
	b.n	.L_0200345c
.L_0200340e:
	mov	r6, r8
	adds	r6, #218
	movs	r1, #0
	ldrsh	r3, [r6, r1]
	ldrh	r2, [r6, #0]
	cmp	r3, #1
	bgt.n	.L_0200345c
	adds	r3, r2, #1
	movs	r2, #128
	strh	r3, [r6, #0]
	lsls	r2, r2, #9
	lsls	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_0200345c
	ldr	r3, [pc, #800]
	ldr	r0, [pc, #800]
	ldr	r1, [pc, #804]
	ldr	r2, [pc, #804]
	stmia	r3!, {r0, r1, r2}
.L_02003434:
	subs	r3, #12
	movs	r0, #128
	lsls	r0, r0, #2
.L_0200343a:
	bl 0x0200bb50
	adds	r5, r0, #0
	adds	r1, r5, #0
	ldr	r0, [pc, #792]
	bl 0x0200bb60
	movs	r1, #128
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	lsls	r1, r1, #2
	adds	r2, r5, #0
	bl 0x0200bb78
	adds	r0, r5, #0
	bl 0x0200bb58
.L_0200345c:
	movs	r1, #0
	ldrsh	r2, [r6, r1]
	cmp	r2, #0
	bne.n	.L_02003472
	ldr	r3, [sp, #12]
	adds	r3, #216
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200bb70
	b.n	.L_02003732
.L_02003472:
	lsls	r3, r2, #1
.L_02003474:
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r7, r3, #0
	subs	r7, #8
	movs	r3, #255
	ands	r7, r3
	mov	r3, fp
	lsls	r3, r3, #4
	str	r3, [sp, #4]
	movs	r2, #128
	ldr	r1, [sp, #4]
	lsls	r2, r2, #8
	movs	r3, #104
	mov	r9, r2
	ldr	r2, [sp, #12]
	subs	r4, r3, r1
	movs	r3, #0
	stmia	r2!, {r3}
	lsls	r3, r4, #16
	adds	r1, r2, #0
	str	r1, [sp, #12]
	orrs	r3, r7
	mov	r1, r9
	orrs	r3, r1
	stmia	r2!, {r3}
	ldr	r3, [sp, #8]
	movs	r5, #228
	lsls	r5, r5, #8
	adds	r1, r2, #0
	orrs	r3, r5
	str	r1, [sp, #12]
	stmia	r2!, {r3}
	adds	r1, r2, #0
	str	r1, [sp, #12]
	mov	r0, r8
	movs	r2, #12
	movs	r1, #255
	movs	r6, #0
	add	r8, r2
	bl 0x0200bb90
	cmp	r6, fp
	bcs.n	.L_0200350c
	ldr	r3, [sp, #8]
	movs	r1, #128
	adds	r3, #2
	orrs	r3, r5
.L_020034d2:
	lsls	r1, r1, #23
	ldr	r5, [sp, #12]
	mov	r9, r1
	mov	sl, r3
.L_020034da:
	lsls	r2, r6, #4
	movs	r3, #96
	subs	r4, r3, r2
	movs	r3, #0
	str	r3, [r5, #0]
	lsls	r3, r4, #16
	mov	r2, r9
	orrs	r3, r7
	orrs	r3, r2
	str	r3, [r5, #4]
	mov	r3, sl
	str	r3, [r5, #8]
	ldr	r1, [sp, #12]
	adds	r1, #12
	str	r1, [sp, #12]
	mov	r0, r8
	movs	r2, #12
	movs	r1, #255
	adds	r6, #1
	add	r8, r2
	adds	r5, #12
	bl 0x0200bb90
	cmp	r6, fp
	bcc.n	.L_020034da
.L_0200350c:
	ldr	r2, [sp, #12]
	movs	r6, #0
	movs	r3, #128
	stmia	r2!, {r6}
	lsls	r3, r3, #8
	mov	r9, r3
	movs	r3, #224
	adds	r1, r2, #0
	lsls	r3, r3, #15
	str	r1, [sp, #12]
	orrs	r3, r7
	mov	r1, r9
	orrs	r3, r1
	stmia	r2!, {r3}
	ldr	r5, [sp, #8]
	adds	r1, r2, #0
	movs	r2, #228
	lsls	r2, r2, #8
	adds	r5, #6
	orrs	r5, r2
	stmia	r1!, {r5}
	mov	r0, r8
	adds	r3, r1, #0
	mov	sl, r2
	movs	r1, #255
	movs	r2, #12
.L_02003540:
	add	r8, r2
	str	r3, [sp, #12]
	bl 0x0200bb90
	ldr	r1, [sp, #12]
	stmia	r1!, {r6}
.L_0200354c:
	adds	r3, r1, #0
	str	r3, [sp, #12]
	movs	r3, #240
	lsls	r3, r3, #15
	mov	r2, r9
	orrs	r3, r7
	orrs	r3, r2
	movs	r2, #128
	lsls	r2, r2, #21
	orrs	r3, r2
	stmia	r1!, {r3}
	adds	r2, r1, #0
	str	r2, [sp, #12]
	stmia	r1!, {r5}
	adds	r3, r1, #0
	movs	r1, #12
	mov	r0, r8
	add	r8, r1
	movs	r1, #255
	str	r3, [sp, #12]
	bl 0x0200bb90
	cmp	r6, fp
	bcs.n	.L_020035ce
	ldr	r4, [sp, #8]
	movs	r2, #128
	movs	r1, #128
	mov	r3, sl
	adds	r4, #2
	lsls	r2, r2, #23
	lsls	r1, r1, #16
	ldr	r5, [sp, #12]
	mov	r9, r2
	orrs	r4, r3
	mov	sl, r1
.L_02003592:
	movs	r3, #0
	str	r3, [r5, #0]
	mov	r2, sl
	adds	r3, r7, #0
	orrs	r3, r2
	mov	r1, r9
	movs	r2, #128
	orrs	r3, r1
	lsls	r2, r2, #21
	orrs	r3, r2
	str	r3, [r5, #4]
	str	r4, [r5, #8]
	ldr	r2, [sp, #12]
	mov	r0, r8
	adds	r2, #12
	movs	r3, #12
	movs	r1, #255
	str	r4, [sp, #0]
	str	r2, [sp, #12]
	add	r8, r3
	bl 0x0200bb90
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r6, #1
	adds	r5, #12
	add	sl, r1
	ldr	r4, [sp, #0]
	cmp	r6, fp
	bcc.n	.L_02003592
.L_020035ce:
	movs	r2, #128
	ldr	r4, [sp, #4]
	lsls	r2, r2, #8
	mov	r9, r2
	ldr	r2, [sp, #12]
	movs	r3, #0
	adds	r4, #128
	stmia	r2!, {r3}
	mov	fp, r3
	lsls	r3, r4, #16
	orrs	r7, r3
	mov	r3, r9
	orrs	r7, r3
	movs	r3, #128
	lsls	r3, r3, #21
	adds	r1, r2, #0
	orrs	r7, r3
	str	r1, [sp, #12]
	stmia	r2!, {r7}
	ldr	r3, [sp, #8]
	adds	r1, r2, #0
	movs	r2, #228
	lsls	r2, r2, #8
	orrs	r3, r2
	mov	sl, r2
	adds	r2, r1, #0
	stmia	r2!, {r3}
	adds	r1, r2, #0
	movs	r3, #12
	str	r1, [sp, #12]
	mov	r0, r8
	movs	r1, #255
	add	r8, r3
	bl 0x0200bb90
	ldr	r3, [pc, #328]
	ldr	r3, [r3, #0]
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #4
	bhi.n	.L_02003622
	b.n	.L_02003732
.L_02003622:
	ldr	r3, [sp, #16]
	movs	r1, #128
	adds	r3, #224
	lsls	r1, r1, #23
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	mov	r9, r1
	bl 0x0200bdc8
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_020036b0
	ldr	r3, [sp, #16]
	adds	r3, #232
	ldr	r3, [r3, #0]
	ldr	r0, [r6, #8]
	movs	r5, #224
	lsls	r5, r5, #12
	subs	r0, r0, r3
	adds	r1, r5, #0
	bl 0x0200bb00
	ldr	r3, [sp, #16]
	adds	r3, #236
	ldr	r3, [r3, #0]
	adds	r4, r0, #0
	ldr	r0, [r6, #16]
	adds	r4, #112
	adds	r1, r5, #0
	subs	r0, r0, r3
	str	r4, [sp, #0]
	bl 0x0200bb00
	ldr	r3, [sp, #16]
	adds	r3, #218
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldr	r1, [sp, #12]
	subs	r7, r0, #4
	movs	r3, #255
	ands	r7, r3
	mov	r3, fp
	stmia	r1!, {r3}
.L_02003680:
	ldr	r4, [sp, #0]
	adds	r2, r1, #0
	lsls	r3, r4, #16
	str	r2, [sp, #12]
	orrs	r7, r3
	mov	r2, r9
	orrs	r7, r2
	stmia	r1!, {r7}
	adds	r3, r1, #0
	str	r3, [sp, #12]
	ldr	r3, [sp, #8]
	mov	r1, sl
	adds	r3, #12
	orrs	r3, r1
	ldr	r1, [sp, #12]
	stmia	r1!, {r3}
	adds	r2, r1, #0
	str	r2, [sp, #12]
	mov	r0, r8
	movs	r2, #12
	movs	r1, #255
	add	r8, r2
	bl 0x0200bb90
.L_020036b0:
	ldr	r3, [sp, #16]
	adds	r3, #222
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl 0x0200bdc8
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02003732
	ldr	r3, [sp, #16]
	adds	r3, #232
	ldr	r3, [r3, #0]
	ldr	r0, [r6, #8]
	movs	r5, #224
	lsls	r5, r5, #12
	subs	r0, r0, r3
	adds	r1, r5, #0
	bl 0x0200bb00
	ldr	r3, [sp, #16]
	adds	r3, #236
	ldr	r3, [r3, #0]
	adds	r4, r0, #0
	ldr	r0, [r6, #16]
	adds	r4, #112
	adds	r1, r5, #0
	subs	r0, r0, r3
	str	r4, [sp, #0]
	bl 0x0200bb00
	ldr	r3, [sp, #16]
	adds	r3, #218
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
.L_020036fa:
	adds	r0, r0, r3
	ldr	r1, [sp, #12]
	subs	r7, r0, #4
	movs	r3, #255
	ands	r7, r3
	mov	r3, fp
	stmia	r1!, {r3}
	ldr	r4, [sp, #0]
	adds	r2, r1, #0
	lsls	r3, r4, #16
	str	r2, [sp, #12]
	orrs	r7, r3
	mov	r2, r9
	orrs	r7, r2
	stmia	r1!, {r7}
	adds	r3, r1, #0
	str	r3, [sp, #12]
	ldr	r3, [sp, #8]
	mov	r1, sl
	adds	r3, #8
	ldr	r2, [sp, #12]
	orrs	r3, r1
	str	r3, [r2, #0]
	mov	r3, r8
	adds	r0, r3, #0
	movs	r1, #255
	bl 0x0200bb90
.L_02003732:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f3c
	.4byte 0x03001b10
	.4byte 0x040000d4
	.4byte 0x0200bef4
	.4byte 0x050003c0
	.4byte 0x80000010
	.4byte 0x0200bf14
	.2byte 0x1e40
	.2byte 0x0300
	.section .text.x0200b84c,"ax",%progbits
	.p2align 2
	.global Func_0200384c
	.thumb_func
Func_0200384c:
	push {r5, r6, lr}
	ldr r3, [pc, #60]
	ldr r6, [r3]
	ldr r5, [pc, #60]
	bl 0x0200bb98
	adds r1, r6, #0
	adds r1, #240
	bl 0x0200bb60
	ldr r0, [pc, #48]
	bl 0x0200bc58
	cmp r0, #0
	bne .L_0200384c_0
	movs r3, #1
	strh r3, [r5]
	strh r3, [r5, #2]
	adds r3, r6, #0
	adds r3, #224
	ldrh r3, [r3]
	strh r0, [r5, #8]
	strh r3, [r5, #4]
	strh r0, [r5, #6]
.L_0200384c_0:
	ldr r1, [pc, #24]
	ldr r0, [pc, #28]
	bl 0x0200bb10
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001f3c
	.4byte 0x02001000
	.4byte 0x00000109
	.4byte 0x00000c85
	.4byte 0x0200b1c1
@ The compiler library links here from its licensed container.
	.section .rodata.part1,"a",%progbits
	.4byte 0x20202000
	.4byte 0x40404060
	.4byte 0x10000080
	.4byte 0x00010200
	.4byte 0x00002000
	.4byte 0x10000001
	.4byte 0x00060100
	.4byte 0x00102000
	.4byte 0x7fff0006
	.4byte 0x2000001e
	.4byte 0x001e0000
	.4byte 0x001e7fff
	.2byte 0xffff
	.global KorosseoKawa_SpanB
KorosseoKawa_SpanB:
	.2byte 0x1000
	.4byte 0x00010080
	.4byte 0x00002000
	.4byte 0x10000001
	.4byte 0x00060100
	.4byte 0x00102000
	.4byte 0x7fff0006
	.4byte 0x1000003c
	.4byte 0x00060180
	.4byte 0x00067fff
	.4byte 0x00f01000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060170
	.4byte 0x00067fff
	.4byte 0x00e01000
	.4byte 0x7fff0006
	.4byte 0x20000006
	.4byte 0x001e0000
	.4byte 0x01601000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000600d0
	.4byte 0x00067fff
	.4byte 0x01501000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000600c0
	.4byte 0x00067fff
	.4byte 0x01401000
	.4byte 0x7fff0006
	.4byte 0xffff0006
	.4byte 0x377f10a0
	.4byte 0x121722bb
	.4byte 0x0ccc1172
	.4byte 0x7df07ef7
	.4byte 0x610b7ce7
	.4byte 0x50007c00
	.4byte 0x00147fff
	.4byte 0x2f1f0000
	.global KorosseoKawa_ImageData
KorosseoKawa_ImageData:
	.4byte 0x82fc0100
	.4byte 0x70462310
	.4byte 0x201abddc
	.4byte 0x648ad0cc
	.4byte 0xa8a89c76
	.4byte 0x81984754
	.4byte 0x6138edda
	.4byte 0x5f28474a
	.4byte 0xa01027d3
	.4byte 0xa0abb823
	.4byte 0xf0214faa
	.4byte 0x5557fc29
	.4byte 0x1c29f456
	.4byte 0xc0a8882e
	.4byte 0xf029560c
	.4byte 0x39f9b811
	.4byte 0xa3e78fa8
	.4byte 0xd88e8df8
	.4byte 0x60101ddd
	.4byte 0x2101d56c
	.4byte 0xa68033e0
	.4byte 0x1ca188ea
	.4byte 0xb037924e
	.4byte 0x8e40d46f
	.4byte 0x56a22701
	.4byte 0x9957005d
	.4byte 0x6e51113b
	.4byte 0x9f8022e5
	.4byte 0x3a3ae095
	.4byte 0x39edc9ab
	.4byte 0x9ddeeaec
	.4byte 0x378733aa
	.4byte 0xbb23a0e8
	.4byte 0x7b4bb0f7
	.4byte 0x0596cc80
	.4byte 0xc075eccf
	.4byte 0x3fd9b0ef
	.4byte 0xcc032ec8
	.4byte 0x8d5aec81
	.4byte 0xee419f03
	.4byte 0x7c20f81f
	.4byte 0x38be6e09
	.4byte 0xbe2a7c41
	.4byte 0x07316774
	.4byte 0xf7104b82
	.4byte 0xf17c90f8
	.4byte 0x00000001
	.global HexDigits
HexDigits:
	.4byte 0x33323130
	.4byte 0x37363534
	.4byte 0x42413938
	.4byte 0x46454443
	.4byte 0x00000000
	.global KorosseoKawa_ScriptA
KorosseoKawa_ScriptA:
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffffc000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000001b
	.global KorosseoKawa_ScriptB
KorosseoKawa_ScriptB:
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte 0x0200afc5
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00012000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00012000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00012000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00012000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000010
	.global KorosseoKawa_DirectionSteps
KorosseoKawa_DirectionSteps:
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.global KorosseoKawa_SceneTableA
KorosseoKawa_SceneTableA:
	.4byte 0xffff0000
	.4byte 0x00000398
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000138
	.4byte 0x40000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global KorosseoKawa_SceneTableB
KorosseoKawa_SceneTableB:
	.4byte 0x0000008f
	.4byte 0x00a0108f
	.4byte 0x00b0c08c
	.4byte 0x0041508c
	.4byte 0x0056208a
	.4byte 0x000001ff
	.global KorosseoKawa_SceneTableC
KorosseoKawa_SceneTableC:
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
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
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff00e5
	.4byte 0x00000007
	.4byte 0x02590000
	.4byte 0x00000000
	.4byte 0x00920000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000007
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00c20000
	.4byte 0x00024000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x03b00000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00028000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x04180000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x03f80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x04380000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x05180000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x05180000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global KorosseoKawa_Countdown
KorosseoKawa_Countdown:
	.4byte 0x00000000
	.global KorosseoKawa_SceneTableD
KorosseoKawa_SceneTableD:
	.4byte 0x00000202
	.4byte 0xffff005a
	.4byte 0x02008955
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x02008239
	.4byte 0x00000202
	.4byte 0xffff000b
	.4byte 0x02008239
	.4byte 0x00008602
	.4byte 0x0302000c
	.4byte 0x02008541
	.4byte 0x0000c602
	.4byte 0xffff0014
	.4byte 0x0200842d
	.4byte 0x00004602
	.4byte 0xffff0014
	.4byte 0x0200842d
	.4byte 0x00008602
	.4byte 0xffff0015
	.4byte 0x02008491
	.4byte 0x00000602
	.4byte 0xffff0015
	.4byte 0x02008491
	.4byte 0x00000002
	.4byte 0x02100032
	.4byte 0x02008841
	.4byte 0x00000013
	.4byte 0x03080064
	.4byte 0x001000b5
	.4byte 0x00000013
	.4byte 0x03090065
	.4byte 0x001000ee
	.4byte 0x00008413
	.4byte 0x030a0067
	.4byte 0x001000b5
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte 0x020081a9
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x020081a9
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x020081a9
	.4byte 0x00000c15
	.4byte 0x0303000f
	.4byte 0x02008249
	.4byte 0x00002115
	.4byte 0x0301000d
	.4byte 0x02008271
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x02009c7d
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x02009215
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x020093e5
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x020095e1
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x020096ed
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x0000213c
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x0000213d
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x0000213e
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x0000213f
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002140
	.4byte 0x00000006
	.4byte 0xffff0063
	.4byte 0x02009a29
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Korosseo_PortraitSlot
Korosseo_PortraitSlot:
	.2byte 0xffff
	.global KorosseoKawa_RoundSpans
KorosseoKawa_RoundSpans:
	.2byte 0x4000
	.4byte 0x0800ff44
	.4byte 0x01801000
	.4byte 0x20000001
	.4byte 0x00010010
	.4byte 0x000e7fff
	.4byte 0x00003000
	.4byte 0x7fff0014
	.4byte 0x3000003c
	.4byte 0x00140800
	.4byte 0x003c7fff
	.4byte 0x1000ffff
	.4byte 0x00010200
	.4byte 0x00002000
	.4byte 0x10000001
	.4byte 0x00060100
	.4byte 0x00102000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060180
	.4byte 0x00067fff
	.4byte 0x01001000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060180
	.4byte 0x00067fff
	.4byte 0x01001000
	.4byte 0x7fff0006
	.4byte 0x20000006
	.4byte 0x001e0000
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060100
	.4byte 0x00067fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060100
	.4byte 0x00067fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0xffff0006
	.global KorosseoKawa_SpanA
KorosseoKawa_SpanA:
	.4byte 0x00801000
	.4byte 0x20000001
	.4byte 0x00010000
	.4byte 0x01001000
	.4byte 0x20000006
	.4byte 0x00060010
	.4byte 0x003c7fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060110
	.4byte 0x00067fff
	.4byte 0x01901000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060120
	.4byte 0x00067fff
	.4byte 0x00002000
	.4byte 0x1000001e
	.4byte 0x000601a0
	.4byte 0x00067fff
	.4byte 0x01301000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000601b0
	.4byte 0x00067fff
	.4byte 0x01401000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000601c0
	.4byte 0x00067fff
	.2byte 0xffff
	.global Korosseo_MarkerSlot
Korosseo_MarkerSlot:
	.2byte 0xffff
	.global Korosseo_RivalFinishScript
Korosseo_RivalFinishScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000022
	.4byte 0x0200b059
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x0000001c
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000096
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000022
	.4byte 0x0200b059
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x0000001c
	.4byte 0x0000001c
	.4byte 0x00000000
	.4byte 0x00000096
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.section .bss,"aw",%nobits
	.global Korosseo_MarkerStep
Korosseo_MarkerStep:
	.space 4
	.global Korosseo_ModeScaleDuration
Korosseo_ModeScaleDuration:
	.space 4
	.global Korosseo_MarkerPriority
Korosseo_MarkerPriority:
	.space 4
	.global Korosseo_CompetitorStartZ
Korosseo_CompetitorStartZ:
	.space 4
	.global Korosseo_MarkerEndX
Korosseo_MarkerEndX:
	.space 4
	.global Korosseo_ModeTaskParam
Korosseo_ModeTaskParam:
	.space 4
	.global Korosseo_ModeScaleStart
Korosseo_ModeScaleStart:
	.space 4
	.global Korosseo_ModeMoveDuration
Korosseo_ModeMoveDuration:
	.space 4
	.global Korosseo_ModeTaskPosition
Korosseo_ModeTaskPosition:
	.space 4
	.global Korosseo_MarkerBlink
Korosseo_MarkerBlink:
	.space 4
	.global Korosseo_ModeScaleTarget
Korosseo_ModeScaleTarget:
	.space 4
	.global Korosseo_ModeMoveStep
Korosseo_ModeMoveStep:
	.space 4
	.global Korosseo_MarkerY
Korosseo_MarkerY:
	.space 4
	.global Korosseo_ModeBlendStep
Korosseo_ModeBlendStep:
	.space 4
	.global Korosseo_CompetitorStartAngle
Korosseo_CompetitorStartAngle:
	.space 4
	.global Korosseo_MarkerSteps
Korosseo_MarkerSteps:
	.space 4
	.global Korosseo_ModeTaskMode
Korosseo_ModeTaskMode:
	.space 4
	.global Korosseo_ModeBlendTarget
Korosseo_ModeBlendTarget:
	.space 4
	.global Korosseo_ModeBlendStart
Korosseo_ModeBlendStart:
	.space 4
	.global Korosseo_ModeTaskTimer
Korosseo_ModeTaskTimer:
	.space 4
	.global Korosseo_ModeTaskScript
Korosseo_ModeTaskScript:
	.space 4
	.global Korosseo_MarkerStartX
Korosseo_MarkerStartX:
	.space 4
	.global Korosseo_ModeBlendDuration
Korosseo_ModeBlendDuration:
	.space 8
	.global Korosseo_MarkerOam
Korosseo_MarkerOam:
	.space 12
	.global Korosseo_MarkerStartY
Korosseo_MarkerStartY:
	.space 4
	.global Korosseo_ModeTaskSprites
Korosseo_ModeTaskSprites:
	.space 48
	.global Korosseo_ModeMoveStart
Korosseo_ModeMoveStart:
	.space 4
	.global Korosseo_MarkerX
Korosseo_MarkerX:
	.space 4
	.global Korosseo_ModeMoveTarget
Korosseo_ModeMoveTarget:
	.space 4
	.global Korosseo_ModeScaleStep
Korosseo_ModeScaleStep:
	.space 4
	.global Korosseo_MarkerEndY
Korosseo_MarkerEndY:
	.space 4
	.global Korosseo_CompetitorStartX
Korosseo_CompetitorStartX:
	.space 4
