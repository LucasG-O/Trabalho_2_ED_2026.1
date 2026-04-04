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
        current_transaction = current_transaction->proximo;
        delete temp;
    }
}

bool OrderBook::submit(Order order) {   
    char tipo_ordem = order.getType(); 
    OrderNode* maior_ordem_compra;
    OrderNode* menor_ordem_venda;

    // CASO QUE A ORDEM RECEBIDA É DE VENDA
    if (tipo_ordem == 'S'){
        maior_ordem_compra = orders[0];
        if (orders[0] == nullptr|| order.getPrice() > maior_ordem_compra->order.getPrice()) { // Nenhuma ordem de compra no banco de dados
            this->armazenarOrdem(order, &orders[1]);
            return false;
        }

        this->executarTransacao(order, maior_ordem_compra->order, 'S'); // A ordem com maior preco de compra é tao grande quando o preco de venda. 
        return true;
    }

    // CASO QUE A ORDEM RECEBIDA É DE COMPRA
    menor_ordem_venda = orders[1];
    if (orders[1] == nullptr|| order.getPrice() < menor_ordem_venda->order.getPrice()) { // Nenhuma ordem de venda no banco de dados
        this->armazenarOrdem(order, &orders[0]);
        return false;
    } 

    this->executarTransacao(menor_ordem_venda->order,order, 'B');
    return true;
}

void OrderBook::armazenarOrdem(Order order, OrderNode* *lista_head) {
    char order_type = order.getType(); 
    OrderNode* nova_ordem = new OrderNode{order, nullptr}; // Alocar memória para a nova ordem

    if (*lista_head == nullptr){ // Lista vazia, é só adicinar order em primeiro lugar
        *lista_head = nova_ordem;
        return;  
    }
        OrderNode* atual = *lista_head;
        OrderNode* anterior = nullptr;

        if (order_type == 'B'){
            if(atual->order.getPrice() < order.getPrice() ||
             (atual->order.getPrice() == order.getPrice() &&
              atual->order.getTimestamp() >= nova_ordem->order.getTimestamp())
            ){ // Se a ordem em questão é ordem de compra então vamos organizar a lista em ordem decrescente
                nova_ordem->proximo = atual;
                *lista_head = nova_ordem;
                return;

            }
            while (atual != nullptr){
                if ((atual->order.getPrice() < order.getPrice())||
                    ((atual->order.getPrice() == order.getPrice())&&
                    (atual->order.getTimestamp() >= nova_ordem->order.getTimestamp()))){ // Se a ordem em questão é ordem de compra então vamos organizar a lista em ordem decrescente
                    
                    anterior->proximo = nova_ordem;
                    nova_ordem->proximo = atual;
                    break;
                }
                    anterior = atual;
                    atual = atual->proximo;
            }
            if (atual == nullptr){ // A nova ordem é a menor da lista, então ela deve ser adicionada no final da lista
                anterior->proximo = nova_ordem;
                nova_ordem->proximo = nullptr;
            }
        }
        else{
            if(atual->order.getPrice() > order.getPrice() || (atual->order.getPrice() == order.getPrice() && atual->order.getTimestamp() <= nova_ordem->order.getTimestamp())){ // Se a ordem em questão é ordem de compra então vamos organizar a lista em ordem decrescente
                nova_ordem->proximo = atual;
                *lista_head = nova_ordem;
                return;
            }
            while (atual != nullptr){
                if ((atual->order.getPrice() > order.getPrice())||
                    ((atual->order.getPrice() == order.getPrice())&&
                    (atual->order.getTimestamp() >= nova_ordem->order.getTimestamp()))){ // Se a ordem em questão é ordem de compra então vamos organizar a lista em ordem decrescente
                    
                    anterior->proximo = nova_ordem;
                    nova_ordem->proximo = atual;    

                    break;
                }
                anterior = atual;
                atual = atual->proximo;
            }
            if (atual == nullptr){ // A nova ordem é a maior da lista, então ela deve ser adicionada no final da lista
                anterior->proximo = nova_ordem;
                nova_ordem->proximo = nullptr;
            }   
    }
}

  

