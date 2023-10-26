#include "main.h"

class imp_res : public Restaurant
{
public:
	imp_res(){};
	int num_curr = 0;	   // num of customers in the table (circular doubly linked list)
	int num_come = 0;	   // num of customers in the coming row (double linked list)
	int num_wait = 0;	   // num of customers in waiting queue (double linked list)
	customer *X = nullptr; // the first node  for the table (circular doubly linked list)
	customer *waitHead = nullptr, *waitTail = nullptr;
	customer *comeHead = nullptr, *comeTail = nullptr;
	bool checkDuplicated(string name)
	{
		if (num_curr == 0 && num_wait == 0)
		{
			return true;
		}

		if (X->name == name)
		{
			return false;
		}

		customer *index = X->next;
		while (index != X && index != nullptr)
		{
			if (index->name == name)
			{
				return false;
			}
			index = index->next;
		}
		index = waitHead;
		while (index != nullptr)
		{
			if (index->name == name)
			{
				return false;
			}
			index = index->next;
		}
		return true;
	}

	void insert(customer *&Customer)
	{

		// INSERT WHEN number of customer in the table < MAXSIZE/2

		if (num_curr < MAXSIZE / 2)
		{
			// Create the customer in the coming row

			customer *cus = new customer(Customer->name, Customer->energy, nullptr, nullptr);
			if (num_come == 0)
			{
				comeHead = cus;
				comeTail = cus;
				num_come++;
			}
			else if (num_come > 0)
			{
				comeTail->next = cus;
				cus->prev = comeTail;
				comeTail = cus;
				num_come++;
			}

			// Create the customer in the table

			if (num_curr == 0)
			{
				X = Customer;
				X->next = X;
				X->prev = X;
				num_curr++;
			}
			else if (num_curr == 1)
			{
				X->next = Customer;
				X->prev = Customer;
				Customer->next = X;
				Customer->prev = X;
				X = Customer;
				num_curr++;
			}

			else if (Customer->energy >= X->energy)
			{
				X->next->prev = Customer;
				Customer->next = X->next;
				X->next = Customer;
				Customer->prev = X;
				X = Customer;
				num_curr++;
			}

			else
			{
				X->prev->next = Customer;
				Customer->prev = X->prev;
				X->prev = Customer;
				Customer->next = X;
				X = Customer;
				num_curr++;
			}
		}
		// INSERT WHEN number of customer in the table >= MAXSIZE/2
		else if (num_curr >= MAXSIZE / 2 && num_curr < MAXSIZE)
		{
			int max_res = abs(Customer->energy - X->energy);
			int res = Customer->energy - X->energy;

			customer *index = X->next;
			customer *temp = X;

			while (index != X)
			{
				int different = abs(Customer->energy - X->energy);
				if (different > max_res)
				{
					max_res = different;
					res = Customer->energy - X->energy;
					temp = index;
				}
				index = index->next;
			}
			X = temp;
			if (res < 0)
			{
				X->prev->next = Customer;
				Customer->prev = X->prev;
				X->prev = Customer;
				Customer->next = X;
				X = Customer;
				num_curr++;
			}
			else
			{
				X->next->prev = Customer;
				Customer->next = X->next;
				X->next = Customer;
				Customer->prev = X;
				X = Customer;
				num_curr++;
			}
		}
		// INSERT TO THE WAITING QUEUSE

		else
		{
			if (num_wait == MAXSIZE)
			{
				delete Customer;
			}

			else if (num_wait == 0)
			{
				waitHead = Customer;
				waitTail = Customer;
				num_wait++;
			}
			else
			{
				waitTail->next = Customer;
				Customer->prev = waitTail;
				waitTail = Customer;
				num_wait++;
			}
		}
	}

	void RED(string name, int energy)
	{
		// cout << name << " " << energy << endl;
		if (energy != 0 || checkDuplicated(name) == true)
		{
			customer *cus = new customer(name, energy, nullptr, nullptr);
			insert(cus);
		}
		// cout << MAXSIZE << endl;
	}

