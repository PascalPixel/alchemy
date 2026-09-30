.syntax unified
	.thumb
	.global Func_080fa478
	.thumb_func
Func_080fa478:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r5, [r3]
	bl Func_0810508c
	bl Func_080fa458
	movs r0, #1
	bl WaitFrames
	movs r2, #184
	lsls r2, r2, #1
	adds r3, r5, r2
	ldr r2, [r3]
	adds r0, r5, #0
	movs r3, #13
	strb r3, [r2, #5]
	adds r0, #16
	movs r1, #1
	bl UiWindow_CloseIfOpen
	adds r0, r5, #0
	adds r0, #36
	movs r1, #1
	bl UiWindow_CloseIfOpen
	adds r0, r5, #0
	adds r0, #240
	movs r1, #1
	bl UiWindow_CloseIfOpen
	adds r0, r5, #0
	adds r0, #40
	movs r1, #1
	bl UiWindow_CloseIfOpen
	adds r0, r5, #0
	adds r0, #44
	movs r1, #1
	bl UiWindow_CloseIfOpen
	adds r0, r5, #0
	adds r0, #48
	movs r1, #1
	bl UiWindow_CloseIfOpen
	adds r0, r5, #0
	adds r0, #52
	movs r1, #1
	bl UiWindow_CloseIfOpen
	adds r0, r5, #0
	adds r0, #56
	movs r1, #1
	bl UiWindow_CloseIfOpen
	adds r0, r5, #0
	adds r0, #60
	movs r1, #1
	bl UiWindow_CloseIfOpen
	adds r0, r5, #0
	adds r0, #64
	movs r1, #1
	bl UiWindow_CloseIfOpen
	adds r0, r5, #0
	adds r0, #68
	movs r1, #1
	bl UiWindow_CloseIfOpen
	pop {r5, pc}
