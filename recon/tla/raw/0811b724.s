.syntax unified
	.thumb
	.global Func_0811b724
	.thumb_func
Func_0811b724:
	push {r5, r6, lr}
	bl GetBattleObjectSlot
	cmp r0, #0
	beq .L_0811b758
	ldr r5, [r0]
	cmp r5, #0
	beq .L_0811b758
	movs r3, #0
	movs r6, #0
	str r3, [r0, #32]
	str r3, [r0, #36]
	b .L_0811b744
.L_0811b73e:
	bl Func_08020040 + 0x8
	adds r6, #1
.L_0811b744:
	adds r0, r5, #0
	adds r1, r6, #0
	bl GetMotionRecord
	cmp r0, #0
	bne .L_0811b73e
	adds r3, r5, #0
	adds r3, #84
	strb r0, [r3]
	str r0, [r5, #80]
.L_0811b758:
	pop {r5, r6, pc}
	.2byte 0x0000
