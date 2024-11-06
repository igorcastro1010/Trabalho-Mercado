#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Fruta {
    int codigo;
    char nome[50];
    int quantidade;
    float preco;
    struct Fruta *prox;
} Fruta;


Fruta* criarFruta(int codigo, char *nome, int quantidade, float preco) {
    Fruta *nova = (Fruta*) malloc(sizeof(Fruta));
    nova->codigo = codigo;
    strcpy(nova->nome, nome);
    nova->quantidade = quantidade;
    nova->preco = preco;
    nova->prox = NULL;
    return nova;
}


void cadastrarFruta(Fruta **lista) {
    int codigo, quantidade;
    char nome[50];
    float preco;

    printf("Digite o código da fruta: ");
    scanf("%d", &codigo);

   
    Fruta *atual = *lista;
    while (atual != NULL) {
        if (atual->codigo == codigo) {
            printf("Código já cadastrado!\n");
            return;
        }
        atual = atual->prox;
    }

    printf("Digite o nome da fruta: ");
    scanf("%s", nome);
    printf("Digite a quantidade da fruta: ");
    scanf("%d", &quantidade);
    printf("Digite o preço da fruta: ");
    scanf("%f", &preco);

    Fruta *nova = criarFruta(codigo, nome, quantidade, preco);
    nova->prox = *lista;
    *lista = nova;
    printf("Fruta cadastrada com sucesso!\n");
}


void listarFrutas(Fruta *lista) {
    if (lista == NULL) {
        printf("Nenhuma fruta cadastrada.\n");
        return;
    }

    Fruta *atual = lista;
    while (atual != NULL) {
        printf("Código: %d, Nome: %s, Quantidade: %d, Preço: %.2f\n",
               atual->codigo, atual->nome, atual->quantidade, atual->preco);
        atual = atual->prox;
    }
}


Fruta* buscarFrutaPorCodigo(Fruta *lista, int codigo) {
    Fruta *atual = lista;
    while (atual != NULL) {
        if (atual->codigo == codigo) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}


void alterarFruta(Fruta *lista, int codigo) {
    Fruta *fruta = buscarFrutaPorCodigo(lista, codigo);
    if (fruta == NULL) {
        printf("Fruta não encontrada.\n");
        return;
    }

    printf("Digite o novo nome da fruta: ");
    scanf("%s", fruta->nome);
    printf("Digite a nova quantidade da fruta: ");
    scanf("%d", &fruta->quantidade);
    printf("Digite o novo preço da fruta: ");
    scanf("%f", &fruta->preco);
    printf("Fruta alterada com sucesso!\n");
}


void excluirFruta(Fruta **lista, int codigo) {
    Fruta *atual = *lista;
    Fruta *anterior = NULL;

    while (atual != NULL && atual->codigo != codigo) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Fruta não encontrada.\n");
        return;
    }

    if (atual->quantidade > 0) {
        printf("Não é possível excluir uma fruta com estoque.\n");
        return;
    }

    if (anterior == NULL) {
        *lista = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual);
    printf("Fruta excluída com sucesso!\n");
}


void registrarVenda(int codigo, int quantidade, float valor) {
    FILE *arquivo = fopen("vendas.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de vendas.\n");
        return;
    }

    fprintf(arquivo, "Código: %d, Quantidade: %d, Valor Total: %.2f\n", codigo, quantidade, valor);
    fclose(arquivo);
}


void venderFruta(Fruta *lista, int codigo, int quantidade) {
    Fruta *fruta = buscarFrutaPorCodigo(lista, codigo);
    if (fruta == NULL) {
        printf("Fruta não encontrada.\n");
        return;
    }

    if (fruta->quantidade < quantidade) {
        printf("Estoque insuficiente para a venda.\n");
        return;
    }

    fruta->quantidade -= quantidade;
    float valorVenda = quantidade * fruta->preco;
    printf("Venda realizada com sucesso! Valor total: %.2f\n", valorVenda);
    registrarVenda(codigo, quantidade, valorVenda);
}


void liberarLista(Fruta **lista) {
    Fruta *atual = *lista;
    while (atual != NULL) {
        Fruta *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    *lista = NULL;
}

int main() {
    Fruta *lista = NULL;
    int opcao, codigo, quantidade;

    do {
        printf("\n1. Cadastrar fruta\n");
        printf("2. Listar frutas\n");
        printf("3. Buscar fruta\n");
        printf("4. Alterar fruta\n");
        printf("5. Excluir fruta\n");
        printf("6. Vender fruta\n");
        printf("7. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarFruta(&lista);
                break;
            case 2:
                listarFrutas(lista);
                break;
            case 3:
                printf("Digite o código da fruta: ");
                scanf("%d", &codigo);
                Fruta *f = buscarFrutaPorCodigo(lista, codigo);
                if (f != NULL) {
                    printf("Código: %d, Nome: %s, Quantidade: %d, Preço: %.2f\n", f->codigo, f->nome, f->quantidade, f->preco);
                } else {
                    printf("Fruta não encontrada.\n");
                }
                break;
            case 4:
                printf("Digite o código da fruta a alterar: ");
                scanf("%d", &codigo);
                alterarFruta(lista, codigo);
                break;
            case 5:
                printf("Digite o código da fruta a excluir: ");
                scanf("%d", &codigo);
                excluirFruta(&lista, codigo);
                break;
            case 6:
                printf("Digite o código da fruta e a quantidade a vender: ");
                scanf("%d %d", &codigo, &quantidade);
                venderFruta(lista, codigo, quantidade);
                break;
            case 7:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 7);

    liberarLista(&lista);
    return 0;
}
