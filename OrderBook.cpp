#include "OrderBook.hpp"
#include <iostream>


OrderBook::OrderBook(){
    *n_sell = 0;
    *n_buy = 0;
    orders[0] = nullptr; // Inicializa a lista de ordens de compra
    orders[1] = nullptr; // Inicializa a lista de ordens de venda
    transactions_head = nullptr; // Inicializa a lista de transações
}

OrderBook::~OrderBook() {  // Vou ter que pensar melhor nessa lógica depois, depende de qual vai estar em órdem crescente e decrescente
    // Libera a memória alocada para as ordens de compra
    OrderNode* current = orders[0];
    while (current != nullptr) {
        OrderNode* temp = current;
        current = current->proximo;
        delete temp;
    }

    // Libera a memória alocada para as ordens de venda
    current = orders[1];
    while (current != nullptr) {
        OrderNode* temp = current;
        current = current->proximo;
        delete temp;
    }

    // Libera a memória alocada para as transações
    TransacoesNode* current_transaction = transactions_head;
    while (current_transaction != nullptr) {
        TransacoesNode* temp = current_transaction;
        current_transaction = current_transaction->next;
        delete temp;
    }
}

bool OrderBook::submit(Order order) {   
    char tipo_ordem = order.getType(); 
    OrderNode* maior_ordem_compra;
    OrderNode* menor_ordem_venda;

    // CASO QUE A ORDEM RECEBIDA É DE VENDA
    if (tipo_ordem == 'S'){
        if (orders[0] == nullptr){ // Nenhuma ordem de compra no banco de dados
            this->armazenarOrdem(order, &orders[1]);
            return false
        }
        else{
            maior_ordem_compra = orders[0];
            if (order.getPrice() > maior_ordem_compra->order.getPrice()) { // A ordem com maior preco de compra é menor do que o preco de venda da ordem em questão
                this->armazenarOrdem(order, &orders[1]);   
                return false
            }
            else{
                this->executarTransacao(order, maior_ordem_compra->order); // A ordem com maior preco de compra é tao grande quando o preco de venda. 
                return true
            }
        }
    }

    // CASO QUE A ORDEM RECEBIDA É DE COMPRA
    else if (tipo_ordem == 'B'){
        if (orders[1] == nullptr) { // Nenhuma ordem de venda no banco de dados
            this->armazenarOrdem(order, &orders[0]);
            return false
        }
        else{
            menor_ordem_venda = orders[1]; 
            if (order.getPrice() < menor_ordem_venda->order.getPrice()){ // Lógica parecida com a de cima
                this->armazenarOrdem(order, &orders[0]);
                return false
            }
            else{
                this->executarTransacao(order, menor_ordem_venda->order);
                return true
            }
        }
    }
}

void OrderBook::armazenarOrdem(Order order, OrderNode* *lista_head) {
    char order_type = order.getType(); 
    OrderNode* nova_ordem; 
    nova_ordem->order = order;
    nova_ordem->proximo = nullptr;

    if (*lista_head == nullptr){ // Lista vazia, é só adicinar order em primeiro lugar
        *lista_head = nova_ordem;   
    }
    else {
        OrderNode* atual = *lista_head;
        OrderNode* anterior = nullptr;

        if (order_type == 'B'){

            while (atual != nullptr){
                if (atual->order.getPrice() < order.getPrice()){ // Se a ordem em questão é ordem de compra então vamos organizar a lista em ordem decrescente
                    nova_ordem->proximo = atual;
                    *lista_head = nova_ordem;
                    break;
                }
                else if (atual->order.getPrice() == order.getPrice()){
                    if (atual->order.getTimestamp() >= nova_ordem->order.getTimestamp()){
                        if (anterior == nullptr){
                            nova_ordem->proximo = atual;
                            *lista_head = nova_ordem;
                            break;
                        }
                        else{
                            anterior->proximo = nova_ordem;
                            nova_ordem->proximo = atual;
                            break;
                        }
                    }
                    anterior = atual;
                    atual = atual->proximo;
                }
            }
        }

        else if (order_type == 'S'){

            while (atual != nullptr){
                if (atual->order.getPrice() < order.getPrice()){ // Se a ordem em questão é ordem de compra então vamos organizar a lista em ordem crescente
                    nova_ordem->proximo = atual;
                    *lista_head = nova_ordem;
                    break;
                }
                else if (atual->order.getPrice() == order.getPrice()){
                    if (atual->order.getTimestamp() >= nova_ordem->order.getTimestamp()){
                        if (anterior == nullptr){
                            nova_ordem->proximo = atual;
                            *lista_head = nova_ordem;
                            break;
                        }
                        else{
                            anterior->proximo = nova_ordem;
                            nova_ordem->proximo = atual;
                        }
                    }

                anterior = atual;
                atual = atual->proximo;
                }
            }
        }

    }
}
  

void OrderBook::executarTransacao(Order order_sell, Order order_buy){
    // Implementação da lógica para executar uma ordem
}

bool OrderBook::cancel(int id) {
    // Implementação da lógica para cancelamento de ordens
    // Verificar se a ordem existe e removê-la da lista correspondente
    // Retornar true se a ordem foi cancelada com sucesso, false caso contrário
}

Order* OrderBook::getBuyOrders(int* n) {
    // Implementação da lógica para obter as ordens de compra
    // Retornar um array de ordens de compra e atualizar o valor de n com o número de ordens
}

Order* OrderBook::getSellOrders(int* n) {
    // Implementação da lógica para obter as ordens de venda
    // Retornar um array de ordens de venda e atualizar o valor de n com o número de ordens
}

Transaction* OrderBook::getTransactions(int* n) {
    // Implementação da lógica para obter as transações realizadas
    // Retornar um array de transações e atualizar o valor de n com o número de transações
}

int OrderBook::getNumBuyOrders() {
    return *(this->n_buy);
}

int OrderBook::getNumSellOrders() {
    return *(this->n_sell); 
}

void OrderBook::printBuyOrders() {
    // Implementação da lógica para imprimir as ordens de compra
}

void OrderBook::printSellOrders() {
    // Implementação da lógica para imprimir as ordens de venda
}

void OrderBook::printTransactions() {
    // Implementação da lógica para imprimir as transações realizadas
}