void OrderBook::executarTransacao(Order order_sell, Order order_buy, char tipo_ordem) {

    if(tipo_ordem == 'S'){
        Transaction nova_transacao(order_buy.getId(), order_sell.getId(), order_buy.getPrice());
        OrderNode* atual = orders[0]; // A primeira ordem de compra
        OrderNode* anterior = nullptr;
        while(order_buy.getId() != atual->order.getId()){
            anterior = atual;
            atual = atual->proximo;

        }
        anterior->proximo = atual->proximo; // Lógica para remover a ordem de compra da lista de ordens de compra
        // Lógica para atualizar ou remover as ordens de compra e venda envolvidas na transação
    }
    else {
        Transaction nova_transacao(order_buy.getId(), order_sell.getId(), order_sell.getPrice());
        OrderNode* atual = orders[1]; // A primeira ordem de venda
        OrderNode* anterior = nullptr;
        while(order_sell.getId() != atual->order.getId()){
            anterior = atual;
            atual = atual->proximo;
        }
        anterior->proximo = atual->proximo;
    }
}

bool OrderBook::cancel(int id) {

    OrderNode* atual_c = orders[0]; // A primeira ordem de compra
    OrderNode* anterior_c    = nullptr;
    OrderNode* atual_v = orders[1]; // A primeira ordem de venda
    OrderNode* anterior_v = nullptr;
    while (atual_c != nullptr){
        if (atual_c->order.getId() == id){
            if (anterior_c == nullptr){ // A ordem a ser cancelada é a primeira da lista de ordens de compra
                orders[0] = atual_c->proximo;
            }
            else{
                anterior_c->proximo = atual_c->proximo;
            }
            delete atual_c; // Libera a memória alocada para a ordem cancelada
            return true; // Ordem cancelada com sucesso
        }
        anterior_c = atual_c;
        atual_c = atual_c->proximo;
    }
    while(atual_v != nullptr){
        if (atual_v->order.getId() == id){
            if (anterior_v == nullptr){ // A ordem a ser cancelada é a primeira da lista de ordens de venda
                orders[1] = atual_v->proximo;
            }
            else{
                anterior_v->proximo = atual_v->proximo;
            }
            delete atual_v; // Libera a memória alocada para a ordem cancelada
            return true; // Ordem cancelada com sucesso
        }
        anterior_v = atual_v;
        atual_v = atual_v->proximo;
    }
    std::cout << "Ordem com ID " << id << " não encontrada. Cancelamento falhou." << std::endl;
    return false; // Ordem não encontrada, cancelamento falhou
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
    while(orders[0] != nullptr){
        std::cout <<"[ "<< orders[0]->order.getId()  << " | " << orders[0]->order.getPrice() << " | " << orders[0]->order.getTimestamp() << " ] " << std::endl;//[ Id | Preço | Timestamp ]
        orders[0] = orders[0]->proximo;
    }
    // Implementação da lógica para imprimir as ordens de compra
}

void OrderBook::printSellOrders() {
    while(orders[1] != nullptr){
        std::cout <<"[ "<< orders[1]->order.getId()  << " | " << orders[1]->order.getPrice() << " | " << orders[1]->order.getTimestamp() << " ] " << std::endl;//[ Id | Preço | Timestamp ]
        orders[1] = orders[1]->proximo;
    }
    // Implementação da lógica para imprimir as ordens de venda
}

void OrderBook::printTransactions() {
    TransacoesNode* atual = transactions_head;
    while(atual != nullptr){
        std::cout <<"[ "<< atual->transaction.getBuyOrderId()  << " | " << atual->transaction.getSellOrderId() << " | " << atual->transaction.getExecutionPrice() << " ] " << std::endl;//[ Id da ordem de compra | Id da ordem de venda | Preço de execução ]
        atual = atual->proximo;
    }
    // Implementação da lógica para imprimir as transações realizadas
}

