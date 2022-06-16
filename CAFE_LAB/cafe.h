#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "countainer.h"

using namespace std;

class Table
{
    int numberOfTable;
    int maxPersons;
    bool isFree;


public:


    explicit Table(int numberOfTable_, int maxPersons_, bool isFree_ = true) : numberOfTable{ numberOfTable_ },
        maxPersons{ maxPersons_ }, isFree{ isFree_ } {} // основной инициализирующий конструктор
    Table() : numberOfTable{ 0 }, maxPersons{ 0 }, isFree{ false } {} // конструктор по умолчанию
    Table(const Table&) = delete; // удаляем конструктор перемещения
    Table& operator=(Table&& other) noexcept // перегруженный оператор = для конструктора перемещения
    {
        if (this != &other)
        {
            numberOfTable = other.numberOfTable;
            maxPersons = other.maxPersons;
            isFree = other.isFree;
            other.numberOfTable = 0;
            other.maxPersons = 0;
            other.isFree = false;
        }
        return *this;
    }
    int getTableNumber() { return numberOfTable; }
    friend ostream& operator << (ostream& out, Table& object) // перегрузка оператора вывода объекта
    {
        string tmp = (object.isFree) ? "свободен" : "занят";
        out << "Столик № " << object.numberOfTable << " на " << object.maxPersons << " персон " << tmp;
        return out;
    }
    int getMaxNumberOfPersons() { return maxPersons; }
    void setFree(bool flag) { isFree = flag; }
    bool isTableFree() { return isFree; }
};

class Reservation
{
    string clientName;
    int numberOfTable;
    int Bill;
    int numberOfOrder;


public:


    Reservation(string clientName_, int numberOfTable_, int numberOfOrder_, int Bill_ = 0) :
        clientName{ clientName_ }, numberOfTable{ numberOfTable_ },
        numberOfOrder{ numberOfOrder_ }, Bill{ Bill_ } {}
    Reservation() :
        clientName{ "Not recorded" }, numberOfTable{ 0 },
        numberOfOrder{ 0 }, Bill{ 0 } {}
    friend ostream& operator << (ostream& out, Reservation& object) // перегрузка оператора вывода объекта
    {
        out << object.clientName << " Столик №: " << object.numberOfTable << " | Номер заказа: " << object.numberOfOrder
            << " | Суммарный счет: " << object.Bill;
        return out;
    }
    int getNumberOfOrder()
    {
        return numberOfOrder;
    }
    int getTableNumber()
    {
        return numberOfTable;
    }
    void setNumberOfOrder(int num) { numberOfOrder = num; }

    void increaseBill(int sum) { Bill += sum; }
};

class Order
{
    struct Dish
    {
        string NameOfDish;
        int cost;
        Dish(string NameOfDish_, int cost_) : NameOfDish{ NameOfDish_ }, cost{ cost_ } {}
    };
    int numberOfOrder;
    int numberOfTable;
    vector<Dish*> sequenceOfOrderedDishes;
public:
    Order(int numberOfOrder_, int numberOfTable_) :
        numberOfOrder{ numberOfOrder_ }, numberOfTable{ numberOfTable_ }, sequenceOfOrderedDishes{} {}
    Order() :
        numberOfOrder{ 0 }, numberOfTable{ 0 }, sequenceOfOrderedDishes{} {}
    friend ostream& operator << (ostream& out, Order& object) // перегрузка оператора вывода объекта
    {
        out << "Заказ №: " << object.numberOfOrder << " Столик №: " << object.numberOfTable << endl;
        for (auto dish : object.sequenceOfOrderedDishes)
            out << "    <" << dish->NameOfDish << " | " << dish->cost << ">" << endl;
        return out;
    }
    void addToOrder(string NameOfDish, int cost)
    {
        Dish* tmp = new Dish(NameOfDish, cost);
        sequenceOfOrderedDishes.push_back(tmp);
    }
    int getNumOfOrder() { return numberOfOrder; }
};


class Cafe //применение паттерна wrapper
{
    UserVector<Table*> listOfTables;
    UserVector<Reservation*> reservationList;
    UserVector<Order*> listOfOrders;

    void addToOrderList(int numoforder, string dish, int cost)
    {
        for (auto& item : listOfOrders)
            if (item.data->getNumOfOrder() == numoforder)
            {
                item.data->addToOrder(dish, cost);
                break;
            }
    }

public:
    Cafe() : listOfTables{}, reservationList{}, listOfOrders{} {}
    void printlistOfTables()
    {
        for (const auto& table : listOfTables)
            cout << *table.data << endl;
        cout << endl;
    }

    void printReservationList()
    {
        for (const auto& reserv : reservationList)
            cout << *reserv.data << endl;
        cout << endl;
    }

    void printListOfOrders()
    {
        for (const auto& order : listOfOrders)
            cout << *order.data << endl;
        cout << endl;
    }

    void addTable(int maxpersons) // добавляем новый столик в базу
    {
        if (maxpersons <= 6)
        {
            Table* tmp = new Table(listOfTables.get_size() + 1, maxpersons);
            listOfTables.push_back(tmp);
        }
        else
            cout << "6 persons per table max" << endl;
    }

    void newReservation(string clientName, int persons)  // делаем бронь столика
    {
        int goodTable{};
        for (const auto& item : listOfTables)
            if (item.data->getMaxNumberOfPersons() >= persons && item.data->isTableFree()) // если есть подходящий по размеру
            {
                goodTable = item.data->getTableNumber();   // получаем его номер
                item.data->setFree(false);                   // отмечаем как занятый
                break;
            }
        if (goodTable)
        {
            Reservation* tmp = new Reservation(clientName, goodTable, 0); // добавляем запись в базу
            reservationList.push_back(tmp);
        }
        else
            cout << "Свободного столика нет" << endl;
    }

    void ToMakeOrder(int tableNum, string NameOfDish, int price)  // делаем заказ
    {
        for (auto& item : reservationList)
            if (item.data->getTableNumber() == tableNum)
            {
                if (item.data->getNumberOfOrder() == 0)
                {
                    Order* tmp = new Order(listOfOrders.get_size() + 1, tableNum);
                    listOfOrders.push_back(tmp);
                    addToOrderList(listOfOrders.get_size(), NameOfDish, price);

                    item.data->setNumberOfOrder(listOfOrders.get_size());
                    item.data->increaseBill(price);
                }
                else
                {
                    addToOrderList(item.data->getNumberOfOrder(), NameOfDish, price);
                    item.data->increaseBill(price);
                }
                break;
            }
    }
};
