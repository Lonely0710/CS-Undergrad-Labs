#include "FAT12.h"
#include "shell.h"
#include "FAT12.h"
int main()
{
    InitMBR();
    InitFAT();
    CreatRootDict("/");
    CreateFile("/", "usr", "", DIRECTORY_TYPE, 512);
    CreateFile("/", "BJTU", "md", REGULAR_TYPE, 0);
    char txt[30] = "知行";
    char buf[512] = {0};
    WriteFile("/BJTU.md", txt, 30);
    ReadFile("/BJTU.md", buf);
    ExecShell();
}