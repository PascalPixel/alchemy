.syntax unified
	.thumb
	.global Func_08043b34
	.thumb_func
Func_08043b34:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r0, .L_08043b6c
	ldr r7, [r3]
	bl Func_08014644
	movs r3, #0
	mov r8, r3
	movs r5, #248
	movs r6, #3
.L_08043b50:
	ldr r0, [r5, r7]
	cmp r0, #0
	beq .L_08043b5e
	bl Func_08020048
	mov r3, r8
	str r3, [r5, r7]
.L_08043b5e:
	subs r6, #1
	adds r5, #4
	cmp r6, #0
	bge .L_08043b50
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_08043b6c:
	.4byte Func_08043b70
