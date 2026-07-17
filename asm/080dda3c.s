@ Reconstructed thumb disassembly of a call-graph-reached code region
@ (code interleaved with its data), verified byte-identical by build_asm.
.syntax unified
	.thumb
	.set sub_080041d8, 0x080041d8
	.set sub_080b50e8, 0x080b50e8
	.set sub_080ddb34, 0x080ddb34
	.set sub_080ddcda, 0x080ddcda
	.set sub_080ddd90, 0x080ddd90
	.set sub_080ed408, 0x080ed408
	.global Region_080dda3c
Region_080dda3c:
	movs r5, #1
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r5, [sp, #0]
	bl sub_080ed408
	ldr r2, [r7, #28]
	movs r1, #7
	str r2, [sp, #36]
	movs r3, #7
	movs r2, #7
	movs r0, #47
	str r5, [sp, #0]
	bl sub_080ed408
	movs r3, #239
	lsls r3, r3, #7
	ldr r2, [pc, #176]
	add r3, r9
	str r5, [r3, #0]
	add r2, r9
	movs r3, #0
	ldr r7, [r7, #32]
	movs r1, #144
	str r3, [r2, #0]
	lsls r1, r1, #3
	ldr r0, [pc, #160]
	str r7, [sp, #40]
	bl sub_080041d8
	ldr r3, [r6, #0]
	ldr r2, [pc, #156]
	ldr r3, [r3, #24]
	ldrb r3, [r2, r3]
	lsls r3, r3, #3
	adds r3, #56
	str r3, [sp, #32]
	movs r3, #0
	mov r11, r3
	movs r1, #1
	movs r2, #128
	ldr r3, [pc, #140]
	negs r1, r1
	lsls r2, r2, #3
.L_080dda98:
	movs r4, #1
	add r11, r4
	str r1, [r3, #0]
	adds r3, #28
	cmp r11, r2
	bne .L_080dda98
	ldr r0, [sp, #32]
	movs r6, #0
	str r6, [sp, #44]
	cmp r0, #0
	bne .L_080ddab0
	b sub_080ddd90
.L_080ddab0:
	ldr r1, [sp, #32]
	subs r0, #64
	subs r1, #16
	str r0, [sp, #28]
	str r1, [sp, #24]
	ldr r2, [sp, #44]
	ldr r3, [sp, #28]
	cmp r2, r3
	bne .L_080ddac8
	movs r0, #132
	bl sub_080b50e8
.L_080ddac8:
	ldr r4, [sp, #44]
	ldr r6, [sp, #24]
	cmp r4, r6
	blt .L_080ddae4
	ldr r2, [pc, #80]
	ldr r3, [pc, #56]
	strh r3, [r2, #0]
	ldr r0, [sp, #32]
	ldr r2, [pc, #52]
	subs r3, r0, r4
	ldr r1, [pc, #72]
	subs r3, #1
	orrs r3, r2
	strh r3, [r1, #0]
.L_080ddae4:
	ldr r3, [pc, #68]
	add r3, r9
	ldr r3, [r3, #0]
	ldr r2, [pc, #48]
	ldr r3, [r3, #24]
	ldrb r3, [r2, r3]
	movs r1, #0
	mov r11, r1
	cmp r3, #0
	bne .L_080ddafa
	b sub_080ddcda
.L_080ddafa:
	ldr r4, [pc, #52]
	ldr r6, [sp, #44]
	movs r3, #12
	movs r0, #8
	subs r6, #8
	str r3, [sp, #20]
	str r4, [sp, #16]
	str r0, [sp, #12]
	b sub_080ddb34
