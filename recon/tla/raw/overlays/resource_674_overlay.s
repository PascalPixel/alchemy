.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x0200ab65, 0x02008039, 0x02008045, 0x0200804d, 0x0200819d, 0x02008041, 0x0200b40d
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xc6cc
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xc6fc
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #88]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bne.n	.L_02000064
	ldr	r0, [pc, #76]
	b.n	.L_020000a4
.L_02000064:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_0200006e
	ldr	r0, [pc, #76]
	b.n	.L_020000a4
.L_0200006e:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_02000078
	ldr	r0, [pc, #72]
	b.n	.L_020000a4
.L_02000078:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_02000082
	ldr	r0, [pc, #72]
	b.n	.L_020000a4
.L_02000082:
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_020000a2
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #165
	bl 0x0200bdcc
	cmp	r0, #0
	beq.n	.L_0200009e
	ldr	r3, [pc, #56]
	movs	r2, #1
	adds	r3, #46
	strb	r2, [r3, #0]
.L_0200009e:
	ldr	r0, [pc, #48]
	b.n	.L_020000a4
.L_020000a2:
	ldr	r0, [pc, #48]
.L_020000a4:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000083
	.4byte 0x0200c76c
	.4byte 0x00000084
	.4byte 0x0200c7e4
	.4byte 0x00000085
	.4byte 0x0200c94c
	.4byte 0x00000086
	.4byte 0x0200c9c4
	.4byte 0x00000087
	.4byte 0x0200ca84
	.2byte 0xc754
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #10
	sub	sp, #32
	bl 0x0200be84
	add	r5, sp, #8
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200b7d8
	cmp	r0, #0
	beq.n	.L_02000192
	mov	r2, sp
	add	r3, sp, #24
	ldmia	r3!, {r0, r1}
	stmia	r2!, {r0, r1}
	ldr	r2, [r5, #8]
	ldr	r1, [r5, #4]
	ldr	r3, [r5, #12]
	ldr	r0, [r5, #0]
	bl 0x0200ba5c
	movs	r0, #5
	bl 0x0200bd9c
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r6, #72]
	movs	r0, #10
	movs	r1, #3
	bl 0x0200bef4
	movs	r2, #8
	movs	r1, #0
	movs	r0, #10
	bl 0x0200becc
	movs	r0, #22
	bl 0x0200be5c
	movs	r0, #240
	bl 0x0200bffc
	movs	r0, #10
	movs	r1, #8
	bl 0x0200bef4
	adds	r0, r6, #0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	adds	r3, r6, #0
	movs	r1, #0
	adds	r3, #85
	strb	r1, [r3, #0]
	ldr	r3, [pc, #76]
	str	r1, [r6, #40]
	str	r3, [r6, #12]
	str	r3, [r6, #20]
	movs	r3, #10
	movs	r5, #9
	str	r3, [sp, #4]
	movs	r0, #9
	movs	r1, #1
	movs	r2, #4
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200be24
	movs	r0, #9
	movs	r1, #8
	movs	r2, #4
	movs	r3, #1
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200be24
	movs	r3, #74
	str	r3, [sp, #4]
	movs	r0, #9
	movs	r1, #64
	movs	r2, #4
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200be24
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #44
	bl 0x0200bdd4
.L_02000192:
	add	sp, #32
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb500
	ldr	r3, [pc, #64]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_020001b4
	ldr	r0, [pc, #52]
	b.n	.L_020001de
.L_020001b4:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_020001be
	ldr	r0, [pc, #52]
	b.n	.L_020001de
.L_020001be:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_020001c8
	ldr	r0, [pc, #48]
	b.n	.L_020001de
.L_020001c8:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_020001d2
	ldr	r0, [pc, #48]
	b.n	.L_020001de
.L_020001d2:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_020001dc
	ldr	r0, [pc, #44]
	b.n	.L_020001de
.L_020001dc:
	ldr	r0, [pc, #44]
.L_020001de:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000083
	.4byte 0x0200cb20
	.4byte 0x00000084
	.4byte 0x0200cbb0
	.4byte 0x00000085
	.4byte 0x0200cca0
	.4byte 0x00000086
	.4byte 0x0200cd84
	.4byte 0x00000087
	.4byte 0x0200ce5c
	.2byte 0xcb14
	.2byte 0x0200
	push	{lr}
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #162
	bl 0x0200bdcc
	cmp	r0, #0
	beq.n	.L_02000260
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200bdcc
	cmp	r0, #0
	beq.n	.L_02000250
	ldr	r3, [pc, #100]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r0, #9
	ldr	r1, [r3, #0]
	movs	r2, #0
	bl 0x0200bf14
	ldr	r0, [pc, #84]
	bl 0x0200bf24
	b.n	.L_02000256
.L_02000250:
	ldr	r0, [pc, #80]
	bl 0x0200bf24
.L_02000256:
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	b.n	.L_02000294
.L_02000260:
	movs	r0, #9
	bl 0x0200be9c
	movs	r0, #1
	bl 0x0200bd9c
	ldr	r3, [pc, #44]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #9
	bl 0x0200bf14
	ldr	r0, [pc, #40]
	bl 0x0200bf24
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	movs	r0, #9
	movs	r1, #19
	bl 0x0200bfcc
.L_02000294:
	bl 0x0200be6c
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000021bb
	.4byte 0x000021b7
	.2byte 0x21b1
	.2byte 0x0000
	push	{lr}
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #162
	bl 0x0200bdcc
	cmp	r0, #0
	beq.n	.L_020002ea
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200bdcc
	cmp	r0, #0
	beq.n	.L_020002da
	ldr	r0, [pc, #44]
	bl 0x0200bf24
	b.n	.L_020002e0
.L_020002da:
	ldr	r0, [pc, #40]
	bl 0x0200bf24
.L_020002e0:
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	b.n	.L_020002f8
.L_020002ea:
	ldr	r0, [pc, #28]
	bl 0x0200bf24
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
.L_020002f8:
	bl 0x0200be6c
	pop	{pc}
	.2byte 0x0000
	.4byte 0x000021bd
	.4byte 0x000021b9
	.2byte 0x21b3
	.2byte 0x0000
	push	{lr}
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #162
	bl 0x0200bdcc
	cmp	r0, #0
	beq.n	.L_0200035c
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200bdcc
	cmp	r0, #0
	beq.n	.L_0200034c
	ldr	r3, [pc, #60]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r0, #19
	ldr	r1, [r3, #0]
	movs	r2, #0
	bl 0x0200bf14
	ldr	r0, [pc, #44]
	bl 0x0200bf24
	b.n	.L_02000352
.L_0200034c:
	ldr	r0, [pc, #40]
	bl 0x0200bf24
.L_02000352:
	movs	r0, #19
	movs	r1, #0
	bl 0x0200bf2c
	b.n	.L_0200036a
.L_0200035c:
	ldr	r0, [pc, #28]
	bl 0x0200bf24
	movs	r0, #19
	movs	r1, #0
	bl 0x0200bf2c
.L_0200036a:
	bl 0x0200be6c
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000021bc
	.4byte 0x000021b7
	.2byte 0x21b2
	.2byte 0x0000
	push	{lr}
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #162
	bl 0x0200bdcc
	cmp	r0, #0
	beq.n	.L_020003be
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200bdcc
	cmp	r0, #0
	beq.n	.L_020003ae
	ldr	r0, [pc, #40]
	bl 0x0200bf24
	b.n	.L_020003b4
.L_020003ae:
	ldr	r0, [pc, #36]
	bl 0x0200bf24
.L_020003b4:
	movs	r0, #19
	movs	r1, #0
	bl 0x0200bf2c
	b.n	.L_020003cc
.L_020003be:
	ldr	r0, [pc, #24]
	bl 0x0200bf24
	movs	r0, #19
	movs	r1, #0
	bl 0x0200bf2c
.L_020003cc:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x000021be
	.4byte 0x000021ba
	.2byte 0x21b4
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200be84
	movs	r2, #190
	ldrh	r3, [r0, #6]
	lsls	r2, r2, #7
	adds	r2, #255
	adds	r3, r3, r2
	ldr	r2, [pc, #16]
	lsls	r3, r3, #16
	movs	r0, #1
	cmp	r3, r2
	bls.n	.L_02000402
	movs	r0, #0
.L_02000402:
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0x3ffe
	push	{lr}
	bl 0x020083dc
	cmp	r0, #0
	beq.n	.L_0200041e
	movs	r0, #21
	bl 0x0200bff4
	b.n	.L_0200043a
.L_0200041e:
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	ldr	r0, [pc, #16]
	bl 0x0200bf24
	movs	r0, #21
	movs	r1, #0
	bl 0x0200bf2c
	bl 0x0200be6c
.L_0200043a:
	pop	{pc}
	.2byte 0x21f7
	.2byte 0x0000
	push	{lr}
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	movs	r0, #133
	lsls	r0, r0, #2
	bl 0x0200bdcc
	cmp	r0, #0
	bne.n	.L_020004b6
	ldr	r0, [pc, #140]
	bl 0x0200bf24
	movs	r0, #8
	movs	r1, #0
	bl 0x0200bf2c
	movs	r1, #129
	movs	r0, #9
	lsls	r1, r1, #1
	bl 0x0200bf5c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200bf34
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	movs	r0, #8
	movs	r1, #0
	bl 0x0200bf3c
	movs	r2, #10
	movs	r0, #9
	movs	r1, #4
	bl 0x0200bf04
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	movs	r1, #129
	movs	r0, #9
	lsls	r1, r1, #1
	bl 0x0200bf5c
	movs	r0, #20
	bl 0x0200be5c
	movs	r0, #133
	lsls	r0, r0, #2
	bl 0x0200bdd4
.L_020004b6:
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #8
	bl 0x0200bf14
	ldr	r0, [pc, #36]
	bl 0x0200bf24
	movs	r0, #8
	movs	r1, #0
	bl 0x0200bf2c
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200bf34
	bl 0x0200be6c
	pop	{pc}
	.4byte 0x000021a4
	.4byte 0x02000240
	.2byte 0x21a7
	.2byte 0x0000
	push	{lr}
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	ldr	r0, [pc, #24]
	bl 0x0200bf24
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	bl 0x0200be6c
	pop	{pc}
	.2byte 0x21a9
	.2byte 0x0000
	push	{lr}
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #164
	bl 0x0200bdcc
	cmp	r0, #0
	beq.n	.L_0200054a
	ldr	r3, [pc, #36]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r0, #10
	bl 0x0200bfcc
.L_0200054a:
	ldr	r0, [pc, #24]
	bl 0x0200bf24
	movs	r0, #10
	movs	r1, #0
	bl 0x0200bf2c
	bl 0x0200be6c
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x21bf
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	movs	r0, #226
	lsls	r0, r0, #1
	bl 0x0200be4c
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_020005be
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #164
	bl 0x0200bdcc
	cmp	r0, #0
	beq.n	.L_020005ae
	ldr	r3, [pc, #332]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r0, #10
	ldr	r1, [r3, #0]
	bl 0x0200bfcc
	ldr	r0, [pc, #320]
	bl 0x0200bf24
	b.n	.L_020005b4
.L_020005ae:
	ldr	r0, [pc, #316]
	bl 0x0200bf24
.L_020005b4:
	movs	r0, #10
	movs	r1, #0
	bl 0x0200bf2c
	b.n	.L_020006da
.L_020005be:
	ldr	r3, [pc, #292]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r7, r3, r2
	ldr	r0, [r7, #0]
	bl 0x0200be84
	adds	r6, r0, #0
	movs	r0, #10
	bl 0x0200be84
	ldr	r1, [r7, #0]
	adds	r5, r0, #0
	movs	r2, #0
	movs	r0, #10
	bl 0x0200bf14
	movs	r2, #10
	movs	r1, #4
	movs	r0, #10
	bl 0x0200bf04
	ldr	r3, [pc, #260]
	mov	r8, r3
	mov	r0, r8
	bl 0x0200bf24
	movs	r0, #10
	movs	r1, #0
	bl 0x0200bf2c
	movs	r0, #10
	movs	r1, #4
	bl 0x0200bef4
	ldr	r3, [r5, #8]
	ldr	r1, [r6, #8]
	ldr	r2, [r5, #16]
	adds	r1, r1, r3
	lsrs	r3, r1, #31
	adds	r1, r1, r3
	ldr	r3, [r6, #16]
	asrs	r1, r1, #1
	adds	r3, r3, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	ldr	r2, [r6, #12]
	adds	r0, r5, #0
	bl 0x0200be04
	movs	r0, #20
	bl 0x0200bd9c
	movs	r1, #240
	movs	r2, #128
	movs	r3, #156
	lsls	r2, r2, #16
	lsls	r3, r3, #17
	adds	r0, r5, #0
	lsls	r1, r1, #15
	bl 0x0200be04
	movs	r1, #5
	movs	r0, #10
	bl 0x0200bef4
	movs	r0, #10
	bl 0x0200bd9c
	movs	r1, #128
	ldr	r0, [r7, #0]
	lsls	r1, r1, #7
	bl 0x0200bf3c
	movs	r0, #234
	movs	r2, #152
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	adds	r0, #255
	lsls	r2, r2, #16
	bl 0x0200bdf4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0200068c
	ldr	r0, [r7, #0]
	movs	r1, #28
	bl 0x0200bef4
	movs	r1, #198
	adds	r0, r5, #0
	adds	r1, #255
	bl 0x0200be3c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200be34
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
.L_0200068c:
	movs	r0, #10
	bl 0x0200be5c
	bl 0x0200bfc4
	mov	r0, r8
	movs	r1, #1
	adds	r0, #1
	bl 0x0200be44
	movs	r0, #226
	lsls	r0, r0, #1
	bl 0x0200be54
	movs	r0, #198
	movs	r1, #0
	adds	r0, #255
	bl 0x0200be74
	bl 0x0200bfbc
	cmp	r5, #0
	beq.n	.L_020006c8
	adds	r0, r5, #0
	bl 0x0200bdfc
	ldr	r0, [r7, #0]
	movs	r1, #1
	bl 0x0200bef4
.L_020006c8:
	movs	r0, #10
	movs	r1, #0
	bl 0x0200bf3c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #164
	bl 0x0200bdd4
.L_020006da:
	bl 0x0200be6c
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x000021c4
	.4byte 0x000021c0
	.2byte 0x21c1
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	movs	r0, #198
	adds	r0, #255
	bl 0x0200be4c
	movs	r5, #1
	negs	r5, r5
	cmp	r0, r5
	bne.n	.L_0200071a
	b.n	.L_02000d5c
.L_0200071a:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bdcc
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_0200072c
	b.n	.L_02000978
.L_0200072c:
	adds	r2, r5, #0
	movs	r3, #0
	adds	r0, r5, #0
	adds	r1, r5, #0
	bl 0x0200bf6c
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200bf64
	movs	r0, #140
	movs	r2, #190
	lsls	r0, r0, #16
	adds	r1, r5, #0
	lsls	r2, r2, #16
	movs	r3, #1
	bl 0x0200bf6c
	ldr	r3, [pc, #528]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r7, r3, r0
	ldr	r1, [r7, #0]
	movs	r0, #10
	movs	r2, #0
	bl 0x0200bf14
	ldr	r2, [pc, #516]
	mov	sl, r2
	mov	r0, sl
	bl 0x0200bf24
	movs	r0, #10
	movs	r1, #0
	bl 0x0200bf2c
	bl 0x0200bf74
	bl 0x0200bfc4
	ldr	r0, [r7, #0]
	movs	r1, #0
	bl 0x0200be7c
	mov	r9, r0
	cmp	r0, #0
	beq.n	.L_02000794
	b.n	.L_02000944
.L_02000794:
	ldr	r0, [r7, #0]
	bl 0x0200be84
	ldr	r1, [r7, #0]
	mov	r8, r0
	movs	r0, #10
	bl 0x0200bfcc
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r7, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200be8c
	movs	r2, #168
	ldr	r0, [r7, #0]
	movs	r1, #98
	bl 0x0200bec4
	movs	r1, #192
	ldr	r0, [r7, #0]
	lsls	r1, r1, #8
	bl 0x0200bf3c
	ldr	r0, [r7, #0]
	movs	r1, #4
	movs	r2, #10
	bl 0x0200bf04
	movs	r0, #234
	movs	r1, #208
	movs	r2, #192
	movs	r3, #160
	adds	r0, #255
	lsls	r1, r1, #15
	lsls	r2, r2, #13
	lsls	r3, r3, #16
	bl 0x0200bdf4
	adds	r5, r0, #0
	movs	r6, #0
.L_020007ec:
	cmp	r5, #0
	beq.n	.L_0200080e
	movs	r1, #198
	adds	r1, #255
	bl 0x0200be3c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200be34
	adds	r3, r5, #0
	adds	r3, #85
	adds	r2, r5, #0
	strb	r6, [r3, #0]
	adds	r2, #92
	movs	r3, #1
	strb	r3, [r2, #0]
.L_0200080e:
	movs	r0, #10
	bl 0x0200be5c
	bl 0x0200bfbc
	movs	r0, #10
	bl 0x0200be9c
	ldr	r0, [r7, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200bf34
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200bf54
	mov	r0, sl
	adds	r0, #1
	bl 0x0200bf24
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #10
	movs	r1, #0
	bl 0x0200bf2c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #10
	adds	r1, #204
	adds	r2, #102
	bl 0x0200be8c
	movs	r2, #164
	movs	r0, #10
	movs	r1, #128
	bl 0x0200bec4
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #10
	bl 0x0200bf3c
	movs	r0, #20
	bl 0x0200be5c
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r1, [pc, #248]
	adds	r2, #204
	movs	r0, #10
	bl 0x0200be8c
	movs	r0, #10
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	mov	sl, r3
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #108
	movs	r2, #168
	movs	r0, #10
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	movs	r0, #10
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r5, #1
	orrs	r3, r5
	strb	r3, [r0, #0]
	mov	r0, r8
	ldr	r2, [r0, #12]
	movs	r3, #192
	lsls	r3, r3, #13
	ldr	r1, [r0, #8]
	adds	r2, r2, r3
	ldr	r3, [r0, #16]
	movs	r0, #234
	adds	r0, #255
	bl 0x0200bdf4
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_020008f0
	ldr	r0, [r7, #0]
	movs	r1, #28
	bl 0x0200bef4
	adds	r3, r6, #0
	adds	r3, #85
	mov	r0, r9
	movs	r1, #227
	strb	r0, [r3, #0]
	lsls	r1, r1, #1
	adds	r0, r6, #0
	bl 0x0200be3c
	movs	r0, #20
	bl 0x0200be5c
.L_020008f0:
	movs	r0, #10
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	mov	r3, sl
	ands	r3, r2
	movs	r1, #116
	strb	r3, [r0, #0]
	movs	r2, #164
	movs	r0, #10
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	movs	r0, #10
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	bl 0x0200bfc4
	movs	r0, #198
	adds	r0, #255
	bl 0x0200be54
	movs	r0, #227
	lsls	r0, r0, #1
	movs	r1, #0
	bl 0x0200be74
	cmp	r6, #0
	bne.n	.L_0200093a
	b.n	.L_02000cf4
.L_0200093a:
	adds	r0, r6, #0
	bl 0x0200bdfc
	ldr	r0, [r7, #0]
	b.n	.L_02000cee
.L_02000944:
	bl 0x0200bfbc
	movs	r1, #10
	adds	r1, #255
	movs	r2, #20
	movs	r0, #10
	bl 0x0200bf54
	mov	r0, sl
	adds	r0, #4
	bl 0x0200bf24
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #10
	movs	r1, #0
	bl 0x0200bf2c
	b.n	.L_02000d80
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000021ca
	.2byte 0x9999
	.2byte 0x0001
.L_02000978:
	ldr	r2, [pc, #788]
	mov	r9, r2
	mov	r0, r9
	bl 0x0200bf24
	movs	r0, #10
	movs	r1, #0
	bl 0x0200bf2c
	adds	r2, r5, #0
	adds	r0, r5, #0
	adds	r1, r5, #0
	movs	r3, #0
	bl 0x0200bf6c
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200bf64
	movs	r0, #140
	movs	r2, #190
	adds	r1, r5, #0
	movs	r3, #1
	lsls	r0, r0, #16
	lsls	r2, r2, #16
	bl 0x0200bf6c
	movs	r1, #160
	movs	r0, #10
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200bf34
	ldr	r3, [pc, #720]
	movs	r0, #133
	lsls	r0, r0, #2
	movs	r1, #204
	movs	r2, #204
	adds	r6, r3, r0
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r6, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200be8c
	movs	r2, #190
	ldr	r0, [r6, #0]
	movs	r1, #120
	bl 0x0200bec4
	movs	r1, #0
	ldr	r0, [r6, #0]
	bl 0x0200bf3c
	ldr	r0, [r6, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	mov	sl, r3
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #132
	movs	r2, #190
	ldr	r0, [r6, #0]
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	ldr	r0, [r6, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #1
	mov	r8, r2
	mov	r2, r8
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #140
	movs	r0, #234
	movs	r3, #190
	adds	r0, #255
	lsls	r1, r1, #16
	movs	r2, #0
	lsls	r3, r3, #16
	bl 0x0200bdf4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000a5a
	movs	r1, #198
	adds	r1, #255
	bl 0x0200be3c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200be34
	adds	r3, r5, #0
	adds	r3, #85
	adds	r2, r5, #0
	strb	r7, [r3, #0]
	adds	r2, #92
	movs	r3, #1
	strb	r3, [r2, #0]
.L_02000a5a:
	ldr	r0, [r6, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	mov	r3, sl
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #120
	movs	r2, #190
	ldr	r0, [r6, #0]
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	ldr	r0, [r6, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, r8
	orrs	r3, r2
	movs	r1, #192
	strb	r3, [r0, #0]
	lsls	r1, r1, #6
	movs	r0, #10
	movs	r2, #0
	bl 0x0200bf34
	movs	r2, #10
	movs	r0, #10
	movs	r1, #4
	bl 0x0200bf04
	movs	r0, #10
	movs	r1, #0
	bl 0x0200bf2c
	ldr	r1, [r6, #0]
	movs	r0, #10
	bl 0x0200bfcc
	ldr	r0, [r6, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	mov	r3, sl
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #132
	movs	r2, #190
	ldr	r0, [r6, #0]
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	ldr	r0, [r6, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, r8
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #195
	movs	r3, #230
	lsls	r1, r1, #16
	movs	r2, #0
	lsls	r3, r3, #15
	adds	r0, r5, #0
	bl 0x0200be04
	ldr	r0, [r6, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	mov	r3, sl
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #120
	movs	r2, #190
	ldr	r0, [r6, #0]
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	ldr	r0, [r6, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, r8
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #20
	bl 0x0200be5c
	ldr	r0, [r6, #0]
	movs	r1, #98
	movs	r2, #168
	bl 0x0200bec4
	ldr	r0, [r6, #0]
	movs	r1, #4
	movs	r2, #10
	bl 0x0200bf04
	movs	r1, #208
	movs	r2, #192
	movs	r3, #160
	lsls	r2, r2, #13
	lsls	r3, r3, #16
	lsls	r1, r1, #15
	adds	r0, r5, #0
	bl 0x0200be04
	movs	r0, #10
	bl 0x0200be5c
	bl 0x0200bfc4
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
	bl 0x0200bf3c
	mov	r0, r9
	adds	r0, #2
	movs	r1, #1
	bl 0x0200be44
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	bl 0x0200bfbc
	movs	r2, #10
	movs	r1, #6
	movs	r0, #10
	bl 0x0200bf04
	mov	r0, r9
	adds	r0, #3
	bl 0x0200bf24
	movs	r1, #0
	movs	r0, #10
	bl 0x0200bf2c
	movs	r0, #10
	bl 0x0200be9c
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x0200bf3c
	bl 0x0200bfc4
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x0200be7c
	cmp	r0, #0
	beq.n	.L_02000bba
	b.n	.L_02000d04
.L_02000bba:
	ldr	r0, [r6, #0]
	bl 0x0200be84
	adds	r5, r0, #0
	bl 0x0200bfbc
	movs	r1, #132
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200bf54
	mov	r0, r9
	adds	r0, #4
	bl 0x0200bf24
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #10
	movs	r1, #0
	bl 0x0200bf2c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #10
	adds	r1, #204
	adds	r2, #102
	bl 0x0200be8c
	movs	r2, #164
	movs	r0, #10
	movs	r1, #128
	bl 0x0200bec4
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #10
	bl 0x0200bf3c
	movs	r0, #20
	bl 0x0200be5c
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r1, [pc, #128]
	adds	r2, #204
	movs	r0, #10
	bl 0x0200be8c
	movs	r0, #10
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	mov	r3, sl
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #108
	movs	r2, #168
	movs	r0, #10
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	movs	r0, #10
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, r8
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r3, #192
	ldr	r2, [r5, #12]
	lsls	r3, r3, #13
	movs	r0, #234
	ldr	r1, [r5, #8]
	adds	r2, r2, r3
	adds	r0, #255
	ldr	r3, [r5, #16]
	bl 0x0200bdf4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000c9c
	ldr	r0, [r6, #0]
	movs	r1, #28
	bl 0x0200bef4
	ldr	r3, [pc, #24]
	adds	r2, r5, #0
	adds	r2, #85
	movs	r1, #227
	adds	r0, r5, #0
	strb	r3, [r2, #0]
	lsls	r1, r1, #1
	bl 0x0200be3c
	movs	r0, #20
	bl 0x0200be5c
	b.n	.L_02000c9c
	.4byte 0x00000000
	.4byte 0x000021c7
	.4byte 0x02000240
	.2byte 0x9999
	.2byte 0x0001
.L_02000c9c:
	movs	r0, #10
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	mov	r3, sl
	ands	r3, r2
	movs	r1, #116
	strb	r3, [r0, #0]
	movs	r2, #164
	movs	r0, #10
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	movs	r0, #10
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, r8
	orrs	r3, r2
	strb	r3, [r0, #0]
	bl 0x0200bfc4
	movs	r0, #198
	adds	r0, #255
	bl 0x0200be54
	movs	r0, #227
	lsls	r0, r0, #1
	movs	r1, #0
	bl 0x0200be74
	cmp	r5, #0
	beq.n	.L_02000cf4
	adds	r0, r5, #0
	bl 0x0200bdfc
	ldr	r0, [r6, #0]
.L_02000cee:
	movs	r1, #1
	bl 0x0200bef4
.L_02000cf4:
	bl 0x0200bfbc
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #163
	bl 0x0200bdd4
	b.n	.L_02000d80
.L_02000d04:
	bl 0x0200bfbc
	movs	r1, #10
	movs	r2, #20
	adds	r1, #255
	movs	r0, #10
	bl 0x0200bf54
	mov	r0, r9
	adds	r0, #7
	bl 0x0200bf24
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #10
	movs	r1, #0
	bl 0x0200bf2c
	movs	r1, #192
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	bl 0x0200bf3c
	ldr	r0, [r6, #0]
	movs	r1, #4
	movs	r2, #10
	bl 0x0200bf04
	cmp	r5, #0
	beq.n	.L_02000d46
	adds	r0, r5, #0
	bl 0x0200bdfc
.L_02000d46:
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
	bl 0x0200bf3c
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bdd4
	b.n	.L_02000d80
.L_02000d5c:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #163
	bl 0x0200bdcc
	cmp	r0, #0
	beq.n	.L_02000d72
	ldr	r0, [pc, #44]
	bl 0x0200bf24
	b.n	.L_02000d78
.L_02000d72:
	ldr	r0, [pc, #40]
	bl 0x0200bf24
.L_02000d78:
	movs	r0, #10
	movs	r1, #0
	bl 0x0200bf2c
.L_02000d80:
	movs	r0, #10
	movs	r1, #4
	bl 0x0200bef4
	bl 0x0200be6c
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x000021cd
	.2byte 0x21c6
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r2, [r0, #12]
	movs	r3, #192
	lsls	r3, r3, #13
	adds	r2, r2, r3
	adds	r6, r1, #0
	ldr	r3, [r0, #16]
	ldr	r1, [r0, #8]
	movs	r0, #234
	adds	r0, #255
	bl 0x0200bdf4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000dee
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #28
	bl 0x0200bef4
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl 0x0200be3c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200be34
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #1
	strb	r3, [r2, #0]
.L_02000dee:
	adds	r0, r5, #0
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #3
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000e2e
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	movs	r0, #14
	bl 0x0200bdf4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000e2e
	movs	r1, #0
	bl 0x0200be34
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200bde4
	ldr	r1, [pc, #12]
	adds	r0, r5, #0
	bl 0x0200bdec
.L_02000e2e:
	pop	{r5, pc}
	.4byte 0x0300122c
	.2byte 0xc4f0
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	movs	r0, #227
	lsls	r0, r0, #1
	bl 0x0200be4c
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	bne.n	.L_02000e64
	b.n	.L_020011f2
.L_02000e64:
	ldr	r3, [pc, #968]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x0200be84
	adds	r6, r0, #0
	movs	r0, #11
	bl 0x0200be84
	movs	r3, #0
	str	r3, [sp, #0]
	mov	sl, r0
	ldr	r1, [r5, #0]
	movs	r0, #11
	movs	r2, #0
	mov	r9, r3
	bl 0x0200bf14
	ldr	r7, [pc, #932]
	mov	r1, sl
	str	r7, [r1, #108]
	movs	r0, #11
	movs	r1, #4
	movs	r2, #10
	bl 0x0200bf04
	mov	r2, r9
	mov	r3, sl
	str	r2, [r3, #108]
	ldr	r0, [pc, #916]
	bl 0x0200bf24
	movs	r1, #0
	movs	r0, #11
	bl 0x0200bf2c
	mov	r1, sl
	ldr	r2, [r6, #8]
	ldr	r3, [r1, #8]
	cmp	r2, r3
	ble.n	.L_02000ebe
	movs	r2, #1
	str	r2, [sp, #0]
.L_02000ebe:
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #884]
	adds	r2, #204
	bl 0x0200be8c
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #11
	ldr	r1, [pc, #872]
	adds	r2, #204
	bl 0x0200be8c
	ldr	r3, [sp, #0]
	cmp	r3, #0
	beq.n	.L_02000f16
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200bf34
	movs	r1, #148
	movs	r2, #214
	lsls	r2, r2, #2
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200bec4
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x0200bf3c
	movs	r1, #227
	lsls	r1, r1, #1
	adds	r0, r6, #0
	bl 0x02008da0
	mov	r1, sl
	str	r7, [r1, #108]
	mov	r8, r0
	ldr	r1, [pc, #812]
	b.n	.L_02000f4a
.L_02000f16:
	movs	r1, #128
	movs	r0, #11
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200bf34
	movs	r1, #132
	movs	r2, #214
	lsls	r2, r2, #2
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200bec4
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200bf3c
	movs	r1, #227
	lsls	r1, r1, #1
	adds	r0, r6, #0
	bl 0x02008da0
	mov	r1, sl
	str	r7, [r1, #108]
	mov	r8, r0
	ldr	r1, [pc, #760]
.L_02000f4a:
	movs	r0, #11
	bl 0x0200bea4
	mov	r2, r9
	mov	r3, sl
	str	r2, [r3, #108]
	movs	r0, #11
	movs	r1, #0
	bl 0x0200bf2c
	ldr	r3, [pc, #720]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r7, r3, r1
	ldr	r1, [r7, #0]
	movs	r0, #11
	bl 0x0200bfcc
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_02000f76
	b.n	.L_02001142
.L_02000f76:
	adds	r2, #89
	movs	r3, #9
	strb	r3, [r2, #0]
	movs	r1, #208
	movs	r3, #210
	lsls	r1, r1, #15
	lsls	r3, r3, #18
	mov	r0, r8
	movs	r2, #0
	bl 0x0200be04
	ldr	r3, [pc, #676]
	ldr	r1, [sp, #0]
	mov	fp, r3
	cmp	r1, #0
	beq.n	.L_0200105a
	movs	r1, #148
	movs	r2, #210
	str	r3, [r6, #108]
	ldr	r0, [r7, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200bec4
	movs	r2, #0
	movs	r1, #128
	str	r2, [r6, #108]
	lsls	r1, r1, #8
	ldr	r0, [r7, #0]
	bl 0x0200bf3c
	mov	r3, fp
	str	r3, [r6, #108]
	ldr	r0, [r7, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #144
	movs	r2, #210
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	ldr	r0, [r7, #0]
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	ldr	r0, [r7, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #1
	mov	r9, r1
	mov	r2, r9
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #140
	movs	r3, #210
	lsls	r1, r1, #17
	movs	r2, #0
	lsls	r3, r3, #18
	mov	r0, r8
	bl 0x0200be04
	ldr	r0, [r7, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #148
	movs	r2, #210
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	ldr	r0, [r7, #0]
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	ldr	r0, [r7, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r1, r9
	orrs	r3, r1
	strb	r3, [r0, #0]
	movs	r2, #0
	str	r2, [r6, #108]
	ldr	r0, [r7, #0]
	bl 0x0200be9c
	movs	r0, #1
	bl 0x0200bd9c
	mov	r3, fp
	mov	r1, sl
	str	r3, [r1, #108]
	movs	r0, #11
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #146
	b.n	.L_0200111c
.L_0200105a:
	mov	r3, fp
	movs	r1, #132
	movs	r2, #210
	str	r3, [r6, #108]
	lsls	r2, r2, #2
	ldr	r0, [r7, #0]
	lsls	r1, r1, #1
	bl 0x0200bec4
	ldr	r1, [sp, #0]
	ldr	r0, [r7, #0]
	str	r1, [r6, #108]
	movs	r1, #0
	bl 0x0200bf3c
	mov	r2, fp
	str	r2, [r6, #108]
	ldr	r0, [r7, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #136
	movs	r2, #210
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	ldr	r0, [r7, #0]
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	ldr	r0, [r7, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #1
	mov	r9, r1
	mov	r2, r9
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #140
	movs	r3, #210
	lsls	r1, r1, #17
	movs	r2, #0
	lsls	r3, r3, #18
	mov	r0, r8
	bl 0x0200be04
	ldr	r0, [r7, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #132
	movs	r2, #210
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	ldr	r0, [r7, #0]
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	ldr	r0, [r7, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r1, r9
	orrs	r3, r1
	strb	r3, [r0, #0]
	ldr	r2, [sp, #0]
	ldr	r0, [r7, #0]
	str	r2, [r6, #108]
	bl 0x0200be9c
	movs	r0, #1
	bl 0x0200bd9c
	mov	r3, fp
	mov	r1, sl
	str	r3, [r1, #108]
	movs	r0, #11
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #134
.L_0200111c:
	ands	r5, r3
	movs	r2, #211
	lsls	r2, r2, #2
	lsls	r1, r1, #1
	strb	r5, [r0, #0]
	movs	r0, #11
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	movs	r0, #11
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, r9
	orrs	r3, r2
	strb	r3, [r0, #0]
.L_02001142:
	movs	r7, #200
	adds	r7, #255
	adds	r1, r7, #0
	adds	r0, r6, #0
	bl 0x02008da0
	mov	r9, r0
	movs	r0, #11
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #140
	movs	r2, #214
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	movs	r0, #11
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	movs	r0, #11
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r5, #0
	mov	r3, sl
	str	r5, [r3, #108]
	bl 0x0200bfc4
	movs	r1, #1
	ldr	r0, [pc, #180]
	bl 0x0200be44
	movs	r0, #227
	lsls	r0, r0, #1
	bl 0x0200be54
	movs	r1, #0
	adds	r0, r7, #0
	bl 0x0200be74
	bl 0x0200bfbc
	ldr	r3, [pc, #132]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r5, r3, r1
	ldr	r0, [r5, #0]
	bl 0x0200be84
	movs	r3, #128
	lsls	r3, r3, #7
	mov	r2, r9
	strh	r3, [r0, #6]
	cmp	r2, #0
	beq.n	.L_020011d2
	mov	r0, r9
	bl 0x0200bdfc
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200bef4
.L_020011d2:
	movs	r1, #192
	movs	r0, #11
	lsls	r1, r1, #8
	bl 0x0200bf3c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #161
	bl 0x0200bdd4
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #22
	bl 0x0200bdd4
	b.n	.L_02001262
.L_020011f2:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #161
	bl 0x0200bdcc
	cmp	r0, #0
	beq.n	.L_02001254
	ldr	r3, [pc, #44]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	movs	r0, #11
	ldr	r1, [r3, #0]
	bl 0x0200bfcc
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #22
	bl 0x0200bdcc
	cmp	r0, #0
	beq.n	.L_02001226
	ldr	r0, [pc, #44]
	bl 0x0200bf24
	b.n	.L_0200125a
.L_02001226:
	ldr	r0, [pc, #40]
	bl 0x0200bf24
	b.n	.L_0200125a
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02008df9
	.4byte 0x000021d2
	.4byte 0x00019999
	.4byte 0x0200c3f8
	.4byte 0x0200c300
	.4byte 0x000021d4
	.4byte 0x000021d5
	.2byte 0x21d6
	.2byte 0x0000
.L_02001254:
	ldr	r0, [pc, #28]
	bl 0x0200bf24
.L_0200125a:
	movs	r0, #11
	movs	r1, #0
	bl 0x0200bf2c
.L_02001262:
	bl 0x0200be6c
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x21d1
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r0, #9
	bl 0x0200be84
	adds	r5, r0, #0
	ldr	r2, [r5, #12]
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r2, r2, r3
	ldr	r1, [r5, #8]
	ldr	r3, [r5, #16]
	adds	r0, r6, #0
	bl 0x0200be04
	ldrh	r3, [r5, #6]
	strh	r3, [r6, #6]
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #536]
	adds	r5, r0, #0
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r6, r6, r0
	ldr	r0, [r6, #0]
	sub	sp, #36
	bl 0x0200be84
	adds	r7, r0, #0
	movs	r0, #9
	bl 0x0200be84
	ldr	r3, [pc, #512]
	mov	r8, sp
	mov	r2, r8
	mov	fp, r0
	ldmia	r3!, {r0, r1, r4}
	stmia	r2!, {r0, r1, r4}
	ldmia	r3!, {r0, r1, r4}
	stmia	r2!, {r0, r1, r4}
	ldmia	r3!, {r0, r1, r4}
	stmia	r2!, {r0, r1, r4}
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	bl 0x0200bf6c
	bl 0x0200bfbc
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #9
	bl 0x0200bf54
	ldr	r1, [pc, #464]
	mov	r2, fp
	str	r1, [r2, #108]
	movs	r0, #9
	mov	r1, r8
	bl 0x0200bea4
	mov	r4, fp
	movs	r3, #0
	str	r3, [r4, #108]
	ldr	r0, [pc, #448]
	mov	r8, r3
	bl 0x0200bf24
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200bf3c
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #9
	ldr	r1, [pc, #412]
	adds	r2, #204
	bl 0x0200be8c
	ldr	r0, [pc, #396]
	mov	r1, fp
	str	r0, [r1, #108]
	movs	r2, #226
	movs	r1, #148
	movs	r0, #9
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200bec4
	mov	r2, r8
	mov	r3, fp
	movs	r1, #160
	str	r2, [r3, #108]
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200bf34
	ldr	r1, [pc, #360]
	movs	r0, #204
	ldr	r4, [pc, #364]
	lsls	r0, r0, #8
	adds	r0, #204
	mov	sl, r0
	str	r0, [r5, #52]
	str	r1, [r5, #108]
	adds	r0, r5, #0
	movs	r1, #2
	mov	r9, r4
	str	r4, [r5, #48]
	bl 0x0200bde4
	movs	r1, #139
	movs	r3, #226
	lsls	r3, r3, #17
	movs	r2, #0
	adds	r0, r5, #0
	lsls	r1, r1, #17
	bl 0x0200be0c
	adds	r0, r5, #0
	bl 0x0200be14
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200bde4
	movs	r0, #200
	mov	r2, r8
	adds	r0, #255
	str	r2, [r5, #108]
	bl 0x0200be54
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	movs	r1, #166
	lsls	r1, r1, #2
	movs	r0, #88
	bl 0x0200bdc4
	movs	r3, #203
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r0, r0, r3
	movs	r3, #99
	strb	r3, [r0, #0]
	movs	r1, #3
	ldr	r0, [r6, #0]
	bl 0x0200befc
	ldr	r0, [r6, #0]
	mov	r1, r9
	mov	r2, sl
	bl 0x0200be8c
	ldr	r4, [pc, #240]
	movs	r1, #156
	movs	r2, #223
	str	r4, [r7, #108]
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200bec4
	mov	r0, r8
	str	r0, [r7, #108]
	movs	r1, #6
	ldr	r0, [r6, #0]
	movs	r2, #0
	bl 0x0200bf04
	movs	r1, #148
	movs	r2, #226
	lsls	r2, r2, #1
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x0200beb4
	ldr	r3, [pc, #208]
	ldr	r0, [r6, #0]
	str	r3, [r7, #108]
	bl 0x0200be84
	movs	r1, #15
	bl 0x0200bf1c
	movs	r1, #3
	movs	r0, #9
	bl 0x0200bef4
	movs	r0, #20
	bl 0x0200be5c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	movs	r2, #204
	lsls	r2, r2, #7
	mov	r1, sl
	movs	r0, #9
	adds	r2, #102
	bl 0x0200be8c
	movs	r3, #204
	ldr	r2, [pc, #140]
	lsls	r3, r3, #7
	adds	r3, #102
	str	r3, [r5, #52]
	mov	r3, fp
	str	r2, [r3, #108]
	mov	r1, sl
	movs	r2, #128
	str	r1, [r5, #48]
	lsls	r2, r2, #2
	movs	r1, #144
	adds	r2, #26
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x0200beac
	movs	r0, #20
	bl 0x0200be5c
	bl 0x0200bfc4
	bl 0x0200b4ec
	ldr	r4, [pc, #96]
	adds	r0, r5, #0
	movs	r1, #2
	str	r4, [r5, #108]
	bl 0x0200bde4
	movs	r1, #144
	lsls	r1, r1, #17
	adds	r0, r5, #0
	movs	r2, #0
	ldr	r3, [pc, #92]
	bl 0x0200be0c
	movs	r0, #20
	bl 0x0200be5c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #214
	movs	r2, #129
	lsls	r0, r0, #1
	lsls	r2, r2, #1
	adds	r3, r3, r0
	adds	r2, #255
	str	r2, [r3, #0]
	bl 0x0200bf94
	bl 0x0200bf9c
	movs	r0, #3
	bl 0x0200bf7c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #165
	bl 0x0200bdd4
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200c004
	.4byte 0x02008df9
	.4byte 0x000021de
	.4byte 0x00019999
	.4byte 0x02009279
	.2byte 0x0000
	.2byte 0x021a
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	movs	r0, #200
	adds	r0, #255
	bl 0x0200be4c
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_0200150a
	b.n	.L_020018ac
.L_0200150a:
	ldr	r3, [pc, #904]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	bl 0x0200be84
	mov	r8, r0
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bdcc
	mov	sl, r0
	cmp	r0, #0
	bne.n	.L_0200152c
	b.n	.L_02001694
.L_0200152c:
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r6, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200be8c
	ldr	r7, [pc, #856]
	mov	r3, r8
	movs	r1, #159
	movs	r2, #212
	str	r7, [r3, #108]
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200bec4
	movs	r3, #0
	mov	r2, r8
	movs	r1, #128
	str	r3, [r2, #108]
	ldr	r0, [r6, #0]
	movs	r2, #0
	lsls	r1, r1, #7
	bl 0x0200bf34
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x0200bf3c
	ldr	r5, [pc, #812]
	adds	r0, r5, #0
	bl 0x0200bf24
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	bl 0x0200bfc4
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x0200be7c
	mov	sl, r0
	cmp	r0, #0
	bne.n	.L_02001688
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	bl 0x0200bf3c
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r1, [pc, #768]
	adds	r2, #204
	ldr	r0, [r6, #0]
	bl 0x0200be8c
	mov	r3, r8
	str	r7, [r3, #108]
	ldr	r0, [r6, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	mov	fp, r3
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #151
	movs	r2, #212
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	ldr	r0, [r6, #0]
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	ldr	r0, [r6, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #1
	mov	r9, r2
	mov	r2, r9
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #148
	movs	r3, #212
	lsls	r1, r1, #17
	movs	r2, #0
	lsls	r3, r3, #17
	movs	r0, #163
	bl 0x0200bdf4
	movs	r1, #148
	movs	r3, #212
	adds	r7, r0, #0
	lsls	r1, r1, #17
	movs	r0, #14
	movs	r2, #0
	lsls	r3, r3, #17
	bl 0x0200bdf4
	adds	r5, r0, #0
	cmp	r7, #0
	beq.n	.L_0200161e
	adds	r3, r7, #0
	adds	r3, #85
	mov	r2, sl
	strb	r2, [r3, #0]
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200be34
.L_0200161e:
	cmp	r5, #0
	beq.n	.L_02001640
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #28]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200be34
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200bde4
	ldr	r1, [pc, #616]
	adds	r0, r5, #0
	bl 0x0200bdec
.L_02001640:
	ldr	r0, [r6, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	mov	r3, fp
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #159
	movs	r2, #212
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	ldr	r0, [r6, #0]
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	ldr	r0, [r6, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, r9
	orrs	r3, r2
	strb	r3, [r0, #0]
	mov	r2, r8
	mov	r3, sl
	movs	r1, #192
	str	r3, [r2, #108]
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200bf34
	b.n	.L_0200182c
.L_02001688:
	bl 0x0200bfbc
	subs	r0, r5, #1
	bl 0x0200bf24
	b.n	.L_02001a1c
.L_02001694:
	ldr	r0, [pc, #528]
	bl 0x0200bf24
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #20
	ldr	r0, [r6, #0]
	bl 0x0200bf54
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r6, #0]
	adds	r1, #204
	adds	r2, #102
	bl 0x0200be8c
	ldr	r3, [pc, #468]
	mov	r2, r8
	str	r3, [r2, #108]
	movs	r1, #159
	movs	r2, #212
	lsls	r2, r2, #1
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	mov	r9, r3
	movs	r7, #0
	bl 0x0200bec4
	mov	r3, r8
	movs	r1, #128
	str	r7, [r3, #108]
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	bl 0x0200bf3c
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r1, [pc, #436]
	ldr	r0, [r6, #0]
	adds	r2, #204
	bl 0x0200be8c
	mov	r2, r9
	mov	r3, r8
	str	r2, [r3, #108]
	ldr	r0, [r6, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #151
	movs	r2, #212
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	ldr	r0, [r6, #0]
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	ldr	r0, [r6, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #1
	mov	fp, r2
	mov	r2, fp
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #148
	movs	r3, #212
	lsls	r1, r1, #17
	movs	r2, #0
	lsls	r3, r3, #17
	movs	r0, #163
	bl 0x0200bdf4
	movs	r1, #148
	movs	r3, #212
	adds	r7, r0, #0
	lsls	r1, r1, #17
	movs	r0, #14
	movs	r2, #0
	lsls	r3, r3, #17
	bl 0x0200bdf4
	adds	r5, r0, #0
	cmp	r7, #0
	beq.n	.L_0200176a
	adds	r3, r7, #0
	adds	r3, #85
	mov	r2, sl
	strb	r2, [r3, #0]
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200be34
.L_0200176a:
	cmp	r5, #0
	beq.n	.L_0200178c
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #28]
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200be34
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200bde4
	ldr	r1, [pc, #284]
	adds	r0, r5, #0
	bl 0x0200bdec
.L_0200178c:
	ldr	r0, [r6, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #159
	movs	r2, #212
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	ldr	r0, [r6, #0]
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	ldr	r0, [r6, #0]
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, fp
	orrs	r3, r2
	strb	r3, [r0, #0]
	mov	r2, r8
	mov	r3, sl
	movs	r1, #128
	str	r3, [r2, #108]
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200bf34
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x0200bf54
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x0200bf3c
	movs	r0, #9
	bl 0x0200be84
	mov	r3, r9
	adds	r5, r0, #0
	str	r3, [r5, #108]
	movs	r0, #9
	movs	r1, #4
	movs	r2, #20
	bl 0x0200bf04
	mov	r2, sl
	str	r2, [r5, #108]
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	movs	r1, #208
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200bf3c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	bl 0x0200bfc4
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x0200be7c
	cmp	r0, #0
	bne.n	.L_02001834
.L_0200182c:
	adds	r0, r7, #0
	bl 0x0200929c
	b.n	.L_02001a3e
.L_02001834:
	bl 0x0200bfbc
	movs	r1, #10
	movs	r2, #0
	adds	r1, #255
	movs	r0, #9
	bl 0x0200bf54
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	movs	r1, #160
	movs	r2, #0
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200bf34
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #8
	bl 0x0200bf3c
	mov	r3, r9
	mov	r2, r8
	str	r3, [r2, #108]
	movs	r1, #151
	movs	r2, #212
	lsls	r2, r2, #1
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x0200bec4
	mov	r3, sl
	mov	r2, r8
	str	r3, [r2, #108]
	cmp	r7, #0
	beq.n	.L_02001886
	adds	r0, r7, #0
	bl 0x0200bdfc
.L_02001886:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bdd4
	b.n	.L_02001a3e
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02008df9
	.4byte 0x000021dd
	.4byte 0x00019999
	.4byte 0x0200c4f0
	.2byte 0x21d9
	.2byte 0x0000
.L_020018ac:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #165
	bl 0x0200bdcc
	cmp	r0, #0
	bne.n	.L_020018bc
	b.n	.L_02001a30
.L_020018bc:
	ldr	r3, [pc, #400]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r7, r3, r2
	ldr	r1, [r7, #0]
	movs	r2, #0
	movs	r0, #9
	bl 0x0200bf14
	ldr	r0, [pc, #388]
	bl 0x0200bf24
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	bl 0x0200bfc4
	ldr	r0, [r7, #0]
	movs	r1, #0
	bl 0x0200be7c
	cmp	r0, #0
	beq.n	.L_020018ee
	b.n	.L_02001a18
.L_020018ee:
	ldr	r0, [r7, #0]
	bl 0x0200be84
	adds	r6, r0, #0
	bl 0x0200bfbc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r5, #226
	lsls	r5, r5, #1
	adds	r2, r2, r5
	mov	r8, r3
	ldrh	r3, [r2, #0]
	movs	r0, #9
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x0200bf2c
	movs	r1, #166
	lsls	r1, r1, #2
	movs	r0, #88
	bl 0x0200bdc4
	movs	r2, #203
	lsls	r2, r2, #1
	adds	r2, #255
	movs	r3, #99
	adds	r0, r0, r2
	strb	r3, [r0, #0]
	movs	r1, #3
	ldr	r0, [r7, #0]
	bl 0x0200befc
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200bf3c
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r0, [r7, #0]
	ldr	r1, [pc, #272]
	adds	r2, #204
	bl 0x0200be8c
	ldr	r0, [r7, #0]
	movs	r1, #6
	movs	r2, #0
	bl 0x0200bf04
	movs	r1, #148
	adds	r2, r5, #0
	lsls	r1, r1, #1
	ldr	r0, [r7, #0]
	bl 0x0200beb4
	ldr	r3, [pc, #248]
	ldr	r0, [r7, #0]
	str	r3, [r6, #108]
	bl 0x0200be84
	movs	r1, #15
	bl 0x0200bf1c
	movs	r1, #3
	movs	r0, #9
	bl 0x0200bef4
	movs	r0, #20
	bl 0x0200be5c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	bl 0x0200bfc4
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #9
	adds	r1, #204
	adds	r2, #102
	bl 0x0200be8c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r1, #204
	adds	r2, #102
	movs	r0, #10
	bl 0x0200be8c
	movs	r0, #9
	bl 0x0200be84
	ldr	r5, [pc, #168]
	movs	r2, #128
	movs	r1, #144
	lsls	r2, r2, #2
	lsls	r1, r1, #1
	adds	r2, #26
	str	r5, [r0, #108]
	movs	r0, #9
	bl 0x0200beac
	movs	r0, #40
	bl 0x0200be5c
	bl 0x0200bfc4
	bl 0x0200b4ec
	movs	r0, #10
	bl 0x0200be84
	movs	r2, #128
	movs	r1, #144
	lsls	r2, r2, #2
	lsls	r1, r1, #1
	adds	r2, #26
	str	r5, [r0, #108]
	movs	r0, #10
	bl 0x0200bebc
	movs	r0, #20
	bl 0x0200be5c
	mov	r2, r8
	ldr	r3, [r2, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #85
	str	r2, [r3, #0]
	bl 0x0200bf94
	bl 0x0200bf9c
	bl 0x0200bfbc
	movs	r0, #3
	bl 0x0200bf7c
	b.n	.L_02001a3e
.L_02001a18:
	bl 0x0200bfbc
.L_02001a1c:
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200bf3c
	b.n	.L_02001a3e
.L_02001a30:
	ldr	r0, [pc, #48]
	bl 0x0200bf24
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
.L_02001a3e:
	bl 0x0200be6c
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000021eb
	.4byte 0x00019999
	.4byte 0x02009279
	.4byte 0x02008df9
	.2byte 0x21d8
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #356]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x0200be84
	mov	sl, r0
	movs	r0, #9
	bl 0x0200be84
	mov	r8, r0
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #9
	bl 0x0200bf14
	movs	r0, #10
	bl 0x0200be5c
	ldr	r6, [pc, #312]
	adds	r0, r6, #0
	bl 0x0200bf24
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	bl 0x0200bfc4
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200be7c
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_02001bae
	bl 0x0200bfbc
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	movs	r1, #166
	lsls	r1, r1, #2
	movs	r0, #88
	bl 0x0200bdc4
	movs	r3, #203
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r0, r0, r3
	movs	r3, #99
	strb	r3, [r0, #0]
	movs	r1, #6
	ldr	r0, [r5, #0]
	movs	r2, #0
	bl 0x0200bf04
	movs	r2, #204
	lsls	r2, r2, #8
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #232]
	adds	r2, #204
	bl 0x0200be8c
	movs	r2, #192
	ldr	r0, [r5, #0]
	movs	r1, #152
	lsls	r2, r2, #2
	bl 0x0200beb4
	ldr	r3, [pc, #216]
	mov	r2, sl
	str	r3, [r2, #108]
	ldr	r0, [r5, #0]
	bl 0x0200be84
	movs	r1, #15
	bl 0x0200bf1c
	movs	r1, #3
	movs	r0, #9
	bl 0x0200bef4
	movs	r0, #20
	bl 0x0200be5c
	bl 0x0200bfc4
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #9
	ldr	r1, [pc, #168]
	adds	r2, #204
	bl 0x0200be8c
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #10
	ldr	r1, [pc, #152]
	bl 0x0200be8c
	ldr	r5, [pc, #156]
	mov	r6, r8
	mov	r3, r8
	adds	r6, #99
	ldr	r1, [pc, #152]
	strb	r7, [r6, #0]
	movs	r0, #9
	str	r5, [r3, #108]
	bl 0x0200be94
	movs	r0, #30
	bl 0x0200be5c
	movs	r0, #10
	bl 0x0200be84
	ldr	r1, [pc, #132]
	str	r5, [r0, #108]
	movs	r0, #10
	bl 0x0200be94
.L_02001b74:
	movs	r0, #1
	bl 0x0200bd9c
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_02001b74
	bl 0x0200b4ec
	movs	r0, #1
	bl 0x0200bd9c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #85
	str	r2, [r3, #0]
	bl 0x0200bf94
	bl 0x0200bf9c
	bl 0x0200bfbc
	movs	r0, #4
	bl 0x0200bf7c
	b.n	.L_02001bca
.L_02001bae:
	bl 0x0200bfbc
	adds	r0, r6, #4
	bl 0x0200bf24
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200bf3c
.L_02001bca:
	bl 0x0200be6c
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000021f2
	.4byte 0x00019999
	.4byte 0x02009279
	.4byte 0x02008df9
	.4byte 0x0200c5a4
	.2byte 0xc5f0
	.2byte 0x0200
	push	{lr}
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	ldr	r3, [pc, #44]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #9
	bl 0x0200bf14
	ldr	r0, [pc, #32]
	bl 0x0200bf24
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	movs	r1, #176
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200bf3c
	bl 0x0200be6c
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000218e
	.4byte 0x049b23c0
	.4byte 0x21ad6edb
	.4byte 0x185a0049
	.4byte 0x80132301
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #88]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200be84
	ldr	r3, [r0, #12]
	movs	r2, #160
	lsls	r2, r2, #14
	cmp	r3, r2
	bge.n	.L_02001c84
	movs	r0, #11
	bl 0x0200be84
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r5, #8
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #12
	bl 0x0200be84
	adds	r0, #89
	ldrb	r3, [r0, #0]
	orrs	r5, r3
	b.n	.L_02001ca2
.L_02001c84:
	movs	r0, #11
	bl 0x0200be84
	adds	r0, #89
	ldrb	r2, [r0, #0]
	movs	r5, #247
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #12
	bl 0x0200be84
	adds	r0, #89
	ldrb	r3, [r0, #0]
	ands	r5, r3
.L_02001ca2:
	strb	r5, [r0, #0]
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x781b4b10
	.4byte 0x18c00098
	.4byte 0x01804b0f
	.4byte 0x220018c0
	.4byte 0x21805e83
	.4byte 0x311404c9
	.4byte 0x2380800b
	.4byte 0x33b004db
	.4byte 0x22c5895c
	.4byte 0x32ff0212
	.4byte 0x815a4022
	.4byte 0x895c22fe
	.4byte 0x32ff01d2
	.4byte 0x815a4022
	.4byte 0x895a3002
	.4byte 0xc3074a03
	.4byte 0x47703b0c
	.4byte 0x0200cf04
	.4byte 0x0200cf10
	.2byte 0x0001
	.2byte 0xa260
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r1, #188
	lsls	r1, r1, #1
	adds	r1, r1, r3
	ldr	r3, [pc, #112]
	sub	sp, #8
	ldrb	r2, [r3, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r2, r3, #2
	adds	r2, r2, r3
	ldr	r3, [pc, #104]
	lsls	r2, r2, #6
	adds	r2, r2, r3
	add	r7, sp, #4
	movs	r3, #0
	str	r3, [r7, #0]
	mov	sl, r1
	mov	r8, r2
.L_02001d2e:
	ldr	r3, [pc, #92]
	ldr	r0, [r7, #0]
	ldrb	r3, [r3, #0]
	mov	r1, sl
	adds	r0, r0, r3
	movs	r2, #6
	ldrsh	r3, [r1, r2]
	adds	r0, r0, r3
	lsls	r0, r0, #9
	bl 0x0200bdb4
	str	r0, [sp, #0]
	mov	r3, sl
	movs	r2, #2
	ldrsh	r5, [r3, r2]
	ldr	r6, [r7, #0]
	asrs	r0, r0, #15
	adds	r5, r5, r0
	movs	r1, #3
	adds	r0, r6, #0
	bl 0x0200bd94
	adds	r5, r5, r0
	mov	r1, r8
	subs	r5, #1
	movs	r2, #2
	adds	r6, #1
	strh	r5, [r1, #0]
	add	r8, r2
	str	r6, [r7, #0]
	cmp	r6, #160
	bne.n	.L_02001d2e
	ldr	r3, [pc, #20]
	movs	r1, #1
	ldrb	r2, [r3, #0]
	add	sp, #8
	eors	r2, r1
	strb	r2, [r3, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200cf04
	.4byte 0x0200cf10
	.2byte 0x122c
	.2byte 0x0300
	push	{lr}
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl 0x0200bda4
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #8]
	bl 0x0200bda4
	pop	{pc}
	.4byte 0x02009cfd
	.2byte 0x9cad
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	movs	r0, #19
	bl 0x0200be84
	ldr	r2, [pc, #396]
	ldr	r3, [r0, #8]
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	cmp	r3, r2
	bls.n	.L_02001dce
	b.n	.L_02001f44
.L_02001dce:
	ldr	r0, [r0, #16]
	movs	r3, #164
	lsls	r3, r3, #16
	cmp	r0, r3
	bge.n	.L_02001dda
	b.n	.L_02001f44
.L_02001dda:
	movs	r2, #172
	lsls	r2, r2, #16
	cmp	r0, r2
	ble.n	.L_02001de4
	b.n	.L_02001f44
.L_02001de4:
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	movs	r0, #19
	movs	r1, #1
	bl 0x0200bef4
	ldr	r3, [pc, #344]
	movs	r2, #133
	mov	r8, r3
	lsls	r2, r2, #2
	add	r8, r2
	mov	r3, r8
	ldr	r0, [r3, #0]
	movs	r1, #216
	movs	r2, #184
	bl 0x0200bec4
	mov	r2, r8
	ldr	r0, [r2, #0]
	movs	r1, #9
	bl 0x0200bfac
	ldr	r0, [pc, #316]
	bl 0x0200bf24
	movs	r0, #9
	movs	r1, #2
	movs	r2, #10
	bl 0x0200bf04
	movs	r0, #9
	movs	r1, #4
	movs	r2, #10
	bl 0x0200bf04
	mov	r3, r8
	ldr	r0, [r3, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200bf34
	movs	r1, #128
	movs	r2, #10
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200bf34
	movs	r1, #0
	movs	r0, #9
	bl 0x0200bf2c
	movs	r0, #9
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	movs	r2, #0
	strb	r3, [r0, #0]
	movs	r1, #222
	mov	sl, r2
	movs	r0, #9
	movs	r2, #186
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	movs	r0, #9
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r6, #1
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #186
	ands	r5, r3
	movs	r1, #232
	strb	r5, [r0, #0]
	movs	r0, #9
	bl 0x0200bec4
	movs	r0, #1
	bl 0x0200be5c
	movs	r0, #9
	bl 0x0200be84
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #128
	orrs	r6, r3
	strb	r6, [r0, #0]
	mov	r3, r8
	ldr	r0, [r3, #0]
	lsls	r1, r1, #7
	bl 0x0200bf3c
	mov	r2, r8
	ldr	r0, [r2, #0]
	movs	r1, #28
	bl 0x0200bef4
	mov	r3, r8
	ldr	r0, [r3, #0]
	bl 0x0200be84
	adds	r6, r0, #0
	movs	r0, #20
	bl 0x0200be84
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #85
	mov	r2, sl
	movs	r1, #226
	strb	r2, [r3, #0]
	lsls	r1, r1, #1
	bl 0x0200be3c
	ldr	r2, [r6, #12]
.L_02001eec:
	movs	r3, #192
	lsls	r3, r3, #13
	adds	r2, r2, r3
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	bl 0x0200be04
	movs	r0, #20
	bl 0x0200be5c
	bl 0x0200bfc4
	movs	r0, #226
	movs	r1, #0
	lsls	r0, r0, #1
	bl 0x0200be74
	bl 0x0200bfbc
	movs	r0, #20
	movs	r1, #0
	movs	r2, #0
	bl 0x0200bedc
	mov	r2, r8
	ldr	r0, [r2, #0]
	movs	r1, #1
	bl 0x0200bef4
	bl 0x0200bfb4
	movs	r1, #176
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x0200bf3c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #162
	bl 0x0200bdd4
	bl 0x0200be6c
.L_02001f44:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.4byte 0xff1c0000
	.4byte 0x02000240
	.2byte 0x21b5
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02001f6c
	movs	r0, #118
	bl 0x0200bffc
.L_02001f6c:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #10
	bl 0x0200be84
	adds	r5, r0, #0
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	movs	r0, #10
	bl 0x0200be9c
	movs	r0, #1
	bl 0x0200bd9c
	movs	r2, #0
	mov	sl, r2
	adds	r7, r5, #0
	movs	r2, #204
	adds	r7, #85
	mov	r3, sl
	lsls	r2, r2, #8
	strb	r3, [r7, #0]
	movs	r0, #10
	ldr	r1, [pc, #208]
	adds	r2, #204
	bl 0x0200be8c
	movs	r1, #134
	movs	r2, #128
	movs	r3, #195
	lsls	r1, r1, #17
	lsls	r3, r3, #17
	adds	r0, r5, #0
	lsls	r2, r2, #15
	bl 0x0200be04
	movs	r0, #1
	bl 0x0200bd9c
	movs	r2, #128
	lsls	r2, r2, #16
	str	r2, [r5, #20]
	movs	r0, #1
	mov	r8, r2
	bl 0x0200bd9c
	movs	r3, #160
	lsls	r3, r3, #16
	str	r3, [r5, #12]
	movs	r0, #1
	bl 0x0200bd9c
	ldr	r6, [pc, #156]
	movs	r1, #144
	adds	r0, r6, #0
	lsls	r1, r1, #3
	bl 0x0200bda4
	movs	r1, #132
	movs	r3, #160
	mov	r2, r8
	lsls	r3, r3, #17
	lsls	r1, r1, #16
	adds	r0, r5, #0
	bl 0x0200be0c
	adds	r0, r5, #0
	bl 0x0200be14
	adds	r0, r6, #0
	bl 0x0200bdac
	movs	r0, #10
	movs	r1, #5
	bl 0x0200bef4
	movs	r3, #3
	strb	r3, [r7, #0]
	movs	r0, #10
	movs	r1, #2
	bl 0x0200bf44
	ldr	r3, [pc, #100]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #10
	bl 0x0200bf14
	movs	r0, #10
	bl 0x0200be5c
	movs	r0, #10
	movs	r1, #4
	movs	r2, #0
	bl 0x0200bf04
	movs	r2, #156
	lsls	r2, r2, #1
	movs	r0, #10
	movs	r1, #120
	bl 0x0200beb4
	movs	r0, #10
	movs	r1, #5
	bl 0x0200bef4
	mov	r3, sl
	strb	r3, [r7, #0]
	movs	r1, #0
	movs	r0, #10
	bl 0x0200bf3c
	movs	r0, #139
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bdd4
	movs	r0, #1
	bl 0x0200bd9c
	bl 0x0200be6c
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x00019999
	.4byte 0x02009f59
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	ldr	r3, [pc, #20]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #6
	movs	r2, #1
	bl 0x0200bf0c
	bl 0x0200be6c
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200be84
	movs	r3, #2
	adds	r0, #34
	strb	r3, [r0, #0]
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #20]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200be84
	movs	r3, #0
	adds	r0, #34
	strb	r3, [r0, #0]
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	adds	r5, r2, #0
	mov	ip, r3
	movs	r3, #156
	lsls	r3, r3, #1
	add	r3, ip
	ldr	r4, [r3, #0]
	movs	r3, #212
	lsls	r3, r3, #1
	add	r3, ip
	ldr	r2, [r3, #0]
	cmp	r0, #0
	bge.n	.L_02002112
	ldr	r3, [pc, #48]
	adds	r0, r0, r3
.L_02002112:
	asrs	r0, r0, #20
	cmp	r1, #0
	bge.n	.L_0200211c
	ldr	r3, [pc, #36]
	adds	r1, r1, r3
.L_0200211c:
	asrs	r3, r1, #20
	lsls	r3, r3, #7
	adds	r0, r0, r3
	movs	r3, #1
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_02002130
	lsls	r3, r0, #2
	adds	r4, r4, r3
	strb	r6, [r4, #2]
.L_02002130:
	movs	r3, #2
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_0200213e
	lsls	r3, r0, #2
	adds	r2, r2, r3
	strb	r6, [r2, #2]
.L_0200213e:
	pop	{r5, r6, pc}
	.2byte 0xffff
	.2byte 0x000f
	push	{r5, r6, lr}
	movs	r0, #12
	bl 0x0200be84
	ldr	r5, [pc, #112]
	ldr	r3, [r0, #8]
	ldr	r6, [pc, #112]
	str	r3, [r5, #0]
	ldr	r3, [r0, #16]
	movs	r0, #13
	str	r3, [r6, #0]
	bl 0x0200be84
	ldr	r3, [pc, #104]
	ldr	r4, [r0, #8]
	str	r4, [r3, #0]
	ldr	r3, [pc, #100]
	ldr	r2, [r0, #16]
	str	r2, [r3, #0]
	ldr	r0, [r5, #0]
	asrs	r3, r0, #20
	cmp	r3, #7
	bne.n	.L_0200218e
	ldr	r1, [r6, #0]
	asrs	r3, r1, #20
	cmp	r3, #29
	bne.n	.L_0200218e
	asrs	r3, r4, #20
	cmp	r3, #7
	bne.n	.L_0200218e
	asrs	r3, r2, #20
	cmp	r3, #28
	bne.n	.L_0200218e
	movs	r2, #2
	movs	r3, #0
	bl 0x0200a0ec
.L_0200218e:
	ldr	r3, [pc, #48]
	ldr	r3, [r3, #0]
	asrs	r3, r3, #20
	cmp	r3, #12
	bne.n	.L_020021be
	ldr	r3, [pc, #40]
	ldr	r3, [r3, #0]
	asrs	r3, r3, #20
	cmp	r3, #25
	bne.n	.L_020021be
	ldr	r3, [pc, #36]
	ldr	r0, [r3, #0]
	asrs	r3, r0, #20
	cmp	r3, #12
	bne.n	.L_020021be
	ldr	r3, [pc, #28]
	ldr	r1, [r3, #0]
	asrs	r3, r1, #20
	cmp	r3, #26
	bne.n	.L_020021be
	movs	r2, #2
	movs	r3, #0
	bl 0x0200a0ec
.L_020021be:
	pop	{r5, r6, pc}
	.4byte 0x0200d19c
	.4byte 0x0200d194
	.4byte 0x0200d198
	.2byte 0xd190
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #12
	sub	sp, #16
	bl 0x0200be84
	adds	r7, r0, #0
	movs	r0, #13
	bl 0x0200be84
	ldr	r3, [r7, #8]
	ldr	r5, [pc, #556]
	asrs	r3, r3, #20
	mov	fp, r3
	ldr	r3, [r7, #16]
	mov	r8, r0
	asrs	r3, r3, #20
	mov	r9, r3
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	str	r3, [sp, #12]
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	str	r3, [sp, #8]
	ldr	r0, [r5, #0]
	asrs	r3, r0, #20
	cmp	r3, #12
	bne.n	.L_02002232
	ldr	r6, [pc, #528]
	ldr	r1, [r6, #0]
	asrs	r3, r1, #20
	cmp	r3, #24
	bne.n	.L_02002232
	movs	r2, #1
	movs	r3, #21
	bl 0x0200a0ec
	ldr	r0, [r5, #0]
	ldr	r1, [r6, #0]
	movs	r2, #2
	movs	r3, #0
	bl 0x0200a0ec
	b.n	.L_02002240
.L_02002232:
	ldr	r3, [pc, #496]
	ldr	r0, [r5, #0]
	ldr	r1, [r3, #0]
	movs	r2, #3
	movs	r3, #0
	bl 0x0200a0ec
.L_02002240:
	ldr	r5, [pc, #484]
	ldr	r0, [r5, #0]
	asrs	r3, r0, #20
	cmp	r3, #7
	bne.n	.L_0200226a
	ldr	r6, [pc, #480]
	ldr	r1, [r6, #0]
	asrs	r3, r1, #20
	cmp	r3, #27
	bne.n	.L_0200226a
	movs	r2, #1
	movs	r3, #21
	bl 0x0200a0ec
	ldr	r0, [r5, #0]
	ldr	r1, [r6, #0]
	movs	r2, #2
	movs	r3, #0
	bl 0x0200a0ec
	b.n	.L_02002278
.L_0200226a:
	ldr	r3, [pc, #448]
	ldr	r0, [r5, #0]
	ldr	r1, [r3, #0]
	movs	r2, #3
	movs	r3, #0
	bl 0x0200a0ec
.L_02002278:
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #16]
	movs	r2, #3
	movs	r3, #255
	bl 0x0200a0ec
	mov	r2, r8
	ldr	r0, [r2, #8]
	ldr	r1, [r2, #16]
	movs	r3, #255
	movs	r2, #3
	bl 0x0200a0ec
	movs	r0, #132
	lsls	r0, r0, #2
	bl 0x0200bdcc
	cmp	r0, #0
	bne.n	.L_02002352
	mov	r3, fp
	cmp	r3, #12
	bne.n	.L_02002352
	mov	r2, r9
	cmp	r2, #26
	bne.n	.L_02002352
	movs	r3, #0
	mov	sl, r3
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	ldr	r2, [sp, #12]
	cmp	r2, #12
	bne.n	.L_020022dc
	ldr	r3, [sp, #8]
	cmp	r3, #26
	bne.n	.L_020022dc
	mov	r2, fp
	mov	r3, r9
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r2, #1
	movs	r0, #7
	movs	r1, #2
	movs	r3, #1
	bl 0x0200be24
	movs	r2, #1
	mov	sl, r2
.L_020022dc:
	adds	r6, r7, #0
	movs	r3, #3
	adds	r6, #85
	strb	r3, [r6, #0]
	movs	r0, #10
	bl 0x0200bd9c
	movs	r0, #161
	bl 0x0200bffc
	movs	r0, #10
	bl 0x0200bd9c
	movs	r5, #0
	movs	r0, #132
	strb	r5, [r6, #0]
	lsls	r0, r0, #2
	bl 0x0200bdd4
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_0200234e
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #16]
	movs	r2, #3
	movs	r3, #0
	bl 0x0200a0ec
	mov	r2, r8
	ldr	r0, [r2, #8]
	ldr	r1, [r2, #16]
	movs	r3, #0
	movs	r2, #3
	bl 0x0200a0ec
	movs	r3, #12
	movs	r2, #26
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r2, #1
	movs	r0, #7
	movs	r1, #3
	bl 0x0200be24
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200bdd4
	adds	r2, r7, #0
	adds	r2, #98
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r2, r8
	adds	r2, #98
	strb	r3, [r2, #0]
.L_0200234e:
	bl 0x0200be6c
.L_02002352:
	movs	r0, #137
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bdcc
	cmp	r0, #0
	bne.n	.L_02002412
	ldr	r3, [sp, #12]
	cmp	r3, #7
	bne.n	.L_02002412
	ldr	r2, [sp, #8]
	cmp	r2, #29
	bne.n	.L_02002412
	movs	r3, #0
	mov	sl, r3
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	mov	r2, fp
	cmp	r2, #7
	bne.n	.L_0200239a
	mov	r3, r9
	cmp	r3, #29
	bne.n	.L_0200239a
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r2, #1
	movs	r0, #7
	movs	r1, #2
	movs	r3, #1
	bl 0x0200be24
	movs	r2, #1
	mov	sl, r2
.L_0200239a:
	mov	r6, r8
	movs	r3, #3
	adds	r6, #85
	strb	r3, [r6, #0]
	movs	r0, #10
	bl 0x0200bd9c
	movs	r0, #161
	bl 0x0200bffc
	movs	r0, #10
	bl 0x0200bd9c
	movs	r0, #137
	movs	r5, #0
	lsls	r0, r0, #1
	strb	r5, [r6, #0]
	adds	r0, #255
	bl 0x0200bdd4
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_0200240e
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #16]
	movs	r2, #3
	movs	r3, #0
	bl 0x0200a0ec
	mov	r2, r8
	ldr	r0, [r2, #8]
	ldr	r1, [r2, #16]
	movs	r3, #0
	movs	r2, #3
	bl 0x0200a0ec
	movs	r3, #7
	movs	r2, #29
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #1
	movs	r2, #1
	movs	r0, #7
	movs	r1, #3
	bl 0x0200be24
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200bdd4
	adds	r2, r7, #0
	adds	r2, #98
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r2, r8
	adds	r2, #98
	strb	r3, [r2, #0]
.L_0200240e:
	bl 0x0200be6c
.L_02002412:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0200d19c
	.4byte 0x0200d194
	.4byte 0x0200d198
	.2byte 0xd190
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #12
	bl 0x0200be84
	adds	r5, r0, #0
	movs	r0, #13
	bl 0x0200be84
	ldr	r3, [r5, #8]
	ldr	r7, [pc, #108]
	ldr	r6, [pc, #112]
	str	r3, [r7, #0]
	ldr	r2, [pc, #112]
	ldr	r3, [r5, #16]
	mov	r8, r2
	str	r3, [r6, #0]
	ldr	r3, [r0, #8]
	str	r3, [r2, #0]
	ldr	r3, [pc, #104]
	mov	sl, r3
	ldr	r3, [r0, #16]
	mov	r2, sl
	str	r3, [r2, #0]
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	ldr	r3, [r7, #0]
	asrs	r3, r3, #20
	cmp	r3, #7
	bne.n	.L_020024a4
	ldr	r3, [r6, #0]
	asrs	r3, r3, #20
	cmp	r3, #29
	bne.n	.L_020024a4
	mov	r2, r8
	ldr	r3, [r2, #0]
	asrs	r3, r3, #20
	cmp	r3, #7
	bne.n	.L_020024a4
	mov	r2, sl
	ldr	r3, [r2, #0]
	asrs	r3, r3, #20
	cmp	r3, #28
	bne.n	.L_020024a4
	adds	r2, r5, #0
	adds	r2, #98
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r0, [r7, #0]
	ldr	r1, [r6, #0]
	movs	r2, #2
	bl 0x0200a0ec
.L_020024a4:
	bl 0x0200bfe4
	bl 0x0200a1d0
	bl 0x0200be6c
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0200d19c
	.4byte 0x0200d194
	.4byte 0x0200d198
	.2byte 0xd190
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	movs	r3, #5
	movs	r2, #28
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #2
	movs	r2, #1
	movs	r3, #2
	movs	r0, #5
	bl 0x0200be24
	movs	r0, #138
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bdd4
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r3, [pc, #64]
	movs	r2, #64
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02002532
	ldr	r5, [pc, #56]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200be84
	adds	r6, r0, #0
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	adds	r6, #85
	movs	r3, #0
	strb	r3, [r6, #0]
	movs	r2, #140
	ldr	r0, [r5, #0]
	movs	r1, #72
	lsls	r2, r2, #1
	bl 0x0200bec4
	movs	r3, #3
	strb	r3, [r6, #0]
	bl 0x0200be6c
.L_02002532:
	pop	{r5, r6, pc}
	.4byte 0x03001150
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #88]
	movs	r2, #128
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02002596
	ldr	r3, [pc, #80]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r7, r3, r2
	ldr	r0, [r7, #0]
	ldr	r5, [pc, #72]
	bl 0x0200be84
	ldrh	r3, [r5, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #246
	adds	r3, r3, r2
	movs	r2, #200
	lsls	r3, r3, #16
	lsls	r2, r2, #16
	adds	r6, r0, #0
	cmp	r3, r2
	bls.n	.L_02002596
	bl 0x0200be64
	adds	r5, r6, #0
	movs	r0, #0
	bl 0x0200bfa4
	adds	r5, #85
	movs	r3, #0
	strb	r3, [r5, #0]
	movs	r2, #148
	ldr	r0, [r7, #0]
	movs	r1, #72
	lsls	r2, r2, #1
	bl 0x0200bec4
	movs	r3, #3
	strb	r3, [r5, #0]
	bl 0x0200be6c
.L_02002596:
	pop	{r5, r6, r7, pc}
	.4byte 0x03001150
	.4byte 0x02000240
	.2byte 0x234c
	.2byte 0x0200
	push	{lr}
	movs	r0, #134
	lsls	r0, r0, #2
	bl 0x0200bdd4
	pop	{pc}
	push	{lr}
	movs	r0, #134
	lsls	r0, r0, #2
	bl 0x0200bddc
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #65
	movs	r2, #10
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #65
	movs	r1, #0
	movs	r2, #4
	movs	r3, #4
	bl 0x0200bfec
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #71
	movs	r2, #10
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #65
	movs	r1, #0
	movs	r2, #2
	movs	r3, #2
	bl 0x0200bfec
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #68
	movs	r2, #15
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #65
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bfec
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #75
	movs	r2, #20
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #65
	movs	r1, #0
	movs	r2, #2
	movs	r3, #3
	bl 0x0200bfec
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #78
	movs	r2, #22
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #65
	movs	r1, #0
	movs	r2, #4
	movs	r3, #1
	bl 0x0200bfec
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #79
	movs	r2, #23
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #65
	movs	r1, #0
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bfec
	add	sp, #12
	pop	{pc}
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	ldr	r3, [pc, #184]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r7, r3, r2
	ldr	r0, [r7, #0]
	bl 0x0200be84
	adds	r5, r0, #0
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r2, r2
	negs	r0, r0
	negs	r1, r1
	bl 0x0200bf6c
	ldr	r0, [r7, #0]
	movs	r1, #2
	bl 0x0200bef4
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r7, #0]
	lsls	r2, r2, #7
	lsls	r1, r1, #8
	bl 0x0200be8c
	ldr	r0, [r5, #8]
	asrs	r2, r0, #16
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_020026d2
	adds	r3, #15
.L_020026d2:
	asrs	r3, r3, #4
	lsls	r3, r3, #4
	subs	r3, r2, r3
	movs	r1, #8
	subs	r1, r1, r3
	lsls	r1, r1, #16
	adds	r1, r1, r0
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	adds	r0, r5, #0
	bl 0x0200be0c
	adds	r0, r5, #0
	bl 0x0200be14
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r5, #6]
	movs	r3, #170
	lsls	r3, r3, #1
	adds	r6, r6, r3
	movs	r2, #0
	ldrsh	r5, [r6, r2]
	movs	r0, #158
	bl 0x0200bffc
	subs	r5, #1
	ldr	r0, [pc, #56]
	lsls	r5, r5, #3
	adds	r3, r5, #4
	ldrh	r1, [r0, r3]
	adds	r3, r3, r0
	ldrh	r2, [r3, #2]
	ldr	r0, [r0, r5]
	bl 0x0200be1c
	ldr	r1, [pc, #44]
	ldr	r0, [r7, #0]
	bl 0x0200be94
	movs	r0, #12
	bl 0x0200bd9c
	movs	r0, #123
	bl 0x0200bffc
	bl 0x0200bf94
	bl 0x0200bf9c
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	bl 0x0200bf7c
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200c6bc
	.2byte 0xc028
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	bl 0x0200b4ec
	movs	r0, #123
	bl 0x0200bffc
	bl 0x0200bf94
	bl 0x0200bf9c
	movs	r3, #170
	lsls	r3, r3, #1
	adds	r5, r5, r3
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl 0x0200bf7c
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #108]
	ldr	r3, [pc, #156]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	bl 0x0200be84
	adds	r5, r0, #0
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r2, r2
	negs	r0, r0
	negs	r1, r1
	bl 0x0200bf6c
	ldr	r0, [r6, #0]
	movs	r1, #2
	bl 0x0200bef4
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r6, #0]
	lsls	r2, r2, #7
	lsls	r1, r1, #8
	bl 0x0200be8c
	ldr	r0, [r5, #8]
	asrs	r2, r0, #16
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_020027d6
	adds	r3, #15
.L_020027d6:
	asrs	r3, r3, #4
	lsls	r3, r3, #4
	subs	r3, r2, r3
	movs	r1, #8
	subs	r1, r1, r3
	lsls	r1, r1, #16
	ldr	r2, [r5, #12]
	adds	r1, r1, r0
	ldr	r3, [r5, #16]
	adds	r0, r5, #0
	bl 0x0200be0c
	adds	r0, r5, #0
	bl 0x0200be14
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r5, #6]
	ldr	r1, [pc, #48]
	ldr	r0, [r6, #0]
	bl 0x0200be94
	movs	r0, #12
	bl 0x0200bd9c
	movs	r0, #123
	bl 0x0200bffc
	bl 0x0200bf94
	bl 0x0200bf9c
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200bf7c
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xc028
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	ldr	r5, [pc, #364]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r5, r5, r1
	ldr	r0, [r5, #0]
	bl 0x0200be84
	mov	sl, r0
	movs	r0, #9
	bl 0x0200be84
	ldr	r1, [r5, #0]
	mov	r8, r0
	movs	r0, #9
	bl 0x0200beec
	movs	r2, #130
	ldr	r1, [pc, #332]
	lsls	r2, r2, #18
	movs	r0, #10
	bl 0x0200bedc
	movs	r0, #1
	bl 0x0200bd9c
	movs	r3, #192
	mov	r2, r8
	lsls	r3, r3, #8
	strh	r3, [r2, #6]
	ldr	r3, [pc, #312]
	mov	r1, sl
	str	r3, [r1, #108]
	ldr	r0, [r5, #0]
	bl 0x0200be84
	movs	r1, #15
	bl 0x0200bf1c
	movs	r1, #3
	movs	r0, #9
	bl 0x0200bef4
	movs	r0, #1
	bl 0x0200bd9c
	movs	r2, #192
	lsls	r2, r2, #18
	movs	r3, #214
	mov	r9, r2
	lsls	r3, r3, #1
	ldr	r2, [r2, #108]
	mov	fp, r3
	mov	r1, fp
	adds	r3, #85
	str	r3, [r2, r1]
	bl 0x0200bf8c
	bl 0x0200bf9c
	movs	r1, #153
	movs	r2, #152
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #9
	adds	r1, #153
	adds	r2, #204
	bl 0x0200be8c
	movs	r1, #153
	movs	r2, #152
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r1, #153
	adds	r2, #204
	movs	r0, #10
	bl 0x0200be8c
	movs	r0, #10
	bl 0x0200be84
	ldr	r3, [pc, #216]
	mov	r2, r8
	str	r3, [r0, #108]
	movs	r1, #140
	str	r3, [r2, #108]
	movs	r2, #234
	movs	r0, #10
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200bebc
	movs	r1, #148
	movs	r2, #226
	lsls	r2, r2, #1
	movs	r0, #9
	lsls	r1, r1, #1
	bl 0x0200beb4
	movs	r0, #10
	movs	r1, #1
	bl 0x0200bef4
	movs	r6, #0
	mov	r3, sl
	mov	r1, r8
	str	r6, [r3, #108]
	movs	r0, #10
	str	r6, [r1, #108]
	bl 0x0200be84
	movs	r1, #9
	str	r6, [r0, #108]
	ldr	r0, [r5, #0]
	bl 0x0200bfac
	ldr	r0, [pc, #148]
	bl 0x0200bf24
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	movs	r0, #9
	movs	r1, #1
	bl 0x0200bef4
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #124]
	bl 0x0200be8c
	ldr	r0, [r5, #0]
	bl 0x0200be84
	movs	r1, #0
	bl 0x0200bf1c
	ldr	r0, [r5, #0]
	movs	r1, #6
	movs	r2, #0
	bl 0x0200bf04
	movs	r1, #156
	movs	r2, #220
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200beb4
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	bl 0x0200bf3c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	bl 0x0200bfb4
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200bf3c
	mov	r3, r9
	ldr	r2, [r3, #108]
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r1, fp
	str	r3, [r2, r1]
	bl 0x0200b4d8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x01110000
	.4byte 0x02009279
	.4byte 0x02008df9
	.4byte 0x000021f4
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #116]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl 0x0200be84
	adds	r6, r0, #0
	movs	r0, #9
	bl 0x0200be84
	ldr	r1, [r5, #0]
	adds	r7, r0, #0
	movs	r0, #9
	bl 0x0200beec
	movs	r1, #240
	movs	r2, #135
	lsls	r2, r2, #18
	lsls	r1, r1, #16
	movs	r0, #10
	bl 0x0200bedc
	movs	r0, #1
	bl 0x0200bd9c
	ldr	r3, [pc, #60]
	mov	r8, r3
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r7, #6]
	ldr	r3, [pc, #56]
	ldr	r0, [r5, #0]
	str	r3, [r6, #108]
	bl 0x0200be84
	movs	r1, #15
	bl 0x0200bf1c
	movs	r1, #3
	movs	r0, #9
	bl 0x0200bef4
	movs	r0, #1
	bl 0x0200bd9c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #85
	str	r2, [r3, #0]
	bl 0x0200bf8c
	b.n	.L_02002a48
	.4byte 0x00000000
	.4byte 0x02000240
	.2byte 0x9279
	.2byte 0x0200
.L_02002a48:
	bl 0x0200bf9c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #9
	adds	r1, #204
	adds	r2, #102
	bl 0x0200be8c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r1, #204
	adds	r2, #102
	movs	r0, #10
	bl 0x0200be8c
	movs	r0, #10
	bl 0x0200be84
	ldr	r3, [pc, #212]
	adds	r5, r7, #0
	str	r3, [r0, #108]
	adds	r5, #99
	str	r3, [r7, #108]
	mov	r3, r8
	ldr	r1, [pc, #204]
	adds	r0, r7, #0
	strb	r3, [r5, #0]
	bl 0x0200bdec
	ldr	r1, [pc, #196]
	movs	r0, #10
	bl 0x0200be94
.L_02002a94:
	movs	r0, #1
	bl 0x0200bd9c
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02002a94
	movs	r5, #0
	str	r5, [r6, #108]
	movs	r0, #10
	str	r5, [r7, #108]
	bl 0x0200be84
	movs	r1, #192
	str	r5, [r0, #108]
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x0200bf3c
	ldr	r5, [pc, #156]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	movs	r1, #9
	bl 0x0200bfac
	ldr	r0, [pc, #144]
	bl 0x0200bf24
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	movs	r0, #9
	movs	r1, #1
	bl 0x0200bef4
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	ldr	r0, [r5, #0]
	ldr	r1, [pc, #120]
	bl 0x0200be8c
	ldr	r0, [r5, #0]
	bl 0x0200be84
	movs	r1, #0
	bl 0x0200bf1c
	ldr	r0, [r5, #0]
	movs	r1, #6
	movs	r2, #0
	bl 0x0200bf04
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #230
	ldr	r0, [r5, #0]
	movs	r1, #152
	bl 0x0200beb4
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #7
	bl 0x0200bf3c
	movs	r0, #9
	movs	r1, #0
	bl 0x0200bf2c
	bl 0x0200bfb4
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200bf3c
	bl 0x0200b4d8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	subs	r2, #172
	str	r2, [r3, #0]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02008df9
	.4byte 0x0200c508
	.4byte 0x0200c554
	.4byte 0x02000240
	.4byte 0x000021ef
	.2byte 0x9999
	.2byte 0x0001
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	movs	r2, #128
	adds	r3, r3, r1
	lsls	r2, r2, #1
	str	r2, [r3, #0]
	ldr	r3, [pc, #68]
	adds	r2, #224
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_02002b8e
	bl 0x0200ac38
	b.n	.L_02002bbc
.L_02002b8e:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02002b9a
	bl 0x0200af1c
	b.n	.L_02002bbc
.L_02002b9a:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_02002ba6
	bl 0x0200b0b4
	b.n	.L_02002bbc
.L_02002ba6:
	ldr	r3, [pc, #40]
	cmp	r2, r3
	bne.n	.L_02002bb2
	bl 0x0200b164
	b.n	.L_02002bbc
.L_02002bb2:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_02002bbc
	bl 0x0200b29c
.L_02002bbc:
	movs	r0, #0
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000083
	.4byte 0x00000084
	.4byte 0x00000085
	.4byte 0x00000086
	.2byte 0x0087
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #88]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200be84
	ldr	r3, [r0, #12]
	movs	r2, #128
	lsls	r2, r2, #12
	cmp	r3, r2
	ble.n	.L_02002c12
	movs	r0, #9
	bl 0x0200be84
	adds	r0, #89
	ldrb	r2, [r0, #0]
	movs	r5, #247
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x0200be84
	adds	r0, #89
	ldrb	r3, [r0, #0]
	ands	r5, r3
	b.n	.L_02002c2e
.L_02002c12:
	movs	r0, #9
	bl 0x0200be84
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r5, #8
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x0200be84
	adds	r0, #89
	ldrb	r3, [r0, #0]
	orrs	r5, r3
.L_02002c2e:
	strb	r5, [r0, #0]
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #32]
	ldr	r3, [pc, #192]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #2
	bne.n	.L_02002c58
	movs	r0, #48
	adds	r0, #255
	bl 0x0200bddc
.L_02002c58:
	ldrb	r2, [r5, #23]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r5, #23]
	movs	r0, #11
	bl 0x0200be84
	movs	r1, #0
	bl 0x0200be34
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #102
	movs	r0, #10
	adds	r1, #204
	bl 0x0200be8c
	movs	r0, #10
	bl 0x0200be84
	adds	r0, #89
	ldrb	r2, [r0, #0]
	movs	r3, #128
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #161
	bl 0x0200bdcc
	cmp	r0, #0
	beq.n	.L_02002cee
	movs	r0, #234
	movs	r1, #140
	movs	r3, #210
	adds	r0, #255
	lsls	r1, r1, #17
	movs	r2, #0
	lsls	r3, r3, #18
	bl 0x0200bdf4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02002ce0
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	movs	r1, #227
	strb	r3, [r2, #0]
	lsls	r1, r1, #1
	bl 0x0200be3c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200be34
	adds	r2, r5, #0
	adds	r2, #89
	movs	r3, #9
	strb	r3, [r2, #0]
	adds	r2, #3
	movs	r3, #1
	strb	r3, [r2, #0]
.L_02002ce0:
	movs	r0, #11
	bl 0x0200be84
	movs	r3, #208
	lsls	r3, r3, #8
	strh	r3, [r0, #6]
	b.n	.L_02002cf6
.L_02002cee:
	movs	r0, #11
	movs	r1, #5
	bl 0x0200bef4
.L_02002cf6:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #12]
	bl 0x0200bda4
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xabd9
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200be84
	movs	r1, #0
	adds	r5, r0, #0
	bl 0x0200be34
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r3, r5, #0
	movs	r1, #0
	adds	r3, #85
	strb	r1, [r3, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #128
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #152
	lsls	r3, r3, #6
	adds	r3, #102
	str	r3, [r5, #52]
	movs	r3, #152
	lsls	r3, r3, #7
	adds	r3, #204
	str	r3, [r5, #48]
	pop	{r5, pc}
	push	{r5, r6, lr}
	adds	r6, r2, #0
	adds	r5, r1, #0
	bl 0x0200be84
	adds	r3, r0, #0
	adds	r3, #100
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r5, r5, #5
	adds	r5, r5, r3
	adds	r3, r0, #0
	adds	r3, #102
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r6, r6, #5
	adds	r6, r6, r3
	lsls	r5, r5, #16
	lsls	r6, r6, #16
	ldr	r2, [r0, #12]
	adds	r1, r5, #0
	adds	r3, r6, #0
	bl 0x0200be0c
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [pc, #312]
	ldr	r6, [r3, #108]
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r1, r7, #2
	adds	r3, r6, r2
	mov	r8, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02002daa
	movs	r0, #134
	lsls	r0, r0, #2
	bl 0x0200bdcc
	movs	r5, #11
	cmp	r0, #0
	beq.n	.L_02002dce
.L_02002daa:
	movs	r5, #11
.L_02002dac:
	adds	r0, r5, #0
	bl 0x0200be84
	adds	r5, #1
	adds	r0, #91
	movs	r3, #1
	strb	r3, [r0, #0]
	cmp	r5, #18
	ble.n	.L_02002dac
	b.n	.L_02002f14
.L_02002dc0:
	adds	r0, r5, #0
	bl 0x0200be84
	movs	r3, #0
	adds	r0, #91
	strb	r3, [r0, #0]
	adds	r5, #1
.L_02002dce:
	cmp	r5, #18
	ble.n	.L_02002dc0
	ldr	r3, [pc, #240]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200be84
	ldr	r1, [pc, #228]
	ldr	r3, [r0, #12]
	cmp	r3, r1
	bge.n	.L_02002e0e
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	movs	r5, #11
.L_02002df0:
	adds	r0, r5, #0
	bl 0x0200be84
	adds	r5, #1
	adds	r0, #91
	movs	r3, #1
	strb	r3, [r0, #0]
	cmp	r5, #18
	ble.n	.L_02002df0
	movs	r3, #181
	lsls	r3, r3, #1
	adds	r2, r6, r3
	movs	r3, #200
	strh	r3, [r2, #0]
	b.n	.L_02002f14
.L_02002e0e:
	mov	r1, r8
	ldrh	r3, [r1, #0]
	movs	r1, #171
	adds	r2, r3, #1
	mov	r3, r8
	strh	r2, [r3, #0]
	lsls	r1, r1, #1
	adds	r3, r6, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_02002e28
	b.n	.L_02002f14
.L_02002e28:
	lsls	r3, r2, #16
	cmp	r3, #0
	beq.n	.L_02002f14
.L_02002e2e:
	ldrh	r3, [r7, #0]
	cmp	r3, #100
	beq.n	.L_02002eae
	cmp	r3, #100
	bgt.n	.L_02002e3e
	cmp	r3, #0
	beq.n	.L_02002e48
	b.n	.L_02002ee6
.L_02002e3e:
	cmp	r3, #130
	beq.n	.L_02002ecc
	cmp	r3, #220
	beq.n	.L_02002eae
	b.n	.L_02002ee6
.L_02002e48:
	movs	r5, #11
.L_02002e4a:
	adds	r0, r5, #0
	movs	r1, #2
	adds	r5, #1
	bl 0x0200bef4
	cmp	r5, #18
	ble.n	.L_02002e4a
	movs	r5, #1
	negs	r5, r5
	movs	r0, #11
	movs	r1, #0
	movs	r2, #1
	bl 0x0200ad48
	movs	r0, #12
	movs	r1, #0
	adds	r2, r5, #0
	bl 0x0200ad48
	movs	r0, #13
	movs	r1, #0
	movs	r2, #1
	bl 0x0200ad48
	movs	r0, #14
	movs	r1, #0
	movs	r2, #1
	bl 0x0200ad48
	movs	r0, #15
	adds	r1, r5, #0
	movs	r2, #0
	bl 0x0200ad48
	movs	r0, #16
	movs	r1, #0
	movs	r2, #1
	bl 0x0200ad48
	movs	r0, #17
	movs	r1, #1
	movs	r2, #0
	bl 0x0200ad48
	movs	r0, #18
	movs	r1, #0
	adds	r2, r5, #0
	bl 0x0200ad48
	b.n	.L_02002ee6
.L_02002eae:
	movs	r5, #11
.L_02002eb0:
	adds	r0, r5, #0
	movs	r1, #1
	adds	r5, #1
	bl 0x0200bef4
	cmp	r5, #18
	ble.n	.L_02002eb0
	b.n	.L_02002ee6
	.4byte 0x0200234c
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff4
.L_02002ecc:
	.2byte 0x250b
.L_02002ece:
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200bef4
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #0
	adds	r5, #1
	bl 0x0200ad48
	cmp	r5, #18
	ble.n	.L_02002ece
.L_02002ee6:
	ldrh	r3, [r7, #0]
	movs	r2, #130
	adds	r3, #1
	strh	r3, [r7, #0]
	lsls	r2, r2, #17
	lsls	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_02002efa
	ldr	r3, [pc, #24]
	strh	r3, [r7, #0]
.L_02002efa:
	mov	r1, r8
	ldrh	r3, [r1, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r3, r3, r2
	strh	r3, [r1, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_02002e2e
	b.n	.L_02002f14
	.2byte 0x0000
	.2byte 0x0000
.L_02002f14:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r5, [pc, #380]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x0200be84
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
	movs	r2, #241
	lsls	r2, r2, #1
	strb	r3, [r0, #0]
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #2
	bne.n	.L_02002f4e
	movs	r0, #48
	adds	r0, #255
	bl 0x0200bddc
.L_02002f4e:
	movs	r5, #11
.L_02002f50:
	adds	r0, r5, #0
	adds	r5, #1
	bl 0x0200ad0c
	cmp	r5, #18
	ble.n	.L_02002f50
	ldr	r5, [pc, #320]
	movs	r0, #10
	adds	r0, #255
	adds	r6, r5, #2
	bl 0x0200bdcc
	cmp	r0, #0
	bne.n	.L_02002f70
	strh	r0, [r5, #0]
	strh	r0, [r6, #0]
.L_02002f70:
	movs	r1, #144
	ldr	r0, [pc, #304]
	lsls	r1, r1, #3
	bl 0x0200bda4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #44
	bl 0x0200bdcc
	cmp	r0, #0
	beq.n	.L_02002fe8
	movs	r0, #10
	bl 0x0200be84
	movs	r1, #176
	movs	r2, #168
	adds	r5, r0, #0
	lsls	r2, r2, #16
	movs	r0, #10
	lsls	r1, r1, #16
	bl 0x0200bedc
	movs	r0, #10
	movs	r1, #4
	bl 0x0200bef4
	adds	r0, r5, #0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	adds	r3, r5, #0
	movs	r1, #0
	adds	r3, #85
	strb	r1, [r3, #0]
	ldr	r3, [pc, #236]
	movs	r0, #9
	str	r3, [r5, #12]
	str	r3, [r5, #20]
	movs	r3, #10
	movs	r5, #9
	str	r3, [sp, #4]
	movs	r1, #1
	movs	r2, #4
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200be24
	movs	r3, #74
	str	r3, [sp, #4]
	movs	r0, #9
	movs	r1, #64
	movs	r2, #4
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200be24
	b.n	.L_02002fee
.L_02002fe8:
	movs	r0, #10
	bl 0x0200b684
.L_02002fee:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #162
	bl 0x0200bdcc
	cmp	r0, #0
	bne.n	.L_0200301e
	movs	r0, #9
	movs	r1, #19
	bl 0x0200bfcc
	movs	r0, #19
	movs	r1, #0
	bl 0x0200bef4
	movs	r0, #19
	bl 0x0200be84
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	b.n	.L_02003096
.L_0200301e:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bdcc
	cmp	r0, #0
	bne.n	.L_02003096
	movs	r0, #9
	bl 0x0200be84
	movs	r3, #160
	lsls	r3, r3, #7
	strh	r3, [r0, #6]
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r5, #128
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r1, #234
	movs	r3, #192
	movs	r2, #204
	lsls	r3, r3, #6
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	movs	r0, #19
	bl 0x0200bee4
	movs	r0, #19
	bl 0x0200be84
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r1, #128
	orrs	r5, r3
	movs	r2, #128
	strb	r5, [r0, #0]
	lsls	r1, r1, #10
	movs	r0, #9
	lsls	r2, r2, #9
	bl 0x0200be8c
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #19
	ldr	r1, [pc, #52]
	bl 0x0200be8c
	ldr	r5, [pc, #48]
	movs	r0, #9
	adds	r1, r5, #0
	bl 0x0200be94
	movs	r0, #19
	adds	r1, r5, #0
	bl 0x0200be94
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200bdd4
.L_02003096:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200234c
	.4byte 0x0200ad7d
	.4byte 0xfff80000
	.4byte 0x00019999
	.2byte 0xc274
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #168]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L_020030ce
	movs	r0, #48
	adds	r0, #255
	bl 0x0200bddc
.L_020030ce:
	movs	r0, #10
	bl 0x0200be84
	movs	r1, #0
	bl 0x0200be34
	movs	r0, #10
	movs	r1, #4
	bl 0x0200bef4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #163
	bl 0x0200bdcc
	cmp	r0, #0
	beq.n	.L_02003138
	movs	r0, #234
	movs	r1, #208
	movs	r2, #192
	movs	r3, #160
	adds	r0, #255
	lsls	r1, r1, #15
	lsls	r2, r2, #13
	lsls	r3, r3, #16
	bl 0x0200bdf4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0200312a
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	movs	r1, #198
	strb	r3, [r2, #0]
	adds	r1, #255
	bl 0x0200be3c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200be34
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #1
	strb	r3, [r2, #0]
.L_0200312a:
	movs	r1, #208
	movs	r2, #204
	movs	r0, #10
	lsls	r1, r1, #15
	lsls	r2, r2, #16
	bl 0x0200bedc
.L_02003138:
	movs	r1, #1
	movs	r0, #11
	bl 0x0200bf44
	movs	r0, #11
	bl 0x0200be84
	movs	r1, #15
	bl 0x0200bf1c
	movs	r0, #11
	bl 0x0200be84
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #10
	ldr	r5, [r3, #32]
	sub	sp, #8
	bl 0x0200be84
	movs	r2, #200
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r6, #0
	adds	r2, #4
	str	r6, [r3, #0]
	adds	r3, r5, r2
	str	r6, [r3, #0]
	ldr	r3, [pc, #272]
	adds	r2, #78
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r7, r0, #0
	cmp	r3, #11
	bne.n	.L_0200319c
	movs	r0, #48
	adds	r0, #255
	bl 0x0200bddc
.L_0200319c:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bdcc
	cmp	r0, #0
	bne.n	.L_020031b6
	adds	r3, r7, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	movs	r0, #10
	movs	r1, #1
	bl 0x0200bf44
.L_020031b6:
	movs	r1, #0
	movs	r2, #0
	movs	r0, #11
	bl 0x0200bedc
	movs	r0, #12
	bl 0x0200bf4c
	movs	r0, #13
	bl 0x0200bf4c
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200bdcc
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02003216
	movs	r0, #12
	bl 0x0200be84
	adds	r5, r0, #0
	movs	r0, #13
	bl 0x0200be84
	adds	r3, r5, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r3, r0, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r0, #98
	movs	r3, #1
	adds	r5, #98
	strb	r3, [r5, #0]
	movs	r2, #29
	strb	r3, [r0, #0]
	movs	r3, #7
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #7
	movs	r1, #3
	movs	r2, #1
	movs	r3, #1
	bl 0x0200be24
	b.n	.L_0200324a
.L_02003216:
	movs	r0, #12
	bl 0x0200be84
	adds	r5, r0, #0
	movs	r0, #13
	bl 0x0200be84
	adds	r3, r5, #0
	adds	r6, r0, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	movs	r2, #3
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	movs	r3, #255
	bl 0x0200a0ec
	ldr	r0, [r6, #8]
	ldr	r1, [r6, #16]
	movs	r2, #3
	movs	r3, #255
	bl 0x0200a0ec
.L_0200324a:
	movs	r0, #14
	bl 0x0200bf4c
	movs	r0, #14
	bl 0x0200be84
	movs	r3, #1
	adds	r1, r0, #0
	adds	r0, #98
	strb	r3, [r0, #0]
	adds	r3, r1, #0
	movs	r2, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	movs	r2, #1
	ldr	r0, [r1, #8]
	movs	r3, #255
	ldr	r1, [r1, #16]
	bl 0x0200a0ec
	movs	r0, #138
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bdcc
	cmp	r0, #0
	beq.n	.L_02003294
	movs	r3, #5
	movs	r2, #28
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #5
	movs	r1, #2
	movs	r2, #1
	movs	r3, #2
	bl 0x0200be24
.L_02003294:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #32]
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	ldrb	r2, [r5, #23]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r5, #23]
	ldr	r3, [pc, #320]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #2
	bne.n	.L_020032d4
	movs	r0, #48
	adds	r0, #255
	bl 0x0200bddc
.L_020032d4:
	bl 0x0200b410
	movs	r1, #12
	movs	r2, #5
	movs	r3, #4
	movs	r0, #3
	bl 0x0200b46c
	movs	r0, #4
	movs	r1, #12
	movs	r2, #5
	movs	r3, #4
	bl 0x0200b46c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #165
	bl 0x0200bdcc
	cmp	r0, #0
	beq.n	.L_02003332
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bdcc
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_02003332
	movs	r1, #140
	movs	r2, #234
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r0, #10
	bl 0x0200bedc
	movs	r0, #10
	bl 0x0200be84
	movs	r1, #148
	adds	r0, #85
	movs	r2, #226
	strb	r5, [r0, #0]
	lsls	r1, r1, #17
	movs	r0, #9
	lsls	r2, r2, #17
	bl 0x0200bedc
.L_02003332:
	movs	r0, #9
	bl 0x0200be84
	movs	r1, #0
	bl 0x0200be34
	movs	r0, #11
	bl 0x0200be84
	movs	r1, #0
	bl 0x0200be34
	movs	r0, #12
	bl 0x0200be84
	movs	r1, #0
	bl 0x0200be34
	movs	r1, #144
	ldr	r0, [pc, #168]
	lsls	r1, r1, #3
	bl 0x0200bda4
	ldr	r3, [pc, #156]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	movs	r2, #128
	subs	r3, #4
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_020033b0
	movs	r0, #140
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200bdd4
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bdcc
	cmp	r0, #0
	bne.n	.L_020033b0
	movs	r3, #128
	movs	r1, #152
	movs	r2, #192
	lsls	r3, r3, #7
	movs	r0, #9
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x0200bee4
	movs	r1, #176
	movs	r0, #10
	lsls	r1, r1, #16
	ldr	r2, [pc, #96]
	bl 0x0200bedc
	movs	r0, #1
	bl 0x0200bd9c
.L_020033b0:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200bdcc
	cmp	r0, #0
	bne.n	.L_020033ee
	ldr	r3, [pc, #64]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L_020033d2
	bl 0x0200a830
	b.n	.L_020033fa
.L_020033d2:
	cmp	r3, #4
	bne.n	.L_020033dc
	bl 0x0200a9c4
	b.n	.L_020033fa
.L_020033dc:
	cmp	r3, #5
	beq.n	.L_020033fa
	bl 0x0200bf8c
	bl 0x0200bf9c
	bl 0x0200b4d8
	b.n	.L_020033fa
.L_020033ee:
	bl 0x0200bf8c
	bl 0x0200bf9c
	bl 0x0200b4d8
.L_020033fa:
	bl 0x0200be6c
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x02009c4d
	.2byte 0x0000
	.2byte 0x0309
	movs	r0, #0
	bx	lr
	push	{r5, lr}
	movs	r0, #112
	movs	r1, #180
	bl 0x0200bdc4
	movs	r5, #0
	adds	r4, r0, #0
.L_0200341e:
	movs	r3, #0
	str	r3, [r4, #0]
	strh	r3, [r4, #4]
	strh	r3, [r4, #6]
	strh	r3, [r4, #8]
	strh	r3, [r4, #10]
	movs	r2, #0
.L_0200342c:
	lsls	r3, r2, #16
	lsrs	r3, r3, #16
	ldr	r1, [pc, #40]
	lsls	r2, r3, #1
	adds	r2, #12
	strh	r1, [r4, r2]
	adds	r3, #1
	movs	r1, #128
	lsls	r3, r3, #16
	lsls	r1, r1, #13
	asrs	r2, r3, #16
	cmp	r3, r1
	bne.n	.L_0200342c
	movs	r2, #128
	lsls	r3, r5, #16
	lsls	r2, r2, #9
	movs	r1, #128
	adds	r3, r3, r2
	lsls	r1, r1, #11
	adds	r4, #44
	asrs	r5, r3, #16
	cmp	r3, r1
	bne.n	.L_0200341e
	b.n	.L_02003460
	.2byte 0x0000
	.2byte 0x0000
.L_02003460:
	adds	r2, r0, #0
	adds	r2, #176
	movs	r3, #0
	strh	r3, [r2, #0]
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	lsls	r2, r2, #16
	lsls	r3, r3, #16
	asrs	r7, r2, #16
	asrs	r2, r3, #16
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #112]
	lsls	r1, r1, #16
	adds	r5, r4, #0
	adds	r5, #176
	asrs	r6, r1, #16
	ldrh	r1, [r5, #0]
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	cmp	r1, #3
	bls.n	.L_02003494
	movs	r0, #1
	negs	r0, r0
	b.n	.L_020034d6
.L_02003494:
	movs	r3, #44
	muls	r1, r3
	lsls	r0, r0, #16
	lsls	r3, r6, #16
	lsrs	r3, r3, #16
	lsrs	r0, r0, #12
	adds	r0, r0, r3
	movs	r3, #160
	lsls	r3, r3, #19
	lsls	r0, r0, #1
	adds	r1, r4, r1
	adds	r0, r0, r3
	movs	r3, #0
	strh	r3, [r1, #4]
	strh	r3, [r1, #6]
	lsls	r2, r2, #16
	movs	r4, #128
	movs	r3, #128
	lsrs	r2, r2, #16
	lsls	r4, r4, #24
	lsls	r3, r3, #19
	strh	r2, [r1, #10]
	str	r0, [r1, #0]
	strh	r7, [r1, #8]
	adds	r3, #212
	adds	r1, #12
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldrh	r3, [r5, #0]
	movs	r0, #0
	adds	r3, #1
	strh	r3, [r5, #0]
.L_020034d6:
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #8]
	bl 0x0200bda4
	pop	{pc}
	.2byte 0x0000
	.2byte 0xb4fd
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200bdac
	pop	{pc}
	.2byte 0x0000
	.2byte 0xb4fd
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #192
	lsls	r1, r1, #18
	ldr	r2, [r1, #108]
	movs	r0, #179
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	sub	sp, #32
	cmp	r3, #0
	bne.n	.L_02003610
	movs	r0, #173
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	bne.n	.L_02003610
	movs	r0, #175
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02003610
	ldr	r1, [r1, #112]
	movs	r0, #0
	mov	r9, r1
	mov	r3, r9
	adds	r3, #176
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	lsls	r1, r3, #16
	mov	r8, r0
	mov	fp, r1
	cmp	r8, r3
	bcs.n	.L_02003610
.L_02003556:
	movs	r3, #44
	mov	r2, r8
	muls	r2, r3
	mov	r0, r9
	adds	r3, r2, #0
	adds	r5, r0, r3
	ldrh	r2, [r5, #6]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L_020035f4
	movs	r1, #4
	ldrsh	r6, [r5, r1]
	movs	r3, #10
	ldrsh	r2, [r5, r3]
	ldr	r0, [r5, #0]
	subs	r3, r2, r6
	lsls	r7, r2, #16
	lsls	r3, r3, #24
	lsrs	r1, r7, #16
	mov	sl, r0
	adds	r4, r5, #0
	lsrs	r0, r3, #24
	mov	ip, r1
	adds	r4, #12
	cmp	r0, ip
	bcs.n	.L_0200359e
.L_0200358a:
	ldrh	r3, [r4, #0]
	lsls	r2, r0, #1
	mov	r1, sp
	strh	r3, [r1, r2]
	adds	r3, r0, #1
	lsls	r3, r3, #24
	lsrs	r0, r3, #24
	adds	r4, #2
	cmp	r0, ip
	bcc.n	.L_0200358a
.L_0200359e:
	lsls	r6, r6, #16
	lsrs	r2, r7, #16
	mov	lr, r6
	lsrs	r6, r6, #16
	movs	r0, #0
	subs	r3, r2, r6
	mov	ip, r2
	cmp	r0, r3
	bge.n	.L_020035c8
.L_020035b0:
	ldrh	r3, [r4, #0]
	mov	r1, sp
	lsls	r2, r0, #1
	strh	r3, [r1, r2]
	adds	r3, r0, #1
	lsls	r3, r3, #24
	mov	r1, ip
	lsrs	r0, r3, #24
	subs	r3, r1, r6
	adds	r4, #2
	cmp	r0, r3
	blt.n	.L_020035b0
.L_020035c8:
	movs	r2, #128
	movs	r3, #128
	lsrs	r4, r7, #16
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	mov	r0, sp
	mov	r1, sl
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #128
	lsls	r3, r3, #9
	add	r3, lr
	asrs	r6, r3, #16
	lsrs	r3, r3, #16
	cmp	r3, r4
	bcc.n	.L_020035ee
	movs	r6, #0
.L_020035ee:
	ldrh	r3, [r5, #8]
	strh	r6, [r5, #4]
	b.n	.L_020035fc
.L_020035f4:
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r3, r2, r0
.L_020035fc:
	strh	r3, [r5, #6]
	mov	r3, r8
	adds	r3, #1
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	r1, fp
	mov	r8, r3
	lsrs	r3, r1, #16
	cmp	r8, r3
	bcc.n	.L_02003556
.L_02003610:
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200be64
	movs	r0, #0
	bl 0x0200bfa4
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #38
	bl 0x0200bffc
	ldr	r5, [pc, #64]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl 0x0200be84
	movs	r1, #0
	bl 0x0200be34
	movs	r1, #18
	ldr	r0, [r5, #0]
	bl 0x0200bef4
	movs	r0, #40
	bl 0x0200be5c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200bf94
	bl 0x0200bf9c
	movs	r0, #3
	bl 0x0200bf7c
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x0200bf84
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	sub	sp, #8
	mov	r8, r3
	bl 0x0200be84
	ldr	r3, [r0, #80]
	ldr	r5, [pc, #232]
	ldr	r3, [r3, #40]
	movs	r1, #0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r4, r3, #16
	ldrh	r3, [r5, r1]
	lsrs	r2, r4, #16
	cmp	r2, r3
	beq.n	.L_020036ca
.L_020036b0:
	movs	r2, #128
	lsls	r3, r1, #16
	lsls	r2, r2, #9
	adds	r3, r3, r2
	lsrs	r2, r3, #16
	asrs	r1, r3, #16
	cmp	r2, #5
	bhi.n	.L_020036ca
	lsls	r3, r2, #1
	ldrh	r3, [r5, r3]
	lsrs	r2, r4, #16
	cmp	r2, r3
	bne.n	.L_020036b0
.L_020036ca:
	lsls	r3, r1, #16
	lsrs	r2, r3, #16
	cmp	r2, #6
	bne.n	.L_020036d6
	movs	r0, #0
	b.n	.L_0200377e
.L_020036d6:
	ldr	r6, [pc, #180]
	lsls	r2, r2, #2
	ldrsb	r4, [r6, r2]
	adds	r1, r4, #0
	cmp	r4, #0
	bge.n	.L_020036e4
	negs	r1, r4
.L_020036e4:
	adds	r3, r2, #2
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	bge.n	.L_020036ee
	negs	r3, r3
.L_020036ee:
	adds	r3, r1, r3
	asrs	r7, r3, #4
	adds	r3, r2, #1
	ldrsb	r1, [r6, r3]
	adds	r5, r1, #0
	cmp	r1, #0
	bge.n	.L_020036fe
	negs	r5, r1
.L_020036fe:
	adds	r3, r2, #3
	ldrsb	r2, [r6, r3]
	cmp	r2, #0
	bge.n	.L_02003708
	negs	r2, r2
.L_02003708:
	adds	r5, r5, r2
	mov	sl, r5
	ldr	r6, [r0, #8]
	mov	r3, sl
	ldr	r5, [r0, #16]
	asrs	r3, r3, #4
	mov	sl, r3
	lsls	r3, r4, #16
	adds	r6, r6, r3
	lsls	r3, r1, #16
	adds	r5, r5, r3
	movs	r3, #164
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r3, [r3, #0]
	asrs	r6, r6, #20
	asrs	r1, r3, #20
	movs	r3, #166
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r3, [r3, #0]
	lsls	r2, r1, #16
	asrs	r3, r3, #20
	lsls	r3, r3, #16
	asrs	r5, r5, #20
	lsrs	r2, r2, #16
	lsrs	r3, r3, #16
	adds	r2, r6, r2
	adds	r3, r5, r3
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r7, #0
	mov	r3, sl
	bl 0x0200be24
	movs	r3, #255
	mov	r2, sl
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	mov	r8, r3
	movs	r0, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	adds	r3, r7, #0
	bl 0x0200b790
	mov	r2, sl
	mov	r3, r8
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #2
	adds	r1, r6, #0
	adds	r2, r5, #0
	adds	r3, r7, #0
	bl 0x0200b790
	movs	r0, #1
.L_0200377e:
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0200c070
	.2byte 0xc07c
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r3, #0
	ldr	r3, [sp, #12]
	lsls	r2, r2, #7
	mov	ip, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r0, r0, #1
	lsls	r3, r3, #3
	adds	r3, r3, r0
	ldr	r0, [r4, r3]
	adds	r1, r1, r2
	lsls	r1, r1, #2
	adds	r0, r0, r1
	movs	r1, #0
	ldr	r6, [sp, #16]
	cmp	r1, ip
	bcs.n	.L_020037d6
.L_020037bc:
	lsls	r3, r1, #9
	movs	r2, #0
	adds	r3, r0, r3
	cmp	r2, r5
	bcs.n	.L_020037d0
.L_020037c6:
	adds	r2, #1
	strb	r6, [r3, #2]
	adds	r3, #4
	cmp	r2, r5
	bcc.n	.L_020037c6
.L_020037d0:
	adds	r1, #1
	cmp	r1, ip
	bcc.n	.L_020037bc
.L_020037d6:
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	adds	r5, r6, #0
	sub	sp, #40
	adds	r1, r6, #0
	adds	r5, #12
	add	r0, sp, #24
	adds	r1, #16
	adds	r2, r5, #0
	bl 0x0200b940
	adds	r4, r0, #0
	cmp	r4, #0
	bne.n	.L_02003802
	b.n	.L_02003922
.L_02003802:
	ldr	r5, [r5, #0]
	ldr	r0, [pc, #300]
	str	r5, [sp, #20]
	lsls	r1, r5, #2
	ldrsb	r2, [r0, r1]
	cmp	r2, #0
	bge.n	.L_02003812
	negs	r2, r2
.L_02003812:
	adds	r3, r1, #2
	ldrsb	r3, [r0, r3]
	cmp	r3, #0
	bge.n	.L_0200381c
	negs	r3, r3
.L_0200381c:
	adds	r3, r2, r3
	asrs	r3, r3, #4
	str	r3, [sp, #16]
	adds	r3, r1, #1
	ldrsb	r2, [r0, r3]
	cmp	r2, #0
	bge.n	.L_0200382c
	negs	r2, r2
.L_0200382c:
	adds	r3, r1, #3
	ldrsb	r3, [r0, r3]
	cmp	r3, #0
	bge.n	.L_02003836
	negs	r3, r3
.L_02003836:
	adds	r3, r2, r3
	asrs	r3, r3, #4
	str	r3, [sp, #12]
	ldr	r3, [sp, #24]
	ldr	r2, [pc, #248]
	ldr	r1, [pc, #248]
	lsls	r3, r3, #2
	ldr	r3, [r2, r3]
	mov	r9, r1
	mov	r2, r9
	ands	r2, r3
	lsls	r3, r3, #16
	mov	sl, r3
	movs	r3, #0
	str	r3, [r6, #20]
	mov	fp, r3
	adds	r3, r4, #0
	adds	r3, #34
	str	r3, [sp, #8]
	ldr	r1, [sp, #8]
	movs	r3, #2
	strb	r3, [r1, #0]
	mov	r9, r2
	ldr	r3, [r4, #8]
	add	r3, r9
	str	r3, [r6, #0]
	ldr	r3, [r4, #16]
	add	r3, sl
	str	r3, [r6, #8]
	ldr	r3, [r4, #12]
	str	r3, [sp, #32]
.L_02003874:
	ldr	r3, [sp, #20]
	ldr	r2, [pc, #188]
	lsls	r3, r3, #2
	str	r3, [sp, #4]
	adds	r3, #1
	ldrsb	r2, [r2, r3]
	ldr	r3, [r6, #8]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	ldr	r2, [sp, #12]
	movs	r1, #0
	mov	r8, r1
	str	r3, [sp, #36]
	cmp	r8, r2
	bge.n	.L_020038e2
.L_02003892:
	ldr	r3, [pc, #160]
	ldr	r1, [sp, #4]
	add	r5, sp, #28
	ldrsb	r2, [r3, r1]
	ldr	r3, [r6, #0]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [sp, #16]
	movs	r7, #0
	cmp	r7, r2
	bge.n	.L_020038cc
.L_020038aa:
	adds	r0, r4, #0
	add	r1, sp, #28
	str	r4, [sp, #0]
	bl 0x0200be2c
	ldr	r4, [sp, #0]
	cmp	r0, #2
	beq.n	.L_020038f4
	ldr	r3, [r5, #0]
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r3, r3, r1
.L_020038c2:
	str	r3, [r5, #0]
	ldr	r2, [sp, #16]
	adds	r7, #1
	cmp	r7, r2
	blt.n	.L_020038aa
.L_020038cc:
	add	r2, sp, #28
	ldr	r3, [r2, #8]
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r2, #8]
	ldr	r3, [sp, #12]
	movs	r2, #1
	add	r8, r2
	cmp	r8, r3
	blt.n	.L_02003892
.L_020038e2:
	ldr	r3, [r6, #0]
	movs	r1, #1
	add	r3, r9
	str	r3, [r6, #0]
	ldr	r3, [r6, #8]
	add	fp, r1
	add	r3, sl
	str	r3, [r6, #8]
	b.n	.L_02003874
.L_020038f4:
	ldr	r2, [sp, #8]
	movs	r3, #0
	strb	r3, [r2, #0]
	mov	r3, fp
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_02003924
	mov	r1, r9
	ldr	r3, [r4, #8]
	mov	r2, fp
	muls	r2, r1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	movs	r0, #1
	ldr	r3, [r4, #12]
	str	r3, [r6, #4]
	mov	r3, sl
	mov	r2, fp
	muls	r2, r3
	ldr	r3, [r4, #16]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	b.n	.L_02003924
.L_02003922:
	movs	r0, #0
.L_02003924:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200c07c
	.4byte 0x0200c094
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r0, [sp, #8]
	str	r1, [sp, #4]
	str	r2, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	ldr	r3, [pc, #236]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl 0x0200be84
	adds	r7, r0, #0
	ldrh	r3, [r7, #6]
	ldr	r1, [sp, #8]
	lsrs	r3, r3, #12
	str	r3, [r1, #0]
	movs	r2, #8
	adds	r5, #52
	mov	fp, r2
	mov	lr, r5
.L_0200397c:
	mov	r3, lr
	ldr	r6, [r3, #0]
	movs	r5, #0
.L_02003982:
	ldr	r3, [r6, #80]
	ldr	r2, [pc, #200]
	ldr	r3, [r3, #40]
	movs	r0, #0
	ldrsh	r1, [r3, r0]
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	cmp	r1, r3
	bne.n	.L_02003a26
	ldr	r0, [sp, #8]
	movs	r2, #10
	ldrsh	r1, [r7, r2]
	ldr	r3, [r0, #0]
	ldr	r2, [pc, #180]
	lsls	r3, r3, #2
	ldr	r3, [r2, r3]
	ldr	r4, [pc, #180]
	asrs	r2, r3, #16
	adds	r1, r1, r2
	asrs	r1, r1, #4
	mov	r9, r1
	movs	r1, #18
	ldrsh	r2, [r7, r1]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	adds	r2, r2, r3
	asrs	r2, r2, #4
	mov	r8, r2
	movs	r2, #10
	ldrsh	r0, [r6, r2]
	lsls	r2, r5, #2
	ldrsb	r3, [r4, r2]
	adds	r3, r0, r3
	asrs	r3, r3, #4
	mov	sl, r3
	movs	r3, #18
	ldrsh	r1, [r6, r3]
	adds	r3, r2, #1
	ldrsb	r3, [r4, r3]
	adds	r3, r1, r3
	asrs	r3, r3, #4
	mov	ip, r3
	adds	r3, r2, #2
	ldrsb	r3, [r4, r3]
	adds	r2, #3
	adds	r0, r0, r3
	ldrsb	r3, [r4, r2]
	asrs	r0, r0, #4
	adds	r1, r1, r3
	asrs	r1, r1, #4
	cmp	sl, r9
	bgt.n	.L_02003a26
	cmp	r9, r0
	bge.n	.L_02003a26
	cmp	ip, r8
	bgt.n	.L_02003a26
	cmp	r8, r1
	bge.n	.L_02003a26
	ldr	r0, [sp, #0]
	movs	r3, #1
	ands	r3, r5
	str	r5, [r0, #0]
	cmp	r3, #0
	beq.n	.L_02003a14
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	sl, r3
	beq.n	.L_02003a26
	ldr	r2, [sp, #4]
	mov	r1, fp
	str	r1, [r2, #0]
	adds	r0, r6, #0
	b.n	.L_02003a3c
.L_02003a14:
	ldr	r3, [r7, #16]
	asrs	r3, r3, #20
	cmp	ip, r3
	beq.n	.L_02003a26
	ldr	r0, [sp, #4]
	mov	r3, fp
	str	r3, [r0, #0]
	adds	r0, r6, #0
	b.n	.L_02003a3c
.L_02003a26:
	adds	r5, #1
	cmp	r5, #5
	bls.n	.L_02003982
	movs	r2, #1
	add	fp, r2
	movs	r1, #4
	mov	r3, fp
	add	lr, r1
	cmp	r3, #63
	bls.n	.L_0200397c
	movs	r0, #0
.L_02003a3c:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200c070
	.4byte 0x0200c094
	.4byte 0x0200c07c
	.2byte 0xb084
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #56
	str	r0, [sp, #88]
	str	r1, [sp, #92]
	str	r2, [sp, #96]
	str	r3, [sp, #100]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r0, #133
	str	r3, [sp, #28]
	ldr	r3, [pc, #632]
	lsls	r0, r0, #2
	adds	r0, r0, r3
	mov	sl, r0
	ldr	r0, [r0, #0]
	bl 0x0200be84
	mov	r8, r0
	ldr	r0, [sp, #104]
	bl 0x0200be84
	mov	r3, r8
	ldr	r3, [r3, #48]
	mov	r4, r8
	str	r3, [sp, #20]
	adds	r6, r0, #0
	ldr	r4, [r4, #52]
	mov	r0, sp
	adds	r0, #32
	str	r0, [sp, #12]
	str	r4, [sp, #16]
	ldr	r2, [sp, #100]
	ldr	r3, [r6, #8]
	movs	r1, #0
	str	r3, [r0, #0]
	mov	r9, r1
	ldr	r3, [r6, #16]
	mov	r1, sp
	adds	r1, #44
	str	r3, [r0, #8]
	ldr	r5, [pc, #576]
	str	r1, [sp, #8]
	lsls	r7, r2, #2
	ldrsb	r1, [r5, r7]
	ldr	r3, [r6, #8]
	lsls	r2, r1, #16
	adds	r3, r3, r2
	ldr	r2, [sp, #8]
	asrs	r3, r3, #20
	str	r3, [r2, #0]
	mov	lr, r3
	adds	r3, r7, #1
	ldrsb	r4, [r5, r3]
	ldr	r3, [r6, #16]
	ldr	r0, [sp, #8]
	lsls	r2, r4, #16
	adds	r3, r3, r2
	asrs	r3, r3, #20
	str	r3, [r0, #8]
	adds	r0, r1, #0
	mov	ip, r3
	cmp	r0, #0
	bge.n	.L_02003aec
	negs	r0, r0
.L_02003aec:
	adds	r3, r7, #2
	ldrsb	r1, [r5, r3]
	cmp	r1, #0
	bge.n	.L_02003af6
	negs	r1, r1
.L_02003af6:
	adds	r3, r0, r1
	asrs	r3, r3, #4
	adds	r1, r4, #0
	str	r3, [sp, #24]
	cmp	r1, #0
	bge.n	.L_02003b04
	negs	r1, r1
.L_02003b04:
	adds	r3, r7, #3
	ldrsb	r2, [r5, r3]
	cmp	r2, #0
	bge.n	.L_02003b0e
	negs	r2, r2
.L_02003b0e:
	adds	r3, r1, r2
	asrs	r3, r3, #4
	str	r3, [sp, #0]
	mov	fp, r3
	movs	r3, #0
	str	r3, [sp, #4]
	mov	r1, lr
	mov	r2, ip
	ldr	r3, [sp, #24]
	movs	r0, #0
	bl 0x0200b790
	mov	r1, sl
	movs	r2, #200
	ldr	r0, [r1, #0]
	lsls	r2, r2, #5
	movs	r1, #128
	lsls	r1, r1, #8
	adds	r2, #153
	bl 0x0200be8c
	mov	r2, sl
	ldr	r0, [r2, #0]
	movs	r1, #8
	bl 0x0200bef4
	movs	r0, #15
	bl 0x0200bd9c
	ldr	r4, [sp, #12]
	ldr	r1, [sp, #88]
	ldr	r3, [r4, #0]
	ldr	r2, [sp, #96]
	subs	r1, r1, r3
	ldr	r3, [r4, #8]
	asrs	r1, r1, #17
	subs	r2, r2, r3
	mov	r3, sl
	asrs	r2, r2, #17
	ldr	r0, [r3, #0]
.L_02003b5e:
	bl 0x0200becc
	mov	r4, sl
	ldr	r0, [r4, #0]
	bl 0x0200be84
	ldr	r3, [pc, #408]
	str	r3, [r0, #108]
	movs	r0, #4
	bl 0x0200bd9c
	movs	r1, #2
	adds	r0, r6, #0
	bl 0x0200bde4
	movs	r0, #239
	bl 0x0200bffc
	movs	r2, #200
	movs	r1, #128
	lsls	r2, r2, #5
	ldr	r0, [sp, #104]
	lsls	r1, r1, #8
	adds	r2, #153
	bl 0x0200be8c
	adds	r0, r6, #0
	ldr	r1, [sp, #88]
	ldr	r2, [sp, #92]
	ldr	r3, [sp, #96]
	bl 0x0200be0c
	ldr	r3, [pc, #348]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r5, r3, r0
	ldr	r0, [r5, #0]
	bl 0x0200bed4
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200bef4
	movs	r1, #152
	movs	r2, #200
	lsls	r1, r1, #7
	lsls	r2, r2, #5
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #153
	bl 0x0200be8c
	ldr	r2, [pc, #320]
	mov	r1, r9
	lsls	r3, r1, #2
	ldr	r2, [r2, r3]
	ldr	r0, [r5, #0]
	lsls	r2, r2, #16
	asrs	r1, r2, #31
	asrs	r2, r2, #17
	bl 0x0200becc
	ldr	r3, [sp, #108]
	cmp	r3, #0
	beq.n	0x0200bbe4
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x6828
	bl 0x0200bed4
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200bef4
	mov	r3, r8
	movs	r2, #0
	str	r2, [r3, #108]
	ldr	r4, [sp, #20]
	movs	r5, #255
	str	r4, [r3, #48]
	ldr	r0, [sp, #16]
	str	r0, [r3, #52]
	adds	r0, r6, #0
	bl 0x0200be14
	movs	r0, #149
	lsls	r0, r0, #1
	bl 0x0200bffc
	movs	r0, #213
	bl 0x0200bffc
	ldr	r2, [r6, #12]
	ldr	r1, [sp, #88]
	ldr	r3, [sp, #96]
	adds	r0, r6, #0
	bl 0x0200be04
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200bde4
	ldr	r1, [pc, #212]
	ldr	r0, [sp, #88]
	ldrsb	r3, [r1, r7]
	adds	r2, r7, #1
	lsls	r3, r3, #16
	adds	r0, r0, r3
	ldrsb	r3, [r1, r2]
	mov	sl, r1
	ldr	r1, [sp, #96]
	lsls	r3, r3, #16
	adds	r1, r1, r3
	ldr	r4, [sp, #28]
	asrs	r0, r0, #20
	asrs	r1, r1, #20
	str	r0, [sp, #88]
	str	r1, [sp, #96]
	mov	r9, r2
	movs	r2, #164
	lsls	r2, r2, #1
	adds	r3, r4, r2
	ldr	r3, [r3, #0]
	adds	r2, #4
	asrs	r3, r3, #20
	mov	r8, r3
	adds	r3, r4, r2
	ldr	r6, [r3, #0]
	mov	r4, r8
	asrs	r6, r6, #20
	adds	r3, r4, r0
	adds	r2, r6, r1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	mov	r3, fp
	ldr	r2, [sp, #24]
	bl 0x0200be24
	mov	r0, fp
	ldr	r1, [sp, #88]
	ldr	r2, [sp, #96]
	str	r0, [sp, #0]
	ldr	r3, [sp, #24]
	movs	r0, #0
	str	r5, [sp, #4]
	bl 0x0200b790
	mov	r3, fp
	ldr	r1, [sp, #88]
	ldr	r2, [sp, #96]
	str	r3, [sp, #0]
	movs	r0, #2
	ldr	r3, [sp, #24]
	str	r5, [sp, #4]
	bl 0x0200b790
	ldr	r0, [sp, #12]
	mov	r4, sl
	ldrsb	r3, [r4, r7]
	ldr	r1, [r0, #0]
	ldr	r2, [sp, #8]
	lsls	r3, r3, #16
	adds	r1, r1, r3
	asrs	r1, r1, #20
	str	r1, [r2, #0]
	mov	r3, r9
	ldrsb	r2, [r4, r3]
	ldr	r3, [r0, #8]
	ldr	r4, [sp, #8]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	asrs	r3, r3, #20
	str	r3, [r4, #8]
	add	r8, r1
	adds	r6, r6, r3
	str	r1, [sp, #0]
	str	r3, [sp, #4]
	ldr	r2, [sp, #24]
	mov	r0, r8
	adds	r1, r6, #0
	mov	r3, fp
	bl 0x0200be24
	ldr	r0, [sp, #8]
	mov	r3, fp
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #8]
	movs	r4, #0
	str	r3, [sp, #0]
	movs	r0, #2
	ldr	r3, [sp, #24]
	str	r4, [sp, #4]
	bl 0x0200b790
	bl 0x0200bfd4
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r3}
	add	sp, #16
	bx	r3
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200c07c
	.4byte 0x0200bd0d
	.2byte 0xc094
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldrh	r3, [r5, #6]
	movs	r2, #12
	lsrs	r1, r3, #12
	adds	r3, r1, #2
	ands	r3, r2
	lsls	r1, r3, #12
	ldr	r3, [r5, #8]
	sub	sp, #12
	mov	r6, sp
	str	r3, [r6, #0]
	ldr	r3, [r5, #12]
	movs	r0, #128
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	lsls	r0, r0, #13
	adds	r2, r6, #0
	str	r3, [r6, #8]
	bl 0x0200bdbc
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200bfdc
	cmp	r0, #0
	beq.n	.L_02003d68
	movs	r4, #0
.L_02003d44:
	ldr	r3, [r0, #80]
	ldr	r3, [r3, #40]
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	ldr	r2, [pc, #64]
	lsls	r3, r4, #1
	ldrh	r3, [r2, r3]
	cmp	r1, r3
	beq.n	.L_02003d8c
	adds	r4, #1
	cmp	r4, #5
	bls.n	.L_02003d44
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	adds	r0, r5, #0
	bl 0x0200be04
.L_02003d68:
	ldr	r3, [r5, #8]
	adds	r0, r5, #0
	str	r3, [r6, #0]
	ldr	r3, [r5, #12]
	adds	r1, r6, #0
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	str	r3, [r6, #8]
	bl 0x0200be2c
	cmp	r0, #0
	ble.n	.L_02003d8c
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	adds	r0, r5, #0
	bl 0x0200be04
.L_02003d8c:
	add	sp, #12
	pop	{r5, r6, pc}
	.4byte 0x0200c070
	.irp EntryTarget, 0x03000508, 0x080000c1, 0x080000d1, 0x080000d9, 0x08000119, 0x08000129, 0x08000149, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020091, 0x080200a9, 0x080200c1, 0x080200c9, 0x080200e9, 0x08020149, 0x08020151, 0x08020171, 0x080201e9, 0x08020211, 0x08020219, 0x08020301, 0x08038041, 0x080ad039, 0x080ad041, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8061, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80b1, 0x080c80b9, 0x080c80c1, 0x080c80c9, 0x080c80d1, 0x080c80d9, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8101, 0x080c8111, 0x080c8119, 0x080c8129, 0x080c8139, 0x080c8151, 0x080c8159, 0x080c8171, 0x080c8181, 0x080c81a1, 0x080c81d1, 0x080c81d9, 0x080c8201, 0x080c8209, 0x080c8211, 0x080c8219, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8279, 0x080c82e1, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c84e9, 0x080c84f1, 0x080c84f9, 0x080c8501, 0x080c8601, 0x080c8691, 0x080c8739, 0x080c8749, 0x080c8761, 0x08108011, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000011
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
	.4byte 0x01030102
	.4byte 0x01260125
	.4byte 0x014b014c
	.4byte 0x0820f8e0
	.4byte 0x2008e0f8
	.4byte 0x0020f0e0
	.4byte 0x2008e0f8
	.4byte 0x0820f8e0
	.4byte 0x2008e0f8
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
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00006666
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00400000
	.4byte 0x80010000
	.4byte 0x00000004
	.4byte 0x01600000
	.4byte 0x00400000
	.4byte 0x01500000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01700000
	.4byte 0x00400000
	.4byte 0x01500000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00400000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00400000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01700000
	.4byte 0x00400000
	.4byte 0x01300000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01600000
	.4byte 0x00400000
	.4byte 0x01300000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00400000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00400000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00ea0000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01140000
	.4byte 0x00000000
	.4byte 0x00de0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01340000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000003
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xfff00000
	.4byte 0x00000001
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000005
	.4byte 0xffe00000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000005
	.4byte 0x00200000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x03580000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000003
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00100000
	.4byte 0x00000001
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000005
	.4byte 0x00200000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x00000001
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000005
	.4byte 0xffe00000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x03580000
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
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02ae0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x03000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x03000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02ae0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x03000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x03090000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x03000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02ae0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x03000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02ae0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x03a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000050
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x03a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x000a005c
	.4byte 0x00020001
	.4byte 0x005c0006
	.4byte 0x00010008
	.4byte 0x00060002
	.4byte 0xffffffff
	.4byte 0x0200c6ba
	.4byte 0x00400040
	.4byte 0x0200c6a4
	.4byte 0x000a0052
	.4byte 0xffff0000
	.4byte 0x000000d0
	.4byte 0x40000320
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000083
	.4byte 0x0011c002
	.4byte 0x002090b7
	.4byte 0x00000084
	.4byte 0x0011d002
	.4byte 0x00205039
	.4byte 0x00301084
	.4byte 0x00000085
	.4byte 0x0011e002
	.4byte 0x00250002
	.4byte 0x0030700b
	.4byte 0x00451002
	.4byte 0x00000086
	.4byte 0x00a1f002
	.4byte 0x00b080b7
	.4byte 0x00000087
	.4byte 0x00120002
	.4byte 0x0020609c
	.4byte 0x00304087
	.4byte 0x00403087
	.4byte 0x00501088
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0049
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00018000
	.4byte 0xffff004d
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x0002a000
	.4byte 0xffff004d
	.4byte 0x0200c640
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x03a80000
	.4byte 0x00010000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x03580000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0047
	.4byte 0x0200c0d4
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00004000
	.4byte 0xffff018f
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0102
	.4byte 0x00000007
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x008c0000
	.4byte 0x00024000
	.4byte 0xffff0192
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0192
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0xffff0192
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0xffff0192
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0xffff0192
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff0192
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff0192
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff0192
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff0190
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00003000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00004000
	.4byte 0xffff0049
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00012000
	.4byte 0xffff0050
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00018000
	.4byte 0xffff012f
	.4byte 0x00000007
	.4byte 0x01460000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0001c000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00014000
	.4byte 0xffff0195
	.4byte 0x0200c198
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0xffff0120
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff0120
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff004b
	.4byte 0x00000002
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff00a2
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00004000
	.4byte 0xffff00a3
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00ca0000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x010a0000
	.4byte 0x00000000
	.4byte 0x00b80000
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
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200a781
	.4byte 0x00000602
	.4byte 0xffff0010
	.4byte 0x0200b67d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000218c
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02009bf5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000218f
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000021d0
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002190
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002192
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002193
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x02008e39
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200a67d
	.4byte 0x00004602
	.4byte 0xffff000a
	.4byte 0x0200a08d
	.4byte 0x00004602
	.4byte 0xffff000b
	.4byte 0x0200b67d
	.4byte 0x00004602
	.4byte 0x0a2c001e
	.4byte 0x020080d9
	.4byte 0x00000002
	.4byte 0x0aa20014
	.4byte 0x02009db1
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte 0x0200a4f1
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x0200a53d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002196
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008211
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x0200830d
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte 0x0200b621
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002199
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x020082ad
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x02008381
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x0200840d
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000021f8
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x0200a5a5
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x0200a5b1
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
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000219a
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000219b
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000021c5
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000219f
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000021a0
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x020086f5
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x0200a0b5
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x0200a0d1
	.4byte 0x50008905
	.4byte 0xffff000a
	.4byte 0x0200a5bd
	.4byte 0x50008905
	.4byte 0xffff000b
	.4byte 0x0200a5dd
	.4byte 0x50008905
	.4byte 0xffff000c
	.4byte 0x0200a5fd
	.4byte 0x50008905
	.4byte 0xffff000d
	.4byte 0x0200a61d
	.4byte 0x50008905
	.4byte 0xffff000e
	.4byte 0x0200a63d
	.4byte 0x50008905
	.4byte 0xffff000f
	.4byte 0x0200a65d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x0000ce01
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00004602
	.4byte 0x02120015
	.4byte 0x0200a431
	.4byte 0x00000008
	.4byte 0x02120000
	.4byte 0x0200a145
	.4byte 0x10008c15
	.4byte 0x0212000c
	.4byte 0x0200a145
	.4byte 0x10008c15
	.4byte 0x0212000d
	.4byte 0x0200a145
	.4byte 0x00000009
	.4byte 0x02120000
	.4byte 0x0200a1d1
	.4byte 0x00008c15
	.4byte 0x0212000c
	.4byte 0x0200a1d1
	.4byte 0x00008c15
	.4byte 0x0212000d
	.4byte 0x0200a1d1
	.4byte 0x00001815
	.4byte 0x0213000e
	.4byte 0x0200a4c9
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008441
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020084f5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008521
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000021ab
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000021ac
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x02008569
	.4byte 0x00000002
	.4byte 0x02150014
	.4byte 0x02009f75
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200a74d
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x0200a74d
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000021ad
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000021af
	.4byte 0x00000000
	.4byte 0x0aa50009
	.4byte 0x000021d7
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000021ea
	.4byte 0x00008d15
	.4byte 0x02170009
	.4byte 0x020094e1
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x02009a69
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02009c39
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02009c39
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x0200a0b5
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte 0x0200a0d1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
