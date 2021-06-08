////////////////////////////////////////////////////////////////////////////////
//                                  ВНИМАНИЕ                                  //
////////////////////////////////////////////////////////////////////////////////
//      Для компиляции под ОС Windows с использованием компилятора MSVC       //
//                     раскомментируйте следующую строку:                     //
//#define CLEARSCREEN "cls"                                                     //
////////////////////////////////////////////////////////////////////////////////
//   Для компиляции под POSIX-совместимой ОС или ОС Windows с использованием  //
//           компилятора GCC/G++ раскомментируйте следующую строку:           //
#define CLEARSCREEN "clear"                                                 //
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstring>
#include <limits>
#include <climits>
#include <fstream>

struct intStack
  {
    int value;
    intStack *next;
  };

struct charStack
  {
    char *value;
    charStack *next;
  };

struct stack
{
    char *Value;
    int Size;
};

void interpretator ();
int getNumericAnswer (int minRange, int maxRange, const char *Question);
void infix (char *Expression);
void prefix (char *Expression);
void postfix (char *Expression);
intStack* pushIntStack (intStack *Stack, int value);
int popIntStack (intStack *Stack);
int peekIntStack (intStack *Stack);
void solvePostfix (char *Expression);
int solvePrefix (char **ExpressionInput);
charStack* pushCharStack (charStack *Stack, char *value);
char* popCharStack (charStack **Stack);
char *translatePrefix2Postfix (char **ExpressionInput);
char *translatePostfix2Prefix (char *Expression);
char *translatePostfix2PrefixStep2 (char **ExpressionInput);
char* translateInfixToRPN (char *Infix);
int RPNPriority (char Char);
stack* createStack ();
void pushStack (stack *Stack, char value);
char popStack (stack *Stack);
char peekStack (stack *Stack);
void solveInfix (char *Expression);
char *translatePrefix2Infix (char **ExpressionInput);
char *translatePostfix2Infix (char *Expression);

int main ()
  {
    interpretator ();
    /*
    char *Char = (char*) malloc (strlen ("(5 - 21 + 324) * 323") + 1);// * + - abra babr cadr dadr
    strcpy (Char, "(5 - 21 + 324) * 323"); //"* + - 5 21 324 323"
    char *Result = translatePostfix2Prefix(translateInfixToRPN (Char));
    std::cout << solvePrefix(&Result);
    */
  }

