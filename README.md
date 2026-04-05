# Sistema de Livro de Ofertas (Order Book)

## Descrição do Projeto
Este projeto implementa um Livro de Ofertas (Order Book) financeiro em C++. O sistema é responsável por receber, armazenar, cruzar e cancelar ordens de compra e venda de ativos. Quando o preço exigido por um vendedor (oferta) é igual ou menor que o preço oferecido por um comprador (demanda), o sistema executa automaticamente uma transação, atualizando o saldo de ordens do livro e guardando o histórico da operação.

O projeto demonstra o uso prático de Ponteiros, Alocação Dinâmica de Memória e Estruturas de Dados Lineares (Listas Encadeadas) desenvolvidas "do zero", sem o uso das bibliotecas prontas do C++ (como `std::vector` ou `std::list`).

---

## Instruções de Compilação
Para compilar o projeto, você precisará de um compilador C++ (como o GCC/g++ ou MinGW) instalado na sua máquina.

Abra o terminal na pasta raiz do projeto e execute o seguinte comando para compilar todos os arquivos fonte em um único executável:

**No Windows / Linux / macOS:**
```bash
g++ main.cpp OrderBook.cpp Order.cpp Transaction.cpp -o order_book
```
## Instruções de Execução
Após a compilação bem-sucedida, você pode executar o programa pelo próprio terminal.

**No Windows:**
> .\order_book.exe

**No Linux / macOS:**
> ./order_book

*O programa executará os casos de teste pré-definidos no arquivo main.cpp, demonstrando a inserção, execução, cancelamento e impressão das ordens e transações no terminal.*

---

## Organização Interna dos Dados

Para garantir eficiência nas inserções e remoções, o sistema foi construído utilizando **Listas Encadeadas (Linked Lists)**. O gerenciamento de memória é feito de forma estritamente manual via alocação dinâmica (new e delete), garantindo que não haja vazamentos de memória (memory leaks).

A classe OrderBook contém três listas encadeadas principais:

1. **Lista de Ordens de Compra (orders[0]):**
   - **Regra de Ordenação:** Decrescente. As ordens que oferecem o **maior preço** ficam no topo da fila, pois têm prioridade na compra.
   - **Desempate:** FIFO (First In, First Out). Se duas ordens têm o mesmo preço, a que chegou primeiro (menor timestamp) tem prioridade.

2. **Lista de Ordens de Venda (orders[1]):**
   - **Regra de Ordenação:** Crescente. As ordens que exigem o **menor preço** ficam no topo da fila, pois são as ofertas mais atrativas para o mercado.
   - **Desempate:** FIFO, utilizando o timestamp.

3. **Lista de Transações (transactions_head):**
   - Sempre que ocorre um match (cruzamento) entre a cabeça da lista de compra e a cabeça da lista de venda, a ordem executada é removida do livro, tem sua memória liberada e uma nova estrutura Transaction é gerada.
   - Essa transação é inserida no início da lista encadeada de histórico para otimizar a velocidade de inserção O(1).

### Retorno de Dados Dinâmicos
Quando os métodos de recuperação de dados (getBuyOrders, getSellOrders e getTransactions) são chamados, o sistema percorre as listas encadeadas contando os elementos, aloca dinamicamente um Array com o tamanho exato necessário no Heap, copia os dados de forma sequencial e retorna o ponteiro deste array para o chamador (que passa a ser o responsável por liberar a memória após o uso).
