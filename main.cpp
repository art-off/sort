#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <fstream>
using namespace std;
using namespace std::chrono;
typedef std::chrono::high_resolution_clock::time_point TimeVar;





// ВНУТРЕННИЕ ФУНКЦИИ ---------------------------------------------------------------------- //
// функция, определяющия, можно ли преобразовать строку в число
bool isNumber(string line)
{
    char* p;
    strtol(line.c_str(), &p, 10);
    return *p == 0;
}

string getNumber(string message)
{
    string answer = "[undefined]";
    while (!isNumber(answer))
    {
        cout << message;
        cin >> answer;
        cout << endl;
    }
    return answer;
}
// ----------------------------------------------------------------------------------------- //

// СОЗДАНИЕ МАССИВА ------------------------------------------------------------------------ //
int* createArray(int size, int min, int max)
{
    if (min > max)
    {
        int tmp = min;
        min = max;
        max = tmp;
    }
    
    int *tmp = new int[size];
    
    for (int i=0; i < size; i++)
    tmp[i] = min + rand() % (max-min+1);
    
    return tmp;
}

int* createSortedArray(int size, int min, int max)
{
    int *tmp = createArray(size, min, max);
    sort(tmp, tmp+size);
    return tmp;
}

int* createReverseSortedArray(int size, int min, int max)
{
    int *tmp = createArray(size, min, max);
    sort(tmp, tmp+size);
    for (int i=0; i < size/2; i++)
        swap(tmp[i], tmp[size-i-1]);
    return tmp;
}
// ----------------------------------------------------------------------------------------- //

// ПОДСЧЕТ ПЕРЕСТАНОВОК И СРАВНЕНИЙ
int countOfPermutations;
int countOfComparisons;


// ФУНКЦИИ СОРТИРОВКИ ---------------------------------------------------------------------- //
void simpleMergingSort(string name)
{
    ifstream iF, iF1, iF2;
    ofstream oF, oF1, oF2;
    string a1, a2;
    int i, j, count, len;
    count = 0;
    
    
    iF.open(name);
    while (iF >> a1)
        count++;
    iF.close();
    
    len = 1;
    while (len < count)
    {
        // запись из основного в 2 вспомогательных файла ---- //
        iF.open(name);
        oF1.open("f1");
        oF2.open("f2");
        if (!iF.eof())
            iF >> a1;
        while (!iF.eof())
        {
            for (i = 0; i < len && !iF.eof() ; i++)
            {
                oF1 << a1 << " ";
                iF >> a1;
            }
            for (j = 0; j < len && !iF.eof() ; j++)
            {
                oF2 << a1 << " ";
                iF >> a1;
            }
        }
        iF.close();
        oF1.close();
        oF2.close();
        // -------------------------------------------------- //
        
        
        // запись из 2 вспомогательный в основной файл ------ //
        oF.open(name);
        iF1.open("f1");
        iF2.open("f2");
        
        if (!iF1.eof())
            iF1 >> a1;
        if (!iF2.eof())
            iF2 >> a2;
        
        while (!iF1.eof() && !iF2.eof())
        {
            i = 0;
            j = 0;
            
            while (i < len && j < len && !iF1.eof() && !iF2.eof())
            {
                if (stoi(a1) < stoi(a2))
                {
                    oF << a1 << " ";
                    iF1 >> a1;
                    i++;
                }
                else
                {
                    oF << a2 << " ";
                    iF2 >> a2;
                    j++;
                }
            }
            while (i < len && !iF1.eof())
            {
                oF << a1 << " ";
                iF1 >> a1;
                i++;
            }
            while (j < len && !iF2.eof())
            {
                oF << a2 << " ";
                iF2 >> a2;
                j++;
            }
        }
        while (!iF1.eof())
        {
            oF << a1 << " ";
            iF1 >> a1;
        }
        while (!iF2.eof())
        {
            oF << a2 << " ";
            iF2 >> a2;
        }
        
        iF2.close();
        iF1.close();
        oF.close();
        // -------------------------------------------------- //
        
        
        len *= 2;
    }
        remove("f1");
        remove("f2");
}


