////////////////////////////////////////////////////////////////////////////////
//                                  ВНИМАНИЕ                                  //
////////////////////////////////////////////////////////////////////////////////
//      Для компиляции под ОС Windows с использованием компилятора MSVC       //
//                     раскомментируйте следующую строку:                     //
#define CLEARSCREEN "cls"                                                     //
////////////////////////////////////////////////////////////////////////////////
//   Для компиляции под POSIX-совместимой ОС или ОС Windows с использованием  //
//           компилятора GCC/G++ раскомментируйте следующую строку:           //
//#define CLEARSCREEN "clear"                                                 //
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <limits>
#include <climits>
#include <chrono>

struct list
  {
    int value;
    list *prev;
    list *next;
  };

struct listAndArray
  {
    int listSize;
    list *List;
    int *Array;
  };

listAndArray* createListAndArray ();
int getNumericAnswer (int minRange, int maxRange, const char *Question);
listAndArray* mainMenu (listAndArray *ListAndArray);
listAndArray* deleteListAndArray (listAndArray *ListAndArray);
void printList (listAndArray *ListAndArray);
void printArray (listAndArray *ListAndArray);
void insertElementToList (listAndArray *ListAndArray, int position, int value);
void insertElementToArray (listAndArray *ListAndArray, int position, int value);
void deleteElementFromListByPosition (listAndArray *ListAndArray, int position);
void deleteElementFromArrayByPosition (listAndArray *ListAndArray, int position);
void deleteElementFromListByValue (listAndArray *ListAndArray, int value);
void deleteElementFromArrayByValue (listAndArray *ListAndArray, int value);
listAndArray* readListFromTerm ();
listAndArray* readListFromFile ();
listAndArray* generateRandomList ();
listAndArray* insertElement (listAndArray *ListAndArray);
listAndArray* deleteElement (listAndArray *ListAndArray);
void printElement (listAndArray *ListAndArray);

int main ()
  {
    setlocale (0, "");
    srand (time (0));
    listAndArray *ListAndArray=NULL;
    ListAndArray=mainMenu (ListAndArray);
    deleteListAndArray (ListAndArray);
    return 0;
  }

listAndArray* createListAndArray ()
  {
    listAndArray *ListAndArray=(listAndArray*)malloc (sizeof (listAndArray));
    ListAndArray->List=NULL;
    ListAndArray->Array=NULL;
    ListAndArray->listSize=0;
    return ListAndArray;
  }

int getNumericAnswer (int minRange, int maxRange, const char *Question)
  {
    int answer;
    std::cout << Question;
    std::cin >> answer;
    if ((std::cin.fail ())||(answer<minRange)||(answer>maxRange))
      {
        std::cin.clear ();
        std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');
        std::cout << "Некорректный ввод!" << std::endl;
        answer=getNumericAnswer (minRange, maxRange, Question);
        return answer;
      }
    else
      {
        std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');
        return answer;
      }
  }

listAndArray* mainMenu (listAndArray *ListAndArray)
  {
    system (CLEARSCREEN);
    std::cout << "Главное меню:\n\nТекущее состояние массива:" << std::endl;
    printArray (ListAndArray);
    std::cout << std::endl << "\nТекущее состояние списка:" << std::endl;
    printList(ListAndArray);
    std::cout << std::endl << "\n1) Сформировать список и массив случайным обра"
                 "зом.\n2) Считать список и массив из терминала.\n3) Считать сп"
                 "исок и массив из файла.\n4) Вставить элемент в список и масси"
                 "в на произвольную позицию.";
    bool isListEmpty;
    if (ListAndArray!=NULL)
      {
        std::cout << "\n5) Удалить произвольный элемент из списка и массива.\n6"
                     ") Вывести значение произвольного элемента массива.\n0) Вы"
                     "йти из программы.\n" << std::endl;
        isListEmpty=0;
      }
    else
      {
        std::cout << "\n0) Выйти из программы.\n" << std::endl;
        isListEmpty=1;
      }
    switch (getNumericAnswer (0, isListEmpty?4:6, "Введите номер желаемого варианта: "))
      {
        case 1:
          ListAndArray=deleteListAndArray (ListAndArray);
          ListAndArray=generateRandomList ();
          ListAndArray=mainMenu (ListAndArray);
          break;
        case 2:
          ListAndArray=deleteListAndArray (ListAndArray);
          ListAndArray=readListFromTerm ();
          ListAndArray=mainMenu (ListAndArray);
          break;
        case 3:
          ListAndArray=deleteListAndArray (ListAndArray);
          ListAndArray=readListFromFile ();
          ListAndArray=mainMenu (ListAndArray);
          break;
        case 4:
          ListAndArray=insertElement (ListAndArray);
          ListAndArray=mainMenu (ListAndArray);
          break;
        case 5:
          ListAndArray=deleteElement (ListAndArray);
          ListAndArray=mainMenu (ListAndArray);
          break;
        case 6:
          printElement (ListAndArray);
          ListAndArray=mainMenu (ListAndArray);
          break;
        default:
          break;
      }
    return ListAndArray;
  }

