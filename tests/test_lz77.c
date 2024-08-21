#include "../src/lz77.h"
#include <check.h>
#include <stdlib.h>
#include <string.h>

START_TEST(test_lz77_string_compression)
{
    const unsigned char input[] = "This is a test string for LZ77 compression. This string is repetitive and should compress well.";
    unsigned char compressed[1024];
    unsigned char decompressed[1024];
    size_t compressed_size, decompressed_size;

    // Compressão
    lz77_compress(input, strlen((char *)input), compressed, &compressed_size);

    // Verifique se a descompressão resulta no texto original
    lz77_decompress(compressed, compressed_size, decompressed, &decompressed_size);
    decompressed[decompressed_size] = '\0'; // Terminar a string descomprimida corretamente

    ck_assert_str_eq((char *)input, (char *)decompressed);

    // Para strings pequenas, não verifique o tamanho comprimido
    printf("Original size: %zu bytes\n", strlen((char *)input));
    printf("Compressed size: %zu bytes\n", compressed_size);

    // Adicional: Testar com uma string maior e repetitiva
    const unsigned char large_input[] = "abcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabc";
    unsigned char large_compressed[1024];
    unsigned char large_decompressed[1024];
    size_t large_compressed_size, large_decompressed_size;

    lz77_compress(large_input, strlen((char *)large_input), large_compressed, &large_compressed_size);
    lz77_decompress(large_compressed, large_compressed_size, large_decompressed, &large_decompressed_size);
    large_decompressed[large_decompressed_size] = '\0';

    ck_assert_str_eq((char *)large_input, (char *)large_decompressed);

    printf("Original large size: %zu bytes\n", strlen((char *)large_input));
    printf("Compressed large size: %zu bytes\n", large_compressed_size);

    // Neste caso, o tamanho comprimido deve ser menor
    ck_assert_int_lt(large_compressed_size, strlen((char *)large_input));
}
END_TEST

START_TEST(test_lz77_file_compression)
{
    const char *input_filename = "./src/lz77.c";
    const char *compressed_filename = "./lz77.c.z77";
    const char *decompressed_filename = "./lz77-decompressed.c";
    long original_size, decompressed_size;

    // Obter o tamanho original do arquivo
    original_size = fsize(fopen(input_filename, "rb"));
    ck_assert_msg(original_size > 0, "Erro ao obter o tamanho do arquivo original.");

    // Compressão
    size_t compressed_size = file_lz77_compress(input_filename, compressed_filename);
    ck_assert_msg(compressed_size > 0, "Erro ao comprimir o arquivo.");

    // Verifique se a compressão realmente reduziu o tamanho
    ck_assert_int_lt(compressed_size, original_size);

    // Descompressão
    size_t decompressed_size_result = file_lz77_decompress(compressed_filename, decompressed_filename);
    ck_assert_msg(decompressed_size_result > 0, "Erro ao descomprimir o arquivo.");

    // Verifique se o tamanho do arquivo descomprimido corresponde ao tamanho original
    decompressed_size = fsize(fopen(decompressed_filename, "rb"));
    ck_assert_int_eq(original_size, decompressed_size);
}
END_TEST

Suite *lz77_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("LZ77");

    /* Testes principais */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_lz77_string_compression);
    tcase_add_test(tc_core, test_lz77_file_compression);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = lz77_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
