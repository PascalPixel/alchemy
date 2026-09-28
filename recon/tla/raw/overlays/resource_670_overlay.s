.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x0200d035, 0x020080f5, 0x02008125, 0x0200812d, 0x020081b5, 0x020080fd, 0x0200d0a9
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x21c06d02
	.4byte 0x00c98a53
	.4byte 0x8253185b
	.4byte 0x47702000
	.4byte 0x64436883
	.4byte 0x648368c3
	.2byte 0x2000
	.2byte 0x4770
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #64]
	movs	r2, #1
	ldr	r3, [r3, #0]
	adds	r7, r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000094
	bl 0x0200d9c4
	lsls	r3, r0, #1
	ldr	r5, [pc, #48]
	adds	r3, r3, r0
	lsrs	r3, r3, #16
	lsls	r3, r3, #16
	adds	r6, r5, #0
	adds	r5, r3, r5
	bl 0x0200d9c4
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsrs	r3, r3, #16
	ldr	r1, [r7, #68]
	ldr	r2, [r7, #72]
	lsls	r3, r3, #16
	adds	r6, r3, r6
	adds	r1, r1, r5
	adds	r2, r2, r6
	ldr	r3, [r7, #16]
	adds	r0, r7, #0
	bl 0x0200da44
.L_02000094:
	movs	r0, #1
	pop	{r5, r6, r7, pc}
	.4byte 0x0300122c
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	ldr	r3, [pc, #28]
	movs	r2, #2
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020000b6
	movs	r1, #10
	bl 0x0200db44
	b.n	.L_020000bc
.L_020000b6:
	movs	r1, #0
	bl 0x0200db44
.L_020000bc:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, lr}
	adds	r6, r0, #0
	adds	r5, r6, #0
	adds	r5, #98
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_020000da
	adds	r3, #255
	strb	r3, [r5, #0]
	b.n	.L_020000f0
.L_020000da:
	bl 0x0200d9c4
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	lsrs	r3, r3, #16
	adds	r3, #10
	strb	r3, [r5, #0]
	bl 0x0200d9c4
	strh	r0, [r6, #6]
.L_020000f0:
	movs	r0, #1
	pop	{r5, r6, pc}
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xe32c
	.2byte 0x0200
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
	bne.n	.L_02000114
	ldr	r0, [pc, #12]
.L_02000114:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000070
	.2byte 0xe35c
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xe39c
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #84]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_02000144
	ldr	r0, [pc, #72]
	b.n	.L_02000182
.L_02000144:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_0200014e
	ldr	r0, [pc, #72]
	b.n	.L_02000182
.L_0200014e:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_02000158
	ldr	r0, [pc, #68]
	b.n	.L_02000182
.L_02000158:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02000162
	ldr	r0, [pc, #68]
	b.n	.L_02000182
.L_02000162:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02000180
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #238
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_0200017c
	ldr	r2, [pc, #52]
	movs	r3, #0
	strb	r3, [r2, #22]
.L_0200017c:
	ldr	r0, [pc, #44]
	b.n	.L_02000182
.L_02000180:
	ldr	r0, [pc, #44]
.L_02000182:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000006e
	.4byte 0x0200e510
	.4byte 0x00000071
	.4byte 0x0200e6d8
	.4byte 0x0000006f
	.4byte 0x0200e9a8
	.4byte 0x00000072
	.4byte 0x0200ea98
	.4byte 0x00000070
	.4byte 0x0200ecd8
	.2byte 0xe4f8
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #64]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_020001cc
	ldr	r0, [pc, #52]
	b.n	.L_020001f6
.L_020001cc:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_020001d6
	ldr	r0, [pc, #52]
	b.n	.L_020001f6
.L_020001d6:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_020001e0
	ldr	r0, [pc, #48]
	b.n	.L_020001f6
.L_020001e0:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_020001ea
	ldr	r0, [pc, #48]
	b.n	.L_020001f6
.L_020001ea:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_020001f4
	ldr	r0, [pc, #44]
	b.n	.L_020001f6
.L_020001f4:
	ldr	r0, [pc, #44]
.L_020001f6:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000006e
	.4byte 0x0200eef4
	.4byte 0x00000071
	.4byte 0x0200f0d4
	.4byte 0x0000006f
	.4byte 0x0200f434
	.4byte 0x00000072
	.4byte 0x0200f4dc
	.4byte 0x00000070
	.4byte 0x0200f698
	.2byte 0xeee8
	.2byte 0x0200
	push	{lr}
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	ldr	r0, [pc, #16]
	bl 0x0200db4c
	movs	r1, #0
	movs	r0, #15
	bl 0x0200db6c
	bl 0x0200daa4
	pop	{pc}
	.2byte 0x2002
	.2byte 0x0000
	push	{lr}
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	ldr	r0, [pc, #16]
	bl 0x0200db4c
	movs	r1, #0
	movs	r0, #23
	bl 0x0200db6c
	bl 0x0200daa4
	pop	{pc}
	.2byte 0x200b
	.2byte 0x0000
	push	{lr}
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	ldr	r0, [pc, #60]
	bl 0x0200db4c
	movs	r0, #24
	movs	r1, #0
	bl 0x0200db64
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #24
	bl 0x0200db3c
	movs	r0, #20
	bl 0x0200da94
	movs	r0, #24
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #192
	movs	r0, #24
	lsls	r1, r1, #6
	bl 0x0200db7c
	bl 0x0200daa4
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0000200f
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	ldr	r0, [pc, #16]
	bl 0x0200db4c
	movs	r1, #0
	movs	r0, #12
	bl 0x0200db6c
	bl 0x0200daa4
	pop	{pc}
	.2byte 0x201d
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200dac4
	movs	r2, #190
	ldrh	r3, [r0, #6]
	lsls	r2, r2, #7
	adds	r2, #255
	adds	r3, r3, r2
	ldr	r2, [pc, #16]
	lsls	r3, r3, #16
	movs	r0, #1
	cmp	r3, r2
	bls.n	.L_0200030e
	movs	r0, #0
.L_0200030e:
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0x3ffe
	push	{lr}
	bl 0x020082e8
	cmp	r0, #0
	beq.n	.L_0200032c
	movs	r0, #17
	movs	r1, #20
	bl 0x0200dc84
	b.n	.L_02000348
.L_0200032c:
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	ldr	r0, [pc, #20]
	bl 0x0200db4c
	movs	r0, #20
	movs	r1, #0
	bl 0x0200db64
	bl 0x0200daa4
.L_02000348:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2030
	.2byte 0x0000
	push	{lr}
	bl 0x020082e8
	cmp	r0, #0
	beq.n	.L_02000364
	movs	r0, #18
	movs	r1, #21
	bl 0x0200dc84
	b.n	.L_02000380
.L_02000364:
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	ldr	r0, [pc, #20]
	bl 0x0200db4c
	movs	r0, #21
	movs	r1, #0
	bl 0x0200db64
	bl 0x0200daa4
.L_02000380:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2032
	.2byte 0x0000
	push	{lr}
	bl 0x020082e8
	cmp	r0, #0
	beq.n	.L_0200039c
	movs	r0, #19
	movs	r1, #22
	bl 0x0200dc84
	b.n	.L_020003b8
.L_0200039c:
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	ldr	r0, [pc, #20]
	bl 0x0200db4c
	movs	r0, #22
	movs	r1, #0
	bl 0x0200db64
	bl 0x0200daa4
.L_020003b8:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2034
	.2byte 0x0000
	push	{lr}
	bl 0x020082e8
	cmp	r0, #0
	beq.n	.L_020003d4
	movs	r0, #6
	movs	r1, #23
	bl 0x0200dc94
	b.n	.L_020003f0
.L_020003d4:
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	ldr	r0, [pc, #20]
	bl 0x0200db4c
	movs	r0, #23
	movs	r1, #0
	bl 0x0200db64
	bl 0x0200daa4
.L_020003f0:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2036
	.2byte 0x0000
	push	{lr}
	bl 0x020082e8
	cmp	r0, #0
	beq.n	.L_0200040a
	movs	r0, #25
	bl 0x0200dc8c
	b.n	.L_02000426
.L_0200040a:
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	ldr	r0, [pc, #16]
	bl 0x0200db4c
	movs	r0, #25
	movs	r1, #0
	bl 0x0200db64
	bl 0x0200daa4
.L_02000426:
	pop	{pc}
	.2byte 0x203a
	.2byte 0x0000
	push	{lr}
	movs	r0, #142
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_02000476
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	ldr	r3, [pc, #140]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #36
	bl 0x0200db3c
	movs	r0, #10
	bl 0x0200da94
	movs	r0, #36
	bl 0x0200dc8c
	movs	r1, #176
	movs	r0, #36
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	bl 0x0200daa4
	b.n	.L_020004d0
.L_02000476:
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	ldr	r0, [pc, #84]
	bl 0x0200db4c
	movs	r2, #40
	movs	r0, #36
	movs	r1, #0
	bl 0x0200db5c
	movs	r0, #36
	movs	r1, #0
	bl 0x0200db64
	ldr	r3, [pc, #56]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #36
	bl 0x0200db3c
	movs	r0, #10
	bl 0x0200da94
	movs	r0, #36
	bl 0x0200dc8c
	movs	r1, #176
	movs	r0, #36
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r0, #142
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200da14
	bl 0x0200daa4
.L_020004d0:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x1efc
	.2byte 0x0000
	push	{lr}
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	ldr	r0, [pc, #16]
	bl 0x0200db4c
	movs	r0, #16
	movs	r1, #0
	bl 0x0200db64
	bl 0x0200daa4
	pop	{pc}
	.2byte 0x2028
	.2byte 0x0000
	push	{lr}
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	ldr	r0, [pc, #16]
	bl 0x0200db4c
	movs	r0, #16
	movs	r1, #0
	bl 0x0200db64
	bl 0x0200daa4
	pop	{pc}
	.2byte 0x202a
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #48]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #40]
	cmp	r2, r3
	beq.n	.L_0200053e
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_02000548
.L_0200053e:
	movs	r1, #15
	movs	r2, #2
	bl 0x02008590
	b.n	.L_02000556
.L_02000548:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_02000556
	movs	r1, #26
	movs	r2, #2
	bl 0x02008590
.L_02000556:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000006f
	.4byte 0x00000072
	.2byte 0x0070
	.2byte 0x0000
	push	{lr}
	movs	r0, #1
	bl 0x02008524
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200da14
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #0
	bl 0x02008524
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200da1c
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r2, #0
	movs	r5, #0
	mov	r8, r0
	adds	r7, r1, #0
	cmp	r5, r6
	bcs.n	.L_020005c8
.L_020005a2:
	adds	r0, r7, r5
	bl 0x0200dac4
	mov	r3, r8
	adds	r0, #35
	adds	r1, r5, #1
	cmp	r3, #0
	beq.n	.L_020005ba
	ldrb	r2, [r0, #0]
	movs	r3, #239
	ands	r3, r2
	b.n	.L_020005c0
.L_020005ba:
	ldrb	r2, [r0, #0]
	movs	r3, #16
	orrs	r3, r2
.L_020005c0:
	strb	r3, [r0, #0]
	adds	r5, r1, #0
	cmp	r5, r6
	bcc.n	.L_020005a2
.L_020005c8:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200dc6c
	pop	{pc}
	.2byte 0x0000
	.2byte 0xe324
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #34
	movs	r1, #33
	movs	r2, #23
	movs	r3, #18
	bl 0x0200da64
	movs	r3, #23
	movs	r2, #18
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #33
	movs	r2, #1
	movs	r3, #1
.L_02000604:
	movs	r0, #34
	bl 0x0200da6c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #106
	bl 0x0200da14
	add	sp, #8
	pop	{pc}
	push	{lr}
	cmp	r0, #1
	bne.n	.L_02000622
	bl 0x020085e0
.L_02000622:
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #36
	movs	r1, #34
	movs	r2, #16
	movs	r3, #7
	bl 0x0200da64
	movs	r3, #16
.L_0200063c:
	movs	r2, #7
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #34
	movs	r2, #1
	movs	r3, #1
	movs	r0, #36
	bl 0x0200da6c
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x0200da14
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	cmp	r0, #1
	bne.n	.L_02000666
	bl 0x02008624
.L_02000666:
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #38
.L_02000674:
	movs	r1, #34
	movs	r2, #16
	movs	r3, #11
	bl 0x0200da64
	movs	r3, #16
	movs	r2, #11
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #34
	movs	r2, #1
	movs	r3, #1
	movs	r0, #38
	bl 0x0200da6c
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200da14
	add	sp, #8
	pop	{pc}
	push	{lr}
	cmp	r0, #1
	bne.n	.L_020006aa
	bl 0x02008668
.L_020006aa:
	pop	{pc}
.L_020006ac:
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #36
	movs	r1, #36
	movs	r2, #14
	movs	r3, #9
	bl 0x0200da64
	movs	r3, #14
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #36
	movs	r2, #1
	movs	r3, #1
	movs	r0, #36
	bl 0x0200da6c
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x0200da14
	add	sp, #8
	pop	{pc}
.L_020006e4:
	push	{lr}
	cmp	r0, #1
	bne.n	.L_020006ee
	bl 0x020086ac
.L_020006ee:
	pop	{pc}
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #38
	movs	r1, #36
	movs	r2, #18
	movs	r3, #9
	bl 0x0200da64
	movs	r3, #18
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #36
	movs	r2, #1
	movs	r3, #1
	movs	r0, #38
	bl 0x0200da6c
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200da14
	add	sp, #8
	pop	{pc}
	push	{lr}
	cmp	r0, #1
	bne.n	.L_02000732
	bl 0x020086f0
.L_02000732:
	pop	{pc}
	push	{r5, r6, r7, lr}
	movs	r0, #11
	sub	sp, #8
	bl 0x0200dac4
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	asrs	r3, r3, #20
	cmp	r3, #6
	bne.n	.L_020007c4
	ldr	r3, [r6, #16]
	asrs	r3, r3, #20
	cmp	r3, #18
	bne.n	.L_020007c4
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	movs	r0, #11
	bl 0x0200dac4
	adds	r7, r6, #0
	movs	r3, #3
	adds	r0, #85
	adds	r7, #85
	strb	r3, [r0, #0]
	strb	r3, [r7, #0]
.L_0200076c:
	movs	r0, #1
	bl 0x0200d9ac
	ldr	r5, [r6, #40]
	cmp	r5, #0
	bne.n	.L_0200076c
	movs	r0, #188
	bl 0x0200dc9c
	movs	r0, #10
	bl 0x0200d9ac
	strb	r5, [r7, #0]
	movs	r0, #11
	bl 0x0200dac4
	movs	r3, #6
	adds	r0, #85
	movs	r2, #18
	strb	r5, [r0, #0]
	movs	r1, #18
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #5
	movs	r2, #1
	movs	r3, #1
	bl 0x0200da6c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #234
	bl 0x0200da14
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #244
	bl 0x0200da0c
	cmp	r0, #0
	bne.n	.L_020007c0
	bl 0x02009318
.L_020007c0:
	bl 0x0200daa4
.L_020007c4:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	negs	r1, r1
	movs	r3, #0
	negs	r0, r0
	bl 0x0200dbac
	bl 0x0200dbbc
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #152
	movs	r0, #153
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #153
	adds	r1, #51
	bl 0x0200dba4
	movs	r0, #176
	movs	r1, #1
	movs	r2, #196
	lsls	r0, r0, #15
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x0200dbac
	ldr	r5, [pc, #328]
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
	bl 0x0200dacc
	movs	r2, #196
	ldr	r0, [r5, #0]
	movs	r1, #72
	lsls	r2, r2, #1
	bl 0x0200dafc
	movs	r2, #196
	ldr	r0, [r5, #0]
	movs	r1, #88
	lsls	r2, r2, #1
	bl 0x0200dafc
	movs	r1, #192
	movs	r2, #10
	lsls	r1, r1, #7
	movs	r0, #7
	bl 0x0200db74
	ldr	r0, [pc, #268]
	bl 0x0200db4c
	movs	r0, #7
	movs	r1, #0
	bl 0x0200db64
	ldr	r1, [r5, #0]
	movs	r0, #26
	bl 0x0200db1c
	ldr	r1, [r5, #0]
	movs	r0, #5
	bl 0x0200db1c
	ldr	r1, [r5, #0]
	movs	r0, #6
	bl 0x0200db1c
	movs	r0, #1
	bl 0x0200d9ac
	movs	r0, #26
	movs	r1, #2
	bl 0x0200db84
	movs	r0, #5
	movs	r1, #2
	bl 0x0200db84
	movs	r0, #6
	movs	r1, #2
	bl 0x0200db84
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #26
	adds	r1, #204
	adds	r2, #102
	bl 0x0200dacc
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #5
	adds	r1, #204
	adds	r2, #102
	bl 0x0200dacc
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #102
	movs	r0, #6
	adds	r1, #204
	bl 0x0200dacc
	ldr	r1, [pc, #152]
	movs	r0, #26
	bl 0x0200dad4
	ldr	r1, [pc, #148]
	movs	r0, #5
	bl 0x0200dad4
	ldr	r1, [pc, #144]
	movs	r0, #6
	bl 0x0200dae4
	movs	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #17
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_020008fe
	bl 0x02008978
	b.n	.L_02000902
.L_020008fe:
	bl 0x02008b2c
.L_02000902:
	bl 0x02008d74
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #26
	ldr	r1, [pc, #96]
	adds	r2, #204
	bl 0x0200dacc
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #84]
	adds	r2, #204
	bl 0x0200dacc
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #6
	ldr	r1, [pc, #68]
	bl 0x0200dacc
	ldr	r5, [pc, #64]
	movs	r0, #5
	adds	r1, r5, #0
	bl 0x0200dad4
	adds	r1, r5, #0
	movs	r0, #6
	bl 0x0200dad4
	adds	r1, r5, #0
	movs	r0, #26
	bl 0x0200dae4
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #243
	bl 0x0200da14
	bl 0x0200daa4
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00001e6a
	.4byte 0x0200dd18
	.4byte 0x0200dd5c
	.4byte 0x0200dda0
	.4byte 0x00019999
	.2byte 0xde54
	.2byte 0x0200
	push	{r5, lr}
	ldr	r5, [pc, #424]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #20
	ldr	r0, [r5, #0]
	bl 0x0200db74
	ldr	r0, [pc, #408]
	bl 0x0200db4c
	movs	r1, #0
	movs	r0, #6
	bl 0x0200db54
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200dabc
	cmp	r0, #0
	bne.n	.L_020009d8
	movs	r1, #128
	movs	r2, #10
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200db74
	movs	r0, #5
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #5
	movs	r1, #0
	bl 0x0200db64
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02000a06
.L_020009d8:
	movs	r1, #128
	movs	r2, #10
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200db74
	movs	r0, #5
	movs	r1, #4
	bl 0x0200db24
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #5
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x0200db64
.L_02000a06:
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #7
	bl 0x0200db94
	movs	r0, #7
	movs	r1, #0
	bl 0x0200db64
	ldr	r5, [pc, #264]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #176
	movs	r0, #26
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200db74
	movs	r1, #224
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #208
	movs	r0, #26
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #224
	movs	r2, #20
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #26
	bl 0x0200db94
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #6
.L_02000aa6:
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #40
	bl 0x0200db74
	movs	r2, #0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200db74
	movs	r0, #5
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #4
.L_02000ad4:
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x0200db94
	movs	r0, #6
	movs	r1, #2
	movs	r2, #10
.L_02000ae4:
	bl 0x0200db34
	movs	r2, #10
	movs	r0, #6
	movs	r1, #4
	bl 0x0200db34
	movs	r0, #6
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #4
	movs	r2, #10
	adds	r1, #255
	movs	r0, #5
	bl 0x0200db94
	movs	r0, #5
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #4
	adds	r1, #255
	movs	r2, #10
	movs	r0, #7
	bl 0x0200db94
	movs	r0, #7
	movs	r1, #0
	bl 0x0200db64
	pop	{r5, pc}
	.4byte 0x02000240
	.2byte 0x1e6c
	.2byte 0x0000
	push	{r5, lr}
	ldr	r0, [pc, #344]
	bl 0x0200db4c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r2, #10
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200db74
	movs	r0, #5
	movs	r1, #4
	bl 0x0200db24
	movs	r0, #5
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #7
	bl 0x0200db94
	movs	r0, #7
	movs	r1, #0
	bl 0x0200db64
	ldr	r5, [pc, #284]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #176
	movs	r0, #26
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200db74
	movs	r1, #224
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #208
	movs	r0, #26
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #224
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200db9c
	movs	r0, #20
	bl 0x0200da94
	movs	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r2, #20
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db5c
	movs	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #10
	movs	r2, #0
.L_02000c00:
	adds	r1, #255
	movs	r0, #6
	bl 0x0200db94
	movs	r0, #6
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #40
	bl 0x0200db74
	movs	r2, #0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200db74
	movs	r0, #5
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #4
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x0200db94
	movs	r0, #6
	movs	r1, #2
	movs	r2, #10
	bl 0x0200db34
	movs	r2, #10
	movs	r0, #6
	movs	r1, #4
	bl 0x0200db34
	movs	r0, #6
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #4
	movs	r2, #10
	adds	r1, #255
	movs	r0, #5
	bl 0x0200db94
	movs	r0, #5
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #4
	adds	r1, #255
	movs	r2, #10
	movs	r0, #7
	bl 0x0200db94
	movs	r0, #7
	movs	r1, #0
	bl 0x0200db64
	pop	{r5, pc}
	.4byte 0x00001e8c
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	ldr	r5, [pc, #196]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r2, #0
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200db74
	movs	r1, #152
	lsls	r1, r1, #7
	ldr	r0, [pc, #180]
	adds	r1, #204
	bl 0x0200dba4
	movs	r0, #140
	movs	r1, #1
	movs	r2, #176
	movs	r3, #1
	lsls	r2, r2, #15
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x0200dbac
	bl 0x0200dbb4
	movs	r0, #20
	bl 0x0200da94
	ldr	r0, [pc, #148]
	bl 0x0200db4c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200db7c
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200db2c
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #7
	ldr	r1, [pc, #100]
	adds	r2, #204
	bl 0x0200dacc
.L_02000d10:
	movs	r0, #7
	movs	r1, #2
	bl 0x0200db24
	ldr	r0, [r5, #0]
	bl 0x0200dac4
	cmp	r0, #0
	beq.n	.L_02000d30
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x0200daec
.L_02000d30:
	movs	r0, #7
	bl 0x0200db04
	movs	r2, #0
	movs	r1, #0
	movs	r0, #7
	bl 0x0200db0c
	movs	r0, #131
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200da1c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #46
	bl 0x0200da14
	movs	r0, #7
	movs	r1, #0
	bl 0x0200daac
	bl 0x0200daa4
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00026666
	.4byte 0x00001efb
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, r6, lr}
	bl 0x0200b2c8
	movs	r0, #0
	bl 0x0200dc9c
	movs	r0, #17
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	ldr	r3, [pc, #1008]
	movs	r1, #133
	lsls	r1, r1, #2
.L_02000d98:
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r0, #26
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r2, #10
	movs	r0, #6
	movs	r1, #0
	bl 0x0200db74
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #960]
	adds	r1, #153
	bl 0x0200dba4
	movs	r0, #132
	movs	r1, #1
	movs	r2, #194
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200dbac
	bl 0x0200dbb4
	movs	r0, #40
	bl 0x0200da94
	movs	r1, #192
	movs	r2, #20
	movs	r0, #17
	lsls	r1, r1, #6
	bl 0x0200db74
	movs	r0, #17
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #17
	bl 0x0200db64
	movs	r0, #26
	bl 0x0200dc9c
	movs	r0, #27
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	movs	r0, #27
	bl 0x0200dac4
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #27
	bl 0x0200dac4
	movs	r2, #192
	movs	r3, #172
	lsls	r2, r2, #13
	lsls	r3, r3, #17
	ldr	r1, [pc, #852]
	bl 0x0200da44
	movs	r0, #204
	movs	r1, #192
	lsls	r0, r0, #7
	lsls	r1, r1, #4
	adds	r0, #102
	adds	r1, #204
	bl 0x0200dba4
	movs	r0, #132
	movs	r1, #1
	movs	r2, #170
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200dbac
	movs	r1, #204
	movs	r2, #200
	lsls	r1, r1, #6
	lsls	r2, r2, #5
	movs	r0, #17
	adds	r1, #51
	adds	r2, #153
	bl 0x0200dacc
	movs	r1, #132
	movs	r2, #187
	lsls	r2, r2, #1
	movs	r0, #17
	lsls	r1, r1, #1
	bl 0x0200dafc
	ldr	r6, [pc, #784]
	movs	r1, #144
	lsls	r1, r1, #3
	adds	r0, r6, #0
	bl 0x0200d9b4
	movs	r0, #20
	bl 0x0200da94
	ldr	r5, [pc, #768]
	adds	r0, r5, #0
	bl 0x0200d9bc
	bl 0x0200b2e0
	movs	r1, #144
	lsls	r1, r1, #3
	adds	r0, r5, #0
	bl 0x0200d9b4
	movs	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #0
	movs	r0, #17
	bl 0x0200db64
	movs	r0, #11
	bl 0x0200dc9c
	bl 0x0200b610
	movs	r0, #160
	bl 0x0200da94
	movs	r2, #80
	movs	r1, #0
	movs	r0, #17
	bl 0x0200db5c
	movs	r0, #144
	lsls	r0, r0, #2
	bl 0x0200dc9c
	movs	r1, #1
	movs	r0, #1
	bl 0x0200b640
	movs	r0, #20
	bl 0x0200da94
	movs	r2, #80
	movs	r1, #0
	movs	r0, #17
	bl 0x0200db5c
	movs	r0, #161
	bl 0x0200dc9c
	movs	r1, #1
	movs	r0, #0
	bl 0x0200b640
	movs	r0, #20
	bl 0x0200da94
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #17
	bl 0x0200db9c
	movs	r0, #20
	bl 0x0200da94
	movs	r1, #0
	movs	r2, #20
	movs	r0, #17
	bl 0x0200db5c
	adds	r0, r6, #0
	bl 0x0200d9bc
	movs	r0, #1
	bl 0x0200d9ac
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db0c
	movs	r1, #208
	movs	r0, #17
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	ldr	r3, [pc, #596]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02000f7c
	ldr	r3, [r3, #12]
	movs	r2, #144
	lsls	r2, r2, #14
	cmp	r3, r2
	ble.n	.L_02000f7c
.L_02000f52:
	movs	r1, #208
	movs	r0, #17
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	ldr	r5, [pc, #568]
	ldr	r1, [pc, #568]
	ldr	r2, [r5, #0]
	movs	r0, #1
	ldr	r3, [r2, #12]
	adds	r3, r3, r1
	str	r3, [r2, #12]
	bl 0x0200d9ac
	ldr	r3, [r5, #0]
	movs	r2, #144
	ldr	r3, [r3, #12]
	lsls	r2, r2, #14
	cmp	r3, r2
	bgt.n	.L_02000f52
.L_02000f7c:
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #6
	bl 0x0200db7c
	movs	r0, #17
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #500]
	adds	r1, #153
	bl 0x0200dba4
	movs	r0, #132
	movs	r1, #128
	movs	r2, #210
	movs	r3, #1
	lsls	r0, r0, #17
	lsls	r1, r1, #13
	lsls	r2, r2, #17
	bl 0x0200dbac
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #17
	ldr	r1, [pc, #492]
	adds	r2, #204
	bl 0x0200dacc
	movs	r1, #132
	movs	r2, #196
	lsls	r2, r2, #1
	lsls	r1, r1, #1
	movs	r0, #17
	bl 0x0200dafc
	movs	r0, #20
	bl 0x0200da94
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r1, #0
	adds	r0, #17
	bl 0x0200db64
	movs	r0, #148
	bl 0x0200dc9c
	movs	r0, #11
	bl 0x0200dc9c
	bl 0x0200b2b0
	movs	r0, #80
	bl 0x0200da94
	movs	r0, #176
	movs	r1, #128
	movs	r2, #196
	movs	r3, #1
	lsls	r2, r2, #17
	lsls	r1, r1, #14
	lsls	r0, r0, #15
	bl 0x0200dbac
	bl 0x0200dbb4
	movs	r0, #40
	bl 0x0200da94
	movs	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #129
	movs	r0, #7
	lsls	r1, r1, #1
	bl 0x0200db9c
	movs	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #7
	movs	r1, #4
	bl 0x0200db2c
	movs	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #7
	movs	r1, #2
	bl 0x0200db84
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #7
	ldr	r1, [pc, #344]
	adds	r2, #204
	bl 0x0200dacc
	movs	r2, #193
	movs	r0, #7
	movs	r1, #152
	lsls	r2, r2, #1
	bl 0x0200dafc
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200db74
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #6
	movs	r2, #20
	bl 0x0200db74
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200db74
	movs	r1, #128
	movs	r2, #40
	movs	r0, #7
	lsls	r1, r1, #6
	bl 0x0200db74
	movs	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r2, #178
	movs	r0, #7
	movs	r1, #124
	lsls	r2, r2, #1
	bl 0x0200dafc
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200db74
	movs	r1, #160
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200db74
	movs	r1, #224
	movs	r2, #20
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #6
	bl 0x0200db74
	movs	r0, #5
	movs	r1, #0
	bl 0x0200db64
	ldr	r5, [pc, #160]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #224
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200db74
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #26
	adds	r1, #204
	adds	r2, #102
	bl 0x0200dacc
	movs	r2, #184
	lsls	r2, r2, #1
	movs	r1, #104
	movs	r0, #26
	bl 0x0200daf4
	movs	r0, #20
	bl 0x0200da94
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200dba4
	movs	r0, #222
	movs	r1, #1
	movs	r2, #178
	movs	r3, #1
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #15
	bl 0x0200dbac
	movs	r0, #26
	bl 0x0200db04
	movs	r0, #26
	movs	r1, #1
	bl 0x0200db24
	movs	r1, #208
	movs	r0, #26
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200db74
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #26
	bl 0x0200db94
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	b.n	.L_020011a4
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0004cccc
	.4byte 0x01090000
	.4byte 0x0200aeb1
	.4byte 0x0200accd
	.4byte 0x0200f950
	.4byte 0xffffc000
	.2byte 0x9999
	.2byte 0x0001
.L_020011a4:
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #160
	movs	r0, #26
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200db74
	movs	r0, #192
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #26
	bl 0x0200db54
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200dabc
	cmp	r0, #0
	bne.n	.L_020011f4
	movs	r0, #192
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #26
	bl 0x0200db64
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02001216
.L_020011f4:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #192
	adds	r3, #1
	lsls	r0, r0, #7
	strh	r3, [r2, #0]
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	bl 0x0200dc7c
.L_02001216:
	pop	{r5, r6, pc}
	push	{r5, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #46
	bl 0x0200da0c
	cmp	r0, #0
	bne.n	.L_02001230
	movs	r0, #5
	bl 0x0200dbc4
	b.n	.L_0200130a
.L_02001230:
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	ldr	r5, [pc, #208]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r1, [r5, #0]
	movs	r0, #7
	bl 0x0200db1c
	ldr	r0, [r5, #0]
	bl 0x0200dac4
	ldr	r3, [r0, #80]
	movs	r0, #7
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl 0x0200db84
	movs	r0, #1
	bl 0x0200d9ac
	movs	r1, #160
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r2, #204
.L_02001272:
	lsls	r2, r2, #8
	movs	r0, #7
	ldr	r1, [pc, #152]
	adds	r2, #204
	bl 0x0200dacc
	movs	r2, #222
	lsls	r2, r2, #1
	movs	r0, #7
	movs	r1, #62
	bl 0x0200dafc
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r0, #7
	bl 0x0200db7c
	ldr	r0, [pc, #124]
	bl 0x0200db4c
	movs	r1, #0
	movs	r0, #7
	bl 0x0200db64
	movs	r0, #7
	bl 0x0200dac4
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	movs	r2, #210
	strb	r3, [r0, #0]
	movs	r1, #56
	movs	r0, #7
	lsls	r2, r2, #1
	bl 0x0200dafc
	movs	r2, #172
	movs	r0, #7
	movs	r1, #40
	lsls	r2, r2, #1
	bl 0x0200dafc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db0c
	movs	r2, #255
	movs	r1, #80
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	bl 0x0200daf4
	movs	r0, #10
	bl 0x0200da94
	movs	r0, #5
	bl 0x0200dbc4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #46
	bl 0x0200da1c
	movs	r0, #131
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200da14
	movs	r0, #7
	bl 0x0200dab4
	bl 0x0200daa4
.L_0200130a:
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x00019999
	.2byte 0x1efa
	.2byte 0x0000
	push	{r5, lr}
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #7
	bl 0x0200db74
	ldr	r0, [pc, #1020]
	bl 0x0200db4c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	negs	r1, r1
	movs	r3, #0
	negs	r0, r0
	bl 0x0200dbac
	bl 0x0200dbbc
	movs	r1, #204
	movs	r3, #0
	adds	r0, #85
	lsls	r1, r1, #7
	strb	r3, [r0, #0]
	adds	r1, #102
	ldr	r0, [pc, #972]
	bl 0x0200dba4
	movs	r0, #192
	movs	r1, #1
	movs	r2, #170
	movs	r3, #1
	lsls	r0, r0, #15
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200dbac
	bl 0x0200dbb4
	movs	r1, #160
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	ldr	r5, [pc, #936]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #128
	movs	r2, #0
	ldr	r0, [r5, #0]
	lsls	r1, r1, #6
	bl 0x0200db74
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200db9c
	movs	r0, #20
	bl 0x0200da94
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #7
	bl 0x0200db6c
	movs	r0, #204
	movs	r1, #192
	lsls	r0, r0, #7
	lsls	r1, r1, #4
	adds	r0, #102
	adds	r1, #204
	bl 0x0200dba4
	movs	r0, #192
	movs	r1, #1
	movs	r2, #186
	movs	r3, #1
	lsls	r0, r0, #15
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200dbac
	movs	r2, #153
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #852]
	adds	r2, #153
	bl 0x0200dacc
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #7
	ldr	r1, [pc, #836]
	bl 0x0200dacc
	ldr	r1, [pc, #832]
	movs	r0, #7
	bl 0x0200dad4
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #828]
	bl 0x0200dae4
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #26
	adds	r1, #204
	adds	r2, #102
	bl 0x0200dacc
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #5
	adds	r1, #204
	adds	r2, #102
	bl 0x0200dacc
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #6
	adds	r1, #204
	adds	r2, #102
	bl 0x0200dacc
	movs	r1, #224
	movs	r2, #186
	movs	r0, #26
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	bl 0x0200db0c
	movs	r1, #224
	movs	r2, #186
	movs	r0, #5
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	bl 0x0200db0c
	movs	r1, #224
	movs	r2, #186
	lsls	r2, r2, #17
	lsls	r1, r1, #14
	movs	r0, #6
	bl 0x0200db0c
	movs	r0, #1
	bl 0x0200d9ac
	ldr	r1, [pc, #712]
	movs	r0, #26
	bl 0x0200dad4
	ldr	r1, [pc, #708]
	movs	r0, #5
	bl 0x0200dad4
	ldr	r1, [pc, #704]
	movs	r0, #6
	bl 0x0200dae4
	movs	r0, #20
	bl 0x0200da94
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #5
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #7
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #26
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #5
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #6
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #2
	movs	r2, #0
	adds	r1, #255
	movs	r0, #6
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #6
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #192
	movs	r2, #0
	movs	r0, #7
	lsls	r1, r1, #7
	bl 0x0200db74
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #2
	movs	r2, #0
	adds	r1, #255
	movs	r0, #26
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #192
	movs	r0, #26
	lsls	r1, r1, #6
	movs	r2, #20
	bl 0x0200db74
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	movs	r2, #40
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200db74
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200db94
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200db94
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #6
	bl 0x0200db94
	movs	r1, #208
	movs	r2, #0
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #7
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200db94
	movs	r2, #20
	movs	r0, #5
	movs	r1, #0
	bl 0x0200db74
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #5
	movs	r1, #0
	bl 0x0200db64
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r2, #0
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db74
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200db7c
	movs	r0, #7
	movs	r1, #4
	bl 0x0200db24
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #192
	movs	r0, #26
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200db94
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200db94
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #6
	bl 0x0200db94
	movs	r1, #6
	movs	r2, #20
	adds	r1, #255
	movs	r0, #7
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #6
	movs	r1, #3
.L_02001678:
	bl 0x0200db24
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #6
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #6
	bl 0x0200db7c
	movs	r0, #7
	movs	r1, #4
	bl 0x0200db24
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #5
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r2, #0
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #7
	movs	r1, #4
	bl 0x0200db24
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #176
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200db7c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #7
	bl 0x0200db7c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db7c
	movs	r0, #26
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	b.n	.L_02001748
	.2byte 0x0000
	.4byte 0x00001eae
	.4byte 0x00033333
	.4byte 0x02000240
	.4byte 0x00013333
	.4byte 0x0200deec
	.4byte 0x0200de88
	.4byte 0x0200df14
	.4byte 0x0200df58
	.2byte 0xdf9c
	.2byte 0x0200
.L_02001748:
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #7
	movs	r1, #3
	bl 0x0200db2c
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #26
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200db94
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200db94
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #6
	bl 0x0200db94
	movs	r1, #208
	movs	r0, #26
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r1, #0
	adds	r0, #26
	bl 0x0200db54
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200dabc
	cmp	r0, #1
	bne.n	.L_0200180e
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #5
	movs	r1, #4
	bl 0x0200db2c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #5
	movs	r1, #0
	bl 0x0200db64
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02001834
.L_0200180e:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #26
	adds	r3, #2
	movs	r1, #3
	strh	r3, [r2, #0]
	bl 0x0200db2c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
.L_02001834:
	movs	r1, #176
	movs	r0, #26
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200db74
	movs	r2, #20
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db74
	movs	r0, #26
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #176
	movs	r2, #0
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #6
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #6
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #26
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	ldr	r3, [pc, #1020]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r2, #20
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db74
	movs	r0, #7
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #26
	movs	r1, #4
	bl 0x0200db24
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #176
	movs	r0, #26
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #6
	movs	r2, #20
	adds	r1, #255
	movs	r0, #26
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #131
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r0, #26
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200db94
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200db94
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #6
	bl 0x0200db94
	movs	r1, #128
	movs	r2, #20
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r0, #7
	movs	r1, #4
	bl 0x0200db24
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #26
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #160
	movs	r2, #20
	movs	r0, #7
	lsls	r1, r1, #7
	bl 0x0200db74
	movs	r0, #5
	movs	r1, #4
	bl 0x0200db2c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #5
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #7
	bl 0x0200db94
	movs	r2, #0
	movs	r0, #5
	movs	r1, #0
	bl 0x0200db74
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #5
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #176
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200db7c
	movs	r0, #26
	movs	r1, #4
	bl 0x0200db24
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #6
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #6
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #192
	movs	r2, #0
	movs	r0, #7
	lsls	r1, r1, #7
	bl 0x0200db74
	movs	r0, #7
	movs	r1, #4
	bl 0x0200db24
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
.L_02001ad6:
	bl 0x0200db74
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r0, #26
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #5
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r2, #0
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #26
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200db74
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #7
	bl 0x0200db7c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r0, #6
	movs	r1, #0
	movs	r2, #40
	bl 0x0200db74
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r2, #20
	movs	r0, #6
	movs	r1, #0
	bl 0x0200db74
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #6
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	movs	r2, #0
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200db9c
	movs	r0, #20
	bl 0x0200da94
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #2
	movs	r2, #0
	adds	r1, #255
	movs	r0, #6
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #6
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #176
	movs	r2, #10
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r0, #26
	movs	r1, #4
	bl 0x0200db24
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #7
	movs	r1, #3
	bl 0x0200db2c
	movs	r2, #0
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db74
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #192
	movs	r2, #20
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #7
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	ldr	r0, [r5, #0]
	movs	r1, #7
	bl 0x0200dc4c
	movs	r0, #5
	movs	r1, #7
	bl 0x0200dc4c
	movs	r0, #6
	movs	r1, #7
	bl 0x0200dc4c
	movs	r0, #26
	movs	r1, #7
	bl 0x0200dc4c
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #7
	ldr	r1, [pc, #16]
	adds	r2, #204
	bl 0x0200dacc
	movs	r2, #184
	lsls	r2, r2, #1
	b.n	.L_02001cc0
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x9999
	.2byte 0x0001
.L_02001cc0:
	movs	r0, #7
	movs	r1, #72
	bl 0x0200dafc
	movs	r1, #129
	movs	r0, #26
	lsls	r1, r1, #1
	bl 0x0200db9c
	movs	r2, #174
	movs	r0, #7
	movs	r1, #40
	lsls	r2, r2, #1
	bl 0x0200dafc
	movs	r2, #166
	movs	r0, #7
	movs	r1, #40
	lsls	r2, r2, #1
	bl 0x0200dafc
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #7
	bl 0x0200db74
	ldr	r0, [r5, #0]
	bl 0x0200dadc
	movs	r0, #5
	bl 0x0200dadc
	movs	r0, #6
	bl 0x0200dadc
	movs	r0, #26
	bl 0x0200dadc
	movs	r1, #176
	movs	r2, #0
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #2
	adds	r1, #255
	movs	r2, #20
	movs	r0, #7
	bl 0x0200db94
	movs	r1, #128
	movs	r2, #40
	movs	r0, #7
	lsls	r1, r1, #7
	bl 0x0200db74
	movs	r1, #208
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200db7c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #176
	movs	r2, #0
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r0, #26
	movs	r1, #4
	bl 0x0200db2c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #10
	movs	r2, #40
	adds	r1, #255
	movs	r0, #7
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #26
	bl 0x0200db9c
	movs	r0, #20
	bl 0x0200da94
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #7
	movs	r1, #4
	bl 0x0200db2c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #26
	movs	r1, #4
	bl 0x0200db24
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	movs	r2, #40
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200db74
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #208
	movs	r0, #26
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r1, #0
	adds	r0, #26
	bl 0x0200db54
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200dabc
	cmp	r0, #0
	bne.n	.L_02001e9c
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #26
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #224
	movs	r2, #20
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r0, #5
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #6
	movs	r1, #3
	bl 0x0200db2c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #3
	strh	r3, [r2, #0]
	b.n	.L_02001f10
.L_02001e9c:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r1, #8
	adds	r3, #1
	strh	r3, [r2, #0]
	adds	r1, #255
	movs	r2, #20
	movs	r0, #26
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	bl 0x0200db7c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #5
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #6
	movs	r1, #0
	bl 0x0200db7c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #6
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r2, #0
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r1, #129
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200db9c
	movs	r0, #40
	bl 0x0200da94
	bl 0x0200dc7c
.L_02001f10:
	movs	r1, #10
	movs	r2, #40
	adds	r1, #255
	movs	r0, #7
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	ldr	r5, [pc, #284]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #176
	movs	r2, #0
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200db7c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #144
	lsls	r0, r0, #8
	adds	r0, #5
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #7
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #6
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #26
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #7
	movs	r1, #3
	bl 0x0200db2c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #7
	adds	r1, #102
	adds	r2, #51
	bl 0x0200dacc
	movs	r2, #175
	lsls	r2, r2, #1
	movs	r0, #7
	movs	r1, #40
	bl 0x0200dafc
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #5
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #6
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #26
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200db2c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #102
	movs	r0, #7
	adds	r1, #204
	bl 0x0200dacc
	ldr	r5, [pc, #72]
	movs	r0, #5
	adds	r1, r5, #0
	bl 0x0200dad4
	adds	r1, r5, #0
	movs	r0, #6
	bl 0x0200dad4
	adds	r1, r5, #0
	movs	r0, #26
	bl 0x0200dad4
	adds	r1, r5, #0
	movs	r0, #7
	bl 0x0200dae4
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #244
	bl 0x0200da14
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #46
	bl 0x0200da14
	movs	r0, #7
	movs	r1, #1
	bl 0x0200daac
	bl 0x0200da8c
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xdfe0
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #106
	bl 0x0200da0c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_0200206a
	bl 0x0200aac8
.L_0200206a:
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	ldr	r5, [pc, #1016]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200dacc
	movs	r1, #172
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #208
	bl 0x0200dafc
	movs	r1, #154
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #200
	bl 0x0200dafc
	movs	r1, #145
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #188
	bl 0x0200dafc
	movs	r1, #160
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #5
	adds	r1, #204
	adds	r2, #102
	bl 0x0200dacc
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #6
	adds	r1, #204
	adds	r2, #102
	bl 0x0200dacc
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #26
	adds	r1, #204
	adds	r2, #102
	bl 0x0200dacc
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #102
	movs	r0, #7
	adds	r1, #204
	bl 0x0200dacc
	ldr	r1, [r5, #0]
	movs	r0, #5
	bl 0x0200db1c
	ldr	r1, [r5, #0]
	movs	r0, #6
	bl 0x0200db1c
	ldr	r1, [r5, #0]
	movs	r0, #26
	bl 0x0200db1c
	ldr	r1, [r5, #0]
	movs	r0, #7
	bl 0x0200db1c
	movs	r0, #1
	bl 0x0200d9ac
	ldr	r1, [pc, #836]
	movs	r0, #5
	bl 0x0200dad4
	ldr	r1, [pc, #832]
	movs	r0, #6
	bl 0x0200dad4
	ldr	r1, [pc, #828]
	movs	r0, #26
	bl 0x0200dad4
	ldr	r1, [pc, #824]
	movs	r0, #7
	bl 0x0200dae4
	movs	r0, #20
	bl 0x0200da94
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #7
	bl 0x0200db74
	ldr	r0, [pc, #804]
	bl 0x0200db4c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #152
	lsls	r1, r1, #7
	adds	r1, #204
	ldr	r0, [pc, #784]
	bl 0x0200dba4
	bl 0x0200dbbc
	adds	r0, #85
	strb	r6, [r0, #0]
	movs	r1, #1
	movs	r0, #200
	movs	r2, #180
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #16
	bl 0x0200dbac
	bl 0x0200dbb4
	movs	r0, #10
	bl 0x0200da94
	movs	r0, #174
	movs	r1, #1
	movs	r2, #224
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #16
	bl 0x0200dbac
	bl 0x0200dbb4
	movs	r0, #10
	bl 0x0200da94
	movs	r0, #134
	movs	r1, #1
	movs	r2, #240
	negs	r1, r1
	lsls	r2, r2, #15
	movs	r3, #1
	lsls	r0, r0, #17
	bl 0x0200dbac
	bl 0x0200dbb4
	movs	r0, #20
	bl 0x0200da94
	movs	r0, #136
	movs	r1, #1
	movs	r2, #180
	movs	r3, #1
	lsls	r2, r2, #16
	lsls	r0, r0, #17
	negs	r1, r1
	bl 0x0200dbac
	bl 0x0200dbb4
	movs	r0, #6
	movs	r1, #4
	bl 0x0200db24
	movs	r0, #6
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #2
	movs	r2, #0
	adds	r1, #255
	movs	r0, #5
	bl 0x0200db94
	movs	r0, #5
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #131
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #26
	bl 0x0200db94
	ldr	r0, [r5, #0]
	movs	r1, #26
	bl 0x0200dc4c
	movs	r0, #5
	movs	r1, #26
	bl 0x0200dc4c
	movs	r0, #6
	movs	r1, #26
	bl 0x0200dc4c
	movs	r0, #7
	movs	r1, #26
	bl 0x0200dc4c
	movs	r1, #152
	lsls	r1, r1, #6
	ldr	r0, [pc, #584]
	adds	r1, #102
	bl 0x0200dba4
	movs	r0, #134
	movs	r1, #1
	movs	r2, #146
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200dbac
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #26
	ldr	r1, [pc, #552]
	adds	r2, #153
	bl 0x0200dacc
	movs	r1, #145
	movs	r0, #26
	lsls	r1, r1, #1
	movs	r2, #116
	bl 0x0200dafc
	movs	r1, #134
	movs	r0, #26
	lsls	r1, r1, #1
	movs	r2, #109
	bl 0x0200dafc
	movs	r1, #160
	movs	r0, #26
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200db74
	movs	r1, #192
	movs	r0, #26
	lsls	r1, r1, #6
	movs	r2, #20
	bl 0x0200db74
	movs	r1, #160
	movs	r2, #40
	movs	r0, #26
	lsls	r1, r1, #7
	bl 0x0200db74
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #134
	movs	r1, #1
	movs	r2, #188
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200dbac
	movs	r0, #26
	movs	r1, #218
	movs	r2, #156
	bl 0x0200dafc
	movs	r2, #40
	movs	r1, #0
	movs	r0, #26
	bl 0x0200db74
	ldr	r0, [r5, #0]
	bl 0x0200dadc
	movs	r0, #5
	bl 0x0200dadc
	movs	r0, #6
	bl 0x0200dadc
	movs	r0, #7
	bl 0x0200dadc
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #224
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200db74
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #26
	adds	r1, #102
	adds	r2, #51
	bl 0x0200dacc
	movs	r2, #156
	movs	r1, #248
	movs	r0, #26
	bl 0x0200dafc
	movs	r0, #40
	bl 0x0200da94
	movs	r0, #26
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	ldr	r0, [r5, #0]
	movs	r1, #26
	bl 0x0200dc4c
	movs	r0, #5
	movs	r1, #26
	bl 0x0200dc4c
	movs	r0, #6
	movs	r1, #26
	bl 0x0200dc4c
	movs	r1, #26
	movs	r0, #7
	bl 0x0200dc4c
	movs	r0, #20
	bl 0x0200da94
	movs	r0, #26
	bl 0x0200dac4
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r6, #254
	adds	r3, r6, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #248
	movs	r2, #188
	movs	r0, #26
	bl 0x0200dafc
	movs	r0, #1
	bl 0x0200da94
	movs	r0, #26
	bl 0x0200dac4
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #1
	mov	r8, r2
	mov	r2, r8
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #20
	bl 0x0200da94
	movs	r1, #7
	movs	r0, #26
	bl 0x0200db24
	movs	r0, #20
	bl 0x0200da94
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200db94
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200db94
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x0200db94
	movs	r1, #2
	movs	r2, #40
	adds	r1, #255
	movs	r0, #7
	bl 0x0200db94
	ldr	r0, [r5, #0]
	bl 0x0200dadc
	movs	r0, #5
	bl 0x0200dadc
	movs	r0, #6
	bl 0x0200dadc
	movs	r0, #7
	bl 0x0200dadc
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #224
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200db94
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #6
	bl 0x0200db94
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200db2c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	ldr	r0, [r5, #0]
	adds	r1, #102
	adds	r2, #51
	bl 0x0200dacc
	movs	r1, #140
	movs	r2, #188
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200dafc
	b.n	.L_02002490
	.4byte 0x02000240
	.4byte 0x0200e014
	.4byte 0x0200e058
	.4byte 0x0200e09c
	.4byte 0x0200e0e0
	.4byte 0x00001f00
	.4byte 0x00026666
	.2byte 0x3333
	.2byte 0x0001
.L_02002490:
	movs	r1, #1
	movs	r0, #26
	bl 0x0200db24
	movs	r0, #10
	bl 0x0200da94
	movs	r0, #26
	bl 0x0200dac4
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #238
	ands	r6, r3
	movs	r2, #188
	strb	r6, [r0, #0]
	movs	r0, #26
	bl 0x0200dafc
	movs	r0, #1
	bl 0x0200da94
	movs	r0, #26
	bl 0x0200dac4
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, r8
	orrs	r2, r3
	strb	r2, [r0, #0]
	movs	r1, #0
	movs	r0, #26
	mov	r8, r2
	bl 0x0200db54
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200db94
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
.L_020024fa:
	ldr	r3, [pc, #76]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x0200dabc
	cmp	r0, #0
	bne.n	.L_02002522
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0200254c
.L_02002522:
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #26
	subs	r3, #2
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x0200db54
	b.n	.L_020024fa
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
.L_0200254c:
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200da14
	bl 0x0200b84c
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200da1c
	ldr	r5, [pc, #1016]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200db94
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200db94
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #6
	bl 0x0200db94
	movs	r1, #128
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200db94
	movs	r0, #26
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #129
	movs	r0, #5
	lsls	r1, r1, #1
	bl 0x0200db9c
	movs	r0, #5
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #26
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200db74
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #6
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #192
	movs	r2, #20
	movs	r0, #26
	lsls	r1, r1, #6
	bl 0x0200db74
	movs	r0, #26
	movs	r1, #3
	bl 0x0200db2c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db7c
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200db2c
	ldr	r1, [r5, #0]
	movs	r0, #5
	bl 0x0200dc4c
	ldr	r1, [r5, #0]
	movs	r0, #6
	bl 0x0200dc4c
	ldr	r1, [r5, #0]
	movs	r0, #26
	bl 0x0200dc4c
	ldr	r1, [r5, #0]
	movs	r0, #7
	bl 0x0200dc4c
	movs	r0, #134
	movs	r1, #1
	movs	r2, #164
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200dbac
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200dacc
	movs	r1, #140
	movs	r2, #120
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200dafc
	movs	r1, #128
	lsls	r1, r1, #8
	ldr	r0, [r5, #0]
	bl 0x0200db7c
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200da14
	bl 0x0200b84c
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200da1c
	movs	r2, #142
	ldr	r0, [r5, #0]
	movs	r1, #232
	bl 0x0200dafc
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r0, [r5, #0]
	bl 0x0200db7c
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #34
	bl 0x0200da14
	bl 0x0200b84c
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #34
	bl 0x0200da1c
	movs	r1, #148
	movs	r2, #142
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200dafc
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r0, [r5, #0]
	bl 0x0200db7c
	movs	r0, #146
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200da14
	bl 0x0200b84c
	movs	r0, #146
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200da1c
	movs	r0, #134
	movs	r1, #1
	movs	r2, #188
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200dbac
	movs	r1, #142
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #188
	bl 0x0200dafc
	movs	r1, #128
	movs	r2, #10
	lsls	r1, r1, #8
	ldr	r0, [r5, #0]
	bl 0x0200db74
	movs	r0, #5
	bl 0x0200dadc
	movs	r0, #6
	bl 0x0200dadc
	movs	r0, #26
	bl 0x0200dadc
	movs	r0, #7
	bl 0x0200dadc
	movs	r0, #26
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #2
	movs	r2, #0
	adds	r1, #255
	movs	r0, #5
	bl 0x0200db94
	movs	r0, #5
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #208
	movs	r2, #0
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #160
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #160
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #160
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #224
	movs	r2, #0
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r1, #129
	movs	r0, #7
	lsls	r1, r1, #1
	bl 0x0200db9c
	movs	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #10
	movs	r2, #0
	adds	r1, #255
	movs	r0, #26
	bl 0x0200db94
	movs	r1, #128
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200db7c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #2
	movs	r2, #20
	adds	r1, #255
	movs	r0, #7
	bl 0x0200db94
	movs	r1, #208
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200db7c
	movs	r0, #26
	movs	r1, #0
	movs	r2, #20
	bl 0x0200db5c
	movs	r1, #224
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #192
	movs	r2, #20
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200db74
	movs	r0, #6
	movs	r1, #0
	bl 0x0200db64
	movs	r2, #10
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db74
	movs	r0, #26
	movs	r1, #4
	bl 0x0200db24
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #160
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #208
	movs	r0, #26
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r0, #134
	movs	r1, #1
	movs	r2, #164
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200dbac
	movs	r1, #148
	movs	r0, #5
	lsls	r1, r1, #1
	movs	r2, #168
	bl 0x0200dafc
	movs	r1, #140
	movs	r0, #5
	lsls	r1, r1, #1
	movs	r2, #154
	bl 0x0200dafc
	movs	r1, #128
	movs	r2, #40
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r0, #5
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #2
	movs	r2, #0
	adds	r1, #255
	movs	r0, #26
	bl 0x0200db94
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #192
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200db74
	movs	r0, #5
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #26
	bl 0x0200db94
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #134
	movs	r1, #1
	movs	r2, #188
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	bl 0x0200dbac
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #192
	movs	r0, #26
	lsls	r1, r1, #6
	movs	r2, #40
	bl 0x0200db74
	movs	r1, #160
	movs	r0, #26
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200db74
	movs	r1, #208
	movs	r2, #40
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db7c
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #129
	movs	r0, #26
	lsls	r1, r1, #1
	bl 0x0200db9c
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	movs	r2, #20
	movs	r0, #5
	lsls	r1, r1, #7
	bl 0x0200db74
	movs	r0, #26
	movs	r1, #4
	bl 0x0200db24
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #192
	movs	r0, #5
	b.n	.L_02002964
	.2byte 0x0240
	.2byte 0x0200
.L_02002964:
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #192
	movs	r2, #20
	movs	r0, #26
	lsls	r1, r1, #6
	bl 0x0200db74
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r2, #10
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db74
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #8
	movs	r2, #0
	adds	r1, #255
	movs	r0, #7
	bl 0x0200db94
	movs	r0, #7
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200db7c
	movs	r0, #26
	movs	r1, #4
	bl 0x0200db24
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #2
	movs	r2, #40
	adds	r1, #255
	movs	r0, #7
	bl 0x0200db94
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db7c
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #7
	bl 0x0200db94
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x0200db94
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200db94
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #6
	bl 0x0200db94
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #26
	bl 0x0200db94
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #26
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #5
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #6
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #7
	movs	r1, #3
	bl 0x0200db2c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #104]
	adds	r2, #153
	bl 0x0200dacc
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #88]
	adds	r2, #153
	bl 0x0200dacc
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #26
	ldr	r1, [pc, #76]
	adds	r2, #153
	bl 0x0200dacc
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #7
	ldr	r1, [pc, #60]
	bl 0x0200dacc
	ldr	r5, [pc, #56]
	movs	r0, #5
	adds	r1, r5, #0
	bl 0x0200dad4
	movs	r0, #6
	adds	r1, r5, #0
	bl 0x0200dad4
	movs	r0, #26
	adds	r1, r5, #0
	bl 0x0200dad4
	movs	r0, #7
	adds	r1, r5, #0
	bl 0x0200dae4
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #245
	bl 0x0200da14
	bl 0x0200daa4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00013333
	.2byte 0xe108
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #144
	ldr	r5, [r3, #0]
	ldr	r3, [pc, #16]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200dac4
	str	r0, [r5, #24]
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x049b23c0
	.4byte 0x681a3390
	.4byte 0x61932300
	.2byte 0x4770
	.2byte 0x0000
	push	{lr}
	cmp	r0, #31
	ble.n	.L_02002b10
	movs	r0, #31
.L_02002b10:
	cmp	r0, #0
	bge.n	.L_02002b16
	movs	r0, #0
.L_02002b16:
	pop	{pc}
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r3, [pc, #76]
	lsls	r1, r1, #1
	mov	sl, r0
	ldrsh	r6, [r3, r1]
	movs	r0, #248
	lsls	r6, r6, #16
	lsls	r0, r0, #13
	ands	r0, r6
	ldr	r3, [pc, #56]
	asrs	r0, r0, #16
	adds	r0, r0, r2
	lsrs	r5, r6, #21
	lsls	r0, r0, #16
	lsrs	r6, r6, #26
	ands	r5, r3
	ands	r6, r3
	asrs	r0, r0, #16
	asrs	r3, r2, #1
	asrs	r2, r2, #2
	adds	r5, r5, r3
	adds	r6, r6, r2
	bl 0x0200ab08
	mov	r8, r0
	mov	r2, r8
	lsls	r5, r5, #16
	asrs	r5, r5, #16
	lsls	r2, r2, #16
	asrs	r2, r2, #16
	adds	r0, r5, #0
	mov	r8, r2
	bl 0x0200ab08
	lsls	r6, r6, #16
	asrs	r6, r6, #16
	adds	r5, r0, #0
	adds	r0, r6, #0
	b.n	.L_02002b74
	.4byte 0x0000001f
	.2byte 0xf948
	.2byte 0x0200
.L_02002b74:
	bl 0x0200ab08
	lsls	r5, r5, #16
	mov	r3, sl
	asrs	r5, r5, #16
	lsls	r0, r0, #16
	lsls	r3, r3, #1
	lsls	r5, r5, #5
	asrs	r0, r0, #6
	orrs	r0, r5
	mov	sl, r3
	movs	r2, #160
	mov	r3, r8
	orrs	r3, r0
	lsls	r2, r2, #19
	add	sl, r2
	mov	r8, r3
	mov	r0, r8
	mov	r2, sl
	strh	r0, [r2, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r3, [pc, #80]
	lsls	r1, r1, #1
	mov	sl, r0
	ldrsh	r6, [r3, r1]
	movs	r0, #248
	lsls	r6, r6, #16
	lsls	r0, r0, #13
	ands	r0, r6
	asrs	r0, r0, #16
	ldr	r3, [pc, #60]
	adds	r0, r0, r2
	adds	r0, #4
	lsrs	r5, r6, #21
	subs	r2, #4
	lsrs	r6, r6, #26
	lsls	r0, r0, #16
	ands	r5, r3
	ands	r6, r3
	asrs	r0, r0, #16
	asrs	r3, r2, #1
	asrs	r2, r2, #2
	adds	r5, r5, r3
	adds	r6, r6, r2
	bl 0x0200ab08
	mov	r8, r0
	mov	r2, r8
	lsls	r5, r5, #16
	asrs	r5, r5, #16
	lsls	r2, r2, #16
	asrs	r2, r2, #16
	adds	r0, r5, #0
	mov	r8, r2
	bl 0x0200ab08
	lsls	r6, r6, #16
	asrs	r6, r6, #16
	adds	r5, r0, #0
	adds	r0, r6, #0
	b.n	.L_02002c04
	.4byte 0x0000001f
	.2byte 0xf948
	.2byte 0x0200
.L_02002c04:
	bl 0x0200ab08
	lsls	r5, r5, #16
	mov	r3, sl
	asrs	r5, r5, #16
	lsls	r0, r0, #16
	lsls	r3, r3, #1
	lsls	r5, r5, #5
	asrs	r0, r0, #6
	orrs	r0, r5
	mov	sl, r3
	movs	r2, #160
	mov	r3, r8
	orrs	r3, r0
	lsls	r2, r2, #19
	add	sl, r2
	mov	r8, r3
	mov	r0, r8
	mov	r2, sl
	strh	r0, [r2, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02002cc4
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02002cc4
	movs	r1, #175
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02002cc4
	ldr	r3, [pc, #96]
	movs	r2, #7
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02002cc4
	bl 0x0200d9c4
	lsls	r2, r0, #2
	adds	r2, r2, r0
	lsls	r2, r2, #1
	lsrs	r2, r2, #16
	subs	r2, #5
	movs	r1, #0
	movs	r0, #9
	bl 0x0200ab18
	bl 0x0200d9c4
	adds	r2, r0, #0
	lsls	r2, r2, #3
	lsrs	r2, r2, #16
	subs	r2, #4
	movs	r1, #1
	movs	r0, #87
	bl 0x0200ab18
	bl 0x0200d9c4
	adds	r2, r0, #0
	lsls	r2, r2, #2
	lsrs	r2, r2, #16
	subs	r2, #2
	movs	r1, #2
	movs	r0, #103
	bl 0x0200ab18
	bl 0x0200d9c4
	adds	r2, r0, #0
	lsls	r2, r2, #2
	lsrs	r2, r2, #16
	subs	r2, #2
	movs	r0, #119
	movs	r1, #3
	bl 0x0200ab18
.L_02002cc4:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02002d5e
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02002d5e
	movs	r1, #175
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02002d5e
	ldr	r3, [pc, #96]
	movs	r2, #7
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02002d5e
	bl 0x0200d9c4
	lsls	r2, r0, #2
	adds	r2, r2, r0
	lsls	r2, r2, #1
	lsrs	r2, r2, #16
	subs	r2, #5
	movs	r1, #0
	movs	r0, #9
	bl 0x0200ab18
	bl 0x0200d9c4
	lsls	r2, r0, #1
	adds	r2, r2, r0
	lsls	r2, r2, #1
	lsrs	r2, r2, #16
	subs	r2, #3
	movs	r1, #1
	movs	r0, #7
	bl 0x0200ab18
	bl 0x0200d9c4
	adds	r2, r0, #0
	lsls	r2, r2, #2
	lsrs	r2, r2, #16
	subs	r2, #2
	movs	r1, #2
	movs	r0, #6
	bl 0x0200ab18
	bl 0x0200d9c4
	adds	r2, r0, #0
	lsls	r2, r2, #3
	lsrs	r2, r2, #16
	subs	r2, #4
	movs	r0, #76
	movs	r1, #3
	bl 0x0200aba4
.L_02002d5e:
	pop	{pc}
	.2byte 0x122c
	.2byte 0x0300
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02002dda
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02002dda
	movs	r1, #175
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02002dda
	ldr	r3, [pc, #68]
	movs	r2, #7
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02002dda
	bl 0x0200d9c4
	adds	r2, r0, #0
	lsls	r2, r2, #2
	lsrs	r2, r2, #16
	subs	r2, #2
	movs	r1, #0
	movs	r0, #169
	bl 0x0200ab18
	bl 0x0200d9c4
	adds	r2, r0, #0
	lsls	r2, r2, #1
	lsrs	r2, r2, #16
	subs	r2, #1
	movs	r1, #1
	movs	r0, #167
	bl 0x0200ab18
	bl 0x0200d9c4
	adds	r2, r0, #0
	lsrs	r2, r2, #16
	movs	r0, #166
	movs	r1, #2
	bl 0x0200ab18
.L_02002dda:
	pop	{pc}
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	movs	r0, #234
	movs	r2, #144
	movs	r3, #172
	adds	r0, #255
	ldr	r1, [pc, #108]
	lsls	r2, r2, #14
	lsls	r3, r3, #17
	ldr	r5, [pc, #104]
	bl 0x0200da34
	movs	r7, #0
	str	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L_02002e56
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
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r6, #9]
	adds	r3, r0, #0
	adds	r3, #85
	adds	r2, r0, #0
	adds	r2, #92
	strb	r7, [r3, #0]
	movs	r1, #193
	movs	r3, #1
	strb	r3, [r2, #0]
	lsls	r1, r1, #3
	strb	r7, [r6, #26]
	strb	r7, [r6, #27]
	movs	r0, #68
	bl 0x0200d9e4
	adds	r5, r0, #0
	movs	r0, #242
	bl 0x0200da84
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r5, r5, r3
	ldrb	r0, [r6, #16]
	movs	r1, #128
	adds	r2, r5, #0
	bl 0x0200d9fc
	movs	r0, #68
	bl 0x0200d9ec
.L_02002e56:
	pop	{r5, r6, r7, pc}
	.4byte 0x01090000
	.2byte 0xf950
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200dac4
	ldr	r3, [r0, #12]
	movs	r2, #128
	lsls	r2, r2, #13
	cmp	r3, r2
	ble.n	.L_02002e8e
	movs	r0, #29
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db0c
	movs	r0, #30
	movs	r1, #2
	bl 0x0200db84
	b.n	.L_02002eaa
.L_02002e8e:
	movs	r0, #29
	bl 0x0200dac4
	movs	r1, #240
	movs	r3, #168
	lsls	r1, r1, #15
	movs	r2, #0
	lsls	r3, r3, #16
	bl 0x0200da44
	movs	r0, #30
	movs	r1, #3
	bl 0x0200db84
.L_02002eaa:
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	ldr	r5, [pc, #36]
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02002ed4
	movs	r0, #27
	bl 0x0200dac4
	ldr	r4, [r5, #0]
	ldr	r2, [r0, #12]
	movs	r3, #192
	lsls	r3, r3, #12
	ldr	r1, [r0, #8]
	adds	r2, r2, r3
	ldr	r3, [r0, #16]
	adds	r0, r4, #0
	bl 0x0200da44
.L_02002ed4:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0xf950
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	ldr	r5, [r6, #68]
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	adds	r3, r3, r7
	movs	r1, #18
	str	r3, [r6, #16]
	bl 0x0200d99c
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02002f0c
	adds	r3, #15
.L_02002f0c:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #156]
	ldr	r7, [r3, #0]
	movs	r3, #3
	ands	r7, r3
	cmp	r7, #0
	bne.n	.L_02002fe8
	movs	r0, #183
	movs	r1, #132
	movs	r2, #128
	movs	r3, #158
	lsls	r0, r0, #1
	lsls	r1, r1, #17
	lsls	r2, r2, #14
	lsls	r3, r3, #17
	bl 0x0200da34
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02002fe8
	bl 0x0200d9c4
	movs	r3, #192
	lsls	r0, r0, #13
	lsls	r3, r3, #6
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	bl 0x0200d9d4
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #72]
	str	r0, [r6, #68]
	bl 0x0200d9c4
	movs	r3, #128
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	lsls	r3, r3, #11
	subs	r3, r3, r0
	str	r3, [r6, #76]
	bl 0x0200d9c4
	ldr	r3, [pc, #76]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	ldr	r3, [pc, #60]
	adds	r5, r6, #0
	adds	r2, r6, #0
	adds	r2, #85
	adds	r5, #100
	strh	r0, [r5, #0]
	movs	r1, #0
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	bl 0x0200da74
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200da24
	ldr	r1, [pc, #40]
	adds	r0, r6, #0
	bl 0x0200da2c
	ldr	r1, [r6, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	ldr	r3, [pc, #20]
	strh	r7, [r5, #0]
	str	r7, [r6, #48]
	b.n	.L_02002fe4
	.4byte 0x00000000
	.4byte 0x0300122c
	.4byte 0xffff8000
	.4byte 0x0200dca4
	.2byte 0xaedd
	.2byte 0x0200
.L_02002fe4:
	str	r7, [r6, #52]
	str	r3, [r6, #108]
.L_02002fe8:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{lr}
	adds	r1, r0, #0
	adds	r1, #100
	movs	r3, #0
	ldrsh	r2, [r1, r3]
	ldr	r3, [r0, #8]
	lsls	r2, r2, #8
	adds	r3, r3, r2
	str	r3, [r0, #8]
	ldr	r3, [r0, #12]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
	str	r3, [r0, #12]
	movs	r2, #160
	ldr	r3, [r0, #24]
	lsls	r2, r2, #3
	adds	r2, #30
	adds	r3, r3, r2
	str	r3, [r0, #24]
	ldr	r3, [r0, #28]
	adds	r3, r3, r2
	str	r3, [r0, #28]
	ldrh	r3, [r1, #0]
	adds	r3, #2
	strh	r3, [r1, #0]
	ldr	r3, [r0, #104]
	subs	r3, #1
	str	r3, [r0, #104]
	cmp	r3, #0
	bne.n	.L_0200302e
	bl 0x0200da3c
.L_0200302e:
	pop	{pc}
	push	{r5, r6, lr}
	ldr	r3, [pc, #124]
	ldr	r6, [r3, #0]
	movs	r3, #63
	ands	r6, r3
	cmp	r6, #0
	bne.n	.L_020030ac
	movs	r0, #30
	movs	r1, #176
	movs	r2, #128
	movs	r3, #157
	adds	r0, #255
	lsls	r1, r1, #15
	lsls	r2, r2, #13
	lsls	r3, r3, #17
	bl 0x0200da34
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020030ac
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #2
	strb	r3, [r2, #0]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r3, #15
	strh	r6, [r3, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #13
	ldr	r1, [r5, #80]
	negs	r3, r3
	ldrb	r2, [r1, #9]
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r1, #9]
	movs	r1, #0
	bl 0x0200da74
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	movs	r3, #60
	str	r3, [r5, #104]
	ldr	r3, [pc, #24]
	adds	r0, r5, #0
	movs	r1, #5
	str	r3, [r5, #108]
	bl 0x0200da24
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200da7c
.L_020030ac:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0300122c
	.2byte 0xafed
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	adds	r5, r6, #0
	adds	r5, #100
	movs	r1, #0
	ldrsh	r7, [r5, r1]
	ldrh	r3, [r5, #0]
	cmp	r7, #0
	beq.n	.L_020030d0
	subs	r3, #1
	strh	r3, [r5, #0]
	b.n	.L_020030fe
.L_020030d0:
	bl 0x0200d9c4
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	lsrs	r3, r3, #16
	adds	r3, #10
	strh	r3, [r5, #0]
	adds	r2, r6, #0
	adds	r2, #102
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	beq.n	.L_020030f6
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r6, #24]
	strh	r7, [r2, #0]
	b.n	.L_020030fe
.L_020030f6:
	ldr	r3, [pc, #8]
	str	r3, [r6, #24]
	movs	r3, #1
	strh	r3, [r2, #0]
.L_020030fe:
	pop	{r5, r6, r7, pc}
	.2byte 0x8000
	.2byte 0xfffe
	.2byte 0xb560
	adds	r4, r0, #0
	adds	r5, r1, #0
	movs	r0, #131
	adds	r3, r2, #0
	lsls	r0, r0, #1
	adds	r1, r4, #0
	adds	r2, r5, #0
	bl 0x0200da34
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_0200317e
	adds	r3, r6, #0
	adds	r3, #85
	movs	r5, #0
	strb	r5, [r3, #0]
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r6, #28]
	bl 0x0200d9c4
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	lsrs	r3, r3, #16
	adds	r2, r6, #0
	adds	r3, #10
	adds	r2, #100
	strh	r3, [r2, #0]
	adds	r3, r6, #0
	adds	r3, #102
	strh	r5, [r3, #0]
	ldr	r3, [pc, #44]
	ldr	r1, [r6, #80]
	str	r3, [r6, #108]
	ldrb	r2, [r1, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200da74
	adds	r0, r6, #0
	movs	r1, #5
	bl 0x0200da24
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200da7c
.L_0200317e:
	pop	{r5, r6, pc}
	.2byte 0xb0b9
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #2
	movs	r5, #220
	strh	r3, [r2, #0]
	lsls	r5, r5, #17
	movs	r1, #160
	movs	r2, #225
	adds	r0, r5, #0
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	bl 0x0200b104
	movs	r1, #184
	movs	r2, #219
	adds	r0, r5, #0
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	bl 0x0200b104
	movs	r1, #208
	movs	r2, #213
	adds	r0, r5, #0
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	b.n	.L_020031cc
	.2byte 0x0000
	.4byte 0x00000c08
	.2byte 0x3f10
	.2byte 0x0000
.L_020031cc:
	bl 0x0200b104
	movs	r1, #232
	movs	r2, #207
	adds	r0, r5, #0
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	bl 0x0200b104
	movs	r1, #128
	movs	r2, #201
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	adds	r0, r5, #0
	bl 0x0200b104
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200db8c
	adds	r0, r5, #0
	bl 0x0200dac4
	movs	r1, #15
	bl 0x0200db44
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200db84
	adds	r0, r5, #0
	bl 0x0200dac4
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #10
	adds	r0, #255
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_02003240
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200da0c
	cmp	r0, #0
	bne.n	.L_02003248
	bl 0x0200857c
	b.n	.L_02003248
.L_02003240:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200da14
.L_02003248:
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r7, #0
.L_02003250:
	adds	r6, r7, #0
	adds	r6, #15
	adds	r0, r6, #0
	bl 0x0200dac4
	adds	r5, r0, #0
	adds	r5, #85
	adds	r0, r6, #0
	adds	r7, #1
	movs	r6, #0
	bl 0x0200b1f0
	strb	r6, [r5, #0]
	cmp	r7, #1
	bls.n	.L_02003250
	movs	r0, #15
	bl 0x0200dac4
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r0, #12]
	movs	r0, #16
	bl 0x0200dac4
	str	r6, [r0, #12]
	bl 0x0200b220
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	movs	r7, #0
.L_0200328c:
	adds	r6, r7, #0
	adds	r6, #26
	adds	r0, r6, #0
	bl 0x0200dac4
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl 0x0200b1f0
	adds	r5, #85
	movs	r3, #0
	adds	r7, #1
	strb	r3, [r5, #0]
	cmp	r7, #1
	bls.n	.L_0200328c
	bl 0x0200b220
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	movs	r5, #0
.L_020032b4:
	adds	r0, r5, #0
	adds	r0, #18
	movs	r1, #5
	adds	r5, #1
	bl 0x0200db24
	cmp	r5, #7
	bls.n	.L_020032b4
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r5, #0
.L_020032cc:
	adds	r0, r5, #0
	adds	r0, #18
	movs	r1, #1
	adds	r5, #1
	bl 0x0200db24
	cmp	r5, #7
	bls.n	.L_020032cc
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #143
	movs	r1, #1
	bl 0x0200dc14
	movs	r1, #27
	movs	r0, #17
	bl 0x0200dc1c
	bl 0x0200dc34
	movs	r0, #1
	bl 0x0200dc0c
	bl 0x0200dc24
	bl 0x0200dc2c
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	movs	r2, #99
	adds	r2, r2, r6
	ldrb	r3, [r2, #0]
	sub	sp, #12
	movs	r1, #0
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_02003342
	adds	r3, r6, #0
	adds	r2, r6, #0
	adds	r3, #100
	adds	r2, #102
	movs	r1, #0
	ldrsh	r2, [r2, r1]
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	adds	r3, r3, r2
	lsls	r0, r3, #3
	adds	r0, r0, r3
	lsls	r0, r0, #9
	bl 0x0200d9cc
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r1, r3, #1
.L_02003342:
	ldr	r3, [r6, #56]
	mov	r5, sp
	str	r3, [r5, #0]
	adds	r7, r6, #0
	ldr	r3, [r6, #60]
	adds	r7, #100
	str	r3, [r5, #4]
	ldr	r3, [r6, #64]
	str	r3, [r5, #8]
	adds	r3, r6, #0
	ldr	r0, [r6, #76]
	adds	r3, #102
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	movs	r4, #0
	ldrsh	r3, [r7, r4]
	lsls	r0, r0, #16
	adds	r0, r0, r1
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #8
	adds	r1, r1, r2
	adds	r2, r5, #0
	bl 0x0200d9dc
	ldr	r3, [r5, #0]
	str	r3, [r6, #8]
	ldr	r3, [r5, #4]
	str	r3, [r6, #12]
	ldr	r3, [r5, #8]
	adds	r5, r6, #0
	str	r3, [r6, #16]
	adds	r5, #98
	ldrb	r3, [r5, #0]
	cmp	r3, #7
	bls.n	.L_0200338c
	b.n	.L_020034f2
.L_0200338c:
	ldr	r2, [pc, #364]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0200b3b4
	.4byte 0x0200b3f8
	.4byte 0x0200b42c
	.4byte 0x0200b44a
	.4byte 0x0200b468
	.4byte 0x0200b482
	.4byte 0x0200b4b0
	.4byte 0x0200b4e4
	.4byte 0x2001883b
	.4byte 0x803b3304
	.4byte 0x6d3264f0
	.4byte 0x8a532180
	.4byte 0x185b0149
	.4byte 0x21c08253
	.4byte 0x00c969f3
	.4byte 0x316669b2
	.4byte 0x61f3185b
	.4byte 0x185223c0
	.4byte 0x61b2025b
	.4byte 0xdc00429a
	.4byte 0x6d33e085
	.4byte 0x2b008a5b
	.4byte 0xe080d000
	.4byte 0x70284644
	.4byte 0xe07c7020
	.4byte 0x2180883b
	.4byte 0x803b3304
	.4byte 0x6d320149
	.4byte 0x185b8a53
	.4byte 0x6d338253
	.4byte 0x2b008a5b
	.4byte 0x4642d16f
	.4byte 0x33ff7813
	.4byte 0x061b7013
	.4byte 0xd1682b00
	.4byte 0xf00220dc
	.4byte 0x2302fc3b
	.4byte 0xe062702b
	.4byte 0x2480883b
	.4byte 0x803b3301
	.4byte 0x6bf302e4
	.4byte 0x191b21c8
	.4byte 0x63f303c9
	.4byte 0xdd56428b
	.4byte 0x702b2303
	.4byte 0x883be053
	.4byte 0x33014642
	.4byte 0x6cf3803b
	.4byte 0x64f33302
	.4byte 0x7013230a
	.4byte 0x2b286cf3
	.4byte 0x2304dd47
	.4byte 0xe044702b
	.4byte 0x4644883b
	.4byte 0x803b3301
	.4byte 0x33ff7823
	.4byte 0x061b7023
	.4byte 0xd13a2b00
	.4byte 0x702b2305
	.4byte 0x883be037
	.4byte 0x33012188
	.4byte 0x0409803b
	.4byte 0x3b026cf3
	.4byte 0x6bf364f3
	.4byte 0xda03428b
	.4byte 0x02922280
	.4byte 0x63f3189b
	.4byte 0x2b016cf3
	.4byte 0x2300dc25
	.4byte 0x230664f3
	.4byte 0xe020702b
	.4byte 0x2480883b
	.4byte 0x803b3304
	.4byte 0x6d320164
	.4byte 0x8a534910
	.4byte 0x8253191b
	.4byte 0x69f369b2
	.4byte 0x185b1852
	.4byte 0x01492190
	.4byte 0x61f361b2
	.4byte 0xda0c428a
	.4byte 0x63b32364
	.4byte 0x23076433
	.4byte 0xe006702b
	.4byte 0x7c186d33
	.4byte 0xfa84f002
	.4byte 0xf0021c30
	.2byte 0xfaa5
.L_020034f2:
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200b394
	.2byte 0xf000
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #17
	bl 0x0200dac4
	mov	r8, r0
	movs	r0, #0
	mov	fp, r0
	movs	r0, #154
	bl 0x0200dc9c
	movs	r2, #0
	mov	sl, r2
.L_02003528:
	movs	r0, #17
	bl 0x0200dac4
	movs	r1, #7
	bl 0x0200db44
	movs	r0, #2
	bl 0x0200d9ac
	movs	r0, #17
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200db44
	movs	r0, #2
	bl 0x0200d9ac
	movs	r3, #1
	add	sl, r3
	mov	r0, sl
	cmp	r0, #7
	bls.n	.L_02003528
	movs	r0, #209
	bl 0x0200dc9c
	movs	r2, #0
	mov	sl, r2
	mov	r9, r2
.L_02003562:
	mov	r3, r8
	ldr	r2, [r3, #12]
	movs	r0, #192
	lsls	r0, r0, #13
	adds	r2, r2, r0
	movs	r0, #209
	lsls	r0, r0, #1
	ldr	r1, [r3, #8]
	adds	r0, #255
	ldr	r3, [r3, #16]
	bl 0x0200da34
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_020035f4
	mov	r1, fp
	ldr	r0, [r7, #80]
	bl 0x0200dc44
	adds	r3, r7, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	ldr	r1, [r7, #80]
	mov	fp, r0
	ldrb	r3, [r1, #9]
	movs	r0, #13
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200da74
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200da24
	adds	r3, r7, #0
	adds	r3, #100
	movs	r1, #180
	strh	r5, [r3, #0]
	lsls	r1, r1, #1
	str	r5, [r7, #76]
	mov	r0, r9
	bl 0x0200d9a4
	ldr	r6, [pc, #36]
	adds	r3, r7, #0
	adds	r3, #102
	strh	r0, [r3, #0]
	subs	r3, #4
	strb	r6, [r3, #0]
	mov	r2, r8
	ldr	r3, [r2, #8]
	movs	r0, #192
	str	r3, [r7, #56]
	lsls	r0, r0, #13
	ldr	r3, [r2, #12]
	adds	r3, r3, r0
	str	r3, [r7, #60]
	ldr	r3, [r2, #16]
	str	r3, [r7, #64]
	ldr	r3, [pc, #8]
	str	r3, [r7, #108]
	b.n	.L_020035f4
	.4byte 0x00000000
	.2byte 0xb309
	.2byte 0x0200
.L_020035f4:
	movs	r3, #1
	movs	r2, #176
	add	sl, r3
	lsls	r2, r2, #13
	mov	r0, sl
	add	r9, r2
	cmp	r0, #15
	bls.n	.L_02003562
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{lr}
	ldr	r0, [pc, #36]
	bl 0x0200d9bc
	movs	r0, #1
	bl 0x0200d9ac
	bl 0x0200dbb4
	movs	r0, #40
	bl 0x0200da94
	ldr	r3, [pc, #16]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02003634
	bl 0x0200b504
.L_02003634:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200accd
	.2byte 0xf950
	.2byte 0x0200
	push	{r5, lr}
	sub	sp, #8
	adds	r5, r1, #0
	cmp	r0, #0
	beq.n	.L_020036ac
	movs	r3, #3
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #37
	movs	r2, #79
	movs	r3, #15
	movs	r0, #49
	bl 0x0200da64
	movs	r0, #12
	bl 0x0200dac4
	movs	r1, #253
	movs	r2, #128
	lsls	r1, r1, #16
	lsls	r2, r2, #14
	ldr	r3, [pc, #104]
	bl 0x0200da44
	movs	r0, #13
	bl 0x0200dac4
	movs	r2, #128
	ldr	r1, [pc, #96]
	lsls	r2, r2, #14
	ldr	r3, [pc, #88]
	bl 0x0200da44
	cmp	r5, #0
	beq.n	.L_0200369a
	movs	r0, #12
	movs	r1, #0
	bl 0x0200db24
	movs	r0, #13
	movs	r1, #0
	bl 0x0200db24
	b.n	.L_020036d4
.L_0200369a:
	movs	r0, #12
	movs	r1, #1
	bl 0x0200db24
	movs	r0, #13
	movs	r1, #1
	bl 0x0200db24
	b.n	.L_020036d4
.L_020036ac:
	movs	r3, #3
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #45
	movs	r1, #37
	movs	r2, #79
	movs	r3, #15
	bl 0x0200da64
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db0c
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db0c
.L_020036d4:
	add	sp, #8
	pop	{r5, pc}
	.4byte 0x01210000
	.2byte 0x0000
	.2byte 0x0113
	push	{r5, lr}
	sub	sp, #8
	adds	r5, r1, #0
	cmp	r0, #0
	beq.n	.L_020037a6
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #41
	movs	r1, #38
	movs	r2, #79
	movs	r3, #16
	bl 0x0200da64
	movs	r3, #16
	movs	r2, #22
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #12
	movs	r1, #21
	movs	r2, #1
	movs	r3, #1
	bl 0x0200da6c
	cmp	r5, #0
	beq.n	.L_0200372a
	movs	r0, #14
	bl 0x0200dac4
	movs	r1, #132
	movs	r2, #128
	movs	r3, #154
	lsls	r1, r1, #17
	lsls	r2, r2, #14
	lsls	r3, r3, #17
	bl 0x0200da44
.L_0200372a:
	cmp	r5, #6
	bhi.n	.L_020037d6
	ldr	r2, [pc, #172]
	lsls	r3, r5, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x0200b754
	.4byte 0x0200b760
	.4byte 0x0200b76a
	.4byte 0x0200b774
	.4byte 0x0200b77e
	.4byte 0x0200b788
	.4byte 0x0200b78e
	.4byte 0x2100200e
	.4byte 0xf0022200
	.4byte 0xe03af9d7
	.4byte 0x2100200e
	.4byte 0xf9def002
	.4byte 0x200ee035
	.4byte 0xf0022101
	.4byte 0xe030f9d9
	.4byte 0x2102200e
	.4byte 0xf9d4f002
	.4byte 0x200ee02b
	.4byte 0xf0022103
	.4byte 0xe026f9cf
	.4byte 0x2106200e
	.4byte 0x200ee001
	.4byte 0xf0022107
	.4byte 0x200ef9c7
	.4byte 0xf994f002
	.4byte 0x68c34a10
	.4byte 0x60c3189b
	.2byte 0xe017
.L_020037a6:
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #45
	movs	r1, #38
	movs	r2, #79
	movs	r3, #16
	bl 0x0200da64
.L_020037b8:
	movs	r3, #16
	movs	r2, #22
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #17
	movs	r1, #22
	movs	r2, #1
	movs	r3, #1
	bl 0x0200da6c
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db0c
.L_020037d6:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0200b738
	.2byte 0x0000
	.2byte 0xffde
	.2byte 0xb500
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r2, #196
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_0200384a
	movs	r0, #136
	lsls	r0, r0, #2
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_02003810
	bl 0x02008624
	b.n	.L_0200384a
.L_02003810:
	movs	r0, #145
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_02003824
	bl 0x02008668
	b.n	.L_0200384a
.L_02003824:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #34
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_02003838
	bl 0x020086ac
	b.n	.L_0200384a
.L_02003838:
	movs	r0, #146
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_0200384a
	bl 0x020086f0
.L_0200384a:
	pop	{pc}
	push	{r5, lr}
	movs	r0, #136
	movs	r1, #1
	bl 0x0200dc14
	ldr	r5, [pc, #56]
	movs	r1, #144
	adds	r0, r5, #0
	lsls	r1, r1, #3
	bl 0x0200d9b4
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r1, #1
	ldr	r0, [r3, #0]
	negs	r1, r1
	bl 0x0200dc1c
	bl 0x0200dc34
	movs	r0, #1
	bl 0x0200dc0c
	bl 0x0200dc24
	adds	r0, r5, #0
	bl 0x0200d9bc
	bl 0x0200dc2c
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0200b7e5
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #52]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_020038b4
	ldr	r0, [pc, #40]
	bl 0x0200d9bc
	b.n	.L_020038ce
.L_020038b4:
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_020038c2
	ldr	r0, [pc, #36]
	bl 0x0200d9bc
	b.n	.L_020038ce
.L_020038c2:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_020038ce
	ldr	r0, [pc, #28]
	bl 0x0200d9bc
.L_020038ce:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000071
	.4byte 0x0200ac35
	.4byte 0x00000072
	.4byte 0x0200accd
	.4byte 0x0000007d
	.2byte 0xad65
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_02003910
	bl 0x0200b898
.L_02003910:
	movs	r0, #123
	bl 0x0200dc9c
	bl 0x0200dbf4
	bl 0x0200dbfc
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200dbc4
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200b8ec
	pop	{pc}
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #108]
	ldr	r3, [pc, #172]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	bl 0x0200dac4
	adds	r5, r0, #0
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r2, r2
	negs	r0, r0
	negs	r1, r1
	bl 0x0200dbac
	ldr	r0, [r6, #0]
	movs	r1, #2
	bl 0x0200db24
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r6, #0]
	lsls	r2, r2, #7
	lsls	r1, r1, #8
	bl 0x0200dacc
	ldr	r0, [r5, #8]
	asrs	r2, r0, #16
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_0200399e
	adds	r3, #15
.L_0200399e:
	asrs	r3, r3, #4
	lsls	r3, r3, #4
	subs	r3, r2, r3
	movs	r1, #8
	subs	r1, r1, r3
	lsls	r1, r1, #16
	adds	r1, r1, r0
	ldr	r3, [r5, #16]
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	bl 0x0200da54
	adds	r0, r5, #0
	bl 0x0200da5c
	movs	r3, #192
	movs	r0, #128
	lsls	r3, r3, #8
	lsls	r0, r0, #2
	strh	r3, [r5, #6]
	adds	r0, #2
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_020039d4
	bl 0x0200b898
.L_020039d4:
	ldr	r1, [pc, #44]
	ldr	r0, [r6, #0]
	bl 0x0200dad4
	movs	r0, #12
	bl 0x0200d9ac
	movs	r0, #123
	bl 0x0200dc9c
	bl 0x0200dbf4
	bl 0x0200dbfc
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200dbc4
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0xdcd0
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	ldr	r5, [pc, #168]
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
	bl 0x0200dac4
	mov	sl, r0
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	mov	r2, sl
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200dacc
	mov	r2, r8
	ldr	r0, [r5, #0]
	adds	r1, r6, #0
	bl 0x0200dafc
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200db7c
	ldr	r0, [r5, #0]
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	ldr	r0, [r5, #0]
	movs	r1, #13
	bl 0x0200db24
	lsls	r6, r6, #16
	mov	r3, r8
	lsls	r3, r3, #16
	ldr	r2, [pc, #68]
	adds	r1, r6, #0
	mov	r0, sl
	mov	r8, r3
	bl 0x0200da54
	mov	r0, sl
	bl 0x0200da5c
	movs	r1, #10
	ldr	r0, [r5, #0]
	bl 0x0200db24
	movs	r0, #123
	bl 0x0200dc9c
	bl 0x0200dbf4
	bl 0x0200dbfc
	movs	r2, #170
	lsls	r2, r2, #1
	add	r9, r2
	mov	r2, r9
	movs	r3, #0
	ldrsh	r0, [r2, r3]
	bl 0x0200dbc4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff4
	.2byte 0xb500
	ldr	r3, [pc, #76]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200dac4
	ldr	r3, [r0, #8]
	asrs	r4, r3, #20
	ldr	r3, [r0, #16]
	asrs	r1, r3, #20
	cmp	r4, #42
	bne.n	.L_02003af4
	cmp	r1, #23
	bne.n	.L_02003af4
	ldr	r3, [pc, #48]
	movs	r2, #64
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003b14
.L_02003af4:
	cmp	r4, #43
	bne.n	.L_02003b08
	cmp	r1, #22
	bne.n	.L_02003b08
	ldr	r3, [pc, #28]
	movs	r2, #32
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003b14
.L_02003b08:
	movs	r0, #170
	movs	r1, #176
	lsls	r0, r0, #2
	lsls	r1, r1, #1
	bl 0x0200ba08
.L_02003b14:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x1150
	.2byte 0x0300
	push	{lr}
	ldr	r3, [pc, #92]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200dac4
	ldr	r3, [r0, #8]
	asrs	r1, r3, #20
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	cmp	r3, #38
	bne.n	.L_02003b5e
	cmp	r1, #23
	bne.n	.L_02003b4c
	ldr	r3, [pc, #64]
	movs	r2, #16
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003b7e
.L_02003b4c:
	cmp	r1, #25
	bne.n	.L_02003b72
	ldr	r3, [pc, #48]
	movs	r2, #32
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02003b72
	b.n	.L_02003b7e
.L_02003b5e:
	cmp	r1, #24
	bne.n	.L_02003b72
	cmp	r3, #39
	bne.n	.L_02003b72
	ldr	r3, [pc, #28]
	movs	r2, #64
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003b7e
.L_02003b72:
	movs	r0, #196
	movs	r1, #152
	lsls	r0, r0, #1
	lsls	r1, r1, #2
	bl 0x0200ba08
.L_02003b7e:
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x1150
	.2byte 0x0300
	push	{lr}
	ldr	r3, [pc, #92]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200dac4
	ldr	r3, [r0, #8]
	asrs	r1, r3, #20
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	cmp	r3, #54
	bne.n	.L_02003bc6
	cmp	r1, #42
	bne.n	.L_02003bb4
	ldr	r3, [pc, #64]
	movs	r2, #16
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003be6
.L_02003bb4:
	cmp	r1, #44
	bne.n	.L_02003bda
	ldr	r3, [pc, #48]
	movs	r2, #32
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02003bda
	b.n	.L_02003be6
.L_02003bc6:
	cmp	r1, #43
	bne.n	.L_02003bda
	cmp	r3, #55
	bne.n	.L_02003bda
	ldr	r3, [pc, #28]
	movs	r2, #64
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003be6
.L_02003bda:
	movs	r0, #174
	movs	r1, #216
	lsls	r0, r0, #2
	lsls	r1, r1, #2
	bl 0x0200ba08
.L_02003be6:
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x1150
	.2byte 0x0300
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_02003c04
	bl 0x0200b898
.L_02003c04:
	movs	r0, #188
	movs	r1, #144
	lsls	r0, r0, #1
	lsls	r1, r1, #1
	bl 0x0200ba08
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	bl 0x0200dbec
	bl 0x0200dbfc
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #76]
	bl 0x0200d9b4
	movs	r0, #20
	bl 0x0200da94
	movs	r5, #0
	b.n	.L_02003c3e
.L_02003c3c:
	adds	r5, #1
.L_02003c3e:
	cmp	r5, #159
	bhi.n	.L_02003c54
	movs	r0, #1
	bl 0x0200da94
	ldr	r3, [pc, #52]
	movs	r2, #11
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02003c3c
.L_02003c54:
	ldr	r0, [pc, #36]
	bl 0x0200d9bc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #218
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #32
	str	r2, [r3, #0]
	bl 0x0200dbf4
	bl 0x0200dbfc
	movs	r0, #11
	bl 0x0200dbc4
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0200ad65
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	movs	r0, #212
	bl 0x0200dc9c
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200dbd4
	movs	r0, #1
	bl 0x0200dbe4
	movs	r0, #1
	bl 0x0200d9ac
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200dbd4
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	mov	sl, r2
	movs	r2, #218
	lsls	r2, r2, #1
	movs	r6, #1
	str	r6, [r3, r2]
	mov	r8, r2
	bl 0x0200dbec
	bl 0x0200dbfc
	ldr	r5, [pc, #280]
	movs	r1, #144
	lsls	r1, r1, #3
	adds	r0, r5, #0
	bl 0x0200d9b4
	movs	r0, #60
	bl 0x0200d9ac
	adds	r0, r5, #0
	bl 0x0200d9bc
	movs	r0, #1
	bl 0x0200d9ac
	movs	r0, #212
	bl 0x0200dc9c
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200dbd4
	movs	r0, #1
	bl 0x0200dbe4
	movs	r0, #1
	bl 0x0200d9ac
	movs	r0, #146
	movs	r1, #1
	movs	r2, #248
	lsls	r2, r2, #16
	movs	r3, #0
	negs	r1, r1
	lsls	r0, r0, #18
	bl 0x0200dbac
	bl 0x0200da4c
	movs	r0, #1
	bl 0x0200d9ac
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200dbd4
	movs	r0, #1
	bl 0x0200dbe4
	movs	r0, #1
	bl 0x0200d9ac
	movs	r1, #144
	lsls	r1, r1, #3
	adds	r0, r5, #0
	bl 0x0200d9b4
	movs	r0, #60
	bl 0x0200d9ac
	adds	r0, r5, #0
	bl 0x0200d9bc
	movs	r0, #1
	bl 0x0200d9ac
	movs	r0, #212
	bl 0x0200dc9c
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200dbd4
	movs	r0, #1
	bl 0x0200dbe4
	movs	r0, #1
	bl 0x0200d9ac
	movs	r0, #248
	movs	r1, #1
	movs	r2, #134
	lsls	r2, r2, #18
	movs	r3, #0
	negs	r1, r1
	lsls	r0, r0, #16
	bl 0x0200dbac
	bl 0x0200da4c
	movs	r0, #1
	bl 0x0200d9ac
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200dbd4
	movs	r0, #1
	bl 0x0200dbe4
	movs	r0, #1
	bl 0x0200d9ac
	movs	r1, #144
	lsls	r1, r1, #3
	adds	r0, r5, #0
	bl 0x0200d9b4
	movs	r0, #180
	bl 0x0200d9ac
	adds	r0, r5, #0
	bl 0x0200d9bc
	movs	r0, #1
	bl 0x0200d9ac
	mov	r2, sl
	ldr	r3, [r2, #108]
	mov	r2, r8
	str	r6, [r3, r2]
	bl 0x0200dbf4
	bl 0x0200dbfc
	movs	r0, #10
	bl 0x0200da94
	movs	r0, #10
	bl 0x0200dbc4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0xad65
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	negs	r1, r1
	movs	r3, #0
	negs	r0, r0
	bl 0x0200dbac
	bl 0x0200dbbc
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	ldr	r0, [pc, #684]
	bl 0x0200db4c
	movs	r1, #0
	movs	r0, #17
	bl 0x0200db64
	movs	r0, #78
	bl 0x0200dc9c
	ldr	r6, [pc, #668]
	movs	r3, #133
	lsls	r3, r3, #2
	movs	r1, #204
	movs	r2, #204
	adds	r6, r6, r3
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r6, #0]
.L_02003e30:
	adds	r1, #204
	adds	r2, #102
	bl 0x0200dacc
	movs	r2, #200
	ldr	r0, [r6, #0]
	movs	r1, #86
	adds	r2, #255
	bl 0x0200dafc
	movs	r1, #172
	lsls	r1, r1, #15
	ldr	r2, [pc, #628]
	movs	r0, #26
	bl 0x0200db0c
	movs	r0, #1
	bl 0x0200d9ac
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #26
	adds	r1, #204
	adds	r2, #102
	bl 0x0200dacc
	movs	r2, #200
	movs	r0, #26
	movs	r1, #72
	adds	r2, #255
	bl 0x0200dafc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200db74
	movs	r1, #208
	movs	r0, #26
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #224
	movs	r2, #0
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r1, #152
	lsls	r1, r1, #7
	ldr	r0, [pc, #540]
	adds	r1, #204
	bl 0x0200dba4
	movs	r0, #132
	movs	r1, #128
	movs	r2, #210
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r1, r1, #13
	lsls	r0, r0, #17
	bl 0x0200dbac
	bl 0x0200dbb4
	movs	r0, #40
	bl 0x0200da94
	movs	r0, #204
	movs	r1, #192
	lsls	r0, r0, #7
	lsls	r1, r1, #4
	adds	r0, #102
	adds	r1, #204
	bl 0x0200dba4
	movs	r0, #132
	movs	r1, #128
	movs	r2, #160
.L_02003ee0:
	movs	r3, #1
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200dbac
	bl 0x0200dbb4
	movs	r0, #80
	bl 0x0200da94
	movs	r2, #40
	movs	r0, #17
	movs	r1, #4
	bl 0x0200db34
	movs	r0, #17
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #440]
	adds	r1, #153
	bl 0x0200dba4
	movs	r0, #132
	movs	r1, #128
	movs	r2, #200
	movs	r3, #1
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200dbac
	bl 0x0200dbb4
	movs	r0, #20
	bl 0x0200da94
	movs	r1, #192
	movs	r2, #20
	movs	r0, #17
	lsls	r1, r1, #6
.L_02003f38:
	bl 0x0200db74
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #17
	bl 0x0200db64
	movs	r0, #11
	bl 0x0200dc9c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #242
	bl 0x0200da14
	movs	r0, #148
	bl 0x0200dc9c
	movs	r0, #18
	bl 0x0200dac4
	movs	r5, #128
	lsls	r5, r5, #11
	str	r5, [r0, #40]
	movs	r0, #5
	bl 0x0200d9ac
	movs	r0, #19
	bl 0x0200dac4
	str	r5, [r0, #40]
	movs	r0, #5
	bl 0x0200d9ac
	movs	r0, #20
	bl 0x0200dac4
	str	r5, [r0, #40]
	movs	r0, #5
	bl 0x0200d9ac
	movs	r0, #21
	bl 0x0200dac4
	str	r5, [r0, #40]
	movs	r0, #5
	bl 0x0200d9ac
	movs	r0, #22
	bl 0x0200dac4
	str	r5, [r0, #40]
	movs	r0, #5
	bl 0x0200d9ac
	movs	r0, #23
	bl 0x0200dac4
	str	r5, [r0, #40]
	movs	r0, #5
	bl 0x0200d9ac
	movs	r0, #24
	bl 0x0200dac4
	str	r5, [r0, #40]
	movs	r0, #5
	bl 0x0200d9ac
	movs	r0, #25
	bl 0x0200dac4
	str	r5, [r0, #40]
	movs	r0, #20
	bl 0x0200d9ac
	movs	r1, #208
	movs	r2, #20
	lsls	r1, r1, #8
	movs	r0, #17
	bl 0x0200db74
	bl 0x0200b2b0
	movs	r0, #40
	bl 0x0200da94
	movs	r0, #204
	movs	r1, #192
	lsls	r0, r0, #6
	lsls	r1, r1, #3
	adds	r0, #51
	adds	r1, #102
	bl 0x0200dba4
	movs	r0, #132
	movs	r2, #200
	lsls	r2, r2, #17
	movs	r3, #1
	movs	r1, #0
	lsls	r0, r0, #17
	bl 0x0200dbac
	bl 0x0200dbb4
	movs	r0, #200
	bl 0x0200da94
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #176]
	adds	r1, #153
	bl 0x0200dba4
	movs	r0, #222
	movs	r1, #128
	movs	r2, #228
	movs	r3, #1
	lsls	r0, r0, #15
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	bl 0x0200dbac
	bl 0x0200dbb4
	movs	r2, #0
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db74
	movs	r0, #26
.L_02004040:
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r2, #20
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	bl 0x0200db74
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #26
	movs	r1, #4
	bl 0x0200db24
	movs	r0, #26
	movs	r1, #0
	bl 0x0200db64
	ldr	r0, [r6, #0]
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #26
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #26
	movs	r1, #2
	bl 0x0200db24
	ldr	r0, [r6, #0]
	bl 0x0200dac4
	cmp	r0, #0
	beq.n	.L_0200409a
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #26
	bl 0x0200daec
.L_0200409a:
	movs	r0, #26
	bl 0x0200db04
	movs	r0, #26
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db0c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #241
	bl 0x0200da14
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00001e65
	.4byte 0x02000240
	.4byte 0x01c70000
	.4byte 0x00026666
	.2byte 0xcccc
	.2byte 0x0004
	push	{r5, r6, lr}
	ldr	r5, [pc, #432]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	ldr	r0, [r5, #0]
	bl 0x0200dac4
	movs	r1, #15
	bl 0x0200db44
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	movs	r3, #0
	negs	r1, r1
	negs	r0, r0
	bl 0x0200dbac
	bl 0x0200dbbc
	movs	r6, #0
	adds	r0, #85
	strb	r6, [r0, #0]
	bl 0x0200dbec
	bl 0x0200dbfc
	ldr	r5, [pc, #368]
	movs	r1, #144
	lsls	r1, r1, #3
	adds	r0, r5, #0
	bl 0x0200d9b4
	movs	r0, #40
	bl 0x0200da94
	movs	r0, #78
	bl 0x0200dc9c
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200dba4
	movs	r0, #132
	movs	r1, #1
	movs	r2, #194
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	bl 0x0200dbac
	bl 0x0200dbb4
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #6
	bl 0x0200db7c
	movs	r1, #3
	movs	r0, #17
	bl 0x0200db2c
	ldr	r0, [pc, #292]
	bl 0x0200db4c
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #17
	bl 0x0200db64
	movs	r0, #26
	bl 0x0200dc9c
.L_0200417a:
	movs	r0, #27
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	movs	r0, #27
	bl 0x0200dac4
	adds	r0, #85
	strb	r6, [r0, #0]
	movs	r0, #27
	bl 0x0200dac4
	movs	r2, #192
	movs	r3, #172
	lsls	r2, r2, #13
	lsls	r3, r3, #17
	ldr	r1, [pc, #236]
	bl 0x0200da44
	movs	r0, #204
	movs	r1, #192
	lsls	r0, r0, #7
	lsls	r1, r1, #4
	adds	r0, #102
	adds	r1, #204
	bl 0x0200dba4
	movs	r0, #132
	movs	r1, #1
	movs	r2, #170
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200dbac
	movs	r1, #204
	movs	r2, #200
	lsls	r1, r1, #6
	lsls	r2, r2, #5
	movs	r0, #17
	adds	r1, #51
	adds	r2, #153
	bl 0x0200dacc
	movs	r1, #132
	movs	r2, #187
	lsls	r2, r2, #1
	lsls	r1, r1, #1
	movs	r0, #17
	bl 0x0200dafc
	movs	r0, #1
	bl 0x0200d9ac
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #156]
	bl 0x0200d9b4
	movs	r0, #20
	bl 0x0200da94
	adds	r0, r5, #0
	bl 0x0200d9bc
	bl 0x0200b2e0
	movs	r1, #144
	adds	r0, r5, #0
	lsls	r1, r1, #3
	bl 0x0200d9b4
	movs	r1, #0
	movs	r0, #17
	bl 0x0200db64
	movs	r0, #11
	bl 0x0200dc9c
	bl 0x0200b610
	movs	r0, #160
	bl 0x0200da94
	movs	r2, #80
	movs	r1, #0
	movs	r0, #17
	bl 0x0200db5c
	movs	r0, #144
	lsls	r0, r0, #2
	bl 0x0200dc9c
	movs	r1, #1
	movs	r0, #1
	bl 0x0200b640
	movs	r0, #20
	bl 0x0200da94
	movs	r1, #0
	movs	r2, #40
	movs	r0, #17
	bl 0x0200db5c
	adds	r0, r5, #0
	bl 0x0200d9bc
	movs	r0, #1
	bl 0x0200d9ac
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #86
	str	r2, [r3, #0]
	bl 0x0200dbf4
	bl 0x0200dbfc
	movs	r0, #10
	bl 0x0200dbc4
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200accd
	.4byte 0x00001f27
	.4byte 0x01090000
	.2byte 0xaeb1
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r5, [pc, #372]
.L_02004298:
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r5, r5, r1
	ldr	r0, [r5, #0]
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	ldr	r0, [r5, #0]
	bl 0x0200dac4
	movs	r1, #15
	bl 0x0200db44
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	movs	r3, #0
	negs	r1, r1
	negs	r0, r0
	bl 0x0200dbac
	bl 0x0200dbbc
	movs	r6, #0
	adds	r0, #85
	strb	r6, [r0, #0]
	movs	r0, #27
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	movs	r0, #27
	bl 0x0200dac4
	adds	r0, #85
	strb	r6, [r0, #0]
	movs	r0, #27
	bl 0x0200dac4
	movs	r2, #192
	movs	r3, #172
	lsls	r2, r2, #13
	lsls	r3, r3, #17
	ldr	r1, [pc, #280]
	bl 0x0200da44
	movs	r0, #1
	movs	r1, #1
	bl 0x0200b640
	movs	r0, #1
	bl 0x0200d9ac
	ldr	r5, [pc, #264]
	ldr	r2, [r5, #0]
	cmp	r2, #0
	beq.n	.L_0200432a
	ldr	r3, [r2, #12]
	movs	r1, #128
	lsls	r1, r1, #14
	adds	r3, r3, r1
	str	r3, [r2, #12]
	movs	r0, #1
	bl 0x0200d9ac
	ldr	r3, [r5, #0]
	movs	r2, #4
	adds	r3, #85
	strb	r2, [r3, #0]
.L_0200432a:
	bl 0x0200dbec
	bl 0x0200dbfc
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #224]
	bl 0x0200d9b4
	movs	r0, #40
	bl 0x0200da94
	movs	r0, #161
	bl 0x0200dc9c
	movs	r1, #1
	movs	r0, #0
	bl 0x0200b640
	movs	r0, #20
	bl 0x0200da94
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #17
	bl 0x0200db9c
	movs	r0, #20
	bl 0x0200da94
	ldr	r0, [pc, #180]
	bl 0x0200db4c
	movs	r0, #17
	movs	r1, #0
	bl 0x0200db64
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_020043ac
	adds	r2, r3, #0
	adds	r2, #85
	strb	r6, [r2, #0]
	b.n	.L_020043a2
.L_02004382:
	movs	r1, #208
	movs	r0, #17
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	ldr	r5, [pc, #132]
	ldr	r1, [pc, #140]
	ldr	r2, [r5, #0]
	movs	r0, #1
	ldr	r3, [r2, #12]
	adds	r3, r3, r1
	str	r3, [r2, #12]
	bl 0x0200d9ac
	ldr	r3, [r5, #0]
.L_020043a2:
	movs	r2, #144
	ldr	r3, [r3, #12]
	lsls	r2, r2, #14
	cmp	r3, r2
	bgt.n	.L_02004382
.L_020043ac:
	movs	r0, #17
	movs	r1, #4
	bl 0x0200db2c
	movs	r0, #17
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #6
	bl 0x0200db7c
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #17
	bl 0x0200db64
	movs	r0, #255
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200da1c
	movs	r0, #11
	bl 0x0200dc9c
	movs	r0, #80
	bl 0x0200da94
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	movs	r2, #129
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	adds	r3, r3, r1
	adds	r2, #255
	str	r2, [r3, #0]
	bl 0x0200dbf4
	bl 0x0200dbfc
	movs	r0, #11
	bl 0x0200dbc4
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x01090000
	.4byte 0x0200f950
	.4byte 0x0200accd
	.4byte 0x00001f2b
	.2byte 0xc000
	.2byte 0xffff
	.2byte 0xb560
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	ldr	r5, [pc, #224]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl 0x0200dac4
	movs	r1, #15
	bl 0x0200db44
	ldr	r0, [r5, #0]
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	bl 0x0200b2b0
	ldr	r5, [pc, #192]
	movs	r1, #144
	lsls	r1, r1, #3
	adds	r0, r5, #0
	bl 0x0200d9b4
	movs	r3, #192
	lsls	r3, r3, #18
	mov	sl, r3
	movs	r6, #129
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r6, r6, #1
	lsls	r2, r2, #1
	adds	r6, #255
	str	r6, [r3, r2]
	mov	r8, r2
	bl 0x0200dbec
	bl 0x0200dbfc
	movs	r0, #40
	bl 0x0200da94
	ldr	r0, [pc, #148]
	bl 0x0200db4c
	movs	r0, #17
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #6
	bl 0x0200db7c
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #17
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #160
	movs	r0, #17
	lsls	r1, r1, #7
	bl 0x0200db7c
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #17
	bl 0x0200db64
	adds	r0, r5, #0
	bl 0x0200d9bc
	movs	r1, #208
	movs	r0, #17
.L_020044ce:
	lsls	r1, r1, #8
	bl 0x0200db7c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200dbdc
	movs	r1, #0
	movs	r0, #0
	bl 0x0200dbd4
	mov	r2, sl
	ldr	r3, [r2, #108]
	mov	r2, r8
	str	r6, [r3, r2]
	bl 0x0200dbf4
	bl 0x0200dbfc
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #252
	bl 0x0200da14
	movs	r0, #10
	adds	r0, #255
	bl 0x0200da14
	movs	r0, #12
	bl 0x0200dbc4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200accd
	.2byte 0x1f45
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	negs	r1, r1
	movs	r3, #0
.L_0200453c:
	negs	r0, r0
	bl 0x0200dbac
	bl 0x0200dbbc
	movs	r3, #0
	adds	r0, #85
	ldr	r5, [pc, #120]
	strb	r3, [r0, #0]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r5, r5, r1
	ldr	r0, [r5, #0]
	bl 0x0200dac4
	movs	r1, #15
	bl 0x0200db44
	ldr	r0, [r5, #0]
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #84]
	bl 0x0200d9b4
	movs	r0, #1
	movs	r1, #1
	bl 0x0200b640
	ldr	r5, [pc, #76]
	ldr	r2, [r5, #0]
	cmp	r2, #0
	beq.n	.L_0200459e
	ldr	r3, [r2, #12]
	movs	r1, #128
	lsls	r1, r1, #14
	adds	r3, r3, r1
	str	r3, [r2, #12]
	movs	r0, #1
	bl 0x0200d9ac
	ldr	r3, [r5, #0]
	movs	r2, #4
	adds	r3, #85
	strb	r2, [r3, #0]
.L_0200459e:
	movs	r0, #1
	bl 0x0200d9ac
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #85
	str	r2, [r3, #0]
	bl 0x0200dbec
	bl 0x0200dbfc
	movs	r0, #40
	bl 0x0200da94
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x0200accd
	.2byte 0xf950
	.2byte 0x0200
	push	{r5, lr}
	movs	r1, #153
	lsls	r1, r1, #8
	ldr	r0, [pc, #516]
	adds	r1, #153
	bl 0x0200dba4
	movs	r0, #132
	movs	r2, #200
	movs	r3, #1
	movs	r1, #0
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200dbac
	bl 0x0200dbb4
	movs	r0, #10
	bl 0x0200da94
	movs	r0, #18
	movs	r1, #6
	movs	r2, #0
	bl 0x0200db34
	movs	r0, #19
	movs	r1, #6
	movs	r2, #0
	bl 0x0200db34
	movs	r0, #20
	movs	r1, #6
	movs	r2, #0
	bl 0x0200db34
	movs	r0, #21
	movs	r1, #6
	movs	r2, #0
	bl 0x0200db34
	movs	r0, #22
	movs	r1, #6
	movs	r2, #0
	bl 0x0200db34
	movs	r0, #23
	movs	r1, #6
	movs	r2, #0
	bl 0x0200db34
	movs	r0, #24
	movs	r1, #6
	movs	r2, #0
	bl 0x0200db34
	movs	r2, #40
	movs	r0, #25
	movs	r1, #6
	bl 0x0200db34
	movs	r1, #160
	movs	r0, #20
	lsls	r1, r1, #7
	bl 0x0200db7c
	movs	r2, #0
	movs	r0, #20
	movs	r1, #2
	bl 0x0200db34
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #20
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #192
	movs	r0, #23
	lsls	r1, r1, #6
	bl 0x0200db7c
	movs	r2, #0
	movs	r0, #23
	movs	r1, #2
	bl 0x0200db34
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #23
	movs	r1, #0
	bl 0x0200db64
	ldr	r5, [pc, #340]
	movs	r0, #18
	adds	r1, r5, #0
	bl 0x0200dad4
	adds	r1, r5, #0
	movs	r0, #19
	bl 0x0200dad4
	adds	r1, r5, #0
	movs	r0, #20
	bl 0x0200dad4
	adds	r1, r5, #0
	movs	r0, #21
	bl 0x0200dad4
	adds	r1, r5, #0
	movs	r0, #22
	bl 0x0200dad4
	adds	r1, r5, #0
	movs	r0, #23
	bl 0x0200dad4
	adds	r1, r5, #0
	movs	r0, #24
	bl 0x0200dad4
	adds	r1, r5, #0
	movs	r0, #25
	bl 0x0200dad4
	movs	r0, #40
	bl 0x0200da94
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #6
	bl 0x0200db7c
	movs	r0, #17
	movs	r1, #2
	movs	r2, #10
	bl 0x0200db34
	movs	r2, #10
	movs	r0, #17
	movs	r1, #4
	bl 0x0200db34
	movs	r1, #0
	movs	r0, #17
	bl 0x0200db64
	movs	r0, #18
	bl 0x0200dadc
	movs	r0, #19
	bl 0x0200dadc
	movs	r0, #20
	bl 0x0200dadc
	movs	r0, #21
	bl 0x0200dadc
	movs	r0, #22
	bl 0x0200dadc
	movs	r0, #23
	bl 0x0200dadc
	movs	r0, #24
	bl 0x0200dadc
	movs	r0, #25
	bl 0x0200dadc
	movs	r0, #40
	bl 0x0200da94
	movs	r1, #208
	movs	r0, #18
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #208
	movs	r0, #19
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #208
	movs	r0, #20
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #208
	movs	r0, #21
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #176
	movs	r0, #22
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #176
	movs	r0, #23
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #176
	movs	r0, #24
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #176
	movs	r0, #25
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #8
	movs	r2, #0
	adds	r1, #255
	movs	r0, #17
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #17
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #17
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #17
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #17
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #17
	movs	r1, #4
	movs	r2, #10
	bl 0x0200db34
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #17
	movs	r1, #0
	bl 0x0200db64
	pop	{r5, pc}
	.4byte 0x0004cccc
	.2byte 0xe1a8
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200c524
	movs	r0, #161
	bl 0x0200dc9c
	movs	r1, #1
	movs	r0, #0
	bl 0x0200b640
	movs	r0, #20
	bl 0x0200da94
	ldr	r0, [pc, #144]
	bl 0x0200db4c
	movs	r0, #17
	movs	r1, #0
	bl 0x0200db64
	ldr	r3, [pc, #132]
	ldr	r2, [r3, #0]
	cmp	r2, #0
	beq.n	.L_02004850
	adds	r1, r2, #0
	adds	r1, #85
	movs	r3, #0
	strb	r3, [r1, #0]
	movs	r1, #144
	ldr	r3, [r2, #12]
	lsls	r1, r1, #14
	cmp	r3, r1
	ble.n	.L_02004850
.L_02004826:
	movs	r1, #208
	movs	r0, #17
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	ldr	r5, [pc, #96]
	ldr	r1, [pc, #96]
	ldr	r2, [r5, #0]
	movs	r0, #1
	ldr	r3, [r2, #12]
	adds	r3, r3, r1
	str	r3, [r2, #12]
	bl 0x0200d9ac
	ldr	r3, [r5, #0]
	movs	r2, #144
	ldr	r3, [r3, #12]
	lsls	r2, r2, #14
	cmp	r3, r2
	bgt.n	.L_02004826
.L_02004850:
	movs	r0, #17
	movs	r1, #4
	bl 0x0200db2c
	movs	r0, #17
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #6
	bl 0x0200db7c
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #17
	bl 0x0200db64
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #252
	bl 0x0200da14
	movs	r0, #10
	adds	r0, #255
	bl 0x0200da14
	movs	r0, #12
	bl 0x0200dbc4
	pop	{r5, pc}
	.4byte 0x00001f49
	.4byte 0x0200f950
	.2byte 0xc000
	.2byte 0xffff
	.2byte 0xb520
	bl 0x0200c524
	movs	r0, #144
	lsls	r0, r0, #2
	bl 0x0200dc9c
	movs	r0, #12
	bl 0x0200dac4
	movs	r1, #7
	bl 0x0200db44
	movs	r0, #13
	bl 0x0200dac4
	movs	r1, #7
	bl 0x0200db44
	movs	r0, #8
	bl 0x0200d9ac
	movs	r0, #12
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200db44
	movs	r0, #13
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200db44
	movs	r0, #8
	bl 0x0200d9ac
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #17
	bl 0x0200db94
	ldr	r0, [pc, #120]
	bl 0x0200db4c
	movs	r1, #0
	movs	r0, #17
	bl 0x0200db64
	movs	r0, #161
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200dc9c
	ldr	r5, [pc, #100]
	movs	r0, #12
	adds	r1, r5, #0
	bl 0x0200dad4
	adds	r1, r5, #0
	movs	r0, #13
	bl 0x0200dae4
	ldr	r5, [pc, #84]
	movs	r0, #12
	adds	r1, r5, #0
	bl 0x0200dad4
	adds	r1, r5, #0
	movs	r0, #13
	bl 0x0200dad4
	movs	r0, #12
	bl 0x0200dac4
	movs	r1, #12
	bl 0x0200db44
	movs	r0, #13
	bl 0x0200dac4
	movs	r1, #12
	bl 0x0200db44
	movs	r0, #120
	bl 0x0200da94
	bl 0x0200c5d0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #252
	bl 0x0200da14
	movs	r0, #10
	adds	r0, #255
	bl 0x0200da14
	movs	r0, #12
	bl 0x0200dbc4
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00001f4c
	.4byte 0x0200e14c
	.2byte 0xe13c
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200c524
	movs	r0, #144
	lsls	r0, r0, #2
	bl 0x0200dc9c
	movs	r0, #12
	bl 0x0200dac4
	movs	r1, #7
	bl 0x0200db44
	movs	r0, #13
	bl 0x0200dac4
	movs	r1, #7
	bl 0x0200db44
	movs	r0, #8
	bl 0x0200d9ac
	movs	r0, #12
	bl 0x0200dac4
	ldr	r5, [pc, #112]
	str	r5, [r0, #108]
	movs	r0, #13
	bl 0x0200dac4
	movs	r1, #128
	str	r5, [r0, #108]
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #17
	bl 0x0200db94
	ldr	r0, [pc, #92]
	bl 0x0200db4c
	movs	r2, #20
	movs	r1, #0
	movs	r0, #17
	bl 0x0200db5c
	movs	r0, #144
	lsls	r0, r0, #2
	bl 0x0200dc9c
	movs	r1, #0
	movs	r0, #1
	bl 0x0200b6e0
	movs	r0, #20
	bl 0x0200da94
	movs	r0, #162
	bl 0x0200dc9c
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl 0x0200d9b4
	movs	r0, #120
	bl 0x0200da94
	bl 0x0200c5d0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #252
	bl 0x0200da14
	movs	r0, #10
	adds	r0, #255
	bl 0x0200da14
	movs	r0, #12
	bl 0x0200dbc4
	pop	{r5, pc}
	.4byte 0x020080a1
	.4byte 0x00001f54
	.2byte 0xaf35
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #14
	bl 0x0200dac4
	adds	r6, r0, #0
	bl 0x0200c524
	movs	r1, #1
	movs	r0, #17
	bl 0x0200db84
	movs	r0, #144
	lsls	r0, r0, #2
	bl 0x0200dc9c
	movs	r0, #12
	bl 0x0200dac4
	movs	r1, #7
	bl 0x0200db44
	movs	r0, #13
	bl 0x0200dac4
	movs	r1, #7
	bl 0x0200db44
	movs	r0, #8
	bl 0x0200d9ac
	movs	r0, #12
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200db44
	movs	r0, #13
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200db44
	movs	r0, #8
	bl 0x0200d9ac
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #17
	bl 0x0200db94
	ldr	r0, [pc, #596]
	bl 0x0200db4c
	movs	r2, #20
	movs	r1, #0
	movs	r0, #17
	bl 0x0200db5c
	movs	r0, #144
	lsls	r0, r0, #2
	bl 0x0200dc9c
	movs	r1, #0
	movs	r0, #1
	bl 0x0200b6e0
	movs	r0, #20
	bl 0x0200da94
	movs	r0, #233
	bl 0x0200dc9c
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r6, #28]
	movs	r0, #14
	bl 0x0200dac4
	movs	r1, #132
	movs	r2, #192
	movs	r3, #154
	lsls	r1, r1, #17
	lsls	r2, r2, #13
	lsls	r3, r3, #17
	bl 0x0200da44
	movs	r0, #1
	bl 0x0200d9ac
	movs	r0, #14
	movs	r1, #3
	bl 0x0200db24
	movs	r5, #0
.L_02004ae8:
	ldr	r3, [r6, #28]
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r3, r3, r1
	str	r3, [r6, #28]
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	str	r3, [r6, #12]
	movs	r0, #1
	adds	r5, #1
	bl 0x0200d9ac
	cmp	r5, #63
	bls.n	.L_02004ae8
	movs	r0, #149
	lsls	r0, r0, #1
	bl 0x0200dc9c
	ldr	r3, [pc, #468]
	ldr	r2, [r3, #0]
	cmp	r2, #0
	beq.n	.L_02004b48
	adds	r1, r2, #0
	adds	r1, #85
	movs	r3, #0
	strb	r3, [r1, #0]
	movs	r1, #144
	ldr	r3, [r2, #12]
	lsls	r1, r1, #14
	cmp	r3, r1
	ble.n	.L_02004b48
.L_02004b2a:
	ldr	r5, [pc, #444]
	ldr	r1, [pc, #444]
	ldr	r2, [r5, #0]
.L_02004b30:
	movs	r0, #1
	ldr	r3, [r2, #12]
	adds	r3, r3, r1
	str	r3, [r2, #12]
	bl 0x0200d9ac
	ldr	r3, [r5, #0]
	movs	r2, #144
	ldr	r3, [r3, #12]
	lsls	r2, r2, #14
	cmp	r3, r2
	bgt.n	.L_02004b2a
.L_02004b48:
	movs	r0, #232
	bl 0x0200dc9c
	ldr	r3, [pc, #408]
	ldr	r2, [r3, #0]
	cmp	r2, #0
	beq.n	.L_02004bb2
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r5, #0
.L_02004b5e:
	ldr	r7, [pc, #392]
	movs	r1, #192
	ldr	r2, [r7, #0]
	lsls	r1, r1, #11
	ldr	r3, [r2, #12]
	movs	r0, #1
	adds	r3, r3, r1
	str	r3, [r2, #12]
	ldr	r2, [pc, #384]
	ldr	r3, [r6, #28]
	ldr	r1, [pc, #376]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r3, [r6, #12]
	adds	r5, #1
	adds	r3, r3, r1
	str	r3, [r6, #12]
	bl 0x0200d9ac
	cmp	r5, #3
	bls.n	.L_02004b5e
	ldr	r0, [r7, #0]
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x0200da44
	movs	r5, #0
.L_02004b96:
	ldr	r3, [r6, #28]
	ldr	r2, [pc, #340]
	ldr	r1, [pc, #336]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r3, [r6, #12]
	movs	r0, #1
	adds	r3, r3, r1
	str	r3, [r6, #12]
	adds	r5, #1
	bl 0x0200d9ac
	cmp	r5, #3
	bls.n	.L_02004b96
.L_02004bb2:
	movs	r2, #0
	movs	r0, #14
	movs	r1, #0
	bl 0x0200db0c
	movs	r0, #0
	movs	r1, #0
	bl 0x0200b6e0
	movs	r1, #1
	movs	r0, #1
	bl 0x0200b640
	movs	r0, #10
	bl 0x0200da94
	movs	r1, #1
	movs	r0, #0
	bl 0x0200b640
	movs	r0, #40
	bl 0x0200da94
	movs	r0, #144
	lsls	r0, r0, #2
	bl 0x0200dc9c
	movs	r0, #1
	movs	r1, #1
	bl 0x0200b640
	movs	r1, #0
	movs	r0, #1
	bl 0x0200b6e0
	movs	r0, #132
	bl 0x0200dc9c
	movs	r0, #14
	bl 0x0200dac4
	movs	r1, #132
	movs	r2, #192
	movs	r3, #154
	lsls	r1, r1, #17
	lsls	r2, r2, #13
	lsls	r3, r3, #17
	bl 0x0200da44
	movs	r0, #1
	bl 0x0200d9ac
	movs	r5, #0
.L_02004c1c:
	ldr	r3, [r6, #28]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r3, [r6, #12]
	movs	r1, #128
	lsls	r1, r1, #8
	adds	r3, r3, r1
	str	r3, [r6, #12]
	movs	r0, #1
	adds	r5, #1
	bl 0x0200d9ac
	cmp	r5, #7
	bls.n	.L_02004c1c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #17
	bl 0x0200db9c
	movs	r0, #40
	bl 0x0200da94
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #6
	bl 0x0200db7c
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #17
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #17
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #17
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #17
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #17
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #17
	adds	r1, #204
	adds	r2, #102
	bl 0x0200dacc
	movs	r1, #132
	movs	r2, #160
	movs	r0, #17
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200dafc
	movs	r1, #0
	movs	r0, #17
	movs	r2, #0
	bl 0x0200db0c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #87
	str	r2, [r3, #0]
	bl 0x0200dbf4
	bl 0x0200dbfc
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #251
	bl 0x0200da14
	movs	r0, #13
	bl 0x0200dbc4
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00001f5c
	.4byte 0x0200f950
	.4byte 0xffff8000
	.2byte 0xe000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	bl 0x0200dbac
	movs	r0, #192
	movs	r1, #1
	negs	r1, r1
	ldr	r2, [pc, #772]
	movs	r3, #0
	lsls	r0, r0, #17
	bl 0x0200dbac
	movs	r0, #1
	bl 0x0200d9ac
	ldr	r5, [pc, #760]
	movs	r2, #133
	lsls	r2, r2, #2
	movs	r6, #192
	adds	r5, r5, r2
	lsls	r6, r6, #8
	movs	r1, #185
	movs	r2, #159
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	adds	r3, r6, #0
	ldr	r0, [r5, #0]
	movs	r7, #192
	bl 0x0200db14
	lsls	r7, r7, #18
	bl 0x0200da4c
	movs	r0, #1
	bl 0x0200d9ac
	ldr	r3, [r7, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	subs	r2, #172
	str	r2, [r3, #0]
	movs	r0, #0
	movs	r1, #0
	mov	r8, r2
	bl 0x0200dbdc
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	adds	r0, #2
	bl 0x0200dbd4
	ldr	r3, [r7, #108]
	movs	r2, #218
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #60
	str	r2, [r3, #0]
	bl 0x0200dbec
	bl 0x0200dbfc
	movs	r0, #40
	bl 0x0200da94
	movs	r1, #176
	movs	r0, #10
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200db74
	movs	r1, #208
	movs	r2, #20
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x0200db74
	movs	r0, #10
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #9
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #8
	movs	r1, #3
	bl 0x0200db2c
	movs	r1, #176
	movs	r0, #10
	lsls	r1, r1, #8
	movs	r2, #20
	bl 0x0200db74
	movs	r1, #208
	movs	r2, #20
	lsls	r1, r1, #8
	movs	r0, #10
	bl 0x0200db74
	movs	r0, #10
	bl 0x0200dac4
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r0, #40]
	movs	r0, #40
	bl 0x0200da94
	movs	r0, #9
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #8
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200dbd4
	movs	r0, #40
	bl 0x0200dbe4
	movs	r0, #60
	bl 0x0200d9ac
	movs	r1, #3
	movs	r0, #10
	bl 0x0200db2c
	ldr	r0, [pc, #528]
	bl 0x0200db4c
	movs	r0, #10
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #9
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #40
	bl 0x0200db74
	movs	r1, #208
	movs	r0, #10
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r2, #10
	ldr	r0, [r5, #0]
	adds	r1, r6, #0
	bl 0x0200db74
	movs	r0, #10
	movs	r1, #3
	bl 0x0200db24
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200db2c
	movs	r2, #10
	movs	r0, #8
	movs	r1, #4
	bl 0x0200db34
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #9
	movs	r1, #0
	bl 0x0200db7c
	movs	r1, #4
	movs	r2, #20
	adds	r1, #255
	movs	r0, #8
	bl 0x0200db94
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200db64
	movs	r2, #20
	mov	r1, r8
	movs	r0, #9
	bl 0x0200db94
	movs	r0, #9
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #8
	bl 0x0200db7c
	movs	r0, #8
	movs	r1, #4
	bl 0x0200db24
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	bl 0x0200db7c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #8
	movs	r1, #4
	bl 0x0200db24
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #208
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #8
	ldr	r1, [pc, #280]
	adds	r2, #204
	bl 0x0200dacc
	movs	r1, #182
	movs	r2, #144
	movs	r0, #8
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200dafc
	movs	r1, #176
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200db74
	movs	r1, #6
	movs	r2, #80
	adds	r1, #255
	movs	r0, #9
	bl 0x0200db94
	movs	r0, #9
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #6
	bl 0x0200db7c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #9
	movs	r1, #4
	bl 0x0200db24
	movs	r0, #9
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #9
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #10
	movs	r1, #3
	bl 0x0200db2c
	movs	r0, #10
	movs	r1, #0
	bl 0x0200db64
	movs	r0, #10
	movs	r1, #0
	bl 0x0200db7c
	movs	r0, #10
	movs	r1, #0
	bl 0x0200db64
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200db7c
	movs	r0, #10
	movs	r1, #3
	bl 0x0200db24
	movs	r0, #10
	movs	r1, #0
	bl 0x0200db64
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200db2c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #10
	adds	r1, #204
	adds	r2, #102
	bl 0x0200dacc
	movs	r0, #10
	movs	r1, #2
	bl 0x0200db24
	ldr	r0, [r5, #0]
	bl 0x0200dac4
	cmp	r0, #0
	beq.n	.L_02004fea
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #10
	bl 0x0200daec
.L_02004fea:
	movs	r0, #10
	bl 0x0200db04
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db0c
	ldr	r1, [r7, #108]
	movs	r3, #218
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #16
	str	r3, [r2, #0]
	movs	r3, #214
	lsls	r3, r3, #1
	movs	r0, #254
	adds	r2, r1, r3
	lsls	r0, r0, #3
	adds	r3, #93
	str	r3, [r2, #0]
	adds	r0, #255
	bl 0x0200da14
	bl 0x0200daa4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02590000
	.4byte 0x02000240
	.4byte 0x00001fee
	.2byte 0x9999
	.2byte 0x0001
	push	{lr}
	ldr	r3, [pc, #84]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_0200504e
	bl 0x0200d114
	b.n	.L_02005088
.L_0200504e:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_0200505a
	bl 0x0200d16c
	b.n	.L_02005088
.L_0200505a:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_02005066
	bl 0x0200d4e0
	b.n	.L_02005088
.L_02005066:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_02005072
	bl 0x0200d594
	b.n	.L_02005088
.L_02005072:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_0200507e
	bl 0x0200d7e0
	b.n	.L_02005088
.L_0200507e:
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_02005088
	bl 0x0200d898
.L_02005088:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000006e
	.4byte 0x00000071
	.4byte 0x0000006f
	.4byte 0x00000072
	.4byte 0x0000007d
	.2byte 0x0070
	.2byte 0x0000
	movs	r0, #0
	bx	lr
	push	{r5, lr}
	movs	r0, #9
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	movs	r0, #10
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	movs	r0, #0
	bl 0x0200dc04
	ldr	r0, [pc, #64]
	bl 0x0200dc64
	movs	r0, #8
	movs	r1, #3
	bl 0x0200db84
	bl 0x0200dc54
	movs	r1, #144
	lsls	r1, r1, #4
	adds	r1, #233
	movs	r2, #12
	movs	r3, #13
	movs	r0, #0
	bl 0x0200dc5c
	movs	r0, #22
	bl 0x0200dac4
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r5, #128
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #23
	bl 0x0200dac4
	adds	r0, #89
	ldrb	r3, [r0, #0]
	orrs	r5, r3
	strb	r5, [r0, #0]
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0xe324
	.2byte 0x0200
	push	{lr}
	movs	r1, #176
	movs	r2, #157
	lsls	r2, r2, #17
	lsls	r1, r1, #15
	movs	r0, #11
	bl 0x0200db0c
	movs	r0, #11
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	movs	r0, #11
	bl 0x0200dac4
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r0, #28]
	bl 0x0200d0ac
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #107
	bl 0x0200da1c
	movs	r0, #25
	bl 0x0200db8c
	movs	r0, #25
	movs	r1, #2
	bl 0x0200db24
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #8]
	bl 0x0200d9b4
	bl 0x0200b184
	pop	{pc}
	.2byte 0xb031
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #11
	bl 0x0200dac4
	adds	r7, r0, #0
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	movs	r0, #10
	adds	r0, #255
	bl 0x0200da0c
	cmp	r0, #0
	bne.n	.L_0200526a
	adds	r3, r7, #0
	adds	r3, #85
	strb	r0, [r3, #0]
	movs	r3, #168
	lsls	r3, r3, #14
	ldr	r1, [pc, #292]
	str	r3, [r7, #12]
	ldr	r3, [r7, #16]
	movs	r0, #11
	adds	r3, r3, r1
	str	r3, [r7, #16]
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	movs	r1, #1
	movs	r0, #11
	bl 0x0200db84
	ldr	r3, [pc, #264]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #19
	bne.n	.L_0200526a
	movs	r0, #7
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_0200526a
	movs	r0, #7
	bl 0x0200da04
	adds	r5, r0, #0
	ldrh	r1, [r5, #52]
	ldrh	r3, [r5, #54]
	strh	r1, [r5, #56]
	strh	r3, [r5, #58]
	lsls	r1, r1, #16
	asrs	r1, r1, #16
	lsls	r0, r1, #14
	bl 0x0200d99c
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_020051f8
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_020051f8
	adds	r3, r0, #0
.L_020051f8:
	strh	r3, [r5, #20]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_0200520c
	movs	r2, #56
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L_0200520c
	movs	r3, #1
	strh	r3, [r5, #20]
.L_0200520c:
	movs	r3, #58
	ldrsh	r0, [r5, r3]
	movs	r2, #54
	ldrsh	r1, [r5, r2]
	lsls	r0, r0, #14
	bl 0x0200d99c
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_0200522a
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_0200522a
	adds	r3, r0, #0
.L_0200522a:
	strh	r3, [r5, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_0200523e
	movs	r1, #58
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	beq.n	.L_0200523e
	movs	r3, #1
	strh	r3, [r5, #22]
.L_0200523e:
	movs	r2, #50
	adds	r2, #255
	movs	r1, #160
	adds	r3, r5, r2
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #131
	strb	r2, [r3, #0]
	lsls	r0, r0, #4
	adds	r3, r5, r1
	strb	r2, [r3, #0]
	adds	r0, #255
	bl 0x0200da14
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #46
	bl 0x0200da1c
	movs	r0, #7
	bl 0x0200dab4
.L_0200526a:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200da14
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #18
	ldr	r2, [pc, #72]
	ldrh	r3, [r3, #0]
	ldr	r5, [pc, #56]
	strh	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #174
	ldrh	r3, [r3, #0]
	movs	r6, #0
	strh	r3, [r2, #2]
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #206
	ldrh	r3, [r3, #0]
	strh	r3, [r2, #4]
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #238
	ldrh	r3, [r3, #0]
	strh	r3, [r2, #6]
	bl 0x0200d0ac
	movs	r0, #30
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	movs	r0, #31
	b.n	.L_020052c8
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xffd60000
	.4byte 0x02000240
	.2byte 0xf948
	.2byte 0x0200
.L_020052c8:
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	movs	r0, #32
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	movs	r0, #33
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	movs	r0, #34
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	movs	r0, #35
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	movs	r0, #133
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200da14
	movs	r0, #29
	bl 0x0200dac4
	adds	r7, r0, #0
	adds	r3, r7, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	movs	r0, #29
	str	r6, [r7, #12]
	bl 0x0200b1f0
	movs	r0, #30
	movs	r1, #3
	bl 0x0200db84
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #253
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_02005364
	movs	r0, #26
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db0c
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db0c
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db0c
	movs	r1, #151
	movs	r2, #172
	movs	r0, #21
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200db0c
.L_02005364:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #107
	bl 0x0200da14
	bl 0x0200dbec
	bl 0x0200dbfc
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #12]
	bl 0x0200d9b4
	bl 0x0200daa4
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xac35
	.2byte 0x0200
	push	{lr}
	movs	r0, #9
	sub	sp, #8
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	movs	r0, #10
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #251
	bl 0x0200da0c
	cmp	r0, #0
	bne.n	.L_020053ba
	bl 0x0200ade0
.L_020053ba:
	bl 0x0200b24c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #105
	bl 0x0200da0c
	cmp	r0, #0
	bne.n	.L_020053d6
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200da14
.L_020053d6:
	ldr	r0, [pc, #256]
	bl 0x0200dc64
	movs	r1, #2
	movs	r0, #8
	bl 0x0200db84
	movs	r0, #11
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	movs	r0, #11
	bl 0x0200dac4
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #234
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_02005438
	movs	r0, #11
	bl 0x0200dac4
	movs	r1, #208
	movs	r3, #148
	lsls	r1, r1, #15
	movs	r2, #0
	lsls	r3, r3, #17
	bl 0x0200da44
	movs	r3, #6
	movs	r2, #18
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #5
	movs	r1, #18
	movs	r2, #1
	movs	r3, #1
	bl 0x0200da6c
	movs	r0, #10
	bl 0x0200d9ac
.L_02005438:
	ldr	r3, [pc, #160]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200dac4
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #106
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_02005462
	bl 0x020085e0
.L_02005462:
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_02005472
	bl 0x02008624
.L_02005472:
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_02005484
	bl 0x02008668
.L_02005484:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_02005496
	bl 0x020086ac
.L_02005496:
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_020054a8
	bl 0x020086f0
.L_020054a8:
	movs	r0, #12
	bl 0x0200db8c
	movs	r0, #13
	bl 0x0200db8c
	movs	r0, #14
	bl 0x0200db8c
	movs	r0, #12
	movs	r1, #1
	bl 0x0200db84
	movs	r0, #13
	movs	r1, #1
	bl 0x0200db84
	movs	r0, #14
	movs	r1, #1
	bl 0x0200db84
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200e324
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x0200d38c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #251
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_02005504
	movs	r0, #1
	movs	r1, #0
	bl 0x0200b640
	movs	r0, #1
	movs	r1, #5
	bl 0x0200b6e0
.L_02005504:
	movs	r0, #1
	bl 0x0200d9ac
	ldr	r3, [pc, #128]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #99
	bne.n	.L_02005528
	movs	r0, #0
	movs	r1, #0
	bl 0x0200b640
	movs	r0, #0
	movs	r1, #0
	b.n	.L_0200554a
.L_02005528:
	cmp	r3, #98
	bne.n	.L_0200553a
	movs	r0, #1
	movs	r1, #0
	bl 0x0200b640
	movs	r0, #1
	movs	r1, #0
	b.n	.L_0200554a
.L_0200553a:
	cmp	r3, #97
	bne.n	.L_0200555e
	movs	r0, #1
	movs	r1, #0
	bl 0x0200b640
	movs	r0, #1
	movs	r1, #2
.L_0200554a:
	bl 0x0200b6e0
	ldr	r3, [pc, #64]
	movs	r1, #0
	ldr	r0, [r3, #0]
	movs	r2, #0
	movs	r3, #0
	bl 0x0200da44
	b.n	.L_02005588
.L_0200555e:
	cmp	r3, #96
	bne.n	.L_02005588
	movs	r0, #1
	movs	r1, #1
	bl 0x0200b640
	movs	r0, #1
	movs	r1, #4
	bl 0x0200b6e0
	ldr	r3, [pc, #28]
	movs	r1, #0
	ldr	r0, [r3, #0]
	movs	r2, #0
	movs	r3, #0
	bl 0x0200da44
	movs	r0, #14
	movs	r1, #4
	bl 0x0200db24
.L_02005588:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xf950
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #105
	bl 0x0200da14
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200da14
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #18
	ldrh	r3, [r3, #0]
	ldr	r2, [pc, #524]
	strh	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #14
	ldrh	r3, [r3, #0]
	strh	r3, [r2, #2]
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #12
	ldrh	r3, [r3, #0]
	strh	r3, [r2, #4]
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #152
	ldrh	r3, [r3, #0]
	strh	r3, [r2, #6]
	bl 0x0200d38c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #251
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_02005600
	movs	r0, #1
	movs	r1, #1
	bl 0x0200b640
	movs	r0, #1
	movs	r1, #4
	bl 0x0200b6e0
.L_02005600:
	movs	r0, #1
	bl 0x0200d9ac
	ldr	r3, [pc, #456]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	subs	r3, #10
	cmp	r3, #6
	bhi.n	.L_02005666
	ldr	r2, [pc, #440]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0200d63c
	.4byte 0x0200d642
	.4byte 0x0200d648
	.4byte 0x0200d64e
	.4byte 0x0200d654
	.4byte 0x0200d65a
	.4byte 0x0200d660
	.4byte 0xfd46f7fe
	.4byte 0xf7fee0c2
	.4byte 0xe0bffe27
	.4byte 0xfeecf7fe
	.4byte 0xf7ffe0bc
	.4byte 0xe0b9f8c9
	.4byte 0xf922f7ff
	.4byte 0xf7ffe0b6
	.4byte 0xe0b3f98d
	.4byte 0xf9e2f7ff
	.2byte 0xe0b0
.L_02005666:
	movs	r0, #17
.L_02005668:
	movs	r1, #1
	bl 0x0200db84
	movs	r0, #7
	movs	r1, #2
	bl 0x0200db84
	movs	r0, #26
	movs	r1, #2
	bl 0x0200db84
.L_0200567e:
	movs	r0, #5
	movs	r1, #2
	bl 0x0200db84
	movs	r0, #6
	movs	r1, #2
	bl 0x0200db84
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #46
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_020056a8
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db0c
	b.n	.L_020056c8
.L_020056a8:
	movs	r0, #131
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_020056c8
	movs	r3, #128
	movs	r1, #152
	movs	r2, #176
	lsls	r3, r3, #6
	movs	r0, #7
	lsls	r1, r1, #17
	lsls	r2, r2, #15
	bl 0x0200db14
.L_020056c8:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #253
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_020056e8
	movs	r0, #1
	movs	r1, #6
	bl 0x0200b6e0
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
.L_020056e4:
	bl 0x0200db0c
.L_020056e8:
	bl 0x0200dbec
	bl 0x0200dbfc
	movs	r1, #144
	ldr	r0, [pc, #228]
	lsls	r1, r1, #3
	bl 0x0200d9b4
	movs	r5, #0
.L_020056fc:
	adds	r0, r5, #0
	adds	r0, #18
	bl 0x0200dac4
	adds	r5, #1
	adds	r0, #92
	movs	r3, #2
	strb	r3, [r0, #0]
	cmp	r5, #7
.L_0200570e:
	bls.n	.L_020056fc
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #241
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_02005732
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #253
	bl 0x0200da0c
	cmp	r0, #0
	bne.n	.L_02005746
	bl 0x0200b2b0
	b.n	.L_02005746
.L_02005732:
	ldr	r3, [pc, #156]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #5
	bne.n	.L_02005746
	bl 0x0200bdec
.L_02005746:
	bl 0x0200daa4
	ldr	r3, [pc, #132]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #99
	bne.n	.L_02005768
	movs	r0, #0
	movs	r1, #0
	bl 0x0200b640
	movs	r0, #0
	movs	r1, #0
	b.n	.L_0200578a
.L_02005768:
	cmp	r3, #98
	bne.n	.L_0200577a
	movs	r0, #1
	movs	r1, #1
	bl 0x0200b640
	movs	r0, #1
	movs	r1, #0
	b.n	.L_0200578a
.L_0200577a:
	cmp	r3, #97
	bne.n	.L_0200579e
	movs	r0, #1
	movs	r1, #1
	bl 0x0200b640
	movs	r0, #1
	movs	r1, #4
.L_0200578a:
	bl 0x0200b6e0
	ldr	r3, [pc, #76]
	movs	r1, #0
	ldr	r0, [r3, #0]
	movs	r2, #0
	movs	r3, #0
	bl 0x0200da44
	b.n	.L_020057c8
.L_0200579e:
	cmp	r3, #96
	bne.n	.L_020057c8
	movs	r0, #1
	movs	r1, #1
	bl 0x0200b640
	movs	r0, #1
	movs	r1, #4
	bl 0x0200b6e0
.L_020057b2:
	ldr	r3, [pc, #40]
	movs	r1, #0
	ldr	r0, [r3, #0]
	movs	r2, #0
	movs	r3, #0
	bl 0x0200da44
	movs	r0, #14
	movs	r1, #5
	bl 0x0200db24
.L_020057c8:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0200f948
	.4byte 0x02000240
	.4byte 0x0200d620
	.4byte 0x0200accd
	.2byte 0xf950
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200da9c
	movs	r0, #0
	bl 0x0200dc3c
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200da14
	ldr	r3, [pc, #136]
	ldr	r2, [pc, #136]
	ldrh	r3, [r3, #0]
	movs	r1, #241
	strh	r3, [r2, #0]
	ldr	r3, [pc, #132]
	lsls	r1, r1, #1
	ldrh	r3, [r3, #0]
	strh	r3, [r2, #2]
	ldr	r3, [pc, #128]
	ldrh	r3, [r3, #0]
	strh	r3, [r2, #4]
	ldr	r3, [pc, #128]
	adds	r2, r3, r1
	movs	r1, #0
	ldrsh	r2, [r2, r1]
	cmp	r2, #10
	bne.n	.L_0200583e
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	ldr	r0, [r5, #0]
	bl 0x0200dac4
	movs	r1, #15
	bl 0x0200db44
	bl 0x0200bc84
	b.n	.L_0200587c
.L_0200583e:
	cmp	r2, #11
	bne.n	.L_02005866
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r5, r3, r1
	ldr	r0, [r5, #0]
	bl 0x0200dac4
	movs	r1, #0
	bl 0x0200da74
	ldr	r0, [r5, #0]
	bl 0x0200dac4
	movs	r1, #15
	bl 0x0200db44
.L_02005860:
	bl 0x0200bc14
	b.n	.L_0200587c
.L_02005866:
	bl 0x0200dbec
	bl 0x0200dbfc
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #32]
	bl 0x0200d9b4
	bl 0x0200daa4
.L_0200587c:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x05000152
	.4byte 0x0200f948
	.4byte 0x0500014e
	.4byte 0x0500014c
	.4byte 0x02000240
	.2byte 0xad65
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200b288
	ldr	r3, [pc, #212]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #9
	bne.n	.L_0200590e
	movs	r0, #254
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_0200590a
.L_020058cc:
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200db0c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #238
	bl 0x0200da0c
.L_020058e0:
	cmp	r0, #0
	beq.n	.L_020058f6
	movs	r3, #176
	movs	r1, #217
	lsls	r3, r3, #8
	movs	r0, #8
	lsls	r1, r1, #17
	ldr	r2, [pc, #152]
.L_020058f0:
	bl 0x0200db14
	b.n	.L_0200590e
.L_020058f6:
	movs	r3, #176
	movs	r1, #182
	movs	r2, #144
	lsls	r3, r3, #8
	movs	r0, #8
.L_02005900:
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200db14
	b.n	.L_0200590e
.L_0200590a:
	bl 0x0200ccf4
.L_0200590e:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200da0c
	cmp	r0, #0
	beq.n	.L_02005930
	movs	r0, #16
	bl 0x0200dac4
	ldr	r2, [pc, #104]
	ldr	r3, [r0, #8]
	adds	r3, r3, r2
	str	r3, [r0, #8]
	ldr	r2, [pc, #100]
	ldr	r3, [r0, #16]
	adds	r3, r3, r2
	str	r3, [r0, #16]
.L_02005930:
	movs	r1, #0
	movs	r0, #16
	bl 0x0200dc74
	movs	r0, #16
	bl 0x0200dac4
	ldr	r2, [r0, #80]
	movs	r3, #0
	strh	r3, [r2, #18]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #24]
	str	r3, [r0, #28]
	ldr	r3, [r0, #8]
	movs	r2, #224
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r0, #8]
	ldr	r3, [r0, #16]
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r0, #16]
	movs	r1, #6
	movs	r0, #16
	bl 0x0200db24
	ldr	r3, [pc, #24]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #14
	bne.n	.L_02005982
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #238
	bl 0x0200da14
.L_02005982:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x027a0000
	.4byte 0xfff20000
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb500
	bl 0x0200dbcc
	pop	{pc}
	.irp EntryTarget, 0x03000528, 0x03000534, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000119, 0x08000121, 0x08000129, 0x08000141, 0x08000151, 0x080001b9, 0x080001c9, 0x080003c1, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020091, 0x080200a9, 0x080200c1, 0x080200c9, 0x080200e9, 0x08020121, 0x08020149, 0x08020151, 0x08020179, 0x080201e9, 0x08020219, 0x08020221, 0x08038249, 0x080ad229, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8049, 0x080c8059, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80b1, 0x080c80b9, 0x080c80c1, 0x080c80d1, 0x080c80d9, 0x080c80f1, 0x080c80f9, 0x080c8101, 0x080c8111, 0x080c8119, 0x080c8129, 0x080c8139, 0x080c8159, 0x080c8171, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81a9, 0x080c81d1, 0x080c81d9, 0x080c8201, 0x080c8209, 0x080c8211, 0x080c8219, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8259, 0x080c8279, 0x080c82e1, 0x080c8379, 0x080c8381, 0x080c8391, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c8481, 0x080c8499, 0x080c84a1, 0x080c84a9, 0x080c84b1, 0x080c84b9, 0x080c84c1, 0x080c84e1, 0x080c8519, 0x080c8601, 0x080c86a9, 0x080c86e9, 0x080c86f9, 0x080c8709, 0x080c87b1, 0x080c8931, 0x08108009, 0x08108011, 0x08108019, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000007
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000018
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00620000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00440000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000800
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00000800
	.4byte 0x0000002e
	.4byte 0x02008039
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffff800
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x0000002e
	.4byte 0x02008039
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000018
	.4byte 0xc0010000
	.4byte 0x00000026
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x01540000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x01640000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x01740000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01740000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00240000
	.4byte 0x00000000
	.4byte 0x01740000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x01740000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x012a0000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01340000
	.4byte 0x00000000
	.4byte 0x00bc0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x012a0000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00de0000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x011c0000
	.4byte 0x00000000
	.4byte 0x00bc0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x02008049
	.4byte 0x0000002e
	.4byte 0x02008055
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xfffe0000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00020000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x0000002e
	.4byte 0x020080c5
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00006666
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00e60000
	.4byte 0x00000000
	.4byte 0x01e60000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01e60000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00006666
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x012a0000
	.4byte 0x00000000
	.4byte 0x01e60000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x01e60000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x02010008
	.4byte 0x0000ffff
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
	.4byte 0x00280190
	.4byte 0x01a00170
	.4byte 0x01800038
	.4byte 0x000effff
	.4byte 0x00280070
	.4byte 0x00800270
	.4byte 0x02800038
	.4byte 0x000fffff
	.4byte 0x00280180
	.4byte 0x01900370
	.4byte 0x03800038
	.4byte 0x0010ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000006e
	.4byte 0x0011b002
	.4byte 0x0020106f
	.4byte 0x00301070
	.4byte 0x00406070
	.4byte 0x00505070
	.4byte 0x00604070
	.4byte 0x00708070
	.4byte 0x0080a070
	.4byte 0x0090206f
	.4byte 0x00a0506f
	.4byte 0x00000071
	.4byte 0x0011b002
	.4byte 0x00201072
	.4byte 0x00301070
	.4byte 0x00406070
	.4byte 0x00505070
	.4byte 0x00604070
	.4byte 0x00708070
	.4byte 0x0080a070
	.4byte 0x00902072
	.4byte 0x00a05072
	.4byte 0x0000006f
	.4byte 0x0010206e
	.4byte 0x0020906e
	.4byte 0x0030107b
	.4byte 0x00403078
	.4byte 0x0050a06e
	.4byte 0x00000072
	.4byte 0x00102071
	.4byte 0x00209071
	.4byte 0x0030107b
	.4byte 0x00403078
	.4byte 0x0050a071
	.4byte 0x00a02079
	.4byte 0x00b03079
	.4byte 0x00c01079
	.4byte 0x00d05078
	.4byte 0x00000070
	.4byte 0x1010306e
	.4byte 0x0000086b
	.4byte 0x10103071
	.4byte 0xffffffff
	.4byte 0x10203070
	.4byte 0xffffffff
	.4byte 0x10302070
	.4byte 0xffffffff
	.4byte 0x1040606e
	.4byte 0x0000086b
	.4byte 0x10406071
	.4byte 0xffffffff
	.4byte 0x1050506e
	.4byte 0x0000086b
	.4byte 0x10505071
	.4byte 0xffffffff
	.4byte 0x1060406e
	.4byte 0x0000086b
	.4byte 0x10604071
	.4byte 0xffffffff
	.4byte 0x1070f070
	.4byte 0xffffffff
	.4byte 0x1080706e
	.4byte 0x0000086b
	.4byte 0x10807071
	.4byte 0xffffffff
	.4byte 0x1090e070
	.4byte 0xffffffff
	.4byte 0x10a0806e
	.4byte 0x0000086b
	.4byte 0x10a08071
	.4byte 0xffffffff
	.4byte 0x10b0c070
	.4byte 0xffffffff
	.4byte 0x10c0b070
	.4byte 0xffffffff
	.4byte 0x10d10070
	.4byte 0xffffffff
	.4byte 0x10e09070
	.4byte 0xffffffff
	.4byte 0x10f07070
	.4byte 0xffffffff
	.4byte 0x1100d070
	.4byte 0xffffffff
	.4byte 0x0000007d
	.4byte 0x00a10072
	.4byte 0x00b0a072
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00024000
	.4byte 0xffff0183
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff0183
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x01024000
	.4byte 0xffff0073
	.4byte 0x00000003
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x00e40000
	.4byte 0x00008000
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x0000d000
	.4byte 0xffff0072
	.4byte 0x00000003
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0xffff0080
	.4byte 0x00000003
	.4byte 0x01200000
	.4byte 0x00000000
	.4byte 0x016c0000
	.4byte 0x00008000
	.4byte 0xffff008b
	.4byte 0x00000001
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x01120000
	.4byte 0x0000b000
	.4byte 0xffff008c
	.4byte 0x00000001
	.4byte 0x003a0000
	.4byte 0x00000000
	.4byte 0x012a0000
	.4byte 0x00013000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x00660000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0001b000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x0001d000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00013000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x011a0000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00015000
	.4byte 0xffff0090
	.4byte 0x00000001
	.4byte 0x01ae0000
	.4byte 0x00000000
	.4byte 0x01ba0000
	.4byte 0x00023000
	.4byte 0xffff0170
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01cc0000
	.4byte 0x00023000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x01024000
	.4byte 0xffff0090
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x0001d000
	.4byte 0xffff008d
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00720000
	.4byte 0x0001d000
	.4byte 0xffff008b
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0001d000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x01240000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x0001b000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x00e60000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x0001d000
	.4byte 0xffff008c
	.4byte 0x00000001
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x00dc0000
	.4byte 0x0001b000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01160000
	.4byte 0x0001d000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x01400000
	.4byte 0x00000000
	.4byte 0x009e0000
	.4byte 0x0001b000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x00ea0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x0001d000
	.4byte 0xffff008b
	.4byte 0x00000001
	.4byte 0x01460000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x0001d000
	.4byte 0xffff008c
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01340000
	.4byte 0x0001d000
	.4byte 0xffff008d
	.4byte 0x00000001
	.4byte 0x01160000
	.4byte 0x00000000
	.4byte 0x00920000
	.4byte 0x0001b000
	.4byte 0xffff008f
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x008a0000
	.4byte 0x0001d000
	.4byte 0xffff0090
	.4byte 0x00000001
	.4byte 0x00c40000
	.4byte 0x00000000
	.4byte 0x008e0000
	.4byte 0x0001d000
	.4byte 0xffff0090
	.4byte 0x00000001
	.4byte 0x00ea0000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x0001d000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00b50000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00c20000
	.4byte 0x01024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00f20000
	.4byte 0x01024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01120000
	.4byte 0x01024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01520000
	.4byte 0x01024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01720000
	.4byte 0x01024000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x019c0000
	.4byte 0x00000000
	.4byte 0x003c0000
	.4byte 0x0002b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff018b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff018b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff018c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff018b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff018b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff018c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01024000
	.4byte 0xffff001a
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0002d000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x00da0000
	.4byte 0x00000000
	.4byte 0x01a40000
	.4byte 0x0003d000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x0003d000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01b60000
	.4byte 0x0003d000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0003d000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01b60000
	.4byte 0x0003b000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x0003b000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x01360000
	.4byte 0x00000000
	.4byte 0x01a60000
	.4byte 0x0003b000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01960000
	.4byte 0x0003b000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00820000
	.4byte 0x00000000
	.4byte 0x016c0000
	.4byte 0x00022000
	.4byte 0xffff0005
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
	.4byte 0xffff001b
	.4byte 0x00000001
	.4byte 0x016e0000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00025000
	.4byte 0xffff001c
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00003000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x027a0000
	.4byte 0x0002d000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x00005000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x007c0000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0000d000
	.4byte 0xffff0088
	.4byte 0x00000003
	.4byte 0x00640000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x0000d000
	.4byte 0xffff0072
	.4byte 0x00000003
	.4byte 0x02c60000
	.4byte 0x00000000
	.4byte 0x00860000
	.4byte 0x00000000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x00003000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00023000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x00015000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x006a0000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00018000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00010000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x005c0000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00003000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00003000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00003000
	.4byte 0xffff0086
	.4byte 0x00000001
	.4byte 0x00540000
	.4byte 0x00000000
	.4byte 0x035a0000
	.4byte 0x00003000
	.4byte 0xffff0087
	.4byte 0x00000003
	.4byte 0x00940000
	.4byte 0x00000000
	.4byte 0x036e0000
	.4byte 0x00005000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01760000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00003000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x01024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x00780000
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
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x0200b949
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x0200b949
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x0200b949
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x0200b949
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x0200b949
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x0200b949
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte 0x0200aad9
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x0200aaf9
	.4byte 0x00001815
	.4byte 0x02010008
	.4byte 0x020085d1
	.4byte 0x00008515
	.4byte 0x09e9000c
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0014
	.4byte 0x0200d995
	.4byte 0x00008602
	.4byte 0xffff0015
	.4byte 0x0200d995
	.4byte 0x10009a15
	.4byte 0xffff000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002001
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002011
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008229
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002012
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002005
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002013
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002006
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002014
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00002007
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002015
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00002008
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002016
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00002009
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002017
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x0000200a
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002018
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x0200824d
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002019
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x0000200e
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x0000201a
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x02008271
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x0000201b
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200b8ed
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x0200b8ed
	.4byte 0x0000c602
	.4byte 0x02090003
	.4byte 0x0200b949
	.4byte 0x0000c602
	.4byte 0x02090004
	.4byte 0x0200b949
	.4byte 0x0000c602
	.4byte 0x02090005
	.4byte 0x0200b949
	.4byte 0x0000c602
	.4byte 0x02090006
	.4byte 0x0200b949
	.4byte 0x0000c602
	.4byte 0x02090007
	.4byte 0x0200b949
	.4byte 0x0000c602
	.4byte 0x02090008
	.4byte 0x0200b949
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte 0x0200b8ed
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x0200b8ed
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte 0x0200aad9
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x0200aaf9
	.4byte 0x00001815
	.4byte 0x02010008
	.4byte 0x020085d1
	.4byte 0x00008515
	.4byte 0x09e9000c
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0014
	.4byte 0x0200d995
	.4byte 0x00000000
	.4byte 0x08fd0014
	.4byte 0x00001e35
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001f9d
	.4byte 0x00008d15
	.4byte 0x08fd0014
	.4byte 0x00001e39
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001fa9
	.4byte 0x00000000
	.4byte 0x08fd000e
	.4byte 0x00001e36
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001f97
	.4byte 0x00008d15
	.4byte 0x08fd000e
	.4byte 0x00001e3a
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001fa3
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x00001e37
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x00001e3b
	.4byte 0x00000000
	.4byte 0x08fd0015
	.4byte 0x00001e3d
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001f9e
	.4byte 0x00008d15
	.4byte 0x08fd0015
	.4byte 0x00001e41
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001faa
	.4byte 0x00000000
	.4byte 0x08fd0011
	.4byte 0x00001e3e
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001f9a
	.4byte 0x00008d15
	.4byte 0x08fd0011
	.4byte 0x00001e42
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001fa6
	.4byte 0x00000000
	.4byte 0x08fd0016
	.4byte 0x00001e40
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001f9f
	.4byte 0x00008d15
	.4byte 0x08fd0016
	.4byte 0x00001e44
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001fab
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00001e46
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x00001e4a
	.4byte 0x00000000
	.4byte 0x08fd000f
	.4byte 0x00001e47
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001f98
	.4byte 0x00008d15
	.4byte 0x08fd000f
	.4byte 0x00001e4b
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001fa4
	.4byte 0x00000000
	.4byte 0x08fd0019
	.4byte 0x00001e48
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00001fa2
	.4byte 0x00008d15
	.4byte 0x08fd0019
	.4byte 0x00001e4c
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00001fae
	.4byte 0x00000000
	.4byte 0x08fd0012
	.4byte 0x00001e4d
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001f9b
	.4byte 0x00008d15
	.4byte 0x08fd0012
	.4byte 0x00001e51
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001fa7
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00001e4e
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001e52
	.4byte 0x00000000
	.4byte 0x08fd0010
	.4byte 0x00001e55
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001f99
	.4byte 0x00008d15
	.4byte 0x08fd0010
	.4byte 0x00001e58
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001fa5
	.4byte 0x00000000
	.4byte 0x08fd0017
	.4byte 0x00001e57
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001fa0
	.4byte 0x00008d15
	.4byte 0x08fd0017
	.4byte 0x00001e5b
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001fac
	.4byte 0x00000000
	.4byte 0x08fd0013
	.4byte 0x00001e5d
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001f9c
	.4byte 0x00008d15
	.4byte 0x08fd0013
	.4byte 0x00001e61
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001fa8
	.4byte 0x00000000
	.4byte 0x08fd0018
	.4byte 0x00001e5f
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001fa1
	.4byte 0x00008d15
	.4byte 0x08fd0018
	.4byte 0x00001e63
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001fad
	.4byte 0x00000000
	.4byte 0xffff0024
	.4byte 0x0200842d
	.4byte 0x00008d15
	.4byte 0xffff0024
	.4byte 0x00001eff
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200b8ed
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x0200b8ed
	.4byte 0x00004602
	.4byte 0x186a0003
	.4byte 0x0200bbf1
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x0200b931
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x0200b8ed
	.4byte 0x00001815
	.4byte 0x02010008
	.4byte 0x020085d1
	.4byte 0x00008c15
	.4byte 0x09ea000b
	.4byte 0x02008735
	.4byte 0x00000009
	.4byte 0x09ea0000
	.4byte 0x02008735
	.4byte 0x50008805
	.4byte 0x086a000a
	.4byte 0x02008619
	.4byte 0x50008805
	.4byte 0x0204000b
	.4byte 0x0200865d
	.4byte 0x50008805
	.4byte 0x0205000c
	.4byte 0x020086a1
	.4byte 0x50008805
	.4byte 0x0206000d
	.4byte 0x020086e5
	.4byte 0x50008805
	.4byte 0x0207000e
	.4byte 0x02008729
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200b8ed
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x0200b8ed
	.4byte 0x00004602
	.4byte 0x186a0003
	.4byte 0x0200bbf1
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x0200b931
	.4byte 0x00000002
	.4byte 0x08f30005
	.4byte 0x0200b8ed
	.4byte 0x00000002
	.4byte 0x092f0005
	.4byte 0x02009219
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x0200b8ed
	.4byte 0x00001815
	.4byte 0x02010008
	.4byte 0x020085d1
	.4byte 0x00008c15
	.4byte 0x09ea000b
	.4byte 0x02008735
	.4byte 0x00000009
	.4byte 0x09ea0000
	.4byte 0x02008735
	.4byte 0x50008805
	.4byte 0x086a000a
	.4byte 0x02008619
	.4byte 0x50008805
	.4byte 0x0204000b
	.4byte 0x0200865d
	.4byte 0x50008805
	.4byte 0x0205000c
	.4byte 0x020086a1
	.4byte 0x50008805
	.4byte 0x0206000d
	.4byte 0x020086e5
	.4byte 0x50008805
	.4byte 0x0207000e
	.4byte 0x02008729
	.4byte 0x00000002
	.4byte 0x08f30014
	.4byte 0x020087c9
	.4byte 0x00000002
	.4byte 0x092e0016
	.4byte 0x02008c91
	.4byte 0x00000000
	.4byte 0xffff0007
	.4byte 0x00001eac
	.4byte 0x00008d15
	.4byte 0xffff0007
	.4byte 0x00001ead
	.4byte 0x00000002
	.4byte 0x08f50015
	.4byte 0x0200a051
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001f87
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001f88
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001f89
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001f8a
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001f8b
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001f8c
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001f8d
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00001f8e
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001f8f
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001f90
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001f91
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001f92
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001f93
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001f94
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001f95
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00001f96
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
	.4byte 0x00000202
	.4byte 0xffff0007
	.4byte 0x0200bac9
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000202
	.4byte 0xffff0009
	.4byte 0x0200bb21
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000202
	.4byte 0xffff000d
	.4byte 0x0200bb89
	.4byte 0x00000000
	.4byte 0x08ee0008
	.4byte 0x00001ff5
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001fff
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001ffd
	.4byte 0x00000000
	.4byte 0x08ee0009
	.4byte 0x00001ff9
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002000
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001ffe
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000201c
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002021
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020082c5
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002022
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002020
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002023
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002024
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002026
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002025
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002027
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x020084dd
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x02008501
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x020084dd
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x02008501
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002029
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000202b
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0000202c
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x0000202e
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x0000202d
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x0000202f
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x02008319
	.4byte 0x00000003
	.4byte 0xffff0014
	.4byte 0x02008319
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002031
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008351
	.4byte 0x00000003
	.4byte 0xffff0015
	.4byte 0x02008351
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002033
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x02008389
	.4byte 0x00000003
	.4byte 0xffff0016
	.4byte 0x02008389
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002035
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x020083c1
	.4byte 0x00000003
	.4byte 0xffff0017
	.4byte 0x020083c1
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002038
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00002037
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00002039
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x020083f9
	.4byte 0x00000003
	.4byte 0xffff0018
	.4byte 0x020083f9
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x0000203b
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
