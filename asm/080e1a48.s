@ 親関数のスタック枠と高位レジスタを引き継ぐ内部断片。
@ 後続52バイトは親関数と共有する定数・リテラル表。
.syntax unified
	.thumb
	.set sub_080e19b2, 0x080e19b2
	.set sub_080e1acc, 0x080e1acc
	.set sub_080e1ae2, 0x080e1ae2
	.set sub_080e1b8e, 0x080e1b8e
	.set sub_080ed408, 0x080ed408
	.global Fragment_080e1a48
Fragment_080e1a48:
	ldr r1, [sp, #40]
	ldr r2, [sp, #36]
	ldr r3, [sp, #32]
	movs r4, #1
	add r9, r4
	adds r1, #24
	adds r2, #24
	subs r3, #48
	mov r5, r9
	str r1, [sp, #40]
	str r2, [sp, #36]
	str r3, [sp, #32]
	cmp r5, #3
	bne sub_080e19b2
	ldr r0, [sp, #116]
	cmp r0, #179
	ble .L_080e1a6c
	b sub_080e1b8e
.L_080e1a6c:
	movs r5, #0
	cmp r0, #155
	ble .L_080e1a76
	adds r5, r0, #0
	subs r5, #156
.L_080e1a76:
	cmp r5, #7
	ble .L_080e1a7c
	movs r5, #7
.L_080e1a7c:
	ldr r1, [sp, #116]
	cmp r1, #139
	bgt sub_080e1acc
	movs r3, #3
	movs r2, #7
	movs r0, #47
	movs r1, #7
	str r3, [sp, #0]
	bl sub_080ed408
	ldr r2, [pc, #52]
	ldr r2, [r2, #0]
	str r2, [sp, #88]
	b sub_080e1ae2
	.global LiteralPool_080e1a98
LiteralPool_080e1a98:
	.4byte 0x00005555
	.4byte 0x0000aaab
	.4byte 0x00007784
	.4byte 0x1a1a1a1a
	.4byte 0x0300013c
	.4byte 0x7fff0000
	.4byte 0xfffff000
	.4byte 0x080eda98
	.4byte 0xffffff00
	.4byte 0x03000118
	.4byte 0x00000555
	.4byte 0x080ede48
	.4byte 0x03001f0c
