.syntax unified
	.thumb
	.global Func_080e8d80
	.thumb_func
Func_080e8d80:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #92]
	movs r4, #0
	mov r12, r3
	ldr r3, [r3, #80]
	cmp r4, r3
	bge .L_080e8dae
	mov r1, r12
	mov r0, r12
	adds r1, #64
	adds r0, #32
.L_080e8d9a:
	ldmia r0!, {r3}
	adds r4, #1
	ldr r2, [r3, #80]
	ldrh r3, [r1]
	adds r1, #2
	strh r3, [r2, #18]
	mov r2, r12
	ldr r3, [r2, #80]
	cmp r4, r3
	blt .L_080e8d9a
.L_080e8dae:
	pop {pc}
