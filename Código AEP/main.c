#include <stdio.h>
#include <string.h>

#define MAX_ACOES 100

typedef struct {
    int codigo;
    char escola[50];
    char tema[50];
    char data[15];
    char publicoAlvo[50];
    char responsavel[50];
    int participantesPrevistos;
    int participantesAtendidos;
    int situacao; /* 1 = planejada, 2 = realizada, 3 = cancelada */
} Acao;

Acao acoes[MAX_ACOES];
int totalAcoes = 0;

int lerInteiro(char mensagem[]) {
    char linha[100];
    int valor;

    while (1) {
        printf("%s", mensagem);
        if (fgets(linha, sizeof(linha), stdin) != NULL &&
            sscanf(linha, "%d", &valor) == 1) {
            return valor;
        }
        printf("Entrada invalida. Digite um numero inteiro.\n");
    }
}

void lerTexto(char mensagem[], char texto[], int tamanho) {
    while (1) {
        printf("%s", mensagem);

        if (fgets(texto, tamanho, stdin) == NULL) {
            texto[0] = '\0';
            return;
        }

        texto[strcspn(texto, "\n")] = '\0';

        if (strlen(texto) > 0) {
            return;
        }

        printf("Este campo e obrigatorio.\n");
    }
}

int buscarPorCodigo(int codigo) {
    int i;

    for (i = 0; i < totalAcoes; i++) {
        if (acoes[i].codigo == codigo) {
            return i;
        }
    }

    return -1;
}

void mostrarAcao(int indice) {
    printf("\nCodigo: %d\n", acoes[indice].codigo);
    printf("Escola: %s\n", acoes[indice].escola);
    printf("Tema: %s\n", acoes[indice].tema);
    printf("Data prevista: %s\n", acoes[indice].data);
    printf("Publico-alvo: %s\n", acoes[indice].publicoAlvo);
    printf("Responsavel: %s\n", acoes[indice].responsavel);
    printf("Participantes previstos: %d\n", acoes[indice].participantesPrevistos);

    if (acoes[indice].situacao == 1) {
        printf("Situacao: Planejada\n");
    } else if (acoes[indice].situacao == 2) {
        printf("Situacao: Realizada\n");
        printf("Participantes atendidos: %d\n", acoes[indice].participantesAtendidos);
    } else {
        printf("Situacao: Cancelada\n");
    }
}

void cadastrarAcao(void) {
    int codigo;

    if (totalAcoes >= MAX_ACOES) {
        printf("Limite de acoes atingido.\n");
        return;
    }

    codigo = lerInteiro("Codigo da acao: ");

    if (buscarPorCodigo(codigo) != -1) {
        printf("Ja existe uma acao com esse codigo.\n");
        return;
    }

    if (codigo <= 0) {
        printf("O codigo deve ser maior que zero.\n");
        return;
    }

    acoes[totalAcoes].codigo = codigo;
    lerTexto("Escola: ", acoes[totalAcoes].escola, 50);
    lerTexto("Tema (ex.: Alimentacao saudavel): ", acoes[totalAcoes].tema, 50);
    lerTexto("Data prevista (DD/MM/AAAA): ", acoes[totalAcoes].data, 15);
    lerTexto("Publico-alvo: ", acoes[totalAcoes].publicoAlvo, 50);
    lerTexto("Responsavel pela acao: ", acoes[totalAcoes].responsavel, 50);

    acoes[totalAcoes].participantesPrevistos =
        lerInteiro("Quantidade prevista de participantes: ");

    if (acoes[totalAcoes].participantesPrevistos < 0) {
        printf("A quantidade nao pode ser negativa. Cadastro cancelado.\n");
        return;
    }

    acoes[totalAcoes].participantesAtendidos = 0;
    acoes[totalAcoes].situacao = 1;
    totalAcoes++;

    printf("Acao cadastrada com situacao inicial planejada.\n");
}

void listarAcoes(void) {
    int i;

    if (totalAcoes == 0) {
        printf("Ainda nao ha acoes cadastradas.\n");
        return;
    }

    printf("\n=== ACOES CADASTRADAS ===\n");
    for (i = 0; i < totalAcoes; i++) {
        mostrarAcao(i);
    }
}