void interpretator ()
  {
    std::cout << "Вы хотите прочитать выражение:\n\n1) из файла.\n2) из терминала.\n";
    
    char *Input = (char*) malloc (sizeof (char));
    *Input = '\0';
    int capacity = 1;
    int length = 0;
    
    switch (getNumericAnswer (1, 2, "Введите номер желаемого варианта: "))
      {
        case 1:
          {
            char fileName[261];
            std::cout << std::endl << "Введите имя (или путь) файла: ";
            std::cin.getline (fileName, 261);
            std::ifstream File;
            File.open (fileName);
            if (File.is_open())
              {
                for (char Char = File.get(); !File.eof(); Char = File.get())
                  {
                    Input[length++] = Char;
                    if (!length && Input[length] == ' ')
                      length--;
                    if (length == capacity)
                      {
                        capacity *= 2;
                        Input = (char*) realloc (Input, capacity * sizeof (char));
                      }
                  }
                File.close();
              }
            else
              {
                std::cout << "Некорректный ввод!" << std::endl;
                free (Input);
                interpretator ();
                return;
              }
            break;
          }
        default:
          {
            std::cout << "> ";
            for (char Char = std::cin.get(); Char != '\n'; Char = std::cin.get())
              {
                Input[length++] = Char;
                if (!length && Input[length] == ' ')
                  length--;
                if (length == capacity)
                  {
                    capacity *= 2;
                    Input = (char*) realloc (Input, capacity * sizeof (char));
                  }
              }
            break;
          }
      }
    
    
    Input[length] = '\0';
    if (strncmp (Input, "exit", 4) && strncmp (Input, "quit", 4))
      {
        std::cout << "Выберите в какой нотации введено выражение:\n\n1) В инфиксной."
                     "\n2) В префиксной (польской).\n3) В постфиксной (обратной "
                     "польской).\n";
        switch (getNumericAnswer (1, 3, "Введите номер желаемого варианта: "))
          {
            case 1:
              infix (Input);
              break;
            case 2:
              prefix (Input);
              break;
            default:
              postfix (Input);
              break;
          }
        std::cout << std::endl;
        free (Input);
        interpretator ();
      }
    else
      free (Input);
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

intStack* pushIntStack (intStack *Stack, int value)
  {
    intStack *newStack = (intStack*) malloc (sizeof (intStack));
    newStack->value = value;
    newStack->next = Stack;
    return newStack;
  }

int popIntStack (intStack **Stack)
  {
    if (*Stack)
      {
        int value = (*Stack)->value;
        intStack *Next = (*Stack)->next;
        free (*Stack);
        *Stack = Next;
        return value;
      }
    return 0;
  }
  
int peekIntStack (intStack *Stack)
  {
    if (Stack)
      return Stack->value;
    return 0;
  }

charStack* pushCharStack (charStack *Stack, char *value)
  {
    charStack *newStack = (charStack*) malloc (sizeof (charStack));
    newStack->value = value;
    newStack->next = Stack;
    return newStack;
  }

char* popCharStack (charStack **Stack)
  {
    if (*Stack)
      {
        char *value = (*Stack)->value;
        charStack *Next = (*Stack)->next;
        free (*Stack);
        *Stack = Next;
        return value;
      }
    return NULL;
  }
  
void infix (char *Expression)
  {
    system(CLEARSCREEN);
    std::cout << "Выражение: " << Expression << std::endl << std::endl <<
                 "Меню:\n\n1) Вычислить выражение.\n2) Преобразовать " <<
                 "выражение в постфиксную форму.\n3) Преобразовать " <<
                 "выражение в префиксную форму\n0) Выход." << std::endl;
    switch (getNumericAnswer (0, 3, "Введите номер желаемого варианта: "))
      {
        case 1:
          {
            solveInfix (Expression);
            break;
          }
        case 2:
          {
            std::cout << "Результат: " << translateInfixToRPN (Expression) << std::endl;
            break;
          }
        case 3:
          {
            std::cout << "Результат: " <<
                         translatePostfix2Prefix(translateInfixToRPN (Expression)) << std::endl;
            break;
          }
        default:
          {
            return;
          }
      }
    std::cout << "Для возврата нажмите клавишу \"Ввод\".";
    std::cin.get();
    infix (Expression);
  }

void prefix (char *Expression)
  {
    system(CLEARSCREEN);
    std::cout << "Выражение: " << Expression << std::endl << std::endl <<
                 "Меню:\n\n1) Вычислить выражение.\n2) Преобразовать " <<
                 "выражение в постфиксную форму.\n3) Преобразовать выражение" <<
                 " в инфиксную форму.\n0) Выход." << std::endl;
    switch (getNumericAnswer (0, 3, "Введите номер желаемого варианта: "))
      {
        case 1:
          {
            char *Exp = Expression;
            std::cout << "Выражение: " << Expression << std::endl;
            int result = solvePrefix (&Exp);
            std::cout << "Результат: " << result << std::endl;
            break;
          }
        case 2:
          {
            char *Exp = Expression;
            std::cout << "Результат: " << translatePrefix2Postfix (&Exp) << std::endl;
            break;
          }
        case 3:
          {
            char *Exp = Expression;
            std::cout << "Результат: " << translatePrefix2Infix (&Exp) << std::endl;
            break;
          }
        default:
          {
            return;
          }
      }
    std::cout << "Для возврата нажмите клавишу \"Ввод\".";
    std::cin.get();
    prefix (Expression);
  }
  
void postfix (char *Expression)
  {
    system(CLEARSCREEN);
    std::cout << "Выражение: " << Expression << std::endl << std::endl <<
                 "Меню:\n\n1) Вычислить выражение.\n2) Преобразовать " <<
                 "выражение в префиксную форму.\n3) Преобразовать выражение" <<
                 " в инфиксную форму.\n0) Выход." << std::endl;
    switch (getNumericAnswer (0, 3, "Введите номер желаемого варианта: "))
      {
        case 1:
          {
            solvePostfix (Expression);
            break;
          }
        case 2:
          {
            std::cout << "Результат: " << translatePostfix2Prefix (Expression) << std::endl;
            break;
          }
        case 3:
          {
            std::cout << "Результат: " << translatePostfix2Infix (Expression) << std::endl;
            break;
          }
        default:
          {
            return;
          }
      }
    std::cout << "Для возврата нажмите клавишу \"Ввод\".";
    std::cin.get();
    postfix (Expression);
  }
  
void solvePostfix (char *Expression)
  {
    std::cout << "Выражение: " << Expression << std::endl;

    intStack *Stack = NULL;
    for (int i=0; Expression[i] != '\0'; i++)
      {
        if ((Expression[i] > 47 && Expression[i] < 58) ||
            (Expression[i] == '-' && Expression[i+1] > 47 && Expression[i+1] < 58))
          {
            bool isNegative = false;
            int num = 0;
            if (Expression[i] == '-')
              {
                isNegative = true;
                i++;
              }
            while (Expression[i] > 47 && Expression[i] < 58)
              num = num * 10 + Expression[i++] - 48;
            if (isNegative)
              num *= -1;
            Stack = pushIntStack (Stack, num);
          }
        else if ((Expression[i] > 64 && Expression[i] < 91) ||
                 (Expression[i] == '-' && Expression[i+1] > 64 && Expression[i+1] < 91) ||
                 (Expression[i] > 96 && Expression[i] < 123) ||
                 (Expression[i] == '-' && Expression[i+1] > 96 && Expression[i+1] < 123))
          {
            bool isNegative = false;
            if (Expression[i] == '-')
              {
                isNegative = true;
                i++;
              }
            
            char *Var = (char*) malloc (1);
            *Var = '\0';

            while ((Expression[i] > 64 && Expression[i] < 91) ||
                   (Expression[i] > 96 && Expression[i] < 123))
              {
                Var = (char*) realloc (Var, strlen (Var) + 2);
                Var[strlen (Var) + 1] = '\0';
                Var[strlen (Var)] = Expression[i++];
              }

            std::cout << "\nОбнаружена переменная " << Var << std::endl;
            int num = getNumericAnswer (INT_MIN, INT_MAX, "Введите значение переменной: ");
            std::cout << std::endl;
            if (isNegative)
              num *= -1;
            free (Var);
            Stack = pushIntStack (Stack, num);
          }
        else if (Expression[i] == '+' || Expression[i] == '-' ||
                 Expression[i] == '*' || Expression[i] == '/')
          {
            int num1;
            int num2;
            if (Stack)
              num2 = popIntStack (&Stack);
            else
              {
                std::cout << "" << std::endl;
                std::cin.get();
              }
            if (Stack)
              num1 = popIntStack (&Stack);
            else
              {
                std::cout << "" << std::endl;
                std::cin.get();
              }
            int num3;
            switch (Expression[i++])
              {
                case '+':
                  num3 = num1 + num2;
                  std::cout << num1 << '+' << num2 << '=' << num3 << std::endl;
                  break;
                case '-':
                  num3 = num1 - num2;
                  std::cout << num1 << '-' << num2 << '=' << num3 << std::endl;
                  break;
                case '*':
                  num3 = num1 * num2;
                  std::cout << num1 << '*' << num2 << '=' << num3 << std::endl;
                  break;
                case '/':
                  num3 = num1 / num2;
                  std::cout << num1 << '/' << num2 << '=' << num3 << std::endl;
                  break;
              }
            Stack = pushIntStack (Stack, num3);
          }
      }
    std::cout << "Результат: " << popIntStack (&Stack) << std::endl;
  }
  
int solvePrefix (char **ExpressionInput)
  {
    char Operation = **ExpressionInput;
    
    *ExpressionInput += 2;
    
    int num1 = 0;
    int num2 = 0;

    if ((**ExpressionInput > 47 && **ExpressionInput < 58) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 47 && *(*ExpressionInput+1) < 58))
      {
        bool isNegative = false;
        if (**ExpressionInput == '-')
          {
            isNegative = true;
            (*ExpressionInput)++;
          }
        while (**ExpressionInput > 47 && **ExpressionInput < 58)
          num1 = num1 * 10 + *((*ExpressionInput)++) - 48;
        if (isNegative)
          num1 *= -1;
      }
    else if ((**ExpressionInput > 64 && **ExpressionInput < 91) ||
             (**ExpressionInput == '-' && *(*ExpressionInput+1) > 64 && *(*ExpressionInput+1) < 91) ||
             (**ExpressionInput > 96 && **ExpressionInput < 123) ||
             (**ExpressionInput == '-' && *(*ExpressionInput+1) > 96 && *(*ExpressionInput+1) < 123))
      {
        bool isNegative = false;
        if (**ExpressionInput == '-')
          {
            isNegative = true;
            (*ExpressionInput)++;
          }
            
        char *Var = (char*) malloc (1);
        *Var = '\0';

        while ((**ExpressionInput > 64 && **ExpressionInput < 91) ||
               (**ExpressionInput > 96 && **ExpressionInput < 123))
          {
            Var = (char*) realloc (Var, strlen (Var) + 2);
            Var[strlen (Var) + 1] = '\0';
            Var[strlen (Var)] = *((*ExpressionInput)++);
          }

        std::cout << "\nОбнаружена переменная " << Var << std::endl;
        num1 = getNumericAnswer (INT_MIN, INT_MAX, "Введите значение переменной: ");
        std::cout << std::endl;
        if (isNegative)
          num1 *= -1;
        free (Var);
      }
    else if (**ExpressionInput == '+' || **ExpressionInput == '-' ||
             **ExpressionInput == '*' || **ExpressionInput == '/')
      num1 = solvePrefix (&(*ExpressionInput));
      
    (*ExpressionInput)++;
        
    if ((**ExpressionInput > 47 && **ExpressionInput < 58) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 47 && *(*ExpressionInput+1) < 58))
      {
        bool isNegative = false;
        if (**ExpressionInput == '-')
          {
            isNegative = true;
            (*ExpressionInput)++;
          }
        while (**ExpressionInput > 47 && **ExpressionInput < 58)
          num2 = num2 * 10 + *((*ExpressionInput)++) - 48;
        if (isNegative)
          num2 *= -1;
      }
    else if ((**ExpressionInput > 64 && **ExpressionInput < 91) ||
             (**ExpressionInput == '-' && *(*ExpressionInput+1) > 64 && *(*ExpressionInput+1) < 91) ||
             (**ExpressionInput > 96 && **ExpressionInput < 123) ||
             (**ExpressionInput == '-' && *(*ExpressionInput+1) > 96 && *(*ExpressionInput+1) < 123))
      {
        bool isNegative = false;
        if (**ExpressionInput == '-')
          {
            isNegative = true;
            (*ExpressionInput)++;
          }
            
        char *Var = (char*) malloc (1);
        *Var = '\0';

        while ((**ExpressionInput > 64 && **ExpressionInput < 91) ||
               (**ExpressionInput > 96 && **ExpressionInput < 123))
          {
            Var = (char*) realloc (Var, strlen (Var) + 2);
            Var[strlen (Var) + 1] = '\0';
            Var[strlen (Var)] = *((*ExpressionInput)++);
          }

        std::cout << "\nОбнаружена переменная " << Var << std::endl;
        num2 = getNumericAnswer (INT_MIN, INT_MAX, "Введите значение переменной: ");
        std::cout << std::endl;
        if (isNegative)
          num2 *= -1;
        free (Var);
      }
    else if (**ExpressionInput == '+' || **ExpressionInput == '-' ||
             **ExpressionInput == '*' || **ExpressionInput == '/')
      num2 = solvePrefix (&(*ExpressionInput));
        
    int num3;
    
    switch (Operation)
      {
        case '+':
          num3 = num1 + num2;
          std::cout << num1 << '+' << num2 << '=' << num3 << std::endl;
          break;
        case '-':
          num3 = num1 - num2;
          std::cout << num1 << '-' << num2 << '=' << num3 << std::endl;
          break;
        case '*':
          num3 = num1 * num2;
          std::cout << num1 << '*' << num2 << '=' << num3 << std::endl;
          break;
        case '/':
          num3 = num1 / num2;
          std::cout << num1 << '/' << num2 << '=' << num3 << std::endl;
          break;
      }
    return num3;
  }
  
