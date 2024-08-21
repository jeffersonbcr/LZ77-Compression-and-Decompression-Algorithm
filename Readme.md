# Projeto de Compressão e Descompressão LZ77

Este projeto implementa um algoritmo de compressão e descompressão baseado na técnica LZ77, que é um método popular de compressão sem perdas. O objetivo é comprimir dados repetitivos em arquivos e descomprimir os mesmos mantendo a integridade dos dados originais.

## Descrição do Projeto

O projeto é dividido em duas partes principais:

1. **Compressão LZ77**: Implementação da compressão baseada em janela deslizante e correspondência de padrões para identificar sequências de dados repetidos e comprimi-los eficientemente.
2. **Descompressão LZ77**: Restauração dos dados originais a partir dos tokens gerados durante o processo de compressão.

O projeto permite comprimir e descomprimir tanto strings quanto arquivos, garantindo que os dados descomprimidos sejam idênticos aos originais.

## Abordagem de Implementação

A implementação do LZ77 foi dividida em:

1. **Estrutura de Dados**:

   - Um token LZ77 é definido por:
     - `encoded`: Um valor que contém o deslocamento e o comprimento da correspondência na janela.
     - `next_char`: O próximo caractere literal após a correspondência.

2. **Funções de Compressão**:

   - A função de compressão utiliza uma janela deslizante de 4096 bytes e um buffer de "lookahead" de 18 bytes.
   - O algoritmo busca correspondências de padrões no buffer deslizante e, quando encontra uma sequência repetitiva de no mínimo 3 bytes, cria um token com o deslocamento e o comprimento da correspondência. Caso contrário, adiciona apenas o próximo caractere literal.

3. **Funções de Descompressão**:

   - A função de descompressão reverte o processo de compressão, restaurando os dados originais a partir dos tokens gerados. O algoritmo navega pelos tokens, decodificando as correspondências e inserindo os caracteres literais onde necessário.

4. **Operações em Arquivos**:
   - As funções de compressão e descompressão de arquivos leem os arquivos, aplicam o algoritmo LZ77, e gravam os resultados em arquivos comprimidos e descomprimidos, respectivamente.

## Problemas e Erros Encontrados

### 1. **Incompatibilidade de Tipos**

Durante a implementação, o tipo `uint32_t` foi inicialmente utilizado para representar o tamanho de dados comprimidos e descomprimidos. Isso gerou incompatibilidades, pois as funções de compressão e descompressão esperavam `size_t`, que é o tipo mais apropriado para lidar com tamanhos de dados. Como resultado:

- O uso de `uint32_t` levou a erros de cálculo de tamanho, especialmente em sistemas de 64 bits, onde o `size_t` possui 64 bits e o `uint32_t` apenas 32 bits.
- O erro se manifestou principalmente na descompressão, onde o tamanho descomprimido era maior que o esperado.

**Solução**: Todos os tipos relacionados ao tamanho de buffers e arquivos foram alterados para `size_t`, garantindo que o código funcionasse corretamente em diferentes plataformas sem perda de precisão.

### 2. **Problema com o Último Token**

Outro problema foi identificado com o último token processado durante a descompressão, onde um byte extra era adicionado ao final do buffer descomprimido, resultando em um tamanho descomprimido maior que o original.

**Solução**: O algoritmo de descompressão foi ajustado para interromper corretamente o processamento após o último token, evitando a adição de bytes extras.

### 3. **Verificação e Validação de Dados**

O projeto implementa verificações adicionais para garantir que a compressão e a descompressão estejam funcionando corretamente. Isso inclui a comparação de bytes finais do arquivo original e descomprimido, além de verificações do tamanho do arquivo.

## Resultados

Após a correção dos erros e ajustes nas funções de compressão e descompressão, o projeto foi testado com sucesso para strings e arquivos de diferentes tamanhos. Os resultados mostram que:

- A compressão reduz significativamente o tamanho de arquivos com dados repetitivos.
- A descompressão restaura corretamente os dados originais, sem perdas.
- Todos os testes realizados indicam que o algoritmo funciona de forma robusta, mantendo a integridade dos dados originais.

### Exemplos de Compressão e Descompressão

**Compressão de Strings**:

```plaintext
Original size: 95 bytes
Compressed size: 288 bytes
Decompressed: This is a test string for LZ77 compression. This string is repetitive and should compress well.
Success: Decompression matches the original text.
```

**Compressão de Arquivos**:

```plaintext
Testing file compression:
Original size: 7324 bytes
Compressed size: 4456 bytes
Decompressed size: 7324 bytes
Success: Decompressed file matches the original file size.
```

## Execução

```bash
make clean
make
./bin/example.c
```

<h1 align='center'>Autor</h1>

<a href="https://github.com/jeffersonbcr">
    <img style="border-radius: 50%;" src="https://avatars.githubusercontent.com/u/58866006?v=4" width="100px;" alt=""/><br/>
    <sub><b>Jefferson Botitano Calderon Romero</b></sub></a>

<br><br/>
