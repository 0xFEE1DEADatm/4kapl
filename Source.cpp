#include <iostream>
#include <fstream>
#include <string>
#include"Queue_list.h"
//using TInfo = int;
//struct NODE
//{
//	TInfo info;
//	NODE* left, * right;
//	int count = 1;
//	NODE() {}
//	NODE(TInfo info, NODE* left = nullptr, NODE* right = nullptr) : info(info), left(left), right(right) {}
//	~NODE()
//	{
//		left = nullptr;
//		right = nullptr;
//	}
//};
//
//using Tree = NODE*;

Tree Build_Balans(std::ifstream& file, int count)
{
	Tree result = nullptr;
	if (count)
	{
		int count_left = count / 2;
		int count_right = count - count_left - 1;
		int x;
		file >> x;
		result = new NODE(x);
		result->left = Build_Balans(file, count_left);
		result->right = Build_Balans(file, count_right);
	}
	return result;
}
bool is_empty(Tree root)
{
	return root == nullptr;
}
void Clear(Tree& t)
{
	if (t)
	{
		Clear(t->left);
		Clear(t->right);
		delete t;
		t = nullptr;
	}
}

void Print(Tree t, int level)
{
	if (t)
	{
		Print(t->right, level + 1);
		for (int i = 0; i < level; i++)
			std::cout << "   ";
		std::cout << t->info << '\n';
		Print(t->left, level + 1);
	}
}

int count_even(Tree t)
{
	int result = 0;
	if (t)
		result = (t->info % 2 == 0 ? 1 : 0) + count_even(t->right) + count_even(t->left);
	return result;
}


void add(Tree& t, TInfo elem)
{
	if (!t)
		t = new NODE(elem);
	else
		if (elem < t->info)
			add(t->left, elem);
		else
			if (elem > t->info)
				add(t->right, elem);
			else
				t->count++;
}

Tree search(const Tree& root, TInfo elem)
{
	Tree t = root, ptr = nullptr;
	while (t && !ptr)
	{
		if (elem < t->info)
			t = t->left;
		else
			if (elem > t->info)
				t = t->right;
			else
				ptr = t;
	}
	return ptr;
}

Tree Build_Search(std::string file_name)
{
	std::ifstream file(file_name);
	Tree root = nullptr;
	int elem;
	while (file >> elem)
		add(root, elem);
	file.close();
	return root;
}

void print_order(Tree t)
{
	if (t)
	{
		print_order(t->left);
		for (int i = 1; i <= t->count; ++i)
			std::cout << t->info << '   ';
		print_order(t->right);
	}
}

void find_element(Tree& r, Tree& q)
{
	if (r->right)
		find_element(r->right, q);
	else
	{
		q->info = r->info;
		q->count = r->count;
		q = r;
		r = r->left;
	}
}

bool delete_element(Tree& t, TInfo elem)
{
	bool result = false;
	if (t)
	{
		if (elem < t->info)
			result = delete_element(t->left, elem);
		else
			if (elem > t->info)
				result = delete_element(t->right, elem);
			else
			{
				if (t->count > 1)
					t->count--;
				else
				{
					Tree q = t;
					if (!t->right)
						t = q->left;
					else
						if (!t->left)
							t = q->right;
						else
							find_element(q->left, q);
					delete q;
				}
				result = true;
			}
	}
	return result;
}

Tree Build_Formula(std::ifstream& file)
{
	char c = file.get();
	Tree result = new NODE(' ');
	if (c >= '0' && c <= '9')
		result->info = c;
	else
	{
		result->left = Build_Formula(file);
		result->info = file.get();
		result->right = Build_Formula(file);
		c = file.get();
	}
	return result;
}

std::string print_to_string(const Tree& t)
{
	std::string result = "";
	if (!t->left && !t->right)
		result += t->info;
	else
		result = '(' + print_to_string(t->left) + std::to_string(t->info) + print_to_string(t->right) + ')';
	return result;
}

void switch_pointers(Tree root, Tree& min, Tree& max, TInfo min_info, TInfo max_info)
{
	if (max == min)
	{
		Tree tmp = max->left;
		max->left = max->right;
		max->right = tmp;
	}
	else
	{
		if (min->left->info == min_info)
		{
			if (max->left->info == max_info)
			{
				Tree tmp = min->left;
				min->left = max->left;
				max->left = tmp;
			}
			else if (max->right->info == max_info)
			{
				Tree tmp = min->left;
				min->left = max->right;
				max->right = tmp;
			}
		}
		else if (min->right->info == min_info)
		{
			if (max->left->info == max_info)
			{
				Tree tmp = min->right;
				min->right = max->left;
				max->left = tmp;
			}
			else if (max->right->info == max_info)
			{
				Tree tmp = min->right;
				min->right = max->right;
				max->right = tmp;
			}
		}
	}
}

void min_max(Tree t, Tree p, Tree& min, Tree& max, TInfo& min_info, TInfo& max_info)
{
	if (!min || p->info < min_info)
	{
		min_info = p->info;
		min = t;
	}
	if (!max || p->info > max_info)
	{
		max_info = p->info;
		max = t;
	}
}

bool is_leaf(Tree p)
{
	if (p != nullptr && p->left == nullptr && p->right == nullptr)
		return true;
	return false;
}

void task1(Tree root, Tree& min, Tree& max, TInfo& min_info, TInfo& max_info)
{
	QUEUE q;
	q.Enqueue(root);
	while (!q.Empty())
	{
		if (root)
		{
			if (root->left)
			{
				if (is_leaf(root->left))
				{
					min_max(root, root->left, min, max, min_info, max_info);
				}
				else q.Enqueue(root->left);
			}

			if (root->right)
			{
				if (is_leaf(root->right))
				{
					min_max(root, root->right, min, max, min_info, max_info);
				}
				else q.Enqueue(root->right);
			}
			q.Dequeue();
		if (!q.Empty())
			root = q.Head();
		}
	}
}
		
int main()
{
	setlocale(0, "");
	std::ifstream file("C:/Users/Пользователь/Desktop/tree.txt");
	int count;
	file >> count;
	//std::cout << count << '\n';
	Tree root = Build_Balans(file, count);
	Print(root, 0);
	Tree min = nullptr, max = nullptr;
	TInfo min_info = 0, max_info = 0;
	task1(root, min, max, min_info, max_info);
	std::cout << "----------------------------------\n";
	std::cout << "Минимум:  " << min_info << "    Максимум:  " << max_info << '\n';
	std::cout << "----------------------------------";
	switch_pointers(root, min, max, min_info, max_info);
	std::cout << '\n';
	Print(root, 0);
	Clear(root);
	std::cin.get();
}
