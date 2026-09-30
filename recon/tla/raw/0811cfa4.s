.syntax unified
	.thumb
	.global BattlePres_BuildUnitEntries
	.thumb_func
BattlePres_BuildUnitEntries:
	push {r5, r6, lr}
	adds r5, r0, #0
	movs r0, #17
	bl Runtime_BumpAllocateAlternatePool
	adds r6, r0, #0
	adds r1, r6, #0
	adds r0, r5, #0
	movs r2, #0
	bl Func_080381c0
	movs r5, #1
	negs r5, r5
	cmp r0, #0
	blt .L_0811cfc4
	adds r5, r0, #0
.L_0811cfc4:
	adds r0, r6, #0
	bl Sys_Free
	adds r0, r5, #0
	pop {r5, r6, pc}
	.2byte 0x0000