char *translatePrefix2Postfix (char **ExpressionInput)
  {
    char *Result = (char*) malloc (1);
    *Result = '\0';
    
    char Operation = **ExpressionInput;
    
    *ExpressionInput += 2;
    
    char *Var1 = (char*) malloc (1);
    *Var1 = '\0';
    char *Var2 = (char*) malloc (1);
    *Var2 = '\0';
    
    if ((**ExpressionInput > 64 && **ExpressionInput < 91) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 64 && *(*ExpressionInput+1) < 91) ||
        (**ExpressionInput > 96 && **ExpressionInput < 123) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 96 && *(*ExpressionInput+1) < 123) ||
        (**ExpressionInput > 47 && **ExpressionInput < 58) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 47 && *(*ExpressionInput+1) < 58))
      {
        while ((**ExpressionInput > 64 && **ExpressionInput < 91) ||
               (**ExpressionInput == '-' && *(*ExpressionInput+1) > 64 && *(*ExpressionInput+1) < 91) ||
               (**ExpressionInput > 96 && **ExpressionInput < 123) ||
               (**ExpressionInput == '-' && *(*ExpressionInput+1) > 96 && *(*ExpressionInput+1) < 123) ||
               (**ExpressionInput > 47 && **ExpressionInput < 58) ||
               (**ExpressionInput == '-' && *(*ExpressionInput+1) > 47 && *(*ExpressionInput+1) < 58))
          {
            Var1 = (char*) realloc (Var1, strlen (Var1) + 2);
            Var1[strlen (Var1) + 1] = '\0';
            Var1[strlen (Var1)] = *((*ExpressionInput)++);
          }
      }
    else if (**ExpressionInput == '+' || **ExpressionInput == '-' ||
             **ExpressionInput == '*' || **ExpressionInput == '/')
      Var1 = translatePrefix2Postfix (ExpressionInput);
      
    (*ExpressionInput)++;

    if ((**ExpressionInput > 64 && **ExpressionInput < 91) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 64 && *(*ExpressionInput+1) < 91) ||
        (**ExpressionInput > 96 && **ExpressionInput < 123) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 96 && *(*ExpressionInput+1) < 123) ||
        (**ExpressionInput > 47 && **ExpressionInput < 58) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 47 && *(*ExpressionInput+1) < 58))
      {
        while ((**ExpressionInput > 64 && **ExpressionInput < 91) ||
               (**ExpressionInput == '-' && *(*ExpressionInput+1) > 64 && *(*ExpressionInput+1) < 91) ||
               (**ExpressionInput > 96 && **ExpressionInput < 123) ||
               (**ExpressionInput == '-' && *(*ExpressionInput+1) > 96 && *(*ExpressionInput+1) < 123) ||
               (**ExpressionInput > 47 && **ExpressionInput < 58) ||
               (**ExpressionInput == '-' && *(*ExpressionInput+1) > 47 && *(*ExpressionInput+1) < 58))
          {
            Var2 = (char*) realloc (Var2, strlen (Var2) + 2);
            Var2[strlen (Var2) + 1] = '\0';
            Var2[strlen (Var2)] = *((*ExpressionInput)++);
          }
      }
    else if (**ExpressionInput == '+' || **ExpressionInput == '-' ||
             **ExpressionInput == '*' || **ExpressionInput == '/')
      Var2 = translatePrefix2Postfix (ExpressionInput);
      
    Result = (char*) realloc (Result, strlen (Var1) + strlen (Var2) + 4);
    strcat (Result, Var1);
    Result[strlen (Result) + 1] = '\0';
    Result[strlen (Result)] = ' ';

    strcat (Result, Var2);
    Result[strlen (Result) + 1] = '\0';
    Result[strlen (Result)] = ' ';

    Result[strlen (Result) + 1] = '\0';
    Result[strlen (Result)] = Operation;
    
    free (Var1);
    free (Var2);
    
    return Result;
  }
  
