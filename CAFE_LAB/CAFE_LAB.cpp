#include "cafe.h"

using namespace std;


int main()
{
	setlocale(LC_ALL, "");
	Cafe test;
	test.addTable(4);
	test.addTable(6); // добавляем несколько столиков
	test.addTable(3);
	test.addTable(4);
	test.addTable(4);

	test.printlistOfTables();

	test.newReservation("Joe", 3);  // бронируем несколько столиков
	test.newReservation("Nick", 4);

	test.printlistOfTables();
	test.printReservationList();

	test.ToMakeOrder(1, "steak", 400);
	test.ToMakeOrder(1, "soup", 250);  // делаем несколько заказов
	test.ToMakeOrder(1, "juice", 100);

	test.ToMakeOrder(2, "Wine", 700);

	test.printReservationList(); // еще раз выводим перечень брони столиков
	test.printListOfOrders();  // выводим все заказы
}

