#ifndef LZ77_H
#define LZ77_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// Estrutura para armazenar um token LZ77
typedef struct
{
    uint16_t encoded;  // Contém o deslocamento e o comprimento da correspondência
    uint8_t next_char; // O próximo caractere literal após a correspondência
} LZ77Token;

// Funções para compressão e descompressão de strings
void lz77_compress(const unsigned char *input, size_t input_size, unsigned char *output, size_t *output_size);
void lz77_decompress(const unsigned char *input, size_t input_size, unsigned char *output, size_t *output_size);

// Funções para compressão e descompressão de arquivos
uint32_t file_lz77_compress(const char *filename_in, const char *filename_out);
uint32_t file_lz77_decompress(const char *filename_in, const char *filename_out);

// Função auxiliar para determinar o tamanho do arquivo
long fsize(FILE *in);

#endif // LZ77_H
