.syntax unified
.include "games/THE BROKEN SEAL/SRC/COMMON/OVERLAY.INC"
	.thumb
	.set sub_02000736, 0x02000736
	.set sub_02000740, 0x02000740
	.set sub_0200076c, 0x0200076c
	.set sub_02000784, 0x02000784
	.set sub_0200078e, 0x0200078e
	.set sub_020007ac, 0x020007ac
	.set sub_020007c8, 0x020007c8
	.set sub_020007dc, 0x020007dc
	.set sub_02000838, 0x02000838
	.set sub_02000860, 0x02000860
	.set sub_0200086a, 0x0200086a
	.set sub_02000882, 0x02000882
	.set sub_02000884, 0x02000884
	.set sub_02000892, 0x02000892
	.set sub_02000928, 0x02000928
	.set sub_02000940, 0x02000940
	.set sub_0200094e, 0x0200094e
	.set sub_0200096e, 0x0200096e
	.set sub_02000984, 0x02000984
	.set sub_02000994, 0x02000994
	.set sub_02000a5a, 0x02000a5a
	.set sub_02000aa6, 0x02000aa6
	.set sub_02000aac, 0x02000aac
	.set sub_02000aba, 0x02000aba
	.set sub_02000ac8, 0x02000ac8
	.set sub_02000ae8, 0x02000ae8
	.set sub_02000aec, 0x02000aec
	.set sub_02000b04, 0x02000b04
	.set sub_02000b5e, 0x02000b5e
	.set sub_02000b64, 0x02000b64
	.set sub_02000b66, 0x02000b66
	.set sub_02000b6e, 0x02000b6e
	.set sub_02000b88, 0x02000b88
	.set sub_02000b96, 0x02000b96
	.set sub_02000b98, 0x02000b98
	.set sub_02000ba2, 0x02000ba2
	.set sub_02000bb0, 0x02000bb0
	.set sub_02000bce, 0x02000bce
	.set sub_02000bdc, 0x02000bdc
	.set sub_02000bf0, 0x02000bf0
	.set sub_02000c2c, 0x02000c2c
	.set sub_02000c86, 0x02000c86
	.set sub_02000c8c, 0x02000c8c
	.set sub_02000c96, 0x02000c96
	.set sub_02000c9c, 0x02000c9c
	.set sub_02000caa, 0x02000caa
	.set sub_02000cba, 0x02000cba
	.set sub_02000ce8, 0x02000ce8
	.set sub_02000cfc, 0x02000cfc
	.set sub_02000d56, 0x02000d56
	.set sub_02000d64, 0x02000d64
	.set sub_02000d86, 0x02000d86
	.set sub_02000dca, 0x02000dca
	.set sub_02000dd2, 0x02000dd2
	.global Overlay_02000000
Overlay_02000000:
.include "games/THE BROKEN SEAL/SRC/FIELD/GOMA_SUIRO/ENTRY.INC"
AlchemyC_02000030:
	.space 0x16
	.2byte 0x0000
AlchemyC_02000048:
	.space 0x56
	.2byte 0x0000
AlchemyC_020000a0:
	.space 0x62
	.2byte 0x0000
