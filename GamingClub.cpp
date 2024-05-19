//файл реализации GamingClub.h

#include "GamingClub.h"

//реализация класса Event

//валидация тела события в соответсвии с id
bool Event::ValidateEvent(string body, int tablesCount) {
    if (id == 1 || id == 3 || id == 4) {        //для этих событий в теле храниться только имя
        //проверяем строку в соответсвии с форматом

        //почему то при сборке компилятором gcc не работают якоря начала и завершения строки "^[0-9]+$" вместе с regex_match, поэтому заменил на search (хотя при компиляции VisualStudio все работает???)
        //но, соответвсенно, провека строк на корректность осуществляться не будет
        regex eventReg("[0-9a-z_\\-]+");
        if (regex_search(body.begin(), body.end(), eventReg)) name = body;
        else return 0;
    }
    else {      //для этого события храниться имя и номер стола
        //проверяем строку в соответсвии с форматом

        //почему то при сборке компилятором gcc не работают якоря начала и завершения строки "^[0-9]+$" вместе с regex_match, поэтому заменил на search (хотя при компиляции VisualStudio все работает???)
        //но, соответвсенно, провека строк на корректность осуществляться не будет
        regex eventReg("^[0-9a-z_\\-]+ [0-9]+");
        if (regex_search(body.begin(), body.end(), eventReg)) {
            //получаем имя клиента
            name = string(body, 0, body.find(' '));
            
            body.erase(0, body.find(' ') + 1);

            //получаем номер стола
            tableId = stoi(body);                      
        }
        else return 0;

        //проверка на корректность номера стола
        if (tablesCount < tableId) return 0;
    }

    return 1;
};

//метод, заполняющий данные события
bool Event::SetInfo(string info, Time &lastTime, int tablesCount) {
    //проверяем строку в соответсвии с форматом
    
    //почему то при сборке компилятором gcc не работают якоря начала и завершения строки "^[0-9]+$" вместе с regex_match, поэтому заменил на search (хотя при компиляции VisualStudio все работает???)
    //но, соответвсенно, провека строк на корректность осуществляться не будет
    regex eventReg("[0-9][0-9]\\:[0-9][0-9] [1-4] [0-9a-z_ -]+");
    if (!regex_search(info.begin(), info.end(), eventReg)) return 0;
    
    //получаем из строки время, ид, тело ивента
    string time = string(info, 0, info.find(' '));

    this->time.Hours = stoi(string(time, 0, 2));        //заполняем часы
    this->time.Min = stoi(string(time, 3, 2));          //заполняем минуты

    //проверка на последовательность событий
    if (this->time >= lastTime)  lastTime = this->time;
    else return 0;

    //удаляем из строки найденное время
    info.erase(0, info.find(' ') + 1);

    string id = string(info, 0, info.find(' '));
    
    //получаем id события
    this->id = stoi(id);

    info.erase(0, info.find(' ') + 1);

    //валидируем тело события в соответствии с id
    if (!ValidateEvent(info, tablesCount)) return 0;

    return 1;
};

//метод, выдающий информацию о событии в консоль
void Event::GetInfo() {
    time.Print();
    cout << " " << id << " " << name;

    if (id == 2) cout << " " << tableId << endl;
    else cout << endl;
};

//реализация структуры данных Table

//метод, вызываемый, когда клиент приходит
void Table::ClientSit(string name, Time time) {
    isBusy = 1;
    this->name = name;
    lastTime = time;
};

//метод, вызываемый, когда клиент уходит
void Table::ClientLeft(Time time, int price) {
    isBusy = 0;
    name = "";

    //подсчет денег
    int hours = time.GetHoursDif(lastTime);
    money += hours * price;

    //подсчет времени
    Time timeDif = time - lastTime;
    this->time = this->time + timeDif;
};

//реализация структуры данных Tables

//вывод информации по столам
void Tables::GetInfo() {
    for (int i = 0; i < tables.size(); i++) {
        cout << i + 1 << " " << tables[i].money << " ";
        tables[i].time.Print();
        cout << endl;
    }
};

//клиент хочет сесть за стол
void Tables::Sit(int tableId, string clientName, Time time, int price) {
    //проверяем, сидит ли клиент за каким то столом
    for (int i = 0; i < tables.size(); i++) if (tables[i].name == clientName)
        tables[i].ClientLeft(time, price);      //клиент покидает старый стол    

    //клиент садиться за новый стол
    tables[tableId].ClientSit(clientName, time);
};

//клиент уходит со стола
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

//реализация класса GamingClub

//конструктор класса
GamingClub::GamingClub(string fileName) :fileName(fileName) {};

//деструктор класса
GamingClub::~GamingClub() {

};