char *translatePostfix2Prefix (char *Expression)
  {
    charStack *Stack = NULL;
    
    char *Var = (char*) malloc (1);
    *Var = '\0';

    for (int i=0; Expression[i] != '\0';)
      {
        if (Expression[i] != ' ' && Expression[i] != '\0')
          {
            char *Var = (char*) malloc (1);
            *Var = '\0';
            while (Expression[i] != ' ' && Expression[i] != '\0')
              {
                Var = (char*) realloc (Var, strlen (Var) + 2);
                Var[strlen (Var) + 1] = '\0';
                Var[strlen (Var)] = Expression[i++];
              }
            Stack = pushCharStack (Stack, Var);
          }
        else if (Expression[i] == ' ')
          i++;
      }
    
    char *NewChar = (char*) malloc (1);
    *NewChar = '\0';
    
    while (Stack)
      {
        char *Var = popCharStack (&Stack);
        NewChar = (char*) realloc (NewChar, strlen (NewChar) + strlen (Var) + 2);
        strcat (NewChar, Var);
        NewChar[strlen (NewChar) + 1] = '\0';
        NewChar[strlen (NewChar)] = ' ';
        free (Var);
      }
    NewChar[strlen (NewChar)] = '\0';

    char *Result = translatePostfix2PrefixStep2 (&NewChar);
    
    return Result;    
  }
  
