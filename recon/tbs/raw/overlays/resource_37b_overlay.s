.syntax unified
	.thumb
	.section .text.x02008c8c,"ax",%progbits
	.global SoruSekizo_RunSealOpenedSequence
	.thumb_func
SoruSekizo_RunSealOpenedSequence:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #516]
	ldr	r3, [r3, #0]
	movs	r0, #0
	mov	sl, r3
	sub	sp, #20
	bl 0x0200a46c
	adds	r5, r0, #0
	movs	r2, #179
	ldr	r3, [r5, #16]
	lsls	r2, r2, #16
	cmp	r3, r2
	bge.n	.L_02000d0c
	movs	r0, #0
	ldr	r1, [pc, #492]
	movs	r2, #132
	bl 0x0200a494
	movs	r1, #128
	movs	r2, #0
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x0200a4f4
	movs	r0, #30
	bl 0x0200a44c
	mov	r3, sl
	ldr	r3, [r3, #0]
	mov	fp, r3
	ldr	r3, [r5, #8]
	add	r7, sp, #8
	str	r3, [r7, #0]
	ldr	r3, [r5, #12]
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	mov	r2, sl
	str	r3, [r7, #8]
	str	r7, [r2, #0]
	movs	r6, #0
	adds	r5, r7, #0
.L_02000cec:
	ldr	r3, [r5, #8]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r5, #8]
	movs	r0, #1
	adds	r6, #1
	bl 0x0200a44c
	cmp	r6, #30
	bne.n	.L_02000cec
	movs	r0, #40
	bl 0x0200a44c
	movs	r3, #1
	b.n	.L_02000d60
.L_02000d0c:
	movs	r0, #0
	ldr	r1, [pc, #408]
	movs	r2, #222
	bl 0x0200a494
	movs	r1, #192
	movs	r2, #0
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x0200a4f4
	movs	r0, #30
	bl 0x0200a44c
	ldr	r3, [r5, #8]
	add	r7, sp, #8
	str	r3, [r7, #0]
	ldr	r3, [r5, #12]
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	mov	r2, sl
	ldr	r2, [r2, #0]
	str	r3, [r7, #8]
	mov	r3, sl
	str	r7, [r3, #0]
	mov	fp, r2
	movs	r6, #0
	adds	r5, r7, #0
.L_02000d44:
	ldr	r3, [r5, #8]
	ldr	r2, [pc, #356]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	movs	r0, #1
	adds	r6, #1
	bl 0x0200a44c
	cmp	r6, #30
	bne.n	.L_02000d44
	movs	r0, #40
	bl 0x0200a44c
	movs	r3, #2
.L_02000d60:
	mov	r9, r3
	movs	r2, #4
	movs	r6, #0
	mov	r8, r2
	movs	r5, #2
.L_02000d6a:
	mov	r3, r8
	str	r3, [sp, #0]
	movs	r1, #28
	movs	r2, #34
	movs	r3, #10
	movs	r0, #2
	str	r5, [sp, #4]
	bl 0x0200a404
	movs	r0, #8
	bl 0x0200a44c
	mov	r2, r8
	str	r2, [sp, #0]
	movs	r0, #2
	movs	r1, #30
	movs	r2, #34
	movs	r3, #10
	str	r5, [sp, #4]
	adds	r6, #1
	bl 0x0200a404
	movs	r0, #8
	bl 0x0200a44c
	cmp	r6, #6
	bne.n	.L_02000d6a
	movs	r3, #4
	movs	r6, #0
	mov	r8, r3
	movs	r5, #2
.L_02000da8:
	mov	r2, r8
	str	r2, [sp, #0]
	movs	r1, #28
	movs	r2, #34
	movs	r3, #10
	movs	r0, #2
	str	r5, [sp, #4]
	bl 0x0200a404
	movs	r0, #4
	bl 0x0200a44c
	mov	r3, r8
	str	r3, [sp, #0]
	movs	r0, #2
	movs	r1, #30
	movs	r2, #34
	movs	r3, #10
	str	r5, [sp, #4]
	adds	r6, #1
	bl 0x0200a404
	movs	r0, #4
	bl 0x0200a44c
	cmp	r6, #10
	bne.n	.L_02000da8
	movs	r2, #4
	movs	r6, #0
	mov	r8, r2
	movs	r5, #2
.L_02000de6:
	mov	r3, r8
	str	r3, [sp, #0]
	movs	r1, #28
	movs	r2, #34
	movs	r3, #10
	movs	r0, #2
	str	r5, [sp, #4]
	bl 0x0200a404
	movs	r0, #2
	bl 0x0200a44c
	mov	r2, r8
	str	r2, [sp, #0]
	movs	r0, #2
	movs	r1, #30
	movs	r2, #34
	movs	r3, #10
	str	r5, [sp, #4]
	adds	r6, #1
	bl 0x0200a404
	movs	r0, #2
	bl 0x0200a44c
	cmp	r6, #12
	bne.n	.L_02000de6
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r5, #4
	movs	r0, #2
	movs	r1, #28
	movs	r2, #34
	movs	r3, #10
	str	r5, [sp, #0]
	bl 0x0200a404
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r0, #8
	movs	r3, #40
	movs	r1, #55
	movs	r2, #32
	str	r5, [sp, #4]
	bl 0x0200a404
	movs	r0, #60
	bl 0x0200a44c
	mov	r3, r9
	cmp	r3, #1
	bne.n	.L_02000e68
	movs	r6, #0
	adds	r5, r7, #0
.L_02000e52:
	ldr	r3, [r5, #8]
	ldr	r2, [pc, #84]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	movs	r0, #1
	adds	r6, #1
	bl 0x0200a44c
	cmp	r6, #30
	bne.n	.L_02000e52
	b.n	.L_02000e88
.L_02000e68:
	mov	r3, r9
	cmp	r3, #2
	bne.n	.L_02000e88
	movs	r6, #0
	adds	r5, r7, #0
.L_02000e72:
	ldr	r3, [r5, #8]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r5, #8]
	movs	r0, #1
	adds	r6, #1
	bl 0x0200a44c
	cmp	r6, #30
	bne.n	.L_02000e72
.L_02000e88:
	mov	r3, fp
	mov	r2, sl
	str	r3, [r2, #0]
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e70
	.4byte 0x0000023f
	.4byte 0x00000241
	.2byte 0x0000
	.2byte 0xffff
	.section .rodata,"a",%progbits
	.global SoruSekizo_SpriteRowsGfx
SoruSekizo_SpriteRowsGfx:
	.4byte 0x01020041
	.4byte 0x000cccc0
	.4byte 0x060100cc
	.4byte 0x012005f8
	.4byte 0x0d200021
	.4byte 0x0c2d4000
	.4byte 0x01111101
	.4byte 0x010a1173
	.4byte 0x03002000
	.4byte 0x00000123
	.4byte 0x40000920
	.4byte 0xffff2324
	.4byte 0xf0ff0302
	.4byte 0x010100ff
	.4byte 0x05f71e04
	.4byte 0x00230322
	.4byte 0x01060420
	.4byte 0x050302f0
	.4byte 0x06200049
	.4byte 0x000d01e0
	.4byte 0x00000162
	.global SceneEventRuntime_ScriptData
SceneEventRuntime_ScriptData:
	.4byte 0xffff0000
	.4byte 0x0000011f
	.4byte 0x400000b0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000286
	.4byte 0x40000149
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000287
	.4byte 0x400001d6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000240
	.4byte 0x40000088
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00000240
	.4byte 0x40000088
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000240
	.4byte 0x40000088
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000240
	.4byte 0x40000088
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0063
	.4byte 0x00000240
	.4byte 0x40000088
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global SceneEventRuntime_MessageData
SceneEventRuntime_MessageData:
	.4byte 0x0000000c
	.4byte 0x0010200b
	.4byte 0x0020100d
	.4byte 0x0030400b
	.4byte 0x0040500b
	.4byte 0x0050600b
	.4byte 0x0060700b
	.4byte 0x00a011fe
	.4byte 0x000001ff
	.global SceneEventRuntime_ActorData
SceneEventRuntime_ActorData:
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x06480000
	.4byte 0x00000000
	.4byte 0x06480000
	.4byte 0x00004000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00005000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x01005000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00003000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x01003000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0000b000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0100b000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0000d000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0100d000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x01200000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff00cc
	.4byte 0x00000001
	.4byte 0x06480000
	.4byte 0x00000000
	.4byte 0x06480000
	.4byte 0x00004000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x02500000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global SceneEventRuntime_EffectData
SceneEventRuntime_EffectData:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x02008fe5
	.4byte 0x00000202
	.4byte 0xffff000b
	.4byte 0x02009001
	.4byte 0x00008c15
	.4byte 0x080b0009
	.4byte 0x02009575
	.4byte 0x00008c15
	.4byte 0x080c000b
	.4byte 0x0200958d
	.4byte 0x00008c15
	.4byte 0x080d000d
	.4byte 0x020095a5
	.4byte 0x00008c15
	.4byte 0x080e000f
	.4byte 0x020095bd
	.4byte 0x00000602
	.4byte 0x080b0005
	.4byte 0x020099e5
	.4byte 0x00008602
	.4byte 0x080c0006
	.4byte 0x02009a05
	.4byte 0x00000602
	.4byte 0x080d0007
	.4byte 0x02009a25
	.4byte 0x00008602
	.4byte 0x080e0008
	.4byte 0x02009a45
	.4byte 0x10008c15
	.4byte 0x0816000a
	.4byte 0x0200966d
	.4byte 0x00008c15
	.4byte 0x0816000a
	.4byte 0x020095d5
	.4byte 0x10008c15
	.4byte 0x0817000c
	.4byte 0x020096a5
	.4byte 0x00008c15
	.4byte 0x0817000c
	.4byte 0x020095fd
	.4byte 0x00000602
	.4byte 0x08160014
	.4byte 0x02009a65
	.4byte 0x00000602
	.4byte 0x08160015
	.4byte 0x02009a65
	.4byte 0x00004602
	.4byte 0x08160015
	.4byte 0x02009a85
	.4byte 0x0000c602
	.4byte 0x08160017
	.4byte 0x02009aad
	.4byte 0x0000c602
	.4byte 0x08160018
	.4byte 0x02009aad
	.4byte 0x00008602
	.4byte 0x08170019
	.4byte 0x02009ad5
	.4byte 0x00008602
	.4byte 0x0817001a
	.4byte 0x02009ad5
	.4byte 0x00004602
	.4byte 0x0817001a
	.4byte 0x02009af5
	.4byte 0x0000c602
	.4byte 0x0817001c
	.4byte 0x02009b1d
	.4byte 0x0000c602
	.4byte 0x0817001d
	.4byte 0x02009b1d
	.4byte 0x10008c15
	.4byte 0x08180011
	.4byte 0x02009625
	.4byte 0x00008c15
	.4byte 0x08180011
	.4byte 0x020096dd
	.4byte 0x0000c602
	.4byte 0x08180020
	.4byte 0x0200995d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global SoruSekizo_ScrollPhase
SoruSekizo_ScrollPhase:
	.4byte 0x00000000
