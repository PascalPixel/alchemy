.syntax unified
	.thumb
	.global Func_081969f8
	.thumb_func
Func_081969f8:
	push {r5, lr}
	adds r5, r0, #0
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, #28
	bl Runtime_BumpAllocateAlternatePool
	adds r3, r5, #1
	adds r1, r0, #0
	cmp r3, #0
	ble .L_08196a24
	movs r3, #0
	adds r2, r1, #0
	adds r0, r5, #1
.L_08196a16:
	subs r0, #1
	str r3, [r2]
	str r3, [r2, #4]
	str r3, [r2, #24]
	adds r2, #28
	cmp r0, #0
	bne .L_08196a16
.L_08196a24:
	adds r0, r1, #0
	pop {r5, pc}
