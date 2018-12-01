#include "LineManager.h"
#include "Utilities.h"

LineManager::LineManager(std::string & fileName, std::vector<Task*>& tasks, std::vector<CustomerOrder>& custToBeFilled)
{
	std::fstream f(fileName);
	if (f.is_open())
	{
		//setup
		CustomerOrder_Count = custToBeFilled.size();
		for (int i = 0; i < custToBeFilled.size(); i++) //int works because vector has an overloaded [] opeartor.
		{											   //Otherwise use auto or iterator
			ToBeFilled.push_front(std::move(custToBeFilled[i]));
		}

		/*****************
		Extraction process
		1-see which two items link to one another from txt file
		2-find the task that has item 1.
		3-push that task to the front of assemblyLine container
		4-Find the task that has item 2
		5-Push that task to the assemblyLine
		6-Link task with item1 to task with item2
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
		std::vector<Task*>::iterator TaskItr;

		for (int i = 0; i < 5; i++)
		{
			std::getline(f, current_line, '\n'); //gets next line
			current_item = util.extractToken(current_line, position, more); //takes the line and extracts next token
			if (more == true)
			{
				next_item = util.extractToken(current_line, position, more);

				for (auto& curr : AssemblyLine)
				{
					if (curr->getName() == current_item)
						current_item = "";
				}
				for (Task* firstTask : tasks)
				{
					if (current_item == firstTask->getName())
					{
						AssemblyLine.push_back(firstTask);
					}
				}
				for (Task* secondTask : tasks)
				{
					if (next_item == secondTask->getName())
						AssemblyLine.push_back(secondTask);
				}

				AssemblyLine[i]->setNextTask(*AssemblyLine[i + 1]);
			}
		}

	}
}

bool LineManager::Run(std::ostream & os)
{
	CustomerOrder temp;
	if (!ToBeFilled.empty())
	{
		*AssemblyLine[0] += std::move(ToBeFilled.back()); //Taking LAST element
		ToBeFilled.pop_back();
	}

	std::vector<Task*>::iterator TaskItr;
	//Running the process on each task (i.e only item that cust order wants and the task has is filled here)
	for (TaskItr = AssemblyLine.begin(); TaskItr != AssemblyLine.end(); TaskItr++)
	{
		(*TaskItr)->RunProcess(os); //iter->runProcess(os); does not work here because Assemblyline is holding Task* and not Task objects.
	}

	//Moving task forward if item was sccessfully filled
	for (Task* currentTask : AssemblyLine)
	{
		currentTask->MoveTask();
	}

	//pushing out from assemblyline to Completed stack once the customer order has went throguh the entire assembly line
	AssemblyLine.back()->RunProcess(os); //last task
	if (AssemblyLine.back()->getCompleted(temp))
	{
		Completed.push_back(std::move(temp));
	}

	//displaying all orders once they are completed
	if (Completed.size() == CustomerOrder_Count)
	{
		os << "\nCOMPLETED\n";
		for (auto& currCompletedOrder : Completed)
		{
			currCompletedOrder.Display(os);
		}
		return true;
	}
	return false;
}