//метод для получения из файла количества столов
bool GamingClub::GetTablesCount(ifstream &file, string &line) {
    if (getline(file, line)) {
        //почему то при сборке компилятором gcc не работают якоря начала и завершения строки "^[0-9]+$" вместе с regex_match, поэтому заменил на search (хотя при компиляции VisualStudio все работает???)
        //но, соответвсенно, провека строк на корректность осуществляться не будет
        regex countReg("[0-9]+");
        if (regex_search(line, countReg)) tablesCount = stoi(line);
        else return 0;       
    }
    else return 0;

    return 1;
};

//метод для получения времени открытия/закрытия клуба
bool GamingClub::GetTime(ifstream& file, string &line) {
    if (getline(file, line)) {
        //почему то при сборке компилятором gcc не работают якоря начала и завершения строки "^[0-9]+$" вместе с regex_match, поэтому заменил на search (хотя при компиляции VisualStudio все работает???)
        //но, соответвсенно, провека строк на корректность осуществляться не будет
        regex timeReg("[0-9][0-9]\\:[0-9][0-9] [0-9][0-9]\\:[0-9][0-9]");
        if (regex_search(line.begin(), line.end(), timeReg)) {
            timeOpening.Hours = stoi(string(line, 0, 2));         //строка, содержащая часы открытия
            timeOpening.Min = stoi(string(line, 3, 2));           //строка, содержащая минуты открытия    
            timeClosing.Hours = stoi(string(line, 6, 2));         //строка, содержащая часы закрытия  
            timeClosing.Min = stoi(string(line, 9, 2));           //строка, содержащая минуты закрытия                       
        }
        else return 0;
    }
    else return 0;

    //проверка корректности полученного времени
    if (timeOpening >= timeClosing) return 0;

    return 1;
};

//метод для получения стоимости часа
bool GamingClub::GetPrice(ifstream& file, string &line) {
    if (getline(file, line)) {
        //почему то при сборке компилятором gcc не работают якоря начала и завершения строки "^[0-9]+$" вместе с regex_match, поэтому заменил на search (хотя при компиляции VisualStudio все работает???)
        //но, соответвсенно, провека строк на корректность осуществляться не будет
        regex priceReg("[0-9]+");       
        if (regex_search(line.begin(), line.end(), priceReg)) price = stoi(line);
        else return 0;
    }
    else return 0;

    return 1;
};

//метод, осуществляющий обработку файла
void GamingClub::ProcessFile() {
    string line;        //строка, в которую будем читать

    ifstream file(fileName); // окрываем файл для чтения

    

    //читаем данные из файла
    if (file.is_open())
    {
       
        //получаем количество столов
        if (!GetTablesCount(file,line)) {
            cout << line << endl;
            file.close();
            return;
        }
       
        //получаем время открытия/закрытия
        if (!GetTime(file,line)){
            cout << line << endl;
            file.close();
            return;            
        }
        
        //получаем стоимость часа
        if (!GetPrice(file,line)){
            cout << line << endl;
            file.close();
            return;
        }
        
        Time timeEvent; //переменная, хранящая время последнего события

        //получаем лог событий
        while (getline(file, line))
        {
            Event e;    //создаем пустой ивент
            if (e.SetInfo(line, timeEvent, tablesCount)) eventsQueue.push(e);    //если строка корректная, то добавляем в очередь
            else {
                cout << line << endl;
                file.close();
                return;
            }
        }

    }
    file.close();     // закрываем файл

    cout << "check1" << endl;

    tables = new Tables(tablesCount);   //создаем столы
    
    //вывдим начало работы
    timeOpening.Print();
    cout << endl;

    //начинаем обработку событий
    ProcessEvents();

    //выводи время окончания
    timeClosing.Print();
    cout << endl;

    //выводим информацию по столам
    tables->GetInfo();
};

//обработка события "Клиент пришел"
void GamingClub::ProcessID1(Event &e) {
    //если клинт уже есть в клубе
    for (int i = 0; i < customers.size(); i++) if (customers[i] == e.name){ 
        ProcessID13(e.time, "YouShallNotPass");     //ошибка
        return;
    }

    //если пришел в нерабочие часы
    if (e.time > timeClosing || (timeOpening > e.time)) {
        ProcessID13(e.time, "NotOpenYet");     //ошибка
        return;
    }

    //добавляем клиента в массив клиентов
    customers.push_back(e.name);
};

