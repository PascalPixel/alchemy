.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008285, 0x02008039, 0x02008049, 0x02008051, 0x0200827d, 0x02008041, 0x02008319
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa92c
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa95c
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa98c
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xaa14
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #102
	bl 0x0200a7e4
	cmp	r0, #0
	beq.n	.L_02000082
	ldr	r0, [pc, #140]
	bl 0x0200a8ac
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200a8c4
	movs	r0, #10
	movs	r1, #4
	bl 0x0200a914
	b.n	.L_020000f6
.L_02000082:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #101
	bl 0x0200a7e4
	cmp	r0, #0
	beq.n	.L_020000ba
	bl 0x0200a82c
	movs	r0, #0
	bl 0x0200a8fc
	ldr	r0, [pc, #96]
	bl 0x0200a8ac
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200a8c4
	movs	r1, #192
	adds	r0, r5, #0
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a8cc
	bl 0x0200a834
	b.n	.L_020000f6
.L_020000ba:
	ldr	r0, [pc, #68]
	bl 0x0200a8ac
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200a8b4
	bl 0x0200a91c
	movs	r1, #0
	bl 0x0200a83c
	cmp	r0, #0
	bne.n	.L_020000dc
	bl 0x0200831c
	b.n	.L_020000f6
.L_020000dc:
	movs	r0, #20
	bl 0x0200a824
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200a8c4
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a8cc
.L_020000f6:
	pop	{r5, pc}
	.4byte 0x00002780
	.4byte 0x00002767
	.2byte 0x274d
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #102
	adds	r5, r1, #0
	bl 0x0200a7e4
	cmp	r0, #0
	beq.n	.L_0200011a
	ldr	r0, [pc, #48]
	b.n	.L_0200012a
.L_0200011a:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #101
	bl 0x0200a7e4
	cmp	r0, #0
	beq.n	.L_02000138
	ldr	r0, [pc, #32]
.L_0200012a:
	bl 0x0200a8ac
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200a8c4
	b.n	.L_02000146
.L_02000138:
	ldr	r0, [pc, #20]
	bl 0x0200a8ac
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200a8c4
.L_02000146:
	pop	{r5, pc}
	.4byte 0x00002781
	.4byte 0x00002768
	.2byte 0x274f
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r5, [pc, #68]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200a8ac
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200a8b4
	bl 0x0200a91c
	movs	r1, #0
	bl 0x0200a83c
	cmp	r0, #0
	bne.n	.L_02000184
	movs	r0, #10
	bl 0x0200a824
	adds	r0, r5, #1
	bl 0x0200a8ac
	b.n	.L_02000190
.L_02000184:
	movs	r0, #20
	bl 0x0200a824
	adds	r0, r5, #2
	bl 0x0200a8ac
.L_02000190:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200a8c4
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x2783
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r5, [pc, #68]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200a8ac
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200a8b4
	bl 0x0200a91c
	movs	r1, #0
	bl 0x0200a83c
	cmp	r0, #0
	bne.n	.L_020001d0
	movs	r0, #10
	bl 0x0200a824
	adds	r0, r5, #1
	bl 0x0200a8ac
	b.n	.L_020001dc
.L_020001d0:
	movs	r0, #20
	bl 0x0200a824
	adds	r0, r5, #2
	bl 0x0200a8ac
.L_020001dc:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200a8c4
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x278a
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r0, r5, #0
	bl 0x0200a844
	ldrh	r7, [r0, #6]
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #103
	bl 0x0200a7ec
	bl 0x0200a82c
	movs	r0, #0
	bl 0x0200a8fc
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	adds	r0, r5, #0
	bl 0x0200a8dc
	movs	r2, #20
	movs	r1, #4
	adds	r0, r5, #0
	bl 0x0200a8a4
	ldr	r6, [pc, #84]
	adds	r0, r6, #0
	bl 0x0200a8ac
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200a8b4
	bl 0x0200a91c
	movs	r1, #0
	bl 0x0200a83c
	cmp	r0, #0
	bne.n	.L_0200024e
	movs	r0, #10
	bl 0x0200a824
	adds	r0, r6, #1
	bl 0x0200a8ac
	b.n	.L_0200025a
.L_0200024e:
	movs	r0, #20
	bl 0x0200a824
	adds	r0, r6, #2
	bl 0x0200a8ac
.L_0200025a:
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200a8c4
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #0
	bl 0x0200a8cc
	movs	r0, #10
	bl 0x0200a824
	bl 0x0200a834
	pop	{r5, r6, r7, pc}
	.2byte 0x278d
	.2byte 0x0000
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xab94
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	movs	r0, #11
	movs	r1, #1
	bl 0x0200a8d4
	movs	r0, #14
	movs	r1, #2
	bl 0x0200a8d4
	movs	r1, #2
	movs	r0, #10
	bl 0x0200a8d4
	movs	r0, #15
	bl 0x0200a844
	adds	r3, r0, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	str	r5, [r0, #20]
	str	r5, [r0, #12]
	movs	r0, #17
	bl 0x0200a844
	adds	r3, r0, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	str	r5, [r0, #20]
	str	r5, [r0, #12]
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #102
	bl 0x0200a7e4
	cmp	r0, #0
	beq.n	.L_020002fa
	movs	r0, #10
	movs	r1, #100
	movs	r2, #112
	bl 0x0200a864
	movs	r0, #10
	bl 0x0200a844
	movs	r3, #192
	lsls	r3, r3, #6
	strh	r3, [r0, #6]
	movs	r0, #1
	bl 0x0200a7cc
.L_020002fa:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #105
	bl 0x0200a7e4
	cmp	r0, #0
	beq.n	.L_02000312
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a874
.L_02000312:
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	movs	r0, #0
	bx	lr
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #101
	bl 0x0200a7ec
	movs	r0, #7
	bl 0x0200a81c
	bl 0x0200a82c
	movs	r0, #0
	bl 0x0200a8fc
	ldr	r0, [pc, #1016]
	bl 0x0200a8ac
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #192
	movs	r0, #10
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a8cc
	movs	r0, #4
	movs	r1, #112
	movs	r2, #128
	bl 0x0200a864
	movs	r3, #176
	movs	r0, #12
	movs	r1, #16
	movs	r2, #0
	lsls	r3, r3, #8
	bl 0x0200a904
	movs	r2, #16
	movs	r3, #128
	lsls	r3, r3, #8
	movs	r0, #7
	movs	r1, #16
	negs	r2, r2
	bl 0x0200a904
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200a8cc
	movs	r0, #7
	bl 0x0200a86c
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #7
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #10
	bl 0x0200a8dc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #10
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #10
	movs	r1, #6
	movs	r2, #15
	bl 0x0200a88c
	movs	r0, #10
	movs	r1, #6
	movs	r2, #23
	bl 0x0200a88c
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #3
	movs	r0, #7
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #7
	movs	r1, #0
.L_020003f4:
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #45
	movs	r0, #12
	bl 0x0200a8dc
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #12
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r2, #0
	movs	r1, #0
	movs	r0, #4
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #4
	bl 0x0200a884
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #176
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #10
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #10
	bl 0x0200a8dc
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #4
	bl 0x0200a8cc
	movs	r0, #40
	bl 0x0200a824
	movs	r1, #176
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r0, #12
	movs	r1, #3
	bl 0x0200a87c
	movs	r1, #3
	movs	r0, #4
	bl 0x0200a884
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a8cc
	movs	r0, #45
	bl 0x0200a824
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #10
	bl 0x0200a8dc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #10
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #2
	movs	r0, #10
	bl 0x0200a89c
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #10
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #45
	movs	r0, #7
	bl 0x0200a8dc
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a8cc
	movs	r0, #35
	bl 0x0200a824
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a8cc
	movs	r0, #35
	bl 0x0200a824
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a8cc
	movs	r0, #35
	bl 0x0200a824
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a8cc
	movs	r0, #35
	bl 0x0200a824
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #2
	movs	r0, #10
	bl 0x0200a89c
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #6
	adds	r1, #255
	movs	r2, #60
	movs	r0, #10
	bl 0x0200a8dc
	movs	r1, #48
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a90c
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a8cc
	movs	r0, #40
	bl 0x0200a824
	movs	r1, #24
	negs	r1, r1
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a90c
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a8cc
	movs	r0, #40
	bl 0x0200a824
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #7
	bl 0x0200a8dc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #7
	bl 0x0200a8dc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #12
	bl 0x0200a8dc
	movs	r1, #224
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #12
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #10
	adds	r0, #12
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #2
	movs	r0, #7
	bl 0x0200a89c
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #7
	bl 0x0200a8dc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r2, #0
	movs	r1, #0
	movs	r0, #10
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #10
	movs	r1, #4
	bl 0x0200a884
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	b.n	.L_02000738
	.2byte 0x0000
	.2byte 0x2750
	.2byte 0x0000
.L_02000738:
	bl 0x0200a8bc
	movs	r1, #128
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #4
	bl 0x0200a8cc
	movs	r0, #50
	bl 0x0200a824
	movs	r1, #176
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #7
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #7
	movs	r1, #3
	bl 0x0200a894
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200a8e4
	movs	r0, #50
	bl 0x0200a824
	movs	r1, #12
	negs	r1, r1
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a90c
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #10
	bl 0x0200a8dc
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #2
	movs	r0, #7
	bl 0x0200a89c
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #10
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #4
	movs	r0, #7
	bl 0x0200a884
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #10
	movs	r1, #4
	bl 0x0200a884
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #10
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r2, #20
	movs	r0, #10
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #3
	movs	r0, #10
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r2, #10
	movs	r1, #0
	movs	r0, #10
	bl 0x0200a8bc
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #9
	movs	r0, #7
	bl 0x0200a87c
	movs	r0, #40
	bl 0x0200a824
	movs	r1, #224
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #12
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #10
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #1
	movs	r0, #7
	bl 0x0200a87c
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #152
	movs	r2, #152
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	adds	r1, #204
	adds	r2, #102
	movs	r0, #7
	bl 0x0200a84c
	movs	r0, #7
	bl 0x0200a844
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #12
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a90c
	movs	r0, #1
	bl 0x0200a824
	movs	r0, #7
	bl 0x0200a844
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #7
	movs	r1, #3
	bl 0x0200a894
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200a8e4
	movs	r0, #55
	bl 0x0200a824
	movs	r0, #4
	movs	r1, #7
	bl 0x0200a914
	movs	r0, #12
	movs	r1, #7
	bl 0x0200a914
	movs	r0, #10
	movs	r1, #7
	bl 0x0200a914
	movs	r0, #7
	ldr	r1, [pc, #296]
	ldr	r2, [pc, #300]
	bl 0x0200a84c
	movs	r1, #8
	negs	r1, r1
	movs	r2, #88
	movs	r0, #7
	bl 0x0200a90c
	movs	r0, #4
	bl 0x0200a854
	movs	r0, #12
	bl 0x0200a854
	movs	r0, #10
	bl 0x0200a854
	movs	r1, #0
	movs	r2, #0
	movs	r0, #7
	bl 0x0200a874
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #12
	bl 0x0200a8dc
	movs	r1, #128
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #4
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #12
	bl 0x0200a8b4
	movs	r0, #4
	movs	r1, #0
	bl 0x0200a83c
	cmp	r0, #0
	bne.n	.L_020009be
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #10
	adds	r0, #12
	movs	r1, #0
	bl 0x0200a8bc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020009e4
.L_020009be:
	movs	r0, #40
	bl 0x0200a824
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #128
	adds	r3, #1
	lsls	r0, r0, #6
	strh	r3, [r2, #0]
	adds	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
.L_020009e4:
	movs	r1, #3
	movs	r0, #12
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #4
	bl 0x0200a884
	movs	r0, #20
	bl 0x0200a824
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #12
	ldr	r1, [pc, #72]
	bl 0x0200a84c
	movs	r0, #12
	movs	r1, #2
	bl 0x0200a87c
	ldr	r3, [pc, #64]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200a844
	cmp	r0, #0
	beq.n	.L_02000a36
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #12
	bl 0x0200a85c
.L_02000a36:
	movs	r0, #12
	bl 0x0200a86c
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a874
	bl 0x0200a834
	pop	{pc}
	.4byte 0x00023333
	.4byte 0x00011999
	.4byte 0x00013333
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #11
	sub	sp, #48
	bl 0x0200a844
	mov	fp, r0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #101
	bl 0x0200a7e4
	cmp	r0, #0
	bne.n	.L_02000a86
	bl 0x0200927c
.L_02000a86:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #102
	bl 0x0200a7ec
	movs	r0, #10
	bl 0x0200a844
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	bl 0x0200a82c
	movs	r0, #0
	bl 0x0200a8fc
	ldr	r0, [pc, #680]
	bl 0x0200a8ac
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #10
	adds	r1, #204
	adds	r2, #102
	bl 0x0200a84c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #10
	bl 0x0200a8dc
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r0, #4
	movs	r1, #136
	movs	r2, #152
	bl 0x0200a864
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #10
	movs	r1, #4
	bl 0x0200a884
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #10
	bl 0x0200a8dc
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r2, #10
	adds	r0, #10
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #3
	movs	r0, #4
	bl 0x0200a884
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #4
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r0, #4
	movs	r1, #8
	movs	r2, #16
	bl 0x0200a90c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #4
	lsls	r1, r1, #7
	bl 0x0200a8cc
	movs	r0, #10
	movs	r1, #2
	bl 0x0200a89c
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #4
	bl 0x0200a8dc
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #8
	movs	r2, #16
	negs	r2, r2
	negs	r1, r1
	movs	r0, #4
	bl 0x0200a90c
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #10
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r2, #10
	adds	r0, #10
	movs	r1, #0
	bl 0x0200a8bc
	movs	r0, #10
	movs	r1, #4
	bl 0x0200a884
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r2, #10
	adds	r0, #10
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #3
	movs	r0, #10
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r2, #10
	adds	r0, #10
	movs	r1, #0
	bl 0x0200a8bc
	movs	r0, #4
	movs	r1, #3
	bl 0x0200a894
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x0200a8e4
	movs	r0, #50
	bl 0x0200a824
	movs	r1, #12
	movs	r2, #12
	movs	r0, #10
	bl 0x0200a90c
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #10
	bl 0x0200a8dc
	movs	r1, #0
	movs	r0, #10
	bl 0x0200a8b4
	movs	r0, #4
	movs	r1, #0
	bl 0x0200a83c
	cmp	r0, #0
	bne.n	.L_02000c64
	movs	r0, #30
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #0
	movs	r2, #10
	adds	r0, #10
	bl 0x0200a8bc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02000c8a
.L_02000c64:
	movs	r0, #40
	bl 0x0200a824
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #128
	adds	r3, #1
	lsls	r0, r0, #7
	strh	r3, [r2, #0]
	adds	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
.L_02000c8a:
	movs	r1, #3
	movs	r0, #10
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #10
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r2, #10
	adds	r0, #10
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #2
	movs	r0, #10
	bl 0x0200a89c
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #10
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r2, #10
	adds	r0, #10
	movs	r1, #0
	bl 0x0200a8bc
	movs	r0, #4
	movs	r1, #3
	bl 0x0200a894
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x0200a8e4
	movs	r0, #50
	bl 0x0200a824
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #10
	bl 0x0200a8dc
	movs	r1, #0
	movs	r0, #10
	bl 0x0200a8b4
	movs	r0, #4
	movs	r1, #0
	bl 0x0200a83c
	cmp	r0, #0
	bne.n	.L_02000d58
	movs	r0, #30
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #0
	movs	r2, #10
	adds	r0, #10
	bl 0x0200a8bc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02000d7e
	.2byte 0x2769
	.2byte 0x0000
.L_02000d58:
	movs	r0, #40
	bl 0x0200a824
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #128
	adds	r3, #1
	lsls	r0, r0, #7
	strh	r3, [r2, #0]
	adds	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
.L_02000d7e:
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #10
	bl 0x0200a8dc
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	movs	r2, #20
	bl 0x0200a8bc
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #10
	bl 0x0200a8dc
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #89
	movs	r2, #92
	movs	r0, #10
	bl 0x0200a864
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #10
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r2, #10
	adds	r0, #10
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #2
	movs	r0, #10
	bl 0x0200a89c
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #10
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r0, #10
	movs	r1, #4
	bl 0x0200a884
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #10
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #132
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200a8dc
	movs	r1, #3
	movs	r0, #10
	bl 0x0200a87c
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #182
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a924
	movs	r0, #60
	bl 0x0200a824
	movs	r0, #11
	movs	r1, #3
	bl 0x0200a894
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #11
	bl 0x0200a8e4
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #11
	movs	r1, #3
	bl 0x0200a894
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #11
	bl 0x0200a8e4
	movs	r0, #50
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #11
	bl 0x0200a87c
	mov	r1, fp
	adds	r1, #85
	str	r1, [sp, #0]
	movs	r2, #0
	movs	r3, #36
	strb	r2, [r1, #0]
	add	r3, sp
	mov	r1, fp
	mov	sl, r3
	ldr	r3, [r1, #8]
	mov	r1, sl
	str	r3, [r1, #0]
	mov	r1, fp
	ldr	r3, [r1, #12]
	mov	r1, sl
	str	r3, [r1, #4]
	mov	r1, fp
	ldr	r3, [r1, #16]
	mov	r1, sl
	str	r3, [r1, #8]
	add	r3, sp, #24
	mov	r8, r3
	movs	r3, #232
	mov	r1, r8
	lsls	r3, r3, #15
	str	r3, [r1, #0]
	str	r2, [r1, #4]
	movs	r3, #220
	movs	r2, #176
	lsls	r3, r3, #15
	lsls	r2, r2, #8
	str	r3, [r1, #8]
	str	r2, [sp, #4]
	movs	r7, #0
.L_02000ede:
	movs	r1, #192
	lsls	r0, r7, #15
	bl 0x0200a7c4
	adds	r6, r0, #0
	bl 0x0200a7d4
	ldr	r3, [sp, #4]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r3, r3, r1
	lsls	r0, r0, #5
	str	r0, [sp, #8]
	str	r3, [sp, #4]
	add	r2, sp, #12
	mov	r9, r2
	mov	r1, r8
	mov	r2, sl
	ldr	r5, [r2, #0]
	ldr	r3, [r1, #0]
	movs	r1, #192
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	bl 0x0200a7c4
	mov	r3, r9
	adds	r5, r5, r0
	str	r5, [r3, #0]
	adds	r0, r6, #0
	bl 0x0200a7d4
	mov	r2, sl
	mov	r1, r8
	ldr	r5, [r2, #4]
	ldr	r3, [r1, #4]
	adds	r6, r0, #0
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	movs	r1, #192
	bl 0x0200a7c4
	lsls	r3, r6, #3
	subs	r3, r3, r6
	adds	r5, r5, r0
	lsls	r3, r3, #3
	adds	r5, r5, r3
	mov	r3, r9
	str	r5, [r3, #4]
	mov	r2, sl
	mov	r1, r8
	ldr	r5, [r2, #8]
	ldr	r3, [r1, #8]
	movs	r1, #192
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	bl 0x0200a7c4
	mov	r3, r9
	adds	r5, r5, r0
	str	r5, [r3, #8]
	ldr	r1, [sp, #4]
	mov	r2, r9
	ldr	r0, [sp, #8]
	bl 0x0200a7dc
	mov	r1, r9
	ldr	r3, [r1, #0]
	mov	r2, fp
	str	r3, [r2, #8]
	ldr	r3, [r1, #4]
	str	r3, [r2, #12]
	ldr	r3, [r1, #8]
	str	r3, [r2, #16]
	ldr	r1, [sp, #4]
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r3, r1, r2
	mov	r1, fp
	strh	r3, [r1, #6]
	cmp	r7, #128
	bne.n	.L_02000f92
	movs	r1, #192
	movs	r0, #10
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a8cc
.L_02000f92:
	movs	r0, #1
	adds	r7, #1
	bl 0x0200a7cc
	cmp	r7, #191
	ble.n	.L_02000ede
	movs	r3, #128
	lsls	r3, r3, #8
	mov	r2, fp
	strh	r3, [r2, #6]
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #1
	movs	r0, #11
	bl 0x0200a87c
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #10
	adds	r1, #204
	adds	r2, #102
	bl 0x0200a84c
	movs	r0, #10
	movs	r1, #100
	movs	r2, #112
	bl 0x0200a864
	movs	r1, #0
	movs	r2, #0
	movs	r0, #10
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #10
	adds	r1, #102
	adds	r2, #51
	bl 0x0200a84c
	movs	r2, #0
	movs	r1, #8
	movs	r0, #10
	bl 0x0200a90c
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #11
	bl 0x0200a87c
	movs	r0, #40
	bl 0x0200a824
	movs	r1, #1
	movs	r0, #11
	bl 0x0200a87c
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #10
	bl 0x0200a844
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #8
	strb	r3, [r0, #0]
	negs	r1, r1
	movs	r2, #0
	movs	r0, #10
	bl 0x0200a90c
	movs	r0, #1
	bl 0x0200a824
	movs	r0, #10
	bl 0x0200a844
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #10
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #10
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #132
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200a8dc
	movs	r1, #3
	movs	r0, #11
	bl 0x0200a87c
	movs	r0, #40
	bl 0x0200a824
	ldr	r5, [pc, #52]
	ldr	r3, [sp, #0]
	mov	r1, fp
	strb	r5, [r3, #0]
	mov	r2, sl
	ldr	r3, [r1, #8]
	movs	r7, #0
	str	r3, [r2, #0]
	ldr	r3, [r1, #12]
	str	r3, [r2, #4]
	ldr	r3, [r1, #16]
	mov	r1, r8
	str	r3, [r2, #8]
	movs	r3, #132
	lsls	r3, r3, #16
	str	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r1, #4]
	movs	r2, #132
	movs	r3, #200
	lsls	r3, r3, #15
	lsls	r2, r2, #7
	str	r3, [r1, #8]
	str	r2, [sp, #4]
	b.n	.L_020010e0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_020010e0:
	movs	r1, #192
	lsls	r0, r7, #14
	bl 0x0200a7c4
	bl 0x0200a7d4
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #3
	str	r3, [sp, #8]
	ldr	r3, [sp, #4]
	movs	r1, #192
	lsls	r1, r1, #2
	adds	r3, r3, r1
	str	r3, [sp, #4]
	mov	r2, r8
	mov	r1, sl
	ldr	r3, [r2, #0]
	ldr	r5, [r1, #0]
	movs	r1, #192
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	bl 0x0200a7c4
	mov	r2, r9
	adds	r5, r5, r0
	str	r5, [r2, #0]
	movs	r1, #192
	lsls	r0, r7, #15
	bl 0x0200a7c4
	bl 0x0200a7d4
	mov	r2, sl
	mov	r1, r8
	ldr	r5, [r2, #4]
	ldr	r3, [r1, #4]
	adds	r6, r0, #0
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	movs	r1, #192
	bl 0x0200a7c4
	lsls	r6, r6, #5
	adds	r5, r5, r0
	adds	r5, r5, r6
	mov	r3, r9
	str	r5, [r3, #4]
	mov	r2, sl
	mov	r1, r8
	ldr	r5, [r2, #8]
	ldr	r3, [r1, #8]
	movs	r1, #192
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	bl 0x0200a7c4
	mov	r3, r9
	adds	r5, r5, r0
	str	r5, [r3, #8]
	ldr	r1, [sp, #4]
	mov	r2, r9
	ldr	r0, [sp, #8]
	bl 0x0200a7dc
	mov	r1, r9
	ldr	r3, [r1, #0]
	mov	r2, fp
	str	r3, [r2, #8]
	ldr	r3, [r1, #4]
	str	r3, [r2, #12]
	ldr	r3, [r1, #8]
	str	r3, [r2, #16]
	ldr	r1, [sp, #4]
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r3, r1, r2
	mov	r1, fp
	strh	r3, [r1, #6]
	cmp	r7, #153
	bne.n	.L_02001194
	movs	r1, #224
	movs	r0, #10
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a8cc
.L_02001194:
	movs	r0, #1
	adds	r7, #1
	bl 0x0200a7cc
	cmp	r7, #191
	ble.n	.L_020010e0
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #10
	bl 0x0200a8cc
	movs	r3, #208
	mov	r2, fp
	lsls	r3, r3, #8
	strh	r3, [r2, #6]
	ldr	r3, [r2, #8]
	mov	r1, sl
	str	r3, [r1, #0]
	movs	r7, #0
	ldr	r3, [r2, #12]
	str	r3, [r1, #4]
	ldr	r3, [r2, #16]
	mov	r2, r8
	str	r3, [r1, #8]
	movs	r3, #180
	lsls	r3, r3, #15
	str	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r2, #4]
	movs	r3, #128
	lsls	r3, r3, #15
	str	r3, [r2, #8]
.L_020011d8:
	mov	r1, r8
	ldr	r3, [r1, #0]
	mov	r1, sl
	ldr	r2, [r1, #0]
	subs	r3, r3, r2
	muls	r3, r7
	cmp	r3, #0
	bge.n	.L_020011ea
	adds	r3, #15
.L_020011ea:
	asrs	r3, r3, #4
	adds	r3, r2, r3
	mov	r2, fp
	str	r3, [r2, #8]
	mov	r1, r8
	ldr	r3, [r1, #4]
	mov	r1, sl
	ldr	r2, [r1, #4]
	subs	r3, r3, r2
	muls	r3, r7
	cmp	r3, #0
	bge.n	.L_02001204
	adds	r3, #15
.L_02001204:
	asrs	r3, r3, #4
	adds	r3, r2, r3
	mov	r2, fp
	str	r3, [r2, #12]
	mov	r1, r8
	ldr	r3, [r1, #8]
	mov	r1, sl
	ldr	r2, [r1, #8]
	subs	r3, r3, r2
	muls	r3, r7
	cmp	r3, #0
	bge.n	.L_0200121e
	adds	r3, #15
.L_0200121e:
	asrs	r3, r3, #4
	adds	r3, r2, r3
	mov	r2, fp
	str	r3, [r2, #16]
	movs	r0, #1
	adds	r7, #1
	bl 0x0200a7cc
	cmp	r7, #15
	ble.n	.L_020011d8
	movs	r1, #0
	movs	r2, #0
	movs	r0, #11
	bl 0x0200a874
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #10
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #10
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r0, #10
	movs	r1, #2
	bl 0x0200a8d4
	bl 0x0200a834
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r4, r0, #0
	adds	r6, r2, #0
	adds	r5, r1, #0
	lsls	r3, r3, #16
	movs	r0, #244
	asrs	r7, r3, #16
	lsls	r0, r0, #1
	adds	r3, r6, #0
	adds	r1, r4, #0
	adds	r2, r5, #0
	bl 0x0200a804
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_020012d0
	movs	r1, #1
	ldr	r5, [r6, #80]
	bl 0x0200a7f4
	ldr	r1, [pc, #32]
	adds	r0, r6, #0
	bl 0x0200a7fc
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r3, [sp, #16]
	ldr	r1, [pc, #12]
	adds	r2, #9
	strh	r3, [r2, #0]
	strb	r1, [r5, #26]
	strh	r7, [r5, #18]
.L_020012d0:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0xadec
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	sub	sp, #4
	bl 0x0200a844
	movs	r3, #128
	adds	r5, r0, #0
	lsls	r3, r3, #12
	ldr	r0, [r5, #8]
	mov	sl, r3
	ldr	r1, [r5, #12]
	movs	r3, #224
	lsls	r3, r3, #13
	mov	r8, r3
	movs	r3, #128
	ldr	r2, [r5, #16]
	add	r1, r8
	lsls	r3, r3, #5
	movs	r6, #15
	add	r0, sl
	str	r6, [sp, #0]
	mov	fp, r3
	bl 0x0200928c
	movs	r0, #151
	bl 0x0200a924
	movs	r0, #15
	bl 0x0200a824
	ldr	r0, [r5, #8]
	ldr	r3, [pc, #108]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
	movs	r3, #240
	ldr	r2, [r5, #16]
	add	r1, r8
	lsls	r3, r3, #8
	str	r6, [sp, #0]
	mov	r9, r3
	bl 0x0200928c
	movs	r0, #151
	bl 0x0200a924
	movs	r0, #15
	bl 0x0200a824
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #12]
	ldr	r2, [r5, #16]
	add	r1, r8
	mov	r3, fp
	add	r0, sl
	str	r6, [sp, #0]
	bl 0x0200928c
	movs	r0, #151
	bl 0x0200a924
	movs	r0, #15
	bl 0x0200a824
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #12]
	ldr	r3, [pc, #40]
	ldr	r2, [r5, #16]
	add	r1, r8
	adds	r0, r0, r3
	mov	r3, r9
	str	r6, [sp, #0]
	bl 0x0200928c
	movs	r0, #151
	bl 0x0200a924
	movs	r0, #15
	bl 0x0200a824
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb500
	ldr	r1, [r0, #80]
	adds	r0, #100
	ldrh	r3, [r0, #0]
	movs	r2, #3
	ands	r2, r3
	ldr	r4, [r1, #40]
	cmp	r2, #1
	beq.n	.L_020013cc
	cmp	r2, #1
	bgt.n	.L_020013b4
	cmp	r2, #0
	beq.n	.L_020013be
	b.n	.L_020013e8
.L_020013b4:
	cmp	r2, #2
	beq.n	.L_020013d0
	cmp	r2, #3
	beq.n	.L_020013de
	b.n	.L_020013e8
.L_020013be:
	movs	r3, #7
	strb	r3, [r4, #5]
	movs	r3, #1
	strb	r3, [r1, #25]
	movs	r3, #2
	strb	r3, [r1, #26]
	b.n	.L_020013e8
.L_020013cc:
	movs	r3, #0
	b.n	.L_020013d8
.L_020013d0:
	movs	r2, #7
	movs	r3, #0
	strb	r2, [r4, #5]
	movs	r2, #1
.L_020013d8:
	strb	r2, [r1, #25]
	strb	r3, [r1, #26]
	b.n	.L_020013e8
.L_020013de:
	movs	r2, #0
	movs	r3, #1
	strb	r2, [r4, #5]
	strb	r3, [r1, #25]
	strb	r2, [r1, #26]
.L_020013e8:
	ldrh	r3, [r0, #0]
	adds	r3, #1
	strh	r3, [r0, #0]
	pop	{pc}
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #105
	bl 0x0200a7ec
	bl 0x0200a82c
	movs	r0, #0
	bl 0x0200a8fc
	ldr	r0, [pc, #428]
	bl 0x0200a8ac
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #128
	movs	r2, #128
	movs	r0, #18
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200a84c
	movs	r0, #18
	movs	r1, #32
	movs	r2, #0
	bl 0x0200a90c
	movs	r2, #32
	movs	r0, #18
	movs	r1, #0
	negs	r2, r2
	bl 0x0200a90c
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #220
	movs	r2, #176
	movs	r0, #4
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200a864
	movs	r1, #220
	movs	r2, #176
	movs	r0, #4
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200a874
	movs	r3, #160
	movs	r0, #12
	movs	r1, #16
	movs	r2, #0
	lsls	r3, r3, #7
	bl 0x0200a904
	movs	r1, #16
	movs	r3, #128
	movs	r0, #5
	negs	r1, r1
	movs	r2, #0
	lsls	r3, r3, #7
	bl 0x0200a904
	movs	r1, #32
	movs	r3, #128
	lsls	r3, r3, #7
	movs	r0, #6
	negs	r1, r1
	movs	r2, #0
	bl 0x0200a904
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #4
	bl 0x0200a8cc
	movs	r0, #6
	bl 0x0200a86c
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #216
	movs	r1, #1
	movs	r2, #184
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200a8ec
	bl 0x0200a8f4
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #18
	bl 0x0200a8dc
	movs	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #6
	adds	r1, #255
	movs	r2, #30
	movs	r0, #6
	bl 0x0200a8dc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #10
	adds	r1, #255
	movs	r2, #40
	movs	r0, #18
	bl 0x0200a8dc
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r2, #10
	movs	r0, #18
	movs	r1, #0
	bl 0x0200a8bc
	movs	r0, #4
	movs	r1, #3
	bl 0x0200a87c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a87c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200a87c
	movs	r1, #3
	movs	r0, #12
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #18
	bl 0x0200a8dc
	movs	r1, #0
	movs	r0, #18
	bl 0x0200a8b4
	movs	r0, #4
	movs	r1, #0
	bl 0x0200a83c
	cmp	r0, #0
	bne.n	.L_020015c0
	movs	r0, #20
	bl 0x0200a824
	movs	r2, #10
	movs	r0, #18
	movs	r1, #0
	bl 0x0200a8bc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020015e2
	.2byte 0x279b
	.2byte 0x0000
.L_020015c0:
	movs	r0, #35
	bl 0x0200a824
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #18
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
.L_020015e2:
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #12
	bl 0x0200a8dc
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #18
	bl 0x0200a8dc
	movs	r2, #10
	movs	r0, #18
	movs	r1, #0
	bl 0x0200a8bc
	movs	r0, #4
	movs	r1, #3
	bl 0x0200a87c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a87c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200a87c
	movs	r0, #12
	movs	r1, #3
	bl 0x0200a884
	movs	r0, #18
	movs	r1, #3
	bl 0x0200a894
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #18
	bl 0x0200a8e4
	movs	r0, #40
	bl 0x0200a824
	movs	r2, #10
	movs	r0, #18
	movs	r1, #0
	bl 0x0200a8bc
	movs	r0, #4
	movs	r1, #3
	bl 0x0200a87c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a87c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200a87c
	movs	r0, #12
	movs	r1, #3
	bl 0x0200a884
	movs	r1, #2
	movs	r0, #18
	bl 0x0200a89c
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #15
	bl 0x0200a824
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #18
	bl 0x0200a8dc
	movs	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a8cc
	movs	r0, #40
	bl 0x0200a824
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a8cc
	movs	r0, #25
	bl 0x0200a824
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #5
	bl 0x0200a8dc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #18
	bl 0x0200a8dc
	movs	r2, #10
	movs	r0, #18
	movs	r1, #0
	bl 0x0200a8bc
	movs	r0, #4
	movs	r1, #3
	bl 0x0200a87c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a87c
	movs	r1, #3
	movs	r0, #6
	bl 0x0200a884
	movs	r0, #15
	bl 0x0200a824
	movs	r0, #18
	movs	r1, #4
	bl 0x0200a884
	movs	r0, #18
	movs	r1, #0
	movs	r2, #20
	bl 0x0200a8bc
	movs	r0, #18
	movs	r1, #12
	movs	r2, #0
	bl 0x0200a90c
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #18
	bl 0x0200a8dc
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #18
	movs	r1, #0
	bl 0x0200a8bc
	movs	r0, #12
	movs	r1, #3
	bl 0x0200a894
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #12
	bl 0x0200a8e4
	movs	r0, #40
	bl 0x0200a824
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a8cc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a8cc
	movs	r2, #0
	movs	r1, #0
	movs	r0, #5
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #2
	movs	r0, #6
	bl 0x0200a89c
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #6
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	movs	r2, #128
	movs	r0, #18
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200a84c
	movs	r1, #12
	movs	r0, #18
	negs	r1, r1
	movs	r2, #0
	bl 0x0200a90c
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #18
	bl 0x0200a8dc
	movs	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #12
	bl 0x0200a8dc
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #50
	movs	r0, #18
	bl 0x0200a8dc
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r2, #10
	movs	r0, #18
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #3
	movs	r0, #18
	bl 0x0200a884
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #18
	bl 0x0200a884
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #18
	bl 0x0200a884
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #6
	adds	r1, #255
	movs	r2, #30
	movs	r0, #12
	bl 0x0200a8dc
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #2
	adds	r1, #255
	movs	r2, #45
	movs	r0, #18
	bl 0x0200a8dc
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #18
	adds	r1, #204
	adds	r2, #102
	bl 0x0200a84c
	movs	r0, #18
	movs	r1, #12
	movs	r2, #0
	bl 0x0200a90c
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #18
	movs	r1, #0
	bl 0x0200a8bc
	movs	r0, #12
	movs	r1, #4
	bl 0x0200a884
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #18
	bl 0x0200a8dc
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #18
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #3
	movs	r0, #12
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r2, #10
	movs	r0, #12
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #2
	movs	r0, #18
	bl 0x0200a89c
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #18
	movs	r1, #4
	bl 0x0200a884
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #18
	movs	r1, #0
	bl 0x0200a8bc
	movs	r0, #5
	movs	r1, #4
	bl 0x0200a884
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #12
	movs	r0, #18
	negs	r1, r1
	movs	r2, #0
	bl 0x0200a90c
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #18
	bl 0x0200a8dc
	movs	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #5
	bl 0x0200a8dc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r0, #18
	bl 0x020092dc
	movs	r0, #151
	bl 0x0200a924
	movs	r1, #2
	adds	r1, #255
	movs	r2, #50
	movs	r0, #18
	bl 0x0200a8dc
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #25
	bl 0x0200a824
	movs	r2, #10
	movs	r0, #18
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #3
	movs	r0, #5
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #18
	bl 0x0200a8dc
	movs	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #12
	bl 0x0200a8dc
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	movs	r2, #128
	movs	r0, #18
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200a84c
	movs	r0, #18
	movs	r1, #12
	movs	r2, #0
	bl 0x0200a90c
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #18
	bl 0x0200a8dc
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #18
	movs	r1, #0
	bl 0x0200a8bc
	movs	r0, #5
	movs	r1, #4
	bl 0x0200a884
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #12
	movs	r0, #18
	negs	r1, r1
	movs	r2, #0
	bl 0x0200a90c
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #18
	movs	r1, #6
	movs	r2, #15
	bl 0x0200a88c
	movs	r0, #18
	movs	r1, #6
	movs	r2, #23
	bl 0x0200a88c
	movs	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #12
	bl 0x0200a8dc
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #2
	movs	r2, #50
	adds	r1, #255
	movs	r0, #18
	bl 0x0200a8dc
	movs	r0, #12
	movs	r1, #4
	bl 0x0200a884
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #129
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #18
	bl 0x0200a8dc
	movs	r1, #2
	movs	r0, #6
	bl 0x0200a89c
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #6
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #18
	bl 0x0200a8dc
	movs	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #0
	movs	r0, #5
	bl 0x0200a8b4
	movs	r0, #4
	movs	r1, #0
	bl 0x0200a83c
	cmp	r0, #0
	bne.n	.L_02001bee
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #208
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #18
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r2, #10
	movs	r0, #18
	movs	r1, #0
	bl 0x0200a8bc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02001c2a
.L_02001bee:
	movs	r0, #35
	bl 0x0200a824
	movs	r1, #208
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #18
	movs	r1, #4
	bl 0x0200a884
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #18
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
.L_02001c2a:
	movs	r1, #2
	movs	r0, #12
	bl 0x0200a89c
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a8cc
	movs	r2, #0
	movs	r1, #0
	movs	r0, #6
	bl 0x0200a8cc
	movs	r0, #25
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #12
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #0
	movs	r0, #12
	bl 0x0200a8b4
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a8cc
	movs	r0, #4
	movs	r1, #0
	bl 0x0200a83c
	cmp	r0, #0
	bne.n	.L_02001cae
	movs	r0, #20
	bl 0x0200a824
	movs	r2, #10
	movs	r0, #12
	movs	r1, #0
	bl 0x0200a8bc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02001cd0
.L_02001cae:
	movs	r0, #35
	bl 0x0200a824
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #12
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
.L_02001cd0:
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a8dc
	movs	r1, #2
	adds	r1, #255
	movs	r2, #50
	movs	r0, #6
	bl 0x0200a8dc
	movs	r1, #254
	lsls	r1, r1, #7
	movs	r2, #0
	adds	r1, #255
	movs	r0, #4
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #12
.L_02001d00:
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #254
	lsls	r1, r1, #7
	movs	r0, #5
	adds	r1, #255
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #6
	bl 0x0200a8cc
	movs	r0, #40
	bl 0x0200a824
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a8dc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #6
	bl 0x0200a8dc
	movs	r2, #0
	movs	r1, #0
	movs	r0, #5
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r0, #6
	movs	r1, #3
	bl 0x0200a894
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #6
	bl 0x0200a8e4
	movs	r0, #40
	bl 0x0200a824
	movs	r2, #10
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #3
	movs	r0, #12
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #254
	lsls	r1, r1, #7
	movs	r0, #5
	adds	r1, #255
	movs	r2, #0
	bl 0x0200a8cc
	movs	r2, #0
	movs	r1, #0
	movs	r0, #6
	bl 0x0200a8cc
	movs	r0, #40
	bl 0x0200a824
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a894
	movs	r1, #129
	movs	r0, #5
	lsls	r1, r1, #1
	bl 0x0200a8e4
	movs	r0, #6
	movs	r1, #3
	bl 0x0200a894
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #6
	bl 0x0200a8e4
	movs	r0, #40
	bl 0x0200a824
	movs	r1, #0
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r2, #10
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #3
	movs	r0, #12
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #2
	adds	r1, #255
	movs	r2, #50
	movs	r0, #18
	bl 0x0200a8dc
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #12
	bl 0x0200a8cc
	movs	r0, #25
	bl 0x0200a824
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #6
	bl 0x0200a8dc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #5
	bl 0x0200a8dc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #12
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #12
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #12
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #153
	movs	r2, #152
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #12
	adds	r1, #153
	adds	r2, #204
	bl 0x0200a84c
	movs	r0, #12
	movs	r1, #12
	movs	r2, #0
	bl 0x0200a90c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #4
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #12
	movs	r2, #0
	movs	r0, #12
	bl 0x0200a90c
	movs	r0, #10
	bl 0x0200a824
	movs	r2, #10
	movs	r0, #12
	movs	r1, #0
	bl 0x0200a8bc
	movs	r0, #4
	movs	r1, #3
	bl 0x0200a87c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a87c
	movs	r1, #3
	movs	r0, #6
	bl 0x0200a884
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #12
	movs	r1, #4
	bl 0x0200a884
	movs	r2, #10
	movs	r0, #12
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #2
	movs	r0, #12
	bl 0x0200a89c
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #18
	movs	r1, #3
	bl 0x0200a894
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #18
	bl 0x0200a8e4
	movs	r0, #50
	bl 0x0200a824
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #5
	bl 0x0200a8dc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	adds	r1, #1
	movs	r0, #12
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #12
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #6
	bl 0x0200a8dc
	movs	r2, #10
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #3
	movs	r0, #12
	bl 0x0200a884
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #2
	movs	r0, #18
	bl 0x0200a89c
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #5
	movs	r1, #4
	bl 0x0200a884
	movs	r2, #10
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #2
	movs	r0, #18
	bl 0x0200a89c
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #128
	movs	r2, #128
	movs	r0, #18
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200a84c
	movs	r0, #18
	movs	r1, #12
	movs	r2, #0
	bl 0x0200a90c
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #4
	bl 0x0200a8dc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a8dc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #6
	bl 0x0200a8dc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #45
	movs	r0, #12
	bl 0x0200a8dc
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #12
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #18
	bl 0x0200a8dc
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #12
	movs	r0, #12
	negs	r1, r1
	movs	r2, #0
	bl 0x0200a90c
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #12
	bl 0x0200a8cc
	movs	r0, #15
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #12
	bl 0x0200a884
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #18
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #12
	bl 0x0200a8dc
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #18
	bl 0x0200a8dc
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a8cc
	movs	r0, #40
	bl 0x0200a824
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #12
	bl 0x0200a8dc
	movs	r2, #10
	movs	r0, #12
	movs	r1, #0
	bl 0x0200a8bc
	movs	r1, #3
	movs	r0, #18
	bl 0x0200a884
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #12
	bl 0x0200a8cc
	movs	r0, #25
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #12
	bl 0x0200a8cc
	movs	r0, #25
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #18
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #18
	movs	r1, #0
	bl 0x0200a8bc
	movs	r0, #4
	movs	r1, #3
	bl 0x0200a87c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a87c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200a87c
	movs	r1, #3
	movs	r0, #12
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #18
	movs	r1, #4
	bl 0x0200a884
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a8cc
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #12
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a8cc
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #12
	bl 0x0200a8cc
	movs	r0, #30
	bl 0x0200a824
	movs	r0, #4
	movs	r1, #4
	bl 0x0200a87c
	movs	r0, #5
	movs	r1, #4
	bl 0x0200a87c
	movs	r0, #6
	movs	r1, #4
	bl 0x0200a87c
	movs	r0, #12
	movs	r1, #4
	bl 0x0200a884
	movs	r0, #18
	movs	r1, #4
	bl 0x0200a884
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #2
	movs	r2, #50
	adds	r1, #255
	movs	r0, #12
	bl 0x0200a8dc
	movs	r1, #2
	movs	r0, #18
	bl 0x0200a89c
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #208
	lsls	r1, r1, #8
	adds	r1, #10
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #12
	bl 0x0200a8dc
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #8
	adds	r1, #255
	movs	r2, #42
	movs	r0, #18
	bl 0x0200a8dc
	movs	r0, #18
	movs	r1, #6
	movs	r2, #15
	bl 0x0200a88c
	movs	r0, #18
	movs	r1, #6
	movs	r2, #23
	bl 0x0200a88c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #2
	movs	r2, #12
	negs	r2, r2
	negs	r1, r1
	movs	r0, #18
	bl 0x0200a90c
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #18
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #18
	ldr	r1, [pc, #1008]
	adds	r2, #204
	bl 0x0200a84c
	movs	r1, #8
	movs	r0, #18
	negs	r1, r1
	movs	r2, #40
	bl 0x0200a90c
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a8cc
	movs	r0, #18
	movs	r1, #0
	movs	r2, #20
	bl 0x0200a90c
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	bl 0x0200a814
	movs	r1, #243
	movs	r2, #243
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	adds	r1, #102
	adds	r2, #51
	movs	r0, #18
	bl 0x0200a84c
	movs	r0, #133
	bl 0x0200a924
	movs	r1, #2
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a88c
	movs	r0, #18
	bl 0x0200a844
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	movs	r2, #0
	strb	r3, [r0, #0]
	movs	r1, #0
	mov	sl, r2
	movs	r0, #18
	subs	r2, #12
	bl 0x0200a90c
	movs	r0, #1
	bl 0x0200a824
	movs	r0, #18
	bl 0x0200a844
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r6, #1
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r1, #0
	movs	r2, #0
	movs	r0, #0
	bl 0x0200a814
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #18
	bl 0x0200a8dc
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #153
	movs	r2, #152
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r1, #153
	adds	r2, #204
	movs	r0, #18
	bl 0x0200a84c
	movs	r0, #18
	bl 0x0200a844
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	movs	r2, #24
	negs	r2, r2
	strb	r3, [r0, #0]
	movs	r1, #0
	movs	r0, #18
	bl 0x0200a90c
	movs	r0, #1
	bl 0x0200a824
	movs	r0, #18
	bl 0x0200a844
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #18
	bl 0x0200a884
	movs	r0, #20
	bl 0x0200a824
	movs	r0, #18
	movs	r1, #6
	movs	r2, #15
	bl 0x0200a88c
	movs	r1, #6
	movs	r2, #23
	movs	r0, #18
	bl 0x0200a88c
	movs	r0, #18
	bl 0x0200a844
	adds	r3, r0, #0
	adds	r3, #100
	mov	r2, sl
	strh	r2, [r3, #0]
	ldr	r3, [pc, #704]
	movs	r1, #0
	str	r3, [r0, #108]
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r2, #10
	bl 0x0200a8bc
	movs	r0, #18
	ldr	r1, [pc, #688]
	ldr	r2, [pc, #688]
	bl 0x0200a84c
	movs	r1, #0
	movs	r2, #32
	movs	r0, #18
	bl 0x0200a90c
	movs	r0, #18
	bl 0x0200a844
	mov	r8, r0
	movs	r0, #134
	bl 0x0200a924
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	bl 0x0200a814
	movs	r0, #18
	bl 0x0200a844
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #16
	ands	r5, r3
	movs	r1, #0
	negs	r2, r2
	strb	r5, [r0, #0]
	movs	r0, #18
	bl 0x0200a90c
	movs	r0, #1
	bl 0x0200a824
	movs	r0, #18
	bl 0x0200a844
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, r8
	adds	r2, #100
	orrs	r6, r3
	movs	r3, #3
	strb	r6, [r0, #0]
	strh	r3, [r2, #0]
	movs	r0, #1
	bl 0x0200a7cc
	mov	r3, sl
	mov	r2, r8
	str	r3, [r2, #108]
	movs	r0, #18
	bl 0x0200a844
	movs	r1, #1
	bl 0x0200a80c
	movs	r0, #40
	bl 0x0200a824
	movs	r1, #0
	movs	r2, #0
	movs	r0, #0
	bl 0x0200a814
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #18
	bl 0x0200a8dc
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #18
	movs	r1, #0
	bl 0x0200a8bc
	movs	r0, #4
	movs	r1, #3
	bl 0x0200a894
	movs	r1, #129
	movs	r0, #4
.L_020025a0:
	lsls	r1, r1, #1
	bl 0x0200a8e4
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a894
	movs	r1, #129
	movs	r0, #5
	lsls	r1, r1, #1
	bl 0x0200a8e4
	movs	r0, #6
	movs	r1, #3
	bl 0x0200a894
	movs	r1, #129
	movs	r0, #6
.L_020025c4:
	lsls	r1, r1, #1
	bl 0x0200a8e4
	movs	r0, #12
	movs	r1, #3
	bl 0x0200a894
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #12
	bl 0x0200a8e4
	movs	r0, #40
	bl 0x0200a824
.L_020025e2:
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a8cc
	movs	r0, #25
	bl 0x0200a824
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
.L_02002602:
	movs	r0, #18
	movs	r1, #0
	movs	r2, #48
	bl 0x0200a90c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a874
	movs	r0, #30
	bl 0x0200a824
	movs	r1, #6
	adds	r1, #255
	movs	r2, #30
	movs	r0, #6
	bl 0x0200a8dc
	movs	r2, #10
	movs	r0, #6
	movs	r1, #0
	bl 0x0200a8bc
	movs	r0, #5
	movs	r1, #4
	bl 0x0200a884
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r0, #12
	movs	r1, #6
	movs	r2, #15
	bl 0x0200a88c
	movs	r0, #12
	movs	r1, #6
	movs	r2, #23
	bl 0x0200a88c
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #12
	bl 0x0200a8cc
	movs	r0, #15
	bl 0x0200a824
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a8bc
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a8cc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a8cc
	movs	r2, #0
	movs	r1, #0
	movs	r0, #6
	bl 0x0200a8cc
	movs	r0, #20
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #4
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #5
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #6
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r1, #3
	movs	r0, #12
	bl 0x0200a884
	movs	r0, #10
	bl 0x0200a824
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #12
	ldr	r1, [pc, #188]
	bl 0x0200a84c
	movs	r0, #12
	movs	r1, #2
	bl 0x0200a87c
	ldr	r3, [pc, #176]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x0200a844
	cmp	r0, #0
	beq.n	.L_02002710
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #12
	bl 0x0200a85c
.L_02002710:
	movs	r0, #12
	bl 0x0200a86c
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a874
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #120]
	adds	r2, #153
	bl 0x0200a84c
	movs	r0, #5
	movs	r1, #2
	bl 0x0200a87c
	ldr	r0, [r5, #0]
	bl 0x0200a844
	cmp	r0, #0
	beq.n	.L_0200274e
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200a85c
.L_0200274e:
	movs	r0, #5
	bl 0x0200a86c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a874
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #56]
	adds	r2, #153
	bl 0x0200a84c
	movs	r0, #6
	movs	r1, #2
	bl 0x0200a87c
	ldr	r0, [r5, #0]
	bl 0x0200a844
	cmp	r0, #0
	beq.n	.L_020027a8
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x0200a85c
	b.n	.L_020027a8
	.2byte 0x0000
	.4byte 0x00019999
	.4byte 0x02009399
	.4byte 0x00023333
	.4byte 0x00011999
	.4byte 0x00013333
	.2byte 0x0240
	.2byte 0x0200
.L_020027a8:
	movs	r0, #6
	bl 0x0200a86c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a874
	bl 0x0200a834
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.irp EntryTarget, 0x03000528, 0x080000c1, 0x08000119, 0x08000129, 0x080003c9, 0x080003d1, 0x08020091, 0x080200a9, 0x080200c1, 0x08020219, 0x08020229, 0x080ad111, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80b1, 0x080c80c1, 0x080c80d9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8129, 0x080c8139, 0x080c8141, 0x080c8149, 0x080c8159, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81d1, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8239, 0x080c8241, 0x080c84e1, 0x080c85e9, 0x080c85f9, 0x080c8601, 0x080c8779, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
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
	.4byte 0x002001a0
	.4byte 0x01b00250
	.4byte 0x02600030
	.4byte 0x000dffff
	.4byte 0xffe802b0
	.4byte 0x02c00260
	.4byte 0x0270fff8
	.4byte 0x000effff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000cb
	.4byte 0x101010ca
	.4byte 0xffffffff
	.4byte 0x102020ca
	.4byte 0xffffffff
	.4byte 0x103030ca
	.4byte 0xffffffff
	.4byte 0x104060cb
	.4byte 0xffffffff
	.4byte 0x105070cb
	.4byte 0xffffffff
	.4byte 0x106040cb
	.4byte 0xffffffff
	.4byte 0x107050cb
	.4byte 0xffffffff
	.4byte 0x1080a0cb
	.4byte 0xffffffff
	.4byte 0x1090b0cb
	.4byte 0xffffffff
	.4byte 0x10a080cb
	.4byte 0xffffffff
	.4byte 0x10b090cb
	.4byte 0xffffffff
	.4byte 0x10c0c0ca
	.4byte 0xffffffff
	.4byte 0x10d0e0cb
	.4byte 0xffffffff
	.4byte 0x10e0d0cb
	.4byte 0xffffffff
	.4byte 0x10f0f0ca
	.4byte 0xffffffff
	.4byte 0x110100ca
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff009d
	.4byte 0x00000002
	.4byte 0x01e00000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00004000
	.4byte 0xffff009c
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001d000
	.4byte 0xffff00a0
	.4byte 0x00000001
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00008000
	.4byte 0xffff00db
	.4byte 0x00000001
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0003b000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x024c0000
	.4byte 0x0003b000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00035000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00033000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00033000
	.4byte 0xffff002b
	.4byte 0x00000001
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x0003d000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x0001b000
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
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00004401
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000002
	.4byte 0x0966001e
	.4byte 0x02008a5d
	.4byte 0x00000002
	.4byte 0x0969001f
	.4byte 0x020093f1
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002749
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000274a
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000274b
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000274c
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008059
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x02008105
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008155
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002786
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002787
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002788
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002789
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x020081a1
	.4byte 0x00008d15
	.4byte 0x0967040d
	.4byte 0x020081ed
	.4byte 0x00008d15
	.4byte 0x0967040e
	.4byte 0x020081ed
	.4byte 0x00008d15
	.4byte 0x0967040f
	.4byte 0x020081ed
	.4byte 0x00008d15
	.4byte 0x09670410
	.4byte 0x020081ed
	.4byte 0x00008d15
	.4byte 0x09670411
	.4byte 0x020081ed
	.4byte 0x00008d15
	.4byte 0x09670413
	.4byte 0x020081ed
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002790
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002791
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002792
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002793
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002794
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002795
	.4byte 0x000000f3
	.4byte 0xffff00c8
	.4byte 0x00403064
	.4byte 0x000000f3
	.4byte 0xffff00c9
	.4byte 0x00403065
	.4byte 0x000000f3
	.4byte 0xffff00ca
	.4byte 0x00403066
	.4byte 0x000000f3
	.4byte 0xffff00cb
	.4byte 0x00403067
	.4byte 0x000000f3
	.4byte 0xffff00cc
	.4byte 0x00403068
	.4byte 0x000000f3
	.4byte 0xffff00cd
	.4byte 0x00403069
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x0040306a
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x0040306b
	.4byte 0x000001c3
	.4byte 0xffff00d0
	.4byte 0x0040306d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000026
