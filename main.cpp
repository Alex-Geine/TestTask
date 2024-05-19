//входная точка в программу

#include "GamingClub.h"

int main(int argc, char *argv[])
{
    cout << argv[argc - 1] << endl;
    //передаем название файла в класс для обработки файла
    GamingClub gc(argv[argc - 1]);    

    //производим обработку файла
    gc.ProcessFile();
    
    //чтобы консоль сразу не закрывалась
   // system("PAUSE");
}

