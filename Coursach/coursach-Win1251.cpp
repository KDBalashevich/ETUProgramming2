////////////////////////////////////////////////////////////////////////////////
//                                  ��������                                  //
////////////////////////////////////////////////////////////////////////////////
//      ��� ���������� ��� �� Windows � �������������� ����������� MSVC       //
//                     ���������������� ��������� ������:                     //
#define CLEARSCREEN "cls"                                                     //
////////////////////////////////////////////////////////////////////////////////
//   ��� ���������� ��� POSIX-����������� �� ��� �� Windows � ��������������  //
//           ����������� GCC/G++ ���������������� ��������� ������:           //
//#define CLEARSCREEN "clear"                                                 //
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstring>
#include <fstream>
#include <limits>
#include <climits>
#include <chrono>

struct tree
  {
    tree *left;
    tree *right;
    int value;
  };

void mainMenu (tree *Tree, int &Size);
int getNumericAnswer (int minRange, int maxRange, const char *Question);
tree* initTree (int value);
void addNode (int value, tree *Tree);
tree* generateTree (int *Array, int size);
void printTree (tree *Tree, int Side, const char *LeftOtp, const char *RightOtp);
void printTreeToFile (tree *Tree, int Side, const char *LeftOtp, const char *RightOtp, std::ostream &File);
int* readTreeFromTerm (int &Size);
int* readTreeFromFile (int &Size);
int* generateRandomTree (int &Size);
tree* deleteElement (int Element, tree *Tree);
tree* deleteNode (tree *Tree);
bool findElement (int Element, tree *Tree);
void obhodPryamoi (tree *Tree);
void obhodObratnyi (tree *Tree);
void obhodShir (tree **Array, int Size);
void obhodShirToFile (tree **Array, int Size, std::ofstream &File);
void test ();
void generateVariant (std::ofstream &Tasks, std::ofstream &Keys, std::ofstream &Answers);
void deleteTree (tree *Tree);

int main ()
  {
    setlocale (0, "");
    srand (time (0));
    
    int Size = 0;
    int *Array = NULL;
    
    std::cout << "����� ����������!\n\n�������� ������ ������������ ��������� "
              << "������:\n\n1) ������������ ������ �������������.\n2) ������� "
              << "������ �� ���������.\n3) ������� ������ �� �����.\n4) "
              << "������������� �������� �������.\n0) �����\n\n";
    
    int answer = getNumericAnswer (0, 4, "������� ����� ��������� ��������: ");
    
    switch (answer)
      {
        case 1:
          Size = getNumericAnswer(0, INT_MAX, "������� ����� ��������� � ������/�������: ");
          Array = generateRandomTree (Size);
          break;
        case 2:
          Array = readTreeFromTerm (Size);
          break;
        case 3:
          Array = readTreeFromFile (Size);
          break;
        case 4:
          test ();
          return 0;
        default:
          return 0;
      }
    
    std::chrono::system_clock::time_point TimeBegin, TimeEnd;
    TimeBegin=std::chrono::system_clock::now();
    
    tree *Tree = generateTree (Array, Size);
    
    TimeEnd=std::chrono::system_clock::now();
    std::cout << "\n�� ��������� ������ ���� ��������� " <<
                 std::chrono::duration_cast<std::chrono::nanoseconds>(TimeEnd-TimeBegin).count()
                 << " ����������.\n\n��� ����������� ������� ������� \"����\"";
    std::cin.get();
    
    free (Array);

    mainMenu (Tree, Size);

    deleteTree (Tree);
    return 0;
  }

