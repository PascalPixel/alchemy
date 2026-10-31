.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_08038328
	.thumb_func
Func_08038328:
	overlay_veneer Func_08044a58
	.global Party_ShowJoinedMessageFar
	.thumb_func
Party_ShowJoinedMessageFar:
	overlay_veneer Func_08044b98
	.global Party_ShowPairJoinedMessageFar
	.thumb_func
Party_ShowPairJoinedMessageFar:
	overlay_veneer Party_ShowPairJoinedMessage
	.global Func_08038340
	.thumb_func
Func_08038340:
	overlay_veneer Func_08044dc8
	.global Func_08038348
	.thumb_func
Func_08038348:
	overlay_veneer Func_0803f9c0
	.global Func_08038350
	.thumb_func
Func_08038350:
	overlay_veneer Func_080400e8
