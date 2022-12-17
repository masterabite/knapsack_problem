#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <cmath>

using namespace std;

//функция получения следующего числа в строке
size_t next_ull(const string& str, size_t& pos) {

    size_t ans = 0;

    for (; pos < str.size() && !isdigit(str[pos]); ++pos);

    for (; pos < str.size() && isdigit(str[pos]); ++pos) {
        ans = ans * 10 + str[pos] - '0';
    }

    return ans;
}

bool has_next_ull(const string& str, size_t& pos) {
    size_t ans = 0;
    size_t my_pos = pos;
    for (; my_pos < str.size() && !isdigit(str[my_pos]); ++my_pos);
    return (my_pos < str.size());
}

//структура множества вещей
struct item_set {
    size_t weight, cost;            //общие вес и ценность
    set<int> number;                  //номера вещей для вывода, входящих во множество
    set<int> index;                   //индексы вещей в исходном массиве, входящих во множество

    item_set() {
        weight = 0;
        cost = 0;
        number = set<int>();
        index = set<int>();
    }

    item_set(size_t w, size_t c, size_t n, size_t i) {
        weight = w;
        cost = c;
        number.insert(n);
        index.insert(i);
    }

    long double unit_cost() {
        if (weight == 0) {
            return cost;
        }
        long double ans = 1;
        return ans * cost / weight;
    }

    //функция объединения двух множеств предметов
    void add(const item_set& other) {
        weight += other.weight;
        cost += other.cost;
        for (int n : other.number) {
            number.insert(n);
        }
        for (int i : other.index) {
            index.insert(i);
        }
    }

    //вывод множества в стандартный поток в формате ответа
    void print() {
        printf("%zu %zu\n", weight, cost);
        for (int n : number) {
            cout << n + 1 << '\n';
        }
    }

    //функция пытается считать информацию о предмете, возвращает true если считывание прошло успешно
    bool try_scan() {
        size_t pos = 0;
        string line;
        getline(cin, line);

        if (has_next_ull(line, pos)) weight = next_ull(line, pos);
        else return false;

        if (has_next_ull(line, pos)) cost = next_ull(line, pos);
        else return false;
        return true;
    }
};

//функция сравнения двух предметов (сравнение удельной ценности)
bool item_cmp(item_set a, item_set b) {
    return a.unit_cost() > b.unit_cost();
}

//решение задачи жадным алгоритмом
//используется для нахождение приблизительного ответа и аппроксимации
item_set backpack_greedy_algorithm(vector<item_set>& items, size_t max_weight) {
    item_set answer;
    sort(items.begin(), items.end(), item_cmp);
    for (item_set i : items) {
        if (answer.weight + i.weight > max_weight) {
            break;
        }
        answer.add(i);
    }
    return answer;
}

//функция решения задачи с помощью динамики на массиве ценностей
item_set backpack_arr_by_cost(vector<item_set>& items, size_t max_weight, size_t sum_cost) {

    //dp[i] - оптимальное множество предметоd для ценности i
    vector<item_set> dp = vector<item_set>(sum_cost + 1);
    dp[0].cost = 0;
    for (item_set i : items) {
        for (long long cost = sum_cost - i.cost; cost >= 0; --cost) {
            if (dp[cost].weight + i.weight > max_weight) { continue; }
            size_t next_cost = i.cost + cost;
            if ((dp[cost].cost > 0 || cost == 0) && (dp[next_cost].cost == 0 || dp[next_cost].weight > dp[cost].weight + i.weight)) {
                dp[next_cost] = dp[cost];
                dp[next_cost].add(i);
            }
        }
    }

    //поиск ответа
    item_set answer = dp[0];
    for (size_t cost = sum_cost; cost > 0; --cost) {
        if (dp[cost].cost > 0 && dp[cost].weight <= max_weight) {
            answer = dp[cost];
            break;
        }
    }

    return answer;
}

//решение задачи с динамикой построенной на массиве весов
item_set backpack_arr_by_weight(vector<item_set>& items, size_t max_weight, size_t sum_weight) {

    //dp[i] - оптимальное множество предметов для веса i
    size_t limit_weight = min(sum_weight, max_weight);

    vector<item_set> dp = vector<item_set>(limit_weight + 1);
    dp[0].cost = 0;

    for (item_set i : items) {
        for (size_t weight = limit_weight - i.weight; weight >= 0; --weight) {
            size_t next_weight = weight + i.weight;
            if ((dp[weight].cost >= 0 || weight == 0) && dp[next_weight].cost < dp[weight].cost + i.cost) {
                dp[next_weight] = dp[weight];
                dp[next_weight].add(i);
            }
            if (weight == 0) { break; }
        }
    }

    //поиск ответа
    item_set answer = dp[0];
    for (size_t weight = limit_weight; weight > 0; --weight) {
        if (dp[weight].cost > answer.cost) {
            answer = dp[weight];
        }
    }

    return answer;
}

//решение задачи в общем случае
item_set backpack(long double eps, vector<item_set> items, size_t max_weight) {
    if (items.size() == 0) {
        return item_set();
    }
    item_set approximate_answer = backpack_greedy_algorithm(items, max_weight);
    long double k = (eps)*approximate_answer.cost / items.size();

    if (k > 1) {
        for (item_set& i : items) {
            i.cost /= k;
        }
    }

    size_t sum_weight = 0, sum_cost = 0;
    for (item_set i : items) {
        sum_weight += i.weight;
        sum_cost += i.cost;
    }

    if (min(max_weight, sum_weight) > sum_cost) {
        //printf("By cost\n");
        return backpack_arr_by_cost(items, max_weight, sum_cost);
    }
    else {
        //printf("By weight\n");
        return backpack_arr_by_weight(items, max_weight, sum_weight);
    }
}

int main() {

    long double eps;
    size_t max_weight;

    vector<item_set> items; //массив вещей

    cin >> eps >> max_weight; //считываем точность и максимальную вместимость
    cin.get();

    int num = 0;
    item_set temp_item; //буферный предмет для считывания
    while (!cin.eof()) {
        if (temp_item.try_scan()) { //если получилось корректно считать информацию о предмете, добавляем его в массив
            if (temp_item.weight <= max_weight) {
                items.push_back(item_set(temp_item.weight, temp_item.cost, num, items.size()));
            }
            ++num;
        }
    }

    item_set answer = backpack(eps, items, max_weight);

    //пересчитываем аппроксимированный ответ
    answer.cost = 0;
    for (int i : answer.index) {
        answer.cost += items[i].cost;
    }
    answer.print();

    return 0;

}
