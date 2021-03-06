#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<windows.h>
#include"ListaVendas.h"

ListaVendas* criaListaVendas(){
    ListaVendas* ListaVendasManager = (ListaVendas*) malloc(sizeof(ListaVendas));

    if(ListaVendasManager != NULL){
        ListaVendasManager->primeiro = NULL;
        ListaVendasManager->ultimo = NULL;
        ListaVendasManager->valorInicialCaixa = 0.0;
        ListaVendasManager->valorFinalCaixa = 0.0;
        ListaVendasManager->lucroFinal = 0.0;
        ListaVendasManager->quantidade = 0;
        ListaVendasManager->codigoAtual = 1;
    }

    return ListaVendasManager;
}

void liberaListaVendas(ListaVendas* ListaVendasManager){
    if(ListaVendasManager != NULL){
        ElementoVenda* no;

        while(ListaVendasManager->primeiro != NULL){
            no = ListaVendasManager->primeiro;
            ListaVendasManager->primeiro = ListaVendasManager->primeiro->proximo;
            free(no);
        }

        free(ListaVendasManager);
    }
}

int listaVendasVazia(ListaVendas* ListaVendasManager){
    if(ListaVendasManager == NULL) return 1;
    if(ListaVendasManager->primeiro == NULL) return 1;
    return 0;
}

int tamanhoListaVendas(ListaVendas* ListaVendasManager){
    if(ListaVendasManager == NULL) return 0;
    return ListaVendasManager->quantidade;
}

int insereListaVendasInicio(ListaVendas* ListaVendasManager, TipoVenda DadosVenda){
    if(ListaVendasManager == NULL) return 0;

    ElementoVenda* no = (ElementoVenda*) malloc(sizeof(ElementoVenda));
    if(no == NULL) return 0;
    no->Dados = DadosVenda;

    if(listaVendasVazia(ListaVendasManager)){
		ListaVendasManager->primeiro = no;
		ListaVendasManager->ultimo = no;
	}
	else{
		no->proximo = ListaVendasManager->primeiro;
		ListaVendasManager->primeiro->anterior = no;
		ListaVendasManager->primeiro = no;
	}

    ListaVendasManager->quantidade++;
    return 1;
}

int removeListaVendasInicio(ListaVendas* ListaVendasManager){
    if(ListaVendasManager == NULL) return 0;
    if((ListaVendasManager->primeiro) == NULL) return 0;

    ElementoVenda *no = ListaVendasManager->primeiro;
    ListaVendasManager->primeiro = no->proximo;

	if(no->proximo != NULL){
        no->proximo->anterior = NULL;
        ListaVendasManager->quantidade--;
    }

    free(no);
    return 1;
}

int removeListaVendasFinal(ListaVendas* ListaVendasManager){
    if(ListaVendasManager == NULL) return 0;
    if((ListaVendasManager->primeiro) == NULL) return 0;

    ElementoVenda *no = ListaVendasManager->ultimo;

    if(no == ListaVendasManager->primeiro){
        free(no);
        return removeListaVendasInicio(ListaVendasManager);
    }
    else{
        ListaVendasManager->ultimo->anterior->proximo = NULL;
        ListaVendasManager->ultimo = ListaVendasManager->ultimo->anterior;
        ListaVendasManager->quantidade--;
        free(no);
        return 1;
    }
}

int removeListaVendas(ListaVendas* ListaVendasManager, int codigo){
    if(ListaVendasManager == NULL) return 0;

    ElementoVenda *no = ListaVendasManager->primeiro;
    while(no != NULL && no->Dados.codigo != codigo){
        no = no->proximo;
    }

    if(no == NULL) return 0;
    if(no == ListaVendasManager->primeiro){
        free(no);
        return removeListaVendasInicio(ListaVendasManager);
    }
    else if(no == ListaVendasManager->ultimo){
        free(no);
        return removeListaVendasFinal(ListaVendasManager);
    }
    else{
        no->anterior->proximo = no->proximo;
        no->proximo->anterior = no->anterior;
        ListaVendasManager->quantidade--;
        free(no);
        return 1;
    }
}

void preencherData(TipoVenda *venda){
	struct tm *data_hora_atual;
	time_t segundos;

	time(&segundos);
	data_hora_atual = (struct tm*) localtime(&segundos);

	venda->DataHora->dia=data_hora_atual->tm_mday;

    venda->DataHora->mes=data_hora_atual->tm_mon+1;

	venda->DataHora->ano=data_hora_atual->tm_year+1900;

	venda->DataHora->hora=data_hora_atual->tm_hour;//hora
	venda->DataHora->minuto=data_hora_atual->tm_min;//minuto
	venda->DataHora->segundo=data_hora_atual->tm_sec;//segundo

    return;
}