AlchemyC_02000104:
	.space 0x38
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	adds	r6, r1, #0
	ldr	r1, [sp, #48]
	adds	r5, r0, #0
	movs	r0, #0
	mov	r8, r2
	str	r3, [sp, #4]
	mov	sl, r1
	ldr	r7, [sp, #52]
	bl	sub_02000882
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r2, sl
	ands	r3, r2
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_0200017a
	cmp	r7, #0
	beq.n	.L_0200017a
	movs	r3, #24
	ldrsh	r0, [r7, r3]
	adds	r2, r6, #0
	b.n	.L_0200017e
.L_0200017a:
	adds	r2, r6, #0
	movs	r0, #222
.L_0200017e:
	adds	r1, r5, #0
	mov	r3, r8
	bl	sub_02000860
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_0200018e
	b.n	.L_020002f6
.L_0200018e:
	ldr	r1, [r6, #80]
	mov	r8, r1
	mov	r1, sl
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	bl	sub_0200086a
	mov	r3, sl
	ldr	r2, [pc, #356]
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	fp, r3
	bl	sub_02000884
	adds	r3, r6, #0
	movs	r0, #0
	adds	r3, #85
	strb	r0, [r3, #0]
	mov	r3, r8
	adds	r3, #38
	strb	r0, [r3, #0]
	ldr	r3, [pc, #328]
	str	r3, [r6, #108]
	ldr	r3, [sp, #4]
	str	r3, [r6, #68]
	ldr	r3, [sp, #40]
	str	r3, [r6, #72]
	ldr	r3, [sp, #44]
	mov	r1, r9
	str	r3, [r6, #76]
	ldr	r3, [r1, #80]
	ldrb	r3, [r3, #9]
	movs	r2, #12
	ands	r2, r3
	mov	r3, r8
	ldrb	r1, [r3, #9]
	movs	r3, #13
	negs	r3, r3
	mov	r9, r3
	ands	r3, r1
	orrs	r3, r2
	adds	r2, r6, #0
	mov	r1, r8
	adds	r2, #100
	strb	r3, [r1, #9]
	adds	r3, r2, #0
	str	r0, [r6, #48]
	str	r0, [r6, #52]
	str	r2, [sp, #0]
	strh	r0, [r3, #0]
	ldr	r3, [pc, #276]
	mov	r1, sl
	ands	r3, r1
	movs	r5, #3
	cmp	r3, #0
	beq.n	.L_020002f6
	cmp	r7, #0
	beq.n	.L_020002f6
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200021c
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl	sub_0200096e
.L_0200021c:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000248
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	mov	r3, r8
	ldrb	r2, [r7, #0]
	ldrb	r1, [r3, #9]
	ands	r2, r5
	mov	r3, r9
	ands	r3, r1
	lsls	r2, r2, #2
	orrs	r3, r2
	mov	r1, r8
	strb	r3, [r1, #9]
.L_02000248:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, sl
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0200025c
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_0200025c:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020002a6
	ldr	r3, [pc, #156]
	mov	r1, fp
	ldr	r5, [r3, r1]
	cmp	r2, #0
	beq.n	.L_0200028a
	ldr	r0, [r7, #16]
	ldr	r3, [r6, #24]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	bl	sub_02000928
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_020002a0
.L_0200028a:
	ldr	r0, [r7, #16]
	ldr	r2, [pc, #128]
	ldr	r1, [r5, #12]
	adds	r0, r0, r2
	bl	sub_02000940
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_020002a0:
	bl	sub_0200094e
	str	r0, [r6, #52]
.L_020002a6:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020002c2
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_02000984
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl	sub_02000994
.L_020002c2:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020002d4
	ldrh	r3, [r7, #32]
	mov	r1, r8
	strh	r3, [r1, #30]
.L_020002d4:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020002e6
	ldrh	r3, [r7, #34]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #0]
.L_020002e6:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020002f6
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_020002f6:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0200880c
	.4byte 0x02008105
	.2byte 0x0000
	.2byte 0xffff
AlchemyC_02000314:
	.space 0x30
AlchemyC_02000344:
	.space 0x4
AlchemyC_02000348:
	.space 0x8
AlchemyC_02000350:
	.space 0x30
AlchemyC_02000380:
	.space 0x64
AlchemyC_020003e4:
	.space 0x30
AlchemyC_02000414:
	.space 0xb0
AlchemyC_020004c4:
	.space 0x10
AlchemyC_020004d4:
	.space 0x20
AlchemyC_020004f4:
	.space 0x1b8
.include "games/THE BROKEN SEAL/SRC/FIELD/GOMA_SUIRO/IMPORT.INC"
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
	.4byte 0x02008764
	.4byte 0x0200879c
	.4byte 0x020087d4
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000178
	.4byte 0xc0000308
	.4byte 0x00580000
	.4byte 0x02980008
	.4byte 0x00000340
	.4byte 0xffff0002
	.4byte 0x000000a8
	.4byte 0x40000228
	.4byte 0x00580000
	.4byte 0x02980008
	.4byte 0x00000340
	.4byte 0xffff0003
	.4byte 0x00000258
	.4byte 0x40000208
	.4byte 0x00580000
	.4byte 0x02980008
	.4byte 0x00000340
	.4byte 0xffff0004
	.4byte 0x00000178
	.4byte 0x400000a8
	.4byte 0x00580000
	.4byte 0x02980008
	.4byte 0x00000340
	.4byte 0xffff0005
	.4byte 0x00000266
	.4byte 0xc0000350
	.4byte 0x01e00000
	.4byte 0x02d0025d
	.4byte 0x00000384
	.4byte 0xffff0063
	.4byte 0x000001a8
	.4byte 0x80000118
	.4byte 0x00580000
	.4byte 0x02980008
	.4byte 0x00000340
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x0000009a
	.4byte 0xc000015e
	.4byte 0x00100000
	.4byte 0x02780030
	.4byte 0x00000168
	.4byte 0xffff0002
	.4byte 0x00000228
	.4byte 0xc0000138
	.4byte 0x00100000
	.4byte 0x02780030
	.4byte 0x00000168
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00102019
	.4byte 0x0020101b
	.4byte 0x0030201b
	.4byte 0x0040101d
	.4byte 0x0050b005
	.4byte 0x0000001d
	.4byte 0x0010401c
	.4byte 0x00228002
	.4byte 0x000001ff
	.4byte 0xffff00d4
	.4byte 0x00000001
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00024000
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
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000004
	.4byte 0x00000013
	.4byte 0x0f280064
	.4byte 0x001000bf
	.4byte 0x00008c15
	.4byte 0x08640008
	.4byte 0x02008381
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
