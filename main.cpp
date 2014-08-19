#include <windows.h>
#include <stdio.h>

/* Timer - Temporizador
 *
 * Cronometra, no Windows, o tempo de execução de programas.
 * Nota: não funciona para comandos internos do Windows.
 *
 * Baseado no código de:
 *   http://www.seucurso.com.br/index.php?option=com_content&view=article&id=79:cronometrar-a-execucao-de-programas&catid=917:c-c-c&Itemid=76
 * o qual, por sua vez, foi baseado no código de:
 *   http://paginas.fe.up.pt/~jmcruz/etc/unix/Win32.pdf
 */

char *skip_arg(char *line)
{
 while (*line != '\0' && *line != ' ' && *line != '\t')
 line++;
 while (*line == ' ' || *line == '\t')
 line++;
 return line;
}
int main(void)
{
 STARTUPINFO stup;
 PROCESS_INFORMATION pinfo;
 LONGLONG ct, et, kt, ut, elapse;
 SYSTEMTIME sct, set;
 char *call;
 call = GetCommandLine(); // string com a linha de comando
 call = skip_arg(call); // salta o 1.o argumento
 GetStartupInfo(&stup); // necessário para a criação de um novo processo
 CreateProcess(NULL, call, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL,&stup, &pinfo); // cria novo processo com parâmetros de chamada
 WaitForSingleObject(pinfo.hProcess, INFINITE); // espera que termine
 GetProcessTimes(pinfo.hProcess, (FILETIME *)&ct, (FILETIME *)&et,(FILETIME *)&kt, (FILETIME *)&ut);
 CloseHandle(pinfo.hProcess); // fecha handle do processo terminado
 CloseHandle(pinfo.hThread); // fecha handle do thread terminado
 elapse = et - ct;
 FileTimeToSystemTime((FILETIME *)&ct, &sct);
 FileTimeToSystemTime((FILETIME *)&et, &set);
 printf("\n\nStart time: %02d:%02d:%02d.%03d\n", sct.wHour, sct.wMinute,sct.wSecond, sct.wMilliseconds);
 printf("End time: %02d:%02d:%02d.%03d\n\n", set.wHour, set.wMinute,set.wSecond, set.wMilliseconds);
 printf("ELAPSED: %.3f ms  (%.3f s)\n", (double)elapse/10000, (double)elapse/1E7);
 printf("  Kernel: %.3f ms  (%.3f s)\n", (double)kt/10000, (double)kt/1E7);
 printf("  User: %.3f ms  (%.3f s)\n", (double)ut/10000, (double)ut/1E7);
 return 0;
}

