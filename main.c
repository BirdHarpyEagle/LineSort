#include <stdio.h>

/**
	\brief Максимальное количество симфолов, которое может быть в текстовом файле для сортировки.
*/
#define MAX_CHARACTERS_TO_SORT 400000

/**
	\brief Максимальное количество строк, которое может быть в текстовом файле для сортировки.
*/
#define MAX_LINES_TO_SORT 15000

/**
	\brief Возвращается функцией FilenameChecker(), если программе linesort переданно более 2 слов или
	вообще не передано.
*/
const int ERROR_ARGC = 1;

/**
	\brief Возвращается функцией FilenameChecker(), если программе linesort переданно 2 слова.
*/
const int OUTPUT_FILENAME_EXISTS = 2;

/**
	\brief Возвращается функцией FilenameChecker(), если программе linesort переданно 1 слово.
*/
const int OUTPUT_FILENAME_EMPTY = 3;

/**
	\brief Возвращается функцией FilenameChecker(), если она не найдет файл с именем,
	указанным в 1 слове.
*/
const int ERROR_FILENAME = 4;

/**
	\brief Возвращается функцией LineCompare(), если по алфавиту первая строка должна быть после второй.
*/
const int FIRST_BIGGER = 1;

/**
	\brief Возвращается функцией LineCompare(), если по алфавиту вторая строка должна быть после первой.
*/
const int SECOND_BIGGER = 2;


int FilenameChecker(int numberOfWords, char *words[]);

int FileCopy(FILE* pointerInputFile, char text[], char* index[]);

void FileSort(char* index[], int numberOfLines);

void LineSwap(char* index[], int firstLine, int secondLine);

int LineCompare(char* index[], int firstLine, int secondLine);

void FileOutput(FILE* pointerOutputFile, char* index[], int numberOfLines);

void HowToUse(void);


/**
    \brief Основные задачи:
    1) Запуск функции FilenameChecker() и обработка её возвращаемых значений;
    2) Открытие файлов ввода и вывода;
    3) Копирование входного файла в массив text[] и создание массива index[],
    элементы которого указывают на начала строк в text[];
    4) Сортировка элементов в index[] (указателей) для последующего вывода по ним;
    5) Запись в файл вывода;

    Массив text[], создаваемый в main(), служит только для хранения текста,
    т. е. не изменяется. Все последующие манипуляции проводятся только с массивом index[],
    содержащим указатели на начала строк в text[].

    Код функции:
    \code
    int main(int argc, char *argv[])
    {
        printf("Program name:\n\tLineSort\n");
        printf("Service:\n\tsorting lines alphabetically\n");

        char text[MAX_CHARACTERS_TO_SORT] = {};
        char *index[MAX_LINES_TO_SORT] = {};

        int numberOfLines = 0;

        int checkerResult = 0;
        checkerResult = FilenameChecker(argc, argv);

        if(checkerResult == ERROR_ARGC)
            return 1;


        FILE *pointerInputFile = fopen(argv[1], "r");

        FILE *pointerOutputFile;


        if(checkerResult == OUTPUT_FILENAME_EXISTS)
            pointerOutputFile = fopen(argv[2], "w");

        if(checkerResult == OUTPUT_FILENAME_EMPTY)
            pointerOutputFile = fopen("text.out", "w");

        numberOfLines = FileCopy(pointerInputFile, text, index);


        FileSort(index, numberOfLines);

        FileOutput(pointerOutputFile, index, numberOfLines);
    }
    \endcode
*/
int main(int argc, char *argv[])
{
    printf("Program name:\n\tLineSort\n");
    printf("Service:\n\tsorting lines alphabetically\n");

    char text[MAX_CHARACTERS_TO_SORT] = {};
    char *index[MAX_LINES_TO_SORT] = {};

    int numberOfLines = 0;

    int checkerResult = 0;
    checkerResult = FilenameChecker(argc, argv);

    if(checkerResult == ERROR_ARGC)
        return 1;


    FILE *pointerInputFile = fopen(argv[1], "r");

    FILE *pointerOutputFile;


    if(checkerResult == OUTPUT_FILENAME_EXISTS)
        pointerOutputFile = fopen(argv[2], "w");
    
    if(checkerResult == OUTPUT_FILENAME_EMPTY)
        pointerOutputFile = fopen("text.out", "w");

    numberOfLines = FileCopy(pointerInputFile, text, index);

    // printf("%c", *(text));
    FileSort(index, numberOfLines);

    FileOutput(pointerOutputFile, index, numberOfLines);
}