void naturalMergingSort(string fileName)
{
    string a1, a2;
    int mark;
    int s1 = 1, s2 = 1;
    ifstream iF, iF1, iF2;
    ofstream oF, oF1, oF2;
    
    while (s1>0 && s2>0)
    {
        mark = 1;
        s1 = 0;
        s2 = 0;
        
        // ЗАПИСЬ ВО ВТОРОСТЕПЕННЫЕ ФАЙЛЫ ПО СЕРИЯМ ------------ //
        iF.open(fileName);
        oF1.open("f1");
        oF2.open("f2");
        
        iF >> a1;
        if (!iF.eof())
        {
            oF1 << a1 << " ";
            iF >> a2;
        }
        
        while (!iF.eof())
        {
            if (stoi(a2) < stoi(a1))
            {
                if (mark == 1)
                {
                    oF1 << "' ";
                    mark = 2;
                    s1++;
                }
                else if (mark == 2)
                {
                    oF2 << "' ";
                    mark = 1;
                    s2++;
                }
            }
            
            if (mark == 1)
            {
                oF1 << a2 << " ";
                s1++;
            }
            else if (mark == 2)
            {
                oF2 << a2 << " ";
                s2++;
            }
            
            a1 = a2;
            iF >> a2;
        }
        
        if (s1>0 && mark==1) oF1 << "'";
        if (s2>0 && mark==2) oF2 << "'";
        
        iF.close();
        oF1.close();
        oF2.close();
        // ------------------------------------------------------- //
        
        
        // ЗАПИСЬ В ОСНОВНОЙ ФАЙЛ -------------------------------- //
        oF.open(fileName);
        iF1.open("f1");
        iF2.open("f2");
        
        a1 = a2 = "'";
        if (!iF1.eof()) iF1 >> a1;
        if (!iF2.eof()) iF2 >> a2;
        
        while (!iF1.eof() || !iF2.eof())
        {
            while (a1 != "'" && a2 != "'")
            {
                if (a1 <= a2)
                {
                    oF << a1 << " ";
                    iF1 >> a1;
                }
                else
                {
                    oF << a2 << " ";
                    iF2 >> a2;
                }
            }
            while (a1 != "'")
            {
                oF << a1 << " ";
                iF1 >> a1;
            }
            while (a2 != "'")
            {
                oF << a2 << " ";
                iF2 >> a2;
            }
            
            if (!iF1.eof())
                iF1 >> a1;
            if (!iF2.eof())
                iF2 >> a2;
        }
        
        oF.close();
        iF1.close();
        iF2.close();
        // ------------------------------------------------------ //
    }
    
    remove("f1");
    remove("f2");
}




bool allNotEmpty(int *array, int N)
{
    for (int i=0; i<N; i++)
        if (array[i] == 0)
            return false;
    return true;
}
bool allIsEmpty(ifstream *array, int N)
{
    for (int i=0; i<N; i++)
        if (!array[i].eof())
            return false;
    return true;
}
bool allIsEnd(string *array, int N)
{
    for (int i=0; i<N; i++)
        if (array[i] != "'")
            return false;
    return true;
}

void multithreadedMergingSortWith(int N, string fileName)
{
    string a[N];
    int s[N]; for (int i=0; i<N; i++) s[i] = 1;
    int mark;
    ifstream iF, iFT[N];
    ofstream oF, oFT[N];
    int indMin;
    
    while (allNotEmpty(s, N))
    {
        mark = 0;
        for (int i=0; i<N; i++) s[i] = 0;
        
        // ЗАПИСЬ В N ВТОРОСТЕПЕННЫХ ФАЙЛОВ ПО СЕРИЯМ ------------ //
        iF.open(fileName);
        for (int i=0; i<N; i++) oFT[i].open("f"+to_string(i));
        
        iF >> a[0];
        if (!iF.eof())
        {
            oFT[0] << a[0] << " ";
            iF >> a[1];
        }
        
        while (!iF.eof())
        {
            if (stoi(a[1]) < stoi(a[0]))
            {
                oFT[mark] << "' ";
                if (mark == N-1) mark = 0;
                else mark++;
                s[mark]++;
            }
            
            oFT[mark] << a[1] << " ";
            s[mark]++;
            
            a[0] = a[1];
            iF >> a[1];
        }
        
        for (int i=0; i<N; i++)
            if (s[i]>0 && mark==i)
                oFT[i] << "'";
        
        iF.close();
        for (int i=0; i<N; i++) oFT[i].close();
        // ------------------------------------------------------- //
        
        
        // ЗАПИСЬ В ОСНОВНОЙ ФАЙЛ -------------------------------- //
        oF.open(fileName);
        for (int i=0; i<N; i++) iFT[i].open("f"+to_string(i));
        
        for (int i=0; i<N; i++) a[i] = "'";
        for (int i=0; i<N; i++)
            if (!iFT[i].eof())
                iFT[i] >> a[i];
        
        while (!allIsEmpty(iFT, N))
        {
            while (!allIsEnd(a, N))
            {
                for (int i=0; i<N; i++)
                    if (isNumber(a[i]))
                    {
                        indMin = i;
                        break;
                    }
                
                for (int i=0; i<N; i++)
                    if (isNumber(a[i]))
                        if (stoi(a[i]) < stoi(a[indMin]))
                            indMin = i;
                oF << a[indMin] << " ";
                iFT[indMin] >> a[indMin];
            }
            
            for (int i=0; i<N; i++)
                if (!iFT[i].eof())
                    iFT[i] >> a[i];
        }
        
        oF.close();
        for (int i=0; i<N; i++) iFT[i].close();
        // ------------------------------------------------------ //
    }
    
    string file;
    for (int i=0; i<N; i++)
    {
        file = "f" + to_string(i);
        remove(file.c_str());
    }
}





// ----------------------------------------------------------------------------------------- //



int main()
{
    srand((unsigned)time(NULL));
    
    char* fileName = "array.txt";
    
    //simpleMergingSort(fileName);
    //naturalMergingSort(fileName);
    //multithreadedMergingSortWith(4, fileName);
}