char *translatePostfix2PrefixStep2 (char **ExpressionInput)
  {
    char *Result = (char*) malloc (1);
    *Result = '\0';
    
    char Operation = **ExpressionInput;
    
    *ExpressionInput += 2;
    
    char *Var1 = (char*) malloc (1);
    *Var1 = '\0';
    char *Var2 = (char*) malloc (1);
    *Var2 = '\0';
    if ((**ExpressionInput > 64 && **ExpressionInput < 91) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 64 && *(*ExpressionInput+1) < 91) ||
        (**ExpressionInput > 96 && **ExpressionInput < 123) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 96 && *(*ExpressionInput+1) < 123) ||
        (**ExpressionInput > 47 && **ExpressionInput < 58) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 47 && *(*ExpressionInput+1) < 58))
      {
        while ((**ExpressionInput > 64 && **ExpressionInput < 91) ||
               (**ExpressionInput == '-' && *(*ExpressionInput+1) > 64 && *(*ExpressionInput+1) < 91) ||
               (**ExpressionInput > 96 && **ExpressionInput < 123) ||
               (**ExpressionInput == '-' && *(*ExpressionInput+1) > 96 && *(*ExpressionInput+1) < 123) ||
               (**ExpressionInput > 47 && **ExpressionInput < 58) ||
               (**ExpressionInput == '-' && *(*ExpressionInput+1) > 47 && *(*ExpressionInput+1) < 58))
          {
            Var1 = (char*) realloc (Var1, strlen (Var1) + 2);
            Var1[strlen (Var1) + 1] = '\0';
            Var1[strlen (Var1)] = *((*ExpressionInput)++);
          }
      }
    else if (**ExpressionInput == '+' || **ExpressionInput == '-' ||
             **ExpressionInput == '*' || **ExpressionInput == '/')
      Var1 = translatePostfix2PrefixStep2 (ExpressionInput);

    (*ExpressionInput)++;

    if ((**ExpressionInput > 64 && **ExpressionInput < 91) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 64 && *(*ExpressionInput+1) < 91) ||
        (**ExpressionInput > 96 && **ExpressionInput < 123) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 96 && *(*ExpressionInput+1) < 123) ||
        (**ExpressionInput > 47 && **ExpressionInput < 58) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 47 && *(*ExpressionInput+1) < 58))
      {
        while ((**ExpressionInput > 64 && **ExpressionInput < 91) ||
               (**ExpressionInput == '-' && *(*ExpressionInput+1) > 64 && *(*ExpressionInput+1) < 91) ||
               (**ExpressionInput > 96 && **ExpressionInput < 123) ||
               (**ExpressionInput == '-' && *(*ExpressionInput+1) > 96 && *(*ExpressionInput+1) < 123) ||
               (**ExpressionInput > 47 && **ExpressionInput < 58) ||
               (**ExpressionInput == '-' && *(*ExpressionInput+1) > 47 && *(*ExpressionInput+1) < 58))
          {
            Var2 = (char*) realloc (Var2, strlen (Var2) + 2);
            Var2[strlen (Var2) + 1] = '\0';
            Var2[strlen (Var2)] = *((*ExpressionInput)++);
          }
      }
    else if (**ExpressionInput == '+' || **ExpressionInput == '-' ||
             **ExpressionInput == '*' || **ExpressionInput == '/')
      Var2 = translatePostfix2PrefixStep2 (ExpressionInput);

    Result = (char*) realloc (Result, strlen (Var1) + strlen (Var2) + 4);
    
    Result[strlen (Result) + 2] = '\0';
    Result[strlen (Result) + 1] = ' ';
    Result[strlen (Result)] = Operation;

    
    strcat (Result, Var2);
    Result[strlen (Result) + 1] = '\0';
    Result[strlen (Result)] = ' ';

    strcat (Result, Var1);

    free (Var1);
    free (Var2);

    return Result;
  }

