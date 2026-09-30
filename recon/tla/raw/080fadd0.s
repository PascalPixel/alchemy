.syntax unified
	.thumb
	.global Func_080fadd0
	.thumb_func
Func_080fadd0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	mov r10, r0
	adds r5, r3, #0
	mov r8, r1
	adds r5, #76
	mov r6, r10
	movs r7, #14
.L_080fadec:
	ldrh r1, [r6]
	adds r6, #2
	cmp r1, #0
	beq .L_080fae14
	mov r3, r8
	cmp r3, #0
	bne .L_080fae08
	ldr r3, [r5]
	movs r0, #2
	ldrb r2, [r3, #14]
	movs r3, #0
	bl Func_08038288
	b .L_080fae14
.L_080fae08:
	ldr r3, [r5]
	movs r0, #7
	ldrb r2, [r3, #14]
	movs r3, #0
	bl Func_08038288
.L_080fae14:
	subs r7, #1
	adds r5, #4
	cmp r7, #0
	bge .L_080fadec
	mov r0, r10
	bl Func_080facd8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
