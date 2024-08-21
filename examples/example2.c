// #include "../src/lz77.h"
// #include <stdio.h>
// #include <string.h>

// int main()
// {
//     const unsigned char input[] = "Hello, this is an example to demonstrate LZ77 compression!";
//     unsigned char compressed[1024];   // Buffer para armazenar dados comprimidos
//     unsigned char decompressed[1024]; // Buffer para armazenar dados descomprimidos
//     size_t compressed_size, decompressed_size;

//     uint8_t pointer_length_width = 4; // Definindo a largura do campo de comprimento no token

//     // Compressão
//     lz77_compress(input, sizeof(input) - 1, compressed, &compressed_size, pointer_length_width); // sizeof(input) - 1 para não incluir o null terminator
//     printf("Original size: %zu bytes\nCompressed size: %zu bytes\n", sizeof(input) - 1, compressed_size);

//     // Descompressão
//     lz77_decompress(compressed, compressed_size, decompressed, &decompressed_size, pointer_length_width);
//     decompressed[decompressed_size] = '\0'; // Certifique-se de que a string descomprimida é terminada corretamente

//     // Exibindo resultados
//     printf("Decompressed: %s\n", decompressed);

//     if (memcmp(input, decompressed, decompressed_size) == 0)
//     {
//         printf("Success: Decompression matches the original text.\n");
//     }
//     else
//     {
//         printf("Error: Decompressed text does not match the original.\n");
//     }

//     return 0;
// }
