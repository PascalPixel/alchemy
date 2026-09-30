.syntax unified
	.thumb
	.global Func_0810824c
	.thumb_func
Func_0810824c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r5, [r3]
	bl Func_0810be3c
	ldr r0, .L_081082c0
	bl Func_08014644
	bl Func_08038140
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #236
	adds r3, r5, r2
	ldrh r0, [r3]
	bl Func_08014274
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #238
	adds r3, r5, r2
	ldrh r0, [r3]
	bl Func_08014274
	movs r2, #158
	lsls r2, r2, #3
	adds r3, r5, r2
	ldrh r0, [r3]
	bl Func_08014274
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #242
	adds r3, r5, r2
	ldrh r0, [r3]
	bl Func_08014274
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #244
	adds r3, r5, r2
	ldrh r0, [r3]
	bl Func_08014274
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #246
	adds r5, r5, r3
	ldrh r0, [r5]
	bl Func_08014274
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	pop {r5, pc}
	.2byte 0x0000
.L_081082c0:
	.4byte Func_08108130
