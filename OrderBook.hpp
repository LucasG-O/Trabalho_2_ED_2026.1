#include "Order.hpp"
#include "Transaction.hpp"


class OrderBook {

private:

    struct OrderNode {
        Order order;
        OrderNode* proximo; // Ponteiro para a próxima ordem 
    };

    struct TransacoesNode{
        Transaction transaction;
        TransacoesNode* proximo;
    };

    int n_sell;
    int n_buy;

    OrderNode* orders[2]; // 0 para ordens de compra, organizada em ordem decrescente de preço
                          // 1 para ordens de venda, organizada em ordem crescente de preço
    TransacoesNode* transactions_head;

    void armazenarOrdem(Order order, OrderNode* *head_lista);
    void executarTransacao(Order order_sell, Order order_buy, char tipo_ordem);


public:

    OrderBook();
    ~OrderBook();

    bool submit(Order order);
    bool cancel(int id);

    
    Order* getBuyOrders(int* n);
    Order* getSellOrders(int* n);
    Transaction* getTransactions(int* n);

    int getNumBuyOrders();
    int getNumSellOrders();
    void printBuyOrders();
    void printSellOrders();
    void printTransactions();

    // Outros métodos auxiliares, se necessário
};