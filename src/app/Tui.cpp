#include "./Tui.h"
#include <curses.h>
#include <cstdlib>

void Tui::finish(int sig)
{
	endwin();
	exit(sig);
}
