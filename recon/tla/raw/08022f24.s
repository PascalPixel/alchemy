.syntax unified
	.thumb
	.global Func_08022f24
	.thumb_func
Func_08022f24:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldrb r3, [r6, #24]
	ldr r5, [r6, #40]
	mov r8, r3
	cmp r6, #0
	beq .L_08022f5e
	ldrb r3, [r5, #20]
	ldr r2, [r5, #16]
	movs r7, #0
	ldrb r3, [r2, r3]
	cmp r3, #241
	beq .L_08022f5e
.L_08022f42:
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #89
	bgt .L_08022f5e
	ldrb r3, [r5, #20]
	ldr r2, [r5, #16]
	ldrb r3, [r2, r3]
	cmp r3, #241
	beq .L_08022f5e
	ldrb r3, [r6, #24]
	cmp r3, r8
	beq .L_08022f42
.L_08022f5e:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
