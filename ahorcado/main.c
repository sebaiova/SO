#include <stdio.h>
#include <stdlib.h>	/* para las funciones system y exit */
#include <string.h>
#include <time.h>

#include "diccionario/diccionario.h"

char veiled_word_s[DIC_WORD_MAX_SIZE];

enum bool { false, true };

void DIC_GetRandomWord(const char* word[], char* veiled_word[], size_t* word_size){
    srand(time(0));
    *word = WORDS[rand()%DIC_SIZE];
    *word_size = strlen(*word);
    *veiled_word = veiled_word_s;

    memset(veiled_word_s, '_', *word_size);
    veiled_word_s[*word_size+1] = '\0';
}


void print_state(const char veiled_word[], const int tries_left)
{
    printf("\r                                                                ");
    printf("\r < %s >\t%i intentos restantes.", veiled_word, tries_left);
}

enum bool try_letter(const char word[], char veiled_word[], char letter)
{
    enum bool success = false;
    while(*word != '\0') {
        if(*word==letter) {
            *veiled_word = letter;
            success = true;
        }
        word++;
        veiled_word++;
    }
    return success;
}

enum bool is_unveiled(char veiled_word[])
{
    enum bool success = true;
    while(*veiled_word++!='\0')
        if(*veiled_word=='_')
            success = false;
    return success;
}

int main() {

    /* Decirle al sistema que el modo input es RAW */
    system("/bin/stty raw && tput civis");
    printf("\nIngrese una letra (0 para salir)\n\n");

    const char* word = NULL;
    char* veiled_word = NULL;
    size_t word_size;
    DIC_GetRandomWord(&word, &veiled_word, &word_size);
    int tries_left = 6;

    while(true) {
        print_state(veiled_word, tries_left);
        char c = getchar();
        if (c == '0')
            break;

        if(!try_letter(word, veiled_word, c)) {
            if(!--tries_left) {
                print_state("GAME OVER", tries_left);
                break;
            }
        }

        else if(is_unveiled(veiled_word)) {
            print_state("YOU WON", tries_left);
            break;
        }
    }

    system ("/bin/stty sane erase ^H");
    printf("\n\n");
}
