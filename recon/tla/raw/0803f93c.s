.syntax unified
	.thumb
	.global Func_0803f93c
	.thumb_func
Func_0803f93c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #135
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	cmp r0, #0
	beq .L_0803f962
	ldrh r3, [r0, #22]
	cmp r3, #0
	beq .L_0803f962
	movs r1, #2
	bl UiWork_Finalize
	ldr r0, .L_0803f964
	bl Func_08014644
.L_0803f962:
	pop {pc}
.L_0803f964:
	.4byte Func_0803f900
