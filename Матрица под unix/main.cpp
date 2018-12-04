// Входные данные - файл с названием "data.txt" в пределах видимости испольняемого файла
// файл содержит матрицу с одинаковым кол. элементов в строчках, элементы разделены пробелами, в конце строчки - перенос.

#include <iostream>
#include <fstream>

using namespace std;
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ матрица вероятностей
class Matrix
{
private:
    void GetData();
    void Add(const double);
    void OutputNMatrixInCSV();
protected:
    typedef struct NMatrix
    {
        size_t id;
        double value;
        struct NMatrix *next;
    }NMatrix;
    NMatrix *head;
    size_t NSize;
    size_t NSize_str;
public:
    Matrix():head(0),NSize(0),NSize_str(1){GetData(); OutputNMatrixInCSV();};
    ~Matrix();
    void ShowNMarix();
};
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ матрица связи
class CouplingMatrix : public Matrix
{
private:
    size_t **CMatrix; //сама матрица связи
    double **fantom; //преображенные данные из структуры в матрицу для удобства обработки
    double EndSum; //сумма вероятностей
    double SumStolb(size_t, size_t);
    void DeleteStrAndStolb(size_t, size_t);
public:
    CouplingMatrix():Matrix(),EndSum(0){};
    ~CouplingMatrix();
    void Generation();
    void OutputCMatrixInCSV();
    void ShowCMatrix();
    void ShowEndSum();
};
////////////////////////////////описание методов класса matrix////////////////////////////////////
Matrix::~Matrix()
{
    NMatrix *tmp = head;
    NMatrix *temp = nullptr;

    while (tmp != nullptr)
    {
        temp = tmp;
        tmp = tmp -> next;
        delete temp;
    }

    head = nullptr;
    NSize = 0;
    NSize_str = 0;
}

void Matrix::ShowNMarix()
{
    NMatrix *tmp = head;
    for (size_t i = 0; i < NSize; ++i)
    {
        for (size_t j = 0; j < NSize_str; ++j)
        {
            cout << tmp -> value << " ";
            tmp = tmp -> next;
        }
        i+=NSize_str;
        cout << endl;
    }
}

void Matrix::Add(const double val)
{
    NMatrix *tmp = new NMatrix;
    NMatrix *temp = head;

    tmp -> value = val;
    tmp -> next = nullptr;

    if (head == nullptr)
    {
        head = tmp;
        tmp -> id = 1;
    }
    else
    {
        while (temp -> next != nullptr)
        {
            temp = temp -> next;
        }
        tmp -> id = temp -> id + 1;
        temp -> next = tmp;
    }
}

void Matrix::GetData()
{
    ifstream in("data.txt");
    if (in.is_open())
    {
		double temp = 0;
		char symbol;

		while (!in.eof())
		{
			in.get(symbol);
			if (symbol == ' ') NSize_str++;
			if (symbol == '\n') break;
        }

        in.seekg(0, ios::beg);
		in.clear();

        while (!in.eof())
		{
			in >> temp;
			Add(temp);
			NSize++;
		}

		in.close();
    }
	else
	{
		cout << "File data.txt not found";
	}
}

