@ Far-call stub table heading the system runtime at 0x080000c0: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080000c0
	.thumb_func
Func_080000c0:
	.irp target, 0x080030f9, 0x080040e9, 0x080041d9, 0x08004279
	overlay_veneer \target
	.endr
	.irp target, 0x080042c9, 0x0800439d, 0x08004421, 0x08004459
	overlay_veneer \target
	.endr
	.irp target, 0x080044d1, 0x080045a5, 0x080045d5, 0x08002323
	overlay_veneer \target
	.endr
	.irp target, 0x0800231d, 0x0800447d, 0x0800307d, 0x08004859
	overlay_veneer \target
	.endr
	.irp target, 0x080048b1, 0x080048f5, 0x08002dd9, 0x0800488d
	overlay_veneer \target
	.endr
	.irp target, 0x080048a1, 0x08004939, 0x08004971, 0x08002df1
	overlay_veneer \target
	.endr
	.irp target, 0x080046c5, 0x08004719, 0x0800473d, 0x08004699
	overlay_veneer \target
	.endr
	.irp target, 0x080053e9, 0x08005341, 0x0800403d, 0x08003f3d
	overlay_veneer \target
	.endr
	.irp target, 0x08003f79, 0x08003fa5, 0x08004081, 0x08004761
	overlay_veneer \target
	.endr
	.irp target, 0x08003d29, 0x08003ded, 0x0800479d, 0x08004839
	overlay_veneer \target
	.endr
	.irp target, 0x080049ad, 0x08004a95, 0x080049e9, 0x08004a5d
	overlay_veneer \target
	.endr
	.irp target, 0x08004ab1, 0x08004bd5, 0x08004c1d, 0x08004c6d
	overlay_veneer \target
	.endr
	.irp target, 0x08004cb5, 0x08004cf1, 0x08004d2d, 0x08004e55
	overlay_veneer \target
	.endr
	.irp target, 0x080051d9, 0x08005269, 0x08005209, 0x08005259
	overlay_veneer \target
	.endr
	.irp target, 0x08002f11, 0x08002f0d, 0x08002f41, 0x08002f3d
	overlay_veneer \target
	.endr
	.irp target, 0x08002fb1, 0x08003b71, 0x08003bb5, 0x08003bf9
	overlay_veneer \target
	.endr
	.irp target, 0x08003c3d, 0x08003c81, 0x08003ce1, 0x08002ef9
	overlay_veneer \target
	.endr
	.irp target, 0x08002efd, 0x08002f01, 0x08002f05, 0x08002f09
	overlay_veneer \target
	.endr
	.irp target, 0x08005d11, 0x08005ee1, 0x0800383d, 0x0800387d
	overlay_veneer \target
	.endr
	.irp target, 0x080038bd, 0x080038fd, 0x0800393d, 0x0800397d
	overlay_veneer \target
	.endr
	.irp target, 0x080039bd, 0x080039fd, 0x08003a3d, 0x0800352d
	overlay_veneer \target
	.endr
	.irp target, 0x080051e9, 0x08006385, 0x08006359, 0x080037d5
	overlay_veneer \target
	.endr
	.irp target, 0x080063bd, 0x08006409, 0x08006459, 0x08006489
	overlay_veneer \target
	.endr
	.irp target, 0x080064b9, 0x080064f5, 0x0800655d, 0x08004359
	overlay_veneer \target
	.endr
