//заголовочный файл GamingClub.h
#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <stdlib.h>
#include <queue>

using namespace std;

//структура данных, хранящая время
struct Time {
	int Hours = 0;		//часы
	int Min = 0;		//минуты

	//оператор сравнения для времени
	bool operator>= (Time t) {
		int minutes1 = this->Hours * 60 + this->Min;	//количество минут у левого операнда
		int minutes2 = t.Hours * 60 + t.Min;			//количество минут у правого операнда

		if (minutes1 >= minutes2) return 1;
		return 0;
	}

	//оператор сравнения для времени
	bool operator> (Time t) {
		int minutes1 = this->Hours * 60 + this->Min;	//количество минут у левого операнда
		int minutes2 = t.Hours * 60 + t.Min;			//количество минут у правого операнда

		if (minutes1 > minutes2) return 1;
		return 0;
	}
	
	//метод вычитания для времени, возвращающий часы
	int GetHoursDif(Time t) {
		int minutes1 = this->Hours * 60 + this->Min;	//количество минут у левого операнда
		int minutes2 = t.Hours * 60 + t.Min;			//количество минут у правого операнда
		
		int res = minutes1 - minutes2;

		//если делим без остатка
		if (!(res % 60))	res /= 60;
		else {
			res /= 60;
			res++;
		}
		
		return res;
	}

	//опрератор вычитания для времени, возвращающиай 
	Time operator-(Time t) {
		int minutes1 = this->Hours * 60 + this->Min;	//количество минут у левого операнда
		int minutes2 = t.Hours * 60 + t.Min;			//количество минут у правого операнда

		int min = minutes1 - minutes2;

		Time res;
		res.Hours = min / 60;
		res.Min = min % 60;

		return res;
	}

	//оператор сложения для времени, возвращающий время
	Time operator+(Time t) {
		int minutes1 = this->Hours * 60 + this->Min;	//количество минут у левого операнда
		int minutes2 = t.Hours * 60 + t.Min;			//количество минут у правого операнда

		int min = minutes1 + minutes2;

		Time res;
		res.Hours = min / 60;
		res.Min = min % 60;

		return res;
	}

	//метод, выводящий время в консоль
	void Print() {
		if (Hours / 10 < 1) cout << "0" << Hours;
		else cout << Hours;

		cout << ":";

		if (Min / 10 < 1) cout << "0" << Min;
		else cout << Min;
	}
};

//структура данных стола
struct Table {
	string name;	//имя клиента, сидящего за столом
	int money = 0;		//количество заработанных денег
	Time time;		//время, проведенное за столом
	Time lastTime;	//время, в которое сели за стол
	bool isBusy = 0;	//флаг, отвечающай за занятость стола

	//метод, вызываемый, когда клиент приходит
	void ClientSit(string name, Time time);

	//метод, вызываемый, когда клиент уходит
	void ClientLeft(Time time, int price);
};

//структура данных всех столов
struct Tables {
	vector<Table> tables;

	//конструктор
	Tables(int tablesCount) {
		tables.resize(tablesCount);
	}

	//вывод информации по столам
	void GetInfo();

	//клиент хочет сесть за стол
	void Sit(int tableId, string clientName, Time time, int price);

	//клиент уходит со стола
	bool Leave(string name, int &tableId, Time time, int price);
};

//структура данных, описывающая событие
struct Event {
private:
	//валидация тела события в соответсвии с id
	bool ValidateEvent(string body, int tablesCount);
public:
	Time time;		//время события
	int id = 0;		//id события
	string name;	//имя клиента
	int tableId = 0;	//ид стола

	//метод, заполняющий данные события
	bool SetInfo(string info,Time &lastTime, int tablesCount);

	//метод, выдающий информацию о событии в консоль
	void GetInfo();
};

//класс отвечающий за обработку входных данных
class GamingClub {
private:
	string fileName;		//строка с названием файла
	
	Time timeOpening;		//время открытия
	Time timeClosing;		//время закрытия
	int tablesCount = 0;	//количество столов
	int price = 0;			//стоимость стола
	
	//очередь с событиями
	queue<Event> eventsQueue;

	//очередь с людьми
	queue<string> customersQueue;

	//массив с клиентами, находящимися в клубе
	vector<string> customers;

	//структура данных со столами
	Tables* tables = nullptr;

	//метод для получения из файла количества столов
	bool GetTablesCount(ifstream &file, string &line);

	//метод для получения времени открытия/закрытия клуба
	bool GetTime(ifstream& file, string &line);

	//метод для получения стоимости часа
	bool GetPrice(ifstream& file, string &line);

	//обработка событий
	void ProcessEvents();

	//обработка события "Клиент пришел"
	void ProcessID1(Event &e);

	//обработка события "Клиент сел за стол"
	void ProcessID2(Event &e);

	//обработка события "Клиент ожидает"
	void ProcessID3(Event &e);

	//обработка события "Клиент ушел"
	void ProcessID4(Event &e);

	//исходящее событие "Клиент ушел"
	void ProcessID11(Time time, string name);

	//исходящее событие "Клиент сел за стол"
	void ProcessID12(Time time, string name, int tableId);

	//исходящее событие "Ошибка"
	void ProcessID13(Time time, string err);

	//ушли все клиенты
	void ClientsLeft();
public:
	//конструктор класса
	GamingClub(string fileName);

	//метод, осуществляющий обработку файла
	void ProcessFile();

	//деструктор класса
	~GamingClub();
};