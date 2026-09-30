.syntax unified
	.thumb
	.global Func_081005e4
	.thumb_func
Func_081005e4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	sub sp, #4
	adds r6, r3, #0
	mov r10, r0
	mov r8, r1
	adds r7, r2, #0
	movs r5, #0
	adds r6, #76
.L_08100602:
	ldmia r6!, {r0}
	cmp r0, #0
	beq .L_08100614
	adds r1, r5, #0
	mov r2, r10
	mov r3, r8
	str r7, [sp, #0]
	bl Menu_PlaceEntryObjectInGrid
.L_08100614:
	adds r5, #1
	cmp r5, #31
	ble .L_08100602
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
