//���� ���������� GamingClub.h

#include "GamingClub.h"

//���������� ������ Event

//��������� ���� ������� � ����������� � id
bool Event::ValidateEvent(string body, int tablesCount) {
    if (id == 1 || id == 3 || id == 4) {        //��� ���� ������� � ���� ��������� ������ ���
        //��������� ������ � ����������� � ��������

        //������ �� ��� ������ ������������ gcc �� �������� ����� ������ � ���������� ������ "^[0-9]+$" ������ � regex_match, ������� ������� �� search (���� ��� ���������� VisualStudio ��� ��������???)
        //��, �������������, ������� ����� �� ������������ �������������� �� �����
        regex eventReg("[0-9a-z_\\-]+");
        if (regex_search(body.begin(), body.end(), eventReg)) name = body;
        else return 0;
    }
    else {      //��� ����� ������� ��������� ��� � ����� �����
        //��������� ������ � ����������� � ��������

        //������ �� ��� ������ ������������ gcc �� �������� ����� ������ � ���������� ������ "^[0-9]+$" ������ � regex_match, ������� ������� �� search (���� ��� ���������� VisualStudio ��� ��������???)
        //��, �������������, ������� ����� �� ������������ �������������� �� �����
        regex eventReg("^[0-9a-z_\\-]+ [0-9]+");
        if (regex_search(body.begin(), body.end(), eventReg)) {
            //�������� ��� �������
            name = string(body, 0, body.find(' '));
            
            body.erase(0, body.find(' ') + 1);

            //�������� ����� �����
            tableId = stoi(body);                      
        }
        else return 0;

        //�������� �� ������������ ������ �����
        if (tablesCount < tableId) return 0;
    }

    return 1;
};

//�����, ����������� ������ �������
bool Event::SetInfo(string info, Time &lastTime, int tablesCount) {
    //��������� ������ � ����������� � ��������
    
    //������ �� ��� ������ ������������ gcc �� �������� ����� ������ � ���������� ������ "^[0-9]+$" ������ � regex_match, ������� ������� �� search (���� ��� ���������� VisualStudio ��� ��������???)
    //��, �������������, ������� ����� �� ������������ �������������� �� �����
    regex eventReg("[0-9][0-9]\\:[0-9][0-9] [1-4] [0-9a-z_ -]+");
    if (!regex_search(info.begin(), info.end(), eventReg)) return 0;
    
    //�������� �� ������ �����, ��, ���� ������
    string time = string(info, 0, info.find(' '));

    this->time.Hours = stoi(string(time, 0, 2));        //��������� ����
    this->time.Min = stoi(string(time, 3, 2));          //��������� ������

    //�������� �� ������������������ �������
    if (this->time >= lastTime)  lastTime = this->time;
    else return 0;

    //������� �� ������ ��������� �����
    info.erase(0, info.find(' ') + 1);

    string id = string(info, 0, info.find(' '));
    
    //�������� id �������
    this->id = stoi(id);

    info.erase(0, info.find(' ') + 1);

    //���������� ���� ������� � ������������ � id
    if (!ValidateEvent(info, tablesCount)) return 0;

    return 1;
};

//�����, �������� ���������� � ������� � �������
void Event::GetInfo() {
    time.Print();
    cout << " " << id << " " << name;

    if (id == 2) cout << " " << tableId << endl;
    else cout << endl;
};

//���������� ��������� ������ Table

//�����, ����������, ����� ������ ��������
void Table::ClientSit(string name, Time time) {
    isBusy = 1;
    this->name = name;
    lastTime = time;
};

//�����, ����������, ����� ������ ������
void Table::ClientLeft(Time time, int price) {
    isBusy = 0;
    name = "";

    //������� �����
    int hours = time.GetHoursDif(lastTime);
    money += hours * price;

    //������� �������
    Time timeDif = time - lastTime;
    this->time = this->time + timeDif;
};

//���������� ��������� ������ Tables

//����� ���������� �� ������
void Tables::GetInfo() {
    for (int i = 0; i < tables.size(); i++) {
        cout << i + 1 << " " << tables[i].money << " ";
        tables[i].time.Print();
        cout << endl;
    }
};

//������ ����� ����� �� ����
void Tables::Sit(int tableId, string clientName, Time time, int price) {
    //���������, ����� �� ������ �� ����� �� ������
    for (int i = 0; i < tables.size(); i++) if (tables[i].name == clientName)
        tables[i].ClientLeft(time, price);      //������ �������� ������ ����    

    //������ �������� �� ����� ����
    tables[tableId].ClientSit(clientName, time);
};

//������ ������ �� �����
bool Tables::Leave(string name, int& tableId, Time time, int price) {
    for (int i = 0; i < tables.size(); i++) {
        if (tables[i].name == name) {
            tableId = i;
            tables[i].ClientLeft(time, price);
            return 1;
        }
    }

    return 0;
};

//���������� ������ GamingClub

//����������� ������
GamingClub::GamingClub(string fileName) :fileName(fileName) {};

