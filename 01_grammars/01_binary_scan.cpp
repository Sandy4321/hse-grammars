//#include "stdafx.h" // Подключаем прекомлируемый заголовочный файл, если он есть в проекте
#include <iostream> // Используем стандартный ввод и вывод - подключаем заголовочный файл

char c; // Текущий считанный символ

int number;
int sign;

/*
  Процедура считывания следующего символа
*/
void gc() {
  std::cin >> c; // Считываем символ со стандартного ввода
}

/*
  Функция преобразования символа в число
*/
int num(char ch) {
  return ch - '0'; // Цифры в таблице символов расположены последовтельно, начиная с 0
}

/*
  Функция проверки ввода на принадлежность языку, описываемому грамматикой:

  START -> + T0 | - T0 | 0 T1 | 1 T1
  T0 -> 0 T1 | 1 T1
  T1 -> 0 T1 | 1 T1 | $
*/
bool check() {  
  enum State { START, T0, T1, ERR, FIN }; // Все нетерминалы + завершающее состояние и состояние ошибки

  State state = START; // Переменная, хранящая состояние, записываем в нее начальное состояние
  gc(); // Считываем первый символ
  
  do { // Вся обработка производится в цикле
    switch ( state ) { // Обработка различных состояний
      case START: // Начальное состояние
        if ( c == '+'  ) { // Если текущий символ - плюс
          sign = 1; // Запоминаем знак
          gc(); // Считываем следующий символ
          state = T0; // И переходим в состояние T0
        } else if ( c == '-' ) { // Если текущий символ - минус
          sign = -1; // Запоминаем знак
          gc(); // Считываем следующий символ
          state = T0; // И переходим в состояние T0
        } else if ( c == '0' || c == '1' ) { // Если текущий символ - цифра
          number = num( c ); // Строим число из первой цифры
          sign = 1; // Если знак не указан - то считаем число положительным
          gc(); // Считываем следующий символ
          state = T1; // И переходим в состояние T1
        } else { // Иначе ошибка
          state = ERR;
        }
        break;
      case T0: // Состояние, когда еще ни одной цифры не считано
        if ( c == '0' || c == '1' ) { // Если текущий символ - цифра
          number = num( c ); // Строим число из первой цифры
          gc(); // Считываем следующий символ
          state = T1; // И переходим в состояние T1
        } else { // Иначе ошибка
          state = ERR;
        }
        break;
      case T1: // Считана одна цифра
        if ( c == '0' || c == '1' ) { // Если текущий символ - цифра
          number = number * 2 + num( c ); // Дополняем число
          gc();  // Считываем следующий символ
          state = T1; // И переходим в то же самое состояние (образуем цикл)
        } else if ( c == '$' ) { // Если считан конец цепочки
          number = number * sign; // Домножаем полученное число на знак
          state = FIN; // И переходим в завершающее состояние
        } else { // Иначе ошибка
          state = ERR;
        }
        break;
      default:
        state = ERR;
    };
  } while ( state != FIN && state != ERR ); // Цикл продолжается пока состояние не станет завершающим или ошибочным
  
  return state == FIN; // Если состояние завершающее, то цепочка принята
}

/*
  Основная программа, просто выводим результат проверки цепочки
*/
int main(int argc,char ** argv) {
  std::cout << check() << std::endl; // Выводим на экран 1 если строка корректна и 0 если некорректна
  std::cout << number << std::endl; // Выводим на экран число - результат преобразования
  
  return 0;
}