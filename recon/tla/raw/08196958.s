.syntax unified
	.thumb
	.global Func_08196958
	.thumb_func
Func_08196958:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	mov r8, r0
	mov r10, r1
	mov r9, r2
	ldr r5, .L_081969a4
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	movs r3, #128
	adds r6, r0, #0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_081969a8
	adds r1, r6, #0
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r0, r8
	mov r1, r10
	mov r2, r9
	mov lr, r6
	.2byte 0xf800
	adds r0, r6, #0
	bl Sys_Free
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_081969a4:
	.4byte 0x000000b0
.L_081969a8:
	.4byte Data_0813b9a0