	void remove()
	{
		if (num_curr == 1)
		{
			X->next = nullptr;
			X->prev = nullptr;
			delete X;
			comeHead->next = nullptr;
			comeHead->prev = nullptr;
			delete comeHead;
			num_curr = 0;
			num_come = 0;
		}
		else if (num_curr > 1)
		{
			customer *temp = comeHead;
			comeHead = comeHead->next;
			customer *index = X->next;
			while (index != X)
			{
				if (index->name == temp->name)
				{
					if (temp->energy > 0)
						X = index->next;
					else
						X = index->prev;
					index->next->prev = index->prev;
					index->prev->next = index->next;
					index->next = index->prev = nullptr;
					delete index;
					num_curr--;
					comeHead->prev = nullptr;
					temp->next = nullptr;
					temp->prev = nullptr;
					delete temp;
					num_come--;
					return;
				}
				index = index->next;
			}

			if (temp->energy > 0)
				X = index->next;
			else
				X = index->prev;
			index->next->prev = index->prev;
			index->prev->next = index->next;
			index->next = nullptr;
			index->prev = nullptr;
			delete index;
			num_curr--;
			comeHead->prev = nullptr;
			temp->next = temp->prev = nullptr;
			num_come--;
			delete temp;
		}
	}

	void BLUE(int num)
	{
		// cout << "blue "<< num << endl;
		int temp1 = num;
		while (num != 0)
		{
			remove();
			num--;
		}
		num = temp1;

		while (waitHead != nullptr && num != 0)
		{
			customer *temp = waitHead;
			waitHead = waitHead->next;
			if (waitHead != nullptr)
			{
				waitHead->prev = nullptr;
			}
			else
			{
				waitTail = waitTail->next;
			}
			temp->next = nullptr;
			insert(temp);
			num--;
			num_wait--;
		}
	}

	int shell_sort(int index)
	{
		int N = 0;
		if (index == 0)
			return 0;
		for (int i = (index + 1) / 2; i > 2; i /= 2)
			for (int j = 0; j < i; j++)
				N += insort(j, index + 1, i);
		N += insort(0, (index + 1), 1);
		return N;
	}

	int insort(int first, int n, int incr)
	{
		int N = 0;
		for (int i = first; i < n - incr; i += incr)
			for (int j = i + incr; j >= incr; j -= incr)
			{
				if (abs(moveTo(j)->energy) > abs(moveTo(j - incr)->energy))
				{
					swap(j, j - incr);
					N++;
				}
			}
		return N;
	}

	void swap(int i, int j)
	{
		int etmp = moveTo(i)->energy;
		string ntmp = moveTo(i)->name;
		moveTo(i)->energy = moveTo(j)->energy;
		moveTo(i)->name = moveTo(j)->name;
		moveTo(j)->energy = etmp;
		moveTo(j)->name = ntmp;
	}

	customer *moveTo(int index)
	{
		int i = 0;
		customer *curr = waitHead;
		while (i != index)
		{
			curr = curr->next;
			i++;
		}
		return curr;
	}

		void PURPLE()
	{
		// cout << "purple"<< endl;
		int index = 0;
		int max_value = 0;
		int i = 0;
		int N = 0;
		customer *curr = waitHead;
		while (curr != NULL)
		{
			if (abs(curr->energy) >= max_value)
			{
				index = i;
				max_value = abs(curr->energy);
			}
			i++;
			curr = curr->next;
		}
		N = shell_sort(index);
		BLUE(N % MAXSIZE);
	}


	void swap(customer *&x, customer *&y)
	{
		int etmp = x->energy;
		string ntmp = x->name;
		x->energy = y->energy;
		x->name = y->name;
		y->energy = etmp;
		y->name = ntmp;
	}

	void reverse_pos()
	{
		customer *it = X;
		customer *head = X, *tail = X->next;
		if (head->energy > 0 && tail->energy > 0 && num_curr == 2)
		{
			X = tail;
			return;
		}
		if (num_curr == 2)
			return;
		while (head != tail)
		{
			while (tail->energy < 0 && tail != head)
				tail = tail->next;
			while (head->energy < 0 && head != tail)
				head = head->prev;
			if (head == tail)
				break;
			swap(head, tail);
			head = head->prev;
			tail = tail->next;
			if (head->next == tail)
				break;
		}
	}