void realizarVenda(ListaProdutos *lista, ListaVendas *listaVendido){
    ElementoProduto *produtoAuxiliar;
    int opcao,controle=0, respostas=0;//variavel respostas que responde todas as perguntas
    float valorEntregadoCliente;
    ElementoProduto *produtoVendido;
    TipoVenda DadosVenda;
    DadosVenda.DataHora = (Time*) calloc(1, sizeof(Time));

    preencherData(&DadosVenda);

    if (lista->quantidade==0){
        printf("\n\n\t\t\t<<< N�O H� PRODUTOS CADASTRADOS >>>\n\n");
        Sleep(2000);
        return;

    }

    produtoAuxiliar=lista->primeiro;
    system("cls");
    printf("\n\n\t\t\t======================================\n");
    printf("\n\t\t\t*** MANAGER - REALIZA��O DE VENDA ***\n");
    printf("\n\t\t\t======================================\n\n");

    while (produtoAuxiliar!=NULL){
        printf("%d) %s\n", produtoAuxiliar->Dados.codigo, produtoAuxiliar->Dados.nome);

        produtoAuxiliar=produtoAuxiliar->proximo;
    }

    printf("\n\nDigite o c�digo do produto que vendeu: ");

    scanf ("%d", &respostas);


    produtoAuxiliar=lista->primeiro;

    while (produtoAuxiliar!=NULL){ // Procura o produto com o c�digo dito.
        if (produtoAuxiliar->Dados.codigo==respostas){
            produtoVendido=produtoAuxiliar;
            controle=1;
            break;
        }
        produtoAuxiliar=produtoAuxiliar->proximo;
    }

    if (controle==0){// variavel controle controla se entrou ou n�o na fun��o que encontra os codigos iguais
        printf("\n\n\t\t\t<<< N�O H� PRODUTOS COM O C�DIGO >>>\n\n");
        Sleep(2000);
        return;
    }

    printf("Produto ( %s ) que custa R$%.2f e tem %d unidades de estoque\n\nVenda de quantas unidades?   ",produtoVendido->Dados.nome, produtoVendido->Dados.valorDeSaida, produtoVendido->Dados.quantidade);

    scanf ("%d", &respostas);

    if (produtoVendido->Dados.quantidade>=respostas){
        printf("Nome do vendedor:  ");
        fflush(stdin);
        gets(DadosVenda.vendedor);

        printf("\n\n");

        printf("Qual o m�todo de pagamento?\n\n1) Cart�o\n2) Dinheiro\n\n : ");
        do{
            scanf ("%d", &opcao);
        }while(opcao!=1 && opcao!=2);

        system("cls");
        printf("\n\n\t\t\t======================================\n");
        printf("\n\t\t\t*** MANAGER - REALIZA��O DE VENDA ***\n");
        printf("\n\t\t\t======================================\n\n");
        printf("Processando venda...\n\n");

        switch(opcao){//DIVIDE CARTAO OU DINHEIRO, POIS SAO TRATAMENTOS DIFERENTES
            case 1:
                strcpy(DadosVenda.metodoPagamento, "Cart�o");
                DadosVenda.troco=0;
                break;
            case 2:
                printf("Quanto o cliente entregou (Ficar� um loop at� o cliente entregar um valor maior ou igual a %.2f ):\n\n", (respostas*produtoVendido->Dados.valorDeSaida));
                do{
                    printf("   ---> ");
                    scanf ("%f", &valorEntregadoCliente);
                }while(valorEntregadoCliente<(respostas*produtoVendido->Dados.valorDeSaida));//controla pro valor entregue ser maior

                strcpy(DadosVenda.metodoPagamento, "Dinheiro");
                DadosVenda.troco=valorEntregadoCliente-(respostas*produtoVendido->Dados.valorDeSaida);
                printf("   TROCO = %.2f\n\n", DadosVenda.troco);
                system("pause");
            break;
        }

        //processando todos os dados da venda
        DadosVenda.valorPago=(respostas*produtoVendido->Dados.valorDeSaida);
        produtoVendido->Dados.quantidade=produtoVendido->Dados.quantidade-respostas;
        DadosVenda.codigo=listaVendido->codigoAtual;
        DadosVenda.lucroVenda=(respostas*produtoVendido->Dados.valorDeSaida)-(respostas*produtoVendido->Dados.valorDeEntrada);
        DadosVenda.totalVenda=respostas*produtoVendido->Dados.valorDeSaida;
        listaVendido->codigoAtual++;

        system("cls");
        printf("\n\n\t\t\t======================================\n");
        printf("\n\t\t\t*** MANAGER - REALIZA��O DE VENDA ***\n");
        printf("\n\t\t\t======================================\n");
        printf("\n\n+-------------------------------+\n");
        printf("|\t  *NOTA FISCAL*\t\t|\n");
        printf("|\t\t\t\t|\n| Vendedor: %s\t\t\t|\n|\t\t\t\t|\n| M�todo: %s\t\t|\n|\t\t\t\t|\n|\t\t\t\t|\n| Total: R$ %.2f\t\t|\n|\t\t\t\t|\n| Lucro: R$ %.2f\t\t|\n|\t\t\t\t|\n| Valor pago: R$ %.2f\t\t|\n|\t\t\t\t|\n| Troco: R$ %.2f\t\t|\n|\t\t\t\t|", DadosVenda.vendedor,DadosVenda.metodoPagamento,DadosVenda.totalVenda,
        DadosVenda.lucroVenda, DadosVenda.valorPago,DadosVenda.troco);
        printf("\n|\t\t\t\t|\n| DATA %d/%d/%d\t\t|\n|\t\t\t\t|\n| HORA %d:%d:%d\t\t\t|\n|\t\t\t\t|\n", DadosVenda.DataHora->dia, DadosVenda.DataHora->mes,DadosVenda.DataHora->ano,DadosVenda.DataHora->hora,DadosVenda.DataHora->minuto, DadosVenda.DataHora->segundo );
        insereListaVendasInicio(listaVendido,DadosVenda);
        printf("+-------------------------------+\n\n");
        listaVendido->valorFinalCaixa += DadosVenda.totalVenda;
        listaVendido->lucroFinal += DadosVenda.lucroVenda;
    }
    else{
        printf("\n\n\t<<< N�o h� esse estoque para venda. H� somente %d unidades. >>>\n\n", produtoVendido->Dados.quantidade);
        Sleep(2000);
        return;
    }

}

