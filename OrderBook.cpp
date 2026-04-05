#include "OrderBook.hpp"
#include <iostream>


OrderBook::OrderBook(){
    n_sell = 0;
    n_buy = 0;
    orders[0] = nullptr; // Inicializa a lista de ordens de compra
    orders[1] = nullptr; // Inicializa a lista de ordens de venda
    transactions_head = nullptr; // Inicializa a lista de transações
}

OrderBook::~OrderBook() {
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

    // ordem de venda
    if (tipo_ordem == 'S'){
        maior_ordem_compra = orders[0];
        // Verifica se há ordens de compra e se há match
        // Se o preço de venda exigido for maior que o que os compradores oferecem, não há negócio. Armazena a ordem
        if (orders[0] == nullptr|| order.getPrice() > maior_ordem_compra->order.getPrice()) {
            this->armazenarOrdem(order, &orders[1]);
            n_sell++;
            return false;
        }

        this->executarTransacao(order, maior_ordem_compra->order, 'S'); // A ordem com maior preco de compra é tao grande quando o preco de venda.
        n_buy--;
        return true;
    }

    // ordem de compra
    menor_ordem_venda = orders[1];
    // Verifica se há ordens de venda e se há match
    // Se o preço de compra oferecido for menor que o exigido pelos vendedores, não há negócio. Armazena a ordem.
    if (orders[1] == nullptr|| order.getPrice() < menor_ordem_venda->order.getPrice()) { // Nenhuma ordem de venda no banco de dados
        this->armazenarOrdem(order, &orders[0]);
        n_buy++;
        return false;
    } 

    this->executarTransacao(menor_ordem_venda->order,order, 'B');
    n_sell--;
    return true;
}

void OrderBook::armazenarOrdem(Order order, OrderNode* *lista_head) {
    char order_type = order.getType(); 
    OrderNode* nova_ordem = new OrderNode{order, nullptr}; // Alocar memória para a nova ordem
    // REGRA DE ORDENAÇÃO DO BOOK:
    // Ordens de Compra (B): Decrescente (Maior preço tem prioridade)
    // Ordens de Venda (S): Crescente (Menor preço tem prioridade)
    // Desempate: Tempo (Quem chegou primeiro, pelo timestamp, fica na frente)

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
                    (atual->order.getTimestamp() >= nova_ordem->order.getTimestamp()))){ // // Se a ordem em questão é de venda, então vamos organizar a lista em ordem crescente
                    
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
    
    // 1. Cria a transação e SALVA NA LISTA ENCADEADA (transactions_head)
    float preco_exec = (tipo_ordem == 'S') ? order_buy.getPrice() : order_sell.getPrice();
    Transaction nova_transacao(order_buy.getId(), order_sell.getId(), preco_exec);
    
    TransacoesNode* novo_no = new TransacoesNode{nova_transacao, transactions_head};
    transactions_head = novo_no;

    // Remove da lista de COMPRA
    if(tipo_ordem == 'S'){
        OrderNode* atual = orders[0]; 
        OrderNode* anterior = nullptr;
        
        while(atual != nullptr && order_buy.getId() != atual->order.getId()){
            anterior = atual;
            atual = atual->proximo;
        }
        
        if (atual != nullptr) {
            if (anterior == nullptr) { 
                orders[0] = atual->proximo; 
            } else {
                anterior->proximo = atual->proximo; 
            }
            delete atual;
        }
    }
    // Remove da lista de VENDA
    else {
        OrderNode* atual = orders[1]; 
        OrderNode* anterior = nullptr;
        
        while(atual != nullptr && order_sell.getId() != atual->order.getId()){
            anterior = atual;
            atual = atual->proximo;
        }
        
        if (atual != nullptr) {
            if (anterior == nullptr) { 
                orders[1] = atual->proximo; 
            } else {
                anterior->proximo = atual->proximo; 
            }
            delete atual; // Libera a memória
        }
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
            n_buy--;
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
            n_sell--;
            return true; // Ordem cancelada com sucesso
        }
        anterior_v = atual_v;
        atual_v = atual_v->proximo;
    }
    std::cout << "Ordem com ID " << id << " não encontrada. Cancelamento falhou." << std::endl;
    return false; // Ordem não encontrada, cancelamento falhou
}

Order* OrderBook::getBuyOrders(int* n) {
    
int tamanho = 0;
    OrderNode* atual = orders[0];
    while (atual != nullptr) {
        tamanho++;
        atual = atual->proximo;
    }
    *n = tamanho;
    Order* ordens_compra = new Order[tamanho];
    atual = orders[0];
    for (int i = 0; i < tamanho; i++) {
        ordens_compra[i] = atual->order;
        atual = atual->proximo;
    }
    return ordens_compra;
}

Order* OrderBook::getSellOrders(int* n) {
    int tamanho = 0;
    OrderNode* atual = orders[1];
    while (atual != nullptr) {
        tamanho++;
        atual = atual->proximo;
    }
    *n = tamanho;
    Order* ordens_venda = new Order[tamanho];
    atual = orders[1];
    for (int i = 0; i < tamanho; i++) {
        ordens_venda[i] = atual->order;
        atual = atual->proximo;
    }
    return ordens_venda;
}

Transaction* OrderBook::getTransactions(int* n) {
    int tamanho = 0;
    TransacoesNode* atual = transactions_head;
    while (atual != nullptr) {
        tamanho++;
        atual = atual->proximo;
    }  
    *n = tamanho;
    Transaction* transacoes = new Transaction[tamanho];

    atual = transactions_head;
    for (int i = 0; i < tamanho; i++) {
        transacoes[i] = atual->transaction;
        atual = atual->proximo;
    }
    
    return transacoes;
}

int OrderBook::getNumBuyOrders() {
    return this->n_buy;
}

int OrderBook::getNumSellOrders() {
    return this->n_sell; 
}

void OrderBook::printBuyOrders() {
    OrderNode* atual = orders[0];
    while(atual != nullptr){
        std::cout <<"[ "<< atual->order.getId()  << " | " << atual->order.getPrice() << " | " << atual->order.getTimestamp() << " ] " << std::endl;
        atual = atual->proximo;
    }
}

void OrderBook::printSellOrders() {
    OrderNode* atual = orders[1];
    while(atual != nullptr){
        std::cout <<"[ "<< atual->order.getId()  << " | " << atual->order.getPrice() << " | " << atual->order.getTimestamp() << " ] " << std::endl;
        atual = atual->proximo;
    }
}

void OrderBook::printTransactions() {
    TransacoesNode* atual = transactions_head;
    while(atual != nullptr){
        std::cout <<"[ "<< atual->transaction.getBuyOrderId()  << " | " << atual->transaction.getSellOrderId() << " | " << atual->transaction.getExecutionPrice() << " ] " << std::endl;//[ Id da ordem de compra | Id da ordem de venda | Preço de execução ]
        atual = atual->proximo;
    }
}