#include "lz77.h"
#include <stdlib.h>
#include <string.h>

long fsize(FILE *in)
{
    if (in == NULL)
    {
        perror("File pointer is null");
        return -1;
    }

    long pos, length;
    pos = ftell(in);
    fseek(in, 0L, SEEK_END);
    length = ftell(in);
    fseek(in, pos, SEEK_SET);

    return length;
}

void lz77_compress(const unsigned char *input, size_t input_size, unsigned char *output, size_t *output_size)
{
    uint16_t WINDOW_SIZE = 4096;
    uint8_t LOOKAHEAD_SIZE = 18;

    size_t output_pos = 0;
    size_t coding_pos = 0;

    while (coding_pos < input_size)
    {
        size_t best_distance = 0;
        size_t best_length = 0;

        // Ajuste para limitar o loop a janelas menores
        for (size_t back_pos = 1; back_pos <= WINDOW_SIZE && back_pos <= coding_pos; back_pos++)
        {
            size_t start = coding_pos - back_pos;
            size_t length = 0;

            while (length < LOOKAHEAD_SIZE && coding_pos + length < input_size && input[start + length] == input[coding_pos + length])
            {
                length++;
            }

            // Atualizar a melhor correspondência encontrada
            if (length > best_length)
            {
                best_length = length;
                best_distance = back_pos;
            }
        }

        // Somente gerar um token para correspondências significativas
        if (best_length >= 3)
        {
            uint16_t encoded = (best_distance << 4) | (best_length - 3);
            LZ77Token token = {encoded, input[coding_pos + best_length]};
            memcpy(output + output_pos, &token, sizeof(token));
            output_pos += sizeof(token);
            coding_pos += best_length + 1;
        }
        else
        {
            // Codificar um único caractere literal
            LZ77Token token = {0, input[coding_pos]};
            memcpy(output + output_pos, &token, sizeof(token));
            output_pos += sizeof(token);
            coding_pos++;
        }
    }

    *output_size = output_pos;
}

void lz77_decompress(const unsigned char *input, size_t input_size, unsigned char *output, size_t *output_size)
{
    size_t input_pos = 0;
    size_t output_pos = 0;

    while (input_pos < input_size)
    {
        LZ77Token token;
        memcpy(&token, input + input_pos, sizeof(token));
        input_pos += sizeof(token);

        uint16_t distance = token.encoded >> 4;
        uint16_t length = (token.encoded & 0x0F) + 3;

        if (distance > 0)
        {
            size_t start = output_pos - distance;
            for (size_t i = 0; i < length; i++)
            {
                if (start + i < output_pos && output_pos < *output_size)
                {
                    output[output_pos++] = output[start + i];
                }
            }
        }

        if (output_pos < *output_size)
        {
            output[output_pos++] = token.next_char;
        }

        if (input_pos >= input_size)
        {
            break;
        }
    }

    *output_size = output_pos;
}

uint32_t file_lz77_compress(const char *filename_in, const char *filename_out)
{
    FILE *in, *out;
    uint8_t *uncompressed_text, *compressed_text;
    size_t uncompressed_size, compressed_size;
    size_t malloc_size;

    in = fopen(filename_in, "rb");
    if (in == NULL)
    {
        perror("Error opening input file");
        return 0;
    }

    uncompressed_size = fsize(in);
    printf("Uncompressed size: %zu bytes\n", uncompressed_size); // %zu para size_t

    uncompressed_text = (uint8_t *)malloc(uncompressed_size);
    if (uncompressed_text == NULL)
    {
        perror("Memory allocation error");
        fclose(in);
        return 0;
    }

    if (fread(uncompressed_text, 1, uncompressed_size, in) != uncompressed_size)
    {
        perror("Error reading input file");
        fclose(in);
        free(uncompressed_text);
        return 0;
    }
    fclose(in);

    malloc_size = uncompressed_size * 2;
    compressed_text = (uint8_t *)malloc(malloc_size);
    if (compressed_text == NULL)
    {
        perror("Memory allocation error");
        free(uncompressed_text);
        return 0;
    }

    lz77_compress(uncompressed_text, uncompressed_size, compressed_text, &compressed_size);
    printf("Compressed size: %zu bytes\n", compressed_size);

    out = fopen(filename_out, "wb");
    if (out == NULL)
    {
        perror("Error opening output file");
        free(uncompressed_text);
        free(compressed_text);
        return 0;
    }

    fwrite(&uncompressed_size, sizeof(uncompressed_size), 1, out);

    if (fwrite(compressed_text, 1, compressed_size, out) != compressed_size)
    {
        perror("Error writing to output file");
        fclose(out);
        free(uncompressed_text);
        free(compressed_text);
        return 0;
    }

    fclose(out);
    free(uncompressed_text);
    free(compressed_text);

    return compressed_size + sizeof(uncompressed_size);
}

uint32_t file_lz77_decompress(const char *filename_in, const char *filename_out)
{
    FILE *in, *out;
    size_t compressed_size, uncompressed_size; // Alterado para size_t
    uint8_t *compressed_text, *uncompressed_text;

    in = fopen(filename_in, "rb");
    if (in == NULL)
    {
        perror("Error opening input file");
        return 0;
    }

    compressed_size = fsize(in);
    printf("Total file size (including header): %zu bytes\n", compressed_size); // %zu para size_t

    if (fread(&uncompressed_size, sizeof(uncompressed_size), 1, in) != 1)
    {
        perror("Error reading original size");
        fclose(in);
        return 0;
    }
    printf("Expected uncompressed size: %zu bytes\n", uncompressed_size);

    compressed_size -= sizeof(uncompressed_size);
    compressed_text = (uint8_t *)malloc(compressed_size);
    if (compressed_text == NULL)
    {
        perror("Memory allocation error");
        fclose(in);
        return 0;
    }

    if (fread(compressed_text, 1, compressed_size, in) != compressed_size)
    {
        perror("Error reading compressed data");
        fclose(in);
        free(compressed_text);
        return 0;
    }
    fclose(in);

    uncompressed_text = (uint8_t *)malloc(uncompressed_size);
    if (uncompressed_text == NULL)
    {
        perror("Memory allocation error");
        free(compressed_text);
        return 0;
    }

    lz77_decompress(compressed_text, compressed_size, uncompressed_text, &uncompressed_size);
    printf("Actual uncompressed size: %zu bytes\n", uncompressed_size);

    out = fopen(filename_out, "wb");
    if (out == NULL)
    {
        perror("Error opening output file");
        free(compressed_text);
        free(uncompressed_text);
        return 0;
    }

    if (fwrite(uncompressed_text, 1, uncompressed_size, out) != uncompressed_size)
    {
        perror("Error writing to output file");
        fclose(out);
        free(compressed_text);
        free(uncompressed_text);
        return 0;
    }

    fclose(out);
    free(compressed_text);
    free(uncompressed_text);

    return uncompressed_size;
}
