#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "trie.h"

int LIMITE_SUGESTOES = 10; // Valor padrão

void remove_newline(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len-1] == '\n')
        str[len-1] = '\0';
}

// Limpa o console (Windows ou Linux/Unix)
void limpar_console() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printMenu() {
    limpar_console();
    printf("==== MENU ====\n");
    printf("1 - Inserir nova palavra\n");
    printf("2 - Remover palavra\n");
    printf("3 - Entrar em modo autocomplete\n");
    printf("4 - Definir limite de sugestoes (atual: %d)\n", LIMITE_SUGESTOES);
    printf("0 - Sair\n");
    printf("Digite uma opcao: ");
}

void modo_autocomplete(TrieNode* trie) {
    char palavra[100];
    while (1) {
        limpar_console();
        printf("=== Modo Autocomplete ===\n");
        printf("Digite ENTER vazio ou 'voltar' para sair deste modo.\n");
        printf("Palavra: ");
        if (fgets(palavra, sizeof(palavra), stdin) == NULL) break;
        remove_newline(palavra);
        if (palavra[0] == '\0' || strcmp(palavra, "voltar") == 0) break;

        int valido = 1;
        for (int i = 0; palavra[i] != '\0'; i++) {
            if (!isalpha((unsigned char)palavra[i])) {
                valido = 0;
                break;
            }
        }
        if (!valido) {
            printf("Digite apenas letras (sem numeros, acentos ou caracteres especiais)!\n");
            printf("Pressione ENTER para continuar...");
            fgets(palavra, sizeof(palavra), stdin);
            continue;
        }

        printf("Sugestoes para \"%s\":\n", palavra);
        mostrarSugestoes(trie, palavra, LIMITE_SUGESTOES);
        printf("\nPressione ENTER para continuar...");
        fgets(palavra, sizeof(palavra), stdin);
    }
    limpar_console();
    printf("Saindo do modo autocomplete...\n");
    printf("Pressione ENTER para voltar ao menu...");
    fgets(palavra, sizeof(palavra), stdin);
}

int main() {
    TrieNode* trie = criarTrie();
    carregarArquivoParaTrie(trie);
    char entrada[100];

    while (1) {
        printMenu();
        if (fgets(entrada, sizeof(entrada), stdin) == NULL) break;
        remove_newline(entrada);

        if (strcmp(entrada, "1") == 0) {
            limpar_console();
            printf("Digite a palavra para inserir: ");
            if (fgets(entrada, sizeof(entrada), stdin) == NULL) continue;
            remove_newline(entrada);
            inserirPalavra(trie, entrada);
            printf("Palavra '%s' inserida com sucesso!\n", entrada);
            printf("Pressione ENTER para voltar ao menu...");
            fgets(entrada, sizeof(entrada), stdin);
        } else if (strcmp(entrada, "2") == 0) {
            limpar_console();
            printf("Digite a palavra para remover: ");
            if (fgets(entrada, sizeof(entrada), stdin) == NULL) continue;
            remove_newline(entrada);
            removerPalavraArquivo(trie, entrada);
            printf("Palavra '%s' removida (se existia).\n", entrada);
            printf("Pressione ENTER para voltar ao menu...");
            fgets(entrada, sizeof(entrada), stdin);
        } else if (strcmp(entrada, "3") == 0) {
            modo_autocomplete(trie);
        } else if (strcmp(entrada, "4") == 0) {
            limpar_console();
            printf("Digite o novo limite de sugestoes (0 para todas): ");
            if (fgets(entrada, sizeof(entrada), stdin) == NULL) continue;
            LIMITE_SUGESTOES = atoi(entrada);
            if (LIMITE_SUGESTOES < 0) LIMITE_SUGESTOES = 0;
            printf("Limite ajustado para %d.\n", LIMITE_SUGESTOES);
            printf("Pressione ENTER para voltar ao menu...");
            fgets(entrada, sizeof(entrada), stdin);
        } else if (strcmp(entrada, "0") == 0) {
            limpar_console();
            printf("Encerrando...\n");
            break;
        } else {
            limpar_console();
            printf("Opcao invalida!\n");
            printf("Pressione ENTER para voltar ao menu...");
            fgets(entrada, sizeof(entrada), stdin);
        }
    }
    liberarTrie(trie);
    return 0;
}