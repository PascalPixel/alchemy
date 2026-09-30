.syntax unified
	.thumb
	.global Func_08014d40
	.thumb_func
Func_08014d40:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r5, #192
	lsls r5, r5, #18
	mov r8, r0
	ldr r1, .L_08014d70
	ldr r0, [r5, #4]
	mov r2, r8
	subs r1, r1, r0
	ldr r6, .L_08014d74
	mov lr, r6
	.2byte 0xf800
	ldr r0, [r5]
	movs r1, #129
	lsls r1, r1, #18
	subs r1, r1, r0
	mov r2, r8
	mov lr, r6
	.2byte 0xf800
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_08014d70:
	.4byte IwramSoundMixWorkspace
.L_08014d74:
	.4byte IwramFillWords
