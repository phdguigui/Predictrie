#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "trie.h"

struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    int is_end_of_word;
};

TrieNode* criarNo(void) {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->is_end_of_word = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        node->children[i] = NULL;
    return node;
}

TrieNode* criarTrie(void) {
    return criarNo();
}

void inserirNaTrie(TrieNode* root, const char* word) {
    TrieNode* curr = root;
    for (int i = 0; word[i] != '\0'; i++) {
        if (!isalpha(word[i])) continue;
        int idx = tolower(word[i]) - 'a';
        if (idx < 0 || idx >= ALPHABET_SIZE) continue;
        if (!curr->children[idx])
            curr->children[idx] = criarNo();
        curr = curr->children[idx];
    }
    curr->is_end_of_word = 1;
}

int buscarPalavra(TrieNode* root, const char* word) {
    TrieNode* curr = root;
    for (int i = 0; word[i] != '\0'; i++) {
        if (!isalpha(word[i])) continue;
        int idx = tolower(word[i]) - 'a';
        if (idx < 0 || idx >= ALPHABET_SIZE) continue;
        if (!curr->children[idx])
            return 0;
        curr = curr->children[idx];
    }
    return curr->is_end_of_word;
}

void carregarArquivoParaTrie(TrieNode* root) {
    FILE* f = fopen(ARQUIVO_PALAVRAS, "r");
    if (!f) return;
    char linha[128];
    while (fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\r\n")] = 0;
        if (linha[0]!='\0')
            inserirNaTrie(root, linha);
    }
    fclose(f);
}

void inserirPalavra(TrieNode* root, const char* word) {
    if (buscarPalavra(root, word)) return;
    inserirNaTrie(root, word);
}

static int removerRec(TrieNode* root, const char* word, int depth) {
    if (!root) return 0;
    if (word[depth] == '\0') {
        if (root->is_end_of_word) root->is_end_of_word = 0;
        int folha = 1;
        for (int i = 0; i < ALPHABET_SIZE; i++)
            if (root->children[i]) folha = 0;
        return folha;
    }
    int idx = tolower(word[depth]) - 'a';
    if (idx < 0 || idx >= ALPHABET_SIZE) return 0;
    if (root->children[idx]) {
        int shouldDelete = removerRec(root->children[idx], word, depth + 1);
        if (shouldDelete) {
            free(root->children[idx]);
            root->children[idx] = NULL;
            int folha = !root->is_end_of_word;
            for (int i = 0; i < ALPHABET_SIZE; i++)
                if (root->children[i]) folha = 0;
            return folha;
        }
    }
    return 0;
}

void removerPalavraArquivo(TrieNode* root, const char* word) {
    if (!buscarPalavra(root, word)) return;
    removerRec(root, word, 0);
}

static void autocomplete(TrieNode* root, char* buffer, int depth, int* count, int limite) {
    if (limite && *count >= limite)
        return;
    if (root->is_end_of_word) {
        buffer[depth] = '\0';
        printf("- %s\n", buffer);
        (*count)++;
        if (limite && *count >= limite)
            return;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i]) {
            buffer[depth] = 'a' + i;
            autocomplete(root->children[i], buffer, depth + 1, count, limite);
            if (limite && *count >= limite)
                return;
        }
    }
}

void mostrarSugestoes(TrieNode* root, const char* prefix, int limite) {
    TrieNode* curr = root;
    char buffer[100];
    int len = 0;
    for (int i = 0; prefix[i] != '\0'; i++) {
        if (!isalpha(prefix[i])) continue;
        int idx = tolower(prefix[i]) - 'a';
        if (idx < 0 || idx >= ALPHABET_SIZE) {
            printf("Prefixo invalido.\n");
            return;
        }
        if (!curr->children[idx]) {
            printf("Nenhuma sugestao encontrada.\n");
            return;
        }
        buffer[len++] = tolower(prefix[i]);
        curr = curr->children[idx];
    }
    buffer[len] = '\0';
    int count = 0;
    autocomplete(curr, buffer, len, &count, limite);
    if (count == 0)
        printf("Nenhuma sugestao encontrada.\n");
}

void liberarTrie(TrieNode* root) {
    if (!root) return;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        liberarTrie(root->children[i]);
    free(root);
}