void mainMenu (tree *Tree, int &Size)
  {
    system (CLEARSCREEN);
    
    std::cout << "������� ��������� ������:\n\n";
    
    printTree (Tree, 0, "", "");
    
    std::cout << "\n\n������� ����:\n\n1) ����� �������� ��������� ������ � ����."
              << "\n2) ������� ������ �������� � ������.\n3) �������� �������� �� "
              << "������.\n4) ��������� �������� �� ������.\n5) ������ ����� ������."
              << "\n6) �������� ����� ������.\n7) ����� ������ � ������.\n0) �����.\n\n";

    int answer = getNumericAnswer (0, 7, "������� ����� ��������� ��������: ");
        
    switch (answer)
      {
        case 1:
          {
            std::ofstream File;
            File.open ("tree");
            if (File.is_open())
              {
                printTreeToFile (Tree, 0, "", "", File);
                File.close();
              }

            system(CLEARSCREEN);

            std::cout << "���������!\n��� �������� ������� ������� \"����\"";
            std::cin.get();
            break;
          }
        case 2:
          {
            int value = getNumericAnswer (INT_MIN, INT_MAX, "������� �������� ������ ��������: ");

            std::chrono::system_clock::time_point TimeBegin, TimeEnd;
            TimeBegin=std::chrono::system_clock::now();

            addNode (value, Tree);

            TimeEnd=std::chrono::system_clock::now();

            system(CLEARSCREEN);
            std::cout << "���������!\n�� ���������� �������� ���� ��������� " <<
                         std::chrono::duration_cast<std::chrono::nanoseconds>(TimeEnd-TimeBegin).count() <<
                         " ����������.\n\n��� �������� ������� ������� \"����\"";
            std::cin.get();
            break;
          }
        case 3:
          {
            int value = getNumericAnswer (INT_MIN, INT_MAX, "������� �������� ���������� ��������: ");
            
            std::chrono::system_clock::time_point TimeBegin, TimeEnd;
            TimeBegin=std::chrono::system_clock::now();
            
            deleteElement (value, Tree);
            
            TimeEnd=std::chrono::system_clock::now();
            
            system(CLEARSCREEN);
            std::cout << "���������!\n�� �������� �������� ���� ��������� " <<
                         std::chrono::duration_cast<std::chrono::nanoseconds>(TimeEnd-TimeBegin).count() <<
                         " ����������.\n\n��� �������� ������� ������� \"����\"";
            std::cin.get();
            break;
          }
        case 4:
          {
            int value = getNumericAnswer (INT_MIN, INT_MAX, "������� �������� �������� ��������: ");
            system(CLEARSCREEN);

            std::chrono::system_clock::time_point TimeBegin, TimeEnd;
            TimeBegin=std::chrono::system_clock::now();

            switch (findElement (value, Tree))
              {
                case true:
                  TimeEnd=std::chrono::system_clock::now();
                  std::cout << "������� ������.";
                  break;
                default:
                  TimeEnd=std::chrono::system_clock::now();
                  std::cout << "������� �� ������.";
                  break;
              }
            
            std::cout << "\n�� ����� �������� ���� ��������� " <<
                         std::chrono::duration_cast<std::chrono::nanoseconds>(TimeEnd-TimeBegin).count() <<
                         " ����������.\n\n��� �������� ������� ������� \"����\"";
            std::cin.get();
            break;
          }
        case 5:
          std::cout << "�������� ������: ";
          obhodPryamoi (Tree);
          std::cout << "\n��� �������� ������� ������� \"����\"";
          std::cin.get();
          break;
        case 6:
          std::cout << "�������� ������: ";
          obhodObratnyi (Tree);
          std::cout << "\n��� �������� ������� ������� \"����\"";
          std::cin.get();
          break;
        case 7:
          std::cout << "�������� ������: ";
          obhodShir (&Tree, 1);
          std::cout << "\n��� �������� ������� ������� \"����\"";
          std::cin.get();
          break;
        default:
          return;
      }
    mainMenu (Tree, Size);
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
        std::cout << "������������ ����!" << std::endl;
        answer=getNumericAnswer (minRange, maxRange, Question);
        return answer;
      }
    else
      {
        std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');
        return answer;
      }
  }

tree* initTree (int value)
  {
    tree *Tree = (tree*) malloc (sizeof (tree));
    Tree->value = value;
    Tree->left = NULL;
    Tree->right = NULL;
    return Tree;
  }

