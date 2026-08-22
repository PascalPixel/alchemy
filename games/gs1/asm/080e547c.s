@ 親関数の分岐表から到達する十個の処理断片。
@ 後続56バイトは親関数と共有するリテラル表。
.syntax unified
	.thumb
	.set sub_080e46f0, 0x080e46f0
	.set sub_080e551a, 0x080e551a
	.global Fragment_080e547c
Fragment_080e547c:
	ldr r0, [pc, #92]
	bl sub_080e46f0
	b sub_080e551a
	ldr r0, [pc, #88]
	bl sub_080e46f0
	b sub_080e551a
	ldr r0, [pc, #84]
	bl sub_080e46f0
	b sub_080e551a
	ldr r0, [pc, #80]
	bl sub_080e46f0
	b sub_080e551a
	ldr r0, [pc, #76]
	bl sub_080e46f0
	b sub_080e551a
	ldr r0, [pc, #72]
	bl sub_080e46f0
	b sub_080e551a
	ldr r0, [pc, #68]
	bl sub_080e46f0
	b sub_080e551a
	ldr r0, [pc, #64]
	bl sub_080e46f0
	b sub_080e551a
	ldr r0, [pc, #60]
	bl sub_080e46f0
	b sub_080e551a
	ldr r0, [pc, #56]
	bl sub_080e46f0
	b sub_080e551a
	.global LiteralPool_080e54cc
LiteralPool_080e54cc:
	.4byte 0x02014000
	.4byte 0x080dbb9d
	.4byte 0x00007828
	.4byte 0x080e53f4
	.4byte 0x00000053
	.4byte 0x0000006f
	.4byte 0x00000079
	.4byte 0x000000c3
	.4byte 0x0000008d
	.4byte 0x000000bb
	.4byte 0x000000b9
	.4byte 0x000000c0
	.4byte 0x000000a4
	.4byte 0x000000a3
