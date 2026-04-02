#include "Order.hpp"
#include "Transaction.hpp"


class OrderBook {

private:

struct OrderNode {
    Order sell_orders;
    Order* next_sell;
};

int n_sell;
int n_buy;
struct list_transaction {

};





    // Estruturas internas escolhidas pelos alunos
    // para armazenar ordens de compra, venda e transações
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