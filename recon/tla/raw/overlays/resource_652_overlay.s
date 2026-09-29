.syntax unified
	.thumb
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x913c
	.2byte 0x0200
	.global Func_02000040
	.thumb_func
Func_02000040:
	movs	r0, #0
	bx	lr
	.global Func_02000044
	.thumb_func
Func_02000044:
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #115
	bl 0x02008c84
	cmp	r0, #0
	beq.n	.L_02000058
	ldr	r0, [pc, #4]
	b.n	.L_0200005a
.L_02000058:
	ldr	r0, [pc, #4]
.L_0200005a:
	pop	{pc}
	.4byte 0x020091b8
	.2byte 0x916c
	.2byte 0x0200
	.global Func_02000064
	.thumb_func
Func_02000064:
	push	{lr}
	ldr	r3, [pc, #44]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_0200007c
	ldr	r0, [pc, #32]
	b.n	.L_02000090
.L_0200007c:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #115
	bl 0x02008c84
	cmp	r0, #0
	beq.n	.L_0200008e
	ldr	r0, [pc, #20]
	b.n	.L_02000090
.L_0200008e:
	ldr	r0, [pc, #20]
.L_02000090:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000000c
	.4byte 0x020093fc
	.4byte 0x020094a4
	.4byte 0x02009204
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r1, #0
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #50
	adds	r0, r5, #0
	bl 0x02008d94
	adds	r0, r5, #0
	movs	r1, #6
	movs	r2, #15
	bl 0x02008d5c
	adds	r0, r5, #0
	movs	r1, #6
	movs	r2, #15
	bl 0x02008d5c
	movs	r0, #10
	bl 0x02008d1c
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02008c84
	cmp	r0, #0
	bne.n	.L_02000112
	ldr	r0, [pc, #100]
	bl 0x02008d6c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008d7c
	movs	r1, #180
	adds	r0, r5, #0
	lsls	r1, r1, #1
	movs	r2, #72
	bl 0x02008d44
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02008d84
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02008c8c
	b.n	.L_02000140
.L_02000112:
	ldr	r0, [pc, #56]
	bl 0x02008d6c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008d7c
	movs	r1, #196
	adds	r0, r5, #0
	lsls	r1, r1, #1
	movs	r2, #104
	bl 0x02008d44
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02008d84
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02008c94
.L_02000140:
	movs	r0, #20
	bl 0x02008d1c
	pop	{r5, pc}
	.4byte 0x0000156d
	.2byte 0x156e
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r0, r5, #0
	movs	r1, #4
	movs	r2, #0
	bl 0x02008d64
	movs	r0, #10
	bl 0x02008d1c
	movs	r1, #4
	adds	r0, r5, #0
	adds	r1, #255
	movs	r2, #50
	bl 0x02008d94
	adds	r0, r5, #0
	movs	r1, #6
	movs	r2, #15
	bl 0x02008d5c
	movs	r2, #15
	movs	r1, #6
	adds	r0, r5, #0
	bl 0x02008d5c
	movs	r0, #10
	bl 0x02008d1c
	ldr	r0, [pc, #20]
	bl 0x02008d6c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008d7c
	movs	r0, #10
	bl 0x02008d1c
	pop	{r5, pc}
	.2byte 0x156f
	.2byte 0x0000
	.section .text.x02008218,"ax",%progbits
	.balign 4
	.global Func_02000218
	.thumb_func
Func_02000218:
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #115
	bl 0x02008c84
	ldr	r3, [pc, #48]
	ldr	r2, [pc, #52]
	cmp	r0, #0
	beq.n	.L_02000242
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, r2
	bne.n	.L_0200023e
	ldr	r0, [pc, #36]
	b.n	.L_02000256
.L_0200023e:
	ldr	r0, [pc, #36]
	b.n	.L_02000256
.L_02000242:
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, r2
	bne.n	.L_02000254
	ldr	r0, [pc, #20]
	b.n	.L_02000256
.L_02000254:
	ldr	r0, [pc, #20]
.L_02000256:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000000c
	.4byte 0x02009c18
	.4byte 0x02009990
	.4byte 0x02009924
	.2byte 0x96e4
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02008d3c
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_020002ac
	movs	r0, #1
	adds	r1, r5, #0
	bl 0x02008ddc
	b.n	.L_020002c8
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_020002ac:
	bl 0x02008d24
	movs	r0, #0
	bl 0x02008db4
	ldr	r0, [pc, #20]
	bl 0x02008d6c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008d7c
	bl 0x02008d2c
.L_020002c8:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1747
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02008d3c
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_0200030c
	movs	r0, #2
	adds	r1, r5, #0
	bl 0x02008ddc
	b.n	.L_02000328
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_0200030c:
	bl 0x02008d24
	movs	r0, #0
	bl 0x02008db4
	ldr	r0, [pc, #20]
	bl 0x02008d6c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008d7c
	bl 0x02008d2c
.L_02000328:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1749
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02008d3c
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_0200036c
	movs	r0, #0
	adds	r1, r5, #0
	bl 0x02008dec
	b.n	.L_02000388
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_0200036c:
	bl 0x02008d24
	movs	r0, #0
	bl 0x02008db4
	ldr	r0, [pc, #20]
	bl 0x02008d6c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008d7c
	bl 0x02008d2c
.L_02000388:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x174d
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r3, [pc, #80]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02008d3c
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #52]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_02000438
	movs	r0, #233
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x02008c84
	cmp	r0, #0
	bne.n	.L_02000430
	bl 0x02008d24
	movs	r0, #0
	bl 0x02008db4
	ldr	r5, [pc, #20]
	adds	r0, r5, #0
	bl 0x02008d6c
	movs	r1, #0
	adds	r0, r6, #0
	b.n	.L_020003ec
	.2byte 0x0000
	.4byte 0xffffc000
	.4byte 0x02000240
	.2byte 0x1755
	.2byte 0x0000
.L_020003ec:
	bl 0x02008d74
	bl 0x02008dd4
	movs	r1, #0
	bl 0x02008d34
	cmp	r0, #0
	bne.n	.L_0200040c
	movs	r0, #10
	bl 0x02008d1c
	adds	r0, r5, #1
	bl 0x02008d6c
	b.n	.L_02000418
.L_0200040c:
	movs	r0, #20
	bl 0x02008d1c
	adds	r0, r5, #2
	bl 0x02008d6c
.L_02000418:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02008d7c
	bl 0x02008d2c
	movs	r0, #233
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x02008c8c
	b.n	.L_02000490
.L_02000430:
	adds	r0, r6, #0
	bl 0x02008de4
	b.n	.L_02000490
.L_02000438:
	bl 0x02008d24
	movs	r0, #0
	bl 0x02008db4
	ldr	r5, [pc, #80]
	adds	r0, r5, #0
	bl 0x02008d6c
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x02008d74
	bl 0x02008dd4
	movs	r1, #0
	bl 0x02008d34
	cmp	r0, #0
	bne.n	.L_0200046e
	movs	r0, #10
	bl 0x02008d1c
	adds	r0, r5, #1
	bl 0x02008d6c
	b.n	.L_0200047a
.L_0200046e:
	movs	r0, #20
	bl 0x02008d1c
	adds	r0, r5, #2
	bl 0x02008d6c
.L_0200047a:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02008d7c
	bl 0x02008d2c
	movs	r0, #233
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x02008c8c
.L_02000490:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x1755
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02008d3c
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #20]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_020004d0
	adds	r0, r5, #0
	bl 0x02008de4
	b.n	.L_020004ec
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_020004d0:
	bl 0x02008d24
	movs	r0, #0
	bl 0x02008db4
	ldr	r0, [pc, #20]
	bl 0x02008d6c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008d7c
	bl 0x02008d2c
.L_020004ec:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1844
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02008d3c
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_02000530
	movs	r0, #1
	adds	r1, r5, #0
	bl 0x02008ddc
	b.n	.L_0200054c
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000530:
	bl 0x02008d24
	movs	r0, #0
	bl 0x02008db4
	ldr	r0, [pc, #20]
	bl 0x02008d6c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008d7c
	bl 0x02008d2c
.L_0200054c:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1833
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02008d3c
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_02000590
	movs	r0, #2
	adds	r1, r5, #0
	bl 0x02008ddc
	b.n	.L_020005ac
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000590:
	bl 0x02008d24
	movs	r0, #0
	bl 0x02008db4
	ldr	r0, [pc, #20]
	bl 0x02008d6c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008d7c
	bl 0x02008d2c
.L_020005ac:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1835
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x02008d3c
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_020005f0
	movs	r0, #0
	adds	r1, r5, #0
	bl 0x02008dec
	b.n	.L_0200060c
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_020005f0:
	bl 0x02008d24
	movs	r0, #0
	bl 0x02008db4
	ldr	r0, [pc, #20]
	bl 0x02008d6c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008d7c
	bl 0x02008d2c
.L_0200060c:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1839
	.2byte 0x0000
	push	{r5, lr}
	bl 0x02008d24
	movs	r0, #0
	bl 0x02008db4
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl 0x02008d6c
	movs	r1, #0
	movs	r0, #12
	bl 0x02008d74
	bl 0x02008dd4
	movs	r1, #0
	bl 0x02008d34
	cmp	r0, #0
	bne.n	.L_0200064c
	movs	r0, #10
	bl 0x02008d1c
	adds	r0, r5, #1
	bl 0x02008d6c
	b.n	.L_02000658
.L_0200064c:
	movs	r0, #20
	bl 0x02008d1c
	adds	r0, r5, #2
	bl 0x02008d6c
.L_02000658:
	movs	r0, #12
	movs	r1, #0
	bl 0x02008d7c
	bl 0x02008d2c
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x180c
	.2byte 0x0000
	push	{r5, lr}
	bl 0x02008d24
	movs	r0, #0
	bl 0x02008db4
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl 0x02008d6c
	movs	r1, #0
	movs	r0, #14
	bl 0x02008d74
	bl 0x02008dd4
	movs	r1, #0
	bl 0x02008d34
	cmp	r0, #0
	bne.n	.L_020006a4
	movs	r0, #10
	bl 0x02008d1c
	adds	r0, r5, #1
	bl 0x02008d6c
	b.n	.L_020006b0
.L_020006a4:
	movs	r0, #20
	bl 0x02008d1c
	adds	r0, r5, #2
	bl 0x02008d6c
.L_020006b0:
	movs	r0, #14
	movs	r1, #0
	bl 0x02008d7c
	bl 0x02008d2c
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1829
	.2byte 0x0000
	push	{lr}
	bl 0x02008d24
	movs	r0, #0
	bl 0x02008db4
	ldr	r0, [pc, #44]
	bl 0x02008d6c
	movs	r1, #0
	movs	r0, #26
	bl 0x02008d7c
	movs	r0, #30
	bl 0x02008d1c
	movs	r1, #3
	movs	r0, #26
	bl 0x02008d54
	movs	r0, #30
	bl 0x02008d1c
	movs	r0, #26
	movs	r1, #0
	bl 0x02008d7c
	bl 0x02008d2c
	pop	{pc}
	.2byte 0x183b
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [pc, #32]
	ldr	r5, [r3, #108]
	bl 0x02008dcc
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #188
	adds	r5, r5, r3
	ldr	r1, [r5, #0]
	movs	r3, #1
	adds	r1, #35
	ldrb	r2, [r1, #0]
	orrs	r3, r2
	movs	r2, #253
	ands	r3, r2
	strb	r3, [r1, #0]
	pop	{r5, pc}
	.2byte 0x8e04
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #9
	bl 0x02008d3c
	ldr	r0, [r0, #72]
	movs	r5, #229
	mov	r8, r0
	movs	r0, #9
	bl 0x02008d3c
	ldr	r6, [r0, #40]
	movs	r0, #9
	bl 0x02008d3c
	lsls	r5, r5, #1
	ldr	r7, [r0, #12]
	bl 0x02008d24
	movs	r0, #0
	bl 0x02008db4
	adds	r0, r5, #0
	bl 0x02008d14
	cmp	r0, #0
	blt.n	.L_020007a0
	movs	r0, #9
	bl 0x02008d3c
	movs	r1, #3
	bl 0x02008da4
	movs	r0, #83
	bl 0x02008dfc
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x02008ccc
	movs	r1, #1
	ldr	r0, [pc, #232]
	bl 0x02008cc4
	adds	r0, r5, #0
	bl 0x02008cf4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x02008d4c
	movs	r1, #232
	movs	r2, #194
	b.n	.L_020007d0
.L_020007a0:
	movs	r0, #9
	bl 0x02008d3c
	movs	r1, #3
	bl 0x02008da4
	movs	r0, #83
	bl 0x02008dfc
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008878
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_020007e4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x02008d4c
	movs	r1, #232
	movs	r2, #196
.L_020007d0:
	movs	r0, #13
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x02008d4c
	movs	r0, #162
	lsls	r0, r0, #4
	bl 0x02008c8c
	b.n	.L_02000866
.L_020007e4:
	movs	r2, #0
	movs	r1, #0
	movs	r0, #9
	bl 0x02008d4c
	movs	r0, #1
	bl 0x02008d1c
	movs	r0, #9
	bl 0x02008d3c
	str	r6, [r0, #40]
	movs	r0, #9
	bl 0x02008d3c
	str	r7, [r0, #12]
	movs	r0, #9
	bl 0x02008d3c
	mov	r3, r8
	str	r3, [r0, #72]
	movs	r0, #9
	bl 0x02008d3c
	movs	r3, #0
	str	r3, [r0, #108]
	movs	r0, #9
	bl 0x02008d3c
	bl 0x02008c9c
	movs	r1, #1
	movs	r0, #9
	bl 0x02008d8c
	movs	r0, #9
	bl 0x02008d3c
	movs	r3, #2
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x02008d3c
	ldr	r5, [pc, #52]
	str	r5, [r0, #28]
	movs	r0, #9
	bl 0x02008d3c
	str	r5, [r0, #24]
	movs	r0, #9
	bl 0x02008d3c
	adds	r0, #89
	ldrb	r2, [r0, #0]
	movs	r3, #4
	orrs	r3, r2
	movs	r1, #232
	movs	r2, #196
	strb	r3, [r0, #0]
	lsls	r1, r1, #16
	movs	r0, #9
	lsls	r2, r2, #16
	bl 0x02008d4c
.L_02000866:
	bl 0x02008d2c
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x00000e11
	.2byte 0x3333
	.2byte 0x0001
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #226
	mov	sl, r3
	lsls	r2, r2, #1
	add	r2, sl
	mov	r8, r2
	movs	r3, #0
	ldrsh	r2, [r2, r3]
	sub	sp, #8
	adds	r6, r0, #0
	mov	r9, r2
	bl 0x02008cf4
	movs	r3, #1
	adds	r7, r0, #0
	negs	r3, r3
	cmp	r7, r3
	beq.n	.L_020008ac
	b.n	.L_020009e8
.L_020008ac:
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x02008ccc
	ldr	r0, [pc, #388]
	movs	r1, #1
	bl 0x02008cc4
	ldr	r0, [pc, #384]
	movs	r1, #1
	bl 0x02008cc4
.L_020008c4:
	ldr	r2, [pc, #380]
	movs	r1, #1
	mov	r8, r2
	mov	r0, r8
	bl 0x02008cc4
	add	r0, sp, #4
	mov	r1, sp
	bl 0x02008df4
	movs	r3, #1
	adds	r5, r0, #0
	negs	r3, r3
	cmp	r5, r3
	bne.n	.L_02000940
	adds	r0, r6, #0
	bl 0x02008ce4
	ldrb	r2, [r0, #3]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000900
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x02008ccc
	mov	r0, r8
	adds	r0, #4
	b.n	.L_02000970
.L_02000900:
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x02008ccc
	mov	r0, r8
	adds	r0, #1
	movs	r1, #5
	bl 0x02008cc4
	movs	r0, #1
	bl 0x02008dbc
	adds	r5, r0, #0
	bl 0x02008cd4
	cmp	r5, #0
	bne.n	.L_020008c4
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x02008ccc
	mov	r0, r8
	adds	r0, #2
	movs	r1, #1
	bl 0x02008cc4
	movs	r3, #226
	lsls	r3, r3, #1
	add	r3, sl
	mov	r2, r9
	strh	r2, [r3, #0]
	b.n	.L_02000a2c
.L_02000940:
	ldr	r0, [sp, #4]
	bl 0x02008c7c
	ldr	r1, [sp, #0]
	ldr	r0, [sp, #4]
	bl 0x02008cec
	adds	r1, r6, #0
	adds	r5, r0, #0
	ldr	r0, [sp, #4]
	bl 0x02008d0c
	cmp	r0, #29
	ble.n	.L_02000978
	ldr	r0, [sp, #4]
	movs	r1, #1
	bl 0x02008ccc
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x02008ccc
	mov	r0, r8
	adds	r0, #7
.L_02000970:
	movs	r1, #1
	bl 0x02008cc4
	b.n	.L_020008c4
.L_02000978:
	cmp	r5, #0
	ble.n	.L_0200098a
.L_0200097c:
	ldr	r0, [sp, #4]
	ldr	r1, [sp, #0]
	subs	r5, #1
	bl 0x02008cfc
	cmp	r5, #0
	bne.n	.L_0200097c
.L_0200098a:
	ldr	r0, [sp, #4]
	bl 0x02008d04
	ldr	r0, [sp, #4]
	bl 0x02008cdc
	adds	r0, r6, #0
	bl 0x02008cf4
	adds	r7, r0, #0
	movs	r0, #83
	bl 0x02008dfc
	ldr	r3, [pc, #160]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	cmp	r7, r3
	bne.n	.L_020009c4
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x02008ccc
	ldr	r0, [pc, #128]
	movs	r1, #3
	bl 0x02008cc4
	b.n	.L_020009dc
.L_020009c4:
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x02008ccc
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x02008ccc
	ldr	r0, [pc, #116]
	movs	r1, #3
	bl 0x02008cc4
.L_020009dc:
	movs	r3, #226
	lsls	r3, r3, #1
	add	r3, sl
	mov	r2, r9
	strh	r2, [r3, #0]
	b.n	.L_02000a2c
.L_020009e8:
	movs	r0, #83
	bl 0x02008dfc
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x02008ccc
	ldr	r5, [pc, #68]
	movs	r1, #3
	adds	r0, r5, #0
	bl 0x02008cc4
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	cmp	r7, r3
	beq.n	.L_02000a26
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x02008ccc
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x02008ccc
	adds	r0, r5, #1
	movs	r1, #1
	bl 0x02008cc4
.L_02000a26:
	mov	r3, r9
	mov	r2, r8
	strh	r3, [r2, #0]
.L_02000a2c:
	adds	r0, r7, #0
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00000e11
	.4byte 0x00000e20
	.4byte 0x00000e21
	.4byte 0x02000240
	.2byte 0x0e12
	.2byte 0x0000
	.global Func_02000a50
	.thumb_func
Func_02000a50:
	push	{r5, r6, lr}
	movs	r1, #192
	lsls	r1, r1, #18
	ldr	r3, [r1, #108]
	movs	r0, #214
	movs	r2, #133
	lsls	r0, r0, #1
	lsls	r2, r2, #1
	adds	r3, r3, r0
	adds	r2, #255
	ldr	r5, [pc, #400]
	str	r2, [r3, #0]
	subs	r2, #41
	adds	r3, r5, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #392]
	cmp	r2, r3
	bne.n	.L_02000b0c
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #7
	bne.n	.L_02000a8a
	bl 0x02008c58
	b.n	.L_02000bf2
.L_02000a8a:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #115
	bl 0x02008c84
	cmp	r0, #0
	beq.n	.L_02000ac0
	movs	r1, #5
	movs	r0, #24
	bl 0x02008d54
	movs	r0, #24
	bl 0x02008d3c
	movs	r1, #0
	bl 0x02008cb4
	movs	r1, #5
	movs	r0, #25
	bl 0x02008d54
	movs	r0, #25
	bl 0x02008d3c
	movs	r1, #0
	bl 0x02008cb4
.L_02000ac0:
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x02008d3c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #32
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #13
	bl 0x02008d3c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #19
	bl 0x02008d3c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r1, #2
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #19
	bl 0x02008d8c
	movs	r0, #19
	bl 0x02008d3c
	adds	r0, #89
	ldrb	r2, [r0, #0]
	movs	r3, #4
	orrs	r3, r2
	strb	r3, [r0, #0]
	b.n	.L_02000bf2
.L_02000b0c:
	ldr	r3, [pc, #240]
	cmp	r2, r3
	bne.n	.L_02000bf2
	ldr	r6, [r1, #32]
	bl 0x02008d24
	movs	r0, #0
	bl 0x02008db4
	movs	r0, #13
	bl 0x02008d3c
	movs	r1, #0
	bl 0x02008cb4
	movs	r0, #0
	bl 0x02008dac
	movs	r0, #162
	lsls	r0, r0, #4
	bl 0x02008c84
	cmp	r0, #0
	bne.n	.L_02000b90
	movs	r0, #9
	bl 0x02008d3c
	movs	r1, #229
	lsls	r1, r1, #1
	bl 0x02008cbc
	movs	r0, #9
	bl 0x02008d3c
	movs	r1, #0
	bl 0x02008cb4
	movs	r1, #1
	movs	r0, #9
	bl 0x02008d8c
	movs	r0, #9
	bl 0x02008d3c
	ldr	r5, [pc, #156]
	str	r5, [r0, #28]
	movs	r0, #9
	bl 0x02008d3c
	str	r5, [r0, #24]
	movs	r0, #9
	bl 0x02008d3c
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r5, #4
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #13
	bl 0x02008d3c
	adds	r0, #89
	ldrb	r3, [r0, #0]
	orrs	r5, r3
	strb	r5, [r0, #0]
	b.n	.L_02000bac
.L_02000b90:
	movs	r0, #9
	bl 0x02008d3c
	adds	r0, #89
	ldrb	r2, [r0, #0]
	movs	r3, #4
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x02008d3c
	movs	r1, #0
	bl 0x02008cb4
.L_02000bac:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #115
	bl 0x02008c84
	cmp	r0, #0
	bne.n	.L_02000be8
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x02008d4c
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x02008d4c
	movs	r2, #0
	movs	r0, #12
	movs	r1, #0
	bl 0x02008d4c
	ldrb	r3, [r6, #23]
	movs	r2, #13
	negs	r2, r2
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	strb	r2, [r6, #23]
	b.n	.L_02000bee
.L_02000be8:
	ldr	r0, [pc, #28]
	bl 0x02008dc4
.L_02000bee:
	bl 0x02008d2c
.L_02000bf2:
	movs	r0, #0
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000000b
	.4byte 0x0000000c
	.4byte 0x00013333
	.2byte 0x8e04
	.2byte 0x0200
	.global Func_02000c0c
	.thumb_func
Func_02000c0c:
	push	{lr}
	ldr	r3, [pc, #64]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #56]
	sub	sp, #8
	cmp	r2, r3
	bne.n	.L_02000c4a
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #115
	bl 0x02008c84
	cmp	r0, #0
	beq.n	.L_02000c4a
	movs	r0, #0
	bl 0x02008ca4
	movs	r3, #10
	movs	r2, #12
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #29
	movs	r1, #33
	movs	r2, #10
	movs	r3, #6
	bl 0x02008cac
.L_02000c4a:
	movs	r0, #0
	add	sp, #8
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x000c
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r0, #48
	adds	r2, #93
	str	r2, [r3, #0]
	adds	r0, #255
	bl 0x02008c94
	pop	{pc}
	push	{lr}
	bl 0x02008d9c
	pop	{pc}
	.section .rodata.x02008e04,"a",%progbits
	.4byte 0x0220000a
	.4byte 0x0221000b
	.4byte 0x0222000c
	.4byte 0x0000ffff
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000000a
	.4byte 0x00020000
	.4byte 0x00020000
	.4byte 0x00060000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000b
	.4byte 0x1010100a
	.4byte 0xffffffff
	.4byte 0x1020200a
	.4byte 0xffffffff
	.4byte 0x1030300a
	.4byte 0xffffffff
	.4byte 0x1040400a
	.4byte 0xffffffff
	.4byte 0x1050500a
	.4byte 0xffffffff
	.4byte 0x1060600a
	.4byte 0xffffffff
	.4byte 0x10703085
	.4byte 0xffffffff
	.4byte 0x0000000c
	.4byte 0x1010700a
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x0000000b
	.4byte 0x1010100d
	.4byte 0xffffffff
	.4byte 0x1020200d
	.4byte 0xffffffff
	.4byte 0x1030300d
	.4byte 0xffffffff
	.4byte 0x1040400d
	.4byte 0xffffffff
	.4byte 0x1050500d
	.4byte 0xffffffff
	.4byte 0x1060600d
	.4byte 0xffffffff
	.4byte 0x10703085
	.4byte 0xffffffff
	.4byte 0x0000000c
	.4byte 0x1010700d
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00008000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x02008e14
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00018000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00012000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0xffff0071
	.4byte 0x00000002
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00004000
	.4byte 0xffff0072
	.4byte 0x00000002
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x0001e000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00016000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00014000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02d40000
	.4byte 0x00014000
	.4byte 0xffff0086
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01d40000
	.4byte 0x00000000
	.4byte 0xffff0087
	.4byte 0x00000002
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0000a000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00012000
	.4byte 0xffff004a
	.4byte 0x00000003
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x02ea0000
	.4byte 0x0001c000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00014000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00018000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x014d0000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00024000
	.4byte 0x02230122
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0x02240122
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0x02250122
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0000a000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x02008e14
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00018000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00012000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0xffff0071
	.4byte 0x00000002
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00004000
	.4byte 0xffff0072
	.4byte 0x00000002
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0001e000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00016000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00014000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02d40000
	.4byte 0x00014000
	.4byte 0xffff0086
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01d40000
	.4byte 0x00000000
	.4byte 0xffff0087
	.4byte 0x02008ecc
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00004000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0000a000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00012000
	.4byte 0xffff0012
	.4byte 0x00000001
	.4byte 0x00a10000
	.4byte 0x00000000
	.4byte 0x006f0000
	.4byte 0x00014000
	.4byte 0xffff0011
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x02008ecc
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000003
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x02ea0000
	.4byte 0x0001c000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00014000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00018000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02d80000
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
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001733
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001734
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001737
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001738
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000173b
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000173c
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000173f
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001740
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001741
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001742
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008271
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x020082d1
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x02008331
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x0000174e
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x0000174f
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001750
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x0000219c
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x0000219d
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x0000219e
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x020082d1
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001735
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001736
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001739
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000173a
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000173d
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000173e
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001743
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001744
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001745
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001746
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001748
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x0000174a
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001751
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001752
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001753
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001754
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x000021a1
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x000021a2
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x000021a3
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x020081a5
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
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008391
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001758
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
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001800
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001801
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001806
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001807
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008615
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000180f
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0200866d
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0000182c
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0000182d
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x0000182e
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x020084f5
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x02008555
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x0000183a
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x0000183d
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x0000183e
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001802
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00001808
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x020086c5
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x0000219c
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x0000219d
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x0000219e
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x020082d1
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001803
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001804
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001809
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000180a
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001810
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001811
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000182f
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001830
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001831
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001832
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001834
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001836
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x0000183f
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001840
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001842
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001843
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001805
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x0000180b
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001841
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x000021a1
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x000021a2
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte 0x000021a3
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x020081cd
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
	.4byte 0x00004602
	.4byte 0xffff001e
	.4byte 0x02008c75
	.4byte 0x00000602
	.4byte 0xffff001f
	.4byte 0x02008c75
	.4byte 0x00008602
	.4byte 0xffff0020
	.4byte 0x02008c75
	.4byte 0x0000c602
	.4byte 0xffff0021
	.4byte 0x02008c75
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008499
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001845
	.4byte 0x00001815
	.4byte 0x0220000a
	.4byte 0x02008705
	.4byte 0x00001815
	.4byte 0x0221000b
	.4byte 0x02008705
	.4byte 0x00001815
	.4byte 0x0222000c
	.4byte 0x02008705
	.4byte 0x00000003
	.4byte 0x0a200028
	.4byte 0x02008731
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
