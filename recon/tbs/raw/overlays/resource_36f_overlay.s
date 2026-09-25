.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_020004be, 0x020004be
	.set sub_020005c0, 0x020005c0
	.set sub_020005d2, 0x020005d2
	.set sub_02000620, 0x02000620
	.set sub_02000656, 0x02000656
	.set sub_02000662, 0x02000662
	.set sub_02000668, 0x02000668
	.set sub_0200066c, 0x0200066c
	.set sub_0200066e, 0x0200066e
	.set sub_0200067c, 0x0200067c
	.set sub_02000680, 0x02000680
	.set sub_02000692, 0x02000692
	.set sub_0200069e, 0x0200069e
	.set sub_020006a4, 0x020006a4
	.set sub_020006ac, 0x020006ac
	.set sub_020006b4, 0x020006b4
	.set sub_020006b8, 0x020006b8
	.set sub_020006c0, 0x020006c0
	.set sub_020006ce, 0x020006ce
	.set sub_020006e2, 0x020006e2
	.set sub_020006ec, 0x020006ec
	.set sub_020006f0, 0x020006f0
	.set sub_020006f4, 0x020006f4
	.set sub_02000706, 0x02000706
	.set sub_0200070a, 0x0200070a
	.set sub_0200070c, 0x0200070c
	.set sub_02000710, 0x02000710
	.set sub_02000716, 0x02000716
	.set sub_02000718, 0x02000718
	.set sub_0200072a, 0x0200072a
	.set sub_02000736, 0x02000736
	.set sub_0200073e, 0x0200073e
	.set sub_02000740, 0x02000740
	.set sub_02000742, 0x02000742
	.set sub_02000744, 0x02000744
	.set sub_02000750, 0x02000750
	.set sub_0200075c, 0x0200075c
	.set sub_02000766, 0x02000766
	.set sub_02000768, 0x02000768
	.set sub_0200076a, 0x0200076a
	.set sub_0200076e, 0x0200076e
	.set sub_02000784, 0x02000784
	.set sub_02000824, 0x02000824
	.set sub_02000848, 0x02000848
	.set sub_0200089a, 0x0200089a
	.set sub_02000942, 0x02000942
	.set sub_0200097c, 0x0200097c
	.set sub_020009e4, 0x020009e4
	.set sub_020009e6, 0x020009e6
	.set sub_020009e8, 0x020009e8
	.set sub_020009f4, 0x020009f4
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/GOMA_IKE/ENTRY.INC"
AlchemyC_02000030:
	.space 0x8
AlchemyC_02000038:
	.space 0x4
AlchemyC_0200003c:
	.space 0x8
AlchemyC_02000044:
	.space 0x8
AlchemyC_0200004c:
	.space 0x8
AlchemyC_02000054:
	.space 0x16c
AlchemyC_020001c0:
	.space 0x78
