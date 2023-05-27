//
// Created by sashmitha on 5/27/23.
//

#ifndef FLOWEREXCHANGE_ORDER_H
#define FLOWEREXCHANGE_ORDER_H


#include <iostream>
#include <string>

using namespace std;

class Order {
private:
    string clOrdID;
    string instrument;
    int side;
    int quantity;
    double price;
public:
    Order(const string &clOrdId, const string &instrument, int side, int quantity, double price) : clOrdID(clOrdId),
                                                                                                   instrument(
                                                                                                           instrument),
                                                                                                   side(side),
                                                                                                   quantity(quantity),
                                                                                                   price(price) {}

    const string &getClOrdId() const {
        return clOrdID;
    }

    void setClOrdId(const string &clOrdId) {
        clOrdID = clOrdId;
    }

    const string &getInstrument() const {
        return instrument;
    }

    void setInstrument(const string &instrument) {
        Order::instrument = instrument;
    }

    int getSide() const {
        return side;
    }

    void setSide(int side) {
        Order::side = side;
    }

    int getQuantity() const {
        return quantity;
    }

    void setQuantity(int quantity) {
        Order::quantity = quantity;
    }

    double getPrice() const {
        return price;
    }

    void setPrice(double price) {
        Order::price = price;
    }

    void print() {
        cout << "clOrdID: " << clOrdID << '\n';
        cout << "instrument: " << instrument << '\n';
        cout << "side: " << side << '\n';
        cout << "quantity: " << quantity << '\n';
        cout << "price: " << price << '\n';
    }

    string sendPrint() {
        return clOrdID + "," + instrument + "," + to_string(side) + "," + to_string(quantity) + "," + to_string(price);
    }
};


#endif //FLOWEREXCHANGE_ORDER_H