listAndArray* deleteListAndArray (listAndArray *ListAndArray)
  {
    if (ListAndArray!=NULL)
      {
        while (ListAndArray->List!=NULL)
          {
            list *Buffer;
            Buffer=ListAndArray->List->next;
            free (ListAndArray->List);
            ListAndArray->List=Buffer;
          }
        if (ListAndArray->Array!=NULL)
          free (ListAndArray->Array);
        free (ListAndArray);
      }
    return NULL;
  }

void printList (listAndArray *ListAndArray)
  {
    if (ListAndArray==NULL)
      std::cout << "Список пуст.";
    else
      {
        list* List=ListAndArray->List;
        do
          {
            std::cout << List->value << " ";
            List=List->next;
          }
        while (List!=NULL);
      }
  }

void printArray (listAndArray *ListAndArray)
  {
    if (ListAndArray==NULL)
      std::cout << "Массив пуст.";
    else
      {
        for (int i=0; i<ListAndArray->listSize; i++)
          std::cout << *(ListAndArray->Array+i) << " ";
      }
  }

void insertElementToList (listAndArray *ListAndArray, int position, int value)
  {
    list *List=ListAndArray->List;
    list *Element=(list*)malloc (sizeof (list));
    Element->value=value;
    if (!(position-1))
      {
        Element->next=List;
        Element->prev=NULL;
        if (List!=NULL)
          List->prev=Element;
        ListAndArray->List=Element;
      }
    else
      {
        for (int i=1; i<position-1; i++)
            List=List->next;
        Element->next=List->next;
        List->next=Element;
        Element->prev=List;
        if (Element->next!=NULL)
          Element->next->prev=Element;
      }
  }

void insertElementToArray (listAndArray *ListAndArray, int position, int value)
  {
    ListAndArray->Array=(int*)realloc (ListAndArray->Array, (ListAndArray->listSize+1)*sizeof(int));
    for (int i=ListAndArray->listSize; i>position-1; i--)
      *(ListAndArray->Array+i)=*(ListAndArray->Array+i-1);
    *(ListAndArray->Array+position-1)=value;
  }

void deleteElementFromListByPosition (listAndArray *ListAndArray, int position)
  {
    list *List=ListAndArray->List;
    for (int i=1; i<position; i++)
      List=List->next;
    if (List->prev!=NULL)
      List->prev->next=List->next;
    else
      ListAndArray->List=List->next;
    if (List->next!=NULL)
      List->next->prev=List->prev;
    free(List);
  }

void deleteElementFromArrayByPosition (listAndArray *ListAndArray, int position)
  {
    for (int i=position-1; i<ListAndArray->listSize; i++)
      *(ListAndArray->Array+i)=*(ListAndArray->Array+i+1);
    ListAndArray->Array=(int*)realloc (ListAndArray->Array, (ListAndArray->listSize-1)*sizeof(int));
  }

void deleteElementFromListByValue (listAndArray *ListAndArray, int value)
  {
    list *List=ListAndArray->List;
    list *Buffer;
    while (List!=NULL)
      {
        Buffer=List->next;
        if (List->value==value)
          {
            if (List->prev!=NULL)
              List->prev->next=List->next;
            else
              ListAndArray->List=List->next;
            if (List->next!=NULL)
              List->next->prev=List->prev;
            free(List);
          }
        List=Buffer;
      }
  }