//���������� ������
GamingClub::~GamingClub() {

};

//����� ��� ��������� �� ����� ���������� ������
bool GamingClub::GetTablesCount(ifstream &file, string &line) {
    if (getline(file, line)) {
        //������ �� ��� ������ ������������ gcc �� �������� ����� ������ � ���������� ������ "^[0-9]+$" ������ � regex_match, ������� ������� �� search (���� ��� ���������� VisualStudio ��� ��������???)
        //��, �������������, ������� ����� �� ������������ �������������� �� �����
        regex countReg("[0-9]+");
        if (regex_search(line, countReg)) tablesCount = stoi(line);
        else return 0;       
    }
    else return 0;

    return 1;
};

//����� ��� ��������� ������� ��������/�������� �����
bool GamingClub::GetTime(ifstream& file, string &line) {
    if (getline(file, line)) {
        //������ �� ��� ������ ������������ gcc �� �������� ����� ������ � ���������� ������ "^[0-9]+$" ������ � regex_match, ������� ������� �� search (���� ��� ���������� VisualStudio ��� ��������???)
        //��, �������������, ������� ����� �� ������������ �������������� �� �����
        regex timeReg("[0-9][0-9]\\:[0-9][0-9] [0-9][0-9]\\:[0-9][0-9]");
        if (regex_search(line.begin(), line.end(), timeReg)) {
            timeOpening.Hours = stoi(string(line, 0, 2));         //������, ���������� ���� ��������
            timeOpening.Min = stoi(string(line, 3, 2));           //������, ���������� ������ ��������    
            timeClosing.Hours = stoi(string(line, 6, 2));         //������, ���������� ���� ��������  
            timeClosing.Min = stoi(string(line, 9, 2));           //������, ���������� ������ ��������                       
        }
        else return 0;
    }
    else return 0;

    //�������� ������������ ����������� �������
    if (timeOpening >= timeClosing) return 0;

    return 1;
};

//����� ��� ��������� ��������� ����
bool GamingClub::GetPrice(ifstream& file, string &line) {
    if (getline(file, line)) {
        //������ �� ��� ������ ������������ gcc �� �������� ����� ������ � ���������� ������ "^[0-9]+$" ������ � regex_match, ������� ������� �� search (���� ��� ���������� VisualStudio ��� ��������???)
        //��, �������������, ������� ����� �� ������������ �������������� �� �����
        regex priceReg("[0-9]+");       
        if (regex_search(line.begin(), line.end(), priceReg)) price = stoi(line);
        else return 0;
    }
    else return 0;

    return 1;
};

//�����, �������������� ��������� �����
void GamingClub::ProcessFile() {
    string line;        //������, � ������� ����� ������

    ifstream file(fileName); // �������� ���� ��� ������

    

    //������ ������ �� �����
    if (file.is_open())
    {
       
        //�������� ���������� ������
        if (!GetTablesCount(file,line)) {
            cout << line << endl;
            file.close();
            return;
        }
       
        //�������� ����� ��������/��������
        if (!GetTime(file,line)){
            cout << line << endl;
            file.close();
            return;            
        }
        
        //�������� ��������� ����
        if (!GetPrice(file,line)){
            cout << line << endl;
            file.close();
            return;
        }
        
        Time timeEvent; //����������, �������� ����� ���������� �������

        //�������� ��� �������
        while (getline(file, line))
        {
            Event e;    //������� ������ �����
            if (e.SetInfo(line, timeEvent, tablesCount)) eventsQueue.push(e);    //���� ������ ����������, �� ��������� � �������
            else {
                cout << line << endl;
                file.close();
                return;
            }
        }

    }
    file.close();     // ��������� ����

    cout << "check1" << endl;

    tables = new Tables(tablesCount);   //������� �����
    
    //������ ������ ������
    timeOpening.Print();
    cout << endl;

    //�������� ��������� �������
    ProcessEvents();

    //������ ����� ���������
    timeClosing.Print();
    cout << endl;

    //������� ���������� �� ������
    tables->GetInfo();
};

//��������� ������� "������ ������"
void GamingClub::ProcessID1(Event &e) {
    //���� ����� ��� ���� � �����
    for (int i = 0; i < customers.size(); i++) if (customers[i] == e.name){ 
        ProcessID13(e.time, "YouShallNotPass");     //������
        return;
    }

    //���� ������ � ��������� ����
    if (e.time > timeClosing || (timeOpening > e.time)) {
        ProcessID13(e.time, "NotOpenYet");     //������
        return;
    }

    //��������� ������� � ������ ��������
    customers.push_back(e.name);
};