	void reverse_neg()
	{
		customer *it = X;
		customer *head = X, *tail = X->next;
		if (head->energy < 0 && tail->energy < 0 && num_curr == 2)
		{
			X = tail;
			return;
		}
		if (num_curr == 2)
			return;
		while (head != tail)
		{
			while (tail->energy > 0 && tail != head)
				tail = tail->next;
			while (head->energy > 0 && head != tail)
				head = head->prev;
			if (head == tail)
				break;
			swap(head, tail);
			head = head->prev;
			tail = tail->next;
			if (head->next == tail)
				break;
		}
	}

	void REVERSAL()
	{
		// cout << "reversal" << endl;
		if (num_curr == 1 || num_curr == 0)
			return;
		string Xname = X->name;
		int etmp = X->energy;
		string ntmp = X->name;
		reverse_pos();
		reverse_neg();
		customer *curr = X->next;
		while (curr != X)
		{
			if (curr->name == Xname)
			{
				X = curr;
				break;
			}
			curr = curr->next;
		}
	}


	void print(customer *c)
	{
		cout << c->name << "-" << c->energy << endl;
	}
	void MinSubstring()
	{
		customer *start = X;
		customer *temp = X;
		customer *end = X->next->next->next;
		int stringMin = X->energy + X->next->energy + X->next->next->energy + X->next->next->next->energy;
		while (start->next != X)
		{
			int sum = start->energy;
			int lenght = 1;
			customer *a = start->next;
			while (a != start)
			{
				sum = sum + a->energy;
				lenght++;
				if (sum <= stringMin && lenght >= 4)
				{
					stringMin = sum;
					temp = start;
					end = a;
				}
				a = a->next;
			}
			start = start->next;
		}

		int sum = start->energy;
		int lenght = 1;
		customer *a = start->next;
		while (a != start)
		{
			sum = sum + a->energy;
			lenght++;
			if (sum <= stringMin && lenght >= 4)
			{
				stringMin = sum;
				temp = start;
				end = a;
			}
			a = a->next;
		}

		start = temp;
		int energyMin = temp->energy;
		customer *positionMin = start;
		customer *c = start;
		while (c != end)
		{
			if (c->energy < energyMin)
			{
				energyMin = start->energy;
				positionMin = start;
			}
			c = c->next;
		}
		if (end->energy < energyMin)
		{
			energyMin = end->energy;
			positionMin = end;
		}

		print(positionMin);
		customer *i = positionMin->next;
		while (i != end)
		{
			print(i);
			i = i->next;
		}
		if (positionMin != end)
		{
			print(end);
		}

		customer *j = start;
		while (j != positionMin)
		{
			print(j);
			j = j->next;
		}
	}

	void UNLIMITED_VOID()
	{
		// cout << "unlimited_void" << endl;
		if (num_curr >= 4)
		{
			MinSubstring();
		}
	}
	void DOMAIN_EXPANSION()
	{
		// cout << "domain_expansion" << endl;
		if (num_curr == 0 || num_curr == 1)
			return;
		// printW();
		int pos = 0, neg = 0;
		customer *curr = X;
		if (curr->energy > 0)
			pos = curr->energy;
		else
			neg = abs(curr->energy);
		curr = curr->next;
		while (curr != X)
		{
			if (curr->energy > 0)
				pos += curr->energy;
			else
				neg += abs(curr->energy);
			curr = curr->next;
		}
		curr = waitHead;
		while (curr != NULL)
		{
			if (curr->energy > 0)
				pos += curr->energy;
			else
				neg += abs(curr->energy);
			curr = curr->next;
		}
		if (pos >= neg)
			removeNeg();
		else
			removePos();
		while (num_curr < MAXSIZE && num_wait != 0)
		{
			customer *tmp = waitHead;
			waitHead = waitHead->next;
			if (waitHead != NULL)
				waitHead->prev = nullptr;
			else
			{
				waitTail = nullptr;
			}
			tmp->next = nullptr;
			insert(tmp);
			num_wait--;
		}
	}

