// vim: set noet ts=4 sw=4:

#include "user/app1/appl.h"

void Application::action() {
    kout << "Called " << __FUNCTION__ << "() in " << __FILE__ << endl;
}
