@ 親関数の共有終端断片。
@ 後続するリテラルプールを同一ファイルに保持する。
.syntax unified
	.thumb
	.set sub_08009048, 0x08009048
	.set sub_080090c8, 0x080090c8
	.set sub_080090e0, 0x080090e0
	.global Fragment_0808b7b8
Fragment_0808b7b8:
	ldr r3, [pc, #96]
	movs r0, #249
	lsls r0, r0, #1
	adds r2, r3, r0
	movs r3, #0
	strb r3, [r2, #0]
	movs r0, #128
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	lsls r0, r0, #8
	bl sub_080090c8
	ldr r3, [r5, #20]
	adds r6, r0, #0
	str r3, [r6, #20]
	adds r1, r5, #0
	bl sub_080090e0
	movs r3, #207
	lsls r3, r3, #1
	add r3, r8
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #3
	bne .L_0808b7fc
	ldr r0, [r5, #80]
	movs r1, #23
	bl sub_08009048
	movs r3, #15
	strb r3, [r0, #5]
	movs r3, #9
	strb r3, [r0, #6]
.L_0808b7fc:
	ldr r3, [pc, #32]
	ldr r2, [r3, #0]
	adds r3, r6, #0
	adds r3, #8
	str r3, [r2, #0]
	movs r3, #240
	lsls r3, r3, #1
	add r3, r8
	str r6, [r3, #0]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0

	.balign 4, 0
	.global LiteralPool_0808b81c
LiteralPool_0808b81c:
	.4byte 0x02000240
	.4byte 0x03001e70