char* translateInfixToRPN (char *Infix)
  {
    int InfixLength = strlen (Infix);
    char *RPN = (char*) malloc (5 * InfixLength+1 * sizeof (char));
    int RPNLength = 0;
    stack *Operations = createStack ();
    for (int i=0; i<InfixLength; i++)
      {
        if (Infix [i] == '\0')
          {
            break;
          }
        if ((Infix [i] < 58 && Infix [i] > 47) || Infix [i] == 32 ||
            (Infix [i] < 91 && Infix [i] > 64) || (Infix [i] < 123 && Infix [i] > 96) ||
            (Infix[i] == '-' && (Infix [i+1] < 91 && Infix [i+1] > 64)) ||
            (Infix[i] == '-' && (Infix [i+1] < 123 && Infix [i+1] > 96)) ||
            (Infix[i] == '-' && (Infix [i+1] < 58 && Infix [i+1] > 47)))
          {
            *(RPN+RPNLength) = Infix [i];
            RPNLength++;
          }
        else
          {
            *(RPN+RPNLength) = ' ';
            RPNLength++;
            char Operation;
            switch (RPNPriority (Infix [i]))
              {
                case 1:
                  pushStack (Operations, Infix [i]);
                  break;
                case 0:
                  while (1)
                    {
                      *(RPN+RPNLength) = ' ';
                      RPNLength++;
                      Operation = popStack (Operations);
                      if (Operation == '(') break;
                      *(RPN+RPNLength) = Operation;
                      RPNLength++;
                    }
                  break;
                case 2:
                  while (Operations->Size && RPNPriority (peekStack (Operations)) >= RPNPriority (Infix [i]))
                    {
                      *(RPN+RPNLength) = ' ';
                      RPNLength++;
                      Operation = popStack (Operations);
                      *(RPN+RPNLength) = Operation;
                      RPNLength++;
                      *(RPN+RPNLength) = ' ';
                      RPNLength++;
                    }
                  pushStack (Operations, Infix [i]);
                  break;
                case 3:
                  while (Operations->Size && RPNPriority (peekStack (Operations)) >= RPNPriority (Infix [i]))
                    {
                      *(RPN+RPNLength) = ' ';
                      RPNLength++;
                      char Operation = popStack (Operations);
                      *(RPN+RPNLength) = Operation;
                      RPNLength++;
                      *(RPN+RPNLength) = ' ';
                      RPNLength++;
                    }
                  pushStack (Operations, Infix [i]);
                  break;
              }
          }
      }
    while (Operations->Size)
      {
        char Operation = popStack (Operations);
        *(RPN+RPNLength) = ' ';
        RPNLength++;
        *(RPN+RPNLength) = Operation;
        RPNLength++;
      }
    *(RPN+RPNLength) = '\0';
    RPNLength++;
    char *RPNfixed = (char*) malloc (2 * InfixLength+1 * sizeof (char)); 
    int RPNfixedLength = 0;
    int spaceCount=0;
    for (unsigned int i=0; i < strlen (RPN); i++)
      {
        if (*(RPN+i)==32)
          {
            if (!spaceCount)
              {
                *(RPNfixed+RPNfixedLength)=' ';
                RPNfixedLength++;
                spaceCount++;
              }
          }
        else
          {
            *(RPNfixed+RPNfixedLength)=*(RPN+i);
            RPNfixedLength++;
            spaceCount=0;
          }
      }
    *(RPNfixed+RPNfixedLength)='\0';
    if (*RPNfixed == ' ')
      for (int i = 0; RPNfixed[i] != '\0'; i++)
        RPNfixed[i] = RPNfixed[i+1];
    return RPNfixed;
  }

