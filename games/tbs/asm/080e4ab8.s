@ 親関数の分岐表から到達し、同じスタック枠を使う内部断片。
@ 後続82バイトは定数・リテラル表。
.syntax unified
	.thumb
	.set sub_080e0524, 0x080e0524
	.set sub_080e4b4c, 0x080e4b4c
	.set sub_080e4b54, 0x080e4b54
	.global Fragment_080e4ab8
Fragment_080e4ab8:
	ldr r0, [pc, #120]
	b .L_080e4ac6
	ldr r0, [pc, #120]
	b .L_080e4ac6
	ldr r0, [pc, #120]
	b .L_080e4ac6
	ldr r0, [pc, #120]
.L_080e4ac6:
	ldr r1, [pc, #124]
	movs r2, #1
	movs r3, #1
	bl sub_080e0524
	b .L_080e4ade
	ldr r0, [pc, #100]
	ldr r1, [pc, #108]
	movs r2, #1
	movs r3, #1
	bl sub_080e0524
.L_080e4ade:
	ldr r3, [sp, #92]
	movs r5, #239
	lsls r5, r5, #7
	adds r2, r3, r5
	movs r3, #2
	str r3, [r2, #0]
	ldr r0, [sp, #96]
	cmp r0, #12
	bne sub_080e4b4c
	ldr r1, [sp, #92]
	ldr r3, [pc, #84]
	adds r2, r1, r3
	movs r3, #75
	b sub_080e4b54
	.balign 4, 0
	.global LiteralPool_080e4afc
LiteralPool_080e4afc:
	.4byte 0x0000007d
	.4byte 0x000000a9
	.4byte 0x000000ce
	.4byte 0x02010000
	.4byte 0x000000c4
	.4byte 0x02010c56
	.4byte 0x00000079
	.4byte 0x000000c3
	.4byte 0x0000006f
	.4byte 0x000000b8
	.4byte 0x000000b4
	.4byte 0x00000053
	.4byte 0x0000009e
	.4byte 0x080e4924
	.4byte 0x00000094
	.4byte 0x00000092
	.4byte 0x0000008e
	.4byte 0x00000090
	.4byte 0x02013c56
	.4byte 0x00007784
