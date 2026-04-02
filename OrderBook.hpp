#include "Order.hpp"
#include "Transaction.hpp"


class OrderBook {

private:

struct OrderNode {
    Order order;
    OrderNode* anterior; // Ponteiro para a ordem anterior
    OrderNode* proximo; // Ponteiro para a próxima ordem 
};

struct TransacoesNode{
    Transaction transaction;
    TransacoesNode* next;
};

int n_sell;
int n_buy;

OrderNode* orders[2]; // 0 para ordens de compra, 1 para ordens de venda
TransacoesNode* transactions_head;


public:

    OrderBook(int n_sell, int n_buy);
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