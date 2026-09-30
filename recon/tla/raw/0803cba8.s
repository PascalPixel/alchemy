.syntax unified
	.thumb
	.global Func_0803cba8
	.thumb_func
Func_0803cba8:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r5, r0, #0
	ldr r0, .L_0803cbf0
	mov r8, r1
	bl Resource_GetTableEntry
	ldr r3, [r5]
	add r0, r8
	movs r2, #12
	ldrsh r4, [r3, r2]
	movs r2, #14
	ldrsh r5, [r3, r2]
	ldrh r6, [r3, #8]
	ldrh r3, [r3, #10]
	movs r2, #132
	mov r12, r3
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r1, .L_0803cbf4
	adds r2, #64
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r4, #0
	adds r1, r5, #0
	adds r2, r6, #0
	mov r3, r12
	bl Func_0803a1c0
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0803cbf0:
	.4byte 0x000001d5
.L_0803cbf4:
	.4byte 0x06000100