void deleteElementFromArrayByValue (listAndArray *ListAndArray, int value)
  {
    for (int i=0; i<ListAndArray->listSize;)
      if (*(ListAndArray->Array+i)==value)
        {
          for (int j=i; j<ListAndArray->listSize-1; j++)
            *(ListAndArray->Array+j)=*(ListAndArray->Array+j+1);
          ListAndArray->Array=(int*)realloc (ListAndArray->Array, (ListAndArray->listSize-1)*sizeof(int));
          ListAndArray->listSize--;
        }
      else
        i++;
  }

listAndArray* readListFromTerm ()
  {
    std::cout << "Введите элементы списка/массива в одну строку через пробел: ";
    listAndArray *ListAndArray=createListAndArray ();
    char Buffer;
    Buffer=std::cin.get();
    std::chrono::system_clock::time_point listTimeBegin, listTimeEnd, arrayTimeBegin, arrayTimeEnd;
    while (Buffer!='\n')
      {
        while (!(Buffer-32))
          Buffer=std::cin.get();
        int value=0;
        bool isNegative=0;
        if (Buffer==45)
          {
            isNegative=1;
            Buffer=std::cin.get();
          }
        while (Buffer<58&&Buffer>47)
          {
            value=value*10+(Buffer-48);
            Buffer=std::cin.get();
          }
        if (isNegative)
          value=value*(-1);
        if (std::cin.fail()||(Buffer!=32&&Buffer!='\n'))
          {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Некорректный ввод!" << std::endl;
            ListAndArray=deleteListAndArray(ListAndArray);
            ListAndArray=readListFromTerm();
            return ListAndArray;
          }
        listTimeBegin=std::chrono::system_clock::now();
        insertElementToList(ListAndArray, ListAndArray->listSize+1, value);
        listTimeEnd=std::chrono::system_clock::now();
        arrayTimeBegin=std::chrono::system_clock::now();
        insertElementToArray(ListAndArray, ListAndArray->listSize+1, value);
        arrayTimeEnd=std::chrono::system_clock::now();
        ListAndArray->listSize++;
        while (!(Buffer-32))
          Buffer=std::cin.get();
      }
    if (!ListAndArray->listSize)
        ListAndArray=deleteListAndArray(ListAndArray);
    std::cout << "\nНа осуществеление операции над списком было затрачено " <<
                 std::chrono::duration_cast<std::chrono::nanoseconds>(listTimeEnd-listTimeBegin).count()
                 << " наносекунд.\nНа осуществеление операции над массивом было"
                 " затрачено " << std::chrono::duration_cast<std::chrono::nanoseconds>(arrayTimeEnd-arrayTimeBegin).count()
                 << " наносекунд.";
    std::cout << "\nДля возврата в меню нажмите \"Enter\".";
    std::cin.get();
    return ListAndArray;
  }

listAndArray* readListFromFile ()
  {
    char fileName[261];
    std::cout << std::endl << "Введите имя (или путь) файла: ";
    std::cin.getline (fileName, 261);
    std::ifstream File;
    File.open (fileName);
    listAndArray *ListAndArray=createListAndArray ();
    std::chrono::system_clock::time_point listTimeBegin, listTimeEnd, arrayTimeBegin, arrayTimeEnd;
    if (File.is_open())
      {
        
        while (!File.eof())
          {
            char *buffer=(char*)malloc (sizeof(char)*20);
            char *bufferTwo=buffer;
            File.getline(buffer, 20);
            int value=0;
            bool isNegative=0;
            if (*buffer==45)
              {
                isNegative=1;
                buffer++;
              }
            while (*buffer!='\0')
              {
                value=value*10+(*buffer-48);
                buffer++;
              }
            if (isNegative) value=value*(-1);
            free(bufferTwo);
            listTimeBegin=std::chrono::system_clock::now();
            insertElementToList (ListAndArray, ListAndArray->listSize+1, value);
            listTimeEnd=std::chrono::system_clock::now();
            arrayTimeBegin=std::chrono::system_clock::now();
            insertElementToArray (ListAndArray, ListAndArray->listSize+1, value);
            arrayTimeEnd=std::chrono::system_clock::now();
            ListAndArray->listSize++;
          }
        File.close();
      }
    else
      {
        ListAndArray=deleteListAndArray (ListAndArray);
        std::cout << "Некорректный ввод!" << std::endl;
        ListAndArray=readListFromFile ();
      }
    if (!ListAndArray->listSize)
        ListAndArray=deleteListAndArray (ListAndArray);
    std::cout << "\nНа осуществеление операции над списком было затрачено " <<
                 std::chrono::duration_cast<std::chrono::nanoseconds>(listTimeEnd-listTimeBegin).count()
                 << " наносекунд.\nНа осуществеление операции над массивом было"
                 " затрачено " << std::chrono::duration_cast<std::chrono::nanoseconds>(arrayTimeEnd-arrayTimeBegin).count()
                 << " наносекунд.";
    std::cout << "\nДля возврата в меню нажмите \"Enter\".";
    std::cin.get();
    return ListAndArray;
  }

