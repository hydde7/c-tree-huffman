#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TF 500

struct HTree{
	double freq;
	char sim[TF];
	struct HTree *esq, *dir;
};typedef struct HTree Huffman;

struct HLista{
	Huffman *info;
	struct HLista *prox;
};typedef struct HLista List;

void InitLista(List **l)
{
	*l = NULL;
}

void InitHuffman(Huffman **h)
{
	*h = NULL;
}

void push(List **l, Huffman *h)
{
	if(*l == NULL)
	{
		*l = (List*)malloc(sizeof(List));
		(*l) -> info = h;
		(*l) -> prox = NULL;
	}
	else
	{
		List *aux = *l;
		List *aux2 = *l;
		while(aux != NULL && aux -> info -> freq <= h -> freq)
		{
			aux2 = aux;
			aux = aux -> prox;
		}
		if(aux == aux2)
		{
			List *caixa = (List*)malloc(sizeof(List));
			caixa -> info = h;
			caixa -> prox = aux;
			*l = caixa;
		}
		else
		{
			List *caixa = (List*)malloc(sizeof(List));
			caixa -> info = h;
			caixa -> prox = aux;
			aux2 -> prox = caixa;
		}
	}
}

Huffman *pop(List **l)
{
	List *aux = *l;
	Huffman *h = (*l) -> info;
	*l = (*l) -> prox;
	free(aux);
	return h;
}

char isNULL(List *l)
{
	return l == NULL;
}

void criaNodo(Huffman **h, char simb[TF],double freq)
{
	*h = (Huffman*)malloc(sizeof(Huffman));
	strcpy((*h) -> sim, simb);
	(*h) -> dir = NULL;
	(*h) -> esq = NULL;
	(*h) -> freq = freq;
}

void criaNodoAponta(Huffman **h, char simb[TF],double freq, Huffman *esq, Huffman *dir)
{
	*h = (Huffman*)malloc(sizeof(Huffman));
	strcpy((*h) -> sim, simb);
	(*h) -> dir = dir;
	(*h) -> esq = esq;
	(*h) -> freq = freq;
}

char alfabeto(char l)
{
	return ((l >=65 && l<=90) || (l >=97 && l<=122) || l == '-' );
}

double frequencia(char palavra[TF],char frase[TF], int inicio)
{
	double qtd = 0;
	char flag = 1;
	int i, j;
	for(i = inicio; i< strlen(frase); i++)
	{
		if(!alfabeto(frase[i-1]) && alfabeto(frase[i]))
		{
			for(j = 0;i+j < strlen(frase) && alfabeto(frase[i+j]) && flag;j++)
			{
				if(j>=strlen(palavra) || frase[i+j] != palavra[j])
					flag =0;
			}
			if(flag)
			{
				qtd++;
				for(j =0;j<strlen(palavra) && flag && i+j < strlen(frase);j++)
				{	
					if(frase[i+j] == palavra[j])
					{
						frase[i+j] = 32;
					}
					else
						flag = 0;
				}
			}
		}
		flag = 1;
	}
	return qtd;
}
List *leFrase(char frase[TF])
{
	List *l = (List*)malloc(sizeof(List));	
	List *laux = (List*)malloc(sizeof(List));
	char aux[TF];
	int i;
	Huffman *h;
	InitHuffman(&h);
	InitLista(&l);
	
	double freqAtual = 0;
	int j =0;
	int contpal = 0;
	for(i =0;i<strlen(frase);i++)
	{
		if(alfabeto(frase[i]))//dentro do alfabeto
		{
			aux[j++] = frase[i];
			aux[j] = 0; 
			frase[i] = 32;
		}
		else if(j!=0)
		{
			j=0;
			freqAtual = frequencia(aux,frase,i) + 1;
			contpal += freqAtual;
			criaNodo(&h,aux,freqAtual);
			push(&l,h);
		}
	}
	Huffman *auxh;
	InitHuffman(&auxh);
	InitLista(&laux);
	while(!isNULL(l))
	{
		auxh = pop(&l);
		auxh -> freq /= contpal;
		push(&laux,auxh);
	}
	return laux;
}
void criaTree(List **l, Huffman **t)
{
	char aux[TF];
	if(!isNULL(*l))
	{
		Huffman *esq;
		esq = pop(&*l);
		if(!isNULL(*l))
		{
			Huffman *dir, *nodo;
			InitHuffman(&nodo);
			char auxiliar[TF];
			double frequencia = esq -> freq;
			strcpy(auxiliar,esq -> sim);
			dir = pop(&*l);
			strcat(auxiliar, dir -> sim);
			frequencia += dir -> freq;
			criaNodoAponta(&nodo, auxiliar, frequencia, esq, dir);
			push(&*l,nodo);
			criaTree(&*l,&*t);
		}
		else
		{
			*t = esq;
		}
	}	
}

