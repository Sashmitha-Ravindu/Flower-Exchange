#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <chrono>

using namespace std;

int order_receiveId = 0;
ofstream outData;

class Order {
public:
    string clOrdID;
    string orderId = "ord" + to_string(++order_receiveId);
    string instrument;
    int side;
    double price;
    int quantity;
    int status = 0;
    string reason = " - ";
    string transactionTime = " ";


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

bool compare_orders_asc(Order ord1, Order ord2) {
    return ord1.price < ord2.price;
}

bool compare_orders_desc(Order ord1, Order ord2) {
    return ord1.price > ord2.price;
}

void processBuyOrder(Order ord, vector<Order> &orders, vector<Order> &relavantOrderList) {

    auto startTime = std::chrono::system_clock::now();

    for (int i = 0; i < orders.size(); ++i) {
        Order currOrder = orders[i];
        if (currOrder.status == 1) {
            auto endTime = std::chrono::system_clock::now();
            auto elapsed_seconds = endTime - startTime;
            currOrder.transactionTime = to_string(elapsed_seconds.count());
            outData << ord.orderId << "," << ord.clOrdID << "," << ord.instrument << "," << ord.side << ","
                    << ord.status << "," << ord.quantity << "," << ord.price << "," << ord.transactionTime << ","
                    << ord.reason << endl;
        } else if (currOrder.price <= ord.price && currOrder.status != 2 && currOrder.quantity > 0) {
            if (ord.quantity == currOrder.quantity) {

                ord.status = 2;
                currOrder.status = 2;
                auto endTime = std::chrono::system_clock::now();
                auto elapsed_seconds = endTime - startTime;
                cout << to_string(elapsed_seconds.count()) << endl;
                cout << to_string(elapsed_seconds.count()) << endl;
                ord.transactionTime = to_string(elapsed_seconds.count());
                outData << ord.orderId << "," << ord.clOrdID << "," << ord.instrument << "," << ord.side << ","
                        << ord.status << "," << ord.quantity << "," << currOrder.price << "," << ord.transactionTime
                        << "," << ord.reason << endl;
                outData << currOrder.orderId << "," << currOrder.clOrdID << "," << currOrder.instrument << ","
                        << currOrder.side << "," << currOrder.status << "," << currOrder.quantity << ","
                        << currOrder.price << "," << currOrder.transactionTime << "," << currOrder.reason << endl;
                currOrder.quantity = 0;
                ord.quantity = 0;
                outData.flush();
            } else if (ord.quantity < currOrder.quantity) {

                ord.status = 2;
                currOrder.status = 3;
                outData << ord.orderId << "," << ord.clOrdID << "," << ord.instrument << "," << ord.side << ","
                        << ord.status << "," << ord.quantity << "," << currOrder.price << "," << ord.transactionTime
                        << ","
                        << ord.reason << endl;
                outData << currOrder.orderId << "," << currOrder.clOrdID << "," << currOrder.instrument << ","
                        << currOrder.side << "," << currOrder.status << "," << ord.quantity << ","
                        << currOrder.price << "," << currOrder.transactionTime << "," << currOrder.reason << endl;
                currOrder.quantity -= ord.quantity;
                ord.quantity = 0;
            } else if (ord.quantity > currOrder.quantity) {

                ord.status = 3;
                currOrder.status = 2;
                relavantOrderList.push_back(ord);
                sort(relavantOrderList.begin(), relavantOrderList.end(), compare_orders_desc);
                outData << ord.orderId << "," << ord.clOrdID << "," << ord.instrument << "," << ord.side << ","
                        << ord.status << "," << currOrder.quantity << "," << currOrder.price << ","
                        << ord.transactionTime
                        << "," << ord.reason << endl;
                outData << currOrder.orderId << "," << currOrder.clOrdID << "," << currOrder.instrument << ","
                        << currOrder.side << "," << currOrder.status << "," << currOrder.quantity << ","
                        << currOrder.price << "," << currOrder.transactionTime << "," << currOrder.reason << endl;
                ord.quantity -= currOrder.quantity;
                currOrder.quantity = 0;
            }
        }
    }
    if (ord.status == 0) {
        outData << ord.orderId << "," << ord.clOrdID << "," << ord.instrument << "," << ord.side << ","
                << ord.status << "," << ord.quantity << "," << ord.price << "," << ord.transactionTime << ","
                << ord.reason << endl;
        relavantOrderList.push_back(ord);
        sort(relavantOrderList.begin(), relavantOrderList.end(), compare_orders_desc);

    }
}

void processSellOrder(Order ord, vector<Order> &orders, vector<Order> &relavantOrderList) {
    auto startTime = std::chrono::system_clock::now();

    for (int i = 0; i < orders.size(); ++i) {
        Order currOrder = orders[i];
        if (currOrder.status == 1) {
            auto endTime = std::chrono::system_clock::now();
            auto elapsed_seconds = endTime - startTime;
            currOrder.transactionTime = to_string(elapsed_seconds.count());
            outData << ord.orderId << "," << ord.clOrdID << "," << ord.instrument << "," << ord.side << ","
                    << ord.status << "," << ord.quantity << "," << ord.price << "," << ord.transactionTime << ","
                    << ord.reason << endl;
        } else if (currOrder.price >= ord.price && currOrder.status != 2 &&
                   currOrder.quantity > 0) {
            if (ord.quantity == currOrder.quantity) {

                ord.status = 2;
                currOrder.status = 2;
                auto endTime = std::chrono::system_clock::now();
                auto elapsed_seconds = endTime - startTime;
                ord.transactionTime = to_string(elapsed_seconds.count());
                outData << ord.orderId << "," << ord.clOrdID << "," << ord.instrument << "," << ord.side << ","
                        << ord.status << "," << ord.quantity << "," << currOrder.price << "," << ord.transactionTime
                        << ","
                        << ord.reason << endl;
                outData << currOrder.orderId << "," << currOrder.clOrdID << "," << currOrder.instrument << ","
                        << currOrder.side << "," << currOrder.status << "," << currOrder.quantity << ","
                        << currOrder.price << "," << currOrder.transactionTime << "," << currOrder.reason << endl;
                currOrder.quantity = 0;
                ord.quantity = 0;
            } else if (ord.quantity < currOrder.quantity) {
                currOrder.quantity -= ord.quantity;
                ord.status = 2;
                currOrder.status = 3;
                outData << ord.orderId << "," << ord.clOrdID << "," << ord.instrument << "," << ord.side << ","
                        << ord.status << "," << ord.quantity << "," << currOrder.price << "," << ord.transactionTime
                        << ","
                        << ord.reason << endl;
                outData << currOrder.orderId << "," << currOrder.clOrdID << "," << currOrder.instrument << ","
                        << currOrder.side << "," << currOrder.status << "," << ord.quantity << ","
                        << currOrder.price << "," << currOrder.transactionTime << "," << currOrder.reason << endl;

                ord.quantity = 0;
            } else if (ord.quantity > currOrder.quantity) {
                ord.quantity -= currOrder.quantity;
                ord.status = 3;
                currOrder.status = 2;
                relavantOrderList.push_back(ord);
                sort(relavantOrderList.begin(), relavantOrderList.end(), compare_orders_asc);
                outData << ord.orderId << "," << ord.clOrdID << "," << ord.instrument << "," << ord.side << ","
                        << ord.status << "," << currOrder.quantity << "," << currOrder.price << ","
                        << ord.transactionTime
                        << ","
                        << ord.reason << endl;
                outData << currOrder.orderId << "," << currOrder.clOrdID << "," << currOrder.instrument << ","
                        << currOrder.side << "," << currOrder.status << "," << currOrder.quantity << ","
                        << currOrder.price << "," << currOrder.transactionTime << "," << currOrder.reason << endl;
                currOrder.quantity = 0;

            }
        }
    }
    if (ord.status == 0) {
        outData << ord.orderId << "," << ord.clOrdID << "," << ord.instrument << "," << ord.side << ","
                << ord.status << "," << ord.quantity << "," << ord.price << "," << ord.transactionTime << ","
                << ord.reason << endl;

        relavantOrderList.push_back(ord);
        sort(relavantOrderList.begin(), relavantOrderList.end(), compare_orders_asc);
    }
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
    int m=0;
    while (ip.good()) {
        getline(ip, clOrdID, ',');
        getline(ip, instrument, ',');
        getline(ip, side, ',');
        getline(ip, quantity, ',');
        getline(ip, price, '\n');

        Order order1(clOrdID, instrument, stoi(side), stoi(quantity), stod(price));

        validateOrder(order1);

        orderList.push_back(order1);
//        cout << ++m << endl;
//        cout <<order1.getDetails() << endl;
    }

    vector<Order> roseBuyOrderBook;
    vector<Order> roseSellOrderBook;
    vector<Order> lavenderBuyOrderBook;
    vector<Order> lavenderSellOrderBook;
    vector<Order> lotusBuyOrderBook;
    vector<Order> lotusSellOrderBook;
    vector<Order> tulipBuyOrderBook;
    vector<Order> tulipSellOrderBook;
    vector<Order> orchidBuyOrderBook;
    vector<Order> orchidSellOrderBook;


    outData.open("../execution_rep.csv", ofstream::out | ofstream::trunc);
    outData << "Order ID,Client Order,Instrument,Side,Exec Status,Quantity,Price,Transaction time,Reason" << endl;

    //process orders

    for (int i = 0; i < orderList.size(); ++i) {
        Order currOrder = orderList[i];
        if (currOrder.status == 1) {
            outData << currOrder.orderId << "," << currOrder.clOrdID << "," << currOrder.instrument << ","
                    << currOrder.side << ","
                    << currOrder.status << "," << currOrder.quantity << "," << currOrder.price << ","
                    << currOrder.transactionTime << ","
                    << currOrder.reason << endl;
            continue;
        }
        else if (currOrder.instrument == "Rose") {
            if (currOrder.side == 1) {
                processBuyOrder(currOrder, roseSellOrderBook, roseBuyOrderBook);
            } else if (currOrder.side == 2) {
                processSellOrder(currOrder, roseBuyOrderBook, roseSellOrderBook);
            }
        } else if (currOrder.instrument == "Lavender") {
            if (currOrder.side == 1) {
                processBuyOrder(currOrder, lavenderSellOrderBook, lavenderBuyOrderBook);
            } else if (currOrder.side == 2) {
                processSellOrder(currOrder, lavenderBuyOrderBook, lavenderSellOrderBook);
            }
        } else if (currOrder.instrument == "Lotus") {
            if (currOrder.side == 1) {
                processBuyOrder(currOrder, lotusSellOrderBook, lotusBuyOrderBook);
            } else if (currOrder.side == 2) {
                processSellOrder(currOrder, lotusBuyOrderBook, lotusSellOrderBook);
            }
        } else if (currOrder.instrument == "Tulip") {
            if (currOrder.side == 1) {
                processBuyOrder(currOrder, tulipSellOrderBook, tulipBuyOrderBook);
            } else if (currOrder.side == 2) {
                processSellOrder(currOrder, tulipBuyOrderBook, tulipSellOrderBook);
            }
        } else if (currOrder.instrument == "Orchid") {
            if (currOrder.side == 1) {
                processBuyOrder(currOrder, orchidSellOrderBook, orchidBuyOrderBook);
            } else if (currOrder.side == 2) {
                processSellOrder(currOrder, orchidBuyOrderBook, orchidSellOrderBook);
            }
        }
    }



    outData.flush();
    outData.close();
    return 0;
}