/**
    \brief
    Проверяет количество введенных слов при запуске программы. Если имя выходного файла
    не будет задано, то создастся файл text.out и результат запишется в него.
    Возвращает ERROR_ARGC, ERROR_FILENAME, OUTPUT_FILENAME_EMPTY и OUTPUT_FILENAME_EXISTS
    в зависимости от количества переданных слов и существования файлов с такими именами.

    Код функции:
    \code
    int FilenameChecker(int numberOfWords, char *words[])
    {
        if(!(numberOfWords >= 2 && numberOfWords <= 3))
        {
            printf("Error(FilenameChecker):\n\tWrong number of input words.\n");
            HowToUse();
            return ERROR_ARGC;
        }

        FILE *pointerInputFile;

        if((pointerInputFile = fopen(*(words + 1), "r")) == NULL)
        {
            printf("Error(FilenameChecker):\n\tWrong name of input file.\n\tProbably, it doesn`t exist\n");
            HowToUse();
            return ERROR_FILENAME;
        }

        fclose(pointerInputFile);


        if(numberOfWords == 3)
            return OUTPUT_FILENAME_EXISTS;
        else // numberOfWords == 2
            return OUTPUT_FILENAME_EMPTY;
    }
    \endcode
*/
int FilenameChecker(int numberOfWords, char *words[])
{

    if(!(numberOfWords >= 2 && numberOfWords <= 3))
    {
        printf("Error(FilenameChecker):\n\tWrong number of input words.\n");
        HowToUse();
        return ERROR_ARGC;
    }

    FILE *pointerInputFile;

    if((pointerInputFile = fopen(*(words + 1), "r")) == NULL)
    {
        printf("Error(FilenameChecker):\n\tWrong name of input file.\n\tProbably, it doesn`t exist\n");
        HowToUse();
        return ERROR_FILENAME;
    }

    fclose(pointerInputFile);


    if(numberOfWords == 3)
        return OUTPUT_FILENAME_EXISTS;
    else // numberOfWords == 2
        return OUTPUT_FILENAME_EMPTY;
}



/**
    \brief
    Проверяет количество введенных слов при запуске программы. Если имя выходного файла
    не будет задано, то создастся файл text.out и результат запишется в него.
    Также она закрывает входной файл. Возвращает количество строк в файле.

    Код функции:
    \code
    int FileCopy(FILE* pointerInputFile, char text[], char* index[])
    {
        int textCounter = 0;
        int indexCounter = 1;

        int numberOfLines = 0;

        int character = 0;
        *(index) = text;
        for(textCounter = 0; (character = getc(pointerInputFile)) != EOF; textCounter++)
        {
            *(text + textCounter) = (char) character;

            if(character == '\n')
            {
                textCounter++;
                *(text + textCounter) = '\0';
                *(index + indexCounter++) = text + textCounter + 1;
            }
        }

        fclose(pointerInputFile);

        numberOfLines = indexCounter;

        return numberOfLines;
    }
    \endcode
*/
int FileCopy(FILE* pointerInputFile, char text[], char* index[])
{
    int textCounter = 0;
    int indexCounter = 1;

    int numberOfLines = 0;

    int character = 0;
    *(index) = text;
    for(textCounter = 0; (character = getc(pointerInputFile)) != EOF; textCounter++)
    {
        *(text + textCounter) = (char) character;

        if(character == '\n')
        {
            textCounter++;
            *(text + textCounter) = '\0';
            *(index + indexCounter++) = text + textCounter + 1;
        }
    }

    fclose(pointerInputFile);

    numberOfLines = indexCounter;

    return numberOfLines;
}


/**
    \brief
    Сортирует элементы в массиве index[] так, что если последовательно обращаться к ним,
    как к адресам, то в результате получим входной текст, но с отсортированными строками.
    Вызывает вспомогательные функции LineCompare() и LineSwap().

    Код функции:
    \code
    void FileSort(char* index[], int numberOfLines)
    {
        int lineCounter = 0;
        int circleCounter = 0;

        for(circleCounter = 0; circleCounter < numberOfLines - 1; circleCounter++)
            for(lineCounter = 0; lineCounter < numberOfLines - 1; lineCounter++)
                if( LineCompare(index, lineCounter, lineCounter + 1) == FIRST_BIGGER )
                    LineSwap(index, lineCounter, lineCounter + 1);
    }
    \endcode
*/
void FileSort(char* index[], int numberOfLines)
{
    int lineCounter = 0;
    int circleCounter = 0;

    for(circleCounter = 0; circleCounter < numberOfLines - 1; circleCounter++)
        for(lineCounter = 0; lineCounter < numberOfLines - 1; lineCounter++)
            if( LineCompare(index, lineCounter, lineCounter + 1) == FIRST_BIGGER )
                LineSwap(index, lineCounter, lineCounter + 1);
}


/**
    \brief
    Меняет местами значения элементов массива index[] под номерами, переданными функции. 

    Код функции:
    \code
    void LineSwap(char* index[], int firstLine, int secondLine)
    {
        char* tempLine = 0;
        tempLine = *(index + firstLine);
        *(index + firstLine) = *(index + secondLine);
        *(index + secondLine) = tempLine;
    }
    \endcode
*/
void LineSwap(char* index[], int firstLine, int secondLine)
{
    char* tempLine = 0;
    tempLine = *(index + firstLine);
    *(index + firstLine) = *(index + secondLine);
    *(index + secondLine) = tempLine;
}