listAndArray* generateRandomList ()
  {
    listAndArray *ListAndArray=createListAndArray ();
    int Size=getNumericAnswer(0, INT_MAX, "Введите число элементов в списке/массиве: ");
    std::chrono::system_clock::time_point listTimeBegin, listTimeEnd, arrayTimeBegin, arrayTimeEnd;
    for (int i=0; i<Size; i++)
      {
        int value=rand()%100;
        listTimeBegin=std::chrono::system_clock::now();
        insertElementToList(ListAndArray, ListAndArray->listSize+1, value);
        listTimeEnd=std::chrono::system_clock::now();
        arrayTimeBegin=std::chrono::system_clock::now();
        insertElementToArray(ListAndArray, ListAndArray->listSize+1, value);
        arrayTimeEnd=std::chrono::system_clock::now();
        ListAndArray->listSize++;
      }
    if (!ListAndArray->listSize)
        ListAndArray=deleteListAndArray(ListAndArray);
    std::cout << "\nНа осуществеление операции над списком было затрачено " <<
                 std::chrono::duration_cast<std::chrono::nanoseconds>(listTimeEnd-listTimeBegin).count()
                 << " наносекунд.\nНа осуществеление операции над массивом было"
                 " затрачено " << std::chrono::duration_cast<std::chrono::nanoseconds>(arrayTimeEnd-arrayTimeBegin).count()
                 << " наносекунд.";
    std::cout << "\nДля возврата в меню нажмите \"Enter\".";
    std::cin.get();
    return ListAndArray;
  }

listAndArray* insertElement (listAndArray *ListAndArray)
  {
    if (ListAndArray==NULL)
        ListAndArray=createListAndArray ();
    std::chrono::system_clock::time_point listTimeBegin, listTimeEnd, arrayTimeBegin, arrayTimeEnd;
    int position=getNumericAnswer (1, ListAndArray->listSize+1, "Введите номер позиции добавляемого элемента в списке/массиве: ");
    int value=getNumericAnswer (INT_MIN, INT_MAX, "Введите значение добавляемого элемента: ");
    listTimeBegin=std::chrono::system_clock::now();
    insertElementToList (ListAndArray, position, value);
    listTimeEnd=std::chrono::system_clock::now();
    arrayTimeBegin=std::chrono::system_clock::now();
    insertElementToArray (ListAndArray, position, value);
    arrayTimeEnd=std::chrono::system_clock::now();
    ListAndArray->listSize++;
    std::cout << "\nНа осуществеление операции над списком было затрачено " <<
                 std::chrono::duration_cast<std::chrono::nanoseconds>(listTimeEnd-listTimeBegin).count()
                 << " наносекунд.\nНа осуществеление операции над массивом было"
                 " затрачено " << std::chrono::duration_cast<std::chrono::nanoseconds>(arrayTimeEnd-arrayTimeBegin).count()
                 << " наносекунд.";
    std::cout << "\nДля возврата в меню нажмите \"Enter\".";
    std::cin.get();
    return ListAndArray;
  }

