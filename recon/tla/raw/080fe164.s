.syntax unified
	.thumb
	.global Func_080fe164
	.thumb_func
Func_080fe164:
	push {lr}
	lsls r0, r0, #18
	lsrs r0, r0, #18
	bl BattleAction_Get
	ldrb r3, [r0, #6]
	cmp r3, #0
	bne .L_080fe180
	ldrb r2, [r0, #1]
	movs r3, #192
	ands r3, r2
	movs r0, #1
	cmp r3, #192
	bne .L_080fe182
.L_080fe180:
	movs r0, #0
.L_080fe182:
	pop {pc}