void exibe(Huffman *h,int temp)
{
    if(h != NULL)
    {
        if(temp != -1)
            printf("%d",temp);
        exibe(h->esq,0);
        if(h -> esq != NULL)
            printf("0");
        exibe(h->dir,1);
        if(h -> dir != NULL)
            printf("1");
    }
}

void exibeCodificacao(Huffman *h)
{
	printf("\n\tCodificacao da tabela\n\t");
	exibe(h, -1);
	printf("\n");
}

int exibeTabela1(Huffman *h,int i)
{
	if(h != NULL && h->esq == NULL && h->dir == NULL)
	{
		printf("\t%d\t\t%s\t\t%.2lf\n",i,h->sim,h->freq);
		return i+1;
	}
	else
	{
		if(h->dir->dir == NULL)
		{
			i = exibeTabela1(h->dir,i);
			i = exibeTabela1(h->esq,i);
		}
		else
		{	
			i = exibeTabela1(h->esq,i);
			i = exibeTabela1(h->dir,i);
			
		}
		return i;
	}
}

void exibicaoTabela1(Huffman *h)
{
	printf("\n\tTabela n1\n\tNumero\t\tSimbolo\t\tFrequencia\n\n");
	exibeTabela1(h,0);
	printf("\n");
}

void exibeTabela2(Huffman *h,char cod[TF])
{
	
	if(h->esq == NULL && h->dir == NULL)
		printf("\t%s\t\t\t\t%s\n",cod,h->sim);
	else
	{
		int tam = strlen(cod);
		cod[tam+1] = 0;
		if(h->dir->dir == NULL)
		{
			cod[tam] = 49;
			exibeTabela2(h->dir,cod);
			cod[tam+1] = 0;
			cod[tam] = 48;
			exibeTabela2(h->esq,cod);
			cod[tam+1] = 0;
		}
		else
		{	
			cod[tam] = 48;	
			exibeTabela2(h->esq,cod);
			cod[tam+1] = 0;
			cod[tam] = 49;
			exibeTabela2(h->dir,cod);
			cod[tam+1] = 0;
		}
	}
}

void exibicaoTabela2(Huffman *h)
{
	char codigo[TF];
	strcpy(codigo,"\0");
	printf("\n\tTabela n2\n\tCodigo\t\t\t\tSimbolo\n\n");
	exibeTabela2(h,codigo);
}

void exibeTree(Huffman *h)
{
	if(h !=NULL)
	{
		if(h->esq == NULL && h->dir == NULL)
			printf("%s %.2lf\n",h->sim,h->freq);
		else
		{
			exibeTree(h->esq);
			exibeTree(h->dir);
		}
	}
}

int main()
{
	char frase[TF];
	Huffman *h;
	List *l;
	
	strcpy(frase,"a a a a a a d d d e e f b b b b b c c c c");
	frase[strlen(frase)+1]=0;
	frase[strlen(frase)]=32;
	strcpy(frase,strlwr(frase));
	InitLista(&l);
	InitHuffman(&h);
	l = leFrase(frase);
	criaTree(&l,&h);
	
	exibicaoTabela1(h);
	exibicaoTabela2(h);
	exibeCodificacao(h);
}
