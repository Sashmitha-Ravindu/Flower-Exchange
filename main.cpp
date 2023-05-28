#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "Order.h"

using namespace std;


bool isOrderValid(string clOrdId, string instrument, string side, string quantity, string price) {
    string instrumentList[] = {"Rose", "Lavender", "Lotus", "Tulip", "Orchid"};
    return !clOrdId.empty() &&
           !instrument.empty() &&
           !side.empty() &&
           !quantity.empty() &&
           !price.empty() &&
           (instrument == "Rose" || instrument == "Lavender" || instrument == "Lotus" || instrument == "Tulip" ||
            instrument == "Orchid") &&
           (stoi(side) == 1 || stoi(side) == 2) &&
           stod(price) > 0.0 &&
           stoi(quantity) % 10 == 0 &&
           stoi(quantity) >= 10 && stoi(quantity) <= 1000;
}

bool compare_orders(Order ord1, Order ord2) {
    return ord1.getPrice() < ord2.getPrice();
}

int main() {
    vector<Order> orderList;
    ifstream ip("../order.csv");
    if (!ip.is_open()) {
        cout << "ERROR: Can't Open File" << '\n';
    }

    string clOrdID;
    string instrument;
    string side;
    string quantity;
    string price;

    getline(ip, clOrdID, ',');
    getline(ip, instrument, ',');
    getline(ip, side, ',');
    getline(ip, quantity, ',');
    getline(ip, price, '\n');

    while (ip.good()) {
        getline(ip, clOrdID, ',');
        getline(ip, instrument, ',');
        getline(ip, side, ',');
        getline(ip, quantity, ',');
        getline(ip, price, '\n');

        if (!isOrderValid(clOrdID, instrument, side, quantity, price)) {
            cout << "Invalid Input" << endl;
            return 1;
        }

        Order order1(clOrdID, instrument, stoi(side), stoi(quantity), stod(price));
        orderList.push_back(order1);
    }

    vector<vector<Order>> roseOrderBook;
    vector<vector<Order>> lavenderOrderBook;
    vector<vector<Order>> lotusOrderBook;
    vector<vector<Order>> tulipOrderBook;
    vector<vector<Order>> orchidOrderBook;


    ofstream outData;
    outData.open("../execution_rep.csv", ofstream::out | ofstream::trunc);
    outData << "Order ID,Client Order,Instrument,Side,Exec Status,Quantity,Price,Reason" << endl;
//    for (int i = 0; i < orderList.size(); ++i) {
//        outData << orderList[i].getDetails() << endl;
//    }

    vector<Order> roseOrders;

    roseOrders.push_back(orderList[0]);
    roseOrders.push_back(orderList[1]);

    roseOrderBook.push_back(roseOrders);




//    for (int i = 0; i < orderList.size(); i++) {
//        string type = orderList[i].getInstrument();
//        if (type == "Rose") {
//            roseOrderBook[orderList[i].getSide() - 1].push_back(orderList[i]);
//            sort(roseOrderBook[orderList[i].getSide() - 1].begin(), roseOrderBook[orderList[i].getSide() - 1].end(),
//                 compare_orders);
//        } else if (type == "Lavender") {
//            lavenderOrderBook[orderList[i].getSide() - 1].push_back(orderList[i]);
//            sort(roseOrderBook[orderList[i].getSide() - 1].begin(), roseOrderBook[orderList[i].getSide() - 1].end(),
//                 compare_orders);
//        } else if (type == "Lotus") {
//            lotusOrderBook[orderList[i].getSide() - 1].push_back(orderList[i]);
//            sort(roseOrderBook[orderList[i].getSide() - 1].begin(), roseOrderBook[orderList[i].getSide() - 1].end(),
//                 compare_orders);
//        } else if (type == "Tulip") {
//            tulipOrderBook[orderList[i].getSide() - 1].push_back(orderList[i]);
//            sort(roseOrderBook[orderList[i].getSide() - 1].begin(), roseOrderBook[orderList[i].getSide() - 1].end(),
//                 compare_orders);
//        } else if (type == "Orchid") {
//            orchidOrderBook[orderList[i].getSide() - 1].push_back(orderList[i]);
//            sort(roseOrderBook[orderList[i].getSide() - 1].begin(), roseOrderBook[orderList[i].getSide() - 1].end(),
//                 compare_orders);
//        }
//    }

    for (int i = 0; i < roseOrderBook[0].size(); ++i) {
        cout << roseOrderBook[0][i].getDetails() << endl;
    }
//    for (int i = 0; i < roseOrderBook[1].size(); ++i) {
//        cout << roseOrderBook[1][i].getDetails() << endl;
//    }

    outData.close();
    return 0;
}
