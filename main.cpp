#include <iostream>
#include <fstream>
#include <string>
#include <functional>

using namespace std;

template <typename K, typename V>
class HashMap{
private:
    struct Node{
        K m_Key;
        V m_Value;
        Node* m_Next;
        Node(const K& k, const V& v) : m_Key(k), m_Value(v), m_Next(nullptr) {}
    };
    
    Node** m_Table;
    int m_Size; /// количество элементов
    int m_Cou; ///счетчик уникальных элементов
    int m_Capacity; ///Размер массива хэштаблицы
    double m_LoadFactor = 0.75;
    hash<K> Hasher;

    void Resize() { ///Функция для увеличения размера хеш-таблицы
        int newCapacity = m_Capacity * 2;
        Node** m_NewTable = new Node*[newCapacity];
        for (int i = 0; i < m_Capacity; ++i) {
            Node* m_Indicator = m_Table[i]; ///указателем на узел в хеш-таблице
            while (m_Indicator != nullptr) {
                Node* m_Next = m_Indicator->m_Next;
                int index = Hasher(m_Indicator->m_Key) % newCapacity;
                m_Indicator->m_Next = m_NewTable[index];
                m_NewTable[index] = m_Indicator;
                m_Indicator = m_Next;
            }
        }

        delete[] m_Table;
        m_Table = m_NewTable;
        m_Capacity = newCapacity;
    }

public:
    HashMap() : m_Size(0), m_Cou(0), m_Capacity(31){
        m_Table = new Node*[m_Capacity]();
    }

    ~HashMap() {
        for (int i = 0; i < m_Capacity; ++i) {
            Node* m_Indicator = m_Table[i];
            while (m_Indicator != nullptr) {
                Node* next = m_Indicator->m_Next;
                delete m_Indicator;
                m_Indicator = next;
            }
        }
        delete[] m_Table;
    }

    V ElementSeatch(const K& key){
        int index = Hasher(key)/m_Capacity;
        Node* Indicator = m_Table[index];

        while(Indicator != nullptr){
            if(Indicator->m_Key == key){
                return Indicator->m_Value;
            }
            Indicator = Indicator->m_Next;
        }
        ///что-то нужно возвращать, если такого ключа нет
    }///Поиск элемента в коллекции по заданному ключу.

    void Insert(const K& key, const V& value) { ///???Нужно ли m_Size преобразовать в double static_cast<double>
        if (m_Size / m_Capacity >= m_LoadFactor) {
            Resize();
        }

        int index = Hasher(key) % m_Capacity; ///гарантируем,что индекс остаеётся в пределах допустимых границ
        Node* m_Indicator = m_Table[index]; ///указатель на начало цепочки
        Node* prev = nullptr; ///указатель на предыдущий элемент в списке

        while (m_Indicator != nullptr) {
            if (m_Indicator->m_Key == key) {///элемент с таким ключом key уже существует в хеш-таблице. => обновляем значение этого элемента
                m_Indicator->m_Value = value;
                return;
            }
            ///Двигаем указатели
            prev = m_Indicator;
            m_Indicator = m_Indicator->m_Next;
        }

        /// вставляет новый элемент в хеш-таблицу
        Node* newNode = new Node(key, value);
        ++m_Size;
        if (prev != nullptr) {
            prev->m_Next = newNode;
        } else {
            m_Table[index] = newNode;
        }
        ++m_Cou;
    } ///Добавление нового элемента с заданными ключом и значением в коллекцию.

    void remove(const K& key) { ///удаления элемента из хеш-таблицы
        int index = Hasher(key) % m_Capacity;
        Node* m_Indicator = m_Table[index];
        Node* prev = nullptr;

        while (m_Indicator != nullptr) {
            if (m_Indicator->m_Key == key) { ///Удаляем элемент
                if (prev != nullptr) {
                    prev->m_Next = m_Indicator->m_Next;
                } else {///обновляет указатель на начало цепочки
                    m_Table[index] = m_Indicator->m_Next;
                }
                delete m_Indicator;
                --m_Size;
                --m_Cou;
                return;
            }
            prev = m_Indicator;
            m_Indicator = m_Indicator->m_Next;
        }
    } /// Удаление из коллекции элемента с заданным ключом.

    int getTotalSize() const {
        return m_Size;
    }

    int getUniqueSize() const {///???
        return m_Cou;
    }

};


int main() {
    system("chcp 65001"); // Русификация ввода/вывода
    ifstream F("input.txt"); //открываем файл "input.txt"
    ofstream Out("output.txt"); //открываем файл "output.txt"

    if(!F && !Out){
        cout << "Не удалось открыть файл"<< endl;
        return 1;
    }

    char keyType, valueType;
    F >> keyType >> valueType;
    int N;
    F >> N;
    switch (keyType) {
        case "I":

    }
    
    Out << m_HasMap.getTotalSize() << " " << m_HasMap.getUniqueSize();
    return 0;
}
