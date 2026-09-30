.syntax unified
	.thumb
	.global Func_0813fd40
	.thumb_func
Func_0813fd40:
	push {lr}
	ldr r0, .L_0813fd70
	bl Func_08014644
	ldr r0, .L_0813fd74
	bl Func_08014644
	ldr r0, .L_0813fd78
	bl Func_08014644
	movs r1, #128
	ldr r3, .L_0813fd7c
	lsls r1, r1, #7
	ldr r0, .L_0813fd80
	mov lr, r3
	.2byte 0xf800
	movs r0, #96
	bl Runtime_ReleaseHeapBlock
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	pop {pc}
	.2byte 0x0000
.L_0813fd70:
	.4byte Func_0813f8d4
.L_0813fd74:
	.4byte Func_0813f89c
.L_0813fd78:
	.4byte Func_0813fa44
.L_0813fd7c:
	.4byte IwramClearWords
.L_0813fd80:
	.4byte 0x06004000
