Rinclude <stdio.h>

int main() {
    extern void asm_main();
    asm_main();
    return 0;
}

void print_message() {
    printf("Hola Mundo\n");
}
