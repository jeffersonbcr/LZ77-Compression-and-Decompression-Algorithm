#include "../src/lz77.h"
#include <stdio.h>
#include <string.h>

// Função para testar compressão e descompressão de strings
void test_string_compression()
{
    const unsigned char input[] = "This is a test string for LZ77 compression. This string is repetitive and should compress well.";
    unsigned char compressed[1024];
    unsigned char decompressed[1024];
    size_t compressed_size, decompressed_size;

    // Compressão
    lz77_compress(input, strlen((char *)input), compressed, &compressed_size);
    printf("Original size: %zu bytes\n", strlen((char *)input));
    printf("Compressed size: %zu bytes\n", compressed_size);

    // Descompressão
    lz77_decompress(compressed, compressed_size, decompressed, &decompressed_size);
    decompressed[decompressed_size] = '\0'; // Certifique-se de que a string descomprimida seja terminada corretamente

    // Exibindo resultados
    printf("Decompressed: %s\n", decompressed);

    if (strcmp((char *)input, (char *)decompressed) == 0)
    {
        printf("Success: Decompression matches the original text.\n");
    }
    else
    {
        printf("Error: Decompressed text does not match the original.\n");
    }
}

// Função para testar compressão e descompressão de arquivos
void test_file_compression()
{
    const char *input_filename = "./src/lz77.c";
    const char *compressed_filename = "./lz77.c.z77";
    const char *decompressed_filename = "./lz77-decompressed.c";

    printf("\nTesting file compression:\n");
    printf("Original size: %ld bytes\n", fsize(fopen(input_filename, "rb")));

    // Compressão
    uint32_t compressed_size = file_lz77_compress(input_filename, compressed_filename);
    printf("Compressed size: %u bytes\n", compressed_size);

    // Descompressão
    uint32_t decompressed_size = file_lz77_decompress(compressed_filename, decompressed_filename);
    printf("Decompressed size: %u bytes\n", decompressed_size);

    // Verificação básica para garantir que a descompressão funcionou corretamente
    if (fsize(fopen(input_filename, "rb")) == fsize(fopen(decompressed_filename, "rb")))
    {
        printf("Success: Decompressed file matches the original file size.\n");
    }
    else
    {
        printf("Error: Decompressed file size does not match the original.\n");
    }
}

int main()
{
    // Teste de compressão e descompressão de strings
    test_string_compression();

    // Teste de compressão e descompressão de arquivos
    test_file_compression();

    return 0;
}
