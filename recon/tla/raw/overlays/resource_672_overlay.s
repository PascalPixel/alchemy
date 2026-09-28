.syntax unified
	.thumb
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa05c
	.2byte 0x0200
	.global Func_02000040
	.thumb_func
Func_02000040:
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_02000058
	ldr	r0, [pc, #12]
.L_02000058:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000078
	.2byte 0xa08c
	.2byte 0x0200
	.global Func_02000068
	.thumb_func
Func_02000068:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa0ac
	.2byte 0x0200
	.global Func_02000070
	.thumb_func
Func_02000070:
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_02000088
	ldr	r0, [pc, #24]
	b.n	.L_02000094
.L_02000088:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_02000092
	ldr	r0, [pc, #24]
	b.n	.L_02000094
.L_02000092:
	ldr	r0, [pc, #24]
.L_02000094:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000078
	.4byte 0x0200a0f8
	.4byte 0x0000007c
	.4byte 0x0200a1e8
	.2byte 0xa458
	.2byte 0x0200
	.global Func_020000b0
	.thumb_func
Func_020000b0:
	push	{lr}
	ldr	r3, [pc, #44]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_020000c8
	ldr	r0, [pc, #32]
	b.n	.L_020000de
.L_020000c8:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_020000d2
	ldr	r0, [pc, #32]
	b.n	.L_020000de
.L_020000d2:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_020000dc
	ldr	r0, [pc, #28]
	b.n	.L_020000de
.L_020000dc:
	ldr	r0, [pc, #28]
.L_020000de:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000078
	.4byte 0x0200a470
	.4byte 0x0000007a
	.4byte 0x0200a4a0
	.4byte 0x0000007c
	.4byte 0x0200a4dc
	.2byte 0xa530
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #8
	movs	r1, #2
	bl 0x02009e28
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #2
	bl 0x02009ec0
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #2
	adds	r0, #3
	bl 0x02009eb8
	movs	r0, #20
	bl 0x02009ec8
	movs	r0, #20
	bl 0x02009cf0
	ldr	r5, [pc, #64]
	ldr	r3, [pc, #60]
	movs	r0, #1
	strh	r3, [r5, #0]
	bl 0x02009cf0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #226
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r0, [r3, #0]
	movs	r1, #0
	adds	r2, r0, #1
	lsls	r0, r0, #16
	strh	r2, [r3, #0]
	asrs	r0, r0, #16
	movs	r2, #2
	bl 0x02009d88
	movs	r0, #128
	movs	r1, #2
	lsls	r0, r0, #9
	bl 0x02009eb8
	movs	r0, #20
	bl 0x02009ec8
	movs	r0, #20
	b.n	.L_02000174
	.2byte 0x0000
	.4byte 0x00007fff
	.2byte 0x021e
	.2byte 0x0500
.L_02000174:
	bl 0x02009cf0
	movs	r0, #8
	movs	r1, #1
	bl 0x02009e28
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	bl 0x02009da0
	movs	r0, #0
	bl 0x02009ee8
	movs	r0, #26
	bl 0x02009f10
	movs	r0, #212
	bl 0x02009f10
	movs	r0, #8
	movs	r1, #3
	bl 0x02009e28
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #2
	bl 0x02009ec0
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #2
	bl 0x02009eb8
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	bl 0x02009ec0
	movs	r1, #1
	ldr	r0, [pc, #488]
	bl 0x02009eb8
	movs	r0, #40
	bl 0x02009ec8
	movs	r0, #40
	bl 0x02009cf0
	movs	r0, #197
	bl 0x02009f10
	movs	r0, #1
	bl 0x02009cf0
	ldr	r3, [pc, #464]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02000222
	movs	r5, #0
.L_020001f0:
	cmp	r5, #20
	bne.n	.L_02000200
	movs	r1, #128
	movs	r0, #10
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x02009e88
.L_02000200:
	ldr	r6, [pc, #436]
	movs	r1, #128
	ldr	r2, [r6, #0]
	lsls	r1, r1, #8
	ldr	r3, [r2, #12]
	movs	r0, #1
	adds	r3, r3, r1
	str	r3, [r2, #12]
	adds	r5, #1
	bl 0x02009cf0
	cmp	r5, #79
	bls.n	.L_020001f0
	ldr	r3, [r6, #0]
	movs	r2, #4
	adds	r3, #85
	strb	r2, [r3, #0]
.L_02000222:
	movs	r0, #8
	movs	r1, #1
	bl 0x02009e28
	ldr	r3, [pc, #400]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r6, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x02009dc8
	movs	r1, #179
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	movs	r2, #108
	bl 0x02009df8
	movs	r1, #224
	movs	r2, #0
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	bl 0x02009e68
	ldr	r1, [r6, #0]
	movs	r0, #9
	bl 0x02009e20
	movs	r0, #1
	bl 0x02009cf0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #9
	adds	r1, #204
	adds	r2, #102
	bl 0x02009dc8
	movs	r1, #182
	movs	r0, #9
	lsls	r1, r1, #1
	movs	r2, #124
	bl 0x02009df8
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x02009e68
	ldr	r0, [pc, #296]
	bl 0x02009e48
	bl 0x02008100
	movs	r0, #10
	movs	r1, #0
	movs	r2, #20
	bl 0x02009e68
	movs	r1, #128
	movs	r0, #10
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x02009e68
	movs	r2, #20
	movs	r0, #10
	movs	r1, #0
	bl 0x02009e68
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
	bl 0x02009e60
	movs	r1, #129
	movs	r0, #9
	lsls	r1, r1, #1
	bl 0x02009e90
	movs	r0, #9
	movs	r1, #0
	bl 0x02009e60
	movs	r1, #176
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x02009e70
	movs	r2, #20
	movs	r0, #10
	movs	r1, #4
	bl 0x02009e38
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #10
	movs	r1, #0
	bl 0x02009e60
	bl 0x02008100
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #176
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x02009e68
	bl 0x02008100
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	ldr	r0, [r6, #0]
	bl 0x02009e88
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #9
	bl 0x02009e88
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #192
	movs	r2, #0
	movs	r0, #9
	lsls	r1, r1, #6
	bl 0x02009e68
	movs	r0, #8
	movs	r1, #3
	bl 0x02009e28
	movs	r1, #192
	movs	r2, #20
	lsls	r1, r1, #6
	movs	r0, #10
	bl 0x02009e68
	movs	r0, #14
	bl 0x02009dc0
	adds	r5, r0, #0
	movs	r0, #10
	bl 0x02009dc0
	ldrh	r3, [r0, #6]
	ldr	r1, [pc, #60]
	strh	r3, [r5, #6]
	movs	r0, #1
	mov	r8, r1
	bl 0x02009cf0
	movs	r0, #14
	movs	r1, #10
	bl 0x02009e20
	movs	r2, #0
	movs	r1, #0
	movs	r0, #10
	bl 0x02009e10
	movs	r0, #110
	bl 0x02009f10
	movs	r1, #5
	movs	r0, #14
	bl 0x02009e28
.L_0200039c:
	movs	r0, #60
	bl 0x02009d98
	movs	r0, #20
	bl 0x02009d98
	movs	r1, #176
	movs	r2, #0
	b.n	.L_020003c4
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x00403108
	.4byte 0x0200a058
	.4byte 0x02000240
	.2byte 0x1fb5
	.2byte 0x0000
.L_020003c4:
	movs	r0, #14
	lsls	r1, r1, #8
	bl 0x02009e68
	movs	r1, #1
	movs	r0, #8
	bl 0x02009e28
	movs	r0, #40
	bl 0x02009d98
	movs	r1, #3
	movs	r0, #8
	bl 0x02009e28
	movs	r0, #110
	bl 0x02009f10
	movs	r1, #6
	movs	r0, #14
	bl 0x02009e28
	movs	r0, #60
	bl 0x02009d98
	movs	r1, #1
	movs	r0, #14
	bl 0x02009e28
.L_020003fe:
	movs	r0, #20
	bl 0x02009d98
	movs	r1, #1
	movs	r0, #8
	bl 0x02009e28
	movs	r0, #40
	bl 0x02009d98
	bl 0x02008100
	movs	r0, #14
	movs	r1, #2
.L_0200041a:
	movs	r2, #10
	bl 0x02009e38
	movs	r2, #10
	movs	r0, #14
	movs	r1, #4
	bl 0x02009e38
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #14
	movs	r1, #0
	bl 0x02009e60
	ldr	r1, [pc, #356]
	movs	r0, #14
	bl 0x02009de8
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #14
	movs	r1, #0
	bl 0x02009e60
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #14
	ldr	r1, [pc, #332]
	adds	r2, #204
	bl 0x02009dc8
	movs	r1, #196
	movs	r0, #14
	lsls	r1, r1, #1
	movs	r2, #164
	bl 0x02009df8
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #14
	bl 0x02009e88
	movs	r2, #20
	movs	r0, #14
	movs	r1, #6
	bl 0x02009e38
	movs	r0, #192
	lsls	r0, r0, #7
	adds	r0, #14
	movs	r1, #0
	bl 0x02009e60
	movs	r1, #196
	movs	r0, #14
	lsls	r1, r1, #1
	movs	r2, #220
	bl 0x02009df8
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x02009e10
	ldr	r0, [r6, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #0
	movs	r2, #0
	movs	r0, #9
	bl 0x02009e68
	movs	r0, #12
	bl 0x02009dc0
	mov	r2, r8
	adds	r0, #85
	strb	r2, [r0, #0]
	ldr	r1, [pc, #228]
	movs	r0, #12
	bl 0x02009de8
	movs	r0, #20
	bl 0x02009d98
	bl 0x02008100
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	ldr	r0, [r6, #0]
	bl 0x02009e88
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #9
	bl 0x02009e88
	movs	r1, #224
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #80
	movs	r0, #9
	bl 0x02009e68
	bl 0x02008100
	movs	r0, #20
	bl 0x02009d98
	bl 0x02008100
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #6
	movs	r2, #20
.L_02000514:
	bl 0x02009e68
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x02009e68
	movs	r1, #128
	movs	r2, #20
	ldr	r0, [r6, #0]
	lsls	r1, r1, #6
	bl 0x02009e68
	movs	r0, #8
	movs	r1, #3
	bl 0x02009e28
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #2
	bl 0x02009ec0
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #2
	adds	r0, #3
	bl 0x02009eb8
	movs	r0, #10
	bl 0x02009ec8
	movs	r0, #10
	bl 0x02009cf0
	ldr	r5, [pc, #76]
	ldr	r3, [pc, #56]
	movs	r7, #192
	strh	r3, [r5, #0]
	movs	r0, #1
	lsls	r7, r7, #18
	bl 0x02009cf0
	ldr	r3, [r7, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldrh	r0, [r3, #0]
	movs	r1, #0
	adds	r2, r0, #1
	lsls	r0, r0, #16
	strh	r2, [r3, #0]
	asrs	r0, r0, #16
	movs	r2, #2
	bl 0x02009d88
	movs	r0, #128
	movs	r1, #2
	lsls	r0, r0, #9
	bl 0x02009eb8
	movs	r0, #20
	bl 0x02009ec8
	movs	r0, #20
	b.n	.L_020005ac
	.4byte 0x00007fff
	.4byte 0x02009f78
	.4byte 0x00019999
	.4byte 0x02009f9c
	.2byte 0x021e
	.2byte 0x0500
.L_020005ac:
	bl 0x02009cf0
	movs	r0, #8
	movs	r1, #1
	bl 0x02009e28
	movs	r1, #224
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #40
	ldr	r0, [r6, #0]
.L_020005cc:
	bl 0x02009e88
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #176
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x02009e68
	bl 0x02008100
	movs	r1, #224
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #208
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
.L_02000600:
	bl 0x02009e68
	bl 0x02008100
	movs	r1, #2
.L_0200060a:
	adds	r1, #255
	movs	r2, #0
	ldr	r0, [r6, #0]
	bl 0x02009e88
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #9
	bl 0x02009e88
	bl 0x02008100
	ldr	r0, [r6, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x02009e68
	movs	r2, #0
	movs	r0, #9
	movs	r1, #0
	bl 0x02009e68
	ldr	r1, [pc, #780]
	movs	r0, #12
	bl 0x02009de8
	bl 0x02008100
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x02009e90
	movs	r1, #129
	movs	r0, #9
	lsls	r1, r1, #1
	bl 0x02009e90
	movs	r1, #224
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #208
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x02009e68
	bl 0x02008100
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #176
	movs	r2, #20
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x02009e68
	bl 0x02008100
	movs	r1, #208
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x02009e70
	movs	r0, #9
	movs	r1, #0
	bl 0x02009e60
	movs	r1, #224
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #13
	bl 0x02009e88
	bl 0x02008100
	movs	r0, #9
	movs	r1, #0
	movs	r2, #40
	bl 0x02009e58
	bl 0x02008100
	movs	r2, #10
	movs	r0, #9
	movs	r1, #4
	bl 0x02009e38
	movs	r0, #9
	movs	r1, #0
	bl 0x02009e60
	bl 0x02008100
	movs	r1, #128
	movs	r2, #0
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
.L_020006ea:
	bl 0x02009e68
	movs	r1, #176
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x02009e70
	movs	r0, #9
	movs	r1, #3
	bl 0x02009e30
	bl 0x02008100
	movs	r1, #224
.L_02000706:
	movs	r2, #0
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	bl 0x02009e68
	movs	r1, #208
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x02009e70
	movs	r0, #9
	movs	r1, #4
	bl 0x02009e28
.L_02000722:
	movs	r1, #0
	movs	r0, #9
	bl 0x02009e60
	bl 0x02008100
	movs	r0, #0
	bl 0x02009f10
	movs	r0, #220
	bl 0x02009f10
	movs	r1, #3
	movs	r0, #8
	bl 0x02009e28
.L_02000742:
	movs	r0, #13
	bl 0x02009dc0
	movs	r1, #7
	bl 0x02009e40
	movs	r1, #212
	movs	r2, #208
	lsls	r2, r2, #15
	movs	r0, #13
	lsls	r1, r1, #17
	bl 0x02009e10
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x02009eb8
	movs	r0, #40
	bl 0x02009ec8
	movs	r0, #40
	bl 0x02009cf0
	bl 0x02009094
	movs	r2, #0
	movs	r1, #0
	movs	r0, #13
	bl 0x02009e10
	movs	r0, #1
	bl 0x02009d98
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #2
.L_0200078e:
	bl 0x02009eb8
	movs	r1, #1
	ldr	r0, [pc, #436]
	bl 0x02009eb8
	movs	r0, #60
	bl 0x02009ec8
	movs	r0, #60
	bl 0x02009cf0
	bl 0x02008100
	movs	r0, #128
	movs	r1, #1
	lsls	r0, r0, #9
	bl 0x02009eb8
	movs	r0, #20
	bl 0x02009ec8
	movs	r0, #20
.L_020007bc:
	bl 0x02009cf0
	ldr	r5, [pc, #396]
	movs	r2, #3
	ldr	r3, [r5, #0]
	movs	r0, #10
	adds	r3, #85
.L_020007ca:
	strb	r2, [r3, #0]
	bl 0x02009d98
	movs	r0, #54
	adds	r0, #255
	bl 0x02009f10
	movs	r0, #40
	bl 0x02009d98
	movs	r0, #25
.L_020007e0:
	bl 0x02009f10
	movs	r0, #40
	bl 0x02009d98
	movs	r1, #128
	movs	r2, #0
	ldr	r0, [r6, #0]
	lsls	r1, r1, #6
	bl 0x02009e68
	movs	r1, #176
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x02009e70
	movs	r0, #9
.L_02000802:
	movs	r1, #0
	bl 0x02009e60
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x02009e30
	movs	r1, #208
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #196
	movs	r2, #104
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x02009df8
	movs	r1, #192
	ldr	r0, [r6, #0]
.L_0200082c:
	lsls	r1, r1, #8
	bl 0x02009e70
	ldr	r0, [r6, #0]
	movs	r1, #28
	bl 0x02009e28
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02000868
	ldr	r0, [r6, #0]
	bl 0x02009dc0
	ldr	r4, [r5, #0]
	mov	r2, r8
	adds	r3, r4, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	movs	r3, #144
	ldr	r2, [r0, #12]
	lsls	r3, r3, #14
	ldr	r1, [r0, #8]
	adds	r2, r2, r3
	ldr	r3, [r0, #16]
	adds	r0, r4, #0
	bl 0x02009d48
	ldr	r0, [r5, #0]
	bl 0x02009d50
.L_02000868:
	movs	r0, #242
	movs	r1, #0
	bl 0x02009db0
	ldr	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L_0200087a
	bl 0x02009d30
.L_0200087a:
	ldr	r0, [r6, #0]
	movs	r1, #1
	bl 0x02009e28
	movs	r1, #192
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #208
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x02009e68
	movs	r1, #0
	movs	r0, #9
	bl 0x02009e50
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x02009db8
	cmp	r0, #0
	bne.n	.L_020008ce
	movs	r0, #9
	movs	r1, #4
	bl 0x02009e28
	movs	r1, #0
	movs	r0, #9
	bl 0x02009e60
	ldr	r2, [r7, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020008ec
.L_020008ce:
	ldr	r2, [r7, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #9
	adds	r3, #1
	movs	r1, #3
	strh	r3, [r2, #0]
	bl 0x02009e28
	movs	r0, #9
	movs	r1, #0
	bl 0x02009e60
.L_020008ec:
	movs	r1, #188
	movs	r2, #124
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x02009df8
	movs	r0, #20
	bl 0x02009d98
	movs	r0, #9
	movs	r1, #0
	bl 0x02009e60
	movs	r1, #208
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x02009e70
	movs	r1, #0
	movs	r0, #9
	bl 0x02009e50
	ldr	r3, [pc, #56]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x02009db8
	cmp	r0, #0
	bne.n	.L_02000958
	movs	r0, #9
	movs	r1, #0
	bl 0x02009e60
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02000972
	.4byte 0x02009fe4
	.4byte 0x00403108
	.4byte 0x0200a058
	.2byte 0x0240
	.2byte 0x0200
.L_02000958:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	movs	r0, #9
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x02009e60
.L_02000972:
	movs	r0, #9
	movs	r1, #4
	bl 0x02009e28
	movs	r0, #9
	movs	r1, #0
	bl 0x02009e60
	ldr	r7, [pc, #256]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r7, r2
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x02009e30
	movs	r0, #9
	movs	r1, #0
	bl 0x02009e60
	movs	r0, #9
	movs	r1, #3
	bl 0x02009e30
	movs	r0, #9
	movs	r1, #0
	bl 0x02009e60
	movs	r0, #9
	movs	r1, #0
	bl 0x02009e60
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x02009e30
	ldr	r5, [pc, #204]
	movs	r0, #9
	adds	r1, r5, #0
	bl 0x02009dd0
	movs	r0, #20
	bl 0x02009d98
	adds	r1, r5, #0
	ldr	r0, [r6, #0]
	bl 0x02009dd0
	movs	r0, #40
	bl 0x02009d98
	movs	r1, #0
	movs	r0, #0
	bl 0x02009eb8
	movs	r6, #192
	movs	r0, #60
	bl 0x02009ec8
	lsls	r6, r6, #18
	movs	r0, #64
	bl 0x02009cf0
	ldr	r1, [r6, #108]
	movs	r3, #214
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r5, #218
	adds	r3, #85
	str	r3, [r2, #0]
	lsls	r5, r5, #1
	movs	r3, #10
	str	r3, [r1, r5]
	bl 0x02009ed8
	bl 0x02009ee0
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x02009eb8
	ldr	r3, [r6, #108]
	movs	r6, #1
	str	r6, [r3, r5]
	movs	r0, #1
	bl 0x02009ec8
	bl 0x02009d78
	movs	r1, #0
	movs	r2, #3
	ldr	r0, [pc, #96]
	bl 0x02009d88
	bl 0x02009d80
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009d18
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #107
	bl 0x02009d20
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #242
	bl 0x02009d20
	ldr	r2, [pc, #60]
	movs	r1, #152
	lsls	r1, r1, #2
	adds	r3, r7, r1
	strh	r2, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #98
	adds	r3, r7, r2
	ldr	r2, [pc, #44]
	subs	r1, #124
	strh	r6, [r3, #0]
	adds	r3, r7, r1
	strh	r2, [r3, #0]
	movs	r3, #243
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #6
	strh	r3, [r2, #0]
	movs	r0, #3
	bl 0x02009eb0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200a00c
	.4byte 0x00001fdf
	.4byte 0x0000006e
	.2byte 0x0070
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	ldr	r5, [pc, #176]
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r2, #133
	ldr	r3, [r3, #108]
	lsls	r2, r2, #2
	adds	r5, r5, r2
	adds	r6, r0, #0
	ldr	r0, [r5, #0]
	mov	r8, r1
	mov	r9, r3
	bl 0x02009dc0
	mov	sl, r0
	bl 0x02009da0
	movs	r0, #0
	bl 0x02009ee8
	mov	r2, sl
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009dc8
	ldr	r0, [r5, #0]
	adds	r1, r6, #0
	mov	r2, r8
	bl 0x02009df8
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	ldr	r0, [r5, #0]
	bl 0x02009e68
	ldr	r0, [r5, #0]
	bl 0x02009dd8
	ldr	r0, [r5, #0]
	bl 0x02009dc0
	movs	r1, #0
	bl 0x02009d70
	ldr	r0, [r5, #0]
	movs	r1, #13
	bl 0x02009e28
	lsls	r6, r6, #16
	mov	r3, r8
	lsls	r3, r3, #16
	ldr	r2, [pc, #68]
	adds	r1, r6, #0
	mov	r0, sl
	mov	r8, r3
	bl 0x02009d48
	mov	r0, sl
	bl 0x02009d50
	movs	r1, #10
	ldr	r0, [r5, #0]
	bl 0x02009e28
	movs	r0, #123
	bl 0x02009f10
	bl 0x02009ed8
	bl 0x02009ee0
	movs	r2, #170
	lsls	r2, r2, #1
	add	r9, r2
	mov	r2, r9
	movs	r3, #0
	ldrsh	r0, [r2, r3]
	bl 0x02009eb0
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff4
	.2byte 0xb500
	movs	r0, #156
	lsls	r0, r0, #1
	movs	r1, #144
	bl 0x02008a98
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #108]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x02009dc0
	ldr	r3, [r0, #8]
	asrs	r4, r3, #20
	ldr	r3, [r0, #16]
	asrs	r1, r3, #20
	cmp	r4, #26
	bne.n	.L_02000bae
	cmp	r1, #5
	bne.n	.L_02000b9c
	ldr	r3, [pc, #80]
	movs	r2, #128
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000bdc
.L_02000b9c:
	cmp	r1, #7
	bne.n	.L_02000bd2
	ldr	r3, [pc, #64]
	movs	r2, #64
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000bd2
	b.n	.L_02000bdc
.L_02000bae:
	cmp	r1, #6
	bne.n	.L_02000bd2
	cmp	r4, #25
	bne.n	.L_02000bc2
	ldr	r3, [pc, #44]
	movs	r2, #16
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000bdc
.L_02000bc2:
	cmp	r4, #27
	bne.n	.L_02000bd2
	ldr	r3, [pc, #28]
	movs	r2, #32
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000bdc
.L_02000bd2:
	movs	r0, #212
	lsls	r0, r0, #1
	movs	r1, #100
	bl 0x02008a98
.L_02000bdc:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r1, #132
	movs	r2, #188
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	adds	r1, r1, r3
	adds	r2, r2, r3
	mov	sl, r1
	mov	r8, r2
	bl 0x02009da0
	movs	r0, #0
	bl 0x02009ee8
	bl 0x02009d60
	movs	r7, #1
	movs	r6, #0
.L_02000c18:
	adds	r3, r6, #0
	subs	r3, #17
	cmp	r3, #62
	bhi.n	.L_02000c3e
	adds	r1, r7, #1
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_02000c2a
	adds	r3, r7, #4
.L_02000c2a:
	asrs	r0, r3, #2
	movs	r3, #16
	subs	r3, r3, r0
	lsls	r2, r0, #8
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	strh	r2, [r3, #0]
	adds	r7, r1, #0
.L_02000c3e:
	mov	r1, sl
	ldr	r3, [r1, #12]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r1, #12]
	mov	r1, r8
	ldr	r3, [r1, #12]
	adds	r6, #1
	adds	r3, r3, r2
	movs	r2, #176
	lsls	r2, r2, #1
	str	r3, [r1, #12]
	cmp	r6, r2
	blt.n	.L_02000c18
	bl 0x02009d40
	movs	r0, #1
	bl 0x02009cf0
	bl 0x02009ed0
	bl 0x02009ee0
	ldr	r5, [pc, #196]
	movs	r3, #133
	lsls	r3, r3, #2
	movs	r1, #204
	movs	r2, #204
	adds	r5, r5, r3
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x02009dc8
	movs	r1, #164
	lsls	r1, r1, #1
	movs	r2, #184
	ldr	r0, [r5, #0]
	bl 0x02009df8
	movs	r0, #20
	bl 0x02009d98
	movs	r0, #154
	lsls	r0, r0, #1
	bl 0x02009f10
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x02009e78
	cmp	r6, #0
	beq.n	.L_02000cf2
.L_02000cae:
	adds	r3, r6, #0
	subs	r3, #33
	cmp	r3, #62
	bhi.n	.L_02000cd4
	adds	r3, r7, #1
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L_02000cc0
	adds	r2, r7, #4
.L_02000cc0:
	asrs	r1, r2, #2
	movs	r3, #16
	subs	r3, r3, r1
	lsls	r2, r1, #8
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	strh	r2, [r3, #0]
	subs	r7, #1
.L_02000cd4:
	mov	r1, sl
	ldr	r3, [r1, #12]
	ldr	r2, [pc, #92]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r1, #12]
	mov	r1, r8
	ldr	r3, [r1, #12]
	subs	r6, #1
	adds	r3, r3, r2
	str	r3, [r1, #12]
	bl 0x02009cf0
	cmp	r6, #0
	bne.n	.L_02000cae
.L_02000cf2:
	ldr	r3, [pc, #64]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x02009e78
	movs	r0, #149
	lsls	r0, r0, #1
	bl 0x02009f10
	movs	r0, #1
	bl 0x02009cf0
	movs	r0, #125
	bl 0x02009f10
	movs	r0, #20
	bl 0x02009d98
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x02009d18
	bl 0x02009da8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #188
	adds	r1, r1, r3
	lsls	r2, r2, #1
	adds	r7, r3, r2
	mov	r8, r1
	bl 0x02009da0
	movs	r0, #0
	bl 0x02009ee8
	bl 0x02009d60
	ldr	r5, [pc, #228]
	movs	r3, #133
	lsls	r3, r3, #2
	movs	r1, #204
	movs	r2, #204
	adds	r5, r5, r3
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x02009dc8
	movs	r2, #8
	negs	r2, r2
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x02009e00
	ldr	r0, [r5, #0]
	bl 0x02009e08
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	bl 0x02009e70
	movs	r0, #154
	lsls	r0, r0, #1
	bl 0x02009f10
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x02009e78
	movs	r6, #1
	movs	r5, #0
.L_02000dae:
	adds	r3, r5, #0
	subs	r3, #17
	cmp	r3, #62
	bhi.n	.L_02000dd4
	adds	r1, r6, #1
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_02000dc0
	adds	r3, r6, #4
.L_02000dc0:
	asrs	r0, r3, #2
	movs	r3, #16
	subs	r3, r3, r0
	lsls	r2, r0, #8
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	strh	r2, [r3, #0]
	adds	r6, r1, #0
.L_02000dd4:
	mov	r1, r8
	ldr	r3, [r1, #12]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r1, #12]
	ldr	r3, [r7, #12]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r7, #12]
	bl 0x02009cf0
	movs	r2, #96
	adds	r5, #1
	adds	r2, #255
	cmp	r5, r2
	ble.n	.L_02000dae
	ldr	r5, [pc, #84]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x02009e78
	movs	r0, #149
	lsls	r0, r0, #1
	bl 0x02009f10
	movs	r0, #1
	bl 0x02009cf0
	movs	r0, #125
	bl 0x02009f10
	movs	r0, #20
	bl 0x02009d98
	ldr	r0, [r5, #0]
	bl 0x02009dc0
	movs	r3, #1
	adds	r0, #34
	strb	r3, [r0, #0]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x02009d18
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009d18
	bl 0x02009da8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x02009d20
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	bl 0x02009da0
	movs	r0, #0
	bl 0x02009ee8
	movs	r0, #196
	movs	r1, #1
	movs	r2, #240
	lsls	r2, r2, #15
	movs	r3, #1
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x02009ea0
	bl 0x02009ea8
	movs	r0, #20
	bl 0x02009d98
	ldr	r0, [pc, #96]
	bl 0x02009e48
	movs	r1, #129
	movs	r0, #10
	lsls	r1, r1, #1
	bl 0x02009e90
	movs	r0, #10
	movs	r1, #0
	bl 0x02009e60
	movs	r1, #128
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x02009e70
	movs	r0, #10
	movs	r1, #0
	bl 0x02009e60
	movs	r0, #10
	movs	r1, #0
	bl 0x02009e70
	movs	r0, #10
	movs	r1, #0
	bl 0x02009e60
	movs	r1, #208
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x02009e70
	movs	r0, #10
	movs	r1, #4
	bl 0x02009e30
	movs	r1, #0
	movs	r0, #10
	bl 0x02009e60
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #254
	bl 0x02009d18
	bl 0x02009da8
	pop	{pc}
	.2byte 0x1faf
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r0, #8
	bl 0x02009dc0
	ldr	r3, [r0, #8]
	str	r3, [r5, #8]
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	movs	r0, #8
	bl 0x02009dc0
	movs	r2, #144
	ldr	r1, [r0, #8]
	ldr	r3, [r0, #16]
	movs	r0, #234
	adds	r0, #255
	lsls	r2, r2, #13
	ldr	r5, [pc, #104]
	bl 0x02009d28
	movs	r7, #0
	str	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L_02000f7e
	ldr	r6, [r0, #80]
	movs	r3, #33
	ldrb	r2, [r6, #5]
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #13
	negs	r2, r2
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r6, #9]
	adds	r3, r0, #0
	adds	r3, #85
	adds	r2, r0, #0
	strb	r7, [r3, #0]
	adds	r2, #92
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r3, [pc, #52]
	movs	r1, #193
	str	r3, [r0, #108]
	lsls	r1, r1, #3
	strb	r7, [r6, #26]
	strb	r7, [r6, #27]
	movs	r0, #68
	bl 0x02009cf8
	adds	r5, r0, #0
	movs	r0, #230
	bl 0x02009d90
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r5, r5, r3
	ldrb	r0, [r6, #16]
	movs	r1, #128
	adds	r2, r5, #0
	bl 0x02009d08
	movs	r0, #68
	bl 0x02009d00
.L_02000f7e:
	pop	{r5, r6, r7, pc}
	.4byte 0x0200a058
	.2byte 0x8ef1
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #8
	bl 0x02009dc0
	movs	r2, #144
	ldr	r1, [r0, #8]
	ldr	r3, [r0, #16]
	movs	r0, #234
	adds	r0, #255
	lsls	r2, r2, #13
	ldr	r5, [pc, #104]
	bl 0x02009d28
	movs	r7, #0
	str	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L_02001006
	ldr	r6, [r0, #80]
	movs	r3, #33
	ldrb	r2, [r6, #5]
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #13
	negs	r2, r2
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r6, #9]
	adds	r3, r0, #0
	adds	r3, #85
	adds	r2, r0, #0
	strb	r7, [r3, #0]
	adds	r2, #92
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r3, [pc, #52]
	movs	r1, #193
	str	r3, [r0, #108]
	lsls	r1, r1, #3
	strb	r7, [r6, #26]
	strb	r7, [r6, #27]
	movs	r0, #68
	bl 0x02009cf8
	adds	r5, r0, #0
	movs	r0, #242
	bl 0x02009d90
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r5, r5, r3
	ldrb	r0, [r6, #16]
	movs	r1, #128
	adds	r2, r5, #0
	bl 0x02009d08
	movs	r0, #68
	bl 0x02009d00
.L_02001006:
	pop	{r5, r6, r7, pc}
	.4byte 0x0200a058
	.2byte 0x8ef1
	.2byte 0x0200
	push	{r5, r6, lr}
	sub	sp, #8
	movs	r3, #11
	str	r3, [sp, #4]
	movs	r6, #5
	movs	r0, #85
	movs	r1, #78
	movs	r2, #22
	movs	r3, #79
	str	r6, [sp, #0]
	bl 0x02009d58
	movs	r3, #22
	movs	r2, #15
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #85
	movs	r1, #78
	movs	r2, #5
	movs	r3, #11
	bl 0x02009d68
	movs	r5, #2
	movs	r0, #86
	movs	r1, #30
	movs	r2, #86
	movs	r3, #20
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02009d58
	movs	r3, #9
	str	r3, [sp, #0]
	movs	r0, #86
	movs	r1, #30
	movs	r2, #84
	movs	r3, #22
	str	r5, [sp, #4]
	bl 0x02009d58
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, lr}
	sub	sp, #8
	movs	r3, #23
	str	r3, [sp, #0]
	movs	r5, #9
	movs	r0, #23
	movs	r1, #1
	movs	r2, #3
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x02009d68
	movs	r3, #25
	str	r3, [sp, #0]
	movs	r0, #23
	movs	r1, #0
	movs	r2, #3
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x02009d68
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	sub	sp, #8
	movs	r5, #1
	movs	r0, #0
	movs	r1, #1
	movs	r2, #26
	movs	r3, #6
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02009d58
	movs	r0, #0
	movs	r1, #65
	movs	r2, #26
	movs	r3, #70
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02009d58
	movs	r3, #25
	movs	r2, #5
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #2
	movs	r2, #3
	movs	r3, #3
	bl 0x02009d68
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r0, #13
	bl 0x02009dc0
	adds	r7, r0, #0
	bl 0x02009da0
	movs	r0, #0
	bl 0x02009ee8
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x02009ea0
	movs	r0, #13
	bl 0x02009dc0
	movs	r1, #196
	movs	r3, #134
	lsls	r1, r1, #17
	movs	r2, #0
	lsls	r3, r3, #17
	bl 0x02009d38
	movs	r0, #1
	bl 0x02009cf0
	ldr	r3, [pc, #1012]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r2, r2, r3
	ldr	r0, [r2, #0]
	mov	sl, r2
	movs	r3, #224
	movs	r1, #180
	movs	r2, #184
	lsls	r3, r3, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	mov	r8, r3
	bl 0x02009e18
	movs	r1, #188
	movs	r2, #184
	movs	r0, #7
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	mov	r3, r8
	bl 0x02009e18
	movs	r1, #180
	movs	r2, #168
	movs	r0, #11
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	mov	r3, r8
	bl 0x02009e18
	movs	r2, #160
	lsls	r2, r2, #8
	mov	r9, r2
	movs	r2, #168
	movs	r0, #6
	ldr	r1, [pc, #944]
	lsls	r2, r2, #16
	mov	r3, r9
	bl 0x02009e18
	movs	r1, #204
	movs	r2, #184
	movs	r0, #5
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	mov	r3, r9
	bl 0x02009e18
	cmp	r7, #0
	beq.n	.L_020011d4
	ldr	r6, [r7, #80]
	movs	r3, #0
	ldrb	r2, [r6, #5]
	strb	r3, [r6, #26]
	strb	r3, [r6, #27]
	subs	r3, #33
	ands	r3, r2
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #13
	negs	r2, r2
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	adds	r2, r7, #0
	adds	r2, #92
	strb	r3, [r6, #9]
	movs	r1, #193
	movs	r3, #1
	strb	r3, [r2, #0]
	lsls	r1, r1, #3
	movs	r0, #68
	bl 0x02009cf8
	adds	r5, r0, #0
	movs	r0, #242
	bl 0x02009d90
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r5, r5, r3
	ldrb	r0, [r6, #16]
	movs	r1, #128
	adds	r2, r5, #0
	bl 0x02009d08
	movs	r0, #68
	bl 0x02009d00
.L_020011d4:
	movs	r6, #192
	lsls	r6, r6, #18
	bl 0x02009010
	ldr	r3, [r6, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	movs	r5, #128
	adds	r3, r3, r2
	lsls	r5, r5, #1
	str	r5, [r3, #0]
	bl 0x02009ed0
	bl 0x02009ee0
	movs	r0, #40
	bl 0x02009d98
	movs	r2, #10
	movs	r1, #4
	movs	r0, #7
	bl 0x02009e38
	ldr	r0, [pc, #792]
	bl 0x02009e48
	movs	r0, #7
	movs	r1, #0
	bl 0x02009e60
	movs	r1, #129
	movs	r0, #6
	lsls	r1, r1, #1
	bl 0x02009e90
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x02009e70
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #6
	movs	r1, #0
	bl 0x02009e60
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #5
	bl 0x02009e88
	movs	r1, #0
	movs	r0, #5
	bl 0x02009e60
	movs	r0, #13
	bl 0x02009dc0
	movs	r1, #196
	movs	r3, #170
	movs	r2, #0
	lsls	r3, r3, #17
	lsls	r1, r1, #17
	bl 0x02009d38
	movs	r0, #78
	bl 0x02009f10
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #2
	bl 0x02009eb8
	movs	r0, #40
	bl 0x02009ec8
	movs	r0, #40
	bl 0x02009cf0
	movs	r0, #9
	movs	r1, #2
	bl 0x02009e28
	movs	r0, #10
	movs	r1, #2
	bl 0x02009e28
	mov	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #13
	bl 0x02009ef8
	movs	r0, #5
	movs	r1, #13
	bl 0x02009ef8
	movs	r0, #6
	movs	r1, #13
	bl 0x02009ef8
	movs	r0, #7
	movs	r1, #13
	bl 0x02009ef8
	movs	r1, #13
	movs	r0, #11
	bl 0x02009ef8
	movs	r0, #10
	bl 0x02009d98
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x02009e98
	movs	r0, #196
	movs	r1, #1
	movs	r2, #246
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x02009ea0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #13
	adds	r1, #204
	adds	r2, #102
	bl 0x02009dc8
	movs	r1, #196
	movs	r2, #246
	movs	r0, #13
	lsls	r1, r1, #1
	bl 0x02009df0
	movs	r0, #9
	movs	r1, #1
	bl 0x02009e28
	movs	r0, #10
	movs	r1, #1
	bl 0x02009e28
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r7, #40]
	movs	r2, #204
	movs	r3, #128
	lsls	r3, r3, #8
	lsls	r2, r2, #8
	str	r3, [r7, #72]
	movs	r0, #13
	ldr	r1, [pc, #516]
	adds	r2, #204
	bl 0x02009dc8
	movs	r1, #196
	lsls	r1, r1, #1
	movs	r2, #234
	movs	r0, #13
	bl 0x02009df0
	movs	r0, #10
	bl 0x02009d98
	movs	r0, #196
	movs	r1, #1
	movs	r2, #168
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x02009ea0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #13
	adds	r1, #204
	adds	r2, #102
	bl 0x02009dc8
	movs	r1, #196
	lsls	r1, r1, #1
	movs	r0, #13
	movs	r2, #160
	bl 0x02009df0
	mov	r2, sl
	ldr	r0, [r2, #0]
	bl 0x02009de0
	movs	r0, #5
	bl 0x02009de0
	movs	r0, #6
	bl 0x02009de0
	movs	r0, #7
	bl 0x02009de0
	movs	r0, #11
	bl 0x02009de0
	movs	r0, #1
	bl 0x02009cf0
	movs	r2, #0
	movs	r1, #0
	movs	r0, #13
	bl 0x02009e10
	movs	r0, #20
	bl 0x02009d98
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x02009eb8
	movs	r0, #40
	bl 0x02009ec8
	movs	r0, #40
	bl 0x02009cf0
	bl 0x02009ef0
	movs	r2, #40
	adds	r1, r5, #0
	movs	r0, #11
	bl 0x02009e88
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #11
	movs	r1, #0
	bl 0x02009e60
	movs	r1, #6
	movs	r2, #20
	adds	r1, #255
	movs	r0, #6
	bl 0x02009e88
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #6
	movs	r1, #0
	bl 0x02009e60
	movs	r0, #5
	mov	r1, r8
	bl 0x02009e70
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #5
	movs	r1, #0
	bl 0x02009e60
	movs	r0, #5
	mov	r1, r9
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #6
	adds	r1, #204
	adds	r2, #102
	bl 0x02009dc8
	movs	r1, #200
	movs	r2, #172
	movs	r0, #6
	lsls	r1, r1, #1
	bl 0x02009df8
	movs	r1, #9
	movs	r0, #6
	bl 0x02009e28
	movs	r0, #20
	bl 0x02009d98
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #6
	movs	r1, #0
	bl 0x02009e60
	movs	r0, #7
	movs	r1, #4
	bl 0x02009e28
	movs	r0, #7
	movs	r1, #0
	bl 0x02009e60
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #6
	movs	r1, #0
	bl 0x02009e60
	movs	r2, #0
	adds	r1, r5, #0
	movs	r0, #11
	bl 0x02009e88
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #11
	movs	r1, #0
	bl 0x02009e60
	movs	r1, #192
	movs	r0, #11
	lsls	r1, r1, #6
	bl 0x02009e70
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #11
	movs	r1, #0
	bl 0x02009e60
	mov	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #3
	bl 0x02009e30
	movs	r0, #0
	bl 0x02009f10
	movs	r3, #208
	movs	r1, #200
	movs	r2, #160
	lsls	r3, r3, #8
	lsls	r2, r2, #17
	lsls	r1, r1, #17
	movs	r0, #12
	bl 0x02009e18
	movs	r0, #1
	bl 0x02009cf0
	movs	r0, #12
	movs	r1, #0
	bl 0x02009e60
	movs	r1, #1
	movs	r0, #6
	bl 0x02009e28
	movs	r0, #1
	bl 0x02009cf0
	mov	r2, sl
	movs	r1, #128
	ldr	r0, [r2, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #192
	movs	r2, #0
	movs	r0, #11
	lsls	r1, r1, #6
	bl 0x02009e68
	movs	r1, #204
	lsls	r1, r1, #6
	ldr	r0, [pc, #32]
	adds	r1, #51
	bl 0x02009e98
	movs	r0, #196
	movs	r1, #1
	movs	r2, #200
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	b.n	.L_02001524
	.4byte 0x02000240
	.4byte 0x01a70000
	.4byte 0x00001f60
	.2byte 0x9999
	.2byte 0x0001
.L_02001524:
	lsls	r2, r2, #16
	bl 0x02009ea0
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #12
	ldr	r1, [pc, #460]
	adds	r2, #153
	bl 0x02009dc8
	movs	r1, #202
	movs	r0, #12
	lsls	r1, r1, #1
	movs	r2, #220
	bl 0x02009df8
	movs	r0, #12
	movs	r1, #4
	movs	r2, #10
	bl 0x02009e38
	movs	r1, #0
	movs	r0, #12
	bl 0x02009e50
	mov	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x02009db8
	cmp	r0, #0
	bne.n	.L_0200158c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #12
	bl 0x02009e90
	movs	r0, #20
	bl 0x02009d98
	movs	r0, #12
	movs	r1, #0
	bl 0x02009e60
	ldr	r2, [r6, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020015ae
.L_0200158c:
	ldr	r2, [r6, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r1, #4
	adds	r3, #1
	strh	r3, [r2, #0]
	adds	r1, #255
	movs	r0, #12
	movs	r2, #20
	bl 0x02009e88
	movs	r0, #12
	movs	r1, #0
	bl 0x02009e60
.L_020015ae:
	movs	r1, #8
	movs	r2, #20
	adds	r1, #255
	movs	r0, #7
	bl 0x02009e88
	movs	r0, #7
	movs	r1, #0
	bl 0x02009e60
	movs	r1, #128
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #12
	bl 0x02009e88
	movs	r0, #12
	movs	r1, #0
	bl 0x02009e60
	movs	r1, #8
	movs	r2, #20
	adds	r1, #255
	movs	r0, #6
	bl 0x02009e88
	movs	r0, #6
	movs	r1, #0
	bl 0x02009e60
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #12
	bl 0x02009e90
	movs	r0, #20
	bl 0x02009d98
	movs	r0, #12
	movs	r1, #0
	bl 0x02009e60
.L_02001602:
	ldr	r3, [pc, #256]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #3
	bl 0x02009e28
	movs	r0, #5
	movs	r1, #3
	bl 0x02009e28
	movs	r0, #6
	movs	r1, #3
	bl 0x02009e28
	movs	r0, #7
	movs	r1, #3
	bl 0x02009e28
	movs	r0, #11
	movs	r1, #3
	bl 0x02009e30
	movs	r0, #12
	movs	r1, #4
	bl 0x02009e28
	movs	r0, #12
	movs	r1, #0
	bl 0x02009e60
	movs	r1, #6
	adds	r1, #255
	movs	r2, #20
	movs	r0, #5
	bl 0x02009e88
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #5
	adds	r1, #204
	adds	r2, #102
	bl 0x02009dc8
	movs	r1, #210
	movs	r2, #228
	movs	r0, #5
	lsls	r1, r1, #1
	bl 0x02009df8
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r0, #5
.L_02001672:
	bl 0x02009e70
	movs	r0, #10
	bl 0x02009d98
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #132]
	adds	r2, #204
	bl 0x02009dc8
	movs	r1, #206
	movs	r0, #5
	lsls	r1, r1, #1
	movs	r2, #222
	bl 0x02009df8
	movs	r1, #128
	movs	r2, #128
	movs	r0, #5
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009dc8
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r0, #12
	lsls	r1, r1, #9
	bl 0x02009dc8
	ldr	r5, [pc, #88]
	movs	r0, #5
	adds	r1, r5, #0
	bl 0x02009dd0
	movs	r0, #10
	bl 0x02009d98
	adds	r1, r5, #0
	movs	r0, #12
	bl 0x02009dd0
	movs	r0, #60
	bl 0x02009d98
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #86
	str	r2, [r3, #0]
	bl 0x02009ed8
	bl 0x02009ee0
	movs	r0, #10
	adds	r0, #255
	bl 0x02009d18
	movs	r0, #2
	bl 0x02009eb0
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x00013333
	.4byte 0x02000240
	.4byte 0x00019999
	.2byte 0x9f18
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	bl 0x02009da0
	movs	r0, #0
	bl 0x02009ee8
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x02009ea0
	movs	r0, #1
	bl 0x02009cf0
	bl 0x02009010
	bl 0x02009d40
	movs	r0, #1
	bl 0x02009cf0
	ldr	r6, [pc, #840]
	movs	r2, #133
	lsls	r2, r2, #2
	movs	r5, #224
	adds	r6, r6, r2
	lsls	r5, r5, #8
	movs	r1, #180
	movs	r2, #216
	adds	r3, r5, #0
	ldr	r0, [r6, #0]
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x02009e18
	movs	r1, #188
	movs	r2, #216
	adds	r3, r5, #0
	movs	r0, #7
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x02009e18
	movs	r1, #180
	movs	r2, #200
	movs	r3, #0
	movs	r0, #11
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	movs	r5, #160
	mov	sl, r3
	lsls	r5, r5, #8
	bl 0x02009e18
	movs	r1, #204
	movs	r2, #216
	movs	r0, #5
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	adds	r3, r5, #0
	bl 0x02009e18
	movs	r2, #128
	lsls	r2, r2, #8
	mov	r9, r2
	movs	r1, #212
	movs	r2, #200
	movs	r0, #6
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	mov	r3, r9
	bl 0x02009e18
	movs	r3, #176
	movs	r1, #196
	movs	r2, #200
	lsls	r1, r1, #17
	lsls	r3, r3, #8
	lsls	r2, r2, #16
	movs	r0, #12
	movs	r7, #192
	bl 0x02009e18
	lsls	r7, r7, #18
	movs	r0, #1
	bl 0x02009cf0
	ldr	r3, [r7, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	subs	r2, #172
	str	r2, [r3, #0]
	mov	r8, r2
	bl 0x02009ed0
	bl 0x02009ee0
	movs	r0, #40
	bl 0x02009d98
	movs	r1, #208
	movs	r2, #20
	movs	r0, #12
	lsls	r1, r1, #8
	bl 0x02009e68
	movs	r1, #160
	movs	r0, #12
	lsls	r1, r1, #7
	bl 0x02009e70
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #12
	bl 0x02009e88
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #208
	movs	r0, #11
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009e68
	movs	r0, #6
	adds	r1, r5, #0
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #12
	adds	r1, #204
	adds	r2, #102
	bl 0x02009dc8
	movs	r1, #196
	lsls	r1, r1, #1
	movs	r2, #168
	movs	r0, #12
	bl 0x02009df8
	movs	r0, #210
	bl 0x02009f10
	bl 0x02008f00
	movs	r0, #40
	bl 0x02009d98
	movs	r0, #185
	bl 0x02009f10
	movs	r0, #8
	bl 0x02009dc0
	mov	r3, sl
	adds	r0, #85
	strb	r3, [r0, #0]
	bl 0x02009064
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #8
	adds	r1, #102
	adds	r2, #51
	bl 0x02009dc8
	movs	r1, #212
	movs	r2, #152
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x02009df0
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #12
	bl 0x02009e70
	ldr	r0, [pc, #496]
	bl 0x02009e48
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #12
	movs	r1, #0
	bl 0x02009e60
	movs	r0, #12
	movs	r1, #3
	bl 0x02009e30
	movs	r1, #196
	lsls	r1, r1, #1
	movs	r2, #140
	movs	r0, #12
	bl 0x02009df8
	movs	r0, #1
	bl 0x02009cf0
	movs	r1, #0
	movs	r2, #0
	movs	r0, #12
	bl 0x02009e10
	movs	r0, #1
	bl 0x02009cf0
	mov	r1, r8
	movs	r2, #20
	movs	r0, #5
	bl 0x02009e88
	movs	r0, #5
	mov	r1, r9
	movs	r2, #0
	bl 0x02009e68
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #5
	bl 0x02009e50
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x02009db8
	cmp	r0, #0
	bne.n	.L_0200193a
	movs	r0, #7
	movs	r1, #0
	bl 0x02009e70
	movs	r0, #7
	movs	r1, #3
	bl 0x02009e28
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #7
	movs	r1, #0
	bl 0x02009e60
	ldr	r2, [r7, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02001960
.L_0200193a:
	ldr	r2, [r7, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #7
	adds	r3, #1
	strh	r3, [r2, #0]
	mov	r1, r9
	bl 0x02009e70
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #7
	movs	r1, #0
	bl 0x02009e60
	bl 0x02009f08
.L_02001960:
	movs	r1, #128
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x02009e68
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #6
	bl 0x02009e90
	movs	r0, #20
	bl 0x02009d98
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #6
	movs	r1, #0
	bl 0x02009e60
	movs	r0, #11
	movs	r1, #0
	bl 0x02009e70
	movs	r0, #11
	movs	r1, #4
	bl 0x02009e28
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #11
	movs	r1, #0
	bl 0x02009e60
	movs	r1, #208
	movs	r0, #11
	lsls	r1, r1, #8
	bl 0x02009e70
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #11
	movs	r1, #0
	bl 0x02009e60
	movs	r1, #192
	movs	r0, #11
	lsls	r1, r1, #6
	bl 0x02009e70
	movs	r0, #11
	movs	r1, #3
	bl 0x02009e30
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #11
	movs	r1, #0
	bl 0x02009e60
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009e68
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x02009e68
	ldr	r3, [pc, #160]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #3
	bl 0x02009e28
	movs	r0, #7
	movs	r1, #3
	bl 0x02009e28
	movs	r0, #5
	movs	r1, #3
	bl 0x02009e28
	movs	r0, #6
	movs	r1, #3
	bl 0x02009e30
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #11
	adds	r1, #204
	adds	r2, #102
	bl 0x02009dc8
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #7
	adds	r1, #204
	adds	r2, #102
	bl 0x02009dc8
	movs	r1, #128
	movs	r2, #128
	movs	r0, #5
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009dc8
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #6
	ldr	r1, [pc, #72]
	bl 0x02009dc8
	ldr	r5, [pc, #68]
	movs	r0, #11
	adds	r1, r5, #0
	bl 0x02009dd0
	adds	r1, r5, #0
	movs	r0, #7
	bl 0x02009dd0
	adds	r1, r5, #0
	movs	r0, #5
	bl 0x02009dd0
	adds	r1, r5, #0
	movs	r0, #6
	bl 0x02009de8
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #253
	bl 0x02009d18
	bl 0x02009da8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00001f7f
	.4byte 0x00013333
	.2byte 0x9f4c
	.2byte 0x0200
	.global Func_02001aa4
	.thumb_func
Func_02001aa4:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	movs	r2, #133
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	adds	r3, r3, r1
	adds	r2, #255
	str	r2, [r3, #0]
	bl 0x02009f00
	bl 0x02009dc0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	ldr	r3, [pc, #48]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #40]
	cmp	r2, r3
	bne.n	.L_02001ae4
	bl 0x02009b14
	b.n	.L_02001afa
.L_02001ae4:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_02001af0
	bl 0x02009ba8
	b.n	.L_02001afa
.L_02001af0:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_02001afa
	bl 0x02009c44
.L_02001afa:
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000078
	.4byte 0x0000007a
	.2byte 0x007c
	.2byte 0x0000
	.global Func_02001b10
	.thumb_func
Func_02001b10:
	movs	r0, #0
	bx	lr
	push	{lr}
	movs	r0, #8
	bl 0x02009e80
	movs	r0, #9
	bl 0x02009e80
	movs	r0, #10
	bl 0x02009e80
	movs	r0, #1
	bl 0x02009cf0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #253
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_02001b5e
	bl 0x02009010
	bl 0x02009064
	movs	r1, #212
	movs	r2, #152
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x02009e10
	movs	r0, #1
	bl 0x02009cf0
	bl 0x02008f00
	b.n	.L_02001b7e
.L_02001b5e:
	ldr	r3, [pc, #68]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #5
	bne.n	.L_02001b74
	bl 0x020090d4
	b.n	.L_02001ba2
.L_02001b74:
	cmp	r3, #6
	bne.n	.L_02001b7e
	bl 0x02009710
	b.n	.L_02001ba2
.L_02001b7e:
	ldr	r3, [pc, #36]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L_02001ba2
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x02009d20
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #40
	bl 0x02009d18
.L_02001ba2:
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #10
	adds	r0, #255
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_02001c28
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_02001c3c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #132
	lsls	r2, r2, #1
	adds	r7, r3, r2
	adds	r2, #112
	adds	r6, r3, r2
	movs	r5, #1
	bl 0x02009d60
	movs	r4, #0
.L_02001bdc:
	adds	r3, r4, #0
	subs	r3, #17
	cmp	r3, #62
	bhi.n	.L_02001c02
	adds	r1, r5, #1
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_02001bee
	adds	r3, r5, #4
.L_02001bee:
	asrs	r0, r3, #2
	movs	r3, #16
	subs	r3, r3, r0
	lsls	r2, r0, #8
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	strh	r2, [r3, #0]
	adds	r5, r1, #0
.L_02001c02:
	ldr	r3, [r7, #12]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r7, #12]
	adds	r4, #1
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	movs	r3, #176
	lsls	r3, r3, #1
	cmp	r4, r3
	blt.n	.L_02001bdc
	bl 0x02009d40
	movs	r0, #1
	bl 0x02009cf0
	b.n	.L_02001c3c
.L_02001c28:
	ldr	r3, [pc, #20]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #2
	bne.n	.L_02001c3c
	bl 0x02008be8
.L_02001c3c:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r5, #0
.L_02001c48:
	adds	r0, r5, #0
	adds	r0, #15
	movs	r1, #2
	adds	r5, #1
	bl 0x02009e78
	cmp	r5, #17
	bls.n	.L_02001c48
	movs	r0, #8
	bl 0x02009e80
	movs	r0, #8
	bl 0x02009dc0
	movs	r3, #0
	str	r3, [r0, #12]
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_02001c86
	bl 0x02009094
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x02009e10
	b.n	.L_02001c8a
.L_02001c86:
	bl 0x02008f88
.L_02001c8a:
	ldr	r5, [pc, #88]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #2
	bne.n	.L_02001ce2
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_02001cbe
	ldr	r2, [pc, #60]
	movs	r1, #152
	lsls	r1, r1, #2
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #98
	adds	r2, r5, r3
	movs	r3, #1
	b.n	.L_02001ce0
.L_02001cbe:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #40
	bl 0x02009d10
	cmp	r0, #0
	beq.n	.L_02001ce2
	ldr	r2, [pc, #28]
	movs	r1, #152
	lsls	r1, r1, #2
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #98
	adds	r2, r5, r3
	movs	r3, #3
.L_02001ce0:
	strh	r3, [r2, #0]
.L_02001ce2:
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x0000007b
	.4byte 0x00000078
	.section .rodata,"a",%progbits
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01ac0000
	.4byte 0x00000000
	.4byte 0x00ea0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00ea0000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00000800
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000002
	.4byte 0x01880000
	.4byte 0x00400000
	.4byte 0x007c0000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000040
	.4byte 0xc0010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00020000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000050
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00940000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00ec0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00000188
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x001001d0
	.4byte 0x01e00090
	.4byte 0x00a00020
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x10102077
	.4byte 0xffffffff
	.4byte 0x10201079
	.4byte 0xffffffff
	.4byte 0x1030406f
	.4byte 0x0000086b
	.4byte 0x10304072
	.4byte 0xffffffff
	.4byte 0x1040107a
	.4byte 0xffffffff
	.4byte 0x0000007a
	.4byte 0x00104078
	.4byte 0x0020107c
	.4byte 0x0000007c
	.4byte 0x0010207a
	.4byte 0x0020107e
	.4byte 0x00309070
	.4byte 0x000001ff
	.4byte 0xffff0177
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff0169
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00020000
	.4byte 0xffff0169
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x01020000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff001a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
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
	.4byte 0xffff0177
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff001a
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0002d000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00024000
	.4byte 0xffff01e7
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff001b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002d000
	.4byte 0xffff014a
	.4byte 0x00000001
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff014a
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x0200a048
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x0200a048
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x0200a048
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x00000001
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x0200a048
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x0200a048
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x0200a048
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x00000001
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x0200a048
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x0200a048
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff014a
	.4byte 0x0200a048
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008b61
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x02060003
	.4byte 0x02008d3d
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008e51
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000202
	.4byte 0xffff0002
	.4byte 0x02008b71
	.4byte 0x00000002
	.4byte 0x08fe000a
	.4byte 0x02008e61
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001fb3
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001fb4
	.4byte 0x0000c403
	.4byte 0x08ff000b
	.4byte 0x02008185
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
