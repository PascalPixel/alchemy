@ Reconstructed thumb disassembly of a call-graph-reached code region
@ (code interleaved with its data), verified byte-identical by build_asm.
.syntax unified
	.thumb
	.set sub_080e0524, 0x080e0524
	.set sub_080e4b4c, 0x080e4b4c
	.set sub_080e4b54, 0x080e4b54
	.global Region_080e4ab8
Region_080e4ab8:
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
