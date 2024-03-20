#include <stdio.h>
#include <proto/dos.h>
#include <exec/types.h>
#include <proto/intuition.h>
#include <intuition/intuition.h>
#include <proto/exec.h>

#include <proto/gadtools.h>

struct IntuitionBase *IntuitionBase;
struct Window *win;
struct IntuiMessage *msg;

void Close_All() {
    if(win)	CloseWindow(win);
    if(IntuitionBase) CloseLibrary((struct Library *)IntuitionBase);
};

BOOL RUNNING() {
	BOOL close;
	close = FALSE;
	while(!close) {
		Wait(1L << win->UserPort->mp_SigBit);
		msg = GT_GetIMsg(win->UserPort);
		GT_ReplyIMsg(msg);
		if(msg->Class == IDCMP_CLOSEWINDOW) {
			close = TRUE;
		};
	};

	return FALSE;
}


main() {
	if(!(IntuitionBase = (struct IntuitionBase *) OpenLibrary("intuition.library", 0L))) {
   	printf("Intuition library not found!\n");
      Close_All();
      return 1;
   };
	if(!(win = OpenWindowTags(NULL,
   	WA_Left, 20,
    	WA_Top, 20,
    	WA_Width, 300,
    	WA_Height, 300,
    	WA_MinWidth, 50,
    	WA_MinHeight, 50,
    	WA_MaxWidth, ~0,
    	WA_MaxHeight, ~0,
    	WA_IDCMP, IDCMP_CLOSEWINDOW,
    	WA_Flags, WFLG_SIZEGADGET | WFLG_DRAGBAR | WFLG_DEPTHGADGET |
    		WFLG_CLOSEGADGET | WFLG_SMART_REFRESH | WFLG_ACTIVATE,
    	WA_Title, "Window",
    	WA_PubScreenName, "Workbench",
    	TAG_DONE))) {
     	printf("Window will not open\n!");
     	Close_All();
     	return 1;
	};

	RUNNING();

   Close_All();

   exit(TRUE);
};