void addNode (int value, tree *Tree)
  {
    if (value == Tree->value)
      return;
    else if (value < Tree->value)
      if (Tree->left)
        addNode (value, Tree->left);
      else
        Tree->left = initTree (value);
    else
      if (Tree->right)
        addNode (value, Tree->right);
      else
        Tree->right = initTree (value);
  }

tree* generateTree (int *Array, int size)
  {
    tree *Tree = initTree (*Array);
    for (int i = 1; i < size; i++)
      addNode (Array[i], Tree);
    return Tree;
  }

void printTree (tree *Tree, int Side, const char *LeftOtp, const char *RightOtp)
  {
    if (Tree->right)
      {
        char *NextLeftOtp = (char*) malloc (strlen (RightOtp) + 6);
        strcpy (NextLeftOtp, RightOtp);
        strcat (NextLeftOtp, "    |");
        
        char *NextRightOtp = (char*) malloc (strlen (RightOtp) + 6);
        strcpy (NextRightOtp, RightOtp);
        strcat (NextRightOtp, "     ");
        
        printTree (Tree->right, 1, NextLeftOtp, NextRightOtp);
        
        free (NextLeftOtp);
        free (NextRightOtp);
    }
    
    switch (Side)
      {
        case 0:
          std::cout << " -->" << Tree->value << std::endl;
          break;
        case 1:
          std::cout << RightOtp << ".-->" << Tree->value << std::endl;
          break;
        case -1:
          std::cout << LeftOtp << "`-->" << Tree->value << std::endl;
          break;
      }
    
    if (Tree->left)
      {
        char *NextLeftOtp = (char*) malloc (strlen (LeftOtp) + 6);
        strcpy (NextLeftOtp, LeftOtp);
        strcat (NextLeftOtp, "     ");
        
        char *NextRightOtp = (char*) malloc (strlen (LeftOtp) + 6);
        strcpy (NextRightOtp, LeftOtp);
        strcat (NextRightOtp, "    |");
        
        printTree (Tree->left, -1, NextLeftOtp, NextRightOtp);
    
        free (NextLeftOtp);
        free (NextRightOtp);
    }
  }

void printTreeToFile (tree *Tree, int Side, const char *LeftOtp, const char *RightOtp, std::ostream &File)
  {
    if (Tree->right)
      {
        char *NextLeftOtp = (char*) malloc (strlen (RightOtp) + 6);
        strcpy (NextLeftOtp, RightOtp);
        strcat (NextLeftOtp, "    |");
        
        char *NextRightOtp = (char*) malloc (strlen (RightOtp) + 6);
        strcpy (NextRightOtp, RightOtp);
        strcat (NextRightOtp, "     ");
        
        printTreeToFile (Tree->right, 1, NextLeftOtp, NextRightOtp, File);
        
        free (NextLeftOtp);
        free (NextRightOtp);
    }
    
    switch (Side)
      {
        case 0:
          File << " -->" << Tree->value << std::endl;
          break;
        case 1:
          File << RightOtp << ".-->" << Tree->value << std::endl;
          break;
        case -1:
          File << LeftOtp << "`-->" << Tree->value << std::endl;
          break;
      }
    
    if (Tree->left)
      {
        char *NextLeftOtp = (char*) malloc (strlen (LeftOtp) + 6);
        strcpy (NextLeftOtp, LeftOtp);
        strcat (NextLeftOtp, "     ");
        
        char *NextRightOtp = (char*) malloc (strlen (LeftOtp) + 6);
        strcpy (NextRightOtp, LeftOtp);
        strcat (NextRightOtp, "    |");
        
        printTreeToFile (Tree->left, -1, NextLeftOtp, NextRightOtp, File);
    
        free (NextLeftOtp);
        free (NextRightOtp);
    }
  }

