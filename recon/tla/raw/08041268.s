.syntax unified
	.thumb
	.global Func_08041268
	.thumb_func
Func_08041268:
	push {lr}
	adds r4, r0, #0
	cmp r4, #0
	beq .L_0804127a
	ldr r0, .L_08041284
	adds r0, r4, r0
	bl UiText_DrawCharacterAtOffset
	b .L_08041280
.L_0804127a:
	ldr r0, .L_08041288
	bl UiText_DrawStringInWindow
.L_08041280:
	pop {pc}
	.2byte 0x0000
.L_08041284:
	.4byte 0x000005a7
.L_08041288:
	.4byte Data_0805eb50
