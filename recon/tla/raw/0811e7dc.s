.syntax unified
	.thumb
	.global Func_0811e7dc
	.thumb_func
Func_0811e7dc:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #48
	ldrb r0, [r3]
	bl GetBattleObjectSlot
	ldr r3, [r5, #44]
	ldr r6, [r0]
	cmp r3, #0
	beq .L_0811e82c
	movs r1, #0
	adds r0, r6, #0
	bl GetMotionRecord
	bl Func_08020370
	movs r0, #1
	bl WaitFrames
	ldr r0, [r5, #44]
	bl Sys_Free
	movs r3, #0
	movs r1, #0
	adds r0, r6, #0
	str r3, [r5, #44]
	bl GetMotionRecord
	ldrb r2, [r0, #26]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0, #26]
	movs r1, #0
	adds r0, r6, #0
	bl GetMotionRecord
	movs r1, #0
	bl ResourceMetadata_RegisterFar + 0x10
.L_0811e82c:
	pop {r5, r6, pc}
	.2byte 0x0000
