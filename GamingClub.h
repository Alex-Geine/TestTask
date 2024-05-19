//������������ ���� GamingClub.h
#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <stdlib.h>
#include <queue>

using namespace std;

//��������� ������, �������� �����
struct Time {
	int Hours = 0;		//����
	int Min = 0;		//������

	//�������� ��������� ��� �������
	bool operator>= (Time t) {
		int minutes1 = this->Hours * 60 + this->Min;	//���������� ����� � ������ ��������
		int minutes2 = t.Hours * 60 + t.Min;			//���������� ����� � ������� ��������

		if (minutes1 >= minutes2) return 1;
		return 0;
	}

	//�������� ��������� ��� �������
	bool operator> (Time t) {
		int minutes1 = this->Hours * 60 + this->Min;	//���������� ����� � ������ ��������
		int minutes2 = t.Hours * 60 + t.Min;			//���������� ����� � ������� ��������

		if (minutes1 > minutes2) return 1;
		return 0;
	}
	
	//����� ��������� ��� �������, ������������ ����
	int GetHoursDif(Time t) {
		int minutes1 = this->Hours * 60 + this->Min;	//���������� ����� � ������ ��������
		int minutes2 = t.Hours * 60 + t.Min;			//���������� ����� � ������� ��������
		
		int res = minutes1 - minutes2;

		//���� ����� ��� �������
		if (!(res % 60))	res /= 60;
		else {
			res /= 60;
			res++;
		}
		
		return res;
	}

	//��������� ��������� ��� �������, ������������� 
	Time operator-(Time t) {
		int minutes1 = this->Hours * 60 + this->Min;	//���������� ����� � ������ ��������
		int minutes2 = t.Hours * 60 + t.Min;			//���������� ����� � ������� ��������

		int min = minutes1 - minutes2;

		Time res;
		res.Hours = min / 60;
		res.Min = min % 60;

		return res;
	}

	//�������� �������� ��� �������, ������������ �����
	Time operator+(Time t) {
		int minutes1 = this->Hours * 60 + this->Min;	//���������� ����� � ������ ��������
		int minutes2 = t.Hours * 60 + t.Min;			//���������� ����� � ������� ��������

		int min = minutes1 + minutes2;

		Time res;
		res.Hours = min / 60;
		res.Min = min % 60;

		return res;
	}

	//�����, ��������� ����� � �������
	void Print() {
		if (Hours / 10 < 1) cout << "0" << Hours;
		else cout << Hours;

		cout << ":";

		if (Min / 10 < 1) cout << "0" << Min;
		else cout << Min;
	}
};

//��������� ������ �����
struct Table {
	string name;	//��� �������, �������� �� ������
	int money = 0;		//���������� ������������ �����
	Time time;		//�����, ����������� �� ������
	Time lastTime;	//�����, � ������� ���� �� ����
	bool isBusy = 0;	//����, ���������� �� ��������� �����

	//�����, ����������, ����� ������ ��������
	void ClientSit(string name, Time time);

	//�����, ����������, ����� ������ ������
	void ClientLeft(Time time, int price);
};

//��������� ������ ���� ������
struct Tables {
	vector<Table> tables;

	//�����������
	Tables(int tablesCount) {
		tables.resize(tablesCount);
	}

	//����� ���������� �� ������
	void GetInfo();

	//������ ����� ����� �� ����
	void Sit(int tableId, string clientName, Time time, int price);

	//������ ������ �� �����
	bool Leave(string name, int &tableId, Time time, int price);
};

//��������� ������, ����������� �������
struct Event {
private:
	//��������� ���� ������� � ����������� � id
	bool ValidateEvent(string body, int tablesCount);
public:
	Time time;		//����� �������
	int id = 0;		//id �������
	string name;	//��� �������
	int tableId = 0;	//�� �����

	//�����, ����������� ������ �������
	bool SetInfo(string info,Time &lastTime, int tablesCount);

	//�����, �������� ���������� � ������� � �������
	void GetInfo();
};

//����� ���������� �� ��������� ������� ������
class GamingClub {
private:
	string fileName;		//������ � ��������� �����
	
	Time timeOpening;		//����� ��������
	Time timeClosing;		//����� ��������
	int tablesCount = 0;	//���������� ������
	int price = 0;			//��������� �����
	
	//������� � ���������
	queue<Event> eventsQueue;

	//������� � ������
	queue<string> customersQueue;

	//������ � ���������, ������������ � �����
	vector<string> customers;

	//��������� ������ �� �������
	Tables* tables = nullptr;

	//����� ��� ��������� �� ����� ���������� ������
	bool GetTablesCount(ifstream &file, string &line);

	//����� ��� ��������� ������� ��������/�������� �����
	bool GetTime(ifstream& file, string &line);

	//����� ��� ��������� ��������� ����
	bool GetPrice(ifstream& file, string &line);

	//��������� �������
	void ProcessEvents();

	//��������� ������� "������ ������"
	void ProcessID1(Event &e);

	//��������� ������� "������ ��� �� ����"
	void ProcessID2(Event &e);

	//��������� ������� "������ �������"
	void ProcessID3(Event &e);

	//��������� ������� "������ ����"
	void ProcessID4(Event &e);

	//��������� ������� "������ ����"
	void ProcessID11(Time time, string name);

	//��������� ������� "������ ��� �� ����"
	void ProcessID12(Time time, string name, int tableId);

	//��������� ������� "������"
	void ProcessID13(Time time, string err);

	//���� ��� �������
	void ClientsLeft();
public:
	//����������� ������
	GamingClub(string fileName);

	//�����, �������������� ��������� �����
	void ProcessFile();

	//���������� ������
	~GamingClub();
};