void Matrix::OutputNMatrixInCSV()
{
    ofstream out("matrix_in.csv");
    NMatrix *tmp = head;

    for (size_t i = 0; i < NSize; ++i)
    {
        for (size_t j = 0; j < NSize_str; ++j)
        {
            out << tmp -> value << ",";
            tmp = tmp -> next;
        }
        i+=NSize_str;
        out << endl;
    }
    out.close();
}
////////////////////////////////описание методов класса couplingmatrix////////////////////////////////////
CouplingMatrix::~CouplingMatrix()
{
    for (int i = 0; i < NSize/NSize_str; i++)
    {
        delete []CMatrix[i];
    }
    delete []CMatrix;
}
void CouplingMatrix::Generation()
{
    size_t t = NSize/NSize_str;
    size_t m = NSize_str;
    //
    size_t NCicle = 0;
    //
    CMatrix = new size_t *[t];
    for (size_t i = 0; i < t; ++i)
    {
        CMatrix[i] = new size_t [2];
    }
    //
    fantom = new double *[t];
    for (size_t i = 0; i < t; ++i)
    {
        fantom[i] = new double[m];
    }
    //
    NMatrix *tmp = head;
    for (size_t i = 0; i < t; ++i)
    {
        for (size_t j = 0; j < m; ++j)
        {
            fantom[i][j] = tmp -> value;
            tmp = tmp -> next;
        }
    }
    //
    double maxval;
    size_t strmax, stolmax;
    size_t errors;
    bool error[m];
    //
    for(;;)
    {
        /*
       for (size_t i = 0; i < t; ++i)
       {
           for (size_t j = 0; j < m; ++j)
           {
               cout << fantom[i][j] << " ";
           }
           cout << endl;
       }
       cout << "======NCicle | endsum " << NCicle << " " << EndSum << endl;
    */
        maxval = -1;
        strmax = -1;
        stolmax = -1;

        for (size_t i = 0; i < m; ++i)
        {
            error[i] = false;
        }
        //поиск максимального числа
        for (size_t i = 0; i < t; ++i)
        {
            for (size_t j = 0; j < m; ++j)
            {
                if (fantom[i][j] == -1) continue;
                if (fantom[i][j] > maxval) {maxval = fantom[i][j]; strmax = i; stolmax = j;};
            }
        }
        //поиск одинаковых значений в одной строке и добавление в массив номер столбца, где лежит такой же
        errors = 0;
        for (size_t i = 0; i < m; ++i)
        {
            if (fantom[strmax][i] == -1) continue;
            if (i == stolmax) continue;
            if (fantom[strmax][i] == maxval) {errors++; error[i] = true;};
        }
        //если совпадения есть - просуммировать эл каждого столбца, где лежит копия макс. элемента. Найдем с наим. суммой
        if (errors > 0)
        {
            double MinSizeStolb = SumStolb(stolmax, NSize/NSize_str);
            size_t NomerMinStolb = stolmax;

            for (size_t i = 0; i < NSize_str; ++i)
            {
                if (error[i] == true)
                {
                    double q = SumStolb(i,NSize/NSize_str);
                    if (q < MinSizeStolb) {MinSizeStolb = q; NomerMinStolb = i;};
                    error[i] = false;
                }
            }
            stolmax = NomerMinStolb;
        }

        EndSum += fantom[strmax][stolmax];
        CMatrix[NCicle][0] = strmax+1;
        CMatrix[NCicle][1] = stolmax+1;
        NCicle++;
        if (NCicle == t) break;

        DeleteStrAndStolb(strmax, stolmax);
    }
}

double CouplingMatrix::SumStolb(size_t nomerstolba, size_t sizestolba)
{
    double sum = 0;
    for (size_t i = 0; i < sizestolba; ++i)
    {
        if (fantom[i][nomerstolba] != -1) sum += fantom[i][nomerstolba];
    }
    return sum;
}

void CouplingMatrix::DeleteStrAndStolb(size_t str, size_t stb)
{
    for (size_t i = 0; i < NSize_str; ++i)
    {
        if (fantom[str][i] == -1) continue;
        fantom[str][i] = -1;
    }

    for (size_t i = 0; i < NSize/NSize_str; ++i)
    {
        if (fantom[i][stb] == -1) continue;
        fantom[i][stb] = -1;
    }
}

void CouplingMatrix::OutputCMatrixInCSV()
{
    ofstream out("matrix_out.csv");

    for (size_t i = 0; i < NSize/NSize_str; ++i)
    {
        for (size_t j = 0; j < 2; ++j)
        {
            out << CMatrix[i][j] << ",";
        }
        out << endl;
    }
    out.close();
}

void CouplingMatrix::ShowCMatrix()
{
    for (size_t i = 0; i < NSize/NSize_str; ++i)
    {
        for (size_t j = 0; j < 2; ++j)
        {
            cout << CMatrix[i][j];
            cout << " ";
        }
        cout << endl;
    }
}

void CouplingMatrix::ShowEndSum()
{
    cout << EndSum;
}
////////////////////////////////////////
int main()
{
    CouplingMatrix m;
    m.ShowNMarix();
    m.Generation();
    m.OutputCMatrixInCSV();
    m.ShowCMatrix();
    m.ShowEndSum();
}
