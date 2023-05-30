#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int order_receiveId = 0;

class Order {
public:
    string clOrdID;
    string orderId = "ord" + to_string(++order_receiveId);
    string instrument;
    int side;
    double price;
    int quantity;
    int status = 0;
    string reason;
    string transactionTime;


    Order(const string &clOrdId, const string &instrument, int side, int quantity, double price) : clOrdID(clOrdId),
                                                                                                   instrument(
                                                                                                           instrument),
                                                                                                   side(side),
                                                                                                   quantity(quantity),
                                                                                                   price(price) {}


    string getDetails() {
        return clOrdID + "," + instrument + "," + to_string(side) + "," + to_string(quantity) + "," + to_string(price);
    }
};


void validateOrder(Order &ord) {
    if (ord.clOrdID.empty()) {
        ord.status = 1;
        ord.reason = "Invalid Client Order ID";
    } else if (ord.instrument.empty() ||
               !(ord.instrument == "Rose" || ord.instrument == "Lavender" || ord.instrument == "Lotus" ||
                 ord.instrument == "Tulip" || ord.instrument == "Orchid")) {
        ord.status = 1;
        ord.reason = "Invalid instrument";
    } else if (ord.side != 1 && ord.side != 2) {
        ord.status = 1;
        ord.reason = "Invalid side";
    } else if (ord.price <= 0) {
        ord.status = 1;
        ord.reason = "Invalid price";
    } else if (ord.quantity < 10 || ord.quantity >= 1000 || ord.quantity % 10 != 0) {
        ord.status = 1;
        ord.reason = "Invalid quantity";
    }
}

/*bool compare_orders(Order ord1, Order ord2) {
    return ord1.getPrice() < ord2.getPrice();
}*/

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

        Order order1(clOrdID, instrument, stoi(side), stoi(quantity), stod(price));

        validateOrder(order1);

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

//    vector<Order> roseOrders;
//
//    roseOrders.push_back(orderList[0]);
//    roseOrders.push_back(orderList[1]);
//
//    roseOrderBook.push_back(roseOrders);

    struct minComparator {
        bool compare(Order &or1, Order &or2) {
            if (or1.price != or2.price) {
                return or1.price > or2.price;
            }

        }
    };



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
