/* The debug menus' destination row. Part of screens.c. */

#include "field/mode_stubs.h"

/* Where the debug destination row's colons and its second column (the entrance
   number and the area name) start, in pixels; further right in Japanese. */
#define DEBUG_ROW_COLON_X  JAPANESE_OR(100, 74)
#define DEBUG_ROW_SECOND_X JAPANESE_OR(110, 82)

/* Draws the debug scene and entrance numbers with their scene code and area
   name. The first line has the scene's code (MsgDebugSceneCode + scene) and,
   beside it, the area its entrance leads to (MsgDebugAreaName + area); the
   numbers, three digits each, go under them. */
void mnDebugSceneSelectDrawRow(
	void *window, Sint16 scene, const Sint16 *entrance)
{
	Sint16 selected = scene;
	Sint32 label =
		fdAreaGetNameIndexStub(selected, *entrance) + (Sint32)MsgDebugAreaName;

	mnWindowSprPrepareForRedraw(window);
	mnTextDrawNum(selected, 3, window, 0, 14);
	mnTextDrawNum(*entrance, 3, window, DEBUG_ROW_SECOND_X, 14);
	mnTextDrawString(Menu_ColonString, window, DEBUG_ROW_COLON_X, 0);
	mnTextDrawMessage(selected + (Sint32)MsgDebugSceneCode, window, 0, 0);
	mnTextDrawString(Menu_ColonString, window, DEBUG_ROW_COLON_X, 14);
	mnTextDrawMessage(label, window, DEBUG_ROW_SECOND_X, 0);
}

/* The debug destination menu: picks a scene and an entrance, starting from the
   scene the party came from, and sends the party there with A; returns -1 after
   setting the destination and -2 when B cancels. */
Sint16 mnDebugSceneSelectRun(void)
{
	Sint32	 arrowSlot;
	Sint16	 field;
	Sint16	 entrance;
	MENU_SPR arrow;
	Sint16	 scene;
	WINDOW	*window;
	Sint16	 result;

	window = 0;
	field = 0;
	scene = MainRoutineStateWork.returnScene;
	entrance = MainRoutineStateWork.returnEntrance;
	window = mnWindowCreate(0, 7, 30, 5, WINDOW_FLAG_INSTANT);
	mnDebugSceneSelectDrawRow(window, scene, &entrance);
	mnArrowCreate(&arrow, &arrowSlot);

	while(PerWork.data != 0)
		sySynch(1);

	for(;;) {
		result = (Sint16)mnDebugSceneSelectHandleRowInput(
			window, scene, &entrance, &field);
		if(result == -1) {
			mnArrowRelease(arrowSlot);
			mnWindowClose(window, 2);
			fdScriptSetDestinationStub(scene, entrance);
			return result;
		}
		if(result == -2) {
			mnArrowRelease(arrowSlot);
			mnWindowClose(window, 2);
			return result;
		}

		mnArrowSetPos(&arrow, DEBUG_SCENE_SELECTOR_COLON_X,
			field * 14 + DEBUG_SCENE_SELECTOR_FIRST_ROW_Y);
		scene = result;
		sySynch(1);
	}
}

#undef DEBUG_ROW_COLON_X
#undef DEBUG_ROW_SECOND_X
