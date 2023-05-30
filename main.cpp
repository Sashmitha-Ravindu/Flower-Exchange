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

class Order {
public:
    string clOrdID;
    string orderId = "ord" + to_string(++order_receiveId);
    string instrument;
    int side;
    double price;
    int quantity;
    int status = 0;
    string reason=" - ";
    string transactionTime="";



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

void processBuyOrder(Order ord,vector<Order>& orders,ofstream& outFile,vector<Order>& relavantOrderList){
//    if (ord.instrument == "Rose") {
//
//    } else if (ord.instrument == "Lavender") {
//
//    } else if (ord.instrument == "Lotus") {
//
//    } else if (ord.instrument == "Tulip") {
//
//    } else if (ord.instrument == "Orchid") {
//
//    }

    auto startTime=std::chrono::system_clock::now();

    for (int i = 0; i < orders.size(); ++i) {
        Order currOrder= orders[i];
        if(currOrder.status==1){
            outFile << ord.orderId<<","<<ord.clOrdID<<","<<ord.instrument<<","<<ord.side<<","<<ord.status<<","<<ord.quantity<<","<<ord.price<<","<<ord.transactionTime<<","<<ord.reason<<endl;
        }
        else if (currOrder.price<=ord.price && currOrder.status!=2 && currOrder.status!=1){
            if (ord.quantity==currOrder.quantity){
                ord.status=2;
                currOrder.status=2;
                auto endTime=std::chrono::system_clock::now();
                auto elapsed_seconds=endTime-startTime;
                ord.transactionTime=to_string(elapsed_seconds.count());
                outFile << ord.orderId<<","<<ord.clOrdID<<","<<ord.instrument<<","<<ord.side<<","<<ord.status<<","<<ord.quantity<<","<<ord.price<<","<<ord.transactionTime<<","<<ord.reason<<endl;
                outFile << currOrder.orderId<<","<<currOrder.clOrdID<<","<<currOrder.instrument<<","<<currOrder.side<<","<<currOrder.status<<","<<currOrder.quantity<<","<<currOrder.price<<","<<currOrder.transactionTime<<","<<currOrder.reason<<endl;
            }else if (ord.quantity<currOrder.quantity){
                ord.status=2;
                currOrder.status=3;
                outFile << ord.orderId<<","<<ord.clOrdID<<","<<ord.instrument<<","<<ord.side<<","<<ord.status<<","<<ord.quantity<<","<<ord.price<<","<<ord.transactionTime<<","<<ord.reason<<endl;
                outFile << currOrder.orderId<<","<<currOrder.clOrdID<<","<<currOrder.instrument<<","<<currOrder.side<<","<<currOrder.status<<","<<currOrder.quantity<<","<<currOrder.price<<","<<currOrder.transactionTime<<","<<currOrder.reason<<endl;
            }else if (ord.quantity>currOrder.quantity){
                ord.status=3;
                currOrder.status=2;
                relavantOrderList.push_back(ord);
                sort(relavantOrderList.begin(),relavantOrderList.end(), compare_orders_desc);
                outFile << ord.orderId<<","<<ord.clOrdID<<","<<ord.instrument<<","<<ord.side<<","<<ord.status<<","<<ord.quantity<<","<<ord.price<<","<<ord.transactionTime<<","<<ord.reason<<endl;
                outFile << currOrder.orderId<<","<<currOrder.clOrdID<<","<<currOrder.instrument<<","<<currOrder.side<<","<<currOrder.status<<","<<currOrder.quantity<<","<<currOrder.price<<","<<currOrder.transactionTime<<","<<currOrder.reason<<endl;
            }
        }else if(ord.status==0){
            outFile << ord.orderId<<","<<ord.clOrdID<<","<<ord.instrument<<","<<ord.side<<","<<ord.status<<","<<ord.quantity<<","<<ord.price<<","<<ord.transactionTime<<","<<ord.reason<<endl;
            relavantOrderList.push_back(ord);
            sort(relavantOrderList.begin(),relavantOrderList.end(), compare_orders_desc);
        }
    }
}

void processSellOrder(Order ord,vector<Order>& orders,ofstream& outFile,vector<Order>& relavantOrderList){
    auto startTime=std::chrono::system_clock::now();

    for (int i = 0; i < orders.size(); ++i) {
        Order currOrder= orders[i];
        if(currOrder.status==1){
            outFile << ord.orderId<<","<<ord.clOrdID<<","<<ord.instrument<<","<<ord.side<<","<<ord.status<<","<<ord.quantity<<","<<ord.price<<","<<ord.transactionTime<<","<<ord.reason<<endl;
        }
        else if (currOrder.price>=ord.price && currOrder.status!=2 && currOrder.status!=1){
            if (ord.quantity==currOrder.quantity){
                ord.status=2;
                currOrder.status=2;
                auto endTime=std::chrono::system_clock::now();
                auto elapsed_seconds=endTime-startTime;
                ord.transactionTime=to_string(elapsed_seconds.count());
                outFile << ord.orderId<<","<<ord.clOrdID<<","<<ord.instrument<<","<<ord.side<<","<<ord.status<<","<<ord.quantity<<","<<ord.price<<","<<ord.transactionTime<<","<<ord.reason<<endl;
                outFile << currOrder.orderId<<","<<currOrder.clOrdID<<","<<currOrder.instrument<<","<<currOrder.side<<","<<currOrder.status<<","<<currOrder.quantity<<","<<currOrder.price<<","<<currOrder.transactionTime<<","<<currOrder.reason<<endl;
            }else if (ord.quantity<currOrder.quantity){
                ord.status=2;
                currOrder.status=3;
                outFile << ord.orderId<<","<<ord.clOrdID<<","<<ord.instrument<<","<<ord.side<<","<<ord.status<<","<<ord.quantity<<","<<ord.price<<","<<ord.transactionTime<<","<<ord.reason<<endl;
                outFile << currOrder.orderId<<","<<currOrder.clOrdID<<","<<currOrder.instrument<<","<<currOrder.side<<","<<currOrder.status<<","<<currOrder.quantity<<","<<currOrder.price<<","<<currOrder.transactionTime<<","<<currOrder.reason<<endl;
            }else if (ord.quantity>currOrder.quantity){
                ord.status=3;
                currOrder.status=2;
                relavantOrderList.push_back(ord);
                sort(relavantOrderList.begin(),relavantOrderList.end(), compare_orders_asc);
                outFile << ord.orderId<<","<<ord.clOrdID<<","<<ord.instrument<<","<<ord.side<<","<<ord.status<<","<<ord.quantity<<","<<ord.price<<","<<ord.transactionTime<<","<<ord.reason<<endl;
                outFile << currOrder.orderId<<","<<currOrder.clOrdID<<","<<currOrder.instrument<<","<<currOrder.side<<","<<currOrder.status<<","<<currOrder.quantity<<","<<currOrder.price<<","<<currOrder.transactionTime<<","<<currOrder.reason<<endl;
            }
        }else if(ord.status==0){
            outFile << ord.orderId<<","<<ord.clOrdID<<","<<ord.instrument<<","<<ord.side<<","<<ord.status<<","<<ord.quantity<<","<<ord.price<<","<<ord.transactionTime<<","<<ord.reason<<endl;
            relavantOrderList.push_back(ord);
            sort(relavantOrderList.begin(),relavantOrderList.end(), compare_orders_asc);
        }
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


    ofstream outData;
    outData.open("../execution_rep.csv", ofstream::out | ofstream::trunc);
    outData << "Order ID,Client Order,Instrument,Side,Exec Status,Quantity,Price,Transaction time,Reason" << endl;

//    for (int i = 0; i < orderList.size(); ++i) {
//        outData << orderList[i].getDetails() << endl;
//    }

//    vector<Order> roseOrders;
//
//    roseOrders.push_back(orderList[0]);
//    roseOrders.push_back(orderList[1]);
//
//    roseOrderBook.push_back(roseOrders);
    /*struct minComparator {
        bool compare(Order &or1, Order &or2) {
            if (or1.price != or2.price) {
                return or1.price > or2.price;
            }
            return or1.orderId > or2.orderId;
        }
    };

    struct maxComparator {
        bool compare(Order &or1, Order &or2) {
            if (or1.price != or2.price) {
                return or1.price < or2.price;
            }
            return or1.orderId > or2.orderId;
        }
    };*/

    /*priority_queue<Order, vector<Order>, maxComparator> buyOrders;
    priority_queue<Order, vector<Order>, minComparator> sellOrders;
*/

    for (int i = 0; i < orderList.size(); ++i) {
        Order currOrder = orderList[i];
        if (currOrder.instrument == "Rose") {
            if (currOrder.side == 1) {
                roseBuyOrderBook.push_back(currOrder);
            } else if (currOrder.side == 2) {
                roseSellOrderBook.push_back(currOrder);
            }
        } else if (currOrder.instrument == "Lavender") {
            if (currOrder.side == 1) {
                lavenderBuyOrderBook.push_back(currOrder);
            } else if (currOrder.side == 2) {
                lavenderSellOrderBook.push_back(currOrder);
            }
        } else if (currOrder.instrument == "Lotus") {
            if (currOrder.side == 1) {
                lotusBuyOrderBook.push_back(currOrder);
            } else if (currOrder.side == 2) {
                lotusSellOrderBook.push_back(currOrder);
            }
        } else if (currOrder.instrument == "Tulip") {
            if (currOrder.side == 1) {
                tulipBuyOrderBook.push_back(currOrder);
            } else if (currOrder.side == 2) {
                tulipSellOrderBook.push_back(currOrder);
            }
        } else if (currOrder.instrument == "Orchid") {
            if (currOrder.side == 1) {
                orchidBuyOrderBook.push_back(currOrder);
            } else if (currOrder.side == 2) {
                orchidSellOrderBook.push_back(currOrder);
            }
        }
    }

    sort(roseBuyOrderBook.begin(), roseBuyOrderBook.end(), compare_orders_desc);
    sort(roseSellOrderBook.begin(), roseSellOrderBook.end(), compare_orders_asc);
    sort(lavenderBuyOrderBook.begin(), lavenderBuyOrderBook.end(), compare_orders_desc);
    sort(lavenderSellOrderBook.begin(), lavenderSellOrderBook.end(), compare_orders_asc);
    sort(tulipBuyOrderBook.begin(), tulipBuyOrderBook.end(), compare_orders_desc);
    sort(tulipSellOrderBook.begin(), tulipSellOrderBook.end(), compare_orders_asc);
    sort(lotusBuyOrderBook.begin(), lotusBuyOrderBook.end(), compare_orders_desc);
    sort(lotusSellOrderBook.begin(), lotusSellOrderBook.end(), compare_orders_asc);
    sort(orchidBuyOrderBook.begin(), orchidBuyOrderBook.end(), compare_orders_desc);
    sort(orchidSellOrderBook.begin(), orchidSellOrderBook.end(), compare_orders_asc);

    //process orders

    for (int i = 0; i < orderList.size(); ++i) {
        Order currOrder = orderList[i];
        if (currOrder.instrument == "Rose") {
            if (currOrder.side == 1) {
                processBuyOrder(currOrder,roseSellOrderBook,outData,roseBuyOrderBook);
            } else if (currOrder.side == 2) {
                processBuyOrder(currOrder,roseBuyOrderBook,outData,roseSellOrderBook);
            }
        } else if (currOrder.instrument == "Lavender") {
            if (currOrder.side == 1) {
                processBuyOrder(currOrder,lavenderSellOrderBook,outData,lavenderBuyOrderBook);
            } else if (currOrder.side == 2) {
                processBuyOrder(currOrder,lavenderBuyOrderBook,outData,lavenderSellOrderBook);
            }
        } else if (currOrder.instrument == "Lotus") {
            if (currOrder.side == 1) {
                processBuyOrder(currOrder,lotusSellOrderBook,outData,lotusBuyOrderBook);
            } else if (currOrder.side == 2) {
                processBuyOrder(currOrder,lotusBuyOrderBook,outData,lotusSellOrderBook);
            }
        } else if (currOrder.instrument == "Tulip") {
            if (currOrder.side == 1) {
                processBuyOrder(currOrder,tulipSellOrderBook,outData,tulipBuyOrderBook);
            } else if (currOrder.side == 2) {
                processBuyOrder(currOrder,tulipBuyOrderBook,outData,tulipSellOrderBook);
            }
        } else if (currOrder.instrument == "Orchid") {
            if (currOrder.side == 1) {
                processBuyOrder(currOrder,orchidSellOrderBook,outData,orchidBuyOrderBook);
            } else if (currOrder.side == 2) {
                processBuyOrder(currOrder,orchidBuyOrderBook,outData,orchidSellOrderBook);
            }
        }
    }



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

    /* for (int i = 0; i < roseBuyOrderBook.size(); ++i) {
         cout << roseOrderBook[0][i].getDetails() << endl;
     }*/
//    for (int i = 0; i < roseOrderBook[1].size(); ++i) {
//        cout << roseOrderBook[1][i].getDetails() << endl;
//    }

    outData.close();
    return 0;
}
