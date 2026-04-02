#include "OrderBook.hpp"


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
    // Implementação da lógica para submissão de ordens
    // Verificar se é uma ordem de compra ou venda e inserir na lista correspondente
    // Verificar se há correspondência para realizar transações
    // Retornar true se a ordem foi processada com sucesso, false caso contrário
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



