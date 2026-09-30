.syntax unified
	.thumb
	.global Func_0810526c
	.thumb_func
Func_0810526c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r6, [r3]
	movs r5, #136
	movs r3, #0
	mov r8, r3
	lsls r5, r5, #2
	movs r7, #3
.L_08105284:
	ldr r0, [r5, r6]
	cmp r0, #0
	beq .L_08105292
	bl Func_08020048
	mov r3, r8
	str r3, [r5, r6]
.L_08105292:
	subs r7, #1
	adds r5, #4
	cmp r7, #0
	bge .L_08105284
	ldr r0, .L_081052a8
	bl Func_08014644
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081052a8:
	.4byte Func_081050cc