AlchemyC_02000238:
	.space 0xb0
	push	{r5, r6, r7, lr}
	bl	sub_02000740
	movs	r0, #30
	bl	sub_0200089a
	ldr	r2, [pc, #40]
	ldr	r3, [pc, #36]
	movs	r0, #0
	strh	r3, [r2, #0]
	bl	sub_020004be
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #28]
	bl	sub_02000848
	ldr	r7, [pc, #28]
	ldr	r5, [pc, #28]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r7, #0]
	cmp	r2, #31
	bgt.n	.L_02000350
	b.n	.L_02000330
	.4byte 0x00000000
	.4byte 0x0200868c
	.4byte 0x02008239
	.4byte 0x02002090
	.2byte 0x0208
	.2byte 0x0400
.L_02000330:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r7
	strh	r2, [r7, #0]
	movs	r2, #170
	adds	r3, #4
	lsls	r2, r2, #5
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_02000350:
	strh	r1, [r5, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r7, #0]
	cmp	r2, #31
	bgt.n	.L_0200037a
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r7
	strh	r2, [r7, #0]
.L_0200036a:
	ldr	r2, [pc, #208]
	adds	r3, #4
	stmia	r3!, {r2}
	ldr	r2, [pc, #204]
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0200037a:
	strh	r1, [r5, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r7, #0]
	cmp	r2, #31
	bgt.n	.L_020003a4
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r7
	adds	r3, #4
	strh	r2, [r7, #0]
	movs	r2, #16
	stmia	r3!, {r2}
	ldr	r2, [pc, #168]
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_020003a4:
	strh	r1, [r5, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r7, #0]
	cmp	r2, #31
	bgt.n	.L_020003ce
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r7
	strh	r2, [r7, #0]
	ldr	r2, [pc, #136]
	adds	r3, #4
	stmia	r3!, {r2}
	ldr	r2, [pc, #132]
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_020003ce:
	strh	r1, [r5, #0]
	movs	r0, #120
	bl	sub_0200097c
	movs	r6, #0
.L_020003d8:
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r3, [r7, #0]
	cmp	r3, #31
	bgt.n	.L_02000402
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r2, r2, #2
	adds	r3, #1
	adds	r2, r7, r2
	strh	r3, [r7, #0]
	movs	r3, #16
	adds	r2, #4
	subs	r3, r3, r6
	stmia	r2!, {r3}
	ldr	r3, [pc, #72]
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_02000402:
	strh	r1, [r5, #0]
	movs	r0, #3
	adds	r6, #1
	bl	sub_02000942
	cmp	r6, #16
	ble.n	.L_020003d8
	ldr	r6, [pc, #60]
	movs	r3, #224
	ldr	r1, [r6, #0]
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r5, #228
	movs	r3, #0
	str	r3, [r2, #0]
	lsls	r5, r5, #1
	movs	r3, #1
	str	r3, [r1, r5]
	bl	sub_020009e8
	bl	sub_020009f4
	ldr	r2, [r6, #0]
	movs	r3, #60
	str	r3, [r2, r5]
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x00002fce
	.4byte 0x04000050
	.4byte 0x04000054
	.4byte 0x00001010
	.4byte 0x04000052
	.2byte 0x1ebc
	.2byte 0x0300
	push	{r5, r6, lr}
	movs	r0, #0
	ldr	r5, [pc, #64]
	bl	sub_020009e4
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #52]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #60]
	movs	r3, #0
	strh	r3, [r2, #10]
	adds	r0, r5, #0
	bl	sub_020009e6
	movs	r1, #160
	ldr	r6, [pc, #52]
	adds	r4, r0, #0
	ldr	r3, [pc, #52]
	lsls	r1, r1, #19
	ldr	r2, [pc, #52]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #224
	lsls	r3, r3, #1
	adds	r4, r4, r3
	adds	r0, r4, #0
	ldr	r1, [pc, #40]
	bl	sub_020009e4
	ldr	r3, [pc, #28]
	ldr	r0, [pc, #32]
	ldr	r1, [pc, #36]
	ldr	r2, [pc, #36]
	b.n	.L_020004c0
	.4byte 0x00000681
	.4byte 0x0000001a
	.4byte 0x0400000c
	.4byte 0x03001ad0
	.4byte 0x000001ff
	.4byte 0x040000d4
	.4byte 0x84000070
	.4byte 0x02010000
	.4byte 0x06006800
	.2byte 0x2580
	.2byte 0x8400
.L_020004c0:
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #208
	ldr	r1, [pc, #88]
	lsls	r3, r3, #1
	movs	r4, #0
.L_020004cc:
	movs	r0, #0
.L_020004ce:
	adds	r2, r3, #0
	movs	r5, #128
	lsls	r3, r2, #16
	lsls	r5, r5, #9
	adds	r3, r3, r5
	adds	r0, #1
	strh	r2, [r1, #0]
	asrs	r3, r3, #16
	adds	r1, #2
	cmp	r0, #29
	bls.n	.L_020004ce
	strh	r6, [r1, #0]
	adds	r4, #1
	adds	r1, #2
	strh	r6, [r1, #0]
	adds	r1, #2
	cmp	r4, #19
	bls.n	.L_020004cc
	ldr	r3, [pc, #48]
	movs	r4, #0
	movs	r2, #0
.L_020004f8:
	adds	r4, #1
	strh	r2, [r3, #2]
	strh	r2, [r3, #0]
	adds	r3, #4
	cmp	r4, #3
	bls.n	.L_020004f8
	ldr	r3, [pc, #32]
	ldr	r0, [pc, #28]
	ldr	r1, [pc, #32]
	ldr	r2, [pc, #36]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #32]
	ldr	r2, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #5
	strh	r3, [r2, #20]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x06003000
	.4byte 0x03001ad0
	.4byte 0x040000d4
	.4byte 0x04000010
	.4byte 0x84000004
	.4byte 0x03001e70
	.include "games/THE BROKEN SEAL/SRC/MENU/TITLE/IMPORT.INC"
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x40000000
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.2byte 0xffff
