#include <string>
#include "LineManager.h"
#include "Utilities.h"

LineManager::LineManager(std::string & fileName, std::vector<Task*>& tasks, std::vector<CustomerOrder>& custToBeFilled)
{
	std::fstream f(fileName);
	if (f.is_open())
	{
		//setup
		CustomerOrder_Count = custToBeFilled.size();
		for (int i = 0; i < custToBeFilled.size(); i++) //int works because vector has an overloaded [] opeartor. Otherwise use auto or iterator
		{
			ToBeFilled.push_back(std::move(custToBeFilled[i]));
		}

		/*****************
		Extraction process
		1-see which two items link to one another from txt file
		2-find the task that has item 1.
		3-Link the pNexttask with the task that has item 2
		4- Loop
		******************/

		//holders
		std::string current_line;
		std::string current_item;
		std::string next_item;

		//init util
		Utilities util;
		bool more = true;
		size_t position = 0;

		for (int i = 0; i < 5; i++) //TODO: Make 5 dynamic
		{
			std::getline(f, current_line, '\n'); //gets next line
			current_item = util.extractToken(current_line, position, more); //takes the line and extracts next token
			if (more == true)
			{
				next_item = util.extractToken(current_line, position, more); //TODO: Might throw error on reading last line
			
			for (auto& curr : AssemblyLine)
			{
				if (curr->getName() == current_item)
					current_item = "";
			}
			for (auto& firstTask : tasks)
			{
				if (current_item == firstTask->getName())
				{
					AssemblyLine.push_back(firstTask);
				}
			}
			for (auto& secondTask : tasks)
			{
				if (next_item == secondTask->getName())
					AssemblyLine.push_back(secondTask);
			}

			AssemblyLine[i]->setNextTask(*AssemblyLine[i+1]);
			}
		}

	}

	for (auto& i : AssemblyLine)
	{
		std::cout << i->getName() <<"\n";
	}

}

bool LineManager::Run(std::ostream & os)
{
	CustomerOrder temp;

	if (!ToBeFilled.empty())
	{
		*AssemblyLine[0] += std::move(ToBeFilled.back());
		ToBeFilled.pop_back();
	}
		//Running the process on each task (i.e only item that cust order wants and the task has is filled here)
		for (auto& currentTask : AssemblyLine)
		{
			currentTask->RunProcess(os);
		}
		//Moving task forward if item was sccessfully filled
		for (auto& currentTask : AssemblyLine)
		{
			currentTask->MoveTask();
		}
		//pushing out from assemblyline to Completed stack once the customer order has went throguh the entire assembly line
		if (AssemblyLine.back()->getCompleted(temp))
		{
			Completed.push_back(std::move(temp));
		}

		//If by some miricle this milestone is FINALLY DONE, we display here
		if (Completed.size() == CustomerOrder_Count)
		{
			for (auto& currCompletedOrder : Completed)
			{
				currCompletedOrder.Display(os);
			}
			return true;
		}

	return false;
}
