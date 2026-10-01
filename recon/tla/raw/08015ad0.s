.syntax unified
	.thumb
	.global Func_08015ad0
	.thumb_func
Func_08015ad0:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #204
	ldr r2, [r3]
	sub sp, #60
	movs r6, #0
	movs r5, #0
	mov r1, sp
.L_08015ae2:
	ldrb r3, [r2]
	adds r2, #3
	cmp r3, #0
	bne .L_08015aee
	stmia r1!, {r5}
	adds r6, #1
.L_08015aee:
	adds r5, #3
	cmp r5, #14
	bls .L_08015ae2
	movs r5, #15
	cmp r6, #0
	beq .L_08015b1e
	cmp r6, #1
	bne .L_08015b0c
	ldr r5, [sp, #0]
	bl Func_08015f0c
	cmp r0, #15
	bne .L_08015b1e
	movs r5, #15
	b .L_08015b1e
.L_08015b0c:
	bl Random16
	adds r1, r6, #0
	bl __umodsi3
	adds r5, r0, #0
	mov r2, sp
	lsls r3, r5, #2
	ldr r5, [r2, r3]
.L_08015b1e:
	adds r0, r5, #0
	add sp, #60
	pop {r5, r6, pc}
