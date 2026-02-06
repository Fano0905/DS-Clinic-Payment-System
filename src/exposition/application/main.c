#include "exposition.h"
#include "mytest.h"
#include <wchar.h>
#include <locale.h>
#include <windows.h>

int main(void)
{
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "");
    run_all_test();
    run_application();
    return 0;
}
