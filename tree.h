#pragma once
#include <array>
#include <ctime>

const int N = 9;// Размер поля
const int NROOT = 3;
const int win_comb[8][3] = {{0, 1, 2},
                            {3, 4, 5},
                            {6, 7, 8},
                            {0, 3, 6},
                            {1, 4, 7},
                            {2, 5, 8},
                            {0, 4, 8},
                            {2, 4, 6}};

inline int find_ind(const std::array<char, N> &arr, int ind = -1) // ищет индекс для выставления элемента
{
    for (int i = ind + 1; i < N; i++)
    {
        if (arr[i] == ' ') return i;
    }
    return -1;
}

inline char set_opposite_char(char ch)
{
    return ch == 'X' ? 'O' : 'X';
}

template<char turn, char opposite_turn>
class Tree_of_variations
{
private:
    class Node
    {
    public:

        int changed_items_index = -1; // в каком месте изменили доску
        int is_win = 0; // победный ли конкретный нод

        std::array<char, N> array;

        Node *pNext; //для перемещения по листьям одного уровня
        Node *pChild; // Для перехода на следующий ход
        Node *pParent;
        char current_turn;// текущий ход

        explicit Node(const std::array<char, N> &arr) : array(arr)
        {
            current_turn = opposite_turn; // тк следующий ход наш, то этот ход обязательно AI
            changed_items_index = -1;
            pChild = nullptr;
            pNext = nullptr;
            pParent = nullptr;

            is_win = check_win();
        }

        Node(const std::array<char, N> &arr, int ind, char prev_turn = 'p') : array(arr)
        {
            changed_items_index = ind;
            current_turn = set_opposite_char(prev_turn);

            array[changed_items_index] = current_turn; // замена по индексу собственно
            pChild = nullptr;
            pNext = nullptr;
            pParent = nullptr;
            // этот кусок кода помечает нод как выигрышный или проигрышный, или никакой

            is_win = check_win();
        }

        ~Node()
        {
            delete pNext;
            delete pChild;
            pChild = nullptr;
            pNext = nullptr;
            pParent = nullptr;
        }

        int check_win()
        {
            auto check_line = [this](int step, int situation = 3) -> bool
            {
                // 1) \ диагональ 2) / диагональ 3) | вертикаль 4) - горизонталь
                int counter = -1; //тк точка считается 2 раза при чеке

                int index = changed_items_index;
                // чекаем первую диагональ
                while (index >= 0)
                {
                    if (array[index] == current_turn) counter++;
                    else break;
                    if (situation == 1 || situation == 4)
                    {
                        if (index % NROOT == 0)//чек на краевую точку
                            break;
                    } else if (situation == 2)
                    {
                        if ((index + 1) % NROOT == 0)//чек на краевую точку
                            break;
                    }
                    index -= step;
                }
                index = changed_items_index;
                while (index < N)
                {
                    if (array[index] == current_turn) counter++;
                    else break;

                    if (situation == 1 || situation == 4)
                    {
                        if ((index + 1) % NROOT == 0)//чек на краевую точку
                            break;
                    } else if (situation == 2)
                    {
                        if (index % NROOT == 0)//чек на краевую точку
                            break;
                    }
                    index += step;
                }

                return (counter >= 3);
            };


//            int res = (check_line(1, 4) || check_line(4, 2) || check_line(5) || check_line(6, 1)); // для 5 на 5
//            int res = (check_line(1, 4) || check_line(3, 2) || check_line(4) || check_line(5, 1)); // для 4 на 4
            int res = (check_line(1, 4) || check_line(2, 2) || check_line(3) || check_line(4, 1));
            if (current_turn == turn && res)
                return 1;
            else if (current_turn == opposite_turn && res)
                return -1;
            else return 0;


        }


        int check_free() // считает количество оставшихся клеток
        {
            int counter = 0;
            for (char elem: array)
            {
                if (elem == ' ') counter++;
            }
            return counter;
        }


        std::array<int, 3> count_wins(std::array<int, 3> &ans, int counter) const
        {
            // ans[0] - loses,ans[1] - draw, ans[2] - wins
            if (is_win == 0 && (pChild != nullptr || pNext != nullptr)) // если ничья не в конце, то ничего не делаем
            {}
            else
            {
                ans[is_win + 1]++;
            }
            if (pChild)
            {
                pChild->count_wins(ans, counter);
            }
            if (pNext)
            {
                if (counter != pNext->check_free())
                {
                    pNext->count_wins(ans, counter);
                }
            }
            return ans;
        }

