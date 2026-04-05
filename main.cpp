##include "OrderBook.hpp"
#include "Order.hpp"
#include "Transaction.hpp"
#include <iostream>

int main() {
    OrderBook orderBook;

    // Submeter algumas ordens de compra e venda
    orderBook.submit(Order(1, 'B', 100.0, 10));
    orderBook.submit(Order(2, 'S', 99.0, 5));
    orderBook.submit(Order(3, 'B', 101.0, 15));
    orderBook.submit(Order(4, 'S', 98.0, 20));


    // Imprimir as ordens de compra e venda
    std::cout << "Ordens de Compra:" << std::endl;
    orderBook.printBuyOrders();

    std::cout << "Ordens de Venda:" << std::endl;
    orderBook.printSellOrders();

    // Cancelar uma ordem
    orderBook.cancel(2);

    // Imprimir as ordens de compra e venda após o cancelamento
    std::cout << "Ordens de Compra após cancelamento:" << std::endl;
    orderBook.printBuyOrders();

    std::cout << "Ordens de Venda após cancelamento:" << std::endl;
    orderBook.printSellOrders();

    // Imprimir as transações realizadas
    std::cout << "Transações Realizadas:" << std::endl;
    orderBook.printTransactions();

    return 0;
}