//обработка события "Клиент сел за стол"
void GamingClub::ProcessID2(Event &e) {
    //вот в этом куске кода у меня тоже возникла ошибка, (хотя при сборке VisualStudio все работало)
    //почему то операция сравнения двух строк выдает false даже в том случае, когда две строки идентичны
    //может быть тут идет сравнение по адресу, а не по значению?? не знаю. На исправление у меня уже нет времени.
    //если вы это читаете и знаете, в чем ошибка, напишите пожалуйта мне, мне правда интересно) 
    //	Тел.: +79087606387
    //	Mail : mrborisov00@gmail.com
    //	Telegram : https://t.me/Alex_Geine
    //  Discord : .geine


    //если клиента нет в клубе
    bool isInTheClub = 0;

    for (int i = 0; i < customers.size(); i++) if (customers[i] == e.name) isInTheClub = 1;

    if (!isInTheClub) {
        ProcessID13(e.time, "ClientUnknown");     //ошибка
        return;
    }

    //если стол занят
    if (tables->tables[e.tableId - 1].isBusy) {
        ProcessID13(e.time, "PlaceIsBusy");     //ошибка
        return;
    }

    //если все нормально, то клиент садится за стол
    tables->Sit(e.tableId - 1, e.name, e.time, price);        
};

//обработка события "Клиент ожидает"
void GamingClub::ProcessID3(Event &e) {
    //если есть свободные столы
    bool freeTables = 0;

    for (int i = 0; i < tables->tables.size(); i++) if (!tables->tables[i].isBusy) freeTables = 1;

    if (freeTables) {
        ProcessID13(e.time, "ICanWaitNoLonger!");     //ошибка
        return;
    }

    //если очередь переполнена, клиент уходит
    if (customersQueue.size() == tables->tables.size()) {
        ProcessID11(e.time, e.name);

        //если клиент находился в клубе, то мы его убираем
        for (int i = 0; i < customers.size(); i++)
            if (customers[i] == e.name) customers.erase(customers.begin() + i);
                    
        return;
    }

    //если все хорошо, то он встает в очередь
    customersQueue.push(e.name);
};

//обработка события "Клиент ушел"
void GamingClub::ProcessID4(Event &e) {
    //вот в этом куске кода у меня тоже возникла ошибка, (хотя при сборке VisualStudio все работало)
    //почему то операция сравнения двух строк выдает false даже в том случае, когда две строки идентичны
    //может быть тут идет сравнение по адресу, а не по значению?? не знаю. На исправление у меня уже нет времени.
    //если вы это читаете и знаете, в чем ошибка, напишите пожалуйта мне, мне правда интересно) 
    //	Тел.: +79087606387
    //	Mail : mrborisov00@gmail.com
    //	Telegram : https://t.me/Alex_Geine
    //  Discord : .geine
    
    //если клиента нет в клубе
    bool isInTheClub = 0;

    for (int i = 0; i < customers.size(); i++) if (customers[i] == e.name) isInTheClub = 1;

    if (!isInTheClub) {
        ProcessID13(e.time, "ClientUnknown");     //ошибка
        return;
    }

    //убираем клиента из клуба
    for (int i = 0; i < customers.size(); i++)
        if (customers[i] == e.name) customers.erase(customers.begin() + i);

    //освобождаем стол и предоставляем его человеку из очереди
    int tableId = 0;    //переменная для id стола уходящего человека

    if (tables->Leave(e.name, tableId, e.time, price)) {   //если уходивший клиент был за столом
        if (!customersQueue.empty()) {  //если очередь не пустая
            tables->Sit(tableId, customersQueue.front(), e.time, price);   //сажаем человека
            ProcessID12(e.time, customersQueue.front(), tableId);    //генерируем событие
            customersQueue.pop();
        }
    }
};

//исходящее событие "Клиент ушел"
void GamingClub::ProcessID11(Time time, string name) {
    time.Print();
    cout << " 11 " << name << endl;
};

//исходящее событие "Клиент сел за стол"
void GamingClub::ProcessID12(Time time, string name, int tableId) {
    time.Print();
    cout << " 12 " << name << " " << tableId + 1 << endl;
};

//исходящее событие "Ошибка"
void GamingClub::ProcessID13(Time time, string err) {
    time.Print();
    cout << " 13 " << err << endl;
};

//ушли все клиенты
void GamingClub::ClientsLeft() {
    //сортировка клиентов
    sort(customers.begin(), customers.end());
    
    //убираем всех клиентов со столов
    int tableId = 0;
    for (int i = 0; i < customers.size(); i++) tables->Leave(customers[i], tableId, timeClosing, price);

    //вызываем для всех клиентов событие клиент ушел
    for (int i = 0; i < customers.size(); i++) ProcessID11(timeClosing, customers[i]);
};

//обработка событий
void GamingClub::ProcessEvents() {
    while(!eventsQueue.empty()) {
        Event e = eventsQueue.front();
        e.GetInfo();      //выводим информацию в консоль

        //обработка событий
        if (e.id == 1) ProcessID1(e);
        else if(e.id == 2) ProcessID2(e);
        else if (e.id == 3) ProcessID3(e);
        else ProcessID4(e);

        eventsQueue.pop();      //удаляем элемент из очереди
    }

    //после обработки вынуждаем всех клиентов уйти
    ClientsLeft();
};