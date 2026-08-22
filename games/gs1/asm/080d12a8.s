@ 親関数の終端断片。
@ 後続するリテラルプールを同一ファイルに保持する。
.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_08004278, 0x08004278
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d1016, 0x080d1016
	.set sub_080d105c, 0x080d105c
	.set sub_080d6888, 0x080d6888
	.set sub_080e155c, 0x080e155c
	.set sub_080f9010, 0x080f9010
	.global Fragment_080d12a8
Fragment_080d12a8:
	movs r0, #134
	bl sub_080f9010
	ldr r4, [pc, #144]
	ldr r2, [sp, #76]
	adds r3, r2, r4
	ldr r3, [r3, #0]
	movs r1, #36
	ldrsh r0, [r3, r1]
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl sub_080d6888
	ldr r3, [sp, #76]
	ldr r4, [pc, #120]
	adds r2, r3, r4
	movs r3, #4
	str r3, [r2, #0]
	ldr r0, [sp, #24]
	ldr r1, [sp, #20]
	ldr r2, [sp, #16]
	ldr r3, [sp, #56]
	adds r0, #10
	adds r1, #28
	adds r2, #12
	adds r3, #1
	str r0, [sp, #24]
	str r1, [sp, #20]
	str r2, [sp, #16]
	str r3, [sp, #56]
	cmp r3, #3
	beq .L_080d12f0
	b sub_080d105c
.L_080d12f0:
	movs r1, #8
	movs r0, #8
	bl sub_080e155c
	bl sub_080cd52c
	ldr r0, [pc, #72]
	ldr r4, [sp, #76]
	movs r3, #1
	adds r2, r4, r0
	str r3, [r2, #0]
	movs r0, #1
	bl sub_080030f8
	ldr r1, [sp, #68]
	adds r1, #1
	str r1, [sp, #68]
	cmp r1, #60
	beq .L_080d1318
	b sub_080d1016
.L_080d1318:
	ldr r0, [pc, #48]
	bl sub_08004278
	movs r0, #47
	bl sub_08002dd8
	movs r0, #46
	bl sub_08002dd8
	bl sub_080cdbc0
	add sp, #116
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0

	.global LiteralPool_080d1340
LiteralPool_080d1340:
	.4byte 0x00007828
	.4byte 0x000077a8
	.4byte 0x00007824
	.4byte 0x080cd261
