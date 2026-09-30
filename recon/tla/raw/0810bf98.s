.syntax unified
	.thumb
	.global Func_0810bf98
	.thumb_func
Func_0810bf98:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	adds r0, r3, #0
	mov r8, r2
	adds r6, r1, #0
	bl Owner_GetState
	movs r2, #12
	ldrsh r3, [r5, r2]
	adds r0, #216
	lsls r3, r3, #3
	adds r3, r3, r6
	adds r3, #8
	mov r10, r3
	movs r2, #14
	ldrsh r3, [r5, r2]
	movs r1, #15
	lsls r3, r3, #3
	add r3, r8
	movs r2, #27
	adds r7, r3, #0
	bl Func_0810bebc
	adds r7, #8
	movs r6, #0
.L_0810bfd0:
	adds r0, r6, #0
	movs r1, #5
	bl Math_Mod
	movs r1, #5
	adds r5, r0, #0
	adds r0, r6, #0
	bl __divsi3
	lsls r5, r5, #4
	adds r2, r0, #0
	add r5, r10
	lsls r2, r2, #4
	adds r0, r6, #0
	adds r2, r7, r2
	adds r1, r5, #0
	adds r6, #1
	bl Func_0810be70
	cmp r6, #14
	ble .L_0810bfd0
	movs r0, #1
	bl Func_0810bea8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
