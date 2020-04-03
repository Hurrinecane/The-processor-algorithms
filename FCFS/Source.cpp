#include <iostream>
using std::cout;
using std::cin;
using std::endl;

class Processor
{
private:
	struct Process
	{
		Process()
		{
			time = 0;
			enter_time = 0;
		};

		Process(int time_, int enter_time_)
		{
			time = time_;
			enter_time = enter_time_;
		};
		int time;
		int enter_time;
	};

	int numberOfProcesses = 0;
	int mas_length = 4;

	Process* process_mas = new Process[mas_length];

private:
	Process* resize(Process* process_mas, int OldSize, int NewSize)
	{
		if (OldSize > NewSize)
			return process_mas;
		Process* new_process_mas = new Process[NewSize];
		memcpy(new_process_mas, process_mas, sizeof(Process) * OldSize);
		delete[] process_mas;
		return new_process_mas;
	}

public:
	void AddProcess()
	{
		int time, enter_time;
		cout << "Процесс № " << numberOfProcesses + 1 << endl;
		do
		{
			cout << "Введите время выполнения процесса: ";
			cin >> time;

			if (time < 1) cout << "Некорректное время выполнения процесса!" << endl;
		} while (time < 1);

		if (numberOfProcesses > 0)
			do
			{
				cout << "Введите время вхождения: ";
				cin >> enter_time;
				if (enter_time <= process_mas[numberOfProcesses - 1].enter_time) cout << "Некорректное время вхождения процесса!" << endl;
			} while (enter_time <= process_mas[numberOfProcesses - 1].enter_time);
		else
			enter_time = 0;

		numberOfProcesses++;

		if (numberOfProcesses > mas_length)
		{
			process_mas = resize(process_mas, mas_length, numberOfProcesses);
			mas_length++;
		}
		process_mas[numberOfProcesses - 1].time = time;
		process_mas[numberOfProcesses - 1].enter_time = enter_time;
	};

	void PlanHPRN()
	{
		struct HPRN
		{
			bool done = 0;
			int wait_time = 0;
			int run_time = 0;
			double penalty_ratio = 1;
		}*hprn = new HPRN[numberOfProcesses];

		int total_time = 0;
		for (int i = 0; i < numberOfProcesses; i++)
			total_time += process_mas[i].time;

		int** processor_time = new int* [numberOfProcesses];
		for (int i = 0; i < numberOfProcesses; i++)
		{
			processor_time[i] = new int[total_time];
			for (int j = 0; j < total_time; j++)
				processor_time[i][j] = -1;
		}

		cout << endl
			<< "HPRN"
			<< endl
			<< "======================================================================================================================"
			<< endl
			<< "Process  	time	enter time" << "	";
		for (int i = 1; i <= total_time; i++)
			cout << i << " ";
		cout << endl;

		int time = 0;
		int run_process = 0;
		int process_count = 0;

		for (int i = 0; i < total_time; i++)
		{
			if (run_process == -1)
				for (int j = 0; j < numberOfProcesses; j++)
				{
					if (!hprn[j].done && i >= process_mas[j].enter_time)
					{
						hprn[j].penalty_ratio = (double(hprn[j].wait_time) + double(process_mas[j].time)) / process_mas[j].time;
						if (hprn[j].penalty_ratio > hprn[run_process].penalty_ratio)
							run_process = j;
					}
				}

			for (int j = 0; j < numberOfProcesses; j++)
			{
				if (hprn[j].done == false)
					if (j == run_process)
					{
						hprn[j].run_time++;
						processor_time[j][i] = 1;
						if (hprn[j].run_time == process_mas[j].time)
						{
							run_process = -1;
							hprn[j].done = true;
						}
					}
					else
						if (i >= process_mas[j].enter_time)
						{
							hprn[j].wait_time++;
							processor_time[j][i] = 0;
						}
			}
		}

		for (int i = 0; i < numberOfProcesses; i++)
		{
			cout << "Process " << i + 1 << "	" << process_mas[i].time << "	" << process_mas[i].enter_time << "		";
			int run_time = 0;
			for (int j = 0; j < total_time; j++)
			{
				if (processor_time[i][j] == 1)
				{
					cout << "* ";
					run_time++;
					if (int(log10(j + 1) + 1) == 2)
						cout << " ";
				}
				else if (processor_time[i][j] == 0)
				{
					cout << "# ";
					if (int(log10(j + 1) + 1) == 2)
						cout << " ";
				}
				else if (processor_time[i][j] == -1)
				{
					cout << "  ";
					if (int(log10(j + 1) + 1) == 2)
						cout << " ";
				}
				if (run_time == process_mas[i].time)
				{
					cout << endl;
					break;
				}
			}
		}
		cout << "======================================================================================================================" << endl;
		for (int i = 0; i < numberOfProcesses; i++)
			delete[] processor_time[i];
		delete[] hprn;
	}

	void PlanFCFS()
	{
		int total_time = 0;
		for (int i = 0; i < numberOfProcesses; i++)
			total_time += process_mas[i].time;

		int** processor_time = new int* [mas_length];
		for (int i = 0; i < mas_length; i++)
		{
			processor_time[i] = new int[total_time];
			for (int j = 0; j < total_time; j++)
				processor_time[i][j] = -1;
		}

		cout << endl
			<< "FCFS"
			<< endl
			<< "======================================================================================================================"
			<< endl
			<< "Process  	time	enter time" << "	";
		for (int i = 1; i <= total_time; i++)
			cout << i << " ";
		cout << endl;
		for (int i = 0; i < numberOfProcesses; i++)
		{
			cout << "Process " << i + 1 << "	" << process_mas[i].time << "	" << process_mas[i].enter_time << "		";
			int run_time = 0;
			for (int j = 1; run_time <= process_mas[i].time; j++)
			{
				if (j >= process_mas[i].enter_time + 1 && run_time < process_mas[i].time)
				{
					bool check = 0;
					for (int i = 0; i < numberOfProcesses; i++)
					{
						if (processor_time[i][j] == 1)
							check = 1;
					}
					if (!check)
					{
						processor_time[i][j] = 1; //процесс идет
						run_time++;
						cout << "* ";
						if (int(log10(j) + 1) == 2)
							cout << " ";

					}
					else
					{
						processor_time[i][j] = 0;
						cout << "# ";
						if (int(log10(j) + 1) == 2)
							cout << " ";
					}
				}
				if (processor_time[i][j] == -1)
				{
					cout << "  ";
					if (int(log10(j) + 1) == 2)
						cout << " ";
				}

				if (run_time == process_mas[i].time)
				{
					cout << endl;
					break;
				}
			}
		}
		cout << "======================================================================================================================" << endl;
		for (int i = 0; i < mas_length - 1; i++)
			delete[] processor_time[i];
	};
};

int main()
{
	setlocale(0, "");
	Processor CPU;
	CPU.AddProcess();
	CPU.AddProcess();
	CPU.AddProcess();
	CPU.AddProcess();
	CPU.AddProcess();
	system("cls");
	CPU.PlanHPRN();
	CPU.PlanFCFS();
	system("pause");
}
//1 2 1 3 2 4 3 5 4 6 5
//10 4 3 8 5 5 6