void pesquisarAcoes(void) {
    int opcao;
    int codigo;
    int i;
    int encontrou = 0;
    char busca[50];

    printf("\nPesquisar por:\n");
    printf("1 - Codigo\n");
    printf("2 - Escola\n");
    printf("3 - Tema\n");
    opcao = lerInteiro("Escolha: ");

    if (opcao == 1) {
        codigo = lerInteiro("Digite o codigo: ");
        i = buscarPorCodigo(codigo);

        if (i != -1) {
            mostrarAcao(i);
        } else {
            printf("Acao nao encontrada.\n");
        }
    } else if (opcao == 2 || opcao == 3) {
        if (opcao == 2) {
            lerTexto("Digite o nome da escola: ", busca, 50);
        } else {
            lerTexto("Digite o tema: ", busca, 50);
        }

        for (i = 0; i < totalAcoes; i++) {
            if ((opcao == 2 && strstr(acoes[i].escola, busca) != NULL) ||
                (opcao == 3 && strstr(acoes[i].tema, busca) != NULL)) {
                mostrarAcao(i);
                encontrou = 1;
            }
        }

        if (!encontrou) {
            printf("Nenhuma acao encontrada.\n");
        }
    } else {
        printf("Opcao de pesquisa invalida.\n");
    }
}

void atualizarSituacao(void) {
    int codigo;
    int indice;
    int novaSituacao;

    codigo = lerInteiro("Codigo da acao: ");
    indice = buscarPorCodigo(codigo);

    if (indice == -1) {
        printf("Acao nao encontrada.\n");
        return;
    }

    printf("1 - Planejada\n");
    printf("2 - Realizada\n");
    printf("3 - Cancelada\n");
    novaSituacao = lerInteiro("Nova situacao: ");

    if (novaSituacao < 1 || novaSituacao > 3) {
        printf("Situacao invalida.\n");
        return;
    }

    acoes[indice].situacao = novaSituacao;

    if (novaSituacao == 2) {
        acoes[indice].participantesAtendidos =
            lerInteiro("Quantidade efetiva de participantes: ");

        if (acoes[indice].participantesAtendidos < 0) {
            printf("A quantidade nao pode ser negativa. Situacao nao atualizada.\n");
            acoes[indice].situacao = 1;
            return;
        }
    } else {
        acoes[indice].participantesAtendidos = 0;
    }

    printf("Situacao atualizada.\n");
}

void gerarResumo(void) {
    int i;
    int planejadas = 0;
    int realizadas = 0;
    int canceladas = 0;
    int previstosRealizadas = 0;
    int atendidos = 0;
    double percentual;

    for (i = 0; i < totalAcoes; i++) {
        if (acoes[i].situacao == 1) {
            planejadas++;
        } else if (acoes[i].situacao == 2) {
            realizadas++;
            previstosRealizadas += acoes[i].participantesPrevistos;
            atendidos += acoes[i].participantesAtendidos;
        } else {
            canceladas++;
        }
    }

    printf("\n=== RESUMO GERAL ===\n");
    printf("Acoes planejadas: %d\n", planejadas);
    printf("Acoes realizadas: %d\n", realizadas);
    printf("Acoes canceladas: %d\n", canceladas);
    printf("Participantes atendidos nas acoes realizadas: %d\n", atendidos);
    printf("Participantes previstos nas acoes realizadas: %d\n", previstosRealizadas);

    if (previstosRealizadas > 0) {
        percentual = (atendidos * 100.0) / previstosRealizadas;
        printf("Percentual de participacao: %.1f%%\n", percentual);
    } else {
        printf("Percentual de participacao: nao disponivel (nao ha previsao nas acoes realizadas).\n");
    }
}

int main(void) {
    int opcao;

    do {
        printf("\n=== SISTEMA INTEGRAPSE ===\n");
        printf("1 - Cadastrar acao\n");
        printf("2 - Listar acoes\n");
        printf("3 - Pesquisar acoes\n");
        printf("4 - Atualizar situacao da acao\n");
        printf("5 - Gerar resumo geral\n");
        printf("6 - Sair\n");

        opcao = lerInteiro("Escolha uma opcao: ");

        if (opcao == 1) {
            cadastrarAcao();
        } else if (opcao == 2) {
            listarAcoes();
        } else if (opcao == 3) {
            pesquisarAcoes();
        } else if (opcao == 4) {
            atualizarSituacao();
        } else if (opcao == 5) {
            gerarResumo();
        } else if (opcao == 6) {
            printf("Programa encerrado.\n");
        } else {
            printf("Opcao inexistente. Escolha de 1 a 6.\n");
        }
    } while (opcao != 6);

    return 0;
}