/**
    \brief
    Посимвольно сравнивает строки. Возвращает FIRST_BIGGER или SECOND_BIGGER в зависимости
    от того, какая старше по алфавиту.

    Код функции:
    \code
    int LineCompare(char* index[], int firstLine, int secondLine)
    {
        int firstCounter = 0;
        int secondCounter = 0;

        char firstCharacter = 0;
        char secondCharacter = 0;

        int compareResult = FIRST_BIGGER;
        while( ( (firstCharacter = *(*(index + firstLine) + firstCounter) ) != '\0') &&
            ( (secondCharacter = *(*(index + secondLine) + secondCounter) ) != '\0') )
        {

            if(firstCharacter >= 'a' && firstCharacter <= 'z')
                firstCharacter -= ('a' - 'A');

            if(secondCharacter >= 'a' && secondCharacter <= 'z')
                secondCharacter -= ('a' - 'A');

            if( !((firstCharacter >= 'a') && (firstCharacter <= 'z') ||
                  (firstCharacter >= 'A') && (firstCharacter <= 'Z') ||
                  (firstCharacter == '\n') ) )
            {
                firstCounter++;
                continue;
            }

            if( !((secondCharacter >= 'a') && (secondCharacter <= 'z') ||
                  (secondCharacter >= 'A') && (secondCharacter <= 'Z') ||
                  (secondCharacter == '\n') ) )
            {
                secondCounter++;
                continue;
            }

            if(firstCharacter == secondCharacter)
            {
                firstCounter++;
                secondCounter++;
                continue;
            }

            if(firstCharacter > secondCharacter)
            {
                compareResult = FIRST_BIGGER;
                break;
            }

            if(firstCharacter < secondCharacter)
            {
                compareResult = SECOND_BIGGER;
                break;
            }
        }

        return compareResult;
    }
    \endcode
*/
int LineCompare(char* index[], int firstLine, int secondLine)
{
    int firstCounter = 0;
    int secondCounter = 0;

    char firstCharacter = 0;
    char secondCharacter = 0;

    int compareResult = FIRST_BIGGER;
    while( ( (firstCharacter = *(*(index + firstLine) + firstCounter) ) != '\0') &&
        ( (secondCharacter = *(*(index + secondLine) + secondCounter) ) != '\0') )
    {

        if(firstCharacter >= 'a' && firstCharacter <= 'z')
            firstCharacter -= ('a' - 'A');
        
        if(secondCharacter >= 'a' && secondCharacter <= 'z')
            secondCharacter -= ('a' - 'A');

        if( !((firstCharacter >= 'a') && (firstCharacter <= 'z') ||
              (firstCharacter >= 'A') && (firstCharacter <= 'Z') ||
              (firstCharacter == '\n') ) )
        {
            firstCounter++;
            continue;
        }

        if( !((secondCharacter >= 'a') && (secondCharacter <= 'z') ||
              (secondCharacter >= 'A') && (secondCharacter <= 'Z') ||
              (secondCharacter == '\n') ) )
        {
            secondCounter++;
            continue;
        }

        if(firstCharacter == secondCharacter)
        {
            firstCounter++;
            secondCounter++;
            continue;
        }

        if(firstCharacter > secondCharacter)
        {
            compareResult = FIRST_BIGGER;
            break;
        }

        if(firstCharacter < secondCharacter)
        {
            compareResult = SECOND_BIGGER;
            break;
        }
    }

    return compareResult;
}




/**
    \brief
    Выводит отсортированный текст и закрывает выходной файл.

    Код функции:
    \code
    void FileOutput(FILE* pointerOutputFile, char* index[], int numberOfLines)
    {
        int indexCounter = 0;
        for(indexCounter = 0; indexCounter < numberOfLines; indexCounter++)
            fprintf(pointerOutputFile, "%s", *(index + indexCounter) );

        fclose(pointerOutputFile);
    }
    \endcode
*/
void FileOutput(FILE* pointerOutputFile, char* index[], int numberOfLines)
{
    int indexCounter = 0;
    for(indexCounter = 0; indexCounter < numberOfLines; indexCounter++)
        fprintf(pointerOutputFile, "%s", *(index + indexCounter) );

    fclose(pointerOutputFile);
}

/**
    \brief
    Выводит краткую информацию о том, как пользоваться программой.

    Код функции:
    \code
    void HowToUse(void)
    {
        printf("Usage:\n\t./progname fileNameToSort fileNameToOutput\n");
    }
    \endcode
*/
void HowToUse(void)
{
    printf("Usage:\n\t./progname fileNameToSort [fileNameToOutput]\n");
    printf("Without fileNameToOutput, program writes result into text.out\n");
}