int* readTreeFromTerm (int &Size)
  {
    std::cout << "������� �������� ������ � ���� ������ ����� ������: ";
    int *Array = NULL;
    char Buffer;
    Buffer=std::cin.get();
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
            std::cout << "������������ ����!" << std::endl;
            free (Array);
            Size = 0;
            Array = readTreeFromTerm (Size);
            return Array;
          }
        Array = (int*) realloc (Array, sizeof (int) * ++Size);
        Array[Size-1] = value;
        while (!(Buffer-32))
          Buffer=std::cin.get();
      }
    if (Size == 0)
      {
        std::cout << "������������ ����!" << std::endl;
        Array = readTreeFromTerm (Size);
      }
    return Array;
  }

int* readTreeFromFile (int &Size)
  {
    char fileName[261];
    std::cout << std::endl << "������� ��� (��� ����) �����: ";
    std::cin.getline (fileName, 261);
    std::ifstream File;
    File.open (fileName);
    int *Array = NULL;
    if (File.is_open())
      {
        
        while (!File.eof())
          {
            char *buffer = (char*) malloc (sizeof (char)*20);
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
            if (isNegative)
              value=value*(-1);
            Array = (int*) realloc (Array, sizeof (int) * ++Size);
            Array[Size-1] = value;
          }
        File.close();
      }
    else
      {
        std::cout << "������������ ����!" << std::endl;
        free (Array);
        Size = 0;
        Array = readTreeFromFile (Size);
      }
    return Array;
  }

int* generateRandomTree (int &Size)
  {
    int *Array = (int*) malloc (sizeof (int) * Size);
    for (int i = 0; i < Size; i++)
      Array[i] = rand()%198 - 99;
    return Array;
  }

tree* deleteElement (int Element, tree *Tree)
  {
    if (Tree->value == Element)
      Tree = deleteNode (Tree);
    else
      {
        if (Tree->left)
          Tree->left = deleteElement (Element, Tree->left);
        if (Tree->right)
          Tree->right = deleteElement (Element, Tree->right);
      }
    return Tree;
  }

tree* deleteNode (tree *Tree)
  {
    if (!Tree->left && !Tree->right)
      {
        free (Tree);
        return NULL;
      }
    else if (Tree->left && Tree->right)
      {
        Tree->value = Tree->left->value;
        Tree->left = deleteNode (Tree->left);
        return Tree;
      }
    else if (Tree->left)
      {
        tree *Buffer = Tree->left;
        free (Tree);
        return Buffer;
      }
    else
      {
        tree *Buffer = Tree->right;
        free (Tree);
        return Buffer;
      }
  }

bool findElement (int Element, tree *Tree)
  {
    if (Tree == NULL)
      return false;
    else if (Tree->value == Element)
      return true;
    else if (Tree->value > Element)
      return findElement (Element, Tree->left);
    else
      return findElement (Element, Tree->right);
  }

void obhodPryamoi (tree *Tree)
  {
    if (!Tree)
      return;
    std::cout << Tree->value << ' ';
    obhodPryamoi (Tree->left);
    obhodPryamoi (Tree->right);
  }

void obhodObratnyi (tree *Tree)
  {
    if (!Tree)
      return;
    obhodObratnyi (Tree->left);
    obhodObratnyi (Tree->right);
    std::cout << Tree->value << ' ';
  }

void obhodShir (tree **Array, int Size)
  {
    if (!Array)
      return;
    for (int i = 0; i < Size; i++)
      std::cout << (*(Array + i))->value << ' ';
    tree **NewArray = NULL;
    int j = 0;
    for (int i = 0; i < Size; i++)
      {
        if ((*(Array + i))->left)
          {
            NewArray = (tree**) realloc (NewArray, sizeof (tree*) * ++j);
            *(NewArray + j - 1) = (*(Array + i))->left;
          }
        if ((*(Array + i))->right)
          {
            NewArray = (tree**) realloc (NewArray, sizeof (tree*) * ++j);
            *(NewArray + j - 1) = (*(Array + i))->right;
          }
      }
    obhodShir (NewArray, j);
    free (NewArray);
  }

