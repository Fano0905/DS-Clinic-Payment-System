#include "exposition.h"
#include <wchar.h>
#include <locale.h>
#include <windows.h>

int main(void)
{
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "");
    run_application();
    return 0;
}
