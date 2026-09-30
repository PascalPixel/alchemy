.syntax unified
	.thumb
	.global Func_080ed030
	.thumb_func
Func_080ed030:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r2, #192
	adds r7, r0, #0
	movs r4, #31
	lsls r2, r2, #2
	sub sp, #4
	mov r8, r1
	movs r6, #0
	ands r4, r7
	mov r10, r2
.L_080ed04a:
	mov r3, r8
	adds r0, r3, r6
	adds r5, r0, #0
	cmp r0, #23
	ble .L_080ed056
	subs r5, #24
.L_080ed056:
	lsls r0, r0, #5
	adds r0, r4, r0
	mov r1, r10
	adds r0, #128
	str r4, [sp, #0]
	bl __modsi3
	lsls r5, r5, #6
	ldr r2, .L_080ed098
	adds r5, r7, r5
	lsls r5, r5, #6
	adds r1, r0, #0
	adds r0, r5, r2
	ldr r2, .L_080ed09c
	lsls r1, r1, #6
	adds r1, r1, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r6, #1
	ldr r4, [sp, #0]
	cmp r6, #20
	bls .L_080ed04a
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080ed098:
	.4byte gMapCellBuffer
.L_080ed09c:
	.4byte 0x06004000
