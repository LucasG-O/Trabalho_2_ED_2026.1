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
