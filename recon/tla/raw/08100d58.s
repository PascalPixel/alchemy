.syntax unified
	.thumb
	.global Func_08100d58
	.thumb_func
Func_08100d58:
	push {lr}
	bl BattleAction_Get
	ldrb r3, [r0, #1]
	movs r2, #15
	ands r2, r3
	cmp r2, #1
	beq .L_08100d6e
	cmp r2, #11
	beq .L_08100d74
	b .L_08100d7c
.L_08100d6e:
	movs r0, #126
	bl Audio_PlayCueReturnOne
.L_08100d74:
	movs r0, #126
	bl Audio_PlayCueReturnOne
	b .L_08100e22
.L_08100d7c:
	ldrb r3, [r0, #3]
	subs r0, r3, #1
	cmp r0, #31
	bhi .L_08100e1c
	ldr r2, .L_08100e24
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_08100d8c:
	.4byte .L_08100e22
	.4byte .L_08100e22
	.4byte .L_08100e14
	.4byte .L_08100e1c
	.4byte .L_08100e0c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e1c
	.4byte .L_08100e22
	.4byte .L_08100e22
.L_08100e0c:
	movs r0, #82
	bl Audio_PlayCueReturnOne
	b .L_08100e22
.L_08100e14:
	movs r0, #84
	bl Audio_PlayCueReturnOne
	b .L_08100e22
.L_08100e1c:
	movs r0, #91
	bl Audio_PlayCueReturnOne
.L_08100e22:
	pop {pc}
.L_08100e24:
	.4byte .L_08100d8c
