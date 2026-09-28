.syntax unified
	.thumb
	.section .text.x020080e4,"ax",%progbits
	.balign 4
	.global Func_020000e4
	.thumb_func
Func_020000e4:
	push {lr}
	ldr r3, [pc, #128]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #120]
	cmp r2, r3
	bne .L_020000e4_0
	ldr r0, [pc, #116]
	b .L_020000e4_1
.L_020000e4_0:
	ldr r3, [pc, #116]
	cmp r2, r3
	bne .L_020000e4_2
	ldr r0, [pc, #116]
	b .L_020000e4_1
.L_020000e4_2:
	ldr r3, [pc, #116]
	cmp r2, r3
	bne .L_020000e4_3
	ldr r0, [pc, #112]
	b .L_020000e4_1
.L_020000e4_3:
	ldr r3, [pc, #112]
	cmp r2, r3
	bne .L_020000e4_4
	ldr r0, [pc, #112]
	b .L_020000e4_1
.L_020000e4_4:
	ldr r3, [pc, #112]
	cmp r2, r3
	bne .L_020000e4_5
	ldr r0, [pc, #108]
	b .L_020000e4_1
.L_020000e4_5:
	ldr r3, [pc, #108]
	cmp r2, r3
	bne .L_020000e4_6
	ldr r0, [pc, #108]
	b .L_020000e4_1
.L_020000e4_6:
	ldr r3, [pc, #108]
	cmp r2, r3
	bne .L_020000e4_7
	ldr r0, [pc, #104]
	b .L_020000e4_1
.L_020000e4_7:
	ldr r3, [pc, #104]
	cmp r2, r3
	bne .L_020000e4_8
	ldr r0, [pc, #104]
	b .L_020000e4_1
.L_020000e4_8:
	ldr r3, [pc, #104]
	cmp r2, r3
	bne .L_020000e4_9
	ldr r0, [pc, #100]
	b .L_020000e4_1
.L_020000e4_9:
	ldr r3, [pc, #100]
	cmp r2, r3
	bne .L_020000e4_10
	ldr r0, [pc, #100]
	b .L_020000e4_1
.L_020000e4_10:
	ldr r3, [pc, #100]
	cmp r2, r3
	bne .L_020000e4_11
	ldr r0, [pc, #96]
	b .L_020000e4_1
.L_020000e4_11:
	ldr r0, [pc, #96]
.L_020000e4_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000004d
	.4byte 0x0200c194
	.4byte 0x0000004e
	.4byte 0x0200c20c
	.4byte 0x0000004f
	.4byte 0x0200c26c
	.4byte 0x00000050
	.4byte 0x0200c314
	.4byte 0x00000051
	.4byte 0x0200c3ec
	.4byte 0x00000052
	.4byte 0x0200c464
	.4byte 0x00000053
	.4byte 0x0200c524
	.4byte 0x00000054
	.4byte 0x0200c59c
	.4byte 0x00000055
	.4byte 0x0200c644
	.4byte 0x00000056
	.4byte 0x0200c704
	.4byte 0x00000057
	.4byte 0x0200c77c
	.4byte 0x0200c164
	.global Func_020001c8
	.thumb_func
Func_020001c8:
	push {lr}
	ldr r3, [pc, #36]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_020001c8_0
	ldr r0, [pc, #24]
	b .L_020001c8_1
.L_020001c8_0:
	ldr r3, [pc, #24]
	movs r0, #0
	cmp r2, r3
	bne .L_020001c8_1
	ldr r0, [pc, #20]
.L_020001c8_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000055
	.4byte 0x0200c80c
	.4byte 0x00000056
	.4byte 0x0200c83c
	.section .text.x0200820c,"ax",%progbits
	.balign 4
	.global Func_0200020c
	.thumb_func
Func_0200020c:
	push {lr}
	ldr r3, [pc, #108]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #100]
	cmp r2, r3
	bne .L_0200020c_0
	ldr r0, [pc, #96]
	b .L_0200020c_1
.L_0200020c_0:
	ldr r3, [pc, #96]
	cmp r2, r3
	bne .L_0200020c_2
	ldr r0, [pc, #96]
	b .L_0200020c_1
.L_0200020c_2:
	ldr r3, [pc, #96]
	cmp r2, r3
	bne .L_0200020c_3
	ldr r0, [pc, #92]
	b .L_0200020c_1
.L_0200020c_3:
	ldr r3, [pc, #92]
	cmp r2, r3
	bne .L_0200020c_4
	ldr r0, [pc, #92]
	b .L_0200020c_1
.L_0200020c_4:
	ldr r3, [pc, #92]
	cmp r2, r3
	bne .L_0200020c_5
	ldr r0, [pc, #88]
	b .L_0200020c_1
.L_0200020c_5:
	ldr r3, [pc, #88]
	cmp r2, r3
	bne .L_0200020c_6
	ldr r0, [pc, #88]
	b .L_0200020c_1
.L_0200020c_6:
	ldr r3, [pc, #88]
	cmp r2, r3
	bne .L_0200020c_7
	ldr r0, [pc, #84]
	b .L_0200020c_1
.L_0200020c_7:
	ldr r3, [pc, #84]
	cmp r2, r3
	bne .L_0200020c_8
	ldr r0, [pc, #84]
	b .L_0200020c_1
.L_0200020c_8:
	ldr r3, [pc, #84]
	cmp r2, r3
	bne .L_0200020c_9
	ldr r0, [pc, #80]
	b .L_0200020c_1
.L_0200020c_9:
	ldr r0, [pc, #80]
.L_0200020c_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000004d
	.4byte 0x0200c940
	.4byte 0x0000004f
	.4byte 0x0200c9a0
	.4byte 0x00000051
	.4byte 0x0200ca00
	.4byte 0x00000052
	.4byte 0x0200ca60
	.4byte 0x00000053
	.4byte 0x0200caa8
	.4byte 0x00000054
	.4byte 0x0200cb68
	.4byte 0x00000055
	.4byte 0x0200cb98
	.4byte 0x00000056
	.4byte 0x0200cc40
	.4byte 0x00000057
	.4byte 0x0200ccd0
	.4byte 0x0200c928
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	movs	r0, #0
	bl 0x0200bb98
	adds	r6, r0, #0
	movs	r0, #8
	bl 0x0200bb98
	adds	r5, r0, #0
	bl 0x0200bd00
	bl 0x0200bb70
	movs	r1, #22
	movs	r0, #0
	bl 0x0200bbf0
	movs	r0, #10
	bl 0x0200bb68
	movs	r0, #152
	bl 0x0200bd20
	ldr	r1, [pc, #128]
	ldr	r2, [pc, #132]
	movs	r0, #0
	bl 0x0200bba0
	ldr	r1, [r5, #12]
	ldr	r2, [r6, #12]
	subs	r3, r1, r2
	cmp	r3, #0
	bge.n	.L_02000312
	subs	r3, r2, r1
.L_02000312:
	asrs	r3, r3, #14
	movs	r2, #128
	lsls	r3, r3, #14
	lsls	r2, r2, #11
	adds	r3, r3, r2
	str	r3, [r6, #40]
	movs	r0, #0
	movs	r1, #7
	bl 0x0200bbf0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	adds	r0, r6, #0
	bl 0x0200baf8
	movs	r0, #10
	bl 0x0200ba78
	ldr	r1, [r6, #80]
	ldrb	r3, [r1, #9]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #9]
	movs	r0, #0
	bl 0x0200bbe0
	b.n	.L_02000350
.L_0200034a:
	movs	r0, #1
	bl 0x0200ba78
.L_02000350:
	ldr	r2, [r5, #12]
	ldr	r3, [r6, #12]
	asrs	r2, r2, #14
	asrs	r3, r3, #14
	cmp	r2, r3
	blt.n	.L_0200034a
	bl 0x0200bb78
	movs	r0, #159
	bl 0x0200bd20
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200b850
	movs	r0, #20
	bl 0x0200ba78
	bl 0x0200bd18
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x00033333
	.2byte 0x9999
	.2byte 0x0001
	.global Func_02000388
	.thumb_func
Func_02000388:
	push {lr}
	ldr r3, [pc, #28]
	ldr r2, [pc, #28]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	ldr r0, [pc, #24]
	movs r1, #99
	bl 0x0200bca8
	movs r0, #53
	movs r1, #2
	bl 0x0200bc98
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x0000004d
	.global Func_020003b4
	.thumb_func
Func_020003b4:
	push {lr}
	ldr r3, [pc, #28]
	ldr r2, [pc, #28]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	ldr r0, [pc, #24]
	movs r1, #99
	bl 0x0200bca8
	movs r0, #53
	movs r1, #2
	bl 0x0200bc98
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x0000004f
	.section .text.x02008d2c,"ax",%progbits
	.balign 4
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #10
	sub	sp, #4
	bl 0x0200bb98
	ldr	r3, [r0, #12]
	ldr	r2, [r0, #8]
	ldr	r6, [r0, #80]
	mov	r9, r2
	str	r3, [sp, #0]
	mov	sl, r0
	bl 0x0200bb70
	movs	r0, #141
	bl 0x0200bd20
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200bb38
	movs	r0, #10
	bl 0x0200bb68
	ldr	r0, [pc, #320]
	bl 0x0200bd20
	movs	r0, #1
	movs	r1, #1
	ldr	r2, [pc, #312]
	negs	r0, r0
	negs	r1, r1
	bl 0x0200bb38
	movs	r0, #20
	bl 0x0200bb68
	ldr	r2, [pc, #300]
	movs	r7, #0
	mov	r8, r2
.L_02000d8e:
	movs	r3, #128
	lsls	r3, r3, #12
	ldrh	r2, [r6, #30]
	adds	r7, r7, r3
	lsrs	r3, r7, #16
	adds	r3, r3, r2
	strh	r3, [r6, #30]
	movs	r2, #128
	ldrh	r0, [r6, #30]
	lsls	r2, r2, #7
	adds	r0, r0, r2
	bl 0x0200baa8
	adds	r5, r0, #0
	lsls	r3, r5, #4
	add	r3, r9
	mov	r2, sl
	str	r3, [r2, #8]
	ldrh	r1, [r6, #30]
	cmp	r1, r8
	bhi.n	.L_02000dc0
	movs	r0, #1
	bl 0x0200ba78
	b.n	.L_02000d8e
.L_02000dc0:
	movs	r3, #224
	lsls	r3, r3, #7
	movs	r7, #0
	mov	r8, r3
.L_02000dc8:
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r7, r7, r2
	lsrs	r3, r7, #16
	subs	r3, r1, r3
	strh	r3, [r6, #30]
	movs	r3, #128
	ldrh	r0, [r6, #30]
	lsls	r3, r3, #7
	adds	r0, r0, r3
	bl 0x0200baa8
	adds	r5, r0, #0
	lsls	r3, r5, #4
	add	r3, r9
	mov	r2, sl
	str	r3, [r2, #8]
	ldrh	r1, [r6, #30]
	cmp	r1, r8
	bls.n	.L_02000dfa
	movs	r0, #1
	bl 0x0200ba78
	ldrh	r1, [r6, #30]
	b.n	.L_02000dc8
.L_02000dfa:
	movs	r3, #128
	movs	r7, #128
	lsls	r3, r3, #8
	lsls	r7, r7, #12
	mov	fp, r3
.L_02000e04:
	lsrs	r2, r7, #19
	lsrs	r3, r7, #16
	adds	r3, r3, r2
	lsls	r3, r3, #16
	adds	r7, r3, #0
	lsrs	r2, r7, #16
	adds	r3, r2, r1
	strh	r3, [r6, #30]
	movs	r3, #128
	ldrh	r0, [r6, #30]
	lsls	r3, r3, #7
	adds	r0, r0, r3
	mov	r8, r2
	bl 0x0200baa8
	adds	r5, r0, #0
	ldrh	r0, [r6, #30]
	add	r0, fp
	bl 0x0200baa0
	lsls	r3, r5, #4
	add	r3, r9
	mov	r2, sl
	str	r3, [r2, #8]
	ldrh	r3, [r6, #30]
	cmp	r3, fp
	bls.n	.L_02000e44
	ldr	r2, [sp, #0]
	lsls	r3, r0, #3
	subs	r3, r2, r3
	mov	r2, sl
	str	r3, [r2, #12]
.L_02000e44:
	ldrh	r3, [r6, #30]
	ldr	r2, [pc, #116]
	add	r3, r8
	cmp	r3, r2
	bgt.n	.L_02000e58
	movs	r0, #1
	bl 0x0200ba78
	ldrh	r1, [r6, #30]
	b.n	.L_02000e04
.L_02000e58:
	movs	r0, #1
	bl 0x0200ba78
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r6, #30]
	movs	r0, #183
	bl 0x0200bd20
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200bb38
	movs	r0, #20
	bl 0x0200bb68
	ldr	r0, [pc, #44]
	bl 0x0200bd20
	movs	r0, #1
	movs	r1, #1
	negs	r1, r1
	ldr	r2, [pc, #36]
	negs	r0, r0
	bl 0x0200bb38
	movs	r0, #5
	bl 0x02008ec0
	bl 0x0200bb78
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00000121
	.4byte 0x0000e666
	.4byte 0x00008fff
	.2byte 0xbfff
	.2byte 0x0000
	.section .text.x02009398,"ax",%progbits
	.balign 4
	.global Func_02001398
	.thumb_func
Func_02001398:
	push {r5, r6, lr}
	bl 0x0200bb70
	ldr r5, [pc, #684]
	movs r1, #1
	adds r0, r5, #0
	bl 0x0200bb48
	ldr r0, [pc, #676]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_02001398_0
	b 0x02009640
.L_02001398_0:
	ldr r0, [pc, #668]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_02001398_1
	b 0x02009640
.L_02001398_1:
	ldr r0, [pc, #660]
	bl 0x0200bb58
	movs r0, #0
	ldr r1, [pc, #656]
	ldr r2, [pc, #660]
	bl 0x0200bba0
	movs r0, #0
	ldr r1, [pc, #656]
	movs r2, #140
	bl 0x0200bbd8
	movs r1, #195
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #140
	bl 0x0200bbd8
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bc50
	movs r0, #0
	bl 0x0200bb98
	cmp r0, #0
	beq .L_02001398_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200bbe8
.L_02001398_2:
	movs r0, #1
	ldr r1, [pc, #592]
	ldr r2, [pc, #596]
	bl 0x0200bba0
	movs r1, #200
	movs r0, #1
	lsls r1, r1, #2
	movs r2, #140
	bl 0x0200bbd8
	movs r1, #192
	movs r2, #20
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200bc50
	adds r0, r5, #1
	bl 0x0200bc30
	movs r1, #4
	movs r0, #1
	bl 0x0200bbf0
	movs r0, #20
	bl 0x0200bb68
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x0200bc48
	movs r0, #1
	movs r1, #6
	movs r2, #0
	bl 0x0200bc00
	ldr r1, [pc, #532]
	ldr r2, [pc, #520]
	movs r0, #1
	bl 0x0200bba0
	movs r0, #1
	bl 0x0200bb98
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
.L_02001466:
	adds r3, r5, #0
	ands r3, r2
	movs r1, #198
	strb r3, [r0]
	lsls r1, r1, #2
	movs r2, #110
	movs r0, #1
	bl 0x0200bbd8
	movs r0, #1
	bl 0x0200bb68
	movs r0, #1
	bl 0x0200bb98
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	strb r3, [r0]
	movs r0, #161
	bl 0x0200bd20
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200bb38
	movs r0, #1
	bl 0x0200bb98
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #198
	ands r5, r3
	lsls r1, r1, #2
	movs r2, #120
	strb r5, [r0]
	movs r0, #1
	bl 0x0200bbd8
	movs r0, #1
	bl 0x0200bb68
	movs r0, #1
	bl 0x0200bb98
	adds r0, #90
	ldrb r3, [r0]
	orrs r6, r3
	strb r6, [r0]
	movs r1, #1
	movs r0, #1
	negs r1, r1
	ldr r2, [pc, #400]
	negs r0, r0
	bl 0x0200bb38
	movs r0, #80
	bl 0x0200bb68
	movs r0, #141
	bl 0x0200bd20
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200bb38
	movs r0, #40
	bl 0x0200bb68
	movs r0, #0
	ldr r1, [pc, #360]
	movs r2, #0
	bl 0x0200bc60
	movs r0, #1
	ldr r1, [pc, #352]
	movs r2, #60
	bl 0x0200bc60
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bc50
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200bc50
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200bc50
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bc50
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bc50
	movs r0, #1
	movs r1, #0
	movs r2, #40
	bl 0x0200bc50
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bc50
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bc50
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200bc60
	movs r1, #128
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200bc50
	movs r0, #1
	movs r1, #2
	bl 0x0200bc08
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x0200bc48
	movs r1, #160
	movs r2, #160
	lsls r2, r2, #9
	movs r0, #1
	lsls r1, r1, #10
	bl 0x0200bba0
	movs r0, #1
	movs r1, #5
	bl 0x0200bbf0
	movs r1, #199
	movs r0, #1
	lsls r1, r1, #2
.L_020015b6:
	movs r2, #138
	bl 0x0200bbc8
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200bc50
	movs r1, #201
	movs r0, #1
	lsls r1, r1, #2
	movs r2, #140
	bl 0x0200bbc8
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bc50
	movs r1, #201
	movs r0, #1
	lsls r1, r1, #2
	movs r2, #166
	bl 0x0200bbc8
	movs r1, #191
	movs r0, #1
	lsls r1, r1, #2
	movs r2, #166
	bl 0x0200bbc8
	movs r1, #191
	movs r0, #1
	lsls r1, r1, #2
	movs r2, #198
	bl 0x0200bbc8
	movs r0, #1
	ldr r1, [pc, #108]
	movs r2, #198
	bl 0x0200bbc8
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200bc60
	movs r2, #246
	movs r0, #1
	ldr r1, [pc, #84]
	bl 0x0200bbc8
	movs r0, #1
	movs r1, #1
	bl 0x0200bbf0
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200bbe8
	movs r0, #40
	bl 0x0200bb68
	movs r0, #10
	bl 0x02008ec0
	bl 0x0200bb78
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x1953
	.2byte 0x0000
	.2byte 0x0908
	.2byte 0x0000
	.2byte 0x0f14
	.2byte 0x0000
	.2byte 0x0205
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x0316
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0001
	.2byte 0xe666
	.2byte 0x0000
	.2byte 0x0101
	.2byte 0x0000
	.4byte 0x00000312
	.global Func_02001678
	.thumb_func
Func_02001678:
	push {lr}
	ldr r3, [pc, #140]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #132]
	cmp r2, r3
	bne .L_02001678_0
	ldr r0, [pc, #128]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_02001678_1
	ldr r0, [pc, #124]
	b .L_02001678_2
.L_02001678_1:
	ldr r0, [pc, #124]
	b .L_02001678_2
.L_02001678_0:
	ldr r3, [pc, #124]
	cmp r2, r3
	bne .L_02001678_3
	ldr r0, [pc, #120]
	b .L_02001678_2
.L_02001678_3:
	ldr r3, [pc, #120]
	cmp r2, r3
	bne .L_02001678_4
	ldr r0, [pc, #120]
	b .L_02001678_2
.L_02001678_4:
	ldr r3, [pc, #120]
	cmp r2, r3
	bne .L_02001678_5
	ldr r0, [pc, #116]
	b .L_02001678_2
.L_02001678_5:
	ldr r3, [pc, #116]
	cmp r2, r3
	bne .L_02001678_6
	ldr r0, [pc, #116]
	b .L_02001678_2
.L_02001678_6:
	ldr r3, [pc, #116]
	cmp r2, r3
	bne .L_02001678_7
	ldr r0, [pc, #112]
	b .L_02001678_2
.L_02001678_7:
	ldr r3, [pc, #112]
	cmp r2, r3
	bne .L_02001678_8
	ldr r0, [pc, #112]
	b .L_02001678_2
.L_02001678_8:
	ldr r3, [pc, #112]
	cmp r2, r3
	bne .L_02001678_9
	ldr r0, [pc, #108]
	b .L_02001678_2
.L_02001678_9:
	ldr r3, [pc, #108]
	cmp r2, r3
	bne .L_02001678_10
	ldr r0, [pc, #108]
	b .L_02001678_2
.L_02001678_10:
	ldr r3, [pc, #108]
	cmp r2, r3
	bne .L_02001678_11
	ldr r0, [pc, #104]
	b .L_02001678_2
.L_02001678_11:
	ldr r3, [pc, #104]
	cmp r2, r3
	bne .L_02001678_12
	ldr r0, [pc, #104]
	b .L_02001678_2
.L_02001678_12:
	ldr r0, [pc, #104]
.L_02001678_2:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x0000004d
	.4byte 0x000008fd
	.4byte 0x0200cd6c
	.4byte 0x0200cd24
	.4byte 0x0000004e
	.4byte 0x0200cd9c
	.4byte 0x0000004f
	.4byte 0x0200cdc0
	.4byte 0x00000050
	.4byte 0x0200ce5c
	.4byte 0x00000051
	.4byte 0x0200cebc
	.4byte 0x00000052
	.4byte 0x0200cf34
	.4byte 0x00000053
	.4byte 0x0200cfb8
	.4byte 0x00000054
	.4byte 0x0200d06c
	.4byte 0x00000055
	.4byte 0x0200d0cc
	.4byte 0x00000056
	.4byte 0x0200d12c
	.4byte 0x00000057
	.4byte 0x0200d150
	.4byte 0x0200cd18
	.section .text.x02009d0c,"ax",%progbits
	.balign 4
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #10
	bl 0x0200bb98
	adds	r7, r0, #0
	bl 0x0200bb70
	movs	r0, #10
	bl 0x0200bbb0
	ldr	r0, [pc, #172]
	ldr	r1, [pc, #176]
	bl 0x0200bc78
	movs	r1, #128
	movs	r2, #216
	movs	r3, #1
	lsls	r2, r2, #16
	lsls	r1, r1, #15
	ldr	r0, [pc, #164]
	bl 0x0200bc80
	bl 0x0200bc88
	movs	r0, #147
	bl 0x0200bd20
	movs	r1, #2
	movs	r0, #10
	bl 0x0200bc10
	movs	r0, #40
.L_02001d58:
	bl 0x0200bb68
	movs	r1, #192
	movs	r0, #10
	lsls	r1, r1, #6
	movs	r2, #20
	bl 0x0200bc50
	movs	r1, #160
	movs	r0, #10
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200bc50
	movs	r1, #128
	movs	r2, #40
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x0200bc50
	ldr	r0, [pc, #100]
	ldr	r1, [pc, #104]
	bl 0x0200bc78
	movs	r0, #128
	movs	r1, #128
	movs	r2, #202
	lsls	r0, r0, #16
	lsls	r1, r1, #15
	lsls	r2, r2, #16
	movs	r3, #1
	bl 0x0200bc80
	ldr	r3, [pc, #60]
	mov	sl, r3
	movs	r3, #102
	adds	r3, r3, r7
	movs	r2, #100
	movs	r5, #0
	adds	r2, r2, r7
	mov	fp, r3
	ldr	r6, [pc, #68]
	ldr	r3, [pc, #68]
	str	r5, [r7, #104]
	mov	r8, r2
	strh	r5, [r2, #0]
	mov	r2, fp
	strh	r5, [r2, #0]
	movs	r0, #10
	str	r3, [r7, #72]
	ldr	r1, [pc, #56]
	ldr	r2, [pc, #60]
	str	r6, [r7, #108]
	bl 0x0200bba0
	movs	r0, #10
	movs	r1, #212
	movs	r2, #200
	bl 0x0200bbc8
	movs	r1, #103
	movs	r0, #10
	movs	r2, #200
	b.n	.L_02001e00
	.4byte 0x00000000
	.4byte 0x00026666
	.4byte 0x00004ccc
	.4byte 0x01170000
	.4byte 0x0000cccc
	.4byte 0x00001999
	.4byte 0x02009771
	.4byte 0x00006666
	.4byte 0x00013333
	.2byte 0x9999
	.2byte 0x0000
.L_02001e00:
	bl 0x0200bbc8
	movs	r3, #91
	adds	r3, r3, r7
	mov	r2, sl
	str	r5, [r7, #108]
	movs	r0, #10
	strb	r2, [r3, #0]
	mov	r9, r3
	bl 0x0200bb68
	movs	r1, #1
	movs	r0, #10
	bl 0x0200bbf0
	movs	r0, #229
	bl 0x0200bd20
	movs	r0, #128
	movs	r2, #128
	movs	r1, #0
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200bb38
	movs	r0, #4
	bl 0x0200bb68
	movs	r0, #1
	movs	r1, #1
	negs	r1, r1
	ldr	r2, [pc, #456]
	negs	r0, r0
	bl 0x0200bb38
	movs	r0, #20
	bl 0x0200bb68
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #40
	movs	r0, #10
	bl 0x0200bc50
	movs	r0, #10
	bl 0x0200bb98
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	ldr	r2, [pc, #416]
	ldr	r1, [pc, #420]
	movs	r0, #10
	bl 0x0200bba0
	movs	r0, #10
	bl 0x0200bb98
	movs	r1, #0
	bl 0x0200bb30
	movs	r0, #153
	bl 0x0200bd20
	movs	r0, #10
	bl 0x0200bb98
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r0, #40]
	movs	r1, #3
	movs	r0, #10
	bl 0x0200bbf0
	movs	r2, #214
	movs	r0, #10
	movs	r1, #86
	bl 0x0200bbc8
	movs	r1, #1
	movs	r0, #10
	bl 0x0200bbf0
	movs	r0, #10
	bl 0x0200bb98
	movs	r1, #1
	bl 0x0200bb30
	movs	r0, #10
	bl 0x0200bb68
	movs	r0, #229
	bl 0x0200bd20
	movs	r0, #128
	movs	r2, #128
	movs	r1, #0
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200bb38
	movs	r0, #8
	bl 0x0200bb68
	movs	r0, #1
	movs	r1, #1
	negs	r1, r1
	ldr	r2, [pc, #296]
	negs	r0, r0
	bl 0x0200bb38
	movs	r0, #40
	bl 0x0200bb68
	movs	r0, #10
	bl 0x0200bb98
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	movs	r1, #192
	strb	r3, [r0, #0]
	lsls	r1, r1, #6
	movs	r0, #10
	movs	r2, #20
	bl 0x0200bc50
	movs	r0, #10
	movs	r1, #0
	movs	r2, #40
	bl 0x0200bc50
	mov	r3, r8
	mov	r2, fp
	str	r5, [r7, #104]
	movs	r0, #10
	strh	r5, [r3, #0]
	ldr	r1, [pc, #244]
	strh	r5, [r2, #0]
	ldr	r2, [pc, #236]
	str	r6, [r7, #108]
	bl 0x0200bba0
	movs	r0, #10
	movs	r1, #120
	movs	r2, #215
	bl 0x0200bbc8
	mov	r3, sl
	mov	r2, r9
	movs	r1, #1
	str	r5, [r7, #108]
	movs	r0, #10
	strb	r3, [r2, #0]
	bl 0x0200bbf0
	movs	r0, #16
	bl 0x0200bb68
	movs	r0, #229
	bl 0x0200bd20
	movs	r0, #128
	movs	r2, #128
	movs	r1, #0
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200bb38
	movs	r0, #4
	bl 0x0200bb68
	movs	r0, #1
	movs	r1, #1
	negs	r1, r1
	ldr	r2, [pc, #160]
	negs	r0, r0
	bl 0x0200bb38
	movs	r0, #40
	bl 0x0200bb68
	movs	r2, #10
	movs	r1, #0
	movs	r0, #10
	bl 0x0200bc50
	movs	r0, #147
	bl 0x0200bd20
	movs	r1, #2
	movs	r0, #10
	bl 0x0200bc10
	movs	r0, #80
	bl 0x0200bb68
	movs	r0, #10
	movs	r1, #3
	bl 0x0200bbf0
	movs	r0, #130
	movs	r2, #168
	lsls	r2, r2, #16
	movs	r3, #0
	lsls	r0, r0, #16
	movs	r1, #0
	bl 0x0200abb0
	movs	r0, #60
	bl 0x0200bb68
	ldr	r2, [pc, #96]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_02001fcc
	adds	r6, r2, #0
.L_02001fba:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200bb68
	cmp	r5, #59
	bhi.n	.L_02001fcc
	ldr	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_02001fba
.L_02001fcc:
	movs	r0, #0
	bl 0x0200bb98
	ldr	r1, [pc, #56]
	adds	r7, r0, #0
	ldr	r0, [pc, #64]
	bl 0x0200bc78
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	ldr	r2, [r7, #16]
	movs	r3, #1
	bl 0x0200bc80
	bl 0x0200bc88
	ldr	r0, [pc, #44]
	bl 0x0200bb58
	bl 0x0200bb78
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x0000e666
	.4byte 0x00009999
	.4byte 0x00013333
	.4byte 0x03001c94
	.4byte 0x0004cccc
	.2byte 0x0905
	.2byte 0x0000
	.global Func_02002020
	.thumb_func
Func_02002020:
	push {lr}
	ldr r3, [pc, #128]
	movs r2, #224
	ldr r1, [r3]
	movs r3, #129
	lsls r2, r2, #1
	lsls r3, r3, #2
	str r3, [r1, r2]
	ldr r3, [pc, #116]
	ldrsh r2, [r3, r2]
	ldr r3, [pc, #116]
	cmp r2, r3
	bne .L_02002020_0
	bl 0x0200a0d0
	b .L_02002020_1
.L_02002020_0:
	ldr r3, [pc, #108]
	cmp r2, r3
	bne .L_02002020_2
	bl 0x0200a310
	b .L_02002020_1
.L_02002020_2:
	ldr r3, [pc, #100]
	cmp r2, r3
	bne .L_02002020_3
	bl 0x0200a428
	b .L_02002020_1
.L_02002020_3:
	ldr r3, [pc, #92]
	cmp r2, r3
	bne .L_02002020_4
	bl 0x0200a490
	b .L_02002020_1
.L_02002020_4:
	ldr r3, [pc, #84]
	cmp r2, r3
	bne .L_02002020_5
	bl 0x0200a5c0
	b .L_02002020_1
.L_02002020_5:
	ldr r3, [pc, #76]
	cmp r2, r3
	bne .L_02002020_6
	bl 0x0200a6c0
	b .L_02002020_1
.L_02002020_6:
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_02002020_7
	bl 0x0200a804
	b .L_02002020_1
.L_02002020_7:
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_02002020_8
	bl 0x0200a934
	b .L_02002020_1
.L_02002020_8:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02002020_1
	bl 0x0200a9dc
.L_02002020_1:
	movs r0, #0
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000004d
	.4byte 0x0000004f
	.4byte 0x00000050
	.4byte 0x00000051
	.4byte 0x00000052
	.4byte 0x00000053
	.4byte 0x00000055
	.4byte 0x00000056
	.4byte 0x00000057
	.section .text.x0200af10,"ax",%progbits
	.balign 4
	.global Func_02002f10
	.thumb_func
Func_02002f10:
	push {lr}
	bl 0x0200bb70
	ldr r0, [pc, #192]
	ldr r1, [pc, #192]
	bl 0x0200bc78
	movs r0, #164
	movs r1, #1
	movs r2, #174
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #15
	bl 0x0200bc80
	movs r0, #0
	ldr r1, [pc, #164]
	ldr r2, [pc, #168]
	bl 0x0200bba0
	movs r1, #164
	movs r2, #116
	lsls r1, r1, #1
	movs r0, #0
	bl 0x0200bbd8
	movs r0, #148
	bl 0x0200bd20
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #144]
	bl 0x0200ba80
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl 0x0200bb38
	movs r0, #8
	ldr r1, [pc, #124]
	ldr r2, [pc, #128]
	bl 0x0200bba0
	ldr r2, [pc, #120]
	movs r0, #9
	ldr r1, [pc, #112]
	bl 0x0200bba0
	movs r0, #8
	movs r1, #2
	bl 0x0200bbf0
	movs r1, #164
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #104
	bl 0x0200bbc0
	movs r1, #164
	lsls r1, r1, #1
	movs r2, #108
	movs r0, #9
	bl 0x0200bbc0
	movs r0, #60
	bl 0x0200bb68
	movs r1, #128
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200bc60
	movs r1, #2
	movs r0, #0
	bl 0x0200bc08
	movs r0, #8
	bl 0x0200bbe0
	ldr r3, [pc, #52]
	ldr r2, [pc, #52]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	ldr r0, [pc, #48]
	movs r1, #99
	bl 0x0200bca8
	movs r0, #53
	movs r1, #3
	bl 0x0200bc98
	pop {r0}
	bx r0
	.4byte 0x00009999
	.4byte 0x00001333
	.4byte 0x00004ccc
	.4byte 0x0200aeed
	.4byte 0x00001999
	.4byte 0x00000ccc
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x00000056
	.section .text.x0200b410,"ax",%progbits
	.balign 4
	.global Func_02003410
	.thumb_func
Func_02003410:
	ldr r3, [pc, #12]
	movs r1, #191
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r2, r3, r1
	ldr r3, [pc, #8]
	strh r3, [r2]
	bx lr
	.4byte 0x03001ebc
	.4byte 0x00001018
	.section .text.x0200b5a8,"ax",%progbits
	.balign 4
	.2byte 0x4770
	.2byte 0x0000
	.global ArutinYama_TurnRollingObjectA
	.thumb_func
ArutinYama_TurnRollingObjectA:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r0, #0
	ldrh	r3, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r6, r3, r2
	movs	r3, #192
	lsls	r3, r3, #8
	sub	sp, #12
	ands	r6, r3
	ldr	r3, [r7, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	movs	r0, #192
	adds	r1, r6, #0
	lsls	r0, r0, #13
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl 0x0200bab0
	ldr	r3, [r5, #0]
	movs	r1, #128
	lsls	r1, r1, #12
	ldr	r2, [pc, #120]
	adds	r3, r3, r1
	ands	r3, r2
	mov	r9, r3
	ldr	r3, [r5, #8]
	adds	r3, r3, r1
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r0, r7, #0
	movs	r1, #5
	mov	sl, r3
	adds	r6, r6, r2
	bl 0x0200bad0
	movs	r0, #184
	bl 0x0200bd20
	movs	r3, #15
	mov	r8, r3
.L_02003610:
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r6, r6, r2
	movs	r0, #192
	mov	r2, sl
	mov	r3, r9
	str	r2, [r5, #8]
	lsls	r0, r0, #13
	adds	r1, r6, #0
	adds	r2, r5, #0
	str	r3, [r5, #0]
	bl 0x0200bab0
	ldr	r3, [r5, #0]
	str	r3, [r7, #8]
	movs	r2, #128
	ldr	r3, [r5, #8]
	lsls	r2, r2, #7
	str	r3, [r7, #16]
	adds	r3, r6, r2
	strh	r3, [r7, #6]
	movs	r0, #1
	bl 0x0200ba78
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L_02003610
.L_0200364c:
	movs	r0, #233
	bl 0x0200bd20
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfff0
	.global ArutinYama_TurnRollingObjectB
	.thumb_func
ArutinYama_TurnRollingObjectB:
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r0, #0
	ldrh	r3, [r7, #6]
	ldr	r2, [pc, #160]
	adds	r6, r3, r2
	movs	r3, #192
	lsls	r3, r3, #8
	sub	sp, #12
	ands	r6, r3
	ldr	r3, [r7, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	movs	r0, #192
	adds	r1, r6, #0
	lsls	r0, r0, #13
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl 0x0200bab0
	ldr	r3, [r5, #0]
	movs	r1, #128
	lsls	r1, r1, #12
	ldr	r2, [pc, #120]
	adds	r3, r3, r1
	ands	r3, r2
	mov	r9, r3
	ldr	r3, [r5, #8]
	adds	r3, r3, r1
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r0, r7, #0
	movs	r1, #6
	mov	sl, r3
	adds	r6, r6, r2
	bl 0x0200bad0
	movs	r0, #184
	bl 0x0200bd20
	movs	r3, #15
	mov	r8, r3
.L_020036ca:
	ldr	r2, [pc, #84]
	movs	r0, #192
	adds	r6, r6, r2
	mov	r2, sl
	mov	r3, r9
	str	r2, [r5, #8]
	lsls	r0, r0, #13
	adds	r1, r6, #0
	adds	r2, r5, #0
	str	r3, [r5, #0]
	bl 0x0200bab0
	ldr	r3, [r5, #0]
	str	r3, [r7, #8]
	ldr	r2, [pc, #48]
	ldr	r3, [r5, #8]
	str	r3, [r7, #16]
	adds	r3, r6, r2
	strh	r3, [r7, #6]
	movs	r0, #1
	bl 0x0200ba78
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L_020036ca
	movs	r0, #233
	bl 0x0200bd20
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0xffffc000
	.4byte 0xfff00000
	.2byte 0xfc00
	.2byte 0xffff
	.section .rodata,"a",%progbits
	.global ArutinYama_PaletteScript
ArutinYama_PaletteScript:
	.4byte 0x20021003
	.4byte 0x20024001
	.4byte 0x000000ff
	.global ArutinYama_ActorScript
ArutinYama_ActorScript:
	.4byte 0x00000022
	.4byte 0x02008041
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global ArutinYama_EarlyActorScript
ArutinYama_EarlyActorScript:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000222
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000222
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000006c
	.4byte 0x00000000
	.4byte 0x00000010
	.global ArutinYama_LogRideScript
ArutinYama_LogRideScript:
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x00dc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x01460000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x01460000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x01e00000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000010
	.global ArutinYama_LeaderRideScript
ArutinYama_LeaderRideScript:
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x008c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x00a60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02fc0000
	.4byte 0x00000000
	.4byte 0x00a60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02fc0000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02fc0000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02fc0000
	.4byte 0x00000000
	.4byte 0x011a0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x030c0000
	.4byte 0x00000000
	.4byte 0x012a0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x012a0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x01460000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01460000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01660000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x01660000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02fa0000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02fa0000
	.4byte 0x00000000
	.4byte 0x01b60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x01b60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x031a0000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03560000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000400
	.4byte 0x0000000c
	.4byte 0x00000018
	.4byte 0xc0010000
	.4byte 0x00000010
	.global ArutinYama_SparkScript
ArutinYama_SparkScript:
	.4byte 0x00000022
	.4byte 0x020080bd
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
	.4byte 0x0000001b
	.global ArutinYama_GeraldScript
ArutinYama_GeraldScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x013a0000
	.4byte 0x00000000
	.4byte 0x007e0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000010
	.global ArutinYama_IvanScript
ArutinYama_IvanScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x015a0000
	.4byte 0x00000000
	.4byte 0x007e0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000010
	.global ArutinYama_MiaScript
ArutinYama_MiaScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x008a0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000010
	.global ArutinYama_CelebrateScript
ArutinYama_CelebrateScript:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000080
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffc00
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
	.global ArutinYama_PartyScript
ArutinYama_PartyScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00760000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
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
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000001c8
	.4byte 0xc0000208
	.4byte 0x00000000
	.4byte 0x01fa0038
	.4byte 0x00000212
	.4byte 0xffff0062
	.4byte 0x000001c8
	.4byte 0xc00001e8
	.4byte 0x00000000
	.4byte 0x01fa0038
	.4byte 0x00000212
	.4byte 0xffff0063
	.4byte 0x00000068
	.4byte 0x400000e8
	.4byte 0x00000000
	.4byte 0x01fa0038
	.4byte 0x00000212
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0xc00001d0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000038
	.4byte 0x40000048
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
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000001e8
	.4byte 0xc0000230
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000298
	.4byte 0x4000017c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000388
	.4byte 0xc00000d6
	.4byte 0x034a0000
	.4byte 0x03f80028
	.4byte 0x000000f8
	.4byte 0xffff0062
	.4byte 0x000001e8
	.4byte 0xc0000230
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0063
	.4byte 0x00000140
	.4byte 0x400001d8
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
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0xc00001f2
	.4byte 0x00180000
	.4byte 0x02880000
	.4byte 0x00000230
	.4byte 0xffff0002
	.4byte 0x00000077
	.4byte 0x4000006c
	.4byte 0x00180000
	.4byte 0x02880000
	.4byte 0x00000230
	.4byte 0xffff0003
	.4byte 0x00000237
	.4byte 0x40000069
	.4byte 0x00180000
	.4byte 0x02880000
	.4byte 0x00000230
	.4byte 0xffff0004
	.4byte 0x00000377
	.4byte 0xc000031e
	.4byte 0x02c80000
	.4byte 0x03c00238
	.4byte 0x0000035c
	.4byte 0xffff0005
	.4byte 0x00000317
	.4byte 0x400002a7
	.4byte 0x02c80000
	.4byte 0x03c00230
	.4byte 0x0000035c
	.4byte 0xffff0006
	.4byte 0x00000058
	.4byte 0xc0000348
	.4byte 0x00200000
	.4byte 0x011002c0
	.4byte 0x00000360
	.4byte 0xffff0007
	.4byte 0x000000d8
	.4byte 0xc0000348
	.4byte 0x00200000
	.4byte 0x011002c0
	.4byte 0x00000360
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000249
	.4byte 0xc00001f5
	.4byte 0x00000000
	.4byte 0x031e0000
	.4byte 0x00000212
	.4byte 0xffff0062
	.4byte 0x00000249
	.4byte 0xc00001f5
	.4byte 0x00000000
	.4byte 0x031e0000
	.4byte 0x00000212
	.4byte 0xffff0063
	.4byte 0x000002eb
	.4byte 0x40000102
	.4byte 0x00000000
	.4byte 0x031e0000
	.4byte 0x00000212
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000238
	.4byte 0xc000015c
	.4byte 0x00000000
	.4byte 0x031e0000
	.4byte 0x00000212
	.4byte 0xffff0002
	.4byte 0x00000127
	.4byte 0x4000005f
	.4byte 0x00000000
	.4byte 0x031e0000
	.4byte 0x00000212
	.4byte 0xffff0003
	.4byte 0x000001ba
	.4byte 0xc0000306
	.4byte 0x00000000
	.4byte 0x02000212
	.4byte 0x000003b4
	.4byte 0xffff0004
	.4byte 0x00000048
	.4byte 0xc0000366
	.4byte 0x00000000
	.4byte 0x02000212
	.4byte 0x000003b4
	.4byte 0xffff0005
	.4byte 0x00000396
	.4byte 0x400002c8
	.4byte 0x02080000
	.4byte 0x03e0023a
	.4byte 0x00000312
	.4byte 0xffff0006
	.4byte 0x00000267
	.4byte 0x400002ac
	.4byte 0x02080000
	.4byte 0x03e0023a
	.4byte 0x00000312
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000048
	.4byte 0xc00001f6
	.4byte 0x00000000
	.4byte 0x03340000
	.4byte 0x00000238
	.4byte 0xffff0002
	.4byte 0x00000098
	.4byte 0x400000ae
	.4byte 0x00000000
	.4byte 0x03340000
	.4byte 0x00000238
	.4byte 0xffff0003
	.4byte 0x00000108
	.4byte 0xc000037a
	.4byte 0x00900000
	.4byte 0x018002d0
	.4byte 0x0000038c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000188
	.4byte 0x40000074
	.4byte 0x00000000
	.4byte 0x033e0000
	.4byte 0x00000234
	.4byte 0xffff0002
	.4byte 0x00000028
	.4byte 0xc0000074
	.4byte 0x00000000
	.4byte 0x033e0000
	.4byte 0x00000234
	.4byte 0xffff0003
	.4byte 0x00000208
	.4byte 0x40000268
	.4byte 0x00f40000
	.4byte 0x04000234
	.4byte 0x00000400
	.4byte 0xffff0004
	.4byte 0x00000198
	.4byte 0xc00003ca
	.4byte 0x00f40000
	.4byte 0x04000234
	.4byte 0x00000400
	.4byte 0xffff0005
	.4byte 0x000003b7
	.4byte 0x40000317
	.4byte 0x00f40000
	.4byte 0x04000234
	.4byte 0x00000400
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000169
	.4byte 0xc0000135
	.4byte 0x00000000
	.4byte 0x020c0000
	.4byte 0x00000330
	.4byte 0xffff0002
	.4byte 0x00000059
	.4byte 0xc00002ed
	.4byte 0x00000000
	.4byte 0x020c0000
	.4byte 0x00000330
	.4byte 0xffff0003
	.4byte 0x000002d8
	.4byte 0xc00002cd
	.4byte 0x026a0000
	.4byte 0x03de0000
	.4byte 0x00000356
	.4byte 0xffff0004
	.4byte 0x000003a7
	.4byte 0x400001ed
	.4byte 0x026a0000
	.4byte 0x03de0000
	.4byte 0x00000356
	.4byte 0xffff0005
	.4byte 0x00000317
	.4byte 0x40000236
	.4byte 0x026a0000
	.4byte 0x03de0000
	.4byte 0x00000356
	.4byte 0xffff0006
	.4byte 0x00100318
	.4byte 0x40000241
	.4byte 0x026a0000
	.4byte 0x03e60000
	.4byte 0x00000356
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00040058
	.4byte 0x400001a8
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x0000020c
	.4byte 0xffff0062
	.4byte 0x00040058
	.4byte 0x400001a8
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x0000020c
	.4byte 0xffff0063
	.4byte 0x00000148
	.4byte 0xc0000076
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x0000020c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000198
	.4byte 0xc0000205
	.4byte 0x00000000
	.4byte 0x021c0000
	.4byte 0x0000024e
	.4byte 0xffff0002
	.4byte 0x000000c6
	.4byte 0x40000150
	.4byte 0x00000000
	.4byte 0x021c0000
	.4byte 0x0000024e
	.4byte 0xffff0003
	.4byte 0x00000238
	.4byte 0xc0000088
	.4byte 0x01e00000
	.4byte 0x02ac000f
	.4byte 0x000000b4
	.4byte 0xffff0004
	.4byte 0x00000049
	.4byte 0xc00000be
	.4byte 0x00000000
	.4byte 0x021c0000
	.4byte 0x0000024e
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xff960316
	.4byte 0x031a023f
	.4byte 0x0243ff9a
	.4byte 0x0006ffff
	.4byte 0xff940314
	.4byte 0x031c0243
	.4byte 0x024bff9c
	.4byte 0x0006ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00260054
	.4byte 0x005c01a4
	.4byte 0x01ac002e
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global ArutinYama_StatueTable
ArutinYama_StatueTable:
	.4byte 0x0000004d
	.4byte 0x0010a04b
	.4byte 0x0000004e
	.4byte 0x00103050
	.4byte 0x0020104f
	.4byte 0x0000004f
	.4byte 0x0010204e
	.4byte 0x0020304f
	.4byte 0x0030204f
	.4byte 0x00000050
	.4byte 0x0010b04b
	.4byte 0x00204050
	.4byte 0x0030104e
	.4byte 0x00402050
	.4byte 0x00501052
	.4byte 0x0060404a
	.4byte 0x0070504a
	.4byte 0x00000051
	.4byte 0x00106052
	.4byte 0x00000052
	.4byte 0x00105050
	.4byte 0x00203052
	.4byte 0x00302052
	.4byte 0x00405052
	.4byte 0x00504052
	.4byte 0x00601051
	.4byte 0x00000053
	.4byte 0x00101054
	.4byte 0x00201055
	.4byte 0x0030d04b
	.4byte 0x00000054
	.4byte 0x00101053
	.4byte 0x00205055
	.4byte 0x00301057
	.4byte 0x0040c04b
	.4byte 0x00503055
	.4byte 0x00000055
	.4byte 0x00102053
	.4byte 0x00204055
	.4byte 0x00305054
	.4byte 0x00402055
	.4byte 0x00502054
	.4byte 0x00601056
	.4byte 0x00000056
	.4byte 0x00106055
	.4byte 0x00000057
	.4byte 0x00103054
	.4byte 0x00203057
	.4byte 0x00302057
	.4byte 0x00434002
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00020000
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00028000
	.4byte 0xffff0127
	.4byte 0x00000001
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00020000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x02f70000
	.4byte 0x00000000
	.4byte 0x011d0000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00020000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00020000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff0127
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00024000
	.4byte 0x0047005b
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0127
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x027c0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x03100000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00024000
	.4byte 0xffff0120
	.4byte 0x00000007
	.4byte 0x03100000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x03060000
	.4byte 0x00000000
	.4byte 0x004c0000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x03060000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff0112
	.4byte 0x00000007
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00024000
	.4byte 0xffff0120
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0127
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01480000
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
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x0904000a
	.4byte 0x02009839
	.4byte 0x00000002
	.4byte 0x0905000b
	.4byte 0x02009d0d
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008389
	.4byte 0x00008d15
	.4byte 0xffff040a
	.4byte 0x02008389
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global ArutinYama_OpenedAreaScript
ArutinYama_OpenedAreaScript:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00001815
	.4byte 0x02000008
	.4byte 0x0200847d
	.4byte 0x00001815
	.4byte 0x02010009
	.4byte 0x020084cd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
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
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020082cd
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020083b5
	.4byte 0x00008d15
	.4byte 0xffff0409
	.4byte 0x020083b5
	.4byte 0x00008f15
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0xffff0053
	.4byte 0x02008a65
	.4byte 0x00000013
	.4byte 0x0f760064
	.4byte 0x001000c0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000013
	.4byte 0x0f050064
	.4byte 0x001000b4
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
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00001815
	.4byte 0x02000009
	.4byte 0x0200855d
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020083e1
	.4byte 0x00008d15
	.4byte 0xffff040a
	.4byte 0x020083e1
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020082cd
	.4byte 0x00000003
	.4byte 0xffff0032
	.4byte 0x02008b0d
	.4byte 0x00002413
	.4byte 0x0f770064
	.4byte 0x0010007b
	.4byte 0x00000413
	.4byte 0x0f770064
	.4byte 0x0010007b
	.4byte 0x0000e413
	.4byte 0x0f770064
	.4byte 0x0010007b
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
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00001815
	.4byte 0x02000009
	.4byte 0x020085ad
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020082cd
	.4byte 0x00000003
	.4byte 0xffff0032
	.4byte 0x02008b3d
	.4byte 0x00000013
	.4byte 0x0ef20064
	.4byte 0x00500002
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
	.4byte 0x00000013
	.4byte 0x0f070065
	.4byte 0x001000ba
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008031
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020082cd
	.4byte 0x00000003
	.4byte 0xffff0032
	.4byte 0x02008bd9
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x02008c75
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008c89
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x0200840d
	.4byte 0x00000602
	.4byte 0xffff000c
	.4byte 0x02008c9d
	.4byte 0x00001815
	.4byte 0x02000009
	.4byte 0x0200850d
	.4byte 0x00001815
	.4byte 0x0201000b
	.4byte 0x020085fd
	.4byte 0x00001815
	.4byte 0x0204000c
	.4byte 0x02008651
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
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000013
	.4byte 0x0f780064
	.4byte 0x001000e5
	.4byte 0x00008f15
	.4byte 0xffff0008
	.4byte 0x00000000
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
	.2byte 0x0001
.L_020050f2:
	movs	r0, r0
	movs	r4, r0
	.2byte 0xffff
	.2byte 0x0004
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r5, r0
	.2byte 0xffff
	.2byte 0x0005
	movs	r0, r0
	movs	r3, r0
	movs	r0, r0
	movs	r4, r1
	.2byte 0xffff
	.2byte 0x9399
	lsls	r0, r0, #8
	ldrh	r5, [r2, #48]
	movs	r0, r0
	movs	r3, r1
	lsrs	r0, r1, #4
	ldrh	r5, [r5, #40]
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r2, r0
	movs	r0, r0
	movs	r2, r1
	lsrs	r1, r1, #4
	add	r7, sp, #68
	lsls	r0, r0, #8
	movs	r3, r2
	movs	r0, r0
	lsls	r4, r4, #1
	lsrs	r0, r1, #28
	lsls	r3, r1, #3
	movs	r0, r2
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r1, r0
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r1, r0
.L_0200515e:
	movs	r0, r0
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0002
	movs	r0, r0
	movs	r1, r0
.L_0200516a:
	movs	r0, r0
	movs	r3, r0
	.2byte 0xffff
	.2byte 0x0003
.L_02005172:
	movs	r0, r0
.L_02005174:
	movs	r1, r0
	movs	r0, r0
	movs	r4, r0
.L_0200517a:
	.2byte 0xffff
	.2byte 0x0004
	movs	r0, r0
	ldrh	r5, [r2, #56]
.L_02005182:
	movs	r0, r0
.L_02005184:
	movs	r0, r1
	.2byte 0xffff
	.2byte 0x0000
	movs	r0, r0
	ldrh	r5, [r2, #32]
	movs	r0, r0
.L_02005190:
	movs	r1, r1
	.2byte 0xffff
	.2byte 0x0000
.L_02005196:
	movs	r0, r0
.L_02005198:
	movs	r3, r2
	movs	r0, r0
.L_0200519c:
	lsls	r4, r4, #1
.L_0200519e:
	lsrs	r1, r7, #29
.L_020051a0:
	lsls	r6, r6, #2
	movs	r0, r2
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_020051b0:
	.global ArutinYama_CueTicks
ArutinYama_CueTicks:
	.2byte 0x0000
.L_020051b2:
	.2byte 0x0000
	.global ArutinYama_RollRadii
ArutinYama_RollRadii:
	movs	r0, r0
	movs	r1, r0
	lsrs	r5, r2, #13
.L_020051ba:
	movs	r1, r0
.L_020051bc:
	asrs	r3, r5, #28
.L_020051be:
	movs	r1, r0
	movs	r3, #135
	movs	r1, r0
	adds	r0, #111
	movs	r1, r0
.L_020051c8:
	subs	r5, #234
	movs	r1, r0
	ldr	r3, [pc, #1012]
	movs	r1, r0
	ldrh	r0, [r6, r2]
.L_020051d2:
	movs	r1, r0
	ldr	r1, [r1, #32]
.L_020051d6:
	movs	r1, r0
	ldrb	r1, [r2, #8]
.L_020051da:
	movs	r1, r0
	ldrh	r6, [r1, #22]
	movs	r1, r0
.L_020051e0:
	ldr	r4, [sp, #292]
	movs	r1, r0
	add	r6, sp, #548
	movs	r1, r0
.L_020051e8:
	stmia	r1!, {r0, r3, r4, r7}
	movs	r1, r0
	bpl.n	.L_020050f2
	movs	r1, r0
	.2byte 0xea4b
	.2byte 0x0001
	movs	r0, r0
	movs	r2, r0
	asrs	r3, r5, #26
	movs	r2, r0
	cmp	r6, #86
	movs	r2, r0
	.2byte 0x470f
	movs	r2, r0
	str	r7, [r3, #12]
	movs	r2, r0
	ldrb	r4, [r2, #15]
.L_0200520a:
	movs	r2, r0
	str	r7, [sp, #1004]
	movs	r2, r0
	push	{r5, r6, lr}
	movs	r2, r0
.L_02005214:
	bmi.n	0x0200d23e
	movs	r2, r0
.L_02005218:
	.2byte 0xf422
	.2byte 0x0002
	asrs	r4, r3, #22
	movs	r3, r0
	subs	r0, #146
	movs	r3, r0
	ldrb	r3, [r2, r4]
	movs	r3, r0
	strh	r3, [r6, #24]
	movs	r3, r0
.L_0200522c:
	add	r3, sp, #12
	movs	r3, r0
	bmi.n	.L_0200515e
	movs	r3, r0
	movs	r0, r0
	movs	r4, r0
	.section .bss,"aw",%nobits
	.global ArutinYama_RiseTimer
ArutinYama_RiseTimer:
	.space 4
	.global ArutinYama_LeafMode
ArutinYama_LeafMode:
	.space 4
	.global ArutinYama_LeafOrigin
ArutinYama_LeafOrigin:
	.space 12
	.space 16
	.global ArutinYama_PaletteHold
ArutinYama_PaletteHold:
	.space 4
	.global ArutinYama_PaletteStep
ArutinYama_PaletteStep:
	.space 4
