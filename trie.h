#ifndef TRIE_H
#define TRIE_H

#define ALPHABET_SIZE 26
#define ARQUIVO_PALAVRAS "palavras.txt"

typedef struct TrieNode TrieNode;

TrieNode* criarTrie(void);
void carregarArquivoParaTrie(TrieNode* root);
void inserirPalavra(TrieNode* root, const char* word);
void removerPalavraArquivo(TrieNode* root, const char* word);
void mostrarSugestoes(TrieNode* root, const char* prefix, int limite);
void liberarTrie(TrieNode* root);

#endif