int RPNPriority (char Char)
  {
    switch (Char)
      {
	      case '(':
          return 1;
	      case '+': case '-':
          return 2;
	      case '*': case '/':
          return 3;
	      default:
          return 0;
      }
  }

stack* createStack ()
  {
    stack *Stack = (stack*) malloc (sizeof (stack));
    Stack->Value = NULL;
    Stack->Size = 0;
    return Stack;
  }

void pushStack (stack *Stack, char value)
  {
    Stack->Size++;
    Stack->Value = (char*) realloc (Stack->Value, Stack->Size * sizeof (char));
    *(Stack->Value+Stack->Size-1) = value;
  }

char popStack (stack *Stack)
  {
    if (Stack->Size)
      {
        Stack->Size--;
        char value = *(Stack->Value+Stack->Size);
        Stack->Value = (char*) realloc (Stack->Value, Stack->Size * sizeof (char));
        return value;
      }
    return 0;
  }

char peekStack (stack *Stack)
  {
    if (Stack->Size)
      {
        char value = *(Stack->Value+Stack->Size-1);
        return value;
      }
    return 0;
  }

void solveInfix (char *Expression)
  {
    std::cout << "Выражение: " << Expression << std::endl;
    char *Postfix = translateInfixToRPN (Expression);
    std::cout << "Преобразуем в постфиксную форму: " << Postfix << std::endl;
    solvePostfix (Postfix);
    free (Postfix);
  }