listAndArray* deleteElement (listAndArray *ListAndArray)
  {
    std::cout << "Удалить элемент по значению или по номеру позиции в массиве/с"
                 "писке?\n1) По значению.\n2) По позиции." << std::endl;
    bool byPosition=getNumericAnswer (1, 2, "Введите номер жедаемого варианта: ")-1;
    std::chrono::system_clock::time_point listTimeBegin, listTimeEnd, arrayTimeBegin, arrayTimeEnd;
    if (byPosition)
      {
        int position=getNumericAnswer (1, ListAndArray->listSize+1, "Введите номер позиции удаляемого элемента в списке/массиве: ");
        listTimeBegin=std::chrono::system_clock::now();
        deleteElementFromListByPosition (ListAndArray, position);
        listTimeEnd=std::chrono::system_clock::now();
        arrayTimeBegin=std::chrono::system_clock::now();
        deleteElementFromArrayByPosition (ListAndArray, position);
        arrayTimeEnd=std::chrono::system_clock::now();
        ListAndArray->listSize--;
        if (ListAndArray->List==NULL)
            ListAndArray=deleteListAndArray(ListAndArray);
      }
    else
      {
        int value=getNumericAnswer (INT_MIN, INT_MAX, "Введите значение удаляемого элемента: ");
        listTimeBegin=std::chrono::system_clock::now();
        deleteElementFromListByValue (ListAndArray, value);
        listTimeEnd=std::chrono::system_clock::now();
        arrayTimeBegin=std::chrono::system_clock::now();
        deleteElementFromArrayByValue (ListAndArray, value);
        arrayTimeEnd=std::chrono::system_clock::now();
        if (ListAndArray->List==NULL)
            ListAndArray=deleteListAndArray(ListAndArray);
      }
    std::cout << "\nНа осуществеление операции над списком было затрачено " <<
                 std::chrono::duration_cast<std::chrono::nanoseconds>(listTimeEnd-listTimeBegin).count()
                 << " наносекунд.\nНа осуществеление операции над массивом было"
                 " затрачено " << std::chrono::duration_cast<std::chrono::nanoseconds>(arrayTimeEnd-arrayTimeBegin).count()
                 << " наносекунд.";
    std::cout << "\nДля возврата в меню нажмите \"Enter\".";
    std::cin.get();
    return ListAndArray;
  }

void printElement (listAndArray *ListAndArray)
  {
    std::cout << "Вывести элемент по значению или по номеру позиции в массиве/с"
                 "писке?\n1) По значению.\n2) По позиции." << std::endl;
    bool byPosition=getNumericAnswer (1, 2, "Введите номер жедаемого варианта: ")-1;
    std::chrono::system_clock::time_point listTimeBegin, listTimeEnd, arrayTimeBegin, arrayTimeEnd;
    if (byPosition)
      {
        int position=getNumericAnswer (1, ListAndArray->listSize+1, "Введите номер позиции выводимого элемента в списке/массиве: ");
        std::cout << "Элемент на позиции " << position << " в списке: ";
        listTimeBegin=std::chrono::system_clock::now();
        list *List=ListAndArray->List;
        for (int i=1; i<position; i++)
          List=List->next;
        listTimeEnd=std::chrono::system_clock::now();
        std::cout << List->value << std::endl;
        std::cout << "Элемент на позиции " << position << " в массиве: ";
        int value;
        arrayTimeBegin=std::chrono::system_clock::now();
        value=*(ListAndArray->Array+position-1);
        arrayTimeEnd=std::chrono::system_clock::now();
        std::cout << value;
      }
    else
      {
        int value=getNumericAnswer (INT_MIN, INT_MAX, "Введите значение выводимого элемента: ");
        std::cout << "Элемент, со значением " << value << " в списке находится "
                     "на позициях: ";
        listTimeBegin=std::chrono::system_clock::now();
        list *List=ListAndArray->List;
        for (int i=1; List!=NULL; i++, List=List->next)
          if (List->value==value)
            std::cout << i << " ";
        listTimeEnd=std::chrono::system_clock::now();
        std::cout << "\nЭлемент, со значением " << value << " в массиве находит"
                     "ся на позициях: ";
        arrayTimeBegin=std::chrono::system_clock::now();
        for (int i=0; i<ListAndArray->listSize; i++)
          if (*(ListAndArray->Array+i)==value)
            std::cout << i+1 << " ";
        arrayTimeEnd=std::chrono::system_clock::now();
      }
    std::cout << "\nНа осуществеление операции над списком было затрачено " <<
                 std::chrono::duration_cast<std::chrono::nanoseconds>(listTimeEnd-listTimeBegin).count()
                 << " наносекунд.\nНа осуществеление операции над массивом было"
                 " затрачено " << std::chrono::duration_cast<std::chrono::nanoseconds>(arrayTimeEnd-arrayTimeBegin).count()
                 << " наносекунд.";
    std::cout << "\nДля возврата в меню нажмите \"Enter\".";
    std::cin.get();
  }
