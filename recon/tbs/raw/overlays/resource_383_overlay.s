.syntax unified
	.thumb
	.section .text.x02008048,"ax",%progbits
	.p2align 2
	.global Func_02000048
	.thumb_func
Func_02000048:
	push {r5, lr}
	ldr r3, [pc, #40]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #17
	bgt .L_02000048_0
	cmp r3, #15
	blt .L_02000048_0
	ldr r5, [pc, #24]
	b .L_02000048_1
.L_02000048_0:
	ldr r5, [pc, #24]
.L_02000048_1:
	adds r0, r5, #0
	bl 0x0200cd54
	adds r0, r5, #0
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200dcc8
	.4byte 0x0200dab8
	.section .text.x02008310,"ax",%progbits
	.p2align 2
	.global Func_02000310
	.thumb_func
Func_02000310:
	push {lr}
	ldr r3, [pc, #44]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #17
	bgt .L_02000310_0
	cmp r3, #15
	blt .L_02000310_0
	ldr r0, [pc, #28]
	b .L_02000310_1
.L_02000310_0:
	ldr r0, [pc, #28]
	bl 0x0200cd14
	cmp r0, #0
	beq .L_02000310_2
	ldr r0, [pc, #20]
	b .L_02000310_1
.L_02000310_2:
	ldr r0, [pc, #20]
.L_02000310_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200e1fc
	.4byte 0x00000855
	.4byte 0x0200e250
	.4byte 0x0200de30
	.section .text.x02008690,"ax",%progbits
	.p2align 2
	push	{r5, r6, r7, lr}
	movs	r0, #25
	bl 0x0200cd7c
	movs	r7, #240
	ldrh	r3, [r0, #6]
	adds	r5, r0, #0
	adds	r5, #100
	lsls	r7, r7, #8
	ands	r7, r3
	ldrh	r3, [r5, #0]
	lsls	r3, r3, #16
	asrs	r6, r3, #17
	bl 0x0200cd44
	movs	r1, #2
	movs	r0, #25
	bl 0x0200cdfc
	ldr	r0, [pc, #240]
	bl 0x0200ce14
	movs	r0, #25
	movs	r1, #0
	bl 0x0200ce24
	movs	r1, #224
	movs	r2, #224
	movs	r0, #25
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200cd84
	movs	r0, #0
	ldrsh	r3, [r5, r0]
	cmp	r3, #4
	bhi.n	.L_0200078e
	ldr	r2, [pc, #208]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x0200871a
	.4byte 0x02008742
	.4byte 0x0200871a
	.4byte 0x02008742
	.4byte 0x020086f8
	.4byte 0x482e4a2d
	.4byte 0x428318bb
	.4byte 0x492dd805
	.4byte 0xf0042019
	.4byte 0x2302fb41
	.4byte 0x492be03e
	.4byte 0xf0042019
	.4byte 0x2303fb3b
	.4byte 0x4a25e038
	.4byte 0x18bb4825
	.4byte 0xd8224283
	.4byte 0x5e2a2000
	.4byte 0x189b00b3
	.4byte 0x009b4924
	.4byte 0x201958c9
	.4byte 0xfb2af004
	.4byte 0x0072882b
	.4byte 0x33011a9b
	.4byte 0x4820e024
	.4byte 0x183b4a1b
	.4byte 0xd80e4293
	.4byte 0x5e2a2000
	.4byte 0x189b00b3
	.4byte 0x009b491a
	.4byte 0x201958c9
	.4byte 0xfb16f004
	.4byte 0x0072882b
	.4byte 0x33011a9b
	.4byte 0x2301e010
	.4byte 0x5e2a2000
	.4byte 0x009b4073
	.4byte 0x4912189b
	.4byte 0x58c9009b
	.4byte 0xf0042019
	.4byte 0x882bfb05
	.4byte 0x1a9b0072
	.4byte 0x189b4a0f
	.2byte 0x802b
.L_0200078e:
	ldrh	r2, [r5, #0]
	movs	r3, #3
	ands	r3, r2
	strh	r3, [r5, #0]
	movs	r0, #25
	bl 0x0200cd94
	bl 0x0200cd4c
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x000012ad
	.4byte 0x020086e4
	.4byte 0xffffdfff
	.4byte 0x00007ffe
	.4byte 0x0200d8bc
	.4byte 0x0200d858
	.4byte 0x0200e4d8
	.4byte 0xffff9fff
	.2byte 0xffff
	.2byte 0x0000
	.section .text.x02009348,"ax",%progbits
	.p2align 2
	.global Func_02001348
	.thumb_func
Func_02001348:
	push {r5, lr}
	bl 0x0200cd44
	movs r0, #0
	ldr r1, [pc, #1016]
	ldr r2, [pc, #1020]
	bl 0x0200cd84
	movs r0, #1
	ldr r1, [pc, #1008]
	ldr r2, [pc, #1008]
	bl 0x0200cd84
	ldr r2, [pc, #1004]
	movs r0, #2
	ldr r1, [pc, #996]
	bl 0x0200cd84
	movs r1, #3
	movs r0, #0
	bl 0x0200cdfc
	movs r0, #20
	bl 0x0200cd3c
	ldr r5, [pc, #984]
	movs r1, #1
	adds r0, r5, #0
	adds r5, #1
	bl 0x0200ccf4
	adds r0, r5, #0
	bl 0x0200ce14
	movs r0, #30
	bl 0x0200cd3c
	movs r1, #1
	movs r0, #8
	bl 0x0200cdd4
	movs r0, #60
	bl 0x0200cd3c
	movs r0, #8
	movs r1, #3
	movs r2, #40
	bl 0x0200c63c
	movs r1, #198
	movs r2, #220
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200cdb4
	movs r0, #0
	movs r1, #8
	movs r2, #20
	bl 0x0200c60c
	movs r1, #198
	movs r2, #220
	movs r0, #1
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200cdcc
	movs r1, #198
	movs r2, #220
	movs r0, #2
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200cdcc
	movs r1, #202
	movs r2, #216
	movs r0, #1
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200cdac
	movs r1, #198
	movs r2, #228
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #2
	bl 0x0200cdac
	movs r0, #1
	bl 0x0200cdc4
	movs r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200ce04
	movs r0, #2
	bl 0x0200cdc4
	movs r0, #2
	movs r1, #8
	movs r2, #60
	bl 0x0200c60c
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #8
	bl 0x0200ce3c
	movs r0, #60
	bl 0x0200cd3c
	movs r0, #8
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #2
	movs r1, #3
	bl 0x0200cdd4
	movs r2, #30
	movs r0, #1
	movs r1, #3
	bl 0x0200c63c
	movs r1, #2
	movs r0, #8
	bl 0x0200cdfc
	movs r0, #20
	bl 0x0200cd3c
	movs r0, #8
	movs r1, #20
	bl 0x0200c5f4
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ce3c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #1
	bl 0x0200ce3c
	movs r0, #60
	bl 0x0200cd3c
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #8
	bl 0x0200ce2c
	movs r0, #10
	bl 0x0200cd3c
	movs r0, #8
	movs r1, #30
	bl 0x0200c5f4
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl 0x0200ce2c
	movs r0, #30
	bl 0x0200cd3c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #8
	bl 0x0200ce3c
	movs r0, #60
	bl 0x0200cd3c
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl 0x0200ce2c
	movs r0, #40
	bl 0x0200cd3c
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl 0x0200ce2c
	movs r0, #40
	bl 0x0200cd3c
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c60c
	movs r2, #30
	movs r0, #8
	movs r1, #4
	bl 0x0200c63c
	movs r0, #8
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #2
	movs r1, #1
	bl 0x0200cdfc
	movs r0, #2
	movs r1, #40
	bl 0x0200c5f4
	movs r1, #186
	movs r2, #204
	lsls r2, r2, #17
	lsls r1, r1, #18
	movs r0, #10
	bl 0x0200cdcc
	movs r0, #61
	bl 0x0200cf24
	movs r0, #10
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #0
	movs r1, #1
	bl 0x0200cdf4
	movs r0, #1
	movs r1, #1
	bl 0x0200cdf4
	movs r0, #2
	movs r1, #1
	bl 0x0200cdf4
	movs r1, #1
	movs r0, #8
	bl 0x0200cdfc
	movs r0, #30
	bl 0x0200cd3c
	movs r0, #0
	movs r1, #10
	movs r2, #0
	bl 0x0200ce04
	movs r0, #1
	movs r1, #10
	movs r2, #0
	bl 0x0200ce04
	movs r0, #2
	movs r1, #10
	movs r2, #0
	bl 0x0200ce04
	movs r0, #8
	movs r1, #10
	movs r2, #40
	bl 0x0200c60c
	movs r0, #10
	ldr r1, [pc, #468]
	ldr r2, [pc, #472]
	bl 0x0200cd84
	movs r1, #192
	movs r2, #192
	movs r0, #11
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200cd84
	movs r1, #192
	movs r2, #192
	movs r0, #12
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200cd84
	movs r1, #186
	movs r2, #204
	movs r0, #11
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200cdcc
	movs r1, #186
	movs r2, #204
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200cdcc
	movs r1, #198
	movs r2, #208
	movs r0, #10
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200cdb4
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl 0x0200ce2c
	movs r0, #70
	bl 0x0200cd3c
	movs r0, #0
	movs r1, #10
	movs r2, #0
	bl 0x0200ce04
	movs r0, #1
	movs r1, #10
	movs r2, #0
	bl 0x0200ce04
	movs r0, #2
	movs r1, #10
	movs r2, #0
	bl 0x0200ce04
	movs r2, #0
	movs r0, #8
	movs r1, #10
	bl 0x0200ce04
	ldr r1, [pc, #344]
	movs r0, #11
	bl 0x0200cd8c
	movs r0, #40
	bl 0x0200cd3c
	ldr r1, [pc, #336]
	movs r0, #12
	bl 0x0200cd8c
	movs r0, #12
	bl 0x0200cd94
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ce2c
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #12
	bl 0x0200ce2c
	movs r0, #40
	bl 0x0200cd3c
	movs r2, #20
	movs r0, #10
	movs r1, #4
	bl 0x0200c63c
	movs r0, #10
	movs r1, #20
	bl 0x0200c5f4
	movs r1, #1
	movs r0, #11
	bl 0x0200cdfc
	movs r0, #10
	bl 0x0200cd3c
	movs r0, #11
	movs r1, #20
	bl 0x0200c5f4
	movs r1, #1
	movs r0, #12
	bl 0x0200cdfc
	movs r0, #10
	bl 0x0200cd3c
	movs r0, #12
	movs r1, #40
	bl 0x0200c5f4
	ldr r1, [pc, #232]
	movs r2, #0
	movs r0, #2
	bl 0x0200ce3c
	movs r0, #60
	bl 0x0200cd3c
	movs r2, #20
	movs r0, #10
	movs r1, #4
	bl 0x0200c63c
	movs r0, #10
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #0
	ldr r1, [pc, #196]
	movs r2, #0
	bl 0x0200ce3c
	ldr r1, [pc, #188]
	movs r2, #0
	movs r0, #1
	bl 0x0200ce3c
	movs r0, #60
	bl 0x0200cd3c
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #8
	bl 0x0200ce3c
	movs r0, #40
	bl 0x0200cd3c
	movs r1, #2
	movs r0, #8
	bl 0x0200cdfc
	movs r0, #20
	bl 0x0200cd3c
	movs r0, #8
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #0
	movs r1, #8
	movs r2, #0
	bl 0x0200ce04
	movs r0, #1
	movs r1, #8
	movs r2, #0
	bl 0x0200ce04
	movs r0, #2
	movs r1, #8
	movs r2, #20
	bl 0x0200c60c
	movs r2, #30
	movs r0, #8
	movs r1, #4
	bl 0x0200c63c
	movs r0, #8
	movs r1, #20
	bl 0x0200c5f4
	movs r1, #2
	movs r0, #2
	bl 0x0200cdfc
	movs r0, #20
	bl 0x0200cd3c
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x0200ce04
	movs r2, #30
	movs r0, #1
	movs r1, #2
	bl 0x0200c60c
	movs r0, #1
	movs r1, #20
	bl 0x0200c5f4
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #8
	bl 0x0200ce3c
	movs r0, #60
	bl 0x0200cd3c
	movs r0, #8
	movs r1, #20
	b .L_02001348_0
	.2byte 0x0000
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x000012c5
	.4byte 0x0200d248
	.4byte 0x0200d2ac
	.4byte 0x00000101
.L_02001348_0:
	bl 0x0200c5f4
	movs r1, #1
	movs r0, #10
	bl 0x0200cdfc
	movs r0, #10
	bl 0x0200cd3c
	movs r0, #10
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #0
	movs r1, #10
	movs r2, #0
	bl 0x0200ce04
	movs r0, #1
	movs r1, #10
	movs r2, #0
	bl 0x0200ce04
	movs r0, #2
	movs r1, #10
	movs r2, #20
	bl 0x0200c60c
	movs r2, #20
	movs r0, #11
	movs r1, #3
	bl 0x0200c63c
	movs r0, #11
	movs r1, #20
	bl 0x0200c5f4
	movs r2, #20
	movs r0, #12
	movs r1, #4
	bl 0x0200c63c
	movs r0, #12
	movs r1, #20
	bl 0x0200c5f4
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ce3c
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ce3c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #2
	bl 0x0200ce3c
	movs r0, #60
	bl 0x0200cd3c
	movs r2, #20
	movs r0, #10
	movs r1, #3
	bl 0x0200c63c
	movs r1, #0
	movs r0, #10
	bl 0x0200ce1c
	movs r0, #50
	bl 0x0200cd3c
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200ce04
	movs r0, #1
	movs r1, #0
	movs r2, #30
	bl 0x0200c60c
	movs r0, #0
	movs r1, #0
	bl 0x0200cd74
	cmp r0, #0
	bne .L_02001348_1
	movs r0, #40
	bl 0x0200cd3c
	movs r0, #1
	movs r1, #10
	movs r2, #0
	bl 0x0200ce04
	movs r2, #0
	movs r0, #2
	movs r1, #10
	bl 0x0200ce04
	movs r1, #2
	movs r0, #10
	bl 0x0200cdfc
	movs r0, #20
	bl 0x0200cd3c
	movs r0, #10
	movs r1, #0
	bl 0x0200ce24
	b .L_02001348_2
.L_02001348_1:
	movs r0, #40
	bl 0x0200cd3c
	movs r0, #1
	movs r1, #10
	movs r2, #0
	bl 0x0200ce04
	movs r2, #0
	movs r1, #10
	movs r0, #2
	bl 0x0200ce04
	ldr r5, [pc, #260]
	adds r0, r5, #0
	bl 0x0200ce14
	movs r1, #2
	movs r0, #10
	bl 0x0200cdfc
	movs r0, #20
	bl 0x0200cd3c
	subs r5, #3
	movs r0, #10
	movs r1, #0
	bl 0x0200ce24
	adds r0, r5, #0
	bl 0x0200ce14
.L_02001348_2:
	movs r2, #0
	ldr r1, [pc, #224]
	movs r0, #11
	bl 0x0200ce3c
	movs r0, #60
	bl 0x0200cd3c
	movs r0, #11
	movs r1, #20
	bl 0x0200c5f4
	movs r2, #20
	movs r0, #12
	movs r1, #4
	bl 0x0200c63c
	movs r0, #12
	movs r1, #30
	bl 0x0200c5f4
	movs r2, #30
	movs r0, #10
	movs r1, #11
	bl 0x0200c624
	movs r0, #10
	movs r1, #3
	bl 0x0200cdd4
	movs r0, #11
	movs r1, #3
	movs r2, #30
	bl 0x0200c63c
	movs r2, #30
	movs r0, #10
	movs r1, #12
	bl 0x0200c624
	movs r0, #10
	movs r1, #3
	bl 0x0200cdd4
	movs r0, #12
	movs r1, #3
	movs r2, #40
	bl 0x0200c63c
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ce2c
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ce2c
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #12
	bl 0x0200ce2c
	movs r0, #20
	bl 0x0200cd3c
	movs r2, #20
	movs r0, #10
	movs r1, #4
	bl 0x0200c63c
	movs r1, #0
	movs r0, #10
	bl 0x0200ce24
	ldr r0, [pc, #72]
	bl 0x0200cd1c
	ldr r3, [pc, #68]
	movs r2, #224
	ldr r3, [r3]
	ldr r5, [pc, #68]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #64
	str r2, [r3]
	adds r0, r5, #0
	movs r1, #17
	bl 0x0200ce7c
	adds r0, r5, #0
	movs r1, #16
	bl 0x0200ce84
	ldr r3, [pc, #44]
	ldr r2, [pc, #48]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	movs r0, #12
	movs r1, #5
	bl 0x0200ce74
	bl 0x0200cd4c
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x000012dc
	.4byte 0x00000103
	.4byte 0x00000854
	.4byte 0x03001ebc
	.4byte 0x00000015
	.4byte 0x02000240
	.4byte 0x0000022b
	.section .text.x0200adb4,"ax",%progbits
	.p2align 2
	.global Func_02002db4
	.thumb_func
Func_02002db4:
	.global KuupuappuHeya_RunVaultEvent
	.thumb_func
KuupuappuHeya_RunVaultEvent:
	push {r5, r6, lr}
	movs r1, #198
	movs r2, #208
	movs r0, #10
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200cdcc
	movs r1, #200
	movs r2, #200
	movs r0, #11
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200cdcc
	movs r1, #194
	movs r2, #208
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200cdcc
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ce2c
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ce2c
	movs r1, #192
	movs r0, #12
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ce2c
	movs r1, #198
	movs r2, #220
	movs r0, #0
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200cdcc
	movs r1, #202
	movs r2, #216
	movs r0, #1
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200cdcc
	movs r1, #198
	movs r2, #228
	movs r0, #2
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200cdcc
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ce2c
	movs r1, #176
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ce2c
	movs r1, #176
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ce2c
	movs r0, #8
	movs r1, #10
	movs r2, #0
	bl 0x0200ce04
	ldr r5, [pc, #268]
	ldr r3, [pc, #272]
	ldr r2, [r5]
	movs r6, #224
	lsls r6, r6, #1
	movs r1, #0
	str r3, [r2, r6]
	movs r0, #0
	bl 0x0200ce4c
	bl 0x0200ce5c
	bl 0x0200ccc4
	movs r0, #1
	bl 0x0200cc6c
	ldr r3, [r5]
	movs r2, #228
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #32
	str r2, [r3]
	bl 0x0200c5cc
	movs r0, #60
	bl 0x0200cd3c
	ldr r0, [pc, #220]
	bl 0x0200ce14
	movs r1, #1
	movs r0, #11
	bl 0x0200cdfc
	movs r0, #20
	bl 0x0200cd3c
	movs r0, #11
	movs r1, #30
	bl 0x0200c5f4
	movs r1, #1
	movs r0, #12
	bl 0x0200cdfc
	movs r0, #20
	bl 0x0200cd3c
	movs r0, #12
	movs r1, #0
	bl 0x0200ce24
	movs r2, #30
	movs r0, #10
	movs r1, #11
	bl 0x0200c624
	movs r0, #10
	movs r1, #3
	bl 0x0200cdd4
	movs r0, #11
	movs r1, #3
	movs r2, #30
	bl 0x0200c63c
	movs r2, #30
	movs r0, #10
	movs r1, #12
	bl 0x0200c624
	movs r0, #10
	movs r1, #3
	bl 0x0200cdd4
	movs r0, #12
	movs r1, #3
	movs r2, #40
	bl 0x0200c63c
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ce2c
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ce2c
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #12
	bl 0x0200ce2c
	movs r0, #20
	bl 0x0200cd3c
	movs r2, #20
	movs r0, #10
	movs r1, #4
	bl 0x0200c63c
	movs r0, #10
	movs r1, #0
	bl 0x0200ce24
	ldr r2, [r5]
	movs r3, #128
	ldr r5, [pc, #56]
	lsls r3, r3, #2
	str r3, [r2, r6]
	adds r0, r5, #0
	movs r1, #17
	bl 0x0200ce7c
	adds r0, r5, #0
	movs r1, #16
	bl 0x0200ce84
	ldr r3, [pc, #36]
	ldr r2, [pc, #40]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	movs r0, #12
	movs r1, #5
	bl 0x0200ce74
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x00000209
	.4byte 0x000012e1
	.4byte 0x00000015
	.4byte 0x02000240
	.4byte 0x0000022b
	.section .text.x0200af98,"ax",%progbits
	.p2align 2
	.global Func_02002f98
	.thumb_func
Func_02002f98:
	push {lr}
	bl 0x0200cf1c
	movs r0, #2
	bl 0x0200cd04
	adds r0, #248
	ldr r3, [r0]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02002f98_0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	movs r0, #2
	bl 0x0200cd2c
	movs r0, #126
	bl 0x0200cf24
	movs r0, #0
	bl 0x0200cd0c
	movs r0, #2
	bl 0x0200cd0c
.L_02002f98_0:
	pop {r1}
	bx r1
	.2byte 0x0000
	.section .text.x0200b6f8,"ax",%progbits
	.p2align 2
	.global Func_020036f8
	.thumb_func
Func_020036f8:
	.global RunDialoguePromptScene
	.thumb_func
RunDialoguePromptScene:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r1, [pc, #1016]
	ldr r0, [pc, #1016]
	mov r10, r1
	ldr r7, [r1]
	bl 0x0200cd1c
	bl 0x0200cd44
	movs r0, #12
	bl 0x0200cd7c
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r1, #218
	strb r3, [r0]
	lsls r1, r1, #2
	movs r0, #15
	ldr r2, [pc, #984]
	bl 0x0200cda4
	movs r1, #218
	movs r0, #16
	lsls r1, r1, #2
	ldr r2, [pc, #976]
	bl 0x0200cda4
	movs r1, #218
	movs r0, #17
	lsls r1, r1, #2
	ldr r2, [pc, #968]
	bl 0x0200cda4
	movs r1, #194
	movs r2, #196
	movs r0, #11
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200cdcc
	movs r1, #198
	movs r2, #196
	movs r0, #10
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200cdcc
	movs r1, #202
	movs r2, #196
	lsls r2, r2, #17
	movs r0, #12
	lsls r1, r1, #18
	bl 0x0200cdcc
	movs r0, #10
	movs r1, #5
	bl 0x0200cdd4
	movs r0, #11
	movs r1, #5
	bl 0x0200cdd4
	movs r0, #12
	movs r1, #5
	bl 0x0200cdd4
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x0200ce04
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200ce04
	movs r2, #0
	movs r1, #0
	movs r0, #12
	bl 0x0200ce04
	movs r0, #10
	bl 0x0200cd7c
	movs r1, #1
	bl 0x0200ccec
	movs r0, #11
	bl 0x0200cd7c
	movs r1, #1
	bl 0x0200ccec
	movs r0, #12
	bl 0x0200cd7c
	movs r1, #1
	bl 0x0200ccec
	movs r1, #192
	movs r2, #204
	movs r0, #13
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200cdcc
	movs r1, #192
	movs r2, #212
	movs r0, #14
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200cdcc
	movs r1, #196
	movs r2, #212
	movs r0, #9
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200cda4
	movs r1, #202
	movs r2, #204
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200cdcc
	movs r0, #13
	movs r1, #9
	movs r2, #0
	bl 0x0200ce04
	movs r0, #8
	movs r1, #9
	movs r2, #0
	bl 0x0200ce04
	movs r0, #14
	movs r1, #10
	movs r2, #0
	bl 0x0200ce04
	movs r0, #9
	movs r1, #10
	movs r2, #0
	bl 0x0200ce04
	movs r1, #198
	movs r2, #220
	movs r0, #0
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200cdcc
	movs r1, #202
	movs r2, #220
	movs r0, #1
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200cdcc
	movs r1, #194
	movs r2, #220
	movs r0, #2
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200cdcc
	movs r0, #0
	movs r1, #10
	movs r2, #0
	bl 0x0200ce04
	movs r0, #1
	movs r1, #10
	movs r2, #0
	bl 0x0200ce04
	movs r0, #2
	movs r1, #10
	movs r2, #0
	bl 0x0200ce04
	mov r3, r10
	ldr r2, [r3]
	movs r1, #228
	lsls r1, r1, #1
	movs r3, #30
	str r3, [r2, r1]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r2, r3
	adds r3, #65
	str r3, [r2]
	mov r9, r1
	bl 0x0200ce9c
	bl 0x0200ceac
	movs r0, #40
	bl 0x0200cd3c
	movs r1, #2
	movs r0, #10
	bl 0x0200cdfc
	ldr r6, [pc, #620]
	adds r0, r6, #0
	bl 0x0200ce14
	movs r0, #10
	movs r1, #20
	bl 0x0200c5f4
	movs r1, #1
	movs r0, #9
	bl 0x0200cdfc
	movs r0, #20
	bl 0x0200cd3c
	movs r2, #20
	movs r0, #9
	movs r1, #4
	bl 0x0200c63c
	movs r0, #9
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #13
	movs r1, #3
	bl 0x0200cdd4
	movs r2, #20
	movs r0, #8
	movs r1, #3
	bl 0x0200c63c
	movs r0, #11
	movs r1, #2
	bl 0x0200cdf4
	movs r1, #2
	movs r0, #12
	bl 0x0200cdf4
	movs r0, #60
	bl 0x0200cd3c
	movs r0, #13
	ldr r1, [pc, #536]
	ldr r2, [pc, #536]
	bl 0x0200cd84
	movs r2, #204
	movs r0, #13
	ldr r1, [pc, #532]
	lsls r2, r2, #1
	bl 0x0200cdac
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ce2c
	movs r1, #176
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #8
	bl 0x0200ce2c
	ldr r5, [pc, #504]
	movs r0, #11
	adds r1, r5, #0
	bl 0x0200cd8c
	movs r0, #20
	bl 0x0200cd3c
	adds r1, r5, #0
	movs r0, #10
	bl 0x0200cd8c
	movs r0, #15
	bl 0x0200cd3c
	adds r1, r5, #0
	movs r0, #12
	bl 0x0200cd8c
	movs r0, #35
	bl 0x0200cd3c
	ldr r1, [pc, #464]
	movs r0, #8
	bl 0x0200cd8c
	movs r0, #20
	bl 0x0200cd3c
	movs r0, #13
	bl 0x0200cdc4
	movs r1, #0
	movs r2, #0
	movs r0, #13
	bl 0x0200cdcc
	movs r0, #40
	bl 0x0200cd3c
	movs r0, #9
	ldr r1, [pc, #408]
	ldr r2, [pc, #412]
	bl 0x0200cd84
	movs r0, #14
	ldr r1, [pc, #400]
	ldr r2, [pc, #400]
	bl 0x0200cd84
	movs r1, #196
	movs r2, #204
	movs r0, #9
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200cdb4
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200ce04
	movs r1, #192
	movs r2, #204
	movs r0, #14
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200cdb4
	movs r0, #14
	movs r1, #0
	movs r2, #20
	bl 0x0200c60c
	movs r2, #20
	movs r0, #9
	movs r1, #3
	bl 0x0200c63c
	movs r0, #9
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #14
	movs r1, #3
	movs r2, #20
	bl 0x0200c63c
	movs r1, #128
	movs r2, #10
	movs r0, #14
	lsls r1, r1, #6
	bl 0x0200ce2c
	movs r0, #14
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #0
	movs r1, #1
	movs r2, #50
	bl 0x0200c624
	movs r0, #0
	movs r1, #2
	movs r2, #50
	bl 0x0200c624
	movs r0, #0
	movs r1, #9
	movs r2, #0
	bl 0x0200ce04
	movs r0, #1
	movs r1, #9
	movs r2, #0
	bl 0x0200ce04
	movs r2, #20
	movs r0, #2
	movs r1, #9
	bl 0x0200c60c
	movs r0, #0
	movs r1, #3
	bl 0x0200cdd4
	movs r0, #1
	movs r1, #3
	bl 0x0200cdd4
	movs r0, #2
	movs r1, #3
	movs r2, #50
	bl 0x0200c63c
	movs r0, #9
	movs r1, #14
	movs r2, #0
	bl 0x0200ce0c
	movs r2, #20
	movs r0, #9
	movs r1, #3
	bl 0x0200c63c
	movs r0, #9
	movs r1, #20
	bl 0x0200c5f4
	ldr r1, [pc, #212]
	movs r0, #14
	bl 0x0200cd8c
	movs r0, #50
	bl 0x0200cd3c
	ldr r1, [pc, #200]
	movs r0, #9
	bl 0x0200cd8c
	movs r0, #1
	ldr r1, [pc, #168]
	ldr r2, [pc, #168]
	bl 0x0200cd84
	movs r0, #2
	ldr r1, [pc, #156]
	ldr r2, [pc, #160]
	bl 0x0200cd84
	movs r1, #198
	movs r0, #1
	lsls r1, r1, #2
	mov r2, r9
	bl 0x0200cdb4
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200ce2c
	movs r0, #30
	bl 0x0200cd3c
	movs r1, #198
	movs r2, #204
	movs r0, #2
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200cdb4
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200ce2c
	movs r1, #202
	movs r0, #1
	lsls r1, r1, #2
	mov r2, r9
	bl 0x0200cdb4
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200ce2c
	movs r0, #100
	bl 0x0200cd3c
	movs r0, #14
	movs r1, #9
	movs r2, #60
	bl 0x0200c60c
	movs r0, #9
	movs r1, #14
	movs r2, #40
	bl 0x0200c60c
	movs r0, #9
	movs r1, #3
	movs r2, #40
	bl 0x0200c63c
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl 0x0200ce2c
	movs r0, #20
	b .L_020036f8_0
	.4byte 0x03001ebc
	.4byte 0x00000855
	.4byte 0x000001a9
	.4byte 0x00000199
	.4byte 0x00000179
	.4byte 0x000012fc
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x000002ea
	.4byte 0x0200d354
	.4byte 0x0200d2fc
	.4byte 0x0200d3ac
	.4byte 0x0200d444
.L_020036f8_0:
	bl 0x0200cd3c
	movs r1, #2
	movs r0, #9
	bl 0x0200cdfc
	movs r0, #124
	bl 0x0200cf24
	movs r0, #15
	movs r1, #4
	bl 0x0200cdd4
	movs r1, #218
	movs r2, #212
	lsls r2, r2, #17
	movs r0, #18
	lsls r1, r1, #18
	bl 0x0200cdcc
	movs r0, #18
	movs r1, #1
	bl 0x0200ce34
	movs r0, #18
	ldr r1, [pc, #268]
	ldr r2, [pc, #268]
	bl 0x0200cd84
	movs r2, #8
	negs r2, r2
	movs r1, #0
	movs r0, #18
	bl 0x0200cdbc
	movs r0, #18
	bl 0x0200cdc4
	movs r1, #2
	movs r0, #18
	bl 0x0200cdfc
	movs r0, #60
	bl 0x0200cd3c
	adds r0, r6, #5
	movs r1, #1
	bl 0x0200ccf4
	movs r0, #15
	movs r1, #2
	bl 0x0200cdd4
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl 0x0200cdcc
	mov r1, r10
	movs r3, #236
	ldr r2, [r1]
	lsls r3, r3, #1
	mov r8, r3
	add r2, r8
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #14
	movs r1, #1
	bl 0x0200cdfc
	movs r0, #14
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #0
	movs r1, #1
	movs r2, #40
	bl 0x0200c624
	movs r0, #9
	movs r1, #14
	movs r2, #20
	bl 0x0200c60c
	movs r2, #20
	movs r0, #9
	movs r1, #3
	bl 0x0200c63c
	movs r0, #9
	movs r1, #30
	bl 0x0200c5f4
	movs r0, #0
	movs r1, #14
	movs r2, #0
	bl 0x0200ce04
	movs r0, #1
	movs r1, #14
	movs r2, #40
	bl 0x0200c60c
	movs r2, #0
	movs r1, #0
	movs r0, #14
	bl 0x0200ce2c
	movs r0, #40
	bl 0x0200cd3c
	movs r1, #2
	movs r0, #14
	bl 0x0200cdfc
	movs r0, #124
	bl 0x0200cf24
	movs r1, #4
	movs r0, #16
	bl 0x0200cdd4
	movs r0, #19
	bl 0x0200cd7c
	ldr r5, [pc, #60]
	adds r0, #85
	strb r5, [r0]
	movs r1, #1
	movs r0, #19
	bl 0x0200ce34
	movs r1, #218
	movs r2, #204
	movs r0, #19
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200cdcc
	movs r0, #19
	ldr r1, [pc, #36]
	ldr r2, [pc, #36]
	bl 0x0200cd84
	movs r2, #8
	negs r2, r2
	movs r1, #0
	movs r0, #19
	bl 0x0200cdbc
	movs r0, #19
	bl 0x0200cdc4
	movs r1, #2
	movs r0, #19
	b .L_020036f8_1
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0000cccc
	.4byte 0x00006666
.L_020036f8_1:
	bl 0x0200cdfc
	adds r6, #8
	movs r0, #60
	bl 0x0200cd3c
	adds r0, r6, #0
	movs r1, #1
	bl 0x0200ccf4
	movs r0, #16
	movs r1, #2
	bl 0x0200cdd4
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl 0x0200cdcc
	mov r1, r10
	ldr r2, [r1]
	add r2, r8
	ldrh r3, [r2]
	movs r1, #129
	adds r3, #1
	strh r3, [r2]
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #9
	bl 0x0200ce3c
	movs r0, #60
	bl 0x0200cd3c
	movs r0, #9
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #0
	movs r1, #2
	bl 0x0200cdf4
	movs r0, #1
	movs r1, #2
	bl 0x0200cdf4
	movs r1, #2
	movs r0, #2
	bl 0x0200cdfc
	movs r0, #20
	bl 0x0200cd3c
	movs r0, #14
	movs r1, #3
	movs r2, #50
	bl 0x0200c63c
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl 0x0200c60c
	movs r0, #9
	movs r1, #30
	bl 0x0200c5f4
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #14
	bl 0x0200ce2c
	movs r0, #30
	bl 0x0200cd3c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #14
	bl 0x0200ce3c
	movs r0, #60
	bl 0x0200cd3c
	movs r1, #214
	movs r2, #188
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #14
	bl 0x0200cdb4
	movs r0, #20
	bl 0x0200cd3c
	movs r2, #20
	movs r0, #14
	movs r1, #9
	bl 0x0200c60c
	movs r0, #14
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #9
	movs r1, #14
	movs r2, #0
	bl 0x0200ce04
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #2
	bl 0x0200ce3c
	movs r0, #60
	bl 0x0200cd3c
	movs r0, #2
	movs r1, #14
	movs r2, #30
	bl 0x0200c60c
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl 0x0200c60c
	movs r2, #20
	movs r0, #9
	movs r1, #3
	bl 0x0200c63c
	movs r0, #9
	movs r1, #0
	bl 0x0200ce24
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #14
	bl 0x0200ce2c
	movs r0, #30
	bl 0x0200cd3c
	movs r0, #2
	movs r1, #9
	movs r2, #20
	bl 0x0200c60c
	movs r2, #20
	movs r0, #2
	movs r1, #3
	bl 0x0200c63c
	movs r0, #9
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x0200ce0c
	movs r2, #20
	movs r0, #1
	movs r1, #2
	bl 0x0200c60c
	movs r0, #0
	movs r1, #3
	bl 0x0200cdd4
	movs r0, #1
	movs r1, #3
	movs r2, #40
	bl 0x0200c63c
	movs r2, #30
	movs r0, #2
	movs r1, #3
	bl 0x0200c63c
	movs r1, #1
	movs r0, #9
	bl 0x0200cdfc
	movs r0, #20
	bl 0x0200cd3c
	movs r2, #20
	movs r0, #9
	movs r1, #4
	bl 0x0200c63c
	movs r0, #9
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #0
	movs r1, #9
	movs r2, #0
	bl 0x0200ce04
	movs r0, #1
	movs r1, #9
	movs r2, #0
	bl 0x0200ce04
	movs r2, #20
	movs r0, #2
	movs r1, #9
	bl 0x0200c60c
	movs r0, #0
	movs r1, #1
	bl 0x0200cdf4
	movs r0, #1
	movs r1, #1
	bl 0x0200cdf4
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #2
	bl 0x0200ce3c
	movs r0, #60
	bl 0x0200cd3c
	movs r0, #2
	movs r1, #40
	bl 0x0200c5f4
	movs r2, #20
	movs r0, #9
	movs r1, #3
	bl 0x0200c63c
	movs r0, #0
	movs r1, #4
	bl 0x0200cdd4
	movs r0, #1
	movs r1, #4
	bl 0x0200cddc
	movs r1, #3
	movs r0, #2
	bl 0x0200cdfc
	movs r0, #30
	bl 0x0200cd3c
	movs r2, #20
	movs r0, #9
	movs r1, #4
	bl 0x0200c63c
	movs r0, #9
	movs r1, #20
	bl 0x0200c5f4
	movs r2, #20
	movs r0, #0
	movs r1, #1
	bl 0x0200c624
	movs r0, #2
	movs r1, #2
	bl 0x0200cdfc
	movs r0, #2
	movs r1, #4
	movs r2, #30
	bl 0x0200c63c
	movs r1, #192
	movs r2, #192
	movs r0, #2
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200cd84
	movs r1, #200
	movs r2, #204
	lsls r2, r2, #1
	movs r0, #2
	lsls r1, r1, #2
	bl 0x0200cdb4
	movs r0, #2
	movs r1, #2
	bl 0x0200cdfc
	movs r0, #2
	movs r1, #0
	bl 0x0200ce24
	movs r0, #0
	movs r1, #9
	movs r2, #0
	bl 0x0200ce04
	movs r0, #1
	movs r1, #9
	movs r2, #30
	bl 0x0200c60c
	movs r2, #20
	movs r0, #9
	movs r1, #4
	bl 0x0200c63c
	movs r0, #9
	movs r1, #20
	bl 0x0200c5f4
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200ce44
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200ce44
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #2
	bl 0x0200ce44
	movs r0, #60
	bl 0x0200cd3c
	movs r0, #2
	movs r1, #20
	bl 0x0200c5f4
	movs r1, #1
	movs r0, #9
	bl 0x0200cdfc
	movs r0, #20
	bl 0x0200cd3c
	movs r0, #9
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #0
	ldr r1, [pc, #992]
	movs r2, #0
	bl 0x0200ce3c
	movs r0, #1
	ldr r1, [pc, #984]
	movs r2, #0
	bl 0x0200ce3c
	ldr r1, [pc, #976]
	movs r2, #0
	movs r0, #2
	bl 0x0200ce3c
	movs r0, #60
	bl 0x0200cd3c
	movs r2, #20
	movs r0, #9
	movs r1, #4
	bl 0x0200c63c
	movs r0, #9
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #0
	movs r1, #1
	bl 0x0200cdf4
	movs r0, #1
	movs r1, #1
	bl 0x0200cdf4
	movs r1, #1
	movs r0, #2
	bl 0x0200cdfc
	movs r0, #20
	bl 0x0200cd3c
	movs r2, #20
	movs r0, #9
	movs r1, #3
	bl 0x0200c63c
	movs r0, #9
	movs r1, #40
	bl 0x0200c5f4
	ldr r1, [pc, #896]
	movs r2, #0
	movs r0, #2
	bl 0x0200ce3c
	movs r0, #60
	bl 0x0200cd3c
	movs r1, #210
	movs r2, #212
	movs r0, #9
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200cdb4
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl 0x0200c624
	movs r0, #9
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #0
	movs r1, #3
	movs r2, #20
	bl 0x0200c63c
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #9
	bl 0x0200ce2c
	movs r0, #20
	bl 0x0200cd3c
	movs r0, #9
	movs r1, #20
	bl 0x0200c5f4
	movs r1, #1
	movs r0, #1
	bl 0x0200cdfc
	movs r0, #20
	bl 0x0200cd3c
	movs r0, #1
	movs r1, #20
	bl 0x0200c5f4
	movs r2, #20
	movs r0, #9
	movs r1, #3
	bl 0x0200c63c
	movs r0, #9
	movs r1, #30
	bl 0x0200c5f4
	movs r2, #20
	movs r0, #9
	movs r1, #14
	bl 0x0200c624
	movs r0, #9
	movs r1, #20
	bl 0x0200c5f4
	movs r1, #214
	movs r2, #204
	lsls r2, r2, #1
	movs r0, #14
	lsls r1, r1, #2
	bl 0x0200cdb4
	ldr r5, [pc, #744]
	movs r0, #9
	adds r1, r5, #0
	bl 0x0200cd8c
	movs r0, #14
	adds r1, r5, #0
	bl 0x0200cd8c
	movs r1, #198
	movs r0, #1
	lsls r1, r1, #2
	mov r2, r9
	bl 0x0200cdac
	movs r0, #2
	ldr r1, [pc, #716]
	ldr r2, [pc, #720]
	bl 0x0200cd84
	movs r1, #194
	movs r2, #216
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #2
	bl 0x0200cdac
	movs r0, #1
	bl 0x0200cdc4
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200ce2c
	movs r0, #2
	bl 0x0200cdc4
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #2
	bl 0x0200ce2c
	movs r0, #9
	bl 0x0200cd94
	movs r1, #9
	movs r2, #0
	movs r0, #0
	bl 0x0200ce04
	movs r0, #14
	bl 0x0200cd7c
	adds r6, r0, #0
	adds r2, r6, #0
	adds r2, #91
	movs r3, #1
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #24
	movs r1, #128
	str r3, [r6, #56]
	str r3, [r6, #60]
	str r3, [r6, #64]
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #1
	bl 0x0200ce3c
	movs r1, #1
	movs r0, #14
	bl 0x0200cdd4
	movs r0, #50
	bl 0x0200cd3c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200ce04
	movs r1, #202
	movs r2, #220
	movs r0, #1
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200cdb4
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200ce2c
	movs r0, #20
	bl 0x0200cd3c
	movs r0, #9
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #0
	movs r1, #1
	movs r2, #50
	bl 0x0200c624
	movs r0, #0
	movs r1, #9
	movs r2, #0
	bl 0x0200ce04
	movs r0, #1
	movs r1, #9
	movs r2, #20
	bl 0x0200c60c
	movs r0, #9
	movs r1, #3
	movs r2, #20
	bl 0x0200c63c
	movs r1, #186
	movs r2, #204
	movs r0, #9
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200cdac
	movs r1, #186
	movs r2, #204
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #14
	bl 0x0200cdac
	movs r0, #9
	bl 0x0200cdc4
	movs r0, #14
	bl 0x0200cdc4
	movs r0, #30
	bl 0x0200cd3c
	movs r1, #198
	movs r2, #204
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200cdb4
	movs r0, #0
	movs r1, #1
	movs r2, #30
	bl 0x0200c624
	ldr r1, [pc, #424]
	movs r2, #0
	movs r0, #2
	bl 0x0200ce3c
	movs r0, #50
	bl 0x0200cd3c
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x0200ce04
	movs r2, #20
	movs r0, #1
	movs r1, #2
	bl 0x0200c60c
	movs r1, #1
	movs r0, #1
	bl 0x0200cdfc
	movs r0, #20
	bl 0x0200cd3c
	movs r0, #1
	movs r1, #40
	bl 0x0200c5f4
	movs r0, #2
	movs r1, #3
	movs r2, #30
	bl 0x0200c63c
	movs r2, #40
	movs r0, #0
	movs r1, #1
	bl 0x0200c624
	movs r0, #0
	movs r1, #4
	bl 0x0200cdd4
	movs r0, #1
	movs r1, #4
	movs r2, #30
	bl 0x0200c63c
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x0200ce04
	movs r2, #20
	movs r0, #1
	movs r1, #2
	bl 0x0200c60c
	movs r1, #1
	movs r0, #1
	bl 0x0200cdfc
	movs r0, #20
	bl 0x0200cd3c
	movs r0, #1
	movs r1, #40
	bl 0x0200c5f4
	movs r1, #2
	movs r0, #2
	bl 0x0200cdfc
	movs r0, #20
	bl 0x0200cd3c
	movs r0, #2
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #0
	movs r1, #3
	bl 0x0200cdd4
	movs r0, #1
	movs r1, #3
	movs r2, #20
	bl 0x0200c63c
	movs r2, #20
	movs r0, #2
	movs r1, #4
	bl 0x0200c63c
	movs r0, #2
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #0
	ldr r1, [pc, #220]
	movs r2, #0
	bl 0x0200ce3c
	ldr r1, [pc, #212]
	movs r2, #0
	movs r0, #1
	bl 0x0200ce3c
	movs r0, #60
	bl 0x0200cd3c
	movs r2, #20
	movs r0, #2
	movs r1, #3
	bl 0x0200c63c
	movs r0, #2
	movs r1, #20
	bl 0x0200c5f4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ce3c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #1
	bl 0x0200ce3c
	movs r0, #60
	bl 0x0200cd3c
	movs r2, #20
	movs r0, #2
	movs r1, #3
	bl 0x0200c63c
	movs r0, #2
	movs r1, #20
	bl 0x0200c5f4
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ce3c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #1
	bl 0x0200ce3c
	movs r0, #60
	bl 0x0200cd3c
	movs r2, #20
	movs r0, #2
	movs r1, #4
	bl 0x0200c63c
	movs r0, #2
	movs r1, #20
	bl 0x0200c5f4
	movs r2, #20
	movs r0, #1
	movs r1, #3
	bl 0x0200c63c
	movs r0, #1
	movs r1, #30
	bl 0x0200c5f4
	movs r1, #0
	movs r0, #2
	bl 0x0200ce1c
	movs r0, #0
	movs r1, #0
	bl 0x0200cd74
	cmp r0, #0
	beq 0x0200c334
	movs r0, #20
	bl 0x0200cd3c
	movs r1, #1
	movs r0, #1
	bl 0x0200cdfc
	movs r0, #20
	bl 0x0200cd3c
	movs r0, #1
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #2
	movs r1, #4
	movs r2, #20
	bl 0x0200c63c
	movs r0, #2
.L_02004318:
	movs r1, #20
	bl 0x0200c5f4
	b .L_02004318_0
	.2byte 0x0101
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0xd4c8
	.2byte 0x0200
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
.L_02004318_0:
	movs r0, #2
	movs r1, #3
	movs r2, #30
	bl 0x0200c63c
	movs r1, #200
	movs r2, #228
	movs r0, #2
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200cdb4
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x0200ce04
	movs r0, #1
	movs r1, #2
	movs r2, #0
	bl 0x0200ce04
	movs r1, #214
	movs r2, #228
	movs r0, #2
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200cdb4
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x0200ce04
	movs r0, #1
	movs r1, #2
	movs r2, #0
	bl 0x0200ce04
	movs r1, #214
	movs r2, #188
	movs r0, #2
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200cdb4
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x0200ce04
	movs r0, #1
	movs r1, #2
	movs r2, #0
	bl 0x0200ce04
	movs r2, #0
	movs r1, #0
	movs r0, #2
	bl 0x0200ce2c
	movs r0, #20
	bl 0x0200cd3c
	movs r0, #2
	movs r1, #2
	bl 0x0200cdfc
	movs r2, #65
	movs r1, #17
	movs r0, #2
	bl 0x0200ceb4
	adds r6, r0, #0
	movs r0, #60
	bl 0x0200cd3c
	ldr r5, [pc, #492]
	movs r1, #1
	adds r0, r5, #0
	bl 0x0200ccf4
	adds r0, r6, #0
	bl 0x0200ccbc
	movs r1, #2
	movs r0, #17
	bl 0x0200cdd4
	movs r0, #20
	bl 0x0200cd3c
	movs r0, #2
	movs r1, #3
	movs r2, #20
	bl 0x0200c63c
	movs r1, #214
	movs r2, #228
	movs r0, #2
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200cdb4
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x0200ce04
	movs r0, #1
	movs r1, #2
	movs r2, #0
	bl 0x0200ce04
	movs r1, #200
	movs r2, #228
	movs r0, #2
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200cdb4
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x0200ce04
	movs r0, #1
	movs r1, #2
	movs r2, #0
	bl 0x0200ce04
	movs r1, #194
	movs r2, #212
	movs r0, #2
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200cdb4
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x0200ce0c
	movs r0, #1
	movs r1, #2
	movs r2, #30
	bl 0x0200c60c
	adds r5, #1
	movs r2, #20
	movs r1, #3
	movs r0, #2
	bl 0x0200c63c
	adds r0, r5, #0
	bl 0x0200ce14
	movs r0, #2
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #0
	movs r1, #3
	bl 0x0200cdd4
	movs r2, #236
	lsls r2, r2, #1
	adds r5, r7, r2
	movs r3, #0
	ldrsh r6, [r5, r3]
	bl 0x0200af84
	cmp r0, #0
	beq .L_02004318_1
	ldr r0, [pc, #300]
	bl 0x0200ce14
	movs r0, #2
	movs r1, #0
	bl 0x0200ce24
	bl 0x0200af98
.L_02004318_1:
	movs r0, #2
	bl 0x0200cd64
	movs r0, #1
	movs r1, #3
	movs r2, #50
	strh r6, [r5]
	bl 0x0200c63c
	movs r1, #194
	movs r2, #204
	movs r0, #2
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200cdb4
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x0200ce04
	movs r0, #1
	movs r1, #2
	movs r2, #0
	bl 0x0200ce04
	movs r1, #186
	movs r2, #204
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #2
	bl 0x0200cdb4
	movs r0, #40
	bl 0x0200cd3c
	movs r2, #20
	movs r0, #0
	movs r1, #1
	bl 0x0200c624
	movs r1, #1
	movs r0, #1
	bl 0x0200cdfc
	movs r0, #20
	bl 0x0200cd3c
	movs r0, #1
	movs r1, #20
	bl 0x0200c5f4
	movs r0, #0
	movs r1, #3
	movs r2, #20
	bl 0x0200c63c
	movs r0, #1
	movs r1, #2
	bl 0x0200cdd4
	movs r0, #0
	bl 0x0200cd7c
	cmp r0, #0
	beq .L_02004318_2
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200cd9c
.L_02004318_2:
	movs r0, #1
	bl 0x0200cdc4
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x0200cdcc
	movs r0, #30
	bl 0x0200cd3c
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200cdcc
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200cdcc
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x0200cdcc
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl 0x0200cdcc
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200cdcc
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x0200cdcc
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x0200cdcc
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200cdcc
	ldr r3, [pc, #40]
	movs r1, #224
	ldr r3, [r3]
	lsls r1, r1, #1
	ldr r2, [pc, #36]
	adds r3, r3, r1
	str r2, [r3]
	bl 0x0200cd4c
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001324
	.4byte 0x0000132a
	.4byte 0x03001ebc
	.4byte 0x00000209
	.section .text.x0200c8c8,"ax",%progbits
	.p2align 2
	.global KuupuappuHeya_UpdateActorStops
	.thumb_func
KuupuappuHeya_UpdateActorStops:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #0
	sub	sp, #4
	bl 0x0200cec4
	ldr	r3, [pc, #576]
	ldr	r3, [r3, #0]
	movs	r1, #0
	mov	r8, r0
	movs	r0, #2
	mov	r9, r1
	mov	fp, r3
	bl 0x0200cec4
	adds	r7, r0, #0
	adds	r5, r7, #0
	adds	r5, #8
	adds	r0, r5, #0
	bl 0x0200c704
	mov	sl, r0
	cmp	r0, #0
	beq.n	.L_020049b8
	movs	r2, #128
	ldr	r3, [r7, #56]
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_020049b8
	mov	r1, r8
	ldr	r2, [r5, #0]
	ldr	r3, [r1, #8]
	subs	r6, r2, r3
	ldr	r2, [r7, #16]
	ldr	r3, [r1, #16]
	subs	r5, r2, r3
	movs	r2, #6
	ldrsh	r3, [r1, r2]
	movs	r1, #2
	add	r1, sp
	mov	r8, r1
	mov	r2, r8
	adds	r1, r6, #0
	strh	r3, [r2, #0]
	adds	r0, r5, #0
	bl 0x0200cc8c
	movs	r3, #206
	lsls	r3, r3, #1
	add	r3, fp
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	asrs	r6, r6, #16
	asrs	r5, r5, #16
	cmp	r3, #0
	ble.n	.L_02004976
	adds	r4, r6, #0
	muls	r4, r6
	adds	r1, r5, #0
	muls	r1, r5
	movs	r2, #200
	adds	r3, r4, r1
	lsls	r2, r2, #1
	cmp	r3, r2
	bgt.n	.L_0200497e
	mov	r3, r8
	ldrh	r2, [r3, #0]
	lsls	r3, r0, #16
	lsrs	r3, r3, #16
	subs	r2, r2, r3
	lsls	r2, r2, #16
	asrs	r0, r2, #16
	ldr	r2, [pc, #444]
	cmp	r0, r2
	ble.n	.L_0200497e
	movs	r3, #128
	lsls	r3, r3, #5
	cmp	r0, r3
	bge.n	.L_0200497e
	b.n	.L_0200498c
.L_02004976:
	adds	r4, r6, #0
	muls	r4, r6
	adds	r1, r5, #0
	muls	r1, r5
.L_0200497e:
	adds	r3, r4, r1
	cmp	r3, #64
	ble.n	.L_0200498c
	movs	r1, #6
	ldrsh	r3, [r7, r1]
	mov	r2, r8
.L_0200498a:
	strh	r3, [r2, #0]
.L_0200498c:
	mov	r0, sl
	mov	r1, r8
	bl 0x0200c754
	adds	r5, r0, #0
	bl 0x0200c840
	cmp	r0, #0
	bne.n	.L_020049b0
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl 0x0200c8a4
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200ccac
	b.n	.L_020049b8
.L_020049b0:
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200ccac
.L_020049b8:
	movs	r0, #24
	bl 0x0200cec4
	adds	r7, r0, #0
	adds	r0, #8
	bl 0x0200c704
	mov	sl, r0
	cmp	r0, #0
	beq.n	.L_02004a4c
	movs	r1, #128
	ldr	r3, [r7, #56]
	lsls	r1, r1, #24
	cmp	r3, r1
	bne.n	.L_02004a4c
	bl 0x0200cc84
	lsls	r0, r0, #1
	lsrs	r0, r0, #16
	lsls	r3, r0, #1
	adds	r3, r3, r0
	movs	r1, #208
	lsls	r1, r1, #24
	lsls	r3, r3, #29
	ldrh	r2, [r7, #6]
.L_020049ea:
	adds	r3, r3, r1
	mov	r6, sp
	lsrs	r3, r3, #16
	adds	r6, #2
	adds	r3, r3, r2
	strh	r3, [r6, #0]
	mov	r0, sl
	adds	r1, r6, #0
	bl 0x0200c754
	adds	r5, r0, #0
	bl 0x0200c840
	cmp	r0, #0
	beq.n	.L_02004a3c
	ldrh	r3, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
	strh	r3, [r6, #0]
	mov	r0, sl
	adds	r1, r6, #0
	bl 0x0200c754
	adds	r5, r0, #0
	bl 0x0200c840
	cmp	r0, #0
	bne.n	.L_02004a2e
	movs	r0, #24
	movs	r1, #2
	bl 0x0200ce44
	b.n	.L_02004a3c
.L_02004a2e:
	adds	r0, r7, #0
	movs	r1, #4
	bl 0x0200ccac
	movs	r3, #1
	mov	r9, r3
	b.n	.L_02004a4c
.L_02004a3c:
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl 0x0200c8a4
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200ccac
.L_02004a4c:
	movs	r0, #25
	bl 0x0200cec4
	adds	r7, r0, #0
	adds	r0, #8
	bl 0x0200c704
	mov	sl, r0
	cmp	r0, #0
	beq.n	.L_02004ae2
	movs	r1, #128
	ldr	r3, [r7, #56]
	lsls	r1, r1, #24
	cmp	r3, r1
	bne.n	.L_02004ae2
	bl 0x0200cc84
	lsls	r2, r0, #1
	adds	r2, r2, r0
	lsrs	r2, r2, #16
	lsls	r3, r2, #1
	adds	r3, r3, r2
	movs	r1, #208
	lsls	r1, r1, #24
	lsls	r3, r3, #28
	ldrh	r2, [r7, #6]
	adds	r3, r3, r1
	mov	r6, sp
	lsrs	r3, r3, #16
	adds	r6, #2
	adds	r3, r3, r2
	strh	r3, [r6, #0]
	mov	r0, sl
	adds	r1, r6, #0
	bl 0x0200c754
	adds	r5, r0, #0
	bl 0x0200c840
	cmp	r0, #0
	beq.n	.L_02004ad2
	ldrh	r3, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
	strh	r3, [r6, #0]
	mov	r0, sl
	adds	r1, r6, #0
	bl 0x0200c754
	adds	r5, r0, #0
	bl 0x0200c840
	cmp	r0, #0
	bne.n	.L_02004ac4
	movs	r0, #25
	movs	r1, #2
	bl 0x0200ce44
	b.n	.L_02004ad2
.L_02004ac4:
	adds	r0, r7, #0
	movs	r1, #4
	bl 0x0200ccac
	movs	r3, #2
	add	r9, r3
	b.n	.L_02004ae2
.L_02004ad2:
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl 0x0200c8a4
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200ccac
.L_02004ae2:
	mov	r1, r9
	cmp	r1, #0
	beq.n	.L_02004b08
	ldr	r2, [pc, #60]
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r2, #232
	lsls	r3, r3, #16
	lsls	r2, r2, #13
	cmp	r3, r2
	bls.n	.L_02004b0e
	movs	r2, #193
	mov	r3, r9
	lsls	r2, r2, #1
	adds	r3, #200
	add	r2, fp
	strh	r3, [r2, #0]
	b.n	.L_02004b0e
.L_02004b08:
	ldr	r3, [pc, #28]
	mov	r1, r9
	strh	r1, [r3, #0]
.L_02004b0e:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.4byte 0xfffff000
	.2byte 0xe4f8
	.2byte 0x0200
	.global Func_02004b2c
	.thumb_func
Func_02004b2c:
	.global FieldScene_RunScene383_02004b2c
	.thumb_func
FieldScene_RunScene383_02004b2c:
	push {lr}
	bl 0x0200cd44
	movs r0, #168
	movs r1, #1
	movs r2, #164
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #18
	bl 0x0200ce54
	movs r0, #0
	ldr r1, [pc, #260]
	ldr r2, [pc, #260]
	bl 0x0200cd84
	movs r0, #1
	ldr r1, [pc, #248]
	ldr r2, [pc, #252]
	bl 0x0200cd84
	movs r0, #2
	ldr r1, [pc, #240]
	ldr r2, [pc, #240]
	bl 0x0200cd84
	movs r2, #174
	movs r0, #0
	movs r1, #248
	lsls r2, r2, #2
	bl 0x0200cdb4
	movs r1, #248
	movs r2, #174
	movs r0, #1
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200cdcc
	movs r1, #248
	movs r2, #174
	movs r0, #2
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200cdcc
	movs r2, #174
	movs r0, #0
	movs r1, #200
	lsls r2, r2, #2
	bl 0x0200cdac
	movs r2, #178
	movs r0, #1
	movs r1, #248
.L_02004b9c:
	lsls r2, r2, #2
	bl 0x0200cdac
	movs r2, #174
	movs r1, #232
	lsls r2, r2, #2
	movs r0, #2
	bl 0x0200cdb4
	movs r0, #1
	bl 0x0200cdc4
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ce2c
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #2
	bl 0x0200ce2c
	movs r0, #0
	bl 0x0200cdc4
	movs r0, #1
	movs r1, #12
	bl 0x0200cdd4
	bl 0x02009e80
	movs r0, #192
	movs r1, #144
	movs r2, #144
	movs r3, #184
	lsls r3, r3, #18
	lsls r0, r0, #14
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x0200ce64
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200cd84
	movs r1, #192
	movs r2, #192
	movs r0, #2
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200cd84
	movs r1, #128
	movs r0, #24
	lsls r1, r1, #9
	ldr r2, [pc, #60]
	bl 0x0200cd84
	movs r1, #192
	movs r2, #192
	movs r0, #25
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl 0x0200cd84
	ldr r3, [pc, #44]
	ldr r2, [pc, #28]
	ldr r1, [pc, #44]
	strh r2, [r3]
	ldr r0, [pc, #44]
	bl 0x0200cc74
	ldr r0, [pc, #44]
	bl 0x0200cd24
	bl 0x0200cd4c
	movs r0, #9
	bl 0x0200cf24
	b .L_02004b9c_0
	.4byte 0x00000000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.4byte 0x00013333
	.4byte 0x0200e4f8
	.4byte 0x00000c94
	.4byte 0x0200c8c9
	.4byte 0x000001ff
.L_02004b9c_0:
	pop {r0}
	bx r0
	.section .rodata,"a",%progbits
	.global KuupuappuHeya_Stops
KuupuappuHeya_Stops:
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0x0000801e
	.4byte 0x0000ffff
	.4byte 0x00000006
	.4byte 0x00000002
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000008
	.4byte 0x00000003
	.4byte 0x00008001
	.4byte 0x0000401f
	.4byte 0x0000000a
	.4byte 0x00000004
	.4byte 0x00008002
	.4byte 0x0000ffff
	.4byte 0x0000000c
	.4byte 0x00000005
	.4byte 0x00008003
	.4byte 0x0000ffff
	.4byte 0x0000000e
	.4byte 0x00000006
	.4byte 0x00008004
	.4byte 0x0000ffff
	.4byte 0x00000010
	.4byte 0x00000007
	.4byte 0x00008005
	.4byte 0x00004022
	.4byte 0x00000012
	.4byte 0x00000008
	.4byte 0x00008006
	.4byte 0x0000ffff
	.4byte 0x00000014
	.4byte 0x00000009
	.4byte 0x00008007
	.4byte 0x0000ffff
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00008008
	.4byte 0x0000ffff
	.4byte 0x00000018
	.4byte 0x0000400b
	.4byte 0x00008009
	.4byte 0x0000ffff
	.4byte 0x00000218
	.4byte 0x0000400c
	.4byte 0x0000c00a
	.4byte 0x0000ffff
	.4byte 0x00000418
	.4byte 0x0000400d
	.4byte 0x0000c00b
	.4byte 0x0000ffff
	.4byte 0x00000618
	.4byte 0x0000400e
	.4byte 0x0000c00c
	.4byte 0x0000ffff
	.4byte 0x00000818
	.4byte 0x0000800f
	.4byte 0x0000c00d
	.4byte 0x0000ffff
	.4byte 0x00000816
	.4byte 0x00008010
	.4byte 0x0000000e
	.4byte 0x0000ffff
	.4byte 0x00000814
	.4byte 0x00008011
	.4byte 0x0000000f
	.4byte 0x0000ffff
	.4byte 0x00000812
	.4byte 0x00008012
	.4byte 0x00000010
	.4byte 0x0000ffff
	.4byte 0x00000810
	.4byte 0x00008013
	.4byte 0x00000011
	.4byte 0x0000c024
	.4byte 0x0000080e
	.4byte 0x00008014
	.4byte 0x00000012
	.4byte 0x0000ffff
	.4byte 0x0000080c
	.4byte 0x00008015
	.4byte 0x00000013
	.4byte 0x0000ffff
	.4byte 0x0000080a
	.4byte 0x00008016
	.4byte 0x00000014
	.4byte 0x0000ffff
	.4byte 0x00000808
	.4byte 0x00008017
	.4byte 0x00000015
	.4byte 0x0000c021
	.4byte 0x00000806
	.4byte 0x00008018
	.4byte 0x00000016
	.4byte 0x0000ffff
	.4byte 0x00000804
	.4byte 0x00008019
	.4byte 0x00000017
	.4byte 0x0000ffff
	.4byte 0x00000802
	.4byte 0x0000801a
	.4byte 0x00000018
	.4byte 0x0000ffff
	.4byte 0x00000800
	.4byte 0x0000c01b
	.4byte 0x00000019
	.4byte 0x0000ffff
	.4byte 0x00000600
	.4byte 0x0000c01c
	.4byte 0x0000401a
	.4byte 0x0000ffff
	.4byte 0x00000400
	.4byte 0x0000c01d
	.4byte 0x0000401b
	.4byte 0x0000ffff
	.4byte 0x00000200
	.4byte 0x0000001e
	.4byte 0x0000401c
	.4byte 0x0000ffff
	.4byte 0x00000102
	.4byte 0x00000000
	.4byte 0x0000801d
	.4byte 0x0000ffff
	.4byte 0x00000208
	.4byte 0x00004020
	.4byte 0x0000c002
	.4byte 0x0000ffff
	.4byte 0x00000408
	.4byte 0x00004021
	.4byte 0x0000c01f
	.4byte 0x0000ffff
	.4byte 0x00000608
	.4byte 0x00004016
	.4byte 0x0000c020
	.4byte 0x0000ffff
	.4byte 0x00000210
	.4byte 0x00004023
	.4byte 0x0000c006
	.4byte 0x0000ffff
	.4byte 0x00000410
	.4byte 0x00004024
	.4byte 0x0000c022
	.4byte 0x0000ffff
	.4byte 0x00000610
	.4byte 0x00004012
	.4byte 0x0000c023
	.4byte 0x0000ffff
	.global KuupuappuHeya_ActionTable
KuupuappuHeya_ActionTable:
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffff00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffff00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x000000b4
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03200000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x0000001c
	.4byte 0x00000006
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptA
KuupuappuHeya_PairScriptA:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptB
KuupuappuHeya_PairScriptB:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x02800000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptC
KuupuappuHeya_PairScriptC:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptD
KuupuappuHeya_PairScriptD:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptE
KuupuappuHeya_PairScriptE:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptF
KuupuappuHeya_PairScriptF:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptG
KuupuappuHeya_PairScriptG:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x02800000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptH
KuupuappuHeya_PairScriptH:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptI
KuupuappuHeya_PairScriptI:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptJ
KuupuappuHeya_PairScriptJ:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptK
KuupuappuHeya_PairScriptK:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptL
KuupuappuHeya_PairScriptL:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptM
KuupuappuHeya_PairScriptM:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptN
KuupuappuHeya_PairScriptN:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptO
KuupuappuHeya_PairScriptO:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptP
KuupuappuHeya_PairScriptP:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptQ
KuupuappuHeya_PairScriptQ:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptR
KuupuappuHeya_PairScriptR:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_Scripts
KuupuappuHeya_Scripts:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000000c0
	.4byte 0xc00000dc
	.4byte 0x00200000
	.4byte 0x01100000
	.4byte 0x000000f0
	.4byte 0xffff0006
	.4byte 0x000001b0
	.4byte 0xc00000fe
	.4byte 0x01200000
	.4byte 0x02100000
	.4byte 0x00000110
	.4byte 0xffff0007
	.4byte 0x000002d0
	.4byte 0xc000030e
	.4byte 0x01b00000
	.4byte 0x03200240
	.4byte 0x00000320
	.4byte 0xffff0008
	.4byte 0x00000270
	.4byte 0xc00000ec
	.4byte 0x02300000
	.4byte 0x03500020
	.4byte 0x00000100
	.4byte 0xffff0009
	.4byte 0x00000090
	.4byte 0xc00001ec
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000200
	.4byte 0xffff000a
	.4byte 0x000001a0
	.4byte 0xc000020e
	.4byte 0x01400000
	.4byte 0x02500120
	.4byte 0x00000220
	.4byte 0xffff000b
	.4byte 0x000002e8
	.4byte 0x40000280
	.4byte 0x01b00000
	.4byte 0x03200240
	.4byte 0x00000320
	.4byte 0xffff000c
	.4byte 0x00000166
	.4byte 0x40000278
	.4byte 0x00200000
	.4byte 0x01900240
	.4byte 0x00000308
	.4byte 0xffff000d
	.4byte 0x00000288
	.4byte 0x400001a8
	.4byte 0x02300000
	.4byte 0x03200140
	.4byte 0x000001e0
	.4byte 0xffff000e
	.4byte 0x000002c0
	.4byte 0x80000198
	.4byte 0x02300000
	.4byte 0x03200140
	.4byte 0x000001e0
	.4byte 0xffff000f
	.4byte 0x00000300
	.4byte 0x00000198
	.4byte 0x02c80000
	.4byte 0x03b80120
	.4byte 0x000001f0
	.4byte 0xffff0010
	.4byte 0x00000318
	.4byte 0xc00001b0
	.4byte 0x02c80000
	.4byte 0x03b80120
	.4byte 0x000001f0
	.4byte 0xffff0011
	.4byte 0x00000318
	.4byte 0xc00001b0
	.4byte 0x02c80000
	.4byte 0x03b80120
	.4byte 0x000001f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global KuupuappuHeya_Regions
KuupuappuHeya_Regions:
	.4byte 0x0021027f
	.4byte 0x02910197
	.4byte 0x01a90033
	.4byte 0x000dffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global KuupuappuHeya_Messages
KuupuappuHeya_Messages:
	.4byte 0x00000015
	.4byte 0x00505014
	.4byte 0x00606014
	.4byte 0x00707014
	.4byte 0x00808014
	.4byte 0x00909014
	.4byte 0x00a0a014
	.4byte 0x00b0b015
	.4byte 0x00c0c015
	.4byte 0x00d0d014
	.4byte 0x00e0e015
	.4byte 0x00f0f015
	.4byte 0x0100c009
	.4byte 0x000001ff
	.global KuupuappuHeya_SceneTableA
KuupuappuHeya_SceneTableA:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0000c000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00004000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00013000
	.4byte 0x0000006a
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00008000
	.4byte 0x00000065
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x0000006b
	.4byte 0x00000002
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0x00000067
	.4byte 0x00000002
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00004000
	.4byte 0x0000006f
	.4byte 0x00000001
	.4byte 0x02f00000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00004000
	.4byte 0x0000006b
	.4byte 0x00000001
	.4byte 0x01f00000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00010000
	.4byte 0x00000066
	.4byte 0x00000001
	.4byte 0x02000000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00018000
	.4byte 0x00000075
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01c30000
	.4byte 0x00004000
	.4byte 0x00000073
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x0000c000
	.4byte 0x0000006a
	.4byte 0x00000002
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x0000c000
	.4byte 0x0000007c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00014000
	.4byte 0x0000007d
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00014000
	.4byte 0x00000076
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00034000
	.4byte 0x00000065
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00018000
	.4byte 0x00000080
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00014000
	.4byte 0x0854003f
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00030000
	.4byte 0x08540040
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00004000
	.4byte 0x000000df
	.4byte 0x00000001
	.4byte 0x02b40000
	.4byte 0x00000000
	.4byte 0x019b0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global KuupuappuHeya_SceneTableB
KuupuappuHeya_SceneTableB:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0000c000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00005000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001b000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00004000
	.4byte 0xffff003f
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00030000
	.4byte 0xffff0040
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00004000
	.4byte 0xffff0040
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00004000
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00004000
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00004000
	.4byte 0xffff00fb
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0xffff00dd
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x0000000a
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte 0x02009991
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0x1856000a
	.4byte 0x02009a4d
	.4byte 0x00000002
	.4byte 0x1300000b
	.4byte 0x0200a565
	.4byte 0x00000002
	.4byte 0x1300000c
	.4byte 0x0200a565
	.4byte 0x00000002
	.4byte 0x1300000d
	.4byte 0x0200a565
	.4byte 0x00000002
	.4byte 0x1300000e
	.4byte 0x0200a565
	.4byte 0x00000002
	.4byte 0x1300000f
	.4byte 0x0200a565
	.4byte 0x00000002
	.4byte 0x13000010
	.4byte 0x0200a565
	.4byte 0x00000002
	.4byte 0x13000011
	.4byte 0x0200a565
	.4byte 0x00000002
	.4byte 0x13000012
	.4byte 0x0200a565
	.4byte 0x00000002
	.4byte 0x13000014
	.4byte 0x02009ba1
	.4byte 0x00000002
	.4byte 0x18520015
	.4byte 0x02009e65
	.4byte 0x00000006
	.4byte 0xffff00c9
	.4byte 0x02009f51
	.4byte 0x00000006
	.4byte 0xffff00ca
	.4byte 0x02009f51
	.4byte 0x00000006
	.4byte 0xffff00cb
	.4byte 0x02009f51
	.4byte 0x00000006
	.4byte 0xffff00fa
	.4byte 0x020082a5
	.4byte 0x00000002
	.4byte 0xffff001a
	.4byte 0x020099a5
	.4byte 0x00000202
	.4byte 0xffff0028
	.4byte 0x02008081
	.4byte 0x00000000
	.4byte 0x12500002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x02008b49
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001242
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020084bd
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001246
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008401
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020084f5
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000124d
	.4byte 0x00000000
	.4byte 0x0856000e
	.4byte 0x00001252
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000127a
	.4byte 0x00000000
	.4byte 0x0856000f
	.4byte 0x02008429
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0000127b
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0200852d
	.4byte 0x00000000
	.4byte 0x08560011
	.4byte 0x00001251
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001279
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0200859d
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x0200813d
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x0200819d
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008455
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x0000128c
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x020081fd
	.4byte 0x00000000
	.4byte 0x12500018
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x12500019
	.4byte 0x00000000
	.4byte 0x00008d15
	.4byte 0x12500018
	.4byte 0x00000000
	.4byte 0x00008d15
	.4byte 0x12500019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x08530018
	.4byte 0x00001290
	.4byte 0x00000000
	.4byte 0x08530019
	.4byte 0x00001291
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x020085bd
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x02008691
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001244
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x02008a15
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000124a
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x02008a4d
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x02008a85
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000124f
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000127e
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000127f
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x02008abd
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000127d
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x02008af5
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001281
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001283
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001287
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001292
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001293
	.4byte 0x00008d15
	.4byte 0xffff0418
	.4byte 0x020085bd
	.4byte 0x00008d15
	.4byte 0xffff0419
	.4byte 0x02008691
	.4byte 0x00008c15
	.4byte 0x0859001a
	.4byte 0x020080fd
	.4byte 0x00000003
	.4byte 0xffff0029
	.4byte 0x020082c1
	.4byte 0x00000023
	.4byte 0x0f4b0064
	.4byte 0x00200007
	.4byte 0x00000033
	.4byte 0x0f4c0065
	.4byte 0x00200004
	.4byte 0x00000033
	.4byte 0x0f4d0066
	.4byte 0x001000e3
	.4byte 0x000000d3
	.4byte 0x0f4e0067
	.4byte 0x001000c3
	.4byte 0x0000c4f3
	.4byte 0xffff00c8
	.4byte 0x004029d1
	.4byte 0x000000f3
	.4byte 0xffff00c9
	.4byte 0x004029d2
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff001b
	.4byte 0x020099e5
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02009349
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x020082e9
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x020082e9
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x020082e9
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000012c4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0xffff0029
	.4byte 0x0200825d
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0xffff001a
	.4byte 0x020099a5
	.4byte 0x00000002
	.4byte 0xffff001b
	.4byte 0x020099e5
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001352
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020084bd
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020087cd
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020087ed
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020084f5
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000135d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0200880d
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0000136b
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0200882d
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001367
	.4byte 0x00000000
	.4byte 0x02500012
	.4byte 0x0200891d
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x0200813d
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x0200819d
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008455
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x0000137a
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x020081fd
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001354
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x02008a15
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000135a
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x02008a4d
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x02008a85
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000135f
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000136e
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000136f
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x02008abd
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000136d
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x02008af5
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001371
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001373
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001375
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001380
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001381
	.4byte 0x00000023
	.4byte 0x0f4b0064
	.4byte 0x00200007
	.4byte 0x00000033
	.4byte 0x0f4c0065
	.4byte 0x00200004
	.4byte 0x00000033
	.4byte 0x0f4d0066
	.4byte 0x001000e3
	.4byte 0x000000d3
	.4byte 0x0f4e0067
	.4byte 0x001000c3
	.4byte 0x0000c4f3
	.4byte 0xffff00c8
	.4byte 0x004029d1
	.4byte 0x000000f3
	.4byte 0xffff00c9
	.4byte 0x004029d2
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global KuupuappuHeya_StepActions
KuupuappuHeya_StepActions:
	.4byte 0x0200d5b0
	.4byte 0x0200d5d8
	.4byte 0x0200d718
	.4byte 0x0200d6c8
	.4byte 0x0200d650
	.4byte 0x0200d560
	.global KuupuappuHeya_IdleActions
KuupuappuHeya_IdleActions:
	.4byte 0x0200d678
	.4byte 0x0200d538
	.4byte 0x0200d5b0
	.4byte 0x0200d600
	.4byte 0x0200d6f0
	.4byte 0x0200d6c8
	.4byte 0x0200d7cc
	.4byte 0x0200d894
	.4byte 0x0200d858
	.4byte 0x0200d7a4
	.4byte 0x0200d830
	.4byte 0x0200d768
	.4byte 0x0200d808
	.4byte 0x0200d8bc
