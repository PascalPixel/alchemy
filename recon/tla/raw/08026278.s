@ Script_ApplyOperandSet, Script_ApplyOperandAdd and Script_ApplyOperandCompare,
@ not yet C: see recon/tla/en/main/08026278.c.
.syntax unified
	.thumb
	.align 2, 0
	.global Script_ApplyOperandSet
	.thumb_func
Script_ApplyOperandSet:
	push {r5, lr}
	adds r5, r0, #0
	movs r3, #4
	ldrsh r2, [r5, r3]
	ldr r3, [r5]
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r1, r3, #4
	ldr r3, [r1]
	ldr r2, [pc, #32]
	lsls r3, r3, #2
	ldr r3, [r2, r3]
	ldrh r0, [r5, #4]
	cmp r3, #0
	beq .L0
	ldr r2, [r1, #4]
	adds r0, r5, #0
	movs r1, #0
	mov lr, r3
	.2byte 0xf800
	.2byte 0x88a8
.L0:
	adds r3, r0, #3
	strh r3, [r5, #4]
	movs r0, #1
	pop {r5, pc}
	.2byte 0x0000
	.4byte 0x0802f2dc
	.2byte 0xb520
	.2byte 0x1c05
	.2byte 0x2304
	.2byte 0x5eea
	.2byte 0x682b
	.2byte 0x0092
	.2byte 0x189b
	.2byte 0x1d19
	.2byte 0x680b
	.2byte 0x4a08
	.2byte 0x009b
	.2byte 0x58d3
	.2byte 0x88a8
	.2byte 0x2b00
	.2byte 0xd005
	.2byte 0x684a
	.2byte 0x1c28
	.2byte 0x2101
	.2byte 0x469e
	.2byte 0xf800
	.2byte 0x88a8
	.2byte 0x1cc3
	.2byte 0x80ab
	.2byte 0x2001
	.2byte 0xbd20
	.2byte 0x0000
	.2byte 0xf2dc
	.2byte 0x0802
	.2byte 0xb520
	.2byte 0x1c05
	.2byte 0x2304
	.2byte 0x5eea
	.2byte 0x682b
	.2byte 0x0092
	.2byte 0x189b
	.2byte 0x1d19
	.2byte 0x680b
	.2byte 0x4a08
	.2byte 0x009b
	.2byte 0x58d3
	.2byte 0x88a8
	.2byte 0x2b00
	.2byte 0xd005
	.2byte 0x684a
	.2byte 0x1c28
	.2byte 0x2102
	.2byte 0x469e
	.2byte 0xf800
	.2byte 0x88a8
	.2byte 0x1cc3
	.2byte 0x80ab
	.2byte 0x2001
	.2byte 0xbd20
	.2byte 0x0000
	.2byte 0xf2dc
	.2byte 0x0802