char *translatePrefix2Infix (char **ExpressionInput)
  {
    char *Result = (char*) malloc (1);
    *Result = '\0';
    
    char Operation = **ExpressionInput;
    
    *ExpressionInput += 2;
    
    char *Var1 = (char*) malloc (1);
    *Var1 = '\0';
    char *Var2 = (char*) malloc (1);
    *Var2 = '\0';
    
    if ((**ExpressionInput > 64 && **ExpressionInput < 91) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 64 && *(*ExpressionInput+1) < 91) ||
        (**ExpressionInput > 96 && **ExpressionInput < 123) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 96 && *(*ExpressionInput+1) < 123) ||
        (**ExpressionInput > 47 && **ExpressionInput < 58) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 47 && *(*ExpressionInput+1) < 58))
      {
        while ((**ExpressionInput > 64 && **ExpressionInput < 91) ||
               (**ExpressionInput == '-' && *(*ExpressionInput+1) > 64 && *(*ExpressionInput+1) < 91) ||
               (**ExpressionInput > 96 && **ExpressionInput < 123) ||
               (**ExpressionInput == '-' && *(*ExpressionInput+1) > 96 && *(*ExpressionInput+1) < 123) ||
               (**ExpressionInput > 47 && **ExpressionInput < 58) ||
               (**ExpressionInput == '-' && *(*ExpressionInput+1) > 47 && *(*ExpressionInput+1) < 58))
          {
            Var1 = (char*) realloc (Var1, strlen (Var1) + 2);
            Var1[strlen (Var1) + 1] = '\0';
            Var1[strlen (Var1)] = *((*ExpressionInput)++);
          }
      }
    else if (**ExpressionInput == '+' || **ExpressionInput == '-' ||
             **ExpressionInput == '*' || **ExpressionInput == '/')
      Var1 = translatePrefix2Infix (ExpressionInput);
      
    (*ExpressionInput)++;

    if ((**ExpressionInput > 64 && **ExpressionInput < 91) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 64 && *(*ExpressionInput+1) < 91) ||
        (**ExpressionInput > 96 && **ExpressionInput < 123) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 96 && *(*ExpressionInput+1) < 123) ||
        (**ExpressionInput > 47 && **ExpressionInput < 58) ||
        (**ExpressionInput == '-' && *(*ExpressionInput+1) > 47 && *(*ExpressionInput+1) < 58))
      {
        while ((**ExpressionInput > 64 && **ExpressionInput < 91) ||
               (**ExpressionInput == '-' && *(*ExpressionInput+1) > 64 && *(*ExpressionInput+1) < 91) ||
               (**ExpressionInput > 96 && **ExpressionInput < 123) ||
               (**ExpressionInput == '-' && *(*ExpressionInput+1) > 96 && *(*ExpressionInput+1) < 123) ||
               (**ExpressionInput > 47 && **ExpressionInput < 58) ||
               (**ExpressionInput == '-' && *(*ExpressionInput+1) > 47 && *(*ExpressionInput+1) < 58))
          {
            Var2 = (char*) realloc (Var2, strlen (Var2) + 2);
            Var2[strlen (Var2) + 1] = '\0';
            Var2[strlen (Var2)] = *((*ExpressionInput)++);
          }
      }
    else if (**ExpressionInput == '+' || **ExpressionInput == '-' ||
             **ExpressionInput == '*' || **ExpressionInput == '/')
      Var2 = translatePrefix2Infix (ExpressionInput);
      
    Result = (char*) realloc (Result, strlen (Var1) + strlen (Var2) + 6);
    
    Result[1] = '\0';
    Result[0] = '(';
    
    strcat (Result, Var1);
    Result[strlen (Result) + 1] = '\0';
    Result[strlen (Result)] = ' ';

    Result[strlen (Result) + 2] = '\0';
    Result[strlen (Result) + 1] = ' ';
    Result[strlen (Result)] = Operation;

    strcat (Result, Var2);
    Result[strlen (Result) + 1] = '\0';
    Result[strlen (Result)] = ')';

    free (Var1);
    free (Var2);
    
    return Result;
  }

char *translatePostfix2Infix (char *Expression)
  {
    char *Prefix = translatePostfix2Prefix (Expression);
    char *Result = translatePrefix2Infix (&Prefix);
    free (Prefix);
    return Result;
  }
  

  
  
  
  
  