int fechamentoDeCaixa(ListaVendas* ListaVendasManager){
	int i;

    FILE *pont_arq;
    pont_arq = fopen("RelatorioVendas.txt", "w");

    printf("\n\n\t\t\t========================================\n");
    printf("\n\t\t\t*** MANAGER - FECHAMENTO DE CAIXA ***\n");
    printf("\n\t\t\t========================================\n\n");
	printf(" > Valor inicial do caixa: ");
	scanf("%f", &ListaVendasManager->valorInicialCaixa);
    system("cls");

    ElementoVenda *VendaAuxiliar = ListaVendasManager->primeiro;

    printf("\n\n\t\t\t====================================\n");
    printf("\n\t\t\t*** MANAGER - FECHAMENTO DE CAIXA ***\n");
    printf("\n\t\t\t====================================\n\n");
    fprintf(pont_arq, "     * RELAT�RIO DE VENDAS *\n\n");
    printf("%-3s   %-10s   %-5s    %-10s    %-5s\n", "COD", "VENDEDOR", "VALOR", "PAGAMENTO", "DATA");
    fprintf(pont_arq, "%-3s   %-10s   %-5s    %-10s    %-5s\n", "COD", "VENDEDOR", "VALOR", "PAGAMENTO", "DATA");

	for(i=0; i < ListaVendasManager->quantidade; i++){
        printf("%3d - %-10s - %5.2f - %-10s - ", VendaAuxiliar->Dados.codigo, VendaAuxiliar->Dados.vendedor, VendaAuxiliar->Dados.valorPago, VendaAuxiliar->Dados.metodoPagamento);
        fprintf(pont_arq, "%3d - %-10s - %5.2f - %-10s - ", VendaAuxiliar->Dados.codigo, VendaAuxiliar->Dados.vendedor, VendaAuxiliar->Dados.valorPago, VendaAuxiliar->Dados.metodoPagamento);
        printf("%d/%d/%d, %d:%d:%d\n", VendaAuxiliar->Dados.DataHora->dia, VendaAuxiliar->Dados.DataHora->mes,VendaAuxiliar->Dados.DataHora->ano,VendaAuxiliar->Dados.DataHora->hora,VendaAuxiliar->Dados.DataHora->minuto, VendaAuxiliar->Dados.DataHora->segundo );
        fprintf(pont_arq,"%d/%d/%d, %d:%d:%d\n", VendaAuxiliar->Dados.DataHora->dia, VendaAuxiliar->Dados.DataHora->mes,VendaAuxiliar->Dados.DataHora->ano,VendaAuxiliar->Dados.DataHora->hora,VendaAuxiliar->Dados.DataHora->minuto, VendaAuxiliar->Dados.DataHora->segundo );
        VendaAuxiliar = VendaAuxiliar->proximo;
    }

	printf("\n----------------------------\n");
	printf(" > Caixa Inicial: %.2f\n", ListaVendasManager->valorInicialCaixa);
    fprintf(pont_arq, "\n\n > Caixa Inicial: %.2f\n", ListaVendasManager->valorInicialCaixa);
	printf(" > Caixa Final: %.2f\n", ListaVendasManager->valorFinalCaixa);
    fprintf(pont_arq, " > Caixa Final: %.2f\n", ListaVendasManager->valorFinalCaixa);
    printf(" > Lucro Total: %.2f\n", ListaVendasManager->lucroFinal);
    fprintf(pont_arq, " > Lucro Total: %.2f\n", ListaVendasManager->lucroFinal);
	printf(" > Quantidade de vendas: %d", ListaVendasManager->quantidade);
    fprintf(pont_arq, " > Quantidade de vendas: %d", ListaVendasManager->quantidade);
	printf("\n----------------------------\n\n");

    fclose(pont_arq);
    return 0;
}
