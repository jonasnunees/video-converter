# Conversor de Vídeo (via FFmpeg)

Este é um programa de linha de comando feito em **C** que permite converter arquivos de vídeo para os formatos mais populares usando o **FFmpeg**. É ideal para quem precisa de uma ferramenta simples e funcional para conversão de vídeos diretamente pelo terminal.

## Funcionalidades

* Verifica automaticamente se o FFmpeg está instalado.
* Pergunta ao usuário se deseja instalar o FFmpeg via `apt` caso não esteja presente.
* Converte arquivos de vídeo para os formatos:

  * MP4 (`--mp4`)
  * MOV (`--mov`)
  * AVI (`--avi`)
  * MPEG (`--mpeg`)
  * WEBM (`--webm`)
* Salva os arquivos convertidos na pasta `~/Downloads`.

## Requisitos

* Sistema Linux com `apt` (Debian, Ubuntu e derivados).
* Conexão com a internet (necessária apenas na primeira execução, caso o FFmpeg precise ser instalado).

## Como usar

```bash
./conversor <caminho_do_video> <formato_destino>
```

### Exemplo:

```bash
./conversor meu_video.webm --mp4
```

Isso converterá `meu_video.webm` para o formato MP4 e salvará o novo arquivo como `~/Downloads/meu_video.mp4`.

## Formatos suportados

* `--mp4`
* `--mov`
* `--avi`
* `--mpeg`
* `--webm`

## Ajuda

Para ver a ajuda do programa:

```bash
./conversor --help
```

## Observação sobre o FFmpeg

Este repositório **não inclui o binário do FFmpeg** devido à política de limite de tamanho de arquivos do GitHub (limite de 25MB por arquivo, e o FFmpeg possui cerca de 80MB). Por isso, o programa verifica automaticamente a existência do FFmpeg no sistema e, caso não esteja instalado, pergunta ao usuário se deseja instalá-lo usando o `apt`.

## Licença

Este projeto é de uso livre e sem fins comerciais. Fique à vontade para estudar, modificar e compartilhar.

---

Desenvolvido com foco em simplicidade, usabilidade e acessibilidade para todos os usuários, mesmo os com pouca experiência em linha de comando ou programação.