        [[nodiscard]] int calculate_points()
        {
            int score = 0;

            Node *cur_node;
            if (pChild)
                cur_node = pChild;
            else // если нет дальше никого, то считаем очки нода и возвращаем
            {
                if (current_turn == opposite_turn) // ход врага
                {
                    if (is_win == -1)
                        score = -10;
                } else // ход игрока
                {
                    if (is_win == 1)
                        score = 10;
                }
                return score;
            }

            // если есть дальше челики, то
            int res;
            score = (cur_node->current_turn == turn) ? -10 : 10; // чтобы было с чем сравнивать
            while (cur_node != nullptr)
            {
                res = cur_node->calculate_points();
                //                cur_node->points = res;
                if (res > score)
                    score = (cur_node->current_turn == opposite_turn) ? score
                                                                      : res; // если ход врага, а результат больше, то не
                // меняем
                else if (res < score)
                    score = (cur_node->current_turn == turn) ? score
                                                             : res; // если ход врага, а результат меньше, то меняем
                cur_node = cur_node->pNext;
            }

            return score;
        }

    };

    void create_brothers(Node *node, int ind)
    {
        std::array<char, N> outer = node->array;
        outer[node->changed_items_index] = ' ';

        node->pNext = new Node(outer, ind, set_opposite_char(node->current_turn)); // меняет ход на другой
        // чтобы в ноде все нормально отработало
        node->pNext->pParent = node;
        int new_ind_for_bro = find_ind(outer, node->pNext->changed_items_index);
        if (!node->pNext->is_win)// если не победный чел, то дальше ему ищем братьев и детей
        {
            if (new_ind_for_bro != -1)//если нашел пустое место после измененного
            {
                create_brothers(node->pNext, new_ind_for_bro);
            } else
            {
                create_child(node->pNext, find_ind(node->pNext->array));//находит индекс для ребенка первый
            }
        }
        int new_ind_for_child = find_ind(node->array);
        if (new_ind_for_child != -1)//если нашел пустое место еще, для дальнейшего хода
        {
            create_child(node, new_ind_for_child);
        }
    };

    void create_child(Node *node, int ind)
    {
        node->pChild = new Node(node->array, ind, node->current_turn);

        std::array<char, N> outer = node->pChild->array;
        outer[node->pChild->changed_items_index] = ' ';

        node->pChild->pParent = node;
        if (!node->pChild->is_win)
        {
            int new_ind_for_bro = find_ind(outer, node->pChild->changed_items_index);
            if (new_ind_for_bro != -1)//если нашел пустое место после измененного
            {
                create_brothers(node->pChild, new_ind_for_bro);
            }

            int new_ind_for_child = find_ind(node->array, node->pChild->changed_items_index);
            if (new_ind_for_child != -1)//если нашел пустое место еще, для дальнейшего хода
            {
                create_child(node->pChild, new_ind_for_child);
            }
        }
    };


public:
    Node *root;
    Node *cur_root;

    explicit Tree_of_variations(const std::array<char, N> &config)
    {
        root = new Node(config); // создали корень
        cur_root = root;

        int ind = find_ind(config); // нашли, какой индекс менять надо - его передадим в ребенка
        if (ind != -1)// То есть если индекс нашелся
        {
            create_child(root, ind); // создаем ребенка этого элемента, говорим, на каком индексе поставить Х\О
        }
    }

    ~Tree_of_variations()
    {
        delete root;
    }

    void print_tree(Node *node)
    {
        node->print_node();
        if (node->pChild) print_tree(node->pChild);
        if (node->pNext) print_tree(node->pNext);
    }

    int find_optimal(bool AI_first = true)
    {
        if (cur_root->pChild == nullptr)
        {
            return -1;
        }

        int index = -1;
        Node *cur_node = cur_root->pChild;
        if (AI_first)
        {
            int score = -100;


            while (cur_node != nullptr )
            {
                int node_points = cur_node->calculate_points();
                if (node_points > score || (node_points == score && rand() % 2 == 1))
                {
                    index = cur_node->changed_items_index;
                    score = node_points;
                }
                cur_node = cur_node->pNext;
            }
        } else
        {
            int score = 100;

            while (cur_node != nullptr )
            {
                int node_points = cur_node->calculate_points();
                if (node_points < score || (node_points == score && rand()%2 == 1))
                {
                    index = cur_node->changed_items_index;
                    score = node_points;
                }

                cur_node = cur_node->pNext;
            }
        }
        return index;
    }

    void update_root(std::array<char, N> &config)
    {
        if (cur_root->pChild == nullptr) return;
        Node *cur_node = cur_root->pChild;
        bool res;

        while (true)
        {
            for (int i = 0; i < N; i++)
            {
                if (cur_node->array[i] != config[i])
                {
                    res = false;
                    break;
                }
                else
                {
                    res = true;
                }
            }
            if (res)
            {
                cur_root = cur_node;
                return;
            }
            cur_node = cur_node->pNext;
        }
    }

    void restart()
    {
        cur_root = root;
    }

};
