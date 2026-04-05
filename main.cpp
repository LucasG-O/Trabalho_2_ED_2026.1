#include "OrderBook.hpp"

#include <iostream>

int main() {
    OrderBook orderBook;

    std::cout << "=================================================" << std::endl;
    std::cout << "1. INSERCAO DE ORDENS NAO EXECUTADAS (AGUARDANDO)" << std::endl;
    std::cout << "=================================================" << std::endl;
    // Inserindo Ordens de Compra (B) - Preços mais baixos
    orderBook.submit(Order(1, 'B', 100.0, 1));
    orderBook.submit(Order(2, 'B', 95.0, 2));
    
    // Inserindo Ordens de Venda (S) - Preços mais altos (Nao cruza com as compras)
    orderBook.submit(Order(3, 'S', 110.0, 3));
    orderBook.submit(Order(4, 'S', 115.0, 4));

    orderBook.printTudo();
    std::cout << "\n";


    std::cout << "=================================================" << std::endl;
    std::cout << "2. EXECUCOES BEM-SUCEDIDAS (MATCH)" << std::endl;
    std::cout << "=================================================" << std::endl;
    // Submete uma venda a 98. Como tem alguém querendo comprar a 100 (ID 1), vai executar!
    std::cout << "> Submetendo ordem de Venda (ID 5, preco 98)..." << std::endl;
    orderBook.submit(Order(5, 'S', 98.0, 5));

    // Submete uma compra a 112. Como tem alguém querendo vender a 110 (ID 3), vai executar!
    std::cout << "> Submetendo ordem de Compra (ID 6, preco 112)..." << std::endl;
    orderBook.submit(Order(6, 'B', 112.0, 6));

    std::cout << "\n--- ESTADO DO SISTEMA APOS AS EXECUCOES ---" << std::endl;
    orderBook.printTudo();
    std::cout << "\n";


    std::cout << "=================================================" << std::endl;
    std::cout << "3. CANCELAMENTOS" << std::endl;
    std::cout << "=================================================" << std::endl;
    std::cout << "> Cancelando ordem de compra ID 2..." << std::endl;
    orderBook.cancel(2);
    
    std::cout << "> Cancelando ordem de venda ID 4..." << std::endl;
    orderBook.cancel(4);

    std::cout << "\n--- ESTADO DO SISTEMA APOS CANCELAMENTOS ---" << std::endl;
    orderBook.printTudo(); // Mostra o estado completo do book
    std::cout << "\n";


    std::cout << "=================================================" << std::endl;
    std::cout << "4. RECUPERACAO DOS DADOS (Metodos get...)" << std::endl;
    std::cout << "=================================================" << std::endl;
    
    // Adicionamos uma ordem de teste só para ter o que recuperar no getBuyOrders
    orderBook.submit(Order(7, 'B', 90.0, 7)); 

    // Testando a recuperação de transações
    int qtd_transacoes = 0;
    Transaction* array_transacoes = orderBook.getTransactions(&qtd_transacoes);
    std::cout << "[getTransactions] Retornou " << qtd_transacoes << " transacoes:" << std::endl;
    for (int i = 0; i < qtd_transacoes; ++i) {
        std::cout << "  -> Compra ID: " << array_transacoes[i].getBuyOrderId() 
                  << " | Venda ID: " << array_transacoes[i].getSellOrderId() 
                  << " | Preco Executado: " << array_transacoes[i].getExecutionPrice() << std::endl;
    }
    if (array_transacoes != nullptr) {
        delete[] array_transacoes; // Liberando a memória dinâmica alocada
    }

    // Testando a recuperação de ordens de compra
    int qtd_compras = 0;
    Order* array_compras = orderBook.getBuyOrders(&qtd_compras);
    std::cout << "\n[getBuyOrders] Retornou " << qtd_compras << " ordens:" << std::endl;
    for (int i = 0; i < qtd_compras; ++i) {
        std::cout << "  -> Ordem ID: " << array_compras[i].getId() << " | Preco: " << array_compras[i].getPrice() << std::endl;
    }
    if (array_compras != nullptr) {
        delete[] array_compras; // Liberando a memória dinâmica alocada
    }
    
    // Testando a recuperação de ordens de venda (deve retornar 0 pois cancelamos e executamos todas)
    int qtd_vendas = 0;
    Order* array_vendas = orderBook.getSellOrders(&qtd_vendas);
    std::cout << "\n[getSellOrders] Retornou " << qtd_vendas << " ordens." << std::endl;
    if (array_vendas != nullptr) {
        delete[] array_vendas; // Só deleta se não for nulo
    }

    std::cout << "\n--- ESTADO FINAL COM printTudo() ---" << std::endl;
    orderBook.printTudo();

    std::cout << "\n=== FIM DOS TESTES ===" << std::endl;
    return 0;
}