	void removePos()
	{
		customer *curr = X->next, *it = X;
		while (curr != it)
		{
			if (curr->energy > 0)
			{
				customer *tmp = curr;
				X = curr->next;
				curr->next->prev = curr->prev;
				curr->prev->next = curr->next;
				curr = curr->next;
				tmp->next = nullptr;
				tmp->prev = nullptr;
				delete tmp;
				num_curr--;
			}
			else
				curr = curr->next;
		}
		if (it->energy > 0)
		{
			X = it->next;
			it->next->prev = it->prev;
			it->prev->next = it->next;
			it->next = nullptr;
			it->prev = nullptr;
			delete it;
			num_curr--;
		}

		curr = waitTail;
		while (curr != NULL && num_wait > 0)
		{
			if (curr->energy > 0)
			{
				if (num_wait == 1)
				{
					print(curr);
					waitHead = waitTail = nullptr;
					num_wait = 0;
					break;
				}
				if (curr == waitHead)
				{
					waitHead = waitHead->next;
					customer *tmp = curr;
					curr = nullptr;
					print(tmp);
					tmp->next = tmp->prev = nullptr;
					waitHead->prev = nullptr;
					delete tmp;
					num_wait--;
				}
				else if (curr == waitTail)
				{
					waitTail = waitTail->prev;
					customer *tmp = curr;
					curr = curr->prev;
					print(tmp);
					waitTail->next = nullptr;
					tmp->prev = tmp->next = nullptr;
					delete tmp;
					num_wait--;
				}
				else
				{
					customer *tmp = curr;
					curr->next->prev = curr->prev;
					curr->prev->next = curr->next;
					curr = curr->prev;
					print(tmp);
					tmp->next = nullptr;
					tmp->prev = nullptr;
					delete tmp;
					num_wait--;
				}
			}
			else
				curr = curr->prev;
		}

		curr = comeTail;
		while (curr != NULL && num_come > 0)
		{
			if (curr->energy > 0)
			{
				if (num_come == 1)
				{
					print(curr);
					comeHead = comeTail = nullptr;
					num_come = 0;
					break;
				}
				if (curr == comeHead)
				{
					comeHead = comeHead->next;
					customer *tmp = curr;
					curr = nullptr;
					print(tmp);
					tmp->next = tmp->prev = nullptr;
					comeHead->prev = nullptr;
					delete tmp;
					num_come--;
				}
				else if (curr == comeTail)
				{
					comeTail = comeTail->prev;
					customer *tmp = curr;
					curr = curr->prev;
					print(tmp);
					comeTail->next = nullptr;
					tmp->prev = tmp->next = nullptr;
					delete tmp;
					num_come--;
				}
				else
				{
					customer *tmp = curr;
					curr->next->prev = curr->prev;
					curr->prev->next = curr->next;
					curr = curr->prev;
					print(tmp);
					tmp->next = nullptr;
					tmp->prev = nullptr;
					delete tmp;
					num_come--;
				}
			}
			else
				curr = curr->prev;
		}
	}

