.syntax unified
	.thumb
	.global Func_0803f82c
	.thumb_func
Func_0803f82c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #72]
	movs r1, #192
	lsls r1, r1, #2
	adds r1, #158
	adds r2, r3, r1
	strh r0, [r2]
	movs r2, #238
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #1
	strh r2, [r3]
	bl Func_0803dd98
	bl Func_0803e7ac
	movs r1, #5
	movs r0, #0
	bl Menu_OpenSelectionWindow
	bl Func_0803df00
	movs r0, #1
	bl Func_0803e998
	adds r5, r0, #0
	bl Resource_ResetOwnerEntries
	adds r0, r5, #0
	pop {r5, pc}
