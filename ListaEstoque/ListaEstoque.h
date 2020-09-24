typedef struct{
    char nome[200];
    char dataValidade[12];
    float valorDeEntrada;
    float valorDeSaida;
    int quantidade;
    int codigo;
}TipoProduto;

typedef struct elemento* Lista;

Lista* criaLista();
void liberaLista(Lista* ListaEstoque);
int tamanhoLista(Lista* ListaEstoque);
int listaVazia(Lista* ListaEstoque);
int insereListaInicio(Lista*ListaEstoque, TipoProduto DadosProduto);
int insereListaFinal(Lista*ListaEstoque, TipoProduto DadosProduto);
int insereListaOrdenada(Lista*ListaEstoque, TipoProduto DadosProduto);
int removeListaInicio(Lista*ListaEstoque);
int removeListaFinal(Lista*ListaEstoque);
int removeLista(Lista*ListaEstoque, int codigo);
int consultaListaPosicao(Lista*ListaEstoque, int posicao, TipoProduto *DadosProduto);
int consultaListaCodigo(Lista*ListaEstoque, int codigo, TipoProduto *DadosProduto);