	void removeNeg()
	{
		customer *curr = X->next, *it = X;
		while (curr != it && curr != NULL)
		{
			if (curr->energy < 0)
			{
				customer *tmp = curr;
				X = curr->prev;
				curr->next->prev = curr->prev;
				curr->prev->next = curr->next;
				curr = curr->next;
				tmp->next = nullptr;
				tmp->prev = nullptr;
				delete tmp;
				num_curr--;
			}
			else
				curr = curr->next;
		}
		if (it->energy < 0)
		{
			customer *tmp = it;
			X = it->prev;
			curr->next->prev = curr->prev;
			curr->prev->next = curr->next;
			tmp->next = nullptr;
			tmp->prev = nullptr;
			delete tmp;
			num_curr--;
		}

		curr = waitTail;
		while (curr != NULL && num_wait > 0)
		{
			if (curr->energy < 0)
			{
				if (num_wait == 1)
				{
					print(curr);
					waitHead = waitTail = nullptr;
					num_wait = 0;
					break;
				}
				if (curr == waitHead)
				{
					waitHead = waitHead->next;
					customer *tmp = curr;
					curr = nullptr;
					print(tmp);
					tmp->next = tmp->prev = nullptr;
					waitHead->prev = nullptr;
					delete tmp;
					num_wait--;
				}
				else if (curr == waitTail)
				{
					waitTail = waitTail->prev;
					customer *tmp = curr;
					curr = curr->prev;
					print(tmp);
					waitTail->next = nullptr;
					tmp->prev = tmp->next = nullptr;
					delete tmp;
					num_wait--;
				}
				else
				{
					customer *tmp = curr;
					curr->next->prev = curr->prev;
					curr->prev->next = curr->next;
					curr = curr->prev;
					print(tmp);
					tmp->next = nullptr;
					tmp->prev = nullptr;
					delete tmp;
					num_wait--;
				}
			}
			else
				curr = curr->prev;
		}

		curr = comeTail;
		while (curr != NULL && num_come > 0)
		{
			if (curr->energy < 0)
			{
				if (num_come == 1)
				{
					print(curr);
					comeHead = comeTail = nullptr;
					num_come = 0;
					break;
				}
				if (curr == comeHead)
				{
					comeHead = comeHead->next;
					customer *tmp = curr;
					curr = nullptr;
					print(tmp);
					tmp->next = tmp->prev = nullptr;
					comeHead->prev = nullptr;
					delete tmp;
					num_come--;
				}
				else if (curr == comeTail)
				{
					comeTail = comeTail->prev;
					customer *tmp = curr;
					curr = curr->prev;
					print(tmp);
					comeTail->next = nullptr;
					tmp->prev = tmp->next = nullptr;
					delete tmp;
					num_come--;
				}
				else
				{
					customer *tmp = curr;
					curr->next->prev = curr->prev;
					curr->prev->next = curr->next;
					curr = curr->prev;
					print(tmp);
					tmp->next = nullptr;
					tmp->prev = nullptr;
					delete tmp;
					num_come--;
				}
			}
			else
				curr = curr->prev;
		}
	}
	void LIGHT(int num)
	{
		// cout << "light " << num << endl; //cout<<count_come;
		if (num_curr == 0)
			return;
		if (num == 0)
			printW();
		else if (num > 0)
			printPos();
		else
			printNeg();
	}

	void printW()
	{
		if (num_wait == 0)
			return;
		customer *curr = waitHead;
		while (curr != waitTail)
		{
			print(curr);
			curr = curr->next;
		}
		print(waitTail);
	}

	void printC()
	{
		if (num_come == 0)
			return;
		customer *curr = comeHead;
		while (curr != comeTail)
		{
			print(curr);
			curr = curr->next;
		}
		print(comeTail);
	}

	void printPos()
	{
		customer *curr = X;
		print(curr);
		curr = curr->next;
		while (curr != X && curr->next != NULL)
		{
			print(curr);
			curr = curr->next;
		}
	}

	void printNeg()
	{
		customer *curr = X;
		print(curr);
		curr = curr->prev;
		while (curr != X && curr->prev != NULL)
		{
			print(curr);
			curr = curr->prev;
		}
	}
	~imp_res()
	{
		while (waitHead != waitTail)
		{
			customer *tmp = waitHead;
			waitHead = waitHead->next;
			tmp->next = nullptr;
			delete tmp;
		}
		delete waitHead;
		num_wait = 0;

		while (comeHead != comeTail)
		{
			customer *tmp = comeHead;
			comeHead = comeHead->next;
			tmp->next = nullptr;
			delete tmp;
		}
		delete comeHead;
		num_come = 0;

		while (num_curr != 0)
		{
			customer *tmp = X;
			X = X->next;
			tmp->next = tmp->prev = nullptr;
			delete tmp;
			num_curr--;
		}
	}
};