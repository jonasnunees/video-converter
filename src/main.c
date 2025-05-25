#include <stdio.h>      // Biblioteca para funções de entrada e saída (como printf, fprintf)
#include <stdlib.h>     // Biblioteca para funções gerais (como system, getenv, malloc)
#include <string.h>     // Biblioteca para manipulação de strings (como strstr, strdup)
#include <libgen.h>     // Biblioteca para extrair o nome base de um caminho de arquivo
#include <unistd.h>     // Biblioteca para acesso a chamadas do sistema (como access)

// Função que extrai apenas o nome do arquivo (sem caminho e sem extensão)
const char *extrair_nome_arquivo(const char *caminho) {
    
    // Duplica o caminho para evitar modificar o original
    char *caminho_dup = strdup(caminho); 
    
    // Verifica se a duplicação falhou
    if (!caminho_dup) return NULL;       

    // Obtém o nome do arquivo (ex: video.mp4)
    char *nome = basename(caminho_dup);  
    
    // Procura o último ponto (para remover a extensão)
    char *ponto = strrchr(nome, '.');    
    
    // Se encontrar, substitui por '\0' para cortar a extensão
    if (ponto) *ponto = '\0';            

    // Duplica o nome limpo para retornar
    char *resultado = strdup(nome);      
    
    // Libera a memória usada pela duplicata
    free(caminho_dup);                   
    return resultado;
}

// Função que exibe a ajuda do programa
void exibir_ajuda(const char *nome_programa) {
    printf("\nConversor de Vídeo - Ajuda\n");
    printf("---------------------------\n");
    printf("Uso: %s <caminho_do_video> <formato_destino>\n", nome_programa);
    printf("Exemplo: %s video.webm --mp4\n", nome_programa);
    printf("Formatos suportados: --mp4, --mov, --avi, --mpeg, --webm\n");
}

// Função que verifica se o ffmpeg está instalado e o instala, se necessário
int verificar_ou_instalar_ffmpeg() {
    // Verifica se o comando ffmpeg está disponível (0 = encontrado)
    if (access("/usr/bin/ffmpeg", X_OK) == 0) {
        return 1; // FFmpeg já está instalado
    }

    // Avisa ao usuário que o ffmpeg não está instalado
    printf("⚠️  O ffmpeg não está instalado no seu sistema.\n");
    printf("Deseja instalar agora? (s/n): ");
    
    char resposta[4];
    
    // Lê a resposta do usuário
    fgets(resposta, sizeof(resposta), stdin); 
    
    // Verifica se a resposta começa com 's' (sim)
    if (resposta[0] == 's' || resposta[0] == 'S') {
        printf("🔄 Instalando ffmpeg...\n");
        
        // Executa o comando para instalar ffmpeg
        int resultado = system("sudo apt update && sudo apt install -y ffmpeg");
        
        // Retorna 1 se sucesso, 0 se falhar
        return (resultado == 0); 
    }

    // Usuário recusou a instalação
    printf("❌ O programa requer o ffmpeg para funcionar. Encerrando.\n");
    return 0;
}

// Função principal que converte o vídeo
int converter_video(const char *caminho_video, const char *formato) {
    
    // Obtém o diretório HOME do usuário (ex: /home/usuario)
    char *home = getenv("HOME");
    
    if (!home) {
        fprintf(stderr, "❌ Não foi possível obter o diretório HOME.\n");
        return 0;
    }

    // Extrai o nome do arquivo sem caminho e sem extensão
    const char *nome_arquivo = extrair_nome_arquivo(caminho_video);
    
    if (!nome_arquivo) {
        fprintf(stderr, "❌ Nome do arquivo inválido.\n");
        return 0;
    }

    // Monta o caminho completo do novo arquivo convertido
    char destino[4096];
    
    // formato+2 pula o "--"
    snprintf(destino, sizeof(destino), "%s/Downloads/%s.%s", home, nome_arquivo, formato + 2); 

    // Informa o início da conversão
    printf("🔄 Iniciando conversão para '%s'...\n", destino);

    // Cria o comando de conversão usando ffmpeg
    char comando[8192];
    snprintf(comando, sizeof(comando), "ffmpeg -i \"%s\" \"%s\" -y", caminho_video, destino);

    // Executa o comando no terminal
    int resultado = system(comando);

    // Verifica se a conversão foi bem-sucedida
    if (resultado == 0) {
        printf("✅ Conversão concluída com sucesso! Salvo em: %s\n", destino);
        return 1;
    } else {
        printf("❌ Erro na conversão. Verifique se o arquivo é válido.\n");
        return 0;
    }
}

// Função principal que roda quando o programa é executado
int main(int argc, char *argv[]) {
    
    // Se o usuário passar --help, mostramos as instruções
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        exibir_ajuda(argv[0]);
        return 0;
    }

    // Validação dos argumentos: o programa precisa de 3 argumentos
    if (argc != 3) {
        printf("❌ Argumentos inválidos.\n");
        exibir_ajuda(argv[0]);
        return 1;
    }

    // Lista de formatos suportados
    const char *formatos[] = { "--mp4", "--mov", "--avi", "--mpeg", "--webm" };
    int formato_valido = 0;

    // Verifica se o formato informado está na lista
    for (int i = 0; i < 5; i++) {
        if (strcmp(argv[2], formatos[i]) == 0) {
            formato_valido = 1;
            break;
        }
    }

    // Se o formato for inválido, informa e encerra
    if (!formato_valido) {
        printf("❌ Formato '%s' não suportado.\n", argv[2]);
        exibir_ajuda(argv[0]);
        return 1;
    }

    // Verifica se o ffmpeg está disponível ou tenta instalá-lo
    if (!verificar_ou_instalar_ffmpeg()) {
        
        // Falha ao instalar ou o usuário recusou
        return 1; 
    }

    // Tenta realizar a conversão do vídeo
    return !converter_video(argv[1], argv[2]);
}
