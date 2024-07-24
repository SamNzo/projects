#include <stdio.h>
#include <windows.h>

char s1[] = "Original code\n";
char s2[] = "Modified code\n";
int (*printf_ptr)(const char *const _Format, ...) = printf;
const char* absolute_s1 = s1;
const char* absolute_s2 = s2;

unsigned char bytes[] = {
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x04, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00
};

void f1() {
    printf_ptr(absolute_s1);
}

int main(int argc, char **argv) {
    f1();
    void (*addr)() = f1;

    DWORD oldProtect;
    SIZE_T size = 29;

    VirtualProtect(addr, size, PAGE_READWRITE, &oldProtect);
    unsigned char* code = (unsigned char*) addr;
    for (SIZE_T i = 0; i < size; i++) {
        code[i] ^= bytes[i];  // XOR operation
    }
    VirtualProtect(addr, size, oldProtect, &oldProtect);

    f1();
}