//��������� ������� "������ ��� �� ����"
void GamingClub::ProcessID2(Event &e) {
    //��� � ���� ����� ���� � ���� ���� �������� ������, (���� ��� ������ VisualStudio ��� ��������)
    //������ �� �������� ��������� ���� ����� ������ false ���� � ��� ������, ����� ��� ������ ���������
    //����� ���� ��� ���� ��������� �� ������, � �� �� ��������?? �� ����. �� ����������� � ���� ��� ��� �������.
    //���� �� ��� ������� � ������, � ��� ������, �������� ��������� ���, ��� ������ ���������) 
    //	���.: +79087606387
    //	Mail : mrborisov00@gmail.com
    //	Telegram : https://t.me/Alex_Geine
    //  Discord : .geine


    //���� ������� ��� � �����
    bool isInTheClub = 0;

    for (int i = 0; i < customers.size(); i++) if (customers[i] == e.name) isInTheClub = 1;

    if (!isInTheClub) {
        ProcessID13(e.time, "ClientUnknown");     //������
        return;
    }

    //���� ���� �����
    if (tables->tables[e.tableId - 1].isBusy) {
        ProcessID13(e.time, "PlaceIsBusy");     //������
        return;
    }

    //���� ��� ���������, �� ������ ������� �� ����
    tables->Sit(e.tableId - 1, e.name, e.time, price);        
};

//��������� ������� "������ �������"
void GamingClub::ProcessID3(Event &e) {
    //���� ���� ��������� �����
    bool freeTables = 0;

    for (int i = 0; i < tables->tables.size(); i++) if (!tables->tables[i].isBusy) freeTables = 1;

    if (freeTables) {
        ProcessID13(e.time, "ICanWaitNoLonger!");     //������
        return;
    }

    //���� ������� �����������, ������ ������
    if (customersQueue.size() == tables->tables.size()) {
        ProcessID11(e.time, e.name);

        //���� ������ ��������� � �����, �� �� ��� �������
        for (int i = 0; i < customers.size(); i++)
            if (customers[i] == e.name) customers.erase(customers.begin() + i);
                    
        return;
    }

    //���� ��� ������, �� �� ������ � �������
    customersQueue.push(e.name);
};

//��������� ������� "������ ����"
void GamingClub::ProcessID4(Event &e) {
    //��� � ���� ����� ���� � ���� ���� �������� ������, (���� ��� ������ VisualStudio ��� ��������)
    //������ �� �������� ��������� ���� ����� ������ false ���� � ��� ������, ����� ��� ������ ���������
    //����� ���� ��� ���� ��������� �� ������, � �� �� ��������?? �� ����. �� ����������� � ���� ��� ��� �������.
    //���� �� ��� ������� � ������, � ��� ������, �������� ��������� ���, ��� ������ ���������) 
    //	���.: +79087606387
    //	Mail : mrborisov00@gmail.com
    //	Telegram : https://t.me/Alex_Geine
    //  Discord : .geine
    
    //���� ������� ��� � �����
    bool isInTheClub = 0;

    for (int i = 0; i < customers.size(); i++) if (customers[i] == e.name) isInTheClub = 1;

    if (!isInTheClub) {
        ProcessID13(e.time, "ClientUnknown");     //������
        return;
    }

    //������� ������� �� �����
    for (int i = 0; i < customers.size(); i++)
        if (customers[i] == e.name) customers.erase(customers.begin() + i);

    //����������� ���� � ������������� ��� �������� �� �������
    int tableId = 0;    //���������� ��� id ����� ��������� ��������

    if (tables->Leave(e.name, tableId, e.time, price)) {   //���� ��������� ������ ��� �� ������
        if (!customersQueue.empty()) {  //���� ������� �� ������
            tables->Sit(tableId, customersQueue.front(), e.time, price);   //������ ��������
            ProcessID12(e.time, customersQueue.front(), tableId);    //���������� �������
            customersQueue.pop();
        }
    }
};

//��������� ������� "������ ����"
void GamingClub::ProcessID11(Time time, string name) {
    time.Print();
    cout << " 11 " << name << endl;
};

//��������� ������� "������ ��� �� ����"
void GamingClub::ProcessID12(Time time, string name, int tableId) {
    time.Print();
    cout << " 12 " << name << " " << tableId + 1 << endl;
};

//��������� ������� "������"
void GamingClub::ProcessID13(Time time, string err) {
    time.Print();
    cout << " 13 " << err << endl;
};

//���� ��� �������
void GamingClub::ClientsLeft() {
    //���������� ��������
    sort(customers.begin(), customers.end());
    
    //������� ���� �������� �� ������
    int tableId = 0;
    for (int i = 0; i < customers.size(); i++) tables->Leave(customers[i], tableId, timeClosing, price);

    //�������� ��� ���� �������� ������� ������ ����
    for (int i = 0; i < customers.size(); i++) ProcessID11(timeClosing, customers[i]);
};

//��������� �������
void GamingClub::ProcessEvents() {
    while(!eventsQueue.empty()) {
        Event e = eventsQueue.front();
        e.GetInfo();      //������� ���������� � �������

        //��������� �������
        if (e.id == 1) ProcessID1(e);
        else if(e.id == 2) ProcessID2(e);
        else if (e.id == 3) ProcessID3(e);
        else ProcessID4(e);

        eventsQueue.pop();      //������� ������� �� �������
    }

    //����� ��������� ��������� ���� �������� ����
    ClientsLeft();
};