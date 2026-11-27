/* @file */
/* The debug menus' destination row. */
/* Part of screens.c. */

#include "field/sequence_stubs.h"

/* Where the debug destination row's colons and its second column (the */
/* entrance number and the area name) start, in pixels; further right in */
/* Japanese. */
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define DEBUG_ROW_COLON_X  100
#define DEBUG_ROW_SECOND_X 110
#else
#define DEBUG_ROW_COLON_X  74
#define DEBUG_ROW_SECOND_X 82
#endif

/* Draws the debug scene and entrance numbers with their scene code and area */
/* name. The first line has the scene's code (MsgDebugSceneCode + scene) */
/* and, beside it, the area its entrance leads to (MsgDebugAreaName + area); */
/* the numbers, three digits each, go under them. */
void mnDebugSceneSelectDrawRow(void *window, s16 scene, const s16 *entrance) {
	s16 selected = scene;
	s32 label =
		fdAreaGetNameIndexStub(selected, *entrance) + (s32)MsgDebugAreaName;

	mnWindowSprPrepareForRedraw(window);
	mnTextDrawNum(selected, 3, window, 0, 14);
	mnTextDrawNum(*entrance, 3, window, DEBUG_ROW_SECOND_X, 14);
	mnTextDrawString(Menu_ColonString, window, DEBUG_ROW_COLON_X, 0);
	mnTextDrawMessage(selected + (s32)MsgDebugSceneCode, window, 0, 0);
	mnTextDrawString(Menu_ColonString, window, DEBUG_ROW_COLON_X, 14);
	mnTextDrawMessage(label, window, DEBUG_ROW_SECOND_X, 0);
}

/* The debug destination menu: picks a scene and an entrance, starting from */
/* the scene the party came from, and sends the party there with A; returns */
/* -1 after setting the destination and -2 when B cancels. */
s16 mnDebugSceneSelectRun(void) {
	s32 arrowSlot;
	s16 field;
	s16 entrance;
	MENU_SPR arrow;
	s16 scene;
	WINDOW *window;
	s16 result;

	window = 0;
	field = 0;
	scene = MainRoutineStateWork.returnScene;
	entrance = MainRoutineStateWork.returnEntrance;
	window = mnWindowCreate(0, 7, 30, 5, WINDOW_FLAG_INSTANT);
	mnDebugSceneSelectDrawRow(window, scene, &entrance);
	mnArrowCreate(&arrow, &arrowSlot);

	while(InputWork.held != 0)
		sySynch(1);

	for(;;) {
		result = (s16)mnDebugSceneSelectHandleRowInput(window, scene, &entrance,
													   &field);
		if(result == -1) {
			mnArrowRelease(arrowSlot);
			mnWindowClose(window, 2);
			fdEventSetDestinationStub(scene, entrance);
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
