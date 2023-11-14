#include <iostream>
#include <fstream>
#include <string>
#include <functional>

#define M 11

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
    int m_Capacity; ///Размер массива хэштаблицы
    double m_LoadFactor = 0.75;
    hash<K> Hasher;

public:
    HashMap();
    ~HashMap();
    void Resize();///Функция для увеличения размера хеш-таблицы
    V ElementSeatch(const K& key);///Поиск элемента в коллекции по заданному ключу.
    void Insert(const K& key, const V& value); ///Добавление нового элемента с заданными ключом и значением в коллекцию.
    void remove(const K& key);/// Удаление из коллекции элемента с заданным ключом.
    void PrintMap();

    // Iterator class
    class Iterator {
    private:
        const HashMap& m_Map;
        int m_Index;
        Node* m_Indicator;

    public:
        Iterator(const HashMap& HashMap) : m_Map(HashMap), m_Index(0), m_Indicator(nullptr) {
            MoveToTheNext();
        }
        Iterator(const HashMap& HashMap, int Index, Node* Indicator) : m_Map(HashMap), m_Index(Index), m_Indicator(Indicator) {}

        bool HasNext() const {
            return m_Indicator != nullptr;
        }

        void Next() {
            m_Indicator = m_Indicator->m_Next;
            if (!m_Indicator) {
                m_Index++;
                MoveToTheNext();
            }
        }///ищем следующий непустой бакет

        const K& getKey() const {
            return m_Indicator->m_Key;
        }

        const V& getValue() const {
            return m_Indicator->m_Value;
        }
        Iterator& operator++() {
            Next();
            return *this;
        }

        void MoveToTheNext() {
            while (m_Index < m_Map.m_Capacity && m_Map.m_Table[m_Index] == nullptr) {
                m_Index++;
            }

            if (m_Index < m_Map.m_Capacity) {
                m_Indicator = m_Map.m_Table[m_Index];
            } else {
                m_Indicator = nullptr;
            }
        } ///ищем следующий непустой бакет
        };

    Iterator GetIterator() const ;

    Iterator Begin() const;
    Iterator End() const; ///последний бакет

    int getTotalSize() const; ///вывод количества элементов в мапе

    int getUniqueSize() const; ///вывод количества уникальных элементов

};

template <typename K, typename V>
HashMap<K, V>::HashMap() : m_Size(0), m_Capacity(M) {
    m_Table = new Node*[m_Capacity]();
}

template <typename K, typename V>
HashMap<K, V>::~HashMap(){
    for (int i = 0; i < m_Capacity; ++i) {
        Node* m_Indicator = m_Table[i];
        while (m_Indicator != nullptr) {
            Node* Next = m_Indicator->m_Next;
            delete m_Indicator;
            m_Indicator = Next;
        }
    }
    delete[] m_Table;
}

template <typename K, typename V>
void HashMap<K, V>::Resize() {
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

template <typename K, typename V>
V HashMap<K, V>::ElementSeatch(const K &key) {
    int index = Hasher(key)%m_Capacity;
    Node* Indicator = m_Table[index];

    while(Indicator != nullptr){
        if(Indicator->m_Key == key){
            return Indicator->m_Value;
        }
        Indicator = Indicator->m_Next;
    }
    ///что-то нужно возвращать, если такого ключа нет
}

template <typename K, typename V>
void HashMap<K, V>::Insert(const K &key, const V &value){ ///???Нужно ли m_Size преобразовать в double static_cast<double>
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

    if (prev != nullptr) {
        prev->m_Next = newNode;
    } else {
        m_Table[index] = newNode;
    }
    ++m_Size;
}

template <typename K, typename V>
void HashMap<K, V>::remove(const K &key) { ///удаления элемента из хеш-таблицы
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
            return;
        }
        prev = m_Indicator;
        m_Indicator = m_Indicator->m_Next;
    }
}

template <typename K, typename V>
void HashMap<K, V>::PrintMap() {
    Node* m_Indicator;
    for(int i = 0; i < m_Capacity; i++){
        cout << i << ": ";
        m_Indicator = m_Table[i];
        while(m_Indicator != nullptr){
            cout << m_Indicator->m_Value << " ";
            m_Indicator = m_Indicator->m_Next;
        }
        cout << endl;
    }

}

template<typename K, typename V>
HashMap<K, V>::Iterator HashMap<K, V>::Begin() const {
    int index = 0;
    while (index < m_Capacity && m_Table[index] == nullptr) {
        index++;
    }
    if (index < m_Capacity) {
        return Iterator(*this, index, m_Table[index]);
    } else {
        return End();
    }
} ///ищем первый не пустой бакет
template<typename K, typename V>

HashMap<K, V>::Iterator HashMap<K, V>::End() const {
    return Iterator(*this, m_Capacity, nullptr);
}

template <typename K, typename V>
int HashMap<K, V>::getTotalSize() const {
    return m_Size;
}

template <typename K, typename V>
int HashMap<K,V>::getUniqueSize() const {
    int count = 0;

    auto it = this->Begin();
    auto EndIt = this->End();
    V* ContainerValues = new V[m_Size];
    while (it.HasNext()) {
        V value =it.getValue();
        int flag = 1;
        for (int i = 0; i < count; ++i) {
            if(ContainerValues[i] == value){
                flag = 0;
            }
        }
        if(flag == 1){
            ContainerValues[count] = value;
            count++;
        }
        ++it;
    }
    delete[] ContainerValues;
    return count;
}

template<typename K, typename V>
HashMap<K, V>::Iterator HashMap<K, V>::GetIterator() const {
    return Iterator(*this);
}

int main() {
    system("chcp 65001"); // Русификация ввода/вывода
    ifstream F("input.txt"); //открываем файл "input.txt"
    ofstream Out("output.txt"); //открываем файл "output.txt"

    if(!F && !Out){
        cout << "Не удалось открыть файл"<< endl;
        return 1;
    }

    char keyType, valueType, comand;
    F >> keyType >> valueType;
    int N;
    F >> N;

    switch (keyType) {
        case ('I'): {
            int key;
            switch (valueType) {
                case 'I':{
                    int value;
                    HashMap<int, int> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;}
                case 'D':{
                    double value;
                    HashMap<int, double> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;
                }
                case 'S': {
                    string value;
                    HashMap<int, string> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;
                }
            }
            break;
        }
        case ('D'): {
            double key;
            switch (valueType) {
                case 'I':{
                    int value;
                    HashMap<double, int> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;}
                case 'D':{
                    double value;
                    HashMap<double, double> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;}
                case 'S': {
                    string value;
                    HashMap<double, string> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;
                }
            }
            break;
        }
        case ('S'): {
            string key;
            switch (valueType) {
                case 'I':{
                    int value;
                    HashMap<string, int> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;}
                case 'D':{
                    double value;
                    HashMap<string , double> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;
                }
                case 'S': {
                    string value;
                    HashMap<string, string> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;
                }
            }
            break;
        }
    }


    return 0;
}
