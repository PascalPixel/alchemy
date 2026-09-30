.syntax unified
	.thumb
	.global FieldEvent_RunTypeHandler
	.thumb_func
FieldEvent_RunTypeHandler:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r2, #30
	ldrsh r3, [r3, r2]
	subs r3, #8
	cmp r3, #20
	bhi .L_080db916
	ldr r2, .L_080db918
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_080db8a0:
	.4byte .L_080db8f4
	.4byte .L_080db916
	.4byte .L_080db8fa
	.4byte .L_080db916
	.4byte .L_080db916
	.4byte .L_080db916
	.4byte .L_080db916
	.4byte .L_080db916
	.4byte .L_080db900
	.4byte .L_080db916
	.4byte .L_080db916
	.4byte .L_080db916
	.4byte .L_080db916
	.4byte .L_080db916
	.4byte .L_080db916
	.4byte .L_080db906
	.4byte .L_080db90c
	.4byte .L_080db916
	.4byte .L_080db916
	.4byte .L_080db916
	.4byte .L_080db912
.L_080db8f4:
	bl Func_080dd950
	b .L_080db916
.L_080db8fa:
	bl Func_080debd8
	b .L_080db916
.L_080db900:
	bl Func_080e11bc
	b .L_080db916
.L_080db906:
	bl Func_080e4730
	b .L_080db916
.L_080db90c:
	bl Func_080e9db4
	b .L_080db916
.L_080db912:
	bl Func_080e87c4
.L_080db916:
	pop {pc}
.L_080db918:
	.4byte .L_080db8a0
