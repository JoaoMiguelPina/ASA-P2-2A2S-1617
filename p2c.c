#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int nr_cidades;
int nr_estradas;
int nr_aeroportos;
int *custo_aeroportos;
long int custo_estradas;

typedef struct edge {
    int src, dest, weight;
} *Edge;


typedef struct graph {
    int V, E;
    Edge edge;
} *Graph;


Graph createGraph(int V, int E) {
    Graph graph = (Graph) malloc( sizeof(struct graph) );
    graph->V = V;
    graph->E = E;

    graph->edge = (Edge) malloc( graph->E * sizeof( struct edge ) );

    return graph;
}


typedef struct subset {
    int parent;
    int rank;
} *Subset;


int find(struct subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);

    return subsets[i].parent;
}


void Union(struct subset subsets[], int x, int y)
{
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;

    else
    {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}


int myComp(const void* a, const void* b) {
    int a1 = ((Edge) a)->weight;
    int b1 = ((Edge) b)->weight;
    return (a1 - b1);
}


int main() {

    int i = 0, j = 0, e = 0, v = 0, origem, destino, custo;
    scanf("%d",&nr_cidades);
    scanf("%d",&nr_aeroportos);

    custo_aeroportos = (int *) malloc(sizeof(int)*(nr_cidades + 1));

    for (i = 1; i <= nr_cidades; i++)
        custo_aeroportos[i] = -1;

    for (i = 1; i <= nr_aeroportos; i++) {
        scanf("%d %d", &origem, &custo);
        custo_aeroportos[origem] = custo;
    }
    scanf("%d", &nr_estradas);

    Graph graph1 = createGraph(nr_cidades + 1, nr_aeroportos + nr_estradas);
    Graph graph2 = createGraph(nr_cidades, nr_estradas);

    for (j = 0; j < nr_estradas; j++) {
        scanf("%d %d %d", &origem, &destino, &custo);
        graph1->edge[j].src = origem;
        graph1->edge[j].dest = destino;
        graph1->edge[j].weight = custo;
        graph2->edge[j].src = origem;
        graph2->edge[j].dest = destino;
        graph2->edge[j].weight = custo;
    }
    for (i = 1; i <= nr_cidades; i++) {
        if (custo_aeroportos[i] != -1) {
            graph1->edge[j].src = 0;
            graph1->edge[j].dest = i;
            graph1->edge[j].weight = custo_aeroportos[i];
            j++;
        }
    }

    struct edge result1[nr_cidades + 1];
    struct edge result2[nr_cidades];


    qsort(graph1->edge, graph1->E, sizeof(graph1->edge[0]), myComp);
    qsort(graph2->edge, graph2->E, sizeof(graph2->edge[0]), myComp);

    Subset subsets1 = (Subset) malloc( (nr_cidades + 1) * sizeof(struct subset) );
    Subset subsets2 = (Subset) malloc( (nr_cidades) * sizeof(struct subset) );

    for (v = 0; v < (nr_cidades + 1); ++v) {
        subsets1[v].parent = v;
        subsets1[v].rank = 0;
    }

    for (v = 0; v < nr_cidades; ++v) {
        subsets2[v].parent = v;
        subsets2[v].rank = 0;
    }
/*------------------------------------------SO ESTRADAS--------------------------------------------------------*/
long int custo_total = 0, aeroportos = 0, estradas1 = 0, estradas2 = 0, custo_aeroporto = -1;

i = 0;
if(nr_estradas >= nr_cidades -1){
  while (e < nr_cidades - 1) {
      if(i > nr_aeroportos + nr_estradas) {
          break;
      }
      struct edge next_edge = graph2->edge[i++];

      int x = find(subsets2, next_edge.src);
      int y = find(subsets2, next_edge.dest);

      if (x != y) {
          result2[e++] = next_edge;
          Union(subsets2, x, y);
      }
  }
}
else
  custo_estradas = 0;

  for (i = 0; i < e; ++i) {
      estradas2++;
      custo_estradas+= result2[i].weight;
      /*printf("%d -- %d == %d\n", result2[i].src, result2[i].dest, result2[i].weight);*/
  }


/*------------------------------------------ESTRADAS E AEROPORTOS----------------------------------------------*/
    i = 0;
    e = 0;
    if (nr_aeroportos == 0)
        nr_cidades--;
    while (e < nr_cidades) {

        if(i > nr_aeroportos + nr_estradas) {
            printf("Insuficiente\n");
            return 0;
        }
        struct edge next_edge = graph1->edge[i++];

        int x = find(subsets1, next_edge.src);
        int y = find(subsets1, next_edge.dest);

        if (x != y) {
            result1[e++] = next_edge;
            Union(subsets1, x, y);
        }
    }



    /*printf("Pineapple on Pizza\n");*/
    for (i = 0; i < e; ++i) {
        if(result1[i].src == 0) {
            custo_aeroporto = result1[i].weight;
            aeroportos++;
        }
        else
            estradas1++;
        custo_total+= result1[i].weight;
        /*printf("%d -- %d == %d\n", result[i].src, result[i].dest, result[i].weight);*/
    }
    if (aeroportos == 1) {
        custo_total -= custo_aeroporto;
        aeroportos--;
    }

    if(custo_estradas == custo_total){
      printf("%li\n0 %li\n", custo_estradas, estradas2);
      return 0;
    }
    printf("%li\n%li %li\n", custo_total, aeroportos, estradas1);
    return 0;
}