void obhodShirToFile (tree **Array, int Size, std::ofstream &File)
  {
    if (!Array)
      return;
    for (int i = 0; i < Size; i++)
      File << (*(Array + i))->value << ' ';
    tree **NewArray = NULL;
    int j = 0;
    for (int i = 0; i < Size; i++)
      {
        if ((*(Array + i))->left)
          {
            NewArray = (tree**) realloc (NewArray, sizeof (tree*) * ++j);
            *(NewArray + j - 1) = (*(Array + i))->left;
          }
        if ((*(Array + i))->right)
          {
            NewArray = (tree**) realloc (NewArray, sizeof (tree*) * ++j);
            *(NewArray + j - 1) = (*(Array + i))->right;
          }
      }
    obhodShirToFile (NewArray, j, File);
    free (NewArray);
  }

void test ()
  {
    int num = getNumericAnswer (1, INT_MAX, "������� ����� ����������� ���������: ");
    std::ofstream Tasks;
    std::ofstream Keys;
    std::ofstream Answers;
    
    Tasks.open ("output_task");
    Keys.open ("output_key");
    Answers.open ("output_ans");
    if (Tasks.is_open () && Answers.is_open () && Keys.is_open ())
      {
        for (int i = 1; i <= num; i++)
          {
            Tasks << "������� " << i << std::endl << std::endl;
            Keys << "������� " << i << std::endl << std::endl;
            Answers << "������� " << i << std::endl << std::endl;
            generateVariant (Tasks, Keys, Answers);
          }
        Tasks.close();
        Keys.close();
        Answers.close();
      }
    else
      {
        if (Tasks.is_open ())
          Tasks.close ();
        if (Keys.is_open ())
          Keys.close ();
        if (Answers.is_open ())
          Answers.close ();
        std::cout << "������ �������� ������!\n��� ������ ������� ������� \"����\"";
        std::cin.get();
      }
  }

void generateVariant (std::ofstream &Tasks, std::ofstream &Keys, std::ofstream &Answers)
  {
    int Size = rand()%10 + 10;
    int *Array = generateRandomTree (Size);
    
    tree *Tree = generateTree (Array, Size);
    
    Tasks << "\n1) ������������ �������� ������ ������ �� ���������� �������: \n\n";
    for (int i=0; i<Size; i++)
      Tasks << Array[i] << ' ';
    Tasks << std::endl;
    
    Keys << "\n1)\n";
    obhodShirToFile (&Tree, 1, Keys);
    Keys << std::endl;
    
    Answers << "\n1)\n";
    printTreeToFile (Tree, 0, "", "", Answers);
    Answers << std::endl;
    
    int num1 = Array[rand()%Size];
    int num2 = Array[rand()%Size];
    
    Tasks << "\n2) ������� �� ������, ��������������� � ������ 1, ��������: ";
    Tasks << num1 << ' ' << num2;
    Tasks << std::endl;
    
    Tree = deleteElement (num1, Tree);
    Tree = deleteElement (num2, Tree);

    Keys << "\n2)\n";
    obhodShirToFile (&Tree, 1, Keys);
    Keys << std::endl;

    Answers << "\n2)\n";
    printTreeToFile (Tree, 0, "", "", Answers);
    Answers << std::endl;

    num1 = rand()%198 - 99;
    num2 = rand()%198 - 99;
    
    Tasks << "\n3) �������� � ������, ����������� � ������ 2, ��������: ";
    Tasks << num1 << ' ' << num2;
    Tasks << std::endl << std::endl << std::endl;
    
    addNode (num1, Tree);
    addNode (num2, Tree);

    Keys << "\n3)\n";
    obhodShirToFile (&Tree, 1, Keys);
    Keys << std::endl << std::endl << std::endl;

    Answers << "\n3)\n";
    printTreeToFile (Tree, 0, "", "", Answers);
    Answers << std::endl << std::endl << std::endl;
    
    free (Array);
    deleteTree (Tree);
  }

void deleteTree (tree *Tree)
  {
    if (!Tree)
      return;
    deleteTree (Tree->left);
    deleteTree (Tree->right);
    free (Tree);
  }
