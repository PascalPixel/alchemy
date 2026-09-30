.syntax unified
	.thumb
	.global Func_08120178
	.thumb_func
Func_08120178:
	push {r5, lr}
	adds r5, r0, #0
	bl Owner_GetState
	movs r2, #149
	lsls r2, r2, #1
	adds r3, r0, r2
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_081201b2
	movs r1, #0
	bl Owner_RecalculateRatiosFar + 0x8
	adds r0, r5, #0
	bl Func_0811f3b8
	adds r0, r5, #0
	bl Func_0811bc64
	adds r0, r5, #0
	bl GetBattleObjectSlot
	adds r5, r0, #0
	ldr r0, [r5]
	bl Func_080200c8
	movs r3, #0
	str r3, [r5]
	strh r3, [r5, #40]
.L_081201b2:
	pop {r5, pc}
